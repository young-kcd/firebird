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



#define	YYFINAL		1857
#define	YYFLAG		-32768
#define	YYNTBASE	280

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 517 ? yytranslate[x] : 695)

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
    1714,  1716,  1717,  1721,  1727,  1729,  1731,  1734,  1738,  1740,
    1743,  1747,  1748,  1750,  1752,  1754,  1760,  1766,  1773,  1775,
    1777,  1779,  1782,  1787,  1789,  1790,  1796,  1798,  1799,  1803,
    1806,  1808,  1809,  1812,  1813,  1815,  1816,  1820,  1822,  1823,
    1825,  1828,  1830,  1832,  1834,  1836,  1839,  1842,  1844,  1847,
    1851,  1853,  1855,  1859,  1863,  1865,  1868,  1872,  1874,  1877,
    1878,  1881,  1883,  1885,  1886,  1888,  1890,  1892,  1896,  1899,
    1903,  1904,  1906,  1910,  1915,  1920,  1922,  1926,  1931,  1935,
    1936,  1938,  1942,  1946,  1948,  1950,  1951,  1953,  1955,  1960,
    1961,  1965,  1966,  1969,  1970,  1973,  1974,  1984,  1995,  1996,
    1997,  1998,  1999,  2003,  2006,  2008,  2009,  2013,  2019,  2023,
    2026,  2032,  2035,  2037,  2039,  2041,  2043,  2045,  2049,  2051,
    2054,  2058,  2061,  2063,  2067,  2069,  2071,  2078,  2082,  2086,
    2089,  2093,  2094,  2096,  2100,  2102,  2104,  2106,  2109,  2111,
    2113,  2115,  2118,  2120,  2123,  2125,  2128,  2129,  2133,  2134,
    2136,  2140,  2142,  2144,  2146,  2148,  2152,  2154,  2156,  2158,
    2161,  2162,  2165,  2166,  2169,  2170,  2175,  2177,  2180,  2182,
    2184,  2185,  2187,  2191,  2194,  2196,  2198,  2201,  2203,  2208,
    2211,  2213,  2217,  2226,  2232,  2234,  2238,  2240,  2242,  2247,
    2252,  2257,  2259,  2261,  2267,  2273,  2275,  2279,  2283,  2285,
    2287,  2295,  2303,  2309,  2313,  2314,  2316,  2318,  2320,  2323,
    2324,  2326,  2328,  2330,  2331,  2335,  2337,  2341,  2343,  2344,
    2348,  2350,  2354,  2356,  2360,  2364,  2366,  2368,  2372,  2374,
    2377,  2379,  2383,  2387,  2389,  2392,  2396,  2400,  2404,  2408,
    2410,  2414,  2417,  2419,  2421,  2423,  2425,  2427,  2429,  2431,
    2433,  2435,  2437,  2441,  2445,  2449,  2453,  2457,  2461,  2465,
    2469,  2476,  2483,  2490,  2497,  2504,  2511,  2518,  2525,  2532,
    2539,  2546,  2553,  2560,  2567,  2574,  2581,  2583,  2585,  2591,
    2598,  2602,  2607,  2613,  2620,  2624,  2629,  2633,  2638,  2642,
    2647,  2652,  2658,  2663,  2668,  2672,  2677,  2679,  2681,  2683,
    2685,  2687,  2689,  2691,  2695,  2699,  2710,  2721,  2723,  2725,
    2727,  2729,  2731,  2733,  2735,  2737,  2739,  2742,  2745,  2749,
    2753,  2757,  2761,  2765,  2769,  2773,  2777,  2779,  2781,  2783,
    2785,  2789,  2791,  2793,  2795,  2797,  2799,  2804,  2806,  2810,
    2812,  2815,  2817,  2819,  2821,  2823,  2825,  2827,  2829,  2832,
    2835,  2838,  2840,  2842,  2844,  2846,  2848,  2850,  2852,  2854,
    2856,  2858,  2861,  2863,  2866,  2868,  2870,  2872,  2874,  2876,
    2879,  2881,  2883,  2885,  2887,  2889,  2894,  2900,  2906,  2912,
    2918,  2924,  2930,  2936,  2942,  2948,  2954,  2961,  2968,  2975,
    2984,  2989,  2994,  2998,  3005,  3007,  3009,  3016,  3023,  3025,
    3027,  3032,  3039,  3044,  3050,  3054,  3060,  3065,  3071,  3073,
    3075,  3077,  3079,  3081,  3083,  3085,  3087,  3089,  3091,  3093,
    3095,  3096,  3098,  3100,  3102,  3104,  3106,  3108,  3110,  3112,
    3114,  3116,  3118,  3120,  3122,  3124,  3126,  3128,  3130,  3132,
    3134,  3136,  3138,  3140,  3142,  3144,  3146,  3148,  3150,  3152,
    3154,  3156,  3158,  3160,  3162,  3164,  3166,  3168,  3170,  3172,
    3174,  3176,  3178
};
static const short yyrhs[] =
{
     281,     0,   281,   271,     0,   457,     0,   596,     0,   512,
       0,   311,     0,   301,     0,   586,     0,   479,     0,   282,
       0,   584,     0,   434,     0,   313,     0,   315,     0,   291,
       0,   513,     0,   506,     0,   535,     0,   504,     0,   590,
       0,    43,   643,     0,    73,   284,   129,   283,   182,   295,
     288,     0,    73,   286,   129,   148,   290,   182,   295,   288,
       0,    73,   284,   129,   283,   182,   293,     0,    73,   286,
     129,   148,   290,   182,   293,     0,    73,   297,   182,   299,
     289,     0,   568,     0,   180,   568,     0,     6,     0,     6,
     147,     0,   285,     0,   287,     0,   285,   272,   287,     0,
      59,     0,   163,     0,    84,     0,    47,     0,   188,   601,
       0,   153,   601,     0,   200,    73,   131,     0,     0,   200,
     210,   131,     0,     0,   684,     0,   158,   292,   284,   129,
     283,    66,   295,     0,   158,   292,   286,   129,   148,   290,
      66,   295,     0,   158,   284,   129,   283,    66,   295,     0,
     158,   286,   129,   148,   290,    66,   295,     0,   158,   284,
     129,   283,    66,   293,     0,   158,   286,   129,   148,   290,
      66,   293,     0,   158,   297,    66,   299,     0,    73,   131,
      64,     0,   294,     0,   293,   272,   294,     0,   293,   272,
     296,     0,   295,   272,   294,     0,   148,   684,     0,   184,
     688,     0,   195,   691,     0,   214,   685,     0,   296,     0,
     295,   272,   296,     0,   689,     0,   189,   689,     0,    74,
     689,     0,   298,     0,   297,   272,   298,     0,   685,     0,
     300,     0,   299,   272,   300,     0,   689,     0,   189,   689,
       0,    45,   302,     0,    63,   310,     0,    62,    68,   303,
       0,   670,   305,   157,   308,    55,   642,   117,   642,     0,
     486,     0,    19,     0,    37,   273,   646,   274,   492,     0,
       0,   306,     0,   273,   306,   274,     0,   307,     0,   306,
     272,   307,     0,   374,   304,     0,   374,   304,    20,   238,
       0,   309,     0,   273,   309,   274,     0,   374,   304,     0,
     374,   304,   212,     0,   374,   304,    20,   101,     0,   374,
     304,    20,   238,     0,   140,   646,     0,   679,    82,   491,
     135,   491,    55,   642,   117,   642,     0,    36,   312,     0,
      58,   678,   642,     0,   317,   540,    83,   682,   129,   568,
     318,     0,   148,   393,     0,   180,   361,     0,   184,   449,
       0,   195,   435,     0,    71,   334,     0,    40,   336,     0,
      51,   325,     0,   165,   319,     0,   214,   335,     0,   237,
     314,     0,   148,   394,     0,   180,   362,     0,   195,   436,
       0,    36,   132,     7,   316,     0,   148,   395,     0,   184,
     450,     0,   187,     0,     0,   603,     0,   602,     0,   368,
     273,   446,   631,   447,   274,     0,   646,   320,   321,   642,
     322,   323,     0,   111,     0,    14,     0,     0,     0,    32,
       0,     0,   102,   337,   649,   360,     0,     0,   324,     0,
     349,     0,   324,   349,     0,   371,   326,   481,   446,   327,
     447,   328,   370,     0,    10,     0,     0,    46,   446,   376,
       0,     0,     0,   329,     0,   330,     0,   329,   330,     0,
     331,     0,   332,     0,   333,     0,   124,    98,     0,   446,
      24,   273,   610,   274,   447,     0,   681,     0,   685,     0,
     338,   339,   342,     0,     0,   264,     0,   642,     0,     0,
     340,     0,   341,     0,   340,   341,     0,   139,   337,   646,
       0,   102,   337,   649,   360,     0,   189,   642,     0,   141,
     642,     0,   164,   118,   642,     0,     0,   343,     0,   344,
       0,   343,   344,     0,   349,     0,   346,     0,   345,     0,
      46,    23,   164,   672,     0,   263,    94,   642,     0,    75,
     337,   649,     0,    25,   337,   649,     0,   127,   337,   646,
       0,   109,   337,   647,     0,   348,     0,   347,     0,   103,
     273,   350,   274,   136,   351,     0,   103,    15,   351,     0,
     103,     0,   355,   642,   356,     0,   351,     0,   350,   272,
     351,     0,   352,   353,     0,   642,     0,     0,   353,   354,
       0,   168,   337,   649,     0,    94,     0,     0,   357,     0,
     358,     0,   357,   358,     0,   175,   359,   649,     0,   102,
     337,   649,   360,     0,     0,    12,     0,    12,   137,     0,
       0,   137,     0,   138,     0,   568,   363,   273,   364,   274,
       0,   568,   363,   273,   364,   274,     0,    62,    94,   642,
       0,    62,   642,     0,     0,   365,     0,   364,   272,   365,
       0,   366,     0,   381,     0,   371,   369,   375,   447,   377,
     370,     0,   371,   482,   367,     0,   371,   367,     0,   368,
     273,   446,   631,   447,   274,     0,    30,    20,     0,    30,
       0,   481,   446,     0,   608,   444,     0,    26,   673,     0,
       0,   608,     0,   608,     0,   374,   481,     0,     0,    46,
     376,     0,     0,   635,     0,   639,     0,   640,     0,   641,
       0,   669,     0,   632,     0,     0,   378,     0,   379,     0,
     378,   379,     0,   382,   380,     0,   124,    98,     0,   153,
     568,   601,   389,   387,     0,   388,     0,   187,   387,     0,
     146,    90,   387,     0,   382,   383,     0,    33,   675,     0,
       0,   384,     0,   385,     0,   386,     0,   388,     0,   187,
     602,   387,     0,   146,    90,   602,   387,     0,    65,    90,
     602,   153,   568,   601,   389,   387,     0,   247,   540,    83,
     682,     0,     0,   446,    24,   273,   610,   274,   447,     0,
     390,     0,   391,     0,   391,   390,     0,   390,   391,     0,
       0,   129,   188,   392,     0,   129,    47,   392,     0,   211,
       0,   164,    46,     0,   164,    98,     0,   123,   209,     0,
     684,   397,   398,    10,   444,   401,   407,   447,     0,   684,
     397,   398,    10,   444,   401,   407,   447,     0,   684,   397,
     398,    10,   444,   401,   407,   447,     0,   684,   397,   398,
      10,   444,   401,   407,   447,     0,   273,   399,   274,     0,
       0,   157,   397,     0,     0,   400,     0,   399,   272,   400,
       0,   372,   482,     0,   402,     0,     0,   403,     0,   402,
     403,     0,    45,   404,   371,   482,   405,   271,     0,   192,
       0,     0,   264,   376,     0,   375,     0,   410,     0,   407,
       0,    17,   408,    54,     0,   409,     0,   409,   430,     0,
       0,   406,     0,   409,   406,     0,   594,   271,     0,   586,
     271,     0,   411,     0,   412,     0,   413,     0,   414,     0,
     416,     0,   419,     0,   584,   271,     0,   420,     0,   422,
       0,   590,   271,     0,   427,     0,   417,     0,   418,     0,
     178,   271,     0,    61,   271,     0,   428,     0,    58,   678,
     271,     0,    58,   678,   631,   271,     0,    58,   271,     0,
      59,   148,   684,   424,   425,   271,     0,    59,   415,   631,
     271,     0,   191,     0,   254,     0,    64,   535,    86,   426,
     429,    50,   406,     0,    64,    59,   415,   631,    86,   426,
      50,   406,     0,    59,   415,   631,    86,   426,   271,     0,
      78,   273,   610,   274,   181,   406,    53,   406,     0,    78,
     273,   610,   274,   181,   406,     0,   144,   631,   421,   271,
       0,     0,   535,    86,   426,   271,     0,   275,   690,     0,
     565,     0,   273,   565,   274,     0,     0,   156,   426,     0,
     156,   273,   426,   274,     0,     0,   423,     0,   607,     0,
     426,   272,   607,     0,   426,   272,   423,     0,   199,   273,
     610,   274,    50,   406,     0,   235,   271,     0,   255,   271,
       0,    10,    39,   676,     0,     0,   431,     0,   430,   431,
       0,   197,   432,    50,   406,     0,   433,     0,   432,   272,
     433,     0,   173,   643,     0,    69,   680,     0,    58,   678,
       0,     9,     0,    59,   148,   684,   424,     0,   691,   601,
      10,   444,   437,   448,   445,     0,   691,   601,    10,   444,
     437,   448,   445,     0,   438,     0,   439,     0,   438,   186,
     439,     0,   438,   186,     6,   439,     0,   163,   555,   556,
     440,   575,   570,   574,     0,    66,   441,     0,   442,     0,
     441,   272,   442,     0,   443,     0,   567,     0,   442,   569,
      89,   442,   129,   610,     0,   273,   443,   274,     0,     0,
       0,     0,     0,   200,    24,   131,     0,     0,   688,    64,
     568,   451,   452,   455,   446,   456,   447,     0,   688,    64,
     568,   451,   452,   455,   446,   456,   447,     0,     3,     0,
      80,     0,     0,   453,   454,     0,    16,     0,     5,     0,
      84,     0,   188,     0,    47,     0,    84,   132,   188,     0,
      84,   132,    47,     0,   188,   132,    84,     0,   188,   132,
      47,     0,    47,   132,    84,     0,    47,   132,   188,     0,
      84,   132,   188,   132,    47,     0,    84,   132,    47,   132,
     188,     0,   188,   132,    84,   132,    47,     0,   188,   132,
      47,   132,    84,     0,    47,   132,    84,   132,   188,     0,
      47,   132,   188,   132,    84,     0,   143,   644,     0,     0,
      10,   446,   401,   407,     0,     7,   458,     0,    58,   678,
     642,     0,   180,   568,   463,     0,   184,   476,     0,   148,
     396,     0,    40,   472,   473,     0,    51,   465,   461,     0,
      83,   471,     0,    46,   446,   376,     0,    24,   446,   273,
     610,   274,   447,     0,   462,     0,   461,   462,     0,   164,
     446,   459,   447,     0,     4,    33,   460,     0,     4,   333,
       0,    52,    46,     0,    52,    33,     0,   182,   608,     0,
     216,   374,   482,     0,   464,     0,   463,   272,   464,     0,
      52,   608,   470,     0,    52,    33,   675,     0,     4,   366,
       0,     4,   381,     0,   467,   608,   143,   646,     0,   467,
     465,   182,   608,     0,   467,   469,   216,   468,   447,     0,
     466,     0,   693,     0,   215,     0,   216,     0,   217,     0,
     218,     0,   219,     0,   220,     0,   221,     0,   222,     0,
     223,     0,   224,     0,   225,     0,   226,     0,   227,     0,
     228,     0,   229,     0,   230,     0,   233,     0,   234,     0,
     241,     0,   242,     0,   250,     0,   252,     0,     7,     0,
       7,   215,     0,   482,   446,     0,   608,   444,     0,   608,
       0,   213,     0,   211,     0,     0,   682,     3,     0,   682,
      80,     0,     0,   474,     0,   473,   474,     0,     4,   324,
       0,    52,   103,     0,   164,   475,     0,     4,   346,     0,
       4,   263,    94,   642,     0,    52,   263,    94,     0,    17,
     262,     0,    54,   262,     0,   345,     0,   475,   272,   345,
       0,   688,   451,   477,   455,   446,   478,   447,     0,   452,
       0,     0,   456,     0,     0,    52,   480,     0,    58,   678,
       0,    83,   682,     0,   148,   684,     0,   180,   687,     0,
     184,   688,     0,   195,   691,     0,    63,   679,     0,    51,
     677,     0,    62,    68,   670,     0,   165,   646,     0,   214,
     685,     0,    71,   681,     0,   482,     0,   483,     0,   486,
       0,   489,     0,   487,   276,   484,   277,     0,   494,   276,
     484,   277,   492,     0,   485,     0,   484,   272,   485,     0,
     648,     0,   648,   275,   648,     0,   487,     0,   494,   492,
       0,   493,     0,   498,     0,   502,     0,   243,     0,   488,
       0,   169,     0,    41,     0,   226,     0,   227,     0,    85,
       0,    96,     0,    19,   491,   490,   492,     0,    19,   273,
     647,   274,     0,    19,   273,   647,   272,   643,   274,     0,
      19,   273,   272,   643,   274,     0,   162,   168,   647,     0,
       0,   177,   643,     0,   177,   671,     0,     0,    23,   164,
     672,     0,     0,   497,   273,   646,   274,     0,   497,     0,
     497,   193,   273,   646,   274,     0,   496,   273,   646,   274,
       0,   496,     0,   495,   273,   646,   274,     0,   191,     0,
      23,   193,     0,    91,   193,     0,    23,     0,    91,     0,
     121,     0,   119,    23,     0,   119,    91,     0,    99,   500,
       0,   501,   500,     0,   646,     0,     0,   273,   648,   274,
       0,   273,   648,   272,   648,   274,     0,    44,     0,    92,
       0,    95,   503,     0,    97,    95,   503,     0,   152,     0,
      93,   145,     0,   273,   644,   274,     0,     0,   517,     0,
     505,     0,   534,     0,   164,    71,   681,   182,   648,     0,
     164,    71,   681,   182,   231,     0,   164,    71,   681,   182,
     268,   231,     0,   507,     0,   508,     0,   510,     0,   252,
     692,     0,   253,   252,   692,   509,     0,   130,     0,     0,
     161,   514,   182,   511,   692,     0,   252,     0,     0,    28,
     514,   515,     0,   161,   514,     0,   201,     0,     0,   155,
     516,     0,     0,   170,     0,     0,   164,   183,   518,     0,
     519,     0,     0,   520,     0,   519,   520,     0,   521,     0,
     522,     0,   523,     0,   527,     0,   151,   130,     0,   151,
     202,     0,   196,     0,   123,   196,     0,    88,   107,   524,
       0,   524,     0,   525,     0,   151,   185,   526,     0,   151,
      29,   526,     0,   170,     0,   170,   180,     0,   170,   180,
     174,     0,   194,     0,   123,   194,     0,     0,   154,   530,
       0,   166,     0,   149,     0,     0,   151,     0,   202,     0,
     531,     0,   530,   272,   531,     0,   533,   532,     0,    64,
     528,   529,     0,     0,   568,     0,   533,   272,   568,     0,
     164,   176,    83,   682,     0,   536,   537,   543,   545,     0,
     546,     0,   536,   186,   546,     0,   536,   186,     6,   546,
       0,   133,    20,   538,     0,     0,   539,     0,   538,   272,
     539,     0,   631,   540,   542,     0,    11,     0,    48,     0,
       0,   239,     0,   249,     0,   248,   550,   541,   551,     0,
       0,    64,   188,   544,     0,     0,   128,   603,     0,     0,
     200,   251,     0,     0,   163,   552,   555,   556,   559,   575,
     570,   574,   576,     0,   163,   552,   555,   556,   559,   575,
     570,   574,   576,   537,     0,     0,     0,     0,     0,   553,
     554,   549,     0,   553,   549,     0,   554,     0,     0,   239,
     649,   548,     0,   239,   273,   631,   274,   548,     0,   239,
     638,   548,     0,   240,   649,     0,   240,   273,   549,   631,
     274,     0,   240,   638,     0,    49,     0,   668,     0,   557,
       0,   269,     0,   558,     0,   557,   272,   558,     0,   595,
       0,   595,   683,     0,   595,    10,   683,     0,    66,   560,
       0,   561,     0,   560,   272,   561,     0,   562,     0,   563,
       0,   561,   569,    89,   561,   129,   610,     0,   273,   562,
     274,     0,   684,   564,   686,     0,   684,   564,     0,   273,
     565,   274,     0,     0,   566,     0,   565,   272,   566,     0,
     669,     0,   631,     0,   568,     0,   687,   686,     0,   687,
       0,    81,     0,   105,     0,   105,   134,     0,   159,     0,
     159,   134,     0,    67,     0,    67,   134,     0,     0,    74,
      20,   571,     0,     0,   572,     0,   571,   272,   572,     0,
     607,     0,   499,     0,   655,     0,   573,     0,   607,    26,
     673,     0,   653,     0,   654,     0,   657,     0,    77,   610,
       0,     0,   198,   610,     0,     0,   142,   577,     0,     0,
     578,   273,   579,   274,     0,    89,     0,   172,   114,     0,
     114,     0,   172,     0,     0,   580,     0,   580,   272,   579,
       0,   581,   582,     0,   577,     0,   687,     0,   687,   581,
       0,   120,     0,    83,   273,   583,   274,     0,   133,   682,
       0,   682,     0,   682,   272,   583,     0,    84,    86,   568,
     604,   190,   273,   585,   274,     0,    84,    86,   568,   604,
     547,     0,   595,     0,   585,   272,   595,     0,   587,     0,
     588,     0,    47,    66,   567,   575,     0,    47,    66,   567,
     589,     0,   198,    38,   128,   676,     0,   591,     0,   592,
       0,   188,   567,   164,   593,   575,     0,   188,   567,   164,
     593,   589,     0,   594,     0,   593,   272,   594,     0,   609,
     264,   595,     0,   631,     0,   669,     0,   151,    19,   608,
      66,   568,   597,   599,     0,    84,    19,   608,    86,   568,
     597,   599,     0,    63,    66,   598,   182,   598,     0,    63,
     182,   598,     0,     0,   491,     0,   638,     0,   643,     0,
     113,   600,     0,     0,   647,     0,   638,     0,   602,     0,
       0,   273,   603,   274,     0,   608,     0,   603,   272,   608,
       0,   605,     0,     0,   273,   606,   274,     0,   609,     0,
     606,   272,   609,     0,   608,     0,   686,   278,   674,     0,
     686,   278,   269,     0,   674,     0,   608,     0,   686,   278,
     674,     0,   625,     0,   124,   625,     0,   612,     0,   610,
     132,   610,     0,   610,     8,   610,     0,   612,     0,   124,
     625,     0,   611,   132,   610,     0,   611,     8,   610,     0,
     626,   132,   610,     0,   626,     8,   610,     0,   613,     0,
     273,   611,   274,     0,   124,   612,     0,   614,     0,   617,
       0,   618,     0,   619,     0,   624,     0,   615,     0,   622,
       0,   620,     0,   621,     0,   623,     0,   631,   264,   631,
       0,   631,   265,   631,     0,   631,   266,   631,     0,   631,
      70,   631,     0,   631,   106,   631,     0,   631,   125,   631,
       0,   631,   126,   631,     0,   631,   122,   631,     0,   631,
     264,     6,   273,   629,   274,     0,   631,   265,     6,   273,
     629,   274,     0,   631,   266,     6,   273,   629,   274,     0,
     631,    70,     6,   273,   629,   274,     0,   631,   106,     6,
     273,   629,   274,     0,   631,   125,     6,   273,   629,   274,
       0,   631,   126,     6,   273,   629,   274,     0,   631,   122,
       6,   273,   629,   274,     0,   631,   264,   616,   273,   629,
     274,     0,   631,   265,   616,   273,   629,   274,     0,   631,
     266,   616,   273,   629,   274,     0,   631,    70,   616,   273,
     629,   274,     0,   631,   106,   616,   273,   629,   274,     0,
     631,   125,   616,   273,   629,   274,     0,   631,   126,   616,
     273,   629,   274,     0,   631,   122,   616,   273,   629,   274,
       0,   171,     0,     9,     0,   631,    18,   631,     8,   631,
       0,   631,   124,    18,   631,     8,   631,     0,   631,   108,
     631,     0,   631,   124,   108,   631,     0,   631,   108,   631,
      57,   631,     0,   631,   124,   108,   631,    57,   631,     0,
     631,    79,   627,     0,   631,   124,    79,   627,     0,   631,
      34,   631,     0,   631,   124,    34,   631,     0,   631,   175,
     631,     0,   631,   124,   175,   631,     0,   631,   175,   200,
     631,     0,   631,   124,   175,   200,   631,     0,    60,   273,
     547,   274,     0,   167,   273,   547,   274,     0,   631,    87,
      98,     0,   631,    87,   124,    98,     0,   256,     0,   257,
       0,   258,     0,   259,     0,   260,     0,   261,     0,   628,
       0,   273,   634,   274,     0,   273,   629,   274,     0,   163,
     552,   555,   631,   559,   575,   570,   574,   576,   537,     0,
     163,   552,   555,   631,   559,   575,   570,   574,   576,   537,
       0,   607,     0,   633,     0,   650,     0,   637,     0,   638,
       0,   423,     0,   656,     0,   657,     0,   655,     0,   268,
     631,     0,   267,   631,     0,   631,   267,   631,     0,   631,
      31,   631,     0,   631,    26,   673,     0,   631,   268,   631,
       0,   631,   269,   631,     0,   631,   270,   631,     0,   273,
     631,   274,     0,   273,   630,   274,     0,   639,     0,   640,
       0,   641,     0,    42,     0,   686,   278,    42,     0,   101,
       0,   632,     0,   228,     0,   229,     0,   230,     0,   607,
     276,   634,   277,     0,   631,     0,   634,   272,   631,     0,
     637,     0,   268,   636,     0,   205,     0,   204,     0,   203,
       0,   231,     0,   232,     0,   636,     0,   642,     0,    41,
     207,     0,   226,   207,     0,   227,   207,     0,   279,     0,
     189,     0,   233,     0,   234,     0,   241,     0,   242,     0,
      69,     0,   173,     0,   250,     0,   207,     0,   208,   207,
       0,   644,     0,   268,   645,     0,   204,     0,   204,     0,
     644,     0,   204,     0,   649,     0,   268,   649,     0,   204,
       0,   651,     0,   652,     0,   653,     0,   654,     0,    35,
     273,   269,   274,     0,    35,   273,   668,   631,   274,     0,
      35,   273,    49,   631,   274,     0,   179,   273,   668,   631,
     274,     0,   179,   273,    49,   631,   274,     0,    13,   273,
     668,   631,   274,     0,    13,   273,    49,   631,   274,     0,
     116,   273,   668,   631,   274,     0,   116,   273,    49,   631,
     274,     0,   112,   273,   668,   631,   274,     0,   112,   273,
      49,   631,   274,     0,    72,   273,   681,   272,   631,   274,
       0,   217,   273,   667,    66,   631,   274,     0,   236,   273,
     631,    66,   646,   274,     0,   236,   273,   631,    66,   646,
      64,   644,   274,     0,   100,   273,   631,   274,     0,   670,
     273,   634,   274,     0,   670,   273,   274,     0,    22,   273,
     595,    10,   373,   274,     0,   658,     0,   659,     0,   245,
     273,   631,   272,   631,   274,     0,   246,   273,   566,   272,
     565,   274,     0,   660,     0,   662,     0,   244,   665,   661,
      54,     0,   244,   665,   661,    53,   666,    54,     0,   197,
     664,   181,   666,     0,   661,   197,   664,   181,   666,     0,
     244,   663,    54,     0,   244,   663,    53,   666,    54,     0,
     197,   610,   181,   666,     0,   663,   197,   610,   181,   666,
       0,   631,     0,   631,     0,   566,     0,   218,     0,   219,
       0,   220,     0,   221,     0,   222,     0,   223,     0,   224,
       0,   225,     0,     6,     0,     0,    98,     0,   206,     0,
     693,     0,   693,     0,   693,     0,   693,     0,   693,     0,
     693,     0,   693,     0,   693,     0,   693,     0,   693,     0,
     693,     0,   693,     0,   693,     0,   693,     0,   693,     0,
     693,     0,   693,     0,   693,     0,   693,     0,   693,     0,
     693,     0,   693,     0,   206,     0,   694,     0,   235,     0,
     238,     0,   236,     0,   246,     0,   249,     0,   255,     0,
     251,     0,   245,     0,   248,     0,   254,     0,   247,     0,
     256,     0,   257,     0,   258,     0,   262,     0,   263,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   529,   531,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   561,   565,   569,   573,   577,   582,   583,   587,
     589,   591,   595,   596,   600,   604,   606,   608,   610,   612,
     616,   618,   622,   624,   628,   635,   640,   645,   650,   655,
     660,   665,   671,   675,   676,   678,   680,   684,   686,   688,
     690,   694,   695,   702,   704,   706,   710,   711,   715,   719,
     720,   724,   726,   733,   737,   739,   744,   750,   751,   753,
     759,   761,   762,   766,   767,   773,   776,   782,   783,   786,
     789,   793,   797,   800,   805,   814,   818,   821,   824,   826,
     828,   830,   832,   834,   836,   838,   840,   845,   849,   851,
     853,   863,   867,   869,   880,   882,   886,   888,   889,   895,
     901,   903,   905,   909,   911,   915,   917,   921,   923,   926,
     927,   934,   952,   954,   958,   960,   964,   966,   969,   970,
     973,   978,   979,   982,   986,   995,  1003,  1012,  1017,  1018,
    1021,  1027,  1029,  1032,  1033,  1037,  1039,  1041,  1043,  1045,
    1049,  1051,  1054,  1055,  1059,  1061,  1062,  1063,  1065,  1069,
    1071,  1073,  1075,  1079,  1084,  1091,  1097,  1104,  1111,  1144,
    1145,  1148,  1154,  1158,  1159,  1162,  1170,  1174,  1175,  1178,
    1179,  1182,  1184,  1188,  1189,  1190,  1193,  1194,  1195,  1201,
    1206,  1211,  1213,  1215,  1219,  1220,  1224,  1225,  1232,  1236,
    1240,  1249,  1255,  1256,  1259,  1261,  1266,  1268,  1273,  1279,
    1285,  1289,  1295,  1297,  1301,  1306,  1307,  1308,  1310,  1312,
    1316,  1318,  1321,  1322,  1326,  1331,  1333,  1338,  1339,  1341,
    1349,  1353,  1355,  1358,  1359,  1360,  1361,  1364,  1368,  1372,
    1379,  1386,  1391,  1397,  1400,  1402,  1404,  1406,  1410,  1413,
    1417,  1420,  1423,  1426,  1435,  1447,  1458,  1469,  1480,  1482,
    1486,  1488,  1492,  1493,  1497,  1503,  1505,  1509,  1510,  1514,
    1519,  1521,  1525,  1527,  1531,  1532,  1535,  1539,  1541,  1543,
    1547,  1548,  1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,
    1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,  1569,  1574,
    1577,  1579,  1583,  1587,  1592,  1596,  1596,  1598,  1603,  1608,
    1613,  1615,  1619,  1623,  1629,  1634,  1639,  1641,  1643,  1647,
    1649,  1651,  1655,  1656,  1657,  1659,  1663,  1674,  1676,  1684,
    1686,  1689,  1690,  1694,  1699,  1700,  1704,  1706,  1708,  1710,
    1717,  1725,  1732,  1752,  1756,  1758,  1760,  1764,  1775,  1779,
    1780,  1784,  1785,  1788,  1791,  1797,  1801,  1807,  1811,  1817,
    1821,  1829,  1840,  1851,  1853,  1855,  1859,  1863,  1865,  1869,
    1871,  1873,  1875,  1877,  1879,  1881,  1883,  1885,  1887,  1889,
    1891,  1893,  1895,  1897,  1901,  1903,  1907,  1913,  1917,  1920,
    1927,  1929,  1931,  1934,  1937,  1942,  1946,  1952,  1953,  1957,
    1966,  1970,  1972,  1974,  1976,  1978,  1982,  1983,  1987,  1989,
    1991,  1993,  1999,  2002,  2004,  2008,  2016,  2017,  2018,  2019,
    2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,
    2030,  2031,  2032,  2033,  2034,  2035,  2036,  2037,  2038,  2041,
    2043,  2047,  2049,  2054,  2060,  2062,  2064,  2068,  2070,  2077,
    2083,  2084,  2088,  2096,  2098,  2100,  2102,  2104,  2106,  2108,
    2112,  2113,  2120,  2130,  2131,  2135,  2136,  2142,  2146,  2148,
    2150,  2152,  2154,  2156,  2158,  2160,  2162,  2164,  2166,  2168,
    2175,  2176,  2179,  2180,  2183,  2188,  2195,  2196,  2200,  2209,
    2215,  2216,  2219,  2220,  2221,  2222,  2239,  2244,  2249,  2268,
    2285,  2292,  2293,  2300,  2304,  2310,  2316,  2324,  2328,  2334,
    2338,  2342,  2348,  2352,  2359,  2365,  2371,  2379,  2384,  2389,
    2396,  2397,  2398,  2401,  2402,  2405,  2406,  2407,  2414,  2418,
    2429,  2436,  2442,  2496,  2558,  2559,  2566,  2579,  2584,  2589,
    2596,  2598,  2605,  2606,  2607,  2611,  2616,  2621,  2632,  2633,
    2634,  2637,  2641,  2645,  2647,  2651,  2655,  2656,  2659,  2663,
    2667,  2668,  2671,  2673,  2677,  2678,  2682,  2686,  2687,  2691,
    2692,  2696,  2697,  2698,  2699,  2702,  2704,  2708,  2710,  2714,
    2716,  2719,  2721,  2723,  2727,  2729,  2731,  2735,  2737,  2739,
    2743,  2747,  2749,  2751,  2755,  2757,  2761,  2762,  2766,  2770,
    2772,  2776,  2777,  2782,  2790,  2794,  2796,  2798,  2802,  2804,
    2808,  2809,  2813,  2817,  2819,  2821,  2825,  2827,  2831,  2833,
    2837,  2839,  2843,  2845,  2849,  2851,  2858,  2870,  2883,  2887,
    2891,  2895,  2899,  2901,  2903,  2905,  2909,  2911,  2913,  2917,
    2919,  2921,  2925,  2927,  2931,  2933,  2937,  2938,  2942,  2943,
    2945,  2952,  2958,  2959,  2963,  2964,  2967,  2969,  2973,  2976,
    2981,  2983,  2987,  2988,  2992,  2993,  2996,  2997,  3002,  3007,
    3009,  3011,  3013,  3015,  3017,  3019,  3021,  3028,  3030,  3034,
    3035,  3039,  3040,  3041,  3042,  3043,  3047,  3048,  3049,  3052,
    3054,  3058,  3060,  3067,  3069,  3073,  3077,  3079,  3081,  3087,
    3089,  3093,  3094,  3098,  3100,  3103,  3104,  3108,  3110,  3112,
    3116,  3117,  3125,  3128,  3132,  3133,  3140,  3141,  3144,  3148,
    3152,  3159,  3160,  3163,  3168,  3173,  3174,  3178,  3182,  3183,
    3189,  3191,  3195,  3197,  3199,  3202,  3203,  3206,  3210,  3212,
    3215,  3217,  3223,  3224,  3228,  3232,  3233,  3238,  3239,  3243,
    3247,  3248,  3253,  3254,  3257,  3262,  3267,  3270,  3277,  3278,
    3280,  3281,  3283,  3287,  3288,  3290,  3292,  3296,  3298,  3302,
    3303,  3305,  3309,  3310,  3311,  3312,  3313,  3314,  3315,  3316,
    3317,  3318,  3323,  3325,  3327,  3329,  3331,  3333,  3335,  3337,
    3344,  3346,  3348,  3350,  3352,  3354,  3356,  3358,  3360,  3362,
    3364,  3366,  3368,  3370,  3372,  3374,  3378,  3379,  3385,  3387,
    3392,  3394,  3396,  3398,  3403,  3405,  3409,  3411,  3415,  3417,
    3419,  3421,  3425,  3429,  3433,  3435,  3439,  3444,  3449,  3456,
    3461,  3466,  3475,  3476,  3480,  3484,  3497,  3514,  3515,  3516,
    3517,  3518,  3519,  3520,  3521,  3522,  3523,  3525,  3527,  3534,
    3536,  3538,  3545,  3552,  3559,  3561,  3563,  3564,  3565,  3566,
    3568,  3570,  3574,  3578,  3594,  3610,  3614,  3618,  3619,  3623,
    3624,  3628,  3630,  3632,  3634,  3636,  3640,  3641,  3643,  3659,
    3675,  3679,  3683,  3685,  3689,  3693,  3696,  3699,  3702,  3705,
    3710,  3712,  3717,  3718,  3722,  3729,  3736,  3743,  3750,  3751,
    3755,  3761,  3762,  3763,  3764,  3767,  3769,  3771,  3774,  3781,
    3790,  3797,  3806,  3808,  3810,  3812,  3818,  3827,  3831,  3838,
    3842,  3846,  3848,  3852,  3858,  3859,  3862,  3866,  3870,  3871,
    3874,  3876,  3880,  3882,  3886,  3888,  3892,  3894,  3898,  3901,
    3904,  3907,  3909,  3911,  3913,  3915,  3917,  3919,  3921,  3925,
    3926,  3929,  3937,  3940,  3943,  3946,  3949,  3952,  3955,  3958,
    3961,  3964,  3967,  3970,  3973,  3976,  3984,  3987,  3990,  3993,
    3996,  3999,  4002,  4005,  4008,  4013,  4014,  4019,  4021,  4022,
    4023,  4024,  4025,  4026,  4027,  4028,  4029,  4030,  4031,  4032,
    4033,  4035,  4036
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
     499,   500,   500,   500,   501,   501,   502,   502,   502,   502,
     503,   503,   504,   504,   504,   505,   505,   505,   506,   506,
     506,   507,   508,   509,   509,   510,   511,   511,   512,   513,
     514,   514,   515,   515,   516,   516,   517,   518,   518,   519,
     519,   520,   520,   520,   520,   521,   521,   522,   522,   523,
     523,   524,   524,   524,   525,   525,   525,   526,   526,   526,
     527,   528,   528,   528,   529,   529,   530,   530,   531,   532,
     532,   533,   533,   534,   535,   536,   536,   536,   537,   537,
     538,   538,   539,   540,   540,   540,   541,   541,   542,   542,
     543,   543,   544,   544,   545,   545,   546,   547,   548,   549,
     550,   551,   552,   552,   552,   552,   553,   553,   553,   554,
     554,   554,   555,   555,   556,   556,   557,   557,   558,   558,
     558,   559,   560,   560,   561,   561,   562,   562,   563,   563,
     564,   564,   565,   565,   566,   566,   567,   567,   568,   569,
     569,   569,   569,   569,   569,   569,   569,   570,   570,   571,
     571,   572,   572,   572,   572,   572,   573,   573,   573,   574,
     574,   575,   575,   576,   576,   577,   578,   578,   578,   578,
     578,   579,   579,   580,   580,   581,   581,   582,   582,   582,
     583,   583,   584,   584,   585,   585,   586,   586,   587,   588,
     589,   590,   590,   591,   592,   593,   593,   594,   595,   595,
     596,   596,   597,   597,   597,   598,   598,   491,   599,   599,
     600,   600,   601,   601,   602,   603,   603,   604,   604,   605,
     606,   606,   607,   607,   607,   608,   609,   609,   610,   610,
     610,   610,   610,   611,   611,   611,   611,   611,   611,   612,
     612,   612,   613,   613,   613,   613,   613,   613,   613,   613,
     613,   613,   614,   614,   614,   614,   614,   614,   614,   614,
     615,   615,   615,   615,   615,   615,   615,   615,   615,   615,
     615,   615,   615,   615,   615,   615,   616,   616,   617,   617,
     618,   618,   618,   618,   619,   619,   620,   620,   621,   621,
     621,   621,   622,   623,   624,   624,   625,   625,   625,   626,
     626,   626,   627,   627,   628,   629,   630,   631,   631,   631,
     631,   631,   631,   631,   631,   631,   631,   631,   631,   631,
     631,   631,   631,   631,   631,   631,   631,   631,   631,   631,
     631,   631,   631,   632,   632,   632,   633,   634,   634,   635,
     635,   636,   636,   636,   636,   636,   637,   637,   637,   637,
     637,   638,   639,   639,   640,   641,   641,   641,   641,   641,
     642,   642,   643,   643,   644,   645,   646,   647,   648,   648,
     649,   650,   650,   650,   650,   651,   651,   651,   651,   651,
     651,   651,   651,   651,   651,   651,   652,   653,   654,   654,
     654,   655,   655,   656,   657,   657,   658,   658,   659,   659,
     660,   660,   661,   661,   662,   662,   663,   663,   664,   665,
     666,   667,   667,   667,   667,   667,   667,   667,   667,   668,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   692,   693,   693,   694,   694,   694,
     694,   694,   694,   694,   694,   694,   694,   694,   694,   694,
     694,   694,   694
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
       1,     0,     3,     5,     1,     1,     2,     3,     1,     2,
       3,     0,     1,     1,     1,     5,     5,     6,     1,     1,
       1,     2,     4,     1,     0,     5,     1,     0,     3,     2,
       1,     0,     2,     0,     1,     0,     3,     1,     0,     1,
       2,     1,     1,     1,     1,     2,     2,     1,     2,     3,
       1,     1,     3,     3,     1,     2,     3,     1,     2,     0,
       2,     1,     1,     0,     1,     1,     1,     3,     2,     3,
       0,     1,     3,     4,     4,     1,     3,     4,     3,     0,
       1,     3,     3,     1,     1,     0,     1,     1,     4,     0,
       3,     0,     2,     0,     2,     0,     9,    10,     0,     0,
       0,     0,     3,     2,     1,     0,     3,     5,     3,     2,
       5,     2,     1,     1,     1,     1,     1,     3,     1,     2,
       3,     2,     1,     3,     1,     1,     6,     3,     3,     2,
       3,     0,     1,     3,     1,     1,     1,     2,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     3,     0,     1,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     2,
       0,     2,     0,     2,     0,     4,     1,     2,     1,     1,
       0,     1,     3,     2,     1,     1,     2,     1,     4,     2,
       1,     3,     8,     5,     1,     3,     1,     1,     4,     4,
       4,     1,     1,     5,     5,     1,     3,     3,     1,     1,
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
       1,     1,     1,     3,     3,    10,    10,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     4,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     6,     6,     6,     8,
       4,     4,     3,     6,     1,     1,     6,     6,     1,     1,
       4,     6,     4,     5,     3,     5,     4,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   571,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   571,   645,     0,     0,     0,     0,     0,
       1,    10,    15,     7,     6,    13,    14,    12,     3,     9,
      19,   553,    17,   558,   559,   560,     5,    16,   552,   554,
      18,   619,   615,    11,     8,   726,   727,    20,   731,   732,
       4,   459,     0,     0,     0,     0,     0,     0,   397,   570,
     573,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,    95,   625,   904,     0,    21,   902,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   477,     0,    29,    37,    34,
      36,   753,    35,   753,   985,   987,   989,   988,   994,   990,
     997,   995,   991,   993,   996,   992,   998,   999,  1000,  1001,
    1002,     0,    31,     0,    32,     0,    66,    68,   977,   986,
       0,     0,     0,     0,     0,     0,     0,     0,   569,     0,
       0,   960,   639,   644,     0,     0,   578,     0,   676,   678,
     979,     0,     0,     0,   107,   561,   984,     0,     2,     0,
       0,   631,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,     0,   425,   426,     0,   970,
     404,     0,   974,   401,   269,   976,     0,   678,   400,   375,
     980,   575,   568,   900,     0,   103,   151,   150,   104,   133,
     218,   765,   966,     0,   102,   145,   973,     0,    98,   269,
     105,   906,   122,    99,   203,   100,     0,   101,   753,   983,
     106,   146,   623,   624,     0,   905,   903,     0,    74,     0,
     971,   702,   485,   969,   478,     0,   484,   489,   479,   480,
     487,   481,   482,   483,   488,   328,    30,     0,    39,   752,
      38,     0,     0,     0,     0,     0,     0,   758,     0,     0,
       0,     0,     0,     0,     0,   567,   910,     0,   891,   638,
     638,   639,   651,   649,   959,   652,     0,   653,   643,   639,
       0,     0,     0,     0,     0,     0,   594,   587,   576,   577,
     579,   581,   582,   583,   590,   591,   584,     0,   677,   978,
     108,   269,   109,   203,   110,   753,   564,     0,     0,   616,
       0,   635,     0,     0,     0,     0,     0,   402,   460,   367,
       0,   367,     0,   221,   403,   407,   398,   457,   458,     0,
     271,   242,   449,     0,   399,   416,     0,   373,   374,   474,
     574,   572,   901,   148,   148,     0,     0,     0,   160,   152,
     153,   132,     0,    96,     0,     0,   111,   271,   121,   120,
     123,     0,     0,     0,     0,     0,   962,    75,   221,   521,
       0,   728,   729,   486,     0,     0,     0,     0,   869,   897,
       0,   961,     0,   871,     0,     0,   898,     0,   892,   883,
     882,   881,   985,     0,     0,     0,   873,   874,   875,   884,
     885,   893,   894,   989,   895,   896,     0,   994,   990,   899,
       0,     0,     0,     0,   852,   350,   326,   672,   847,   762,
     675,   872,   848,   886,   850,   851,   866,   867,   868,   887,
     849,   911,   912,   913,   914,   855,   853,   854,   934,   935,
     938,   939,   674,     0,     0,   966,     0,   755,     0,     0,
      27,    33,     0,     0,    43,    69,    71,   981,    67,     0,
       0,     0,   757,     0,    52,     0,     0,     0,     0,    51,
     566,     0,     0,     0,   648,   646,     0,   655,     0,   654,
     656,   658,   738,   739,   642,     0,   613,     0,   588,   599,
     585,   599,   586,   600,   606,   610,   611,   595,   580,   702,
     735,   766,     0,     0,   271,     0,     0,   563,   562,   618,
     620,   625,   617,   633,     0,   614,   186,   177,     0,   462,
     465,   174,   173,   129,     0,   468,   463,     0,   469,   148,
     148,   148,   148,   470,   464,   461,     0,   411,     0,   413,
     412,     0,   414,     0,   408,     0,     0,   272,   219,   269,
       0,     0,   420,     0,   421,   367,   450,     0,   456,     0,
       0,     0,   453,   966,   378,   377,   473,     0,   395,   149,
       0,     0,   158,     0,   157,     0,     0,   147,   161,   162,
     166,   165,   164,   154,   521,   533,   508,   544,   511,   534,
     545,     0,   551,   512,     0,   541,     0,   535,   548,   507,
     530,   509,   510,   505,   367,   490,   491,   492,   500,   506,
     493,   502,   523,     0,   528,   525,   503,   541,   504,   112,
     269,   113,     0,     0,   124,     0,     0,   202,   242,   375,
     365,     0,   221,     0,    81,    83,     0,     0,     0,   747,
       0,     0,     0,     0,   998,   999,  1000,     0,   701,   770,
     779,   782,   787,   783,   784,   785,   789,   790,   788,   791,
     786,   768,     0,   960,     0,   960,   888,     0,     0,   960,
     960,   960,     0,   889,   890,     0,     0,   949,     0,     0,
       0,     0,   857,   856,   645,     0,     0,   675,   325,   982,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   754,    28,     0,     0,    44,    72,     0,     0,    26,
     744,     0,   760,   645,     0,   723,   744,     0,     0,     0,
       0,   565,     0,   638,     0,     0,   702,     0,     0,   659,
     975,   556,     0,   555,   908,     0,   589,     0,   597,   593,
     592,     0,   603,     0,   608,   596,     0,   733,   734,     0,
       0,     0,   242,   365,     0,   629,     0,   630,   634,     0,
       0,     0,   130,   187,   467,     0,     0,     0,     0,     0,
     367,   410,     0,   367,   368,   415,   500,   523,   274,     0,
     268,   270,   365,   241,   967,   213,   210,     0,   223,   367,
     490,   365,     0,     0,     0,   240,   243,   244,   245,   246,
       0,   419,   455,   454,   418,   417,     0,     0,     0,   381,
     379,   380,   376,     0,   367,   196,   155,   159,     0,     0,
     163,     0,   518,   531,   532,   549,     0,   546,   551,     0,
     538,   536,   537,   135,     0,     0,     0,   501,     0,     0,
       0,     0,   539,   271,     0,   365,   125,   201,     0,   204,
     206,   207,     0,     0,     0,     0,   221,   221,    78,     0,
      85,    77,   519,   520,   963,   521,     0,     0,     0,   781,
     769,     0,     0,   839,   840,   841,     0,   773,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   951,   952,   953,   954,   955,   956,   957,   958,     0,
       0,     0,     0,   944,     0,     0,     0,     0,     0,   960,
     327,   865,   864,   673,   877,     0,   860,   965,   859,   858,
     861,   862,   863,   932,     0,   870,   764,   763,   756,     0,
       0,     0,     0,     0,     0,    24,    53,    41,    61,    63,
       0,     0,    70,     0,   749,     0,   759,   960,     0,   749,
      49,    47,     0,     0,     0,   647,   650,     0,   661,   662,
     664,   665,   671,     0,   688,   657,   660,   557,   909,   598,
     607,   602,   601,     0,   612,   736,   737,   767,   365,     0,
       0,   621,   640,   622,   632,   176,   183,   182,     0,   179,
     466,   148,   193,   178,   188,   189,   170,   169,   907,   172,
     171,   471,     0,     0,     0,   409,   273,   276,   212,   367,
       0,   368,   214,   209,   215,     0,     0,   251,     0,   423,
     368,   367,   365,   422,     0,     0,     0,   394,   476,   197,
     198,   156,     0,   168,     0,     0,     0,   523,     0,   547,
       0,     0,   367,   368,     0,   496,   498,     0,     0,     0,
       0,     0,     0,     0,   375,   276,   148,   127,   242,   199,
     395,   960,   370,   353,   354,     0,    82,     0,   221,     0,
      87,     0,    84,     0,     0,     0,   730,   968,     0,     0,
     774,     0,     0,   780,     0,     0,   772,   771,     0,   826,
       0,   817,   816,     0,   795,     0,   824,   842,   834,     0,
       0,     0,   796,   820,     0,     0,   799,     0,     0,     0,
       0,     0,     0,     0,   797,     0,     0,   798,     0,   828,
       0,     0,   792,     0,     0,   793,     0,     0,   794,     0,
       0,   221,     0,   915,     0,     0,   930,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   950,     0,     0,   948,
       0,     0,   940,     0,     0,     0,     0,     0,   876,   931,
      65,    57,    58,    64,    59,    60,     0,     0,     0,    22,
      25,    41,    42,   521,   521,     0,   741,   761,     0,     0,
     724,   740,    50,    48,    45,     0,   686,   664,     0,   684,
     679,   680,   682,     0,     0,   669,     0,   700,   604,   605,
     609,   276,   200,   370,     0,   181,     0,     0,     0,   194,
       0,   190,     0,     0,     0,   405,   229,   224,   879,   225,
     226,   227,   228,   281,     0,   275,   277,     0,   222,   230,
       0,   251,   625,   247,     0,   424,   451,   452,   386,   387,
     383,   382,   385,   384,   367,   475,   368,   167,   964,     0,
       0,   514,     0,   513,   550,     0,   542,     0,   136,     0,
     494,     0,   522,   523,   529,   527,     0,   524,   365,     0,
       0,     0,   119,   128,   205,   367,     0,     0,   366,     0,
      97,     0,   117,   116,    93,     0,     0,    89,     0,    86,
       0,   832,   833,   776,   775,   778,   777,     0,     0,     0,
     645,     0,     0,   835,     0,     0,     0,     0,     0,     0,
     827,   825,   821,     0,   829,     0,     0,     0,     0,   830,
       0,     0,     0,     0,     0,     0,   921,   920,     0,     0,
     917,   916,     0,   925,   924,   923,   922,   919,   918,     0,
       0,   946,   945,     0,     0,     0,     0,     0,     0,     0,
     878,    54,    55,     0,    56,    62,    23,   745,     0,   746,
     743,   748,   751,   750,     0,     0,   722,     0,    46,   667,
     663,   685,   681,   683,     0,     0,   668,     0,     0,   704,
       0,   366,   626,   627,   641,   148,   184,   180,     0,   196,
     195,   191,     0,   368,   880,   280,     0,   289,   368,   278,
     368,   217,   231,   232,   367,     0,   248,     0,     0,     0,
       0,     0,     0,     0,     0,   276,   472,   516,     0,   517,
       0,   134,     0,   217,   137,   138,   140,   141,   142,   497,
     499,   495,   526,   276,   395,   368,   196,     0,     0,     0,
     351,     0,   355,   367,    88,     0,     0,    90,   523,     0,
     818,     0,     0,   960,   844,   843,     0,     0,   822,     0,
       0,     0,     0,   831,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   933,   220,   926,   927,     0,   928,
     947,   942,   941,     0,   936,   937,   702,    40,   521,   702,
     725,   686,   670,   692,   687,   689,   694,   691,   540,   696,
     697,   693,   698,     0,   699,   710,   636,   368,   352,   628,
       0,   175,   192,   368,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   987,   992,   290,   285,     0,
     287,   284,   294,   295,   296,   297,   298,   305,   306,   299,
     301,   302,   304,   309,     0,     0,     0,     0,     0,   267,
       0,     0,   208,   233,     0,     0,     0,   251,   234,   237,
     753,     0,   368,   392,   393,   389,   388,   391,   390,     0,
     515,   543,   143,   131,   139,     0,   367,   264,   126,   368,
       0,   702,   369,   356,     0,     0,    91,    92,    79,     0,
     803,   811,     0,   804,   812,   807,   815,   819,   823,   805,
     813,   806,   814,   800,   808,   801,   809,   802,   810,     0,
     943,   688,   742,   688,     0,     0,     0,     0,   706,   708,
     709,   703,     0,   265,   185,   406,   223,   312,     0,     0,
     315,   316,     0,   308,     0,     0,     0,   323,   307,     0,
     337,   338,   286,     0,   291,   288,   341,     0,   300,   293,
     303,   292,   211,   216,   235,   251,   753,   238,   257,   250,
     252,   396,   368,     0,   371,     0,   358,   359,   361,   362,
     688,   368,    76,    94,     0,   929,   700,   700,   666,   690,
     695,   707,   710,     0,   283,     0,   310,     0,   328,     0,
       0,     0,     0,     0,     0,   349,     0,     0,     0,     0,
     344,   342,   332,     0,   333,   239,   257,     0,   251,   253,
     254,   266,   368,   686,   361,     0,     0,   700,     0,   702,
     704,   704,   714,     0,   711,     0,   715,   282,   279,   311,
     331,     0,   314,     0,   340,     0,   322,     0,   348,   347,
     972,   346,     0,     0,   324,     0,   251,     0,     0,   249,
       0,   256,     0,   255,   372,   364,   360,     0,   357,   118,
     688,   619,   619,   705,   710,     0,   717,     0,   713,   716,
       0,     0,     0,     0,     0,     0,     0,     0,   343,   345,
     335,   334,   236,     0,     0,   260,   259,   258,   686,   700,
     846,   637,   712,     0,   719,     0,   329,   313,   319,     0,
       0,     0,   321,   336,   263,   261,   262,     0,   704,     0,
     720,     0,     0,   339,   317,     0,   363,   619,   718,     0,
     330,   318,   320,   845,   721,     0,     0,     0
};

static const short yydefgoto[] =
{
    1855,    20,    21,   459,   121,   122,   123,   124,  1199,   719,
     714,    22,   136,   965,   966,   967,   968,   125,   126,   464,
     465,    23,    81,   377,   870,   643,   644,   645,  1099,  1100,
     238,    24,    73,    25,   154,    26,   366,    74,  1310,   220,
     370,   635,  1087,  1302,   529,   208,   362,  1073,  1453,  1454,
    1455,  1456,  1457,  1458,   214,   230,   205,   580,   206,   358,
     359,   360,   587,   588,   589,   590,   591,   531,   532,   533,
    1018,  1015,  1016,  1235,  1416,   534,  1023,  1024,  1025,  1240,
    1061,   223,   312,   372,   858,   859,   860,   796,   797,   798,
    1582,   563,   555,  1358,   646,  1041,  1245,  1431,  1432,  1433,
    1588,   861,   565,   805,   806,   807,   808,  1263,   809,  1738,
    1739,  1740,  1816,   218,   310,   629,   193,   340,   560,   556,
     557,  1254,  1255,  1256,  1426,  1715,  1557,  1558,  1559,  1560,
    1561,  1562,  1563,  1564,  1565,  1662,  1566,  1567,  1568,  1569,
    1570,  1723,  1571,   424,   425,  1801,  1733,  1572,  1573,  1805,
    1675,  1676,  1729,  1730,    27,   227,   314,  1092,  1093,  1094,
    1611,  1696,  1697,  1698,   863,  1470,   548,  1035,  1308,   225,
     631,   349,   576,   577,   822,   824,  1275,    28,    58,   784,
     781,   334,   335,   344,   345,   185,   186,   346,  1050,   571,
     814,   190,   162,   327,   328,   544,   198,   578,  1276,    29,
      95,   614,   615,   616,  1074,  1075,   617,   786,   619,   620,
    1067,  1387,   847,   621,   787,   623,   624,   625,   626,  1523,
     840,   627,   628,   837,    30,    31,    32,    33,    34,   518,
      35,   481,    36,    37,    60,   202,   351,    38,   298,   299,
     300,   301,   302,   303,   304,   305,   749,   306,  1003,  1230,
     503,   504,   754,   505,    39,  1574,    41,   161,   519,   520,
     234,  1414,  1013,   321,   767,   525,    42,   725,   484,   288,
    1234,  1539,   141,   142,   143,   286,   488,   489,   490,   736,
     988,   989,   990,   991,  1225,   426,  1176,  1699,   148,  1223,
    1227,  1524,  1525,  1526,  1409,   381,  1536,  1752,  1652,  1753,
    1754,  1755,  1798,  1839,  1575,  1209,  1576,    45,    46,   382,
    1577,    48,    49,   509,  1578,   491,    50,   974,  1388,  1206,
    1391,   258,   259,   456,   471,   472,   721,   428,   429,   512,
     658,   886,   659,   660,   661,   662,  1123,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   888,  1126,  1127,  1331,
     696,   672,   431,   432,   945,  1247,   433,   434,   435,   436,
     437,   438,   439,   649,   221,   236,  1528,  1029,  1076,   744,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   936,   451,   688,  1180,   689,  1177,   929,   287,   452,
     453,   873,  1277,   946,   211,   793,  1106,   242,   188,   239,
    1769,   215,  1840,   739,   992,   127,   454,   197,   199,   969,
     698,   228,   155,   455,   129
};

static const short yypact[] =
{
    1361,   676,    38,  1493,   480,   687,   210,  2115,   112,  1703,
     193,   484,  1075,    38,   525,   130,  5515,   696,  5515,   270,
     292,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    85,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  5289,  5515,  5515,  5515,  5515,  5515,-32768,-32768,
     386,   604,  5515,  5515,  5515,   584,  5515,   399,  5515,  5515,
  -32768,  5515,  5515,-32768,   222,-32768,   417,-32768,-32768,   631,
    5515,-32768,  5515,  5515,  5515,   644,  5515,  5515,  5515,  5515,
     399,  5515,  5515,  5515,  5515,-32768,  5515,   583,-32768,-32768,
  -32768,   559,-32768,   559,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   652,   528,   736,-32768,    23,-32768,-32768,-32768,-32768,
    5515,  5515,  5515,   731,   743,   752,   580,   124,   711,   513,
     537,   789,   662,-32768,  5515,   816,   848,   756,-32768,  5515,
  -32768,  5515,  5515,  5515,-32768,-32768,-32768,  5515,-32768,   929,
     207,   905,   527,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   667,-32768,-32768,   604,-32768,
  -32768,   548,-32768,-32768,   661,-32768,   931,-32768,-32768,   607,
  -32768,   819,-32768,-32768,   788,-32768,   633,-32768,-32768,  1004,
  -32768,-32768,-32768,   604,-32768,-32768,-32768,    83,-32768,   661,
  -32768,-32768,   451,-32768,   958,-32768,   970,-32768,   559,-32768,
  -32768,-32768,-32768,-32768,   974,-32768,-32768,   860,-32768,   985,
  -32768,   886,-32768,-32768,-32768,   860,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  4757,-32768,  5515,-32768,-32768,
  -32768,  3306,   755,   922,  3674,  5515,  1001,   827,  1035,  1055,
    3306,   978,  1000,  1012,  3674,   902,-32768,  6077,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  4430,-32768,-32768,-32768,
     963,  5515,  1049,   961,   470,  5515,   984,-32768,-32768,   848,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  5515,-32768,-32768,
  -32768,   661,-32768,   958,-32768,   559,  1036,  6077,  1006,-32768,
     982,   976,   184,   916,   189,   917,   649,   527,-32768,  1153,
     201,-32768,  5515,-32768,   667,-32768,-32768,-32768,-32768,  5515,
    1033,  1973,   977,  2272,   926,-32768,  5289,-32768,-32768,   241,
  -32768,-32768,-32768,   935,   935,   604,  1082,   604,   216,   633,
  -32768,-32768,  2639,-32768,  5515,  5515,-32768,  1033,-32768,-32768,
    1171,   442,   934,  5515,  1194,  5515,-32768,-32768,   294,   338,
    3776,-32768,-32768,-32768,   938,   940,   941,  1010,-32768,-32768,
     945,-32768,   946,-32768,   948,   949,-32768,   953,-32768,-32768,
  -32768,-32768,   954,   964,  1027,  1028,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   971,-32768,-32768,  4837,   975,   980,-32768,
    6077,  6077,  4527,  5515,-32768,-32768,   936,-32768,   981,-32768,
    1234,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   988,   969,   973,   561,-32768,  5515,  1076,
  -32768,-32768,  5515,  5515,   337,-32768,-32768,-32768,-32768,  5515,
    5515,    73,-32768,  5515,-32768,  1188,  5515,  3306,  1116,   995,
  -32768,  5515,  5067,   206,-32768,-32768,  6077,-32768,  1202,   998,
  -32768,  1215,  1234,-32768,-32768,   609,-32768,    89,-32768,   508,
  -32768,   508,-32768,  1007,-32768,   122,-32768,  1099,-32768,   303,
  -32768,-32768,  1018,  1015,  1033,  1003,  1273,-32768,-32768,  1026,
  -32768,   374,-32768,  1175,  1053,-32768,-32768,   136,  1211,  1214,
  -32768,-32768,-32768,-32768,   604,-32768,-32768,  1218,-32768,   935,
     935,   935,   935,-32768,  1037,-32768,  1291,-32768,  1292,-32768,
  -32768,  1279,-32768,  2639,-32768,  2639,   612,-32768,-32768,   661,
    1318,  5515,-32768,  6332,-32768,   427,-32768,  5515,   754,   931,
    1152,  1119,  1193,  1163,-32768,-32768,-32768,   180,  1204,-32768,
    1145,   399,-32768,   604,-32768,  1327,  1257,-32768,   216,-32768,
  -32768,-32768,-32768,-32768,   590,  1159,-32768,-32768,-32768,  1160,
  -32768,  1209,  1088,-32768,  1260,  1096,   203,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1094,-32768,
  -32768,-32768,   132,  1100,  1103,   340,-32768,  1096,-32768,-32768,
     661,-32768,  1300,  1367,-32768,   604,   604,-32768,  1973,   607,
  -32768,  1250,-32768,  1224,  1110,-32768,  1660,  4949,  1249,-32768,
    1258,  1114,  3873,  1118,   923,  1091,  1185,  3524,   192,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1984,   812,  5147,   162,-32768,  5515,  6077,   873,
     933,   942,  1261,-32768,-32768,  6077,  4103,  1234,   248,  1203,
    6077,  5147,   165,   165,   525,   747,  1125,   315,-32768,-32768,
    5147,  6077,  5515,  6077,  6077,  6077,  6077,  6077,  5377,  2319,
    5515,-32768,-32768,  2966,  1219,-32768,-32768,  1192,  3674,-32768,
    1344,   753,-32768,   525,  1136,-32768,  1344,  2966,  1345,  1346,
    5515,-32768,   315,-32768,   348,  4001,  1216,  5147,  5515,-32768,
  -32768,-32768,    41,-32768,-32768,   164,-32768,  1222,-32768,-32768,
  -32768,  5515,   813,  5515,-32768,-32768,  5515,-32768,-32768,  5147,
    5515,  1409,  1973,-32768,  6077,  1174,  5515,-32768,-32768,   604,
     604,   604,-32768,   426,-32768,  1145,  1145,  1220,   399,   649,
  -32768,-32768,  1150,-32768,-32768,-32768,-32768,  1402,-32768,  5515,
  -32768,-32768,-32768,-32768,-32768,  1411,-32768,  1156,  1386,-32768,
    1403,-32768,  1347,  1348,   559,-32768,-32768,-32768,-32768,-32768,
    1412,-32768,-32768,-32768,-32768,-32768,  5515,  6422,   399,  1303,
    1307,  1308,-32768,   399,-32768,   870,-32768,-32768,  1277,   604,
  -32768,   298,  1281,-32768,-32768,-32768,   399,-32768,  1088,   589,
  -32768,-32768,-32768,  1398,   589,  1288,   589,-32768,   399,   399,
    1181,   399,-32768,  1033,  5515,-32768,  1353,-32768,   773,-32768,
  -32768,-32768,   241,  1304,  5515,   777,    48,-32768,-32768,  1195,
    1454,-32768,-32768,-32768,-32768,   338,  5515,  1313,  4200,-32768,
  -32768,  1313,  3873,-32768,-32768,-32768,   137,-32768,   194,   772,
    4103,  4103,  6077,  6077,  1895,  1217,   787,  2175,  6077,  2440,
     677,  2705,  2803,  5457,  3066,  3164,  3427,  6077,  6077,  1477,
    6077,  1221,  6077,  1225,   366,  6077,  6077,  6077,  6077,  6077,
    6077,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1422,
    1179,   186,  5147,-32768,  4103,  6077,   455,   620,  1226,   789,
  -32768,-32768,-32768,-32768,  1234,   433,-32768,-32768,  1463,   165,
     165,   840,   840,-32768,   820,-32768,-32768,-32768,-32768,  5515,
    5515,  5515,  5515,  5515,  5515,  1227,-32768,   459,-32768,-32768,
    2966,  1360,-32768,   392,  1380,  5515,-32768,   789,  5147,  1380,
    1227,  1228,  2966,  2874,  1428,-32768,-32768,  4001,  1233,   748,
  -32768,-32768,  1235,  4103,  1433,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    91,-32768,-32768,-32768,-32768,-32768,   823,
    1304,-32768,-32768,-32768,  1237,-32768,-32768,-32768,   833,-32768,
  -32768,   935,  1498,-32768,   426,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1238,  4103,  1631,-32768,-32768,  1469,-32768,-32768,
    1631,-32768,-32768,-32768,-32768,   559,   559,  1268,  1244,-32768,
  -32768,-32768,-32768,-32768,   120,   126,   630,-32768,  1508,-32768,
  -32768,-32768,  5515,-32768,   480,   834,  1352,  1402,  1247,-32768,
    1145,   842,-32768,-32768,   724,-32768,  1248,  5515,   733,  1253,
    1254,   399,  1255,  1520,   607,  1469,   935,  1214,  1973,-32768,
    1204,   789,  1331,  1350,-32768,  1736,-32768,   399,  1392,  1479,
  -32768,  1660,-32768,   399,  1297,  1484,-32768,-32768,  1266,  1269,
  -32768,  4103,  4103,-32768,  4103,  4103,-32768,  1534,   227,  1234,
    1274,-32768,-32768,  1275,  1234,  5687,-32768,-32768,-32768,  1448,
    1280,  1282,  1234,  1317,  1283,  1285,  1234,  6077,  6077,  1217,
    6077,  5767,  1287,  1293,  1234,  1294,  1295,  1234,  6077,  1234,
    1296,  1298,  1234,  1301,  1302,  1234,  1305,  1306,  1234,   412,
     636,-32768,   647,-32768,   659,  6077,-32768,   689,   843,   869,
     893,   914,   962,  6077,   399,  5147,-32768,  1496,   331,  1234,
    1371,  5147,-32768,  6077,  6077,  5147,  6077,  6077,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2966,  1481,  2966,-32768,
    1227,   459,-32768,   492,   492,    11,-32768,-32768,  4430,   868,
  -32768,-32768,  1227,  1228,  1309,  2874,   497,  1299,  4001,  1431,
  -32768,  1436,  1442,  1491,  5147,  5515,  1563,  1513,-32768,-32768,
  -32768,  1469,-32768,  1331,     5,  1424,   604,  1457,  1145,  1459,
    1145,-32768,  4103,   142,   828,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1405,  1577,  1469,-32768,  6077,-32768,   345,
    1446,  1268,   222,-32768,  4103,-32768,-32768,-32768,  1470,  1471,
    1472,  1473,  1474,  1475,-32768,-32768,-32768,-32768,-32768,  1354,
     480,-32768,  1220,-32768,-32768,   589,-32768,  1631,   463,   589,
  -32768,   589,-32768,  1402,-32768,-32768,  1356,-32768,-32768,   241,
    1577,  1145,-32768,  1214,-32768,-32768,  4430,  1591,-32768,   283,
  -32768,  1351,-32768,  1237,-32768,  1357,   604,   229,  1358,-32768,
     604,-32768,-32768,-32768,  1534,-32768,  1534,  6077,  1458,  1458,
     525,  1359,   872,-32768,  1458,  1458,  6077,  1458,  1458,   327,
    1234,-32768,  1341,  6077,  1234,  1458,  1458,  1458,  1458,  1234,
    1458,  1458,  1458,  1458,  1458,  1458,-32768,-32768,  1362,  2639,
  -32768,-32768,   972,-32768,-32768,-32768,-32768,-32768,-32768,  1032,
     119,-32768,-32768,  5147,  5147,  1566,  1441,  1072,   875,  1349,
    1234,-32768,-32768,  1495,-32768,-32768,-32768,-32768,  1452,-32768,
  -32768,-32768,-32768,-32768,  1202,  5147,-32768,  2874,  1309,-32768,
     748,-32768,-32768,-32768,  4001,   878,-32768,  3246,  4103,  1497,
    1577,-32768,-32768,-32768,-32768,   935,-32768,-32768,   604,   870,
  -32768,-32768,   145,-32768,-32768,-32768,  5515,  6299,-32768,-32768,
    1234,  1609,   345,-32768,   722,  5515,-32768,  1559,   151,  1456,
    1561,  1464,  1604,  1569,  1607,  1469,-32768,-32768,  1381,-32768,
    1382,-32768,  1562,  1609,   463,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1469,  1204,-32768,   870,  1508,  1593,  1526,
  -32768,  1304,-32768,-32768,-32768,  1544,   202,-32768,  1402,  1545,
    1234,  1389,  1390,   789,-32768,-32768,  1391,  1393,  1234,  1395,
    1396,  6077,  6077,  1234,  1404,  1407,  1408,  1410,  1413,  1415,
    1416,  1417,  1418,  1420,-32768,-32768,-32768,-32768,   399,-32768,
  -32768,-32768,-32768,  5147,-32768,-32768,  1216,-32768,   492,  1216,
  -32768,   863,-32768,-32768,  1394,-32768,-32768,  1669,-32768,-32768,
  -32768,-32768,-32768,  1427,   192,   516,-32768,-32768,-32768,-32768,
    1145,-32768,-32768,-32768,-32768,  2639,  4359,   323,  1425,   403,
    1435,  1612,  6077,  1440,  1439,  1443,  1444,-32768,-32768,  1652,
    4665,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1630,  1449,  1450,  1451,  1453,-32768,
    1445,  5515,-32768,-32768,  1625,  1635,  5515,  1268,-32768,-32768,
     559,  5515,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1577,
  -32768,-32768,-32768,-32768,-32768,  1577,-32768,-32768,-32768,-32768,
    4328,  1216,-32768,-32768,  6077,   604,-32768,-32768,-32768,   604,
  -32768,-32768,  6077,-32768,-32768,-32768,-32768,  1234,  1234,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1460,
  -32768,  1433,-32768,  1433,  4103,  3246,  5515,  5205,-32768,-32768,
    1613,-32768,  1466,-32768,-32768,-32768,   134,-32768,  5997,  5515,
  -32768,-32768,  6077,-32768,   344,  1644,  4103,  1234,-32768,  4103,
  -32768,-32768,-32768,   530,-32768,  1535,-32768,  4258,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1268,   559,-32768,  1602,-32768,
  -32768,-32768,-32768,  1508,-32768,  4328,  1461,   748,-32768,-32768,
    1433,  1234,-32768,-32768,  1349,-32768,  1513,  1513,   192,-32768,
  -32768,-32768,  3604,  1631,-32768,  1478,-32768,  1017,  4757,  1089,
    6077,  4258,   152,  1483,   153,-32768,  5515,  5515,   480,   117,
  -32768,-32768,-32768,   714,-32768,-32768,  1602,   148,  1268,  1608,
    1611,-32768,-32768,   497,  1467,  4328,  1653,  1513,  1486,  1216,
    1497,  1497,-32768,  1487,  1476,   721,  5515,-32768,-32768,-32768,
    1587,  4258,-32768,  1379,   161,  1582,-32768,  1708,-32768,-32768,
  -32768,-32768,  6299,   530,-32768,  4258,  1268,   396,   396,-32768,
    1717,-32768,  1579,-32768,-32768,-32768,   748,  4328,-32768,-32768,
    1433,  1632,  1632,-32768,  3604,  1501,-32768,  5515,-32768,-32768,
    3933,  1499,   758,  4258,  1729,  1719,  6299,  6299,-32768,-32768,
  -32768,-32768,-32768,  1567,   615,-32768,-32768,-32768,   865,  1513,
  -32768,-32768,-32768,  5515,-32768,  4258,  1503,-32768,-32768,   127,
    5515,  6299,  1718,-32768,-32768,-32768,-32768,  4103,  1497,  1506,
    1510,   879,  6299,-32768,-32768,  6299,   192,  1632,-32768,  5515,
  -32768,-32768,-32768,-32768,-32768,  1783,  1786,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,   -73,   191,-32768,   308,  1527,   591,-32768,
    -307,-32768,-32768,  -580,  -185,  -607, -1011,  1778,  1528,  1517,
    1077,-32768,-32768,-32768,   693,-32768,  1154,   932,-32768,   702,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   715,-32768,-32768,-32768,-32768,-32768,
     347,-32768,-32768,  1480,-32768,-32768,-32768,  -332,-32768,-32768,
  -32768,  1447,-32768,-32768,  1229,  -256,  1485,-32768,-32768,  -337,
  -32768,  -720,-32768,-32768,-32768,-32768,-32768,-32768,   779,-32768,
    -696,-32768,-32768,  1492,  1048,   725,  1482,  1016,   723,-32768,
     369,   -48,-32768,-32768,  -297,   168,  -988,-32768,-32768,   393,
  -32768,  1489, -1078,-32768,-32768,-32768,-32768, -1200,   394,    97,
      87,    98,    65,-32768,-32768,-32768,-32768,   -81,  -300,-32768,
    1061,  -956,-32768,   597,-32768,-32768, -1424, -1122,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   190,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -506,   135,-32768, -1064,-32768,-32768,-32768,
  -32768,   195,-32768,    82,-32768,-32768,-32768,   857,-32768, -1111,
  -32768,-32768,  -875,   173,  -635,   458,  -299,  -917,   638,-32768,
  -32768,  -539,  -756,-32768,-32768,  -971, -1305,-32768,-32768,-32768,
  -32768,-32768,  1540,-32768,  1310,  1529,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1549,-32768,-32768,-32768,-32768,-32768,
  -32768,  -536,  -460,-32768,  1031,   593,  -577,  -344,-32768,-32768,
  -32768,  -314,  -921,-32768,  -342,-32768,-32768,-32768,-32768,-32768,
    1256,-32768,-32768,  1042,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1871,-32768,-32768,-32768,-32768,-32768,
    1586,-32768,-32768,-32768,  1397,-32768,  1387,-32768,-32768,-32768,
  -32768,  1138,-32768,-32768,-32768,    10,-32768,  -965,-32768,  1126,
    -483,-32768,-32768,-32768,-32768,-32768,    24,  -258,  -186,   683,
  -32768,-32768,  -592,-32768,  1750,  -823, -1002,-32768,  1158, -1245,
  -32768,  -874,   906,-32768,-32768,  -396,  -243,   489,   -45, -1193,
   -1419,-32768,   251,-32768, -1053,  -493, -1522,   362,-32768,   104,
  -32768,   146,-32768,    57,  1907,-32768,  1910,-32768,-32768,  1406,
    1911,-32768,-32768,-32768,  -210,  -633,-32768,  1186, -1063,   937,
  -32768,   -98,  -665,  -639,-32768,-32768,-32768, -1235,   -34,  -419,
    -594,-32768,  -487,-32768,-32768,-32768,   172,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  -476,-32768,   774,-32768,  1005,
  -32768,  1141,  -976,-32768,  -591,-32768,   670,  -935,  -115,  -926,
    -925,  -922,   -24,     0,    -2,-32768,    -1,  -755,  -455,  -110,
  -32768,-32768,-32768, -1244, -1233, -1225,-32768, -1208,-32768,-32768,
  -32768,-32768,-32768,-32768,   735,-32768, -1023,-32768,   452,  -271,
     739,-32768,   844, -1009,  -670,  1355,    90,-32768,   -50,  1833,
  -32768,   -19,   -53,  1187,   -47,    16,  -132,   -10,   -36,  -163,
  -32768,   -62,   -20,    -9,-32768
};


#define	YYLAST		6685


static const short yytable[] =
{
     128,   191,    78,   128,    77,   260,   149,   150,   194,   156,
      40,   196,   427,   213,   209,   493,   757,   308,   618,   219,
     622,   592,   581,   224,   279,   282,   695,   799,   210,   280,
     283,   253,   551,   226,   244,   248,   553,   207,   765,   957,
     743,   909,   249,   187,   189,   192,   195,   150,   200,   255,
    1019,   722,  1258,   212,   189,   216,   252,   195,  1246,   150,
     200,  1436,   229,   128,  1246,   648,   222,   633,   247,   871,
     543,   240,   149,   150,   243,   189,  1065,   240,   216,   192,
     195,   251,   150,   200,   229,   128,   267,   195,   231,   250,
    1007,   315,   931,   785,   485,   788,   266,   510,   268,  1248,
     862,   466,   939,   800,   311,  1248,  1090,   313,  1249,  1250,
     254,   466,  1251,  1216,  1249,  1250,  1186,   954,  1251,  1305,
     981,   212,   150,   212,  1259,   290,  1006,  1014,  1010,  1300,
     374,   977,  1428,  1265,  1516,   216,  1674,   316,   367,  1047,
     309,  1390,   195,   150,   229,  1111,  1283,   980,   156,  1519,
     890,   769,  1371,   890,  1208,   845,  1288,  1037,  1375,   890,
     890,   890,  1609,  1529,   336,   879,  1044,  1772,   284,   728,
     887,  1804,  1527,  1270,  1530,   513,   880,  1842,  1465,   427,
    1040,  1434,  1531,  1508,   319,  1382,   752,  1385,  1097,   363,
     274,   702,   772,   499,   890,  1777,   703,   475,  1472,  1532,
     890,   144,  1114,   134,  1268,   264,  1394,   775,   776,   777,
     778,   910,   130,   318,   761,  1028,   460,   516,   159,   618,
    1085,   622,  1706,   457,  1707,   460,   841,   819,  1791,  1792,
     514,   364,   702,   232,   549,  1327,   723,   703,   496,    59,
     745,   539,  1228,   994,  1412,   276,   574,   550,   212,  1476,
     506,   592,   150,   702,  1413,   467,   128,   575,   703,   296,
      96,   150,   585,   724,   820,   467,   810,   365,  1306,  1112,
     233,   160,   997,   511,   891,  1410,    82,   891,   526,   131,
     832,  1747,   192,   891,   891,   891,   150,   527,  1537,  1471,
     278,   540,   536,  1229,   842,   265,  1116,  1117,   552,  1451,
     716,   932,   933,  1616,  1468,   558,   145,   210,  1269,   568,
     526,  1246,   572,   146,  1271,   843,  1847,   630,   891,   527,
     135,  1098,   641,   212,   891,   541,  1115,   272,   639,   632,
     212,   582,   212,   584,   212,  1491,  1778,   573,   513,   890,
    1178,   702,   522,   542,  1400,  1210,   703,   637,  1808,   501,
    1510,  1511,  1248,   702,  1434,   195,   200,  1051,   703,  1446,
    1613,  1249,  1250,  1201,   150,  1251,   192,  1175,   821,  -242,
      14,  1819,  1461,  1231,   702,  1213,  1214,    78,   561,   703,
    1260,  1261,  1832,  1833,  1071,   232,  1385,  1687,  1742,  1773,
    1200,   879,   702,  1509,   753,   879,   265,   703,  1713,  1775,
     702,  1529,  1212,   493,   729,   703,  1110,  1844,   846,   770,
    1527,  1113,  1530,   712,   699,   715,  1423,  1267,  1851,  1543,
    1531,  1852,   233,   984,   720,  1592,  1765,  1767,   726,   715,
    1312,   911,   460,  1775,   706,   707,   511,  1532,   702,  1243,
    1617,  1477,  1734,   703,   273,   934,  1091,   528,   938,   150,
    1393,   -80,   537,   195,   467,  1642,  1313,   943,  1203,  1749,
     150,   731,  1664,   891,   150,   368,   493,   195,   150,  -242,
     825,  1659,   156,   704,   705,   706,   707,  1691,   791,   586,
     733,  1032,   740,  1692,  1034,  1735,  1734,  -367,   493,  1599,
    1640,  -242,   802,  1606,   704,   705,   706,   707,  -242,   499,
    1042,   380,  1028,   132,  1746,   147,  1544,  1605,  1181,  1182,
     773,  1579,  1373,  1580,  1660,   647,  1417,  1323,  1324,  1813,
    1325,  1326,   157,  1031,   871,  1058,  1734,  1449,  1021,   801,
    1521,   322,  -242,   850,  1332,  1660,   636,   717,  1779,  1725,
    1811,   201,    75,  1464,   323,  1299,  1005,   985,  1607,   853,
    1746,   337,   794,  1083,   212,   466,  1207,  1618,   794,   827,
    1814,  1105,   369,   158,  1219,  1734,    14,   642,  1734,  1101,
    1064,   241,  1683,   803,  1204,   756,  1812,  1661,  1220,   324,
     826,   325,   704,   705,   706,   707,    97,  1452,  1726,   942,
    1734,   217,    78,  1746,   704,   705,   706,   707,  1661,  1727,
     500,  1022,  1221,    75,   210,  1648,    76,  1815,  1398,   718,
     347,   856,   857,   851,   804,   704,   705,   706,   707,  1108,
    1653,   235,   986,  1109,   513,  1746,  1655,    98,   338,   212,
    1649,   747,   998,   704,   705,   706,   707,  1710,   874,    99,
    1166,   704,   705,   706,   707,    78,   702,   872,  1422,   203,
     204,   703,  1183,  1750,  1751,   501,  1222,  1764,   913,  1197,
    1622,  1835,   702,  1463,   100,  1026,  1027,   703,   216,   647,
    1438,   329,   502,   702,   539,  1690,   958,  1272,   703,   704,
     705,   706,   707,   715,    75,   702,  1356,   348,  1650,  1238,
     703,   326,  1694,   947,  1788,  1137,    75,  1802,  1541,   237,
     212,   212,   748,  1728,   467,  1187,   506,   493,  1004,   467,
    1188,  1138,   245,  1836,  1273,   702,    51,   276,   467,   330,
     703,   195,   511,  1542,   540,  1757,   195,    52,   210,   740,
     256,  1198,   457,   101,    53,   353,  1826,  1246,  1483,  1829,
    1257,   276,   150,   102,   150,  1017,  1017,  1020,    76,    79,
      80,   212,  1266,   212,  1301,   558,  1139,   212,   541,    54,
      76,  1841,  1520,  1252,   139,   140,  1838,   647,   103,  1252,
    1608,   278,   354,  1287,   355,  1741,   542,  1030,  1248,  1437,
     212,   261,  1049,  1052,  1748,  1140,   277,  1249,  1250,  1378,
     892,  1251,   278,   276,    75,   284,  1190,   356,   702,  1193,
     262,  1101,    98,   703,  1795,  1063,   893,   212,   212,  1084,
     281,   203,   204,   276,  1534,  1219,   278,  1053,   284,  1095,
    1743,  1057,   357,  1505,    55,  1784,  1820,  1821,  1405,  1220,
    1450,   331,   257,   710,  1068,   711,  1460,  -686,   285,   100,
     741,  1796,   894,   513,   151,   150,  1584,  1079,  1080,   332,
    1082,   895,  1141,  1221,  1797,   150,    56,  1070,    76,   896,
      57,   907,   269,   831,  1359,   263,   702,  1107,  1585,   702,
    1786,   703,   270,    78,   703,  1586,   152,   742,   897,   284,
     898,   271,  1853,   333,   789,  1128,   790,   704,   705,   706,
     707,   153,  1184,   275,   899,   702,   900,   901,   902,   291,
     703,  1194,   140,   704,   705,   706,   707,  1222,   101,  1587,
    1357,  1129,  1818,  1191,   704,   705,   706,   707,   102,   702,
     307,  1360,   915,  -836,   703,  1192,   704,   705,   706,   707,
    1219,  -836,  1219,  1361,   339,   341,   292,   493,   342,   284,
     702,   511,   427,   103,  1220,   703,  1220,   903,   284,   317,
     467,   195,   200,   467,   229,   128,   704,   705,   706,   707,
     998,   467,  1001,  1363,   486,   812,   772,   813,  1221,   320,
    1221,   293,   494,   467,   467,  1445,   378,   957,   195,  1002,
    1195,   427,   917,   343,   383,  1774,  1775,  -836,   702,   350,
    -836,   919,  1644,   703,  1837,   352,  1289,  -836,   702,   294,
    -836,  1290,   295,   703,  -836,  1289,  1467,  1059,  1060,  -836,
    1293,  1381,  -836,  1384,   361,   618,  1252,   622,   296,   700,
     371,   940,  1222,  1641,  1222,   975,  1643,   976,  -836,  1828,
    1775,   399,   400,   401,   373,   493,   904,   905,   906,   704,
     705,   706,   707,   702,   297,  1088,   942,  1089,   703,   867,
    1708,  1096,  -836,  1278,   210,  -836,  -836,   375,   702,   409,
     410,   457,    78,   703,  1279,  -836,   376,   379,  1278,  1131,
     462,  1135,  1722,  1143,  1146,  1724,  1151,  1154,  1157,   212,
    1296,    97,  -836,  1540,   380,  1656,   212,   469,  1389,  1389,
    1392,  -837,  1187,  1406,  1189,  1088,  1314,  1232,   702,  -837,
     470,   473,  1318,   703,  -836,  1236,  1280,  1237,  1281,  -836,
     704,   705,   706,   707,  1285,   702,  1286,  1364,  1700,   474,
     703,  -836,    98,  -836,   493,   908,   476,   912,  1419,   477,
    1421,   916,   918,   920,    99,   810,   704,   705,   706,   707,
    1395,   478,  1396,  1365,  1187,   495,  1485,   700,   133,  1515,
     700,  1775,  1522,  1850,   480,  -837,   497,   498,  -837,   100,
     704,   705,   706,   707,   507,  -837,   517,  1366,  -837,    14,
     523,  1732,  -837,  1370,  1614,  1761,   524,  -837,   535,   538,
    -837,   704,   705,   706,   707,  -838,   546,   467,  1367,   467,
     559,  1466,   566,  -838,  -836,  -836,  -837,  -836,   569,   579,
     583,    78,    78,   634,   640,   702,   467,   638,   700,   195,
     703,   673,  1017,   674,   675,  1732,   309,   676,   677,   678,
    -837,   679,   680,  -837,  -837,   738,   681,  -962,   101,   704,
     705,   706,   707,  -837,   683,   684,  1368,   682,   102,   704,
     705,   706,   707,  1846,   685,  1174,  1506,   709,   690,  -838,
    -837,  -978,  -838,   691,   727,  1732,  1790,   701,   713,  -838,
     702,   708,  -838,   103,   730,   703,  -838,   718,   735,  1810,
     737,  -838,  -837,   755,  -838,  1533,   762,  -837,    78,   751,
    1448,   104,   759,   763,   704,   705,   706,   707,  1759,  -837,
    -838,  -837,  1475,   760,  1732,   513,  1479,  1732,   764,   704,
     705,   706,   707,   766,   768,   771,  1507,  1693,   526,   779,
     105,   106,   774,   107,  -838,   780,   782,  -838,  -838,  1732,
     108,   109,   110,   111,   112,   783,   113,  -838,   792,   114,
     115,   116,   117,   118,   816,   817,   818,   119,   120,   704,
     705,   706,   707,   702,  -838,  -426,  1514,   823,   703,   276,
     828,   829,   833,   834,   835,   838,   704,   705,   706,   707,
    1762,   836,  -837,  -837,   854,  -837,  -838,   702,     1,   839,
     844,  -838,   703,   848,  1336,   702,   849,   855,  1545,   864,
     703,   866,   867,  -838,   875,  -838,   876,   877,   467,     2,
    1590,   881,   210,   511,  1017,   195,   430,     3,  1492,   941,
     935,   970,   971,  1389,     4,   702,     5,   973,     6,   978,
     703,   982,   983,     7,   993,   735,   999,   212,   483,  1008,
       8,   104,  1012,  1033,  1028,   845,   150,   492,   513,  1039,
    1654,  1038,  1040,   795,     9,  1054,  1048,  1045,  1046,  1055,
    1056,  1062,  1252,  1066,  1072,    10,   704,   705,   706,   707,
     105,   106,  1077,   107,  1081,  1086,  -838,  -838,   521,  -838,
     108,   109,   110,   111,   112,  1803,   113,  1091,  1103,   114,
     115,   116,   117,   118,  1104,   427,   723,   119,   120,   921,
     922,   923,   924,   925,   926,   927,   928,  1161,  1173,   702,
    1125,  1202,  1688,  1205,  1215,  1163,  1658,  1165,  1185,  1196,
    1198,   704,   705,   706,   707,  1218,  1639,  1226,  1224,   710,
    1239,  1242,    11,  1533,  1253,  1262,    78,  1264,  1274,    12,
    1282,  1284,    13,  1291,    14,    15,   511,  1294,  1295,  1297,
    1298,  1307,  1097,    61,  1316,  1319,  1309,   189,  1689,  1320,
    1321,  1686,   890,  1322,    62,  1533,  1333,  1328,  1329,    16,
    1372,    63,  1374,  1334,  1383,  1335,  1337,   687,  1338,  1665,
    1345,   692,   693,   697,    64,  1401,  1346,  1347,  1348,  1350,
    1402,  1351,   947,  1399,  1352,  1353,  1403,   150,  1354,  1355,
    1404,  1397,   192,  1407,   704,   705,   706,   707,  1736,  1533,
    1408,  1702,  1415,  1418,  1427,  1703,  1420,  1425,    17,  1435,
     149,   150,  1439,  1440,  1441,  1442,  1443,  1444,   704,   705,
     706,   707,  1718,    18,    19,  1469,   704,   705,   706,   707,
    1512,  1330,  1513,   732,  1473,    65,  1517,   734,  1447,  1533,
    1462,  1474,  1478,  1484,  1518,  1581,  1504,   947,   212,  1535,
     513,    66,  1591,  1533,  1593,  1594,   704,   705,   706,   707,
     195,  1596,  1595,  1597,  1598,  1600,  1601,  1612,    67,  1610,
    1602,  1615,  1619,  1620,  1621,  1623,  1645,  1624,  1533,  1625,
    1626,  1533,   387,    68,   513,   513,  1768,    69,  1629,   868,
      70,  1630,  1631,   595,  1632,   149,   150,  1633,    71,  1634,
    1635,  1636,  1637,  1533,  1638,  1646,  1663,   869,   131,   513,
     389,   596,  1756,   150,   597,  1647,  1672,    72,  1666,    97,
     513,  1668,  1669,   513,  1670,  1671,  1677,   189,  1770,  1682,
    1678,  1679,  1680,  1684,  1681,  1685,    78,  1711,  1771,   391,
    1721,  1737,  1673,  1745,  1705,   149,   150,  1780,   511,  1712,
    1782,  1785,  1787,  1800,  1824,   598,  1756,   150,  1794,  1758,
      98,   599,   600,   601,  1766,   602,   603,   604,  1807,   605,
    1789,  1793,    99,  1806,  1777,   159,   795,  1778,  1830,  1831,
    1827,  1845,   511,   511,  1823,  1775,  1834,   149,   150,   606,
    1848,   607,  1849,  1856,  1756,   150,  1857,   100,   192,   461,
     137,   479,  1386,   468,  1317,   972,   865,   511,   889,  1102,
    1315,  1604,  1303,  1241,   396,   515,   593,   530,   511,   547,
    1009,   511,   608,  1304,   192,   492,  1043,   830,  1311,   914,
     398,  1107,  1603,   562,  1714,  1583,   930,  1783,  1589,   609,
     564,   937,   430,  1776,   399,   400,   401,  1781,   203,   204,
     192,   430,   944,  1817,   948,   949,   950,   951,   952,   944,
    1036,   610,  1429,  1760,  1720,  1809,   101,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   102,  1233,  1744,  1538,
    1731,  1411,   414,   415,   554,   570,   545,  1078,   492,   815,
    1069,   419,  1459,   852,   138,   508,   611,   612,   750,  1000,
    1011,   103,   289,  1217,   746,   995,  1709,  1651,  1822,  1244,
     492,  1120,  1799,   613,  1121,   521,  1854,    43,   384,   104,
      44,    47,   979,  1341,  1424,   758,  1211,   385,  1376,   246,
    1843,  1292,   811,     0,     0,   996,     0,     0,     0,     0,
     386,     0,     0,     0,     0,     0,   387,   388,   105,   106,
       0,   107,   104,     0,     0,     0,     0,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,   389,   119,   120,   390,     0,     0,
       0,   105,   106,     0,   107,     0,     0,     0,     0,     0,
       0,   108,   109,   110,   111,   112,     0,   113,     0,     0,
     114,   115,   116,   117,   118,   392,   393,     0,   119,   120,
       0,     0,   892,     0,     0,     0,   561,   394,     0,   257,
     702,   395,     0,     0,     0,   703,     0,     0,   893,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1118,  1119,  1124,     0,     0,  1132,  1133,
    1136,     0,  1144,  1147,  1149,  1152,  1155,  1158,  1159,  1160,
       0,  1162,     0,  1164,   894,     0,  1167,  1168,  1169,  1170,
    1171,  1172,     0,   895,     0,     0,  1122,     0,   396,     0,
       0,   896,     0,   430,   397,     0,  1179,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
     897,     0,   898,     0,     0,     0,     0,     0,   399,   400,
     401,   402,   203,   204,     0,     0,   899,     0,   900,   901,
     902,     0,   403,     0,     0,     0,     0,     0,     0,   492,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,     0,   414,   415,     0,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,   903,
       0,     0,   420,   421,     0,     0,    83,     0,   482,     0,
     423,     0,     0,    84,   278,     0,     0,    85,    86,   104,
       0,  1130,     0,     0,  1121,     0,    87,     0,   384,     0,
       0,     0,     0,     0,     0,     0,     0,   385,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   106,
     386,   107,     0,     0,     0,     0,   387,   388,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,   389,     0,     0,   390,   904,   905,
     906,   704,   705,   706,   707,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,   944,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,  1339,  1340,
      90,  1342,  1344,     0,     0,     0,     0,   394,     0,  1349,
       0,   395,     0,     0,     0,    91,     0,     0,     0,    92,
       0,     0,     0,     0,     0,   567,  1362,     0,     0,     0,
      93,     0,     0,     0,  1369,     0,   430,     0,     0,     0,
       0,     0,   430,     0,  1179,  1377,   430,  1379,  1380,    94,
       0,     0,     0,  1481,  1482,     0,     0,     0,     0,  1486,
    1487,     0,  1489,  1490,     0,     0,  1122,     0,   396,   492,
    1494,  1495,  1496,  1497,   397,  1498,  1499,  1500,  1501,  1502,
    1503,   955,     0,     0,   398,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
     401,   402,   203,   204,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,  1430,     0,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,     0,   414,   415,     0,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
       0,     0,   420,   421,     0,     0,  1134,   492,   482,  1121,
     423,     0,     0,   384,   278,     0,     0,     0,     0,     0,
       0,     0,   385,     0,     0,     0,     0,     0,  1480,     0,
       0,     0,     0,     0,     0,   386,     0,  1488,   104,     0,
       0,   387,   388,     0,  1493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   106,   389,
     107,     0,   390,     0,   430,   430,     0,   108,   109,   110,
     111,   112,     0,   113,     0,   104,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120,   492,     0,     0,     0,
     392,   393,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,   105,   106,   395,   107,     0,     0,
       0,     0,     0,     0,   108,   109,   110,   111,   112,     0,
     113,     0,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,     0,     0,     0,   956,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1122,     0,   396,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,  1627,  1628,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,   401,   402,   203,   204,     0,
       0,     0,     0,     0,   430,     0,     0,   403,   594,     0,
       0,     0,   595,     0,     0,     0,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,     0,
     596,   414,   415,   597,   416,   417,   418,   110,   111,   112,
     419,   113,     0,  1667,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,     0,     0,     0,   420,   421,     0,
       0,  1142,     0,   482,  1121,   423,     0,     0,   384,   278,
       0,     0,     0,     0,   598,     0,     0,   385,     0,     0,
     599,   600,   601,     0,   602,   603,   604,     0,   605,     0,
     386,     0,     0,     0,     0,     0,   387,   388,     0,     0,
       0,     0,     0,     0,     0,  1701,     0,     0,   606,     0,
     607,     0,     0,  1704,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   389,     0,     0,   390,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   608,     0,     0,     0,     0,     0,     0,     0,  1717,
       0,     0,     0,  1719,     0,   392,   393,     0,   609,  1145,
       0,     0,  1121,     0,     0,     0,   384,   394,     0,     0,
       0,   395,     0,     0,     0,   385,     0,     0,     0,     0,
     610,     0,     0,     0,     0,     0,     0,     0,   386,     0,
       0,     0,     0,     0,   387,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,  1763,     0,     0,     0,   611,   612,     0,     0,     0,
       0,     0,   389,     0,     0,   390,  1122,     0,   396,     0,
       0,     0,   613,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,   399,   400,
     401,   402,   203,   204,     0,   394,     0,     0,     0,   395,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     105,   413,     0,   107,     0,     0,   414,   415,   959,   416,
     417,   418,   110,   111,   112,   419,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
       0,     0,   420,   421,  1122,     0,   396,     0,   482,     0,
     423,     0,   397,     0,   278,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
     959,   107,     0,     0,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,     0,     0,   114,   115,   116,
     117,   118,     0,   962,     0,   119,   120,     0,     0,     0,
     420,   421,  1150,     0,     0,  1121,   482,     0,   423,   384,
     104,     0,   278,     0,     0,     0,     0,     0,   385,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   386,     0,     0,     0,     0,     0,   387,   388,   105,
     106,     0,   107,     0,   960,     0,     0,     0,     0,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,   115,
     116,   117,   118,     0,     0,   389,   119,   120,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     961,     0,     0,     0,     0,   962,     0,     0,     0,     0,
       0,   963,     0,     0,     0,     0,   392,   393,     0,     0,
    1153,     0,   104,  1121,     0,     0,     0,   384,   394,     0,
     964,     0,   395,     0,     0,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,   105,   106,     0,   107,   387,   388,     0,     0,     0,
       0,   108,   109,   110,   111,   112,     0,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,   389,     0,     0,   390,  1122,     0,   396,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,   392,   393,     0,     0,     0,   399,
     400,   401,   402,   203,   204,     0,   394,     0,     0,     0,
     395,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,     0,   420,   421,  1122,     0,   396,     0,   482,
       0,   423,     0,   397,     0,   278,   392,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
     402,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   105,
     413,     0,   107,     0,     0,   414,   415,     0,   416,   417,
     418,   110,   111,   112,   419,   113,     0,     0,   114,   115,
     116,   117,   118,     0,     0,     0,   119,   120,     0,     0,
       0,   420,   421,  1156,     0,     0,  1121,   482,     0,   423,
     384,     0,     0,   278,     0,     0,     0,     0,     0,   385,
      75,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   386,   403,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   413,     0,   107,     0,   458,     0,     0,     0,
     416,   417,   418,   110,   111,   112,   389,   113,     0,   390,
     114,   115,   116,   117,   118,     0,     0,     0,   119,   120,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,   384,     0,   394,
       0,   105,   106,   395,   107,     0,   385,     0,     0,     0,
       0,   108,   109,   110,   111,   112,     0,   113,     0,   386,
     114,   115,   116,   117,   118,   387,   388,     0,   119,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   651,     0,     0,     0,     0,     0,
       0,     0,     0,   389,     0,     0,   390,     0,  1122,     0,
     396,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,   392,   393,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,   394,     0,     0,     0,
     395,     0,     0,     0,   403,     0,     0,     0,   882,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,   694,     0,   119,
     120,   653,     0,  1648,   420,   421,     0,   396,     0,     0,
     482,     0,   423,   397,     0,     0,   278,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,  1649,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
     402,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   105,
     413,     0,   107,     0,     0,   414,   415,     0,   416,   417,
     418,   110,   111,   112,   419,   113,  1650,     0,   114,   115,
     116,   117,   118,   883,   884,   885,   119,   120,     0,   384,
       0,   420,   421,     0,     0,     0,     0,   657,   385,   423,
       0,     0,     0,   278,     0,     0,     0,     0,     0,     0,
     104,   386,     0,     0,   650,     0,     0,   387,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   651,     0,     0,   105,
     106,     0,   107,     0,     0,   389,     0,     0,   390,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,   115,
     116,   117,   118,   463,     0,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   393,     0,     0,
     104,     0,     0,     0,     0,     0,   384,     0,   394,     0,
       0,     0,   395,     0,     0,   385,     0,     0,     0,     0,
     652,     0,     0,     0,     0,     0,     0,     0,   386,   105,
     106,     0,   107,     0,   387,   388,     0,     0,     0,   108,
     109,   110,   111,   112,     0,   113,     0,     0,   114,   115,
     116,   117,   118,   651,     0,     0,   119,   120,     0,     0,
       0,     0,   389,   653,     0,   390,     0,     0,     0,   396,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   399,
     400,   401,   402,   203,   204,   394,     0,     0,     0,   395,
       0,     0,     0,   403,     0,     0,     0,   878,     0,     0,
       0,     0,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   105,   413,     0,   107,     0,     0,   414,   415,     0,
     416,   417,   418,   110,   111,   112,   419,   113,     0,     0,
     114,   115,   654,   655,   656,     0,     0,     0,   119,   120,
     653,     0,     0,   420,   421,     0,   396,     0,     0,   657,
       0,   423,   397,     0,     0,   278,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,   384,   416,   417,   418,
     110,   111,   112,   419,   113,   385,     0,   114,   115,   654,
     655,   656,     0,     0,     0,   119,   120,     0,   386,   104,
     420,   421,     0,     0,   387,   388,   657,     0,   423,     0,
       0,     0,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   651,     0,     0,     0,     0,   105,   106,
       0,   107,   389,     0,     0,   390,     0,     0,   108,   109,
     110,   111,   112,     0,   113,     0,     0,   114,   115,   116,
     117,   118,     0,     0,     0,   119,   120,     0,     0,     0,
       0,     0,     0,   392,   393,     0,  1825,   104,   423,     0,
       0,     0,     0,   384,     0,   394,     0,     0,     0,   395,
       0,     0,   385,     0,     0,     0,     0,   652,     0,     0,
       0,     0,     0,     0,     0,   386,   105,   106,     0,   107,
       0,   387,   388,     0,     0,     0,   108,   109,   110,   111,
     112,     0,   113,     0,     0,   114,   115,   116,   117,   118,
     651,     0,     0,   119,   120,     0,     0,     0,     0,   389,
     653,     0,   390,     0,   987,     0,   396,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
     392,   393,     0,     0,     0,     0,   399,   400,   401,   402,
     203,   204,   394,     0,     0,     0,   395,     0,     0,     0,
     403,     0,     0,     0,   878,     0,     0,     0,     0,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   105,   413,
       0,   107,     0,     0,   414,   415,     0,   416,   417,   418,
     110,   111,   112,   419,   113,     0,     0,   114,   115,   654,
     655,   656,     0,     0,     0,   119,   120,   653,     0,     0,
     420,   421,     0,   396,     0,     0,   657,     0,   423,   397,
       0,     0,   278,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,   401,   402,   203,   204,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,     0,
       0,   414,   415,   384,   416,   417,   418,   110,   111,   112,
     419,   113,   385,     0,   114,   115,   116,   117,   118,     0,
       0,     0,   119,   120,   104,   386,     0,   420,   421,     0,
       0,   387,   388,   657,     0,   423,     0,     0,     0,   278,
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
       0,  1695,     0,   396,   108,   109,   110,   111,   112,   397,
     113,     0,     0,   114,   115,   116,   117,   118,     0,   398,
       0,   119,   120,     0,     0,   391,     0,   392,   393,     0,
    1657,     0,     0,   399,   400,   401,   402,   203,   204,   394,
       0,     0,     0,   395,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   105,   413,     0,   107,     0,
       0,   414,   415,     0,   416,   417,   418,   110,   111,   112,
     419,   113,  1427,     0,   114,   115,   116,   117,   118,     0,
     694,     0,   119,   120,     0,     0,     0,   420,   421,   487,
     396,     0,     0,   482,     0,   423,   397,     0,     0,   278,
       0,     0,     6,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,  1546,  1547,     0,  1548,     0,     0,  1549,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,  1550,   403,     0,     0,     0,     0,  1551,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   386,     0,   420,   421,     0,     0,   387,   388,
     482,     0,   423,     0,     0,     0,   278,     0,     0,  1552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,    14,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1553,     0,     0,     0,     0,     0,     0,
     384,     0,     0,    16,     0,   391,     0,   392,   393,   385,
       0,     0,  1673,     0,  1554,     0,     0,     0,     0,   394,
       0,   104,   386,   395,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1555,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
    1556,   116,   117,   118,     0,     0,     0,   119,   120,     0,
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
     422,     0,   423,     0,   686,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   386,     0,   420,   421,     0,     0,   387,   388,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,   104,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,     0,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   386,   395,   105,   106,     0,   107,   387,   388,
       0,     0,     0,     0,   108,   109,   110,   111,   112,     0,
     113,     0,     0,   114,   115,   116,   117,   118,     0,     0,
       0,   119,   120,     0,     0,     0,   389,    76,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     694,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,   391,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,     0,     0,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,   104,   386,     0,   420,   421,     0,     0,   387,   388,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,   107,     0,     0,   389,     0,     0,   390,
     108,   109,   110,   111,   112,     0,   113,     0,     0,   114,
     115,   116,   117,   118,     0,     0,     0,   119,   120,     0,
     384,     0,     0,     0,   956,     0,     0,   392,   393,   385,
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
     120,     0,     0,     0,   420,   421,   398,     0,     0,     0,
     482,   953,   423,     0,     0,     0,   278,  1148,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,   104,   386,     0,   420,   421,     0,     0,   387,   388,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
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
    1330,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     482,     0,   423,     0,     0,     0,   278,  1343,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
     384,   416,   417,   418,   110,   111,   112,   419,   113,   385,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,   386,     0,   420,   421,     0,     0,   387,   388,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,     0,   392,   393,   385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   386,   395,     0,     0,     0,     0,   387,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   389,     0,     0,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,   397,   392,   393,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,     0,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
     396,   114,   115,   116,   117,   118,   397,     0,     0,   119,
     120,     0,     0,     0,   420,   421,   398,     0,  1716,     0,
     482,     0,   423,     0,     0,     0,   278,     0,     0,     0,
     399,   400,   401,   402,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   105,   413,     0,   107,  1427,     0,   414,   415,
       0,   416,   417,   418,   110,   111,   112,   419,   113,     0,
       0,   114,   115,   116,   117,   118,     0,     0,     0,   119,
     120,     0,     0,     0,   420,   421,     6,     0,     0,     0,
     482,   594,   423,     0,     0,   595,   278,  1546,  1547,     0,
    1548,     0,   795,  1549,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   596,     0,     0,   597,  1550,     0,     0,
       0,     0,     0,  1551,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   598,     0,     0,
       0,     0,     0,   599,   600,   601,     0,   602,   603,   604,
       0,   605,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   594,     0,  1552,     0,   595,     0,     0,     0,     0,
       0,   606,     0,   607,     0,     0,     0,     0,     0,     0,
       0,     0,    14,   596,     0,     0,   597,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1553,     0,     0,
       0,     0,     0,     0,   608,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1554,     0,
       0,   609,     0,     0,     0,   104,     0,   598,     0,     0,
       0,     0,     0,   599,   600,   601,     0,   602,   603,   604,
       0,   605,     0,   610,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1555,   106,     0,   107,   104,     0,
       0,   606,     0,   607,   108,   109,   110,   111,   112,     0,
     113,     0,     0,   114,  1556,   116,   117,   118,   611,   612,
       0,   119,   120,     0,     0,     0,     0,   105,   106,     0,
     107,     0,     0,     0,   608,   613,     0,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,   115,   116,   117,
     118,   609,     0,     0,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   610,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   611,   612,
       0,     0,     0,     0,     0,     0,     0,   105,   106,     0,
     107,     0,     0,     0,     0,   613,     0,   108,   109,   110,
     111,   112,     0,   113,     0,     0,   114,   115,   116,   117,
     118,     0,     0,     0,   119,   120
};

static const short yycheck[] =
{
       9,    54,     4,    12,     4,   103,    16,    16,    55,    18,
       0,    56,   255,    63,    62,   286,   509,   149,   362,    66,
     362,   358,   354,    68,   139,   140,   422,   563,    62,   139,
     140,    93,   331,    69,    84,    88,   333,    61,   521,   709,
     495,   674,    89,    52,    53,    54,    55,    56,    57,    96,
     770,   470,  1040,    62,    63,    64,    92,    66,  1034,    68,
      69,  1261,    71,    72,  1040,   379,    67,   367,    87,   646,
     326,    80,    82,    82,    83,    84,   831,    86,    87,    88,
      89,    91,    91,    92,    93,    94,   131,    96,    72,    90,
     760,   153,   686,   553,   280,   555,   130,   307,   132,  1034,
     639,   264,   694,   563,   151,  1040,   862,   152,  1034,  1034,
      94,   274,  1034,   987,  1040,  1040,   939,   708,  1040,  1090,
     727,   130,   131,   132,  1041,   144,   759,   766,   763,  1085,
     228,   723,  1254,  1050,  1379,   144,  1560,   157,   219,   804,
     149,  1204,   151,   152,   153,     8,  1067,   727,   157,  1394,
       8,    15,  1175,     8,   977,    23,  1073,   792,  1181,     8,
       8,     8,  1467,  1407,   188,   652,   801,    50,     6,   476,
     657,    10,  1407,    47,  1407,   307,   652,    50,  1300,   422,
      46,  1259,  1407,    64,   160,  1196,    64,  1198,   140,   213,
      66,    26,   529,    29,     8,    47,    31,   270,  1309,  1407,
       8,    71,     8,    12,    84,   182,  1208,   539,   540,   541,
     542,    49,    19,     6,   514,   204,   261,   315,   133,   563,
     855,   563,  1641,   257,  1643,   270,    23,    47,  1750,  1751,
     311,   148,    26,    11,    33,     8,   163,    31,   291,   201,
     151,    25,   151,   736,   239,   204,     5,    46,   257,    20,
     295,   588,   261,    26,   249,   264,   265,    16,    31,   170,
     148,   270,    46,   190,    84,   274,   565,   184,  1091,   132,
      48,   186,   231,   307,   132,  1231,    66,   132,    94,    86,
     594,  1700,   291,   132,   132,   132,   295,   103,  1410,     6,
     279,    75,   103,   202,    91,   272,   890,   891,   332,  1287,
     463,    53,    54,   101,  1306,   339,   176,   341,   188,   343,
      94,  1287,   346,   183,   188,   614,  1838,   364,   132,   103,
      12,   273,   375,   332,   132,   109,   132,   136,   373,   365,
     339,   355,   341,   357,   343,     8,   188,   346,   470,     8,
     934,    26,   318,   127,  1218,   978,    31,   371,  1772,   185,
    1373,  1374,  1287,    26,  1432,   364,   365,   817,    31,  1276,
    1471,  1287,  1287,   970,   373,  1287,   375,   181,   188,    24,
     163,  1790,  1293,  1008,    26,   982,   983,   379,    33,    31,
    1045,  1046,  1806,  1807,   839,    11,  1397,  1587,  1693,   272,
     970,   878,    26,   274,   272,   882,   272,    31,   264,   272,
      26,  1645,   982,   674,   477,    31,   882,  1831,   276,   273,
    1645,   274,  1645,   458,   423,   462,   274,  1052,  1842,   274,
    1645,  1845,    48,   730,   469,   274,   274,   274,   473,   476,
    1095,   269,   477,   272,   269,   270,   470,  1645,    26,  1033,
     238,   212,  1677,    31,   136,   197,   163,   263,   691,   458,
    1205,   157,   263,   462,   463,  1518,  1095,   700,    66,  1704,
     469,   481,    59,   132,   473,    14,   737,   476,   477,   124,
     580,   148,   481,   267,   268,   269,   270,  1599,   559,   263,
     274,   780,   491,  1605,   783,  1685,  1721,    24,   759,  1445,
    1513,   146,    65,  1464,   267,   268,   269,   270,   153,    29,
     799,   198,   204,    19,  1697,    16,  1423,  1463,    53,    54,
     534,  1428,   181,  1430,   191,   177,  1236,  1111,  1112,   123,
    1114,  1115,   252,   779,  1101,   824,  1761,  1282,   102,   563,
    1404,     4,   187,   193,  1125,   191,    94,   200,  1738,     9,
    1775,   155,   204,  1299,    17,  1084,   756,   733,  1465,   630,
    1743,     3,   561,   853,   563,   718,   975,  1478,   567,   583,
     164,   875,   111,   271,    67,  1800,   163,   273,  1803,   866,
     272,    82,  1581,   146,   182,   272,  1776,   254,    81,    52,
     581,    54,   267,   268,   269,   270,     6,   124,    58,   274,
    1825,     7,   594,  1786,   267,   268,   269,   270,   254,    69,
     130,   175,   105,   204,   638,    89,   268,   211,  1215,   272,
       3,   635,   636,   273,   187,   267,   268,   269,   270,   877,
    1537,   204,   274,   881,   756,  1818,  1543,    47,    80,   638,
     114,   123,   742,   267,   268,   269,   270,  1646,   647,    59,
     274,   267,   268,   269,   270,   647,    26,   647,  1242,   207,
     208,    31,   197,  1706,  1707,   185,   159,  1721,   677,   200,
    1483,    46,    26,  1298,    84,   775,   776,    31,   677,   177,
    1264,     4,   202,    26,    25,  1592,   710,    47,    31,   267,
     268,   269,   270,   730,   204,    26,   274,    80,   172,  1021,
      31,   164,  1609,   702,  1747,    18,   204,  1761,  1418,    68,
     709,   710,   194,   173,   713,   272,   751,   978,   753,   718,
     277,    34,    68,    98,    84,    26,    40,   204,   727,    52,
      31,   730,   756,  1419,    75,  1713,   735,    51,   762,   738,
     147,   272,   766,   153,    58,   102,  1800,  1713,  1330,  1803,
    1039,   204,   751,   163,   753,   769,   770,   771,   268,    62,
      63,   760,  1051,   762,  1086,   789,    79,   766,   109,    83,
     268,  1825,  1395,  1034,   239,   240,  1819,   177,   188,  1040,
    1466,   279,   139,  1072,   141,  1692,   127,   778,  1713,  1262,
     789,   129,   816,   817,  1701,   108,   273,  1713,  1713,  1185,
      18,  1713,   279,   204,   204,     6,   959,   164,    26,   962,
     272,  1098,    47,    31,    83,   829,    34,   816,   817,   854,
     273,   207,   208,   204,  1408,    67,   279,   818,     6,   864,
    1695,   823,   189,  1359,   148,  1742,  1791,  1792,  1224,    81,
    1285,   164,   273,   272,   836,   274,  1291,    89,    49,    84,
     231,   120,    70,   975,   148,   854,   124,   848,   849,   182,
     851,    79,   175,   105,   133,   864,   180,   268,   268,    87,
     184,    49,   131,   273,  1161,   129,    26,   876,   146,    26,
    1745,    31,   129,   875,    31,   153,   180,   268,   106,     6,
     108,   129,  1847,   216,   272,    98,   274,   267,   268,   269,
     270,   195,   272,   182,   122,    26,   124,   125,   126,    83,
      31,   963,   240,   267,   268,   269,   270,   159,   153,   187,
     274,   124,  1787,   960,   267,   268,   269,   270,   163,    26,
     164,   274,    49,     0,    31,   961,   267,   268,   269,   270,
      67,     8,    67,   274,   273,     4,    88,  1208,     7,     6,
      26,   975,  1185,   188,    81,    31,    81,   175,     6,    20,
     959,   960,   961,   962,   963,   964,   267,   268,   269,   270,
    1070,   970,   149,   274,   281,   211,  1303,   213,   105,    64,
     105,   123,   289,   982,   983,  1274,   237,  1647,   987,   166,
     964,  1224,    49,    52,   245,   271,   272,    64,    26,   170,
      67,    49,   129,    31,   129,   207,   272,    74,    26,   151,
      77,   277,   154,    31,    81,   272,  1305,   137,   138,    86,
     277,  1196,    89,  1198,    10,  1359,  1287,  1359,   170,   272,
      62,   274,   159,  1516,   159,   272,  1519,   274,   105,   271,
     272,   203,   204,   205,    64,  1306,   264,   265,   266,   267,
     268,   269,   270,    26,   196,   272,   274,   274,    31,   272,
    1644,   274,   129,  1062,  1088,   132,   133,    83,    26,   231,
     232,  1095,  1064,    31,  1064,   142,   206,    82,  1077,   897,
     148,   899,  1666,   901,   902,  1669,   904,   905,   906,  1088,
    1081,     6,   159,  1415,   198,  1545,  1095,    86,  1203,  1204,
    1205,     0,   272,  1225,   274,   272,  1097,   274,    26,     8,
     273,    66,  1103,    31,   181,   272,   272,   274,   274,   186,
     267,   268,   269,   270,   272,    26,   274,   274,  1611,    64,
      31,   198,    47,   200,  1395,   673,   148,   675,  1238,   129,
    1240,   679,   680,   681,    59,  1434,   267,   268,   269,   270,
     272,   129,   274,   274,   272,   182,   274,   272,    73,   274,
     272,   272,   274,   274,   252,    64,   107,   196,    67,    84,
     267,   268,   269,   270,   180,    74,   130,   274,    77,   163,
     188,  1677,    81,  1174,  1473,    86,   200,    86,   262,   262,
      89,   267,   268,   269,   270,     0,    33,  1196,   274,  1198,
     157,  1301,   215,     8,   271,   272,   105,   274,   272,   264,
     118,  1203,  1204,    32,    10,    26,  1215,   273,   272,  1218,
      31,   273,  1236,   273,   273,  1721,  1225,   207,   273,   273,
     129,   273,   273,   132,   133,    10,   273,   273,   153,   267,
     268,   269,   270,   142,   207,   207,   274,   273,   163,   267,
     268,   269,   270,  1837,   273,    66,   274,   278,   273,    64,
     159,   278,    67,   273,    66,  1761,  1749,   276,   182,    74,
      26,   273,    77,   188,   148,    31,    81,   272,    66,  1775,
     272,    86,   181,   174,    89,  1407,   273,   186,  1280,   272,
    1280,   206,   264,    10,   267,   268,   269,   270,   271,   198,
     105,   200,  1316,   278,  1800,  1427,  1320,  1803,   272,   267,
     268,   269,   270,   128,   251,    94,   274,  1606,    94,   272,
     235,   236,    94,   238,   129,    24,    24,   132,   133,  1825,
     245,   246,   247,   248,   249,    46,   251,   142,    10,   254,
     255,   256,   257,   258,   182,   216,   143,   262,   263,   267,
     268,   269,   270,    26,   159,   182,   274,   143,    31,   204,
      23,    94,   193,   193,   145,    95,   267,   268,   269,   270,
     271,   273,   271,   272,    64,   274,   181,    26,     7,   273,
     276,   186,    31,   273,    57,    26,   273,    10,  1426,   129,
      31,   157,   272,   198,   135,   200,   128,   273,  1397,    28,
    1435,   273,  1426,  1427,  1418,  1404,   255,    36,    57,   274,
     197,   182,   210,  1518,    43,    26,    45,    63,    47,   273,
      31,    66,    66,    52,   198,    66,   194,  1426,   277,    10,
      59,   206,   248,   273,   204,    23,  1435,   286,  1560,   273,
    1540,    20,    46,    30,    73,   132,    24,    90,    90,   132,
     132,   164,  1713,   162,    46,    84,   267,   268,   269,   270,
     235,   236,   164,   238,   273,   102,   271,   272,   317,   274,
     245,   246,   247,   248,   249,    86,   251,   163,   273,   254,
     255,   256,   257,   258,    20,  1718,   163,   262,   263,   218,
     219,   220,   221,   222,   223,   224,   225,    10,    66,    26,
     273,   131,  1590,   113,    66,   274,  1546,   272,   272,   272,
     272,   267,   268,   269,   270,   272,  1508,    74,   273,   272,
      12,   273,   151,  1645,    45,   247,  1518,   273,    10,   158,
     168,   274,   161,   275,   163,   164,  1560,   274,   274,   274,
      10,   200,   140,    40,    55,   238,   186,  1546,  1591,    55,
     274,  1586,     8,   274,    51,  1677,    98,   273,   273,   188,
      54,    58,   181,   273,    73,   273,   273,   416,   273,  1549,
     273,   420,   421,   422,    71,   134,   273,   273,   273,   273,
     134,   273,  1581,   274,   273,   273,   134,  1586,   273,   273,
      89,   272,  1591,    20,   267,   268,   269,   270,  1686,  1721,
      77,  1615,   168,   136,    17,  1619,   137,   192,   237,   153,
    1610,  1610,   132,   132,   132,   132,   132,   132,   267,   268,
     269,   270,  1659,   252,   253,    24,   267,   268,   269,   270,
      54,   163,   181,   482,   273,   132,   131,   486,   274,  1761,
     274,   274,   274,   274,   182,    26,   274,  1646,  1647,   142,
    1772,   148,    83,  1775,   188,    84,   267,   268,   269,   270,
    1659,    47,   188,    84,    47,   274,   274,   131,   165,    66,
      98,   117,   117,   274,   274,   274,   272,   274,  1800,   274,
     274,  1803,    41,   180,  1806,  1807,  1726,   184,   274,    19,
     187,   274,   274,    23,   274,  1695,  1695,   274,   195,   274,
     274,   274,   274,  1825,   274,    26,   271,    37,    86,  1831,
      69,    41,  1712,  1712,    44,   278,    54,   214,   273,     6,
    1842,   271,   273,  1845,   271,   271,    86,  1726,  1727,   274,
     271,   271,   271,    98,   271,    90,  1728,   114,  1728,    98,
      86,   129,   197,   272,   274,  1745,  1745,   129,  1772,   273,
     129,   274,    89,   156,  1797,    85,  1756,  1756,   272,   271,
      47,    91,    92,    93,   271,    95,    96,    97,    50,    99,
     274,   274,    59,   181,    47,   133,    30,   188,    39,    50,
     271,    53,  1806,  1807,   273,   272,   209,  1787,  1787,   119,
     274,   121,   272,     0,  1794,  1794,     0,    84,  1797,   262,
      12,   274,  1201,   265,  1101,   718,   642,  1831,   657,   867,
    1098,  1454,  1087,  1024,   173,   313,   359,   322,  1842,   329,
     762,  1845,   152,  1088,  1823,   674,   800,   588,  1095,   678,
     189,  1830,  1453,   341,  1656,  1432,   685,  1740,  1434,   169,
     341,   690,   691,  1736,   203,   204,   205,  1739,   207,   208,
    1849,   700,   701,  1778,   703,   704,   705,   706,   707,   708,
     789,   191,  1255,  1718,  1664,  1773,   153,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   163,  1010,  1695,  1411,
    1675,  1233,   241,   242,   334,   346,   327,   846,   737,   569,
     838,   250,  1289,   627,    13,   299,   226,   227,   501,   751,
     764,   188,   142,   987,   497,   737,  1645,  1535,  1794,   268,
     759,     6,  1756,   243,     9,   764,  1849,     0,    13,   206,
       0,     0,   726,  1139,  1244,   509,   979,    22,  1183,    86,
    1830,  1077,   567,    -1,    -1,   738,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    41,    42,   235,   236,
      -1,   238,   206,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    69,   262,   263,    72,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,   100,   101,    -1,   262,   263,
      -1,    -1,    18,    -1,    -1,    -1,    33,   112,    -1,   273,
      26,   116,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   892,   893,   894,    -1,    -1,   897,   898,
     899,    -1,   901,   902,   903,   904,   905,   906,   907,   908,
      -1,   910,    -1,   912,    70,    -1,   915,   916,   917,   918,
     919,   920,    -1,    79,    -1,    -1,   171,    -1,   173,    -1,
      -1,    87,    -1,   932,   179,    -1,   935,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
     106,    -1,   108,    -1,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,   122,    -1,   124,   125,
     126,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,   978,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,   175,
      -1,    -1,   267,   268,    -1,    -1,    51,    -1,   273,    -1,
     275,    -1,    -1,    58,   279,    -1,    -1,    62,    63,   206,
      -1,     6,    -1,    -1,     9,    -1,    71,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,   264,   265,
     266,   267,   268,   269,   270,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,    -1,    -1,  1125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,  1137,  1138,
     165,  1140,  1141,    -1,    -1,    -1,    -1,   112,    -1,  1148,
      -1,   116,    -1,    -1,    -1,   180,    -1,    -1,    -1,   184,
      -1,    -1,    -1,    -1,    -1,    33,  1165,    -1,    -1,    -1,
     195,    -1,    -1,    -1,  1173,    -1,  1175,    -1,    -1,    -1,
      -1,    -1,  1181,    -1,  1183,  1184,  1185,  1186,  1187,   214,
      -1,    -1,    -1,  1328,  1329,    -1,    -1,    -1,    -1,  1334,
    1335,    -1,  1337,  1338,    -1,    -1,   171,    -1,   173,  1208,
    1345,  1346,  1347,  1348,   179,  1350,  1351,  1352,  1353,  1354,
    1355,    42,    -1,    -1,   189,  1224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,  1257,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,   267,   268,    -1,    -1,     6,  1306,   273,     9,
     275,    -1,    -1,    13,   279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,  1327,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,  1336,   206,    -1,
      -1,    41,    42,    -1,  1343,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    69,
     238,    -1,    72,    -1,  1373,  1374,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,   206,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,  1395,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   235,   236,   116,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,    -1,   269,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,  1491,  1492,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,  1513,    -1,    -1,   217,    19,    -1,
      -1,    -1,    23,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      41,   241,   242,    44,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,  1552,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,   267,   268,    -1,
      -1,     6,    -1,   273,     9,   275,    -1,    -1,    13,   279,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    22,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1614,    -1,    -1,   119,    -1,
     121,    -1,    -1,  1622,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1658,
      -1,    -1,    -1,  1662,    -1,   100,   101,    -1,   169,     6,
      -1,    -1,     9,    -1,    -1,    -1,    13,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1718,
      -1,  1720,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,
      -1,    -1,   243,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    74,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,    -1,
     275,    -1,   179,    -1,   279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      74,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   189,    -1,   262,   263,    -1,    -1,    -1,
     267,   268,     6,    -1,    -1,     9,   273,    -1,   275,    13,
     206,    -1,   279,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,   235,
     236,    -1,   238,    -1,   148,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    69,   262,   263,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
       6,    -1,   206,     9,    -1,    -1,    -1,    13,   112,    -1,
     214,    -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,   235,   236,    -1,   238,    41,    42,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    69,    -1,    -1,    72,   171,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,
      -1,   275,    -1,   179,    -1,   279,   100,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,   267,   268,     6,    -1,    -1,     9,   273,    -1,   275,
      13,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    22,
     204,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,   217,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,   238,    -1,   180,    -1,    -1,    -1,
     244,   245,   246,   247,   248,   249,    69,   251,    -1,    72,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,
      -1,   235,   236,   116,   238,    -1,    22,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    35,
     254,   255,   256,   257,   258,    41,    42,    -1,   262,   263,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   163,    -1,   262,
     263,   167,    -1,    89,   267,   268,    -1,   173,    -1,    -1,
     273,    -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   172,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,    13,
      -1,   267,   268,    -1,    -1,    -1,    -1,   273,    22,   275,
      -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    35,    -1,    -1,    38,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,   189,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   235,
     236,    -1,   238,    -1,    41,    42,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    60,    -1,    -1,   262,   263,    -1,    -1,
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
      -1,    -1,    47,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,    61,    -1,    -1,    64,
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
      -1,    -1,   197,    -1,   199,    -1,    -1,    -1,    -1,   112,
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
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   204,    -1,   206,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,   235,   236,    -1,   238,    41,    42,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    69,   268,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    98,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
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
      13,    -1,    -1,    -1,   269,    -1,    -1,   100,   101,    22,
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
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,   271,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    17,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,    47,    -1,    -1,    -1,
     273,    19,   275,    -1,    -1,    23,   279,    58,    59,    -1,
      61,    -1,    30,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    44,    78,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   144,    -1,    23,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    41,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,
      -1,   169,    -1,    -1,    -1,   206,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,   206,    -1,
      -1,   119,    -1,   121,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,   226,   227,
      -1,   262,   263,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,   152,   243,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,   169,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
			  yyval = (DSQL_NOD) NULL; }
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
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;
case 517:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
    break;
case 518:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 519:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
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
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
    break;
case 541:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
    break;
case 542:
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
case 543:
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
case 546:
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
case 547:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 550:
{ yyval = yyvsp[-1]; }
    break;
case 551:
{ yyval = 0; }
    break;
case 555:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;
case 556:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;
case 557:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;
case 561:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 562:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 563:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 564:
{ yyval = 0; }
    break;
case 565:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 568:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 569:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 572:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 573:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 575:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 576:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 578:
{ yyval = (DSQL_NOD) NULL; }
    break;
case 580:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 585:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 586:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 587:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 588:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 589:
{ yyval = yyvsp[0];}
    break;
case 591:
{ yyval = yyvsp[0];}
    break;
case 592:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 597:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 598:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 599:
{ yyval = 0; }
    break;
case 600:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 601:
{ yyval = (DSQL_NOD) NOD_SHARED; }
    break;
case 602:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;
case 603:
{ yyval = (DSQL_NOD) 0; }
    break;
case 604:
{ yyval = (DSQL_NOD) NOD_READ; }
    break;
case 605:
{ yyval = (DSQL_NOD) NOD_WRITE; }
    break;
case 607:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 608:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 609:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 610:
{ yyval = 0; }
    break;
case 612:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 613:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 614:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 615:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 616:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 617:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 618:
{ yyval = make_list (yyvsp[0]); }
    break;
case 619:
{ yyval = 0; }
    break;
case 621:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 622:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 623:
{ yyval = 0; }
    break;
case 624:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 625:
{ yyval = 0; }
    break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 627:
{ yyval = 0; }
    break;
case 628:
{ yyval = yyvsp[-1]; }
    break;
case 629:
{ yyval = 0; }
    break;
case 630:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 631:
{ yyval = 0; }
    break;
case 632:
{ yyval = yyvsp[0]; }
    break;
case 633:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 634:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 635:
{ yyval = 0; }
    break;
case 636:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;
case 637:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 638:
{ lex.limit_clause = true; }
    break;
case 639:
{ lex.limit_clause = false; }
    break;
case 640:
{ lex.first_detection = true; }
    break;
case 641:
{ lex.first_detection = false; }
    break;
case 642:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 643:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;
case 644:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;
case 645:
{ yyval = 0; }
    break;
case 646:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 647:
{ yyval = yyvsp[-2]; }
    break;
case 648:
{ yyval = yyvsp[-1]; }
    break;
case 649:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 650:
{ yyval = yyvsp[-1]; }
    break;
case 651:
{ yyval = yyvsp[0]; }
    break;
case 652:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 653:
{ yyval = 0; }
    break;
case 654:
{ yyval = make_list (yyvsp[0]); }
    break;
case 655:
{ yyval = 0; }
    break;
case 657:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 659:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 660:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 661:
{ yyval = make_list (yyvsp[0]); }
    break;
case 663:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 666:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 667:
{ yyval = yyvsp[-1]; }
    break;
case 668:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
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
case 695:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 699:
{ yyval = yyvsp[0]; }
    break;
case 700:
{ yyval = 0; }
    break;
case 701:
{ yyval = yyvsp[0]; }
    break;
case 702:
{ yyval = 0; }
    break;
case 703:
{ yyval = yyvsp[0]; }
    break;
case 704:
{ yyval = 0; }
    break;
case 705:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 706:
{ yyval = 0; }
    break;
case 707:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 708:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 709:
{ yyval = 0; }
    break;
case 710:
{ yyval = 0; }
    break;
case 712:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 713:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 717:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 718:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 719:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
    break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 722:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 723:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 728:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;
case 729:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 730:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;
case 733:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;
case 734:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 737:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 740:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 741:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 743:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 747:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 748:
{ yyval = yyvsp[0]; }
    break;
case 750:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
    break;
case 759:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 763:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 764:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 765:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 767:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 771:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 775:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 778:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 780:
{ yyval = yyvsp[-1]; }
    break;
case 781:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 798:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 801:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 802:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 803:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 804:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 805:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 806:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 807:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 808:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 809:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 810:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 811:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 812:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 813:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 814:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 815:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 818:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 820:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 822:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 824:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 826:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 828:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 830:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 832:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 836:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 837:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 838:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 839:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 840:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;
case 841:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;
case 843:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 844:
{ yyval = yyvsp[-1]; }
    break;
case 845:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 846:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;
case 856:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 857:
{ yyval = yyvsp[0]; }
    break;
case 858:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 859:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 860:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;
case 861:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 862:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 863:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 864:
{ yyval = yyvsp[-1]; }
    break;
case 865:
{ yyval = yyvsp[-1]; }
    break;
case 869:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 870:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 871:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
    break;
case 872:
{ yyval = yyvsp[0]; }
    break;
case 873:
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
case 874:
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
case 875:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 876:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 878:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 880:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 881:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;
case 882:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 884:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 885:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;
case 887:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;
case 888:
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
case 889:
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
case 890:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 891:
{ yyval = make_parameter (); }
    break;
case 892:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 893:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 894:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 895:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 896:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 897:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 898:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 899:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 900:
{ yyval = yyvsp[0]; }
    break;
case 901:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 903:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 904:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 905:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 906:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 907:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 909:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;
case 910:
{ yyval = yyvsp[0];}
    break;
case 915:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 916:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 917:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 918:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 919:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 920:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 921:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 922:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 923:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 924:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 925:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 926:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
    break;
case 927:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 928:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
    break;
case 929:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
    break;
case 930:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 931:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 932:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 933:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 936:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 937:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 940:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 941:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 943:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 944:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 945:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 946:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 947:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;
case 954:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 955:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 956:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;
case 957:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 958:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 961:
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
