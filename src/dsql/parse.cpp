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
# define	IIF	523
# define	SCALAR_ARRAY	524


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



#define	YYFINAL		1908
#define	YYFLAG		-32768
#define	YYNTBASE	287

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 524 ? yytranslate[x] : 714)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     280,   281,   276,   274,   279,   275,   285,   277,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   282,   278,
     272,   271,   273,   286,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   283,     2,   284,     2,     2,     2,     2,     2,     2,
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
     266,   267,   268,   269,   270
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
     266,   272,   273,   275,   279,   281,   285,   289,   290,   293,
     296,   298,   300,   304,   308,   311,   312,   315,   318,   320,
     324,   334,   337,   341,   349,   352,   355,   358,   361,   364,
     367,   370,   373,   376,   379,   382,   385,   388,   393,   396,
     399,   401,   402,   404,   406,   413,   420,   422,   424,   425,
     426,   428,   429,   434,   435,   437,   439,   442,   451,   453,
     454,   458,   459,   460,   462,   464,   467,   469,   471,   473,
     476,   483,   485,   487,   491,   492,   494,   496,   497,   499,
     501,   504,   508,   513,   516,   519,   523,   524,   526,   528,
     531,   533,   538,   542,   546,   548,   549,   551,   553,   556,
     560,   565,   566,   568,   571,   572,   574,   576,   582,   588,
     592,   595,   596,   598,   602,   604,   606,   613,   617,   620,
     627,   630,   632,   635,   638,   641,   642,   644,   646,   649,
     650,   653,   654,   656,   658,   660,   662,   664,   666,   667,
     669,   671,   674,   677,   680,   686,   688,   691,   695,   698,
     701,   702,   704,   706,   708,   710,   714,   719,   728,   733,
     734,   741,   743,   745,   748,   751,   752,   756,   760,   762,
     765,   768,   771,   780,   789,   798,   807,   811,   812,   817,
     818,   820,   824,   829,   831,   835,   838,   842,   846,   848,
     850,   851,   853,   856,   861,   863,   865,   869,   871,   872,
     875,   877,   884,   886,   888,   892,   894,   897,   898,   900,
     903,   906,   909,   911,   913,   915,   917,   919,   921,   924,
     926,   928,   931,   933,   935,   937,   940,   943,   945,   948,
     952,   957,   960,   967,   972,   981,   991,   998,  1007,  1014,
    1019,  1020,  1025,  1028,  1030,  1034,  1035,  1038,  1043,  1044,
    1046,  1048,  1052,  1056,  1064,  1067,  1068,  1071,  1074,  1078,
    1082,  1083,  1085,  1088,  1093,  1095,  1099,  1102,  1105,  1108,
    1110,  1112,  1114,  1116,  1119,  1122,  1128,  1129,  1134,  1142,
    1146,  1147,  1149,  1153,  1157,  1165,  1173,  1175,  1177,  1181,
    1186,  1194,  1197,  1199,  1203,  1205,  1207,  1214,  1218,  1219,
    1220,  1221,  1222,  1226,  1227,  1237,  1247,  1249,  1251,  1252,
    1255,  1257,  1259,  1261,  1263,  1265,  1269,  1273,  1277,  1281,
    1285,  1289,  1295,  1301,  1307,  1313,  1319,  1325,  1328,  1329,
    1334,  1337,  1341,  1345,  1348,  1351,  1355,  1359,  1362,  1366,
    1373,  1375,  1378,  1383,  1387,  1390,  1393,  1396,  1399,  1403,
    1405,  1409,  1413,  1417,  1420,  1423,  1428,  1433,  1439,  1441,
    1443,  1445,  1447,  1449,  1451,  1453,  1455,  1457,  1459,  1461,
    1463,  1465,  1467,  1469,  1471,  1473,  1475,  1477,  1479,  1481,
    1483,  1485,  1487,  1489,  1491,  1493,  1495,  1497,  1500,  1503,
    1506,  1508,  1510,  1512,  1513,  1516,  1519,  1520,  1522,  1525,
    1528,  1533,  1537,  1540,  1543,  1551,  1553,  1554,  1556,  1557,
    1560,  1563,  1566,  1569,  1572,  1575,  1578,  1581,  1584,  1588,
    1591,  1594,  1597,  1599,  1601,  1603,  1605,  1610,  1616,  1618,
    1622,  1624,  1628,  1630,  1633,  1635,  1637,  1639,  1641,  1643,
    1645,  1647,  1649,  1651,  1653,  1655,  1660,  1665,  1672,  1678,
    1682,  1683,  1686,  1689,  1690,  1694,  1695,  1700,  1702,  1708,
    1713,  1715,  1720,  1722,  1725,  1728,  1730,  1732,  1734,  1737,
    1740,  1743,  1746,  1747,  1751,  1757,  1759,  1761,  1764,  1768,
    1770,  1773,  1777,  1778,  1780,  1782,  1784,  1790,  1796,  1803,
    1805,  1807,  1809,  1812,  1817,  1819,  1820,  1826,  1828,  1829,
    1833,  1836,  1838,  1839,  1842,  1843,  1845,  1846,  1850,  1852,
    1853,  1855,  1858,  1860,  1862,  1864,  1866,  1869,  1872,  1874,
    1877,  1881,  1883,  1885,  1889,  1893,  1895,  1898,  1902,  1904,
    1907,  1908,  1911,  1913,  1915,  1916,  1918,  1920,  1922,  1926,
    1929,  1933,  1934,  1936,  1940,  1945,  1951,  1953,  1957,  1962,
    1966,  1967,  1969,  1973,  1977,  1979,  1981,  1982,  1984,  1986,
    1991,  1992,  1995,  2000,  2001,  2005,  2006,  2009,  2010,  2013,
    2014,  2024,  2036,  2037,  2038,  2039,  2040,  2044,  2047,  2049,
    2050,  2054,  2060,  2064,  2067,  2073,  2076,  2078,  2080,  2082,
    2084,  2086,  2090,  2092,  2096,  2098,  2099,  2102,  2104,  2108,
    2110,  2112,  2114,  2121,  2123,  2124,  2128,  2129,  2131,  2135,
    2142,  2146,  2151,  2154,  2158,  2159,  2161,  2164,  2166,  2168,
    2170,  2173,  2175,  2178,  2180,  2183,  2184,  2188,  2189,  2191,
    2195,  2197,  2200,  2201,  2204,  2205,  2208,  2209,  2214,  2216,
    2219,  2221,  2223,  2224,  2226,  2230,  2233,  2235,  2237,  2240,
    2242,  2247,  2251,  2253,  2257,  2262,  2263,  2272,  2278,  2280,
    2282,  2287,  2292,  2297,  2299,  2301,  2307,  2313,  2315,  2319,
    2323,  2331,  2339,  2345,  2349,  2350,  2352,  2354,  2356,  2359,
    2360,  2362,  2364,  2366,  2367,  2371,  2373,  2377,  2379,  2380,
    2384,  2386,  2390,  2392,  2396,  2400,  2402,  2404,  2408,  2410,
    2413,  2415,  2419,  2423,  2425,  2428,  2432,  2436,  2440,  2444,
    2446,  2450,  2453,  2455,  2457,  2459,  2461,  2463,  2465,  2467,
    2469,  2471,  2473,  2477,  2481,  2485,  2489,  2493,  2497,  2501,
    2505,  2512,  2519,  2526,  2533,  2540,  2547,  2554,  2561,  2568,
    2575,  2582,  2589,  2596,  2603,  2610,  2617,  2619,  2621,  2627,
    2634,  2638,  2643,  2649,  2656,  2660,  2665,  2669,  2674,  2678,
    2683,  2688,  2694,  2699,  2704,  2708,  2713,  2715,  2717,  2719,
    2721,  2723,  2725,  2727,  2731,  2735,  2747,  2759,  2761,  2763,
    2765,  2767,  2769,  2771,  2773,  2775,  2777,  2780,  2783,  2787,
    2791,  2795,  2799,  2803,  2807,  2811,  2815,  2817,  2819,  2821,
    2823,  2827,  2829,  2831,  2833,  2835,  2837,  2839,  2844,  2846,
    2850,  2852,  2855,  2857,  2859,  2861,  2863,  2865,  2867,  2869,
    2872,  2875,  2878,  2880,  2882,  2884,  2886,  2888,  2890,  2892,
    2894,  2896,  2898,  2901,  2903,  2906,  2908,  2910,  2912,  2914,
    2916,  2919,  2921,  2923,  2925,  2927,  2929,  2934,  2940,  2946,
    2952,  2958,  2964,  2970,  2976,  2982,  2988,  2994,  3001,  3003,
    3010,  3012,  3017,  3025,  3028,  3029,  3034,  3038,  3045,  3047,
    3049,  3056,  3065,  3072,  3074,  3076,  3081,  3088,  3093,  3099,
    3103,  3109,  3114,  3120,  3122,  3124,  3126,  3128,  3130,  3132,
    3134,  3136,  3138,  3140,  3142,  3144,  3145,  3147,  3149,  3151,
    3153,  3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,
    3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,
    3193,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229,  3231,
    3233,  3235
};
static const short yyrhs[] =
{
     288,     0,   288,   278,     0,   472,     0,   611,     0,   525,
       0,   320,     0,   308,     0,   602,     0,   493,     0,   289,
       0,   601,     0,   445,     0,   446,     0,   322,     0,   324,
       0,   298,     0,   526,     0,   519,     0,   548,     0,   517,
       0,   606,     0,    43,   658,     0,    73,   291,   129,   290,
     182,   302,   295,     0,    73,   293,   129,   148,   297,   182,
     302,   295,     0,    73,   291,   129,   290,   182,   300,     0,
      73,   293,   129,   148,   297,   182,   300,     0,    73,   304,
     182,   306,   296,     0,   585,     0,   180,   585,     0,     6,
       0,     6,   147,     0,   292,     0,   294,     0,   292,   279,
     294,     0,    59,     0,   163,     0,    84,     0,    47,     0,
     188,   616,     0,   153,   616,     0,   200,    73,   131,     0,
       0,   200,   210,   131,     0,     0,   703,     0,   158,   299,
     291,   129,   290,    66,   302,     0,   158,   299,   293,   129,
     148,   297,    66,   302,     0,   158,   291,   129,   290,    66,
     302,     0,   158,   293,   129,   148,   297,    66,   302,     0,
     158,   291,   129,   290,    66,   300,     0,   158,   293,   129,
     148,   297,    66,   300,     0,   158,   304,    66,   306,     0,
      73,   131,    64,     0,   301,     0,   300,   279,   301,     0,
     300,   279,   303,     0,   302,   279,   301,     0,   148,   703,
       0,   184,   707,     0,   195,   710,     0,   214,   704,     0,
     303,     0,   302,   279,   303,     0,   708,     0,   189,   708,
       0,    74,   708,     0,   305,     0,   304,   279,   305,     0,
     704,     0,   307,     0,   306,   279,   307,     0,   708,     0,
     189,   708,     0,    45,   309,     0,    63,   319,     0,    62,
      68,   310,     0,   688,   312,   157,   316,    55,   657,   117,
     657,     0,   500,     0,    19,     0,    37,   280,   661,   281,
     506,     0,     0,   313,     0,   280,   313,   281,     0,   314,
       0,   313,   279,   314,     0,   374,   311,   315,     0,     0,
      20,   238,     0,    20,   270,     0,    98,     0,   317,     0,
     280,   317,   281,     0,   374,   311,   318,     0,   140,   661,
       0,     0,    20,   101,     0,    20,   238,     0,   212,     0,
      20,   238,   212,     0,   697,    82,   505,   135,   505,    55,
     657,   117,   657,     0,    36,   321,     0,    58,   696,   657,
       0,   326,   553,    83,   700,   129,   585,   327,     0,   148,
     393,     0,   180,   361,     0,   184,   464,     0,   195,   450,
       0,    71,   343,     0,    40,   345,     0,    51,   334,     0,
     165,   328,     0,   214,   344,     0,   237,   323,     0,   148,
     394,     0,   180,   362,     0,   195,   451,     0,    36,   132,
       7,   325,     0,   148,   395,     0,   184,   465,     0,   187,
       0,     0,   618,     0,   617,     0,   368,   280,   461,   646,
     462,   281,     0,   661,   329,   330,   657,   331,   332,     0,
     111,     0,    14,     0,     0,     0,    32,     0,     0,   102,
     346,   664,   360,     0,     0,   333,     0,   354,     0,   333,
     354,     0,   371,   335,   495,   461,   336,   462,   337,   370,
       0,    10,     0,     0,    46,   461,   376,     0,     0,     0,
     338,     0,   339,     0,   338,   339,     0,   340,     0,   341,
       0,   342,     0,   124,    98,     0,   461,    24,   280,   625,
     281,   462,     0,   699,     0,   704,     0,   347,   348,   351,
       0,     0,   271,     0,   657,     0,     0,   349,     0,   350,
       0,   349,   350,     0,   139,   346,   661,     0,   102,   346,
     664,   360,     0,   189,   657,     0,   141,   657,     0,   164,
     118,   657,     0,     0,   352,     0,   353,     0,   352,   353,
       0,   354,     0,    46,    23,   164,   690,     0,   263,    94,
     657,     0,   355,   657,   356,     0,    94,     0,     0,   357,
       0,   358,     0,   357,   358,     0,   175,   359,   664,     0,
     102,   346,   664,   360,     0,     0,    12,     0,    12,   137,
       0,     0,   137,     0,   138,     0,   585,   363,   280,   364,
     281,     0,   585,   363,   280,   364,   281,     0,    62,    94,
     657,     0,    62,   657,     0,     0,   365,     0,   364,   279,
     365,     0,   366,     0,   381,     0,   371,   369,   375,   462,
     377,   370,     0,   371,   496,   367,     0,   371,   367,     0,
     368,   280,   461,   646,   462,   281,     0,    30,    20,     0,
      30,     0,   495,   461,     0,   623,   459,     0,    26,   691,
       0,     0,   623,     0,   623,     0,   374,   495,     0,     0,
      46,   376,     0,     0,   650,     0,   654,     0,   655,     0,
     656,     0,   687,     0,   647,     0,     0,   378,     0,   379,
       0,   378,   379,     0,   382,   380,     0,   124,    98,     0,
     153,   585,   616,   389,   387,     0,   388,     0,   187,   387,
       0,   146,    90,   387,     0,   382,   383,     0,    33,   693,
       0,     0,   384,     0,   385,     0,   386,     0,   388,     0,
     187,   617,   387,     0,   146,    90,   617,   387,     0,    65,
      90,   617,   153,   585,   616,   389,   387,     0,   247,   553,
      83,   700,     0,     0,   461,    24,   280,   625,   281,   462,
       0,   390,     0,   391,     0,   391,   390,     0,   390,   391,
       0,     0,   129,   188,   392,     0,   129,    47,   392,     0,
     211,     0,   164,    46,     0,   164,    98,     0,   123,   209,
       0,   703,   397,   398,    10,   459,   404,   413,   462,     0,
     703,   397,   398,    10,   459,   404,   413,   462,     0,   703,
     397,   398,    10,   459,   404,   413,   462,     0,   703,   397,
     398,    10,   459,   404,   413,   462,     0,   280,   399,   281,
       0,     0,   157,   280,   401,   281,     0,     0,   400,     0,
     399,   279,   400,     0,   372,   496,   403,   462,     0,   402,
       0,   401,   279,   402,     0,   372,   496,     0,    46,   459,
     376,     0,   271,   459,   376,     0,   459,     0,   405,     0,
       0,   406,     0,   405,   406,     0,    45,   409,   407,   278,
       0,   408,     0,   411,     0,   371,   496,   410,     0,   192,
       0,     0,   271,   376,     0,   375,     0,   694,    39,    64,
     280,   561,   281,     0,   416,     0,   413,     0,    17,   414,
      54,     0,   415,     0,   415,   436,     0,     0,   412,     0,
     415,   412,     0,   610,   278,     0,   602,   278,     0,   417,
       0,   418,     0,   419,     0,   420,     0,   421,     0,   424,
       0,   601,   278,     0,   425,     0,   427,     0,   606,   278,
       0,   432,     0,   422,     0,   423,     0,   178,   278,     0,
      61,   278,     0,   434,     0,   440,   278,     0,    58,   696,
     278,     0,    58,   696,   646,   278,     0,    58,   278,     0,
      59,   148,   703,   429,   430,   278,     0,    59,   254,   646,
     278,     0,   433,    64,   548,    86,   431,   435,    50,   412,
       0,   433,    64,    59,   254,   646,    86,   431,    50,   412,
       0,    59,   254,   646,    86,   431,   278,     0,    78,   280,
     625,   281,   181,   412,    53,   412,     0,    78,   280,   625,
     281,   181,   412,     0,   144,   646,   426,   278,     0,     0,
     548,    86,   431,   278,     0,   282,   709,     0,   649,     0,
     280,   649,   281,     0,     0,   156,   431,     0,   156,   280,
     431,   281,     0,     0,   428,     0,   622,     0,   431,   279,
     622,     0,   431,   279,   428,     0,   433,   199,   280,   625,
     281,    50,   412,     0,   702,   282,     0,     0,   235,   278,
       0,   255,   278,     0,   255,   702,   278,     0,    10,    39,
     694,     0,     0,   437,     0,   436,   437,     0,   197,   438,
      50,   412,     0,   439,     0,   438,   279,   439,     0,   173,
     658,     0,    69,   698,     0,    58,   696,     0,     9,     0,
     441,     0,   443,     0,   442,     0,   264,   694,     0,   265,
     694,     0,   266,   444,   694,    86,   431,     0,     0,    59,
     148,   703,   429,     0,    59,   268,   447,   398,    10,   404,
     413,     0,   280,   448,   281,     0,     0,   449,     0,   448,
     279,   449,     0,   402,   271,   653,     0,   710,   616,    10,
     459,   452,   463,   460,     0,   710,   616,    10,   459,   452,
     463,   460,     0,   453,     0,   454,     0,   453,   186,   454,
       0,   453,   186,     6,   454,     0,   163,   569,   570,   455,
     591,   587,   590,     0,    66,   456,     0,   457,     0,   456,
     279,   457,     0,   458,     0,   584,     0,   457,   586,    89,
     457,   129,   625,     0,   280,   458,   281,     0,     0,     0,
       0,     0,   200,    24,   131,     0,     0,   707,    64,   585,
     466,   467,   470,   461,   471,   462,     0,   707,    64,   585,
     466,   467,   470,   461,   471,   462,     0,     3,     0,    80,
       0,     0,   468,   469,     0,    16,     0,     5,     0,    84,
       0,   188,     0,    47,     0,    84,   132,   188,     0,    84,
     132,    47,     0,   188,   132,    84,     0,   188,   132,    47,
       0,    47,   132,    84,     0,    47,   132,   188,     0,    84,
     132,   188,   132,    47,     0,    84,   132,    47,   132,   188,
       0,   188,   132,    84,   132,    47,     0,   188,   132,    47,
     132,    84,     0,    47,   132,    84,   132,   188,     0,    47,
     132,   188,   132,    84,     0,   143,   659,     0,     0,    10,
     461,   404,   413,     0,     7,   473,     0,    58,   696,   657,
       0,   180,   585,   478,     0,   184,   490,     0,   148,   396,
       0,    40,   487,   488,     0,    51,   480,   476,     0,    83,
     486,     0,    46,   461,   376,     0,    24,   461,   280,   625,
     281,   462,     0,   477,     0,   476,   477,     0,   164,   461,
     474,   462,     0,     4,    33,   475,     0,     4,   342,     0,
      52,    46,     0,    52,    33,     0,   182,   623,     0,   216,
     374,   496,     0,   479,     0,   478,   279,   479,     0,    52,
     623,   485,     0,    52,    33,   693,     0,     4,   366,     0,
       4,   381,     0,   482,   623,   143,   661,     0,   482,   480,
     182,   623,     0,   482,   484,   216,   483,   462,     0,   481,
       0,   712,     0,   215,     0,   216,     0,   217,     0,   218,
       0,   219,     0,   220,     0,   221,     0,   222,     0,   223,
       0,   224,     0,   225,     0,   226,     0,   227,     0,   228,
       0,   229,     0,   230,     0,   233,     0,   234,     0,   241,
       0,   242,     0,   250,     0,   252,     0,   264,     0,   265,
       0,   266,     0,   267,     0,     7,     0,     7,   215,     0,
     496,   461,     0,   623,   459,     0,   623,     0,   213,     0,
     211,     0,     0,   700,     3,     0,   700,    80,     0,     0,
     489,     0,   488,   489,     0,     4,   333,     0,     4,   263,
      94,   657,     0,    52,   263,    94,     0,    17,   262,     0,
      54,   262,     0,   707,   466,   491,   470,   461,   492,   462,
       0,   467,     0,     0,   471,     0,     0,    52,   494,     0,
      58,   696,     0,    83,   700,     0,   148,   703,     0,   180,
     706,     0,   184,   707,     0,   195,   710,     0,    63,   697,
       0,    51,   695,     0,    62,    68,   688,     0,   165,   661,
       0,   214,   704,     0,    71,   699,     0,   496,     0,   497,
       0,   500,     0,   503,     0,   501,   283,   498,   284,     0,
     508,   283,   498,   284,   506,     0,   499,     0,   498,   279,
     499,     0,   663,     0,   663,   282,   663,     0,   501,     0,
     508,   506,     0,   507,     0,   512,     0,   515,     0,   243,
       0,   502,     0,   169,     0,    41,     0,   226,     0,   227,
       0,    85,     0,    96,     0,    19,   505,   504,   506,     0,
      19,   280,   662,   281,     0,    19,   280,   662,   279,   658,
     281,     0,    19,   280,   279,   658,   281,     0,   162,   168,
     662,     0,     0,   177,   658,     0,   177,   689,     0,     0,
      23,   164,   690,     0,     0,   511,   280,   661,   281,     0,
     511,     0,   511,   193,   280,   661,   281,     0,   510,   280,
     661,   281,     0,   510,     0,   509,   280,   661,   281,     0,
     191,     0,    23,   193,     0,    91,   193,     0,    23,     0,
      91,     0,   121,     0,   119,    23,     0,   119,    91,     0,
      99,   513,     0,   514,   513,     0,     0,   280,   663,   281,
       0,   280,   663,   279,   663,   281,     0,    44,     0,    92,
       0,    95,   516,     0,    97,    95,   516,     0,   152,     0,
      93,   145,     0,   280,   659,   281,     0,     0,   530,     0,
     518,     0,   547,     0,   164,    71,   699,   182,   663,     0,
     164,    71,   699,   182,   231,     0,   164,    71,   699,   182,
     275,   231,     0,   520,     0,   521,     0,   523,     0,   252,
     711,     0,   253,   252,   711,   522,     0,   130,     0,     0,
     161,   527,   182,   524,   711,     0,   252,     0,     0,    28,
     527,   528,     0,   161,   527,     0,   201,     0,     0,   155,
     529,     0,     0,   170,     0,     0,   164,   183,   531,     0,
     532,     0,     0,   533,     0,   532,   533,     0,   534,     0,
     535,     0,   536,     0,   540,     0,   151,   130,     0,   151,
     202,     0,   196,     0,   123,   196,     0,    88,   107,   537,
       0,   537,     0,   538,     0,   151,   185,   539,     0,   151,
      29,   539,     0,   170,     0,   170,   180,     0,   170,   180,
     174,     0,   194,     0,   123,   194,     0,     0,   154,   543,
       0,   166,     0,   149,     0,     0,   151,     0,   202,     0,
     544,     0,   543,   279,   544,     0,   546,   545,     0,    64,
     541,   542,     0,     0,   585,     0,   546,   279,   585,     0,
     164,   176,    83,   700,     0,   549,   550,   556,   557,   559,
       0,   560,     0,   549,   186,   560,     0,   549,   186,     6,
     560,     0,   133,    20,   551,     0,     0,   552,     0,   551,
     279,   552,     0,   646,   553,   555,     0,    11,     0,    48,
       0,     0,   239,     0,   249,     0,   248,   564,   554,   565,
       0,     0,   267,   646,     0,   267,   646,   182,   646,     0,
       0,    64,   188,   558,     0,     0,   128,   618,     0,     0,
     200,   251,     0,     0,   163,   566,   569,   570,   574,   591,
     587,   590,   592,     0,   163,   566,   569,   570,   574,   591,
     587,   590,   592,   550,   556,     0,     0,     0,     0,     0,
     567,   568,   563,     0,   567,   563,     0,   568,     0,     0,
     239,   664,   562,     0,   239,   280,   646,   281,   562,     0,
     239,   653,   562,     0,   240,   664,     0,   240,   280,   563,
     646,   281,     0,   240,   653,     0,    49,     0,   686,     0,
     571,     0,   276,     0,   572,     0,   571,   279,   572,     0,
     646,     0,   646,   573,   701,     0,    10,     0,     0,    66,
     575,     0,   576,     0,   575,   279,   576,     0,   581,     0,
     582,     0,   577,     0,   280,   548,   281,   573,   578,   579,
       0,   705,     0,     0,   280,   580,   281,     0,     0,   701,
       0,   580,   279,   701,     0,   576,   586,    89,   576,   129,
     625,     0,   280,   581,   281,     0,   703,   583,   573,   705,
       0,   703,   583,     0,   280,   649,   281,     0,     0,   585,
       0,   706,   705,     0,   706,     0,    81,     0,   105,     0,
     105,   134,     0,   159,     0,   159,   134,     0,    67,     0,
      67,   134,     0,     0,    74,    20,   588,     0,     0,   589,
       0,   588,   279,   589,     0,   646,     0,    77,   625,     0,
       0,   198,   625,     0,     0,   142,   593,     0,     0,   594,
     280,   595,   281,     0,    89,     0,   172,   114,     0,   114,
       0,   172,     0,     0,   596,     0,   596,   279,   595,     0,
     597,   598,     0,   593,     0,   706,     0,   706,   597,     0,
     120,     0,    83,   280,   599,   281,     0,   133,   700,   600,
       0,   700,     0,   700,   279,   599,     0,    83,   280,   599,
     281,     0,     0,    84,    86,   585,   619,   190,   280,   649,
     281,     0,    84,    86,   585,   619,   561,     0,   603,     0,
     604,     0,    47,    66,   584,   591,     0,    47,    66,   584,
     605,     0,   198,    38,   128,   694,     0,   607,     0,   608,
       0,   188,   584,   164,   609,   591,     0,   188,   584,   164,
     609,   605,     0,   610,     0,   609,   279,   610,     0,   624,
     271,   646,     0,   151,    19,   623,    66,   585,   612,   614,
       0,    84,    19,   623,    86,   585,   612,   614,     0,    63,
      66,   613,   182,   613,     0,    63,   182,   613,     0,     0,
     505,     0,   653,     0,   658,     0,   113,   615,     0,     0,
     662,     0,   653,     0,   617,     0,     0,   280,   618,   281,
       0,   623,     0,   618,   279,   623,     0,   620,     0,     0,
     280,   621,   281,     0,   624,     0,   621,   279,   624,     0,
     623,     0,   705,   285,   692,     0,   705,   285,   276,     0,
     692,     0,   623,     0,   705,   285,   692,     0,   640,     0,
     124,   640,     0,   627,     0,   625,   132,   625,     0,   625,
       8,   625,     0,   627,     0,   124,   640,     0,   626,   132,
     625,     0,   626,     8,   625,     0,   641,   132,   625,     0,
     641,     8,   625,     0,   628,     0,   280,   626,   281,     0,
     124,   627,     0,   629,     0,   632,     0,   633,     0,   634,
       0,   639,     0,   630,     0,   637,     0,   635,     0,   636,
       0,   638,     0,   646,   271,   646,     0,   646,   272,   646,
       0,   646,   273,   646,     0,   646,    70,   646,     0,   646,
     106,   646,     0,   646,   125,   646,     0,   646,   126,   646,
       0,   646,   122,   646,     0,   646,   271,     6,   280,   644,
     281,     0,   646,   272,     6,   280,   644,   281,     0,   646,
     273,     6,   280,   644,   281,     0,   646,    70,     6,   280,
     644,   281,     0,   646,   106,     6,   280,   644,   281,     0,
     646,   125,     6,   280,   644,   281,     0,   646,   126,     6,
     280,   644,   281,     0,   646,   122,     6,   280,   644,   281,
       0,   646,   271,   631,   280,   644,   281,     0,   646,   272,
     631,   280,   644,   281,     0,   646,   273,   631,   280,   644,
     281,     0,   646,    70,   631,   280,   644,   281,     0,   646,
     106,   631,   280,   644,   281,     0,   646,   125,   631,   280,
     644,   281,     0,   646,   126,   631,   280,   644,   281,     0,
     646,   122,   631,   280,   644,   281,     0,   171,     0,     9,
       0,   646,    18,   646,     8,   646,     0,   646,   124,    18,
     646,     8,   646,     0,   646,   108,   646,     0,   646,   124,
     108,   646,     0,   646,   108,   646,    57,   646,     0,   646,
     124,   108,   646,    57,   646,     0,   646,    79,   642,     0,
     646,   124,    79,   642,     0,   646,    34,   646,     0,   646,
     124,    34,   646,     0,   646,   175,   646,     0,   646,   124,
     175,   646,     0,   646,   175,   200,   646,     0,   646,   124,
     175,   200,   646,     0,    60,   280,   561,   281,     0,   167,
     280,   561,   281,     0,   646,    87,    98,     0,   646,    87,
     124,    98,     0,   256,     0,   257,     0,   258,     0,   259,
       0,   260,     0,   261,     0,   643,     0,   280,   649,   281,
       0,   280,   644,   281,     0,   163,   566,   569,   646,   574,
     591,   587,   590,   592,   550,   556,     0,   163,   566,   569,
     646,   574,   591,   587,   590,   592,   550,   556,     0,   622,
       0,   648,     0,   665,     0,   652,     0,   653,     0,   428,
       0,   674,     0,   675,     0,   673,     0,   275,   646,     0,
     274,   646,     0,   646,   274,   646,     0,   646,    31,   646,
       0,   646,    26,   691,     0,   646,   275,   646,     0,   646,
     276,   646,     0,   646,   277,   646,     0,   280,   646,   281,
       0,   280,   645,   281,     0,   654,     0,   655,     0,   656,
       0,    42,     0,   705,   285,    42,     0,   101,     0,   647,
       0,   687,     0,   228,     0,   229,     0,   230,     0,   622,
     283,   649,   284,     0,   646,     0,   649,   279,   646,     0,
     652,     0,   275,   651,     0,   205,     0,   204,     0,   203,
       0,   231,     0,   232,     0,   651,     0,   657,     0,    41,
     207,     0,   226,   207,     0,   227,   207,     0,   286,     0,
     189,     0,   233,     0,   234,     0,   241,     0,   242,     0,
      69,     0,   173,     0,   250,     0,   207,     0,   208,   207,
       0,   659,     0,   275,   660,     0,   204,     0,   204,     0,
     659,     0,   204,     0,   664,     0,   275,   664,     0,   204,
       0,   666,     0,   667,     0,   668,     0,   670,     0,    35,
     280,   276,   281,     0,    35,   280,   686,   646,   281,     0,
      35,   280,    49,   646,   281,     0,   179,   280,   686,   646,
     281,     0,   179,   280,    49,   646,   281,     0,    13,   280,
     686,   646,   281,     0,    13,   280,    49,   646,   281,     0,
     116,   280,   686,   646,   281,     0,   116,   280,    49,   646,
     281,     0,   112,   280,   686,   646,   281,     0,   112,   280,
      49,   646,   281,     0,    72,   280,   699,   279,   646,   281,
       0,   669,     0,   217,   280,   685,    66,   646,   281,     0,
     671,     0,   100,   280,   646,   281,     0,   236,   280,   646,
      66,   646,   672,   281,     0,    64,   646,     0,     0,   688,
     280,   649,   281,     0,   688,   280,   281,     0,    22,   280,
     646,    10,   373,   281,     0,   676,     0,   677,     0,   245,
     280,   646,   279,   646,   281,     0,   269,   280,   625,   279,
     646,   279,   646,   281,     0,   246,   280,   646,   279,   649,
     281,     0,   678,     0,   680,     0,   244,   683,   679,    54,
       0,   244,   683,   679,    53,   684,    54,     0,   197,   682,
     181,   684,     0,   679,   197,   682,   181,   684,     0,   244,
     681,    54,     0,   244,   681,    53,   684,    54,     0,   197,
     625,   181,   684,     0,   681,   197,   625,   181,   684,     0,
     646,     0,   646,     0,   646,     0,   218,     0,   219,     0,
     220,     0,   221,     0,   222,     0,   223,     0,   224,     0,
     225,     0,     6,     0,     0,    98,     0,   206,     0,   712,
       0,   712,     0,   712,     0,   712,     0,   712,     0,   712,
       0,   712,     0,   712,     0,   712,     0,   712,     0,   712,
       0,   712,     0,   712,     0,   712,     0,   712,     0,   712,
       0,   712,     0,   712,     0,   712,     0,   712,     0,   712,
       0,   712,     0,   712,     0,   206,     0,   713,     0,   235,
       0,   238,     0,   236,     0,   246,     0,   249,     0,   255,
       0,   251,     0,   245,     0,   248,     0,   254,     0,   247,
       0,   256,     0,   257,     0,   258,     0,   268,     0,   262,
       0,   263,     0,   269,     0,   270,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   540,   542,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   573,   577,   581,   585,   589,   594,   595,
     599,   601,   603,   607,   608,   612,   616,   618,   620,   622,
     624,   628,   630,   634,   636,   640,   647,   652,   657,   662,
     667,   672,   677,   683,   687,   688,   690,   692,   696,   698,
     700,   702,   706,   707,   714,   716,   718,   722,   723,   727,
     731,   732,   736,   738,   745,   749,   751,   756,   762,   763,
     765,   771,   773,   774,   778,   779,   785,   790,   792,   794,
     796,   800,   801,   805,   808,   813,   815,   817,   819,   822,
     827,   836,   840,   843,   846,   848,   850,   852,   854,   856,
     858,   860,   862,   867,   871,   873,   875,   885,   889,   891,
     902,   904,   908,   910,   911,   917,   923,   925,   927,   931,
     933,   937,   939,   943,   945,   948,   949,   956,   974,   976,
     980,   982,   986,   988,   991,   992,   995,  1000,  1001,  1004,
    1008,  1017,  1025,  1034,  1039,  1040,  1043,  1049,  1051,  1054,
    1055,  1059,  1061,  1063,  1065,  1067,  1071,  1073,  1076,  1077,
    1081,  1086,  1088,  1137,  1194,  1198,  1199,  1202,  1203,  1206,
    1208,  1212,  1213,  1214,  1217,  1218,  1219,  1225,  1230,  1235,
    1237,  1239,  1243,  1244,  1248,  1249,  1256,  1260,  1264,  1273,
    1279,  1280,  1283,  1285,  1290,  1292,  1297,  1303,  1309,  1313,
    1319,  1321,  1325,  1330,  1331,  1332,  1333,  1334,  1337,  1339,
    1342,  1343,  1347,  1352,  1354,  1359,  1360,  1362,  1370,  1374,
    1376,  1379,  1380,  1381,  1382,  1385,  1389,  1393,  1400,  1407,
    1412,  1418,  1421,  1423,  1425,  1427,  1431,  1434,  1438,  1441,
    1444,  1447,  1456,  1467,  1477,  1487,  1497,  1499,  1503,  1505,
    1509,  1510,  1514,  1520,  1521,  1525,  1530,  1532,  1534,  1538,
    1540,  1544,  1545,  1549,  1553,  1554,  1557,  1562,  1564,  1568,
    1570,  1574,  1579,  1580,  1583,  1587,  1589,  1591,  1595,  1596,
    1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,
    1610,  1611,  1612,  1613,  1614,  1615,  1617,  1619,  1620,  1623,
    1625,  1629,  1633,  1638,  1642,  1647,  1651,  1655,  1657,  1661,
    1665,  1671,  1676,  1681,  1683,  1685,  1689,  1691,  1693,  1697,
    1698,  1699,  1701,  1705,  1709,  1711,  1715,  1717,  1719,  1724,
    1726,  1730,  1731,  1735,  1740,  1741,  1745,  1747,  1749,  1751,
    1755,  1756,  1757,  1760,  1764,  1768,  1772,  1814,  1821,  1829,
    1831,  1835,  1836,  1840,  1846,  1853,  1873,  1877,  1879,  1881,
    1885,  1896,  1900,  1901,  1905,  1906,  1914,  1917,  1923,  1927,
    1933,  1937,  1943,  1947,  1955,  1966,  1977,  1979,  1981,  1985,
    1989,  1991,  1995,  1997,  1999,  2001,  2003,  2005,  2007,  2009,
    2011,  2013,  2015,  2017,  2019,  2021,  2023,  2027,  2029,  2033,
    2039,  2043,  2046,  2053,  2055,  2057,  2060,  2063,  2068,  2072,
    2078,  2079,  2083,  2092,  2096,  2098,  2100,  2102,  2104,  2108,
    2109,  2113,  2115,  2117,  2119,  2125,  2128,  2130,  2134,  2142,
    2143,  2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,
    2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,
    2163,  2164,  2165,  2166,  2167,  2168,  2171,  2173,  2177,  2179,
    2184,  2190,  2192,  2194,  2198,  2200,  2207,  2213,  2214,  2218,
    2232,  2234,  2236,  2238,  2252,  2262,  2263,  2267,  2268,  2274,
    2278,  2280,  2282,  2284,  2286,  2288,  2290,  2292,  2294,  2296,
    2298,  2300,  2307,  2308,  2311,  2312,  2315,  2320,  2327,  2328,
    2332,  2341,  2347,  2348,  2351,  2352,  2353,  2354,  2371,  2376,
    2381,  2400,  2417,  2424,  2425,  2432,  2436,  2442,  2448,  2456,
    2460,  2466,  2470,  2474,  2480,  2484,  2491,  2497,  2503,  2511,
    2516,  2521,  2528,  2529,  2530,  2533,  2534,  2537,  2538,  2539,
    2546,  2550,  2561,  2567,  2621,  2683,  2684,  2691,  2704,  2709,
    2714,  2721,  2723,  2730,  2731,  2732,  2736,  2741,  2746,  2757,
    2758,  2759,  2762,  2766,  2770,  2772,  2776,  2780,  2781,  2784,
    2788,  2792,  2793,  2796,  2798,  2802,  2803,  2807,  2811,  2812,
    2816,  2817,  2821,  2822,  2823,  2824,  2827,  2829,  2833,  2835,
    2839,  2841,  2844,  2846,  2848,  2852,  2854,  2856,  2860,  2862,
    2864,  2868,  2872,  2874,  2876,  2880,  2882,  2886,  2887,  2891,
    2895,  2897,  2901,  2902,  2907,  2915,  2919,  2921,  2923,  2927,
    2929,  2933,  2934,  2938,  2942,  2944,  2946,  2950,  2952,  2956,
    2958,  2962,  2965,  2973,  2977,  2979,  2983,  2985,  2989,  2991,
    2998,  3010,  3024,  3028,  3032,  3036,  3040,  3042,  3044,  3046,
    3050,  3052,  3054,  3058,  3060,  3062,  3066,  3068,  3072,  3074,
    3078,  3079,  3083,  3084,  3088,  3089,  3094,  3098,  3099,  3103,
    3104,  3105,  3109,  3114,  3115,  3119,  3121,  3125,  3126,  3130,
    3132,  3136,  3139,  3144,  3146,  3150,  3151,  3156,  3161,  3163,
    3165,  3167,  3169,  3171,  3173,  3175,  3182,  3184,  3188,  3189,
    3195,  3198,  3200,  3204,  3206,  3213,  3215,  3219,  3223,  3225,
    3227,  3233,  3235,  3239,  3240,  3244,  3246,  3249,  3250,  3254,
    3256,  3258,  3262,  3263,  3267,  3269,  3276,  3279,  3286,  3287,
    3290,  3294,  3298,  3305,  3306,  3309,  3314,  3319,  3320,  3324,
    3331,  3333,  3337,  3339,  3341,  3344,  3345,  3348,  3352,  3354,
    3357,  3359,  3365,  3366,  3370,  3374,  3375,  3380,  3381,  3385,
    3389,  3390,  3395,  3396,  3399,  3404,  3409,  3412,  3419,  3420,
    3422,  3423,  3425,  3429,  3430,  3432,  3434,  3438,  3440,  3444,
    3445,  3447,  3451,  3452,  3453,  3454,  3455,  3456,  3457,  3458,
    3459,  3460,  3465,  3467,  3469,  3471,  3473,  3475,  3477,  3479,
    3486,  3488,  3490,  3492,  3494,  3496,  3498,  3500,  3502,  3504,
    3506,  3508,  3510,  3512,  3514,  3516,  3520,  3521,  3527,  3529,
    3534,  3536,  3538,  3540,  3545,  3547,  3551,  3553,  3557,  3559,
    3561,  3563,  3567,  3571,  3575,  3577,  3581,  3586,  3591,  3598,
    3603,  3608,  3617,  3618,  3622,  3626,  3640,  3658,  3659,  3660,
    3661,  3662,  3663,  3664,  3665,  3666,  3667,  3669,  3671,  3678,
    3680,  3682,  3689,  3696,  3703,  3705,  3707,  3708,  3709,  3710,
    3712,  3714,  3718,  3719,  3722,  3738,  3754,  3758,  3762,  3763,
    3767,  3768,  3772,  3774,  3776,  3778,  3780,  3784,  3785,  3787,
    3803,  3819,  3823,  3827,  3829,  3833,  3837,  3840,  3843,  3846,
    3849,  3854,  3856,  3861,  3862,  3866,  3873,  3880,  3887,  3894,
    3895,  3899,  3905,  3906,  3907,  3908,  3911,  3913,  3915,  3918,
    3925,  3934,  3941,  3950,  3952,  3954,  3956,  3962,  3971,  3974,
    3978,  3979,  3983,  3992,  3994,  3998,  4000,  4004,  4010,  4011,
    4014,  4018,  4021,  4025,  4026,  4029,  4031,  4035,  4037,  4041,
    4043,  4047,  4049,  4053,  4056,  4059,  4062,  4064,  4066,  4068,
    4070,  4072,  4074,  4076,  4080,  4081,  4084,  4092,  4095,  4098,
    4101,  4104,  4107,  4110,  4113,  4116,  4119,  4122,  4125,  4128,
    4131,  4134,  4137,  4140,  4143,  4146,  4149,  4152,  4155,  4158,
    4161,  4166,  4167,  4172,  4174,  4175,  4176,  4177,  4178,  4179,
    4180,  4181,  4182,  4183,  4184,  4185,  4186,  4188,  4189,  4190,
    4191,  4192
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
  "OPEN", "CLOSE", "FETCH", "ROWS", "BLOCK", "IIF", "SCALAR_ARRAY", "'='", 
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", 
  "':'", "'['", "']'", "'.'", "'?'", "top", "statement", "grant", 
  "prot_table_name", "privileges", "privilege_list", "proc_privileges", 
  "privilege", "grant_option", "role_admin_option", "simple_proc_name", 
  "revoke", "rev_grant_option", "grantee_list", "grantee", 
  "user_grantee_list", "user_grantee", "role_name_list", "role_name", 
  "role_grantee_list", "role_grantee", "declare", "declare_clause", 
  "udf_decl_clause", "udf_data_type", "arg_desc_list1", "arg_desc_list", 
  "arg_desc", "param_mechanism", "return_value1", "return_value", 
  "return_mechanism", "filter_decl_clause", "create", "create_clause", 
  "recreate", "recreate_clause", "replace", "replace_clause", 
  "unique_opt", "index_definition", "shadow_clause", "manual_auto", 
  "conditional", "first_file_length", "sec_shadow_files", "db_file_list", 
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
  "update_positioned", "assignments", "assignment", "blob", 
  "filter_clause", "blob_subtype_value", "segment_clause", 
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
       0,   287,   287,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   289,   289,   289,   289,   289,   290,   290,
     291,   291,   291,   292,   292,   293,   294,   294,   294,   294,
     294,   295,   295,   296,   296,   297,   298,   298,   298,   298,
     298,   298,   298,   299,   300,   300,   300,   300,   301,   301,
     301,   301,   302,   302,   303,   303,   303,   304,   304,   305,
     306,   306,   307,   307,   308,   309,   309,   310,   311,   311,
     311,   312,   312,   312,   313,   313,   314,   315,   315,   315,
     315,   316,   316,   317,   317,   318,   318,   318,   318,   318,
     319,   320,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   322,   323,   323,   323,   324,   325,   325,
     326,   326,   327,   327,   327,   328,   329,   329,   329,   330,
     330,   331,   331,   332,   332,   333,   333,   334,   335,   335,
     336,   336,   337,   337,   338,   338,   339,   340,   340,   341,
     342,   343,   344,   345,   346,   346,   347,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   351,   351,   352,   352,
     353,   353,   353,   354,   355,   356,   356,   357,   357,   358,
     358,   359,   359,   359,   360,   360,   360,   361,   362,   363,
     363,   363,   364,   364,   365,   365,   366,   366,   366,   367,
     368,   368,   369,   369,   370,   370,   371,   372,   373,   374,
     375,   375,   376,   376,   376,   376,   376,   376,   377,   377,
     378,   378,   379,   380,   380,   380,   380,   380,   381,   382,
     382,   383,   383,   383,   383,   384,   385,   386,   387,   387,
     388,   389,   389,   389,   389,   389,   390,   391,   392,   392,
     392,   392,   393,   394,   395,   396,   397,   397,   398,   398,
     399,   399,   400,   401,   401,   402,   403,   403,   403,   404,
     404,   405,   405,   406,   407,   407,   408,   409,   409,   410,
     410,   411,   412,   412,   413,   414,   414,   414,   415,   415,
     416,   416,   416,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   416,   416,   416,   417,
     417,   418,   419,   420,   421,   422,   423,   424,   424,   425,
     426,   427,   428,   429,   429,   429,   430,   430,   430,   431,
     431,   431,   431,   432,   433,   433,   434,   434,   434,   435,
     435,   436,   436,   437,   438,   438,   439,   439,   439,   439,
     440,   440,   440,   441,   442,   443,   444,   445,   446,   447,
     447,   448,   448,   449,   450,   451,   452,   453,   453,   453,
     454,   455,   456,   456,   457,   457,   458,   458,   459,   460,
     461,   462,   463,   463,   464,   465,   466,   466,   466,   467,
     468,   468,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   470,   470,   471,
     472,   473,   473,   473,   473,   473,   473,   473,   474,   475,
     476,   476,   477,   477,   477,   477,   477,   477,   477,   478,
     478,   479,   479,   479,   479,   479,   479,   479,   480,   481,
     481,   481,   481,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   481,   481,   482,   482,   483,   483,
     484,   485,   485,   485,   486,   486,   487,   488,   488,   489,
     489,   489,   489,   489,   490,   491,   491,   492,   492,   493,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   495,   495,   496,   496,   497,   497,   498,   498,
     499,   499,   500,   500,   501,   501,   501,   501,   501,   501,
     501,   501,   501,   502,   502,   503,   503,   503,   503,   504,
     504,   505,   505,   505,   506,   506,   507,   507,   507,   508,
     508,   508,   509,   509,   509,   510,   510,   511,   511,   511,
     512,   512,   513,   513,   513,   514,   514,   515,   515,   515,
     515,   516,   516,   517,   517,   517,   518,   518,   518,   519,
     519,   519,   520,   521,   522,   522,   523,   524,   524,   525,
     526,   527,   527,   528,   528,   529,   529,   530,   531,   531,
     532,   532,   533,   533,   533,   533,   534,   534,   535,   535,
     536,   536,   537,   537,   537,   538,   538,   538,   539,   539,
     539,   540,   541,   541,   541,   542,   542,   543,   543,   544,
     545,   545,   546,   546,   547,   548,   549,   549,   549,   550,
     550,   551,   551,   552,   553,   553,   553,   554,   554,   555,
     555,   556,   556,   556,   557,   557,   558,   558,   559,   559,
     560,   561,   562,   563,   564,   565,   566,   566,   566,   566,
     567,   567,   567,   568,   568,   568,   569,   569,   570,   570,
     571,   571,   572,   572,   573,   573,   574,   575,   575,   576,
     576,   576,   577,   578,   578,   579,   579,   580,   580,   581,
     581,   582,   582,   583,   583,   584,   584,   585,   586,   586,
     586,   586,   586,   586,   586,   586,   587,   587,   588,   588,
     589,   590,   590,   591,   591,   592,   592,   593,   594,   594,
     594,   594,   594,   595,   595,   596,   596,   597,   597,   598,
     598,   598,   599,   599,   600,   600,   601,   601,   602,   602,
     603,   604,   605,   606,   606,   607,   608,   609,   609,   610,
     611,   611,   612,   612,   612,   613,   613,   505,   614,   614,
     615,   615,   616,   616,   617,   618,   618,   619,   619,   620,
     621,   621,   622,   622,   622,   623,   624,   624,   625,   625,
     625,   625,   625,   626,   626,   626,   626,   626,   626,   627,
     627,   627,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   629,   629,   629,   629,   629,   629,   629,   629,
     630,   630,   630,   630,   630,   630,   630,   630,   630,   630,
     630,   630,   630,   630,   630,   630,   631,   631,   632,   632,
     633,   633,   633,   633,   634,   634,   635,   635,   636,   636,
     636,   636,   637,   638,   639,   639,   640,   640,   640,   641,
     641,   641,   642,   642,   643,   644,   645,   646,   646,   646,
     646,   646,   646,   646,   646,   646,   646,   646,   646,   646,
     646,   646,   646,   646,   646,   646,   646,   646,   646,   646,
     646,   646,   646,   646,   647,   647,   647,   648,   649,   649,
     650,   650,   651,   651,   651,   651,   651,   652,   652,   652,
     652,   652,   653,   654,   654,   655,   656,   656,   656,   656,
     656,   657,   657,   658,   658,   659,   660,   661,   662,   663,
     663,   664,   665,   665,   665,   665,   666,   666,   666,   666,
     666,   666,   666,   666,   666,   666,   666,   667,   668,   669,
     670,   670,   671,   672,   672,   673,   673,   674,   675,   675,
     676,   676,   676,   677,   677,   678,   678,   679,   679,   680,
     680,   681,   681,   682,   683,   684,   685,   685,   685,   685,
     685,   685,   685,   685,   686,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,   710,
     711,   712,   712,   713,   713,   713,   713,   713,   713,   713,
     713,   713,   713,   713,   713,   713,   713,   713,   713,   713,
     713,   713
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
       5,     0,     1,     3,     1,     3,     3,     0,     2,     2,
       1,     1,     3,     3,     2,     0,     2,     2,     1,     3,
       9,     2,     3,     7,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     4,     2,     2,
       1,     0,     1,     1,     6,     6,     1,     1,     0,     0,
       1,     0,     4,     0,     1,     1,     2,     8,     1,     0,
       3,     0,     0,     1,     1,     2,     1,     1,     1,     2,
       6,     1,     1,     3,     0,     1,     1,     0,     1,     1,
       2,     3,     4,     2,     2,     3,     0,     1,     1,     2,
       1,     4,     3,     3,     1,     0,     1,     1,     2,     3,
       4,     0,     1,     2,     0,     1,     1,     5,     5,     3,
       2,     0,     1,     3,     1,     1,     6,     3,     2,     6,
       2,     1,     2,     2,     2,     0,     1,     1,     2,     0,
       2,     0,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     2,     2,     5,     1,     2,     3,     2,     2,
       0,     1,     1,     1,     1,     3,     4,     8,     4,     0,
       6,     1,     1,     2,     2,     0,     3,     3,     1,     2,
       2,     2,     8,     8,     8,     8,     3,     0,     4,     0,
       1,     3,     4,     1,     3,     2,     3,     3,     1,     1,
       0,     1,     2,     4,     1,     1,     3,     1,     0,     2,
       1,     6,     1,     1,     3,     1,     2,     0,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     2,     1,     1,     1,     2,     2,     1,     2,     3,
       4,     2,     6,     4,     8,     9,     6,     8,     6,     4,
       0,     4,     2,     1,     3,     0,     2,     4,     0,     1,
       1,     3,     3,     7,     2,     0,     2,     2,     3,     3,
       0,     1,     2,     4,     1,     3,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     5,     0,     4,     7,     3,
       0,     1,     3,     3,     7,     7,     1,     1,     3,     4,
       7,     2,     1,     3,     1,     1,     6,     3,     0,     0,
       0,     0,     3,     0,     9,     9,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     5,     5,     5,     5,     5,     5,     2,     0,     4,
       2,     3,     3,     2,     2,     3,     3,     2,     3,     6,
       1,     2,     4,     3,     2,     2,     2,     2,     3,     1,
       3,     3,     3,     2,     2,     4,     4,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     0,     2,     2,     0,     1,     2,     2,
       4,     3,     2,     2,     7,     1,     0,     1,     0,     2,
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
       7,     7,     5,     3,     0,     1,     1,     1,     2,     0,
       1,     1,     1,     0,     3,     1,     3,     1,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     2,
       1,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     1,     1,     5,     6,
       3,     4,     5,     6,     3,     4,     3,     4,     3,     4,
       4,     5,     4,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     3,     3,    11,    11,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     4,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     6,     1,     6,
       1,     4,     7,     2,     0,     4,     3,     6,     1,     1,
       6,     8,     6,     1,     1,     4,     6,     4,     5,     3,
       5,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     1,     1,
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
       0,     0,   582,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   582,   659,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   564,    18,   569,   570,   571,     5,    17,   563,
     565,    19,   630,   626,    11,     8,   738,   739,    21,   743,
     744,     4,   476,     0,     0,     0,     0,     0,     0,   410,
     581,   584,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   120,     0,     0,   101,   636,   915,     0,    22,   913,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   489,     0,   360,    30,
      38,    35,    37,   763,    36,   763,  1001,  1003,  1005,  1004,
    1010,  1006,  1013,  1011,  1007,  1009,  1012,  1008,  1014,  1015,
    1016,  1018,  1019,  1017,  1020,  1021,     0,    32,     0,    33,
       0,    67,    69,   993,  1002,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   975,   653,   658,     0,
       0,   589,     0,   695,   697,   995,     0,     0,     0,   113,
     572,  1000,     0,     2,     0,     0,   643,     0,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,   438,   439,     0,   985,   417,
       0,   989,   414,   257,   992,     0,   697,   413,   388,   996,
     586,   579,   911,     0,   109,   157,   156,   110,   139,   206,
     775,   981,     0,   108,   151,   988,     0,   104,   257,   111,
     917,   128,   105,   191,   106,     0,   107,   763,   999,   112,
     152,   634,   635,     0,   916,   914,     0,    75,     0,   986,
     714,   497,   984,   490,     0,   496,   501,   491,   492,   499,
     493,   494,   495,   500,   325,     0,   259,    31,     0,    40,
     762,    39,     0,     0,     0,     0,     0,     0,   768,     0,
       0,     0,     0,     0,     0,     0,   578,   921,     0,   902,
     652,   652,   653,   665,   663,   974,   666,     0,   667,   657,
     653,     0,     0,     0,     0,     0,     0,   605,   598,   587,
     588,   590,   592,   593,   594,   601,   602,   595,     0,   696,
     994,   114,   257,   115,   191,   116,   763,   575,     0,     0,
     627,     0,   645,     0,     0,     0,     0,   415,   477,   380,
       0,   380,     0,   209,   416,   420,   411,   474,   475,     0,
     259,   230,   466,     0,   412,   429,     0,   386,   387,   486,
     585,   583,   912,   154,   154,     0,     0,     0,   166,   158,
     159,   138,     0,   102,     0,     0,   117,   259,   127,   126,
     129,     0,     0,     0,     0,     0,   977,    76,   209,   533,
       0,   740,   741,   498,     0,     0,     0,     0,   879,   908,
       0,   976,     0,   881,     0,     0,   909,     0,   903,   894,
     893,   892,  1001,     0,     0,     0,   884,   885,   886,   895,
     896,   904,   905,  1005,   906,   907,     0,  1010,  1006,   910,
    1020,     0,     0,     0,     0,   862,   357,   857,   772,   888,
     882,   858,   323,   897,   860,   861,   876,   877,   878,   898,
     859,   922,   923,   924,   938,   925,   940,   865,   863,   864,
     948,   949,   953,   954,   883,     0,     0,   981,     0,     0,
       0,   361,   207,     0,     0,     0,   765,     0,     0,    28,
      34,     0,     0,    44,    70,    72,   997,    68,     0,     0,
       0,   767,     0,    53,     0,     0,     0,     0,    52,   577,
       0,     0,     0,   662,   660,     0,   669,     0,   668,   670,
     675,   656,     0,   624,     0,   599,   610,   596,   610,   597,
     611,   617,   621,   622,   606,   591,   714,   747,   776,     0,
       0,   259,     0,     0,   574,   573,   629,   631,   636,   628,
     641,     0,   649,   174,     0,   479,   135,     0,   482,     0,
     483,   478,     0,   424,     0,   426,   425,     0,   427,     0,
     421,     0,     0,   260,     0,     0,   433,     0,   434,   380,
     467,     0,   473,     0,     0,     0,   470,   981,   391,   390,
     485,     0,   408,   155,     0,     0,   164,     0,   163,     0,
       0,   153,   167,   168,   170,   160,   533,   545,   520,   555,
     523,   546,   556,     0,   562,   524,     0,   552,     0,   547,
     559,   519,   542,   521,   522,   517,   380,   502,   503,   504,
     512,   518,   505,   514,   535,     0,   540,   537,   515,   552,
     516,   118,   257,   119,     0,     0,   130,     0,     0,   190,
     230,   388,   378,     0,   209,     0,    82,    84,     0,     0,
       0,   757,     0,     0,     0,     0,   846,   847,   848,     0,
     713,   780,   789,   792,   797,   793,   794,   795,   799,   800,
     798,   801,   796,   778,     0,   975,     0,   975,   899,     0,
       0,   975,   975,   975,     0,   900,   901,     0,     0,   964,
       0,     0,     0,     0,     0,   867,   866,   659,     0,   888,
       0,   322,   998,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,   512,   535,     0,     0,   359,     0,
     270,     0,   764,    29,     0,     0,    45,    73,     0,     0,
      27,   754,     0,   770,   659,     0,   737,   754,     0,     0,
       0,     0,   576,     0,   652,     0,     0,   714,     0,   674,
       0,   567,     0,   566,   919,     0,   600,     0,   608,   604,
     603,     0,   614,     0,   619,   607,     0,   745,   746,     0,
       0,     0,   230,   378,     0,   640,     0,   647,     0,   625,
       0,   136,   175,   481,   380,   423,     0,   380,   381,   428,
     378,     0,   256,   378,   229,   982,   201,   198,     0,   211,
     380,   502,   378,     0,     0,     0,   228,   231,   232,   233,
     234,     0,   432,   472,   471,   431,   430,     0,     0,     0,
     394,   392,   393,   389,     0,   380,   184,   161,   165,     0,
       0,   169,     0,   530,   543,   544,   560,     0,   557,   562,
       0,   550,   548,   549,   141,     0,     0,     0,   513,     0,
       0,     0,     0,   551,   259,     0,   378,   131,   189,     0,
     192,   194,   195,     0,     0,     0,     0,   209,   209,    79,
       0,    87,    78,   531,   532,   978,   533,     0,     0,     0,
     791,   779,     0,     0,   849,   850,   851,     0,   783,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   966,   967,   968,   969,   970,   971,   972,   973,
       0,     0,     0,     0,   959,     0,     0,     0,     0,     0,
       0,   975,   875,   874,   324,     0,   870,   980,   869,   868,
     871,   872,   873,   889,   946,     0,   880,   774,   773,   363,
     362,     0,   263,   278,     0,   269,   271,   766,     0,     0,
       0,     0,     0,     0,    25,    54,    42,    62,    64,     0,
       0,    71,     0,   759,     0,   769,   975,     0,   759,    50,
      48,     0,     0,     0,   661,   664,     0,   676,   677,   681,
     679,   680,   694,     0,   707,   671,   673,   990,   568,   920,
     609,   618,   613,   612,     0,   623,   748,   749,   777,   378,
       0,     0,   632,   654,   633,   642,     0,   644,   648,   480,
     154,   181,   173,   176,   177,     0,     0,     0,   422,   378,
     378,   381,   268,   261,   270,   200,   380,     0,   381,   202,
     197,   203,     0,     0,   239,     0,   436,   381,   380,   378,
     435,     0,     0,     0,   407,   488,   185,   186,   162,     0,
     172,   918,     0,     0,     0,   535,     0,   558,     0,     0,
     380,   381,     0,   508,   510,     0,     0,     0,     0,     0,
       0,     0,   388,   270,   154,   133,   230,   187,   408,   975,
     383,   366,   367,     0,    83,     0,   209,     0,    91,     0,
      85,     0,     0,    90,    86,     0,   742,   983,     0,     0,
     784,     0,     0,   790,     0,     0,   782,   781,     0,   836,
       0,   827,   826,     0,   805,     0,   834,   852,   844,     0,
       0,     0,   806,   830,     0,     0,   809,     0,     0,     0,
       0,     0,     0,     0,   807,     0,     0,   808,     0,   838,
       0,     0,   802,     0,     0,   803,     0,     0,   804,     0,
       0,   209,     0,   926,     0,     0,   941,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   965,     0,     0,   963,
       0,     0,   955,     0,     0,     0,     0,     0,   887,   945,
       0,   258,   277,     0,   335,   358,   272,    66,    58,    59,
      65,    60,    61,     0,     0,     0,    23,    26,    42,    43,
     533,   533,     0,   751,   771,     0,     0,   750,    51,    49,
      46,     0,     0,   705,   679,     0,   703,   698,   699,   701,
       0,     0,   675,     0,   712,   615,   616,   620,   270,   188,
     383,     0,   646,     0,   182,     0,   178,     0,     0,     0,
     418,   217,   212,   890,   213,   214,   215,   216,     0,     0,
     262,     0,     0,   210,   218,     0,   239,   636,   235,     0,
     437,   468,   469,   399,   400,   396,   395,   398,   397,   380,
     487,   381,   171,   979,     0,     0,   526,     0,   525,   561,
       0,   553,     0,   142,     0,   506,     0,   534,   535,   541,
     539,     0,   536,   378,     0,     0,     0,   125,   134,   193,
     380,     0,     0,   379,     0,   103,     0,   123,   122,    94,
       0,     0,    95,     0,    88,    89,     0,   842,   843,   786,
     785,   788,   787,     0,     0,     0,   659,     0,     0,   845,
       0,     0,     0,     0,     0,     0,   837,   835,   831,     0,
     839,     0,     0,     0,     0,   840,     0,     0,     0,     0,
       0,     0,   932,   931,     0,     0,   928,   927,     0,   936,
     935,   934,   933,   930,   929,     0,   944,   961,   960,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
     274,   275,     0,   981,     0,     0,     0,     0,     0,     0,
       0,  1003,  1008,     0,     0,   356,   288,   283,     0,   335,
     282,   292,   293,   294,   295,   296,   303,   304,   297,   299,
     300,   302,     0,   307,     0,   350,   352,   351,     0,     0,
       0,     0,     0,     0,   981,    55,    56,     0,    57,    63,
      24,   755,     0,   756,   753,   758,   761,   760,     0,   736,
       0,    47,   675,   690,   678,   704,   700,   702,     0,     0,
       0,     0,     0,   716,     0,   379,   637,   638,   655,   184,
     183,   179,     0,   381,   891,   266,   267,   381,   381,   205,
     219,   220,   380,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   484,   528,     0,   529,     0,   140,
       0,   205,   143,   144,   146,   147,   148,   509,   511,   507,
     538,   270,   408,   381,   184,     0,     0,     0,   364,     0,
     368,   380,    92,     0,     0,    98,    93,   535,     0,   828,
       0,     0,   975,   854,   853,     0,     0,   832,     0,     0,
       0,     0,   841,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   947,   208,   937,   939,     0,     0,   962,
     957,   956,     0,   950,   952,     0,   714,   211,   273,     0,
     311,     0,     0,     0,   306,     0,   320,   305,   336,   337,
       0,   991,   353,   354,     0,   284,     0,   289,   286,   341,
       0,     0,   308,     0,   298,   291,   301,   290,   334,    41,
     533,   714,   684,   705,   693,   691,   706,   708,   710,   711,
     722,   650,   381,   365,   639,   180,   381,   150,   255,     0,
       0,   196,   221,     0,     0,     0,   239,   222,   225,   763,
       0,   381,   405,   406,   402,   401,   404,   403,     0,   527,
     554,   149,   137,   145,     0,   380,   252,   132,   381,     0,
     714,   382,   369,     0,     0,    96,    97,    80,     0,   813,
     821,     0,   814,   822,   817,   825,   829,   833,   815,   823,
     816,   824,   810,   818,   811,   819,   812,   820,   943,   942,
     958,     0,   707,     0,   280,   276,     0,   309,     0,   325,
       0,     0,     0,   338,     0,   349,     0,     0,     0,     0,
     344,   342,     0,     0,     0,   329,     0,   330,     0,   752,
     707,   686,   683,     0,     0,   718,   720,   721,   715,     0,
     253,   419,   199,   204,   223,   239,   763,   226,   245,   238,
     240,   409,   381,     0,   384,     0,   371,   372,   374,   375,
     707,   381,    77,    99,   100,     0,   951,   712,   279,     0,
     310,   328,     0,   313,     0,   319,     0,   348,   347,   987,
     346,   335,     0,     0,     0,     0,   321,     0,     0,   712,
       0,   682,   689,   709,   719,   722,   227,   245,     0,   239,
     241,   242,   254,   381,   705,   374,     0,     0,   712,     0,
     714,   716,     0,     0,     0,     0,   335,   355,   343,   345,
       0,   340,     0,   332,   331,   716,     0,   687,   726,     0,
     723,     0,   727,   239,     0,     0,   237,     0,   244,     0,
     243,   385,   377,   373,     0,   370,   124,   707,   630,   281,
       0,   326,   312,   316,   318,     0,     0,     0,   335,   630,
       0,   685,   717,   722,     0,   729,     0,   725,   728,   224,
       0,     0,   248,   247,   246,   705,   712,   643,     0,   335,
       0,     0,   335,   333,   643,   688,   724,     0,   735,   251,
     249,   250,     0,   716,   856,   327,   317,   335,   339,   314,
     651,     0,   732,     0,   731,   376,   630,   315,   730,     0,
       0,   643,   733,     0,   855,   734,     0,     0,     0
};

static const short yydefgoto[] =
{
    1906,    20,    21,   478,   126,   127,   128,   129,  1216,   730,
     725,    22,   141,   974,   975,   976,   977,   130,   131,   483,
     484,    23,    82,   387,   871,   645,   646,   647,  1114,  1107,
    1108,  1536,   247,    24,    74,    25,   159,    26,   376,    75,
    1325,   229,   380,   637,  1095,  1317,   545,   217,   372,  1081,
    1511,  1512,  1513,  1514,  1515,  1516,   223,   239,   214,   584,
     215,   368,   369,   370,   591,   592,   593,   546,   547,  1032,
    1033,  1034,  1255,  1068,   232,   323,   382,   859,   860,   861,
     797,   798,   799,  1631,   567,   468,  1374,   648,  1048,  1260,
    1489,  1490,  1491,  1637,   862,   569,   806,   807,   808,   809,
    1278,   810,  1789,  1790,  1791,  1863,   227,   321,   631,   202,
     350,   474,   562,   563,   961,   469,  1041,   964,   965,   966,
    1399,  1400,  1203,  1695,  1401,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1702,
    1430,   435,   436,  1804,  1716,  1431,  1432,  1433,  1847,  1598,
    1599,  1709,  1710,  1434,  1435,  1436,  1437,  1594,    27,    28,
     266,   470,   471,   236,   325,  1100,  1101,  1102,  1660,  1746,
    1747,  1748,   864,  1528,   554,  1038,  1323,   234,   633,   359,
     580,   581,   823,   825,  1290,    29,    59,   788,   785,   344,
     345,   354,   355,   194,   195,   356,  1057,   575,   815,   199,
     167,   337,   338,   207,   582,  1291,    30,    96,   616,   617,
     618,  1082,  1083,   619,   714,   621,   622,  1075,  1451,   848,
     623,   715,   625,   626,   627,   628,   841,   629,   630,   838,
      31,    32,    33,    34,    35,   535,    36,   500,    37,    38,
      61,   211,   361,    39,   309,   310,   311,   312,   313,   314,
     315,   316,   759,   317,  1014,  1247,   520,   521,   764,   522,
      40,  1438,    42,   166,   536,   537,   243,  1478,  1024,   332,
     542,  1027,   779,    43,   736,   503,   299,  1251,  1624,   146,
     147,   148,   297,   507,   508,   509,   750,   747,   997,   998,
     999,  1721,  1781,  1816,  1000,  1001,  1242,  1749,   153,  1240,
    1244,  1616,  1617,  1473,   391,  1621,  1818,  1729,  1819,  1820,
    1821,  1857,  1891,  1894,  1439,  1440,    46,    47,   392,  1441,
      49,    50,   526,  1442,    51,   983,  1452,  1223,  1455,   269,
     270,   475,   490,   491,   732,   437,   438,   529,   660,   887,
     661,   662,   663,   664,  1133,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   889,  1136,  1137,  1347,   698,   674,
     440,   441,   442,  1262,   443,   444,   445,   446,   447,   448,
     449,   651,    79,   245,   231,  1073,  1084,   754,   450,   451,
     452,   453,   454,   455,   456,  1568,   457,   458,   459,   460,
     461,   462,   937,   463,   690,  1190,   691,  1187,   930,   298,
     464,   465,   874,  1292,   946,   220,   794,  1116,   251,   197,
     248,  1768,   224,  1892,  1006,  1443,  1002,   132,   466,   206,
     208,   978,   701,   237,   160,   467,   134
};

static const short yypact[] =
{
    3296,   556,    40,  2206,   494,  1008,   191,  1381,    81,  2278,
     611,   489,  1285,    40,   646,   421,  6829,   747,  6829,   397,
     378,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   553,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  6792,  6829,  6829,  6829,  6829,  6829,-32768,
  -32768,   518,   824,  6829,  6829,  6829,   744,  6829,   492,  6829,
    6829,-32768,  6829,  6829,-32768,   226,-32768,   601,-32768,-32768,
     692,  6829,-32768,  6829,  6829,  6829,   703,  6829,  6829,  6829,
    6829,   492,  6829,  6829,  6829,  6829,-32768,  6829,   547,   690,
  -32768,-32768,-32768,   596,-32768,   596,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   750,   594,   790,-32768,
      24,-32768,-32768,-32768,-32768,  6829,  6829,  6829,   776,   799,
     802,  1258,    89,   762,   377,   616,   234,   701,-32768,  6829,
     886,  1264,   813,-32768,  6829,-32768,  6829,  6829,  6829,-32768,
  -32768,-32768,  6829,-32768,   964,   198,   740,   921,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   294,-32768,-32768,   824,-32768,-32768,
     497,-32768,-32768,   748,-32768,   243,-32768,-32768,   512,-32768,
     850,-32768,-32768,   855,-32768,   691,-32768,-32768,  1031,-32768,
  -32768,-32768,   824,-32768,-32768,-32768,    91,-32768,   748,-32768,
  -32768,   441,-32768,   994,-32768,  1002,-32768,   596,-32768,-32768,
  -32768,-32768,-32768,   999,-32768,-32768,   891,-32768,   997,-32768,
     908,-32768,-32768,-32768,   891,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  5711,  6829,   957,-32768,  6829,-32768,
  -32768,-32768,  4181,   531,   979,  5181,  6829,  1023,   852,  1098,
    1116,  4181,  1036,  1059,  1063,  5181,   946,-32768,  5941,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  4399,-32768,-32768,
  -32768,  1017,  6829,  1095,  1011,   404,  6829,  1028,-32768,-32768,
    1264,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6829,-32768,
  -32768,-32768,   748,-32768,   994,-32768,   596,  1082,  5941,  1050,
  -32768,  5941,  1159,    67,   968,   971,   977,   921,-32768,  1214,
     778,-32768,  6829,-32768,   294,-32768,-32768,-32768,-32768,  6829,
     957,  1834,  1033,  2189,   976,-32768,  6792,-32768,-32768,   508,
  -32768,-32768,-32768,   985,   985,   824,  1144,   824,   190,   691,
  -32768,-32768,  4595,-32768,  6829,  6829,-32768,   957,-32768,-32768,
    1231,   438,   986,  6829,  1261,  6829,-32768,-32768,   299,   495,
    3739,-32768,-32768,-32768,   992,  1003,  1007,  1081,-32768,-32768,
    1010,-32768,  1012,-32768,  1014,  1015,-32768,  1018,-32768,-32768,
  -32768,-32768,  1019,  1020,  1094,  1096,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1024,-32768,-32768,  4629,  1029,  1035,-32768,
    1054,  5941,  5941,  4727,  6829,-32768,-32768,  1006,-32768,  1193,
  -32768,-32768,  1047,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1055,  1043,  1051,  4595,  1045,
     655,-32768,-32768,  1058,  1298,   695,-32768,  6829,  1157,-32768,
  -32768,  6829,  6829,   303,-32768,-32768,-32768,-32768,  6829,  6829,
      88,-32768,  6829,-32768,  1275,  6829,  4181,  1195,  1071,-32768,
    6829,  4727,   336,-32768,-32768,  5941,-32768,  1287,  1072,-32768,
     160,-32768,   794,-32768,   107,-32768,   598,-32768,   598,-32768,
    1075,-32768,   108,-32768,  1181,-32768,    30,-32768,-32768,  1086,
    1078,   957,  1087,  1355,-32768,-32768,  1089,-32768,   185,-32768,
    1148,  1194,  1183,-32768,  1290,  1291,-32768,   824,-32768,  1299,
  -32768,-32768,  1370,-32768,  1372,-32768,-32768,  1351,-32768,  4595,
  -32768,  4595,   713,-32768,  1389,  6829,-32768,  6382,-32768,   482,
  -32768,  6829,   803,   243,  1218,  1185,  1259,  1230,-32768,-32768,
  -32768,   420,  1270,-32768,  1210,   492,-32768,   824,-32768,  1393,
    1323,-32768,   190,-32768,-32768,-32768,   743,  1226,-32768,-32768,
  -32768,  1227,-32768,  1286,  1150,-32768,  1338,  1156,   451,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1154,-32768,-32768,-32768,   127,  1165,  1167,   257,-32768,  1156,
  -32768,-32768,   748,-32768,  1385,  1440,-32768,   824,   824,-32768,
    1834,   512,-32768,  1328,-32768,  1301,  1184,-32768,  6643,  6691,
    1327,-32768,  1337,  1191,  3969,  1192,   827,  1103,  1223,  3639,
     199,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  2696,   638,  5941,   125,-32768,  6829,
    5941,   811,   823,   856,  1453,-32768,-32768,  5941,  4069,  1193,
     232,  1277,  5941,  5941,  4069,   193,   193,   646,  1196,   390,
     758,-32768,-32768,  5941,  6829,  5941,  5941,  5941,  5941,  5941,
    5941,  4957,  4853,-32768,-32768,  1456,  1190,  6829,-32768,  6829,
    1435,  6829,-32768,-32768,  5823,  1300,-32768,-32768,  1271,  5181,
  -32768,  1420,   768,-32768,   646,  1205,-32768,  1420,  5823,  1421,
    1422,  6829,-32768,   390,-32768,   468,  6086,  1292,  5941,-32768,
    6829,-32768,    48,-32768,-32768,   158,-32768,  1295,-32768,-32768,
  -32768,  6829,   353,  6829,-32768,-32768,  6829,-32768,-32768,  5941,
    6829,  1482,  1834,-32768,  5941,  1245,  5941,  1373,  1251,-32768,
     824,-32768,   558,-32768,-32768,-32768,  1224,-32768,-32768,-32768,
     137,  6829,-32768,-32768,-32768,-32768,  1483,-32768,  1225,  1461,
  -32768,  1479,-32768,  1423,  1425,   596,-32768,-32768,-32768,-32768,
  -32768,  1487,-32768,-32768,-32768,-32768,-32768,  6829,  6562,   492,
    1380,  1384,  1386,-32768,   492,-32768,   947,-32768,-32768,  1353,
     824,-32768,   443,  1357,-32768,-32768,-32768,   492,-32768,  1150,
     595,-32768,-32768,-32768,  1476,   595,  1371,   595,-32768,   492,
     492,  1257,   492,-32768,   957,  6829,-32768,  1442,-32768,   773,
  -32768,-32768,-32768,   508,  1382,  6829,   845,    28,-32768,-32768,
    1272,   201,-32768,-32768,-32768,-32768,   495,  6829,  1387,  4299,
  -32768,-32768,  1387,  3969,-32768,-32768,-32768,   112,-32768,   298,
    2101,  4069,  4069,  5941,  5941,  1773,  1276,   640,  2065,  5941,
    2391,   749,  2669,  2939,  5055,  3039,  3309,  3409,  5941,  5941,
     189,  5941,  1278,  5941,  1281,   559,  5941,  5941,  5941,  5941,
    5941,  5941,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1485,  1085,   194,  5941,-32768,  4069,  5941,   444,   414,   499,
     130,   234,-32768,-32768,-32768,   259,-32768,-32768,  1531,   193,
     193,   238,   238,  1193,-32768,   849,-32768,-32768,-32768,-32768,
  -32768,   863,-32768,  1374,  1545,  1435,-32768,-32768,  6829,  6829,
    6829,  6829,  6829,  6829,  1284,-32768,   309,-32768,-32768,  5823,
    1433,-32768,   156,  1454,  6829,-32768,   234,  5941,  1454,  1284,
    1289,  5823,  3877,  1503,-32768,-32768,  3128,  1293,   834,-32768,
  -32768,-32768,  1294,  4069,  1496,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   338,-32768,-32768,  1193,-32768,-32768,
     874,  1382,-32768,-32768,-32768,  1193,  6829,-32768,-32768,-32768,
     985,  1559,-32768,   558,-32768,  1297,  4069,  2793,-32768,-32768,
  -32768,-32768,-32768,-32768,  1435,-32768,-32768,  2793,-32768,-32768,
  -32768,-32768,   596,   596,  1333,  1302,-32768,-32768,-32768,-32768,
  -32768,   104,   240,   433,-32768,  1565,-32768,-32768,-32768,  6829,
  -32768,-32768,   494,   896,  1415,  1456,  1303,-32768,  1210,   906,
  -32768,-32768,   471,-32768,  1304,  6829,   632,  1312,  1315,   492,
    1321,  1594,   512,  1435,   985,  1291,  1834,-32768,  1270,   234,
    1407,  1424,-32768,  1343,-32768,   492,  1469,  1560,-32768,  6643,
  -32768,   492,   246,-32768,-32768,  1561,-32768,-32768,  1336,  1339,
  -32768,  4069,  4069,-32768,  4069,  4069,-32768,  1606,   172,  1193,
    1342,-32768,-32768,  1344,  1193,  5285,-32768,-32768,-32768,  1520,
    1347,  1348,  1193,  1179,  1350,  1352,  1193,  5941,  5941,  1276,
    5941,  5383,  1356,  1358,  1193,  1359,  1361,  1193,  5941,  1193,
    1362,  1363,  1193,  1366,  1367,  1193,  1375,  1378,  1193,   567,
     590,-32768,   607,-32768,   617,  5941,-32768,   704,   759,   769,
     873,   895,   940,  5941,  5941,  5941,  1193,  1579,   312,  1193,
    1438,  5941,-32768,  5941,  5941,  5941,  5941,  5941,-32768,-32768,
    6829,-32768,-32768,  6829,  6183,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  5823,  1578,  5823,-32768,  1284,   309,-32768,
     432,   432,    -4,-32768,-32768,  4399,   939,-32768,  1284,  1289,
    1383,  3877,  1398,   808,  1399,  6086,  1519,-32768,  1525,  1527,
    1574,  5941,   922,  1634,  1587,-32768,-32768,-32768,  1435,-32768,
    1407,    16,  1388,  1210,  1528,  1210,-32768,  4069,   113,  1048,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2793,  2793,
  -32768,  1545,  5941,-32768,   481,  1529,  1333,   226,-32768,  4069,
  -32768,-32768,-32768,  1549,  1551,  1552,  1555,  1556,  1558,-32768,
  -32768,-32768,-32768,-32768,  1410,   494,-32768,  1488,-32768,-32768,
     595,-32768,  2793,   177,   595,-32768,   595,-32768,  1456,-32768,
  -32768,  1412,-32768,-32768,   508,  1545,  1210,-32768,  1291,-32768,
  -32768,  4399,  1670,-32768,   237,-32768,  1419,-32768,  1388,-32768,
    1414,   824,   146,  1426,-32768,-32768,   824,-32768,-32768,-32768,
    1606,-32768,  1606,  5941,  1537,  1537,   646,  1429,   952,-32768,
    1537,  1537,  5941,  1537,  1537,   297,  1193,-32768,  1250,  5941,
    1193,  1537,  1537,  1537,  1537,  1193,  1537,  1537,  1537,  1537,
    1537,  1537,-32768,-32768,  1432,  4595,-32768,-32768,   969,-32768,
  -32768,-32768,-32768,-32768,-32768,   984,  1132,-32768,-32768,  5941,
    5941,  1650,  1524,   993,   956,  1046,  1115,-32768,  4595,  1428,
  -32768,-32768,  1682,  1683,  6609,   329,  1446,  1445,  1640,  5941,
    1449,  1451,  6645,  6829,  6829,-32768,-32768,-32768,  1677,  6050,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   115,-32768,  1459,-32768,-32768,-32768,  1648,  1460,
    1462,  1463,  1464,  1457,   376,-32768,-32768,  1612,-32768,-32768,
  -32768,-32768,  1562,-32768,-32768,-32768,-32768,-32768,  1287,-32768,
    3877,  1383,  1735,-32768,   834,-32768,-32768,-32768,  6086,   961,
    6829,  5941,  4069,  1604,  1545,-32768,-32768,-32768,-32768,   947,
  -32768,-32768,   114,-32768,-32768,-32768,-32768,-32768,  1193,  1721,
     481,-32768,   819,  6829,-32768,  1665,   116,  1563,  1668,  1566,
    1702,  1669,  1708,  1435,-32768,-32768,  1475,-32768,  1477,-32768,
    1661,  1721,   177,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1435,  1270,-32768,   947,  1565,  1694,  1630,-32768,  1382,
  -32768,-32768,-32768,  1646,    72,-32768,-32768,  1456,  1647,  1193,
    1484,  1486,   234,-32768,-32768,  1491,  1492,  1193,  1493,  1494,
    5941,  5941,  1193,  1499,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1515,  1517,-32768,-32768,-32768,-32768,  5941,  1518,-32768,
  -32768,-32768,  5941,-32768,-32768,  5941,  1292,   143,-32768,  1713,
  -32768,  5613,  6829,  5941,-32768,  4069,  1193,-32768,-32768,-32768,
    1506,-32768,-32768,-32768,  6829,-32768,   154,-32768,  1569,-32768,
     423,  1521,-32768,  5509,-32768,-32768,-32768,-32768,-32768,-32768,
     432,  1292,  6829,   924,-32768,-32768,  1523,-32768,  1193,   199,
     634,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1530,
    6829,-32768,-32768,  1706,  1710,  6829,  1333,-32768,-32768,   596,
    6829,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1545,-32768,
  -32768,-32768,-32768,-32768,  1545,-32768,-32768,-32768,-32768,  6450,
    1292,-32768,-32768,  5941,   824,-32768,  1593,-32768,   824,-32768,
  -32768,  5941,-32768,-32768,-32768,-32768,  1193,  1193,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1193,-32768,
  -32768,  1037,  1496,  2793,-32768,-32768,  1526,-32768,   400,  5711,
     680,   121,  1534,-32768,  1723,-32768,  6829,  6829,   494,    85,
  -32768,-32768,  1564,  1730,  4069,-32768,   814,-32768,  1532,-32768,
    1496,  1540,-32768,  4069,  5941,-32768,-32768,  1707,-32768,  1542,
  -32768,-32768,-32768,-32768,-32768,  1333,   596,-32768,  1695,-32768,
  -32768,-32768,-32768,  1565,-32768,  6450,  1544,   834,-32768,-32768,
    1496,  1193,-32768,-32768,-32768,  1115,-32768,  1587,-32768,  1387,
  -32768,  1671,  5509,-32768,  1645,-32768,  5509,-32768,-32768,-32768,
  -32768,  6292,   154,  5941,  5509,   122,-32768,  5509,  6727,  1587,
    6829,-32768,   199,-32768,-32768,  2581,-32768,  1695,   253,  1333,
    1699,  1700,-32768,-32768,   808,  1554,  6450,  1741,  1587,  1567,
    1292,  1604,  1570,  4525,  1568,   839,  6292,  1553,-32768,-32768,
    1152,   134,  1783,-32768,-32768,  1604,   996,-32768,-32768,  1571,
    1557,   148,  6829,  1333,   593,   593,-32768,  1790,-32768,  1652,
  -32768,-32768,-32768,   834,  6450,-32768,-32768,  1496,  1717,-32768,
    5509,  1553,-32768,-32768,  1800,  5509,  1804,  1805,  6292,  1717,
    6829,-32768,-32768,  2581,  1576,-32768,  6829,-32768,-32768,-32768,
    1649,   192,-32768,-32768,-32768,  1038,  1587,   740,  1005,  6292,
     126,  6829,  6292,-32768,   740,-32768,-32768,  6829,  1771,-32768,
  -32768,-32768,  4069,  1604,-32768,-32768,-32768,  6292,-32768,-32768,
  -32768,  1580,  1585,  1577,-32768,   199,  1717,-32768,-32768,  6829,
    6829,   740,-32768,  1584,-32768,-32768,  1859,  1866,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -73,   178,-32768,   279,  1596,   652,-32768,
    -358,-32768,-32768,  -595,  -156,  -667, -1053,  1860,  1599,  1591,
    1151,-32768,-32768,-32768,   770,-32768,  1233,  1013,-32768,-32768,
     777,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   787,-32768,-32768,-32768,
  -32768,-32768,   372,-32768,-32768,  1547,-32768,-32768,-32768,  -329,
  -32768,-32768,-32768,  1536,-32768,-32768,  1296,  -350,-32768,-32768,
  -32768,   854,-32768,  -842,-32768,-32768,  1572,  1120,   797,  1543,
    1097,   792,-32768,   386,   -47,  -241,-32768,  -322,   322, -1008,
  -32768,-32768,   410,-32768,  1550, -1092,-32768,-32768,-32768,-32768,
   -1224,   418,   119,   128,   131,    98,-32768,-32768,-32768,-32768,
    -125,  -260,-32768,  1133,-32768,  -679,-32768,  -975,-32768,   970,
  -32768,-32768,-32768,-32768,-32768, -1328,  -923,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  -765,   235,-32768,  -688,-32768,-32768,-32768,-32768,-32768,
     335,-32768,   164,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1222,-32768,-32768,   926,-32768, -1130,-32768,-32768,
   -1239,   214,  -439,   485,  -299,  -864,   693,-32768,-32768,  -532,
    -753,-32768,-32768,  -986, -1383,-32768,-32768,-32768,-32768,-32768,
    1617,-32768,  1390,  1608,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1628,-32768,-32768,-32768,-32768,-32768,  -544,  -448,
  -32768,  1119,   663,  -554,  -362,-32768,-32768,-32768,  -300,  -969,
  -32768,  -353,-32768,-32768,-32768,-32768,  1340,-32768,-32768,  1129,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1957,-32768,-32768,-32768,-32768,-32768,  1662,-32768,-32768,-32768,
    1468,-32768,  1455,-32768,-32768,-32768,-32768,  1213,-32768,-32768,
  -32768,     8,-32768, -1037,-32768,  1197,  -505,-32768,-32768, -1051,
  -32768,-32768,-32768,     0,  -856,  -191,   362,-32768,-32768,  -632,
  -32768,  1828,  -890,  -730,-32768,  1235, -1117, -1280,-32768,  -914,
  -32768,-32768,-32768,-32768,   988,-32768,-32768,   626,    35,  -925,
   -1202,-32768,   261, -1052,  -515, -1183,   366,-32768,   135,-32768,
     165,-32768,  -739,-32768,  1989,  1991,-32768,-32768,  1466,  1993,
  -32768,-32768,-32768,  -203,-32768,  1260, -1114,  1022,-32768,  -101,
    -671,  -423,-32768,-32768,-32768,  -117,   -38,  -451,  -555,-32768,
    -441,-32768,-32768,-32768,   233,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -479,-32768,   846,-32768,   587,-32768,  1009,
    -980,-32768,  -332,-32768,   735,  -896,  -132,  -891,  -883,  -880,
     -12,     1,     2,-32768,   -63,  -571,  -481,  -130,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   805,-32768, -1033,-32768,   689,
    -878,   664,-32768,   927,   383,  -683,  1467, -1179,-32768,   -58,
    1929,-32768,   -26,   -53,  -973,   613,   -24,   -36,  -137,   -15,
     -40,  -180,-32768,   -60,   -48,    -9,-32768
};


#define	YYLAST		7099


static const short yytable[] =
{
     133,   154,   200,   133,   271,    78,   222,   155,    41,   161,
     620,   767,   290,   293,   291,   294,   218,   319,   594,   624,
     713,   559,  1118,   800,  1402,   219,  1119,   253,   259,   958,
     235,   753,   203,   775,   262,   585,   257,   240,   733,  1273,
     962,  1205,   557,   228,   196,   198,   201,   204,   155,   209,
     216,  1197,  1494,   261,   221,   198,   225,  1261,   204,   263,
     155,   209,   256,   238,   133,   941,   258,  1261,   154,  1271,
     230,   990,   249,   264,   155,   252,   198,   260,   249,   225,
     201,   204,  1233,   155,   209,   238,   133,  1018,   204,   650,
     564,  1597,   205,   230,   872,   485,  1225,   277,   326,   279,
     504,   700,   986,   377,   233,   485,  1298,  1454,   561,   863,
    1098,   789,  1320,   790,   327,   527,  1576,   635,  1315,   801,
    1121,   891,   891,   301,   891,  1470,   221,   155,   221,   891,
     891,   295,   322,   932,  1054,  1771,   384,   739,   891,   940,
     225,  1263,  1658,   989,  1846,   320,  1264,   204,   155,   238,
     846,  1263,  1387,   161,  1265,   285,  1264,  1266,  1391,  1267,
    1446,   543,  1449,  1705,  1265,   330,  1534,  1266,  1105,  1267,
     749,   278,   762,  1665,   911,   881,  1887,  1270,  1611,  1600,
    1343,   530,  1492,  1039,  1274,   346,   704,   516,  1283,  1047,
     139,   705,   324,  1280,  1530,   781,   241,   531,   704,  1171,
    1071,  -380,   891,   705,   329,   620,   275,   891,   494,  1321,
     373,   704,  1706,   880,   624,   704,   705,  1303,   888,   704,
     705,  1112,  1220,  1707,   705,   533,  -672,   472,   390,    97,
     476,  1854,  1004,   242,  1592,  1593,   589,   241,  1880,   374,
     295,    60,   594,  1529,  1122,   892,   892,   351,   892,   513,
     352,   734,   287,   892,   892,  1476,   221,    83,   755,   221,
    1485,  1486,   892,   155,   704,  1477,   486,   133,  1855,   705,
     811,   771,   155,  1474,   242,   375,   486,   307,   735,  1008,
     528,  1856,   289,   296,   543,   933,   934,  1285,  1261,  1261,
    1881,   140,  1284,   201,  1509,   353,   833,   155,   339,  1113,
    1824,  1510,   727,   276,   558,  1550,  1124,   479,  1106,   766,
    1666,   472,  1218,   219,  1601,   572,   479,   844,   576,   283,
     891,  1464,  1261,   704,  1229,  1230,   892,  1708,   705,   539,
     544,   892,   643,   221,  1021,   634,  1126,  1127,  1221,  1519,
     221,   523,   221,   518,   221,  1612,   340,   577,  1487,    98,
     632,  1042,   530,   586,  1044,   588,  1569,  1570,  1535,  1079,
    1793,    14,   704,  1051,  1772,   204,   209,   705,   276,   639,
    1058,   945,  1263,  1263,   155,  1185,   201,  1264,  1264,   955,
    1188,  1275,  1276,   993,  1217,  1265,  1265,   763,  1266,  1266,
    1267,  1267,  1523,  1123,  1483,  1626,  1228,  1641,  1492,  1662,
    1099,   912,  1764,  1812,  1120,  1777,  1263,  1449,  1040,  1196,
     847,  1264,  1737,  1777,  1693,  1704,   704,  1093,   641,  1265,
     284,   705,  1266,   740,  1267,   702,   704,  1504,  1286,   935,
    1125,   705,  1327,   516,   706,   707,   708,   709,   880,  -672,
     704,  1825,   880,  1808,   892,   705,   706,   707,   708,   709,
     851,   528,   742,   590,   826,   378,   -81,   726,   341,   706,
     707,   708,   709,   706,   707,   708,   709,   820,   155,   708,
     709,   726,   204,   486,   842,  1800,   342,  1582,  1844,   155,
    1287,  1258,  1712,   155,  1334,  1035,   204,   155,  1037,  1245,
    1757,   161,   149,  1389,   704,  1458,  1719,  1191,  1192,   705,
     347,  1049,  1012,   728,   821,  -230,  1794,   854,   137,  1214,
     343,  1786,   723,   578,   565,   357,  1335,  1288,  1779,  1013,
    1873,  1397,   827,   731,   579,   704,  1065,   737,  1648,   802,
     705,   479,   638,  1224,   517,   782,  1655,   852,   710,  1690,
    1246,  1886,   843,  1198,  1889,  1109,  1654,   803,  1798,   485,
     561,  1622,   379,   994,  1613,   872,   795,  1833,   221,  1897,
    1314,  1522,   795,  1016,  1461,  1826,  1339,  1340,  1667,  1341,
    1342,   706,   707,   708,   709,   828,  1115,   348,   100,   644,
    1248,   287,   729,  1583,   959,   704,    14,   230,  1215,   518,
     705,  1526,   358,   704,  1091,  1865,    52,   150,   705,  1859,
    1268,  1269,   219,  1252,   151,  -230,   519,    53,   822,   649,
     706,   707,   708,   709,    54,   102,   704,   744,  1838,  1627,
    1282,   705,  1009,  1628,  1629,   857,   858,  -230,   804,   530,
     135,   221,  1849,   704,  -230,  1866,    76,  1625,   705,    55,
     875,  1193,   152,   704,   295,   212,   213,  1071,   705,   162,
     873,  1457,  1671,   914,   505,  1226,   163,   288,  -991,  1656,
    1030,  -994,   511,   289,   706,   707,   708,   709,  -230,   805,
     225,   943,   649,   210,   706,   707,   708,   709,  1760,   472,
    1328,   472,  1657,   967,   103,  1758,   164,   908,   706,   707,
     708,   709,  1888,  1194,   104,   947,    76,   136,    76,    76,
    1896,  1253,  1482,   221,    56,  1801,   704,    77,   221,   250,
     221,   705,   221,  1261,  1542,   486,  1860,   726,   289,   105,
     486,   757,  1072,  1725,  1496,  1741,  1507,  1815,   528,   486,
     704,  1742,   204,  1031,   219,   705,    57,   204,  1138,   165,
      58,  1007,   706,   707,   708,   709,  1835,  1272,  1726,   995,
    1304,   226,   155,   472,   155,  1305,  1060,  1861,  1730,  1281,
     246,   221,  1731,   221,  1139,  1316,  1762,  1147,  1029,    77,
      77,   254,  1495,   706,   707,   708,   709,  1740,  1195,  1056,
    1059,  1302,   221,  1148,  1109,   704,  1087,  1088,  1207,  1090,
     705,  1210,   758,   363,  1744,   704,   523,  1263,  1015,   287,
     705,  1867,  1264,  1348,  1862,   244,  1727,  1817,   221,   221,
    1265,   555,  1874,  1266,  1883,  1267,  1884,   295,  1070,  1508,
     287,   230,  1797,  1890,   556,  1518,  1064,   265,  1149,   295,
     364,  1564,   365,   706,   707,   708,   709,   267,  1715,  1076,
    1176,   706,   707,   708,   709, -1014,   155,   530,  1372,  1375,
    1904,   230,   230, -1014,   230,   366,   155,  1150, -1014,  1901,
     916, -1014,   295,  1394,   706,   707,   708,   709,  1117,  1797,
    1078,  1373,   918,   273,  1521,  1236,   268,  1875,  1792,   272,
     367,   706,   707,   708,   709,   144,   145,  1799,  1376,  1237,
    1092,   706,   707,   708,   709,   156,   292, -1014,  1377,   704,
    1103,  1236,   289,  1802,   705,   920, -1014,   280,  1797,  1469,
     388,  1304,  1211,  1238, -1014,  1237,  1308,  1619,   393,   274,
     649,   704,  -692,  -705,  1151,   333,   705,   157,   281,  1831,
    1209,   282,   749, -1014,   717, -1014,   718,  1212,   334,  1238,
    1797,   145,   158,  1633,   286,  1208,   528,    76,  1009, -1014,
    1577, -1014, -1014, -1014,   706,   707,   708,   709,  1763,   486,
     204,   209,   486,   238,   133,  1634,   704,  1239,   781,   302,
     486,   705,  1635,   335,   721,   336,   722,   318,   706,   707,
     708,   709,   486,   486,   328,  1379,  -692,   204,   476,  -692,
    1503,  1236,   791,  1239,   792,   704,  -692,  1715,   287,  -692,
     705,  1715, -1014,  -692,  1232,  1237,  1636,   331,  -692,  1715,
     704,  -692,  1813,   620,   813,   705,   814,   221,    77,   704,
     360,  1525,   624,   832,   705,   751,  1311,  -692,   349,  1238,
    1701,   212,   213,   706,   707,   708,   709,   710,  1715,   944,
    1380,   371,  1329,   706,   707,   708,   709,   984,  1333,   985,
    1381,  -692,  1096,  1723,  1097,  -692,   381,  1445,   219,  1448,
    1293,  1692,   362,   704,  -692,   476,   383,   530,   705,   752,
      80,    81,   704,  1294,  1805,  1715,  1293,   705,  1807,   389,
    1715,  -692,   385,  1239,  1066,  1067,  1811,   221,  1453,  1453,
    1456,   230,  1776,  1777,   221,   958,  1720,   386, -1014, -1014,
   -1014, -1014, -1014, -1014, -1014,  1236,   390,   230,  -692,   488,
   -1014,   704, -1014,   230,   473,  1841,   705,  1843,  1777,  1237,
    -692, -1015,  -692,  1479,   868,  1481,  1104,   481,   710, -1015,
    1199,  1141,   489,  1145, -1015,  1153,  1156, -1015,  1161,  1164,
    1167,   704,  1200,  1238,  1201,  1750,   705,   706,   707,   708,
     709,  1184,  1868,  1096,  1382,  1249,  1398,  1870,   704,  1775,
    1902,  1903,   472,   705,   492,   219,   528,  1882,  1782,   706,
     707,   708,   709, -1015,   704,  1295,  1383,  1296,   704,   705,
     493,   746, -1015,   705,   495,  1300,  1524,  1301,   496,  -692,
   -1015,   221,   497,   811,  1403,  1444,  1567,  1239,   499,   512,
    -692,  -692,   514,  -692,   486,   704,   486,   515,   524, -1015,
     705, -1015,   534,    14,   706,   707,   708,   709,   710,   704,
    1459,  1384,   486,   541,   705, -1015,   204, -1015, -1015, -1015,
     548,   710,  1663,  1544,   549,   710,  1352,  1574,  1845,   550,
     710, -1016,  1614,   706,   707,   708,   709,   552,   570, -1016,
    1565,   409,   410,   411, -1016,   573,   583, -1016,   706,   707,
     708,   709,   587,   636,    99,  1566,   640,   706,   707,   708,
     709,   642,   675,   439,  1573,  1850,   704,  1851, -1015,   419,
     420,   705,   530,   676,  1777,  1837,  1885,   677,   678,   703,
     679,    99,   680, -1016,   681,   682,  1506,   502,   683,  -977,
     684,   685, -1016,   686,   687,   100,   510,  1551,   720,   692,
   -1016,   706,   707,   708,   709,   693,   716,   101,  1756,  1533,
     706,   707,   708,   709,  1538,  1575,   710,  1895,   712, -1016,
     776, -1016,   100,  1615,   694,   711,  -994,   538,   719,   724,
     540,   738,   102,   741,   101, -1016,  1581, -1016, -1016, -1016,
     729,   748,   303,   746,   761,   765,  1743,   769,   138,   706,
     707,   708,   709,   770,   909,   773,   913,   772,   774,   102,
     917,   919,   921,   796, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015,   528,   777,   778,   780,   543, -1015,   304, -1015,   706,
     707,   708,   709,   783,   784,   198,   786,   787, -1016,   793,
     817,   818,   819,  1591,  1117,  1117,   706,   707,   708,   709,
    1444,   103,  -439,   824,   287,   305,   829,   830,   306,   834,
     835,   104,   706,   707,   708,   709,   706,   707,   708,   709,
     837,   836,    84,   839,   307,   689,   840,   845,   103,    85,
     695,   696,   699,    86,    87,   849,   105,   850,   104,   855,
     856,   486,    88,   706,   707,   708,   709,   865,   867,   204,
     308,   320,   876,   868,    89,   877,  1718,   706,   707,   708,
     709,   878,   882,   105,   936,  1722,   289,   942,  1453,   846,
     963,   980,   979,   982,   155,   987,  1717,   991,   992,  1010,
    1003,   106,  1019,  1023, -1016, -1016, -1016, -1016, -1016, -1016,
   -1016,  1026,  1028,  1045,  1036,  1046, -1016,  1047, -1016,   796,
     743,  1055,  1061,  1052,   745,  1053,  1062,  1069,  1063,  1074,
     107,   108,  1080,   109,   706,   707,   708,   709,  1639,    90,
     110,   111,   112,   113,   114,  1085,   115,  1089,  1738,   116,
     117,   118,   119,   120,  1094,  1099,    91,   121,   122,   106,
     734,  1183,  1111,   123,   124,   125,  1135,   704,  1699,  1173,
    1175,    92,  1204,  1213,  1219,    93,  1202,  1222,  1215,  1231,
    1243,  1254,  1235,   204,  1241,  1289,    94,  1257,   107,   108,
    1277,   109,  1279,  1297,  1299,  1117,  1306,  1739,   110,   111,
     112,   113,   114,  1309,   115,    95,  1310,   116,   117,   118,
     119,   120,  1312,   320,  1313,   121,   122,  1322,  1713,  1105,
    1324,   123,   124,   125,   891,  1331,  1336,  1337,  1349,  1390,
    1338,   947,  1344,   268,  1345,  1718,   155,  1350,  1351,  1718,
    1353,   201,  1354,  1388,   530,  1787,  1361,  1718,  1362,  1363,
    1718,  1364,  1366,  1367,   154,  1717,  1368,  1369,  1767,  1717,
     155,  1447,  1752,  1465,  1471,  1370,  1754,  1717,  1371,  1466,
    1814,  1467,  1460,  1468,  1472,  1480,  1718,   721,   890,   530,
    1736,   922,   923,   924,   925,   926,   927,   928,   929,  1462,
    1463,  1497,  1493,  1498,  1499,   910,  1717,  1500,  1501,   915,
    1502,  1505,  1071,  1520,  1527,  1532,   931,   198,  1769,  1531,
    1346,   938,   939,  1718,  1571,  1572,  1578,  1537,  1718,  1770,
    1543,   530,   439,  1563,   948,   949,   950,   951,   952,   953,
     439,  1579,  -983,  1717,  1584,  1585,   136,  1587,  1717,  1588,
     154,  1595,   530,   528,  1603,   530,   155,  1602,  1604,  1608,
    1605,  1606,  1607,  1609,  1610,   749,  1620,  1630,  1640,  1645,
     530,  1642,  1643,  1646,  1644,  1647,  1649,   510,  1650,  1651,
    1659,  1661,  1444,  1664,  1668,  1669,  1596,  1670,   528,   221,
    1822,  1007,  1672,  1673,  1674,  1675,   155,  1696,  1017,  1130,
    1678,   154,  1131,   538,  1703,  1025,   394,   155,  1679,  1680,
    1681,  1682,  1683,  1684,  1685,   395,  1686,  1444,  1687,  1689,
    1735,  1714,  1724,  1878,  1734,  1753,  1759,  1822,   396,  1766,
     528,  1732,  1765,   155,   397,   398,  1774,  1778,  1773,   154,
    1780,  1784,  1785,  1796,  1788,   155,  1806,  1803,  1827,  1829,
    1834,   528,  1777,  1848,   528,  1832,  1853,  1824,  1822,  1444,
    1825,  1007,   399,  1871,   155,   400,  1842,   201,  1836,   528,
     164,  1839,  1852,  1869,  1893,  1872,  1877,  1900,  1879,  1907,
    1444,  1898,  1117,  1444,  1899,  1905,  1908,   565,   201,   480,
    1450,   401,   142,   402,   403,   487,   498,   866,  1444,  1332,
     981,  1110,  1318,  1330,  1653,   404,   553,  1256,   831,   405,
     201,   201,  1020,  1319,   566,  1326,   532,  1652,  1050,  1694,
    1632,   568,  1128,  1129,  1134,   595,  1823,  1142,  1143,  1146,
    1638,  1154,  1157,  1159,  1162,  1165,  1168,  1169,  1170,  1830,
    1172,  1828,  1174,  1864,  1043,  1177,  1178,  1179,  1180,  1181,
    1182,  1540,  1541,  1711,  1761,  1206,  1809,  1545,  1546,   960,
    1548,  1549,  1186,  1475,  1132,  1189,   406,  1250,  1553,  1554,
    1555,  1556,   407,  1557,  1558,  1559,  1560,  1561,  1562,  1795,
    1623,   560,   408,   816,   574,   551,  1086,  1517,  1077,   853,
     143,  1022,   525,   760,  1011,   300,   409,   410,   411,   412,
     212,   213,   756,  1005,  1234,  1783,  1728,  1858,  1876,    44,
     413,    45,   768,    48,  1484,  1357,   439,   988,  1392,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   107,   423,
    1227,   109,  1307,  1733,   424,   425,   255,   426,   427,   428,
     112,   113,   114,   429,   115,  1590,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,   812,     0,
     106,   123,   430,   125,     0,     0,     0,   431,   432,     0,
       0,     0,     0,   501,     0,   434,     0,     0,     0,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     108,  1140,   109,     0,  1131,     0,     0,     0,   394,   110,
     111,   112,   113,   114,     0,   115,     0,   395,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
     396,     0,   123,   124,   125,     0,   397,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   893,
       0,     0,     0,     0,     0,     0,     0,   704,     0,     0,
       0,     0,   705,     0,   399,   894,     0,   400,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1355,  1356,     0,  1358,
    1360,     0,     0,   401,     0,   402,   403,  1365,     0,     0,
       0,   895,     0,     0,     0,     0,     0,   404,     0,     0,
     896,   405,     0,     0,  1378,     0,     0,     0,   897,     0,
       0,     0,  1385,  1386,  1186,     0,     0,     0,     0,     0,
    1186,     0,  1189,  1393,   439,  1395,  1396,   898,     0,   899,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   900,     0,   901,   902,   903,     0,     0,
       0,     0,     0,     0,   510,     0,  1132,     0,   406,     0,
       0,     0,     0,     0,   407,     0,    62,     0,     0,     0,
     439,     0,     0,     0,   408,     0,     0,    63,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,   409,   410,
     411,   412,   212,   213,     0,     0,   904,    65,     0,     0,
       0,  1488,   413,     0,    99,     0,     0,     0,     0,     0,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     107,   423,     0,   109,     0,     0,   424,   425,     0,   426,
     427,   428,   112,   113,   114,   429,   115,     0,     0,   116,
     117,   118,   119,   120,     0,   100,     0,   121,   122,     0,
     510,     0,     0,   123,   430,   125,     0,   101,    66,   431,
     432,     0,     0,     0,     0,   501,     0,   434,     0,     0,
       0,   289,  1539,     0,    67,     0,     0,     0,     0,     0,
       0,  1547,   102,     0,     0,     0,     0,     0,  1552,     0,
       0,    68,   905,   906,   907,   706,   707,   708,   709,     0,
       0,     0,   943,     0,     0,     0,    69,     0,     0,     0,
      70,     0,     0,    71,     0,   106,     0,  1144,  1186,  1186,
    1131,    72,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,  1586,     0,
      73,     0,     0,     0,   107,   108,   396,   109,     0,     0,
       0,   103,   397,   398,   110,   111,   112,   113,   114,     0,
     115,   104,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125,
     399,     0,     0,   400,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1618,     0,     0,     0,   106,     0,     0,     0,     0,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,     0,   405,     0,     0,
       0,     0,     0,   107,   108,     0,   109,     0,     0,     0,
       0,     0,     0,   110,   111,   112,   113,   114,     0,   115,
       0,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   124,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1676,
    1677,     0,  1132,     0,   406,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,  1688,     0,     0,     0,
     408,  1186,     0,     0,  1691,     0,     0,     0,     0,     0,
    1698,     0,  1700,     0,   409,   410,   411,   412,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   107,   423,     0,   109,
       0,     0,   424,   425,     0,   426,   427,   428,   112,   113,
     114,   429,   115,     0,     0,   116,   117,   118,   119,   120,
       0,     0,     0,   121,   122,     0,     0,     0,     0,   123,
     430,   125,     0,     0,     0,   431,   432,     0,     0,     0,
    1725,   501,  1751,   434,     0,  1152,     0,   289,  1131,     0,
    1755,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,  1726,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,   439,     0,
     397,   398,     0,     0,   893,     0,     0,     0,     0,     0,
       0,     0,   704,     0,     0,     0,     0,   705,     0,     0,
     894,     0,     0,  1618,     0,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1727,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   895,   401,     0,   402,
     403,     0,     0,     0,     0,   896,     0,     0,     0,     0,
       0,   404,  1810,   897,     0,   405,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   898,     0,   899,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,   108,   900,   109,
     901,   902,   903,     0,     0,     0,   110,   111,   112,   113,
     114,     0,   115,     0,   397,   116,   117,   118,   119,   120,
    1132,     0,   406,   121,   122,     0,     0,     0,   407,   123,
     124,   125,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   904,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,   401,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   430,   125,
       0,     0,     0,   431,   432,  1155,     0,     0,  1131,   501,
       0,   434,   394,     0,     0,   289,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,   406,   905,   906,   907,
     706,   707,   708,   709,   396,     0,     0,     0,     0,     0,
     397,   398,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,   410,   411,     0,
     212,   213,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,     0,     0,
       0,     0,     0,     0,   424,   425,     0,   401,     0,   402,
     403,     0,     0,   429,     0,  1160,     0,     0,  1131,     0,
       0,   404,   394,     0,     0,   405,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,  1259,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
     397,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
    1132,   400,   406,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   404,     0,     0,     0,   405,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   430,   125,
    1132,     0,   406,   431,   432,     0,     0,     0,   407,   501,
       0,   434,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,    14,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     1,     0,     0,     0,   123,   430,   125,
       0,     0,     0,   431,   432,  1163,     0,     0,  1131,   501,
       0,   434,   394,     0,     2,   289,     0,     0,     0,     0,
       0,   395,     3,     0,   106,     0,     0,     0,     0,     4,
       0,     5,     0,     6,   396,     0,     0,     0,     7,     0,
     397,   398,     0,     0,     0,     8,     0,     0,     0,     0,
       0,     0,     0,   107,   108,     0,   109,     0,     0,     9,
       0,     0,     0,   110,   111,   112,   113,   114,   399,   115,
      10,   400,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   124,   125,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   996,   402,
     403,     0,     0,     0,     0,  1166,     0,     0,  1131,     0,
       0,   404,   394,     0,     0,   405,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,    11,     0,     0,
     397,   398,     0,     0,    12,     0,     0,    13,     0,    14,
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
    1132,   400,   406,     0,    16,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   404,     0,     0,     0,   405,   413,     0,     0,     0,
       0,     0,     0,    17,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,    18,    19,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   430,   125,
    1132,     0,   406,   431,   432,     0,     0,     0,   407,   501,
       0,   434,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,   394,   426,   427,   428,   112,   113,   114,   429,
     115,   395,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,   396,     0,     0,   123,   430,   125,
     397,   398,     0,   431,   432,     0,     0,     0,     0,   501,
       0,   434,     0,     0,     0,   289,     0,     0,     0,   653,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   394,     0,     0,   405,     0,     0,     0,     0,
       0,   395,     0,   883,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,   652,     0,     0,
     397,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   653,
       0,     0,   697,     0,     0,     0,   655,     0,   399,     0,
       0,   400,   406,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   404,     0,     0,     0,   405,   413,     0,     0,     0,
       0,     0,     0,   654,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,   884,   885,
     886,   121,   122,     0,     0,     0,   655,   123,   430,   125,
       0,     0,   406,   431,   432,     0,     0,     0,   407,   659,
       0,   434,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   968,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,   394,   426,   427,   428,   112,   113,   114,   429,
     115,   395,     0,   116,   117,   656,   657,   658,     0,     0,
       0,   121,   122,     0,   396,     0,     0,   123,   430,   125,
     397,   398,     0,   431,   432,     0,     0,     0,     0,   659,
       0,   434,     0,     0,     0,   289,     0,     0,     0,   653,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   971,   401,     0,   402,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   394,   106,     0,   405,     0,     0,     0,     0,
       0,   395,     0,   879,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
     397,   398,   107,   108,     0,   109,     0,     0,     0,     0,
       0,     0,   110,   111,   112,   113,   114,     0,   115,   653,
       0,   116,   117,   118,   119,   120,   655,     0,   399,   121,
     122,   400,   406,     0,     0,   123,   124,   125,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   404,     0,     0,     0,   405,   413,     0,     0,     0,
       0,     0,     0,   654,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   656,   657,   658,     0,     0,
       0,   121,   122,     0,     0,     0,   655,   123,   430,   125,
       0,     0,   406,   431,   432,     0,     0,     0,   407,   659,
       0,   434,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,   394,   426,   427,   428,   112,   113,   114,   429,
     115,   395,     0,   116,   117,   656,   657,   658,     0,     0,
       0,   121,   122,     0,   396,     0,     0,   123,   430,   125,
     397,   398,     0,   431,   432,     0,     0,     0,     0,   659,
       0,   434,     0,     0,     0,   289,     0,     0,     0,   653,
       0,   477,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   394,     0,     0,   405,   107,   108,     0,   109,
       0,   395,     0,   879,     0,     0,   110,   111,   112,   113,
     114,     0,   115,     0,   396,   116,   117,   118,   119,   120,
     397,   398,     0,   121,   122,     0,     0,     0,     0,   123,
     124,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   655,     0,   399,     0,
       0,   400,   406,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,   404,     0,     0,     0,   405,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   430,   125,
       0,     0,   406,   431,   432,     0,     0,     0,   407,   659,
       0,   434,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,   596,     0,   413,     0,   597,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,   598,   109,     0,   599,
     424,   425,   394,   426,   427,   428,   112,   113,   114,   429,
     115,   395,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,   396,     0,     0,   123,   430,   125,
     397,   398,     0,   431,   432,   506,     0,     0,     0,   501,
     600,   434,     0,     0,     0,   289,   601,   602,   603,     0,
     604,   605,   606,     0,   607,     0,     0,     0,   399,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   608,     0,   609,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   402,
     403,   106,     0,     0,     0,     0,     0,     0,     0,     0,
     394,   404,     0,     0,     0,   405,     0,   610,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   108,   396,   109,   611,     0,     0,     0,   397,   398,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,   612,   121,   122,     0,
       0,     0,     0,   123,   124,   125,   399,     0,     0,   400,
       0,     0,   406,     0,     0,  1840,     0,   434,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
       0,   613,   614,     0,     0,   401,   688,   402,   403,     0,
       0,     0,   409,   410,   411,   412,   212,   213,   615,   404,
       0,     0,     0,   405,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   107,   423,     0,   109,     0,     0,
     424,   425,     0,   426,   427,   428,   112,   113,   114,   429,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
     697,   121,   122,     0,     0,   956,     0,   123,   430,   125,
     406,     0,     0,   431,   432,     0,   407,     0,     0,   501,
       0,   434,     0,     0,     0,   289,   408,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,   410,   411,   412,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,   413,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   107,   423,     0,   109,     0,     0,   424,   425,
     394,   426,   427,   428,   112,   113,   114,   429,   115,   395,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,   396,     0,     0,   123,   430,   125,   397,   398,
       0,   431,   432,     0,     0,     0,     0,   501,     0,   434,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,   402,   403,   106,
       0,     0,     0,     0,     0,     0,     0,     0,   394,   404,
       0,     0,     0,   405,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   107,   108,
     396,   109,     0,     0,     0,     0,   397,   398,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,   124,   125,   399,     0,     0,   400,     0,   957,
     406,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   408,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,     0,     0,     0,
     409,   410,   411,   412,   212,   213,     0,   404,     0,     0,
       0,   405,     0,     0,   413,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   107,   423,     0,   109,     0,     0,   424,   425,
       0,   426,   427,   428,   112,   113,   114,   429,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,   430,   125,   406,     0,
       0,   431,   432,     0,   407,     0,     0,   501,   954,   434,
       0,     0,     0,   289,   408,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1158,     0,     0,   409,   410,
     411,   412,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     107,   423,     0,   109,     0,     0,   424,   425,   394,   426,
     427,   428,   112,   113,   114,   429,   115,   395,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
     396,     0,     0,   123,   430,   125,   397,   398,     0,   431,
     432,     0,     0,     0,     0,   501,     0,   434,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,   402,   403,   106,     0,     0,
       0,     0,     0,     0,     0,     0,   394,   404,     0,     0,
       0,   405,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,   108,   396,   109,
       0,     0,     0,     0,   397,   398,   110,   111,   112,   113,
     114,     0,   115,     0,     0,   116,   117,   118,   119,   120,
       0,     0,     0,   121,   122,     0,     0,     0,  1346,   123,
     124,   125,   399,     0,     0,   400,     0,     0,   406,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   408,     0,     0,     0,     0,     0,
       0,   401,     0,   402,   403,     0,     0,     0,   409,   410,
     411,   412,   212,   213,     0,   404,     0,     0,     0,   405,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     107,   423,     0,   109,     0,     0,   424,   425,     0,   426,
     427,   428,   112,   113,   114,   429,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,   430,   125,   406,     0,     0,   431,
     432,     0,   407,     0,     0,   501,     0,   434,     0,     0,
       0,   289,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1359,     0,     0,   409,   410,   411,   412,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   107,   423,
       0,   109,     0,     0,   424,   425,   394,   426,   427,   428,
     112,   113,   114,   429,   115,   395,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,   396,     0,
       0,   123,   430,   125,   397,   398,     0,   431,   432,     0,
       0,     0,     0,   501,     0,   434,     0,     0,     0,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,   402,   403,   106,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   404,     0,     0,     0,   405,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   108,   396,   109,     0,     0,
       0,     0,   397,   398,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125,
     399,     0,     0,   400,     0,     0,   406,     0,     0,     0,
       0,   434,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   408,     0,     0,     0,     0,     0,     0,   401,
       0,   402,   403,     0,     0,     0,   409,   410,   411,   412,
     212,   213,     0,   404,     0,     0,     0,   405,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   107,   423,
       0,   109,     0,     0,   424,   425,     0,   426,   427,   428,
     112,   113,   114,   429,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,   430,   125,   406,     0,     0,   431,   432,     0,
     407,  1697,     0,   501,     0,   434,     0,   968,     0,   289,
     408,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,   410,   411,   412,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   107,   423,     0,   109,
       0,     0,   424,   425,   394,   426,   427,   428,   112,   113,
     114,   429,   115,   395,     0,   116,   117,   118,   119,   120,
       0,   969,     0,   121,   122,     0,   396,     0,     0,   123,
     430,   125,   397,   398,     0,   431,   432,     0,     0,     0,
       0,   433,     0,   434,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   970,     0,     0,
     399,     0,   971,   400,     0,     0,     0,     0,   972,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,   973,     0,   401,
       0,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,     0,   405,   107,   108,
       0,   109,     0,     0,     0,     0,     0,  1204,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,     0,     0,     0,
       0,   123,   124,   125,     0,     0,     0,     6,     0,     0,
       0,     0,     0,     0,  -285,     0,     0,     0,  1404,  1405,
       0,  1406,     0,     0,   406,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,  1407,     0,
     408,     0,     0,     0,  1408,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,   410,   411,   412,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   107,   423,     0,   109,
       0,     0,   424,   425,     0,   426,   427,   428,   112,   113,
     114,   429,   115,     0,  1409,   116,   117,   118,   119,   120,
    1204,     0,     0,   121,   122,     0,     0,     0,     0,   123,
     430,   125,     0,    14,     0,   431,   432,     0,     0,     0,
       0,   501,     0,   434,     0,     0,     0,   289,  1410,     0,
       6,     0,     0,     0,     0,     0,     0,  -287,    16,     0,
       0,  1404,  1405,     0,  1406,     0,     0,  1596,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,  1407,     0,     0,     0,     0,     0,  1408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1411,   108,     0,   109,     0,
       0,     0,   106,     0,     0,   110,   111,   112,   113,   114,
       0,   115,     0,     0,   116,  1412,   118,   119,   120,  1204,
       0,     0,   121,   122,  1413,  1414,  1415,     0,   123,   124,
     125,   107,   108,     0,   109,     0,     0,  1409,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     6,
     116,   117,   118,   119,   120,     0,    14,     0,   121,   122,
    1404,  1405,     0,  1406,   123,   124,   125,     0,     0,     0,
       0,  1410,     0,     0,     0,     0,   996,     0,     0,     0,
    1407,    16,     0,     0,     0,     0,  1408,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   596,     0,     0,     0,   597,     0,     0,     0,     0,
       0,     0,   796,     0,     0,     0,     0,     0,  1411,   108,
       0,   109,     0,   598,     0,     0,   599,     0,   110,   111,
     112,   113,   114,     0,   115,     0,  1409,   116,  1412,   118,
     119,   120,     0,     0,     0,   121,   122,  1413,  1414,  1415,
       0,   123,   124,   125,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
    1410,     0,     0,   601,   602,   603,     0,   604,   605,   606,
      16,   607,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
       0,   608,     0,   609,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1411,   108,     0,
     109,     0,     0,     0,   610,     0,     0,   110,   111,   112,
     113,   114,     0,   115,     0,     0,   116,  1412,   118,   119,
     120,   611,     0,     0,   121,   122,  1413,  1414,  1415,     0,
     123,   124,   125,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   612,     0,     0,     0,     0,     0,     0,
       0,   596,     0,     0,     0,   597,     0,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   598,     0,     0,   599,     0,   613,   614,
       0,     0,     0,     0,     0,     0,     0,   107,   108,     0,
     109,     0,     0,     0,     0,   615,     0,   110,   111,   112,
     113,   114,     0,   115,     0,     0,   116,   117,   118,   119,
     120,     0,     0,     0,   121,   122,     0,   600,     0,     0,
     123,   124,   125,   601,   602,   603,   106,   604,   605,   606,
       0,   607,   869,     0,     0,     0,   597,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     870,   608,     0,   609,   598,   107,   108,   599,   109,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
       0,   115,     0,     0,   116,   117,   118,   119,   120,     0,
       0,     0,   121,   122,   610,     0,     0,     0,   123,   124,
     125,     0,     0,     0,     0,     0,     0,     0,   600,     0,
    1745,   611,     0,     0,   601,   602,   603,     0,   604,   605,
     606,     0,   607,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   612,     0,     0,     0,     0,     0,     0,
       0,     0,   608,     0,   609,     0,     0,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   613,   614,
       0,     0,     0,     0,     0,   610,     0,   107,   108,     0,
     109,     0,     0,     0,     0,   615,     0,   110,   111,   112,
     113,   114,   611,   115,     0,   106,   116,   117,   118,   119,
     120,     0,     0,     0,   121,   122,     0,     0,     0,     0,
     123,   124,   125,     0,   612,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   108,     0,   109,     0,     0,
       0,   106,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,   613,
     614,   121,   122,     0,     0,     0,     0,   123,   124,   125,
     107,   108,     0,   109,     0,     0,   615,  1580,     0,     0,
     110,   111,   112,   113,   114,    76,   115,   106,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,   124,   125,     0,     0,     0,     0,
       0,     0,     0,  1589,     0,     0,   107,   108,     0,   109,
       0,     0,     0,   106,     0,     0,   110,   111,   112,   113,
     114,     0,   115,     0,     0,   116,   117,   118,   119,   120,
       0,     0,     0,   121,   122,     0,     0,     0,     0,   123,
     124,   125,   107,   108,     0,   109,    77,     0,     0,     0,
       0,     0,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,     0,     0,     0,     0,   123,   124,   125,   106,     0,
       0,     0,     0,   957,     0,     0,     0,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,     0,     0,   184,   185,   107,   108,     0,
     109,     0,     0,   186,   187,   106,     0,   110,   111,   112,
     113,   114,   188,   115,   189,     0,   116,   117,   118,   119,
     120,     0,     0,     0,   121,   122,   190,   191,   192,   193,
     123,   124,   125,     0,   107,   108,     0,   109,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125
};

static const short yycheck[] =
{
       9,    16,    55,    12,   105,     4,    64,    16,     0,    18,
     372,   526,   144,   145,   144,   145,    63,   154,   368,   372,
     468,   343,   878,   567,  1203,    63,   882,    85,    91,   712,
      70,   512,    56,   538,    94,   364,    89,    73,   489,  1047,
     719,   964,   341,    67,    53,    54,    55,    56,    57,    58,
      62,   941,  1276,    93,    63,    64,    65,  1037,    67,    95,
      69,    70,    88,    72,    73,   697,    90,  1047,    83,  1044,
      68,   738,    81,    97,    83,    84,    85,    92,    87,    88,
      89,    90,   996,    92,    93,    94,    95,   770,    97,   389,
     350,  1419,    57,    91,   648,   275,   986,   135,   158,   137,
     291,   433,   734,   228,    69,   285,  1075,  1221,   349,   641,
     863,   559,  1098,   561,   162,   318,  1396,   377,  1093,   567,
       8,     8,     8,   149,     8,  1242,   135,   136,   137,     8,
       8,     6,   156,   688,   805,    50,   237,   495,     8,   694,
     149,  1037,  1525,   738,    10,   154,  1037,   156,   157,   158,
      23,  1047,  1185,   162,  1037,    66,  1047,  1037,  1191,  1037,
    1213,    94,  1215,     9,  1047,   165,    20,  1047,   140,  1047,
      10,   136,    64,   101,    49,   654,    50,  1041,  1458,    64,
       8,   318,  1274,    46,  1048,   197,    26,    29,    84,    46,
      12,    31,   157,  1057,  1324,   545,    11,   322,    26,    10,
     204,    24,     8,    31,     6,   567,   182,     8,   281,  1099,
     222,    26,    58,   654,   567,    26,    31,  1081,   659,    26,
      31,    20,    66,    69,    31,   326,    66,   265,   198,   148,
     268,    83,   747,    48,  1413,  1414,    46,    11,    46,   148,
       6,   201,   592,     6,   132,   132,   132,     4,   132,   302,
       7,   163,   204,   132,   132,   239,   265,    66,   151,   268,
    1268,  1269,   132,   272,    26,   249,   275,   276,   120,    31,
     569,   531,   281,  1248,    48,   184,   285,   170,   190,   231,
     318,   133,   286,    49,    94,    53,    54,    47,  1268,  1269,
      98,    12,   188,   302,  1302,    52,   596,   306,     4,    98,
      47,   124,   482,   279,   342,     8,     8,   272,   280,   279,
     238,   349,   979,   351,   199,   353,   281,   616,   356,   141,
       8,  1235,  1302,    26,   991,   992,   132,   173,    31,   329,
     263,   132,   385,   342,   773,   375,   891,   892,   182,  1308,
     349,   306,   351,   185,   353,  1462,    52,   356,  1271,   268,
     374,   790,   489,   365,   793,   367,  1389,  1390,   212,   840,
    1743,   163,    26,   802,   279,   374,   375,    31,   279,   381,
     818,   703,  1268,  1269,   383,   181,   385,  1268,  1269,   711,
     935,  1052,  1053,   741,   979,  1268,  1269,   279,  1268,  1269,
    1268,  1269,  1315,   281,   281,   281,   991,   281,  1490,  1529,
     163,   276,   281,   281,   883,   279,  1302,  1460,   271,   279,
     283,  1302,  1636,   279,   271,  1594,    26,   856,   383,  1302,
     141,    31,  1302,   496,  1302,   434,    26,  1291,   188,   197,
     132,    31,  1103,    29,   274,   275,   276,   277,   879,   279,
      26,   188,   883,  1771,   132,    31,   274,   275,   276,   277,
     193,   489,   500,   263,   584,    14,   157,   481,   164,   274,
     275,   276,   277,   274,   275,   276,   277,    47,   477,   276,
     277,   495,   481,   482,    23,  1755,   182,   148,  1806,   488,
      47,  1036,    59,   492,   238,   784,   495,   496,   787,   151,
    1692,   500,    71,   181,    26,  1225,  1610,    53,    54,    31,
       3,   800,   149,   200,    84,    24,  1745,   632,    19,   200,
     216,  1735,   477,     5,    33,     3,   270,    84,  1720,   166,
    1848,  1200,   585,   488,    16,    26,   825,   492,  1503,   567,
      31,   496,    94,   984,   130,   547,  1522,   280,   279,  1572,
     202,  1869,    91,   284,  1872,   867,  1521,    65,  1750,   729,
     791,  1474,   111,   744,  1468,  1109,   565,  1796,   567,  1887,
    1092,  1314,   571,   766,  1231,  1789,  1121,  1122,  1537,  1124,
    1125,   274,   275,   276,   277,   587,   876,    80,    47,   280,
    1019,   204,   279,   254,   716,    26,   163,   585,   279,   185,
      31,  1321,    80,    26,   854,  1834,    40,   176,    31,  1823,
    1039,  1040,   640,  1026,   183,   124,   202,    51,   188,   177,
     274,   275,   276,   277,    58,    84,    26,   281,  1801,  1483,
    1059,    31,   752,  1487,  1488,   637,   638,   146,   146,   766,
      19,   640,  1815,    26,   153,  1837,   204,  1479,    31,    83,
     649,   197,    16,    26,     6,   207,   208,   204,    31,   252,
     649,  1222,  1542,   679,   292,   987,   278,   280,   282,  1523,
     102,   285,   300,   286,   274,   275,   276,   277,   187,   187,
     679,   281,   177,   155,   274,   275,   276,   277,   278,   717,
    1103,   719,  1524,   721,   153,  1693,   133,    49,   274,   275,
     276,   277,  1871,   279,   163,   704,   204,    86,   204,   204,
    1883,  1030,  1257,   712,   148,  1757,    26,   275,   717,    83,
     719,    31,   721,  1693,  1346,   724,   123,   741,   286,   188,
     729,   123,   279,    89,  1279,  1648,  1297,  1779,   766,   738,
      26,  1654,   741,   175,   772,    31,   180,   746,    98,   186,
     184,   750,   274,   275,   276,   277,  1798,  1046,   114,   281,
     279,     7,   761,   791,   763,   284,   819,   164,  1622,  1058,
      68,   770,  1626,   772,   124,  1094,    86,    18,   780,   275,
     275,    68,  1277,   274,   275,   276,   277,  1641,   279,   817,
     818,  1080,   791,    34,  1106,    26,   849,   850,   968,   852,
      31,   971,   194,   102,  1658,    26,   761,  1693,   763,   204,
      31,  1838,  1693,  1135,   211,   204,   172,  1780,   817,   818,
    1693,    33,  1849,  1693,  1866,  1693,  1867,     6,   830,  1300,
     204,   819,  1747,  1874,    46,  1306,   824,   280,    79,     6,
     139,  1375,   141,   274,   275,   276,   277,   147,  1603,   837,
     281,   274,   275,   276,   277,    18,   855,   984,   281,  1171,
    1901,   849,   850,    26,   852,   164,   865,   108,    31,  1896,
      49,    34,     6,  1195,   274,   275,   276,   277,   877,  1794,
     275,   281,    49,   279,  1313,    67,   280,  1850,  1742,   129,
     189,   274,   275,   276,   277,   239,   240,  1751,   281,    81,
     855,   274,   275,   276,   277,   148,   280,    70,   281,    26,
     865,    67,   286,  1759,    31,    49,    79,   131,  1833,  1241,
     246,   279,   972,   105,    87,    81,   284,  1472,   254,   129,
     177,    26,     0,    89,   175,     4,    31,   180,   129,  1793,
     970,   129,    10,   106,   279,   108,   281,   973,    17,   105,
    1865,   240,   195,   124,   182,   969,   984,   204,  1078,   122,
    1398,   124,   125,   126,   274,   275,   276,   277,   278,   968,
     969,   970,   971,   972,   973,   146,    26,   159,  1318,    83,
     979,    31,   153,    52,   279,    54,   281,   164,   274,   275,
     276,   277,   991,   992,    20,   281,    64,   996,  1026,    67,
    1289,    67,   279,   159,   281,    26,    74,  1762,   204,    77,
      31,  1766,   175,    81,   996,    81,   187,   267,    86,  1774,
      26,    89,  1777,  1375,   211,    31,   213,  1026,   275,    26,
     170,  1320,  1375,   280,    31,   231,  1089,   105,   280,   105,
    1585,   207,   208,   274,   275,   276,   277,   279,  1803,   281,
     281,    10,  1105,   274,   275,   276,   277,   279,  1111,   281,
     281,   129,   279,   129,   281,   133,    62,  1213,  1096,  1215,
    1069,  1576,   207,    26,   142,  1103,    64,  1204,    31,   275,
      62,    63,    26,  1072,  1762,  1840,  1085,    31,  1766,    82,
    1845,   159,    83,   159,   137,   138,  1774,  1096,  1220,  1221,
    1222,  1089,   278,   279,  1103,  1778,  1611,   206,   271,   272,
     273,   274,   275,   276,   277,    67,   198,  1105,   186,    86,
     283,    26,   285,  1111,   157,  1803,    31,   278,   279,    81,
     198,    18,   200,  1253,   279,  1255,   281,   148,   279,    26,
     281,   898,   280,   900,    31,   902,   903,    34,   905,   906,
     907,    26,   279,   105,   281,  1660,    31,   274,   275,   276,
     277,    66,  1840,   279,   281,   281,  1203,  1845,    26,  1714,
    1899,  1900,  1200,    31,    66,  1203,  1204,   129,  1723,   274,
     275,   276,   277,    70,    26,   279,   281,   281,    26,    31,
      64,    66,    79,    31,   148,   279,  1316,   281,   129,   267,
      87,  1200,   129,  1492,  1203,  1204,    64,   159,   252,   182,
     278,   279,   107,   281,  1213,    26,  1215,   196,   180,   106,
      31,   108,   130,   163,   274,   275,   276,   277,   279,    26,
     281,   281,  1231,    64,    31,   122,  1235,   124,   125,   126,
     262,   279,  1531,   281,   263,   279,    57,   281,    86,   262,
     279,    18,   281,   274,   275,   276,   277,    33,   215,    26,
     281,   203,   204,   205,    31,   279,   271,    34,   274,   275,
     276,   277,   118,    32,     6,   281,   280,   274,   275,   276,
     277,    10,   280,   264,   281,   279,    26,   281,   175,   231,
     232,    31,  1419,   280,   279,  1800,   281,   280,   207,   283,
     280,     6,   280,    70,   280,   280,  1295,   288,   280,   280,
     280,   207,    79,   207,   280,    47,   297,    57,    10,   280,
      87,   274,   275,   276,   277,   280,   271,    59,   281,  1331,
     274,   275,   276,   277,  1336,   279,   279,  1882,   285,   106,
     182,   108,    47,  1470,   280,   280,   285,   328,   280,   182,
     331,    66,    84,   148,    59,   122,  1404,   124,   125,   126,
     279,   279,    88,    66,   279,   174,  1655,   271,    73,   274,
     275,   276,   277,   285,   675,    10,   677,   280,   279,    84,
     681,   682,   683,    30,   271,   272,   273,   274,   275,   276,
     277,  1419,   188,   200,    94,    94,   283,   123,   285,   274,
     275,   276,   277,    94,    24,  1404,    24,    46,   175,    10,
     182,   216,   143,  1412,  1413,  1414,   274,   275,   276,   277,
    1419,   153,   182,   143,   204,   151,    23,    94,   154,   193,
     193,   163,   274,   275,   276,   277,   274,   275,   276,   277,
     280,   145,    51,    95,   170,   426,   280,   283,   153,    58,
     431,   432,   433,    62,    63,   280,   188,   280,   163,    64,
      10,  1460,    71,   274,   275,   276,   277,   129,   157,  1468,
     196,  1470,   135,   279,    83,   128,  1603,   274,   275,   276,
     277,   280,   280,   188,   197,  1612,   286,   281,  1610,    23,
      45,   210,   182,    63,  1493,   280,  1603,    66,    66,   194,
     198,   206,    10,   248,   271,   272,   273,   274,   275,   276,
     277,   128,   251,    20,   280,   280,   283,    46,   285,    30,
     501,    24,   132,    90,   505,    90,   132,   164,   132,   162,
     235,   236,    46,   238,   274,   275,   276,   277,  1493,   148,
     245,   246,   247,   248,   249,   164,   251,   280,  1639,   254,
     255,   256,   257,   258,   102,   163,   165,   262,   263,   206,
     163,    66,   280,   268,   269,   270,   280,    26,  1582,   281,
     279,   180,    17,   279,   131,   184,   192,   113,   279,    66,
      74,    12,   279,  1582,   280,    10,   195,   280,   235,   236,
     247,   238,   280,   168,   281,  1594,   282,  1640,   245,   246,
     247,   248,   249,   281,   251,   214,   281,   254,   255,   256,
     257,   258,   281,  1612,    10,   262,   263,   200,  1600,   140,
     186,   268,   269,   270,     8,    55,    55,   281,    98,   181,
     281,  1630,   280,   280,   280,  1762,  1635,   280,   280,  1766,
     280,  1640,   280,    54,  1771,  1736,   280,  1774,   280,   280,
    1777,   280,   280,   280,  1659,  1762,   280,   280,  1706,  1766,
    1659,    73,  1664,   134,    20,   280,  1668,  1774,   280,   134,
    1777,   134,   279,    89,    77,   137,  1803,   279,   659,  1806,
    1635,   218,   219,   220,   221,   222,   223,   224,   225,   281,
     281,   132,   153,   132,   132,   676,  1803,   132,   132,   680,
     132,   281,   204,   281,    24,   281,   687,  1706,  1707,   280,
     163,   692,   693,  1840,    54,   181,   278,   281,  1845,  1708,
     281,  1848,   703,   281,   705,   706,   707,   708,   709,   710,
     711,    39,    39,  1840,   278,   280,    86,   278,  1845,   278,
    1745,    54,  1869,  1771,    86,  1872,  1745,   278,   278,   282,
     278,   278,   278,   131,   182,    10,   142,    26,    83,    47,
    1887,   188,    84,    84,   188,    47,   281,   748,   281,    98,
      66,   131,  1771,   117,   117,   281,   197,   281,  1806,  1778,
    1785,  1780,   281,   281,   281,   281,  1785,    64,   769,     6,
     281,  1796,     9,   774,   278,   776,    13,  1796,   281,   281,
     281,   281,   281,   281,   281,    22,   281,  1806,   281,   281,
      90,   280,   279,  1856,    98,   212,   280,  1822,    35,    86,
    1848,   281,   278,  1822,    41,    42,    86,   285,   254,  1834,
     280,   114,   280,   279,   129,  1834,   181,   156,   129,   129,
      89,  1869,   279,    50,  1872,   281,   279,    47,  1853,  1848,
     188,  1850,    69,    39,  1853,    72,   278,  1856,   281,  1887,
     133,   281,   281,    53,    83,    50,   280,   280,   209,     0,
    1869,   281,  1871,  1872,   279,   281,     0,    33,  1877,   273,
    1218,    98,    12,   100,   101,   276,   285,   644,  1887,  1109,
     729,   868,  1095,  1106,  1512,   112,   339,  1033,   592,   116,
    1899,  1900,   772,  1096,   351,  1103,   324,  1511,   801,  1577,
    1490,   351,   893,   894,   895,   369,  1787,   898,   899,   900,
    1492,   902,   903,   904,   905,   906,   907,   908,   909,  1791,
     911,  1790,   913,  1825,   791,   916,   917,   918,   919,   920,
     921,  1344,  1345,  1598,  1699,   965,  1772,  1350,  1351,   717,
    1353,  1354,   933,  1250,   171,   936,   173,  1021,  1361,  1362,
    1363,  1364,   179,  1366,  1367,  1368,  1369,  1370,  1371,  1745,
    1475,   344,   189,   573,   356,   337,   847,  1304,   839,   629,
      13,   774,   310,   518,   761,   147,   203,   204,   205,   206,
     207,   208,   514,   748,   996,  1724,  1620,  1822,  1853,     0,
     217,     0,   526,     0,  1259,  1149,   987,   737,  1193,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     988,   238,  1085,  1630,   241,   242,    87,   244,   245,   246,
     247,   248,   249,   250,   251,  1412,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,   571,    -1,
     206,   268,   269,   270,    -1,    -1,    -1,   274,   275,    -1,
      -1,    -1,    -1,   280,    -1,   282,    -1,    -1,    -1,   286,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,     6,   238,    -1,     9,    -1,    -1,    -1,    13,   245,
     246,   247,   248,   249,    -1,   251,    -1,    22,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      35,    -1,   268,   269,   270,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    31,    -1,    69,    34,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,  1135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1147,  1148,    -1,  1150,
    1151,    -1,    -1,    98,    -1,   100,   101,  1158,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      79,   116,    -1,    -1,  1175,    -1,    -1,    -1,    87,    -1,
      -1,    -1,  1183,  1184,  1185,    -1,    -1,    -1,    -1,    -1,
    1191,    -1,  1193,  1194,  1195,  1196,  1197,   106,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,  1225,    -1,   171,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    40,    -1,    -1,    -1,
    1241,    -1,    -1,    -1,   189,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,   175,    71,    -1,    -1,
      -1,  1272,   217,    -1,     6,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    47,    -1,   262,   263,    -1,
    1321,    -1,    -1,   268,   269,   270,    -1,    59,   132,   274,
     275,    -1,    -1,    -1,    -1,   280,    -1,   282,    -1,    -1,
      -1,   286,  1343,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,  1352,    84,    -1,    -1,    -1,    -1,    -1,  1359,    -1,
      -1,   165,   271,   272,   273,   274,   275,   276,   277,    -1,
      -1,    -1,   281,    -1,    -1,    -1,   180,    -1,    -1,    -1,
     184,    -1,    -1,   187,    -1,   206,    -1,     6,  1389,  1390,
       9,   195,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,  1409,    -1,
     214,    -1,    -1,    -1,   235,   236,    35,   238,    -1,    -1,
      -1,   153,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,   163,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
      69,    -1,    -1,    72,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1471,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1550,
    1551,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,  1567,    -1,    -1,    -1,
     189,  1572,    -1,    -1,  1575,    -1,    -1,    -1,    -1,    -1,
    1581,    -1,  1583,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,    -1,    -1,   274,   275,    -1,    -1,    -1,
      89,   280,  1663,   282,    -1,     6,    -1,   286,     9,    -1,
    1671,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,  1699,    -1,
      41,    42,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,  1724,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    98,    -1,   100,
     101,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,   112,  1773,    87,    -1,   116,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,   122,   238,
     124,   125,   126,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    41,   254,   255,   256,   257,   258,
     171,    -1,   173,   262,   263,    -1,    -1,    -1,   179,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   175,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    98,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,   274,   275,     6,    -1,    -1,     9,   280,
      -1,   282,    13,    -1,    -1,   286,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,   173,   271,   272,   273,
     274,   275,   276,   277,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,    -1,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,    -1,    -1,
      -1,    -1,    -1,    -1,   241,   242,    -1,    98,    -1,   100,
     101,    -1,    -1,   250,    -1,     6,    -1,    -1,     9,    -1,
      -1,   112,    13,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   275,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
     171,    72,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
     171,    -1,   173,   274,   275,    -1,    -1,    -1,   179,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   163,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,     7,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,   274,   275,     6,    -1,    -1,     9,   280,
      -1,   282,    13,    -1,    28,   286,    -1,    -1,    -1,    -1,
      -1,    22,    36,    -1,   206,    -1,    -1,    -1,    -1,    43,
      -1,    45,    -1,    47,    35,    -1,    -1,    -1,    52,    -1,
      41,    42,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    73,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    69,   251,
      84,    72,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,   280,   100,
     101,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,    -1,
      -1,   112,    13,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,   151,    -1,    -1,
      41,    42,    -1,    -1,   158,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
     171,    72,   173,    -1,   188,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   217,    -1,    -1,    -1,
      -1,    -1,    -1,   237,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,   252,   253,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
     171,    -1,   173,   274,   275,    -1,    -1,    -1,   179,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    13,   244,   245,   246,   247,   248,   249,   250,
     251,    22,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    35,    -1,    -1,   268,   269,   270,
      41,    42,    -1,   274,   275,    -1,    -1,    -1,    -1,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    13,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    38,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,   163,    -1,    -1,    -1,   167,    -1,    69,    -1,
      -1,    72,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   217,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,    -1,    -1,    -1,   167,   268,   269,   270,
      -1,    -1,   173,   274,   275,    -1,    -1,    -1,   179,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    13,   244,   245,   246,   247,   248,   249,   250,
     251,    22,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    35,    -1,    -1,   268,   269,   270,
      41,    42,    -1,   274,   275,    -1,    -1,    -1,    -1,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    98,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    13,   206,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    22,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    60,
      -1,   254,   255,   256,   257,   258,   167,    -1,    69,   262,
     263,    72,   173,    -1,    -1,   268,   269,   270,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   217,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,   167,   268,   269,   270,
      -1,    -1,   173,   274,   275,    -1,    -1,    -1,   179,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    13,   244,   245,   246,   247,   248,   249,   250,
     251,    22,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    35,    -1,    -1,   268,   269,   270,
      41,    42,    -1,   274,   275,    -1,    -1,    -1,    -1,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,    -1,    60,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    13,    -1,    -1,   116,   235,   236,    -1,   238,
      -1,    22,    -1,   124,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    35,   254,   255,   256,   257,   258,
      41,    42,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    69,    -1,
      -1,    72,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,   173,   274,   275,    -1,    -1,    -1,   179,   280,
      -1,   282,    -1,    -1,    -1,   286,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   217,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    41,   238,    -1,    44,
     241,   242,    13,   244,   245,   246,   247,   248,   249,   250,
     251,    22,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    35,    -1,    -1,   268,   269,   270,
      41,    42,    -1,   274,   275,   276,    -1,    -1,    -1,   280,
      85,   282,    -1,    -1,    -1,   286,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,   112,    -1,    -1,    -1,   116,    -1,   152,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    35,   238,   169,    -1,    -1,    -1,    41,    42,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,   191,   262,   263,    -1,
      -1,    -1,    -1,   268,   269,   270,    69,    -1,    -1,    72,
      -1,    -1,   173,    -1,    -1,   280,    -1,   282,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,   226,   227,    -1,    -1,    98,   197,   100,   101,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,   243,   112,
      -1,    -1,    -1,   116,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
     163,   262,   263,    -1,    -1,    42,    -1,   268,   269,   270,
     173,    -1,    -1,   274,   275,    -1,   179,    -1,    -1,   280,
      -1,   282,    -1,    -1,    -1,   286,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    35,    -1,    -1,   268,   269,   270,    41,    42,
      -1,   274,   275,    -1,    -1,    -1,    -1,   280,    -1,   282,
      -1,    -1,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,   269,   270,    69,    -1,    -1,    72,    -1,   276,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,   269,   270,   173,    -1,
      -1,   274,   275,    -1,   179,    -1,    -1,   280,   281,   282,
      -1,    -1,    -1,   286,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    13,   244,
     245,   246,   247,   248,   249,   250,   251,    22,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      35,    -1,    -1,   268,   269,   270,    41,    42,    -1,   274,
     275,    -1,    -1,    -1,    -1,   280,    -1,   282,    -1,    -1,
      -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,   163,   268,
     269,   270,    69,    -1,    -1,    72,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,   269,   270,   173,    -1,    -1,   274,
     275,    -1,   179,    -1,    -1,   280,    -1,   282,    -1,    -1,
      -1,   286,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    13,   244,   245,   246,
     247,   248,   249,   250,   251,    22,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    35,    -1,
      -1,   268,   269,   270,    41,    42,    -1,   274,   275,    -1,
      -1,    -1,    -1,   280,    -1,   282,    -1,    -1,    -1,   286,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    35,   238,    -1,    -1,
      -1,    -1,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
      69,    -1,    -1,    72,    -1,    -1,   173,    -1,    -1,    -1,
      -1,   282,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,   269,   270,   173,    -1,    -1,   274,   275,    -1,
     179,   278,    -1,   280,    -1,   282,    -1,    74,    -1,   286,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    13,   244,   245,   246,   247,   248,
     249,   250,   251,    22,    -1,   254,   255,   256,   257,   258,
      -1,   148,    -1,   262,   263,    -1,    35,    -1,    -1,   268,
     269,   270,    41,    42,    -1,   274,   275,    -1,    -1,    -1,
      -1,   280,    -1,   282,    -1,    -1,    -1,   286,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      69,    -1,   189,    72,    -1,    -1,    -1,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    17,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,   269,   270,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    58,    59,
      -1,    61,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
     189,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,   144,   254,   255,   256,   257,   258,
      17,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,   163,    -1,   274,   275,    -1,    -1,    -1,
      -1,   280,    -1,   282,    -1,    -1,    -1,   286,   178,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    54,   188,    -1,
      -1,    58,    59,    -1,    61,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    -1,   206,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    17,
      -1,    -1,   262,   263,   264,   265,   266,    -1,   268,   269,
     270,   235,   236,    -1,   238,    -1,    -1,   144,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    47,
     254,   255,   256,   257,   258,    -1,   163,    -1,   262,   263,
      58,    59,    -1,    61,   268,   269,   270,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,   280,    -1,    -1,    -1,
      78,   188,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    41,    -1,    -1,    44,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,   144,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,   264,   265,   266,
      -1,   268,   269,   270,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
     178,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
     188,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,   152,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,   169,    -1,    -1,   262,   263,   264,   265,   266,    -1,
     268,   269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    44,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    85,    -1,    -1,
     268,   269,   270,    91,    92,    93,   206,    95,    96,    97,
      -1,    99,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,   119,    -1,   121,    41,   235,   236,    44,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   152,    -1,    -1,    -1,   268,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
     280,   169,    -1,    -1,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,
     248,   249,   169,   251,    -1,   206,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,   269,   270,    -1,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      -1,   206,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,   226,
     227,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270,
     235,   236,    -1,   238,    -1,    -1,   243,   278,    -1,    -1,
     245,   246,   247,   248,   249,   204,   251,   206,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,   206,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     269,   270,   235,   236,    -1,   238,   275,    -1,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,   269,   270,   206,    -1,
      -1,    -1,    -1,   276,    -1,    -1,    -1,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,    -1,    -1,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,   206,    -1,   245,   246,   247,
     248,   249,   250,   251,   252,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,   264,   265,   266,   267,
     268,   269,   270,    -1,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,   269,   270
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
{ yyval = yyvsp[0]; }
    break;
case 102:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 103:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_unique, 0, NULL); }
    break;
case 121:
{ yyval = NULL; }
    break;
case 122:
{ yyval = make_list (yyvsp[0]); }
    break;
case 124:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 125:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 126:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 127:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 128:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 129:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 130:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 131:
{ yyval = (dsql_nod*) 0;}
    break;
case 132:
{ yyval = yyvsp[-1]; }
    break;
case 133:
{ yyval = NULL; }
    break;
case 136:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 137:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 138:
{ yyval = NULL; }
    break;
case 139:
{ yyval = NULL; }
    break;
case 140:
{ yyval = yyvsp[0]; }
    break;
case 141:
{ yyval = NULL; }
    break;
case 142:
{ yyval = NULL; }
    break;
case 145:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 146:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 149:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 150:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 151:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
    break;
case 152:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
    break;
case 153:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 156:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
    break;
case 157:
{yyval = NULL;}
    break;
case 160:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 161:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 162:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 163:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 164:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 165:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 166:
{yyval = NULL;}
    break;
case 169:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 171:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 172:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 173:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 174:
{ lex.g_file  = make_file();}
    break;
case 179:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 180:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;
case 187:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 188:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 189:
{ yyval = yyvsp[0]; }
    break;
case 190:
{ yyval = yyvsp[0]; }
    break;
case 191:
{ yyval = NULL; }
    break;
case 193:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 196:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 197:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 198:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 199:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 202:
{ yyval = NULL; }
    break;
case 203:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 204:
{ yyval = yyvsp[0]; }
    break;
case 205:
{ yyval = NULL; }
    break;
case 206:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 207:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 208:
{ yyval = yyvsp[-1]; }
    break;
case 209:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 210:
{ yyval = yyvsp[0]; }
    break;
case 211:
{ yyval = NULL; }
    break;
case 218:
{ yyval = NULL; }
    break;
case 221:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 222:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 223:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 224:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 226:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 227:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 228:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 229:
{ yyval = yyvsp[0]; }
    break;
case 230:
{ yyval = NULL ;}
    break;
case 235:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 236:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 237:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 238:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 239:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 240:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 241:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 242:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 243:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 245:
{ yyval = NULL;}
    break;
case 246:
{ yyval = yyvsp[0];}
    break;
case 247:
{ yyval = yyvsp[0];}
    break;
case 248:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 249:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 250:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 252:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 253:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 254:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 255:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 256:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 257:
{ yyval = NULL; }
    break;
case 258:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 259:
{ yyval = NULL; }
    break;
case 261:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 262:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 264:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 265:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 266:
{ yyval = yyvsp[0]; }
    break;
case 267:
{ yyval = yyvsp[0]; }
    break;
case 268:
{ yyval = (dsql_nod*) NULL; }
    break;
case 269:
{ yyval = make_list (yyvsp[0]); }
    break;
case 270:
{ yyval = NULL; }
    break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 273:
{ yyval = yyvsp[-1]; }
    break;
case 276:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 277:
{ yyval = NULL; }
    break;
case 278:
{ yyval = NULL; }
    break;
case 279:
{ yyval = yyvsp[0]; }
    break;
case 280:
{ yyval = yyvsp[0]; }
    break;
case 281:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 284:
{ yyval = yyvsp[-1]; }
    break;
case 285:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 286:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 287:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 289:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 305:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 306:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 309:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 310:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 311:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 312:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 313:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 314:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 315:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 316:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 317:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 318:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 319:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 320:
{ yyval = NULL; }
    break;
case 321:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 322:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 323:
{ yyval = make_list (yyvsp[0]); }
    break;
case 324:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 325:
{ yyval = NULL; }
    break;
case 326:
{ yyval = make_list (yyvsp[0]); }
    break;
case 327:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 328:
{ yyval = NULL; }
    break;
case 331:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 332:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 333:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 334:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 335:
{ yyval = NULL; }
    break;
case 336:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 337:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 338:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 339:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 340:
{ yyval = NULL; }
    break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 343:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 346:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 347:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 348:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 349:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 353:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 354:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 355:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 356:
{ yyval = NULL; }
    break;
case 357:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 358:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 359:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 360:
{ yyval = NULL; }
    break;
case 362:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 363:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 364:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 365:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 366:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 367:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 368:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 369:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 370:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 371:
{ yyval = make_list (yyvsp[0]); }
    break;
case 373:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 376:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 377:
{ yyval = yyvsp[-1]; }
    break;
case 378:
{ lex.beginning = lex_position(); }
    break;
case 379:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 380:
{ lex.beginning = lex.last_token; }
    break;
case 381:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 382:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 383:
{ yyval = 0; }
    break;
case 384:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 385:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 388:
{ yyval = NULL; }
    break;
case 389:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 390:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 403:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 404:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 406:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 407:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 408:
{ yyval = NULL; }
    break;
case 409:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 410:
{ yyval = yyvsp[0]; }
    break;
case 411:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 412:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 413:
{ yyval = yyvsp[0]; }
    break;
case 414:
{ yyval = yyvsp[0]; }
    break;
case 415:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 416:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 417:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 418:
{ yyval = yyvsp[0]; }
    break;
case 419:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 422:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 423:
{ yyval = yyvsp[0]; }
    break;
case 424:
{ yyval = yyvsp[0]; }
    break;
case 425:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 426:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 427:
{ yyval = yyvsp[0]; }
    break;
case 428:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 430:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 431:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 432:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 433:
{ yyval = yyvsp[0]; }
    break;
case 434:
{ yyval = yyvsp[0]; }
    break;
case 435:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 436:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 437:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 438:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 466:
{ yyval = NULL; }
    break;
case 467:
{ yyval = NULL; }
    break;
case 468:
{ yyval = NULL; }
    break;
case 469:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 470:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 471:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 472:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 473:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 474:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 475:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 476:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 478:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 479:
{ yyval = yyvsp[0]; }
    break;
case 480:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 481:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 482:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 483:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 484:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 486:
{ yyval = NULL; }
    break;
case 488:
{ yyval = NULL; }
    break;
case 489:
{ yyval = yyvsp[0]; }
    break;
case 490:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 491:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 492:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 494:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 501:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 506:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 507:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 509:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 510:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 511:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 517:
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
case 518:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 520:
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
case 521:
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
case 522:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 529:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 530:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 531:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 532:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 533:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 534:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 550:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 551:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 552:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 553:
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
case 554:
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
case 557:
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
case 558:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 561:
{ yyval = yyvsp[-1]; }
    break;
case 562:
{ yyval = 0; }
    break;
case 566:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 567:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 568:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 572:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 573:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 574:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 575:
{ yyval = 0; }
    break;
case 576:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 579:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 580:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 583:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 584:
{ yyval = NULL; }
    break;
case 586:
{ yyval = NULL; }
    break;
case 587:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 589:
{ yyval = NULL; }
    break;
case 591:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 596:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 597:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 598:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 599:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 600:
{ yyval = yyvsp[0];}
    break;
case 602:
{ yyval = yyvsp[0];}
    break;
case 603:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 604:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 605:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 606:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 607:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 608:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 609:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 610:
{ yyval = 0; }
    break;
case 611:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 612:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 613:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 614:
{ yyval = (dsql_nod*) 0; }
    break;
case 615:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 616:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 618:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 619:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 620:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 621:
{ yyval = 0; }
    break;
case 623:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 624:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 625:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 626:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 627:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 628:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 629:
{ yyval = make_list (yyvsp[0]); }
    break;
case 630:
{ yyval = 0; }
    break;
case 632:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 633:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 634:
{ yyval = 0; }
    break;
case 635:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 636:
{ yyval = 0; }
    break;
case 637:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 638:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 639:
{ yyval = yyvsp[-1]; }
    break;
case 640:
{ yyval = 0; }
    break;
case 641:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 642:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 643:
{ yyval = NULL; }
    break;
case 644:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 645:
{ yyval = 0; }
    break;
case 646:
{ yyval = yyvsp[0]; }
    break;
case 647:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 648:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 649:
{ yyval = 0; }
    break;
case 650:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 651:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 652:
{ lex.limit_clause = true; }
    break;
case 653:
{ lex.limit_clause = false; }
    break;
case 654:
{ lex.first_detection = true; }
    break;
case 655:
{ lex.first_detection = false; }
    break;
case 656:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 657:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 658:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 659:
{ yyval = 0; }
    break;
case 660:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 661:
{ yyval = yyvsp[-2]; }
    break;
case 662:
{ yyval = yyvsp[-1]; }
    break;
case 663:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 664:
{ yyval = yyvsp[-1]; }
    break;
case 665:
{ yyval = yyvsp[0]; }
    break;
case 666:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 667:
{ yyval = 0; }
    break;
case 668:
{ yyval = make_list (yyvsp[0]); }
    break;
case 669:
{ yyval = 0; }
    break;
case 671:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 673:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 676:
{ yyval = make_list (yyvsp[0]); }
    break;
case 678:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 682:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 684:
{ yyval = NULL; }
    break;
case 685:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 686:
{ yyval = NULL; }
    break;
case 688:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 689:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 690:
{ yyval = yyvsp[-1]; }
    break;
case 691:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 692:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 693:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 694:
{ yyval = NULL; }
    break;
case 696:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 697:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 698:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 699:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 700:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 701:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 702:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 703:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 704:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 705:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 706:
{ yyval = make_list (yyvsp[0]); }
    break;
case 707:
{ yyval = 0; }
    break;
case 709:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 711:
{ yyval = yyvsp[0]; }
    break;
case 712:
{ yyval = 0; }
    break;
case 713:
{ yyval = yyvsp[0]; }
    break;
case 714:
{ yyval = 0; }
    break;
case 715:
{ yyval = yyvsp[0]; }
    break;
case 716:
{ yyval = 0; }
    break;
case 717:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 718:
{ yyval = 0; }
    break;
case 719:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 720:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 721:
{ yyval = 0; }
    break;
case 722:
{ yyval = 0; }
    break;
case 724:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 725:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 728:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 729:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 730:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 731:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 733:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 734:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 735:
{ yyval = 0; }
    break;
case 736:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 737:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 740:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 741:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 745:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 746:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 748:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 749:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 750:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 751:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 752:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 753:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 757:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 758:
{ yyval = yyvsp[0]; }
    break;
case 760:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 763:
{ yyval = NULL; }
    break;
case 764:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 768:
{ yyval = NULL; }
    break;
case 769:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 771:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 774:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 775:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 779:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 781:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 782:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 785:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = yyvsp[-1]; }
    break;
case 791:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 802:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 807:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 811:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 812:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 816:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 817:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 818:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 819:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 820:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 821:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 822:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 825:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 828:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 830:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 832:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 834:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 836:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 838:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 839:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 840:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 842:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 843:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 844:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 847:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 848:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 849:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 850:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 851:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 853:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 854:
{ yyval = yyvsp[-1]; }
    break;
case 855:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 856:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 866:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 867:
{ yyval = yyvsp[0]; }
    break;
case 868:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 869:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 870:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 871:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 872:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 873:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 874:
{ yyval = yyvsp[-1]; }
    break;
case 875:
{ yyval = yyvsp[-1]; }
    break;
case 879:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 880:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 881:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 884:
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
case 885:
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
case 886:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 887:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 889:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 891:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 892:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 893:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 894:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 895:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 898:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 899:
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
case 900:
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
case 901:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 902:
{ yyval = make_parameter (); }
    break;
case 903:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 904:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 905:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 906:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 907:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 908:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 909:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 910:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 911:
{ yyval = yyvsp[0]; }
    break;
case 912:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 914:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 915:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 916:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 917:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 918:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 920:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 921:
{ yyval = yyvsp[0];}
    break;
case 926:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 927:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 928:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 929:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 930:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 933:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 934:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 935:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 936:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 939:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 941:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 942:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 943:
{ yyval = yyvsp[0]; }
    break;
case 944:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 945:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 946:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 947:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 950:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 951:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 952:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 955:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 956:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 957:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 958:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 959:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 960:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 961:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 962:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 966:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 967:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 968:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 976:
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

