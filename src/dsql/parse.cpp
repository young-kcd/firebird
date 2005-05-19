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
# define	COMMENT	521
# define	COLLATION	522


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



#define	YYFINAL		1959
#define	YYFLAG		-32768
#define	YYNTBASE	285

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 522 ? yytranslate[x] : 732)

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
    1286,  1289,  1292,  1296,  1300,  1303,  1306,  1310,  1317,  1319,
    1322,  1327,  1331,  1334,  1337,  1340,  1343,  1347,  1349,  1353,
    1357,  1361,  1364,  1367,  1372,  1377,  1383,  1385,  1387,  1389,
    1391,  1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,
    1411,  1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,
    1431,  1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,  1449,
    1451,  1454,  1457,  1460,  1462,  1464,  1466,  1467,  1470,  1473,
    1478,  1483,  1489,  1490,  1492,  1495,  1498,  1503,  1507,  1510,
    1513,  1521,  1523,  1524,  1526,  1527,  1530,  1533,  1536,  1539,
    1542,  1545,  1548,  1551,  1554,  1558,  1561,  1564,  1567,  1570,
    1572,  1574,  1576,  1578,  1583,  1589,  1591,  1595,  1597,  1601,
    1603,  1606,  1608,  1610,  1612,  1614,  1616,  1618,  1620,  1622,
    1624,  1626,  1628,  1633,  1638,  1645,  1651,  1655,  1656,  1659,
    1662,  1663,  1667,  1668,  1673,  1675,  1681,  1686,  1688,  1693,
    1695,  1698,  1701,  1703,  1705,  1707,  1710,  1713,  1716,  1719,
    1720,  1724,  1730,  1732,  1734,  1737,  1741,  1743,  1746,  1750,
    1751,  1753,  1755,  1757,  1763,  1769,  1776,  1778,  1780,  1782,
    1785,  1790,  1792,  1793,  1799,  1801,  1802,  1806,  1809,  1811,
    1812,  1815,  1816,  1818,  1819,  1823,  1825,  1826,  1828,  1831,
    1833,  1835,  1837,  1839,  1842,  1845,  1847,  1850,  1854,  1856,
    1858,  1862,  1866,  1868,  1871,  1875,  1877,  1880,  1881,  1884,
    1886,  1888,  1889,  1891,  1893,  1895,  1899,  1902,  1906,  1907,
    1909,  1913,  1918,  1924,  1931,  1939,  1941,  1943,  1945,  1947,
    1949,  1951,  1954,  1956,  1958,  1960,  1962,  1964,  1966,  1969,
    1971,  1973,  1975,  1978,  1980,  1982,  1986,  1990,  1991,  1994,
    1995,  1998,  1999,  2003,  2007,  2011,  2013,  2018,  2023,  2025,
    2035,  2036,  2037,  2038,  2039,  2043,  2046,  2048,  2049,  2053,
    2059,  2063,  2066,  2072,  2075,  2077,  2079,  2081,  2083,  2085,
    2089,  2091,  2095,  2097,  2098,  2101,  2103,  2107,  2109,  2111,
    2113,  2115,  2119,  2126,  2128,  2129,  2133,  2134,  2136,  2140,
    2142,  2144,  2146,  2151,  2157,  2163,  2165,  2167,  2170,  2175,
    2180,  2183,  2187,  2188,  2190,  2193,  2195,  2197,  2200,  2203,
    2206,  2207,  2209,  2210,  2214,  2215,  2217,  2221,  2223,  2226,
    2227,  2230,  2231,  2234,  2235,  2240,  2242,  2245,  2247,  2249,
    2250,  2252,  2256,  2259,  2261,  2263,  2266,  2268,  2273,  2277,
    2279,  2283,  2288,  2289,  2293,  2294,  2296,  2300,  2304,  2306,
    2308,  2309,  2311,  2313,  2318,  2319,  2322,  2327,  2328,  2337,
    2343,  2345,  2347,  2355,  2360,  2362,  2364,  2373,  2379,  2384,
    2386,  2390,  2394,  2396,  2404,  2412,  2418,  2422,  2423,  2425,
    2427,  2429,  2432,  2433,  2435,  2437,  2439,  2440,  2444,  2446,
    2450,  2452,  2453,  2457,  2459,  2463,  2465,  2469,  2473,  2475,
    2477,  2481,  2483,  2486,  2488,  2492,  2496,  2498,  2501,  2505,
    2509,  2513,  2517,  2519,  2523,  2526,  2528,  2530,  2532,  2534,
    2536,  2538,  2540,  2542,  2544,  2546,  2548,  2552,  2556,  2560,
    2564,  2568,  2572,  2576,  2580,  2587,  2594,  2601,  2608,  2615,
    2622,  2629,  2636,  2643,  2650,  2657,  2664,  2671,  2678,  2685,
    2692,  2694,  2696,  2702,  2709,  2715,  2722,  2726,  2731,  2737,
    2744,  2748,  2753,  2757,  2762,  2766,  2771,  2776,  2782,  2787,
    2792,  2796,  2801,  2803,  2805,  2807,  2809,  2811,  2813,  2815,
    2819,  2823,  2825,  2827,  2829,  2831,  2833,  2835,  2837,  2839,
    2841,  2843,  2846,  2849,  2853,  2857,  2861,  2865,  2869,  2873,
    2877,  2881,  2883,  2885,  2887,  2889,  2893,  2895,  2897,  2899,
    2901,  2903,  2905,  2910,  2912,  2916,  2918,  2921,  2923,  2925,
    2927,  2929,  2931,  2933,  2935,  2938,  2941,  2944,  2946,  2948,
    2950,  2952,  2954,  2956,  2958,  2960,  2962,  2964,  2967,  2969,
    2972,  2974,  2976,  2978,  2980,  2982,  2985,  2987,  2989,  2991,
    2993,  2998,  3004,  3010,  3016,  3022,  3028,  3034,  3040,  3046,
    3052,  3058,  3060,  3067,  3069,  3074,  3082,  3085,  3086,  3091,
    3095,  3102,  3104,  3106,  3113,  3122,  3129,  3131,  3133,  3138,
    3145,  3150,  3156,  3160,  3166,  3171,  3177,  3179,  3181,  3183,
    3188,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3214,  3216,  3217,  3219,  3221,  3223,  3225,  3227,  3229,
    3231,  3233,  3235,  3237,  3239,  3241,  3243,  3245,  3247,  3249,
    3251,  3253,  3255,  3257,  3259,  3261,  3263,  3265,  3267,  3269,
    3271,  3273,  3275,  3277,  3279,  3281,  3283,  3285,  3287,  3289,
    3291,  3293,  3295,  3297,  3299,  3301,  3303,  3305,  3307,  3309,
    3311,  3313,  3315,  3317,  3319,  3321,  3323,  3325,  3327,  3329,
    3331
};
static const short yyrhs[] =
{
     286,     0,   286,   276,     0,   469,     0,   627,     0,   546,
       0,   523,     0,   321,     0,   308,     0,   617,     0,   491,
       0,   287,     0,   616,     0,   447,     0,   450,     0,   323,
       0,   325,     0,   296,     0,   524,     0,   517,     0,   552,
       0,   515,     0,   620,     0,    40,   674,     0,    70,   289,
     122,   288,   592,   174,   299,   293,     0,    70,   291,   122,
     141,   295,   174,   299,   293,     0,    70,   304,   174,   306,
     294,     0,   172,     0,     0,     6,     0,     6,   140,     0,
     290,     0,   292,     0,   290,   277,   292,     0,    56,     0,
     155,     0,    80,     0,    44,     0,   180,   633,     0,   145,
     633,     0,   192,    70,   124,     0,     0,   192,   202,   124,
       0,     0,   721,     0,   150,   297,   289,   122,   288,   592,
      63,   299,     0,   150,   297,   291,   122,   141,   295,    63,
     299,     0,   150,   298,   304,    63,   306,     0,    70,   124,
      61,     0,     0,   202,   124,    61,     0,     0,   300,     0,
     302,     0,   301,     0,   300,   277,   301,     0,   300,   277,
     303,     0,   302,   277,   301,     0,   141,   721,     0,   176,
     725,     0,   187,   728,     0,   206,   722,     0,   303,     0,
     302,   277,   303,     0,   726,     0,   181,   726,     0,    71,
     726,     0,   305,     0,   304,   277,   305,     0,   722,     0,
     307,     0,   306,   277,   307,     0,   726,     0,   181,   726,
       0,    42,   309,     0,    60,   319,     0,    59,    65,   310,
       0,   705,   312,   149,   316,    52,   673,   111,   673,     0,
     498,     0,    18,     0,    34,   278,   677,   279,   504,     0,
       0,   313,     0,   278,   313,   279,     0,   314,     0,   313,
     277,   314,     0,   379,   311,   315,     0,     0,    19,   230,
       0,    19,   262,     0,    94,     0,   317,     0,   278,   317,
     279,     0,   379,   311,   318,     0,   133,   677,     0,     0,
      19,    97,     0,    19,   230,     0,   204,     0,    19,   230,
     204,     0,   714,    78,   320,   128,   320,    52,   673,   111,
     673,     0,   706,     0,   674,     0,    33,   322,     0,    55,
     327,     0,   331,   612,    79,   717,   122,   592,   332,     0,
     141,   398,     0,   172,   366,     0,   176,   461,     0,   187,
     454,     0,    68,   348,     0,   265,   348,     0,    37,   350,
       0,    48,   339,     0,   157,   333,     0,   206,   349,     0,
     229,   324,     0,   141,   399,     0,   172,   367,     0,   187,
     455,     0,    55,   328,     0,    33,   125,     7,   326,     0,
     141,   400,     0,   176,   462,     0,    55,   329,     0,   713,
     673,     0,   713,   673,     0,   713,   673,     0,   713,   673,
       0,   179,     0,     0,   635,     0,   634,     0,   373,   278,
     458,   662,   459,   279,     0,   677,   334,   335,   673,   336,
     337,     0,   105,     0,    14,     0,     0,     0,    29,     0,
       0,    98,   351,   680,   365,     0,     0,   338,     0,   359,
       0,   338,   359,     0,   376,   340,   493,   458,   341,   459,
     342,   375,     0,    10,     0,     0,    43,   458,   381,     0,
       0,     0,   343,     0,   344,     0,   343,   344,     0,   345,
       0,   346,     0,   347,     0,   118,    94,     0,   458,    22,
     278,   642,   279,   459,     0,   716,     0,   722,     0,   352,
     353,   356,     0,     0,   269,     0,   673,     0,     0,   354,
       0,   355,     0,   354,   355,     0,   132,   351,   677,     0,
      98,   351,   680,   365,     0,   181,   673,     0,   134,   673,
       0,   156,   112,   673,     0,     0,   357,     0,   358,     0,
     357,   358,     0,   359,     0,    43,    21,   156,   707,     0,
     255,    90,   673,     0,   360,   673,   361,     0,    90,     0,
       0,   362,     0,   363,     0,   362,   363,     0,   167,   364,
     680,     0,    98,   351,   680,   365,     0,     0,    12,     0,
      12,   130,     0,     0,   130,     0,   131,     0,   592,   368,
     278,   369,   279,     0,   592,   368,   278,   369,   279,     0,
      59,    90,   673,     0,    59,   673,     0,     0,   370,     0,
     369,   277,   370,     0,   371,     0,   386,     0,   376,   374,
     341,   459,   382,   375,     0,   376,   494,   372,     0,   376,
     372,     0,   373,   278,   458,   662,   459,   279,     0,    27,
      19,     0,    27,     0,   493,   458,     0,   640,   456,     0,
      23,   708,     0,     0,   640,     0,   640,     0,   379,   493,
       0,     0,    43,   381,     0,     0,   666,     0,   670,     0,
     671,     0,   672,     0,   704,     0,   663,     0,     0,   383,
       0,   384,     0,   383,   384,     0,   387,   385,     0,   118,
      94,     0,   145,   592,   633,   394,   392,     0,   393,     0,
     179,   392,     0,   139,    86,   392,     0,   387,   388,     0,
      30,   710,     0,     0,   389,     0,   390,     0,   391,     0,
     393,     0,   179,   634,   392,     0,   139,    86,   634,   392,
       0,    62,    86,   634,   145,   592,   633,   394,   392,     0,
     239,   612,    79,   717,     0,     0,   458,    22,   278,   642,
     279,   459,     0,   395,     0,   396,     0,   396,   395,     0,
     395,   396,     0,     0,   122,   180,   397,     0,   122,    44,
     397,     0,   203,     0,   156,    43,     0,   156,    94,     0,
     117,   201,     0,   721,   402,   403,    10,   456,   409,   418,
     459,     0,   721,   402,   403,    10,   456,   409,   418,   459,
       0,   721,   402,   403,    10,   456,   409,   418,   459,     0,
     721,   402,   403,    10,   456,   409,   418,   459,     0,   278,
     404,   279,     0,     0,   149,   278,   406,   279,     0,     0,
     405,     0,   404,   277,   405,     0,   377,   494,   408,   459,
       0,   407,     0,   406,   277,   407,     0,   377,   494,     0,
      43,   456,   381,     0,   269,   456,   381,     0,   456,     0,
     410,     0,     0,   411,     0,   410,   411,     0,    42,   414,
     412,   276,     0,   413,     0,   416,     0,   376,   494,   415,
       0,   184,     0,     0,   269,   381,     0,   380,     0,   711,
      36,    61,   278,   552,   279,     0,   421,     0,   418,     0,
      16,   419,    51,     0,   420,     0,   420,   438,     0,     0,
     417,     0,   420,   417,     0,   625,   276,     0,   616,   276,
       0,   620,   276,     0,   617,   276,     0,   431,   276,     0,
     447,   276,     0,   424,   276,     0,   427,   276,     0,   626,
     276,     0,   422,   276,     0,   423,   276,     0,   429,   276,
       0,   442,   276,     0,   436,   276,     0,   170,   276,     0,
      58,   276,     0,   428,     0,   434,     0,   425,     0,   426,
       0,    55,   713,     0,    55,   713,   662,     0,    55,     0,
      56,   246,   662,     0,   435,    61,   552,    82,   433,   437,
      47,   417,     0,   435,    61,    56,   246,   662,    82,   433,
      47,   417,     0,    56,   246,   662,    82,   433,     0,    74,
     278,   642,   279,   173,   417,    50,   417,     0,    74,   278,
     642,   279,   173,   417,     0,   137,   662,   430,     0,     0,
     552,    82,   433,     0,   280,   727,     0,   432,     0,   639,
       0,   433,   277,   639,     0,   433,   277,   432,     0,   435,
     191,   278,   642,   279,    47,   417,     0,   719,   280,     0,
       0,   227,     0,   247,     0,   247,   719,     0,    10,    36,
     711,     0,     0,   439,     0,   438,   439,     0,   189,   440,
      47,   417,     0,   441,     0,   440,   277,   441,     0,   165,
     674,     0,    66,   715,     0,    55,   713,     0,     9,     0,
     443,     0,   445,     0,   444,     0,   256,   711,     0,   257,
     711,     0,   258,   446,   711,    82,   433,     0,     0,    56,
     141,   721,   448,   449,     0,   665,     0,   278,   665,   279,
       0,     0,   148,   433,     0,   148,   278,   433,   279,     0,
       0,    56,   260,   451,   403,    10,   409,   418,     0,   278,
     452,   279,     0,     0,   453,     0,   452,   277,   453,     0,
     407,   269,   669,     0,   728,   633,    10,   456,   556,   460,
     457,     0,   728,   633,    10,   456,   556,   460,   457,     0,
       0,     0,     0,     0,   192,    22,   124,     0,     0,   725,
      61,   592,   463,   464,   467,   458,   468,   459,     0,   725,
      61,   592,   463,   464,   467,   458,   468,   459,     0,     3,
       0,    76,     0,     0,   465,   466,     0,    15,     0,     5,
       0,    80,     0,   180,     0,    44,     0,    80,   125,   180,
       0,    80,   125,    44,     0,   180,   125,    80,     0,   180,
     125,    44,     0,    44,   125,    80,     0,    44,   125,   180,
       0,    80,   125,   180,   125,    44,     0,    80,   125,    44,
     125,   180,     0,   180,   125,    80,   125,    44,     0,   180,
     125,    44,   125,    80,     0,    44,   125,    80,   125,   180,
       0,    44,   125,   180,   125,    80,     0,   136,   675,     0,
       0,    10,   458,   409,   418,     0,     7,   470,     0,    55,
     330,     0,   172,   592,   475,     0,   176,   488,     0,   141,
     401,     0,    37,   485,   486,     0,    48,   477,   473,     0,
      79,   483,     0,   265,   484,     0,    43,   458,   381,     0,
      22,   458,   278,   642,   279,   459,     0,   474,     0,   473,
     474,     0,   156,   458,   471,   459,     0,     4,    30,   472,
       0,     4,   347,     0,    49,    43,     0,    49,    30,     0,
     174,   640,     0,   208,   379,   494,     0,   476,     0,   475,
     277,   476,     0,    49,   640,   482,     0,    49,    30,   710,
       0,     4,   371,     0,     4,   386,     0,   479,   640,   136,
     677,     0,   479,   477,   174,   640,     0,   479,   481,   208,
     480,   459,     0,   478,     0,   730,     0,   202,     0,   207,
       0,   209,     0,   210,     0,   211,     0,   212,     0,   213,
       0,   214,     0,   215,     0,   218,     0,   219,     0,   220,
       0,   221,     0,   222,     0,   225,     0,   226,     0,   229,
       0,   233,     0,   234,     0,   235,     0,   236,     0,   245,
       0,   242,     0,   244,     0,   256,     0,   257,     0,   258,
       0,   259,     0,   239,     0,   263,     0,   267,     0,     7,
       0,     7,   207,     0,   494,   458,     0,   640,   456,     0,
     640,     0,   205,     0,   203,     0,     0,   717,     3,     0,
     717,    76,     0,   716,   266,   192,   679,     0,   716,   266,
     192,   223,     0,   716,   266,   192,   273,   223,     0,     0,
     487,     0,   486,   487,     0,     4,   338,     0,     4,   255,
      90,   673,     0,    49,   255,    90,     0,    16,   254,     0,
      51,   254,     0,   725,   463,   489,   467,   458,   490,   459,
       0,   464,     0,     0,   468,     0,     0,    49,   492,     0,
      55,   713,     0,    79,   717,     0,   141,   721,     0,   172,
     724,     0,   176,   725,     0,   187,   728,     0,    60,   714,
       0,    48,   712,     0,    59,    65,   705,     0,   157,   677,
       0,   206,   722,     0,    68,   716,     0,   265,   716,     0,
     494,     0,   495,     0,   498,     0,   501,     0,   499,   281,
     496,   282,     0,   506,   281,   496,   282,   504,     0,   497,
       0,   496,   277,   497,     0,   679,     0,   679,   280,   679,
       0,   499,     0,   506,   504,     0,   505,     0,   510,     0,
     513,     0,   235,     0,   500,     0,   161,     0,    38,     0,
     218,     0,   219,     0,    81,     0,    92,     0,    18,   503,
     502,   504,     0,    18,   278,   678,   279,     0,    18,   278,
     678,   277,   674,   279,     0,    18,   278,   277,   674,   279,
       0,   154,   160,   678,     0,     0,   169,   674,     0,   169,
     706,     0,     0,    21,   156,   707,     0,     0,   509,   278,
     677,   279,     0,   509,     0,   509,   185,   278,   677,   279,
       0,   508,   278,   677,   279,     0,   508,     0,   507,   278,
     677,   279,     0,   183,     0,    21,   185,     0,    87,   185,
       0,    21,     0,    87,     0,   115,     0,   113,    21,     0,
     113,    87,     0,    95,   511,     0,   512,   511,     0,     0,
     278,   679,   279,     0,   278,   679,   277,   679,   279,     0,
      41,     0,    88,     0,    91,   514,     0,    93,    91,   514,
       0,   144,     0,    89,   138,     0,   278,   675,   279,     0,
       0,   528,     0,   516,     0,   545,     0,   156,    68,   716,
     174,   679,     0,   156,    68,   716,   174,   223,     0,   156,
      68,   716,   174,   273,   223,     0,   518,     0,   519,     0,
     521,     0,   244,   729,     0,   245,   244,   729,   520,     0,
     123,     0,     0,   153,   525,   174,   522,   729,     0,   244,
       0,     0,    25,   525,   526,     0,   153,   525,     0,   193,
       0,     0,   147,   527,     0,     0,   162,     0,     0,   156,
     175,   529,     0,   530,     0,     0,   531,     0,   530,   531,
       0,   532,     0,   533,     0,   534,     0,   538,     0,   143,
     123,     0,   143,   194,     0,   188,     0,   117,   188,     0,
      84,   102,   535,     0,   535,     0,   536,     0,   143,   177,
     537,     0,   143,    26,   537,     0,   162,     0,   162,   172,
       0,   162,   172,   166,     0,   186,     0,   117,   186,     0,
       0,   146,   541,     0,   158,     0,   142,     0,     0,   143,
       0,   194,     0,   542,     0,   541,   277,   542,     0,   544,
     543,     0,    61,   539,   540,     0,     0,   592,     0,   544,
     277,   592,     0,   156,   168,    79,   717,     0,   267,   122,
     547,    83,   551,     0,   267,   122,   548,   720,    83,   551,
       0,   267,   122,   549,   720,   550,    83,   551,     0,    37,
       0,    48,     0,   172,     0,   187,     0,   141,     0,   176,
       0,    59,    65,     0,    60,     0,    55,     0,    68,     0,
     265,     0,    79,     0,   206,     0,    21,   156,     0,   268,
       0,   207,     0,   133,     0,   283,   720,     0,   673,     0,
      94,     0,   556,   553,   555,     0,    61,   180,   554,     0,
       0,   121,   635,     0,     0,   192,   243,     0,     0,   559,
     609,   615,     0,   559,   609,   615,     0,   559,   609,   615,
       0,   560,     0,   559,   178,   703,   560,     0,   559,   178,
       6,   560,     0,   561,     0,   155,   566,   569,   570,   574,
     599,   595,   598,   600,     0,     0,     0,     0,     0,   567,
     568,   563,     0,   567,   563,     0,   568,     0,     0,   231,
     680,   562,     0,   231,   278,   662,   279,   562,     0,   231,
     669,   562,     0,   232,   680,     0,   232,   278,   563,   662,
     279,     0,   232,   669,     0,    46,     0,   702,     0,   571,
       0,   274,     0,   572,     0,   571,   277,   572,     0,   662,
       0,   662,   573,   718,     0,    10,     0,     0,    63,   575,
       0,   576,     0,   575,   277,   576,     0,   582,     0,   577,
       0,   589,     0,   578,     0,   278,   582,   279,     0,   278,
     556,   279,   573,   579,   580,     0,   723,     0,     0,   278,
     581,   279,     0,     0,   718,     0,   581,   277,   718,     0,
     583,     0,   584,     0,   585,     0,   576,   263,    85,   577,
       0,   576,   114,   593,    85,   577,     0,   576,   593,    85,
     576,   586,     0,   587,     0,   588,     0,   122,   642,     0,
     239,   278,   635,   279,     0,   721,   590,   573,   723,     0,
     721,   590,     0,   278,   665,   279,     0,     0,   592,     0,
     724,   723,     0,   724,     0,    77,     0,   100,   594,     0,
     151,   594,     0,    64,   594,     0,     0,   127,     0,     0,
      71,    19,   596,     0,     0,   597,     0,   596,   277,   597,
       0,   662,     0,    73,   642,     0,     0,   190,   642,     0,
       0,   135,   601,     0,     0,   602,   278,   603,   279,     0,
      85,     0,   164,   108,     0,   108,     0,   164,     0,     0,
     604,     0,   604,   277,   603,     0,   605,   606,     0,   601,
       0,   724,     0,   724,   605,     0,   114,     0,    79,   278,
     607,   279,     0,   126,   717,   608,     0,   717,     0,   717,
     277,   607,     0,    79,   278,   607,   279,     0,     0,   126,
      19,   610,     0,     0,   611,     0,   610,   277,   611,     0,
     662,   612,   614,     0,    11,     0,    45,     0,     0,   231,
       0,   241,     0,   240,   564,   613,   565,     0,     0,   259,
     662,     0,   259,   662,   174,   662,     0,     0,    80,    82,
     592,   636,   182,   278,   665,   279,     0,    80,    82,   592,
     636,   556,     0,   618,     0,   619,     0,    44,    63,   591,
     599,   600,   609,   615,     0,    44,    63,   591,   623,     0,
     621,     0,   622,     0,   180,   591,   156,   624,   599,   600,
     609,   615,     0,   180,   591,   156,   624,   623,     0,   190,
      35,   121,   711,     0,   625,     0,   624,   277,   625,     0,
     641,   269,   662,     0,   688,     0,   143,    18,   640,    63,
     592,   628,   631,     0,    80,    18,   640,    82,   592,   628,
     631,     0,    60,    63,   629,   174,   629,     0,    60,   174,
     629,     0,     0,   630,     0,   669,     0,   674,     0,   107,
     632,     0,     0,   678,     0,   669,     0,   634,     0,     0,
     278,   635,   279,     0,   640,     0,   635,   277,   640,     0,
     637,     0,     0,   278,   638,   279,     0,   641,     0,   638,
     277,   641,     0,   640,     0,   723,   283,   709,     0,   723,
     283,   274,     0,   709,     0,   640,     0,   723,   283,   709,
       0,   658,     0,   118,   658,     0,   644,     0,   642,   125,
     642,     0,   642,     8,   642,     0,   644,     0,   118,   658,
       0,   643,   125,   642,     0,   643,     8,   642,     0,   659,
     125,   642,     0,   659,     8,   642,     0,   645,     0,   278,
     643,   279,     0,   118,   644,     0,   646,     0,   649,     0,
     650,     0,   651,     0,   652,     0,   657,     0,   647,     0,
     655,     0,   653,     0,   654,     0,   656,     0,   662,   269,
     662,     0,   662,   270,   662,     0,   662,   271,   662,     0,
     662,    67,   662,     0,   662,   101,   662,     0,   662,   119,
     662,     0,   662,   120,   662,     0,   662,   116,   662,     0,
     662,   269,     6,   278,   557,   279,     0,   662,   270,     6,
     278,   557,   279,     0,   662,   271,     6,   278,   557,   279,
       0,   662,    67,     6,   278,   557,   279,     0,   662,   101,
       6,   278,   557,   279,     0,   662,   119,     6,   278,   557,
     279,     0,   662,   120,     6,   278,   557,   279,     0,   662,
     116,     6,   278,   557,   279,     0,   662,   269,   648,   278,
     557,   279,     0,   662,   270,   648,   278,   557,   279,     0,
     662,   271,   648,   278,   557,   279,     0,   662,    67,   648,
     278,   557,   279,     0,   662,   101,   648,   278,   557,   279,
       0,   662,   119,   648,   278,   557,   279,     0,   662,   120,
     648,   278,   557,   279,     0,   662,   116,   648,   278,   557,
     279,     0,   163,     0,     9,     0,   662,    83,    46,    63,
     662,     0,   662,    83,   118,    46,    63,   662,     0,   662,
      17,   662,     8,   662,     0,   662,   118,    17,   662,     8,
     662,     0,   662,   103,   662,     0,   662,   118,   103,   662,
       0,   662,   103,   662,    54,   662,     0,   662,   118,   103,
     662,    54,   662,     0,   662,    75,   660,     0,   662,   118,
      75,   660,     0,   662,    31,   662,     0,   662,   118,    31,
     662,     0,   662,   167,   662,     0,   662,   118,   167,   662,
       0,   662,   167,   192,   662,     0,   662,   118,   167,   192,
     662,     0,    57,   278,   556,   279,     0,   159,   278,   556,
     279,     0,   662,    83,    94,     0,   662,    83,   118,    94,
       0,   248,     0,   249,     0,   250,     0,   251,     0,   252,
       0,   253,     0,   661,     0,   278,   665,   279,     0,   278,
     557,   279,     0,   639,     0,   664,     0,   681,     0,   668,
       0,   669,     0,   432,     0,   689,     0,   690,     0,   700,
       0,   688,     0,   273,   662,     0,   272,   662,     0,   662,
     272,   662,     0,   662,    28,   662,     0,   662,    23,   708,
       0,   662,   273,   662,     0,   662,   274,   662,     0,   662,
     275,   662,     0,   278,   662,   279,     0,   278,   558,   279,
       0,   670,     0,   671,     0,   672,     0,    39,     0,   723,
     283,    39,     0,    97,     0,   663,     0,   704,     0,   220,
       0,   221,     0,   222,     0,   639,   281,   665,   282,     0,
     662,     0,   665,   277,   662,     0,   668,     0,   273,   667,
       0,   197,     0,   196,     0,   195,     0,   223,     0,   224,
       0,   667,     0,   673,     0,    38,   199,     0,   218,   199,
       0,   219,   199,     0,   284,     0,   181,     0,   225,     0,
     226,     0,   233,     0,   234,     0,    66,     0,   165,     0,
     242,     0,   199,     0,   200,   199,     0,   675,     0,   273,
     676,     0,   196,     0,   196,     0,   675,     0,   196,     0,
     680,     0,   273,   680,     0,   196,     0,   682,     0,   683,
       0,   685,     0,    32,   278,   274,   279,     0,    32,   278,
     702,   662,   279,     0,    32,   278,    46,   662,   279,     0,
     171,   278,   702,   662,   279,     0,   171,   278,    46,   662,
     279,     0,    13,   278,   702,   662,   279,     0,    13,   278,
      46,   662,   279,     0,   110,   278,   702,   662,   279,     0,
     110,   278,    46,   662,   279,     0,   106,   278,   702,   662,
     279,     0,   106,   278,    46,   662,   279,     0,   684,     0,
     209,   278,   701,    63,   662,   279,     0,   686,     0,    96,
     278,   662,   279,     0,   228,   278,   662,    63,   662,   687,
     279,     0,    61,   662,     0,     0,   705,   278,   665,   279,
       0,   705,   278,   279,     0,    20,   278,   662,    10,   378,
     279,     0,   691,     0,   692,     0,   237,   278,   662,   277,
     662,   279,     0,   261,   278,   642,   277,   662,   277,   662,
     279,     0,   238,   278,   662,   277,   665,   279,     0,   693,
       0,   695,     0,   236,   698,   694,    51,     0,   236,   698,
     694,    50,   699,    51,     0,   189,   697,   173,   699,     0,
     694,   189,   697,   173,   699,     0,   236,   696,    51,     0,
     236,   696,    50,   699,    51,     0,   189,   642,   173,   699,
       0,   696,   189,   642,   173,   699,     0,   662,     0,   662,
       0,   662,     0,   264,    97,    61,   716,     0,    69,   278,
     716,   277,   662,   279,     0,   210,     0,   211,     0,   212,
       0,   213,     0,   214,     0,   215,     0,   216,     0,   217,
       0,     6,     0,     0,    46,     0,     0,    94,     0,   198,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   198,
       0,   731,     0,   201,     0,   203,     0,   204,     0,   205,
       0,   206,     0,   208,     0,   227,     0,   230,     0,   228,
       0,   238,     0,   241,     0,   247,     0,   243,     0,   237,
       0,   240,     0,   246,     0,   248,     0,   249,     0,   250,
       0,   260,     0,   254,     0,   255,     0,   261,     0,   262,
       0,   216,     0,   217,     0,   265,     0,   264,     0,   266,
       0,   268,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   535,   537,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   569,   573,   577,   582,   583,   586,
     588,   590,   594,   595,   599,   603,   605,   607,   609,   611,
     615,   617,   621,   623,   627,   634,   638,   642,   647,   649,
     653,   655,   659,   660,   663,   664,   666,   668,   672,   674,
     676,   678,   682,   683,   690,   692,   694,   698,   699,   703,
     707,   708,   712,   714,   721,   725,   727,   732,   738,   739,
     741,   747,   749,   750,   754,   755,   761,   766,   768,   770,
     772,   776,   777,   781,   784,   789,   791,   793,   795,   798,
     803,   809,   811,   818,   822,   824,   827,   829,   831,   833,
     835,   837,   839,   841,   843,   845,   850,   854,   856,   858,
     866,   870,   874,   876,   882,   889,   894,   899,   904,   912,
     914,   918,   920,   921,   927,   933,   935,   937,   941,   943,
     947,   949,   953,   955,   958,   959,   966,   984,   986,   990,
     992,   996,   998,  1001,  1002,  1005,  1010,  1011,  1014,  1018,
    1027,  1034,  1042,  1047,  1048,  1051,  1055,  1057,  1060,  1061,
    1065,  1067,  1069,  1071,  1073,  1077,  1079,  1082,  1083,  1087,
    1088,  1090,  1094,  1100,  1104,  1105,  1108,  1109,  1112,  1114,
    1118,  1119,  1120,  1123,  1124,  1125,  1131,  1136,  1141,  1143,
    1145,  1149,  1150,  1154,  1155,  1162,  1166,  1170,  1179,  1185,
    1186,  1189,  1191,  1196,  1198,  1203,  1209,  1215,  1219,  1225,
    1227,  1231,  1232,  1233,  1234,  1235,  1236,  1239,  1241,  1244,
    1245,  1249,  1254,  1256,  1261,  1262,  1264,  1272,  1276,  1278,
    1281,  1282,  1283,  1284,  1287,  1291,  1295,  1302,  1309,  1314,
    1320,  1323,  1325,  1327,  1329,  1333,  1336,  1340,  1343,  1346,
    1349,  1358,  1369,  1379,  1389,  1399,  1401,  1405,  1407,  1411,
    1412,  1416,  1422,  1423,  1427,  1432,  1434,  1436,  1440,  1442,
    1446,  1447,  1451,  1455,  1456,  1459,  1464,  1466,  1470,  1472,
    1476,  1481,  1482,  1485,  1489,  1491,  1493,  1497,  1498,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1514,  1515,  1516,  1518,  1520,  1521,  1522,  1523,  1526,
    1528,  1532,  1536,  1540,  1545,  1549,  1553,  1555,  1559,  1563,
    1569,  1574,  1579,  1580,  1581,  1583,  1587,  1591,  1593,  1597,
    1599,  1601,  1606,  1609,  1613,  1614,  1618,  1623,  1624,  1628,
    1630,  1632,  1634,  1638,  1639,  1640,  1643,  1647,  1651,  1655,
    1698,  1703,  1705,  1707,  1711,  1713,  1715,  1721,  1729,  1731,
    1735,  1736,  1740,  1746,  1753,  1776,  1780,  1786,  1790,  1796,
    1800,  1808,  1819,  1830,  1832,  1834,  1838,  1842,  1844,  1848,
    1850,  1852,  1854,  1856,  1858,  1860,  1862,  1864,  1866,  1868,
    1870,  1872,  1874,  1876,  1880,  1882,  1886,  1892,  1896,  1898,
    1905,  1907,  1909,  1912,  1915,  1918,  1922,  1926,  1932,  1933,
    1937,  1946,  1950,  1952,  1954,  1956,  1958,  1962,  1963,  1967,
    1969,  1971,  1973,  1979,  1982,  1984,  1988,  1996,  1997,  1998,
    1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,  2008,
    2009,  2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,
    2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2030,
    2032,  2036,  2038,  2043,  2049,  2051,  2053,  2057,  2059,  2063,
    2066,  2069,  2077,  2081,  2082,  2086,  2088,  2090,  2092,  2094,
    2101,  2111,  2112,  2116,  2117,  2123,  2127,  2129,  2131,  2133,
    2135,  2137,  2139,  2141,  2143,  2145,  2147,  2149,  2151,  2158,
    2159,  2162,  2163,  2166,  2171,  2178,  2179,  2183,  2192,  2198,
    2199,  2202,  2203,  2204,  2205,  2222,  2227,  2232,  2251,  2268,
    2275,  2276,  2283,  2288,  2295,  2302,  2311,  2315,  2321,  2325,
    2329,  2335,  2339,  2346,  2352,  2358,  2366,  2371,  2376,  2383,
    2384,  2385,  2388,  2389,  2392,  2393,  2394,  2401,  2405,  2416,
    2422,  2476,  2538,  2539,  2546,  2559,  2564,  2569,  2576,  2578,
    2585,  2586,  2587,  2591,  2594,  2597,  2605,  2606,  2607,  2610,
    2614,  2618,  2620,  2624,  2628,  2629,  2632,  2636,  2640,  2641,
    2644,  2646,  2650,  2651,  2655,  2659,  2660,  2664,  2665,  2669,
    2670,  2671,  2672,  2675,  2677,  2681,  2683,  2687,  2689,  2692,
    2694,  2696,  2700,  2702,  2704,  2708,  2710,  2712,  2716,  2720,
    2722,  2724,  2728,  2730,  2734,  2735,  2739,  2743,  2745,  2749,
    2750,  2755,  2759,  2761,  2763,  2767,  2771,  2773,  2775,  2777,
    2779,  2781,  2783,  2785,  2787,  2789,  2791,  2793,  2795,  2797,
    2805,  2807,  2811,  2815,  2816,  2823,  2827,  2829,  2833,  2835,
    2839,  2841,  2848,  2852,  2857,  2862,  2863,  2865,  2869,  2872,
    2884,  2888,  2892,  2896,  2900,  2902,  2904,  2906,  2910,  2912,
    2914,  2918,  2920,  2922,  2926,  2928,  2932,  2934,  2938,  2939,
    2943,  2944,  2948,  2949,  2954,  2958,  2959,  2963,  2964,  2967,
    2968,  2969,  2974,  2979,  2980,  2984,  2986,  2990,  2991,  2995,
    2996,  2997,  3000,  3005,  3010,  3014,  3015,  3018,  3022,  3026,
    3029,  3034,  3036,  3040,  3041,  3046,  3051,  3053,  3055,  3057,
    3059,  3063,  3064,  3070,  3072,  3076,  3077,  3083,  3086,  3088,
    3092,  3094,  3101,  3103,  3107,  3111,  3113,  3115,  3121,  3123,
    3127,  3128,  3132,  3134,  3137,  3138,  3142,  3144,  3146,  3150,
    3151,  3155,  3157,  3163,  3165,  3169,  3170,  3174,  3178,  3180,
    3182,  3186,  3188,  3192,  3194,  3200,  3203,  3211,  3218,  3221,
    3228,  3229,  3232,  3236,  3243,  3244,  3247,  3252,  3258,  3265,
    3266,  3270,  3274,  3281,  3283,  3287,  3289,  3291,  3295,  3296,
    3299,  3303,  3305,  3309,  3311,  3317,  3318,  3322,  3326,  3327,
    3332,  3333,  3337,  3341,  3342,  3347,  3348,  3351,  3356,  3361,
    3364,  3371,  3372,  3374,  3375,  3377,  3381,  3382,  3384,  3386,
    3390,  3392,  3396,  3397,  3399,  3403,  3404,  3405,  3406,  3407,
    3408,  3409,  3410,  3411,  3412,  3413,  3418,  3420,  3422,  3424,
    3426,  3428,  3430,  3432,  3438,  3440,  3442,  3444,  3446,  3448,
    3450,  3452,  3454,  3456,  3458,  3460,  3462,  3464,  3466,  3468,
    3472,  3473,  3479,  3481,  3485,  3487,  3492,  3494,  3496,  3498,
    3503,  3505,  3509,  3511,  3515,  3517,  3519,  3521,  3525,  3529,
    3533,  3535,  3539,  3544,  3549,  3556,  3561,  3566,  3575,  3576,
    3580,  3587,  3588,  3589,  3590,  3591,  3592,  3593,  3594,  3595,
    3596,  3597,  3599,  3601,  3608,  3610,  3612,  3619,  3626,  3633,
    3635,  3637,  3638,  3639,  3640,  3642,  3644,  3648,  3649,  3652,
    3668,  3684,  3688,  3692,  3693,  3697,  3698,  3702,  3704,  3706,
    3708,  3710,  3714,  3715,  3717,  3733,  3749,  3753,  3757,  3759,
    3763,  3767,  3770,  3773,  3776,  3779,  3784,  3786,  3791,  3792,
    3796,  3803,  3810,  3817,  3824,  3825,  3829,  3836,  3837,  3838,
    3841,  3843,  3845,  3848,  3855,  3864,  3871,  3880,  3882,  3884,
    3886,  3890,  3893,  3897,  3898,  3902,  3911,  3913,  3917,  3919,
    3923,  3929,  3930,  3933,  3937,  3940,  3944,  3945,  3948,  3950,
    3954,  3956,  3960,  3962,  3966,  3968,  3972,  3975,  3978,  3983,
    3986,  3996,  3998,  4000,  4002,  4004,  4006,  4008,  4010,  4014,
    4015,  4018,  4019,  4022,  4030,  4033,  4036,  4039,  4042,  4045,
    4048,  4051,  4054,  4057,  4060,  4063,  4066,  4069,  4072,  4075,
    4078,  4081,  4084,  4087,  4090,  4093,  4096,  4099,  4102,  4107,
    4108,  4113,  4115,  4116,  4117,  4118,  4119,  4120,  4121,  4122,
    4123,  4124,  4125,  4126,  4127,  4128,  4129,  4130,  4131,  4132,
    4134,  4135,  4136,  4137,  4138,  4139,  4140,  4141,  4142,  4143,
    4144
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
  "CROSS", "NEXT", "SEQUENCE", "RESTART", "COMMENT", "COLLATION", "'='", 
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", 
  "':'", "'['", "']'", "'.'", "'?'", "top", "statement", "grant", 
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
  "alter_sequence_clause", "init_alter_db", "alter_db", "db_alter_clause", 
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
  "extract_expression", "string_value_function", "substring_function", 
  "string_length_opt", "udf", "cast_specification", "case_expression", 
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
       0,   285,   285,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   287,   287,   287,   288,   288,   289,
     289,   289,   290,   290,   291,   292,   292,   292,   292,   292,
     293,   293,   294,   294,   295,   296,   296,   296,   297,   297,
     298,   298,   299,   299,   300,   300,   300,   300,   301,   301,
     301,   301,   302,   302,   303,   303,   303,   304,   304,   305,
     306,   306,   307,   307,   308,   309,   309,   310,   311,   311,
     311,   312,   312,   312,   313,   313,   314,   315,   315,   315,
     315,   316,   316,   317,   317,   318,   318,   318,   318,   318,
     319,   320,   320,   321,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   323,   324,   324,   324,
     324,   325,   326,   326,   326,   327,   328,   329,   330,   331,
     331,   332,   332,   332,   333,   334,   334,   334,   335,   335,
     336,   336,   337,   337,   338,   338,   339,   340,   340,   341,
     341,   342,   342,   343,   343,   344,   345,   345,   346,   347,
     348,   349,   350,   351,   351,   352,   353,   353,   354,   354,
     355,   355,   355,   355,   355,   356,   356,   357,   357,   358,
     358,   358,   359,   360,   361,   361,   362,   362,   363,   363,
     364,   364,   364,   365,   365,   365,   366,   367,   368,   368,
     368,   369,   369,   370,   370,   371,   371,   371,   372,   373,
     373,   374,   374,   375,   375,   376,   377,   378,   379,   380,
     380,   381,   381,   381,   381,   381,   381,   382,   382,   383,
     383,   384,   385,   385,   385,   385,   385,   386,   387,   387,
     388,   388,   388,   388,   389,   390,   391,   392,   392,   393,
     394,   394,   394,   394,   394,   395,   396,   397,   397,   397,
     397,   398,   399,   400,   401,   402,   402,   403,   403,   404,
     404,   405,   406,   406,   407,   408,   408,   408,   409,   409,
     410,   410,   411,   412,   412,   413,   414,   414,   415,   415,
     416,   417,   417,   418,   419,   419,   419,   420,   420,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   422,
     422,   423,   424,   425,   426,   427,   428,   428,   429,   430,
     431,   432,   433,   433,   433,   433,   434,   435,   435,   436,
     436,   436,   437,   437,   438,   438,   439,   440,   440,   441,
     441,   441,   441,   442,   442,   442,   443,   444,   445,   446,
     447,   448,   448,   448,   449,   449,   449,   450,   451,   451,
     452,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     460,   461,   462,   463,   463,   463,   464,   465,   465,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   467,   467,   468,   469,   470,   470,
     470,   470,   470,   470,   470,   470,   471,   472,   473,   473,
     474,   474,   474,   474,   474,   474,   474,   475,   475,   476,
     476,   476,   476,   476,   476,   476,   477,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   479,
     479,   480,   480,   481,   482,   482,   482,   483,   483,   484,
     484,   484,   485,   486,   486,   487,   487,   487,   487,   487,
     488,   489,   489,   490,   490,   491,   492,   492,   492,   492,
     492,   492,   492,   492,   492,   492,   492,   492,   492,   493,
     493,   494,   494,   495,   495,   496,   496,   497,   497,   498,
     498,   499,   499,   499,   499,   499,   499,   499,   499,   499,
     500,   500,   501,   501,   501,   501,   502,   502,   503,   503,
     503,   504,   504,   505,   505,   505,   506,   506,   506,   507,
     507,   507,   508,   508,   509,   509,   509,   510,   510,   511,
     511,   511,   512,   512,   513,   513,   513,   513,   514,   514,
     515,   515,   515,   516,   516,   516,   517,   517,   517,   518,
     519,   520,   520,   521,   522,   522,   523,   524,   525,   525,
     526,   526,   527,   527,   528,   529,   529,   530,   530,   531,
     531,   531,   531,   532,   532,   533,   533,   534,   534,   535,
     535,   535,   536,   536,   536,   537,   537,   537,   538,   539,
     539,   539,   540,   540,   541,   541,   542,   543,   543,   544,
     544,   545,   546,   546,   546,   547,   548,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     549,   549,   550,   551,   551,   552,   553,   553,   554,   554,
     555,   555,   556,   557,   558,   559,   559,   559,   560,   561,
     562,   563,   564,   565,   566,   566,   566,   566,   567,   567,
     567,   568,   568,   568,   569,   569,   570,   570,   571,   571,
     572,   572,   573,   573,   574,   575,   575,   576,   576,   577,
     577,   577,   578,   579,   579,   580,   580,   581,   581,   582,
     582,   582,   583,   584,   585,   586,   586,   587,   588,   589,
     589,   590,   590,   591,   591,   592,   593,   593,   593,   593,
     593,   594,   594,   595,   595,   596,   596,   597,   598,   598,
     599,   599,   600,   600,   601,   602,   602,   602,   602,   602,
     603,   603,   604,   604,   605,   605,   606,   606,   606,   607,
     607,   608,   608,   609,   609,   610,   610,   611,   612,   612,
     612,   613,   613,   614,   614,   615,   615,   615,   616,   616,
     617,   617,   618,   619,   620,   620,   621,   622,   623,   624,
     624,   625,   626,   627,   627,   628,   628,   628,   629,   629,
     630,   631,   631,   632,   632,   633,   633,   634,   635,   635,
     636,   636,   637,   638,   638,   639,   639,   639,   640,   641,
     641,   642,   642,   642,   642,   642,   643,   643,   643,   643,
     643,   643,   644,   644,   644,   645,   645,   645,   645,   645,
     645,   645,   645,   645,   645,   645,   646,   646,   646,   646,
     646,   646,   646,   646,   647,   647,   647,   647,   647,   647,
     647,   647,   647,   647,   647,   647,   647,   647,   647,   647,
     648,   648,   649,   649,   650,   650,   651,   651,   651,   651,
     652,   652,   653,   653,   654,   654,   654,   654,   655,   656,
     657,   657,   658,   658,   658,   659,   659,   659,   660,   660,
     661,   662,   662,   662,   662,   662,   662,   662,   662,   662,
     662,   662,   662,   662,   662,   662,   662,   662,   662,   662,
     662,   662,   662,   662,   662,   662,   662,   662,   662,   663,
     663,   663,   664,   665,   665,   666,   666,   667,   667,   667,
     667,   667,   668,   668,   668,   668,   668,   669,   670,   670,
     671,   672,   672,   672,   672,   672,   673,   673,   674,   674,
     675,   676,   677,   678,   679,   679,   680,   681,   681,   681,
     682,   682,   682,   682,   682,   682,   682,   682,   682,   682,
     682,   683,   684,   685,   685,   686,   687,   687,   688,   688,
     689,   690,   690,   691,   691,   691,   692,   692,   693,   693,
     694,   694,   695,   695,   696,   696,   697,   698,   699,   700,
     700,   701,   701,   701,   701,   701,   701,   701,   701,   702,
     702,   703,   703,   704,   705,   706,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   729,   730,
     730,   731,   731,   731,   731,   731,   731,   731,   731,   731,
     731,   731,   731,   731,   731,   731,   731,   731,   731,   731,
     731,   731,   731,   731,   731,   731,   731,   731,   731,   731,
     731
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
       2,     2,     3,     3,     2,     2,     3,     6,     1,     2,
       4,     3,     2,     2,     2,     2,     3,     1,     3,     3,
       3,     2,     2,     4,     4,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     0,     2,     2,     4,
       4,     5,     0,     1,     2,     2,     4,     3,     2,     2,
       7,     1,     0,     1,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,     2,     2,     2,     1,
       1,     1,     1,     4,     5,     1,     3,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     6,     5,     3,     0,     2,     2,
       0,     3,     0,     4,     1,     5,     4,     1,     4,     1,
       2,     2,     1,     1,     1,     2,     2,     2,     2,     0,
       3,     5,     1,     1,     2,     3,     1,     2,     3,     0,
       1,     1,     1,     5,     5,     6,     1,     1,     1,     2,
       4,     1,     0,     5,     1,     0,     3,     2,     1,     0,
       2,     0,     1,     0,     3,     1,     0,     1,     2,     1,
       1,     1,     1,     2,     2,     1,     2,     3,     1,     1,
       3,     3,     1,     2,     3,     1,     2,     0,     2,     1,
       1,     0,     1,     1,     1,     3,     2,     3,     0,     1,
       3,     4,     5,     6,     7,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     2,     1,     1,     3,     3,     0,     2,     0,
       2,     0,     3,     3,     3,     1,     4,     4,     1,     9,
       0,     0,     0,     0,     3,     2,     1,     0,     3,     5,
       3,     2,     5,     2,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     0,     2,     1,     3,     1,     1,     1,
       1,     3,     6,     1,     0,     3,     0,     1,     3,     1,
       1,     1,     4,     5,     5,     1,     1,     2,     4,     4,
       2,     3,     0,     1,     2,     1,     1,     2,     2,     2,
       0,     1,     0,     3,     0,     1,     3,     1,     2,     0,
       2,     0,     2,     0,     4,     1,     2,     1,     1,     0,
       1,     3,     2,     1,     1,     2,     1,     4,     3,     1,
       3,     4,     0,     3,     0,     1,     3,     3,     1,     1,
       0,     1,     1,     4,     0,     2,     4,     0,     8,     5,
       1,     1,     7,     4,     1,     1,     8,     5,     4,     1,
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
       5,     1,     6,     1,     4,     7,     2,     0,     4,     3,
       6,     1,     1,     6,     8,     6,     1,     1,     4,     6,
       4,     5,     3,     5,     4,     5,     1,     1,     1,     4,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   589,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   589,   677,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   571,    19,   576,   577,   578,     6,    18,
     570,   572,     5,    20,   657,   764,   665,   668,    12,     9,
     780,   781,    22,   784,   785,     4,   482,     0,     0,     0,
       0,     0,     0,     0,   407,   588,   591,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,     0,     0,     0,
     103,   770,   960,     0,    23,   958,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   495,     0,   369,    29,    37,    34,    36,
     806,    35,   806,  1049,  1051,  1052,  1053,  1054,  1055,  1056,
    1075,  1076,  1057,  1059,  1058,  1064,  1060,  1065,  1061,  1063,
    1066,  1062,  1067,  1068,  1069,  1071,  1072,  1070,  1073,  1074,
    1078,  1077,  1079,  1080,     0,    31,     0,    32,     0,    67,
      69,  1041,  1050,     0,     0,     0,     0,     0,     0,     0,
     587,     0,     0,  1020,   671,   676,     0,     0,   596,     0,
     723,   725,  1043,     0,     0,     0,     0,   116,   579,  1048,
       0,     0,     2,     0,   661,     0,  1022,   777,     0,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     466,   460,   461,   459,   462,   463,   464,   465,   467,   468,
       0,   436,   437,   408,     0,  1032,   414,     0,  1036,   411,
     266,  1040,     0,   725,   410,   385,  1044,   415,     0,  1035,
     593,   586,   956,     0,   112,   166,   165,   113,   148,   215,
     818,  1028,   104,     0,   110,   160,     0,   106,   266,   114,
     962,   137,   107,   200,   108,     0,   109,   806,  1047,   115,
     161,   111,   768,   769,     0,   961,   959,     0,    75,     0,
    1033,   741,   503,  1031,   496,     0,   502,   507,   497,   498,
     505,   499,   500,   501,   506,   508,   363,     0,   268,    30,
       0,    39,   805,    38,    28,     0,     0,     0,     0,     0,
     811,     0,     0,     0,     0,     0,     0,   585,   966,     0,
     947,   670,   670,   671,   683,   681,  1019,   684,     0,   685,
     675,   671,     0,     0,     0,     0,     0,     0,   612,   605,
     594,   595,   597,   599,   600,   601,   608,   609,   602,     0,
     724,  1042,   120,     0,   117,   266,   118,   200,   119,   806,
     582,     0,   635,   636,   643,     0,   642,   644,   646,   651,
     639,   637,   640,   638,   647,   650,   645,   649,     0,     0,
       0,   659,     0,   655,     0,     0,  1021,     0,     0,   662,
       0,     0,     0,     0,   412,   483,   377,     0,   377,     0,
     218,   413,   418,   128,   477,   478,     0,   268,   239,   469,
       0,   409,   427,     0,   383,   384,   492,     0,   592,   590,
     957,   163,   163,     0,     0,     0,   175,   167,   168,   147,
       0,   125,     0,     0,     0,   121,   268,   136,   135,   138,
       0,     0,     0,     0,     0,  1024,    76,   218,     0,     0,
     743,   783,   504,     0,     0,     0,     0,   924,   953,     0,
    1023,     0,   926,     0,     0,   954,     0,   948,   939,   938,
     937,  1049,     0,     0,     0,   929,   930,   931,   940,   941,
     949,   950,  1059,   951,   952,     0,  1064,  1060,   955,  1073,
    1078,     0,     0,     0,     0,   906,   366,   901,   815,   933,
     927,   902,   361,   942,   904,   905,   921,   922,   923,   943,
     903,   967,   968,   981,   969,   983,   910,   907,   908,   991,
     992,   996,   997,   909,   928,     0,     0,  1028,     0,     0,
       0,   370,   216,     0,     0,     0,   808,    27,     0,    33,
       0,     0,    43,    70,    72,  1045,    68,     0,     0,     0,
     810,     0,    48,    50,    28,     0,     0,   584,     0,     0,
       0,   680,   678,     0,   687,     0,   686,   688,   693,   674,
       0,   631,     0,   606,   617,   603,   617,   604,   618,   624,
     628,   629,   613,   598,   741,   789,   819,     0,     0,   126,
     268,     0,     0,   581,   580,   648,   641,     0,     0,  1039,
       0,     0,   656,   660,   763,   765,   770,   667,   666,   775,
     183,     0,   485,   144,     0,   488,     0,   489,   484,     0,
     422,     0,   424,   423,     0,   425,     0,   419,     0,     0,
     269,     0,     0,   431,     0,   432,   377,   470,     0,   476,
       0,     0,     0,   473,  1028,   388,   387,   491,     0,   405,
       0,   164,     0,     0,   173,     0,   172,     0,     0,   162,
     176,   177,   179,   169,   540,   552,   527,   562,   530,   553,
     563,     0,   569,   531,     0,   559,     0,   554,   566,   526,
     549,   528,   529,   524,   377,   509,   510,   511,   519,   525,
     512,   521,   542,     0,   547,   544,   522,   559,   523,   124,
       0,   122,   266,   123,     0,     0,   139,     0,     0,   199,
     239,   385,   375,     0,   218,     0,    82,    84,     0,     0,
     102,   101,  1025,     0,     0,     0,     0,   892,   893,   894,
       0,   740,   823,   832,   835,   841,   836,   837,   838,   839,
     843,   844,   842,   845,   840,   821,     0,   749,   764,  1020,
       0,  1020,   944,     0,     0,  1020,  1020,  1020,     0,   945,
     946,     0,     0,  1007,     0,     0,     0,     0,     0,     0,
     912,   911,     0,   764,   933,     0,   331,  1046,     0,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   519,   542,     0,     0,   368,     0,   279,     0,   807,
       0,     0,    44,    73,     0,     0,    26,   797,     0,   813,
       0,   779,   797,     0,     0,    47,   583,     0,   670,     0,
       0,   741,     0,   692,     0,   574,     0,   573,   964,     0,
     607,     0,   615,   611,   610,     0,   621,     0,   626,   614,
       0,   743,   787,     0,     0,     0,   239,   375,   654,   632,
     653,     0,     0,     0,   658,     0,   774,     0,     0,   145,
     184,   487,   377,   421,     0,   377,   378,   426,   375,     0,
     265,   375,   238,  1029,   210,   207,     0,   150,   377,   509,
     375,     0,     0,     0,   237,   240,   241,   242,   243,     0,
     430,   475,   474,   429,   428,     0,     0,     0,   391,   389,
     390,   386,     0,   377,   480,     0,   479,   193,   170,   174,
       0,     0,   178,     0,     0,   537,   550,   551,   567,     0,
     564,   569,     0,   557,   555,   556,   150,     0,     0,     0,
     520,     0,     0,     0,     0,   558,   127,   268,     0,   375,
     140,   198,     0,   201,   203,   204,     0,     0,     0,     0,
     218,   218,    79,     0,    87,    78,     0,     0,     0,     0,
     834,   822,     0,     0,   895,   896,   897,     0,   826,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   745,   747,
     748,   742,     0,   777,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1011,  1012,
    1013,  1014,  1015,  1016,  1017,  1018,     0,     0,     0,     0,
    1002,     0,     0,     0,     0,     0,     0,     0,   920,   777,
     919,   362,     0,   332,   364,   333,     0,     0,   915,  1027,
     914,   913,   916,   917,   918,   934,   989,     0,   925,   817,
     816,   372,   371,     0,   272,   287,     0,   278,   280,   809,
       0,     0,     0,    71,     0,   802,     0,   812,     0,   802,
       0,     0,   679,   682,     0,   694,   695,   698,   700,   697,
     709,   710,   711,   699,   722,     0,   734,   689,   691,  1037,
     575,   965,   616,   625,   620,   619,     0,   630,   790,   764,
     791,   820,   375,     0,     0,   633,   652,     0,   766,   672,
     767,   776,   486,   163,   190,   182,   185,   186,     0,     0,
       0,   420,   375,   375,   378,   277,   270,   279,   209,   377,
     377,   378,   211,   206,   212,     0,     0,   248,     0,   434,
     378,   377,   375,   433,     0,     0,     0,   404,   494,   481,
     194,   195,   171,     0,   181,   538,   539,   963,     0,     0,
       0,   542,     0,   565,     0,     0,   378,     0,   515,   517,
       0,     0,     0,     0,     0,     0,     0,   385,   279,   163,
     142,   239,   196,   405,   380,     0,    83,     0,   218,     0,
      91,     0,    85,     0,     0,    90,    86,     0,   788,  1030,
       0,     0,   827,     0,     0,   833,     0,     0,   825,   824,
       0,   882,     0,   871,   870,     0,   849,     0,   880,   898,
       0,   890,     0,     0,     0,   850,   876,     0,     0,   853,
       0,     0,     0,     0,     0,     0,     0,   851,     0,     0,
     852,     0,   884,     0,     0,   846,     0,     0,   847,     0,
       0,   848,   746,   749,   782,     0,     0,   218,     0,   970,
       0,     0,   984,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1008,     0,     0,  1006,     0,     0,   998,     0,
       0,     0,     0,  1009,   664,     0,     0,     0,   932,   988,
       0,   267,   286,     0,   338,   367,   281,     0,     0,     0,
       0,     0,  1055,    41,    52,    54,    53,    62,    64,    41,
      42,     0,     0,     0,   794,   814,     0,   793,     0,     0,
       0,   730,   697,     0,   732,   726,   732,   730,   732,     0,
       0,     0,   693,     0,   739,   622,   623,   627,   777,   279,
     197,   380,   634,     0,     0,   191,     0,   187,     0,     0,
       0,   416,   226,   221,   935,   222,   223,   224,   225,     0,
       0,   271,     0,     0,     0,   227,     0,   248,   770,   244,
       0,   435,   471,   472,   396,   397,   393,   392,   395,   394,
     377,   493,   378,   180,  1026,     0,     0,   533,     0,   532,
     568,     0,   560,   151,     0,   513,     0,   541,   542,   548,
     546,     0,   543,   375,     0,     0,     0,   134,   143,   202,
     377,     0,   376,   105,     0,   132,   131,    94,     0,     0,
      95,     0,    88,    89,     0,   888,   889,   829,   828,   831,
     830,     0,     0,     0,     0,   764,     0,     0,     0,   891,
       0,     0,     0,     0,     0,     0,   883,   881,   877,     0,
     885,     0,     0,     0,     0,   886,     0,     0,     0,     0,
       0,     0,   753,     0,   750,     0,   754,   976,   975,     0,
       0,   972,   971,     0,   980,   979,   978,   977,   974,   973,
       0,   987,  1004,  1003,     0,     0,     0,     0,     0,     0,
       0,   365,   335,   334,   273,     0,     0,   283,   284,     0,
    1028,   321,     0,     0,     0,     0,     0,     0,  1057,  1062,
       0,     0,   359,   297,   292,     0,   338,   291,     0,     0,
       0,   317,   318,     0,   315,     0,     0,   316,     0,     0,
       0,   353,   355,   354,     0,     0,     0,     0,     0,     0,
       0,   792,     0,  1028,    66,    58,    59,    65,    60,    61,
       0,    24,     0,     0,    25,     0,   798,   799,   800,   796,
     801,   804,   803,   778,    45,    46,   693,   701,   696,   731,
     729,   727,     0,   728,     0,     0,     0,     0,     0,     0,
     743,   786,     0,   376,   771,   772,   673,   193,   192,   188,
       0,   378,   936,   275,   276,   378,   378,   149,   214,   228,
     229,   377,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   490,   535,     0,   536,     0,     0,   214,
     152,   153,   155,   156,   157,   516,   518,   514,   545,   279,
     405,   378,   193,     0,     0,   373,   377,    92,     0,     0,
      98,    93,   542,     0,   874,     0,     0,   900,   777,   899,
     872,     0,     0,     0,   878,     0,     0,     0,     0,   887,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     744,   749,     0,   756,     0,   752,   755,   990,   217,  1010,
     982,     0,     0,  1005,  1000,   999,     0,   993,   995,     0,
     220,   282,     0,   319,     0,   314,     0,   329,   313,   341,
    1038,   356,   357,     0,   293,     0,   298,   295,   344,   308,
     309,   305,   306,   310,   303,     0,     0,   312,   311,   304,
       0,   300,   302,   301,   299,   307,   337,     0,    55,    56,
      57,    63,     0,   704,     0,   712,   730,   721,   719,   733,
     735,   737,   738,   669,   378,   374,   773,   189,   378,   159,
     264,     0,     0,   205,   230,     0,     0,     0,   248,   231,
     234,   806,     0,   378,   402,   403,   399,   398,   401,   400,
       0,   534,   561,   158,   146,   154,     0,   377,   261,   141,
     378,   379,     0,     0,    96,    97,    80,     0,   857,   865,
     663,   873,   858,   866,   861,   869,   875,   879,   859,   867,
     860,   868,   854,   862,   855,   863,   856,   864,   751,     0,
     762,   986,   985,  1001,     0,     0,     0,   289,   285,     0,
     320,   322,     0,   328,     0,   352,     0,     0,     0,     0,
     347,   345,     0,     0,     0,   330,    40,   795,   706,   703,
     713,     0,     0,   714,   715,   716,     0,   262,   417,   208,
     213,   232,   248,   806,   235,   254,   247,   249,   406,   378,
       0,   381,   378,    77,    99,   100,     0,   759,     0,   758,
     994,   219,   288,     0,     0,     0,     0,   351,   350,  1034,
     349,   338,     0,     0,     0,     0,     0,   702,   717,     0,
     736,   236,   254,     0,   248,   250,   251,   263,   378,     0,
     757,     0,     0,     0,   325,   338,   358,   346,   348,     0,
     343,     0,     0,   707,     0,   248,     0,     0,   246,     0,
     253,     0,   252,   382,   133,   760,     0,   290,   327,     0,
       0,     0,   338,     0,   705,   718,   233,     0,     0,   257,
     256,   255,   761,   338,     0,     0,   338,   336,   708,   260,
     258,   259,   326,   338,   342,   323,   324,     0,     0,     0
};

static const short yydefgoto[] =
{
    1957,    21,    22,   538,   144,   145,   146,   147,  1551,   806,
     801,    23,   158,   159,  1303,  1304,  1305,  1306,  1307,   148,
     149,   542,   543,    24,    88,   446,   954,   715,   716,   717,
    1196,  1189,  1190,  1641,   278,   719,    25,    80,    26,   177,
      27,   435,   252,   352,   699,   223,    81,  1413,   259,   439,
     707,  1180,  1407,   612,   247,   430,  1131,  1619,  1620,  1621,
    1622,  1623,  1624,   254,   269,   244,   652,   245,   426,   427,
     428,   659,   660,   661,   613,   614,  1115,  1116,  1117,  1346,
    1152,   262,   356,   441,   942,   943,   944,   875,   876,   877,
    1753,   634,   528,  1469,   718,  1817,  1351,  1598,  1599,  1600,
    1759,   945,   636,   884,   885,   886,   887,  1369,   888,  1894,
    1895,  1896,  1940,   257,   354,   701,   229,   407,   534,   629,
     630,  1053,   529,  1124,  1056,  1057,  1058,  1496,  1497,  1293,
    1818,  1498,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1823,  1526,   495,  1034,  1527,
    1528,  1529,  1931,  1707,  1708,  1829,  1830,  1530,  1531,  1532,
    1533,  1703,  1534,   496,   779,    29,   298,   530,   531,   266,
     358,   947,  1635,   621,  1121,  1412,   264,   703,   416,   647,
     648,   901,   903,  1381,    30,    64,   866,   863,   401,   402,
     411,   412,   220,   221,   413,  1140,   642,   893,   226,   237,
     188,   394,   395,   234,   649,  1382,    31,   103,   684,   685,
     686,  1167,  1168,   687,   791,   689,   690,  1161,   915,   930,
     691,   792,   693,   694,   695,   696,   923,   697,   698,   920,
      32,    33,    34,    35,    36,   594,    37,   558,    38,    39,
      66,   241,   419,    40,   340,   341,   342,   343,   344,   345,
     346,   347,   833,   348,  1096,  1337,   578,   579,   838,   580,
      41,    42,   378,   379,   380,   853,   849,  1535,   184,   602,
     383,    44,  1434,   772,    45,    46,    47,   561,   330,  1343,
    1746,   163,   164,   165,   328,   565,   566,   567,   824,   821,
    1075,  1076,  1077,  1078,  1838,  1887,  1912,  1079,  1080,  1081,
    1082,  1843,  1844,  1845,  1083,  1332,   169,   170,  1330,  1570,
    1334,  1739,  1740,  1580,   450,   748,  1462,   992,  1463,  1464,
    1465,  1675,  1866,  1869,   187,   604,   605,   274,  1586,  1110,
     389,  1536,  1537,    50,    51,  1538,    53,    54,   451,   584,
    1539,  1540,    55,  1065,  1555,  1556,  1314,  1560,   301,   302,
     535,   549,   550,   808,   497,   498,   587,   731,   967,   732,
     733,   734,   735,  1215,   736,   737,   738,   739,   740,   741,
     742,   743,   744,   745,   969,  1218,  1219,   746,   500,   501,
     502,  1353,   503,   504,   505,   506,   507,   508,   509,  1558,
      85,   276,   261,  1159,  1169,   828,   510,   511,   512,   513,
     514,   515,  1682,   516,   517,   518,   519,   520,   521,  1023,
     522,   764,  1276,   765,  1273,   523,  1016,   329,   387,   524,
     525,   721,  1383,  1038,   250,   872,  1198,   282,   224,   279,
    1878,   255,  1867,  1088,  1542,   598,  1084,   150,   526,   233,
     235,  1308,   776,   267,   178,   527,   152
};

static const short yypact[] =
{
    1615,   616,    79,  1784,   285,   797,   215,  2427,    46,  6761,
     446,   263,  1489,    79,   671,   453,  5870,   454,  5870,   252,
     450,   411,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   552,   462,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  7714,  5870,  5870,
    5870,  5870,  5870,  5870,-32768,-32768,   547,   717,  5870,  5870,
    5870,   699,  5870,   541,  5870,  5870,-32768,  5870,  5870,  5870,
  -32768,   644,-32768,   553,-32768,-32768,   697,  5870,-32768,  5870,
    5870,  5870,   714,  5870,  5870,  5870,  5870,   541,  5870,  5870,
    5870,  5870,  5870,-32768,  5870,   475,   635,-32768,-32768,-32768,
     515,-32768,   515,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   692,   545,   709,-32768,    34,-32768,
  -32768,-32768,-32768,  5870,  5870,  5870,   739,   747,   590,  5870,
     702,   301,   355,   484,   652,-32768,  5870,   809,  1202,   735,
  -32768,  5870,-32768,  5870,  5870,  5870,  5870,-32768,-32768,-32768,
    5870,  2421,-32768,   785,   746,   913,   650,   712,   787,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     474,-32768,-32768,-32768,   717,-32768,-32768,   226,-32768,-32768,
     703,-32768,   236,-32768,-32768,   599,-32768,-32768,   743,-32768,
     852,-32768,-32768,   792,-32768,   678,-32768,-32768,   991,-32768,
  -32768,-32768,-32768,   717,-32768,-32768,   151,-32768,   703,-32768,
  -32768,   422,-32768,   961,-32768,   962,-32768,   515,-32768,-32768,
  -32768,-32768,-32768,-32768,   947,-32768,-32768,   837,-32768,   970,
  -32768,   867,-32768,-32768,-32768,   837,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  6434,  5870,   907,-32768,
    5870,-32768,-32768,-32768,   888,   727,   922,  4337,  5870,  1012,
     817,  1038,  1041,  1065,  1005,  1008,    92,   890,-32768,  6653,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5120,-32768,
  -32768,-32768,   959,  5870,  1035,   955,   250,  5870,   974,-32768,
  -32768,  1202,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5870,
  -32768,-32768,-32768,   717,-32768,   703,-32768,   961,-32768,   515,
    1019,   994,-32768,-32768,-32768,  1094,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1077,  5870,
    5870,  1046,   926,-32768,  6653,  1023,-32768,  1023,  6653,-32768,
      68,   928,   925,   930,   787,-32768,  1155,   205,-32768,  5870,
  -32768,   474,-32768,-32768,-32768,-32768,  5870,   907,  3149,   983,
    3399,   914,-32768,  7714,-32768,-32768,   762,  1002,-32768,-32768,
  -32768,   924,   924,   717,  1083,   717,   177,   678,-32768,-32768,
    7661,-32768,  5870,  5870,  5870,-32768,   907,-32768,-32768,  1167,
     367,   920,  5870,  1190,  5870,-32768,-32768,    62,  5432,  4244,
    1064,-32768,-32768,   923,   927,   929,  1004,-32768,-32768,   931,
  -32768,   932,-32768,   933,   943,-32768,   944,-32768,-32768,-32768,
  -32768,   946,   952,  1020,  1026,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   953,-32768,-32768,  5339,   957,   963,-32768,   964,
    1109,  6653,  6653,  5558,  5870,-32768,  1084,   958,-32768,  1151,
  -32768,-32768,   968,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   971,   965,   967,  7661,   977,
     529,-32768,-32768,   978,  1242,   620,-32768,-32768,  5870,-32768,
    5870,  5870,    32,-32768,-32768,-32768,-32768,  5870,  5870,    97,
  -32768,  5870,-32768,-32768,   888,  1114,  4337,-32768,  5870,  5558,
     214,-32768,-32768,  6653,-32768,  1194,   981,-32768,   175,-32768,
     426,-32768,   351,-32768,   407,-32768,   407,-32768,   985,-32768,
     100,-32768,  1097,-32768,   324,-32768,-32768,  1000,   996,-32768,
     907,   982,  1267,-32768,-32768,-32768,-32768,   337,  1199,-32768,
    1017,  5870,-32768,-32768,  1006,-32768,   406,-32768,-32768,   804,
  -32768,  1197,  1211,-32768,   717,-32768,  1217,-32768,-32768,  1289,
  -32768,  1294,-32768,-32768,  1274,-32768,  7661,-32768,  7661,   665,
  -32768,  1308,  5870,-32768,  7328,-32768,   155,-32768,  5870,   752,
     236,  1149,  1113,  1191,  1152,-32768,-32768,-32768,   182,  1193,
     442,-32768,  1129,   541,-32768,   717,-32768,  1309,  1241,-32768,
     177,-32768,-32768,-32768,    80,  1150,-32768,-32768,-32768,  1153,
  -32768,  1196,  1054,-32768,  1246,  1061,   435,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1059,-32768,
  -32768,-32768,   105,  1063,  1066,   102,-32768,  1061,-32768,-32768,
     717,-32768,   703,-32768,  1281,  1336,-32768,   717,   717,-32768,
    3149,   599,-32768,  1227,-32768,  1213,  1086,-32768,  7617,  1232,
  -32768,-32768,-32768,  1245,  1089,  4463,  1090,  1330,  2245,  2549,
    3994,   208,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  3049,   539,  1243,   651,
    6653,   147,-32768,  5870,  6653,   667,   685,   704,   710,-32768,
  -32768,  6653,  4682,  1151,   150,  1182,  6653,  6653,  4682,  1311,
     179,   179,  1095,   462,   227,   693,-32768,-32768,  4556,-32768,
    6653,  5870,  6653,  6653,  6653,  6653,  6653,  6653,  5777,  3650,
  -32768,-32768,  1355,  1101,  5870,-32768,  5870,  1337,  5870,-32768,
    1204,  1214,-32768,-32768,  1185,  4337,-32768,  1329,   719,-32768,
    1115,-32768,  1329,  5870,  5870,  1118,-32768,   227,-32768,   280,
    4994,  1201,  6653,-32768,  5870,-32768,   316,-32768,-32768,   148,
  -32768,  1206,-32768,-32768,-32768,  5870,   131,  5870,-32768,-32768,
    5870,  1064,-32768,  6653,  5870,  1386,  3149,-32768,-32768,-32768,
  -32768,   337,  5870,  1315,  1122,  6653,  1160,  6653,   717,-32768,
     476,-32768,-32768,-32768,  1123,-32768,-32768,-32768,   127,  5870,
  -32768,-32768,-32768,-32768,  1384,-32768,  1126,  1363,-32768,  1380,
  -32768,  1323,  1324,   515,-32768,-32768,-32768,-32768,-32768,  1392,
  -32768,-32768,-32768,-32768,-32768,  5870,  7426,   541,  1302,  1303,
    1305,-32768,   541,-32768,-32768,   502,-32768,   779,-32768,-32768,
    1276,   717,-32768,  5432,   312,  1280,-32768,-32768,-32768,   541,
  -32768,  1054,   287,-32768,-32768,-32768,  1363,   287,  1282,   287,
  -32768,   541,   541,  1158,   541,-32768,-32768,   907,  5870,-32768,
    1339,-32768,   725,-32768,-32768,-32768,   762,  1023,  5870,   736,
      44,-32768,-32768,  1161,   220,-32768,  5432,  5870,  1023,  4901,
  -32768,-32768,  1023,  4463,-32768,-32768,-32768,   129,-32768,   211,
    1457,  4682,  4682,  6653,  6653,  1882,  1162,   623,  2173,  6653,
    2525,   460,  2775,  3025,  5996,  3275,  3525,  3775,-32768,-32768,
    1334,-32768,  1166,   712,  6653,  6653,   200,  6653,  1168,  6653,
    1174,   448,  6653,  6653,  6653,  6653,  6653,  6653,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1382,   720,   210,  6653,
  -32768,  4682,  6653,   281,  1022,  1031,   121,  5870,-32768,   712,
  -32768,-32768,  4775,-32768,  1175,-32768,  1170,   570,-32768,-32768,
    1431,   179,   179,   826,   826,  1151,-32768,   740,-32768,-32768,
  -32768,-32768,-32768,   767,-32768,  1271,  1442,  1337,-32768,-32768,
    7645,  7645,  1335,-32768,   343,  1358,  5870,-32768,  6653,  1358,
    1403,  1404,-32768,-32768,  4118,  1192,  1007,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1195,  4682,  1397,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   473,-32768,-32768,  1243,
    1151,-32768,-32768,   770,  1023,-32768,-32768,   337,-32768,-32768,
  -32768,  1151,-32768,   924,  1458,-32768,   476,-32768,  1200,  4682,
    1976,-32768,-32768,-32768,-32768,-32768,-32768,  1337,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   515,   515,  1233,  1203,-32768,
  -32768,-32768,-32768,-32768,   110,   138,   606,-32768,  1461,-32768,
  -32768,-32768,-32768,  5870,-32768,-32768,-32768,-32768,   285,   776,
    1317,  1355,  1205,-32768,  1129,   814,-32768,   573,-32768,  1207,
    5870,   588,  1212,  1215,   541,  1219,  1465,   599,  1337,   924,
    1211,  3149,-32768,  1193,  1287,  2649,-32768,   541,  1349,  1434,
  -32768,  7617,-32768,   541,   254,-32768,-32768,  1441,-32768,-32768,
    1220,  1229,-32768,  4682,  4682,-32768,  4682,  4682,-32768,  1475,
     168,  1151,  1218,-32768,-32768,  1235,  1151,  5558,-32768,-32768,
    1448,-32768,   598,  1240,  1244,  1151,  1108,  1247,  1253,  1151,
    6653,  6653,  1162,  6653,  6215,  1257,  1258,  1151,  1259,  1260,
    1151,  6653,  1151,  1261,  1263,  1151,  1264,  1268,  1151,  1269,
    1270,  1151,-32768,  3878,-32768,   640,   662,-32768,   677,-32768,
     688,  6653,-32768,   701,   711,   733,   758,   825,   866,  6653,
    6653,  6653,  1151,  1468,   330,  1151,  1347,  6653,-32768,  6653,
    6653,  6653,  6653,-32768,-32768,   829,  4775,  5651,-32768,-32768,
    5870,-32768,-32768,  5870,  7002,-32768,-32768,  5870,  5870,  5870,
    5870,  5870,  5870,  1331,  1252,-32768,  1272,-32768,-32768,  1331,
  -32768,   284,   284,    72,-32768,-32768,   832,-32768,  7645,  7645,
    1273,   889,  1277,  4994,  1394,-32768,  1394,   509,  1394,  1466,
    1470,  6653,  1251,  1531,  1480,-32768,-32768,-32768,   712,  1337,
  -32768,  1287,-32768,   592,  1129,  1427,  1129,-32768,  4682,   134,
     682,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1976,
    1976,-32768,  1442,  6653,  1976,   706,  1416,  1233,   644,-32768,
    4682,-32768,-32768,-32768,  1439,  1445,  1446,  1447,  1449,  1453,
  -32768,-32768,-32768,-32768,-32768,  1300,   285,-32768,  1385,-32768,
  -32768,   287,-32768,   203,   287,-32768,   287,-32768,  1355,-32768,
  -32768,  1301,-32768,-32768,   762,  1442,  1129,-32768,  1211,-32768,
  -32768,  1560,-32768,-32768,  1307,-32768,  1122,-32768,  1310,   717,
     144,  1312,-32768,-32768,   717,-32768,-32768,-32768,  1475,-32768,
    1475,  6653,  1023,  1023,  1313,   462,   835,  6653,  1523,-32768,
    1023,  1023,  6653,  1023,  1023,   186,  1151,-32768,  1143,  6653,
    1151,  1023,  1023,  1023,  1023,  1151,  1023,  1023,  1023,  1023,
    1023,  1023,-32768,  1316,  1319,   604,  5870,-32768,-32768,  1318,
    7661,-32768,-32768,   941,-32768,-32768,-32768,-32768,-32768,-32768,
     954,  1147,-32768,-32768,  6653,  6653,  1537,  1417,   993,   839,
    1082,-32768,-32768,-32768,-32768,  7661,  1322,-32768,-32768,  1558,
    1570,  5870,   304,  1332,  1338,  1525,  6653,  1333,  1341,  5213,
    5870,  5870,-32768,-32768,-32768,  1559,  6894,-32768,  1342,  1343,
    1344,-32768,-32768,  1345,-32768,  1350,  1351,-32768,   125,  1352,
    1353,-32768,-32768,-32768,  1354,  1530,  1356,  1357,  1359,  1360,
    1361,-32768,  1362,   -22,-32768,-32768,-32768,-32768,-32768,-32768,
    1544,-32768,  7645,  7645,-32768,  1451,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1605,-32768,  1007,-32768,
  -32768,-32768,  1546,-32768,  4994,  4994,   840,  5870,  6653,  4682,
    1064,-32768,  1442,-32768,-32768,-32768,-32768,   779,-32768,-32768,
     135,-32768,-32768,-32768,-32768,-32768,  1151,-32768,  1616,   706,
  -32768,   615,  5870,-32768,  1562,   139,  1463,  1565,  1467,  1594,
    1566,  1606,  1337,-32768,-32768,  1370,-32768,  1373,  1564,  1616,
     203,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1337,
    1193,-32768,   779,  1461,  1532,-32768,-32768,-32768,  1543,   116,
  -32768,-32768,  1355,  1549,  1151,  1387,  1388,-32768,   712,-32768,
    1151,  6653,  1389,  1391,  1151,  1393,  1395,  6653,  6653,  1151,
    1398,  1401,  1402,  1405,  1409,  1413,  1414,  1422,  1430,  1433,
  -32768,  3878,  1418,-32768,  5870,-32768,-32768,-32768,-32768,-32768,
  -32768,  6653,  1435,-32768,-32768,-32768,  6653,-32768,-32768,  6653,
     146,-32768,  1602,  6653,  6653,-32768,  4682,  1151,-32768,-32768,
  -32768,-32768,-32768,  5870,-32768,   190,-32768,  1484,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   171,  1443,-32768,-32768,-32768,
    4775,-32768,-32768,-32768,-32768,-32768,-32768,  1552,-32768,-32768,
  -32768,-32768,   284,  5870,  4994,-32768,  1412,-32768,-32768,  1406,
  -32768,  1151,   208,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1440,  5870,-32768,-32768,  1588,  1627,  5870,  1233,-32768,
  -32768,   515,  5870,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1442,-32768,-32768,-32768,-32768,-32768,  1442,-32768,-32768,-32768,
  -32768,-32768,  6653,   717,-32768,  1516,-32768,   717,-32768,-32768,
  -32768,  1151,-32768,-32768,-32768,-32768,  1151,  1151,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5870,
    1643,  1151,-32768,-32768,  1001,  1976,  1976,-32768,-32768,  1455,
    1151,  1189,   140,-32768,  1641,-32768,  5870,  5870,   285,   107,
  -32768,-32768,  1491,  1653,  4682,  1175,-32768,-32768,  1462,-32768,
  -32768,  4682,  1464,-32768,-32768,-32768,  6653,-32768,-32768,-32768,
  -32768,-32768,  1233,   515,-32768,  1619,-32768,-32768,-32768,-32768,
    1461,-32768,  1151,-32768,-32768,-32768,  1471,  1469,  1473,-32768,
  -32768,-32768,-32768,  1023,  4775,  1571,  4775,-32768,-32768,-32768,
  -32768,  7137,   190,  6653,  4775,   141,  5870,-32768,   208,  5870,
  -32768,-32768,  1619,   192,  1233,  1625,  1630,-32768,-32768,  1476,
  -32768,  5870,  5870,  1477,  1175,  7137,  1175,-32768,-32768,  1231,
     130,  1702,   870,-32768,   874,  1233,   459,   459,-32768,  1713,
  -32768,  1579,-32768,-32768,-32768,-32768,  1482,-32768,  1712,  4775,
    1728,  1720,  7137,  5870,-32768,-32768,-32768,  1568,   594,-32768,
  -32768,-32768,-32768,  7137,   126,  5870,  7137,-32768,-32768,-32768,
  -32768,-32768,-32768,  7137,-32768,-32768,-32768,  1772,  1773,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1221,  1618,-32768,  1621,  1478,   472,-32768,
     972,-32768,-32768,-32768,  -909,-32768,  -565,-32768,  -478,  1623,
    1481,  1228,   987,-32768,-32768,-32768,   597,-32768,  1079,   843,
  -32768,-32768,   608,-32768,-32768,   841,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   618,-32768,-32768,   873,-32768,-32768,   181,
  -32768,-32768,  1407,  1723,-32768,-32768,  -381,-32768,-32768,-32768,
    1378,-32768,-32768,  1154,  -397,-32768,-32768,-32768,   700,-32768,
   -1317,-32768,-32768,  1456,   969,   638,  1415,   945,   637,-32768,
     201,   -57,  -294,-32768,  -367,-32768, -1180,-32768,-32768,   228,
  -32768,  1420, -1232,-32768,-32768,-32768,-32768, -1296,   229,   -63,
     -65,   -61,   -82,-32768,-32768,-32768,-32768,  -133,  -333,-32768,
     973,-32768,  -683,-32768, -1009,-32768,   780,-32768,-32768,-32768,
  -32768,-32768, -1335, -1011,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -761, -1001,-32768,
  -32768,-32768,-32768,-32768,   133,-32768,   -46,-32768,-32768,-32768,
  -32768,-32768,  1838,-32768,-32768,-32768,-32768,-32768,  1047,-32768,
  -32768,  -561,   260,  -383,  -919,   504,-32768,-32768,  -595,  -827,
  -32768,-32768, -1066, -1438,-32768,-32768,-32768,-32768,-32768,  1450,
  -32768,  1208,  1437,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1459,-32768,-32768,-32768,-32768,-32768,  -596,  -496,
  -32768,   918,   461,  -607,  -403,-32768,-32768,-32768,-32768, -1051,
  -32768,  -402,-32768,-32768,-32768,-32768,  1157,-32768,-32768,   935,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1844,-32768,-32768,-32768,-32768,-32768,  1517,-32768,-32768,-32768,
    1290,-32768,  1285,-32768,-32768,-32768,-32768,  1028,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  -720,     1,-32768,-32768,
  -32768,  -443,   246,-32768,  -371,   679,-32768,  -228,   195,-32768,
  -32768,-32768,-32768,  1700,-32768,-32768,-32768,  1043, -1181,-32768,
  -32768,  -970, -1414,-32768,-32768,-32768,-32768,   794,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1777,   -40,   542,  -171,
  -32768,-32768,    24,-32768,  -416,  -802,  1124,-32768,   202,-32768,
     409,-32768,  -956,-32768,  -671,-32768,  1029,  -569,-32768,-32768,
    -946,  1878,  1883,-32768,-32768,  1885,-32768,-32768,  1314,-32768,
    -240,-32768,-32768,  1074, -1192,-32768,   820,-32768,  -108,  -747,
    -591,-32768,-32768,-32768,  -752,   -56,  -486,  -738,-32768,  -533,
  -32768,-32768,-32768,    56,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -584,-32768,   668,-32768,  1024, -1064,-32768,
    -451,-32768,   544, -1047,  -148, -1015,  -999,  -996,   -49,    -2,
      -1,-32768,   -39, -1083,  -527,  -142,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1239,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   622,-32768, -1143,-32768,-32768,   432,-32768,  -948,
     497,   986,   734,   153,  -688,  1265, -1236,-32768,   -66,  1813,
  -32768,   -27,   -51, -1178,   398,  -265,     4,   -55,  -166,   -10,
     -59,  -272,-32768,   -78,   -53,    -9,-32768
};


#define	YYLAST		7982


static const short yytable[] =
{
     151,    43,    84,   253,   303,   350,   171,   172,   227,   179,
     854,   248,   249,   321,   324,   624,   265,  1033,   246,   322,
     325,   232,   293,   270,  1018,   284,  1035,   688,   692,   662,
    1026,  1285,   790,   626,   263,   544,   238,   856,   878,  1099,
     292,   653,   775,   827,   288,  1295,   294,  1254,   222,   225,
     228,   231,   172,   236,   239,  1541,  1352,  1499,   290,   251,
     225,   239,   809,   231,   230,   172,   236,   287,   268,   151,
     239,  1603,   260,  1354,   631,   295,   258,   993,   280,   171,
     172,   283,   225,  1284,   280,   239,   228,   231,   291,   172,
     236,   268,   151,   239,   562,   231,   260,   309,   359,   311,
     289,  1050,  1029,   705,  1321,  1355,   811,   353,   296,   585,
    1389,   955,   628,  1054,   310,   600,   946,  1410,  1362,  1183,
    1559,  1356,   773,   906,  1357,   436,   928,   360,  1482,   971,
     867,  1105,   868,  1601,  1486,   357,  1137,  1203,   879,   332,
    1930,   961,   971,   971,   251,   172,   251,   971,   971,   971,
     151,  1577,  1309,   326,  1881,   556,  1101,   239,   610,   443,
    1735,   836,   351,  1639,   225,   231,   172,   268,   841,  1405,
    1122,   179,  1358,  1953,   574,   403,  1431,  1187,   355,  1593,
    1594,  1706,  1376,   588,  1597,   823,  1715,   104,   773,  1815,
    1374,   781,   960,   997,  1657,  1780,   782,   968,   781,  1825,
    1019,  1020,   781,   782,   431,  1361,   432,   782,   307,   781,
    1257,   -81,  1365,  1784,   782,   859,   971,   881,   971,  1206,
     657,  1371,   590,   781,   804,  -377,   898,  1832,   782,   404,
    1562,   688,   692,  1208,  1209,   622,  1916,   781,  -690,  1194,
     408,   532,   782,   409,   536,  1826,   972,  1393,   623,   913,
     781,   592,    14,   889,  1204,   782,  1827,   845, -1038,   972,
     972, -1042,   899,   662,   972,   972,   972,   610,  1157,   803,
    1747,  1033,    65,  1094,  1701,  1702,   574,  1541,    89,   810,
    1035,   155,   571,  1274,   544,   410,  1104,   933,   251,  1095,
    1375,   251,   433,   586,   882,  1352,  1352,   581,   545,   151,
    1352,   926,   405,   781,   589,  1616,   105,  1125,   782,   805,
    1127,   308,  1354,  1354,  1195,  1779,  1716,  1354,  1377,  1134,
    1840,  1618,  1188,   611,   228,   576,    14,   434,   172,  1037,
    1582,  1277,  1278,   972,   883,   972,  1207,  1047,   971,  1021,
     714,  1683,  1684,   625,  1355,  1355,  1785,  1627,  1640,  1355,
     532,  1595,   249,  1568,   639,  1828,   320,   643,   914,   773,
    1356,  1356,   900,  1357,  1357,  1356,   700,  1601,  1357,   308,
     599,   599,  1917,   575,   654,   704,   656,   837,  1178,  1202,
     934,  1349,   588,  1271,  1882,  1733,   929,  1342,  1366,  1367,
     251,   709,  1581,   713,  1631,  1165,  1123,   251,  1282,   251,
    1141,   251,   711,  1286,   644,  1086,  1311,  1286,  1205,  1564,
    1565,  1358,  1358,  1591,  1748,  1816,  1358,   272,  1763,  1875,
    1911,   998,  1898,   225,   231,   236,   960,   576,  1338,   781,
     960,   848,   658,   172,   782,   228,   437,   702,  1415,   722,
     783,   784,   785,   786,   577,   104,   720,   783,   784,   785,
     786,   273,  -690,   785,   786,   972,   924,   708,   783,   784,
     785,   786,  1854,  1613,   153,  1427,  1428,  1824,  1429,  1430,
    1279,   781,   783,   784,   785,   786,   782,  1230,   396,  1118,
      82,    82,  1120,   318,  1422,   777,   783,   784,   785,   786,
     326,  1231,   586,   818,   829,  1132,   180,   318,   800,   783,
     784,   785,   786,  1484,  1184,   816,  1030,   807,  1157,   173,
     907,   812,   318,   338,   449,  1200,  1423,  1312,   563,  1201,
    1148,   166,   925,   397,   831,  1492,   569,   438,   154,   172,
     327,   231,   545,   544,  1493,  1232,   242,   243,   172,  1090,
    1837,  1339,   172,  1813,   802,   536,  1907,   545,   850,   179,
    1694,   318,   783,   784,   785,   786,  1891,    83,    83,  1073,
    1164,  1359,  1360,  1233,  1777,   860,   242,   243,   320,   937,
    1928,  1744,   181,  1324,  1113,   628,  1937,  1630,   880,   319,
    1315,  1373,  1404,  1191,   955,   320,  1325,  1106,   185,  1158,
    1072,  1786,   251,   832,  1416,   174,   106,  1947,  1918,  1050,
    1098,   840,   414,  1770,  1176,  1736,   909,  1494,  1952,  1326,
    1590,  1955,  1036,   183,   908,  1938,  1335,  1316,  1956,  1936,
    1776,   167,   318,   873,   988,   251,   175,  1234,   168,   873,
     398,  1320,  1605,   323,   107,  1871,  1872,  1950,   318,   320,
     186,   176,  1541,  1114,  1438,  1051,   108,   989,   399,   825,
    1378,   936,   260,    56,   249,   272,   385,   326,   940,   941,
    1328,  1341,  1939,   781,    57,   904,  1541,  1336,   782,  1220,
     109,    58,  1749,   326,   588,   415,  1750,  1751,   783,   784,
     785,   786,   400,  1672,  1091,   781,  1379,   182,  1951,   273,
     782,   326,  1439,  1541,   240,    59,   386,   994,   318,   826,
     781,   251,  1790,   990,  1541,   782,   256,  1541,  1913,  1954,
     326,   781,  1778,  1002,  1541,   905,   782,  1221,  1673,  1835,
     783,   784,   785,   786,   781,  1149,  1000,  1262,  -239,   782,
    1674,  1004,  1344,  1755,   781,   110,   632,    82,   532,   782,
     532,  1222,  1059,   781,   239,   111,  1363,  1364,   782,   275,
    1006,  1352,  1352,   297,  1756,  1948,   781,    60,  1372,  1858,
    1757,   782,   277,  1091,  1648,  1859,  1436,   645,  1354,  1354,
     112,   107,  1039,  1070,   447,   299,   421,   646,  1743,   285,
     251,   781,   452,  1270,   586,   251,   782,   251,    61,   251,
     249,   390,    62,   300,  1758,   581,   545,  1097,  1406,  1604,
    1355,  1355,   850,   391,   172,   231,   794,   109,   795,  1112,
     422,   231,   423,   532,   304,  1089,  1356,  1356,   802,  1357,
    1357,  1191,   305,  1584,  -239,  1847,   172,   781,   172,  1848,
    1489,   306,   782,  1585,   424,   251,   392,   251,   393,  1139,
    1142,  1742,  1629,   599,  1857,  -239,  1435,   787,   781,   781,
    1394,  -239,  1288,   782,   782,  1395,    86,    87,  1143,   425,
     251,  1861,  1154,   312,  1617,  1394,  1036,  1358,  1358,  1626,
    1398,   313,   110,  1904,  1678,  1906,   317,   468,   469,   470,
    1576,    63,   111,  1910,   162,  -239,   251,   251,   333,   781,
    1470,   349,  1172,  1173,   782,  1175,   260,   798,  1177,   799,
     588,  1147,   161,   162,   722,   478,   479,   112,  1185,  1150,
    1151,  1155,   783,   784,   785,   786,   242,   243,  1162,  1467,
    1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,  1944,   172,
     260,   260,   384,   260,   783,   784,   785,   786,   382,   172,
    1897,  1468,   869,  1899,   870,  1925,  1926,   722,  1199,   783,
     784,   785,   786,  1324,   720,   891,  1471,   892,  1822,  1033,
     783,   784,   785,   786,   781,   381,  1325,  1472,  1035,   782,
     787,   388,  1031,   783,   784,   785,   786,   781,   857,  1923,
    1474,   406,   782,   783,   784,   785,   786,  1728,  1730,  1326,
    1475,   420,   783,   784,   785,   786,  1066,  1612,  1067,  1690,
    1283,   429,  1181,  1327,  1182,   783,   784,   785,   786,   417,
     586,   859,  1476,   951,   418,  1186,   781,   787,   239,  1289,
     440,   782,  1091,   442,   781,  1544,   444,  1633,  1547,   782,
     783,   784,   785,   786,  1224,   445,  1228,  1477,  1236,  1239,
    1328,  1244,  1247,  1250,  1290,   781,  1291,  1181,   448,  1340,
     782,   545,   545,  1386,   781,  1387,   533,   449,   850,   782,
     537,  1435,  1435,   540,   607,   231,   608,   688,   692,  1435,
    1435,  1324,  1435,  1435,  1729,  1731,   783,   784,   785,   786,
    1435,  1435,  1435,  1435,  1325,  1435,  1435,  1435,  1435,  1435,
    1435,  1391,  -730,  1392,   547,   548,  1885,   783,   784,   785,
     786,   551,   552,  1888,  1478,   781,  1286,  1326,  1491,   787,
     782,  1563,   787,  1033,  1649,  1033,   787,   787,  1688,  1737,
    1036,  1327,  1035,  1033,  1035,   249,   553,   554,   588,   536,
     555,   781,  1035,   570,   557,  1401,   782,   572,   783,   784,
     785,   786,   593,   573,  1384,  1479,   582,  1933,  1417,  1934,
     595,   798,  1329,  1935,  1421,  1571,  1385,  1573,  1328,   596,
     597,  1384,  1442,  1557,  1557,  1561,   781,   601,  1033,   603,
     781,   782,   251,   260,   781,   782,   251,  1035,    14,   782,
     616,   995,   615,   999,   617,   619,   260,  1003,  1005,  1007,
     637,   640,   260,   651,   650,   655,   706,  1658,   710,   747,
     712,   749,  1587,   752,  1589,   750,   769,   751,  1681,   753,
     754,   755,   781,   783,   784,   785,   786,   782,   889,   759,
    1679,   756,   757,  1548, -1024,   760,   783,   784,   785,   786,
     758,   761,   778,  1680,   532,   766,  1495,   249,   586,   780,
    1546,   767,   768,  1466,   172,   787,   793,  1549,   789,   788,
   -1042,  -720,   797,  1782,   781,   814,   796,   820,   822,   782,
     846,   823,   835,   839,  1632,   783,   784,   785,   786,   843,
    1329,  1874,  1687,   783,   784,   785,   786,   847,   251,   844,
    1870,   251,   851,   855,  1500,  1543,   334,   858,   545,   231,
     236,   545,   268,   151,   783,   784,   785,   786,  1914,  1280,
     852,   610,  1545,   783,   784,   785,   786,   861,  1281,   545,
     545,   862,  -720,  1929,   231,  -720,   864,   865,   871,   335,
     499,   896,  -720,   895,  -720,   318,  -437,   897,  -720,   902,
     910,   911,   919,  -720,   918,   916,  -720,   921,   917,   922,
     927,   931,   938,   560,   932,   336,   939, -1067,   337,   948,
     588,  -720,   568, -1067,   783,   784,   785,   786, -1067,  1689,
     956, -1067,   950,   951,   338,  -720,   957,   958,   962,   185,
    1638,  1022,  1027,  -720,  1028,  1643,   928,  -720,  1060,  1055,
     783,   784,   785,   786,  1615,   320,  -720,  1062,  1061,  1064,
     339,  1085,  1092,  1068,  1860,   805,  1102, -1067,  1107,   798,
    1109,  1119,  -720,  1128,  1129, -1067,  1130,   874,   606,  1135,
    1136,  1738,   609, -1067,  1138,   783,   784,   785,   786,   783,
     784,   785,   786,   783,   784,   785,   786,  1144,  1145,  -720,
    1146, -1067,  1153, -1067,  1160,  1693,  1174,  1179,  1170,  1193,
    1217,  -720,  1252,  -720,  1253,  1269, -1067,  1259, -1067, -1067,
   -1067,  1261,  1286,  1287,   781,  1292,  1466,   172,  1294,  1310,
     586,   783,   784,   785,   786,  1313,  1318,  1319,  1333,  1323,
    1345,  1380,  1368,  1331,   973,  1403,  1324,  1388,  1348,  1411,
     781,  1370,  1187,   971,  1390,   782,  1419,  1396,   974,  1325,
    -720,  1399,   225,  1424,  1400,   -49,  1432, -1067,  1402,  1425,
    1700,  1199,  1199,   783,   784,   785,   786,  1543,  1426,   763,
    -720,  1437,  1326,  1433,  -720,   770,   771,   774,  1440,  1483,
    1485,  1569,  1441,  1550,   975,  1443,  1327,  -720,  -720,  1552,
    -720,  1444,   976,   -49,  1841,  1451,  1452,  1453,  1454,  1456,
     977,  1457,  1458,   545,   545,   -49,  1459,  1460,  1461,  1553,
    1578,  1574,  1566,  1579,  1036,  1575,  1567,  1588,   978,   156,
     979,  1602,  1761,  1328,  1606,   231,   231,  1839,   351,   -49,
    1607,  1608,  1609,   980,  1610,   981,   982,   983,  1611,  1614,
    1628,  1157,  1634,   817,  1557,  1636,  1651,   819,  1685,  1637,
    1686,  1642,  1647,   172,  1692,  1670,  1671,  1677,  1691, -1067,
   -1067, -1067, -1067, -1067, -1067, -1067, -1030,   154,  1695,  1698,
    1704, -1067,  1720, -1067,  1727,   823,  1696,  -339,  1709,  1710,
    1711,  1712,     1,  1810,   984,  1732,  1713,  1714,  1717,  1718,
    1719,  1734,  1721,  1722,   -49,  1723,  1724,  1725,  1767,  1752,
       2,  1762,  1726,  1764,   -49,  1765,  1768,  1766,     3,  1771,
    1769,  1842,  1772,  1855,  1783,     4,  1781,     5,  1773,     6,
    1787,  1466,   172,  1819,     7,   228,  1788,  1789,  1792,   -49,
    1793,     8,  1794,  1705,  1795,  1329,  1836,  1798,  1645,  1646,
    1799,  1800,  1851,  1846,  1801,     9,  1652,  1653,  1802,  1655,
    1656,   157,  1803,  1804,  1199,    10,  1809,  1660,  1661,  1662,
    1663,  1805,  1664,  1665,  1666,  1667,  1668,  1669,  1036,  1806,
    1036,  1856,  1807,  1852,  1812,   588,  1833,  1853,  1036,  1849,
    1864,  1834,  1868,  1876,   351,   231,   985,   986,   987,   783,
     784,   785,   786,  1873,  1863,  1884,  1030,  1883,  1865,   588,
    1886,  1893,  1889,  1039,  1905,  1892,  1901,  1919,   172,  1932,
    1900,  1902,  1921,   228,   970,  1924,  1927,  1916,    11,  1917,
    1877,  1942,  1943,  1036,  1945,    12,   588,  1946,    13,  1949,
      14,    15,  1958,  1959,   996,   813,   314,   588,  1001,   315,
     588,  1554,   316,   539,   815,  1017,  1071,   588,  1420,   546,
    1024,  1025,  1063,   949,  1192,    16,  1418,  1197,  1408,  1166,
     228,  1775,   271,   620,   499,   663,  1040,  1041,  1042,  1043,
    1044,  1045,   499,   591,   912,  1103,  1347,   225,  1879,  1409,
    1774,    67,  1414,   633,  1133,   586,  1880,  1754,   635,  1915,
    1760,  1922,    68,   536,  1920,  1941,  1908,  1296,    28,    69,
    1831,  1052,  1126,  1745,    17,  1583,   568,  1171,   894,   586,
     641,   627,    70,   618,   935,  1625,  1163,   160,   583,    18,
      19,   834,   830,  1093,   331,  1087,   281,  1100,  1322,  1572,
    1890,   991,  1543,  1808,  1903,  1676,   586,  1089,    48,   606,
     251,  1111,    20,    49,  1108,    52,  1069,   586,  1212,  1317,
     586,  1213,   228,   228,  1592,   453,  1543,   586,   842,  1156,
    1447,  1487,   454,   890,  1397,  1850,   286,  1699,     0,    71,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
     456,   457,     0,  1543,  1089,    72,     0,     0,     0,     0,
       0,     0,     0,     0,  1543,     0,  1199,  1543,     0,     0,
       0,    73,     0,     0,  1543,     0,     0,     0,   458,     0,
       0,   459,     0,     0,     0,     0,    74,     0,     0,     0,
      75,     0,     0,    76,     0,     0,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,   460,     0,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
      78,     0,   464,     0,     0,     0,     0,  1210,  1211,  1216,
       0,     0,  1225,  1226,  1229,     0,  1237,  1240,  1242,  1245,
    1248,  1251,     0,     0,   456,     0,     0,     0,  1255,  1256,
       0,  1258,     0,  1260,     0,     0,  1263,  1264,  1265,  1266,
    1267,  1268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,  1272,     0,  1214,  1275,   465,     0,    79,
       0,     0,     0,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   242,   243,   114,     0,   115,   116,   117,   118,     0,
     119,   472,   499,     0,     0,     0,     0,     0,   120,   121,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   122,
     482,     0,   124,     0,     0,   483,   484,     0,   485,   486,
     487,     0,   127,   128,   488,   129,     0,     0,   130,   131,
     132,   133,   134,     0,     0,     0,   135,   136,     0,     0,
       0,   465,   137,   489,   139,     0,   490,   141,   142,     0,
     143,     0,     0,     0,   491,   492,     0,   467,     0,     0,
     559,     0,   494,     0,     0,     0,   320,     0,     0,     0,
       0,   468,   469,   470,     0,   242,   243,     0,     0,  1223,
       0,     0,  1213,     0,     0,     0,   453,     0,     0,     0,
       0,     0,     0,   454,   473,   474,   475,   476,   477,   478,
     479,   480,   481,     0,     0,   455,     0,     0,     0,   483,
     484,   456,   457,     0,     0,     0,     0,     0,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,   499,   459,     0,     0,     0,     0,     0,     0,  1350,
       0,     0,     0,     0,  1445,  1446,     0,  1448,  1450,     0,
       0,     0, -1068,     0,     0,  1455,     0,   460, -1068,   461,
     462,     0,     0, -1068,     0,     0, -1068,     0,     0,   463,
       0,     0,     0,   464,     0,  1473,     0,     0,     0,     0,
       0,     0,     0,  1480,  1481,  1272,     0,     0,     0,     0,
       0,  1272,     0,  1275,  1488,   499,  1490,     0,     0,     0,
       0,     0, -1068,     0,     0,     0,     0,     0,     0,     0,
   -1068,     0,     0,     0,     0,     0,     0,     0, -1068,     0,
       0,     0,     0,     0,     0,     0,  1214,     0,   465,     0,
       0,     0,     0,     0,   466,     0, -1068,     0, -1068,     0,
       0,     0,     0,     0,   467,   499,     0,     0,     0,     0,
       0, -1068,     0, -1068, -1068, -1068,     0,     0,   468,   469,
     470,   471,   242,   243,   114,     0,   115,   116,   117,   118,
       0,   119,   472,     0,     0,     0,     0,  1596,     0,   120,
     121,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     122,   482,     0,   124,     0,     0,   483,   484,     0,   485,
     486,   487, -1068,   127,   128,   488,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,     0,   135,   136,     0,
       0,     0,     0,   137,   489,   139,     0,   490,   141,   142,
       0,   143,   361,     0,     0,   491,   492,     0,     0,     0,
       0,   559,     0,   494,     0,  1644,     0,   320,   362,     0,
       0,  1650,     0,     0,     0,     0,  1654,     0,     0,   363,
       0,     0,     0,  1659,     0,    90,   364,     0,     0,     0,
     365,   366,    91,     0,     0,     0,    92,    93,     0,   367,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
     368,     0,     0,     0,     0,     0,    95,     0,  1272,  1272,
       0,     0,     0,     0, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068,     0,     0,     0,     0,     0, -1068,     0, -1068,     0,
    1697,  1227,     0,     0,  1213,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,   455,     0,     0,
       0,     0,   370,   456,   457,     0, -1069,     0,    96,     0,
       0,     0, -1069,     0,     0,     0,     0, -1069,     0,     0,
   -1069,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,   458,     0,   371,   459,     0,     0,   372,     0,    98,
       0,     0,  1741,    99,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,   100,     0, -1069,     0,     0,   460,
       0,   461,   462,     0, -1069,     0,     0,   374,   375,     0,
       0,   463, -1069,   101,     0,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   -1069,     0, -1069,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0, -1069,     0, -1069, -1069, -1069,
       0,     0,     0,     0,     0,  1791,   874,     0,     0,     0,
       0,  1796,  1797,     0,     0,     0,   376,     0,  1214,   377,
     465,     0,   102,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,  1811,   467,     0,     0,     0,
    1272,     0,     0,  1814,     0,     0, -1069,  1820,  1821,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1235,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,   143,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,  1862,   455,     0,   320,
       0,     0,     0,   456,   457,     0,     0,     0, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069,     0,     0,     0,     0,     0,
   -1069,     0, -1069,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   113,     0,     0,
     114,     0,   115,   116,   117,   118,     0,   119,     0,     0,
       0,     0,     0,     0,     0,   120,   121,     0,     0,   460,
    1741,   461,   462,     0,     0,     0,   122,   123,     0,   124,
       0,   463,     0,     0,     0,   464,   125,   126,     0,   127,
     128,     0,   129,     0,     0,   130,   131,   132,   133,   134,
       0,     0,     0,   135,   136,     0,     0,  1909,     0,   137,
     138,   139,     0,   140,   141,   142,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1214,     0,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1238,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,   143,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,     0,   455,     0,   320,
       0,     0,     0,   456,   457,     0,   973,     0,     0,     0,
       0,     0,   781,     0,     0,     0,     0,   782,     0,     0,
     974,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   975,     0,     0,   460,
       0,   461,   462,     0,   976,     0,     0,     0,     0,     0,
       0,   463,   977,     0,     0,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     978,     0,   979,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   980,     0,   981,   982,   983,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,  1214,     0,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   984,     0,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1243,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,   143,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,     0,   455,     0,   320,
       0,     0,     0,   456,   457,     0,     0,     0,   985,   986,
     987,   783,   784,   785,   786,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   113,     0,     0,
     114,     0,   115,   116,   117,   118,     0,   119,     0,     0,
       0,     0,     0,     0,     0,   120,   121,     0,     0,   460,
       0,   461,   462,     0,     0,     0,   122,   123,     0,   124,
       0,   463,     0,     0,     0,   464,   125,   126,     0,   127,
     128,     0,   129,     0,     0,   130,   131,   132,   133,   134,
       0,     0,     0,   135,   136,     0,     0,     0,     0,   137,
     138,   139,     0,   140,   141,   142,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   638,
       0,     0,     0,     0,     0,     0,     0,     0,  1214,     0,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1246,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,   143,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,     0,   455,     0,   320,
       0,     0,     0,   456,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   113,     0,     0,
     114,     0,   115,   116,   117,   118,     0,   119,     0,     0,
       0,     0,     0,     0,     0,   120,   121,     0,     0,   460,
       0,   461,   462,     0,     0,     0,   122,   123,     0,   124,
       0,   463,     0,     0,     0,   464,   125,   126,     0,   127,
     128,     0,   129,     0,     0,   130,   131,   132,   133,   134,
       0,     0,     0,   135,   136,     0,     0,     0,     0,   137,
     138,   139,     0,   140,   141,   142,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1214,  1048,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1249,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,   143,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,     0,   455,     0,   320,
       0,     0,     0,   456,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,     0,   113,     0,
       0,   114,     0,   115,   116,   117,   118,     0,   119,     0,
       0,     0,     0,     0,     0,     0,   120,   121,     0,   460,
       0,   461,   462,     0,     0,     0,     0,   122,   123,     0,
     124,   463,     0,     0,     0,   464,     0,   125,   126,     0,
     127,   128,     0,   129,     0,     0,   130,   131,   132,   133,
     134,     0,     0,     0,   135,   136,     0,     0,     0,     0,
     137,   138,   139,     0,   140,   141,   142,     0,   143,     0,
       0,     0,     0,     0,  1049,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1214,     0,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,   988,     0,     0,     0,     0,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,   989,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,   453,   483,   484,
       0,   485,   486,   487,   454,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,   455,     0,     0,   135,
     136,     0,   456,   457,     0,   137,   489,   139,     0,   490,
     141,   142,   990,   143,     0,     0,     0,   491,   492,     0,
       0,   724,     0,   559,     0,   494,     0,     0,     0,   320,
     458,     0,     0,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,     0,   460,     0,
     461,   462,     0,     0,   120,   121,     0,     0,     0,     0,
     463,     0,     0,     0,   464,   122,   123,     0,   124,     0,
       0,     0,   963,     0,     0,   125,   126,     0,   127,   128,
       0,   129,     0,     0,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,     0,     0,     0,     0,   137,   138,
     139,     0,   140,   141,   142,     0,   143,     0,     0,    14,
       0,     0,     0,   726,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   242,   243,   114,     0,   115,   116,   117,
     118,     0,   119,   472,     0,     0,     0,     0,     0,     0,
     120,   121,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   122,   482,     0,   124,     0,     0,   483,   484,     0,
     485,   486,   487,     0,   127,   128,   488,   129,     0,     0,
     130,   131,   132,   133,   134,   964,   965,   966,   135,   136,
       0,     0,     0,     0,   137,   489,   139,   453,   490,   141,
     142,     0,   143,     0,   454,     0,   491,   492,     0,     0,
       0,     0,   730,    14,   494,     0,   455,     0,   320,   723,
       0,     0,   456,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   724,     0,     0,     0,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,     0,     0,     0,
       0,     0,     0,     0,   120,   121,     0,     0,   460,     0,
     461,   462,     0,     0,     0,   122,   123,     0,   124,     0,
     463,     0,     0,     0,   464,   125,   126,     0,   127,   128,
       0,   129,   725,     0,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,     0,     0,     0,     0,   137,   138,
     139,     0,   140,   141,   142,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1074,     0,     0,     0,
       0,     0,     0,   726,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   242,   243,   114,     0,   115,   116,   117,
     118,     0,   119,   472,     0,     0,     0,     0,     0,     0,
     120,   121,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   122,   482,     0,   124,     0,   453,   483,   484,     0,
     485,   486,   487,   454,   127,   128,   488,   129,     0,     0,
     130,   131,   727,   728,   729,   455,     0,     0,   135,   136,
       0,   456,   457,     0,   137,   489,   139,     0,   490,   141,
     142,     0,   143,     0,     0,     0,   491,   492,   541,     0,
     724,     0,   730,     0,   494,     0,     0,     0,   320,   458,
       0,     0,   459,     0,     0,   113,     0,     0,   114,     0,
     115,   116,   117,   118,     0,   119,     0,     0,     0,     0,
       0,     0,     0,   120,   121,     0,     0,   460,     0,   461,
     462,     0,     0,     0,   122,   123,     0,   124,     0,   463,
       0,     0,     0,   464,   125,   126,     0,   127,   128,     0,
     129,   959,     0,   130,   131,   132,   133,   134,     0,     0,
       0,   135,   136,     0,     0,     0,     0,   137,   138,   139,
       0,   140,   141,   142,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   726,     0,     0,     0,     0,     0,   465,     0,
       0,     0,     0,     0,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   242,   243,   114,     0,   115,   116,   117,   118,
       0,   119,   472,     0,     0,     0,     0,     0,     0,   120,
     121,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     122,   482,     0,   124,     0,   453,   483,   484,     0,   485,
     486,   487,   454,   127,   128,   488,   129,     0,     0,   130,
     131,   727,   728,   729,   455,     0,     0,   135,   136,     0,
     456,   457,     0,   137,   489,   139,     0,   490,   141,   142,
       0,   143,     0,     0,     0,   491,   492,     0,     0,   724,
       0,   730,     0,   494,     0,     0,     0,   320,   458,     0,
       0,   459,     0,     0,   113,     0,     0,   114,     0,   115,
     116,   117,   118,     0,   119,     0,     0,     0,     0,     0,
       0,     0,   120,   121,     0,     0,   460,     0,   461,   462,
       0,     0,     0,   122,   123,     0,   124,     0,   463,     0,
       0,     0,   464,   125,   126,     0,   127,   128,     0,   129,
     725,     0,   130,   131,   132,   133,   134,     0,     0,     0,
     135,   136,     0,     0,     0,     0,   137,   138,   139,     0,
     140,   141,   142,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1032,     0,   494,     0,     0,     0,
       0,   726,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     471,   242,   243,   114,     0,   115,   116,   117,   118,     0,
     119,   472,     0,     0,     0,     0,     0,     0,   120,   121,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   122,
     482,     0,   124,     0,   453,   483,   484,     0,   485,   486,
     487,   454,   127,   128,   488,   129,     0,     0,   130,   131,
     727,   728,   729,   455,     0,     0,   135,   136,     0,   456,
     457,     0,   137,   489,   139,     0,   490,   141,   142,     0,
     143,     0,     0,     0,   491,   492,     0,     0,   724,     0,
     730,     0,   494,     0,     0,     0,   320,   458,     0,     0,
     459,     0,     0,   113,     0,     0,   114,     0,   115,   116,
     117,   118,     0,   119,     0,     0,     0,     0,     0,     0,
       0,   120,   121,     0,     0,   460,     0,   461,   462,     0,
       0,     0,   122,   123,     0,   124,     0,   463,     0,     0,
       0,   464,   125,   126,     0,   127,   128,     0,   129,   959,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,     0,     0,     0,     0,   137,   138,   139,     0,   140,
     141,   142,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
     726,     0,     0,     0,     0,     0,   465,     0,     0,     0,
       0,     0,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     242,   243,   114,     0,   115,   116,   117,   118,     0,   119,
     472,     0,     0,     0,     0,     0,     0,   120,   121,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   122,   482,
       0,   124,     0,   453,   483,   484,     0,   485,   486,   487,
     454,   127,   128,   488,   129,     0,     0,   130,   131,   132,
     133,   134,   455,     0,     0,   135,   136,     0,   456,   457,
       0,   137,   489,   139,     0,   490,   141,   142,     0,   143,
       0,     0,     0,   491,   492,     0,     0,     0,     0,   730,
       0,   494,     0,     0,     0,   320,   458,     0,     0,   459,
       0,     0,   113,     0,     0,   114,     0,   115,   116,   117,
     118,     0,   119,     0,     0,     0,     0,     0,     0,     0,
     120,   121,     0,     0,   460,     0,   461,   462,     0,     0,
       0,   122,   123,     0,   124,     0,   463,     0,     0,     0,
     464,   125,   126,     0,   127,   128,     0,   129,     0,     0,
     130,   131,   132,   133,   134,     0,     0,     0,   135,   136,
       0,     0,     0,     0,   137,   138,   139,     0,   140,   141,
     142,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1074,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   242,
     243,   114,     0,   115,   116,   117,   118,     0,   119,   472,
       0,     0,     0,     0,     0,     0,   120,   121,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   122,   482,     0,
     124,     0,   453,   483,   484,     0,   485,   486,   487,   454,
     127,   128,   488,   129,     0,     0,   130,   131,   132,   133,
     134,   455,     0,     0,   135,   136,     0,   456,   457,     0,
     137,   489,   139,     0,   490,   141,   142,     0,   143,     0,
       0,     0,   491,   492,   564,     0,     0,     0,   559,     0,
     494,     0,     0,     0,   320,   458,     0,     0,   459,     0,
       0,   113,     0,     0,   114,     0,   115,   116,   117,   118,
       0,   119,     0,     0,     0,     0,     0,     0,     0,   120,
     121,     0,     0,   460,     0,   461,   462,     0,     0,     0,
     122,   123,     0,   124,     0,   463,     0,     0,     0,   464,
     125,   126,     0,   127,   128,     0,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,     0,   135,   136,     0,
       0,     0,     0,   137,   138,   139,     0,   140,   141,   142,
       0,   143,     0,     0,     0,     0,     0,     0,     0,  -340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   465,     0,     0,     0,     0,     0,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   762,     0,
       0,     0,     0,     0,   468,   469,   470,   471,   242,   243,
     114,     0,   115,   116,   117,   118,     0,   119,   472,     0,
       0,     0,     0,     0,     0,   120,   121,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   122,   482,     0,   124,
       0,   453,   483,   484,     0,   485,   486,   487,   454,   127,
     128,   488,   129,     0,     0,   130,   131,   132,   133,   134,
     455,     0,     0,   135,   136,     0,   456,   457,     0,   137,
     489,   139,     0,   490,   141,   142,     0,   143,     0,     0,
       0,   491,   492,     0,     0,     0,     0,   559,     0,   494,
       0,     0,     0,   320,   458,     0,     0,   459,    82,     0,
     113,     0,     0,   114,     0,   115,   116,   117,   118,     0,
     119,     0,     0,     0,     0,     0,     0,     0,   120,   121,
       0,     0,   460,     0,   461,   462,     0,     0,     0,   122,
     123,     0,   124,     0,   463,     0,     0,     0,   464,   125,
     126,     0,   127,   128,     0,   129,     0,     0,   130,   131,
     132,   133,   134,     0,     0,     0,   135,   136,     0,     0,
       0,     0,   137,   138,   139,     0,   140,   141,   142,     0,
     143,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   468,   469,   470,   471,   242,   243,   114,
       0,   115,   116,   117,   118,     0,   119,   472,     0,     0,
       0,     0,     0,     0,   120,   121,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   122,   482,     0,   124,     0,
     453,   483,   484,     0,   485,   486,   487,   454,   127,   128,
     488,   129,     0,     0,   130,   131,   132,   133,   134,   455,
       0,     0,   135,   136,     0,   456,   457,     0,   137,   489,
     139,     0,   490,   141,   142,     0,   143,     0,     0,     0,
     491,   492,     0,     0,     0,     0,   559,     0,   494,     0,
       0,     0,   320,   458,     0,     0,   459,     0,     0,   113,
       0,     0,   114,     0,   115,   116,   117,   118,     0,   119,
       0,     0,     0,     0,     0,     0,     0,   120,   121,     0,
       0,   460,     0,   461,   462,     0,     0,     0,   122,   123,
       0,   124,     0,   463,     0,     0,     0,   464,   125,   126,
       0,   127,   128,     0,   129,     0,     0,   130,   131,   132,
     133,   134,     0,     0,     0,   135,   136,     0,     0,     0,
       0,   137,   138,   139,     0,   140,   141,   142,     0,   143,
       0,     0,     0,     0,     0,  1049,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,     0,     0,     0,     0,     0,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   469,   470,   471,   242,   243,   114,     0,
     115,   116,   117,   118,     0,   119,   472,     0,     0,     0,
       0,     0,     0,   120,   121,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   122,   482,     0,   124,     0,   453,
     483,   484,     0,   485,   486,   487,   454,   127,   128,   488,
     129,     0,     0,   130,   131,   132,   133,   134,   455,     0,
       0,   135,   136,     0,   456,   457,     0,   137,   489,   139,
       0,   490,   141,   142,     0,   143,     0,     0,     0,   491,
     492,     0,     0,     0,     0,   559,  1046,   494,     0,     0,
       0,   320,   458,     0,     0,   459,     0,     0,   113,     0,
       0,   114,     0,   115,   116,   117,   118,     0,   119,     0,
       0,     0,     0,     0,     0,     0,   120,   121,     0,     0,
     460,     0,   461,   462,     0,     0,     0,   122,   123,     0,
     124,     0,   463,     0,     0,     0,   464,   125,   126,     0,
     127,   128,     0,   129,     0,     0,   130,   131,   132,   133,
     134,     0,     0,     0,   135,   136,     0,     0,     0,     0,
     137,   138,   139,     0,   140,   141,   142,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   466,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1241,     0,
       0,   468,   469,   470,   471,   242,   243,   114,     0,   115,
     116,   117,   118,     0,   119,   472,     0,     0,     0,     0,
       0,     0,   120,   121,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   122,   482,     0,   124,     0,   453,   483,
     484,     0,   485,   486,   487,   454,   127,   128,   488,   129,
       0,     0,   130,   131,   132,   133,   134,   455,     0,     0,
     135,   136,     0,   456,   457,     0,   137,   489,   139,     0,
     490,   141,   142,     0,   143,     0,     0,     0,   491,   492,
       0,     0,     0,     0,   559,     0,   494,     0,     0,     0,
     320,   458,     0,     0,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   460,
       0,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,   463,     0,     0,     0,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     465,     0,     0,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1449,     0,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,   453,   483,   484,
       0,   485,   486,   487,   454,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,   455,     0,     0,   135,
     136,     0,   456,   457,     0,   137,   489,   139,     0,   490,
     141,   142,     0,   143,     0,     0,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,     0,     0,     0,   320,
     458,     0,     0,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   460,     0,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   466,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   468,
     469,   470,   471,   242,   243,   114,     0,   115,   116,   117,
     118,     0,   119,   472,     0,     0,     0,     0,     0,     0,
     120,   121,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   122,   482,     0,   124,     0,   453,   483,   484,     0,
     485,   486,   487,   454,   127,   128,   488,   129,     0,     0,
     130,   131,   132,   133,   134,   455,     0,     0,   135,   136,
       0,   456,   457,     0,   137,   489,   139,     0,   490,   141,
     142,     0,   143,     0,     0,     0,   491,   492,     0,     0,
       0,     0,   493,     0,   494,     0,     0,     0,   320,   458,
       0,     0,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   464,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   465,     0,
       0,     0,     0,     0,   466,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,   109,     0,     0,     0,     0,     0,     0,   468,   469,
     470,   471,   242,   243,   114,     0,   115,   116,   117,   118,
       0,   119,   472,     0,     0,     0,     0,     0,     0,   120,
     121,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     122,   482,     0,   124,     0,     0,   483,   484,     0,   485,
     486,   487,     0,   127,   128,   488,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,   110,   135,   136,     0,
    1294,     0,     0,   137,   489,   139,   111,   490,   141,   142,
       0,   143,     0,     0,     0,   491,   492,     0,     0,     0,
       0,   559,     0,   494,     0,     0,     0,   320,     6,     0,
       0,   112,     0,     0,     0,  -294,     0,     0,     0,  1501,
    1502,     0,  1503,     0,     0,     0,     0,     0,     0,   113,
       0,     0,   114,     0,   115,   116,   117,   118,  1504,   119,
       0,     0,     0,     0,  1505,     0,     0,   120,   121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   122,   123,
       0,   124,     0,     0,     0,     0,     0,     0,   125,   126,
       0,   127,   128,     0,   129,     0,     0,   130,   131,   132,
     133,   134,     0,     0,     0,   135,   136,     0,  1294,     0,
       0,   137,   138,   139,     0,   140,   141,   142,     0,   143,
       0,  1506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,     0,    14,
       0,     0,     0,  -296,     0,     0,     0,  1501,  1502,     0,
    1503,     0,     0,     0,  1507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,     0,  1504,     0,     0,     0,
       0,     0,  1505,  1705,     0,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,   114,     0,   115,   116,   117,
     118,     0,   119,     0,     0,     0,     0,     0,     0,     0,
     120,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1508,   123,     0,   124,     0,     0,     0,     0,     0,
       0,   125,   126,     0,   127,   128,     0,   129,     0,  1506,
     130,  1509,   132,   133,   134,     0,     0,     0,   135,   136,
    1510,  1511,  1512,  1294,   137,   138,   139,    14,   140,   141,
     142,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1507,     0,     0,     0,     0,     0,     0,     0,
       0,     6,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1501,  1502,     0,  1503,     0,     0,     0,     0,
     471,     0,     0,   114,     0,   115,   116,   117,   118,     0,
     119,  1504,     0,     0,     0,     0,     0,  1505,   120,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1508,
     123,     0,   124,     0,     0,     0,     0,     0,     0,   125,
     126,     0,   127,   128,     0,   129,     0,     0,   130,  1509,
     132,   133,   134,     0,     0,     0,   135,   136,  1510,  1511,
    1512,     0,   137,   138,   139,     0,   140,   141,   142,     0,
     143,     0,     0,     0,  1506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,   114,     0,
     115,   116,   117,   118,     0,   119,   664,     0,     0,   665,
       0,     0,     0,   120,   121,   874,     0,     0,     0,     0,
       0,     0,     0,     0,  1508,   123,   666,   124,     0,   667,
       0,     0,     0,     0,   125,   126,     0,   127,   128,     0,
     129,     0,     0,   130,  1509,   132,   133,   134,     0,     0,
       0,   135,   136,  1510,  1511,  1512,     0,   137,   138,   139,
       0,   140,   141,   142,     0,   143,     0,     0,     0,   668,
       0,     0,     0,     0,     0,   669,   670,   671,     0,   672,
     673,   674,     0,   675,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   676,     0,   677,   664,     0,     0,   665,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   666,     0,     0,   667,     0,     0,
       0,     0,   678,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   679,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   668,     0,     0,
       0,   680,     0,   669,   670,   671,     0,   672,   673,   674,
       0,   675,     0,     0,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,     0,     0,   676,
       0,   677,     0,     0,   120,   121,   681,   682,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,   124,     0,
       0,     0,     0,   683,     0,   125,   126,     0,   127,   128,
     678,   129,     0,     0,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,     0,     0,     0,   679,   137,   138,
     139,     0,   140,   141,   142,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   680,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,   114,     0,   115,
     116,   117,   118,     0,   119,   952,     0,     0,   665,     0,
       0,     0,   120,   121,   681,   682,     0,     0,     0,     0,
       0,   953,     0,   122,   123,   666,   124,     0,   667,     0,
       0,   683,     0,   125,   126,     0,   127,   128,     0,   129,
       0,     0,   130,   131,   132,   133,   134,     0,     0,   664,
     135,   136,   665,     0,     0,     0,   137,   138,   139,     0,
     140,   141,   142,     0,   143,     0,     0,     0,   668,   666,
       0,     0,   667,     0,   669,   670,   671,     0,   672,   673,
     674,     0,   675,     0,     0,     0,  1297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     676,     0,   677,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   668,     0,     0,     0,     0,     0,   669,   670,
     671,     0,   672,   673,   674,     0,   675,     0,     0,     0,
       0,   678,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   676,     0,   677,     0,   679,     0,
       0,     0,     0,     0,     0,     0,  1298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     680,     0,     0,     0,     0,   678,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1299,   679,     0,     0,     0,  1300,     0,     0,     0,
       0,     0,  1301,     0,     0,   681,   682,     0,     0,     0,
       0,     0,     0,   113,   680,     0,   114,     0,   115,   116,
     117,  1302,   683,   119,     0,     0,     0,     0,     0,     0,
       0,   120,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   123,     0,   124,     0,     0,     0,   681,
     682,     0,   125,   126,     0,   127,   128,     0,   129,     0,
       0,   130,   131,   132,   133,   134,   683,     0,     0,   135,
     136,     0,     0,     0,     0,   137,   138,   139,     0,   140,
     141,   142,   113,   143,     0,   114,   189,   115,   116,   117,
     118,   190,   119,   191,   192,   193,   194,   195,   196,   197,
     120,   121,   198,   199,   200,   201,   202,     0,     0,   203,
     204,   122,   123,   205,   124,     0,     0,   206,   207,   208,
     209,   125,   126,   210,   127,   128,   211,   129,   212,   213,
     130,   131,   132,   133,   134,     0,     0,     0,   135,   136,
     214,   215,   216,   217,   137,   138,   139,   218,   140,   141,
     142,   219,   143
};

static const short yycheck[] =
{
       9,     0,     4,    69,   112,   171,    16,    16,    59,    18,
     601,    68,    68,   161,   162,   398,    75,   778,    67,   161,
     162,    61,   100,    78,   762,    91,   778,   430,   430,   426,
     768,  1032,   528,   400,    74,   307,    63,   606,   634,   841,
      99,   422,   493,   570,    95,  1056,   101,   993,    57,    58,
      59,    60,    61,    62,    63,  1294,  1120,  1293,    97,    68,
      69,    70,   548,    72,    60,    74,    75,    94,    77,    78,
      79,  1367,    73,  1120,   407,   102,    72,   748,    87,    89,
      89,    90,    91,  1029,    93,    94,    95,    96,    98,    98,
      99,   100,   101,   102,   322,   104,    97,   153,   176,   155,
      96,   789,   773,   436,  1074,  1120,   549,   173,   104,   349,
    1161,   718,   406,   796,   154,   380,   711,  1183,  1127,   946,
    1312,  1120,   493,   650,  1120,   258,    21,   180,  1271,     8,
     626,   851,   628,  1365,  1277,   175,   883,     8,   634,   166,
      10,   725,     8,     8,   153,   154,   155,     8,     8,     8,
     159,  1332,  1061,     6,    47,    63,   844,   166,    90,   267,
    1574,    61,   171,    19,   173,   174,   175,   176,   584,  1178,
      43,   180,  1120,    47,    26,   224,     8,   133,   174,  1359,
    1360,  1516,    44,   349,  1364,    10,    61,   141,   559,    43,
      80,    23,   725,    46,     8,  1633,    28,   730,    23,     9,
      50,    51,    23,    28,   253,  1124,    55,    28,   174,    23,
      10,   149,  1131,    97,    28,   612,     8,    62,     8,     8,
      43,  1140,   355,    23,   192,    22,    44,    56,    28,     3,
    1313,   634,   634,   971,   972,    30,    44,    23,    63,    19,
       4,   297,    28,     7,   300,    55,   125,  1166,    43,   169,
      23,   359,   155,   636,   125,    28,    66,   590,   280,   125,
     125,   283,    80,   660,   125,   125,   125,    90,   196,   541,
    1587,  1032,   193,   142,  1510,  1511,    26,  1516,    63,   182,
    1032,    18,   333,  1021,   556,    49,   847,   185,   297,   158,
     180,   300,   141,   349,   139,  1359,  1360,   337,   307,   308,
    1364,   684,    76,    23,   353,  1388,   260,   868,    28,   277,
     871,   277,  1359,  1360,    94,  1632,   191,  1364,   180,   880,
    1734,   118,   278,   255,   333,   177,   155,   176,   337,   780,
    1339,    50,    51,   125,   179,   125,   125,   788,     8,   189,
     278,  1484,  1485,   399,  1359,  1360,   230,  1398,   204,  1364,
     406,  1362,   408,  1323,   410,   165,   284,   413,   278,   730,
    1359,  1360,   180,  1359,  1360,  1364,   432,  1599,  1364,   277,
     379,   380,   180,   123,   423,   434,   425,   277,   939,   963,
     278,  1119,   548,   173,   277,  1566,   281,  1107,  1135,  1136,
     399,   440,  1338,   444,  1405,   922,   269,   406,   277,   408,
     896,   410,   442,   277,   413,   821,    63,   277,   279,  1318,
    1319,  1359,  1360,   279,   279,   269,  1364,    11,   279,   279,
     279,   274,  1860,   432,   433,   434,   959,   177,  1099,    23,
     963,    94,   255,   442,    28,   444,    14,   433,  1185,   448,
     272,   273,   274,   275,   194,   141,   448,   272,   273,   274,
     275,    45,   277,   274,   275,   125,    21,    90,   272,   273,
     274,   275,  1758,  1382,    18,  1203,  1204,  1703,  1206,  1207,
     189,    23,   272,   273,   274,   275,    28,    17,     4,   862,
     196,   196,   865,   196,   230,   494,   272,   273,   274,   275,
       6,    31,   548,   279,   143,   878,   244,   196,   538,   272,
     273,   274,   275,   173,   947,   558,   279,   547,   196,    55,
     652,   551,   196,   162,   190,   958,   262,   174,   323,   962,
     903,    68,    87,    49,   117,  1286,   331,   105,    82,   538,
      46,   540,   541,   805,  1286,    75,   199,   200,   547,   223,
    1732,  1102,   551,  1686,   540,   601,  1881,   556,   597,   558,
     246,   196,   272,   273,   274,   275,  1852,   273,   273,   279,
     273,  1122,  1123,   103,  1630,   614,   199,   200,   284,   702,
    1905,  1582,   122,    64,    98,   869,   117,  1404,   634,   278,
    1066,  1142,  1177,   950,  1191,   284,    77,   852,   126,   277,
     818,  1642,   601,   186,  1185,   141,     6,  1932,  1894,  1287,
     840,   277,     3,  1612,   937,  1575,   655,  1290,  1943,   100,
    1348,  1946,   778,    61,   653,   156,   143,  1068,  1953,  1915,
    1629,   168,   196,   632,    85,   634,   172,   167,   175,   638,
     156,  1074,  1370,   278,    44,  1815,  1816,    43,   196,   284,
     178,   187,  1881,   167,    46,   793,    56,   108,   174,   223,
      44,   700,   653,    37,   710,    11,     6,     6,   707,   708,
     151,  1104,   203,    23,    48,   223,  1905,   194,    28,    46,
      80,    55,  1591,     6,   840,    76,  1595,  1596,   272,   273,
     274,   275,   208,    79,   826,    23,    80,   276,    94,    45,
      28,     6,    94,  1932,   147,    79,    46,    46,   196,   273,
      23,   710,  1648,   164,  1943,    28,     7,  1946,  1886,  1945,
       6,    23,  1631,    46,  1953,   273,    28,    94,   114,  1720,
     272,   273,   274,   275,    23,   223,   753,   279,    22,    28,
     126,    46,  1113,   118,    23,   145,    30,   196,   794,    28,
     796,   118,   798,    23,   753,   155,  1129,  1130,    28,   196,
      46,  1815,  1816,   278,   139,  1933,    23,   141,  1141,  1770,
     145,    28,    65,   905,  1435,  1776,  1217,     5,  1815,  1816,
     180,    44,   781,   813,   277,   140,    98,    15,  1580,    65,
     789,    23,   285,    63,   840,   794,    28,   796,   172,   798,
     846,     4,   176,   278,   179,   835,   805,   837,  1179,  1368,
    1815,  1816,   851,    16,   813,   814,   277,    80,   279,   858,
     132,   820,   134,   869,   122,   824,  1815,  1816,   814,  1815,
    1816,  1188,   277,   231,   118,  1744,   835,    23,   837,  1748,
    1281,   122,    28,   241,   156,   844,    49,   846,    51,   895,
     896,  1579,  1403,   852,  1763,   139,  1217,   277,    23,    23,
     277,   145,   282,    28,    28,   282,    59,    60,   897,   181,
     869,  1780,   911,   124,  1391,   277,  1032,  1815,  1816,  1396,
     282,   124,   145,  1874,  1470,  1876,   174,   195,   196,   197,
    1331,   265,   155,  1884,   232,   179,   895,   896,    79,    23,
    1257,   156,   931,   932,    28,   934,   897,   277,   938,   279,
    1066,   902,   231,   232,   913,   223,   224,   180,   948,   130,
     131,   913,   272,   273,   274,   275,   199,   200,   919,   279,
     210,   211,   212,   213,   214,   215,   216,   217,  1929,   938,
     931,   932,    19,   934,   272,   273,   274,   275,   192,   948,
    1859,   279,   277,  1862,   279,  1901,  1902,   956,   957,   272,
     273,   274,   275,    64,   956,   203,   279,   205,  1696,  1720,
     272,   273,   274,   275,    23,   180,    77,   279,  1720,    28,
     277,   259,   279,   272,   273,   274,   275,    23,   174,  1898,
     279,   278,    28,   272,   273,   274,   275,  1552,  1553,   100,
     279,   199,   272,   273,   274,   275,   277,  1380,   279,  1495,
    1027,    10,   277,   114,   279,   272,   273,   274,   275,   266,
    1066,  1408,   279,   277,   162,   279,    23,   277,  1027,   279,
      59,    28,  1164,    61,    23,  1297,    79,  1410,  1300,    28,
     272,   273,   274,   275,   978,   198,   980,   279,   982,   983,
     151,   985,   986,   987,   277,    23,   279,   277,    78,   279,
      28,  1060,  1061,   277,    23,   279,   149,   190,  1107,    28,
     172,  1432,  1433,   141,   385,  1074,   387,  1470,  1470,  1440,
    1441,    64,  1443,  1444,  1552,  1553,   272,   273,   274,   275,
    1451,  1452,  1453,  1454,    77,  1456,  1457,  1458,  1459,  1460,
    1461,   277,    85,   279,    82,   278,  1834,   272,   273,   274,
     275,    63,    61,  1841,   279,    23,   277,   100,   279,   277,
      28,   279,   277,  1874,   279,  1876,   277,   277,   279,   279,
    1286,   114,  1874,  1884,  1876,  1181,    61,   122,  1294,  1185,
     122,    23,  1884,   174,   244,  1174,    28,   102,   272,   273,
     274,   275,   123,   188,  1153,   279,   172,   277,  1187,   279,
     156,   277,   263,   279,  1193,  1326,  1158,  1328,   151,    65,
      83,  1170,    54,  1311,  1312,  1313,    23,   121,  1929,   243,
      23,    28,  1181,  1174,    23,    28,  1185,  1929,   155,    28,
     255,   749,   254,   751,   254,    30,  1187,   755,   756,   757,
     207,   277,  1193,   269,   192,   112,    29,    54,   278,   135,
      10,   278,  1344,   199,  1346,   278,    97,   278,    61,   278,
     278,   278,    23,   272,   273,   274,   275,    28,  1601,   199,
     279,   278,   278,  1301,   278,   199,   272,   273,   274,   275,
     278,   278,   148,   279,  1290,   278,  1293,  1293,  1294,   281,
    1299,   278,   278,  1253,  1253,   277,   269,  1302,   283,   278,
     283,     0,    10,  1636,    23,   141,   278,    63,   277,    28,
     278,    10,   277,   166,  1406,   272,   273,   274,   275,   269,
     263,    82,   279,   272,   273,   274,   275,    10,  1287,   283,
     279,  1290,    83,   277,  1293,  1294,    84,    90,  1297,  1298,
    1299,  1300,  1301,  1302,   272,   273,   274,   275,  1889,   277,
     283,    90,  1298,   272,   273,   274,   275,    90,   277,  1318,
    1319,    22,    61,    82,  1323,    64,    22,    43,    10,   117,
     296,   208,    71,   174,    73,   196,   174,   136,    77,   136,
      21,    90,   278,    82,   138,   185,    85,    91,   185,   278,
     281,   278,    61,   319,   278,   143,    10,    17,   146,   122,
    1516,   100,   328,    23,   272,   273,   274,   275,    28,   277,
     128,    31,   149,   277,   162,   114,   121,   278,   278,   126,
    1419,   189,    61,   122,   279,  1424,    21,   126,   174,    42,
     272,   273,   274,   275,  1386,   284,   135,   202,   174,    60,
     188,   190,   186,   278,  1777,   277,    10,    67,    83,   277,
     240,   278,   151,    19,   278,    75,    43,    27,   384,    86,
      86,  1577,   388,    83,    22,   272,   273,   274,   275,   272,
     273,   274,   275,   272,   273,   274,   275,   125,   125,   178,
     125,   101,   156,   103,   154,  1501,   278,    98,   156,   278,
     278,   190,   108,   192,   278,    63,   116,   279,   118,   119,
     120,   277,   277,   283,    23,   184,  1466,  1466,    16,   124,
    1516,   272,   273,   274,   275,   107,    63,    63,    71,   277,
      12,    10,   239,   278,    17,    10,    64,   160,   278,   192,
      23,   278,   133,     8,   279,    28,    52,   280,    31,    77,
     239,   279,  1501,    52,   279,     6,   278,   167,   279,   279,
    1509,  1510,  1511,   272,   273,   274,   275,  1516,   279,   485,
     259,    63,   100,   278,   263,   491,   492,   493,   278,    51,
     173,   127,   278,   192,    67,   278,   114,   276,   277,   277,
     279,   278,    75,    44,   122,   278,   278,   278,   278,   278,
      83,   278,   278,  1552,  1553,    56,   278,   278,   278,   277,
      19,    85,   279,    73,  1720,    85,   279,   130,   101,    70,
     103,   145,  1602,   151,   125,  1574,  1575,  1733,  1577,    80,
     125,   125,   125,   116,   125,   118,   119,   120,   125,   279,
     279,   196,    22,   559,  1732,   278,    63,   563,    51,   279,
     173,   279,   279,  1602,    36,   279,   277,   279,   276,   269,
     270,   271,   272,   273,   274,   275,    36,    82,   276,   276,
      51,   281,    82,   283,    70,    10,   278,   276,   276,   276,
     276,   276,     7,  1674,   167,   174,   276,   276,   276,   276,
     276,    85,   276,   276,   145,   276,   276,   276,    44,    23,
      25,    79,   280,   180,   155,    80,    80,   180,    33,   279,
      44,   239,   279,  1761,   111,    40,   124,    42,    94,    44,
     111,  1671,  1671,    61,    49,  1674,   279,   279,   279,   180,
     279,    56,   279,   189,   279,   263,   124,   279,  1432,  1433,
     279,   279,    94,   277,   279,    70,  1440,  1441,   279,  1443,
    1444,   202,   279,   279,  1703,    80,   278,  1451,  1452,  1453,
    1454,   279,  1456,  1457,  1458,  1459,  1460,  1461,  1874,   279,
    1876,  1762,   279,    86,   279,  1881,  1715,  1757,  1884,   279,
     204,   278,    79,    82,  1733,  1734,   269,   270,   271,   272,
     273,   274,   275,   278,  1783,    82,   279,   246,  1787,  1905,
     278,   122,   278,  1752,   173,  1853,   277,   122,  1757,    47,
     279,   278,   122,  1762,   730,   279,   279,    44,   143,   180,
    1826,   279,    50,  1929,    36,   150,  1932,    47,   153,   201,
     155,   156,     0,     0,   750,   554,   158,  1943,   754,   158,
    1946,  1309,   159,   305,   556,   761,   814,  1953,  1191,   308,
     766,   767,   805,   714,   951,   180,  1188,   956,  1180,   926,
    1809,  1620,    79,   396,   780,   427,   782,   783,   784,   785,
     786,   787,   788,   357,   660,   846,  1116,  1826,  1827,  1181,
    1619,    37,  1185,   408,   879,  1881,  1828,  1599,   408,  1892,
    1601,  1896,    48,  1889,  1895,  1917,  1882,  1057,     0,    55,
    1707,   794,   869,  1583,   229,  1341,   822,   929,   640,  1905,
     413,   401,    68,   394,   697,  1394,   921,    13,   341,   244,
     245,   576,   572,   835,   164,   822,    89,   843,  1074,  1327,
    1846,   747,  1881,  1671,  1873,  1466,  1932,  1886,     0,   855,
    1889,   857,   267,     0,   855,     0,   812,  1943,     6,  1069,
    1946,     9,  1901,  1902,  1350,    13,  1905,  1953,   584,   913,
    1232,  1279,    20,   638,  1170,  1752,    93,  1509,    -1,   125,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,  1932,  1933,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1943,    -1,  1945,  1946,    -1,    -1,
      -1,   157,    -1,    -1,  1953,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
     176,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   187,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
     206,    -1,   110,    -1,    -1,    -1,    -1,   973,   974,   975,
      -1,    -1,   978,   979,   980,    -1,   982,   983,   984,   985,
     986,   987,    -1,    -1,    38,    -1,    -1,    -1,   994,   995,
      -1,   997,    -1,   999,    -1,    -1,  1002,  1003,  1004,  1005,
    1006,  1007,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,  1019,    -1,   163,  1022,   165,    -1,   265,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,  1068,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,   165,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,   272,   273,    -1,   181,    -1,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,   195,   196,   197,    -1,   199,   200,    -1,    -1,     6,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    20,   218,   219,   220,   221,   222,   223,
     224,   225,   226,    -1,    -1,    32,    -1,    -1,    -1,   233,
     234,    38,    39,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,  1217,    69,    -1,    -1,    -1,    -1,    -1,    -1,   273,
      -1,    -1,    -1,    -1,  1230,  1231,    -1,  1233,  1234,    -1,
      -1,    -1,    17,    -1,    -1,  1241,    -1,    94,    23,    96,
      97,    -1,    -1,    28,    -1,    -1,    31,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,  1261,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1269,  1270,  1271,    -1,    -1,    -1,    -1,
      -1,  1277,    -1,  1279,  1280,  1281,  1282,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,   101,    -1,   103,    -1,
      -1,    -1,    -1,    -1,   181,  1331,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,   119,   120,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,  1363,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,   167,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    21,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,  1431,    -1,   284,    37,    -1,
      -1,  1437,    -1,    -1,    -1,    -1,  1442,    -1,    -1,    48,
      -1,    -1,    -1,  1449,    -1,    48,    55,    -1,    -1,    -1,
      59,    60,    55,    -1,    -1,    -1,    59,    60,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    79,    -1,  1484,  1485,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,   273,   274,
     275,    -1,    -1,    -1,    -1,    -1,   281,    -1,   283,    -1,
    1506,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    32,    -1,    -1,
      -1,    -1,   141,    38,    39,    -1,    17,    -1,   141,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,   172,    69,    -1,    -1,   176,    -1,   172,
      -1,    -1,  1578,   176,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    67,    -1,    -1,    94,
      -1,    96,    97,    -1,    75,    -1,    -1,   206,   207,    -1,
      -1,   106,    83,   206,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,  1651,    27,    -1,    -1,    -1,
      -1,  1657,  1658,    -1,    -1,    -1,   265,    -1,   163,   268,
     165,    -1,   265,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1681,   181,    -1,    -1,    -1,
    1686,    -1,    -1,  1689,    -1,    -1,   167,  1693,  1694,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,  1782,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,   269,   270,
     271,   272,   273,   274,   275,    -1,    -1,    -1,    -1,    -1,
     281,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
    1846,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,  1883,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    94,
      -1,    96,    97,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    83,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,   119,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,   269,   270,
     271,   272,   273,   274,   275,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    39,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,   227,   228,    -1,
     230,   106,    -1,    -1,    -1,   110,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,   108,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,   164,   268,    -1,    -1,    -1,   272,   273,    -1,
      -1,    57,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    94,    -1,
      96,    97,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,   227,   228,    -1,   230,    -1,
      -1,    -1,   118,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,   155,
      -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    13,   264,   265,
     266,    -1,   268,    -1,    20,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,   155,   280,    -1,    32,    -1,   284,    35,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
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
     266,    -1,   268,    -1,    -1,    -1,   272,   273,   181,    -1,
      57,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
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
      -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,    57,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
     118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,
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
     268,    -1,    -1,    -1,   272,   273,    -1,    -1,    57,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,   118,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   272,   273,   274,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    66,    -1,    -1,    69,   196,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,   273,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
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
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,   272,
     273,    -1,    -1,    -1,    -1,   278,   279,   280,    -1,    -1,
      -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
     234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
     254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,   272,   273,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,
     284,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,   145,   254,   255,    -1,
      16,    -1,    -1,   260,   261,   262,   155,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    44,    -1,
      -1,   180,    -1,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    74,   208,
      -1,    -1,    -1,    -1,    80,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    16,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,   155,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      58,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    80,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,   137,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,    16,   260,   261,   262,   155,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    74,    -1,    -1,    -1,    -1,    -1,    80,   216,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,   256,   257,
     258,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    18,    -1,    -1,    21,
      -1,    -1,    -1,   216,   217,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   227,   228,    38,   230,    -1,    41,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    18,    -1,    -1,    21,    -1,
      -1,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    34,    -1,   227,   228,    38,   230,    -1,    41,    -1,
      -1,   235,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    18,
     254,   255,    21,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    81,    38,
      -1,    -1,    41,    -1,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   161,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,   218,   219,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   183,    -1,   201,    -1,   203,   204,
     205,   206,   235,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,   218,
     219,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,   235,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,   198,   268,    -1,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,    -1,   225,
     226,   227,   228,   229,   230,    -1,    -1,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268
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
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 415:
{ yyval = yyvsp[0]; }
    break;
case 416:
{ yyval = yyvsp[0]; }
    break;
case 417:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 419:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 420:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 421:
{ yyval = yyvsp[0]; }
    break;
case 422:
{ yyval = yyvsp[0]; }
    break;
case 423:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 424:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 425:
{ yyval = yyvsp[0]; }
    break;
case 426:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 428:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 429:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 430:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 431:
{ yyval = yyvsp[0]; }
    break;
case 432:
{ yyval = yyvsp[0]; }
    break;
case 433:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 434:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 435:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 436:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 469:
{ yyval = NULL; }
    break;
case 470:
{ yyval = NULL; }
    break;
case 471:
{ yyval = NULL; }
    break;
case 472:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 473:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 474:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 475:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 476:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 477:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 478:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 479:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 480:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 481:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 482:
{ yyval = NULL; }
    break;
case 484:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 485:
{ yyval = yyvsp[0]; }
    break;
case 486:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 487:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 488:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 489:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 490:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 492:
{ yyval = NULL; }
    break;
case 494:
{ yyval = NULL; }
    break;
case 495:
{ yyval = yyvsp[0]; }
    break;
case 496:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 501:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 502:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 503:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 505:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 506:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 507:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 508:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 513:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 514:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 516:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 517:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 518:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 524:
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
case 525:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 527:
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
case 528:
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
case 529:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 536:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 537:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 538:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 539:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 540:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 541:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 557:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 558:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 559:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 560:
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
case 561:
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
case 564:
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
case 565:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 566:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 567:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 568:
{ yyval = yyvsp[-1]; }
    break;
case 569:
{ yyval = 0; }
    break;
case 573:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 574:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 575:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 579:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 580:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 581:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 582:
{ yyval = 0; }
    break;
case 583:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 586:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 587:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 590:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 591:
{ yyval = NULL; }
    break;
case 593:
{ yyval = NULL; }
    break;
case 594:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 596:
{ yyval = NULL; }
    break;
case 598:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 603:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 604:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 605:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 606:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 607:
{ yyval = yyvsp[0];}
    break;
case 609:
{ yyval = yyvsp[0];}
    break;
case 610:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 613:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 614:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 615:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 616:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 617:
{ yyval = 0; }
    break;
case 618:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 619:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 620:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 621:
{ yyval = (dsql_nod*) 0; }
    break;
case 622:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 623:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 625:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 626:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 627:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 628:
{ yyval = 0; }
    break;
case 630:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 631:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 632:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;
case 633:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 634:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;
case 635:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;
case 636:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;
case 637:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 638:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;
case 639:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 640:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;
case 641:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;
case 642:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;
case 643:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;
case 644:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 645:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 646:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;
case 647:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;
case 648:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;
case 649:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;
case 650:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 651:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 652:
{ yyval = yyvsp[0]; }
    break;
case 654:
{ yyval = NULL; }
    break;
case 655:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 656:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 657:
{ yyval = NULL; }
    break;
case 658:
{ yyval = yyvsp[0]; }
    break;
case 659:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 660:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 661:
{ yyval = NULL; }
    break;
case 662:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 663:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 664:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 667:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 669:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
case 701:
{ yyval = yyvsp[-1]; }
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
case 712:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 713:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 714:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 717:
{ yyval = yyvsp[0]; }
    break;
case 718:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 719:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 720:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 721:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 722:
{ yyval = NULL; }
    break;
case 724:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 725:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 726:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 727:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 728:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 729:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 730:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 733:
{ yyval = make_list (yyvsp[0]); }
    break;
case 734:
{ yyval = NULL; }
    break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 738:
{ yyval = yyvsp[0]; }
    break;
case 739:
{ yyval = NULL; }
    break;
case 740:
{ yyval = yyvsp[0]; }
    break;
case 741:
{ yyval = NULL; }
    break;
case 742:
{ yyval = yyvsp[0]; }
    break;
case 743:
{ yyval = NULL; }
    break;
case 744:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 745:
{ yyval = 0; }
    break;
case 746:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 747:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 748:
{ yyval = 0; }
    break;
case 749:
{ yyval = 0; }
    break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 752:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 756:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 757:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 758:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 761:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 762:
{ yyval = 0; }
    break;
case 763:
{ yyval = make_list (yyvsp[0]); }
    break;
case 764:
{ yyval = 0; }
    break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 768:
{ yyval = 0; }
    break;
case 769:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 770:
{ yyval = 0; }
    break;
case 771:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 772:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 773:
{ yyval = yyvsp[-1]; }
    break;
case 774:
{ yyval = 0; }
    break;
case 775:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 777:
{ yyval = NULL; }
    break;
case 778:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 779:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 782:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 783:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 787:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 790:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 792:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 793:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 797:
{ yyval = NULL; }
    break;
case 800:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 801:
{ yyval = yyvsp[0]; }
    break;
case 802:
{ yyval = NULL; }
    break;
case 803:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 806:
{ yyval = NULL; }
    break;
case 807:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 809:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 811:
{ yyval = NULL; }
    break;
case 812:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 817:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 818:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 825:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 827:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 833:
{ yyval = yyvsp[-1]; }
    break;
case 834:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 846:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 848:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 850:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 854:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 855:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 856:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 857:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 858:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 859:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 860:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 861:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 862:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 863:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 864:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 865:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 866:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 867:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 868:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 869:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 872:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 873:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 874:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 875:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 876:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 877:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 878:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 879:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 880:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 881:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 882:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 883:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 884:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 885:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 886:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 887:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 888:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 889:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 890:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 891:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 892:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 893:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 894:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 895:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 896:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 897:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 899:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 900:
{ yyval = yyvsp[-1]; }
    break;
case 911:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 912:
{ yyval = yyvsp[0]; }
    break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 914:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 915:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 916:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 917:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 918:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 919:
{ yyval = yyvsp[-1]; }
    break;
case 920:
{ yyval = yyvsp[-1]; }
    break;
case 924:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 925:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 926:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 929:
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
case 930:
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
case 931:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 932:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 934:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 936:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 937:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 938:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 939:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 940:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 941:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 943:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
    break;
case 946:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 947:
{ yyval = make_parameter (); }
    break;
case 948:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 949:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 950:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 951:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 952:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 953:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 954:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 955:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 956:
{ yyval = yyvsp[0]; }
    break;
case 957:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 959:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 960:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 961:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 962:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 963:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 965:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 966:
{ yyval = yyvsp[0];}
    break;
case 970:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 971:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 972:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 973:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 974:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 975:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 976:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 977:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 978:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 979:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 980:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 982:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 984:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 985:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 986:
{ yyval = yyvsp[0]; }
    break;
case 987:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 988:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 989:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 990:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 993:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 994:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 995:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 998:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 999:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1000:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1001:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1002:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1003:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1004:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1005:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1009:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 1010:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1011:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1012:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1013:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1014:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1015:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1016:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1018:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1023:
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

