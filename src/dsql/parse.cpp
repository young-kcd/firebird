/* A Bison parser, made from ../src/dsql/parse.y
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

const int MIN_CACHE_BUFFERS	= 250;
const int DEF_CACHE_BUFFERS	= 1000;

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
	bool log_defined, cache_defined;
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



#define	YYFINAL		1897
#define	YYFLAG		-32768
#define	YYNTBASE	285

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 522 ? yytranslate[x] : 712)

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
     514,   518,   519,   521,   523,   526,   528,   533,   537,   541,
     543,   544,   546,   548,   551,   555,   560,   561,   563,   566,
     567,   569,   571,   577,   583,   587,   590,   591,   593,   597,
     599,   601,   608,   612,   615,   622,   625,   627,   630,   633,
     636,   637,   639,   641,   644,   645,   648,   649,   651,   653,
     655,   657,   659,   661,   662,   664,   666,   669,   672,   675,
     681,   683,   686,   690,   693,   696,   697,   699,   701,   703,
     705,   709,   714,   723,   728,   729,   736,   738,   740,   743,
     746,   747,   751,   755,   757,   760,   763,   766,   775,   784,
     793,   802,   806,   807,   812,   813,   815,   819,   824,   826,
     830,   833,   837,   841,   843,   845,   846,   848,   851,   856,
     858,   860,   864,   866,   867,   870,   872,   879,   881,   883,
     887,   889,   892,   893,   895,   898,   901,   904,   906,   908,
     910,   912,   914,   916,   919,   921,   923,   926,   928,   930,
     932,   935,   938,   940,   943,   947,   952,   955,   962,   967,
     976,   986,   993,  1002,  1009,  1014,  1015,  1020,  1023,  1025,
    1029,  1030,  1033,  1038,  1039,  1041,  1043,  1047,  1051,  1059,
    1062,  1063,  1066,  1069,  1073,  1077,  1078,  1080,  1083,  1088,
    1090,  1094,  1097,  1100,  1103,  1105,  1107,  1109,  1111,  1114,
    1117,  1123,  1124,  1129,  1137,  1141,  1142,  1144,  1148,  1152,
    1160,  1168,  1170,  1172,  1176,  1181,  1189,  1192,  1194,  1198,
    1200,  1202,  1209,  1213,  1214,  1215,  1216,  1217,  1221,  1222,
    1232,  1242,  1244,  1246,  1247,  1250,  1252,  1254,  1256,  1258,
    1260,  1264,  1268,  1272,  1276,  1280,  1284,  1290,  1296,  1302,
    1308,  1314,  1320,  1323,  1324,  1329,  1332,  1336,  1340,  1343,
    1346,  1350,  1354,  1357,  1361,  1368,  1370,  1373,  1378,  1382,
    1385,  1388,  1391,  1394,  1398,  1400,  1404,  1408,  1412,  1415,
    1418,  1423,  1428,  1434,  1436,  1438,  1440,  1442,  1444,  1446,
    1448,  1450,  1452,  1454,  1456,  1458,  1460,  1462,  1464,  1466,
    1468,  1470,  1472,  1474,  1476,  1478,  1480,  1482,  1484,  1486,
    1488,  1490,  1492,  1495,  1498,  1501,  1503,  1505,  1507,  1508,
    1511,  1514,  1515,  1517,  1520,  1523,  1528,  1532,  1535,  1538,
    1546,  1548,  1549,  1551,  1552,  1555,  1558,  1561,  1564,  1567,
    1570,  1573,  1576,  1579,  1583,  1586,  1589,  1592,  1594,  1596,
    1598,  1600,  1605,  1611,  1613,  1617,  1619,  1623,  1625,  1628,
    1630,  1632,  1634,  1636,  1638,  1640,  1642,  1644,  1646,  1648,
    1650,  1655,  1660,  1667,  1673,  1677,  1678,  1681,  1684,  1685,
    1689,  1690,  1695,  1697,  1703,  1708,  1710,  1715,  1717,  1720,
    1723,  1725,  1727,  1729,  1732,  1735,  1738,  1741,  1742,  1746,
    1752,  1754,  1756,  1759,  1763,  1765,  1768,  1772,  1773,  1775,
    1777,  1779,  1785,  1791,  1798,  1800,  1802,  1804,  1807,  1812,
    1814,  1815,  1821,  1823,  1824,  1828,  1831,  1833,  1834,  1837,
    1838,  1840,  1841,  1845,  1847,  1848,  1850,  1853,  1855,  1857,
    1859,  1861,  1864,  1867,  1869,  1872,  1876,  1878,  1880,  1884,
    1888,  1890,  1893,  1897,  1899,  1902,  1903,  1906,  1908,  1910,
    1911,  1913,  1915,  1917,  1921,  1924,  1928,  1929,  1931,  1935,
    1940,  1946,  1948,  1952,  1957,  1961,  1962,  1964,  1968,  1972,
    1974,  1976,  1977,  1979,  1981,  1986,  1987,  1990,  1995,  1996,
    2000,  2001,  2004,  2005,  2008,  2009,  2019,  2031,  2032,  2033,
    2034,  2035,  2039,  2042,  2044,  2045,  2049,  2055,  2059,  2062,
    2068,  2071,  2073,  2075,  2077,  2079,  2081,  2085,  2087,  2091,
    2093,  2094,  2097,  2099,  2103,  2105,  2107,  2109,  2116,  2118,
    2119,  2123,  2124,  2126,  2130,  2137,  2141,  2146,  2149,  2153,
    2154,  2156,  2159,  2161,  2163,  2165,  2168,  2170,  2173,  2175,
    2178,  2179,  2183,  2184,  2186,  2190,  2192,  2195,  2196,  2199,
    2200,  2203,  2204,  2209,  2211,  2214,  2216,  2218,  2219,  2221,
    2225,  2228,  2230,  2232,  2235,  2237,  2242,  2246,  2248,  2252,
    2257,  2258,  2267,  2273,  2275,  2277,  2282,  2287,  2292,  2294,
    2296,  2302,  2308,  2310,  2314,  2318,  2320,  2322,  2324,  2328,
    2336,  2344,  2350,  2354,  2355,  2357,  2359,  2361,  2364,  2365,
    2367,  2369,  2371,  2372,  2376,  2378,  2382,  2384,  2385,  2389,
    2391,  2395,  2397,  2401,  2405,  2407,  2409,  2413,  2415,  2418,
    2420,  2424,  2428,  2430,  2433,  2437,  2441,  2445,  2449,  2451,
    2455,  2458,  2460,  2462,  2464,  2466,  2468,  2470,  2472,  2474,
    2476,  2478,  2482,  2486,  2490,  2494,  2498,  2502,  2506,  2510,
    2517,  2524,  2531,  2538,  2545,  2552,  2559,  2566,  2573,  2580,
    2587,  2594,  2601,  2608,  2615,  2622,  2624,  2626,  2632,  2639,
    2643,  2648,  2654,  2661,  2665,  2670,  2674,  2679,  2683,  2688,
    2693,  2699,  2704,  2709,  2713,  2718,  2720,  2722,  2724,  2726,
    2728,  2730,  2732,  2736,  2740,  2752,  2764,  2766,  2768,  2770,
    2772,  2774,  2776,  2778,  2780,  2782,  2785,  2788,  2792,  2796,
    2800,  2804,  2808,  2812,  2816,  2820,  2822,  2824,  2826,  2828,
    2832,  2834,  2836,  2838,  2840,  2842,  2847,  2849,  2853,  2855,
    2858,  2860,  2862,  2864,  2866,  2868,  2870,  2872,  2875,  2878,
    2881,  2883,  2885,  2887,  2889,  2891,  2893,  2895,  2897,  2899,
    2901,  2904,  2906,  2909,  2911,  2913,  2915,  2917,  2919,  2922,
    2924,  2926,  2928,  2930,  2932,  2937,  2943,  2949,  2955,  2961,
    2967,  2973,  2979,  2985,  2991,  2997,  3004,  3006,  3013,  3015,
    3020,  3028,  3031,  3032,  3037,  3041,  3048,  3050,  3052,  3059,
    3066,  3068,  3070,  3075,  3082,  3087,  3093,  3097,  3103,  3108,
    3114,  3116,  3118,  3120,  3122,  3124,  3126,  3128,  3130,  3132,
    3134,  3136,  3138,  3139,  3141,  3143,  3145,  3147,  3149,  3151,
    3153,  3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,
    3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,
    3193,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3215,  3217,  3219,  3221,  3223,  3225
};
static const short yyrhs[] =
{
     286,     0,   286,   276,     0,   468,     0,   609,     0,   521,
       0,   316,     0,   306,     0,   598,     0,   489,     0,   287,
       0,   597,     0,   441,     0,   442,     0,   318,     0,   320,
       0,   296,     0,   522,     0,   515,     0,   544,     0,   513,
       0,   602,     0,    43,   656,     0,    73,   289,   129,   288,
     182,   300,   293,     0,    73,   291,   129,   148,   295,   182,
     300,   293,     0,    73,   289,   129,   288,   182,   298,     0,
      73,   291,   129,   148,   295,   182,   298,     0,    73,   302,
     182,   304,   294,     0,   581,     0,   180,   581,     0,     6,
       0,     6,   147,     0,   290,     0,   292,     0,   290,   277,
     292,     0,    59,     0,   163,     0,    84,     0,    47,     0,
     188,   614,     0,   153,   614,     0,   200,    73,   131,     0,
       0,   200,   210,   131,     0,     0,   701,     0,   158,   297,
     289,   129,   288,    66,   300,     0,   158,   297,   291,   129,
     148,   295,    66,   300,     0,   158,   289,   129,   288,    66,
     300,     0,   158,   291,   129,   148,   295,    66,   300,     0,
     158,   289,   129,   288,    66,   298,     0,   158,   291,   129,
     148,   295,    66,   298,     0,   158,   302,    66,   304,     0,
      73,   131,    64,     0,   299,     0,   298,   277,   299,     0,
     298,   277,   301,     0,   300,   277,   299,     0,   148,   701,
       0,   184,   705,     0,   195,   708,     0,   214,   702,     0,
     301,     0,   300,   277,   301,     0,   706,     0,   189,   706,
       0,    74,   706,     0,   303,     0,   302,   277,   303,     0,
     702,     0,   305,     0,   304,   277,   305,     0,   706,     0,
     189,   706,     0,    45,   307,     0,    63,   315,     0,    62,
      68,   308,     0,   686,   310,   157,   313,    55,   655,   117,
     655,     0,   496,     0,    19,     0,    37,   278,   659,   279,
     502,     0,     0,   311,     0,   278,   311,   279,     0,   312,
       0,   311,   277,   312,     0,   370,   309,     0,   370,   309,
      20,   238,     0,   314,     0,   278,   314,   279,     0,   370,
     309,     0,   370,   309,   212,     0,   370,   309,    20,   101,
       0,   370,   309,    20,   238,     0,   140,   659,     0,   695,
      82,   501,   135,   501,    55,   655,   117,   655,     0,    36,
     317,     0,    58,   694,   655,     0,   322,   549,    83,   698,
     129,   581,   323,     0,   148,   389,     0,   180,   357,     0,
     184,   460,     0,   195,   446,     0,    71,   339,     0,    40,
     341,     0,    51,   330,     0,   165,   324,     0,   214,   340,
       0,   237,   319,     0,   148,   390,     0,   180,   358,     0,
     195,   447,     0,    36,   132,     7,   321,     0,   148,   391,
       0,   184,   461,     0,   187,     0,     0,   616,     0,   615,
       0,   364,   278,   457,   644,   458,   279,     0,   659,   325,
     326,   655,   327,   328,     0,   111,     0,    14,     0,     0,
       0,    32,     0,     0,   102,   342,   662,   356,     0,     0,
     329,     0,   350,     0,   329,   350,     0,   367,   331,   491,
     457,   332,   458,   333,   366,     0,    10,     0,     0,    46,
     457,   372,     0,     0,     0,   334,     0,   335,     0,   334,
     335,     0,   336,     0,   337,     0,   338,     0,   124,    98,
       0,   457,    24,   278,   623,   279,   458,     0,   697,     0,
     702,     0,   343,   344,   347,     0,     0,   269,     0,   655,
       0,     0,   345,     0,   346,     0,   345,   346,     0,   139,
     342,   659,     0,   102,   342,   662,   356,     0,   189,   655,
       0,   141,   655,     0,   164,   118,   655,     0,     0,   348,
       0,   349,     0,   348,   349,     0,   350,     0,    46,    23,
     164,   688,     0,   263,    94,   655,     0,   351,   655,   352,
       0,    94,     0,     0,   353,     0,   354,     0,   353,   354,
       0,   175,   355,   662,     0,   102,   342,   662,   356,     0,
       0,    12,     0,    12,   137,     0,     0,   137,     0,   138,
       0,   581,   359,   278,   360,   279,     0,   581,   359,   278,
     360,   279,     0,    62,    94,   655,     0,    62,   655,     0,
       0,   361,     0,   360,   277,   361,     0,   362,     0,   377,
       0,   367,   365,   371,   458,   373,   366,     0,   367,   492,
     363,     0,   367,   363,     0,   364,   278,   457,   644,   458,
     279,     0,    30,    20,     0,    30,     0,   491,   457,     0,
     621,   455,     0,    26,   689,     0,     0,   621,     0,   621,
       0,   370,   491,     0,     0,    46,   372,     0,     0,   648,
       0,   652,     0,   653,     0,   654,     0,   685,     0,   645,
       0,     0,   374,     0,   375,     0,   374,   375,     0,   378,
     376,     0,   124,    98,     0,   153,   581,   614,   385,   383,
       0,   384,     0,   187,   383,     0,   146,    90,   383,     0,
     378,   379,     0,    33,   691,     0,     0,   380,     0,   381,
       0,   382,     0,   384,     0,   187,   615,   383,     0,   146,
      90,   615,   383,     0,    65,    90,   615,   153,   581,   614,
     385,   383,     0,   247,   549,    83,   698,     0,     0,   457,
      24,   278,   623,   279,   458,     0,   386,     0,   387,     0,
     387,   386,     0,   386,   387,     0,     0,   129,   188,   388,
       0,   129,    47,   388,     0,   211,     0,   164,    46,     0,
     164,    98,     0,   123,   209,     0,   701,   393,   394,    10,
     455,   400,   409,   458,     0,   701,   393,   394,    10,   455,
     400,   409,   458,     0,   701,   393,   394,    10,   455,   400,
     409,   458,     0,   701,   393,   394,    10,   455,   400,   409,
     458,     0,   278,   395,   279,     0,     0,   157,   278,   397,
     279,     0,     0,   396,     0,   395,   277,   396,     0,   368,
     492,   399,   458,     0,   398,     0,   397,   277,   398,     0,
     368,   492,     0,    46,   455,   372,     0,   269,   455,   372,
       0,   455,     0,   401,     0,     0,   402,     0,   401,   402,
       0,    45,   405,   403,   276,     0,   404,     0,   407,     0,
     367,   492,   406,     0,   192,     0,     0,   269,   372,     0,
     371,     0,   692,    39,    64,   278,   557,   279,     0,   412,
       0,   409,     0,    17,   410,    54,     0,   411,     0,   411,
     432,     0,     0,   408,     0,   411,   408,     0,   606,   276,
       0,   598,   276,     0,   413,     0,   414,     0,   415,     0,
     416,     0,   417,     0,   420,     0,   597,   276,     0,   421,
       0,   423,     0,   602,   276,     0,   428,     0,   418,     0,
     419,     0,   178,   276,     0,    61,   276,     0,   430,     0,
     436,   276,     0,    58,   694,   276,     0,    58,   694,   644,
     276,     0,    58,   276,     0,    59,   148,   701,   425,   426,
     276,     0,    59,   254,   644,   276,     0,   429,    64,   544,
      86,   427,   431,    50,   408,     0,   429,    64,    59,   254,
     644,    86,   427,    50,   408,     0,    59,   254,   644,    86,
     427,   276,     0,    78,   278,   623,   279,   181,   408,    53,
     408,     0,    78,   278,   623,   279,   181,   408,     0,   144,
     644,   422,   276,     0,     0,   544,    86,   427,   276,     0,
     280,   707,     0,   608,     0,   278,   608,   279,     0,     0,
     156,   427,     0,   156,   278,   427,   279,     0,     0,   424,
       0,   620,     0,   427,   277,   620,     0,   427,   277,   424,
       0,   429,   199,   278,   623,   279,    50,   408,     0,   700,
     280,     0,     0,   235,   276,     0,   255,   276,     0,   255,
     700,   276,     0,    10,    39,   692,     0,     0,   433,     0,
     432,   433,     0,   197,   434,    50,   408,     0,   435,     0,
     434,   277,   435,     0,   173,   656,     0,    69,   696,     0,
      58,   694,     0,     9,     0,   437,     0,   439,     0,   438,
       0,   264,   692,     0,   265,   692,     0,   266,   440,   692,
      86,   427,     0,     0,    59,   148,   701,   425,     0,    59,
     268,   443,   394,    10,   400,   409,     0,   278,   444,   279,
       0,     0,   445,     0,   444,   277,   445,     0,   398,   269,
     651,     0,   708,   614,    10,   455,   448,   459,   456,     0,
     708,   614,    10,   455,   448,   459,   456,     0,   449,     0,
     450,     0,   449,   186,   450,     0,   449,   186,     6,   450,
       0,   163,   565,   566,   451,   587,   583,   586,     0,    66,
     452,     0,   453,     0,   452,   277,   453,     0,   454,     0,
     580,     0,   453,   582,    89,   453,   129,   623,     0,   278,
     454,   279,     0,     0,     0,     0,     0,   200,    24,   131,
       0,     0,   705,    64,   581,   462,   463,   466,   457,   467,
     458,     0,   705,    64,   581,   462,   463,   466,   457,   467,
     458,     0,     3,     0,    80,     0,     0,   464,   465,     0,
      16,     0,     5,     0,    84,     0,   188,     0,    47,     0,
      84,   132,   188,     0,    84,   132,    47,     0,   188,   132,
      84,     0,   188,   132,    47,     0,    47,   132,    84,     0,
      47,   132,   188,     0,    84,   132,   188,   132,    47,     0,
      84,   132,    47,   132,   188,     0,   188,   132,    84,   132,
      47,     0,   188,   132,    47,   132,    84,     0,    47,   132,
      84,   132,   188,     0,    47,   132,   188,   132,    84,     0,
     143,   657,     0,     0,    10,   457,   400,   409,     0,     7,
     469,     0,    58,   694,   655,     0,   180,   581,   474,     0,
     184,   486,     0,   148,   392,     0,    40,   483,   484,     0,
      51,   476,   472,     0,    83,   482,     0,    46,   457,   372,
       0,    24,   457,   278,   623,   279,   458,     0,   473,     0,
     472,   473,     0,   164,   457,   470,   458,     0,     4,    33,
     471,     0,     4,   338,     0,    52,    46,     0,    52,    33,
       0,   182,   621,     0,   216,   370,   492,     0,   475,     0,
     474,   277,   475,     0,    52,   621,   481,     0,    52,    33,
     691,     0,     4,   362,     0,     4,   377,     0,   478,   621,
     143,   659,     0,   478,   476,   182,   621,     0,   478,   480,
     216,   479,   458,     0,   477,     0,   710,     0,   215,     0,
     216,     0,   217,     0,   218,     0,   219,     0,   220,     0,
     221,     0,   222,     0,   223,     0,   224,     0,   225,     0,
     226,     0,   227,     0,   228,     0,   229,     0,   230,     0,
     233,     0,   234,     0,   241,     0,   242,     0,   250,     0,
     252,     0,   264,     0,   265,     0,   266,     0,   267,     0,
       7,     0,     7,   215,     0,   492,   457,     0,   621,   455,
       0,   621,     0,   213,     0,   211,     0,     0,   698,     3,
       0,   698,    80,     0,     0,   485,     0,   484,   485,     0,
       4,   329,     0,     4,   263,    94,   655,     0,    52,   263,
      94,     0,    17,   262,     0,    54,   262,     0,   705,   462,
     487,   466,   457,   488,   458,     0,   463,     0,     0,   467,
       0,     0,    52,   490,     0,    58,   694,     0,    83,   698,
       0,   148,   701,     0,   180,   704,     0,   184,   705,     0,
     195,   708,     0,    63,   695,     0,    51,   693,     0,    62,
      68,   686,     0,   165,   659,     0,   214,   702,     0,    71,
     697,     0,   492,     0,   493,     0,   496,     0,   499,     0,
     497,   281,   494,   282,     0,   504,   281,   494,   282,   502,
       0,   495,     0,   494,   277,   495,     0,   661,     0,   661,
     280,   661,     0,   497,     0,   504,   502,     0,   503,     0,
     508,     0,   511,     0,   243,     0,   498,     0,   169,     0,
      41,     0,   226,     0,   227,     0,    85,     0,    96,     0,
      19,   501,   500,   502,     0,    19,   278,   660,   279,     0,
      19,   278,   660,   277,   656,   279,     0,    19,   278,   277,
     656,   279,     0,   162,   168,   660,     0,     0,   177,   656,
       0,   177,   687,     0,     0,    23,   164,   688,     0,     0,
     507,   278,   659,   279,     0,   507,     0,   507,   193,   278,
     659,   279,     0,   506,   278,   659,   279,     0,   506,     0,
     505,   278,   659,   279,     0,   191,     0,    23,   193,     0,
      91,   193,     0,    23,     0,    91,     0,   121,     0,   119,
      23,     0,   119,    91,     0,    99,   509,     0,   510,   509,
       0,     0,   278,   661,   279,     0,   278,   661,   277,   661,
     279,     0,    44,     0,    92,     0,    95,   512,     0,    97,
      95,   512,     0,   152,     0,    93,   145,     0,   278,   657,
     279,     0,     0,   526,     0,   514,     0,   543,     0,   164,
      71,   697,   182,   661,     0,   164,    71,   697,   182,   231,
       0,   164,    71,   697,   182,   273,   231,     0,   516,     0,
     517,     0,   519,     0,   252,   709,     0,   253,   252,   709,
     518,     0,   130,     0,     0,   161,   523,   182,   520,   709,
       0,   252,     0,     0,    28,   523,   524,     0,   161,   523,
       0,   201,     0,     0,   155,   525,     0,     0,   170,     0,
       0,   164,   183,   527,     0,   528,     0,     0,   529,     0,
     528,   529,     0,   530,     0,   531,     0,   532,     0,   536,
       0,   151,   130,     0,   151,   202,     0,   196,     0,   123,
     196,     0,    88,   107,   533,     0,   533,     0,   534,     0,
     151,   185,   535,     0,   151,    29,   535,     0,   170,     0,
     170,   180,     0,   170,   180,   174,     0,   194,     0,   123,
     194,     0,     0,   154,   539,     0,   166,     0,   149,     0,
       0,   151,     0,   202,     0,   540,     0,   539,   277,   540,
       0,   542,   541,     0,    64,   537,   538,     0,     0,   581,
       0,   542,   277,   581,     0,   164,   176,    83,   698,     0,
     545,   546,   552,   553,   555,     0,   556,     0,   545,   186,
     556,     0,   545,   186,     6,   556,     0,   133,    20,   547,
       0,     0,   548,     0,   547,   277,   548,     0,   644,   549,
     551,     0,    11,     0,    48,     0,     0,   239,     0,   249,
       0,   248,   560,   550,   561,     0,     0,   267,   644,     0,
     267,   644,   182,   644,     0,     0,    64,   188,   554,     0,
       0,   128,   616,     0,     0,   200,   251,     0,     0,   163,
     562,   565,   566,   570,   587,   583,   586,   588,     0,   163,
     562,   565,   566,   570,   587,   583,   586,   588,   546,   552,
       0,     0,     0,     0,     0,   563,   564,   559,     0,   563,
     559,     0,   564,     0,     0,   239,   662,   558,     0,   239,
     278,   644,   279,   558,     0,   239,   651,   558,     0,   240,
     662,     0,   240,   278,   559,   644,   279,     0,   240,   651,
       0,    49,     0,   684,     0,   567,     0,   274,     0,   568,
       0,   567,   277,   568,     0,   607,     0,   607,   569,   699,
       0,    10,     0,     0,    66,   571,     0,   572,     0,   571,
     277,   572,     0,   577,     0,   578,     0,   573,     0,   278,
     544,   279,   569,   574,   575,     0,   703,     0,     0,   278,
     576,   279,     0,     0,   699,     0,   576,   277,   699,     0,
     572,   582,    89,   572,   129,   623,     0,   278,   577,   279,
       0,   701,   579,   569,   703,     0,   701,   579,     0,   278,
     608,   279,     0,     0,   581,     0,   704,   703,     0,   704,
       0,    81,     0,   105,     0,   105,   134,     0,   159,     0,
     159,   134,     0,    67,     0,    67,   134,     0,     0,    74,
      20,   584,     0,     0,   585,     0,   584,   277,   585,     0,
     644,     0,    77,   623,     0,     0,   198,   623,     0,     0,
     142,   589,     0,     0,   590,   278,   591,   279,     0,    89,
       0,   172,   114,     0,   114,     0,   172,     0,     0,   592,
       0,   592,   277,   591,     0,   593,   594,     0,   589,     0,
     704,     0,   704,   593,     0,   120,     0,    83,   278,   595,
     279,     0,   133,   698,   596,     0,   698,     0,   698,   277,
     595,     0,    83,   278,   595,   279,     0,     0,    84,    86,
     581,   617,   190,   278,   608,   279,     0,    84,    86,   581,
     617,   557,     0,   599,     0,   600,     0,    47,    66,   580,
     587,     0,    47,    66,   580,   601,     0,   198,    38,   128,
     692,     0,   603,     0,   604,     0,   188,   580,   164,   605,
     587,     0,   188,   580,   164,   605,   601,     0,   606,     0,
     605,   277,   606,     0,   622,   269,   607,     0,   644,     0,
     685,     0,   607,     0,   608,   277,   607,     0,   151,    19,
     621,    66,   581,   610,   612,     0,    84,    19,   621,    86,
     581,   610,   612,     0,    63,    66,   611,   182,   611,     0,
      63,   182,   611,     0,     0,   501,     0,   651,     0,   656,
       0,   113,   613,     0,     0,   660,     0,   651,     0,   615,
       0,     0,   278,   616,   279,     0,   621,     0,   616,   277,
     621,     0,   618,     0,     0,   278,   619,   279,     0,   622,
       0,   619,   277,   622,     0,   621,     0,   703,   283,   690,
       0,   703,   283,   274,     0,   690,     0,   621,     0,   703,
     283,   690,     0,   638,     0,   124,   638,     0,   625,     0,
     623,   132,   623,     0,   623,     8,   623,     0,   625,     0,
     124,   638,     0,   624,   132,   623,     0,   624,     8,   623,
       0,   639,   132,   623,     0,   639,     8,   623,     0,   626,
       0,   278,   624,   279,     0,   124,   625,     0,   627,     0,
     630,     0,   631,     0,   632,     0,   637,     0,   628,     0,
     635,     0,   633,     0,   634,     0,   636,     0,   644,   269,
     644,     0,   644,   270,   644,     0,   644,   271,   644,     0,
     644,    70,   644,     0,   644,   106,   644,     0,   644,   125,
     644,     0,   644,   126,   644,     0,   644,   122,   644,     0,
     644,   269,     6,   278,   642,   279,     0,   644,   270,     6,
     278,   642,   279,     0,   644,   271,     6,   278,   642,   279,
       0,   644,    70,     6,   278,   642,   279,     0,   644,   106,
       6,   278,   642,   279,     0,   644,   125,     6,   278,   642,
     279,     0,   644,   126,     6,   278,   642,   279,     0,   644,
     122,     6,   278,   642,   279,     0,   644,   269,   629,   278,
     642,   279,     0,   644,   270,   629,   278,   642,   279,     0,
     644,   271,   629,   278,   642,   279,     0,   644,    70,   629,
     278,   642,   279,     0,   644,   106,   629,   278,   642,   279,
       0,   644,   125,   629,   278,   642,   279,     0,   644,   126,
     629,   278,   642,   279,     0,   644,   122,   629,   278,   642,
     279,     0,   171,     0,     9,     0,   644,    18,   644,     8,
     644,     0,   644,   124,    18,   644,     8,   644,     0,   644,
     108,   644,     0,   644,   124,   108,   644,     0,   644,   108,
     644,    57,   644,     0,   644,   124,   108,   644,    57,   644,
       0,   644,    79,   640,     0,   644,   124,    79,   640,     0,
     644,    34,   644,     0,   644,   124,    34,   644,     0,   644,
     175,   644,     0,   644,   124,   175,   644,     0,   644,   175,
     200,   644,     0,   644,   124,   175,   200,   644,     0,    60,
     278,   557,   279,     0,   167,   278,   557,   279,     0,   644,
      87,    98,     0,   644,    87,   124,    98,     0,   256,     0,
     257,     0,   258,     0,   259,     0,   260,     0,   261,     0,
     641,     0,   278,   647,   279,     0,   278,   642,   279,     0,
     163,   562,   565,   644,   570,   587,   583,   586,   588,   546,
     552,     0,   163,   562,   565,   644,   570,   587,   583,   586,
     588,   546,   552,     0,   620,     0,   646,     0,   663,     0,
     650,     0,   651,     0,   424,     0,   672,     0,   673,     0,
     671,     0,   273,   644,     0,   272,   644,     0,   644,   272,
     644,     0,   644,    31,   644,     0,   644,    26,   689,     0,
     644,   273,   644,     0,   644,   274,   644,     0,   644,   275,
     644,     0,   278,   644,   279,     0,   278,   643,   279,     0,
     652,     0,   653,     0,   654,     0,    42,     0,   703,   283,
      42,     0,   101,     0,   645,     0,   228,     0,   229,     0,
     230,     0,   620,   281,   647,   282,     0,   644,     0,   647,
     277,   644,     0,   650,     0,   273,   649,     0,   205,     0,
     204,     0,   203,     0,   231,     0,   232,     0,   649,     0,
     655,     0,    41,   207,     0,   226,   207,     0,   227,   207,
       0,   284,     0,   189,     0,   233,     0,   234,     0,   241,
       0,   242,     0,    69,     0,   173,     0,   250,     0,   207,
       0,   208,   207,     0,   657,     0,   273,   658,     0,   204,
       0,   204,     0,   657,     0,   204,     0,   662,     0,   273,
     662,     0,   204,     0,   664,     0,   665,     0,   666,     0,
     668,     0,    35,   278,   274,   279,     0,    35,   278,   684,
     644,   279,     0,    35,   278,    49,   644,   279,     0,   179,
     278,   684,   644,   279,     0,   179,   278,    49,   644,   279,
       0,    13,   278,   684,   644,   279,     0,    13,   278,    49,
     644,   279,     0,   116,   278,   684,   644,   279,     0,   116,
     278,    49,   644,   279,     0,   112,   278,   684,   644,   279,
       0,   112,   278,    49,   644,   279,     0,    72,   278,   697,
     277,   644,   279,     0,   667,     0,   217,   278,   683,    66,
     644,   279,     0,   669,     0,   100,   278,   644,   279,     0,
     236,   278,   644,    66,   644,   670,   279,     0,    64,   644,
       0,     0,   686,   278,   647,   279,     0,   686,   278,   279,
       0,    22,   278,   607,    10,   369,   279,     0,   674,     0,
     675,     0,   245,   278,   644,   277,   644,   279,     0,   246,
     278,   607,   277,   608,   279,     0,   676,     0,   678,     0,
     244,   681,   677,    54,     0,   244,   681,   677,    53,   682,
      54,     0,   197,   680,   181,   682,     0,   677,   197,   680,
     181,   682,     0,   244,   679,    54,     0,   244,   679,    53,
     682,    54,     0,   197,   623,   181,   682,     0,   679,   197,
     623,   181,   682,     0,   644,     0,   644,     0,   607,     0,
     218,     0,   219,     0,   220,     0,   221,     0,   222,     0,
     223,     0,   224,     0,   225,     0,     6,     0,     0,    98,
       0,   206,     0,   710,     0,   710,     0,   710,     0,   710,
       0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
       0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
       0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
       0,   710,     0,   710,     0,   710,     0,   710,     0,   206,
       0,   711,     0,   235,     0,   238,     0,   236,     0,   246,
       0,   249,     0,   255,     0,   251,     0,   245,     0,   248,
       0,   254,     0,   247,     0,   256,     0,   257,     0,   258,
       0,   268,     0,   262,     0,   263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   538,   540,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   571,   575,   579,   583,   587,   592,   593,
     597,   599,   601,   605,   606,   610,   614,   616,   618,   620,
     622,   626,   628,   632,   634,   638,   645,   650,   655,   660,
     665,   670,   675,   681,   685,   686,   688,   690,   694,   696,
     698,   700,   704,   705,   712,   714,   716,   720,   721,   725,
     729,   730,   734,   736,   743,   747,   749,   754,   760,   761,
     763,   769,   771,   772,   776,   777,   783,   786,   792,   793,
     796,   799,   803,   807,   810,   815,   824,   828,   831,   834,
     836,   838,   840,   842,   844,   846,   848,   850,   855,   859,
     861,   863,   873,   877,   879,   890,   892,   896,   898,   899,
     905,   911,   913,   915,   919,   921,   925,   927,   931,   933,
     936,   937,   944,   962,   964,   968,   970,   974,   976,   979,
     980,   983,   988,   989,   992,   996,  1005,  1013,  1022,  1027,
    1028,  1031,  1037,  1039,  1042,  1043,  1047,  1049,  1051,  1053,
    1055,  1059,  1061,  1064,  1065,  1069,  1074,  1076,  1125,  1182,
    1186,  1187,  1190,  1191,  1194,  1196,  1200,  1201,  1202,  1205,
    1206,  1207,  1213,  1218,  1223,  1225,  1227,  1231,  1232,  1236,
    1237,  1244,  1248,  1252,  1261,  1267,  1268,  1271,  1273,  1278,
    1280,  1285,  1291,  1297,  1301,  1307,  1309,  1313,  1318,  1319,
    1320,  1322,  1324,  1328,  1330,  1333,  1334,  1338,  1343,  1345,
    1350,  1351,  1353,  1361,  1365,  1367,  1370,  1371,  1372,  1373,
    1376,  1380,  1384,  1391,  1398,  1403,  1409,  1412,  1414,  1416,
    1418,  1422,  1425,  1429,  1432,  1435,  1438,  1447,  1458,  1468,
    1478,  1488,  1490,  1494,  1496,  1500,  1501,  1505,  1511,  1512,
    1516,  1521,  1523,  1525,  1529,  1531,  1535,  1536,  1540,  1544,
    1545,  1548,  1553,  1555,  1559,  1561,  1565,  1570,  1571,  1574,
    1578,  1580,  1582,  1586,  1587,  1591,  1592,  1593,  1594,  1595,
    1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1608,  1610,  1611,  1614,  1616,  1620,  1624,  1629,  1633,
    1638,  1642,  1646,  1648,  1652,  1656,  1662,  1667,  1672,  1674,
    1676,  1680,  1682,  1684,  1688,  1689,  1690,  1692,  1696,  1700,
    1702,  1706,  1708,  1710,  1715,  1717,  1721,  1722,  1726,  1731,
    1732,  1736,  1738,  1740,  1742,  1746,  1747,  1748,  1751,  1755,
    1759,  1763,  1805,  1812,  1820,  1822,  1826,  1827,  1831,  1837,
    1844,  1864,  1868,  1870,  1872,  1876,  1887,  1891,  1892,  1896,
    1897,  1905,  1908,  1914,  1918,  1924,  1928,  1934,  1938,  1946,
    1957,  1968,  1970,  1972,  1976,  1980,  1982,  1986,  1988,  1990,
    1992,  1994,  1996,  1998,  2000,  2002,  2004,  2006,  2008,  2010,
    2012,  2014,  2018,  2020,  2024,  2030,  2034,  2037,  2044,  2046,
    2048,  2051,  2054,  2059,  2063,  2069,  2070,  2074,  2083,  2087,
    2089,  2091,  2093,  2095,  2099,  2100,  2104,  2106,  2108,  2110,
    2116,  2119,  2121,  2125,  2133,  2134,  2135,  2136,  2137,  2138,
    2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,
    2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,
    2159,  2162,  2164,  2168,  2170,  2175,  2181,  2183,  2185,  2189,
    2191,  2198,  2204,  2205,  2209,  2223,  2225,  2227,  2229,  2243,
    2253,  2254,  2258,  2259,  2265,  2269,  2271,  2273,  2275,  2277,
    2279,  2281,  2283,  2285,  2287,  2289,  2291,  2298,  2299,  2302,
    2303,  2306,  2311,  2318,  2319,  2323,  2332,  2338,  2339,  2342,
    2343,  2344,  2345,  2362,  2367,  2372,  2391,  2408,  2415,  2416,
    2423,  2427,  2433,  2439,  2447,  2451,  2457,  2461,  2465,  2471,
    2475,  2482,  2488,  2494,  2502,  2507,  2512,  2519,  2520,  2521,
    2524,  2525,  2528,  2529,  2530,  2537,  2541,  2552,  2558,  2612,
    2674,  2675,  2682,  2695,  2700,  2705,  2712,  2714,  2721,  2722,
    2723,  2727,  2732,  2737,  2748,  2749,  2750,  2753,  2757,  2761,
    2763,  2767,  2771,  2772,  2775,  2779,  2783,  2784,  2787,  2789,
    2793,  2794,  2798,  2802,  2803,  2807,  2808,  2812,  2813,  2814,
    2815,  2818,  2820,  2824,  2826,  2830,  2832,  2835,  2837,  2839,
    2843,  2845,  2847,  2851,  2853,  2855,  2859,  2863,  2865,  2867,
    2871,  2873,  2877,  2878,  2882,  2886,  2888,  2892,  2893,  2898,
    2906,  2910,  2912,  2914,  2918,  2920,  2924,  2925,  2929,  2933,
    2935,  2937,  2941,  2943,  2947,  2949,  2953,  2956,  2964,  2968,
    2970,  2974,  2976,  2980,  2982,  2989,  3001,  3015,  3019,  3023,
    3027,  3031,  3033,  3035,  3037,  3041,  3043,  3045,  3049,  3051,
    3053,  3057,  3059,  3063,  3065,  3069,  3070,  3074,  3075,  3079,
    3080,  3085,  3089,  3090,  3094,  3095,  3096,  3100,  3105,  3106,
    3110,  3112,  3116,  3117,  3121,  3123,  3127,  3130,  3135,  3137,
    3141,  3142,  3147,  3152,  3154,  3156,  3158,  3160,  3162,  3164,
    3166,  3173,  3175,  3179,  3180,  3186,  3189,  3191,  3195,  3197,
    3204,  3206,  3210,  3214,  3216,  3218,  3224,  3226,  3230,  3231,
    3235,  3237,  3240,  3241,  3245,  3247,  3249,  3253,  3254,  3258,
    3260,  3267,  3270,  3277,  3278,  3281,  3285,  3289,  3296,  3297,
    3300,  3305,  3310,  3311,  3315,  3319,  3320,  3323,  3324,  3331,
    3333,  3337,  3339,  3341,  3344,  3345,  3348,  3352,  3354,  3357,
    3359,  3365,  3366,  3370,  3374,  3375,  3380,  3381,  3385,  3389,
    3390,  3395,  3396,  3399,  3404,  3409,  3412,  3419,  3420,  3422,
    3423,  3425,  3429,  3430,  3432,  3434,  3438,  3440,  3444,  3445,
    3447,  3451,  3452,  3453,  3454,  3455,  3456,  3457,  3458,  3459,
    3460,  3465,  3467,  3469,  3471,  3473,  3475,  3477,  3479,  3486,
    3488,  3490,  3492,  3494,  3496,  3498,  3500,  3502,  3504,  3506,
    3508,  3510,  3512,  3514,  3516,  3520,  3521,  3527,  3529,  3534,
    3536,  3538,  3540,  3545,  3547,  3551,  3553,  3557,  3559,  3561,
    3563,  3567,  3571,  3575,  3577,  3581,  3586,  3591,  3598,  3603,
    3608,  3617,  3618,  3622,  3626,  3640,  3658,  3659,  3660,  3661,
    3662,  3663,  3664,  3665,  3666,  3667,  3669,  3671,  3678,  3680,
    3682,  3689,  3696,  3703,  3705,  3707,  3708,  3709,  3710,  3712,
    3714,  3718,  3722,  3738,  3754,  3758,  3762,  3763,  3767,  3768,
    3772,  3774,  3776,  3778,  3780,  3784,  3785,  3787,  3803,  3819,
    3823,  3827,  3829,  3833,  3837,  3840,  3843,  3846,  3849,  3854,
    3856,  3861,  3862,  3866,  3873,  3880,  3887,  3894,  3895,  3899,
    3905,  3906,  3907,  3908,  3911,  3913,  3915,  3918,  3925,  3934,
    3941,  3950,  3952,  3954,  3956,  3962,  3971,  3974,  3978,  3979,
    3983,  3992,  3994,  3998,  4000,  4004,  4010,  4011,  4014,  4018,
    4022,  4023,  4026,  4028,  4032,  4034,  4038,  4040,  4044,  4046,
    4050,  4053,  4056,  4059,  4061,  4063,  4065,  4067,  4069,  4071,
    4073,  4077,  4078,  4081,  4089,  4092,  4095,  4098,  4101,  4104,
    4107,  4110,  4113,  4116,  4119,  4122,  4125,  4128,  4131,  4134,
    4137,  4140,  4143,  4146,  4149,  4152,  4155,  4158,  4163,  4164,
    4169,  4171,  4172,  4173,  4174,  4175,  4176,  4177,  4178,  4179,
    4180,  4181,  4182,  4183,  4185,  4186,  4187
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
     346,   347,   347,   348,   348,   349,   349,   349,   350,   351,
     352,   352,   353,   353,   354,   354,   355,   355,   355,   356,
     356,   356,   357,   358,   359,   359,   359,   360,   360,   361,
     361,   362,   362,   362,   363,   364,   364,   365,   365,   366,
     366,   367,   368,   369,   370,   371,   371,   372,   372,   372,
     372,   372,   372,   373,   373,   374,   374,   375,   376,   376,
     376,   376,   376,   377,   378,   378,   379,   379,   379,   379,
     380,   381,   382,   383,   383,   384,   385,   385,   385,   385,
     385,   386,   387,   388,   388,   388,   388,   389,   390,   391,
     392,   393,   393,   394,   394,   395,   395,   396,   397,   397,
     398,   399,   399,   399,   400,   400,   401,   401,   402,   403,
     403,   404,   405,   405,   406,   406,   407,   408,   408,   409,
     410,   410,   410,   411,   411,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   413,   413,   414,   415,   416,   417,
     418,   419,   420,   420,   421,   422,   423,   424,   425,   425,
     425,   426,   426,   426,   427,   427,   427,   427,   428,   429,
     429,   430,   430,   430,   431,   431,   432,   432,   433,   434,
     434,   435,   435,   435,   435,   436,   436,   436,   437,   438,
     439,   440,   441,   442,   443,   443,   444,   444,   445,   446,
     447,   448,   449,   449,   449,   450,   451,   452,   452,   453,
     453,   454,   454,   455,   456,   457,   458,   459,   459,   460,
     461,   462,   462,   462,   463,   464,   464,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   466,   466,   467,   468,   469,   469,   469,   469,
     469,   469,   469,   470,   471,   472,   472,   473,   473,   473,
     473,   473,   473,   473,   474,   474,   475,   475,   475,   475,
     475,   475,   475,   476,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   478,   478,   479,   479,   480,   481,   481,   481,   482,
     482,   483,   484,   484,   485,   485,   485,   485,   485,   486,
     487,   487,   488,   488,   489,   490,   490,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   490,   491,   491,   492,
     492,   493,   493,   494,   494,   495,   495,   496,   496,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   498,   498,
     499,   499,   499,   499,   500,   500,   501,   501,   501,   502,
     502,   503,   503,   503,   504,   504,   504,   505,   505,   505,
     506,   506,   507,   507,   507,   508,   508,   509,   509,   509,
     510,   510,   511,   511,   511,   511,   512,   512,   513,   513,
     513,   514,   514,   514,   515,   515,   515,   516,   517,   518,
     518,   519,   520,   520,   521,   522,   523,   523,   524,   524,
     525,   525,   526,   527,   527,   528,   528,   529,   529,   529,
     529,   530,   530,   531,   531,   532,   532,   533,   533,   533,
     534,   534,   534,   535,   535,   535,   536,   537,   537,   537,
     538,   538,   539,   539,   540,   541,   541,   542,   542,   543,
     544,   545,   545,   545,   546,   546,   547,   547,   548,   549,
     549,   549,   550,   550,   551,   551,   552,   552,   552,   553,
     553,   554,   554,   555,   555,   556,   557,   558,   559,   560,
     561,   562,   562,   562,   562,   563,   563,   563,   564,   564,
     564,   565,   565,   566,   566,   567,   567,   568,   568,   569,
     569,   570,   571,   571,   572,   572,   572,   573,   574,   574,
     575,   575,   576,   576,   577,   577,   578,   578,   579,   579,
     580,   580,   581,   582,   582,   582,   582,   582,   582,   582,
     582,   583,   583,   584,   584,   585,   586,   586,   587,   587,
     588,   588,   589,   590,   590,   590,   590,   590,   591,   591,
     592,   592,   593,   593,   594,   594,   594,   595,   595,   596,
     596,   597,   597,   598,   598,   599,   600,   601,   602,   602,
     603,   604,   605,   605,   606,   607,   607,   608,   608,   609,
     609,   610,   610,   610,   611,   611,   501,   612,   612,   613,
     613,   614,   614,   615,   616,   616,   617,   617,   618,   619,
     619,   620,   620,   620,   621,   622,   622,   623,   623,   623,
     623,   623,   624,   624,   624,   624,   624,   624,   625,   625,
     625,   626,   626,   626,   626,   626,   626,   626,   626,   626,
     626,   627,   627,   627,   627,   627,   627,   627,   627,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   629,   629,   630,   630,   631,
     631,   631,   631,   632,   632,   633,   633,   634,   634,   634,
     634,   635,   636,   637,   637,   638,   638,   638,   639,   639,
     639,   640,   640,   641,   642,   643,   644,   644,   644,   644,
     644,   644,   644,   644,   644,   644,   644,   644,   644,   644,
     644,   644,   644,   644,   644,   644,   644,   644,   644,   644,
     644,   644,   645,   645,   645,   646,   647,   647,   648,   648,
     649,   649,   649,   649,   649,   650,   650,   650,   650,   650,
     651,   652,   652,   653,   654,   654,   654,   654,   654,   655,
     655,   656,   656,   657,   658,   659,   660,   661,   661,   662,
     663,   663,   663,   663,   664,   664,   664,   664,   664,   664,
     664,   664,   664,   664,   664,   665,   666,   667,   668,   668,
     669,   670,   670,   671,   671,   672,   673,   673,   674,   674,
     675,   675,   676,   676,   677,   677,   678,   678,   679,   679,
     680,   681,   682,   683,   683,   683,   683,   683,   683,   683,
     683,   684,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   710,
     711,   711,   711,   711,   711,   711,   711,   711,   711,   711,
     711,   711,   711,   711,   711,   711,   711
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
       3,     0,     1,     1,     2,     1,     4,     3,     3,     1,
       0,     1,     1,     2,     3,     4,     0,     1,     2,     0,
       1,     1,     5,     5,     3,     2,     0,     1,     3,     1,
       1,     6,     3,     2,     6,     2,     1,     2,     2,     2,
       0,     1,     1,     2,     0,     2,     0,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     2,     2,     2,     5,
       1,     2,     3,     2,     2,     0,     1,     1,     1,     1,
       3,     4,     8,     4,     0,     6,     1,     1,     2,     2,
       0,     3,     3,     1,     2,     2,     2,     8,     8,     8,
       8,     3,     0,     4,     0,     1,     3,     4,     1,     3,
       2,     3,     3,     1,     1,     0,     1,     2,     4,     1,
       1,     3,     1,     0,     2,     1,     6,     1,     1,     3,
       1,     2,     0,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     2,     1,     1,     1,
       2,     2,     1,     2,     3,     4,     2,     6,     4,     8,
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
       2,     0,     1,     2,     2,     4,     3,     2,     2,     7,
       1,     0,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     2,     2,     1,     1,     1,
       1,     4,     5,     1,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     6,     5,     3,     0,     2,     2,     0,     3,
       0,     4,     1,     5,     4,     1,     4,     1,     2,     2,
       1,     1,     1,     2,     2,     2,     2,     0,     3,     5,
       1,     1,     2,     3,     1,     2,     3,     0,     1,     1,
       1,     5,     5,     6,     1,     1,     1,     2,     4,     1,
       0,     5,     1,     0,     3,     2,     1,     0,     2,     0,
       1,     0,     3,     1,     0,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     2,     3,     1,     1,     3,     3,
       1,     2,     3,     1,     2,     0,     2,     1,     1,     0,
       1,     1,     1,     3,     2,     3,     0,     1,     3,     4,
       5,     1,     3,     4,     3,     0,     1,     3,     3,     1,
       1,     0,     1,     1,     4,     0,     2,     4,     0,     3,
       0,     2,     0,     2,     0,     9,    11,     0,     0,     0,
       0,     3,     2,     1,     0,     3,     5,     3,     2,     5,
       2,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       0,     2,     1,     3,     1,     1,     1,     6,     1,     0,
       3,     0,     1,     3,     6,     3,     4,     2,     3,     0,
       1,     2,     1,     1,     1,     2,     1,     2,     1,     2,
       0,     3,     0,     1,     3,     1,     2,     0,     2,     0,
       2,     0,     4,     1,     2,     1,     1,     0,     1,     3,
       2,     1,     1,     2,     1,     4,     3,     1,     3,     4,
       0,     8,     5,     1,     1,     4,     4,     4,     1,     1,
       5,     5,     1,     3,     3,     1,     1,     1,     3,     7,
       7,     5,     3,     0,     1,     1,     1,     2,     0,     1,
       1,     1,     0,     3,     1,     3,     1,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     2,     1,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     1,     1,     5,     6,     3,
       4,     5,     6,     3,     4,     3,     4,     3,     4,     4,
       5,     4,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,    11,    11,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     4,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     6,     1,     6,     1,     4,
       7,     2,     0,     4,     3,     6,     1,     1,     6,     6,
       1,     1,     4,     6,     4,     5,     3,     5,     4,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     1,
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
       0,     0,   577,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   654,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   559,    18,   564,   565,   566,     5,    17,   558,
     560,    19,   625,   621,    11,     8,   733,   734,    21,   738,
     739,     4,   471,     0,     0,     0,     0,     0,     0,   405,
     576,   579,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,     0,     0,    96,   631,   913,     0,    22,   911,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,     0,   355,    30,
      38,    35,    37,   762,    36,   762,   998,  1000,  1002,  1001,
    1007,  1003,  1010,  1008,  1004,  1006,  1009,  1005,  1011,  1012,
    1013,  1015,  1016,  1014,     0,    32,     0,    33,     0,    67,
      69,   990,   999,     0,     0,     0,     0,     0,     0,     0,
       0,   575,     0,     0,   972,   648,   653,     0,     0,   584,
       0,   690,   692,   992,     0,     0,     0,   108,   567,   997,
       0,     2,     0,     0,   638,     0,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,   433,   434,     0,   982,   412,     0,   986,
     409,   252,   989,     0,   692,   408,   383,   993,   581,   574,
     909,     0,   104,   152,   151,   105,   134,   201,   774,   978,
       0,   103,   146,   985,     0,    99,   252,   106,   915,   123,
     100,   186,   101,     0,   102,   762,   996,   107,   147,   629,
     630,     0,   914,   912,     0,    75,     0,   983,   709,   492,
     981,   485,     0,   491,   496,   486,   487,   494,   488,   489,
     490,   495,   320,     0,   254,    31,     0,    40,   761,    39,
       0,     0,     0,     0,     0,     0,   767,     0,     0,     0,
       0,     0,     0,     0,   573,   919,     0,   900,   647,   647,
     648,   660,   658,   971,   661,     0,   662,   652,   648,     0,
       0,     0,     0,     0,     0,   600,   593,   582,   583,   585,
     587,   588,   589,   596,   597,   590,     0,   691,   991,   109,
     252,   110,   186,   111,   762,   570,     0,     0,   622,     0,
     640,     0,     0,     0,     0,   410,   472,   375,     0,   375,
       0,   204,   411,   415,   406,   469,   470,     0,   254,   225,
     461,     0,   407,   424,     0,   381,   382,   481,   580,   578,
     910,   149,   149,     0,     0,     0,   161,   153,   154,   133,
       0,    97,     0,     0,   112,   254,   122,   121,   124,     0,
       0,     0,     0,     0,   974,    76,   204,   528,     0,   735,
     736,   493,     0,     0,     0,     0,   878,   906,     0,   973,
       0,   880,     0,     0,   907,     0,   901,   892,   891,   890,
     998,     0,     0,     0,   882,   883,   884,   893,   894,   902,
     903,  1002,   904,   905,     0,  1007,  1003,   908,     0,     0,
       0,     0,   861,   352,   747,   318,   856,   771,   745,   881,
     857,   895,   859,   860,   875,   876,   877,   896,   858,   920,
     921,   922,   936,   923,   938,   864,   862,   863,   946,   947,
     950,   951,   746,     0,     0,   978,     0,     0,     0,   356,
     202,     0,     0,     0,   764,     0,     0,    28,    34,     0,
       0,    44,    70,    72,   994,    68,     0,     0,     0,   766,
       0,    53,     0,     0,     0,     0,    52,   572,     0,     0,
       0,   657,   655,     0,   664,     0,   663,   665,   670,   651,
       0,   619,     0,   594,   605,   591,   605,   592,   606,   612,
     616,   617,   601,   586,   709,   742,   775,     0,     0,   254,
       0,     0,   569,   568,   624,   626,   631,   623,   636,     0,
     644,   169,     0,   474,   130,     0,   477,     0,   478,   473,
       0,   419,     0,   421,   420,     0,   422,     0,   416,     0,
       0,   255,     0,     0,   428,     0,   429,   375,   462,     0,
     468,     0,     0,     0,   465,   978,   386,   385,   480,     0,
     403,   150,     0,     0,   159,     0,   158,     0,     0,   148,
     162,   163,   165,   155,   528,   540,   515,   550,   518,   541,
     551,     0,   557,   519,     0,   547,     0,   542,   554,   514,
     537,   516,   517,   512,   375,   497,   498,   499,   507,   513,
     500,   509,   530,     0,   535,   532,   510,   547,   511,   113,
     252,   114,     0,     0,   125,     0,     0,   185,   225,   383,
     373,     0,   204,     0,    82,    84,     0,     0,     0,   756,
       0,     0,     0,     0,   845,   846,   847,     0,   708,   779,
     788,   791,   796,   792,   793,   794,   798,   799,   797,   800,
     795,   777,     0,   972,     0,   972,   897,     0,     0,   972,
     972,   972,     0,   898,   899,     0,     0,   961,     0,     0,
       0,     0,   866,   865,   654,     0,     0,   745,   317,   995,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,   507,   530,     0,     0,   354,     0,   265,     0,   763,
      29,     0,     0,    45,    73,     0,     0,    27,   753,     0,
     769,   654,     0,   732,   753,     0,     0,     0,     0,   571,
       0,   647,     0,     0,   709,     0,   669,     0,   562,     0,
     561,   917,     0,   595,     0,   603,   599,   598,     0,   609,
       0,   614,   602,     0,   740,   741,     0,     0,     0,   225,
     373,     0,   635,     0,   642,     0,   620,     0,   131,   170,
     476,   375,   418,     0,   375,   376,   423,   373,     0,   251,
     373,   224,   979,   196,   193,     0,   206,   375,   497,   373,
       0,     0,     0,   223,   226,   227,   228,   229,     0,   427,
     467,   466,   426,   425,     0,     0,     0,   389,   387,   388,
     384,     0,   375,   179,   156,   160,     0,     0,   164,     0,
     525,   538,   539,   555,     0,   552,   557,     0,   545,   543,
     544,   136,     0,     0,     0,   508,     0,     0,     0,     0,
     546,   254,     0,   373,   126,   184,     0,   187,   189,   190,
       0,     0,     0,     0,   204,   204,    79,     0,    86,    78,
     526,   527,   975,   528,     0,     0,     0,   790,   778,     0,
       0,   848,   849,   850,     0,   782,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   963,
     964,   965,   966,   967,   968,   969,   970,     0,     0,     0,
       0,   956,     0,     0,     0,     0,     0,   972,   319,   874,
     873,   748,   886,     0,   869,   977,   868,   867,   870,   871,
     872,   944,     0,   879,   773,   772,   358,   357,     0,   258,
     273,     0,   264,   266,   765,     0,     0,     0,     0,     0,
       0,    25,    54,    42,    62,    64,     0,     0,    71,     0,
     758,     0,   768,   972,     0,   758,    50,    48,     0,     0,
       0,   656,   659,     0,   671,   672,   676,   674,   675,   689,
       0,   702,   666,   668,   987,   563,   918,   604,   613,   608,
     607,     0,   618,   743,   744,   776,   373,     0,     0,   627,
     649,   628,   637,     0,   639,   643,   475,   149,   176,   168,
     171,   172,     0,     0,     0,   417,   373,   373,   376,   263,
     256,   265,   195,   375,     0,   376,   197,   192,   198,     0,
       0,   234,     0,   431,   376,   375,   373,   430,     0,     0,
       0,   402,   483,   180,   181,   157,     0,   167,   916,     0,
       0,     0,   530,     0,   553,     0,     0,   375,   376,     0,
     503,   505,     0,     0,     0,     0,     0,     0,     0,   383,
     265,   149,   128,   225,   182,   403,   972,   378,   361,   362,
       0,    83,     0,   204,     0,    88,     0,    85,     0,     0,
       0,   737,   980,     0,     0,   783,     0,     0,   789,     0,
       0,   781,   780,     0,   835,     0,   826,   825,     0,   804,
       0,   833,   851,   843,     0,     0,     0,   805,   829,     0,
       0,   808,     0,     0,     0,     0,     0,     0,     0,   806,
       0,     0,   807,     0,   837,     0,     0,   801,     0,     0,
     802,     0,     0,   803,     0,     0,   204,     0,   924,     0,
       0,   939,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   962,     0,     0,   960,     0,     0,   952,     0,     0,
       0,     0,     0,   885,   943,     0,   253,   272,     0,   330,
     353,   267,    66,    58,    59,    65,    60,    61,     0,     0,
       0,    23,    26,    42,    43,   528,   528,     0,   750,   770,
       0,     0,   749,    51,    49,    46,     0,     0,   700,   674,
       0,   698,   693,   694,   696,     0,     0,   687,     0,   707,
     610,   611,   615,   265,   183,   378,     0,   641,     0,   177,
       0,   173,     0,     0,     0,   413,   212,   207,   888,   208,
     209,   210,   211,     0,     0,   257,     0,     0,   205,   213,
       0,   234,   631,   230,     0,   432,   463,   464,   394,   395,
     391,   390,   393,   392,   375,   482,   376,   166,   976,     0,
       0,   521,     0,   520,   556,     0,   548,     0,   137,     0,
     501,     0,   529,   530,   536,   534,     0,   531,   373,     0,
       0,     0,   120,   129,   188,   375,     0,     0,   374,     0,
      98,     0,   118,   117,    94,     0,     0,    90,     0,    87,
       0,   841,   842,   785,   784,   787,   786,     0,     0,     0,
     654,     0,     0,   844,     0,     0,     0,     0,     0,     0,
     836,   834,   830,     0,   838,     0,     0,     0,     0,   839,
       0,     0,     0,     0,     0,     0,   930,   929,     0,     0,
     926,   925,     0,   934,   933,   932,   931,   928,   927,     0,
     942,   958,   957,     0,     0,     0,     0,     0,     0,     0,
     887,   259,     0,     0,   269,   270,     0,   978,     0,     0,
       0,     0,     0,     0,     0,  1000,  1005,     0,     0,   351,
     283,   278,     0,   330,   277,   287,   288,   289,   290,   291,
     298,   299,   292,   294,   295,   297,     0,   302,     0,   345,
     347,   346,     0,     0,     0,     0,     0,     0,   978,    55,
      56,     0,    57,    63,    24,   754,     0,   755,   752,   757,
     760,   759,     0,   731,     0,    47,   670,   685,   673,   699,
     695,   697,     0,     0,     0,     0,     0,   711,     0,   374,
     632,   633,   650,   179,   178,   174,     0,   376,   889,   261,
     262,   376,   376,   200,   214,   215,   375,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   479,   523,
       0,   524,     0,   135,     0,   200,   138,   139,   141,   142,
     143,   504,   506,   502,   533,   265,   403,   376,   179,     0,
       0,     0,   359,     0,   363,   375,    89,     0,     0,    91,
     530,     0,   827,     0,     0,   972,   853,   852,     0,     0,
     831,     0,     0,     0,     0,   840,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   945,   203,   935,   937,
       0,     0,   959,   954,   953,     0,   948,   949,   709,   206,
     268,     0,   306,     0,     0,     0,   301,     0,   315,   300,
     331,   332,     0,   988,   348,   349,     0,   279,     0,   284,
     281,   336,     0,     0,   303,     0,   293,   286,   296,   285,
     329,    41,   528,   709,   679,   700,   688,   686,   701,   703,
     705,   706,   717,   645,   376,   360,   634,   175,   376,   145,
     250,     0,     0,   191,   216,     0,     0,     0,   234,   217,
     220,   762,     0,   376,   400,   401,   397,   396,   399,   398,
       0,   522,   549,   144,   132,   140,     0,   375,   247,   127,
     376,     0,   709,   377,   364,     0,     0,    92,    93,    80,
       0,   812,   820,     0,   813,   821,   816,   824,   828,   832,
     814,   822,   815,   823,   809,   817,   810,   818,   811,   819,
     941,   940,   955,   702,     0,   275,   271,     0,   304,     0,
     320,     0,     0,     0,   333,     0,   344,     0,     0,     0,
       0,   339,   337,     0,     0,     0,   324,     0,   325,     0,
     751,   702,   681,   678,     0,     0,   713,   715,   716,   710,
       0,   248,   414,   194,   199,   218,   234,   762,   221,   240,
     233,   235,   404,   376,     0,   379,     0,   366,   367,   369,
     370,   702,   376,    77,    95,     0,   707,   274,     0,   305,
     323,     0,   308,     0,   314,     0,   343,   342,   984,   341,
     330,     0,     0,     0,     0,   316,     0,     0,   707,     0,
     677,   684,   704,   714,   717,   222,   240,     0,   234,   236,
     237,   249,   376,   700,   369,     0,     0,   707,     0,   709,
     711,     0,     0,     0,     0,   330,   350,   338,   340,     0,
     335,     0,   327,   326,   711,     0,   682,   721,     0,   718,
       0,   722,   234,     0,     0,   232,     0,   239,     0,   238,
     380,   372,   368,     0,   365,   119,   702,   625,   276,     0,
     321,   307,   311,   313,     0,     0,     0,   330,   625,     0,
     680,   712,   717,     0,   724,     0,   720,   723,   219,     0,
       0,   243,   242,   241,   700,   707,   638,     0,   330,     0,
       0,   330,   328,   638,   683,   719,     0,   730,   246,   244,
     245,     0,   711,   855,   322,   312,   330,   334,   309,   646,
       0,   727,     0,   726,   371,   625,   310,   725,     0,     0,
     638,   728,     0,   854,   729,     0,     0,     0
};

static const short yydefgoto[] =
{
    1895,    20,    21,   476,   124,   125,   126,   127,  1211,   727,
     722,    22,   139,   971,   972,   973,   974,   128,   129,   481,
     482,    23,    82,   385,   868,   643,   644,   645,  1104,  1105,
     245,    24,    74,    25,   157,    26,   374,    75,  1320,   227,
     378,   635,  1092,  1312,   543,   215,   370,  1078,  1505,  1506,
    1507,  1508,  1509,  1510,   221,   237,   212,   582,   213,   366,
     367,   368,   589,   590,   591,   544,   545,  1029,  1030,  1031,
    1250,  1065,   230,   321,   380,   856,   857,   858,   794,   795,
     796,  1623,   565,   466,  1368,   646,  1045,  1255,  1483,  1484,
    1485,  1629,   859,   567,   803,   804,   805,   806,  1273,   807,
    1778,  1779,  1780,  1852,   225,   319,   629,   200,   348,   472,
     560,   561,   958,   467,  1038,   961,   962,   963,  1393,  1394,
    1198,  1686,  1395,  1410,  1411,  1412,  1413,  1414,  1415,  1416,
    1417,  1418,  1419,  1420,  1421,  1422,  1423,  1693,  1424,   432,
     433,  1793,  1707,  1425,  1426,  1427,  1836,  1590,  1591,  1700,
    1701,  1428,  1429,  1430,  1431,  1586,    27,    28,   264,   468,
     469,   234,   323,  1097,  1098,  1099,  1652,  1737,  1738,  1739,
     861,  1522,   552,  1035,  1318,   232,   631,   357,   578,   579,
     820,   822,  1285,    29,    59,   785,   782,   342,   343,   352,
     353,   192,   193,   354,  1054,   573,   812,   197,   165,   335,
     336,   205,   580,  1286,    30,    96,   614,   615,   616,  1079,
    1080,   617,   711,   619,   620,  1072,  1445,   845,   621,   712,
     623,   624,   625,   626,   838,   627,   628,   835,    31,    32,
      33,    34,    35,   533,    36,   498,    37,    38,    61,   209,
     359,    39,   307,   308,   309,   310,   311,   312,   313,   314,
     756,   315,  1011,  1242,   518,   519,   761,   520,    40,  1432,
      42,   164,   534,   535,   241,  1472,  1021,   330,   540,  1024,
     776,    43,   733,   501,   297,  1246,  1616,   144,   145,   146,
     295,   505,   506,   507,   747,   744,   994,   995,   996,  1712,
    1770,  1805,   997,   998,  1237,  1740,   151,  1235,  1239,  1608,
    1609,  1467,   389,  1613,  1807,  1720,  1808,  1809,  1810,  1846,
    1880,  1883,  1433,  1434,    46,    47,   390,  1435,    49,    50,
     524,  1436,   434,   435,    51,   980,  1446,  1218,  1449,   267,
     268,   473,   488,   489,   729,   436,   437,   527,   658,   884,
     659,   660,   661,   662,  1128,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   886,  1131,  1132,  1341,   696,   672,
     439,   440,   943,  1257,   441,   442,   443,   444,   445,   446,
     447,   649,    79,   243,   229,  1070,  1081,   751,   448,   449,
     450,   451,   452,   453,   454,  1561,   455,   456,   457,   458,
     459,   460,   934,   461,   688,  1185,   689,  1182,   927,   296,
     462,   463,   871,  1287,   944,   218,   791,  1111,   249,   195,
     246,  1757,   222,  1881,  1003,  1437,   999,   130,   464,   204,
     206,   975,   698,   235,   158,   465,   132
};

static const short yypact[] =
{
    2081,   633,    68,  1216,    37,   725,   215,  1620,   347,  2176,
     217,   282,  1335,    68,   734,   494,  5079,   661,  5079,   290,
     291,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   411,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  6890,  5079,  5079,  5079,  5079,  5079,-32768,
  -32768,   416,   868,  5079,  5079,  5079,   582,  5079,   401,  5079,
    5079,-32768,  5079,  5079,-32768,   244,-32768,   423,-32768,-32768,
     565,  5079,-32768,  5079,  5079,  5079,   575,  5079,  5079,  5079,
    5079,   401,  5079,  5079,  5079,  5079,-32768,  5079,   393,   609,
  -32768,-32768,-32768,   469,-32768,   469,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   690,   570,   779,-32768,    42,-32768,
  -32768,-32768,-32768,  5079,  5079,  5079,   814,   802,   834,  1129,
     119,   812,   307,   397,   474,   768,-32768,  5079,   928,   827,
     860,-32768,  5079,-32768,  5079,  5079,  5079,-32768,-32768,-32768,
    5079,-32768,  1017,   194,   749,   901,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   438,-32768,-32768,   868,-32768,-32768,   226,-32768,
  -32768,   769,-32768,   819,-32768,-32768,   519,-32768,   873,-32768,
  -32768,   845,-32768,  1109,-32768,-32768,  1046,-32768,-32768,-32768,
     868,-32768,-32768,-32768,   673,-32768,   769,-32768,-32768,   541,
  -32768,   999,-32768,  1008,-32768,   469,-32768,-32768,-32768,-32768,
  -32768,   998,-32768,-32768,   889,-32768,  1023,-32768,   911,-32768,
  -32768,-32768,   889,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  4679,  5079,   942,-32768,  5079,-32768,-32768,-32768,
    2286,   394,   970,  3134,  5079,  1054,   900,  1082,  1090,  2286,
    1015,  1044,  1062,  3134,   940,-32768,  6281,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4346,-32768,-32768,-32768,  1040,
    5079,  1121,  1020,   409,  5079,  1049,-32768,-32768,   827,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  5079,-32768,-32768,-32768,
     769,-32768,   999,-32768,   469,  1067,  6281,  1072,-32768,  6281,
    1176,   102,   995,  1002,  1004,   901,-32768,  1237,   852,-32768,
    5079,-32768,   438,-32768,-32768,-32768,-32768,  5079,   942,  1956,
    1057,  2239,  1000,-32768,  6890,-32768,-32768,   266,-32768,-32768,
  -32768,  1011,  1011,   868,  1163,   868,   231,  1109,-32768,-32768,
    6907,-32768,  5079,  5079,-32768,   942,-32768,-32768,  1251,   490,
    1006,  5079,  1276,  5079,-32768,-32768,    50,   560,  3680,-32768,
  -32768,-32768,  1010,  1012,  1021,  1093,-32768,-32768,  1025,-32768,
    1026,-32768,  1033,  1035,-32768,  1038,-32768,-32768,-32768,-32768,
    1047,  1050,  1094,  1105,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1052,-32768,-32768,  4926,  1056,  1059,-32768,  6281,  6281,
    4593,  5079,-32768,-32768,-32768,  1061,  1045,-32768,  1048,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1064,  1066,  1069,  6907,  1058,   685,-32768,
  -32768,  1076,  1279,   751,-32768,  5079,  1157,-32768,-32768,  5079,
    5079,   378,-32768,-32768,-32768,-32768,  5079,  5079,   687,-32768,
    5079,-32768,  1278,  5079,  2286,  1197,  1070,-32768,  5079,  5024,
     200,-32768,-32768,  6281,-32768,  1284,  1078,-32768,   153,-32768,
     576,-32768,   116,-32768,   530,-32768,   530,-32768,  1079,-32768,
     115,-32768,  1183,-32768,   424,-32768,-32768,  1089,  1077,   942,
    1083,  1352,-32768,-32768,  1086,-32768,   173,-32768,   663,  1177,
    1166,-32768,  1274,  1280,-32768,   868,-32768,  1282,-32768,-32768,
    1349,-32768,  1353,-32768,-32768,  1332,-32768,  6907,-32768,  6907,
     755,-32768,  1369,  5079,-32768,  6714,-32768,   137,-32768,  5079,
     822,   819,  1198,  1168,  1243,  1211,-32768,-32768,-32768,   444,
    1252,-32768,  1201,   401,-32768,   868,-32768,  1378,  1308,-32768,
     231,-32768,-32768,-32768,    39,  1213,-32768,-32768,-32768,  1214,
  -32768,  1265,  1134,-32768,  1318,  1137,   628,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1135,-32768,
  -32768,-32768,   114,  1139,  1140,    29,-32768,  1137,-32768,-32768,
     769,-32768,  1365,  1422,-32768,   868,   868,-32768,  1956,   519,
  -32768,  1304,-32768,  1281,  1162,-32768,  4959,  4477,  1305,-32768,
    1313,  1164,  3927,  1165,   896,   972,  1245,  3583,   460,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2662,   729,  5271,   126,-32768,  5079,  6281,   765,
     832,   876,  1203,-32768,-32768,  6281,  4013,  1048,   448,  1247,
    6281,  5271,   208,   208,   734,   767,  1167,   225,-32768,-32768,
    5271,  6281,  5079,  6281,  6281,  6281,  6281,  6281,  5357,  3391,
  -32768,-32768,  1424,  1170,  5079,-32768,  5079,  1403,  5079,-32768,
  -32768,  5483,  1267,-32768,-32768,  1240,  3134,-32768,  1388,   813,
  -32768,   734,  1174,-32768,  1388,  5483,  1390,  1391,  5079,-32768,
     225,-32768,   335,  3841,  1260,  5271,-32768,  5079,-32768,   632,
  -32768,-32768,   224,-32768,  1268,-32768,-32768,-32768,  5079,   750,
    5079,-32768,-32768,  5079,-32768,-32768,  5271,  5079,  1449,  1956,
  -32768,  6281,  1217,  6281,  1333,  1219,-32768,   868,-32768,   118,
  -32768,-32768,-32768,  1189,-32768,-32768,-32768,   104,  5079,-32768,
  -32768,-32768,-32768,  1448,-32768,  1193,  1434,-32768,  1451,-32768,
    1397,  1399,   469,-32768,-32768,-32768,-32768,-32768,  1468,-32768,
  -32768,-32768,-32768,-32768,  5079,  6795,   401,  1361,  1362,  1367,
  -32768,   401,-32768,   990,-32768,-32768,  1336,   868,-32768,   255,
    1339,-32768,-32768,-32768,   401,-32768,  1134,   475,-32768,-32768,
  -32768,  1456,   475,  1340,   475,-32768,   401,   401,  1225,   401,
  -32768,   942,  5079,-32768,  1395,-32768,   823,-32768,-32768,-32768,
     266,  1342,  5079,   829,    51,-32768,-32768,  1229,  1488,-32768,
  -32768,-32768,-32768,   560,  5079,  1346,  4260,-32768,-32768,  1346,
    3927,-32768,-32768,-32768,   127,-32768,   505,  2637,  4013,  4013,
    6281,  6281,  1775,  1233,   164,  2058,  6281,  2357,   158,  2618,
    2879,  5604,  2977,  3238,  3336,  6281,  6281,  1502,  6281,  1234,
    6281,  1244,   367,  6281,  6281,  6281,  6281,  6281,  6281,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1463,   878,   165,
    5271,-32768,  4013,  6281,   523,   242,  1254,   474,-32768,-32768,
  -32768,-32768,  1048,   642,-32768,-32768,  1506,   208,   208,   915,
     915,-32768,   837,-32768,-32768,-32768,-32768,-32768,   842,-32768,
    1341,  1522,  1403,-32768,-32768,  5079,  5079,  5079,  5079,  5079,
    5079,  1271,-32768,   436,-32768,-32768,  5483,  1411,-32768,   439,
    1436,  5079,-32768,   474,  5271,  1436,  1271,  1277,  5483,  2547,
    1487,-32768,-32768,  2707,  1283,   924,-32768,-32768,-32768,  1285,
    4013,  1481,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   610,-32768,-32768,-32768,-32768,-32768,   857,  1342,-32768,
  -32768,-32768,  1048,  5079,-32768,-32768,-32768,  1011,  1544,-32768,
     118,-32768,  1286,  4013,  3063,-32768,-32768,-32768,-32768,-32768,
  -32768,  1403,-32768,-32768,  3063,-32768,-32768,-32768,-32768,   469,
     469,  1311,  1288,-32768,-32768,-32768,-32768,-32768,   459,   315,
     839,-32768,  1549,-32768,-32768,-32768,  5079,-32768,-32768,    37,
     862,  1393,  1424,  1290,-32768,  1201,   885,-32768,-32768,   672,
  -32768,  1292,  5079,   718,  1295,  1296,   401,  1297,  1552,   519,
    1403,  1011,  1280,  1956,-32768,  1252,   474,  1368,  1392,-32768,
    1289,-32768,   401,  1445,  1532,-32768,  4959,-32768,   401,  1350,
    1539,-32768,-32768,  1317,  1320,-32768,  4013,  4013,-32768,  4013,
    4013,-32768,  1593,   162,  1048,  1324,-32768,-32768,  1326,  1048,
    5701,-32768,-32768,-32768,  1507,  1328,  1329,  1048,   794,  1330,
    1331,  1048,  6281,  6281,  1233,  6281,  5948,  1334,  1337,  1048,
    1343,  1344,  1048,  6281,  1048,  1351,  1354,  1048,  1355,  1356,
    1048,  1357,  1359,  1048,   387,   413,-32768,   555,-32768,   600,
    6281,-32768,   638,   697,   714,   766,   838,   858,  6281,  6281,
    5271,-32768,  1556,   195,  1048,  1430,  5271,-32768,  6281,  6281,
    5271,  6281,  6281,-32768,-32768,  5079,-32768,-32768,  5079,  6524,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5483,  1541,
    5483,-32768,  1271,   436,-32768,   479,   479,   303,-32768,-32768,
    4346,   895,-32768,  1271,  1277,  1347,  2547,  1338,   717,  1348,
    3841,  1485,-32768,  1496,  1504,  1551,  5271,  1228,  1621,  1566,
  -32768,-32768,-32768,  1403,-32768,  1368,    36,  1370,  1201,  1508,
    1201,-32768,  4013,   128,  1001,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  3063,  3063,-32768,  1522,  6281,-32768,   742,
    1493,  1311,   244,-32768,  4013,-32768,-32768,-32768,  1517,  1518,
    1519,  1520,  1521,  1523,-32768,-32768,-32768,-32768,-32768,  1375,
      37,-32768,  1452,-32768,-32768,   475,-32768,  3063,   464,   475,
  -32768,   475,-32768,  1424,-32768,-32768,  1379,-32768,-32768,   266,
    1522,  1201,-32768,  1280,-32768,-32768,  4346,  1635,-32768,   293,
  -32768,  1383,-32768,  1370,-32768,  1384,   868,   193,  1386,-32768,
     868,-32768,-32768,-32768,  1593,-32768,  1593,  6281,  1503,  1503,
     734,  1389,   930,-32768,  1503,  1503,  6281,  1503,  1503,   179,
    1048,-32768,   908,  6281,  1048,  1503,  1503,  1503,  1503,  1048,
    1503,  1503,  1503,  1503,  1503,  1503,-32768,-32768,  1394,  6907,
  -32768,-32768,   870,-32768,-32768,-32768,-32768,-32768,-32768,   921,
     986,-32768,-32768,  5271,  5271,  1613,  1489,   951,   931,  1022,
    1048,-32768,  6907,  1396,-32768,-32768,  1630,  1636,  4144,   309,
    1398,  1401,  1590,  6281,  1404,  1408,  4401,  5079,  5079,-32768,
  -32768,-32768,  1623,  6389,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   150,-32768,  1410,-32768,
  -32768,-32768,  1601,  1414,  1417,  1419,  1420,  1421,   727,-32768,
  -32768,  1573,-32768,-32768,-32768,-32768,  1524,-32768,-32768,-32768,
  -32768,-32768,  1284,-32768,  2547,  1347,  1695,-32768,   924,-32768,
  -32768,-32768,  3841,   957,  5079,  6281,  4013,  1567,  1522,-32768,
  -32768,-32768,-32768,   990,-32768,-32768,   131,-32768,-32768,-32768,
  -32768,-32768,  1048,  1690,   742,-32768,   969,  5079,-32768,  1634,
     140,  1530,  1640,  1531,  1673,  1641,  1681,  1403,-32768,-32768,
    1450,-32768,  1453,-32768,  1632,  1690,   464,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1403,  1252,-32768,   990,  1549,
    1665,  1602,-32768,  1342,-32768,-32768,-32768,  1617,    96,-32768,
    1424,  1618,  1048,  1457,  1458,   474,-32768,-32768,  1459,  1460,
    1048,  1461,  1464,  6281,  6281,  1048,  1465,  1466,  1467,  1469,
    1470,  1471,  1474,  1475,  1476,  1477,-32768,-32768,-32768,-32768,
    6281,  1482,-32768,-32768,-32768,  5271,-32768,-32768,  1260,   132,
  -32768,  1678,-32768,  6034,  5079,  6281,-32768,  4013,  1048,-32768,
  -32768,-32768,  1486,-32768,-32768,-32768,  5079,-32768,   229,-32768,
    1550,-32768,   187,  1491,-32768,  3735,-32768,-32768,-32768,-32768,
  -32768,-32768,   479,  1260,  5079,   773,-32768,-32768,  1490,-32768,
    1048,   460,   622,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1484,  5079,-32768,-32768,  1666,  1676,  5079,  1311,-32768,
  -32768,   469,  5079,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1522,-32768,-32768,-32768,-32768,-32768,  1522,-32768,-32768,-32768,
  -32768,  4068,  1260,-32768,-32768,  6281,   868,-32768,-32768,-32768,
     868,-32768,-32768,  6281,-32768,-32768,-32768,-32768,  1048,  1048,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1048,-32768,-32768,  1481,  3063,-32768,-32768,  1494,-32768,   618,
    4679,   586,   148,  1495,-32768,  1688,-32768,  5079,  5079,    37,
     103,-32768,-32768,  1525,  1689,  4013,-32768,   882,-32768,  1499,
  -32768,  1481,  1505,-32768,  4013,  6281,-32768,-32768,  1663,-32768,
    1509,-32768,-32768,-32768,-32768,-32768,  1311,   469,-32768,  1660,
  -32768,-32768,-32768,-32768,  1549,-32768,  4068,  1513,   924,-32768,
  -32768,  1481,  1048,-32768,-32768,  1022,  1566,-32768,  1346,-32768,
    1637,  3735,-32768,  1610,-32768,  3735,-32768,-32768,-32768,-32768,
    6613,   229,  6281,  3735,   152,-32768,  3735,  4766,  1566,  5079,
  -32768,   460,-32768,-32768,  6163,-32768,  1660,   415,  1311,  1669,
    1672,-32768,-32768,   717,  1515,  4068,  1706,  1566,  1526,  1260,
    1567,  1527,  2775,  1533,   926,  6613,  1534,-32768,-32768,  1034,
     156,  1753,-32768,-32768,  1567,   975,-32768,-32768,  1528,  1535,
     800,  5079,  1311,   542,   542,-32768,  1766,-32768,  1631,-32768,
  -32768,-32768,   924,  4068,-32768,-32768,  1481,  1685,-32768,  3735,
    1534,-32768,-32768,  1768,  3735,  1783,  1774,  6613,  1685,  5079,
  -32768,-32768,  6163,  1547,-32768,  5079,-32768,-32768,-32768,  1622,
     196,-32768,-32768,-32768,   996,  1566,   749,   985,  6613,   122,
    5079,  6613,-32768,   749,-32768,-32768,  5079,  1743,-32768,-32768,
  -32768,  4013,  1567,-32768,-32768,-32768,  6613,-32768,-32768,-32768,
    1553,  1558,  1559,-32768,   460,  1685,-32768,-32768,  5079,  5079,
     749,-32768,  1560,-32768,-32768,  1829,  1840,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -90,   183,-32768,   236,  1570,   629,-32768,
    -370,-32768,-32768,  -591,    41,  -633, -1028,  1831,  1571,  1563,
    1122,-32768,-32768,-32768,   744,-32768,  1212,   988,-32768,   752,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   764,-32768,-32768,-32768,-32768,-32768,
     352,-32768,-32768,  1529,-32768,-32768,-32768,  -323,-32768,-32768,
  -32768,  1492,-32768,-32768,  1270,  -345,-32768,-32768,-32768,   831,
  -32768,  -638,-32768,-32768,  1540,  1095,   770,  1516,  1071,   771,
  -32768,   363,   -52,  -236,-32768,  -318,   301,  -974,-32768,-32768,
     388,-32768,  1536, -1052,-32768,-32768,-32768,-32768, -1214,   391,
      97,    94,    99,    67,-32768,-32768,-32768,-32768,   -93,  -277,
  -32768,  1096,-32768,  -647,-32768,  -925,-32768,   920,-32768,-32768,
  -32768,-32768,-32768, -1242,  -919,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768, -1033,
     198,-32768,  -524,-32768,-32768,-32768,-32768,-32768,   296,-32768,
     129,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1169,-32768,-32768,   871,-32768, -1179,-32768,-32768, -1527,   157,
    -613,   425,  -288,  -951,   647,-32768,-32768,  -519,  -764,-32768,
  -32768,  -981, -1284,-32768,-32768,-32768,-32768,-32768,  1554,-32768,
    1345,  1546,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1579,-32768,-32768,-32768,-32768,-32768,  -530,  -444,-32768,  1051,
     602,  -537,  -354,-32768,-32768,-32768,  -294,  -962,-32768,  -350,
  -32768,-32768,-32768,-32768,  1291,-32768,-32768,  1091,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1892,-32768,
  -32768,-32768,-32768,-32768,  1611,-32768,-32768,-32768,  1416,-32768,
    1413,-32768,-32768,-32768,-32768,  1172,-32768,-32768,-32768,     8,
  -32768, -1095,-32768,  1155,  -499,-32768,-32768,  -864,-32768,-32768,
  -32768,    81,  -841,  -183,   554,-32768,-32768,  -632,-32768,  1786,
    -878, -1008,-32768,  1187, -1070, -1258,-32768,  -926,-32768,-32768,
  -32768,-32768,   941,-32768,-32768,   728,    35, -1080, -1191,-32768,
     218, -1072,  -514, -1266,   323,-32768,   100,-32768,   125,-32768,
    -704,-32768,  1938,  1939,-32768,-32768,  1423,  1941,-32768,-32768,
  -32768,  -188,  -280,  -404,-32768,  1221, -1094,   960,-32768,  -101,
    -660,  -410,-32768,-32768,-32768,  -420,   -38,  -422,  -656,-32768,
    -451,-32768,-32768,-32768,   488,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -511,-32768,   807,-32768,   605,-32768,  1007,
    -943,-32768,  -589,-32768,   702,  -927,  -130,  -885,  -882,  -880,
     -12,     1,    21,-32768,   -77,  -741,  -477,  -124,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   783,-32768, -1025,-32768,   539,
    -876,   510,-32768,   875,   336,  -685,  1405, -1158,-32768,   -58,
    1872,-32768,   -47,   -53, -1023,   566,   -24,   -42,  -135,   -15,
     -41,  -165,-32768,   -66,   -31,    -9,-32768
};


#define	YYLAST		7158


static const short yytable[] =
{
     131,   152,   198,   131,   269,    78,   220,   153,    41,   159,
     764,   216,   288,   291,   257,   508,   618,   317,   289,   292,
     622,   592,   710,   557,   955,   217,   695,   251,   260,   233,
     929,   238,   201,   750,  1113,   797,   255,   772,  1114,   583,
    1396,   254,  1200,   226,   194,   196,   199,   202,   153,   207,
     214,   555,   259,   261,   219,   196,   223,  1488,   202,  1191,
     153,   207,   937,   236,   131,   730,   256,  1228,   152,   959,
    1268,   562,   247,   262,   153,   250,   196,   258,   247,   223,
     199,   202,  1015,   153,   207,   236,   131,  1265,   202,   228,
     324,  1256,   203,   648,  1269,   275,  1095,   277,   633,   983,
     299,  1256,   987,  1275,   231,  1220,   502,  1258,   483,   869,
    1293,   559,   228,   786,  1315,   787,  1266,  1258,   483,   952,
     860,   798,  1448,   736,   219,   153,   219,  1298,   525,   325,
     320,  1568,   293,   375,   382,  1116,   888,   843,   223,   888,
    1524,   878,  1051,   318,   986,   202,   153,   236,   888,  1259,
    1036,   159,  1260,  1760,  1261,  1381,   888,  1018,  1262,  1259,
     888,  1385,  1260,   746,  1261,  1310,  1835,  1464,  1262,   276,
    1337,  1589,  1876,   888,  1039,   908,  1142,  1041,  1044,   759,
    1440,   528,  1443,   344,   239,   283,  1048,  1543,   702,   492,
     322,  1102,  1143,   703,  1603,   137,   541,  1657,   778,   702,
     327,   877,   800,   888,   703,   702,   885,   -81,   371,  1783,
     703,   618,  1452,  1528,  1592,   622,   647,  1486,  1316,  -667,
    1027,   240,   848,   531,   273,   470,   702,   529,   474,   345,
    1001,   703,  1121,  1122,   702,  1650,   133,  1144,  1696,   703,
    1090,    76,  1869,    76,   328,   592,  1703,   511,   138,  1584,
    1585,   702,   768,   514,   219,   239,   703,   219,  1822,  1117,
     889,   153,  1133,   889,   484,   131,  1145,   752,   702,    60,
     153,   576,   889,   703,   484,  1470,  1183,   587,   526,   808,
     889,    83,   577,   801,   889,  1471,   305,  1697,  1134,  1479,
    1480,   199,   240,  1028,  1870,   153,  1854,   889,  1698,  1523,
     830,   135,   556,   134,  1458,   477,   346,   849,  1520,   470,
      77,   217,    77,   570,   477,   724,   574,   829,  1468,   274,
    1256,  1256,   281,  1503,   802,   541,   841,   889,   642,  1103,
     641,   219,   632,  1146,  1658,  1498,  1258,  1258,   219,   521,
     219,  1513,   219,  1213,  1654,   575,  1180,  1481,   630,  1593,
      14,   584,   528,   586,  1256,  1224,  1225,    14,  1562,  1563,
    1076,   702,  1280,   202,   207,   542,   703,   637,   990,  1115,
    1258,  1055,   153,  1037,   199,   282,  1383,  1253,  1259,  1259,
    1761,  1260,  1260,  1261,  1261,  1212,  1604,  1262,  1262,  1270,
    1271,  1517,   760,   702,   907,   844,   274,  1223,   703,  1766,
     909,  1684,  1699,  1243,   737,  1529,  1118,  1477,   537,   516,
    1618,   936,  1259,   702,  1728,  1260,   639,  1261,   703,  1633,
     941,  1262,   699,  1263,  1264,   877,  1443,  1753,  1695,   877,
    -667,  1801,  1486,  1766,   704,   705,   706,   707,   514,   702,
    1322,   100,   337,  1277,   703,   704,   705,   706,   707,   526,
    1782,   704,   705,   706,   707,   723,  1096,  1574,   823,  1068,
    1333,  1334,  1813,  1335,  1336,   508,   153,   739,   888,   723,
     202,   484,   704,   705,   706,   707,  1451,   153,   102,   741,
     293,   153,   706,   707,   202,   153,  1014,  1789,  -375,   159,
     338,   817,  1746,  1032,   588,    97,  1034,   704,   705,   706,
     707,   930,   931,  1281,   940,  1215,   824,  1068,  1710,  1046,
     720,   285,  1775,  1119,   704,   705,   706,   707,  1797,  1189,
    1768,   728,   355,   294,  1827,   734,  1619,   799,   818,   477,
    1620,  1621,  1069,   779,  1062,  1647,  1605,   851,  1838,   515,
    1682,  1342,   160,  1278,   162,  1516,  1106,   103,  1391,  1614,
    1787,  1501,   559,  1833,   792,   376,   219,   104,   991,  1219,
     792,   483,  1706,  1575,  1815,   147,  1648,   161,  1659,   869,
    1309,   208,  1640,   825,  1088,  1013,  1186,  1187,   725,  1110,
    1221,   702,   105,   956,   636,   286,   703,   287,  1504,   224,
    1646,   287,   889,  1455,   516,  1862,  1476,   163,  1848,   356,
     217,   285,   339,  1814,   228,    76,  1885,   704,   705,   706,
     707,   517,   702,  1247,   992,    98,  1875,   703,  1490,  1878,
     340,  1216,   388,   854,   855,  1006,   702,   242,   528,   219,
     911,   703,   819,   244,  1886,  1855,  1209,  1120,   872,   704,
     705,   706,   707,   252,   702,   932,  1171,  1279,   870,   703,
    1181,   839,   377,   754,   341,   726,   647,  1663,  1786,   704,
     705,   706,   707,  1721,   702,  1849,  1366,  1722,   223,   703,
     148,   263,  1751,    52,  1790,   290,   470,   149,   470,   285,
     964,   287,  1731,    76,    53,   704,   705,   706,   707,   702,
    1323,    54,  1367,   945,   703,  1515,  1804,   210,   211,  1735,
     219,   763,  1877,  1786,  1248,   219,  1850,   219,  1535,   219,
    1747,  1716,   484,  1210,   723,  1824,    55,   484,  1706,   840,
    1188,  1732,  1706,   702,   755,   526,   484,  1733,   703,   202,
    1706,   217,  1856,  1802,   202,   293,  1717,   647,  1004,  1057,
     702,  1256,  1786,  1863,   150,   703,  1806,   266,  1075,   153,
     470,   153,    77,  1851,   386,  1267,   265,  1258,   219,  1706,
     219,  1240,   391,   287,    76,  1026,  -225,  1276,  1311,  1084,
    1085,   293,  1087,  1489,  1786,   563,  1053,  1056,   905,   219,
     285,    56,  1781,  1872,  1231,  1106,  1388,    80,    81,  1297,
    1890,  1788,   702,   521,  1718,  1012,  1706,   703,  1232,  1259,
    1202,  1706,  1260,  1205,  1261,   219,   219,   748,  1262,   154,
    1611,   248,  1241,    57,   913,  1067,  1864,    58,  1502,   270,
     702,   372,  1233,   349,  1512,   703,   350,   704,   705,   706,
     707,  1820,  1463,    77,  1370,  1617,   285,   228,   293,  1557,
    1231,   155,  1061,   153,   503,   773,   528,   271,  1369,   749,
     731,  1346,   509,   153,  1232,  1073,   156,   373,   704,   705,
     706,   707,  1752,  1005,   702,  1112,  -225,   228,   228,   703,
     228,   351,   704,   705,   706,   707,  1234,   732,  1233,  1371,
    1649,   915,   293,  1843,   702,   553,  1282,  1089,  -225,   703,
     704,   705,   706,   707,  1749,  -225,   702,  1100,   554,  1009,
    1181,   703,  1714,  1206,   702,   331,  1181,  1791,   272,   703,
     704,   705,   706,   707, -1011,   301,  1010,  1373,   332,  1192,
    1844,  1692, -1011,  1283,  1193,   917,  1204, -1011,  1207,  -225,
   -1011,   279,  1234,  1845,   702,   704,   705,   706,   707,   703,
     508,   702,  1203,   526,  1179,   278,   703,   702,  1569,  1299,
     302,  1006,   703,   333,  1300,   334,   484,   202,   207,   484,
     236,   131,   714,   280,   715,  1544, -1011,   484,   778,   704,
     705,   706,   707,   142,   143, -1011,  1374,   702,   303,   484,
     484,   304,   703, -1011,   202,   474,   704,   705,   706,   707,
   -1012,  1231,  1873,  1375,   284,  1299,  1497,   305, -1012,  1879,
    1303,  1227, -1011, -1012, -1011,  1232, -1012,  -988,   143,  1306,
    -991,   300,   702,  -700,   219,   618,   329,   703, -1011,   622,
   -1011, -1011, -1011,   306,   316,  1324,  1893,  1519,   718,  1233,
     719,  1328,   788,   810,   789,   811,   508,   326,   704,   705,
     706,   707, -1012,   358,   700,  1376,   938,   347,   702,  1764,
    1560, -1012,   360,   703,  1683,   217,   369,  1288,  1771, -1012,
     702,   379,   474,  1231,   528,   703,   704,   705,   706,   707,
    1289, -1011,   381,  1288,   702,   210,   211,  1232, -1012,   703,
   -1012,   383,   955,  1234,   219,  1447,  1447,  1450,   743,  1711,
     981,   219,   982,  1625, -1012,   384, -1012, -1012, -1012,   471,
    1093,  1233,  1094,  1181,  1181,   387,   865,   228,  1101,   388,
     704,   705,   706,   707,  1192,  1626,  1194,  1377,   479,  1195,
    1834,  1196,  1627,   228,  1473,  1871,  1475,  1063,  1064,   228,
     704,   705,   706,   707,  1093,    99,  1244,  1378,  1741,  1290,
     486,  1291,   704,   705,   706,   707,  1392, -1012,   490,  1558,
     704,   705,   706,   707,   491,  1234,  1628,   470,  1765,  1766,
     217,   526,  1295,   493,  1296, -1011, -1011, -1011, -1011, -1011,
   -1011, -1011,   700,   494,  1453,  1708,   100, -1011,   487, -1011,
     704,   705,   706,   707,  1891,  1892,   219,  1518,   101,  1397,
    1438,   495,   497,   704,   705,   706,   707,   532,   808,   484,
    1559,   484,  1832,  1766,   407,   408,   409,  1192,   700,  1537,
    1567,   361,   906,   102,   910,  1884,   513,   484,   914,   916,
     918,   202,   510,   704,   705,   706,   707,  1794,   512,   522,
    1566,  1796,   417,   418,   700,    14,  1606,  1655,   746,  1800,
     539, -1012, -1012, -1012, -1012, -1012, -1012, -1012,   362,  1439,
     363,  1442,  1839, -1012,  1840, -1012,    62,   546,   704,   705,
     706,   707,  1766, -1013,  1874,   547,   548,    63,  1830,   438,
     550, -1013,   568,   364,    64,  1826, -1013,   571,   528, -1013,
     581,   585,   103,   634,   638,  1181,   640,    65,   673,   717,
     674,  1500,   104,   500,   704,   705,   706,   707,   365,   675,
     676,   683,   438,   677,   678,  1857,   704,   705,   706,   707,
    1859,   679,   684,   680,  1527, -1013,   681,   105,  1531,   793,
     704,   705,   706,   707, -1013,  -974,   701,   713,   682,  1607,
     685,  1708, -1013,   536,   690,  1708,   538,   691,   700,   721,
    1573,    99,   708,  1708,   735,   738,  1803,   726,    66,   709,
     743, -1013,  -991, -1013,   716,   745,   758,   762,   766,  1734,
     767,   769,   770,   771,    67,   774,   775, -1013,   777, -1013,
   -1013, -1013,  1708,   781,   541,   526,   780,   783,   784,   790,
     814,    68,   100,  1136,   815,  1140,   816,  1148,  1151,   196,
    1156,  1159,  1162,  -434,   101,   821,    69,  1583,  1112,  1112,
      70,   826,   827,    71,  1438,   285,   831,   832,   136,  1708,
     833,    72,   834,   836,  1708,   837,   842,   846,   847,   102,
   -1013,   919,   920,   921,   922,   923,   924,   925,   926,   852,
      73,   687,   853,   862,  -670,   692,   693,   697,   864,   865,
     873,   874,   875,   879,   933,   484,   939,   843,   960,   976,
     977,   979,   984,   202,   287,   318,   988,   989,  1000,  1016,
    1709,  1023,  1007,  -670,  -670,  1020,  -670,  1033,  1042,  1713,
    1025,  1043,  1447,  -670,  -670,  -670,  -670,  -670,   153,  -670,
    1044,   793,  -670,  -670,  -670,  -670,  -670,  1049,   103,  1050,
    -670,  -670,  1052,  1058,  1059,   106,  -670,  1091,   104,  1060,
    1066,  1071,  1077,  1086,  1082,  1096,   740,  1108,  1109,   731,
     742,  1130,  1166,  1168, -1013, -1013, -1013, -1013, -1013, -1013,
   -1013,  1170,  1631,   105,   107,   108, -1013,   109, -1013,  1178,
    1729,  1190,   702,  1197,   110,   111,   112,   113,   114,  1199,
     115,   106,  1214,   116,   117,   118,   119,   120,  1208,  1217,
    1690,   121,   122,  1226,  1210,  1238,  1249,   123,  1272,  1284,
    1230,  1292,  1308,  1236,  1252,   202,  1274,   266,  1317,  1294,
     107,   108,  1301,   109,  1304,  1305,  1307,  1112,  1319,  1730,
     110,   111,   112,   113,   114,  1102,   115,  1326,  1329,   116,
     117,   118,   119,   120,  1330,   318,  1331,   121,   122,  1332,
    1704,   888,  1338,   123,  1339,  1343,  1344,  1345,  1347,  1348,
    1382,  1384,  1355,   945,  1441,  1356,  1709,  1456,   153,  1459,
    1709,  1357,  1358,   199,  1454,   528,  1776,  1457,  1709,  1360,
    1460,  1709,  1361,  1362,  1363,  1364,   152,  1365,  1461,  1756,
    1462,  1465,   153,  1466,  1743,  1474,  1487,   718,  1744,  1491,
    1492,  1493,  1494,  1495,  1499,  1496,  1068,  1709,  1514,  1521,
     528,  1525,  1727,  1526,   887,  1530,  1340,  1564,  1536,  1571,
    1565,    84,  1570,  1556,  1576,  -980,   134,  1587,    85,  1577,
    1579,   438,    86,    87,  1580,   912,  1594,  1595,   196,  1758,
    1596,    88,   928,  1597,  1709,  1598,  1599,   935,   438,  1709,
    1759,  1600,   528,    89,  1601,   746,  1602,   438,   942,  1612,
     946,   947,   948,   949,   950,   942,  1622,  1632,  1634,  1636,
    1637,   152,   526,   528,  1635,  1638,   528,   153,  1639,  1641,
    1643,  1651,  1642,  1653,  1656,  1660,  1661,  1662,  1664,  1665,
    1666,   528,  1687,  1667,  1670,  1671,  1672,  1588,  1673,  1674,
    1675,  1438,   438,  1676,  1677,  1678,  1679,   526,   219,  1811,
    1004,  1681,  1694,  1723,  1725,   153,  1726,  1715,    90,  1705,
     152,  1754,  1748,   438,  1755,  1763,   153,  1773,   536,  1762,
    1022,  1125,  1767,  1769,  1126,    91,  1438,  1774,   392,  1777,
    1785,  1795,  1867,  1792,  1821,  1823,  1811,   393,  1816,   526,
      92,  1818,   153,  1837,    93,  1825,  1828,  1841,   152,  1831,
     394,  1766,  1842,  1813,   153,    94,   395,   396,   162,  1814,
     526,  1858,  1860,   526,  1861,  1866,  1882,  1811,  1438,  1896,
    1004,  1868,  1887,   153,    95,  1888,   199,  1889,   526,  1894,
    1897,   478,  1444,   140,   397,   485,   496,   398,   978,  1438,
    1327,  1112,  1438,  1107,   863,  1325,  1313,   199,  1645,   593,
     828,  1251,   530,  1314,  1017,   564,   551,  1438,  1644,  1047,
    1685,  1321,  1624,  1812,  1819,   400,   401,  1630,  1817,   199,
     199,  1853,  1201,   957,  1040,   566,  1702,   402,  1750,  1245,
    1798,   403,  1469,  1784,  1615,  1083,   558,  1123,  1124,  1129,
     572,  1511,  1137,  1138,  1141,   141,  1149,  1152,  1154,  1157,
    1160,  1163,  1164,  1165,   549,  1167,   813,  1169,   850,   523,
    1172,  1173,  1174,  1175,  1176,  1177,  1019,  1074,   753,   757,
    1008,   298,  1002,  1772,  1229,  1719,  1847,   438,    44,    45,
    1184,    48,  1865,  1533,  1534,  1222,  1127,   765,   404,  1538,
    1539,  1351,  1541,  1542,   405,   985,  1478,  1302,  1724,   253,
    1546,  1547,  1548,  1549,   406,  1550,  1551,  1552,  1553,  1554,
    1555,  1386,  1582,     0,   809,     0,     0,     0,   407,   408,
     409,   410,   210,   211,     0,     0,     0,     0,     0,   563,
       0,   438,   411,     0,     0,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     107,   421,     0,   109,     0,     0,   422,   423,     0,   424,
     425,   426,   112,   113,   114,   427,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,     0,     0,     0,   428,   429,     0,
       0,     0,     0,   499,     0,   431,     0,     0,     0,   287,
       0,     0,     0,     0,  1135,     0,     0,  1126,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
     393,     0,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,   395,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,     3,     0,     0,
       0,     0,     0,     0,     4,     0,     5,   397,     6,     0,
     398,     0,     0,     7,     0,     0,     0,   942,     0,     0,
       8,     0,     0,     0,     0,     0,     0,     0,     0,  1349,
    1350,     0,  1352,  1354,     9,     0,     0,     0,   400,   401,
    1359,     0,   106,     0,     0,    10,     0,     0,     0,     0,
     402,     0,     0,     0,   403,     0,     0,  1372,     0,     0,
       0,     0,    99,     0,     0,  1379,  1380,   438,     0,     0,
       0,   107,   108,   438,   109,  1184,  1387,   438,  1389,  1390,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,   100,   123,     0,     0,   438,     0,  1127,
       0,   404,    11,     0,     0,   101,     0,   405,     0,    12,
       0,     0,    13,   438,    14,    15,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,   407,   408,   409,   410,   210,   211,     0,     0,    16,
       0,     0,   569,     0,  1482,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   107,   421,     0,   109,     0,     0,   422,
     423,     0,   424,   425,   426,   112,   113,   114,   427,   115,
       0,     0,   116,   117,   118,   119,   120,     0,    17,     0,
     121,   122,     0,   438,     0,     0,   123,     0,     0,   103,
     428,   429,     0,    18,    19,     0,   499,     0,   431,   104,
       0,     0,   287,     0,  1532,     0,     0,     0,     0,     0,
       0,     0,     0,  1540,     0,     0,     0,     0,     0,     0,
    1545,     0,     0,  1139,   105,     0,  1126,     0,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
     438,   438,   394,     0,     0,     0,     0,     0,   395,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1578,   107,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   397,   115,     0,   398,
     116,   117,   118,   119,   120,     0,     0,     0,   121,   122,
       0,     0,     0,     0,   123,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,   401,     0,
       0,     0,     0,     0,     0,     0,   475,     0,     0,   402,
       0,     0,  1610,   403,   107,   108,     0,   109,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,   106,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,   108,     0,   109,     0,     0,     0,  1127,     0,
     404,   110,   111,   112,   113,   114,   405,   115,     0,     0,
     116,   117,   118,   119,   120,     0,   406,     0,   121,   122,
    1668,  1669,     0,     0,   123,     0,     0,     0,     0,     0,
     407,   408,   409,   410,   210,   211,     0,  1680,     0,     0,
       0,     0,   438,     0,   411,     0,     0,     0,     0,     0,
    1689,     0,  1691,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   107,   421,     0,   109,     0,     0,   422,   423,
       0,   424,   425,   426,   112,   113,   114,   427,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,   965,     0,     0,  1147,   123,     0,  1126,     0,   428,
     429,   392,     0,     0,     0,   499,     0,   431,     0,     0,
     393,   287,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,   890,     0,     0,     0,   395,
     396,     0,  1742,   702,     0,     0,     0,     0,   703,     0,
    1745,   891,     0,     0,     0,     0,     0,     0,     0,     0,
     890,     0,     0,     0,     0,     0,     0,   397,   702,     0,
     398,     0,     0,   703,     0,     0,   891,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   892,     0,     0,
       0,     0,     0,     0,     0,     0,   893,     0,   400,   401,
       0,     0,  1610,     0,   894,     0,     0,     0,     0,     0,
     402,     0,   892,     0,   403,     0,   968,     0,     0,     0,
       0,   893,     0,   895,     0,   896,     0,     0,     0,   894,
       0,     0,     0,   106,     0,     0,     0,     0,     0,   897,
       0,   898,   899,   900,     0,     0,     0,     0,   895,  1799,
     896,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,   108,   897,   109,   898,   899,   900,  1127,
       0,   404,   110,   111,   112,   113,   114,   405,   115,     0,
       0,   116,   117,   118,   119,   120,     0,   406,     0,   121,
     122,     0,   901,     0,     0,   123,     0,     0,     0,     0,
       0,   407,   408,   409,   410,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   411,     0,   901,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   107,   421,     0,   109,     0,     0,   422,
     423,     0,   424,   425,   426,   112,   113,   114,   427,   115,
      14,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,  1150,   123,     0,  1126,     0,
     428,   429,   392,     0,     0,     0,   499,     0,   431,     0,
       0,   393,   287,     0,     0,     0,   902,   903,   904,   704,
     705,   706,   707,   106,   394,     0,   940,     0,     0,     0,
     395,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   902,   903,   904,   704,   705,   706,   707,     0,     0,
       0,     0,   107,   108,     0,   109,     0,     0,   397,     0,
       0,   398,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,     0,     0,     0,   400,
     401,   106,     0,  1155,     0,   993,  1126,     0,     0,     0,
     392,   402,     0,     0,     0,   403,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   108,   394,   109,     0,     0,     0,     0,   395,   396,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,     0,     0,   397,     0,     0,   398,
    1127,     0,   404,  1829,     0,   431,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,     0,
       0,     0,     0,     0,     0,     0,     0,   400,   401,     0,
       0,     0,   407,   408,   409,   410,   210,   211,     0,   402,
       0,     0,     0,   403,     0,     0,   411,     0,     0,     0,
       0,     0,     0,     0,   395,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   107,   421,     0,   109,     0,     0,
     422,   423,     0,   424,   425,   426,   112,   113,   114,   427,
     115,     0,   397,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,  1127,     0,
     404,   428,   429,     0,     0,     0,   405,   499,     0,   431,
       0,   399,     0,   287,     0,     0,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,   408,   409,   410,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   107,   421,     0,   109,     0,     0,   422,   423,
       0,   424,   425,   426,   112,   113,   114,   427,   115,     0,
       0,   116,   117,   118,   119,   120,   404,     0,     0,   121,
     122,     0,     0,     0,  1158,   123,     0,  1126,     0,   428,
     429,   392,   406,     0,     0,   499,     0,   431,     0,     0,
     393,   287,     0,     0,     0,     0,   407,   408,   409,     0,
     210,   211,     0,   394,     0,     0,     0,     0,     0,   395,
     396,     0,     0,     0,     0,     0,     0,     0,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,     0,     0,
       0,     0,     0,     0,   422,   423,     0,   397,     0,     0,
     398,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1254,     0,   400,   401,
     106,     0,  1161,     0,     0,  1126,     0,     0,     0,   392,
     402,     0,     0,     0,   403,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     108,   394,   109,     0,     0,     0,     0,   395,   396,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,   397,     0,     0,   398,  1127,
       0,   404,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,     0,     0,
       0,     0,     0,   953,     0,     0,   400,   401,     0,     0,
       0,   407,   408,   409,   410,   210,   211,     0,   402,     0,
       0,     0,   403,     0,     0,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   107,   421,     0,   109,     0,     0,   422,
     423,     0,   424,   425,   426,   112,   113,   114,   427,   115,
       0,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,  1127,     0,   404,
     428,   429,     0,     0,     0,   405,   499,     0,   431,     0,
       0,     0,   287,     0,     0,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   407,
     408,   409,   410,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   107,   421,     0,   109,     0,     0,   422,   423,     0,
     424,   425,   426,   112,   113,   114,   427,   115,     0,     0,
     116,   117,   118,   119,   120,     0,   392,   106,   121,   122,
       0,     0,     0,     0,   123,   393,     0,     0,   428,   429,
       0,     0,     0,     0,   499,     0,   431,     0,   394,     0,
     287,     0,     0,     0,   395,   396,   107,   108,     0,   109,
       0,     0,     0,     0,     0,     0,   110,   111,   112,   113,
     114,     0,   115,   651,     0,   116,   117,   118,   119,   120,
       0,     0,   397,   121,   122,   398,     0,     0,     0,   123,
       0,     0,     0,     0,     0,   954,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,   392,     0,   402,     0,     0,     0,   403,
       0,     0,   393,     0,     0,     0,     0,   880,     0,     0,
       0,     0,     0,     0,     0,   394,     0,     0,   650,     0,
       0,   395,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     651,     0,     0,     0,     0,     0,   694,     0,     0,   397,
     653,     0,   398,     0,     0,     0,   404,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,     0,     0,     0,     0,     0,     0,     0,
     400,   401,     0,     0,     0,     0,   407,   408,   409,   410,
     210,   211,   402,     0,     0,     0,   403,     0,     0,     0,
     411,     0,     0,     0,   652,     0,     0,     0,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   107,   421,
       0,   109,     0,     0,   422,   423,     0,   424,   425,   426,
     112,   113,   114,   427,   115,     0,     0,   116,   117,   118,
     119,   120,   881,   882,   883,   121,   122,   653,     0,     0,
       0,   123,     0,   404,     0,   428,   429,     0,     0,   405,
       0,   657,     0,   431,     0,     0,     0,   287,     0,   406,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,   408,   409,   410,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   107,   421,     0,   109,     0,
       0,   422,   423,     0,   424,   425,   426,   112,   113,   114,
     427,   115,     0,     0,   116,   117,   654,   655,   656,     0,
     392,   106,   121,   122,     0,     0,     0,     0,   123,   393,
       0,     0,   428,   429,     0,     0,     0,     0,   657,     0,
     431,     0,   394,     0,   287,     0,     0,     0,   395,   396,
     107,   108,     0,   109,     0,     0,     0,     0,     0,     0,
     110,   111,   112,   113,   114,     0,   115,   651,     0,   116,
     117,   118,   119,   120,     0,     0,   397,   121,   122,   398,
       0,     0,     0,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   400,   401,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,   402,
       0,     0,     0,   403,     0,     0,     0,   106,   394,     0,
       0,   876,     0,     0,   395,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,     0,     0,   107,   108,     0,   109,
       0,     0,   397,     0,     0,   398,   110,   111,   112,   113,
     114,     0,   115,     0,   653,   116,   117,   118,   119,   120,
     404,     0,     0,   121,   122,     0,   405,     0,     0,   123,
       0,     0,     0,   400,   401,     0,   406,     0,     0,   993,
       0,     0,     0,     0,     0,   402,     0,     0,     0,   403,
     407,   408,   409,   410,   210,   211,     0,   652,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   107,   421,     0,   109,     0,     0,   422,   423,
       0,   424,   425,   426,   112,   113,   114,   427,   115,     0,
     653,   116,   117,   654,   655,   656,   404,     0,     0,   121,
     122,     0,   405,     0,     0,   123,     0,     0,     0,   428,
     429,     0,   406,     0,     0,   657,     0,   431,     0,     0,
       0,   287,     0,     0,     0,     0,   407,   408,   409,   410,
     210,   211,     0,     0,     0,     0,     0,     0,     0,     0,
     411,     0,     0,     0,     0,     0,     0,     0,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   107,   421,
       0,   109,     0,     0,   422,   423,     0,   424,   425,   426,
     112,   113,   114,   427,   115,     0,     0,   116,   117,   654,
     655,   656,     0,   392,   106,   121,   122,     0,     0,     0,
       0,   123,   393,     0,     0,   428,   429,     0,     0,     0,
       0,   657,     0,   431,     0,   394,     0,   287,     0,     0,
       0,   395,   396,   107,   108,     0,   109,     0,     0,     0,
       0,     0,     0,   110,   111,   112,   113,   114,     0,   115,
     651,     0,   116,   117,   118,   119,   120,     0,     0,   397,
     121,   122,   398,     0,     0,     0,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1736,     0,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     400,   401,     0,     0,     0,     0,     0,     0,   393,     0,
       0,     0,   402,     0,     0,     0,   403,     0,     0,   107,
     108,   394,   109,     0,   876,     0,     0,   395,   396,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,   397,     0,     0,   398,     0,
    1572,     0,     0,     0,     0,     0,     0,   653,     0,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,   399,     0,   400,   401,     0,   406,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,   403,   407,   408,   409,   410,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   107,   421,     0,   109,     0,
       0,   422,   423,     0,   424,   425,   426,   112,   113,   114,
     427,   115,     0,     0,   116,   117,   118,   119,   120,   404,
       0,     0,   121,   122,     0,   405,     0,     0,   123,     0,
       0,     0,   428,   429,     0,   406,     0,     0,   657,     0,
     431,     0,     0,     0,   287,     0,     0,     0,     0,   407,
     408,   409,   410,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,     0,
       0,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   107,   421,     0,   109,     0,     0,   422,   423,     0,
     424,   425,   426,   112,   113,   114,   427,   115,     0,     0,
     116,   117,   118,   119,   120,     0,   392,   106,   121,   122,
       0,     0,     0,     0,   123,   393,     0,     0,   428,   429,
     504,     0,     0,     0,   499,     0,   431,     0,   394,     0,
     287,     0,     0,     0,   395,   396,   107,   108,     0,   109,
       0,     0,     0,     0,     0,     0,   110,   111,   112,   113,
     114,     0,   115,     0,     0,   116,   117,   118,   119,   120,
       0,     0,   397,   121,   122,   398,     0,     0,     0,   123,
       0,     0,     0,     0,     0,     0,     0,  1581,     0,     0,
       0,    76,     0,   106,     0,     0,     0,     0,     0,     0,
       0,   399,   392,   400,   401,     0,     0,     0,     0,     0,
       0,   393,     0,     0,     0,   402,     0,     0,     0,   403,
       0,     0,   107,   108,   394,   109,     0,     0,     0,     0,
     395,   396,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,     0,     0,   397,     0,
      77,   398,     0,     0,     0,     0,   694,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,   399,     0,   400,
     401,     0,   406,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   403,   407,   408,   409,   410,
     210,   211,     0,     0,     0,     0,     0,     0,     0,     0,
     411,     0,     0,     0,     0,     0,     0,     0,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   107,   421,
       0,   109,     0,     0,   422,   423,     0,   424,   425,   426,
     112,   113,   114,   427,   115,     0,     0,   116,   117,   118,
     119,   120,   404,     0,     0,   121,   122,     0,   405,     0,
       0,   123,     0,     0,     0,   428,   429,     0,   406,     0,
       0,   499,     0,   431,     0,     0,     0,   287,     0,     0,
       0,     0,   407,   408,   409,   410,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,   411,     0,     0,     0,
       0,     0,     0,     0,     0,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   107,   421,     0,   109,     0,     0,
     422,   423,     0,   424,   425,   426,   112,   113,   114,   427,
     115,     0,     0,   116,   117,   118,   119,   120,     0,   392,
       0,   121,   122,     0,     0,     0,     0,   123,   393,     0,
       0,   428,   429,     0,     0,     0,     0,   430,     0,   431,
       0,   394,     0,   287,     0,     0,     0,   395,   396,     0,
       0,     0,   106,     0,     0,     0,     0,     0,   866,     0,
       0,     0,   595,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,   867,     0,   398,     0,
     596,   107,   108,   597,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,   117,   118,   119,   120,     0,   400,   401,   121,   122,
       0,     0,     0,     0,   123,     0,     0,   392,   402,     0,
     954,     0,   403,     0,   598,     0,   393,     0,     0,     0,
     599,   600,   601,     0,   602,   603,   604,     0,   605,   394,
       0,     0,     0,     0,     0,   395,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   606,     0,
     607,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,   398,     0,     0,   404,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,   608,     0,     0,     0,   406,     0,     0,     0,     0,
       0,     0,     0,   686,   400,   401,     0,     0,   609,   407,
     408,   409,   410,   210,   211,     0,   402,     0,     0,     0,
     403,     0,     0,   411,     0,     0,     0,     0,     0,     0,
     610,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   107,   421,     0,   109,     0,     0,   422,   423,     0,
     424,   425,   426,   112,   113,   114,   427,   115,     0,     0,
     116,   117,   118,   119,   120,   611,   612,   694,   121,   122,
       0,     0,     0,     0,   123,     0,     0,   404,   428,   429,
       0,     0,   613,   405,   499,     0,   431,     0,     0,     0,
     287,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,   408,   409,
     410,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   107,
     421,     0,   109,     0,     0,   422,   423,     0,   424,   425,
     426,   112,   113,   114,   427,   115,     0,     0,   116,   117,
     118,   119,   120,     0,   392,   106,   121,   122,     0,     0,
       0,     0,   123,   393,     0,     0,   428,   429,     0,     0,
       0,     0,   499,     0,   431,     0,   394,     0,   287,     0,
       0,     0,   395,   396,   107,   108,     0,   109,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
     397,   121,   122,   398,     0,     0,     0,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     392,   400,   401,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,   402,     0,     0,     0,   403,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,   395,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,   400,   401,     0,
     406,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,   403,   407,   408,   409,   410,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   107,   421,     0,   109,
       0,     0,   422,   423,     0,   424,   425,   426,   112,   113,
     114,   427,   115,     0,     0,   116,   117,   118,   119,   120,
     404,     0,     0,   121,   122,     0,   405,     0,     0,   123,
       0,     0,     0,   428,   429,     0,   406,     0,     0,   499,
       0,   431,     0,     0,     0,   287,     0,   965,     0,     0,
     407,   408,   409,   410,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,   411,     0,     0,     0,     0,     0,
       0,     0,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   107,   421,     0,   109,     0,     0,   422,   423,
       0,   424,   425,   426,   112,   113,   114,   427,   115,     0,
       0,   116,   117,   118,   119,   120,     0,   392,     0,   121,
     122,     0,     0,     0,     0,   123,   393,     0,     0,   428,
     429,   966,     0,     0,     0,   499,   951,   431,     0,   394,
       0,   287,     0,     0,     0,   395,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   967,     0,     0,
       0,     0,   968,   397,     0,     0,   398,     0,   969,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,   970,     0,     0,
       0,     0,     0,     0,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,   392,     0,   402,     0,   107,   108,
     403,   109,     0,   393,     0,     0,     0,     0,   110,   111,
     112,   113,   114,     0,   115,     0,   394,   116,   117,   118,
     119,   120,   395,   396,     0,   121,   122,     0,     0,     0,
       0,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,     0,     0,   398,     0,     0,     0,   404,     0,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,   400,   401,     0,  1153,     0,     0,   407,   408,   409,
     410,   210,   211,   402,     0,     0,     0,   403,     0,     0,
       0,   411,     0,     0,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   107,
     421,     0,   109,     0,     0,   422,   423,     0,   424,   425,
     426,   112,   113,   114,   427,   115,     0,     0,   116,   117,
     118,   119,   120,     0,  1340,     0,   121,   122,     0,     0,
       0,     0,   123,     0,   404,     0,   428,   429,     0,     0,
     405,     0,   499,     0,   431,     0,     0,     0,   287,     0,
     406,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,   408,   409,   410,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   107,   421,     0,   109,
       0,     0,   422,   423,     0,   424,   425,   426,   112,   113,
     114,   427,   115,     0,     0,   116,   117,   118,   119,   120,
       0,   392,     0,   121,   122,     0,     0,     0,     0,   123,
     393,     0,     0,   428,   429,     0,     0,     0,     0,   499,
       0,   431,     0,   394,     0,   287,     0,     0,     0,   395,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   400,   401,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
     402,     0,     0,     0,   403,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,   395,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,  1353,     0,
     403,   407,   408,   409,   410,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   411,     0,     0,     0,     0,
       0,     0,     0,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   107,   421,     0,   109,     0,     0,   422,
     423,     0,   424,   425,   426,   112,   113,   114,   427,   115,
       0,     0,   116,   117,   118,   119,   120,   404,     0,     0,
     121,   122,     0,   405,     0,     0,   123,     0,     0,     0,
     428,   429,     0,   406,     0,     0,   499,     0,   431,     0,
       0,     0,   287,     0,     0,     0,     0,   407,   408,   409,
     410,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   411,  1716,     0,     0,     0,     0,     0,     0,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   107,
     421,     0,   109,     0,     0,   422,   423,  1717,   424,   425,
     426,   112,   113,   114,   427,   115,     0,     0,   116,   117,
     118,   119,   120,     0,   392,     0,   121,   122,     0,     0,
       0,     0,   123,   393,     0,     0,   428,   429,     0,     0,
    1688,     0,   499,     0,   431,     0,   394,     0,   287,     0,
       0,     0,   395,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1718,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,   403,   107,   108,
       0,   109,     0,     0,     0,     0,  1199,     0,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,     0,     0,     0,     0,     6,     0,     0,     0,
       0,     0,     0,  -280,     0,     0,     0,  1398,  1399,     0,
    1400,     0,     0,     0,   404,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,  1401,     0,     0,
     406,     0,     0,  1402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,   408,   409,   410,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,     0,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   107,   421,     0,   109,
       0,     0,   422,   423,     0,   424,   425,   426,   112,   113,
     114,   427,   115,  1403,     0,   116,   117,   118,   119,   120,
       0,  1199,     0,   121,   122,     0,     0,     0,     0,   123,
       0,     0,    14,   428,   429,     0,     0,     0,     0,   499,
       0,   431,     0,     0,     0,   287,     0,  1404,     0,     0,
       0,     6,     0,     0,     0,     0,     0,    16,  -282,     0,
       0,     0,  1398,  1399,     0,  1400,  1588,     0,     0,     0,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,  1401,     0,     0,     0,     0,     0,  1402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1405,   108,     0,   109,     0,     0,
    1199,     0,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,  1406,   118,   119,   120,     0,     0,
       0,   121,   122,  1407,  1408,  1409,     0,   123,     0,     0,
       6,     0,     0,     0,     0,     0,     0,     0,  1403,     0,
       0,  1398,  1399,     0,  1400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,     0,     0,
       0,  1401,     0,     0,     0,     0,     0,  1402,     0,     0,
       0,     0,  1404,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,     0,     0,   594,     0,     0,     0,   595,     0,     0,
       0,     0,     0,     0,   793,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   596,     0,  1403,   597,  1405,
     108,     0,   109,     0,     0,     0,     0,     0,     0,   110,
     111,   112,   113,   114,     0,   115,    14,     0,   116,  1406,
     118,   119,   120,     0,     0,     0,   121,   122,  1407,  1408,
    1409,  1404,   123,     0,     0,     0,     0,     0,     0,   598,
       0,    16,     0,     0,     0,   599,   600,   601,     0,   602,
     603,   604,     0,   605,   594,     0,     0,     0,   595,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   606,     0,   607,   596,     0,     0,   597,
       0,     0,     0,     0,     0,     0,     0,     0,  1405,   108,
       0,   109,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,   114,     0,   115,     0,   608,   116,  1406,   118,
     119,   120,     0,     0,     0,   121,   122,  1407,  1408,  1409,
     598,   123,     0,   609,     0,     0,   599,   600,   601,     0,
     602,   603,   604,     0,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   610,     0,     0,     0,     0,
       0,     0,     0,     0,   606,     0,   607,     0,     0,     0,
     106,     0,     0,     0,     0,     0,   594,     0,     0,     0,
     595,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     611,   612,     0,     0,     0,     0,     0,   608,   596,   107,
     108,   597,   109,     0,     0,     0,     0,   613,     0,   110,
     111,   112,   113,   114,   609,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,     0,     0,     0,   610,     0,     0,     0,
       0,     0,   598,     0,     0,     0,     0,     0,   599,   600,
     601,   106,   602,   603,   604,     0,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   611,   612,     0,     0,     0,   606,     0,   607,     0,
     107,   108,     0,   109,     0,     0,     0,     0,   613,     0,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,   608,
       0,     0,     0,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   609,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,   610,     0,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,   182,   183,   107,   108,     0,   109,     0,
       0,   184,   185,   611,   612,   110,   111,   112,   113,   114,
     186,   115,   187,     0,   116,   117,   118,   119,   120,     0,
     613,     0,   121,   122,   188,   189,   190,   191,   123
};

static const short yycheck[] =
{
       9,    16,    55,    12,   105,     4,    64,    16,     0,    18,
     524,    63,   142,   143,    91,   295,   370,   152,   142,   143,
     370,   366,   466,   341,   709,    63,   430,    85,    94,    70,
     686,    73,    56,   510,   875,   565,    89,   536,   879,   362,
    1198,    88,   961,    67,    53,    54,    55,    56,    57,    58,
      62,   339,    93,    95,    63,    64,    65,  1271,    67,   937,
      69,    70,   694,    72,    73,   487,    90,   993,    83,   716,
    1044,   348,    81,    97,    83,    84,    85,    92,    87,    88,
      89,    90,   767,    92,    93,    94,    95,  1038,    97,    68,
     156,  1034,    57,   387,  1045,   133,   860,   135,   375,   731,
     147,  1044,   735,  1054,    69,   983,   289,  1034,   273,   646,
    1072,   347,    91,   557,  1095,   559,  1041,  1044,   283,   708,
     639,   565,  1216,   493,   133,   134,   135,  1078,   316,   160,
     154,  1389,     6,   226,   235,     8,     8,    23,   147,     8,
    1319,   652,   802,   152,   735,   154,   155,   156,     8,  1034,
      46,   160,  1034,    50,  1034,  1180,     8,   770,  1034,  1044,
       8,  1186,  1044,    10,  1044,  1090,    10,  1237,  1044,   134,
       8,  1413,    50,     8,   787,    49,    18,   790,    46,    64,
    1208,   316,  1210,   195,    11,    66,   799,     8,    26,   279,
     155,   140,    34,    31,  1452,    12,    94,   101,   543,    26,
       6,   652,    65,     8,    31,    26,   657,   157,   220,  1736,
      31,   565,  1220,    20,    64,   565,   177,  1269,  1096,    66,
     102,    48,   193,   324,   182,   263,    26,   320,   266,     3,
     744,    31,   888,   889,    26,  1519,    19,    79,     9,    31,
     853,   204,    46,   204,   163,   590,    59,   300,    12,  1407,
    1408,    26,   529,    29,   263,    11,    31,   266,  1785,   132,
     132,   270,    98,   132,   273,   274,   108,   151,    26,   201,
     279,     5,   132,    31,   283,   239,   932,    46,   316,   567,
     132,    66,    16,   146,   132,   249,   170,    58,   124,  1263,
    1264,   300,    48,   175,    98,   304,  1823,   132,    69,     6,
     594,    19,   340,    86,  1230,   270,    80,   278,  1316,   347,
     273,   349,   273,   351,   279,   480,   354,   278,  1243,   277,
    1263,  1264,   139,  1297,   187,    94,   614,   132,   278,   278,
     383,   340,   373,   175,   238,  1286,  1263,  1264,   347,   304,
     349,  1303,   351,   976,  1523,   354,   181,  1266,   372,   199,
     163,   363,   487,   365,  1297,   988,   989,   163,  1383,  1384,
     837,    26,    47,   372,   373,   263,    31,   379,   738,   880,
    1297,   815,   381,   269,   383,   139,   181,  1033,  1263,  1264,
     277,  1263,  1264,  1263,  1264,   976,  1456,  1263,  1264,  1049,
    1050,  1310,   277,    26,   674,   281,   277,   988,    31,   277,
     274,   269,   173,  1016,   494,   212,   279,   279,   327,   185,
     279,   691,  1297,    26,  1628,  1297,   381,  1297,    31,   279,
     700,  1297,   431,  1036,  1037,   876,  1454,   279,  1586,   880,
     277,   279,  1484,   277,   272,   273,   274,   275,    29,    26,
    1100,    47,     4,  1056,    31,   272,   273,   274,   275,   487,
    1734,   272,   273,   274,   275,   479,   163,   148,   582,   204,
    1116,  1117,    47,  1119,  1120,   745,   475,   498,     8,   493,
     479,   480,   272,   273,   274,   275,  1217,   486,    84,   279,
       6,   490,   274,   275,   493,   494,   766,  1745,    24,   498,
      52,    47,  1683,   781,   263,   148,   784,   272,   273,   274,
     275,    53,    54,   188,   279,    66,   583,   204,  1602,   797,
     475,   204,  1726,     8,   272,   273,   274,   275,  1760,   277,
    1711,   486,     3,    49,  1790,   490,  1477,   565,    84,   494,
    1481,  1482,   277,   545,   822,  1516,  1462,   630,  1804,   130,
    1565,  1130,   252,    84,   133,  1309,   864,   153,  1195,  1468,
    1741,  1292,   788,  1795,   563,    14,   565,   163,   741,   981,
     569,   726,  1595,   254,  1778,    71,  1517,   276,  1530,  1106,
    1089,   155,  1497,   585,   851,   763,    53,    54,   200,   873,
     984,    26,   188,   713,    94,   278,    31,   284,   124,     7,
    1515,   284,   132,  1226,   185,  1837,  1252,   186,  1812,    80,
     638,   204,   164,   188,   583,   204,  1872,   272,   273,   274,
     275,   202,    26,  1023,   279,   268,  1858,    31,  1274,  1861,
     182,   182,   198,   635,   636,   749,    26,   204,   763,   638,
     677,    31,   188,    68,  1876,  1826,   200,   132,   647,   272,
     273,   274,   275,    68,    26,   197,   279,   188,   647,    31,
     930,    23,   111,   123,   216,   277,   177,  1535,  1738,   272,
     273,   274,   275,  1614,    26,   123,   279,  1618,   677,    31,
     176,   278,    86,    40,  1746,   278,   714,   183,   716,   204,
     718,   284,  1633,   204,    51,   272,   273,   274,   275,    26,
    1100,    58,   279,   702,    31,  1308,  1768,   207,   208,  1650,
     709,   277,  1860,  1783,  1027,   714,   164,   716,  1340,   718,
    1684,    89,   721,   277,   738,  1787,    83,   726,  1751,    91,
     197,  1640,  1755,    26,   194,   763,   735,  1646,    31,   738,
    1763,   769,  1827,  1766,   743,     6,   114,   177,   747,   816,
      26,  1684,  1822,  1838,    16,    31,  1769,   278,   273,   758,
     788,   760,   273,   211,   244,  1043,   147,  1684,   767,  1792,
     769,   151,   252,   284,   204,   777,    24,  1055,  1091,   846,
     847,     6,   849,  1272,  1854,    33,   814,   815,    49,   788,
     204,   148,  1733,  1855,    67,  1103,  1190,    62,    63,  1077,
    1885,  1742,    26,   758,   172,   760,  1829,    31,    81,  1684,
     965,  1834,  1684,   968,  1684,   814,   815,   231,  1684,   148,
    1466,    83,   202,   180,    49,   827,  1839,   184,  1295,   129,
      26,   148,   105,     4,  1301,    31,     7,   272,   273,   274,
     275,  1782,  1236,   273,   279,  1473,   204,   816,     6,  1369,
      67,   180,   821,   852,   290,   182,   981,   277,  1166,   273,
     163,    57,   298,   862,    81,   834,   195,   184,   272,   273,
     274,   275,   276,   231,    26,   874,   124,   846,   847,    31,
     849,    52,   272,   273,   274,   275,   159,   190,   105,   279,
    1518,    49,     6,    83,    26,    33,    47,   852,   146,    31,
     272,   273,   274,   275,   276,   153,    26,   862,    46,   149,
    1180,    31,   129,   969,    26,     4,  1186,  1748,   129,    31,
     272,   273,   274,   275,    18,    88,   166,   279,    17,   277,
     120,  1577,    26,    84,   282,    49,   967,    31,   970,   187,
      34,   129,   159,   133,    26,   272,   273,   274,   275,    31,
    1220,    26,   966,   981,    66,   131,    31,    26,  1392,   277,
     123,  1075,    31,    52,   282,    54,   965,   966,   967,   968,
     969,   970,   277,   129,   279,    57,    70,   976,  1313,   272,
     273,   274,   275,   239,   240,    79,   279,    26,   151,   988,
     989,   154,    31,    87,   993,  1023,   272,   273,   274,   275,
      18,    67,  1856,   279,   182,   277,  1284,   170,    26,  1863,
     282,   993,   106,    31,   108,    81,    34,   280,   240,  1086,
     283,    83,    26,    89,  1023,  1369,   267,    31,   122,  1369,
     124,   125,   126,   196,   164,  1102,  1890,  1315,   277,   105,
     279,  1108,   277,   211,   279,   213,  1316,    20,   272,   273,
     274,   275,    70,   170,   277,   279,   279,   278,    26,  1705,
      64,    79,   207,    31,  1568,  1093,    10,  1066,  1714,    87,
      26,    62,  1100,    67,  1199,    31,   272,   273,   274,   275,
    1069,   175,    64,  1082,    26,   207,   208,    81,   106,    31,
     108,    83,  1767,   159,  1093,  1215,  1216,  1217,    66,  1603,
     277,  1100,   279,   124,   122,   206,   124,   125,   126,   157,
     277,   105,   279,  1383,  1384,    82,   277,  1086,   279,   198,
     272,   273,   274,   275,   277,   146,   279,   279,   148,   277,
      86,   279,   153,  1102,  1248,   129,  1250,   137,   138,  1108,
     272,   273,   274,   275,   277,     6,   279,   279,  1652,   277,
      86,   279,   272,   273,   274,   275,  1198,   175,    66,   279,
     272,   273,   274,   275,    64,   159,   187,  1195,   276,   277,
    1198,  1199,   277,   148,   279,   269,   270,   271,   272,   273,
     274,   275,   277,   129,   279,  1595,    47,   281,   278,   283,
     272,   273,   274,   275,  1888,  1889,  1195,  1311,    59,  1198,
    1199,   129,   252,   272,   273,   274,   275,   130,  1486,  1208,
     279,  1210,   276,   277,   203,   204,   205,   277,   277,   279,
     279,   102,   673,    84,   675,  1871,   196,  1226,   679,   680,
     681,  1230,   182,   272,   273,   274,   275,  1751,   107,   180,
     279,  1755,   231,   232,   277,   163,   279,  1525,    10,  1763,
      64,   269,   270,   271,   272,   273,   274,   275,   139,  1208,
     141,  1210,   277,   281,   279,   283,    40,   262,   272,   273,
     274,   275,   277,    18,   279,   263,   262,    51,  1792,   262,
      33,    26,   215,   164,    58,  1789,    31,   277,  1413,    34,
     269,   118,   153,    32,   278,  1565,    10,    71,   278,    10,
     278,  1290,   163,   286,   272,   273,   274,   275,   189,   278,
     207,   207,   295,   278,   278,  1829,   272,   273,   274,   275,
    1834,   278,   207,   278,  1326,    70,   278,   188,  1330,    30,
     272,   273,   274,   275,    79,   278,   281,   269,   278,  1464,
     278,  1751,    87,   326,   278,  1755,   329,   278,   277,   182,
    1398,     6,   278,  1763,    66,   148,  1766,   277,   132,   283,
      66,   106,   283,   108,   278,   277,   277,   174,   269,  1647,
     283,   278,    10,   277,   148,   188,   200,   122,    94,   124,
     125,   126,  1792,    24,    94,  1413,    94,    24,    46,    10,
     182,   165,    47,   895,   216,   897,   143,   899,   900,  1398,
     902,   903,   904,   182,    59,   143,   180,  1406,  1407,  1408,
     184,    23,    94,   187,  1413,   204,   193,   193,    73,  1829,
     145,   195,   278,    95,  1834,   278,   281,   278,   278,    84,
     175,   218,   219,   220,   221,   222,   223,   224,   225,    64,
     214,   424,    10,   129,   206,   428,   429,   430,   157,   277,
     135,   128,   278,   278,   197,  1454,   279,    23,    45,   182,
     210,    63,   278,  1462,   284,  1464,    66,    66,   198,    10,
    1595,   128,   194,   235,   236,   248,   238,   278,    20,  1604,
     251,   278,  1602,   245,   246,   247,   248,   249,  1487,   251,
      46,    30,   254,   255,   256,   257,   258,    90,   153,    90,
     262,   263,    24,   132,   132,   206,   268,   102,   163,   132,
     164,   162,    46,   278,   164,   163,   499,   278,    20,   163,
     503,   278,    10,   279,   269,   270,   271,   272,   273,   274,
     275,   277,  1487,   188,   235,   236,   281,   238,   283,    66,
    1631,   277,    26,   192,   245,   246,   247,   248,   249,    17,
     251,   206,   131,   254,   255,   256,   257,   258,   277,   113,
    1574,   262,   263,    66,   277,    74,    12,   268,   247,    10,
     277,   168,    10,   278,   278,  1574,   278,   278,   200,   279,
     235,   236,   280,   238,   279,   279,   279,  1586,   186,  1632,
     245,   246,   247,   248,   249,   140,   251,    55,   238,   254,
     255,   256,   257,   258,    55,  1604,   279,   262,   263,   279,
    1592,     8,   278,   268,   278,    98,   278,   278,   278,   278,
      54,   181,   278,  1622,    73,   278,  1751,   279,  1627,   134,
    1755,   278,   278,  1632,   277,  1760,  1727,   279,  1763,   278,
     134,  1766,   278,   278,   278,   278,  1651,   278,   134,  1697,
      89,    20,  1651,    77,  1656,   137,   153,   277,  1660,   132,
     132,   132,   132,   132,   279,   132,   204,  1792,   279,    24,
    1795,   278,  1627,   279,   657,   279,   163,    54,   279,    39,
     181,    51,   276,   279,   276,    39,    86,    54,    58,   278,
     276,   674,    62,    63,   276,   678,   276,    86,  1697,  1698,
     276,    71,   685,   276,  1829,   276,   276,   690,   691,  1834,
    1699,   280,  1837,    83,   131,    10,   182,   700,   701,   142,
     703,   704,   705,   706,   707,   708,    26,    83,   188,   188,
      47,  1736,  1760,  1858,    84,    84,  1861,  1736,    47,   279,
      98,    66,   279,   131,   117,   117,   279,   279,   279,   279,
     279,  1876,    64,   279,   279,   279,   279,   197,   279,   279,
     279,  1760,   745,   279,   279,   279,   279,  1795,  1767,  1774,
    1769,   279,   276,   279,    98,  1774,    90,   277,   148,   278,
    1785,   276,   278,   766,    86,    86,  1785,   114,   771,   254,
     773,     6,   283,   278,     9,   165,  1795,   278,    13,   129,
     277,   181,  1845,   156,   279,    89,  1811,    22,   129,  1837,
     180,   129,  1811,    50,   184,   279,   279,   279,  1823,   276,
      35,   277,   277,    47,  1823,   195,    41,    42,   133,   188,
    1858,    53,    39,  1861,    50,   278,    83,  1842,  1837,     0,
    1839,   209,   279,  1842,   214,   277,  1845,   278,  1876,   279,
       0,   271,  1213,    12,    69,   274,   283,    72,   726,  1858,
    1106,  1860,  1861,   865,   642,  1103,  1092,  1866,  1506,   367,
     590,  1030,   322,  1093,   769,   349,   337,  1876,  1505,   798,
    1569,  1100,  1484,  1776,  1780,   100,   101,  1486,  1779,  1888,
    1889,  1814,   962,   714,   788,   349,  1590,   112,  1690,  1018,
    1761,   116,  1245,  1736,  1469,   844,   342,   890,   891,   892,
     354,  1299,   895,   896,   897,    13,   899,   900,   901,   902,
     903,   904,   905,   906,   335,   908,   571,   910,   627,   308,
     913,   914,   915,   916,   917,   918,   771,   836,   512,   516,
     758,   145,   745,  1715,   993,  1612,  1811,   930,     0,     0,
     933,     0,  1842,  1338,  1339,   985,   171,   524,   173,  1344,
    1345,  1144,  1347,  1348,   179,   734,  1254,  1082,  1622,    87,
    1355,  1356,  1357,  1358,   189,  1360,  1361,  1362,  1363,  1364,
    1365,  1188,  1406,    -1,   569,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    33,
      -1,   984,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    45,    69,    47,    -1,
      72,    -1,    -1,    52,    -1,    -1,    -1,  1130,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1142,
    1143,    -1,  1145,  1146,    73,    -1,    -1,    -1,   100,   101,
    1153,    -1,   206,    -1,    -1,    84,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,  1170,    -1,    -1,
      -1,    -1,     6,    -1,    -1,  1178,  1179,  1180,    -1,    -1,
      -1,   235,   236,  1186,   238,  1188,  1189,  1190,  1191,  1192,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    47,   268,    -1,    -1,  1220,    -1,   171,
      -1,   173,   151,    -1,    -1,    59,    -1,   179,    -1,   158,
      -1,    -1,   161,  1236,   163,   164,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,   203,   204,   205,   206,   207,   208,    -1,    -1,   188,
      -1,    -1,    33,    -1,  1267,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,   237,    -1,
     262,   263,    -1,  1316,    -1,    -1,   268,    -1,    -1,   153,
     272,   273,    -1,   252,   253,    -1,   278,    -1,   280,   163,
      -1,    -1,   284,    -1,  1337,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1346,    -1,    -1,    -1,    -1,    -1,    -1,
    1353,    -1,    -1,     6,   188,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1383,  1384,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1403,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    69,   251,    -1,    72,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,   112,
      -1,    -1,  1465,   116,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,   206,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,   171,    -1,
     173,   245,   246,   247,   248,   249,   179,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   189,    -1,   262,   263,
    1543,  1544,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,  1560,    -1,    -1,
      -1,    -1,  1565,    -1,   217,    -1,    -1,    -1,    -1,    -1,
    1573,    -1,  1575,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    74,    -1,    -1,     6,   268,    -1,     9,    -1,   272,
     273,    13,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,
      22,   284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    18,    -1,    -1,    -1,    41,
      42,    -1,  1655,    26,    -1,    -1,    -1,    -1,    31,    -1,
    1663,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    69,    26,    -1,
      72,    -1,    -1,    31,    -1,    -1,    34,  1690,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,   100,   101,
      -1,    -1,  1715,    -1,    87,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    70,    -1,   116,    -1,   189,    -1,    -1,    -1,
      -1,    79,    -1,   106,    -1,   108,    -1,    -1,    -1,    87,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,   106,  1762,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,   122,   238,   124,   125,   126,   171,
      -1,   173,   245,   246,   247,   248,   249,   179,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   189,    -1,   262,
     263,    -1,   175,    -1,    -1,   268,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
     163,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,     6,   268,    -1,     9,    -1,
     272,   273,    13,    -1,    -1,    -1,   278,    -1,   280,    -1,
      -1,    22,   284,    -1,    -1,    -1,   269,   270,   271,   272,
     273,   274,   275,   206,    35,    -1,   279,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,   270,   271,   272,   273,   274,   275,    -1,    -1,
      -1,    -1,   235,   236,    -1,   238,    -1,    -1,    69,    -1,
      -1,    72,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,   100,
     101,   206,    -1,     6,    -1,   278,     9,    -1,    -1,    -1,
      13,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,    69,    -1,    -1,    72,
     171,    -1,   173,   278,    -1,   280,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    69,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   171,    -1,
     173,   272,   273,    -1,    -1,    -1,   179,   278,    -1,   280,
      -1,    98,    -1,   284,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,   173,    -1,    -1,   262,
     263,    -1,    -1,    -1,     6,   268,    -1,     9,    -1,   272,
     273,    13,   189,    -1,    -1,   278,    -1,   280,    -1,    -1,
      22,   284,    -1,    -1,    -1,    -1,   203,   204,   205,    -1,
     207,   208,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,    -1,    -1,
      -1,    -1,    -1,    -1,   241,   242,    -1,    69,    -1,    -1,
      72,    -1,    -1,   250,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,    -1,   100,   101,
     206,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    69,    -1,    -1,    72,   171,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,   100,   101,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,   171,    -1,   173,
     272,   273,    -1,    -1,    -1,   179,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    13,   206,   262,   263,
      -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,    -1,
     284,    -1,    -1,    -1,    41,    42,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    60,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    69,   262,   263,    72,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    22,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    38,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    69,
     167,    -1,    72,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   167,    -1,    -1,
      -1,   268,    -1,   173,    -1,   272,   273,    -1,    -1,   179,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      13,   206,   262,   263,    -1,    -1,    -1,    -1,   268,    22,
      -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    60,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   206,    35,    -1,
      -1,   124,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    69,    -1,    -1,    72,   245,   246,   247,   248,
     249,    -1,   251,    -1,   167,   254,   255,   256,   257,   258,
     173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,
      -1,    -1,    -1,   100,   101,    -1,   189,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
     203,   204,   205,   206,   207,   208,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     167,   254,   255,   256,   257,   258,   173,    -1,    -1,   262,
     263,    -1,   179,    -1,    -1,   268,    -1,    -1,    -1,   272,
     273,    -1,   189,    -1,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    13,   206,   262,   263,    -1,    -1,    -1,
      -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    35,    -1,   284,    -1,    -1,
      -1,    41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      60,    -1,   254,   255,   256,   257,   258,    -1,    -1,    69,
     262,   263,    72,    -1,    -1,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,   235,
     236,    35,   238,    -1,   124,    -1,    -1,    41,    42,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    69,    -1,    -1,    72,    -1,
     276,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   116,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,   173,
      -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,   189,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    13,   206,   262,   263,
      -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,
     274,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,    -1,
     284,    -1,    -1,    -1,    41,    42,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    69,   262,   263,    72,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,
      -1,   204,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,
      41,    42,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,
     273,    72,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,
      -1,   268,    -1,    -1,    -1,   272,   273,    -1,   189,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    13,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    35,    -1,   284,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    37,    -1,    72,    -1,
      41,   235,   236,    44,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   100,   101,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    13,   112,    -1,
     274,    -1,   116,    -1,    85,    -1,    22,    -1,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   100,   101,    -1,    -1,   169,   203,
     204,   205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,   226,   227,   163,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,   173,   272,   273,
      -1,    -1,   243,   179,   278,    -1,   280,    -1,    -1,    -1,
     284,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   280,    -1,    -1,    -1,   284,    -1,    74,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    13,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,
     273,   148,    -1,    -1,    -1,   278,   279,   280,    -1,    35,
      -1,   284,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      -1,    -1,   189,    69,    -1,    -1,    72,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,   112,    -1,   235,   236,
     116,   238,    -1,    22,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    35,   254,   255,   256,
     257,   258,    41,    42,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,   200,    -1,    -1,   203,   204,   205,
     206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,   163,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,   173,    -1,   272,   273,    -1,    -1,
     179,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
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
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   200,    -1,
     116,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,   173,    -1,    -1,
     262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,    -1,
     272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,   114,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    13,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,
     276,    -1,   278,    -1,   280,    -1,    35,    -1,   284,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    17,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    58,    59,    -1,
      61,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
     189,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,   144,    -1,   254,   255,   256,   257,   258,
      -1,    17,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,    -1,   163,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,   178,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,   188,    54,    -1,
      -1,    -1,    58,    59,    -1,    61,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      17,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,   264,   265,   266,    -1,   268,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    58,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,   144,    44,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,   163,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,   264,   265,
     266,   178,   268,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,   188,    -1,    -1,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    19,    -1,    -1,    -1,    23,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,   152,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,   264,   265,   266,
      85,   268,    -1,   169,    -1,    -1,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,   152,    41,   235,
     236,    44,   238,    -1,    -1,    -1,    -1,   243,    -1,   245,
     246,   247,   248,   249,   169,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    -1,   191,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    91,    92,
      93,   206,    95,    96,    97,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,   119,    -1,   121,    -1,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,   243,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,   152,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,   191,    -1,
      -1,    -1,    -1,    -1,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,    -1,    -1,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,   226,   227,   245,   246,   247,   248,   249,
     250,   251,   252,    -1,   254,   255,   256,   257,   258,    -1,
     243,    -1,   262,   263,   264,   265,   266,   267,   268
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
{ lex.log_defined = false;
			  lex.cache_defined = false;
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
case 166:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 167:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 168:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 169:
{ lex.g_file  = make_file();}
    break;
case 174:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 175:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;
case 182:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 183:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 184:
{ yyval = yyvsp[0]; }
    break;
case 185:
{ yyval = yyvsp[0]; }
    break;
case 186:
{ yyval = NULL; }
    break;
case 188:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 191:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 192:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 193:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 194:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 197:
{ yyval = NULL; }
    break;
case 198:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 199:
{ yyval = yyvsp[0]; }
    break;
case 200:
{ yyval = NULL; }
    break;
case 201:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 202:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 203:
{ yyval = yyvsp[-1]; }
    break;
case 204:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 205:
{ yyval = yyvsp[0]; }
    break;
case 206:
{ yyval = NULL; }
    break;
case 210:
{ yyval = yyvsp[0]; }
    break;
case 211:
{ yyval = yyvsp[0]; }
    break;
case 212:
{ yyval = yyvsp[0]; }
    break;
case 213:
{ yyval = NULL; }
    break;
case 216:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 217:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 218:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 219:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 221:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 222:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 223:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 224:
{ yyval = yyvsp[0]; }
    break;
case 225:
{ yyval = NULL ;}
    break;
case 230:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 231:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 232:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 233:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 234:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 235:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 236:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 237:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 238:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 239:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 240:
{ yyval = NULL;}
    break;
case 241:
{ yyval = yyvsp[0];}
    break;
case 242:
{ yyval = yyvsp[0];}
    break;
case 243:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 244:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 245:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 246:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 247:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 248:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 249:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 250:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 251:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 252:
{ yyval = NULL; }
    break;
case 253:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 254:
{ yyval = NULL; }
    break;
case 256:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 257:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 259:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 260:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 261:
{ yyval = yyvsp[0]; }
    break;
case 262:
{ yyval = yyvsp[0]; }
    break;
case 263:
{ yyval = (dsql_nod*) NULL; }
    break;
case 264:
{ yyval = make_list (yyvsp[0]); }
    break;
case 265:
{ yyval = NULL; }
    break;
case 267:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 268:
{ yyval = yyvsp[-1]; }
    break;
case 271:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 272:
{ yyval = NULL; }
    break;
case 273:
{ yyval = NULL; }
    break;
case 274:
{ yyval = yyvsp[0]; }
    break;
case 275:
{ yyval = yyvsp[0]; }
    break;
case 276:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 279:
{ yyval = yyvsp[-1]; }
    break;
case 280:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 281:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 282:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 284:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 300:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 301:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 304:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 305:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 306:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 307:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 308:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 309:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 310:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 311:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 312:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 313:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 314:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 315:
{ yyval = NULL; }
    break;
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 317:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 318:
{ yyval = make_list (yyvsp[0]); }
    break;
case 319:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 320:
{ yyval = NULL; }
    break;
case 321:
{ yyval = make_list (yyvsp[0]); }
    break;
case 322:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 323:
{ yyval = NULL; }
    break;
case 326:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 327:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 328:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 329:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 330:
{ yyval = NULL; }
    break;
case 331:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 332:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 333:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 334:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 335:
{ yyval = NULL; }
    break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 338:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 341:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 342:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 343:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 344:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 348:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 349:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 350:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 351:
{ yyval = NULL; }
    break;
case 352:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 353:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 354:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 355:
{ yyval = NULL; }
    break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 358:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 359:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 360:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 361:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 362:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 363:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 364:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 365:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 366:
{ yyval = make_list (yyvsp[0]); }
    break;
case 368:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 371:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 372:
{ yyval = yyvsp[-1]; }
    break;
case 373:
{ lex.beginning = lex_position(); }
    break;
case 374:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 375:
{ lex.beginning = lex.last_token; }
    break;
case 376:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 377:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 378:
{ yyval = 0; }
    break;
case 379:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 380:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 383:
{ yyval = NULL; }
    break;
case 384:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 385:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 403:
{ yyval = NULL; }
    break;
case 404:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 405:
{ yyval = yyvsp[0]; }
    break;
case 406:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 407:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 408:
{ yyval = yyvsp[0]; }
    break;
case 409:
{ yyval = yyvsp[0]; }
    break;
case 410:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 411:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 412:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 413:
{ yyval = yyvsp[0]; }
    break;
case 414:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 416:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 417:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 418:
{ yyval = yyvsp[0]; }
    break;
case 419:
{ yyval = yyvsp[0]; }
    break;
case 420:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 421:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 422:
{ yyval = yyvsp[0]; }
    break;
case 423:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 425:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 426:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 427:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 428:
{ yyval = yyvsp[0]; }
    break;
case 429:
{ yyval = yyvsp[0]; }
    break;
case 430:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 431:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 432:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 433:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 461:
{ yyval = NULL; }
    break;
case 462:
{ yyval = NULL; }
    break;
case 463:
{ yyval = NULL; }
    break;
case 464:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 465:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 466:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 467:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 468:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 469:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 470:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 471:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 473:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 474:
{ yyval = yyvsp[0]; }
    break;
case 475:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 476:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 477:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 478:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 479:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 481:
{ yyval = NULL; }
    break;
case 483:
{ yyval = NULL; }
    break;
case 484:
{ yyval = yyvsp[0]; }
    break;
case 485:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 486:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 487:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 488:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 489:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 490:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 491:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 492:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 494:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 501:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 504:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 505:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 506:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 512:
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
case 513:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 515:
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
case 516:
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
case 517:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 524:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 526:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 527:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 528:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 529:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 545:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 546:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 547:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 548:
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
case 549:
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
case 552:
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
case 553:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 556:
{ yyval = yyvsp[-1]; }
    break;
case 557:
{ yyval = 0; }
    break;
case 561:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 562:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 563:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 567:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 568:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 569:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 570:
{ yyval = 0; }
    break;
case 571:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 574:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 575:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 578:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 579:
{ yyval = NULL; }
    break;
case 581:
{ yyval = NULL; }
    break;
case 582:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 584:
{ yyval = NULL; }
    break;
case 586:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 591:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 592:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 593:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 594:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 595:
{ yyval = yyvsp[0];}
    break;
case 597:
{ yyval = yyvsp[0];}
    break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 602:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 603:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 604:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 605:
{ yyval = 0; }
    break;
case 606:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 607:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 608:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 609:
{ yyval = (dsql_nod*) 0; }
    break;
case 610:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 611:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 614:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 615:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 616:
{ yyval = 0; }
    break;
case 618:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 619:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 620:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 621:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 622:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 623:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 624:
{ yyval = make_list (yyvsp[0]); }
    break;
case 625:
{ yyval = 0; }
    break;
case 627:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 628:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 629:
{ yyval = 0; }
    break;
case 630:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 631:
{ yyval = 0; }
    break;
case 632:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 633:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 634:
{ yyval = yyvsp[-1]; }
    break;
case 635:
{ yyval = 0; }
    break;
case 636:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 637:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 638:
{ yyval = NULL; }
    break;
case 639:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 640:
{ yyval = 0; }
    break;
case 641:
{ yyval = yyvsp[0]; }
    break;
case 642:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 643:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 644:
{ yyval = 0; }
    break;
case 645:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 646:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 647:
{ lex.limit_clause = true; }
    break;
case 648:
{ lex.limit_clause = false; }
    break;
case 649:
{ lex.first_detection = true; }
    break;
case 650:
{ lex.first_detection = false; }
    break;
case 651:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 652:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 653:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 654:
{ yyval = 0; }
    break;
case 655:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 656:
{ yyval = yyvsp[-2]; }
    break;
case 657:
{ yyval = yyvsp[-1]; }
    break;
case 658:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 659:
{ yyval = yyvsp[-1]; }
    break;
case 660:
{ yyval = yyvsp[0]; }
    break;
case 661:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 662:
{ yyval = 0; }
    break;
case 663:
{ yyval = make_list (yyvsp[0]); }
    break;
case 664:
{ yyval = 0; }
    break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 668:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 671:
{ yyval = make_list (yyvsp[0]); }
    break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 677:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 679:
{ yyval = NULL; }
    break;
case 680:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 681:
{ yyval = NULL; }
    break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 684:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 685:
{ yyval = yyvsp[-1]; }
    break;
case 686:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 687:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 688:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 689:
{ yyval = NULL; }
    break;
case 691:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 692:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 693:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 694:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 695:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 696:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 697:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 698:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 699:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
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
{ yyval = yyvsp[0]; }
    break;
case 707:
{ yyval = 0; }
    break;
case 708:
{ yyval = yyvsp[0]; }
    break;
case 709:
{ yyval = 0; }
    break;
case 710:
{ yyval = yyvsp[0]; }
    break;
case 711:
{ yyval = 0; }
    break;
case 712:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 713:
{ yyval = 0; }
    break;
case 714:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 715:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 716:
{ yyval = 0; }
    break;
case 717:
{ yyval = 0; }
    break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 720:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 723:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 724:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 725:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 726:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 728:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 729:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 730:
{ yyval = 0; }
    break;
case 731:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 732:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 735:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 736:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 737:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 740:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 741:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 743:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 744:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 748:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 749:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 750:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 752:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 756:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 757:
{ yyval = yyvsp[0]; }
    break;
case 759:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 762:
{ yyval = NULL; }
    break;
case 763:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 765:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = NULL; }
    break;
case 768:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 774:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 778:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 780:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 781:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 783:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 785:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 789:
{ yyval = yyvsp[-1]; }
    break;
case 790:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 802:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 807:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 810:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 811:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 812:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 816:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 817:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 818:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 819:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 820:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 821:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 822:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 829:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 831:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 833:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 835:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 837:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 839:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 841:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 842:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 843:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 847:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 848:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 849:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 850:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 852:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 853:
{ yyval = yyvsp[-1]; }
    break;
case 854:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 855:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 865:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 866:
{ yyval = yyvsp[0]; }
    break;
case 867:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 868:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 869:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 870:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 871:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 872:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 873:
{ yyval = yyvsp[-1]; }
    break;
case 874:
{ yyval = yyvsp[-1]; }
    break;
case 878:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 879:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 880:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 881:
{ yyval = yyvsp[0]; }
    break;
case 882:
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
case 883:
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
case 884:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 885:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 887:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 889:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 890:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 891:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 892:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 893:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 894:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 896:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 897:
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
case 898:
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
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 900:
{ yyval = make_parameter (); }
    break;
case 901:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 902:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 903:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 904:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 905:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 906:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 907:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 908:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 909:
{ yyval = yyvsp[0]; }
    break;
case 910:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 912:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 913:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 914:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 915:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 916:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 918:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 919:
{ yyval = yyvsp[0];}
    break;
case 924:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 925:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 926:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 927:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 928:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 929:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 930:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 931:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 932:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 933:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 934:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 937:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 939:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 940:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 941:
{ yyval = yyvsp[0]; }
    break;
case 942:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 943:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 944:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 945:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 948:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 949:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 952:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 953:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 954:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 955:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 956:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 957:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 958:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 959:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 963:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 964:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 965:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 966:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 967:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 968:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 973:
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
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
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

	if (!node)
		return node;

	DsqlNodStack stack;
	stack_nodes (node, stack);
	USHORT l = stack.getCount();

	dsql_nod* old  = node;
	node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	dsql_nod** ptr = node->nod_arg + node->nod_count;

	while (stack.hasData())
		*--ptr = stack.pop();

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

