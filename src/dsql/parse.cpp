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

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
#include <stdio.h>
/*#include <wincon.h>*/
#endif

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
#include "../wal/wal.h"
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
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(DSQL_NOD, SLONG *);
#endif
static dsql_fld*	make_field (DSQL_NOD);
static dsql_fil*	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(DSQL_NOD, SLONG *, SSHORT);
#endif
static void	stack_nodes (DSQL_NOD, dsql_lls**);
inline static int	yylex (USHORT, USHORT, USHORT, BOOLEAN*);
static void	yyabandon (SSHORT, ISC_STATUS);
static void	check_log_file_attrs (void);

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
	dsql_fil*	g_file;
	DSQL_NOD g_field_name;
	SSHORT log_defined, cache_defined;
	int dsql_debug;
	
	/* Actual lexer state begins from here */
	TEXT *beginning;
	TEXT	*ptr, *end, *last_token, *line_start;
	TEXT	*last_token_bk, *line_start_bk;
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
		BOOLEAN* stmt_ambiguous);
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



#define	YYFINAL		1907
#define	YYFLAG		-32768
#define	YYNTBASE	284

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 521 ? yytranslate[x] : 715)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     277,   278,   273,   271,   276,   272,   282,   274,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   279,   275,
     269,   268,   270,   283,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   280,     2,   281,     2,     2,     2,     2,     2,     2,
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
     266,   267
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    44,    52,    61,    68,    76,    82,    84,    87,
      89,    92,    94,    96,   100,   102,   104,   106,   108,   111,
     114,   118,   119,   123,   124,   126,   134,   143,   150,   158,
     165,   173,   178,   182,   184,   188,   192,   196,   199,   202,
     205,   208,   210,   214,   216,   219,   222,   224,   228,   230,
     232,   236,   238,   241,   244,   247,   251,   260,   262,   264,
     270,   271,   273,   277,   279,   283,   286,   291,   293,   297,
     300,   304,   309,   314,   317,   327,   330,   334,   342,   345,
     348,   351,   354,   357,   360,   363,   366,   369,   372,   375,
     378,   381,   386,   389,   392,   394,   395,   397,   399,   406,
     413,   415,   417,   418,   419,   421,   422,   427,   428,   430,
     432,   435,   444,   446,   447,   451,   452,   453,   455,   457,
     460,   462,   464,   466,   469,   476,   478,   480,   484,   485,
     487,   489,   490,   492,   494,   497,   501,   506,   509,   512,
     516,   517,   519,   521,   524,   526,   528,   530,   535,   539,
     543,   547,   551,   555,   557,   559,   566,   570,   572,   576,
     578,   582,   585,   587,   588,   591,   595,   597,   598,   600,
     602,   605,   609,   614,   615,   617,   620,   621,   623,   625,
     631,   637,   641,   644,   645,   647,   651,   653,   655,   662,
     666,   669,   676,   679,   681,   684,   687,   690,   691,   693,
     695,   698,   699,   702,   703,   705,   707,   709,   711,   713,
     715,   716,   718,   720,   723,   726,   729,   735,   737,   740,
     744,   747,   750,   751,   753,   755,   757,   759,   763,   768,
     777,   782,   783,   790,   792,   794,   797,   800,   801,   805,
     809,   811,   814,   817,   820,   829,   838,   847,   856,   860,
     861,   864,   865,   867,   871,   874,   876,   877,   879,   882,
     887,   889,   891,   895,   897,   898,   901,   903,   910,   912,
     914,   918,   920,   923,   924,   926,   929,   932,   935,   937,
     939,   941,   943,   945,   947,   950,   952,   954,   957,   959,
     961,   963,   966,   969,   971,   974,   978,   983,   986,   993,
     998,  1000,  1002,  1011,  1021,  1028,  1037,  1044,  1049,  1050,
    1055,  1058,  1060,  1064,  1065,  1068,  1073,  1074,  1076,  1078,
    1082,  1086,  1094,  1097,  1098,  1101,  1104,  1108,  1112,  1113,
    1115,  1118,  1123,  1125,  1129,  1132,  1135,  1138,  1140,  1142,
    1144,  1146,  1149,  1152,  1158,  1159,  1164,  1172,  1180,  1182,
    1184,  1188,  1193,  1201,  1204,  1206,  1210,  1212,  1214,  1221,
    1225,  1226,  1227,  1228,  1229,  1233,  1234,  1244,  1254,  1256,
    1258,  1259,  1262,  1264,  1266,  1268,  1270,  1272,  1276,  1280,
    1284,  1288,  1292,  1296,  1302,  1308,  1314,  1320,  1326,  1332,
    1335,  1336,  1341,  1344,  1348,  1352,  1355,  1358,  1362,  1366,
    1369,  1373,  1380,  1382,  1385,  1390,  1394,  1397,  1400,  1403,
    1406,  1410,  1412,  1416,  1420,  1424,  1427,  1430,  1435,  1440,
    1446,  1448,  1450,  1452,  1454,  1456,  1458,  1460,  1462,  1464,
    1466,  1468,  1470,  1472,  1474,  1476,  1478,  1480,  1482,  1484,
    1486,  1488,  1490,  1492,  1494,  1496,  1498,  1500,  1502,  1504,
    1507,  1510,  1513,  1515,  1517,  1519,  1520,  1523,  1526,  1527,
    1529,  1532,  1535,  1538,  1541,  1544,  1549,  1553,  1556,  1559,
    1561,  1565,  1573,  1575,  1576,  1578,  1579,  1582,  1585,  1588,
    1591,  1594,  1597,  1600,  1603,  1606,  1610,  1613,  1616,  1619,
    1621,  1623,  1625,  1627,  1632,  1638,  1640,  1644,  1646,  1650,
    1652,  1655,  1657,  1659,  1661,  1663,  1665,  1667,  1669,  1671,
    1673,  1675,  1677,  1682,  1687,  1694,  1700,  1704,  1705,  1708,
    1711,  1712,  1716,  1717,  1722,  1724,  1730,  1735,  1737,  1742,
    1744,  1747,  1750,  1752,  1754,  1756,  1759,  1762,  1765,  1768,
    1769,  1773,  1779,  1781,  1783,  1786,  1790,  1792,  1795,  1799,
    1800,  1802,  1804,  1806,  1812,  1818,  1825,  1827,  1829,  1831,
    1834,  1839,  1841,  1842,  1848,  1850,  1851,  1855,  1858,  1860,
    1861,  1864,  1865,  1867,  1868,  1872,  1874,  1875,  1877,  1880,
    1882,  1884,  1886,  1888,  1891,  1894,  1896,  1899,  1903,  1905,
    1907,  1911,  1915,  1917,  1920,  1924,  1926,  1929,  1930,  1933,
    1935,  1937,  1938,  1940,  1942,  1944,  1948,  1951,  1955,  1956,
    1958,  1962,  1967,  1973,  1975,  1979,  1984,  1988,  1989,  1991,
    1995,  1999,  2001,  2003,  2004,  2006,  2008,  2013,  2014,  2017,
    2022,  2023,  2027,  2028,  2031,  2032,  2035,  2036,  2046,  2058,
    2059,  2060,  2061,  2062,  2066,  2069,  2071,  2072,  2076,  2082,
    2086,  2089,  2095,  2098,  2100,  2102,  2104,  2106,  2108,  2112,
    2114,  2118,  2120,  2121,  2124,  2126,  2130,  2132,  2134,  2136,
    2143,  2145,  2146,  2150,  2151,  2153,  2157,  2164,  2168,  2173,
    2176,  2180,  2181,  2183,  2186,  2188,  2190,  2192,  2195,  2197,
    2200,  2202,  2205,  2206,  2210,  2211,  2213,  2217,  2219,  2222,
    2223,  2226,  2227,  2230,  2231,  2236,  2238,  2241,  2243,  2245,
    2246,  2248,  2252,  2255,  2257,  2259,  2262,  2264,  2269,  2273,
    2275,  2279,  2284,  2285,  2294,  2300,  2302,  2304,  2309,  2314,
    2319,  2321,  2323,  2329,  2335,  2337,  2341,  2345,  2347,  2349,
    2351,  2355,  2363,  2371,  2377,  2381,  2382,  2384,  2386,  2388,
    2391,  2392,  2394,  2396,  2398,  2399,  2403,  2405,  2409,  2411,
    2412,  2416,  2418,  2422,  2424,  2428,  2432,  2434,  2436,  2440,
    2442,  2445,  2447,  2451,  2455,  2457,  2460,  2464,  2468,  2472,
    2476,  2478,  2482,  2485,  2487,  2489,  2491,  2493,  2495,  2497,
    2499,  2501,  2503,  2505,  2509,  2513,  2517,  2521,  2525,  2529,
    2533,  2537,  2544,  2551,  2558,  2565,  2572,  2579,  2586,  2593,
    2600,  2607,  2614,  2621,  2628,  2635,  2642,  2649,  2651,  2653,
    2659,  2666,  2670,  2675,  2681,  2688,  2692,  2697,  2701,  2706,
    2710,  2715,  2720,  2726,  2731,  2736,  2740,  2745,  2747,  2749,
    2751,  2753,  2755,  2757,  2759,  2763,  2767,  2779,  2791,  2793,
    2795,  2797,  2799,  2801,  2803,  2805,  2807,  2809,  2812,  2815,
    2819,  2823,  2827,  2831,  2835,  2839,  2843,  2847,  2849,  2851,
    2853,  2855,  2859,  2861,  2863,  2865,  2867,  2869,  2874,  2876,
    2880,  2882,  2885,  2887,  2889,  2891,  2893,  2895,  2897,  2899,
    2902,  2905,  2908,  2910,  2912,  2914,  2916,  2918,  2920,  2922,
    2924,  2926,  2928,  2931,  2933,  2936,  2938,  2940,  2942,  2944,
    2946,  2949,  2951,  2953,  2955,  2957,  2959,  2964,  2970,  2976,
    2982,  2988,  2994,  3000,  3006,  3012,  3018,  3024,  3031,  3033,
    3040,  3042,  3047,  3055,  3058,  3059,  3064,  3068,  3075,  3077,
    3079,  3086,  3093,  3095,  3097,  3102,  3109,  3114,  3120,  3124,
    3130,  3135,  3141,  3143,  3145,  3147,  3149,  3151,  3153,  3155,
    3157,  3159,  3161,  3163,  3165,  3166,  3168,  3170,  3172,  3174,
    3176,  3178,  3180,  3182,  3184,  3186,  3188,  3190,  3192,  3194,
    3196,  3198,  3200,  3202,  3204,  3206,  3208,  3210,  3212,  3214,
    3216,  3218,  3220,  3222,  3224,  3226,  3228,  3230,  3232,  3234,
    3236,  3238,  3240,  3242,  3244,  3246,  3248,  3250
};
static const short yyrhs[] =
{
     285,     0,   285,   275,     0,   470,     0,   612,     0,   524,
       0,   315,     0,   305,     0,   601,     0,   492,     0,   286,
       0,   600,     0,   447,     0,   317,     0,   319,     0,   295,
       0,   525,     0,   518,     0,   547,     0,   516,     0,   605,
       0,    43,   659,     0,    73,   288,   129,   287,   182,   299,
     292,     0,    73,   290,   129,   148,   294,   182,   299,   292,
       0,    73,   288,   129,   287,   182,   297,     0,    73,   290,
     129,   148,   294,   182,   297,     0,    73,   301,   182,   303,
     293,     0,   584,     0,   180,   584,     0,     6,     0,     6,
     147,     0,   289,     0,   291,     0,   289,   276,   291,     0,
      59,     0,   163,     0,    84,     0,    47,     0,   188,   617,
       0,   153,   617,     0,   200,    73,   131,     0,     0,   200,
     210,   131,     0,     0,   704,     0,   158,   296,   288,   129,
     287,    66,   299,     0,   158,   296,   290,   129,   148,   294,
      66,   299,     0,   158,   288,   129,   287,    66,   299,     0,
     158,   290,   129,   148,   294,    66,   299,     0,   158,   288,
     129,   287,    66,   297,     0,   158,   290,   129,   148,   294,
      66,   297,     0,   158,   301,    66,   303,     0,    73,   131,
      64,     0,   298,     0,   297,   276,   298,     0,   297,   276,
     300,     0,   299,   276,   298,     0,   148,   704,     0,   184,
     708,     0,   195,   711,     0,   214,   705,     0,   300,     0,
     299,   276,   300,     0,   709,     0,   189,   709,     0,    74,
     709,     0,   302,     0,   301,   276,   302,     0,   705,     0,
     304,     0,   303,   276,   304,     0,   709,     0,   189,   709,
       0,    45,   306,     0,    63,   314,     0,    62,    68,   307,
       0,   689,   309,   157,   312,    55,   658,   117,   658,     0,
     499,     0,    19,     0,    37,   277,   662,   278,   505,     0,
       0,   310,     0,   277,   310,   278,     0,   311,     0,   310,
     276,   311,     0,   378,   308,     0,   378,   308,    20,   238,
       0,   313,     0,   277,   313,   278,     0,   378,   308,     0,
     378,   308,   212,     0,   378,   308,    20,   101,     0,   378,
     308,    20,   238,     0,   140,   662,     0,   698,    82,   504,
     135,   504,    55,   658,   117,   658,     0,    36,   316,     0,
      58,   697,   658,     0,   321,   552,    83,   701,   129,   584,
     322,     0,   148,   397,     0,   180,   365,     0,   184,   462,
       0,   195,   448,     0,    71,   338,     0,    40,   340,     0,
      51,   329,     0,   165,   323,     0,   214,   339,     0,   237,
     318,     0,   148,   398,     0,   180,   366,     0,   195,   449,
       0,    36,   132,     7,   320,     0,   148,   399,     0,   184,
     463,     0,   187,     0,     0,   619,     0,   618,     0,   372,
     277,   459,   647,   460,   278,     0,   662,   324,   325,   658,
     326,   327,     0,   111,     0,    14,     0,     0,     0,    32,
       0,     0,   102,   341,   665,   364,     0,     0,   328,     0,
     353,     0,   328,   353,     0,   375,   330,   494,   459,   331,
     460,   332,   374,     0,    10,     0,     0,    46,   459,   380,
       0,     0,     0,   333,     0,   334,     0,   333,   334,     0,
     335,     0,   336,     0,   337,     0,   124,    98,     0,   459,
      24,   277,   626,   278,   460,     0,   700,     0,   705,     0,
     342,   343,   346,     0,     0,   268,     0,   658,     0,     0,
     344,     0,   345,     0,   344,   345,     0,   139,   341,   662,
       0,   102,   341,   665,   364,     0,   189,   658,     0,   141,
     658,     0,   164,   118,   658,     0,     0,   347,     0,   348,
       0,   347,   348,     0,   353,     0,   350,     0,   349,     0,
      46,    23,   164,   691,     0,   263,    94,   658,     0,    75,
     341,   665,     0,    25,   341,   665,     0,   127,   341,   662,
       0,   109,   341,   663,     0,   352,     0,   351,     0,   103,
     277,   354,   278,   136,   355,     0,   103,    15,   355,     0,
     103,     0,   359,   658,   360,     0,   355,     0,   354,   276,
     355,     0,   356,   357,     0,   658,     0,     0,   357,   358,
       0,   168,   341,   665,     0,    94,     0,     0,   361,     0,
     362,     0,   361,   362,     0,   175,   363,   665,     0,   102,
     341,   665,   364,     0,     0,    12,     0,    12,   137,     0,
       0,   137,     0,   138,     0,   584,   367,   277,   368,   278,
       0,   584,   367,   277,   368,   278,     0,    62,    94,   658,
       0,    62,   658,     0,     0,   369,     0,   368,   276,   369,
       0,   370,     0,   385,     0,   375,   373,   379,   460,   381,
     374,     0,   375,   495,   371,     0,   375,   371,     0,   372,
     277,   459,   647,   460,   278,     0,    30,    20,     0,    30,
       0,   494,   459,     0,   624,   457,     0,    26,   692,     0,
       0,   624,     0,   624,     0,   378,   494,     0,     0,    46,
     380,     0,     0,   651,     0,   655,     0,   656,     0,   657,
       0,   688,     0,   648,     0,     0,   382,     0,   383,     0,
     382,   383,     0,   386,   384,     0,   124,    98,     0,   153,
     584,   617,   393,   391,     0,   392,     0,   187,   391,     0,
     146,    90,   391,     0,   386,   387,     0,    33,   694,     0,
       0,   388,     0,   389,     0,   390,     0,   392,     0,   187,
     618,   391,     0,   146,    90,   618,   391,     0,    65,    90,
     618,   153,   584,   617,   393,   391,     0,   247,   552,    83,
     701,     0,     0,   459,    24,   277,   626,   278,   460,     0,
     394,     0,   395,     0,   395,   394,     0,   394,   395,     0,
       0,   129,   188,   396,     0,   129,    47,   396,     0,   211,
       0,   164,    46,     0,   164,    98,     0,   123,   209,     0,
     704,   401,   402,    10,   457,   405,   414,   460,     0,   704,
     401,   402,    10,   457,   405,   414,   460,     0,   704,   401,
     402,    10,   457,   405,   414,   460,     0,   704,   401,   402,
      10,   457,   405,   414,   460,     0,   277,   403,   278,     0,
       0,   157,   401,     0,     0,   404,     0,   403,   276,   404,
       0,   376,   495,     0,   406,     0,     0,   407,     0,   406,
     407,     0,    45,   410,   408,   275,     0,   409,     0,   412,
       0,   375,   495,   411,     0,   192,     0,     0,   268,   380,
       0,   379,     0,   695,    39,    64,   277,   560,   278,     0,
     417,     0,   414,     0,    17,   415,    54,     0,   416,     0,
     416,   438,     0,     0,   413,     0,   416,   413,     0,   609,
     275,     0,   601,   275,     0,   418,     0,   419,     0,   420,
       0,   421,     0,   423,     0,   426,     0,   600,   275,     0,
     427,     0,   429,     0,   605,   275,     0,   434,     0,   424,
       0,   425,     0,   178,   275,     0,    61,   275,     0,   436,
       0,   442,   275,     0,    58,   697,   275,     0,    58,   697,
     647,   275,     0,    58,   275,     0,    59,   148,   704,   431,
     432,   275,     0,    59,   422,   647,   275,     0,   191,     0,
     254,     0,   435,    64,   547,    86,   433,   437,    50,   413,
       0,   435,    64,    59,   422,   647,    86,   433,    50,   413,
       0,    59,   422,   647,    86,   433,   275,     0,    78,   277,
     626,   278,   181,   413,    53,   413,     0,    78,   277,   626,
     278,   181,   413,     0,   144,   647,   428,   275,     0,     0,
     547,    86,   433,   275,     0,   279,   710,     0,   611,     0,
     277,   611,   278,     0,     0,   156,   433,     0,   156,   277,
     433,   278,     0,     0,   430,     0,   623,     0,   433,   276,
     623,     0,   433,   276,   430,     0,   435,   199,   277,   626,
     278,    50,   413,     0,   703,   279,     0,     0,   235,   275,
       0,   255,   275,     0,   255,   703,   275,     0,    10,    39,
     695,     0,     0,   439,     0,   438,   439,     0,   197,   440,
      50,   413,     0,   441,     0,   440,   276,   441,     0,   173,
     659,     0,    69,   699,     0,    58,   697,     0,     9,     0,
     443,     0,   445,     0,   444,     0,   264,   695,     0,   265,
     695,     0,   266,   446,   695,    86,   433,     0,     0,    59,
     148,   704,   431,     0,   711,   617,    10,   457,   450,   461,
     458,     0,   711,   617,    10,   457,   450,   461,   458,     0,
     451,     0,   452,     0,   451,   186,   452,     0,   451,   186,
       6,   452,     0,   163,   568,   569,   453,   590,   586,   589,
       0,    66,   454,     0,   455,     0,   454,   276,   455,     0,
     456,     0,   583,     0,   455,   585,    89,   455,   129,   626,
       0,   277,   456,   278,     0,     0,     0,     0,     0,   200,
      24,   131,     0,     0,   708,    64,   584,   464,   465,   468,
     459,   469,   460,     0,   708,    64,   584,   464,   465,   468,
     459,   469,   460,     0,     3,     0,    80,     0,     0,   466,
     467,     0,    16,     0,     5,     0,    84,     0,   188,     0,
      47,     0,    84,   132,   188,     0,    84,   132,    47,     0,
     188,   132,    84,     0,   188,   132,    47,     0,    47,   132,
      84,     0,    47,   132,   188,     0,    84,   132,   188,   132,
      47,     0,    84,   132,    47,   132,   188,     0,   188,   132,
      84,   132,    47,     0,   188,   132,    47,   132,    84,     0,
      47,   132,    84,   132,   188,     0,    47,   132,   188,   132,
      84,     0,   143,   660,     0,     0,    10,   459,   405,   414,
       0,     7,   471,     0,    58,   697,   658,     0,   180,   584,
     476,     0,   184,   489,     0,   148,   400,     0,    40,   485,
     486,     0,    51,   478,   474,     0,    83,   484,     0,    46,
     459,   380,     0,    24,   459,   277,   626,   278,   460,     0,
     475,     0,   474,   475,     0,   164,   459,   472,   460,     0,
       4,    33,   473,     0,     4,   337,     0,    52,    46,     0,
      52,    33,     0,   182,   624,     0,   216,   378,   495,     0,
     477,     0,   476,   276,   477,     0,    52,   624,   483,     0,
      52,    33,   694,     0,     4,   370,     0,     4,   385,     0,
     480,   624,   143,   662,     0,   480,   478,   182,   624,     0,
     480,   482,   216,   481,   460,     0,   479,     0,   713,     0,
     215,     0,   216,     0,   217,     0,   218,     0,   219,     0,
     220,     0,   221,     0,   222,     0,   223,     0,   224,     0,
     225,     0,   226,     0,   227,     0,   228,     0,   229,     0,
     230,     0,   233,     0,   234,     0,   241,     0,   242,     0,
     250,     0,   252,     0,   264,     0,   265,     0,   266,     0,
     267,     0,     7,     0,     7,   215,     0,   495,   459,     0,
     624,   457,     0,   624,     0,   213,     0,   211,     0,     0,
     701,     3,     0,   701,    80,     0,     0,   487,     0,   486,
     487,     0,     4,   328,     0,    52,   103,     0,   164,   488,
       0,     4,   350,     0,     4,   263,    94,   658,     0,    52,
     263,    94,     0,    17,   262,     0,    54,   262,     0,   349,
       0,   488,   276,   349,     0,   708,   464,   490,   468,   459,
     491,   460,     0,   465,     0,     0,   469,     0,     0,    52,
     493,     0,    58,   697,     0,    83,   701,     0,   148,   704,
       0,   180,   707,     0,   184,   708,     0,   195,   711,     0,
      63,   698,     0,    51,   696,     0,    62,    68,   689,     0,
     165,   662,     0,   214,   705,     0,    71,   700,     0,   495,
       0,   496,     0,   499,     0,   502,     0,   500,   280,   497,
     281,     0,   507,   280,   497,   281,   505,     0,   498,     0,
     497,   276,   498,     0,   664,     0,   664,   279,   664,     0,
     500,     0,   507,   505,     0,   506,     0,   511,     0,   514,
       0,   243,     0,   501,     0,   169,     0,    41,     0,   226,
       0,   227,     0,    85,     0,    96,     0,    19,   504,   503,
     505,     0,    19,   277,   663,   278,     0,    19,   277,   663,
     276,   659,   278,     0,    19,   277,   276,   659,   278,     0,
     162,   168,   663,     0,     0,   177,   659,     0,   177,   690,
       0,     0,    23,   164,   691,     0,     0,   510,   277,   662,
     278,     0,   510,     0,   510,   193,   277,   662,   278,     0,
     509,   277,   662,   278,     0,   509,     0,   508,   277,   662,
     278,     0,   191,     0,    23,   193,     0,    91,   193,     0,
      23,     0,    91,     0,   121,     0,   119,    23,     0,   119,
      91,     0,    99,   512,     0,   513,   512,     0,     0,   277,
     664,   278,     0,   277,   664,   276,   664,   278,     0,    44,
       0,    92,     0,    95,   515,     0,    97,    95,   515,     0,
     152,     0,    93,   145,     0,   277,   660,   278,     0,     0,
     529,     0,   517,     0,   546,     0,   164,    71,   700,   182,
     664,     0,   164,    71,   700,   182,   231,     0,   164,    71,
     700,   182,   272,   231,     0,   519,     0,   520,     0,   522,
       0,   252,   712,     0,   253,   252,   712,   521,     0,   130,
       0,     0,   161,   526,   182,   523,   712,     0,   252,     0,
       0,    28,   526,   527,     0,   161,   526,     0,   201,     0,
       0,   155,   528,     0,     0,   170,     0,     0,   164,   183,
     530,     0,   531,     0,     0,   532,     0,   531,   532,     0,
     533,     0,   534,     0,   535,     0,   539,     0,   151,   130,
       0,   151,   202,     0,   196,     0,   123,   196,     0,    88,
     107,   536,     0,   536,     0,   537,     0,   151,   185,   538,
       0,   151,    29,   538,     0,   170,     0,   170,   180,     0,
     170,   180,   174,     0,   194,     0,   123,   194,     0,     0,
     154,   542,     0,   166,     0,   149,     0,     0,   151,     0,
     202,     0,   543,     0,   542,   276,   543,     0,   545,   544,
       0,    64,   540,   541,     0,     0,   584,     0,   545,   276,
     584,     0,   164,   176,    83,   701,     0,   548,   549,   555,
     556,   558,     0,   559,     0,   548,   186,   559,     0,   548,
     186,     6,   559,     0,   133,    20,   550,     0,     0,   551,
       0,   550,   276,   551,     0,   647,   552,   554,     0,    11,
       0,    48,     0,     0,   239,     0,   249,     0,   248,   563,
     553,   564,     0,     0,   267,   647,     0,   267,   647,   182,
     647,     0,     0,    64,   188,   557,     0,     0,   128,   619,
       0,     0,   200,   251,     0,     0,   163,   565,   568,   569,
     573,   590,   586,   589,   591,     0,   163,   565,   568,   569,
     573,   590,   586,   589,   591,   549,   555,     0,     0,     0,
       0,     0,   566,   567,   562,     0,   566,   562,     0,   567,
       0,     0,   239,   665,   561,     0,   239,   277,   647,   278,
     561,     0,   239,   654,   561,     0,   240,   665,     0,   240,
     277,   562,   647,   278,     0,   240,   654,     0,    49,     0,
     687,     0,   570,     0,   273,     0,   571,     0,   570,   276,
     571,     0,   610,     0,   610,   572,   702,     0,    10,     0,
       0,    66,   574,     0,   575,     0,   574,   276,   575,     0,
     580,     0,   581,     0,   576,     0,   277,   547,   278,   572,
     577,   578,     0,   706,     0,     0,   277,   579,   278,     0,
       0,   702,     0,   579,   276,   702,     0,   575,   585,    89,
     575,   129,   626,     0,   277,   580,   278,     0,   704,   582,
     572,   706,     0,   704,   582,     0,   277,   611,   278,     0,
       0,   584,     0,   707,   706,     0,   707,     0,    81,     0,
     105,     0,   105,   134,     0,   159,     0,   159,   134,     0,
      67,     0,    67,   134,     0,     0,    74,    20,   587,     0,
       0,   588,     0,   587,   276,   588,     0,   647,     0,    77,
     626,     0,     0,   198,   626,     0,     0,   142,   592,     0,
       0,   593,   277,   594,   278,     0,    89,     0,   172,   114,
       0,   114,     0,   172,     0,     0,   595,     0,   595,   276,
     594,     0,   596,   597,     0,   592,     0,   707,     0,   707,
     596,     0,   120,     0,    83,   277,   598,   278,     0,   133,
     701,   599,     0,   701,     0,   701,   276,   598,     0,    83,
     277,   598,   278,     0,     0,    84,    86,   584,   620,   190,
     277,   611,   278,     0,    84,    86,   584,   620,   560,     0,
     602,     0,   603,     0,    47,    66,   583,   590,     0,    47,
      66,   583,   604,     0,   198,    38,   128,   695,     0,   606,
       0,   607,     0,   188,   583,   164,   608,   590,     0,   188,
     583,   164,   608,   604,     0,   609,     0,   608,   276,   609,
       0,   625,   268,   610,     0,   647,     0,   688,     0,   610,
       0,   611,   276,   610,     0,   151,    19,   624,    66,   584,
     613,   615,     0,    84,    19,   624,    86,   584,   613,   615,
       0,    63,    66,   614,   182,   614,     0,    63,   182,   614,
       0,     0,   504,     0,   654,     0,   659,     0,   113,   616,
       0,     0,   663,     0,   654,     0,   618,     0,     0,   277,
     619,   278,     0,   624,     0,   619,   276,   624,     0,   621,
       0,     0,   277,   622,   278,     0,   625,     0,   622,   276,
     625,     0,   624,     0,   706,   282,   693,     0,   706,   282,
     273,     0,   693,     0,   624,     0,   706,   282,   693,     0,
     641,     0,   124,   641,     0,   628,     0,   626,   132,   626,
       0,   626,     8,   626,     0,   628,     0,   124,   641,     0,
     627,   132,   626,     0,   627,     8,   626,     0,   642,   132,
     626,     0,   642,     8,   626,     0,   629,     0,   277,   627,
     278,     0,   124,   628,     0,   630,     0,   633,     0,   634,
       0,   635,     0,   640,     0,   631,     0,   638,     0,   636,
       0,   637,     0,   639,     0,   647,   268,   647,     0,   647,
     269,   647,     0,   647,   270,   647,     0,   647,    70,   647,
       0,   647,   106,   647,     0,   647,   125,   647,     0,   647,
     126,   647,     0,   647,   122,   647,     0,   647,   268,     6,
     277,   645,   278,     0,   647,   269,     6,   277,   645,   278,
       0,   647,   270,     6,   277,   645,   278,     0,   647,    70,
       6,   277,   645,   278,     0,   647,   106,     6,   277,   645,
     278,     0,   647,   125,     6,   277,   645,   278,     0,   647,
     126,     6,   277,   645,   278,     0,   647,   122,     6,   277,
     645,   278,     0,   647,   268,   632,   277,   645,   278,     0,
     647,   269,   632,   277,   645,   278,     0,   647,   270,   632,
     277,   645,   278,     0,   647,    70,   632,   277,   645,   278,
       0,   647,   106,   632,   277,   645,   278,     0,   647,   125,
     632,   277,   645,   278,     0,   647,   126,   632,   277,   645,
     278,     0,   647,   122,   632,   277,   645,   278,     0,   171,
       0,     9,     0,   647,    18,   647,     8,   647,     0,   647,
     124,    18,   647,     8,   647,     0,   647,   108,   647,     0,
     647,   124,   108,   647,     0,   647,   108,   647,    57,   647,
       0,   647,   124,   108,   647,    57,   647,     0,   647,    79,
     643,     0,   647,   124,    79,   643,     0,   647,    34,   647,
       0,   647,   124,    34,   647,     0,   647,   175,   647,     0,
     647,   124,   175,   647,     0,   647,   175,   200,   647,     0,
     647,   124,   175,   200,   647,     0,    60,   277,   560,   278,
       0,   167,   277,   560,   278,     0,   647,    87,    98,     0,
     647,    87,   124,    98,     0,   256,     0,   257,     0,   258,
       0,   259,     0,   260,     0,   261,     0,   644,     0,   277,
     650,   278,     0,   277,   645,   278,     0,   163,   565,   568,
     647,   573,   590,   586,   589,   591,   549,   555,     0,   163,
     565,   568,   647,   573,   590,   586,   589,   591,   549,   555,
       0,   623,     0,   649,     0,   666,     0,   653,     0,   654,
       0,   430,     0,   675,     0,   676,     0,   674,     0,   272,
     647,     0,   271,   647,     0,   647,   271,   647,     0,   647,
      31,   647,     0,   647,    26,   692,     0,   647,   272,   647,
       0,   647,   273,   647,     0,   647,   274,   647,     0,   277,
     647,   278,     0,   277,   646,   278,     0,   655,     0,   656,
       0,   657,     0,    42,     0,   706,   282,    42,     0,   101,
       0,   648,     0,   228,     0,   229,     0,   230,     0,   623,
     280,   650,   281,     0,   647,     0,   650,   276,   647,     0,
     653,     0,   272,   652,     0,   205,     0,   204,     0,   203,
       0,   231,     0,   232,     0,   652,     0,   658,     0,    41,
     207,     0,   226,   207,     0,   227,   207,     0,   283,     0,
     189,     0,   233,     0,   234,     0,   241,     0,   242,     0,
      69,     0,   173,     0,   250,     0,   207,     0,   208,   207,
       0,   660,     0,   272,   661,     0,   204,     0,   204,     0,
     660,     0,   204,     0,   665,     0,   272,   665,     0,   204,
       0,   667,     0,   668,     0,   669,     0,   671,     0,    35,
     277,   273,   278,     0,    35,   277,   687,   647,   278,     0,
      35,   277,    49,   647,   278,     0,   179,   277,   687,   647,
     278,     0,   179,   277,    49,   647,   278,     0,    13,   277,
     687,   647,   278,     0,    13,   277,    49,   647,   278,     0,
     116,   277,   687,   647,   278,     0,   116,   277,    49,   647,
     278,     0,   112,   277,   687,   647,   278,     0,   112,   277,
      49,   647,   278,     0,    72,   277,   700,   276,   647,   278,
       0,   670,     0,   217,   277,   686,    66,   647,   278,     0,
     672,     0,   100,   277,   647,   278,     0,   236,   277,   647,
      66,   647,   673,   278,     0,    64,   647,     0,     0,   689,
     277,   650,   278,     0,   689,   277,   278,     0,    22,   277,
     610,    10,   377,   278,     0,   677,     0,   678,     0,   245,
     277,   647,   276,   647,   278,     0,   246,   277,   610,   276,
     611,   278,     0,   679,     0,   681,     0,   244,   684,   680,
      54,     0,   244,   684,   680,    53,   685,    54,     0,   197,
     683,   181,   685,     0,   680,   197,   683,   181,   685,     0,
     244,   682,    54,     0,   244,   682,    53,   685,    54,     0,
     197,   626,   181,   685,     0,   682,   197,   626,   181,   685,
       0,   647,     0,   647,     0,   610,     0,   218,     0,   219,
       0,   220,     0,   221,     0,   222,     0,   223,     0,   224,
       0,   225,     0,     6,     0,     0,    98,     0,   206,     0,
     713,     0,   713,     0,   713,     0,   713,     0,   713,     0,
     713,     0,   713,     0,   713,     0,   713,     0,   713,     0,
     713,     0,   713,     0,   713,     0,   713,     0,   713,     0,
     713,     0,   713,     0,   713,     0,   713,     0,   713,     0,
     713,     0,   713,     0,   713,     0,   206,     0,   714,     0,
     235,     0,   238,     0,   236,     0,   246,     0,   249,     0,
     255,     0,   251,     0,   245,     0,   248,     0,   254,     0,
     247,     0,   256,     0,   257,     0,   258,     0,   262,     0,
     263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   533,   535,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   565,   569,   573,   577,   581,   586,   587,   591,
     593,   595,   599,   600,   604,   608,   610,   612,   614,   616,
     620,   622,   626,   628,   632,   639,   644,   649,   654,   659,
     664,   669,   675,   679,   680,   682,   684,   688,   690,   692,
     694,   698,   699,   706,   708,   710,   714,   715,   719,   723,
     724,   728,   730,   737,   741,   743,   748,   754,   755,   757,
     763,   765,   766,   770,   771,   777,   780,   786,   787,   790,
     793,   797,   801,   804,   809,   818,   822,   825,   828,   830,
     832,   834,   836,   838,   840,   842,   844,   849,   853,   855,
     857,   867,   871,   873,   884,   886,   890,   892,   893,   899,
     905,   907,   909,   913,   915,   919,   921,   925,   927,   930,
     931,   938,   956,   958,   962,   964,   968,   970,   973,   974,
     977,   982,   983,   986,   990,   999,  1007,  1016,  1021,  1022,
    1025,  1031,  1033,  1036,  1037,  1041,  1043,  1045,  1047,  1049,
    1053,  1055,  1058,  1059,  1063,  1065,  1066,  1067,  1069,  1073,
    1075,  1077,  1079,  1083,  1088,  1095,  1101,  1108,  1115,  1148,
    1149,  1152,  1158,  1162,  1163,  1166,  1174,  1178,  1179,  1182,
    1183,  1186,  1188,  1192,  1193,  1194,  1197,  1198,  1199,  1205,
    1210,  1215,  1217,  1219,  1223,  1224,  1228,  1229,  1236,  1240,
    1244,  1253,  1259,  1260,  1263,  1265,  1270,  1272,  1277,  1283,
    1289,  1293,  1299,  1301,  1305,  1310,  1311,  1312,  1314,  1316,
    1320,  1322,  1325,  1326,  1330,  1335,  1337,  1342,  1343,  1345,
    1353,  1357,  1359,  1362,  1363,  1364,  1365,  1368,  1372,  1376,
    1383,  1390,  1395,  1401,  1404,  1406,  1408,  1410,  1414,  1417,
    1421,  1424,  1427,  1430,  1439,  1450,  1460,  1470,  1480,  1482,
    1486,  1488,  1492,  1493,  1497,  1502,  1504,  1508,  1509,  1513,
    1517,  1518,  1521,  1526,  1528,  1532,  1534,  1538,  1543,  1544,
    1547,  1551,  1553,  1555,  1559,  1560,  1564,  1565,  1566,  1567,
    1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,
    1578,  1579,  1581,  1583,  1584,  1587,  1589,  1593,  1597,  1602,
    1606,  1606,  1608,  1613,  1617,  1621,  1623,  1627,  1631,  1637,
    1642,  1647,  1649,  1651,  1655,  1657,  1659,  1663,  1664,  1665,
    1667,  1671,  1675,  1677,  1681,  1683,  1685,  1690,  1692,  1696,
    1697,  1701,  1706,  1707,  1711,  1713,  1715,  1717,  1721,  1722,
    1723,  1726,  1730,  1734,  1738,  1780,  1788,  1795,  1815,  1819,
    1821,  1823,  1827,  1838,  1842,  1843,  1847,  1848,  1856,  1859,
    1865,  1869,  1875,  1879,  1885,  1889,  1897,  1908,  1919,  1921,
    1923,  1927,  1931,  1933,  1937,  1939,  1941,  1943,  1945,  1947,
    1949,  1951,  1953,  1955,  1957,  1959,  1961,  1963,  1965,  1969,
    1971,  1975,  1981,  1985,  1988,  1995,  1997,  1999,  2002,  2005,
    2010,  2014,  2020,  2021,  2025,  2034,  2038,  2040,  2042,  2044,
    2046,  2050,  2051,  2055,  2057,  2059,  2061,  2067,  2070,  2072,
    2076,  2084,  2085,  2086,  2087,  2088,  2089,  2090,  2091,  2092,
    2093,  2094,  2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,
    2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,  2113,  2115,
    2119,  2121,  2126,  2132,  2134,  2136,  2140,  2142,  2149,  2155,
    2156,  2160,  2168,  2170,  2172,  2174,  2176,  2178,  2180,  2184,
    2185,  2192,  2202,  2203,  2207,  2208,  2214,  2218,  2220,  2222,
    2224,  2226,  2228,  2230,  2232,  2234,  2236,  2238,  2240,  2247,
    2248,  2251,  2252,  2255,  2260,  2267,  2268,  2272,  2281,  2287,
    2288,  2291,  2292,  2293,  2294,  2311,  2316,  2321,  2340,  2357,
    2364,  2365,  2372,  2376,  2382,  2388,  2396,  2400,  2406,  2410,
    2414,  2420,  2424,  2431,  2437,  2443,  2451,  2456,  2461,  2468,
    2469,  2470,  2473,  2474,  2477,  2478,  2479,  2486,  2490,  2501,
    2507,  2561,  2623,  2624,  2631,  2644,  2649,  2654,  2661,  2663,
    2670,  2671,  2672,  2676,  2681,  2686,  2697,  2698,  2699,  2702,
    2706,  2710,  2712,  2716,  2720,  2721,  2724,  2728,  2732,  2733,
    2736,  2738,  2742,  2743,  2747,  2751,  2752,  2756,  2757,  2761,
    2762,  2763,  2764,  2767,  2769,  2773,  2775,  2779,  2781,  2784,
    2786,  2788,  2792,  2794,  2796,  2800,  2802,  2804,  2808,  2812,
    2814,  2816,  2820,  2822,  2826,  2827,  2831,  2835,  2837,  2841,
    2842,  2847,  2855,  2859,  2861,  2863,  2867,  2869,  2873,  2874,
    2878,  2882,  2884,  2886,  2890,  2892,  2896,  2898,  2902,  2905,
    2913,  2917,  2919,  2923,  2925,  2929,  2931,  2938,  2950,  2964,
    2968,  2972,  2976,  2980,  2982,  2984,  2986,  2990,  2992,  2994,
    2998,  3000,  3002,  3006,  3008,  3012,  3014,  3018,  3019,  3023,
    3024,  3028,  3029,  3034,  3038,  3039,  3043,  3044,  3045,  3049,
    3054,  3055,  3059,  3061,  3065,  3066,  3070,  3072,  3076,  3079,
    3084,  3086,  3090,  3091,  3096,  3101,  3103,  3105,  3107,  3109,
    3111,  3113,  3115,  3122,  3124,  3128,  3129,  3135,  3138,  3140,
    3144,  3146,  3153,  3155,  3159,  3163,  3165,  3167,  3173,  3175,
    3179,  3180,  3184,  3186,  3189,  3190,  3194,  3196,  3198,  3202,
    3203,  3207,  3209,  3216,  3219,  3226,  3227,  3230,  3234,  3238,
    3245,  3246,  3249,  3254,  3259,  3260,  3264,  3268,  3269,  3272,
    3273,  3280,  3282,  3286,  3288,  3290,  3293,  3294,  3297,  3301,
    3303,  3306,  3308,  3314,  3315,  3319,  3323,  3324,  3329,  3330,
    3334,  3338,  3339,  3344,  3345,  3348,  3353,  3358,  3361,  3368,
    3369,  3371,  3372,  3374,  3378,  3379,  3381,  3383,  3387,  3389,
    3393,  3394,  3396,  3400,  3401,  3402,  3403,  3404,  3405,  3406,
    3407,  3408,  3409,  3414,  3416,  3418,  3420,  3422,  3424,  3426,
    3428,  3435,  3437,  3439,  3441,  3443,  3445,  3447,  3449,  3451,
    3453,  3455,  3457,  3459,  3461,  3463,  3465,  3469,  3470,  3476,
    3478,  3483,  3485,  3487,  3489,  3494,  3496,  3500,  3502,  3506,
    3508,  3510,  3512,  3516,  3520,  3524,  3526,  3530,  3535,  3540,
    3547,  3552,  3557,  3566,  3567,  3571,  3575,  3589,  3607,  3608,
    3609,  3610,  3611,  3612,  3613,  3614,  3615,  3616,  3618,  3620,
    3627,  3629,  3631,  3638,  3645,  3652,  3654,  3656,  3657,  3658,
    3659,  3661,  3663,  3667,  3671,  3687,  3703,  3707,  3711,  3712,
    3716,  3717,  3721,  3723,  3725,  3727,  3729,  3733,  3734,  3736,
    3752,  3768,  3772,  3776,  3778,  3782,  3786,  3789,  3792,  3795,
    3798,  3803,  3805,  3810,  3811,  3815,  3822,  3829,  3836,  3843,
    3844,  3848,  3854,  3855,  3856,  3857,  3860,  3862,  3864,  3867,
    3874,  3883,  3890,  3899,  3901,  3903,  3905,  3911,  3920,  3923,
    3927,  3928,  3932,  3941,  3943,  3947,  3949,  3953,  3959,  3960,
    3963,  3967,  3971,  3972,  3975,  3977,  3981,  3983,  3987,  3989,
    3993,  3995,  3999,  4002,  4005,  4008,  4010,  4012,  4014,  4016,
    4018,  4020,  4022,  4026,  4027,  4030,  4038,  4041,  4044,  4047,
    4050,  4053,  4056,  4059,  4062,  4065,  4068,  4071,  4074,  4077,
    4080,  4083,  4086,  4089,  4092,  4095,  4098,  4101,  4104,  4107,
    4112,  4113,  4118,  4120,  4121,  4122,  4123,  4124,  4125,  4126,
    4127,  4128,  4129,  4130,  4131,  4132,  4134,  4135
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
  "OPEN", "CLOSE", "FETCH", "ROWS", "'='", "'<'", "'>'", "'+'", "'-'", 
  "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", "'.'", 
  "'?'", "top", "statement", "grant", "prot_table_name", "privileges", 
  "privilege_list", "proc_privileges", "privilege", "grant_option", 
  "role_admin_option", "simple_proc_name", "revoke", "rev_grant_option", 
  "grantee_list", "grantee", "user_grantee_list", "user_grantee", 
  "role_name_list", "role_name", "role_grantee_list", "role_grantee", 
  "declare", "declare_clause", "udf_decl_clause", "udf_data_type", 
  "arg_desc_list1", "arg_desc_list", "arg_desc", "return_value1", 
  "return_value", "filter_decl_clause", "create", "create_clause", 
  "recreate", "recreate_clause", "replace", "replace_clause", 
  "unique_opt", "index_definition", "shadow_clause", "manual_auto", 
  "conditional", "first_file_length", "sec_shadow_files", "db_file_list", 
  "domain_clause", "as_opt", "domain_default_opt", 
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
  "input_parameters", "output_parameters", "proc_parameters", 
  "proc_parameter", "local_declaration_list", "local_declarations", 
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
  "invoke_procedure", "view_clause", "rview_clause", "union_view", 
  "union_view_expr", "select_view_expr", "from_view_clause", 
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
       0,   284,   284,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   285,   285,
     285,   285,   286,   286,   286,   286,   286,   287,   287,   288,
     288,   288,   289,   289,   290,   291,   291,   291,   291,   291,
     292,   292,   293,   293,   294,   295,   295,   295,   295,   295,
     295,   295,   296,   297,   297,   297,   297,   298,   298,   298,
     298,   299,   299,   300,   300,   300,   301,   301,   302,   303,
     303,   304,   304,   305,   306,   306,   307,   308,   308,   308,
     309,   309,   309,   310,   310,   311,   311,   312,   312,   313,
     313,   313,   313,   313,   314,   315,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   317,   318,   318,
     318,   319,   320,   320,   321,   321,   322,   322,   322,   323,
     324,   324,   324,   325,   325,   326,   326,   327,   327,   328,
     328,   329,   330,   330,   331,   331,   332,   332,   333,   333,
     334,   335,   335,   336,   337,   338,   339,   340,   341,   341,
     342,   343,   343,   344,   344,   345,   345,   345,   345,   345,
     346,   346,   347,   347,   348,   348,   348,   348,   348,   349,
     349,   349,   349,   350,   350,   351,   351,   352,   353,   354,
     354,   355,   356,   357,   357,   358,   359,   360,   360,   361,
     361,   362,   362,   363,   363,   363,   364,   364,   364,   365,
     366,   367,   367,   367,   368,   368,   369,   369,   370,   370,
     370,   371,   372,   372,   373,   373,   374,   374,   375,   376,
     377,   378,   379,   379,   380,   380,   380,   380,   380,   380,
     381,   381,   382,   382,   383,   384,   384,   384,   384,   384,
     385,   386,   386,   387,   387,   387,   387,   388,   389,   390,
     391,   391,   392,   393,   393,   393,   393,   393,   394,   395,
     396,   396,   396,   396,   397,   398,   399,   400,   401,   401,
     402,   402,   403,   403,   404,   405,   405,   406,   406,   407,
     408,   408,   409,   410,   410,   411,   411,   412,   413,   413,
     414,   415,   415,   415,   416,   416,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   417,   417,   417,   417,   417,
     417,   417,   417,   417,   417,   418,   418,   419,   420,   421,
     422,   422,   423,   424,   425,   426,   426,   427,   428,   429,
     430,   431,   431,   431,   432,   432,   432,   433,   433,   433,
     433,   434,   435,   435,   436,   436,   436,   437,   437,   438,
     438,   439,   440,   440,   441,   441,   441,   441,   442,   442,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     451,   451,   452,   453,   454,   454,   455,   455,   456,   456,
     457,   458,   459,   460,   461,   461,   462,   463,   464,   464,
     464,   465,   466,   466,   467,   467,   467,   467,   467,   467,
     467,   467,   467,   467,   467,   467,   467,   467,   467,   468,
     468,   469,   470,   471,   471,   471,   471,   471,   471,   471,
     472,   473,   474,   474,   475,   475,   475,   475,   475,   475,
     475,   476,   476,   477,   477,   477,   477,   477,   477,   477,
     478,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   480,   480,
     481,   481,   482,   483,   483,   483,   484,   484,   485,   486,
     486,   487,   487,   487,   487,   487,   487,   487,   487,   488,
     488,   489,   490,   490,   491,   491,   492,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   494,
     494,   495,   495,   496,   496,   497,   497,   498,   498,   499,
     499,   500,   500,   500,   500,   500,   500,   500,   500,   500,
     501,   501,   502,   502,   502,   502,   503,   503,   504,   504,
     504,   505,   505,   506,   506,   506,   507,   507,   507,   508,
     508,   508,   509,   509,   510,   510,   510,   511,   511,   512,
     512,   512,   513,   513,   514,   514,   514,   514,   515,   515,
     516,   516,   516,   517,   517,   517,   518,   518,   518,   519,
     520,   521,   521,   522,   523,   523,   524,   525,   526,   526,
     527,   527,   528,   528,   529,   530,   530,   531,   531,   532,
     532,   532,   532,   533,   533,   534,   534,   535,   535,   536,
     536,   536,   537,   537,   537,   538,   538,   538,   539,   540,
     540,   540,   541,   541,   542,   542,   543,   544,   544,   545,
     545,   546,   547,   548,   548,   548,   549,   549,   550,   550,
     551,   552,   552,   552,   553,   553,   554,   554,   555,   555,
     555,   556,   556,   557,   557,   558,   558,   559,   560,   561,
     562,   563,   564,   565,   565,   565,   565,   566,   566,   566,
     567,   567,   567,   568,   568,   569,   569,   570,   570,   571,
     571,   572,   572,   573,   574,   574,   575,   575,   575,   576,
     577,   577,   578,   578,   579,   579,   580,   580,   581,   581,
     582,   582,   583,   583,   584,   585,   585,   585,   585,   585,
     585,   585,   585,   586,   586,   587,   587,   588,   589,   589,
     590,   590,   591,   591,   592,   593,   593,   593,   593,   593,
     594,   594,   595,   595,   596,   596,   597,   597,   597,   598,
     598,   599,   599,   600,   600,   601,   601,   602,   603,   604,
     605,   605,   606,   607,   608,   608,   609,   610,   610,   611,
     611,   612,   612,   613,   613,   613,   614,   614,   504,   615,
     615,   616,   616,   617,   617,   618,   619,   619,   620,   620,
     621,   622,   622,   623,   623,   623,   624,   625,   625,   626,
     626,   626,   626,   626,   627,   627,   627,   627,   627,   627,
     628,   628,   628,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   630,   630,   630,   630,   630,   630,   630,
     630,   631,   631,   631,   631,   631,   631,   631,   631,   631,
     631,   631,   631,   631,   631,   631,   631,   632,   632,   633,
     633,   634,   634,   634,   634,   635,   635,   636,   636,   637,
     637,   637,   637,   638,   639,   640,   640,   641,   641,   641,
     642,   642,   642,   643,   643,   644,   645,   646,   647,   647,
     647,   647,   647,   647,   647,   647,   647,   647,   647,   647,
     647,   647,   647,   647,   647,   647,   647,   647,   647,   647,
     647,   647,   647,   647,   648,   648,   648,   649,   650,   650,
     651,   651,   652,   652,   652,   652,   652,   653,   653,   653,
     653,   653,   654,   655,   655,   656,   657,   657,   657,   657,
     657,   658,   658,   659,   659,   660,   661,   662,   663,   664,
     664,   665,   666,   666,   666,   666,   667,   667,   667,   667,
     667,   667,   667,   667,   667,   667,   667,   668,   669,   670,
     671,   671,   672,   673,   673,   674,   674,   675,   676,   676,
     677,   677,   678,   678,   679,   679,   680,   680,   681,   681,
     682,   682,   683,   684,   685,   686,   686,   686,   686,   686,
     686,   686,   686,   687,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   713,   714,   714,   714,   714,   714,   714,   714,   714,
     714,   714,   714,   714,   714,   714,   714,   714
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     7,     8,     6,     7,     5,     1,     2,     1,
       2,     1,     1,     3,     1,     1,     1,     1,     2,     2,
       3,     0,     3,     0,     1,     7,     8,     6,     7,     6,
       7,     4,     3,     1,     3,     3,     3,     2,     2,     2,
       2,     1,     3,     1,     2,     2,     1,     3,     1,     1,
       3,     1,     2,     2,     2,     3,     8,     1,     1,     5,
       0,     1,     3,     1,     3,     2,     4,     1,     3,     2,
       3,     4,     4,     2,     9,     2,     3,     7,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     2,     2,     1,     0,     1,     1,     6,     6,
       1,     1,     0,     0,     1,     0,     4,     0,     1,     1,
       2,     8,     1,     0,     3,     0,     0,     1,     1,     2,
       1,     1,     1,     2,     6,     1,     1,     3,     0,     1,
       1,     0,     1,     1,     2,     3,     4,     2,     2,     3,
       0,     1,     1,     2,     1,     1,     1,     4,     3,     3,
       3,     3,     3,     1,     1,     6,     3,     1,     3,     1,
       3,     2,     1,     0,     2,     3,     1,     0,     1,     1,
       2,     3,     4,     0,     1,     2,     0,     1,     1,     5,
       5,     3,     2,     0,     1,     3,     1,     1,     6,     3,
       2,     6,     2,     1,     2,     2,     2,     0,     1,     1,
       2,     0,     2,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     2,     2,     5,     1,     2,     3,
       2,     2,     0,     1,     1,     1,     1,     3,     4,     8,
       4,     0,     6,     1,     1,     2,     2,     0,     3,     3,
       1,     2,     2,     2,     8,     8,     8,     8,     3,     0,
       2,     0,     1,     3,     2,     1,     0,     1,     2,     4,
       1,     1,     3,     1,     0,     2,     1,     6,     1,     1,
       3,     1,     2,     0,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     2,     1,     1,
       1,     2,     2,     1,     2,     3,     4,     2,     6,     4,
       1,     1,     8,     9,     6,     8,     6,     4,     0,     4,
       2,     1,     3,     0,     2,     4,     0,     1,     1,     3,
       3,     7,     2,     0,     2,     2,     3,     3,     0,     1,
       2,     4,     1,     3,     2,     2,     2,     1,     1,     1,
       1,     2,     2,     5,     0,     4,     7,     7,     1,     1,
       3,     4,     7,     2,     1,     3,     1,     1,     6,     3,
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
       2,     2,     2,     2,     2,     4,     3,     2,     2,     1,
       3,     7,     1,     0,     1,     0,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     2,     2,     2,     1,
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
       3,     4,     5,     1,     3,     4,     3,     0,     1,     3,
       3,     1,     1,     0,     1,     1,     4,     0,     2,     4,
       0,     3,     0,     2,     0,     2,     0,     9,    11,     0,
       0,     0,     0,     3,     2,     1,     0,     3,     5,     3,
       2,     5,     2,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     0,     2,     1,     3,     1,     1,     1,     6,
       1,     0,     3,     0,     1,     3,     6,     3,     4,     2,
       3,     0,     1,     2,     1,     1,     1,     2,     1,     2,
       1,     2,     0,     3,     0,     1,     3,     1,     2,     0,
       2,     0,     2,     0,     4,     1,     2,     1,     1,     0,
       1,     3,     2,     1,     1,     2,     1,     4,     3,     1,
       3,     4,     0,     8,     5,     1,     1,     4,     4,     4,
       1,     1,     5,     5,     1,     3,     3,     1,     1,     1,
       3,     7,     7,     5,     3,     0,     1,     1,     1,     2,
       0,     1,     1,     1,     0,     3,     1,     3,     1,     0,
       3,     1,     3,     1,     3,     3,     1,     1,     3,     1,
       2,     1,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     1,     1,     5,
       6,     3,     4,     5,     6,     3,     4,     3,     4,     3,
       4,     4,     5,     4,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,    11,    11,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     4,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     4,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     6,     1,     6,
       1,     4,     7,     2,     0,     4,     3,     6,     1,     1,
       6,     6,     1,     1,     4,     6,     4,     5,     3,     5,
       4,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   589,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   589,   666,     0,     0,     0,     0,     0,
       1,    10,    15,     7,     6,    13,    14,    12,     3,     9,
      19,   571,    17,   576,   577,   578,     5,    16,   570,   572,
      18,   637,   633,    11,     8,   745,   746,    20,   750,   751,
       4,   478,     0,     0,     0,     0,     0,     0,   412,   588,
     591,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,    95,   643,   925,     0,    21,   923,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,     0,    29,    37,    34,
      36,   774,    35,   774,  1010,  1012,  1014,  1013,  1019,  1015,
    1022,  1020,  1016,  1018,  1021,  1017,  1023,  1024,  1025,  1026,
    1027,     0,    31,     0,    32,     0,    66,    68,  1002,  1011,
       0,     0,     0,     0,     0,     0,     0,     0,   587,     0,
       0,   984,   660,   665,     0,     0,   596,     0,   702,   704,
    1004,     0,     0,     0,   107,   579,  1009,     0,     2,     0,
       0,   650,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
     440,   441,     0,   994,   419,     0,   998,   416,   269,  1001,
       0,   704,   415,   390,  1005,   593,   586,   921,     0,   103,
     151,   150,   104,   133,   218,   786,   990,     0,   102,   145,
     997,     0,    98,   269,   105,   927,   122,    99,   203,   100,
       0,   101,   774,  1008,   106,   146,   641,   642,     0,   926,
     924,     0,    74,     0,   995,   721,   504,   993,   497,     0,
     503,   508,   498,   499,   506,   500,   501,   502,   507,   333,
      30,     0,    39,   773,    38,     0,     0,     0,     0,     0,
       0,   779,     0,     0,     0,     0,     0,     0,     0,   585,
     931,     0,   912,   659,   659,   660,   672,   670,   983,   673,
       0,   674,   664,   660,     0,     0,     0,     0,     0,     0,
     612,   605,   594,   595,   597,   599,   600,   601,   608,   609,
     602,     0,   703,  1003,   108,   269,   109,   203,   110,   774,
     582,     0,     0,   634,     0,   652,     0,     0,     0,     0,
       0,   417,   479,   382,     0,   382,     0,   221,   418,   422,
     413,   476,   477,     0,   271,   242,   468,     0,   414,   431,
       0,   388,   389,   493,   592,   590,   922,   148,   148,     0,
       0,     0,   160,   152,   153,   132,     0,    96,     0,     0,
     111,   271,   121,   120,   123,     0,     0,     0,     0,     0,
     986,    75,   221,   540,     0,   747,   748,   505,     0,     0,
       0,     0,   890,   918,     0,   985,     0,   892,     0,     0,
     919,     0,   913,   904,   903,   902,  1010,     0,     0,     0,
     894,   895,   896,   905,   906,   914,   915,  1014,   916,   917,
       0,  1019,  1015,   920,     0,     0,     0,     0,   873,   365,
     759,   331,   868,   783,   757,   893,   869,   907,   871,   872,
     887,   888,   889,   908,   870,   932,   933,   934,   948,   935,
     950,   876,   874,   875,   958,   959,   962,   963,   758,     0,
       0,   990,     0,   776,     0,     0,    27,    33,     0,     0,
      43,    69,    71,  1006,    67,     0,     0,     0,   778,     0,
      52,     0,     0,     0,     0,    51,   584,     0,     0,     0,
     669,   667,     0,   676,     0,   675,   677,   682,   663,     0,
     631,     0,   606,   617,   603,   617,   604,   618,   624,   628,
     629,   613,   598,   721,   754,   787,     0,     0,   271,     0,
       0,   581,   580,   636,   638,   643,   635,   648,     0,   656,
     186,   177,     0,   481,   484,   174,   173,   129,     0,   487,
     482,     0,   488,   148,   148,   148,   148,   489,   483,   480,
       0,   426,     0,   428,   427,     0,   429,     0,   423,     0,
       0,   272,   219,   269,     0,     0,   435,     0,   436,   382,
     469,     0,   475,     0,     0,     0,   472,   990,   393,   392,
     492,     0,   410,   149,     0,     0,   158,     0,   157,     0,
       0,   147,   161,   162,   166,   165,   164,   154,   540,   552,
     527,   562,   530,   553,   563,     0,   569,   531,     0,   559,
       0,   554,   566,   526,   549,   528,   529,   524,   382,   509,
     510,   511,   519,   525,   512,   521,   542,     0,   547,   544,
     522,   559,   523,   112,   269,   113,     0,     0,   124,     0,
       0,   202,   242,   390,   380,     0,   221,     0,    81,    83,
       0,     0,     0,   768,     0,     0,     0,     0,   857,   858,
     859,     0,   720,   791,   800,   803,   808,   804,   805,   806,
     810,   811,   809,   812,   807,   789,     0,   984,     0,   984,
     909,     0,     0,   984,   984,   984,     0,   910,   911,     0,
       0,   973,     0,     0,     0,     0,   878,   877,   666,     0,
       0,   757,   330,  1007,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   775,    28,     0,     0,    44,
      72,     0,     0,    26,   765,     0,   781,   666,     0,   744,
     765,     0,     0,     0,     0,   583,     0,   659,     0,     0,
     721,     0,   681,     0,   574,     0,   573,   929,     0,   607,
       0,   615,   611,   610,     0,   621,     0,   626,   614,     0,
     752,   753,     0,     0,     0,   242,   380,     0,   647,     0,
     654,     0,   632,     0,     0,     0,   130,   187,   486,     0,
       0,     0,     0,     0,   382,   425,     0,   382,   383,   430,
     519,   542,   274,     0,   268,   270,   380,   241,   991,   213,
     210,     0,   223,   382,   509,   380,     0,     0,     0,   240,
     243,   244,   245,   246,     0,   434,   474,   473,   433,   432,
       0,     0,     0,   396,   394,   395,   391,     0,   382,   196,
     155,   159,     0,     0,   163,     0,   537,   550,   551,   567,
       0,   564,   569,     0,   557,   555,   556,   135,     0,     0,
       0,   520,     0,     0,     0,     0,   558,   271,     0,   380,
     125,   201,     0,   204,   206,   207,     0,     0,     0,     0,
     221,   221,    78,     0,    85,    77,   538,   539,   987,   540,
       0,     0,     0,   802,   790,     0,     0,   860,   861,   862,
       0,   794,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   975,   976,   977,   978,   979,
     980,   981,   982,     0,     0,     0,     0,   968,     0,     0,
       0,     0,     0,   984,   332,   886,   885,   760,   898,     0,
     881,   989,   880,   879,   882,   883,   884,   956,     0,   891,
     785,   784,   777,     0,     0,     0,     0,     0,     0,    24,
      53,    41,    61,    63,     0,     0,    70,     0,   770,     0,
     780,   984,     0,   770,    49,    47,     0,     0,     0,   668,
     671,     0,   683,   684,   688,   686,   687,   701,     0,   714,
     678,   680,   999,   575,   930,   616,   625,   620,   619,     0,
     630,   755,   756,   788,   380,     0,     0,   639,   661,   640,
     649,     0,   651,   655,   176,   183,   182,     0,   179,   485,
     148,   193,   178,   188,   189,   170,   169,   928,   172,   171,
     490,     0,     0,     0,   424,   273,   276,   212,   382,     0,
     383,   214,   209,   215,     0,     0,   251,     0,   438,   383,
     382,   380,   437,     0,     0,     0,   409,   495,   197,   198,
     156,     0,   168,     0,     0,     0,   542,     0,   565,     0,
       0,   382,   383,     0,   515,   517,     0,     0,     0,     0,
       0,     0,     0,   390,   276,   148,   127,   242,   199,   410,
     984,   385,   368,   369,     0,    82,     0,   221,     0,    87,
       0,    84,     0,     0,     0,   749,   992,     0,     0,   795,
       0,     0,   801,     0,     0,   793,   792,     0,   847,     0,
     838,   837,     0,   816,     0,   845,   863,   855,     0,     0,
       0,   817,   841,     0,     0,   820,     0,     0,     0,     0,
       0,     0,     0,   818,     0,     0,   819,     0,   849,     0,
       0,   813,     0,     0,   814,     0,     0,   815,     0,     0,
     221,     0,   936,     0,     0,   951,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   974,     0,     0,   972,     0,
       0,   964,     0,     0,     0,     0,     0,   897,   955,    65,
      57,    58,    64,    59,    60,     0,     0,     0,    22,    25,
      41,    42,   540,   540,     0,   762,   782,     0,     0,   761,
      50,    48,    45,     0,     0,   712,   686,     0,   710,   705,
     706,   708,     0,     0,   699,     0,   719,   622,   623,   627,
     276,   200,   385,     0,   653,   181,     0,     0,     0,   194,
       0,   190,     0,     0,     0,   420,   229,   224,   900,   225,
     226,   227,   228,   284,     0,   275,   277,     0,   222,   230,
       0,   251,   643,   247,     0,   439,   470,   471,   401,   402,
     398,   397,   400,   399,   382,   494,   383,   167,   988,     0,
       0,   533,     0,   532,   568,     0,   560,     0,   136,     0,
     513,     0,   541,   542,   548,   546,     0,   543,   380,     0,
       0,     0,   119,   128,   205,   382,     0,     0,   381,     0,
      97,     0,   117,   116,    93,     0,     0,    89,     0,    86,
       0,   853,   854,   797,   796,   799,   798,     0,     0,     0,
     666,     0,     0,   856,     0,     0,     0,     0,     0,     0,
     848,   846,   842,     0,   850,     0,     0,     0,     0,   851,
       0,     0,     0,     0,     0,     0,   942,   941,     0,     0,
     938,   937,     0,   946,   945,   944,   943,   940,   939,     0,
     954,   970,   969,     0,     0,     0,     0,     0,     0,     0,
     899,    54,    55,     0,    56,    62,    23,   766,     0,   767,
     764,   769,   772,   771,     0,   743,     0,    46,   682,   697,
     685,   711,   707,   709,     0,     0,     0,     0,     0,   723,
       0,   381,   644,   645,   662,   148,   184,   180,     0,   196,
     195,   191,     0,   383,   901,   283,     0,   343,   383,   278,
     383,   217,   231,   232,   382,     0,   248,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   491,   535,     0,   536,
       0,   134,     0,   217,   137,   138,   140,   141,   142,   516,
     518,   514,   545,   276,   410,   383,   196,     0,     0,     0,
     366,     0,   370,   382,    88,     0,     0,    90,   542,     0,
     839,     0,     0,   984,   865,   864,     0,     0,   843,     0,
       0,     0,     0,   852,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   957,   220,   947,   949,     0,     0,
     971,   966,   965,     0,   960,   961,   721,    40,   540,   721,
     691,   712,   700,   698,   713,   715,   717,   718,   729,   657,
     383,   367,   646,     0,   175,   192,   383,   144,     0,     0,
     280,   281,     0,   990,     0,     0,     0,     0,     0,     0,
       0,  1012,  1017,     0,     0,   364,   294,   289,     0,   343,
     288,   298,   299,   300,   301,   302,   309,   310,   303,   305,
     306,   308,     0,   313,     0,   358,   360,   359,     0,     0,
       0,     0,     0,     0,   990,   267,     0,     0,   208,   233,
       0,     0,     0,   251,   234,   237,   774,     0,   383,   407,
     408,   404,   403,   406,   405,     0,   534,   561,   143,   131,
     139,     0,   382,   264,   126,   383,     0,   721,   384,   371,
       0,     0,    91,    92,    79,     0,   824,   832,     0,   825,
     833,   828,   836,   840,   844,   826,   834,   827,   835,   821,
     829,   822,   830,   823,   831,   953,   952,   967,   714,   763,
     714,   693,   690,     0,     0,   725,   727,   728,   722,     0,
     265,   185,   421,   223,   279,     0,   317,     0,     0,   320,
     321,     0,   312,     0,   328,   311,   344,   345,     0,  1000,
     361,   362,     0,   290,     0,   295,   292,   349,     0,     0,
     314,     0,   304,   297,   307,   296,   342,   211,   216,   235,
     251,   774,   238,   257,   250,   252,   411,   383,     0,   386,
       0,   373,   374,   376,   377,   714,   383,    76,    94,     0,
     719,   719,     0,   689,   696,   716,   726,   729,     0,   286,
     282,     0,   315,     0,   333,     0,     0,     0,   346,     0,
     357,     0,     0,     0,     0,   352,   350,     0,     0,     0,
     337,     0,   338,     0,   239,   257,     0,   251,   253,   254,
     266,   383,   712,   376,     0,     0,   719,     0,   721,   723,
     723,     0,   694,   733,     0,   730,     0,   734,   285,     0,
     316,   336,     0,   319,     0,   327,     0,   356,   355,   996,
     354,   343,     0,     0,     0,     0,   329,     0,     0,   251,
       0,     0,   249,     0,   256,     0,   255,   387,   379,   375,
       0,   372,   118,   714,   637,   637,     0,   692,   724,   729,
       0,   736,     0,   732,   735,     0,     0,     0,     0,   343,
     363,   351,   353,     0,   348,     0,   340,   339,   236,     0,
       0,   260,   259,   258,   712,   719,   650,   650,   695,   731,
       0,   742,   287,     0,   334,   318,   324,   326,     0,     0,
       0,   343,   263,   261,   262,     0,   723,   867,   658,     0,
     739,     0,   738,     0,   343,     0,     0,   343,   341,   378,
     637,   737,     0,     0,   335,   325,   343,   347,   322,   650,
     740,     0,   323,   866,   741,     0,     0,     0
};

static const short yydefgoto[] =
{
    1905,    20,    21,   465,   121,   122,   123,   124,  1208,   723,
     718,    22,   136,   969,   970,   971,   972,   125,   126,   470,
     471,    23,    81,   381,   874,   647,   648,   649,  1108,  1109,
     242,    24,    73,    25,   154,    26,   370,    74,  1320,   224,
     374,   639,  1096,  1312,   533,   212,   366,  1082,  1463,  1464,
    1465,  1466,  1467,  1468,   218,   234,   209,   584,   210,   362,
     363,   364,   591,   592,   593,   594,   595,   535,   536,   537,
    1027,  1024,  1025,  1245,  1426,   538,  1032,  1033,  1034,  1250,
    1070,   227,   316,   376,   862,   863,   864,   800,   801,   802,
    1598,   567,   559,  1368,   650,  1050,  1255,  1441,  1442,  1443,
    1604,   865,   569,   809,   810,   811,   812,  1273,   813,  1767,
    1768,  1769,  1852,   222,   314,   633,   197,   344,   564,   560,
     561,  1264,  1265,  1266,  1549,  1550,  1436,  1740,  1551,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1681,  1575,
    1576,  1577,  1578,  1579,  1747,  1580,   428,   429,  1837,  1761,
    1581,  1582,  1583,  1870,  1696,  1697,  1754,  1755,  1584,  1585,
    1586,  1587,  1692,    27,   231,   318,  1101,  1102,  1103,  1627,
    1721,  1722,  1723,   867,  1480,   552,  1044,  1318,   229,   635,
     353,   580,   581,   826,   828,  1285,    28,    58,   788,   785,
     338,   339,   348,   349,   189,   190,   350,  1059,   575,   818,
     194,   162,   331,   332,   548,   202,   582,  1286,    29,    95,
     618,   619,   620,  1083,  1084,   621,   790,   623,   624,  1076,
    1397,   851,   625,   791,   627,   628,   629,   630,   844,   631,
     632,   841,    30,    31,    32,    33,    34,   522,    35,   487,
      36,    37,    60,   206,   355,    38,   302,   303,   304,   305,
     306,   307,   308,   309,   752,   310,  1009,  1239,   507,   508,
     757,   509,    39,  1588,    41,   161,   523,   524,   238,  1424,
    1019,   325,   529,  1022,   772,    42,   729,   490,   292,  1243,
    1542,   141,   142,   143,   290,   494,   495,   496,   743,   740,
     992,   993,   994,  1661,  1733,  1781,   995,   996,  1234,  1724,
     148,  1232,  1236,  1534,  1535,  1419,   385,  1539,  1783,  1669,
    1784,  1785,  1786,  1833,  1879,  1882,  1589,  1590,    45,    46,
     386,  1591,    48,    49,   513,  1592,   430,   431,    50,   978,
    1398,  1215,  1401,   262,   263,   462,   477,   478,   725,   432,
     433,   516,   662,   890,   663,   664,   665,   666,  1132,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   892,  1135,
    1136,  1341,   700,   676,   435,   436,   949,  1257,   437,   438,
     439,   440,   441,   442,   443,   653,    78,   240,   226,  1038,
    1085,   747,   444,   445,   446,   447,   448,   449,   450,  1519,
     451,   452,   453,   454,   455,   456,   940,   457,   692,  1189,
     693,  1186,   933,   291,   458,   459,   877,  1287,   950,   215,
     797,  1115,   246,   192,   243,  1798,   219,  1880,  1001,  1593,
     997,   127,   460,   201,   203,   973,   702,   232,   155,   461,
     129
};

static const short yypact[] =
{
    6478,   786,   251,  1471,   317,   853,   421,  1601,   350,  2322,
     426,   554,  1932,   251,    15,   404,  5356,   845,  5356,   340,
     454,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   101,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  6736,  5356,  5356,  5356,  5356,  5356,-32768,-32768,
     473,   715,  5356,  5356,  5356,   650,  5356,   540,  5356,  5356,
  -32768,  5356,  5356,-32768,   452,-32768,   572,-32768,-32768,   794,
    5356,-32768,  5356,  5356,  5356,   814,  5356,  5356,  5356,  5356,
     540,  5356,  5356,  5356,  5356,-32768,  5356,   622,-32768,-32768,
  -32768,   547,-32768,   547,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   718,   636,   790,-32768,   157,-32768,-32768,-32768,-32768,
    5356,  5356,  5356,   777,   823,   835,  1195,   111,   792,   494,
     496,   711,   728,-32768,  5356,   914,  1056,   830,-32768,  5356,
  -32768,  5356,  5356,  5356,-32768,-32768,-32768,  5356,-32768,   982,
     183,   743,   268,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   319,
  -32768,-32768,   715,-32768,-32768,   219,-32768,-32768,   737,-32768,
     729,-32768,-32768,   222,-32768,   849,-32768,-32768,   816,-32768,
     573,-32768,-32768,  1039,-32768,-32768,-32768,   715,-32768,-32768,
  -32768,   320,-32768,   737,-32768,-32768,   428,-32768,   995,-32768,
     999,-32768,   547,-32768,-32768,-32768,-32768,-32768,   993,-32768,
  -32768,   881,-32768,  1015,-32768,   906,-32768,-32768,-32768,   881,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  4811,
  -32768,  5356,-32768,-32768,-32768,  3375,   647,   964,  3406,  5356,
    1030,   874,  1057,  1062,  3375,   979,  1025,  1035,  3406,   928,
  -32768,  6219,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    4475,-32768,-32768,-32768,   989,  5356,  1095,  1013,   397,  5356,
    1033,-32768,-32768,  1056,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  5356,-32768,-32768,-32768,   737,-32768,   995,-32768,   547,
    1085,  6219,  1064,-32768,  6219,  1155,   118,   963,    80,   968,
     565,   268,-32768,  1202,   850,-32768,  5356,-32768,   319,-32768,
  -32768,-32768,-32768,  5356,  1082,  2110,  1026,  2561,   985,-32768,
    6736,-32768,-32768,   241,-32768,-32768,-32768,   976,   976,   715,
    1140,   715,   382,   573,-32768,-32768,  2604,-32768,  5356,  5356,
  -32768,  1082,-32768,-32768,  1231,   395,   987,  5356,  1256,  5356,
  -32768,-32768,    49,   392,  3825,-32768,-32768,-32768,   991,   992,
     994,  1065,-32768,-32768,   997,-32768,  1001,-32768,  1004,  1010,
  -32768,  1014,-32768,-32768,-32768,-32768,  1019,  1020,  1069,  1092,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1028,-32768,-32768,
    4908,  1036,  1037,-32768,  6219,  6219,  4572,  5356,-32768,-32768,
  -32768,  1041,  1038,-32768,  1063,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1042,
    1024,  1040,   623,-32768,  5356,  1133,-32768,-32768,  5356,  5356,
     322,-32768,-32768,-32768,-32768,  5356,  5356,    98,-32768,  5356,
  -32768,  1255,  5356,  3375,  1175,  1048,-32768,  5356,  5147,   353,
  -32768,-32768,  6219,-32768,  1265,  1066,-32768,   139,-32768,   495,
  -32768,    88,-32768,   370,-32768,   370,-32768,  1067,-32768,   109,
  -32768,  1158,-32768,    -1,-32768,-32768,  1071,  1058,  1082,  1068,
    1334,-32768,-32768,  1070,-32768,   388,-32768,  1055,  1159,  1157,
  -32768,   153,  1266,  1268,-32768,-32768,-32768,-32768,   715,-32768,
  -32768,  1269,-32768,   976,   976,   976,   976,-32768,  1083,-32768,
    1341,-32768,  1342,-32768,-32768,  1321,-32768,  2604,-32768,  2604,
     697,-32768,-32768,   737,  1359,  5356,-32768,  6553,-32768,   459,
  -32768,  5356,   779,   729,  1188,  1156,  1228,  1191,-32768,-32768,
  -32768,   256,  1232,-32768,  1170,   540,-32768,   715,-32768,  1353,
    1284,-32768,   382,-32768,-32768,-32768,-32768,-32768,   375,  1187,
  -32768,-32768,-32768,  1189,-32768,  1234,  1104,-32768,  1289,  1110,
     683,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1108,-32768,-32768,-32768,   133,  1112,  1113,    27,
  -32768,  1110,-32768,-32768,   737,-32768,  1328,  1383,-32768,   715,
     715,-32768,  2110,   222,-32768,  1267,-32768,  1238,  1124,-32768,
    6828,  5031,  1273,-32768,  1274,  1126,  3911,  1127,   863,   977,
    1186,  3574,   192,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1408,   787,  5233,   132,
  -32768,  5356,  6219,   802,   839,   846,  1131,-32768,-32768,  6219,
    4150,  1063,   209,  1204,  6219,  5233,   167,   167,    15,   731,
    1132,   468,-32768,-32768,  5233,  6219,  5356,  6219,  6219,  6219,
    6219,  6219,  5472,  1694,  5356,-32768,-32768,  2999,  1227,-32768,
  -32768,  1201,  3406,-32768,  1349,   756,-32768,    15,  1137,-32768,
    1349,  2999,  1350,  1354,  5356,-32768,   468,-32768,   544,  4037,
    1221,  5233,-32768,  5356,-32768,    39,-32768,-32768,   197,-32768,
    1230,-32768,-32768,-32768,  5356,   708,  5356,-32768,-32768,  5356,
  -32768,-32768,  5233,  5356,  1411,  2110,-32768,  6219,  1177,  6219,
    1294,  1172,-32768,   715,   715,   715,-32768,   378,-32768,  1170,
    1170,  1233,   540,   565,-32768,-32768,  1161,-32768,-32768,-32768,
  -32768,  1412,-32768,  5356,-32768,-32768,-32768,-32768,-32768,  1420,
  -32768,  1164,  1397,-32768,  1414,-32768,  1360,  1361,   547,-32768,
  -32768,-32768,-32768,-32768,  1425,-32768,-32768,-32768,-32768,-32768,
    5356,  6654,   540,  1320,  1329,  1331,-32768,   540,-32768,   807,
  -32768,-32768,  1300,   715,-32768,   362,  1291,-32768,-32768,-32768,
     540,-32768,  1104,   506,-32768,-32768,-32768,  1419,   506,  1303,
     506,-32768,   540,   540,  1192,   540,-32768,  1082,  5356,-32768,
    1368,-32768,   757,-32768,-32768,-32768,   241,  1308,  5356,   763,
     170,-32768,-32768,  1196,  1452,-32768,-32768,-32768,-32768,   392,
    5356,  1311,  4236,-32768,-32768,  1311,  3911,-32768,-32768,-32768,
     142,-32768,   196,  1307,  4150,  4150,  6219,  6219,  1819,  1198,
     140,  2211,  6219,  2503,   701,  2763,  2860,  5558,  3120,  3217,
    3477,  6219,  6219,  1466,  6219,  1205,  6219,  1209,   560,  6219,
    6219,  6219,  6219,  6219,  6219,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1413,  1174,   182,  5233,-32768,  4150,  6219,
     214,   270,  1210,   711,-32768,-32768,-32768,-32768,  1063,   332,
  -32768,-32768,  1454,   167,   167,   662,   662,-32768,   767,-32768,
  -32768,-32768,-32768,  5356,  5356,  5356,  5356,  5356,  5356,  1212,
  -32768,   394,-32768,-32768,  2999,  1358,-32768,   234,  1377,  5356,
  -32768,   711,  5233,  1377,  1212,  1215,  2999,  2668,  1415,-32768,
  -32768,  2043,  1216,   772,-32768,-32768,-32768,  1217,  4150,  1422,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    93,
  -32768,-32768,-32768,-32768,-32768,   806,  1308,-32768,-32768,-32768,
    1063,  5356,-32768,-32768,-32768,-32768,-32768,   832,-32768,-32768,
     976,  1486,-32768,   378,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1222,  4150,  2409,-32768,-32768,  1459,-32768,-32768,  2409,
  -32768,-32768,-32768,-32768,   547,   547,  1258,  1235,-32768,-32768,
  -32768,-32768,-32768,   125,   147,   766,-32768,  1496,-32768,-32768,
  -32768,  5356,-32768,   317,   852,  1345,  1412,  1239,-32768,  1170,
     872,-32768,-32768,   626,-32768,  1240,  5356,   637,  1242,  1245,
     540,  1247,  1508,   222,  1459,   976,  1268,  2110,-32768,  1232,
     711,  1326,  1355,-32768,  2009,-32768,   540,  1388,  1484,-32768,
    6828,-32768,   540,  1302,  1488,-32768,-32768,  1270,  1280,-32768,
    4150,  4150,-32768,  4150,  4150,-32768,  1536,   177,  1063,  1279,
  -32768,-32768,  1282,  1063,  5797,-32768,-32768,-32768,  1448,  1286,
    1287,  1063,  1229,  1288,  1290,  1063,  6219,  6219,  1198,  6219,
    5883,  1292,  1293,  1063,  1295,  1296,  1063,  6219,  1063,  1297,
    1305,  1063,  1309,  1310,  1063,  1312,  1313,  1063,   592,   613,
  -32768,   632,-32768,   654,  6219,-32768,   689,   727,   744,   847,
     904,   950,  6219,  6219,  5233,-32768,  1493,   193,  1063,  1380,
    5233,-32768,  6219,  6219,  5233,  6219,  6219,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  2999,  1495,  2999,-32768,  1212,
     394,-32768,   328,   328,    28,-32768,-32768,  4475,   877,-32768,
    1212,  1215,  1315,  2668,  1306,   602,  1314,  4037,  1432,-32768,
    1437,  1460,  1506,  5233,   911,  1576,  1520,-32768,-32768,-32768,
    1459,-32768,  1326,   368,  1323,  1433,   715,  1464,  1170,  1465,
    1170,-32768,  4150,   145,   875,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1416,  1587,  1459,-32768,  6219,-32768,   490,
    1453,  1258,   452,-32768,  4150,-32768,-32768,-32768,  1473,  1475,
    1477,  1478,  1480,  1481,-32768,-32768,-32768,-32768,-32768,  1336,
     317,-32768,  1233,-32768,-32768,   506,-32768,  2409,   195,   506,
  -32768,   506,-32768,  1412,-32768,-32768,  1337,-32768,-32768,   241,
    1587,  1170,-32768,  1268,-32768,-32768,  4475,  1592,-32768,   190,
  -32768,  1347,-32768,  1323,-32768,  1340,   715,   155,  1344,-32768,
     715,-32768,-32768,-32768,  1536,-32768,  1536,  6219,  1463,  1463,
      15,  1352,   885,-32768,  1463,  1463,  6219,  1463,  1463,   245,
    1063,-32768,  1236,  6219,  1063,  1463,  1463,  1463,  1463,  1063,
    1463,  1463,  1463,  1463,  1463,  1463,-32768,-32768,  1357,  2604,
  -32768,-32768,   960,-32768,-32768,-32768,-32768,-32768,-32768,  1011,
     920,-32768,-32768,  5233,  5233,  1578,  1456,  1029,   894,  1264,
    1063,-32768,-32768,  1502,-32768,-32768,-32768,-32768,  1457,-32768,
  -32768,-32768,-32768,-32768,  1265,-32768,  2668,  1315,  1628,-32768,
     772,-32768,-32768,-32768,  4037,   908,  5356,  6219,  4150,  1498,
    1587,-32768,-32768,-32768,-32768,   976,-32768,-32768,   715,   807,
  -32768,-32768,   149,-32768,-32768,-32768,  5356,  4710,-32768,-32768,
    1063,  1615,   490,-32768,   722,  5356,-32768,  1559,   151,  1455,
    1560,  1458,  1598,  1563,  1602,  1459,-32768,-32768,  1370,-32768,
    1372,-32768,  1556,  1615,   195,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1459,  1232,-32768,   807,  1496,  1590,  1529,
  -32768,  1308,-32768,-32768,-32768,  1544,   216,-32768,  1412,  1550,
    1063,  1392,  1393,   711,-32768,-32768,  1395,  1396,  1063,  1409,
    1410,  6219,  6219,  1063,  1418,  1421,  1423,  1424,  1426,  1428,
    1429,  1430,  1431,  1434,-32768,-32768,-32768,-32768,  6219,  1435,
  -32768,-32768,-32768,  5233,-32768,-32768,  1221,-32768,   328,  1221,
    5356,   939,-32768,-32768,  1399,-32768,  1063,   192,   576,-32768,
  -32768,-32768,-32768,  1170,-32768,-32768,-32768,-32768,  2604,  1417,
  -32768,-32768,  1647,  1650,  4354,   329,  1439,  1438,  1604,  6219,
    1441,  1442,  4966,  5356,  5356,-32768,-32768,-32768,  1639,  3712,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   131,-32768,  1443,-32768,-32768,-32768,  1612,  1444,
    1445,  1446,  1450,  1447,   513,-32768,  1449,  5356,-32768,-32768,
    1632,  1620,  5356,  1258,-32768,-32768,   547,  5356,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1587,-32768,-32768,-32768,-32768,
  -32768,  1587,-32768,-32768,-32768,-32768,  4294,  1221,-32768,-32768,
    6219,   715,-32768,-32768,-32768,   715,-32768,-32768,  6219,-32768,
  -32768,-32768,-32768,  1063,  1063,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1063,-32768,-32768,  1422,-32768,
    1422,  1461,-32768,  4150,  6219,-32768,-32768,  1617,-32768,  1462,
  -32768,-32768,-32768,   134,-32768,  1669,-32768,  6122,  5356,-32768,
  -32768,  6219,-32768,  4150,  1063,-32768,-32768,-32768,  1469,-32768,
  -32768,-32768,  5356,-32768,   179,-32768,  1537,-32768,   259,  1470,
  -32768,  3969,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1258,   547,-32768,  1606,-32768,-32768,-32768,-32768,  1496,-32768,
    4294,  1474,   772,-32768,-32768,  1422,  1063,-32768,-32768,  1264,
    1520,  1520,  5356,-32768,   192,-32768,-32768,  3305,  2409,-32768,
  -32768,  1476,-32768,   867,  4811,   665,   159,  1479,-32768,  1651,
  -32768,  5356,  5356,   317,   116,-32768,-32768,   430,  1654,  4150,
  -32768,   673,-32768,  1482,-32768,  1606,   200,  1258,  1623,  1626,
  -32768,-32768,   602,  1468,  4294,  1652,  1520,  1483,  1221,  1498,
    1498,   919,-32768,-32768,  1489,  1492,   759,  5356,-32768,  1311,
  -32768,  1600,  3969,-32768,  1577,-32768,  3969,-32768,-32768,-32768,
  -32768,  6357,   179,  6219,  3969,   161,-32768,  3969,  5291,  1258,
     586,   586,-32768,  1713,-32768,  1574,-32768,-32768,-32768,   772,
    4294,-32768,-32768,  1422,  1636,  1636,  5356,-32768,-32768,  3305,
    1494,-32768,  5356,-32768,-32768,  1497,  2918,  1499,   704,  6357,
    1500,-32768,-32768,  1278,   141,  1720,-32768,-32768,-32768,  1564,
     184,-32768,-32768,-32768,   946,  1520,   743,   743,-32768,-32768,
    5356,  1696,-32768,  3969,  1500,-32768,-32768,  1724,  3969,  1741,
    1733,  6357,-32768,-32768,-32768,  4150,  1498,-32768,-32768,  1509,
    1510,  1507,-32768,   930,  6357,   121,  5356,  6357,-32768,   192,
    1636,-32768,  5356,  5356,-32768,-32768,  6357,-32768,-32768,   743,
  -32768,  1511,-32768,-32768,-32768,  1788,  1790,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -83,   206,-32768,   254,  1525,   583,-32768,
    -322,-32768,-32768,  -576,  -157,  -642, -1029,  1782,  1526,  1519,
    1076,-32768,-32768,-32768,   690,-32768,  1160,   931,-32768,   696,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   712,-32768,-32768,-32768,-32768,-32768,
     343,-32768,-32768,  1485,-32768,-32768,-32768,  -329,-32768,-32768,
  -32768,  1472,-32768,-32768,  1218,  -235,  1487,-32768,-32768,  -341,
  -32768,  -710,-32768,-32768,-32768,-32768,-32768,-32768,   771,-32768,
    -727,-32768,-32768,  1504,  1044,   719,  1467,  1018,   720,-32768,
     356,   -51,-32768,-32768,  -303,   154,  -981,-32768,-32768,   384,
  -32768,  1491, -1067,-32768,-32768,-32768,-32768, -1172,   385,    66,
      65,    69,    29,-32768,-32768,-32768,-32768,   -84,  -285,-32768,
    1045,  -960,-32768,   574,-32768,-32768,-32768,-32768,-32768, -1415,
   -1131,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    85,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768, -1195,    99,-32768,  -593,
  -32768,-32768,-32768,-32768,-32768,   148,-32768,    43,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   831,-32768, -1133,-32768,
  -32768, -1088,   128,  -660,   429,  -254,  -930,   610,-32768,-32768,
    -538,  -750,-32768,-32768,  -972, -1315,-32768,-32768,-32768,-32768,
  -32768,  1515,-32768,  1283,  1505,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1528,-32768,-32768,-32768,-32768,-32768,-32768,
    -509,  -527,-32768,  1007,   564,  -559,  -343,-32768,-32768,-32768,
    -314,  -948,-32768,  -340,-32768,-32768,-32768,-32768,  1237,-32768,
  -32768,  1023,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1853,-32768,-32768,-32768,-32768,-32768,  1566,-32768,
  -32768,-32768,  1366,-32768,  1365,-32768,-32768,-32768,-32768,  1117,
  -32768,-32768,-32768,     5,-32768, -1101,-32768,  1105,  -500,-32768,
  -32768, -1607,-32768,-32768,-32768,    50,  -846,  -183,   535,-32768,
  -32768,  -637,-32768,  1731,  -871,  -828,-32768,  1135, -1047, -1243,
  -32768,  -880,-32768,-32768,-32768,-32768,   883,-32768,-32768,   569,
      32, -1138, -1447,-32768,   215, -1205,  -498, -1124,   342,-32768,
      53,-32768,   102,-32768,  -839,-32768,  1886,  1890,-32768,-32768,
    1381,  1893,-32768,-32768,-32768,  -202,  -270,  -404,-32768,  1165,
   -1082,   913,-32768,   -91,  -661,  -415,-32768,-32768,-32768, -1125,
     -38,  -411,  -659,-32768,  -491,-32768,-32768,-32768,   164,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -482,-32768,   751,
  -32768,   639,-32768,  1203,  -947,-32768,  -597,-32768,   648,  -935,
    -122,  -931,  -924,  -919,   -10,    -2,     3,-32768,   -54,  -732,
    -447,  -126,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   709,
  -32768, -1026,-32768,   504,  -917,   631,-32768,   817,   307,  -697,
    1335, -1323,-32768,   -57,  1823,-32768,   -46,   -50, -1279,   354,
     -47,   -44,  -139,   -15,   -36,  -161,-32768,   -56,   -20,    -9,
  -32768
};


#define	YYLAST		7071


static const short yytable[] =
{
     128,   149,    77,   128,   195,    40,   217,   150,   198,   156,
     312,   213,   264,   284,   287,   760,   961,   283,   286,   223,
     497,   596,   699,   622,   214,   768,   626,   248,   235,   585,
     789,   935,   792,   230,   557,  1117,   254,   257,   252,  1118,
     804,   251,   253,   191,   193,   196,   199,   150,   204,   259,
     258,   211,   746,   216,   193,   220,   256,   199,   803,   150,
     204,   943,   233,   128,  1028,   726,  1013,   149,  1268,   652,
     225,   244,  1195,   150,   247,   193,   255,   244,   220,   196,
     199,   555,   150,   204,   233,   128,   637,   199,   200,   985,
     981,   875,   270,   225,   272,   547,  1256,   319,   294,  1446,
     228,   491,  1256,  1074,   315,   866,  1016,   472,  1258,   514,
    1217,  1225,  1259,  1552,  1258,   958,  1099,   472,  1259,  1260,
    1269,   216,   150,   216,  1261,  1260,  1262,  1315,  1293,  1275,
    1261,  1400,  1262,  1438,  1310,   220,  1046,   320,   288,   371,
     313,   378,   199,   150,   233,  1053,  1526,  1056,   156,   742,
    1120,  1869,  1298,   894,  1695,   984,   849,   894,  1381,   894,
     732,  1529,  1625,   271,  1385,   883,  1801,   894,   773,   894,
     891,  1896,   517,   755,   884,  1486,  1392,   278,  1395,  1475,
    1049,   914,   340,   540,   317,  1337,  1482,  1416,  1750,   322,
     894,   481,   776,   706,  1280,  1698,  1481,   384,   707,  1094,
     894,   894,  1444,   706,  1123,  -679,   -80,   367,   707,  1278,
     323,  1730,   530,  1731,   779,   780,   781,   782,   134,  -382,
     854,   531,   341,   463,   622,   351,   503,   626,   520,  1316,
    1873,   518,  1037,   764,   159,  1125,  1126,  1751,  1137,   748,
    1690,  1691,   999,   280,  1237,   500,   578,  1810,  1752,  1877,
    1878,   596,   216,  1501,   139,   140,   150,   579,   300,   473,
     128,   727,   936,   937,  1138,   150,   135,  1190,  1191,   473,
    1003,   706,   326,   515,  1121,   759,   707,   895,  1776,  1187,
    1420,   895,  1874,   895,   836,   327,   196,   160,   728,  1540,
     150,   895,  1903,   895,  1060,  1238,   706,   466,   556,   342,
    1212,   707,   352,   823,   855,   562,   466,   214,   720,   572,
    1106,   282,   576,  1279,   895,   814,  1461,  1632,  1757,  1462,
     328,   634,   329,   333,   895,   895,   646,   216,  1124,   645,
    1699,   510,  1210,   636,   216,  1281,   216,   517,   216,   268,
     824,   577,   276,   541,  1221,  1222,    14,  1410,  1629,   586,
    1256,   588,  1753,  1100,  1240,  1471,  1456,  1520,  1521,   199,
     204,  1530,  1258,  1184,   847,   641,  1259,  1487,   150,  1749,
     196,   334,   526,  1260,  1383,  1444,  1855,  1395,  1261,   706,
    1262,   532,   505,  1253,   707,   756,  1841,   269,  1811,  1404,
     277,   883,  1802,  1270,  1271,   883,  1080,  1807,  1209,   236,
     733,  1277,  1738,  1771,  1119,   915,   938,   543,   913,   643,
    1220,  1192,   988,   850,   706,  -679,  1213,  1807,   703,   707,
    1122,   719,    14,  1433,  1867,   942,   503,  1546,   589,  1608,
     774,  1712,   330,   269,   947,   719,   237,  1794,   515,  1845,
     710,   711,   372,  1322,   825,   130,  1659,  1107,   708,   709,
     710,   711,    59,  1782,  1633,   150,  1888,   544,   829,   199,
     473,  1333,  1334,   236,  1335,  1336,   150,   735,   368,  1895,
     150,   497,  1898,   199,   150,   144,   530,  1678,   156,   795,
    1030,  1902,  1403,   335,  1716,   531,  1778,    82,  1478,   640,
    1717,   545,  1012,   750,   706,  1615,   716,  1657,    96,   707,
     237,   336,  1622,  1547,   369,   651,  1760,   724,  1595,   546,
    1596,   730,   131,  1621,  -242,   466,   708,   709,   710,   711,
    1679,    75,   721,   565,   806,  1779,  1780,   504,   777,   805,
    1041,   830,    75,  1043,  1531,   337,  1427,  1342,  1764,   373,
    1634,   708,   709,   710,   711,  1623,  1193,  1858,  1040,  1051,
     857,   875,   651,  1031,   989,  1309,   798,  1011,   216,  1474,
    1459,   472,   798,  1897,   751,  1114,  1037,  1110,  1216,   651,
     706,  1821,  1092,   132,  1067,   707,  1762,   831,  1218,    75,
     145,  1407,   505,  1680,  1775,   147,   706,   146,   225,    76,
     543,   707,   157,  1432,  1206,  1812,    75,  1760,   722,   506,
      76,  1760,   207,   208,   214,   807,  1244,  1422,  1196,  1760,
    1670,   282,  1846,  1197,  -242,  1448,  1672,  1423,   706,  1004,
     517,  1679,  1638,   707,   708,   709,   710,   711,   205,   860,
     861,   737,  1772,   216,  1775,   917,  -242,  1848,  1073,   706,
     544,  1760,   878,  -242,   707,   590,   808,    76,  1473,   876,
    1876,   245,   835,  1035,  1036,  1824,  1825,   221,   706,   708,
     709,   710,   711,   707,    76,  1665,  1185,  1762,  1760,  1228,
    1207,  1762,   220,  1760,   545,   357,   962,  -242,  1715,  1762,
     706,  1775,  1847,  1229,  1680,   707,  1819,   719,   706,  1323,
    1666,   706,   546,   707,    98,  1719,   707,   951,   280,   280,
     280,  1248,  1545,  1493,   216,   216,   845,  1230,   473,  1849,
     280,  1762,   358,   473,   359,   706,  1775,   288,  1544,  1146,
     707,   515,   473,  1856,  1857,   199,   744,   214,  1039,   158,
     199,   100,  1854,   345,  1002,  1147,   346,   360,  1762,   708,
     709,   710,   711,  1762,    75,   150,   946,   150,  1667,  1624,
    1850,  1792,  1890,   706,   216,   562,   216,  1788,   707,  1537,
     289,  1231,   361,  1026,  1026,  1029,  1311,   745,  1062,   260,
     706,   281,  1447,   285,   846,   707,   239,   282,  1079,   282,
    1148,   347,  1058,  1061,   216,   225,   510,  1770,  1010,  1899,
    1388,  1256, -1000,   288,  1267, -1003,  1777,  1851,  1088,  1089,
     101,  1091,  1199,  1258,  1110,  1202,  1276,  1259,   288,  1149,
     102,   216,   216,  1282,  1260,   708,   709,   710,   711,  1261,
     492,  1262,   990,  1072,   261,   225,    51,  1297,   498,  1415,
    1066,   708,   709,   710,   711,   103,   911,    52,  1175,  1228,
     517,  1817,  1830,  1077,    53,   288,  1600,   265,  1460,   150,
    1283,   919,   288,  1229,  1470,   225,   225,  1007,   225,   150,
    1515,  -712,   241,   708,   709,   710,   711,  1369,  1601,    54,
    1366,  1116,   382,   706,  1008,  1602,  1150,  1230,   707,  1831,
     387, -1023,   249,   553,   708,   709,   710,   711,   921, -1023,
    1093,  1367,  1832,   706, -1023,   923,   554, -1023,   707,   714,
    1104,   715,  1299,   708,   709,   710,   711,  1300,   273,  1603,
    1370,  1203,   266,  1299,  1185,    79,    80,  1200,  1303,   267,
    1185,   742,   207,   208,  1204,   708,   709,   710,   711,  1201,
     706,  1231,  1371, -1023,    55,   707,   708,   709,   710,   711,
    1793,   515, -1023,  1835,  1068,  1069,   706,   497,  1806,  1807,
   -1023,   707,   274,  1004,   473,   199,   204,   473,   233,   128,
     708,   709,   710,   711,   275,   473,    56,  1373,   140, -1023,
      57, -1023,   776,   793,   279,   794,   706,   473,   473,  1866,
    1807,   707,   199,   463,  1518, -1023,   706, -1023, -1023, -1023,
     816,   707,   817,   151,   311, -1024,  1224,   295,   708,   709,
     710,   711,   321, -1024,  1734,  1374,  1228,   704, -1024,   944,
     324, -1024,   216,  1228,   343,   708,   709,   710,   711,   354,
    1229,  1673,  1375,   356,  1746,   152,   622,  1229,  1658,   626,
    1455,  1660,   979,  1097,   980,  1098,  1306,   706, -1023,   871,
     153,  1105,   707,  1196,  1230,  1198,   497, -1024,  1391,   365,
    1394,  1230,  1324,  1900,  1901,   706, -1024,   375,  1328,   214,
     707,  1477,  1288,   377, -1024,  1140,   463,  1144,  1663,  1152,
    1155,  1289,  1160,  1163,  1166,  1875,   379,  1288,   403,   404,
     405,   706,  1097, -1024,  1241, -1024,   707,   380,   216,   706,
    1399,  1399,  1402,   225,   707,   216,  1543,   383,  1231, -1024,
    1805, -1024, -1024, -1024,   384,  1231,   413,   414,  1246,   225,
    1247,   961,   468,  1185,  1185,   225,   475,  -682,   708,   709,
     710,   711,  1429,   479,  1431,  1376,   480,   482,  1290,  1725,
    1291, -1023, -1023, -1023, -1023, -1023, -1023, -1023,   708,   709,
     710,   711,  1790, -1023,   296, -1023,  -682,  -682,  1295,  -682,
    1296,   476, -1024,   704,   483,  1405,  -682,  -682,  -682,  -682,
    -682,  1196,  -682,  1495,   484,  -682,  -682,  -682,  -682,  -682,
     704,   499,  1525,  -682,  -682,   708,   709,   710,   711,   297,
     486,   912,  1377,   916,   704,  1476,  1532,   920,   922,   924,
     814,   708,   709,   710,   711,  1826,   473,  1827,   473,  1838,
     706,    97,   501,  1840, -1025,   707,  1807,   298,  1894,   502,
     299,  1844, -1025,   511,   473,   521,  1889, -1025,   199,   528,
   -1025,   708,   709,   710,   711,   539,   300,    14,  1378,  1630,
     542,   708,   709,   710,   711,   550,  1026,   769,  1516,   563,
    1183,   570,    98,  1864,   583, -1024, -1024, -1024, -1024, -1024,
   -1024, -1024,   301,  1185,    99,   706, -1025, -1024,   587, -1024,
     707,   573,   706,   638,   642, -1025,   644,   707,   677,   678,
    1883,   679,   680, -1025,   681,  1885,   687,  1533,   682,   100,
    1823,   683,   708,   709,   710,   711,  1346,   684,  1458,  1517,
     706,   685, -1025,  1502, -1025,   707,  -986,   686,   517,   688,
     708,   709,   710,   711,   706,   689,   713,  1524, -1025,   707,
   -1025, -1025, -1025,   694,   695,   717,  1485,   704,   705,   712,
    1489,   731, -1003,   734,   722,   896,   708,   709,   710,   711,
     739,   739,   758,   706,   708,   709,   710,   711,   707,   762,
     763,   897,   741,   754,   766,   765,   767,   770,   101,   925,
     926,   927,   928,   929,   930,   931,   932,   771,   102,   783,
     775, -1025,   530,   778,  1868,   784,   786,   787,  1718,   796,
     820,   822,   821,  -441,   280,   827,   832,   898,   833,   839,
     837,   840,   838,   103,   842,  1548,   899,   843,   848,   852,
     853,  1662,   858,   859,   900,   870,   868,   473,   214,   515,
     871,   939,   880,   881,   885,   199,  1399,   313,   879,   974,
     945,   975,   977,   901,   982,   902,   986,  1671,  1026,   998,
     987,  1014,  1021,  1023,  1005,  1018,   896,  1553,  1594,   903,
     517,   904,   905,   906,   706,   849,   150,  1037,  1042,   707,
    1047,  1048,   897,  1049,   799,   708,   709,   710,   711,  1057,
    1054,  1055,  1063,  1075, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025,  1064,   434,  1065,  1071,  1081, -1025,  1086, -1025,  1090,
    1095,  1100,  1113,  1112,   727,  1134,  1170,  1606,   898,  1182,
     706,  1223,   907,  1172,   489,  1174,  1194,   899,  1205,  1211,
    1214,  1207,  1227,   434,  1233,   900,  1235,  1677,  1249,  1252,
     708,   709,   710,   711,  1263,  1272,  1284,   708,   709,   710,
     711,    61,  1274,  1292,   901,  1713,   902,  1294,  1308,  1301,
    1304,   313,    62,  1305,   525,  1307,  1317,   527,  1106,    63,
     903,   515,   904,   905,   906,   708,   709,   710,   711,  1326,
    1329,  1319,    64,  1330,   894,   193,  1343,  1382,  1331,   708,
     709,   710,   711,  1689,  1116,  1116,  1338,  1714,  1332,  1339,
    1594,  1384,  1763,  1344,  1345,  1347,  1411,  1348,  1393,  1355,
    1356,  1412,  1357,  1358,  1360,   908,   909,   910,   708,   709,
     710,   711,  1361,   907,  1408,   946,  1362,  1363,   951,  1364,
    1365,  1406,  1409,   150,  1413,  1414,  1417,  1418,   196,   714,
    1428,  1425,  1430,    65,  1437,  1449,  1445,  1450,  1435,  1451,
    1452,   149,  1453,  1454,  1457,  1472,  1479,   150,  1484,    66,
    1765,  1727,  1488,   691,  1483,  1728,  1340,   696,   697,   701,
    1494,  1744,  1522,  1527,  1711,  1514,    67,  1523,   742,  1528,
    1538,  1597,  1607,  1609,  1610,  1612,  1611,  1613,  1616,  1614,
    1617,    68,    83,  1763,  1618,    69,  1626,  1763,    70,    84,
    1628,  1631,   517,    85,    86,  1763,    71,  1635,  1763,   199,
    1636,  1637,    87,  1639,  1640,  1664,   908,   909,   910,   708,
     709,   710,   711,  1116,    88,    72,  1675,  1641,  1642,  -992,
     131,   736,  1674,  1693,  1797,   738,  1645,  1763,  1701,  1646,
     517,  1647,  1648,  1758,  1649,   149,  1650,  1651,  1652,  1653,
    1710,   150,  1654,  1656,  1682,  1683,  1685,  1686,  1700,  1702,
    1703,  1704,  1787,  1002,  1763,  1705,  1706,  1707,   150,  1763,
    1709,  1736,   517,  1741,  1694,  1766,   959,  1796,  1732,  1737,
    1804,  1820,   193,  1799,  1748,   517,  1818,  1759,   517,    89,
    1774,  1800,  1813,  1789,  1795,  1815,  1836,   517,  1839,   149,
    1810,  1822,  1811,   515,  1808,   150,    90,  1828,  1829,   159,
    1871,  1860,  1787,  1872,  1865,  1862,  1807,  1884,   150,  1881,
    1886,    91,  1861,  1887,  1893,    92,  1892,  1891,  1906,  1904,
    1907,   467,  1594,  1396,   137,   474,    93,   485,   976,   216,
    1327,   515,  1111,  1325,  1251,   149,   869,  1620,  1313,  1015,
     834,   150,   566,   534,  1787,    94,  1314,  1002,   551,  1619,
     150,   519,  1052,   196,  1321,  1129,  1599,  1739,  1130,  1605,
    1594,  1809,   388,   515,  1816,   597,   568,  1814,  1045,  1439,
    1853,   389,  1803,  1791,  1756,  1842,   515,  1242,  1773,   515,
    1541,   196,  1421,   558,   390,   574,   819,  1087,   515,   549,
     391,   392,  1594,  1469,   893,  1078,   138,   749,   856,   512,
     753,  1006,  1017,   293,  1226,  1594,  1000,  1116,  1594,  1735,
    1668,   434,  1859,   196,   196,   918,    43,  1594,   393,  1834,
      44,   394,   934,    47,   761,   983,  1219,   941,   434,  1351,
     104,  1386,  1434,  1302,  1708,     0,   815,   434,   948,   250,
     952,   953,   954,   955,   956,   948,  1688,     0,     0,   396,
     397,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     106,   398,   107,     0,     0,   399,     0,     0,    97,   108,
     109,   110,   111,   112,   434,   113,     0,     0,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,   434,     0,   960,     0,     0,
     525,     0,  1020,     0,     0,     0,     0,  1491,  1492,    98,
       0,     0,     0,  1496,  1497,     0,  1499,  1500,     0,     0,
    1131,    99,   400,     0,  1504,  1505,  1506,  1507,   401,  1508,
    1509,  1510,  1511,  1512,  1513,   133,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,   403,   404,   405,   406,   207,   208,     0,     0,
       0,     0,     0,     0,     0,     0,   407,     0,     0,   799,
       0,     0,     0,     0,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   105,   417,     0,   107,     0,     0,
     418,   419,     0,   420,   421,   422,   110,   111,   112,   423,
     113,     0,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,   101,     0,     0,     0,     0,
     424,   425,     0,     0,     0,   102,   488,     0,   427,  1127,
    1128,  1133,   282,     0,  1141,  1142,  1145,     0,  1153,  1156,
    1158,  1161,  1164,  1167,  1168,  1169,     0,  1171,     0,  1173,
     103,     0,  1176,  1177,  1178,  1179,  1180,  1181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,   434,
       0,     0,  1188,   565,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   106,     0,
     107,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   112,     0,   113,     0,   434,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,  1139,     0,     0,
    1130,     0,     0,     0,   388,     0,     0,     0,     0,     0,
       0,     0,     0,   389,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   106,   390,   107,     0,   104,
       0,     0,   391,   392,   108,   109,   110,   111,   112,     0,
     113,     0,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,     0,     0,     0,   105,   106,
     393,   107,     0,   394,     0,     0,   261,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,   396,   397,     0,     0,     0,   104,     0,     0,     0,
     991,     0,     0,   398,     0,     0,     0,   399,    97,     0,
       0,     0,     0,     0,     0,     0,     0,   948,     0,     0,
       0,     0,     0,     0,     0,   105,   106,     0,   107,  1349,
    1350,     0,  1352,  1354,     0,   108,   109,   110,   111,   112,
    1359,   113,     0,     0,   114,   115,   116,   117,   118,    98,
       0,     0,   119,   120,     0,     0,     0,  1372,     0,     0,
       0,    99,  1131,     0,   400,  1379,  1380,   434,     0,     0,
     401,     0,     0,   434,     0,  1188,  1387,   434,  1389,  1390,
     402,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,   403,   404,   405,   406,   207,   208,
     434,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,   434,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   105,   417,     0,   107,
     391,     0,   418,   419,     0,   420,   421,   422,   110,   111,
     112,   423,   113,     0,     0,   114,   115,   116,   117,   118,
    1440,     0,     0,   119,   120,   101,     0,     0,   393,     0,
       0,     0,   424,   425,     0,   102,     0,     0,   488,     0,
     427,     0,     0,     0,   282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,  1143,
     103,     0,  1130,     0,     0,     0,   388,     0,     0,   434,
       0,     0,     0,     0,     0,   389,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   390,     0,
    1490,     0,     0,     0,   391,   392,     0,     0,     0,  1498,
       0,     0,     0,     0,     0,     0,  1503,   105,   106,     0,
     107,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   112,   393,   113,     0,   394,   114,   115,   116,   117,
     118,     0,   400,     0,   119,   120,   434,   434,     0,     0,
       0,     0,     0,     0,   571,     0,     0,     0,   402,     0,
       0,     0,     0,   396,   397,     0,     0,     0,     0,     0,
       0,     0,   403,   404,   405,   398,   207,   208,     0,   399,
    1536,     0,     0,   598,     0,     0,     0,   599,     0,     0,
       0,     0,     0,     0,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,     0,   600,     0,     0,   601,     0,
     418,   419,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1131,     0,   400,     0,     0,     0,
       0,  1254,   401,     0,     0,     0,     0,     0,     0,   602,
       0,     0,   402,     0,     0,   603,   604,   605,     0,   606,
     607,   608,     0,   609,  1643,  1644,   403,   404,   405,   406,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
     407,  1655,     0,   610,     0,   611,   434,     0,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   105,   417,
       0,   107,   963,     0,   418,   419,     0,   420,   421,   422,
     110,   111,   112,   423,   113,     0,   612,   114,   115,   116,
     117,   118,  1684,     0,     0,   119,   120,   104,     0,  1151,
       0,     0,  1130,   613,   424,   425,   388,     0,     0,     0,
     488,     0,   427,     0,     0,   389,   282,     0,     0,     0,
       0,     0,     0,     0,     0,   614,   105,   106,   390,   107,
       0,     0,     0,     0,   391,   392,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
     615,   616,   393,  1726,     0,   394,     0,     0,     0,     0,
       0,  1729,     0,     0,     0,     0,     0,   617,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   966,     0,     0,
       0,     0,     0,   396,   397,     0,  1154,  1536,     0,  1130,
       0,     0,     0,   388,   104,   398,     0,     0,     0,   399,
    1743,     0,   389,     0,  1745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,   391,   392,   105,   106,     0,   107,     0,     0,     0,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,   393,
     119,   120,   394,     0,  1131,     0,   400,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,   434,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
     396,   397,     0,     0,     0,     0,   403,   404,   405,   406,
     207,   208,   398,     0,     0,     0,   399,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   105,   417,
       0,   107,     0,     0,   418,   419,  1843,   420,   421,   422,
     110,   111,   112,   423,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,  1131,     0,   400,   424,   425,     0,     0,     0,   401,
     488,     0,   427,     0,     0,     0,   282,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   404,   405,   406,   207,   208,     0,
       0,     0,     0,   963,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   105,   417,     0,   107,     0,
       0,   418,   419,     0,   420,   421,   422,   110,   111,   112,
     423,   113,     0,     0,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,   104,     0,  1159,     0,     0,  1130,
       0,   424,   425,   388,     0,     0,     0,   488,     0,   427,
       0,     0,   389,   282,     0,     0,     0,   964,     0,     0,
       0,     0,     0,   105,   106,   390,   107,     0,     0,     0,
       0,   391,   392,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,   965,     0,     0,     0,     0,   966,   393,
       0,     0,   394,     0,   967,  1863,     0,   427,     0,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,   968,     0,     0,     0,     0,     0,     0,
     396,   397,     0,  1162,     0,     0,  1130,     0,     0,     0,
     388,     0,   398,     0,   105,   106,   399,   107,     0,   389,
       0,     0,     0,     0,   108,   109,   110,   111,   112,     0,
     113,     0,   390,   114,   115,   116,   117,   118,   391,   392,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,   394,
       0,  1131,     0,   400,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,   396,   397,     0,
       0,     0,     0,   403,   404,   405,   406,   207,   208,   398,
       0,     0,     0,   399,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   105,   417,     0,   107,     0,
       0,   418,   419,     0,   420,   421,   422,   110,   111,   112,
     423,   113,     0,     0,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,     0,     0,     0,     0,  1131,     0,
     400,   424,   425,     0,  1665,     0,   401,   488,     0,   427,
       0,     0,     0,   282,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1666,
     403,   404,   405,   406,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   105,   417,     0,   107,     0,     0,   418,   419,
       0,   420,   421,   422,   110,   111,   112,   423,   113,     0,
       0,   114,   115,   116,   117,   118,     0,  1667,     0,   119,
     120,     0,     0,  1165,     0,     0,  1130,     0,   424,   425,
     388,     0,     0,     0,   488,     0,   427,     0,     0,   389,
     282,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,   390,     0,     0,     0,     0,     0,   391,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,   107,     0,     0,   393,     0,     0,   394,
     108,   109,   110,   111,   112,   464,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,   396,   397,     0,
       0,   104,     0,     0,     0,     0,     0,   388,     0,   398,
       0,     0,     0,   399,     0,   469,   389,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     105,   106,   104,   107,     0,   391,   392,     0,     0,     0,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,   655,     0,     0,   119,   120,     0,
       0,   105,   106,   393,   107,     0,   394,     0,  1131,     0,
     400,   108,   109,   110,   111,   112,   401,   113,     0,     0,
     114,   115,   116,   117,   118,     0,   402,     0,   119,   120,
       0,     0,     0,     0,   396,   397,     0,     0,     0,     0,
     403,   404,   405,   406,   207,   208,   398,     0,     0,     0,
     399,     0,     0,     0,   407,     0,     0,     0,   886,     0,
       0,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   105,   417,     0,   107,     0,     0,   418,   419,
       0,   420,   421,   422,   110,   111,   112,   423,   113,  1437,
       0,   114,   115,   116,   117,   118,     0,   698,     0,   119,
     120,   657,     0,     0,     0,     0,     0,   400,   424,   425,
       0,     0,     0,   401,   488,     0,   427,     0,     0,     6,
     282,     0,     0,   402,     0,     0,  -291,     0,     0,     0,
    1554,  1555,     0,  1556,     0,     0,     0,   403,   404,   405,
     406,   207,   208,     0,     0,     0,     0,     0,     0,     0,
    1557,   407,     0,     0,     0,     0,  1558,     0,     0,     0,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   105,
     417,     0,   107,     0,     0,   418,   419,     0,   420,   421,
     422,   110,   111,   112,   423,   113,     0,     0,   114,   115,
     116,   117,   118,   887,   888,   889,   119,   120,   388,     0,
       0,     0,     0,     0,     0,   424,   425,   389,     0,     0,
       0,   661,     0,   427,     0,     0,  1559,   282,     0,     0,
     390,     0,     0,   654,     0,     0,   391,   392,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   655,     0,     0,     0,     0,
    1560,     0,     0,     0,   393,     0,     0,   394,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,  1694,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,   388,   396,   397,     0,     0,     0,
       0,     0,     0,   389,     0,     0,     0,   398,     0,     0,
       0,   399,     0,     0,     0,     0,   390,  1561,   106,   656,
     107,     0,   391,   392,     0,     0,     0,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,  1562,   116,   117,
     118,   655,     0,     0,   119,   120,  1563,  1564,  1565,     0,
     393,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,   657,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,   396,   397,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,   399,   403,   404,
     405,   406,   207,   208,     0,   882,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     105,   417,     0,   107,     0,     0,   418,   419,     0,   420,
     421,   422,   110,   111,   112,   423,   113,     0,   657,   114,
     115,   658,   659,   660,   400,     0,     0,   119,   120,     0,
     401,     0,     0,     0,     0,     0,   424,   425,     0,     0,
     402,     0,   661,     0,   427,     0,     0,     0,   282,     0,
       0,     0,     0,     0,   403,   404,   405,   406,   207,   208,
       0,     0,     0,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   105,   417,     0,   107,
       0,     0,   418,   419,     0,   420,   421,   422,   110,   111,
     112,   423,   113,   388,     0,   114,   115,   658,   659,   660,
       0,     0,   389,   119,   120,   104,     0,     0,     0,     0,
       0,     0,   424,   425,     0,   390,     0,     0,   661,     0,
     427,   391,   392,     0,   282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   106,     0,   107,     0,     0,
     655,     0,     0,     0,   108,   109,   110,   111,   112,   393,
     113,     0,   394,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,   427,   388,
     396,   397,     0,     0,     0,     0,     0,     0,   389,     0,
       0,     0,   398,     0,     0,     0,   399,     0,     0,     0,
       0,   390,   105,   106,   656,   107,     0,   391,   392,     0,
       0,     0,   108,   109,   110,   111,   112,     0,   113,     0,
       0,   114,   115,   116,   117,   118,   655,     0,     0,   119,
     120,     0,     0,     0,     0,   393,     0,     0,   394,     0,
       0,     0,     0,     0,   991,     0,     0,   657,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,   396,   397,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,   399,   403,   404,   405,   406,   207,   208,     0,
     882,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   105,   417,     0,   107,     0,
       0,   418,   419,     0,   420,   421,   422,   110,   111,   112,
     423,   113,     0,   657,   114,   115,   658,   659,   660,   400,
       0,     0,   119,   120,     0,   401,     0,     0,     0,     0,
       0,   424,   425,     0,     0,   402,     0,   661,     0,   427,
       0,     0,     0,   282,     0,     0,     0,     0,     0,   403,
     404,   405,   406,   207,   208,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
       0,     0,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   105,   417,     0,   107,     0,     0,   418,   419,     0,
     420,   421,   422,   110,   111,   112,   423,   113,   388,     0,
     114,   115,   116,   117,   118,     0,     0,   389,   119,   120,
     104,     0,     0,     0,     0,     0,     0,   424,   425,     0,
     390,     0,     0,   661,     0,   427,   391,   392,     0,   282,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     106,     0,   107,     0,     0,     0,     0,     0,     0,   108,
     109,   110,   111,   112,   393,   113,     0,   394,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1720,     0,   395,     0,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,   388,     0,   398,     0,   105,
     106,   399,   107,     0,   389,     0,     0,     0,     0,   108,
     109,   110,   111,   112,     0,   113,     0,   390,   114,   115,
     116,   117,   118,   391,   392,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1676,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,     0,     0,   394,     0,     0,     0,   400,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
     395,     0,   396,   397,     0,     0,     0,     0,   403,   404,
     405,   406,   207,   208,   398,     0,     0,     0,   399,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     105,   417,     0,   107,     0,     0,   418,   419,     0,   420,
     421,   422,   110,   111,   112,   423,   113,  1437,     0,   114,
     115,   116,   117,   118,     0,   698,     0,   119,   120,     0,
       0,     0,     0,     0,     0,   400,   424,   425,   493,     0,
       0,   401,   488,     0,   427,     0,     0,     6,   282,     0,
       0,   402,     0,     0,  -293,     0,     0,     0,  1554,  1555,
       0,  1556,     0,     0,     0,   403,   404,   405,   406,   207,
     208,     0,     0,     0,     0,     0,     0,     0,  1557,   407,
       0,     0,     0,     0,  1558,     0,     0,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   105,   417,     0,
     107,     0,     0,   418,   419,     0,   420,   421,   422,   110,
     111,   112,   423,   113,   388,     0,   114,   115,   116,   117,
     118,     0,     0,   389,   119,   120,     0,     0,     0,     0,
       0,     0,     0,   424,   425,     0,   390,     0,     0,   488,
       0,   427,   391,   392,  1559,   282,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
     393,     0,     0,   394,     0,     0,     0,     0,  1560,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,   396,   397,     0,     0,     0,   104,     0,     0,     0,
       0,   388,     0,   398,     0,     0,     0,   399,     0,     0,
     389,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   390,     0,  1561,   106,     0,   107,   391,
     392,     0,     0,     0,     0,   108,   109,   110,   111,   112,
       0,   113,     0,     0,   114,  1562,   116,   117,   118,     0,
       0,     0,   119,   120,  1563,  1564,  1565,   393,     0,     0,
     394,     0,     0,     0,   400,     0,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,   396,   397,
       0,     0,     0,     0,   403,   404,   405,   406,   207,   208,
     398,     0,     0,     0,   399,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   105,   417,     0,   107,
       0,     0,   418,   419,     0,   420,   421,   422,   110,   111,
     112,   423,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
       0,   400,   424,   425,     0,     0,     0,   401,   426,     0,
     427,     0,     0,     0,   282,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,   690,     0,     0,     0,     0,
       0,   403,   404,   405,   406,   207,   208,     0,     0,     0,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   105,   417,     0,   107,     0,     0,   418,
     419,     0,   420,   421,   422,   110,   111,   112,   423,   113,
     388,     0,   114,   115,   116,   117,   118,     0,     0,   389,
     119,   120,   104,     0,     0,     0,     0,     0,     0,   424,
     425,     0,   390,     0,     0,   488,     0,   427,   391,   392,
       0,   282,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   106,     0,   107,     0,     0,     0,     0,     0,
       0,   108,   109,   110,   111,   112,   393,   113,     0,   394,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,     0,     0,    75,     0,   104,     0,     0,
       0,  1687,     0,     0,     0,     0,   388,   396,   397,     0,
       0,     0,     0,     0,     0,   389,     0,     0,     0,   398,
       0,     0,     0,   399,     0,     0,   105,   106,   390,   107,
       0,     0,     0,     0,   391,   392,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
       0,     0,   393,    76,     0,   394,     0,     0,     0,     0,
     698,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,   395,     0,   396,   397,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,   399,
     403,   404,   405,   406,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   105,   417,     0,   107,     0,     0,   418,   419,
       0,   420,   421,   422,   110,   111,   112,   423,   113,     0,
       0,   114,   115,   116,   117,   118,   400,     0,     0,   119,
     120,     0,   401,     0,     0,     0,     0,     0,   424,   425,
       0,     0,   402,     0,   488,     0,   427,     0,     0,     0,
     282,     0,     0,     0,     0,     0,   403,   404,   405,   406,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   105,   417,
       0,   107,     0,     0,   418,   419,     0,   420,   421,   422,
     110,   111,   112,   423,   113,   388,     0,   114,   115,   116,
     117,   118,     0,     0,   389,   119,   120,   104,     0,     0,
       0,     0,     0,     0,   424,   425,     0,   390,     0,     0,
     488,     0,   427,   391,   392,     0,   282,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,   106,     0,   107,
       0,     0,     0,     0,     0,     0,   108,   109,   110,   111,
     112,   393,   113,     0,   394,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
       0,     0,   104,     0,   960,     0,     0,     0,     0,     0,
       0,   388,   396,   397,     0,     0,     0,     0,     0,     0,
     389,     0,     0,     0,   398,     0,     0,     0,   399,     0,
       0,   105,   106,   390,   107,     0,     0,     0,     0,   391,
     392,   108,   109,   110,   111,   112,     0,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,     0,     0,     0,     0,   393,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,   396,   397,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,   399,   403,   404,   405,   406,   207,
     208,     0,     0,     0,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   105,   417,     0,
     107,     0,     0,   418,   419,     0,   420,   421,   422,   110,
     111,   112,   423,   113,     0,     0,   114,   115,   116,   117,
     118,   400,     0,     0,   119,   120,     0,   401,     0,     0,
       0,     0,     0,   424,   425,     0,     0,   402,     0,   488,
     957,   427,     0,     0,     0,   282,     0,     0,  1157,     0,
       0,   403,   404,   405,   406,   207,   208,     0,     0,     0,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   105,   417,     0,   107,     0,     0,   418,
     419,     0,   420,   421,   422,   110,   111,   112,   423,   113,
     388,     0,   114,   115,   116,   117,   118,     0,     0,   389,
     119,   120,     0,     0,     0,     0,     0,     0,     0,   424,
     425,     0,   390,     0,     0,   488,     0,   427,   391,   392,
       0,   282,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   388,   396,   397,     0,
       0,     0,     0,     0,     0,   389,     0,     0,     0,   398,
       0,     0,     0,   399,     0,     0,     0,     0,   390,     0,
       0,     0,     0,     0,   391,   392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,   394,     0,     0,     0,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,   396,   397,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,   399,
     403,   404,   405,   406,   207,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   105,   417,     0,   107,     0,     0,   418,   419,
       0,   420,   421,   422,   110,   111,   112,   423,   113,     0,
       0,   114,   115,   116,   117,   118,   400,     0,     0,   119,
     120,     0,   401,     0,     0,     0,     0,     0,   424,   425,
       0,     0,   402,     0,   488,     0,   427,     0,     0,     0,
     282,     0,     0,  1353,     0,     0,   403,   404,   405,   406,
     207,   208,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   105,   417,
       0,   107,     0,     0,   418,   419,     0,   420,   421,   422,
     110,   111,   112,   423,   113,   388,     0,   114,   115,   116,
     117,   118,     0,     0,   389,   119,   120,     0,     0,     0,
       0,     0,     0,     0,   424,   425,     0,   390,     0,     0,
     488,     0,   427,   391,   392,     0,   282,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,   397,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,   398,     0,     0,     0,   399,     0,
       0,   389,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   390,     0,     0,     0,     0,     0,
     391,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
       0,   394,     0,     0,     0,   400,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,   396,
     397,     0,     0,     0,     0,   403,   404,   405,   406,   207,
     208,   398,     0,     0,     0,   399,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   105,   417,     0,
     107,     0,     0,   418,   419,     0,   420,   421,   422,   110,
     111,   112,   423,   113,  1437,     0,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,     0,     0,     0,     0,
       0,     0,   400,   424,   425,     0,     0,  1742,   401,   488,
       0,   427,     0,     0,     6,   282,     0,     0,   402,     0,
       0,     0,     0,     0,     0,  1554,  1555,     0,  1556,     0,
       0,     0,   403,   404,   405,   406,   207,   208,     0,     0,
       0,     0,     0,     0,     0,  1557,   407,     0,     0,     0,
       0,  1558,     0,     0,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   105,   417,     0,   107,     0,     0,
     418,   419,     0,   420,   421,   422,   110,   111,   112,   423,
     113,     0,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,     1,     0,     0,     0,     0,
     424,   425,     0,     0,     0,     0,   488,     0,   427,     0,
       0,  1559,   282,     0,     0,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     3,     0,     0,     0,     0,     0,
      14,     4,     0,     5,     0,     6,     0,     0,     0,     0,
       7,     0,     0,     0,     0,  1560,     0,     8,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,   104,     0,     0,     0,     0,     0,     0,
       0,     0,   598,     0,     0,     0,   599,     0,     0,     0,
       0,     0,     0,   799,     0,     0,     0,     0,     0,     0,
       0,     0,  1561,   106,   600,   107,     0,   601,     0,     0,
       0,     0,   108,   109,   110,   111,   112,     0,   113,     0,
       0,   114,  1562,   116,   117,   118,     0,     0,     0,   119,
     120,  1563,  1564,  1565,     0,     0,     0,     0,     0,    11,
       0,     0,     0,     0,     0,     0,    12,     0,   602,    13,
       0,    14,    15,     0,   603,   604,   605,     0,   606,   607,
     608,     0,   609,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,   610,   598,   611,     0,     0,   599,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   600,     0,     0,   601,     0,
       0,     0,     0,     0,     0,   612,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     0,
       0,     0,   613,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,     0,     0,     0,     0,     0,     0,   602,
       0,     0,     0,     0,   614,   603,   604,   605,     0,   606,
     607,   608,     0,   609,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   610,     0,   611,     0,     0,     0,   615,
     616,     0,     0,     0,     0,     0,     0,     0,   105,   106,
       0,   107,     0,     0,     0,     0,   617,     0,   108,   109,
     110,   111,   112,     0,   113,     0,   612,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,   613,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   614,     0,   872,     0,     0,
       0,   599,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,   873,     0,     0,     0,   600,
       0,     0,   601,     0,     0,     0,     0,     0,     0,     0,
     615,   616,     0,     0,     0,     0,     0,     0,     0,   105,
     106,     0,   107,     0,     0,     0,     0,   617,     0,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,   115,
     116,   117,   118,   602,     0,     0,   119,   120,     0,   603,
     604,   605,     0,   606,   607,   608,     0,   609,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,   610,     0,   611,
       0,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,     0,     0,   179,
     180,   105,   106,     0,   107,     0,     0,   181,   182,     0,
     612,   108,   109,   110,   111,   112,   183,   113,   184,     0,
     114,   115,   116,   117,   118,     0,     0,   613,   119,   120,
     185,   186,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   614,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   615,   616,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   617
};

static const short yycheck[] =
{
       9,    16,     4,    12,    54,     0,    63,    16,    55,    18,
     149,    62,   103,   139,   140,   513,   713,   139,   140,    66,
     290,   362,   426,   366,    62,   525,   366,    84,    72,   358,
     557,   690,   559,    69,   337,   881,    90,    93,    88,   885,
     567,    87,    89,    52,    53,    54,    55,    56,    57,    96,
      94,    61,   499,    62,    63,    64,    92,    66,   567,    68,
      69,   698,    71,    72,   774,   476,   763,    82,  1049,   383,
      67,    80,   943,    82,    83,    84,    91,    86,    87,    88,
      89,   335,    91,    92,    93,    94,   371,    96,    56,   731,
     727,   650,   130,    90,   132,   330,  1043,   153,   144,  1271,
      68,   284,  1049,   835,   151,   643,   766,   268,  1043,   311,
     981,   991,  1043,  1436,  1049,   712,   866,   278,  1049,  1043,
    1050,   130,   131,   132,  1043,  1049,  1043,  1099,  1076,  1059,
    1049,  1213,  1049,  1264,  1094,   144,   796,   157,     6,   223,
     149,   232,   151,   152,   153,   805,  1389,   808,   157,    10,
       8,    10,  1082,     8,  1569,   731,    23,     8,  1184,     8,
     482,  1404,  1477,   131,  1190,   656,    50,     8,    15,     8,
     661,    50,   311,    64,   656,    20,  1205,    66,  1207,  1310,
      46,    49,   192,   103,   152,     8,  1319,  1234,     9,     6,
       8,   274,   533,    26,    47,    64,     6,   198,    31,   859,
       8,     8,  1269,    26,     8,    66,   157,   217,    31,    84,
     160,  1658,    94,  1660,   543,   544,   545,   546,    12,    24,
     193,   103,     3,   261,   567,     3,    29,   567,   319,  1100,
      46,   315,   204,   518,   133,   894,   895,    58,    98,   151,
    1563,  1564,   740,   204,   151,   295,     5,    47,    69,  1856,
    1857,   592,   261,     8,   239,   240,   265,    16,   170,   268,
     269,   163,    53,    54,   124,   274,    12,    53,    54,   278,
     231,    26,     4,   311,   132,   276,    31,   132,  1725,   938,
    1240,   132,    98,   132,   598,    17,   295,   186,   190,  1420,
     299,   132,  1899,   132,   821,   202,    26,   265,   336,    80,
      66,    31,    80,    47,   277,   343,   274,   345,   469,   347,
     140,   283,   350,   188,   132,   569,  1297,   101,    59,   124,
      52,   368,    54,     4,   132,   132,   277,   336,   132,   379,
     199,   299,   974,   369,   343,   188,   345,   476,   347,   182,
      84,   350,   136,   263,   986,   987,   163,  1227,  1481,   359,
    1297,   361,   173,   163,  1014,  1303,  1286,  1383,  1384,   368,
     369,  1408,  1297,   181,   618,   375,  1297,   212,   377,  1692,
     379,    52,   322,  1297,   181,  1442,  1823,  1406,  1297,    26,
    1297,   263,   185,  1042,    31,   276,  1801,   276,   188,  1217,
     136,   882,   276,  1054,  1055,   886,   843,   276,   974,    11,
     483,  1061,   268,  1718,   886,   273,   197,    25,   678,   377,
     986,   197,   734,   280,    26,   276,   182,   276,   427,    31,
     278,   468,   163,   278,  1839,   695,    29,   278,    46,   278,
     277,  1603,   164,   276,   704,   482,    48,   278,   476,   278,
     273,   274,    14,  1104,   188,    19,  1528,   277,   271,   272,
     273,   274,   201,  1732,   238,   464,  1871,    75,   584,   468,
     469,  1120,  1121,    11,  1123,  1124,   475,   487,   148,  1884,
     479,   741,  1887,   482,   483,    71,    94,   148,   487,   563,
     102,  1896,  1214,   164,  1615,   103,  1729,    66,  1316,    94,
    1621,   109,   762,   123,    26,  1455,   464,  1523,   148,    31,
      48,   182,  1474,  1433,   184,   177,  1701,   475,  1438,   127,
    1440,   479,    86,  1473,    24,   483,   271,   272,   273,   274,
     191,   204,   200,    33,    65,  1730,  1731,   130,   538,   567,
     784,   585,   204,   787,  1414,   216,  1246,  1134,  1710,   111,
    1488,   271,   272,   273,   274,  1475,   276,  1826,   783,   803,
     634,  1110,   177,   175,   737,  1093,   565,   759,   567,  1309,
    1292,   722,   571,  1886,   194,   879,   204,   870,   979,   177,
      26,  1776,   857,    19,   828,    31,  1701,   587,   982,   204,
     176,  1223,   185,   254,  1722,    16,    26,   183,   585,   272,
      25,    31,   252,  1252,   200,  1767,   204,  1792,   276,   202,
     272,  1796,   207,   208,   642,   146,  1021,   239,   276,  1804,
    1540,   283,  1807,   281,   124,  1274,  1546,   249,    26,   745,
     759,   191,  1493,    31,   271,   272,   273,   274,   155,   639,
     640,   278,  1720,   642,  1772,   681,   146,  1809,   276,    26,
      75,  1836,   651,   153,    31,   263,   187,   272,  1308,   651,
    1855,    82,   277,   779,   780,  1779,  1780,     7,    26,   271,
     272,   273,   274,    31,   272,    89,   936,  1792,  1863,    67,
     276,  1796,   681,  1868,   109,   102,   714,   187,  1608,  1804,
      26,  1819,  1807,    81,   254,    31,  1774,   734,    26,  1104,
     114,    26,   127,    31,    47,  1625,    31,   706,   204,   204,
     204,  1030,  1429,  1340,   713,   714,    23,   105,   717,   123,
     204,  1836,   139,   722,   141,    26,  1854,     6,  1428,    18,
      31,   759,   731,  1824,  1825,   734,   231,   765,   782,   275,
     739,    84,  1820,     4,   743,    34,     7,   164,  1863,   271,
     272,   273,   274,  1868,   204,   754,   278,   756,   172,  1476,
     164,    86,  1876,    26,   763,   793,   765,  1738,    31,  1418,
      49,   159,   189,   773,   774,   775,  1095,   272,   822,   147,
      26,   277,  1272,   277,    91,    31,   204,   283,   272,   283,
      79,    52,   820,   821,   793,   782,   754,  1717,   756,  1890,
    1194,  1738,   279,     6,  1048,   282,  1726,   211,   852,   853,
     153,   855,   963,  1738,  1107,   966,  1060,  1738,     6,   108,
     163,   820,   821,    47,  1738,   271,   272,   273,   274,  1738,
     285,  1738,   278,   833,   277,   822,    40,  1081,   293,  1233,
     827,   271,   272,   273,   274,   188,    49,    51,   278,    67,
     979,  1771,    83,   840,    58,     6,   124,   129,  1295,   858,
      84,    49,     6,    81,  1301,   852,   853,   149,   855,   868,
    1369,    89,    68,   271,   272,   273,   274,  1170,   146,    83,
     278,   880,   241,    26,   166,   153,   175,   105,    31,   120,
     249,    18,    68,    33,   271,   272,   273,   274,    49,    26,
     858,   278,   133,    26,    31,    49,    46,    34,    31,   276,
     868,   278,   276,   271,   272,   273,   274,   281,   131,   187,
     278,   967,   276,   276,  1184,    62,    63,   964,   281,   129,
    1190,    10,   207,   208,   968,   271,   272,   273,   274,   965,
      26,   159,   278,    70,   148,    31,   271,   272,   273,   274,
     275,   979,    79,  1789,   137,   138,    26,  1217,   275,   276,
      87,    31,   129,  1079,   963,   964,   965,   966,   967,   968,
     271,   272,   273,   274,   129,   974,   180,   278,   240,   106,
     184,   108,  1313,   276,   182,   278,    26,   986,   987,   275,
     276,    31,   991,  1021,    64,   122,    26,   124,   125,   126,
     211,    31,   213,   148,   164,    18,   991,    83,   271,   272,
     273,   274,    20,    26,  1663,   278,    67,   276,    31,   278,
     267,    34,  1021,    67,   277,   271,   272,   273,   274,   170,
      81,  1548,   278,   207,  1683,   180,  1369,    81,  1526,  1369,
    1284,  1529,   276,   276,   278,   278,  1090,    26,   175,   276,
     195,   278,    31,   276,   105,   278,  1316,    70,  1205,    10,
    1207,   105,  1106,  1892,  1893,    26,    79,    62,  1112,  1097,
      31,  1315,  1071,    64,    87,   901,  1104,   903,   129,   905,
     906,  1073,   908,   909,   910,   129,    83,  1086,   203,   204,
     205,    26,   276,   106,   278,   108,    31,   206,  1097,    26,
    1212,  1213,  1214,  1090,    31,  1104,  1425,    82,   159,   122,
    1759,   124,   125,   126,   198,   159,   231,   232,   276,  1106,
     278,  1808,   148,  1383,  1384,  1112,    86,   206,   271,   272,
     273,   274,  1248,    66,  1250,   278,    64,   148,   276,  1627,
     278,   268,   269,   270,   271,   272,   273,   274,   271,   272,
     273,   274,   275,   280,    88,   282,   235,   236,   276,   238,
     278,   277,   175,   276,   129,   278,   245,   246,   247,   248,
     249,   276,   251,   278,   129,   254,   255,   256,   257,   258,
     276,   182,   278,   262,   263,   271,   272,   273,   274,   123,
     252,   677,   278,   679,   276,  1311,   278,   683,   684,   685,
    1444,   271,   272,   273,   274,   276,  1205,   278,  1207,  1792,
      26,     6,   107,  1796,    18,    31,   276,   151,   278,   196,
     154,  1804,    26,   180,  1223,   130,  1875,    31,  1227,    64,
      34,   271,   272,   273,   274,   262,   170,   163,   278,  1483,
     262,   271,   272,   273,   274,    33,  1246,   182,   278,   157,
      66,   215,    47,  1836,   268,   268,   269,   270,   271,   272,
     273,   274,   196,  1523,    59,    26,    70,   280,   118,   282,
      31,   276,    26,    32,   277,    79,    10,    31,   277,   277,
    1863,   277,   207,    87,   277,  1868,   207,  1416,   277,    84,
    1778,   277,   271,   272,   273,   274,    57,   277,  1290,   278,
      26,   277,   106,    57,   108,    31,   277,   277,  1437,   207,
     271,   272,   273,   274,    26,   277,   282,   278,   122,    31,
     124,   125,   126,   277,   277,   182,  1326,   276,   280,   277,
    1330,    66,   282,   148,   276,    18,   271,   272,   273,   274,
      66,    66,   174,    26,   271,   272,   273,   274,    31,   268,
     282,    34,   276,   276,    10,   277,   276,   188,   153,   218,
     219,   220,   221,   222,   223,   224,   225,   200,   163,   276,
      94,   175,    94,    94,    86,    24,    24,    46,  1622,    10,
     182,   143,   216,   182,   204,   143,    23,    70,    94,   145,
     193,   277,   193,   188,    95,  1436,    79,   277,   280,   277,
     277,  1530,    64,    10,    87,   157,   129,  1406,  1436,  1437,
     276,   197,   128,   277,   277,  1414,  1528,  1416,   135,   182,
     278,   210,    63,   106,   277,   108,    66,  1543,  1428,   198,
      66,    10,   128,   251,   194,   248,    18,  1436,  1437,   122,
    1569,   124,   125,   126,    26,    23,  1445,   204,   277,    31,
      20,   277,    34,    46,    30,   271,   272,   273,   274,    24,
      90,    90,   132,   162,   268,   269,   270,   271,   272,   273,
     274,   132,   259,   132,   164,    46,   280,   164,   282,   277,
     102,   163,    20,   277,   163,   277,    10,  1445,    70,    66,
      26,    66,   175,   278,   281,   276,   276,    79,   276,   131,
     113,   276,   276,   290,   277,    87,    74,  1554,    12,   277,
     271,   272,   273,   274,    45,   247,    10,   271,   272,   273,
     274,    40,   277,   168,   106,  1606,   108,   278,    10,   279,
     278,  1530,    51,   278,   321,   278,   200,   324,   140,    58,
     122,  1569,   124,   125,   126,   271,   272,   273,   274,    55,
     238,   186,    71,    55,     8,  1554,    98,    54,   278,   271,
     272,   273,   274,  1562,  1563,  1564,   277,  1607,   278,   277,
    1569,   181,  1701,   277,   277,   277,   134,   277,    73,   277,
     277,   134,   277,   277,   277,   268,   269,   270,   271,   272,
     273,   274,   277,   175,   278,   278,   277,   277,  1597,   277,
     277,   276,   278,  1602,   134,    89,    20,    77,  1607,   276,
     136,   168,   137,   132,    17,   132,   153,   132,   192,   132,
     132,  1626,   132,   132,   278,   278,    24,  1626,   278,   148,
    1711,  1631,   278,   420,   277,  1635,   163,   424,   425,   426,
     278,  1678,    54,   131,  1602,   278,   165,   181,    10,   182,
     142,    26,    83,   188,    84,    47,   188,    84,   278,    47,
     278,   180,    51,  1792,    98,   184,    66,  1796,   187,    58,
     131,   117,  1801,    62,    63,  1804,   195,   117,  1807,  1678,
     278,   278,    71,   278,   278,   276,   268,   269,   270,   271,
     272,   273,   274,  1692,    83,   214,    39,   278,   278,    39,
      86,   488,   275,    54,  1751,   492,   278,  1836,    86,   278,
    1839,   278,   278,  1698,   278,  1720,   278,   278,   278,   278,
      90,  1720,   278,   278,   275,   277,   275,   275,   275,   275,
     275,   275,  1737,  1732,  1863,   275,   279,   278,  1737,  1868,
      98,   114,  1871,    64,   197,   129,    42,    86,   277,   277,
      86,    89,  1751,  1752,   275,  1884,   278,   277,  1887,   148,
     276,  1753,   129,   277,   275,   129,   156,  1896,   181,  1774,
      47,   278,   188,  1801,   282,  1774,   165,   278,   276,   133,
      50,   277,  1787,   209,   275,   278,   276,    53,  1787,    83,
      39,   180,  1832,    50,   277,   184,   276,   278,     0,   278,
       0,   266,  1801,  1210,    12,   269,   195,   278,   722,  1808,
    1110,  1839,   871,  1107,  1033,  1820,   646,  1464,  1096,   765,
     592,  1820,   345,   326,  1829,   214,  1097,  1826,   333,  1463,
    1829,   317,   804,  1832,  1104,     6,  1442,  1673,     9,  1444,
    1839,  1765,    13,  1871,  1769,   363,   345,  1768,   793,  1265,
    1811,    22,  1757,  1744,  1696,  1802,  1884,  1016,  1720,  1887,
    1421,  1860,  1242,   338,    35,   350,   573,   850,  1896,   331,
      41,    42,  1871,  1299,   661,   842,    13,   501,   631,   303,
     505,   754,   767,   142,   991,  1884,   741,  1886,  1887,  1664,
    1538,   678,  1829,  1892,  1893,   682,     0,  1896,    69,  1787,
       0,    72,   689,     0,   513,   730,   983,   694,   695,  1148,
     206,  1192,  1254,  1086,  1597,    -1,   571,   704,   705,    86,
     707,   708,   709,   710,   711,   712,  1562,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,   112,   238,    -1,    -1,   116,    -1,    -1,     6,   245,
     246,   247,   248,   249,   741,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   762,    -1,   273,    -1,    -1,
     767,    -1,   769,    -1,    -1,    -1,    -1,  1338,  1339,    47,
      -1,    -1,    -1,  1344,  1345,    -1,  1347,  1348,    -1,    -1,
     171,    59,   173,    -1,  1355,  1356,  1357,  1358,   179,  1360,
    1361,  1362,  1363,  1364,  1365,    73,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,   153,    -1,    -1,    -1,    -1,
     271,   272,    -1,    -1,    -1,   163,   277,    -1,   279,   896,
     897,   898,   283,    -1,   901,   902,   903,    -1,   905,   906,
     907,   908,   909,   910,   911,   912,    -1,   914,    -1,   916,
     188,    -1,   919,   920,   921,   922,   923,   924,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   936,
      -1,    -1,   939,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,   982,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,     6,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    35,   238,    -1,   206,
      -1,    -1,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,    -1,   235,   236,
      69,   238,    -1,    72,    -1,    -1,   277,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,   206,    -1,    -1,    -1,
     277,    -1,    -1,   112,    -1,    -1,    -1,   116,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,  1146,
    1147,    -1,  1149,  1150,    -1,   245,   246,   247,   248,   249,
    1157,   251,    -1,    -1,   254,   255,   256,   257,   258,    47,
      -1,    -1,   262,   263,    -1,    -1,    -1,  1174,    -1,    -1,
      -1,    59,   171,    -1,   173,  1182,  1183,  1184,    -1,    -1,
     179,    -1,    -1,  1190,    -1,  1192,  1193,  1194,  1195,  1196,
     189,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
    1217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1233,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      41,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
    1267,    -1,    -1,   262,   263,   153,    -1,    -1,    69,    -1,
      -1,    -1,   271,   272,    -1,   163,    -1,    -1,   277,    -1,
     279,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,     6,
     188,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,  1316,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
    1337,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,  1346,
      -1,    -1,    -1,    -1,    -1,    -1,  1353,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    69,   251,    -1,    72,   254,   255,   256,   257,
     258,    -1,   173,    -1,   262,   263,  1383,  1384,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   112,   207,   208,    -1,   116,
    1417,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,    41,    -1,    -1,    44,    -1,
     241,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   250,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,   272,   179,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,   189,    -1,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,  1501,  1502,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,  1518,    -1,   119,    -1,   121,  1523,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    74,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,   152,   254,   255,   256,
     257,   258,  1559,    -1,    -1,   262,   263,   206,    -1,     6,
      -1,    -1,     9,   169,   271,   272,    13,    -1,    -1,    -1,
     277,    -1,   279,    -1,    -1,    22,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    -1,
     226,   227,    69,  1630,    -1,    72,    -1,    -1,    -1,    -1,
      -1,  1638,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,     6,  1664,    -1,     9,
      -1,    -1,    -1,    13,   206,   112,    -1,    -1,    -1,   116,
    1677,    -1,    22,    -1,  1681,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    69,
     262,   263,    72,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,  1744,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,  1803,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,   171,    -1,   173,   271,   272,    -1,    -1,    -1,   179,
     277,    -1,   279,    -1,    -1,    -1,   283,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   206,    -1,     6,    -1,    -1,     9,
      -1,   271,   272,    13,    -1,    -1,    -1,   277,    -1,   279,
      -1,    -1,    22,   283,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,
      -1,    41,    42,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,   184,    -1,    -1,    -1,    -1,   189,    69,
      -1,    -1,    72,    -1,   195,   277,    -1,   279,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,   112,    -1,   235,   236,   116,   238,    -1,    22,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    35,   254,   255,   256,   257,   258,    41,    42,
      -1,   262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   171,    -1,
     173,   271,   272,    -1,    89,    -1,   179,   277,    -1,   279,
      -1,    -1,    -1,   283,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   172,    -1,   262,
     263,    -1,    -1,     6,    -1,    -1,     9,    -1,   271,   272,
      13,    -1,    -1,    -1,   277,    -1,   279,    -1,    -1,    22,
     283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,   180,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,
      -1,    -1,    -1,   116,    -1,   189,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
     235,   236,   206,   238,    -1,    41,    42,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    60,    -1,    -1,   262,   263,    -1,
      -1,   235,   236,    69,   238,    -1,    72,    -1,   171,    -1,
     173,   245,   246,   247,   248,   249,   179,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   189,    -1,   262,   263,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    17,
      -1,   254,   255,   256,   257,   258,    -1,   163,    -1,   262,
     263,   167,    -1,    -1,    -1,    -1,    -1,   173,   271,   272,
      -1,    -1,    -1,   179,   277,    -1,   279,    -1,    -1,    47,
     283,    -1,    -1,   189,    -1,    -1,    54,    -1,    -1,    -1,
      58,    59,    -1,    61,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,   217,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,    13,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,    22,    -1,    -1,
      -1,   277,    -1,   279,    -1,    -1,   144,   283,    -1,    -1,
      35,    -1,    -1,    38,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    13,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    35,   235,   236,   124,
     238,    -1,    41,    42,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    60,    -1,    -1,   262,   263,   264,   265,   266,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   203,   204,
     205,   206,   207,   208,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,   167,   254,
     255,   256,   257,   258,   173,    -1,    -1,   262,   263,    -1,
     179,    -1,    -1,    -1,    -1,    -1,   271,   272,    -1,    -1,
     189,    -1,   277,    -1,   279,    -1,    -1,    -1,   283,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    13,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    22,   262,   263,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,    -1,    35,    -1,    -1,   277,    -1,
     279,    41,    42,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      60,    -1,    -1,    -1,   245,   246,   247,   248,   249,    69,
     251,    -1,    72,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,   279,    13,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    35,   235,   236,   124,   238,    -1,    41,    42,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    60,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,   277,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   116,   203,   204,   205,   206,   207,   208,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,   167,   254,   255,   256,   257,   258,   173,
      -1,    -1,   262,   263,    -1,   179,    -1,    -1,    -1,    -1,
      -1,   271,   272,    -1,    -1,   189,    -1,   277,    -1,   279,
      -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    13,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    22,   262,   263,
     206,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,    -1,
      35,    -1,    -1,   277,    -1,   279,    41,    42,    -1,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    69,   251,    -1,    72,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   277,    -1,    98,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,   112,    -1,   235,
     236,   116,   238,    -1,    22,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    35,   254,   255,
     256,   257,   258,    41,    42,    -1,   262,   263,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    17,    -1,   254,
     255,   256,   257,   258,    -1,   163,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,   173,   271,   272,   273,    -1,
      -1,   179,   277,    -1,   279,    -1,    -1,    47,   283,    -1,
      -1,   189,    -1,    -1,    54,    -1,    -1,    -1,    58,    59,
      -1,    61,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,   217,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    13,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    22,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,    -1,    35,    -1,    -1,   277,
      -1,   279,    41,    42,   144,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    13,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,   235,   236,    -1,   238,    41,
      42,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   264,   265,   266,    69,    -1,    -1,
      72,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    -1,
      -1,   173,   271,   272,    -1,    -1,    -1,   179,   277,    -1,
     279,    -1,    -1,    -1,   283,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      13,    -1,   254,   255,   256,   257,   258,    -1,    -1,    22,
     262,   263,   206,    -1,    -1,    -1,    -1,    -1,    -1,   271,
     272,    -1,    35,    -1,    -1,   277,    -1,   279,    41,    42,
      -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    69,   251,    -1,    72,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,    -1,   204,    -1,   206,    -1,    -1,
      -1,   275,    -1,    -1,    -1,    -1,    13,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,   272,    -1,    72,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,   173,    -1,    -1,   262,
     263,    -1,   179,    -1,    -1,    -1,    -1,    -1,   271,   272,
      -1,    -1,   189,    -1,   277,    -1,   279,    -1,    -1,    -1,
     283,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    13,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    22,   262,   263,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   272,    -1,    35,    -1,    -1,
     277,    -1,   279,    41,    42,    -1,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    69,   251,    -1,    72,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,   273,    -1,    -1,    -1,    -1,    -1,
      -1,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,
      42,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
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
      -1,    -1,    -1,   271,   272,    -1,    -1,   189,    -1,   277,
     278,   279,    -1,    -1,    -1,   283,    -1,    -1,   200,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      13,    -1,   254,   255,   256,   257,   258,    -1,    -1,    22,
     262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
     272,    -1,    35,    -1,    -1,   277,    -1,   279,    41,    42,
      -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,   173,    -1,    -1,   262,
     263,    -1,   179,    -1,    -1,    -1,    -1,    -1,   271,   272,
      -1,    -1,   189,    -1,   277,    -1,   279,    -1,    -1,    -1,
     283,    -1,    -1,   200,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    13,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    22,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   272,    -1,    35,    -1,    -1,
     277,    -1,   279,    41,    42,    -1,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    17,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   271,   272,    -1,    -1,   275,   179,   277,
      -1,   279,    -1,    -1,    47,   283,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    61,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,   217,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,     7,    -1,    -1,    -1,    -1,
     271,   272,    -1,    -1,    -1,    -1,   277,    -1,   279,    -1,
      -1,   144,   283,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
     163,    43,    -1,    45,    -1,    47,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,   178,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    41,   238,    -1,    44,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    85,   161,
      -1,   163,   164,    -1,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,   119,    19,   121,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   237,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     252,   253,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,   191,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,   152,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    19,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    41,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,   243,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    85,    -1,    -1,   262,   263,    -1,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,   119,    -1,   121,
      -1,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,    -1,    -1,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     152,   245,   246,   247,   248,   249,   250,   251,   252,    -1,
     254,   255,   256,   257,   258,    -1,    -1,   169,   262,   263,
     264,   265,   266,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   243
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
case 21:
{ prepare_console_debug ((int) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 22:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
    break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
    break;
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 28:
{ yyval = yyvsp[0]; }
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
{ yyval = 0; }
    break;
case 42:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;
case 43:
{ yyval = 0; }
    break;
case 44:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
    break;
case 45:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
    break;
case 46:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
    break;
case 47:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;
case 48:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;
case 49:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;
case 50:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;
case 51:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
    break;
case 52:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
    break;
case 54:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 57:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
    break;
case 58:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
    break;
case 59:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
    break;
case 60:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
    break;
case 62:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 63:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;
case 64:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
    break;
case 65:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
    break;
case 67:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 68:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
    break;
case 70:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 71:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;
case 73:
{ yyval = yyvsp[0];}
    break;
case 74:
{ yyval = yyvsp[0]; }
    break;
case 75:
{ yyval = yyvsp[0]; }
    break;
case 76:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
    break;
case 78:
{ lex.g_field->fld_dtype = dtype_blob; }
    break;
case 79:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-2]; }
    break;
case 80:
{ yyval = NULL; }
    break;
case 82:
{ yyval = yyvsp[-1]; }
    break;
case 84:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 85:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-1], NULL); }
    break;
case 86:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
    break;
case 88:
{ yyval = yyvsp[-1]; }
    break;
case 89:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
    break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
    break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
    break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
    break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
    break;
case 94:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 95:
{ yyval = yyvsp[0]; }
    break;
case 96:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 97:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 98:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_unique, 0, NULL); }
    break;
case 115:
{ yyval = NULL; }
    break;
case 116:
{ yyval = make_list (yyvsp[0]); }
    break;
case 118:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 119:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 120:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 121:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 122:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 123:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 124:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 125:
{ yyval = (DSQL_NOD) 0;}
    break;
case 126:
{ yyval = yyvsp[-1]; }
    break;
case 127:
{ yyval = NULL; }
    break;
case 130:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 131:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 132:
{ yyval = NULL; }
    break;
case 133:
{ yyval = NULL; }
    break;
case 134:
{ yyval = yyvsp[0]; }
    break;
case 135:
{ yyval = NULL; }
    break;
case 136:
{ yyval = NULL; }
    break;
case 139:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 140:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 143:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 144:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 145:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
    break;
case 146:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
    break;
case 147:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 150:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) yyvsp[0]; }
    break;
case 151:
{yyval = NULL;}
    break;
case 154:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 155:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 156:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 157:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 158:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 159:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 160:
{yyval = NULL;}
    break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 168:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 169:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
    break;
case 170:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
    break;
case 171:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
    break;
case 172:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
    break;
case 173:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;
case 174:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 176:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
    break;
case 177:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
    break;
case 178:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
    break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 181:
{ 
				 check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
												(DSQL_NOD) lex.g_file); }
    break;
case 182:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (dsql_str*) yyvsp[0]; }
    break;
case 185:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
    break;
case 186:
{ lex.g_file  = make_file ();}
    break;
case 191:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
    break;
case 192:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
    break;
case 199:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 200:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 201:
{ yyval = yyvsp[0]; }
    break;
case 202:
{ yyval = yyvsp[0]; }
    break;
case 203:
{ yyval = NULL; }
    break;
case 205:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 210:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 211:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 214:
{ yyval = NULL; }
    break;
case 215:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 216:
{ yyval = yyvsp[0]; }
    break;
case 217:
{ yyval = NULL; }
    break;
case 218:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 219:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 220:
{ yyval = yyvsp[-1]; }
    break;
case 221:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 222:
{ yyval = yyvsp[0]; }
    break;
case 223:
{ yyval = NULL; }
    break;
case 227:
{ yyval = yyvsp[0]; }
    break;
case 228:
{ yyval = yyvsp[0]; }
    break;
case 229:
{ yyval = yyvsp[0]; }
    break;
case 230:
{ yyval = NULL; }
    break;
case 233:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 234:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 235:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 236:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 238:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 239:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 240:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 241:
{ yyval = yyvsp[0]; }
    break;
case 242:
{ yyval = NULL ;}
    break;
case 247:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 248:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 249:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 250:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 251:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 252:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 253:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 254:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 255:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 256:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 257:
{ yyval = NULL;}
    break;
case 258:
{ yyval = yyvsp[0];}
    break;
case 259:
{ yyval = yyvsp[0];}
    break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 264:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 265:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 266:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 267:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 268:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 269:
{ yyval = NULL; }
    break;
case 270:
{ yyval = yyvsp[0]; }
    break;
case 271:
{ yyval = NULL; }
    break;
case 273:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 274:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 275:
{ yyval = make_list (yyvsp[0]); }
    break;
case 276:
{ yyval = NULL; }
    break;
case 278:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 279:
{ yyval = yyvsp[-1]; }
    break;
case 282:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 283:
{ yyval = NULL; }
    break;
case 284:
{ yyval = NULL; }
    break;
case 285:
{ yyval = yyvsp[0]; }
    break;
case 286:
{ yyval = yyvsp[0]; }
    break;
case 287:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 290:
{ yyval = yyvsp[-1]; }
    break;
case 291:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 292:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 293:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 295:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 311:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 312:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 315:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 316:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 317:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 318:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 319:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 322:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 323:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 325:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 327:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 328:
{ yyval = NULL; }
    break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 330:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 331:
{ yyval = make_list (yyvsp[0]); }
    break;
case 332:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 333:
{ yyval = NULL; }
    break;
case 334:
{ yyval = make_list (yyvsp[0]); }
    break;
case 335:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 336:
{ yyval = NULL; }
    break;
case 339:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 341:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 342:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 343:
{ yyval = NULL; }
    break;
case 344:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 345:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 346:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 347:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 348:
{ yyval = NULL; }
    break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 351:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 353:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 354:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 355:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 356:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 357:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 361:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 362:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 363:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 364:
{ yyval = NULL; }
    break;
case 365:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
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
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 371:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 372:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
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
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;
case 382:
{ lex.beginning = lex.last_token; }
    break;
case 383:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
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
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 403:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 404:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 405:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 406:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 407:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 408:
{ yyval = MAKE_constant ((dsql_str*) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
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
			  yyval = (DSQL_NOD) lex.g_field; }
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
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;
case 480:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 481:
{ yyval = yyvsp[0]; }
    break;
case 482:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 483:
{ yyval = yyvsp[0]; }
    break;
case 484:
{ yyval = yyvsp[0]; }
    break;
case 485:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 486:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 487:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 488:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 490:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
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
{ if ((SLONG) yyvsp[0] < 1)
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
			*stmt_ambiguous = TRUE;
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
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 536:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
    break;
case 537:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 538:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
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
				};
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
case 561:
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
case 564:
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
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 574:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 575:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
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
{ yyval = (DSQL_NOD) NOD_SHARED; }
    break;
case 620:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;
case 621:
{ yyval = (DSQL_NOD) 0; }
    break;
case 622:
{ yyval = (DSQL_NOD) NOD_READ; }
    break;
case 623:
{ yyval = (DSQL_NOD) NOD_WRITE; }
    break;
case 625:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 626:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 627:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 628:
{ yyval = 0; }
    break;
case 630:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 631:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 632:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 633:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 634:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 635:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 636:
{ yyval = make_list (yyvsp[0]); }
    break;
case 637:
{ yyval = 0; }
    break;
case 639:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 640:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 641:
{ yyval = 0; }
    break;
case 642:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 643:
{ yyval = 0; }
    break;
case 644:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 645:
{ yyval = 0; }
    break;
case 646:
{ yyval = yyvsp[-1]; }
    break;
case 647:
{ yyval = 0; }
    break;
case 648:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 649:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 650:
{ yyval = NULL; }
    break;
case 651:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 652:
{ yyval = 0; }
    break;
case 653:
{ yyval = yyvsp[0]; }
    break;
case 654:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 656:
{ yyval = 0; }
    break;
case 657:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;
case 658:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 659:
{ lex.limit_clause = true; }
    break;
case 660:
{ lex.limit_clause = false; }
    break;
case 661:
{ lex.first_detection = true; }
    break;
case 662:
{ lex.first_detection = false; }
    break;
case 663:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 664:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 665:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 666:
{ yyval = 0; }
    break;
case 667:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 668:
{ yyval = yyvsp[-2]; }
    break;
case 669:
{ yyval = yyvsp[-1]; }
    break;
case 670:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 671:
{ yyval = yyvsp[-1]; }
    break;
case 672:
{ yyval = yyvsp[0]; }
    break;
case 673:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 674:
{ yyval = 0; }
    break;
case 675:
{ yyval = make_list (yyvsp[0]); }
    break;
case 676:
{ yyval = 0; }
    break;
case 678:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 680:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 683:
{ yyval = make_list (yyvsp[0]); }
    break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 689:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 691:
{ yyval = NULL; }
    break;
case 692:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 693:
{ yyval = NULL; }
    break;
case 695:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 696:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 697:
{ yyval = yyvsp[-1]; }
    break;
case 698:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 699:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 700:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 701:
{ yyval = NULL; }
    break;
case 703:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 704:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 705:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 706:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 707:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 708:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 709:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 710:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 711:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 712:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 713:
{ yyval = make_list (yyvsp[0]); }
    break;
case 714:
{ yyval = 0; }
    break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 718:
{ yyval = yyvsp[0]; }
    break;
case 719:
{ yyval = 0; }
    break;
case 720:
{ yyval = yyvsp[0]; }
    break;
case 721:
{ yyval = 0; }
    break;
case 722:
{ yyval = yyvsp[0]; }
    break;
case 723:
{ yyval = 0; }
    break;
case 724:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 725:
{ yyval = 0; }
    break;
case 726:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 727:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 728:
{ yyval = 0; }
    break;
case 729:
{ yyval = 0; }
    break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 732:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 736:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 737:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 738:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 740:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 741:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 742:
{ yyval = 0; }
    break;
case 743:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 744:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 747:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 748:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 749:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 752:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 753:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 756:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 763:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 764:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 768:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 769:
{ yyval = yyvsp[0]; }
    break;
case 771:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 774:
{ yyval = NULL; }
    break;
case 775:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 777:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 779:
{ yyval = NULL; }
    break;
case 780:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 782:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 785:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 798:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 801:
{ yyval = yyvsp[-1]; }
    break;
case 802:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 813:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 815:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 817:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 819:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 822:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 825:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 826:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 828:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 829:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 830:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 831:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 832:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 841:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 843:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 845:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 847:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 848:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 849:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 851:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 853:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 854:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 855:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 856:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 857:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 859:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 860:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 861:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 864:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 865:
{ yyval = yyvsp[-1]; }
    break;
case 866:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 867:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 877:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 878:
{ yyval = yyvsp[0]; }
    break;
case 879:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 880:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 881:
{ yyval = make_node (nod_collate, (int) e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 882:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 883:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 884:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 885:
{ yyval = yyvsp[-1]; }
    break;
case 886:
{ yyval = yyvsp[-1]; }
    break;
case 890:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 891:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 892:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 893:
{ yyval = yyvsp[0]; }
    break;
case 894:
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
case 895:
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
case 896:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 897:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 899:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 901:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 902:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 905:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 908:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 909:
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
case 910:
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
case 911:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 912:
{ yyval = make_parameter (); }
    break;
case 913:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 914:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 915:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 916:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 918:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 919:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 920:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 921:
{ yyval = yyvsp[0]; }
    break;
case 922:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 924:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 925:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 926:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 927:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 928:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 930:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 931:
{ yyval = yyvsp[0];}
    break;
case 936:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 937:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 938:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 941:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 943:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 944:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 945:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 946:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 947:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 949:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 951:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 952:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 953:
{ yyval = yyvsp[0]; }
    break;
case 954:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 955:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 956:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 957:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 960:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 961:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 964:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 965:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 966:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 967:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 968:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 969:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 971:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 979:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 985:
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
	for (const TOK *token = KEYWORD_getTokens(); token->tok_string; ++token)
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
	TEXT* string,
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


static void check_log_file_attrs (void)
{
/**********************************************
 *
 *	c h e c k _ l o g _ f i l e _ a t t r s
 *
 **********************************************
 *
 * Functional description
 *	Check if log file attributes are valid
 *
 *********************************************/

	if (lex.g_file->fil_partitions) {
		if (!lex.g_file->fil_length) {
			yyabandon (-261, isc_log_length_spec);
			/* Total length of a partitioned log must be specified */
		}
		
		if (PARTITION_SIZE (OneK * lex.g_file->fil_length, lex.g_file->fil_partitions) <
			(OneK*MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_partition_too_small);
			/* Log partition size too small */
		}
	}
	else {
		if ((lex.g_file->fil_length) && (lex.g_file->fil_length < MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_log_too_small);   /* Log size too small */
		}
	}	 
}


static TEXT* lex_position (void)
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
static bool long_int(DSQL_NOD string,
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

static dsql_fld* make_field (DSQL_NOD field_name)
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


static dsql_fil* make_file (void)
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


static DSQL_NOD make_list (DSQL_NOD node)
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
		*--ptr = (DSQL_NOD) LLS_POP (&stack);

	return node;
}


static DSQL_NOD make_parameter (void)
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
	node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

	return node;
}


static DSQL_NOD make_node (NOD_TYPE	type,
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
		*p++ = va_arg (ptr, DSQL_NOD);

	return node;
}


static DSQL_NOD make_flag_node (NOD_TYPE	type,
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
		*p++ = va_arg (ptr, DSQL_NOD);

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
static bool short_int(DSQL_NOD string,
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

static void stack_nodes (DSQL_NOD	node,
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
	BOOLEAN* stmt_ambiguous)
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
	BOOLEAN* stmt_ambiguous)
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
	char* p;
	char* buffer;
	char* buffer_end;
	char* new_buffer;
	DSQL_SYM	sym;
	SSHORT	c;
	USHORT	buffer_len;

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
		p = string;
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

		yylval = (DSQL_NOD) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
	}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
	{
		buffer = string;
		buffer_len = sizeof (string);
		buffer_end = buffer + buffer_len - 1;
		for (p = buffer; ; p++)
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
				new_buffer = (char*)gds__alloc (2 * buffer_len);
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
			*stmt_ambiguous = TRUE; /* string delimited by double quotes could be
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
				yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
				dsql_str* delimited_id_str = (dsql_str*) yylval;
				delimited_id_str->str_flags |= STR_delimited_id;
				if (buffer != string)
					gds__free (buffer);
				return SYMBOL;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
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
				yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);
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
					yylval = (DSQL_NOD) (ULONG) number;
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

					yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);

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
		p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; ptr < end && classes[*ptr] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER (*ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		sym = HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
				(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
				 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
				 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
			{			
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
				lex = savedState;
				if (nextToken==OR || nextToken==AND) {
					switch(sym->sym_keyword) {
					case INSERTING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (DSQL_NOD) sym->sym_object;
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
					int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (nextToken != NUMBER && nextToken != '?' && nextToken != '(') {
						yylval = (DSQL_NOD) MAKE_string(string, p - string);
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return SYMBOL;
					} else {
						yylval = (DSQL_NOD) sym->sym_object;
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			}
			else {
				yylval = (DSQL_NOD) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
				return sym->sym_keyword;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(string, p - string);
		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end)
	{
		sym = HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
		(prev_keyword=='(' || prev_keyword==NOT || prev_keyword==AND || prev_keyword==OR ||
		 prev_keyword==ON || prev_keyword==HAVING || prev_keyword==WHERE || prev_keyword==WHEN) ) 
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
		} while (nextToken == '(');
		DSQL_NOD temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect,db_dialect,
						  parser_version,stmt_ambiguous) == ')')
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
					(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
					 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
					 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
				{			
					savedState = lex;
					int token = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (token==OR || token==AND) {
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

