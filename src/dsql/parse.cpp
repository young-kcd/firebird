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



#define	YYFINAL		2027
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
    2353,  2356,  2361,  2362,  2372,  2378,  2380,  2382,  2390,  2395,
    2397,  2399,  2408,  2414,  2417,  2422,  2423,  2428,  2430,  2434,
    2438,  2440,  2448,  2456,  2462,  2466,  2467,  2469,  2471,  2473,
    2476,  2477,  2479,  2481,  2483,  2484,  2488,  2490,  2494,  2496,
    2497,  2501,  2503,  2507,  2509,  2513,  2517,  2519,  2521,  2525,
    2527,  2530,  2532,  2536,  2540,  2542,  2545,  2549,  2553,  2557,
    2561,  2563,  2567,  2570,  2572,  2574,  2576,  2578,  2580,  2582,
    2584,  2586,  2588,  2590,  2592,  2596,  2600,  2604,  2608,  2612,
    2616,  2620,  2624,  2631,  2638,  2645,  2652,  2659,  2666,  2673,
    2680,  2687,  2694,  2701,  2708,  2715,  2722,  2729,  2736,  2738,
    2740,  2746,  2753,  2759,  2766,  2770,  2775,  2781,  2788,  2792,
    2797,  2801,  2806,  2810,  2815,  2820,  2826,  2831,  2836,  2840,
    2845,  2847,  2849,  2851,  2853,  2855,  2857,  2859,  2863,  2867,
    2869,  2871,  2873,  2875,  2877,  2879,  2881,  2883,  2885,  2887,
    2890,  2893,  2897,  2901,  2905,  2909,  2913,  2917,  2921,  2925,
    2927,  2929,  2931,  2933,  2937,  2939,  2941,  2943,  2945,  2947,
    2949,  2954,  2956,  2960,  2962,  2965,  2967,  2969,  2971,  2973,
    2975,  2977,  2979,  2982,  2985,  2988,  2990,  2992,  2994,  2996,
    2998,  3000,  3002,  3004,  3006,  3008,  3011,  3013,  3016,  3018,
    3020,  3022,  3024,  3026,  3029,  3031,  3033,  3035,  3037,  3042,
    3048,  3054,  3060,  3066,  3072,  3078,  3084,  3090,  3096,  3102,
    3104,  3106,  3113,  3115,  3117,  3119,  3124,  3129,  3134,  3139,
    3141,  3143,  3148,  3153,  3161,  3164,  3165,  3173,  3180,  3187,
    3192,  3194,  3196,  3198,  3203,  3207,  3214,  3216,  3218,  3225,
    3234,  3241,  3243,  3245,  3250,  3257,  3262,  3268,  3272,  3278,
    3283,  3289,  3291,  3293,  3295,  3300,  3307,  3309,  3311,  3313,
    3315,  3317,  3319,  3321,  3323,  3325,  3326,  3328,  3329,  3331,
    3333,  3335,  3337,  3339,  3341,  3343,  3345,  3347,  3349,  3351,
    3353,  3355,  3357,  3359,  3361,  3363,  3365,  3367,  3369,  3371,
    3373,  3375,  3377,  3379,  3381,  3383,  3385,  3387,  3389,  3391,
    3393,  3395,  3397,  3399,  3401,  3403,  3405,  3407,  3409,  3411,
    3413,  3415,  3417,  3419,  3421,  3423,  3425,  3427,  3429,  3431,
    3433,  3435,  3437,  3439,  3441,  3443,  3445
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
       0,   489,   491,   208,   490,   469,     0,   488,     0,   750,
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
     153,   538,     0,   193,     0,     0,   147,   540,     0,     0,
     162,     0,     0,   156,   175,   542,     0,   543,     0,     0,
     544,     0,   543,   544,     0,   545,     0,   546,     0,   547,
       0,   551,     0,   143,   123,     0,   143,   194,     0,   188,
       0,   117,   188,     0,    84,   102,   548,     0,   548,     0,
     549,     0,   143,   177,   550,     0,   143,    26,   550,     0,
     162,     0,   162,   172,     0,   162,   172,   166,     0,   186,
       0,   117,   186,     0,     0,   146,   554,     0,   158,     0,
     142,     0,     0,   143,     0,   194,     0,   555,     0,   554,
     287,   555,     0,   557,   556,     0,    61,   552,   553,     0,
       0,   605,     0,   557,   287,   605,     0,   156,   168,    79,
     737,     0,   269,   122,   560,    83,   564,     0,   269,   122,
     561,   740,    83,   564,     0,   269,   122,   562,   740,   563,
      83,   564,     0,    37,     0,    48,     0,   172,     0,   187,
       0,   141,     0,   176,     0,    59,    65,     0,    60,     0,
      55,     0,    68,     0,   265,     0,    79,     0,   206,     0,
      21,   156,     0,   268,     0,   207,     0,   133,     0,   293,
     740,     0,   687,     0,    94,     0,   569,   566,   568,     0,
      61,   180,   567,     0,     0,   121,   649,     0,     0,   192,
     243,     0,     0,   572,   622,   628,     0,   572,   622,   628,
       0,   572,   622,   628,     0,   573,     0,   572,   178,   723,
     573,     0,   572,   178,     6,   573,     0,   574,     0,   155,
     579,   582,   583,   587,   612,   608,   611,   613,     0,     0,
       0,     0,     0,   580,   581,   576,     0,   580,   576,     0,
     581,     0,     0,   231,   694,   575,     0,   231,   288,   676,
     289,   575,     0,   231,   683,   575,     0,   232,   694,     0,
     232,   288,   576,   676,   289,     0,   232,   683,     0,    46,
       0,   722,     0,   584,     0,   284,     0,   585,     0,   584,
     287,   585,     0,   676,     0,   676,   586,   738,     0,    10,
       0,     0,    63,   588,     0,   589,     0,   588,   287,   589,
       0,   595,     0,   590,     0,   602,     0,   591,     0,   288,
     595,   289,     0,   288,   569,   289,   586,   592,   593,     0,
     743,     0,     0,   288,   594,   289,     0,     0,   738,     0,
     594,   287,   738,     0,   596,     0,   597,     0,   598,     0,
     589,   263,    85,   590,     0,   589,   114,   606,    85,   590,
       0,   589,   606,    85,   589,   599,     0,   600,     0,   601,
       0,   122,   656,     0,   239,   288,   649,   289,     0,   741,
     603,   586,   743,     0,   741,   603,     0,   288,   679,   289,
       0,     0,   605,     0,   744,   743,     0,   744,     0,    77,
       0,   100,   607,     0,   151,   607,     0,    64,   607,     0,
       0,   127,     0,     0,    71,    19,   609,     0,     0,   610,
       0,   609,   287,   610,     0,   676,     0,    73,   656,     0,
       0,   190,   656,     0,     0,   135,   614,     0,     0,   615,
     288,   616,   289,     0,    85,     0,   164,   108,     0,   108,
       0,   164,     0,     0,   617,     0,   617,   287,   616,     0,
     618,   619,     0,   614,     0,   744,     0,   744,   618,     0,
     114,     0,    79,   288,   620,   289,     0,   126,   737,   621,
       0,   737,     0,   737,   287,   620,     0,    79,   288,   620,
     289,     0,     0,   126,    19,   623,     0,     0,   624,     0,
     623,   287,   624,     0,   676,   625,   627,     0,    11,     0,
      45,     0,     0,   231,     0,   241,     0,   240,   577,   626,
     578,     0,     0,   259,   676,     0,   259,   676,   174,   676,
       0,     0,    80,    82,   605,   650,   182,   288,   679,   289,
     636,     0,    80,    82,   605,   650,   569,     0,   631,     0,
     632,     0,    44,    63,   604,   612,   613,   622,   628,     0,
      44,    63,   604,   637,     0,   634,     0,   635,     0,   180,
     604,   156,   638,   612,   613,   622,   628,     0,   180,   604,
     156,   638,   637,     0,   278,   649,     0,   278,   649,    82,
     443,     0,     0,   190,    35,   121,   731,     0,   639,     0,
     638,   287,   639,     0,   655,   279,   676,     0,   708,     0,
     143,    18,   654,    63,   605,   642,   645,     0,    80,    18,
     654,    82,   605,   642,   645,     0,    60,    63,   643,   174,
     643,     0,    60,   174,   643,     0,     0,   644,     0,   683,
       0,   688,     0,   107,   646,     0,     0,   692,     0,   683,
       0,   648,     0,     0,   288,   649,   289,     0,   654,     0,
     649,   287,   654,     0,   651,     0,     0,   288,   652,   289,
       0,   655,     0,   652,   287,   655,     0,   654,     0,   743,
     293,   729,     0,   743,   293,   284,     0,   729,     0,   654,
       0,   743,   293,   729,     0,   672,     0,   118,   672,     0,
     658,     0,   656,   125,   656,     0,   656,     8,   656,     0,
     658,     0,   118,   672,     0,   657,   125,   656,     0,   657,
       8,   656,     0,   673,   125,   656,     0,   673,     8,   656,
       0,   659,     0,   288,   657,   289,     0,   118,   658,     0,
     660,     0,   663,     0,   664,     0,   665,     0,   666,     0,
     671,     0,   661,     0,   669,     0,   667,     0,   668,     0,
     670,     0,   676,   279,   676,     0,   676,   280,   676,     0,
     676,   281,   676,     0,   676,    67,   676,     0,   676,   101,
     676,     0,   676,   119,   676,     0,   676,   120,   676,     0,
     676,   116,   676,     0,   676,   279,     6,   288,   570,   289,
       0,   676,   280,     6,   288,   570,   289,     0,   676,   281,
       6,   288,   570,   289,     0,   676,    67,     6,   288,   570,
     289,     0,   676,   101,     6,   288,   570,   289,     0,   676,
     119,     6,   288,   570,   289,     0,   676,   120,     6,   288,
     570,   289,     0,   676,   116,     6,   288,   570,   289,     0,
     676,   279,   662,   288,   570,   289,     0,   676,   280,   662,
     288,   570,   289,     0,   676,   281,   662,   288,   570,   289,
       0,   676,    67,   662,   288,   570,   289,     0,   676,   101,
     662,   288,   570,   289,     0,   676,   119,   662,   288,   570,
     289,     0,   676,   120,   662,   288,   570,   289,     0,   676,
     116,   662,   288,   570,   289,     0,   163,     0,     9,     0,
     676,    83,    46,    63,   676,     0,   676,    83,   118,    46,
      63,   676,     0,   676,    17,   676,     8,   676,     0,   676,
     118,    17,   676,     8,   676,     0,   676,   103,   676,     0,
     676,   118,   103,   676,     0,   676,   103,   676,    54,   676,
       0,   676,   118,   103,   676,    54,   676,     0,   676,    75,
     674,     0,   676,   118,    75,   674,     0,   676,    31,   676,
       0,   676,   118,    31,   676,     0,   676,   167,   676,     0,
     676,   118,   167,   676,     0,   676,   167,   192,   676,     0,
     676,   118,   167,   192,   676,     0,    57,   288,   569,   289,
       0,   159,   288,   569,   289,     0,   676,    83,    94,     0,
     676,    83,   118,    94,     0,   248,     0,   249,     0,   250,
       0,   251,     0,   252,     0,   253,     0,   675,     0,   288,
     679,   289,     0,   288,   570,   289,     0,   653,     0,   678,
       0,   695,     0,   682,     0,   683,     0,   442,     0,   709,
       0,   710,     0,   720,     0,   708,     0,   283,   676,     0,
     282,   676,     0,   676,   282,   676,     0,   676,    28,   676,
       0,   676,    23,   728,     0,   676,   283,   676,     0,   676,
     284,   676,     0,   676,   285,   676,     0,   288,   676,   289,
       0,   288,   571,   289,     0,   684,     0,   685,     0,   686,
       0,    39,     0,   743,   293,    39,     0,    97,     0,   677,
       0,   724,     0,   220,     0,   221,     0,   222,     0,   653,
     291,   679,   292,     0,   676,     0,   679,   287,   676,     0,
     682,     0,   283,   681,     0,   197,     0,   196,     0,   195,
       0,   223,     0,   224,     0,   681,     0,   687,     0,    38,
     199,     0,   218,   199,     0,   219,   199,     0,   294,     0,
     181,     0,   225,     0,   226,     0,   233,     0,   234,     0,
      66,     0,   165,     0,   242,     0,   199,     0,   200,   199,
       0,   689,     0,   283,   690,     0,   196,     0,   196,     0,
     689,     0,   196,     0,   694,     0,   283,   694,     0,   196,
       0,   696,     0,   697,     0,   703,     0,    32,   288,   284,
     289,     0,    32,   288,   722,   676,   289,     0,    32,   288,
      46,   676,   289,     0,   171,   288,   722,   676,   289,     0,
     171,   288,    46,   676,   289,     0,    13,   288,   722,   676,
     289,     0,    13,   288,    46,   676,   289,     0,   110,   288,
     722,   676,   289,     0,   110,   288,    46,   676,   289,     0,
     106,   288,   722,   676,   289,     0,   106,   288,    46,   676,
     289,     0,   698,     0,   699,     0,   209,   288,   721,    63,
     676,   289,     0,   700,     0,   701,     0,   702,     0,   270,
     288,   676,   289,     0,   271,   288,   676,   289,     0,   272,
     288,   676,   289,     0,   275,   288,   676,   289,     0,   704,
       0,   706,     0,    96,   288,   676,   289,     0,   274,   288,
     676,   289,     0,   228,   288,   676,    63,   676,   705,   289,
       0,    61,   676,     0,     0,   277,   288,   707,   676,    63,
     676,   289,     0,   277,   288,   676,    63,   676,   289,     0,
     277,   288,   707,    63,   676,   289,     0,   277,   288,   676,
     289,     0,   267,     0,   276,     0,   273,     0,   725,   288,
     679,   289,     0,   725,   288,   289,     0,    20,   288,   676,
      10,   388,   289,     0,   711,     0,   712,     0,   237,   288,
     676,   287,   676,   289,     0,   261,   288,   656,   287,   676,
     287,   676,   289,     0,   238,   288,   676,   287,   679,   289,
       0,   713,     0,   715,     0,   236,   718,   714,    51,     0,
     236,   718,   714,    50,   719,    51,     0,   189,   717,   173,
     719,     0,   714,   189,   717,   173,   719,     0,   236,   716,
      51,     0,   236,   716,    50,   719,    51,     0,   189,   656,
     173,   719,     0,   716,   189,   656,   173,   719,     0,   676,
       0,   676,     0,   676,     0,   264,    97,    61,   736,     0,
      69,   288,   736,   287,   676,   289,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   216,
       0,   217,     0,     6,     0,     0,    46,     0,     0,    94,
       0,   198,     0,   750,     0,   750,     0,   750,     0,   750,
       0,   750,     0,   750,     0,   750,     0,   750,     0,   750,
       0,   750,     0,   750,     0,   750,     0,   750,     0,   750,
       0,   750,     0,   750,     0,   750,     0,   750,     0,   750,
       0,   750,     0,   750,     0,   750,     0,   750,     0,   750,
       0,   198,     0,   751,     0,   201,     0,   203,     0,   204,
       0,   205,     0,   206,     0,   208,     0,   227,     0,   230,
       0,   228,     0,   238,     0,   241,     0,   247,     0,   243,
       0,   237,     0,   240,     0,   246,     0,   248,     0,   249,
       0,   250,     0,   260,     0,   254,     0,   255,     0,   261,
       0,   262,     0,   216,     0,   217,     0,   265,     0,   264,
       0,   266,     0,   268,     0,   278,     0
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
    2045,  2047,  2049,  2051,  2057,  2060,  2062,  2066,  2074,  2075,
    2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2085,
    2086,  2087,  2088,  2089,  2090,  2091,  2092,  2093,  2094,  2095,
    2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,  2104,  2105,
    2106,  2107,  2108,  2109,  2111,  2112,  2113,  2115,  2116,  2119,
    2121,  2125,  2127,  2132,  2138,  2140,  2142,  2146,  2148,  2152,
    2155,  2158,  2163,  2167,  2169,  2173,  2175,  2182,  2186,  2187,
    2191,  2193,  2195,  2197,  2199,  2206,  2216,  2217,  2221,  2222,
    2228,  2232,  2234,  2236,  2238,  2240,  2242,  2244,  2246,  2248,
    2250,  2252,  2254,  2256,  2263,  2264,  2267,  2268,  2271,  2276,
    2283,  2284,  2288,  2297,  2303,  2304,  2307,  2308,  2309,  2310,
    2327,  2332,  2337,  2356,  2373,  2380,  2381,  2388,  2393,  2400,
    2407,  2416,  2420,  2426,  2430,  2434,  2440,  2444,  2451,  2457,
    2463,  2471,  2476,  2481,  2488,  2489,  2490,  2493,  2494,  2497,
    2498,  2499,  2506,  2510,  2521,  2527,  2581,  2643,  2644,  2651,
    2664,  2669,  2674,  2681,  2683,  2690,  2691,  2692,  2696,  2699,
    2702,  2710,  2711,  2712,  2715,  2719,  2723,  2725,  2729,  2733,
    2734,  2737,  2741,  2745,  2746,  2749,  2751,  2755,  2756,  2760,
    2764,  2765,  2769,  2770,  2774,  2775,  2776,  2777,  2780,  2782,
    2786,  2788,  2792,  2794,  2797,  2799,  2801,  2805,  2807,  2809,
    2813,  2815,  2817,  2821,  2825,  2827,  2829,  2833,  2835,  2839,
    2840,  2844,  2848,  2850,  2854,  2855,  2860,  2864,  2866,  2868,
    2872,  2876,  2878,  2880,  2882,  2884,  2886,  2888,  2890,  2892,
    2894,  2896,  2898,  2900,  2902,  2910,  2912,  2916,  2920,  2921,
    2928,  2932,  2934,  2938,  2940,  2944,  2946,  2953,  2957,  2962,
    2967,  2968,  2970,  2974,  2977,  2989,  2993,  2997,  3001,  3005,
    3007,  3009,  3011,  3015,  3017,  3019,  3023,  3025,  3027,  3031,
    3033,  3037,  3039,  3043,  3044,  3048,  3049,  3053,  3054,  3059,
    3063,  3064,  3068,  3069,  3072,  3073,  3074,  3079,  3084,  3085,
    3089,  3091,  3095,  3096,  3100,  3101,  3102,  3105,  3110,  3115,
    3119,  3120,  3123,  3127,  3131,  3134,  3139,  3141,  3145,  3146,
    3151,  3156,  3158,  3160,  3162,  3164,  3168,  3169,  3175,  3177,
    3181,  3182,  3188,  3191,  3193,  3197,  3199,  3206,  3208,  3212,
    3216,  3218,  3220,  3226,  3228,  3232,  3233,  3237,  3239,  3242,
    3243,  3247,  3249,  3251,  3255,  3256,  3260,  3262,  3268,  3270,
    3274,  3275,  3279,  3283,  3285,  3287,  3291,  3293,  3297,  3299,
    3305,  3308,  3316,  3323,  3327,  3335,  3336,  3339,  3345,  3353,
    3354,  3357,  3363,  3369,  3372,  3375,  3379,  3386,  3387,  3391,
    3395,  3402,  3404,  3408,  3410,  3412,  3416,  3417,  3420,  3424,
    3426,  3430,  3432,  3438,  3439,  3443,  3447,  3448,  3453,  3454,
    3458,  3462,  3463,  3468,  3469,  3472,  3477,  3482,  3485,  3492,
    3493,  3495,  3496,  3498,  3502,  3503,  3505,  3507,  3511,  3513,
    3517,  3518,  3520,  3524,  3525,  3526,  3527,  3528,  3529,  3530,
    3531,  3532,  3533,  3534,  3539,  3541,  3543,  3545,  3547,  3549,
    3551,  3553,  3559,  3561,  3563,  3565,  3567,  3569,  3571,  3573,
    3575,  3577,  3579,  3581,  3583,  3585,  3587,  3589,  3593,  3594,
    3600,  3602,  3606,  3608,  3613,  3615,  3617,  3619,  3624,  3626,
    3630,  3632,  3636,  3638,  3640,  3642,  3646,  3650,  3654,  3656,
    3660,  3665,  3670,  3677,  3682,  3687,  3696,  3697,  3701,  3708,
    3709,  3710,  3711,  3712,  3713,  3714,  3715,  3716,  3717,  3718,
    3720,  3722,  3729,  3731,  3733,  3740,  3747,  3754,  3756,  3758,
    3759,  3760,  3761,  3763,  3765,  3769,  3770,  3773,  3789,  3805,
    3809,  3813,  3814,  3818,  3819,  3823,  3825,  3827,  3829,  3831,
    3835,  3836,  3838,  3854,  3870,  3874,  3878,  3880,  3884,  3888,
    3891,  3894,  3897,  3900,  3905,  3907,  3912,  3913,  3917,  3924,
    3931,  3938,  3945,  3946,  3950,  3957,  3958,  3959,  3962,  3964,
    3966,  3969,  3976,  3985,  3992,  4001,  4003,  4005,  4007,  4011,
    4012,  4015,  4019,  4020,  4021,  4024,  4029,  4032,  4037,  4042,
    4043,  4044,  4046,  4050,  4059,  4061,  4065,  4067,  4070,  4072,
    4077,  4079,  4081,  4085,  4087,  4091,  4097,  4098,  4101,  4105,
    4108,  4112,  4113,  4116,  4118,  4122,  4124,  4128,  4130,  4134,
    4136,  4140,  4143,  4146,  4151,  4154,  4164,  4166,  4168,  4170,
    4172,  4174,  4176,  4178,  4182,  4183,  4186,  4187,  4190,  4198,
    4201,  4204,  4207,  4210,  4213,  4216,  4219,  4222,  4225,  4228,
    4231,  4234,  4237,  4240,  4243,  4246,  4249,  4252,  4255,  4258,
    4261,  4264,  4267,  4270,  4275,  4276,  4281,  4283,  4284,  4285,
    4286,  4287,  4288,  4289,  4290,  4291,  4292,  4293,  4294,  4295,
    4296,  4297,  4298,  4299,  4300,  4302,  4304,  4305,  4306,  4308,
    4310,  4311,  4312,  4313,  4314,  4315,  4316
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
     486,   486,   486,   486,   486,   486,   486,   487,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   489,
     489,   490,   490,   491,   492,   492,   492,   493,   493,   494,
     494,   494,   495,   496,   496,   497,   497,   498,   499,   499,
     500,   500,   500,   500,   500,   501,   502,   502,   503,   503,
     504,   505,   505,   505,   505,   505,   505,   505,   505,   505,
     505,   505,   505,   505,   506,   506,   507,   507,   508,   508,
     509,   509,   510,   510,   511,   511,   512,   512,   512,   512,
     512,   512,   512,   512,   512,   513,   513,   514,   514,   514,
     514,   515,   515,   516,   516,   516,   517,   517,   518,   518,
     518,   519,   519,   519,   520,   520,   520,   521,   521,   522,
     522,   522,   523,   523,   524,   524,   524,   525,   525,   526,
     526,   526,   526,   527,   527,   528,   528,   528,   529,   529,
     529,   530,   530,   530,   531,   532,   533,   533,   534,   535,
     535,   536,   537,   538,   538,   539,   539,   540,   540,   541,
     542,   542,   543,   543,   544,   544,   544,   544,   545,   545,
     546,   546,   547,   547,   548,   548,   548,   549,   549,   549,
     550,   550,   550,   551,   552,   552,   552,   553,   553,   554,
     554,   555,   556,   556,   557,   557,   558,   559,   559,   559,
     560,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   562,   562,   563,   564,   564,
     565,   566,   566,   567,   567,   568,   568,   569,   570,   571,
     572,   572,   572,   573,   574,   575,   576,   577,   578,   579,
     579,   579,   579,   580,   580,   580,   581,   581,   581,   582,
     582,   583,   583,   584,   584,   585,   585,   586,   586,   587,
     588,   588,   589,   589,   590,   590,   590,   591,   592,   592,
     593,   593,   594,   594,   595,   595,   595,   596,   597,   598,
     599,   599,   600,   601,   602,   602,   603,   603,   604,   604,
     605,   606,   606,   606,   606,   606,   607,   607,   608,   608,
     609,   609,   610,   611,   611,   612,   612,   613,   613,   614,
     615,   615,   615,   615,   615,   616,   616,   617,   617,   618,
     618,   619,   619,   619,   620,   620,   621,   621,   622,   622,
     623,   623,   624,   625,   625,   625,   626,   626,   627,   627,
     628,   628,   628,   629,   629,   630,   630,   631,   632,   633,
     633,   634,   635,   636,   636,   636,   637,   638,   638,   639,
     640,   641,   641,   642,   642,   642,   643,   643,   644,   645,
     645,   646,   646,   647,   647,   648,   649,   649,   650,   650,
     651,   652,   652,   653,   653,   653,   654,   655,   655,   656,
     656,   656,   656,   656,   657,   657,   657,   657,   657,   657,
     658,   658,   658,   659,   659,   659,   659,   659,   659,   659,
     659,   659,   659,   659,   660,   660,   660,   660,   660,   660,
     660,   660,   661,   661,   661,   661,   661,   661,   661,   661,
     661,   661,   661,   661,   661,   661,   661,   661,   662,   662,
     663,   663,   664,   664,   665,   665,   665,   665,   666,   666,
     667,   667,   668,   668,   668,   668,   669,   670,   671,   671,
     672,   672,   672,   673,   673,   673,   674,   674,   675,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   677,   677,   677,
     678,   679,   679,   680,   680,   681,   681,   681,   681,   681,
     682,   682,   682,   682,   682,   683,   684,   684,   685,   686,
     686,   686,   686,   686,   687,   687,   688,   688,   689,   690,
     691,   692,   693,   693,   694,   695,   695,   695,   696,   696,
     696,   696,   696,   696,   696,   696,   696,   696,   696,   697,
     697,   698,   699,   699,   699,   700,   701,   701,   702,   703,
     703,   703,   703,   704,   705,   705,   706,   706,   706,   706,
     707,   707,   707,   708,   708,   709,   710,   710,   711,   711,
     711,   712,   712,   713,   713,   714,   714,   715,   715,   716,
     716,   717,   718,   719,   720,   720,   721,   721,   721,   721,
     721,   721,   721,   721,   722,   722,   723,   723,   724,   725,
     726,   727,   728,   729,   730,   731,   732,   733,   734,   735,
     736,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   750,   750,   751,   751,   751,   751,
     751,   751,   751,   751,   751,   751,   751,   751,   751,   751,
     751,   751,   751,   751,   751,   751,   751,   751,   751,   751,
     751,   751,   751,   751,   751,   751,   751
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
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       4,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     1,
       1,     6,     1,     1,     1,     4,     4,     4,     4,     1,
       1,     4,     4,     7,     2,     0,     7,     6,     6,     4,
       1,     1,     1,     4,     3,     6,     1,     1,     6,     8,
       6,     1,     1,     4,     6,     4,     5,     3,     5,     4,
       5,     1,     1,     1,     4,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
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
       0,   103,   785,   978,     0,    23,   976,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,     0,   369,    29,    37,    34,
      36,   824,    35,   824,  1084,  1086,  1087,  1088,  1089,  1090,
    1091,  1110,  1111,  1092,  1094,  1093,  1099,  1095,  1100,  1096,
    1098,  1101,  1097,  1102,  1103,  1104,  1106,  1107,  1105,  1108,
    1109,  1113,  1112,  1114,  1115,  1116,     0,    31,     0,    32,
       0,    67,    69,  1076,  1085,     0,     0,     0,     0,     0,
       0,     0,   602,     0,     0,  1055,   686,   691,     0,     0,
     611,     0,   738,   740,  1078,     0,     0,     0,     0,   116,
     594,  1083,     0,     0,     2,     0,   676,     0,  1057,   792,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   467,   461,   462,   460,   463,   464,   465,   466,
     468,   470,   473,   469,   471,   472,   474,   475,   476,   477,
     478,     0,   437,   438,   408,     0,  1067,     0,   414,     0,
    1071,   411,   266,  1075,     0,   740,   410,   385,  1079,   415,
       0,  1070,   608,   601,   974,     0,   112,   166,   165,   113,
     148,   215,   836,  1063,   104,     0,   110,   160,     0,   106,
     266,   114,   980,   137,   107,   200,   108,     0,   109,   824,
    1082,   115,   161,   111,   783,   784,     0,   979,   977,     0,
      75,     0,  1068,   756,   518,  1066,   511,     0,   517,   522,
     512,   513,   520,   514,   515,   516,   521,   523,   363,     0,
     268,    30,     0,    39,   823,    38,    28,     0,     0,     0,
       0,     0,   829,     0,     0,     0,     0,     0,     0,   600,
     984,     0,   965,   685,   685,   686,   698,   696,  1054,   699,
       0,   700,   690,   686,     0,     0,     0,     0,     0,     0,
     627,   620,   609,   610,   612,   614,   615,   616,   623,   624,
     617,     0,   739,  1077,   120,     0,   117,   266,   118,   200,
     119,   824,   597,     0,   650,   651,   658,     0,   657,   659,
     661,   666,   654,   652,   655,   653,   662,   665,   660,   664,
       0,     0,     0,   674,     0,   670,     0,     0,  1056,     0,
       0,   677,     0,     0,     0,     0,   412,   498,   377,     0,
     377,     0,   218,   413,   419,   128,  1059,   416,   494,   487,
     488,     0,   268,   239,   479,     0,   409,   428,     0,   383,
     384,   507,     0,   607,   605,   975,   163,   163,     0,     0,
       0,   175,   167,   168,   147,     0,   125,     0,     0,     0,
     121,   268,   136,   135,   138,     0,     0,     0,     0,     0,
      76,   218,     0,     0,   758,   798,   519,     0,     0,     0,
       0,   942,   971,     0,  1058,     0,   944,     0,     0,   972,
       0,   966,   957,   956,   955,  1084,     0,     0,     0,   947,
     948,   949,   958,   959,   967,   968,  1094,   969,   970,     0,
    1099,  1095,   973,  1108,  1113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   924,   366,   919,   833,   951,
     945,   920,   361,   960,   922,   923,   939,   940,   941,   961,
     921,   985,   986,   999,  1000,  1002,  1003,  1004,   987,  1009,
    1010,   928,   925,   926,  1026,  1027,  1031,  1032,   927,   946,
       0,     0,  1063,     0,     0,     0,   370,   216,     0,     0,
       0,   826,    27,     0,    33,     0,     0,    43,    70,    72,
    1080,    68,     0,     0,     0,   828,     0,    48,    50,    28,
       0,     0,   599,     0,     0,     0,   695,   693,     0,   702,
       0,   701,   703,   708,   689,     0,   646,     0,   621,   632,
     618,   632,   619,   633,   639,   643,   644,   628,   613,   756,
     807,   837,     0,     0,   126,   268,     0,     0,   596,   595,
     663,   656,     0,     0,  1074,     0,     0,   671,   675,   778,
     780,   785,   682,   681,   790,   183,     0,   500,   144,     0,
     503,     0,   504,   499,     0,   423,     0,   425,   424,     0,
     426,     0,   420,     0,   496,     0,     0,   269,     0,     0,
     432,     0,   433,   377,   480,     0,   486,     0,     0,     0,
     483,  1063,   388,   387,   506,     0,   405,     0,   164,     0,
       0,   173,     0,   172,     0,     0,   162,   176,   177,   179,
     169,   555,   567,   542,   577,   545,   568,   578,     0,   584,
     546,     0,   574,     0,   569,   581,   541,   564,   543,   544,
     539,   377,   524,   525,   526,   534,   540,   527,   536,   557,
       0,   562,   559,   537,   574,   538,   124,     0,   122,   266,
     123,     0,     0,   139,     0,     0,   199,   239,   385,   375,
       0,   218,     0,    82,    84,     0,     0,   102,   101,  1060,
       0,     0,     0,     0,   910,   911,   912,     0,   755,   841,
     850,   853,   859,   854,   855,   856,   857,   861,   862,   860,
     863,   858,   839,     0,   764,   779,  1055,     0,  1055,   962,
       0,     0,  1055,  1055,  1055,     0,   963,   964,     0,     0,
    1042,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   930,   929,     0,   779,   951,     0,   331,
    1081,     0,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   534,   557,     0,     0,   368,     0,
     279,     0,   825,     0,     0,    44,    73,     0,     0,    26,
     815,     0,   831,     0,   794,   815,     0,     0,    47,   598,
       0,   685,     0,     0,   756,     0,   707,     0,   589,     0,
     588,   982,     0,   622,     0,   630,   626,   625,     0,   636,
       0,   641,   629,     0,   758,   802,     0,     0,     0,   239,
     375,   669,   647,   668,     0,     0,     0,   673,     0,   789,
       0,     0,   145,   184,   502,   377,   422,     0,   377,   378,
     427,   493,     0,   492,   375,     0,   265,   375,   238,  1064,
     210,   207,     0,   150,   377,   524,   375,     0,     0,     0,
     237,   240,   241,   242,   243,     0,   431,   485,   484,   430,
     429,     0,     0,     0,   391,   389,   390,   386,     0,   377,
     490,     0,   489,   193,   170,   174,     0,     0,   178,     0,
       0,   552,   565,   566,   582,     0,   579,   584,     0,   572,
     570,   571,   150,     0,     0,     0,   535,     0,     0,     0,
       0,   573,   127,   268,     0,   375,   140,   198,     0,   201,
     203,   204,     0,     0,     0,     0,   218,   218,    79,     0,
      87,    78,     0,     0,     0,     0,   852,   840,     0,     0,
     913,   914,   915,     0,   844,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   760,   762,   763,   757,     0,   792,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1046,  1047,  1048,  1049,  1050,  1051,
    1052,  1053,     0,     0,     0,     0,  1037,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1020,
    1022,  1021,     0,     0,   938,   792,   937,   362,     0,   332,
     364,   333,     0,     0,   933,  1062,   932,   931,   934,   935,
     936,   952,  1024,     0,   943,   835,   834,   372,   371,     0,
     272,   287,     0,   278,   280,   827,     0,     0,     0,    71,
       0,   820,     0,   830,     0,   820,     0,     0,   694,   697,
       0,   709,   710,   713,   715,   712,   724,   725,   726,   714,
     737,     0,   749,   704,   706,  1072,   590,   983,   631,   640,
     635,   634,     0,   645,   808,   779,   809,   838,   375,     0,
       0,   648,   667,     0,   781,   687,   782,   791,   501,   163,
     190,   182,   185,   186,     0,     0,     0,   421,   495,   375,
     375,   378,   277,   270,   279,   209,   377,   377,   378,   211,
     206,   212,     0,     0,   248,     0,   435,   378,   377,   375,
     434,     0,     0,     0,   404,   509,   491,   194,   195,   171,
       0,   181,   553,   554,   981,     0,     0,     0,   557,     0,
     580,     0,     0,   378,     0,   530,   532,     0,     0,     0,
       0,     0,     0,     0,   385,   279,   163,   142,   239,   196,
     405,   380,     0,    83,     0,   218,     0,    91,     0,    85,
       0,     0,    90,    86,     0,   806,  1065,     0,     0,   845,
       0,     0,   851,     0,     0,   843,   842,     0,   900,     0,
     889,   888,     0,   867,     0,   898,   916,     0,   908,     0,
       0,     0,   868,   894,     0,     0,   871,     0,     0,     0,
       0,     0,     0,     0,   869,     0,     0,   870,     0,   902,
       0,     0,   864,     0,     0,   865,     0,     0,   866,   761,
     764,   797,     0,     0,   218,     0,   988,     0,     0,  1011,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1043,
       0,     0,  1041,     0,     0,  1033,     0,     0,     0,     0,
    1044,  1005,  1006,  1007,  1012,  1008,     0,  1019,     0,     0,
     679,     0,     0,     0,   950,  1023,     0,   267,   286,     0,
     338,   367,   281,     0,     0,     0,     0,     0,  1090,    41,
      52,    54,    53,    62,    64,    41,    42,     0,     0,     0,
     812,   832,     0,   811,     0,     0,     0,   745,   712,     0,
     747,   741,   747,   745,   747,     0,     0,     0,   708,     0,
     754,   637,   638,   642,   792,   279,   197,   380,   649,     0,
       0,   191,     0,   187,     0,     0,     0,   417,   226,   221,
     953,   222,   223,   224,   225,     0,     0,   271,     0,     0,
       0,   227,     0,   248,   785,   244,     0,   436,   481,   482,
     396,   397,   393,   392,   395,   394,   377,   508,   378,   180,
    1061,     0,     0,   548,     0,   547,   583,     0,   575,   151,
       0,   528,     0,   556,   557,   563,   561,     0,   558,   375,
       0,     0,     0,   134,   143,   202,   377,     0,   376,   105,
       0,   132,   131,    94,     0,     0,    95,     0,    88,    89,
       0,   906,   907,   847,   846,   849,   848,     0,     0,     0,
       0,   779,     0,     0,     0,   909,     0,     0,     0,     0,
       0,     0,   901,   899,   895,     0,   903,     0,     0,     0,
       0,   904,     0,     0,     0,     0,     0,     0,   768,     0,
     765,     0,   769,   994,   993,     0,     0,   990,   989,     0,
     998,   997,   996,   995,   992,   991,     0,  1015,  1039,  1038,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,   335,   334,   273,     0,     0,   283,   284,     0,  1063,
     321,     0,     0,     0,     0,     0,     0,  1092,  1097,     0,
       0,   359,   297,   292,     0,   338,   291,     0,     0,     0,
     317,   318,     0,   315,     0,     0,   316,     0,     0,     0,
     353,   355,   354,     0,     0,     0,     0,     0,     0,     0,
     810,     0,  1063,    66,    58,    59,    65,    60,    61,     0,
      24,     0,     0,    25,     0,   816,   817,   818,   814,   819,
     822,   821,   805,    45,    46,   708,   716,   711,   746,   744,
     742,     0,   743,     0,     0,     0,     0,     0,     0,   758,
     801,     0,   376,   786,   787,   688,   193,   192,   188,     0,
     378,   954,   275,   276,   378,   378,   149,   214,   228,   229,
     377,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,   279,   505,   550,     0,   551,     0,     0,   214,   152,
     153,   155,   156,   157,   531,   533,   529,   560,   279,   405,
     378,   193,     0,     0,   373,   377,    92,     0,     0,    98,
      93,   557,     0,   892,     0,     0,   918,   792,   917,   890,
       0,     0,     0,   896,     0,     0,     0,     0,   905,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   759,
     764,     0,   771,     0,   767,   770,  1025,   217,  1045,  1001,
       0,     0,  1040,  1035,  1034,     0,  1028,  1030,     0,  1017,
    1018,     0,   220,   282,     0,   319,     0,   314,     0,   329,
     313,   341,  1073,   356,   357,     0,   293,     0,   298,   295,
     344,   308,   309,   305,   306,   310,   303,     0,     0,   312,
     311,   304,     0,   300,   302,   301,   299,   307,   337,     0,
      55,    56,    57,    63,     0,     0,   793,   719,     0,   727,
     745,   736,   734,   748,   750,   752,   753,   684,   378,   374,
     788,   189,   378,   159,   264,     0,     0,   205,   230,     0,
       0,     0,   248,   231,   234,   824,     0,   378,   402,   403,
     399,   398,   401,   400,     0,   549,   576,   158,   146,   154,
       0,   377,   261,   141,   378,   379,     0,     0,    96,    97,
      80,     0,   875,   883,   678,   891,   876,   884,   879,   887,
     893,   897,   877,   885,   878,   886,   872,   880,   873,   881,
     874,   882,   766,     0,   777,  1014,  1013,  1036,     0,  1016,
       0,     0,   289,   285,     0,   320,   322,     0,   328,     0,
     352,     0,     0,     0,     0,   347,   345,     0,     0,     0,
     330,    40,   813,   803,   721,   718,   728,     0,     0,   729,
     730,   731,     0,   262,   418,   208,   213,   232,   248,   824,
     235,   254,   247,   249,   406,   378,     0,   381,   378,    77,
      99,   100,     0,   774,     0,   773,  1029,   219,   288,     0,
       0,     0,     0,   351,   350,  1069,   349,   338,     0,     0,
       0,     0,     0,     0,   717,   732,     0,   751,   236,   254,
       0,   248,   250,   251,   263,   378,     0,   772,     0,     0,
       0,   325,   338,   358,   346,   348,     0,   343,     0,   804,
       0,   722,     0,   248,     0,     0,   246,     0,   253,     0,
     252,   382,   133,   775,     0,   290,   327,     0,     0,     0,
     338,     0,   720,   733,   233,     0,     0,   257,   256,   255,
     776,   338,     0,     0,   338,   336,   723,   260,   258,   259,
     326,   338,   342,   323,   324,     0,     0,     0
};

static const short yydefgoto[] =
{
    2025,    21,    22,   563,   146,   147,   148,   149,  1610,   839,
     834,    23,   160,   161,  1359,  1360,  1361,  1362,  1363,   150,
     151,   567,   568,    24,    89,   460,   990,   742,   743,   744,
    1243,  1236,  1237,  1700,   290,   746,    25,    81,    26,   179,
      27,   450,   264,   364,   726,   234,    82,  1469,   271,   454,
     734,  1227,  1463,   637,   259,   445,  1178,  1678,  1679,  1680,
    1681,  1682,  1683,   266,   281,   256,   679,   257,   441,   442,
     443,   686,   687,   688,   638,   639,  1161,  1162,  1163,  1402,
    1199,   274,   368,   456,   978,   979,   980,   911,   912,   913,
    1817,   661,   553,  1525,   745,  1882,  1407,  1657,  1658,  1659,
    1823,   981,   663,   920,   921,   922,   923,  1425,   924,  1961,
    1962,  1963,  2008,   269,   366,   728,   241,   422,   559,   656,
     657,  1099,   554,  1171,  1102,  1103,  1104,  1555,  1556,  1349,
    1883,  1557,  1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,
    1580,  1581,  1582,  1583,  1584,  1888,  1585,   515,  1080,  1586,
    1587,  1588,  1999,  1769,  1770,  1894,  1895,  1589,  1590,  1591,
    1592,  1765,  1593,   516,   812,    29,   310,   555,   556,   278,
     370,   983,  1694,   646,  1167,  1468,   276,   730,   431,   674,
     675,   937,   939,  1437,    30,    65,   899,   896,   413,   414,
     426,   427,   231,   232,   428,  1187,   669,   929,   238,   249,
     417,   654,   903,   190,   406,   407,   246,   676,  1438,    31,
     104,   711,   712,   713,  1214,  1215,   714,   824,   716,   717,
    1208,   951,   966,   718,   825,   720,   721,   722,   723,   959,
     724,   725,   956,    32,    33,    34,    35,    36,   619,    37,
     583,    38,    39,    67,   253,   434,    40,   352,   353,   354,
     355,   356,   357,   358,   359,   866,   360,  1142,  1393,   603,
     604,   871,   605,    41,    42,   390,   391,   392,   886,   882,
    1594,   186,   627,   395,    44,  1490,   805,    45,    46,    47,
     586,   342,  1399,  1810,   165,   166,   167,   340,   590,   591,
     592,   857,   854,  1121,  1122,  1123,  1124,  1904,  1954,  1980,
    1125,  1126,  1127,  1128,  1909,  1910,  1911,  1129,  1388,   171,
     172,  1386,  1629,  1390,  1803,  1804,  1639,   464,   775,  1518,
    1028,  1519,  1520,  1521,  1734,  1932,  1935,   189,   629,   630,
     286,  1645,  1156,   401,  1595,  1596,    50,    51,  1597,    53,
      54,  1796,   465,   609,  1598,  1599,    55,  1111,  1614,  1615,
    1370,  1619,   313,   314,   560,   574,   575,   841,   517,   518,
     612,   758,  1003,   759,   760,   761,   762,  1262,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,  1005,  1265,
    1266,   773,   520,   521,   522,  1409,   523,   524,   525,   526,
     527,   528,   529,  1617,    86,   288,   273,  1206,  1216,   861,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
    1741,   540,  1073,   541,   542,   543,   544,   545,   546,  1059,
     547,   791,  1323,   792,  1320,   548,  1052,   341,   399,   549,
     550,   748,  1439,  1084,   262,   908,  1245,   294,   235,   291,
    1944,   267,  1933,  1134,  1601,   623,  1130,   152,   551,   245,
     247,  1364,   809,   279,   180,   552,   154
};

static const short yypact[] =
{
    1886,  1978,    76,  1921,   268,   751,   227,  2434,    67,  4296,
     207,   431,   151,    76,   603,   183,  6902,   392,  6902,   265,
     412,   274,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   519,   102,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  8990,  6902,   558,
    6902,  6902,  6902,  6902,  6902,-32768,-32768,   526,   664,  6902,
    6902,  6902,   679,  6902,   516,  6902,  6902,-32768,  6902,  6902,
    6902,-32768,   247,-32768,   598,-32768,-32768,   696,  6902,-32768,
    6902,  6902,  6902,   754,  6902,  6902,  6902,  6902,   516,  6902,
    6902,  6902,  6902,  6902,-32768,  6902,   563,   706,-32768,-32768,
  -32768,   567,-32768,   567,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   718,   573,   747,-32768,
      79,-32768,-32768,-32768,-32768,  6902,  6902,  6902,   760,   776,
     686,  6902,   752,   401,   419,   688,   676,-32768,  6902,   842,
     948,   775,-32768,  6902,-32768,  6902,  6902,  6902,  6902,-32768,
  -32768,-32768,  6902,  8922,-32768,   766,   750,   950,   698,   719,
     647,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   152,-32768,-32768,-32768,   664,-32768,   788,-32768,   302,
  -32768,-32768,   694,-32768,   870,-32768,-32768,   585,-32768,-32768,
     737,-32768,   831,-32768,-32768,   822,-32768,   832,-32768,-32768,
    1019,-32768,-32768,-32768,-32768,   664,-32768,-32768,   418,-32768,
     694,-32768,-32768,   197,-32768,   975,-32768,   977,-32768,   567,
  -32768,-32768,-32768,-32768,-32768,-32768,   960,-32768,-32768,   788,
  -32768,   967,-32768,   879,-32768,-32768,-32768,   788,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  7808,  6902,
     921,-32768,  6902,-32768,-32768,-32768,   900,   466,   932,  4046,
    6902,   993,   790,  1013,  1018,  1024,   962,   964,    97,   844,
  -32768,  8068,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5988,-32768,-32768,-32768,   915,  6902,   988,   904,   160,  6902,
     927,-32768,-32768,   948,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  6902,-32768,-32768,-32768,   664,-32768,   694,-32768,   975,
  -32768,   567,   970,   946,-32768,-32768,-32768,  1048,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1035,  6902,  6902,  1004,   883,-32768,  8068,   972,-32768,   972,
    8068,-32768,    80,   876,   877,   881,   647,-32768,  1109,   243,
  -32768,  6902,-32768,   152,-32768,-32768,-32768,-32768,  1091,-32768,
  -32768,  6902,   921,  2163,   938,  2968,   859,-32768,  8990,-32768,
  -32768,   269,   955,-32768,-32768,-32768,   869,   869,   664,  1039,
     664,   132,   832,-32768,-32768,  3891,-32768,  6902,  6902,  6902,
  -32768,   921,-32768,-32768,  1124,   458,   866,  6902,  1147,  6902,
  -32768,    36,  6382,  4682,  1025,-32768,-32768,   873,   874,   882,
     978,-32768,-32768,   888,-32768,   890,-32768,   892,   896,-32768,
     897,-32768,-32768,-32768,-32768,   899,   902,   989,   995,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   916,-32768,-32768,  6248,
     918,   919,-32768,   922,  1072,   923,   924,   926,   933,   934,
     936,  8068,  8068,  6508,  6902,-32768,  1044,   880,-32768,  1566,
  -32768,-32768,   914,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     943,   939,   940,  3891,   930,   528,-32768,-32768,   947,  1216,
     536,-32768,-32768,  6902,-32768,  6902,  6902,   286,-32768,-32768,
  -32768,-32768,  6902,  6902,   489,-32768,  6902,-32768,-32768,   900,
    1095,  4046,-32768,  6902,  6508,   440,-32768,-32768,  8068,-32768,
    1174,   966,-32768,   187,-32768,   285,-32768,   612,-32768,   400,
  -32768,   400,-32768,   968,-32768,   119,-32768,  1081,-32768,    42,
  -32768,-32768,   981,   956,-32768,   921,   963,  1244,-32768,-32768,
  -32768,-32768,   468,  1173,-32768,   965,  6902,-32768,-32768,   980,
  -32768,   674,-32768,-32768,   605,-32768,  1175,  1182,-32768,   664,
  -32768,  1191,-32768,-32768,  1260,-32768,  1263,-32768,-32768,  1243,
  -32768,  3891,-32768,   664,  1177,  3891,   560,-32768,  1279,  6902,
  -32768,  8649,-32768,   120,-32768,  6902,   649,   870,  1116,  1085,
    1158,  1121,-32768,-32768,-32768,   175,  1162,   327,-32768,  1108,
     516,-32768,   664,-32768,  1284,  1217,-32768,   132,-32768,-32768,
  -32768,    61,  1125,-32768,-32768,-32768,  1126,-32768,  1171,  1028,
  -32768,  1226,  1031,   479,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1029,-32768,-32768,-32768,   103,
    1033,  1036,    29,-32768,  1031,-32768,-32768,   664,-32768,   694,
  -32768,  1262,  1315,-32768,   664,   664,-32768,  2163,   585,-32768,
    1206,-32768,  1183,  1042,-32768,  3624,  1207,-32768,-32768,-32768,
    1218,  1050,  4942,  1054,  1355,  1377,  1434,  4422,   191,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  2332,   407,  1225,   727,  8068,   138,-32768,
    6902,  8068,   762,   770,   781,  1837,-32768,-32768,  8068,  5202,
    1566,   188,  1154,  8068,  8068,  5202,  1291,  8068,  8068,  8068,
    8068,  8068,  5462,   177,   177,  1068,   102,   463,   586,-32768,
  -32768,  5336,-32768,  8068,  6902,  8068,  8068,  8068,  8068,  8068,
    8068,  6768,  3235,-32768,-32768,  1337,  1069,  6902,-32768,  6902,
    1325,  6902,-32768,  1194,  1195,-32768,-32768,  1172,  4046,-32768,
    1313,   591,-32768,  1087,-32768,  1313,  6902,  6902,  1090,-32768,
     463,-32768,   517,  5862,  1189,  8068,-32768,  6902,-32768,    65,
  -32768,-32768,   155,-32768,  1196,-32768,-32768,-32768,  6902,   385,
    6902,-32768,-32768,  6902,  1025,-32768,  8068,  6902,  1370,  2163,
  -32768,-32768,-32768,-32768,   468,  6902,  1298,  1097,  8068,  1145,
    8068,   664,-32768,   510,-32768,-32768,-32768,  1099,-32768,-32768,
  -32768,-32768,   664,-32768,   106,  6902,-32768,-32768,-32768,-32768,
    1373,-32768,  1105,  1358,-32768,  1379,-32768,  1318,  1328,   567,
  -32768,-32768,-32768,-32768,-32768,  1391,-32768,-32768,-32768,-32768,
  -32768,  6902,  8842,   516,  1290,  1292,  1302,-32768,   516,-32768,
  -32768,   485,-32768,   784,-32768,-32768,  1268,   664,-32768,  6382,
      47,  1275,-32768,-32768,-32768,   516,-32768,  1028,   293,-32768,
  -32768,-32768,  1358,   293,  1276,   293,-32768,   516,   516,  1146,
     516,-32768,-32768,   921,  6902,-32768,  1335,-32768,   592,-32768,
  -32768,-32768,   269,   972,  6902,   617,    45,-32768,-32768,  1149,
     427,-32768,  6382,  6902,   972,  5728,-32768,-32768,   972,  4942,
  -32768,-32768,-32768,   121,-32768,   216,  1738,  5202,  5202,  8068,
    8068,  2063,  1151,   609,  2538,  8068,  2816,   700,  3094,  3361,
    7028,  3628,  3895,  4162,-32768,-32768,  1334,-32768,  1155,   719,
    8068,  8068,   254,  8068,  1157,  8068,  1148,   678,  8068,  8068,
    8068,  8068,  8068,  8068,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1384,  1220,   184,  8068,-32768,  5202,  8068,   294,
     913,   979,   137,  6902,   712,   726,   734,   742,   758,-32768,
  -32768,-32768,   352,  7288,-32768,   719,-32768,-32768,  5602,-32768,
    1161,-32768,  1160,   239,-32768,-32768,  1431,   177,   177,   709,
     709,  1566,-32768,   624,-32768,-32768,-32768,-32768,-32768,   638,
  -32768,  1271,  1443,  1325,-32768,-32768,  4566,  4566,  1339,-32768,
     133,  1354,  6902,-32768,  8068,  1354,  1401,  1404,-32768,-32768,
    5076,  1192,   390,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1181,  5202,  1399,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    91,-32768,-32768,  1225,  1566,-32768,-32768,   656,
     972,-32768,-32768,   468,-32768,-32768,-32768,  1566,-32768,   869,
    1464,-32768,   510,-32768,  1197,  5202,  2360,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1325,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   567,   567,  1245,  1199,-32768,-32768,-32768,-32768,
  -32768,   185,   159,   547,-32768,  1472,-32768,-32768,-32768,-32768,
    6902,-32768,-32768,-32768,-32768,   268,   702,  1323,  1337,  1200,
  -32768,  1108,   717,-32768,   472,-32768,  1201,  6902,   480,  1203,
    1209,   516,  1210,  1480,   585,  1325,   869,  1182,  2163,-32768,
    1162,  1314,  2422,-32768,   516,  1367,  1455,-32768,  3624,-32768,
     516,    19,-32768,-32768,  1456,-32768,-32768,  1221,  1223,-32768,
    5202,  5202,-32768,  5202,  5202,-32768,  1506,   212,  1566,  1227,
  -32768,-32768,  1228,  1566,  6508,-32768,-32768,  1460,-32768,   436,
    1230,  1231,  1566,  1264,  1236,  1237,  1566,  8068,  8068,  1151,
    8068,  7548,  1239,  1240,  1566,  1246,  1254,  1566,  8068,  1566,
    1255,  1267,  1566,  1269,  1270,  1566,  1274,  1277,  1566,-32768,
    4826,-32768,   798,   945,-32768,   957,-32768,  1106,  8068,-32768,
    1114,  1127,  1136,  1247,  1256,  1285,  8068,  8068,  8068,  1566,
    1482,   190,  1566,  1383,  8068,-32768,  8068,  8068,  8068,  8068,
  -32768,-32768,-32768,-32768,-32768,-32768,  8068,-32768,  8068,  1403,
  -32768,   725,  5602,  6642,-32768,-32768,  6902,-32768,-32768,  6902,
    8386,-32768,-32768,  6902,  6902,  6902,  6902,  6902,  6902,  1371,
    1286,-32768,  1288,-32768,-32768,  1371,-32768,   306,   306,    31,
  -32768,-32768,   733,-32768,  4566,  4566,  1282,   455,  1283,  5862,
    1437,-32768,  1437,   692,  1437,  1492,  1493,  8068,  1067,  1560,
    1512,-32768,-32768,-32768,   719,  1325,-32768,  1314,-32768,   449,
    1108,  1457,  1108,-32768,  5202,   129,   753,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  2360,  2360,-32768,  1443,  8068,
    2360,   531,  1441,  1245,   247,-32768,  5202,-32768,-32768,-32768,
    1470,  1471,  1473,  1474,  1475,  1479,-32768,-32768,-32768,-32768,
  -32768,  1317,   268,-32768,  1411,-32768,-32768,   293,-32768,   234,
     293,-32768,   293,-32768,  1337,-32768,-32768,  1319,-32768,-32768,
     269,  1443,  1108,-32768,  1182,-32768,-32768,  1593,-32768,-32768,
    1331,-32768,  1097,-32768,  1332,   664,   146,  1352,-32768,-32768,
     664,-32768,-32768,-32768,  1506,-32768,  1506,  8068,   972,   972,
    1356,   102,   741,  8068,  1557,-32768,   972,   972,  8068,   972,
     972,   229,  1566,-32768,  1348,  8068,  1566,   972,   972,   972,
     972,  1566,   972,   972,   972,   972,   972,   972,-32768,  1360,
    1336,   540,  6902,-32768,-32768,  1361,  3891,-32768,-32768,  1299,
  -32768,-32768,-32768,-32768,-32768,-32768,  1308,  1590,-32768,-32768,
    8068,  8068,  1575,  1481,  1327,   748,  1077,  1413,  1422,  8068,
  -32768,-32768,-32768,-32768,  3891,  1342,-32768,-32768,  1607,  1608,
    6902,   289,  1369,  1359,  1571,  8068,  1378,  1380,  6122,  6902,
    6902,-32768,-32768,-32768,  1612,  8297,-32768,  1381,  1385,  1386,
  -32768,-32768,  1387,-32768,  1388,  1389,-32768,   127,  1392,  1394,
  -32768,-32768,-32768,  1395,  1583,  1396,  1398,  1408,  1414,  1415,
  -32768,  1400,   552,-32768,-32768,-32768,-32768,-32768,-32768,  1599,
  -32768,  4566,  4566,-32768,  1503,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1421,-32768,-32768,  1679,-32768,   390,-32768,-32768,
  -32768,  1618,-32768,  5862,  5862,   759,  6902,  8068,  5202,  1025,
  -32768,  1443,-32768,-32768,-32768,-32768,   784,-32768,-32768,   143,
  -32768,-32768,-32768,-32768,-32768,  1566,-32768,  1685,   531,-32768,
     581,  6902,-32768,  1630,   145,  1530,  1632,  1542,  1682,  1643,
    1684,  1325,-32768,-32768,  1446,-32768,  1447,  1636,  1685,   234,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1325,  1162,
  -32768,   784,  1472,  1613,-32768,-32768,-32768,  1628,    92,-32768,
  -32768,  1337,  1633,  1566,  1459,  1461,-32768,   719,-32768,  1566,
    8068,  1462,  1463,  1566,  1468,  1469,  8068,  8068,  1566,  1478,
    1484,  1486,  1488,  1491,  1494,  1496,  1498,  1501,  1502,-32768,
    4826,  1465,-32768,  6902,-32768,-32768,-32768,-32768,-32768,-32768,
    8068,  1504,-32768,-32768,-32768,  8068,-32768,-32768,  8068,-32768,
  -32768,  1449,   112,-32768,  1698,  8068,  8068,-32768,  5202,  1566,
  -32768,-32768,-32768,-32768,-32768,  6902,-32768,   163,-32768,  1556,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   311,  1483,-32768,
  -32768,-32768,  5602,-32768,-32768,-32768,-32768,-32768,-32768,  1638,
  -32768,-32768,-32768,-32768,   306,  6902,-32768,  6902,  5862,-32768,
     565,-32768,-32768,  1476,-32768,  1566,   191,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1505,  6902,-32768,-32768,  1666,
    1695,  6902,  1245,-32768,-32768,   567,  6902,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1443,-32768,-32768,-32768,-32768,-32768,
    1443,-32768,-32768,-32768,-32768,-32768,  8068,   664,-32768,  1588,
  -32768,   664,-32768,-32768,-32768,  1566,-32768,-32768,-32768,-32768,
    1566,  1566,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  6902,  1691,  1566,-32768,-32768,  1458,-32768,
    2360,  2360,-32768,-32768,  1508,  1566,  1594,   153,-32768,  1715,
  -32768,  6902,  6902,   268,   111,-32768,-32768,  1552,  1718,  5202,
    1161,-32768,-32768,    81,  1513,-32768,-32768,  5202,  1514,-32768,
  -32768,-32768,  8068,-32768,-32768,-32768,-32768,-32768,  1245,   567,
  -32768,  1681,-32768,-32768,-32768,-32768,  1472,-32768,  1566,-32768,
  -32768,-32768,  1517,  1522,  1523,-32768,-32768,-32768,-32768,   972,
    5602,  1637,  5602,-32768,-32768,-32768,-32768,  8551,   163,  8068,
    5602,   154,  5602,  6902,-32768,   191,  6902,-32768,-32768,  1681,
     162,  1245,  1693,  1694,-32768,-32768,  1529,-32768,  6902,  6902,
    1531,  1161,  8551,  1161,-32768,-32768,  1726,   113,  1772,  1161,
     763,-32768,   771,  1245,   475,   475,-32768,  1778,-32768,  1644,
  -32768,-32768,-32768,-32768,  1534,-32768,  1776,  5602,  1791,  1781,
    8551,  6902,-32768,-32768,-32768,  1629,   621,-32768,-32768,-32768,
  -32768,  8551,   157,  6902,  8551,-32768,-32768,-32768,-32768,-32768,
  -32768,  8551,-32768,-32768,-32768,  1831,  1833,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1257,  1674,-32768,  1675,  1520,   473,-32768,
     997,-32768,-32768,-32768,  -917,-32768,  -673,-32768,  -659,  1686,
    1525,  1259,  1014,-32768,-32768,-32768,   604,-32768,  1118,   868,
  -32768,-32768,   625,-32768,-32768,   871,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   634,-32768,-32768,   903,-32768,-32768,   189,
  -32768,-32768,  1454,  1786,-32768,-32768,  -364,-32768,-32768,-32768,
    1425,-32768,-32768,  1184,  -411,-32768,-32768,-32768,   707,-32768,
    -953,-32768,-32768,  1511,   991,   653,  1466,   969,   654,-32768,
     209,   -47,  -307,-32768,  -370,-32768, -1298,-32768,-32768,   230,
  -32768,  1467, -1288,-32768,-32768,-32768,-32768, -1343,   225,   -67,
     -69,   -65,   -90,-32768,-32768,-32768,-32768,  -151,  -348,-32768,
     994,-32768,  -724,-32768, -1075,-32768,   793,-32768,-32768,-32768,
  -32768,-32768, -1352, -1057,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -784, -1043,-32768,
  -32768,-32768,-32768,-32768,   131,-32768,   -46,-32768,-32768,-32768,
  -32768,-32768,  1901,-32768,-32768,-32768,-32768,-32768,  1076,-32768,
  -32768,  -671,   262,  -392,  -857,   509,-32768,-32768,  -625,  -867,
  -32768,-32768, -1104, -1557,-32768,-32768,-32768,-32768,-32768,  1495,
  -32768,  1242,  1485,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1509,-32768,-32768,-32768,-32768,
  -32768,  -621,  -521,-32768,   949,   460,  -635,  -417,-32768,-32768,
  -32768,-32768, -1111,-32768,  -414,-32768,-32768,-32768,-32768,  1188,
  -32768,-32768,   959,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1904,-32768,-32768,-32768,-32768,-32768,  1565,
  -32768,-32768,-32768,  1324,-32768,  1321,-32768,-32768,-32768,-32768,
    1052,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -746,
       1,-32768,-32768,-32768,  -518,  1023,-32768,  -393,   582,-32768,
    -227,   415,-32768,-32768,-32768,-32768,  1758,-32768,-32768,-32768,
    1070, -1212,-32768,-32768, -1008, -1439,-32768,-32768,-32768,-32768,
     807,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1839,
     -56,   548,  -318,-32768,-32768,    20,-32768,  -481,  -830,  1159,
  -32768,   204,-32768,   414,-32768,  -969,-32768,  -728,-32768,  1051,
    -588,-32768,-32768,  -971,  1941,  1943,-32768,-32768,  1945,-32768,
  -32768,-32768,  1340,-32768,  -266,-32768,-32768,  1101, -1252,-32768,
     833,-32768,  -105,  -787,  -623,-32768,-32768,-32768,  -777,   -49,
    -498,  -766,-32768,  -540,-32768,-32768,-32768,   255,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -610,-32768,   671,
  -32768,  1180, -1023,-32768,  -475,-32768,   546,  -995,  -152,  -989,
    -987,  -979,   -52,    -2,   -35,-32768,     4, -1121,  -523,  -150,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1300,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   627,-32768, -1193,-32768,-32768,   273,-32768,  -964,
     454,  1005,   738,   148,  -738,  1305, -1272,-32768,   -66,  1873,
  -32768,   -27,   -50, -1452,   403,  -281,   -40,   -43,  -168,    -1,
     -59,  -294,-32768,   -77,   -55,    -9,-32768
};


#define	YYLAST		9268


static const short yytable[] =
{
     153,    43,    85,   887,   265,   362,   244,   174,   315,   181,
     239,   333,   336,   334,   337,   173,   258,   277,   649,   275,
     261,   242,   260,  1054,   305,   569,   296,  1079,   715,  1062,
     689,   719,   823,   270,  1081,  1341,   282,   250,   808,   272,
     914,   304,   651,   889,  1145,  1351,   300,  1029,   233,   236,
    1600,   240,   243,   174,   248,   251,   844,   301,  1301,   306,
     263,   236,   251,   272,   243,   308,   174,   248,   299,   280,
     153,   251,   860,   680,   658,   842,   307,  1558,  1075,   292,
    1662,   174,   295,   236,  1096,   292,   251,   240,   243,   173,
     174,   248,   280,   153,   251,   610,   243,  1445,   303,  1418,
     322,   371,   302,   732,  1340,  1100,   321,   587,   323,   365,
     991,   625,  1377,   982,   655,  1230,  1618,  1652,  1653,   451,
     806,   369,  1656,  1998,   964,  1538,  1466,   372,   874,  1250,
     900,  1542,  1184,  1660,   904,  1844,   367,  1007,  1151,  1147,
     915,   344,   997,  1408,   338,  1007,   263,   174,   263,  1169,
    1461,  1007,   153,  1007,   942,  1880,   408,   -49,  1947,   251,
     581,  1007,  1007,  1952,   363,  1698,   236,   243,   174,   280,
     635,  1410,  1890,   181,   458,   684,  1636,  1411,  1234,  1412,
     869,   599,   917,   415,  1033,   -81,   599,  1413,  1777,  1848,
    1365,   806,  1007,   613,  1799,   -49,  1367,   856,  1007,  1007,
     814,   409,  1414,  1432,  2021,   815,  1984,   -49,   105,  1150,
     814,   452,   996,   446,   969,   815,   615,  1004,  1891,   934,
    1487,   158,   635,  1768,  1253,   155,   892,  1204,   187,  1892,
     949,   -49,   463,  1172,  1391,   814,  1174,  1716,  1055,  1056,
     815,  1255,  1256,  1204,   715,  1181,  1251,   719,  1621,  1478,
    -705,   168,   814,   319,  1008,   935,  -377,   815,   284,   918,
     557,   330,  1008,   561,  1304,  1430,   617,   878,  1008,    66,
    1008,   925,   836,   647,   672,  1600,   689,   814,  1008,  1008,
     188,  1479,   815,   600,   673,  1392,   648,   569,  1136,   156,
      90,  1321,   285,   606,  1079,   596,   -49,  1763,  1764,   919,
     263,  1081,   453,   263,  1225,   419,   -49,  1368,   410,  1008,
     570,   153,   611,   614,  1417,  1008,  1008,   970,  1778,   962,
    1641,  1421,  1849,  1675,   741,   332,   411,   106,  1893,   873,
    1427,   -49,   601,  1235,  1205,   636,   240,   601,  1083,  1433,
     174,  1254,  1985,  1686,  1324,  1325,  1093,  1742,  1743,   950,
    1699,   169,  1677,   159,   602,   936,  1449,  1318,   170,  1906,
     412,  1654,   650,  1540,   806,  1431,   320,  1897,   831,  1965,
    1660,  1627,   557,  1132,   261,   814,   666,  1057,   420,   670,
     815,   727,   624,   624,   320,  1170,   681,   685,   683,  1249,
     731,  1881,  1408,  1408,   965,  1422,  1423,  1408,  1948,  1405,
    1342,   738,   263,   736,  1690,   613,   870,  1398,   729,   740,
    1252,  1188,   263,  1797,   263,  1336,   263,  1394,  1650,   671,
    1410,  1410,  1034,  1640,  1329,  1410,  1411,  1411,  1412,  1412,
     105,  1411,  1812,  1412,  1827,  1212,  1413,  1413,   236,   243,
     248,  1413,  1941,  1978,  1342,  1471,  1241,   175,   174,   157,
     240,  1414,  1414,   749,  1380,   996,  1414,  1623,  1624,   996,
     747,   818,   819,   814,    83,  1231,    14,  1381,   815,   816,
     817,   818,   819,   447,  -705,  -745,  1247,  1395,   837,  1920,
    1248,   330,  1494,  1326,  1483,  1484,   814,  1485,  1486,   330,
    1382,   815,  1024,  1889,   816,   817,   818,   819,  1415,  1416,
     960,  1981,    83,  1164,  1383,   810,  1166,   833,   858,   182,
     108,   816,   817,   818,   819,  1025,   840,   864,  1429,  1380,
     845,  1242,  1179,   330,   611,   835,   820,  1140,   849,   943,
    1495,  1344,  1381,   176,   183,  1756,   816,   817,   818,   819,
     814,  1384,  1902,  1141,   569,   815,   110,  1195,   735,  2016,
     940,    84,  1877,  -239,   174,  1382,   243,   570,  1551,   448,
     184,   659,   881,   174,   177,  1552,   961,   174,   859,  1383,
     883,  1026,   570,   838,   181,  1958,  1211,   561,   973,   178,
     185,  1672,  1937,  1938,  1808,  1841,   865,   893,   429,    84,
    1850,  1434,  2005,  1689,   449,  1974,  1834,   330,   655,  1460,
     332,   901,  1376,   991,  1152,  1096,  1384,  1144,  1159,  1472,
     941,   111,   916,  1840,  1371,   330,  1238,   263,  1986,  1731,
    1996,   112,  1553,   237,  1118,  1223,  1800,  1435,   814,  1380,
     945,  2006,  1397,   815,   816,   817,   818,   819,  1649,  1372,
    2004,  1337,  1381,  1082,    14,   272,   113,  1600,  2015,  -239,
     909,   402,   263,  1385,  1732,  1267,   909,   254,   255,  2020,
    1664,   430,  2023,   403,  2018,  1382,  1733,   254,   255,  2024,
    -239,   843,  1600,   252,  1097,   972,  -239,  1160,  2007,  1383,
    1643,   330,   976,   977,   944,   284,   268,  1907,   261,   331,
    1644,   418,   107,  1811,   338,   332,   404,   814,   405,  1819,
    1600,   814,   815,  1268,   397,   613,   815,   335,  1196,  1137,
    -239,  1600,    83,   332,  1600,  2019,  1384,  1277,  1385,   285,
    1820,  1600,   816,   817,   818,   819,  1821,  1269,   263,   851,
     108,  1278,   814,   338,   339,   814,  1854,   815,  1843,  1900,
     815,  2022,   109,   461,   398,   816,   817,   818,   819,   814,
     588,   466,  1076,  1036,   815,   862,  1380,   814,   594,  1450,
    1822,   289,   815,  1707,  1451,   814,   110,  1450,   338,  1381,
     815,   251,  1454,  1030,   350,  1279,   338,  1924,   557,   890,
     557,   814,  1105,  1925,  1419,  1420,   815,   338,  1688,  1492,
    1116,  1137,  1382,  1813,   287,  1400,  1428,  1814,  1815,   816,
     817,   818,   819,  1280,  1908,  1085,  1119,   835,  1038,  1807,
      87,    88,   606,   263,  1143,   827,  1040,   828,   263,   297,
     263,   814,   263,   831,   611,   832,   815,  1042,  1385,   570,
     261,   111,   883,  1842,   163,   164,  1663,   174,   243,  1158,
     316,   112, -1073,  1384,   243, -1077,   311,   905,  1135,   906,
    1168,   309,   927,  1545,   928,   312,   557,  1408,  1408,   174,
     317,   174,  1462,   254,   255,  1238,   113,  1281,   263,   318,
     263,  1491,  1806,   820,   423,  1077,   624,   424,  1112,  1228,
    1113,  1229,  1186,  1189,   324,  1410,  1410,   816,   817,   818,
     819,  1411,  1411,  1412,  1412,  1201,   263,  1971,   272,  1973,
     325,  1413,  1413,  1194,   987,  1737,  1233,  1977,   164,  1979,
    1082,   820,  1635,  1345,  1197,  1198,  1414,  1414,  1224,   425,
    1209,   345,   263,   263,  1676,  1346,   329,  1347,  1232,  1685,
     436,   361,   272,   272,  1526,   272,   814,  1190,  1790,  1792,
     749,   815,   394,  1228,   613,  1396,   393,  1202,   482,   483,
     484,  1913,  1791,  1793,  2012,  1914,   816,   817,   818,   819,
     816,   817,   818,   819,   437,   174,   438,  1309,   814,   396,
    1923,  1219,  1220,   815,  1222,   174,   492,   493,   400,   632,
     814,   633,   421,   749,  1246,   815,   416,  1927,   439,  1442,
     747,  1443,  1887,   433,   816,   817,   818,   819,  1079,  1993,
    1994,  1331,   814,   432,  1447,  1081,  1448,   815,   816,   817,
     818,   819,  1342,   440,  1550,  1332,   816,   817,   818,   819,
     820,   435,  1622,  1333,   816,   817,   818,   819,   820,   444,
    1708,  1334,   346,  1752,   455,   820,  1330,  1747,   457,   459,
     816,   817,   818,   819,  1671,   462,   820,  1335,  1801,  1031,
    2001,  1035,  2002,   892,   251,  1039,  1041,  1043,   831,  1603,
    2003,  1137,  1606,   611,  1630,   347,  1632,  -735,  1964,   463,
     558,  1966,   562,   565,  1692,   572,   576,   856,   573,   577,
     816,   817,   818,   819,   579,   578,   580,  1523,   582,   595,
     597,   348,   598,   618,   349,  1491,  1491,   570,   570,   607,
     814,   883,   620,  1491,  1491,   815,  1491,  1491,  1991,   715,
     350,   243,   719,   621,  1491,  1491,  1491,  1491,   622,  1491,
    1491,  1491,  1491,  1491,  1491,   626,   628,    14,  -735,   814,
     640,  -735,   641,  1951,   815,   642,   351,   814,  -735,   644,
    -735,  1955,   815,   653,  -735,   664,   667,   677,   678,  -735,
     814,   682,  -735,   733,   737,   815,  1079,   739,  1079,   814,
     774,   776,   777,  1081,   815,  1081,  1079,  -735,  1079,   796,
     778,   813,  1903,  1081,  1082,  1081,   780,   779,   781,   261,
     782,  -735,   613,   561,   783,   784,   272, -1059,   786,  -735,
     785,  1440,   811,  -735,   787,   816,   817,   818,   819,   272,
    1327,   820,  -735,  1441,   788,   272,   793,   794,  1440,   826,
     795,   797,   798,  1079,   799,  1616,  1616,  1620,  -735,   263,
    1081,   800,   801,   263,   802,  1457,   830,   816,   817,   818,
     819,   821,   822, -1077,  1524,   829,   847,   853,  1473,   816,
     817,   818,   819,   814,  1477,  -735,  1527,   872,   815,   877,
    1646,   879,  1648,   855,   880,   868,   884,  -735,   885,  -735,
     876,   816,   817,   818,   819,   891,  1328,   888,   925,  1271,
     814,  1275,   635,  1283,  1286,   815,  1291,  1294,  1297,   814,
    1607,   894,   895,  1317,   815,   897,   898,   814,   902,   907,
     931,   174,   815,   932,   933,  -438,  1605,   557,   938,  1522,
     261,   611,  1554,  1846,   330,   946,  -735,   947,   814,   954,
     952,   953,  1691,   815,  1604,  1608,   955,   957,  1498,   958,
     963,   967,   814,   974,   968,   975,  -735,   815,   984,   987,
    -735,   814,   986,  1982,   263,   992,   815,   263,   994,   993,
    1559,  1602,   998,  1058,   570,   243,   248,   570,   280,   153,
     814,   187,  1063,  -735,  -735,   815,  -735,  1074,   964,   816,
     817,   818,   819,   332,  1748,   570,   570,  1101,  1106,  1107,
     243,   814, -1102,  1110,  1108,  1114,   815,   838, -1102,  1131,
    1148,  1153,  1138, -1102,   831,  1155, -1102,  1165,   816,   817,
     818,   819,  1175,  1176, -1103,  1528,   816,   817,   818,   819,
   -1103,  1177,  1717,  1530,  1182, -1103,   910,   613, -1103,   816,
     817,   818,   819,  1185,  1183,  1191,  1531,  1192,   816,   817,
     818,   819, -1102,  1697,  1200,  1532,   814,  1193,  1702,  1207,
   -1102,   815,  1217,  1226,  1221,  1308,   814,  1240, -1102,  1264,
    1674,   815,  1299,  1300, -1103,   814,  1306,  1316,  1342,  1926,
     815, -1104, -1103,  1343,   814,  1348, -1102, -1104, -1102,  1350,
   -1103,  1369, -1104,  1366,  1374, -1104,  1549,  1375,  1802,  1387,
    1389, -1102,   814, -1102, -1102, -1102,  1401,   815, -1103,  1379,
   -1103,   814,  1436,  1444,  1424,  1404,   815,  1426,   519,  1446,
    1459,  1452,  1455, -1103,  1755, -1103, -1103, -1103,  1456,  1458,
    1234, -1104,   816,   817,   818,   819,  1467,  1475,  1480, -1104,
    1481,   585,  1482,   174,  1007,  1488,  1489, -1104,  1496,  1497,
     593,  1522, -1102,  1493,  1499,  1500,   611,  1507,  1508,   816,
     817,   818,   819,  1539,  1509, -1104,  1533, -1104,   816,   817,
     818,   819,  1510,  1512, -1103,  1534,   816,   817,   818,   819,
   -1104,   236, -1104, -1104, -1104,  1513,  1541,  1514,  1515,  1762,
    1246,  1246,  1516,  1609,  1628,  1517,  1602,   816,   817,   818,
     819,  1625,  1626,  1611,  1535,  1612,   631,  1633,  1634,  1637,
     634,   816,   817,   818,   819,  1638,  1661,  1647,  1738,   814,
     816,   817,   818,   819,   815,  1665,  1666,  1739,  1667,  1668,
    1669, -1104,   570,   570,  1670,  1825,  1673,  1204,  1687,   816,
     817,   818,   819,   814,  1082,  1693,  1746,   814,   815,  1695,
    1710,  1696,   815,  1730,   243,   243,  1744,   363,  1753,  1905,
     816,   817,   818,   819, -1102, -1102, -1102, -1102, -1102, -1102,
   -1102,  1701,  1616,  1754, -1065,  1706, -1102,  1758, -1102,  1729,
    1736,  1740,   174,   156,  1745,  1757, -1103, -1103, -1103, -1103,
   -1103, -1103, -1103,  1766,  1760,  1782,  -339,  1771, -1103,  1789,
   -1103,  1772,  1773,  1774,  1775,  1776,  1940,  1794,  1779,   790,
    1780,  1781,  1783,  1874,  1784,   816,   817,   818,   819,   856,
    1788,   803,   804,   807,  1785,   816,   817,   818,   819,  1795,
    1786,  1787,  1749,  1798,   816,   817,   818,   819,  1816,  1826,
    1828,  1750,  1829, -1104, -1104, -1104, -1104, -1104, -1104, -1104,
    1921,   174,  1830,  1832,   240, -1104,  1831, -1104,  1833,  1522,
    1837,   816,   817,   818,   819,  1835,  1836,  1845,  1879,  1847,
     816,   817,   818,   819,  1851,  1767,   561,  1936,  1852,   814,
    1853,  1856,  1857,  1873,   815,  1009,  1246,  1858,  1859,  1884,
    1917,   814,  1901,  1912,   850,  1919,   815,  1862,   852,  1010,
    1934,  1899,  1082,  1863,  1082,  1864,  1922,  1865,  1898,   613,
    1866,  1918,  1082,  1867,  1082,  1868,   263,  1869,   363,   243,
    1870,  1871,  1930,  1876,  1915,  1929,  1939,  1942,  1949,  1931,
    1950,  1953,  1956,  1960,   613,  1011,  1967,  1085,  1997,  1968,
    1972,  1969,   174,  1012,  1959,  1987,  1989,   240,  1992,  2000,
    1995,  1013,  1984,  2010,  1985,  1943,  2011,  2013,  2014,  1082,
    2017,  2026,   613,  2027,   326,   327,   846,   564,  1613,  1014,
     848,  1015,  1476,   613,  1117,   571,   613,   328,   816,   817,
     818,   819,  1109,   613,  1016,  1239,  1017,  1018,  1019,   985,
    1474,  1464,   645,  1244,   240,  1213,   283,   690,  1839,  1403,
    1149,   948,   816,   817,   818,   819,   816,   817,   818,   819,
     616,  1465,   236,  1945,  1180,  1824,  1470,  1838,  1818,   660,
     662,  1946,  1983,     1,  1990,  2009,  1352,  1988,   611,  1173,
    1896,    28,  1975,  1098,  1809,  1020,  1642,   561,   652,   930,
    1684,     2,   971,   668,  1218,   643,  1210,   162,   608,     3,
    1139,   863,   867,   611,   343,  1133,     4,  1378,     5,   293,
       6,  1631,  1957,  1027,  1872,     7,  1735,  1006,  1602,  1154,
    1970,    48,     8,    49,  1135,    52,  1115,   263,  1373,   875,
    1503,   611,  1651,  1543,  1203,  1453,     9,  1032,    68,   240,
     240,  1037,   611,  1602,  1916,   611,    10,   298,  1053,    69,
     926,  1761,   611,  1060,  1061,     0,    70,  1064,  1065,  1066,
    1067,  1068,  1072,     0,     0,     0,     0,     0,     0,    71,
       0,  1602,  1135,   519,     0,  1086,  1087,  1088,  1089,  1090,
    1091,   519,  1602,     0,  1246,  1602,     0,     0,   816,   817,
     818,   819,  1602,     0,     0,    56,     0,  1021,  1022,  1023,
     816,   817,   818,   819,     0,     0,    57,  1076,     0,    11,
       0,     0,     0,    58,     0,   593,    12,    59,     0,    13,
       0,    14,    15,     0,     0,     0,    72,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,  1051,     0,  1146,    60,     0,     0,
       0,     0,    73,     0,     0,     0,    16,     0,   631,  1259,
    1157,     0,  1260,     0,     0,     0,   467,     0,    74,     0,
       0,     0,     0,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,   469,     0,    76,     0,     0,
      77,   470,   471,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,    79,     0,   472,
      18,    19,   473,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,    63,    20,     0,   474,     0,   475,
     476,     0,     0,     0,     0,     0,     0,     0,     0,   477,
       0,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,     0,  1257,
    1258,  1263,     0,   659,  1272,  1273,  1276,     0,  1284,  1287,
    1289,  1292,  1295,  1298,     0,     0,     0,     0,     0,     0,
    1302,  1303,     0,  1305,     0,  1307,     0,     0,  1310,  1311,
    1312,  1313,  1314,  1315,     0,     0,  1261,     0,   479,     0,
       0,     0,     0,     0,   480,  1319,     0,     0,  1322,     0,
       0,     0,     0,    64,   481,     0,     0,     0,     0,     0,
       0,     0,     0,  1339,     0,     0,     0,     0,   482,   483,
     484,   485,   254,   255,   115,     0,   116,   117,   118,   119,
       0,   120,   486,     0,     0,     0,     0,     0,     0,   121,
     122,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     123,   496,     0,   125,   519,     0,   497,   498,     0,   499,
     500,   501,     0,   128,   129,   502,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   503,   140,     0,   504,   142,   143,
       0,   144,     0,   505,   506,   507,     0,   508,   509,     0,
     510,   145,     0,     0,     0,   511,   512,     0,     0,  1009,
       0,   584,     0,   514,     0,   814,     0,   332,     0,     0,
     815,   114,     0,  1010,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,     0,     0,     0,     0,   470,  1011,
     126,   127,     0,   128,   129,     0,   130,  1012,     0,   131,
     132,   133,   134,   135,     0,  1013,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,   472,   141,   142,   143,
       0,   144,     0,  1014,     0,  1015,     0,     0,     0,     0,
       0,   145,     0,     0,   519,     0,     0,     0,  1016,   910,
    1017,  1018,  1019,     0,   474,     0,     0,  1501,  1502,     0,
    1504,  1506,     0,     0,     0,     0,     0,     0,  1511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,  1529,    92,
       0,     0,     0,    93,    94,     0,  1536,  1537,  1319,  1020,
       0,     0,    95,     0,  1319,     0,  1322,  1544,   519,  1546,
       0,  1704,  1705,    96,     0,     0,  1547,     0,  1548,  1711,
    1712,     0,  1714,  1715,     0,   479,     0,     0,     0,     0,
    1719,  1720,  1721,  1722,     0,  1723,  1724,  1725,  1726,  1727,
    1728,   481,     0,     0,  1270,     0,     0,  1260,     0,     0,
       0,   467,     0,     0,     0,   482,   483,   484,   468,   254,
     255,     0,     0,     0,     0,     0,     0,   519,     0,     0,
     469,     0,     0,     0,     0,    97,   470,   471,   487,   488,
     489,   490,   491,   492,   493,   494,   495,     0,     0,     0,
       0,    98,     0,   497,   498,     0,     0,     0,     0,  1655,
       0,     0,   502,     0,   472,     0,    99,   473,     0,     0,
     100,  1021,  1022,  1023,   816,   817,   818,   819,     0,     0,
     114,   101,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,   474,     0,   475,   476,     0,     0,   121,   122,
     102,     0,     0,  1406,   477,     0,     0,     0,   478,   123,
     124,     0,   125,     0,     0,     0,     0,     0,     0,   126,
     127,     0,   128,   129,     0,   130,     0,  1703,   131,   132,
     133,   134,   135,  1709,     0,     0,   136,   137,  1713,     0,
       0,     0,   138,   139,   140,  1718,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,   103,
     145,  1261,     0,   479,     0,     0,     0,     0,     0,   480,
     312,     0,     0,     0,     0,     0,     0,     0,     0,   481,
    1319,  1319,     0,     0,     0,     0,     0,     0,     0,  1751,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,  1759,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,     0,   508,   509,     0,   510,   145,  1805,     0,     0,
     511,   512,  1274,     0,     0,  1260,   584,     0,   514,   467,
       0,     0,   332,     0,     0,     0,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   470,   471,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,     0,     0,
    1855,     0,     0,     0,     0,     0,  1860,  1861,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     474,     0,   475,   476,     0,     0,     0,     0,     0,     0,
    1875,     0,   477,     0,     0,  1319,   478,     0,  1878,     0,
       0,     0,     0,     0,     0,  1885,  1886,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1261,
       0,   479,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   665,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,   483,   484,   485,   254,   255,   115,     0,   116,
     117,   118,   119,     0,   120,   486,  1928,     0,     0,     0,
       0,     0,   121,   122,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   123,   496,     0,   125,     0,     0,   497,
     498,     0,   499,   500,   501,     0,   128,   129,   502,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   503,   140,     0,
     504,   142,   143,     0,   144,     0,   505,   506,   507,     0,
     508,   509,  1805,   510,   145,     0,     0,     0,   511,   512,
    1282,     0,     0,  1260,   584,     0,   514,   467,     0,     0,
     332,     0,     0,     0,   468,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,  1976,
       0,     0,   470,   471,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   474,     0,
     475,   476,     0,     0,     0,   123,   124,     0,   125,     0,
     477,     0,     0,     0,   478,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1261,     0,   479,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,     0,  1094,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   482,
     483,   484,   485,   254,   255,   115,     0,   116,   117,   118,
     119,     0,   120,   486,     0,     0,     0,     0,     0,     0,
     121,   122,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   123,   496,     0,   125,     0,     0,   497,   498,     0,
     499,   500,   501,     0,   128,   129,   502,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   503,   140,     0,   504,   142,
     143,     0,   144,     0,   505,   506,   507,  1285,   508,   509,
    1260,   510,   145,     0,   467,     0,   511,   512,     0,     0,
       0,   468,   584,     0,   514,     0,     0,     0,   332,     0,
       0,     0,     0,   469,     0,     0,     0,     0,     0,   470,
     471,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   472,     0,     0,
     473,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   474,     0,   475,   476,     0,
       0,     0,   123,   124,     0,   125,     0,   477,     0,     0,
       0,   478,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,  1095,
       0,     0,     0,     0,  1261,     0,   479,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,   483,   484,   485,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     486,     0,     0,     0,     0,     0,     0,   121,   122,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   123,   496,
       0,   125,     0,     0,   497,   498,     0,   499,   500,   501,
       0,   128,   129,   502,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   503,   140,     0,   504,   142,   143,     0,   144,
       0,   505,   506,   507,  1290,   508,   509,  1260,   510,   145,
       0,   467,   988,   511,   512,   692,     0,     0,   468,   584,
       0,   514,     0,     0,     0,   332,     0,     0,   989,     0,
     469,     0,   693,     0,     0,   694,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,   695,     0,     0,     0,     0,
       0,   696,   697,   698,     0,   699,   700,   701,     0,   702,
       0,     0,   474,     0,   475,   476,     0,     0,     0,     0,
       0,     0,     0,     0,   477,     0,     0,   703,   478,   704,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   705,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   706,     0,     0,     0,     0,
       0,  1261,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,   707,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,   708,   709,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,   710,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,  1293,   508,   509,  1260,   510,   145,     0,   467,   691,
     511,   512,   692,     0,     0,   468,   584,     0,   514,     0,
       0,     0,   332,     0,     0,     0,     0,   469,     0,   693,
       0,     0,   694,   470,   471,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   472,     0,     0,   473,     0,     0,     0,     0,     0,
       0,     0,   695,     0,     0,     0,     0,     0,   696,   697,
     698,     0,   699,   700,   701,     0,   702,     0,     0,   474,
       0,   475,   476,     0,     0,     0,     0,     0,     0,     0,
       0,   477,     0,     0,   703,   478,   704,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   705,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   706,     0,     0,     0,     0,     0,  1261,     0,
     479,     0,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,     0,     0,   707,     0,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     482,   483,   484,   485,   254,   255,   115,     0,   116,   117,
     118,   119,     0,   120,   486,     0,     0,     0,     0,   708,
     709,   121,   122,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   123,   496,     0,   125,   710,     0,   497,   498,
       0,   499,   500,   501,     0,   128,   129,   502,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   503,   140,     0,   504,
     142,   143,     0,   144,     0,   505,   506,   507,  1296,   508,
     509,  1260,   510,   145,     0,   467,     0,   511,   512,     0,
       0,     0,   468,   584,     0,   514,     0,     0,     0,   332,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   566,   472,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,   474,     0,   475,   476,
       0,     0,   121,   122,     0,     0,     0,     0,   477,     0,
       0,     0,   478,   123,   124,     0,   125,     0,     0,     0,
       0,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,   107,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,  1261,     0,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
     108,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,   109,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,   110,     0,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,     0,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,     0,
     144,     0,   505,   506,   507,   467,   508,   509,     0,   510,
     145,   111,   468,     0,   511,   512,     0,     0,     0,     0,
     584,   112,   514,     0,   469,     0,   332,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,   474,     0,   475,   476,
       0,     0,     0,   123,   124,     0,   125,     0,   477,     0,
       0,     0,   478,   126,   127,     0,   128,   129,     0,   130,
     999,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,    14,     0,     0,
       0,   753,     0,     0,     0,     0,     0,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,     0,  1353,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,     0,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     133,   134,   135,  1000,  1001,  1002,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,     0,
     144,     0,   505,   506,   507,   467,   508,   509,     0,   510,
     145,     0,   468,     0,   511,   512,     0,  1354,     0,     0,
     757,     0,   514,     0,   469,     0,   332,   750,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,  1355,     0,     0,     0,     0,  1356,   472,     0,
       0,   473,     0,  1357,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,  1358,     0,   120,     0,   474,     0,   475,   476,
       0,     0,   121,   122,     0,     0,     0,     0,   477,     0,
       0,     0,   478,   123,   124,     0,   125,     0,     0,     0,
     752,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,   753,     0,     0,   145,     0,     0,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,     0,     0,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,  1024,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     754,   755,   756,     0,  1025,     0,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,     0,
     144,     0,   505,   506,   507,   467,   508,   509,     0,   510,
     145,     0,   468,     0,   511,   512,     0,     0,     0,     0,
     757,     0,   514,     0,   469,     0,   332,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
    1026,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,   474,     0,   475,   476,
       0,     0,   121,   122,     0,     0,     0,     0,   477,     0,
       0,     0,   478,   123,   124,     0,   125,     0,     0,     0,
     995,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,   753,     0,     0,   145,     0,     0,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,     0,     0,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,     0,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     754,   755,   756,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,     0,
     144,     0,   505,   506,   507,   467,   508,   509,     0,   510,
     145,     0,   468,     0,   511,   512,     0,     0,     0,     0,
     757,    14,   514,     0,   469,     0,   332,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,   474,     0,   475,   476,
       0,     0,     0,   123,   124,     0,   125,     0,   477,     0,
       0,     0,   478,   126,   127,     0,   128,   129,     0,   130,
     752,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,   753,     0,     0,  1120,     0,     0,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,     0,     0,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,     0,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     754,   755,   756,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,     0,
     144,     0,   505,   506,   507,   467,   508,   509,     0,   510,
     145,     0,   468,     0,   511,   512,     0,     0,     0,     0,
     757,     0,   514,     0,   469,     0,   332,     0,     0,     0,
     470,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,   474,     0,   475,   476,
       0,     0,     0,   123,   124,     0,   125,     0,   477,     0,
       0,     0,   478,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1078,     0,   514,   479,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,   483,   484,
     485,   254,   255,   115,     0,   116,   117,   118,   119,     0,
     120,   486,     0,     0,     0,     0,     0,     0,   121,   122,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   123,
     496,     0,   125,     0,     0,   497,   498,     0,   499,   500,
     501,     0,   128,   129,   502,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   503,   140,     0,   504,   142,   143,  1069,
     144,     0,   505,   506,   507,  1070,   508,   509,  1071,   510,
     145,   467,     0,     0,   511,   512,     0,     0,   468,     0,
     584,     0,   514,     0,     0,     0,   332,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,   995,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,   753,     0,     0,
       0,     0,   514,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   757,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1120,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,   589,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,  -340,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,   789,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,    83,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,    14,     0,    84,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,  1095,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,  1092,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   474,     0,   475,   476,     0,     0,     0,   123,
     124,     0,   125,     0,   477,     0,     0,     0,   478,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1288,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1338,     0,     0,   472,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,     0,   475,   476,     0,     0,     0,     0,
       0,     0,     0,     0,   477,     0,     0,     0,   478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,     0,   475,   476,     0,     0,     0,     0,
       0,     0,     0,     0,   477,     0,     0,     0,   478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1505,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   584,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,     0,   475,   476,     0,     0,     0,     0,
       0,     0,     0,     0,   477,     0,     0,     0,   478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,   467,   508,   509,     0,   510,   145,     0,   468,     0,
     511,   512,     0,     0,     0,     0,   513,     0,   514,     0,
     469,     0,   332,     0,     0,     0,   470,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   474,     0,   475,   476,     0,     0,     0,     0,
       0,     0,     0,     0,   477,     0,     0,     0,   478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   483,   484,   485,   254,   255,   115,
       0,   116,   117,   118,   119,     0,   120,   486,     0,     0,
       0,     0,     0,     0,   121,   122,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   123,   496,     0,   125,     0,
       0,   497,   498,     0,   499,   500,   501,     0,   128,   129,
     502,   130,     0,  1350,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   503,
     140,     0,   504,   142,   143,     0,   144,     0,   505,   506,
     507,     6,   508,   509,     0,   510,   145,     0,  -294,     0,
     511,   512,  1560,  1561,     0,  1562,   584,     0,   514,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,  1563,     0,     0,     0,     0,     0,  1564,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,     0,     0,  1565,     0,     0,  -296,     0,     0,
       0,  1560,  1561,     0,  1562,     0,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
    1563,     0,     0,     0,     0,     0,  1564,  1566,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,  1767,     0,     0,     0,
       0,     0,     0,     0,     0,   485,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,  1565,  1567,   124,     0,   125,     0,     0,
       0,     0,     0,     0,   126,   127,     0,   128,   129,     0,
     130,    14,     0,   131,  1568,   133,   134,   135,     0,     0,
       0,   136,   137,  1569,  1570,  1571,  1566,   138,   139,   140,
       0,   141,   142,   143,     0,   144,    16,  1350,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,   485,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     6,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,  1560,  1561,     0,  1562,
       0,     0,     0,  1567,   124,     0,   125,     0,     0,     0,
       0,     0,     0,   126,   127,  1563,   128,   129,     0,   130,
       0,  1564,   131,  1568,   133,   134,   135,     0,     0,     0,
     136,   137,  1569,  1570,  1571,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,   691,     0,     0,
     692,     0,     0,     0,     0,     0,   910,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   693,  1565,     0,
     694,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1566,     0,     0,     0,     0,     0,     0,     0,     0,
     695,    16,     0,     0,     0,     0,   696,   697,   698,     0,
     699,   700,   701,     0,   702,     0,     0,     0,     0,   485,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,   703,     0,   704,     0,     0,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1567,   124,
       0,   125,     0,     0,     0,     0,     0,     0,   126,   127,
       0,   128,   129,   705,   130,     0,     0,   131,  1568,   133,
     134,   135,     0,     0,     0,   136,   137,  1569,  1570,  1571,
     706,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,   707,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
     691,     0,     0,   692,     0,   121,   122,   708,   709,     0,
       0,     0,     0,     0,     0,     0,   123,   124,     0,   125,
     693,     0,     0,   694,   710,     0,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,   695,     0,     0,     0,   145,     0,   696,
     697,   698,     0,   699,   700,   701,     0,   702,     0,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   703,     0,   704,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,   377,   378,     0,     0,     0,   705,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,   706,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   707,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,   381,     0,     0,   121,   122,
     708,   709,     0,   382,     0,     0,     0,     0,     0,   123,
     124,     0,   125,     0,     0,     0,     0,   710,     0,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,   383,     0,   136,   137,   384,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,   385,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,   386,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,   114,     0,
     389,   115,   191,   116,   117,   118,   119,   192,   120,   193,
     194,   195,   196,   197,   198,   199,   121,   122,   200,   201,
     202,   203,   204,     0,     0,   205,   206,   123,   124,   207,
     125,     0,     0,   208,   209,   210,   211,   126,   127,   212,
     128,   129,   213,   130,   214,   215,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,   216,   217,   218,   219,
     138,   139,   140,   220,   141,   142,   143,   221,   144,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   145
};

static const short yycheck[] =
{
       9,     0,     4,   626,    70,   173,    62,    16,   113,    18,
      60,   163,   164,   163,   164,    16,    68,    76,   410,    75,
      69,    61,    69,   789,   101,   319,    92,   811,   445,   795,
     441,   445,   553,    73,   811,  1078,    79,    64,   513,    74,
     661,   100,   412,   631,   874,  1102,    96,   775,    57,    58,
    1350,    60,    61,    62,    63,    64,   574,    97,  1029,   102,
      69,    70,    71,    98,    73,   105,    75,    76,    95,    78,
      79,    80,   595,   437,   422,   573,   103,  1349,   806,    88,
    1423,    90,    91,    92,   822,    94,    95,    96,    97,    90,
      99,   100,   101,   102,   103,   361,   105,  1208,    99,  1174,
     156,   178,    98,   451,  1075,   829,   155,   334,   157,   175,
     745,   392,  1120,   738,   421,   982,  1368,  1415,  1416,   270,
     513,   177,  1420,    10,    21,  1318,  1230,   182,   609,     8,
     651,  1324,   919,  1421,   655,  1692,   176,     8,   884,   877,
     661,   168,   752,  1166,     6,     8,   155,   156,   157,    43,
    1225,     8,   161,     8,   677,    43,     4,     6,    47,   168,
      63,     8,     8,    82,   173,    19,   175,   176,   177,   178,
      90,  1166,     9,   182,   279,    43,  1388,  1166,   133,  1166,
      61,    26,    62,   235,    46,   149,    26,  1166,    61,    97,
    1107,   584,     8,   361,  1633,    44,    63,    10,     8,     8,
      23,    49,  1166,    44,    47,    28,    44,    56,   141,   880,
      23,    14,   752,   265,   185,    28,   367,   757,    55,    44,
       8,    70,    90,  1575,     8,    18,   637,   196,   126,    66,
     169,    80,   190,   904,   143,    23,   907,     8,    50,    51,
      28,  1007,  1008,   196,   661,   916,   125,   661,  1369,   230,
      63,    68,    23,   174,   125,    80,    22,    28,    11,   139,
     309,   196,   125,   312,    10,    80,   371,   615,   125,   193,
     125,   663,   566,    30,     5,  1575,   687,    23,   125,   125,
     178,   262,    28,   123,    15,   194,    43,   581,   223,    82,
      63,  1057,    45,   349,  1078,   345,   145,  1569,  1570,   179,
     309,  1078,   105,   312,   975,     3,   155,   174,   156,   125,
     319,   320,   361,   365,  1171,   125,   125,   288,   191,   711,
    1395,  1178,   230,  1444,   288,   294,   174,   260,   165,   287,
    1187,   180,   177,   288,   287,   255,   345,   177,   813,   180,
     349,   125,   180,  1454,    50,    51,   821,  1540,  1541,   288,
     204,   168,   118,   202,   194,   180,  1213,   173,   175,  1798,
     208,  1418,   411,   173,   757,   180,   287,    56,   287,  1926,
    1658,  1379,   421,   854,   423,    23,   425,   189,    76,   428,
      28,   447,   391,   392,   287,   279,   438,   255,   440,   999,
     449,   279,  1415,  1416,   291,  1182,  1183,  1420,   287,  1165,
     287,   457,   411,   455,  1461,   573,   287,  1153,   448,   459,
     289,   932,   421,  1625,   423,    63,   425,  1145,   289,   428,
    1415,  1416,   284,  1394,   287,  1420,  1415,  1416,  1415,  1416,
     141,  1420,   289,  1420,   289,   958,  1415,  1416,   447,   448,
     449,  1420,   289,   289,   287,  1232,    19,    55,   457,    18,
     459,  1415,  1416,   462,    64,   995,  1420,  1374,  1375,   999,
     462,   284,   285,    23,   196,   983,   155,    77,    28,   282,
     283,   284,   285,    55,   287,    85,   994,  1148,   192,  1822,
     998,   196,    46,   189,  1250,  1251,    23,  1253,  1254,   196,
     100,    28,    85,  1765,   282,   283,   284,   285,  1169,  1170,
      21,  1953,   196,   895,   114,   514,   898,   563,   223,   244,
      44,   282,   283,   284,   285,   108,   572,   117,  1189,    64,
     576,    94,   914,   196,   573,   565,   287,   142,   583,   679,
      94,   292,    77,   141,   122,   246,   282,   283,   284,   285,
      23,   151,  1794,   158,   838,    28,    80,   939,    90,  2001,
     223,   283,  1745,    22,   563,   100,   565,   566,  1342,   141,
     286,    30,    94,   572,   172,  1342,    87,   576,   283,   114,
     622,   164,   581,   287,   583,  1918,   283,   626,   729,   187,
      61,  1438,  1880,  1881,  1641,  1689,   186,   639,     3,   283,
    1701,    44,   117,  1460,   176,  1947,  1671,   196,   905,  1224,
     294,   653,  1120,  1238,   885,  1343,   151,   873,    98,  1232,
     283,   145,   661,  1688,  1112,   196,   986,   626,  1961,    79,
    1972,   155,  1346,    65,   851,   973,  1634,    80,    23,    64,
     682,   156,  1150,    28,   282,   283,   284,   285,  1404,  1114,
    1983,   289,    77,   811,   155,   680,   180,  1947,  2000,   118,
     659,     4,   661,   263,   114,    46,   665,   199,   200,  2011,
    1426,    76,  2014,    16,    43,   100,   126,   199,   200,  2021,
     139,   182,  1972,   147,   826,   727,   145,   167,   203,   114,
     231,   196,   734,   735,   680,    11,     7,   122,   737,   288,
     241,   237,     6,  1646,     6,   294,    49,    23,    51,   118,
    2000,    23,    28,    94,     6,   873,    28,   288,   223,   859,
     179,  2011,   196,   294,  2014,    94,   151,    17,   263,    45,
     139,  2021,   282,   283,   284,   285,   145,   118,   737,   289,
      44,    31,    23,     6,    46,    23,  1707,    28,  1691,  1782,
      28,  2013,    56,   289,    46,   282,   283,   284,   285,    23,
     335,   297,   289,   780,    28,   143,    64,    23,   343,   287,
     179,    65,    28,  1491,   292,    23,    80,   287,     6,    77,
      28,   780,   292,    46,   162,    75,     6,  1834,   827,   174,
     829,    23,   831,  1840,  1176,  1177,    28,     6,  1459,  1264,
     846,   941,   100,  1650,   196,  1159,  1188,  1654,  1655,   282,
     283,   284,   285,   103,   239,   814,   289,   847,    46,  1639,
      59,    60,   868,   822,   870,   287,    46,   289,   827,    65,
     829,    23,   831,   287,   873,   289,    28,    46,   263,   838,
     879,   145,   884,  1690,   231,   232,  1424,   846,   847,   891,
     122,   155,   290,   151,   853,   293,   140,   287,   857,   289,
     902,   288,   203,  1328,   205,   288,   905,  1880,  1881,   868,
     287,   870,  1226,   199,   200,  1235,   180,   167,   877,   122,
     879,  1264,  1638,   287,     4,   289,   885,     7,   287,   287,
     289,   289,   931,   932,   124,  1880,  1881,   282,   283,   284,
     285,  1880,  1881,  1880,  1881,   947,   905,  1940,   933,  1942,
     124,  1880,  1881,   938,   287,  1526,   289,  1950,   232,  1952,
    1078,   287,  1387,   289,   130,   131,  1880,  1881,   974,    49,
     955,    79,   931,   932,  1447,   287,   174,   289,   984,  1452,
      98,   156,   967,   968,  1304,   970,    23,   933,  1611,  1612,
     949,    28,   192,   287,  1112,   289,   180,   949,   195,   196,
     197,  1808,  1611,  1612,  1997,  1812,   282,   283,   284,   285,
     282,   283,   284,   285,   132,   974,   134,   289,    23,    19,
    1827,   967,   968,    28,   970,   984,   223,   224,   259,   397,
      23,   399,   288,   992,   993,    28,   198,  1844,   156,   287,
     992,   289,  1758,   162,   282,   283,   284,   285,  1782,  1968,
    1969,   289,    23,   266,   287,  1782,   289,    28,   282,   283,
     284,   285,   287,   181,   289,   289,   282,   283,   284,   285,
     287,   199,   289,   289,   282,   283,   284,   285,   287,    10,
     289,   289,    84,  1554,    59,   287,  1063,   289,    61,    79,
     282,   283,   284,   285,  1436,    78,   287,   289,   289,   776,
     287,   778,   289,  1464,  1063,   782,   783,   784,   287,  1353,
     289,  1211,  1356,  1112,  1382,   117,  1384,     0,  1925,   190,
     149,  1928,   172,   141,  1466,    82,    63,    10,   288,    61,
     282,   283,   284,   285,   122,    61,   122,   289,   244,   174,
     102,   143,   188,   123,   146,  1488,  1489,  1106,  1107,   172,
      23,  1153,   156,  1496,  1497,    28,  1499,  1500,  1965,  1526,
     162,  1120,  1526,    65,  1507,  1508,  1509,  1510,    83,  1512,
    1513,  1514,  1515,  1516,  1517,   121,   243,   155,    61,    23,
     254,    64,   255,  1899,    28,   254,   188,    23,    71,    30,
      73,  1907,    28,    52,    77,   207,   287,   192,   279,    82,
      23,   112,    85,    29,   288,    28,  1940,    10,  1942,    23,
     135,   288,   288,  1940,    28,  1942,  1950,   100,  1952,    97,
     288,   291,  1795,  1950,  1342,  1952,   288,   199,   288,  1228,
     288,   114,  1350,  1232,   288,   288,  1221,   288,   199,   122,
     288,  1200,   148,   126,   199,   282,   283,   284,   285,  1234,
     287,   287,   135,  1205,   288,  1240,   288,   288,  1217,   279,
     288,   288,   288,  1997,   288,  1367,  1368,  1369,   151,  1228,
    1997,   288,   288,  1232,   288,  1221,    10,   282,   283,   284,
     285,   288,   293,   293,   289,   288,   141,    63,  1234,   282,
     283,   284,   285,    23,  1240,   178,   289,   166,    28,   293,
    1400,   288,  1402,   287,    10,   287,    83,   190,   293,   192,
     279,   282,   283,   284,   285,    90,   287,   287,  1660,  1014,
      23,  1016,    90,  1018,  1019,    28,  1021,  1022,  1023,    23,
    1357,    90,    22,    63,    28,    22,    43,    23,   111,    10,
     174,  1300,    28,   208,   136,   174,  1355,  1346,   136,  1300,
    1349,  1350,  1349,  1695,   196,    21,   239,    90,    23,   138,
     185,   185,  1462,    28,  1354,  1358,   288,    91,    54,   288,
     291,   288,    23,    61,   288,    10,   259,    28,   122,   287,
     263,    23,   149,  1956,  1343,   128,    28,  1346,   288,   121,
    1349,  1350,   288,   189,  1353,  1354,  1355,  1356,  1357,  1358,
      23,   126,    61,   286,   287,    28,   289,   289,    21,   282,
     283,   284,   285,   294,   287,  1374,  1375,    42,   174,   174,
    1379,    23,    17,    60,   202,   288,    28,   287,    23,   190,
      10,    83,   186,    28,   287,   240,    31,   288,   282,   283,
     284,   285,    19,   288,    17,   289,   282,   283,   284,   285,
      23,    43,    54,   289,    86,    28,    27,  1575,    31,   282,
     283,   284,   285,    22,    86,   125,   289,   125,   282,   283,
     284,   285,    67,  1475,   156,   289,    23,   125,  1480,   154,
      75,    28,   156,    98,   288,   287,    23,   288,    83,   288,
    1442,    28,   108,   288,    67,    23,   289,    63,   287,  1841,
      28,    17,    75,   293,    23,   184,   101,    23,   103,    16,
      83,   107,    28,   124,    63,    31,    63,    63,  1636,   288,
      71,   116,    23,   118,   119,   120,    12,    28,   101,   287,
     103,    23,    10,   160,   239,   288,    28,   288,   308,   289,
      10,   290,   289,   116,  1560,   118,   119,   120,   289,   289,
     133,    67,   282,   283,   284,   285,   192,    52,    52,    75,
     289,   331,   289,  1522,     8,   288,   288,    83,   288,   288,
     340,  1522,   167,    63,   288,   288,  1575,   288,   288,   282,
     283,   284,   285,    51,   288,   101,   289,   103,   282,   283,
     284,   285,   288,   288,   167,   289,   282,   283,   284,   285,
     116,  1560,   118,   119,   120,   288,   173,   288,   288,  1568,
    1569,  1570,   288,   192,   127,   288,  1575,   282,   283,   284,
     285,   289,   289,   287,   289,   287,   396,    85,    85,    19,
     400,   282,   283,   284,   285,    73,   145,   130,   289,    23,
     282,   283,   284,   285,    28,   125,   125,   289,   125,   125,
     125,   167,  1611,  1612,   125,  1661,   289,   196,   289,   282,
     283,   284,   285,    23,  1782,    22,   289,    23,    28,   288,
      63,   289,    28,   287,  1633,  1634,    51,  1636,   286,  1797,
     282,   283,   284,   285,   279,   280,   281,   282,   283,   284,
     285,   289,  1794,    36,    36,   289,   291,   288,   293,   289,
     289,    61,  1661,    82,   173,   286,   279,   280,   281,   282,
     283,   284,   285,    51,   286,    82,   286,   286,   291,    70,
     293,   286,   286,   286,   286,   286,    82,   174,   286,   499,
     286,   286,   286,  1733,   286,   282,   283,   284,   285,    10,
     290,   511,   512,   513,   286,   282,   283,   284,   285,   278,
     286,   286,   289,    85,   282,   283,   284,   285,    23,    79,
     180,   289,    80,   279,   280,   281,   282,   283,   284,   285,
    1825,  1730,   180,    80,  1733,   291,    44,   293,    44,  1730,
      94,   282,   283,   284,   285,   289,   289,   124,   289,   111,
     282,   283,   284,   285,   111,   189,  1795,   289,   289,    23,
     289,   289,   289,   288,    28,    17,  1765,   289,   289,    61,
      94,    23,   124,   287,   584,  1821,    28,   289,   588,    31,
      79,   288,  1940,   289,  1942,   289,  1826,   289,  1777,  1947,
     289,    86,  1950,   289,  1952,   289,  1795,   289,  1797,  1798,
     289,   289,   204,   289,   289,  1847,   288,    82,   246,  1851,
      82,   288,   288,   122,  1972,    67,   289,  1816,    82,   287,
     173,   288,  1821,    75,  1919,   122,   122,  1826,   289,    47,
     289,    83,    44,   289,   180,  1891,    50,    36,    47,  1997,
     201,     0,  2000,     0,   160,   160,   579,   317,  1365,   101,
     581,   103,  1238,  2011,   847,   320,  2014,   161,   282,   283,
     284,   285,   838,  2021,   116,   987,   118,   119,   120,   741,
    1235,  1227,   408,   992,  1873,   962,    80,   442,  1679,  1162,
     879,   687,   282,   283,   284,   285,   282,   283,   284,   285,
     369,  1228,  1891,  1892,   915,  1660,  1232,  1678,  1658,   423,
     423,  1893,  1959,     7,  1963,  1985,  1103,  1962,  1947,   905,
    1769,     0,  1948,   827,  1642,   167,  1397,  1956,   413,   667,
    1450,    25,   724,   428,   965,   406,   957,    13,   353,    33,
     868,   597,   601,  1972,   166,   855,    40,  1120,    42,    90,
      44,  1383,  1912,   774,  1730,    49,  1522,   757,  1947,   888,
    1939,     0,    56,     0,  1953,     0,   845,  1956,  1115,   609,
    1279,  2000,  1406,  1326,   949,  1217,    70,   777,    37,  1968,
    1969,   781,  2011,  1972,  1816,  2014,    80,    94,   788,    48,
     665,  1568,  2021,   793,   794,    -1,    55,   797,   798,   799,
     800,   801,   802,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,  2000,  2001,   813,    -1,   815,   816,   817,   818,   819,
     820,   821,  2011,    -1,  2013,  2014,    -1,    -1,   282,   283,
     284,   285,  2021,    -1,    -1,    37,    -1,   279,   280,   281,
     282,   283,   284,   285,    -1,    -1,    48,   289,    -1,   143,
      -1,    -1,    -1,    55,    -1,   855,   150,    59,    -1,   153,
      -1,   155,   156,    -1,    -1,    -1,   125,   210,   211,   212,
     213,   214,   215,   216,   217,    -1,   876,    79,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,   180,    -1,   888,     6,
     890,    -1,     9,    -1,    -1,    -1,    13,    -1,   157,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    32,    -1,   176,    -1,    -1,
     179,    38,    39,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,    -1,   229,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    66,
     244,   245,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,    -1,   176,   269,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,    -1,  1009,
    1010,  1011,    -1,    30,  1014,  1015,  1016,    -1,  1018,  1019,
    1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,    -1,    -1,
    1030,  1031,    -1,  1033,    -1,  1035,    -1,    -1,  1038,  1039,
    1040,  1041,  1042,  1043,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,  1055,    -1,    -1,  1058,    -1,
      -1,    -1,    -1,   265,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1073,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,  1114,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    -1,
     277,   278,    -1,    -1,    -1,   282,   283,    -1,    -1,    17,
      -1,   288,    -1,   290,    -1,    23,    -1,   294,    -1,    -1,
      28,   198,    -1,    31,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    38,    67,
     237,   238,    -1,   240,   241,    -1,   243,    75,    -1,   246,
     247,   248,   249,   250,    -1,    83,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    66,   264,   265,   266,
      -1,   268,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   278,    -1,    -1,  1264,    -1,    -1,    -1,   116,    27,
     118,   119,   120,    -1,    94,    -1,    -1,  1277,  1278,    -1,
    1280,  1281,    -1,    -1,    -1,    -1,    -1,    -1,  1288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,  1308,    55,
      -1,    -1,    -1,    59,    60,    -1,  1316,  1317,  1318,   167,
      -1,    -1,    68,    -1,  1324,    -1,  1326,  1327,  1328,  1329,
      -1,  1488,  1489,    79,    -1,    -1,  1336,    -1,  1338,  1496,
    1497,    -1,  1499,  1500,    -1,   165,    -1,    -1,    -1,    -1,
    1507,  1508,  1509,  1510,    -1,  1512,  1513,  1514,  1515,  1516,
    1517,   181,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,   195,   196,   197,    20,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,  1387,    -1,    -1,
      32,    -1,    -1,    -1,    -1,   141,    38,    39,   218,   219,
     220,   221,   222,   223,   224,   225,   226,    -1,    -1,    -1,
      -1,   157,    -1,   233,   234,    -1,    -1,    -1,    -1,  1419,
      -1,    -1,   242,    -1,    66,    -1,   172,    69,    -1,    -1,
     176,   279,   280,   281,   282,   283,   284,   285,    -1,    -1,
     198,   187,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,
     206,    -1,    -1,   283,   106,    -1,    -1,    -1,   110,   227,
     228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,  1487,   246,   247,
     248,   249,   250,  1493,    -1,    -1,   254,   255,  1498,    -1,
      -1,    -1,   260,   261,   262,  1505,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,
     278,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
    1540,  1541,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1549,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,  1565,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,  1637,    -1,    -1,
     282,   283,     6,    -1,    -1,     9,   288,    -1,   290,    13,
      -1,    -1,   294,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,
    1710,    -1,    -1,    -1,    -1,    -1,  1716,  1717,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
    1740,    -1,   106,    -1,    -1,  1745,   110,    -1,  1748,    -1,
      -1,    -1,    -1,    -1,    -1,  1755,  1756,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,  1846,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,  1912,   277,   278,    -1,    -1,    -1,   282,   283,
       6,    -1,    -1,     9,   288,    -1,   290,    13,    -1,    -1,
     294,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,  1949,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,     6,   274,   275,
       9,   277,   278,    -1,    13,    -1,   282,   283,    -1,    -1,
      -1,    20,   288,    -1,   290,    -1,    -1,    -1,   294,    -1,
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
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,   284,
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
      -1,   270,   271,   272,     6,   274,   275,     9,   277,   278,
      -1,    13,    18,   282,   283,    21,    -1,    -1,    20,   288,
      -1,   290,    -1,    -1,    -1,   294,    -1,    -1,    34,    -1,
      32,    -1,    38,    -1,    -1,    41,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,   113,   110,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,   218,   219,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,   235,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,     6,   274,   275,     9,   277,   278,    -1,    13,    18,
     282,   283,    21,    -1,    -1,    20,   288,    -1,   290,    -1,
      -1,    -1,   294,    -1,    -1,    -1,    -1,    32,    -1,    38,
      -1,    -1,    41,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,   113,   110,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,   218,
     219,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,   235,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,     6,   274,
     275,     9,   277,   278,    -1,    13,    -1,   282,   283,    -1,
      -1,    -1,    20,   288,    -1,   290,    -1,    -1,    -1,   294,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    94,    -1,    96,    97,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,     6,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    80,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    13,   274,   275,    -1,   277,
     278,   145,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,
     288,   155,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
     118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,   155,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    71,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    13,   274,   275,    -1,   277,
     278,    -1,    20,    -1,   282,   283,    -1,   141,    -1,    -1,
     288,    -1,   290,    -1,    32,    -1,   294,    35,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,   176,    -1,    -1,    -1,    -1,   181,    66,    -1,
      -1,    69,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    94,    -1,    96,    97,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,
     118,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   278,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    85,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,   108,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    13,   274,   275,    -1,   277,
     278,    -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,
     288,    -1,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    94,    -1,    96,    97,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,
     118,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   278,    -1,    -1,   165,    -1,    -1,
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
     278,    -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,
     288,   155,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
     118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   288,    -1,    -1,   165,    -1,    -1,
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
     278,    -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,
     288,    -1,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,   290,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
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
     278,    13,    -1,    -1,   282,   283,    -1,    -1,    20,    -1,
     288,    -1,   290,    -1,    -1,    -1,   294,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,   118,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,   290,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
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
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,   284,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
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
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,   196,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,    -1,    -1,   155,    -1,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
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
     278,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,   289,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
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
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
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
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
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
     192,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
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
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    13,   274,   275,    -1,   277,   278,    -1,    20,    -1,
     282,   283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
      32,    -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
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
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    16,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    44,   274,   275,    -1,   277,   278,    -1,    51,    -1,
     282,   283,    55,    56,    -1,    58,   288,    -1,   290,    -1,
      -1,    -1,   294,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,   137,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    80,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,   155,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,   256,   257,   258,   170,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,   180,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    44,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    55,    56,    -1,    58,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    74,   240,   241,    -1,   243,
      -1,    80,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    18,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,   137,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,   180,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,   113,    -1,   115,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,   144,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
     161,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      18,    -1,    -1,    21,    -1,   216,   217,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
      38,    -1,    -1,    41,   235,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,   278,    -1,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,   133,    -1,    -1,   216,   217,
     218,   219,    -1,   141,    -1,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,   172,    -1,   254,   255,   176,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,   187,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,   198,    -1,
     268,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,    -1,    -1,   225,   226,   227,   228,   229,
     230,    -1,    -1,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278
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
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
    break;
case 797:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 798:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
    break;
case 804:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
    break;
case 805:
{ yyval = NULL; }
    break;
case 806:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 808:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 810:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 811:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 813:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 815:
{ yyval = NULL; }
    break;
case 818:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 819:
{ yyval = yyvsp[0]; }
    break;
case 820:
{ yyval = NULL; }
    break;
case 821:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 824:
{ yyval = NULL; }
    break;
case 825:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = NULL; }
    break;
case 830:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 832:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 834:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 835:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 836:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 838:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 842:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 843:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 845:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 846:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 848:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = yyvsp[-1]; }
    break;
case 852:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 864:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 865:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 866:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 867:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 868:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 869:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 870:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 871:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 872:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 873:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 874:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 875:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 876:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 877:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 878:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 879:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 880:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 881:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 882:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 883:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 884:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 885:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 886:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 887:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 890:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 891:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 892:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 893:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 894:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 895:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 896:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 898:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 900:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 901:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 902:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 903:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 904:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 905:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 906:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 907:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 908:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 909:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 913:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 914:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 915:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 917:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 918:
{ yyval = yyvsp[-1]; }
    break;
case 929:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 930:
{ yyval = yyvsp[0]; }
    break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 932:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 933:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 936:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 937:
{ yyval = yyvsp[-1]; }
    break;
case 938:
{ yyval = yyvsp[-1]; }
    break;
case 942:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 943:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 944:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 947:
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
case 948:
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
case 949:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 950:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 952:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 954:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 955:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 956:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 958:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 961:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 962:
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
case 963:
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
case 964:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 965:
{ yyval = make_parameter (); }
    break;
case 966:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 967:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 968:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 969:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 970:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 971:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 972:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 973:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 974:
{ yyval = yyvsp[0]; }
    break;
case 975:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 977:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 978:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 979:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 980:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 981:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 983:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 984:
{ yyval = yyvsp[0];}
    break;
case 988:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 989:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 990:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 991:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 992:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 993:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 994:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 995:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 996:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 997:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 998:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 1001:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 1005:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1006:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1007:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1008:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1011:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 1012:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;
case 1013:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 1014:
{ yyval = yyvsp[0]; }
    break;
case 1015:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 1016:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;
case 1017:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
    break;
case 1018:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;
case 1019:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
    break;
case 1020:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
    break;
case 1021:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
    break;
case 1022:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
    break;
case 1023:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1024:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 1025:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 1028:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 1029:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 1030:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1033:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1034:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1035:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1036:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1037:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1038:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1039:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1040:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1044:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 1045:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1046:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1047:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1048:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1049:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1051:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1058:
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
