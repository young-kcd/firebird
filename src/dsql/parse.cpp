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



#define	YYFINAL		1843
#define	YYFLAG		-32768
#define	YYNTBASE	278

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 515 ? yytranslate[x] : 693)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     271,   272,   267,   265,   270,   266,   276,   268,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   273,   269,
     263,   262,   264,   277,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   274,     2,   275,     2,     2,     2,     2,     2,     2,
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
     256,   257,   258,   259,   260,   261
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
     543,   547,   551,   553,   555,   562,   566,   568,   572,   574,
     578,   581,   583,   584,   587,   591,   593,   594,   596,   598,
     601,   605,   610,   611,   613,   616,   617,   619,   621,   627,
     633,   637,   640,   641,   643,   647,   649,   651,   658,   662,
     665,   672,   675,   677,   680,   683,   686,   687,   689,   691,
     694,   695,   698,   699,   701,   703,   705,   707,   709,   711,
     712,   714,   716,   719,   722,   725,   731,   733,   736,   740,
     743,   746,   747,   749,   751,   753,   755,   759,   764,   773,
     778,   779,   786,   788,   790,   793,   796,   797,   801,   805,
     807,   810,   813,   816,   825,   834,   843,   852,   856,   857,
     860,   861,   863,   867,   870,   872,   873,   875,   878,   885,
     887,   888,   891,   893,   895,   897,   901,   903,   906,   907,
     909,   912,   915,   918,   920,   922,   924,   926,   928,   930,
     933,   935,   937,   940,   942,   944,   946,   949,   952,   954,
     958,   963,   966,   973,   978,   980,   982,   990,   999,  1006,
    1015,  1022,  1027,  1028,  1033,  1036,  1038,  1042,  1043,  1046,
    1051,  1052,  1054,  1056,  1060,  1064,  1071,  1074,  1077,  1081,
    1082,  1084,  1087,  1092,  1094,  1098,  1101,  1104,  1107,  1109,
    1114,  1122,  1130,  1132,  1134,  1138,  1143,  1151,  1154,  1156,
    1160,  1162,  1164,  1171,  1175,  1176,  1177,  1178,  1179,  1183,
    1184,  1194,  1204,  1206,  1208,  1209,  1212,  1214,  1216,  1218,
    1220,  1222,  1226,  1230,  1234,  1238,  1242,  1246,  1252,  1258,
    1264,  1270,  1276,  1282,  1285,  1286,  1291,  1294,  1298,  1302,
    1305,  1308,  1312,  1316,  1319,  1323,  1330,  1332,  1335,  1340,
    1344,  1347,  1350,  1353,  1356,  1360,  1362,  1366,  1370,  1374,
    1377,  1380,  1385,  1390,  1396,  1398,  1400,  1402,  1404,  1406,
    1408,  1410,  1412,  1414,  1416,  1418,  1420,  1422,  1424,  1426,
    1428,  1430,  1432,  1434,  1436,  1438,  1440,  1442,  1444,  1446,
    1449,  1452,  1455,  1457,  1459,  1461,  1462,  1465,  1468,  1469,
    1471,  1474,  1477,  1480,  1483,  1486,  1488,  1492,  1500,  1502,
    1503,  1505,  1506,  1509,  1512,  1515,  1518,  1521,  1524,  1527,
    1530,  1533,  1537,  1540,  1543,  1546,  1548,  1550,  1552,  1554,
    1559,  1565,  1567,  1571,  1573,  1577,  1579,  1582,  1584,  1586,
    1588,  1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,  1609,
    1614,  1621,  1627,  1631,  1632,  1635,  1638,  1639,  1643,  1644,
    1649,  1651,  1657,  1662,  1664,  1669,  1671,  1674,  1677,  1679,
    1681,  1683,  1686,  1689,  1692,  1695,  1697,  1698,  1702,  1708,
    1710,  1712,  1715,  1719,  1721,  1724,  1728,  1729,  1731,  1733,
    1735,  1741,  1747,  1754,  1756,  1758,  1760,  1763,  1768,  1770,
    1771,  1777,  1779,  1780,  1784,  1787,  1789,  1790,  1793,  1794,
    1796,  1797,  1801,  1803,  1804,  1806,  1809,  1811,  1813,  1815,
    1817,  1820,  1823,  1825,  1828,  1832,  1834,  1836,  1840,  1844,
    1846,  1849,  1853,  1855,  1858,  1859,  1862,  1864,  1866,  1867,
    1869,  1871,  1873,  1877,  1880,  1884,  1885,  1887,  1891,  1896,
    1901,  1903,  1907,  1912,  1916,  1917,  1919,  1923,  1927,  1929,
    1931,  1932,  1934,  1936,  1941,  1942,  1946,  1947,  1950,  1951,
    1954,  1955,  1965,  1976,  1977,  1978,  1979,  1980,  1984,  1987,
    1989,  1990,  1994,  2000,  2004,  2007,  2013,  2016,  2018,  2020,
    2022,  2024,  2026,  2030,  2032,  2035,  2039,  2042,  2044,  2048,
    2050,  2052,  2059,  2063,  2067,  2070,  2074,  2075,  2077,  2081,
    2083,  2085,  2087,  2090,  2092,  2094,  2096,  2099,  2101,  2104,
    2106,  2109,  2110,  2114,  2115,  2117,  2121,  2123,  2125,  2127,
    2129,  2133,  2135,  2137,  2139,  2142,  2143,  2146,  2147,  2150,
    2151,  2156,  2158,  2161,  2163,  2165,  2166,  2168,  2172,  2175,
    2177,  2179,  2182,  2184,  2189,  2192,  2194,  2198,  2207,  2213,
    2215,  2219,  2221,  2223,  2228,  2233,  2238,  2240,  2242,  2248,
    2254,  2256,  2260,  2264,  2266,  2268,  2276,  2284,  2290,  2294,
    2295,  2297,  2299,  2301,  2304,  2305,  2307,  2309,  2311,  2312,
    2316,  2318,  2322,  2324,  2325,  2329,  2331,  2335,  2337,  2341,
    2345,  2347,  2349,  2353,  2355,  2358,  2360,  2364,  2368,  2370,
    2373,  2377,  2381,  2385,  2389,  2391,  2395,  2398,  2400,  2402,
    2404,  2406,  2408,  2410,  2412,  2414,  2416,  2418,  2422,  2426,
    2430,  2434,  2438,  2442,  2446,  2450,  2457,  2464,  2471,  2478,
    2485,  2492,  2499,  2506,  2513,  2520,  2527,  2534,  2541,  2548,
    2555,  2562,  2564,  2566,  2572,  2579,  2583,  2588,  2594,  2601,
    2605,  2610,  2614,  2619,  2623,  2628,  2633,  2639,  2644,  2649,
    2653,  2658,  2660,  2662,  2664,  2666,  2668,  2670,  2672,  2676,
    2680,  2691,  2702,  2704,  2706,  2708,  2710,  2712,  2714,  2716,
    2718,  2720,  2723,  2726,  2730,  2734,  2738,  2742,  2746,  2750,
    2754,  2758,  2760,  2762,  2764,  2766,  2770,  2772,  2774,  2776,
    2778,  2780,  2785,  2787,  2791,  2793,  2796,  2798,  2800,  2802,
    2804,  2806,  2808,  2810,  2813,  2816,  2819,  2821,  2823,  2825,
    2827,  2829,  2831,  2833,  2835,  2837,  2839,  2842,  2844,  2847,
    2849,  2851,  2853,  2855,  2857,  2860,  2862,  2864,  2866,  2868,
    2870,  2875,  2881,  2887,  2893,  2899,  2905,  2911,  2917,  2923,
    2929,  2935,  2942,  2949,  2956,  2965,  2970,  2975,  2979,  2986,
    2988,  2990,  2997,  3004,  3006,  3008,  3013,  3020,  3025,  3031,
    3035,  3041,  3046,  3052,  3054,  3056,  3058,  3060,  3062,  3064,
    3066,  3068,  3070,  3072,  3074,  3076,  3077,  3079,  3081,  3083,
    3085,  3087,  3089,  3091,  3093,  3095,  3097,  3099,  3101,  3103,
    3105,  3107,  3109,  3111,  3113,  3115,  3117,  3119,  3121,  3123,
    3125,  3127,  3129,  3131,  3133,  3135,  3137,  3139,  3141,  3143,
    3145,  3147,  3149,  3151,  3153,  3155
};
static const short yyrhs[] =
{
     279,     0,   279,   269,     0,   455,     0,   594,     0,   510,
       0,   309,     0,   299,     0,   584,     0,   477,     0,   280,
       0,   582,     0,   432,     0,   311,     0,   313,     0,   289,
       0,   511,     0,   504,     0,   533,     0,   502,     0,   588,
       0,    43,   641,     0,    73,   282,   129,   281,   182,   293,
     286,     0,    73,   284,   129,   148,   288,   182,   293,   286,
       0,    73,   282,   129,   281,   182,   291,     0,    73,   284,
     129,   148,   288,   182,   291,     0,    73,   295,   182,   297,
     287,     0,   566,     0,   180,   566,     0,     6,     0,     6,
     147,     0,   283,     0,   285,     0,   283,   270,   285,     0,
      59,     0,   163,     0,    84,     0,    47,     0,   188,   599,
       0,   153,   599,     0,   200,    73,   131,     0,     0,   200,
     210,   131,     0,     0,   682,     0,   158,   290,   282,   129,
     281,    66,   293,     0,   158,   290,   284,   129,   148,   288,
      66,   293,     0,   158,   282,   129,   281,    66,   293,     0,
     158,   284,   129,   148,   288,    66,   293,     0,   158,   282,
     129,   281,    66,   291,     0,   158,   284,   129,   148,   288,
      66,   291,     0,   158,   295,    66,   297,     0,    73,   131,
      64,     0,   292,     0,   291,   270,   292,     0,   291,   270,
     294,     0,   293,   270,   292,     0,   148,   682,     0,   184,
     686,     0,   195,   689,     0,   214,   683,     0,   294,     0,
     293,   270,   294,     0,   687,     0,   189,   687,     0,    74,
     687,     0,   296,     0,   295,   270,   296,     0,   683,     0,
     298,     0,   297,   270,   298,     0,   687,     0,   189,   687,
       0,    45,   300,     0,    63,   308,     0,    62,    68,   301,
       0,   668,   303,   157,   306,    55,   640,   117,   640,     0,
     484,     0,    19,     0,    37,   271,   644,   272,   490,     0,
       0,   304,     0,   271,   304,   272,     0,   305,     0,   304,
     270,   305,     0,   372,   302,     0,   372,   302,    20,   238,
       0,   307,     0,   271,   307,   272,     0,   372,   302,     0,
     372,   302,   212,     0,   372,   302,    20,   101,     0,   372,
     302,    20,   238,     0,   140,   644,     0,   677,    82,   489,
     135,   489,    55,   640,   117,   640,     0,    36,   310,     0,
      58,   676,   640,     0,   315,   538,    83,   680,   129,   566,
     316,     0,   148,   391,     0,   180,   359,     0,   184,   447,
       0,   195,   433,     0,    71,   332,     0,    40,   334,     0,
      51,   323,     0,   165,   317,     0,   214,   333,     0,   237,
     312,     0,   148,   392,     0,   180,   360,     0,   195,   434,
       0,    36,   132,     7,   314,     0,   148,   393,     0,   184,
     448,     0,   187,     0,     0,   601,     0,   600,     0,   366,
     271,   444,   629,   445,   272,     0,   644,   318,   319,   640,
     320,   321,     0,   111,     0,    14,     0,     0,     0,    32,
       0,     0,   102,   335,   647,   358,     0,     0,   322,     0,
     347,     0,   322,   347,     0,   369,   324,   479,   444,   325,
     445,   326,   368,     0,    10,     0,     0,    46,   444,   374,
       0,     0,     0,   327,     0,   328,     0,   327,   328,     0,
     329,     0,   330,     0,   331,     0,   124,    98,     0,   444,
      24,   271,   608,   272,   445,     0,   679,     0,   683,     0,
     336,   337,   340,     0,     0,   262,     0,   640,     0,     0,
     338,     0,   339,     0,   338,   339,     0,   139,   335,   644,
       0,   102,   335,   647,   358,     0,   189,   640,     0,   141,
     640,     0,   164,   118,   640,     0,     0,   341,     0,   342,
       0,   341,   342,     0,   347,     0,   344,     0,   343,     0,
      46,    23,   164,   670,     0,    75,   335,   647,     0,    25,
     335,   647,     0,   127,   335,   644,     0,   109,   335,   645,
       0,   346,     0,   345,     0,   103,   271,   348,   272,   136,
     349,     0,   103,    15,   349,     0,   103,     0,   353,   640,
     354,     0,   349,     0,   348,   270,   349,     0,   350,   351,
       0,   640,     0,     0,   351,   352,     0,   168,   335,   647,
       0,    94,     0,     0,   355,     0,   356,     0,   355,   356,
       0,   175,   357,   647,     0,   102,   335,   647,   358,     0,
       0,    12,     0,    12,   137,     0,     0,   137,     0,   138,
       0,   566,   361,   271,   362,   272,     0,   566,   361,   271,
     362,   272,     0,    62,    94,   640,     0,    62,   640,     0,
       0,   363,     0,   362,   270,   363,     0,   364,     0,   379,
       0,   369,   367,   373,   445,   375,   368,     0,   369,   480,
     365,     0,   369,   365,     0,   366,   271,   444,   629,   445,
     272,     0,    30,    20,     0,    30,     0,   479,   444,     0,
     606,   442,     0,    26,   671,     0,     0,   606,     0,   606,
       0,   372,   479,     0,     0,    46,   374,     0,     0,   633,
       0,   637,     0,   638,     0,   639,     0,   667,     0,   630,
       0,     0,   376,     0,   377,     0,   376,   377,     0,   380,
     378,     0,   124,    98,     0,   153,   566,   599,   387,   385,
       0,   386,     0,   187,   385,     0,   146,    90,   385,     0,
     380,   381,     0,    33,   673,     0,     0,   382,     0,   383,
       0,   384,     0,   386,     0,   187,   600,   385,     0,   146,
      90,   600,   385,     0,    65,    90,   600,   153,   566,   599,
     387,   385,     0,   247,   538,    83,   680,     0,     0,   444,
      24,   271,   608,   272,   445,     0,   388,     0,   389,     0,
     389,   388,     0,   388,   389,     0,     0,   129,   188,   390,
       0,   129,    47,   390,     0,   211,     0,   164,    46,     0,
     164,    98,     0,   123,   209,     0,   682,   395,   396,    10,
     442,   399,   405,   445,     0,   682,   395,   396,    10,   442,
     399,   405,   445,     0,   682,   395,   396,    10,   442,   399,
     405,   445,     0,   682,   395,   396,    10,   442,   399,   405,
     445,     0,   271,   397,   272,     0,     0,   157,   395,     0,
       0,   398,     0,   397,   270,   398,     0,   370,   480,     0,
     400,     0,     0,   401,     0,   400,   401,     0,    45,   402,
     369,   480,   403,   269,     0,   192,     0,     0,   262,   374,
       0,   373,     0,   408,     0,   405,     0,    17,   406,    54,
       0,   407,     0,   407,   428,     0,     0,   404,     0,   407,
     404,     0,   592,   269,     0,   584,   269,     0,   409,     0,
     410,     0,   411,     0,   412,     0,   414,     0,   417,     0,
     582,   269,     0,   418,     0,   420,     0,   588,   269,     0,
     425,     0,   415,     0,   416,     0,   178,   269,     0,    61,
     269,     0,   426,     0,    58,   676,   269,     0,    58,   676,
     629,   269,     0,    58,   269,     0,    59,   148,   682,   422,
     423,   269,     0,    59,   413,   629,   269,     0,   191,     0,
     254,     0,    64,   533,    86,   424,   427,    50,   404,     0,
      64,    59,   413,   629,    86,   424,    50,   404,     0,    59,
     413,   629,    86,   424,   269,     0,    78,   271,   608,   272,
     181,   404,    53,   404,     0,    78,   271,   608,   272,   181,
     404,     0,   144,   629,   419,   269,     0,     0,   533,    86,
     424,   269,     0,   273,   688,     0,   563,     0,   271,   563,
     272,     0,     0,   156,   424,     0,   156,   271,   424,   272,
       0,     0,   421,     0,   605,     0,   424,   270,   605,     0,
     424,   270,   421,     0,   199,   271,   608,   272,    50,   404,
       0,   235,   269,     0,   255,   269,     0,    10,    39,   674,
       0,     0,   429,     0,   428,   429,     0,   197,   430,    50,
     404,     0,   431,     0,   430,   270,   431,     0,   173,   641,
       0,    69,   678,     0,    58,   676,     0,     9,     0,    59,
     148,   682,   422,     0,   689,   599,    10,   442,   435,   446,
     443,     0,   689,   599,    10,   442,   435,   446,   443,     0,
     436,     0,   437,     0,   436,   186,   437,     0,   436,   186,
       6,   437,     0,   163,   553,   554,   438,   573,   568,   572,
       0,    66,   439,     0,   440,     0,   439,   270,   440,     0,
     441,     0,   565,     0,   440,   567,    89,   440,   129,   608,
       0,   271,   441,   272,     0,     0,     0,     0,     0,   200,
      24,   131,     0,     0,   686,    64,   566,   449,   450,   453,
     444,   454,   445,     0,   686,    64,   566,   449,   450,   453,
     444,   454,   445,     0,     3,     0,    80,     0,     0,   451,
     452,     0,    16,     0,     5,     0,    84,     0,   188,     0,
      47,     0,    84,   132,   188,     0,    84,   132,    47,     0,
     188,   132,    84,     0,   188,   132,    47,     0,    47,   132,
      84,     0,    47,   132,   188,     0,    84,   132,   188,   132,
      47,     0,    84,   132,    47,   132,   188,     0,   188,   132,
      84,   132,    47,     0,   188,   132,    47,   132,    84,     0,
      47,   132,    84,   132,   188,     0,    47,   132,   188,   132,
      84,     0,   143,   642,     0,     0,    10,   444,   399,   405,
       0,     7,   456,     0,    58,   676,   640,     0,   180,   566,
     461,     0,   184,   474,     0,   148,   394,     0,    40,   470,
     471,     0,    51,   463,   459,     0,    83,   469,     0,    46,
     444,   374,     0,    24,   444,   271,   608,   272,   445,     0,
     460,     0,   459,   460,     0,   164,   444,   457,   445,     0,
       4,    33,   458,     0,     4,   331,     0,    52,    46,     0,
      52,    33,     0,   182,   606,     0,   216,   372,   480,     0,
     462,     0,   461,   270,   462,     0,    52,   606,   468,     0,
      52,    33,   673,     0,     4,   364,     0,     4,   379,     0,
     465,   606,   143,   644,     0,   465,   463,   182,   606,     0,
     465,   467,   216,   466,   445,     0,   464,     0,   691,     0,
     215,     0,   216,     0,   217,     0,   218,     0,   219,     0,
     220,     0,   221,     0,   222,     0,   223,     0,   224,     0,
     225,     0,   226,     0,   227,     0,   228,     0,   229,     0,
     230,     0,   233,     0,   234,     0,   241,     0,   242,     0,
     250,     0,   252,     0,     7,     0,     7,   215,     0,   480,
     444,     0,   606,   442,     0,   606,     0,   213,     0,   211,
       0,     0,   680,     3,     0,   680,    80,     0,     0,   472,
       0,   471,   472,     0,     4,   322,     0,    52,   103,     0,
     164,   473,     0,     4,   344,     0,   343,     0,   473,   270,
     343,     0,   686,   449,   475,   453,   444,   476,   445,     0,
     450,     0,     0,   454,     0,     0,    52,   478,     0,    58,
     676,     0,    83,   680,     0,   148,   682,     0,   180,   685,
       0,   184,   686,     0,   195,   689,     0,    63,   677,     0,
      51,   675,     0,    62,    68,   668,     0,   165,   644,     0,
     214,   683,     0,    71,   679,     0,   480,     0,   481,     0,
     484,     0,   487,     0,   485,   274,   482,   275,     0,   492,
     274,   482,   275,   490,     0,   483,     0,   482,   270,   483,
       0,   646,     0,   646,   273,   646,     0,   485,     0,   492,
     490,     0,   491,     0,   496,     0,   500,     0,   243,     0,
     486,     0,   169,     0,    41,     0,   226,     0,   227,     0,
      85,     0,    96,     0,    19,   489,   488,   490,     0,    19,
     271,   645,   272,     0,    19,   271,   645,   270,   641,   272,
       0,    19,   271,   270,   641,   272,     0,   162,   168,   645,
       0,     0,   177,   641,     0,   177,   669,     0,     0,    23,
     164,   670,     0,     0,   495,   271,   644,   272,     0,   495,
       0,   495,   193,   271,   644,   272,     0,   494,   271,   644,
     272,     0,   494,     0,   493,   271,   644,   272,     0,   191,
       0,    23,   193,     0,    91,   193,     0,    23,     0,    91,
       0,   121,     0,   119,    23,     0,   119,    91,     0,    99,
     498,     0,   499,   498,     0,   644,     0,     0,   271,   646,
     272,     0,   271,   646,   270,   646,   272,     0,    44,     0,
      92,     0,    95,   501,     0,    97,    95,   501,     0,   152,
       0,    93,   145,     0,   271,   642,   272,     0,     0,   515,
       0,   503,     0,   532,     0,   164,    71,   679,   182,   646,
       0,   164,    71,   679,   182,   231,     0,   164,    71,   679,
     182,   266,   231,     0,   505,     0,   506,     0,   508,     0,
     252,   690,     0,   253,   252,   690,   507,     0,   130,     0,
       0,   161,   512,   182,   509,   690,     0,   252,     0,     0,
      28,   512,   513,     0,   161,   512,     0,   201,     0,     0,
     155,   514,     0,     0,   170,     0,     0,   164,   183,   516,
       0,   517,     0,     0,   518,     0,   517,   518,     0,   519,
       0,   520,     0,   521,     0,   525,     0,   151,   130,     0,
     151,   202,     0,   196,     0,   123,   196,     0,    88,   107,
     522,     0,   522,     0,   523,     0,   151,   185,   524,     0,
     151,    29,   524,     0,   170,     0,   170,   180,     0,   170,
     180,   174,     0,   194,     0,   123,   194,     0,     0,   154,
     528,     0,   166,     0,   149,     0,     0,   151,     0,   202,
       0,   529,     0,   528,   270,   529,     0,   531,   530,     0,
      64,   526,   527,     0,     0,   566,     0,   531,   270,   566,
       0,   164,   176,    83,   680,     0,   534,   535,   541,   543,
       0,   544,     0,   534,   186,   544,     0,   534,   186,     6,
     544,     0,   133,    20,   536,     0,     0,   537,     0,   536,
     270,   537,     0,   629,   538,   540,     0,    11,     0,    48,
       0,     0,   239,     0,   249,     0,   248,   548,   539,   549,
       0,     0,    64,   188,   542,     0,     0,   128,   601,     0,
       0,   200,   251,     0,     0,   163,   550,   553,   554,   557,
     573,   568,   572,   574,     0,   163,   550,   553,   554,   557,
     573,   568,   572,   574,   535,     0,     0,     0,     0,     0,
     551,   552,   547,     0,   551,   547,     0,   552,     0,     0,
     239,   647,   546,     0,   239,   271,   629,   272,   546,     0,
     239,   636,   546,     0,   240,   647,     0,   240,   271,   547,
     629,   272,     0,   240,   636,     0,    49,     0,   666,     0,
     555,     0,   267,     0,   556,     0,   555,   270,   556,     0,
     593,     0,   593,   681,     0,   593,    10,   681,     0,    66,
     558,     0,   559,     0,   558,   270,   559,     0,   560,     0,
     561,     0,   559,   567,    89,   559,   129,   608,     0,   271,
     560,   272,     0,   682,   562,   684,     0,   682,   562,     0,
     271,   563,   272,     0,     0,   564,     0,   563,   270,   564,
       0,   667,     0,   629,     0,   566,     0,   685,   684,     0,
     685,     0,    81,     0,   105,     0,   105,   134,     0,   159,
       0,   159,   134,     0,    67,     0,    67,   134,     0,     0,
      74,    20,   569,     0,     0,   570,     0,   569,   270,   570,
       0,   605,     0,   497,     0,   653,     0,   571,     0,   605,
      26,   671,     0,   651,     0,   652,     0,   655,     0,    77,
     608,     0,     0,   198,   608,     0,     0,   142,   575,     0,
       0,   576,   271,   577,   272,     0,    89,     0,   172,   114,
       0,   114,     0,   172,     0,     0,   578,     0,   578,   270,
     577,     0,   579,   580,     0,   575,     0,   685,     0,   685,
     579,     0,   120,     0,    83,   271,   581,   272,     0,   133,
     680,     0,   680,     0,   680,   270,   581,     0,    84,    86,
     566,   602,   190,   271,   583,   272,     0,    84,    86,   566,
     602,   545,     0,   593,     0,   583,   270,   593,     0,   585,
       0,   586,     0,    47,    66,   565,   573,     0,    47,    66,
     565,   587,     0,   198,    38,   128,   674,     0,   589,     0,
     590,     0,   188,   565,   164,   591,   573,     0,   188,   565,
     164,   591,   587,     0,   592,     0,   591,   270,   592,     0,
     607,   262,   593,     0,   629,     0,   667,     0,   151,    19,
     606,    66,   566,   595,   597,     0,    84,    19,   606,    86,
     566,   595,   597,     0,    63,    66,   596,   182,   596,     0,
      63,   182,   596,     0,     0,   489,     0,   636,     0,   641,
       0,   113,   598,     0,     0,   645,     0,   636,     0,   600,
       0,     0,   271,   601,   272,     0,   606,     0,   601,   270,
     606,     0,   603,     0,     0,   271,   604,   272,     0,   607,
       0,   604,   270,   607,     0,   606,     0,   684,   276,   672,
       0,   684,   276,   267,     0,   672,     0,   606,     0,   684,
     276,   672,     0,   623,     0,   124,   623,     0,   610,     0,
     608,   132,   608,     0,   608,     8,   608,     0,   610,     0,
     124,   623,     0,   609,   132,   608,     0,   609,     8,   608,
       0,   624,   132,   608,     0,   624,     8,   608,     0,   611,
       0,   271,   609,   272,     0,   124,   610,     0,   612,     0,
     615,     0,   616,     0,   617,     0,   622,     0,   613,     0,
     620,     0,   618,     0,   619,     0,   621,     0,   629,   262,
     629,     0,   629,   263,   629,     0,   629,   264,   629,     0,
     629,    70,   629,     0,   629,   106,   629,     0,   629,   125,
     629,     0,   629,   126,   629,     0,   629,   122,   629,     0,
     629,   262,     6,   271,   627,   272,     0,   629,   263,     6,
     271,   627,   272,     0,   629,   264,     6,   271,   627,   272,
       0,   629,    70,     6,   271,   627,   272,     0,   629,   106,
       6,   271,   627,   272,     0,   629,   125,     6,   271,   627,
     272,     0,   629,   126,     6,   271,   627,   272,     0,   629,
     122,     6,   271,   627,   272,     0,   629,   262,   614,   271,
     627,   272,     0,   629,   263,   614,   271,   627,   272,     0,
     629,   264,   614,   271,   627,   272,     0,   629,    70,   614,
     271,   627,   272,     0,   629,   106,   614,   271,   627,   272,
       0,   629,   125,   614,   271,   627,   272,     0,   629,   126,
     614,   271,   627,   272,     0,   629,   122,   614,   271,   627,
     272,     0,   171,     0,     9,     0,   629,    18,   629,     8,
     629,     0,   629,   124,    18,   629,     8,   629,     0,   629,
     108,   629,     0,   629,   124,   108,   629,     0,   629,   108,
     629,    57,   629,     0,   629,   124,   108,   629,    57,   629,
       0,   629,    79,   625,     0,   629,   124,    79,   625,     0,
     629,    34,   629,     0,   629,   124,    34,   629,     0,   629,
     175,   629,     0,   629,   124,   175,   629,     0,   629,   175,
     200,   629,     0,   629,   124,   175,   200,   629,     0,    60,
     271,   545,   272,     0,   167,   271,   545,   272,     0,   629,
      87,    98,     0,   629,    87,   124,    98,     0,   256,     0,
     257,     0,   258,     0,   259,     0,   260,     0,   261,     0,
     626,     0,   271,   632,   272,     0,   271,   627,   272,     0,
     163,   550,   553,   629,   557,   573,   568,   572,   574,   535,
       0,   163,   550,   553,   629,   557,   573,   568,   572,   574,
     535,     0,   605,     0,   631,     0,   648,     0,   635,     0,
     636,     0,   421,     0,   654,     0,   655,     0,   653,     0,
     266,   629,     0,   265,   629,     0,   629,   265,   629,     0,
     629,    31,   629,     0,   629,    26,   671,     0,   629,   266,
     629,     0,   629,   267,   629,     0,   629,   268,   629,     0,
     271,   629,   272,     0,   271,   628,   272,     0,   637,     0,
     638,     0,   639,     0,    42,     0,   684,   276,    42,     0,
     101,     0,   630,     0,   228,     0,   229,     0,   230,     0,
     605,   274,   632,   275,     0,   629,     0,   632,   270,   629,
       0,   635,     0,   266,   634,     0,   205,     0,   204,     0,
     203,     0,   231,     0,   232,     0,   634,     0,   640,     0,
      41,   207,     0,   226,   207,     0,   227,   207,     0,   277,
       0,   189,     0,   233,     0,   234,     0,   241,     0,   242,
       0,    69,     0,   173,     0,   250,     0,   207,     0,   208,
     207,     0,   642,     0,   266,   643,     0,   204,     0,   204,
       0,   642,     0,   204,     0,   647,     0,   266,   647,     0,
     204,     0,   649,     0,   650,     0,   651,     0,   652,     0,
      35,   271,   267,   272,     0,    35,   271,   666,   629,   272,
       0,    35,   271,    49,   629,   272,     0,   179,   271,   666,
     629,   272,     0,   179,   271,    49,   629,   272,     0,    13,
     271,   666,   629,   272,     0,    13,   271,    49,   629,   272,
       0,   116,   271,   666,   629,   272,     0,   116,   271,    49,
     629,   272,     0,   112,   271,   666,   629,   272,     0,   112,
     271,    49,   629,   272,     0,    72,   271,   679,   270,   629,
     272,     0,   217,   271,   665,    66,   629,   272,     0,   236,
     271,   629,    66,   644,   272,     0,   236,   271,   629,    66,
     644,    64,   642,   272,     0,   100,   271,   629,   272,     0,
     668,   271,   632,   272,     0,   668,   271,   272,     0,    22,
     271,   593,    10,   371,   272,     0,   656,     0,   657,     0,
     245,   271,   629,   270,   629,   272,     0,   246,   271,   564,
     270,   563,   272,     0,   658,     0,   660,     0,   244,   663,
     659,    54,     0,   244,   663,   659,    53,   664,    54,     0,
     197,   662,   181,   664,     0,   659,   197,   662,   181,   664,
       0,   244,   661,    54,     0,   244,   661,    53,   664,    54,
       0,   197,   608,   181,   664,     0,   661,   197,   608,   181,
     664,     0,   629,     0,   629,     0,   564,     0,   218,     0,
     219,     0,   220,     0,   221,     0,   222,     0,   223,     0,
     224,     0,   225,     0,     6,     0,     0,    98,     0,   206,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   691,     0,   691,     0,   691,
       0,   691,     0,   691,     0,   206,     0,   692,     0,   235,
       0,   238,     0,   236,     0,   246,     0,   249,     0,   255,
       0,   251,     0,   245,     0,   248,     0,   254,     0,   247,
       0,   256,     0,   257,     0,   258,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   525,   527,   531,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   557,   561,   565,   569,   573,   578,   579,   583,
     585,   587,   591,   592,   596,   600,   602,   604,   606,   608,
     612,   614,   618,   620,   624,   631,   636,   641,   646,   651,
     656,   661,   667,   671,   672,   674,   676,   680,   682,   684,
     686,   690,   691,   698,   700,   702,   706,   707,   711,   715,
     716,   720,   722,   729,   733,   735,   740,   746,   747,   749,
     755,   757,   758,   762,   763,   769,   772,   778,   779,   782,
     785,   789,   793,   796,   801,   810,   814,   817,   820,   822,
     824,   826,   828,   830,   832,   834,   836,   841,   845,   847,
     849,   859,   863,   865,   876,   878,   882,   884,   885,   891,
     897,   899,   901,   905,   907,   911,   913,   917,   919,   922,
     923,   930,   948,   950,   954,   956,   960,   962,   965,   966,
     969,   974,   975,   978,   982,   991,   999,  1008,  1013,  1014,
    1017,  1023,  1025,  1028,  1029,  1033,  1035,  1037,  1039,  1041,
    1045,  1047,  1050,  1051,  1055,  1057,  1058,  1059,  1063,  1065,
    1067,  1069,  1073,  1078,  1085,  1091,  1098,  1105,  1138,  1139,
    1142,  1148,  1152,  1153,  1156,  1164,  1168,  1169,  1172,  1173,
    1176,  1178,  1182,  1183,  1184,  1187,  1188,  1189,  1195,  1200,
    1205,  1207,  1209,  1213,  1214,  1218,  1219,  1226,  1230,  1234,
    1243,  1249,  1250,  1253,  1255,  1260,  1262,  1267,  1273,  1279,
    1283,  1289,  1291,  1295,  1300,  1301,  1302,  1304,  1306,  1310,
    1312,  1315,  1316,  1320,  1325,  1327,  1332,  1333,  1335,  1343,
    1347,  1349,  1352,  1353,  1354,  1355,  1358,  1362,  1366,  1373,
    1380,  1385,  1391,  1394,  1396,  1398,  1400,  1404,  1407,  1411,
    1414,  1417,  1420,  1429,  1441,  1452,  1463,  1474,  1476,  1480,
    1482,  1486,  1487,  1491,  1497,  1499,  1503,  1504,  1508,  1513,
    1515,  1519,  1521,  1525,  1526,  1529,  1533,  1535,  1537,  1541,
    1542,  1546,  1547,  1548,  1549,  1550,  1551,  1552,  1553,  1554,
    1555,  1556,  1557,  1558,  1559,  1560,  1561,  1563,  1568,  1571,
    1573,  1577,  1581,  1586,  1590,  1590,  1592,  1597,  1602,  1607,
    1609,  1613,  1617,  1623,  1628,  1633,  1635,  1637,  1641,  1643,
    1645,  1649,  1650,  1651,  1653,  1657,  1668,  1670,  1678,  1680,
    1683,  1684,  1688,  1693,  1694,  1698,  1700,  1702,  1704,  1711,
    1719,  1726,  1746,  1750,  1752,  1754,  1758,  1769,  1773,  1774,
    1778,  1779,  1782,  1785,  1791,  1795,  1801,  1805,  1811,  1815,
    1823,  1834,  1845,  1847,  1849,  1853,  1857,  1859,  1863,  1865,
    1867,  1869,  1871,  1873,  1875,  1877,  1879,  1881,  1883,  1885,
    1887,  1889,  1891,  1895,  1897,  1901,  1907,  1911,  1914,  1921,
    1923,  1925,  1928,  1931,  1936,  1940,  1946,  1947,  1951,  1960,
    1964,  1966,  1968,  1970,  1972,  1976,  1977,  1981,  1983,  1985,
    1987,  1993,  1996,  1998,  2002,  2010,  2011,  2012,  2013,  2014,
    2015,  2016,  2017,  2018,  2019,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2027,  2028,  2029,  2030,  2031,  2032,  2035,  2037,
    2041,  2043,  2048,  2054,  2056,  2058,  2062,  2064,  2071,  2077,
    2078,  2082,  2090,  2092,  2094,  2098,  2099,  2106,  2116,  2117,
    2121,  2122,  2128,  2132,  2134,  2136,  2138,  2140,  2142,  2144,
    2146,  2148,  2150,  2152,  2154,  2161,  2162,  2165,  2166,  2169,
    2174,  2181,  2182,  2186,  2195,  2201,  2202,  2205,  2206,  2207,
    2208,  2225,  2230,  2235,  2254,  2271,  2278,  2279,  2286,  2290,
    2296,  2302,  2310,  2314,  2320,  2324,  2328,  2334,  2338,  2345,
    2351,  2357,  2365,  2370,  2375,  2382,  2383,  2384,  2387,  2388,
    2391,  2392,  2393,  2400,  2404,  2415,  2422,  2428,  2482,  2544,
    2545,  2552,  2565,  2570,  2575,  2582,  2584,  2591,  2592,  2593,
    2597,  2602,  2607,  2618,  2619,  2620,  2623,  2627,  2631,  2633,
    2637,  2641,  2642,  2645,  2649,  2653,  2654,  2657,  2659,  2663,
    2664,  2668,  2672,  2673,  2677,  2678,  2682,  2683,  2684,  2685,
    2688,  2690,  2694,  2696,  2700,  2702,  2705,  2707,  2709,  2713,
    2715,  2717,  2721,  2723,  2725,  2729,  2733,  2735,  2737,  2741,
    2743,  2747,  2748,  2752,  2756,  2758,  2762,  2763,  2768,  2776,
    2780,  2782,  2784,  2788,  2790,  2794,  2795,  2799,  2803,  2805,
    2807,  2811,  2813,  2817,  2819,  2823,  2825,  2829,  2831,  2835,
    2837,  2844,  2856,  2869,  2873,  2877,  2881,  2885,  2887,  2889,
    2891,  2895,  2897,  2899,  2903,  2905,  2907,  2911,  2913,  2917,
    2919,  2923,  2924,  2928,  2929,  2931,  2938,  2944,  2945,  2949,
    2950,  2953,  2955,  2959,  2962,  2967,  2969,  2973,  2974,  2978,
    2979,  2982,  2983,  2988,  2993,  2995,  2997,  2999,  3001,  3003,
    3005,  3007,  3014,  3016,  3020,  3021,  3025,  3026,  3027,  3028,
    3029,  3033,  3034,  3035,  3038,  3040,  3044,  3046,  3053,  3055,
    3059,  3063,  3065,  3067,  3073,  3075,  3079,  3080,  3084,  3086,
    3089,  3090,  3094,  3096,  3098,  3102,  3103,  3111,  3114,  3118,
    3119,  3126,  3127,  3130,  3134,  3138,  3145,  3146,  3149,  3154,
    3159,  3160,  3164,  3168,  3169,  3175,  3177,  3181,  3183,  3185,
    3188,  3189,  3192,  3196,  3198,  3201,  3203,  3209,  3210,  3214,
    3218,  3219,  3224,  3225,  3229,  3233,  3234,  3239,  3240,  3243,
    3248,  3253,  3256,  3263,  3264,  3266,  3267,  3269,  3273,  3274,
    3276,  3278,  3282,  3284,  3288,  3289,  3291,  3295,  3296,  3297,
    3298,  3299,  3300,  3301,  3302,  3303,  3304,  3309,  3311,  3313,
    3315,  3317,  3319,  3321,  3323,  3330,  3332,  3334,  3336,  3338,
    3340,  3342,  3344,  3346,  3348,  3350,  3352,  3354,  3356,  3358,
    3360,  3364,  3365,  3371,  3373,  3378,  3380,  3382,  3384,  3389,
    3391,  3395,  3397,  3401,  3403,  3405,  3407,  3411,  3415,  3419,
    3421,  3425,  3430,  3435,  3442,  3447,  3452,  3461,  3462,  3466,
    3470,  3483,  3500,  3501,  3502,  3503,  3504,  3505,  3506,  3507,
    3508,  3509,  3511,  3513,  3520,  3522,  3524,  3531,  3538,  3545,
    3547,  3549,  3550,  3551,  3552,  3554,  3556,  3560,  3564,  3580,
    3596,  3600,  3604,  3605,  3609,  3610,  3614,  3616,  3618,  3620,
    3622,  3626,  3627,  3629,  3645,  3661,  3665,  3669,  3671,  3675,
    3679,  3682,  3685,  3688,  3691,  3696,  3698,  3703,  3704,  3708,
    3715,  3722,  3729,  3736,  3737,  3741,  3747,  3748,  3749,  3750,
    3753,  3755,  3757,  3760,  3767,  3776,  3783,  3792,  3794,  3796,
    3798,  3804,  3813,  3817,  3824,  3828,  3832,  3834,  3838,  3844,
    3845,  3848,  3852,  3856,  3857,  3860,  3862,  3866,  3868,  3872,
    3874,  3878,  3880,  3884,  3887,  3890,  3893,  3895,  3897,  3899,
    3901,  3903,  3905,  3907,  3911,  3912,  3915,  3923,  3926,  3929,
    3932,  3935,  3938,  3941,  3944,  3947,  3950,  3953,  3956,  3959,
    3962,  3970,  3973,  3976,  3979,  3982,  3985,  3988,  3991,  3994,
    3999,  4000,  4005,  4007,  4008,  4009,  4010,  4011,  4012,  4013,
    4014,  4015,  4016,  4017,  4018,  4019
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
  "KW_INSERTING", "KW_UPDATING", "KW_DELETING", "'='", "'<'", "'>'", 
  "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", 
  "']'", "'.'", "'?'", "top", "statement", "grant", "prot_table_name", 
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
  "numeric_type", "ordinal", "prec_scale", "decimal_keyword", 
  "float_type", "precision_opt", "set", "set_generator", "savepoint", 
  "set_savepoint", "release_savepoint", "release_only_opt", 
  "undo_savepoint", "optional_savepoint", "commit", "rollback", 
  "optional_work", "optional_retain", "opt_snapshot", "set_transaction", 
  "tran_opt_list_m", "tran_opt_list", "tran_opt", "access_mode", 
  "lock_wait", "isolation_mode", "iso_mode", "snap_shot", "version_mode", 
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
  "group_clause", "grp_column_list", "grp_column_elem", 
  "group_by_function", "having_clause", "where_clause", "plan_clause", 
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
       0,   278,   278,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   280,   280,   280,   280,   280,   281,   281,   282,
     282,   282,   283,   283,   284,   285,   285,   285,   285,   285,
     286,   286,   287,   287,   288,   289,   289,   289,   289,   289,
     289,   289,   290,   291,   291,   291,   291,   292,   292,   292,
     292,   293,   293,   294,   294,   294,   295,   295,   296,   297,
     297,   298,   298,   299,   300,   300,   301,   302,   302,   302,
     303,   303,   303,   304,   304,   305,   305,   306,   306,   307,
     307,   307,   307,   307,   308,   309,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   311,   312,   312,
     312,   313,   314,   314,   315,   315,   316,   316,   316,   317,
     318,   318,   318,   319,   319,   320,   320,   321,   321,   322,
     322,   323,   324,   324,   325,   325,   326,   326,   327,   327,
     328,   329,   329,   330,   331,   332,   333,   334,   335,   335,
     336,   337,   337,   338,   338,   339,   339,   339,   339,   339,
     340,   340,   341,   341,   342,   342,   342,   342,   343,   343,
     343,   343,   344,   344,   345,   345,   346,   347,   348,   348,
     349,   350,   351,   351,   352,   353,   354,   354,   355,   355,
     356,   356,   357,   357,   357,   358,   358,   358,   359,   360,
     361,   361,   361,   362,   362,   363,   363,   364,   364,   364,
     365,   366,   366,   367,   367,   368,   368,   369,   370,   371,
     372,   373,   373,   374,   374,   374,   374,   374,   374,   375,
     375,   376,   376,   377,   378,   378,   378,   378,   378,   379,
     380,   380,   381,   381,   381,   381,   382,   383,   384,   385,
     385,   386,   387,   387,   387,   387,   387,   388,   389,   390,
     390,   390,   390,   391,   392,   393,   394,   395,   395,   396,
     396,   397,   397,   398,   399,   399,   400,   400,   401,   402,
     402,   403,   403,   404,   404,   405,   406,   406,   406,   407,
     407,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   409,
     409,   410,   411,   412,   413,   413,   414,   415,   416,   417,
     417,   418,   419,   420,   421,   422,   422,   422,   423,   423,
     423,   424,   424,   424,   424,   425,   426,   426,   427,   427,
     428,   428,   429,   430,   430,   431,   431,   431,   431,   432,
     433,   434,   435,   436,   436,   436,   437,   438,   439,   439,
     440,   440,   441,   441,   442,   443,   444,   445,   446,   446,
     447,   448,   449,   449,   449,   450,   451,   451,   452,   452,
     452,   452,   452,   452,   452,   452,   452,   452,   452,   452,
     452,   452,   452,   453,   453,   454,   455,   456,   456,   456,
     456,   456,   456,   456,   457,   458,   459,   459,   460,   460,
     460,   460,   460,   460,   460,   461,   461,   462,   462,   462,
     462,   462,   462,   462,   463,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   465,   465,
     466,   466,   467,   468,   468,   468,   469,   469,   470,   471,
     471,   472,   472,   472,   472,   473,   473,   474,   475,   475,
     476,   476,   477,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   479,   479,   480,   480,   481,
     481,   482,   482,   483,   483,   484,   484,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   486,   486,   487,   487,
     487,   487,   488,   488,   489,   489,   489,   490,   490,   491,
     491,   491,   492,   492,   492,   493,   493,   493,   494,   494,
     495,   495,   495,   496,   496,   497,   498,   498,   498,   499,
     499,   500,   500,   500,   500,   501,   501,   502,   502,   502,
     503,   503,   503,   504,   504,   504,   505,   506,   507,   507,
     508,   509,   509,   510,   511,   512,   512,   513,   513,   514,
     514,   515,   516,   516,   517,   517,   518,   518,   518,   518,
     519,   519,   520,   520,   521,   521,   522,   522,   522,   523,
     523,   523,   524,   524,   524,   525,   526,   526,   526,   527,
     527,   528,   528,   529,   530,   530,   531,   531,   532,   533,
     534,   534,   534,   535,   535,   536,   536,   537,   538,   538,
     538,   539,   539,   540,   540,   541,   541,   542,   542,   543,
     543,   544,   545,   546,   547,   548,   549,   550,   550,   550,
     550,   551,   551,   551,   552,   552,   552,   553,   553,   554,
     554,   555,   555,   556,   556,   556,   557,   558,   558,   559,
     559,   560,   560,   561,   561,   562,   562,   563,   563,   564,
     564,   565,   565,   566,   567,   567,   567,   567,   567,   567,
     567,   567,   568,   568,   569,   569,   570,   570,   570,   570,
     570,   571,   571,   571,   572,   572,   573,   573,   574,   574,
     575,   576,   576,   576,   576,   576,   577,   577,   578,   578,
     579,   579,   580,   580,   580,   581,   581,   582,   582,   583,
     583,   584,   584,   585,   586,   587,   588,   588,   589,   590,
     591,   591,   592,   593,   593,   594,   594,   595,   595,   595,
     596,   596,   489,   597,   597,   598,   598,   599,   599,   600,
     601,   601,   602,   602,   603,   604,   604,   605,   605,   605,
     606,   607,   607,   608,   608,   608,   608,   608,   609,   609,
     609,   609,   609,   609,   610,   610,   610,   611,   611,   611,
     611,   611,   611,   611,   611,   611,   611,   612,   612,   612,
     612,   612,   612,   612,   612,   613,   613,   613,   613,   613,
     613,   613,   613,   613,   613,   613,   613,   613,   613,   613,
     613,   614,   614,   615,   615,   616,   616,   616,   616,   617,
     617,   618,   618,   619,   619,   619,   619,   620,   621,   622,
     622,   623,   623,   623,   624,   624,   624,   625,   625,   626,
     627,   628,   629,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   629,   629,   629,   629,   629,   630,   630,
     630,   631,   632,   632,   633,   633,   634,   634,   634,   634,
     634,   635,   635,   635,   635,   635,   636,   637,   637,   638,
     639,   639,   639,   639,   639,   640,   640,   641,   641,   642,
     643,   644,   645,   646,   646,   647,   648,   648,   648,   648,
     649,   649,   649,   649,   649,   649,   649,   649,   649,   649,
     649,   650,   651,   652,   652,   652,   653,   653,   654,   655,
     655,   656,   656,   657,   657,   658,   658,   659,   659,   660,
     660,   661,   661,   662,   663,   664,   665,   665,   665,   665,
     665,   665,   665,   665,   666,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   691,   692,   692,   692,   692,   692,   692,   692,   692,
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
       3,     3,     1,     1,     6,     3,     1,     3,     1,     3,
       2,     1,     0,     2,     3,     1,     0,     1,     1,     2,
       3,     4,     0,     1,     2,     0,     1,     1,     5,     5,
       3,     2,     0,     1,     3,     1,     1,     6,     3,     2,
       6,     2,     1,     2,     2,     2,     0,     1,     1,     2,
       0,     2,     0,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     2,     2,     2,     5,     1,     2,     3,     2,
       2,     0,     1,     1,     1,     1,     3,     4,     8,     4,
       0,     6,     1,     1,     2,     2,     0,     3,     3,     1,
       2,     2,     2,     8,     8,     8,     8,     3,     0,     2,
       0,     1,     3,     2,     1,     0,     1,     2,     6,     1,
       0,     2,     1,     1,     1,     3,     1,     2,     0,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     1,     1,     1,     2,     2,     1,     3,
       4,     2,     6,     4,     1,     1,     7,     8,     6,     8,
       6,     4,     0,     4,     2,     1,     3,     0,     2,     4,
       0,     1,     1,     3,     3,     6,     2,     2,     3,     0,
       1,     2,     4,     1,     3,     2,     2,     2,     1,     4,
       7,     7,     1,     1,     3,     4,     7,     2,     1,     3,
       1,     1,     6,     3,     0,     0,     0,     0,     3,     0,
       9,     9,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     5,     5,     5,
       5,     5,     5,     2,     0,     4,     2,     3,     3,     2,
       2,     3,     3,     2,     3,     6,     1,     2,     4,     3,
       2,     2,     2,     2,     3,     1,     3,     3,     3,     2,
       2,     4,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     0,     2,     2,     0,     1,
       2,     2,     2,     2,     2,     1,     3,     7,     1,     0,
       1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     2,     2,     2,     1,     1,     1,     1,     4,
       5,     1,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       6,     5,     3,     0,     2,     2,     0,     3,     0,     4,
       1,     5,     4,     1,     4,     1,     2,     2,     1,     1,
       1,     2,     2,     2,     2,     1,     0,     3,     5,     1,
       1,     2,     3,     1,     2,     3,     0,     1,     1,     1,
       5,     5,     6,     1,     1,     1,     2,     4,     1,     0,
       5,     1,     0,     3,     2,     1,     0,     2,     0,     1,
       0,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       2,     2,     1,     2,     3,     1,     1,     3,     3,     1,
       2,     3,     1,     2,     0,     2,     1,     1,     0,     1,
       1,     1,     3,     2,     3,     0,     1,     3,     4,     4,
       1,     3,     4,     3,     0,     1,     3,     3,     1,     1,
       0,     1,     1,     4,     0,     3,     0,     2,     0,     2,
       0,     9,    10,     0,     0,     0,     0,     3,     2,     1,
       0,     3,     5,     3,     2,     5,     2,     1,     1,     1,
       1,     1,     3,     1,     2,     3,     2,     1,     3,     1,
       1,     6,     3,     3,     2,     3,     0,     1,     3,     1,
       1,     1,     2,     1,     1,     1,     2,     1,     2,     1,
       2,     0,     3,     0,     1,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     2,     0,     2,     0,     2,     0,
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
       5,     6,     6,     6,     8,     4,     4,     3,     6,     1,
       1,     6,     6,     1,     1,     4,     6,     4,     5,     3,
       5,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     1,     1,
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
       0,     0,   566,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   566,   640,     0,     0,     0,     0,     0,
       1,    10,    15,     7,     6,    13,    14,    12,     3,     9,
      19,   548,    17,   553,   554,   555,     5,    16,   547,   549,
      18,   614,   610,    11,     8,   721,   722,    20,   726,   727,
       4,   458,     0,     0,     0,     0,     0,     0,   396,   565,
     568,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,    95,   620,   899,     0,    21,   897,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   472,     0,    29,    37,    34,
      36,   748,    35,   748,   980,   982,   984,   983,   989,   985,
     992,   990,   986,   988,   991,   987,   993,   994,   995,     0,
      31,     0,    32,     0,    66,    68,   972,   981,     0,     0,
       0,     0,     0,     0,     0,     0,   564,     0,     0,   955,
     634,   639,     0,     0,   573,     0,   671,   673,   974,     0,
       0,     0,   107,   556,   979,     0,     2,     0,     0,   626,
       0,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,     0,   424,   425,     0,   965,   403,     0,
     969,   400,   268,   971,     0,   673,   399,   374,   975,   570,
     563,   895,     0,   103,   151,   150,   104,   133,   217,   760,
     961,     0,   102,   145,   968,     0,    98,   268,   105,   901,
     122,    99,   202,   100,     0,   101,   748,   978,   106,   146,
     618,   619,     0,   900,   898,     0,    74,     0,   966,   697,
     480,   964,   473,     0,   479,   484,   474,   475,   482,   476,
     477,   478,   483,   327,    30,     0,    39,   747,    38,     0,
       0,     0,     0,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,   562,   905,     0,   886,   633,   633,   634,
     646,   644,   954,   647,     0,   648,   638,   634,     0,     0,
       0,     0,     0,     0,   589,   582,   571,   572,   574,   576,
     577,   578,   585,   586,   579,     0,   672,   973,   108,   268,
     109,   202,   110,   748,   559,     0,     0,   611,     0,   630,
       0,     0,     0,   401,   459,   366,     0,   366,     0,   220,
     402,   406,   397,   456,   457,     0,   270,   241,   448,     0,
     398,   415,     0,   372,   373,   469,   569,   567,   896,   148,
     148,     0,     0,     0,   160,   152,   153,   132,     0,    96,
       0,     0,   111,   270,   121,   120,   123,     0,     0,     0,
       0,     0,   957,    75,   220,   516,     0,   723,   724,   481,
       0,     0,     0,     0,   864,   892,     0,   956,     0,   866,
       0,     0,   893,     0,   887,   878,   877,   876,   980,     0,
       0,     0,   868,   869,   870,   879,   880,   888,   889,   984,
     890,   891,     0,   989,   985,   894,     0,     0,     0,     0,
     847,   349,   325,   667,   842,   757,   670,   867,   843,   881,
     845,   846,   861,   862,   863,   882,   844,   906,   907,   908,
     909,   850,   848,   849,   929,   930,   933,   934,   669,     0,
       0,   961,     0,   750,     0,     0,    27,    33,     0,     0,
      43,    69,    71,   976,    67,     0,     0,     0,   752,     0,
      52,     0,     0,     0,     0,    51,   561,     0,     0,     0,
     643,   641,     0,   650,     0,   649,   651,   653,   733,   734,
     637,     0,   608,     0,   583,   594,   580,   594,   581,   595,
     601,   605,   606,   590,   575,   697,   730,   761,     0,     0,
     270,     0,     0,   558,   557,   613,   615,   620,   612,   628,
       0,   609,   185,   176,   461,   464,   173,   172,   129,     0,
     462,   148,   148,   148,   148,   465,   463,   460,     0,   410,
       0,   412,   411,     0,   413,     0,   407,     0,     0,   271,
     218,   268,     0,     0,   419,     0,   420,   366,   449,     0,
     455,     0,     0,     0,   452,   961,   377,   376,   468,     0,
     394,   149,     0,     0,   158,     0,   157,     0,   147,   161,
     162,   166,   165,   164,   154,   516,   528,   503,   539,   506,
     529,   540,     0,   546,   507,     0,   536,     0,   530,   543,
     502,   525,   504,   505,   500,   366,   485,   486,   487,   495,
     501,   488,   497,   518,     0,   523,   520,   498,   536,   499,
     112,   268,   113,     0,     0,   124,     0,     0,   201,   241,
     374,   364,     0,   220,     0,    81,    83,     0,     0,     0,
     742,     0,     0,     0,     0,   993,   994,   995,     0,   696,
     765,   774,   777,   782,   778,   779,   780,   784,   785,   783,
     786,   781,   763,     0,   955,     0,   955,   883,     0,     0,
     955,   955,   955,     0,   884,   885,     0,     0,   944,     0,
       0,     0,     0,   852,   851,   640,     0,     0,   670,   324,
     977,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   749,    28,     0,     0,    44,    72,     0,     0,
      26,   739,     0,   755,   640,     0,   718,   739,     0,     0,
       0,     0,   560,     0,   633,     0,     0,   697,     0,     0,
     654,   970,   551,     0,   550,   903,     0,   584,     0,   592,
     588,   587,     0,   598,     0,   603,   591,     0,   728,   729,
       0,     0,     0,   241,   364,     0,   624,     0,   625,   629,
       0,     0,   130,   186,     0,     0,     0,     0,     0,   366,
     409,     0,   366,   367,   414,   495,   518,   273,     0,   267,
     269,   364,   240,   962,   212,   209,     0,   222,   366,   485,
     364,     0,     0,     0,   239,   242,   243,   244,   245,     0,
     418,   454,   453,   417,   416,     0,     0,     0,   380,   378,
     379,   375,     0,   366,   195,   155,   159,     0,   163,     0,
     513,   526,   527,   544,     0,   541,   546,     0,   533,   531,
     532,   135,     0,     0,     0,   496,     0,     0,     0,     0,
     534,   270,     0,   364,   125,   200,     0,   203,   205,   206,
       0,     0,     0,     0,   220,   220,    78,     0,    85,    77,
     514,   515,   958,   516,     0,     0,     0,   776,   764,     0,
       0,   834,   835,   836,     0,   768,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   946,
     947,   948,   949,   950,   951,   952,   953,     0,     0,     0,
       0,   939,     0,     0,     0,     0,     0,   955,   326,   860,
     859,   668,   872,     0,   855,   960,   854,   853,   856,   857,
     858,   927,     0,   865,   759,   758,   751,     0,     0,     0,
       0,     0,     0,    24,    53,    41,    61,    63,     0,     0,
      70,     0,   744,     0,   754,   955,     0,   744,    49,    47,
       0,     0,     0,   642,   645,     0,   656,   657,   659,   660,
     666,     0,   683,   652,   655,   552,   904,   593,   602,   597,
     596,     0,   607,   731,   732,   762,   364,     0,     0,   616,
     635,   617,   627,   175,   182,   181,     0,   178,   148,   192,
     177,   187,   188,   169,   168,   902,   171,   170,   466,     0,
       0,     0,   408,   272,   275,   211,   366,     0,   367,   213,
     208,   214,     0,     0,   250,     0,   422,   367,   366,   364,
     421,     0,     0,     0,   393,   471,   196,   197,   156,     0,
       0,     0,     0,   518,     0,   542,     0,     0,   366,   367,
       0,   491,   493,     0,     0,     0,     0,     0,     0,     0,
     374,   275,   148,   127,   241,   198,   394,   955,   369,   352,
     353,     0,    82,     0,   220,     0,    87,     0,    84,     0,
       0,     0,   725,   963,     0,     0,   769,     0,     0,   775,
       0,     0,   767,   766,     0,   821,     0,   812,   811,     0,
     790,     0,   819,   837,   829,     0,     0,     0,   791,   815,
       0,     0,   794,     0,     0,     0,     0,     0,     0,     0,
     792,     0,     0,   793,     0,   823,     0,     0,   787,     0,
       0,   788,     0,     0,   789,     0,     0,   220,     0,   910,
       0,     0,   925,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   945,     0,     0,   943,     0,     0,   935,     0,
       0,     0,     0,     0,   871,   926,    65,    57,    58,    64,
      59,    60,     0,     0,     0,    22,    25,    41,    42,   516,
     516,     0,   736,   756,     0,     0,   719,   735,    50,    48,
      45,     0,   681,   659,     0,   679,   674,   675,   677,     0,
       0,   664,     0,   695,   599,   600,   604,   275,   199,   369,
       0,   180,     0,     0,     0,   193,     0,   189,     0,     0,
       0,   404,   228,   223,   874,   224,   225,   226,   227,   280,
       0,   274,   276,     0,   221,   229,     0,   250,   620,   246,
       0,   423,   450,   451,   385,   386,   382,   381,   384,   383,
     366,   470,   367,   167,   959,     0,     0,   509,     0,   508,
     545,     0,   537,     0,   136,     0,   489,     0,   517,   518,
     524,   522,     0,   519,   364,     0,     0,     0,   119,   128,
     204,   366,     0,     0,   365,     0,    97,     0,   117,   116,
      93,     0,     0,    89,     0,    86,     0,   827,   828,   771,
     770,   773,   772,     0,     0,     0,   640,     0,     0,   830,
       0,     0,     0,     0,     0,     0,   822,   820,   816,     0,
     824,     0,     0,     0,     0,   825,     0,     0,     0,     0,
       0,     0,   916,   915,     0,     0,   912,   911,     0,   920,
     919,   918,   917,   914,   913,     0,     0,   941,   940,     0,
       0,     0,     0,     0,     0,     0,   873,    54,    55,     0,
      56,    62,    23,   740,     0,   741,   738,   743,   746,   745,
       0,     0,   717,     0,    46,   662,   658,   680,   676,   678,
       0,     0,   663,     0,     0,   699,     0,   365,   621,   622,
     636,   148,   183,   179,     0,   195,   194,   190,     0,   367,
     875,   279,     0,   288,   367,   277,   367,   216,   230,   231,
     366,     0,   247,     0,     0,     0,     0,     0,     0,     0,
       0,   275,   467,   511,     0,   512,     0,   134,     0,   216,
     137,   138,   140,   141,   142,   492,   494,   490,   521,   275,
     394,   367,   195,     0,     0,     0,   350,     0,   354,   366,
      88,     0,     0,    90,   518,     0,   813,     0,     0,   955,
     839,   838,     0,     0,   817,     0,     0,     0,     0,   826,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     928,   219,   921,   922,     0,   923,   942,   937,   936,     0,
     931,   932,   697,    40,   516,   697,   720,   681,   665,   687,
     682,   684,   689,   686,   535,   691,   692,   688,   693,     0,
     694,   705,   631,   367,   351,   623,     0,   174,   191,   367,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   982,   987,   289,   284,     0,   286,   283,   293,   294,
     295,   296,   297,   304,   305,   298,   300,   301,   303,   308,
       0,     0,     0,     0,     0,   266,     0,     0,   207,   232,
       0,     0,     0,   250,   233,   236,   748,     0,   367,   391,
     392,   388,   387,   390,   389,     0,   510,   538,   143,   131,
     139,     0,   366,   263,   126,   367,     0,   697,   368,   355,
       0,     0,    91,    92,    79,     0,   798,   806,     0,   799,
     807,   802,   810,   814,   818,   800,   808,   801,   809,   795,
     803,   796,   804,   797,   805,     0,   938,   683,   737,   683,
       0,     0,     0,     0,   701,   703,   704,   698,     0,   264,
     184,   405,   222,   311,     0,     0,   314,   315,     0,   307,
       0,     0,     0,   322,   306,     0,   336,   337,   285,     0,
     290,   287,   340,     0,   299,   292,   302,   291,   210,   215,
     234,   250,   748,   237,   256,   249,   251,   395,   367,     0,
     370,     0,   357,   358,   360,   361,   683,   367,    76,    94,
       0,   924,   695,   695,   661,   685,   690,   702,   705,     0,
     282,     0,   309,     0,   327,     0,     0,     0,     0,     0,
       0,   348,     0,     0,     0,     0,   343,   341,   331,     0,
     332,   238,   256,     0,   250,   252,   253,   265,   367,   681,
     360,     0,     0,   695,     0,   697,   699,   699,   709,     0,
     706,     0,   710,   281,   278,   310,   330,     0,   313,     0,
     339,     0,   321,     0,   347,   346,   967,   345,     0,     0,
     323,     0,   250,     0,     0,   248,     0,   255,     0,   254,
     371,   363,   359,     0,   356,   118,   683,   614,   614,   700,
     705,     0,   712,     0,   708,   711,     0,     0,     0,     0,
       0,     0,     0,     0,   342,   344,   334,   333,   235,     0,
       0,   259,   258,   257,   681,   695,   841,   632,   707,     0,
     714,     0,   328,   312,   318,     0,     0,     0,   320,   335,
     262,   260,   261,     0,   699,     0,   715,     0,     0,   338,
     316,     0,   362,   614,   713,     0,   329,   317,   319,   840,
     716,     0,     0,     0
};

static const short yydefgoto[] =
{
    1841,    20,    21,   455,   119,   120,   121,   122,  1185,   710,
     705,    22,   134,   953,   954,   955,   956,   123,   124,   460,
     461,    23,    81,   373,   858,   634,   635,   636,  1085,  1086,
     236,    24,    73,    25,   152,    26,   362,    74,  1296,   218,
     366,   626,  1073,  1288,   524,   206,   358,  1059,  1439,  1440,
    1441,  1442,  1443,  1444,   212,   228,   203,   572,   204,   354,
     355,   356,   578,   579,   580,   581,   582,   526,   527,   528,
    1006,  1003,  1004,  1221,  1402,   529,  1010,  1011,  1012,  1226,
    1048,   221,   310,   368,   846,   847,   848,   785,   786,   787,
    1568,   555,   547,  1344,   637,  1028,  1231,  1417,  1418,  1419,
    1574,   849,   557,   794,   795,   796,   797,  1249,   798,  1724,
    1725,  1726,  1802,   216,   308,   620,   191,   336,   552,   548,
     549,  1240,  1241,  1242,  1412,  1701,  1543,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,  1648,  1552,  1553,  1554,  1555,
    1556,  1709,  1557,   420,   421,  1787,  1719,  1558,  1559,  1791,
    1661,  1662,  1715,  1716,    27,   225,   312,  1078,  1079,  1080,
    1597,  1682,  1683,  1684,   851,  1456,   540,  1022,  1294,   223,
     622,   345,   568,   569,   811,   813,  1261,    28,    58,   773,
     770,   330,   331,   340,   341,   183,   184,   342,  1037,   563,
     803,   188,   160,   323,   324,   536,   196,   570,  1262,    29,
      95,   605,   606,   607,  1060,  1061,   608,   775,   610,   611,
    1053,  1373,   835,   612,   776,   614,   615,   616,   617,  1509,
     828,   618,   619,   825,    30,    31,    32,    33,    34,   514,
      35,   477,    36,    37,    60,   200,   347,    38,   296,   297,
     298,   299,   300,   301,   302,   303,   740,   304,   991,  1216,
     499,   500,   745,   501,    39,  1560,    41,   159,   515,   516,
     232,  1400,  1001,   319,   758,   521,    42,   716,   480,   286,
    1220,  1525,   139,   140,   141,   284,   484,   485,   486,   727,
     976,   977,   978,   979,  1211,   422,  1162,  1685,   146,  1209,
    1213,  1510,  1511,  1512,  1395,   377,  1522,  1738,  1638,  1739,
    1740,  1741,  1784,  1825,  1561,  1195,  1562,    45,    46,   378,
    1563,    48,    49,   505,  1564,   487,    50,   962,  1374,  1192,
    1377,   256,   257,   452,   467,   468,   712,   424,   425,   508,
     649,   874,   650,   651,   652,   653,  1109,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   876,  1112,  1113,  1317,
     687,   663,   427,   428,   933,  1233,   429,   430,   431,   432,
     433,   434,   435,   640,   219,   234,  1514,  1016,  1062,   735,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   924,   447,   679,  1166,   680,  1163,   917,   285,   448,
     449,   861,  1263,   934,   209,   782,  1092,   240,   186,   237,
    1755,   213,  1826,   730,   980,   125,   450,   195,   197,   957,
     689,   226,   153,   451,   127
};

static const short yypact[] =
{
    2129,  1052,   293,  1350,   535,   914,   435,  2773,   391,  2091,
     185,   485,  1676,   293,   676,   543,  2984,   333,  2984,   290,
     328,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    79,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  4195,  2984,  2984,  2984,  2984,  2984,-32768,-32768,
     470,   794,  2984,  2984,  2984,   632,  2984,   441,  2984,  2984,
  -32768,  2984,  2984,-32768,   902,-32768,   462,-32768,-32768,   646,
    2984,-32768,  2984,  2984,  2984,   688,  2984,  2984,  2984,  2984,
     441,  2984,  2984,  2984,  2984,-32768,  2984,   645,-32768,-32768,
  -32768,   566,-32768,   566,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   716,
     507,   736,-32768,   287,-32768,-32768,-32768,-32768,  2984,  2984,
    2984,   730,   776,   802,   979,   129,   689,   459,   617,   692,
     717,-32768,  2984,   928,   620,   823,-32768,  2984,-32768,  2984,
    2984,  2984,-32768,-32768,-32768,  2984,-32768,   999,   215,   958,
     284,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   209,-32768,-32768,   794,-32768,-32768,   205,
  -32768,-32768,   756,-32768,   496,-32768,-32768,   207,-32768,   866,
  -32768,-32768,   834,-32768,   771,-32768,-32768,  1041,-32768,-32768,
  -32768,   794,-32768,-32768,-32768,    84,-32768,   756,-32768,-32768,
     210,-32768,  1002,-32768,   997,-32768,   566,-32768,-32768,-32768,
  -32768,-32768,   987,-32768,-32768,   880,-32768,  1009,-32768,   897,
  -32768,-32768,-32768,   880,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  4602,-32768,  2984,-32768,-32768,-32768,  2822,
     143,   954,  2165,  2984,  1022,   845,  1056,  1066,  2822,   986,
     998,  1007,  2165,   896,-32768,  5992,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  4283,-32768,-32768,-32768,   969,  2984,
    1033,   961,   163,  2984,   974,-32768,-32768,   620,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  2984,-32768,-32768,-32768,   756,
  -32768,  1002,-32768,   566,  1032,  5992,  1014,-32768,  1018,  1010,
     864,  1111,   648,   284,-32768,  1186,   726,-32768,  2984,-32768,
     209,-32768,-32768,-32768,-32768,  2984,  1069,  1873,  1019,  2354,
     963,-32768,  4195,-32768,-32768,   825,-32768,-32768,-32768,   976,
     976,   794,  1121,   794,  1104,   771,-32768,-32768,  3570,-32768,
    2984,  2984,-32768,  1069,-32768,-32768,  1208,   429,   970,  2984,
    1232,  2984,-32768,-32768,    36,    32,  3611,-32768,-32768,-32768,
     973,   982,   984,  1040,-32768,-32768,   988,-32768,   989,-32768,
     991,   992,-32768,   993,-32768,-32768,-32768,-32768,   994,   996,
    1047,  1050,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1000,
  -32768,-32768,  4699,  1004,  1013,-32768,  5992,  5992,  4363,  2984,
  -32768,-32768,  1012,-32768,   995,-32768,  1080,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1015,
    1020,  1021,   723,-32768,  2984,  1076,-32768,-32768,  2984,  2984,
     401,-32768,-32768,-32768,-32768,  2984,  2984,    62,-32768,  2984,
  -32768,  1202,  2984,  2822,  1132,  1017,-32768,  2984,  4938,   315,
  -32768,-32768,  5992,-32768,  1219,  1024,-32768,   621,  1080,-32768,
  -32768,   699,-32768,    90,-32768,   473,-32768,   473,-32768,  1028,
  -32768,   123,-32768,  1114,-32768,   322,-32768,-32768,  1029,  1026,
    1069,  1034,  1280,-32768,-32768,  1036,-32768,   251,-32768,  1171,
    1057,-32768,-32768,   148,  1209,-32768,-32768,-32768,-32768,   794,
  -32768,   976,   976,   976,   976,-32768,  1037,-32768,  1285,-32768,
    1286,-32768,-32768,  1266,-32768,  3570,-32768,  3570,   748,-32768,
  -32768,   756,  1303,  2984,-32768,  6261,-32768,   412,-32768,  2984,
     817,   496,  1133,  1100,  1174,  1138,-32768,-32768,-32768,   186,
    1176,-32768,  1117,   441,-32768,   794,-32768,  1301,-32768,  1104,
  -32768,-32768,-32768,-32768,-32768,   458,  1143,-32768,-32768,-32768,
    1144,-32768,  1180,  1072,-32768,  1243,  1073,   203,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1075,
  -32768,-32768,-32768,   132,  1081,  1092,   297,-32768,  1073,-32768,
  -32768,   756,-32768,  1276,  1340,-32768,   794,   794,-32768,  1873,
     207,-32768,  1222,-32768,  1210,  1095,-32768,  6525,  3845,  1233,
  -32768,  1244,  1102,  3708,  1105,  1023,  1141,  1237,  3371,   239,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  3301,   764,  5018,   162,-32768,  2984,  5992,
     836,   884,   895,  1137,-32768,-32768,  5992,  3947,  1080,   461,
    1178,  5992,  5018,   269,   269,   676,   799,  1099,   323,-32768,
  -32768,  5018,  5992,  2984,  5992,  5992,  5992,  5992,  5992,  5257,
    1193,  2984,-32768,-32768,  2471,  1195,-32768,-32768,  1170,  2165,
  -32768,  1319,   829,-32768,   676,  1112,-32768,  1319,  2471,  1323,
    1325,  2984,-32768,   323,-32768,   341,  3439,  1187,  5018,  2984,
  -32768,-32768,-32768,    26,-32768,-32768,   219,-32768,  1198,-32768,
  -32768,-32768,  2984,   779,  2984,-32768,-32768,  2984,-32768,-32768,
    5018,  2984,  1383,  1873,-32768,  5992,  1146,  2984,-32768,-32768,
     794,   794,-32768,   409,  1117,  1117,  1191,   441,   648,-32768,
  -32768,  1126,-32768,-32768,-32768,-32768,  1375,-32768,  2984,-32768,
  -32768,-32768,-32768,-32768,  1380,-32768,  1134,  1358,-32768,  1376,
  -32768,  1317,  1326,   566,-32768,-32768,-32768,-32768,-32768,  1385,
  -32768,-32768,-32768,-32768,-32768,  2984,  6351,   441,  1283,  1287,
    1288,-32768,   441,-32768,  1079,-32768,-32768,  1253,-32768,   426,
    1260,-32768,-32768,-32768,   441,-32768,  1072,   563,-32768,-32768,
  -32768,  1379,   563,  1263,   563,-32768,   441,   441,  1159,   441,
  -32768,  1069,  2984,-32768,  1332,-32768,   843,-32768,-32768,-32768,
     825,  1273,  2984,   847,    43,-32768,-32768,  1172,  1425,-32768,
  -32768,-32768,-32768,    32,  2984,  1289,  4044,-32768,-32768,  1289,
    3708,-32768,-32768,-32768,   142,-32768,   247,  1522,  3947,  3947,
    5992,  5992,  1737,  1175,   518,  1991,  5992,  2293,   731,  2572,
    2669,  5337,  2923,  3020,  3274,  5992,  5992,  1443,  5992,  1182,
    5992,  1185,   356,  5992,  5992,  5992,  5992,  5992,  5992,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1390,   903,   178,
    5018,-32768,  3947,  5992,   506,   792,  1188,   692,-32768,-32768,
  -32768,-32768,  1080,   713,-32768,-32768,  1433,   269,   269,   892,
     892,-32768,   853,-32768,-32768,-32768,-32768,  2984,  2984,  2984,
    2984,  2984,  2984,  1192,-32768,   452,-32768,-32768,  2471,  1330,
  -32768,   380,  1352,  2984,-32768,   692,  5018,  1352,  1192,  1199,
    2471,  1701,  1397,-32768,-32768,  3439,  1200,   538,-32768,-32768,
    1197,  3947,  1400,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   504,-32768,-32768,-32768,-32768,-32768,   888,  1273,-32768,
  -32768,-32768,  1207,-32768,-32768,-32768,   891,-32768,   976,  1466,
  -32768,   409,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1216,
    3947,  2226,-32768,-32768,  1434,-32768,-32768,  2226,-32768,-32768,
  -32768,-32768,   566,   566,  1248,  1225,-32768,-32768,-32768,-32768,
  -32768,   104,   155,   900,-32768,  1470,-32768,-32768,-32768,  2984,
     535,   894,  1329,  1375,  1228,-32768,  1117,   904,-32768,-32768,
     733,-32768,  1229,  2984,   742,  1231,  1236,   441,  1238,  1491,
     207,  1434,   976,  1209,  1873,-32768,  1176,   692,  1305,  1327,
  -32768,  1348,-32768,   441,  1371,  1457,-32768,  6525,-32768,   441,
    1278,  1459,-32768,-32768,  1245,  1246,-32768,  3947,  3947,-32768,
    3947,  3947,-32768,  1511,   208,  1080,  1250,-32768,-32768,  1251,
    1080,  5576,-32768,-32768,-32768,  1426,  1254,  1255,  1080,  1011,
    1256,  1258,  1080,  5992,  5992,  1175,  5992,  5656,  1261,  1262,
    1080,  1265,  1267,  1080,  5992,  1080,  1268,  1270,  1080,  1271,
    1272,  1080,  1275,  1279,  1080,   382,   411,-32768,   421,-32768,
     586,  5992,-32768,   615,   630,   654,   687,   706,   714,  5992,
     441,  5018,-32768,  1490,   183,  1080,  1368,  5018,-32768,  5992,
    5992,  5018,  5992,  5992,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2471,  1479,  2471,-32768,  1192,   452,-32768,   288,
     288,   317,-32768,-32768,  4283,   908,-32768,-32768,  1192,  1199,
    1290,  1701,   741,  1297,  3439,  1423,-32768,  1428,  1436,  1482,
    5018,  2984,  1539,  1495,-32768,-32768,-32768,  1434,-32768,  1305,
     246,  1405,   794,  1438,  1117,  1439,  1117,-32768,  3947,   144,
     980,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1386,
    1562,  1434,-32768,  5992,-32768,   447,  1427,  1248,   902,-32768,
    3947,-32768,-32768,-32768,  1450,  1453,  1456,  1458,  1468,  1475,
  -32768,-32768,-32768,-32768,-32768,  1336,   535,-32768,  1191,-32768,
  -32768,   563,-32768,  2226,   446,   563,-32768,   563,-32768,  1375,
  -32768,-32768,  1338,-32768,-32768,   825,  1562,  1117,-32768,  1209,
  -32768,-32768,  4283,  1565,-32768,   217,-32768,  1320,-32768,  1207,
  -32768,  1339,   794,   165,  1341,-32768,   794,-32768,-32768,-32768,
    1511,-32768,  1511,  5992,  1435,  1435,   676,  1344,   916,-32768,
    1435,  1435,  5992,  1435,  1435,   241,  1080,-32768,  1063,  5992,
    1080,  1435,  1435,  1435,  1435,  1080,  1435,  1435,  1435,  1435,
    1435,  1435,-32768,-32768,  1345,  3570,-32768,-32768,   724,-32768,
  -32768,-32768,-32768,-32768,-32768,   732,   117,-32768,-32768,  5018,
    5018,  1558,  1437,   767,   921,   983,  1080,-32768,-32768,  1489,
  -32768,-32768,-32768,-32768,  1440,-32768,-32768,-32768,-32768,-32768,
    1219,  5018,-32768,  1701,  1290,-32768,   538,-32768,-32768,-32768,
    3439,   924,-32768,  2763,  3947,  1483,  1562,-32768,-32768,-32768,
  -32768,   976,-32768,-32768,   794,  1079,-32768,-32768,   146,-32768,
  -32768,-32768,  2984,  6228,-32768,-32768,  1080,  1595,   447,-32768,
     738,  2984,-32768,  1549,   149,  1445,  1550,  1447,  1590,  1554,
    1592,  1434,-32768,-32768,  1369,-32768,  1370,-32768,  1542,  1595,
     446,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1434,
    1176,-32768,  1079,  1470,  1577,  1514,-32768,  1273,-32768,-32768,
  -32768,  1532,   337,-32768,  1375,  1533,  1080,  1381,  1384,   692,
  -32768,-32768,  1387,  1391,  1080,  1392,  1393,  5992,  5992,  1080,
    1395,  1396,  1398,  1399,  1401,  1402,  1403,  1404,  1406,  1408,
  -32768,-32768,-32768,-32768,   441,-32768,-32768,-32768,-32768,  5018,
  -32768,-32768,  1187,-32768,   288,  1187,-32768,   865,-32768,-32768,
    1382,-32768,-32768,  1629,-32768,-32768,-32768,-32768,-32768,  1410,
     239,   662,-32768,-32768,-32768,-32768,  1117,-32768,-32768,-32768,
  -32768,  3570,  3755,   319,  1389,   276,  1413,  1576,  5992,  1412,
    1416,  1419,  1421,-32768,-32768,  1615,  6130,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1591,  1422,  1424,  1429,  1431,-32768,  1420,  2984,-32768,-32768,
    1596,  1605,  2984,  1248,-32768,-32768,   566,  2984,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1562,-32768,-32768,-32768,-32768,
  -32768,  1562,-32768,-32768,-32768,-32768,  3518,  1187,-32768,-32768,
    5992,   794,-32768,-32768,-32768,   794,-32768,-32768,  5992,-32768,
  -32768,-32768,-32768,  1080,  1080,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1430,-32768,  1400,-32768,  1400,
    3947,  2763,  2984,  4091,-32768,-32768,  1587,-32768,  1441,-32768,
  -32768,-32768,   124,-32768,  5895,  2984,-32768,-32768,  5992,-32768,
      15,  1619,  3947,  1080,-32768,  3947,-32768,-32768,-32768,   281,
  -32768,  1509,-32768,  2633,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1248,   566,-32768,  1578,-32768,-32768,-32768,-32768,  1470,
  -32768,  3518,  1448,   538,-32768,-32768,  1400,  1080,-32768,-32768,
     983,-32768,  1495,  1495,   239,-32768,-32768,-32768,  3883,  2226,
  -32768,  1442,-32768,   878,  4602,   778,  5992,  2633,   151,  1444,
     154,-32768,  2984,  2984,   535,   122,-32768,-32768,-32768,   955,
  -32768,-32768,  1578,   242,  1248,  1579,  1580,-32768,-32768,   741,
    1449,  3518,  1621,  1495,  1452,  1187,  1483,  1483,-32768,  1454,
    1455,   831,  2984,-32768,-32768,-32768,  1559,  2633,-32768,  1067,
     141,  1536,-32768,  1669,-32768,-32768,-32768,-32768,  6228,   281,
  -32768,  2633,  1248,   517,   517,-32768,  1681,-32768,  1541,-32768,
  -32768,-32768,   538,  3518,-32768,-32768,  1400,  1597,  1597,-32768,
    3883,  1460,-32768,  2984,-32768,-32768,  3093,  1467,   959,  2633,
    1698,  1688,  6228,  6228,-32768,-32768,-32768,-32768,-32768,  1530,
     168,-32768,-32768,-32768,   948,  1495,-32768,-32768,-32768,  2984,
  -32768,  2633,  1471,-32768,-32768,   139,  2984,  6228,  1687,-32768,
  -32768,-32768,-32768,  3947,  1483,  1472,  1477,   927,  6228,-32768,
  -32768,  6228,   239,  1597,-32768,  2984,-32768,-32768,-32768,-32768,
  -32768,  1742,  1745,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -90,   189,-32768,   193,  1488,   564,-32768,
    -316,-32768,-32768,  -558,    19,  -646,  -940,  1740,  1492,  1481,
    1045,-32768,-32768,-32768,   669,-32768,  1124,   906,-32768,   678,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   690,-32768,-32768,-32768,-32768,-32768,
     329,-32768,-32768,  1451,-32768,-32768,-32768,  -334,-32768,-32768,
  -32768,  1415,-32768,-32768,  1189,  -225,  1461,-32768,-32768,  -321,
  -32768,  -700,-32768,-32768,-32768,-32768,-32768,-32768,   762,-32768,
    -641,-32768,-32768,  1469,  1030,   703,  1462,  1003,   701,-32768,
     354,   -48,-32768,-32768,  -303,   153,  -998,-32768,-32768,   378,
  -32768,  1464, -1065,-32768,-32768,-32768,-32768, -1183,   383,    76,
      78,    83,    46,-32768,-32768,-32768,-32768,   -89,  -272,-32768,
    1035,  -944,-32768,   570,-32768,-32768, -1397, -1092,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   166,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -961,   108,-32768,  -900,-32768,-32768,-32768,
  -32768,   156,-32768,    56,-32768,-32768,-32768,   821,-32768, -1099,
  -32768,-32768,  -899,   140,  -615,   423,  -239,  -925,   603,-32768,
  -32768,  -525,  -724,-32768,-32768,  -968, -1277,-32768,-32768,-32768,
  -32768,-32768,  1493,-32768,  1274,  1485,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1501,-32768,-32768,-32768,-32768,-32768,
  -32768,  -505,  -515,-32768,  1008,   553,  -556,  -340,-32768,-32768,
  -32768,  -277,  -924,-32768,  -338,-32768,-32768,-32768,-32768,-32768,
    1214,-32768,-32768,  1025,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1820,-32768,-32768,-32768,-32768,-32768,
    1537,-32768,-32768,-32768,  1343,-32768,  1353,-32768,-32768,-32768,
  -32768,  1110,-32768,-32768,-32768,    12,-32768,  -885,-32768,  1101,
    -465,-32768,-32768,-32768,-32768,-32768,    24,   -74,  -172,   -42,
  -32768,-32768,  -591,-32768,  1714,  -858,  -987,-32768,  1127, -1233,
  -32768,  -866,   882,-32768,-32768,  -393,  -247,   652,   -37, -1057,
   -1174,-32768,   227,-32768, -1195,  -492,  -888,   338,-32768,    80,
  -32768,   119,-32768,    27,  1863,-32768,  1865,-32768,-32768,  1361,
    1867,-32768,-32768,-32768,  -191,  -597,-32768,  1151, -1053,   905,
  -32768,   -95,  -648,  -622,-32768,-32768,-32768, -1129,   -26,  -410,
    -534,-32768,  -469,-32768,-32768,-32768,   191,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  -476,-32768,   744,-32768,   152,
  -32768,  1149,  -931,-32768,  -584,-32768,   640,  -926,  -116,  -910,
    -905,  -897,   -50,     6,     0,-32768,    -1,  -733,  -457,  -100,
  -32768,-32768,-32768, -1228, -1224, -1216,-32768, -1209,-32768,-32768,
  -32768,-32768,-32768,-32768,   702,-32768, -1003,-32768,   467,  -267,
       8,-32768,   810,  -806,  -658,  1315,    59,-32768,   -61,  1790,
  -32768,   -17,   -49,  1150,   -31,   -45,  -132,   -15,   -41,  -162,
  -32768,   -52,   -11,    -9,-32768
};


#define	YYLAST		6768


static const short yytable[] =
{
     126,   147,   211,   126,    78,   189,   423,   148,   258,   154,
      77,   205,    40,   748,   207,   306,   573,   489,   609,   194,
     613,   277,   280,   242,   192,   686,   545,   229,   224,  1244,
     774,   222,   777,   583,   734,   217,   208,   278,   281,   246,
     789,   251,   945,   185,   187,   190,   193,   148,   198,   252,
     788,   250,   756,   210,   187,   214,   713,   193,   247,   148,
     198,  1007,   227,   126,  1422,   253,   220,   147,   897,  1172,
     245,   238,   969,   148,   241,   187,   249,   238,   214,   190,
     193,   859,   148,   198,   227,   126,  1051,   193,   543,   248,
    1232,   624,   265,   995,   927,  1234,  1232,   535,   639,   313,
     462,  1234,   264,  1245,   266,   850,   481,  1194,  1291,  1202,
     462,  1235,  1251,   311,   506,   942,  1236,  1235,   309,   210,
     148,   210,  1236,   965,  1237,   288,  1076,  1286,   363,  1269,
    1237,   370,  1502,   214,  1274,  1002,   332,  1376,   307,   998,
     193,   148,   227,   919,   314,  1034,   154,  1505,  1414,  1660,
    1097,  1790,   878,   994,   878,   833,   719,   878,  1357,   878,
     968,   359,   878,   760,  1361,  1515,  1024,   868,   282,  1516,
    1027,   423,  1758,   509,   867,  1031,  1595,  1517,   471,   875,
    1420,  1494,   317,  1083,  1518,  1462,   878,   743,  1254,  1828,
      98,   878,   495,   -80,  1451,   272,  1458,   764,   765,   766,
     767,   132,  1256,   762,   128,   133,  1646,  1380,   333,   638,
     343,   898,   157,   325,  1821,   609,  1313,   613,   512,  1292,
     510,   316,   456,  1457,   364,   714,   829,   100,  1071,   453,
     274,   456,   360,   808,   693,   982,    75,   482,   752,   694,
     492,   736,  1368,   374,  1371,   490,   210,   878,   495,  1477,
     148,   379,   715,   463,   126,  1100,   502,   985,   583,   148,
     294,   326,   230,   463,  1513,   158,  1822,   693,   361,  1647,
     809,   129,   694,  1396,  1098,  1437,   879,   693,   879,   507,
     190,   879,   694,   879,   148,   334,   879,   344,   320,  1763,
    1711,  1038,  1255,   496,   830,   693,   101,   707,    76,   231,
     694,   574,   544,   576,  1523,  1454,   102,   633,   820,   550,
     879,   208,  1187,   560,  1084,   879,   564,   628,   799,   210,
     623,   365,   632,   270,  1199,  1200,   210,   271,   210,   621,
     210,   103,   630,   565,   509,  1650,   321,  1432,  1386,  1712,
     518,   693,  1232,  1257,  1102,  1103,   694,  1234,   497,   693,
    1713,   193,   198,  1420,   694,  1447,  1496,  1497,  1599,  1161,
     148,  1794,   190,  1235,  1359,   498,   831,   693,  1236,  1196,
    1057,   879,   694,   327,   810,    78,  1237,  1463,    14,  1101,
    1077,  1217,   693,   720,  1246,  1247,  1699,   694,  1164,  1495,
    1673,   328,  1759,   744,  1096,  1818,  1819,   867,   489,   263,
    1186,   867,  1728,  1515,   497,   972,   834,  1516,   693,  1761,
     690,  1761,  1198,   694,  1099,  1517,  1409,   703,  1529,   761,
    1830,  1578,  1518,  1751,  1253,   329,  1753,   706,   711,   899,
    1764,  1837,   717,  1298,  1838,   926,   456,   693,  1602,    14,
     507,   706,   694,  1371,   931,   148,  1189,   693,   322,   193,
     463,  1628,   694,  1692,  1714,  1693,   148,  1735,  1379,  1299,
     148,   489,   780,   193,   148,   638,   722,  1645,   154,   262,
    -366,  -241,   814,   695,   696,   697,   698,   791,   731,   763,
     553,   149,  1592,   489,  1530,  1398,  1229,  1585,  1721,  1565,
     838,  1566,    75,  1677,    59,  1399,  1626,  1736,  1737,  1678,
     337,    82,  1513,   338,   130,  1591,   695,   696,   697,   698,
    1646,  1008,  1733,   150,   920,   921,   695,   696,   697,   698,
     376,  1015,  1403,   627,  1507,   816,  1593,  1318,   151,   790,
    1019,   859,   841,  1021,  1720,  1435,   697,   698,  1774,    96,
    1604,  1765,   155,  1018,   783,  1285,   210,   462,   339,  1029,
     783,  1087,   973,  1193,    76,  1384,   993,   263,   792,  1167,
    1168,  1450,  1190,  1309,  1310,   276,  1311,  1312,   839,  1069,
    1438,  -241,   815,  1647,  1045,  1603,   844,   845,  1720,  1798,
     695,   696,   697,   698,  1009,    78,  1091,   724,   695,   696,
     697,   698,   747,  -241,   276,   930,   738,   156,  1639,   793,
    -241,   708,  1805,   208,  1641,  1205,   695,   696,   697,   698,
    1824,  1608,   693,   974,   142,   509,  1114,   694,  1720,  1206,
     210,   695,   696,   697,   698,   199,  1732,  -681,  1152,   862,
    1015,   729,  1797,   986,  -241,   638,   201,   202,    78,   215,
    1799,   693,  1115,  1207,   860,    75,   694,   695,   696,   697,
     698,   901,  1183,  1676,  1342,  1214,   693,  1720,   922,   214,
    1720,   694,    75,   274,  1013,  1014,   233,   739,   145,  1449,
    1680,   709,  1732,   531,  1224,   946,   695,   696,   697,   698,
     693,  1800,  1720,  1343,   935,   694,   695,   696,   697,   698,
     706,   210,   210,  1346,  1408,   463,  1050,  1208,   282,   489,
     463,  1743,  1718,  1169,  1527,   502,  1215,   992,   290,   463,
    1005,  1005,   193,   693,   235,  1732,  1424,   193,   694,   143,
     731,   507,  1184,   532,    76,  1469,   144,   208,  1801,   819,
     275,   453,   693,   148,   239,   148,   276,   694,  1287,    75,
     693,   283,   210,   291,   210,   694,  1718,  1732,   210,  1123,
     693,  1634,   550,  1727,  1238,   694,   243,   533,   693,   541,
    1238,  1669,  1734,   694,  1528,  1124,  1017,   274,  1232,   210,
     282,   292,   542,  1234,   293,   534,  1635,   260,  1364,  1036,
    1039,  1087,  1729,  1423,  1506,  1176,  1718,  1243,  1179,  1235,
     294,  1094,   254,   693,  1236,  1095,   210,   210,   694,  1252,
    1796,    76,  1237,  1770,   693,  1070,  1040,  1750,  1205,   694,
    1125,  1594,  1044,   895,  1436,  1081,   295,  1391,   693,  1273,
    1446,   274,  1206,   694,  1054,  1718,  1696,   104,  1718,  1056,
     566,   509,  1772,   148,  1636,  1065,  1066,   255,  1068,  1126,
    1491,   567,   282,   148,  1345,   259,  1207,  1788,  1777,  1778,
    1718,   695,   696,   697,   698,  1093,   105,   106,  1347,   107,
    1520,   267,  1570,    78,  1747,   261,   108,   109,   110,   111,
     112,   273,   113,   349,  1804,   114,   115,   116,   117,   118,
     695,   696,   697,   698,  1571,   903,  1812,  1349,   279,  1815,
     282,  1572,  1806,  1807,   276,   695,   696,   697,   698,  1180,
    1208,   282,  1350,   274,   693,   268,  1127,  1181,  1178,   694,
     350,  1827,   351,   230,  1781,   137,   138,  1177,   693,   695,
     696,   697,   698,   694,   423,  1573,  1351,   489,   989,   693,
     732,   269,  1205,   905,   694,   352,  1833,   507,   463,   193,
     198,   463,   227,   126,   907,   990,  1206,  1258,  1839,   463,
     231,  1782,   695,   696,   697,   698,   986,   138,   522,  1352,
     353,   463,   463,   423,  1783,   733,   193,   523,   762,  1160,
    1207,   695,   696,   697,   698,   945,    79,    80,  1353,   695,
     696,   697,   698,  1173,  1259,    97,  1354,   305,  1174,   695,
     696,   697,   698,   701,  1630,   702,  1492,   695,   696,   697,
     698,   201,   202,  1275,  1493,   609,  1238,   613,  1276,   693,
    1627,   289,  1275,  1629,   694,  1205,  1642,  1279,   778,   315,
     779,  1431,   318,  -831,  1208,   489,    98,   335,   801,  1206,
     802,  -831,   695,   696,   697,   698,   346,   693,    99,  1500,
    1264,   348,   694,   695,   696,   697,   698,  1748,   208,   726,
      78,   357,  1453,  1207,  1264,   453,  1265,   695,   696,   697,
     698,   369,  1170,   100,   367,   210,  1282,  1526,  1322,   691,
     371,   928,   210,  1375,  1375,  1378,  1117,  1823,  1121,  1392,
    1129,  1132,  1300,  1137,  1140,  1143,   372,  -831,  1304,   693,
    -831,   375,    51,   693,   694,   376,  1694,  -831,   694,   963,
    -831,   964,   458,    52,  -831,  1686,   693,  1208,   465,  -831,
      53,   694,  -831,  1074,   489,  1075,   466,   855,  1708,  1082,
    1478,  1710,   469,  1173,  1405,  1175,  1407,   473,  -831,   531,
     470,   896,   101,   900,   472,    54,   474,   904,   906,   908,
     493,  -832,   102,   695,   696,   697,   698,  1745,   476,  -832,
     577,   491,  -831,  1789,   503,  -831,  -831,   494,  1074,  1356,
    1218,  1222,   513,  1223,  1266,  -831,  1267,   103,   695,   696,
     697,   698,  1005,   463,  1271,   463,  1272,    14,  1381,   532,
    1382,   799,  -831,   395,   396,   397,  1173,  1452,  1471,    78,
      78,   691,   463,  1501,   691,   193,  1508,  1761,   522,  1836,
      55,  1367,   307,  1370,  -831,  -832,   519,   523,  -832,  -831,
     520,   405,   406,   533,   530,  -832,  1046,  1047,  -832,   538,
    1600,  -831,  -832,  -831,  1760,  1761,   551,  -832,  1814,  1761,
    -832,   534,    56,   561,   558,   943,    57,  -833,   571,   575,
     625,   629,   631,  1776,   664,  -833,  -832,   667,   695,   696,
     697,   698,  1461,   665,   674,   666,  1465,   675,   704,   668,
     669,  1519,   670,   671,   672,  -957,    78,   673,   718,   692,
    -832,   676,  1434,  -832,  -832,   681,   695,   696,   697,   698,
     721,   509,   691,  -832,   682,   726,   699,   709,   746,  1832,
     754,   750,  -831,  -831,   728,  -831,   700,  -973,   742,   757,
    -832,  -833,   751,   522,  -833,   753,   755,   768,   759,   769,
     771,  -833,   772,   781,  -833,   805,   806,   807,  -833,   812,
    -425,   274,  -832,  -833,   817,   823,  -833,  -832,   695,   696,
     697,   698,   695,   696,   697,   698,   821,   822,   826,  -832,
     842,  -832,  -833,   824,   827,   695,   696,   697,   698,   832,
     843,   852,   836,  1679,  1005,   909,   910,   911,   912,   913,
     914,   915,   916,   837,  1531,   855,  -833,   854,   863,  -833,
    -833,   929,   864,   865,   463,   923,   869,   958,   784,  -833,
     959,   193,   961,   966,  1576,   981,   208,   507,  1375,   970,
      61,   971,   987,   996,  1000,  1015,  -833,  1020,   833,   104,
    1025,    62,   426,   210,  1027,  1026,   784,  1032,    63,  1035,
    -832,  -832,   148,  -832,   509,  1041,  1033,  1049,  -833,  1042,
    1043,    64,  1052,  -833,   479,  1058,  1640,  1063,   105,   106,
    1067,   107,  1238,   488,  1072,  -833,  1077,  -833,   108,   109,
     110,   111,   112,  1089,   113,  1090,  1111,   114,   115,   116,
     117,   118,   714,  1147,  1149,  1151,  1159,   423,  1171,   693,
     944,  1188,  1182,  1201,   517,  1191,  1467,  1468,  1210,  1184,
    1204,  1644,  1472,  1473,  1212,  1475,  1476,   701,  1225,  1239,
    1260,  1674,    65,  1480,  1481,  1482,  1483,  1228,  1484,  1485,
    1486,  1487,  1488,  1489,  1625,  1248,  1250,  1268,    66,  1519,
    1270,  1284,  1277,  1280,    78,  1293,  -833,  -833,  1281,  -833,
    1283,  1083,  1302,  1295,  1306,    67,  1305,  1307,  1308,   878,
     507,  1314,  1315,   187,  1319,  1320,  1321,  1323,  1675,  1324,
      68,  1519,  1331,  1332,    69,  1672,  1333,    70,  1334,  1336,
     880,  1337,  1338,  1339,  1358,    71,  1340,  1651,   693,  1360,
    1341,  1688,  1369,   694,   104,  1689,   881,  1387,   935,  1393,
    1383,   678,  1388,   148,    72,   683,   684,   688,   190,  1385,
    1389,  1390,  1394,  1401,  1404,  1519,  1406,  1722,  1411,  1413,
    1421,   147,  1425,   105,   106,  1426,   107,   148,  1427,  1455,
    1428,  1459,   882,   108,   109,   110,   111,   112,  1316,   113,
    1429,   883,   114,   115,   116,   117,   118,  1430,  1433,   884,
    1448,  1460,  1498,  1464,  1704,  1519,  1470,  1490,  1499,   255,
    1503,  1567,  1504,   935,   210,  1521,   509,   723,   885,  1519,
     886,   725,  1577,  1579,  1580,  1581,   193,  1582,  1583,  1584,
    1588,  1586,  1587,  1596,   887,  1598,   888,   889,   890,  1601,
    1605,  1754,  1631,  1606,  1519,  1632,  1607,  1519,  1649,  1609,
     509,   509,   129,  1610,  1611,  1612,   147,  1615,  1616,  1658,
    1617,  1618,   148,  1619,  1620,  1621,  1622,  1663,  1623,  1519,
    1624,  1654,    97,  1742,  1652,   509,  1633,  1655,  1656,   148,
    1657,  1664,  1668,  1665,  1670,  1671,   509,   891,  1666,   509,
    1667,  1697,  1691,   187,  1756,  1707,  1659,  1723,  1766,  1768,
    1773,  1744,  1698,  1752,    78,  1786,   147,  1792,  1731,  1793,
    1757,  1771,   148,    98,  1775,  1780,  1779,  1742,  1763,  1764,
     157,  1809,   507,   148,  1810,    99,  1813,  1816,  1817,  1820,
    1831,  1761,  1842,  1106,  1834,  1843,  1107,  1835,   457,   131,
     380,  1372,   135,   475,   960,   464,  1303,   853,   147,   381,
     100,  1088,  1301,  1289,   148,  1742,   507,   507,   818,  1590,
     584,   148,   382,  1227,   190,   947,   539,  1290,   383,   384,
     511,   525,  1297,   997,   892,   893,   894,   695,   696,   697,
     698,   507,  1030,  1589,   930,  1700,  1569,   877,  1762,   554,
     190,   556,   507,  1575,  1769,   507,   385,  1093,  1767,   386,
    1803,  1415,  1746,  1023,   488,  1795,  1706,  1717,   902,  1219,
    1524,  1730,  1397,   546,   537,   918,   190,   562,  1445,   101,
     925,   426,   840,   136,   504,   804,   737,   388,   389,   102,
     426,   932,  1064,   936,   937,   938,   939,   940,   932,   390,
     741,  1055,   988,   391,   287,   983,   999,  1203,  1695,  1637,
    1808,  1785,  1840,    43,   103,    44,   749,    47,   967,  1327,
    1410,  1362,  1197,  1278,   800,  1829,   244,   488,     0,   984,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
     950,     0,     0,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,   517,     0,   553,   104,  1108,     0,
     392,   105,   106,     0,   107,     0,   393,     0,     0,     0,
       0,   108,   109,   110,   111,   112,   394,   113,     0,     0,
     114,   115,   116,   117,   118,     0,   105,   106,     0,   107,
     395,   396,   397,   398,   201,   202,   108,   109,   110,   111,
     112,     0,   113,     0,   399,   114,   115,   116,   117,   118,
       0,     0,     0,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   105,   409,     0,   107,     0,     0,   410,   411,
       0,   412,   413,   414,   110,   111,   112,   415,   113,     0,
       0,   114,   115,   116,   117,   118,     0,  1116,     0,     0,
    1107,     0,   416,   417,   380,     0,     0,     0,   478,     0,
     419,     0,     0,   381,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   382,     0,     0,  1104,
    1105,  1110,   383,   384,  1118,  1119,  1122,     0,  1130,  1133,
    1135,  1138,  1141,  1144,  1145,  1146,     0,  1148,     0,  1150,
       0,     0,  1153,  1154,  1155,  1156,  1157,  1158,     0,     0,
     385,     0,     0,   386,     0,     0,     0,     0,     0,   426,
       0,     0,  1165,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   388,   389,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,   390,     0,     0,     0,   391,   105,   106,
       0,   107,     0,     0,     0,   488,     0,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,     0,     1,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     2,     0,     0,
       0,     0,  1108,     0,   392,     3,     0,     0,     0,     0,
     393,     0,     4,     0,     5,   100,     6,     0,     0,     0,
     394,     7,     0,     0,     0,     0,     0,     0,     8,     0,
       0,     0,     0,     0,   395,   396,   397,   398,   201,   202,
       0,     0,     9,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,    10,     0,     0,     0,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   105,   409,     0,   107,
       0,     0,   410,   411,     0,   412,   413,   414,   110,   111,
     112,   415,   113,     0,   101,   114,   115,   116,   117,   118,
       0,     0,     0,     0,   102,     0,   416,   417,     0,     0,
     932,     0,   478,     0,   419,     0,     0,   383,   276,     0,
       0,     0,  1325,  1326,     0,  1328,  1330,     0,     0,   103,
      11,     0,     0,  1335,     0,     0,     0,    12,     0,     0,
      13,     0,    14,    15,     0,   385,     0,   104,     0,  1120,
    1348,     0,  1107,     0,     0,     0,   380,     0,  1355,     0,
     426,     0,     0,     0,     0,   381,   426,    16,  1165,  1363,
     426,  1365,  1366,     0,   387,     0,   105,   106,   382,   107,
       0,     0,     0,     0,   383,   384,   108,   109,   110,   111,
     112,     0,   113,   488,     0,   114,   115,   116,   117,   118,
       0,     0,     0,     0,   459,     0,     0,     0,     0,   426,
       0,     0,   385,     0,     0,   386,    17,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,     0,     0,     0,   559,     0,     0,
       0,     0,  1416,   388,   389,     0,     0,     0,     0,   392,
     105,   106,     0,   107,     0,   390,     0,     0,     0,   391,
     108,   109,   110,   111,   112,   394,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,     0,     0,   395,
     396,   397,     0,   201,   202,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,   402,   403,   404,   405,   406,   407,
     408,     0,  1466,     0,  1108,     0,   392,   410,   411,     0,
       0,  1474,   393,     0,     0,     0,   415,     0,  1479,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1230,     0,     0,     0,   395,   396,   397,   398,
     201,   202,     0,     0,     0,     0,     0,     0,   426,   426,
     399,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   105,   409,
     488,   107,     0,     0,   410,   411,     0,   412,   413,   414,
     110,   111,   112,   415,   113,   947,     0,   114,   115,   116,
     117,   118,     0,     0,     0,     0,     0,     0,   416,   417,
     104,     0,     0,     0,   478,     0,   419,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,  1128,     0,
       0,  1107,     0,     0,     0,   380,     0,     0,     0,   105,
     106,     0,   107,     0,   381,     0,     0,     0,     0,   108,
     109,   110,   111,   112,     0,   113,     0,   382,   114,   115,
     116,   117,   118,   383,   384,     0,     0,     0,     0,   948,
       0,     0,     0,     0,     0,     0,  1613,  1614,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   385,     0,     0,   386,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,   949,     0,     0,     0,     0,
     950,     0,     0,     0,     0,     0,   951,     0,     0,     0,
       0,     0,   388,   389,     0,  1131,     0,   104,  1107,     0,
       0,     0,   380,     0,   390,   952,     0,  1653,   391,     0,
       0,   381,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   382,     0,   105,   106,     0,   107,
     383,   384,     0,     0,     0,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,     0,     0,     0,     0,     0,     0,   385,     0,
       0,   386,     0,  1108,     0,   392,     0,     0,     0,  1687,
       0,   393,     0,     0,     0,     0,     0,  1690,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,   388,
     389,     0,     0,     0,     0,   395,   396,   397,   398,   201,
     202,   390,     0,     0,     0,   391,     0,     0,     0,   399,
       0,     0,     0,  1703,     0,     0,     0,  1705,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   105,   409,     0,
     107,     0,     0,   410,   411,     0,   412,   413,   414,   110,
     111,   112,   415,   113,    83,     0,   114,   115,   116,   117,
     118,    84,     0,     0,     0,    85,    86,   416,   417,   104,
    1108,     0,   392,   478,    87,   419,     0,     0,   393,   276,
       0,     0,     0,   426,     0,  1749,    88,     0,   394,     0,
       0,     0,     0,   388,     0,     0,     0,     0,   105,   106,
       0,   107,   395,   396,   397,   398,   201,   202,   108,   109,
     110,   111,   112,     0,   113,     0,   399,   114,   115,   116,
     117,   118,     0,     0,     0,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   105,   409,   419,   107,     0,     0,
     410,   411,     0,   412,   413,   414,   110,   111,   112,   415,
     113,    89,     0,   114,   115,   116,   117,   118,     0,  1136,
       0,     0,  1107,     0,   416,   417,   380,     0,    90,     0,
     478,     0,   419,     0,     0,   381,   276,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,    92,   382,     0,
       0,     0,     0,     0,   383,   384,     0,    75,    93,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,   385,     0,     0,   386,     0,     0,   105,   409,
       0,   107,   454,     0,     0,     0,     0,   412,   413,   414,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,   388,   389,     0,  1139,     0,   104,  1107,
       0,     0,     0,   380,     0,   390,     0,     0,     0,   391,
       0,     0,   381,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   382,     0,   105,   106,     0,
     107,   383,   384,     0,     0,     0,     0,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,     0,     0,     0,     0,     0,   385,
       0,     0,   386,     0,  1108,     0,   392,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
     388,   389,     0,     0,     0,     0,   395,   396,   397,   398,
     201,   202,   390,     0,     0,     0,   391,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   105,   409,
       0,   107,     0,     0,   410,   411,     0,   412,   413,   414,
     110,   111,   112,   415,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,     0,     0,     0,   416,   417,
     104,  1108,     0,   392,   478,     0,   419,     0,     0,   393,
     276,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
     106,     0,   107,   395,   396,   397,   398,   201,   202,   108,
     109,   110,   111,   112,     0,   113,     0,   399,   114,   115,
     116,   117,   118,     0,     0,     0,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   105,   409,     0,   107,     0,
       0,   410,   411,     0,   412,   413,   414,   110,   111,   112,
     415,   113,     0,     0,   114,   115,   116,   117,   118,     0,
    1142,     0,     0,  1107,     0,   416,   417,   380,     0,     0,
       0,   478,     0,   419,     0,     0,   381,   276,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   382,
       0,     0,     0,     0,     0,   383,   384,     0,     0,   880,
       0,     0,     0,     0,     0,     0,     0,   693,   105,   106,
       0,   107,   694,     0,     0,   881,     0,     0,   108,   109,
     110,   111,   112,   385,   113,     0,   386,   114,   115,   116,
     117,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1811,     0,   419,     0,     0,     0,
       0,   882,     0,     0,   388,   389,     0,     0,     0,     0,
     883,     0,     0,     0,   380,     0,   390,     0,   884,     0,
     391,     0,     0,   381,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   382,   885,     0,   886,
       0,     0,   383,   384,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   887,     0,   888,   889,   890,     0,     0,
       0,   642,     0,     0,     0,     0,     0,     0,     0,     0,
     385,     0,     0,   386,     0,  1108,     0,   392,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,   388,   389,     0,     0,     0,   891,   395,   396,   397,
     398,   201,   202,   390,     0,     0,     0,   391,     0,     0,
       0,   399,     0,     0,     0,   870,     0,     0,     0,     0,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   105,
     409,     0,   107,     0,     0,   410,   411,     0,   412,   413,
     414,   110,   111,   112,   415,   113,     0,     0,   114,   115,
     116,   117,   118,     0,   685,     0,     0,     0,   644,   416,
     417,     0,     0,     0,   392,   478,     0,   419,     0,     0,
     393,   276,     0,     0,     0,     0,     0,     0,     0,     0,
     394,     0,     0,   892,   893,   894,   695,   696,   697,   698,
       0,     0,     0,     0,   395,   396,   397,   398,   201,   202,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   585,
       0,     0,     0,   586,     0,     0,     0,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   105,   409,     0,   107,
       0,   587,   410,   411,   588,   412,   413,   414,   110,   111,
     112,   415,   113,     0,   380,   114,   115,   116,   117,   118,
     871,   872,   873,   381,     0,     0,   416,   417,     0,     0,
       0,     0,   648,     0,   419,   104,   382,     0,   276,   641,
       0,     0,   383,   384,     0,   589,     0,     0,     0,     0,
       0,   590,   591,   592,     0,   593,   594,   595,     0,   596,
       0,   642,     0,     0,   105,   106,     0,   107,     0,     0,
     385,     0,     0,   386,   108,   109,   110,   111,   112,   597,
     113,   598,     0,   114,   115,   116,   117,   118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     975,   388,   389,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   599,   390,   104,     0,     0,   391,     0,     0,
     381,     0,     0,     0,     0,   643,     0,     0,     0,   600,
       0,     0,     0,   382,     0,     0,     0,     0,     0,   383,
     384,     0,     0,   105,   106,     0,   107,     0,     0,     0,
       0,   601,     0,   108,   109,   110,   111,   112,   642,   113,
       0,     0,   114,   115,   116,   117,   118,   385,   644,     0,
     386,     0,     0,     0,   392,     0,     0,     0,     0,  1681,
     393,     0,     0,     0,     0,     0,   602,   603,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,   388,   389,
       0,     0,     0,   604,   395,   396,   397,   398,   201,   202,
     390,     0,     0,     0,   391,     0,     0,     0,   399,     0,
       0,     0,   866,     0,     0,     0,     0,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   105,   409,     0,   107,
       0,     0,   410,   411,     0,   412,   413,   414,   110,   111,
     112,   415,   113,     0,     0,   114,   115,   645,   646,   647,
       0,     0,     0,     0,     0,   644,   416,   417,     0,     0,
       0,   392,   648,     0,   419,     0,     0,   393,   276,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,   396,   397,   398,   201,   202,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   105,   409,     0,   107,     0,     0,   410,
     411,     0,   412,   413,   414,   110,   111,   112,   415,   113,
     380,   104,   114,   115,   645,   646,   647,     0,     0,   381,
       0,     0,  1634,   416,   417,     0,     0,     0,     0,   648,
       0,   419,   382,     0,     0,   276,     0,     0,   383,   384,
     105,   106,     0,   107,     0,     0,     0,  1635,     0,     0,
     108,   109,   110,   111,   112,     0,   113,   642,     0,   114,
     115,   116,   117,   118,     0,     0,   385,     0,     0,   386,
       0,     0,     0,     0,  1643,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,   389,    75,
       0,   104,     0,     0,     0,  1636,     0,   380,     0,   390,
       0,     0,     0,   391,     0,     0,   381,     0,     0,     0,
       0,   643,     0,     0,     0,     0,     0,     0,     0,   382,
     105,   106,     0,   107,     0,   383,   384,     0,     0,   104,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,   642,     0,     0,     0,     0,     0,
       0,    76,     0,   385,   644,     0,   386,     0,   105,   106,
     392,   107,     0,     0,     0,     0,   393,     0,   108,   109,
     110,   111,   112,     0,   113,     0,   394,   114,   115,   116,
     117,   118,     0,     0,   388,   389,     0,     0,     0,     0,
     395,   396,   397,   398,   201,   202,   390,     0,     0,     0,
     391,     0,     0,     0,   399,     0,     0,     0,   866,     0,
       0,     0,     0,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   105,   409,     0,   107,     0,     0,   410,   411,
       0,   412,   413,   414,   110,   111,   112,   415,   113,     0,
       0,   114,   115,   645,   646,   647,     0,     0,     0,     0,
       0,   644,   416,   417,     0,     0,     0,   392,   648,     0,
     419,     0,     0,   393,   276,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,   396,   397,
     398,   201,   202,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   105,
     409,     0,   107,     0,     0,   410,   411,     0,   412,   413,
     414,   110,   111,   112,   415,   113,   380,   104,   114,   115,
     116,   117,   118,     0,     0,   381,     0,     0,     0,   416,
     417,     0,     0,     0,     0,   648,     0,   419,   382,     0,
       0,   276,     0,     0,   383,   384,   105,   106,     0,   107,
       0,     0,     0,     0,     0,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
       0,     0,   385,     0,     0,   386,     0,     0,   944,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,   387,     0,   388,   389,   381,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   390,     0,     0,   382,   391,
       0,   104,     0,     0,   383,   384,     0,     0,     0,     0,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,     0,     0,   177,   178,
     105,   106,   385,   107,     0,   386,   179,   180,     0,     0,
     108,   109,   110,   111,   112,   181,   113,   182,     0,   114,
     115,   116,   117,   118,     0,     0,   392,     0,     0,     0,
       0,   387,   393,   388,   389,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,   390,     0,     0,     0,   391,
       0,     0,     0,     0,     0,     0,   395,   396,   397,   398,
     201,   202,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   105,   409,
       0,   107,     0,     0,   410,   411,   685,   412,   413,   414,
     110,   111,   112,   415,   113,     0,   392,   114,   115,   116,
     117,   118,   393,     0,     0,     0,     0,     0,   416,   417,
     483,     0,   394,     0,   478,     0,   419,     0,     0,     0,
     276,     0,     0,     0,     0,     0,   395,   396,   397,   398,
     201,   202,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   105,   409,
       0,   107,     0,     0,   410,   411,     0,   412,   413,   414,
     110,   111,   112,   415,   113,   380,     0,   114,   115,   116,
     117,   118,     0,     0,   381,     0,     0,     0,   416,   417,
       0,     0,     0,     0,   478,     0,   419,   382,     0,     0,
     276,     0,     0,   383,   384,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   385,     0,     0,   386,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     387,     0,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,   390,     0,     0,     0,   391,     0,
       0,   381,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   382,     0,     0,     0,     0,     0,
     383,   384,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   385,     0,
       0,   386,     0,     0,     0,   392,     0,     0,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,   388,
     389,     0,     0,     0,     0,   395,   396,   397,   398,   201,
     202,   390,     0,     0,     0,   391,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   105,   409,     0,
     107,     0,     0,   410,   411,     0,   412,   413,   414,   110,
     111,   112,   415,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,     0,     0,     0,   416,   417,     0,
       0,     0,   392,   418,     0,   419,     0,     0,   393,   276,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,   677,     0,     0,     0,
       0,     0,   395,   396,   397,   398,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   105,   409,     0,   107,     0,     0,
     410,   411,     0,   412,   413,   414,   110,   111,   112,   415,
     113,   380,     0,   114,   115,   116,   117,   118,     0,     0,
     381,     0,     0,     0,   416,   417,     0,     0,     0,     0,
     478,     0,   419,   382,     0,     0,   276,     0,     0,   383,
     384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   385,     0,     0,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,   388,   389,
     381,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     390,     0,     0,   382,   391,     0,     0,     0,     0,   383,
     384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   385,     0,     0,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   685,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,     0,     0,     0,     0,   387,   393,   388,   389,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
     390,     0,     0,     0,   391,     0,     0,     0,     0,     0,
       0,   395,   396,   397,   398,   201,   202,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   105,   409,     0,   107,     0,     0,   410,
     411,     0,   412,   413,   414,   110,   111,   112,   415,   113,
       0,   392,   114,   115,   116,   117,   118,   393,     0,     0,
       0,     0,     0,   416,   417,     0,     0,   394,     0,   478,
       0,   419,     0,     0,     0,   276,     0,     0,     0,     0,
       0,   395,   396,   397,   398,   201,   202,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   105,   409,     0,   107,     0,     0,   410,
     411,     0,   412,   413,   414,   110,   111,   112,   415,   113,
     380,     0,   114,   115,   116,   117,   118,     0,     0,   381,
       0,     0,     0,   416,   417,     0,     0,     0,     0,   478,
       0,   419,   382,     0,     0,   276,     0,     0,   383,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   385,     0,     0,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,   388,   389,   381,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,   382,   391,     0,     0,     0,     0,   383,   384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   385,     0,     0,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,   393,   388,   389,     0,
       0,     0,     0,     0,     0,     0,   394,     0,     0,   390,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
     395,   396,   397,   398,   201,   202,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   105,   409,     0,   107,     0,     0,   410,   411,
       0,   412,   413,   414,   110,   111,   112,   415,   113,     0,
     392,   114,   115,   116,   117,   118,   393,     0,     0,     0,
       0,     0,   416,   417,     0,     0,   394,     0,   478,   941,
     419,     0,     0,     0,   276,     0,     0,  1134,     0,     0,
     395,   396,   397,   398,   201,   202,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   105,   409,     0,   107,     0,     0,   410,   411,
       0,   412,   413,   414,   110,   111,   112,   415,   113,   380,
       0,   114,   115,   116,   117,   118,     0,     0,   381,     0,
       0,     0,   416,   417,     0,     0,     0,     0,   478,     0,
     419,   382,     0,     0,   276,     0,     0,   383,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   385,     0,     0,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,   388,   389,   381,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   390,     0,
       0,   382,   391,     0,     0,     0,     0,   383,   384,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   385,     0,     0,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
       0,     0,     0,     0,     0,   393,   388,   389,     0,     0,
       0,     0,     0,     0,     0,   394,     0,     0,   390,     0,
       0,     0,   391,     0,     0,     0,     0,     0,     0,   395,
     396,   397,   398,   201,   202,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   105,   409,     0,   107,     0,     0,   410,   411,     0,
     412,   413,   414,   110,   111,   112,   415,   113,     0,   392,
     114,   115,   116,   117,   118,   393,     0,     0,     0,     0,
       0,   416,   417,     0,     0,   394,     0,   478,     0,   419,
       0,     0,     0,   276,     0,     0,  1329,     0,     0,   395,
     396,   397,   398,   201,   202,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   105,   409,     0,   107,     0,     0,   410,   411,     0,
     412,   413,   414,   110,   111,   112,   415,   113,   380,     0,
     114,   115,   116,   117,   118,     0,     0,   381,     0,     0,
       0,   416,   417,     0,     0,     0,     0,   478,     0,   419,
     382,     0,     0,   276,     0,     0,   383,   384,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   385,     0,     0,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   388,   389,     0,     0,     0,
       0,     0,     0,     0,     0,   380,     0,   390,     0,     0,
       0,   391,     0,     0,   381,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   382,     0,     0,
       0,     0,     0,   383,   384,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   385,     0,     0,   386,     0,     0,     0,   392,     0,
       0,     0,     0,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,   388,   389,     0,     0,     0,     0,   395,   396,
     397,   398,   201,   202,   390,     0,     0,     0,   391,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     105,   409,     0,   107,     0,     0,   410,   411,     0,   412,
     413,   414,   110,   111,   112,   415,   113,  1413,     0,   114,
     115,   116,   117,   118,     0,     0,     0,     0,     0,     0,
     416,   417,     0,     0,  1702,   392,   478,     0,   419,     0,
       0,   393,   276,     0,     0,     0,     0,     6,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,  1532,  1533,
       0,  1534,     0,     0,  1535,   395,   396,   397,   398,   201,
     202,     0,     0,     0,     0,     0,     0,     0,  1536,   399,
       0,     0,     0,     0,  1537,     0,     0,     0,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   105,   409,     0,
     107,     0,     0,   410,   411,     0,   412,   413,   414,   110,
     111,   112,   415,   113,     0,  1413,   114,   115,   116,   117,
     118,     0,     0,     0,     0,     0,     0,   416,   417,     0,
       0,     0,     0,   478,     0,   419,     0,     0,     0,   276,
       0,     0,     0,     0,  1538,     6,     0,     0,     0,     0,
     585,     0,     0,     0,   586,     0,  1532,  1533,     0,  1534,
       0,   784,  1535,    14,     0,     0,     0,     0,     0,     0,
       0,     0,   587,     0,     0,   588,  1536,     0,  1539,     0,
       0,     0,  1537,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,  1659,     0,  1540,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   589,     0,     0,     0,
       0,     0,   590,   591,   592,     0,   593,   594,   595,     0,
     596,     0,     0,     0,     0,  1541,   106,     0,   107,     0,
     585,     0,  1538,     0,   586,   108,   109,   110,   111,   112,
     597,   113,   598,     0,   114,  1542,   116,   117,   118,     0,
       0,    14,   587,     0,     0,   588,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1539,     0,     0,     0,
       0,     0,     0,   599,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1540,     0,     0,
     600,     0,     0,     0,   104,     0,   589,     0,     0,     0,
       0,     0,   590,   591,   592,     0,   593,   594,   595,     0,
     596,     0,   601,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1541,   106,     0,   107,   104,     0,     0,
     597,     0,   598,   108,   109,   110,   111,   112,     0,   113,
       0,     0,   114,  1542,   116,   117,   118,   602,   603,     0,
       0,     0,     0,     0,     0,     0,   105,   106,     0,   107,
       0,     0,     0,   599,   604,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
     600,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   601,     0,   856,     0,     0,     0,   586,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,     0,   857,     0,     0,     0,   587,     0,     0,   588,
       0,     0,     0,     0,     0,     0,     0,   602,   603,     0,
       0,     0,     0,     0,     0,     0,   105,   106,     0,   107,
       0,     0,     0,     0,   604,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
     589,     0,     0,     0,     0,     0,   590,   591,   592,     0,
     593,   594,   595,     0,   596,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   597,     0,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   599,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   600,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   602,   603,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   604
};

static const short yycheck[] =
{
       9,    16,    63,    12,     4,    54,   253,    16,   103,    18,
       4,    61,     0,   505,    62,   147,   350,   284,   358,    56,
     358,   137,   138,    84,    55,   418,   329,    72,    69,  1027,
     545,    68,   547,   354,   491,    66,    62,   137,   138,    88,
     555,    93,   700,    52,    53,    54,    55,    56,    57,    94,
     555,    92,   517,    62,    63,    64,   466,    66,    89,    68,
      69,   761,    71,    72,  1247,    96,    67,    82,   665,   927,
      87,    80,   718,    82,    83,    84,    91,    86,    87,    88,
      89,   637,    91,    92,    93,    94,   819,    96,   327,    90,
    1021,   363,   129,   751,   685,  1021,  1027,   322,   375,   151,
     262,  1027,   128,  1028,   130,   630,   278,   965,  1076,   975,
     272,  1021,  1037,   150,   305,   699,  1021,  1027,   149,   128,
     129,   130,  1027,   714,  1021,   142,   850,  1071,   217,  1053,
    1027,   226,  1365,   142,  1059,   757,   186,  1190,   147,   754,
     149,   150,   151,   677,   155,   793,   155,  1380,  1240,  1546,
       8,    10,     8,   750,     8,    23,   472,     8,  1161,     8,
     718,   211,     8,    15,  1167,  1393,   781,   643,     6,  1393,
      46,   418,    50,   305,   643,   790,  1453,  1393,   268,   648,
    1245,    64,   158,   140,  1393,    20,     8,    64,    84,    50,
      47,     8,    29,   157,  1286,    66,  1295,   531,   532,   533,
     534,    12,    47,   524,    19,    12,   191,  1194,     3,   177,
       3,    49,   133,     4,    46,   555,     8,   555,   313,  1077,
     309,     6,   259,     6,    14,   163,    23,    84,   843,   255,
     204,   268,   148,    47,    26,   727,   204,   279,   510,    31,
     289,   151,  1182,   235,  1184,   287,   255,     8,    29,     8,
     259,   243,   190,   262,   263,     8,   293,   231,   579,   268,
     170,    52,    11,   272,  1393,   186,    98,    26,   184,   254,
      84,    86,    31,  1217,   132,  1273,   132,    26,   132,   305,
     289,   132,    31,   132,   293,    80,   132,    80,     4,    47,
       9,   806,   188,   130,    91,    26,   153,   459,   266,    48,
      31,   351,   328,   353,  1396,  1292,   163,   271,   585,   335,
     132,   337,   958,   339,   271,   132,   342,   367,   557,   328,
     361,   111,   371,   134,   970,   971,   335,   134,   337,   360,
     339,   188,   369,   342,   466,    59,    52,  1262,  1204,    58,
     316,    26,  1273,   188,   878,   879,    31,  1273,   185,    26,
      69,   360,   361,  1418,    31,  1279,  1359,  1360,  1457,   181,
     369,  1758,   371,  1273,   181,   202,   605,    26,  1273,   966,
     827,   132,    31,   164,   188,   375,  1273,   212,   163,   132,
     163,   996,    26,   473,  1032,  1033,   262,    31,   922,   272,
    1573,   182,   270,   270,   870,  1792,  1793,   866,   665,   270,
     958,   870,  1679,  1631,   185,   721,   274,  1631,    26,   270,
     419,   270,   970,    31,   272,  1631,   272,   454,   272,   271,
    1817,   272,  1631,   272,  1039,   216,   272,   458,   465,   267,
     188,  1828,   469,  1081,  1831,   682,   473,    26,   101,   163,
     466,   472,    31,  1383,   691,   454,    66,    26,   164,   458,
     459,  1504,    31,  1627,   173,  1629,   465,  1690,  1191,  1081,
     469,   728,   551,   472,   473,   177,   477,   148,   477,   182,
      24,    24,   572,   265,   266,   267,   268,    65,   487,   529,
      33,   148,  1450,   750,  1409,   239,  1020,  1431,  1671,  1414,
     193,  1416,   204,  1585,   201,   249,  1499,  1692,  1693,  1591,
       4,    66,  1631,     7,    19,  1449,   265,   266,   267,   268,
     191,   102,  1686,   180,    53,    54,   265,   266,   267,   268,
     198,   204,  1222,    94,  1390,   575,  1451,  1111,   195,   555,
     769,  1087,   621,   772,  1663,  1268,   267,   268,  1733,   148,
    1464,  1724,   252,   768,   553,  1070,   555,   709,    52,   788,
     559,   854,   724,   963,   266,  1201,   747,   270,   146,    53,
      54,  1285,   182,  1097,  1098,   277,  1100,  1101,   271,   841,
     124,   124,   573,   254,   813,   238,   626,   627,  1707,  1762,
     265,   266,   267,   268,   175,   585,   863,   272,   265,   266,
     267,   268,   270,   146,   277,   272,   123,   269,  1523,   187,
     153,   200,  1776,   629,  1529,    67,   265,   266,   267,   268,
    1805,  1469,    26,   272,    71,   747,    98,    31,  1747,    81,
     629,   265,   266,   267,   268,   155,  1683,    89,   272,   638,
     204,    10,  1761,   733,   187,   177,   207,   208,   638,     7,
     123,    26,   124,   105,   638,   204,    31,   265,   266,   267,
     268,   668,   200,  1578,   272,   151,    26,  1786,   197,   668,
    1789,    31,   204,   204,   764,   765,   204,   194,    16,  1284,
    1595,   270,  1729,    25,  1008,   701,   265,   266,   267,   268,
      26,   164,  1811,   272,   693,    31,   265,   266,   267,   268,
     721,   700,   701,   272,  1228,   704,   270,   159,     6,   966,
     709,  1699,  1663,   197,  1404,   742,   202,   744,    88,   718,
     760,   761,   721,    26,    68,  1772,  1250,   726,    31,   176,
     729,   747,   270,    75,   266,  1316,   183,   753,   211,   271,
     271,   757,    26,   742,    82,   744,   277,    31,  1072,   204,
      26,    49,   751,   123,   753,    31,  1707,  1804,   757,    18,
      26,    89,   778,  1678,  1021,    31,    68,   109,    26,    33,
    1027,  1567,  1687,    31,  1405,    34,   767,   204,  1699,   778,
       6,   151,    46,  1699,   154,   127,   114,   270,  1171,   805,
     806,  1084,  1681,  1248,  1381,   947,  1747,  1026,   950,  1699,
     170,   865,   147,    26,  1699,   869,   805,   806,    31,  1038,
    1761,   266,  1699,  1728,    26,   842,   807,  1707,    67,    31,
      79,  1452,   812,    49,  1271,   852,   196,  1210,    26,  1058,
    1277,   204,    81,    31,   824,  1786,  1632,   206,  1789,   266,
       5,   963,  1731,   842,   172,   836,   837,   271,   839,   108,
    1345,    16,     6,   852,  1147,   129,   105,  1747,  1736,  1737,
    1811,   265,   266,   267,   268,   864,   235,   236,   272,   238,
    1394,   131,   124,   863,    86,   129,   245,   246,   247,   248,
     249,   182,   251,   102,  1773,   254,   255,   256,   257,   258,
     265,   266,   267,   268,   146,    49,  1786,   272,   271,  1789,
       6,   153,  1777,  1778,   277,   265,   266,   267,   268,   951,
     159,     6,   272,   204,    26,   129,   175,   952,   949,    31,
     139,  1811,   141,    11,    83,   239,   240,   948,    26,   265,
     266,   267,   268,    31,  1171,   187,   272,  1194,   149,    26,
     231,   129,    67,    49,    31,   164,  1824,   963,   947,   948,
     949,   950,   951,   952,    49,   166,    81,    47,  1833,   958,
      48,   120,   265,   266,   267,   268,  1056,   240,    94,   272,
     189,   970,   971,  1210,   133,   266,   975,   103,  1289,    66,
     105,   265,   266,   267,   268,  1633,    62,    63,   272,   265,
     266,   267,   268,   270,    84,     6,   272,   164,   275,   265,
     266,   267,   268,   270,   129,   272,   272,   265,   266,   267,
     268,   207,   208,   270,   272,  1345,  1273,  1345,   275,    26,
    1502,    83,   270,  1505,    31,    67,  1531,   275,   270,    20,
     272,  1260,    64,     0,   159,  1292,    47,   271,   211,    81,
     213,     8,   265,   266,   267,   268,   170,    26,    59,   272,
    1049,   207,    31,   265,   266,   267,   268,   269,  1074,    66,
    1050,    10,  1291,   105,  1063,  1081,  1050,   265,   266,   267,
     268,    64,   270,    84,    62,  1074,  1067,  1401,    57,   270,
      83,   272,  1081,  1189,  1190,  1191,   885,   129,   887,  1211,
     889,   890,  1083,   892,   893,   894,   206,    64,  1089,    26,
      67,    82,    40,    26,    31,   198,  1630,    74,    31,   270,
      77,   272,   148,    51,    81,  1597,    26,   159,    86,    86,
      58,    31,    89,   270,  1381,   272,   271,   270,  1652,   272,
      57,  1655,    66,   270,  1224,   272,  1226,   129,   105,    25,
      64,   664,   153,   666,   148,    83,   129,   670,   671,   672,
     107,     0,   163,   265,   266,   267,   268,   269,   252,     8,
      46,   182,   129,    86,   180,   132,   133,   196,   270,  1160,
     272,   270,   130,   272,   270,   142,   272,   188,   265,   266,
     267,   268,  1222,  1182,   270,  1184,   272,   163,   270,    75,
     272,  1420,   159,   203,   204,   205,   270,  1287,   272,  1189,
    1190,   270,  1201,   272,   270,  1204,   272,   270,    94,   272,
     148,  1182,  1211,  1184,   181,    64,   188,   103,    67,   186,
     200,   231,   232,   109,   103,    74,   137,   138,    77,    33,
    1459,   198,    81,   200,   269,   270,   157,    86,   269,   270,
      89,   127,   180,   270,   215,    42,   184,     0,   262,   118,
      32,   271,    10,  1735,   271,     8,   105,   207,   265,   266,
     267,   268,  1302,   271,   207,   271,  1306,   207,   182,   271,
     271,  1393,   271,   271,   271,   271,  1266,   271,    66,   274,
     129,   271,  1266,   132,   133,   271,   265,   266,   267,   268,
     148,  1413,   270,   142,   271,    66,   271,   270,   174,  1823,
      10,   262,   269,   270,   270,   272,   276,   276,   270,   128,
     159,    64,   276,    94,    67,   271,   270,   270,   251,    24,
      24,    74,    46,    10,    77,   182,   216,   143,    81,   143,
     182,   204,   181,    86,    23,   145,    89,   186,   265,   266,
     267,   268,   265,   266,   267,   268,   193,   193,    95,   198,
      64,   200,   105,   271,   271,   265,   266,   267,   268,   274,
      10,   129,   271,  1592,  1404,   218,   219,   220,   221,   222,
     223,   224,   225,   271,  1412,   270,   129,   157,   135,   132,
     133,   272,   128,   271,  1383,   197,   271,   182,    30,   142,
     210,  1390,    63,   271,  1421,   198,  1412,  1413,  1504,    66,
      40,    66,   194,    10,   248,   204,   159,   271,    23,   206,
      20,    51,   253,  1412,    46,   271,    30,    90,    58,    24,
     269,   270,  1421,   272,  1546,   132,    90,   164,   181,   132,
     132,    71,   162,   186,   275,    46,  1526,   164,   235,   236,
     271,   238,  1699,   284,   102,   198,   163,   200,   245,   246,
     247,   248,   249,   271,   251,    20,   271,   254,   255,   256,
     257,   258,   163,    10,   272,   270,    66,  1704,   270,    26,
     267,   131,   270,    66,   315,   113,  1314,  1315,   271,   270,
     270,  1532,  1320,  1321,    74,  1323,  1324,   270,    12,    45,
      10,  1576,   132,  1331,  1332,  1333,  1334,   271,  1336,  1337,
    1338,  1339,  1340,  1341,  1494,   247,   271,   168,   148,  1631,
     272,    10,   273,   272,  1504,   200,   269,   270,   272,   272,
     272,   140,    55,   186,    55,   165,   238,   272,   272,     8,
    1546,   271,   271,  1532,    98,   271,   271,   271,  1577,   271,
     180,  1663,   271,   271,   184,  1572,   271,   187,   271,   271,
      18,   271,   271,   271,    54,   195,   271,  1535,    26,   181,
     271,  1601,    73,    31,   206,  1605,    34,   134,  1567,    20,
     270,   412,   134,  1572,   214,   416,   417,   418,  1577,   272,
     134,    89,    77,   168,   136,  1707,   137,  1672,   192,    17,
     153,  1596,   132,   235,   236,   132,   238,  1596,   132,    24,
     132,   271,    70,   245,   246,   247,   248,   249,   163,   251,
     132,    79,   254,   255,   256,   257,   258,   132,   272,    87,
     272,   272,    54,   272,  1645,  1747,   272,   272,   181,   271,
     131,    26,   182,  1632,  1633,   142,  1758,   478,   106,  1761,
     108,   482,    83,   188,    84,   188,  1645,    47,    84,    47,
      98,   272,   272,    66,   122,   131,   124,   125,   126,   117,
     117,  1712,   270,   272,  1786,    26,   272,  1789,   269,   272,
    1792,  1793,    86,   272,   272,   272,  1681,   272,   272,    54,
     272,   272,  1681,   272,   272,   272,   272,    86,   272,  1811,
     272,   269,     6,  1698,   271,  1817,   276,   271,   269,  1698,
     269,   269,   272,   269,    98,    90,  1828,   175,   269,  1831,
     269,   114,   272,  1712,  1713,    86,   197,   129,   129,   129,
      89,   269,   271,   269,  1714,   156,  1731,   181,   270,    50,
    1714,   272,  1731,    47,   272,   270,   272,  1742,    47,   188,
     133,   271,  1758,  1742,  1783,    59,   269,    39,    50,   209,
      53,   270,     0,     6,   272,     0,     9,   270,   260,    73,
      13,  1187,    12,   272,   709,   263,  1087,   633,  1773,    22,
      84,   855,  1084,  1073,  1773,  1780,  1792,  1793,   579,  1440,
     355,  1780,    35,  1011,  1783,    74,   325,  1074,    41,    42,
     311,   320,  1081,   753,   262,   263,   264,   265,   266,   267,
     268,  1817,   789,  1439,   272,  1642,  1418,   648,  1722,   337,
    1809,   337,  1828,  1420,  1726,  1831,    69,  1816,  1725,    72,
    1764,  1241,  1704,   778,   665,  1759,  1650,  1661,   669,   998,
    1397,  1681,  1219,   330,   323,   676,  1835,   342,  1275,   153,
     681,   682,   618,    13,   297,   561,   493,   100,   101,   163,
     691,   692,   834,   694,   695,   696,   697,   698,   699,   112,
     497,   826,   742,   116,   140,   728,   755,   975,  1631,  1521,
    1780,  1742,  1835,     0,   188,     0,   505,     0,   717,  1125,
    1230,  1169,   967,  1063,   559,  1816,    86,   728,    -1,   729,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   750,
      -1,    -1,    -1,    -1,   755,    -1,    33,   206,   171,    -1,
     173,   235,   236,    -1,   238,    -1,   179,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,   189,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   235,   236,    -1,   238,
     203,   204,   205,   206,   207,   208,   245,   246,   247,   248,
     249,    -1,   251,    -1,   217,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,     6,    -1,    -1,
       9,    -1,   265,   266,    13,    -1,    -1,    -1,   271,    -1,
     273,    -1,    -1,    22,   277,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,   880,
     881,   882,    41,    42,   885,   886,   887,    -1,   889,   890,
     891,   892,   893,   894,   895,   896,    -1,   898,    -1,   900,
      -1,    -1,   903,   904,   905,   906,   907,   908,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,   920,
      -1,    -1,   923,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,     6,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   235,   236,
      -1,   238,    -1,    -1,    -1,   966,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,     7,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   171,    -1,   173,    36,    -1,    -1,    -1,    -1,
     179,    -1,    43,    -1,    45,    84,    47,    -1,    -1,    -1,
     189,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,   153,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,   163,    -1,   265,   266,    -1,    -1,
    1111,    -1,   271,    -1,   273,    -1,    -1,    41,   277,    -1,
      -1,    -1,  1123,  1124,    -1,  1126,  1127,    -1,    -1,   188,
     151,    -1,    -1,  1134,    -1,    -1,    -1,   158,    -1,    -1,
     161,    -1,   163,   164,    -1,    69,    -1,   206,    -1,     6,
    1151,    -1,     9,    -1,    -1,    -1,    13,    -1,  1159,    -1,
    1161,    -1,    -1,    -1,    -1,    22,  1167,   188,  1169,  1170,
    1171,  1172,  1173,    -1,    98,    -1,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,  1194,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,  1210,
      -1,    -1,    69,    -1,    -1,    72,   237,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   252,   253,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,  1243,   100,   101,    -1,    -1,    -1,    -1,   173,
     235,   236,    -1,   238,    -1,   112,    -1,    -1,    -1,   116,
     245,   246,   247,   248,   249,   189,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,    -1,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,  1292,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,  1313,    -1,   171,    -1,   173,   241,   242,    -1,
      -1,  1322,   179,    -1,    -1,    -1,   250,    -1,  1329,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   266,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,  1359,  1360,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
    1381,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    74,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,   265,   266,
     206,    -1,    -1,    -1,   271,    -1,   273,    -1,    -1,    -1,
     277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,   235,
     236,    -1,   238,    -1,    22,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    35,   254,   255,
     256,   257,   258,    41,    42,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,  1477,  1478,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,  1499,    -1,
      -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,     6,    -1,   206,     9,    -1,
      -1,    -1,    13,    -1,   112,   214,    -1,  1538,   116,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,   235,   236,    -1,   238,
      41,    42,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,   171,    -1,   173,    -1,    -1,    -1,  1600,
      -1,   179,    -1,    -1,    -1,    -1,    -1,  1608,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,
      -1,    -1,    -1,  1644,    -1,    -1,    -1,  1648,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    51,    -1,   254,   255,   256,   257,
     258,    58,    -1,    -1,    -1,    62,    63,   265,   266,   206,
     171,    -1,   173,   271,    71,   273,    -1,    -1,   179,   277,
      -1,    -1,    -1,  1704,    -1,  1706,    83,    -1,   189,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,   203,   204,   205,   206,   207,   208,   245,   246,
     247,   248,   249,    -1,   251,    -1,   217,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   273,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,   148,    -1,   254,   255,   256,   257,   258,    -1,     6,
      -1,    -1,     9,    -1,   265,   266,    13,    -1,   165,    -1,
     271,    -1,   273,    -1,    -1,    22,   277,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,   184,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,   204,   195,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,   235,   236,
      -1,   238,   180,    -1,    -1,    -1,    -1,   244,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   100,   101,    -1,     6,    -1,   206,     9,
      -1,    -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,   235,   236,    -1,
     238,    41,    42,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,   171,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,   265,   266,
     206,   171,    -1,   173,   271,    -1,   273,    -1,    -1,   179,
     277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     236,    -1,   238,   203,   204,   205,   206,   207,   208,   245,
     246,   247,   248,   249,    -1,   251,    -1,   217,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
       6,    -1,    -1,     9,    -1,   265,   266,    13,    -1,    -1,
      -1,   271,    -1,   273,    -1,    -1,    22,   277,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   235,   236,
      -1,   238,    31,    -1,    -1,    34,    -1,    -1,   245,   246,
     247,   248,   249,    69,   251,    -1,    72,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,    -1,   273,    -1,    -1,    -1,
      -1,    70,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    13,    -1,   112,    -1,    87,    -1,
     116,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,   106,    -1,   108,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,   175,   203,   204,   205,
     206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,   163,    -1,    -1,    -1,   167,   265,
     266,    -1,    -1,    -1,   173,   271,    -1,   273,    -1,    -1,
     179,   277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,   262,   263,   264,   265,   266,   267,   268,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    19,
      -1,    -1,    -1,    23,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    41,   241,   242,    44,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    13,   254,   255,   256,   257,   258,
     259,   260,   261,    22,    -1,    -1,   265,   266,    -1,    -1,
      -1,    -1,   271,    -1,   273,   206,    35,    -1,   277,    38,
      -1,    -1,    41,    42,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,    60,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      69,    -1,    -1,    72,   245,   246,   247,   248,   249,   119,
     251,   121,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,   152,   112,   206,    -1,    -1,   116,    -1,    -1,
      22,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,   191,    -1,   245,   246,   247,   248,   249,    60,   251,
      -1,    -1,   254,   255,   256,   257,   258,    69,   167,    -1,
      72,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,   271,
     179,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,   243,   203,   204,   205,   206,   207,   208,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,    -1,   167,   265,   266,    -1,    -1,
      -1,   173,   271,    -1,   273,    -1,    -1,   179,   277,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      13,   206,   254,   255,   256,   257,   258,    -1,    -1,    22,
      -1,    -1,    89,   265,   266,    -1,    -1,    -1,    -1,   271,
      -1,   273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,   114,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    60,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   204,
      -1,   206,    -1,    -1,    -1,   172,    -1,    13,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
     235,   236,    -1,   238,    -1,    41,    42,    -1,    -1,   206,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    60,    -1,    -1,    -1,    -1,    -1,
      -1,   266,    -1,    69,   167,    -1,    72,    -1,   235,   236,
     173,   238,    -1,    -1,    -1,    -1,   179,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,   189,   254,   255,   256,
     257,   258,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,    -1,
      -1,   167,   265,   266,    -1,    -1,    -1,   173,   271,    -1,
     273,    -1,    -1,   179,   277,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    13,   206,   254,   255,
     256,   257,   258,    -1,    -1,    22,    -1,    -1,    -1,   265,
     266,    -1,    -1,    -1,    -1,   271,    -1,   273,    35,    -1,
      -1,   277,    -1,    -1,    41,    42,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    35,   116,
      -1,   206,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,    -1,    -1,   233,   234,
     235,   236,    69,   238,    -1,    72,   241,   242,    -1,    -1,
     245,   246,   247,   248,   249,   250,   251,   252,    -1,   254,
     255,   256,   257,   258,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    98,   179,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,   163,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,   173,   254,   255,   256,
     257,   258,   179,    -1,    -1,    -1,    -1,    -1,   265,   266,
     267,    -1,   189,    -1,   271,    -1,   273,    -1,    -1,    -1,
     277,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    13,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    22,    -1,    -1,    -1,   265,   266,
      -1,    -1,    -1,    -1,   271,    -1,   273,    35,    -1,    -1,
     277,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
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
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,    -1,    -1,    -1,   265,   266,    -1,
      -1,    -1,   173,   271,    -1,   273,    -1,    -1,   179,   277,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    13,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      22,    -1,    -1,    -1,   265,   266,    -1,    -1,    -1,    -1,
     271,    -1,   273,    35,    -1,    -1,   277,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    98,   179,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,   173,   254,   255,   256,   257,   258,   179,    -1,    -1,
      -1,    -1,    -1,   265,   266,    -1,    -1,   189,    -1,   271,
      -1,   273,    -1,    -1,    -1,   277,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      13,    -1,   254,   255,   256,   257,   258,    -1,    -1,    22,
      -1,    -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,
      -1,   273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,    -1,
      -1,    -1,   265,   266,    -1,    -1,   189,    -1,   271,   272,
     273,    -1,    -1,    -1,   277,    -1,    -1,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    13,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    22,    -1,
      -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,
     273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,    -1,
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
     254,   255,   256,   257,   258,   179,    -1,    -1,    -1,    -1,
      -1,   265,   266,    -1,    -1,   189,    -1,   271,    -1,   273,
      -1,    -1,    -1,   277,    -1,    -1,   200,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    13,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    22,    -1,    -1,
      -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,   273,
      35,    -1,    -1,   277,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    17,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,    -1,    -1,    -1,
     265,   266,    -1,    -1,   269,   173,   271,    -1,   273,    -1,
      -1,   179,   277,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,    61,    -1,    -1,    64,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,   217,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    17,   254,   255,   256,   257,
     258,    -1,    -1,    -1,    -1,    -1,    -1,   265,   266,    -1,
      -1,    -1,    -1,   271,    -1,   273,    -1,    -1,    -1,   277,
      -1,    -1,    -1,    -1,   144,    47,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    23,    -1,    58,    59,    -1,    61,
      -1,    30,    64,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    44,    78,    -1,   178,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,   199,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,
      19,    -1,   144,    -1,    23,   245,   246,   247,   248,   249,
     119,   251,   121,    -1,   254,   255,   256,   257,   258,    -1,
      -1,   163,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,    -1,
     169,    -1,    -1,    -1,   206,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,   206,    -1,    -1,
     119,    -1,   121,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,   152,   243,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,    -1,    19,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      85,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,    -1,
      95,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   243
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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-2]; }
    break;
case 80:
{ yyval = (DSQL_NOD) NULL; }
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
		  		(DSQL_NOD) NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
    break;
case 136:
{ yyval = (DSQL_NOD) NULL; }
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
{yyval = (DSQL_NOD) NULL;}
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
{yyval = (DSQL_NOD) NULL;}
    break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 168:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
    break;
case 169:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
    break;
case 170:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
    break;
case 171:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
    break;
case 172:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;
case 173:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;
case 174:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
    break;
case 176:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
    break;
case 177:
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
    break;
case 179:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 180:
{ 
		         check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
                                                (DSQL_NOD) lex.g_file); }
    break;
case 181:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
    break;
case 184:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
    break;
case 185:
{ lex.g_file  = make_file ();}
    break;
case 190:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
    break;
case 191:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
    break;
case 198:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 199:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 200:
{ yyval = yyvsp[0]; }
    break;
case 201:
{ yyval = yyvsp[0]; }
    break;
case 202:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 204:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 210:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 213:
{ yyval = NULL; }
    break;
case 214:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                            yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 215:
{ yyval = yyvsp[0]; }
    break;
case 216:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 217:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 218:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 219:
{ yyval = yyvsp[-1]; }
    break;
case 220:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 221:
{ yyval = yyvsp[0]; }
    break;
case 222:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 226:
{ yyval = yyvsp[0]; }
    break;
case 227:
{ yyval = yyvsp[0]; }
    break;
case 228:
{ yyval = yyvsp[0]; }
    break;
case 229:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 232:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 233:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 234:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 235:
{ yyval = make_node (nod_foreign, e_for_count,
                        make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 237:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 238:
{ yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
    break;
case 239:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 240:
{ yyval = yyvsp[0]; }
    break;
case 241:
{ yyval = NULL ;}
    break;
case 246:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 247:
{ yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 248:
{ yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
			         yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 249:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 250:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 251:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 252:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 253:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 254:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 255:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 256:
{ yyval = NULL;}
    break;
case 257:
{ yyval = yyvsp[0];}
    break;
case 258:
{ yyval = yyvsp[0];}
    break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
    break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
    break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
    break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
    break;
case 263:
{ yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 264:
{ yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 265:
{ yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 266:
{ yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 267:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 268:
{ yyval = NULL; }
    break;
case 269:
{ yyval = yyvsp[0]; }
    break;
case 270:
{ yyval = NULL; }
    break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 273:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 274:
{ yyval = make_list (yyvsp[0]); }
    break;
case 275:
{ yyval = NULL; }
    break;
case 277:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
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
case 285:
{ yyval = yyvsp[-1]; }
    break;
case 286:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 287:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 288:
{ yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
    break;
case 290:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 306:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
    break;
case 307:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 309:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
    break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 311:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
    break;
case 312:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 313:
{ yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
    break;
case 316:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
    break;
case 317:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
    break;
case 318:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
    break;
case 319:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 320:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 321:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 322:
{ yyval = NULL; }
    break;
case 323:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
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
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
    break;
case 336:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;
case 337:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;
case 338:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 339:
{ yyval = NULL; }
    break;
case 341:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 342:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 345:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 346:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 347:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 348:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 349:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 350:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 351:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 352:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
    break;
case 353:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;
case 354:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 355:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 356:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 357:
{ yyval = make_list (yyvsp[0]); }
    break;
case 359:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 362:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 363:
{ yyval = yyvsp[-1]; }
    break;
case 364:
{ lex.beginning = lex_position(); }
    break;
case 365:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;
case 366:
{ lex.beginning = lex.last_token; }
    break;
case 367:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
    break;
case 368:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 369:
{ yyval = 0; }
    break;
case 370:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 371:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 372:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 373:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 374:
{ yyval = NULL; }
    break;
case 375:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 376:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;
case 377:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;
case 378:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
    break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
    break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
    break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
    break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
    break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
    break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
    break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
    break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 394:
{ yyval = NULL; }
    break;
case 395:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 396:
{ yyval = yyvsp[0]; }
    break;
case 397:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 398:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 399:
{ yyval = yyvsp[0]; }
    break;
case 400:
{ yyval = yyvsp[0]; }
    break;
case 401:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 402:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 403:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 404:
{ yyval = yyvsp[0]; }
    break;
case 405:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 407:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 408:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
    break;
case 409:
{ yyval = yyvsp[0]; }
    break;
case 410:
{ yyval = yyvsp[0]; }
    break;
case 411:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 412:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 413:
{ yyval = yyvsp[0]; }
    break;
case 414:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 416:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 417:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 418:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 419:
{ yyval = yyvsp[0]; }
    break;
case 420:
{ yyval = yyvsp[0]; }
    break;
case 421:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 422:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 423:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 424:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 448:
{ yyval = NULL; }
    break;
case 449:
{ yyval = NULL; }
    break;
case 450:
{ yyval = NULL; }
    break;
case 451:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 452:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;
case 453:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 454:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 455:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 456:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 457:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 458:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
    break;
case 460:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 461:
{ yyval = yyvsp[0]; }
    break;
case 462:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;
case 463:
{ yyval = yyvsp[0]; }
    break;
case 464:
{ yyval = yyvsp[0]; }
    break;
case 466:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 467:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 469:
{ yyval = NULL; }
    break;
case 471:
{ yyval = NULL; }
    break;
case 472:
{ yyval = yyvsp[0]; }
    break;
case 473:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 474:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 475:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 476:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 477:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 478:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 479:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 480:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 481:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 482:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 483:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 484:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 489:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
    break;
case 490:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
    break;
case 492:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 493:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 494:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 500:
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
case 501:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 502:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 503:
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
case 504:
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
case 505:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 508:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;
case 512:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
    break;
case 513:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 514:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
    break;
case 515:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 516:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 517:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
    break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
    break;
case 533:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 534:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;
case 535:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
    break;
case 536:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
    break;
case 537:
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-1];
			}
    break;
case 538:
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT) yyvsp[-1];
			}
    break;
case 541:
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
case 542:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 545:
{ yyval = yyvsp[-1]; }
    break;
case 546:
{ yyval = 0; }
    break;
case 550:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 551:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 552:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 556:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 557:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 558:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 559:
{ yyval = 0; }
    break;
case 560:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 563:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 564:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 567:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 568:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 570:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 571:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 573:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 575:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 580:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 581:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 582:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 583:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 584:
{ yyval = yyvsp[0];}
    break;
case 586:
{ yyval = yyvsp[0];}
    break;
case 587:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 590:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 591:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 592:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 593:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 594:
{ yyval = 0; }
    break;
case 595:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 596:
{ yyval = (DSQL_NOD) NOD_SHARED; }
    break;
case 597:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;
case 598:
{ yyval = (DSQL_NOD) 0; }
    break;
case 599:
{ yyval = (DSQL_NOD) NOD_READ; }
    break;
case 600:
{ yyval = (DSQL_NOD) NOD_WRITE; }
    break;
case 602:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 603:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 604:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 605:
{ yyval = 0; }
    break;
case 607:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 608:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 609:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 610:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 611:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 612:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 613:
{ yyval = make_list (yyvsp[0]); }
    break;
case 614:
{ yyval = 0; }
    break;
case 616:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 617:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 618:
{ yyval = 0; }
    break;
case 619:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 620:
{ yyval = 0; }
    break;
case 621:
{ yyval = MAKE_constant((STR)NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 622:
{ yyval = MAKE_constant((STR)NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 623:
{ yyval = yyvsp[-1]; }
    break;
case 624:
{ yyval = 0; }
    break;
case 625:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 626:
{ yyval = 0; }
    break;
case 627:
{ yyval = yyvsp[0]; }
    break;
case 628:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 630:
{ yyval = 0; }
    break;
case 631:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 632:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 633:
{ lex.limit_clause = true; }
    break;
case 634:
{ lex.limit_clause = false; }
    break;
case 635:
{ lex.first_detection = true; }
    break;
case 636:
{ lex.first_detection = false; }
    break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 638:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;
case 639:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;
case 640:
{ yyval = 0; }
    break;
case 641:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 642:
{ yyval = yyvsp[-2]; }
    break;
case 643:
{ yyval = yyvsp[-1]; }
    break;
case 644:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 645:
{ yyval = yyvsp[-1]; }
    break;
case 646:
{ yyval = yyvsp[0]; }
    break;
case 647:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 648:
{ yyval = 0; }
    break;
case 649:
{ yyval = make_list (yyvsp[0]); }
    break;
case 650:
{ yyval = 0; }
    break;
case 652:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 654:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 655:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 656:
{ yyval = make_list (yyvsp[0]); }
    break;
case 658:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 661:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 662:
{ yyval = yyvsp[-1]; }
    break;
case 663:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
    break;
case 664:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 665:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 666:
{ yyval = NULL; }
    break;
case 668:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 672:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 673:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 674:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 675:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 676:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 677:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 678:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 679:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 680:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 681:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 682:
{ yyval = make_list (yyvsp[0]); }
    break;
case 683:
{ yyval = 0; }
    break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 690:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
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
{ yyval = yyvsp[0]; }
    break;
case 699:
{ yyval = 0; }
    break;
case 700:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 701:
{ yyval = 0; }
    break;
case 702:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 703:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 704:
{ yyval = 0; }
    break;
case 705:
{ yyval = 0; }
    break;
case 707:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 708:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 712:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 713:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 714:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
    break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 717:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 718:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 723:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 724:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 725:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 728:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 729:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 732:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 735:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 736:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 737:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 738:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 742:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 743:
{ yyval = yyvsp[0]; }
    break;
case 745:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 748:
{ yyval = NULL; }
    break;
case 749:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 753:
{ yyval = NULL; }
    break;
case 754:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 758:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 759:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 760:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 764:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 766:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 767:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 775:
{ yyval = yyvsp[-1]; }
    break;
case 776:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 789:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 796:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 797:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 798:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 799:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 800:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 801:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 802:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 803:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 804:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 805:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 806:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 807:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 808:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 809:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 810:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 815:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 817:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 819:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 821:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 823:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 825:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 827:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 828:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 829:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 835:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 836:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 838:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 839:
{ yyval = yyvsp[-1]; }
    break;
case 840:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 841:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 851:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 852:
{ yyval = yyvsp[0]; }
    break;
case 853:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 854:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 856:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 857:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 858:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 859:
{ yyval = yyvsp[-1]; }
    break;
case 860:
{ yyval = yyvsp[-1]; }
    break;
case 864:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 865:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 866:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
    break;
case 867:
{ yyval = yyvsp[0]; }
    break;
case 868:
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
case 869:
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
case 870:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 871:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 873:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 875:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 880:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 882:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;
case 883:
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
case 884:
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
case 885:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 886:
{ yyval = make_parameter (); }
    break;
case 887:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 888:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 889:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 893:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 894:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 895:
{ yyval = yyvsp[0]; }
    break;
case 896:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 898:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 899:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 901:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 902:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 904:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 905:
{ yyval = yyvsp[0];}
    break;
case 910:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 911:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 912:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 915:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 916:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 917:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 918:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 919:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 920:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 921:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
    break;
case 922:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 923:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
    break;
case 924:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
    break;
case 925:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 926:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 927:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 928:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 931:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 932:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 935:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 936:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 937:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 939:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 940:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 941:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 956:
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
node->nod_arg[0] = (DSQL_NOD)lex.param_number++;

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
end_chain = (DSQL_NOD) NULL;
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
		yylval = (DSQL_NOD) number;
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
