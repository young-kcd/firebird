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



#define	YYFINAL		1969
#define	YYFLAG		-32768
#define	YYNTBASE	285

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 522 ? yytranslate[x] : 735)

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
    1286,  1289,  1292,  1296,  1300,  1303,  1306,  1310,  1314,  1321,
    1323,  1326,  1331,  1335,  1338,  1341,  1344,  1347,  1351,  1353,
    1357,  1361,  1365,  1368,  1371,  1376,  1381,  1387,  1389,  1391,
    1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,
    1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,  1431,
    1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,
    1453,  1455,  1458,  1461,  1464,  1466,  1468,  1470,  1471,  1474,
    1477,  1482,  1487,  1493,  1497,  1500,  1501,  1504,  1505,  1506,
    1508,  1511,  1514,  1519,  1523,  1526,  1529,  1537,  1539,  1540,
    1542,  1543,  1546,  1549,  1552,  1555,  1558,  1561,  1564,  1567,
    1570,  1574,  1577,  1580,  1583,  1586,  1588,  1590,  1592,  1594,
    1599,  1605,  1607,  1611,  1613,  1617,  1619,  1622,  1624,  1626,
    1628,  1630,  1632,  1634,  1636,  1638,  1640,  1642,  1644,  1649,
    1654,  1661,  1667,  1671,  1672,  1675,  1678,  1679,  1683,  1684,
    1689,  1691,  1697,  1702,  1704,  1709,  1711,  1714,  1717,  1719,
    1721,  1723,  1726,  1729,  1732,  1735,  1736,  1740,  1746,  1748,
    1750,  1753,  1757,  1759,  1762,  1766,  1767,  1769,  1771,  1773,
    1779,  1785,  1792,  1794,  1796,  1798,  1801,  1806,  1808,  1809,
    1815,  1817,  1818,  1822,  1825,  1827,  1828,  1831,  1832,  1834,
    1835,  1839,  1841,  1842,  1844,  1847,  1849,  1851,  1853,  1855,
    1858,  1861,  1863,  1866,  1870,  1872,  1874,  1878,  1882,  1884,
    1887,  1891,  1893,  1896,  1897,  1900,  1902,  1904,  1905,  1907,
    1909,  1911,  1915,  1918,  1922,  1923,  1925,  1929,  1934,  1940,
    1947,  1955,  1957,  1959,  1961,  1963,  1965,  1967,  1970,  1972,
    1974,  1976,  1978,  1980,  1982,  1985,  1987,  1989,  1991,  1994,
    1996,  1998,  2002,  2006,  2007,  2010,  2011,  2014,  2015,  2019,
    2023,  2027,  2029,  2034,  2039,  2041,  2051,  2052,  2053,  2054,
    2055,  2059,  2062,  2064,  2065,  2069,  2075,  2079,  2082,  2088,
    2091,  2093,  2095,  2097,  2099,  2101,  2105,  2107,  2111,  2113,
    2114,  2117,  2119,  2123,  2125,  2127,  2129,  2131,  2135,  2142,
    2144,  2145,  2149,  2150,  2152,  2156,  2158,  2160,  2162,  2167,
    2173,  2179,  2181,  2183,  2186,  2191,  2196,  2199,  2203,  2204,
    2206,  2209,  2211,  2213,  2216,  2219,  2222,  2223,  2225,  2226,
    2230,  2231,  2233,  2237,  2239,  2242,  2243,  2246,  2247,  2250,
    2251,  2256,  2258,  2261,  2263,  2265,  2266,  2268,  2272,  2275,
    2277,  2279,  2282,  2284,  2289,  2293,  2295,  2299,  2304,  2305,
    2309,  2310,  2312,  2316,  2320,  2322,  2324,  2325,  2327,  2329,
    2334,  2335,  2338,  2343,  2344,  2353,  2359,  2361,  2363,  2371,
    2376,  2378,  2380,  2389,  2395,  2400,  2402,  2406,  2410,  2412,
    2420,  2428,  2434,  2438,  2439,  2441,  2443,  2445,  2448,  2449,
    2451,  2453,  2455,  2456,  2460,  2462,  2466,  2468,  2469,  2473,
    2475,  2479,  2481,  2485,  2489,  2491,  2493,  2497,  2499,  2502,
    2504,  2508,  2512,  2514,  2517,  2521,  2525,  2529,  2533,  2535,
    2539,  2542,  2544,  2546,  2548,  2550,  2552,  2554,  2556,  2558,
    2560,  2562,  2564,  2568,  2572,  2576,  2580,  2584,  2588,  2592,
    2596,  2603,  2610,  2617,  2624,  2631,  2638,  2645,  2652,  2659,
    2666,  2673,  2680,  2687,  2694,  2701,  2708,  2710,  2712,  2718,
    2725,  2731,  2738,  2742,  2747,  2753,  2760,  2764,  2769,  2773,
    2778,  2782,  2787,  2792,  2798,  2803,  2808,  2812,  2817,  2819,
    2821,  2823,  2825,  2827,  2829,  2831,  2835,  2839,  2841,  2843,
    2845,  2847,  2849,  2851,  2853,  2855,  2857,  2859,  2862,  2865,
    2869,  2873,  2877,  2881,  2885,  2889,  2893,  2897,  2899,  2901,
    2903,  2905,  2909,  2911,  2913,  2915,  2917,  2919,  2921,  2926,
    2928,  2932,  2934,  2937,  2939,  2941,  2943,  2945,  2947,  2949,
    2951,  2954,  2957,  2960,  2962,  2964,  2966,  2968,  2970,  2972,
    2974,  2976,  2978,  2980,  2983,  2985,  2988,  2990,  2992,  2994,
    2996,  2998,  3001,  3003,  3005,  3007,  3009,  3014,  3020,  3026,
    3032,  3038,  3044,  3050,  3056,  3062,  3068,  3074,  3076,  3083,
    3085,  3090,  3098,  3101,  3102,  3107,  3111,  3118,  3120,  3122,
    3129,  3138,  3145,  3147,  3149,  3154,  3161,  3166,  3172,  3176,
    3182,  3187,  3193,  3195,  3197,  3199,  3204,  3211,  3213,  3215,
    3217,  3219,  3221,  3223,  3225,  3227,  3229,  3230,  3232,  3233,
    3235,  3237,  3239,  3241,  3243,  3245,  3247,  3249,  3251,  3253,
    3255,  3257,  3259,  3261,  3263,  3265,  3267,  3269,  3271,  3273,
    3275,  3277,  3279,  3281,  3283,  3285,  3287,  3289,  3291,  3293,
    3295,  3297,  3299,  3301,  3303,  3305,  3307,  3309,  3311,  3313,
    3315,  3317,  3319,  3321,  3323,  3325,  3327,  3329,  3331,  3333,
    3335,  3337,  3339,  3341,  3343,  3345,  3347
};
static const short yyrhs[] =
{
     286,     0,   286,   276,     0,   469,     0,   630,     0,   549,
       0,   526,     0,   321,     0,   308,     0,   620,     0,   494,
       0,   287,     0,   619,     0,   447,     0,   450,     0,   323,
       0,   325,     0,   296,     0,   527,     0,   520,     0,   555,
       0,   518,     0,   623,     0,    40,   677,     0,    70,   289,
     122,   288,   595,   174,   299,   293,     0,    70,   291,   122,
     141,   295,   174,   299,   293,     0,    70,   304,   174,   306,
     294,     0,   172,     0,     0,     6,     0,     6,   140,     0,
     290,     0,   292,     0,   290,   277,   292,     0,    56,     0,
     155,     0,    80,     0,    44,     0,   180,   636,     0,   145,
     636,     0,   192,    70,   124,     0,     0,   192,   202,   124,
       0,     0,   724,     0,   150,   297,   289,   122,   288,   595,
      63,   299,     0,   150,   297,   291,   122,   141,   295,    63,
     299,     0,   150,   298,   304,    63,   306,     0,    70,   124,
      61,     0,     0,   202,   124,    61,     0,     0,   300,     0,
     302,     0,   301,     0,   300,   277,   301,     0,   300,   277,
     303,     0,   302,   277,   301,     0,   141,   724,     0,   176,
     728,     0,   187,   731,     0,   206,   725,     0,   303,     0,
     302,   277,   303,     0,   729,     0,   181,   729,     0,    71,
     729,     0,   305,     0,   304,   277,   305,     0,   725,     0,
     307,     0,   306,   277,   307,     0,   729,     0,   181,   729,
       0,    42,   309,     0,    60,   319,     0,    59,    65,   310,
       0,   708,   312,   149,   316,    52,   676,   111,   676,     0,
     501,     0,    18,     0,    34,   278,   680,   279,   507,     0,
       0,   313,     0,   278,   313,   279,     0,   314,     0,   313,
     277,   314,     0,   379,   311,   315,     0,     0,    19,   230,
       0,    19,   262,     0,    94,     0,   317,     0,   278,   317,
     279,     0,   379,   311,   318,     0,   133,   680,     0,     0,
      19,    97,     0,    19,   230,     0,   204,     0,    19,   230,
     204,     0,   717,    78,   320,   128,   320,    52,   676,   111,
     676,     0,   709,     0,   677,     0,    33,   322,     0,    55,
     327,     0,   331,   615,    79,   720,   122,   595,   332,     0,
     141,   398,     0,   172,   366,     0,   176,   461,     0,   187,
     454,     0,    68,   348,     0,   265,   348,     0,    37,   350,
       0,    48,   339,     0,   157,   333,     0,   206,   349,     0,
     229,   324,     0,   141,   399,     0,   172,   367,     0,   187,
     455,     0,    55,   328,     0,    33,   125,     7,   326,     0,
     141,   400,     0,   176,   462,     0,    55,   329,     0,   716,
     676,     0,   716,   676,     0,   716,   676,     0,   716,   676,
       0,   179,     0,     0,   638,     0,   637,     0,   373,   278,
     458,   665,   459,   279,     0,   680,   334,   335,   676,   336,
     337,     0,   105,     0,    14,     0,     0,     0,    29,     0,
       0,    98,   351,   683,   365,     0,     0,   338,     0,   359,
       0,   338,   359,     0,   376,   340,   496,   458,   341,   459,
     342,   375,     0,    10,     0,     0,    43,   458,   381,     0,
       0,     0,   343,     0,   344,     0,   343,   344,     0,   345,
       0,   346,     0,   347,     0,   118,    94,     0,   458,    22,
     278,   645,   279,   459,     0,   719,     0,   725,     0,   352,
     353,   356,     0,     0,   269,     0,   676,     0,     0,   354,
       0,   355,     0,   354,   355,     0,   132,   351,   680,     0,
      98,   351,   683,   365,     0,   181,   676,     0,   134,   676,
       0,   156,   112,   676,     0,     0,   357,     0,   358,     0,
     357,   358,     0,   359,     0,    43,    21,   156,   710,     0,
     255,    90,   676,     0,   360,   676,   361,     0,    90,     0,
       0,   362,     0,   363,     0,   362,   363,     0,   167,   364,
     683,     0,    98,   351,   683,   365,     0,     0,    12,     0,
      12,   130,     0,     0,   130,     0,   131,     0,   595,   368,
     278,   369,   279,     0,   595,   368,   278,   369,   279,     0,
      59,    90,   676,     0,    59,   676,     0,     0,   370,     0,
     369,   277,   370,     0,   371,     0,   386,     0,   376,   374,
     341,   459,   382,   375,     0,   376,   497,   372,     0,   376,
     372,     0,   373,   278,   458,   665,   459,   279,     0,    27,
      19,     0,    27,     0,   496,   458,     0,   643,   456,     0,
      23,   711,     0,     0,   643,     0,   643,     0,   379,   496,
       0,     0,    43,   381,     0,     0,   669,     0,   673,     0,
     674,     0,   675,     0,   707,     0,   666,     0,     0,   383,
       0,   384,     0,   383,   384,     0,   387,   385,     0,   118,
      94,     0,   145,   595,   636,   394,   392,     0,   393,     0,
     179,   392,     0,   139,    86,   392,     0,   387,   388,     0,
      30,   713,     0,     0,   389,     0,   390,     0,   391,     0,
     393,     0,   179,   637,   392,     0,   139,    86,   637,   392,
       0,    62,    86,   637,   145,   595,   636,   394,   392,     0,
     239,   615,    79,   720,     0,     0,   458,    22,   278,   645,
     279,   459,     0,   395,     0,   396,     0,   396,   395,     0,
     395,   396,     0,     0,   122,   180,   397,     0,   122,    44,
     397,     0,   203,     0,   156,    43,     0,   156,    94,     0,
     117,   201,     0,   724,   402,   403,    10,   456,   409,   418,
     459,     0,   724,   402,   403,    10,   456,   409,   418,   459,
       0,   724,   402,   403,    10,   456,   409,   418,   459,     0,
     724,   402,   403,    10,   456,   409,   418,   459,     0,   278,
     404,   279,     0,     0,   149,   278,   406,   279,     0,     0,
     405,     0,   404,   277,   405,     0,   377,   497,   408,   459,
       0,   407,     0,   406,   277,   407,     0,   377,   497,     0,
      43,   456,   381,     0,   269,   456,   381,     0,   456,     0,
     410,     0,     0,   411,     0,   410,   411,     0,    42,   414,
     412,   276,     0,   413,     0,   416,     0,   376,   497,   415,
       0,   184,     0,     0,   269,   381,     0,   380,     0,   714,
      36,    61,   278,   555,   279,     0,   421,     0,   418,     0,
      16,   419,    51,     0,   420,     0,   420,   438,     0,     0,
     417,     0,   420,   417,     0,   628,   276,     0,   619,   276,
       0,   623,   276,     0,   620,   276,     0,   431,   276,     0,
     447,   276,     0,   424,   276,     0,   427,   276,     0,   629,
     276,     0,   422,   276,     0,   423,   276,     0,   429,   276,
       0,   442,   276,     0,   436,   276,     0,   170,   276,     0,
      58,   276,     0,   428,     0,   434,     0,   425,     0,   426,
       0,    55,   716,     0,    55,   716,   665,     0,    55,     0,
      56,   246,   665,     0,   435,    61,   555,    82,   433,   437,
      47,   417,     0,   435,    61,    56,   246,   665,    82,   433,
      47,   417,     0,    56,   246,   665,    82,   433,     0,    74,
     278,   645,   279,   173,   417,    50,   417,     0,    74,   278,
     645,   279,   173,   417,     0,   137,   665,   430,     0,     0,
     555,    82,   433,     0,   280,   730,     0,   432,     0,   642,
       0,   433,   277,   642,     0,   433,   277,   432,     0,   435,
     191,   278,   645,   279,    47,   417,     0,   722,   280,     0,
       0,   227,     0,   247,     0,   247,   722,     0,    10,    36,
     714,     0,     0,   439,     0,   438,   439,     0,   189,   440,
      47,   417,     0,   441,     0,   440,   277,   441,     0,   165,
     677,     0,    66,   718,     0,    55,   716,     0,     9,     0,
     443,     0,   445,     0,   444,     0,   256,   714,     0,   257,
     714,     0,   258,   446,   714,    82,   433,     0,     0,    56,
     141,   724,   448,   449,     0,   668,     0,   278,   668,   279,
       0,     0,   148,   433,     0,   148,   278,   433,   279,     0,
       0,    56,   260,   451,   403,    10,   409,   418,     0,   278,
     452,   279,     0,     0,   453,     0,   452,   277,   453,     0,
     407,   269,   672,     0,   731,   636,    10,   456,   559,   460,
     457,     0,   731,   636,    10,   456,   559,   460,   457,     0,
       0,     0,     0,     0,   192,    22,   124,     0,     0,   728,
      61,   595,   463,   464,   467,   458,   468,   459,     0,   728,
      61,   595,   463,   464,   467,   458,   468,   459,     0,     3,
       0,    76,     0,     0,   465,   466,     0,    15,     0,     5,
       0,    80,     0,   180,     0,    44,     0,    80,   125,   180,
       0,    80,   125,    44,     0,   180,   125,    80,     0,   180,
     125,    44,     0,    44,   125,    80,     0,    44,   125,   180,
       0,    80,   125,   180,   125,    44,     0,    80,   125,    44,
     125,   180,     0,   180,   125,    80,   125,    44,     0,   180,
     125,    44,   125,    80,     0,    44,   125,    80,   125,   180,
       0,    44,   125,   180,   125,    80,     0,   136,   678,     0,
       0,    10,   458,   409,   418,     0,     7,   470,     0,    55,
     330,     0,   172,   595,   475,     0,   176,   491,     0,   141,
     401,     0,    37,   488,   489,     0,    48,   477,   473,     0,
      79,   483,     0,   265,   484,     0,    59,    65,   485,     0,
      43,   458,   381,     0,    22,   458,   278,   645,   279,   459,
       0,   474,     0,   473,   474,     0,   156,   458,   471,   459,
       0,     4,    30,   472,     0,     4,   347,     0,    49,    43,
       0,    49,    30,     0,   174,   643,     0,   208,   379,   497,
       0,   476,     0,   475,   277,   476,     0,    49,   643,   482,
       0,    49,    30,   713,     0,     4,   371,     0,     4,   386,
       0,   479,   643,   136,   680,     0,   479,   477,   174,   643,
       0,   479,   481,   208,   480,   459,     0,   478,     0,   733,
       0,   202,     0,   207,     0,   209,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   218,
       0,   219,     0,   220,     0,   221,     0,   222,     0,   225,
       0,   226,     0,   229,     0,   233,     0,   234,     0,   235,
       0,   236,     0,   245,     0,   242,     0,   244,     0,   256,
       0,   257,     0,   258,     0,   259,     0,   239,     0,   263,
       0,   267,     0,     7,     0,     7,   207,     0,   497,   458,
       0,   643,   456,     0,   643,     0,   205,     0,   203,     0,
       0,   720,     3,     0,   720,    76,     0,   719,   266,   192,
     682,     0,   719,   266,   192,   223,     0,   719,   266,   192,
     273,   223,     0,   708,   486,   487,     0,    52,   676,     0,
       0,   111,   676,     0,     0,     0,   490,     0,   489,   490,
       0,     4,   338,     0,     4,   255,    90,   676,     0,    49,
     255,    90,     0,    16,   254,     0,    51,   254,     0,   728,
     463,   492,   467,   458,   493,   459,     0,   464,     0,     0,
     468,     0,     0,    49,   495,     0,    55,   716,     0,    79,
     720,     0,   141,   724,     0,   172,   727,     0,   176,   728,
       0,   187,   731,     0,    60,   717,     0,    48,   715,     0,
      59,    65,   708,     0,   157,   680,     0,   206,   725,     0,
      68,   719,     0,   265,   719,     0,   497,     0,   498,     0,
     501,     0,   504,     0,   502,   281,   499,   282,     0,   509,
     281,   499,   282,   507,     0,   500,     0,   499,   277,   500,
       0,   682,     0,   682,   280,   682,     0,   502,     0,   509,
     507,     0,   508,     0,   513,     0,   516,     0,   235,     0,
     503,     0,   161,     0,    38,     0,   218,     0,   219,     0,
      81,     0,    92,     0,    18,   506,   505,   507,     0,    18,
     278,   681,   279,     0,    18,   278,   681,   277,   677,   279,
       0,    18,   278,   277,   677,   279,     0,   154,   160,   681,
       0,     0,   169,   677,     0,   169,   709,     0,     0,    21,
     156,   710,     0,     0,   512,   278,   680,   279,     0,   512,
       0,   512,   185,   278,   680,   279,     0,   511,   278,   680,
     279,     0,   511,     0,   510,   278,   680,   279,     0,   183,
       0,    21,   185,     0,    87,   185,     0,    21,     0,    87,
       0,   115,     0,   113,    21,     0,   113,    87,     0,    95,
     514,     0,   515,   514,     0,     0,   278,   682,   279,     0,
     278,   682,   277,   682,   279,     0,    41,     0,    88,     0,
      91,   517,     0,    93,    91,   517,     0,   144,     0,    89,
     138,     0,   278,   678,   279,     0,     0,   531,     0,   519,
       0,   548,     0,   156,    68,   719,   174,   682,     0,   156,
      68,   719,   174,   223,     0,   156,    68,   719,   174,   273,
     223,     0,   521,     0,   522,     0,   524,     0,   244,   732,
       0,   245,   244,   732,   523,     0,   123,     0,     0,   153,
     528,   174,   525,   732,     0,   244,     0,     0,    25,   528,
     529,     0,   153,   528,     0,   193,     0,     0,   147,   530,
       0,     0,   162,     0,     0,   156,   175,   532,     0,   533,
       0,     0,   534,     0,   533,   534,     0,   535,     0,   536,
       0,   537,     0,   541,     0,   143,   123,     0,   143,   194,
       0,   188,     0,   117,   188,     0,    84,   102,   538,     0,
     538,     0,   539,     0,   143,   177,   540,     0,   143,    26,
     540,     0,   162,     0,   162,   172,     0,   162,   172,   166,
       0,   186,     0,   117,   186,     0,     0,   146,   544,     0,
     158,     0,   142,     0,     0,   143,     0,   194,     0,   545,
       0,   544,   277,   545,     0,   547,   546,     0,    61,   542,
     543,     0,     0,   595,     0,   547,   277,   595,     0,   156,
     168,    79,   720,     0,   267,   122,   550,    83,   554,     0,
     267,   122,   551,   723,    83,   554,     0,   267,   122,   552,
     723,   553,    83,   554,     0,    37,     0,    48,     0,   172,
       0,   187,     0,   141,     0,   176,     0,    59,    65,     0,
      60,     0,    55,     0,    68,     0,   265,     0,    79,     0,
     206,     0,    21,   156,     0,   268,     0,   207,     0,   133,
       0,   283,   723,     0,   676,     0,    94,     0,   559,   556,
     558,     0,    61,   180,   557,     0,     0,   121,   638,     0,
       0,   192,   243,     0,     0,   562,   612,   618,     0,   562,
     612,   618,     0,   562,   612,   618,     0,   563,     0,   562,
     178,   706,   563,     0,   562,   178,     6,   563,     0,   564,
       0,   155,   569,   572,   573,   577,   602,   598,   601,   603,
       0,     0,     0,     0,     0,   570,   571,   566,     0,   570,
     566,     0,   571,     0,     0,   231,   683,   565,     0,   231,
     278,   665,   279,   565,     0,   231,   672,   565,     0,   232,
     683,     0,   232,   278,   566,   665,   279,     0,   232,   672,
       0,    46,     0,   705,     0,   574,     0,   274,     0,   575,
       0,   574,   277,   575,     0,   665,     0,   665,   576,   721,
       0,    10,     0,     0,    63,   578,     0,   579,     0,   578,
     277,   579,     0,   585,     0,   580,     0,   592,     0,   581,
       0,   278,   585,   279,     0,   278,   559,   279,   576,   582,
     583,     0,   726,     0,     0,   278,   584,   279,     0,     0,
     721,     0,   584,   277,   721,     0,   586,     0,   587,     0,
     588,     0,   579,   263,    85,   580,     0,   579,   114,   596,
      85,   580,     0,   579,   596,    85,   579,   589,     0,   590,
       0,   591,     0,   122,   645,     0,   239,   278,   638,   279,
       0,   724,   593,   576,   726,     0,   724,   593,     0,   278,
     668,   279,     0,     0,   595,     0,   727,   726,     0,   727,
       0,    77,     0,   100,   597,     0,   151,   597,     0,    64,
     597,     0,     0,   127,     0,     0,    71,    19,   599,     0,
       0,   600,     0,   599,   277,   600,     0,   665,     0,    73,
     645,     0,     0,   190,   645,     0,     0,   135,   604,     0,
       0,   605,   278,   606,   279,     0,    85,     0,   164,   108,
       0,   108,     0,   164,     0,     0,   607,     0,   607,   277,
     606,     0,   608,   609,     0,   604,     0,   727,     0,   727,
     608,     0,   114,     0,    79,   278,   610,   279,     0,   126,
     720,   611,     0,   720,     0,   720,   277,   610,     0,    79,
     278,   610,   279,     0,     0,   126,    19,   613,     0,     0,
     614,     0,   613,   277,   614,     0,   665,   615,   617,     0,
      11,     0,    45,     0,     0,   231,     0,   241,     0,   240,
     567,   616,   568,     0,     0,   259,   665,     0,   259,   665,
     174,   665,     0,     0,    80,    82,   595,   639,   182,   278,
     668,   279,     0,    80,    82,   595,   639,   559,     0,   621,
       0,   622,     0,    44,    63,   594,   602,   603,   612,   618,
       0,    44,    63,   594,   626,     0,   624,     0,   625,     0,
     180,   594,   156,   627,   602,   603,   612,   618,     0,   180,
     594,   156,   627,   626,     0,   190,    35,   121,   714,     0,
     628,     0,   627,   277,   628,     0,   644,   269,   665,     0,
     691,     0,   143,    18,   643,    63,   595,   631,   634,     0,
      80,    18,   643,    82,   595,   631,   634,     0,    60,    63,
     632,   174,   632,     0,    60,   174,   632,     0,     0,   633,
       0,   672,     0,   677,     0,   107,   635,     0,     0,   681,
       0,   672,     0,   637,     0,     0,   278,   638,   279,     0,
     643,     0,   638,   277,   643,     0,   640,     0,     0,   278,
     641,   279,     0,   644,     0,   641,   277,   644,     0,   643,
       0,   726,   283,   712,     0,   726,   283,   274,     0,   712,
       0,   643,     0,   726,   283,   712,     0,   661,     0,   118,
     661,     0,   647,     0,   645,   125,   645,     0,   645,     8,
     645,     0,   647,     0,   118,   661,     0,   646,   125,   645,
       0,   646,     8,   645,     0,   662,   125,   645,     0,   662,
       8,   645,     0,   648,     0,   278,   646,   279,     0,   118,
     647,     0,   649,     0,   652,     0,   653,     0,   654,     0,
     655,     0,   660,     0,   650,     0,   658,     0,   656,     0,
     657,     0,   659,     0,   665,   269,   665,     0,   665,   270,
     665,     0,   665,   271,   665,     0,   665,    67,   665,     0,
     665,   101,   665,     0,   665,   119,   665,     0,   665,   120,
     665,     0,   665,   116,   665,     0,   665,   269,     6,   278,
     560,   279,     0,   665,   270,     6,   278,   560,   279,     0,
     665,   271,     6,   278,   560,   279,     0,   665,    67,     6,
     278,   560,   279,     0,   665,   101,     6,   278,   560,   279,
       0,   665,   119,     6,   278,   560,   279,     0,   665,   120,
       6,   278,   560,   279,     0,   665,   116,     6,   278,   560,
     279,     0,   665,   269,   651,   278,   560,   279,     0,   665,
     270,   651,   278,   560,   279,     0,   665,   271,   651,   278,
     560,   279,     0,   665,    67,   651,   278,   560,   279,     0,
     665,   101,   651,   278,   560,   279,     0,   665,   119,   651,
     278,   560,   279,     0,   665,   120,   651,   278,   560,   279,
       0,   665,   116,   651,   278,   560,   279,     0,   163,     0,
       9,     0,   665,    83,    46,    63,   665,     0,   665,    83,
     118,    46,    63,   665,     0,   665,    17,   665,     8,   665,
       0,   665,   118,    17,   665,     8,   665,     0,   665,   103,
     665,     0,   665,   118,   103,   665,     0,   665,   103,   665,
      54,   665,     0,   665,   118,   103,   665,    54,   665,     0,
     665,    75,   663,     0,   665,   118,    75,   663,     0,   665,
      31,   665,     0,   665,   118,    31,   665,     0,   665,   167,
     665,     0,   665,   118,   167,   665,     0,   665,   167,   192,
     665,     0,   665,   118,   167,   192,   665,     0,    57,   278,
     559,   279,     0,   159,   278,   559,   279,     0,   665,    83,
      94,     0,   665,    83,   118,    94,     0,   248,     0,   249,
       0,   250,     0,   251,     0,   252,     0,   253,     0,   664,
       0,   278,   668,   279,     0,   278,   560,   279,     0,   642,
       0,   667,     0,   684,     0,   671,     0,   672,     0,   432,
       0,   692,     0,   693,     0,   703,     0,   691,     0,   273,
     665,     0,   272,   665,     0,   665,   272,   665,     0,   665,
      28,   665,     0,   665,    23,   711,     0,   665,   273,   665,
       0,   665,   274,   665,     0,   665,   275,   665,     0,   278,
     665,   279,     0,   278,   561,   279,     0,   673,     0,   674,
       0,   675,     0,    39,     0,   726,   283,    39,     0,    97,
       0,   666,     0,   707,     0,   220,     0,   221,     0,   222,
       0,   642,   281,   668,   282,     0,   665,     0,   668,   277,
     665,     0,   671,     0,   273,   670,     0,   197,     0,   196,
       0,   195,     0,   223,     0,   224,     0,   670,     0,   676,
       0,    38,   199,     0,   218,   199,     0,   219,   199,     0,
     284,     0,   181,     0,   225,     0,   226,     0,   233,     0,
     234,     0,    66,     0,   165,     0,   242,     0,   199,     0,
     200,   199,     0,   678,     0,   273,   679,     0,   196,     0,
     196,     0,   678,     0,   196,     0,   683,     0,   273,   683,
       0,   196,     0,   685,     0,   686,     0,   688,     0,    32,
     278,   274,   279,     0,    32,   278,   705,   665,   279,     0,
      32,   278,    46,   665,   279,     0,   171,   278,   705,   665,
     279,     0,   171,   278,    46,   665,   279,     0,    13,   278,
     705,   665,   279,     0,    13,   278,    46,   665,   279,     0,
     110,   278,   705,   665,   279,     0,   110,   278,    46,   665,
     279,     0,   106,   278,   705,   665,   279,     0,   106,   278,
      46,   665,   279,     0,   687,     0,   209,   278,   704,    63,
     665,   279,     0,   689,     0,    96,   278,   665,   279,     0,
     228,   278,   665,    63,   665,   690,   279,     0,    61,   665,
       0,     0,   708,   278,   668,   279,     0,   708,   278,   279,
       0,    20,   278,   665,    10,   378,   279,     0,   694,     0,
     695,     0,   237,   278,   665,   277,   665,   279,     0,   261,
     278,   645,   277,   665,   277,   665,   279,     0,   238,   278,
     665,   277,   668,   279,     0,   696,     0,   698,     0,   236,
     701,   697,    51,     0,   236,   701,   697,    50,   702,    51,
       0,   189,   700,   173,   702,     0,   697,   189,   700,   173,
     702,     0,   236,   699,    51,     0,   236,   699,    50,   702,
      51,     0,   189,   645,   173,   702,     0,   699,   189,   645,
     173,   702,     0,   665,     0,   665,     0,   665,     0,   264,
      97,    61,   719,     0,    69,   278,   719,   277,   665,   279,
       0,   210,     0,   211,     0,   212,     0,   213,     0,   214,
       0,   215,     0,   216,     0,   217,     0,     6,     0,     0,
      46,     0,     0,    94,     0,   198,     0,   733,     0,   733,
       0,   733,     0,   733,     0,   733,     0,   733,     0,   733,
       0,   733,     0,   733,     0,   733,     0,   733,     0,   733,
       0,   733,     0,   733,     0,   733,     0,   733,     0,   733,
       0,   733,     0,   733,     0,   733,     0,   733,     0,   733,
       0,   733,     0,   733,     0,   198,     0,   734,     0,   201,
       0,   203,     0,   204,     0,   205,     0,   206,     0,   208,
       0,   227,     0,   230,     0,   228,     0,   238,     0,   241,
       0,   247,     0,   243,     0,   237,     0,   240,     0,   246,
       0,   248,     0,   249,     0,   250,     0,   260,     0,   254,
       0,   255,     0,   261,     0,   262,     0,   216,     0,   217,
       0,   265,     0,   264,     0,   266,     0,   268,     0
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
    1905,  1907,  1909,  1912,  1915,  1917,  1919,  1923,  1927,  1933,
    1934,  1938,  1947,  1951,  1953,  1955,  1957,  1959,  1963,  1964,
    1968,  1970,  1972,  1974,  1980,  1983,  1985,  1989,  1997,  1998,
    1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,  2008,
    2009,  2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,
    2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,
    2031,  2033,  2037,  2039,  2044,  2050,  2052,  2054,  2058,  2060,
    2064,  2067,  2070,  2075,  2079,  2081,  2085,  2087,  2094,  2098,
    2099,  2103,  2105,  2107,  2109,  2111,  2118,  2128,  2129,  2133,
    2134,  2140,  2144,  2146,  2148,  2150,  2152,  2154,  2156,  2158,
    2160,  2162,  2164,  2166,  2168,  2175,  2176,  2179,  2180,  2183,
    2188,  2195,  2196,  2200,  2209,  2215,  2216,  2219,  2220,  2221,
    2222,  2239,  2244,  2249,  2268,  2285,  2292,  2293,  2300,  2305,
    2312,  2319,  2328,  2332,  2338,  2342,  2346,  2352,  2356,  2363,
    2369,  2375,  2383,  2388,  2393,  2400,  2401,  2402,  2405,  2406,
    2409,  2410,  2411,  2418,  2422,  2433,  2439,  2493,  2555,  2556,
    2563,  2576,  2581,  2586,  2593,  2595,  2602,  2603,  2604,  2608,
    2611,  2614,  2622,  2623,  2624,  2627,  2631,  2635,  2637,  2641,
    2645,  2646,  2649,  2653,  2657,  2658,  2661,  2663,  2667,  2668,
    2672,  2676,  2677,  2681,  2682,  2686,  2687,  2688,  2689,  2692,
    2694,  2698,  2700,  2704,  2706,  2709,  2711,  2713,  2717,  2719,
    2721,  2725,  2727,  2729,  2733,  2737,  2739,  2741,  2745,  2747,
    2751,  2752,  2756,  2760,  2762,  2766,  2767,  2772,  2776,  2778,
    2780,  2784,  2788,  2790,  2792,  2794,  2796,  2798,  2800,  2802,
    2804,  2806,  2808,  2810,  2812,  2814,  2822,  2824,  2828,  2832,
    2833,  2840,  2844,  2846,  2850,  2852,  2856,  2858,  2865,  2869,
    2874,  2879,  2880,  2882,  2886,  2889,  2901,  2905,  2909,  2913,
    2917,  2919,  2921,  2923,  2927,  2929,  2931,  2935,  2937,  2939,
    2943,  2945,  2949,  2951,  2955,  2956,  2960,  2961,  2965,  2966,
    2971,  2975,  2976,  2980,  2981,  2984,  2985,  2986,  2991,  2996,
    2997,  3001,  3003,  3007,  3008,  3012,  3013,  3014,  3017,  3022,
    3027,  3031,  3032,  3035,  3039,  3043,  3046,  3051,  3053,  3057,
    3058,  3063,  3068,  3070,  3072,  3074,  3076,  3080,  3081,  3087,
    3089,  3093,  3094,  3100,  3103,  3105,  3109,  3111,  3118,  3120,
    3124,  3128,  3130,  3132,  3138,  3140,  3144,  3145,  3149,  3151,
    3154,  3155,  3159,  3161,  3163,  3167,  3168,  3172,  3174,  3180,
    3182,  3186,  3187,  3191,  3195,  3197,  3199,  3203,  3205,  3209,
    3211,  3217,  3220,  3228,  3235,  3238,  3245,  3246,  3249,  3253,
    3260,  3261,  3264,  3269,  3275,  3282,  3283,  3287,  3291,  3298,
    3300,  3304,  3306,  3308,  3312,  3313,  3316,  3320,  3322,  3326,
    3328,  3334,  3335,  3339,  3343,  3344,  3349,  3350,  3354,  3358,
    3359,  3364,  3365,  3368,  3373,  3378,  3381,  3388,  3389,  3391,
    3392,  3394,  3398,  3399,  3401,  3403,  3407,  3409,  3413,  3414,
    3416,  3420,  3421,  3422,  3423,  3424,  3425,  3426,  3427,  3428,
    3429,  3430,  3435,  3437,  3439,  3441,  3443,  3445,  3447,  3449,
    3455,  3457,  3459,  3461,  3463,  3465,  3467,  3469,  3471,  3473,
    3475,  3477,  3479,  3481,  3483,  3485,  3489,  3490,  3496,  3498,
    3502,  3504,  3509,  3511,  3513,  3515,  3520,  3522,  3526,  3528,
    3532,  3534,  3536,  3538,  3542,  3546,  3550,  3552,  3556,  3561,
    3566,  3573,  3578,  3583,  3592,  3593,  3597,  3604,  3605,  3606,
    3607,  3608,  3609,  3610,  3611,  3612,  3613,  3614,  3616,  3618,
    3625,  3627,  3629,  3636,  3643,  3650,  3652,  3654,  3655,  3656,
    3657,  3659,  3661,  3665,  3666,  3669,  3685,  3701,  3705,  3709,
    3710,  3714,  3715,  3719,  3721,  3723,  3725,  3727,  3731,  3732,
    3734,  3750,  3766,  3770,  3774,  3776,  3780,  3784,  3787,  3790,
    3793,  3796,  3801,  3803,  3808,  3809,  3813,  3820,  3827,  3834,
    3841,  3842,  3846,  3853,  3854,  3855,  3858,  3860,  3862,  3865,
    3872,  3881,  3888,  3897,  3899,  3901,  3903,  3907,  3910,  3914,
    3915,  3919,  3928,  3930,  3934,  3936,  3940,  3946,  3947,  3950,
    3954,  3957,  3961,  3962,  3965,  3967,  3971,  3973,  3977,  3979,
    3983,  3985,  3989,  3992,  3995,  4000,  4003,  4013,  4015,  4017,
    4019,  4021,  4023,  4025,  4027,  4031,  4032,  4035,  4036,  4039,
    4047,  4050,  4053,  4056,  4059,  4062,  4065,  4068,  4071,  4074,
    4077,  4080,  4083,  4086,  4089,  4092,  4095,  4098,  4101,  4104,
    4107,  4110,  4113,  4116,  4119,  4124,  4125,  4130,  4132,  4133,
    4134,  4135,  4136,  4137,  4138,  4139,  4140,  4141,  4142,  4143,
    4144,  4145,  4146,  4147,  4148,  4149,  4151,  4152,  4153,  4154,
    4155,  4156,  4157,  4158,  4159,  4160,  4161
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
     470,   470,   470,   470,   470,   470,   470,   471,   472,   473,
     473,   474,   474,   474,   474,   474,   474,   474,   475,   475,
     476,   476,   476,   476,   476,   476,   476,   477,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     479,   479,   480,   480,   481,   482,   482,   482,   483,   483,
     484,   484,   484,   485,   486,   486,   487,   487,   488,   489,
     489,   490,   490,   490,   490,   490,   491,   492,   492,   493,
     493,   494,   495,   495,   495,   495,   495,   495,   495,   495,
     495,   495,   495,   495,   495,   496,   496,   497,   497,   498,
     498,   499,   499,   500,   500,   501,   501,   502,   502,   502,
     502,   502,   502,   502,   502,   502,   503,   503,   504,   504,
     504,   504,   505,   505,   506,   506,   506,   507,   507,   508,
     508,   508,   509,   509,   509,   510,   510,   510,   511,   511,
     512,   512,   512,   513,   513,   514,   514,   514,   515,   515,
     516,   516,   516,   516,   517,   517,   518,   518,   518,   519,
     519,   519,   520,   520,   520,   521,   522,   523,   523,   524,
     525,   525,   526,   527,   528,   528,   529,   529,   530,   530,
     531,   532,   532,   533,   533,   534,   534,   534,   534,   535,
     535,   536,   536,   537,   537,   538,   538,   538,   539,   539,
     539,   540,   540,   540,   541,   542,   542,   542,   543,   543,
     544,   544,   545,   546,   546,   547,   547,   548,   549,   549,
     549,   550,   551,   551,   551,   551,   551,   551,   551,   551,
     551,   551,   551,   551,   551,   551,   552,   552,   553,   554,
     554,   555,   556,   556,   557,   557,   558,   558,   559,   560,
     561,   562,   562,   562,   563,   564,   565,   566,   567,   568,
     569,   569,   569,   569,   570,   570,   570,   571,   571,   571,
     572,   572,   573,   573,   574,   574,   575,   575,   576,   576,
     577,   578,   578,   579,   579,   580,   580,   580,   581,   582,
     582,   583,   583,   584,   584,   585,   585,   585,   586,   587,
     588,   589,   589,   590,   591,   592,   592,   593,   593,   594,
     594,   595,   596,   596,   596,   596,   596,   597,   597,   598,
     598,   599,   599,   600,   601,   601,   602,   602,   603,   603,
     604,   605,   605,   605,   605,   605,   606,   606,   607,   607,
     608,   608,   609,   609,   609,   610,   610,   611,   611,   612,
     612,   613,   613,   614,   615,   615,   615,   616,   616,   617,
     617,   618,   618,   618,   619,   619,   620,   620,   621,   622,
     623,   623,   624,   625,   626,   627,   627,   628,   629,   630,
     630,   631,   631,   631,   632,   632,   633,   634,   634,   635,
     635,   636,   636,   637,   638,   638,   639,   639,   640,   641,
     641,   642,   642,   642,   643,   644,   644,   645,   645,   645,
     645,   645,   646,   646,   646,   646,   646,   646,   647,   647,
     647,   648,   648,   648,   648,   648,   648,   648,   648,   648,
     648,   648,   649,   649,   649,   649,   649,   649,   649,   649,
     650,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   650,   650,   650,   650,   651,   651,   652,   652,
     653,   653,   654,   654,   654,   654,   655,   655,   656,   656,
     657,   657,   657,   657,   658,   659,   660,   660,   661,   661,
     661,   662,   662,   662,   663,   663,   664,   665,   665,   665,
     665,   665,   665,   665,   665,   665,   665,   665,   665,   665,
     665,   665,   665,   665,   665,   665,   665,   665,   665,   665,
     665,   665,   665,   665,   665,   666,   666,   666,   667,   668,
     668,   669,   669,   670,   670,   670,   670,   670,   671,   671,
     671,   671,   671,   672,   673,   673,   674,   675,   675,   675,
     675,   675,   676,   676,   677,   677,   678,   679,   680,   681,
     682,   682,   683,   684,   684,   684,   685,   685,   685,   685,
     685,   685,   685,   685,   685,   685,   685,   686,   687,   688,
     688,   689,   690,   690,   691,   691,   692,   693,   693,   694,
     694,   694,   695,   695,   696,   696,   697,   697,   698,   698,
     699,   699,   700,   701,   702,   703,   703,   704,   704,   704,
     704,   704,   704,   704,   704,   705,   705,   706,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   732,   733,   733,   734,   734,   734,
     734,   734,   734,   734,   734,   734,   734,   734,   734,   734,
     734,   734,   734,   734,   734,   734,   734,   734,   734,   734,
     734,   734,   734,   734,   734,   734,   734
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
       5,     5,     5,     5,     5,     5,     5,     1,     6,     1,
       4,     7,     2,     0,     4,     3,     6,     1,     1,     6,
       8,     6,     1,     1,     4,     6,     4,     5,     3,     5,
       4,     5,     1,     1,     1,     4,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     1,
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
       0,     0,   595,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   595,   683,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   577,    19,   582,   583,   584,     6,    18,
     576,   578,     5,    20,   663,   770,   671,   674,    12,     9,
     786,   787,    22,   790,   791,     4,   488,     0,     0,     0,
       0,     0,     0,     0,     0,   407,   594,   597,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   129,     0,     0,
       0,   103,   776,   966,     0,    23,   964,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,     0,   369,    29,    37,    34,
      36,   812,    35,   812,  1055,  1057,  1058,  1059,  1060,  1061,
    1062,  1081,  1082,  1063,  1065,  1064,  1070,  1066,  1071,  1067,
    1069,  1072,  1068,  1073,  1074,  1075,  1077,  1078,  1076,  1079,
    1080,  1084,  1083,  1085,  1086,     0,    31,     0,    32,     0,
      67,    69,  1047,  1056,     0,     0,     0,     0,     0,     0,
       0,   593,     0,     0,  1026,   677,   682,     0,     0,   602,
       0,   729,   731,  1049,     0,     0,     0,     0,   116,   585,
    1054,     0,     0,     2,     0,   667,     0,  1028,   783,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   467,   461,   462,   460,   463,   464,   465,   466,   468,
     469,     0,   437,   438,   408,     0,  1038,     0,   414,     0,
    1042,   411,   266,  1046,     0,   731,   410,   385,  1050,   415,
       0,  1041,   599,   592,   962,     0,   112,   166,   165,   113,
     148,   215,   824,  1034,   104,     0,   110,   160,     0,   106,
     266,   114,   968,   137,   107,   200,   108,     0,   109,   812,
    1053,   115,   161,   111,   774,   775,     0,   967,   965,     0,
      75,     0,  1039,   747,   509,  1037,   502,     0,   508,   513,
     503,   504,   511,   505,   506,   507,   512,   514,   363,     0,
     268,    30,     0,    39,   811,    38,    28,     0,     0,     0,
       0,     0,   817,     0,     0,     0,     0,     0,     0,   591,
     972,     0,   953,   676,   676,   677,   689,   687,  1025,   690,
       0,   691,   681,   677,     0,     0,     0,     0,     0,     0,
     618,   611,   600,   601,   603,   605,   606,   607,   614,   615,
     608,     0,   730,  1048,   120,     0,   117,   266,   118,   200,
     119,   812,   588,     0,   641,   642,   649,     0,   648,   650,
     652,   657,   645,   643,   646,   644,   653,   656,   651,   655,
       0,     0,     0,   665,     0,   661,     0,     0,  1027,     0,
       0,   668,     0,     0,     0,     0,   412,   489,   377,     0,
     377,     0,   218,   413,   419,   128,  1030,   416,   485,   478,
     479,     0,   268,   239,   470,     0,   409,   428,     0,   383,
     384,   498,     0,   598,   596,   963,   163,   163,     0,     0,
       0,   175,   167,   168,   147,     0,   125,     0,     0,     0,
     121,   268,   136,   135,   138,     0,     0,     0,     0,     0,
      76,   218,     0,     0,   749,   789,   510,     0,     0,     0,
       0,   930,   959,     0,  1029,     0,   932,     0,     0,   960,
       0,   954,   945,   944,   943,  1055,     0,     0,     0,   935,
     936,   937,   946,   947,   955,   956,  1065,   957,   958,     0,
    1070,  1066,   961,  1079,  1084,     0,     0,     0,     0,   912,
     366,   907,   821,   939,   933,   908,   361,   948,   910,   911,
     927,   928,   929,   949,   909,   973,   974,   987,   975,   989,
     916,   913,   914,   997,   998,  1002,  1003,   915,   934,     0,
       0,  1034,     0,     0,     0,   370,   216,     0,     0,     0,
     814,    27,     0,    33,     0,     0,    43,    70,    72,  1051,
      68,     0,     0,     0,   816,     0,    48,    50,    28,     0,
       0,   590,     0,     0,     0,   686,   684,     0,   693,     0,
     692,   694,   699,   680,     0,   637,     0,   612,   623,   609,
     623,   610,   624,   630,   634,   635,   619,   604,   747,   795,
     825,     0,     0,   126,   268,     0,     0,   587,   586,   654,
     647,     0,     0,  1045,     0,     0,   662,   666,   769,   771,
     776,   673,   672,   781,   183,     0,   491,   144,     0,   494,
       0,   495,   490,     0,   423,     0,   425,   424,     0,   426,
       0,   420,     0,   487,     0,     0,   269,     0,     0,   432,
       0,   433,   377,   471,     0,   477,     0,     0,     0,   474,
    1034,   388,   387,   497,     0,   405,     0,   164,     0,     0,
     173,     0,   172,     0,     0,   162,   176,   177,   179,   169,
     546,   558,   533,   568,   536,   559,   569,     0,   575,   537,
       0,   565,     0,   560,   572,   532,   555,   534,   535,   530,
     377,   515,   516,   517,   525,   531,   518,   527,   548,     0,
     553,   550,   528,   565,   529,   124,     0,   122,   266,   123,
       0,     0,   139,     0,     0,   199,   239,   385,   375,     0,
     218,     0,    82,    84,     0,     0,   102,   101,  1031,     0,
       0,     0,     0,   898,   899,   900,     0,   746,   829,   838,
     841,   847,   842,   843,   844,   845,   849,   850,   848,   851,
     846,   827,     0,   755,   770,  1026,     0,  1026,   950,     0,
       0,  1026,  1026,  1026,     0,   951,   952,     0,     0,  1013,
       0,     0,     0,     0,     0,     0,   918,   917,     0,   770,
     939,     0,   331,  1052,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,   525,   548,     0,
       0,   368,     0,   279,     0,   813,     0,     0,    44,    73,
       0,     0,    26,   803,     0,   819,     0,   785,   803,     0,
       0,    47,   589,     0,   676,     0,     0,   747,     0,   698,
       0,   580,     0,   579,   970,     0,   613,     0,   621,   617,
     616,     0,   627,     0,   632,   620,     0,   749,   793,     0,
       0,     0,   239,   375,   660,   638,   659,     0,     0,     0,
     664,     0,   780,     0,     0,   145,   184,   493,   377,   422,
       0,   377,   378,   427,   484,     0,   483,   375,     0,   265,
     375,   238,  1035,   210,   207,     0,   150,   377,   515,   375,
       0,     0,     0,   237,   240,   241,   242,   243,     0,   431,
     476,   475,   430,   429,     0,     0,     0,   391,   389,   390,
     386,     0,   377,   481,     0,   480,   193,   170,   174,     0,
       0,   178,     0,     0,   543,   556,   557,   573,     0,   570,
     575,     0,   563,   561,   562,   150,     0,     0,     0,   526,
       0,     0,     0,     0,   564,   127,   268,     0,   375,   140,
     198,     0,   201,   203,   204,     0,     0,     0,     0,   218,
     218,    79,     0,    87,    78,     0,     0,     0,     0,   840,
     828,     0,     0,   901,   902,   903,     0,   832,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   751,   753,   754,
     748,     0,   783,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,     0,     0,     0,     0,  1008,
       0,     0,     0,     0,     0,     0,     0,   926,   783,   925,
     362,     0,   332,   364,   333,     0,     0,   921,  1033,   920,
     919,   922,   923,   924,   940,   995,     0,   931,   823,   822,
     372,   371,     0,   272,   287,     0,   278,   280,   815,     0,
       0,     0,    71,     0,   808,     0,   818,     0,   808,     0,
       0,   685,   688,     0,   700,   701,   704,   706,   703,   715,
     716,   717,   705,   728,     0,   740,   695,   697,  1043,   581,
     971,   622,   631,   626,   625,     0,   636,   796,   770,   797,
     826,   375,     0,     0,   639,   658,     0,   772,   678,   773,
     782,   492,   163,   190,   182,   185,   186,     0,     0,     0,
     421,   486,   375,   375,   378,   277,   270,   279,   209,   377,
     377,   378,   211,   206,   212,     0,     0,   248,     0,   435,
     378,   377,   375,   434,     0,     0,     0,   404,   500,   482,
     194,   195,   171,     0,   181,   544,   545,   969,     0,     0,
       0,   548,     0,   571,     0,     0,   378,     0,   521,   523,
       0,     0,     0,     0,     0,     0,     0,   385,   279,   163,
     142,   239,   196,   405,   380,     0,    83,     0,   218,     0,
      91,     0,    85,     0,     0,    90,    86,     0,   794,  1036,
       0,     0,   833,     0,     0,   839,     0,     0,   831,   830,
       0,   888,     0,   877,   876,     0,   855,     0,   886,   904,
       0,   896,     0,     0,     0,   856,   882,     0,     0,   859,
       0,     0,     0,     0,     0,     0,     0,   857,     0,     0,
     858,     0,   890,     0,     0,   852,     0,     0,   853,     0,
       0,   854,   752,   755,   788,     0,     0,   218,     0,   976,
       0,     0,   990,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1014,     0,     0,  1012,     0,     0,  1004,     0,
       0,     0,     0,  1015,   670,     0,     0,     0,   938,   994,
       0,   267,   286,     0,   338,   367,   281,     0,     0,     0,
       0,     0,  1061,    41,    52,    54,    53,    62,    64,    41,
      42,     0,     0,     0,   800,   820,     0,   799,     0,     0,
       0,   736,   703,     0,   738,   732,   738,   736,   738,     0,
       0,     0,   699,     0,   745,   628,   629,   633,   783,   279,
     197,   380,   640,     0,     0,   191,     0,   187,     0,     0,
       0,   417,   226,   221,   941,   222,   223,   224,   225,     0,
       0,   271,     0,     0,     0,   227,     0,   248,   776,   244,
       0,   436,   472,   473,   396,   397,   393,   392,   395,   394,
     377,   499,   378,   180,  1032,     0,     0,   539,     0,   538,
     574,     0,   566,   151,     0,   519,     0,   547,   548,   554,
     552,     0,   549,   375,     0,     0,     0,   134,   143,   202,
     377,     0,   376,   105,     0,   132,   131,    94,     0,     0,
      95,     0,    88,    89,     0,   894,   895,   835,   834,   837,
     836,     0,     0,     0,     0,   770,     0,     0,     0,   897,
       0,     0,     0,     0,     0,     0,   889,   887,   883,     0,
     891,     0,     0,     0,     0,   892,     0,     0,     0,     0,
       0,     0,   759,     0,   756,     0,   760,   982,   981,     0,
       0,   978,   977,     0,   986,   985,   984,   983,   980,   979,
       0,   993,  1010,  1009,     0,     0,     0,     0,     0,     0,
       0,   365,   335,   334,   273,     0,     0,   283,   284,     0,
    1034,   321,     0,     0,     0,     0,     0,     0,  1063,  1068,
       0,     0,   359,   297,   292,     0,   338,   291,     0,     0,
       0,   317,   318,     0,   315,     0,     0,   316,     0,     0,
       0,   353,   355,   354,     0,     0,     0,     0,     0,     0,
       0,   798,     0,  1034,    66,    58,    59,    65,    60,    61,
       0,    24,     0,     0,    25,     0,   804,   805,   806,   802,
     807,   810,   809,   784,    45,    46,   699,   707,   702,   737,
     735,   733,     0,   734,     0,     0,     0,     0,     0,     0,
     749,   792,     0,   376,   777,   778,   679,   193,   192,   188,
       0,   378,   942,   275,   276,   378,   378,   149,   214,   228,
     229,   377,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   279,   496,   541,     0,   542,     0,     0,   214,
     152,   153,   155,   156,   157,   522,   524,   520,   551,   279,
     405,   378,   193,     0,     0,   373,   377,    92,     0,     0,
      98,    93,   548,     0,   880,     0,     0,   906,   783,   905,
     878,     0,     0,     0,   884,     0,     0,     0,     0,   893,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     750,   755,     0,   762,     0,   758,   761,   996,   217,  1016,
     988,     0,     0,  1011,  1006,  1005,     0,   999,  1001,     0,
     220,   282,     0,   319,     0,   314,     0,   329,   313,   341,
    1044,   356,   357,     0,   293,     0,   298,   295,   344,   308,
     309,   305,   306,   310,   303,     0,     0,   312,   311,   304,
       0,   300,   302,   301,   299,   307,   337,     0,    55,    56,
      57,    63,     0,   710,     0,   718,   736,   727,   725,   739,
     741,   743,   744,   675,   378,   374,   779,   189,   378,   159,
     264,     0,     0,   205,   230,     0,     0,     0,   248,   231,
     234,   812,     0,   378,   402,   403,   399,   398,   401,   400,
       0,   540,   567,   158,   146,   154,     0,   377,   261,   141,
     378,   379,     0,     0,    96,    97,    80,     0,   863,   871,
     669,   879,   864,   872,   867,   875,   881,   885,   865,   873,
     866,   874,   860,   868,   861,   869,   862,   870,   757,     0,
     768,   992,   991,  1007,     0,     0,     0,   289,   285,     0,
     320,   322,     0,   328,     0,   352,     0,     0,     0,     0,
     347,   345,     0,     0,     0,   330,    40,   801,   712,   709,
     719,     0,     0,   720,   721,   722,     0,   262,   418,   208,
     213,   232,   248,   812,   235,   254,   247,   249,   406,   378,
       0,   381,   378,    77,    99,   100,     0,   765,     0,   764,
    1000,   219,   288,     0,     0,     0,     0,   351,   350,  1040,
     349,   338,     0,     0,     0,     0,     0,   708,   723,     0,
     742,   236,   254,     0,   248,   250,   251,   263,   378,     0,
     763,     0,     0,     0,   325,   338,   358,   346,   348,     0,
     343,     0,     0,   713,     0,   248,     0,     0,   246,     0,
     253,     0,   252,   382,   133,   766,     0,   290,   327,     0,
       0,     0,   338,     0,   711,   724,   233,     0,     0,   257,
     256,   255,   767,   338,     0,     0,   338,   336,   714,   260,
     258,   259,   326,   338,   342,   323,   324,     0,     0,     0
};

static const short yydefgoto[] =
{
    1967,    21,    22,   542,   145,   146,   147,   148,  1561,   812,
     807,    23,   159,   160,  1313,  1314,  1315,  1316,  1317,   149,
     150,   546,   547,    24,    89,   450,   963,   721,   722,   723,
    1206,  1199,  1200,  1651,   280,   725,    25,    81,    26,   178,
      27,   440,   254,   354,   705,   224,    82,  1423,   261,   444,
     713,  1190,  1417,   616,   249,   435,  1141,  1629,  1630,  1631,
    1632,  1633,  1634,   256,   271,   246,   658,   247,   431,   432,
     433,   665,   666,   667,   617,   618,  1124,  1125,  1126,  1356,
    1162,   264,   358,   446,   951,   952,   953,   884,   885,   886,
    1763,   640,   532,  1479,   724,  1827,  1361,  1608,  1609,  1610,
    1769,   954,   642,   893,   894,   895,   896,  1379,   897,  1904,
    1905,  1906,  1950,   259,   356,   707,   231,   412,   538,   635,
     636,  1062,   533,  1134,  1065,  1066,  1067,  1506,  1507,  1303,
    1828,  1508,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1532,  1533,  1534,  1535,  1833,  1536,   499,  1043,  1537,
    1538,  1539,  1941,  1717,  1718,  1839,  1840,  1540,  1541,  1542,
    1543,  1713,  1544,   500,   785,    29,   300,   534,   535,   268,
     360,   956,  1645,   625,  1130,  1422,   266,   709,   421,   653,
     654,   910,   912,  1391,    30,    65,   872,   869,   403,   404,
     416,   417,   221,   222,   418,  1150,   648,   902,   228,   239,
     407,   633,   876,   189,   396,   397,   236,   655,  1392,    31,
     104,   690,   691,   692,  1177,  1178,   693,   797,   695,   696,
    1171,   924,   939,   697,   798,   699,   700,   701,   702,   932,
     703,   704,   929,    32,    33,    34,    35,    36,   598,    37,
     562,    38,    39,    67,   243,   424,    40,   342,   343,   344,
     345,   346,   347,   348,   349,   839,   350,  1105,  1347,   582,
     583,   844,   584,    41,    42,   380,   381,   382,   859,   855,
    1545,   185,   606,   385,    44,  1444,   778,    45,    46,    47,
     565,   332,  1353,  1756,   164,   165,   166,   330,   569,   570,
     571,   830,   827,  1084,  1085,  1086,  1087,  1848,  1897,  1922,
    1088,  1089,  1090,  1091,  1853,  1854,  1855,  1092,  1342,   170,
     171,  1340,  1580,  1344,  1749,  1750,  1590,   454,   754,  1472,
    1001,  1473,  1474,  1475,  1685,  1876,  1879,   188,   608,   609,
     276,  1596,  1119,   391,  1546,  1547,    50,    51,  1548,    53,
      54,   455,   588,  1549,  1550,    55,  1074,  1565,  1566,  1324,
    1570,   303,   304,   539,   553,   554,   814,   501,   502,   591,
     737,   976,   738,   739,   740,   741,  1225,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   978,  1228,  1229,
     752,   504,   505,   506,  1363,   507,   508,   509,   510,   511,
     512,   513,  1568,    86,   278,   263,  1169,  1179,   834,   514,
     515,   516,   517,   518,   519,  1692,   520,   521,   522,   523,
     524,   525,  1032,   526,   770,  1286,   771,  1283,   527,  1025,
     331,   389,   528,   529,   727,  1393,  1047,   252,   881,  1208,
     284,   225,   281,  1888,   257,  1877,  1097,  1552,   602,  1093,
     151,   530,   235,   237,  1318,   782,   269,   179,   531,   153
};

static const short yypact[] =
{
    1344,   586,    87,  1639,   314,   730,   434,  1492,    76,  6796,
     624,   525,  1435,    87,   690,   384,  5686,   694,  5686,   420,
     516,   453,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   609,   318,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  7749,  5686,   720,
    5686,  5686,  5686,  5686,  5686,-32768,-32768,   604,   513,  5686,
    5686,  5686,   748,  5686,   603,  5686,  5686,-32768,  5686,  5686,
    5686,-32768,   231,-32768,   613,-32768,-32768,   758,  5686,-32768,
    5686,  5686,  5686,   765,  5686,  5686,  5686,  5686,   603,  5686,
    5686,  5686,  5686,  5686,-32768,  5686,   558,   728,-32768,-32768,
  -32768,   601,-32768,   601,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   764,   614,   789,-32768,    70,
  -32768,-32768,-32768,-32768,  5686,  5686,  5686,   779,   790,   481,
    5686,   734,   272,   294,   653,   696,-32768,  5686,   860,   546,
     808,-32768,  5686,-32768,  5686,  5686,  5686,  5686,-32768,-32768,
  -32768,  5686,  2456,-32768,   800,   792,   972,   708,   750,   969,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   356,-32768,-32768,-32768,   513,-32768,   828,-32768,   469,
  -32768,-32768,   723,-32768,   236,-32768,-32768,   508,-32768,-32768,
     787,-32768,   896,-32768,-32768,   865,-32768,   784,-32768,-32768,
    1057,-32768,-32768,-32768,-32768,   513,-32768,-32768,   169,-32768,
     723,-32768,-32768,   207,-32768,  1020,-32768,   998,-32768,   601,
  -32768,-32768,-32768,-32768,-32768,-32768,  1001,-32768,-32768,   828,
  -32768,  1013,-32768,   909,-32768,-32768,-32768,   828,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6469,  5686,
     951,-32768,  5686,-32768,-32768,-32768,   935,   577,   974,  4153,
    5686,  1031,   839,  1055,  1059,  1060,  1002,  1007,   159,   879,
  -32768,  6688,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5155,-32768,-32768,-32768,   956,  5686,  1029,   946,   155,  5686,
     966,-32768,-32768,   546,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  5686,-32768,-32768,-32768,   513,-32768,   723,-32768,  1020,
  -32768,   601,  1014,   983,-32768,-32768,-32768,  1075,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1058,  5686,  5686,  1026,   905,-32768,  6688,   995,-32768,   995,
    6688,-32768,    83,   897,   901,   906,   969,-32768,  1129,   211,
  -32768,  5686,-32768,   356,-32768,-32768,-32768,-32768,  1110,-32768,
  -32768,  5686,   951,  2934,   957,  3184,   888,-32768,  7749,-32768,
  -32768,   479,   977,-32768,-32768,-32768,   915,   915,   513,  1077,
     513,   133,   784,-32768,-32768,  7696,-32768,  5686,  5686,  5686,
  -32768,   951,-32768,-32768,  1158,   367,   914,  5686,  1188,  5686,
  -32768,    57,  5248,  4279,  1062,-32768,-32768,   925,   926,   934,
    1018,-32768,-32768,   940,-32768,   941,-32768,   947,   950,-32768,
     952,-32768,-32768,-32768,-32768,   953,   955,  1027,  1044,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   967,-32768,-32768,  5374,
     975,   978,-32768,   979,  1150,  6688,  6688,  5593,  5686,-32768,
    1111,   984,-32768,  1157,-32768,-32768,   981,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   999,
     993,  1003,  7696,  1009,   339,-32768,-32768,  1006,  1257,   348,
  -32768,-32768,  5686,-32768,  5686,  5686,    12,-32768,-32768,-32768,
  -32768,  5686,  5686,   591,-32768,  5686,-32768,-32768,   935,  1144,
    4153,-32768,  5686,  5593,   394,-32768,-32768,  6688,-32768,  1226,
    1015,-32768,   187,-32768,   324,-32768,   686,-32768,   382,-32768,
     382,-32768,  1016,-32768,   101,-32768,  1124,-32768,    89,-32768,
  -32768,  1028,  1021,-32768,   951,  1032,  1286,-32768,-32768,-32768,
  -32768,   413,  1232,-32768,  1033,  5686,-32768,-32768,  1045,-32768,
     380,-32768,-32768,   989,-32768,  1241,  1243,-32768,   513,-32768,
    1244,-32768,-32768,  1318,-32768,  1326,-32768,-32768,  1306,-32768,
    7696,-32768,   513,  1242,  7696,   596,-32768,  1342,  5686,-32768,
    7363,-32768,   163,-32768,  5686,   682,   236,  1181,  1148,  1222,
    1185,-32768,-32768,-32768,   151,  1230,   399,-32768,  1171,   603,
  -32768,   513,-32768,  1350,  1284,-32768,   133,-32768,-32768,-32768,
      31,  1193,-32768,-32768,-32768,  1196,-32768,  1245,  1104,-32768,
    1296,  1112,   641,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1108,-32768,-32768,-32768,   109,  1113,
    1117,   241,-32768,  1112,-32768,-32768,   513,-32768,   723,-32768,
    1335,  1387,-32768,   513,   513,-32768,  2934,   508,-32768,  1276,
  -32768,  1258,  1122,-32768,  7652,  1278,-32768,-32768,-32768,  1287,
    1131,  4498,  1132,   965,  1467,  1577,  4029,   193,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2280,   552,  1289,   742,  6688,   150,-32768,  5686,
    6688,   769,   776,   781,  1587,-32768,-32768,  6688,  4717,  1157,
     176,  1223,  6688,  6688,  4717,  1356,   157,   157,  1134,   318,
     651,   620,-32768,-32768,  4372,-32768,  6688,  5686,  6688,  6688,
    6688,  6688,  6688,  6688,  5812,  3435,-32768,-32768,  1402,  1141,
    5686,-32768,  5686,  1384,  5686,-32768,  1262,  1263,-32768,-32768,
    1236,  4153,-32768,  1393,   630,-32768,  1166,-32768,  1393,  5686,
    5686,  1177,-32768,   651,-32768,   662,  4810,  1265,  6688,-32768,
    5686,-32768,   569,-32768,-32768,   177,-32768,  1270,-32768,-32768,
  -32768,  5686,   104,  5686,-32768,-32768,  5686,  1062,-32768,  6688,
    5686,  1447,  2934,-32768,-32768,-32768,-32768,   413,  5686,  1375,
    1186,  6688,  1224,  6688,   513,-32768,   519,-32768,-32768,-32768,
    1190,-32768,-32768,-32768,-32768,   513,-32768,   107,  5686,-32768,
  -32768,-32768,-32768,  1446,-32768,  1192,  1423,-32768,  1444,-32768,
    1386,  1391,   601,-32768,-32768,-32768,-32768,-32768,  1456,-32768,
  -32768,-32768,-32768,-32768,  5686,  7461,   603,  1355,  1357,  1360,
  -32768,   603,-32768,-32768,   583,-32768,   816,-32768,-32768,  1330,
     513,-32768,  5248,   285,  1334,-32768,-32768,-32768,   603,-32768,
    1104,   336,-32768,-32768,-32768,  1423,   336,  1337,   336,-32768,
     603,   603,  1218,   603,-32768,-32768,   951,  5686,-32768,  1403,
  -32768,   640,-32768,-32768,-32768,   479,   995,  5686,   654,    36,
  -32768,-32768,  1225,   275,-32768,  5248,  5686,   995,  4936,-32768,
  -32768,   995,  4498,-32768,-32768,-32768,   120,-32768,   197,  1983,
    4717,  4717,  6688,  6688,  1911,  1228,   557,  2208,  6688,  2560,
     616,  2810,  3060,  6031,  3310,  3560,  3810,-32768,-32768,  1396,
  -32768,  1229,   750,  6688,  6688,   254,  6688,  1235,  6688,  1231,
     670,  6688,  6688,  6688,  6688,  6688,  6688,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1453,  1130,   182,  6688,-32768,
    4717,  6688,   248,   428,   465,   141,  5686,-32768,   750,-32768,
  -32768,  4591,-32768,  1246,-32768,  1238,   -19,-32768,-32768,  1495,
     157,   157,   648,   648,  1157,-32768,   698,-32768,-32768,-32768,
  -32768,-32768,   704,-32768,  1338,  1503,  1384,-32768,-32768,  7680,
    7680,  1401,-32768,   153,  1421,  5686,-32768,  6688,  1421,  1468,
    1469,-32768,-32768,  3684,  1253,   220,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1255,  4717,  1464,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,    92,-32768,-32768,  1289,  1157,
  -32768,-32768,   715,   995,-32768,-32768,   413,-32768,-32768,-32768,
    1157,-32768,   915,  1524,-32768,   519,-32768,  1259,  4717,  1740,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1384,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   601,   601,  1300,  1271,-32768,
  -32768,-32768,-32768,-32768,   181,   139,   441,-32768,  1531,-32768,
  -32768,-32768,-32768,  5686,-32768,-32768,-32768,-32768,   314,   731,
    1383,  1402,  1269,-32768,  1171,   745,-32768,   449,-32768,  1275,
    5686,   570,  1277,  1279,   603,  1280,  1547,   508,  1384,   915,
    1243,  2934,-32768,  1230,  1370,  2684,-32768,   603,  1430,  1512,
  -32768,  7652,-32768,   603,     9,-32768,-32768,  1514,-32768,-32768,
    1288,  1290,-32768,  4717,  4717,-32768,  4717,  4717,-32768,  1564,
     205,  1157,  1299,-32768,-32768,  1301,  1157,  5593,-32768,-32768,
    1518,-32768,   202,  1304,  1313,  1157,  1187,  1314,  1317,  1157,
    6688,  6688,  1228,  6688,  6250,  1319,  1320,  1157,  1321,  1323,
    1157,  6688,  1157,  1324,  1328,  1157,  1329,  1331,  1157,  1332,
    1336,  1157,-32768,  3913,-32768,   687,   695,-32768,   777,-32768,
     803,  6688,-32768,   814,   822,   830,   870,   904,   927,  6688,
    6688,  6688,  1157,  1533,   186,  1157,  1431,  6688,-32768,  6688,
    6688,  6688,  6688,-32768,-32768,   760,  4591,  5467,-32768,-32768,
    5686,-32768,-32768,  5686,  7037,-32768,-32768,  5686,  5686,  5686,
    5686,  5686,  5686,  1404,  1346,-32768,  1351,-32768,-32768,  1404,
  -32768,   320,   320,   250,-32768,-32768,   767,-32768,  7680,  7680,
    1343,  1369,  1359,  4810,  1486,-32768,  1486,   647,  1486,  1536,
    1544,  6688,  1250,  1612,  1562,-32768,-32768,-32768,   750,  1384,
  -32768,  1370,-32768,   274,  1171,  1506,  1171,-32768,  4717,   128,
     819,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1740,
    1740,-32768,  1503,  6688,  1740,   501,  1494,  1300,   231,-32768,
    4717,-32768,-32768,-32768,  1515,  1516,  1517,  1520,  1521,  1522,
  -32768,-32768,-32768,-32768,-32768,  1364,   314,-32768,  1452,-32768,
  -32768,   336,-32768,   198,   336,-32768,   336,-32768,  1402,-32768,
  -32768,  1371,-32768,-32768,   479,  1503,  1171,-32768,  1243,-32768,
  -32768,  1629,-32768,-32768,  1376,-32768,  1186,-32768,  1374,   513,
     170,  1377,-32768,-32768,   513,-32768,-32768,-32768,  1564,-32768,
    1564,  6688,   995,   995,  1378,   318,   768,  6688,  1592,-32768,
     995,   995,  6688,   995,   995,   228,  1157,-32768,  1201,  6688,
    1157,   995,   995,   995,   995,  1157,   995,   995,   995,   995,
     995,   995,-32768,  1379,  1382,   638,  5686,-32768,-32768,  1388,
    7696,-32768,-32768,   948,-32768,-32768,-32768,-32768,-32768,-32768,
    1000,  1147,-32768,-32768,  6688,  6688,  1610,  1490,  1008,   786,
    1088,-32768,-32768,-32768,-32768,  7696,  1389,-32768,-32768,  1630,
    1633,  5686,   229,  1394,  1395,  1589,  6688,  1398,  1405,  5029,
    5686,  5686,-32768,-32768,-32768,  1626,  6929,-32768,  1406,  1407,
    1408,-32768,-32768,  1409,-32768,  1412,  1413,-32768,   137,  1414,
    1415,-32768,-32768,-32768,  1416,  1617,  1424,  1425,  1426,  1427,
    1429,-32768,  1428,   600,-32768,-32768,-32768,-32768,-32768,-32768,
    1636,-32768,  7680,  7680,-32768,  1535,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1700,-32768,   220,-32768,
  -32768,-32768,  1627,-32768,  4810,  4810,   794,  5686,  6688,  4717,
    1062,-32768,  1503,-32768,-32768,-32768,-32768,   816,-32768,-32768,
     132,-32768,-32768,-32768,-32768,-32768,  1157,-32768,  1688,   501,
  -32768,   722,  5686,-32768,  1634,   135,  1534,  1641,  1539,  1672,
    1643,  1680,  1384,-32768,-32768,  1448,-32768,  1449,  1635,  1688,
     198,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1384,
    1230,-32768,   816,  1531,  1606,-32768,-32768,-32768,  1620,    91,
  -32768,-32768,  1402,  1621,  1157,  1454,  1466,-32768,   750,-32768,
    1157,  6688,  1470,  1472,  1157,  1473,  1476,  6688,  6688,  1157,
    1477,  1480,  1481,  1482,  1483,  1487,  1488,  1489,  1493,  1497,
  -32768,  3913,  1465,-32768,  5686,-32768,-32768,-32768,-32768,-32768,
  -32768,  6688,  1498,-32768,-32768,-32768,  6688,-32768,-32768,  6688,
     116,-32768,  1686,  6688,  6688,-32768,  4717,  1157,-32768,-32768,
  -32768,-32768,-32768,  5686,-32768,   183,-32768,  1576,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   158,  1491,-32768,-32768,-32768,
    4591,-32768,-32768,-32768,-32768,-32768,-32768,  1647,-32768,-32768,
  -32768,-32768,   320,  5686,  4810,-32768,  1091,-32768,-32768,  1504,
  -32768,  1157,   193,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1507,  5686,-32768,-32768,  1689,  1696,  5686,  1300,-32768,
  -32768,   601,  5686,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1503,-32768,-32768,-32768,-32768,-32768,  1503,-32768,-32768,-32768,
  -32768,-32768,  6688,   513,-32768,  1584,-32768,   513,-32768,-32768,
  -32768,  1157,-32768,-32768,-32768,-32768,  1157,  1157,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5686,
    1710,  1157,-32768,-32768,  1034,  1740,  1740,-32768,-32768,  1513,
    1157,  1345,   162,-32768,  1708,-32768,  5686,  5686,   314,   124,
  -32768,-32768,  1546,  1711,  4717,  1246,-32768,-32768,  1529,-32768,
  -32768,  4717,  1530,-32768,-32768,-32768,  6688,-32768,-32768,-32768,
  -32768,-32768,  1300,   601,-32768,  1673,-32768,-32768,-32768,-32768,
    1531,-32768,  1157,-32768,-32768,-32768,  1538,  1537,  1541,-32768,
  -32768,-32768,-32768,   995,  4591,  1640,  4591,-32768,-32768,-32768,
  -32768,  7172,   183,  6688,  4591,   166,  5686,-32768,   193,  5686,
  -32768,-32768,  1673,   143,  1300,  1690,  1698,-32768,-32768,  1545,
  -32768,  5686,  5686,  1550,  1246,  7172,  1246,-32768,-32768,  1352,
     117,  1776,   813,-32768,   831,  1300,   522,   522,-32768,  1781,
  -32768,  1650,-32768,-32768,-32768,-32768,  1552,-32768,  1782,  4591,
    1797,  1806,  7172,  5686,-32768,-32768,-32768,  1653,   415,-32768,
  -32768,-32768,-32768,  7172,   144,  5686,  7172,-32768,-32768,-32768,
  -32768,-32768,-32768,  7172,-32768,-32768,-32768,  1844,  1855,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1298,  1702,-32768,  1703,  1556,   540,-32768,
    1046,-32768,-32768,-32768,  -913,-32768,  -609,-32768,  -529,  1704,
    1557,  1308,  1061,-32768,-32768,-32768,   668,-32768,  1151,   910,
  -32768,-32768,   675,-32768,-32768,   911,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   684,-32768,-32768,   943,-32768,-32768,   245,
  -32768,-32768,  1485,  1799,-32768,-32768,  -386,-32768,-32768,-32768,
    1457,-32768,-32768,  1214,  -407,-32768,-32768,-32768,   761,-32768,
   -1128,-32768,-32768,  1526,  1036,   700,  1479,  1005,   699,-32768,
     266,   -59,  -293,-32768,  -373,-32768, -1235,-32768,-32768,   287,
  -32768,  1484, -1223,-32768,-32768,-32768,-32768, -1305,   288,    -1,
       3,    -5,   -20,-32768,-32768,-32768,-32768,  -139,  -339,-32768,
    1035,-32768,  -726,-32768, -1034,-32768,   845,-32768,-32768,-32768,
  -32768,-32768, -1393, -1019,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -772, -1010,-32768,
  -32768,-32768,-32768,-32768,   199,-32768,    22,-32768,-32768,-32768,
  -32768,-32768,  1915,-32768,-32768,-32768,-32768,-32768,  1118,-32768,
  -32768,  -678,   330,  -395,  -957,   574,-32768,-32768,  -598,  -849,
  -32768,-32768, -1086, -1431,-32768,-32768,-32768,-32768,-32768,  1523,
  -32768,  1281,  1510,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1542,-32768,-32768,-32768,-32768,
  -32768,  -602,  -492,-32768,   992,   528,  -646,  -410,-32768,-32768,
  -32768,-32768, -1058,-32768,  -408,-32768,-32768,-32768,-32768,  1239,
  -32768,-32768,  1011,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1932,-32768,-32768,-32768,-32768,-32768,  1605,
  -32768,-32768,-32768,  1380,-32768,  1372,-32768,-32768,-32768,-32768,
    1114,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -693,
       1,-32768,-32768,-32768,  -485,  -125,-32768,  -464,   467,-32768,
    -249,   451,-32768,-32768,-32768,-32768,  1786,-32768,-32768,-32768,
    1125, -1189,-32768,-32768, -1027, -1398,-32768,-32768,-32768,-32768,
     874,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1877,
     -47,   631,  -317,-32768,-32768,   113,-32768,  -447,  -813,  1217,
  -32768,   290,-32768,   496,-32768,  -857,-32768,  -674,-32768,  1115,
    -574,-32768,-32768,  -928,  1975,  1978,-32768,-32768,  1979,-32768,
  -32768,  1397,-32768,  -236,-32768,-32768,  1163, -1198,-32768,   908,
  -32768,  -109,  -748,  -594,-32768,-32768,-32768,  -758,   -34,  -493,
    -637,-32768,  -502,-32768,-32768,-32768,   456,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  -553,-32768,   741,-32768,
    1049, -1066,-32768,  -453,-32768,   627, -1045,  -149, -1018, -1012,
   -1006,   -18,    -2,   -51,-32768,   -78,  -858,  -542,  -145,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768, -1259,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   701,-32768, -1155,-32768,-32768,
     433,-32768,  -921,   454,  1066,   804,   227,  -695,  1347, -1246,
  -32768,   -62,  1898,-32768,    -6,   -54, -1285,   474,  -266,     4,
     -63,  -169,     5,   -57,  -272,-32768,   -79,   -56,    -9,-32768
};


#define	YYLAST		8017


static const short yytable[] =
{
     152,    43,    85,   352,   305,   628,   229,   173,   255,   180,
     250,   860,  1042,   323,   326,   234,   272,   324,   327,   267,
     292,   172,   295,   262,   668,   694,  1044,   698,   265,   630,
     286,  1295,   833,   779,  1108,   251,   862,   548,   887,   296,
     796,   659,   290,   294,   781,  1551,  1305,   262,   223,   226,
     248,   230,   233,   173,   238,   241,  1331,  1509,   240,   815,
     253,   226,   241,  1362,   233,   232,   173,   238,   817,   270,
     152,   241,  1613,   637,  1264,   566,  1063,   260,   964,   282,
    1002,   173,   285,   226,  1364,   282,   241,   230,   233,   289,
     173,   238,   270,   152,   241,   172,   233,   297,   361,   779,
    1059,   291,   711,  1372,   293,  1038,  1193,  1420,   312,   298,
    1294,  1365,   355,  1399,   915,   589,   604,  1366,   634,   955,
     311,   441,   313,  1367,  1569,   362,  1492,  1940,  1213,   359,
     937,  1027,  1496,  1716,  1603,  1604,   980,  1035,   873,  1607,
     980,   847,   877,   980,  1147,   253,   173,   253,   888,   980,
    1132,   152,  1611,  1587,  1415,  1110,   328,  1319,   241,  1825,
     448,   334,   842,   353,  1114,   226,   233,   173,   270,  1197,
     980,  1891,   180,   614,   980,  1113,   663,  1371,   970,   357,
     787,   578,   592,  1386,  1375,   788,  1745,  1926,  1794,  1649,
     980,  1963,  1835,  1381,   980,   907,  1006,   829,  1725,  1135,
     922,   980,  1137,   578,   810,  1216,   -81,   405,  1368,   865,
     787,  1144,  1790,  1441,  1842,   788,  1321,   105,   594,  1403,
    -377,   442,   560,   614,   437,   890,  1028,  1029,   787,   969,
     694,   908,   698,   788,   977,  1345,  1667,   436,  1836,  1432,
     413,   626,   274,   414,   309,  1214,  1103,   898,  1448,  1837,
    -696,   787,   596,   981,   627,   851,   788,   981,   793,   668,
     981,  1384,  1104,  1298,  1267,   536,   981,  1551,   540,  1042,
    1188,  1433,   779,   809,  1711,  1712,   275,   787,   579,   453,
      66,   575,   788,  1044,  1334,   415,  1346,   981,   548,   811,
     253,   981,   585,   253,  1204,   935,  1449,  1335,  1287,  1288,
     549,   152,   891,  1362,  1362,  -736,  1578,   981,  1362,   923,
     438,   981,   443,    14,  1198,  1592,  1628,   590,   981,  1387,
    1336,  1795,  1217,  1927,  1364,  1364,   230,  1322,  1726,  1364,
     173,   909,   580,  1046,  1337,   720,   106,   593,   615,  1693,
    1694,  1056,   892,  1218,  1219,   439,  1850,   310,  1838,   581,
    1637,  1365,  1365,  1605,   580,  1281,  1365,  1366,  1366,  1494,
     398,  1385,  1366,  1367,  1367,  1030,   846,   629,  1367,  1205,
     105,  1338,   603,   603,  1650,   706,  1133,   536,   843,   251,
    1095,   645,   710,   592,   649,  1826,  1611,  1743,   664,  1175,
     938,   274,   253,  1284,  1296,   719,  1641,  1376,  1377,  1215,
     717,  1892,   253,   787,   253,   399,   253,  1601,   788,   650,
     660,  1758,   662,  1151,  1773,  1574,  1575,   787,  1292,  1212,
    1591,  1296,   788,  1352,  1007,   275,   942,   715,   226,   233,
     238,   791,   792,  1349,  1348,  1623,   310,  1289,   173,  1908,
     230,  1885,   708,   728,   186,  1921,  1167,  1425,  1368,  1368,
     726,   787,   167,  1368,  1369,  1370,   788,   714,  1960,   789,
     790,   791,   792,  1864,  -696,  1572,   969,  1834,   320,  1757,
     969,  1194,   409,  1127,  1383,  1704,  1129,   789,   790,   791,
     792,  1167,  1210,  1339,   651,  1388,  1211,   107,   787,   783,
     320,  1359,  1142,   788,   652,   806,   187,    90,  1917,   837,
     789,   790,   791,   792,   813,  1594,   822,   854,   818,  1961,
      83,   419,   400,   916,  1789,  1595,    83,  1158,   590,   943,
     320,  1389,  1938,  -239,  1502,   108,   789,   790,   791,   792,
     401,   638,   320,   173,   322,   233,   549,   109,  1503,   548,
    1626,  1823,   173,   156,  1847,   410,   173,   831,   808,  1957,
     321,   549,   168,   180,  1787,   964,   322,  1901,  1746,   169,
    1962,   110,  1168,  1965,   402,  1640,   244,   245,   838,   946,
    1966,   540,   325,  1754,  1504,  1081,  1437,  1438,   322,  1439,
    1440,   917,  1325,   856,   420,   634,  1201,    84,  1780,  1414,
    1881,  1882,  1115,    84,  1796,   320,   253,   832,  1330,  1928,
     866,  1426,  1059,  1230,   322,  1786,   889,  1186,   262,  1174,
    1107,  1923,   244,   245,   874,  1045,   800,  1122,   801,  -239,
    1946,   108,   913,    56,  1326,   804,   111,   805,  1351,   882,
     336,   253,  1551,  1240,    57,   882,   112,   997,   182,  1947,
    -239,    58,   154,   918,  1759,    59,  -239,  1241,  1760,  1761,
    1060,  1231,   789,   790,   791,   792,  1551,   110,  1958,   328,
     998,   113,   933,   337,   181,    60,   789,   790,   791,   792,
     184,   787,   914,   824,   787,  1232,   788,   592,  1948,   788,
    -239,   408,   251,  1551,  1788,   787,  1123,  1100,   945,   338,
     788,  1242,   339,   787,  1551,   949,   950,  1551,   788,   329,
     789,   790,   791,   792,  1551,  1290,   155,   253,   340,  1964,
     787,  1334,   244,   245,   387,   788,   999,  1682,   787,  1243,
    1845,  1600,   111,   788,  1335,  1949,  1404,    61,   934,   183,
    1800,  1405,   112,   451,   341,  1639,  1354,   789,   790,   791,
     792,   456,  1291,  1615,  1373,  1374,    14,  1336,   328,   174,
     241,   242,  1683,  1009,   388,   258,  1382,   113,    62,  1362,
    1362,  1868,    63,  1445,  1684,   320,   536,  1869,   536,  1100,
    1068,  1658,  1079,   816,  1446,   328,   567,  1753,  1048,   320,
    1364,  1364,   328,  1244,   573,   227,   253,   328,  1003,    87,
      88,   253,  1099,   253,   585,   253,  1106,  1857,  1338,    83,
     787,  1858,   549,  1416,  1614,   788,  1159,  1365,  1365,   277,
     173,   233,   590,  1366,  1366,  1011,  1867,   233,   251,  1367,
    1367,  1098,  1013,   279,   808,  1201,   787,  1015,  1153,   835,
     287,   788,   173,  1871,   173,   175,   299,   787,  1499,   856,
    1765,   253,   788,   253,   536,   787,  1121,  1404,   340,   603,
     788,    64,  1408,   787,   611,   262,   612,  1131,   788,  1627,
    1157,  1766,  1182,  1183,  1636,  1185,   176,  1767,   301,   253,
    1149,  1152,  1045,   878,  1914,   879,  1916,  1172,  1688,   302,
   -1044,   177,   426, -1048,  1920,   900,   306,   901,  1586,   262,
     262,   307,   262,   787,  1480,   253,   253,   793,   788,  1040,
    1187,  1768,  1164,   314,  1368,  1368,   592,  1075,   319,  1076,
    1195,   308,  1907,   728,   315,  1909,   427,  1191,   428,  1192,
    1165,   162,   163,   789,   790,   791,   792,   787,   163,  1954,
    1039,   960,   788,  1196,   789,   790,   791,   792,   173,   335,
     429,  1082,   789,   790,   791,   792,  1160,  1161,   173,  1272,
     787,  1933,  1752,  1738,  1740,   788,   728,  1209,  1042,   789,
     790,   791,   792,   726,   351,   430,  1477,   789,   790,   791,
     792,   787,  1044,   392,  1478,   793,   788,  1299,  1445,  1445,
     383,  1300, -1073,  1301,   384,   393,  1445,  1445, -1073,  1445,
    1445,   386,  1191, -1073,  1350,  1622, -1073,  1445,  1445,  1445,
    1445,   411,  1445,  1445,  1445,  1445,  1445,  1445,  1396,   390,
    1397,   865,   787,  1700,   472,   473,   474,   788,   394,  1581,
     395,  1583,  1401,   787,  1402,  1643,   406,   241,   788,  1100,
    1293,   787, -1073,  1739,  1741,  1554,   788,  1296,  1557,  1501,
   -1073,   590,   482,   483,   793,   793,  1573,  1659, -1073,   789,
     790,   791,   792,   422,  1935,  1936,  1481,   787,   423,   447,
     549,   549,   788,   793,   425,  1698, -1073,   434, -1073,  1832,
     694,   793,   698,  1747,   233,   789,   790,   791,   792,   445,
     449, -1073,  1482, -1073, -1073, -1073,   789,   790,   791,   792,
    1943,   452,  1944,  1484,   789,   790,   791,   792,   856,   453,
     537,  1485,   789,   790,   791,   792,  1411,   541,   804,  1486,
    1945,   787,  1042,   551,  1042,   544,   788,   552,   555,  1427,
     556,   557,  1042,   561,   558,  1431,  1044,  1045,  1044,   559,
     574,   576, -1073,   262,   577,   592,  1044,   597,   586,   599,
     600,   601,   789,   790,   791,   792,   262,   605,   607,  1487,
      14,   619,   262,   787,  1394,  1334,   620,   251,   788,   623,
     621,   540,   632,   863,   643,   646,  1395,  1042,  1335,   656,
     787,  1394,  1567,  1567,  1571,   788,   789,   790,   791,   792,
     787,  1044,   253,  1488,   657,   788,   253,   712,  1004,   661,
    1008,  1336,   716,  1280,  1012,  1014,  1016,   753,   718,   789,
     790,   791,   792,   755,   756,  1337,  1489,  1895,  1691,  1597,
     787,  1599,   757,  1851,  1898,   788,   898,   758,   759,   760,
     789,   790,   791,   792,   787,   761,   765,  1689,   762,   788,
     763, -1030,  1558,   764, -1073, -1073, -1073, -1073, -1073, -1073,
   -1073,  1452,  1338,   766,  1505,   767, -1073,   775, -1073,  1559,
    -726,  1792,  1556,   772,   173,  1668,   773,   774,   793,   784,
     829,   789,   790,   791,   792,   786,   536,   803,  1476,   251,
     590,  1642,   789,   790,   791,   792,   795,   794,   799,  1690,
     789,   790,   791,   792,   802,   820, -1048,  1697,   253,   826,
     845,   253,   828,   841,  1510,  1553,   853,   849,   549,   233,
     238,   549,   270,   152,   850,  1924,   789,   790,   791,   792,
     852,  -726,  1555,  1880,  -726,   857,   858,  1655,  1656,   549,
     549,  -726,   861,  -726,   233,  1662,  1663,  -726,  1665,  1666,
    1852,   864,  -726,   614,   867,  -726,  1670,  1671,  1672,  1673,
     868,  1674,  1675,  1676,  1677,  1678,  1679,   503,   870,   871,
    -726,     1,   880,   875,  1339,   904,   905,   592,   906,  -438,
     789,   790,   791,   792,  -726,  1699,   911,   320,   787,     2,
     564,   919,  -726,   788,   920,   787,  -726,     3,   925,   572,
     788,   926,   928,   927,     4,  -726,     5,   930,     6,   936,
     931,   940,  1870,     7,  1625,   941,   947,   948,   957,   960,
       8,  -726,   789,   790,   791,   792,   965,   959,   966,   967,
     971,  1648,  1031,  1037,     9,   186,  1653,  1036,  1748,   789,
     790,   791,   792,   937,    10,   322,  1064,  1884,  -726,   789,
     790,   791,   792,  1334,  1939,   610,  1069,  1070,  1071,   613,
    -726,   -49,  -726,  1234,  1077,  1238,  1335,  1246,  1249,  1703,
    1254,  1257,  1260,  1073,   811,  1094,  1101,  1111,  1116,   789,
     790,   791,   792,   804,  1118,  1138,  1140,   173,  1128,  1336,
    1139,   883,  1145,   789,   790,   791,   792,  1146,  1148,   -49,
    1154,  1476,  1155,  1337, -1074,  1156,  1163,    11,  1170,  -726,
   -1074,   -49,   590,  1180,    12, -1074,  1184,    13, -1074,    14,
      15,  1189,   226,  1203,  1262,   157,  1227,  1263,  1271,  -726,
    1710,  1209,  1209,  -726,  1269,   -49,  1279,  1553,   787,  1304,
    1338,  1297,  1302,  1296,    16,  1320,  -726,  -726,  1323,  -726,
    1333,  1328,  1329,  1341, -1074,  1343,  1355,  1358,   769,  1378,
      91,  1390, -1074,  1398,   776,   777,   780,    92,  1400,  1380,
   -1074,    93,    94,   549,   549,  1406,  1409,  1413,  1410,  1412,
      95,  1045,  1421,  1197,  1429,  1771,  1434,  1435, -1074,  1436,
   -1074,    96,   980,    17,  1849,   233,   233,  1442,   353,  1443,
     -49,  1447,  1450, -1074,  1493, -1074, -1074, -1074,    18,    19,
     -49,  1451,  1453,  1567, -1075,  1454,  1560,  1461,  1462,  1463,
   -1075,  1464,  1466,   173,  1495, -1075,  1467,  1468, -1075,  1469,
    1470,    20,   823,  1579,  1471,   -49,   825,   789,   790,   791,
     792,  1584,  1576,  1562,   789,   790,   791,   792,  1563,  1585,
    1820,  1588,  1339,    97, -1074,  1589,  1598,   158,  1577,  1612,
    1616,  1617,  1618,  1624, -1075,  1619,  1620,  1621,  1167,    98,
    1638,  1644, -1075,  1647,  1646,  1661,  1652,  1657,  1680,  1681,
   -1075,  1695,  1865,  1696,    99,  1701,  1702,  1687,   100, -1036,
    1705,   155,   173,  1706,  1708,   230,    68,  1714, -1075,   101,
   -1075,  -339,  1719,  1720,  1721,  1722,  1476,    69,  1723,  1724,
    1727,  1728,  1729, -1075,    70, -1075, -1075, -1075,   102,  1730,
    1731,  1732,  1733,  1734,  1209,  1735,  1737,    71,  1736,  1742,
     829,  1762,  1744,  1772,  1774,  1045,  1777,  1045,  1866,  1776,
    1863,  1775,   592,  1778,  1779,  1045,  1843,  1781,  1782,  1783,
    1791,  1793,  1797,  1798,   353,   233, -1074, -1074, -1074, -1074,
   -1074, -1074, -1074,  1819, -1075,  1799,   592,  1829, -1074,  1802,
   -1074,  1803,  1804,  1048,  1902,  1805,  1808,   103,   173,  1809,
    1810,  1811,  1812,   230,    72,  1715,  1813,  1814,  1815,  1844,
    1045,  1846,  1816,   592,  1887,  1873,  1817,  1822,   460,  1875,
      73,  1856,  1862,  1861,   592,   979,  1859,   592,  1874,  1878,
    1886,  1883,  1893,  1894,   592,  1903,    74,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1005,   462,  1896,  1899,  1010,
     230,    75,  1929,  1915,  1911,    76,  1026,  1910,    77,  1912,
    1931,  1033,  1034,  1942,  1934,  1926,    78,   226,  1889,  1937,
    1927,  1952,  1953,  1955,   464,   503,  1890,  1049,  1050,  1051,
    1052,  1053,  1054,   503,  1968,    79, -1075, -1075, -1075, -1075,
   -1075, -1075, -1075,  1956,  1959,  1969,   819,   590, -1075,  1564,
   -1075,   316,   317,   543,   318,   540,  1080,   550,   821,  1430,
    1202,   958,  1072,  1428,  1418,  1785,  1207,   572,  1176,   273,
     921,   590,  1553,   624,  1913,   595,  1357,  1098,  1112,   669,
     253,  1419,   639,  1143,  1424,  1784,  1764,   641,  1109,  1770,
    1930,  1925,   230,   230,    80,   469,  1553,  1951,   590,  1932,
     610,  1306,  1120,  1136,  1918,    28,  1841,  1222,  1061,   590,
    1223,   471,   590,  1755,   457,  1593,   631,   903,   647,   590,
    1181,   458,  1635,  1553,  1098,   472,   473,   474,   622,   244,
     245,  1173,   944,   459,  1553,   161,  1209,  1553,   587,   460,
     461,   333,   840,  1096,  1553,  1102,   836,  1332,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   283,  1582,  1900,
    1000,  1818,  1686,   487,   488,    48,  1117,   462,    49,    52,
     463,  1078,   492,  1457,  1407,   848,  1327,  1602,  1166,  1860,
    1497,   899,   288,  1709,     0,     0,     0,     0,     0,     0,
     982,     0,     0,     0,     0,   464,   787,   465,   466,     0,
       0,   788,     0,  1360,   983,     0,     0,   467,     0,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1220,  1221,  1226,     0,     0,  1235,  1236,  1239,     0,
    1247,  1250,  1252,  1255,  1258,  1261,     0,     0,     0,     0,
     984,     0,  1265,  1266,     0,  1268,     0,  1270,   985,     0,
    1273,  1274,  1275,  1276,  1277,  1278,   986,     0,     0,     0,
       0,     0,     0,     0,  1224,     0,   469,  1282,     0,     0,
    1285,     0,   470,     0,   987,     0,   988,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,     0,   989,
       0,   990,   991,   992,     0,     0,   472,   473,   474,   475,
     244,   245,   115,     0,   116,   117,   118,   119,     0,   120,
     476,     0,     0,     0,     0,     0,   503,   121,   122,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   123,   486,
       0,   125,     0,     0,   487,   488,     0,   489,   490,   491,
     993,   128,   129,   492,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   493,   140,     0,   494,   142,   143,     0,   144,
       0,     0,     0,   495,   496,     0,     0,     0,     0,   563,
       0,   498,     0,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1233,     0,     0,  1223,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     459,     0,     0,     0,     0,     0,   460,   461,     0,     0,
       0,     0,   994,   995,   996,   789,   790,   791,   792,     0,
       0,     0,  1039,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   462,     0,   503,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1455,
    1456,     0,  1458,  1460,     0,     0,     0,   982,     0,     0,
    1465,     0,   464,   787,   465,   466,     0,     0,   788,     0,
       0,   983,     0,     0,   467,     0,     0,     0,   468,     0,
    1483,     0,     0,     0,     0,     0,     0,     0,  1490,  1491,
    1282,     0,     0,     0,     0,     0,  1282,     0,  1285,  1498,
     503,  1500,     0,     0,     0,     0,     0,   984,     0,     0,
       0,     0,     0,     0,     0,   985,     0,     0,     0,     0,
       0,     0,     0,   986,     0,     0,     0,     0,     0,     0,
       0,  1224,     0,   469,     0,     0,     0,     0,     0,   470,
       0,   987,     0,   988,     0,     0,     0,     0,     0,   471,
     503,     0,     0,     0,     0,     0,   989,     0,   990,   991,
     992,     0,     0,   472,   473,   474,   475,   244,   245,   115,
       0,   116,   117,   118,   119,     0,   120,   476,     0,     0,
       0,     0,  1606,     0,   121,   122,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   123,   486,     0,   125,     0,
       0,   487,   488,     0,   489,   490,   491,   993,   128,   129,
     492,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   493,
     140,     0,   494,   142,   143,     0,   144,   363,     0,     0,
     495,   496,     0,     0,     0,     0,   563,     0,   498,     0,
    1654,     0,   322,   364,     0,     0,  1660,     0,     0,     0,
       0,  1664,     0,     0,   365,     0,     0,     0,  1669,     0,
       0,   366,     0,     0,     0,   367,   368,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   370,     0,     0,     0,     0,
       0,     0,     0,  1282,  1282,     0,     0,     0,     0,   994,
     995,   996,   789,   790,   791,   792,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1707,  1237,     0,     0,  1223,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
     458,     0,     0,     0,     0,     0,     0,     0,     0,   371,
       0,     0,   459,     0,     0,     0,     0,   372,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,   373,   463,
       0,     0,   374,     0,     0,     0,     0,  1751,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,     0,   465,   466,     0,     0,
       0,     0,   376,   377,     0,     0,   467,     0,     0,     0,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1801,   883,     0,     0,     0,     0,  1806,  1807,     0,     0,
       0,   378,     0,  1224,   379,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
    1821,   471,     0,     0,     0,  1282,     0,     0,  1824,     0,
       0,     0,  1830,  1831,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,     0,   487,   488,     0,   489,   490,   491,     0,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,  1245,     0,     0,  1223,
     138,   493,   140,   457,   494,   142,   143,     0,   144,     0,
     458,     0,   495,   496,     0,     0,     0,     0,   563,     0,
     498,  1872,   459,     0,   322,     0,     0,     0,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,   463,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   464,  1751,   465,   466,     0,     0,
       0,   123,   124,     0,   125,     0,   467,     0,     0,     0,
     468,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,  1919,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   638,     0,     0,     0,     0,     0,
       0,     0,     0,  1224,     0,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,     0,   487,   488,     0,   489,   490,   491,     0,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,  1248,     0,     0,  1223,
     138,   493,   140,   457,   494,   142,   143,     0,   144,     0,
     458,     0,   495,   496,     0,     0,     0,     0,   563,     0,
     498,     0,   459,     0,   322,     0,     0,     0,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,   463,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   464,     0,   465,   466,     0,     0,
       0,   123,   124,     0,   125,     0,   467,     0,     0,     0,
     468,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   644,     0,     0,     0,     0,     0,
       0,     0,     0,  1224,     0,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,     0,   487,   488,     0,   489,   490,   491,     0,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,  1253,     0,     0,  1223,
     138,   493,   140,   457,   494,   142,   143,     0,   144,     0,
     458,     0,   495,   496,     0,     0,     0,     0,   563,     0,
     498,     0,   459,     0,   322,     0,     0,     0,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,   463,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   464,     0,   465,   466,     0,     0,
       0,   123,   124,     0,   125,     0,   467,     0,     0,     0,
     468,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1224,  1057,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,     0,   487,   488,     0,   489,   490,   491,     0,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,  1256,     0,     0,  1223,
     138,   493,   140,   457,   494,   142,   143,     0,   144,     0,
     458,     0,   495,   496,     0,     0,     0,     0,   563,     0,
     498,     0,   459,     0,   322,     0,     0,     0,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,   463,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,   464,     0,   465,   466,     0,     0,
       0,     0,   123,   124,     0,   125,   467,     0,     0,     0,
     468,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,  1058,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1224,     0,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,     0,   487,   488,     0,   489,   490,   491,     0,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,  1259,     0,     0,  1223,
     138,   493,   140,   457,   494,   142,   143,     0,   144,     0,
     458,     0,   495,   496,     0,     0,     0,     0,   563,    14,
     498,     0,   459,     0,   322,     0,     0,     0,   460,   461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,   463,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   464,     0,   465,   466,     0,     0,
       0,   123,   124,     0,   125,     0,   467,     0,     0,     0,
     468,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1083,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1224,     0,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,   997,     0,
       0,     0,     0,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,   998,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,   457,   487,   488,     0,   489,   490,   491,   458,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,   459,     0,     0,   136,   137,     0,   460,   461,     0,
     138,   493,   140,     0,   494,   142,   143,   999,   144,     0,
       0,     0,   495,   496,     0,     0,   730,     0,   563,     0,
     498,     0,     0,     0,   322,   462,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,   464,     0,   465,   466,     0,     0,   121,
     122,     0,     0,     0,     0,   467,     0,     0,     0,   468,
     123,   124,     0,   125,     0,     0,     0,   972,     0,     0,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,    14,     0,     0,     0,   732,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,   473,   474,   475,   244,   245,
     115,     0,   116,   117,   118,   119,     0,   120,   476,     0,
       0,     0,     0,     0,     0,   121,   122,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   123,   486,     0,   125,
       0,     0,   487,   488,     0,   489,   490,   491,     0,   128,
     129,   492,   130,     0,     0,   131,   132,   133,   134,   135,
     973,   974,   975,   136,   137,     0,     0,     0,     0,   138,
     493,   140,   457,   494,   142,   143,     0,   144,     0,   458,
       0,   495,   496,     0,     0,     0,     0,   736,     0,   498,
       0,   459,     0,   322,   729,     0,     0,   460,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,     0,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   462,     0,     0,   463,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   464,     0,   465,   466,     0,     0,     0,
     123,   124,     0,   125,     0,   467,     0,     0,     0,   468,
     126,   127,     0,   128,   129,     0,   130,   731,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   732,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,   473,   474,   475,   244,   245,
     115,     0,   116,   117,   118,   119,     0,   120,   476,     0,
       0,     0,     0,     0,     0,   121,   122,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   123,   486,     0,   125,
       0,   457,   487,   488,     0,   489,   490,   491,   458,   128,
     129,   492,   130,     0,     0,   131,   132,   733,   734,   735,
     459,     0,     0,   136,   137,     0,   460,   461,     0,   138,
     493,   140,     0,   494,   142,   143,     0,   144,     0,     0,
       0,   495,   496,     0,     0,   730,     0,   736,     0,   498,
       0,     0,     0,   322,   462,     0,     0,   463,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   464,     0,   465,   466,     0,     0,     0,   123,
     124,     0,   125,     0,   467,     0,     0,     0,   468,   126,
     127,     0,   128,   129,     0,   130,   968,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1041,     0,   498,     0,     0,     0,     0,   732,     0,     0,
       0,     0,     0,   469,     0,     0,     0,     0,     0,   470,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,   473,   474,   475,   244,   245,   115,
       0,   116,   117,   118,   119,     0,   120,   476,     0,     0,
       0,     0,     0,     0,   121,   122,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   123,   486,     0,   125,     0,
     457,   487,   488,     0,   489,   490,   491,   458,   128,   129,
     492,   130,     0,     0,   131,   132,   733,   734,   735,   459,
       0,     0,   136,   137,     0,   460,   461,     0,   138,   493,
     140,     0,   494,   142,   143,     0,   144,     0,     0,     0,
     495,   496,     0,     0,   730,     0,   736,     0,   498,     0,
       0,     0,   322,   462,     0,     0,   463,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   464,     0,   465,   466,     0,     0,     0,   123,   124,
       0,   125,     0,   467,     0,     0,     0,   468,   126,   127,
       0,   128,   129,     0,   130,   731,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,   732,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   472,   473,   474,   475,   244,   245,   115,     0,
     116,   117,   118,   119,     0,   120,   476,     0,     0,     0,
       0,     0,     0,   121,   122,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   123,   486,     0,   125,     0,   457,
     487,   488,     0,   489,   490,   491,   458,   128,   129,   492,
     130,     0,     0,   131,   132,   733,   734,   735,   459,     0,
       0,   136,   137,     0,   460,   461,     0,   138,   493,   140,
       0,   494,   142,   143,     0,   144,     0,     0,     0,   495,
     496,     0,     0,   730,     0,   736,     0,   498,     0,     0,
       0,   322,   462,     0,     0,   463,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
     464,     0,   465,   466,     0,     0,     0,   123,   124,     0,
     125,     0,   467,     0,     0,     0,   468,   126,   127,     0,
     128,   129,     0,   130,   968,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1083,     0,
       0,     0,     0,     0,     0,   732,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,   470,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   472,   473,   474,   475,   244,   245,   115,     0,   116,
     117,   118,   119,     0,   120,   476,     0,     0,     0,     0,
       0,     0,   121,   122,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   123,   486,     0,   125,     0,   457,   487,
     488,     0,   489,   490,   491,   458,   128,   129,   492,   130,
       0,     0,   131,   132,   133,   134,   135,   459,     0,     0,
     136,   137,     0,   460,   461,     0,   138,   493,   140,     0,
     494,   142,   143,     0,   144,     0,     0,     0,   495,   496,
       0,     0,     0,     0,   736,     0,   498,     0,     0,     0,
     322,   462,     0,     0,   463,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   464,
       0,   465,   466,     0,     0,     0,   123,   124,     0,   125,
       0,   467,     0,     0,     0,   468,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,  -340,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,   473,   474,   475,   244,   245,   115,     0,   116,   117,
     118,   119,     0,   120,   476,     0,     0,     0,     0,     0,
       0,   121,   122,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   123,   486,     0,   125,     0,   457,   487,   488,
       0,   489,   490,   491,   458,   128,   129,   492,   130,     0,
       0,   131,   132,   133,   134,   135,   459,     0,     0,   136,
     137,     0,   460,   461,     0,   138,   493,   140,     0,   494,
     142,   143,     0,   144,     0,     0,     0,   495,   496,   568,
       0,     0,     0,   563,     0,   498,     0,     0,     0,   322,
     462,     0,     0,   463,    83,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   464,     0,
     465,   466,     0,     0,     0,   123,   124,     0,   125,     0,
     467,     0,     0,     0,   468,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   470,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,     0,     0,   768,     0,     0,     0,     0,     0,   472,
     473,   474,   475,   244,   245,   115,     0,   116,   117,   118,
     119,     0,   120,   476,     0,     0,     0,     0,     0,     0,
     121,   122,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   123,   486,     0,   125,     0,   457,   487,   488,     0,
     489,   490,   491,   458,   128,   129,   492,   130,     0,     0,
     131,   132,   133,   134,   135,   459,     0,     0,   136,   137,
       0,   460,   461,     0,   138,   493,   140,     0,   494,   142,
     143,     0,   144,     0,     0,     0,   495,   496,     0,     0,
       0,     0,   563,     0,   498,     0,     0,     0,   322,   462,
       0,     0,   463,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   464,     0,   465,
     466,     0,     0,     0,   123,   124,     0,   125,     0,   467,
       0,     0,     0,   468,   126,   127,     0,   128,   129,     0,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,  1058,     0,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,   473,
     474,   475,   244,   245,   115,     0,   116,   117,   118,   119,
       0,   120,   476,     0,     0,     0,     0,     0,     0,   121,
     122,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     123,   486,     0,   125,     0,   457,   487,   488,     0,   489,
     490,   491,   458,   128,   129,   492,   130,     0,     0,   131,
     132,   133,   134,   135,   459,     0,     0,   136,   137,     0,
     460,   461,     0,   138,   493,   140,     0,   494,   142,   143,
       0,   144,     0,     0,     0,   495,   496,     0,     0,     0,
       0,   563,     0,   498,     0,     0,     0,   322,   462,     0,
       0,   463,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,   464,     0,   465,   466,
       0,     0,     0,   123,   124,     0,   125,     0,   467,     0,
       0,     0,   468,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   472,   473,   474,
     475,   244,   245,   115,     0,   116,   117,   118,   119,     0,
     120,   476,     0,     0,     0,     0,     0,     0,   121,   122,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   123,
     486,     0,   125,     0,   457,   487,   488,     0,   489,   490,
     491,   458,   128,   129,   492,   130,     0,     0,   131,   132,
     133,   134,   135,   459,     0,     0,   136,   137,     0,   460,
     461,     0,   138,   493,   140,     0,   494,   142,   143,     0,
     144,     0,     0,     0,   495,   496,     0,     0,     0,     0,
     563,  1055,   498,     0,     0,     0,   322,   462,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   464,     0,   465,   466,     0,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1251,     0,     0,   472,   473,   474,   475,
     244,   245,   115,     0,   116,   117,   118,   119,     0,   120,
     476,     0,     0,     0,     0,     0,     0,   121,   122,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   123,   486,
       0,   125,     0,   457,   487,   488,     0,   489,   490,   491,
     458,   128,   129,   492,   130,     0,     0,   131,   132,   133,
     134,   135,   459,     0,     0,   136,   137,     0,   460,   461,
       0,   138,   493,   140,     0,   494,   142,   143,     0,   144,
       0,     0,     0,   495,   496,     0,     0,     0,     0,   563,
       0,   498,     0,     0,     0,   322,   462,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,     0,   465,   466,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,     0,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1459,     0,     0,   472,   473,   474,   475,   244,
     245,   115,     0,   116,   117,   118,   119,     0,   120,   476,
       0,     0,     0,     0,     0,     0,   121,   122,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   123,   486,     0,
     125,     0,   457,   487,   488,     0,   489,   490,   491,   458,
     128,   129,   492,   130,     0,     0,   131,   132,   133,   134,
     135,   459,     0,     0,   136,   137,     0,   460,   461,     0,
     138,   493,   140,     0,   494,   142,   143,     0,   144,     0,
       0,     0,   495,   496,     0,     0,     0,     0,   563,     0,
     498,     0,     0,     0,   322,   462,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   464,     0,   465,   466,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,   468,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,   473,   474,   475,   244,   245,
     115,     0,   116,   117,   118,   119,     0,   120,   476,     0,
       0,     0,     0,     0,     0,   121,   122,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   123,   486,     0,   125,
       0,   457,   487,   488,     0,   489,   490,   491,   458,   128,
     129,   492,   130,     0,     0,   131,   132,   133,   134,   135,
     459,     0,     0,   136,   137,     0,   460,   461,     0,   138,
     493,   140,     0,   494,   142,   143,     0,   144,     0,     0,
       0,   495,   496,     0,     0,     0,     0,   497,     0,   498,
       0,     0,     0,   322,   462,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   464,     0,   465,   466,     0,     0,     0,     0,
       0,     0,     0,     0,   467,     0,     0,     0,   468,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   109,   469,     0,     0,     0,     0,     0,   470,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,   472,   473,   474,   475,   244,   245,   115,
       0,   116,   117,   118,   119,     0,   120,   476,     0,     0,
       0,     0,     0,     0,   121,   122,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   123,   486,     0,   125,     0,
       0,   487,   488,     0,   489,   490,   491,     0,   128,   129,
     492,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,   111,   136,   137,     0,  1304,     0,     0,   138,   493,
     140,   112,   494,   142,   143,     0,   144,     0,     0,     0,
     495,   496,     0,     0,     0,     0,   563,     0,   498,     0,
       0,     0,   322,     6,     0,     0,   113,     0,     0,     0,
    -294,     0,     0,     0,  1511,  1512,     0,  1513,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,  1514,   120,     0,     0,     0,     0,  1515,
       0,     0,   121,   122,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,   125,     0,     0,     0,
       0,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,  1304,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,  1516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     0,     0,    14,     0,     0,     0,  -296,     0,
       0,     0,  1511,  1512,     0,  1513,     0,     0,     0,  1517,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,  1514,     0,     0,     0,     0,     0,  1515,  1715,     0,
       0,     0,     0,     0,     0,     0,     0,   475,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1518,   124,     0,   125,
       0,     0,     0,     0,     0,     0,   126,   127,     0,   128,
     129,     0,   130,     0,  1516,   131,  1519,   133,   134,   135,
       0,     0,     0,   136,   137,  1520,  1521,  1522,  1304,   138,
     139,   140,    14,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1517,     0,     0,
       0,     0,     0,     0,     0,     0,     6,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1511,  1512,     0,
    1513,     0,     0,     0,     0,   475,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,  1514,     0,     0,     0,
       0,     0,  1515,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1518,   124,     0,   125,     0,     0,
       0,     0,     0,     0,   126,   127,     0,   128,   129,     0,
     130,     0,     0,   131,  1519,   133,   134,   135,     0,     0,
       0,   136,   137,  1520,  1521,  1522,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,  1516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1517,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     475,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,   670,     0,     0,   671,     0,     0,     0,   121,   122,
     883,     0,     0,     0,     0,     0,     0,     0,     0,  1518,
     124,   672,   125,     0,   673,     0,     0,     0,     0,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,  1519,
     133,   134,   135,     0,     0,     0,   136,   137,  1520,  1521,
    1522,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,   674,     0,     0,     0,     0,     0,
     675,   676,   677,     0,   678,   679,   680,     0,   681,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   682,     0,   683,   670,
       0,     0,   671,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   672,
       0,     0,   673,     0,     0,     0,     0,   684,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   685,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   674,     0,     0,     0,   686,     0,   675,   676,
     677,     0,   678,   679,   680,     0,   681,     0,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,   682,     0,   683,     0,     0,   121,
     122,   687,   688,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,     0,     0,     0,     0,   689,     0,
     126,   127,     0,   128,   129,   684,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,   685,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   686,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
     961,     0,     0,   671,     0,     0,     0,   121,   122,   687,
     688,     0,     0,     0,     0,     0,   962,     0,   123,   124,
     672,   125,     0,   673,     0,     0,   689,     0,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,   670,   136,   137,   671,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,   674,   672,     0,     0,   673,     0,   675,
     676,   677,     0,   678,   679,   680,     0,   681,     0,     0,
       0,  1307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   682,     0,   683,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   674,     0,     0,
       0,     0,     0,   675,   676,   677,     0,   678,   679,   680,
       0,   681,     0,     0,     0,     0,   684,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   682,
       0,   683,     0,   685,     0,     0,     0,     0,     0,     0,
       0,  1308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   686,     0,     0,     0,     0,
     684,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1309,   685,     0,     0,
       0,  1310,     0,     0,     0,     0,     0,  1311,     0,     0,
     687,   688,     0,     0,     0,     0,     0,     0,   114,   686,
       0,   115,     0,   116,   117,   118,  1312,   689,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   123,   124,     0,
     125,     0,     0,     0,   687,   688,     0,   126,   127,     0,
     128,   129,     0,   130,     0,     0,   131,   132,   133,   134,
     135,   689,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,   114,   144,     0,
     115,   190,   116,   117,   118,   119,   191,   120,   192,   193,
     194,   195,   196,   197,   198,   121,   122,   199,   200,   201,
     202,   203,     0,     0,   204,   205,   123,   124,   206,   125,
       0,     0,   207,   208,   209,   210,   126,   127,   211,   128,
     129,   212,   130,   213,   214,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,   215,   216,   217,   218,   138,
     139,   140,   219,   141,   142,   143,   220,   144
};

static const short yycheck[] =
{
       9,     0,     4,   172,   113,   400,    60,    16,    70,    18,
      69,   605,   784,   162,   163,    62,    79,   162,   163,    76,
      98,    16,   101,    74,   431,   435,   784,   435,    75,   402,
      92,  1041,   574,   497,   847,    69,   610,   309,   640,   102,
     532,   427,    96,   100,   497,  1304,  1065,    98,    57,    58,
      68,    60,    61,    62,    63,    64,  1083,  1303,    64,   552,
      69,    70,    71,  1129,    73,    61,    75,    76,   553,    78,
      79,    80,  1377,   412,  1002,   324,   802,    73,   724,    88,
     754,    90,    91,    92,  1129,    94,    95,    96,    97,    95,
      99,   100,   101,   102,   103,    90,   105,   103,   177,   563,
     795,    97,   441,  1137,    99,   779,   955,  1193,   155,   105,
    1038,  1129,   174,  1171,   656,   351,   382,  1129,   411,   717,
     154,   260,   156,  1129,  1322,   181,  1281,    10,     8,   176,
      21,   768,  1287,  1526,  1369,  1370,     8,   774,   630,  1374,
       8,   588,   634,     8,   892,   154,   155,   156,   640,     8,
      43,   160,  1375,  1342,  1188,   850,     6,  1070,   167,    43,
     269,   167,    61,   172,   857,   174,   175,   176,   177,   133,
       8,    47,   181,    90,     8,   853,    43,  1134,   731,   175,
      23,    26,   351,    44,  1141,    28,  1584,    44,    97,    19,
       8,    47,     9,  1150,     8,    44,    46,    10,    61,   877,
     169,     8,   880,    26,   192,     8,   149,   225,  1129,   616,
      23,   889,  1643,     8,    56,    28,    63,   141,   357,  1176,
      22,    14,    63,    90,    55,    62,    50,    51,    23,   731,
     640,    80,   640,    28,   736,   143,     8,   255,    55,   230,
       4,    30,    11,     7,   174,   125,   142,   642,    46,    66,
      63,    23,   361,   125,    43,   594,    28,   125,   277,   666,
     125,    80,   158,   282,    10,   299,   125,  1526,   302,  1041,
     948,   262,   736,   545,  1520,  1521,    45,    23,   123,   190,
     193,   335,    28,  1041,    64,    49,   194,   125,   560,   277,
     299,   125,   339,   302,    19,   690,    94,    77,    50,    51,
     309,   310,   139,  1369,  1370,    85,  1333,   125,  1374,   278,
     141,   125,   105,   155,   278,  1349,   118,   351,   125,   180,
     100,   230,   125,   180,  1369,  1370,   335,   174,   191,  1374,
     339,   180,   177,   786,   114,   278,   260,   355,   255,  1494,
    1495,   794,   179,   980,   981,   176,  1744,   277,   165,   194,
    1408,  1369,  1370,  1372,   177,   173,  1374,  1369,  1370,   173,
       4,   180,  1374,  1369,  1370,   189,   277,   401,  1374,    94,
     141,   151,   381,   382,   204,   437,   269,   411,   277,   413,
     827,   415,   439,   552,   418,   269,  1609,  1576,   255,   931,
     281,    11,   401,  1030,   277,   449,  1415,  1145,  1146,   279,
     447,   277,   411,    23,   413,    49,   415,   279,    28,   418,
     428,   279,   430,   905,   279,  1328,  1329,    23,   277,   972,
    1348,   277,    28,  1116,   274,    45,   185,   445,   437,   438,
     439,   274,   275,  1111,  1108,  1392,   277,   189,   447,  1870,
     449,   279,   438,   452,   126,   279,   196,  1195,  1369,  1370,
     452,    23,    68,  1374,  1132,  1133,    28,    90,    43,   272,
     273,   274,   275,  1768,   277,  1323,   968,  1713,   196,  1597,
     972,   956,     3,   868,  1152,   246,   871,   272,   273,   274,
     275,   196,   967,   263,     5,    44,   971,     6,    23,   498,
     196,  1128,   887,    28,    15,   542,   178,    63,  1891,   117,
     272,   273,   274,   275,   551,   231,   562,    94,   555,    94,
     196,     3,   156,   658,  1642,   241,   196,   912,   552,   278,
     196,    80,  1915,    22,  1296,    44,   272,   273,   274,   275,
     174,    30,   196,   542,   284,   544,   545,    56,  1296,   811,
    1398,  1696,   551,    18,  1742,    76,   555,   223,   544,  1942,
     278,   560,   168,   562,  1640,  1201,   284,  1862,  1585,   175,
    1953,    80,   277,  1956,   208,  1414,   199,   200,   186,   708,
    1963,   605,   278,  1592,  1300,   824,  1213,  1214,   284,  1216,
    1217,   659,  1075,   601,    76,   878,   959,   273,  1622,  1187,
    1825,  1826,   858,   273,  1652,   196,   605,   273,  1083,  1904,
     618,  1195,  1297,    46,   284,  1639,   640,   946,   659,   273,
     846,  1896,   199,   200,   632,   784,   277,    98,   279,   118,
    1925,    44,   223,    37,  1077,   277,   145,   279,  1113,   638,
      84,   640,  1891,    17,    48,   644,   155,    85,   122,   117,
     139,    55,    18,   661,  1601,    59,   145,    31,  1605,  1606,
     799,    94,   272,   273,   274,   275,  1915,    80,  1943,     6,
     108,   180,    21,   117,   244,    79,   272,   273,   274,   275,
      61,    23,   273,   279,    23,   118,    28,   846,   156,    28,
     179,   227,   716,  1942,  1641,    23,   167,   832,   706,   143,
      28,    75,   146,    23,  1953,   713,   714,  1956,    28,    46,
     272,   273,   274,   275,  1963,   277,    82,   716,   162,  1955,
      23,    64,   199,   200,     6,    28,   164,    79,    23,   103,
    1730,  1358,   145,    28,    77,   203,   277,   141,    87,   276,
    1658,   282,   155,   279,   188,  1413,  1122,   272,   273,   274,
     275,   287,   277,  1380,  1139,  1140,   155,   100,     6,    55,
     759,   147,   114,   759,    46,     7,  1151,   180,   172,  1825,
    1826,  1780,   176,  1227,   126,   196,   800,  1786,   802,   914,
     804,  1445,   819,   182,  1227,     6,   325,  1590,   787,   196,
    1825,  1826,     6,   167,   333,    65,   795,     6,    46,    59,
      60,   800,   223,   802,   841,   804,   843,  1754,   151,   196,
      23,  1758,   811,  1189,  1378,    28,   223,  1825,  1826,   196,
     819,   820,   846,  1825,  1826,    46,  1773,   826,   852,  1825,
    1826,   830,    46,    65,   820,  1198,    23,    46,   906,   143,
      65,    28,   841,  1790,   843,   141,   278,    23,  1291,   857,
     118,   850,    28,   852,   878,    23,   864,   277,   162,   858,
      28,   265,   282,    23,   387,   906,   389,   875,    28,  1401,
     911,   139,   940,   941,  1406,   943,   172,   145,   140,   878,
     904,   905,  1041,   277,  1884,   279,  1886,   928,  1480,   278,
     280,   187,    98,   283,  1894,   203,   122,   205,  1341,   940,
     941,   277,   943,    23,  1267,   904,   905,   277,    28,   279,
     947,   179,   920,   124,  1825,  1826,  1075,   277,   174,   279,
     957,   122,  1869,   922,   124,  1872,   132,   277,   134,   279,
     922,   231,   232,   272,   273,   274,   275,    23,   232,  1939,
     279,   277,    28,   279,   272,   273,   274,   275,   947,    79,
     156,   279,   272,   273,   274,   275,   130,   131,   957,   279,
      23,  1908,  1589,  1562,  1563,    28,   965,   966,  1730,   272,
     273,   274,   275,   965,   156,   181,   279,   272,   273,   274,
     275,    23,  1730,     4,   279,   277,    28,   279,  1442,  1443,
     180,   277,    17,   279,   192,    16,  1450,  1451,    23,  1453,
    1454,    19,   277,    28,   279,  1390,    31,  1461,  1462,  1463,
    1464,   278,  1466,  1467,  1468,  1469,  1470,  1471,   277,   259,
     279,  1418,    23,  1505,   195,   196,   197,    28,    49,  1336,
      51,  1338,   277,    23,   279,  1420,   198,  1036,    28,  1174,
    1036,    23,    67,  1562,  1563,  1307,    28,   277,  1310,   279,
      75,  1075,   223,   224,   277,   277,   279,   279,    83,   272,
     273,   274,   275,   266,  1911,  1912,   279,    23,   162,    61,
    1069,  1070,    28,   277,   199,   279,   101,    10,   103,  1706,
    1480,   277,  1480,   279,  1083,   272,   273,   274,   275,    59,
      79,   116,   279,   118,   119,   120,   272,   273,   274,   275,
     277,    78,   279,   279,   272,   273,   274,   275,  1116,   190,
     149,   279,   272,   273,   274,   275,  1184,   172,   277,   279,
     279,    23,  1884,    82,  1886,   141,    28,   278,    63,  1197,
      61,    61,  1894,   244,   122,  1203,  1884,  1296,  1886,   122,
     174,   102,   167,  1184,   188,  1304,  1894,   123,   172,   156,
      65,    83,   272,   273,   274,   275,  1197,   121,   243,   279,
     155,   254,  1203,    23,  1163,    64,   255,  1191,    28,    30,
     254,  1195,    52,   174,   207,   277,  1168,  1939,    77,   192,
      23,  1180,  1321,  1322,  1323,    28,   272,   273,   274,   275,
      23,  1939,  1191,   279,   269,    28,  1195,    29,   755,   112,
     757,   100,   278,    63,   761,   762,   763,   135,    10,   272,
     273,   274,   275,   278,   278,   114,   279,  1844,    61,  1354,
      23,  1356,   278,   122,  1851,    28,  1611,   199,   278,   278,
     272,   273,   274,   275,    23,   278,   199,   279,   278,    28,
     278,   278,  1311,   278,   269,   270,   271,   272,   273,   274,
     275,    54,   151,   199,  1303,   278,   281,    97,   283,  1312,
       0,  1646,  1309,   278,  1263,    54,   278,   278,   277,   148,
      10,   272,   273,   274,   275,   281,  1300,    10,  1263,  1303,
    1304,  1416,   272,   273,   274,   275,   283,   278,   269,   279,
     272,   273,   274,   275,   278,   141,   283,   279,  1297,    63,
     166,  1300,   277,   277,  1303,  1304,    10,   269,  1307,  1308,
    1309,  1310,  1311,  1312,   283,  1899,   272,   273,   274,   275,
     278,    61,  1308,   279,    64,    83,   283,  1442,  1443,  1328,
    1329,    71,   277,    73,  1333,  1450,  1451,    77,  1453,  1454,
     239,    90,    82,    90,    90,    85,  1461,  1462,  1463,  1464,
      22,  1466,  1467,  1468,  1469,  1470,  1471,   298,    22,    43,
     100,     7,    10,   111,   263,   174,   208,  1526,   136,   174,
     272,   273,   274,   275,   114,   277,   136,   196,    23,    25,
     321,    21,   122,    28,    90,    23,   126,    33,   185,   330,
      28,   185,   278,   138,    40,   135,    42,    91,    44,   281,
     278,   278,  1787,    49,  1396,   278,    61,    10,   122,   277,
      56,   151,   272,   273,   274,   275,   128,   149,   121,   278,
     278,  1429,   189,   279,    70,   126,  1434,    61,  1587,   272,
     273,   274,   275,    21,    80,   284,    42,    82,   178,   272,
     273,   274,   275,    64,    82,   386,   174,   174,   202,   390,
     190,     6,   192,   987,   278,   989,    77,   991,   992,  1511,
     994,   995,   996,    60,   277,   190,   186,    10,    83,   272,
     273,   274,   275,   277,   240,    19,    43,  1476,   278,   100,
     278,    27,    86,   272,   273,   274,   275,    86,    22,    44,
     125,  1476,   125,   114,    17,   125,   156,   143,   154,   239,
      23,    56,  1526,   156,   150,    28,   278,   153,    31,   155,
     156,    98,  1511,   278,   108,    70,   278,   278,   277,   259,
    1519,  1520,  1521,   263,   279,    80,    63,  1526,    23,    16,
     151,   283,   184,   277,   180,   124,   276,   277,   107,   279,
     277,    63,    63,   278,    67,    71,    12,   278,   489,   239,
      48,    10,    75,   160,   495,   496,   497,    55,   279,   278,
      83,    59,    60,  1562,  1563,   280,   279,    10,   279,   279,
      68,  1730,   192,   133,    52,  1612,    52,   279,   101,   279,
     103,    79,     8,   229,  1743,  1584,  1585,   278,  1587,   278,
     145,    63,   278,   116,    51,   118,   119,   120,   244,   245,
     155,   278,   278,  1742,    17,   278,   192,   278,   278,   278,
      23,   278,   278,  1612,   173,    28,   278,   278,    31,   278,
     278,   267,   563,   127,   278,   180,   567,   272,   273,   274,
     275,    85,   279,   277,   272,   273,   274,   275,   277,    85,
    1684,    19,   263,   141,   167,    73,   130,   202,   279,   145,
     125,   125,   125,   279,    67,   125,   125,   125,   196,   157,
     279,    22,    75,   279,   278,    63,   279,   279,   279,   277,
      83,    51,  1771,   173,   172,   276,    36,   279,   176,    36,
     276,    82,  1681,   278,   276,  1684,    37,    51,   101,   187,
     103,   276,   276,   276,   276,   276,  1681,    48,   276,   276,
     276,   276,   276,   116,    55,   118,   119,   120,   206,    82,
     276,   276,   276,   276,  1713,   276,    70,    68,   280,   174,
      10,    23,    85,    79,   180,  1884,    44,  1886,  1772,   180,
    1767,    80,  1891,    80,    44,  1894,  1725,   279,   279,    94,
     124,   111,   111,   279,  1743,  1744,   269,   270,   271,   272,
     273,   274,   275,   278,   167,   279,  1915,    61,   281,   279,
     283,   279,   279,  1762,  1863,   279,   279,   265,  1767,   279,
     279,   279,   279,  1772,   125,   189,   279,   279,   279,   278,
    1939,   124,   279,  1942,  1836,  1793,   279,   279,    38,  1797,
     141,   277,    86,    94,  1953,   736,   279,  1956,   204,    79,
      82,   278,   246,    82,  1963,   122,   157,   210,   211,   212,
     213,   214,   215,   216,   217,   756,    66,   278,   278,   760,
    1819,   172,   122,   173,   277,   176,   767,   279,   179,   278,
     122,   772,   773,    47,   279,    44,   187,  1836,  1837,   279,
     180,   279,    50,    36,    94,   786,  1838,   788,   789,   790,
     791,   792,   793,   794,     0,   206,   269,   270,   271,   272,
     273,   274,   275,    47,   201,     0,   558,  1891,   281,  1319,
     283,   159,   159,   307,   160,  1899,   820,   310,   560,  1201,
     960,   720,   811,  1198,  1190,  1630,   965,   828,   935,    80,
     666,  1915,  1891,   398,  1883,   359,  1125,  1896,   852,   432,
    1899,  1191,   413,   888,  1195,  1629,  1609,   413,   849,  1611,
    1905,  1902,  1911,  1912,   265,   165,  1915,  1927,  1942,  1906,
     861,  1066,   863,   878,  1892,     0,  1717,     6,   800,  1953,
       9,   181,  1956,  1593,    13,  1351,   403,   646,   418,  1963,
     938,    20,  1404,  1942,  1943,   195,   196,   197,   396,   199,
     200,   930,   703,    32,  1953,    13,  1955,  1956,   343,    38,
      39,   165,   580,   828,  1963,   841,   576,  1083,   218,   219,
     220,   221,   222,   223,   224,   225,   226,    90,  1337,  1856,
     753,  1681,  1476,   233,   234,     0,   861,    66,     0,     0,
      69,   818,   242,  1242,  1180,   588,  1078,  1360,   922,  1762,
    1289,   644,    94,  1519,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    94,    23,    96,    97,    -1,
      -1,    28,    -1,   273,    31,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   982,   983,   984,    -1,    -1,   987,   988,   989,    -1,
     991,   992,   993,   994,   995,   996,    -1,    -1,    -1,    -1,
      67,    -1,  1003,  1004,    -1,  1006,    -1,  1008,    75,    -1,
    1011,  1012,  1013,  1014,  1015,  1016,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,  1028,    -1,    -1,
    1031,    -1,   171,    -1,   101,    -1,   103,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,   119,   120,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,  1077,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
     167,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,   273,   274,   275,    -1,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,  1227,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1240,
    1241,    -1,  1243,  1244,    -1,    -1,    -1,    17,    -1,    -1,
    1251,    -1,    94,    23,    96,    97,    -1,    -1,    28,    -1,
      -1,    31,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
    1271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1279,  1280,
    1281,    -1,    -1,    -1,    -1,    -1,  1287,    -1,  1289,  1290,
    1291,  1292,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   181,
    1341,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,   119,
     120,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,  1373,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,   167,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    21,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
    1441,    -1,   284,    37,    -1,    -1,  1447,    -1,    -1,    -1,
      -1,  1452,    -1,    -1,    48,    -1,    -1,    -1,  1459,    -1,
      -1,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1494,  1495,    -1,    -1,    -1,    -1,   269,
     270,   271,   272,   273,   274,   275,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1516,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   141,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,   172,    69,
      -1,    -1,   176,    -1,    -1,    -1,    -1,  1588,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,   206,   207,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1661,    27,    -1,    -1,    -1,    -1,  1667,  1668,    -1,    -1,
      -1,   265,    -1,   163,   268,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1691,   181,    -1,    -1,    -1,  1696,    -1,    -1,  1699,    -1,
      -1,    -1,  1703,  1704,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,
     260,   261,   262,    13,   264,   265,   266,    -1,   268,    -1,
      20,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,  1792,    32,    -1,   284,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,  1856,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,  1893,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,
     260,   261,   262,    13,   264,   265,   266,    -1,   268,    -1,
      20,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    32,    -1,   284,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,
     260,   261,   262,    13,   264,   265,   266,    -1,   268,    -1,
      20,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    32,    -1,   284,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    39,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,
     260,   261,   262,    13,   264,   265,   266,    -1,   268,    -1,
      20,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    32,    -1,   284,    -1,    -1,    -1,    38,    39,
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
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,
     260,   261,   262,    13,   264,   265,   266,    -1,   268,    -1,
      20,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,   155,
     280,    -1,    32,    -1,   284,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,   108,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,   164,   268,    -1,
      -1,    -1,   272,   273,    -1,    -1,    57,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
     227,   228,    -1,   230,    -1,    -1,    -1,   118,    -1,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    13,   264,   265,   266,    -1,   268,    -1,    20,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    32,    -1,   284,    35,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,   272,   273,    -1,    -1,    57,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,   118,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,    -1,   280,    -1,    -1,    -1,    -1,   159,    -1,    -1,
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
     272,   273,    -1,    -1,    57,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,   118,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   280,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
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
     273,    -1,    -1,    57,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
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
     264,   265,   266,    -1,   268,    -1,    -1,    -1,   272,   273,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,
     284,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     265,   266,    -1,   268,    -1,    -1,    -1,   272,   273,   274,
      -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
      66,    -1,    -1,    69,   196,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     266,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
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
      -1,   268,    -1,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
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
     268,    -1,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,
     278,   279,   280,    -1,    -1,    -1,   284,    66,    -1,    -1,
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
      -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,
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
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,
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
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,   145,   254,   255,    -1,    16,    -1,    -1,   260,   261,
     262,   155,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
     272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    44,    -1,    -1,   180,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    74,   208,    -1,    -1,    -1,    -1,    80,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    16,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,   155,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    80,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,   137,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,   256,   257,   258,    16,   260,
     261,   262,   155,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,
      58,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    74,    -1,    -1,    -1,
      -1,    -1,    80,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    18,    -1,    -1,    21,    -1,    -1,    -1,   216,   217,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,
     228,    38,   230,    -1,    41,    -1,    -1,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,   256,   257,
     258,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    18,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    41,    -1,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,   183,    -1,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,   113,    -1,   115,    -1,    -1,   216,
     217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
     237,   238,    -1,   240,   241,   144,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,   161,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      18,    -1,    -1,    21,    -1,    -1,    -1,   216,   217,   218,
     219,    -1,    -1,    -1,    -1,    -1,    34,    -1,   227,   228,
      38,   230,    -1,    41,    -1,    -1,   235,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    18,   254,   255,    21,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    81,    38,    -1,    -1,    41,    -1,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,   115,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,   161,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,   198,   183,
      -1,   201,    -1,   203,   204,   205,   206,   235,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,   218,   219,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,   235,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,   198,   268,    -1,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,    -1,    -1,   225,   226,   227,   228,   229,   230,
      -1,    -1,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268
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
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 484:
{ yyval = yyvsp[0]; }
    break;
case 485:
{ yyval = NULL; }
    break;
case 486:
{ yyval = yyvsp[0]; }
    break;
case 487:
{ yyval = NULL; }
    break;
case 488:
{ yyval = NULL; }
    break;
case 490:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 491:
{ yyval = yyvsp[0]; }
    break;
case 492:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 494:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 495:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 496:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 498:
{ yyval = NULL; }
    break;
case 500:
{ yyval = NULL; }
    break;
case 501:
{ yyval = yyvsp[0]; }
    break;
case 502:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 503:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 505:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 506:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 507:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 508:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 509:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 510:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 511:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 512:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 513:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 514:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 519:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 520:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 522:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 523:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 524:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 530:
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
case 531:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 533:
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
case 534:
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
case 535:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 542:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 543:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 544:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 545:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 546:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 547:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 553:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 563:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 564:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 565:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 566:
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
case 567:
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
case 570:
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
case 571:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 572:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 573:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 574:
{ yyval = yyvsp[-1]; }
    break;
case 575:
{ yyval = 0; }
    break;
case 579:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 580:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 581:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 585:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 586:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 587:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 588:
{ yyval = 0; }
    break;
case 589:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 592:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 593:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 596:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 597:
{ yyval = NULL; }
    break;
case 599:
{ yyval = NULL; }
    break;
case 600:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 602:
{ yyval = NULL; }
    break;
case 604:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 609:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 610:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 611:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 612:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 613:
{ yyval = yyvsp[0];}
    break;
case 615:
{ yyval = yyvsp[0];}
    break;
case 616:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 617:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 618:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 619:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 620:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 621:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 622:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 623:
{ yyval = 0; }
    break;
case 624:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 625:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 626:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 627:
{ yyval = (dsql_nod*) 0; }
    break;
case 628:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 629:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 631:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 632:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 633:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 634:
{ yyval = 0; }
    break;
case 636:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 637:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 638:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;
case 639:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 640:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;
case 641:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;
case 642:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;
case 643:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 644:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;
case 645:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 646:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;
case 647:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;
case 648:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;
case 649:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;
case 650:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 651:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 652:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;
case 653:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;
case 654:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;
case 655:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;
case 656:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 657:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 658:
{ yyval = yyvsp[0]; }
    break;
case 660:
{ yyval = NULL; }
    break;
case 661:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 662:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 663:
{ yyval = NULL; }
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
{ yyval = NULL; }
    break;
case 668:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 669:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 670:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 672:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 673:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 675:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 676:
{ lex.limit_clause = true; }
    break;
case 677:
{ lex.limit_clause = false; }
    break;
case 678:
{ lex.first_detection = true; }
    break;
case 679:
{ lex.first_detection = false; }
    break;
case 680:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 681:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 682:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 683:
{ yyval = 0; }
    break;
case 684:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 685:
{ yyval = yyvsp[-2]; }
    break;
case 686:
{ yyval = yyvsp[-1]; }
    break;
case 687:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 688:
{ yyval = yyvsp[-1]; }
    break;
case 689:
{ yyval = yyvsp[0]; }
    break;
case 690:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 691:
{ yyval = 0; }
    break;
case 692:
{ yyval = make_list (yyvsp[0]); }
    break;
case 693:
{ yyval = 0; }
    break;
case 695:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 697:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 700:
{ yyval = make_list (yyvsp[0]); }
    break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 707:
{ yyval = yyvsp[-1]; }
    break;
case 708:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 710:
{ yyval = NULL; }
    break;
case 711:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 712:
{ yyval = NULL; }
    break;
case 714:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 718:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 719:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 720:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 723:
{ yyval = yyvsp[0]; }
    break;
case 724:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 725:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 726:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 727:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 728:
{ yyval = NULL; }
    break;
case 730:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 731:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 732:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 733:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 734:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 735:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 736:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 739:
{ yyval = make_list (yyvsp[0]); }
    break;
case 740:
{ yyval = NULL; }
    break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 744:
{ yyval = yyvsp[0]; }
    break;
case 745:
{ yyval = NULL; }
    break;
case 746:
{ yyval = yyvsp[0]; }
    break;
case 747:
{ yyval = NULL; }
    break;
case 748:
{ yyval = yyvsp[0]; }
    break;
case 749:
{ yyval = NULL; }
    break;
case 750:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 751:
{ yyval = 0; }
    break;
case 752:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 753:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 754:
{ yyval = 0; }
    break;
case 755:
{ yyval = 0; }
    break;
case 757:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 758:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 763:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 764:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 768:
{ yyval = 0; }
    break;
case 769:
{ yyval = make_list (yyvsp[0]); }
    break;
case 770:
{ yyval = 0; }
    break;
case 772:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 774:
{ yyval = 0; }
    break;
case 775:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 776:
{ yyval = 0; }
    break;
case 777:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 778:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 779:
{ yyval = yyvsp[-1]; }
    break;
case 780:
{ yyval = 0; }
    break;
case 781:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 782:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 783:
{ yyval = NULL; }
    break;
case 784:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 785:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 789:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 793:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 796:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 798:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 799:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 802:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 803:
{ yyval = NULL; }
    break;
case 806:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 807:
{ yyval = yyvsp[0]; }
    break;
case 808:
{ yyval = NULL; }
    break;
case 809:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
    break;
case 818:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 820:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 824:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 833:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 834:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 835:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 836:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 837:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 839:
{ yyval = yyvsp[-1]; }
    break;
case 840:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 854:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 856:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 857:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 858:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 859:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 860:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 861:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 862:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 863:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 864:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 865:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 866:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 867:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 868:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 869:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 870:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 871:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 872:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 873:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 874:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 875:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 878:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 879:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 880:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 881:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 882:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 883:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 884:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 885:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 886:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 887:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 888:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 889:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 890:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 891:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 892:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 893:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 894:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 895:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 896:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 898:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 899:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 900:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 901:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 902:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 903:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 905:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 906:
{ yyval = yyvsp[-1]; }
    break;
case 917:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 918:
{ yyval = yyvsp[0]; }
    break;
case 919:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 920:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 921:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 922:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 923:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 924:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 925:
{ yyval = yyvsp[-1]; }
    break;
case 926:
{ yyval = yyvsp[-1]; }
    break;
case 930:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 931:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 932:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 935:
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
case 936:
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
case 937:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 938:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 940:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 942:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 943:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 944:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 945:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 946:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 947:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 949:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 950:
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
case 951:
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
case 952:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 953:
{ yyval = make_parameter (); }
    break;
case 954:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 955:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 956:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 957:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 958:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 959:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 960:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 961:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 962:
{ yyval = yyvsp[0]; }
    break;
case 963:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 965:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 966:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 967:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 968:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 969:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 971:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 972:
{ yyval = yyvsp[0];}
    break;
case 976:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 977:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 978:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 979:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 980:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 981:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 982:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 983:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 984:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 985:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 986:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 988:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 990:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 991:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 992:
{ yyval = yyvsp[0]; }
    break;
case 993:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 994:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 995:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 996:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 999:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 1000:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 1001:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1004:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1005:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1006:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1007:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1008:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1009:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1010:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1011:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1015:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 1016:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1018:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1019:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1020:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1021:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1022:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1023:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1024:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1029:
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

