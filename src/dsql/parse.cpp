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
# define	KW_PAGE_SIZE	393
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

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		1938
#define	YYFLAG		-32768
#define	YYNTBASE	285

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 522 ? yytranslate[x] : 722)

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
     994,   996,   999,  1003,  1008,  1011,  1018,  1023,  1032,  1042,
    1049,  1058,  1065,  1070,  1071,  1076,  1079,  1081,  1085,  1086,
    1089,  1094,  1095,  1097,  1099,  1103,  1107,  1115,  1118,  1119,
    1122,  1125,  1129,  1133,  1134,  1136,  1139,  1144,  1146,  1150,
    1153,  1156,  1159,  1161,  1163,  1165,  1167,  1170,  1173,  1179,
    1180,  1185,  1193,  1197,  1198,  1200,  1204,  1208,  1216,  1224,
    1226,  1228,  1232,  1237,  1245,  1248,  1250,  1254,  1256,  1258,
    1265,  1269,  1270,  1271,  1272,  1273,  1277,  1278,  1288,  1298,
    1300,  1302,  1303,  1306,  1308,  1310,  1312,  1314,  1316,  1320,
    1324,  1328,  1332,  1336,  1340,  1346,  1352,  1358,  1364,  1370,
    1376,  1379,  1380,  1385,  1388,  1392,  1396,  1399,  1402,  1406,
    1410,  1413,  1417,  1424,  1426,  1429,  1434,  1438,  1441,  1444,
    1447,  1450,  1454,  1456,  1460,  1464,  1468,  1471,  1474,  1479,
    1484,  1490,  1492,  1494,  1496,  1498,  1500,  1502,  1504,  1506,
    1508,  1510,  1512,  1514,  1516,  1518,  1520,  1522,  1524,  1526,
    1528,  1530,  1532,  1534,  1536,  1538,  1540,  1542,  1544,  1546,
    1548,  1551,  1554,  1557,  1559,  1561,  1563,  1564,  1567,  1570,
    1571,  1573,  1576,  1579,  1582,  1585,  1588,  1593,  1597,  1600,
    1603,  1605,  1609,  1617,  1619,  1620,  1622,  1623,  1626,  1629,
    1632,  1635,  1638,  1641,  1644,  1647,  1650,  1654,  1657,  1660,
    1663,  1665,  1667,  1669,  1671,  1676,  1682,  1684,  1688,  1690,
    1694,  1696,  1699,  1701,  1703,  1705,  1707,  1709,  1711,  1713,
    1715,  1717,  1719,  1721,  1726,  1731,  1738,  1744,  1748,  1749,
    1752,  1755,  1756,  1760,  1761,  1766,  1768,  1774,  1779,  1781,
    1786,  1788,  1791,  1794,  1796,  1798,  1800,  1803,  1806,  1809,
    1812,  1813,  1817,  1823,  1825,  1827,  1830,  1834,  1836,  1839,
    1843,  1844,  1846,  1848,  1850,  1856,  1862,  1869,  1871,  1873,
    1875,  1878,  1883,  1885,  1886,  1892,  1894,  1895,  1899,  1902,
    1904,  1905,  1908,  1909,  1911,  1912,  1916,  1918,  1919,  1921,
    1924,  1926,  1928,  1930,  1932,  1935,  1938,  1940,  1943,  1947,
    1949,  1951,  1955,  1959,  1961,  1964,  1968,  1970,  1973,  1974,
    1977,  1979,  1981,  1982,  1984,  1986,  1988,  1992,  1995,  1999,
    2000,  2002,  2006,  2011,  2017,  2019,  2023,  2028,  2032,  2033,
    2035,  2039,  2043,  2045,  2047,  2048,  2050,  2052,  2057,  2058,
    2061,  2066,  2067,  2071,  2072,  2075,  2076,  2079,  2080,  2090,
    2102,  2103,  2104,  2105,  2106,  2110,  2113,  2115,  2116,  2120,
    2126,  2130,  2133,  2139,  2142,  2144,  2146,  2148,  2150,  2152,
    2156,  2158,  2162,  2164,  2165,  2168,  2170,  2174,  2176,  2178,
    2180,  2187,  2189,  2190,  2194,  2195,  2197,  2201,  2208,  2212,
    2217,  2220,  2224,  2225,  2227,  2230,  2232,  2234,  2236,  2239,
    2241,  2244,  2246,  2249,  2250,  2254,  2255,  2257,  2261,  2263,
    2266,  2267,  2270,  2271,  2274,  2275,  2280,  2282,  2285,  2287,
    2289,  2290,  2292,  2296,  2299,  2301,  2303,  2306,  2308,  2313,
    2317,  2319,  2323,  2328,  2329,  2338,  2344,  2346,  2348,  2353,
    2358,  2363,  2365,  2367,  2373,  2379,  2381,  2385,  2389,  2391,
    2393,  2395,  2399,  2407,  2415,  2421,  2425,  2426,  2428,  2430,
    2432,  2435,  2436,  2438,  2440,  2442,  2443,  2447,  2449,  2453,
    2455,  2456,  2460,  2462,  2466,  2468,  2472,  2476,  2478,  2480,
    2484,  2486,  2489,  2491,  2495,  2499,  2501,  2504,  2508,  2512,
    2516,  2520,  2522,  2526,  2529,  2531,  2533,  2535,  2537,  2539,
    2541,  2543,  2545,  2547,  2549,  2553,  2557,  2561,  2565,  2569,
    2573,  2577,  2581,  2588,  2595,  2602,  2609,  2616,  2623,  2630,
    2637,  2644,  2651,  2658,  2665,  2672,  2679,  2686,  2693,  2695,
    2697,  2703,  2710,  2714,  2719,  2725,  2732,  2736,  2741,  2745,
    2750,  2754,  2759,  2764,  2770,  2775,  2780,  2784,  2789,  2791,
    2793,  2795,  2797,  2799,  2801,  2803,  2807,  2811,  2823,  2835,
    2837,  2839,  2841,  2843,  2845,  2847,  2849,  2851,  2853,  2856,
    2859,  2863,  2867,  2871,  2875,  2879,  2883,  2887,  2891,  2893,
    2895,  2897,  2899,  2903,  2905,  2907,  2909,  2911,  2913,  2918,
    2920,  2924,  2926,  2929,  2931,  2933,  2935,  2937,  2939,  2941,
    2943,  2946,  2949,  2952,  2954,  2956,  2958,  2960,  2962,  2964,
    2966,  2968,  2970,  2972,  2975,  2977,  2980,  2982,  2984,  2986,
    2988,  2990,  2993,  2995,  2997,  2999,  3001,  3003,  3008,  3014,
    3020,  3026,  3032,  3038,  3044,  3050,  3056,  3062,  3068,  3075,
    3077,  3084,  3086,  3091,  3099,  3102,  3103,  3108,  3112,  3119,
    3121,  3123,  3130,  3137,  3139,  3141,  3146,  3153,  3158,  3164,
    3168,  3174,  3179,  3185,  3187,  3189,  3191,  3193,  3195,  3197,
    3199,  3201,  3203,  3205,  3207,  3209,  3210,  3212,  3214,  3216,
    3218,  3220,  3222,  3224,  3226,  3228,  3230,  3232,  3234,  3236,
    3238,  3240,  3242,  3244,  3246,  3248,  3250,  3252,  3254,  3256,
    3258,  3260,  3262,  3264,  3266,  3268,  3270,  3272,  3274,  3276,
    3278,  3280,  3282,  3284,  3286,  3288,  3290,  3292,  3294,  3296
};
static const short yyrhs[] =
{
     286,     0,   286,   276,     0,   477,     0,   619,     0,   531,
       0,   316,     0,   306,     0,   608,     0,   499,     0,   287,
       0,   607,     0,   450,     0,   451,     0,   318,     0,   320,
       0,   296,     0,   532,     0,   525,     0,   554,     0,   523,
       0,   612,     0,    43,   666,     0,    73,   289,   129,   288,
     182,   300,   293,     0,    73,   291,   129,   148,   295,   182,
     300,   293,     0,    73,   289,   129,   288,   182,   298,     0,
      73,   291,   129,   148,   295,   182,   298,     0,    73,   302,
     182,   304,   294,     0,   591,     0,   180,   591,     0,     6,
       0,     6,   147,     0,   290,     0,   292,     0,   290,   277,
     292,     0,    59,     0,   163,     0,    84,     0,    47,     0,
     188,   624,     0,   153,   624,     0,   200,    73,   131,     0,
       0,   200,   210,   131,     0,     0,   711,     0,   158,   297,
     289,   129,   288,    66,   300,     0,   158,   297,   291,   129,
     148,   295,    66,   300,     0,   158,   289,   129,   288,    66,
     300,     0,   158,   291,   129,   148,   295,    66,   300,     0,
     158,   289,   129,   288,    66,   298,     0,   158,   291,   129,
     148,   295,    66,   298,     0,   158,   302,    66,   304,     0,
      73,   131,    64,     0,   299,     0,   298,   277,   299,     0,
     298,   277,   301,     0,   300,   277,   299,     0,   148,   711,
       0,   184,   715,     0,   195,   718,     0,   214,   712,     0,
     301,     0,   300,   277,   301,     0,   716,     0,   189,   716,
       0,    74,   716,     0,   303,     0,   302,   277,   303,     0,
     712,     0,   305,     0,   304,   277,   305,     0,   716,     0,
     189,   716,     0,    45,   307,     0,    63,   315,     0,    62,
      68,   308,     0,   696,   310,   157,   313,    55,   665,   117,
     665,     0,   506,     0,    19,     0,    37,   278,   669,   279,
     512,     0,     0,   311,     0,   278,   311,   279,     0,   312,
       0,   311,   277,   312,     0,   379,   309,     0,   379,   309,
      20,   238,     0,   314,     0,   278,   314,   279,     0,   379,
     309,     0,   379,   309,   212,     0,   379,   309,    20,   101,
       0,   379,   309,    20,   238,     0,   140,   669,     0,   705,
      82,   511,   135,   511,    55,   665,   117,   665,     0,    36,
     317,     0,    58,   704,   665,     0,   322,   559,    83,   708,
     129,   591,   323,     0,   148,   398,     0,   180,   366,     0,
     184,   469,     0,   195,   455,     0,    71,   339,     0,    40,
     341,     0,    51,   330,     0,   165,   324,     0,   214,   340,
       0,   237,   319,     0,   148,   399,     0,   180,   367,     0,
     195,   456,     0,    36,   132,     7,   321,     0,   148,   400,
       0,   184,   470,     0,   187,     0,     0,   626,     0,   625,
       0,   373,   278,   466,   654,   467,   279,     0,   669,   325,
     326,   665,   327,   328,     0,   111,     0,    14,     0,     0,
       0,    32,     0,     0,   102,   342,   672,   365,     0,     0,
     329,     0,   354,     0,   329,   354,     0,   376,   331,   501,
     466,   332,   467,   333,   375,     0,    10,     0,     0,    46,
     466,   381,     0,     0,     0,   334,     0,   335,     0,   334,
     335,     0,   336,     0,   337,     0,   338,     0,   124,    98,
       0,   466,    24,   278,   633,   279,   467,     0,   707,     0,
     712,     0,   343,   344,   347,     0,     0,   269,     0,   665,
       0,     0,   345,     0,   346,     0,   345,   346,     0,   139,
     342,   669,     0,   102,   342,   672,   365,     0,   189,   665,
       0,   141,   665,     0,   164,   118,   665,     0,     0,   348,
       0,   349,     0,   348,   349,     0,   354,     0,   351,     0,
     350,     0,    46,    23,   164,   698,     0,   263,    94,   665,
       0,    75,   342,   672,     0,    25,   342,   672,     0,   127,
     342,   669,     0,   109,   342,   670,     0,   353,     0,   352,
       0,   103,   278,   355,   279,   136,   356,     0,   103,    15,
     356,     0,   103,     0,   360,   665,   361,     0,   356,     0,
     355,   277,   356,     0,   357,   358,     0,   665,     0,     0,
     358,   359,     0,   168,   342,   672,     0,    94,     0,     0,
     362,     0,   363,     0,   362,   363,     0,   175,   364,   672,
       0,   102,   342,   672,   365,     0,     0,    12,     0,    12,
     137,     0,     0,   137,     0,   138,     0,   591,   368,   278,
     369,   279,     0,   591,   368,   278,   369,   279,     0,    62,
      94,   665,     0,    62,   665,     0,     0,   370,     0,   369,
     277,   370,     0,   371,     0,   386,     0,   376,   374,   380,
     467,   382,   375,     0,   376,   502,   372,     0,   376,   372,
       0,   373,   278,   466,   654,   467,   279,     0,    30,    20,
       0,    30,     0,   501,   466,     0,   631,   464,     0,    26,
     699,     0,     0,   631,     0,   631,     0,   379,   501,     0,
       0,    46,   381,     0,     0,   658,     0,   662,     0,   663,
       0,   664,     0,   695,     0,   655,     0,     0,   383,     0,
     384,     0,   383,   384,     0,   387,   385,     0,   124,    98,
       0,   153,   591,   624,   394,   392,     0,   393,     0,   187,
     392,     0,   146,    90,   392,     0,   387,   388,     0,    33,
     701,     0,     0,   389,     0,   390,     0,   391,     0,   393,
       0,   187,   625,   392,     0,   146,    90,   625,   392,     0,
      65,    90,   625,   153,   591,   624,   394,   392,     0,   247,
     559,    83,   708,     0,     0,   466,    24,   278,   633,   279,
     467,     0,   395,     0,   396,     0,   396,   395,     0,   395,
     396,     0,     0,   129,   188,   397,     0,   129,    47,   397,
       0,   211,     0,   164,    46,     0,   164,    98,     0,   123,
     209,     0,   711,   402,   403,    10,   464,   409,   418,   467,
       0,   711,   402,   403,    10,   464,   409,   418,   467,     0,
     711,   402,   403,    10,   464,   409,   418,   467,     0,   711,
     402,   403,    10,   464,   409,   418,   467,     0,   278,   404,
     279,     0,     0,   157,   278,   406,   279,     0,     0,   405,
       0,   404,   277,   405,     0,   377,   502,   408,   467,     0,
     407,     0,   406,   277,   407,     0,   377,   502,     0,    46,
     464,   381,     0,   269,   464,   381,     0,   464,     0,   410,
       0,     0,   411,     0,   410,   411,     0,    45,   414,   412,
     276,     0,   413,     0,   416,     0,   376,   502,   415,     0,
     192,     0,     0,   269,   381,     0,   380,     0,   702,    39,
      64,   278,   567,   279,     0,   421,     0,   418,     0,    17,
     419,    54,     0,   420,     0,   420,   441,     0,     0,   417,
       0,   420,   417,     0,   616,   276,     0,   608,   276,     0,
     422,     0,   423,     0,   424,     0,   425,     0,   426,     0,
     429,     0,   607,   276,     0,   430,     0,   432,     0,   612,
     276,     0,   437,     0,   427,     0,   428,     0,   178,   276,
       0,    61,   276,     0,   439,     0,   445,   276,     0,    58,
     704,   276,     0,    58,   704,   654,   276,     0,    58,   276,
       0,    59,   148,   711,   434,   435,   276,     0,    59,   254,
     654,   276,     0,   438,    64,   554,    86,   436,   440,    50,
     417,     0,   438,    64,    59,   254,   654,    86,   436,    50,
     417,     0,    59,   254,   654,    86,   436,   276,     0,    78,
     278,   633,   279,   181,   417,    53,   417,     0,    78,   278,
     633,   279,   181,   417,     0,   144,   654,   431,   276,     0,
       0,   554,    86,   436,   276,     0,   280,   717,     0,   618,
       0,   278,   618,   279,     0,     0,   156,   436,     0,   156,
     278,   436,   279,     0,     0,   433,     0,   630,     0,   436,
     277,   630,     0,   436,   277,   433,     0,   438,   199,   278,
     633,   279,    50,   417,     0,   710,   280,     0,     0,   235,
     276,     0,   255,   276,     0,   255,   710,   276,     0,    10,
      39,   702,     0,     0,   442,     0,   441,   442,     0,   197,
     443,    50,   417,     0,   444,     0,   443,   277,   444,     0,
     173,   666,     0,    69,   706,     0,    58,   704,     0,     9,
       0,   446,     0,   448,     0,   447,     0,   264,   702,     0,
     265,   702,     0,   266,   449,   702,    86,   436,     0,     0,
      59,   148,   711,   434,     0,    59,   268,   452,   403,    10,
     409,   418,     0,   278,   453,   279,     0,     0,   454,     0,
     453,   277,   454,     0,   407,   269,   661,     0,   718,   624,
      10,   464,   457,   468,   465,     0,   718,   624,    10,   464,
     457,   468,   465,     0,   458,     0,   459,     0,   458,   186,
     459,     0,   458,   186,     6,   459,     0,   163,   575,   576,
     460,   597,   593,   596,     0,    66,   461,     0,   462,     0,
     461,   277,   462,     0,   463,     0,   590,     0,   462,   592,
      89,   462,   129,   633,     0,   278,   463,   279,     0,     0,
       0,     0,     0,   200,    24,   131,     0,     0,   715,    64,
     591,   471,   472,   475,   466,   476,   467,     0,   715,    64,
     591,   471,   472,   475,   466,   476,   467,     0,     3,     0,
      80,     0,     0,   473,   474,     0,    16,     0,     5,     0,
      84,     0,   188,     0,    47,     0,    84,   132,   188,     0,
      84,   132,    47,     0,   188,   132,    84,     0,   188,   132,
      47,     0,    47,   132,    84,     0,    47,   132,   188,     0,
      84,   132,   188,   132,    47,     0,    84,   132,    47,   132,
     188,     0,   188,   132,    84,   132,    47,     0,   188,   132,
      47,   132,    84,     0,    47,   132,    84,   132,   188,     0,
      47,   132,   188,   132,    84,     0,   143,   667,     0,     0,
      10,   466,   409,   418,     0,     7,   478,     0,    58,   704,
     665,     0,   180,   591,   483,     0,   184,   496,     0,   148,
     401,     0,    40,   492,   493,     0,    51,   485,   481,     0,
      83,   491,     0,    46,   466,   381,     0,    24,   466,   278,
     633,   279,   467,     0,   482,     0,   481,   482,     0,   164,
     466,   479,   467,     0,     4,    33,   480,     0,     4,   338,
       0,    52,    46,     0,    52,    33,     0,   182,   631,     0,
     216,   379,   502,     0,   484,     0,   483,   277,   484,     0,
      52,   631,   490,     0,    52,    33,   701,     0,     4,   371,
       0,     4,   386,     0,   487,   631,   143,   669,     0,   487,
     485,   182,   631,     0,   487,   489,   216,   488,   467,     0,
     486,     0,   720,     0,   215,     0,   216,     0,   217,     0,
     218,     0,   219,     0,   220,     0,   221,     0,   222,     0,
     223,     0,   224,     0,   225,     0,   226,     0,   227,     0,
     228,     0,   229,     0,   230,     0,   233,     0,   234,     0,
     241,     0,   242,     0,   250,     0,   252,     0,   264,     0,
     265,     0,   266,     0,   267,     0,     7,     0,     7,   215,
       0,   502,   466,     0,   631,   464,     0,   631,     0,   213,
       0,   211,     0,     0,   708,     3,     0,   708,    80,     0,
       0,   494,     0,   493,   494,     0,     4,   329,     0,    52,
     103,     0,   164,   495,     0,     4,   351,     0,     4,   263,
      94,   665,     0,    52,   263,    94,     0,    17,   262,     0,
      54,   262,     0,   350,     0,   495,   277,   350,     0,   715,
     471,   497,   475,   466,   498,   467,     0,   472,     0,     0,
     476,     0,     0,    52,   500,     0,    58,   704,     0,    83,
     708,     0,   148,   711,     0,   180,   714,     0,   184,   715,
       0,   195,   718,     0,    63,   705,     0,    51,   703,     0,
      62,    68,   696,     0,   165,   669,     0,   214,   712,     0,
      71,   707,     0,   502,     0,   503,     0,   506,     0,   509,
       0,   507,   281,   504,   282,     0,   514,   281,   504,   282,
     512,     0,   505,     0,   504,   277,   505,     0,   671,     0,
     671,   280,   671,     0,   507,     0,   514,   512,     0,   513,
       0,   518,     0,   521,     0,   243,     0,   508,     0,   169,
       0,    41,     0,   226,     0,   227,     0,    85,     0,    96,
       0,    19,   511,   510,   512,     0,    19,   278,   670,   279,
       0,    19,   278,   670,   277,   666,   279,     0,    19,   278,
     277,   666,   279,     0,   162,   168,   670,     0,     0,   177,
     666,     0,   177,   697,     0,     0,    23,   164,   698,     0,
       0,   517,   278,   669,   279,     0,   517,     0,   517,   193,
     278,   669,   279,     0,   516,   278,   669,   279,     0,   516,
       0,   515,   278,   669,   279,     0,   191,     0,    23,   193,
       0,    91,   193,     0,    23,     0,    91,     0,   121,     0,
     119,    23,     0,   119,    91,     0,    99,   519,     0,   520,
     519,     0,     0,   278,   671,   279,     0,   278,   671,   277,
     671,   279,     0,    44,     0,    92,     0,    95,   522,     0,
      97,    95,   522,     0,   152,     0,    93,   145,     0,   278,
     667,   279,     0,     0,   536,     0,   524,     0,   553,     0,
     164,    71,   707,   182,   671,     0,   164,    71,   707,   182,
     231,     0,   164,    71,   707,   182,   273,   231,     0,   526,
       0,   527,     0,   529,     0,   252,   719,     0,   253,   252,
     719,   528,     0,   130,     0,     0,   161,   533,   182,   530,
     719,     0,   252,     0,     0,    28,   533,   534,     0,   161,
     533,     0,   201,     0,     0,   155,   535,     0,     0,   170,
       0,     0,   164,   183,   537,     0,   538,     0,     0,   539,
       0,   538,   539,     0,   540,     0,   541,     0,   542,     0,
     546,     0,   151,   130,     0,   151,   202,     0,   196,     0,
     123,   196,     0,    88,   107,   543,     0,   543,     0,   544,
       0,   151,   185,   545,     0,   151,    29,   545,     0,   170,
       0,   170,   180,     0,   170,   180,   174,     0,   194,     0,
     123,   194,     0,     0,   154,   549,     0,   166,     0,   149,
       0,     0,   151,     0,   202,     0,   550,     0,   549,   277,
     550,     0,   552,   551,     0,    64,   547,   548,     0,     0,
     591,     0,   552,   277,   591,     0,   164,   176,    83,   708,
       0,   555,   556,   562,   563,   565,     0,   566,     0,   555,
     186,   566,     0,   555,   186,     6,   566,     0,   133,    20,
     557,     0,     0,   558,     0,   557,   277,   558,     0,   654,
     559,   561,     0,    11,     0,    48,     0,     0,   239,     0,
     249,     0,   248,   570,   560,   571,     0,     0,   267,   654,
       0,   267,   654,   182,   654,     0,     0,    64,   188,   564,
       0,     0,   128,   626,     0,     0,   200,   251,     0,     0,
     163,   572,   575,   576,   580,   597,   593,   596,   598,     0,
     163,   572,   575,   576,   580,   597,   593,   596,   598,   556,
     562,     0,     0,     0,     0,     0,   573,   574,   569,     0,
     573,   569,     0,   574,     0,     0,   239,   672,   568,     0,
     239,   278,   654,   279,   568,     0,   239,   661,   568,     0,
     240,   672,     0,   240,   278,   569,   654,   279,     0,   240,
     661,     0,    49,     0,   694,     0,   577,     0,   274,     0,
     578,     0,   577,   277,   578,     0,   617,     0,   617,   579,
     709,     0,    10,     0,     0,    66,   581,     0,   582,     0,
     581,   277,   582,     0,   587,     0,   588,     0,   583,     0,
     278,   554,   279,   579,   584,   585,     0,   713,     0,     0,
     278,   586,   279,     0,     0,   709,     0,   586,   277,   709,
       0,   582,   592,    89,   582,   129,   633,     0,   278,   587,
     279,     0,   711,   589,   579,   713,     0,   711,   589,     0,
     278,   618,   279,     0,     0,   591,     0,   714,   713,     0,
     714,     0,    81,     0,   105,     0,   105,   134,     0,   159,
       0,   159,   134,     0,    67,     0,    67,   134,     0,     0,
      74,    20,   594,     0,     0,   595,     0,   594,   277,   595,
       0,   654,     0,    77,   633,     0,     0,   198,   633,     0,
       0,   142,   599,     0,     0,   600,   278,   601,   279,     0,
      89,     0,   172,   114,     0,   114,     0,   172,     0,     0,
     602,     0,   602,   277,   601,     0,   603,   604,     0,   599,
       0,   714,     0,   714,   603,     0,   120,     0,    83,   278,
     605,   279,     0,   133,   708,   606,     0,   708,     0,   708,
     277,   605,     0,    83,   278,   605,   279,     0,     0,    84,
      86,   591,   627,   190,   278,   618,   279,     0,    84,    86,
     591,   627,   567,     0,   609,     0,   610,     0,    47,    66,
     590,   597,     0,    47,    66,   590,   611,     0,   198,    38,
     128,   702,     0,   613,     0,   614,     0,   188,   590,   164,
     615,   597,     0,   188,   590,   164,   615,   611,     0,   616,
       0,   615,   277,   616,     0,   632,   269,   617,     0,   654,
       0,   695,     0,   617,     0,   618,   277,   617,     0,   151,
      19,   631,    66,   591,   620,   622,     0,    84,    19,   631,
      86,   591,   620,   622,     0,    63,    66,   621,   182,   621,
       0,    63,   182,   621,     0,     0,   511,     0,   661,     0,
     666,     0,   113,   623,     0,     0,   670,     0,   661,     0,
     625,     0,     0,   278,   626,   279,     0,   631,     0,   626,
     277,   631,     0,   628,     0,     0,   278,   629,   279,     0,
     632,     0,   629,   277,   632,     0,   631,     0,   713,   283,
     700,     0,   713,   283,   274,     0,   700,     0,   631,     0,
     713,   283,   700,     0,   648,     0,   124,   648,     0,   635,
       0,   633,   132,   633,     0,   633,     8,   633,     0,   635,
       0,   124,   648,     0,   634,   132,   633,     0,   634,     8,
     633,     0,   649,   132,   633,     0,   649,     8,   633,     0,
     636,     0,   278,   634,   279,     0,   124,   635,     0,   637,
       0,   640,     0,   641,     0,   642,     0,   647,     0,   638,
       0,   645,     0,   643,     0,   644,     0,   646,     0,   654,
     269,   654,     0,   654,   270,   654,     0,   654,   271,   654,
       0,   654,    70,   654,     0,   654,   106,   654,     0,   654,
     125,   654,     0,   654,   126,   654,     0,   654,   122,   654,
       0,   654,   269,     6,   278,   652,   279,     0,   654,   270,
       6,   278,   652,   279,     0,   654,   271,     6,   278,   652,
     279,     0,   654,    70,     6,   278,   652,   279,     0,   654,
     106,     6,   278,   652,   279,     0,   654,   125,     6,   278,
     652,   279,     0,   654,   126,     6,   278,   652,   279,     0,
     654,   122,     6,   278,   652,   279,     0,   654,   269,   639,
     278,   652,   279,     0,   654,   270,   639,   278,   652,   279,
       0,   654,   271,   639,   278,   652,   279,     0,   654,    70,
     639,   278,   652,   279,     0,   654,   106,   639,   278,   652,
     279,     0,   654,   125,   639,   278,   652,   279,     0,   654,
     126,   639,   278,   652,   279,     0,   654,   122,   639,   278,
     652,   279,     0,   171,     0,     9,     0,   654,    18,   654,
       8,   654,     0,   654,   124,    18,   654,     8,   654,     0,
     654,   108,   654,     0,   654,   124,   108,   654,     0,   654,
     108,   654,    57,   654,     0,   654,   124,   108,   654,    57,
     654,     0,   654,    79,   650,     0,   654,   124,    79,   650,
       0,   654,    34,   654,     0,   654,   124,    34,   654,     0,
     654,   175,   654,     0,   654,   124,   175,   654,     0,   654,
     175,   200,   654,     0,   654,   124,   175,   200,   654,     0,
      60,   278,   567,   279,     0,   167,   278,   567,   279,     0,
     654,    87,    98,     0,   654,    87,   124,    98,     0,   256,
       0,   257,     0,   258,     0,   259,     0,   260,     0,   261,
       0,   651,     0,   278,   657,   279,     0,   278,   652,   279,
       0,   163,   572,   575,   654,   580,   597,   593,   596,   598,
     556,   562,     0,   163,   572,   575,   654,   580,   597,   593,
     596,   598,   556,   562,     0,   630,     0,   656,     0,   673,
       0,   660,     0,   661,     0,   433,     0,   682,     0,   683,
       0,   681,     0,   273,   654,     0,   272,   654,     0,   654,
     272,   654,     0,   654,    31,   654,     0,   654,    26,   699,
       0,   654,   273,   654,     0,   654,   274,   654,     0,   654,
     275,   654,     0,   278,   654,   279,     0,   278,   653,   279,
       0,   662,     0,   663,     0,   664,     0,    42,     0,   713,
     283,    42,     0,   101,     0,   655,     0,   228,     0,   229,
       0,   230,     0,   630,   281,   657,   282,     0,   654,     0,
     657,   277,   654,     0,   660,     0,   273,   659,     0,   205,
       0,   204,     0,   203,     0,   231,     0,   232,     0,   659,
       0,   665,     0,    41,   207,     0,   226,   207,     0,   227,
     207,     0,   284,     0,   189,     0,   233,     0,   234,     0,
     241,     0,   242,     0,    69,     0,   173,     0,   250,     0,
     207,     0,   208,   207,     0,   667,     0,   273,   668,     0,
     204,     0,   204,     0,   667,     0,   204,     0,   672,     0,
     273,   672,     0,   204,     0,   674,     0,   675,     0,   676,
       0,   678,     0,    35,   278,   274,   279,     0,    35,   278,
     694,   654,   279,     0,    35,   278,    49,   654,   279,     0,
     179,   278,   694,   654,   279,     0,   179,   278,    49,   654,
     279,     0,    13,   278,   694,   654,   279,     0,    13,   278,
      49,   654,   279,     0,   116,   278,   694,   654,   279,     0,
     116,   278,    49,   654,   279,     0,   112,   278,   694,   654,
     279,     0,   112,   278,    49,   654,   279,     0,    72,   278,
     707,   277,   654,   279,     0,   677,     0,   217,   278,   693,
      66,   654,   279,     0,   679,     0,   100,   278,   654,   279,
       0,   236,   278,   654,    66,   654,   680,   279,     0,    64,
     654,     0,     0,   696,   278,   657,   279,     0,   696,   278,
     279,     0,    22,   278,   617,    10,   378,   279,     0,   684,
       0,   685,     0,   245,   278,   654,   277,   654,   279,     0,
     246,   278,   617,   277,   618,   279,     0,   686,     0,   688,
       0,   244,   691,   687,    54,     0,   244,   691,   687,    53,
     692,    54,     0,   197,   690,   181,   692,     0,   687,   197,
     690,   181,   692,     0,   244,   689,    54,     0,   244,   689,
      53,   692,    54,     0,   197,   633,   181,   692,     0,   689,
     197,   633,   181,   692,     0,   654,     0,   654,     0,   617,
       0,   218,     0,   219,     0,   220,     0,   221,     0,   222,
       0,   223,     0,   224,     0,   225,     0,     6,     0,     0,
      98,     0,   206,     0,   720,     0,   720,     0,   720,     0,
     720,     0,   720,     0,   720,     0,   720,     0,   720,     0,
     720,     0,   720,     0,   720,     0,   720,     0,   720,     0,
     720,     0,   720,     0,   720,     0,   720,     0,   720,     0,
     720,     0,   720,     0,   720,     0,   720,     0,   720,     0,
     206,     0,   721,     0,   235,     0,   238,     0,   236,     0,
     246,     0,   249,     0,   255,     0,   251,     0,   245,     0,
     248,     0,   254,     0,   247,     0,   256,     0,   257,     0,
     258,     0,   268,     0,   262,     0,   263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   536,   538,   542,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   569,   573,   577,   581,   585,   590,   591,
     595,   597,   599,   603,   604,   608,   612,   614,   616,   618,
     620,   624,   626,   630,   632,   636,   643,   648,   653,   658,
     663,   668,   673,   679,   683,   684,   686,   688,   692,   694,
     696,   698,   702,   703,   710,   712,   714,   718,   719,   723,
     727,   728,   732,   734,   741,   745,   747,   752,   758,   759,
     761,   767,   769,   770,   774,   775,   781,   784,   790,   791,
     794,   797,   801,   805,   808,   813,   822,   826,   829,   832,
     834,   836,   838,   840,   842,   844,   846,   848,   853,   857,
     859,   861,   871,   875,   877,   888,   890,   894,   896,   897,
     903,   909,   911,   913,   917,   919,   923,   925,   929,   931,
     934,   935,   942,   960,   962,   966,   968,   972,   974,   977,
     978,   981,   986,   987,   990,   994,  1003,  1011,  1020,  1025,
    1026,  1029,  1035,  1037,  1040,  1041,  1045,  1047,  1049,  1051,
    1053,  1057,  1059,  1062,  1063,  1067,  1069,  1070,  1071,  1073,
    1077,  1079,  1081,  1083,  1087,  1092,  1099,  1105,  1112,  1119,
    1152,  1153,  1156,  1161,  1165,  1166,  1169,  1177,  1181,  1182,
    1185,  1186,  1189,  1191,  1195,  1196,  1197,  1200,  1201,  1202,
    1208,  1213,  1218,  1220,  1222,  1226,  1227,  1231,  1232,  1239,
    1243,  1247,  1256,  1262,  1263,  1266,  1268,  1273,  1275,  1280,
    1286,  1292,  1296,  1302,  1304,  1308,  1313,  1314,  1315,  1317,
    1319,  1323,  1325,  1328,  1329,  1333,  1338,  1340,  1345,  1346,
    1348,  1356,  1360,  1362,  1365,  1366,  1367,  1368,  1371,  1375,
    1379,  1386,  1393,  1398,  1404,  1407,  1409,  1411,  1413,  1417,
    1420,  1424,  1427,  1430,  1433,  1442,  1453,  1463,  1473,  1483,
    1485,  1489,  1491,  1495,  1496,  1500,  1506,  1507,  1511,  1516,
    1518,  1520,  1524,  1526,  1530,  1531,  1535,  1539,  1540,  1543,
    1548,  1550,  1554,  1556,  1560,  1565,  1566,  1569,  1573,  1575,
    1577,  1581,  1582,  1586,  1587,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1603,
    1605,  1606,  1609,  1611,  1615,  1619,  1624,  1628,  1633,  1637,
    1641,  1643,  1647,  1651,  1657,  1662,  1667,  1669,  1671,  1675,
    1677,  1679,  1683,  1684,  1685,  1687,  1691,  1695,  1697,  1701,
    1703,  1705,  1710,  1712,  1716,  1717,  1721,  1726,  1727,  1731,
    1733,  1735,  1737,  1741,  1742,  1743,  1746,  1750,  1754,  1758,
    1800,  1807,  1815,  1817,  1821,  1822,  1826,  1832,  1839,  1859,
    1863,  1865,  1867,  1871,  1882,  1886,  1887,  1891,  1892,  1900,
    1903,  1909,  1913,  1919,  1923,  1929,  1933,  1941,  1952,  1963,
    1965,  1967,  1971,  1975,  1977,  1981,  1983,  1985,  1987,  1989,
    1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,  2007,  2009,
    2013,  2015,  2019,  2025,  2029,  2032,  2039,  2041,  2043,  2046,
    2049,  2054,  2058,  2064,  2065,  2069,  2078,  2082,  2084,  2086,
    2088,  2090,  2094,  2095,  2099,  2101,  2103,  2105,  2111,  2114,
    2116,  2120,  2128,  2129,  2130,  2131,  2132,  2133,  2134,  2135,
    2136,  2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,
    2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2157,
    2159,  2163,  2165,  2170,  2176,  2178,  2180,  2184,  2186,  2193,
    2199,  2200,  2204,  2212,  2214,  2216,  2218,  2220,  2222,  2224,
    2228,  2229,  2236,  2246,  2247,  2251,  2252,  2258,  2262,  2264,
    2266,  2268,  2270,  2272,  2274,  2276,  2278,  2280,  2282,  2284,
    2291,  2292,  2295,  2296,  2299,  2304,  2311,  2312,  2316,  2325,
    2331,  2332,  2335,  2336,  2337,  2338,  2355,  2360,  2365,  2384,
    2401,  2408,  2409,  2416,  2420,  2426,  2432,  2440,  2444,  2450,
    2454,  2458,  2464,  2468,  2475,  2481,  2487,  2495,  2500,  2505,
    2512,  2513,  2514,  2517,  2518,  2521,  2522,  2523,  2530,  2534,
    2545,  2551,  2605,  2667,  2668,  2675,  2688,  2693,  2698,  2705,
    2707,  2714,  2715,  2716,  2720,  2725,  2730,  2741,  2742,  2743,
    2746,  2750,  2754,  2756,  2760,  2764,  2765,  2768,  2772,  2776,
    2777,  2780,  2782,  2786,  2787,  2791,  2795,  2796,  2800,  2801,
    2805,  2806,  2807,  2808,  2811,  2813,  2817,  2819,  2823,  2825,
    2828,  2830,  2832,  2836,  2838,  2840,  2844,  2846,  2848,  2852,
    2856,  2858,  2860,  2864,  2866,  2870,  2871,  2875,  2879,  2881,
    2885,  2886,  2891,  2899,  2903,  2905,  2907,  2911,  2913,  2917,
    2918,  2922,  2926,  2928,  2930,  2934,  2936,  2940,  2942,  2946,
    2949,  2957,  2961,  2963,  2967,  2969,  2973,  2975,  2982,  2994,
    3008,  3012,  3016,  3020,  3024,  3026,  3028,  3030,  3034,  3036,
    3038,  3042,  3044,  3046,  3050,  3052,  3056,  3058,  3062,  3063,
    3067,  3068,  3072,  3073,  3078,  3082,  3083,  3087,  3088,  3089,
    3093,  3098,  3099,  3103,  3105,  3109,  3110,  3114,  3116,  3120,
    3123,  3128,  3130,  3134,  3135,  3140,  3145,  3147,  3149,  3151,
    3153,  3155,  3157,  3159,  3166,  3168,  3172,  3173,  3179,  3182,
    3184,  3188,  3190,  3197,  3199,  3203,  3207,  3209,  3211,  3217,
    3219,  3223,  3224,  3228,  3230,  3233,  3234,  3238,  3240,  3242,
    3246,  3247,  3251,  3253,  3260,  3263,  3270,  3271,  3274,  3278,
    3282,  3289,  3290,  3293,  3298,  3303,  3304,  3308,  3312,  3313,
    3316,  3317,  3324,  3326,  3330,  3332,  3334,  3337,  3338,  3341,
    3345,  3347,  3350,  3352,  3358,  3359,  3363,  3367,  3368,  3373,
    3374,  3378,  3382,  3383,  3388,  3389,  3392,  3397,  3402,  3405,
    3412,  3413,  3415,  3416,  3418,  3422,  3423,  3425,  3427,  3431,
    3433,  3437,  3438,  3440,  3444,  3445,  3446,  3447,  3448,  3449,
    3450,  3451,  3452,  3453,  3458,  3460,  3462,  3464,  3466,  3468,
    3470,  3472,  3479,  3481,  3483,  3485,  3487,  3489,  3491,  3493,
    3495,  3497,  3499,  3501,  3503,  3505,  3507,  3509,  3513,  3514,
    3520,  3522,  3527,  3529,  3531,  3533,  3538,  3540,  3544,  3546,
    3550,  3552,  3554,  3556,  3560,  3564,  3568,  3570,  3574,  3579,
    3584,  3591,  3596,  3601,  3610,  3611,  3615,  3619,  3633,  3651,
    3652,  3653,  3654,  3655,  3656,  3657,  3658,  3659,  3660,  3662,
    3664,  3671,  3673,  3675,  3682,  3689,  3696,  3698,  3700,  3701,
    3702,  3703,  3705,  3707,  3711,  3715,  3731,  3747,  3751,  3755,
    3756,  3760,  3761,  3765,  3767,  3769,  3771,  3773,  3777,  3778,
    3780,  3796,  3812,  3816,  3820,  3822,  3826,  3830,  3833,  3836,
    3839,  3842,  3847,  3849,  3854,  3855,  3859,  3866,  3873,  3880,
    3887,  3888,  3892,  3898,  3899,  3900,  3901,  3904,  3906,  3908,
    3911,  3918,  3927,  3934,  3943,  3945,  3947,  3949,  3955,  3964,
    3967,  3971,  3972,  3976,  3985,  3987,  3991,  3993,  3997,  4003,
    4004,  4007,  4011,  4015,  4016,  4019,  4021,  4025,  4027,  4031,
    4033,  4037,  4039,  4043,  4046,  4049,  4052,  4054,  4056,  4058,
    4060,  4062,  4064,  4066,  4070,  4071,  4074,  4082,  4085,  4088,
    4091,  4094,  4097,  4100,  4103,  4106,  4109,  4112,  4115,  4118,
    4121,  4124,  4127,  4130,  4133,  4136,  4139,  4142,  4145,  4148,
    4151,  4156,  4157,  4162,  4164,  4165,  4166,  4167,  4168,  4169,
    4170,  4171,  4172,  4173,  4174,  4175,  4176,  4178,  4179,  4180
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
     421,   421,   422,   422,   423,   424,   425,   426,   427,   428,
     429,   429,   430,   431,   432,   433,   434,   434,   434,   435,
     435,   435,   436,   436,   436,   436,   437,   438,   438,   439,
     439,   439,   440,   440,   441,   441,   442,   443,   443,   444,
     444,   444,   444,   445,   445,   445,   446,   447,   448,   449,
     450,   451,   452,   452,   453,   453,   454,   455,   456,   457,
     458,   458,   458,   459,   460,   461,   461,   462,   462,   463,
     463,   464,   465,   466,   467,   468,   468,   469,   470,   471,
     471,   471,   472,   473,   473,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     475,   475,   476,   477,   478,   478,   478,   478,   478,   478,
     478,   479,   480,   481,   481,   482,   482,   482,   482,   482,
     482,   482,   483,   483,   484,   484,   484,   484,   484,   484,
     484,   485,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   487,
     487,   488,   488,   489,   490,   490,   490,   491,   491,   492,
     493,   493,   494,   494,   494,   494,   494,   494,   494,   494,
     495,   495,   496,   497,   497,   498,   498,   499,   500,   500,
     500,   500,   500,   500,   500,   500,   500,   500,   500,   500,
     501,   501,   502,   502,   503,   503,   504,   504,   505,   505,
     506,   506,   507,   507,   507,   507,   507,   507,   507,   507,
     507,   508,   508,   509,   509,   509,   509,   510,   510,   511,
     511,   511,   512,   512,   513,   513,   513,   514,   514,   514,
     515,   515,   515,   516,   516,   517,   517,   517,   518,   518,
     519,   519,   519,   520,   520,   521,   521,   521,   521,   522,
     522,   523,   523,   523,   524,   524,   524,   525,   525,   525,
     526,   527,   528,   528,   529,   530,   530,   531,   532,   533,
     533,   534,   534,   535,   535,   536,   537,   537,   538,   538,
     539,   539,   539,   539,   540,   540,   541,   541,   542,   542,
     543,   543,   543,   544,   544,   544,   545,   545,   545,   546,
     547,   547,   547,   548,   548,   549,   549,   550,   551,   551,
     552,   552,   553,   554,   555,   555,   555,   556,   556,   557,
     557,   558,   559,   559,   559,   560,   560,   561,   561,   562,
     562,   562,   563,   563,   564,   564,   565,   565,   566,   567,
     568,   569,   570,   571,   572,   572,   572,   572,   573,   573,
     573,   574,   574,   574,   575,   575,   576,   576,   577,   577,
     578,   578,   579,   579,   580,   581,   581,   582,   582,   582,
     583,   584,   584,   585,   585,   586,   586,   587,   587,   588,
     588,   589,   589,   590,   590,   591,   592,   592,   592,   592,
     592,   592,   592,   592,   593,   593,   594,   594,   595,   596,
     596,   597,   597,   598,   598,   599,   600,   600,   600,   600,
     600,   601,   601,   602,   602,   603,   603,   604,   604,   604,
     605,   605,   606,   606,   607,   607,   608,   608,   609,   610,
     611,   612,   612,   613,   614,   615,   615,   616,   617,   617,
     618,   618,   619,   619,   620,   620,   620,   621,   621,   511,
     622,   622,   623,   623,   624,   624,   625,   626,   626,   627,
     627,   628,   629,   629,   630,   630,   630,   631,   632,   632,
     633,   633,   633,   633,   633,   634,   634,   634,   634,   634,
     634,   635,   635,   635,   636,   636,   636,   636,   636,   636,
     636,   636,   636,   636,   637,   637,   637,   637,   637,   637,
     637,   637,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   639,   639,
     640,   640,   641,   641,   641,   641,   642,   642,   643,   643,
     644,   644,   644,   644,   645,   646,   647,   647,   648,   648,
     648,   649,   649,   649,   650,   650,   651,   652,   653,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   655,   655,   655,   656,   657,
     657,   658,   658,   659,   659,   659,   659,   659,   660,   660,
     660,   660,   660,   661,   662,   662,   663,   664,   664,   664,
     664,   664,   665,   665,   666,   666,   667,   668,   669,   670,
     671,   671,   672,   673,   673,   673,   673,   674,   674,   674,
     674,   674,   674,   674,   674,   674,   674,   674,   675,   676,
     677,   678,   678,   679,   680,   680,   681,   681,   682,   683,
     683,   684,   684,   685,   685,   686,   686,   687,   687,   688,
     688,   689,   689,   690,   691,   692,   693,   693,   693,   693,
     693,   693,   693,   693,   694,   694,   695,   696,   697,   698,
     699,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,   710,   711,   712,   713,   714,   715,   716,   717,   718,
     719,   720,   720,   721,   721,   721,   721,   721,   721,   721,
     721,   721,   721,   721,   721,   721,   721,   721,   721,   721
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
       1,     2,     3,     4,     2,     6,     4,     8,     9,     6,
       8,     6,     4,     0,     4,     2,     1,     3,     0,     2,
       4,     0,     1,     1,     3,     3,     7,     2,     0,     2,
       2,     3,     3,     0,     1,     2,     4,     1,     3,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     5,     0,
       4,     7,     3,     0,     1,     3,     3,     7,     7,     1,
       1,     3,     4,     7,     2,     1,     3,     1,     1,     6,
       3,     0,     0,     0,     0,     3,     0,     9,     9,     1,
       1,     0,     2,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     5,     5,     5,     5,     5,     5,
       2,     0,     4,     2,     3,     3,     2,     2,     3,     3,
       2,     3,     6,     1,     2,     4,     3,     2,     2,     2,
       2,     3,     1,     3,     3,     3,     2,     2,     4,     4,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     0,     2,     2,     0,
       1,     2,     2,     2,     2,     2,     4,     3,     2,     2,
       1,     3,     7,     1,     0,     1,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     2,     2,     2,
       1,     1,     1,     1,     4,     5,     1,     3,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     6,     5,     3,     0,     2,
       2,     0,     3,     0,     4,     1,     5,     4,     1,     4,
       1,     2,     2,     1,     1,     1,     2,     2,     2,     2,
       0,     3,     5,     1,     1,     2,     3,     1,     2,     3,
       0,     1,     1,     1,     5,     5,     6,     1,     1,     1,
       2,     4,     1,     0,     5,     1,     0,     3,     2,     1,
       0,     2,     0,     1,     0,     3,     1,     0,     1,     2,
       1,     1,     1,     1,     2,     2,     1,     2,     3,     1,
       1,     3,     3,     1,     2,     3,     1,     2,     0,     2,
       1,     1,     0,     1,     1,     1,     3,     2,     3,     0,
       1,     3,     4,     5,     1,     3,     4,     3,     0,     1,
       3,     3,     1,     1,     0,     1,     1,     4,     0,     2,
       4,     0,     3,     0,     2,     0,     2,     0,     9,    11,
       0,     0,     0,     0,     3,     2,     1,     0,     3,     5,
       3,     2,     5,     2,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     0,     2,     1,     3,     1,     1,     1,
       6,     1,     0,     3,     0,     1,     3,     6,     3,     4,
       2,     3,     0,     1,     2,     1,     1,     1,     2,     1,
       2,     1,     2,     0,     3,     0,     1,     3,     1,     2,
       0,     2,     0,     2,     0,     4,     1,     2,     1,     1,
       0,     1,     3,     2,     1,     1,     2,     1,     4,     3,
       1,     3,     4,     0,     8,     5,     1,     1,     4,     4,
       4,     1,     1,     5,     5,     1,     3,     3,     1,     1,
       1,     3,     7,     7,     5,     3,     0,     1,     1,     1,
       2,     0,     1,     1,     1,     0,     3,     1,     3,     1,
       0,     3,     1,     3,     1,     3,     3,     1,     1,     3,
       1,     2,     1,     3,     3,     1,     2,     3,     3,     3,
       3,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     1,     1,
       5,     6,     3,     4,     5,     6,     3,     4,     3,     4,
       3,     4,     4,     5,     4,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,    11,    11,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     4,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     4,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     6,     1,
       6,     1,     4,     7,     2,     0,     4,     3,     6,     1,
       1,     6,     6,     1,     1,     4,     6,     4,     5,     3,
       5,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   600,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,   677,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   582,    18,   587,   588,   589,     5,    17,   581,
     583,    19,   648,   644,    11,     8,   756,   757,    21,   761,
     762,     4,   489,     0,     0,     0,     0,     0,     0,   423,
     599,   602,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,     0,     0,    96,   654,   936,     0,    22,   934,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,     0,   373,    30,
      38,    35,    37,   785,    36,   785,  1021,  1023,  1025,  1024,
    1030,  1026,  1033,  1031,  1027,  1029,  1032,  1028,  1034,  1035,
    1036,  1038,  1039,  1037,     0,    32,     0,    33,     0,    67,
      69,  1013,  1022,     0,     0,     0,     0,     0,     0,     0,
       0,   598,     0,     0,   995,   671,   676,     0,     0,   607,
       0,   713,   715,  1015,     0,     0,     0,   108,   590,  1020,
       0,     2,     0,     0,   661,     0,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,     0,   451,   452,     0,  1005,   430,     0,  1009,
     427,   270,  1012,     0,   715,   426,   401,  1016,   604,   597,
     932,     0,   104,   152,   151,   105,   134,   219,   797,  1001,
       0,   103,   146,  1008,     0,    99,   270,   106,   938,   123,
     100,   204,   101,     0,   102,   785,  1019,   107,   147,   652,
     653,     0,   937,   935,     0,    75,     0,  1006,   732,   515,
    1004,   508,     0,   514,   519,   509,   510,   517,   511,   512,
     513,   518,   338,     0,   272,    31,     0,    40,   784,    39,
       0,     0,     0,     0,     0,     0,   790,     0,     0,     0,
       0,     0,     0,     0,   596,   942,     0,   923,   670,   670,
     671,   683,   681,   994,   684,     0,   685,   675,   671,     0,
       0,     0,     0,     0,     0,   623,   616,   605,   606,   608,
     610,   611,   612,   619,   620,   613,     0,   714,  1014,   109,
     270,   110,   204,   111,   785,   593,     0,     0,   645,     0,
     663,     0,     0,     0,     0,     0,   428,   490,   393,     0,
     393,     0,   222,   429,   433,   424,   487,   488,     0,   272,
     243,   479,     0,   425,   442,     0,   399,   400,   504,   603,
     601,   933,   149,   149,     0,     0,     0,   161,   153,   154,
     133,     0,    97,     0,     0,   112,   272,   122,   121,   124,
       0,     0,     0,     0,     0,   997,    76,   222,   551,     0,
     758,   759,   516,     0,     0,     0,     0,   901,   929,     0,
     996,     0,   903,     0,     0,   930,     0,   924,   915,   914,
     913,  1021,     0,     0,     0,   905,   906,   907,   916,   917,
     925,   926,  1025,   927,   928,     0,  1030,  1026,   931,     0,
       0,     0,     0,   884,   370,   770,   336,   879,   794,   768,
     904,   880,   918,   882,   883,   898,   899,   900,   919,   881,
     943,   944,   945,   959,   946,   961,   887,   885,   886,   969,
     970,   973,   974,   769,     0,     0,  1001,     0,     0,     0,
     374,   220,     0,     0,     0,   787,     0,     0,    28,    34,
       0,     0,    44,    70,    72,  1017,    68,     0,     0,     0,
     789,     0,    53,     0,     0,     0,     0,    52,   595,     0,
       0,     0,   680,   678,     0,   687,     0,   686,   688,   693,
     674,     0,   642,     0,   617,   628,   614,   628,   615,   629,
     635,   639,   640,   624,   609,   732,   765,   798,     0,     0,
     272,     0,     0,   592,   591,   647,   649,   654,   646,   659,
       0,   667,   187,   178,     0,   492,   495,   175,   174,   130,
       0,   498,   493,     0,   499,   149,   149,   149,   149,   500,
     494,   491,     0,   437,     0,   439,   438,     0,   440,     0,
     434,     0,     0,   273,     0,     0,   446,     0,   447,   393,
     480,     0,   486,     0,     0,     0,   483,  1001,   404,   403,
     503,     0,   421,   150,     0,     0,   159,     0,   158,     0,
       0,   148,   162,   163,   167,   166,   165,   155,   551,   563,
     538,   573,   541,   564,   574,     0,   580,   542,     0,   570,
       0,   565,   577,   537,   560,   539,   540,   535,   393,   520,
     521,   522,   530,   536,   523,   532,   553,     0,   558,   555,
     533,   570,   534,   113,   270,   114,     0,     0,   125,     0,
       0,   203,   243,   401,   391,     0,   222,     0,    82,    84,
       0,     0,     0,   779,     0,     0,     0,     0,   868,   869,
     870,     0,   731,   802,   811,   814,   819,   815,   816,   817,
     821,   822,   820,   823,   818,   800,     0,   995,     0,   995,
     920,     0,     0,   995,   995,   995,     0,   921,   922,     0,
       0,   984,     0,     0,     0,     0,   889,   888,   677,     0,
       0,   768,   335,  1018,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,   530,   553,     0,     0,   372,
       0,   283,     0,   786,    29,     0,     0,    45,    73,     0,
       0,    27,   776,     0,   792,   677,     0,   755,   776,     0,
       0,     0,     0,   594,     0,   670,     0,     0,   732,     0,
     692,     0,   585,     0,   584,   940,     0,   618,     0,   626,
     622,   621,     0,   632,     0,   637,   625,     0,   763,   764,
       0,     0,     0,   243,   391,     0,   658,     0,   665,     0,
     643,     0,     0,     0,   131,   188,   497,     0,     0,     0,
       0,     0,   393,   436,     0,   393,   394,   441,   391,     0,
     269,   391,   242,  1002,   214,   211,     0,   224,   393,   520,
     391,     0,     0,     0,   241,   244,   245,   246,   247,     0,
     445,   485,   484,   444,   443,     0,     0,     0,   407,   405,
     406,   402,     0,   393,   197,   156,   160,     0,     0,   164,
       0,   548,   561,   562,   578,     0,   575,   580,     0,   568,
     566,   567,   136,     0,     0,     0,   531,     0,     0,     0,
       0,   569,   272,     0,   391,   126,   202,     0,   205,   207,
     208,     0,     0,     0,     0,   222,   222,    79,     0,    86,
      78,   549,   550,   998,   551,     0,     0,     0,   813,   801,
       0,     0,   871,   872,   873,     0,   805,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     986,   987,   988,   989,   990,   991,   992,   993,     0,     0,
       0,     0,   979,     0,     0,     0,     0,     0,   995,   337,
     897,   896,   771,   909,     0,   892,  1000,   891,   890,   893,
     894,   895,   967,     0,   902,   796,   795,   376,   375,     0,
     276,   291,     0,   282,   284,   788,     0,     0,     0,     0,
       0,     0,    25,    54,    42,    62,    64,     0,     0,    71,
       0,   781,     0,   791,   995,     0,   781,    50,    48,     0,
       0,     0,   679,   682,     0,   694,   695,   699,   697,   698,
     712,     0,   725,   689,   691,  1010,   586,   941,   627,   636,
     631,   630,     0,   641,   766,   767,   799,   391,     0,     0,
     650,   672,   651,   660,     0,   662,   666,   177,   184,   183,
       0,   180,   496,   149,   194,   179,   189,   190,   171,   170,
     939,   173,   172,   501,     0,     0,     0,   435,   391,   391,
     394,   281,   274,   283,   213,   393,     0,   394,   215,   210,
     216,     0,     0,   252,     0,   449,   394,   393,   391,   448,
       0,     0,     0,   420,   506,   198,   199,   157,     0,   169,
       0,     0,     0,   553,     0,   576,     0,     0,   393,   394,
       0,   526,   528,     0,     0,     0,     0,     0,     0,     0,
     401,   283,   149,   128,   243,   200,   421,   995,   396,   379,
     380,     0,    83,     0,   222,     0,    88,     0,    85,     0,
       0,     0,   760,  1003,     0,     0,   806,     0,     0,   812,
       0,     0,   804,   803,     0,   858,     0,   849,   848,     0,
     827,     0,   856,   874,   866,     0,     0,     0,   828,   852,
       0,     0,   831,     0,     0,     0,     0,     0,     0,     0,
     829,     0,     0,   830,     0,   860,     0,     0,   824,     0,
       0,   825,     0,     0,   826,     0,     0,   222,     0,   947,
       0,     0,   962,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   985,     0,     0,   983,     0,     0,   975,     0,
       0,     0,     0,     0,   908,   966,     0,   271,   290,     0,
     348,   371,   285,    66,    58,    59,    65,    60,    61,     0,
       0,     0,    23,    26,    42,    43,   551,   551,     0,   773,
     793,     0,     0,   772,    51,    49,    46,     0,     0,   723,
     697,     0,   721,   716,   717,   719,     0,     0,   710,     0,
     730,   633,   634,   638,   283,   201,   396,     0,   664,   182,
       0,     0,     0,   195,     0,   191,     0,     0,     0,   431,
     230,   225,   911,   226,   227,   228,   229,     0,     0,   275,
       0,     0,   223,   231,     0,   252,   654,   248,     0,   450,
     481,   482,   412,   413,   409,   408,   411,   410,   393,   505,
     394,   168,   999,     0,     0,   544,     0,   543,   579,     0,
     571,     0,   137,     0,   524,     0,   552,   553,   559,   557,
       0,   554,   391,     0,     0,     0,   120,   129,   206,   393,
       0,     0,   392,     0,    98,     0,   118,   117,    94,     0,
       0,    90,     0,    87,     0,   864,   865,   808,   807,   810,
     809,     0,     0,     0,   677,     0,     0,   867,     0,     0,
       0,     0,     0,     0,   859,   857,   853,     0,   861,     0,
       0,     0,     0,   862,     0,     0,     0,     0,     0,     0,
     953,   952,     0,     0,   949,   948,     0,   957,   956,   955,
     954,   951,   950,     0,   965,   981,   980,     0,     0,     0,
       0,     0,     0,     0,   910,   277,     0,     0,   287,   288,
       0,  1001,     0,     0,     0,     0,     0,     0,     0,  1023,
    1028,     0,     0,   369,   301,   296,     0,   348,   295,   305,
     306,   307,   308,   309,   316,   317,   310,   312,   313,   315,
       0,   320,     0,   363,   365,   364,     0,     0,     0,     0,
       0,     0,  1001,    55,    56,     0,    57,    63,    24,   777,
       0,   778,   775,   780,   783,   782,     0,   754,     0,    47,
     693,   708,   696,   722,   718,   720,     0,     0,     0,     0,
       0,   734,     0,   392,   655,   656,   673,   149,   185,   181,
       0,   197,   196,   192,     0,   394,   912,   279,   280,   394,
     394,   218,   232,   233,   393,     0,   249,     0,     0,     0,
       0,     0,     0,     0,     0,   283,   502,   546,     0,   547,
       0,   135,     0,   218,   138,   139,   141,   142,   143,   527,
     529,   525,   556,   283,   421,   394,   197,     0,     0,     0,
     377,     0,   381,   393,    89,     0,     0,    91,   553,     0,
     850,     0,     0,   995,   876,   875,     0,     0,   854,     0,
       0,     0,     0,   863,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   968,   221,   958,   960,     0,     0,
     982,   977,   976,     0,   971,   972,   732,   224,   286,     0,
     324,     0,     0,     0,   319,     0,   333,   318,   349,   350,
       0,  1011,   366,   367,     0,   297,     0,   302,   299,   354,
       0,     0,   321,     0,   311,   304,   314,   303,   347,    41,
     551,   732,   702,   723,   711,   709,   724,   726,   728,   729,
     740,   668,   394,   378,   657,     0,   176,   193,   394,   145,
     268,     0,     0,   209,   234,     0,     0,     0,   252,   235,
     238,   785,     0,   394,   418,   419,   415,   414,   417,   416,
       0,   545,   572,   144,   132,   140,     0,   393,   265,   127,
     394,     0,   732,   395,   382,     0,     0,    92,    93,    80,
       0,   835,   843,     0,   836,   844,   839,   847,   851,   855,
     837,   845,   838,   846,   832,   840,   833,   841,   834,   842,
     964,   963,   978,   725,     0,   293,   289,     0,   322,     0,
     338,     0,     0,     0,   351,     0,   362,     0,     0,     0,
       0,   357,   355,     0,     0,     0,   342,     0,   343,     0,
     774,   725,   704,   701,     0,     0,   736,   738,   739,   733,
       0,   266,   186,   432,   212,   217,   236,   252,   785,   239,
     258,   251,   253,   422,   394,     0,   397,     0,   384,   385,
     387,   388,   725,   394,    77,    95,     0,   730,   292,     0,
     323,   341,     0,   326,     0,   332,     0,   361,   360,  1007,
     359,   348,     0,     0,     0,     0,   334,     0,     0,   730,
       0,   700,   707,   727,   737,   740,   240,   258,     0,   252,
     254,   255,   267,   394,   723,   387,     0,     0,   730,     0,
     732,   734,     0,     0,     0,     0,   348,   368,   356,   358,
       0,   353,     0,   345,   344,   734,     0,   705,   744,     0,
     741,     0,   745,   252,     0,     0,   250,     0,   257,     0,
     256,   398,   390,   386,     0,   383,   119,   725,   648,   294,
       0,   339,   325,   329,   331,     0,     0,     0,   348,   648,
       0,   703,   735,   740,     0,   747,     0,   743,   746,   237,
       0,     0,   261,   260,   259,   723,   730,   661,     0,   348,
       0,     0,   348,   346,   661,   706,   742,     0,   753,   264,
     262,   263,     0,   734,   878,   340,   330,   348,   352,   327,
     669,     0,   750,     0,   749,   389,   648,   328,   748,     0,
       0,   661,   751,     0,   877,   752,     0,     0,     0
};

static const short yydefgoto[] =
{
    1936,    20,    21,   477,   124,   125,   126,   127,  1242,   741,
     736,    22,   139,   992,   993,   994,   995,   128,   129,   482,
     483,    23,    82,   386,   889,   657,   658,   659,  1135,  1136,
     245,    24,    74,    25,   157,    26,   375,    75,  1354,   227,
     379,   649,  1123,  1346,   545,   215,   371,  1109,  1543,  1544,
    1545,  1546,  1547,  1548,   221,   237,   212,   594,   213,   367,
     368,   369,   601,   602,   603,   604,   605,   547,   548,   549,
    1050,  1047,  1048,  1279,  1508,   550,  1055,  1056,  1057,  1284,
    1097,   230,   321,   381,   877,   878,   879,   815,   816,   817,
    1663,   577,   467,  1402,   660,  1077,  1289,  1521,  1522,  1523,
    1669,   880,   579,   824,   825,   826,   827,  1307,   828,  1819,
    1820,  1821,  1893,   225,   319,   643,   200,   349,   473,   572,
     573,   979,   468,  1070,   982,   983,   984,  1427,  1428,  1229,
    1726,  1429,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1457,  1733,  1458,   433,   434,
    1834,  1747,  1459,  1460,  1461,  1877,  1628,  1629,  1740,  1741,
    1462,  1463,  1464,  1465,  1624,    27,    28,   264,   469,   470,
     234,   323,  1128,  1129,  1130,  1692,  1778,  1779,  1780,   882,
    1560,   564,  1067,  1352,   232,   645,   358,   590,   591,   841,
     843,  1319,    29,    59,   806,   803,   343,   344,   353,   354,
     192,   193,   355,  1086,   585,   833,   197,   165,   336,   337,
     560,   205,   592,  1320,    30,    96,   628,   629,   630,  1110,
    1111,   631,   725,   633,   634,  1103,  1479,   866,   635,   726,
     637,   638,   639,   640,   859,   641,   642,   856,    31,    32,
      33,    34,    35,   534,    36,   499,    37,    38,    61,   209,
     360,    39,   307,   308,   309,   310,   311,   312,   313,   314,
     770,   315,  1032,  1273,   519,   520,   775,   521,    40,  1466,
      42,   164,   535,   536,   241,  1506,  1042,   330,   541,  1045,
     790,    43,   747,   502,   297,  1277,  1654,   144,   145,   146,
     295,   506,   507,   508,   761,   758,  1015,  1016,  1017,  1752,
    1811,  1846,  1018,  1019,  1268,  1781,   151,  1266,  1270,  1646,
    1647,  1501,   390,  1651,  1848,  1760,  1849,  1850,  1851,  1887,
    1921,  1924,  1467,  1468,    46,    47,   391,  1469,    49,    50,
     525,  1470,   435,   436,    51,  1001,  1480,  1249,  1483,   267,
     268,   474,   489,   490,   743,   437,   438,   528,   672,   905,
     673,   674,   675,   676,  1159,   677,   678,   679,   680,   681,
     682,   683,   684,   685,   907,  1162,  1163,  1375,   710,   686,
     440,   441,   964,  1291,   442,   443,   444,   445,   446,   447,
     448,   663,    79,   243,   229,  1061,  1112,   765,   449,   450,
     451,   452,   453,   454,   455,  1599,   456,   457,   458,   459,
     460,   461,   955,   462,   702,  1216,   703,  1213,   948,   296,
     463,   464,   892,  1321,   965,   218,   812,  1142,   249,   195,
     246,  1798,   222,  1922,  1024,  1471,  1020,   130,   465,   204,
     206,   996,   712,   235,   158,   466,   132
};

static const short yypact[] =
{
    1978,  1047,   100,  2820,   284,   986,   259,  2274,   314,  1694,
     611,   451,  2046,   100,   286,   428,  5599,   877,  5599,   289,
     228,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   374,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  6885,  5599,  5599,  5599,  5599,  5599,-32768,
  -32768,   505,   595,  5599,  5599,  5599,   682,  5599,   633,  5599,
    5599,-32768,  5599,  5599,-32768,   241,-32768,   643,-32768,-32768,
     631,  5599,-32768,  5599,  5599,  5599,   641,  5599,  5599,  5599,
    5599,   633,  5599,  5599,  5599,  5599,-32768,  5599,   486,   725,
  -32768,-32768,-32768,   598,-32768,   598,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   775,   639,   819,-32768,   275,-32768,
  -32768,-32768,-32768,  5599,  5599,  5599,   782,   858,   867,   604,
     156,   827,   512,   636,   610,   772,-32768,  5599,   936,   599,
     859,-32768,  5599,-32768,  5599,  5599,  5599,-32768,-32768,-32768,
    5599,-32768,  1006,   184,   778,   652,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   379,-32768,-32768,   595,-32768,-32768,   235,-32768,
  -32768,   774,-32768,   956,-32768,-32768,   625,-32768,   904,-32768,
  -32768,   862,-32768,   707,-32768,-32768,  1055,-32768,-32768,-32768,
     595,-32768,-32768,-32768,   488,-32768,   774,-32768,-32768,   491,
  -32768,  1020,-32768,  1021,-32768,   598,-32768,-32768,-32768,-32768,
  -32768,  1011,-32768,-32768,   897,-32768,  1031,-32768,   944,-32768,
  -32768,-32768,   897,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  4792,  5599,   982,-32768,  5599,-32768,-32768,-32768,
    3696,   574,  1000,  4029,  5599,  1063,   888,  1111,  1120,  3696,
    1042,  1064,  1072,  4029,   952,-32768,  6210,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4448,-32768,-32768,-32768,  1024,
    5599,  1100,  1013,   175,  5599,  1030,-32768,-32768,   599,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  5599,-32768,-32768,-32768,
     774,-32768,  1020,-32768,   598,  1081,  6210,  1049,-32768,  6210,
    1149,   224,   953,    81,   957,   609,   652,-32768,  1185,   470,
  -32768,  5599,-32768,   379,-32768,-32768,-32768,-32768,  5599,   982,
    1914,  1007,  2893,   946,-32768,  6885,-32768,-32768,   264,-32768,
  -32768,-32768,   955,   955,   595,  1107,   595,  1259,   707,-32768,
  -32768,  1533,-32768,  5599,  5599,-32768,   982,-32768,-32768,  1194,
     401,   950,  5599,  1219,  5599,-32768,-32768,    48,   487,  3782,
  -32768,-32768,-32768,   958,   960,   961,  1026,-32768,-32768,   962,
  -32768,   965,-32768,   971,   978,-32768,   979,-32768,-32768,-32768,
  -32768,   985,   990,  1058,  1071,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   995,-32768,-32768,  4878,  1010,  1012,-32768,  6210,
    6210,  4545,  5599,-32768,-32768,-32768,  1009,   983,-32768,  1097,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1016,  1002,  1015,  1533,  1022,    13,
  -32768,-32768,  1035,  1279,   456,-32768,  5599,  1124,-32768,-32768,
    5599,  5599,   457,-32768,-32768,-32768,-32768,  5599,  5599,   511,
  -32768,  5599,-32768,  1248,  5599,  3696,  1168,  1044,-32768,  5599,
    5125,   201,-32768,-32768,  6210,-32768,  1256,  1050,-32768,   155,
  -32768,   637,-32768,   109,-32768,   114,-32768,   114,-32768,  1059,
  -32768,   113,-32768,  1152,-32768,   322,-32768,-32768,  1073,  1052,
     982,  1067,  1331,-32768,-32768,  1070,-32768,   219,-32768,   972,
    1155,  1144,-32768,   139,  1254,  1255,-32768,-32768,-32768,-32768,
     595,-32768,-32768,  1257,-32768,   955,   955,   955,   955,-32768,
    1078,-32768,  1332,-32768,  1340,-32768,-32768,  1319,-32768,  1533,
  -32768,  1533,   514,-32768,  1357,  5599,-32768,  6735,-32768,   450,
  -32768,  5599,   761,   956,  1195,  1160,  1235,  1198,-32768,-32768,
  -32768,   164,  1242,-32768,  1184,   633,-32768,   595,-32768,  1366,
    1296,-32768,  1259,-32768,-32768,-32768,-32768,-32768,   681,  1201,
  -32768,-32768,-32768,  1203,-32768,  1246,  1128,-32768,  1312,  1133,
     218,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1132,-32768,-32768,-32768,   110,  1136,  1137,    43,
  -32768,  1133,-32768,-32768,   774,-32768,  1353,  1408,-32768,   595,
     595,-32768,  1914,   625,-32768,  1290,-32768,  1263,  1145,-32768,
    6977,  5266,  1286,-32768,  1297,  1146,  3868,  1148,  1129,  1171,
    1438,  3530,   366,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  3093,   780,  5211,   144,
  -32768,  5599,  6210,   869,   875,   937,  1317,-32768,-32768,  6210,
    4115,  1097,   148,  1230,  6210,  5211,   257,   257,   286,   745,
    1150,   350,-32768,-32768,  5211,  6210,  5599,  6210,  6210,  6210,
    6210,  6210,  5458,  2284,-32768,-32768,  1405,  1151,  5599,-32768,
    5599,  1385,  5599,-32768,-32768,  3312,  1265,-32768,-32768,  1226,
    4029,-32768,  1374,   750,-32768,   286,  1161,-32768,  1374,  3312,
    1382,  1383,  5599,-32768,   350,-32768,   404,  4600,  1252,  5211,
  -32768,  5599,-32768,   471,-32768,-32768,   157,-32768,  1266,-32768,
  -32768,-32768,  5599,   804,  5599,-32768,-32768,  5599,-32768,-32768,
    5211,  5599,  1447,  1914,-32768,  6210,  1211,  6210,  1333,  1214,
  -32768,   595,   595,   595,-32768,   387,-32768,  1184,  1184,  1258,
     633,   609,-32768,-32768,  1189,-32768,-32768,-32768,   143,  5599,
  -32768,-32768,-32768,-32768,  1448,-32768,  1192,  1427,-32768,  1444,
  -32768,  1387,  1388,   598,-32768,-32768,-32768,-32768,-32768,  1451,
  -32768,-32768,-32768,-32768,-32768,  5599,  6781,   633,  1348,  1349,
    1350,-32768,   633,-32768,   807,-32768,-32768,  1320,   595,-32768,
      29,  1321,-32768,-32768,-32768,   633,-32768,  1128,   317,-32768,
  -32768,-32768,  1439,   317,  1322,   317,-32768,   633,   633,  1210,
     633,-32768,   982,  5599,-32768,  1391,-32768,   759,-32768,-32768,
  -32768,   264,  1327,  5599,   767,    60,-32768,-32768,  1216,  1479,
  -32768,-32768,-32768,-32768,   487,  5599,  1338,  4201,-32768,-32768,
    1338,  3868,-32768,-32768,-32768,   123,-32768,   377,  2474,  4115,
    4115,  6210,  6210,  1853,  1224,   420,  2145,  6210,  2454,   495,
    2715,  2813,  5544,  3074,  3172,  3433,  6210,  6210,  1493,  6210,
    1225,  6210,  1228,   560,  6210,  6210,  6210,  6210,  6210,  6210,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1440,   895,
     167,  5211,-32768,  4115,  6210,   190,   373,  1232,   610,-32768,
  -32768,-32768,-32768,  1097,    -6,-32768,-32768,  1484,   257,   257,
     698,   698,-32768,   776,-32768,-32768,-32768,-32768,-32768,   779,
  -32768,  1323,  1495,  1385,-32768,-32768,  5599,  5599,  5599,  5599,
    5599,  5599,  1236,-32768,   465,-32768,-32768,  3312,  1389,-32768,
     403,  1401,  5599,-32768,   610,  5211,  1401,  1236,  1241,  3312,
    2356,  1453,-32768,-32768,  3923,  1244,   830,-32768,-32768,-32768,
    1245,  4115,  1452,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   315,-32768,-32768,-32768,-32768,-32768,   784,  1327,
  -32768,-32768,-32768,  1097,  5599,-32768,-32768,-32768,-32768,-32768,
     787,-32768,-32768,   955,  1515,-32768,   387,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1250,  4115,  3258,-32768,-32768,-32768,
  -32768,-32768,-32768,  1385,-32768,-32768,  3258,-32768,-32768,-32768,
  -32768,   598,   598,  1283,  1253,-32768,-32768,-32768,-32768,-32768,
     412,   161,   781,-32768,  1522,-32768,-32768,-32768,  5599,-32768,
     284,   796,  1365,  1405,  1264,-32768,  1184,   820,-32768,-32768,
     640,-32768,  1267,  5599,   713,  1269,  1270,   633,  1272,  1535,
     625,  1385,   955,  1255,  1914,-32768,  1242,   610,  1354,  1367,
  -32768,  1344,-32768,   633,  1415,  1503,-32768,  6977,-32768,   633,
    1329,  1504,-32768,-32768,  1282,  1289,-32768,  4115,  4115,-32768,
    4115,  4115,-32768,  1557,   172,  1097,  1291,-32768,-32768,  1293,
    1097,  5791,-32768,-32768,-32768,  1475,  1300,  1303,  1097,   581,
    1305,  1306,  1097,  6210,  6210,  1224,  6210,  5877,  1307,  1308,
    1097,  1309,  1310,  1097,  6210,  1097,  1316,  1318,  1097,  1326,
    1330,  1097,  1336,  1339,  1097,   655,   667,-32768,   677,-32768,
     694,  6210,-32768,   742,   758,   768,   852,   907,   987,  6210,
    6210,  5211,-32768,  1521,   191,  1097,  1416,  5211,-32768,  6210,
    6210,  5211,  6210,  6210,-32768,-32768,  5599,-32768,-32768,  5599,
    6480,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  3312,
    1532,  3312,-32768,  1236,   465,-32768,   347,   347,   526,-32768,
  -32768,  4448,   833,-32768,  1236,  1241,  1334,  2356,  1337,   665,
    1341,  4600,  1476,-32768,  1487,  1489,  1520,  5211,   644,  1607,
    1554,-32768,-32768,-32768,  1385,-32768,  1354,    23,  1358,  1466,
     595,  1501,  1184,  1502,  1184,-32768,  4115,   124,   941,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  3258,  3258,-32768,
    1495,  6210,-32768,   509,  1485,  1283,   241,-32768,  4115,-32768,
  -32768,-32768,  1508,  1509,  1510,  1511,  1512,  1513,-32768,-32768,
  -32768,-32768,-32768,  1368,   284,-32768,  1258,-32768,-32768,   317,
  -32768,  3258,   195,   317,-32768,   317,-32768,  1405,-32768,-32768,
    1369,-32768,-32768,   264,  1495,  1184,-32768,  1255,-32768,-32768,
    4448,  1622,-32768,   279,-32768,  1371,-32768,  1358,-32768,  1372,
     595,   229,  1376,-32768,   595,-32768,-32768,-32768,  1557,-32768,
    1557,  6210,  1496,  1496,   286,  1377,   882,-32768,  1496,  1496,
    6210,  1496,  1496,   186,  1097,-32768,  1045,  6210,  1097,  1496,
    1496,  1496,  1496,  1097,  1496,  1496,  1496,  1496,  1496,  1496,
  -32768,-32768,  1381,  1533,-32768,-32768,  1008,-32768,-32768,-32768,
  -32768,-32768,-32768,  1028,  1065,-32768,-32768,  5211,  5211,  1596,
    1481,  1036,   885,  1109,  1097,-32768,  1533,  1392,-32768,-32768,
    1625,  1626,  4933,    59,  1394,  1393,  1581,  6210,  1398,  1399,
    5009,  5599,  5599,-32768,-32768,-32768,  1623,  6445,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     252,-32768,  1403,-32768,-32768,-32768,  1594,  1407,  1411,  1412,
    1413,  1404,   583,-32768,-32768,  1550,-32768,-32768,-32768,-32768,
    1514,-32768,-32768,-32768,-32768,-32768,  1256,-32768,  2356,  1334,
    1681,-32768,   830,-32768,-32768,-32768,  4600,   899,  5599,  6210,
    4115,  1551,  1495,-32768,-32768,-32768,-32768,   955,-32768,-32768,
     595,   807,-32768,-32768,   131,-32768,-32768,-32768,-32768,-32768,
    1097,  1666,   509,-32768,   812,  5599,-32768,  1611,   136,  1507,
    1613,  1516,  1656,  1621,  1659,  1385,-32768,-32768,  1435,-32768,
    1436,-32768,  1618,  1666,   195,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1385,  1242,-32768,   807,  1522,  1651,  1587,
  -32768,  1327,-32768,-32768,-32768,  1603,   183,-32768,  1405,  1605,
    1097,  1446,  1454,   610,-32768,-32768,  1455,  1457,  1097,  1459,
    1463,  6210,  6210,  1097,  1465,  1467,  1468,  1469,  1470,  1471,
    1472,  1473,  1477,  1478,-32768,-32768,-32768,-32768,  6210,  1482,
  -32768,-32768,-32768,  5211,-32768,-32768,  1252,   150,-32768,  1662,
  -32768,  6124,  5599,  6210,-32768,  4115,  1097,-32768,-32768,-32768,
    1456,-32768,-32768,-32768,  5599,-32768,   182,-32768,  1526,-32768,
     455,  1449,-32768,  4327,-32768,-32768,-32768,-32768,-32768,-32768,
     347,  1252,  5599,   744,-32768,-32768,  1492,-32768,  1097,   366,
     422,-32768,-32768,-32768,-32768,  1184,-32768,-32768,-32768,-32768,
  -32768,  1494,  5599,-32768,-32768,  1632,  1641,  5599,  1283,-32768,
  -32768,   598,  5599,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1495,-32768,-32768,-32768,-32768,-32768,  1495,-32768,-32768,-32768,
  -32768,  4676,  1252,-32768,-32768,  6210,   595,-32768,-32768,-32768,
     595,-32768,-32768,  6210,-32768,-32768,-32768,-32768,  1097,  1097,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1097,-32768,-32768,  1452,  3258,-32768,-32768,  1488,-32768,   805,
    4792,  1057,   141,  1498,-32768,  1669,-32768,  5599,  5599,   284,
     121,-32768,-32768,  1491,  1686,  4115,-32768,   844,-32768,  1506,
  -32768,  1452,  1497,-32768,  4115,  6210,-32768,-32768,  1663,-32768,
    1517,-32768,-32768,-32768,-32768,-32768,-32768,  1283,   598,-32768,
    1650,-32768,-32768,-32768,-32768,  1522,-32768,  4676,  1519,   830,
  -32768,-32768,  1452,  1097,-32768,-32768,  1109,  1554,-32768,  1338,
  -32768,  1634,  4327,-32768,  1610,-32768,  4327,-32768,-32768,-32768,
  -32768,  6526,   182,  6210,  4327,   145,-32768,  4327,  5346,  1554,
    5599,-32768,   366,-32768,-32768,  3610,-32768,  1650,   179,  1283,
    1664,  1665,-32768,-32768,   665,  1518,  4676,  1713,  1554,  1524,
    1252,  1551,  1525,  4256,  1529,   857,  6526,  1530,-32768,-32768,
    1085,   149,  1758,-32768,-32768,  1551,   906,-32768,-32768,  1531,
    1536,   874,  5599,  1283,   504,   504,-32768,  1762,-32768,  1624,
  -32768,-32768,-32768,   830,  4676,-32768,-32768,  1452,  1682,-32768,
    4327,  1530,-32768,-32768,  1761,  4327,  1777,  1768,  6526,  1682,
    5599,-32768,-32768,  3610,  1541,-32768,  5599,-32768,-32768,-32768,
    1612,   200,-32768,-32768,-32768,   803,  1554,   778,   917,  6526,
     132,  5599,  6526,-32768,   778,-32768,-32768,  5599,  1737,-32768,
  -32768,-32768,  4115,  1551,-32768,-32768,-32768,  6526,-32768,-32768,
  -32768,  1543,  1546,  1548,-32768,   366,  1682,-32768,-32768,  5599,
    5599,   778,-32768,  1545,-32768,-32768,  1828,  1829,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  -109,   185,-32768,   316,  1559,   587,-32768,
    -325,-32768,-32768,  -581,   -41,  -631, -1081,  1820,  1560,  1552,
    1096,-32768,-32768,-32768,   701,-32768,  1183,   959,-32768,   708,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   718,-32768,-32768,-32768,-32768,-32768,
     300,-32768,-32768,  1527,-32768,-32768,-32768,  -342,-32768,-32768,
  -32768,  1480,-32768,-32768,  1261,  -224,  1523,-32768,-32768,  -336,
  -32768,  -752,-32768,-32768,-32768,-32768,-32768,-32768,   794,-32768,
   -1252,-32768,-32768,  1534,  1069,   734,  1528,  1041,   736,-32768,
     327,   -55,  -226,-32768,  -320,   265, -1011,-32768,-32768,   354,
  -32768,  1537, -1111,-32768,-32768,-32768,-32768, -1200,   349,    63,
      62,    61,    30,-32768,-32768,-32768,-32768,  -114,  -255,-32768,
    1075,-32768,  -677,-32768,  -980,-32768,   903,-32768,-32768,-32768,
  -32768,-32768, -1324,  -931,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -399,   159,
  -32768,  -417,-32768,-32768,-32768,-32768,-32768,   263,-32768,    94,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1169,
  -32768,-32768,   860,-32768, -1122,-32768,-32768, -1276,   125,  -632,
     398,  -321,  -914,   627,-32768,-32768,  -557,  -774,-32768,-32768,
    -996, -1337,-32768,-32768,-32768,-32768,-32768,  1561,-32768,  1324,
    1555,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1569,
  -32768,-32768,-32768,-32768,-32768,-32768,  -542,  -435,-32768,  1046,
     573,  -589,  -353,-32768,-32768,-32768,  -315,  -955,-32768,  -343,
  -32768,-32768,-32768,-32768,  1268,-32768,-32768,  1056,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1899,-32768,
  -32768,-32768,-32768,-32768,  1606,-32768,-32768,-32768,  1402,-32768,
    1400,-32768,-32768,-32768,-32768,  1147,-32768,-32768,-32768,     6,
  -32768,  -875,-32768,  1131,  -499,-32768,-32768,  -921,-32768,-32768,
  -32768,    10,  -866,  -174,   554,-32768,-32768,  -646,-32768,  1773,
    -888,  -786,-32768,  1164, -1090, -1303,-32768,  -904,-32768,-32768,
  -32768,-32768,   910,-32768,-32768,   664,    32, -1001, -1167,-32768,
     171, -1008,  -505, -1196,   277,-32768,    45,-32768,    79,-32768,
    -823,-32768,  1933,  1934,-32768,-32768,  1410,  1936,-32768,-32768,
  -32768,  -188,  -280,  -408,-32768,  1190, -1130,   931,-32768,   -95,
    -688,  -809,-32768,-32768,-32768,  -341,   -38,  -449,  -613,-32768,
    -448,-32768,-32768,-32768,   351,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -504,-32768,   769,-32768,  1083,-32768,  1066,
    -968,-32768,  -616,-32768,   658,  -963,  -129,  -947,  -939,  -909,
     -10,     0,   -25,-32768,   -49,  -746,  -478,  -131,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   739,-32768, -1056,-32768,   463,
    -902,   740,-32768,   842,   297,  -699,  1379, -1160,-32768,   -59,
    1874,-32768,   -47,   -53, -1304,   523,   -40,   -36,  -135,   -15,
      -3,  -181,-32768,   -65,   -51,    -9,-32768
};


#define	YYLAST		7220


static const short yytable[] =
{
     131,   152,   198,   131,    78,   220,    41,   153,   216,   159,
     269,   289,   292,   288,   291,   509,   201,   317,   632,   567,
     778,   595,   569,   709,   976,   217,   251,   226,   636,   260,
    1144,   606,   724,   764,  1145,   818,   255,   238,   786,   744,
    1051,   254,   257,   228,   194,   196,   199,   202,   153,   207,
     256,  1231,   214,   980,   219,   196,   223,   262,   202,   261,
     153,   207,   958,   236,   131,  1302,   228,   233,   152,  1430,
    1222,   890,   247,   662,   153,   250,   196,   258,   247,   223,
     199,   202,  1036,   153,   207,   236,   131,   950,   202,   203,
     259,   324,   484,  1300,   574,   275,   881,   277,  1290,  1004,
     299,   231,   484,  1292,  1101,  1526,   973,  1126,  1290,   325,
    1259,   559,   376,  1292,   320,   503,  1251,  1482,  1008,  1293,
    1606,   647,   571,  1627,   219,   153,   219,  1294,   526,  1293,
    1349,  1147,   909,   864,   807,  1083,   808,  1294,   223,   909,
     383,  1344,   819,   318,   909,   202,   153,   236,  1327,   909,
     293,   159,  1039,   909,   791,  1415,  1299,  1295,  1474,  1876,
    1477,  1419,   899,  1303,  1296,   760,   276,  1295,  1007,   750,
     493,  1801,  1309,   328,  1296,   909,  1071,   773,  1498,  1073,
    1371,   529,  1917,  1641,   552,   345,   515,   322,  1080,  1068,
     327,  1736,  1524,   929,  1581,  1332,  1076,   137,   716,   909,
    1133,   951,   952,   717,   515,   -81,   530,  1612,  1314,   794,
     372,   838,   716,   797,   798,   799,   800,   717,   898,  -393,
    1690,  -690,   283,   906,   632,   471,  1854,   716,   475,   532,
     239,  1562,   717,  1060,   636,  1278,   869,   768,   346,  1350,
    1737,   860,  1121,  1217,  1218,   716,  1910,   512,   839,  1566,
     717,  1738,   239,  1022,   219,  1148,   910,   219,   829,  1657,
     766,   153,  1504,   910,   485,   131,   606,   240,   910,   588,
     153,  1223,  1505,   910,   485,   782,  1224,   910,   527,   305,
     589,  1622,  1623,   716,  1697,  1561,  1517,  1518,   717,   240,
     728,   199,   729,   851,  1502,   153,  1152,  1153,  1911,   910,
     738,    60,   478,   568,  1689,   516,  1100,   862,   769,   861,
     471,   478,   217,  1613,   582,   347,  1630,   586,   542,  1542,
    1541,   870,  1357,   910,   281,    83,   656,   543,   138,  1290,
    1290,   655,   219,   644,  1292,  1292,   522,   538,  1134,   219,
    1214,   219,   517,   219,   553,   953,   587,    14,  1211,  1315,
    1293,  1293,   840,   529,   596,  1739,   598,  1492,  1294,  1294,
     517,  1600,  1601,  1290,   202,   207,  1244,  1855,  1292,  1519,
     651,   646,  1417,   153,   909,   199,   716,   518,  1255,  1256,
    1107,   717,  1551,   338,  1293,  1150,   751,  1219,  1295,  1295,
     774,   865,  1294,  1304,  1305,  1296,  1296,  1146,  1802,   716,
    1642,  1087,  1149,  1515,   717,  1274,  1536,  1477,   928,  1807,
    1658,  1524,  1069,  1555,   653,  1673,  1243,   792,   930,  1724,
    1794,  1698,  1295,   713,  1842,   957,  1807,  1011,  1254,  1296,
     716,   339,  -690,   274,   962,   717,  1297,  1298,  1823,  1694,
     737,  1567,  1127,  1356,   718,   719,   720,   721,   753,   898,
     527,  1631,  1287,   898,   737,   282,  1311,   273,   718,   719,
     720,   721,    97,   844,  1735,  1486,  1271,   153,  1769,  1246,
     135,   202,   485,   718,   719,   720,   721,  1838,   153,   509,
     755,  1064,   153,  1830,  1066,   202,   153,   544,    76,  1053,
     159,   718,   719,   720,   721,   650,  1312,  1078,   910,   147,
    1035,  1824,  1485,   565,   161,   377,  1847,   162,   734,  1151,
    1750,  1756,  1874,  1173,  1743,   821,   566,  1272,  1164,   742,
     389,   285,  1094,   748,   661,   142,   143,   478,  1509,  1174,
     872,   720,   721,  -243,  1367,  1368,  1757,  1369,  1370,   820,
     795,   160,   575,   340,  1165,  1376,   845,  1722,   890,  1425,
    1863,    76,   274,  1250,  1903,  1680,  1787,    77,  1687,   484,
     163,   341,  1054,  1343,  1558,  1137,   813,  1816,   219,  1554,
     228,  1652,   813,  1686,  1175,  1916,  1905,  1063,  1919,  1141,
    1539,  1012,    98,   571,  1809,  1247,   716,   846,  1895,  1034,
    1106,   717,  1643,  1927,  1758,   342,   822,  1252,   977,   777,
    1313,  1659,   378,  1176,   148,  1660,  1661,   716,   210,   211,
      99,   149,   717,  1699,   217,  1828,   293,  1119,    14,  1856,
      77,   100,   718,   719,   720,   721,  1489,  1890,   356,   961,
     133,   287,  1027,  -243,   555,  1868,   373,   823,  1380,   875,
     876,  1688,   529,   219,   932,   718,   719,   720,   721,  1879,
    1220,   100,   893,  1889,   760,  -243,   331,   739,   102,   294,
     208,   891,  -243,   101,   661,  1240,  1058,  1059,  1891,   332,
    1177,  1212,   374,  1514,   745,   285,   718,   719,   720,   721,
     150,   716,   223,  1013,   556,  1703,   717,   301,   102,   224,
     471,    76,   471,   716,   985,  1528,  -243,   134,   717,   244,
    1896,   746,  1026,   716,   333,   357,   334,   966,   717,   252,
    1553,  1282,   737,  1788,   219,  1892,   285,  1926,   557,   219,
     716,   219,   302,   219,   716,   717,   485,   103,  1573,   717,
    1060,   485,  1262,   732,   740,   733,   558,   104,  1761,   527,
     485,  1918,  1241,   202,  1763,   217,  1263,   248,   202,  1773,
     303,  1062,  1025,   304,  1301,  1774,  1290,   103,  1656,  1772,
      77,  1292,   105,   153,   263,   153,  1310,   104,   716,   305,
    1264,   471,   219,   717,   219,   228,  1776,  1293,  1827,  1831,
    1345,  1049,  1049,  1052,   716,  1294,   293,  1331,  1089,   717,
     286,   809,   105,   810,   716,   306,   287,  1085,  1088,   717,
     219,  1845,   210,   211,   522,  1233,  1033,  1527,  1236,   362,
     287,  1262,   228,  1422,  1137,  1295,   335,  1093,  1115,  1116,
    1865,  1118,  1296,  1827,  1265,  1263,   219,   219,  1316,   926,
    1104,   716,   718,   719,   720,   721,   717,    76,  1099,  1202,
     285,   285,   228,   228,   504,   228,   363,   242,   364,  1264,
    -693,  1540,   510,   718,   719,   720,   721,  1550,   661,  1497,
    1822,  1595,  1827, -1011,   153,  1317, -1014,   529,   762,  1829,
    1262,   365,   265,  1754,   153,   293,   266,  1403,   716,  -693,
    -693,   293,  -693,   717,  1263,    76,  1143,  1649,  1913,  -693,
    -693,  -693,  -693,  -693,  1827,  -693,   366,  1262,  -693,  -693,
    -693,  -693,  -693,  1265,   270,  1120,  -693,  -693,  1264,  1861,
     763,  1263,  -693,   278,   290,  1131,   271,  1333,   934,  -723,
     287,   716,  1334,  1832,   936,  1237,   717,   718,   719,   720,
     721,  1212,  1912,   716,  1400,  1264,  1665,  1212,   717,   718,
     719,   720,   721,   293,  1095,  1096,  1401,  1234,   272,   718,
     719,   720,   721,  1030,    77,  1238,  1404,  1884,  1666,   850,
     350,  1210,  1265,   351,   527,  1667,   718,   719,   720,   721,
    1031,   509,   831,  1405,   832,  1027,  1914,   485,   202,   207,
     485,   236,   131,  1920,   387,  1235,   938,   279,   485,  1265,
    1333,  1607,   392,  1897,  1885,  1337,   280,  1535,   716,  1668,
     485,   485,  1732,   717,  1904,   202,   475,  1886,   352,   284,
    1934,   794,   143,   716,   718,   719,   720,   721,   717,   300,
    1258,  1407,   714,   316,   959,   154,   326,  1002,  1557,  1003,
     718,   719,   720,   721,   716,   219,  1124,  1408,  1125,   717,
     718,   719,   720,   721,   886,   329,  1132,  1409,    80,    81,
     632,  1931,   348,  1223,   716,  1225,  1226,   155,  1227,   717,
     636,  1124,   716,  1275,  1280,   370,  1281,   717,  1340,   361,
     509,   716,   156,  1324,   359,  1325,   717,   718,   719,   720,
     721,  1790,   380,   716,  1358,   382,   217,    52,   717,  1322,
    1362,   716,   228,   475,   384,   529,   717,  1329,    53,  1330,
    1323,  1723,  1582,   385,  1322,    54,  1932,  1933,   228,   976,
     714,   716,  1487,   388,   228,   219,   717,  1481,  1481,  1484,
    1806,  1807,   219,   716,   718,   719,   720,   721,   717,  1598,
      55,  1410,  1805,  1873,  1807,   716,  1751,  1212,  1212,   472,
     717,  1812,   389,  1792,   408,   409,   410, -1034,   480,   487,
     927,  1511,   931,  1513,   787, -1034,   935,   937,   939,  1223,
   -1034,  1575,   714, -1034,  1605,  1655,   488,   718,   719,   720,
     721,  1875,   418,   419,  1426,   757,   714,   491,  1644,   718,
     719,   720,   721,  1880,   492,  1881,  1411,  1782,   471, -1035,
     494,   217,   527,   495,  1807,    56,  1915, -1035,  1473, -1034,
    1476,   496, -1035,   829,   498, -1035,   511,   513, -1034,   514,
     523,   533,    14,   540,  1556,   551, -1034,   219,   562,   554,
    1431,  1472,   580,   583,   593,   597,   648,    57,   652,   654,
     485,    58,   485,   690,  1746, -1034,   687, -1034,   688,   689,
     691, -1035,  1695,   692,   718,   719,   720,   721,   485,   693,
   -1035, -1034,   202, -1034, -1034, -1034,   694,   695, -1035,   718,
     719,   720,   721,  -997,   715,   697,  1412,  1167,   696,  1171,
    1049,  1179,  1182,   699,  1187,  1190,  1193, -1035,   698, -1035,
     718,   719,   720,   721,   555,   723,   714,  1596,   704,   731,
     705,   727,  1748, -1035,   722, -1035, -1035, -1035, -1014,  1925,
     718,   719,   720,   721, -1034,   599,   735,  1597,   718,   719,
     720,   721,   529,   730,   749,  1604,   752,   718,   719,   720,
     721,   740,   757,  1212,  1538,  1867,   776,   759,   439,   718,
     719,   720,   721,  1793,   556,   781,   772,   718,   719,   720,
     721,   784,   780,   788,   789,   783, -1035,   785,   793,   542,
    1565,   796,   501,   542,  1569,   801,   802,   718,   719,   720,
     721,   439,   543,  1645,   804,   805,  1775,   811,   557,   718,
     719,   720,   721,  1611,   814,  1835,   836,   835,   837,  1837,
    -452,   718,   719,   720,   721,   842,   558,  1841,   285,   847,
     848,   854,   537,  1746,   852,   539,   853,  1746, -1034, -1034,
   -1034, -1034, -1034, -1034, -1034,  1746,   855,   857,  1843,   527,
   -1034,   858, -1034,   863,   867,   868,  1871,   873,   874,   883,
     885,   894,   886,   196,   896,   895,   900,   954,   864,   960,
     981,  1621,  1143,  1143,  1746,   287,   998,  1000,  1472,  1005,
   -1035, -1035, -1035, -1035, -1035, -1035, -1035,   997,  1009,  1010,
    1021,  1748, -1035,  1898, -1035,  1748, -1036,  1037,  1900,  1041,
    1028,  1044,  1060,  1748, -1036,  1046,  1844,  1065,  1074, -1036,
    1075,  1746, -1036,  1076,   814,  1084,  1746,  1081,  1082,   485,
    1090,  1091,  1092,  1102,  1098,  1108,  1113,   202,  1117,   318,
    1127,   701,  1748,  1122,  1139,   706,   707,   711,  1749,  1140,
    1049,   745,  1161,  1197,  1199,  1201,  1209,  1753, -1036,  1221,
     716,  1481,  1230,  1239,  1248,  1228,   153, -1036,  1241,  1257,
    1245,  1261,   600,  1267,  1762, -1036,  1269,  1283,  1286,  1748,
    1306,  1308,  1318,  1326,  1748,   940,   941,   942,   943,   944,
     945,   946,   947,  1328, -1036,  1342, -1036,  1335,  1338,  1339,
     106,  1341,   608,  1353,  1351,  1133,   609,  1671,  1360,  1364,
   -1036,  1365, -1036, -1036, -1036,   909,   754,  1363,  1366,  1372,
     756,  1373,  1730,  1377,   610,  1416,  1770,   611,  1378,   107,
     108,  1379,   109,  1381,  1382,  1389,  1390,  1391,  1392,   110,
     111,   112,   113,   114,  1394,   115,  1395,  1418,   116,   117,
     118,   119,   120,   202,  1396,  1475,   121,   122,  1397,  1496,
    1493,  1488,   123, -1036,  1398,  1143,  1490,  1399,   612,  1771,
    1491,  1494,   266,  1495,   613,   614,   615,  1499,   616,   617,
     618,  1500,   619,   318,  1507,   732,  1744,  1510,  1525,  1512,
    1529,  1530,  1531,  1532,  1533,  1534,  1559,  1537,  1552,  1563,
    1602,  1564,   620,   966,   621,  1568,  1574,  1749,   153,  1374,
    1594,  1749,  1603,   199,  1609, -1003,   529,   134,  1608,  1749,
    1614,  1615,  1749,  1817,  1617,  1618,   152,  1625,  1797,  1632,
    1633,  1639,   153,  1634,  1638,   622,  1784,  1635,  1636,  1637,
    1785,   760,  1662,  1650,  1672,  1674,  1640,  1675,  1749,  1768,
      99,   529,   623,  1677,  1676,  1678,  1679, -1036, -1036, -1036,
   -1036, -1036, -1036, -1036,  1681,  1682,  1683,  1691,  1693, -1036,
    1696, -1036,  1700,  1626,   624,  1701,  1727,  1745,   196,  1799,
    1766,  1767,  1734,  1702,  1704,  1749,  1705,   908,  1706,  1800,
    1749,   100,  1707,   529,  1710,  1803,  1711,  1712,  1713,  1714,
    1715,  1716,  1717,   101,   439,  1796,  1718,  1719,   933,   625,
     626,  1721,   152,   527,   529,   949,  1789,   529,   153,  1755,
     956,   439,  1804,  1764,  1795,  1810,   627,  1814,   102,  1818,
     439,   963,   529,   967,   968,   969,   970,   971,   963,  1808,
    1833,  1836,  1472,  1857,  1859,  1815,  1826,  1862,   527,   219,
    1852,  1025,  1864,  1866,  1869,  1872,   153,  1807,  1878,  1854,
    1882,   152,  1855,  1883,  1899,   162,  1901,   153,  1902,  1907,
    1923,  1909,  1928,  1929,  1935,   439,  1930,  1472,  1937,  1938,
     479,  1478,   140,  1908,   486,   497,   999,  1852,  1361,   884,
     527,  1347,  1359,   153,  1685,  1138,   439,   103,   607,   152,
    1285,   537,  1038,  1043,   546,   153,   531,   104,  1348,  1156,
    1079,   527,  1157,   849,   527,   563,   393,  1355,  1852,  1472,
    1684,  1025,  1725,  1670,   153,   394,  1664,   199,   576,   527,
    1853,  1858,   105,  1860,  1072,  1894,  1232,   578,   395,  1791,
    1472,  1742,  1143,  1472,   396,   397,  1839,   978,   199,  1276,
     106,  1653,  1825,  1503,   570,   561,  1549,   834,  1472,   871,
     584,  1114,   141,  1105,   524,   767,  1040,   771,   298,  1029,
     199,   199,   398,  1023,  1260,   399,  1813,  1759,  1906,   107,
     108,  1888,   109,    44,    45,   779,    48,  1253,  1006,   110,
     111,   112,   113,   114,  1385,   115,  1516,   575,   116,   117,
     118,   119,   120,   401,   402,  1336,   121,   122,  1420,  1765,
     830,   253,   123,  1620,     0,   403,     0,     0,     0,   404,
       0,     0,     0,     0,     0,     0,     0,  1154,  1155,  1160,
       0,     0,  1168,  1169,  1172,     1,  1180,  1183,  1185,  1188,
    1191,  1194,  1195,  1196,     0,  1198,     0,  1200,     0,     0,
    1203,  1204,  1205,  1206,  1207,  1208,     2,     0,     0,     0,
       0,     0,     0,     0,     3,     0,     0,   439,     0,     0,
    1215,     4,     0,     5,  1158,     6,   405,     0,     0,     0,
       7,     0,   406,     0,     0,     0,     0,     8,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     9,    99,     0,     0,     0,   408,   409,   410,   411,
     210,   211,    10,     0,     0,     0,     0,     0,     0,     0,
     412,   439,     0,     0,     0,     0,     0,     0,     0,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   107,   422,
       0,   109,     0,   100,   423,   424,     0,   425,   426,   427,
     112,   113,   114,   428,   115,   101,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,   136,
     106,   123,     0,     0,     0,   429,   430,     0,     0,    11,
     102,   500,     0,   432,     0,     0,    12,   287,     0,    13,
       0,    14,    15,     0,     0,     0,     0,     0,     0,   107,
     108,  1166,   109,     0,  1157,     0,     0,     0,   393,   110,
     111,   112,   113,   114,     0,   115,    16,   394,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
     395,     0,   123,     0,     0,     0,   396,   397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,   398,    17,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   963,     0,     0,
      18,    19,     0,     0,   105,     0,     0,     0,     0,  1383,
    1384,     0,  1386,  1388,     0,   401,   402,     0,     0,     0,
    1393,     0,   106,     0,     0,     0,     0,   403,     0,     0,
       0,   404,     0,     0,     0,     0,     0,  1406,     0,     0,
       0,     0,     0,     0,     0,  1413,  1414,   439,     0,     0,
       0,   107,   108,   439,   109,  1215,  1421,   439,  1423,  1424,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,     0,  1158,   439,   405,     0,
       0,     0,     0,     0,   406,    84,   974,     0,     0,     0,
       0,     0,    85,   439,   407,     0,    86,    87,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,   408,   409,
     410,   411,   210,   211,     0,     0,     0,    89,     0,     0,
       0,     0,   412,     0,     0,     0,     0,  1520,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,     0,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,     0,     0,   439,   429,   430,     0,
       0,     0,    90,   500,     0,   432,     0,     0,     0,   287,
     986,     0,     0,     0,     0,     0,     0,  1570,     0,    91,
       0,     0,     0,     0,     0,     0,  1578,     0,     0,     0,
       0,     0,     0,  1583,    92,  1571,  1572,     0,    93,     0,
    1170,  1576,  1577,  1157,  1579,  1580,     0,   393,     0,    94,
       0,     0,  1584,  1585,  1586,  1587,   394,  1588,  1589,  1590,
    1591,  1592,  1593,   439,   439,     0,     0,     0,    95,   395,
     106,     0,   911,     0,     0,   396,   397,     0,     0,     0,
     716,     0,     0,  1616,     0,   717,     0,     0,   912,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     108,     0,   109,   398,     0,     0,   399,     0,     0,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,   913,   989,   121,   122,     0,     0,
       0,     0,   123,   914,   401,   402,     0,     0,   975,     0,
       0,   915,   106,     0,     0,  1648,   403,     0,     0,     0,
     404,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     916,     0,   917,     0,     0,     0,     0,     0,     0,     0,
       0,   107,   108,     0,   109,     0,   918,     0,   919,   920,
     921,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,  1158,     0,   405,     0,     0,
       0,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,     0,     0,  1708,  1709,   922,
       0,     0,     0,     0,     0,     0,     0,   408,   409,   410,
     411,   210,   211,     0,  1720,     0,     0,     0,     0,   439,
       0,   412,     0,     0,     0,     0,     0,  1729,     0,  1731,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,  1178,   123,     0,  1157,     0,   429,   430,   393,     0,
       0,     0,   500,     0,   432,     0,     0,   394,   287,     0,
       0,     0,     0,   923,   924,   925,   718,   719,   720,   721,
     395,     0,     0,   961,     0,     0,   396,   397,     0,     0,
       0,  1783,     0,     0,     0,     0,     0,     0,     0,  1786,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,   402,     0,     0,  1181,
       0,  1648,  1157,     0,     0,     0,   393,   403,     0,     0,
       0,   404,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,   396,   397,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,  1840,
       0,    63,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,   398,     0,     0,   399,  1158,     0,   405,     0,
       0,    65,     0,     0,   406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   402,     0,     0,     0,   408,   409,
     410,   411,   210,   211,     0,   403,   581,     0,     0,   404,
       0,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,    66,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,    67,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,  1158,    68,   405,   429,   430,     0,
       0,     0,   406,   500,     0,   432,     0,     0,     0,   287,
      69,     0,   407,     0,    70,     0,     0,    71,     0,     0,
       0,     0,     0,     0,     0,    72,   408,   409,   410,   411,
     210,   211,     0,     0,     0,     0,     0,     0,     0,     0,
     412,     0,     0,     0,    73,     0,     0,     0,     0,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   107,   422,
       0,   109,     0,     0,   423,   424,     0,   425,   426,   427,
     112,   113,   114,   428,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
    1186,   123,     0,  1157,     0,   429,   430,   393,     0,     0,
       0,   500,     0,   432,     0,     0,   394,   287,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,   911,     0,     0,     0,   396,   397,     0,     0,   716,
       0,     0,     0,     0,   717,     0,     0,   912,   107,   108,
       0,   109,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,   114,   398,   115,     0,   399,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,     0,   913,     0,     0,     0,     0,     0,     0,
       0,     0,   914,     0,   401,   402,     0,     0,  1189,     0,
     915,  1157,     0,     0,     0,   393,   403,     0,     0,     0,
     404,     0,     0,     0,   394,     0,     0,     0,     0,   916,
       0,   917,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,   396,   397,   918,     0,   919,   920,   921,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,   399,  1158,     0,   405,     0,     0,
       0,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,   922,     0,
       0,     0,   401,   402,     0,     0,     0,   408,   409,   410,
     411,   210,   211,     0,   403,     0,     0,     0,   404,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,   396,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,     0,   398,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,  1158,     0,   405,   429,   430,     0,     0,
       0,   406,   500,     0,   432,     0,   400,     0,   287,     0,
       0,   407,   923,   924,   925,   718,   719,   720,   721,     0,
       0,     0,     0,     0,     0,   408,   409,   410,   411,   210,
     211,     0,     0,     0,     0,     0,   986,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,     0,   116,   117,   118,   119,
     120,   405,     0,     0,   121,   122,     0,     0,     0,  1192,
     123,     0,  1157,     0,   429,   430,   393,   407,     0,     0,
     500,     0,   432,     0,     0,   394,   287,     0,     0,     0,
     987,   408,   409,   410,     0,   210,   211,     0,   395,     0,
       0,     0,     0,     0,   396,   397,     0,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,     0,     0,     0,   988,     0,     0,   423,
     424,   989,   398,     0,     0,   399,     0,   990,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
       0,     0,     0,     0,     0,     0,   991,     0,     0,     0,
       0,  1288,     0,   401,   402,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,   403,     0,   107,   108,   404,
     109,     0,   394,     0,     0,     0,     0,   110,   111,   112,
     113,   114,     0,   115,     0,   395,   116,   117,   118,   119,
     120,   396,   397,     0,   121,   122,     0,     0,     0,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     665,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,   399,     0,  1158,     0,   405,     0,     0,     0,
       0,     0,   406,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
     401,   402,     0,     0,     0,     0,   408,   409,   410,   411,
     210,   211,   403,     0,     0,     0,   404,     0,     0,     0,
     412,     0,     0,     0,   901,     0,     0,     0,     0,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   107,   422,
       0,   109,     0,     0,   423,   424,     0,   425,   426,   427,
     112,   113,   114,   428,   115,     0,     0,   116,   117,   118,
     119,   120,     0,   708,     0,   121,   122,   667,     0,  1756,
       0,   123,     0,   405,     0,   429,   430,     0,     0,   406,
       0,   500,     0,   432,     0,     0,     0,   287,     0,   407,
       0,     0,     0,     0,  1757,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   409,   410,   411,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,   412,     0,     0,
       0,     0,     0,     0,     0,     0,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   107,   422,     0,   109,     0,
       0,   423,   424,     0,   425,   426,   427,   112,   113,   114,
     428,   115,  1758,     0,   116,   117,   118,   119,   120,   902,
     903,   904,   121,   122,     0,   393,     0,     0,   123,     0,
       0,     0,   429,   430,   394,     0,     0,     0,   671,     0,
     432,     0,     0,     0,   287,     0,   106,   395,     0,     0,
     664,     0,     0,   396,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   665,     0,     0,   107,   108,     0,   109,     0,
       0,   398,     0,     0,   399,   110,   111,   112,   113,   114,
       0,   115,     0,     0,   116,   117,   118,   119,   120,     0,
       0,     0,   121,   122,     0,     0,   476,     0,   123,     0,
       0,   393,   401,   402,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,   403,     0,     0,     0,   404,     0,
       0,     0,   106,   395,     0,     0,   666,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   665,     0,
       0,   107,   108,     0,   109,     0,     0,   398,     0,     0,
     399,   110,   111,   112,   113,   114,     0,   115,     0,   667,
     116,   117,   118,   119,   120,   405,     0,     0,   121,   122,
       0,   406,     0,     0,   123,     0,     0,     0,   401,   402,
       0,   407,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,   404,   408,   409,   410,   411,   210,
     211,     0,   897,     0,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,     0,     0,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   107,   422,     0,
     109,     0,     0,   423,   424,     0,   425,   426,   427,   112,
     113,   114,   428,   115,     0,   667,   116,   117,   668,   669,
     670,   405,     0,     0,   121,   122,     0,   406,     0,     0,
     123,     0,     0,     0,   429,   430,     0,   407,     0,     0,
     671,     0,   432,     0,     0,     0,   287,     0,     0,     0,
       0,   408,   409,   410,   411,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   412,    14,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
       0,     0,   116,   117,   668,   669,   670,     0,   393,   106,
     121,   122,     0,     0,     0,     0,   123,   394,     0,     0,
     429,   430,     0,     0,     0,     0,   671,     0,   432,     0,
     395,     0,   287,     0,     0,     0,   396,   397,   107,   108,
       0,   109,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,   114,     0,   115,   665,     0,   116,   117,   118,
     119,   120,     0,     0,   398,   121,   122,   399,     0,     0,
       0,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1014,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   401,   402,     0,   481,     0,
       0,     0,     0,   394,     0,     0,     0,   403,     0,     0,
       0,   404,     0,     0,     0,   106,   395,     0,     0,   666,
       0,     0,   396,   397,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   665,     0,     0,   107,   108,     0,   109,     0,     0,
     398,     0,     0,   399,   110,   111,   112,   113,   114,     0,
     115,     0,   667,   116,   117,   118,   119,   120,   405,     0,
       0,   121,   122,     0,   406,     0,     0,   123,     0,     0,
       0,   401,   402,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,   404,   408,   409,
     410,   411,   210,   211,     0,   897,     0,     0,     0,     0,
       0,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     107,   422,     0,   109,     0,     0,   423,   424,     0,   425,
     426,   427,   112,   113,   114,   428,   115,     0,   667,   116,
     117,   668,   669,   670,   405,     0,     0,   121,   122,     0,
     406,     0,     0,   123,     0,     0,     0,   429,   430,     0,
     407,     0,     0,   671,     0,   432,     0,     0,     0,   287,
       0,     0,     0,     0,   408,   409,   410,   411,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   412,     0,
       0,     0,     0,     0,     0,     0,     0,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   107,   422,     0,   109,
       0,     0,   423,   424,     0,   425,   426,   427,   112,   113,
     114,   428,   115,     0,     0,   116,   117,   118,   119,   120,
       0,   393,   106,   121,   122,     0,     0,     0,     0,   123,
     394,     0,     0,   429,   430,     0,     0,     0,     0,   671,
       0,   432,     0,   395,     0,   287,     0,     0,     0,   396,
     397,   107,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,   398,   121,   122,
     399,     0,     0,     0,   123,     0,     0,     0,     0,     0,
       0,     0,     0,   106,  1870,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,   401,   402,
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
     403,     0,   107,   108,   404,   109,     0,   394,     0,     0,
       0,     0,   110,   111,   112,   113,   114,     0,   115,     0,
     395,   116,   117,   118,   119,   120,   396,   397,     0,   121,
     122,     0,     0,     0,     0,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,   398,     0,     0,   399,     0,     0,
       0,   405,     0,     0,     0,     0,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   400,     0,   401,   402,     0,     0,     0,
       0,   408,   409,   410,   411,   210,   211,   403,     0,     0,
       0,   404,     0,     0,     0,   412,     0,     0,     0,     0,
       0,     0,     0,     0,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   107,   422,     0,   109,     0,     0,   423,
     424,     0,   425,   426,   427,   112,   113,   114,   428,   115,
       0,     0,   116,   117,   118,   119,   120,     0,   708,     0,
     121,   122,     0,     0,     0,     0,   123,     0,   405,     0,
     429,   430,   505,     0,   406,     0,   500,     0,   432,     0,
       0,     0,   287,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   409,
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
       0,     0,     0,     0,     0,     0,     0,     0,  1014,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
     400,   393,   401,   402,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,   403,     0,     0,     0,   404,     0,
       0,   107,   108,   395,   109,     0,     0,     0,     0,   396,
     397,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,     0,     0,   398,     0,     0,
     399,     0,     0,     0,  1777,     0,     0,     0,     0,     0,
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
     431,     0,   432,     0,     0,   700,   287,     0,     0,     0,
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
       0,   123,     0,     0,     0,     0,     0,     0,     0,  1610,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   401,   402,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,   403,     0,     0,
       0,   404,     0,     0,   107,   108,   395,   109,     0,     0,
       0,     0,   396,   397,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,     0,     0,
     398,     0,     0,   399,     0,  1619,     0,     0,   708,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,   406,     0,     0,     0,     0,   400,
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
       0,     0,     0,     0,   408,   409,   410,   411,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   412,     0,
       0,     0,     0,     0,     0,     0,     0,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   107,   422,     0,   109,
       0,     0,   423,   424,     0,   425,   426,   427,   112,   113,
     114,   428,   115,     0,     0,   116,   117,   118,   119,   120,
      76,   393,   106,   121,   122,     0,     0,     0,     0,   123,
     394,     0,     0,   429,   430,     0,     0,     0,     0,   500,
       0,   432,     0,   395,     0,   287,     0,     0,     0,   396,
     397,   107,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,   398,   121,   122,
     399,     0,     0,     0,   123,     0,     0,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,     0,     0,     0,   393,   401,   402,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
     403,     0,     0,     0,   404,     0,     0,     0,     0,   395,
       0,   107,   108,     0,   109,   396,   397,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,   398,   123,     0,   399,     0,     0,     0,
     975,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     429,   430,     0,   407,     0,     0,   500,   972,   432,     0,
       0,     0,   287,     0,  1184,     0,     0,   408,   409,   410,
     411,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,     0,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,     0,     0,   116,   117,
     118,   119,   120,     0,   393,   106,   121,   122,     0,     0,
       0,     0,   123,   394,     0,     0,   429,   430,     0,     0,
       0,     0,   500,     0,   432,     0,   395,     0,   287,     0,
       0,     0,   396,   397,   107,   108,     0,   109,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
     398,   121,   122,   399,     0,     0,     0,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     393,   401,   402,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,   403,     0,     0,     0,   404,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,   396,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   399,
       0,     0,     0,     0,  1374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
     406,     0,     0,     0,     0,     0,     0,   401,   402,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,   404,   408,   409,   410,   411,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   412,     0,
       0,     0,     0,     0,     0,     0,     0,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   107,   422,     0,   109,
       0,     0,   423,   424,     0,   425,   426,   427,   112,   113,
     114,   428,   115,     0,     0,   116,   117,   118,   119,   120,
     405,     0,     0,   121,   122,     0,   406,     0,     0,   123,
       0,     0,     0,   429,   430,     0,   407,     0,     0,   500,
       0,   432,     0,     0,     0,   287,     0,  1387,     0,     0,
     408,   409,   410,   411,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,   412,     0,     0,     0,     0,     0,
       0,     0,     0,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   107,   422,     0,   109,     0,     0,   423,   424,
       0,   425,   426,   427,   112,   113,   114,   428,   115,     0,
       0,   116,   117,   118,   119,   120,     0,   393,     0,   121,
     122,     0,     0,     0,     0,   123,   394,     0,     0,   429,
     430,     0,     0,     0,     0,   500,     0,   432,     0,   395,
       0,   287,     0,     0,     0,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,   401,   402,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,   403,     0,     0,     0,
     404,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,   396,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,   406,     0,     0,     0,     0,     0,     0,
     401,   402,     0,   407,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,   404,   408,   409,   410,
     411,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,     0,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   107,
     422,     0,   109,     0,     0,   423,   424,     0,   425,   426,
     427,   112,   113,   114,   428,   115,     0,     0,   116,   117,
     118,   119,   120,   405,     0,     0,   121,   122,     0,   406,
       0,     0,   123,     0,     0,     0,   429,   430,     0,   407,
    1728,     0,   500,     0,   432,     0,     0,     0,   287,     0,
       0,     0,     0,   408,   409,   410,   411,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,   412,     0,     0,
       0,     0,     0,     0,     0,     0,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   107,   422,     0,   109,     0,
       0,   423,   424,     0,   425,   426,   427,   112,   113,   114,
     428,   115,  1230,     0,   116,   117,   118,   119,   120,     0,
       0,     0,   121,   122,     0,     0,     0,     0,   123,     0,
       0,     0,   429,   430,     0,     0,     0,     0,   500,     0,
     432,     0,     6,     0,   287,     0,     0,  1230,     0,  -298,
       0,     0,     0,  1432,  1433,     0,  1434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1435,     0,     0,     0,     6,     0,  1436,
       0,     0,     0,     0,  -300,     0,     0,     0,  1432,  1433,
       0,  1434,     0,  1230,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1435,     0,
       0,     0,     0,     0,  1436,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1432,  1433,     0,  1434,     0,  1437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1435,     0,     0,     0,    14,     0,
    1436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1438,  1437,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,     0,     0,     0,
       0,     0,  1626,    14,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,  1438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
    1437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1439,   108,     0,   109,     0,     0,   106,     0,     0,    14,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
    1440,   118,   119,   120,  1438,     0,     0,   121,   122,  1441,
    1442,  1443,     0,   123,    16,  1439,   108,     0,   109,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
       0,   115,   106,     0,   116,  1440,   118,   119,   120,     0,
       0,     0,   121,   122,  1441,  1442,  1443,     0,   123,     0,
       0,     0,     0,     0,   608,     0,     0,     0,   609,     0,
       0,  1439,   108,     0,   109,   814,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   610,   115,     0,   611,
     116,  1440,   118,   119,   120,     0,     0,     0,   121,   122,
    1441,  1442,  1443,     0,   123,     0,     0,     0,     0,     0,
     608,     0,     0,     0,   609,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     612,     0,   610,     0,     0,   611,   613,   614,   615,     0,
     616,   617,   618,     0,   619,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   620,     0,   621,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   612,     0,     0,     0,
       0,     0,   613,   614,   615,     0,   616,   617,   618,     0,
     619,     0,     0,     0,     0,     0,     0,   622,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     620,     0,   621,     0,   623,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   624,     0,     0,     0,
       0,     0,     0,   622,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
     623,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   625,   626,     0,     0,     0,     0,     0,     0,     0,
     107,   108,   624,   109,     0,     0,     0,     0,   627,     0,
     110,   111,   112,   113,   114,     0,   115,   106,     0,   116,
     117,   118,   119,   120,     0,     0,   887,   121,   122,     0,
     609,     0,     0,   123,     0,     0,     0,   625,   626,     0,
       0,     0,     0,     0,   888,     0,   107,   108,   610,   109,
       0,   611,     0,     0,   627,     0,   110,   111,   112,   113,
     114,     0,   115,     0,     0,   116,   117,   118,   119,   120,
       0,     0,     0,   121,   122,     0,     0,     0,     0,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   612,     0,     0,     0,     0,     0,   613,   614,
     615,     0,   616,   617,   618,     0,   619,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,   620,     0,   621,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,     0,   182,   183,
     107,   108,     0,   109,     0,     0,   184,   185,     0,   622,
     110,   111,   112,   113,   114,   186,   115,   187,     0,   116,
     117,   118,   119,   120,     0,     0,   623,   121,   122,   188,
     189,   190,   191,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   624,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   625,   626,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     627
};

static const short yycheck[] =
{
       9,    16,    55,    12,     4,    64,     0,    16,    63,    18,
     105,   142,   143,   142,   143,   295,    56,   152,   371,   340,
     525,   363,   342,   431,   723,    63,    85,    67,   371,    94,
     896,   367,   467,   511,   900,   577,    89,    73,   537,   488,
     792,    88,    91,    68,    53,    54,    55,    56,    57,    58,
      90,   982,    62,   730,    63,    64,    65,    97,    67,    95,
      69,    70,   708,    72,    73,  1076,    91,    70,    83,  1229,
     958,   660,    81,   388,    83,    84,    85,    92,    87,    88,
      89,    90,   781,    92,    93,    94,    95,   700,    97,    57,
      93,   156,   273,  1073,   349,   133,   653,   135,  1066,   745,
     147,    69,   283,  1066,   850,  1305,   722,   881,  1076,   160,
    1014,   335,   226,  1076,   154,   289,  1004,  1247,   749,  1066,
    1423,   376,   348,  1447,   133,   134,   135,  1066,   316,  1076,
    1126,     8,     8,    23,   569,   823,   571,  1076,   147,     8,
     235,  1121,   577,   152,     8,   154,   155,   156,  1103,     8,
       6,   160,   784,     8,    15,  1211,  1070,  1066,  1239,    10,
    1241,  1217,   666,  1077,  1066,    10,   134,  1076,   749,   494,
     279,    50,  1086,   163,  1076,     8,   808,    64,  1268,   811,
       8,   316,    50,  1486,   103,   195,    29,   155,   820,    46,
       6,     9,  1303,    49,     8,  1109,    46,    12,    26,     8,
     140,    53,    54,    31,    29,   157,   320,   148,    47,   545,
     220,    47,    26,   555,   556,   557,   558,    31,   666,    24,
    1557,    66,    66,   671,   577,   263,    47,    26,   266,   324,
      11,  1353,    31,   204,   577,  1044,   193,   123,     3,  1127,
      58,    23,   874,    53,    54,    26,    46,   300,    84,    20,
      31,    69,    11,   758,   263,   132,   132,   266,   579,  1511,
     151,   270,   239,   132,   273,   274,   602,    48,   132,     5,
     279,   277,   249,   132,   283,   530,   282,   132,   316,   170,
      16,  1441,  1442,    26,   101,     6,  1297,  1298,    31,    48,
     277,   300,   279,   608,  1274,   304,   909,   910,    98,   132,
     481,   201,   270,   341,  1556,   130,   277,   628,   194,    91,
     348,   279,   350,   254,   352,    80,    64,   355,    94,   124,
    1331,   278,  1131,   132,   139,    66,   278,   103,    12,  1297,
    1298,   384,   341,   373,  1297,  1298,   304,   327,   278,   348,
     953,   350,   185,   352,   263,   197,   355,   163,   181,   188,
    1297,  1298,   188,   488,   364,   173,   366,  1261,  1297,  1298,
     185,  1417,  1418,  1331,   373,   374,   997,   188,  1331,  1300,
     380,   374,   181,   382,     8,   384,    26,   202,  1009,  1010,
     858,    31,  1337,     4,  1331,     8,   495,   197,  1297,  1298,
     277,   281,  1331,  1081,  1082,  1297,  1298,   901,   277,    26,
    1490,   836,   279,   279,    31,  1037,  1320,  1488,   688,   277,
     279,  1522,   269,  1344,   382,   279,   997,   278,   274,   269,
     279,   238,  1331,   432,   279,   705,   277,   752,  1009,  1331,
      26,    52,   277,   277,   714,    31,  1068,  1069,  1775,  1561,
     480,   212,   163,  1131,   272,   273,   274,   275,   499,   897,
     488,   199,  1065,   901,   494,   139,  1088,   182,   272,   273,
     274,   275,   148,   594,  1624,  1251,   151,   476,  1668,    66,
      19,   480,   481,   272,   273,   274,   275,  1801,   487,   759,
     279,   802,   491,  1786,   805,   494,   495,   263,   204,   102,
     499,   272,   273,   274,   275,    94,    84,   818,   132,    71,
     780,  1777,  1248,    33,   276,    14,  1810,   133,   476,   132,
    1640,    89,  1836,    18,    59,    65,    46,   202,    98,   487,
     198,   204,   843,   491,   177,   239,   240,   495,  1280,    34,
     644,   274,   275,    24,  1147,  1148,   114,  1150,  1151,   577,
     550,   252,    33,   164,   124,  1161,   595,  1603,  1137,  1226,
    1826,   204,   277,  1002,  1878,  1535,  1723,   273,  1554,   740,
     186,   182,   175,  1120,  1350,   885,   575,  1767,   577,  1343,
     595,  1502,   581,  1553,    79,  1899,  1880,   801,  1902,   894,
    1326,   755,   268,   809,  1751,   182,    26,   597,  1864,   777,
     273,    31,  1496,  1917,   172,   216,   146,  1005,   727,   277,
     188,  1515,   111,   108,   176,  1519,  1520,    26,   207,   208,
       6,   183,    31,  1568,   652,  1782,     6,   872,   163,  1819,
     273,    47,   272,   273,   274,   275,  1257,   123,     3,   279,
      19,   284,   763,   124,    25,  1831,   148,   187,    57,   649,
     650,  1555,   777,   652,   691,   272,   273,   274,   275,  1845,
     277,    47,   661,  1853,    10,   146,     4,   200,    84,    49,
     155,   661,   153,    59,   177,   200,   797,   798,   164,    17,
     175,   951,   184,  1286,   163,   204,   272,   273,   274,   275,
      16,    26,   691,   279,    75,  1573,    31,    88,    84,     7,
     728,   204,   730,    26,   732,  1308,   187,    86,    31,    68,
    1867,   190,   231,    26,    52,    80,    54,   716,    31,    68,
    1342,  1053,   752,  1724,   723,   211,   204,  1913,   109,   728,
      26,   730,   123,   732,    26,    31,   735,   153,  1374,    31,
     204,   740,    67,   277,   277,   279,   127,   163,  1652,   777,
     749,  1901,   277,   752,  1658,   783,    81,    83,   757,  1680,
     151,   800,   761,   154,  1075,  1686,  1724,   153,  1510,  1673,
     273,  1724,   188,   772,   278,   774,  1087,   163,    26,   170,
     105,   809,   781,    31,   783,   800,  1690,  1724,  1779,  1787,
    1122,   791,   792,   793,    26,  1724,     6,  1108,   837,    31,
     278,   277,   188,   279,    26,   196,   284,   835,   836,    31,
     809,  1809,   207,   208,   772,   986,   774,  1306,   989,   102,
     284,    67,   837,  1221,  1134,  1724,   164,   842,   867,   868,
    1828,   870,  1724,  1824,   159,    81,   835,   836,    47,    49,
     855,    26,   272,   273,   274,   275,    31,   204,   848,   279,
     204,   204,   867,   868,   290,   870,   139,   204,   141,   105,
     206,  1329,   298,   272,   273,   274,   275,  1335,   177,  1267,
    1774,  1403,  1863,   280,   873,    84,   283,  1002,   231,  1783,
      67,   164,   147,   129,   883,     6,   278,  1197,    26,   235,
     236,     6,   238,    31,    81,   204,   895,  1500,  1896,   245,
     246,   247,   248,   249,  1895,   251,   189,    67,   254,   255,
     256,   257,   258,   159,   129,   873,   262,   263,   105,  1823,
     273,    81,   268,   131,   278,   883,   277,   277,    49,    89,
     284,    26,   282,  1789,    49,   990,    31,   272,   273,   274,
     275,  1211,   129,    26,   279,   105,   124,  1217,    31,   272,
     273,   274,   275,     6,   137,   138,   279,   987,   129,   272,
     273,   274,   275,   149,   273,   991,   279,    83,   146,   278,
       4,    66,   159,     7,  1002,   153,   272,   273,   274,   275,
     166,  1251,   211,   279,   213,  1106,  1897,   986,   987,   988,
     989,   990,   991,  1904,   244,   988,    49,   129,   997,   159,
     277,  1426,   252,  1868,   120,   282,   129,  1318,    26,   187,
    1009,  1010,  1615,    31,  1879,  1014,  1044,   133,    52,   182,
    1931,  1347,   240,    26,   272,   273,   274,   275,    31,    83,
    1014,   279,   277,   164,   279,   148,    20,   277,  1349,   279,
     272,   273,   274,   275,    26,  1044,   277,   279,   279,    31,
     272,   273,   274,   275,   277,   267,   279,   279,    62,    63,
    1403,  1926,   278,   277,    26,   279,   277,   180,   279,    31,
    1403,   277,    26,   279,   277,    10,   279,    31,  1117,   207,
    1350,    26,   195,   277,   170,   279,    31,   272,   273,   274,
     275,   276,    62,    26,  1133,    64,  1124,    40,    31,  1098,
    1139,    26,  1117,  1131,    83,  1230,    31,   277,    51,   279,
    1100,  1606,    57,   206,  1113,    58,  1929,  1930,  1133,  1808,
     277,    26,   279,    82,  1139,  1124,    31,  1246,  1247,  1248,
     276,   277,  1131,    26,   272,   273,   274,   275,    31,    64,
      83,   279,  1745,   276,   277,    26,  1641,  1417,  1418,   157,
      31,  1754,   198,    86,   203,   204,   205,    18,   148,    86,
     687,  1282,   689,  1284,   182,    26,   693,   694,   695,   277,
      31,   279,   277,    34,   279,  1507,   278,   272,   273,   274,
     275,    86,   231,   232,  1229,    66,   277,    66,   279,   272,
     273,   274,   275,   277,    64,   279,   279,  1692,  1226,    18,
     148,  1229,  1230,   129,   277,   148,   279,    26,  1239,    70,
    1241,   129,    31,  1524,   252,    34,   182,   107,    79,   196,
     180,   130,   163,    64,  1345,   262,    87,  1226,    33,   262,
    1229,  1230,   215,   277,   269,   118,    32,   180,   278,    10,
    1239,   184,  1241,   207,  1633,   106,   278,   108,   278,   278,
     278,    70,  1563,   278,   272,   273,   274,   275,  1257,   278,
      79,   122,  1261,   124,   125,   126,   278,   278,    87,   272,
     273,   274,   275,   278,   281,   207,   279,   916,   278,   918,
    1280,   920,   921,   278,   923,   924,   925,   106,   207,   108,
     272,   273,   274,   275,    25,   283,   277,   279,   278,    10,
     278,   269,  1633,   122,   278,   124,   125,   126,   283,  1912,
     272,   273,   274,   275,   175,    46,   182,   279,   272,   273,
     274,   275,  1447,   278,    66,   279,   148,   272,   273,   274,
     275,   277,    66,  1603,  1324,  1830,   174,   277,   262,   272,
     273,   274,   275,   276,    75,   283,   277,   272,   273,   274,
     275,    10,   269,   188,   200,   278,   175,   277,    94,    94,
    1360,    94,   286,    94,  1364,   277,    24,   272,   273,   274,
     275,   295,   103,  1498,    24,    46,  1687,    10,   109,   272,
     273,   274,   275,  1432,    30,  1792,   216,   182,   143,  1796,
     182,   272,   273,   274,   275,   143,   127,  1804,   204,    23,
      94,   145,   326,  1792,   193,   329,   193,  1796,   269,   270,
     271,   272,   273,   274,   275,  1804,   278,    95,  1807,  1447,
     281,   278,   283,   281,   278,   278,  1833,    64,    10,   129,
     157,   135,   277,  1432,   278,   128,   278,   197,    23,   279,
      45,  1440,  1441,  1442,  1833,   284,   210,    63,  1447,   278,
     269,   270,   271,   272,   273,   274,   275,   182,    66,    66,
     198,  1792,   281,  1870,   283,  1796,    18,    10,  1875,   248,
     194,   128,   204,  1804,    26,   251,  1807,   278,    20,    31,
     278,  1870,    34,    46,    30,    24,  1875,    90,    90,  1488,
     132,   132,   132,   162,   164,    46,   164,  1496,   278,  1498,
     163,   425,  1833,   102,   278,   429,   430,   431,  1633,    20,
    1510,   163,   278,    10,   279,   277,    66,  1642,    70,   277,
      26,  1640,    17,   277,   113,   192,  1525,    79,   277,    66,
     131,   277,   263,   278,  1655,    87,    74,    12,   278,  1870,
     247,   278,    10,   168,  1875,   218,   219,   220,   221,   222,
     223,   224,   225,   279,   106,    10,   108,   280,   279,   279,
     206,   279,    19,   186,   200,   140,    23,  1525,    55,    55,
     122,   279,   124,   125,   126,     8,   500,   238,   279,   278,
     504,   278,  1612,    98,    41,    54,  1671,    44,   278,   235,
     236,   278,   238,   278,   278,   278,   278,   278,   278,   245,
     246,   247,   248,   249,   278,   251,   278,   181,   254,   255,
     256,   257,   258,  1612,   278,    73,   262,   263,   278,    89,
     134,   277,   268,   175,   278,  1624,   279,   278,    85,  1672,
     279,   134,   278,   134,    91,    92,    93,    20,    95,    96,
      97,    77,    99,  1642,   168,   277,  1630,   136,   153,   137,
     132,   132,   132,   132,   132,   132,    24,   279,   279,   278,
      54,   279,   119,  1662,   121,   279,   279,  1792,  1667,   163,
     279,  1796,   181,  1672,    39,    39,  1801,    86,   276,  1804,
     276,   278,  1807,  1768,   276,   276,  1691,    54,  1737,   276,
      86,   131,  1691,   276,   280,   152,  1696,   276,   276,   276,
    1700,    10,    26,   142,    83,   188,   182,    84,  1833,  1667,
       6,  1836,   169,    47,   188,    84,    47,   269,   270,   271,
     272,   273,   274,   275,   279,   279,    98,    66,   131,   281,
     117,   283,   117,   197,   191,   279,    64,   278,  1737,  1738,
      98,    90,   276,   279,   279,  1870,   279,   671,   279,  1739,
    1875,    47,   279,  1878,   279,   254,   279,   279,   279,   279,
     279,   279,   279,    59,   688,    86,   279,   279,   692,   226,
     227,   279,  1777,  1801,  1899,   699,   278,  1902,  1777,   277,
     704,   705,    86,   279,   276,   278,   243,   114,    84,   129,
     714,   715,  1917,   717,   718,   719,   720,   721,   722,   283,
     156,   181,  1801,   129,   129,   278,   277,   279,  1836,  1808,
    1815,  1810,    89,   279,   279,   276,  1815,   277,    50,    47,
     279,  1826,   188,   277,    53,   133,    39,  1826,    50,   278,
      83,   209,   279,   277,   279,   759,   278,  1836,     0,     0,
     271,  1244,    12,  1886,   274,   283,   740,  1852,  1137,   656,
    1878,  1123,  1134,  1852,  1544,   886,   780,   153,   368,  1864,
    1056,   785,   783,   787,   331,  1864,   322,   163,  1124,     6,
     819,  1899,     9,   602,  1902,   338,    13,  1131,  1883,  1878,
    1543,  1880,  1607,  1524,  1883,    22,  1522,  1886,   350,  1917,
    1817,  1820,   188,  1821,   809,  1855,   983,   350,    35,  1730,
    1899,  1628,  1901,  1902,    41,    42,  1802,   728,  1907,  1039,
     206,  1503,  1777,  1276,   343,   336,  1333,   583,  1917,   641,
     355,   865,    13,   857,   308,   513,   785,   517,   145,   772,
    1929,  1930,    69,   759,  1014,    72,  1755,  1650,  1883,   235,
     236,  1852,   238,     0,     0,   525,     0,  1006,   748,   245,
     246,   247,   248,   249,  1175,   251,  1288,    33,   254,   255,
     256,   257,   258,   100,   101,  1113,   262,   263,  1219,  1662,
     581,    87,   268,  1440,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   911,   912,   913,
      -1,    -1,   916,   917,   918,     7,   920,   921,   922,   923,
     924,   925,   926,   927,    -1,   929,    -1,   931,    -1,    -1,
     934,   935,   936,   937,   938,   939,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,   951,    -1,    -1,
     954,    43,    -1,    45,   171,    47,   173,    -1,    -1,    -1,
      52,    -1,   179,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,     6,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,  1005,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    47,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    59,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    73,
     206,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,   151,
      84,   278,    -1,   280,    -1,    -1,   158,   284,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,     6,   238,    -1,     9,    -1,    -1,    -1,    13,   245,
     246,   247,   248,   249,    -1,   251,   188,    22,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      35,    -1,   268,    -1,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    69,   237,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1161,    -1,    -1,
     252,   253,    -1,    -1,   188,    -1,    -1,    -1,    -1,  1173,
    1174,    -1,  1176,  1177,    -1,   100,   101,    -1,    -1,    -1,
    1184,    -1,   206,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,  1201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1209,  1210,  1211,    -1,    -1,
      -1,   235,   236,  1217,   238,  1219,  1220,  1221,  1222,  1223,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,   171,  1251,   173,    -1,
      -1,    -1,    -1,    -1,   179,    51,    42,    -1,    -1,    -1,
      -1,    -1,    58,  1267,   189,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,  1301,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,  1350,   272,   273,    -1,
      -1,    -1,   148,   278,    -1,   280,    -1,    -1,    -1,   284,
      74,    -1,    -1,    -1,    -1,    -1,    -1,  1371,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,  1380,    -1,    -1,    -1,
      -1,    -1,    -1,  1387,   180,  1372,  1373,    -1,   184,    -1,
       6,  1378,  1379,     9,  1381,  1382,    -1,    13,    -1,   195,
      -1,    -1,  1389,  1390,  1391,  1392,    22,  1394,  1395,  1396,
    1397,  1398,  1399,  1417,  1418,    -1,    -1,    -1,   214,    35,
     206,    -1,    18,    -1,    -1,    41,    42,    -1,    -1,    -1,
      26,    -1,    -1,  1437,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    69,    -1,    -1,    72,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    70,   189,   262,   263,    -1,    -1,
      -1,    -1,   268,    79,   100,   101,    -1,    -1,   274,    -1,
      -1,    87,   206,    -1,    -1,  1499,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,   122,    -1,   124,   125,
     126,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,  1581,  1582,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,  1598,    -1,    -1,    -1,    -1,  1603,
      -1,   217,    -1,    -1,    -1,    -1,    -1,  1611,    -1,  1613,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,     6,   268,    -1,     9,    -1,   272,   273,    13,    -1,
      -1,    -1,   278,    -1,   280,    -1,    -1,    22,   284,    -1,
      -1,    -1,    -1,   269,   270,   271,   272,   273,   274,   275,
      35,    -1,    -1,   279,    -1,    -1,    41,    42,    -1,    -1,
      -1,  1695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1703,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1730,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,     6,
      -1,  1755,     9,    -1,    -1,    -1,    13,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1803,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,
      -1,    71,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    33,    -1,    -1,   116,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   132,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,   148,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,   171,   165,   173,   272,   273,    -1,
      -1,    -1,   179,   278,    -1,   280,    -1,    -1,    -1,   284,
     180,    -1,   189,    -1,   184,    -1,    -1,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
       6,   268,    -1,     9,    -1,   272,   273,    13,    -1,    -1,
      -1,   278,    -1,   280,    -1,    -1,    22,   284,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    18,    -1,    -1,    -1,    41,    42,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    69,   251,    -1,    72,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,   100,   101,    -1,    -1,     6,    -1,
      87,     9,    -1,    -1,    -1,    13,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,   106,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    41,    42,   122,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    69,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,   171,    -1,   173,   272,   273,    -1,    -1,
      -1,   179,   278,    -1,   280,    -1,    98,    -1,   284,    -1,
      -1,   189,   269,   270,   271,   272,   273,   274,   275,    -1,
      -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,   173,    -1,    -1,   262,   263,    -1,    -1,    -1,     6,
     268,    -1,     9,    -1,   272,   273,    13,   189,    -1,    -1,
     278,    -1,   280,    -1,    -1,    22,   284,    -1,    -1,    -1,
     148,   203,   204,   205,    -1,   207,   208,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,    -1,    -1,    -1,   184,    -1,    -1,   241,
     242,   189,    69,    -1,    -1,    72,    -1,   195,   250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,
      -1,   273,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,   112,    -1,   235,   236,   116,
     238,    -1,    22,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    35,   254,   255,   256,   257,
     258,    41,    42,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   163,    -1,   262,   263,   167,    -1,    89,
      -1,   268,    -1,   173,    -1,   272,   273,    -1,    -1,   179,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,   189,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,   172,    -1,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,    -1,    13,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    22,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,   206,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    69,    -1,    -1,    72,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,   180,    -1,   268,    -1,
      -1,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,   206,    35,    -1,    -1,   124,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,
      72,   245,   246,   247,   248,   249,    -1,   251,    -1,   167,
     254,   255,   256,   257,   258,   173,    -1,    -1,   262,   263,
      -1,   179,    -1,    -1,   268,    -1,    -1,    -1,   100,   101,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,   203,   204,   205,   206,   207,
     208,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,   167,   254,   255,   256,   257,
     258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,
     268,    -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    13,   206,
     262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
      35,    -1,   284,    -1,    -1,    -1,    41,    42,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    60,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    69,   262,   263,    72,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,   100,   101,    -1,   189,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,   206,    35,    -1,    -1,   124,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      69,    -1,    -1,    72,   245,   246,   247,   248,   249,    -1,
     251,    -1,   167,   254,   255,   256,   257,   258,   173,    -1,
      -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,
      -1,   100,   101,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   203,   204,
     205,   206,   207,   208,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,   167,   254,
     255,   256,   257,   258,   173,    -1,    -1,   262,   263,    -1,
     179,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,
     189,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
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
      -1,    -1,    -1,   206,   278,    -1,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
     112,    -1,   235,   236,   116,   238,    -1,    22,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      35,   254,   255,   256,   257,   258,    41,    42,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,   163,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,    -1,   173,    -1,
     272,   273,   274,    -1,   179,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,
      42,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
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
     278,    -1,   280,    -1,    -1,   197,   284,    -1,    -1,    -1,
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
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,   235,   236,    35,   238,    -1,    -1,
      -1,    -1,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      69,    -1,    -1,    72,    -1,   276,    -1,    -1,   163,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
     204,    13,   206,   262,   263,    -1,    -1,    -1,    -1,   268,
      22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,
      42,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    69,   262,   263,
      72,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,   273,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    13,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    35,
      -1,   235,   236,    -1,   238,    41,    42,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    69,   268,    -1,    72,    -1,    -1,    -1,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     272,   273,    -1,   189,    -1,    -1,   278,   279,   280,    -1,
      -1,    -1,   284,    -1,   200,    -1,    -1,   203,   204,   205,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
     173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    13,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,
     273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,
      -1,   284,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     276,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    17,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    47,    -1,   284,    -1,    -1,    17,    -1,    54,
      -1,    -1,    -1,    58,    59,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    47,    -1,    84,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    58,    59,
      -1,    61,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    -1,    61,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,   163,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,   206,    -1,    -1,   163,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,   178,    -1,    -1,   262,   263,   264,
     265,   266,    -1,   268,   188,   235,   236,    -1,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,   206,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,
      -1,   235,   236,    -1,   238,    30,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    41,   251,    -1,    44,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    41,    -1,    -1,    44,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,   191,   238,    -1,    -1,    -1,    -1,   243,    -1,
     245,   246,   247,   248,   249,    -1,   251,   206,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    19,   262,   263,    -1,
      23,    -1,    -1,   268,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    37,    -1,   235,   236,    41,   238,
      -1,    44,    -1,    -1,   243,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,    -1,    -1,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   152,
     245,   246,   247,   248,   249,   250,   251,   252,    -1,   254,
     255,   256,   257,   258,    -1,    -1,   169,   262,   263,   264,
     265,   266,   267,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     243
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
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
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
{ lex.g_file->fil_length = (IPTR) yyvsp[0]; }
    break;
case 187:
{ lex.g_file  = make_file ();}
    break;
case 192:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 193:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
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
case 327:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 328:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 329:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 330:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 331:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 332:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 333:
{ yyval = NULL; }
    break;
case 334:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 335:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 336:
{ yyval = make_list (yyvsp[0]); }
    break;
case 337:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 338:
{ yyval = NULL; }
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
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 346:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 347:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 348:
{ yyval = NULL; }
    break;
case 349:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 350:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 351:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 352:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 353:
{ yyval = NULL; }
    break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 356:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 359:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 360:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 361:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 362:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 366:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 367:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 368:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 369:
{ yyval = NULL; }
    break;
case 370:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 371:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 372:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 373:
{ yyval = NULL; }
    break;
case 375:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 376:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 377:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 378:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 379:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 380:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 381:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 382:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 383:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 384:
{ yyval = make_list (yyvsp[0]); }
    break;
case 386:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 389:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 390:
{ yyval = yyvsp[-1]; }
    break;
case 391:
{ lex.beginning = lex_position(); }
    break;
case 392:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 393:
{ lex.beginning = lex.last_token; }
    break;
case 394:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 395:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 396:
{ yyval = 0; }
    break;
case 397:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 398:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 401:
{ yyval = NULL; }
    break;
case 402:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 403:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 404:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 406:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 407:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 408:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 409:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 410:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 411:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 412:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 413:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 414:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 415:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 416:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 417:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 418:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 419:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 420:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 421:
{ yyval = NULL; }
    break;
case 422:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 423:
{ yyval = yyvsp[0]; }
    break;
case 424:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 425:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 426:
{ yyval = yyvsp[0]; }
    break;
case 427:
{ yyval = yyvsp[0]; }
    break;
case 428:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 429:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 430:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 431:
{ yyval = yyvsp[0]; }
    break;
case 432:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 434:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 435:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 436:
{ yyval = yyvsp[0]; }
    break;
case 437:
{ yyval = yyvsp[0]; }
    break;
case 438:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 439:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 440:
{ yyval = yyvsp[0]; }
    break;
case 441:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 443:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 444:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 445:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 446:
{ yyval = yyvsp[0]; }
    break;
case 447:
{ yyval = yyvsp[0]; }
    break;
case 448:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 449:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 450:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 451:
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
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;
case 491:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 492:
{ yyval = yyvsp[0]; }
    break;
case 493:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 494:
{ yyval = yyvsp[0]; }
    break;
case 495:
{ yyval = yyvsp[0]; }
    break;
case 496:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 498:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 499:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 502:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 504:
{ yyval = NULL; }
    break;
case 506:
{ yyval = NULL; }
    break;
case 507:
{ yyval = yyvsp[0]; }
    break;
case 508:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 509:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 510:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 511:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 512:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 513:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 514:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 515:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 516:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 517:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 518:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 519:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 524:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 525:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 527:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 528:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 529:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 535:
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
case 536:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 538:
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
case 539:
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
case 540:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 547:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 548:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 549:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 550:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 551:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 552:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 557:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 568:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 569:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 570:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 571:
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
case 572:
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
case 575:
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
case 576:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 577:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 578:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 579:
{ yyval = yyvsp[-1]; }
    break;
case 580:
{ yyval = 0; }
    break;
case 584:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 585:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 586:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 590:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 591:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 592:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 593:
{ yyval = 0; }
    break;
case 594:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 597:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 598:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 601:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 602:
{ yyval = NULL; }
    break;
case 604:
{ yyval = NULL; }
    break;
case 605:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 607:
{ yyval = NULL; }
    break;
case 609:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 614:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 615:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 616:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 617:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 618:
{ yyval = yyvsp[0];}
    break;
case 620:
{ yyval = yyvsp[0];}
    break;
case 621:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 622:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 623:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 624:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 625:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 626:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 627:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 628:
{ yyval = 0; }
    break;
case 629:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 630:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 631:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 632:
{ yyval = (dsql_nod*) 0; }
    break;
case 633:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 634:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 636:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 637:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 638:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 639:
{ yyval = 0; }
    break;
case 641:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 642:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 643:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 644:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 645:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 646:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 647:
{ yyval = make_list (yyvsp[0]); }
    break;
case 648:
{ yyval = 0; }
    break;
case 650:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 651:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 652:
{ yyval = 0; }
    break;
case 653:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 654:
{ yyval = 0; }
    break;
case 655:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 656:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 657:
{ yyval = yyvsp[-1]; }
    break;
case 658:
{ yyval = 0; }
    break;
case 659:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 660:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 661:
{ yyval = NULL; }
    break;
case 662:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 663:
{ yyval = 0; }
    break;
case 664:
{ yyval = yyvsp[0]; }
    break;
case 665:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 666:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 667:
{ yyval = 0; }
    break;
case 668:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 669:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 670:
{ lex.limit_clause = true; }
    break;
case 671:
{ lex.limit_clause = false; }
    break;
case 672:
{ lex.first_detection = true; }
    break;
case 673:
{ lex.first_detection = false; }
    break;
case 674:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 675:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 676:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 677:
{ yyval = 0; }
    break;
case 678:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 679:
{ yyval = yyvsp[-2]; }
    break;
case 680:
{ yyval = yyvsp[-1]; }
    break;
case 681:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 682:
{ yyval = yyvsp[-1]; }
    break;
case 683:
{ yyval = yyvsp[0]; }
    break;
case 684:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 685:
{ yyval = 0; }
    break;
case 686:
{ yyval = make_list (yyvsp[0]); }
    break;
case 687:
{ yyval = 0; }
    break;
case 689:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 691:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 694:
{ yyval = make_list (yyvsp[0]); }
    break;
case 696:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 700:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 702:
{ yyval = NULL; }
    break;
case 703:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 704:
{ yyval = NULL; }
    break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 707:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 708:
{ yyval = yyvsp[-1]; }
    break;
case 709:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 710:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 711:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 712:
{ yyval = NULL; }
    break;
case 714:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 715:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 716:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 717:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 718:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 719:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 720:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 721:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 722:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 723:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 724:
{ yyval = make_list (yyvsp[0]); }
    break;
case 725:
{ yyval = 0; }
    break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 729:
{ yyval = yyvsp[0]; }
    break;
case 730:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 736:
{ yyval = 0; }
    break;
case 737:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 738:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 739:
{ yyval = 0; }
    break;
case 740:
{ yyval = 0; }
    break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 743:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 746:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 747:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 748:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 749:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 752:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 753:
{ yyval = 0; }
    break;
case 754:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 755:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 758:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 759:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 763:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 764:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 771:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 774:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 775:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 779:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 780:
{ yyval = yyvsp[0]; }
    break;
case 782:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 785:
{ yyval = NULL; }
    break;
case 786:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 788:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = NULL; }
    break;
case 791:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 793:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 797:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 807:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 812:
{ yyval = yyvsp[-1]; }
    break;
case 813:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 825:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 827:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 837:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 840:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 841:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 842:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 843:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 844:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 845:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 846:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 847:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 850:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 852:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 854:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 856:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 858:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 859:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 860:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 861:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 862:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 863:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 864:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 865:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 866:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 867:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 868:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 869:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 870:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
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
case 875:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 876:
{ yyval = yyvsp[-1]; }
    break;
case 877:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 878:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 888:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 889:
{ yyval = yyvsp[0]; }
    break;
case 890:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 891:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 892:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 893:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 894:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 895:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 896:
{ yyval = yyvsp[-1]; }
    break;
case 897:
{ yyval = yyvsp[-1]; }
    break;
case 901:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 902:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 903:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 904:
{ yyval = yyvsp[0]; }
    break;
case 905:
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
case 906:
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
case 907:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 908:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 910:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 912:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 913:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 914:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 917:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 919:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 920:
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
case 921:
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
case 922:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 923:
{ yyval = make_parameter (); }
    break;
case 924:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 925:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 926:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 927:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 928:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 929:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 930:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 931:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 932:
{ yyval = yyvsp[0]; }
    break;
case 933:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 935:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 936:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 937:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 938:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 939:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 941:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 942:
{ yyval = yyvsp[0];}
    break;
case 947:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 948:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 949:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 950:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 951:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 952:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 953:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 954:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 955:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 956:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 957:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 958:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 960:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 962:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 963:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 964:
{ yyval = yyvsp[0]; }
    break;
case 965:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 966:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 967:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 968:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 971:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 972:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 975:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 976:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 977:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 978:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 979:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 980:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 981:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 982:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 989:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 990:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 991:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 992:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 993:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 996:
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

