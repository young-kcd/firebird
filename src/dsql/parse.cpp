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



#define	YYFINAL		1913
#define	YYFLAG		-32768
#define	YYNTBASE	279

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 516 ? yytranslate[x] : 708)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     272,   273,   268,   266,   271,   267,   277,   269,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   274,   270,
     264,   263,   265,   278,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   275,     2,   276,     2,     2,     2,     2,     2,     2,
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
     256,   257,   258,   259,   260,   261,   262
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
     324,   334,   336,   338,   341,   345,   353,   356,   359,   362,
     365,   368,   371,   374,   377,   380,   383,   386,   389,   392,
     397,   400,   403,   405,   406,   408,   410,   417,   424,   426,
     428,   429,   430,   432,   433,   438,   439,   441,   443,   446,
     455,   457,   458,   462,   463,   464,   466,   468,   471,   473,
     475,   477,   480,   487,   489,   491,   495,   496,   498,   500,
     501,   503,   505,   508,   512,   517,   520,   523,   527,   528,
     530,   532,   535,   537,   542,   546,   550,   552,   553,   555,
     557,   560,   564,   569,   570,   572,   575,   576,   578,   580,
     586,   592,   596,   599,   600,   602,   606,   608,   610,   617,
     621,   624,   631,   634,   636,   639,   642,   645,   646,   648,
     650,   653,   654,   657,   658,   660,   662,   664,   666,   668,
     670,   671,   673,   675,   678,   681,   684,   690,   692,   695,
     699,   702,   705,   706,   708,   710,   712,   714,   718,   723,
     732,   737,   738,   745,   747,   749,   752,   755,   756,   760,
     764,   766,   769,   772,   775,   784,   793,   802,   811,   815,
     816,   821,   822,   824,   828,   833,   835,   839,   842,   846,
     850,   852,   854,   855,   857,   860,   865,   867,   869,   873,
     875,   876,   879,   881,   888,   890,   892,   896,   898,   901,
     902,   904,   907,   910,   913,   915,   917,   919,   921,   923,
     925,   928,   930,   932,   935,   937,   939,   941,   944,   947,
     949,   952,   956,   961,   964,   971,   976,   985,   995,  1002,
    1011,  1018,  1023,  1024,  1029,  1032,  1034,  1038,  1039,  1042,
    1047,  1048,  1050,  1052,  1056,  1060,  1068,  1071,  1072,  1075,
    1078,  1082,  1086,  1087,  1089,  1092,  1097,  1099,  1103,  1106,
    1109,  1112,  1114,  1116,  1118,  1120,  1123,  1126,  1132,  1133,
    1138,  1146,  1150,  1151,  1153,  1157,  1161,  1169,  1177,  1179,
    1181,  1186,  1191,  1199,  1202,  1204,  1208,  1210,  1212,  1219,
    1223,  1224,  1225,  1226,  1227,  1231,  1232,  1242,  1252,  1254,
    1256,  1257,  1260,  1262,  1264,  1266,  1268,  1270,  1274,  1278,
    1282,  1286,  1290,  1294,  1300,  1306,  1312,  1318,  1324,  1330,
    1333,  1334,  1339,  1342,  1346,  1350,  1353,  1356,  1360,  1364,
    1367,  1371,  1378,  1380,  1383,  1388,  1392,  1395,  1398,  1401,
    1404,  1408,  1410,  1414,  1418,  1422,  1425,  1428,  1433,  1438,
    1444,  1446,  1448,  1450,  1452,  1454,  1456,  1458,  1460,  1462,
    1464,  1466,  1468,  1470,  1472,  1474,  1476,  1478,  1480,  1482,
    1484,  1486,  1488,  1490,  1492,  1494,  1496,  1498,  1500,  1502,
    1505,  1508,  1511,  1513,  1515,  1517,  1518,  1521,  1524,  1525,
    1527,  1530,  1533,  1538,  1542,  1545,  1548,  1556,  1558,  1559,
    1561,  1562,  1565,  1568,  1571,  1574,  1577,  1580,  1583,  1586,
    1589,  1593,  1596,  1599,  1602,  1604,  1606,  1608,  1610,  1615,
    1621,  1623,  1627,  1629,  1633,  1635,  1638,  1640,  1642,  1644,
    1646,  1648,  1650,  1652,  1654,  1656,  1658,  1660,  1665,  1670,
    1677,  1683,  1687,  1688,  1691,  1694,  1695,  1699,  1700,  1705,
    1707,  1713,  1718,  1720,  1725,  1727,  1730,  1733,  1735,  1737,
    1739,  1742,  1745,  1748,  1751,  1752,  1756,  1762,  1764,  1766,
    1769,  1773,  1775,  1778,  1782,  1783,  1785,  1787,  1789,  1795,
    1801,  1808,  1810,  1812,  1814,  1817,  1822,  1824,  1825,  1831,
    1833,  1834,  1838,  1841,  1843,  1844,  1847,  1848,  1850,  1851,
    1855,  1857,  1858,  1860,  1863,  1865,  1867,  1869,  1871,  1874,
    1877,  1879,  1882,  1886,  1888,  1890,  1894,  1898,  1900,  1903,
    1907,  1909,  1912,  1913,  1916,  1918,  1920,  1921,  1923,  1925,
    1927,  1931,  1934,  1938,  1939,  1941,  1945,  1950,  1956,  1958,
    1963,  1968,  1972,  1973,  1975,  1979,  1983,  1985,  1987,  1988,
    1990,  1992,  1997,  1998,  2001,  2006,  2007,  2011,  2012,  2015,
    2016,  2019,  2020,  2030,  2042,  2043,  2044,  2045,  2046,  2050,
    2053,  2055,  2056,  2060,  2066,  2070,  2073,  2079,  2082,  2084,
    2086,  2088,  2090,  2092,  2096,  2098,  2102,  2104,  2105,  2108,
    2110,  2114,  2116,  2118,  2120,  2127,  2129,  2130,  2134,  2135,
    2137,  2141,  2148,  2152,  2157,  2160,  2164,  2165,  2167,  2170,
    2172,  2174,  2176,  2179,  2181,  2184,  2186,  2189,  2190,  2194,
    2195,  2197,  2201,  2203,  2206,  2207,  2210,  2211,  2214,  2215,
    2220,  2222,  2225,  2227,  2229,  2230,  2232,  2236,  2239,  2241,
    2243,  2246,  2248,  2253,  2257,  2259,  2263,  2268,  2269,  2278,
    2284,  2286,  2288,  2293,  2298,  2303,  2305,  2307,  2313,  2319,
    2321,  2325,  2329,  2337,  2345,  2351,  2355,  2356,  2358,  2360,
    2362,  2365,  2366,  2368,  2370,  2372,  2373,  2377,  2379,  2383,
    2385,  2386,  2390,  2392,  2396,  2398,  2402,  2406,  2408,  2410,
    2414,  2416,  2419,  2421,  2425,  2429,  2431,  2434,  2438,  2442,
    2446,  2450,  2452,  2456,  2459,  2461,  2463,  2465,  2467,  2469,
    2471,  2473,  2475,  2477,  2479,  2483,  2487,  2491,  2495,  2499,
    2503,  2507,  2511,  2518,  2525,  2532,  2539,  2546,  2553,  2560,
    2567,  2574,  2581,  2588,  2595,  2602,  2609,  2616,  2623,  2625,
    2627,  2633,  2640,  2644,  2649,  2655,  2662,  2666,  2671,  2675,
    2680,  2684,  2689,  2694,  2700,  2705,  2710,  2714,  2719,  2721,
    2723,  2725,  2727,  2729,  2731,  2733,  2737,  2741,  2753,  2765,
    2767,  2769,  2771,  2773,  2775,  2777,  2779,  2781,  2783,  2786,
    2789,  2793,  2797,  2801,  2805,  2809,  2813,  2817,  2821,  2823,
    2825,  2827,  2829,  2833,  2835,  2837,  2839,  2841,  2843,  2845,
    2850,  2852,  2856,  2858,  2861,  2863,  2865,  2867,  2869,  2871,
    2873,  2875,  2878,  2881,  2884,  2886,  2888,  2890,  2892,  2894,
    2896,  2898,  2900,  2902,  2904,  2907,  2909,  2912,  2914,  2916,
    2918,  2920,  2922,  2925,  2927,  2929,  2931,  2933,  2935,  2940,
    2946,  2952,  2958,  2964,  2970,  2976,  2982,  2988,  2994,  3000,
    3007,  3009,  3016,  3018,  3023,  3031,  3034,  3035,  3040,  3044,
    3051,  3053,  3055,  3062,  3071,  3078,  3080,  3082,  3087,  3094,
    3099,  3105,  3109,  3115,  3120,  3126,  3128,  3130,  3132,  3134,
    3136,  3138,  3140,  3142,  3144,  3146,  3148,  3150,  3151,  3153,
    3154,  3156,  3158,  3160,  3162,  3164,  3166,  3168,  3170,  3172,
    3174,  3176,  3178,  3180,  3182,  3184,  3186,  3188,  3190,  3192,
    3194,  3196,  3198,  3200,  3202,  3204,  3206,  3208,  3210,  3212,
    3214,  3216,  3218,  3220,  3222,  3224,  3226,  3228,  3230,  3232,
    3234,  3236,  3238,  3240,  3242,  3244
};
static const short yyrhs[] =
{
     280,     0,   280,   270,     0,   465,     0,   604,     0,   518,
       0,   313,     0,   300,     0,   595,     0,   486,     0,   281,
       0,   594,     0,   438,     0,   439,     0,   315,     0,   317,
       0,   290,     0,   519,     0,   512,     0,   541,     0,   510,
       0,   599,     0,    40,   651,     0,    70,   283,   122,   282,
     174,   294,   287,     0,    70,   285,   122,   141,   289,   174,
     294,   287,     0,    70,   283,   122,   282,   174,   292,     0,
      70,   285,   122,   141,   289,   174,   292,     0,    70,   296,
     174,   298,   288,     0,   578,     0,   172,   578,     0,     6,
       0,     6,   140,     0,   284,     0,   286,     0,   284,   271,
     286,     0,    56,     0,   155,     0,    80,     0,    44,     0,
     180,   609,     0,   145,   609,     0,   192,    70,   124,     0,
       0,   192,   202,   124,     0,     0,   697,     0,   150,   291,
     283,   122,   282,    63,   294,     0,   150,   291,   285,   122,
     141,   289,    63,   294,     0,   150,   283,   122,   282,    63,
     294,     0,   150,   285,   122,   141,   289,    63,   294,     0,
     150,   283,   122,   282,    63,   292,     0,   150,   285,   122,
     141,   289,    63,   292,     0,   150,   296,    63,   298,     0,
      70,   124,    61,     0,   293,     0,   292,   271,   293,     0,
     292,   271,   295,     0,   294,   271,   293,     0,   141,   697,
       0,   176,   701,     0,   187,   704,     0,   206,   698,     0,
     295,     0,   294,   271,   295,     0,   702,     0,   181,   702,
       0,    71,   702,     0,   297,     0,   296,   271,   297,     0,
     698,     0,   299,     0,   298,   271,   299,     0,   702,     0,
     181,   702,     0,    42,   301,     0,    60,   311,     0,    59,
      65,   302,     0,   682,   304,   149,   308,    52,   650,   111,
     650,     0,   493,     0,    18,     0,    34,   272,   654,   273,
     499,     0,     0,   305,     0,   272,   305,   273,     0,   306,
       0,   305,   271,   306,     0,   367,   303,   307,     0,     0,
      19,   230,     0,    19,   262,     0,    94,     0,   309,     0,
     272,   309,   273,     0,   367,   303,   310,     0,   133,   654,
       0,     0,    19,    97,     0,    19,   230,     0,   204,     0,
      19,   230,   204,     0,   691,    78,   312,   128,   312,    52,
     650,   111,   650,     0,   683,     0,   651,     0,    33,   314,
       0,    55,   690,   650,     0,   319,   546,    79,   694,   122,
     578,   320,     0,   141,   386,     0,   172,   354,     0,   176,
     457,     0,   187,   443,     0,    68,   336,     0,    37,   338,
       0,    48,   327,     0,   157,   321,     0,   206,   337,     0,
     229,   316,     0,   141,   387,     0,   172,   355,     0,   187,
     444,     0,    33,   125,     7,   318,     0,   141,   388,     0,
     176,   458,     0,   179,     0,     0,   611,     0,   610,     0,
     361,   272,   454,   639,   455,   273,     0,   654,   322,   323,
     650,   324,   325,     0,   105,     0,    14,     0,     0,     0,
      29,     0,     0,    98,   339,   657,   353,     0,     0,   326,
       0,   347,     0,   326,   347,     0,   364,   328,   488,   454,
     329,   455,   330,   363,     0,    10,     0,     0,    43,   454,
     369,     0,     0,     0,   331,     0,   332,     0,   331,   332,
       0,   333,     0,   334,     0,   335,     0,   118,    94,     0,
     454,    22,   272,   618,   273,   455,     0,   693,     0,   698,
       0,   340,   341,   344,     0,     0,   263,     0,   650,     0,
       0,   342,     0,   343,     0,   342,   343,     0,   132,   339,
     654,     0,    98,   339,   657,   353,     0,   181,   650,     0,
     134,   650,     0,   156,   112,   650,     0,     0,   345,     0,
     346,     0,   345,   346,     0,   347,     0,    43,    21,   156,
     684,     0,   255,    90,   650,     0,   348,   650,   349,     0,
      90,     0,     0,   350,     0,   351,     0,   350,   351,     0,
     167,   352,   657,     0,    98,   339,   657,   353,     0,     0,
      12,     0,    12,   130,     0,     0,   130,     0,   131,     0,
     578,   356,   272,   357,   273,     0,   578,   356,   272,   357,
     273,     0,    59,    90,   650,     0,    59,   650,     0,     0,
     358,     0,   357,   271,   358,     0,   359,     0,   374,     0,
     364,   362,   368,   455,   370,   363,     0,   364,   489,   360,
       0,   364,   360,     0,   361,   272,   454,   639,   455,   273,
       0,    27,    19,     0,    27,     0,   488,   454,     0,   616,
     452,     0,    23,   685,     0,     0,   616,     0,   616,     0,
     367,   488,     0,     0,    43,   369,     0,     0,   643,     0,
     647,     0,   648,     0,   649,     0,   681,     0,   640,     0,
       0,   371,     0,   372,     0,   371,   372,     0,   375,   373,
       0,   118,    94,     0,   145,   578,   609,   382,   380,     0,
     381,     0,   179,   380,     0,   139,    86,   380,     0,   375,
     376,     0,    30,   687,     0,     0,   377,     0,   378,     0,
     379,     0,   381,     0,   179,   610,   380,     0,   139,    86,
     610,   380,     0,    62,    86,   610,   145,   578,   609,   382,
     380,     0,   239,   546,    79,   694,     0,     0,   454,    22,
     272,   618,   273,   455,     0,   383,     0,   384,     0,   384,
     383,     0,   383,   384,     0,     0,   122,   180,   385,     0,
     122,    44,   385,     0,   203,     0,   156,    43,     0,   156,
      94,     0,   117,   201,     0,   697,   390,   391,    10,   452,
     397,   406,   455,     0,   697,   390,   391,    10,   452,   397,
     406,   455,     0,   697,   390,   391,    10,   452,   397,   406,
     455,     0,   697,   390,   391,    10,   452,   397,   406,   455,
       0,   272,   392,   273,     0,     0,   149,   272,   394,   273,
       0,     0,   393,     0,   392,   271,   393,     0,   365,   489,
     396,   455,     0,   395,     0,   394,   271,   395,     0,   365,
     489,     0,    43,   452,   369,     0,   263,   452,   369,     0,
     452,     0,   398,     0,     0,   399,     0,   398,   399,     0,
      42,   402,   400,   270,     0,   401,     0,   404,     0,   364,
     489,   403,     0,   184,     0,     0,   263,   369,     0,   368,
       0,   688,    36,    61,   272,   554,   273,     0,   409,     0,
     406,     0,    16,   407,    51,     0,   408,     0,   408,   429,
       0,     0,   405,     0,   408,   405,     0,   603,   270,     0,
     595,   270,     0,   410,     0,   411,     0,   412,     0,   413,
       0,   414,     0,   417,     0,   594,   270,     0,   418,     0,
     420,     0,   599,   270,     0,   425,     0,   415,     0,   416,
       0,   170,   270,     0,    58,   270,     0,   427,     0,   433,
     270,     0,    55,   690,   270,     0,    55,   690,   639,   270,
       0,    55,   270,     0,    56,   141,   697,   422,   423,   270,
       0,    56,   246,   639,   270,     0,   426,    61,   541,    82,
     424,   428,    47,   405,     0,   426,    61,    56,   246,   639,
      82,   424,    47,   405,     0,    56,   246,   639,    82,   424,
     270,     0,    74,   272,   618,   273,   173,   405,    50,   405,
       0,    74,   272,   618,   273,   173,   405,     0,   137,   639,
     419,   270,     0,     0,   541,    82,   424,   270,     0,   274,
     703,     0,   642,     0,   272,   642,   273,     0,     0,   148,
     424,     0,   148,   272,   424,   273,     0,     0,   421,     0,
     615,     0,   424,   271,   615,     0,   424,   271,   421,     0,
     426,   191,   272,   618,   273,    47,   405,     0,   696,   274,
       0,     0,   227,   270,     0,   247,   270,     0,   247,   696,
     270,     0,    10,    36,   688,     0,     0,   430,     0,   429,
     430,     0,   189,   431,    47,   405,     0,   432,     0,   431,
     271,   432,     0,   165,   651,     0,    66,   692,     0,    55,
     690,     0,     9,     0,   434,     0,   436,     0,   435,     0,
     256,   688,     0,   257,   688,     0,   258,   437,   688,    82,
     424,     0,     0,    56,   141,   697,   422,     0,    56,   260,
     440,   391,    10,   397,   406,     0,   272,   441,   273,     0,
       0,   442,     0,   441,   271,   442,     0,   395,   263,   646,
       0,   704,   609,    10,   452,   445,   456,   453,     0,   704,
     609,    10,   452,   445,   456,   453,     0,   446,     0,   447,
       0,   446,   178,   680,   447,     0,   446,   178,     6,   447,
       0,   155,   562,   563,   448,   584,   580,   583,     0,    63,
     449,     0,   450,     0,   449,   271,   450,     0,   451,     0,
     577,     0,   450,   579,    85,   450,   122,   618,     0,   272,
     451,   273,     0,     0,     0,     0,     0,   192,    22,   124,
       0,     0,   701,    61,   578,   459,   460,   463,   454,   464,
     455,     0,   701,    61,   578,   459,   460,   463,   454,   464,
     455,     0,     3,     0,    76,     0,     0,   461,   462,     0,
      15,     0,     5,     0,    80,     0,   180,     0,    44,     0,
      80,   125,   180,     0,    80,   125,    44,     0,   180,   125,
      80,     0,   180,   125,    44,     0,    44,   125,    80,     0,
      44,   125,   180,     0,    80,   125,   180,   125,    44,     0,
      80,   125,    44,   125,   180,     0,   180,   125,    80,   125,
      44,     0,   180,   125,    44,   125,    80,     0,    44,   125,
      80,   125,   180,     0,    44,   125,   180,   125,    80,     0,
     136,   652,     0,     0,    10,   454,   397,   406,     0,     7,
     466,     0,    55,   690,   650,     0,   172,   578,   471,     0,
     176,   483,     0,   141,   389,     0,    37,   480,   481,     0,
      48,   473,   469,     0,    79,   479,     0,    43,   454,   369,
       0,    22,   454,   272,   618,   273,   455,     0,   470,     0,
     469,   470,     0,   156,   454,   467,   455,     0,     4,    30,
     468,     0,     4,   335,     0,    49,    43,     0,    49,    30,
       0,   174,   616,     0,   208,   367,   489,     0,   472,     0,
     471,   271,   472,     0,    49,   616,   478,     0,    49,    30,
     687,     0,     4,   359,     0,     4,   374,     0,   475,   616,
     136,   654,     0,   475,   473,   174,   616,     0,   475,   477,
     208,   476,   455,     0,   474,     0,   706,     0,   207,     0,
     208,     0,   209,     0,   210,     0,   211,     0,   212,     0,
     213,     0,   214,     0,   215,     0,   216,     0,   217,     0,
     218,     0,   219,     0,   220,     0,   221,     0,   222,     0,
     225,     0,   226,     0,   233,     0,   234,     0,   242,     0,
     244,     0,   256,     0,   257,     0,   258,     0,   259,     0,
       7,     0,     7,   207,     0,   489,   454,     0,   616,   452,
       0,   616,     0,   205,     0,   203,     0,     0,   694,     3,
       0,   694,    76,     0,     0,   482,     0,   481,   482,     0,
       4,   326,     0,     4,   255,    90,   650,     0,    49,   255,
      90,     0,    16,   254,     0,    51,   254,     0,   701,   459,
     484,   463,   454,   485,   455,     0,   460,     0,     0,   464,
       0,     0,    49,   487,     0,    55,   690,     0,    79,   694,
       0,   141,   697,     0,   172,   700,     0,   176,   701,     0,
     187,   704,     0,    60,   691,     0,    48,   689,     0,    59,
      65,   682,     0,   157,   654,     0,   206,   698,     0,    68,
     693,     0,   489,     0,   490,     0,   493,     0,   496,     0,
     494,   275,   491,   276,     0,   501,   275,   491,   276,   499,
       0,   492,     0,   491,   271,   492,     0,   656,     0,   656,
     274,   656,     0,   494,     0,   501,   499,     0,   500,     0,
     505,     0,   508,     0,   235,     0,   495,     0,   161,     0,
      38,     0,   218,     0,   219,     0,    81,     0,    92,     0,
      18,   498,   497,   499,     0,    18,   272,   655,   273,     0,
      18,   272,   655,   271,   651,   273,     0,    18,   272,   271,
     651,   273,     0,   154,   160,   655,     0,     0,   169,   651,
       0,   169,   683,     0,     0,    21,   156,   684,     0,     0,
     504,   272,   654,   273,     0,   504,     0,   504,   185,   272,
     654,   273,     0,   503,   272,   654,   273,     0,   503,     0,
     502,   272,   654,   273,     0,   183,     0,    21,   185,     0,
      87,   185,     0,    21,     0,    87,     0,   115,     0,   113,
      21,     0,   113,    87,     0,    95,   506,     0,   507,   506,
       0,     0,   272,   656,   273,     0,   272,   656,   271,   656,
     273,     0,    41,     0,    88,     0,    91,   509,     0,    93,
      91,   509,     0,   144,     0,    89,   138,     0,   272,   652,
     273,     0,     0,   523,     0,   511,     0,   540,     0,   156,
      68,   693,   174,   656,     0,   156,    68,   693,   174,   223,
       0,   156,    68,   693,   174,   267,   223,     0,   513,     0,
     514,     0,   516,     0,   244,   705,     0,   245,   244,   705,
     515,     0,   123,     0,     0,   153,   520,   174,   517,   705,
       0,   244,     0,     0,    25,   520,   521,     0,   153,   520,
       0,   193,     0,     0,   147,   522,     0,     0,   162,     0,
       0,   156,   175,   524,     0,   525,     0,     0,   526,     0,
     525,   526,     0,   527,     0,   528,     0,   529,     0,   533,
       0,   143,   123,     0,   143,   194,     0,   188,     0,   117,
     188,     0,    84,   102,   530,     0,   530,     0,   531,     0,
     143,   177,   532,     0,   143,    26,   532,     0,   162,     0,
     162,   172,     0,   162,   172,   166,     0,   186,     0,   117,
     186,     0,     0,   146,   536,     0,   158,     0,   142,     0,
       0,   143,     0,   194,     0,   537,     0,   536,   271,   537,
       0,   539,   538,     0,    61,   534,   535,     0,     0,   578,
       0,   539,   271,   578,     0,   156,   168,    79,   694,     0,
     542,   543,   549,   550,   552,     0,   553,     0,   542,   178,
     680,   553,     0,   542,   178,     6,   553,     0,   126,    19,
     544,     0,     0,   545,     0,   544,   271,   545,     0,   639,
     546,   548,     0,    11,     0,    45,     0,     0,   231,     0,
     241,     0,   240,   557,   547,   558,     0,     0,   259,   639,
       0,   259,   639,   174,   639,     0,     0,    61,   180,   551,
       0,     0,   121,   611,     0,     0,   192,   243,     0,     0,
     155,   559,   562,   563,   567,   584,   580,   583,   585,     0,
     155,   559,   562,   563,   567,   584,   580,   583,   585,   543,
     549,     0,     0,     0,     0,     0,   560,   561,   556,     0,
     560,   556,     0,   561,     0,     0,   231,   657,   555,     0,
     231,   272,   639,   273,   555,     0,   231,   646,   555,     0,
     232,   657,     0,   232,   272,   556,   639,   273,     0,   232,
     646,     0,    46,     0,   679,     0,   564,     0,   268,     0,
     565,     0,   564,   271,   565,     0,   639,     0,   639,   566,
     695,     0,    10,     0,     0,    63,   568,     0,   569,     0,
     568,   271,   569,     0,   574,     0,   575,     0,   570,     0,
     272,   541,   273,   566,   571,   572,     0,   699,     0,     0,
     272,   573,   273,     0,     0,   695,     0,   573,   271,   695,
       0,   569,   579,    85,   569,   122,   618,     0,   272,   574,
     273,     0,   697,   576,   566,   699,     0,   697,   576,     0,
     272,   642,   273,     0,     0,   578,     0,   700,   699,     0,
     700,     0,    77,     0,   100,     0,   100,   127,     0,   151,
       0,   151,   127,     0,    64,     0,    64,   127,     0,     0,
      71,    19,   581,     0,     0,   582,     0,   581,   271,   582,
       0,   639,     0,    73,   618,     0,     0,   190,   618,     0,
       0,   135,   586,     0,     0,   587,   272,   588,   273,     0,
      85,     0,   164,   108,     0,   108,     0,   164,     0,     0,
     589,     0,   589,   271,   588,     0,   590,   591,     0,   586,
       0,   700,     0,   700,   590,     0,   114,     0,    79,   272,
     592,   273,     0,   126,   694,   593,     0,   694,     0,   694,
     271,   592,     0,    79,   272,   592,   273,     0,     0,    80,
      82,   578,   612,   182,   272,   642,   273,     0,    80,    82,
     578,   612,   554,     0,   596,     0,   597,     0,    44,    63,
     577,   584,     0,    44,    63,   577,   598,     0,   190,    35,
     121,   688,     0,   600,     0,   601,     0,   180,   577,   156,
     602,   584,     0,   180,   577,   156,   602,   598,     0,   603,
       0,   602,   271,   603,     0,   617,   263,   639,     0,   143,
      18,   616,    63,   578,   605,   607,     0,    80,    18,   616,
      82,   578,   605,   607,     0,    60,    63,   606,   174,   606,
       0,    60,   174,   606,     0,     0,   498,     0,   646,     0,
     651,     0,   107,   608,     0,     0,   655,     0,   646,     0,
     610,     0,     0,   272,   611,   273,     0,   616,     0,   611,
     271,   616,     0,   613,     0,     0,   272,   614,   273,     0,
     617,     0,   614,   271,   617,     0,   616,     0,   699,   277,
     686,     0,   699,   277,   268,     0,   686,     0,   616,     0,
     699,   277,   686,     0,   633,     0,   118,   633,     0,   620,
       0,   618,   125,   618,     0,   618,     8,   618,     0,   620,
       0,   118,   633,     0,   619,   125,   618,     0,   619,     8,
     618,     0,   634,   125,   618,     0,   634,     8,   618,     0,
     621,     0,   272,   619,   273,     0,   118,   620,     0,   622,
       0,   625,     0,   626,     0,   627,     0,   632,     0,   623,
       0,   630,     0,   628,     0,   629,     0,   631,     0,   639,
     263,   639,     0,   639,   264,   639,     0,   639,   265,   639,
       0,   639,    67,   639,     0,   639,   101,   639,     0,   639,
     119,   639,     0,   639,   120,   639,     0,   639,   116,   639,
       0,   639,   263,     6,   272,   637,   273,     0,   639,   264,
       6,   272,   637,   273,     0,   639,   265,     6,   272,   637,
     273,     0,   639,    67,     6,   272,   637,   273,     0,   639,
     101,     6,   272,   637,   273,     0,   639,   119,     6,   272,
     637,   273,     0,   639,   120,     6,   272,   637,   273,     0,
     639,   116,     6,   272,   637,   273,     0,   639,   263,   624,
     272,   637,   273,     0,   639,   264,   624,   272,   637,   273,
       0,   639,   265,   624,   272,   637,   273,     0,   639,    67,
     624,   272,   637,   273,     0,   639,   101,   624,   272,   637,
     273,     0,   639,   119,   624,   272,   637,   273,     0,   639,
     120,   624,   272,   637,   273,     0,   639,   116,   624,   272,
     637,   273,     0,   163,     0,     9,     0,   639,    17,   639,
       8,   639,     0,   639,   118,    17,   639,     8,   639,     0,
     639,   103,   639,     0,   639,   118,   103,   639,     0,   639,
     103,   639,    54,   639,     0,   639,   118,   103,   639,    54,
     639,     0,   639,    75,   635,     0,   639,   118,    75,   635,
       0,   639,    31,   639,     0,   639,   118,    31,   639,     0,
     639,   167,   639,     0,   639,   118,   167,   639,     0,   639,
     167,   192,   639,     0,   639,   118,   167,   192,   639,     0,
      57,   272,   554,   273,     0,   159,   272,   554,   273,     0,
     639,    83,    94,     0,   639,    83,   118,    94,     0,   248,
       0,   249,     0,   250,     0,   251,     0,   252,     0,   253,
       0,   636,     0,   272,   642,   273,     0,   272,   637,   273,
       0,   155,   559,   562,   639,   567,   584,   580,   583,   585,
     543,   549,     0,   155,   559,   562,   639,   567,   584,   580,
     583,   585,   543,   549,     0,   615,     0,   641,     0,   658,
       0,   645,     0,   646,     0,   421,     0,   667,     0,   668,
       0,   666,     0,   267,   639,     0,   266,   639,     0,   639,
     266,   639,     0,   639,    28,   639,     0,   639,    23,   685,
       0,   639,   267,   639,     0,   639,   268,   639,     0,   639,
     269,   639,     0,   272,   639,   273,     0,   272,   638,   273,
       0,   647,     0,   648,     0,   649,     0,    39,     0,   699,
     277,    39,     0,    97,     0,   640,     0,   681,     0,   220,
       0,   221,     0,   222,     0,   615,   275,   642,   276,     0,
     639,     0,   642,   271,   639,     0,   645,     0,   267,   644,
       0,   197,     0,   196,     0,   195,     0,   223,     0,   224,
       0,   644,     0,   650,     0,    38,   199,     0,   218,   199,
       0,   219,   199,     0,   278,     0,   181,     0,   225,     0,
     226,     0,   233,     0,   234,     0,    66,     0,   165,     0,
     242,     0,   199,     0,   200,   199,     0,   652,     0,   267,
     653,     0,   196,     0,   196,     0,   652,     0,   196,     0,
     657,     0,   267,   657,     0,   196,     0,   659,     0,   660,
       0,   661,     0,   663,     0,    32,   272,   268,   273,     0,
      32,   272,   679,   639,   273,     0,    32,   272,    46,   639,
     273,     0,   171,   272,   679,   639,   273,     0,   171,   272,
      46,   639,   273,     0,    13,   272,   679,   639,   273,     0,
      13,   272,    46,   639,   273,     0,   110,   272,   679,   639,
     273,     0,   110,   272,    46,   639,   273,     0,   106,   272,
     679,   639,   273,     0,   106,   272,    46,   639,   273,     0,
      69,   272,   693,   271,   639,   273,     0,   662,     0,   209,
     272,   678,    63,   639,   273,     0,   664,     0,    96,   272,
     639,   273,     0,   228,   272,   639,    63,   639,   665,   273,
       0,    61,   639,     0,     0,   682,   272,   642,   273,     0,
     682,   272,   273,     0,    20,   272,   639,    10,   366,   273,
       0,   669,     0,   670,     0,   237,   272,   639,   271,   639,
     273,     0,   261,   272,   618,   271,   639,   271,   639,   273,
       0,   238,   272,   639,   271,   642,   273,     0,   671,     0,
     673,     0,   236,   676,   672,    51,     0,   236,   676,   672,
      50,   677,    51,     0,   189,   675,   173,   677,     0,   672,
     189,   675,   173,   677,     0,   236,   674,    51,     0,   236,
     674,    50,   677,    51,     0,   189,   618,   173,   677,     0,
     674,   189,   618,   173,   677,     0,   639,     0,   639,     0,
     639,     0,   210,     0,   211,     0,   212,     0,   213,     0,
     214,     0,   215,     0,   216,     0,   217,     0,     6,     0,
       0,    46,     0,     0,    94,     0,   198,     0,   706,     0,
     706,     0,   706,     0,   706,     0,   706,     0,   706,     0,
     706,     0,   706,     0,   706,     0,   706,     0,   706,     0,
     706,     0,   706,     0,   706,     0,   706,     0,   706,     0,
     706,     0,   706,     0,   706,     0,   706,     0,   706,     0,
     706,     0,   706,     0,   198,     0,   707,     0,   227,     0,
     230,     0,   228,     0,   238,     0,   241,     0,   247,     0,
     243,     0,   237,     0,   240,     0,   246,     0,   239,     0,
     248,     0,   249,     0,   250,     0,   260,     0,   254,     0,
     255,     0,   261,     0,   262,     0
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
     827,   833,   835,   842,   846,   849,   852,   854,   856,   858,
     860,   862,   864,   866,   868,   873,   877,   879,   881,   891,
     895,   897,   908,   910,   914,   916,   917,   923,   929,   931,
     933,   937,   939,   943,   945,   949,   951,   954,   955,   962,
     980,   982,   986,   988,   992,   994,   997,   998,  1001,  1006,
    1007,  1010,  1014,  1023,  1031,  1040,  1045,  1046,  1049,  1055,
    1057,  1060,  1061,  1065,  1067,  1069,  1071,  1073,  1077,  1079,
    1082,  1083,  1087,  1092,  1094,  1143,  1200,  1204,  1205,  1208,
    1209,  1212,  1214,  1218,  1219,  1220,  1223,  1224,  1225,  1231,
    1236,  1241,  1243,  1245,  1249,  1250,  1254,  1255,  1262,  1266,
    1270,  1279,  1285,  1286,  1289,  1291,  1296,  1298,  1303,  1309,
    1315,  1319,  1325,  1327,  1331,  1336,  1337,  1338,  1339,  1340,
    1343,  1345,  1348,  1349,  1353,  1358,  1360,  1365,  1366,  1368,
    1376,  1380,  1382,  1385,  1386,  1387,  1388,  1391,  1395,  1399,
    1406,  1413,  1418,  1424,  1427,  1429,  1431,  1433,  1437,  1440,
    1444,  1447,  1450,  1453,  1462,  1473,  1483,  1493,  1503,  1505,
    1509,  1511,  1515,  1516,  1520,  1526,  1527,  1531,  1536,  1538,
    1540,  1544,  1546,  1550,  1551,  1555,  1559,  1560,  1563,  1568,
    1570,  1574,  1576,  1580,  1585,  1586,  1589,  1593,  1595,  1597,
    1601,  1602,  1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,
    1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,  1623,  1625,
    1626,  1629,  1631,  1635,  1639,  1644,  1648,  1653,  1657,  1661,
    1663,  1667,  1671,  1677,  1682,  1687,  1689,  1691,  1695,  1697,
    1699,  1703,  1704,  1705,  1707,  1711,  1715,  1717,  1721,  1723,
    1725,  1730,  1732,  1736,  1737,  1741,  1746,  1747,  1751,  1753,
    1755,  1757,  1761,  1762,  1763,  1766,  1770,  1774,  1778,  1820,
    1827,  1835,  1837,  1841,  1842,  1846,  1852,  1859,  1879,  1883,
    1885,  1887,  1891,  1902,  1906,  1907,  1911,  1912,  1920,  1923,
    1929,  1933,  1939,  1943,  1949,  1953,  1961,  1972,  1983,  1985,
    1987,  1991,  1995,  1997,  2001,  2003,  2005,  2007,  2009,  2011,
    2013,  2015,  2017,  2019,  2021,  2023,  2025,  2027,  2029,  2033,
    2035,  2039,  2045,  2049,  2052,  2059,  2061,  2063,  2066,  2069,
    2074,  2078,  2084,  2085,  2089,  2098,  2102,  2104,  2106,  2108,
    2110,  2114,  2115,  2119,  2121,  2123,  2125,  2131,  2134,  2136,
    2140,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,  2165,  2166,
    2167,  2168,  2169,  2170,  2171,  2172,  2173,  2174,  2177,  2179,
    2183,  2185,  2190,  2196,  2198,  2200,  2204,  2206,  2213,  2219,
    2220,  2224,  2238,  2240,  2242,  2244,  2258,  2268,  2269,  2273,
    2274,  2280,  2284,  2286,  2288,  2290,  2292,  2294,  2296,  2298,
    2300,  2302,  2304,  2306,  2313,  2314,  2317,  2318,  2321,  2326,
    2333,  2334,  2338,  2347,  2353,  2354,  2357,  2358,  2359,  2360,
    2377,  2382,  2387,  2406,  2423,  2430,  2431,  2438,  2442,  2448,
    2454,  2462,  2466,  2472,  2476,  2480,  2486,  2490,  2497,  2503,
    2509,  2517,  2522,  2527,  2534,  2535,  2536,  2539,  2540,  2543,
    2544,  2545,  2552,  2556,  2567,  2573,  2627,  2689,  2690,  2697,
    2710,  2715,  2720,  2727,  2729,  2736,  2737,  2738,  2742,  2747,
    2752,  2763,  2764,  2765,  2768,  2772,  2776,  2778,  2782,  2786,
    2787,  2790,  2794,  2798,  2799,  2802,  2804,  2808,  2809,  2813,
    2817,  2818,  2822,  2823,  2827,  2828,  2829,  2830,  2833,  2835,
    2839,  2841,  2845,  2847,  2850,  2852,  2854,  2858,  2860,  2862,
    2866,  2868,  2870,  2874,  2878,  2880,  2882,  2886,  2888,  2892,
    2893,  2897,  2901,  2903,  2907,  2908,  2913,  2921,  2925,  2927,
    2929,  2933,  2935,  2939,  2940,  2944,  2948,  2950,  2952,  2956,
    2958,  2962,  2964,  2968,  2971,  2979,  2983,  2985,  2989,  2991,
    2995,  2997,  3004,  3016,  3030,  3034,  3038,  3042,  3046,  3048,
    3050,  3052,  3056,  3058,  3060,  3064,  3066,  3068,  3072,  3074,
    3078,  3080,  3084,  3085,  3089,  3090,  3094,  3095,  3100,  3104,
    3105,  3109,  3110,  3111,  3115,  3120,  3121,  3125,  3127,  3131,
    3132,  3136,  3138,  3142,  3145,  3150,  3152,  3156,  3157,  3162,
    3167,  3169,  3171,  3173,  3175,  3177,  3179,  3181,  3188,  3190,
    3194,  3195,  3201,  3204,  3206,  3210,  3212,  3219,  3221,  3225,
    3229,  3231,  3233,  3239,  3241,  3245,  3246,  3250,  3252,  3255,
    3256,  3260,  3262,  3264,  3268,  3269,  3273,  3275,  3282,  3285,
    3292,  3293,  3296,  3300,  3304,  3311,  3312,  3315,  3320,  3325,
    3326,  3330,  3337,  3339,  3343,  3345,  3347,  3350,  3351,  3354,
    3358,  3360,  3363,  3365,  3371,  3372,  3376,  3380,  3381,  3386,
    3387,  3391,  3395,  3396,  3401,  3402,  3405,  3410,  3415,  3418,
    3425,  3426,  3428,  3429,  3431,  3435,  3436,  3438,  3440,  3444,
    3446,  3450,  3451,  3453,  3457,  3458,  3459,  3460,  3461,  3462,
    3463,  3464,  3465,  3466,  3471,  3473,  3475,  3477,  3479,  3481,
    3483,  3485,  3492,  3494,  3496,  3498,  3500,  3502,  3504,  3506,
    3508,  3510,  3512,  3514,  3516,  3518,  3520,  3522,  3526,  3527,
    3533,  3535,  3540,  3542,  3544,  3546,  3551,  3553,  3557,  3559,
    3563,  3565,  3567,  3569,  3573,  3577,  3581,  3583,  3587,  3592,
    3597,  3604,  3609,  3614,  3623,  3624,  3628,  3632,  3646,  3664,
    3665,  3666,  3667,  3668,  3669,  3670,  3671,  3672,  3673,  3675,
    3677,  3684,  3686,  3688,  3695,  3702,  3709,  3711,  3713,  3714,
    3715,  3716,  3718,  3720,  3724,  3725,  3728,  3744,  3760,  3764,
    3768,  3769,  3773,  3774,  3778,  3780,  3782,  3784,  3786,  3790,
    3791,  3793,  3809,  3825,  3829,  3833,  3835,  3839,  3843,  3846,
    3849,  3852,  3855,  3860,  3862,  3867,  3868,  3872,  3879,  3886,
    3893,  3900,  3901,  3905,  3911,  3912,  3913,  3914,  3917,  3919,
    3921,  3924,  3931,  3940,  3947,  3956,  3958,  3960,  3962,  3968,
    3977,  3980,  3984,  3985,  3989,  3998,  4000,  4004,  4006,  4010,
    4016,  4017,  4020,  4024,  4027,  4031,  4032,  4035,  4037,  4041,
    4043,  4047,  4049,  4053,  4055,  4059,  4062,  4065,  4068,  4070,
    4072,  4074,  4076,  4078,  4080,  4082,  4086,  4087,  4090,  4091,
    4094,  4102,  4105,  4108,  4111,  4114,  4117,  4120,  4123,  4126,
    4129,  4132,  4135,  4138,  4141,  4144,  4147,  4150,  4153,  4156,
    4159,  4162,  4165,  4168,  4171,  4176,  4177,  4182,  4184,  4185,
    4186,  4187,  4188,  4189,  4190,  4191,  4192,  4193,  4194,  4195,
    4196,  4198,  4199,  4200,  4201,  4202
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
  "return_mechanism", "filter_decl_clause", "blob_filter_subtype", 
  "create", "create_clause", "recreate", "recreate_clause", "replace", 
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
  "timestamp_part", "all_noise", "distinct_noise", "null_value", 
  "symbol_UDF_name", "symbol_blob_subtype_name", 
  "symbol_character_set_name", "symbol_collation_name", 
  "symbol_column_name", "symbol_constraint_name", "symbol_cursor_name", 
  "symbol_domain_name", "symbol_exception_name", "symbol_filter_name", 
  "symbol_gdscode_name", "symbol_generator_name", "symbol_index_name", 
  "symbol_item_alias_name", "symbol_label_name", "symbol_procedure_name", 
  "symbol_role_name", "symbol_table_alias_name", "symbol_table_name", 
  "symbol_trigger_name", "symbol_user_name", "symbol_variable_name", 
  "symbol_view_name", "symbol_savepoint_name", "valid_symbol_name", 
  "non_reserved_word", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   279,   279,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   281,   281,   281,   281,   281,   282,   282,
     283,   283,   283,   284,   284,   285,   286,   286,   286,   286,
     286,   287,   287,   288,   288,   289,   290,   290,   290,   290,
     290,   290,   290,   291,   292,   292,   292,   292,   293,   293,
     293,   293,   294,   294,   295,   295,   295,   296,   296,   297,
     298,   298,   299,   299,   300,   301,   301,   302,   303,   303,
     303,   304,   304,   304,   305,   305,   306,   307,   307,   307,
     307,   308,   308,   309,   309,   310,   310,   310,   310,   310,
     311,   312,   312,   313,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   315,   316,   316,   316,   317,
     318,   318,   319,   319,   320,   320,   320,   321,   322,   322,
     322,   323,   323,   324,   324,   325,   325,   326,   326,   327,
     328,   328,   329,   329,   330,   330,   331,   331,   332,   333,
     333,   334,   335,   336,   337,   338,   339,   339,   340,   341,
     341,   342,   342,   343,   343,   343,   343,   343,   344,   344,
     345,   345,   346,   346,   346,   347,   348,   349,   349,   350,
     350,   351,   351,   352,   352,   352,   353,   353,   353,   354,
     355,   356,   356,   356,   357,   357,   358,   358,   359,   359,
     359,   360,   361,   361,   362,   362,   363,   363,   364,   365,
     366,   367,   368,   368,   369,   369,   369,   369,   369,   369,
     370,   370,   371,   371,   372,   373,   373,   373,   373,   373,
     374,   375,   375,   376,   376,   376,   376,   377,   378,   379,
     380,   380,   381,   382,   382,   382,   382,   382,   383,   384,
     385,   385,   385,   385,   386,   387,   388,   389,   390,   390,
     391,   391,   392,   392,   393,   394,   394,   395,   396,   396,
     396,   397,   397,   398,   398,   399,   400,   400,   401,   402,
     402,   403,   403,   404,   405,   405,   406,   407,   407,   407,
     408,   408,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   410,   410,   411,   412,   413,   414,   415,   416,   417,
     417,   418,   419,   420,   421,   422,   422,   422,   423,   423,
     423,   424,   424,   424,   424,   425,   426,   426,   427,   427,
     427,   428,   428,   429,   429,   430,   431,   431,   432,   432,
     432,   432,   433,   433,   433,   434,   435,   436,   437,   438,
     439,   440,   440,   441,   441,   442,   443,   444,   445,   446,
     446,   446,   447,   448,   449,   449,   450,   450,   451,   451,
     452,   453,   454,   455,   456,   456,   457,   458,   459,   459,
     459,   460,   461,   461,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   463,
     463,   464,   465,   466,   466,   466,   466,   466,   466,   466,
     467,   468,   469,   469,   470,   470,   470,   470,   470,   470,
     470,   471,   471,   472,   472,   472,   472,   472,   472,   472,
     473,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   475,   475,
     476,   476,   477,   478,   478,   478,   479,   479,   480,   481,
     481,   482,   482,   482,   482,   482,   483,   484,   484,   485,
     485,   486,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   488,   488,   489,   489,   490,   490,
     491,   491,   492,   492,   493,   493,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   495,   495,   496,   496,   496,
     496,   497,   497,   498,   498,   498,   499,   499,   500,   500,
     500,   501,   501,   501,   502,   502,   502,   503,   503,   504,
     504,   504,   505,   505,   506,   506,   506,   507,   507,   508,
     508,   508,   508,   509,   509,   510,   510,   510,   511,   511,
     511,   512,   512,   512,   513,   514,   515,   515,   516,   517,
     517,   518,   519,   520,   520,   521,   521,   522,   522,   523,
     524,   524,   525,   525,   526,   526,   526,   526,   527,   527,
     528,   528,   529,   529,   530,   530,   530,   531,   531,   531,
     532,   532,   532,   533,   534,   534,   534,   535,   535,   536,
     536,   537,   538,   538,   539,   539,   540,   541,   542,   542,
     542,   543,   543,   544,   544,   545,   546,   546,   546,   547,
     547,   548,   548,   549,   549,   549,   550,   550,   551,   551,
     552,   552,   553,   554,   555,   556,   557,   558,   559,   559,
     559,   559,   560,   560,   560,   561,   561,   561,   562,   562,
     563,   563,   564,   564,   565,   565,   566,   566,   567,   568,
     568,   569,   569,   569,   570,   571,   571,   572,   572,   573,
     573,   574,   574,   575,   575,   576,   576,   577,   577,   578,
     579,   579,   579,   579,   579,   579,   579,   579,   580,   580,
     581,   581,   582,   583,   583,   584,   584,   585,   585,   586,
     587,   587,   587,   587,   587,   588,   588,   589,   589,   590,
     590,   591,   591,   591,   592,   592,   593,   593,   594,   594,
     595,   595,   596,   597,   598,   599,   599,   600,   601,   602,
     602,   603,   604,   604,   605,   605,   605,   606,   606,   498,
     607,   607,   608,   608,   609,   609,   610,   611,   611,   612,
     612,   613,   614,   614,   615,   615,   615,   616,   617,   617,
     618,   618,   618,   618,   618,   619,   619,   619,   619,   619,
     619,   620,   620,   620,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   622,   622,   622,   622,   622,   622,
     622,   622,   623,   623,   623,   623,   623,   623,   623,   623,
     623,   623,   623,   623,   623,   623,   623,   623,   624,   624,
     625,   625,   626,   626,   626,   626,   627,   627,   628,   628,
     629,   629,   629,   629,   630,   631,   632,   632,   633,   633,
     633,   634,   634,   634,   635,   635,   636,   637,   638,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   639,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   639,   639,
     639,   639,   639,   639,   639,   639,   640,   640,   640,   641,
     642,   642,   643,   643,   644,   644,   644,   644,   644,   645,
     645,   645,   645,   645,   646,   647,   647,   648,   649,   649,
     649,   649,   649,   650,   650,   651,   651,   652,   653,   654,
     655,   656,   656,   657,   658,   658,   658,   658,   659,   659,
     659,   659,   659,   659,   659,   659,   659,   659,   659,   660,
     661,   662,   663,   663,   664,   665,   665,   666,   666,   667,
     668,   668,   669,   669,   669,   670,   670,   671,   671,   672,
     672,   673,   673,   674,   674,   675,   676,   677,   678,   678,
     678,   678,   678,   678,   678,   678,   679,   679,   680,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   706,   707,   707,   707,
     707,   707,   707,   707,   707,   707,   707,   707,   707,   707,
     707,   707,   707,   707,   707,   707
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
       9,     1,     1,     2,     3,     7,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     4,
       2,     2,     1,     0,     1,     1,     6,     6,     1,     1,
       0,     0,     1,     0,     4,     0,     1,     1,     2,     8,
       1,     0,     3,     0,     0,     1,     1,     2,     1,     1,
       1,     2,     6,     1,     1,     3,     0,     1,     1,     0,
       1,     1,     2,     3,     4,     2,     2,     3,     0,     1,
       1,     2,     1,     4,     3,     3,     1,     0,     1,     1,
       2,     3,     4,     0,     1,     2,     0,     1,     1,     5,
       5,     3,     2,     0,     1,     3,     1,     1,     6,     3,
       2,     6,     2,     1,     2,     2,     2,     0,     1,     1,
       2,     0,     2,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     2,     2,     5,     1,     2,     3,
       2,     2,     0,     1,     1,     1,     1,     3,     4,     8,
       4,     0,     6,     1,     1,     2,     2,     0,     3,     3,
       1,     2,     2,     2,     8,     8,     8,     8,     3,     0,
       4,     0,     1,     3,     4,     1,     3,     2,     3,     3,
       1,     1,     0,     1,     2,     4,     1,     1,     3,     1,
       0,     2,     1,     6,     1,     1,     3,     1,     2,     0,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     1,     1,     2,     2,     1,
       2,     3,     4,     2,     6,     4,     8,     9,     6,     8,
       6,     4,     0,     4,     2,     1,     3,     0,     2,     4,
       0,     1,     1,     3,     3,     7,     2,     0,     2,     2,
       3,     3,     0,     1,     2,     4,     1,     3,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     5,     0,     4,
       7,     3,     0,     1,     3,     3,     7,     7,     1,     1,
       4,     4,     7,     2,     1,     3,     1,     1,     6,     3,
       0,     0,     0,     0,     3,     0,     9,     9,     1,     1,
       0,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     5,     5,     5,     5,     5,     5,     2,
       0,     4,     2,     3,     3,     2,     2,     3,     3,     2,
       3,     6,     1,     2,     4,     3,     2,     2,     2,     2,
       3,     1,     3,     3,     3,     2,     2,     4,     4,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     0,     2,     2,     0,     1,
       2,     2,     4,     3,     2,     2,     7,     1,     0,     1,
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     2,     2,     2,     1,     1,     1,     1,     4,     5,
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
       3,     2,     3,     0,     1,     3,     4,     5,     1,     4,
       4,     3,     0,     1,     3,     3,     1,     1,     0,     1,
       1,     4,     0,     2,     4,     0,     3,     0,     2,     0,
       2,     0,     9,    11,     0,     0,     0,     0,     3,     2,
       1,     0,     3,     5,     3,     2,     5,     2,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     0,     2,     1,
       3,     1,     1,     1,     6,     1,     0,     3,     0,     1,
       3,     6,     3,     4,     2,     3,     0,     1,     2,     1,
       1,     1,     2,     1,     2,     1,     2,     0,     3,     0,
       1,     3,     1,     2,     0,     2,     0,     2,     0,     4,
       1,     2,     1,     1,     0,     1,     3,     2,     1,     1,
       2,     1,     4,     3,     1,     3,     4,     0,     8,     5,
       1,     1,     4,     4,     4,     1,     1,     5,     5,     1,
       3,     3,     7,     7,     5,     3,     0,     1,     1,     1,
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
       1,     1,     3,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     6,
       1,     6,     1,     4,     7,     2,     0,     4,     3,     6,
       1,     1,     6,     8,     6,     1,     1,     4,     6,     4,
       5,     3,     5,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   584,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,   661,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   566,    18,   571,   572,   573,     5,    17,   565,
     567,    19,   632,   628,    11,     8,   740,   741,    21,   745,
     746,     4,   478,     0,     0,     0,     0,     0,     0,   412,
     583,   586,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,     0,   103,   638,   917,     0,    22,   915,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,     0,   362,    30,
      38,    35,    37,   765,    36,   765,  1005,  1007,  1009,  1008,
    1014,  1010,  1017,  1015,  1011,  1013,  1016,  1012,  1018,  1019,
    1020,  1022,  1023,  1021,  1024,  1025,     0,    32,     0,    33,
       0,    67,    69,   997,  1006,     0,     0,     0,     0,     0,
       0,     0,     0,   582,     0,     0,   977,   655,   660,     0,
       0,   591,     0,   697,   699,   999,     0,     0,     0,   115,
     574,  1004,     0,     2,     0,   979,   645,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,   440,   441,     0,   989,   419,
       0,   993,   416,   259,   996,     0,   699,   415,   390,  1000,
     588,   581,   913,     0,   111,   159,   158,   112,   141,   208,
     777,   985,     0,   110,   153,   992,     0,   106,   259,   113,
     919,   130,   107,   193,   108,     0,   109,   765,  1003,   114,
     154,   636,   637,     0,   918,   916,     0,    75,     0,   990,
     716,   499,   988,   492,     0,   498,   503,   493,   494,   501,
     495,   496,   497,   502,   327,     0,   261,    31,     0,    40,
     764,    39,     0,     0,     0,     0,     0,     0,   770,     0,
       0,     0,     0,     0,     0,     0,   580,   923,     0,   904,
     654,   654,   655,   667,   665,   976,   668,     0,   669,   659,
     655,     0,     0,     0,     0,     0,     0,   607,   600,   589,
     590,   592,   594,   595,   596,   603,   604,   597,     0,   698,
     998,   116,   259,   117,   193,   118,   765,   577,     0,     0,
     978,     0,     0,   647,     0,     0,     0,     0,   417,   479,
     382,     0,   382,     0,   211,   418,   422,   413,   476,   477,
       0,   261,   232,   468,     0,   414,   431,     0,   388,   389,
     488,   587,   585,   914,   156,   156,     0,     0,     0,   168,
     160,   161,   140,     0,   104,     0,     0,   119,   261,   129,
     128,   131,     0,     0,     0,     0,     0,   981,    76,   211,
       0,     0,   742,   743,   500,     0,     0,     0,     0,   881,
     910,     0,   980,     0,   883,     0,     0,   911,     0,   905,
     896,   895,   894,  1005,     0,     0,     0,   886,   887,   888,
     897,   898,   906,   907,  1009,   908,   909,     0,  1014,  1010,
     912,  1024,     0,     0,     0,     0,   864,   359,   859,   774,
     890,   884,   860,   325,   899,   862,   863,   878,   879,   880,
     900,   861,   924,   925,   926,   940,   927,   942,   867,   865,
     866,   950,   951,   955,   956,   885,     0,     0,   985,     0,
       0,     0,   363,   209,     0,     0,     0,   767,     0,     0,
      28,    34,     0,     0,    44,    70,    72,  1001,    68,     0,
       0,     0,   769,     0,    53,     0,     0,     0,     0,    52,
     579,     0,     0,     0,   664,   662,     0,   671,     0,   670,
     672,   677,   658,     0,   626,     0,   601,   612,   598,   612,
     599,   613,   619,   623,   624,   608,   593,   716,   749,   778,
       0,     0,   261,     0,     0,   576,   575,   631,   633,   638,
     630,   629,   643,     0,   651,   176,     0,   481,   137,     0,
     484,     0,   485,   480,     0,   426,     0,   428,   427,     0,
     429,     0,   423,     0,     0,   262,     0,     0,   435,     0,
     436,   382,   469,     0,   475,     0,     0,     0,   472,   985,
     393,   392,   487,     0,   410,   157,     0,     0,   166,     0,
     165,     0,     0,   155,   169,   170,   172,   162,   535,   547,
     522,   557,   525,   548,   558,     0,   564,   526,     0,   554,
       0,   549,   561,   521,   544,   523,   524,   519,   382,   504,
     505,   506,   514,   520,   507,   516,   537,     0,   542,   539,
     517,   554,   518,   120,   259,   121,     0,     0,   132,     0,
       0,   192,   232,   390,   380,     0,   211,     0,    82,    84,
       0,     0,   102,   101,   982,     0,     0,     0,     0,   848,
     849,   850,     0,   715,   782,   791,   794,   799,   795,   796,
     797,   801,   802,   800,   803,   798,   780,     0,   977,     0,
     977,   901,     0,     0,   977,   977,   977,     0,   902,   903,
       0,     0,   966,     0,     0,     0,     0,     0,   869,   868,
     661,     0,   890,     0,   324,  1002,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   267,   514,   537,     0,
       0,   361,     0,   272,     0,   766,    29,     0,     0,    45,
      73,     0,     0,    27,   756,     0,   772,   661,     0,   739,
     756,     0,     0,     0,     0,   578,     0,   654,     0,     0,
     716,     0,   676,     0,   569,     0,   568,   921,     0,   602,
       0,   610,   606,   605,     0,   616,     0,   621,   609,     0,
     747,   748,     0,     0,     0,   232,   380,     0,   642,     0,
     649,     0,   627,     0,   138,   177,   483,   382,   425,     0,
     382,   383,   430,   380,     0,   258,   380,   231,   986,   203,
     200,     0,   213,   382,   504,   380,     0,     0,     0,   230,
     233,   234,   235,   236,     0,   434,   474,   473,   433,   432,
       0,     0,     0,   396,   394,   395,   391,     0,   382,   186,
     163,   167,     0,     0,   171,     0,     0,   532,   759,   545,
     546,   562,     0,   559,   564,     0,   552,   550,   551,   143,
       0,     0,     0,   515,     0,     0,     0,     0,   553,   261,
       0,   380,   133,   191,     0,   194,   196,   197,     0,     0,
       0,     0,   211,   211,    79,     0,    87,    78,     0,     0,
       0,     0,   793,   781,     0,     0,   851,   852,   853,     0,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   968,   969,   970,   971,   972,   973,
     974,   975,     0,     0,     0,     0,   961,     0,     0,     0,
       0,     0,     0,   977,   877,   876,   326,     0,   872,   984,
     871,   870,   873,   874,   875,   891,   948,     0,   882,   776,
     775,   365,   364,     0,   265,   280,     0,   271,   273,   768,
       0,     0,     0,     0,     0,     0,    25,    54,    42,    62,
      64,     0,     0,    71,     0,   761,     0,   771,   977,     0,
     761,    50,    48,     0,     0,     0,   663,   666,     0,   678,
     679,   683,   681,   682,   696,     0,   709,   673,   675,   994,
     570,   922,   611,   620,   615,   614,     0,   625,   750,   751,
     779,   380,     0,     0,   634,   656,   635,   644,     0,   646,
     650,   482,   156,   183,   175,   178,   179,     0,     0,     0,
     424,   380,   380,   383,   270,   263,   272,   202,   382,     0,
     383,   204,   199,   205,     0,     0,   241,     0,   438,   383,
     382,   380,   437,     0,     0,     0,   409,   490,   187,   188,
     164,     0,   174,   533,   534,   920,     0,     0,     0,   537,
       0,   560,     0,     0,   382,   383,     0,   510,   512,     0,
       0,     0,     0,     0,     0,     0,   390,   272,   156,   135,
     232,   189,   410,   977,   385,   368,   369,     0,    83,     0,
     211,     0,    91,     0,    85,     0,     0,    90,    86,     0,
     744,   987,     0,     0,   786,     0,     0,   792,     0,     0,
     784,   783,     0,   838,     0,   829,   828,     0,   807,     0,
     836,   854,   846,     0,     0,     0,   808,   832,     0,     0,
     811,     0,     0,     0,     0,     0,     0,     0,   809,     0,
       0,   810,     0,   840,     0,     0,   804,     0,     0,   805,
       0,     0,   806,     0,     0,   211,     0,   928,     0,     0,
     943,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     967,     0,     0,   965,     0,     0,   957,     0,     0,     0,
       0,     0,   889,   947,     0,   260,   279,     0,   337,   360,
     274,    66,    58,    59,    65,    60,    61,     0,     0,     0,
      23,    26,    42,    43,   535,   535,     0,   753,   773,     0,
       0,   752,    51,    49,    46,     0,     0,   707,   681,     0,
     705,   700,   701,   703,     0,     0,   677,     0,   714,   617,
     618,   622,   272,   190,   385,     0,   648,     0,   184,     0,
     180,     0,     0,     0,   420,   219,   214,   892,   215,   216,
     217,   218,     0,     0,   264,     0,     0,   212,   220,     0,
     241,   638,   237,     0,   439,   470,   471,   401,   402,   398,
     397,   400,   399,   382,   489,   383,   173,   983,     0,     0,
     528,     0,   527,   563,     0,   555,     0,   144,     0,   508,
       0,   536,   537,   543,   541,     0,   538,   380,     0,     0,
       0,   127,   136,   195,   382,     0,     0,   381,   979,   105,
       0,   125,   124,    94,     0,     0,    95,     0,    88,    89,
       0,   844,   845,   788,   787,   790,   789,     0,     0,     0,
     661,     0,     0,   847,     0,     0,     0,     0,     0,     0,
     839,   837,   833,     0,   841,     0,     0,     0,     0,   842,
       0,     0,     0,     0,     0,     0,   934,   933,     0,     0,
     930,   929,     0,   938,   937,   936,   935,   932,   931,     0,
     946,   963,   962,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,   276,   277,     0,   985,     0,     0,
       0,     0,     0,     0,     0,  1007,  1012,     0,     0,   358,
     290,   285,     0,   337,   284,   294,   295,   296,   297,   298,
     305,   306,   299,   301,   302,   304,     0,   309,     0,   352,
     354,   353,     0,     0,     0,     0,     0,     0,   985,    55,
      56,     0,    57,    63,    24,   757,     0,   758,   755,   760,
     763,   762,     0,   738,     0,    47,   677,   692,   680,   706,
     702,   704,     0,     0,     0,     0,     0,   718,     0,   381,
     639,   640,   657,   186,   185,   181,     0,   383,   893,   268,
     269,   383,   383,   207,   221,   222,   382,     0,   238,     0,
       0,     0,     0,     0,     0,     0,     0,   272,   486,   530,
       0,   531,     0,   142,     0,   207,   145,   146,   148,   149,
     150,   511,   513,   509,   540,   272,   410,   383,   186,     0,
       0,     0,   366,     0,     0,   382,    92,     0,     0,    98,
      93,   537,     0,   830,     0,     0,   977,   856,   855,     0,
       0,   834,     0,     0,     0,     0,   843,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   949,   210,   939,
     941,     0,     0,   964,   959,   958,     0,   952,   954,     0,
     716,   213,   275,     0,   313,     0,     0,     0,   308,     0,
     322,   307,   338,   339,     0,   995,   355,   356,     0,   286,
       0,   291,   288,   343,     0,     0,   310,     0,   300,   293,
     303,   292,   336,    41,   535,   716,   686,   707,   695,   693,
     708,   710,   712,   713,   724,   652,   383,   367,   641,   182,
     383,   152,   257,     0,     0,   198,   223,     0,     0,     0,
     241,   224,   227,   765,     0,   383,   407,   408,   404,   403,
     406,   405,     0,   529,   556,   151,   139,   147,     0,   382,
     254,   134,   383,     0,   716,   384,   371,   370,     0,     0,
      96,    97,    80,     0,   815,   823,     0,   816,   824,   819,
     827,   831,   835,   817,   825,   818,   826,   812,   820,   813,
     821,   814,   822,   945,   944,   960,     0,   709,     0,   282,
     278,     0,   311,     0,   327,     0,     0,     0,   340,     0,
     351,     0,     0,     0,     0,   346,   344,     0,     0,     0,
     331,     0,   332,     0,   754,   709,   688,   685,     0,     0,
     720,   722,   723,   717,     0,   255,   421,   201,   206,   225,
     241,   765,   228,   247,   240,   242,   411,   383,     0,   386,
       0,   373,   374,   376,   377,   709,   383,    77,    99,   100,
       0,   953,   714,   281,     0,   312,   330,     0,   315,     0,
     321,     0,   350,   349,   991,   348,   337,     0,     0,     0,
       0,   323,     0,     0,   714,     0,   684,   691,   711,   721,
     724,   229,   247,     0,   241,   243,   244,   256,   383,   707,
     376,     0,     0,   714,     0,   716,   718,     0,     0,     0,
       0,   337,   357,   345,   347,     0,   342,     0,   334,   333,
     718,     0,   689,   728,     0,   725,     0,   729,   241,     0,
       0,   239,     0,   246,     0,   245,   387,   379,   375,     0,
     372,   126,   709,   632,   283,     0,   328,   314,   318,   320,
       0,     0,     0,   337,   632,     0,   687,   719,   724,     0,
     731,     0,   727,   730,   226,     0,     0,   250,   249,   248,
     707,   714,   645,     0,   337,     0,     0,   337,   335,   645,
     690,   726,     0,   737,   253,   251,   252,     0,   718,   858,
     329,   319,   337,   341,   316,   653,     0,   734,     0,   733,
     378,   632,   317,   732,     0,     0,   645,   735,     0,   857,
     736,     0,     0,     0
};

static const short yydefgoto[] =
{
    1911,    20,    21,   479,   126,   127,   128,   129,  1220,   733,
     728,    22,   141,   976,   977,   978,   979,   130,   131,   484,
     485,    23,    82,   388,   876,   647,   648,   649,  1118,  1111,
    1112,  1540,   247,   651,    24,    74,    25,   159,    26,   377,
      75,  1329,   229,   381,   639,  1099,  1321,   547,   217,   373,
    1085,  1515,  1516,  1517,  1518,  1519,  1520,   223,   239,   214,
     586,   215,   369,   370,   371,   593,   594,   595,   548,   549,
    1034,  1035,  1036,  1259,  1070,   232,   323,   383,   864,   865,
     866,   800,   801,   802,  1635,   569,   469,  1378,   650,  1050,
    1264,  1493,  1494,  1495,  1641,   867,   571,   809,   810,   811,
     812,  1282,   813,  1794,  1795,  1796,  1868,   227,   321,   633,
     202,   351,   475,   564,   565,   963,   470,  1043,   966,   967,
     968,  1403,  1404,  1207,  1700,  1405,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1707,  1434,   436,   437,  1809,  1721,  1435,  1436,  1437,  1852,
    1602,  1603,  1714,  1715,  1438,  1439,  1440,  1441,  1598,    27,
      28,   266,   471,   472,   236,   325,  1104,  1105,  1106,  1664,
    1751,  1752,  1753,   869,  1532,   556,  1040,  1327,   234,   635,
     360,   582,   583,   826,   828,  1294,    29,    59,   791,   788,
     345,   346,   355,   356,   194,   195,   357,  1059,   577,   818,
     199,   167,   338,   339,   207,   584,  1295,    30,    96,   618,
     619,   620,  1086,  1087,   621,   717,   623,   624,  1079,  1455,
     853,   625,   718,   627,   628,   629,   630,   846,   631,   632,
     843,    31,    32,    33,    34,    35,   536,    36,   501,    37,
      38,    61,   211,   362,    39,   309,   310,   311,   312,   313,
     314,   315,   316,   762,   317,  1016,  1251,   521,   522,   767,
     523,    40,  1442,    42,   166,   537,   538,   243,  1482,  1026,
     333,   544,  1029,   782,    43,   739,   504,   299,  1255,  1628,
     146,   147,   148,   297,   508,   509,   510,   753,   750,   999,
    1000,  1001,  1726,  1786,  1821,  1002,  1003,  1246,  1754,   153,
    1244,  1248,  1620,  1621,  1477,   392,  1625,  1823,  1734,  1824,
    1825,  1826,  1862,  1896,  1899,  1443,  1444,    46,    47,   393,
    1445,    49,    50,   527,  1446,    51,   985,  1456,  1227,  1459,
     269,   270,   476,   491,   492,   735,   438,   439,   530,   663,
     889,   664,   665,   666,   667,  1137,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   891,  1140,  1141,  1351,   701,
     677,   441,   442,   443,  1266,   444,   445,   446,   447,   448,
     449,   450,   838,    79,   245,   231,  1077,  1088,   757,   451,
     452,   453,   454,   455,   456,   457,  1572,   458,   459,   460,
     461,   462,   463,   939,   464,   693,  1194,   694,  1191,   932,
     298,   331,   465,   466,   653,  1296,   948,   220,   797,  1120,
     251,   197,   248,  1773,   224,  1897,  1008,  1447,  1004,   132,
     467,   206,   208,   980,   704,   237,   160,   468,   134
};

static const short yypact[] =
{
    3917,  1412,   284,  1275,   478,   184,   442,  2016,   151,  3031,
     614,   598,  2781,   284,   692,   381,  5890,   324,  5890,   384,
     378,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    86,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  7231,  5890,  5890,  5890,  5890,  5890,-32768,
  -32768,   540,   291,  5890,  5890,  5890,   723,  5890,   543,  5890,
    5890,-32768,  5890,  5890,-32768,   210,-32768,   546,-32768,-32768,
     683,  5890,-32768,  5890,  5890,  5890,   703,  5890,  5890,  5890,
    5890,   543,  5890,  5890,  5890,  5890,-32768,  5890,   553,   696,
  -32768,-32768,-32768,   567,-32768,   567,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   776,   574,   833,-32768,
      18,-32768,-32768,-32768,-32768,  5890,  5890,  5890,   866,   876,
     884,  1025,   131,   828,     1,   444,   223,   798,-32768,  5890,
     942,   883,   877,-32768,  5890,-32768,  5890,  5890,  5890,-32768,
  -32768,-32768,  5890,-32768,  1037,   464,   804,  1086,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   305,-32768,-32768,   291,-32768,-32768,
     195,-32768,-32768,   810,-32768,   846,-32768,-32768,   496,-32768,
     916,-32768,-32768,   897,-32768,   770,-32768,-32768,  1108,-32768,
  -32768,-32768,   291,-32768,-32768,-32768,   348,-32768,   810,-32768,
  -32768,   420,-32768,  1067,-32768,  1072,-32768,   567,-32768,-32768,
  -32768,-32768,-32768,  1071,-32768,-32768,   956,-32768,  1082,-32768,
     973,-32768,-32768,-32768,   956,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  6666,  5890,  1018,-32768,  5890,-32768,
  -32768,-32768,  2291,   697,  1027,  4449,  5890,  1092,   911,  1126,
    1131,  2291,  1052,  1075,  1081,  4449,   965,-32768,  6897,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  4806,-32768,-32768,
  -32768,  1042,  5890,  1118,  1035,   394,  5890,  1055,-32768,-32768,
     883,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5890,-32768,
  -32768,-32768,   810,-32768,  1067,-32768,   567,  1115,  6897,  1076,
  -32768,  1076,  6897,  1182,    68,   990,   993,   991,  1086,-32768,
    1217,   196,-32768,  5890,-32768,   305,-32768,-32768,-32768,-32768,
    5890,  1018,  2194,  1049,  2543,   978,-32768,  7231,-32768,-32768,
     839,-32768,-32768,-32768,   987,   987,   291,  1151,   291,   167,
     770,-32768,-32768,  5545,-32768,  5890,  5890,-32768,  1018,-32768,
  -32768,  1246,   414,  1012,  5890,  1266,  5890,-32768,-32768,    41,
    6297,  3882,-32768,-32768,-32768,  1015,  1020,  1022,  1083,-32768,
  -32768,  1024,-32768,  1028,-32768,  1032,  1033,-32768,  1038,-32768,
  -32768,-32768,-32768,  1039,  1047,  1084,  1091,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1048,-32768,-32768,  5037,  1053,  1054,
  -32768,  1056,  6897,  6897,  5268,  5890,-32768,-32768,  1058,-32768,
    1004,-32768,-32768,  1026,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1059,  1050,  1057,  5545,
    1061,   529,-32768,-32768,  1068,  1329,   533,-32768,  5890,  1171,
  -32768,-32768,  5890,  5890,   308,-32768,-32768,-32768,-32768,  5890,
    5890,   610,-32768,  5890,-32768,  1283,  5890,  2291,  1210,  1087,
  -32768,  5890,  5268,   327,-32768,-32768,  6897,-32768,  1292,  1088,
  -32768,   336,-32768,   461,-32768,    76,-32768,    88,-32768,    88,
  -32768,  1090,-32768,    71,-32768,  1191,-32768,    14,-32768,-32768,
    1104,  1094,  1018,  1100,  1363,-32768,-32768,  1105,-32768,   606,
  -32768,-32768,   691,  1197,  1187,-32768,  1290,  1291,-32768,   291,
  -32768,  1293,-32768,-32768,  1360,-32768,  1370,-32768,-32768,  1350,
  -32768,  5545,-32768,  5545,   585,-32768,  1384,  5890,-32768,  7135,
  -32768,   458,-32768,  5890,   766,   846,  1228,  1196,  1274,  1237,
  -32768,-32768,-32768,   350,  1276,-32768,  1221,   543,-32768,   291,
  -32768,  1392,  1328,-32768,   167,-32768,-32768,-32768,   489,  1248,
  -32768,-32768,-32768,  1250,-32768,  1298,  1166,-32768,  1349,  1169,
     621,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1170,-32768,-32768,-32768,    98,  1174,  1176,    -4,
  -32768,  1169,-32768,-32768,   810,-32768,  1389,  1442,-32768,   291,
     291,-32768,  2194,   496,-32768,  1331,-32768,  1307,  1186,-32768,
    5314,  1330,-32768,-32768,-32768,  1338,  1189,  4113,  1192,  1121,
    2112,  2441,  3638,   181,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1234,   472,  6897,
     149,-32768,  5890,  6897,   616,   706,   772,  1214,-32768,-32768,
    6897,  4344,  1004,   230,  1277,  6897,  6897,  4344,   214,   214,
     692,  1195,   377,   730,-32768,-32768,  6897,  5890,  6897,  6897,
    6897,  6897,  6897,  6897,  5499,  2710,-32768,-32768,  1449,  1193,
    5890,-32768,  5890,  1431,  5890,-32768,-32768,  3522,  1301,-32768,
  -32768,  1281,  4449,-32768,  1416,   745,-32768,   692,  1205,-32768,
    1416,  3522,  1421,  1422,  5890,-32768,   377,-32768,   387,  5607,
    1289,  6897,-32768,  5890,-32768,   664,-32768,-32768,   133,-32768,
    1300,-32768,-32768,-32768,  5890,   789,  5890,-32768,-32768,  5890,
  -32768,-32768,  6897,  5890,  1477,  2194,-32768,  6897,  1249,  6897,
    1371,  1251,-32768,   291,-32768,   494,-32768,-32768,-32768,  1223,
  -32768,-32768,-32768,    81,  5890,-32768,-32768,-32768,-32768,  1474,
  -32768,  1224,  1467,-32768,  1484,-32768,  1427,  1428,   567,-32768,
  -32768,-32768,-32768,-32768,  1499,-32768,-32768,-32768,-32768,-32768,
    5890,  7173,   543,  1400,  1401,  1413,-32768,   543,-32768,   807,
  -32768,-32768,  1385,   291,-32768,  6297,    28,  1406,-32768,-32768,
  -32768,-32768,   543,-32768,  1166,   524,-32768,-32768,-32768,  1511,
     524,  1405,   524,-32768,   543,   543,  1294,   543,-32768,  1018,
    5890,-32768,  1464,-32768,   779,-32768,-32768,-32768,   839,  1410,
    5890,   806,   259,-32768,-32768,  1295,   452,-32768,  6297,  5890,
    1417,  4575,-32768,-32768,  1417,  4113,-32768,-32768,-32768,   105,
  -32768,   416,  3424,  4344,  4344,  6897,  6897,  1309,  1296,   129,
    1784,  6897,  2072,   288,  2407,  2657,  5730,  2907,  3157,  3407,
    6897,  6897,   307,  6897,  1305,  6897,  1302,   410,  6897,  6897,
    6897,  6897,  6897,  6897,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1516,   992,   125,  6897,-32768,  4344,  6897,   250,
     400,   434,   103,   223,-32768,-32768,-32768,   588,-32768,-32768,
    1551,   214,   214,   834,   834,  1004,-32768,   824,-32768,-32768,
  -32768,-32768,-32768,   830,-32768,  1396,  1566,  1431,-32768,-32768,
    5890,  5890,  5890,  5890,  5890,  5890,  1314,-32768,   310,-32768,
  -32768,  3522,  1462,-32768,   396,  1483,  5890,-32768,   223,  6897,
    1483,  1314,  1321,  3522,  3987,  1530,-32768,-32768,  2960,  1323,
     799,-32768,-32768,-32768,  1324,  4344,  1526,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   581,-32768,-32768,  1004,
  -32768,-32768,   835,  1410,-32768,-32768,-32768,  1004,  5890,-32768,
  -32768,-32768,   987,  1586,-32768,   494,-32768,  1327,  4344,  4957,
  -32768,-32768,-32768,-32768,-32768,-32768,  1431,-32768,-32768,  4957,
  -32768,-32768,-32768,-32768,   567,   567,  1361,  1332,-32768,-32768,
  -32768,-32768,-32768,   116,   147,   165,-32768,  1591,-32768,-32768,
  -32768,  5890,-32768,-32768,-32768,-32768,   478,   859,  1443,  1449,
    1333,-32768,  1221,   863,-32768,-32768,   677,-32768,  1334,  5890,
     715,  1336,  1337,   543,  1340,  1592,   496,  1431,   987,  1291,
    2194,-32768,  1276,   223,  1419,  1438,-32768,  1859,-32768,   543,
    1472,  1565,-32768,  5314,-32768,   543,   376,-32768,-32768,  1568,
  -32768,-32768,  1348,  1351,-32768,  4344,  4344,-32768,  4344,  4344,
  -32768,  1614,   178,  1004,  1354,-32768,-32768,  1355,  1004,  5961,
  -32768,-32768,-32768,  1529,  1356,  1359,  1004,   986,  1362,  1364,
    1004,  6897,  6897,  1296,  6897,  6192,  1365,  1367,  1004,  1368,
    1373,  1004,  6897,  1004,  1374,  1375,  1004,  1377,  1378,  1004,
    1379,  1381,  1004,   613,   624,-32768,   642,-32768,   652,  6897,
  -32768,   666,   676,   710,   726,   744,   792,  6897,  6897,  6897,
    1004,  1581,   158,  1004,  1470,  6897,-32768,  6897,  6897,  6897,
    6897,  6897,-32768,-32768,  5890,-32768,-32768,  5890,  4244,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  3522,  1585,  3522,
  -32768,  1314,   310,-32768,   415,   415,   245,-32768,-32768,  4806,
     874,-32768,  1314,  1321,  1386,  3987,  1383,   595,  1387,  5607,
    1531,-32768,  1532,  1534,  1577,  6897,  1043,  1644,  1593,-32768,
  -32768,-32768,  1431,-32768,  1419,   675,  1393,  1221,  1535,  1221,
  -32768,  4344,   110,  1005,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  4957,  4957,-32768,  1566,  6897,-32768,   704,  1522,
    1361,   210,-32768,  4344,-32768,-32768,-32768,  1543,  1544,  1546,
    1550,  1552,  1553,-32768,-32768,-32768,-32768,-32768,  1403,   478,
  -32768,  1485,-32768,-32768,   524,-32768,  4957,   471,   524,-32768,
     524,-32768,  1449,-32768,-32768,  1407,-32768,-32768,   839,  1566,
    1221,-32768,  1291,-32768,-32768,  4806,  1657,-32768,   788,-32768,
    1411,-32768,  1393,-32768,  1414,   291,   117,  1415,-32768,-32768,
     291,-32768,-32768,-32768,  1614,-32768,  1614,  6897,  1536,  1536,
     692,  1423,   880,-32768,  1536,  1536,  6897,  1536,  1536,   185,
    1004,-32768,  1000,  6897,  1004,  1536,  1536,  1536,  1536,  1004,
    1536,  1536,  1536,  1536,  1536,  1536,-32768,-32768,  1424,  5545,
  -32768,-32768,   873,-32768,-32768,-32768,-32768,-32768,-32768,   889,
    1011,-32768,-32768,  6897,  6897,  1633,  1512,   918,   893,   780,
    1096,-32768,  5545,  1420,-32768,-32768,  1650,  1656,  5835,    44,
    1425,  1426,  1612,  6897,  1429,  1434,  6066,  5890,  5890,-32768,
  -32768,-32768,  1658,  3774,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   156,-32768,  1440,-32768,
  -32768,-32768,  1619,  1441,  1444,  1447,  1455,  1452,   -52,-32768,
  -32768,  1603,-32768,-32768,-32768,-32768,  1554,-32768,-32768,-32768,
  -32768,-32768,  1292,-32768,  3987,  1386,  1719,-32768,   799,-32768,
  -32768,-32768,  5607,   900,  5890,  6897,  4344,  1595,  1566,-32768,
  -32768,-32768,-32768,   807,-32768,-32768,   152,-32768,-32768,-32768,
  -32768,-32768,  1004,  1708,   704,-32768,   749,  5890,-32768,  1653,
     162,  1562,  1654,  1563,  1689,  1665,  1702,  1431,-32768,-32768,
    1479,-32768,  1482,-32768,  1668,  1708,   471,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1431,  1276,-32768,   807,  1591,
    1700,  1642,-32768,  1410,  1410,-32768,-32768,  1662,    78,-32768,
  -32768,  1449,  1666,  1004,  1495,  1496,   223,-32768,-32768,  1505,
    1507,  1004,  1515,  1518,  6897,  6897,  1004,  1521,  1523,  1525,
    1527,  1528,  1533,  1537,  1538,  1540,  1541,-32768,-32768,-32768,
  -32768,  6897,  1542,-32768,-32768,-32768,  6897,-32768,-32768,  6897,
    1289,   141,-32768,  1722,-32768,  6423,  5890,  6897,-32768,  4344,
    1004,-32768,-32768,-32768,  1514,-32768,-32768,-32768,  5890,-32768,
     560,-32768,  1596,-32768,   412,  1545,-32768,  5554,-32768,-32768,
  -32768,-32768,-32768,-32768,   415,  1289,  5890,  1142,-32768,-32768,
    1524,-32768,  1004,   181,   673,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1547,  5890,-32768,-32768,  1705,  1717,  5890,
    1361,-32768,-32768,   567,  5890,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1566,-32768,-32768,-32768,-32768,-32768,  1566,-32768,
  -32768,-32768,-32768,  5643,  1289,-32768,-32768,-32768,  6897,   291,
  -32768,  1601,-32768,   291,-32768,-32768,  6897,-32768,-32768,-32768,
  -32768,  1004,  1004,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1004,-32768,-32768,   945,  1526,  4957,-32768,
  -32768,  1555,-32768,   843,  6666,   818,   163,  1539,-32768,  1725,
  -32768,  5890,  5890,   478,    97,-32768,-32768,  1573,  1743,  4344,
  -32768,   711,-32768,  1549,-32768,  1526,  1556,-32768,  4344,  6897,
  -32768,-32768,  1721,-32768,  1559,-32768,-32768,-32768,-32768,-32768,
    1361,   567,-32768,  1710,-32768,-32768,-32768,-32768,  1591,-32768,
    5643,  1564,   799,-32768,-32768,  1526,  1004,-32768,-32768,-32768,
    1096,-32768,  1593,-32768,  1417,-32768,  1685,  5554,-32768,  1661,
  -32768,  5554,-32768,-32768,-32768,-32768,  4703,   560,  6897,  5554,
     170,-32768,  5554,  6490,  1593,  5890,-32768,   181,-32768,-32768,
    1510,-32768,  1710,   234,  1361,  1714,  1715,-32768,-32768,   595,
    1567,  5643,  1753,  1593,  1574,  1289,  1595,  1575,  3086,  1569,
     771,  4703,  1570,-32768,-32768,  1154,   111,  1804,-32768,-32768,
    1595,   908,-32768,-32768,  1582,  1583,   861,  5890,  1361,   325,
     325,-32768,  1812,-32768,  1677,-32768,-32768,-32768,   799,  5643,
  -32768,-32768,  1526,  1732,-32768,  5554,  1570,-32768,-32768,  1809,
    5554,  1825,  1815,  4703,  1732,  5890,-32768,-32768,  1510,  1594,
  -32768,  5890,-32768,-32768,-32768,  1663,   168,-32768,-32768,-32768,
    1244,  1593,   804,   944,  4703,   122,  5890,  4703,-32768,   804,
  -32768,-32768,  5890,  1790,-32768,-32768,-32768,  4344,  1595,-32768,
  -32768,-32768,  4703,-32768,-32768,-32768,  1590,  1599,  1600,-32768,
     181,  1732,-32768,-32768,  5890,  5890,   804,-32768,  1598,-32768,
  -32768,  1874,  1875,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  -146,   175,-32768,   228,  1604,   654,-32768,
    -374,-32768,-32768,  -621,  -212,  -690, -1052,  1867,  1606,  1602,
    1152,-32768,-32768,-32768,   775,-32768,  1239,  1016,-32768,-32768,
     781,-32768,-32768,  1014,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   794,-32768,-32768,
  -32768,-32768,-32768,   382,-32768,-32768,  1557,-32768,-32768,-32768,
    -325,-32768,-32768,-32768,  1572,-32768,-32768,  1306,  -348,-32768,
  -32768,-32768,   864,-32768,  -709,-32768,-32768,  1578,  1128,   801,
    1561,  1120,   797,-32768,   393,   -46,  -251,-32768,  -320,   328,
    -875,-32768,-32768,   428,-32768,  1584, -1136,-32768,-32768,-32768,
  -32768, -1237,   430,   135,   142,   144,   107,-32768,-32768,-32768,
  -32768,  -119,  -312,-32768,  1156,-32768,  -651,-32768,  -945,-32768,
     979,-32768,-32768,-32768,-32768,-32768, -1326,  -859,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768, -1159,   258,-32768,  -411,-32768,-32768,-32768,-32768,
  -32768,   361,-32768,   187,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1247,-32768,-32768,   943,-32768,  -497,-32768,
  -32768, -1200,   218,  -535,   490,  -289,  -909,   716,-32768,-32768,
    -541,  -776,-32768,-32768, -1007, -1412,-32768,-32768,-32768,-32768,
  -32768,  1626,-32768,  1397,  1616,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1636,-32768,-32768,-32768,-32768,-32768,  -539,
    -432,-32768,  1123,   668,  -580,  -355,-32768,-32768,-32768,  1380,
    -973,-32768,  -354,-32768,-32768,-32768,-32768,  1346,-32768,-32768,
    1141,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1973,-32768,-32768,-32768,-32768,-32768,  1678,-32768,-32768,
  -32768,  1475,-32768,  1468,-32768,-32768,-32768,-32768,  1225,-32768,
  -32768,-32768,    10,-32768, -1636,-32768,  1215,  -501,-32768,-32768,
    -950,-32768,-32768,-32768,   903,  -857,  -218,   335,-32768,-32768,
    -641,-32768,  1844,  -849,  -735,-32768,  1243, -1058, -1285,-32768,
    -907,-32768,-32768,-32768,-32768,   997,-32768,-32768,   648,    43,
   -1240,  -928,-32768,   267,  -986,  -516, -1604,   373,-32768,   140,
  -32768,   173,-32768,  -831,-32768,  2001,  2013,-32768,-32768,  1488,
    2019,-32768,-32768,-32768,  -231,-32768,  1288, -1117,  1045,-32768,
     -99,  -698,  -347,-32768,-32768,-32768,  -476,   -32,  -423,  -662,
  -32768,  -409,-32768,-32768,-32768,   615,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  -532,-32768,   887,-32768,   586,-32768,
    1010,  -935,-32768,  -412,-32768,   753,  -893,  -132,  -888,  -885,
    -877,   -54,     0,   -26,-32768,   -71,  -692,  -480,  -130,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   832,-32768, -1032,-32768,
     437,   708,  -866,   734,  1202,   952,   408,  -683,  1476, -1166,
  -32768,   -59,  1956,-32768,   -19,   -53, -1231,   631,    26,   -45,
    -138,   -15,   -36,  -223,-32768,   -69,   -64,    -9,-32768
};


#define	YYLAST		7493


static const short yytable[] =
{
     133,   154,   200,   133,    78,   222,   271,   155,   216,   161,
      41,   770,   290,   293,   291,   294,   319,   218,   622,   626,
     259,   596,   703,  1122,   561,   262,   253,  1123,   240,   934,
     803,   219,   960,   756,   235,   942,   257,   716,   778,   566,
     587,  1406,   230,  1498,   196,   198,   201,   204,   155,   209,
     263,   992,   486,   559,   221,   198,   225,   261,   204,   943,
     155,   209,   486,   238,   133,   230,   637,   736,   154,   256,
     877,   964,   249,   505,   155,   252,   198,   260,   249,   225,
     201,   204,   203,   155,   209,   238,   133,   528,   204,   326,
    1020,  1237,  1102,   228,  1201,  1324,   988,  1601,   327,   563,
     205,  1275,   868,   277,  1265,   279,  1302,  1209,  1458,   378,
    1056,   893,   233,  1125,  1265,  1580,   258,  1662,   893,   851,
     991,  1851,   742,   264,  1041,   883,   221,   155,   221,   792,
     301,   793,   765,   893,  1274,   495,  1538,   804,   385,  1229,
     225,  1278,  1496,   347,  1776,   320,  1267,   204,   155,   238,
    1284,  1268,  1319,   161,  1269,   295,  1267,  1391,   545,   517,
     893,  1268,  1270,  1395,  1269,  1450,   893,  1453,   374,  1892,
     893,   893,  1270,  1271,  1277,  1670,  1307,  1615,   893,   278,
     531,   856,   322,  1271,  1049,  1586,  1347,   139,  1474,   893,
     -81,  1289,   275,  1554,   285,   913,  1287,   287,   348,   784,
     324,   707,  1843,   532,   391,   760,   708,  1872,   707,  1291,
     591,  1885,   164,   708,   622,   626,  1854,  1604,  1879,   758,
     774,   241,  -995,  1142,  1075,  -998,   557,   534,   894,   295,
    1126,  1130,  1131,   473,  1006,   894,   477,   707,   307,   558,
     140,  1023,   708,    80,    81,  1292,   596,  1143,   882,   514,
     894,  1596,  1597,   890,  1325,   242,   221,   545,  1044,   221,
     730,  1046,  1886,   155,   165,  1906,   487,   133,   857,   296,
    1053,   349,   155,   288,   761,  1192,   487,   894,  1829,   289,
     935,   936,   814,   894,  1901,   769,   529,   894,   894,   276,
    1587,  1222,    97,   201,   947,   894,  1288,   155,  1189,  1076,
    1195,  1196,   957,  1233,  1234,  1151,   894,  1478,  1671,   340,
     519,   560,   588,   646,   590,   480,   283,  1175,   473,  1152,
     219,  1539,   574,   546,   480,   578,  1097,  1290,   641,   849,
     707,  1393,  1468,   645,   221,   708,  1798,  1265,  1265,  1523,
     636,   221,   766,   221,  1042,   221,   752,  1605,   579,   524,
     707,   743,   531,  1124,   341,   708,  1279,  1280,  1496,   707,
    1221,  1573,  1574,  1153,   708,  1083,   204,   209,  1777,   284,
     995,  1265,  1232,   852,  1200,   155,  1262,   201,  1127,  1267,
    1267,   654,  1782,  1487,  1268,  1268,  1508,  1269,  1269,  1060,
     652,  1154,  1109,  1782,   823,  1270,  1270,  1489,  1490,  -674,
     707,   634,   276,  1742,  1698,   708,  1271,  1271,  1616,  1331,
     707,    98,  1453,  1267,  1830,   708,  1491,   914,  1268,   937,
     517,  1269,   592,   707,  1128,  1630,   705,   643,   708,  1270,
     824,  1513,  1709,   707,   379,  1645,  1769,   745,   708,  1197,
    1271,  1075,  1865,  1817,   709,   710,   711,   712,  1720,   149,
    1813,   709,   710,   711,   712,  1155,   829,   707,   529,  1224,
    1527,   342,   708,  1343,  1344,   156,  1345,  1346,  1717,   155,
     329,  1116,   882,   204,   487,  1805,   882,    60,   295,   343,
     155,  1866,   711,   712,   155,  1849,  1252,   204,   155,   375,
     212,   213,   161,  -382,  1462,   785,   157,  1724,  1037,   358,
     731,  1039,  1218,  1791,   640,    83,  1272,  1273,   729,   486,
     330,   158,  1802,   344,  1051,   859,   830,   518,   910,  1659,
     806,   726,   729,   289,   376,   380,  1286,  1878,  1867,   996,
     825,  1110,   734,   877,  1461,   831,   740,   805,  1018,  1067,
     480,  1129,  1526,   563,  1695,  1465,  1117,  1095,  1891,   150,
    1799,  1894,  1113,  1401,  1822,  1318,   151,  1831,   798,  1802,
     221,   230,  1652,  1228,   798,  1617,  1902,    14,  1672,  1710,
    1225,   519,   359,   709,   710,   711,   712,  1230,  1631,   732,
    1658,  1219,  1632,  1633,   835,   862,   863,   961,   520,  1514,
    1530,  1864,  1032,   709,   710,   711,   712,   807,  1802,  1486,
     747,  1838,   709,   710,   711,   712,  1338,  -674,  1720,  1511,
     219,    76,  1720,   212,   213,  1711,   137,   241,  1660,  1626,
    1720,  1500,   295,  1818,  1880,  1011,  1712,   506,   162,   707,
    1802,   531,   135,   221,   708,   512,   707,   808,  1339,  1870,
     287,   708,   847,   709,   710,   711,   712,   707,   163,  1720,
     945,   242,   708,   709,   710,   711,   712,   287,   835,  1240,
     997,  1033,   918,   916,   152,   707,   709,   710,   711,   712,
     708,  1198,  1241,   225,    76,   707,   709,   710,   711,   712,
     708,  1256,    77,  1180,   754,    76,  1720,   210,   473,   707,
     473,  1720,   969,   289,   708,  1242,   136,  1676,   949,   707,
     709,   710,   711,   712,   708,  1199,   221,  1257,   848,  1546,
    1893,   221,   295,   221,   707,   221,   292,  1735,   487,   708,
     287,  1736,   289,   487,  1249,  1713,  -232,  1352,   755,  1031,
     226,   250,   487,   707,   567,   204,  1745,   529,   708,    76,
     204,   100,   244,   219,  1009,    77,  1243,  1211,   246,   707,
    1214,  1062,   920,  1749,   708,   155,    77,   155,  1730,  1276,
    1332,   836,   473,  1265,   221,   737,   221,   707,   254,  1762,
     729,  1285,   708,  1320,  1629,  1250,  1806,   102,   295,  1072,
    1499,  1731,  1525,  1091,  1092,   221,  1094,  1398,  1058,  1061,
    1113,  1082,   738,  1746,  1533,  1306,   230,  1784,  1820,  1747,
     720,  1066,   721,   707,   724,  1267,   725,   524,   708,  1017,
    1268,   221,   221,  1269,  1623,   707,  1080,  1840,   922,  1661,
     708,  1270,  -232,  1763,  1512,   265,   654,  1803,   230,   230,
    1522,   230,  1271,  1473,   330,  1073,   267,  1732,  1797,   268,
    1568,   707,   103,  -232,   580,   273,   708,  1804,   531,  -232,
     352,   155,   104,   353,   581,  1379,   794,   707,   795,   713,
     287,   155,   708,  1240,  1202,   779,   707,  1637,   364,   654,
    1121,   708,   709,   710,   711,   712,  1241,   105,   652,   709,
     710,   711,   712,  -232,  -707,  1888,  1376,  1010,  1638,  1836,
     709,   710,   711,   712,  1639,   354,   707,  1377,   272,  1242,
    1767,   708,   365,  1096,   366,  1215,  1480,  1807,   709,   710,
     711,   712,   707,  1107,  1871,  1380,  1481,   708,   709,   710,
     711,   712,  1889,   144,   145,  1381,   367,  1706,  1640,  1895,
    1216,  1014,   709,   710,   711,   712,  1213,  1068,  1069,  1383,
    1859,   707,   709,   710,   711,   712,   708,  1015,  1308,  1384,
    1243,   368,  1011,  1309,   529,   274,  1909,   709,   710,   711,
     712,   487,   204,   209,   487,   238,   133,   303,   707,   816,
    1581,   817,   487,   708,   784,  1860,   709,   710,   711,   712,
     389,  1781,  1782,  1385,   487,   487,  1308,  1861,   394,   204,
     280,  1312,   709,   710,   711,   712,   477,  1212,   281,  1386,
     304,   713,   286,   946,  1507,  1449,   282,  1452,  1236,   707,
     709,   710,   711,   712,   708,   707,   986,  1387,   987,   221,
     708,   302,  1315,   707,   622,   626,   305,   707,   708,   306,
     145,    99,   708,   318,   707,  1529,  1666,  1667,  1333,   708,
    1356,  1848,  1782,  -694,  1337,   307,   709,   710,   711,   712,
    1100,  1579,  1101,   752,  1555,  1188,   328,  1780,   709,   710,
     711,   712,  1297,   332,  1697,  1388,  1787,   230,   219,   100,
     531,   308,  1571,  1907,  1908,   477,  1298,   873,   361,  1108,
    1297,   101,   350,   230,   709,   710,   711,   712,  1768,   230,
     334,   221,  1457,  1457,  1460,   713,   363,  1203,   221,  1725,
     960,  1204,   335,  1205,  -694,   102,  1100,  -694,  1253,   709,
     710,   711,   712,  1765,  -694,   911,  -694,   915,   372,   707,
    -694,   919,   921,   923,   708,  -694,   382,  1483,  -694,  1485,
    1299,  1722,  1300,   384,  1304,   336,  1305,   337, -1018,   709,
     710,   711,   712,  -694, -1018,   713,  1569,  1463,  1755, -1018,
     386,   713, -1018,  1548,   387,   709,   710,   711,   712,   749,
     390,  1402,  1570,   391,   713,  -694,  1578,   474,   482,  -694,
     103,   713,   473,  1618,   489,   219,   529,   707,  -694,  1855,
     104,  1856,   708,   490,   709,   710,   711,   712, -1018,   493,
    1528,  1577,   494,   496,  -694,   221, -1018,   497,  1407,  1448,
     410,   411,   412,   498, -1018,   105,  1240,   814,   487,   500,
     487,   709,   710,   711,   712,  1782,   513,  1890,  1761,  1241,
     515,  -694, -1018,   516, -1018,  1900,   487,   525,   420,   421,
     204,    14,   540,  -694,   541,  -694,  1850, -1018,   535, -1018,
   -1018, -1018,  1242,   543,   550,   552,  1668,   554,   551,   575,
     585,   895,   709,   710,   711,   712,   572,   707,   709,   710,
     711,   712,   708,   589,  1728,   896,   709,   710,   711,   712,
     709,   710,   711,   712,   440,   638,   644,   709,   710,   711,
     712,  1537,   681,   688,   642,   531,  1542,   678, -1018,  1842,
     689,  1722,   679,  1243,   680,  1722,   682,   713,   503,  1510,
     683,   897,  -694,  1722,   684,   685,  1819,   511,  1240,   898,
     686,  -981,    62,  -694,  -694,  1134,  -694,   899,  1135,   687,
     690,  1241,   395,    63,   719,   695,   696,   715,   697,   396,
      64,   714,  1722,   706,  -998,   900,  1619,   901,   539,   723,
     722,   397,   542,    65,  1242,   727,   741,   398,   399,  1585,
     902,   744,   903,   904,   905,   749,  1810,   768,   732,   751,
    1812,   764,   709,   710,   711,   712,  1887,   772,  1816,  1722,
    1748,   773,   775,   776,  1722,   400,   777,   780,   401,   781,
     783,   545,   787,   786, -1018, -1018, -1018, -1018, -1018, -1018,
   -1018,   529,   789,   790,   796,  1243, -1018,  1846, -1018,   198,
      66,   906,   820,   402,   821,   403,   404,  1595,  1121,  1121,
     822,  -441,   827,   832,  1448,   405,    67,   287,   833,   406,
     709,   710,   711,   712,   924,   925,   926,   927,   928,   929,
     930,   931,    68,   839,  1873,   840,   841,   692,   842,  1875,
     844,   845,   698,   699,   702,   850,   854,    69,   855,    52,
     860,    70,   861,   870,    71,   487,   872,   873,   878,   879,
      53,   880,    72,   204,   884,   320,   938,    54,   944,  1723,
     851,   289,  1136,   965,   407,   981,   984,   989,  1727,  1005,
     408,    73,  1457,   982,   993,   994,  1012,  1021,   155,  1025,
     409,    55,  1028,  1047,  1030,  1038,  1048,   907,   908,   909,
     709,   710,   711,   712,   410,   411,   412,   413,   212,   213,
    1049,   799,   746,  1054,  1055,  1145,   748,  1149,   414,  1157,
    1160,  1057,  1165,  1168,  1171,  1063,  1064,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   107,   424,  1065,   109,
    1643,  1071,   425,   426,  1743,   427,   428,   429,   112,   113,
     114,   430,   115,    56,  1084,   116,   117,   118,   119,   120,
    1078,  1089,  1098,   121,   122,  1103,  1093,  1115,  1139,   123,
     431,   125,   737,  1179,   707,   432,   433,   204,  1177,  1187,
    1206,   502,  1208,   435,    57,  1217,  1223,   289,    58,  1121,
    1226,  1744,  1219,  1235,  1239,  1730,  1245,  1247,  1258,  1261,
    1281,  1293,  1317,  1301,  1283,  1109,  1303,   320,  1310,  1313,
    1314,  1326,  1704,  1316,  1718,  1757,  1328,  1335,  1731,  1759,
    1340,  1341,   893,  1353,  1342,   949,  1348,  1349,  1354,  1723,
     155,  1355,  1392,  1723,  1357,   201,  1358,  1365,   531,  1366,
    1367,  1723,  1792,  1394,  1723,  1368,  1370,  1371,   154,  1372,
    1373,  1374,  1772,  1375,   155,  1451,  1466,  1464,  1469,  1470,
    1467,  1471,  1472,  1475,   724,  1484,  1476,  1497,  1501,  1502,
    1723,  1503,   892,   531,  1732,  1504,  1509,  1505,  1506,  1531,
    1524,  1075,  1741,  1535,  1575,  1576,  1583,  1536,  1541,   912,
    1582,  1350,  -987,   917,   136,  1588,  1547,  1567,  1589,  1591,
     933,  1607,   198,  1774,  1592,   940,   941,  1723,   106,  1599,
    1606,  1608,  1723,  1775,  1609,   531,   440,  1610,   950,   951,
     952,   953,   954,   955,   440,  1611,  1612,  1613,  1614,   752,
    1624,  1634,  1644,  1649,  1647,   154,   531,   107,   108,   531,
     109,   155,  1646,  1648,   529,  1650,  1651,   110,   111,   112,
     113,   114,  1653,   115,   531,  1654,   116,   117,   118,   119,
     120,   511,  1655,  1663,   121,   122,  1665,  1448,  1674,  1675,
     123,   124,   125,  1669,   221,  1827,  1009,  1673,  1677,   529,
    1678,   155,  1019,  1701,  1708,  1600,   154,   539,  1679,  1027,
    1144,  1680,   155,  1135,  1683,  1729,  1684,   395,  1685,  1739,
    1686,  1687,  1448,  1740,   396,  1758,  1688,  1771,  1883,  1770,
    1689,  1690,  1827,  1691,  1692,  1694,   397,  1719,   155,  1778,
    1737,   529,   398,   399,   154,  1779,  1783,  1764,  1785,  1789,
     155,  1790,  1793,  1808,  1811,  1801,  1832,  1834,  1839,  1847,
    1837,  1782,   529,  1827,  1448,   529,  1009,  1841,  1844,   155,
     400,  1853,   201,   401,  1858,  1857,  1829,  1830,   164,  1874,
     529,  1876,  1877,  1903,  1884,  1448,  1882,  1121,  1448,  1898,
    1904,  1910,  1905,   201,  1912,  1913,  1454,   481,   402,   142,
     403,   404,   488,  1448,   983,   871,   799,   499,  1336,  1114,
     405,  1334,  1119,  1322,   406,   201,   201,   555,  1657,  1260,
     834,  1323,   533,  1022,  1330,  1132,  1133,  1138,  1656,  1699,
    1146,  1147,  1150,   568,  1158,  1161,  1163,  1166,  1169,  1172,
    1173,  1174,  1636,  1176,  1052,  1178,  1642,  1828,  1181,  1182,
    1183,  1184,  1185,  1186,  1544,  1545,   570,  1869,  1835,  1833,
    1549,  1550,   597,  1552,  1553,  1190,  1210,  1136,  1193,   407,
    1045,  1557,  1558,  1559,  1560,   408,  1561,  1562,  1563,  1564,
    1565,  1566,  1766,  1716,  1814,   409,  1254,   962,  1800,  1627,
    1479,   562,   819,   576,   553,  1090,  1521,   858,   837,   410,
     411,   412,   413,   212,   213,  1081,   143,   763,   526,  1013,
     759,   300,  1024,   414,  1007,  1238,  1788,  1733,  1881,   440,
    1863,    44,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   107,   424,    45,   109,   771,  1488,   425,   426,    48,
     427,   428,   429,   112,   113,   114,   430,   115,   990,  1396,
     116,   117,   118,   119,   120,  1231,  1534,  1074,   121,   122,
    1361,  1311,  1738,   255,   123,   431,   125,  1594,     0,   815,
     432,   433,     0,     0,     0,     0,   502,   106,   435,     0,
       0,     0,   289,     0,    84,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,    86,    87,     0,  1148,     0,
       0,  1135,     0,     0,    88,   395,   107,   108,     0,   109,
       0,     0,   396,     0,     0,    89,   110,   111,   112,   113,
     114,     0,   115,     0,   397,   116,   117,   118,   119,   120,
     398,   399,     0,   121,   122,     0,     0,     0,     0,   123,
     124,   125,     0,     0,     0,     0,     0,     0,     0, -1019,
       0,   268,     0,     0,     0, -1019,     0,     0,   400,     0,
   -1019,   401,     0, -1019,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,  1359,  1360,     0,  1362,  1364,   402,     0,   403,   404,
       0,     0,  1369,    91,     0,     0,     0,     0,   405, -1019,
       0,     0,   406,     0,     0,     0,     0, -1019,    92,  1382,
       0,     0,    93,     0,     0, -1019,     0,  1389,  1390,  1190,
       0,     0,     0,    94,     0,  1190,     0,  1193,  1397,   440,
    1399,  1400,     0, -1019,     0, -1019,     0,     0,     0,     0,
       0,     0,    95,     0,   567,     0,     0,     0, -1019,     0,
   -1019, -1019, -1019,     0,     0,  1136,     0,   407,     0,   511,
       0,     0,     0,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0, -1019,
       0,   414,     0,     0,     0,     0,  1492,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   107,
     424,     0,   109,     0,     0,   425,   426,     0,   427,   428,
     429,   112,   113,   114,   430,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,   431,   125,   511,     0,     0,   432,   433,
       0,     0,     0,     0,   502,     0,   435,     0,     0,     0,
     289,     0,     0,     0,     0,     0,     0,  1543,     0,     0,
       0,     0,     0,     0,     0,     0,  1551,     0,     0,     0,
       0,     0,     0,  1556,     0, -1019, -1019, -1019, -1019, -1019,
   -1019, -1019,     0,     0,     0,     0,     0, -1019,     0, -1019,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1190,  1190,     0,     0,     0,     0,     0,
       0,     0,     0,  1156,     0,     0,  1135,     0,     0,     0,
     395,   107,   108,  1590,   109,     0,     0,   396,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,   397,
     116,   117,   118,   119,   120,   398,   399,     0,   121,   122,
       0,     0,     0,     0,   123,   124,   125,     0, -1020,     0,
       0,     0,     0,   478, -1020,     0,     0,     0,     0, -1020,
       0,     0, -1020,   400,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,  1622,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,   403,   404,     0,     0,     0, -1020,     0,
       0,     0,     0,   405,     0,     0, -1020,   406,   107,   108,
       0,   109,     0,     0, -1020,     0,     0,     0,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120, -1020,     0, -1020,   121,   122,     0,     0,     0,
       0,   123,   124,   125,     0,     0,     0, -1020,     0, -1020,
   -1020, -1020,     0,     0,  1681,  1682,     0,     0,     0,     0,
    1136,     0,   407,   573,     0,     0,     0,     0,   408,     0,
       0,  1693,     0,     0,     0,     0,  1190,     0,   409,  1696,
       0,     0,     0,     0,     0,  1703,     0,  1705,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213, -1020,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,  1159,     0,     0,  1135,   123,   431,   125,
     395,     0,     0,   432,   433,     0,     0,   396,  1756,   502,
       0,   435,     0,     0,     0,   289,  1760,     0,     0,   397,
       0,     0,     0,     0,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0, -1020, -1020, -1020, -1020, -1020, -1020,
   -1020,     0,     0,     0,   440,     0, -1020,     0, -1020,     0,
       0,     0,     0,   400,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1622,
       0,   106,     0,     0,     0,     0,     0,     0,     0,   958,
       0,   402,     0,   403,   404,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
     107,   108,     0,   109,     0,     0,     0,     0,     0,     0,
     110,   111,   112,   113,   114,     0,   115,    99,  1815,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,     0,
       0,     0,     0,   123,   124,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1136,     0,   407,     0,     0,   100,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   101,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   138,   410,   411,   412,   413,   212,   213,     0,     0,
       0,   102,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,     0,     0,   116,   117,   118,   119,   120,   106,     0,
       0,   121,   122,  1164,     0,     0,  1135,   123,   431,   125,
     395,     0,     0,   432,   433,     0,   103,   396,     0,   502,
       0,   435,     0,     0,     0,   289,   104,   107,   108,   397,
     109,     0,     0,     0,     0,   398,   399,   110,   111,   112,
     113,   114,     0,   115,     0,     0,   116,   117,   118,   119,
     120,   105,     0,     0,   121,   122,     0,     0,     0,     0,
     123,   124,   125,   400,     0,     0,   401,     0,   959,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,   403,   404,     0,     0,     0,   107,   108,
       0,   109,     0,   405,     0,     0,     0,   406,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,    99,     0,     0,
       0,   123,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1136,     0,   407,     0,     0,   100,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,   101,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,   102,     0,     0,     0,    14,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,     0,     0,   116,   117,   118,   119,   120,   106,     0,
       0,   121,   122,  1167,     0,     0,  1135,   123,   431,   125,
     395,     0,     0,   432,   433,     0,   103,   396,     0,   502,
       0,   435,     0,     0,     0,   289,   104,   107,   108,   397,
     109,     0,     0,     0,     0,   398,   399,   110,   111,   112,
     113,   114,     0,   115,     0,     0,   116,   117,   118,   119,
     120,   105,     0,     0,   121,   122,     0,     0,     0,     0,
     123,   124,   125,   400,     0,     0,   401,     0,     0,   106,
       0,     0,   998,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,   403,   404,     0,     0,     0,   107,   108,
       0,   109,     0,   405,     0,     0,     0,   406,   110,   111,
     112,   113,   114,     0,   115,     0,     0,   116,   117,   118,
     119,   120,     0,     0,   106,   121,   122,     0,     0,     0,
       0,   123,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,   108,     0,   109,     0,     0,     0,
    1136,     0,   407,   110,   111,   112,   113,   114,   408,   115,
       0,     0,   116,   117,   118,   119,   120,     0,   409,     0,
     121,   122,     0,     0,     0,     0,   123,   124,   125,     0,
       0,     0,   410,   411,   412,   413,   212,   213,  1845,     0,
     435,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,  1170,     0,     0,  1135,   123,   431,   125,
     395,     0,     0,   432,   433,     0,     0,   396,     0,   502,
       0,   435,     0,     0,     0,   289,     0,     0,     0,   397,
       0,   895,     0,     0,     0,   398,   399,   707,     0,     0,
       0,     0,   708,     0,     0,   896,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   897,     0,     0,     0,     0,     0,     0,     0,   898,
       0,   402,     0,   403,   404,     0,     0,   899,     0,     0,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
       0,     0,     0,     0,     0,   900,     0,   901,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     902,     0,   903,   904,   905,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1136,     0,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,     0,
       0,   906,     0,   970,     0,     0,     0,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,   395,     0,   116,   117,   118,   119,   120,   396,     0,
       0,   121,   122,   971,     0,     0,     0,   123,   431,   125,
     397,     0,     0,   432,   433,     0,   398,   399,     0,   502,
       0,   435,     0,     0,     0,   289,     0,   907,   908,   909,
     709,   710,   711,   712,     0,   656,     0,   945,   972,     0,
       0,     0,     0,   973,   400,     0,     0,   401,     0,   974,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,   975,     0,
       0,     0,   402,     0,   403,   404,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,   406,   107,
     108,     0,   109,     0,     0,     0,   885,     0,     0,   110,
     111,   112,   113,   114,     0,   115,     0,     0,   116,   117,
     118,   119,   120,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   123,   124,   125,     0,     0,     0,     0,     0,
    1208,     0,     0,   700,     0,     0,     0,   658,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,   408,
       0,     0,     0,     0,     0,     0,     0,     0,     6,   409,
       0,     0,     0,     0,     0,  -287,     0,     0,     0,  1408,
    1409,     0,  1410,   410,   411,   412,   413,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,   414,  1411,     0,
       0,     0,     0,     0,  1412,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   107,   424,     0,   109,     0,
       0,   425,   426,     0,   427,   428,   429,   112,   113,   114,
     430,   115,     0,     0,   116,   117,   118,   119,   120,   886,
     887,   888,   121,   122,     0,   395,     0,     0,   123,   431,
     125,     0,   396,     0,   432,   433,     0,     0,     0,     0,
     662,  1413,   435,     0,   397,     0,   289,   655,     0,     0,
     398,   399,     0,     0,     1,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   656,
       0,     0,     2,     0,  1414,     0,     0,     0,   400,     0,
       3,   401,     0,     0,    16,     0,     0,     4,     0,     5,
       0,     6,     0,  1600,     0,     0,     7,     0,     0,     0,
       0,     0,   106,     8,     0,     0,   402,     0,   403,   404,
       0,     0,     0,     0,     0,     0,     0,     9,   405,     0,
       0,     0,   406,     0,     0,     0,     0,    10,     0,     0,
     657,  1415,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,  1416,   118,   119,   120,     0,     0,     0,   121,   122,
    1417,  1418,  1419,     0,   123,   124,   125,     0,     0,     0,
       0,   658,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,   970,     0,
      11,     0,     0,   409,     0,     0,     0,    12,     0,     0,
      13,     0,    14,    15,     0,     0,     0,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,    16,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   107,
     424,     0,   109,     0,     0,   425,   426,     0,   427,   428,
     429,   112,   113,   114,   430,   115,   395,     0,   116,   117,
     659,   660,   661,   396,     0,     0,   121,   122,     0,     0,
       0,     0,   123,   431,   125,   397,    17,     0,   432,   433,
       0,   398,   399,     0,   662,     0,   435,     0,     0,     0,
     289,    18,    19,     0,     0,     0,     0,     0,   973,     0,
     656,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,   401,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,   403,
     404,     0,     0,     0,   107,   108,     0,   109,     0,   405,
       0,     0,     0,   406,   110,   111,   112,   113,   114,     0,
     115,   881,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   658,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,   408,     0,     0,     0,     6,     0,
       0,     0,     0,     0,   409,  -289,     0,     0,     0,  1408,
    1409,     0,  1410,     0,     0,     0,     0,     0,   410,   411,
     412,   413,   212,   213,     0,     0,     0,     0,  1411,     0,
       0,     0,   414,     0,  1412,     0,     0,     0,     0,     0,
       0,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     107,   424,     0,   109,     0,     0,   425,   426,     0,   427,
     428,   429,   112,   113,   114,   430,   115,   395,     0,   116,
     117,   659,   660,   661,   396,     0,     0,   121,   122,     0,
       0,     0,     0,   123,   431,   125,   397,     0,     0,   432,
     433,  1413,   398,   399,     0,   662,     0,   435,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,    14,
       0,   656,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,   401,  1414,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
     403,   404,   106,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,   406,     0,     0,     0,     0,     0,
       0,     0,   657,     0,     0,     0,     0,     0,     0,     0,
       0,  1415,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,     0,   115,     0,     0,
     116,  1416,   118,   119,   120,     0,     0,     0,   121,   122,
    1417,  1418,  1419,   658,   123,   124,   125,     0,     0,   407,
       0,     0,     0,     0,     0,   408,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   410,
     411,   412,   413,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   107,   424,     0,   109,     0,     0,   425,   426,     0,
     427,   428,   429,   112,   113,   114,   430,   115,   395,     0,
     116,   117,   659,   660,   661,   396,     0,     0,   121,   122,
       0,     0,     0,     0,   123,   431,   125,   397,     0,     0,
     432,   433,     0,   398,   399,     0,   662,     0,   435,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
     483,     0,   656,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,   401,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,   403,   404,     0,     0,     0,   107,   108,     0,   109,
       0,   405,     0,     0,     0,   406,   110,   111,   112,   113,
     114,     0,   115,   881,     0,   116,   117,   118,   119,   120,
       0,     0,     0,   121,   122,     0,     0,     0,     0,   123,
     124,   125,     0,     0,     0,     0,     0,     0,     0,  1208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   658,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,   408,     6,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,  1408,  1409,
       0,  1410,     0,     0,     0,     0,     0,     0,     0,     0,
     410,   411,   412,   413,   212,   213,     0,  1411,     0,     0,
       0,     0,     0,  1412,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   107,   424,     0,   109,     0,     0,   425,   426,
       0,   427,   428,   429,   112,   113,   114,   430,   115,   395,
       0,   116,   117,   118,   119,   120,   396,     0,     0,   121,
     122,     0,     0,     0,     0,   123,   431,   125,   397,     0,
    1413,   432,   433,     0,   398,   399,     0,   662,     0,   435,
       0,     0,     0,   289,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,  1414,     0,   401,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   106,   403,   404,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1415,   108,     0,   109,     0,     0,     0,     0,     0,     0,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
    1416,   118,   119,   120,     0,     0,     0,   121,   122,  1417,
    1418,  1419,     0,   123,   124,   125,     0,     0,     0,     0,
       0,   407,     0,     0,     0,     0,     0,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   409,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,   410,   411,   412,   413,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,   400,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   107,   424,     0,   109,     0,     0,   425,
     426,     0,   427,   428,   429,   112,   113,   114,   430,   115,
     395,   402,   116,   117,   118,   119,   120,   396,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   431,   125,   397,
       0,     0,   432,   433,   507,   398,   399,     0,   502,     0,
     435,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,   403,   404,     0,     0,     0,   409,     0,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
       0,     0,   410,   411,   412,     0,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,     0,     0,     0,     0,
     425,   426,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,     0,
       0,     0,     0,     0,  1263,     0,   691,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,   395,     0,   116,   117,   118,   119,   120,   396,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   431,   125,
     397,     0,     0,   432,   433,     0,   398,   399,     0,   502,
       0,   435,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   874,     0,   400,   599,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   875,     0,
       0,     0,   600,     0,     0,   601,     0,     0,     0,     0,
       0,     0,   402,     0,   403,   404,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,   406,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   602,     0,     0,     0,     0,
       0,   603,   604,   605,     0,   606,   607,   608,     0,   609,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   700,     0,     0,     0,   610,     0,   611,
       0,     0,     0,   407,     0,     0,     0,     0,     0,   408,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   409,
       0,     0,     0,     0,     0,     0,     0,     0,   612,     0,
       0,     0,     0,   410,   411,   412,   413,   212,   213,     0,
       0,     0,     0,     0,     0,   613,     0,   414,     0,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   107,   424,   614,   109,     0,
       0,   425,   426,     0,   427,   428,   429,   112,   113,   114,
     430,   115,   395,     0,   116,   117,   118,   119,   120,   396,
       0,     0,   121,   122,     0,     0,     0,     0,   123,   431,
     125,   397,   615,   616,   432,   433,     0,   398,   399,     0,
     502,     0,   435,     0,     0,     0,   289,     0,     0,   617,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   598,     0,   400,   599,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   601,     0,     0,     0,
       0,     0,     0,   402,     0,   403,   404,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,   406,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   602,     0,     0,     0,
       0,     0,   603,   604,   605,     0,   606,   607,   608,     0,
     609,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   610,     0,
     611,     0,     0,     0,   407,     0,     0,     0,     0,     0,
     408,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,   612,
       0,     0,     0,     0,   410,   411,   412,   413,   212,   213,
       0,     0,     0,     0,     0,     0,   613,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   107,   424,   614,   109,
       0,     0,   425,   426,     0,   427,   428,   429,   112,   113,
     114,   430,   115,   395,     0,   116,   117,   118,   119,   120,
     396,     0,   106,   121,   122,     0,     0,     0,     0,   123,
     431,   125,   397,   615,   616,   432,   433,     0,   398,   399,
       0,   502,   956,   435,     0,     0,     0,   289,     0,     0,
     617,   107,   108,     0,   109,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   400,   115,     0,   401,
     116,   117,   118,   119,   120,   106,     0,     0,   121,   122,
       0,     0,     0,     0,   123,   124,   125,     0,     0,     0,
       0,     0,     0,     0,   402,     0,   403,   404,   435,     0,
       0,     0,     0,     0,   107,   108,   405,   109,     0,     0,
     406,   106,     0,     0,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125,
     107,   108,     0,   109,     0,     0,     0,     0,     0,   998,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
     117,   118,   119,   120,     0,   407,     0,   121,   122,     0,
       0,   408,     0,   123,   124,   125,     0,     0,     0,     0,
       0,   409,     0,     0,     0,  1750,     0,     0,     0,     0,
       0,     0,  1162,     0,     0,   410,   411,   412,   413,   212,
     213,     0,     0,     0,     0,     0,     0,     0,     0,   414,
       0,     0,     0,     0,     0,     0,     0,     0,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   107,   424,     0,
     109,     0,     0,   425,   426,     0,   427,   428,   429,   112,
     113,   114,   430,   115,   395,     0,   116,   117,   118,   119,
     120,   396,     0,     0,   121,   122,     0,     0,     0,     0,
     123,   431,   125,   397,     0,     0,   432,   433,     0,   398,
     399,     0,   502,     0,   435,     0,     0,     0,   289,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
     401,     0,     0,   106,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,   403,   404,     0,
       0,     0,   107,   108,     0,   109,     0,   405,     0,     0,
       0,   406,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,   106,   121,
     122,     0,     0,     0,     0,   123,   124,   125,     0,     0,
       0,     0,     0,     0,     0,  1584,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1350,   107,   108,     0,
     109,     0,     0,     0,     0,     0,   407,   110,   111,   112,
     113,   114,   408,   115,     0,     0,   116,   117,   118,   119,
     120,     0,   409,     0,   121,   122,     0,     0,     0,     0,
     123,   124,   125,     0,     0,     0,   410,   411,   412,   413,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   107,   424,
       0,   109,     0,     0,   425,   426,     0,   427,   428,   429,
     112,   113,   114,   430,   115,   395,     0,   116,   117,   118,
     119,   120,   396,     0,     0,   121,   122,     0,     0,     0,
       0,   123,   431,   125,   397,     0,     0,   432,   433,     0,
     398,   399,     0,   502,     0,   435,     0,     0,     0,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,   401,     0,     0,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,   403,   404,
       0,     0,     0,   107,   108,     0,   109,     0,   405,     0,
       0,     0,   406,   110,   111,   112,   113,   114,     0,   115,
       0,     0,   116,   117,   118,   119,   120,     0,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   124,   125,     0,
       0,     0,     0,     0,     0,     0,  1593,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1363,     0,     0,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   107,
     424,     0,   109,     0,     0,   425,   426,     0,   427,   428,
     429,   112,   113,   114,   430,   115,   395,     0,   116,   117,
     118,   119,   120,   396,     0,     0,   121,   122,     0,     0,
       0,     0,   123,   431,   125,   397,     0,     0,   432,   433,
       0,   398,   399,     0,   502,     0,   435,     0,     0,     0,
     289,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,   401,    76,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,   403,
     404,     0,     0,     0,   107,   108,     0,   109,     0,   405,
       0,     0,     0,   406,   110,   111,   112,   113,   114,     0,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,     0,     0,     0,     0,   123,   124,   125,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,   408,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   409,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   410,   411,
     412,   413,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     107,   424,     0,   109,     0,     0,   425,   426,     0,   427,
     428,   429,   112,   113,   114,   430,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,   395,
       0,     0,     0,   123,   431,   125,   396,     0,   106,   432,
     433,     0,     0,  1702,     0,   502,     0,   435,   397,     0,
       0,   289,     0,     0,   398,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,   108,     0,
     109,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   400,   115,     0,   401,   116,   117,   118,   119,
     120,     0,     0,     0,   121,   122,     0,     0,     0,     0,
     123,   124,   125,     0,     0,     0,     0,     0,   959,     0,
     402,     0,   403,   404,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,     0,     0,     0,     0,   408,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   409,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   410,   411,   412,   413,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   107,   424,     0,   109,     0,     0,   425,
     426,     0,   427,   428,   429,   112,   113,   114,   430,   115,
     395,     0,   116,   117,   118,   119,   120,   396,     0,     0,
     121,   122,     0,     0,     0,     0,   123,   431,   125,   397,
       0,     0,   432,   433,     0,   398,   399,     0,   434,     0,
     435,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,   403,   404,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   107,   424,     0,   109,     0,     0,
     425,   426,     0,   427,   428,   429,   112,   113,   114,   430,
     115,     0,     0,   116,   117,   118,   119,   120,     0,     0,
       0,   121,   122,   598,     0,     0,   599,   123,   431,   125,
       0,     0,   799,   432,   433,     0,     0,     0,     0,   502,
       0,   435,     0,   600,     0,   289,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   598,     0,     0,   599,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   601,     0,   602,     0,     0,     0,
       0,     0,   603,   604,   605,     0,   606,   607,   608,     0,
     609,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   610,     0,
     611,     0,     0,     0,   602,     0,     0,     0,     0,     0,
     603,   604,   605,     0,   606,   607,   608,     0,   609,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   612,
       0,     0,     0,     0,     0,     0,   610,     0,   611,     0,
       0,     0,     0,     0,     0,     0,   613,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   612,   614,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,   613,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   615,   616,     0,   614,     0,     0,     0,
       0,     0,   107,   108,     0,   109,     0,     0,     0,     0,
     617,   106,   110,   111,   112,   113,   114,     0,   115,     0,
       0,   116,   117,   118,   119,   120,     0,     0,     0,   121,
     122,   615,   616,     0,     0,   123,   124,   125,     0,     0,
     107,   108,     0,   109,     0,     0,     0,     0,   617,     0,
     110,   111,   112,   113,   114,     0,   115,     0,     0,   116,
     117,   118,   119,   120,     0,     0,     0,   121,   122,   106,
       0,     0,     0,   123,   124,   125,     0,     0,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,     0,     0,   184,   185,   107,   108,
       0,   109,     0,     0,   186,   187,     0,     0,   110,   111,
     112,   113,   114,   188,   115,   189,     0,   116,   117,   118,
     119,   120,     0,     0,     0,   121,   122,   190,   191,   192,
     193,   123,   124,   125
};

static const short yycheck[] =
{
       9,    16,    55,    12,     4,    64,   105,    16,    62,    18,
       0,   527,   144,   145,   144,   145,   154,    63,   373,   373,
      91,   369,   434,   880,   344,    94,    85,   884,    73,   691,
     569,    63,   715,   513,    70,   697,    89,   469,   539,   351,
     365,  1207,    68,  1280,    53,    54,    55,    56,    57,    58,
      95,   741,   275,   342,    63,    64,    65,    93,    67,   700,
      69,    70,   285,    72,    73,    91,   378,   490,    83,    88,
     650,   722,    81,   291,    83,    84,    85,    92,    87,    88,
      89,    90,    56,    92,    93,    94,    95,   318,    97,   158,
     773,   998,   868,    67,   943,  1102,   737,  1423,   162,   350,
      57,  1046,   643,   135,  1039,   137,  1079,   966,  1225,   228,
     808,     8,    69,     8,  1049,  1400,    90,  1529,     8,    21,
     741,    10,   496,    97,    43,   657,   135,   136,   137,   561,
     149,   563,    61,     8,  1043,   281,    19,   569,   237,   988,
     149,  1050,  1278,   197,    47,   154,  1039,   156,   157,   158,
    1059,  1039,  1097,   162,  1039,     6,  1049,  1189,    90,    26,
       8,  1049,  1039,  1195,  1049,  1217,     8,  1219,   222,    47,
       8,     8,  1049,  1039,  1049,    97,  1085,  1462,     8,   136,
     318,   185,   156,  1049,    43,   141,     8,    12,  1246,     8,
     149,    44,   174,     8,    63,    46,    80,   196,     3,   547,
     157,    23,  1806,   322,   190,   117,    28,  1843,    23,    44,
      43,    43,   126,    28,   569,   569,  1820,    61,  1854,   143,
     532,    11,   274,    94,   196,   277,    30,   326,   125,     6,
     125,   893,   894,   265,   750,   125,   268,    23,   162,    43,
      12,   776,    28,    59,    60,    80,   594,   118,   657,   302,
     125,  1417,  1418,   662,  1103,    45,   265,    90,   793,   268,
     483,   796,    94,   272,   178,  1901,   275,   276,   272,    46,
     805,    76,   281,   272,   186,   937,   285,   125,    44,   278,
      50,    51,   571,   125,  1888,   271,   318,   125,   125,   271,
     246,   981,   141,   302,   706,   125,   180,   306,   173,   271,
      50,    51,   714,   993,   994,    17,   125,  1252,   230,     4,
     177,   343,   366,   272,   368,   272,   141,    10,   350,    31,
     352,   204,   354,   255,   281,   357,   861,   180,   382,   618,
      23,   173,  1239,   386,   343,    28,  1748,  1272,  1273,  1312,
     376,   350,   271,   352,   263,   354,    10,   191,   357,   306,
      23,   497,   490,   885,    49,    28,  1054,  1055,  1494,    23,
     981,  1393,  1394,    75,    28,   845,   375,   376,   271,   141,
     744,  1306,   993,   275,   271,   384,  1038,   386,   273,  1272,
    1273,   390,   271,   273,  1272,  1273,  1295,  1272,  1273,   821,
     390,   103,   133,   271,    44,  1272,  1273,  1272,  1273,    63,
      23,   375,   271,  1640,   263,    28,  1272,  1273,  1466,  1107,
      23,   260,  1464,  1306,   180,    28,  1275,   268,  1306,   189,
      26,  1306,   255,    23,     8,   273,   435,   384,    28,  1306,
      80,  1306,  1598,    23,    14,   273,   273,   501,    28,   189,
    1306,   196,   117,   273,   266,   267,   268,   269,  1607,    68,
    1776,   266,   267,   268,   269,   167,   586,    23,   490,    63,
    1319,   156,    28,  1125,  1126,   141,  1128,  1129,    56,   478,
       6,    19,   881,   482,   483,  1760,   885,   193,     6,   174,
     489,   156,   268,   269,   493,  1811,  1021,   496,   497,   141,
     199,   200,   501,    22,  1229,   549,   172,  1614,   787,     3,
     192,   790,   192,  1740,    90,    63,  1041,  1042,   482,   732,
      46,   187,  1752,   208,   803,   634,   587,   123,    46,  1526,
      62,   478,   496,   278,   176,   105,  1061,  1853,   203,   747,
     180,   272,   489,  1113,  1226,   589,   493,   569,   769,   828,
     497,   125,  1318,   794,  1576,  1235,    94,   859,  1874,   168,
    1750,  1877,   872,  1204,  1785,  1096,   175,  1794,   567,  1799,
     569,   587,  1507,   986,   573,  1472,  1892,   155,  1541,     9,
     174,   177,    76,   266,   267,   268,   269,   989,  1487,   271,
    1525,   271,  1491,  1492,   169,   639,   640,   719,   194,   118,
    1325,  1828,    98,   266,   267,   268,   269,   139,  1838,  1261,
     273,  1801,   266,   267,   268,   269,   230,   271,  1767,  1301,
     642,   196,  1771,   199,   200,    55,    18,    11,  1527,  1478,
    1779,  1283,     6,  1782,  1855,   755,    66,   292,   244,    23,
    1870,   769,    18,   642,    28,   300,    23,   179,   262,  1839,
     196,    28,    21,   266,   267,   268,   269,    23,   270,  1808,
     273,    45,    28,   266,   267,   268,   269,   196,   169,    64,
     273,   167,    46,   682,    16,    23,   266,   267,   268,   269,
      28,   271,    77,   682,   196,    23,   266,   267,   268,   269,
      28,  1028,   267,   273,   223,   196,  1845,   147,   720,    23,
     722,  1850,   724,   278,    28,   100,    82,  1546,   707,    23,
     266,   267,   268,   269,    28,   271,   715,  1032,    87,  1350,
    1876,   720,     6,   722,    23,   724,   272,  1626,   727,    28,
     196,  1630,   278,   732,   143,   165,    22,  1139,   267,   783,
       7,    83,   741,    23,    30,   744,  1645,   769,    28,   196,
     749,    44,   196,   775,   753,   267,   151,   970,    65,    23,
     973,   822,    46,  1662,    28,   764,   267,   766,    85,  1048,
    1107,   272,   794,  1698,   773,   155,   775,    23,    65,  1697,
     744,  1060,    28,  1098,  1483,   194,  1762,    80,     6,   833,
    1281,   108,  1317,   854,   855,   794,   857,  1199,   820,   821,
    1110,   267,   182,  1652,     6,  1084,   822,  1725,  1784,  1658,
     271,   827,   273,    23,   271,  1698,   273,   764,    28,   766,
    1698,   820,   821,  1698,  1476,    23,   842,  1803,    46,  1528,
      28,  1698,   118,  1698,  1304,   272,   835,  1755,   854,   855,
    1310,   857,  1698,  1245,    46,   835,   140,   164,  1747,   272,
    1379,    23,   145,   139,     5,   271,    28,  1756,   986,   145,
       4,   860,   155,     7,    15,  1175,   271,    23,   273,   271,
     196,   870,    28,    64,   276,   174,    23,   118,    98,   878,
     879,    28,   266,   267,   268,   269,    77,   180,   878,   266,
     267,   268,   269,   179,    85,  1871,   273,   223,   139,  1798,
     266,   267,   268,   269,   145,    49,    23,   273,   122,   100,
      82,    28,   132,   860,   134,   974,   231,  1764,   266,   267,
     268,   269,    23,   870,  1842,   273,   241,    28,   266,   267,
     268,   269,  1872,   231,   232,   273,   156,  1589,   179,  1879,
     975,   142,   266,   267,   268,   269,   972,   130,   131,   273,
      79,    23,   266,   267,   268,   269,    28,   158,   271,   273,
     151,   181,  1082,   276,   986,   122,  1906,   266,   267,   268,
     269,   970,   971,   972,   973,   974,   975,    84,    23,   203,
    1402,   205,   981,    28,  1322,   114,   266,   267,   268,   269,
     246,   270,   271,   273,   993,   994,   271,   126,   254,   998,
     124,   276,   266,   267,   268,   269,  1028,   971,   122,   273,
     117,   271,   174,   273,  1293,  1217,   122,  1219,   998,    23,
     266,   267,   268,   269,    28,    23,   271,   273,   273,  1028,
      28,    79,  1093,    23,  1379,  1379,   143,    23,    28,   146,
     232,     6,    28,   156,    23,  1324,  1533,  1534,  1109,    28,
      54,   270,   271,     0,  1115,   162,   266,   267,   268,   269,
     271,   271,   273,    10,    54,    63,    19,  1719,   266,   267,
     268,   269,  1071,   259,  1580,   273,  1728,  1093,  1100,    44,
    1208,   188,    61,  1904,  1905,  1107,  1076,   271,   162,   273,
    1089,    56,   272,  1109,   266,   267,   268,   269,   270,  1115,
       4,  1100,  1224,  1225,  1226,   271,   199,   273,  1107,  1615,
    1783,   271,    16,   273,    61,    80,   271,    64,   273,   266,
     267,   268,   269,   270,    71,   678,    73,   680,    10,    23,
      77,   684,   685,   686,    28,    82,    59,  1257,    85,  1259,
     271,  1607,   273,    61,   271,    49,   273,    51,    17,   266,
     267,   268,   269,   100,    23,   271,   273,   273,  1664,    28,
      79,   271,    31,   273,   198,   266,   267,   268,   269,    63,
      78,  1207,   273,   190,   271,   122,   273,   149,   141,   126,
     145,   271,  1204,   273,    82,  1207,  1208,    23,   135,   271,
     155,   273,    28,   272,   266,   267,   268,   269,    67,    63,
    1320,   273,    61,   141,   151,  1204,    75,   122,  1207,  1208,
     195,   196,   197,   122,    83,   180,    64,  1496,  1217,   244,
    1219,   266,   267,   268,   269,   271,   174,   273,   273,    77,
     102,   178,   101,   188,   103,  1887,  1235,   172,   223,   224,
    1239,   155,   329,   190,   331,   192,    82,   116,   123,   118,
     119,   120,   100,    61,   254,   254,  1535,    30,   255,   271,
     263,    17,   266,   267,   268,   269,   207,    23,   266,   267,
     268,   269,    28,   112,   122,    31,   266,   267,   268,   269,
     266,   267,   268,   269,   264,    29,    10,   266,   267,   268,
     269,  1335,   199,   199,   272,  1423,  1340,   272,   167,  1805,
     199,  1767,   272,   151,   272,  1771,   272,   271,   288,  1299,
     272,    67,   259,  1779,   272,   272,  1782,   297,    64,    75,
     272,   272,    37,   270,   271,     6,   273,    83,     9,   272,
     272,    77,    13,    48,   263,   272,   272,   277,   272,    20,
      55,   272,  1808,   275,   277,   101,  1474,   103,   328,    10,
     272,    32,   332,    68,   100,   174,    63,    38,    39,  1408,
     116,   141,   118,   119,   120,    63,  1767,   166,   271,   271,
    1771,   271,   266,   267,   268,   269,   122,   263,  1779,  1845,
    1659,   277,   272,    10,  1850,    66,   271,   180,    69,   192,
      90,    90,    22,    90,   263,   264,   265,   266,   267,   268,
     269,  1423,    22,    43,    10,   151,   275,  1808,   277,  1408,
     125,   167,   174,    94,   208,    96,    97,  1416,  1417,  1418,
     136,   174,   136,    21,  1423,   106,   141,   196,    90,   110,
     266,   267,   268,   269,   210,   211,   212,   213,   214,   215,
     216,   217,   157,   185,  1845,   185,   138,   427,   272,  1850,
      91,   272,   432,   433,   434,   275,   272,   172,   272,    37,
      61,   176,    10,   122,   179,  1464,   149,   271,   128,   121,
      48,   272,   187,  1472,   272,  1474,   189,    55,   273,  1607,
      21,   278,   163,    42,   165,   174,    60,   272,  1616,   190,
     171,   206,  1614,   202,    63,    63,   186,    10,  1497,   240,
     181,    79,   121,    19,   243,   272,   272,   263,   264,   265,
     266,   267,   268,   269,   195,   196,   197,   198,   199,   200,
      43,    27,   502,    86,    86,   900,   506,   902,   209,   904,
     905,    22,   907,   908,   909,   125,   125,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   125,   230,
    1497,   156,   233,   234,  1643,   236,   237,   238,   239,   240,
     241,   242,   243,   141,    43,   246,   247,   248,   249,   250,
     154,   156,    98,   254,   255,   155,   272,   272,   272,   260,
     261,   262,   155,   271,    23,   266,   267,  1586,   273,    63,
     184,   272,    16,   274,   172,   271,   124,   278,   176,  1598,
     107,  1644,   271,    63,   271,    85,   272,    71,    12,   272,
     239,    10,    10,   160,   272,   133,   273,  1616,   274,   273,
     273,   192,  1586,   273,  1604,  1669,   178,    52,   108,  1673,
      52,   273,     8,    94,   273,  1634,   272,   272,   272,  1767,
    1639,   272,    51,  1771,   272,  1644,   272,   272,  1776,   272,
     272,  1779,  1741,   173,  1782,   272,   272,   272,  1663,   272,
     272,   272,  1711,   272,  1663,    70,   273,   271,   127,   127,
     273,   127,    85,    19,   271,   130,    73,   145,   125,   125,
    1808,   125,   662,  1811,   164,   125,   273,   125,   125,    22,
     273,   196,  1639,   272,    51,   173,    36,   273,   273,   679,
     270,   155,    36,   683,    82,   270,   273,   273,   272,   270,
     690,    82,  1711,  1712,   270,   695,   696,  1845,   198,    51,
     270,   270,  1850,  1713,   270,  1853,   706,   270,   708,   709,
     710,   711,   712,   713,   714,   270,   274,   124,   174,    10,
     135,    23,    79,    44,    80,  1750,  1874,   227,   228,  1877,
     230,  1750,   180,   180,  1776,    80,    44,   237,   238,   239,
     240,   241,   273,   243,  1892,   273,   246,   247,   248,   249,
     250,   751,    94,    63,   254,   255,   124,  1776,   273,   273,
     260,   261,   262,   111,  1783,  1790,  1785,   111,   273,  1811,
     273,  1790,   772,    61,   270,   189,  1801,   777,   273,   779,
       6,   273,  1801,     9,   273,   271,   273,    13,   273,    94,
     273,   273,  1811,    86,    20,   204,   273,    82,  1861,   270,
     273,   273,  1827,   273,   273,   273,    32,   272,  1827,   246,
     273,  1853,    38,    39,  1839,    82,   277,   272,   272,   108,
    1839,   272,   122,   148,   173,   271,   122,   122,    85,   270,
     273,   271,  1874,  1858,  1853,  1877,  1855,   273,   273,  1858,
      66,    47,  1861,    69,   271,   273,    44,   180,   126,    50,
    1892,    36,    47,   273,   201,  1874,   272,  1876,  1877,    79,
     271,   273,   272,  1882,     0,     0,  1222,   273,    94,    12,
      96,    97,   276,  1892,   732,   646,    27,   285,  1113,   873,
     106,  1110,   878,  1099,   110,  1904,  1905,   340,  1516,  1035,
     594,  1100,   324,   775,  1107,   895,   896,   897,  1515,  1581,
     900,   901,   902,   352,   904,   905,   906,   907,   908,   909,
     910,   911,  1494,   913,   804,   915,  1496,  1792,   918,   919,
     920,   921,   922,   923,  1348,  1349,   352,  1830,  1796,  1795,
    1354,  1355,   370,  1357,  1358,   935,   967,   163,   938,   165,
     794,  1365,  1366,  1367,  1368,   171,  1370,  1371,  1372,  1373,
    1374,  1375,  1704,  1602,  1777,   181,  1023,   720,  1750,  1479,
    1254,   345,   575,   357,   338,   852,  1308,   631,   598,   195,
     196,   197,   198,   199,   200,   844,    13,   519,   310,   764,
     515,   147,   777,   209,   751,   998,  1729,  1624,  1858,   989,
    1827,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,     0,   230,   527,  1263,   233,   234,     0,
     236,   237,   238,   239,   240,   241,   242,   243,   740,  1197,
     246,   247,   248,   249,   250,   990,  1328,   835,   254,   255,
    1153,  1089,  1634,    87,   260,   261,   262,  1416,    -1,   573,
     266,   267,    -1,    -1,    -1,    -1,   272,   198,   274,    -1,
      -1,    -1,   278,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    59,    60,    -1,     6,    -1,
      -1,     9,    -1,    -1,    68,    13,   227,   228,    -1,   230,
      -1,    -1,    20,    -1,    -1,    79,   237,   238,   239,   240,
     241,    -1,   243,    -1,    32,   246,   247,   248,   249,   250,
      38,    39,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,   272,    -1,    -1,    -1,    23,    -1,    -1,    66,    -1,
      28,    69,    -1,    31,    -1,    -1,    -1,    -1,    -1,  1139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,  1151,  1152,    -1,  1154,  1155,    94,    -1,    96,    97,
      -1,    -1,  1162,   157,    -1,    -1,    -1,    -1,   106,    67,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    75,   172,  1179,
      -1,    -1,   176,    -1,    -1,    83,    -1,  1187,  1188,  1189,
      -1,    -1,    -1,   187,    -1,  1195,    -1,  1197,  1198,  1199,
    1200,  1201,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    30,    -1,    -1,    -1,   116,    -1,
     118,   119,   120,    -1,    -1,   163,    -1,   165,    -1,  1229,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,  1245,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,   209,    -1,    -1,    -1,    -1,  1276,    -1,    -1,    -1,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,  1325,    -1,    -1,   266,   267,
      -1,    -1,    -1,    -1,   272,    -1,   274,    -1,    -1,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,  1347,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1356,    -1,    -1,    -1,
      -1,    -1,    -1,  1363,    -1,   263,   264,   265,   266,   267,
     268,   269,    -1,    -1,    -1,    -1,    -1,   275,    -1,   277,
      -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1393,  1394,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,
      13,   227,   228,  1413,   230,    -1,    -1,    20,    -1,    -1,
      -1,   237,   238,   239,   240,   241,    -1,   243,    -1,    32,
     246,   247,   248,   249,   250,    38,    39,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,    17,    -1,
      -1,    -1,    -1,   172,    23,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    31,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1475,    -1,    -1,    -1,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    75,   110,   227,   228,
      -1,   230,    -1,    -1,    83,    -1,    -1,    -1,   237,   238,
     239,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,   101,    -1,   103,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,    -1,    -1,   116,    -1,   118,
     119,   120,    -1,    -1,  1554,  1555,    -1,    -1,    -1,    -1,
     163,    -1,   165,    30,    -1,    -1,    -1,    -1,   171,    -1,
      -1,  1571,    -1,    -1,    -1,    -1,  1576,    -1,   181,  1579,
      -1,    -1,    -1,    -1,    -1,  1585,    -1,  1587,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,     6,    -1,    -1,     9,   260,   261,   262,
      13,    -1,    -1,   266,   267,    -1,    -1,    20,  1668,   272,
      -1,   274,    -1,    -1,    -1,   278,  1676,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   263,   264,   265,   266,   267,   268,
     269,    -1,    -1,    -1,  1704,    -1,   275,    -1,   277,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1729,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,   239,   240,   241,    -1,   243,     6,  1778,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    44,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,   198,    -1,
      -1,   254,   255,     6,    -1,    -1,     9,   260,   261,   262,
      13,    -1,    -1,   266,   267,    -1,   145,    20,    -1,   272,
      -1,   274,    -1,    -1,    -1,   278,   155,   227,   228,    32,
     230,    -1,    -1,    -1,    -1,    38,    39,   237,   238,   239,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,   180,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    66,    -1,    -1,    69,    -1,   268,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
     239,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,     6,    -1,    -1,
      -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    44,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    80,    -1,    -1,    -1,   155,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,   198,    -1,
      -1,   254,   255,     6,    -1,    -1,     9,   260,   261,   262,
      13,    -1,    -1,   266,   267,    -1,   145,    20,    -1,   272,
      -1,   274,    -1,    -1,    -1,   278,   155,   227,   228,    32,
     230,    -1,    -1,    -1,    -1,    38,    39,   237,   238,   239,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,   180,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
     239,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,   198,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
     163,    -1,   165,   237,   238,   239,   240,   241,   171,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,   181,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   272,    -1,
     274,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,     6,    -1,    -1,     9,   260,   261,   262,
      13,    -1,    -1,   266,   267,    -1,    -1,    20,    -1,   272,
      -1,   274,    -1,    -1,    -1,   278,    -1,    -1,    -1,    32,
      -1,    17,    -1,    -1,    -1,    38,    39,    23,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    94,    -1,    96,    97,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,   119,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,   167,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    13,    -1,   246,   247,   248,   249,   250,    20,    -1,
      -1,   254,   255,   141,    -1,    -1,    -1,   260,   261,   262,
      32,    -1,    -1,   266,   267,    -1,    38,    39,    -1,   272,
      -1,   274,    -1,    -1,    -1,   278,    -1,   263,   264,   265,
     266,   267,   268,   269,    -1,    57,    -1,   273,   176,    -1,
      -1,    -1,    -1,   181,    66,    -1,    -1,    69,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,
     228,    -1,   230,    -1,    -1,    -1,   118,    -1,    -1,   237,
     238,   239,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,   181,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    58,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    74,    -1,
      -1,    -1,    -1,    -1,    80,    -1,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,    -1,    13,    -1,    -1,   260,   261,
     262,    -1,    20,    -1,   266,   267,    -1,    -1,    -1,    -1,
     272,   137,   274,    -1,    32,    -1,   278,    35,    -1,    -1,
      38,    39,    -1,    -1,     7,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    25,    -1,   170,    -1,    -1,    -1,    66,    -1,
      33,    69,    -1,    -1,   180,    -1,    -1,    40,    -1,    42,
      -1,    44,    -1,   189,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,   198,    56,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    80,    -1,    -1,
     118,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,   239,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,    -1,   260,   261,   262,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    71,    -1,
     143,    -1,    -1,   181,    -1,    -1,    -1,   150,    -1,    -1,
     153,    -1,   155,   156,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    13,    -1,   246,   247,
     248,   249,   250,    20,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    32,   229,    -1,   266,   267,
      -1,    38,    39,    -1,   272,    -1,   274,    -1,    -1,    -1,
     278,   244,   245,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,   239,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,   181,    51,    -1,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,   209,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    13,    -1,   246,
     247,   248,   249,   250,    20,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    32,    -1,    -1,   266,
     267,   137,    38,    39,    -1,   272,    -1,   274,    -1,    -1,
      -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,   239,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,   159,   260,   261,   262,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,   239,   240,   241,   242,   243,    13,    -1,
     246,   247,   248,   249,   250,    20,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    32,    -1,    -1,
     266,   267,    -1,    38,    39,    -1,   272,    -1,   274,    -1,
      -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,   239,   240,
     241,    -1,   243,   118,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    55,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    80,   209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,   239,   240,   241,   242,   243,    13,
      -1,   246,   247,   248,   249,   250,    20,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    32,    -1,
     137,   266,   267,    -1,    38,    39,    -1,   272,    -1,   274,
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,   170,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,   198,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,   239,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      13,    94,   246,   247,   248,   249,   250,    20,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    32,
      -1,    -1,   266,   267,   268,    38,    39,    -1,   272,    -1,
     274,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,    -1,    -1,    -1,    -1,    -1,    -1,
     233,   234,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   242,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,   267,    -1,   189,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    13,    -1,   246,   247,   248,   249,   250,    20,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      32,    -1,    -1,   266,   267,    -1,    38,    39,    -1,   272,
      -1,   274,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    66,    21,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,   113,    -1,   115,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   183,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,   239,   240,   241,
     242,   243,    13,    -1,   246,   247,   248,   249,   250,    20,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    32,   218,   219,   266,   267,    -1,    38,    39,    -1,
     272,    -1,   274,    -1,    -1,    -1,   278,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    66,    21,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   209,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   183,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,   239,   240,
     241,   242,   243,    13,    -1,   246,   247,   248,   249,   250,
      20,    -1,   198,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    32,   218,   219,   266,   267,    -1,    38,    39,
      -1,   272,   273,   274,    -1,    -1,    -1,   278,    -1,    -1,
     235,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,   239,   240,   241,    66,   243,    -1,    69,
     246,   247,   248,   249,   250,   198,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,   274,    -1,
      -1,    -1,    -1,    -1,   227,   228,   106,   230,    -1,    -1,
     110,   198,    -1,    -1,   237,   238,   239,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,   272,
     237,   238,   239,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,   165,    -1,   254,   255,    -1,
      -1,   171,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,   272,    -1,    -1,    -1,    -1,
      -1,    -1,   192,    -1,    -1,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,   239,
     240,   241,   242,   243,    13,    -1,   246,   247,   248,   249,
     250,    20,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    32,    -1,    -1,   266,   267,    -1,    38,
      39,    -1,   272,    -1,   274,    -1,    -1,    -1,   278,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,   239,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,   198,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,   165,   237,   238,   239,
     240,   241,   171,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,   181,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
     239,   240,   241,   242,   243,    13,    -1,   246,   247,   248,
     249,   250,    20,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    32,    -1,    -1,   266,   267,    -1,
      38,    39,    -1,   272,    -1,   274,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,   239,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,   239,   240,   241,   242,   243,    13,    -1,   246,   247,
     248,   249,   250,    20,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    32,    -1,    -1,   266,   267,
      -1,    38,    39,    -1,   272,    -1,   274,    -1,    -1,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,   196,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,   239,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,   239,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    13,
      -1,    -1,    -1,   260,   261,   262,    20,    -1,   198,   266,
     267,    -1,    -1,   270,    -1,   272,    -1,   274,    32,    -1,
      -1,   278,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,   239,
     240,   241,    66,   243,    -1,    69,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,    -1,    -1,    -1,    -1,   268,    -1,
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
      -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,   239,   240,   241,   242,   243,
      13,    -1,   246,   247,   248,   249,   250,    20,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    32,
      -1,    -1,   266,   267,    -1,    38,    39,    -1,   272,    -1,
     274,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,   239,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    18,    -1,    -1,    21,   260,   261,   262,
      -1,    -1,    27,   266,   267,    -1,    -1,    -1,    -1,   272,
      -1,   274,    -1,    38,    -1,   278,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    41,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,   219,    -1,   183,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
     235,   198,   237,   238,   239,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   218,   219,    -1,    -1,   260,   261,   262,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
     237,   238,   239,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   198,
      -1,    -1,    -1,   260,   261,   262,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,    -1,    -1,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,    -1,   237,   238,
     239,   240,   241,   242,   243,   244,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
     259,   260,   261,   262
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
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 102:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 103:
{ yyval = yyvsp[0]; }
    break;
case 104:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_unique, 0, NULL); }
    break;
case 123:
{ yyval = NULL; }
    break;
case 124:
{ yyval = make_list (yyvsp[0]); }
    break;
case 126:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 127:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 128:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 129:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 130:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 131:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 132:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 133:
{ yyval = (dsql_nod*) 0;}
    break;
case 134:
{ yyval = yyvsp[-1]; }
    break;
case 135:
{ yyval = NULL; }
    break;
case 138:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 139:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 140:
{ yyval = NULL; }
    break;
case 141:
{ yyval = NULL; }
    break;
case 142:
{ yyval = yyvsp[0]; }
    break;
case 143:
{ yyval = NULL; }
    break;
case 144:
{ yyval = NULL; }
    break;
case 147:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 148:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 151:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 152:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 153:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
    break;
case 154:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
    break;
case 155:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 158:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
    break;
case 159:
{yyval = NULL;}
    break;
case 162:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 163:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 164:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 165:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 166:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 167:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 168:
{yyval = NULL;}
    break;
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 173:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 174:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 175:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 176:
{ lex.g_file  = make_file();}
    break;
case 181:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 182:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;
case 189:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 190:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 191:
{ yyval = yyvsp[0]; }
    break;
case 192:
{ yyval = yyvsp[0]; }
    break;
case 193:
{ yyval = NULL; }
    break;
case 195:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 198:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 199:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 200:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 201:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 204:
{ yyval = NULL; }
    break;
case 205:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 206:
{ yyval = yyvsp[0]; }
    break;
case 207:
{ yyval = NULL; }
    break;
case 208:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 209:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 210:
{ yyval = yyvsp[-1]; }
    break;
case 211:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 212:
{ yyval = yyvsp[0]; }
    break;
case 213:
{ yyval = NULL; }
    break;
case 220:
{ yyval = NULL; }
    break;
case 223:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 224:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 225:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 226:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 228:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 229:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 231:
{ yyval = yyvsp[0]; }
    break;
case 232:
{ yyval = NULL ;}
    break;
case 237:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 238:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 239:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 240:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 241:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 242:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 243:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 245:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 246:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 247:
{ yyval = NULL;}
    break;
case 248:
{ yyval = yyvsp[0];}
    break;
case 249:
{ yyval = yyvsp[0];}
    break;
case 250:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 252:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 253:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 254:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 255:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 256:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 257:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 258:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 259:
{ yyval = NULL; }
    break;
case 260:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 261:
{ yyval = NULL; }
    break;
case 263:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 264:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 266:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 267:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 268:
{ yyval = yyvsp[0]; }
    break;
case 269:
{ yyval = yyvsp[0]; }
    break;
case 270:
{ yyval = (dsql_nod*) NULL; }
    break;
case 271:
{ yyval = make_list (yyvsp[0]); }
    break;
case 272:
{ yyval = NULL; }
    break;
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 275:
{ yyval = yyvsp[-1]; }
    break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 279:
{ yyval = NULL; }
    break;
case 280:
{ yyval = NULL; }
    break;
case 281:
{ yyval = yyvsp[0]; }
    break;
case 282:
{ yyval = yyvsp[0]; }
    break;
case 283:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 286:
{ yyval = yyvsp[-1]; }
    break;
case 287:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 288:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 289:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 307:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 308:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 311:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 314:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 315:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 317:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 319:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 321:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 322:
{ yyval = NULL; }
    break;
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 324:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 325:
{ yyval = make_list (yyvsp[0]); }
    break;
case 326:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 327:
{ yyval = NULL; }
    break;
case 328:
{ yyval = make_list (yyvsp[0]); }
    break;
case 329:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 330:
{ yyval = NULL; }
    break;
case 333:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 335:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 336:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 337:
{ yyval = NULL; }
    break;
case 338:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 341:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 342:
{ yyval = NULL; }
    break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 345:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 348:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 349:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 350:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 351:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 355:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 356:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 357:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 358:
{ yyval = NULL; }
    break;
case 359:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 360:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 361:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 362:
{ yyval = NULL; }
    break;
case 364:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 365:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 366:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 367:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 368:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 369:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 371:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 372:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 373:
{ yyval = make_list (yyvsp[0]); }
    break;
case 375:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 378:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 379:
{ yyval = yyvsp[-1]; }
    break;
case 380:
{ lex.beginning = lex_position(); }
    break;
case 381:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 382:
{ lex.beginning = lex.last_token; }
    break;
case 383:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 384:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 385:
{ yyval = 0; }
    break;
case 386:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 387:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 388:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 390:
{ yyval = NULL; }
    break;
case 391:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 392:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 403:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 404:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 406:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 407:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 408:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 409:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 410:
{ yyval = NULL; }
    break;
case 411:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 412:
{ yyval = yyvsp[0]; }
    break;
case 413:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 414:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 415:
{ yyval = yyvsp[0]; }
    break;
case 416:
{ yyval = yyvsp[0]; }
    break;
case 417:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 418:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 419:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 420:
{ yyval = yyvsp[0]; }
    break;
case 421:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 423:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 424:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 425:
{ yyval = yyvsp[0]; }
    break;
case 426:
{ yyval = yyvsp[0]; }
    break;
case 427:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 428:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 429:
{ yyval = yyvsp[0]; }
    break;
case 430:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 432:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 433:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 434:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 435:
{ yyval = yyvsp[0]; }
    break;
case 436:
{ yyval = yyvsp[0]; }
    break;
case 437:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 438:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 439:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 440:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 468:
{ yyval = NULL; }
    break;
case 469:
{ yyval = NULL; }
    break;
case 470:
{ yyval = NULL; }
    break;
case 471:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 472:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 473:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 474:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 475:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 476:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 477:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 478:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 480:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 481:
{ yyval = yyvsp[0]; }
    break;
case 482:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 483:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 484:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 485:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 486:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 488:
{ yyval = NULL; }
    break;
case 490:
{ yyval = NULL; }
    break;
case 491:
{ yyval = yyvsp[0]; }
    break;
case 492:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 494:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 501:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 502:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 503:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 508:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 509:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 511:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 512:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 513:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 519:
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
case 520:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 522:
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
case 523:
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
case 524:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 531:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 532:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 533:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 534:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 535:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 536:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 552:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 553:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 554:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 555:
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
case 556:
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
case 559:
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
case 560:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 563:
{ yyval = yyvsp[-1]; }
    break;
case 564:
{ yyval = 0; }
    break;
case 568:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 569:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 570:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 574:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 575:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 576:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 577:
{ yyval = 0; }
    break;
case 578:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 581:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 582:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 585:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 586:
{ yyval = NULL; }
    break;
case 588:
{ yyval = NULL; }
    break;
case 589:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 591:
{ yyval = NULL; }
    break;
case 593:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 598:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 599:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 600:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 601:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 602:
{ yyval = yyvsp[0];}
    break;
case 604:
{ yyval = yyvsp[0];}
    break;
case 605:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 606:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 607:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 608:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 609:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 610:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 611:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 612:
{ yyval = 0; }
    break;
case 613:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 614:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 615:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 616:
{ yyval = (dsql_nod*) 0; }
    break;
case 617:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 618:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 620:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 621:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 622:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 623:
{ yyval = 0; }
    break;
case 625:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 626:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 627:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 628:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 629:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 630:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 631:
{ yyval = make_list (yyvsp[0]); }
    break;
case 632:
{ yyval = 0; }
    break;
case 634:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 635:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 636:
{ yyval = 0; }
    break;
case 637:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 638:
{ yyval = 0; }
    break;
case 639:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 640:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 641:
{ yyval = yyvsp[-1]; }
    break;
case 642:
{ yyval = 0; }
    break;
case 643:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 644:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 645:
{ yyval = NULL; }
    break;
case 646:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 647:
{ yyval = 0; }
    break;
case 648:
{ yyval = yyvsp[0]; }
    break;
case 649:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 650:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 651:
{ yyval = 0; }
    break;
case 652:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 653:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 654:
{ lex.limit_clause = true; }
    break;
case 655:
{ lex.limit_clause = false; }
    break;
case 656:
{ lex.first_detection = true; }
    break;
case 657:
{ lex.first_detection = false; }
    break;
case 658:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 659:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 660:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 661:
{ yyval = 0; }
    break;
case 662:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 663:
{ yyval = yyvsp[-2]; }
    break;
case 664:
{ yyval = yyvsp[-1]; }
    break;
case 665:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 666:
{ yyval = yyvsp[-1]; }
    break;
case 667:
{ yyval = yyvsp[0]; }
    break;
case 668:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 669:
{ yyval = 0; }
    break;
case 670:
{ yyval = make_list (yyvsp[0]); }
    break;
case 671:
{ yyval = 0; }
    break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 675:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 678:
{ yyval = make_list (yyvsp[0]); }
    break;
case 680:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 684:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 686:
{ yyval = NULL; }
    break;
case 687:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 688:
{ yyval = NULL; }
    break;
case 690:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 691:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 692:
{ yyval = yyvsp[-1]; }
    break;
case 693:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 694:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 695:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 696:
{ yyval = NULL; }
    break;
case 698:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 699:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 701:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 702:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 703:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 704:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 705:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 706:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 707:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 708:
{ yyval = make_list (yyvsp[0]); }
    break;
case 709:
{ yyval = 0; }
    break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
    break;
case 718:
{ yyval = 0; }
    break;
case 719:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 720:
{ yyval = 0; }
    break;
case 721:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 722:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 723:
{ yyval = 0; }
    break;
case 724:
{ yyval = 0; }
    break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 727:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 731:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 732:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 733:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 736:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 737:
{ yyval = 0; }
    break;
case 738:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 739:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 743:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 744:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 747:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 748:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 750:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 751:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 752:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 753:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 755:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 759:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 760:
{ yyval = yyvsp[0]; }
    break;
case 762:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 765:
{ yyval = NULL; }
    break;
case 766:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 770:
{ yyval = NULL; }
    break;
case 771:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 773:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 775:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 779:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 781:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 783:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 789:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = yyvsp[-1]; }
    break;
case 793:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 807:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 811:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 816:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 817:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 818:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 819:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 820:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 821:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 822:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 825:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 826:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 830:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 832:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 834:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 836:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 838:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 839:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 840:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 842:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 844:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 845:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 846:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 851:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 852:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 853:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 855:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 856:
{ yyval = yyvsp[-1]; }
    break;
case 857:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 858:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 868:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 869:
{ yyval = yyvsp[0]; }
    break;
case 870:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 871:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 872:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 873:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 874:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 875:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 876:
{ yyval = yyvsp[-1]; }
    break;
case 877:
{ yyval = yyvsp[-1]; }
    break;
case 881:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 882:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 883:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 886:
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
case 887:
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
case 888:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 889:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 891:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 893:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 894:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 895:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 900:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 901:
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
case 902:
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
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 904:
{ yyval = make_parameter (); }
    break;
case 905:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 906:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 907:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 908:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 909:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 910:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 913:
{ yyval = yyvsp[0]; }
    break;
case 914:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 916:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 917:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 918:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 919:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 920:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 922:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 923:
{ yyval = yyvsp[0];}
    break;
case 928:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 929:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 930:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 935:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 936:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 937:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 938:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 941:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 943:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 944:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 945:
{ yyval = yyvsp[0]; }
    break;
case 946:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 947:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 948:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 949:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 952:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 953:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 954:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 957:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 958:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 959:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 960:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 961:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 962:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 963:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 964:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 968:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 980:
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

