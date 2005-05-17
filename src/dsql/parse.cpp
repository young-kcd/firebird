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
	//bool log_defined, cache_defined;
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
    1451,  1453,  1456,  1459,  1462,  1464,  1466,  1468,  1469,  1472,
    1475,  1480,  1485,  1491,  1492,  1494,  1497,  1500,  1505,  1509,
    1512,  1515,  1523,  1525,  1526,  1528,  1529,  1532,  1535,  1538,
    1541,  1544,  1547,  1550,  1553,  1556,  1560,  1563,  1566,  1569,
    1572,  1574,  1576,  1578,  1580,  1585,  1591,  1593,  1597,  1599,
    1603,  1605,  1608,  1610,  1612,  1614,  1616,  1618,  1620,  1622,
    1624,  1626,  1628,  1630,  1635,  1640,  1647,  1653,  1657,  1658,
    1661,  1664,  1665,  1669,  1670,  1675,  1677,  1683,  1688,  1690,
    1695,  1697,  1700,  1703,  1705,  1707,  1709,  1712,  1715,  1718,
    1721,  1722,  1726,  1732,  1734,  1736,  1739,  1743,  1745,  1748,
    1752,  1753,  1755,  1757,  1759,  1765,  1771,  1778,  1780,  1782,
    1784,  1787,  1792,  1794,  1795,  1801,  1803,  1804,  1808,  1811,
    1813,  1814,  1817,  1818,  1820,  1821,  1825,  1827,  1828,  1830,
    1833,  1835,  1837,  1839,  1841,  1844,  1847,  1849,  1852,  1856,
    1858,  1860,  1864,  1868,  1870,  1873,  1877,  1879,  1882,  1883,
    1886,  1888,  1890,  1891,  1893,  1895,  1897,  1901,  1904,  1908,
    1909,  1911,  1915,  1920,  1926,  1933,  1941,  1943,  1945,  1947,
    1949,  1951,  1953,  1956,  1958,  1960,  1962,  1964,  1966,  1968,
    1971,  1973,  1975,  1977,  1980,  1982,  1984,  1988,  1992,  1993,
    1996,  1997,  2000,  2001,  2005,  2009,  2013,  2015,  2020,  2025,
    2027,  2037,  2038,  2039,  2040,  2041,  2045,  2048,  2050,  2051,
    2055,  2061,  2065,  2068,  2074,  2077,  2079,  2081,  2083,  2085,
    2087,  2091,  2093,  2097,  2099,  2100,  2103,  2105,  2109,  2111,
    2113,  2115,  2117,  2121,  2128,  2130,  2131,  2135,  2136,  2138,
    2142,  2144,  2146,  2148,  2153,  2159,  2165,  2167,  2169,  2172,
    2177,  2182,  2185,  2189,  2190,  2192,  2195,  2197,  2199,  2202,
    2205,  2208,  2209,  2211,  2212,  2216,  2217,  2219,  2223,  2225,
    2228,  2229,  2232,  2233,  2236,  2237,  2242,  2244,  2247,  2249,
    2251,  2252,  2254,  2258,  2261,  2263,  2265,  2268,  2270,  2275,
    2279,  2281,  2285,  2290,  2291,  2295,  2296,  2298,  2302,  2306,
    2308,  2310,  2311,  2313,  2315,  2320,  2321,  2324,  2329,  2330,
    2339,  2345,  2347,  2349,  2357,  2362,  2364,  2366,  2375,  2381,
    2386,  2388,  2392,  2396,  2398,  2406,  2414,  2420,  2424,  2425,
    2427,  2429,  2431,  2434,  2435,  2437,  2439,  2441,  2442,  2446,
    2448,  2452,  2454,  2455,  2459,  2461,  2465,  2467,  2471,  2475,
    2477,  2479,  2483,  2485,  2488,  2490,  2494,  2498,  2500,  2503,
    2507,  2511,  2515,  2519,  2521,  2525,  2528,  2530,  2532,  2534,
    2536,  2538,  2540,  2542,  2544,  2546,  2548,  2550,  2554,  2558,
    2562,  2566,  2570,  2574,  2578,  2582,  2589,  2596,  2603,  2610,
    2617,  2624,  2631,  2638,  2645,  2652,  2659,  2666,  2673,  2680,
    2687,  2694,  2696,  2698,  2704,  2711,  2717,  2724,  2728,  2733,
    2739,  2746,  2750,  2755,  2759,  2764,  2768,  2773,  2778,  2784,
    2789,  2794,  2798,  2803,  2805,  2807,  2809,  2811,  2813,  2815,
    2817,  2821,  2825,  2827,  2829,  2831,  2833,  2835,  2837,  2839,
    2841,  2843,  2845,  2848,  2851,  2855,  2859,  2863,  2867,  2871,
    2875,  2879,  2883,  2885,  2887,  2889,  2891,  2895,  2897,  2899,
    2901,  2903,  2905,  2907,  2912,  2914,  2918,  2920,  2923,  2925,
    2927,  2929,  2931,  2933,  2935,  2937,  2940,  2943,  2946,  2948,
    2950,  2952,  2954,  2956,  2958,  2960,  2962,  2964,  2966,  2969,
    2971,  2974,  2976,  2978,  2980,  2982,  2984,  2987,  2989,  2991,
    2993,  2995,  3000,  3006,  3012,  3018,  3024,  3030,  3036,  3042,
    3048,  3054,  3060,  3062,  3069,  3071,  3076,  3084,  3087,  3088,
    3093,  3097,  3104,  3106,  3108,  3115,  3124,  3131,  3133,  3135,
    3140,  3147,  3152,  3158,  3162,  3168,  3173,  3179,  3181,  3183,
    3185,  3190,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3215,  3216,  3218,  3219,  3221,  3223,  3225,  3227,  3229,
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
       0,   259,     0,   239,     0,   263,     0,   267,     0,   268,
       0,     7,     0,     7,   207,     0,   494,   458,     0,   640,
     456,     0,   640,     0,   205,     0,   203,     0,     0,   717,
       3,     0,   717,    76,     0,   716,   266,   192,   679,     0,
     716,   266,   192,   223,     0,   716,   266,   192,   273,   223,
       0,     0,   487,     0,   486,   487,     0,     4,   338,     0,
       4,   255,    90,   673,     0,    49,   255,    90,     0,    16,
     254,     0,    51,   254,     0,   725,   463,   489,   467,   458,
     490,   459,     0,   464,     0,     0,   468,     0,     0,    49,
     492,     0,    55,   713,     0,    79,   717,     0,   141,   721,
       0,   172,   724,     0,   176,   725,     0,   187,   728,     0,
      60,   714,     0,    48,   712,     0,    59,    65,   705,     0,
     157,   677,     0,   206,   722,     0,    68,   716,     0,   265,
     716,     0,   494,     0,   495,     0,   498,     0,   501,     0,
     499,   281,   496,   282,     0,   506,   281,   496,   282,   504,
       0,   497,     0,   496,   277,   497,     0,   679,     0,   679,
     280,   679,     0,   499,     0,   506,   504,     0,   505,     0,
     510,     0,   513,     0,   235,     0,   500,     0,   161,     0,
      38,     0,   218,     0,   219,     0,    81,     0,    92,     0,
      18,   503,   502,   504,     0,    18,   278,   678,   279,     0,
      18,   278,   678,   277,   674,   279,     0,    18,   278,   277,
     674,   279,     0,   154,   160,   678,     0,     0,   169,   674,
       0,   169,   706,     0,     0,    21,   156,   707,     0,     0,
     509,   278,   677,   279,     0,   509,     0,   509,   185,   278,
     677,   279,     0,   508,   278,   677,   279,     0,   508,     0,
     507,   278,   677,   279,     0,   183,     0,    21,   185,     0,
      87,   185,     0,    21,     0,    87,     0,   115,     0,   113,
      21,     0,   113,    87,     0,    95,   511,     0,   512,   511,
       0,     0,   278,   679,   279,     0,   278,   679,   277,   679,
     279,     0,    41,     0,    88,     0,    91,   514,     0,    93,
      91,   514,     0,   144,     0,    89,   138,     0,   278,   675,
     279,     0,     0,   528,     0,   516,     0,   545,     0,   156,
      68,   716,   174,   679,     0,   156,    68,   716,   174,   223,
       0,   156,    68,   716,   174,   273,   223,     0,   518,     0,
     519,     0,   521,     0,   244,   729,     0,   245,   244,   729,
     520,     0,   123,     0,     0,   153,   525,   174,   522,   729,
       0,   244,     0,     0,    25,   525,   526,     0,   153,   525,
       0,   193,     0,     0,   147,   527,     0,     0,   162,     0,
       0,   156,   175,   529,     0,   530,     0,     0,   531,     0,
     530,   531,     0,   532,     0,   533,     0,   534,     0,   538,
       0,   143,   123,     0,   143,   194,     0,   188,     0,   117,
     188,     0,    84,   102,   535,     0,   535,     0,   536,     0,
     143,   177,   537,     0,   143,    26,   537,     0,   162,     0,
     162,   172,     0,   162,   172,   166,     0,   186,     0,   117,
     186,     0,     0,   146,   541,     0,   158,     0,   142,     0,
       0,   143,     0,   194,     0,   542,     0,   541,   277,   542,
       0,   544,   543,     0,    61,   539,   540,     0,     0,   592,
       0,   544,   277,   592,     0,   156,   168,    79,   717,     0,
     267,   122,   547,    83,   551,     0,   267,   122,   548,   720,
      83,   551,     0,   267,   122,   549,   720,   550,    83,   551,
       0,    37,     0,    48,     0,   172,     0,   187,     0,   141,
       0,   176,     0,    59,    65,     0,    60,     0,    55,     0,
      68,     0,   265,     0,    79,     0,   206,     0,    21,   156,
       0,   268,     0,   207,     0,   133,     0,   283,   720,     0,
     673,     0,    94,     0,   556,   553,   555,     0,    61,   180,
     554,     0,     0,   121,   635,     0,     0,   192,   243,     0,
       0,   559,   609,   615,     0,   559,   609,   615,     0,   559,
     609,   615,     0,   560,     0,   559,   178,   703,   560,     0,
     559,   178,     6,   560,     0,   561,     0,   155,   566,   569,
     570,   574,   599,   595,   598,   600,     0,     0,     0,     0,
       0,   567,   568,   563,     0,   567,   563,     0,   568,     0,
       0,   231,   680,   562,     0,   231,   278,   662,   279,   562,
       0,   231,   669,   562,     0,   232,   680,     0,   232,   278,
     563,   662,   279,     0,   232,   669,     0,    46,     0,   702,
       0,   571,     0,   274,     0,   572,     0,   571,   277,   572,
       0,   662,     0,   662,   573,   718,     0,    10,     0,     0,
      63,   575,     0,   576,     0,   575,   277,   576,     0,   582,
       0,   577,     0,   589,     0,   578,     0,   278,   582,   279,
       0,   278,   556,   279,   573,   579,   580,     0,   723,     0,
       0,   278,   581,   279,     0,     0,   718,     0,   581,   277,
     718,     0,   583,     0,   584,     0,   585,     0,   576,   263,
      85,   577,     0,   576,   114,   593,    85,   577,     0,   576,
     593,    85,   576,   586,     0,   587,     0,   588,     0,   122,
     642,     0,   239,   278,   635,   279,     0,   721,   590,   573,
     723,     0,   721,   590,     0,   278,   665,   279,     0,     0,
     592,     0,   724,   723,     0,   724,     0,    77,     0,   100,
     594,     0,   151,   594,     0,    64,   594,     0,     0,   127,
       0,     0,    71,    19,   596,     0,     0,   597,     0,   596,
     277,   597,     0,   662,     0,    73,   642,     0,     0,   190,
     642,     0,     0,   135,   601,     0,     0,   602,   278,   603,
     279,     0,    85,     0,   164,   108,     0,   108,     0,   164,
       0,     0,   604,     0,   604,   277,   603,     0,   605,   606,
       0,   601,     0,   724,     0,   724,   605,     0,   114,     0,
      79,   278,   607,   279,     0,   126,   717,   608,     0,   717,
       0,   717,   277,   607,     0,    79,   278,   607,   279,     0,
       0,   126,    19,   610,     0,     0,   611,     0,   610,   277,
     611,     0,   662,   612,   614,     0,    11,     0,    45,     0,
       0,   231,     0,   241,     0,   240,   564,   613,   565,     0,
       0,   259,   662,     0,   259,   662,   174,   662,     0,     0,
      80,    82,   592,   636,   182,   278,   665,   279,     0,    80,
      82,   592,   636,   556,     0,   618,     0,   619,     0,    44,
      63,   591,   599,   600,   609,   615,     0,    44,    63,   591,
     623,     0,   621,     0,   622,     0,   180,   591,   156,   624,
     599,   600,   609,   615,     0,   180,   591,   156,   624,   623,
       0,   190,    35,   121,   711,     0,   625,     0,   624,   277,
     625,     0,   641,   269,   662,     0,   688,     0,   143,    18,
     640,    63,   592,   628,   631,     0,    80,    18,   640,    82,
     592,   628,   631,     0,    60,    63,   629,   174,   629,     0,
      60,   174,   629,     0,     0,   630,     0,   669,     0,   674,
       0,   107,   632,     0,     0,   678,     0,   669,     0,   634,
       0,     0,   278,   635,   279,     0,   640,     0,   635,   277,
     640,     0,   637,     0,     0,   278,   638,   279,     0,   641,
       0,   638,   277,   641,     0,   640,     0,   723,   283,   709,
       0,   723,   283,   274,     0,   709,     0,   640,     0,   723,
     283,   709,     0,   658,     0,   118,   658,     0,   644,     0,
     642,   125,   642,     0,   642,     8,   642,     0,   644,     0,
     118,   658,     0,   643,   125,   642,     0,   643,     8,   642,
       0,   659,   125,   642,     0,   659,     8,   642,     0,   645,
       0,   278,   643,   279,     0,   118,   644,     0,   646,     0,
     649,     0,   650,     0,   651,     0,   652,     0,   657,     0,
     647,     0,   655,     0,   653,     0,   654,     0,   656,     0,
     662,   269,   662,     0,   662,   270,   662,     0,   662,   271,
     662,     0,   662,    67,   662,     0,   662,   101,   662,     0,
     662,   119,   662,     0,   662,   120,   662,     0,   662,   116,
     662,     0,   662,   269,     6,   278,   557,   279,     0,   662,
     270,     6,   278,   557,   279,     0,   662,   271,     6,   278,
     557,   279,     0,   662,    67,     6,   278,   557,   279,     0,
     662,   101,     6,   278,   557,   279,     0,   662,   119,     6,
     278,   557,   279,     0,   662,   120,     6,   278,   557,   279,
       0,   662,   116,     6,   278,   557,   279,     0,   662,   269,
     648,   278,   557,   279,     0,   662,   270,   648,   278,   557,
     279,     0,   662,   271,   648,   278,   557,   279,     0,   662,
      67,   648,   278,   557,   279,     0,   662,   101,   648,   278,
     557,   279,     0,   662,   119,   648,   278,   557,   279,     0,
     662,   120,   648,   278,   557,   279,     0,   662,   116,   648,
     278,   557,   279,     0,   163,     0,     9,     0,   662,    83,
      46,    63,   662,     0,   662,    83,   118,    46,    63,   662,
       0,   662,    17,   662,     8,   662,     0,   662,   118,    17,
     662,     8,   662,     0,   662,   103,   662,     0,   662,   118,
     103,   662,     0,   662,   103,   662,    54,   662,     0,   662,
     118,   103,   662,    54,   662,     0,   662,    75,   660,     0,
     662,   118,    75,   660,     0,   662,    31,   662,     0,   662,
     118,    31,   662,     0,   662,   167,   662,     0,   662,   118,
     167,   662,     0,   662,   167,   192,   662,     0,   662,   118,
     167,   192,   662,     0,    57,   278,   556,   279,     0,   159,
     278,   556,   279,     0,   662,    83,    94,     0,   662,    83,
     118,    94,     0,   248,     0,   249,     0,   250,     0,   251,
       0,   252,     0,   253,     0,   661,     0,   278,   665,   279,
       0,   278,   557,   279,     0,   639,     0,   664,     0,   681,
       0,   668,     0,   669,     0,   432,     0,   689,     0,   690,
       0,   700,     0,   688,     0,   273,   662,     0,   272,   662,
       0,   662,   272,   662,     0,   662,    28,   662,     0,   662,
      23,   708,     0,   662,   273,   662,     0,   662,   274,   662,
       0,   662,   275,   662,     0,   278,   662,   279,     0,   278,
     558,   279,     0,   670,     0,   671,     0,   672,     0,    39,
       0,   723,   283,    39,     0,    97,     0,   663,     0,   704,
       0,   220,     0,   221,     0,   222,     0,   639,   281,   665,
     282,     0,   662,     0,   665,   277,   662,     0,   668,     0,
     273,   667,     0,   197,     0,   196,     0,   195,     0,   223,
       0,   224,     0,   667,     0,   673,     0,    38,   199,     0,
     218,   199,     0,   219,   199,     0,   284,     0,   181,     0,
     225,     0,   226,     0,   233,     0,   234,     0,    66,     0,
     165,     0,   242,     0,   199,     0,   200,   199,     0,   675,
       0,   273,   676,     0,   196,     0,   196,     0,   675,     0,
     196,     0,   680,     0,   273,   680,     0,   196,     0,   682,
       0,   683,     0,   685,     0,    32,   278,   274,   279,     0,
      32,   278,   702,   662,   279,     0,    32,   278,    46,   662,
     279,     0,   171,   278,   702,   662,   279,     0,   171,   278,
      46,   662,   279,     0,    13,   278,   702,   662,   279,     0,
      13,   278,    46,   662,   279,     0,   110,   278,   702,   662,
     279,     0,   110,   278,    46,   662,   279,     0,   106,   278,
     702,   662,   279,     0,   106,   278,    46,   662,   279,     0,
     684,     0,   209,   278,   701,    63,   662,   279,     0,   686,
       0,    96,   278,   662,   279,     0,   228,   278,   662,    63,
     662,   687,   279,     0,    61,   662,     0,     0,   705,   278,
     665,   279,     0,   705,   278,   279,     0,    20,   278,   662,
      10,   378,   279,     0,   691,     0,   692,     0,   237,   278,
     662,   277,   662,   279,     0,   261,   278,   642,   277,   662,
     277,   662,   279,     0,   238,   278,   662,   277,   665,   279,
       0,   693,     0,   695,     0,   236,   698,   694,    51,     0,
     236,   698,   694,    50,   699,    51,     0,   189,   697,   173,
     699,     0,   694,   189,   697,   173,   699,     0,   236,   696,
      51,     0,   236,   696,    50,   699,    51,     0,   189,   642,
     173,   699,     0,   696,   189,   642,   173,   699,     0,   662,
       0,   662,     0,   662,     0,   264,    97,    61,   716,     0,
      69,   278,   716,   277,   662,   279,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   216,
       0,   217,     0,     6,     0,     0,    46,     0,     0,    94,
       0,   198,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   730,     0,   730,     0,   730,     0,   730,     0,   730,
       0,   198,     0,   731,     0,   201,     0,   203,     0,   204,
       0,   205,     0,   206,     0,   208,     0,   227,     0,   230,
       0,   228,     0,   238,     0,   241,     0,   247,     0,   243,
       0,   237,     0,   240,     0,   246,     0,   248,     0,   249,
       0,   250,     0,   260,     0,   254,     0,   255,     0,   261,
       0,   262,     0,   216,     0,   217,     0,   265,     0,   264,
       0,   266,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   544,   546,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   578,   582,   586,   591,   592,   595,
     597,   599,   603,   604,   608,   612,   614,   616,   618,   620,
     624,   626,   630,   632,   636,   643,   647,   651,   656,   658,
     662,   664,   668,   669,   672,   673,   675,   677,   681,   683,
     685,   687,   691,   692,   699,   701,   703,   707,   708,   712,
     716,   717,   721,   723,   730,   734,   736,   741,   747,   748,
     750,   756,   758,   759,   763,   764,   770,   775,   777,   779,
     781,   785,   786,   790,   793,   798,   800,   802,   804,   807,
     812,   818,   820,   827,   831,   833,   836,   838,   840,   842,
     844,   846,   848,   850,   852,   854,   859,   863,   865,   867,
     875,   879,   883,   885,   891,   898,   903,   908,   913,   921,
     923,   927,   929,   930,   936,   942,   944,   946,   950,   952,
     956,   958,   962,   964,   967,   968,   975,   993,   995,   999,
    1001,  1005,  1007,  1010,  1011,  1014,  1019,  1020,  1023,  1027,
    1036,  1043,  1051,  1056,  1057,  1060,  1066,  1068,  1071,  1072,
    1076,  1078,  1080,  1082,  1084,  1088,  1090,  1093,  1094,  1098,
    1103,  1105,  1154,  1211,  1215,  1216,  1219,  1220,  1223,  1225,
    1229,  1230,  1231,  1234,  1235,  1236,  1242,  1247,  1252,  1254,
    1256,  1260,  1261,  1265,  1266,  1273,  1277,  1281,  1290,  1296,
    1297,  1300,  1302,  1307,  1309,  1314,  1320,  1326,  1330,  1336,
    1338,  1342,  1343,  1344,  1345,  1346,  1347,  1350,  1352,  1355,
    1356,  1360,  1365,  1367,  1372,  1373,  1375,  1383,  1387,  1389,
    1392,  1393,  1394,  1395,  1398,  1402,  1406,  1413,  1420,  1425,
    1431,  1434,  1436,  1438,  1440,  1444,  1447,  1451,  1454,  1457,
    1460,  1469,  1480,  1490,  1500,  1510,  1512,  1516,  1518,  1522,
    1523,  1527,  1533,  1534,  1538,  1543,  1545,  1547,  1551,  1553,
    1557,  1558,  1562,  1566,  1567,  1570,  1575,  1577,  1581,  1583,
    1587,  1592,  1593,  1596,  1600,  1602,  1604,  1608,  1609,  1613,
    1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,  1622,  1623,
    1624,  1625,  1626,  1627,  1629,  1631,  1632,  1633,  1634,  1637,
    1639,  1643,  1647,  1651,  1656,  1660,  1664,  1666,  1670,  1674,
    1680,  1685,  1690,  1691,  1692,  1694,  1698,  1702,  1704,  1708,
    1710,  1712,  1717,  1720,  1724,  1725,  1729,  1734,  1735,  1739,
    1741,  1743,  1745,  1749,  1750,  1751,  1754,  1758,  1762,  1766,
    1809,  1814,  1816,  1818,  1822,  1824,  1826,  1832,  1840,  1842,
    1846,  1847,  1851,  1857,  1864,  1887,  1891,  1897,  1901,  1907,
    1911,  1919,  1930,  1941,  1943,  1945,  1949,  1953,  1955,  1959,
    1961,  1963,  1965,  1967,  1969,  1971,  1973,  1975,  1977,  1979,
    1981,  1983,  1985,  1987,  1991,  1993,  1997,  2003,  2007,  2009,
    2016,  2018,  2020,  2023,  2026,  2029,  2033,  2037,  2043,  2044,
    2048,  2057,  2061,  2063,  2065,  2067,  2069,  2073,  2074,  2078,
    2080,  2082,  2084,  2090,  2093,  2095,  2099,  2107,  2108,  2109,
    2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,  2119,
    2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,  2128,  2129,
    2130,  2131,  2132,  2133,  2134,  2135,  2136,  2137,  2138,  2139,
    2142,  2144,  2148,  2150,  2155,  2161,  2163,  2165,  2169,  2171,
    2175,  2178,  2181,  2189,  2195,  2196,  2200,  2214,  2216,  2218,
    2220,  2234,  2244,  2245,  2249,  2250,  2256,  2260,  2262,  2264,
    2266,  2268,  2270,  2272,  2274,  2276,  2278,  2280,  2282,  2284,
    2291,  2292,  2295,  2296,  2299,  2304,  2311,  2312,  2316,  2325,
    2331,  2332,  2335,  2336,  2337,  2338,  2355,  2360,  2365,  2384,
    2401,  2408,  2409,  2416,  2421,  2428,  2435,  2444,  2448,  2454,
    2458,  2462,  2468,  2472,  2479,  2485,  2491,  2499,  2504,  2509,
    2516,  2517,  2518,  2521,  2522,  2525,  2526,  2527,  2534,  2538,
    2549,  2555,  2609,  2671,  2672,  2679,  2692,  2697,  2702,  2709,
    2711,  2718,  2719,  2720,  2724,  2727,  2730,  2738,  2739,  2740,
    2743,  2747,  2751,  2753,  2757,  2761,  2762,  2765,  2769,  2773,
    2774,  2777,  2779,  2783,  2784,  2788,  2792,  2793,  2797,  2798,
    2802,  2803,  2804,  2805,  2808,  2810,  2814,  2816,  2820,  2822,
    2825,  2827,  2829,  2833,  2835,  2837,  2841,  2843,  2845,  2849,
    2853,  2855,  2857,  2861,  2863,  2867,  2868,  2872,  2876,  2878,
    2882,  2883,  2888,  2892,  2894,  2896,  2900,  2904,  2906,  2908,
    2910,  2912,  2914,  2916,  2918,  2920,  2922,  2924,  2926,  2928,
    2930,  2938,  2940,  2944,  2948,  2949,  2956,  2960,  2962,  2966,
    2968,  2972,  2974,  2981,  2985,  2990,  2995,  2996,  2998,  3002,
    3005,  3017,  3021,  3025,  3029,  3033,  3035,  3037,  3039,  3043,
    3045,  3047,  3051,  3053,  3055,  3059,  3061,  3065,  3067,  3071,
    3072,  3076,  3077,  3081,  3082,  3087,  3091,  3092,  3096,  3097,
    3100,  3101,  3102,  3107,  3112,  3113,  3117,  3119,  3123,  3124,
    3128,  3129,  3130,  3133,  3138,  3143,  3147,  3148,  3151,  3155,
    3159,  3162,  3167,  3169,  3173,  3174,  3179,  3184,  3186,  3188,
    3190,  3192,  3196,  3197,  3203,  3205,  3209,  3210,  3216,  3219,
    3221,  3225,  3227,  3234,  3236,  3240,  3244,  3246,  3248,  3254,
    3256,  3260,  3261,  3265,  3267,  3270,  3271,  3275,  3277,  3279,
    3283,  3284,  3288,  3290,  3296,  3298,  3302,  3303,  3307,  3311,
    3313,  3315,  3319,  3321,  3325,  3327,  3333,  3336,  3344,  3351,
    3354,  3361,  3362,  3365,  3369,  3376,  3377,  3380,  3385,  3391,
    3398,  3399,  3403,  3407,  3414,  3416,  3420,  3422,  3424,  3428,
    3429,  3432,  3436,  3438,  3442,  3444,  3450,  3451,  3455,  3459,
    3460,  3465,  3466,  3470,  3474,  3475,  3480,  3481,  3484,  3489,
    3494,  3497,  3504,  3505,  3507,  3508,  3510,  3514,  3515,  3517,
    3519,  3523,  3525,  3529,  3530,  3532,  3536,  3537,  3538,  3539,
    3540,  3541,  3542,  3543,  3544,  3545,  3546,  3551,  3553,  3555,
    3557,  3559,  3561,  3563,  3565,  3571,  3573,  3575,  3577,  3579,
    3581,  3583,  3585,  3587,  3589,  3591,  3593,  3595,  3597,  3599,
    3601,  3605,  3606,  3612,  3614,  3618,  3620,  3625,  3627,  3629,
    3631,  3636,  3638,  3642,  3644,  3648,  3650,  3652,  3654,  3658,
    3662,  3666,  3668,  3672,  3677,  3682,  3689,  3694,  3699,  3708,
    3709,  3713,  3720,  3721,  3722,  3723,  3724,  3725,  3726,  3727,
    3728,  3729,  3730,  3732,  3734,  3741,  3743,  3745,  3752,  3759,
    3766,  3768,  3770,  3771,  3772,  3773,  3775,  3777,  3781,  3782,
    3785,  3801,  3817,  3821,  3825,  3826,  3830,  3831,  3835,  3837,
    3839,  3841,  3843,  3847,  3848,  3850,  3866,  3882,  3886,  3890,
    3892,  3896,  3900,  3903,  3906,  3909,  3912,  3917,  3919,  3924,
    3925,  3929,  3936,  3943,  3950,  3957,  3958,  3962,  3969,  3970,
    3971,  3974,  3976,  3978,  3981,  3988,  3997,  4004,  4013,  4015,
    4017,  4019,  4023,  4026,  4030,  4031,  4035,  4044,  4046,  4050,
    4052,  4056,  4062,  4063,  4066,  4070,  4073,  4077,  4078,  4081,
    4083,  4087,  4089,  4093,  4095,  4099,  4101,  4105,  4108,  4111,
    4116,  4119,  4129,  4131,  4133,  4135,  4137,  4139,  4141,  4143,
    4147,  4148,  4151,  4152,  4155,  4163,  4166,  4169,  4172,  4175,
    4178,  4181,  4184,  4187,  4190,  4193,  4196,  4199,  4202,  4205,
    4208,  4211,  4214,  4217,  4220,  4223,  4226,  4229,  4232,  4235,
    4240,  4241,  4246,  4248,  4249,  4250,  4251,  4252,  4253,  4254,
    4255,  4256,  4257,  4258,  4259,  4260,  4261,  4262,  4263,  4264,
    4265,  4267,  4268,  4269,  4270,  4271,  4272,  4273,  4274,  4275,
    4276
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
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     479,   479,   480,   480,   481,   482,   482,   482,   483,   483,
     484,   484,   484,   485,   486,   486,   487,   487,   487,   487,
     487,   488,   489,   489,   490,   490,   491,   492,   492,   492,
     492,   492,   492,   492,   492,   492,   492,   492,   492,   492,
     493,   493,   494,   494,   495,   495,   496,   496,   497,   497,
     498,   498,   499,   499,   499,   499,   499,   499,   499,   499,
     499,   500,   500,   501,   501,   501,   501,   502,   502,   503,
     503,   503,   504,   504,   505,   505,   505,   506,   506,   506,
     507,   507,   507,   508,   508,   509,   509,   509,   510,   510,
     511,   511,   511,   512,   512,   513,   513,   513,   513,   514,
     514,   515,   515,   515,   516,   516,   516,   517,   517,   517,
     518,   519,   520,   520,   521,   522,   522,   523,   524,   525,
     525,   526,   526,   527,   527,   528,   529,   529,   530,   530,
     531,   531,   531,   531,   532,   532,   533,   533,   534,   534,
     535,   535,   535,   536,   536,   536,   537,   537,   537,   538,
     539,   539,   539,   540,   540,   541,   541,   542,   543,   543,
     544,   544,   545,   546,   546,   546,   547,   548,   548,   548,
     548,   548,   548,   548,   548,   548,   548,   548,   548,   548,
     548,   549,   549,   550,   551,   551,   552,   553,   553,   554,
     554,   555,   555,   556,   557,   558,   559,   559,   559,   560,
     561,   562,   563,   564,   565,   566,   566,   566,   566,   567,
     567,   567,   568,   568,   568,   569,   569,   570,   570,   571,
     571,   572,   572,   573,   573,   574,   575,   575,   576,   576,
     577,   577,   577,   578,   579,   579,   580,   580,   581,   581,
     582,   582,   582,   583,   584,   585,   586,   586,   587,   588,
     589,   589,   590,   590,   591,   591,   592,   593,   593,   593,
     593,   593,   594,   594,   595,   595,   596,   596,   597,   598,
     598,   599,   599,   600,   600,   601,   602,   602,   602,   602,
     602,   603,   603,   604,   604,   605,   605,   606,   606,   606,
     607,   607,   608,   608,   609,   609,   610,   610,   611,   612,
     612,   612,   613,   613,   614,   614,   615,   615,   615,   616,
     616,   617,   617,   618,   619,   620,   620,   621,   622,   623,
     624,   624,   625,   626,   627,   627,   628,   628,   628,   629,
     629,   630,   631,   631,   632,   632,   633,   633,   634,   635,
     635,   636,   636,   637,   638,   638,   639,   639,   639,   640,
     641,   641,   642,   642,   642,   642,   642,   643,   643,   643,
     643,   643,   643,   644,   644,   644,   645,   645,   645,   645,
     645,   645,   645,   645,   645,   645,   645,   646,   646,   646,
     646,   646,   646,   646,   646,   647,   647,   647,   647,   647,
     647,   647,   647,   647,   647,   647,   647,   647,   647,   647,
     647,   648,   648,   649,   649,   650,   650,   651,   651,   651,
     651,   652,   652,   653,   653,   654,   654,   654,   654,   655,
     656,   657,   657,   658,   658,   658,   659,   659,   659,   660,
     660,   661,   662,   662,   662,   662,   662,   662,   662,   662,
     662,   662,   662,   662,   662,   662,   662,   662,   662,   662,
     662,   662,   662,   662,   662,   662,   662,   662,   662,   662,
     663,   663,   663,   664,   665,   665,   666,   666,   667,   667,
     667,   667,   667,   668,   668,   668,   668,   668,   669,   670,
     670,   671,   672,   672,   672,   672,   672,   673,   673,   674,
     674,   675,   676,   677,   678,   679,   679,   680,   681,   681,
     681,   682,   682,   682,   682,   682,   682,   682,   682,   682,
     682,   682,   683,   684,   685,   685,   686,   687,   687,   688,
     688,   689,   690,   690,   691,   691,   691,   692,   692,   693,
     693,   694,   694,   695,   695,   696,   696,   697,   698,   699,
     700,   700,   701,   701,   701,   701,   701,   701,   701,   701,
     702,   702,   703,   703,   704,   705,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
     720,   721,   722,   723,   724,   725,   726,   727,   728,   729,
     730,   730,   731,   731,   731,   731,   731,   731,   731,   731,
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
       1,     2,     2,     2,     1,     1,     1,     0,     2,     2,
       4,     4,     5,     0,     1,     2,     2,     4,     3,     2,
       2,     7,     1,     0,     1,     0,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     2,     2,     2,     2,
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
       1,     3,     4,     5,     6,     7,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     2,     1,     1,     3,     3,     0,     2,
       0,     2,     0,     3,     3,     3,     1,     4,     4,     1,
       9,     0,     0,     0,     0,     3,     2,     1,     0,     3,
       5,     3,     2,     5,     2,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     0,     2,     1,     3,     1,     1,
       1,     1,     3,     6,     1,     0,     3,     0,     1,     3,
       1,     1,     1,     4,     5,     5,     1,     1,     2,     4,
       4,     2,     3,     0,     1,     2,     1,     1,     2,     2,
       2,     0,     1,     0,     3,     0,     1,     3,     1,     2,
       0,     2,     0,     2,     0,     4,     1,     2,     1,     1,
       0,     1,     3,     2,     1,     1,     2,     1,     4,     3,
       1,     3,     4,     0,     3,     0,     1,     3,     3,     1,
       1,     0,     1,     1,     4,     0,     2,     4,     0,     8,
       5,     1,     1,     7,     4,     1,     1,     8,     5,     4,
       1,     3,     3,     1,     7,     7,     5,     3,     0,     1,
       1,     1,     2,     0,     1,     1,     1,     0,     3,     1,
       3,     1,     0,     3,     1,     3,     1,     3,     3,     1,
       1,     3,     1,     2,     1,     3,     3,     1,     2,     3,
       3,     3,     3,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     1,     1,     5,     6,     5,     6,     3,     4,     5,
       6,     3,     4,     3,     4,     3,     4,     4,     5,     4,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     4,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     4,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     1,     6,     1,     4,     7,     2,     0,     4,
       3,     6,     1,     1,     6,     8,     6,     1,     1,     4,
       6,     4,     5,     3,     5,     4,     5,     1,     1,     1,
       4,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     0,     1,     1,     1,     1,     1,     1,
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
       0,     0,   590,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   590,   678,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   572,    19,   577,   578,   579,     6,    18,
     571,   573,     5,    20,   658,   765,   666,   669,    12,     9,
     781,   782,    22,   785,   786,     4,   483,     0,     0,     0,
       0,     0,     0,     0,   407,   589,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,     0,     0,     0,
     103,   771,   961,     0,    23,   959,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,     0,   369,    29,    37,    34,    36,
     807,    35,   807,  1050,  1052,  1053,  1054,  1055,  1056,  1057,
    1076,  1077,  1058,  1060,  1059,  1065,  1061,  1066,  1062,  1064,
    1067,  1063,  1068,  1069,  1070,  1072,  1073,  1071,  1074,  1075,
    1079,  1078,  1080,     0,    31,     0,    32,     0,    67,    69,
    1042,  1051,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,  1021,   672,   677,     0,     0,   597,     0,   724,
     726,  1044,     0,     0,     0,     0,   116,   580,  1049,     0,
       0,     2,     0,   662,     0,  1023,   778,     0,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   466,
     460,   461,   459,   462,   463,   464,   465,   467,   468,   469,
       0,   436,   437,   408,     0,  1033,   414,     0,  1037,   411,
     266,  1041,     0,   726,   410,   385,  1045,   415,     0,  1036,
     594,   587,   957,     0,   112,   166,   165,   113,   148,   215,
     819,  1029,   104,     0,   110,   160,     0,   106,   266,   114,
     963,   137,   107,   200,   108,     0,   109,   807,  1048,   115,
     161,   111,   769,   770,     0,   962,   960,     0,    75,     0,
    1034,   742,   504,  1032,   497,     0,   503,   508,   498,   499,
     506,   500,   501,   502,   507,   509,   363,     0,   268,    30,
       0,    39,   806,    38,    28,     0,     0,     0,     0,     0,
     812,     0,     0,     0,     0,     0,     0,   586,   967,     0,
     948,   671,   671,   672,   684,   682,  1020,   685,     0,   686,
     676,   672,     0,     0,     0,     0,     0,     0,   613,   606,
     595,   596,   598,   600,   601,   602,   609,   610,   603,     0,
     725,  1043,   120,     0,   117,   266,   118,   200,   119,   807,
     583,     0,   636,   637,   644,     0,   643,   645,   647,   652,
     640,   638,   641,   639,   648,   651,   646,   650,     0,     0,
       0,   660,     0,   656,     0,     0,  1022,     0,     0,   663,
       0,     0,     0,     0,   412,   484,   377,     0,   377,     0,
     218,   413,   418,   128,   478,   479,     0,   268,   239,   470,
       0,   409,   427,     0,   383,   384,   493,     0,   593,   591,
     958,   163,   163,     0,     0,     0,   175,   167,   168,   147,
       0,   125,     0,     0,     0,   121,   268,   136,   135,   138,
       0,     0,     0,     0,     0,  1025,    76,   218,     0,     0,
     744,   784,   505,     0,     0,     0,     0,   925,   954,     0,
    1024,     0,   927,     0,     0,   955,     0,   949,   940,   939,
     938,  1050,     0,     0,     0,   930,   931,   932,   941,   942,
     950,   951,  1060,   952,   953,     0,  1065,  1061,   956,  1074,
    1079,     0,     0,     0,     0,   907,   366,   902,   816,   934,
     928,   903,   361,   943,   905,   906,   922,   923,   924,   944,
     904,   968,   969,   982,   970,   984,   911,   908,   909,   992,
     993,   997,   998,   910,   929,     0,     0,  1029,     0,     0,
       0,   370,   216,     0,     0,     0,   809,    27,     0,    33,
       0,     0,    43,    70,    72,  1046,    68,     0,     0,     0,
     811,     0,    48,    50,    28,     0,     0,   585,     0,     0,
       0,   681,   679,     0,   688,     0,   687,   689,   694,   675,
       0,   632,     0,   607,   618,   604,   618,   605,   619,   625,
     629,   630,   614,   599,   742,   790,   820,     0,     0,   126,
     268,     0,     0,   582,   581,   649,   642,     0,     0,  1040,
       0,     0,   657,   661,   764,   766,   771,   668,   667,   776,
     183,     0,   486,   144,     0,   489,     0,   490,   485,     0,
     422,     0,   424,   423,     0,   425,     0,   419,     0,     0,
     269,     0,     0,   431,     0,   432,   377,   471,     0,   477,
       0,     0,     0,   474,  1029,   388,   387,   492,     0,   405,
       0,   164,     0,     0,   173,     0,   172,     0,     0,   162,
     176,   177,   179,   169,   541,   553,   528,   563,   531,   554,
     564,     0,   570,   532,     0,   560,     0,   555,   567,   527,
     550,   529,   530,   525,   377,   510,   511,   512,   520,   526,
     513,   522,   543,     0,   548,   545,   523,   560,   524,   124,
       0,   122,   266,   123,     0,     0,   139,     0,     0,   199,
     239,   385,   375,     0,   218,     0,    82,    84,     0,     0,
     102,   101,  1026,     0,     0,     0,     0,   893,   894,   895,
       0,   741,   824,   833,   836,   842,   837,   838,   839,   840,
     844,   845,   843,   846,   841,   822,     0,   750,   765,  1021,
       0,  1021,   945,     0,     0,  1021,  1021,  1021,     0,   946,
     947,     0,     0,  1008,     0,     0,     0,     0,     0,     0,
     913,   912,     0,   765,   934,     0,   331,  1047,     0,   360,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   520,   543,     0,     0,   368,     0,   279,     0,   808,
       0,     0,    44,    73,     0,     0,    26,   798,     0,   814,
       0,   780,   798,     0,     0,    47,   584,     0,   671,     0,
       0,   742,     0,   693,     0,   575,     0,   574,   965,     0,
     608,     0,   616,   612,   611,     0,   622,     0,   627,   615,
       0,   744,   788,     0,     0,     0,   239,   375,   655,   633,
     654,     0,     0,     0,   659,     0,   775,     0,     0,   145,
     184,   488,   377,   421,     0,   377,   378,   426,   375,     0,
     265,   375,   238,  1030,   210,   207,     0,   150,   377,   510,
     375,     0,     0,     0,   237,   240,   241,   242,   243,     0,
     430,   476,   475,   429,   428,     0,     0,     0,   391,   389,
     390,   386,     0,   377,   481,     0,   480,   193,   170,   174,
       0,     0,   178,     0,     0,   538,   551,   552,   568,     0,
     565,   570,     0,   558,   556,   557,   150,     0,     0,     0,
     521,     0,     0,     0,     0,   559,   127,   268,     0,   375,
     140,   198,     0,   201,   203,   204,     0,     0,     0,     0,
     218,   218,    79,     0,    87,    78,     0,     0,     0,     0,
     835,   823,     0,     0,   896,   897,   898,     0,   827,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   746,   748,
     749,   743,     0,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,     0,     0,     0,     0,
    1003,     0,     0,     0,     0,     0,     0,     0,   921,   778,
     920,   362,     0,   332,   364,   333,     0,     0,   916,  1028,
     915,   914,   917,   918,   919,   935,   990,     0,   926,   818,
     817,   372,   371,     0,   272,   287,     0,   278,   280,   810,
       0,     0,     0,    71,     0,   803,     0,   813,     0,   803,
       0,     0,   680,   683,     0,   695,   696,   699,   701,   698,
     710,   711,   712,   700,   723,     0,   735,   690,   692,  1038,
     576,   966,   617,   626,   621,   620,     0,   631,   791,   765,
     792,   821,   375,     0,     0,   634,   653,     0,   767,   673,
     768,   777,   487,   163,   190,   182,   185,   186,     0,     0,
       0,   420,   375,   375,   378,   277,   270,   279,   209,   377,
     377,   378,   211,   206,   212,     0,     0,   248,     0,   434,
     378,   377,   375,   433,     0,     0,     0,   404,   495,   482,
     194,   195,   171,     0,   181,   539,   540,   964,     0,     0,
       0,   543,     0,   566,     0,     0,   378,     0,   516,   518,
       0,     0,     0,     0,     0,     0,     0,   385,   279,   163,
     142,   239,   196,   405,   380,     0,    83,     0,   218,     0,
      91,     0,    85,     0,     0,    90,    86,     0,   789,  1031,
       0,     0,   828,     0,     0,   834,     0,     0,   826,   825,
       0,   883,     0,   872,   871,     0,   850,     0,   881,   899,
       0,   891,     0,     0,     0,   851,   877,     0,     0,   854,
       0,     0,     0,     0,     0,     0,     0,   852,     0,     0,
     853,     0,   885,     0,     0,   847,     0,     0,   848,     0,
       0,   849,   747,   750,   783,     0,     0,   218,     0,   971,
       0,     0,   985,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1009,     0,     0,  1007,     0,     0,   999,     0,
       0,     0,     0,  1010,   665,     0,     0,     0,   933,   989,
       0,   267,   286,     0,   338,   367,   281,     0,     0,     0,
       0,     0,  1056,    41,    52,    54,    53,    62,    64,    41,
      42,     0,     0,     0,   795,   815,     0,   794,     0,     0,
       0,   731,   698,     0,   733,   727,   733,   731,   733,     0,
       0,     0,   694,     0,   740,   623,   624,   628,   778,   279,
     197,   380,   635,     0,     0,   191,     0,   187,     0,     0,
       0,   416,   226,   221,   936,   222,   223,   224,   225,     0,
       0,   271,     0,     0,     0,   227,     0,   248,   771,   244,
       0,   435,   472,   473,   396,   397,   393,   392,   395,   394,
     377,   494,   378,   180,  1027,     0,     0,   534,     0,   533,
     569,     0,   561,   151,     0,   514,     0,   542,   543,   549,
     547,     0,   544,   375,     0,     0,     0,   134,   143,   202,
     377,     0,   376,   105,     0,   132,   131,    94,     0,     0,
      95,     0,    88,    89,     0,   889,   890,   830,   829,   832,
     831,     0,     0,     0,     0,   765,     0,     0,     0,   892,
       0,     0,     0,     0,     0,     0,   884,   882,   878,     0,
     886,     0,     0,     0,     0,   887,     0,     0,     0,     0,
       0,     0,   754,     0,   751,     0,   755,   977,   976,     0,
       0,   973,   972,     0,   981,   980,   979,   978,   975,   974,
       0,   988,  1005,  1004,     0,     0,     0,     0,     0,     0,
       0,   365,   335,   334,   273,     0,     0,   283,   284,     0,
    1029,   321,     0,     0,     0,     0,     0,     0,  1058,  1063,
       0,     0,   359,   297,   292,     0,   338,   291,     0,     0,
       0,   317,   318,     0,   315,     0,     0,   316,     0,     0,
       0,   353,   355,   354,     0,     0,     0,     0,     0,     0,
       0,   793,     0,  1029,    66,    58,    59,    65,    60,    61,
       0,    24,     0,     0,    25,     0,   799,   800,   801,   797,
     802,   805,   804,   779,    45,    46,   694,   702,   697,   732,
     730,   728,     0,   729,     0,     0,     0,     0,     0,     0,
     744,   787,     0,   376,   772,   773,   674,   193,   192,   188,
       0,   378,   937,   275,   276,   378,   378,   149,   214,   228,
     229,   377,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   491,   536,     0,   537,     0,     0,   214,
     152,   153,   155,   156,   157,   517,   519,   515,   546,   279,
     405,   378,   193,     0,     0,   373,   377,    92,     0,     0,
      98,    93,   543,     0,   875,     0,     0,   901,   778,   900,
     873,     0,     0,     0,   879,     0,     0,     0,     0,   888,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   750,     0,   757,     0,   753,   756,   991,   217,  1011,
     983,     0,     0,  1006,  1001,  1000,     0,   994,   996,     0,
     220,   282,     0,   319,     0,   314,     0,   329,   313,   341,
    1039,   356,   357,     0,   293,     0,   298,   295,   344,   308,
     309,   305,   306,   310,   303,     0,     0,   312,   311,   304,
       0,   300,   302,   301,   299,   307,   337,     0,    55,    56,
      57,    63,     0,   705,     0,   713,   731,   722,   720,   734,
     736,   738,   739,   670,   378,   374,   774,   189,   378,   159,
     264,     0,     0,   205,   230,     0,     0,     0,   248,   231,
     234,   807,     0,   378,   402,   403,   399,   398,   401,   400,
       0,   535,   562,   158,   146,   154,     0,   377,   261,   141,
     378,   379,     0,     0,    96,    97,    80,     0,   858,   866,
     664,   874,   859,   867,   862,   870,   876,   880,   860,   868,
     861,   869,   855,   863,   856,   864,   857,   865,   752,     0,
     763,   987,   986,  1002,     0,     0,     0,   289,   285,     0,
     320,   322,     0,   328,     0,   352,     0,     0,     0,     0,
     347,   345,     0,     0,     0,   330,    40,   796,   707,   704,
     714,     0,     0,   715,   716,   717,     0,   262,   417,   208,
     213,   232,   248,   807,   235,   254,   247,   249,   406,   378,
       0,   381,   378,    77,    99,   100,     0,   760,     0,   759,
     995,   219,   288,     0,     0,     0,     0,   351,   350,  1035,
     349,   338,     0,     0,     0,     0,     0,   703,   718,     0,
     737,   236,   254,     0,   248,   250,   251,   263,   378,     0,
     758,     0,     0,     0,   325,   338,   358,   346,   348,     0,
     343,     0,     0,   708,     0,   248,     0,     0,   246,     0,
     253,     0,   252,   382,   133,   761,     0,   290,   327,     0,
       0,     0,   338,     0,   706,   719,   233,     0,     0,   257,
     256,   255,   762,   338,     0,     0,   338,   336,   709,   260,
     258,   259,   326,   338,   342,   323,   324,     0,     0,     0
};

static const short yydefgoto[] =
{
    1957,    21,    22,   538,   143,   144,   145,   146,  1551,   806,
     801,    23,   157,   158,  1303,  1304,  1305,  1306,  1307,   147,
     148,   542,   543,    24,    88,   446,   954,   715,   716,   717,
    1196,  1189,  1190,  1641,   278,   719,    25,    80,    26,   176,
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
     187,   394,   395,   234,   649,  1382,    31,   103,   684,   685,
     686,  1167,  1168,   687,   791,   689,   690,  1161,   915,   930,
     691,   792,   693,   694,   695,   696,   923,   697,   698,   920,
      32,    33,    34,    35,    36,   594,    37,   558,    38,    39,
      66,   241,   419,    40,   340,   341,   342,   343,   344,   345,
     346,   347,   833,   348,  1096,  1337,   578,   579,   838,   580,
      41,    42,   378,   379,   380,   853,   849,  1535,   183,   602,
     383,    44,  1434,   772,    45,    46,    47,   561,   330,  1343,
    1746,   162,   163,   164,   328,   565,   566,   567,   824,   821,
    1075,  1076,  1077,  1078,  1838,  1887,  1912,  1079,  1080,  1081,
    1082,  1843,  1844,  1845,  1083,  1332,   168,   169,  1330,  1570,
    1334,  1739,  1740,  1580,   450,   748,  1462,   992,  1463,  1464,
    1465,  1675,  1866,  1869,   186,   604,   605,   274,  1586,  1110,
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
    1878,   255,  1867,  1088,  1542,   598,  1084,   149,   526,   233,
     235,  1308,   776,   267,   177,   527,   151
};

static const short yypact[] =
{
    1688,   638,    88,  1784,   252,   456,   511,  2427,    48,  5033,
     597,   478,  1483,    88,   518,   493,  5870,   435,  5870,   425,
     573,   453,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   704,   368,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  7588,  5870,  5870,
    5870,  5870,  5870,  5870,-32768,-32768,   634,   670,  5870,  5870,
    5870,   779,  5870,   605,  5870,  5870,-32768,  5870,  5870,  5870,
  -32768,   679,-32768,   612,-32768,-32768,   773,  5870,-32768,  5870,
    5870,  5870,   777,  5870,  5870,  5870,  5870,   605,  5870,  5870,
    5870,  5870,  5870,-32768,  5870,   579,   721,-32768,-32768,-32768,
     598,-32768,   598,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   769,   624,   788,-32768,    40,-32768,-32768,
  -32768,-32768,  5870,  5870,  5870,   797,   802,  1484,  5870,   756,
     255,   278,   222,   705,-32768,  5870,   854,   690,   785,-32768,
    5870,-32768,  5870,  5870,  5870,  5870,-32768,-32768,-32768,  5870,
    2421,-32768,   775,   764,   947,   241,   714,   636,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     296,-32768,-32768,-32768,   670,-32768,-32768,   450,-32768,-32768,
     703,-32768,   702,-32768,-32768,   635,-32768,-32768,   718,-32768,
     829,-32768,-32768,   793,-32768,  1023,-32768,-32768,   995,-32768,
  -32768,-32768,-32768,   670,-32768,-32768,   130,-32768,   703,-32768,
  -32768,   225,-32768,   956,-32768,   967,-32768,   598,-32768,-32768,
  -32768,-32768,-32768,-32768,   950,-32768,-32768,   834,-32768,   955,
  -32768,   850,-32768,-32768,-32768,   834,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  6434,  5870,   892,-32768,
    5870,-32768,-32768,-32768,   875,   547,   909,  4118,  5870,   972,
     778,   994,   998,  1000,   940,   942,   109,   822,-32768,  6653,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5120,-32768,
  -32768,-32768,   895,  5870,   975,   894,   179,  5870,   908,-32768,
  -32768,   690,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5870,
  -32768,-32768,-32768,   670,-32768,   703,-32768,   956,-32768,   598,
     961,   929,-32768,-32768,-32768,  1021,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1004,  5870,
    5870,   977,   853,-32768,  6653,   957,-32768,   957,  6653,-32768,
      92,   851,   858,   860,   636,-32768,  1087,   696,-32768,  5870,
  -32768,   296,-32768,-32768,-32768,-32768,  5870,   892,  2899,   912,
    3149,   846,-32768,  7588,-32768,-32768,   738,   935,-32768,-32768,
  -32768,   861,   861,   670,  1022,   670,   205,  1023,-32768,-32768,
    7553,-32768,  5870,  5870,  5870,-32768,   892,-32768,-32768,  1107,
     310,   862,  5870,  1128,  5870,-32768,-32768,    73,  5432,  4244,
    1011,-32768,-32768,   869,   872,   873,   960,-32768,-32768,   874,
  -32768,   882,-32768,   884,   885,-32768,   897,-32768,-32768,-32768,
  -32768,   900,   902,   971,   974,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   903,-32768,-32768,  5339,   905,   910,-32768,   911,
    1042,  6653,  6653,  5558,  5870,-32768,  1043,   913,-32768,  1343,
  -32768,-32768,   919,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   920,   914,   916,  7553,   932,
     515,-32768,-32768,   927,  1196,   544,-32768,-32768,  5870,-32768,
    5870,  5870,    85,-32768,-32768,-32768,-32768,  5870,  5870,   107,
  -32768,  5870,-32768,-32768,   875,  1068,  4118,-32768,  5870,  5558,
     458,-32768,-32768,  6653,-32768,  1144,   933,-32768,   203,-32768,
     432,-32768,   108,-32768,   106,-32768,   106,-32768,   934,-32768,
     101,-32768,  1047,-32768,    67,-32768,-32768,   945,   944,-32768,
     892,   937,  1221,-32768,-32768,-32768,-32768,   135,  1152,-32768,
     958,  5870,-32768,-32768,   962,-32768,   330,-32768,-32768,  1222,
  -32768,  1150,  1157,-32768,   670,-32768,  1159,-32768,-32768,  1220,
  -32768,  1229,-32768,-32768,  1209,-32768,  7553,-32768,  7553,   552,
  -32768,  1244,  5870,-32768,  7220,-32768,   385,-32768,  5870,   629,
     702,  1081,  1048,  1125,  1089,-32768,-32768,-32768,   129,  1134,
     449,-32768,  1077,   605,-32768,   670,-32768,  1262,  1190,-32768,
     205,-32768,-32768,-32768,   245,  1101,-32768,-32768,-32768,  1109,
  -32768,  1161,  1017,-32768,  1206,  1025,   308,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1019,-32768,
  -32768,-32768,   126,  1034,  1037,    49,-32768,  1025,-32768,-32768,
     670,-32768,   703,-32768,  1255,  1307,-32768,   670,   670,-32768,
    2899,   635,-32768,  1197,-32768,  1169,  1044,-32768,  7509,  1194,
  -32768,-32768,-32768,  1202,  1052,  4463,  1055,  1066,  1331,  1362,
    3994,   185,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2245,   539,  1205,   491,
    6653,   153,-32768,  5870,  6653,   507,   637,   651,  1436,-32768,
  -32768,  6653,  4682,  1343,   147,  1153,  6653,  6653,  4682,  1283,
     156,   156,  1074,   368,   671,   568,-32768,-32768,  4337,-32768,
    6653,  5870,  6653,  6653,  6653,  6653,  6653,  6653,  5777,  3400,
  -32768,-32768,  1324,  1071,  5870,-32768,  5870,  1314,  5870,-32768,
    1183,  1186,-32768,-32768,  1163,  4118,-32768,  1308,   588,-32768,
    1083,-32768,  1308,  5870,  5870,  1092,-32768,   671,-32768,   685,
    4786,  1182,  6653,-32768,  5870,-32768,   295,-32768,-32768,   155,
  -32768,  1187,-32768,-32768,-32768,  5870,   361,  5870,-32768,-32768,
    5870,  1011,-32768,  6653,  5870,  1364,  2899,-32768,-32768,-32768,
  -32768,   135,  5870,  1293,  1103,  6653,  1138,  6653,   670,-32768,
     391,-32768,-32768,-32768,  1104,-32768,-32768,-32768,   111,  5870,
  -32768,-32768,-32768,-32768,  1372,-32768,  1105,  1338,-32768,  1365,
  -32768,  1309,  1311,   598,-32768,-32768,-32768,-32768,-32768,  1377,
  -32768,-32768,-32768,-32768,-32768,  5870,  7318,   605,  1275,  1276,
    1282,-32768,   605,-32768,-32768,   525,-32768,   719,-32768,-32768,
    1254,   670,-32768,  5432,   244,  1261,-32768,-32768,-32768,   605,
  -32768,  1017,   297,-32768,-32768,-32768,  1338,   297,  1257,   297,
  -32768,   605,   605,  1140,   605,-32768,-32768,   892,  5870,-32768,
    1318,-32768,   611,-32768,-32768,-32768,   738,   957,  5870,   617,
      47,-32768,-32768,  1142,   486,-32768,  5432,  5870,   957,  4901,
  -32768,-32768,   957,  4463,-32768,-32768,-32768,   117,-32768,   186,
    1402,  4682,  4682,  6653,  6653,  1882,  1143,   586,  2173,  6653,
    2525,   170,  2775,  3025,  5996,  3275,  3525,  3775,-32768,-32768,
    1315,-32768,  1146,   714,  6653,  6653,   192,  6653,  1147,  6653,
    1145,   697,  6653,  6653,  6653,  6653,  6653,  6653,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1368,  1301,   160,  6653,
  -32768,  4682,  6653,   174,   344,   650,   115,  5870,-32768,   714,
  -32768,-32768,  4556,-32768,  1151,-32768,  1155,   475,-32768,-32768,
    1404,   156,   156,   687,   687,  1343,-32768,   630,-32768,-32768,
  -32768,-32768,-32768,   659,-32768,  1252,  1427,  1314,-32768,-32768,
    7515,  7515,  1320,-32768,   175,  1341,  5870,-32768,  6653,  1341,
    1395,  1398,-32768,-32768,  3649,  1185,   169,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1188,  4682,  1393,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   369,-32768,-32768,  1205,
    1343,-32768,-32768,   663,   957,-32768,-32768,   135,-32768,-32768,
  -32768,  1343,-32768,   861,  1455,-32768,   391,-32768,  1192,  4682,
    1976,-32768,-32768,-32768,-32768,-32768,-32768,  1314,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   598,   598,  1232,  1198,-32768,
  -32768,-32768,-32768,-32768,   337,   162,   632,-32768,  1458,-32768,
  -32768,-32768,-32768,  5870,-32768,-32768,-32768,-32768,   252,   672,
    1326,  1324,  1204,-32768,  1077,   688,-32768,   489,-32768,  1207,
    5870,   501,  1212,  1214,   605,  1225,  1474,   635,  1314,   861,
    1157,  2899,-32768,  1134,  1296,  2649,-32768,   605,  1366,  1454,
  -32768,  7509,-32768,   605,   379,-32768,-32768,  1456,-32768,-32768,
    1231,  1233,-32768,  4682,  4682,-32768,  4682,  4682,-32768,  1503,
     227,  1343,  1235,-32768,-32768,  1236,  1343,  5558,-32768,-32768,
    1472,-32768,   441,  1241,  1258,  1343,  1114,  1263,  1264,  1343,
    6653,  6653,  1143,  6653,  6215,  1271,  1272,  1343,  1274,  1277,
    1343,  6653,  1343,  1278,  1292,  1343,  1294,  1299,  1343,  1300,
    1302,  1343,-32768,  3878,-32768,   770,   799,-32768,   828,-32768,
     836,  6653,-32768,   985,   993,  1032,  1053,  1130,  1167,  6653,
    6653,  6653,  1343,  1486,   182,  1343,  1378,  6653,-32768,  6653,
    6653,  6653,  6653,-32768,-32768,   700,  4556,  5651,-32768,-32768,
    5870,-32768,-32768,  5870,  6894,-32768,-32768,  5870,  5870,  5870,
    5870,  5870,  5870,  1369,  1304,-32768,  1305,-32768,-32768,  1369,
  -32768,   326,   326,   287,-32768,-32768,   701,-32768,  7515,  7515,
    1310,   556,  1312,  4786,  1444,-32768,  1444,   748,  1444,  1494,
    1499,  6653,   953,  1567,  1515,-32768,-32768,-32768,   714,  1314,
  -32768,  1296,-32768,   514,  1077,  1460,  1077,-32768,  4682,   131,
     658,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1976,
    1976,-32768,  1427,  6653,  1976,   521,  1447,  1232,   679,-32768,
    4682,-32768,-32768,-32768,  1469,  1488,  1495,  1496,  1497,  1500,
  -32768,-32768,-32768,-32768,-32768,  1332,   252,-32768,  1403,-32768,
  -32768,   297,-32768,   208,   297,-32768,   297,-32768,  1324,-32768,
  -32768,  1340,-32768,-32768,   738,  1427,  1077,-32768,  1157,-32768,
  -32768,  1602,-32768,-32768,  1348,-32768,  1103,-32768,  1351,   670,
     173,  1361,-32768,-32768,   670,-32768,-32768,-32768,  1503,-32768,
    1503,  6653,   957,   957,  1363,   368,   710,  6653,  1564,-32768,
     957,   957,  6653,   957,   957,   390,  1343,-32768,  1323,  6653,
    1343,   957,   957,   957,   957,  1343,   957,   957,   957,   957,
     957,   957,-32768,  1376,  1367,   570,  5870,-32768,-32768,  1379,
    7553,-32768,-32768,  1180,-32768,-32768,-32768,-32768,-32768,-32768,
    1200,  1273,-32768,-32768,  6653,  6653,  1590,  1487,  1251,   711,
     818,-32768,-32768,-32768,-32768,  7553,  1380,-32768,-32768,  1621,
    1623,  5870,    70,  1390,  1389,  1586,  6653,  1394,  1406,  5213,
    5870,  5870,-32768,-32768,-32768,  1618,  6786,-32768,  1407,  1408,
    1412,-32768,-32768,  1415,-32768,  1416,  1417,-32768,   157,  1420,
    1425,-32768,-32768,-32768,  1433,  1598,  1438,  1443,  1446,  1450,
    1451,-32768,  1432,   508,-32768,-32768,-32768,-32768,-32768,-32768,
    1650,-32768,  7515,  7515,-32768,  1549,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1719,-32768,   169,-32768,
  -32768,-32768,  1646,-32768,  4786,  4786,   727,  5870,  6653,  4682,
    1011,-32768,  1427,-32768,-32768,-32768,-32768,   719,-32768,-32768,
     133,-32768,-32768,-32768,-32768,-32768,  1343,-32768,  1710,   521,
  -32768,   596,  5870,-32768,  1656,   140,  1556,  1660,  1561,  1698,
    1665,  1703,  1314,-32768,-32768,  1470,-32768,  1471,  1657,  1710,
     208,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1314,
    1134,-32768,   719,  1458,  1628,-32768,-32768,-32768,  1644,    89,
  -32768,-32768,  1324,  1645,  1343,  1478,  1480,-32768,   714,-32768,
    1343,  6653,  1482,  1485,  1343,  1490,  1491,  6653,  6653,  1343,
    1492,  1493,  1502,  1504,  1505,  1507,  1509,  1510,  1513,  1514,
  -32768,  3878,  1489,-32768,  5870,-32768,-32768,-32768,-32768,-32768,
  -32768,  6653,  1516,-32768,-32768,-32768,  6653,-32768,-32768,  6653,
     114,-32768,  1701,  6653,  6653,-32768,  4682,  1343,-32768,-32768,
  -32768,-32768,-32768,  5870,-32768,   194,-32768,  1576,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   139,  1498,-32768,-32768,-32768,
    4556,-32768,-32768,-32768,-32768,-32768,-32768,  1649,-32768,-32768,
  -32768,-32768,   326,  5870,  4786,-32768,   567,-32768,-32768,  1517,
  -32768,  1343,   185,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1518,  5870,-32768,-32768,  1681,  1693,  5870,  1232,-32768,
  -32768,   598,  5870,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1427,-32768,-32768,-32768,-32768,-32768,  1427,-32768,-32768,-32768,
  -32768,-32768,  6653,   670,-32768,  1578,-32768,   670,-32768,-32768,
  -32768,  1343,-32768,-32768,-32768,-32768,  1343,  1343,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5870,
    1717,  1343,-32768,-32768,  1259,  1976,  1976,-32768,-32768,  1520,
    1343,  1285,   142,-32768,  1720,-32768,  5870,  5870,   252,   149,
  -32768,-32768,  1553,  1721,  4682,  1151,-32768,-32768,  1523,-32768,
  -32768,  4682,  1527,-32768,-32768,-32768,  6653,-32768,-32768,-32768,
  -32768,-32768,  1232,   598,-32768,  1691,-32768,-32768,-32768,-32768,
    1458,-32768,  1343,-32768,-32768,-32768,  1535,  1538,  1541,-32768,
  -32768,-32768,-32768,   957,  4556,  1643,  4556,-32768,-32768,-32768,
  -32768,  7029,   194,  6653,  4556,   150,  5870,-32768,   185,  5870,
  -32768,-32768,  1691,   199,  1232,  1700,  1702,-32768,-32768,  1544,
  -32768,  5870,  5870,  1548,  1151,  7029,  1151,-32768,-32768,  1335,
     132,  1773,   732,-32768,   746,  1232,   433,   433,-32768,  1785,
  -32768,  1648,-32768,-32768,-32768,-32768,  1551,-32768,  1786,  4556,
    1798,  1788,  7029,  5870,-32768,-32768,-32768,  1636,   554,-32768,
  -32768,-32768,-32768,  7029,   161,  5870,  7029,-32768,-32768,-32768,
  -32768,-32768,-32768,  7029,-32768,-32768,-32768,  1840,  1842,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1291,  1690,-32768,  1694,  1543,   541,-32768,
    1039,-32768,-32768,-32768,  -931,-32768,  -668,-32768,  -635,  1696,
    1547,  1303,  1051,-32768,-32768,-32768,   666,-32768,  1148,   907,
  -32768,-32768,   673,-32768,-32768,   904,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   683,-32768,-32768,   938,-32768,-32768,   249,
  -32768,-32768,  1475,  1787,-32768,-32768,  -379,-32768,-32768,-32768,
    1448,-32768,-32768,  1210,  -396,-32768,-32768,-32768,   749,-32768,
    -916,-32768,-32768,  1521,  1027,   708,  1476,  1003,   698,-32768,
     266,   -57,  -290,-32768,  -368,-32768, -1231,-32768,-32768,   299,
  -32768,  1508, -1214,-32768,-32768,-32768,-32768, -1322,   285,     2,
       3,     5,   -16,-32768,-32768,-32768,-32768,  -134,  -300,-32768,
    1035,-32768,  -713,-32768, -1060,-32768,   848,-32768,-32768,-32768,
  -32768,-32768, -1345,  -994,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -758, -1001,-32768,
  -32768,-32768,-32768,-32768,   196,-32768,    24,-32768,-32768,-32768,
  -32768,-32768,  1907,-32768,-32768,-32768,-32768,-32768,  1116,-32768,
  -32768,  -680,   325,  -383,  -828,   571,-32768,-32768,  -596,  -829,
  -32768,-32768, -1064, -1496,-32768,-32768,-32768,-32768,-32768,  1512,
  -32768,  1279,  1522,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1524,-32768,-32768,-32768,-32768,-32768,  -593,  -488,
  -32768,   982,   528,  -608,  -407,-32768,-32768,-32768,-32768, -1052,
  -32768,  -402,-32768,-32768,-32768,-32768,  1218,-32768,-32768,  1005,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1914,-32768,-32768,-32768,-32768,-32768,  1587,-32768,-32768,-32768,
    1357,-32768,  1354,-32768,-32768,-32768,-32768,  1096,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  -673,     1,-32768,-32768,
  -32768,  -474,   246,-32768,  -458,   652,-32768,  -251,   -47,-32768,
  -32768,-32768,-32768,  1775,-32768,-32768,-32768,  1117, -1163,-32768,
  -32768, -1018, -1419,-32768,-32768,-32768,-32768,   866,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1853,   -40,   616,  -280,
  -32768,-32768,    99,-32768,  -432,  -803,  1199,-32768,   276,-32768,
     483,-32768, -1022,-32768,  -671,-32768,  1095,  -569,-32768,-32768,
    -894,  1952,  1953,-32768,-32768,  1954,-32768,-32768,  1373,-32768,
    -244,-32768,-32768,  1149, -1192,-32768,   889,-32768,  -107,  -730,
    -591,-32768,-32768,-32768,  -752,   -56,  -493,  -735,-32768,  -518,
  -32768,-32768,-32768,   239,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -594,-32768,   730,-32768,  1024, -1046,-32768,
    -457,-32768,   609,  -999,  -147,  -944,  -910,  -903,   -49,    -2,
       0,-32768,   -39,  -754,  -528,  -144,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1255,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   686,-32768, -1145,-32768,-32768,   520,-32768,  -901,
     451,  1054,   794,   214,  -732,  1330, -1189,-32768,   -66,  1876,
  -32768,     9,   -51, -1291,   461,  -266,     4,   -54,  -166,   -10,
     -53,  -278,-32768,   -81,   -52,    -9,-32768
};


#define	YYLAST		7856


static const short yytable[] =
{
     150,    43,    84,   253,   350,   303,   170,   171,   227,   178,
     854,   248,   249,   321,   324,   624,   322,   325,   246,   293,
    1033,   232,   265,   688,   270,   284,  1035,  1018,   692,   544,
     662,  1285,   626,  1026,   263,   773,   775,   856,  1099,  1541,
     790,   878,   827,   653,   288,  1603,   292,   294,   222,   225,
     228,   231,   171,   236,   239,   809,  1321,  1050,   290,   251,
     225,   239,  1295,   231,   230,   171,   236,  1362,   268,   150,
     239,   562,   238,   260,  1352,   811,   258,   993,   280,   170,
     171,   283,   225,  1054,   280,   239,   228,   231,   291,   171,
     236,   268,   150,   239,   359,   231,   309,   260,   311,  1254,
     289,   773,  1029,   287,  1499,   585,   353,   631,   296,  1389,
     955,   295,  1101,   310,   600,   946,   628,  1183,  1405,  1410,
    1559,  1354,   906,   971,   436,  1203,  1482,   360,  1593,  1594,
    1309,   961,  1486,  1597,   357,  1284,   705,  1780,   867,   971,
     868,   971,  1930,   251,   171,   251,   879,   928,   971,   150,
     971,  1601,   841,  1137,  1122,  1735,   239,  1815,   971,   326,
     443,   351,   836,   225,   231,   171,   268,  1104,   971,  1577,
     178,  1706,   556,   898,   332,   403,  1355,   355,  1105,   781,
    1187,   574,   610,   588,   782,   432,  1784,  1230,  1125,   104,
     971,  1127,  1639,   971,  1206,  1832,  1881,  1019,  1020,   997,
    1134,  1231,  1257,  1825,   431,   574,  1376,   960,  1953,   899,
    1356,   104,   968,   823,   307,   781,   859,  1357,  1715,  1358,
     782,   590,   -81,   831,  1277,  1278,   781,   688,   326,   848,
    -377,   782,   692,  1324,   933,  1431,  1208,  1209,  1311,   437,
     972,   532,  1204,  1916,   536,  1232,  1325,   385,   657,  1826,
     781,   829,   592,   889,  -731,   782,   972,   449,   972,  1178,
    1827,  1541,    14,   803,   662,   972,  -691,   972,   327,  1326,
     338,   433,   773,  1233,  1033,   972,   563,   804,   544,  1582,
    1035,    65,   571,  1327,   569,   972,  1274,   386,   251,   810,
     845,   251,   832,   586,    14,   610,  1361,   581,   545,   150,
     396,   926,   575,  1365,   589,  1568,   434,   972,   105,   900,
     972,  1207,  1371,  1352,  1352,  1840,  1694,   308,  1352,  1785,
    1328,  1701,  1702,  1037,   228,  1188,  1618,   934,   171,   924,
     438,  1047,   576,  1271,   242,   243,  1021,  1234,  1393,  1683,
    1684,   272,  1377,   625,   840,   397,  1627,   611,  1716,  1312,
     532,   714,   249,   781,   639,  1484,   576,   643,   782,  1828,
    1354,  1354,   805,  1279,  1898,  1354,   700,   781,  1595,  1202,
     599,   599,   782,   577,   654,   273,   656,  1640,   837,  1917,
    1123,   704,   588,  1816,  1349,  1601,   308,  1564,  1565,  1086,
     251,   709,  1282,   713,  1165,   925,  1205,   251,  1657,   251,
     708,   251,   711,  1733,   644,  1366,  1367,   929,  1141,  1286,
    1591,  1631,  1748,   781,   913,  1355,  1355,  1374,   782,  1763,
    1355,  1875,  1339,   225,   231,   236,  1882,   998,  1338,  1911,
     785,   786,  1329,   171,  1342,   228,  1854,   702,  1286,   722,
    1157,   960,  1359,  1360,  1581,   960,   720,   881,    82,  1356,
    1356,   318,   398,   404,  1356,  1415,  1357,  1357,  1358,  1358,
     658,  1357,  1373,  1358,   783,   784,   785,   786,  1427,  1428,
     399,  1429,  1430,  1184,   318,   783,   784,   785,   786,  1118,
    -691,   781,  1120,  1157,  1200,   777,   782,  1438,  1201,  1113,
     172,   318,   586,   318,   184,  1132,   154,   326,   800,   783,
     784,   785,   786,  1094,   400,  1194,   816,   807,   907,   242,
     243,   812,  1335,   326,  1824,    86,    87,  1375,  1090,  1095,
    1148,  1158,    82,   914,   882,    83,   405,   544,  1492,   171,
    1891,   231,   545,   319,  1493,  1439,  1907,   994,   171,   320,
    1837,  1813,   171,  -239,   802,   536,   185,   545,   850,   178,
    1937,   632,  1770,  1002,  1613,  1050,   323,  1736,  1114,  1562,
    1928,   165,   320,  1336,   883,   860,  1777,  1072,   937,  1776,
    1164,   320,  1918,  1315,    89,  1630,   173,  1494,   880,   628,
    1195,  1404,  1191,   955,  1871,  1872,  1106,  1947,  1744,  1938,
    1786,   107,   251,  1936,  1416,  1913,  1098,  1950,  1952,    83,
    1320,  1955,   783,   784,   785,   786,   909,   174,  1956,  1422,
     320,  1316,  1036,  1590,   908,   152,   783,   784,   785,   786,
    1324,  1280,   175,   873,   988,   251,  1541,   109,   318,   873,
    1341,  1324,  1220,  1325,  1616,  1605,  1939,  1176,   414,  -239,
     390,  1423,  1948,   326,  1325,   318,  1051,   989,  1951,  1672,
    1541,   936,   391,   260,   249,   825,  1326,   326,   940,   941,
    -239,   166,   783,   784,   785,   786,  -239,  1326,   167,   179,
    1327,  1747,   904,   781,   588,    56,  1378,  1541,   782,   153,
    1221,  1327,  1091,  1004,  1673,   392,    57,   393,  1541,  1841,
     272,  1541,   110,    58,   781,   180,  1674,  1006,  1541,   782,
    -239,   251,   111,   990,  1222,   826,   408,  1328,   781,   409,
     781,   415,  1379,   782,  1755,   782,  1779,    59,  1328,  1835,
     781,   318,   905,  1629,   273,   782,   622,   112,   447,   181,
     783,   784,   785,   786,  1344,  1756,   452,   818,   532,   623,
     532,  1757,  1059,   645,   239,  1584,  1363,  1364,  1149,   160,
     161,   410,   787,   646,  1790,  1585,  1954,  1288,  1372,  1435,
    1436,  1091,  1000,  1749,  1648,   182,  1394,  1750,  1751,  1352,
    1352,  1395,  1039,  1070,   334,  1758,  1858,  1743,  1394,    60,
     251,   240,  1859,  1398,   586,   251,   256,   251, -1039,   251,
     249, -1043,   794,   781,   795,   581,   545,  1097,   782,  1604,
    1406,    82,   850,  1778,   171,   231,  1842,   335,   275,  1112,
      61,   231,  1324,   532,    62,  1089,  1354,  1354,   802,  1329,
    1191,   798,   781,   799,  1489,  1325,   171,   782,   171,   869,
    1329,   870,   891,   336,   892,   251,   337,   251,   277,  1139,
    1142,   781,   285,   599,  1742,   787,   782,  1031,  1326,  1150,
    1151,   781,   338,   468,   469,   470,   782,   297,  1143,   781,
     251,   299,  1154,  1617,   782,  1066,  1036,  1067,  1626,   242,
     243,  1355,  1355,  1904,  1576,  1906,   300,  1678,   339,  1925,
    1926,   478,   479,  1910,  1728,  1730,   251,   251,  1181,  1470,
    1182,   304,  1172,  1173,   951,  1175,  1186,   260,  1177,  1328,
     588,   305,  1147,    63,   722,  1356,  1356,   787,  1185,  1289,
     306,  1155,  1357,  1357,  1358,  1358,  1847,  1729,  1731,  1162,
    1848,   312,   783,   784,   785,   786,   313,  1281,  1944,   171,
     317,   260,   260,   333,   260,  1857,  1290,   161,  1291,   171,
    1181,   349,  1340,   783,   784,   785,   786,   722,  1199,  1386,
    1030,  1387,  1861,  -721,   720,   381,   382,   783,   784,   785,
     786,  1822,  1033,   823,  1073,  1391,   384,  1392,  1035,   783,
     784,   785,   786,   388,  1435,  1435,  1262,  1286,   787,  1491,
    1563,   406,  1435,  1435,   417,  1435,  1435,   787,   787,  1649,
    1688,   418,   420,  1435,  1435,  1435,  1435,  1612,  1435,  1435,
    1435,  1435,  1435,  1435,   787,   429,  1737,  1690,   781,  1933,
     586,  1934,   859,   782,  -721,   440,   781,  -721,   239,  1544,
    1091,   782,  1547,   798,  -721,  1935,  -721,  1633,   442,   444,
    -721,  1897,   445,   448,  1899,  -721,  1283,   607,  -721,   608,
     449,   533,   783,   784,   785,   786,  1571,   537,  1573,  1467,
     540,   545,   545,  -721,   547,   781,   548,   551,   850,   552,
     782,   553,   554,   688,   555,   231,   557,  -721,   692,   570,
    1923,   783,   784,   785,   786,  -721,   781,   572,  1468,  -721,
     582,   782,   573, -1068,   593,   595,   596,   597,  -721, -1068,
     783,   784,   785,   786, -1068,  1689,   603, -1068,   601,  1885,
     783,   784,   785,   786,  -721,   615,  1888,  1471,   783,   784,
     785,   786,    14,   616,   617,  1472,  1033,   619,  1033,   637,
    1036,   421,  1035,   640,  1035,   249,  1033,   650,   588,   536,
     651,  -721,  1035, -1068,   655,  1401,   706,   781,   712,   769,
     710, -1068,   782,  -721,  1384,  -721,   747,   749,  1417, -1068,
     750,   751,   753,   781,  1421,   422,  1385,   423,   782,   752,
     754,  1384,   755,   756,  1557,  1557,  1561, -1068,  1442, -1068,
     759,  1033,   251,   760,   260,   757,   251,  1035, -1025,   424,
     758,   761, -1068,   766, -1068, -1068, -1068,   260,   767,   768,
     781,   778,  -721,   260,   780,   782,   787,   789,   788, -1043,
    1587,   793,  1589,   781,   425,   796,   797,   820,   782,   814,
     822,   835,  -721,   839,   843,   846,  -721,  1224,   889,  1228,
    1548,  1236,  1239,   781,  1244,  1247,  1250,   844,   782,  -721,
    -721,   847,  -721, -1068,   532,   851,  1495,   249,   586,   855,
     858,   852,   862,  1466,   171,   781,  1546,   610,  1549,   861,
     782,   864,   865,  1782,   871,   895,   896,   783,   784,   785,
     786,   897,  1632,  -437,  1474,   783,   784,   785,   786,   995,
     902,   999,  1475,   318,   781,  1003,  1005,  1007,   251,   782,
     911,   251,   781,   910,  1500,  1543,   916,   782,   545,   231,
     236,   545,   268,   150,   917,   919,   781,   921,  1914,   918,
     927,   782,  1545,   922,   783,   784,   785,   786,   781,   545,
     545,  1476,   931,   782,   231,   932,   938,   939,   950,   948,
     499,   951,   956,   957,   781,   783,   784,   785,   786,   782,
     958,   184,  1477,   962,  1681, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068,  1022,   560,  1027,   928,   781, -1068, -1069, -1068,
     588,   782,   568,  1028, -1069,   320,  1055,  1060,   781, -1069,
    1061,  1068, -1069,   782,  1270,  1062,   781,  1874,  1064,   805,
    1638,   782,  1085,  1092,  1102,  1643,  1107,  1658,  1109, -1070,
     798,  1130,  1119,  1129,  1615, -1070,   783,   784,   785,   786,
   -1070,  1128,   874, -1070,  1860,  1135,   857,  1136, -1069,  1138,
    1144,  1145,   783,   784,   785,   786, -1069,  1146,   606,  1478,
    1153,  1738,   609,  1170, -1069,  1160,  1179,  1929,  1174,   973,
    1193,  1217,  1261,  1252,  1253,   781,  1259,   781,  1286, -1070,
     782,  1269, -1069,   974, -1069,  1693,  1292, -1070,  1287,   783,
     784,   785,   786,  1294,  1310, -1070,  1479, -1069,  1313, -1069,
   -1069, -1069,   783,   784,   785,   786,  1466,   171,  1318,  1679,
     586,  1319,  1323, -1070,  1333, -1070,  1331,  1345,  1380,   975,
    1348,  1368,   783,   784,   785,   786,  1370,   976, -1070,  1680,
   -1070, -1070, -1070,  1390,  1403,   977,  1388,  1396,  1411,   -49,
     106,  1399,   225,  1400,   783,   784,   785,   786, -1069,  1187,
    1700,  1199,  1199,   978,  1402,   979,  1419,  1543,  1424,   763,
    1425,   971,  1426,  1432,  1433,   770,   771,   774,   980,  1440,
     981,   982,   983,   783,   784,   785,   786,   -49,   107, -1070,
    1687,   783,   784,   785,   786,  1437,  1441,  1483,  1870,   -49,
     108,  1443,  1444,   545,   545,   783,   784,   785,   786,  1451,
    1452,  1485,  1453,   155,  1036,  1454,  1456,   783,   784,   785,
     786,  1550,  1761,   -49,   109,   231,   231,  1839,   351,   984,
    1457,  1569,  1458,   783,   784,   785,   786,  1459,  1460,  1574,
    1461,  1552,  1553,   817,  1575,  1557,  1578,   819,  1579,  1566,
    1588,  1567,  1602,   171,  1606,   783,   784,   785,   786,  1157,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069,   783,   784,   785,
     786,  1614, -1069,  1607, -1069,   783,   784,   785,   786,  1628,
    1608,  1609,  1610,  1810,  1634,  1611,  1636,  1651,   -49,   110,
    1637, -1070, -1070, -1070, -1070, -1070, -1070, -1070,   -49,   111,
    1642,  1685,  1647, -1070,  1671, -1070,  1008,  1009,  1010,  1011,
    1012,  1013,  1014,  1015,  1855,  1670,  1691,  1692,  1677, -1031,
    1686,  1466,   171,   -49,   112,   228,  1695,  1696,   153,  1704,
    1698,   985,   986,   987,   783,   784,   785,   786,  1645,  1646,
    1720,  1030,  -339,  1709,  1710,   156,  1652,  1653,  1711,  1655,
    1656,  1712,  1713,  1714,  1199,     1,  1717,  1660,  1661,  1662,
    1663,  1718,  1664,  1665,  1666,  1667,  1668,  1669,  1036,  1719,
    1036,  1856,  1726,     2,  1721,   588,  1833,  1853,  1036,  1722,
    1727,     3,  1723,  1732,   351,   231,  1724,  1725,     4,   823,
       5,  1734,     6,  1752,  1863,  1762,  1764,     7,  1865,   588,
    1765,  1766,  1767,  1039,     8,  1768,  1892,  1769,   171,  1771,
    1772,  1773,  1781,   228,   970,  1783,  1787,  1788,     9,  1789,
    1877,  1792,  1819,  1036,  1793,  1705,   588,  1809,    10,  1794,
    1795,  1798,  1799,  1836,   996,  1851,  1834,   588,  1001,  1852,
     588,  1800,  1864,  1801,  1802,  1017,  1803,   588,  1804,  1805,
    1024,  1025,  1806,  1807,  1846,  1812,  1868,  1849,  1873,  1883,
     228,  1886,  1876,  1884,   499,  1889,  1040,  1041,  1042,  1043,
    1044,  1045,   499,  1893,  1900,  1901,  1905,   225,  1879,  1902,
    1932,    67,  1919,  1924,  1921,   586,  1880,  1927,  1917,  1916,
    1942,    11,    68,   536,  1945,  1946,  1943,  1949,    12,    69,
    1958,    13,  1959,    14,    15,   813,   568,   314,   539,   586,
    1554,   315,    70,  1071,   316,   546,  1063,  1420,  1192,   815,
    1197,  1418,   949,  1408,  1166,  1347,   271,  1100,    16,  1775,
     912,   620,  1543,  1103,  1903,   663,   586,  1089,   591,   606,
     251,  1111,  1133,  1414,   633,  1774,  1760,   586,  1212,  1409,
     586,  1213,   228,   228,  1915,   453,  1543,   586,  1754,  1922,
    1920,  1941,   454,  1831,  1126,  1296,  1908,    28,  1745,    71,
    1052,  1171,  1583,   627,   455,   935,   635,    17,   618,   894,
     456,   457,  1625,  1543,  1089,    72,  1163,   159,   583,   830,
     834,  1093,    18,    19,  1543,   641,  1199,  1543,   331,  1087,
    1322,    73,   281,  1572,  1543,  1890,   991,  1808,   458,  1676,
    1108,   459,    48,    49,    52,    20,    74,   842,  1317,  1592,
      75,  1069,  1447,    76,  1397,  1487,  1850,  1156,   890,   286,
    1699,    77,     0,     0,     0,     0,   460,     0,   461,   462,
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
       0,     0,     0,     0,   491,   492,     0,   467,     0,     0,
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
       0,     0,   973,     0,     0,  1455,     0,   460,   781,   461,
     462,     0,     0,   782,     0,     0,   974,     0,     0,   463,
       0,     0,     0,   464,     0,  1473,     0,     0,     0,     0,
       0,     0,     0,  1480,  1481,  1272,     0,     0,     0,     0,
       0,  1272,     0,  1275,  1488,   499,  1490,     0,     0,     0,
       0,     0,   975,     0,     0,     0,     0,     0,     0,     0,
     976,     0,     0,     0,     0,     0,     0,     0,   977,     0,
       0,     0,     0,     0,     0,     0,  1214,     0,   465,     0,
       0,     0,     0,     0,   466,     0,   978,     0,   979,     0,
       0,     0,     0,     0,   467,   499,     0,     0,     0,     0,
       0,   980,     0,   981,   982,   983,     0,     0,   468,   469,
     470,   471,   242,   243,   114,     0,   115,   116,   117,   118,
       0,   119,   472,     0,     0,     0,     0,  1596,     0,   120,
     121,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     122,   482,     0,   124,     0,     0,   483,   484,     0,   485,
     486,   487,   984,   127,   128,   488,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,     0,   135,   136,     0,
       0,     0,     0,   137,   489,   139,     0,   490,   141,   142,
       0,     0,   361,     0,     0,   491,   492,     0,     0,     0,
       0,   559,     0,   494,     0,  1644,     0,   320,   362,     0,
       0,  1650,     0,     0,     0,     0,  1654,     0,     0,   363,
       0,     0,     0,  1659,     0,    90,   364,     0,     0,     0,
     365,   366,    91,     0,     0,     0,    92,    93,     0,   367,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
     368,     0,     0,     0,     0,     0,    95,     0,  1272,  1272,
       0,     0,     0,     0,   985,   986,   987,   783,   784,   785,
     786,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1697,  1227,     0,     0,  1213,     0,     0,     0,   453,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,   455,     0,     0,
       0,     0,   370,   456,   457,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,   458,     0,   371,   459,     0,     0,   372,     0,    98,
       0,     0,  1741,    99,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,   100,     0,     0,     0,     0,   460,
       0,   461,   462,     0,     0,     0,     0,   374,   375,     0,
       0,   463,     0,   101,     0,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1791,   874,     0,     0,     0,
       0,  1796,  1797,     0,     0,     0,   376,     0,  1214,   377,
     465,     0,   102,     0,     0,     0,   466,     0,     0,     0,
       0,     0,     0,     0,     0,  1811,   467,     0,     0,     0,
    1272,     0,     0,  1814,     0,     0,     0,  1820,  1821,     0,
     468,   469,   470,   471,   242,   243,   114,     0,   115,   116,
     117,   118,     0,   119,   472,     0,     0,     0,     0,     0,
       0,   120,   121,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   122,   482,     0,   124,     0,     0,   483,   484,
       0,   485,   486,   487,     0,   127,   128,   488,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,  1235,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,     0,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,     0,   494,  1862,   455,     0,   320,
       0,     0,     0,   456,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   458,     0,     0,   459,     0,     0,   113,     0,     0,
     114,     0,   115,   116,   117,   118,     0,   119,     0,     0,
       0,     0,     0,     0,     0,   120,   121,     0,     0,   460,
    1741,   461,   462,     0,     0,     0,   122,   123,     0,   124,
       0,   463,     0,     0,     0,   464,   125,   126,     0,   127,
     128,     0,   129,     0,     0,   130,   131,   132,   133,   134,
       0,     0,     0,   135,   136,     0,     0,  1909,     0,   137,
     138,   139,     0,   140,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   300,     0,   632,
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
     141,   142,     0,     0,     0,   454,     0,   491,   492,     0,
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
     138,   139,     0,   140,   141,   142,     0,     0,     0,     0,
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
     136,  1243,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,     0,     0,   454,     0,   491,   492,     0,
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
     138,   139,     0,   140,   141,   142,     0,     0,     0,     0,
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
     136,  1246,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,     0,     0,   454,     0,   491,   492,     0,
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
     137,   138,   139,     0,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,  1049,     0,     0,     0,     0,     0,
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
     136,  1249,     0,     0,  1213,   137,   489,   139,   453,   490,
     141,   142,     0,     0,     0,   454,     0,   491,   492,     0,
       0,     0,     0,   559,    14,   494,     0,   455,     0,   320,
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
     138,   139,     0,   140,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1074,     0,     0,
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
     141,   142,   990,     0,     0,     0,     0,   491,   492,     0,
       0,   724,     0,   559,     0,   494,     0,     0,     0,   320,
     458,     0,     0,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,     0,   460,     0,
     461,   462,     0,     0,   120,   121,     0,     0,     0,     0,
     463,     0,     0,     0,   464,   122,   123,     0,   124,     0,
       0,     0,   963,     0,     0,   125,   126,     0,   127,   128,
       0,   129,     0,     0,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,     0,     0,     0,     0,   137,   138,
     139,     0,   140,   141,   142,     0,     0,     0,     0,    14,
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
     142,     0,     0,     0,   454,     0,   491,   492,     0,     0,
       0,     0,   730,     0,   494,     0,   455,     0,   320,   723,
       0,     0,   456,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
       0,   724,     0,     0,     0,     0,     0,     0,     0,     0,
     458,     0,     0,   459,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,     0,     0,     0,
       0,     0,     0,     0,   120,   121,     0,     0,   460,     0,
     461,   462,     0,     0,     0,   122,   123,     0,   124,     0,
     463,     0,     0,     0,   464,   125,   126,     0,   127,   128,
       0,   129,   725,     0,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,     0,     0,     0,     0,   137,   138,
     139,     0,   140,   141,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     142,     0,     0,     0,     0,     0,   491,   492,     0,     0,
     724,     0,   730,     0,   494,     0,     0,     0,   320,   458,
       0,     0,   459,     0,     0,   113,     0,     0,   114,     0,
     115,   116,   117,   118,     0,   119,     0,     0,     0,     0,
       0,     0,     0,   120,   121,     0,     0,   460,     0,   461,
     462,     0,     0,     0,   122,   123,     0,   124,     0,   463,
       0,     0,     0,   464,   125,   126,     0,   127,   128,     0,
     129,   959,     0,   130,   131,   132,   133,   134,     0,     0,
       0,   135,   136,     0,     0,     0,     0,   137,   138,   139,
       0,   140,   141,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1032,     0,   494,     0,     0,
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
       0,     0,     0,     0,     0,   491,   492,     0,     0,   724,
       0,   730,     0,   494,     0,     0,     0,   320,   458,     0,
       0,   459,     0,     0,   113,     0,     0,   114,     0,   115,
     116,   117,   118,     0,   119,     0,     0,     0,     0,     0,
       0,     0,   120,   121,     0,     0,   460,     0,   461,   462,
       0,     0,     0,   122,   123,     0,   124,     0,   463,     0,
       0,     0,   464,   125,   126,     0,   127,   128,     0,   129,
     725,     0,   130,   131,   132,   133,   134,     0,     0,     0,
     135,   136,     0,     0,     0,     0,   137,   138,   139,     0,
     140,   141,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,     0,     0,     0,
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
       0,     0,     0,     0,   491,   492,     0,     0,   724,     0,
     730,     0,   494,     0,     0,     0,   320,   458,     0,     0,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,   114,     0,   115,
     116,   117,   118,     0,   119,   460,     0,   461,   462,     0,
       0,     0,   120,   121,     0,     0,     0,   463,     0,     0,
       0,   464,     0,   122,   123,     0,   124,     0,     0,   959,
       0,     0,     0,   125,   126,     0,   127,   128,     0,   129,
       0,     0,   130,   131,   132,   133,   134,     0,     0,   106,
     135,   136,     0,     0,     0,     0,   137,   138,   139,     0,
     140,   141,   142,     0,     0,     0,     0,     0,     0,     0,
     726,     0,     0,     0,  1074,     0,   465,     0,     0,     0,
       0,     0,   466,     0,     0,     0,     0,   107,     0,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,   108,
       0,     0,     0,     0,     0,     0,   468,   469,   470,   471,
     242,   243,   114,     0,   115,   116,   117,   118,     0,   119,
     472,     0,     0,   109,     0,     0,     0,   120,   121,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   122,   482,
       0,   124,     0,   453,   483,   484,     0,   485,   486,   487,
     454,   127,   128,   488,   129,     0,     0,   130,   131,   132,
     133,   134,   455,     0,     0,   135,   136,     0,   456,   457,
       0,   137,   489,   139,     0,   490,   141,   142,     0,     0,
       0,     0,     0,   491,   492,     0,     0,     0,   110,   730,
       0,   494,     0,     0,     0,   320,   458,     0,   111,   459,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,   460,     0,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,     0,     0,     0,
     464,   113,     0,     0,   114,     0,   115,   116,   117,   118,
       0,   119,     0,     0,     0,     0,     0,     0,     0,   120,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     122,   123,     0,   124,     0,     0,     0,     0,     0,     0,
     125,   126,     0,   127,   128,     0,   129,     0,     0,   130,
     131,   132,   133,   134,     0,   465,     0,   135,   136,     0,
       0,   466,     0,   137,   138,   139,     0,   140,   141,   142,
       0,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   468,   469,   470,   471,   242,
     243,   114,     0,   115,   116,   117,   118,     0,   119,   472,
       0,     0,     0,     0,     0,     0,   120,   121,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   122,   482,     0,
     124,     0,   453,   483,   484,     0,   485,   486,   487,   454,
     127,   128,   488,   129,     0,     0,   130,   131,   132,   133,
     134,   455,     0,     0,   135,   136,     0,   456,   457,     0,
     137,   489,   139,     0,   490,   141,   142,     0,     0,     0,
       0,     0,   491,   492,   564,     0,     0,     0,   559,     0,
     494,     0,     0,     0,   320,   458,     0,     0,   459,     0,
       0,   113,     0,     0,   114,     0,   115,   116,   117,   118,
       0,   119,     0,     0,     0,     0,     0,     0,     0,   120,
     121,     0,     0,   460,     0,   461,   462,     0,     0,     0,
     122,   123,     0,   124,     0,   463,     0,     0,     0,   464,
     125,   126,     0,   127,   128,     0,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,     0,   135,   136,     0,
       0,     0,     0,   137,   138,   139,     0,   140,   141,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -340,
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
     489,   139,     0,   490,   141,   142,     0,     0,     0,     0,
       0,   491,   492,     0,     0,     0,     0,   559,     0,   494,
       0,     0,     0,   320,   458,     0,     0,   459,    82,     0,
     113,     0,     0,   114,     0,   115,   116,   117,   118,     0,
     119,     0,     0,     0,     0,     0,     0,     0,   120,   121,
       0,     0,   460,     0,   461,   462,     0,     0,     0,   122,
     123,     0,   124,     0,   463,     0,     0,     0,   464,   125,
     126,     0,   127,   128,     0,   129,     0,     0,   130,   131,
     132,   133,   134,     0,     0,     0,   135,   136,     0,     0,
       0,     0,   137,   138,   139,     0,   140,   141,   142,     0,
       0,     0,     0,     0,     0,    83,     0,     0,     0,     0,
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
     139,     0,   490,   141,   142,     0,     0,     0,     0,     0,
     491,   492,     0,     0,     0,     0,   559,     0,   494,     0,
       0,     0,   320,   458,     0,     0,   459,     0,     0,   113,
       0,     0,   114,     0,   115,   116,   117,   118,     0,   119,
       0,     0,     0,     0,     0,     0,     0,   120,   121,     0,
       0,   460,     0,   461,   462,     0,     0,     0,   122,   123,
       0,   124,     0,   463,     0,     0,     0,   464,   125,   126,
       0,   127,   128,     0,   129,     0,     0,   130,   131,   132,
     133,   134,     0,     0,     0,   135,   136,     0,     0,     0,
       0,   137,   138,   139,     0,   140,   141,   142,     0,     0,
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
       0,   490,   141,   142,     0,     0,     0,     0,     0,   491,
     492,     0,     0,     0,     0,   559,  1046,   494,     0,     0,
       0,   320,   458,     0,     0,   459,     0,     0,   113,     0,
       0,   114,     0,   115,   116,   117,   118,     0,   119,     0,
       0,     0,     0,     0,     0,     0,   120,   121,     0,     0,
     460,     0,   461,   462,     0,     0,     0,   122,   123,     0,
     124,     0,   463,     0,     0,     0,   464,   125,   126,     0,
     127,   128,     0,   129,     0,     0,   130,   131,   132,   133,
     134,     0,     0,     0,   135,   136,     0,     0,     0,     0,
     137,   138,   139,     0,   140,   141,   142,     0,     0,     0,
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
     490,   141,   142,     0,     0,     0,     0,     0,   491,   492,
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
     141,   142,     0,     0,     0,     0,     0,   491,   492,     0,
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
     142,     0,     0,     0,     0,     0,   491,   492,     0,     0,
       0,     0,   493,     0,   494,     0,     0,     0,   320,   458,
       0,     0,   459,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   464,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1294,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   465,     0,
       0,     0,     0,     0,   466,     0,     0,     0,     0,     0,
       6,     0,     0,     0,   467,     0,     0,  -294,     0,     0,
       0,  1501,  1502,     0,  1503,     0,     0,     0,   468,   469,
     470,   471,   242,   243,   114,     0,   115,   116,   117,   118,
    1504,   119,   472,     0,     0,     0,  1505,     0,     0,   120,
     121,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     122,   482,     0,   124,     0,     0,   483,   484,     0,   485,
     486,   487,     0,   127,   128,   488,   129,     0,     0,   130,
     131,   132,   133,   134,     0,     0,     0,   135,   136,     0,
    1294,     0,     0,   137,   489,   139,     0,   490,   141,   142,
       0,     0,     0,  1506,     0,   491,   492,     0,     0,     0,
       0,   559,     0,   494,     0,     0,     0,   320,     6,     0,
       0,    14,     0,     0,     0,  -296,     0,     0,     0,  1501,
    1502,     0,  1503,     0,     0,     0,  1507,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,  1504,     0,
       0,     0,     0,     0,  1505,  1705,     0,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,   114,     0,   115,
     116,   117,   118,     0,   119,     0,     0,     0,     0,     0,
       0,     0,   120,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1508,   123,     0,   124,     0,     0,     0,
       0,     0,     0,   125,   126,     0,   127,   128,     0,   129,
       0,  1506,   130,  1509,   132,   133,   134,     0,     0,     0,
     135,   136,  1510,  1511,  1512,  1294,   137,   138,   139,    14,
     140,   141,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1507,     0,     0,     0,     0,     0,
       0,     0,     0,     6,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1501,  1502,     0,  1503,     0,     0,
       0,     0,   471,     0,     0,   114,     0,   115,   116,   117,
     118,     0,   119,  1504,     0,     0,     0,     0,     0,  1505,
     120,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1508,   123,     0,   124,     0,     0,     0,     0,     0,
       0,   125,   126,     0,   127,   128,     0,   129,     0,     0,
     130,  1509,   132,   133,   134,     0,     0,     0,   135,   136,
    1510,  1511,  1512,     0,   137,   138,   139,     0,   140,   141,
     142,     0,     0,     0,     0,     0,  1506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
     114,     0,   115,   116,   117,   118,     0,   119,   664,     0,
       0,   665,     0,     0,     0,   120,   121,   874,     0,     0,
       0,     0,     0,     0,     0,     0,  1508,   123,   666,   124,
       0,   667,     0,     0,     0,     0,   125,   126,     0,   127,
     128,     0,   129,     0,     0,   130,  1509,   132,   133,   134,
       0,     0,     0,   135,   136,  1510,  1511,  1512,     0,   137,
     138,   139,     0,   140,   141,   142,     0,     0,     0,     0,
       0,   668,     0,     0,     0,     0,     0,   669,   670,   671,
       0,   672,   673,   674,     0,   675,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   676,     0,   677,   664,     0,     0,   665,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   666,     0,     0,   667,
       0,     0,     0,     0,   678,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   679,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   668,
       0,     0,     0,   680,     0,   669,   670,   671,     0,   672,
     673,   674,     0,   675,     0,     0,     0,     0,   113,     0,
       0,   114,     0,   115,   116,   117,   118,     0,   119,     0,
       0,   676,     0,   677,     0,     0,   120,   121,   681,   682,
       0,     0,     0,     0,     0,     0,     0,   122,   123,     0,
     124,     0,     0,     0,     0,   683,     0,   125,   126,     0,
     127,   128,   678,   129,     0,     0,   130,   131,   132,   133,
     134,     0,     0,     0,   135,   136,     0,     0,     0,   679,
     137,   138,   139,     0,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   680,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,   114,
       0,   115,   116,   117,   118,     0,   119,   952,     0,     0,
     665,     0,     0,     0,   120,   121,   681,   682,     0,     0,
       0,     0,     0,   953,     0,   122,   123,   666,   124,     0,
     667,     0,     0,   683,     0,   125,   126,     0,   127,   128,
       0,   129,     0,     0,   130,   131,   132,   133,   134,     0,
       0,   664,   135,   136,   665,     0,     0,     0,   137,   138,
     139,     0,   140,   141,   142,     0,  1297,     0,     0,     0,
     668,   666,     0,     0,   667,     0,   669,   670,   671,     0,
     672,   673,   674,     0,   675,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   676,     0,   677,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   668,     0,     0,     0,     0,     0,
     669,   670,   671,     0,   672,   673,   674,     0,   675,     0,
       0,     0,     0,   678,     0,     0,  1298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   676,     0,   677,     0,
     679,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1299,   680,     0,     0,     0,  1300,   678,     0,     0,
       0,     0,  1301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,   679,     0,   114,     0,   115,   116,
     117,  1302,     0,   119,     0,     0,     0,   681,   682,     0,
       0,   120,   121,     0,     0,     0,   680,     0,     0,     0,
       0,     0,   122,   123,   683,   124,     0,     0,     0,     0,
       0,     0,   125,   126,     0,   127,   128,     0,   129,     0,
       0,   130,   131,   132,   133,   134,     0,     0,     0,   135,
     136,   681,   682,     0,     0,   137,   138,   139,     0,   140,
     141,   142,     0,     0,     0,     0,   113,     0,   683,   114,
     188,   115,   116,   117,   118,   189,   119,   190,   191,   192,
     193,   194,   195,   196,   120,   121,   197,   198,   199,   200,
     201,     0,     0,   202,   203,   122,   123,   204,   124,     0,
       0,   205,   206,   207,   208,   125,   126,   209,   127,   128,
     210,   129,   211,   212,   130,   131,   132,   133,   134,     0,
       0,     0,   135,   136,   213,   214,   215,   216,   137,   138,
     139,   217,   140,   141,   142,   218,   219
};

static const short yycheck[] =
{
       9,     0,     4,    69,   170,   112,    16,    16,    59,    18,
     601,    68,    68,   160,   161,   398,   160,   161,    67,   100,
     778,    61,    75,   430,    78,    91,   778,   762,   430,   307,
     426,  1032,   400,   768,    74,   493,   493,   606,   841,  1294,
     528,   634,   570,   422,    95,  1367,    99,   101,    57,    58,
      59,    60,    61,    62,    63,   548,  1074,   789,    97,    68,
      69,    70,  1056,    72,    60,    74,    75,  1127,    77,    78,
      79,   322,    63,    73,  1120,   549,    72,   748,    87,    89,
      89,    90,    91,   796,    93,    94,    95,    96,    98,    98,
      99,   100,   101,   102,   175,   104,   152,    97,   154,   993,
      96,   559,   773,    94,  1293,   349,   172,   407,   104,  1161,
     718,   102,   844,   153,   380,   711,   406,   946,  1178,  1183,
    1312,  1120,   650,     8,   258,     8,  1271,   179,  1359,  1360,
    1061,   725,  1277,  1364,   174,  1029,   436,  1633,   626,     8,
     628,     8,    10,   152,   153,   154,   634,    21,     8,   158,
       8,  1365,   584,   883,    43,  1574,   165,    43,     8,     6,
     267,   170,    61,   172,   173,   174,   175,   847,     8,  1332,
     179,  1516,    63,    44,   165,   224,  1120,   173,   851,    23,
     133,    26,    90,   349,    28,    55,    97,    17,   868,   141,
       8,   871,    19,     8,     8,    56,    47,    50,    51,    46,
     880,    31,    10,     9,   253,    26,    44,   725,    47,    80,
    1120,   141,   730,    10,   174,    23,   612,  1120,    61,  1120,
      28,   355,   149,   117,    50,    51,    23,   634,     6,    94,
      22,    28,   634,    64,   185,     8,   971,   972,    63,    14,
     125,   297,   125,    44,   300,    75,    77,     6,    43,    55,
      23,   143,   359,   636,    85,    28,   125,   190,   125,   939,
      66,  1516,   155,   541,   660,   125,    63,   125,    46,   100,
     162,   141,   730,   103,  1032,   125,   323,   192,   556,  1339,
    1032,   193,   333,   114,   331,   125,  1021,    46,   297,   182,
     590,   300,   186,   349,   155,    90,  1124,   337,   307,   308,
       4,   684,   123,  1131,   353,  1323,   176,   125,   260,   180,
     125,   125,  1140,  1359,  1360,  1734,   246,   277,  1364,   230,
     151,  1510,  1511,   780,   333,   278,   118,   278,   337,    21,
     105,   788,   177,   173,   199,   200,   189,   167,  1166,  1484,
    1485,    11,   180,   399,   277,    49,  1398,   255,   191,   174,
     406,   278,   408,    23,   410,   173,   177,   413,    28,   165,
    1359,  1360,   277,   189,  1860,  1364,   432,    23,  1362,   963,
     379,   380,    28,   194,   423,    45,   425,   204,   277,   180,
     269,   434,   548,   269,  1119,  1599,   277,  1318,  1319,   821,
     399,   440,   277,   444,   922,    87,   279,   406,     8,   408,
      90,   410,   442,  1566,   413,  1135,  1136,   281,   896,   277,
     279,  1405,   279,    23,   169,  1359,  1360,    80,    28,   279,
    1364,   279,  1102,   432,   433,   434,   277,   274,  1099,   279,
     274,   275,   263,   442,  1107,   444,  1758,   433,   277,   448,
     196,   959,  1122,  1123,  1338,   963,   448,    62,   196,  1359,
    1360,   196,   156,     3,  1364,  1185,  1359,  1360,  1359,  1360,
     255,  1364,  1142,  1364,   272,   273,   274,   275,  1203,  1204,
     174,  1206,  1207,   947,   196,   272,   273,   274,   275,   862,
     277,    23,   865,   196,   958,   494,    28,    46,   962,    98,
      55,   196,   548,   196,   126,   878,    18,     6,   538,   272,
     273,   274,   275,   142,   208,    19,   558,   547,   652,   199,
     200,   551,   143,     6,  1703,    59,    60,   180,   223,   158,
     903,   277,   196,   278,   139,   273,    76,   805,  1286,   538,
    1852,   540,   541,   278,  1286,    94,  1881,    46,   547,   284,
    1732,  1686,   551,    22,   540,   601,   178,   556,   597,   558,
     117,    30,  1612,    46,  1382,  1287,   278,  1575,   167,  1313,
    1905,    68,   284,   194,   179,   614,  1630,   818,   702,  1629,
     273,   284,  1894,  1066,    63,  1404,   141,  1290,   634,   869,
      94,  1177,   950,  1191,  1815,  1816,   852,  1932,  1582,   156,
    1642,    44,   601,  1915,  1185,  1886,   840,    43,  1943,   273,
    1074,  1946,   272,   273,   274,   275,   655,   172,  1953,   230,
     284,  1068,   778,  1348,   653,    18,   272,   273,   274,   275,
      64,   277,   187,   632,    85,   634,  1881,    80,   196,   638,
    1104,    64,    46,    77,  1388,  1370,   203,   937,     3,   118,
       4,   262,  1933,     6,    77,   196,   793,   108,    94,    79,
    1905,   700,    16,   653,   710,   223,   100,     6,   707,   708,
     139,   168,   272,   273,   274,   275,   145,   100,   175,   244,
     114,  1587,   223,    23,   840,    37,    44,  1932,    28,    82,
      94,   114,   826,    46,   114,    49,    48,    51,  1943,   122,
      11,  1946,   145,    55,    23,   122,   126,    46,  1953,    28,
     179,   710,   155,   164,   118,   273,     4,   151,    23,     7,
      23,    76,    80,    28,   118,    28,  1632,    79,   151,  1720,
      23,   196,   273,  1403,    45,    28,    30,   180,   277,   276,
     272,   273,   274,   275,  1113,   139,   285,   279,   794,    43,
     796,   145,   798,     5,   753,   231,  1129,  1130,   223,   231,
     232,    49,   277,    15,  1648,   241,  1945,   282,  1141,  1217,
    1217,   905,   753,  1591,  1435,    61,   277,  1595,  1596,  1815,
    1816,   282,   781,   813,    84,   179,  1770,  1580,   277,   141,
     789,   147,  1776,   282,   840,   794,     7,   796,   280,   798,
     846,   283,   277,    23,   279,   835,   805,   837,    28,  1368,
    1179,   196,   851,  1631,   813,   814,   239,   117,   196,   858,
     172,   820,    64,   869,   176,   824,  1815,  1816,   814,   263,
    1188,   277,    23,   279,  1281,    77,   835,    28,   837,   277,
     263,   279,   203,   143,   205,   844,   146,   846,    65,   895,
     896,    23,    65,   852,  1579,   277,    28,   279,   100,   130,
     131,    23,   162,   195,   196,   197,    28,   278,   897,    23,
     869,   140,   911,  1391,    28,   277,  1032,   279,  1396,   199,
     200,  1815,  1816,  1874,  1331,  1876,   278,  1470,   188,  1901,
    1902,   223,   224,  1884,  1552,  1553,   895,   896,   277,  1257,
     279,   122,   931,   932,   277,   934,   279,   897,   938,   151,
    1066,   277,   902,   265,   913,  1815,  1816,   277,   948,   279,
     122,   913,  1815,  1816,  1815,  1816,  1744,  1552,  1553,   919,
    1748,   124,   272,   273,   274,   275,   124,   277,  1929,   938,
     174,   931,   932,    79,   934,  1763,   277,   232,   279,   948,
     277,   156,   279,   272,   273,   274,   275,   956,   957,   277,
     279,   279,  1780,     0,   956,   180,   192,   272,   273,   274,
     275,  1696,  1720,    10,   279,   277,    19,   279,  1720,   272,
     273,   274,   275,   259,  1432,  1433,   279,   277,   277,   279,
     279,   278,  1440,  1441,   266,  1443,  1444,   277,   277,   279,
     279,   162,   199,  1451,  1452,  1453,  1454,  1380,  1456,  1457,
    1458,  1459,  1460,  1461,   277,    10,   279,  1495,    23,   277,
    1066,   279,  1408,    28,    61,    59,    23,    64,  1027,  1297,
    1164,    28,  1300,   277,    71,   279,    73,  1410,    61,    79,
      77,  1859,   198,    78,  1862,    82,  1027,   385,    85,   387,
     190,   149,   272,   273,   274,   275,  1326,   172,  1328,   279,
     141,  1060,  1061,   100,    82,    23,   278,    63,  1107,    61,
      28,    61,   122,  1470,   122,  1074,   244,   114,  1470,   174,
    1898,   272,   273,   274,   275,   122,    23,   102,   279,   126,
     172,    28,   188,    17,   123,   156,    65,    83,   135,    23,
     272,   273,   274,   275,    28,   277,   243,    31,   121,  1834,
     272,   273,   274,   275,   151,   254,  1841,   279,   272,   273,
     274,   275,   155,   255,   254,   279,  1874,    30,  1876,   207,
    1286,    98,  1874,   277,  1876,  1181,  1884,   192,  1294,  1185,
     269,   178,  1884,    67,   112,  1174,    29,    23,    10,    97,
     278,    75,    28,   190,  1153,   192,   135,   278,  1187,    83,
     278,   278,   278,    23,  1193,   132,  1158,   134,    28,   199,
     278,  1170,   278,   278,  1311,  1312,  1313,   101,    54,   103,
     199,  1929,  1181,   199,  1174,   278,  1185,  1929,   278,   156,
     278,   278,   116,   278,   118,   119,   120,  1187,   278,   278,
      23,   148,   239,  1193,   281,    28,   277,   283,   278,   283,
    1344,   269,  1346,    23,   181,   278,    10,    63,    28,   141,
     277,   277,   259,   166,   269,   278,   263,   978,  1601,   980,
    1301,   982,   983,    23,   985,   986,   987,   283,    28,   276,
     277,    10,   279,   167,  1290,    83,  1293,  1293,  1294,   277,
      90,   283,    22,  1253,  1253,    23,  1299,    90,  1302,    90,
      28,    22,    43,  1636,    10,   174,   208,   272,   273,   274,
     275,   136,  1406,   174,   279,   272,   273,   274,   275,   749,
     136,   751,   279,   196,    23,   755,   756,   757,  1287,    28,
      90,  1290,    23,    21,  1293,  1294,   185,    28,  1297,  1298,
    1299,  1300,  1301,  1302,   185,   278,    23,    91,  1889,   138,
     281,    28,  1298,   278,   272,   273,   274,   275,    23,  1318,
    1319,   279,   278,    28,  1323,   278,    61,    10,   149,   122,
     296,   277,   128,   121,    23,   272,   273,   274,   275,    28,
     278,   126,   279,   278,    61,   269,   270,   271,   272,   273,
     274,   275,   189,   319,    61,    21,    23,   281,    17,   283,
    1516,    28,   328,   279,    23,   284,    42,   174,    23,    28,
     174,   278,    31,    28,    63,   202,    23,    82,    60,   277,
    1419,    28,   190,   186,    10,  1424,    83,    54,   240,    17,
     277,    43,   278,   278,  1386,    23,   272,   273,   274,   275,
      28,    19,    27,    31,  1777,    86,   174,    86,    67,    22,
     125,   125,   272,   273,   274,   275,    75,   125,   384,   279,
     156,  1577,   388,   156,    83,   154,    98,    82,   278,    17,
     278,   278,   277,   108,   278,    23,   279,    23,   277,    67,
      28,    63,   101,    31,   103,  1501,   184,    75,   283,   272,
     273,   274,   275,    16,   124,    83,   279,   116,   107,   118,
     119,   120,   272,   273,   274,   275,  1466,  1466,    63,   279,
    1516,    63,   277,   101,    71,   103,   278,    12,    10,    67,
     278,   239,   272,   273,   274,   275,   278,    75,   116,   279,
     118,   119,   120,   279,    10,    83,   160,   280,   192,     6,
       6,   279,  1501,   279,   272,   273,   274,   275,   167,   133,
    1509,  1510,  1511,   101,   279,   103,    52,  1516,    52,   485,
     279,     8,   279,   278,   278,   491,   492,   493,   116,   278,
     118,   119,   120,   272,   273,   274,   275,    44,    44,   167,
     279,   272,   273,   274,   275,    63,   278,    51,   279,    56,
      56,   278,   278,  1552,  1553,   272,   273,   274,   275,   278,
     278,   173,   278,    70,  1720,   278,   278,   272,   273,   274,
     275,   192,  1602,    80,    80,  1574,  1575,  1733,  1577,   167,
     278,   127,   278,   272,   273,   274,   275,   278,   278,    85,
     278,   277,   277,   559,    85,  1732,    19,   563,    73,   279,
     130,   279,   145,  1602,   125,   272,   273,   274,   275,   196,
     269,   270,   271,   272,   273,   274,   275,   272,   273,   274,
     275,   279,   281,   125,   283,   272,   273,   274,   275,   279,
     125,   125,   125,  1674,    22,   125,   278,    63,   145,   145,
     279,   269,   270,   271,   272,   273,   274,   275,   155,   155,
     279,    51,   279,   281,   277,   283,   210,   211,   212,   213,
     214,   215,   216,   217,  1761,   279,   276,    36,   279,    36,
     173,  1671,  1671,   180,   180,  1674,   276,   278,    82,    51,
     276,   269,   270,   271,   272,   273,   274,   275,  1432,  1433,
      82,   279,   276,   276,   276,   202,  1440,  1441,   276,  1443,
    1444,   276,   276,   276,  1703,     7,   276,  1451,  1452,  1453,
    1454,   276,  1456,  1457,  1458,  1459,  1460,  1461,  1874,   276,
    1876,  1762,   280,    25,   276,  1881,  1715,  1757,  1884,   276,
      70,    33,   276,   174,  1733,  1734,   276,   276,    40,    10,
      42,    85,    44,    23,  1783,    79,   180,    49,  1787,  1905,
      80,   180,    44,  1752,    56,    80,  1853,    44,  1757,   279,
     279,    94,   124,  1762,   730,   111,   111,   279,    70,   279,
    1826,   279,    61,  1929,   279,   189,  1932,   278,    80,   279,
     279,   279,   279,   124,   750,    94,   278,  1943,   754,    86,
    1946,   279,   204,   279,   279,   761,   279,  1953,   279,   279,
     766,   767,   279,   279,   277,   279,    79,   279,   278,   246,
    1809,   278,    82,    82,   780,   278,   782,   783,   784,   785,
     786,   787,   788,   122,   279,   277,   173,  1826,  1827,   278,
      47,    37,   122,   279,   122,  1881,  1828,   279,   180,    44,
     279,   143,    48,  1889,    36,    47,    50,   201,   150,    55,
       0,   153,     0,   155,   156,   554,   822,   157,   305,  1905,
    1309,   157,    68,   814,   158,   308,   805,  1191,   951,   556,
     956,  1188,   714,  1180,   926,  1116,    79,   843,   180,  1620,
     660,   396,  1881,   846,  1873,   427,  1932,  1886,   357,   855,
    1889,   857,   879,  1185,   408,  1619,  1601,  1943,     6,  1181,
    1946,     9,  1901,  1902,  1892,    13,  1905,  1953,  1599,  1896,
    1895,  1917,    20,  1707,   869,  1057,  1882,     0,  1583,   125,
     794,   929,  1341,   401,    32,   697,   408,   229,   394,   640,
      38,    39,  1394,  1932,  1933,   141,   921,    13,   341,   572,
     576,   835,   244,   245,  1943,   413,  1945,  1946,   163,   822,
    1074,   157,    89,  1327,  1953,  1846,   747,  1671,    66,  1466,
     855,    69,     0,     0,     0,   267,   172,   584,  1069,  1350,
     176,   812,  1232,   179,  1170,  1279,  1752,   913,   638,    93,
    1509,   187,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
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
      -1,    -1,    -1,    -1,   272,   273,    -1,   181,    -1,    -1,
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
      -1,    -1,    21,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,  1431,    -1,   284,    37,    -1,
      -1,  1437,    -1,    -1,    -1,    -1,  1442,    -1,    -1,    48,
      -1,    -1,    -1,  1449,    -1,    48,    55,    -1,    -1,    -1,
      59,    60,    55,    -1,    -1,    -1,    59,    60,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    79,    -1,  1484,  1485,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,   273,   274,
     275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1506,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    32,    -1,    -1,
      -1,    -1,   141,    38,    39,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,   172,    69,    -1,    -1,   176,    -1,   172,
      -1,    -1,  1578,   176,    -1,    -1,    -1,    -1,   187,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,   206,   207,    -1,
      -1,   106,    -1,   206,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1651,    27,    -1,    -1,    -1,
      -1,  1657,  1658,    -1,    -1,    -1,   265,    -1,   163,   268,
     165,    -1,   265,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1681,   181,    -1,    -1,    -1,
    1686,    -1,    -1,  1689,    -1,    -1,    -1,  1693,  1694,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,     6,    -1,    -1,     9,   260,   261,   262,    13,   264,
     265,   266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,  1782,    32,    -1,   284,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
    1846,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,  1883,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    30,
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
     265,   266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,
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
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
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
     265,   266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,
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
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
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
     265,   266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,
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
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,
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
     265,   266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,   155,   280,    -1,    32,    -1,   284,
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
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,
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
     265,   266,   164,    -1,    -1,    -1,    -1,   272,   273,    -1,
      -1,    57,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    94,    -1,
      96,    97,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,   227,   228,    -1,   230,    -1,
      -1,    -1,   118,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,   155,
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
     266,    -1,    -1,    -1,    20,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    32,    -1,   284,    35,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,   118,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     266,    -1,    -1,    -1,    -1,    -1,   272,   273,    -1,    -1,
      57,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   272,   273,    -1,    -1,    57,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
     118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   272,   273,    -1,    -1,    57,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    94,    -1,    96,    97,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   227,   228,    -1,   230,    -1,    -1,   118,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,     6,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,   278,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    80,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,   272,   273,    -1,    -1,    -1,   145,   278,
      -1,   280,    -1,    -1,    -1,   284,    66,    -1,   155,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,   165,    -1,   254,   255,    -1,
      -1,   171,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,   272,   273,   274,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,
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
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    66,    -1,    -1,    69,   196,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,    -1,    -1,   273,    -1,    -1,    -1,    -1,
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
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
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
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,   272,
     273,    -1,    -1,    -1,    -1,   278,   279,   280,    -1,    -1,
      -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
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
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,   272,   273,
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
     265,   266,    -1,    -1,    -1,    -1,    -1,   272,   273,    -1,
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
     266,    -1,    -1,    -1,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,   181,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    58,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      74,   208,   209,    -1,    -1,    -1,    80,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      16,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,   137,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    44,    -1,
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
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     266,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
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
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
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
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    18,    -1,    -1,
      21,    -1,    -1,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    34,    -1,   227,   228,    38,   230,    -1,
      41,    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    18,   254,   255,    21,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    71,    -1,    -1,    -1,
      81,    38,    -1,    -1,    41,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    -1,    -1,   144,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,   183,    -1,    -1,    -1,   181,   144,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   161,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,   218,   219,    -1,
      -1,   216,   217,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,   227,   228,   235,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   218,   219,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,    -1,   198,    -1,   235,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,    -1,    -1,   225,   226,   227,   228,   229,   230,    -1,
      -1,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268
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
{ //lex.log_defined = false;
			  //lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
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
case 470:
{ yyval = NULL; }
    break;
case 471:
{ yyval = NULL; }
    break;
case 472:
{ yyval = NULL; }
    break;
case 473:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 474:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 475:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 476:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 477:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 478:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 479:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 480:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 481:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 482:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 483:
{ //lex.log_defined = false;
			  //lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 485:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 486:
{ yyval = yyvsp[0]; }
    break;
case 487:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 488:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 489:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 490:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 491:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 493:
{ yyval = NULL; }
    break;
case 495:
{ yyval = NULL; }
    break;
case 496:
{ yyval = yyvsp[0]; }
    break;
case 497:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 501:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 502:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 503:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 505:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 506:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 507:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 508:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 509:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 514:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 515:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 517:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 518:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 519:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 525:
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
case 526:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 528:
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
case 529:
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
case 530:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 537:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 538:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 539:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 540:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 541:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 542:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 558:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 559:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 560:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 561:
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
case 562:
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
case 565:
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
case 566:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 567:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 568:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 569:
{ yyval = yyvsp[-1]; }
    break;
case 570:
{ yyval = 0; }
    break;
case 574:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 575:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 576:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 580:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 581:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 582:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 583:
{ yyval = 0; }
    break;
case 584:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 587:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 588:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 591:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 592:
{ yyval = NULL; }
    break;
case 594:
{ yyval = NULL; }
    break;
case 595:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 597:
{ yyval = NULL; }
    break;
case 599:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 604:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 605:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 606:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 607:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 608:
{ yyval = yyvsp[0];}
    break;
case 610:
{ yyval = yyvsp[0];}
    break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 613:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 614:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 615:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 616:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 617:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 618:
{ yyval = 0; }
    break;
case 619:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 620:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 621:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 622:
{ yyval = (dsql_nod*) 0; }
    break;
case 623:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 624:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 626:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 627:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 628:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 629:
{ yyval = 0; }
    break;
case 631:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 632:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 633:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;
case 634:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 635:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;
case 636:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;
case 637:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;
case 638:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 639:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;
case 640:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 641:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;
case 642:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;
case 643:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;
case 644:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;
case 645:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 646:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 647:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;
case 648:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;
case 649:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;
case 650:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;
case 651:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 652:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 653:
{ yyval = yyvsp[0]; }
    break;
case 655:
{ yyval = NULL; }
    break;
case 656:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 657:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 658:
{ yyval = NULL; }
    break;
case 659:
{ yyval = yyvsp[0]; }
    break;
case 660:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 661:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 662:
{ yyval = NULL; }
    break;
case 663:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 664:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 665:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 667:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 668:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 670:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 671:
{ lex.limit_clause = true; }
    break;
case 672:
{ lex.limit_clause = false; }
    break;
case 673:
{ lex.first_detection = true; }
    break;
case 674:
{ lex.first_detection = false; }
    break;
case 675:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 676:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 677:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 678:
{ yyval = 0; }
    break;
case 679:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 680:
{ yyval = yyvsp[-2]; }
    break;
case 681:
{ yyval = yyvsp[-1]; }
    break;
case 682:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 683:
{ yyval = yyvsp[-1]; }
    break;
case 684:
{ yyval = yyvsp[0]; }
    break;
case 685:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 686:
{ yyval = 0; }
    break;
case 687:
{ yyval = make_list (yyvsp[0]); }
    break;
case 688:
{ yyval = 0; }
    break;
case 690:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 692:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 695:
{ yyval = make_list (yyvsp[0]); }
    break;
case 697:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 702:
{ yyval = yyvsp[-1]; }
    break;
case 703:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 705:
{ yyval = NULL; }
    break;
case 706:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 707:
{ yyval = NULL; }
    break;
case 709:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 713:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 714:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 715:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 718:
{ yyval = yyvsp[0]; }
    break;
case 719:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 720:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 721:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 722:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 723:
{ yyval = NULL; }
    break;
case 725:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 726:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 727:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 728:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 729:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 730:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 731:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 734:
{ yyval = make_list (yyvsp[0]); }
    break;
case 735:
{ yyval = NULL; }
    break;
case 737:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 739:
{ yyval = yyvsp[0]; }
    break;
case 740:
{ yyval = NULL; }
    break;
case 741:
{ yyval = yyvsp[0]; }
    break;
case 742:
{ yyval = NULL; }
    break;
case 743:
{ yyval = yyvsp[0]; }
    break;
case 744:
{ yyval = NULL; }
    break;
case 745:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 746:
{ yyval = 0; }
    break;
case 747:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 748:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 749:
{ yyval = 0; }
    break;
case 750:
{ yyval = 0; }
    break;
case 752:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 753:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 757:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 758:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 759:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 762:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 763:
{ yyval = 0; }
    break;
case 764:
{ yyval = make_list (yyvsp[0]); }
    break;
case 765:
{ yyval = 0; }
    break;
case 767:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 768:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 769:
{ yyval = 0; }
    break;
case 770:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 771:
{ yyval = 0; }
    break;
case 772:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 773:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 774:
{ yyval = yyvsp[-1]; }
    break;
case 775:
{ yyval = 0; }
    break;
case 776:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 778:
{ yyval = NULL; }
    break;
case 779:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 780:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 783:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 784:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 788:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 789:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 791:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 793:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 794:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 798:
{ yyval = NULL; }
    break;
case 801:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 802:
{ yyval = yyvsp[0]; }
    break;
case 803:
{ yyval = NULL; }
    break;
case 804:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 807:
{ yyval = NULL; }
    break;
case 808:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 810:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 812:
{ yyval = NULL; }
    break;
case 813:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 817:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 819:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 825:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 834:
{ yyval = yyvsp[-1]; }
    break;
case 835:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 848:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 850:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 854:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 856:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 857:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 858:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 859:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 860:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 861:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 862:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 863:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 864:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 865:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 866:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 867:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 868:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 869:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 870:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 873:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 874:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 875:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 876:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 877:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 878:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 879:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 880:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 881:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 882:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 883:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 884:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 885:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 886:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 887:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 888:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 889:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 890:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 891:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 892:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 893:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 894:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 895:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 896:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 897:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 898:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 900:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 901:
{ yyval = yyvsp[-1]; }
    break;
case 912:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 913:
{ yyval = yyvsp[0]; }
    break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 915:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 916:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 917:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 918:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 919:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 920:
{ yyval = yyvsp[-1]; }
    break;
case 921:
{ yyval = yyvsp[-1]; }
    break;
case 925:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 926:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 927:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 930:
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
case 931:
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
case 932:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 933:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 937:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 938:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 939:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 940:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 941:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 942:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 944:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
    break;
case 947:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 948:
{ yyval = make_parameter (); }
    break;
case 949:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 950:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 951:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 952:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 953:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 954:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 955:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 956:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 957:
{ yyval = yyvsp[0]; }
    break;
case 958:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 960:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 961:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 962:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 963:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 964:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 966:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 967:
{ yyval = yyvsp[0];}
    break;
case 971:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 972:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 973:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 974:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 975:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 976:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 977:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 978:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 979:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 980:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 981:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 983:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 985:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 986:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 987:
{ yyval = yyvsp[0]; }
    break;
case 988:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 989:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 990:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 991:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 994:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 995:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 996:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 999:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1000:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1001:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1002:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1003:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1004:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1005:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1006:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1010:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 1011:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1012:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1013:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1014:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1015:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1016:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1018:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1019:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1024:
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

