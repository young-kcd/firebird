/* A Bison parser, made from E:\WinCvs\Root\FB2\firebird2\src\dsql\parse.y
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
# define	BASENAME	269
# define	BEFORE	270
# define	BEGIN	271
# define	BETWEEN	272
# define	BLOB	273
# define	BY	274
# define	CACHE	275
# define	CAST	276
# define	CHARACTER	277
# define	CHECK	278
# define	CHECK_POINT_LEN	279
# define	COLLATE	280
# define	COMMA	281
# define	COMMIT	282
# define	COMMITTED	283
# define	COMPUTED	284
# define	CONCATENATE	285
# define	CONDITIONAL	286
# define	CONSTRAINT	287
# define	CONTAINING	288
# define	COUNT	289
# define	CREATE	290
# define	CSTRING	291
# define	CURRENT	292
# define	CURSOR	293
# define	DATABASE	294
# define	DATE	295
# define	DB_KEY	296
# define	KW_DEBUG	297
# define	DECIMAL	298
# define	DECLARE	299
# define	DEFAULT	300
# define	KW_DELETE	301
# define	DESC	302
# define	DISTINCT	303
# define	DO	304
# define	DOMAIN	305
# define	DROP	306
# define	ELSE	307
# define	END	308
# define	ENTRY_POINT	309
# define	EQL	310
# define	ESCAPE	311
# define	EXCEPTION	312
# define	EXECUTE	313
# define	EXISTS	314
# define	EXIT	315
# define	EXTERNAL	316
# define	FILTER	317
# define	FOR	318
# define	FOREIGN	319
# define	FROM	320
# define	FULL	321
# define	FUNCTION	322
# define	GDSCODE	323
# define	GEQ	324
# define	GENERATOR	325
# define	GEN_ID	326
# define	GRANT	327
# define	GROUP	328
# define	GROUP_COMMIT_WAIT	329
# define	GTR	330
# define	HAVING	331
# define	IF	332
# define	KW_IN	333
# define	INACTIVE	334
# define	INNER	335
# define	INPUT_TYPE	336
# define	INDEX	337
# define	INSERT	338
# define	INTEGER	339
# define	INTO	340
# define	IS	341
# define	ISOLATION	342
# define	JOIN	343
# define	KEY	344
# define	KW_CHAR	345
# define	KW_DEC	346
# define	KW_DOUBLE	347
# define	KW_FILE	348
# define	KW_FLOAT	349
# define	KW_INT	350
# define	KW_LONG	351
# define	KW_NULL	352
# define	KW_NUMERIC	353
# define	KW_UPPER	354
# define	KW_VALUE	355
# define	LENGTH	356
# define	LOGFILE	357
# define	LPAREN	358
# define	LEFT	359
# define	LEQ	360
# define	LEVEL	361
# define	LIKE	362
# define	LOG_BUF_SIZE	363
# define	LSS	364
# define	MANUAL	365
# define	MAXIMUM	366
# define	MAX_SEGMENT	367
# define	MERGE	368
# define	MESSAGE	369
# define	MINIMUM	370
# define	MODULE_NAME	371
# define	NAMES	372
# define	NATIONAL	373
# define	NATURAL	374
# define	NCHAR	375
# define	NEQ	376
# define	NO	377
# define	NOT	378
# define	NOT_GTR	379
# define	NOT_LSS	380
# define	NUM_LOG_BUFS	381
# define	OF	382
# define	ON	383
# define	ONLY	384
# define	OPTION	385
# define	OR	386
# define	ORDER	387
# define	OUTER	388
# define	OUTPUT_TYPE	389
# define	OVERFLOW	390
# define	PAGE	391
# define	PAGES	392
# define	PAGE_SIZE	393
# define	PARAMETER	394
# define	PASSWORD	395
# define	PLAN	396
# define	POSITION	397
# define	POST_EVENT	398
# define	PRECISION	399
# define	PRIMARY	400
# define	PRIVILEGES	401
# define	PROCEDURE	402
# define	PROTECTED	403
# define	RAW_PARTITIONS	404
# define	READ	405
# define	REAL	406
# define	REFERENCES	407
# define	RESERVING	408
# define	RETAIN	409
# define	RETURNING_VALUES	410
# define	RETURNS	411
# define	REVOKE	412
# define	RIGHT	413
# define	RPAREN	414
# define	ROLLBACK	415
# define	SEGMENT	416
# define	SELECT	417
# define	SET	418
# define	SHADOW	419
# define	KW_SHARED	420
# define	SINGULAR	421
# define	KW_SIZE	422
# define	SMALLINT	423
# define	SNAPSHOT	424
# define	SOME	425
# define	SORT	426
# define	SQLCODE	427
# define	STABILITY	428
# define	STARTING	429
# define	STATISTICS	430
# define	SUB_TYPE	431
# define	SUSPEND	432
# define	SUM	433
# define	TABLE	434
# define	THEN	435
# define	TO	436
# define	TRANSACTION	437
# define	TRIGGER	438
# define	UNCOMMITTED	439
# define	UNION	440
# define	UNIQUE	441
# define	UPDATE	442
# define	USER	443
# define	VALUES	444
# define	VARCHAR	445
# define	VARIABLE	446
# define	VARYING	447
# define	VERSION	448
# define	VIEW	449
# define	WAIT	450
# define	WHEN	451
# define	WHERE	452
# define	WHILE	453
# define	WITH	454
# define	WORK	455
# define	WRITE	456
# define	FLOAT_NUMBER	457
# define	NUMBER	458
# define	NUMERIC	459
# define	SYMBOL	460
# define	STRING	461
# define	INTRODUCER	462
# define	ACTION	463
# define	ADMIN	464
# define	CASCADE	465
# define	FREE_IT	466
# define	RESTRICT	467
# define	ROLE	468
# define	COLUMN	469
# define	TYPE	470
# define	EXTRACT	471
# define	YEAR	472
# define	MONTH	473
# define	DAY	474
# define	HOUR	475
# define	MINUTE	476
# define	SECOND	477
# define	WEEKDAY	478
# define	YEARDAY	479
# define	TIME	480
# define	TIMESTAMP	481
# define	CURRENT_DATE	482
# define	CURRENT_TIME	483
# define	CURRENT_TIMESTAMP	484
# define	NUMBER64BIT	485
# define	SCALEDINT	486
# define	CURRENT_USER	487
# define	CURRENT_ROLE	488
# define	KW_BREAK	489
# define	SUBSTRING	490
# define	RECREATE	491
# define	KW_DESCRIPTOR	492
# define	FIRST	493
# define	SKIP	494
# define	CURRENT_CONNECTION	495
# define	CURRENT_TRANSACTION	496
# define	BIGINT	497
# define	CASE	498
# define	NULLIF	499
# define	COALESCE	500
# define	USING	501
# define	NULLS	502
# define	LAST	503
# define	ROW_COUNT	504
# define	LOCK	505
# define	SAVEPOINT	506
# define	RELEASE	507
# define	STATEMENT	508
# define	LEAVE	509
# define	INSERTING	510
# define	UPDATING	511
# define	DELETING	512
# define	KW_INSERTING	513
# define	KW_UPDATING	514
# define	KW_DELETING	515
# define	BACKUP	516
# define	KW_DIFFERENCE	517
# define	OPEN	518
# define	CLOSE	519
# define	FETCH	520
# define	ROWS	521
# define	BLOCK	522


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

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		1940
#define	YYFLAG		-32768
#define	YYNTBASE	285

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 522 ? yytranslate[x] : 723)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
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
     266,   267,   268
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    43,    46,    54,    63,    70,    78,    84,    86,
      89,    91,    94,    96,    98,   102,   104,   106,   108,   110,
     113,   116,   120,   121,   125,   126,   128,   136,   145,   152,
     160,   167,   175,   180,   184,   186,   190,   194,   198,   201,
     204,   207,   210,   212,   216,   218,   221,   224,   226,   230,
     232,   234,   238,   240,   243,   246,   249,   253,   262,   264,
     266,   272,   273,   275,   279,   281,   285,   288,   293,   295,
     299,   302,   306,   311,   316,   319,   329,   332,   336,   344,
     347,   350,   353,   356,   359,   362,   365,   368,   371,   374,
     377,   380,   383,   388,   391,   394,   396,   397,   399,   401,
     408,   415,   417,   419,   420,   421,   423,   424,   429,   430,
     432,   434,   437,   446,   448,   449,   453,   454,   455,   457,
     459,   462,   464,   466,   468,   471,   478,   480,   482,   486,
     487,   489,   491,   492,   494,   496,   499,   503,   508,   511,
     514,   518,   519,   521,   523,   526,   528,   530,   532,   537,
     541,   545,   549,   553,   557,   559,   561,   568,   572,   574,
     578,   580,   584,   587,   589,   590,   593,   597,   599,   600,
     602,   604,   607,   611,   616,   617,   619,   622,   623,   625,
     627,   633,   639,   643,   646,   647,   649,   653,   655,   657,
     664,   668,   671,   678,   681,   683,   686,   689,   692,   693,
     695,   697,   700,   701,   704,   705,   707,   709,   711,   713,
     715,   717,   718,   720,   722,   725,   728,   731,   737,   739,
     742,   746,   749,   752,   753,   755,   757,   759,   761,   765,
     770,   779,   784,   785,   792,   794,   796,   799,   802,   803,
     807,   811,   813,   816,   819,   822,   831,   840,   849,   858,
     862,   863,   868,   869,   871,   875,   880,   882,   886,   889,
     893,   897,   899,   901,   902,   904,   907,   912,   914,   916,
     920,   922,   923,   926,   928,   935,   937,   939,   943,   945,
     948,   949,   951,   954,   957,   960,   962,   964,   966,   968,
     970,   972,   975,   977,   979,   982,   984,   986,   988,   991,
     994,   996,   999,  1003,  1008,  1011,  1018,  1023,  1025,  1027,
    1036,  1046,  1053,  1062,  1069,  1074,  1075,  1080,  1083,  1085,
    1089,  1090,  1093,  1098,  1099,  1101,  1103,  1107,  1111,  1119,
    1122,  1123,  1126,  1129,  1133,  1137,  1138,  1140,  1143,  1148,
    1150,  1154,  1157,  1160,  1163,  1165,  1167,  1169,  1171,  1174,
    1177,  1183,  1184,  1189,  1197,  1201,  1202,  1204,  1208,  1212,
    1220,  1228,  1230,  1232,  1236,  1241,  1249,  1252,  1254,  1258,
    1260,  1262,  1269,  1273,  1274,  1275,  1276,  1277,  1281,  1282,
    1292,  1302,  1304,  1306,  1307,  1310,  1312,  1314,  1316,  1318,
    1320,  1324,  1328,  1332,  1336,  1340,  1344,  1350,  1356,  1362,
    1368,  1374,  1380,  1383,  1384,  1389,  1392,  1396,  1400,  1403,
    1406,  1410,  1414,  1417,  1421,  1428,  1430,  1433,  1438,  1442,
    1445,  1448,  1451,  1454,  1458,  1460,  1464,  1468,  1472,  1475,
    1478,  1483,  1488,  1494,  1496,  1498,  1500,  1502,  1504,  1506,
    1508,  1510,  1512,  1514,  1516,  1518,  1520,  1522,  1524,  1526,
    1528,  1530,  1532,  1534,  1536,  1538,  1540,  1542,  1544,  1546,
    1548,  1550,  1552,  1555,  1558,  1561,  1563,  1565,  1567,  1568,
    1571,  1574,  1575,  1577,  1580,  1583,  1586,  1589,  1592,  1597,
    1601,  1604,  1607,  1609,  1613,  1621,  1623,  1624,  1626,  1627,
    1630,  1633,  1636,  1639,  1642,  1645,  1648,  1651,  1654,  1658,
    1661,  1664,  1667,  1669,  1671,  1673,  1675,  1680,  1686,  1688,
    1692,  1694,  1698,  1700,  1703,  1705,  1707,  1709,  1711,  1713,
    1715,  1717,  1719,  1721,  1723,  1725,  1730,  1735,  1742,  1748,
    1752,  1753,  1756,  1759,  1760,  1764,  1765,  1770,  1772,  1778,
    1783,  1785,  1790,  1792,  1795,  1798,  1800,  1802,  1804,  1807,
    1810,  1813,  1816,  1817,  1821,  1827,  1829,  1831,  1834,  1838,
    1840,  1843,  1847,  1848,  1850,  1852,  1854,  1860,  1866,  1873,
    1875,  1877,  1879,  1882,  1887,  1889,  1890,  1896,  1898,  1899,
    1903,  1906,  1908,  1909,  1912,  1913,  1915,  1916,  1920,  1922,
    1923,  1925,  1928,  1930,  1932,  1934,  1936,  1939,  1942,  1944,
    1947,  1951,  1953,  1955,  1959,  1963,  1965,  1968,  1972,  1974,
    1977,  1978,  1981,  1983,  1985,  1986,  1988,  1990,  1992,  1996,
    1999,  2003,  2004,  2006,  2010,  2015,  2021,  2023,  2027,  2032,
    2036,  2037,  2039,  2043,  2047,  2049,  2051,  2052,  2054,  2056,
    2061,  2062,  2065,  2070,  2071,  2075,  2076,  2079,  2080,  2083,
    2084,  2094,  2106,  2107,  2108,  2109,  2110,  2114,  2117,  2119,
    2120,  2124,  2130,  2134,  2137,  2143,  2146,  2148,  2150,  2152,
    2154,  2156,  2160,  2162,  2166,  2168,  2169,  2172,  2174,  2178,
    2180,  2182,  2184,  2191,  2193,  2194,  2198,  2199,  2201,  2205,
    2212,  2216,  2221,  2224,  2228,  2229,  2231,  2234,  2236,  2238,
    2240,  2243,  2245,  2248,  2250,  2253,  2254,  2258,  2259,  2261,
    2265,  2267,  2270,  2271,  2274,  2275,  2278,  2279,  2284,  2286,
    2289,  2291,  2293,  2294,  2296,  2300,  2303,  2305,  2307,  2310,
    2312,  2317,  2321,  2323,  2327,  2332,  2333,  2342,  2348,  2350,
    2352,  2357,  2362,  2367,  2369,  2371,  2377,  2383,  2385,  2389,
    2393,  2395,  2397,  2399,  2403,  2411,  2419,  2425,  2429,  2430,
    2432,  2434,  2436,  2439,  2440,  2442,  2444,  2446,  2447,  2451,
    2453,  2457,  2459,  2460,  2464,  2466,  2470,  2472,  2476,  2480,
    2482,  2484,  2488,  2490,  2493,  2495,  2499,  2503,  2505,  2508,
    2512,  2516,  2520,  2524,  2526,  2530,  2533,  2535,  2537,  2539,
    2541,  2543,  2545,  2547,  2549,  2551,  2553,  2557,  2561,  2565,
    2569,  2573,  2577,  2581,  2585,  2592,  2599,  2606,  2613,  2620,
    2627,  2634,  2641,  2648,  2655,  2662,  2669,  2676,  2683,  2690,
    2697,  2699,  2701,  2707,  2714,  2718,  2723,  2729,  2736,  2740,
    2745,  2749,  2754,  2758,  2763,  2768,  2774,  2779,  2784,  2788,
    2793,  2795,  2797,  2799,  2801,  2803,  2805,  2807,  2811,  2815,
    2827,  2839,  2841,  2843,  2845,  2847,  2849,  2851,  2853,  2855,
    2857,  2860,  2863,  2867,  2871,  2875,  2879,  2883,  2887,  2891,
    2895,  2897,  2899,  2901,  2903,  2907,  2909,  2911,  2913,  2915,
    2917,  2922,  2924,  2928,  2930,  2933,  2935,  2937,  2939,  2941,
    2943,  2945,  2947,  2950,  2953,  2956,  2958,  2960,  2962,  2964,
    2966,  2968,  2970,  2972,  2974,  2976,  2979,  2981,  2984,  2986,
    2988,  2990,  2992,  2994,  2997,  2999,  3001,  3003,  3005,  3007,
    3012,  3018,  3024,  3030,  3036,  3042,  3048,  3054,  3060,  3066,
    3072,  3079,  3081,  3088,  3090,  3095,  3103,  3106,  3107,  3112,
    3116,  3123,  3125,  3127,  3134,  3141,  3143,  3145,  3150,  3157,
    3162,  3168,  3172,  3178,  3183,  3189,  3191,  3193,  3195,  3197,
    3199,  3201,  3203,  3205,  3207,  3209,  3211,  3213,  3214,  3216,
    3218,  3220,  3222,  3224,  3226,  3228,  3230,  3232,  3234,  3236,
    3238,  3240,  3242,  3244,  3246,  3248,  3250,  3252,  3254,  3256,
    3258,  3260,  3262,  3264,  3266,  3268,  3270,  3272,  3274,  3276,
    3278,  3280,  3282,  3284,  3286,  3288,  3290,  3292,  3294,  3296,
    3298,  3300
};
static const short yyrhs[] =
{
     286,     0,   286,   276,     0,   478,     0,   620,     0,   532,
       0,   316,     0,   306,     0,   609,     0,   500,     0,   287,
       0,   608,     0,   451,     0,   452,     0,   318,     0,   320,
       0,   296,     0,   533,     0,   526,     0,   555,     0,   524,
       0,   613,     0,    43,   667,     0,    73,   289,   129,   288,
     182,   300,   293,     0,    73,   291,   129,   148,   295,   182,
     300,   293,     0,    73,   289,   129,   288,   182,   298,     0,
      73,   291,   129,   148,   295,   182,   298,     0,    73,   302,
     182,   304,   294,     0,   592,     0,   180,   592,     0,     6,
       0,     6,   147,     0,   290,     0,   292,     0,   290,   277,
     292,     0,    59,     0,   163,     0,    84,     0,    47,     0,
     188,   625,     0,   153,   625,     0,   200,    73,   131,     0,
       0,   200,   210,   131,     0,     0,   712,     0,   158,   297,
     289,   129,   288,    66,   300,     0,   158,   297,   291,   129,
     148,   295,    66,   300,     0,   158,   289,   129,   288,    66,
     300,     0,   158,   291,   129,   148,   295,    66,   300,     0,
     158,   289,   129,   288,    66,   298,     0,   158,   291,   129,
     148,   295,    66,   298,     0,   158,   302,    66,   304,     0,
      73,   131,    64,     0,   299,     0,   298,   277,   299,     0,
     298,   277,   301,     0,   300,   277,   299,     0,   148,   712,
       0,   184,   716,     0,   195,   719,     0,   214,   713,     0,
     301,     0,   300,   277,   301,     0,   717,     0,   189,   717,
       0,    74,   717,     0,   303,     0,   302,   277,   303,     0,
     713,     0,   305,     0,   304,   277,   305,     0,   717,     0,
     189,   717,     0,    45,   307,     0,    63,   315,     0,    62,
      68,   308,     0,   697,   310,   157,   313,    55,   666,   117,
     666,     0,   507,     0,    19,     0,    37,   278,   670,   279,
     513,     0,     0,   311,     0,   278,   311,   279,     0,   312,
       0,   311,   277,   312,     0,   379,   309,     0,   379,   309,
      20,   238,     0,   314,     0,   278,   314,   279,     0,   379,
     309,     0,   379,   309,   212,     0,   379,   309,    20,   101,
       0,   379,   309,    20,   238,     0,   140,   670,     0,   706,
      82,   512,   135,   512,    55,   666,   117,   666,     0,    36,
     317,     0,    58,   705,   666,     0,   322,   560,    83,   709,
     129,   592,   323,     0,   148,   398,     0,   180,   366,     0,
     184,   470,     0,   195,   456,     0,    71,   339,     0,    40,
     341,     0,    51,   330,     0,   165,   324,     0,   214,   340,
       0,   237,   319,     0,   148,   399,     0,   180,   367,     0,
     195,   457,     0,    36,   132,     7,   321,     0,   148,   400,
       0,   184,   471,     0,   187,     0,     0,   627,     0,   626,
       0,   373,   278,   467,   655,   468,   279,     0,   670,   325,
     326,   666,   327,   328,     0,   111,     0,    14,     0,     0,
       0,    32,     0,     0,   102,   342,   673,   365,     0,     0,
     329,     0,   354,     0,   329,   354,     0,   376,   331,   502,
     467,   332,   468,   333,   375,     0,    10,     0,     0,    46,
     467,   381,     0,     0,     0,   334,     0,   335,     0,   334,
     335,     0,   336,     0,   337,     0,   338,     0,   124,    98,
       0,   467,    24,   278,   634,   279,   468,     0,   708,     0,
     713,     0,   343,   344,   347,     0,     0,   269,     0,   666,
       0,     0,   345,     0,   346,     0,   345,   346,     0,   139,
     342,   670,     0,   102,   342,   673,   365,     0,   189,   666,
       0,   141,   666,     0,   164,   118,   666,     0,     0,   348,
       0,   349,     0,   348,   349,     0,   354,     0,   351,     0,
     350,     0,    46,    23,   164,   699,     0,   263,    94,   666,
       0,    75,   342,   673,     0,    25,   342,   673,     0,   127,
     342,   670,     0,   109,   342,   671,     0,   353,     0,   352,
       0,   103,   278,   355,   279,   136,   356,     0,   103,    15,
     356,     0,   103,     0,   360,   666,   361,     0,   356,     0,
     355,   277,   356,     0,   357,   358,     0,   666,     0,     0,
     358,   359,     0,   168,   342,   673,     0,    94,     0,     0,
     362,     0,   363,     0,   362,   363,     0,   175,   364,   673,
       0,   102,   342,   673,   365,     0,     0,    12,     0,    12,
     137,     0,     0,   137,     0,   138,     0,   592,   368,   278,
     369,   279,     0,   592,   368,   278,   369,   279,     0,    62,
      94,   666,     0,    62,   666,     0,     0,   370,     0,   369,
     277,   370,     0,   371,     0,   386,     0,   376,   374,   380,
     468,   382,   375,     0,   376,   503,   372,     0,   376,   372,
       0,   373,   278,   467,   655,   468,   279,     0,    30,    20,
       0,    30,     0,   502,   467,     0,   632,   465,     0,    26,
     700,     0,     0,   632,     0,   632,     0,   379,   502,     0,
       0,    46,   381,     0,     0,   659,     0,   663,     0,   664,
       0,   665,     0,   696,     0,   656,     0,     0,   383,     0,
     384,     0,   383,   384,     0,   387,   385,     0,   124,    98,
       0,   153,   592,   625,   394,   392,     0,   393,     0,   187,
     392,     0,   146,    90,   392,     0,   387,   388,     0,    33,
     702,     0,     0,   389,     0,   390,     0,   391,     0,   393,
       0,   187,   626,   392,     0,   146,    90,   626,   392,     0,
      65,    90,   626,   153,   592,   625,   394,   392,     0,   247,
     560,    83,   709,     0,     0,   467,    24,   278,   634,   279,
     468,     0,   395,     0,   396,     0,   396,   395,     0,   395,
     396,     0,     0,   129,   188,   397,     0,   129,    47,   397,
       0,   211,     0,   164,    46,     0,   164,    98,     0,   123,
     209,     0,   712,   402,   403,    10,   465,   409,   418,   468,
       0,   712,   402,   403,    10,   465,   409,   418,   468,     0,
     712,   402,   403,    10,   465,   409,   418,   468,     0,   712,
     402,   403,    10,   465,   409,   418,   468,     0,   278,   404,
     279,     0,     0,   157,   278,   406,   279,     0,     0,   405,
       0,   404,   277,   405,     0,   377,   503,   408,   468,     0,
     407,     0,   406,   277,   407,     0,   377,   503,     0,    46,
     465,   381,     0,   269,   465,   381,     0,   465,     0,   410,
       0,     0,   411,     0,   410,   411,     0,    45,   414,   412,
     276,     0,   413,     0,   416,     0,   376,   503,   415,     0,
     192,     0,     0,   269,   381,     0,   380,     0,   703,    39,
      64,   278,   568,   279,     0,   421,     0,   418,     0,    17,
     419,    54,     0,   420,     0,   420,   442,     0,     0,   417,
       0,   420,   417,     0,   617,   276,     0,   609,   276,     0,
     422,     0,   423,     0,   424,     0,   425,     0,   427,     0,
     430,     0,   608,   276,     0,   431,     0,   433,     0,   613,
     276,     0,   438,     0,   428,     0,   429,     0,   178,   276,
       0,    61,   276,     0,   440,     0,   446,   276,     0,    58,
     705,   276,     0,    58,   705,   655,   276,     0,    58,   276,
       0,    59,   148,   712,   435,   436,   276,     0,    59,   426,
     655,   276,     0,   191,     0,   254,     0,   439,    64,   555,
      86,   437,   441,    50,   417,     0,   439,    64,    59,   426,
     655,    86,   437,    50,   417,     0,    59,   426,   655,    86,
     437,   276,     0,    78,   278,   634,   279,   181,   417,    53,
     417,     0,    78,   278,   634,   279,   181,   417,     0,   144,
     655,   432,   276,     0,     0,   555,    86,   437,   276,     0,
     280,   718,     0,   619,     0,   278,   619,   279,     0,     0,
     156,   437,     0,   156,   278,   437,   279,     0,     0,   434,
       0,   631,     0,   437,   277,   631,     0,   437,   277,   434,
       0,   439,   199,   278,   634,   279,    50,   417,     0,   711,
     280,     0,     0,   235,   276,     0,   255,   276,     0,   255,
     711,   276,     0,    10,    39,   703,     0,     0,   443,     0,
     442,   443,     0,   197,   444,    50,   417,     0,   445,     0,
     444,   277,   445,     0,   173,   667,     0,    69,   707,     0,
      58,   705,     0,     9,     0,   447,     0,   449,     0,   448,
       0,   264,   703,     0,   265,   703,     0,   266,   450,   703,
      86,   437,     0,     0,    59,   148,   712,   435,     0,    59,
     268,   453,   403,    10,   409,   418,     0,   278,   454,   279,
       0,     0,   455,     0,   454,   277,   455,     0,   407,   269,
     662,     0,   719,   625,    10,   465,   458,   469,   466,     0,
     719,   625,    10,   465,   458,   469,   466,     0,   459,     0,
     460,     0,   459,   186,   460,     0,   459,   186,     6,   460,
       0,   163,   576,   577,   461,   598,   594,   597,     0,    66,
     462,     0,   463,     0,   462,   277,   463,     0,   464,     0,
     591,     0,   463,   593,    89,   463,   129,   634,     0,   278,
     464,   279,     0,     0,     0,     0,     0,   200,    24,   131,
       0,     0,   716,    64,   592,   472,   473,   476,   467,   477,
     468,     0,   716,    64,   592,   472,   473,   476,   467,   477,
     468,     0,     3,     0,    80,     0,     0,   474,   475,     0,
      16,     0,     5,     0,    84,     0,   188,     0,    47,     0,
      84,   132,   188,     0,    84,   132,    47,     0,   188,   132,
      84,     0,   188,   132,    47,     0,    47,   132,    84,     0,
      47,   132,   188,     0,    84,   132,   188,   132,    47,     0,
      84,   132,    47,   132,   188,     0,   188,   132,    84,   132,
      47,     0,   188,   132,    47,   132,    84,     0,    47,   132,
      84,   132,   188,     0,    47,   132,   188,   132,    84,     0,
     143,   668,     0,     0,    10,   467,   409,   418,     0,     7,
     479,     0,    58,   705,   666,     0,   180,   592,   484,     0,
     184,   497,     0,   148,   401,     0,    40,   493,   494,     0,
      51,   486,   482,     0,    83,   492,     0,    46,   467,   381,
       0,    24,   467,   278,   634,   279,   468,     0,   483,     0,
     482,   483,     0,   164,   467,   480,   468,     0,     4,    33,
     481,     0,     4,   338,     0,    52,    46,     0,    52,    33,
       0,   182,   632,     0,   216,   379,   503,     0,   485,     0,
     484,   277,   485,     0,    52,   632,   491,     0,    52,    33,
     702,     0,     4,   371,     0,     4,   386,     0,   488,   632,
     143,   670,     0,   488,   486,   182,   632,     0,   488,   490,
     216,   489,   468,     0,   487,     0,   721,     0,   215,     0,
     216,     0,   217,     0,   218,     0,   219,     0,   220,     0,
     221,     0,   222,     0,   223,     0,   224,     0,   225,     0,
     226,     0,   227,     0,   228,     0,   229,     0,   230,     0,
     233,     0,   234,     0,   241,     0,   242,     0,   250,     0,
     252,     0,   264,     0,   265,     0,   266,     0,   267,     0,
       7,     0,     7,   215,     0,   503,   467,     0,   632,   465,
       0,   632,     0,   213,     0,   211,     0,     0,   709,     3,
       0,   709,    80,     0,     0,   495,     0,   494,   495,     0,
       4,   329,     0,    52,   103,     0,   164,   496,     0,     4,
     351,     0,     4,   263,    94,   666,     0,    52,   263,    94,
       0,    17,   262,     0,    54,   262,     0,   350,     0,   496,
     277,   350,     0,   716,   472,   498,   476,   467,   499,   468,
       0,   473,     0,     0,   477,     0,     0,    52,   501,     0,
      58,   705,     0,    83,   709,     0,   148,   712,     0,   180,
     715,     0,   184,   716,     0,   195,   719,     0,    63,   706,
       0,    51,   704,     0,    62,    68,   697,     0,   165,   670,
       0,   214,   713,     0,    71,   708,     0,   503,     0,   504,
       0,   507,     0,   510,     0,   508,   281,   505,   282,     0,
     515,   281,   505,   282,   513,     0,   506,     0,   505,   277,
     506,     0,   672,     0,   672,   280,   672,     0,   508,     0,
     515,   513,     0,   514,     0,   519,     0,   522,     0,   243,
       0,   509,     0,   169,     0,    41,     0,   226,     0,   227,
       0,    85,     0,    96,     0,    19,   512,   511,   513,     0,
      19,   278,   671,   279,     0,    19,   278,   671,   277,   667,
     279,     0,    19,   278,   277,   667,   279,     0,   162,   168,
     671,     0,     0,   177,   667,     0,   177,   698,     0,     0,
      23,   164,   699,     0,     0,   518,   278,   670,   279,     0,
     518,     0,   518,   193,   278,   670,   279,     0,   517,   278,
     670,   279,     0,   517,     0,   516,   278,   670,   279,     0,
     191,     0,    23,   193,     0,    91,   193,     0,    23,     0,
      91,     0,   121,     0,   119,    23,     0,   119,    91,     0,
      99,   520,     0,   521,   520,     0,     0,   278,   672,   279,
       0,   278,   672,   277,   672,   279,     0,    44,     0,    92,
       0,    95,   523,     0,    97,    95,   523,     0,   152,     0,
      93,   145,     0,   278,   668,   279,     0,     0,   537,     0,
     525,     0,   554,     0,   164,    71,   708,   182,   672,     0,
     164,    71,   708,   182,   231,     0,   164,    71,   708,   182,
     273,   231,     0,   527,     0,   528,     0,   530,     0,   252,
     720,     0,   253,   252,   720,   529,     0,   130,     0,     0,
     161,   534,   182,   531,   720,     0,   252,     0,     0,    28,
     534,   535,     0,   161,   534,     0,   201,     0,     0,   155,
     536,     0,     0,   170,     0,     0,   164,   183,   538,     0,
     539,     0,     0,   540,     0,   539,   540,     0,   541,     0,
     542,     0,   543,     0,   547,     0,   151,   130,     0,   151,
     202,     0,   196,     0,   123,   196,     0,    88,   107,   544,
       0,   544,     0,   545,     0,   151,   185,   546,     0,   151,
      29,   546,     0,   170,     0,   170,   180,     0,   170,   180,
     174,     0,   194,     0,   123,   194,     0,     0,   154,   550,
       0,   166,     0,   149,     0,     0,   151,     0,   202,     0,
     551,     0,   550,   277,   551,     0,   553,   552,     0,    64,
     548,   549,     0,     0,   592,     0,   553,   277,   592,     0,
     164,   176,    83,   709,     0,   556,   557,   563,   564,   566,
       0,   567,     0,   556,   186,   567,     0,   556,   186,     6,
     567,     0,   133,    20,   558,     0,     0,   559,     0,   558,
     277,   559,     0,   655,   560,   562,     0,    11,     0,    48,
       0,     0,   239,     0,   249,     0,   248,   571,   561,   572,
       0,     0,   267,   655,     0,   267,   655,   182,   655,     0,
       0,    64,   188,   565,     0,     0,   128,   627,     0,     0,
     200,   251,     0,     0,   163,   573,   576,   577,   581,   598,
     594,   597,   599,     0,   163,   573,   576,   577,   581,   598,
     594,   597,   599,   557,   563,     0,     0,     0,     0,     0,
     574,   575,   570,     0,   574,   570,     0,   575,     0,     0,
     239,   673,   569,     0,   239,   278,   655,   279,   569,     0,
     239,   662,   569,     0,   240,   673,     0,   240,   278,   570,
     655,   279,     0,   240,   662,     0,    49,     0,   695,     0,
     578,     0,   274,     0,   579,     0,   578,   277,   579,     0,
     618,     0,   618,   580,   710,     0,    10,     0,     0,    66,
     582,     0,   583,     0,   582,   277,   583,     0,   588,     0,
     589,     0,   584,     0,   278,   555,   279,   580,   585,   586,
       0,   714,     0,     0,   278,   587,   279,     0,     0,   710,
       0,   587,   277,   710,     0,   583,   593,    89,   583,   129,
     634,     0,   278,   588,   279,     0,   712,   590,   580,   714,
       0,   712,   590,     0,   278,   619,   279,     0,     0,   592,
       0,   715,   714,     0,   715,     0,    81,     0,   105,     0,
     105,   134,     0,   159,     0,   159,   134,     0,    67,     0,
      67,   134,     0,     0,    74,    20,   595,     0,     0,   596,
       0,   595,   277,   596,     0,   655,     0,    77,   634,     0,
       0,   198,   634,     0,     0,   142,   600,     0,     0,   601,
     278,   602,   279,     0,    89,     0,   172,   114,     0,   114,
       0,   172,     0,     0,   603,     0,   603,   277,   602,     0,
     604,   605,     0,   600,     0,   715,     0,   715,   604,     0,
     120,     0,    83,   278,   606,   279,     0,   133,   709,   607,
       0,   709,     0,   709,   277,   606,     0,    83,   278,   606,
     279,     0,     0,    84,    86,   592,   628,   190,   278,   619,
     279,     0,    84,    86,   592,   628,   568,     0,   610,     0,
     611,     0,    47,    66,   591,   598,     0,    47,    66,   591,
     612,     0,   198,    38,   128,   703,     0,   614,     0,   615,
       0,   188,   591,   164,   616,   598,     0,   188,   591,   164,
     616,   612,     0,   617,     0,   616,   277,   617,     0,   633,
     269,   618,     0,   655,     0,   696,     0,   618,     0,   619,
     277,   618,     0,   151,    19,   632,    66,   592,   621,   623,
       0,    84,    19,   632,    86,   592,   621,   623,     0,    63,
      66,   622,   182,   622,     0,    63,   182,   622,     0,     0,
     512,     0,   662,     0,   667,     0,   113,   624,     0,     0,
     671,     0,   662,     0,   626,     0,     0,   278,   627,   279,
       0,   632,     0,   627,   277,   632,     0,   629,     0,     0,
     278,   630,   279,     0,   633,     0,   630,   277,   633,     0,
     632,     0,   714,   283,   701,     0,   714,   283,   274,     0,
     701,     0,   632,     0,   714,   283,   701,     0,   649,     0,
     124,   649,     0,   636,     0,   634,   132,   634,     0,   634,
       8,   634,     0,   636,     0,   124,   649,     0,   635,   132,
     634,     0,   635,     8,   634,     0,   650,   132,   634,     0,
     650,     8,   634,     0,   637,     0,   278,   635,   279,     0,
     124,   636,     0,   638,     0,   641,     0,   642,     0,   643,
       0,   648,     0,   639,     0,   646,     0,   644,     0,   645,
       0,   647,     0,   655,   269,   655,     0,   655,   270,   655,
       0,   655,   271,   655,     0,   655,    70,   655,     0,   655,
     106,   655,     0,   655,   125,   655,     0,   655,   126,   655,
       0,   655,   122,   655,     0,   655,   269,     6,   278,   653,
     279,     0,   655,   270,     6,   278,   653,   279,     0,   655,
     271,     6,   278,   653,   279,     0,   655,    70,     6,   278,
     653,   279,     0,   655,   106,     6,   278,   653,   279,     0,
     655,   125,     6,   278,   653,   279,     0,   655,   126,     6,
     278,   653,   279,     0,   655,   122,     6,   278,   653,   279,
       0,   655,   269,   640,   278,   653,   279,     0,   655,   270,
     640,   278,   653,   279,     0,   655,   271,   640,   278,   653,
     279,     0,   655,    70,   640,   278,   653,   279,     0,   655,
     106,   640,   278,   653,   279,     0,   655,   125,   640,   278,
     653,   279,     0,   655,   126,   640,   278,   653,   279,     0,
     655,   122,   640,   278,   653,   279,     0,   171,     0,     9,
       0,   655,    18,   655,     8,   655,     0,   655,   124,    18,
     655,     8,   655,     0,   655,   108,   655,     0,   655,   124,
     108,   655,     0,   655,   108,   655,    57,   655,     0,   655,
     124,   108,   655,    57,   655,     0,   655,    79,   651,     0,
     655,   124,    79,   651,     0,   655,    34,   655,     0,   655,
     124,    34,   655,     0,   655,   175,   655,     0,   655,   124,
     175,   655,     0,   655,   175,   200,   655,     0,   655,   124,
     175,   200,   655,     0,    60,   278,   568,   279,     0,   167,
     278,   568,   279,     0,   655,    87,    98,     0,   655,    87,
     124,    98,     0,   256,     0,   257,     0,   258,     0,   259,
       0,   260,     0,   261,     0,   652,     0,   278,   658,   279,
       0,   278,   653,   279,     0,   163,   573,   576,   655,   581,
     598,   594,   597,   599,   557,   563,     0,   163,   573,   576,
     655,   581,   598,   594,   597,   599,   557,   563,     0,   631,
       0,   657,     0,   674,     0,   661,     0,   662,     0,   434,
       0,   683,     0,   684,     0,   682,     0,   273,   655,     0,
     272,   655,     0,   655,   272,   655,     0,   655,    31,   655,
       0,   655,    26,   700,     0,   655,   273,   655,     0,   655,
     274,   655,     0,   655,   275,   655,     0,   278,   655,   279,
       0,   278,   654,   279,     0,   663,     0,   664,     0,   665,
       0,    42,     0,   714,   283,    42,     0,   101,     0,   656,
       0,   228,     0,   229,     0,   230,     0,   631,   281,   658,
     282,     0,   655,     0,   658,   277,   655,     0,   661,     0,
     273,   660,     0,   205,     0,   204,     0,   203,     0,   231,
       0,   232,     0,   660,     0,   666,     0,    41,   207,     0,
     226,   207,     0,   227,   207,     0,   284,     0,   189,     0,
     233,     0,   234,     0,   241,     0,   242,     0,    69,     0,
     173,     0,   250,     0,   207,     0,   208,   207,     0,   668,
       0,   273,   669,     0,   204,     0,   204,     0,   668,     0,
     204,     0,   673,     0,   273,   673,     0,   204,     0,   675,
       0,   676,     0,   677,     0,   679,     0,    35,   278,   274,
     279,     0,    35,   278,   695,   655,   279,     0,    35,   278,
      49,   655,   279,     0,   179,   278,   695,   655,   279,     0,
     179,   278,    49,   655,   279,     0,    13,   278,   695,   655,
     279,     0,    13,   278,    49,   655,   279,     0,   116,   278,
     695,   655,   279,     0,   116,   278,    49,   655,   279,     0,
     112,   278,   695,   655,   279,     0,   112,   278,    49,   655,
     279,     0,    72,   278,   708,   277,   655,   279,     0,   678,
       0,   217,   278,   694,    66,   655,   279,     0,   680,     0,
     100,   278,   655,   279,     0,   236,   278,   655,    66,   655,
     681,   279,     0,    64,   655,     0,     0,   697,   278,   658,
     279,     0,   697,   278,   279,     0,    22,   278,   618,    10,
     378,   279,     0,   685,     0,   686,     0,   245,   278,   655,
     277,   655,   279,     0,   246,   278,   618,   277,   619,   279,
       0,   687,     0,   689,     0,   244,   692,   688,    54,     0,
     244,   692,   688,    53,   693,    54,     0,   197,   691,   181,
     693,     0,   688,   197,   691,   181,   693,     0,   244,   690,
      54,     0,   244,   690,    53,   693,    54,     0,   197,   634,
     181,   693,     0,   690,   197,   634,   181,   693,     0,   655,
       0,   655,     0,   618,     0,   218,     0,   219,     0,   220,
       0,   221,     0,   222,     0,   223,     0,   224,     0,   225,
       0,     6,     0,     0,    98,     0,   206,     0,   721,     0,
     721,     0,   721,     0,   721,     0,   721,     0,   721,     0,
     721,     0,   721,     0,   721,     0,   721,     0,   721,     0,
     721,     0,   721,     0,   721,     0,   721,     0,   721,     0,
     721,     0,   721,     0,   721,     0,   721,     0,   721,     0,
     721,     0,   721,     0,   206,     0,   722,     0,   235,     0,
     238,     0,   236,     0,   246,     0,   249,     0,   255,     0,
     251,     0,   245,     0,   248,     0,   254,     0,   247,     0,
     256,     0,   257,     0,   258,     0,   268,     0,   262,     0,
     263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   530,   532,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   563,   567,   571,   575,   579,   584,   585,
     589,   591,   593,   597,   598,   602,   606,   608,   610,   612,
     614,   618,   620,   624,   626,   630,   637,   642,   647,   652,
     657,   662,   667,   673,   677,   678,   680,   682,   686,   688,
     690,   692,   696,   697,   704,   706,   708,   712,   713,   717,
     721,   722,   726,   728,   735,   739,   741,   746,   752,   753,
     755,   761,   763,   764,   768,   769,   775,   778,   784,   785,
     788,   791,   795,   799,   802,   807,   816,   820,   823,   826,
     828,   830,   832,   834,   836,   838,   840,   842,   847,   851,
     853,   855,   865,   869,   871,   882,   884,   888,   890,   891,
     897,   903,   905,   907,   911,   913,   917,   919,   923,   925,
     928,   929,   936,   954,   956,   960,   962,   966,   968,   971,
     972,   975,   980,   981,   984,   988,   997,  1005,  1014,  1019,
    1020,  1023,  1029,  1031,  1034,  1035,  1039,  1041,  1043,  1045,
    1047,  1051,  1053,  1056,  1057,  1061,  1063,  1064,  1065,  1067,
    1071,  1073,  1075,  1077,  1081,  1086,  1093,  1099,  1106,  1113,
    1146,  1147,  1150,  1155,  1159,  1160,  1163,  1171,  1175,  1176,
    1179,  1180,  1183,  1185,  1189,  1190,  1191,  1194,  1195,  1196,
    1202,  1207,  1212,  1214,  1216,  1220,  1221,  1225,  1226,  1233,
    1237,  1241,  1250,  1256,  1257,  1260,  1262,  1267,  1269,  1274,
    1280,  1286,  1290,  1296,  1298,  1302,  1307,  1308,  1309,  1311,
    1313,  1317,  1319,  1322,  1323,  1327,  1332,  1334,  1339,  1340,
    1342,  1350,  1354,  1356,  1359,  1360,  1361,  1362,  1365,  1369,
    1373,  1380,  1387,  1392,  1398,  1401,  1403,  1405,  1407,  1411,
    1414,  1418,  1421,  1424,  1427,  1436,  1447,  1457,  1467,  1477,
    1479,  1483,  1485,  1489,  1490,  1494,  1500,  1501,  1505,  1510,
    1512,  1514,  1518,  1520,  1524,  1525,  1529,  1533,  1534,  1537,
    1542,  1544,  1548,  1550,  1554,  1559,  1560,  1563,  1567,  1569,
    1571,  1575,  1576,  1580,  1581,  1582,  1583,  1584,  1585,  1586,
    1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,  1597,
    1599,  1600,  1603,  1605,  1609,  1613,  1618,  1622,  1622,  1624,
    1629,  1633,  1637,  1639,  1643,  1647,  1653,  1658,  1663,  1665,
    1667,  1671,  1673,  1675,  1679,  1680,  1681,  1683,  1687,  1691,
    1693,  1697,  1699,  1701,  1706,  1708,  1712,  1713,  1717,  1722,
    1723,  1727,  1729,  1731,  1733,  1737,  1738,  1739,  1742,  1746,
    1750,  1754,  1796,  1803,  1811,  1813,  1817,  1818,  1822,  1828,
    1835,  1855,  1859,  1861,  1863,  1867,  1878,  1882,  1883,  1887,
    1888,  1896,  1899,  1905,  1909,  1915,  1919,  1925,  1929,  1937,
    1948,  1959,  1961,  1963,  1967,  1971,  1973,  1977,  1979,  1981,
    1983,  1985,  1987,  1989,  1991,  1993,  1995,  1997,  1999,  2001,
    2003,  2005,  2009,  2011,  2015,  2021,  2025,  2028,  2035,  2037,
    2039,  2042,  2045,  2050,  2054,  2060,  2061,  2065,  2074,  2078,
    2080,  2082,  2084,  2086,  2090,  2091,  2095,  2097,  2099,  2101,
    2107,  2110,  2112,  2116,  2124,  2125,  2126,  2127,  2128,  2129,
    2130,  2131,  2132,  2133,  2134,  2135,  2136,  2137,  2138,  2139,
    2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,  2149,
    2150,  2153,  2155,  2159,  2161,  2166,  2172,  2174,  2176,  2180,
    2182,  2189,  2195,  2196,  2200,  2208,  2210,  2212,  2214,  2216,
    2218,  2220,  2224,  2225,  2232,  2242,  2243,  2247,  2248,  2254,
    2258,  2260,  2262,  2264,  2266,  2268,  2270,  2272,  2274,  2276,
    2278,  2280,  2287,  2288,  2291,  2292,  2295,  2300,  2307,  2308,
    2312,  2321,  2327,  2328,  2331,  2332,  2333,  2334,  2351,  2356,
    2361,  2380,  2397,  2404,  2405,  2412,  2416,  2422,  2428,  2436,
    2440,  2446,  2450,  2454,  2460,  2464,  2471,  2477,  2483,  2491,
    2496,  2501,  2508,  2509,  2510,  2513,  2514,  2517,  2518,  2519,
    2526,  2530,  2541,  2547,  2601,  2663,  2664,  2671,  2684,  2689,
    2694,  2701,  2703,  2710,  2711,  2712,  2716,  2721,  2726,  2737,
    2738,  2739,  2742,  2746,  2750,  2752,  2756,  2760,  2761,  2764,
    2768,  2772,  2773,  2776,  2778,  2782,  2783,  2787,  2791,  2792,
    2796,  2797,  2801,  2802,  2803,  2804,  2807,  2809,  2813,  2815,
    2819,  2821,  2824,  2826,  2828,  2832,  2834,  2836,  2840,  2842,
    2844,  2848,  2852,  2854,  2856,  2860,  2862,  2866,  2867,  2871,
    2875,  2877,  2881,  2882,  2887,  2895,  2899,  2901,  2903,  2907,
    2909,  2913,  2914,  2918,  2922,  2924,  2926,  2930,  2932,  2936,
    2938,  2942,  2945,  2953,  2957,  2959,  2963,  2965,  2969,  2971,
    2978,  2990,  3004,  3008,  3012,  3016,  3020,  3022,  3024,  3026,
    3030,  3032,  3034,  3038,  3040,  3042,  3046,  3048,  3052,  3054,
    3058,  3059,  3063,  3064,  3068,  3069,  3074,  3078,  3079,  3083,
    3084,  3085,  3089,  3094,  3095,  3099,  3101,  3105,  3106,  3110,
    3112,  3116,  3119,  3124,  3126,  3130,  3131,  3136,  3141,  3143,
    3145,  3147,  3149,  3151,  3153,  3155,  3162,  3164,  3168,  3169,
    3175,  3178,  3180,  3184,  3186,  3193,  3195,  3199,  3203,  3205,
    3207,  3213,  3215,  3219,  3220,  3224,  3226,  3229,  3230,  3234,
    3236,  3238,  3242,  3243,  3247,  3249,  3256,  3259,  3266,  3267,
    3270,  3274,  3278,  3285,  3286,  3289,  3294,  3299,  3300,  3304,
    3308,  3309,  3312,  3313,  3320,  3322,  3326,  3328,  3330,  3333,
    3334,  3337,  3341,  3343,  3346,  3348,  3354,  3355,  3359,  3363,
    3364,  3369,  3370,  3374,  3378,  3379,  3384,  3385,  3388,  3393,
    3398,  3401,  3408,  3409,  3411,  3412,  3414,  3418,  3419,  3421,
    3423,  3427,  3429,  3433,  3434,  3436,  3440,  3441,  3442,  3443,
    3444,  3445,  3446,  3447,  3448,  3449,  3454,  3456,  3458,  3460,
    3462,  3464,  3466,  3468,  3475,  3477,  3479,  3481,  3483,  3485,
    3487,  3489,  3491,  3493,  3495,  3497,  3499,  3501,  3503,  3505,
    3509,  3510,  3516,  3518,  3523,  3525,  3527,  3529,  3534,  3536,
    3540,  3542,  3546,  3548,  3550,  3552,  3556,  3560,  3564,  3566,
    3570,  3575,  3580,  3587,  3592,  3597,  3606,  3607,  3611,  3615,
    3629,  3647,  3648,  3649,  3650,  3651,  3652,  3653,  3654,  3655,
    3656,  3658,  3660,  3667,  3669,  3671,  3678,  3685,  3692,  3694,
    3696,  3697,  3698,  3699,  3701,  3703,  3707,  3711,  3727,  3743,
    3747,  3751,  3752,  3756,  3757,  3761,  3763,  3765,  3767,  3769,
    3773,  3774,  3776,  3792,  3808,  3812,  3816,  3818,  3822,  3826,
    3829,  3832,  3835,  3838,  3843,  3845,  3850,  3851,  3855,  3862,
    3869,  3876,  3883,  3884,  3888,  3894,  3895,  3896,  3897,  3900,
    3902,  3904,  3907,  3914,  3923,  3930,  3939,  3941,  3943,  3945,
    3951,  3960,  3963,  3967,  3968,  3972,  3981,  3983,  3987,  3989,
    3993,  3999,  4000,  4003,  4007,  4011,  4012,  4015,  4017,  4021,
    4023,  4027,  4029,  4033,  4035,  4039,  4042,  4045,  4048,  4050,
    4052,  4054,  4056,  4058,  4060,  4062,  4066,  4067,  4070,  4078,
    4081,  4084,  4087,  4090,  4093,  4096,  4099,  4102,  4105,  4108,
    4111,  4114,  4117,  4120,  4123,  4126,  4129,  4132,  4135,  4138,
    4141,  4144,  4147,  4152,  4153,  4158,  4160,  4161,  4162,  4163,
    4164,  4165,  4166,  4167,  4168,  4169,  4170,  4171,  4172,  4174,
    4175,  4176
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "ACTIVE", "ADD", "AFTER", "ALL", "ALTER", 
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
  "OPEN", "CLOSE", "FETCH", "ROWS", "BLOCK", "'='", "'<'", "'>'", "'+'", 
  "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", 
  "'.'", "'?'", "top", "statement", "grant", "prot_table_name", 
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

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   285,   285,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   287,   287,   287,   287,   287,   288,   288,
     289,   289,   289,   290,   290,   291,   292,   292,   292,   292,
     292,   293,   293,   294,   294,   295,   296,   296,   296,   296,
     296,   296,   296,   297,   298,   298,   298,   298,   299,   299,
     299,   299,   300,   300,   301,   301,   301,   302,   302,   303,
     304,   304,   305,   305,   306,   307,   307,   308,   309,   309,
     309,   310,   310,   310,   311,   311,   312,   312,   313,   313,
     314,   314,   314,   314,   314,   315,   316,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   318,   319,
     319,   319,   320,   321,   321,   322,   322,   323,   323,   323,
     324,   325,   325,   325,   326,   326,   327,   327,   328,   328,
     329,   329,   330,   331,   331,   332,   332,   333,   333,   334,
     334,   335,   336,   336,   337,   338,   339,   340,   341,   342,
     342,   343,   344,   344,   345,   345,   346,   346,   346,   346,
     346,   347,   347,   348,   348,   349,   349,   349,   349,   349,
     350,   350,   350,   350,   351,   351,   352,   352,   353,   354,
     355,   355,   356,   357,   358,   358,   359,   360,   361,   361,
     362,   362,   363,   363,   364,   364,   364,   365,   365,   365,
     366,   367,   368,   368,   368,   369,   369,   370,   370,   371,
     371,   371,   372,   373,   373,   374,   374,   375,   375,   376,
     377,   378,   379,   380,   380,   381,   381,   381,   381,   381,
     381,   382,   382,   383,   383,   384,   385,   385,   385,   385,
     385,   386,   387,   387,   388,   388,   388,   388,   389,   390,
     391,   392,   392,   393,   394,   394,   394,   394,   394,   395,
     396,   397,   397,   397,   397,   398,   399,   400,   401,   402,
     402,   403,   403,   404,   404,   405,   406,   406,   407,   408,
     408,   408,   409,   409,   410,   410,   411,   412,   412,   413,
     414,   414,   415,   415,   416,   417,   417,   418,   419,   419,
     419,   420,   420,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   422,   422,   423,   424,   425,   426,   426,   427,
     428,   429,   430,   430,   431,   432,   433,   434,   435,   435,
     435,   436,   436,   436,   437,   437,   437,   437,   438,   439,
     439,   440,   440,   440,   441,   441,   442,   442,   443,   444,
     444,   445,   445,   445,   445,   446,   446,   446,   447,   448,
     449,   450,   451,   452,   453,   453,   454,   454,   455,   456,
     457,   458,   459,   459,   459,   460,   461,   462,   462,   463,
     463,   464,   464,   465,   466,   467,   468,   469,   469,   470,
     471,   472,   472,   472,   473,   474,   474,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   476,   476,   477,   478,   479,   479,   479,   479,
     479,   479,   479,   480,   481,   482,   482,   483,   483,   483,
     483,   483,   483,   483,   484,   484,   485,   485,   485,   485,
     485,   485,   485,   486,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   488,   488,   489,   489,   490,   491,   491,   491,   492,
     492,   493,   494,   494,   495,   495,   495,   495,   495,   495,
     495,   495,   496,   496,   497,   498,   498,   499,   499,   500,
     501,   501,   501,   501,   501,   501,   501,   501,   501,   501,
     501,   501,   502,   502,   503,   503,   504,   504,   505,   505,
     506,   506,   507,   507,   508,   508,   508,   508,   508,   508,
     508,   508,   508,   509,   509,   510,   510,   510,   510,   511,
     511,   512,   512,   512,   513,   513,   514,   514,   514,   515,
     515,   515,   516,   516,   516,   517,   517,   518,   518,   518,
     519,   519,   520,   520,   520,   521,   521,   522,   522,   522,
     522,   523,   523,   524,   524,   524,   525,   525,   525,   526,
     526,   526,   527,   528,   529,   529,   530,   531,   531,   532,
     533,   534,   534,   535,   535,   536,   536,   537,   538,   538,
     539,   539,   540,   540,   540,   540,   541,   541,   542,   542,
     543,   543,   544,   544,   544,   545,   545,   545,   546,   546,
     546,   547,   548,   548,   548,   549,   549,   550,   550,   551,
     552,   552,   553,   553,   554,   555,   556,   556,   556,   557,
     557,   558,   558,   559,   560,   560,   560,   561,   561,   562,
     562,   563,   563,   563,   564,   564,   565,   565,   566,   566,
     567,   568,   569,   570,   571,   572,   573,   573,   573,   573,
     574,   574,   574,   575,   575,   575,   576,   576,   577,   577,
     578,   578,   579,   579,   580,   580,   581,   582,   582,   583,
     583,   583,   584,   585,   585,   586,   586,   587,   587,   588,
     588,   589,   589,   590,   590,   591,   591,   592,   593,   593,
     593,   593,   593,   593,   593,   593,   594,   594,   595,   595,
     596,   597,   597,   598,   598,   599,   599,   600,   601,   601,
     601,   601,   601,   602,   602,   603,   603,   604,   604,   605,
     605,   605,   606,   606,   607,   607,   608,   608,   609,   609,
     610,   611,   612,   613,   613,   614,   615,   616,   616,   617,
     618,   618,   619,   619,   620,   620,   621,   621,   621,   622,
     622,   512,   623,   623,   624,   624,   625,   625,   626,   627,
     627,   628,   628,   629,   630,   630,   631,   631,   631,   632,
     633,   633,   634,   634,   634,   634,   634,   635,   635,   635,
     635,   635,   635,   636,   636,   636,   637,   637,   637,   637,
     637,   637,   637,   637,   637,   637,   638,   638,   638,   638,
     638,   638,   638,   638,   639,   639,   639,   639,   639,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   639,   639,
     640,   640,   641,   641,   642,   642,   642,   642,   643,   643,
     644,   644,   645,   645,   645,   645,   646,   647,   648,   648,
     649,   649,   649,   650,   650,   650,   651,   651,   652,   653,
     654,   655,   655,   655,   655,   655,   655,   655,   655,   655,
     655,   655,   655,   655,   655,   655,   655,   655,   655,   655,
     655,   655,   655,   655,   655,   655,   655,   656,   656,   656,
     657,   658,   658,   659,   659,   660,   660,   660,   660,   660,
     661,   661,   661,   661,   661,   662,   663,   663,   664,   665,
     665,   665,   665,   665,   666,   666,   667,   667,   668,   669,
     670,   671,   672,   672,   673,   674,   674,   674,   674,   675,
     675,   675,   675,   675,   675,   675,   675,   675,   675,   675,
     676,   677,   678,   679,   679,   680,   681,   681,   682,   682,
     683,   684,   684,   685,   685,   686,   686,   687,   687,   688,
     688,   689,   689,   690,   690,   691,   692,   693,   694,   694,
     694,   694,   694,   694,   694,   694,   695,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   721,   722,   722,   722,   722,   722,
     722,   722,   722,   722,   722,   722,   722,   722,   722,   722,
     722,   722
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
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
       0,     4,     0,     1,     3,     4,     1,     3,     2,     3,
       3,     1,     1,     0,     1,     2,     4,     1,     1,     3,
       1,     0,     2,     1,     6,     1,     1,     3,     1,     2,
       0,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     2,     1,     1,     1,     2,     2,
       1,     2,     3,     4,     2,     6,     4,     1,     1,     8,
       9,     6,     8,     6,     4,     0,     4,     2,     1,     3,
       0,     2,     4,     0,     1,     1,     3,     3,     7,     2,
       0,     2,     2,     3,     3,     0,     1,     2,     4,     1,
       3,     2,     2,     2,     1,     1,     1,     1,     2,     2,
       5,     0,     4,     7,     3,     0,     1,     3,     3,     7,
       7,     1,     1,     3,     4,     7,     2,     1,     3,     1,
       1,     6,     3,     0,     0,     0,     0,     3,     0,     9,
       9,     1,     1,     0,     2,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     5,     5,     5,     5,
       5,     5,     2,     0,     4,     2,     3,     3,     2,     2,
       3,     3,     2,     3,     6,     1,     2,     4,     3,     2,
       2,     2,     2,     3,     1,     3,     3,     3,     2,     2,
       4,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     1,     0,     2,
       2,     0,     1,     2,     2,     2,     2,     2,     4,     3,
       2,     2,     1,     3,     7,     1,     0,     1,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
       2,     2,     1,     1,     1,     1,     4,     5,     1,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     6,     5,     3,
       0,     2,     2,     0,     3,     0,     4,     1,     5,     4,
       1,     4,     1,     2,     2,     1,     1,     1,     2,     2,
       2,     2,     0,     3,     5,     1,     1,     2,     3,     1,
       2,     3,     0,     1,     1,     1,     5,     5,     6,     1,
       1,     1,     2,     4,     1,     0,     5,     1,     0,     3,
       2,     1,     0,     2,     0,     1,     0,     3,     1,     0,
       1,     2,     1,     1,     1,     1,     2,     2,     1,     2,
       3,     1,     1,     3,     3,     1,     2,     3,     1,     2,
       0,     2,     1,     1,     0,     1,     1,     1,     3,     2,
       3,     0,     1,     3,     4,     5,     1,     3,     4,     3,
       0,     1,     3,     3,     1,     1,     0,     1,     1,     4,
       0,     2,     4,     0,     3,     0,     2,     0,     2,     0,
       9,    11,     0,     0,     0,     0,     3,     2,     1,     0,
       3,     5,     3,     2,     5,     2,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     0,     2,     1,     3,     1,
       1,     1,     6,     1,     0,     3,     0,     1,     3,     6,
       3,     4,     2,     3,     0,     1,     2,     1,     1,     1,
       2,     1,     2,     1,     2,     0,     3,     0,     1,     3,
       1,     2,     0,     2,     0,     2,     0,     4,     1,     2,
       1,     1,     0,     1,     3,     2,     1,     1,     2,     1,
       4,     3,     1,     3,     4,     0,     8,     5,     1,     1,
       4,     4,     4,     1,     1,     5,     5,     1,     3,     3,
       1,     1,     1,     3,     7,     7,     5,     3,     0,     1,
       1,     1,     2,     0,     1,     1,     1,     0,     3,     1,
       3,     1,     0,     3,     1,     3,     1,     3,     3,     1,
       1,     3,     1,     2,     1,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       1,     1,     5,     6,     3,     4,     5,     6,     3,     4,
       3,     4,     3,     4,     4,     5,     4,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,    11,
      11,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       4,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     4,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       6,     1,     6,     1,     4,     7,     2,     0,     4,     3,
       6,     1,     1,     6,     6,     1,     1,     4,     6,     4,
       5,     3,     5,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   602,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   602,   679,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   584,    18,   589,   590,   591,     5,    17,   583,
     585,    19,   650,   646,    11,     8,   758,   759,    21,   763,
     764,     4,   491,     0,     0,     0,     0,     0,     0,   425,
     601,   604,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,     0,     0,    96,   656,   938,     0,    22,   936,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,     0,   375,    30,
      38,    35,    37,   787,    36,   787,  1023,  1025,  1027,  1026,
    1032,  1028,  1035,  1033,  1029,  1031,  1034,  1030,  1036,  1037,
    1038,  1040,  1041,  1039,     0,    32,     0,    33,     0,    67,
      69,  1015,  1024,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   997,   673,   678,     0,     0,   609,
       0,   715,   717,  1017,     0,     0,     0,   108,   592,  1022,
       0,     2,     0,     0,   663,     0,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   453,   454,     0,  1007,   432,     0,  1011,
     429,   270,  1014,     0,   717,   428,   403,  1018,   606,   599,
     934,     0,   104,   152,   151,   105,   134,   219,   799,  1003,
       0,   103,   146,  1010,     0,    99,   270,   106,   940,   123,
     100,   204,   101,     0,   102,   787,  1021,   107,   147,   654,
     655,     0,   939,   937,     0,    75,     0,  1008,   734,   517,
    1006,   510,     0,   516,   521,   511,   512,   519,   513,   514,
     515,   520,   340,     0,   272,    31,     0,    40,   786,    39,
       0,     0,     0,     0,     0,     0,   792,     0,     0,     0,
       0,     0,     0,     0,   598,   944,     0,   925,   672,   672,
     673,   685,   683,   996,   686,     0,   687,   677,   673,     0,
       0,     0,     0,     0,     0,   625,   618,   607,   608,   610,
     612,   613,   614,   621,   622,   615,     0,   716,  1016,   109,
     270,   110,   204,   111,   787,   595,     0,     0,   647,     0,
     665,     0,     0,     0,     0,     0,   430,   492,   395,     0,
     395,     0,   222,   431,   435,   426,   489,   490,     0,   272,
     243,   481,     0,   427,   444,     0,   401,   402,   506,   605,
     603,   935,   149,   149,     0,     0,     0,   161,   153,   154,
     133,     0,    97,     0,     0,   112,   272,   122,   121,   124,
       0,     0,     0,     0,     0,   999,    76,   222,   553,     0,
     760,   761,   518,     0,     0,     0,     0,   903,   931,     0,
     998,     0,   905,     0,     0,   932,     0,   926,   917,   916,
     915,  1023,     0,     0,     0,   907,   908,   909,   918,   919,
     927,   928,  1027,   929,   930,     0,  1032,  1028,   933,     0,
       0,     0,     0,   886,   372,   772,   338,   881,   796,   770,
     906,   882,   920,   884,   885,   900,   901,   902,   921,   883,
     945,   946,   947,   961,   948,   963,   889,   887,   888,   971,
     972,   975,   976,   771,     0,     0,  1003,     0,     0,     0,
     376,   220,     0,     0,     0,   789,     0,     0,    28,    34,
       0,     0,    44,    70,    72,  1019,    68,     0,     0,     0,
     791,     0,    53,     0,     0,     0,     0,    52,   597,     0,
       0,     0,   682,   680,     0,   689,     0,   688,   690,   695,
     676,     0,   644,     0,   619,   630,   616,   630,   617,   631,
     637,   641,   642,   626,   611,   734,   767,   800,     0,     0,
     272,     0,     0,   594,   593,   649,   651,   656,   648,   661,
       0,   669,   187,   178,     0,   494,   497,   175,   174,   130,
       0,   500,   495,     0,   501,   149,   149,   149,   149,   502,
     496,   493,     0,   439,     0,   441,   440,     0,   442,     0,
     436,     0,     0,   273,     0,     0,   448,     0,   449,   395,
     482,     0,   488,     0,     0,     0,   485,  1003,   406,   405,
     505,     0,   423,   150,     0,     0,   159,     0,   158,     0,
       0,   148,   162,   163,   167,   166,   165,   155,   553,   565,
     540,   575,   543,   566,   576,     0,   582,   544,     0,   572,
       0,   567,   579,   539,   562,   541,   542,   537,   395,   522,
     523,   524,   532,   538,   525,   534,   555,     0,   560,   557,
     535,   572,   536,   113,   270,   114,     0,     0,   125,     0,
       0,   203,   243,   403,   393,     0,   222,     0,    82,    84,
       0,     0,     0,   781,     0,     0,     0,     0,   870,   871,
     872,     0,   733,   804,   813,   816,   821,   817,   818,   819,
     823,   824,   822,   825,   820,   802,     0,   997,     0,   997,
     922,     0,     0,   997,   997,   997,     0,   923,   924,     0,
       0,   986,     0,     0,     0,     0,   891,   890,   679,     0,
       0,   770,   337,  1020,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,   532,   555,     0,     0,   374,
       0,   283,     0,   788,    29,     0,     0,    45,    73,     0,
       0,    27,   778,     0,   794,   679,     0,   757,   778,     0,
       0,     0,     0,   596,     0,   672,     0,     0,   734,     0,
     694,     0,   587,     0,   586,   942,     0,   620,     0,   628,
     624,   623,     0,   634,     0,   639,   627,     0,   765,   766,
       0,     0,     0,   243,   393,     0,   660,     0,   667,     0,
     645,     0,     0,     0,   131,   188,   499,     0,     0,     0,
       0,     0,   395,   438,     0,   395,   396,   443,   393,     0,
     269,   393,   242,  1004,   214,   211,     0,   224,   395,   522,
     393,     0,     0,     0,   241,   244,   245,   246,   247,     0,
     447,   487,   486,   446,   445,     0,     0,     0,   409,   407,
     408,   404,     0,   395,   197,   156,   160,     0,     0,   164,
       0,   550,   563,   564,   580,     0,   577,   582,     0,   570,
     568,   569,   136,     0,     0,     0,   533,     0,     0,     0,
       0,   571,   272,     0,   393,   126,   202,     0,   205,   207,
     208,     0,     0,     0,     0,   222,   222,    79,     0,    86,
      78,   551,   552,  1000,   553,     0,     0,     0,   815,   803,
       0,     0,   873,   874,   875,     0,   807,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     988,   989,   990,   991,   992,   993,   994,   995,     0,     0,
       0,     0,   981,     0,     0,     0,     0,     0,   997,   339,
     899,   898,   773,   911,     0,   894,  1002,   893,   892,   895,
     896,   897,   969,     0,   904,   798,   797,   378,   377,     0,
     276,   291,     0,   282,   284,   790,     0,     0,     0,     0,
       0,     0,    25,    54,    42,    62,    64,     0,     0,    71,
       0,   783,     0,   793,   997,     0,   783,    50,    48,     0,
       0,     0,   681,   684,     0,   696,   697,   701,   699,   700,
     714,     0,   727,   691,   693,  1012,   588,   943,   629,   638,
     633,   632,     0,   643,   768,   769,   801,   393,     0,     0,
     652,   674,   653,   662,     0,   664,   668,   177,   184,   183,
       0,   180,   498,   149,   194,   179,   189,   190,   171,   170,
     941,   173,   172,   503,     0,     0,     0,   437,   393,   393,
     396,   281,   274,   283,   213,   395,     0,   396,   215,   210,
     216,     0,     0,   252,     0,   451,   396,   395,   393,   450,
       0,     0,     0,   422,   508,   198,   199,   157,     0,   169,
       0,     0,     0,   555,     0,   578,     0,     0,   395,   396,
       0,   528,   530,     0,     0,     0,     0,     0,     0,     0,
     403,   283,   149,   128,   243,   200,   423,   997,   398,   381,
     382,     0,    83,     0,   222,     0,    88,     0,    85,     0,
       0,     0,   762,  1005,     0,     0,   808,     0,     0,   814,
       0,     0,   806,   805,     0,   860,     0,   851,   850,     0,
     829,     0,   858,   876,   868,     0,     0,     0,   830,   854,
       0,     0,   833,     0,     0,     0,     0,     0,     0,     0,
     831,     0,     0,   832,     0,   862,     0,     0,   826,     0,
       0,   827,     0,     0,   828,     0,     0,   222,     0,   949,
       0,     0,   964,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   987,     0,     0,   985,     0,     0,   977,     0,
       0,     0,     0,     0,   910,   968,     0,   271,   290,     0,
     350,   373,   285,    66,    58,    59,    65,    60,    61,     0,
       0,     0,    23,    26,    42,    43,   553,   553,     0,   775,
     795,     0,     0,   774,    51,    49,    46,     0,     0,   725,
     699,     0,   723,   718,   719,   721,     0,     0,   712,     0,
     732,   635,   636,   640,   283,   201,   398,     0,   666,   182,
       0,     0,     0,   195,     0,   191,     0,     0,     0,   433,
     230,   225,   913,   226,   227,   228,   229,     0,     0,   275,
       0,     0,   223,   231,     0,   252,   656,   248,     0,   452,
     483,   484,   414,   415,   411,   410,   413,   412,   395,   507,
     396,   168,  1001,     0,     0,   546,     0,   545,   581,     0,
     573,     0,   137,     0,   526,     0,   554,   555,   561,   559,
       0,   556,   393,     0,     0,     0,   120,   129,   206,   395,
       0,     0,   394,     0,    98,     0,   118,   117,    94,     0,
       0,    90,     0,    87,     0,   866,   867,   810,   809,   812,
     811,     0,     0,     0,   679,     0,     0,   869,     0,     0,
       0,     0,     0,     0,   861,   859,   855,     0,   863,     0,
       0,     0,     0,   864,     0,     0,     0,     0,     0,     0,
     955,   954,     0,     0,   951,   950,     0,   959,   958,   957,
     956,   953,   952,     0,   967,   983,   982,     0,     0,     0,
       0,     0,     0,     0,   912,   277,     0,     0,   287,   288,
       0,  1003,     0,     0,     0,     0,     0,     0,     0,  1025,
    1030,     0,     0,   371,   301,   296,     0,   350,   295,   305,
     306,   307,   308,   309,   316,   317,   310,   312,   313,   315,
       0,   320,     0,   365,   367,   366,     0,     0,     0,     0,
       0,     0,  1003,    55,    56,     0,    57,    63,    24,   779,
       0,   780,   777,   782,   785,   784,     0,   756,     0,    47,
     695,   710,   698,   724,   720,   722,     0,     0,     0,     0,
       0,   736,     0,   394,   657,   658,   675,   149,   185,   181,
       0,   197,   196,   192,     0,   396,   914,   279,   280,   396,
     396,   218,   232,   233,   395,     0,   249,     0,     0,     0,
       0,     0,     0,     0,     0,   283,   504,   548,     0,   549,
       0,   135,     0,   218,   138,   139,   141,   142,   143,   529,
     531,   527,   558,   283,   423,   396,   197,     0,     0,     0,
     379,     0,   383,   395,    89,     0,     0,    91,   555,     0,
     852,     0,     0,   997,   878,   877,     0,     0,   856,     0,
       0,     0,     0,   865,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   970,   221,   960,   962,     0,     0,
     984,   979,   978,     0,   973,   974,   734,   224,   286,     0,
     324,     0,     0,   327,   328,     0,   319,     0,   335,   318,
     351,   352,     0,  1013,   368,   369,     0,   297,     0,   302,
     299,   356,     0,     0,   321,     0,   311,   304,   314,   303,
     349,    41,   553,   734,   704,   725,   713,   711,   726,   728,
     730,   731,   742,   670,   396,   380,   659,     0,   176,   193,
     396,   145,   268,     0,     0,   209,   234,     0,     0,     0,
     252,   235,   238,   787,     0,   396,   420,   421,   417,   416,
     419,   418,     0,   547,   574,   144,   132,   140,     0,   395,
     265,   127,   396,     0,   734,   397,   384,     0,     0,    92,
      93,    80,     0,   837,   845,     0,   838,   846,   841,   849,
     853,   857,   839,   847,   840,   848,   834,   842,   835,   843,
     836,   844,   966,   965,   980,   727,     0,   293,   289,     0,
     322,     0,   340,     0,     0,     0,   353,     0,   364,     0,
       0,     0,     0,   359,   357,     0,     0,     0,   344,     0,
     345,     0,   776,   727,   706,   703,     0,     0,   738,   740,
     741,   735,     0,   266,   186,   434,   212,   217,   236,   252,
     787,   239,   258,   251,   253,   424,   396,     0,   399,     0,
     386,   387,   389,   390,   727,   396,    77,    95,     0,   732,
     292,     0,   323,   343,     0,   326,     0,   334,     0,   363,
     362,  1009,   361,   350,     0,     0,     0,     0,   336,     0,
       0,   732,     0,   702,   709,   729,   739,   742,   240,   258,
       0,   252,   254,   255,   267,   396,   725,   389,     0,     0,
     732,     0,   734,   736,     0,     0,     0,     0,   350,   370,
     358,   360,     0,   355,     0,   347,   346,   736,     0,   707,
     746,     0,   743,     0,   747,   252,     0,     0,   250,     0,
     257,     0,   256,   400,   392,   388,     0,   385,   119,   727,
     650,   294,     0,   341,   325,   331,   333,     0,     0,     0,
     350,   650,     0,   705,   737,   742,     0,   749,     0,   745,
     748,   237,     0,     0,   261,   260,   259,   725,   732,   663,
       0,   350,     0,     0,   350,   348,   663,   708,   744,     0,
     755,   264,   262,   263,     0,   736,   880,   342,   332,   350,
     354,   329,   671,     0,   752,     0,   751,   391,   650,   330,
     750,     0,     0,   663,   753,     0,   879,   754,     0,     0,
       0
};

static const short yydefgoto[] =
{
    1938,    20,    21,   477,   124,   125,   126,   127,  1242,   741,
     736,    22,   139,   992,   993,   994,   995,   128,   129,   482,
     483,    23,    82,   386,   889,   657,   658,   659,  1135,  1136,
     245,    24,    74,    25,   157,    26,   375,    75,  1354,   227,
     379,   649,  1123,  1346,   545,   215,   371,  1109,  1543,  1544,
    1545,  1546,  1547,  1548,   221,   237,   212,   594,   213,   367,
     368,   369,   601,   602,   603,   604,   605,   547,   548,   549,
    1050,  1047,  1048,  1279,  1508,   550,  1055,  1056,  1057,  1284,
    1097,   230,   321,   381,   877,   878,   879,   815,   816,   817,
    1665,   577,   467,  1402,   660,  1077,  1289,  1521,  1522,  1523,
    1671,   880,   579,   824,   825,   826,   827,  1307,   828,  1821,
    1822,  1823,  1895,   225,   319,   643,   200,   349,   473,   572,
     573,   979,   468,  1070,   982,   983,   984,  1427,  1428,  1229,
    1728,  1429,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1615,  1453,  1454,  1455,  1456,  1457,  1735,  1458,   433,
     434,  1836,  1749,  1459,  1460,  1461,  1879,  1630,  1631,  1742,
    1743,  1462,  1463,  1464,  1465,  1626,    27,    28,   264,   469,
     470,   234,   323,  1128,  1129,  1130,  1694,  1780,  1781,  1782,
     882,  1560,   564,  1067,  1352,   232,   645,   358,   590,   591,
     841,   843,  1319,    29,    59,   806,   803,   343,   344,   353,
     354,   192,   193,   355,  1086,   585,   833,   197,   165,   336,
     337,   560,   205,   592,  1320,    30,    96,   628,   629,   630,
    1110,  1111,   631,   725,   633,   634,  1103,  1479,   866,   635,
     726,   637,   638,   639,   640,   859,   641,   642,   856,    31,
      32,    33,    34,    35,   534,    36,   499,    37,    38,    61,
     209,   360,    39,   307,   308,   309,   310,   311,   312,   313,
     314,   770,   315,  1032,  1273,   519,   520,   775,   521,    40,
    1466,    42,   164,   535,   536,   241,  1506,  1042,   330,   541,
    1045,   790,    43,   747,   502,   297,  1277,  1656,   144,   145,
     146,   295,   506,   507,   508,   761,   758,  1015,  1016,  1017,
    1754,  1813,  1848,  1018,  1019,  1268,  1783,   151,  1266,  1270,
    1648,  1649,  1501,   390,  1653,  1850,  1762,  1851,  1852,  1853,
    1889,  1923,  1926,  1467,  1468,    46,    47,   391,  1469,    49,
      50,   525,  1470,   435,   436,    51,  1001,  1480,  1249,  1483,
     267,   268,   474,   489,   490,   743,   437,   438,   528,   672,
     905,   673,   674,   675,   676,  1159,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   907,  1162,  1163,  1375,   710,
     686,   440,   441,   964,  1291,   442,   443,   444,   445,   446,
     447,   448,   663,    79,   243,   229,  1061,  1112,   765,   449,
     450,   451,   452,   453,   454,   455,  1599,   456,   457,   458,
     459,   460,   461,   955,   462,   702,  1216,   703,  1213,   948,
     296,   463,   464,   892,  1321,   965,   218,   812,  1142,   249,
     195,   246,  1800,   222,  1924,  1024,  1471,  1020,   130,   465,
     204,   206,   996,   712,   235,   158,   466,   132
};

static const short yypact[] =
{
    3168,   861,    54,  1353,   474,   933,   201,  2471,   163,  1344,
     211,   436,  1470,    54,   885,   432,  5739,   663,  5739,    10,
       7,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   100,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  6909,  5739,  5739,  5739,  5739,  5739,-32768,
  -32768,   361,   836,  5739,  5739,  5739,   480,  5739,   425,  5739,
    5739,-32768,  5739,  5739,-32768,   713,-32768,   431,-32768,-32768,
     702,  5739,-32768,  5739,  5739,  5739,   720,  5739,  5739,  5739,
    5739,   425,  5739,  5739,  5739,  5739,-32768,  5739,   396,   538,
  -32768,-32768,-32768,   531,-32768,   531,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   688,   544,   704,-32768,   344,-32768,
  -32768,-32768,-32768,  5739,  5739,  5739,   768,   743,   756,  1237,
     113,   746,   545,   585,   236,   705,-32768,  5739,   873,   914,
     809,-32768,  5739,-32768,  5739,  5739,  5739,-32768,-32768,-32768,
    5739,-32768,   964,   188,   748,   219,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   706,-32768,-32768,   836,-32768,-32768,   604,-32768,
  -32768,   749,-32768,   886,-32768,-32768,   606,-32768,   852,-32768,
  -32768,   823,-32768,   355,-32768,-32768,  1046,-32768,-32768,-32768,
     836,-32768,-32768,-32768,   648,-32768,   749,-32768,-32768,   501,
  -32768,  1000,-32768,  1002,-32768,   531,-32768,-32768,-32768,-32768,
  -32768,   987,-32768,-32768,   869,-32768,  1006,-32768,   893,-32768,
  -32768,-32768,   869,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  4856,  5739,   937,-32768,  5739,-32768,-32768,-32768,
    3760,   747,   954,  4007,  5739,  1030,   843,  1082,  1094,  3760,
    1022,  1044,  1064,  4007,   945,-32768,  6274,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4512,-32768,-32768,-32768,  1031,
    5739,  1124,  1045,   451,  5739,  1039,-32768,-32768,   914,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  5739,-32768,-32768,-32768,
     749,-32768,  1000,-32768,   531,  1119,  6274,  1075,-32768,  6274,
    1193,    95,   996,    94,  1003,   641,   219,-32768,  1233,   233,
  -32768,  5739,-32768,   706,-32768,-32768,-32768,-32768,  5739,   937,
    2054,  1060,  2679,   999,-32768,  6909,-32768,-32768,   857,-32768,
  -32768,-32768,  1012,  1012,   836,  1151,   836,   602,   355,-32768,
  -32768,  6880,-32768,  5739,  5739,-32768,   937,-32768,-32768,  1250,
     394,  1007,  5739,  1273,  5739,-32768,-32768,    59,   469,  3846,
  -32768,-32768,-32768,  1009,  1019,  1029,  1091,-32768,-32768,  1032,
  -32768,  1033,-32768,  1052,  1055,-32768,  1056,-32768,-32768,-32768,
  -32768,  1058,  1059,  1093,  1101,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1061,-32768,-32768,  4942,  1062,  1063,-32768,  6274,
    6274,  4609,  5739,-32768,-32768,-32768,  1043,  1038,-32768,  1112,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1068,  1049,  1076,  6880,  1069,   575,
  -32768,-32768,  1084,  1337,   599,-32768,  5739,  1178,-32768,-32768,
    5739,  5739,    19,-32768,-32768,-32768,-32768,  5739,  5739,    85,
  -32768,  5739,-32768,  1298,  5739,  3760,  1217,  1089,-32768,  5739,
    5189,   273,-32768,-32768,  6274,-32768,  1301,  1092,-32768,   122,
  -32768,   460,-32768,   338,-32768,   597,-32768,   597,-32768,  1095,
  -32768,   112,-32768,  1200,-32768,   264,-32768,-32768,  1102,  1096,
     937,  1097,  1366,-32768,-32768,  1100,-32768,   350,-32768,  1005,
    1201,  1188,-32768,   139,  1300,  1302,-32768,-32768,-32768,-32768,
     836,-32768,-32768,  1304,-32768,  1012,  1012,  1012,  1012,-32768,
    1115,-32768,  1371,-32768,  1377,-32768,-32768,  1356,-32768,  6880,
  -32768,  6880,   695,-32768,  1400,  5739,-32768,  2598,-32768,   444,
  -32768,  5739,   778,   886,  1230,  1198,  1275,  1238,-32768,-32768,
  -32768,   198,  1276,-32768,  1218,   425,-32768,   836,-32768,  1398,
    1332,-32768,   602,-32768,-32768,-32768,-32768,-32768,   468,  1234,
  -32768,-32768,-32768,  1236,-32768,  1290,  1158,-32768,  1342,  1161,
     442,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1159,-32768,-32768,-32768,   148,  1163,  1165,   275,
  -32768,  1161,-32768,-32768,   749,-32768,  1380,  1435,-32768,   836,
     836,-32768,  2054,   606,-32768,  1317,-32768,  1299,  1170,-32768,
    6842,  5406,  1313,-32768,  1329,  1182,  3932,  1186,  1020,  1180,
    1291,  3594,   216,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2119,   243,  5275,   144,
  -32768,  5739,  6274,   626,   754,   769,  1004,-32768,-32768,  6274,
    4179,  1112,   262,  1268,  6274,  5275,   173,   173,   885,   739,
    1189,   383,-32768,-32768,  5275,  6274,  5739,  6274,  6274,  6274,
    6274,  6274,  5522,  2325,-32768,-32768,  1444,  1185,  5739,-32768,
    5739,  1427,  5739,-32768,-32768,  4391,  1292,-32768,-32768,  1265,
    4007,-32768,  1414,   744,-32768,   885,  1202,-32768,  1414,  4391,
    1412,  1415,  5739,-32768,   383,-32768,   427,  4740,  1284,  5275,
  -32768,  5739,-32768,    49,-32768,-32768,   189,-32768,  1294,-32768,
  -32768,-32768,  5739,   102,  5739,-32768,-32768,  5739,-32768,-32768,
    5275,  5739,  1474,  2054,-32768,  6274,  1243,  6274,  1358,  1242,
  -32768,   836,   836,   836,-32768,   130,-32768,  1218,  1218,  1306,
     425,   641,-32768,-32768,  1225,-32768,-32768,-32768,   141,  5739,
  -32768,-32768,-32768,-32768,  1475,-32768,  1226,  1450,-32768,  1476,
  -32768,  1421,  1422,   531,-32768,-32768,-32768,-32768,-32768,  1491,
  -32768,-32768,-32768,-32768,-32768,  5739,  6799,   425,  1387,  1388,
    1389,-32768,   425,-32768,  1023,-32768,-32768,  1360,   836,-32768,
     455,  1363,-32768,-32768,-32768,   425,-32768,  1158,   569,-32768,
  -32768,-32768,  1482,   569,  1370,   569,-32768,   425,   425,  1244,
     425,-32768,   937,  5739,-32768,  1428,-32768,   792,-32768,-32768,
  -32768,   857,  1372,  5739,   826,   183,-32768,-32768,  1260,  1519,
  -32768,-32768,-32768,-32768,   469,  5739,  1379,  4265,-32768,-32768,
    1379,  3932,-32768,-32768,-32768,   128,-32768,   290,  2059,  4179,
    4179,  6274,  6274,  1808,  1266,   152,  2224,  6274,  2497,   495,
    2779,  2877,  5608,  3138,  3236,  3497,  6274,  6274,  1535,  6274,
    1267,  6274,  1270,   511,  6274,  6274,  6274,  6274,  6274,  6274,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1483,   775,
     174,  5275,-32768,  4179,  6274,   282,   935,  1274,   236,-32768,
  -32768,-32768,-32768,  1112,   568,-32768,-32768,  1526,   173,   173,
     880,   880,-32768,   876,-32768,-32768,-32768,-32768,-32768,   889,
  -32768,  1361,  1539,  1427,-32768,-32768,  5739,  5739,  5739,  5739,
    5739,  5739,  1280,-32768,    31,-32768,-32768,  4391,  1438,-32768,
     411,  1445,  5739,-32768,   236,  5275,  1445,  1280,  1282,  4391,
    3316,  1504,-32768,-32768,  3034,  1307,   992,-32768,-32768,-32768,
    1293,  4179,  1501,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    86,-32768,-32768,-32768,-32768,-32768,   890,  1372,
  -32768,-32768,-32768,  1112,  5739,-32768,-32768,-32768,-32768,-32768,
     902,-32768,-32768,  1012,  1564,-32768,   130,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1303,  4179,  3419,-32768,-32768,-32768,
  -32768,-32768,-32768,  1427,-32768,-32768,  3419,-32768,-32768,-32768,
  -32768,   531,   531,  1330,  1305,-32768,-32768,-32768,-32768,-32768,
     416,   194,   731,-32768,  1575,-32768,-32768,-32768,  5739,-32768,
     474,   906,  1418,  1444,  1308,-32768,  1218,   907,-32768,-32768,
     578,-32768,  1314,  5739,   677,  1318,  1325,   425,  1326,  1586,
     606,  1427,  1012,  1302,  2054,-32768,  1276,   236,  1408,  1423,
  -32768,  1692,-32768,   425,  1471,  1555,-32768,  6842,-32768,   425,
    1375,  1559,-32768,-32768,  1336,  1339,-32768,  4179,  4179,-32768,
    4179,  4179,-32768,  1608,   170,  1112,  1341,-32768,-32768,  1346,
    1112,  5855,-32768,-32768,-32768,  1522,  1347,  1351,  1112,  1041,
    1352,  1354,  1112,  6274,  6274,  1266,  6274,  5941,  1359,  1362,
    1112,  1364,  1365,  1112,  6274,  1112,  1367,  1368,  1112,  1369,
    1373,  1112,  1374,  1376,  1112,   565,   574,-32768,   608,-32768,
     623,  6274,-32768,   667,   678,   696,   738,   804,   858,  6274,
    6274,  5275,-32768,  1577,   317,  1112,  1453,  5275,-32768,  6274,
    6274,  5275,  6274,  6274,-32768,-32768,  5739,-32768,-32768,  5739,
    6544,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  4391,
    1563,  4391,-32768,  1280,    31,-32768,   483,   483,   332,-32768,
  -32768,  4512,   917,-32768,  1280,  1282,  1384,  3316,  1378,   881,
    1383,  4740,  1505,-32768,  1507,  1510,  1560,  5275,   669,  1628,
    1573,-32768,-32768,-32768,  1427,-32768,  1408,    20,  1390,  1485,
     836,  1520,  1218,  1527,  1218,-32768,  4179,   140,   829,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  3419,  3419,-32768,
    1539,  6274,-32768,   494,  1513,  1330,   713,-32768,  4179,-32768,
  -32768,-32768,  1537,  1538,  1540,  1541,  1545,  1547,-32768,-32768,
  -32768,-32768,-32768,  1402,   474,-32768,  1306,-32768,-32768,   569,
  -32768,  3419,   486,   569,-32768,   569,-32768,  1444,-32768,-32768,
    1403,-32768,-32768,   857,  1539,  1218,-32768,  1302,-32768,-32768,
    4512,  1659,-32768,   196,-32768,  1407,-32768,  1390,-32768,  1409,
     836,   160,  1410,-32768,   836,-32768,-32768,-32768,  1608,-32768,
    1608,  6274,  1524,  1524,   885,  1413,   923,-32768,  1524,  1524,
    6274,  1524,  1524,   314,  1112,-32768,  1070,  6274,  1112,  1524,
    1524,  1524,  1524,  1112,  1524,  1524,  1524,  1524,  1524,  1524,
  -32768,-32768,  1416,  6880,-32768,-32768,   903,-32768,-32768,-32768,
  -32768,-32768,-32768,   961,  1108,-32768,-32768,  5275,  5275,  1637,
    1512,   972,   939,  1133,  1112,-32768,  6880,  1420,-32768,-32768,
    1655,  1663,  5073,   359,  1431,  1426,  1612,  6274,  1433,  1434,
    5330,  5739,  5739,-32768,-32768,-32768,  1657,  6509,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     131,-32768,  1436,-32768,-32768,-32768,  1627,  1447,  1458,  1459,
    1460,  1440,   724,-32768,-32768,  1583,-32768,-32768,-32768,-32768,
    1557,-32768,-32768,-32768,-32768,-32768,  1301,-32768,  3316,  1384,
    1719,-32768,   992,-32768,-32768,-32768,  4740,   960,  5739,  6274,
    4179,  1598,  1539,-32768,-32768,-32768,-32768,  1012,-32768,-32768,
     836,  1023,-32768,-32768,   145,-32768,-32768,-32768,-32768,-32768,
    1112,  1717,   494,-32768,   812,  5739,-32768,  1661,   149,  1558,
    1664,  1561,  1700,  1666,  1704,  1427,-32768,-32768,  1473,-32768,
    1477,-32768,  1656,  1717,   486,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1427,  1276,-32768,  1023,  1575,  1687,  1624,
  -32768,  1372,-32768,-32768,-32768,  1640,   104,-32768,  1444,  1641,
    1112,  1480,  1481,   236,-32768,-32768,  1484,  1488,  1112,  1489,
    1492,  6274,  6274,  1112,  1493,  1494,  1495,  1496,  1497,  1498,
    1499,  1500,  1502,  1503,-32768,-32768,-32768,-32768,  6274,  1506,
  -32768,-32768,-32768,  5275,-32768,-32768,  1284,   169,-32768,  1697,
  -32768,  6188,  5739,-32768,-32768,  6274,-32768,  4179,  1112,-32768,
  -32768,-32768,  1486,-32768,-32768,-32768,  5739,-32768,   177,-32768,
    1589,-32768,   443,  1509,-32768,  4664,-32768,-32768,-32768,-32768,
  -32768,-32768,   483,  1284,  5739,   958,-32768,-32768,  1511,-32768,
    1112,   216,   562,-32768,-32768,-32768,-32768,  1218,-32768,-32768,
  -32768,-32768,-32768,  1514,  5739,-32768,-32768,  1682,  1693,  5739,
    1330,-32768,-32768,   531,  5739,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1539,-32768,-32768,-32768,-32768,-32768,  1539,-32768,
  -32768,-32768,-32768,  4997,  1284,-32768,-32768,  6274,   836,-32768,
  -32768,-32768,   836,-32768,-32768,  6274,-32768,-32768,-32768,-32768,
    1112,  1112,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1112,-32768,-32768,  1501,  3419,-32768,-32768,  1517,
  -32768,   998,  4856,   988,   155,  1515,-32768,  1703,-32768,  5739,
    5739,   474,    67,-32768,-32768,   522,  1706,  4179,-32768,   859,
  -32768,  1516,-32768,  1501,  1518,-32768,  4179,  6274,-32768,-32768,
    1676,-32768,  1528,-32768,-32768,-32768,-32768,-32768,-32768,  1330,
     531,-32768,  1668,-32768,-32768,-32768,-32768,  1575,-32768,  4997,
    1521,   992,-32768,-32768,  1501,  1112,-32768,-32768,  1133,  1573,
  -32768,  1379,-32768,  1648,  4664,-32768,  1626,-32768,  4664,-32768,
  -32768,-32768,-32768,  6590,   177,  6274,  4664,   161,-32768,  4664,
    5663,  1573,  5739,-32768,   216,-32768,-32768,  3674,-32768,  1668,
     266,  1330,  1680,  1681,-32768,-32768,   881,  1532,  4997,  1716,
    1573,  1533,  1284,  1598,  1536,  4093,  1542,   913,  6590,  1543,
  -32768,-32768,  1054,   124,  1766,-32768,-32768,  1598,   976,-32768,
  -32768,  1544,  1548,   788,  5739,  1330,   548,   548,-32768,  1775,
  -32768,  1636,-32768,-32768,-32768,   992,  4997,-32768,-32768,  1501,
    1694,-32768,  4664,  1543,-32768,-32768,  1773,  4664,  1789,  1781,
    6590,  1694,  5739,-32768,-32768,  3674,  1554,-32768,  5739,-32768,
  -32768,-32768,  1625,   617,-32768,-32768,-32768,  1042,  1573,   748,
     977,  6590,   159,  5739,  6590,-32768,   748,-32768,-32768,  5739,
    1750,-32768,-32768,-32768,  4179,  1598,-32768,-32768,-32768,  6590,
  -32768,-32768,-32768,  1562,  1567,  1568,-32768,   216,  1694,-32768,
  -32768,  5739,  5739,   748,-32768,  1569,-32768,-32768,  1836,  1837,
  -32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -62,   208,-32768,   228,  1576,   594,-32768,
    -326,-32768,-32768,  -546,  -184,  -631, -1080,  1828,  1578,  1570,
    1114,-32768,-32768,-32768,   718,-32768,  1203,   970,-32768,   726,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   735,-32768,-32768,-32768,-32768,-32768,
     318,-32768,-32768,  1523,-32768,-32768,-32768,  -345,-32768,-32768,
  -32768,  1525,-32768,-32768,  1262,  -273,  1534,-32768,-32768,  -339,
  -32768,  -757,-32768,-32768,-32768,-32768,-32768,-32768,   811,-32768,
   -1272,-32768,-32768,  1550,  1086,   750,  1549,  1065,   751,-32768,
     335,   -55,  -229,-32768,  -320,   268,  -971,-32768,-32768,   364,
  -32768,  1553, -1081,-32768,-32768,-32768,-32768, -1238,   363,    64,
      65,    68,    34,-32768,-32768,-32768,-32768,  -120,  -278,-32768,
    1088,-32768,  -677,-32768,  -984,-32768,   921,-32768,-32768,-32768,
  -32768,-32768, -1337,  -931,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   162,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -336,
     181,-32768,  -364,-32768,-32768,-32768,-32768,-32768,   276,-32768,
     101,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1204,-32768,-32768,   882,-32768, -1176,-32768,-32768, -1572,   146,
    -653,   423,  -321,  -957,   653,-32768,-32768,  -550,  -767,-32768,
  -32768, -1024, -1366,-32768,-32768,-32768,-32768,-32768,  1588,-32768,
    1350,  1579,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1599,-32768,-32768,-32768,-32768,-32768,-32768,  -547,  -436,-32768,
    1071,   609,  -603,  -351,-32768,-32768,-32768,  -319, -1013,-32768,
    -350,-32768,-32768,-32768,-32768,  1310,-32768,-32768,  1087,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1932,
  -32768,-32768,-32768,-32768,-32768,  1644,-32768,-32768,-32768,  1446,
  -32768,  1441,-32768,-32768,-32768,-32768,  1181,-32768,-32768,-32768,
       6,-32768, -1240,-32768,  1172,  -498,-32768,-32768,  -939,-32768,
  -32768,-32768,    29,  -858,  -177,   185,-32768,-32768,  -638,-32768,
    1816,  -893,  -758,-32768,  1205, -1095, -1296,-32768,  -923,-32768,
  -32768,-32768,-32768,   948,-32768,-32768,   722,    30, -1286, -1249,
  -32768,   206, -1161,  -514, -1103,   313,-32768,    81,-32768,   114,
  -32768,  -727,-32768,  1967,  1969,-32768,-32768,  1448,  1971,-32768,
  -32768,-32768,  -193,  -268,  -408,-32768,  1224, -1131,   968,-32768,
     -95,  -679,  -830,-32768,-32768,-32768,  -304,   -38,  -448,  -591,
  -32768,  -428,-32768,-32768,-32768,   433,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  -524,-32768,   800,-32768,   818,-32768,
    1197,  -927,-32768,  -629,-32768,   690,  -910,  -128,  -906,  -904,
    -901,   -12,     0,    24,-32768,   -75,  -749,  -478,  -130,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   757,-32768, -1089,-32768,
     419,  -892,   741,-32768,   867,   319,  -699,  1404, -1121,-32768,
     -59,  1899,-32768,   -47,   -53, -1217,   549,   -24,   -36,  -135,
     -15,   -41,  -179,-32768,   -60,   -39,    -9,-32768
};


#define	YYLAST		7177


static const short yytable[] =
{
     131,   152,   198,   131,    78,   220,    41,   153,   216,   159,
     269,   778,   289,   292,   288,   291,   257,   317,   595,   567,
     632,   636,   569,   709,   976,   217,   251,   509,   606,   233,
     818,   724,   201,   764,   260,  1051,   255,   238,  1144,   786,
     744,   254,  1145,   226,   194,   196,   199,   202,   153,   207,
     214,  1231,   259,   980,   219,   196,   223,   890,   202,   261,
     153,   207,   559,   236,   131,  1222,   256,  1526,   152,   662,
     958,   574,   247,   262,   153,   250,   196,   258,   247,   223,
     199,   202,  1036,   153,   207,   236,   131,   203,   202,  1300,
    1327,  1259,   228,   973,   484,   275,   324,   277,   647,   231,
     299,  1101,  1349,   881,   484,  1302,   376,  1004,  1430,   950,
    1629,  1251,   503,  1299,  1126,   228,  1482,  1803,  1008,   571,
    1303,   325,  1415,   526,   219,   153,   219,  1606,  1419,  1309,
     320,  1039,   760,   807,  1878,   808,  1147,  1344,   223,  1290,
     383,   819,   899,   318,  1083,   202,   153,   236,   909,  1290,
     293,   159,  1332,   909,   791,  1071,  1292,   909,  1073,  1474,
    1293,  1477,  1294,   909,   276,  1295,  1292,  1080,   750,   909,
    1293,   864,  1294,  1498,  1296,  1295,   773,  1562,  1371,   283,
    1566,   529,   909,   345,  1296,   322,  1738,  1068,  -692,   542,
    1643,  1692,   328,   929,   327,  1632,   716,   552,   543,   716,
     530,   717,  1561,  1007,   717,  1699,   794,  1826,   372,  1919,
     797,   798,   799,   800,  1278,  1076,   -81,   493,   515,   739,
     137,  1121,  1524,   331,   909,   471,   632,   636,   475,   532,
     133,  1240,  1053,   162,  1350,  1739,   332,  1271,   898,  1659,
     138,  1314,   293,   906,  1022,   838,  1740,   512,   745,   293,
    1164,  1030,   782,   285,   219,    60,  1865,   219,   829,  1504,
    1148,   153,   160,   606,   485,   131,   565,    83,  1031,  1505,
     153,   333,   910,   334,   485,   746,  1165,   910,   527,   566,
    1026,   910,   839,   161,  1691,   294,   163,   910,  1272,   851,
    1502,   199,   926,   910,  1897,   153,   740,   134,  1150,   716,
     478,  1357,   738,   568,   717,  1054,   910,   862,  1241,   478,
     471,    97,   217,  1856,   582,   951,   952,   586,  1152,  1153,
    1624,  1625,  1581,  1133,  1551,   909,  1517,  1518,  1600,  1601,
    1633,   655,   219,   646,   522,  1217,  1218,   656,  1492,   219,
     716,   219,  1700,   219,  1804,   717,   587,   281,   910,   644,
    1741,    14,   596,   529,   598,  1211,   538,   553,   544,  1127,
    1541,   239,  1214,  1536,   202,   207,  1244,   282,   651,  1519,
    1290,  1290,  1567,   153,   517,   199,   716,  1146,  1255,  1256,
    1107,   717,  1315,   335,  1274,  1696,   840,  1292,  1292,   774,
     274,  1293,  1293,  1294,  1294,  1644,  1295,  1295,   240,  -692,
    1087,  1809,  1304,  1305,  1290,  1296,  1296,  1149,  1477,   716,
    1069,  1825,   653,  1555,   717,  1297,  1298,   792,   930,  1515,
     928,  1292,  1151,   713,  1660,  1293,  1011,  1294,  1675,   865,
    1295,    98,  1771,   751,  1796,  1311,  1809,   957,  1726,  1296,
    1844,  1524,   718,   719,   720,   721,   962,   720,   721,   910,
     527,  1243,  1356,   716,  1857,   135,   737,   362,   717,   953,
     753,  1134,   389,  1254,   844,   860,  1840,   153,   869,   898,
     737,   202,   485,   898,  1287,   504,  1789,  1246,   153,  1219,
     515,  1064,   153,   510,  1066,   202,   153,   224,   650,   766,
     159,   509,  1832,  1486,   363,  1829,   364,  1078,  1417,  1485,
    1312,  1876,  1745,   147,  1811,  1737,   734,  1612,   305,   821,
    -395,  1752,  1035,  1173,  1724,   377,   208,   742,  -243,   365,
     845,   748,  1094,  1509,   872,   478,   273,   575,  1063,  1174,
    1689,  1818,  1376,   861,   890,  1830,  1060,   716,   795,   820,
    1829,   777,   717,  1905,   366,   718,   719,   720,   721,  1425,
    1613,  1682,   755,   870,  1250,  1701,  1367,  1368,  1661,  1369,
    1370,   484,  1662,  1663,  1918,  1137,   813,  1921,   219,  1688,
    1343,  1654,   813,  1645,  1175,  1141,  1554,  1539,  1012,  1829,
     571,   516,  1929,  1858,  1034,   846,   718,   719,   720,   721,
     822,   716,  1558,  1247,  1119,  1849,   717,  1252,  1690,   977,
     716,   210,   211,  1176,  1313,   717,    14,   346,   148,   356,
    1542,  1829,   378,  1614,   217,   149,   287,  1891,  -243,   228,
    1898,   274,   718,   719,   720,   721,  1489,   555,  1833,    76,
    1899,   823,   293,  1027,   716,   242,   517,   875,   876,   717,
    -243,  1906,   529,   219,   932,   661,   661,  -243,   599,   716,
    1847,  1758,   893,   518,   717,   718,   719,   720,   721,  1060,
     661,   891,   961,  1912,   285,  1907,   555,  1058,  1059,  1867,
    1177,  1892,    76,    76,   263,   934,  1759,   556,    76,   760,
    1705,  -243,   223,  1212,   347,   265,   357,    76,  1933,  1553,
     471,   762,   471,   716,   985,  1514,   542,  1763,   717,   718,
     719,   720,   721,  1765,   716,   543,  1013,   966,  1282,   717,
     338,   557,  1893,  1613,   219,  1913,   556,  1528,  1774,   219,
     768,   219,   716,   219,   239,  1062,   485,   717,   737,   558,
    1870,   485,  1100,   763,  1760,  1778,  1573,  1915,   150,   527,
     485,    77,    77,   202,  1881,   217,   850,    77,   202,   285,
     557,  1775,  1025,  1658,  1301,  1790,    77,  1776,   339,  1894,
     293,   240,  1089,   153,   716,   153,  1310,   287,   558,   717,
     244,   471,   219,   285,   219,   293,  1614,  1345,  1316,  1049,
    1049,  1052,  1920,   718,   719,   720,   721,  1331,   252,   285,
    1202,   769,  1115,  1116,   100,  1118,   373,  1085,  1088,  1290,
     219,   716,   522,   936,  1033,   248,   717,  1233,  1527,   266,
    1236,   154,  1928,  1422,  1137,  1317,  1292,   270,   938,  1824,
    1293,   271,  1294,   286,   228,  1295,   219,   219,  1831,   287,
     716,   102,   374,   272,  1296,   717,  1099,   718,   719,   720,
     721,  1210,  1106,   155,  1400,  1223,   718,   719,   720,   721,
    1224,  1540,   728,  1401,   729,  1333,  1595,  1550,   156,  1497,
    1334,   228,   588,   290,   153,   600,  1093,   529,  1863,   287,
     340,  1886,   279,   589,   153,  -695,   732,  1403,   733,  1104,
     718,   719,   720,   721,   716,   280,  1143,  1404,   341,   717,
     350,   228,   228,   351,   228,   718,   719,   720,   721,   278,
     103,    52,  1405,  1120,  -695,  -695,   716,  -695,  1887,  1651,
     104,   717,    53,  1131,  -695,  -695,  -695,  -695,  -695,    54,
    -695,  1888,   342,  -695,  -695,  -695,  -695,  -695,   284,   716,
    1237,  -695,  -695,  1834,   717,   105,  1667,  -695,   352,   718,
     719,   720,   721,  1212,    55,   143,  1407,  1235,  1262,  1212,
     718,   719,   720,   721,  1333,  1238,   300,  1408,  1668,  1337,
    1916,   716,  1263,  1234,   527,  1669,   717,  1922,   718,   719,
     720,   721,   809,   316,   810,  1409,  1027,   485,   202,   207,
     485,   236,   131,   509,   326,   387,  1264,   716,   485,   831,
    1607,   832,   717,   392,  1936,    80,    81,  1535,   716,  1670,
     485,   485,   301,   717, -1013,   202,   475, -1016,   794,    56,
     718,   719,   720,   721,   716,   329,   714,  1410,   959,   717,
    1258,  1002,   359,  1003,   716,  1262,  1734,   348,  1557,   717,
     361,   716,   408,   409,   410,   219,   717,   302, -1036,  1263,
    1265,    57,  1340,   210,   211,    58, -1036,   718,   719,   720,
     721, -1036,   632,   636, -1036,  1473,   370,  1476,  1358,  1262,
     418,   419,   380,  1264,  1362,   303,   382,   716,   304,  1124,
     384,  1125,   717,  1263,  1794,   385,   718,   719,   720,   721,
     716,  -725,   509,  1411,   305,   717,   217,  1756,   388,  1322,
   -1036,   389,  1725,   475,   472,   529,   716,  1264,  1380, -1036,
    1323,   717,   480,   886,  1322,  1132,   927, -1036,   931,  1262,
     306,   976,   935,   937,   939,   219,   487,  1265,  1481,  1481,
    1484,   488,   219,  1263,   142,   143, -1036,  1582, -1036,  1753,
     718,   719,   720,   721,   716,  1808,  1809,  1412,   716,   717,
    1877,   228, -1036,   717, -1036, -1036, -1036,  1264,   491,  1212,
    1212,  1265,  1511,  1223,  1513,  1225,  1807,   228,   492,   716,
    1095,  1096,  1657,   228,   717,  1814,  1226,  1124,  1227,  1275,
     494,  1914,  1598,   495,  1426,   718,   719,   720,   721,  1280,
    1784,  1281,  1596,  1324,  1329,  1325,  1330,   787,   471,  1875,
    1809,   217,   527,   496,   714, -1036,  1487,   498, -1037,   757,
    1223,  1265,  1575,   829,  1934,  1935, -1037,   718,   719,   720,
     721, -1037,  1220,   511, -1037,  1556,   714,   219,  1605,   523,
    1431,  1472,   940,   941,   942,   943,   944,   945,   946,   947,
     485,   513,   485,   718,   719,   720,   721,   714,    14,  1646,
    1597,   514,  1697,    99,   718,   719,   720,   721,   485,   533,
   -1037,  1604,   202,  1882,  1809,  1883,  1917,   540,   551, -1037,
     718,   719,   720,   721,  1795,   554,   562, -1037,  1049,   597,
     718,   719,   720,   721,  1792,   580,   583,   718,   719,   720,
     721,   593,   648,   654,   100,   652, -1037,   687, -1037, -1036,
   -1036, -1036, -1036, -1036, -1036, -1036,   101,   688,   690,  1748,
     697, -1036, -1037, -1036, -1037, -1037, -1037,   689,   698, -1038,
     691,   692,   529,   718,   719,   720,   721, -1038,  1869,   715,
     714,   102, -1038,  1927,  1538, -1038,   718,   719,   720,   721,
     693,  1750,   723,   694,   695,  1212,  -999,   696,   727,   699,
     704,   705,   718,   719,   720,   721,   722,   731,  1565,  1167,
      99,  1171,  1569,  1179,  1182, -1037,  1187,  1190,  1193, -1016,
     735, -1038,   730,  1647,   749,   752,   740,   757,  1777,   759,
   -1038,   780,   772,  1611,   776,   783,   784,   785, -1038,   781,
     718,   719,   720,   721,   718,   719,   720,   721,   789,   788,
     103,   100,   801,    62,   793,   802,   542, -1038,   796, -1038,
     104,   804,   805,   101,    63,   718,   719,   720,   721,   527,
     811,    64,   835, -1038,   836, -1038, -1038, -1038,   837,   842,
    -454,   847,   285,   196,    65,   105,   848,   852,   102,   853,
    1837,  1623,  1143,  1143,  1839,   854,   855,   857,  1472,   858,
     863,   867,  1843,   868,   873,   874,   883,   886,   894, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037,   885,   895,  1748,   439,
     896, -1037,  1748, -1037,   900,   954, -1038,   864,   960,   287,
    1748,  1873,   981,  1845,   997,   998,    99,  1000,  1009,   485,
    1005,  1010,  1021,   501,  1037,    66,  1044,   202,  1028,   318,
    1750,  1041,   439,  1046,  1750,  1074,  1076,   103,  1049,  1748,
    1751,    67,  1750,  1065,  1075,  1846,   814,   104,  1900,  1755,
    1060,  1081,  1082,  1902,  1481,  1084,   153,   100,    68,  1090,
    1091,  1092,  1117,   537,  1098,  1102,   539,  1764,  1108,   101,
    1122,  1750,   105,    69,  1113,  1127,  1748,    70,  1139,  1140,
      71,  1748,   745,   136,  1161,  1197,  1199,  1201,    72,  1209,
     106,  1221,   716,  1228,   102,  1673,  1230,  1239,  1248,  1241,
   -1038, -1038, -1038, -1038, -1038, -1038, -1038,    73,  1750,  1245,
    1257,  1267, -1038,  1750, -1038,  1269,  1283,  1306,  1772,   107,
     108,  1286,   109,  1308,  1261,  1318,  1326,  1328,  1732,   110,
     111,   112,   113,   114,  1335,   115,  1342,  1338,   116,   117,
     118,   119,   120,   202,  1339,  1341,   121,   122,  1351,  1353,
    1360,  1133,   123,  1363,  1364,  1365,   909,  1143,  1366,  1372,
    1377,  1773,   701,   103,  1373,  1378,   706,   707,   711,  1379,
    1381,  1416,  1382,   104,  1418,   318,  1475,  1389,  1746,  1493,
    1390,  1494,  1391,  1392,  1495,  1394,  1395,  1396,  1499,  1496,
    1500,  1397,  1398,  1507,  1399,   966,  1510,  1490,   105,  1751,
     153,  1488,  1491,  1751,  1512,   199,  1525,   732,   529,  1529,
    1530,  1751,  1531,  1532,  1751,  1819,   106,  1533,   152,  1534,
    1799,  1537,  1552,  1559,   153,  1563,  1786,  1374,  1564,  1568,
    1787,  1602,  1574,  1603,  1609,  1594,  1608,   754,   134,  1770,
    1751,   756, -1005,   529,  1617,   107,   108,  1616,   109,  1619,
    1620,  1627,  1634,  1635,  1641,   110,   111,   112,   113,   114,
    1640,   115,   814,  1636,   116,   117,   118,   119,   120,   760,
     196,  1801,   121,   122,  1637,  1638,  1639,  1751,   123,  1642,
    1652,  1802,  1751,  1664,  1674,   529,  1676,  1679,  1677,  1678,
    1680,  1681,  1683,  1693,  1685,  1695,  1684,  1698,  1702,  1703,
    1704,  1729,  1736,  1706,   152,   527,   529,  1707,  1708,   529,
     153,  1709,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,
    1768,  1720,  1721,  1769,   529,  1723,  1628,  1747,  1757,  1798,
    1816,  1797,  1806,  1766,  1472,  1791,  1812,  1820,  1828,  1810,
     527,   219,  1854,  1025,  1835,  1866,  1817,  1838,   153,  1859,
    1861,  1864,  1868,   152,  1156,  1871,  1880,  1157,  1874,   153,
    1809,   393,  1856,  1884,  1857,  1885,  1901,   162,  1903,  1472,
     394,  1904,  1909,  1925,  1911,  1910,  1939,  1940,  1478,  1854,
     140,  1930,   527,   395,  1931,   153,  1932,   479,  1937,   396,
     397,   152,   486,   497,   999,  1361,  1138,   153,  1347,   884,
    1359,   563,  1687,   527,   849,   546,   527,  1285,   908,  1038,
    1854,  1472,   531,  1025,  1348,  1727,   153,   398,  1686,   199,
     399,   527,  1355,  1855,  1079,   439,  1666,  1672,  1862,   933,
    1860,  1896,  1472,   607,  1143,  1472,   949,  1072,   106,   576,
     199,   956,   439,   578,  1232,  1841,  1744,  1805,   401,   402,
    1472,   439,   963,  1793,   967,   968,   969,   970,   971,   963,
     403,  1276,   199,   199,   404,  1827,  1655,   107,   108,  1503,
     109,   570,   978,   834,   584,   561,  1114,   110,   111,   112,
     113,   114,  1549,   115,  1105,   141,   116,   117,   118,   119,
     120,   871,   524,  1029,   121,   122,   439,  1040,   771,   767,
     123,   298,  1260,  1815,  1023,  1761,  1908,    44,  1890,    45,
     266,    48,  1006,   779,  1253,  1385,  1420,   439,  1516,  1158,
    1336,   405,   537,  1767,  1043,   830,   253,   406,     0,  1622,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   409,   410,   411,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
       0,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   911,     0,     0,
     429,   430,     0,     0,     0,   716,   500,   575,   432,     0,
     717,     0,   287,   912,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1154,  1155,
    1160,     0,     0,  1168,  1169,  1172,     0,  1180,  1183,  1185,
    1188,  1191,  1194,  1195,  1196,     0,  1198,     0,  1200,   913,
       0,  1203,  1204,  1205,  1206,  1207,  1208,   911,   914,     0,
       0,     0,     0,     0,     0,   716,   915,     0,   439,     0,
     717,  1215,     0,   912,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   916,     0,   917,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   918,     0,   919,   920,   921,     0,     0,     0,   913,
    1571,  1572,     0,     0,     0,     0,  1576,  1577,   914,  1579,
    1580,     0,   439,     0,     0,     0,   915,  1584,  1585,  1586,
    1587,     0,  1588,  1589,  1590,  1591,  1592,  1593,     0,     0,
       0,     0,     0,     0,     0,   916,     0,   917,     0,     0,
    1166,     0,     0,  1157,   922,     0,     0,   393,     0,     0,
       0,   918,     0,   919,   920,   921,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     106,     0,     0,     0,     0,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     108,     0,   109,   398,   922,     0,   399,     0,     0,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,   401,   402,     0,     0,   923,   924,
     925,   718,   719,   720,   721,     0,   403,     0,   961,     0,
     404,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   963,     0,
       0,     0,     0,     0,     0,     0,     0,   974,     0,     0,
    1383,  1384,     0,  1386,  1388,     0,     0,     0,     0,     0,
       0,  1393,     0,     0,     0,     0,     0,     0,   923,   924,
     925,   718,   719,   720,   721,  1158,     0,   405,  1406,     0,
       0,     0,     0,   406,     0,     0,  1413,  1414,   439,     0,
       0,     0,     0,   407,   439,     0,  1215,  1421,   439,  1423,
    1424,     0,     0,     0,     0,     0,     0,   408,   409,   410,
     411,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,   439,     0,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,   439,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,     0,   429,   430,  1520,     0,
       0,     0,   500,  1170,   432,     0,  1157,     0,   287,     0,
     393,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,    84,     0,     0,     0,     0,     0,     0,    85,
       0,   106,   395,    86,    87,     0,     0,     0,   396,   397,
       0,     0,    88,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
     107,   108,     0,   109,     0,     0,   398,     0,  1570,   399,
     110,   111,   112,   113,   114,     0,   115,  1578,     0,   116,
     117,   118,   119,   120,  1583,     0,     0,   121,   122,     0,
       0,     0,     0,   123,     0,     0,     0,   401,   402,   975,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,   404,   439,   439,     0,   608,     0,    90,
       0,   609,     0,     0,     0,     0,     0,     0,   814,     0,
       0,     0,     0,     0,  1618,     0,    91,     0,     0,   610,
       0,     0,   611,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,  1158,     0,
     405,     0,     0,     0,     0,     0,   406,     0,     0,     0,
       0,     0,     0,   612,     0,    95,   407,     0,     0,   613,
     614,   615,     0,   616,   617,   618,  1650,   619,     0,     0,
     408,   409,   410,   411,   210,   211,     0,     0,     0,     0,
       0,     0,   581,     0,   412,     0,     0,   620,     0,   621,
       0,     0,     0,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   107,   422,     0,   109,     0,     0,   423,   424,
       0,   425,   426,   427,   112,   113,   114,   428,   115,     0,
     622,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,     0,   623,     0,   429,
     430,     0,     0,     0,     0,   500,     0,   432,  1710,  1711,
       0,   287,     0,     0,     0,  1178,     0,     0,  1157,   624,
       0,     0,   393,     0,     0,  1722,     0,     0,     0,     0,
     439,   394,     0,     0,   106,     0,     0,     0,  1731,     0,
       0,     0,  1733,     0,   395,     0,     0,     0,     0,     0,
     396,   397,     0,     0,   625,   626,     0,     0,     0,     0,
       0,     0,     0,   107,   108,     0,   109,     0,     0,     0,
       0,   627,     0,   110,   111,   112,   113,   114,   398,   115,
       0,   399,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
     402,     0,     0,  1181,     0,   106,  1157,     0,     0,     0,
     393,   403,     0,     0,  1785,   404,     0,     0,     0,   394,
       0,     0,  1788,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,   107,   108,     0,   109,   396,   397,
       0,     0,     0,     0,   110,   111,   112,   113,   114,   439,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,   398,   123,     0,   399,
    1158,     0,   405,     0,  1650,     0,     0,     0,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   402,     0,
       0,     0,   408,   409,   410,   411,   210,   211,     0,   403,
       0,     0,     0,   404,     0,     0,   412,     0,     0,     0,
       0,     0,  1842,     0,     0,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   107,   422,     0,   109,     0,     0,
     423,   424,     0,   425,   426,   427,   112,   113,   114,   428,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,  1158,     0,
     405,   429,   430,     0,     0,     0,   406,   500,     0,   432,
       0,     0,     0,   287,     0,     0,   407,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     408,   409,   410,   411,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,     0,     0,     0,     0,
       0,     0,     0,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   107,   422,     0,   109,     0,     0,   423,   424,
       0,   425,   426,   427,   112,   113,   114,   428,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,  1186,   123,     0,  1157,     0,   429,
     430,   393,     0,     0,     0,   500,     0,   432,     0,     0,
     394,   287,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     1,     0,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,    14,     0,     0,
       0,     0,     0,     0,     3,     0,     0,   398,     0,     0,
     399,     4,     0,     5,     0,     6,     0,     0,     0,     0,
       7,     0,     0,     0,     0,     0,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,   402,
     106,     9,  1189,     0,     0,  1157,     0,     0,     0,   393,
     403,     0,    10,     0,   404,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     108,   395,   109,     0,     0,     0,     0,   396,   397,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,   398,     0,     0,   399,  1158,
       0,   405,  1014,     0,     0,     0,     0,   406,     0,    11,
       0,     0,     0,     0,     0,     0,    12,   407,     0,    13,
       0,    14,    15,     0,     0,     0,   401,   402,     0,     0,
       0,   408,   409,   410,   411,   210,   211,     0,   403,     0,
       0,     0,   404,     0,     0,   412,    16,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
     986,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,    17,   123,  1158,     0,   405,
     429,   430,     0,     0,     0,   406,   500,     0,   432,     0,
      18,    19,   287,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     409,   410,   411,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,   412,     0,     0,     0,     0,     0,     0,
     396,     0,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   107,   422,     0,   109,     0,     0,   423,   424,     0,
     425,   426,   427,   112,   113,   114,   428,   115,   398,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,  1192,   123,   989,  1157,     0,   429,   430,
     393,     0,     0,     0,   500,     0,   432,   400,     0,   394,
     287,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,   396,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   398,   115,     0,   399,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,   401,   402,     0,
       0,     0,     0,     0,     0,     0,     0,   393,   407,   403,
       0,     0,     0,   404,     0,     0,   394,     0,     0,     0,
       0,     0,   408,   409,   410,     0,   210,   211,     0,   395,
       0,     0,     0,     0,     0,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   665,     0,     0,     0,     0,     0,
     423,   424,     0,   398,     0,     0,   399,     0,  1158,   428,
     405,     0,     0,     0,     0,     0,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,  1288,     0,   401,   402,     0,     0,     0,     0,
     408,   409,   410,   411,   210,   211,   403,     0,     0,     0,
     404,     0,     0,     0,   412,     0,     0,     0,   901,     0,
       0,     0,     0,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   107,   422,     0,   109,     0,     0,   423,   424,
       0,   425,   426,   427,   112,   113,   114,   428,   115,     0,
       0,   116,   117,   118,   119,   120,     0,   708,     0,   121,
     122,   667,     0,  1758,     0,   123,     0,   405,     0,   429,
     430,     0,     0,   406,     0,   500,     0,   432,     0,     0,
       0,   287,     0,   407,     0,     0,     0,     0,  1759,     0,
       0,     0,     0,     0,     0,     0,     0,   408,   409,   410,
     411,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,     0,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,  1760,     0,   116,   117,
     118,   119,   120,   902,   903,   904,   121,   122,     0,   393,
       0,     0,   123,     0,     0,     0,   429,   430,   394,     0,
       0,     0,   671,     0,   432,     0,     0,     0,   287,     0,
     106,   395,     0,     0,   664,     0,     0,   396,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   665,     0,     0,   107,
     108,     0,   109,     0,     0,   398,     0,     0,   399,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
     476,     0,   123,     0,     0,   393,   401,   402,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,   403,     0,
       0,     0,   404,     0,     0,     0,   106,   395,     0,     0,
     666,     0,     0,   396,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   665,     0,     0,   107,   108,     0,   109,     0,
       0,   398,     0,     0,   399,   110,   111,   112,   113,   114,
       0,   115,     0,   667,   116,   117,   118,   119,   120,   405,
       0,     0,   121,   122,     0,   406,     0,     0,   123,     0,
       0,     0,   401,   402,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,   404,   408,
     409,   410,   411,   210,   211,     0,   897,     0,     0,     0,
       0,     0,     0,   412,     0,     0,     0,     0,     0,     0,
       0,     0,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   107,   422,     0,   109,     0,     0,   423,   424,     0,
     425,   426,   427,   112,   113,   114,   428,   115,     0,   667,
     116,   117,   668,   669,   670,   405,     0,     0,   121,   122,
       0,   406,     0,     0,   123,     0,     0,     0,   429,   430,
       0,   407,     0,     0,   671,     0,   432,     0,     0,     0,
     287,     0,     0,     0,     0,   408,   409,   410,   411,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,     0,   116,   117,   668,   669,
     670,     0,   393,     0,   121,   122,   481,     0,     0,     0,
     123,   394,     0,     0,   429,   430,     0,     0,     0,     0,
     671,     0,   432,   106,   395,     0,   287,     0,     0,     0,
     396,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   665,
       0,     0,   107,   108,     0,   109,     0,     0,   398,     0,
       0,   399,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,     0,     0,   393,   401,
     402,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,   403,     0,     0,     0,   404,     0,     0,     0,   106,
     395,     0,     0,   666,     0,     0,   396,   397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   665,     0,     0,   107,   108,
       0,   109,     0,     0,   398,     0,     0,   399,   110,   111,
     112,   113,   114,     0,   115,     0,   667,   116,   117,   118,
     119,   120,   405,     0,     0,   121,   122,     0,   406,     0,
       0,   123,     0,     0,     0,   401,   402,     0,   407,     0,
       0,  1872,     0,   432,     0,     0,     0,   403,     0,     0,
       0,   404,   408,   409,   410,   411,   210,   211,     0,   897,
       0,     0,     0,     0,     0,     0,   412,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   107,   422,     0,   109,     0,     0,
     423,   424,     0,   425,   426,   427,   112,   113,   114,   428,
     115,     0,   667,   116,   117,   668,   669,   670,   405,     0,
       0,   121,   122,     0,   406,     0,     0,   123,     0,     0,
       0,   429,   430,     0,   407,     0,     0,   671,     0,   432,
       0,     0,     0,   287,     0,   986,     0,     0,   408,   409,
     410,   411,   210,   211,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,     0,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,     0,   116,
     117,   118,   119,   120,     0,   393,     0,   121,   122,     0,
       0,     0,     0,   123,   394,     0,     0,   429,   430,   987,
       0,     0,     0,   671,     0,   432,     0,   395,     0,   287,
       0,     0,     0,   396,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   988,     0,     0,     0,     0,
     989,   398,     0,     0,   399,     0,   990,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,   991,     0,     0,     0,     0,
     400,     0,   401,   402,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,   403,     0,   107,   108,   404,   109,
       0,   394,     0,     0,     0,     0,   110,   111,   112,   113,
     114,     0,   115,     0,   395,   116,   117,   118,   119,   120,
     396,   397,     0,   121,   122,     0,     0,     0,     0,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,   399,     0,     0,     0,   405,     0,     0,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,     0,     0,     0,     0,   400,     0,   401,
     402,     0,     0,     0,     0,   408,   409,   410,   411,   210,
     211,   403,     0,     0,     0,   404,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,     0,   116,   117,   118,   119,
     120,     0,   708,     0,   121,   122,     0,     0,     0,     0,
     123,     0,   405,     0,   429,   430,   505,     0,   406,     0,
     500,     0,   432,     0,     0,     0,   287,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   408,   409,   410,   411,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,   412,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   107,   422,     0,   109,     0,     0,
     423,   424,     0,   425,   426,   427,   112,   113,   114,   428,
     115,     0,     0,   116,   117,   118,   119,   120,     0,   393,
     106,   121,   122,     0,     0,     0,     0,   123,   394,     0,
       0,   429,   430,     0,     0,     0,     0,   500,     0,   432,
       0,   395,     0,   287,     0,     0,     0,   396,   397,   107,
     108,     0,   109,     0,     0,     0,     0,     0,     0,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,   398,   121,   122,   399,     0,
       0,     0,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,   106,     0,     0,     0,
       0,     0,     0,     0,   400,   393,   401,   402,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,   403,     0,
       0,     0,   404,     0,     0,   107,   108,   395,   109,     0,
       0,     0,     0,   396,   397,   110,   111,   112,   113,   114,
       0,   115,     0,     0,   116,   117,   118,   119,   120,     0,
       0,     0,   121,   122,     0,     0,     0,     0,   123,     0,
       0,   398,     0,     0,   399,     0,     0,     0,  1014,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,     0,   406,     0,     0,     0,     0,
       0,     0,   401,   402,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,   404,   408,
     409,   410,   411,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,   412,     0,     0,     0,     0,     0,     0,
       0,     0,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   107,   422,     0,   109,     0,     0,   423,   424,     0,
     425,   426,   427,   112,   113,   114,   428,   115,     0,     0,
     116,   117,   118,   119,   120,   405,     0,     0,   121,   122,
       0,   406,     0,     0,   123,     0,     0,     0,   429,   430,
       0,   407,     0,     0,   431,     0,   432,     0,     0,   700,
     287,     0,     0,     0,     0,   408,   409,   410,   411,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,     0,   116,   117,   118,   119,
     120,     0,   393,   106,   121,   122,     0,     0,     0,     0,
     123,   394,     0,     0,   429,   430,     0,     0,     0,     0,
     500,     0,   432,     0,   395,     0,   287,     0,     0,     0,
     396,   397,   107,   108,     0,   109,     0,     0,     0,     0,
       0,     0,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,   398,   121,
     122,   399,     0,     0,     0,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1779,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,   393,   401,
     402,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,   403,     0,     0,     0,   404,     0,     0,   107,   108,
     395,   109,     0,     0,     0,     0,   396,   397,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,     0,     0,   398,     0,     0,   399,     0,  1610,
       0,     0,   708,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,     0,   406,     0,
       0,     0,     0,   400,     0,   401,   402,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,   404,   408,   409,   410,   411,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,   412,     0,     0,     0,
       0,     0,     0,     0,     0,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   107,   422,     0,   109,     0,     0,
     423,   424,     0,   425,   426,   427,   112,   113,   114,   428,
     115,     0,     0,   116,   117,   118,   119,   120,   405,     0,
       0,   121,   122,     0,   406,     0,     0,   123,     0,     0,
       0,   429,   430,     0,   407,     0,     0,   500,     0,   432,
       0,     0,     0,   287,     0,     0,     0,     0,   408,   409,
     410,   411,   210,   211,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,     0,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,     0,   116,
     117,   118,   119,   120,     0,   393,   106,   121,   122,     0,
       0,     0,     0,   123,   394,     0,     0,   429,   430,     0,
       0,     0,     0,   500,     0,   432,     0,   395,     0,   287,
       0,     0,     0,   396,   397,   107,   108,     0,   109,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
       0,   115,     0,     0,   116,   117,   118,   119,   120,     0,
       0,   398,   121,   122,   399,     0,     0,     0,   123,     0,
       0,     0,     0,     0,     0,     0,  1621,     0,     0,     0,
      76,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,   393,   401,   402,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,   403,     0,     0,     0,   404,     0,
       0,   107,   108,   395,   109,     0,     0,     0,     0,   396,
     397,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,     0,     0,   398,     0,    77,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,   406,     0,     0,     0,     0,     0,     0,   401,   402,
       0,   407,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,   404,   408,   409,   410,   411,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,     0,   116,   117,   118,   119,
     120,   405,     0,     0,   121,   122,     0,   406,     0,     0,
     123,     0,     0,     0,   429,   430,     0,   407,     0,     0,
     500,   972,   432,     0,     0,     0,   287,     0,  1184,     0,
       0,   408,   409,   410,   411,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
       0,     0,   116,   117,   118,   119,   120,     0,   393,   106,
     121,   122,     0,     0,     0,     0,   123,   394,     0,     0,
     429,   430,     0,     0,     0,     0,   500,     0,   432,     0,
     395,     0,   287,     0,     0,     0,   396,   397,   107,   108,
       0,   109,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,   398,   121,   122,   399,     0,     0,
       0,   123,     0,     0,     0,     0,     0,   975,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   401,   402,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,   403,     0,     0,
       0,   404,     0,     0,   107,   108,   395,   109,     0,     0,
       0,     0,   396,   397,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,     0,     0,
     398,     0,     0,   399,     0,     0,     0,     0,  1374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,   406,     0,     0,     0,     0,     0,
       0,   401,   402,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,   404,   408,   409,
     410,   411,   210,   211,     0,     0,     0,     0,     0,     0,
       0,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,     0,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,     0,   116,
     117,   118,   119,   120,   405,     0,     0,   121,   122,     0,
     406,     0,     0,   123,     0,     0,     0,   429,   430,     0,
     407,     0,     0,   500,     0,   432,     0,     0,     0,   287,
       0,  1387,     0,     0,   408,   409,   410,   411,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   412,     0,
       0,     0,     0,     0,     0,     0,     0,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   107,   422,     0,   109,
       0,     0,   423,   424,     0,   425,   426,   427,   112,   113,
     114,   428,   115,     0,     0,   116,   117,   118,   119,   120,
       0,   393,     0,   121,   122,     0,     0,     0,     0,   123,
     394,     0,     0,   429,   430,     0,     0,     0,     0,   500,
       0,   432,     0,   395,     0,   287,     0,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,   401,   402,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
     403,     0,     0,     0,   404,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,     0,     0,     0,     0,   406,     0,     0,
       0,     0,     0,     0,   401,   402,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
     404,   408,   409,   410,   411,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
       0,     0,   116,   117,   118,   119,   120,   405,     0,     0,
     121,   122,     0,   406,     0,     0,   123,     0,     0,     0,
     429,   430,     0,   407,  1730,     0,   500,     0,   432,     0,
       0,     0,   287,     0,     0,     0,     0,   408,   409,   410,
     411,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,     0,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,  1230,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,     0,   429,   430,     0,     0,
       0,     0,   500,     0,   432,     0,     6,     0,   287,     0,
       0,  1230,     0,  -298,     0,     0,     0,  1432,  1433,     0,
    1434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1435,     0,     0,
       0,     6,     0,  1436,     0,     0,     0,     0,  -300,     0,
       0,     0,  1432,  1433,     0,  1434,     0,  1230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1435,     0,     0,     0,     0,     0,  1436,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1432,  1433,
       0,  1434,     0,  1437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1435,     0,
       0,     0,    14,     0,  1436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1438,  1437,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,  1628,    14,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,  1438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,  1437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1439,   108,     0,   109,     0,     0,
     106,     0,     0,    14,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,  1440,   118,   119,   120,  1438,     0,
       0,   121,   122,  1441,  1442,  1443,     0,   123,    16,  1439,
     108,     0,   109,     0,     0,     0,     0,     0,     0,   110,
     111,   112,   113,   114,     0,   115,   106,     0,   116,  1440,
     118,   119,   120,     0,     0,     0,   121,   122,  1441,  1442,
    1443,     0,   123,     0,     0,     0,     0,     0,   608,     0,
       0,     0,   609,     0,     0,  1439,   108,     0,   109,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
     610,   115,     0,   611,   116,  1440,   118,   119,   120,     0,
       0,     0,   121,   122,  1441,  1442,  1443,     0,   123,     0,
       0,   887,     0,     0,     0,   609,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   888,
       0,     0,     0,   610,   612,     0,   611,     0,     0,     0,
     613,   614,   615,     0,   616,   617,   618,     0,   619,   608,
       0,     0,     0,   609,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   620,     0,
     621,   610,     0,     0,   611,     0,     0,   612,     0,     0,
       0,     0,     0,   613,   614,   615,     0,   616,   617,   618,
       0,   619,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   622,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   620,     0,   621,     0,   612,     0,     0,   623,     0,
       0,   613,   614,   615,     0,   616,   617,   618,     0,   619,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     624,     0,     0,     0,   622,     0,     0,     0,     0,   620,
       0,   621,     0,     0,     0,   106,     0,     0,     0,     0,
       0,   623,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   625,   626,     0,     0,     0,
       0,     0,   622,   624,   107,   108,     0,   109,     0,     0,
       0,     0,   627,     0,   110,   111,   112,   113,   114,   623,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   625,   626,
       0,   624,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   627,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,   626,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,   627,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,   182,   183,   107,   108,     0,   109,     0,     0,
     184,   185,     0,     0,   110,   111,   112,   113,   114,   186,
     115,   187,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,   188,   189,   190,   191,   123
};

static const short yycheck[] =
{
       9,    16,    55,    12,     4,    64,     0,    16,    63,    18,
     105,   525,   142,   143,   142,   143,    91,   152,   363,   340,
     371,   371,   342,   431,   723,    63,    85,   295,   367,    70,
     577,   467,    56,   511,    94,   792,    89,    73,   896,   537,
     488,    88,   900,    67,    53,    54,    55,    56,    57,    58,
      62,   982,    93,   730,    63,    64,    65,   660,    67,    95,
      69,    70,   335,    72,    73,   958,    90,  1305,    83,   388,
     708,   349,    81,    97,    83,    84,    85,    92,    87,    88,
      89,    90,   781,    92,    93,    94,    95,    57,    97,  1073,
    1103,  1014,    68,   722,   273,   133,   156,   135,   376,    69,
     147,   850,  1126,   653,   283,  1076,   226,   745,  1229,   700,
    1447,  1004,   289,  1070,   881,    91,  1247,    50,   749,   348,
    1077,   160,  1211,   316,   133,   134,   135,  1423,  1217,  1086,
     154,   784,    10,   569,    10,   571,     8,  1121,   147,  1066,
     235,   577,   666,   152,   823,   154,   155,   156,     8,  1076,
       6,   160,  1109,     8,    15,   808,  1066,     8,   811,  1239,
    1066,  1241,  1066,     8,   134,  1066,  1076,   820,   494,     8,
    1076,    23,  1076,  1268,  1066,  1076,    64,  1353,     8,    66,
      20,   316,     8,   195,  1076,   155,     9,    46,    66,    94,
    1486,  1557,   163,    49,     6,    64,    26,   103,   103,    26,
     320,    31,     6,   749,    31,   101,   545,  1779,   220,    50,
     555,   556,   557,   558,  1044,    46,   157,   279,    29,   200,
      12,   874,  1303,     4,     8,   263,   577,   577,   266,   324,
      19,   200,   102,   133,  1127,    58,    17,   151,   666,  1511,
      12,    47,     6,   671,   758,    47,    69,   300,   163,     6,
      98,   149,   530,   204,   263,   201,  1828,   266,   579,   239,
     132,   270,   252,   602,   273,   274,    33,    66,   166,   249,
     279,    52,   132,    54,   283,   190,   124,   132,   316,    46,
     231,   132,    84,   276,  1556,    49,   186,   132,   202,   608,
    1274,   300,    49,   132,  1866,   304,   277,    86,     8,    26,
     270,  1131,   481,   341,    31,   175,   132,   628,   277,   279,
     348,   148,   350,    47,   352,    53,    54,   355,   909,   910,
    1441,  1442,     8,   140,  1337,     8,  1297,  1298,  1417,  1418,
     199,   384,   341,   374,   304,    53,    54,   278,  1261,   348,
      26,   350,   238,   352,   277,    31,   355,   139,   132,   373,
     173,   163,   364,   488,   366,   181,   327,   263,   263,   163,
    1331,    11,   953,  1320,   373,   374,   997,   139,   380,  1300,
    1297,  1298,   212,   382,   185,   384,    26,   901,  1009,  1010,
     858,    31,   188,   164,  1037,  1561,   188,  1297,  1298,   277,
     277,  1297,  1298,  1297,  1298,  1490,  1297,  1298,    48,   277,
     836,   277,  1081,  1082,  1331,  1297,  1298,   279,  1488,    26,
     269,  1777,   382,  1344,    31,  1068,  1069,   278,   274,   279,
     688,  1331,   132,   432,   279,  1331,   752,  1331,   279,   281,
    1331,   268,  1670,   495,   279,  1088,   277,   705,   269,  1331,
     279,  1522,   272,   273,   274,   275,   714,   274,   275,   132,
     488,   997,  1131,    26,   188,    19,   480,   102,    31,   197,
     499,   278,   198,  1009,   594,    23,  1803,   476,   193,   897,
     494,   480,   481,   901,  1065,   290,  1725,    66,   487,   197,
      29,   802,   491,   298,   805,   494,   495,     7,    94,   151,
     499,   759,  1788,  1251,   139,  1781,   141,   818,   181,  1248,
      84,  1838,    59,    71,  1753,  1626,   476,   148,   170,    65,
      24,  1642,   780,    18,  1603,    14,   155,   487,    24,   164,
     595,   491,   843,  1280,   644,   495,   182,    33,   801,    34,
    1554,  1769,  1161,    91,  1137,  1784,   204,    26,   550,   577,
    1826,   277,    31,  1880,   189,   272,   273,   274,   275,  1226,
     191,  1535,   279,   278,  1002,  1568,  1147,  1148,  1515,  1150,
    1151,   740,  1519,  1520,  1901,   885,   575,  1904,   577,  1553,
    1120,  1502,   581,  1496,    79,   894,  1343,  1326,   755,  1865,
     809,   130,  1919,  1821,   777,   597,   272,   273,   274,   275,
     146,    26,  1350,   182,   872,  1812,    31,  1005,  1555,   727,
      26,   207,   208,   108,   188,    31,   163,     3,   176,     3,
     124,  1897,   111,   254,   652,   183,   284,  1855,   124,   595,
    1869,   277,   272,   273,   274,   275,  1257,    25,  1789,   204,
    1870,   187,     6,   763,    26,   204,   185,   649,   650,    31,
     146,  1881,   777,   652,   691,   177,   177,   153,    46,    26,
    1811,    89,   661,   202,    31,   272,   273,   274,   275,   204,
     177,   661,   279,    46,   204,  1882,    25,   797,   798,  1830,
     175,   123,   204,   204,   278,    49,   114,    75,   204,    10,
    1573,   187,   691,   951,    80,   147,    80,   204,  1928,  1342,
     728,   231,   730,    26,   732,  1286,    94,  1654,    31,   272,
     273,   274,   275,  1660,    26,   103,   279,   716,  1053,    31,
       4,   109,   164,   191,   723,    98,    75,  1308,  1675,   728,
     123,   730,    26,   732,    11,   800,   735,    31,   752,   127,
    1833,   740,   277,   273,   172,  1692,  1374,  1898,    16,   777,
     749,   273,   273,   752,  1847,   783,   278,   273,   757,   204,
     109,  1682,   761,  1510,  1075,  1726,   273,  1688,    52,   211,
       6,    48,   837,   772,    26,   774,  1087,   284,   127,    31,
      68,   809,   781,   204,   783,     6,   254,  1122,    47,   791,
     792,   793,  1903,   272,   273,   274,   275,  1108,    68,   204,
     279,   194,   867,   868,    47,   870,   148,   835,   836,  1726,
     809,    26,   772,    49,   774,    83,    31,   986,  1306,   278,
     989,   148,  1915,  1221,  1134,    84,  1726,   129,    49,  1776,
    1726,   277,  1726,   278,   800,  1726,   835,   836,  1785,   284,
      26,    84,   184,   129,  1726,    31,   848,   272,   273,   274,
     275,    66,   273,   180,   279,   277,   272,   273,   274,   275,
     282,  1329,   277,   279,   279,   277,  1403,  1335,   195,  1267,
     282,   837,     5,   278,   873,   263,   842,  1002,  1825,   284,
     164,    83,   129,    16,   883,   206,   277,  1197,   279,   855,
     272,   273,   274,   275,    26,   129,   895,   279,   182,    31,
       4,   867,   868,     7,   870,   272,   273,   274,   275,   131,
     153,    40,   279,   873,   235,   236,    26,   238,   120,  1500,
     163,    31,    51,   883,   245,   246,   247,   248,   249,    58,
     251,   133,   216,   254,   255,   256,   257,   258,   182,    26,
     990,   262,   263,  1791,    31,   188,   124,   268,    52,   272,
     273,   274,   275,  1211,    83,   240,   279,   988,    67,  1217,
     272,   273,   274,   275,   277,   991,    83,   279,   146,   282,
    1899,    26,    81,   987,  1002,   153,    31,  1906,   272,   273,
     274,   275,   277,   164,   279,   279,  1106,   986,   987,   988,
     989,   990,   991,  1251,    20,   244,   105,    26,   997,   211,
    1426,   213,    31,   252,  1933,    62,    63,  1318,    26,   187,
    1009,  1010,    88,    31,   280,  1014,  1044,   283,  1347,   148,
     272,   273,   274,   275,    26,   267,   277,   279,   279,    31,
    1014,   277,   170,   279,    26,    67,  1617,   278,  1349,    31,
     207,    26,   203,   204,   205,  1044,    31,   123,    18,    81,
     159,   180,  1117,   207,   208,   184,    26,   272,   273,   274,
     275,    31,  1403,  1403,    34,  1239,    10,  1241,  1133,    67,
     231,   232,    62,   105,  1139,   151,    64,    26,   154,   277,
      83,   279,    31,    81,    86,   206,   272,   273,   274,   275,
      26,    89,  1350,   279,   170,    31,  1124,   129,    82,  1098,
      70,   198,  1606,  1131,   157,  1230,    26,   105,    57,    79,
    1100,    31,   148,   277,  1113,   279,   687,    87,   689,    67,
     196,  1810,   693,   694,   695,  1124,    86,   159,  1246,  1247,
    1248,   278,  1131,    81,   239,   240,   106,    57,   108,  1643,
     272,   273,   274,   275,    26,   276,   277,   279,    26,    31,
      86,  1117,   122,    31,   124,   125,   126,   105,    66,  1417,
    1418,   159,  1282,   277,  1284,   279,  1747,  1133,    64,    26,
     137,   138,  1507,  1139,    31,  1756,   277,   277,   279,   279,
     148,   129,    64,   129,  1229,   272,   273,   274,   275,   277,
    1694,   279,   279,   277,   277,   279,   279,   182,  1226,   276,
     277,  1229,  1230,   129,   277,   175,   279,   252,    18,    66,
     277,   159,   279,  1524,  1931,  1932,    26,   272,   273,   274,
     275,    31,   277,   182,    34,  1345,   277,  1226,   279,   180,
    1229,  1230,   218,   219,   220,   221,   222,   223,   224,   225,
    1239,   107,  1241,   272,   273,   274,   275,   277,   163,   279,
     279,   196,  1563,     6,   272,   273,   274,   275,  1257,   130,
      70,   279,  1261,   277,   277,   279,   279,    64,   262,    79,
     272,   273,   274,   275,   276,   262,    33,    87,  1280,   118,
     272,   273,   274,   275,   276,   215,   277,   272,   273,   274,
     275,   269,    32,    10,    47,   278,   106,   278,   108,   269,
     270,   271,   272,   273,   274,   275,    59,   278,   207,  1635,
     207,   281,   122,   283,   124,   125,   126,   278,   207,    18,
     278,   278,  1447,   272,   273,   274,   275,    26,  1832,   281,
     277,    84,    31,  1914,  1324,    34,   272,   273,   274,   275,
     278,  1635,   283,   278,   278,  1603,   278,   278,   269,   278,
     278,   278,   272,   273,   274,   275,   278,    10,  1360,   916,
       6,   918,  1364,   920,   921,   175,   923,   924,   925,   283,
     182,    70,   278,  1498,    66,   148,   277,    66,  1689,   277,
      79,   269,   277,  1432,   174,   278,    10,   277,    87,   283,
     272,   273,   274,   275,   272,   273,   274,   275,   200,   188,
     153,    47,   277,    40,    94,    24,    94,   106,    94,   108,
     163,    24,    46,    59,    51,   272,   273,   274,   275,  1447,
      10,    58,   182,   122,   216,   124,   125,   126,   143,   143,
     182,    23,   204,  1432,    71,   188,    94,   193,    84,   193,
    1794,  1440,  1441,  1442,  1798,   145,   278,    95,  1447,   278,
     281,   278,  1806,   278,    64,    10,   129,   277,   135,   269,
     270,   271,   272,   273,   274,   275,   157,   128,  1794,   262,
     278,   281,  1798,   283,   278,   197,   175,    23,   279,   284,
    1806,  1835,    45,  1809,   182,   210,     6,    63,    66,  1488,
     278,    66,   198,   286,    10,   132,   128,  1496,   194,  1498,
    1794,   248,   295,   251,  1798,    20,    46,   153,  1510,  1835,
    1635,   148,  1806,   278,   278,  1809,    30,   163,  1872,  1644,
     204,    90,    90,  1877,  1642,    24,  1525,    47,   165,   132,
     132,   132,   278,   326,   164,   162,   329,  1657,    46,    59,
     102,  1835,   188,   180,   164,   163,  1872,   184,   278,    20,
     187,  1877,   163,    73,   278,    10,   279,   277,   195,    66,
     206,   277,    26,   192,    84,  1525,    17,   277,   113,   277,
     269,   270,   271,   272,   273,   274,   275,   214,  1872,   131,
      66,   278,   281,  1877,   283,    74,    12,   247,  1673,   235,
     236,   278,   238,   278,   277,    10,   168,   279,  1612,   245,
     246,   247,   248,   249,   280,   251,    10,   279,   254,   255,
     256,   257,   258,  1612,   279,   279,   262,   263,   200,   186,
      55,   140,   268,   238,    55,   279,     8,  1626,   279,   278,
      98,  1674,   425,   153,   278,   278,   429,   430,   431,   278,
     278,    54,   278,   163,   181,  1644,    73,   278,  1632,   134,
     278,   134,   278,   278,   134,   278,   278,   278,    20,    89,
      77,   278,   278,   168,   278,  1664,   136,   279,   188,  1794,
    1669,   277,   279,  1798,   137,  1674,   153,   277,  1803,   132,
     132,  1806,   132,   132,  1809,  1770,   206,   132,  1693,   132,
    1739,   279,   279,    24,  1693,   278,  1698,   163,   279,   279,
    1702,    54,   279,   181,    39,   279,   276,   500,    86,  1669,
    1835,   504,    39,  1838,   278,   235,   236,   276,   238,   276,
     276,    54,   276,    86,   131,   245,   246,   247,   248,   249,
     280,   251,    30,   276,   254,   255,   256,   257,   258,    10,
    1739,  1740,   262,   263,   276,   276,   276,  1872,   268,   182,
     142,  1741,  1877,    26,    83,  1880,   188,    47,    84,   188,
      84,    47,   279,    66,    98,   131,   279,   117,   117,   279,
     279,    64,   276,   279,  1779,  1803,  1901,   279,   279,  1904,
    1779,   279,   279,   279,   279,   279,   279,   279,   279,   279,
      98,   279,   279,    90,  1919,   279,   197,   278,   277,    86,
     114,   276,    86,   279,  1803,   278,   278,   129,   277,   283,
    1838,  1810,  1817,  1812,   156,    89,   278,   181,  1817,   129,
     129,   279,   279,  1828,     6,   279,    50,     9,   276,  1828,
     277,    13,    47,   279,   188,   277,    53,   133,    39,  1838,
      22,    50,   278,    83,   209,  1888,     0,     0,  1244,  1854,
      12,   279,  1880,    35,   277,  1854,   278,   271,   279,    41,
      42,  1866,   274,   283,   740,  1137,   886,  1866,  1123,   656,
    1134,   338,  1544,  1901,   602,   331,  1904,  1056,   671,   783,
    1885,  1880,   322,  1882,  1124,  1607,  1885,    69,  1543,  1888,
      72,  1919,  1131,  1819,   819,   688,  1522,  1524,  1823,   692,
    1822,  1857,  1901,   368,  1903,  1904,   699,   809,   206,   350,
    1909,   704,   705,   350,   983,  1804,  1630,  1745,   100,   101,
    1919,   714,   715,  1732,   717,   718,   719,   720,   721,   722,
     112,  1039,  1931,  1932,   116,  1779,  1503,   235,   236,  1276,
     238,   343,   728,   583,   355,   336,   865,   245,   246,   247,
     248,   249,  1333,   251,   857,    13,   254,   255,   256,   257,
     258,   641,   308,   772,   262,   263,   759,   785,   517,   513,
     268,   145,  1014,  1757,   759,  1652,  1885,     0,  1854,     0,
     278,     0,   748,   525,  1006,  1175,  1219,   780,  1288,   171,
    1113,   173,   785,  1664,   787,   581,    87,   179,    -1,  1440,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,    18,    -1,    -1,
     272,   273,    -1,    -1,    -1,    26,   278,    33,   280,    -1,
      31,    -1,   284,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   911,   912,
     913,    -1,    -1,   916,   917,   918,    -1,   920,   921,   922,
     923,   924,   925,   926,   927,    -1,   929,    -1,   931,    70,
      -1,   934,   935,   936,   937,   938,   939,    18,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    87,    -1,   951,    -1,
      31,   954,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,    70,
    1372,  1373,    -1,    -1,    -1,    -1,  1378,  1379,    79,  1381,
    1382,    -1,  1005,    -1,    -1,    -1,    87,  1389,  1390,  1391,
    1392,    -1,  1394,  1395,  1396,  1397,  1398,  1399,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,
       6,    -1,    -1,     9,   175,    -1,    -1,    13,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
     206,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    69,   175,    -1,    72,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,   100,   101,    -1,    -1,   269,   270,
     271,   272,   273,   274,   275,    -1,   112,    -1,   279,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
    1173,  1174,    -1,  1176,  1177,    -1,    -1,    -1,    -1,    -1,
      -1,  1184,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,
     271,   272,   273,   274,   275,   171,    -1,   173,  1201,    -1,
      -1,    -1,    -1,   179,    -1,    -1,  1209,  1210,  1211,    -1,
      -1,    -1,    -1,   189,  1217,    -1,  1219,  1220,  1221,  1222,
    1223,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,  1251,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,  1267,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    -1,   272,   273,  1301,    -1,
      -1,    -1,   278,     6,   280,    -1,     9,    -1,   284,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      -1,   206,    35,    62,    63,    -1,    -1,    -1,    41,    42,
      -1,    -1,    71,    -1,    -1,    -1,    -1,  1350,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,  1371,    72,
     245,   246,   247,   248,   249,    -1,   251,  1380,    -1,   254,
     255,   256,   257,   258,  1387,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,    -1,   100,   101,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,  1417,  1418,    -1,    19,    -1,   148,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,  1437,    -1,   165,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,   214,   189,    -1,    -1,    91,
      92,    93,    -1,    95,    96,    97,  1499,    99,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,   217,    -1,    -1,   119,    -1,   121,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     152,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,   169,    -1,   272,
     273,    -1,    -1,    -1,    -1,   278,    -1,   280,  1581,  1582,
      -1,   284,    -1,    -1,    -1,     6,    -1,    -1,     9,   191,
      -1,    -1,    13,    -1,    -1,  1598,    -1,    -1,    -1,    -1,
    1603,    22,    -1,    -1,   206,    -1,    -1,    -1,  1611,    -1,
      -1,    -1,  1615,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,   243,    -1,   245,   246,   247,   248,   249,    69,   251,
      -1,    72,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,     6,    -1,   206,     9,    -1,    -1,    -1,
      13,   112,    -1,    -1,  1697,   116,    -1,    -1,    -1,    22,
      -1,    -1,  1705,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,   235,   236,    -1,   238,    41,    42,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,  1732,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    69,   268,    -1,    72,
     171,    -1,   173,    -1,  1757,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,  1805,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   171,    -1,
     173,   272,   273,    -1,    -1,    -1,   179,   278,    -1,   280,
      -1,    -1,    -1,   284,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,     6,   268,    -1,     9,    -1,   272,
     273,    13,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,
      22,   284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,     7,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    69,    -1,    -1,
      72,    43,    -1,    45,    -1,    47,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     206,    73,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,
     112,    -1,    84,    -1,   116,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    69,    -1,    -1,    72,   171,
      -1,   173,   278,    -1,    -1,    -1,    -1,   179,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   189,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,   217,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      74,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,   237,   268,   171,    -1,   173,
     272,   273,    -1,    -1,    -1,   179,   278,    -1,   280,    -1,
     252,   253,   284,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    69,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,     6,   268,   189,     9,    -1,   272,   273,
      13,    -1,    -1,    -1,   278,    -1,   280,    98,    -1,    22,
     284,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    69,   251,    -1,    72,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,   189,   112,
      -1,    -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,    -1,   207,   208,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    60,    -1,    -1,    -1,    -1,    -1,
     241,   242,    -1,    69,    -1,    -1,    72,    -1,   171,   250,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,   273,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   163,    -1,   262,
     263,   167,    -1,    89,    -1,   268,    -1,   173,    -1,   272,
     273,    -1,    -1,   179,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    -1,   189,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   172,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,    13,
      -1,    -1,   268,    -1,    -1,    -1,   272,   273,    22,    -1,
      -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,
     206,    35,    -1,    -1,    38,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
     180,    -1,   268,    -1,    -1,    13,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,   206,    35,    -1,    -1,
     124,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    69,    -1,    -1,    72,   245,   246,   247,   248,   249,
      -1,   251,    -1,   167,   254,   255,   256,   257,   258,   173,
      -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,
      -1,    -1,   100,   101,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   203,
     204,   205,   206,   207,   208,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   167,
     254,   255,   256,   257,   258,   173,    -1,    -1,   262,   263,
      -1,   179,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,
      -1,   189,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    13,    -1,   262,   263,   189,    -1,    -1,    -1,
     268,    22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,
     278,    -1,   280,   206,    35,    -1,   284,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,   235,   236,    -1,   238,    -1,    -1,    69,    -1,
      -1,    72,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    13,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   206,
      35,    -1,    -1,   124,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,   246,
     247,   248,   249,    -1,   251,    -1,   167,   254,   255,   256,
     257,   258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,
      -1,   268,    -1,    -1,    -1,   100,   101,    -1,   189,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,   203,   204,   205,   206,   207,   208,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   167,   254,   255,   256,   257,   258,   173,    -1,
      -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,
      -1,   272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    -1,    74,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    13,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,   148,
      -1,    -1,    -1,   278,    -1,   280,    -1,    35,    -1,   284,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
     189,    69,    -1,    -1,    72,    -1,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,   112,    -1,   235,   236,   116,   238,
      -1,    22,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    35,   254,   255,   256,   257,   258,
      41,    42,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,   163,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    -1,   173,    -1,   272,   273,   274,    -1,   179,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    13,
     206,   262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    35,    -1,   284,    -1,    -1,    -1,    41,    42,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    69,   262,   263,    72,    -1,
      -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   280,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    13,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,   235,   236,    35,   238,    -1,
      -1,    -1,    -1,    41,    42,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,   278,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,   173,    -1,    -1,   262,   263,
      -1,   179,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,
      -1,   189,    -1,    -1,   278,    -1,   280,    -1,    -1,   197,
     284,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    13,   206,   262,   263,    -1,    -1,    -1,    -1,
     268,    22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,
     278,    -1,   280,    -1,    35,    -1,   284,    -1,    -1,    -1,
      41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    69,   262,
     263,    72,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,    -1,    -1,   235,   236,
      35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,    -1,    69,    -1,    -1,    72,    -1,   276,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,   173,    -1,
      -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,
      -1,   272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    13,   206,   262,   263,    -1,
      -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    35,    -1,   284,
      -1,    -1,    -1,    41,    42,   235,   236,    -1,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    69,   262,   263,    72,    -1,    -1,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,
     204,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,
      42,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,   273,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,
     268,    -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,
     278,   279,   280,    -1,    -1,    -1,   284,    -1,   200,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    13,   206,
     262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
      35,    -1,   284,    -1,    -1,    -1,    41,    42,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    69,   262,   263,    72,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,   235,   236,    35,   238,    -1,    -1,
      -1,    -1,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
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
      -1,   200,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    13,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     272,   273,    -1,   189,   276,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    17,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    47,    -1,   284,    -1,
      -1,    17,    -1,    54,    -1,    -1,    -1,    58,    59,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    47,    -1,    84,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    58,    59,    -1,    61,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,    61,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,   163,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
     206,    -1,    -1,   163,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,   178,    -1,
      -1,   262,   263,   264,   265,   266,    -1,   268,   188,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,   206,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    23,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      41,   251,    -1,    44,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   264,   265,   266,    -1,   268,    -1,
      -1,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,    85,    -1,    44,    -1,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    19,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    41,    -1,    -1,    44,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    85,    -1,    -1,   169,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,   119,
      -1,   121,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,   152,   191,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,   243,    -1,   245,   246,   247,   248,   249,   169,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   226,   227,
      -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
      -1,    -1,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,    -1,   245,   246,   247,   248,   249,   250,
     251,   252,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,   264,   265,   266,   267,   268
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
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
    break;
case 89:
{ yyval = yyvsp[-1]; }
    break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
    break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
    break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
    break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
    break;
case 94:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 122:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 123:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 124:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 125:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 126:
{ yyval = (dsql_nod*) 0;}
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
			  yyval = (dsql_nod*) yyvsp[0]; }
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
						(dsql_nod*) lex.g_file);}
    break;
case 179:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 181:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 182:
{ 
			 yyval = (dsql_nod*) make_node (nod_log_file_desc, (int) 1,
												(dsql_nod*) lex.g_file); }
    break;
case 183:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (dsql_str*) yyvsp[0]; }
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
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 220:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 221:
{ yyval = yyvsp[-1]; }
    break;
case 222:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
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
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 239:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 240:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 250:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
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
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 255:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 256:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 257:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
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
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 264:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 265:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 266:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 267:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 268:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 269:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 270:
{ yyval = NULL; }
    break;
case 271:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 272:
{ yyval = NULL; }
    break;
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 275:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 277:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 279:
{ yyval = yyvsp[0]; }
    break;
case 280:
{ yyval = yyvsp[0]; }
    break;
case 281:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 282:
{ yyval = make_list (yyvsp[0]); }
    break;
case 283:
{ yyval = NULL; }
    break;
case 285:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 286:
{ yyval = yyvsp[-1]; }
    break;
case 289:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 290:
{ yyval = NULL; }
    break;
case 291:
{ yyval = NULL; }
    break;
case 292:
{ yyval = yyvsp[0]; }
    break;
case 293:
{ yyval = yyvsp[0]; }
    break;
case 294:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 297:
{ yyval = yyvsp[-1]; }
    break;
case 298:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 299:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 300:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 302:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 318:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 319:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 322:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 323:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 324:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 325:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 326:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 330:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 331:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 332:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 333:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 334:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 335:
{ yyval = NULL; }
    break;
case 336:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 337:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 338:
{ yyval = make_list (yyvsp[0]); }
    break;
case 339:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 340:
{ yyval = NULL; }
    break;
case 341:
{ yyval = make_list (yyvsp[0]); }
    break;
case 342:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 343:
{ yyval = NULL; }
    break;
case 346:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 348:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 349:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 350:
{ yyval = NULL; }
    break;
case 351:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 352:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 353:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 354:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 355:
{ yyval = NULL; }
    break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 358:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 361:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 362:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 363:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 364:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 368:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 369:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 370:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 371:
{ yyval = NULL; }
    break;
case 372:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 373:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 374:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 375:
{ yyval = NULL; }
    break;
case 377:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 378:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 379:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 380:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 381:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 382:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 383:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 384:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 385:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 386:
{ yyval = make_list (yyvsp[0]); }
    break;
case 388:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 391:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 392:
{ yyval = yyvsp[-1]; }
    break;
case 393:
{ lex.beginning = lex_position(); }
    break;
case 394:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 395:
{ lex.beginning = lex.last_token; }
    break;
case 396:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 397:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 398:
{ yyval = 0; }
    break;
case 399:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 400:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 403:
{ yyval = NULL; }
    break;
case 404:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 405:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 406:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 407:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 408:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 409:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 410:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 411:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 412:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 413:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 414:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 415:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 416:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 417:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 418:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 419:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 420:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 421:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 422:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 423:
{ yyval = NULL; }
    break;
case 424:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 425:
{ yyval = yyvsp[0]; }
    break;
case 426:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 427:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 428:
{ yyval = yyvsp[0]; }
    break;
case 429:
{ yyval = yyvsp[0]; }
    break;
case 430:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 431:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 432:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 433:
{ yyval = yyvsp[0]; }
    break;
case 434:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 436:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 437:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 438:
{ yyval = yyvsp[0]; }
    break;
case 439:
{ yyval = yyvsp[0]; }
    break;
case 440:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 441:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 442:
{ yyval = yyvsp[0]; }
    break;
case 443:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 445:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 446:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 447:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 448:
{ yyval = yyvsp[0]; }
    break;
case 449:
{ yyval = yyvsp[0]; }
    break;
case 450:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 451:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 452:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 453:
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
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;
case 493:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 494:
{ yyval = yyvsp[0]; }
    break;
case 495:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 496:
{ yyval = yyvsp[0]; }
    break;
case 497:
{ yyval = yyvsp[0]; }
    break;
case 498:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 500:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 501:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 503:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 506:
{ yyval = NULL; }
    break;
case 508:
{ yyval = NULL; }
    break;
case 509:
{ yyval = yyvsp[0]; }
    break;
case 510:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 511:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 512:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 513:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 514:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 515:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 516:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 517:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 518:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 519:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 520:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 521:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 526:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 527:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 529:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 530:
{ if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 531:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 537:
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
case 538:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 540:
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
case 541:
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
case 542:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 549:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
    break;
case 550:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 551:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
    break;
case 552:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 553:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 554:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 557:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 570:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 571:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 572:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 573:
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
case 574:
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
case 577:
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
case 578:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 579:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 580:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 581:
{ yyval = yyvsp[-1]; }
    break;
case 582:
{ yyval = 0; }
    break;
case 586:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 587:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 588:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 592:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 593:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 594:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 595:
{ yyval = 0; }
    break;
case 596:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 599:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 600:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 603:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 604:
{ yyval = NULL; }
    break;
case 606:
{ yyval = NULL; }
    break;
case 607:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 609:
{ yyval = NULL; }
    break;
case 611:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 616:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 617:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 618:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 619:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 620:
{ yyval = yyvsp[0];}
    break;
case 622:
{ yyval = yyvsp[0];}
    break;
case 623:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 624:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 625:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 626:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 627:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 628:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 629:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 630:
{ yyval = 0; }
    break;
case 631:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 632:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 633:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 634:
{ yyval = (dsql_nod*) 0; }
    break;
case 635:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 636:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 638:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 639:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 640:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 641:
{ yyval = 0; }
    break;
case 643:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 644:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 645:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 646:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 647:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 648:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 649:
{ yyval = make_list (yyvsp[0]); }
    break;
case 650:
{ yyval = 0; }
    break;
case 652:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 653:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 654:
{ yyval = 0; }
    break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 656:
{ yyval = 0; }
    break;
case 657:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 658:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 659:
{ yyval = yyvsp[-1]; }
    break;
case 660:
{ yyval = 0; }
    break;
case 661:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 662:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 663:
{ yyval = NULL; }
    break;
case 664:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 665:
{ yyval = 0; }
    break;
case 666:
{ yyval = yyvsp[0]; }
    break;
case 667:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 668:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 669:
{ yyval = 0; }
    break;
case 670:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 671:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 672:
{ lex.limit_clause = true; }
    break;
case 673:
{ lex.limit_clause = false; }
    break;
case 674:
{ lex.first_detection = true; }
    break;
case 675:
{ lex.first_detection = false; }
    break;
case 676:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 677:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 678:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 679:
{ yyval = 0; }
    break;
case 680:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 681:
{ yyval = yyvsp[-2]; }
    break;
case 682:
{ yyval = yyvsp[-1]; }
    break;
case 683:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 684:
{ yyval = yyvsp[-1]; }
    break;
case 685:
{ yyval = yyvsp[0]; }
    break;
case 686:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 687:
{ yyval = 0; }
    break;
case 688:
{ yyval = make_list (yyvsp[0]); }
    break;
case 689:
{ yyval = 0; }
    break;
case 691:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 693:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 696:
{ yyval = make_list (yyvsp[0]); }
    break;
case 698:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 702:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 704:
{ yyval = NULL; }
    break;
case 705:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 706:
{ yyval = NULL; }
    break;
case 708:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 709:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 710:
{ yyval = yyvsp[-1]; }
    break;
case 711:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 712:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 713:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 714:
{ yyval = NULL; }
    break;
case 716:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 717:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 718:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 719:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 720:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 721:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 722:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 723:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 724:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 725:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 726:
{ yyval = make_list (yyvsp[0]); }
    break;
case 727:
{ yyval = 0; }
    break;
case 729:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 731:
{ yyval = yyvsp[0]; }
    break;
case 732:
{ yyval = 0; }
    break;
case 733:
{ yyval = yyvsp[0]; }
    break;
case 734:
{ yyval = 0; }
    break;
case 735:
{ yyval = yyvsp[0]; }
    break;
case 736:
{ yyval = 0; }
    break;
case 737:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 738:
{ yyval = 0; }
    break;
case 739:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 740:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 741:
{ yyval = 0; }
    break;
case 742:
{ yyval = 0; }
    break;
case 744:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 745:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 748:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 749:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 750:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 751:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 753:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 754:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 755:
{ yyval = 0; }
    break;
case 756:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 757:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 761:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 765:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 766:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 773:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 774:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 775:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 781:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 782:
{ yyval = yyvsp[0]; }
    break;
case 784:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 787:
{ yyval = NULL; }
    break;
case 788:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 790:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = NULL; }
    break;
case 793:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 795:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 798:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 799:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 811:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = yyvsp[-1]; }
    break;
case 815:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 827:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 833:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 834:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 837:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 840:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 841:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 842:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 843:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 844:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 845:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 846:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 847:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 848:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 849:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 852:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 854:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 856:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 858:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 859:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 860:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 861:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 862:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 863:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 864:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 865:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 866:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 867:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 868:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 869:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 873:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 874:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 875:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 877:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 878:
{ yyval = yyvsp[-1]; }
    break;
case 879:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 880:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 890:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 891:
{ yyval = yyvsp[0]; }
    break;
case 892:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 893:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 894:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 895:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 896:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 897:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 898:
{ yyval = yyvsp[-1]; }
    break;
case 899:
{ yyval = yyvsp[-1]; }
    break;
case 903:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 904:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 905:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 906:
{ yyval = yyvsp[0]; }
    break;
case 907:
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
case 908:
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
case 909:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 910:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 912:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 914:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 917:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 918:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 919:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 921:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 922:
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
case 923:
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
case 924:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 925:
{ yyval = make_parameter (); }
    break;
case 926:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 927:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 928:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 929:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 930:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 931:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 932:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 933:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 934:
{ yyval = yyvsp[0]; }
    break;
case 935:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 937:
{ yyval = (dsql_nod*) - (SLONG) yyvsp[0]; }
    break;
case 938:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 939:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 940:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 941:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 943:
{ yyval = (dsql_nod*) - (SLONG) yyvsp[0]; }
    break;
case 944:
{ yyval = yyvsp[0];}
    break;
case 949:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 950:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 951:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 952:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 953:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 954:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 955:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 956:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 957:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 958:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 959:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 960:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 962:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 964:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 965:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 966:
{ yyval = yyvsp[0]; }
    break;
case 967:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 968:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 969:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 970:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 973:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 974:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 977:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 978:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 979:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 980:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 981:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 982:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 983:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 984:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 989:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 990:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 991:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 992:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 993:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 994:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 995:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 998:
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
	node->nod_arg[0] = (dsql_nod*)(ULONG) lex.param_number++;

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
					yylval = (dsql_nod*) (ULONG) number;
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

