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
 *            clashes with normal DEBUG macro.
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
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
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
 *                           with table alias. Also removed group_by_function and ordinal.
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
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

/* Can't include ../jrd/err_proto.h here because it pulls jrd.h. */
#if !defined(JRD_ERR_PROTO_H)
extern "C" TEXT *DLL_EXPORT ERR_string(const TEXT*, int);
#endif

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

#define MIN_CACHE_BUFFERS       250
#define DEF_CACHE_BUFFERS       1000

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
    if ((to - string) >= MAX_TOKEN_LEN)        \
	yyabandon (-104, isc_token_too_long); \
    }
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	long_int (DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	short_int (DSQL_NOD, SLONG *, SSHORT);
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



#define	YYFINAL		1850
#define	YYFLAG		-32768
#define	YYNTBASE	280

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 517 ? yytranslate[x] : 693)

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
     958,   962,   967,   970,   977,   982,   984,   986,   994,  1003,
    1010,  1019,  1026,  1031,  1032,  1037,  1040,  1042,  1046,  1047,
    1050,  1055,  1056,  1058,  1060,  1064,  1068,  1075,  1078,  1081,
    1085,  1086,  1088,  1091,  1096,  1098,  1102,  1105,  1108,  1111,
    1113,  1118,  1126,  1134,  1136,  1138,  1142,  1147,  1155,  1158,
    1160,  1164,  1166,  1168,  1175,  1179,  1180,  1181,  1182,  1183,
    1187,  1188,  1198,  1208,  1210,  1212,  1213,  1216,  1218,  1220,
    1222,  1224,  1226,  1230,  1234,  1238,  1242,  1246,  1250,  1256,
    1262,  1268,  1274,  1280,  1286,  1289,  1290,  1295,  1298,  1302,
    1306,  1309,  1312,  1316,  1320,  1323,  1327,  1334,  1336,  1339,
    1344,  1348,  1351,  1354,  1357,  1360,  1364,  1366,  1370,  1374,
    1378,  1381,  1384,  1389,  1394,  1400,  1402,  1404,  1406,  1408,
    1410,  1412,  1414,  1416,  1418,  1420,  1422,  1424,  1426,  1428,
    1430,  1432,  1434,  1436,  1438,  1440,  1442,  1444,  1446,  1448,
    1450,  1453,  1456,  1459,  1461,  1463,  1465,  1466,  1469,  1472,
    1473,  1475,  1478,  1481,  1484,  1487,  1490,  1495,  1499,  1502,
    1505,  1507,  1511,  1519,  1521,  1522,  1524,  1525,  1528,  1531,
    1534,  1537,  1540,  1543,  1546,  1549,  1552,  1556,  1559,  1562,
    1565,  1567,  1569,  1571,  1573,  1578,  1584,  1586,  1590,  1592,
    1596,  1598,  1601,  1603,  1605,  1607,  1609,  1611,  1613,  1615,
    1617,  1619,  1621,  1623,  1628,  1633,  1640,  1646,  1650,  1651,
    1654,  1657,  1658,  1662,  1663,  1668,  1670,  1676,  1681,  1683,
    1688,  1690,  1693,  1696,  1698,  1700,  1702,  1705,  1708,  1711,
    1714,  1715,  1719,  1725,  1727,  1729,  1732,  1736,  1738,  1741,
    1745,  1746,  1748,  1750,  1752,  1758,  1764,  1771,  1773,  1775,
    1777,  1780,  1785,  1787,  1788,  1794,  1796,  1797,  1801,  1804,
    1806,  1807,  1810,  1811,  1813,  1814,  1818,  1820,  1821,  1823,
    1826,  1828,  1830,  1832,  1834,  1837,  1840,  1842,  1845,  1849,
    1851,  1853,  1857,  1861,  1863,  1866,  1870,  1872,  1875,  1876,
    1879,  1881,  1883,  1884,  1886,  1888,  1890,  1894,  1897,  1901,
    1902,  1904,  1908,  1913,  1918,  1920,  1924,  1929,  1933,  1934,
    1936,  1940,  1944,  1946,  1948,  1949,  1951,  1953,  1958,  1959,
    1963,  1964,  1967,  1968,  1971,  1972,  1982,  1993,  1994,  1995,
    1996,  1997,  2001,  2004,  2006,  2007,  2011,  2017,  2021,  2024,
    2030,  2033,  2035,  2037,  2039,  2041,  2043,  2047,  2049,  2052,
    2056,  2059,  2061,  2065,  2067,  2069,  2076,  2080,  2084,  2089,
    2092,  2096,  2097,  2099,  2103,  2105,  2107,  2109,  2112,  2114,
    2116,  2118,  2121,  2123,  2126,  2128,  2131,  2132,  2136,  2137,
    2139,  2143,  2145,  2148,  2149,  2152,  2153,  2156,  2157,  2162,
    2164,  2167,  2169,  2171,  2172,  2174,  2178,  2181,  2183,  2185,
    2188,  2190,  2195,  2198,  2200,  2204,  2213,  2219,  2221,  2225,
    2227,  2229,  2234,  2239,  2244,  2246,  2248,  2254,  2260,  2262,
    2266,  2270,  2272,  2274,  2282,  2290,  2296,  2300,  2301,  2303,
    2305,  2307,  2310,  2311,  2313,  2315,  2317,  2318,  2322,  2324,
    2328,  2330,  2331,  2335,  2337,  2341,  2343,  2347,  2351,  2353,
    2355,  2359,  2361,  2364,  2366,  2370,  2374,  2376,  2379,  2383,
    2387,  2391,  2395,  2397,  2401,  2404,  2406,  2408,  2410,  2412,
    2414,  2416,  2418,  2420,  2422,  2424,  2428,  2432,  2436,  2440,
    2444,  2448,  2452,  2456,  2463,  2470,  2477,  2484,  2491,  2498,
    2505,  2512,  2519,  2526,  2533,  2540,  2547,  2554,  2561,  2568,
    2570,  2572,  2578,  2585,  2589,  2594,  2600,  2607,  2611,  2616,
    2620,  2625,  2629,  2634,  2639,  2645,  2650,  2655,  2659,  2664,
    2666,  2668,  2670,  2672,  2674,  2676,  2678,  2682,  2686,  2697,
    2708,  2710,  2712,  2714,  2716,  2718,  2720,  2722,  2724,  2726,
    2729,  2732,  2736,  2740,  2744,  2748,  2752,  2756,  2760,  2764,
    2766,  2768,  2770,  2772,  2776,  2778,  2780,  2782,  2784,  2786,
    2791,  2793,  2797,  2799,  2802,  2804,  2806,  2808,  2810,  2812,
    2814,  2816,  2819,  2822,  2825,  2827,  2829,  2831,  2833,  2835,
    2837,  2839,  2841,  2843,  2845,  2848,  2850,  2853,  2855,  2857,
    2859,  2861,  2863,  2866,  2868,  2870,  2872,  2874,  2876,  2881,
    2887,  2893,  2899,  2905,  2911,  2917,  2923,  2929,  2935,  2941,
    2948,  2955,  2962,  2971,  2976,  2981,  2985,  2992,  2994,  2996,
    3003,  3010,  3012,  3014,  3019,  3026,  3031,  3037,  3041,  3047,
    3052,  3058,  3060,  3062,  3064,  3066,  3068,  3070,  3072,  3074,
    3076,  3078,  3080,  3082,  3083,  3085,  3087,  3089,  3091,  3093,
    3095,  3097,  3099,  3101,  3103,  3105,  3107,  3109,  3111,  3113,
    3115,  3117,  3119,  3121,  3123,  3125,  3127,  3129,  3131,  3133,
    3135,  3137,  3139,  3141,  3143,  3145,  3147,  3149,  3151,  3153,
    3155,  3157,  3159,  3161,  3163,  3165
};
static const short yyrhs[] =
{
     281,     0,   281,   271,     0,   457,     0,   594,     0,   511,
       0,   311,     0,   301,     0,   584,     0,   479,     0,   282,
       0,   582,     0,   434,     0,   313,     0,   315,     0,   291,
       0,   512,     0,   505,     0,   534,     0,   503,     0,   588,
       0,    43,   641,     0,    73,   284,   129,   283,   182,   295,
     288,     0,    73,   286,   129,   148,   290,   182,   295,   288,
       0,    73,   284,   129,   283,   182,   293,     0,    73,   286,
     129,   148,   290,   182,   293,     0,    73,   297,   182,   299,
     289,     0,   567,     0,   180,   567,     0,     6,     0,     6,
     147,     0,   285,     0,   287,     0,   285,   272,   287,     0,
      59,     0,   163,     0,    84,     0,    47,     0,   188,   599,
       0,   153,   599,     0,   200,    73,   131,     0,     0,   200,
     210,   131,     0,     0,   682,     0,   158,   292,   284,   129,
     283,    66,   295,     0,   158,   292,   286,   129,   148,   290,
      66,   295,     0,   158,   284,   129,   283,    66,   295,     0,
     158,   286,   129,   148,   290,    66,   295,     0,   158,   284,
     129,   283,    66,   293,     0,   158,   286,   129,   148,   290,
      66,   293,     0,   158,   297,    66,   299,     0,    73,   131,
      64,     0,   294,     0,   293,   272,   294,     0,   293,   272,
     296,     0,   295,   272,   294,     0,   148,   682,     0,   184,
     686,     0,   195,   689,     0,   214,   683,     0,   296,     0,
     295,   272,   296,     0,   687,     0,   189,   687,     0,    74,
     687,     0,   298,     0,   297,   272,   298,     0,   683,     0,
     300,     0,   299,   272,   300,     0,   687,     0,   189,   687,
       0,    45,   302,     0,    63,   310,     0,    62,    68,   303,
       0,   668,   305,   157,   308,    55,   640,   117,   640,     0,
     486,     0,    19,     0,    37,   273,   644,   274,   492,     0,
       0,   306,     0,   273,   306,   274,     0,   307,     0,   306,
     272,   307,     0,   374,   304,     0,   374,   304,    20,   238,
       0,   309,     0,   273,   309,   274,     0,   374,   304,     0,
     374,   304,   212,     0,   374,   304,    20,   101,     0,   374,
     304,    20,   238,     0,   140,   644,     0,   677,    82,   491,
     135,   491,    55,   640,   117,   640,     0,    36,   312,     0,
      58,   676,   640,     0,   317,   539,    83,   680,   129,   567,
     318,     0,   148,   393,     0,   180,   361,     0,   184,   449,
       0,   195,   435,     0,    71,   334,     0,    40,   336,     0,
      51,   325,     0,   165,   319,     0,   214,   335,     0,   237,
     314,     0,   148,   394,     0,   180,   362,     0,   195,   436,
       0,    36,   132,     7,   316,     0,   148,   395,     0,   184,
     450,     0,   187,     0,     0,   601,     0,   600,     0,   368,
     273,   446,   629,   447,   274,     0,   644,   320,   321,   640,
     322,   323,     0,   111,     0,    14,     0,     0,     0,    32,
       0,     0,   102,   337,   647,   360,     0,     0,   324,     0,
     349,     0,   324,   349,     0,   371,   326,   481,   446,   327,
     447,   328,   370,     0,    10,     0,     0,    46,   446,   376,
       0,     0,     0,   329,     0,   330,     0,   329,   330,     0,
     331,     0,   332,     0,   333,     0,   124,    98,     0,   446,
      24,   273,   608,   274,   447,     0,   679,     0,   683,     0,
     338,   339,   342,     0,     0,   264,     0,   640,     0,     0,
     340,     0,   341,     0,   340,   341,     0,   139,   337,   644,
       0,   102,   337,   647,   360,     0,   189,   640,     0,   141,
     640,     0,   164,   118,   640,     0,     0,   343,     0,   344,
       0,   343,   344,     0,   349,     0,   346,     0,   345,     0,
      46,    23,   164,   670,     0,   263,    94,   640,     0,    75,
     337,   647,     0,    25,   337,   647,     0,   127,   337,   644,
       0,   109,   337,   645,     0,   348,     0,   347,     0,   103,
     273,   350,   274,   136,   351,     0,   103,    15,   351,     0,
     103,     0,   355,   640,   356,     0,   351,     0,   350,   272,
     351,     0,   352,   353,     0,   640,     0,     0,   353,   354,
       0,   168,   337,   647,     0,    94,     0,     0,   357,     0,
     358,     0,   357,   358,     0,   175,   359,   647,     0,   102,
     337,   647,   360,     0,     0,    12,     0,    12,   137,     0,
       0,   137,     0,   138,     0,   567,   363,   273,   364,   274,
       0,   567,   363,   273,   364,   274,     0,    62,    94,   640,
       0,    62,   640,     0,     0,   365,     0,   364,   272,   365,
       0,   366,     0,   381,     0,   371,   369,   375,   447,   377,
     370,     0,   371,   482,   367,     0,   371,   367,     0,   368,
     273,   446,   629,   447,   274,     0,    30,    20,     0,    30,
       0,   481,   446,     0,   606,   444,     0,    26,   671,     0,
       0,   606,     0,   606,     0,   374,   481,     0,     0,    46,
     376,     0,     0,   633,     0,   637,     0,   638,     0,   639,
       0,   667,     0,   630,     0,     0,   378,     0,   379,     0,
     378,   379,     0,   382,   380,     0,   124,    98,     0,   153,
     567,   599,   389,   387,     0,   388,     0,   187,   387,     0,
     146,    90,   387,     0,   382,   383,     0,    33,   673,     0,
       0,   384,     0,   385,     0,   386,     0,   388,     0,   187,
     600,   387,     0,   146,    90,   600,   387,     0,    65,    90,
     600,   153,   567,   599,   389,   387,     0,   247,   539,    83,
     680,     0,     0,   446,    24,   273,   608,   274,   447,     0,
     390,     0,   391,     0,   391,   390,     0,   390,   391,     0,
       0,   129,   188,   392,     0,   129,    47,   392,     0,   211,
       0,   164,    46,     0,   164,    98,     0,   123,   209,     0,
     682,   397,   398,    10,   444,   401,   407,   447,     0,   682,
     397,   398,    10,   444,   401,   407,   447,     0,   682,   397,
     398,    10,   444,   401,   407,   447,     0,   682,   397,   398,
      10,   444,   401,   407,   447,     0,   273,   399,   274,     0,
       0,   157,   397,     0,     0,   400,     0,   399,   272,   400,
       0,   372,   482,     0,   402,     0,     0,   403,     0,   402,
     403,     0,    45,   404,   371,   482,   405,   271,     0,   192,
       0,     0,   264,   376,     0,   375,     0,   410,     0,   407,
       0,    17,   408,    54,     0,   409,     0,   409,   430,     0,
       0,   406,     0,   409,   406,     0,   592,   271,     0,   584,
     271,     0,   411,     0,   412,     0,   413,     0,   414,     0,
     416,     0,   419,     0,   582,   271,     0,   420,     0,   422,
       0,   588,   271,     0,   427,     0,   417,     0,   418,     0,
     178,   271,     0,    61,   271,     0,   428,     0,    58,   676,
     271,     0,    58,   676,   629,   271,     0,    58,   271,     0,
      59,   148,   682,   424,   425,   271,     0,    59,   415,   629,
     271,     0,   191,     0,   254,     0,    64,   534,    86,   426,
     429,    50,   406,     0,    64,    59,   415,   629,    86,   426,
      50,   406,     0,    59,   415,   629,    86,   426,   271,     0,
      78,   273,   608,   274,   181,   406,    53,   406,     0,    78,
     273,   608,   274,   181,   406,     0,   144,   629,   421,   271,
       0,     0,   534,    86,   426,   271,     0,   275,   688,     0,
     564,     0,   273,   564,   274,     0,     0,   156,   426,     0,
     156,   273,   426,   274,     0,     0,   423,     0,   605,     0,
     426,   272,   605,     0,   426,   272,   423,     0,   199,   273,
     608,   274,    50,   406,     0,   235,   271,     0,   255,   271,
       0,    10,    39,   674,     0,     0,   431,     0,   430,   431,
       0,   197,   432,    50,   406,     0,   433,     0,   432,   272,
     433,     0,   173,   641,     0,    69,   678,     0,    58,   676,
       0,     9,     0,    59,   148,   682,   424,     0,   689,   599,
      10,   444,   437,   448,   445,     0,   689,   599,    10,   444,
     437,   448,   445,     0,   438,     0,   439,     0,   438,   186,
     439,     0,   438,   186,     6,   439,     0,   163,   554,   555,
     440,   573,   569,   572,     0,    66,   441,     0,   442,     0,
     441,   272,   442,     0,   443,     0,   566,     0,   442,   568,
      89,   442,   129,   608,     0,   273,   443,   274,     0,     0,
       0,     0,     0,   200,    24,   131,     0,     0,   686,    64,
     567,   451,   452,   455,   446,   456,   447,     0,   686,    64,
     567,   451,   452,   455,   446,   456,   447,     0,     3,     0,
      80,     0,     0,   453,   454,     0,    16,     0,     5,     0,
      84,     0,   188,     0,    47,     0,    84,   132,   188,     0,
      84,   132,    47,     0,   188,   132,    84,     0,   188,   132,
      47,     0,    47,   132,    84,     0,    47,   132,   188,     0,
      84,   132,   188,   132,    47,     0,    84,   132,    47,   132,
     188,     0,   188,   132,    84,   132,    47,     0,   188,   132,
      47,   132,    84,     0,    47,   132,    84,   132,   188,     0,
      47,   132,   188,   132,    84,     0,   143,   642,     0,     0,
      10,   446,   401,   407,     0,     7,   458,     0,    58,   676,
     640,     0,   180,   567,   463,     0,   184,   476,     0,   148,
     396,     0,    40,   472,   473,     0,    51,   465,   461,     0,
      83,   471,     0,    46,   446,   376,     0,    24,   446,   273,
     608,   274,   447,     0,   462,     0,   461,   462,     0,   164,
     446,   459,   447,     0,     4,    33,   460,     0,     4,   333,
       0,    52,    46,     0,    52,    33,     0,   182,   606,     0,
     216,   374,   482,     0,   464,     0,   463,   272,   464,     0,
      52,   606,   470,     0,    52,    33,   673,     0,     4,   366,
       0,     4,   381,     0,   467,   606,   143,   644,     0,   467,
     465,   182,   606,     0,   467,   469,   216,   468,   447,     0,
     466,     0,   691,     0,   215,     0,   216,     0,   217,     0,
     218,     0,   219,     0,   220,     0,   221,     0,   222,     0,
     223,     0,   224,     0,   225,     0,   226,     0,   227,     0,
     228,     0,   229,     0,   230,     0,   233,     0,   234,     0,
     241,     0,   242,     0,   250,     0,   252,     0,     7,     0,
       7,   215,     0,   482,   446,     0,   606,   444,     0,   606,
       0,   213,     0,   211,     0,     0,   680,     3,     0,   680,
      80,     0,     0,   474,     0,   473,   474,     0,     4,   324,
       0,    52,   103,     0,   164,   475,     0,     4,   346,     0,
       4,   263,    94,   640,     0,    52,   263,    94,     0,    17,
     262,     0,    54,   262,     0,   345,     0,   475,   272,   345,
       0,   686,   451,   477,   455,   446,   478,   447,     0,   452,
       0,     0,   456,     0,     0,    52,   480,     0,    58,   676,
       0,    83,   680,     0,   148,   682,     0,   180,   685,     0,
     184,   686,     0,   195,   689,     0,    63,   677,     0,    51,
     675,     0,    62,    68,   668,     0,   165,   644,     0,   214,
     683,     0,    71,   679,     0,   482,     0,   483,     0,   486,
       0,   489,     0,   487,   276,   484,   277,     0,   494,   276,
     484,   277,   492,     0,   485,     0,   484,   272,   485,     0,
     646,     0,   646,   275,   646,     0,   487,     0,   494,   492,
       0,   493,     0,   498,     0,   501,     0,   243,     0,   488,
       0,   169,     0,    41,     0,   226,     0,   227,     0,    85,
       0,    96,     0,    19,   491,   490,   492,     0,    19,   273,
     645,   274,     0,    19,   273,   645,   272,   641,   274,     0,
      19,   273,   272,   641,   274,     0,   162,   168,   645,     0,
       0,   177,   641,     0,   177,   669,     0,     0,    23,   164,
     670,     0,     0,   497,   273,   644,   274,     0,   497,     0,
     497,   193,   273,   644,   274,     0,   496,   273,   644,   274,
       0,   496,     0,   495,   273,   644,   274,     0,   191,     0,
      23,   193,     0,    91,   193,     0,    23,     0,    91,     0,
     121,     0,   119,    23,     0,   119,    91,     0,    99,   499,
       0,   500,   499,     0,     0,   273,   646,   274,     0,   273,
     646,   272,   646,   274,     0,    44,     0,    92,     0,    95,
     502,     0,    97,    95,   502,     0,   152,     0,    93,   145,
       0,   273,   642,   274,     0,     0,   516,     0,   504,     0,
     533,     0,   164,    71,   679,   182,   646,     0,   164,    71,
     679,   182,   231,     0,   164,    71,   679,   182,   268,   231,
       0,   506,     0,   507,     0,   509,     0,   252,   690,     0,
     253,   252,   690,   508,     0,   130,     0,     0,   161,   513,
     182,   510,   690,     0,   252,     0,     0,    28,   513,   514,
       0,   161,   513,     0,   201,     0,     0,   155,   515,     0,
       0,   170,     0,     0,   164,   183,   517,     0,   518,     0,
       0,   519,     0,   518,   519,     0,   520,     0,   521,     0,
     522,     0,   526,     0,   151,   130,     0,   151,   202,     0,
     196,     0,   123,   196,     0,    88,   107,   523,     0,   523,
       0,   524,     0,   151,   185,   525,     0,   151,    29,   525,
       0,   170,     0,   170,   180,     0,   170,   180,   174,     0,
     194,     0,   123,   194,     0,     0,   154,   529,     0,   166,
       0,   149,     0,     0,   151,     0,   202,     0,   530,     0,
     529,   272,   530,     0,   532,   531,     0,    64,   527,   528,
       0,     0,   567,     0,   532,   272,   567,     0,   164,   176,
      83,   680,     0,   535,   536,   542,   544,     0,   545,     0,
     535,   186,   545,     0,   535,   186,     6,   545,     0,   133,
      20,   537,     0,     0,   538,     0,   537,   272,   538,     0,
     629,   539,   541,     0,    11,     0,    48,     0,     0,   239,
       0,   249,     0,   248,   549,   540,   550,     0,     0,    64,
     188,   543,     0,     0,   128,   601,     0,     0,   200,   251,
       0,     0,   163,   551,   554,   555,   558,   573,   569,   572,
     574,     0,   163,   551,   554,   555,   558,   573,   569,   572,
     574,   536,     0,     0,     0,     0,     0,   552,   553,   548,
       0,   552,   548,     0,   553,     0,     0,   239,   647,   547,
       0,   239,   273,   629,   274,   547,     0,   239,   636,   547,
       0,   240,   647,     0,   240,   273,   548,   629,   274,     0,
     240,   636,     0,    49,     0,   666,     0,   556,     0,   269,
       0,   557,     0,   556,   272,   557,     0,   593,     0,   593,
     681,     0,   593,    10,   681,     0,    66,   559,     0,   560,
       0,   559,   272,   560,     0,   561,     0,   562,     0,   560,
     568,    89,   560,   129,   608,     0,   273,   561,   274,     0,
     682,   563,   684,     0,   682,   563,    10,   684,     0,   682,
     563,     0,   273,   564,   274,     0,     0,   565,     0,   564,
     272,   565,     0,   667,     0,   629,     0,   567,     0,   685,
     684,     0,   685,     0,    81,     0,   105,     0,   105,   134,
       0,   159,     0,   159,   134,     0,    67,     0,    67,   134,
       0,     0,    74,    20,   570,     0,     0,   571,     0,   570,
     272,   571,     0,   629,     0,    77,   608,     0,     0,   198,
     608,     0,     0,   142,   575,     0,     0,   576,   273,   577,
     274,     0,    89,     0,   172,   114,     0,   114,     0,   172,
       0,     0,   578,     0,   578,   272,   577,     0,   579,   580,
       0,   575,     0,   685,     0,   685,   579,     0,   120,     0,
      83,   273,   581,   274,     0,   133,   680,     0,   680,     0,
     680,   272,   581,     0,    84,    86,   567,   602,   190,   273,
     583,   274,     0,    84,    86,   567,   602,   546,     0,   593,
       0,   583,   272,   593,     0,   585,     0,   586,     0,    47,
      66,   566,   573,     0,    47,    66,   566,   587,     0,   198,
      38,   128,   674,     0,   589,     0,   590,     0,   188,   566,
     164,   591,   573,     0,   188,   566,   164,   591,   587,     0,
     592,     0,   591,   272,   592,     0,   607,   264,   593,     0,
     629,     0,   667,     0,   151,    19,   606,    66,   567,   595,
     597,     0,    84,    19,   606,    86,   567,   595,   597,     0,
      63,    66,   596,   182,   596,     0,    63,   182,   596,     0,
       0,   491,     0,   636,     0,   641,     0,   113,   598,     0,
       0,   645,     0,   636,     0,   600,     0,     0,   273,   601,
     274,     0,   606,     0,   601,   272,   606,     0,   603,     0,
       0,   273,   604,   274,     0,   607,     0,   604,   272,   607,
       0,   606,     0,   684,   278,   672,     0,   684,   278,   269,
       0,   672,     0,   606,     0,   684,   278,   672,     0,   623,
       0,   124,   623,     0,   610,     0,   608,   132,   608,     0,
     608,     8,   608,     0,   610,     0,   124,   623,     0,   609,
     132,   608,     0,   609,     8,   608,     0,   624,   132,   608,
       0,   624,     8,   608,     0,   611,     0,   273,   609,   274,
       0,   124,   610,     0,   612,     0,   615,     0,   616,     0,
     617,     0,   622,     0,   613,     0,   620,     0,   618,     0,
     619,     0,   621,     0,   629,   264,   629,     0,   629,   265,
     629,     0,   629,   266,   629,     0,   629,    70,   629,     0,
     629,   106,   629,     0,   629,   125,   629,     0,   629,   126,
     629,     0,   629,   122,   629,     0,   629,   264,     6,   273,
     627,   274,     0,   629,   265,     6,   273,   627,   274,     0,
     629,   266,     6,   273,   627,   274,     0,   629,    70,     6,
     273,   627,   274,     0,   629,   106,     6,   273,   627,   274,
       0,   629,   125,     6,   273,   627,   274,     0,   629,   126,
       6,   273,   627,   274,     0,   629,   122,     6,   273,   627,
     274,     0,   629,   264,   614,   273,   627,   274,     0,   629,
     265,   614,   273,   627,   274,     0,   629,   266,   614,   273,
     627,   274,     0,   629,    70,   614,   273,   627,   274,     0,
     629,   106,   614,   273,   627,   274,     0,   629,   125,   614,
     273,   627,   274,     0,   629,   126,   614,   273,   627,   274,
       0,   629,   122,   614,   273,   627,   274,     0,   171,     0,
       9,     0,   629,    18,   629,     8,   629,     0,   629,   124,
      18,   629,     8,   629,     0,   629,   108,   629,     0,   629,
     124,   108,   629,     0,   629,   108,   629,    57,   629,     0,
     629,   124,   108,   629,    57,   629,     0,   629,    79,   625,
       0,   629,   124,    79,   625,     0,   629,    34,   629,     0,
     629,   124,    34,   629,     0,   629,   175,   629,     0,   629,
     124,   175,   629,     0,   629,   175,   200,   629,     0,   629,
     124,   175,   200,   629,     0,    60,   273,   546,   274,     0,
     167,   273,   546,   274,     0,   629,    87,    98,     0,   629,
      87,   124,    98,     0,   256,     0,   257,     0,   258,     0,
     259,     0,   260,     0,   261,     0,   626,     0,   273,   632,
     274,     0,   273,   627,   274,     0,   163,   551,   554,   629,
     558,   573,   569,   572,   574,   536,     0,   163,   551,   554,
     629,   558,   573,   569,   572,   574,   536,     0,   605,     0,
     631,     0,   648,     0,   635,     0,   636,     0,   423,     0,
     654,     0,   655,     0,   653,     0,   268,   629,     0,   267,
     629,     0,   629,   267,   629,     0,   629,    31,   629,     0,
     629,    26,   671,     0,   629,   268,   629,     0,   629,   269,
     629,     0,   629,   270,   629,     0,   273,   629,   274,     0,
     273,   628,   274,     0,   637,     0,   638,     0,   639,     0,
      42,     0,   684,   278,    42,     0,   101,     0,   630,     0,
     228,     0,   229,     0,   230,     0,   605,   276,   632,   277,
       0,   629,     0,   632,   272,   629,     0,   635,     0,   268,
     634,     0,   205,     0,   204,     0,   203,     0,   231,     0,
     232,     0,   634,     0,   640,     0,    41,   207,     0,   226,
     207,     0,   227,   207,     0,   279,     0,   189,     0,   233,
       0,   234,     0,   241,     0,   242,     0,    69,     0,   173,
       0,   250,     0,   207,     0,   208,   207,     0,   642,     0,
     268,   643,     0,   204,     0,   204,     0,   642,     0,   204,
       0,   647,     0,   268,   647,     0,   204,     0,   649,     0,
     650,     0,   651,     0,   652,     0,    35,   273,   269,   274,
       0,    35,   273,   666,   629,   274,     0,    35,   273,    49,
     629,   274,     0,   179,   273,   666,   629,   274,     0,   179,
     273,    49,   629,   274,     0,    13,   273,   666,   629,   274,
       0,    13,   273,    49,   629,   274,     0,   116,   273,   666,
     629,   274,     0,   116,   273,    49,   629,   274,     0,   112,
     273,   666,   629,   274,     0,   112,   273,    49,   629,   274,
       0,    72,   273,   679,   272,   629,   274,     0,   217,   273,
     665,    66,   629,   274,     0,   236,   273,   629,    66,   644,
     274,     0,   236,   273,   629,    66,   644,    64,   642,   274,
       0,   100,   273,   629,   274,     0,   668,   273,   632,   274,
       0,   668,   273,   274,     0,    22,   273,   593,    10,   373,
     274,     0,   656,     0,   657,     0,   245,   273,   629,   272,
     629,   274,     0,   246,   273,   565,   272,   564,   274,     0,
     658,     0,   660,     0,   244,   663,   659,    54,     0,   244,
     663,   659,    53,   664,    54,     0,   197,   662,   181,   664,
       0,   659,   197,   662,   181,   664,     0,   244,   661,    54,
       0,   244,   661,    53,   664,    54,     0,   197,   608,   181,
     664,     0,   661,   197,   608,   181,   664,     0,   629,     0,
     629,     0,   565,     0,   218,     0,   219,     0,   220,     0,
     221,     0,   222,     0,   223,     0,   224,     0,   225,     0,
       6,     0,     0,    98,     0,   206,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   206,     0,   692,     0,   235,     0,   238,     0,   236,
       0,   246,     0,   249,     0,   255,     0,   251,     0,   245,
       0,   248,     0,   254,     0,   247,     0,   256,     0,   257,
       0,   258,     0,   262,     0,   263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   531,   533,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   563,   567,   571,   575,   579,   584,   585,   589,
     591,   593,   597,   598,   602,   606,   608,   610,   612,   614,
     618,   620,   624,   626,   630,   637,   642,   647,   652,   657,
     662,   667,   673,   677,   678,   680,   682,   686,   688,   690,
     692,   696,   697,   704,   706,   708,   712,   713,   717,   721,
     722,   726,   728,   735,   739,   741,   746,   752,   753,   755,
     761,   763,   764,   768,   769,   775,   778,   784,   785,   788,
     791,   795,   799,   802,   807,   816,   820,   823,   826,   828,
     830,   832,   834,   836,   838,   840,   842,   847,   851,   853,
     855,   865,   869,   871,   882,   884,   888,   890,   891,   897,
     903,   905,   907,   911,   913,   917,   919,   923,   925,   928,
     929,   936,   954,   956,   960,   962,   966,   968,   971,   972,
     975,   980,   981,   984,   988,   997,  1005,  1014,  1019,  1020,
    1023,  1029,  1031,  1034,  1035,  1039,  1041,  1043,  1045,  1047,
    1051,  1053,  1056,  1057,  1061,  1063,  1064,  1065,  1067,  1071,
    1073,  1075,  1077,  1081,  1086,  1093,  1099,  1106,  1113,  1146,
    1147,  1150,  1156,  1160,  1161,  1164,  1172,  1176,  1177,  1180,
    1181,  1184,  1186,  1190,  1191,  1192,  1195,  1196,  1197,  1203,
    1208,  1213,  1215,  1217,  1221,  1222,  1226,  1227,  1234,  1238,
    1242,  1251,  1257,  1258,  1261,  1263,  1268,  1270,  1275,  1281,
    1287,  1291,  1297,  1299,  1303,  1308,  1309,  1310,  1312,  1314,
    1318,  1320,  1323,  1324,  1328,  1333,  1335,  1340,  1341,  1343,
    1351,  1355,  1357,  1360,  1361,  1362,  1363,  1366,  1370,  1374,
    1381,  1388,  1393,  1399,  1402,  1404,  1406,  1408,  1412,  1415,
    1419,  1422,  1425,  1428,  1437,  1449,  1460,  1471,  1482,  1484,
    1488,  1490,  1494,  1495,  1499,  1505,  1507,  1511,  1512,  1516,
    1521,  1523,  1527,  1529,  1533,  1534,  1537,  1541,  1543,  1545,
    1549,  1550,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1561,
    1562,  1563,  1564,  1565,  1566,  1567,  1568,  1569,  1571,  1576,
    1579,  1581,  1585,  1589,  1594,  1598,  1598,  1600,  1605,  1610,
    1615,  1617,  1621,  1625,  1631,  1636,  1641,  1643,  1645,  1649,
    1651,  1653,  1657,  1658,  1659,  1661,  1665,  1676,  1678,  1686,
    1688,  1691,  1692,  1696,  1701,  1702,  1706,  1708,  1710,  1712,
    1719,  1727,  1734,  1754,  1758,  1760,  1762,  1766,  1777,  1781,
    1782,  1786,  1787,  1790,  1793,  1799,  1803,  1809,  1813,  1819,
    1823,  1831,  1842,  1853,  1855,  1857,  1861,  1865,  1867,  1871,
    1873,  1875,  1877,  1879,  1881,  1883,  1885,  1887,  1889,  1891,
    1893,  1895,  1897,  1899,  1903,  1905,  1909,  1915,  1919,  1922,
    1929,  1931,  1933,  1936,  1939,  1944,  1948,  1954,  1955,  1959,
    1968,  1972,  1974,  1976,  1978,  1980,  1984,  1985,  1989,  1991,
    1993,  1995,  2001,  2004,  2006,  2010,  2018,  2019,  2020,  2021,
    2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,  2031,
    2032,  2033,  2034,  2035,  2036,  2037,  2038,  2039,  2040,  2043,
    2045,  2049,  2051,  2056,  2062,  2064,  2066,  2070,  2072,  2079,
    2085,  2086,  2090,  2098,  2100,  2102,  2104,  2106,  2108,  2110,
    2114,  2115,  2122,  2132,  2133,  2137,  2138,  2144,  2148,  2150,
    2152,  2154,  2156,  2158,  2160,  2162,  2164,  2166,  2168,  2170,
    2177,  2178,  2181,  2182,  2185,  2190,  2197,  2198,  2202,  2211,
    2217,  2218,  2221,  2222,  2223,  2224,  2241,  2246,  2251,  2270,
    2287,  2294,  2295,  2302,  2306,  2312,  2318,  2326,  2330,  2336,
    2340,  2344,  2350,  2354,  2361,  2367,  2373,  2381,  2386,  2391,
    2398,  2399,  2400,  2403,  2404,  2407,  2408,  2409,  2416,  2420,
    2431,  2437,  2491,  2553,  2554,  2561,  2574,  2579,  2584,  2591,
    2593,  2600,  2601,  2602,  2606,  2611,  2616,  2627,  2628,  2629,
    2632,  2636,  2640,  2642,  2646,  2650,  2651,  2654,  2658,  2662,
    2663,  2666,  2668,  2672,  2673,  2677,  2681,  2682,  2686,  2687,
    2691,  2692,  2693,  2694,  2697,  2699,  2703,  2705,  2709,  2711,
    2714,  2716,  2718,  2722,  2724,  2726,  2730,  2732,  2734,  2738,
    2742,  2744,  2746,  2750,  2752,  2756,  2757,  2761,  2765,  2767,
    2771,  2772,  2777,  2785,  2789,  2791,  2793,  2797,  2799,  2803,
    2804,  2808,  2812,  2814,  2816,  2820,  2822,  2826,  2828,  2832,
    2834,  2838,  2840,  2844,  2846,  2853,  2865,  2878,  2882,  2886,
    2890,  2894,  2896,  2898,  2900,  2904,  2906,  2908,  2912,  2914,
    2916,  2920,  2922,  2926,  2928,  2932,  2933,  2937,  2938,  2940,
    2947,  2953,  2954,  2958,  2959,  2962,  2964,  2968,  2971,  2974,
    2979,  2981,  2985,  2986,  2990,  2991,  2994,  2995,  3000,  3005,
    3007,  3009,  3011,  3013,  3015,  3017,  3019,  3026,  3028,  3032,
    3033,  3037,  3040,  3042,  3046,  3048,  3055,  3057,  3061,  3065,
    3067,  3069,  3075,  3077,  3081,  3082,  3086,  3088,  3091,  3092,
    3096,  3098,  3100,  3104,  3105,  3113,  3116,  3120,  3121,  3128,
    3129,  3132,  3136,  3140,  3147,  3148,  3151,  3156,  3161,  3162,
    3166,  3170,  3171,  3177,  3179,  3183,  3185,  3187,  3190,  3191,
    3194,  3198,  3200,  3203,  3205,  3211,  3212,  3216,  3220,  3221,
    3226,  3227,  3231,  3235,  3236,  3241,  3242,  3245,  3250,  3255,
    3258,  3265,  3266,  3268,  3269,  3271,  3275,  3276,  3278,  3280,
    3284,  3286,  3290,  3291,  3293,  3297,  3298,  3299,  3300,  3301,
    3302,  3303,  3304,  3305,  3306,  3311,  3313,  3315,  3317,  3319,
    3321,  3323,  3325,  3332,  3334,  3336,  3338,  3340,  3342,  3344,
    3346,  3348,  3350,  3352,  3354,  3356,  3358,  3360,  3362,  3366,
    3367,  3373,  3375,  3380,  3382,  3384,  3386,  3391,  3393,  3397,
    3399,  3403,  3405,  3407,  3409,  3413,  3417,  3421,  3423,  3427,
    3432,  3437,  3444,  3449,  3454,  3463,  3464,  3468,  3472,  3485,
    3502,  3503,  3504,  3505,  3506,  3507,  3508,  3509,  3510,  3511,
    3513,  3515,  3522,  3524,  3526,  3533,  3540,  3547,  3549,  3551,
    3552,  3553,  3554,  3556,  3558,  3562,  3566,  3582,  3598,  3602,
    3606,  3607,  3611,  3612,  3616,  3618,  3620,  3622,  3624,  3628,
    3629,  3631,  3647,  3663,  3667,  3671,  3673,  3677,  3681,  3684,
    3687,  3690,  3693,  3698,  3700,  3705,  3706,  3710,  3717,  3724,
    3731,  3738,  3739,  3743,  3749,  3750,  3751,  3752,  3755,  3757,
    3759,  3762,  3769,  3778,  3785,  3794,  3796,  3798,  3800,  3806,
    3815,  3819,  3826,  3830,  3834,  3836,  3840,  3846,  3847,  3850,
    3854,  3858,  3859,  3862,  3864,  3868,  3870,  3874,  3876,  3880,
    3882,  3886,  3889,  3892,  3895,  3897,  3899,  3901,  3903,  3905,
    3907,  3909,  3913,  3914,  3917,  3925,  3928,  3931,  3934,  3937,
    3940,  3943,  3946,  3949,  3952,  3955,  3958,  3961,  3964,  3972,
    3975,  3978,  3981,  3984,  3987,  3990,  3993,  3996,  4001,  4002,
    4007,  4009,  4010,  4011,  4012,  4013,  4014,  4015,  4016,  4017,
    4018,  4019,  4020,  4021,  4023,  4024
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
  "breakleave", "cursor_def", "excp_hndl_statements", 
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
  "from_clause", "from_list", "table_reference", "joined_table", 
  "table_proc", "proc_table_inputs", "null_or_value_list", 
  "null_or_value", "table_name", "simple_table_name", "join_type", 
  "group_clause", "group_by_list", "group_by_item", "having_clause", 
  "where_clause", "plan_clause", "plan_expression", "plan_type", 
  "plan_item_list", "plan_item", "table_or_alias_list", "access_type", 
  "index_list", "insert", "insert_value_list", "delete", 
  "delete_searched", "delete_positioned", "cursor_clause", "update", 
  "update_searched", "update_positioned", "assignments", "assignment", 
  "rhs", "blob", "filter_clause", "blob_subtype_value", "segment_clause", 
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
  "numeric_value_function", "string_value_function", "udf", 
  "cast_specification", "case_expression", "case_abbreviation", 
  "case_specification", "simple_case", "simple_when_clause", 
  "searched_case", "searched_when_clause", "when_operand", "case_operand", 
  "case_result", "timestamp_part", "all_noise", "null_value", 
  "symbol_UDF_name", "symbol_blob_subtype_name", 
  "symbol_character_set_name", "symbol_collation_name", 
  "symbol_column_name", "symbol_constraint_name", "symbol_cursor_name", 
  "symbol_domain_name", "symbol_exception_name", "symbol_filter_name", 
  "symbol_gdscode_name", "symbol_generator_name", "symbol_index_name", 
  "symbol_item_alias_name", "symbol_procedure_name", "symbol_role_name", 
  "symbol_table_alias_name", "symbol_table_name", "symbol_trigger_name", 
  "symbol_user_name", "symbol_variable_name", "symbol_view_name", 
  "symbol_savepoint_name", "valid_symbol_name", "non_reserved_word", 0
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
     429,   430,   430,   431,   432,   432,   433,   433,   433,   433,
     434,   435,   436,   437,   438,   438,   438,   439,   440,   441,
     441,   442,   442,   443,   443,   444,   445,   446,   447,   448,
     448,   449,   450,   451,   451,   451,   452,   453,   453,   454,
     454,   454,   454,   454,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   455,   455,   456,   457,   458,   458,
     458,   458,   458,   458,   458,   459,   460,   461,   461,   462,
     462,   462,   462,   462,   462,   462,   463,   463,   464,   464,
     464,   464,   464,   464,   464,   465,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   467,
     467,   468,   468,   469,   470,   470,   470,   471,   471,   472,
     473,   473,   474,   474,   474,   474,   474,   474,   474,   474,
     475,   475,   476,   477,   477,   478,   478,   479,   480,   480,
     480,   480,   480,   480,   480,   480,   480,   480,   480,   480,
     481,   481,   482,   482,   483,   483,   484,   484,   485,   485,
     486,   486,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   488,   488,   489,   489,   489,   489,   490,   490,   491,
     491,   491,   492,   492,   493,   493,   493,   494,   494,   494,
     495,   495,   495,   496,   496,   497,   497,   497,   498,   498,
     499,   499,   499,   500,   500,   501,   501,   501,   501,   502,
     502,   503,   503,   503,   504,   504,   504,   505,   505,   505,
     506,   507,   508,   508,   509,   510,   510,   511,   512,   513,
     513,   514,   514,   515,   515,   516,   517,   517,   518,   518,
     519,   519,   519,   519,   520,   520,   521,   521,   522,   522,
     523,   523,   523,   524,   524,   524,   525,   525,   525,   526,
     527,   527,   527,   528,   528,   529,   529,   530,   531,   531,
     532,   532,   533,   534,   535,   535,   535,   536,   536,   537,
     537,   538,   539,   539,   539,   540,   540,   541,   541,   542,
     542,   543,   543,   544,   544,   545,   546,   547,   548,   549,
     550,   551,   551,   551,   551,   552,   552,   552,   553,   553,
     553,   554,   554,   555,   555,   556,   556,   557,   557,   557,
     558,   559,   559,   560,   560,   561,   561,   562,   562,   562,
     563,   563,   564,   564,   565,   565,   566,   566,   567,   568,
     568,   568,   568,   568,   568,   568,   568,   569,   569,   570,
     570,   571,   572,   572,   573,   573,   574,   574,   575,   576,
     576,   576,   576,   576,   577,   577,   578,   578,   579,   579,
     580,   580,   580,   581,   581,   582,   582,   583,   583,   584,
     584,   585,   586,   587,   588,   588,   589,   590,   591,   591,
     592,   593,   593,   594,   594,   595,   595,   595,   596,   596,
     491,   597,   597,   598,   598,   599,   599,   600,   601,   601,
     602,   602,   603,   604,   604,   605,   605,   605,   606,   607,
     607,   608,   608,   608,   608,   608,   609,   609,   609,   609,
     609,   609,   610,   610,   610,   611,   611,   611,   611,   611,
     611,   611,   611,   611,   611,   612,   612,   612,   612,   612,
     612,   612,   612,   613,   613,   613,   613,   613,   613,   613,
     613,   613,   613,   613,   613,   613,   613,   613,   613,   614,
     614,   615,   615,   616,   616,   616,   616,   617,   617,   618,
     618,   619,   619,   619,   619,   620,   621,   622,   622,   623,
     623,   623,   624,   624,   624,   625,   625,   626,   627,   628,
     629,   629,   629,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   629,   629,   629,   630,   630,   630,   631,
     632,   632,   633,   633,   634,   634,   634,   634,   634,   635,
     635,   635,   635,   635,   636,   637,   637,   638,   639,   639,
     639,   639,   639,   640,   640,   641,   641,   642,   643,   644,
     645,   646,   646,   647,   648,   648,   648,   648,   649,   649,
     649,   649,   649,   649,   649,   649,   649,   649,   649,   650,
     651,   652,   652,   652,   653,   653,   654,   655,   655,   656,
     656,   657,   657,   658,   658,   659,   659,   660,   660,   661,
     661,   662,   663,   664,   665,   665,   665,   665,   665,   665,
     665,   665,   666,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   691,
     692,   692,   692,   692,   692,   692,   692,   692,   692,   692,
     692,   692,   692,   692,   692,   692
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
       3,     4,     2,     6,     4,     1,     1,     7,     8,     6,
       8,     6,     4,     0,     4,     2,     1,     3,     0,     2,
       4,     0,     1,     1,     3,     3,     6,     2,     2,     3,
       0,     1,     2,     4,     1,     3,     2,     2,     2,     1,
       4,     7,     7,     1,     1,     3,     4,     7,     2,     1,
       3,     1,     1,     6,     3,     0,     0,     0,     0,     3,
       0,     9,     9,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     5,     5,
       5,     5,     5,     5,     2,     0,     4,     2,     3,     3,
       2,     2,     3,     3,     2,     3,     6,     1,     2,     4,
       3,     2,     2,     2,     2,     3,     1,     3,     3,     3,
       2,     2,     4,     4,     5,     1,     1,     1,     1,     1,
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
       1,     3,     4,     4,     1,     3,     4,     3,     0,     1,
       3,     3,     1,     1,     0,     1,     1,     4,     0,     3,
       0,     2,     0,     2,     0,     9,    10,     0,     0,     0,
       0,     3,     2,     1,     0,     3,     5,     3,     2,     5,
       2,     1,     1,     1,     1,     1,     3,     1,     2,     3,
       2,     1,     3,     1,     1,     6,     3,     3,     4,     2,
       3,     0,     1,     3,     1,     1,     1,     2,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     3,     0,     1,
       3,     1,     2,     0,     2,     0,     2,     0,     4,     1,
       2,     1,     1,     0,     1,     3,     2,     1,     1,     2,
       1,     4,     2,     1,     3,     8,     5,     1,     3,     1,
       1,     4,     4,     4,     1,     1,     5,     5,     1,     3,
       3,     1,     1,     7,     7,     5,     3,     0,     1,     1,
       1,     2,     0,     1,     1,     1,     0,     3,     1,     3,
       1,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     2,     1,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     1,
       1,     5,     6,     3,     4,     5,     6,     3,     4,     3,
       4,     3,     4,     4,     5,     4,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,    10,    10,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     4,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     6,
       6,     6,     8,     4,     4,     3,     6,     1,     1,     6,
       6,     1,     1,     4,     6,     4,     5,     3,     5,     4,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
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
       0,     0,   570,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,   644,     0,     0,     0,     0,     0,
       1,    10,    15,     7,     6,    13,    14,    12,     3,     9,
      19,   552,    17,   557,   558,   559,     5,    16,   551,   553,
      18,   618,   614,    11,     8,   719,   720,    20,   724,   725,
       4,   459,     0,     0,     0,     0,     0,     0,   397,   569,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,    95,   624,   897,     0,    21,   895,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   477,     0,    29,    37,    34,
      36,   746,    35,   746,   978,   980,   982,   981,   987,   983,
     990,   988,   984,   986,   989,   985,   991,   992,   993,   994,
     995,     0,    31,     0,    32,     0,    66,    68,   970,   979,
       0,     0,     0,     0,     0,     0,     0,     0,   568,     0,
       0,   953,   638,   643,     0,     0,   577,     0,   676,   678,
     972,     0,     0,     0,   107,   560,   977,     0,     2,     0,
       0,   630,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,     0,   425,   426,     0,   963,
     404,     0,   967,   401,   269,   969,     0,   678,   400,   375,
     973,   574,   567,   893,     0,   103,   151,   150,   104,   133,
     218,   758,   959,     0,   102,   145,   966,     0,    98,   269,
     105,   899,   122,    99,   203,   100,     0,   101,   746,   976,
     106,   146,   622,   623,     0,   898,   896,     0,    74,     0,
     964,   695,   485,   962,   478,     0,   484,   489,   479,   480,
     487,   481,   482,   483,   488,   328,    30,     0,    39,   745,
      38,     0,     0,     0,     0,     0,     0,   751,     0,     0,
       0,     0,     0,     0,     0,   566,   903,     0,   884,   637,
     637,   638,   650,   648,   952,   651,     0,   652,   642,   638,
       0,     0,     0,     0,     0,     0,   593,   586,   575,   576,
     578,   580,   581,   582,   589,   590,   583,     0,   677,   971,
     108,   269,   109,   203,   110,   746,   563,     0,     0,   615,
       0,   634,     0,     0,     0,     0,     0,   402,   460,   367,
       0,   367,     0,   221,   403,   407,   398,   457,   458,     0,
     271,   242,   449,     0,   399,   416,     0,   373,   374,   474,
     573,   571,   894,   148,   148,     0,     0,     0,   160,   152,
     153,   132,     0,    96,     0,     0,   111,   271,   121,   120,
     123,     0,     0,     0,     0,     0,   955,    75,   221,   521,
       0,   721,   722,   486,     0,     0,     0,     0,   862,   890,
       0,   954,     0,   864,     0,     0,   891,     0,   885,   876,
     875,   874,   978,     0,     0,     0,   866,   867,   868,   877,
     878,   886,   887,   982,   888,   889,     0,   987,   983,   892,
       0,     0,     0,     0,   845,   350,   326,   672,   840,   755,
     675,   865,   841,   879,   843,   844,   859,   860,   861,   880,
     842,   904,   905,   906,   907,   848,   846,   847,   927,   928,
     931,   932,   674,     0,     0,   959,     0,   748,     0,     0,
      27,    33,     0,     0,    43,    69,    71,   974,    67,     0,
       0,     0,   750,     0,    52,     0,     0,     0,     0,    51,
     565,     0,     0,     0,   647,   645,     0,   654,     0,   653,
     655,   657,   731,   732,   641,     0,   612,     0,   587,   598,
     584,   598,   585,   599,   605,   609,   610,   594,   579,   695,
     728,   759,     0,     0,   271,     0,     0,   562,   561,   617,
     619,   624,   616,   632,     0,   613,   186,   177,     0,   462,
     465,   174,   173,   129,     0,   468,   463,     0,   469,   148,
     148,   148,   148,   470,   464,   461,     0,   411,     0,   413,
     412,     0,   414,     0,   408,     0,     0,   272,   219,   269,
       0,     0,   420,     0,   421,   367,   450,     0,   456,     0,
       0,     0,   453,   959,   378,   377,   473,     0,   395,   149,
       0,     0,   158,     0,   157,     0,     0,   147,   161,   162,
     166,   165,   164,   154,   521,   533,   508,   543,   511,   534,
     544,     0,   550,   512,     0,   540,     0,   535,   547,   507,
     530,   509,   510,   505,   367,   490,   491,   492,   500,   506,
     493,   502,   523,     0,   528,   525,   503,   540,   504,   112,
     269,   113,     0,     0,   124,     0,     0,   202,   242,   375,
     365,     0,   221,     0,    81,    83,     0,     0,     0,   740,
       0,     0,     0,     0,   991,   992,   993,     0,   694,   763,
     772,   775,   780,   776,   777,   778,   782,   783,   781,   784,
     779,   761,     0,   953,     0,   953,   881,     0,     0,   953,
     953,   953,     0,   882,   883,     0,     0,   942,     0,     0,
       0,     0,   850,   849,   644,     0,     0,   675,   325,   975,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   747,    28,     0,     0,    44,    72,     0,     0,    26,
     737,     0,   753,   644,     0,   716,   737,     0,     0,     0,
       0,   564,     0,   637,     0,     0,   695,     0,     0,   658,
     968,   555,     0,   554,   901,     0,   588,     0,   596,   592,
     591,     0,   602,     0,   607,   595,     0,   726,   727,     0,
       0,     0,   242,   365,     0,   628,     0,   629,   633,     0,
       0,     0,   130,   187,   467,     0,     0,     0,     0,     0,
     367,   410,     0,   367,   368,   415,   500,   523,   274,     0,
     268,   270,   365,   241,   960,   213,   210,     0,   223,   367,
     490,   365,     0,     0,     0,   240,   243,   244,   245,   246,
       0,   419,   455,   454,   418,   417,     0,     0,     0,   381,
     379,   380,   376,     0,   367,   196,   155,   159,     0,     0,
     163,     0,   518,   531,   532,   548,     0,   545,   550,     0,
     538,   536,   537,   135,     0,     0,     0,   501,     0,     0,
       0,     0,   539,   271,     0,   365,   125,   201,     0,   204,
     206,   207,     0,     0,     0,     0,   221,   221,    78,     0,
      85,    77,   519,   520,   956,   521,     0,     0,     0,   774,
     762,     0,     0,   832,   833,   834,     0,   766,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   944,   945,   946,   947,   948,   949,   950,   951,     0,
       0,     0,     0,   937,     0,     0,     0,     0,     0,   953,
     327,   858,   857,   673,   870,     0,   853,   958,   852,   851,
     854,   855,   856,   925,     0,   863,   757,   756,   749,     0,
       0,     0,     0,     0,     0,    24,    53,    41,    61,    63,
       0,     0,    70,     0,   742,     0,   752,   953,     0,   742,
      49,    47,     0,     0,     0,   646,   649,     0,   660,   661,
     663,   664,   671,     0,   688,   656,   659,   556,   902,   597,
     606,   601,   600,     0,   611,   729,   730,   760,   365,     0,
       0,   620,   639,   621,   631,   176,   183,   182,     0,   179,
     466,   148,   193,   178,   188,   189,   170,   169,   900,   172,
     171,   471,     0,     0,     0,   409,   273,   276,   212,   367,
       0,   368,   214,   209,   215,     0,     0,   251,     0,   423,
     368,   367,   365,   422,     0,     0,     0,   394,   476,   197,
     198,   156,     0,   168,     0,     0,     0,   523,     0,   546,
       0,     0,   367,   368,     0,   496,   498,     0,     0,     0,
       0,     0,     0,     0,   375,   276,   148,   127,   242,   199,
     395,   953,   370,   353,   354,     0,    82,     0,   221,     0,
      87,     0,    84,     0,     0,     0,   723,   961,     0,     0,
     767,     0,     0,   773,     0,     0,   765,   764,     0,   819,
       0,   810,   809,     0,   788,     0,   817,   835,   827,     0,
       0,     0,   789,   813,     0,     0,   792,     0,     0,     0,
       0,     0,     0,     0,   790,     0,     0,   791,     0,   821,
       0,     0,   785,     0,     0,   786,     0,     0,   787,     0,
       0,   221,     0,   908,     0,     0,   923,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   943,     0,     0,   941,
       0,     0,   933,     0,     0,     0,     0,     0,   869,   924,
      65,    57,    58,    64,    59,    60,     0,     0,     0,    22,
      25,    41,    42,   521,   521,     0,   734,   754,     0,     0,
     717,   733,    50,    48,    45,     0,   686,   663,     0,   684,
     679,   680,   682,     0,     0,   669,     0,   693,   603,   604,
     608,   276,   200,   370,     0,   181,     0,     0,     0,   194,
       0,   190,     0,     0,     0,   405,   229,   224,   872,   225,
     226,   227,   228,   281,     0,   275,   277,     0,   222,   230,
       0,   251,   624,   247,     0,   424,   451,   452,   386,   387,
     383,   382,   385,   384,   367,   475,   368,   167,   957,     0,
       0,   514,     0,   513,   549,     0,   541,     0,   136,     0,
     494,     0,   522,   523,   529,   527,     0,   524,   365,     0,
       0,     0,   119,   128,   205,   367,     0,     0,   366,     0,
      97,     0,   117,   116,    93,     0,     0,    89,     0,    86,
       0,   825,   826,   769,   768,   771,   770,     0,     0,     0,
     644,     0,     0,   828,     0,     0,     0,     0,     0,     0,
     820,   818,   814,     0,   822,     0,     0,     0,     0,   823,
       0,     0,     0,     0,     0,     0,   914,   913,     0,     0,
     910,   909,     0,   918,   917,   916,   915,   912,   911,     0,
       0,   939,   938,     0,     0,     0,     0,     0,     0,     0,
     871,    54,    55,     0,    56,    62,    23,   738,     0,   739,
     736,   741,   744,   743,     0,     0,   715,     0,    46,   666,
     662,   685,   681,   683,     0,     0,     0,   667,     0,     0,
     697,     0,   366,   625,   626,   640,   148,   184,   180,     0,
     196,   195,   191,     0,   368,   873,   280,     0,   289,   368,
     278,   368,   217,   231,   232,   367,     0,   248,     0,     0,
       0,     0,     0,     0,     0,     0,   276,   472,   516,     0,
     517,     0,   134,     0,   217,   137,   138,   140,   141,   142,
     497,   499,   495,   526,   276,   395,   368,   196,     0,     0,
       0,   351,     0,   355,   367,    88,     0,     0,    90,   523,
       0,   811,     0,     0,   953,   837,   836,     0,     0,   815,
       0,     0,     0,     0,   824,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   926,   220,   919,   920,     0,
     921,   940,   935,   934,     0,   929,   930,   695,    40,   521,
     695,   718,   686,   670,   668,   687,   689,   691,   692,   703,
     635,   368,   352,   627,     0,   175,   192,   368,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   980,
     985,   290,   285,     0,   287,   284,   294,   295,   296,   297,
     298,   305,   306,   299,   301,   302,   304,   309,     0,     0,
       0,     0,     0,   267,     0,     0,   208,   233,     0,     0,
       0,   251,   234,   237,   746,     0,   368,   392,   393,   389,
     388,   391,   390,     0,   515,   542,   143,   131,   139,     0,
     367,   264,   126,   368,     0,   695,   369,   356,     0,     0,
      91,    92,    79,     0,   796,   804,     0,   797,   805,   800,
     808,   812,   816,   798,   806,   799,   807,   793,   801,   794,
     802,   795,   803,     0,   936,   688,   735,   688,     0,     0,
     699,   701,   702,   696,     0,   265,   185,   406,   223,   312,
       0,     0,   315,   316,     0,   308,     0,     0,     0,   323,
     307,     0,   337,   338,   286,     0,   291,   288,   341,     0,
     300,   293,   303,   292,   211,   216,   235,   251,   746,   238,
     257,   250,   252,   396,   368,     0,   371,     0,   358,   359,
     361,   362,   688,   368,    76,    94,     0,   922,   693,   693,
     665,   690,   700,   703,     0,   283,     0,   310,     0,   328,
       0,     0,     0,     0,     0,     0,   349,     0,     0,     0,
       0,   344,   342,   332,     0,   333,     0,   239,   257,     0,
     251,   253,   254,   266,   368,   686,   361,     0,     0,   693,
       0,   695,   697,   697,   707,     0,   704,     0,   708,   282,
     279,   311,   331,     0,   314,     0,   340,     0,   322,     0,
     348,   347,   965,   346,     0,     0,   324,     0,     0,   251,
       0,     0,   249,     0,   256,     0,   255,   372,   364,   360,
       0,   357,   118,   688,   618,   618,   698,   703,     0,   710,
       0,   706,   709,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   345,   335,   334,   236,     0,     0,   260,   259,
     258,   686,   693,   839,   636,   705,     0,   712,     0,   329,
     313,   319,     0,     0,     0,   321,   336,   263,   261,   262,
       0,   697,     0,   713,     0,     0,   339,   317,     0,   363,
     618,   711,     0,   330,   318,   320,   838,   714,     0,     0,
       0
};

static const short yydefgoto[] =
{
    1848,    20,    21,   459,   121,   122,   123,   124,  1199,   719,
     714,    22,   136,   965,   966,   967,   968,   125,   126,   464,
     465,    23,    81,   377,   870,   643,   644,   645,  1099,  1100,
     238,    24,    73,    25,   154,    26,   366,    74,  1310,   220,
     370,   635,  1087,  1302,   529,   208,   362,  1073,  1454,  1455,
    1456,  1457,  1458,  1459,   214,   230,   205,   580,   206,   358,
     359,   360,   587,   588,   589,   590,   591,   531,   532,   533,
    1018,  1015,  1016,  1235,  1417,   534,  1023,  1024,  1025,  1240,
    1061,   223,   312,   372,   858,   859,   860,   796,   797,   798,
    1576,   563,   555,  1358,   646,  1041,  1245,  1432,  1433,  1434,
    1582,   861,   565,   805,   806,   807,   808,  1263,   809,  1730,
    1731,  1732,  1809,   218,   310,   629,   193,   340,   560,   556,
     557,  1254,  1255,  1256,  1427,  1706,  1551,  1552,  1553,  1554,
    1555,  1556,  1557,  1558,  1559,  1654,  1560,  1561,  1562,  1563,
    1564,  1714,  1565,   424,   425,  1794,  1724,  1566,  1567,  1798,
    1667,  1668,  1720,  1721,    27,   227,   314,  1092,  1093,  1094,
    1605,  1688,  1689,  1690,   863,  1471,   548,  1035,  1308,   225,
     631,   349,   576,   577,   822,   824,  1275,    28,    58,   784,
     781,   334,   335,   344,   345,   185,   186,   346,  1050,   571,
     814,   190,   162,   327,   328,   544,   198,   578,  1276,    29,
      95,   614,   615,   616,  1074,  1075,   617,   786,   619,   620,
    1067,  1387,   847,   621,   787,   623,   624,   625,   626,   840,
     627,   628,   837,    30,    31,    32,    33,    34,   518,    35,
     481,    36,    37,    60,   202,   351,    38,   298,   299,   300,
     301,   302,   303,   304,   305,   749,   306,  1003,  1230,   503,
     504,   754,   505,    39,  1568,    41,   161,   519,   520,   234,
    1415,  1013,   321,   767,   525,    42,   725,   484,   288,  1234,
    1533,   141,   142,   143,   286,   488,   489,   490,   736,   988,
     989,   990,   991,  1225,   426,  1176,  1691,   148,  1223,  1227,
    1525,  1526,  1410,   381,  1530,  1744,  1644,  1745,  1746,  1747,
    1791,  1832,  1569,  1209,  1570,    45,    46,   382,  1571,    48,
      49,   509,  1572,   491,    50,   974,  1388,  1206,  1391,   258,
     259,   456,   471,   472,   721,   428,   429,   512,   658,   886,
     659,   660,   661,   662,  1123,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   888,  1126,  1127,  1331,   696,   672,
     431,   432,   945,  1247,   433,   434,   435,   436,   437,   438,
     439,   649,   221,   236,   222,  1029,  1076,   744,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   936,
     451,   688,  1180,   689,  1177,   929,   287,   452,   453,   873,
    1277,   946,   211,   793,  1106,   242,   188,   239,  1761,   215,
    1833,   739,   992,   127,   454,   197,   199,   969,   698,   228,
     155,   455,   129
};

static const short yypact[] =
{
    1867,   929,    40,  1365,   405,   602,   446,  1377,   323,  2026,
     589,   482,  1218,    40,   625,   121,  5286,   601,  5286,   391,
     340,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    71,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  5898,  5286,  5286,  5286,  5286,  5286,-32768,-32768,
     516,   736,  5286,  5286,  5286,   690,  5286,   515,  5286,  5286,
  -32768,  5286,  5286,-32768,   215,-32768,   521,-32768,-32768,   693,
    5286,-32768,  5286,  5286,  5286,   703,  5286,  5286,  5286,  5286,
     515,  5286,  5286,  5286,  5286,-32768,  5286,   643,-32768,-32768,
  -32768,   537,-32768,   537,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   687,   551,   714,-32768,   314,-32768,-32768,-32768,-32768,
    5286,  5286,  5286,   720,   747,   754,   664,   114,   713,   320,
     384,   204,   662,-32768,  5286,   809,   544,   753,-32768,  5286,
  -32768,  5286,  5286,  5286,-32768,-32768,-32768,  5286,-32768,   916,
     180,   896,   266,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   321,-32768,-32768,   736,-32768,
  -32768,   197,-32768,-32768,   651,-32768,   856,-32768,-32768,   518,
  -32768,   794,-32768,-32768,   764,-32768,  1055,-32768,-32768,   966,
  -32768,-32768,-32768,   736,-32768,-32768,-32768,    84,-32768,   651,
  -32768,-32768,   428,-32768,   924,-32768,   926,-32768,   537,-32768,
  -32768,-32768,-32768,-32768,   915,-32768,-32768,   805,-32768,   919,
  -32768,   817,-32768,-32768,-32768,   805,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4746,-32768,  5286,-32768,-32768,
  -32768,  3295,   496,   873,  2887,  5286,   948,   771,   981,   991,
    3295,   922,   951,   955,  2887,   850,-32768,  6066,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  4419,-32768,-32768,-32768,
     906,  5286,   985,   910,   155,  5286,   931,-32768,-32768,   544,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  5286,-32768,-32768,
  -32768,   651,-32768,   924,-32768,   537,   989,  6066,   977,-32768,
     959,   950,   136,   881,   168,   889,   473,   266,-32768,  1120,
     196,-32768,  5286,-32768,   321,-32768,-32768,-32768,-32768,  5286,
     997,  2271,   944,  2856,   891,-32768,  5898,-32768,-32768,   231,
  -32768,-32768,-32768,   902,   902,   736,  1051,   736,   942,  1055,
  -32768,-32768,  3254,-32768,  5286,  5286,-32768,   997,-32768,-32768,
    1143,   352,   903,  5286,  1167,  5286,-32768,-32768,   262,   570,
    3765,-32768,-32768,-32768,   917,   918,   930,   988,-32768,-32768,
     938,-32768,   939,-32768,   945,   949,-32768,   953,-32768,-32768,
  -32768,-32768,   954,   957,   990,   992,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   958,-32768,-32768,  4826,   960,   962,-32768,
    6066,  6066,  4516,  5286,-32768,-32768,   920,-32768,   952,-32768,
     804,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   968,   937,   964,   552,-32768,  5286,  1039,
  -32768,-32768,  5286,  5286,   256,-32768,-32768,-32768,-32768,  5286,
    5286,    74,-32768,  5286,-32768,  1185,  5286,  3295,  1104,   982,
  -32768,  5286,  5056,   327,-32768,-32768,  6066,-32768,  1189,   986,
  -32768,   623,   804,-32768,-32768,   523,-32768,   661,-32768,   468,
  -32768,   468,-32768,   994,-32768,   106,-32768,  1083,-32768,   355,
  -32768,-32768,  1004,  1008,   997,   996,  1260,-32768,-32768,  1000,
  -32768,   300,-32768,  1159,  1037,-32768,-32768,   150,  1198,  1201,
  -32768,-32768,-32768,-32768,   736,-32768,-32768,  1203,-32768,   902,
     902,   902,   902,-32768,  1026,-32768,  1275,-32768,  1276,-32768,
  -32768,  1261,-32768,  3254,-32768,  3254,   610,-32768,-32768,   651,
    1298,  5286,-32768,  2522,-32768,   439,-32768,  5286,   721,   856,
    1128,  1095,  1169,  1136,-32768,-32768,-32768,   361,  1177,-32768,
    1117,   515,-32768,   736,-32768,  1304,  1234,-32768,   942,-32768,
  -32768,-32768,-32768,-32768,   564,  1144,-32768,-32768,-32768,  1145,
  -32768,  1184,  1067,-32768,  1246,  1069,   583,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1072,-32768,
  -32768,-32768,   124,  1070,  1071,    16,-32768,  1069,-32768,-32768,
     651,-32768,  1281,  1340,-32768,   736,   736,-32768,  2271,   518,
  -32768,  1222,-32768,  1197,  1084,-32768,  2004,  5248,  1225,-32768,
    1229,  1088,  3862,  1089,   833,  1075,  1081,  3513,   181,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2720,   654,  5136,   132,-32768,  5286,  6066,   724,
     795,   869,  1387,-32768,-32768,  6066,  4092,   804,   145,  1176,
    6066,  5136,   137,   137,   625,   675,  1105,   349,-32768,-32768,
    5136,  6066,  5286,  6066,  6066,  6066,  6066,  6066,  5366,  2951,
    5286,-32768,-32768,  4938,  1200,-32768,-32768,  1170,  2887,-32768,
    1320,   698,-32768,   625,  1111,-32768,  1320,  4938,  1319,  1321,
    5286,-32768,   349,-32768,   367,  3663,  1188,  5136,  5286,-32768,
  -32768,-32768,   546,-32768,-32768,   191,-32768,  1195,-32768,-32768,
  -32768,  5286,    78,  5286,-32768,-32768,  5286,-32768,-32768,  5136,
    5286,  1380,  2271,-32768,  6066,  1150,  5286,-32768,-32768,   736,
     736,   736,-32768,   331,-32768,  1117,  1117,  1196,   515,   473,
  -32768,-32768,  1119,-32768,-32768,-32768,-32768,  1384,-32768,  5286,
  -32768,-32768,-32768,-32768,-32768,  1390,-32768,  1138,  1368,-32768,
    1385,-32768,  1327,  1331,   537,-32768,-32768,-32768,-32768,-32768,
    1398,-32768,-32768,-32768,-32768,-32768,  5286,  6321,   515,  1294,
    1299,  1300,-32768,   515,-32768,   900,-32768,-32768,  1266,   736,
  -32768,   438,  1272,-32768,-32768,-32768,   515,-32768,  1067,   444,
  -32768,-32768,-32768,  1392,   444,  1277,   444,-32768,   515,   515,
    1171,   515,-32768,   997,  5286,-32768,  1341,-32768,   707,-32768,
  -32768,-32768,   231,  1279,  5286,   710,    48,-32768,-32768,  1172,
    1426,-32768,-32768,-32768,-32768,   570,  5286,  1284,  4189,-32768,
  -32768,  1284,  3862,-32768,-32768,-32768,   128,-32768,   295,  2028,
    4092,  4092,  6066,  6066,  1727,  1178,   142,  2136,  6066,  2431,
     638,  2694,  2792,  5446,  3055,  3153,  3416,  6066,  6066,  1439,
    6066,  1181,  6066,  1180,   412,  6066,  6066,  6066,  6066,  6066,
    6066,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1391,
    1134,   182,  5136,-32768,  4092,  6066,   247,   831,  1186,   204,
  -32768,-32768,-32768,-32768,   804,   222,-32768,-32768,  1433,   137,
     137,   865,   865,-32768,   717,-32768,-32768,-32768,-32768,  5286,
    5286,  5286,  5286,  5286,  5286,  1190,-32768,   270,-32768,-32768,
    4938,  1337,-32768,   359,  1358,  5286,-32768,   204,  5136,  1358,
    1190,  1205,  4938,  2332,  1412,-32768,-32768,  3663,  1207,   834,
  -32768,-32768,  1210,  4092,  1410,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    67,-32768,-32768,-32768,-32768,-32768,   722,
    1279,-32768,-32768,-32768,  1213,-32768,-32768,-32768,   730,-32768,
  -32768,   902,  1478,-32768,   331,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1221,  4092,  3279,-32768,-32768,  1447,-32768,-32768,
    3279,-32768,-32768,-32768,-32768,   537,   537,  1248,  1223,-32768,
  -32768,-32768,-32768,-32768,   396,   263,   403,-32768,  1488,-32768,
  -32768,-32768,  5286,-32768,   405,   748,  1336,  1384,  1231,-32768,
    1117,   778,-32768,-32768,   634,-32768,  1224,  5286,   644,  1232,
    1235,   515,  1236,  1498,   518,  1447,   902,  1201,  2271,-32768,
    1177,   204,  1311,  1326,-32768,  2213,-32768,   515,  1375,  1469,
  -32768,  2004,-32768,   515,  1288,  1472,-32768,-32768,  1254,  1255,
  -32768,  4092,  4092,-32768,  4092,  4092,-32768,  1525,   241,   804,
    1263,-32768,-32768,  1268,   804,  5676,-32768,-32768,-32768,  1445,
    1271,  1273,   804,  1219,  1274,  1278,   804,  6066,  6066,  1178,
    6066,  5756,  1282,  1283,   804,  1285,  1286,   804,  6066,   804,
    1289,  1290,   804,  1292,  1295,   804,  1296,  1297,   804,   469,
     579,-32768,   620,-32768,   659,  6066,-32768,   689,   739,   867,
     904,   914,   969,  6066,   515,  5136,-32768,  1494,   294,   804,
    1369,  5136,-32768,  6066,  6066,  5136,  6066,  6066,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  4938,  1481,  4938,-32768,
    1190,   270,-32768,   410,   410,   409,-32768,-32768,  4419,   793,
  -32768,-32768,  1190,  1205,  1301,  2332,   761,  1293,  3663,  1430,
  -32768,  1440,  1441,  1489,  5136,  1068,  1560,  1504,-32768,-32768,
  -32768,  1447,-32768,  1311,    -4,  1414,   736,  1448,  1117,  1446,
    1117,-32768,  4092,   143,   825,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1393,  1569,  1447,-32768,  6066,-32768,   879,
    1435,  1248,   215,-32768,  4092,-32768,-32768,-32768,  1457,  1458,
    1460,  1461,  1464,  1465,-32768,-32768,-32768,-32768,-32768,  1325,
     405,-32768,  1196,-32768,-32768,   444,-32768,  3279,   167,   444,
  -32768,   444,-32768,  1384,-32768,-32768,  1329,-32768,-32768,   231,
    1569,  1117,-32768,  1201,-32768,-32768,  4419,  1577,-32768,   209,
  -32768,  1342,-32768,  1213,-32768,  1339,   736,   162,  1345,-32768,
     736,-32768,-32768,-32768,  1525,-32768,  1525,  6066,  1451,  1451,
     625,  1346,   813,-32768,  1451,  1451,  6066,  1451,  1451,   307,
     804,-32768,  1233,  6066,   804,  1451,  1451,  1451,  1451,   804,
    1451,  1451,  1451,  1451,  1451,  1451,-32768,-32768,  1348,  3254,
  -32768,-32768,   979,-32768,-32768,-32768,-32768,-32768,-32768,  1015,
     115,-32768,-32768,  5136,  5136,  1562,  1442,  1065,   836,  1270,
     804,-32768,-32768,  1493,-32768,-32768,-32768,-32768,  1443,-32768,
  -32768,-32768,-32768,-32768,  1189,  5136,-32768,  2332,  1301,-32768,
     834,-32768,-32768,-32768,  3663,   840,  5286,-32768,  6066,  4092,
    1484,  1569,-32768,-32768,-32768,-32768,   902,-32768,-32768,   736,
     900,-32768,-32768,   144,-32768,-32768,-32768,  5286,  6288,-32768,
  -32768,   804,  1601,   879,-32768,   669,  5286,-32768,  1547,   146,
    1449,  1551,  1450,  1584,  1552,  1592,  1447,-32768,-32768,  1366,
  -32768,  1367,-32768,  1554,  1601,   167,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1447,  1177,-32768,   900,  1488,  1579,
    1522,-32768,  1279,-32768,-32768,-32768,  1537,    86,-32768,  1384,
    1538,   804,  1382,  1383,   204,-32768,-32768,  1386,  1388,   804,
    1389,  1394,  6066,  6066,   804,  1395,  1396,  1397,  1399,  1400,
    1401,  1402,  1405,  1407,  1408,-32768,-32768,-32768,-32768,   515,
  -32768,-32768,-32768,-32768,  5136,-32768,-32768,  1188,-32768,   410,
    1188,-32768,  1291,-32768,-32768,  1413,-32768,   804,   181,   430,
  -32768,-32768,-32768,-32768,  1117,-32768,-32768,-32768,-32768,  3254,
    4318,   335,  1415,   427,  1411,  1580,  6066,  1416,  1417,  1420,
    1421,-32768,-32768,  1613,  4654,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1586,  1424,
    1425,  1427,  1428,-32768,  1423,  5286,-32768,-32768,  1590,  1611,
    5286,  1248,-32768,-32768,   537,  5286,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1569,-32768,-32768,-32768,-32768,-32768,  1569,
  -32768,-32768,-32768,-32768,  3990,  1188,-32768,-32768,  6066,   736,
  -32768,-32768,-32768,   736,-32768,-32768,  6066,-32768,-32768,-32768,
  -32768,   804,   804,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1431,-32768,  1410,-32768,  1410,  4092,  6066,
  -32768,-32768,  1596,-32768,  1438,-32768,-32768,-32768,   130,-32768,
    5986,  5286,-32768,-32768,  6066,-32768,   248,  1618,  4092,   804,
  -32768,  4092,-32768,-32768,-32768,   237,-32768,  1509,-32768,  4247,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1248,   537,-32768,
    1583,-32768,-32768,-32768,-32768,  1488,-32768,  3990,  1444,   834,
  -32768,-32768,  1410,   804,-32768,-32768,  1270,-32768,  1504,  1504,
     181,-32768,-32768,  3593,  3279,-32768,  1452,-32768,   535,  4746,
    1099,  6066,  4247,   147,  1453,   148,-32768,  5286,  5286,   405,
     112,-32768,-32768,-32768,   787,-32768,  1436,-32768,  1583,   269,
    1248,  1589,  1591,-32768,-32768,   761,  1455,  3990,  1624,  1504,
    1467,  1188,  1484,  1484,-32768,  1468,  1454,   652,  5286,-32768,
  -32768,-32768,  1559,  4247,-32768,  1107,   140,  1540,-32768,  1672,
  -32768,-32768,-32768,-32768,  6288,   237,-32768,  4247,  5504,  1248,
      70,    70,-32768,  1678,-32768,  1543,-32768,-32768,-32768,   834,
    3990,-32768,-32768,  1410,  1599,  1599,-32768,  3593,  1462,-32768,
    5286,-32768,-32768,  3922,  1463,   860,  4247,  1704,  1694,  6288,
    6288,-32768,-32768,-32768,-32768,-32768,  1536,   594,-32768,-32768,
  -32768,  1332,  1504,-32768,-32768,-32768,  5286,-32768,  4247,  1474,
  -32768,-32768,   119,  5286,  6288,  1695,-32768,-32768,-32768,-32768,
    4092,  1484,  1473,  1479,   854,  6288,-32768,-32768,  6288,   181,
    1599,-32768,  5286,-32768,-32768,-32768,-32768,-32768,  1750,  1752,
  -32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -96,   435,-32768,   447,  1491,   553,-32768,
    -319,-32768,-32768,  -566,   -52,  -600, -1002,  1743,  1492,  1482,
    1040,-32768,-32768,-32768,   660,-32768,  1118,   897,-32768,   665,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   680,-32768,-32768,-32768,-32768,-32768,
     317,-32768,-32768,  1456,-32768,-32768,-32768,  -334,-32768,-32768,
  -32768,  1429,-32768,-32768,  1187,  -233,  1470,-32768,-32768,  -326,
  -32768,  -706,-32768,-32768,-32768,-32768,-32768,-32768,   749,-32768,
    -768,-32768,-32768,  1466,  1012,   688,  1459,   980,   691,-32768,
     328,   -49,-32768,-32768,  -311,   135,  -988,-32768,-32768,   351,
  -32768,  1471, -1084,-32768,-32768,-32768,-32768, -1196,   354,    59,
      61,    60,    23,-32768,-32768,-32768,-32768,   -90,  -297,-32768,
    1006,  -957,-32768,   542,-32768,-32768, -1359, -1115,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   149,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1062,    89,-32768, -1215,-32768,-32768,-32768,
  -32768,   139,-32768,    37,-32768,-32768,-32768,   798,-32768, -1131,
  -32768,-32768, -1486,   116,  -643,   397,  -306,  -909,   577,-32768,
  -32768,  -522,  -752,-32768,-32768,  -960, -1295,-32768,-32768,-32768,
  -32768,-32768,  1477,-32768,  1244,  1475,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1490,-32768,-32768,-32768,-32768,-32768,
  -32768,  -523,  -463,-32768,   970,   526,  -570,  -341,-32768,-32768,
  -32768,  -310,  -954,-32768,  -335,-32768,-32768,-32768,-32768,  1191,
  -32768,-32768,   984,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1806,-32768,-32768,-32768,-32768,-32768,  1521,
  -32768,-32768,-32768,  1328,-32768,  1322,-32768,-32768,-32768,-32768,
    1073,-32768,-32768,-32768,    10,-32768,  -986,-32768,  1062,  -470,
  -32768,-32768,-32768,-32768,-32768,    51,    28,  -169,   350,-32768,
  -32768,  -628,-32768,  1687,  -872,  -774,-32768,  1093, -1234,-32768,
    -871,   844,-32768,-32768,  -385,  -239,   614,   -45, -1492, -1404,
  -32768,   193, -1485,  -478, -1252,   305,-32768,    65,-32768,    90,
  -32768,    12,  1856,-32768,  1857,-32768,-32768,  1355,  1865,-32768,
  -32768,-32768,  -205,  -633,-32768,  1140, -1071,   888,-32768,   -91,
    -658,  -632,-32768,-32768,-32768,  -770,   -34,  -412,  -598,-32768,
    -486,-32768,-32768,-32768,   617,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -485,-32768,   729,-32768,  1079,-32768,   943,
    -937,-32768,  -589,-32768,   626,  -928,  -110,  -926,  -925,  -916,
     -11,     0,    -2,-32768,   -54,  -732,  -471,  -122,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   686,-32768, -1022,-32768,   442,  -271,   424,-32768,
     796,   296,  -671,  1305,    53,-32768,   -58,  1791,-32768,   -19,
     -53,  1141,   -47,   -33,  -135,   -10,   -36,  -173,-32768,   -59,
     -26,    -9,-32768
};


#define	YYLAST		6584


static const short yytable[] =
{
     128,   191,    78,   128,    77,   213,   149,   150,   194,   156,
      40,   196,   260,   209,   308,   493,   427,   280,   283,   219,
     581,   618,   553,   224,   743,   551,   244,   622,   210,   279,
     282,   757,   592,   226,   253,   248,   250,   695,   957,   231,
     799,   909,   249,   187,   189,   192,   195,   150,   200,   255,
     207,   765,  1258,   212,   189,   216,   252,   195,   722,   150,
     200,   254,   229,   128,  1019,  1437,   939,  1186,   247,   648,
     633,   240,   149,   150,   243,   189,   871,   240,   216,   192,
     195,   251,   150,   200,   229,   128,   267,   195,   931,  1007,
     785,   466,   788,   543,   315,   977,   266,  1246,   268,  1065,
     800,   466,   510,  1246,   311,  1208,  1248,   313,  1249,  1250,
    1090,   485,  1248,  1283,  1249,  1250,  1216,   862,  1251,   954,
    1010,   212,   150,   212,  1251,   290,  1006,   981,  1300,   367,
    1305,   316,  1259,  1390,  1014,   216,  1111,   374,   284,  1429,
     309,  1265,   195,   150,   229,  1517,  1047,   845,   156,  1037,
    1797,   890,   890,  1371,   890,   890,   890,   728,  1044,  1375,
    1520,   980,  1764,   702,  1288,   769,   879,   880,   703,  1835,
     752,   887,   513,  1603,   475,  1435,  1040,   336,  1473,  1509,
     274,   910,  1477,   427,   499,  1466,   318,  1610,  1097,   890,
     890,  -367,   144,  1806,  1382,  1666,  1385,  1738,   932,   933,
     337,  1735,   363,   772,   159,   775,   776,   777,   778,   850,
     284,   319,  1085,  1742,  1743,  1472,   460,   761,  1228,  1306,
     499,   514,   618,   457,   516,   460,   232,  1001,   622,   549,
     526,  1698,   364,  1699,  1807,  1413,   574,   723,   496,   527,
    1128,    59,   550,  1738,  1002,  1414,  1716,   575,   212,  1327,
     506,  1779,   150,   285,  1781,   467,   128,   160,   994,   810,
    1112,   150,   592,   233,   724,   467,  1129,   702,   365,  1229,
     322,   536,   703,   511,  1411,   891,   891,   338,   891,   891,
     891,  1808,   192,   323,   832,   500,   150,  1738,  1739,   851,
     716,  1453,  1116,  1117,  1811,  1717,  1531,   145,   552,  1452,
    1181,  1182,   890,  1114,   146,   558,  1718,   210,   843,   568,
    1270,   232,   572,   891,   891,  1492,  1770,   630,   324,  1738,
     325,  1098,   641,   212,  1611,   329,   702,  1831,   639,   632,
     212,   703,   212,   702,   212,   513,  1178,   573,   703,  1462,
     501,  1607,   934,    14,   582,  1210,   584,  1400,   233,  1435,
    1246,  1511,  1512,   702,  1051,   195,   200,   502,   703,  1248,
     637,  1249,  1250,  1175,   150,  1231,   192,  1447,  1071,   522,
    1201,  1251,  1091,   330,  1478,   702,   501,    78,   753,  1812,
     703,   729,  1213,  1214,  1765,  1679,   265,  1260,  1261,  1510,
    1734,  1767,   879,   702,  1704,  1385,   879,  1110,   703,   528,
     846,   911,  1113,   493,  1200,  1801,   706,   707,   819,  1267,
    1719,   984,  1767,   712,   699,   715,  1212,  1424,  1537,   -80,
    1586,  1757,  1759,   770,   720,  1203,   891,  1115,   726,   715,
     326,   537,   460,  1021,  1394,  1243,   511,  1312,   702,  1652,
    1825,  1826,   368,   703,  1183,   820,   636,   134,  1636,   150,
    1272,  1271,   938,   195,   467,   731,   717,  1771,   825,   135,
     150,   943,  1741,  1313,   150,  1837,   493,   195,   150,   791,
    1197,    96,   156,  1393,  1032,  1373,  1844,  1034,  1683,  1845,
    1268,  1727,   740,  1651,  1684,   331,  1656,  1273,   493,  1593,
    1784,  1785,  1634,  1042,  1187,   702,   264,  1756,   539,  1188,
     703,   132,  1653,   332,   802,  1600,  1022,  1599,   704,   705,
     706,   707,    82,  1323,  1324,  1538,  1325,  1326,  1058,  1640,
    1573,   347,  1574,   773,   276,  1612,  1652,   826,   718,   801,
    1418,   871,  1469,  1522,  1772,   642,  1332,   333,  1795,   369,
     853,  1204,  1198,    98,  1641,   466,  1031,  1465,   540,   821,
    1450,  1005,   794,   380,   212,  1101,  1083,  1601,   794,   203,
     204,   702,  1299,  1207,   985,  1105,   703,   704,   705,   706,
     707,   272,   827,  1805,   704,   705,   706,   707,  1819,  1840,
     100,  1822,   541,   273,  1269,   803,   265,   647,   276,  1653,
      14,   747,    78,   277,   704,   705,   706,   707,   348,   278,
     542,   733,  1642,  1834,   210,   702,   841,  1723,   130,    75,
     703,   158,  1616,  1028,    75,  1398,   704,   705,   706,   707,
     998,   513,  1645,   942,   856,   857,   804,   756,  1647,   212,
     147,   486,   292,   738,   704,   705,   706,   707,   874,   494,
    1828,   986,  1028,   157,  1423,    78,   702,   872,   276,   101,
    1723,   703,  1536,  1026,  1027,  1464,  1137,   281,   913,   102,
     284,   378,   748,   278,    79,    80,  1439,   293,   216,   383,
      97,   201,  1138,    76,   842,   131,   958,  1682,    76,   704,
     705,   706,   707,   715,   103,   702,  1166,  1238,   278,   278,
     703,  1723,  1829,   947,  1686,   294,   241,   217,   295,  1602,
     212,   212,  1484,   907,   467,  1803,   506,   493,  1004,   467,
    1064,    98,  1070,  1535,   296,   702,  1749,  1139,   467,    75,
     703,   195,   511,    99,  1030,   235,   195,   276,   210,   740,
     284,  1723,   457,  1257,  1723,  1788,   704,   705,   706,   707,
     297,   647,   150,  1356,   150,  1266,  1140,   647,   100,   151,
     276,   212,  1301,   212,   741,   558,  1723,   212,  1017,  1017,
    1020,   237,  1521,  1252,  1053,   702,  1287,  1246,    75,  1252,
     703,   245,  1789,   915,    75,  1733,  1248,   997,  1249,  1250,
     212,   152,  1049,  1052,  1740,  1790,  1190,  1101,  1251,  1193,
     256,   742,  1438,  1578,  1079,  1080,   153,  1082,  1813,  1814,
    1378,   284,   704,   705,   706,   707,  1751,   212,   212,  1084,
     257,  1528,   745,  1141,  1451,  1579,   261,   101,  1063,  1095,
    1461,  1057,  1580,   262,   710,  1777,   711,   102,  1219,   104,
     702,   296,    76,  -829,  1068,   703,  1506,   831,    76,  1405,
     513,  -829,  1220,   263,   917,   150,   704,   705,   706,   707,
    1359,   269,   103,  1357,  1846,   150,  1581,   702,   105,   106,
     341,   107,   703,   342,   139,   140,  1221,  1107,   108,   109,
     110,   111,   112,    78,   113,   284,   270,   114,   115,   116,
     117,   118,   789,   271,   790,   119,   120,   704,   705,   706,
     707,   702,   291,   702,  1360,   275,   703,  -829,   703,  1725,
    -829,  1219,   140,  -242,  1194,  1108,  1289,  -829,   343,  1109,
    -829,  1290,   561,  1191,  -829,  1220,  1289,   307,   919,  -829,
    1222,  1293,  -829,  -686,   339,  1192,   704,   705,   706,   707,
     702,  1195,   812,  1361,   813,   703,   317,   493,  -829,  1221,
     702,   511,  1725,   203,   204,   703,   427,   700,   998,   940,
     467,   195,   200,   467,   229,   128,   704,   705,   706,   707,
     320,   467,  -829,  1363,   350,  -829,  -829,   539,  1446,    51,
     975,   352,   976,   467,   467,  -829,   361,   772,   195,  1088,
      52,  1089,   867,  1725,  1096,   427,   371,    53,   585,  1187,
     373,  1189,  -829,  1222,  1088,   702,  1232,  1804,   375,  1468,
     703,   379,  1236,  -242,  1237,   702,   704,   705,   706,   707,
     703,   376,    54,  1364,  -829,   380,  1252,   540,   618,  -829,
    1280,   462,  1281,  1725,   622,  -242,  1725,  1296,   399,   400,
     401,  -829,  -242,  -829,   469,   493,   526,  1059,  1060,  1635,
    1700,   702,  1637,  1314,   470,   527,   703,   473,  1725,  1318,
    1285,   541,  1286,  1278,   210,   474,   409,   410,  1766,  1767,
    1713,   457,    78,  1715,  1279,  1395,  -242,  1396,  1278,   542,
     476,   704,   705,   706,   707,  -830,  1648,    55,  1406,   212,
     477,  -831,  1534,  -830,   478,  1187,   212,  1486,   495,  -831,
    1407,   702,   497,  1389,  1389,  1392,   703,   957,   704,   705,
     706,   707,   480,  1184,  -829,  -829,   498,  -829,   700,    56,
    1516,   507,   700,    57,  1523,   908,  1420,   912,  1422,   517,
    1370,   916,   918,   920,   493,   702,  1767,  1692,  1843,   810,
     703,  1821,  1767,   702,   704,   705,   706,   707,   703,  -830,
      14,  1365,  -830,   535,  1381,  -831,  1384,   523,  -831,  -830,
     524,   538,  -830,   546,   559,  -831,  -830,   353,  -831,   566,
     702,  -830,  -831,   569,  -830,   703,   579,  -831,  1608,   583,
    -831,   704,   705,   706,   707,   634,   638,   640,  1366,  1467,
    -830,   704,   705,   706,   707,  1753,  -831,   467,  1367,   467,
     673,   674,   700,  1796,   354,   676,   355,   683,   430,   684,
    1174,    78,    78,   675,  -830,   586,   467,  -830,  -830,   195,
    -831,   677,   678,  -831,  -831,   709,   309,  -830,   679,   356,
     483,   713,   680,  -831,    97,  1017,   681,  -955,   701,   492,
     682,   685,  1839,   690,  -830,   691,   704,   705,   706,   707,
    -831,   708,  -971,  1368,   357,   702,   704,   705,   706,   707,
     703,   727,   730,  1507,   718,   735,  -830,   755,   737,   702,
     521,  -830,  -831,  1783,   703,    98,   751,  -831,   759,   762,
     763,  1524,   764,  -830,   104,  -830,  1336,    99,    78,  -831,
    1449,  -831,   704,   705,   706,   707,   760,   766,   768,  1508,
    1493,   133,   771,   513,  1685,   526,   702,   774,   779,   780,
     782,   703,   100,   105,   106,  1476,   107,   783,   792,  1480,
     816,   817,   818,   108,   109,   110,   111,   112,  -426,   113,
     823,   276,   114,   115,   116,   117,   118,   828,   829,   835,
     119,   120,   704,   705,   706,   707,   735,   833,   834,  1515,
     836,   838,   839,   848,   849,   854,  -830,  -830,   844,  -830,
     855,   864,  -831,  -831,   866,  -831,   867,   876,  1219,   687,
     875,   877,   881,   692,   693,   697,   704,   705,   706,   707,
    1754,   101,  1220,   935,   704,   705,   706,   707,  1539,   941,
     971,   102,   970,   973,   978,   982,   993,   983,   467,   999,
    1008,  1584,  1033,   210,   511,   195,  1221,   309,  1012,  1219,
    1028,   704,   705,   706,   707,    61,   103,   845,  1017,  1389,
    1038,  1039,  1646,  1220,  1040,   795,    62,  1045,   212,   513,
    1638,  1046,  1048,    63,   104,   732,  1054,   150,    83,   734,
    1062,  1055,  1056,  1252,  1066,    84,    64,  1221,  1072,    85,
      86,  1077,  1091,  1086,  1081,  1103,  1104,   723,    87,  1161,
    1222,  1125,  1165,   105,   106,  1163,   107,  1173,  1185,   702,
      88,  1830,  1196,   108,   109,   110,   111,   112,  1202,   113,
     427,  1205,   114,   115,   116,   117,   118,  1198,  1215,  1218,
     119,   120,  1650,  1224,  1226,   710,   704,   705,   706,   707,
    1239,  1222,  1253,  1680,  1242,  1262,  1264,    65,  1274,  1291,
     704,   705,   706,   707,  1282,  1284,  1294,  1633,  1298,  1295,
    1297,  1307,  1309,    66,  1131,  1097,  1135,    78,  1143,  1146,
     511,  1151,  1154,  1157,  1316,    89,  1319,  1320,  1321,  1322,
      67,   189,  1681,   890,  1726,  1678,  1328,   704,   705,   706,
     707,  1329,    90,  1333,  1334,    68,  1335,  1337,  1372,    69,
    1374,  1338,    70,  1657,  1383,  1345,  1346,    91,  1347,  1348,
      71,    92,  1350,  1351,  1401,  1352,   947,  1399,  1353,  1354,
    1355,   150,    93,  1397,  1402,  1403,   192,  1726,  1404,    72,
    1408,  1409,  1416,  1421,  1419,  1426,  1428,  1728,  1436,  1440,
    1441,    94,  1442,  1443,   149,   150,  1444,  1445,  1694,  1448,
     889,  1470,  1695,  1463,  1709,   921,   922,   923,   924,   925,
     926,   927,   928,  1475,  1330,  1474,  1513,   492,  1726,  1479,
    1485,   914,  1505,  1514,  1518,  1519,  1529,  1575,   930,   513,
    1585,  1590,  1726,   937,   430,  1588,  1591,  1587,  1589,  1592,
    1594,  1595,   195,   430,   944,  1604,   948,   949,   950,   951,
     952,   944,  1596,  1606,  1609,  1613,  1614,  1615,  1726,  1760,
    1617,  1726,  1618,  1619,   513,   513,   131,  1664,  1620,  1623,
    1624,  1625,  1669,  1626,  1627,  1628,  1629,   149,   150,  1630,
     492,  1631,  1632,  1726,  1658,  1639,  1655,  1660,  1676,   513,
    1661,  1662,  1663,  1748,   150,  1670,  1671,  1674,  1672,  1673,
     513,  1677,   492,   513,  1712,  1697,  1665,   521,   189,  1762,
    1702,  1703,  1729,  1780,  1768,  1793,  1737,    78,  1773,  1763,
    1775,  1799,  1800,  1750,  1758,  1770,  1787,   149,   150,  1778,
     511,  1771,   159,  1120,  1820,  1816,  1121,  1817,  1748,   150,
     384,  1782,  1786,  1823,  1824,  1827,  1767,  1841,  1838,   385,
    1849,  1842,  1850,   461,  1386,   137,   479,   468,   972,   212,
     865,  1317,   386,  1315,  1102,   511,   511,  1303,   387,   388,
     149,   150,  1598,  1241,  1009,   830,  1304,  1748,   150,   515,
    1043,   192,  1597,  1705,  1577,   547,  1311,  1769,   593,  1583,
     511,  1774,   530,  1776,  1810,  1036,   389,  1430,  1752,   390,
     562,   511,  1802,  1736,   511,  1711,  1722,   192,  1233,  1532,
    1412,   554,   564,   815,  1107,  1460,  1078,   545,   852,   138,
     508,   570,  1069,   750,  1000,   746,  1011,   392,   393,   289,
     995,  1217,  1701,   192,  1643,  1118,  1119,  1124,  1792,   394,
    1132,  1133,  1136,   395,  1144,  1147,  1149,  1152,  1155,  1158,
    1159,  1160,  1815,  1162,  1847,  1164,    43,    44,  1167,  1168,
    1169,  1170,  1171,  1172,   758,    47,   979,  1211,  1341,  1376,
    1425,  1675,   811,  1292,     1,   430,  1836,   246,  1179,   996,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,     0,  1122,     0,
     396,     0,     0,     3,     0,     0,   397,     0,     0,     0,
       4,     0,     5,     0,     6,     0,   398,     0,     0,     7,
       0,   492,     0,     0,     0,     0,     8,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       9,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,    10,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,   420,   421,     0,     0,     0,     0,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,     0,
       0,     0,     0,   868,     0,    12,     0,   595,    13,     0,
      14,    15,    97,     0,     0,     0,     0,     0,     0,     0,
       0,   869,     0,     0,     0,   596,   892,     0,   597,     0,
       0,     0,     0,     0,   702,    16,     0,     0,     0,   703,
       0,     0,   893,     0,     0,     0,     0,     0,   944,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
    1339,  1340,     0,  1342,  1344,    99,     0,     0,     0,   598,
       0,  1349,     0,     0,     0,   599,   600,   601,   894,   602,
     603,   604,     0,   605,    17,     0,     0,   895,  1362,     0,
     100,     0,     0,     0,     0,   896,  1369,     0,   430,    18,
      19,     0,     0,   606,   430,   607,  1179,  1377,   430,  1379,
    1380,     0,     0,     0,   897,     0,   898,     0,     0,     0,
       0,     0,  1130,     0,     0,  1121,     0,     0,     0,   384,
     899,   492,   900,   901,   902,     0,   608,     0,   385,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   386,     0,   609,     0,     0,     0,   387,   388,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,     0,     0,     0,   610,     0,     0,     0,     0,
    1431,     0,     0,   903,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,   103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     611,   612,   104,     0,     0,     0,   392,   393,     0,     0,
       0,     0,     0,   795,     0,     0,     0,   613,   394,   492,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   106,     0,   107,     0,     0,     0,     0,     0,
    1481,   108,   109,   110,   111,   112,     0,   113,     0,  1489,
     114,   115,   116,   117,   118,     0,  1494,     0,   119,   120,
       0,     0,   904,   905,   906,   704,   705,   706,   707,     0,
       0,     0,   942,     0,   561,     0,     0,  1122,     0,   396,
       0,     0,     0,     0,     0,   397,   430,   430,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   492,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,  1527,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,   420,   421,     0,   959,  1482,  1483,   482,
       0,   423,     0,  1487,  1488,   278,  1490,  1491,     0,   104,
       0,     0,     0,     0,  1495,  1496,  1497,  1498,     0,  1499,
    1500,  1501,  1502,  1503,  1504,  1621,  1622,  1134,     0,     0,
    1121,     0,     0,     0,   384,     0,     0,     0,   105,   106,
       0,   107,     0,   385,     0,     0,     0,   430,   108,   109,
     110,   111,   112,     0,   113,     0,   386,   114,   115,   116,
     117,   118,   387,   388,     0,   119,   120,   104,     0,     0,
       0,     0,     0,     0,     0,     0,   257,     0,     0,  1659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     389,     0,     0,   390,     0,     0,   105,   106,     0,   107,
       0,     0,     0,     0,     0,     0,   108,   109,   110,   111,
     112,   962,   113,     0,     0,   114,   115,   116,   117,   118,
       0,   392,   393,   119,   120,     0,     0,     0,   104,     0,
       0,   594,     0,   394,     0,   595,     0,   395,     0,     0,
       0,  1693,   795,     0,     0,     0,     0,     0,     0,  1696,
       0,     0,     0,   596,     0,     0,   597,   105,   106,     0,
     107,     0,     0,     0,     0,     0,     0,   108,   109,   110,
     111,   112,  1527,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,  1708,   119,   120,     0,  1710,     0,     0,
       0,     0,  1122,     0,   396,     0,     0,   598,     0,     0,
     397,     0,     0,   599,   600,   601,     0,   602,   603,   604,
     398,   605,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,   401,   402,   203,   204,
       0,   606,     0,   607,     0,     0,     0,     0,   403,     0,
       0,     0,   430,     0,  1755,     0,     0,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   105,   413,     0,   107,
       0,     0,   414,   415,   608,   416,   417,   418,   110,   111,
     112,   419,   113,     0,     0,   114,   115,   116,   117,   118,
       0,   609,     0,   119,   120,     0,     0,     0,   420,   421,
    1142,     0,     0,  1121,   482,     0,   423,   384,     0,     0,
     278,     0,     0,   610,     0,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,   386,
       0,     0,     0,     0,     0,   387,   388,     0,   892,     0,
       0,     0,     0,     0,     0,     0,   702,     0,   611,   612,
       0,   703,     0,     0,   893,     0,     0,   105,   106,     0,
     107,     0,     0,   389,     0,   613,   390,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,     0,     0,     0,     0,
     894,     0,     0,     0,   392,   393,     0,     0,  1145,   895,
       0,  1121,     0,     0,     0,   384,   394,   896,     0,     0,
     395,     0,     0,     0,   385,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   897,   386,   898,     0,
       0,     0,     0,   387,   388,     0,     0,     0,     0,     0,
       0,     0,   899,     0,   900,   901,   902,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,     0,     0,   390,  1122,     0,   396,     0,     0,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,   567,
       0,     0,   392,   393,     0,   903,     0,   399,   400,   401,
     402,   203,   204,     0,   394,     0,     0,     0,   395,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   105,
     413,     0,   107,     0,     0,   414,   415,     0,   416,   417,
     418,   110,   111,   112,   419,   113,     0,     0,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,     0,
       0,   420,   421,  1122,     0,   396,     0,   482,     0,   423,
       0,   397,     0,   278,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,   904,   905,   906,   704,   705,   706,
     707,     0,     0,   955,     0,   399,   400,   401,   402,   203,
     204,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   105,   413,     0,
     107,     0,     0,   414,   415,     0,   416,   417,   418,   110,
     111,   112,   419,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,     0,     0,     0,   420,
     421,  1150,   104,     0,  1121,   482,     0,   423,   384,     0,
       0,   278,     0,     0,     0,     0,   463,   385,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     386,   105,   106,   104,   107,     0,   387,   388,     0,     0,
       0,   108,   109,   110,   111,   112,     0,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,   105,   106,   389,   107,     0,   390,     0,     0,
       0,     0,   108,   109,   110,   111,   112,     0,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,     0,   392,   393,   104,     0,  1153,
       0,     0,  1121,     0,     0,     0,   384,   394,     0,     0,
       0,   395,     0,     0,     0,   385,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,   106,   386,   107,
       0,     0,     0,     0,   387,   388,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120,     0,     0,     0,     0,     0,
     956,     0,   389,     0,     0,   390,  1122,     0,   396,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,   399,   400,
     401,   402,   203,   204,     0,   394,     0,     0,     0,   395,
       0,     0,   403,   594,     0,     0,     0,   595,     0,     0,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,   596,   414,   415,   597,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     387,     0,   420,   421,  1122,     0,   396,     0,   482,     0,
     423,     0,   397,     0,   278,     0,     0,     0,     0,   598,
       0,     0,   398,     0,     0,   599,   600,   601,   389,   602,
     603,   604,     0,   605,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,   606,     0,   607,     0,   391,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,     0,   608,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
     420,   421,  1156,   609,     0,  1121,   482,     0,   423,   384,
       0,     0,   278,     0,     0,     0,     0,     0,   385,     0,
       0,     0,     0,     0,     0,   610,     0,     0,     0,     0,
       0,   386,   396,     0,     0,     0,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,   458,     0,     0,     0,     0,
     611,   612,   399,   400,   401,   389,   203,   204,   390,     0,
       0,     0,     0,     0,     0,     0,     0,   613,     0,     0,
       0,   104,     0,     0,     0,   404,   405,   406,   407,   408,
     409,   410,   411,   412,     0,     0,   392,   393,     0,     0,
     414,   415,     0,     0,     0,     0,   384,     0,   394,   419,
     105,   106,   395,   107,     0,   385,     0,     0,     0,     0,
     108,   109,   110,   111,   112,     0,   113,  1244,   386,   114,
     115,   116,   117,   118,   387,   388,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,     0,     0,     0,     0,     0,     0,
       0,     0,   389,     0,     0,   390,     0,  1122,     0,   396,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,   394,     0,     0,     0,   395,
       0,     0,     0,   403,     0,     0,     0,   882,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   116,   117,   118,     0,   694,     0,   119,   120,
     653,     0,  1640,   420,   421,     0,   396,     0,     0,   482,
       0,   423,   397,     0,     0,   278,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,  1641,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,  1642,     0,   114,   115,   116,
     117,   118,   883,   884,   885,   119,   120,     0,   384,     0,
     420,   421,     0,     0,     0,     0,   657,   385,   423,     0,
       0,     0,   278,     0,     0,     0,     0,     0,     0,   104,
     386,     0,     0,   650,     0,     0,   387,   388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   651,     0,     0,   105,   106,
       0,   107,     0,     0,   389,     0,     0,   390,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,     0,   104,
       0,     0,     0,     0,     0,   384,     0,   394,     0,     0,
       0,   395,     0,     0,   385,     0,     0,     0,     0,   652,
       0,     0,     0,     0,     0,     0,     0,   386,   105,   106,
       0,   107,     0,   387,   388,     0,     0,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,   651,     0,     0,   119,   120,     0,     0,     0,
       0,   389,   653,     0,   390,     0,   987,     0,   396,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,   392,   393,     0,     0,     0,     0,   399,   400,
     401,   402,   203,   204,   394,     0,     0,     0,   395,     0,
       0,     0,   403,     0,     0,     0,   878,     0,     0,     0,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,     0,   414,   415,     0,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   654,   655,   656,     0,     0,     0,   119,   120,   653,
       0,     0,   420,   421,     0,   396,     0,     0,   657,     0,
     423,   397,     0,     0,   278,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,   401,   402,   203,
     204,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   105,   413,     0,
     107,     0,     0,   414,   415,   384,   416,   417,   418,   110,
     111,   112,   419,   113,   385,     0,   114,   115,   654,   655,
     656,     0,     0,     0,   119,   120,     0,   386,   104,   420,
     421,     0,     0,   387,   388,   657,     0,   423,     0,     0,
       0,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   651,     0,     0,     0,     0,   105,   106,     0,
     107,   389,     0,     0,   390,     0,     0,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,     0,     0,     0,     0,
       0,     0,   392,   393,     0,  1818,   104,   423,     0,     0,
       0,     0,   384,     0,   394,     0,     0,     0,   395,     0,
       0,   385,     0,     0,     0,     0,   652,     0,     0,     0,
       0,     0,     0,     0,   386,   105,   106,     0,   107,     0,
     387,   388,     0,     0,     0,   108,   109,   110,   111,   112,
       0,   113,     0,     0,   114,   115,   116,   117,   118,   651,
       0,     0,   119,   120,     0,     0,     0,     0,   389,   653,
       0,   390,     0,  1687,     0,   396,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,   392,
     393,     0,     0,     0,     0,   399,   400,   401,   402,   203,
     204,   394,     0,     0,     0,   395,     0,     0,     0,   403,
       0,     0,     0,   878,     0,     0,     0,     0,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   105,   413,     0,
     107,     0,     0,   414,   415,     0,   416,   417,   418,   110,
     111,   112,   419,   113,     0,     0,   114,   115,   654,   655,
     656,     0,     0,     0,   119,   120,   653,     0,     0,   420,
     421,     0,   396,     0,     0,   657,     0,   423,   397,     0,
       0,   278,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,   401,   402,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   105,   413,     0,   107,     0,     0,
     414,   415,   384,   416,   417,   418,   110,   111,   112,   419,
     113,   385,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,   104,   386,     0,   420,   421,     0,     0,
     387,   388,   657,     0,   423,     0,     0,     0,   278,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   106,     0,   107,     0,     0,   389,     0,
       0,   390,   108,   109,   110,   111,   112,     0,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,     0,     0,     0,   391,     0,   392,
     393,     0,   423,     0,   104,     0,     0,     0,     0,   384,
       0,   394,     0,     0,     0,   395,     0,     0,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   386,     0,   105,   106,     0,   107,   387,   388,     0,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,     0,   389,     0,     0,   390,  1649,
       0,     0,   396,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,   391,     0,   392,   393,     0,     0,
       0,     0,   399,   400,   401,   402,   203,   204,   394,     0,
       0,     0,   395,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   105,   413,     0,   107,     0,     0,
     414,   415,     0,   416,   417,   418,   110,   111,   112,   419,
     113,  1428,     0,   114,   115,   116,   117,   118,     0,   694,
       0,   119,   120,     0,     0,     0,   420,   421,   487,   396,
       0,     0,   482,     0,   423,   397,     0,     0,   278,     0,
       0,     6,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,  1540,  1541,     0,  1542,     0,     0,  1543,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,  1544,   403,     0,     0,     0,     0,  1545,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,   384,
     416,   417,   418,   110,   111,   112,   419,   113,   385,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,   386,     0,   420,   421,     0,     0,   387,   388,   482,
       0,   423,     0,     0,     0,   278,     0,     0,  1546,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,     0,    14,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1547,     0,     0,     0,     0,     0,     0,   384,
       0,     0,    16,     0,   391,     0,   392,   393,   385,     0,
       0,  1665,     0,  1548,     0,     0,     0,     0,   394,     0,
     104,   386,   395,     0,     0,     0,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1549,
     106,     0,   107,     0,     0,   389,     0,     0,   390,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,  1550,
     116,   117,   118,     0,     0,     0,   119,   120,     0,   396,
       0,     0,     0,     0,     0,   397,   392,   393,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,   396,
     114,   115,   116,   117,   118,   397,     0,     0,   119,   120,
       0,     0,   959,   420,   421,   398,     0,     0,     0,   422,
       0,   423,     0,   686,     0,   278,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,   384,
     416,   417,   418,   110,   111,   112,   419,   113,   385,     0,
     114,   115,   116,   117,   118,     0,   960,     0,   119,   120,
       0,   386,     0,   420,   421,     0,     0,   387,   388,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   961,     0,     0,   389,     0,   962,   390,     0,
       0,     0,     0,   963,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,   384,
       0,     0,   964,     0,     0,     0,   392,   393,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,   386,   395,   105,   106,     0,   107,   387,   388,     0,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   694,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,   391,   397,   392,   393,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,   396,
     114,   115,   116,   117,   118,   397,     0,     0,   119,   120,
       0,     0,     0,   420,   421,   398,     0,     0,     0,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,   384,
     416,   417,   418,   110,   111,   112,   419,   113,   385,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,   386,     0,   420,   421,     0,     0,   387,   388,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,     0,   104,     0,     0,     0,     0,   384,
       0,     0,     0,     0,     0,     0,   392,   393,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,   386,   395,   105,   106,     0,   107,   387,   388,     0,
       0,     0,   104,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,   115,   116,   117,   118,     0,     0,     0,
     119,   120,     0,     0,     0,   389,    76,     0,   390,     0,
       0,   105,   106,     0,   107,     0,     0,     0,     0,     0,
       0,   108,   109,   110,   111,   112,     0,   113,     0,   396,
     114,   115,   116,   117,   118,   397,   392,   393,   119,   120,
       0,     0,     0,     0,     0,   398,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,   396,
     114,   115,   116,   117,   118,   397,     0,     0,   119,   120,
       0,     0,     0,   420,   421,   398,     0,     0,     0,   482,
     953,   423,     0,     0,     0,   278,  1148,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,   384,
     416,   417,   418,   110,   111,   112,   419,   113,   385,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
     104,   386,     0,   420,   421,     0,     0,   387,   388,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     106,     0,   107,     0,     0,   389,     0,     0,   390,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,   384,
       0,     0,     0,   956,     0,     0,   392,   393,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,   386,   395,     0,     0,     0,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1330,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,   397,   392,   393,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,   396,
     114,   115,   116,   117,   118,   397,     0,     0,   119,   120,
       0,     0,     0,   420,   421,   398,     0,     0,     0,   482,
       0,   423,     0,     0,     0,   278,  1343,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,   384,
     416,   417,   418,   110,   111,   112,   419,   113,   385,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,   386,     0,   420,   421,     0,     0,   387,   388,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,     0,     0,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   384,
       0,     0,     0,     0,     0,     0,   392,   393,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,   386,   395,     0,   104,     0,     0,   387,   388,     0,
       0,     0,     0,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,     0,
       0,   179,   180,   105,   106,   389,   107,     0,   390,   181,
     182,     0,     0,   108,   109,   110,   111,   112,   183,   113,
     184,     0,   114,   115,   116,   117,   118,     0,     0,   396,
     119,   120,     0,     0,     0,   397,   392,   393,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,   396,
     114,   115,   116,   117,   118,   397,     0,     0,   119,   120,
       0,     0,     0,   420,   421,   398,     0,  1707,     0,   482,
       0,   423,     0,     0,     0,   278,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,  1428,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,   420,   421,     6,     0,     0,     0,   482,
     594,   423,     0,     0,   595,   278,  1540,  1541,     0,  1542,
       0,     0,  1543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   596,     0,     0,   597,  1544,     0,     0,     0,
       0,     0,  1545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   598,     0,     0,     0,
       0,     0,   599,   600,   601,     0,   602,   603,   604,     0,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1546,     0,     0,     0,     0,     0,     0,     0,
     606,     0,   607,     0,     0,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1547,     0,     0,     0,
       0,     0,     0,   608,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1548,     0,     0,
     609,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   610,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1549,   106,     0,   107,   104,     0,     0,
       0,     0,     0,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,  1550,   116,   117,   118,   611,   612,     0,
     119,   120,     0,     0,     0,     0,   105,   106,     0,   107,
       0,     0,     0,     0,   613,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,   119,   120
};

static const short yycheck[] =
{
       9,    54,     4,    12,     4,    63,    16,    16,    55,    18,
       0,    56,   103,    62,   149,   286,   255,   139,   140,    66,
     354,   362,   333,    68,   495,   331,    84,   362,    62,   139,
     140,   509,   358,    69,    93,    88,    90,   422,   709,    72,
     563,   674,    89,    52,    53,    54,    55,    56,    57,    96,
      61,   521,  1040,    62,    63,    64,    92,    66,   470,    68,
      69,    94,    71,    72,   770,  1261,   694,   939,    87,   379,
     367,    80,    82,    82,    83,    84,   646,    86,    87,    88,
      89,    91,    91,    92,    93,    94,   131,    96,   686,   760,
     553,   264,   555,   326,   153,   723,   130,  1034,   132,   831,
     563,   274,   307,  1040,   151,   977,  1034,   152,  1034,  1034,
     862,   280,  1040,  1067,  1040,  1040,   987,   639,  1034,   708,
     763,   130,   131,   132,  1040,   144,   759,   727,  1085,   219,
    1090,   157,  1041,  1204,   766,   144,     8,   228,     6,  1254,
     149,  1050,   151,   152,   153,  1379,   804,    23,   157,   792,
      10,     8,     8,  1175,     8,     8,     8,   476,   801,  1181,
    1394,   727,    50,    26,  1073,    15,   652,   652,    31,    50,
      64,   657,   307,  1468,   270,  1259,    46,   188,  1309,    64,
      66,    49,    20,   422,    29,  1300,     6,   101,   140,     8,
       8,    24,    71,   123,  1196,  1554,  1198,  1689,    53,    54,
       3,  1687,   213,   529,   133,   539,   540,   541,   542,   193,
       6,   160,   855,  1698,  1699,     6,   261,   514,   151,  1091,
      29,   311,   563,   257,   315,   270,    11,   149,   563,    33,
      94,  1635,   148,  1637,   164,   239,     5,   163,   291,   103,
      98,   201,    46,  1735,   166,   249,     9,    16,   257,     8,
     295,  1737,   261,    49,  1739,   264,   265,   186,   736,   565,
     132,   270,   588,    48,   190,   274,   124,    26,   184,   202,
       4,   103,    31,   307,  1231,   132,   132,    80,   132,   132,
     132,   211,   291,    17,   594,   130,   295,  1779,  1692,   273,
     463,   124,   890,   891,  1780,    58,  1411,   176,   332,  1287,
      53,    54,     8,     8,   183,   339,    69,   341,   614,   343,
      47,    11,   346,   132,   132,     8,    47,   364,    52,  1811,
      54,   273,   375,   332,   238,     4,    26,  1812,   373,   365,
     339,    31,   341,    26,   343,   470,   934,   346,    31,  1293,
     185,  1472,   197,   163,   355,   978,   357,  1218,    48,  1433,
    1287,  1373,  1374,    26,   817,   364,   365,   202,    31,  1287,
     371,  1287,  1287,   181,   373,  1008,   375,  1276,   839,   318,
     970,  1287,   163,    52,   212,    26,   185,   379,   272,  1783,
      31,   477,   982,   983,   272,  1581,   272,  1045,  1046,   274,
    1685,   272,   878,    26,   264,  1397,   882,   882,    31,   263,
     276,   269,   274,   674,   970,  1764,   269,   270,    47,  1052,
     173,   730,   272,   458,   423,   462,   982,   274,   274,   157,
     274,   274,   274,   273,   469,    66,   132,   132,   473,   476,
     164,   263,   477,   102,  1208,  1033,   470,  1095,    26,   191,
    1799,  1800,    14,    31,   197,    84,    94,    12,  1519,   458,
      47,   188,   691,   462,   463,   481,   200,   188,   580,    12,
     469,   700,  1696,  1095,   473,  1824,   737,   476,   477,   559,
     200,   148,   481,  1205,   780,   181,  1835,   783,  1593,  1838,
      84,  1677,   491,   148,  1599,   164,    59,    84,   759,  1446,
    1742,  1743,  1514,   799,   272,    26,   182,  1712,    25,   277,
      31,    19,   254,   182,    65,  1465,   175,  1464,   267,   268,
     269,   270,    66,  1111,  1112,  1424,  1114,  1115,   824,    89,
    1429,     3,  1431,   534,   204,  1479,   191,   581,   272,   563,
    1236,  1101,  1306,  1404,  1730,   273,  1125,   216,  1753,   111,
     630,   182,   272,    47,   114,   718,   779,  1299,    75,   188,
    1282,   756,   561,   198,   563,   866,   853,  1466,   567,   207,
     208,    26,  1084,   975,   733,   875,    31,   267,   268,   269,
     270,   136,   583,  1769,   267,   268,   269,   270,  1793,  1831,
      84,  1796,   109,   136,   188,   146,   272,   177,   204,   254,
     163,   123,   594,   273,   267,   268,   269,   270,    80,   279,
     127,   274,   172,  1818,   638,    26,    23,  1669,    19,   204,
      31,   271,  1484,   204,   204,  1215,   267,   268,   269,   270,
     742,   756,  1531,   274,   635,   636,   187,   272,  1537,   638,
      16,   281,    88,    10,   267,   268,   269,   270,   647,   289,
      46,   274,   204,   252,  1242,   647,    26,   647,   204,   153,
    1712,    31,  1420,   775,   776,  1298,    18,   273,   677,   163,
       6,   237,   194,   279,    62,    63,  1264,   123,   677,   245,
       6,   155,    34,   268,    91,    86,   710,  1586,   268,   267,
     268,   269,   270,   730,   188,    26,   274,  1021,   279,   279,
      31,  1753,    98,   702,  1603,   151,    82,     7,   154,  1467,
     709,   710,  1330,    49,   713,  1767,   751,   978,   753,   718,
     272,    47,   268,  1419,   170,    26,  1704,    79,   727,   204,
      31,   730,   756,    59,   778,   204,   735,   204,   762,   738,
       6,  1793,   766,  1039,  1796,    83,   267,   268,   269,   270,
     196,   177,   751,   274,   753,  1051,   108,   177,    84,   148,
     204,   760,  1086,   762,   231,   789,  1818,   766,   769,   770,
     771,    68,  1395,  1034,   818,    26,  1072,  1704,   204,  1040,
      31,    68,   120,    49,   204,  1684,  1704,   231,  1704,  1704,
     789,   180,   816,   817,  1693,   133,   959,  1098,  1704,   962,
     147,   268,  1262,   124,   848,   849,   195,   851,  1784,  1785,
    1185,     6,   267,   268,   269,   270,   271,   816,   817,   854,
     273,  1409,   151,   175,  1285,   146,   129,   153,   829,   864,
    1291,   823,   153,   272,   272,  1734,   274,   163,    67,   206,
      26,   170,   268,     0,   836,    31,  1359,   273,   268,  1224,
     975,     8,    81,   129,    49,   854,   267,   268,   269,   270,
    1161,   131,   188,   274,  1840,   864,   187,    26,   235,   236,
       4,   238,    31,     7,   239,   240,   105,   876,   245,   246,
     247,   248,   249,   875,   251,     6,   129,   254,   255,   256,
     257,   258,   272,   129,   274,   262,   263,   267,   268,   269,
     270,    26,    83,    26,   274,   182,    31,    64,    31,  1669,
      67,    67,   240,    24,   963,   877,   272,    74,    52,   881,
      77,   277,    33,   960,    81,    81,   272,   164,    49,    86,
     159,   277,    89,    89,   273,   961,   267,   268,   269,   270,
      26,   964,   211,   274,   213,    31,    20,  1208,   105,   105,
      26,   975,  1712,   207,   208,    31,  1185,   272,  1070,   274,
     959,   960,   961,   962,   963,   964,   267,   268,   269,   270,
      64,   970,   129,   274,   170,   132,   133,    25,  1274,    40,
     272,   207,   274,   982,   983,   142,    10,  1303,   987,   272,
      51,   274,   272,  1753,   274,  1224,    62,    58,    46,   272,
      64,   274,   159,   159,   272,    26,   274,  1767,    83,  1305,
      31,    82,   272,   124,   274,    26,   267,   268,   269,   270,
      31,   206,    83,   274,   181,   198,  1287,    75,  1359,   186,
     272,   148,   274,  1793,  1359,   146,  1796,  1081,   203,   204,
     205,   198,   153,   200,    86,  1306,    94,   137,   138,  1517,
    1638,    26,  1520,  1097,   273,   103,    31,    66,  1818,  1103,
     272,   109,   274,  1062,  1088,    64,   231,   232,   271,   272,
    1658,  1095,  1064,  1661,  1064,   272,   187,   274,  1077,   127,
     148,   267,   268,   269,   270,     0,  1539,   148,    10,  1088,
     129,     0,  1416,     8,   129,   272,  1095,   274,   182,     8,
    1225,    26,   107,  1203,  1204,  1205,    31,  1768,   267,   268,
     269,   270,   252,   272,   271,   272,   196,   274,   272,   180,
     274,   180,   272,   184,   274,   673,  1238,   675,  1240,   130,
    1174,   679,   680,   681,  1395,    26,   272,  1605,   274,  1435,
      31,   271,   272,    26,   267,   268,   269,   270,    31,    64,
     163,   274,    67,   262,  1196,    64,  1198,   188,    67,    74,
     200,   262,    77,    33,   157,    74,    81,   102,    77,   215,
      26,    86,    81,   272,    89,    31,   264,    86,  1474,   118,
      89,   267,   268,   269,   270,    32,   273,    10,   274,  1301,
     105,   267,   268,   269,   270,    86,   105,  1196,   274,  1198,
     273,   273,   272,    86,   139,   207,   141,   207,   255,   207,
      66,  1203,  1204,   273,   129,   263,  1215,   132,   133,  1218,
     129,   273,   273,   132,   133,   278,  1225,   142,   273,   164,
     277,   182,   273,   142,     6,  1236,   273,   273,   276,   286,
     273,   273,  1830,   273,   159,   273,   267,   268,   269,   270,
     159,   273,   278,   274,   189,    26,   267,   268,   269,   270,
      31,    66,   148,   274,   272,    66,   181,   174,   272,    26,
     317,   186,   181,  1741,    31,    47,   272,   186,   264,   273,
      10,  1406,   272,   198,   206,   200,    57,    59,  1280,   198,
    1280,   200,   267,   268,   269,   270,   278,   128,   251,   274,
      57,    73,    94,  1428,  1600,    94,    26,    94,   272,    24,
      24,    31,    84,   235,   236,  1316,   238,    46,    10,  1320,
     182,   216,   143,   245,   246,   247,   248,   249,   182,   251,
     143,   204,   254,   255,   256,   257,   258,    23,    94,   145,
     262,   263,   267,   268,   269,   270,    66,   193,   193,   274,
     273,    95,   273,   273,   273,    64,   271,   272,   276,   274,
      10,   129,   271,   272,   157,   274,   272,   128,    67,   416,
     135,   273,   273,   420,   421,   422,   267,   268,   269,   270,
     271,   153,    81,   197,   267,   268,   269,   270,  1427,   274,
     210,   163,   182,    63,   273,    66,   198,    66,  1397,   194,
      10,  1436,   273,  1427,  1428,  1404,   105,  1406,   248,    67,
     204,   267,   268,   269,   270,    40,   188,    23,  1419,  1519,
      20,   273,  1534,    81,    46,    30,    51,    90,  1427,  1554,
     129,    90,    24,    58,   206,   482,   132,  1436,    51,   486,
     164,   132,   132,  1704,   162,    58,    71,   105,    46,    62,
      63,   164,   163,   102,   273,   273,    20,   163,    71,    10,
     159,   273,   272,   235,   236,   274,   238,    66,   272,    26,
      83,   129,   272,   245,   246,   247,   248,   249,   131,   251,
    1709,   113,   254,   255,   256,   257,   258,   272,    66,   272,
     262,   263,  1540,   273,    74,   272,   267,   268,   269,   270,
      12,   159,    45,  1584,   273,   247,   273,   132,    10,   275,
     267,   268,   269,   270,   168,   274,   274,  1509,    10,   274,
     274,   200,   186,   148,   897,   140,   899,  1519,   901,   902,
    1554,   904,   905,   906,    55,   148,   238,    55,   274,   274,
     165,  1540,  1585,     8,  1669,  1580,   273,   267,   268,   269,
     270,   273,   165,    98,   273,   180,   273,   273,    54,   184,
     181,   273,   187,  1543,    73,   273,   273,   180,   273,   273,
     195,   184,   273,   273,   134,   273,  1575,   274,   273,   273,
     273,  1580,   195,   272,   134,   134,  1585,  1712,    89,   214,
      20,    77,   168,   137,   136,   192,    17,  1678,   153,   132,
     132,   214,   132,   132,  1604,  1604,   132,   132,  1609,   274,
     657,    24,  1613,   274,  1651,   218,   219,   220,   221,   222,
     223,   224,   225,   274,   163,   273,    54,   674,  1753,   274,
     274,   678,   274,   181,   131,   182,   142,    26,   685,  1764,
      83,    47,  1767,   690,   691,    84,    84,   188,   188,    47,
     274,   274,  1651,   700,   701,    66,   703,   704,   705,   706,
     707,   708,    98,   131,   117,   117,   274,   274,  1793,  1717,
     274,  1796,   274,   274,  1799,  1800,    86,    54,   274,   274,
     274,   274,    86,   274,   274,   274,   274,  1687,  1687,   274,
     737,   274,   274,  1818,   273,   272,   271,   271,    98,  1824,
     273,   271,   271,  1703,  1703,   271,   271,   274,   271,   271,
    1835,    90,   759,  1838,    86,   274,   197,   764,  1717,  1718,
     114,   273,   129,    89,   278,   156,   272,  1719,   129,  1719,
     129,   181,    50,   271,   271,    47,   272,  1737,  1737,   274,
    1764,   188,   133,     6,   271,   273,     9,  1790,  1748,  1748,
      13,   274,   274,    39,    50,   209,   272,   274,    53,    22,
       0,   272,     0,   262,  1201,    12,   274,   265,   718,  1768,
     642,  1101,    35,  1098,   867,  1799,  1800,  1087,    41,    42,
    1780,  1780,  1455,  1024,   762,   588,  1088,  1787,  1787,   313,
     800,  1790,  1454,  1648,  1433,   329,  1095,  1728,   359,  1435,
    1824,  1731,   322,  1732,  1771,   789,    69,  1255,  1709,    72,
     341,  1835,  1765,  1687,  1838,  1656,  1667,  1816,  1010,  1412,
    1233,   334,   341,   569,  1823,  1289,   846,   327,   627,    13,
     299,   346,   838,   501,   751,   497,   764,   100,   101,   142,
     737,   987,  1639,  1842,  1529,   892,   893,   894,  1748,   112,
     897,   898,   899,   116,   901,   902,   903,   904,   905,   906,
     907,   908,  1787,   910,  1842,   912,     0,     0,   915,   916,
     917,   918,   919,   920,   509,     0,   726,   979,  1139,  1183,
    1244,  1575,   567,  1077,     7,   932,  1823,    86,   935,   738,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   171,    -1,
     173,    -1,    -1,    36,    -1,    -1,   179,    -1,    -1,    -1,
      43,    -1,    45,    -1,    47,    -1,   189,    -1,    -1,    52,
      -1,   978,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    19,    -1,   158,    -1,    23,   161,    -1,
     163,   164,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    18,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    26,   188,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,  1125,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
    1137,  1138,    -1,  1140,  1141,    59,    -1,    -1,    -1,    85,
      -1,  1148,    -1,    -1,    -1,    91,    92,    93,    70,    95,
      96,    97,    -1,    99,   237,    -1,    -1,    79,  1165,    -1,
      84,    -1,    -1,    -1,    -1,    87,  1173,    -1,  1175,   252,
     253,    -1,    -1,   119,  1181,   121,  1183,  1184,  1185,  1186,
    1187,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,    -1,
      -1,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,
     122,  1208,   124,   125,   126,    -1,   152,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1224,    -1,    -1,
      -1,    35,    -1,   169,    -1,    -1,    -1,    41,    42,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,
    1257,    -1,    -1,   175,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   206,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,   243,   112,  1306,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
    1327,   245,   246,   247,   248,   249,    -1,   251,    -1,  1336,
     254,   255,   256,   257,   258,    -1,  1343,    -1,   262,   263,
      -1,    -1,   264,   265,   266,   267,   268,   269,   270,    -1,
      -1,    -1,   274,    -1,    33,    -1,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,  1373,  1374,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1395,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,  1408,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,    -1,    74,  1328,  1329,   273,
      -1,   275,    -1,  1334,  1335,   279,  1337,  1338,    -1,   206,
      -1,    -1,    -1,    -1,  1345,  1346,  1347,  1348,    -1,  1350,
    1351,  1352,  1353,  1354,  1355,  1492,  1493,     6,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    22,    -1,    -1,    -1,  1514,   245,   246,
     247,   248,   249,    -1,   251,    -1,    35,   254,   255,   256,
     257,   258,    41,    42,    -1,   262,   263,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,    -1,    -1,  1546,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,   189,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,   100,   101,   262,   263,    -1,    -1,    -1,   206,    -1,
      -1,    19,    -1,   112,    -1,    23,    -1,   116,    -1,    -1,
      -1,  1608,    30,    -1,    -1,    -1,    -1,    -1,    -1,  1616,
      -1,    -1,    -1,    41,    -1,    -1,    44,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,  1639,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,  1650,   262,   263,    -1,  1654,    -1,    -1,
      -1,    -1,   171,    -1,   173,    -1,    -1,    85,    -1,    -1,
     179,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
     189,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,  1709,    -1,  1711,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,   152,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,   169,    -1,   262,   263,    -1,    -1,    -1,   267,   268,
       6,    -1,    -1,     9,   273,    -1,   275,    13,    -1,    -1,
     279,    -1,    -1,   191,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,   226,   227,
      -1,    31,    -1,    -1,    34,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    69,    -1,   243,    72,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,   100,   101,    -1,    -1,     6,    79,
      -1,     9,    -1,    -1,    -1,    13,   112,    87,    -1,    -1,
     116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    35,   108,    -1,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,   100,   101,    -1,   175,    -1,   203,   204,   205,
     206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,   267,   268,   171,    -1,   173,    -1,   273,    -1,   275,
      -1,   179,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,   264,   265,   266,   267,   268,   269,
     270,    -1,    -1,    42,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,   267,
     268,     6,   206,    -1,     9,   273,    -1,   275,    13,    -1,
      -1,   279,    -1,    -1,    -1,    -1,   189,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,   235,   236,   206,   238,    -1,    41,    42,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,   235,   236,    69,   238,    -1,    72,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   100,   101,   206,    -1,     6,
      -1,    -1,     9,    -1,    -1,    -1,    13,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    -1,
     269,    -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   217,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    41,   241,   242,    44,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      41,    -1,   267,   268,   171,    -1,   173,    -1,   273,    -1,
     275,    -1,   179,    -1,   279,    -1,    -1,    -1,    -1,    85,
      -1,    -1,   189,    -1,    -1,    91,    92,    93,    69,    95,
      96,    97,    -1,    99,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,   119,    -1,   121,    -1,    98,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,   152,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
     267,   268,     6,   169,    -1,     9,   273,    -1,   275,    13,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    35,   173,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
     226,   227,   203,   204,   205,    69,   207,   208,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,
      -1,   206,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,    -1,   100,   101,    -1,    -1,
     241,   242,    -1,    -1,    -1,    -1,    13,    -1,   112,   250,
     235,   236,   116,   238,    -1,    22,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,   268,    35,   254,
     255,   256,   257,   258,    41,    42,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,   171,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   163,    -1,   262,   263,
     167,    -1,    89,   267,   268,    -1,   173,    -1,    -1,   273,
      -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,   172,    -1,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,    -1,    13,    -1,
     267,   268,    -1,    -1,    -1,    -1,   273,    22,   275,    -1,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      35,    -1,    -1,    38,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    13,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    22,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   235,   236,
      -1,   238,    -1,    41,    42,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    60,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    69,   167,    -1,    72,    -1,   273,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,   217,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,   167,
      -1,    -1,   267,   268,    -1,   173,    -1,    -1,   273,    -1,
     275,   179,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    13,   244,   245,   246,   247,
     248,   249,   250,   251,    22,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    35,   206,   267,
     268,    -1,    -1,    41,    42,   273,    -1,   275,    -1,    -1,
      -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    69,    -1,    -1,    72,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,   273,   206,   275,    -1,    -1,
      -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,    22,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,   235,   236,    -1,   238,    -1,
      41,    42,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    60,
      -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    69,   167,
      -1,    72,    -1,   273,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,   167,    -1,    -1,   267,
     268,    -1,   173,    -1,    -1,   273,    -1,   275,   179,    -1,
      -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    13,   244,   245,   246,   247,   248,   249,   250,
     251,    22,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,   206,    35,    -1,   267,   268,    -1,    -1,
      41,    42,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,   238,    -1,    -1,    69,    -1,
      -1,    72,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   275,    -1,   206,    -1,    -1,    -1,    -1,    13,
      -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,   235,   236,    -1,   238,    41,    42,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    69,    -1,    -1,    72,   271,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    17,    -1,   254,   255,   256,   257,   258,    -1,   163,
      -1,   262,   263,    -1,    -1,    -1,   267,   268,   269,   173,
      -1,    -1,   273,    -1,   275,   179,    -1,    -1,   279,    -1,
      -1,    47,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    61,    -1,    -1,    64,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,   217,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    13,
     244,   245,   246,   247,   248,   249,   250,   251,    22,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,   163,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,   188,    -1,    98,    -1,   100,   101,    22,    -1,
      -1,   197,    -1,   199,    -1,    -1,    -1,    -1,   112,    -1,
     206,    35,   116,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,   262,   263,
      -1,    -1,    74,   267,   268,   189,    -1,    -1,    -1,   273,
      -1,   275,    -1,   197,    -1,   279,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    13,
     244,   245,   246,   247,   248,   249,   250,   251,    22,    -1,
     254,   255,   256,   257,   258,    -1,   148,    -1,   262,   263,
      -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,    -1,    -1,    69,    -1,   189,    72,    -1,
      -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    13,
      -1,    -1,   214,    -1,    -1,    -1,   100,   101,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    35,   116,   235,   236,    -1,   238,    41,    42,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    98,   179,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    13,
     244,   245,   246,   247,   248,   249,   250,   251,    22,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   204,    -1,   206,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    35,   116,   235,   236,    -1,   238,    41,    42,    -1,
      -1,    -1,   206,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    69,   268,    -1,    72,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,   100,   101,   262,   263,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,   273,
     274,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    13,
     244,   245,   246,   247,   248,   249,   250,   251,    22,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     206,    35,    -1,   267,   268,    -1,    -1,    41,    42,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    13,
      -1,    -1,    -1,   269,    -1,    -1,   100,   101,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,   273,
      -1,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    13,
     244,   245,   246,   247,   248,   249,   250,   251,    22,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    35,   116,    -1,   206,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,    -1,
      -1,   233,   234,   235,   236,    69,   238,    -1,    72,   241,
     242,    -1,    -1,   245,   246,   247,   248,   249,   250,   251,
     252,    -1,   254,   255,   256,   257,   258,    -1,    -1,   173,
     262,   263,    -1,    -1,    -1,   179,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   189,    -1,   271,    -1,   273,
      -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    17,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,    47,    -1,    -1,    -1,   273,
      19,   275,    -1,    -1,    23,   279,    58,    59,    -1,    61,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    44,    78,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,    -1,
     169,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,   206,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,   226,   227,    -1,
     262,   263,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263
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
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
    break;
case 318:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
    break;
case 319:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
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
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
    break;
case 337:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;
case 338:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;
case 339:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 340:
{ yyval = NULL; }
    break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 343:
{ yyval = make_node (nod_on_error, e_err_count,
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
case 350:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 351:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 352:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 353:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
    break;
case 354:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 356:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 357:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 358:
{ yyval = make_list (yyvsp[0]); }
    break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 363:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 364:
{ yyval = yyvsp[-1]; }
    break;
case 365:
{ lex.beginning = lex_position(); }
    break;
case 366:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;
case 367:
{ lex.beginning = lex.last_token; }
    break;
case 368:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
    break;
case 369:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 370:
{ yyval = 0; }
    break;
case 371:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 372:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 373:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 374:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 375:
{ yyval = NULL; }
    break;
case 376:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
    break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
    break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
    break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
    break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
    break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
    break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
    break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 395:
{ yyval = NULL; }
    break;
case 396:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 397:
{ yyval = yyvsp[0]; }
    break;
case 398:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 399:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 400:
{ yyval = yyvsp[0]; }
    break;
case 401:
{ yyval = yyvsp[0]; }
    break;
case 402:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 403:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 404:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 405:
{ yyval = yyvsp[0]; }
    break;
case 406:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 408:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 409:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
    break;
case 410:
{ yyval = yyvsp[0]; }
    break;
case 411:
{ yyval = yyvsp[0]; }
    break;
case 412:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 413:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 414:
{ yyval = yyvsp[0]; }
    break;
case 415:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 417:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 418:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 419:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 420:
{ yyval = yyvsp[0]; }
    break;
case 421:
{ yyval = yyvsp[0]; }
    break;
case 422:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 423:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 424:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 425:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 449:
{ yyval = NULL; }
    break;
case 450:
{ yyval = NULL; }
    break;
case 451:
{ yyval = NULL; }
    break;
case 452:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 453:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 454:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 455:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 456:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 457:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 458:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 459:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;
case 461:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 462:
{ yyval = yyvsp[0]; }
    break;
case 463:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 464:
{ yyval = yyvsp[0]; }
    break;
case 465:
{ yyval = yyvsp[0]; }
    break;
case 466:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 467:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 468:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 469:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 471:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 472:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 474:
{ yyval = NULL; }
    break;
case 476:
{ yyval = NULL; }
    break;
case 477:
{ yyval = yyvsp[0]; }
    break;
case 478:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 479:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 480:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 481:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 482:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 483:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 484:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 485:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 486:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 487:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 488:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 489:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 494:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
    break;
case 495:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
    break;
case 497:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 498:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 499:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 505:
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
case 506:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 507:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 508:
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
case 509:
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
case 510:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 513:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;
case 517:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
    break;
case 518:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 519:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
    break;
case 520:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 521:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 522:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;
case 538:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 539:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 540:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
    break;
case 541:
{         
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
			    yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
			    {
			    if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
case 542:
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
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
case 545:
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
case 546:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 549:
{ yyval = yyvsp[-1]; }
    break;
case 550:
{ yyval = 0; }
    break;
case 554:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 555:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 556:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 560:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 561:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 562:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 563:
{ yyval = 0; }
    break;
case 564:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 567:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 568:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 571:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 572:
{ yyval = NULL; }
    break;
case 574:
{ yyval = NULL; }
    break;
case 575:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 577:
{ yyval = NULL; }
    break;
case 579:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 584:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 585:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 586:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 587:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 588:
{ yyval = yyvsp[0];}
    break;
case 590:
{ yyval = yyvsp[0];}
    break;
case 591:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 592:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 596:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 597:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 598:
{ yyval = 0; }
    break;
case 599:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 600:
{ yyval = (DSQL_NOD) NOD_SHARED; }
    break;
case 601:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;
case 602:
{ yyval = (DSQL_NOD) 0; }
    break;
case 603:
{ yyval = (DSQL_NOD) NOD_READ; }
    break;
case 604:
{ yyval = (DSQL_NOD) NOD_WRITE; }
    break;
case 606:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 607:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 608:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 609:
{ yyval = 0; }
    break;
case 611:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 612:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 613:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 614:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 615:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 616:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 617:
{ yyval = make_list (yyvsp[0]); }
    break;
case 618:
{ yyval = 0; }
    break;
case 620:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 621:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 622:
{ yyval = 0; }
    break;
case 623:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 624:
{ yyval = 0; }
    break;
case 625:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 626:
{ yyval = 0; }
    break;
case 627:
{ yyval = yyvsp[-1]; }
    break;
case 628:
{ yyval = 0; }
    break;
case 629:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 630:
{ yyval = 0; }
    break;
case 631:
{ yyval = yyvsp[0]; }
    break;
case 632:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 633:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 634:
{ yyval = 0; }
    break;
case 635:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 636:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 637:
{ lex.limit_clause = true; }
    break;
case 638:
{ lex.limit_clause = false; }
    break;
case 639:
{ lex.first_detection = true; }
    break;
case 640:
{ lex.first_detection = false; }
    break;
case 641:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 642:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;
case 643:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;
case 644:
{ yyval = 0; }
    break;
case 645:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 646:
{ yyval = yyvsp[-2]; }
    break;
case 647:
{ yyval = yyvsp[-1]; }
    break;
case 648:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 649:
{ yyval = yyvsp[-1]; }
    break;
case 650:
{ yyval = yyvsp[0]; }
    break;
case 651:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 652:
{ yyval = 0; }
    break;
case 653:
{ yyval = make_list (yyvsp[0]); }
    break;
case 654:
{ yyval = 0; }
    break;
case 656:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 658:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 659:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 660:
{ yyval = make_list (yyvsp[0]); }
    break;
case 662:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 665:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 666:
{ yyval = yyvsp[-1]; }
    break;
case 667:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
    break;
case 668:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 669:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 670:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 671:
{ yyval = NULL; }
    break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 677:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 678:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 679:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 680:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 681:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 682:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 683:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 684:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 685:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 686:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
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
{ yyval = yyvsp[0]; }
    break;
case 693:
{ yyval = 0; }
    break;
case 694:
{ yyval = yyvsp[0]; }
    break;
case 695:
{ yyval = 0; }
    break;
case 696:
{ yyval = yyvsp[0]; }
    break;
case 697:
{ yyval = 0; }
    break;
case 698:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 699:
{ yyval = 0; }
    break;
case 700:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 701:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 702:
{ yyval = 0; }
    break;
case 703:
{ yyval = 0; }
    break;
case 705:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 706:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 709:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 710:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 711:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 712:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
    break;
case 714:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 715:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 716:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 718:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 721:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 722:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 723:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 726:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 727:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 729:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 730:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 733:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 734:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 736:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 740:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 741:
{ yyval = yyvsp[0]; }
    break;
case 743:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 746:
{ yyval = NULL; }
    break;
case 747:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 749:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 751:
{ yyval = NULL; }
    break;
case 752:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 756:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 757:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 758:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 764:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 765:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 768:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 773:
{ yyval = yyvsp[-1]; }
    break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 785:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 789:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 794:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 795:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 796:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 797:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 798:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 799:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 800:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 801:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 802:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 803:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 804:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 805:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 806:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 807:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 808:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 811:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 813:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 815:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 817:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 819:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 821:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 823:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 825:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 826:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 829:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 836:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 837:
{ yyval = yyvsp[-1]; }
    break;
case 838:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 839:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 849:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 850:
{ yyval = yyvsp[0]; }
    break;
case 851:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 852:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 854:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 855:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 856:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 857:
{ yyval = yyvsp[-1]; }
    break;
case 858:
{ yyval = yyvsp[-1]; }
    break;
case 862:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 863:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 864:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
    break;
case 865:
{ yyval = yyvsp[0]; }
    break;
case 866:
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
case 867:
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
case 868:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 869:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 871:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 873:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 880:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;
case 881:
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
case 882:
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
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 884:
{ yyval = make_parameter (); }
    break;
case 885:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 886:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 887:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 893:
{ yyval = yyvsp[0]; }
    break;
case 894:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 896:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 897:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 898:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 899:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 902:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 903:
{ yyval = yyvsp[0];}
    break;
case 908:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 909:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 910:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 911:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 915:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 916:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 917:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 918:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 919:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
    break;
case 920:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 921:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
    break;
case 922:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
    break;
case 923:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 924:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 925:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 926:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 929:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 930:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 933:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 934:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 936:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 937:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 938:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 939:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 940:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 954:
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
static BOOLEAN long_int (
    DSQL_NOD		string,
    SLONG	*long_value)
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
			return FALSE;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return TRUE;
}
#endif

static DSQL_FLD make_field (
    DSQL_NOD		field_name)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;
       
temp_file = FB_NEW(*tdsql->tsql_default) fil;

return temp_file;
}


static DSQL_NOD make_list (
    DSQL_NOD		node)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;

node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
node->nod_type = nod_parameter;
node->nod_line = (USHORT) lex.lines_bk;
node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
node->nod_count = 1;
node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

return node;
}


static DSQL_NOD make_node (
    NOD_TYPE	type,
    int		count,
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
TSQL    tdsql;

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


static DSQL_NOD make_flag_node (
    NOD_TYPE	type,
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
TSQL    tdsql;

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
    DSQL_debug = level;
    if (level >> 8)
        *yydeb = level >> 8;
    /* CVC: I added this code form Mike Nordell to see the output from internal
       operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
       When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
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
static BOOLEAN short_int (
    DSQL_NOD		string,
    SLONG	*long_value,
    SSHORT	range)
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
		return FALSE;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return FALSE;
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

	BOOLEAN return_value;

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

static void stack_nodes (
    DSQL_NOD		node,
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
DSQL_NOD     curr_node, next_node, start_chain, end_chain, save_link;

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
    while ( TRUE)
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

STR	delimited_id_str;

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

    BOOLEAN have_error     = FALSE;	/* syntax error or value too large */
    BOOLEAN have_digit     = FALSE;	/* we've seen a digit              */
    BOOLEAN have_decimal   = FALSE;	/* we've seen a '.'                */
    BOOLEAN have_exp       = FALSE;	/* digit ... [eE]                  */
    BOOLEAN have_exp_sign  = FALSE; /* digit ... [eE] {+-]             */
    BOOLEAN have_exp_digit = FALSE; /* digit ... [eE] ... digit        */
    UINT64	number         = 0;
    UINT64	limit_by_10    = MAX_SINT64 / 10;

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
	      have_error = TRUE;
	      break;
	    }
	else if (have_exp)
	    {
	    /* We've seen e or E, but nothing beyond that. */
	    if ( ('-' == c) || ('+' == c) )
		have_exp_sign = TRUE;
	    else if ( classes [c]  & CHR_DIGIT )
		/* We have a digit: we haven't seen a sign yet,
		   but it's too late now. */
		have_exp_digit = have_exp_sign  = TRUE;
	    else
		{
		/* end of the token */
		have_error = TRUE;
		break;
		}
	    }
	else if ('.' == c)
	    {
	    if (!have_decimal)
		have_decimal = TRUE;
	    else
		{
		have_error = TRUE;
		break;
		}
	    }
	else if (classes [c] & CHR_DIGIT)
	  {
	    /* Before computing the next value, make sure there will be
	       no overflow.  */

	    have_digit = TRUE;

	    if (number >= limit_by_10)
		/* possibility of an overflow */
		if ((number > limit_by_10) || (c > '8'))
		    {
		    have_error = TRUE;
		    break;
		    }
	    number = number * 10 + (c - '0');
	  }
	else if ( (('E' == c) || ('e' == c)) && have_digit )
	    have_exp = TRUE;
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
    sym = HSHD_lookup (NULL_PTR, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
					return sym->sym_keyword;
				}
			} /* else fall down and return token as SYMBOL */
		} else {
			yylval = (DSQL_NOD) sym->sym_object;
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
    sym = HSHD_lookup (NULL_PTR, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
	gds_arg_gds, gds_command_end_err,    /* Unexpected end of command */
	0);
else
    {
    ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
 	gds_arg_gds, gds_dsql_token_unk_err, 
	gds_arg_number, (SLONG) lex.lines, 
	gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
	    /* Token unknown - line %d, char %d */
 	gds_arg_gds, gds_random, 
	gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
 	0);
    }
}


static void yyabandon (
    SSHORT      sql_code,
    ISC_STATUS      error_symbol)
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
