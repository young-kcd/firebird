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
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../jrd/gds.h"
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

ASSERT_FILENAME

static void	yyerror (TEXT *);

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

#define SHRT_POS_MAX		32767
#define SHRT_UNSIGNED_MAX	65535
#define SHRT_NEG_MAX		32768
#define LONG_POS_MAX		2147483647
#define POSITIVE		0
#define NEGATIVE		1
#define UNSIGNED		2

#define MIN_CACHE_BUFFERS	   250
#define DEF_CACHE_BUFFERS	   1000

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE		2048
#define YYMAXDEPTH		2048

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

#define TRIGGER_TYPE_SUFFIX(slot1, slot2, slot3) \
	((slot1 << 1) | (slot2 << 3) | (slot3 << 5))

extern "C" {

#ifndef SHLIB_DEFS
DSQL_NOD		DSQL_parse;
#else
extern DSQL_NOD	DSQL_parse;
#endif

}	// extern "C"
static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
	{\
	if ((to - string) >= MAX_TOKEN_LEN)		\
	yyabandon (-104, isc_token_too_long); \
	}
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(DSQL_NOD, SLONG *, SSHORT);
#endif
static void	stack_nodes (DSQL_NOD, DLLS *);
inline static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, ISC_STATUS);
static void	check_log_file_attrs (void);

struct LexerState {
	/* This is, in fact, parser state. Not used in lexer itself */
	DSQL_FLD g_field;
	FIL	g_file;
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
		BOOLEAN	*stmt_ambiguous);
};

/* Get ready for thread-safety. Move this to BISON object pointer when we 
   switch to generating "pure" reenterant parser. */
static struct LexerState lex;

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		1872
#define	YYFLAG		-32768
#define	YYNTBASE	280

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 517 ? yytranslate[x] : 703)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     273,   274,   269,   267,   272,   268,   278,   270,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   275,   271,
     265,   264,   266,   279,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   276,     2,   277,     2,     2,     2,     2,     2,     2,
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
     256,   257,   258,   259,   260,   261,   262,   263
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
     889,   891,   892,   895,   897,   899,   901,   905,   907,   910,
     911,   913,   916,   919,   922,   924,   926,   928,   930,   932,
     934,   937,   939,   941,   944,   946,   948,   950,   953,   956,
     958,   962,   967,   970,   977,   982,   984,   986,   995,  1005,
    1012,  1021,  1028,  1033,  1034,  1039,  1042,  1044,  1048,  1049,
    1052,  1057,  1058,  1060,  1062,  1066,  1070,  1078,  1081,  1082,
    1085,  1088,  1092,  1096,  1097,  1099,  1102,  1107,  1109,  1113,
    1116,  1119,  1122,  1124,  1129,  1137,  1145,  1147,  1149,  1153,
    1158,  1166,  1169,  1171,  1175,  1177,  1179,  1181,  1188,  1192,
    1193,  1194,  1195,  1196,  1200,  1201,  1211,  1221,  1223,  1225,
    1226,  1229,  1231,  1233,  1235,  1237,  1239,  1243,  1247,  1251,
    1255,  1259,  1263,  1269,  1275,  1281,  1287,  1293,  1299,  1302,
    1303,  1308,  1311,  1315,  1319,  1322,  1325,  1329,  1333,  1336,
    1340,  1347,  1349,  1352,  1357,  1361,  1364,  1367,  1370,  1373,
    1377,  1379,  1383,  1387,  1391,  1394,  1397,  1402,  1407,  1413,
    1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,  1431,  1433,
    1435,  1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,  1453,
    1455,  1457,  1459,  1461,  1463,  1466,  1469,  1472,  1474,  1476,
    1478,  1479,  1482,  1485,  1486,  1488,  1491,  1494,  1497,  1500,
    1503,  1508,  1512,  1515,  1518,  1520,  1524,  1532,  1534,  1535,
    1537,  1538,  1541,  1544,  1547,  1550,  1553,  1556,  1559,  1562,
    1565,  1569,  1572,  1575,  1578,  1580,  1582,  1584,  1586,  1591,
    1597,  1599,  1603,  1605,  1609,  1611,  1614,  1616,  1618,  1620,
    1622,  1624,  1626,  1628,  1630,  1632,  1634,  1636,  1641,  1646,
    1653,  1659,  1663,  1664,  1667,  1670,  1671,  1675,  1676,  1681,
    1683,  1689,  1694,  1696,  1701,  1703,  1706,  1709,  1711,  1713,
    1715,  1718,  1721,  1724,  1727,  1728,  1732,  1738,  1740,  1742,
    1745,  1749,  1751,  1754,  1758,  1759,  1761,  1763,  1765,  1771,
    1777,  1784,  1786,  1788,  1790,  1793,  1798,  1800,  1801,  1807,
    1809,  1810,  1814,  1817,  1819,  1820,  1823,  1824,  1826,  1827,
    1831,  1833,  1834,  1836,  1839,  1841,  1843,  1845,  1847,  1850,
    1853,  1855,  1858,  1862,  1864,  1866,  1870,  1874,  1876,  1879,
    1883,  1885,  1888,  1889,  1892,  1894,  1896,  1897,  1899,  1901,
    1903,  1907,  1910,  1914,  1915,  1917,  1921,  1926,  1931,  1933,
    1937,  1942,  1946,  1947,  1949,  1953,  1957,  1959,  1961,  1962,
    1964,  1966,  1971,  1972,  1976,  1977,  1980,  1981,  1984,  1985,
    1995,  2006,  2007,  2008,  2009,  2010,  2014,  2017,  2019,  2020,
    2024,  2030,  2034,  2037,  2043,  2046,  2048,  2050,  2052,  2054,
    2056,  2060,  2062,  2066,  2068,  2069,  2072,  2074,  2078,  2080,
    2082,  2084,  2091,  2093,  2094,  2098,  2099,  2101,  2105,  2112,
    2116,  2121,  2124,  2128,  2129,  2131,  2135,  2137,  2139,  2141,
    2144,  2146,  2148,  2150,  2153,  2155,  2158,  2160,  2163,  2164,
    2168,  2169,  2171,  2175,  2177,  2180,  2181,  2184,  2185,  2188,
    2189,  2194,  2196,  2199,  2201,  2203,  2204,  2206,  2210,  2213,
    2215,  2217,  2220,  2222,  2227,  2230,  2232,  2236,  2245,  2251,
    2253,  2257,  2259,  2261,  2266,  2271,  2276,  2278,  2280,  2286,
    2292,  2294,  2298,  2302,  2304,  2306,  2314,  2322,  2328,  2332,
    2333,  2335,  2337,  2339,  2342,  2343,  2345,  2347,  2349,  2350,
    2354,  2356,  2360,  2362,  2363,  2367,  2369,  2373,  2375,  2379,
    2383,  2385,  2387,  2391,  2393,  2396,  2398,  2402,  2406,  2408,
    2411,  2415,  2419,  2423,  2427,  2429,  2433,  2436,  2438,  2440,
    2442,  2444,  2446,  2448,  2450,  2452,  2454,  2456,  2460,  2464,
    2468,  2472,  2476,  2480,  2484,  2488,  2495,  2502,  2509,  2516,
    2523,  2530,  2537,  2544,  2551,  2558,  2565,  2572,  2579,  2586,
    2593,  2600,  2602,  2604,  2610,  2617,  2621,  2626,  2632,  2639,
    2643,  2648,  2652,  2657,  2661,  2666,  2671,  2677,  2682,  2687,
    2691,  2696,  2698,  2700,  2702,  2704,  2706,  2708,  2710,  2714,
    2718,  2729,  2740,  2742,  2744,  2746,  2748,  2750,  2752,  2754,
    2756,  2758,  2761,  2764,  2768,  2772,  2776,  2780,  2784,  2788,
    2792,  2796,  2798,  2800,  2802,  2804,  2808,  2810,  2812,  2814,
    2816,  2818,  2823,  2825,  2829,  2831,  2834,  2836,  2838,  2840,
    2842,  2844,  2846,  2848,  2851,  2854,  2857,  2859,  2861,  2863,
    2865,  2867,  2869,  2871,  2873,  2875,  2877,  2880,  2882,  2885,
    2887,  2889,  2891,  2893,  2895,  2898,  2900,  2902,  2904,  2906,
    2908,  2913,  2919,  2925,  2931,  2937,  2943,  2949,  2955,  2961,
    2967,  2973,  2980,  2982,  2989,  2991,  2996,  3004,  3007,  3008,
    3013,  3017,  3024,  3026,  3028,  3035,  3042,  3044,  3046,  3051,
    3058,  3063,  3069,  3073,  3079,  3084,  3090,  3092,  3094,  3096,
    3098,  3100,  3102,  3104,  3106,  3108,  3110,  3112,  3114,  3115,
    3117,  3119,  3121,  3123,  3125,  3127,  3129,  3131,  3133,  3135,
    3137,  3139,  3141,  3143,  3145,  3147,  3149,  3151,  3153,  3155,
    3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,  3173,  3175,
    3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,  3193,  3195,
    3197,  3199
};
static const short yyrhs[] =
{
     281,     0,   281,   271,     0,   458,     0,   600,     0,   512,
       0,   311,     0,   301,     0,   590,     0,   480,     0,   282,
       0,   588,     0,   435,     0,   313,     0,   315,     0,   291,
       0,   513,     0,   506,     0,   535,     0,   504,     0,   594,
       0,    43,   647,     0,    73,   284,   129,   283,   182,   295,
     288,     0,    73,   286,   129,   148,   290,   182,   295,   288,
       0,    73,   284,   129,   283,   182,   293,     0,    73,   286,
     129,   148,   290,   182,   293,     0,    73,   297,   182,   299,
     289,     0,   573,     0,   180,   573,     0,     6,     0,     6,
     147,     0,   285,     0,   287,     0,   285,   272,   287,     0,
      59,     0,   163,     0,    84,     0,    47,     0,   188,   605,
       0,   153,   605,     0,   200,    73,   131,     0,     0,   200,
     210,   131,     0,     0,   692,     0,   158,   292,   284,   129,
     283,    66,   295,     0,   158,   292,   286,   129,   148,   290,
      66,   295,     0,   158,   284,   129,   283,    66,   295,     0,
     158,   286,   129,   148,   290,    66,   295,     0,   158,   284,
     129,   283,    66,   293,     0,   158,   286,   129,   148,   290,
      66,   293,     0,   158,   297,    66,   299,     0,    73,   131,
      64,     0,   294,     0,   293,   272,   294,     0,   293,   272,
     296,     0,   295,   272,   294,     0,   148,   692,     0,   184,
     696,     0,   195,   699,     0,   214,   693,     0,   296,     0,
     295,   272,   296,     0,   697,     0,   189,   697,     0,    74,
     697,     0,   298,     0,   297,   272,   298,     0,   693,     0,
     300,     0,   299,   272,   300,     0,   697,     0,   189,   697,
       0,    45,   302,     0,    63,   310,     0,    62,    68,   303,
       0,   677,   305,   157,   308,    55,   646,   117,   646,     0,
     487,     0,    19,     0,    37,   273,   650,   274,   493,     0,
       0,   306,     0,   273,   306,   274,     0,   307,     0,   306,
     272,   307,     0,   374,   304,     0,   374,   304,    20,   238,
       0,   309,     0,   273,   309,   274,     0,   374,   304,     0,
     374,   304,   212,     0,   374,   304,    20,   101,     0,   374,
     304,    20,   238,     0,   140,   650,     0,   686,    82,   492,
     135,   492,    55,   646,   117,   646,     0,    36,   312,     0,
      58,   685,   646,     0,   317,   540,    83,   689,   129,   573,
     318,     0,   148,   393,     0,   180,   361,     0,   184,   450,
       0,   195,   436,     0,    71,   334,     0,    40,   336,     0,
      51,   325,     0,   165,   319,     0,   214,   335,     0,   237,
     314,     0,   148,   394,     0,   180,   362,     0,   195,   437,
       0,    36,   132,     7,   316,     0,   148,   395,     0,   184,
     451,     0,   187,     0,     0,   607,     0,   606,     0,   368,
     273,   447,   635,   448,   274,     0,   650,   320,   321,   646,
     322,   323,     0,   111,     0,    14,     0,     0,     0,    32,
       0,     0,   102,   337,   653,   360,     0,     0,   324,     0,
     349,     0,   324,   349,     0,   371,   326,   482,   447,   327,
     448,   328,   370,     0,    10,     0,     0,    46,   447,   376,
       0,     0,     0,   329,     0,   330,     0,   329,   330,     0,
     331,     0,   332,     0,   333,     0,   124,    98,     0,   447,
      24,   273,   614,   274,   448,     0,   688,     0,   693,     0,
     338,   339,   342,     0,     0,   264,     0,   646,     0,     0,
     340,     0,   341,     0,   340,   341,     0,   139,   337,   650,
       0,   102,   337,   653,   360,     0,   189,   646,     0,   141,
     646,     0,   164,   118,   646,     0,     0,   343,     0,   344,
       0,   343,   344,     0,   349,     0,   346,     0,   345,     0,
      46,    23,   164,   679,     0,   263,    94,   646,     0,    75,
     337,   653,     0,    25,   337,   653,     0,   127,   337,   650,
       0,   109,   337,   651,     0,   348,     0,   347,     0,   103,
     273,   350,   274,   136,   351,     0,   103,    15,   351,     0,
     103,     0,   355,   646,   356,     0,   351,     0,   350,   272,
     351,     0,   352,   353,     0,   646,     0,     0,   353,   354,
       0,   168,   337,   653,     0,    94,     0,     0,   357,     0,
     358,     0,   357,   358,     0,   175,   359,   653,     0,   102,
     337,   653,   360,     0,     0,    12,     0,    12,   137,     0,
       0,   137,     0,   138,     0,   573,   363,   273,   364,   274,
       0,   573,   363,   273,   364,   274,     0,    62,    94,   646,
       0,    62,   646,     0,     0,   365,     0,   364,   272,   365,
       0,   366,     0,   381,     0,   371,   369,   375,   448,   377,
     370,     0,   371,   483,   367,     0,   371,   367,     0,   368,
     273,   447,   635,   448,   274,     0,    30,    20,     0,    30,
       0,   482,   447,     0,   612,   445,     0,    26,   680,     0,
       0,   612,     0,   612,     0,   374,   482,     0,     0,    46,
     376,     0,     0,   639,     0,   643,     0,   644,     0,   645,
       0,   676,     0,   636,     0,     0,   378,     0,   379,     0,
     378,   379,     0,   382,   380,     0,   124,    98,     0,   153,
     573,   605,   389,   387,     0,   388,     0,   187,   387,     0,
     146,    90,   387,     0,   382,   383,     0,    33,   682,     0,
       0,   384,     0,   385,     0,   386,     0,   388,     0,   187,
     606,   387,     0,   146,    90,   606,   387,     0,    65,    90,
     606,   153,   573,   605,   389,   387,     0,   247,   540,    83,
     689,     0,     0,   447,    24,   273,   614,   274,   448,     0,
     390,     0,   391,     0,   391,   390,     0,   390,   391,     0,
       0,   129,   188,   392,     0,   129,    47,   392,     0,   211,
       0,   164,    46,     0,   164,    98,     0,   123,   209,     0,
     692,   397,   398,    10,   445,   401,   407,   448,     0,   692,
     397,   398,    10,   445,   401,   407,   448,     0,   692,   397,
     398,    10,   445,   401,   407,   448,     0,   692,   397,   398,
      10,   445,   401,   407,   448,     0,   273,   399,   274,     0,
       0,   157,   397,     0,     0,   400,     0,   399,   272,   400,
       0,   372,   483,     0,   402,     0,     0,   403,     0,   402,
     403,     0,    45,   404,   371,   483,   405,   271,     0,   192,
       0,     0,   264,   376,     0,   375,     0,   410,     0,   407,
       0,    17,   408,    54,     0,   409,     0,   409,   431,     0,
       0,   406,     0,   409,   406,     0,   598,   271,     0,   590,
     271,     0,   411,     0,   412,     0,   413,     0,   414,     0,
     416,     0,   419,     0,   588,   271,     0,   420,     0,   422,
       0,   594,   271,     0,   427,     0,   417,     0,   418,     0,
     178,   271,     0,    61,   271,     0,   429,     0,    58,   685,
     271,     0,    58,   685,   635,   271,     0,    58,   271,     0,
      59,   148,   692,   424,   425,   271,     0,    59,   415,   635,
     271,     0,   191,     0,   254,     0,   428,    64,   535,    86,
     426,   430,    50,   406,     0,   428,    64,    59,   415,   635,
      86,   426,    50,   406,     0,    59,   415,   635,    86,   426,
     271,     0,    78,   273,   614,   274,   181,   406,    53,   406,
       0,    78,   273,   614,   274,   181,   406,     0,   144,   635,
     421,   271,     0,     0,   535,    86,   426,   271,     0,   275,
     698,     0,   570,     0,   273,   570,   274,     0,     0,   156,
     426,     0,   156,   273,   426,   274,     0,     0,   423,     0,
     611,     0,   426,   272,   611,     0,   426,   272,   423,     0,
     428,   199,   273,   614,   274,    50,   406,     0,   691,   275,
       0,     0,   235,   271,     0,   255,   271,     0,   255,   691,
     271,     0,    10,    39,   683,     0,     0,   432,     0,   431,
     432,     0,   197,   433,    50,   406,     0,   434,     0,   433,
     272,   434,     0,   173,   647,     0,    69,   687,     0,    58,
     685,     0,     9,     0,    59,   148,   692,   424,     0,   699,
     605,    10,   445,   438,   449,   446,     0,   699,   605,    10,
     445,   438,   449,   446,     0,   439,     0,   440,     0,   439,
     186,   440,     0,   439,   186,     6,   440,     0,   163,   555,
     556,   441,   579,   575,   578,     0,    66,   442,     0,   443,
       0,   442,   272,   443,     0,   444,     0,   572,     0,   563,
       0,   443,   574,    89,   443,   129,   614,     0,   273,   444,
     274,     0,     0,     0,     0,     0,   200,    24,   131,     0,
       0,   696,    64,   573,   452,   453,   456,   447,   457,   448,
       0,   696,    64,   573,   452,   453,   456,   447,   457,   448,
       0,     3,     0,    80,     0,     0,   454,   455,     0,    16,
       0,     5,     0,    84,     0,   188,     0,    47,     0,    84,
     132,   188,     0,    84,   132,    47,     0,   188,   132,    84,
       0,   188,   132,    47,     0,    47,   132,    84,     0,    47,
     132,   188,     0,    84,   132,   188,   132,    47,     0,    84,
     132,    47,   132,   188,     0,   188,   132,    84,   132,    47,
       0,   188,   132,    47,   132,    84,     0,    47,   132,    84,
     132,   188,     0,    47,   132,   188,   132,    84,     0,   143,
     648,     0,     0,    10,   447,   401,   407,     0,     7,   459,
       0,    58,   685,   646,     0,   180,   573,   464,     0,   184,
     477,     0,   148,   396,     0,    40,   473,   474,     0,    51,
     466,   462,     0,    83,   472,     0,    46,   447,   376,     0,
      24,   447,   273,   614,   274,   448,     0,   463,     0,   462,
     463,     0,   164,   447,   460,   448,     0,     4,    33,   461,
       0,     4,   333,     0,    52,    46,     0,    52,    33,     0,
     182,   612,     0,   216,   374,   483,     0,   465,     0,   464,
     272,   465,     0,    52,   612,   471,     0,    52,    33,   682,
       0,     4,   366,     0,     4,   381,     0,   468,   612,   143,
     650,     0,   468,   466,   182,   612,     0,   468,   470,   216,
     469,   448,     0,   467,     0,   701,     0,   215,     0,   216,
       0,   217,     0,   218,     0,   219,     0,   220,     0,   221,
       0,   222,     0,   223,     0,   224,     0,   225,     0,   226,
       0,   227,     0,   228,     0,   229,     0,   230,     0,   233,
       0,   234,     0,   241,     0,   242,     0,   250,     0,   252,
       0,     7,     0,     7,   215,     0,   483,   447,     0,   612,
     445,     0,   612,     0,   213,     0,   211,     0,     0,   689,
       3,     0,   689,    80,     0,     0,   475,     0,   474,   475,
       0,     4,   324,     0,    52,   103,     0,   164,   476,     0,
       4,   346,     0,     4,   263,    94,   646,     0,    52,   263,
      94,     0,    17,   262,     0,    54,   262,     0,   345,     0,
     476,   272,   345,     0,   696,   452,   478,   456,   447,   479,
     448,     0,   453,     0,     0,   457,     0,     0,    52,   481,
       0,    58,   685,     0,    83,   689,     0,   148,   692,     0,
     180,   695,     0,   184,   696,     0,   195,   699,     0,    63,
     686,     0,    51,   684,     0,    62,    68,   677,     0,   165,
     650,     0,   214,   693,     0,    71,   688,     0,   483,     0,
     484,     0,   487,     0,   490,     0,   488,   276,   485,   277,
       0,   495,   276,   485,   277,   493,     0,   486,     0,   485,
     272,   486,     0,   652,     0,   652,   275,   652,     0,   488,
       0,   495,   493,     0,   494,     0,   499,     0,   502,     0,
     243,     0,   489,     0,   169,     0,    41,     0,   226,     0,
     227,     0,    85,     0,    96,     0,    19,   492,   491,   493,
       0,    19,   273,   651,   274,     0,    19,   273,   651,   272,
     647,   274,     0,    19,   273,   272,   647,   274,     0,   162,
     168,   651,     0,     0,   177,   647,     0,   177,   678,     0,
       0,    23,   164,   679,     0,     0,   498,   273,   650,   274,
       0,   498,     0,   498,   193,   273,   650,   274,     0,   497,
     273,   650,   274,     0,   497,     0,   496,   273,   650,   274,
       0,   191,     0,    23,   193,     0,    91,   193,     0,    23,
       0,    91,     0,   121,     0,   119,    23,     0,   119,    91,
       0,    99,   500,     0,   501,   500,     0,     0,   273,   652,
     274,     0,   273,   652,   272,   652,   274,     0,    44,     0,
      92,     0,    95,   503,     0,    97,    95,   503,     0,   152,
       0,    93,   145,     0,   273,   648,   274,     0,     0,   517,
       0,   505,     0,   534,     0,   164,    71,   688,   182,   652,
       0,   164,    71,   688,   182,   231,     0,   164,    71,   688,
     182,   268,   231,     0,   507,     0,   508,     0,   510,     0,
     252,   700,     0,   253,   252,   700,   509,     0,   130,     0,
       0,   161,   514,   182,   511,   700,     0,   252,     0,     0,
      28,   514,   515,     0,   161,   514,     0,   201,     0,     0,
     155,   516,     0,     0,   170,     0,     0,   164,   183,   518,
       0,   519,     0,     0,   520,     0,   519,   520,     0,   521,
       0,   522,     0,   523,     0,   527,     0,   151,   130,     0,
     151,   202,     0,   196,     0,   123,   196,     0,    88,   107,
     524,     0,   524,     0,   525,     0,   151,   185,   526,     0,
     151,    29,   526,     0,   170,     0,   170,   180,     0,   170,
     180,   174,     0,   194,     0,   123,   194,     0,     0,   154,
     530,     0,   166,     0,   149,     0,     0,   151,     0,   202,
       0,   531,     0,   530,   272,   531,     0,   533,   532,     0,
      64,   528,   529,     0,     0,   573,     0,   533,   272,   573,
       0,   164,   176,    83,   689,     0,   536,   537,   543,   545,
       0,   546,     0,   536,   186,   546,     0,   536,   186,     6,
     546,     0,   133,    20,   538,     0,     0,   539,     0,   538,
     272,   539,     0,   635,   540,   542,     0,    11,     0,    48,
       0,     0,   239,     0,   249,     0,   248,   550,   541,   551,
       0,     0,    64,   188,   544,     0,     0,   128,   607,     0,
       0,   200,   251,     0,     0,   163,   552,   555,   556,   560,
     579,   575,   578,   580,     0,   163,   552,   555,   556,   560,
     579,   575,   578,   580,   537,     0,     0,     0,     0,     0,
     553,   554,   549,     0,   553,   549,     0,   554,     0,     0,
     239,   653,   548,     0,   239,   273,   635,   274,   548,     0,
     239,   642,   548,     0,   240,   653,     0,   240,   273,   549,
     635,   274,     0,   240,   642,     0,    49,     0,   675,     0,
     557,     0,   269,     0,   558,     0,   557,   272,   558,     0,
     599,     0,   599,   559,   690,     0,    10,     0,     0,    66,
     561,     0,   562,     0,   561,   272,   562,     0,   567,     0,
     568,     0,   563,     0,   273,   536,   274,   559,   564,   565,
       0,   694,     0,     0,   273,   566,   274,     0,     0,   690,
       0,   566,   272,   690,     0,   562,   574,    89,   562,   129,
     614,     0,   273,   567,   274,     0,   692,   569,   559,   694,
       0,   692,   569,     0,   273,   570,   274,     0,     0,   571,
       0,   570,   272,   571,     0,   676,     0,   635,     0,   573,
       0,   695,   694,     0,   695,     0,    81,     0,   105,     0,
     105,   134,     0,   159,     0,   159,   134,     0,    67,     0,
      67,   134,     0,     0,    74,    20,   576,     0,     0,   577,
       0,   576,   272,   577,     0,   635,     0,    77,   614,     0,
       0,   198,   614,     0,     0,   142,   581,     0,     0,   582,
     273,   583,   274,     0,    89,     0,   172,   114,     0,   114,
       0,   172,     0,     0,   584,     0,   584,   272,   583,     0,
     585,   586,     0,   581,     0,   695,     0,   695,   585,     0,
     120,     0,    83,   273,   587,   274,     0,   133,   689,     0,
     689,     0,   689,   272,   587,     0,    84,    86,   573,   608,
     190,   273,   589,   274,     0,    84,    86,   573,   608,   547,
       0,   599,     0,   589,   272,   599,     0,   591,     0,   592,
       0,    47,    66,   572,   579,     0,    47,    66,   572,   593,
       0,   198,    38,   128,   683,     0,   595,     0,   596,     0,
     188,   572,   164,   597,   579,     0,   188,   572,   164,   597,
     593,     0,   598,     0,   597,   272,   598,     0,   613,   264,
     599,     0,   635,     0,   676,     0,   151,    19,   612,    66,
     573,   601,   603,     0,    84,    19,   612,    86,   573,   601,
     603,     0,    63,    66,   602,   182,   602,     0,    63,   182,
     602,     0,     0,   492,     0,   642,     0,   647,     0,   113,
     604,     0,     0,   651,     0,   642,     0,   606,     0,     0,
     273,   607,   274,     0,   612,     0,   607,   272,   612,     0,
     609,     0,     0,   273,   610,   274,     0,   613,     0,   610,
     272,   613,     0,   612,     0,   694,   278,   681,     0,   694,
     278,   269,     0,   681,     0,   612,     0,   694,   278,   681,
       0,   629,     0,   124,   629,     0,   616,     0,   614,   132,
     614,     0,   614,     8,   614,     0,   616,     0,   124,   629,
       0,   615,   132,   614,     0,   615,     8,   614,     0,   630,
     132,   614,     0,   630,     8,   614,     0,   617,     0,   273,
     615,   274,     0,   124,   616,     0,   618,     0,   621,     0,
     622,     0,   623,     0,   628,     0,   619,     0,   626,     0,
     624,     0,   625,     0,   627,     0,   635,   264,   635,     0,
     635,   265,   635,     0,   635,   266,   635,     0,   635,    70,
     635,     0,   635,   106,   635,     0,   635,   125,   635,     0,
     635,   126,   635,     0,   635,   122,   635,     0,   635,   264,
       6,   273,   633,   274,     0,   635,   265,     6,   273,   633,
     274,     0,   635,   266,     6,   273,   633,   274,     0,   635,
      70,     6,   273,   633,   274,     0,   635,   106,     6,   273,
     633,   274,     0,   635,   125,     6,   273,   633,   274,     0,
     635,   126,     6,   273,   633,   274,     0,   635,   122,     6,
     273,   633,   274,     0,   635,   264,   620,   273,   633,   274,
       0,   635,   265,   620,   273,   633,   274,     0,   635,   266,
     620,   273,   633,   274,     0,   635,    70,   620,   273,   633,
     274,     0,   635,   106,   620,   273,   633,   274,     0,   635,
     125,   620,   273,   633,   274,     0,   635,   126,   620,   273,
     633,   274,     0,   635,   122,   620,   273,   633,   274,     0,
     171,     0,     9,     0,   635,    18,   635,     8,   635,     0,
     635,   124,    18,   635,     8,   635,     0,   635,   108,   635,
       0,   635,   124,   108,   635,     0,   635,   108,   635,    57,
     635,     0,   635,   124,   108,   635,    57,   635,     0,   635,
      79,   631,     0,   635,   124,    79,   631,     0,   635,    34,
     635,     0,   635,   124,    34,   635,     0,   635,   175,   635,
       0,   635,   124,   175,   635,     0,   635,   175,   200,   635,
       0,   635,   124,   175,   200,   635,     0,    60,   273,   547,
     274,     0,   167,   273,   547,   274,     0,   635,    87,    98,
       0,   635,    87,   124,    98,     0,   256,     0,   257,     0,
     258,     0,   259,     0,   260,     0,   261,     0,   632,     0,
     273,   638,   274,     0,   273,   633,   274,     0,   163,   552,
     555,   635,   560,   579,   575,   578,   580,   537,     0,   163,
     552,   555,   635,   560,   579,   575,   578,   580,   537,     0,
     611,     0,   637,     0,   654,     0,   641,     0,   642,     0,
     423,     0,   663,     0,   664,     0,   662,     0,   268,   635,
       0,   267,   635,     0,   635,   267,   635,     0,   635,    31,
     635,     0,   635,    26,   680,     0,   635,   268,   635,     0,
     635,   269,   635,     0,   635,   270,   635,     0,   273,   635,
     274,     0,   273,   634,   274,     0,   643,     0,   644,     0,
     645,     0,    42,     0,   694,   278,    42,     0,   101,     0,
     636,     0,   228,     0,   229,     0,   230,     0,   611,   276,
     638,   277,     0,   635,     0,   638,   272,   635,     0,   641,
       0,   268,   640,     0,   205,     0,   204,     0,   203,     0,
     231,     0,   232,     0,   640,     0,   646,     0,    41,   207,
       0,   226,   207,     0,   227,   207,     0,   279,     0,   189,
       0,   233,     0,   234,     0,   241,     0,   242,     0,    69,
       0,   173,     0,   250,     0,   207,     0,   208,   207,     0,
     648,     0,   268,   649,     0,   204,     0,   204,     0,   648,
       0,   204,     0,   653,     0,   268,   653,     0,   204,     0,
     655,     0,   656,     0,   657,     0,   659,     0,    35,   273,
     269,   274,     0,    35,   273,   675,   635,   274,     0,    35,
     273,    49,   635,   274,     0,   179,   273,   675,   635,   274,
       0,   179,   273,    49,   635,   274,     0,    13,   273,   675,
     635,   274,     0,    13,   273,    49,   635,   274,     0,   116,
     273,   675,   635,   274,     0,   116,   273,    49,   635,   274,
       0,   112,   273,   675,   635,   274,     0,   112,   273,    49,
     635,   274,     0,    72,   273,   688,   272,   635,   274,     0,
     658,     0,   217,   273,   674,    66,   635,   274,     0,   660,
       0,   100,   273,   635,   274,     0,   236,   273,   635,    66,
     635,   661,   274,     0,    64,   635,     0,     0,   677,   273,
     638,   274,     0,   677,   273,   274,     0,    22,   273,   599,
      10,   373,   274,     0,   665,     0,   666,     0,   245,   273,
     635,   272,   635,   274,     0,   246,   273,   571,   272,   570,
     274,     0,   667,     0,   669,     0,   244,   672,   668,    54,
       0,   244,   672,   668,    53,   673,    54,     0,   197,   671,
     181,   673,     0,   668,   197,   671,   181,   673,     0,   244,
     670,    54,     0,   244,   670,    53,   673,    54,     0,   197,
     614,   181,   673,     0,   670,   197,   614,   181,   673,     0,
     635,     0,   635,     0,   571,     0,   218,     0,   219,     0,
     220,     0,   221,     0,   222,     0,   223,     0,   224,     0,
     225,     0,     6,     0,     0,    98,     0,   206,     0,   701,
       0,   701,     0,   701,     0,   701,     0,   701,     0,   701,
       0,   701,     0,   701,     0,   701,     0,   701,     0,   701,
       0,   701,     0,   701,     0,   701,     0,   701,     0,   701,
       0,   701,     0,   701,     0,   701,     0,   701,     0,   701,
       0,   701,     0,   701,     0,   206,     0,   702,     0,   235,
       0,   238,     0,   236,     0,   246,     0,   249,     0,   255,
       0,   251,     0,   245,     0,   248,     0,   254,     0,   247,
       0,   256,     0,   257,     0,   258,     0,   262,     0,   263,
       0
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
    1421,  1424,  1427,  1430,  1439,  1451,  1462,  1473,  1484,  1486,
    1490,  1492,  1496,  1497,  1501,  1507,  1509,  1513,  1514,  1518,
    1523,  1525,  1529,  1531,  1535,  1536,  1539,  1543,  1545,  1547,
    1551,  1552,  1556,  1557,  1558,  1559,  1560,  1561,  1562,  1563,
    1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1573,  1578,
    1581,  1583,  1587,  1591,  1596,  1600,  1600,  1602,  1607,  1611,
    1615,  1617,  1621,  1625,  1631,  1636,  1641,  1643,  1645,  1649,
    1651,  1653,  1657,  1658,  1659,  1661,  1665,  1669,  1671,  1675,
    1677,  1679,  1684,  1686,  1689,  1690,  1694,  1699,  1700,  1704,
    1706,  1708,  1710,  1717,  1725,  1732,  1752,  1756,  1758,  1760,
    1764,  1775,  1779,  1780,  1784,  1785,  1786,  1789,  1792,  1798,
    1802,  1808,  1812,  1818,  1822,  1830,  1841,  1852,  1854,  1856,
    1860,  1864,  1866,  1870,  1872,  1874,  1876,  1878,  1880,  1882,
    1884,  1886,  1888,  1890,  1892,  1894,  1896,  1898,  1902,  1904,
    1908,  1914,  1918,  1921,  1928,  1930,  1932,  1935,  1938,  1943,
    1947,  1953,  1954,  1958,  1967,  1971,  1973,  1975,  1977,  1979,
    1983,  1984,  1988,  1990,  1992,  1994,  2000,  2003,  2005,  2009,
    2017,  2018,  2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,
    2027,  2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2039,  2042,  2044,  2048,  2050,  2055,  2061,  2063,
    2065,  2069,  2071,  2078,  2084,  2085,  2089,  2097,  2099,  2101,
    2103,  2105,  2107,  2109,  2113,  2114,  2121,  2131,  2132,  2136,
    2137,  2143,  2147,  2149,  2151,  2153,  2155,  2157,  2159,  2161,
    2163,  2165,  2167,  2169,  2176,  2177,  2180,  2181,  2184,  2189,
    2196,  2197,  2201,  2210,  2216,  2217,  2220,  2221,  2222,  2223,
    2240,  2245,  2250,  2269,  2286,  2293,  2294,  2301,  2305,  2311,
    2317,  2325,  2329,  2335,  2339,  2343,  2349,  2353,  2360,  2366,
    2372,  2380,  2385,  2390,  2397,  2398,  2399,  2402,  2403,  2406,
    2407,  2408,  2415,  2419,  2430,  2436,  2490,  2552,  2553,  2560,
    2573,  2578,  2583,  2590,  2592,  2599,  2600,  2601,  2605,  2610,
    2615,  2626,  2627,  2628,  2631,  2635,  2639,  2641,  2645,  2649,
    2650,  2653,  2657,  2661,  2662,  2665,  2667,  2671,  2672,  2676,
    2680,  2681,  2685,  2686,  2690,  2691,  2692,  2693,  2696,  2698,
    2702,  2704,  2708,  2710,  2713,  2715,  2717,  2721,  2723,  2725,
    2729,  2731,  2733,  2737,  2741,  2743,  2745,  2749,  2751,  2755,
    2756,  2760,  2764,  2766,  2770,  2771,  2776,  2784,  2788,  2790,
    2792,  2796,  2798,  2802,  2803,  2807,  2811,  2813,  2815,  2819,
    2821,  2825,  2827,  2831,  2833,  2837,  2839,  2843,  2845,  2852,
    2864,  2877,  2881,  2885,  2889,  2893,  2895,  2897,  2899,  2903,
    2905,  2907,  2911,  2913,  2915,  2919,  2921,  2925,  2927,  2931,
    2932,  2936,  2937,  2941,  2942,  2947,  2951,  2952,  2956,  2957,
    2958,  2962,  2967,  2968,  2972,  2974,  2978,  2979,  2983,  2985,
    2989,  2992,  2997,  2999,  3003,  3004,  3008,  3009,  3012,  3013,
    3018,  3023,  3025,  3027,  3029,  3031,  3033,  3035,  3037,  3044,
    3046,  3050,  3051,  3057,  3060,  3062,  3066,  3068,  3075,  3077,
    3081,  3085,  3087,  3089,  3095,  3097,  3101,  3102,  3106,  3108,
    3111,  3112,  3116,  3118,  3120,  3124,  3125,  3133,  3136,  3140,
    3141,  3148,  3149,  3152,  3156,  3160,  3167,  3168,  3171,  3176,
    3181,  3182,  3186,  3190,  3191,  3197,  3199,  3203,  3205,  3207,
    3210,  3211,  3214,  3218,  3220,  3223,  3225,  3231,  3232,  3236,
    3240,  3241,  3246,  3247,  3251,  3255,  3256,  3261,  3262,  3265,
    3270,  3275,  3278,  3285,  3286,  3288,  3289,  3291,  3295,  3296,
    3298,  3300,  3304,  3306,  3310,  3311,  3313,  3317,  3318,  3319,
    3320,  3321,  3322,  3323,  3324,  3325,  3326,  3331,  3333,  3335,
    3337,  3339,  3341,  3343,  3345,  3352,  3354,  3356,  3358,  3360,
    3362,  3364,  3366,  3368,  3370,  3372,  3374,  3376,  3378,  3380,
    3382,  3386,  3387,  3393,  3395,  3400,  3402,  3404,  3406,  3411,
    3413,  3417,  3419,  3423,  3425,  3427,  3429,  3433,  3437,  3441,
    3443,  3447,  3452,  3457,  3464,  3469,  3474,  3483,  3484,  3488,
    3492,  3505,  3522,  3523,  3524,  3525,  3526,  3527,  3528,  3529,
    3530,  3531,  3533,  3535,  3542,  3544,  3546,  3553,  3560,  3567,
    3569,  3571,  3572,  3573,  3574,  3576,  3578,  3582,  3586,  3602,
    3618,  3622,  3626,  3627,  3631,  3632,  3636,  3638,  3640,  3642,
    3644,  3648,  3649,  3651,  3667,  3683,  3687,  3691,  3693,  3697,
    3701,  3704,  3707,  3710,  3713,  3718,  3720,  3725,  3726,  3730,
    3737,  3744,  3751,  3758,  3759,  3763,  3769,  3770,  3771,  3772,
    3775,  3777,  3779,  3782,  3789,  3798,  3805,  3814,  3816,  3818,
    3820,  3826,  3835,  3838,  3842,  3843,  3847,  3856,  3858,  3862,
    3864,  3868,  3874,  3875,  3878,  3882,  3886,  3887,  3890,  3892,
    3896,  3898,  3902,  3904,  3908,  3910,  3914,  3917,  3920,  3923,
    3925,  3927,  3929,  3931,  3933,  3935,  3937,  3941,  3942,  3945,
    3953,  3956,  3959,  3962,  3965,  3968,  3971,  3974,  3977,  3980,
    3983,  3986,  3989,  3992,  3995,  3998,  4001,  4004,  4007,  4010,
    4013,  4016,  4019,  4022,  4027,  4028,  4033,  4035,  4036,  4037,
    4038,  4039,  4040,  4041,  4042,  4043,  4044,  4045,  4046,  4047,
    4049,  4050
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
  "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", 
  "')'", "':'", "'['", "']'", "'.'", "'?'", "top", "statement", "grant", 
  "prot_table_name", "privileges", "privilege_list", "proc_privileges", 
  "privilege", "grant_option", "role_admin_option", "simple_proc_name", 
  "revoke", "rev_grant_option", "grantee_list", "grantee", 
  "user_grantee_list", "user_grantee", "role_name_list", "role_name", 
  "role_grantee_list", "role_grantee", "declare", "declare_clause", 
  "udf_decl_clause", "udf_data_type", "arg_desc_list1", "arg_desc_list", 
  "arg_desc", "return_value1", "return_value", "filter_decl_clause", 
  "create", "create_clause", "recreate", "recreate_clause", "replace", 
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
  "input_parameters", "output_parameters", "proc_parameters", 
  "proc_parameter", "var_declaration_list", "var_declarations", 
  "var_declaration", "var_decl_opt", "var_init_opt", "proc_block", 
  "full_proc_block", "full_proc_block_body", "proc_statements", 
  "proc_statement", "excp_statement", "raise_statement", "exec_procedure", 
  "exec_sql", "varstate", "for_select", "for_exec_into", "exec_into", 
  "if_then_else", "post_event", "event_argument_opt", "singleton_select", 
  "variable", "proc_inputs", "proc_outputs", "variable_list", "while", 
  "label_opt", "breakleave", "cursor_def", "excp_hndl_statements", 
  "excp_hndl_statement", "errors", "err", "invoke_procedure", 
  "view_clause", "rview_clause", "union_view", "union_view_expr", 
  "select_view_expr", "from_view_clause", "from_view_list", "view_table", 
  "joined_view_table", "begin_string", "end_string", "begin_trigger", 
  "end_trigger", "check_opt", "def_trigger_clause", 
  "replace_trigger_clause", "trigger_active", "trigger_type", 
  "trigger_type_prefix", "trigger_type_suffix", "trigger_position", 
  "trigger_action", "alter", "alter_clause", "domain_default_opt2", 
  "domain_check_constraint2", "alter_domain_ops", "alter_domain_op", 
  "alter_ops", "alter_op", "alter_column_name", "keyword_or_column", 
  "col_opt", "alter_data_type_or_domain", "alter_col_name", 
  "drop_behaviour", "alter_index_clause", "init_alter_db", "alter_db", 
  "db_alter_clause", "db_log_option_list", "alter_trigger_clause", 
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
  "order_direction", "nulls_placement", "nulls_clause", 
  "for_update_clause", "for_update_list", "lock_clause", "select_expr", 
  "ordered_select_expr", "begin_limit", "end_limit", "begin_first", 
  "end_first", "limit_clause", "first_clause", "skip_clause", 
  "distinct_clause", "select_list", "select_items", "select_item", 
  "as_noise", "from_clause", "from_list", "table_reference", 
  "derived_table", "correlation_name", "derived_column_list", 
  "alias_list", "joined_table", "table_proc", "proc_table_inputs", 
  "null_or_value_list", "null_or_value", "table_name", 
  "simple_table_name", "join_type", "group_clause", "group_by_list", 
  "group_by_item", "having_clause", "where_clause", "plan_clause", 
  "plan_expression", "plan_type", "plan_item_list", "plan_item", 
  "table_or_alias_list", "access_type", "index_list", "insert", 
  "insert_value_list", "delete", "delete_searched", "delete_positioned", 
  "cursor_clause", "update", "update_searched", "update_positioned", 
  "assignments", "assignment", "rhs", "blob", "filter_clause", 
  "blob_subtype_value", "segment_clause", "segment_length", 
  "column_parens_opt", "column_parens", "column_list", 
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
       0,   280,   280,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   282,   282,   282,   282,   282,   283,   283,   284,
     284,   284,   285,   285,   286,   287,   287,   287,   287,   287,
     288,   288,   289,   289,   290,   291,   291,   291,   291,   291,
     291,   291,   292,   293,   293,   293,   293,   294,   294,   294,
     294,   295,   295,   296,   296,   296,   297,   297,   298,   299,
     299,   300,   300,   301,   302,   302,   303,   304,   304,   304,
     305,   305,   305,   306,   306,   307,   307,   308,   308,   309,
     309,   309,   309,   309,   310,   311,   312,   312,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   313,   314,   314,
     314,   315,   316,   316,   317,   317,   318,   318,   318,   319,
     320,   320,   320,   321,   321,   322,   322,   323,   323,   324,
     324,   325,   326,   326,   327,   327,   328,   328,   329,   329,
     330,   331,   331,   332,   333,   334,   335,   336,   337,   337,
     338,   339,   339,   340,   340,   341,   341,   341,   341,   341,
     342,   342,   343,   343,   344,   344,   344,   344,   344,   345,
     345,   345,   345,   346,   346,   347,   347,   348,   349,   350,
     350,   351,   352,   353,   353,   354,   355,   356,   356,   357,
     357,   358,   358,   359,   359,   359,   360,   360,   360,   361,
     362,   363,   363,   363,   364,   364,   365,   365,   366,   366,
     366,   367,   368,   368,   369,   369,   370,   370,   371,   372,
     373,   374,   375,   375,   376,   376,   376,   376,   376,   376,
     377,   377,   378,   378,   379,   380,   380,   380,   380,   380,
     381,   382,   382,   383,   383,   383,   383,   384,   385,   386,
     387,   387,   388,   389,   389,   389,   389,   389,   390,   391,
     392,   392,   392,   392,   393,   394,   395,   396,   397,   397,
     398,   398,   399,   399,   400,   401,   401,   402,   402,   403,
     404,   404,   405,   405,   406,   406,   407,   408,   408,   408,
     409,   409,   410,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
     411,   411,   412,   413,   414,   415,   415,   416,   417,   418,
     419,   419,   420,   421,   422,   423,   424,   424,   424,   425,
     425,   425,   426,   426,   426,   426,   427,   428,   428,   429,
     429,   429,   430,   430,   431,   431,   432,   433,   433,   434,
     434,   434,   434,   435,   436,   437,   438,   439,   439,   439,
     440,   441,   442,   442,   443,   443,   443,   444,   444,   445,
     446,   447,   448,   449,   449,   450,   451,   452,   452,   452,
     453,   454,   454,   455,   455,   455,   455,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   456,   456,
     457,   458,   459,   459,   459,   459,   459,   459,   459,   460,
     461,   462,   462,   463,   463,   463,   463,   463,   463,   463,
     464,   464,   465,   465,   465,   465,   465,   465,   465,   466,
     467,   467,   467,   467,   467,   467,   467,   467,   467,   467,
     467,   467,   467,   467,   467,   467,   467,   467,   467,   467,
     467,   467,   467,   468,   468,   469,   469,   470,   471,   471,
     471,   472,   472,   473,   474,   474,   475,   475,   475,   475,
     475,   475,   475,   475,   476,   476,   477,   478,   478,   479,
     479,   480,   481,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   482,   482,   483,   483,   484,   484,
     485,   485,   486,   486,   487,   487,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   489,   489,   490,   490,   490,
     490,   491,   491,   492,   492,   492,   493,   493,   494,   494,
     494,   495,   495,   495,   496,   496,   496,   497,   497,   498,
     498,   498,   499,   499,   500,   500,   500,   501,   501,   502,
     502,   502,   502,   503,   503,   504,   504,   504,   505,   505,
     505,   506,   506,   506,   507,   508,   509,   509,   510,   511,
     511,   512,   513,   514,   514,   515,   515,   516,   516,   517,
     518,   518,   519,   519,   520,   520,   520,   520,   521,   521,
     522,   522,   523,   523,   524,   524,   524,   525,   525,   525,
     526,   526,   526,   527,   528,   528,   528,   529,   529,   530,
     530,   531,   532,   532,   533,   533,   534,   535,   536,   536,
     536,   537,   537,   538,   538,   539,   540,   540,   540,   541,
     541,   542,   542,   543,   543,   544,   544,   545,   545,   546,
     547,   548,   549,   550,   551,   552,   552,   552,   552,   553,
     553,   553,   554,   554,   554,   555,   555,   556,   556,   557,
     557,   558,   558,   559,   559,   560,   561,   561,   562,   562,
     562,   563,   564,   564,   565,   565,   566,   566,   567,   567,
     568,   568,   569,   569,   570,   570,   571,   571,   572,   572,
     573,   574,   574,   574,   574,   574,   574,   574,   574,   575,
     575,   576,   576,   577,   578,   578,   579,   579,   580,   580,
     581,   582,   582,   582,   582,   582,   583,   583,   584,   584,
     585,   585,   586,   586,   586,   587,   587,   588,   588,   589,
     589,   590,   590,   591,   592,   593,   594,   594,   595,   596,
     597,   597,   598,   599,   599,   600,   600,   601,   601,   601,
     602,   602,   492,   603,   603,   604,   604,   605,   605,   606,
     607,   607,   608,   608,   609,   610,   610,   611,   611,   611,
     612,   613,   613,   614,   614,   614,   614,   614,   615,   615,
     615,   615,   615,   615,   616,   616,   616,   617,   617,   617,
     617,   617,   617,   617,   617,   617,   617,   618,   618,   618,
     618,   618,   618,   618,   618,   619,   619,   619,   619,   619,
     619,   619,   619,   619,   619,   619,   619,   619,   619,   619,
     619,   620,   620,   621,   621,   622,   622,   622,   622,   623,
     623,   624,   624,   625,   625,   625,   625,   626,   627,   628,
     628,   629,   629,   629,   630,   630,   630,   631,   631,   632,
     633,   634,   635,   635,   635,   635,   635,   635,   635,   635,
     635,   635,   635,   635,   635,   635,   635,   635,   635,   635,
     635,   635,   635,   635,   635,   635,   635,   635,   636,   636,
     636,   637,   638,   638,   639,   639,   640,   640,   640,   640,
     640,   641,   641,   641,   641,   641,   642,   643,   643,   644,
     645,   645,   645,   645,   645,   646,   646,   647,   647,   648,
     649,   650,   651,   652,   652,   653,   654,   654,   654,   654,
     655,   655,   655,   655,   655,   655,   655,   655,   655,   655,
     655,   656,   657,   658,   659,   659,   660,   661,   661,   662,
     662,   663,   664,   664,   665,   665,   666,   666,   667,   667,
     668,   668,   669,   669,   670,   670,   671,   672,   673,   674,
     674,   674,   674,   674,   674,   674,   674,   675,   675,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   691,   692,   693,   694,   695,   696,
     697,   698,   699,   700,   701,   701,   702,   702,   702,   702,
     702,   702,   702,   702,   702,   702,   702,   702,   702,   702,
     702,   702
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
       2,     0,     1,     3,     2,     1,     0,     1,     2,     6,
       1,     0,     2,     1,     1,     1,     3,     1,     2,     0,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     1,     1,     2,     2,     1,
       3,     4,     2,     6,     4,     1,     1,     8,     9,     6,
       8,     6,     4,     0,     4,     2,     1,     3,     0,     2,
       4,     0,     1,     1,     3,     3,     7,     2,     0,     2,
       2,     3,     3,     0,     1,     2,     4,     1,     3,     2,
       2,     2,     1,     4,     7,     7,     1,     1,     3,     4,
       7,     2,     1,     3,     1,     1,     1,     6,     3,     0,
       0,     0,     0,     3,     0,     9,     9,     1,     1,     0,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     5,     5,     5,     5,     5,     5,     2,     0,
       4,     2,     3,     3,     2,     2,     3,     3,     2,     3,
       6,     1,     2,     4,     3,     2,     2,     2,     2,     3,
       1,     3,     3,     3,     2,     2,     4,     4,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       0,     2,     2,     0,     1,     2,     2,     2,     2,     2,
       4,     3,     2,     2,     1,     3,     7,     1,     0,     1,
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
       3,     2,     3,     0,     1,     3,     4,     4,     1,     3,
       4,     3,     0,     1,     3,     3,     1,     1,     0,     1,
       1,     4,     0,     3,     0,     2,     0,     2,     0,     9,
      10,     0,     0,     0,     0,     3,     2,     1,     0,     3,
       5,     3,     2,     5,     2,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     0,     2,     1,     3,     1,     1,
       1,     6,     1,     0,     3,     0,     1,     3,     6,     3,
       4,     2,     3,     0,     1,     3,     1,     1,     1,     2,
       1,     1,     1,     2,     1,     2,     1,     2,     0,     3,
       0,     1,     3,     1,     2,     0,     2,     0,     2,     0,
       4,     1,     2,     1,     1,     0,     1,     3,     2,     1,
       1,     2,     1,     4,     2,     1,     3,     8,     5,     1,
       3,     1,     1,     4,     4,     4,     1,     1,     5,     5,
       1,     3,     3,     1,     1,     7,     7,     5,     3,     0,
       1,     1,     1,     2,     0,     1,     1,     1,     0,     3,
       1,     3,     1,     0,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     1,     2,     1,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     1,     1,     5,     6,     3,     4,     5,     6,     3,
       4,     3,     4,     3,     4,     4,     5,     4,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
      10,    10,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     4,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       4,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     6,     1,     6,     1,     4,     7,     2,     0,     4,
       3,     6,     1,     1,     6,     6,     1,     1,     4,     6,
       4,     5,     3,     5,     4,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
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
       0,     0,   574,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,   648,     0,     0,     0,     0,     0,
       1,    10,    15,     7,     6,    13,    14,    12,     3,     9,
      19,   556,    17,   561,   562,   563,     5,    16,   555,   557,
      18,   622,   618,    11,     8,   731,   732,    20,   736,   737,
       4,   463,     0,     0,     0,     0,     0,     0,   401,   573,
     576,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,    95,   628,   909,     0,    21,   907,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,     0,    29,    37,    34,
      36,   758,    35,   758,   994,   996,   998,   997,  1003,   999,
    1006,  1004,  1000,  1002,  1005,  1001,  1007,  1008,  1009,  1010,
    1011,     0,    31,     0,    32,     0,    66,    68,   986,   995,
       0,     0,     0,     0,     0,     0,     0,     0,   572,     0,
       0,   968,   642,   647,     0,     0,   581,     0,   688,   690,
     988,     0,     0,     0,   107,   564,   993,     0,     2,     0,
       0,   634,     0,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,   429,   430,     0,   978,
     408,     0,   982,   405,   269,   985,     0,   690,   404,   379,
     989,   578,   571,   905,     0,   103,   151,   150,   104,   133,
     218,   770,   974,     0,   102,   145,   981,     0,    98,   269,
     105,   911,   122,    99,   203,   100,     0,   101,   758,   992,
     106,   146,   626,   627,     0,   910,   908,     0,    74,     0,
     979,   707,   489,   977,   482,     0,   488,   493,   483,   484,
     491,   485,   486,   487,   492,   328,    30,     0,    39,   757,
      38,     0,     0,     0,     0,     0,     0,   763,     0,     0,
       0,     0,     0,     0,     0,   570,   915,     0,   896,   641,
     641,   642,   654,   652,   967,   655,     0,   656,   646,   642,
       0,     0,     0,     0,     0,     0,   597,   590,   579,   580,
     582,   584,   585,   586,   593,   594,   587,     0,   689,   987,
     108,   269,   109,   203,   110,   758,   567,     0,     0,   619,
       0,   638,     0,     0,     0,     0,     0,   406,   464,   371,
       0,   371,     0,   221,   407,   411,   402,   461,   462,     0,
     271,   242,   453,     0,   403,   420,     0,   377,   378,   478,
     577,   575,   906,   148,   148,     0,     0,     0,   160,   152,
     153,   132,     0,    96,     0,     0,   111,   271,   121,   120,
     123,     0,     0,     0,     0,     0,   970,    75,   221,   525,
       0,   733,   734,   490,     0,     0,     0,     0,   874,   902,
       0,   969,     0,   876,     0,     0,   903,     0,   897,   888,
     887,   886,   994,     0,     0,     0,   878,   879,   880,   889,
     890,   898,   899,   998,   900,   901,     0,  1003,   999,   904,
       0,     0,     0,     0,   857,   353,   326,   684,   852,   767,
     687,   877,   853,   891,   855,   856,   871,   872,   873,   892,
     854,   916,   917,   918,   932,   919,   934,   860,   858,   859,
     942,   943,   946,   947,   686,     0,     0,   974,     0,   760,
       0,     0,    27,    33,     0,     0,    43,    69,    71,   990,
      67,     0,     0,     0,   762,     0,    52,     0,     0,     0,
       0,    51,   569,     0,     0,     0,   651,   649,     0,   658,
       0,   657,   659,   664,   743,   744,   645,     0,   616,     0,
     591,   602,   588,   602,   589,   603,   609,   613,   614,   598,
     583,   707,   740,   771,     0,     0,   271,     0,     0,   566,
     565,   621,   623,   628,   620,   636,     0,   617,   186,   177,
       0,   466,   469,   174,   173,   129,     0,   472,   467,     0,
     473,   148,   148,   148,   148,   474,   468,   465,     0,   415,
       0,   417,   416,     0,   418,     0,   412,     0,     0,   272,
     219,   269,     0,     0,   424,     0,   425,   371,   454,     0,
     460,     0,     0,     0,   457,   974,   382,   381,   477,     0,
     399,   149,     0,     0,   158,     0,   157,     0,     0,   147,
     161,   162,   166,   165,   164,   154,   525,   537,   512,   547,
     515,   538,   548,     0,   554,   516,     0,   544,     0,   539,
     551,   511,   534,   513,   514,   509,   371,   494,   495,   496,
     504,   510,   497,   506,   527,     0,   532,   529,   507,   544,
     508,   112,   269,   113,     0,     0,   124,     0,     0,   202,
     242,   379,   369,     0,   221,     0,    81,    83,     0,     0,
       0,   752,     0,     0,     0,     0,  1007,  1008,  1009,     0,
     706,   775,   784,   787,   792,   788,   789,   790,   794,   795,
     793,   796,   791,   773,     0,   968,     0,   968,   893,     0,
       0,   968,   968,   968,     0,   894,   895,     0,     0,   957,
       0,     0,     0,     0,   862,   861,   648,     0,     0,   687,
     325,   991,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   759,    28,     0,     0,    44,    72,     0,
       0,    26,   749,     0,   765,   648,     0,   728,   749,     0,
       0,     0,     0,   568,     0,   641,     0,     0,   707,     0,
     663,     0,   559,     0,   558,   913,     0,   592,     0,   600,
     596,   595,     0,   606,     0,   611,   599,     0,   738,   739,
       0,     0,     0,   242,   369,     0,   632,     0,   633,   637,
       0,     0,     0,   130,   187,   471,     0,     0,     0,     0,
       0,   371,   414,     0,   371,   372,   419,   504,   527,   274,
       0,   268,   270,   369,   241,   975,   213,   210,     0,   223,
     371,   494,   369,     0,     0,     0,   240,   243,   244,   245,
     246,     0,   423,   459,   458,   422,   421,     0,     0,     0,
     385,   383,   384,   380,     0,   371,   196,   155,   159,     0,
       0,   163,     0,   522,   535,   536,   552,     0,   549,   554,
       0,   542,   540,   541,   135,     0,     0,     0,   505,     0,
       0,     0,     0,   543,   271,     0,   369,   125,   201,     0,
     204,   206,   207,     0,     0,     0,     0,   221,   221,    78,
       0,    85,    77,   523,   524,   971,   525,     0,     0,     0,
     786,   774,     0,     0,   844,   845,   846,     0,   778,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   959,   960,   961,   962,   963,   964,   965,   966,
       0,     0,     0,     0,   952,     0,     0,     0,     0,     0,
     968,   327,   870,   869,   685,   882,     0,   865,   973,   864,
     863,   866,   867,   868,   940,     0,   875,   769,   768,   761,
       0,     0,     0,     0,     0,     0,    24,    53,    41,    61,
      63,     0,     0,    70,     0,   754,     0,   764,   968,     0,
     754,    49,    47,     0,     0,     0,   650,   653,     0,   665,
     666,   670,   668,   669,   683,     0,   700,   660,   662,   983,
     560,   914,   601,   610,   605,   604,     0,   615,   741,   742,
     772,   369,     0,     0,   624,   643,   625,   635,   176,   183,
     182,     0,   179,   470,   148,   193,   178,   188,   189,   170,
     169,   912,   172,   171,   475,     0,     0,     0,   413,   273,
     276,   212,   371,     0,   372,   214,   209,   215,     0,     0,
     251,     0,   427,   372,   371,   369,   426,     0,     0,     0,
     398,   480,   197,   198,   156,     0,   168,     0,     0,     0,
     527,     0,   550,     0,     0,   371,   372,     0,   500,   502,
       0,     0,     0,     0,     0,     0,     0,   379,   276,   148,
     127,   242,   199,   399,   968,   374,   356,   357,     0,    82,
       0,   221,     0,    87,     0,    84,     0,     0,     0,   735,
     976,     0,     0,   779,     0,     0,   785,     0,     0,   777,
     776,     0,   831,     0,   822,   821,     0,   800,     0,   829,
     847,   839,     0,     0,     0,   801,   825,     0,     0,   804,
       0,     0,     0,     0,     0,     0,     0,   802,     0,     0,
     803,     0,   833,     0,     0,   797,     0,     0,   798,     0,
       0,   799,     0,     0,   221,     0,   920,     0,     0,   935,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   958,
       0,     0,   956,     0,     0,   948,     0,     0,     0,     0,
       0,   881,   939,    65,    57,    58,    64,    59,    60,     0,
       0,     0,    22,    25,    41,    42,   525,   525,     0,   746,
     766,     0,     0,   729,   745,    50,    48,    45,     0,     0,
     698,   668,     0,   696,   691,   692,   694,     0,     0,   681,
       0,   705,   607,   608,   612,   276,   200,   374,     0,   181,
       0,     0,     0,   194,     0,   190,     0,     0,     0,   409,
     229,   224,   884,   225,   226,   227,   228,   281,     0,   275,
     277,     0,   222,   230,     0,   251,   628,   247,     0,   428,
     455,   456,   390,   391,   387,   386,   389,   388,   371,   479,
     372,   167,   972,     0,     0,   518,     0,   517,   553,     0,
     545,     0,   136,     0,   498,     0,   526,   527,   533,   531,
       0,   528,   369,     0,     0,     0,   119,   128,   205,   371,
       0,     0,   370,     0,    97,     0,   117,   116,    93,     0,
       0,    89,     0,    86,     0,   837,   838,   781,   780,   783,
     782,     0,     0,     0,   648,     0,     0,   840,     0,     0,
       0,     0,     0,     0,   832,   830,   826,     0,   834,     0,
       0,     0,     0,   835,     0,     0,     0,     0,     0,     0,
     926,   925,     0,     0,   922,   921,     0,   930,   929,   928,
     927,   924,   923,     0,   938,   954,   953,     0,     0,     0,
       0,     0,     0,     0,   883,    54,    55,     0,    56,    62,
      23,   750,     0,   751,   748,   753,   756,   755,     0,     0,
     727,     0,    46,   664,   679,   667,   697,   693,   695,     0,
       0,     0,     0,     0,   709,     0,   370,   629,   630,   644,
     148,   184,   180,     0,   196,   195,   191,     0,   372,   885,
     280,     0,   338,   372,   278,   372,   217,   231,   232,   371,
       0,   248,     0,     0,     0,     0,     0,     0,     0,     0,
     276,   476,   520,     0,   521,     0,   134,     0,   217,   137,
     138,   140,   141,   142,   501,   503,   499,   530,   276,   399,
     372,   196,     0,     0,     0,   354,     0,   358,   371,    88,
       0,     0,    90,   527,     0,   823,     0,     0,   968,   849,
     848,     0,     0,   827,     0,     0,     0,     0,   836,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   941,
     220,   931,   933,     0,     0,   955,   950,   949,     0,   944,
     945,   707,    40,   525,   707,   730,   673,   698,   682,   680,
     699,   701,   703,   704,   715,   639,   372,   355,   631,     0,
     175,   192,   372,   144,     0,     0,     0,     0,     0,     0,
       0,     0,   996,  1001,   290,   285,     0,   338,   284,   294,
     295,   296,   297,   298,   305,   306,   299,   301,   302,   304,
       0,   309,     0,     0,     0,     0,     0,     0,   974,   267,
       0,     0,   208,   233,     0,     0,     0,   251,   234,   237,
     758,     0,   372,   396,   397,   393,   392,   395,   394,     0,
     519,   546,   143,   131,   139,     0,   371,   264,   126,   372,
       0,   707,   373,   359,     0,     0,    91,    92,    79,     0,
     808,   816,     0,   809,   817,   812,   820,   824,   828,   810,
     818,   811,   819,   805,   813,   806,   814,   807,   815,   937,
     936,   951,   700,   747,   700,   675,   672,     0,     0,   711,
     713,   714,   708,     0,   265,   185,   410,   223,   312,     0,
       0,   315,   316,     0,   308,     0,   323,   307,   339,   340,
       0,   984,   286,     0,   291,   288,   344,     0,     0,     0,
     300,   293,   303,   292,   337,   211,   216,   235,   251,   758,
     238,   257,   250,   252,   400,   372,     0,   375,     0,   361,
     362,   364,   366,   365,   700,   372,    76,    94,     0,   705,
     705,     0,   671,   678,   702,   712,   715,     0,   283,     0,
     310,     0,   328,     0,     0,     0,   341,   352,     0,     0,
       0,     0,   347,   345,     0,     0,     0,   332,     0,   333,
       0,   239,   257,     0,   251,   253,   254,   266,   372,   698,
     364,     0,     0,   705,     0,   707,   709,   709,     0,   676,
     719,     0,   716,     0,   720,   282,   279,   311,   331,     0,
     314,     0,   322,   351,   350,   980,   349,   338,     0,     0,
       0,     0,   324,     0,     0,   251,     0,     0,   249,     0,
     256,     0,   255,   376,   368,   363,     0,   360,   118,   700,
     622,   622,     0,   674,   710,   715,     0,   722,     0,   718,
     721,     0,     0,     0,   338,   346,   348,     0,   343,     0,
     335,   334,   236,     0,     0,   260,   259,   258,   698,   705,
     851,   640,   677,   717,     0,   724,     0,   329,   313,   319,
     321,     0,     0,     0,   338,   263,   261,   262,     0,   709,
       0,   725,     0,   338,     0,     0,   338,   336,   367,   622,
     723,     0,   330,   320,   338,   342,   317,   850,   726,   318,
       0,     0,     0
};

static const short yydefgoto[] =
{
    1870,    20,    21,   461,   121,   122,   123,   124,  1202,   721,
     716,    22,   136,   966,   967,   968,   969,   125,   126,   466,
     467,    23,    81,   377,   871,   645,   646,   647,  1102,  1103,
     238,    24,    73,    25,   154,    26,   366,    74,  1314,   220,
     370,   637,  1090,  1306,   531,   208,   362,  1076,  1458,  1459,
    1460,  1461,  1462,  1463,   214,   230,   205,   582,   206,   358,
     359,   360,   589,   590,   591,   592,   593,   533,   534,   535,
    1021,  1018,  1019,  1239,  1421,   536,  1026,  1027,  1028,  1244,
    1064,   223,   312,   372,   859,   860,   861,   797,   798,   799,
    1582,   565,   557,  1362,   648,  1044,  1249,  1436,  1437,  1438,
    1588,   862,   567,   806,   807,   808,   809,  1267,   810,  1744,
    1745,  1746,  1826,   218,   310,   631,   193,   340,   562,   558,
     559,  1258,  1259,  1260,  1431,  1719,  1554,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1562,  1663,  1563,  1564,  1565,  1566,
    1567,  1725,  1568,   424,   425,  1812,  1738,  1569,  1570,  1571,
    1843,  1675,  1676,  1731,  1732,    27,   227,   314,  1095,  1096,
    1097,  1611,  1699,  1700,  1701,   864,  1475,   550,  1038,  1312,
     225,   633,   349,   578,   579,   823,   825,  1279,    28,    58,
     785,   782,   334,   335,   344,   345,   185,   186,   346,  1053,
     573,   815,   190,   162,   327,   328,   546,   198,   580,  1280,
      29,    95,   616,   617,   618,  1077,  1078,   619,   787,   621,
     622,  1070,  1391,   848,   623,   788,   625,   626,   627,   628,
     841,   629,   630,   838,    30,    31,    32,    33,    34,   520,
      35,   483,    36,    37,    60,   202,   351,    38,   298,   299,
     300,   301,   302,   303,   304,   305,   750,   306,  1006,  1234,
     505,   506,   755,   507,    39,  1572,    41,   161,   521,   522,
     234,  1419,  1016,   321,   768,   527,    42,   727,   486,   288,
    1238,  1538,   141,   142,   143,   286,   490,   491,   492,   741,
     738,   989,   990,   991,  1645,  1712,  1758,   992,   993,  1229,
     426,  1179,  1703,   148,  1227,  1231,  1530,  1531,  1414,   381,
    1535,  1760,  1653,  1761,  1762,  1763,  1809,  1850,  1573,  1212,
    1574,    45,    46,   382,  1575,    48,    49,   511,  1576,   493,
      50,   975,  1392,  1209,  1395,   258,   259,   458,   473,   474,
     723,   428,   429,   514,   660,   887,   661,   662,   663,   664,
    1126,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     889,  1129,  1130,  1335,   698,   674,   431,   432,   946,  1251,
     433,   434,   435,   436,   437,   438,   439,   651,    78,   236,
     222,  1032,  1079,   745,   440,   441,   442,   443,   444,   445,
     446,  1514,   447,   448,   449,   450,   451,   452,   937,   453,
     690,  1183,   691,  1180,   930,   287,   454,   455,   874,  1281,
     947,   211,   794,  1109,   242,   188,   239,  1774,   215,  1851,
     998,  1577,   994,   127,   456,   197,   199,   970,   700,   228,
     155,   457,   129
};

static const short yypact[] =
{
    2421,   955,    48,  1614,   538,   920,   222,  2027,   228,   671,
     491,   510,  5215,    48,   769,   427,  5445,   801,  5445,   263,
     276,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   106,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  5839,  5445,  5445,  5445,  5445,  5445,-32768,-32768,
     475,   807,  5445,  5445,  5445,   561,  5445,   483,  5445,  5445,
  -32768,  5445,  5445,-32768,   689,-32768,   527,-32768,-32768,   796,
    5445,-32768,  5445,  5445,  5445,   834,  5445,  5445,  5445,  5445,
     483,  5445,  5445,  5445,  5445,-32768,  5445,   494,-32768,-32768,
  -32768,   526,-32768,   526,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   654,   603,   779,-32768,   321,-32768,-32768,-32768,-32768,
    5445,  5445,  5445,   784,   818,   865,   503,   118,   809,   403,
     484,   778,   760,-32768,  5445,   935,   651,   841,-32768,  5445,
  -32768,  5445,  5445,  5445,-32768,-32768,-32768,  5445,-32768,  1011,
     225,   982,   607,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   183,-32768,-32768,   807,-32768,
  -32768,   496,-32768,-32768,   786,-32768,   815,-32768,-32768,   577,
  -32768,   902,-32768,-32768,   856,-32768,   701,-32768,-32768,  1056,
  -32768,-32768,-32768,   807,-32768,-32768,-32768,   775,-32768,   786,
  -32768,-32768,   463,-32768,  1007,-32768,  1012,-32768,   526,-32768,
  -32768,-32768,-32768,-32768,  1000,-32768,-32768,   898,-32768,  1017,
  -32768,   910,-32768,-32768,-32768,   898,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4687,-32768,  5445,-32768,-32768,
  -32768,  3514,   505,   968,  3547,  5445,  1023,   844,  1064,  1058,
    3514,   983,  1020,  1026,  3547,   894,-32768,  6007,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  4360,-32768,-32768,-32768,
     986,  5445,  1057,   969,   451,  5445,   991,-32768,-32768,   651,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  5445,-32768,-32768,
  -32768,   786,-32768,  1007,-32768,   526,  1046,  6007,  1014,-32768,
     993,   988,   142,   937,   112,   940,   776,   607,-32768,  1173,
     816,-32768,  5445,-32768,   183,-32768,-32768,-32768,-32768,  5445,
    1050,  1885,   996,  1989,   931,-32768,  5839,-32768,-32768,   694,
  -32768,-32768,-32768,   944,   944,   807,  1094,   807,   251,   701,
  -32768,-32768,  2929,-32768,  5445,  5445,-32768,  1050,-32768,-32768,
    1187,   387,   952,  5445,  1210,  5445,-32768,-32768,    38,   692,
    3706,-32768,-32768,-32768,   959,   960,   963,  1035,-32768,-32768,
     970,-32768,   973,-32768,   975,   976,-32768,   977,-32768,-32768,
  -32768,-32768,   984,   987,  1051,  1054,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   989,-32768,-32768,  4767,   990,   992,-32768,
    6007,  6007,  4457,  5445,-32768,-32768,   995,-32768,   997,-32768,
    1306,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   999,  1002,  1004,     0,-32768,
    5445,  1086,-32768,-32768,  5445,  5445,   437,-32768,-32768,-32768,
  -32768,  5445,  5445,    79,-32768,  5445,-32768,  1203,  5445,  3514,
    1126,  1013,-32768,  5445,  4997,   353,-32768,-32768,  6007,-32768,
    1220,  1015,-32768,   154,  1306,-32768,-32768,   529,-32768,   103,
  -32768,   489,-32768,   489,-32768,  1018,-32768,   124,-32768,  1115,
  -32768,   343,-32768,-32768,  1028,  1016,  1050,  1027,  1283,-32768,
  -32768,  1029,-32768,   302,-32768,  1177,  1059,-32768,-32768,   167,
    1217,  1221,-32768,-32768,-32768,-32768,   807,-32768,-32768,  1224,
  -32768,   944,   944,   944,   944,-32768,  1049,-32768,  1290,-32768,
    1298,-32768,-32768,  1277,-32768,  2929,-32768,  2929,   623,-32768,
  -32768,   786,  1314,  5445,-32768,  6337,-32768,   518,-32768,  5445,
     754,   815,  1144,  1122,  1188,  1157,-32768,-32768,-32768,   220,
    1198,-32768,  1138,   483,-32768,   807,-32768,  1329,  1259,-32768,
     251,-32768,-32768,-32768,-32768,-32768,   413,  1161,-32768,-32768,
  -32768,  1162,-32768,  1212,  1091,-32768,  1271,  1104,   203,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1103,-32768,-32768,-32768,   152,  1111,  1113,    34,-32768,  1104,
  -32768,-32768,   786,-32768,  1325,  1383,-32768,   807,   807,-32768,
    1885,   577,-32768,  1265,-32768,  1238,  1124,-32768,  2992,  4885,
    1262,-32768,  1275,  1131,  3803,  1132,  1024,  1170,  1410,  3454,
     335,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1380,   790,  5077,   140,-32768,  5445,
    6007,   837,   852,   893,  1673,-32768,-32768,  6007,  4033,  1306,
     206,  1202,  6007,  5077,   172,   172,   769,   762,  1133,   375,
  -32768,-32768,  5077,  6007,  5445,  6007,  6007,  6007,  6007,  6007,
    5307,  2152,  5445,-32768,-32768,  3198,  1226,-32768,-32768,  1199,
    3547,-32768,  1349,   765,-32768,   769,  1142,-32768,  1349,  3198,
    1353,  1354,  5445,-32768,   375,-32768,   445,  3931,  1223,  5077,
  -32768,  5445,-32768,   753,-32768,-32768,   188,-32768,  1230,-32768,
  -32768,-32768,  5445,   367,  5445,-32768,-32768,  5445,-32768,-32768,
    5077,  5445,  1415,  1885,-32768,  6007,  1179,  5445,-32768,-32768,
     807,   807,   807,-32768,   444,-32768,  1138,  1138,  1225,   483,
     776,-32768,-32768,  1155,-32768,-32768,-32768,-32768,  1407,-32768,
    5445,-32768,-32768,-32768,-32768,-32768,  1412,-32768,  1160,  1391,
  -32768,  1408,-32768,  1350,  1355,   526,-32768,-32768,-32768,-32768,
  -32768,  1419,-32768,-32768,-32768,-32768,-32768,  5445,  6438,   483,
    1307,  1315,  1316,-32768,   483,-32768,   884,-32768,-32768,  1282,
     807,-32768,   474,  1287,-32768,-32768,-32768,   483,-32768,  1091,
     546,-32768,-32768,-32768,  1405,   546,  1288,   546,-32768,   483,
     483,  1181,   483,-32768,  1050,  5445,-32768,  1358,-32768,   788,
  -32768,-32768,-32768,   694,  1293,  5445,   840,    69,-32768,-32768,
    1184,  1441,-32768,-32768,-32768,-32768,   692,  5445,  1302,  4130,
  -32768,-32768,  1302,  3803,-32768,-32768,-32768,   147,-32768,   456,
    1347,  4033,  4033,  6007,  6007,  1808,  1193,   731,  2087,  6007,
    2370,   500,  2635,  2733,  5387,  2996,  3094,  3357,  6007,  6007,
    1458,  6007,  1196,  6007,  1204,   499,  6007,  6007,  6007,  6007,
    6007,  6007,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1409,   985,   304,  5077,-32768,  4033,  6007,   466,   270,  1206,
     778,-32768,-32768,-32768,-32768,  1306,   250,-32768,-32768,  1454,
     172,   172,   799,   799,-32768,   849,-32768,-32768,-32768,-32768,
    5445,  5445,  5445,  5445,  5445,  5445,  1209,-32768,   452,-32768,
  -32768,  3198,  1351,-32768,   409,  1370,  5445,-32768,   778,  5077,
    1370,  1209,  1213,  3198,  2189,  1423,-32768,-32768,  2253,  1218,
     157,-32768,-32768,-32768,  1222,  4033,  1420,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    82,-32768,-32768,-32768,
  -32768,-32768,   862,  1293,-32768,-32768,-32768,  1228,-32768,-32768,
  -32768,   873,-32768,-32768,   944,  1481,-32768,   444,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1239,  4033,  2554,-32768,-32768,
    1452,-32768,-32768,  2554,-32768,-32768,-32768,-32768,   526,   526,
    1254,  1241,-32768,-32768,-32768,-32768,-32768,   257,   261,   877,
  -32768,  1493,-32768,-32768,-32768,  5445,-32768,   538,   880,  1343,
    1407,  1244,-32768,  1138,   886,-32768,-32768,   573,-32768,  1245,
    5445,   669,  1247,  1249,   483,  1250,  1503,   577,  1452,   944,
    1221,  1885,-32768,  1198,   778,  1319,  1339,-32768,  1627,-32768,
     483,  1387,  1473,-32768,  2992,-32768,   483,  1291,  1476,-32768,
  -32768,  1258,  1260,-32768,  4033,  4033,-32768,  4033,  4033,-32768,
    1525,   143,  1306,  1264,-32768,-32768,  1267,  1306,  5617,-32768,
  -32768,-32768,  1437,  1272,  1273,  1306,  1076,  1274,  1276,  1306,
    6007,  6007,  1193,  6007,  5697,  1278,  1280,  1306,  1281,  1284,
    1306,  6007,  1306,  1285,  1292,  1306,  1295,  1297,  1306,  1305,
    1308,  1306,   624,   670,-32768,   826,-32768,   858,  6007,-32768,
     874,   905,   917,   947,   961,   971,  6007,  6007,  5077,-32768,
    1496,   307,  1306,  1375,  5077,-32768,  6007,  6007,  5077,  6007,
    6007,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  3198,
    1487,  3198,-32768,  1209,   452,-32768,   544,   544,   359,-32768,
  -32768,  4360,   889,-32768,-32768,  1209,  1213,  1299,  2189,    35,
     899,  1310,  3931,  1432,-32768,  1433,  1446,  1497,  5077,   625,
    1565,  1510,-32768,-32768,-32768,  1452,-32768,  1319,   737,  1421,
     807,  1456,  1138,  1451,  1138,-32768,  4033,   148,   767,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1398,  1576,  1452,
  -32768,  6007,-32768,   480,  1442,  1254,   689,-32768,  4033,-32768,
  -32768,-32768,  1462,  1466,  1467,  1470,  1471,  1472,-32768,-32768,
  -32768,-32768,-32768,  1331,   538,-32768,  1225,-32768,-32768,   546,
  -32768,  2554,   175,   546,-32768,   546,-32768,  1407,-32768,-32768,
    1332,-32768,-32768,   694,  1576,  1138,-32768,  1221,-32768,-32768,
    4360,  1583,-32768,   226,-32768,  1336,-32768,  1228,-32768,  1344,
     807,   174,  1345,-32768,   807,-32768,-32768,-32768,  1525,-32768,
    1525,  6007,  1459,  1459,   769,  1352,   895,-32768,  1459,  1459,
    6007,  1459,  1459,   150,  1306,-32768,  1080,  6007,  1306,  1459,
    1459,  1459,  1459,  1306,  1459,  1459,  1459,  1459,  1459,  1459,
  -32768,-32768,  1356,  2929,-32768,-32768,  1009,-32768,-32768,-32768,
  -32768,-32768,-32768,  1039,  1240,-32768,-32768,  5077,  5077,  1569,
    1443,  1066,   908,  1093,  1306,-32768,-32768,  1494,-32768,-32768,
  -32768,-32768,  1445,-32768,-32768,-32768,-32768,-32768,  1220,  5077,
  -32768,  2189,  1299,  1619,-32768,   157,-32768,-32768,-32768,  3931,
     922,  5445,  6007,  4033,  1489,  1576,-32768,-32768,-32768,-32768,
     944,-32768,-32768,   807,   884,-32768,-32768,   149,-32768,-32768,
  -32768,  5445,  6229,-32768,-32768,  1306,  1606,   480,-32768,   691,
    5445,-32768,  1550,   151,  1447,  1553,  1450,  1587,  1556,  1594,
    1452,-32768,-32768,  1369,-32768,  1378,-32768,  1555,  1606,   175,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1452,  1198,
  -32768,   884,  1493,  1589,  1527,-32768,  1293,-32768,-32768,-32768,
    1539,   100,-32768,  1407,  1542,  1306,  1386,  1388,   778,-32768,
  -32768,  1389,  1390,  1306,  1392,  1393,  6007,  6007,  1306,  1394,
    1395,  1397,  1400,  1402,  1403,  1404,  1406,  1413,  1416,-32768,
  -32768,-32768,-32768,  6007,  1417,-32768,-32768,-32768,  5077,-32768,
  -32768,  1223,-32768,   544,  1223,-32768,  5445,   883,-32768,-32768,
    1411,-32768,  1306,   335,   434,-32768,-32768,-32768,-32768,  1138,
  -32768,-32768,-32768,-32768,  2929,  4289,    49,  1422,  1424,  1575,
    6007,  1425,  1427,  4825,-32768,-32768,  1632,  4595,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     141,-32768,  1608,  1428,  1434,  1438,  1439,  1429,   715,-32768,
    1418,  5445,-32768,-32768,  1610,  1621,  5445,  1254,-32768,-32768,
     526,  5445,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1576,
  -32768,-32768,-32768,-32768,-32768,  1576,-32768,-32768,-32768,-32768,
    4258,  1223,-32768,-32768,  6007,   807,-32768,-32768,-32768,   807,
  -32768,-32768,  6007,-32768,-32768,-32768,-32768,  1306,  1306,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1306,
  -32768,-32768,  1420,-32768,  1420,  1440,-32768,  4033,  6007,-32768,
  -32768,  1600,-32768,  1444,-32768,-32768,-32768,   134,-32768,  5927,
    5445,-32768,-32768,  6007,-32768,  4033,  1306,-32768,-32768,-32768,
    1453,-32768,-32768,   195,-32768,  1519,-32768,   159,  1448,  4188,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1254,   526,
  -32768,  1596,-32768,-32768,-32768,-32768,  1493,-32768,  3262,  1460,
     157,-32768,-32768,-32768,  1420,  1306,-32768,-32768,  1093,  1510,
    1510,  5445,-32768,   335,-32768,-32768,  2475,  2554,-32768,  1455,
  -32768,   810,  4687,   643,   153,  1457,-32768,-32768,  5445,  5445,
     538,   120,-32768,-32768,   443,  1644,  4033,-32768,   756,-32768,
    1461,-32768,  1596,   273,  1254,  1602,  1604,-32768,-32768,   899,
    1463,  4258,  1649,  1510,  1469,  1223,  1489,  1489,   923,-32768,
  -32768,  1479,  1468,   733,  5445,-32768,-32768,-32768,  1601,  4188,
  -32768,  1578,-32768,-32768,-32768,-32768,-32768,  6262,   195,  6007,
    4188,   155,-32768,  4188,  5135,  1254,    99,    99,-32768,  1709,
  -32768,  1572,-32768,-32768,-32768,   157,  4258,-32768,-32768,  1420,
    1628,  1628,  5445,-32768,-32768,  2475,  1490,-32768,  5445,-32768,
  -32768,  3863,  1495,   782,  6262,-32768,-32768,  1294,   158,  1714,
  -32768,-32768,-32768,  1558,   191,-32768,-32768,-32768,   896,  1510,
  -32768,-32768,-32768,-32768,  5445,-32768,  4188,  1498,-32768,-32768,
    1712,  4188,  1730,  1721,  6262,-32768,-32768,-32768,  4033,  1489,
    1500,  1504,   926,  6262,   121,  5445,  6262,-32768,   335,  1628,
  -32768,  5445,-32768,-32768,  6262,-32768,-32768,-32768,-32768,-32768,
    1772,  1777,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -79,   460,-32768,   461,  1516,   576,-32768,
    -295,-32768,-32768,  -536,  -126,  -632,  -999,  1770,  1518,  1511,
    1068,-32768,-32768,-32768,   680,-32768,  1145,   924,-32768,   690,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   700,-32768,-32768,-32768,-32768,-32768,
     338,-32768,-32768,  1475,-32768,-32768,-32768,  -330,-32768,-32768,
  -32768,  1449,-32768,-32768,  1216,  -213,  1478,-32768,-32768,  -339,
  -32768,  -734,-32768,-32768,-32768,-32768,-32768,-32768,   783,-32768,
    -707,-32768,-32768,  1505,  1048,   722,  1483,  1019,   718,-32768,
     361,   -49,-32768,-32768,  -310,   165,  -973,-32768,-32768,   389,
  -32768,  1486, -1078,-32768,-32768,-32768,-32768, -1171,   390,    89,
      86,    91,    47,-32768,-32768,-32768,-32768,  -101,  -272,-32768,
    1047,  -960,-32768,   579,-32768,-32768, -1410, -1098,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   105,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1151,   119,-32768,  -618,-32768,-32768,-32768,
  -32768,-32768,   170,-32768,    62,-32768,-32768,-32768,   829,-32768,
   -1127,-32768,-32768, -1196,   156,  -648,   432,  -289,  -944,   614,
  -32768,-32768,  -534,  -744,-32768,-32768,  -964, -1299,-32768,-32768,
  -32768,-32768,-32768,  1522,-32768,  1286,  1507,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1531,-32768,-32768,-32768,-32768,
  -32768,-32768,  -515,  -525,-32768,  1021,   566,  -546,  -346,-32768,
  -32768,-32768,  -267,  -940,-32768,  -340,-32768,-32768,-32768,-32768,
    1231,-32768,-32768,  1022,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1851,-32768,-32768,-32768,-32768,-32768,
    1567,-32768,-32768,-32768,  1368,-32768,  1366,-32768,-32768,-32768,
  -32768,  1118,-32768,-32768,-32768,     2,  -920, -1121,-32768,  1106,
    -471,-32768,-32768,-32768,-32768,-32768,    30,   125,  -174,   -31,
  -32768,-32768,  -627,-32768,  1737,  -837, -1004,-32768,  1147, -1033,
   -1245,-32768,  -864, -1230,-32768,-32768,-32768,   900,-32768,-32768,
    -395,  -243,   213,   -27, -1087, -1414,-32768,   239, -1125,  -491,
   -1274,   365,-32768,    96,-32768,   138,-32768,    42,  1904,-32768,
    1905,-32768,-32768,  1396,  1906,-32768,-32768,-32768,  -192,  -610,
  -32768,  1182, -1070,   932,-32768,   -92,  -666,  -633,-32768,-32768,
  -32768,  -453,   -41,  -371,  -621,-32768,  -492,-32768,-32768,-32768,
     469,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -489,
  -32768,   771,-32768,   644,-32768,  1042,  -947,-32768,  -593,-32768,
     663,  -938,  -122,  -929,  -917,  -910,   -36,    -3,   -39,-32768,
     -32,  -722,  -463,  -104,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   728,-32768, -1006,-32768,   366,  -281,   545,-32768,   835,
     336,  -697,  1357,    61,-32768,   -53,  1833,-32768,     5,   -50,
   -1224,   368,   -40,   -33,  -141,   -10,    -4,  -188,-32768,   -60,
     -26,    -9,-32768
};


#define	YYLAST		6701


static const short yytable[] =
{
     128,    77,    40,   128,   191,   495,   149,   150,   308,   156,
     213,   260,   427,   209,   958,   194,   620,   279,   282,   594,
     758,   210,   624,   555,   583,   207,   219,   697,   221,   196,
     786,   244,   789,   253,   744,   280,   283,  1022,   248,   231,
     801,   224,   553,   187,   189,   192,   195,   150,   200,   249,
     800,   221,   766,   212,   189,   216,   255,   195,   250,   150,
     200,   254,   229,   128,  1010,   226,   910,   932,  1219,   940,
    1262,   240,   149,   150,   243,   189,   468,   240,   216,   192,
     195,   251,   150,   200,   229,   128,   468,   195,   252,   266,
    1250,   268,   247,   315,  1441,   635,  1250,   982,   978,  1252,
    1263,   724,   872,  1189,   267,  1252,   487,   863,  1253,  1269,
    1068,   311,   650,   545,  1253,   512,  1013,   955,   367,  1093,
    1254,   212,   150,   212,  1220,   313,  1254,  1255,  1304,  1309,
    1287,   316,  1292,  1255,  1017,   216,   374,  1394,  1521,  1050,
     309,  1211,   195,   150,   229,  1040,   284,  1674,   156,   290,
    1009,  1331,   336,  1524,  1047,  1114,   891,   891,  1496,   891,
    1433,   891,   880,   891,   740,   881,   515,   888,  1842,   704,
    1777,  1864,  1375,  1609,   705,   846,   704,   363,  1379,   427,
    1043,   705,   770,   730,   274,  1439,  1477,   329,   753,   911,
     319,   477,   773,   981,  1481,   -80,  1411,  1660,   704,  -371,
    1386,  1616,  1389,   705,  1727,  1677,  1470,  1398,  1088,  1100,
     516,   776,   777,   778,   779,   538,   459,   501,  1734,   620,
    -661,   160,  1823,   518,  1223,   624,   842,   851,  1709,   147,
    1710,   318,  1476,  1232,   462,   330,   528,  1846,  1224,   159,
    1661,   498,   725,   462,   762,   529,  -698,   996,   212,    59,
     488,   594,   150,  1728,   746,   469,   128,  1310,   496,   933,
     934,   150,  1225,  1824,  1729,   469,   513,   820,   508,   726,
    1119,  1120,   712,   296,   713,  1415,   541,   718,   811,  1115,
     892,   892,   192,   892,  1233,   892,   150,   892,    82,  1847,
    1753,   554,   160,  1054,   843,   241,   704,   587,   560,  1457,
     210,   705,   570,  1662,   821,   574,  1473,   852,  1274,  1403,
    1825,   644,   891,   232,  1181,   891,  1226,  1536,  1456,   584,
    1786,   586,    14,   212,   632,   643,   542,   844,   704,   833,
     212,   515,   212,   705,   212,   639,  1451,   575,  1617,  1204,
    1678,  1272,  1101,   891,  1250,   528,   641,   331,   524,  1613,
     233,  1216,  1217,  1252,   529,   195,   200,  1466,  1405,  1439,
     543,   634,  1253,  1235,   150,   332,   192,  1815,  1730,  1213,
    1526,  1515,  1516,   503,  1254,   539,    96,  1074,   544,   704,
    1702,  1255,  1264,  1265,   705,  1829,  1482,   880,    14,  1094,
     265,   880,  1778,  1783,  1113,   495,   754,  1748,  1717,   333,
     731,   704,  1389,   935,  1840,   530,   705,  1271,   822,   912,
     706,   707,   708,   709,   701,  1247,  1690,   706,   707,   708,
     709,  1116,  1428,  1542,   717,  1592,  -661,  1771,   847,  1819,
    1783,   513,  1316,   714,  1857,  1203,   892,   985,   717,   892,
     771,   708,   709,  1863,   722,  1273,  1866,  1215,   728,  1275,
     939,   150,   462,  1643,  1869,   195,   469,   733,   495,   944,
     792,  1787,   150,  1755,  1117,  1317,   150,   892,  1702,   195,
     150,   704,   134,   135,   156,  1206,   705,   368,   826,   495,
     501,   638,  1800,  1801,  1543,  1178,  1397,  1759,  1377,  1579,
    1599,  1580,  1035,  1327,  1328,  1037,  1329,  1330,   144,   337,
     774,  1694,  1749,   264,  -242,  1606,  1422,  1695,  1605,    97,
     130,  1045,  1641,   563,   588,   157,  1004,  1741,  1140,  1184,
    1185,  1702,  1190,  1649,   802,   704,  1607,  1191,  1737,   132,
     705,   854,   468,  1005,  1141,  1336,  1061,   706,   707,   708,
     709,   380,  1187,  1618,   221,  1527,  1024,   158,  1650,   828,
      98,   827,    98,  1303,   795,  1795,   212,  1104,   872,  1469,
     795,   986,    99,  1031,  1454,  1008,  1702,  1034,   217,   706,
     707,   708,   709,  1788,   369,  1859,   338,   131,  1832,  1142,
     347,   502,  1086,   803,  1756,  1757,  1402,   100,  1118,   100,
     649,  1207,  1654,   265,   203,   204,   272,   273,  1656,   210,
    1828,   857,   858,   145,  -242,  1210,  1651,   276,  1143,  1108,
     146,   322,   748,  1752,  1822,   757,   515,    75,  1737,  1025,
     706,   707,   708,   709,   323,  1427,  -242,   735,  1797,  1737,
     201,   212,  1820,  -242,  1661,   740,   503,   719,   278,  1001,
     875,   256,   706,   707,   708,   709,   873,  1443,  1693,   943,
     704,  1622,  1200,   504,  1468,   705,   101,   348,   101,   324,
    1737,   325,  1752,  1186,   804,  1697,   102,  -242,   102,   704,
     216,   959,  1029,  1030,   705,  1144,   277,    97,  1031,  1830,
    1831,    76,   278,   749,   914,  1737,   832,    75,   276,  1540,
    1737,   103,   717,   103,  1242,   948,   704,  1662,   495,   576,
     232,   705,   212,   212,  1849,   805,   469,  1488,  1752,   720,
     577,   469,   706,   707,   708,   709,   513,  1541,    98,   987,
     469,   649,   210,   195,  1201,   508,   459,  1007,   195,  1769,
      99,   235,   999,   276,  1020,  1020,  1023,   233,  1867,   292,
     221,  1752,    75,   150,  1765,   150,  1067,  1033,    75,   560,
     276,  1747,   212,  1261,   212,   100,  1256,   281,   212,  1305,
     742,  1754,  1256,   278,  1608,  1270,   706,   707,   708,   709,
    1250,   326,  1193,  1169,   293,  1196,  1052,  1055,  1219,  1252,
     221,   212,   378,   261,   284,  1060,  1291,  1056,  1253,  1525,
     383,  1104,  1533,  1382,  1066,  1442,   284,   743,  1071,   257,
    1254,   541,   294,   353,  1793,   295,    76,  1255,   212,   212,
     221,   221,    76,   221,  1073,  1584,  1806,  1082,  1083,   341,
    1085,   296,   342,   278,   101,   704,  1455,   285,  1087,  1131,
     705,  -664,  1465,  1410,   102,   515,   704,  1585,  1098,   908,
     354,   705,   355,   284,  1586,  1293,   150,   297,  1510,   551,
    1294,   542,   704,  1807,  1363,  1132,   150,   705,   284,   103,
    -664,  -664,   552,  -664,   237,   356,  1808,   343,  1110,   649,
    -664,  -664,  -664,  -664,  -664,   262,  -664,   104,  1587,  -664,
    -664,  -664,  -664,  -664,   704,   543,   916,  -664,  -664,   705,
     357,   706,   707,   708,   709,   790,    75,   791,  1360,   284,
     704,   918,   245,   544,  1197,   705,   105,   106,   263,   107,
     706,   707,   708,   709,  1770,   269,   108,   109,   110,   111,
     112,  1194,   113,   364,  1276,   114,   115,   116,   117,   118,
     495,   704,  1198,   119,   120,   513,   705,   706,   707,   708,
     709,  1293,   920,   704,  1361,   427,  1297,   270,   705,   151,
    1223,   469,   195,   200,   469,   229,   128,   276,  1195,   365,
      76,  1277,   469,  1223,  1224,   813,  1223,   814,   773,  1001,
     399,   400,   401,   704,   469,   469,  1417,  1224,   705,   195,
    1224,   152,    79,    80,  1000,   427,  1418,   704,  1225,  1450,
    -984,   275,   705,  -987,   271,    51,   153,   704,   409,   410,
     140,  1225,   705,  1111,  1225,   307,    52,  1112,   139,   140,
    1256,   704,  1647,    53,   203,   204,   705,   620,   291,  1657,
    1472,  1062,  1063,   624,  -841,  1848,  1713,  1782,  1783,   495,
    1642,   317,  -841,  1644,   702,   704,   941,   976,    54,   977,
     705,   909,  1226,   913,  1724,   221,   320,   917,   919,   921,
     210,  1177,  1300,  1839,  1783,  1226,  1282,   459,  1226,   339,
    1091,   221,  1092,   352,  1283,   704,   361,   221,  1318,   371,
     705,  1282,   350,  1385,  1322,  1388,   373,   706,   707,   708,
     709,  1767,   212,   375,  1393,  1393,  1396,   958,  -841,   212,
    1539,  -841,   704,   706,   707,   708,   709,   705,  -841,   379,
    1364,  -841,   704,    55,   376,  -841,   704,   705,   380,   471,
    -841,   705,   868,  -841,  1099,  1781,   464,   472,   495,   704,
    1704,  1190,   476,  1192,   705,   706,   707,   708,   709,  -841,
     475,   478,  1365,  1340,  1091,    56,  1236,  1497,  1424,    57,
    1426,   706,   707,   708,   709,  1240,   482,  1241,  1367,   479,
     811,  1813,  1284,  -841,  1285,   480,  -841,  -841,  1289,   737,
    1290,  1399,  1818,  1400,   499,   500,  -841,  1190,   497,  1490,
    -842,   509,   706,   707,   708,   709,   519,    14,  -842,  1368,
     702,   525,  1520,  -841,   706,   707,   708,   709,   526,  1614,
     469,  1369,   469,  1837,   702,  1802,  1528,  1803,  1783,   537,
    1862,  1471,   540,   571,  1020,  -841,   548,   561,   581,   469,
    -841,   568,   585,   195,   706,   707,   708,   709,  1852,   636,
     642,  1370,  -841,  1854,  -841,   640,  1739,  1858,   706,   707,
     708,   709,   675,   676,  -842,  1371,   677,  -842,   706,   707,
     708,   709,   678,   679,  -842,  1372,   680,  -842,   681,   682,
     683,  -842,   706,   707,   708,   709,  -842,  -970,   685,  -842,
     684,   686,   687,   692,  1799,   693,   704,   702,   715,   729,
    1529,   705,   710,   703,   732,  -842,   706,   707,   708,   709,
     711,  1453,  -987,  1511,  1480,   720,   737,   739,  1484,   756,
     752,   515,   760,   764,   761,  -841,  -841,   430,  -841,  -842,
     763,   765,  -842,  -842,  1513,   767,   706,   707,   708,   709,
     769,   772,  -842,  1512,   781,   528,  1739,  1696,   775,   485,
     704,   780,   783,   784,   793,   705,   817,  1739,   494,  -842,
    1821,   819,   704,   706,   707,   708,   709,   705,   818,  -430,
    1519,   824,   276,   706,   707,   708,   709,   706,   707,   708,
     709,  -842,   829,   830,   834,   835,  -842,   836,  1739,   523,
     706,   707,   708,   709,   837,   893,   839,  1134,  -842,  1138,
    -842,  1146,  1149,   704,  1154,  1157,  1160,   840,   705,   845,
    1841,   894,  1544,  1739,   849,  1646,   850,  1020,  1739,   855,
     210,   513,   469,   856,   865,   867,   868,   876,   893,   936,
     195,  1393,   309,   877,   878,   882,   704,   942,   971,   972,
    -843,   705,   974,  1590,   894,   979,   515,   895,  -843,   983,
     984,   995,   212,  1578,  1002,  1011,   896,  1015,  1036,  1031,
     846,   150,  1041,  1042,   897,  1655,  1256,  1043,   796,  1057,
    1048,  -842,  -842,  1051,  -842,  1049,  1065,  1058,  1059,  1069,
     895,  1075,  1080,   898,  1084,   899,  1094,  1106,   689,   896,
    1089,  1107,   694,   695,   699,   725,  1128,   897,  1164,   900,
    1166,   901,   902,   903,  -843,  1176,  1168,  -843,  1188,   427,
     704,  1199,  1205,  1208,  -843,  1201,   898,  -843,   899,  1218,
    1222,  -843,  1659,  1243,  1230,  1228,  -843,  1257,  1691,  -843,
     712,  1266,   900,  1278,   901,   902,   903,   706,   707,   708,
     709,  1286,  1246,  1302,  1268,  -843,   513,   309,  1288,  1311,
    1295,  1298,   904,  1299,  1301,  1313,   734,  1100,  1320,  1323,
     736,  1324,  1325,   891,  1326,  1337,   189,  1332,  1740,  -843,
    1333,  1692,  -843,  -843,  1671,  1338,  1339,  1341,  1578,  1342,
    1376,  1349,  -843,  1350,  1351,   904,  1378,  1352,  1354,  1689,
    1387,   706,   707,   708,   709,  1355,  1406,  1407,  1356,  -843,
    1357,  1401,   948,   706,   707,   708,   709,   150,  1358,  1706,
    1408,  1359,   192,  1707,  1404,  1412,  1409,  1413,  1425,  1420,
    1430,  -843,  1423,  1432,  1444,  1440,  -843,  1742,  1445,  1446,
     149,   150,  1447,  1448,  1449,  1452,  1467,  1474,  -843,  1478,
    -843,   905,   906,   907,   706,   707,   708,   709,  1479,  1483,
    1722,   943,  1334,  1517,  1518,  1522,  1489,  1523,  1740,   740,
    1509,  1534,  1581,  1591,  1596,  1593,   515,  1594,  1595,  1740,
    1597,  1598,  1740,  1600,   905,   906,   907,   706,   707,   708,
     709,   195,  1601,  1602,    61,  1610,  1615,   796,  1612,  1619,
    1620,   131,  1621,  1623,  1624,    62,  1625,  1626,  1629,  1630,
    1740,  1631,    63,   515,  1632,  1773,  1633,  1634,  1635,  1735,
    1636,  -843,  -843,  1648,  -843,    64,  1672,  1637,   149,   150,
    1638,  1640,  1685,  1664,  1679,  1740,  1667,  1665,  1668,  1680,
    1740,   890,   999,   515,  1684,  1681,  1764,   150,  1687,  1682,
    1683,  1688,   515,  1711,  1715,   515,  1673,  1716,   494,   189,
    1775,  1736,   915,   515,  1726,  1743,  1766,  1776,  1772,   931,
    1780,  1789,  1751,  1791,   938,   430,   513,  1794,  1796,  1784,
    1805,   149,   150,  1798,   430,   945,    65,   949,   950,   951,
     952,   953,   945,  1804,  1764,   150,  1786,  1811,  1835,  1814,
    1787,   159,    66,  1834,  1844,  1853,  1838,  1845,  1578,  1855,
    1783,  1856,  1871,   513,  1860,   212,  1861,  1872,   463,    67,
    1390,   494,   137,   470,  1321,   481,   149,   150,   973,   866,
    1307,  1319,  1105,   999,    68,  1764,   150,  1604,    69,   192,
     532,    70,   494,   513,   549,  1578,   831,   523,   595,    71,
    1245,  1012,   513,  1308,  1123,   513,  1315,  1124,   517,  1603,
    1046,   384,  1718,   513,   564,   192,  1583,   566,    72,  1589,
     385,  1785,  1792,   104,  1827,  1578,  1790,  1039,  1434,  1779,
    1816,  1768,  1237,   386,  1578,  1733,  1110,  1578,  1537,   387,
     388,  1416,   192,   572,  1750,  1578,   556,   816,   547,  1464,
     853,  1072,   105,   106,   138,   107,   510,   747,  1081,   751,
    1003,  1014,   108,   109,   110,   111,   112,   389,   113,   289,
     390,   114,   115,   116,   117,   118,   997,  1714,  1221,   119,
     120,   922,   923,   924,   925,   926,   927,   928,   929,  1652,
     257,  1833,  1810,  1868,    43,    44,    47,   759,   392,   393,
     980,  1429,  1214,  1345,  1380,  1296,  1865,  1686,   563,   246,
     394,  1670,     0,     0,   395,     0,   812,     0,     0,     0,
       0,     0,     0,     0,     0,  1121,  1122,  1127,     0,     0,
    1135,  1136,  1139,     0,  1147,  1150,  1152,  1155,  1158,  1161,
    1162,  1163,     0,  1165,     0,  1167,     0,     0,  1170,  1171,
    1172,  1173,  1174,  1175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,  1486,  1487,  1182,  1125,
       0,   396,  1491,  1492,     0,  1494,  1495,   397,     0,     0,
       0,     0,     0,  1499,  1500,  1501,  1502,   398,  1503,  1504,
    1505,  1506,  1507,  1508,     0,     0,     0,     0,     0,     0,
       0,   399,   400,   401,   402,   203,   204,     0,     0,     0,
       0,   494,   569,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   105,   413,     0,   107,     0,     0,   414,
     415,     0,   416,   417,   418,   110,   111,   112,   419,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,     0,   420,   421,     0,    83,     0,
       0,   484,     0,   423,     0,    84,     0,   278,     0,    85,
      86,   104,     0,  1133,     0,     0,  1124,     0,    87,     0,
     384,     0,     0,     0,     0,     0,     0,     0,     0,   385,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,   386,   107,     0,     0,     0,     0,   387,   388,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     945,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,  1343,  1344,     0,  1346,  1348,   392,   393,     0,
       0,     0,    90,  1353,   956,   104,     0,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,    91,     0,     0,
    1366,    92,     0,     0,     0,     0,     0,     0,  1373,  1374,
     430,     0,    93,     0,   105,   106,   430,   107,  1182,  1381,
     430,  1383,  1384,     0,   108,   109,   110,   111,   112,     0,
     113,    94,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,   494,     0,     0,     0,     0,  1125,     0,
     396,     0,     0,   960,     0,     0,   397,     0,     0,     0,
     430,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,  1435,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   494,     0,   420,   421,     0,     0,   104,     0,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,  1485,     0,     0,  1137,     0,   963,  1124,
       0,     0,  1493,   384,     0,     0,     0,   105,   106,  1498,
     107,     0,   385,     0,     0,   104,     0,   108,   109,   110,
     111,   112,     0,   113,     0,   386,   114,   115,   116,   117,
     118,   387,   388,     0,   119,   120,    14,     0,     0,   430,
     430,   957,     0,     0,   105,   106,     0,   107,     1,     0,
       0,     0,     0,     0,   108,   109,   110,   111,   112,   389,
     113,   494,   390,   114,   115,   116,   117,   118,     0,     2,
       0,   119,   120,     0,  1532,     0,     0,     3,     0,   104,
       0,     0,     0,     0,     4,     0,     5,     0,     6,     0,
     392,   393,     0,     7,     0,     0,     0,     0,     0,     0,
       8,     0,   394,     0,     0,     0,   395,     0,   105,   106,
       0,   107,     0,     0,     9,     0,     0,     0,   108,   109,
     110,   111,   112,     0,   113,    10,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   988,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1627,  1628,
       0,  1125,     0,   396,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,  1639,     0,     0,     0,   398,
     430,     0,     0,     0,  1649,     0,     0,     0,     0,     0,
       0,     0,    11,   399,   400,   401,   402,   203,   204,    12,
       0,     0,    13,     0,    14,    15,     0,   403,     0,  1650,
       0,     0,  1666,     0,     0,   387,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,    16,
       0,   414,   415,     0,   416,   417,   418,   110,   111,   112,
     419,   113,     0,   389,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,     0,     0,     0,   420,   421,     0,
       0,  1145,     0,   484,  1124,   423,     0,  1651,   384,   278,
       0,     0,   391,     0,     0,     0,  1705,   385,    17,     0,
       0,     0,     0,     0,  1708,     0,     0,     0,     0,     0,
     386,     0,     0,    18,    19,     0,   387,   388,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
    1532,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1721,     0,     0,   389,  1723,     0,   390,     0,     0,
     105,   106,     0,   107,     0,     0,     0,     0,     0,     0,
     108,   109,   110,   111,   112,     0,   113,   396,     0,   114,
     115,   116,   117,   118,     0,   392,   393,   119,   120,  1148,
       0,     0,  1124,   398,     0,     0,   384,   394,     0,     0,
       0,   395,     0,     0,     0,   385,     0,   399,   400,   401,
       0,   203,   204,     0,   430,     0,     0,     0,   386,     0,
       0,     0,     0,     0,   387,   388,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,     0,
       0,     0,     0,     0,     0,   414,   415,     0,     0,     0,
       0,     0,   389,     0,   419,   390,  1125,     0,   396,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,  1817,  1248,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,   399,   400,
     401,   402,   203,   204,     0,   394,     0,     0,     0,   395,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,     0,   414,   415,     0,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
       0,     0,   420,   421,  1125,     0,   396,     0,   484,     0,
     423,     0,   397,     0,   278,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,   596,     0,
     403,     0,   597,     0,     0,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
     598,   107,     0,   599,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
     420,   421,  1153,     0,     0,  1124,   484,     0,   423,   384,
       0,   869,   278,     0,   600,   597,     0,     0,   385,     0,
     601,   602,   603,     0,   604,   605,   606,     0,   607,   870,
       0,   386,     0,   598,     0,     0,   599,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   608,     0,
     609,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,   600,     0,     0,
       0,   610,     0,   601,   602,   603,     0,   604,   605,   606,
       0,   607,     0,     0,     0,     0,   392,   393,   611,     0,
    1156,     0,     0,  1124,     0,     0,     0,   384,   394,     0,
       0,   608,   395,   609,     0,     0,   385,     0,     0,     0,
     612,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,     0,     0,   387,   388,     0,     0,     0,
       0,     0,     0,     0,   610,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   613,   614,     0,     0,     0,
       0,   611,     0,   389,     0,     0,   390,  1125,     0,   396,
       0,     0,   615,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,   612,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,   394,     0,     0,     0,
     395,     0,     0,   403,     0,     0,     0,     0,   613,   614,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,   615,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,   420,   421,  1125,     0,   396,     0,   484,
       0,   423,   960,   397,     0,   278,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
     402,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   105,
     413,     0,   107,     0,     0,   414,   415,     0,   416,   417,
     418,   110,   111,   112,   419,   113,   961,     0,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,     0,
       0,   420,   421,  1159,     0,     0,  1124,   484,     0,   423,
     384,     0,     0,   278,     0,     0,     0,     0,     0,   385,
       0,     0,   962,     0,     0,     0,     0,   963,     0,     0,
       0,     0,   386,   964,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,   965,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,   389,     0,     0,   390,
       0,     0,     0,   105,   106,     0,   107,     0,     0,     0,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,   392,   393,     0,
     119,   120,     0,     0,     0,     0,     0,   384,   104,   394,
       0,     0,     0,   395,     0,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,     0,     0,   387,   388,   105,   106,     0,
     107,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   112,     0,   113,   653,     0,   114,   115,   116,   117,
     118,     0,     0,   389,   119,   120,   390,     0,  1125,     0,
     396,     0,     0,     0,     0,  1698,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,   392,   393,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,   394,     0,     0,     0,
     395,     0,     0,     0,   403,     0,     0,     0,   883,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,   696,     0,   119,
     120,   655,     0,     0,   420,   421,     0,   396,     0,     0,
     484,     0,   423,   397,     0,     0,   278,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
     402,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   105,
     413,     0,   107,     0,   460,   414,   415,     0,   416,   417,
     418,   110,   111,   112,   419,   113,     0,     0,   114,   115,
     116,   117,   118,   884,   885,   886,   119,   120,     0,   384,
     104,   420,   421,     0,     0,     0,     0,   659,   385,   423,
       0,     0,     0,   278,     0,     0,   465,     0,     0,     0,
       0,   386,     0,     0,   652,     0,     0,   387,   388,   105,
     106,     0,   107,   104,     0,     0,     0,     0,     0,   108,
     109,   110,   111,   112,     0,   113,   653,     0,   114,   115,
     116,   117,   118,     0,     0,   389,   119,   120,   390,     0,
       0,     0,   105,   106,     0,   107,     0,     0,     0,     0,
       0,     0,   108,   109,   110,   111,   112,     0,   113,     0,
       0,   114,   115,   116,   117,   118,   392,   393,     0,   119,
     120,     0,     0,     0,     0,     0,   384,     0,   394,     0,
       0,     0,   395,     0,     0,   385,     0,     0,     0,     0,
     654,     0,     0,     0,     0,     0,     0,     0,   386,     0,
       0,     0,     0,     0,   387,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   653,     0,     0,     0,     0,     0,     0,
       0,     0,   389,   655,     0,   390,     0,     0,     0,   396,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,   394,     0,     0,     0,   395,
       0,     0,     0,   403,     0,     0,     0,   879,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   656,   657,   658,     0,     0,     0,   119,   120,
     655,     0,     0,   420,   421,     0,   396,     0,     0,   659,
       0,   423,   397,     0,     0,   278,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,   384,   416,   417,   418,
     110,   111,   112,   419,   113,   385,     0,   114,   115,   656,
     657,   658,     0,     0,     0,   119,   120,     0,   386,   104,
     420,   421,     0,     0,   387,   388,   659,     0,   423,     0,
       0,     0,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   653,     0,     0,     0,     0,   105,   106,
       0,   107,   389,     0,     0,   390,     0,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,   392,   393,     0,  1836,   104,   423,     0,
       0,     0,     0,   384,     0,   394,     0,     0,     0,   395,
       0,     0,   385,     0,     0,     0,     0,   654,     0,     0,
       0,     0,     0,     0,     0,   386,   105,   106,     0,   107,
       0,   387,   388,     0,     0,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
     653,     0,     0,   119,   120,     0,     0,     0,     0,   389,
     655,     0,   390,     0,   988,     0,   396,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
     392,   393,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,   394,     0,     0,     0,   395,     0,     0,     0,
     403,     0,     0,     0,   879,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,     0,     0,   114,   115,   656,
     657,   658,     0,     0,     0,   119,   120,   655,     0,     0,
     420,   421,     0,   396,     0,     0,   659,     0,   423,   397,
       0,     0,   278,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,   401,   402,   203,   204,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,     0,
       0,   414,   415,   384,   416,   417,   418,   110,   111,   112,
     419,   113,   385,     0,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,   104,   386,     0,   420,   421,     0,
       0,   387,   388,   659,     0,   423,     0,     0,     0,   278,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,   106,     0,   107,     0,     0,   389,
       0,     0,   390,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,     0,     0,     0,     0,   391,     0,
     392,   393,     0,   423,   104,     0,     0,     0,     0,     0,
     384,     0,   394,     0,     0,     0,   395,     0,     0,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   386,   105,   106,   104,   107,     0,   387,   388,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,   105,   106,   389,   107,     0,   390,
       0,  1698,     0,   396,   108,   109,   110,   111,   112,   397,
     113,     0,     0,   114,   115,   116,   117,   118,     0,   398,
       0,   119,   120,     0,     0,   391,     0,   392,   393,     0,
    1658,     0,     0,   399,   400,   401,   402,   203,   204,   394,
       0,     0,     0,   395,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,     0,
       0,   414,   415,     0,   416,   417,   418,   110,   111,   112,
     419,   113,  1432,     0,   114,   115,   116,   117,   118,     0,
     696,     0,   119,   120,     0,     0,     0,   420,   421,   489,
     396,     0,     0,   484,     0,   423,   397,     0,     0,   278,
       0,     0,     6,     0,     0,     0,   398,     0,     0,  -287,
       0,     0,     0,  1545,  1546,     0,  1547,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,  1548,   403,     0,     0,     0,     0,  1549,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   386,     0,   420,   421,     0,     0,   387,   388,
     484,     0,   423,     0,     0,     0,   278,     0,     0,  1550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,    14,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1551,     0,     0,     0,     0,     0,     0,
     384,     0,     0,    16,     0,   391,     0,   392,   393,   385,
       0,     0,  1673,     0,     0,     0,     0,     0,     0,   394,
       0,   104,   386,   395,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1552,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
    1553,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     396,     0,     0,     0,     0,     0,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,     0,     0,
     422,     0,   423,     0,   688,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,   104,   386,     0,   420,   421,     0,     0,   387,   388,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,    75,
     384,   104,     0,     0,     0,     0,  1669,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   386,   395,     0,     0,     0,     0,   387,   388,
     105,   106,     0,   107,     0,     0,     0,     0,     0,     0,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,   389,   119,   120,   390,
       0,     0,     0,    76,     0,     0,     0,     0,     0,     0,
     696,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,   391,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,    97,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,    98,     0,   420,   421,   398,     0,     0,     0,
     484,     0,   423,     0,    99,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,   133,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,   100,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,   104,   386,     0,   420,   421,     0,     0,   387,   388,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
     105,   106,     0,   107,     0,     0,   389,     0,   102,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     384,     0,     0,   103,   957,     0,     0,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,   104,   386,   395,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     396,     0,     0,     0,     0,     0,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,     0,     0,
     484,   954,   423,     0,     0,     0,   278,  1151,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,   104,   386,     0,   420,   421,     0,     0,   387,   388,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     384,     0,     0,     0,     0,     0,     0,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   386,   395,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,     0,     0,
     484,     0,   423,     0,     0,     0,   278,  1347,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   386,     0,   420,   421,     0,     0,   387,   388,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,     0,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   386,   395,     0,   104,     0,     0,   387,   388,
       0,     0,     0,     0,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,     0,   179,   180,   105,   106,   389,   107,     0,   390,
     181,   182,     0,     0,   108,   109,   110,   111,   112,   183,
     113,   184,     0,   114,   115,   116,   117,   118,     0,     0,
     396,   119,   120,     0,     0,     0,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,  1720,     0,
     484,     0,   423,     0,     0,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,  1432,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,   420,   421,     6,     0,     0,  1432,
     484,     0,   423,  -289,     0,     0,   278,  1545,  1546,     0,
    1547,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1548,     0,     6,
       0,     0,     0,  1549,     0,     0,     0,     0,     0,     0,
    1545,  1546,     0,  1547,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1548,     0,     0,     0,     0,     0,  1549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   596,     0,     0,     0,
     597,     0,     0,     0,     0,     0,     0,   796,     0,     0,
       0,     0,     0,  1550,     0,     0,     0,     0,   598,     0,
       0,   599,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1550,  1551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,   600,     0,     0,    14,     0,     0,   601,   602,
     603,     0,   604,   605,   606,   104,   607,     0,     0,     0,
    1551,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,   608,   596,   609,     0,
       0,   597,     0,     0,  1552,   106,     0,   107,   104,     0,
       0,     0,     0,     0,   108,   109,   110,   111,   112,   598,
     113,     0,   599,   114,  1553,   116,   117,   118,     0,   610,
       0,   119,   120,     0,     0,     0,     0,  1552,   106,     0,
     107,     0,     0,     0,     0,     0,   611,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,  1553,   116,   117,
     118,     0,     0,   600,   119,   120,     0,     0,   612,   601,
     602,   603,     0,   604,   605,   606,     0,   607,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   608,     0,   609,
       0,     0,     0,   613,   614,     0,     0,     0,     0,     0,
       0,     0,   105,   106,     0,   107,     0,     0,     0,     0,
     615,     0,   108,   109,   110,   111,   112,     0,   113,     0,
     610,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,     0,     0,     0,   611,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   612,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   613,   614,     0,     0,     0,     0,
       0,     0,     0,   105,   106,     0,   107,     0,     0,     0,
       0,   615,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120
};

static const short yycheck[] =
{
       9,     4,     0,    12,    54,   286,    16,    16,   149,    18,
      63,   103,   255,    62,   711,    55,   362,   139,   140,   358,
     511,    62,   362,   333,   354,    61,    66,   422,    67,    56,
     555,    84,   557,    93,   497,   139,   140,   771,    88,    72,
     565,    68,   331,    52,    53,    54,    55,    56,    57,    89,
     565,    90,   523,    62,    63,    64,    96,    66,    90,    68,
      69,    94,    71,    72,   761,    69,   676,   688,   988,   696,
    1043,    80,    82,    82,    83,    84,   264,    86,    87,    88,
      89,    91,    91,    92,    93,    94,   274,    96,    92,   130,
    1037,   132,    87,   153,  1265,   367,  1043,   729,   725,  1037,
    1044,   472,   648,   940,   131,  1043,   280,   641,  1037,  1053,
     832,   151,   379,   326,  1043,   307,   764,   710,   219,   863,
    1037,   130,   131,   132,   988,   152,  1043,  1037,  1088,  1093,
    1070,   157,  1076,  1043,   767,   144,   228,  1207,  1383,   805,
     149,   978,   151,   152,   153,   793,     6,  1557,   157,   144,
     760,     8,   188,  1398,   802,     8,     8,     8,     8,     8,
    1258,     8,   654,     8,    10,   654,   307,   659,    10,    26,
      50,    50,  1178,  1472,    31,    23,    26,   213,  1184,   422,
      46,    31,    15,   478,    66,  1263,  1313,     4,    64,    49,
     160,   270,   531,   729,    20,   157,  1229,   148,    26,    24,
    1199,   101,  1201,    31,     9,    64,  1304,  1211,   856,   140,
     311,   541,   542,   543,   544,   103,   257,    29,    59,   565,
      66,   186,   123,   315,    67,   565,    23,   193,  1642,    16,
    1644,     6,     6,   151,   261,    52,    94,    46,    81,   133,
     191,   291,   163,   270,   516,   103,    89,   738,   257,   201,
     281,   590,   261,    58,   151,   264,   265,  1094,   289,    53,
      54,   270,   105,   164,    69,   274,   307,    47,   295,   190,
     891,   892,   272,   170,   274,  1235,    25,   465,   567,   132,
     132,   132,   291,   132,   202,   132,   295,   132,    66,    98,
    1704,   332,   186,   818,    91,    82,    26,    46,   339,   124,
     341,    31,   343,   254,    84,   346,  1310,   273,    47,   274,
     211,   273,     8,    11,   935,     8,   159,  1415,  1291,   355,
      47,   357,   163,   332,   364,   375,    75,   616,    26,   596,
     339,   472,   341,    31,   343,   371,  1280,   346,   238,   971,
     199,    84,   273,     8,  1291,    94,   373,   164,   318,  1476,
      48,   983,   984,  1291,   103,   364,   365,  1297,  1222,  1437,
     109,   365,  1291,  1011,   373,   182,   375,  1777,   173,   979,
    1403,  1377,  1378,   185,  1291,   263,   148,   840,   127,    26,
    1610,  1291,  1048,  1049,    31,  1799,   212,   879,   163,   163,
     272,   883,   272,   272,   883,   676,   272,  1696,   264,   216,
     479,    26,  1401,   197,  1814,   263,    31,  1055,   188,   269,
     267,   268,   269,   270,   423,  1036,  1587,   267,   268,   269,
     270,   274,   274,   274,   464,   274,   272,   274,   276,   274,
     272,   472,  1098,   460,  1844,   971,   132,   732,   478,   132,
     273,   269,   270,  1853,   471,   188,  1856,   983,   475,   188,
     693,   460,   479,  1523,  1864,   464,   465,   483,   739,   702,
     561,   188,   471,  1708,     8,  1098,   475,   132,  1698,   478,
     479,    26,    12,    12,   483,    66,    31,    14,   582,   760,
      29,    94,  1756,  1757,  1428,   181,  1208,  1711,   181,  1433,
    1450,  1435,   781,  1114,  1115,   784,  1117,  1118,    71,     3,
     536,  1599,  1698,   182,    24,  1469,  1240,  1605,  1468,     6,
      19,   800,  1518,    33,   263,   252,   149,  1688,    18,    53,
      54,  1751,   272,    89,   565,    26,  1470,   277,  1679,    19,
      31,   632,   720,   166,    34,  1128,   825,   267,   268,   269,
     270,   198,   272,  1483,   583,  1409,   102,   271,   114,   585,
      47,   583,    47,  1087,   563,  1751,   565,   867,  1104,  1303,
     569,   735,    59,   204,  1286,   757,  1796,   780,     7,   267,
     268,   269,   270,  1744,   111,  1849,    80,    86,  1802,    79,
       3,   130,   854,    65,  1709,  1710,  1218,    84,   132,    84,
     177,   182,  1536,   272,   207,   208,   136,   136,  1542,   640,
    1796,   637,   638,   176,   124,   976,   172,   204,   108,   876,
     183,     4,   123,  1700,  1785,   272,   757,   204,  1769,   175,
     267,   268,   269,   270,    17,  1246,   146,   274,  1753,  1780,
     155,   640,  1783,   153,   191,    10,   185,   200,   279,   743,
     649,   147,   267,   268,   269,   270,   649,  1268,  1592,   274,
      26,  1488,   200,   202,  1302,    31,   153,    80,   153,    52,
    1811,    54,  1749,   197,   146,  1609,   163,   187,   163,    26,
     679,   712,   776,   777,    31,   175,   273,     6,   204,  1800,
    1801,   268,   279,   194,   679,  1836,   273,   204,   204,  1423,
    1841,   188,   732,   188,  1024,   704,    26,   254,   979,     5,
      11,    31,   711,   712,  1829,   187,   715,  1334,  1795,   272,
      16,   720,   267,   268,   269,   270,   757,  1424,    47,   274,
     729,   177,   763,   732,   272,   752,   767,   754,   737,    86,
      59,   204,   741,   204,   770,   771,   772,    48,  1859,    88,
     779,  1828,   204,   752,  1717,   754,   272,   779,   204,   790,
     204,  1695,   761,  1042,   763,    84,  1037,   273,   767,  1089,
     231,  1705,  1043,   279,  1471,  1054,   267,   268,   269,   270,
    1717,   164,   960,   274,   123,   963,   817,   818,  1698,  1717,
     819,   790,   237,   129,     6,   824,  1075,   819,  1717,  1399,
     245,  1101,  1413,  1188,   830,  1266,     6,   268,   837,   273,
    1717,    25,   151,   102,  1748,   154,   268,  1717,   817,   818,
     849,   850,   268,   852,   268,   124,    83,   849,   850,     4,
     852,   170,     7,   279,   153,    26,  1289,    49,   855,    98,
      31,   206,  1295,  1228,   163,   976,    26,   146,   865,    49,
     139,    31,   141,     6,   153,   272,   855,   196,  1363,    33,
     277,    75,    26,   120,  1164,   124,   865,    31,     6,   188,
     235,   236,    46,   238,    68,   164,   133,    52,   877,   177,
     245,   246,   247,   248,   249,   272,   251,   206,   187,   254,
     255,   256,   257,   258,    26,   109,    49,   262,   263,    31,
     189,   267,   268,   269,   270,   272,   204,   274,   274,     6,
      26,    49,    68,   127,   964,    31,   235,   236,   129,   238,
     267,   268,   269,   270,   271,   131,   245,   246,   247,   248,
     249,   961,   251,   148,    47,   254,   255,   256,   257,   258,
    1211,    26,   965,   262,   263,   976,    31,   267,   268,   269,
     270,   272,    49,    26,   274,  1188,   277,   129,    31,   148,
      67,   960,   961,   962,   963,   964,   965,   204,   962,   184,
     268,    84,   971,    67,    81,   211,    67,   213,  1307,  1073,
     203,   204,   205,    26,   983,   984,   239,    81,    31,   988,
      81,   180,    62,    63,   231,  1228,   249,    26,   105,  1278,
     275,   182,    31,   278,   129,    40,   195,    26,   231,   232,
     240,   105,    31,   878,   105,   164,    51,   882,   239,   240,
    1291,    26,   129,    58,   207,   208,    31,  1363,    83,  1544,
    1309,   137,   138,  1363,     0,   129,  1647,   271,   272,  1310,
    1521,    20,     8,  1524,   272,    26,   274,   272,    83,   274,
      31,   675,   159,   677,  1665,  1084,    64,   681,   682,   683,
    1091,    66,  1084,   271,   272,   159,  1065,  1098,   159,   273,
     272,  1100,   274,   207,  1067,    26,    10,  1106,  1100,    62,
      31,  1080,   170,  1199,  1106,  1201,    64,   267,   268,   269,
     270,   271,  1091,    83,  1206,  1207,  1208,  1784,    64,  1098,
    1420,    67,    26,   267,   268,   269,   270,    31,    74,    82,
     274,    77,    26,   148,   206,    81,    26,    31,   198,    86,
      86,    31,   272,    89,   274,  1736,   148,   273,  1399,    26,
    1611,   272,    64,   274,    31,   267,   268,   269,   270,   105,
      66,   148,   274,    57,   272,   180,   274,    57,  1242,   184,
    1244,   267,   268,   269,   270,   272,   252,   274,   274,   129,
    1439,  1769,   272,   129,   274,   129,   132,   133,   272,    66,
     274,   272,  1780,   274,   107,   196,   142,   272,   182,   274,
       0,   180,   267,   268,   269,   270,   130,   163,     8,   274,
     272,   188,   274,   159,   267,   268,   269,   270,   200,  1478,
    1199,   274,  1201,  1811,   272,   272,   274,   274,   272,   262,
     274,  1305,   262,   272,  1240,   181,    33,   157,   264,  1218,
     186,   215,   118,  1222,   267,   268,   269,   270,  1836,    32,
      10,   274,   198,  1841,   200,   273,  1679,  1848,   267,   268,
     269,   270,   273,   273,    64,   274,   273,    67,   267,   268,
     269,   270,   207,   273,    74,   274,   273,    77,   273,   273,
     273,    81,   267,   268,   269,   270,    86,   273,   207,    89,
     273,   207,   273,   273,  1755,   273,    26,   272,   182,    66,
    1411,    31,   273,   276,   148,   105,   267,   268,   269,   270,
     278,  1284,   278,   274,  1320,   272,    66,   272,  1324,   174,
     272,  1432,   264,    10,   278,   271,   272,   255,   274,   129,
     273,   272,   132,   133,    64,   128,   267,   268,   269,   270,
     251,    94,   142,   274,    24,    94,  1769,  1606,    94,   277,
      26,   272,    24,    46,    10,    31,   182,  1780,   286,   159,
    1783,   143,    26,   267,   268,   269,   270,    31,   216,   182,
     274,   143,   204,   267,   268,   269,   270,   267,   268,   269,
     270,   181,    23,    94,   193,   193,   186,   145,  1811,   317,
     267,   268,   269,   270,   273,    18,    95,   898,   198,   900,
     200,   902,   903,    26,   905,   906,   907,   273,    31,   276,
      86,    34,  1431,  1836,   273,  1526,   273,  1423,  1841,    64,
    1431,  1432,  1401,    10,   129,   157,   272,   135,    18,   197,
    1409,  1523,  1411,   128,   273,   273,    26,   274,   182,   210,
       0,    31,    63,  1440,    34,   273,  1557,    70,     8,    66,
      66,   198,  1431,  1432,   194,    10,    79,   248,   273,   204,
      23,  1440,    20,   273,    87,  1539,  1717,    46,    30,   132,
      90,   271,   272,    24,   274,    90,   164,   132,   132,   162,
      70,    46,   164,   106,   273,   108,   163,   273,   416,    79,
     102,    20,   420,   421,   422,   163,   273,    87,    10,   122,
     274,   124,   125,   126,    64,    66,   272,    67,   272,  1722,
      26,   272,   131,   113,    74,   272,   106,    77,   108,    66,
     272,    81,  1545,    12,    74,   273,    86,    45,  1590,    89,
     272,   247,   122,    10,   124,   125,   126,   267,   268,   269,
     270,   168,   273,    10,   273,   105,  1557,  1526,   274,   200,
     275,   274,   175,   274,   274,   186,   484,   140,    55,   238,
     488,    55,   274,     8,   274,    98,  1545,   273,  1679,   129,
     273,  1591,   132,   133,  1553,   273,   273,   273,  1557,   273,
      54,   273,   142,   273,   273,   175,   181,   273,   273,  1586,
      73,   267,   268,   269,   270,   273,   134,   134,   273,   159,
     273,   272,  1581,   267,   268,   269,   270,  1586,   273,  1615,
     134,   273,  1591,  1619,   274,    20,    89,    77,   137,   168,
     192,   181,   136,    17,   132,   153,   186,  1689,   132,   132,
    1610,  1610,   132,   132,   132,   274,   274,    24,   198,   273,
     200,   264,   265,   266,   267,   268,   269,   270,   274,   274,
    1660,   274,   163,    54,   181,   131,   274,   182,  1769,    10,
     274,   142,    26,    83,    47,   188,  1777,    84,   188,  1780,
      84,    47,  1783,   274,   264,   265,   266,   267,   268,   269,
     270,  1660,   274,    98,    40,    66,   117,    30,   131,   117,
     274,    86,   274,   274,   274,    51,   274,   274,   274,   274,
    1811,   274,    58,  1814,   274,  1728,   274,   274,   274,  1677,
     274,   271,   272,   272,   274,    71,    54,   274,  1698,  1698,
     274,   274,   274,   271,    86,  1836,   271,   273,   271,   271,
    1841,   659,  1711,  1844,   275,   271,  1716,  1716,    98,   271,
     271,    90,  1853,   273,   114,  1856,   197,   273,   676,  1728,
    1729,   273,   680,  1864,   271,   129,   271,  1730,   271,   687,
      86,   129,   272,   129,   692,   693,  1777,   274,    89,   278,
     272,  1751,  1751,   274,   702,   703,   132,   705,   706,   707,
     708,   709,   710,   274,  1764,  1764,    47,   156,  1808,   181,
     188,   133,   148,   273,    50,    53,   271,   209,  1777,    39,
     272,    50,     0,  1814,   274,  1784,   272,     0,   262,   165,
    1204,   739,    12,   265,  1104,   274,  1796,  1796,   720,   644,
    1090,  1101,   868,  1802,   180,  1805,  1805,  1459,   184,  1808,
     322,   187,   760,  1844,   329,  1814,   590,   765,   359,   195,
    1027,   763,  1853,  1091,     6,  1856,  1098,     9,   313,  1458,
     801,    13,  1657,  1864,   341,  1834,  1437,   341,   214,  1439,
      22,  1742,  1746,   206,  1787,  1844,  1745,   790,  1259,  1734,
    1778,  1722,  1013,    35,  1853,  1675,  1855,  1856,  1416,    41,
      42,  1237,  1861,   346,  1698,  1864,   334,   571,   327,  1293,
     629,   839,   235,   236,    13,   238,   299,   499,   847,   503,
     752,   765,   245,   246,   247,   248,   249,    69,   251,   142,
      72,   254,   255,   256,   257,   258,   739,  1648,   988,   262,
     263,   218,   219,   220,   221,   222,   223,   224,   225,  1534,
     273,  1805,  1764,  1861,     0,     0,     0,   511,   100,   101,
     728,  1248,   980,  1142,  1186,  1080,  1855,  1581,    33,    86,
     112,  1553,    -1,    -1,   116,    -1,   569,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   893,   894,   895,    -1,    -1,
     898,   899,   900,    -1,   902,   903,   904,   905,   906,   907,
     908,   909,    -1,   911,    -1,   913,    -1,    -1,   916,   917,
     918,   919,   920,   921,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   933,  1332,  1333,   936,   171,
      -1,   173,  1338,  1339,    -1,  1341,  1342,   179,    -1,    -1,
      -1,    -1,    -1,  1349,  1350,  1351,  1352,   189,  1354,  1355,
    1356,  1357,  1358,  1359,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,   979,    33,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,   267,   268,    -1,    51,    -1,
      -1,   273,    -1,   275,    -1,    58,    -1,   279,    -1,    62,
      63,   206,    -1,     6,    -1,    -1,     9,    -1,    71,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1128,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,  1140,  1141,    -1,  1143,  1144,   100,   101,    -1,
      -1,    -1,   165,  1151,    42,   206,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   180,    -1,    -1,
    1168,   184,    -1,    -1,    -1,    -1,    -1,    -1,  1176,  1177,
    1178,    -1,   195,    -1,   235,   236,  1184,   238,  1186,  1187,
    1188,  1189,  1190,    -1,   245,   246,   247,   248,   249,    -1,
     251,   214,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,  1211,    -1,    -1,    -1,    -1,   171,    -1,
     173,    -1,    -1,    74,    -1,    -1,   179,    -1,    -1,    -1,
    1228,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1261,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,  1310,    -1,   267,   268,    -1,    -1,   206,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,  1331,    -1,    -1,     6,    -1,   189,     9,
      -1,    -1,  1340,    13,    -1,    -1,    -1,   235,   236,  1347,
     238,    -1,    22,    -1,    -1,   206,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    35,   254,   255,   256,   257,
     258,    41,    42,    -1,   262,   263,   163,    -1,    -1,  1377,
    1378,   269,    -1,    -1,   235,   236,    -1,   238,     7,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    69,
     251,  1399,    72,   254,   255,   256,   257,   258,    -1,    28,
      -1,   262,   263,    -1,  1412,    -1,    -1,    36,    -1,   206,
      -1,    -1,    -1,    -1,    43,    -1,    45,    -1,    47,    -1,
     100,   101,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,   112,    -1,    -1,    -1,   116,    -1,   235,   236,
      -1,   238,    -1,    -1,    73,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    84,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1496,  1497,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,  1513,    -1,    -1,    -1,   189,
    1518,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   203,   204,   205,   206,   207,   208,   158,
      -1,    -1,   161,    -1,   163,   164,    -1,   217,    -1,   114,
      -1,    -1,  1550,    -1,    -1,    41,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,   188,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    69,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,   267,   268,    -1,
      -1,     6,    -1,   273,     9,   275,    -1,   172,    13,   279,
      -1,    -1,    98,    -1,    -1,    -1,  1614,    22,   237,    -1,
      -1,    -1,    -1,    -1,  1622,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,   252,   253,    -1,    41,    42,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1648,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1659,    -1,    -1,    69,  1663,    -1,    72,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,   173,    -1,   254,
     255,   256,   257,   258,    -1,   100,   101,   262,   263,     6,
      -1,    -1,     9,   189,    -1,    -1,    13,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,   203,   204,   205,
      -1,   207,   208,    -1,  1722,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,    -1,
      -1,    -1,    -1,    -1,    -1,   241,   242,    -1,    -1,    -1,
      -1,    -1,    69,    -1,   250,    72,   171,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,  1779,   268,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,    -1,
     275,    -1,   179,    -1,   279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     217,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      41,   238,    -1,    44,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
     267,   268,     6,    -1,    -1,     9,   273,    -1,   275,    13,
      -1,    19,   279,    -1,    85,    23,    -1,    -1,    22,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    37,
      -1,    35,    -1,    41,    -1,    -1,    44,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,   152,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,   100,   101,   169,    -1,
       6,    -1,    -1,     9,    -1,    -1,    -1,    13,   112,    -1,
      -1,   119,   116,   121,    -1,    -1,    22,    -1,    -1,    -1,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,   169,    -1,    69,    -1,    -1,    72,   171,    -1,   173,
      -1,    -1,   243,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,   217,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,   243,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,
      -1,   275,    74,   179,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   148,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,   267,   268,     6,    -1,    -1,     9,   273,    -1,   275,
      13,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,   184,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    35,   195,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    69,    -1,    -1,    72,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,   100,   101,    -1,
     262,   263,    -1,    -1,    -1,    -1,    -1,    13,   206,   112,
      -1,    -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    60,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    69,   262,   263,    72,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   273,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   163,    -1,   262,
     263,   167,    -1,    -1,   267,   268,    -1,   173,    -1,    -1,
     273,    -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,   180,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,    13,
     206,   267,   268,    -1,    -1,    -1,    -1,   273,    22,   275,
      -1,    -1,    -1,   279,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    38,    -1,    -1,    41,    42,   235,
     236,    -1,   238,   206,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    60,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    69,   262,   263,    72,    -1,
      -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,   100,   101,    -1,   262,
     263,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,   167,    -1,    72,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     167,    -1,    -1,   267,   268,    -1,   173,    -1,    -1,   273,
      -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    13,   244,   245,   246,
     247,   248,   249,   250,   251,    22,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    35,   206,
     267,   268,    -1,    -1,    41,    42,   273,    -1,   275,    -1,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    69,    -1,    -1,    72,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,   273,   206,   275,    -1,
      -1,    -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    22,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,   235,   236,    -1,   238,
      -1,    41,    42,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      60,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    69,
     167,    -1,    72,    -1,   273,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,   167,    -1,    -1,
     267,   268,    -1,   173,    -1,    -1,   273,    -1,   275,   179,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    13,   244,   245,   246,   247,   248,   249,
     250,   251,    22,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   206,    35,    -1,   267,   268,    -1,
      -1,    41,    42,   273,    -1,   275,    -1,    -1,    -1,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    69,
      -1,    -1,    72,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,    -1,   275,   206,    -1,    -1,    -1,    -1,    -1,
      13,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,   235,   236,   206,   238,    -1,    41,    42,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,   235,   236,    69,   238,    -1,    72,
      -1,   273,    -1,   173,   245,   246,   247,   248,   249,   179,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,   189,
      -1,   262,   263,    -1,    -1,    98,    -1,   100,   101,    -1,
     271,    -1,    -1,   203,   204,   205,   206,   207,   208,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    17,    -1,   254,   255,   256,   257,   258,    -1,
     163,    -1,   262,   263,    -1,    -1,    -1,   267,   268,   269,
     173,    -1,    -1,   273,    -1,   275,   179,    -1,    -1,   279,
      -1,    -1,    47,    -1,    -1,    -1,   189,    -1,    -1,    54,
      -1,    -1,    -1,    58,    59,    -1,    61,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,   217,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,   163,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,   188,    -1,    98,    -1,   100,   101,    22,
      -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   206,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,   197,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   206,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,   204,
      13,   206,    -1,    -1,    -1,    -1,   271,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    98,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,     6,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    47,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,    59,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   206,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,   163,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      13,    -1,    -1,   188,   269,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   206,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,   274,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   206,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,   206,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
      -1,    -1,   233,   234,   235,   236,    69,   238,    -1,    72,
     241,   242,    -1,    -1,   245,   246,   247,   248,   249,   250,
     251,   252,    -1,   254,   255,   256,   257,   258,    -1,    -1,
     173,   262,   263,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,   271,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    17,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,    47,    -1,    -1,    17,
     273,    -1,   275,    54,    -1,    -1,   279,    58,    59,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    47,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    85,    -1,    -1,   163,    -1,    -1,    91,    92,
      93,    -1,    95,    96,    97,   206,    99,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,   119,    19,   121,    -1,
      -1,    23,    -1,    -1,   235,   236,    -1,   238,   206,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    41,
     251,    -1,    44,   254,   255,   256,   257,   258,    -1,   152,
      -1,   262,   263,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,   169,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    85,   262,   263,    -1,    -1,   191,    91,
      92,    93,    -1,    95,    96,    97,    -1,    99,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,
     243,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
     152,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,   243,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263
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

#ifndef SHLIB_DEFS
int DSQL_yychar;
#endif
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
				yyvsp[-3], MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG)); }
    break;
case 88:
{ yyval = yyvsp[-1]; }
    break;
case 89:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((STR) FUN_reference, CONSTANT_SLONG));}
    break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((STR) (-1 * FUN_reference), CONSTANT_SLONG));}
    break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((STR) FUN_value, CONSTANT_SLONG));}
    break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG));}
    break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 121:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 122:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 123:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 124:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
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
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
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
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
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
{ yyval = make_node (nod_foreign, e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 238:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 239:
{ yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 249:
{ yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
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
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 254:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 255:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 256:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
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
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
    break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
    break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
    break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
    break;
case 264:
{ yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 265:
{ yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 266:
{ yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 267:
{ yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
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
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
    break;
case 280:
{ yyval = NULL; }
    break;
case 281:
{ yyval = NULL; }
    break;
case 282:
{ yyval = yyvsp[0]; }
    break;
case 283:
{ yyval = yyvsp[0]; }
    break;
case 286:
{ yyval = yyvsp[-1]; }
    break;
case 287:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 288:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 289:
{ yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
    break;
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 307:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
    break;
case 308:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
    break;
case 311:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 312:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
    break;
case 313:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 314:
{ yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
    break;
case 317:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 318:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 319:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 320:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 321:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 322:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 323:
{ yyval = NULL; }
    break;
case 324:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;
case 325:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
case 329:
{ yyval = make_list (yyvsp[0]); }
    break;
case 330:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 331:
{ yyval = NULL; }
    break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 336:
{ yyval = make_node (nod_while, e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 337:
{ yyval = make_node (nod_label, 1, yyvsp[-1]); }
    break;
case 338:
{ yyval = NULL; }
    break;
case 339:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
    break;
case 340:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
    break;
case 341:
{ yyval = make_node (nod_breakleave, e_breakleave_count,
				make_node (nod_label, 1, yyvsp[-1])); }
    break;
case 342:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 343:
{ yyval = NULL; }
    break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 346:
{ yyval = make_node (nod_on_error, e_err_count,
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
case 353:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 354:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 355:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 356:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
    break;
case 357:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 359:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 360:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 361:
{ yyval = make_list (yyvsp[0]); }
    break;
case 363:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 367:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 368:
{ yyval = yyvsp[-1]; }
    break;
case 369:
{ lex.beginning = lex_position(); }
    break;
case 370:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;
case 371:
{ lex.beginning = lex.last_token; }
    break;
case 372:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
    break;
case 373:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 374:
{ yyval = 0; }
    break;
case 375:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 379:
{ yyval = NULL; }
    break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 381:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 382:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
    break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
    break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
    break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 399:
{ yyval = NULL; }
    break;
case 400:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 401:
{ yyval = yyvsp[0]; }
    break;
case 402:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 403:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 404:
{ yyval = yyvsp[0]; }
    break;
case 405:
{ yyval = yyvsp[0]; }
    break;
case 406:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 407:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 408:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 409:
{ yyval = yyvsp[0]; }
    break;
case 410:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 412:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 413:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 414:
{ yyval = yyvsp[0]; }
    break;
case 415:
{ yyval = yyvsp[0]; }
    break;
case 416:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 417:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 418:
{ yyval = yyvsp[0]; }
    break;
case 419:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 422:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 424:
{ yyval = yyvsp[0]; }
    break;
case 425:
{ yyval = yyvsp[0]; }
    break;
case 426:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 427:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 428:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 429:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 453:
{ yyval = NULL; }
    break;
case 454:
{ yyval = NULL; }
    break;
case 455:
{ yyval = NULL; }
    break;
case 456:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 457:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 458:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 459:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 460:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 461:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 462:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 463:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;
case 465:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 466:
{ yyval = yyvsp[0]; }
    break;
case 467:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 468:
{ yyval = yyvsp[0]; }
    break;
case 469:
{ yyval = yyvsp[0]; }
    break;
case 470:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 471:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 472:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 473:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 475:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 476:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 478:
{ yyval = NULL; }
    break;
case 480:
{ yyval = NULL; }
    break;
case 481:
{ yyval = yyvsp[0]; }
    break;
case 482:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 483:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 484:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 485:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 486:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 487:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 488:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 489:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 490:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 491:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 492:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 498:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 499:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 502:
{ if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 503:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 509:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
    break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 512:
{ 
			*stmt_ambiguous = TRUE;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, gds_arg_number, (SLONG) 301, 
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
case 513:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
    break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 521:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
    break;
case 522:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 523:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
    break;
case 524:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 525:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 526:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 542:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 543:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 544:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 545:
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
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );

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
case 546:
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
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );
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
case 549:
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
case 550:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 553:
{ yyval = yyvsp[-1]; }
    break;
case 554:
{ yyval = 0; }
    break;
case 558:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 559:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 560:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 564:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 565:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 566:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 567:
{ yyval = 0; }
    break;
case 568:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 571:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 572:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 575:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 576:
{ yyval = NULL; }
    break;
case 578:
{ yyval = NULL; }
    break;
case 579:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 581:
{ yyval = NULL; }
    break;
case 583:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 588:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 589:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 590:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 591:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 592:
{ yyval = yyvsp[0];}
    break;
case 594:
{ yyval = yyvsp[0];}
    break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 600:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 601:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 602:
{ yyval = 0; }
    break;
case 603:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 604:
{ yyval = (DSQL_NOD) NOD_SHARED; }
    break;
case 605:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;
case 606:
{ yyval = (DSQL_NOD) 0; }
    break;
case 607:
{ yyval = (DSQL_NOD) NOD_READ; }
    break;
case 608:
{ yyval = (DSQL_NOD) NOD_WRITE; }
    break;
case 610:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 611:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 612:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 613:
{ yyval = 0; }
    break;
case 615:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 616:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 617:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 618:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 619:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 620:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 621:
{ yyval = make_list (yyvsp[0]); }
    break;
case 622:
{ yyval = 0; }
    break;
case 624:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 625:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 626:
{ yyval = 0; }
    break;
case 627:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 628:
{ yyval = 0; }
    break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 630:
{ yyval = 0; }
    break;
case 631:
{ yyval = yyvsp[-1]; }
    break;
case 632:
{ yyval = 0; }
    break;
case 633:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 634:
{ yyval = 0; }
    break;
case 635:
{ yyval = yyvsp[0]; }
    break;
case 636:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 637:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 638:
{ yyval = 0; }
    break;
case 639:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 640:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 641:
{ lex.limit_clause = true; }
    break;
case 642:
{ lex.limit_clause = false; }
    break;
case 643:
{ lex.first_detection = true; }
    break;
case 644:
{ lex.first_detection = false; }
    break;
case 645:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 646:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;
case 647:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;
case 648:
{ yyval = 0; }
    break;
case 649:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 650:
{ yyval = yyvsp[-2]; }
    break;
case 651:
{ yyval = yyvsp[-1]; }
    break;
case 652:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 653:
{ yyval = yyvsp[-1]; }
    break;
case 654:
{ yyval = yyvsp[0]; }
    break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 656:
{ yyval = 0; }
    break;
case 657:
{ yyval = make_list (yyvsp[0]); }
    break;
case 658:
{ yyval = 0; }
    break;
case 660:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 662:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 665:
{ yyval = make_list (yyvsp[0]); }
    break;
case 667:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 671:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 673:
{ yyval = NULL; }
    break;
case 674:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 675:
{ yyval = NULL; }
    break;
case 677:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 679:
{ yyval = yyvsp[-1]; }
    break;
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 681:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 682:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 683:
{ yyval = NULL; }
    break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 689:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 690:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 691:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 692:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 693:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 695:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 696:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 697:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 698:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 699:
{ yyval = make_list (yyvsp[0]); }
    break;
case 700:
{ yyval = 0; }
    break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 704:
{ yyval = yyvsp[0]; }
    break;
case 705:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 711:
{ yyval = 0; }
    break;
case 712:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 713:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 714:
{ yyval = 0; }
    break;
case 715:
{ yyval = 0; }
    break;
case 717:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 718:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 722:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 723:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 724:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
    break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 727:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 728:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 733:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 734:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 735:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 738:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 739:
{ yyval = make_node (nod_update, e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 741:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 745:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 746:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 747:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 748:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 752:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 753:
{ yyval = yyvsp[0]; }
    break;
case 755:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 758:
{ yyval = NULL; }
    break;
case 759:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 779:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 780:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 781:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 782:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 783:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 785:
{ yyval = yyvsp[-1]; }
    break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 798:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 802:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 806:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 807:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 808:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 809:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 810:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 811:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 812:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 816:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 817:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 818:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 819:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 820:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 825:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 827:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 829:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 831:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 833:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 835:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 837:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 841:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 842:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 843:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 844:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 848:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 849:
{ yyval = yyvsp[-1]; }
    break;
case 850:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 851:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 861:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 862:
{ yyval = yyvsp[0]; }
    break;
case 863:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 864:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 865:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 866:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 867:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 868:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 869:
{ yyval = yyvsp[-1]; }
    break;
case 870:
{ yyval = yyvsp[-1]; }
    break;
case 874:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 875:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 876:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 877:
{ yyval = yyvsp[0]; }
    break;
case 878:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
    break;
case 879:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
    break;
case 880:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 881:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 883:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 885:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 886:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;
case 887:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 888:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 889:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 890:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 892:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;
case 893:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DATE);
			}
    break;
case 894:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIME); 
			}
    break;
case 895:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 896:
{ yyval = make_parameter (); }
    break;
case 897:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 898:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 899:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 900:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 901:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 902:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 903:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 904:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 905:
{ yyval = yyvsp[0]; }
    break;
case 906:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 908:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 909:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 910:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 911:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 912:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 914:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 915:
{ yyval = yyvsp[0];}
    break;
case 920:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 921:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 922:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 923:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 924:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 925:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 926:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 927:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 928:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 929:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 930:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 933:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 935:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 936:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 937:
{ yyval = yyvsp[0]; }
    break;
case 938:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 939:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 940:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 941:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 944:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 945:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 948:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 949:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 950:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 951:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 952:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 953:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 954:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 955:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 959:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;
case 960:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;
case 961:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;
case 962:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 963:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 964:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;
case 965:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 966:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 969:
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
		SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) sym;
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		STR str_ = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) str;
		str_->str_length = symbol->sym_length;
		strncpy((char*)str_->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str_;
		HSHD_insert(symbol);
	}
}


void LEX_string (
	TEXT	*string,
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
#ifdef DEV_BUILD
	if (DSQL_debug & 32)
		printf("%.*s\n", (int)length, string);
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


static TEXT *lex_position (void)
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

	for (const char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return true;
}
#endif

static DSQL_FLD make_field (DSQL_NOD field_name)
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
	DSQL_FLD	field;
	STR	string;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (field_name == NULL)
	{
		field = FB_NEW_RPT(*tdsql->tsql_default, sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	string = (STR) field_name->nod_arg [1];
	field = FB_NEW_RPT(*tdsql->tsql_default, strlen ((SCHAR*) string->str_data)) dsql_fld;
	strcpy (field->fld_name, (TEXT*) string->str_data);

	return field;
}


static FIL make_file (void)
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
	FIL	temp_file;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;
		   
	temp_file = FB_NEW(*tdsql->tsql_default) fil;

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
	DSQL_NOD	*ptr;
	DLLS	stack, temp;
	USHORT	l;
	DSQL_NOD	old;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	stack = 0;
	stack_nodes (node, &stack);
	for (l = 0, temp = stack; temp; temp = temp->lls_next)
		l++;

	old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	ptr = node->nod_arg + node->nod_count;

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
	DSQL_NOD	node;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
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
#if defined(DEV_BUILD)
	DSQL_debug = level;
#endif
	if (level >> 8)
		*yydeb = level >> 8;
	/* CVC: I added this code form Mike Nordell to see the output from internal
	   operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
	   When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
	static FILE* redirected_output;
	if (level > 0) {
		/* Console debug code inside this scope */
		if (AllocConsole()) {
			redirected_output = freopen("CONOUT$", "wt", stdout);
			printf("DebugConsole - Yes, it's working.\n");
		}
	}
	else if (level <= 0 && redirected_output) {
		fclose (redirected_output);
		redirected_output = 0;
		FreeConsole();
	}
#endif
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

	if (((STR) string)->str_length > 5) {
		return false;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];	
	buf [0] = ((STR) string)->str_data[0];
	buf [1] = ((STR) string)->str_data[1];
	buf [2] = ((STR) string)->str_data[2];
	buf [3] = ((STR) string)->str_data[3];
	buf [4] = ((STR) string)->str_data[4];
	buf [5] = '\0';

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
						 DLLS		*stack)
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
	DSQL_NOD	*ptr, *end;
	DSQL_NOD	 curr_node, next_node, start_chain, end_chain, save_link;

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

	start_chain = node;
	end_chain = NULL;
	curr_node = node;
	next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
		{

		/* pattern was found so reverse the links and go to next node */

		save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
		}

	/* see if any chain was found */

	if ( end_chain)
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
			save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
			}
		return;
		}

	for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

inline static int yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
{
	int temp = lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
}

int LexerState::yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
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
	SYM	sym;
	SSHORT	c;
	USHORT	buffer_len;

	STR delimited_id_str;

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

		tok_class = classes [c];

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
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
		if (ptr >= end)
			return -1;
		CHECK_COPY_INCR(p, UPPER7(*ptr));
		}
		
		CHECK_BOUND(p);
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
			delimited_id_str = (STR) yylval;
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

	assert(ptr <= end);

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (ptr < end) && (classes [*ptr] & CHR_DIGIT)))
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
			if (have_exp_digit && (! (classes [c]  & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes [c]  & CHR_DIGIT)))
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
				else if ( classes [c]  & CHR_DIGIT )
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
			else if (classes [c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
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
			assert(have_digit);

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
							   gds_arg_string,
							   ERR_string( last_token,
								   ptr - last_token ),
							   gds_arg_end );
					ERRD_post_warning( isc_dsql_warning_number_ambiguous1,
							   gds_arg_end );
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
		CHECK_COPY_INCR(p, UPPER (c));
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			CHECK_COPY_INCR(p, UPPER (*ptr));
		}

		CHECK_BOUND(p);
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
			if ((sym->sym_keyword == FIRST && !first_detection) || sym->sym_keyword == SKIP) {
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
			} else {
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
		} else {
			/* Restore status quo. */
			lex = savedState;
		}
	}

	/* Single character punctuation are simply passed on */

	return c;
}


static void yyerror (
	TEXT	*error_string)
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
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
		0);
	else
	{
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_dsql_token_unk_err, 
		gds_arg_number, (SLONG) lex.lines, 
		gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
		gds_arg_gds, gds_random, 
		gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
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

	ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) sql_code, 
		gds_arg_gds, error_symbol, 0);
}
