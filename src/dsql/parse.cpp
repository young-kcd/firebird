/* A Bison parser, made from E:\WinCvs\Root\FB2\FB2.HEAD\src\dsql\parse.y
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
# define	BOTH	521
# define	COLLATION	522
# define	COMMENT	523
# define	BIT_LENGTH	524
# define	CHAR_LENGTH	525
# define	CHARACTER_LENGTH	526
# define	LEADING	527
# define	KW_LOWER	528
# define	OCTET_LENGTH	529
# define	TRAILING	530
# define	TRIM	531
# define	RETURNING	532
# define	KW_IGNORE	533
# define	LIMBO	534
# define	UNDO	535
# define	REQUESTS	536
# define	TIMEOUT	537


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
#include "../jrd/intlobj_new.h"

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



#define	YYFINAL		2029
#define	YYFLAG		-32768
#define	YYNTBASE	300

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 537 ? yytranslate[x] : 760)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     293,   294,   289,   287,   292,   288,   298,   290,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   295,   291,
     285,   284,   286,   299,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   296,     2,   297,     2,     2,     2,     2,     2,     2,
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
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283
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
     362,   365,   368,   373,   376,   379,   382,   385,   388,   391,
     394,   396,   397,   399,   401,   408,   415,   417,   419,   420,
     421,   423,   424,   429,   430,   432,   434,   437,   446,   448,
     449,   453,   455,   456,   458,   459,   461,   464,   466,   468,
     470,   473,   480,   482,   484,   488,   489,   491,   493,   494,
     496,   498,   501,   505,   510,   513,   516,   520,   521,   523,
     525,   528,   530,   535,   539,   543,   545,   546,   548,   550,
     553,   557,   562,   563,   565,   568,   569,   571,   573,   579,
     585,   589,   592,   593,   595,   599,   601,   603,   610,   614,
     617,   624,   627,   629,   632,   635,   638,   639,   641,   643,
     646,   647,   649,   651,   653,   655,   657,   659,   660,   662,
     664,   667,   670,   672,   674,   680,   683,   687,   690,   693,
     694,   696,   698,   700,   702,   706,   711,   720,   725,   726,
     728,   730,   733,   736,   737,   741,   745,   747,   750,   753,
     756,   765,   774,   783,   792,   796,   797,   802,   803,   805,
     809,   815,   817,   821,   824,   828,   832,   833,   835,   836,
     838,   841,   846,   848,   850,   854,   856,   857,   860,   863,
     864,   871,   873,   875,   879,   881,   884,   885,   887,   890,
     893,   895,   897,   899,   901,   903,   905,   907,   909,   911,
     913,   915,   917,   919,   921,   923,   925,   927,   929,   931,
     933,   935,   938,   942,   944,   948,   957,   967,   973,   982,
     989,   993,   994,   998,  1001,  1003,  1005,  1009,  1013,  1021,
    1024,  1025,  1027,  1029,  1032,  1036,  1037,  1039,  1042,  1047,
    1049,  1053,  1056,  1059,  1062,  1064,  1066,  1068,  1070,  1073,
    1076,  1082,  1083,  1089,  1091,  1095,  1096,  1099,  1104,  1105,
    1113,  1117,  1118,  1120,  1124,  1128,  1136,  1144,  1145,  1146,
    1147,  1148,  1152,  1153,  1162,  1171,  1180,  1182,  1184,  1185,
    1188,  1190,  1192,  1194,  1196,  1198,  1202,  1206,  1210,  1214,
    1218,  1222,  1228,  1234,  1240,  1246,  1252,  1258,  1261,  1262,
    1267,  1270,  1273,  1277,  1280,  1283,  1287,  1291,  1294,  1297,
    1301,  1303,  1306,  1310,  1314,  1317,  1320,  1323,  1326,  1330,
    1332,  1336,  1340,  1344,  1347,  1350,  1355,  1360,  1365,  1371,
    1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,  1394,
    1396,  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,
    1416,  1418,  1420,  1422,  1424,  1426,  1428,  1430,  1432,  1434,
    1436,  1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,  1454,
    1456,  1458,  1460,  1462,  1465,  1467,  1469,  1471,  1473,  1475,
    1476,  1479,  1482,  1487,  1492,  1498,  1502,  1505,  1506,  1509,
    1510,  1511,  1513,  1516,  1519,  1524,  1528,  1531,  1534,  1542,
    1544,  1545,  1547,  1548,  1551,  1554,  1557,  1560,  1563,  1566,
    1569,  1572,  1575,  1579,  1582,  1585,  1588,  1591,  1593,  1595,
    1597,  1599,  1604,  1610,  1612,  1616,  1618,  1622,  1624,  1627,
    1629,  1631,  1633,  1635,  1637,  1639,  1641,  1643,  1645,  1647,
    1649,  1654,  1659,  1666,  1672,  1676,  1677,  1680,  1683,  1684,
    1688,  1689,  1694,  1696,  1702,  1707,  1709,  1714,  1716,  1719,
    1722,  1724,  1726,  1728,  1731,  1734,  1737,  1740,  1741,  1745,
    1751,  1753,  1755,  1758,  1762,  1764,  1767,  1771,  1772,  1774,
    1776,  1778,  1784,  1790,  1797,  1799,  1801,  1803,  1806,  1811,
    1813,  1814,  1820,  1822,  1823,  1827,  1831,  1833,  1834,  1837,
    1838,  1840,  1841,  1845,  1847,  1848,  1850,  1853,  1855,  1857,
    1859,  1861,  1863,  1865,  1868,  1871,  1873,  1876,  1880,  1882,
    1884,  1888,  1892,  1894,  1897,  1901,  1903,  1906,  1907,  1911,
    1914,  1917,  1921,  1924,  1926,  1928,  1929,  1931,  1933,  1935,
    1939,  1942,  1946,  1947,  1949,  1953,  1958,  1964,  1971,  1979,
    1981,  1983,  1985,  1987,  1989,  1991,  1994,  1996,  1998,  2000,
    2002,  2004,  2006,  2009,  2011,  2013,  2015,  2018,  2020,  2022,
    2026,  2030,  2031,  2034,  2035,  2038,  2039,  2043,  2047,  2051,
    2053,  2058,  2063,  2065,  2075,  2076,  2077,  2078,  2079,  2083,
    2086,  2088,  2089,  2093,  2099,  2103,  2106,  2112,  2115,  2117,
    2119,  2121,  2123,  2125,  2129,  2131,  2135,  2137,  2138,  2141,
    2143,  2147,  2149,  2151,  2153,  2155,  2159,  2166,  2168,  2169,
    2173,  2174,  2176,  2180,  2182,  2184,  2186,  2191,  2197,  2203,
    2205,  2207,  2210,  2215,  2220,  2223,  2227,  2228,  2230,  2233,
    2235,  2237,  2240,  2243,  2246,  2247,  2249,  2250,  2254,  2255,
    2257,  2261,  2263,  2266,  2267,  2270,  2271,  2274,  2275,  2280,
    2282,  2285,  2287,  2289,  2290,  2292,  2296,  2299,  2301,  2303,
    2306,  2308,  2313,  2317,  2319,  2323,  2328,  2329,  2333,  2334,
    2336,  2340,  2344,  2346,  2348,  2349,  2351,  2353,  2358,  2359,
    2362,  2367,  2368,  2378,  2384,  2386,  2388,  2396,  2401,  2403,
    2405,  2414,  2420,  2423,  2428,  2429,  2434,  2436,  2440,  2444,
    2446,  2454,  2462,  2468,  2472,  2473,  2475,  2477,  2479,  2482,
    2483,  2485,  2487,  2489,  2490,  2494,  2496,  2500,  2502,  2503,
    2507,  2509,  2513,  2515,  2519,  2523,  2525,  2527,  2531,  2533,
    2536,  2538,  2542,  2546,  2548,  2551,  2555,  2559,  2563,  2567,
    2569,  2573,  2576,  2578,  2580,  2582,  2584,  2586,  2588,  2590,
    2592,  2594,  2596,  2598,  2602,  2606,  2610,  2614,  2618,  2622,
    2626,  2630,  2637,  2644,  2651,  2658,  2665,  2672,  2679,  2686,
    2693,  2700,  2707,  2714,  2721,  2728,  2735,  2742,  2744,  2746,
    2752,  2759,  2765,  2772,  2776,  2781,  2787,  2794,  2798,  2803,
    2807,  2812,  2816,  2821,  2826,  2832,  2837,  2842,  2846,  2851,
    2853,  2855,  2857,  2859,  2861,  2863,  2865,  2869,  2873,  2875,
    2877,  2879,  2881,  2883,  2885,  2887,  2889,  2891,  2893,  2896,
    2899,  2903,  2907,  2911,  2915,  2919,  2923,  2927,  2931,  2933,
    2935,  2937,  2939,  2943,  2945,  2947,  2949,  2951,  2954,  2957,
    2961,  2962,  2967,  2969,  2973,  2975,  2978,  2980,  2982,  2984,
    2986,  2988,  2990,  2992,  2995,  2998,  3001,  3003,  3005,  3007,
    3009,  3011,  3013,  3015,  3017,  3019,  3021,  3024,  3026,  3029,
    3031,  3033,  3035,  3037,  3039,  3042,  3044,  3046,  3048,  3050,
    3055,  3061,  3067,  3073,  3079,  3085,  3091,  3097,  3103,  3109,
    3115,  3117,  3119,  3126,  3128,  3130,  3132,  3137,  3142,  3147,
    3152,  3154,  3156,  3161,  3166,  3174,  3177,  3178,  3186,  3193,
    3200,  3205,  3207,  3209,  3211,  3216,  3220,  3227,  3229,  3231,
    3238,  3247,  3254,  3256,  3258,  3263,  3270,  3275,  3281,  3285,
    3291,  3296,  3302,  3304,  3306,  3308,  3313,  3320,  3322,  3324,
    3326,  3328,  3330,  3332,  3334,  3336,  3338,  3339,  3341,  3342,
    3344,  3346,  3348,  3350,  3352,  3354,  3356,  3358,  3360,  3362,
    3364,  3366,  3368,  3370,  3372,  3374,  3376,  3378,  3380,  3382,
    3384,  3386,  3388,  3390,  3392,  3394,  3396,  3398,  3400,  3402,
    3404,  3406,  3408,  3410,  3412,  3414,  3416,  3418,  3420,  3422,
    3424,  3426,  3428,  3430,  3432,  3434,  3436,  3438,  3440,  3442,
    3444,  3446,  3448,  3450,  3452,  3454,  3456,  3458,  3460,  3462,
    3464,  3466,  3468
};
static const short yyrhs[] =
{
     301,     0,   301,   291,     0,   486,     0,   648,     0,   566,
       0,   541,     0,   336,     0,   323,     0,   637,     0,   509,
       0,   302,     0,   636,     0,   463,     0,   466,     0,   338,
       0,   340,     0,   311,     0,   542,     0,   535,     0,   572,
       0,   533,     0,   640,     0,    40,   696,     0,    70,   304,
     122,   303,   612,   174,   314,   308,     0,    70,   306,   122,
     141,   310,   174,   314,   308,     0,    70,   319,   174,   321,
     309,     0,   172,     0,     0,     6,     0,     6,   140,     0,
     305,     0,   307,     0,   305,   292,   307,     0,    56,     0,
     155,     0,    80,     0,    44,     0,   180,   654,     0,   145,
     654,     0,   192,    70,   124,     0,     0,   192,   202,   124,
       0,     0,   749,     0,   150,   312,   304,   122,   303,   612,
      63,   314,     0,   150,   312,   306,   122,   141,   310,    63,
     314,     0,   150,   313,   319,    63,   321,     0,    70,   124,
      61,     0,     0,   202,   124,    61,     0,     0,   315,     0,
     317,     0,   316,     0,   315,   292,   316,     0,   315,   292,
     318,     0,   317,   292,   316,     0,   141,   749,     0,   176,
     753,     0,   187,   756,     0,   206,   750,     0,   318,     0,
     317,   292,   318,     0,   754,     0,   181,   754,     0,    71,
     754,     0,   320,     0,   319,   292,   320,     0,   750,     0,
     322,     0,   321,   292,   322,     0,   754,     0,   181,   754,
       0,    42,   324,     0,    60,   334,     0,    59,    65,   325,
       0,   733,   327,   149,   331,    52,   695,   111,   695,     0,
     516,     0,    18,     0,    34,   293,   699,   294,   522,     0,
       0,   328,     0,   293,   328,   294,     0,   329,     0,   328,
     292,   329,     0,   395,   326,   330,     0,     0,    19,   230,
       0,    19,   262,     0,    94,     0,   332,     0,   293,   332,
     294,     0,   395,   326,   333,     0,   133,   699,     0,     0,
      19,    97,     0,    19,   230,     0,   204,     0,    19,   230,
     204,     0,   742,    78,   335,   128,   335,    52,   695,   111,
     695,     0,   734,     0,   696,     0,    33,   337,     0,    55,
     342,     0,   346,   632,    79,   745,   122,   612,   347,     0,
     141,   412,     0,   172,   382,     0,   176,   477,     0,   187,
     470,     0,    68,   364,     0,   265,   364,     0,    37,   366,
       0,    48,   354,     0,   157,   348,     0,   206,   365,     0,
     229,   339,     0,   141,   413,     0,   172,   383,     0,   187,
     471,     0,   176,   478,     0,    55,   343,     0,    33,   125,
       7,   341,     0,   141,   414,     0,   176,   479,     0,    55,
     344,     0,   741,   695,     0,   741,   695,     0,   741,   695,
       0,   741,   695,     0,   179,     0,     0,   656,     0,   655,
       0,   389,   293,   473,   683,   474,   294,     0,   699,   349,
     350,   695,   351,   352,     0,   105,     0,    14,     0,     0,
       0,    29,     0,     0,    98,   367,   702,   381,     0,     0,
     353,     0,   375,     0,   353,   375,     0,   392,   355,   511,
     473,   357,   475,   358,   391,     0,    10,     0,     0,    43,
     473,   396,     0,   356,     0,     0,   359,     0,     0,   360,
       0,   359,   360,     0,   361,     0,   362,     0,   363,     0,
     118,    94,     0,    22,   473,   293,   663,   294,   474,     0,
     744,     0,   750,     0,   368,   369,   372,     0,     0,   284,
       0,   695,     0,     0,   370,     0,   371,     0,   370,   371,
       0,   132,   367,   699,     0,    98,   367,   702,   381,     0,
     181,   695,     0,   134,   695,     0,   156,   112,   695,     0,
       0,   373,     0,   374,     0,   373,   374,     0,   375,     0,
      43,    21,   156,   735,     0,   255,    90,   695,     0,   376,
     695,   377,     0,    90,     0,     0,   378,     0,   379,     0,
     378,   379,     0,   167,   380,   702,     0,    98,   367,   702,
     381,     0,     0,    12,     0,    12,   130,     0,     0,   130,
       0,   131,     0,   612,   384,   293,   385,   294,     0,   612,
     384,   293,   385,   294,     0,    59,    90,   695,     0,    59,
     695,     0,     0,   386,     0,   385,   292,   386,     0,   387,
       0,   401,     0,   392,   390,   357,   475,   397,   391,     0,
     392,   512,   388,     0,   392,   388,     0,   389,   293,   473,
     683,   474,   294,     0,    27,    19,     0,    27,     0,   511,
     473,     0,   661,   472,     0,    23,   736,     0,     0,   661,
       0,   661,     0,   395,   511,     0,     0,   688,     0,   692,
       0,   693,     0,   694,     0,   732,     0,   684,     0,     0,
     398,     0,   399,     0,   398,   399,     0,   402,   400,     0,
     362,     0,   363,     0,   145,   612,   654,   408,   407,     0,
     179,   407,     0,   139,    86,   407,     0,   402,   403,     0,
      30,   738,     0,     0,   404,     0,   405,     0,   406,     0,
     363,     0,   179,   655,   407,     0,   139,    86,   655,   407,
       0,    62,    86,   655,   145,   612,   654,   408,   407,     0,
     239,   632,    79,   745,     0,     0,   409,     0,   410,     0,
     410,   409,     0,   409,   410,     0,     0,   122,   180,   411,
       0,   122,    44,   411,     0,   203,     0,   156,    43,     0,
     156,    94,     0,   117,   201,     0,   749,   416,   417,    10,
     472,   423,   432,   474,     0,   749,   416,   417,    10,   472,
     423,   432,   474,     0,   749,   416,   417,    10,   472,   423,
     432,   474,     0,   749,   416,   417,    10,   472,   423,   432,
     474,     0,   293,   418,   294,     0,     0,   149,   293,   420,
     294,     0,     0,   419,     0,   418,   292,   419,     0,   393,
     512,   473,   422,   474,     0,   421,     0,   420,   292,   421,
       0,   393,   512,     0,    43,   473,   396,     0,   284,   473,
     396,     0,     0,   424,     0,     0,   425,     0,   424,   425,
       0,    42,   428,   426,   291,     0,   427,     0,   430,     0,
     392,   512,   429,     0,   184,     0,     0,    43,   396,     0,
     284,   396,     0,     0,   739,    36,    61,   293,   572,   294,
       0,   435,     0,   432,     0,    16,   433,    51,     0,   434,
       0,   434,   454,     0,     0,   431,     0,   434,   431,     0,
     436,   291,     0,   437,     0,   646,     0,   636,     0,   640,
       0,   637,     0,   447,     0,   463,     0,   440,     0,   443,
       0,   647,     0,   438,     0,   439,     0,   445,     0,   458,
       0,   452,     0,   170,     0,    58,     0,   444,     0,   450,
       0,   441,     0,   442,     0,    55,   741,     0,    55,   741,
     683,     0,    55,     0,    56,   246,   683,     0,   451,    61,
     572,    82,   449,   453,    47,   431,     0,   451,    61,    56,
     246,   683,    82,   449,    47,   431,     0,    56,   246,   683,
      82,   449,     0,    74,   293,   663,   294,   173,   431,    50,
     431,     0,    74,   293,   663,   294,   173,   431,     0,   137,
     683,   446,     0,     0,   572,    82,   449,     0,   295,   755,
       0,   448,     0,   660,     0,   449,   292,   660,     0,   449,
     292,   448,     0,   451,   191,   293,   663,   294,    47,   431,
       0,   747,   295,     0,     0,   227,     0,   247,     0,   247,
     747,     0,    10,    36,   739,     0,     0,   455,     0,   454,
     455,     0,   189,   456,    47,   431,     0,   457,     0,   456,
     292,   457,     0,   165,   696,     0,    66,   743,     0,    55,
     741,     0,     9,     0,   459,     0,   461,     0,   460,     0,
     256,   739,     0,   257,   739,     0,   258,   462,   739,    82,
     449,     0,     0,    56,   141,   749,   464,   465,     0,   687,
       0,   293,   687,   294,     0,     0,   148,   449,     0,   148,
     293,   449,   294,     0,     0,    56,   260,   467,   417,    10,
     423,   432,     0,   293,   468,   294,     0,     0,   469,     0,
     468,   292,   469,     0,   421,   284,   691,     0,   756,   654,
      10,   472,   576,   476,   474,     0,   756,   654,    10,   472,
     576,   476,   474,     0,     0,     0,     0,     0,   192,    22,
     124,     0,     0,   753,    61,   612,   480,   481,   484,   485,
     474,     0,   753,    61,   612,   480,   481,   484,   485,   474,
       0,   753,    61,   612,   480,   481,   484,   485,   474,     0,
       3,     0,    76,     0,     0,   482,   483,     0,    15,     0,
       5,     0,    80,     0,   180,     0,    44,     0,    80,   125,
     180,     0,    80,   125,    44,     0,   180,   125,    80,     0,
     180,   125,    44,     0,    44,   125,    80,     0,    44,   125,
     180,     0,    80,   125,   180,   125,    44,     0,    80,   125,
      44,   125,   180,     0,   180,   125,    80,   125,    44,     0,
     180,   125,    44,   125,    80,     0,    44,   125,    80,   125,
     180,     0,    44,   125,   180,   125,    80,     0,   136,   697,
       0,     0,    10,   473,   423,   432,     0,     7,   487,     0,
      55,   345,     0,   172,   612,   490,     0,   176,   506,     0,
     141,   415,     0,    37,   503,   504,     0,    48,   492,   488,
       0,    79,   498,     0,   265,   499,     0,    59,    65,   500,
       0,   489,     0,   488,   489,     0,   156,   356,   474,     0,
       4,    30,   363,     0,     4,   363,     0,    49,    43,     0,
      49,    30,     0,   174,   661,     0,   208,   395,   512,     0,
     491,     0,   490,   292,   491,     0,    49,   661,   497,     0,
      49,    30,   738,     0,     4,   387,     0,     4,   401,     0,
     494,   661,   136,   699,     0,   494,   492,   174,   661,     0,
     494,   496,   208,   495,     0,   494,   496,   156,   356,   474,
       0,   494,   496,    49,    43,     0,   493,     0,   758,     0,
     202,     0,   207,     0,   209,     0,   210,     0,   211,     0,
     212,     0,   213,     0,   214,     0,   215,     0,   218,     0,
     219,     0,   220,     0,   221,     0,   222,     0,   225,     0,
     226,     0,   229,     0,   233,     0,   234,     0,   235,     0,
     236,     0,   245,     0,   242,     0,   244,     0,   256,     0,
     257,     0,   258,     0,   259,     0,   239,     0,   263,     0,
     270,     0,   267,     0,   271,     0,   272,     0,   269,     0,
     273,     0,   274,     0,   275,     0,   276,     0,   277,     0,
       7,     0,     7,   207,     0,   512,     0,   661,     0,   661,
       0,   205,     0,   203,     0,     0,   745,     3,     0,   745,
      76,     0,   744,   266,   192,   701,     0,   744,   266,   192,
     223,     0,   744,   266,   192,   288,   223,     0,   733,   501,
     502,     0,    52,   695,     0,     0,   111,   695,     0,     0,
       0,   505,     0,   504,   505,     0,     4,   353,     0,     4,
     255,    90,   695,     0,    49,   255,    90,     0,    16,   254,
       0,    51,   254,     0,   753,   480,   507,   484,   473,   508,
     474,     0,   481,     0,     0,   485,     0,     0,    49,   510,
       0,    55,   741,     0,    79,   745,     0,   141,   749,     0,
     172,   752,     0,   176,   753,     0,   187,   756,     0,    60,
     742,     0,    48,   740,     0,    59,    65,   733,     0,   157,
     699,     0,   206,   750,     0,    68,   744,     0,   265,   744,
       0,   512,     0,   513,     0,   516,     0,   519,     0,   517,
     296,   514,   297,     0,   524,   296,   514,   297,   522,     0,
     515,     0,   514,   292,   515,     0,   701,     0,   701,   295,
     701,     0,   517,     0,   524,   522,     0,   523,     0,   528,
       0,   531,     0,   235,     0,   518,     0,   161,     0,    38,
       0,   218,     0,   219,     0,    81,     0,    92,     0,    18,
     521,   520,   522,     0,    18,   293,   700,   294,     0,    18,
     293,   700,   292,   696,   294,     0,    18,   293,   292,   696,
     294,     0,   154,   160,   700,     0,     0,   169,   696,     0,
     169,   734,     0,     0,    21,   156,   735,     0,     0,   527,
     293,   699,   294,     0,   527,     0,   527,   185,   293,   699,
     294,     0,   526,   293,   699,   294,     0,   526,     0,   525,
     293,   699,   294,     0,   183,     0,    21,   185,     0,    87,
     185,     0,    21,     0,    87,     0,   115,     0,   113,    21,
       0,   113,    87,     0,    95,   529,     0,   530,   529,     0,
       0,   293,   701,   294,     0,   293,   701,   292,   701,   294,
       0,    41,     0,    88,     0,    91,   532,     0,    93,    91,
     532,     0,   144,     0,    89,   138,     0,   293,   697,   294,
       0,     0,   546,     0,   534,     0,   565,     0,   156,    68,
     744,   174,   701,     0,   156,    68,   744,   174,   223,     0,
     156,    68,   744,   174,   288,   223,     0,   536,     0,   537,
       0,   539,     0,   244,   757,     0,   245,   244,   757,   538,
       0,   123,     0,     0,   153,   543,   174,   540,   757,     0,
     244,     0,     0,    25,   543,   544,     0,   153,   543,   544,
       0,   193,     0,     0,   147,   545,     0,     0,   162,     0,
       0,   156,   175,   547,     0,   548,     0,     0,   549,     0,
     548,   549,     0,   550,     0,   551,     0,   552,     0,   556,
       0,   557,     0,   558,     0,   143,   123,     0,   143,   194,
       0,   188,     0,   117,   188,     0,    84,   102,   553,     0,
     553,     0,   554,     0,   143,   177,   555,     0,   143,    26,
     555,     0,   162,     0,   162,   172,     0,   162,   172,   166,
       0,   186,     0,   117,   186,     0,     0,   117,    14,   281,
       0,   279,   280,     0,   266,   282,     0,   243,   283,   697,
       0,   146,   561,     0,   158,     0,   142,     0,     0,   143,
       0,   194,     0,   562,     0,   561,   292,   562,     0,   564,
     563,     0,    61,   559,   560,     0,     0,   612,     0,   564,
     292,   612,     0,   156,   168,    79,   745,     0,   269,   122,
     567,    83,   571,     0,   269,   122,   568,   748,    83,   571,
       0,   269,   122,   569,   748,   570,    83,   571,     0,    37,
       0,    48,     0,   172,     0,   187,     0,   141,     0,   176,
       0,    59,    65,     0,    60,     0,    55,     0,    68,     0,
     265,     0,    79,     0,   206,     0,    21,   156,     0,   268,
       0,   207,     0,   133,     0,   298,   748,     0,   695,     0,
      94,     0,   576,   573,   575,     0,    61,   180,   574,     0,
       0,   121,   656,     0,     0,   192,   243,     0,     0,   579,
     629,   635,     0,   579,   629,   635,     0,   579,   629,   635,
       0,   580,     0,   579,   178,   731,   580,     0,   579,   178,
       6,   580,     0,   581,     0,   155,   586,   589,   590,   594,
     619,   615,   618,   620,     0,     0,     0,     0,     0,   587,
     588,   583,     0,   587,   583,     0,   588,     0,     0,   231,
     702,   582,     0,   231,   293,   683,   294,   582,     0,   231,
     691,   582,     0,   232,   702,     0,   232,   293,   583,   683,
     294,     0,   232,   691,     0,    46,     0,   730,     0,   591,
       0,   289,     0,   592,     0,   591,   292,   592,     0,   683,
       0,   683,   593,   746,     0,    10,     0,     0,    63,   595,
       0,   596,     0,   595,   292,   596,     0,   602,     0,   597,
       0,   609,     0,   598,     0,   293,   602,   294,     0,   293,
     576,   294,   593,   599,   600,     0,   751,     0,     0,   293,
     601,   294,     0,     0,   746,     0,   601,   292,   746,     0,
     603,     0,   604,     0,   605,     0,   596,   263,    85,   597,
       0,   596,   114,   613,    85,   597,     0,   596,   613,    85,
     596,   606,     0,   607,     0,   608,     0,   122,   663,     0,
     239,   293,   656,   294,     0,   749,   610,   593,   751,     0,
     749,   610,     0,   293,   687,   294,     0,     0,   612,     0,
     752,   751,     0,   752,     0,    77,     0,   100,   614,     0,
     151,   614,     0,    64,   614,     0,     0,   127,     0,     0,
      71,    19,   616,     0,     0,   617,     0,   616,   292,   617,
       0,   683,     0,    73,   663,     0,     0,   190,   663,     0,
       0,   135,   621,     0,     0,   622,   293,   623,   294,     0,
      85,     0,   164,   108,     0,   108,     0,   164,     0,     0,
     624,     0,   624,   292,   623,     0,   625,   626,     0,   621,
       0,   752,     0,   752,   625,     0,   114,     0,    79,   293,
     627,   294,     0,   126,   745,   628,     0,   745,     0,   745,
     292,   627,     0,    79,   293,   627,   294,     0,     0,   126,
      19,   630,     0,     0,   631,     0,   630,   292,   631,     0,
     683,   632,   634,     0,    11,     0,    45,     0,     0,   231,
       0,   241,     0,   240,   584,   633,   585,     0,     0,   259,
     683,     0,   259,   683,   174,   683,     0,     0,    80,    82,
     612,   657,   182,   293,   687,   294,   643,     0,    80,    82,
     612,   657,   576,     0,   638,     0,   639,     0,    44,    63,
     611,   619,   620,   629,   635,     0,    44,    63,   611,   644,
       0,   641,     0,   642,     0,   180,   611,   156,   645,   619,
     620,   629,   635,     0,   180,   611,   156,   645,   644,     0,
     278,   687,     0,   278,   687,    82,   449,     0,     0,   190,
      35,   121,   739,     0,   646,     0,   645,   292,   646,     0,
     662,   284,   683,     0,   716,     0,   143,    18,   661,    63,
     612,   649,   652,     0,    80,    18,   661,    82,   612,   649,
     652,     0,    60,    63,   650,   174,   650,     0,    60,   174,
     650,     0,     0,   651,     0,   691,     0,   696,     0,   107,
     653,     0,     0,   700,     0,   691,     0,   655,     0,     0,
     293,   656,   294,     0,   661,     0,   656,   292,   661,     0,
     658,     0,     0,   293,   659,   294,     0,   662,     0,   659,
     292,   662,     0,   661,     0,   751,   298,   737,     0,   751,
     298,   289,     0,   737,     0,   661,     0,   751,   298,   737,
       0,   679,     0,   118,   679,     0,   665,     0,   663,   125,
     663,     0,   663,     8,   663,     0,   665,     0,   118,   679,
       0,   664,   125,   663,     0,   664,     8,   663,     0,   680,
     125,   663,     0,   680,     8,   663,     0,   666,     0,   293,
     664,   294,     0,   118,   665,     0,   667,     0,   670,     0,
     671,     0,   672,     0,   673,     0,   678,     0,   668,     0,
     676,     0,   674,     0,   675,     0,   677,     0,   683,   284,
     683,     0,   683,   285,   683,     0,   683,   286,   683,     0,
     683,    67,   683,     0,   683,   101,   683,     0,   683,   119,
     683,     0,   683,   120,   683,     0,   683,   116,   683,     0,
     683,   284,     6,   293,   577,   294,     0,   683,   285,     6,
     293,   577,   294,     0,   683,   286,     6,   293,   577,   294,
       0,   683,    67,     6,   293,   577,   294,     0,   683,   101,
       6,   293,   577,   294,     0,   683,   119,     6,   293,   577,
     294,     0,   683,   120,     6,   293,   577,   294,     0,   683,
     116,     6,   293,   577,   294,     0,   683,   284,   669,   293,
     577,   294,     0,   683,   285,   669,   293,   577,   294,     0,
     683,   286,   669,   293,   577,   294,     0,   683,    67,   669,
     293,   577,   294,     0,   683,   101,   669,   293,   577,   294,
       0,   683,   119,   669,   293,   577,   294,     0,   683,   120,
     669,   293,   577,   294,     0,   683,   116,   669,   293,   577,
     294,     0,   163,     0,     9,     0,   683,    83,    46,    63,
     683,     0,   683,    83,   118,    46,    63,   683,     0,   683,
      17,   683,     8,   683,     0,   683,   118,    17,   683,     8,
     683,     0,   683,   103,   683,     0,   683,   118,   103,   683,
       0,   683,   103,   683,    54,   683,     0,   683,   118,   103,
     683,    54,   683,     0,   683,    75,   681,     0,   683,   118,
      75,   681,     0,   683,    31,   683,     0,   683,   118,    31,
     683,     0,   683,   167,   683,     0,   683,   118,   167,   683,
       0,   683,   167,   192,   683,     0,   683,   118,   167,   192,
     683,     0,    57,   293,   576,   294,     0,   159,   293,   576,
     294,     0,   683,    83,    94,     0,   683,    83,   118,    94,
       0,   248,     0,   249,     0,   250,     0,   251,     0,   252,
       0,   253,     0,   682,     0,   293,   687,   294,     0,   293,
     577,   294,     0,   660,     0,   686,     0,   703,     0,   690,
       0,   691,     0,   448,     0,   717,     0,   718,     0,   728,
       0,   716,     0,   288,   683,     0,   287,   683,     0,   683,
     287,   683,     0,   683,    28,   683,     0,   683,    23,   736,
       0,   683,   288,   683,     0,   683,   289,   683,     0,   683,
     290,   683,     0,   293,   683,   294,     0,   293,   578,   294,
       0,   692,     0,   693,     0,   694,     0,    39,     0,   751,
     298,    39,     0,    97,     0,   684,     0,   732,     0,   220,
       0,   221,   685,     0,   222,   685,     0,   293,   697,   294,
       0,     0,   660,   296,   687,   297,     0,   683,     0,   687,
     292,   683,     0,   690,     0,   288,   689,     0,   197,     0,
     196,     0,   195,     0,   223,     0,   224,     0,   689,     0,
     695,     0,    38,   199,     0,   218,   199,     0,   219,   199,
       0,   299,     0,   181,     0,   225,     0,   226,     0,   233,
       0,   234,     0,    66,     0,   165,     0,   242,     0,   199,
       0,   200,   199,     0,   697,     0,   288,   698,     0,   196,
       0,   196,     0,   697,     0,   196,     0,   702,     0,   288,
     702,     0,   196,     0,   704,     0,   705,     0,   711,     0,
      32,   293,   289,   294,     0,    32,   293,   730,   683,   294,
       0,    32,   293,    46,   683,   294,     0,   171,   293,   730,
     683,   294,     0,   171,   293,    46,   683,   294,     0,    13,
     293,   730,   683,   294,     0,    13,   293,    46,   683,   294,
       0,   110,   293,   730,   683,   294,     0,   110,   293,    46,
     683,   294,     0,   106,   293,   730,   683,   294,     0,   106,
     293,    46,   683,   294,     0,   706,     0,   707,     0,   209,
     293,   729,    63,   683,   294,     0,   708,     0,   709,     0,
     710,     0,   270,   293,   683,   294,     0,   271,   293,   683,
     294,     0,   272,   293,   683,   294,     0,   275,   293,   683,
     294,     0,   712,     0,   714,     0,    96,   293,   683,   294,
       0,   274,   293,   683,   294,     0,   228,   293,   683,    63,
     683,   713,   294,     0,    61,   683,     0,     0,   277,   293,
     715,   683,    63,   683,   294,     0,   277,   293,   683,    63,
     683,   294,     0,   277,   293,   715,    63,   683,   294,     0,
     277,   293,   683,   294,     0,   267,     0,   276,     0,   273,
       0,   733,   293,   687,   294,     0,   733,   293,   294,     0,
      20,   293,   683,    10,   394,   294,     0,   719,     0,   720,
       0,   237,   293,   683,   292,   683,   294,     0,   261,   293,
     663,   292,   683,   292,   683,   294,     0,   238,   293,   683,
     292,   687,   294,     0,   721,     0,   723,     0,   236,   726,
     722,    51,     0,   236,   726,   722,    50,   727,    51,     0,
     189,   725,   173,   727,     0,   722,   189,   725,   173,   727,
       0,   236,   724,    51,     0,   236,   724,    50,   727,    51,
       0,   189,   663,   173,   727,     0,   724,   189,   663,   173,
     727,     0,   683,     0,   683,     0,   683,     0,   264,    97,
      61,   744,     0,    69,   293,   744,   292,   683,   294,     0,
     210,     0,   211,     0,   212,     0,   213,     0,   214,     0,
     215,     0,   216,     0,   217,     0,     6,     0,     0,    46,
       0,     0,    94,     0,   198,     0,   758,     0,   758,     0,
     758,     0,   758,     0,   758,     0,   758,     0,   758,     0,
     758,     0,   758,     0,   758,     0,   758,     0,   758,     0,
     758,     0,   758,     0,   758,     0,   758,     0,   758,     0,
     758,     0,   758,     0,   758,     0,   758,     0,   758,     0,
     758,     0,   758,     0,   198,     0,   759,     0,   201,     0,
     203,     0,   204,     0,   205,     0,   206,     0,   208,     0,
     227,     0,   230,     0,   228,     0,   238,     0,   241,     0,
     247,     0,   243,     0,   237,     0,   240,     0,   246,     0,
     248,     0,   249,     0,   250,     0,   260,     0,   254,     0,
     255,     0,   261,     0,   262,     0,   216,     0,   217,     0,
     265,     0,   264,     0,   266,     0,   268,     0,   278,     0,
     279,     0,   280,     0,   281,     0,   282,     0,   283,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   556,   558,   562,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   590,   594,   598,   603,   604,   607,
     609,   611,   615,   616,   620,   624,   626,   628,   630,   632,
     636,   638,   642,   644,   648,   655,   659,   663,   668,   670,
     674,   676,   680,   681,   684,   685,   687,   689,   693,   695,
     697,   699,   703,   704,   711,   713,   715,   719,   720,   724,
     728,   729,   733,   735,   742,   746,   748,   753,   759,   760,
     762,   768,   770,   771,   775,   776,   782,   787,   789,   791,
     793,   797,   798,   802,   805,   810,   812,   814,   816,   819,
     824,   830,   832,   839,   843,   845,   848,   850,   852,   854,
     856,   858,   860,   862,   864,   866,   875,   879,   881,   883,
     885,   891,   895,   899,   901,   907,   914,   919,   924,   929,
     937,   939,   943,   945,   946,   952,   958,   960,   962,   966,
     968,   972,   974,   978,   980,   983,   984,   991,  1009,  1011,
    1015,  1019,  1020,  1024,  1025,  1029,  1030,  1034,  1038,  1039,
    1042,  1046,  1054,  1061,  1126,  1131,  1132,  1135,  1139,  1141,
    1144,  1145,  1149,  1151,  1153,  1155,  1157,  1161,  1163,  1166,
    1167,  1171,  1172,  1174,  1178,  1184,  1188,  1189,  1192,  1193,
    1196,  1198,  1202,  1203,  1204,  1207,  1208,  1209,  1215,  1220,
    1225,  1227,  1229,  1233,  1234,  1238,  1239,  1246,  1250,  1254,
    1263,  1269,  1270,  1273,  1275,  1280,  1282,  1287,  1293,  1299,
    1303,  1309,  1310,  1311,  1312,  1313,  1314,  1317,  1319,  1322,
    1323,  1327,  1332,  1333,  1334,  1339,  1341,  1349,  1353,  1355,
    1359,  1360,  1361,  1362,  1365,  1369,  1373,  1380,  1387,  1392,
    1395,  1397,  1399,  1401,  1405,  1408,  1412,  1415,  1418,  1421,
    1430,  1441,  1451,  1461,  1471,  1473,  1477,  1479,  1483,  1484,
    1488,  1494,  1495,  1499,  1504,  1506,  1508,  1512,  1514,  1518,
    1519,  1523,  1527,  1528,  1531,  1536,  1538,  1542,  1544,  1546,
    1550,  1555,  1556,  1559,  1563,  1565,  1567,  1571,  1572,  1576,
    1577,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1591,  1592,  1593,  1594,  1596,  1600,  1601,  1602,
    1603,  1606,  1608,  1612,  1616,  1620,  1625,  1629,  1633,  1635,
    1639,  1643,  1649,  1654,  1659,  1660,  1661,  1663,  1667,  1671,
    1673,  1677,  1679,  1681,  1686,  1689,  1693,  1694,  1698,  1703,
    1704,  1708,  1710,  1712,  1714,  1718,  1719,  1720,  1723,  1727,
    1731,  1735,  1778,  1783,  1785,  1787,  1791,  1793,  1795,  1801,
    1809,  1811,  1815,  1816,  1820,  1826,  1833,  1856,  1866,  1870,
    1875,  1882,  1885,  1893,  1903,  1913,  1923,  1925,  1927,  1931,
    1935,  1937,  1941,  1943,  1945,  1947,  1949,  1951,  1953,  1955,
    1957,  1959,  1961,  1963,  1965,  1967,  1969,  1973,  1975,  1979,
    1985,  1989,  1991,  1998,  2000,  2002,  2005,  2008,  2010,  2012,
    2016,  2017,  2021,  2023,  2025,  2027,  2029,  2031,  2033,  2037,
    2038,  2042,  2044,  2046,  2048,  2054,  2057,  2059,  2061,  2064,
    2069,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2085,
    2086,  2087,  2088,  2089,  2090,  2091,  2092,  2093,  2094,  2095,
    2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,  2104,  2105,
    2106,  2107,  2108,  2109,  2110,  2111,  2112,  2114,  2115,  2116,
    2118,  2119,  2122,  2124,  2128,  2130,  2135,  2141,  2143,  2145,
    2149,  2151,  2155,  2158,  2161,  2166,  2170,  2172,  2176,  2178,
    2185,  2189,  2190,  2194,  2196,  2198,  2200,  2202,  2209,  2219,
    2220,  2224,  2225,  2231,  2235,  2237,  2239,  2241,  2243,  2245,
    2247,  2249,  2251,  2253,  2255,  2257,  2259,  2266,  2267,  2270,
    2271,  2274,  2279,  2286,  2287,  2291,  2300,  2306,  2307,  2310,
    2311,  2312,  2313,  2330,  2335,  2340,  2359,  2376,  2383,  2384,
    2391,  2396,  2403,  2410,  2419,  2423,  2429,  2433,  2437,  2443,
    2447,  2454,  2460,  2466,  2474,  2479,  2484,  2491,  2492,  2493,
    2496,  2497,  2500,  2501,  2502,  2509,  2513,  2524,  2530,  2584,
    2646,  2647,  2654,  2667,  2672,  2677,  2684,  2686,  2693,  2694,
    2695,  2699,  2702,  2705,  2713,  2714,  2715,  2718,  2722,  2726,
    2728,  2732,  2736,  2737,  2740,  2744,  2748,  2749,  2752,  2754,
    2758,  2759,  2763,  2767,  2768,  2772,  2773,  2777,  2778,  2779,
    2780,  2781,  2782,  2785,  2787,  2791,  2793,  2797,  2799,  2802,
    2804,  2806,  2810,  2812,  2814,  2818,  2820,  2822,  2826,  2828,
    2830,  2834,  2838,  2842,  2844,  2846,  2850,  2852,  2856,  2857,
    2861,  2865,  2867,  2871,  2872,  2877,  2881,  2883,  2885,  2889,
    2893,  2895,  2897,  2899,  2901,  2903,  2905,  2907,  2909,  2911,
    2913,  2915,  2917,  2919,  2927,  2929,  2933,  2937,  2938,  2945,
    2949,  2951,  2955,  2957,  2961,  2963,  2970,  2974,  2979,  2984,
    2985,  2987,  2991,  2994,  3006,  3010,  3014,  3018,  3022,  3024,
    3026,  3028,  3032,  3034,  3036,  3040,  3042,  3044,  3048,  3050,
    3054,  3056,  3060,  3061,  3065,  3066,  3070,  3071,  3076,  3080,
    3081,  3085,  3086,  3089,  3090,  3091,  3096,  3101,  3102,  3106,
    3108,  3112,  3113,  3117,  3118,  3119,  3122,  3127,  3132,  3136,
    3137,  3140,  3144,  3148,  3151,  3156,  3158,  3162,  3163,  3168,
    3173,  3175,  3177,  3179,  3181,  3185,  3186,  3192,  3194,  3198,
    3199,  3205,  3208,  3210,  3214,  3216,  3223,  3225,  3229,  3233,
    3235,  3237,  3243,  3245,  3249,  3250,  3254,  3256,  3259,  3260,
    3264,  3266,  3268,  3272,  3273,  3277,  3279,  3285,  3287,  3291,
    3292,  3296,  3300,  3302,  3304,  3308,  3310,  3314,  3316,  3322,
    3325,  3333,  3340,  3344,  3352,  3353,  3356,  3362,  3370,  3371,
    3374,  3380,  3386,  3389,  3392,  3396,  3403,  3404,  3408,  3412,
    3419,  3421,  3425,  3427,  3429,  3433,  3434,  3437,  3441,  3443,
    3447,  3449,  3455,  3456,  3460,  3464,  3465,  3470,  3471,  3475,
    3479,  3480,  3485,  3486,  3489,  3494,  3499,  3502,  3509,  3510,
    3512,  3513,  3515,  3519,  3520,  3522,  3524,  3528,  3530,  3534,
    3535,  3537,  3541,  3542,  3543,  3544,  3545,  3546,  3547,  3548,
    3549,  3550,  3551,  3556,  3558,  3560,  3562,  3564,  3566,  3568,
    3570,  3576,  3578,  3580,  3582,  3584,  3586,  3588,  3590,  3592,
    3594,  3596,  3598,  3600,  3602,  3604,  3606,  3610,  3611,  3617,
    3619,  3623,  3625,  3630,  3632,  3634,  3636,  3641,  3643,  3647,
    3649,  3653,  3655,  3657,  3659,  3663,  3667,  3671,  3673,  3677,
    3682,  3687,  3694,  3699,  3704,  3713,  3714,  3718,  3725,  3726,
    3727,  3728,  3729,  3730,  3731,  3732,  3733,  3734,  3735,  3737,
    3739,  3746,  3748,  3750,  3757,  3764,  3771,  3773,  3775,  3776,
    3777,  3778,  3780,  3782,  3786,  3787,  3790,  3806,  3822,  3826,
    3828,  3832,  3836,  3837,  3841,  3842,  3846,  3848,  3850,  3852,
    3854,  3858,  3859,  3861,  3877,  3893,  3897,  3901,  3903,  3907,
    3911,  3914,  3917,  3920,  3923,  3928,  3930,  3935,  3936,  3940,
    3947,  3954,  3961,  3968,  3969,  3973,  3980,  3981,  3982,  3985,
    3987,  3989,  3992,  3999,  4008,  4015,  4024,  4026,  4028,  4030,
    4034,  4035,  4038,  4042,  4043,  4044,  4047,  4052,  4055,  4060,
    4065,  4066,  4067,  4069,  4073,  4082,  4084,  4088,  4090,  4093,
    4095,  4100,  4102,  4104,  4108,  4110,  4114,  4120,  4121,  4124,
    4128,  4131,  4135,  4136,  4139,  4141,  4145,  4147,  4151,  4153,
    4157,  4159,  4163,  4166,  4169,  4174,  4183,  4193,  4195,  4197,
    4199,  4201,  4203,  4205,  4207,  4211,  4212,  4215,  4216,  4219,
    4227,  4230,  4233,  4236,  4239,  4242,  4245,  4248,  4251,  4254,
    4257,  4260,  4263,  4266,  4269,  4272,  4275,  4278,  4281,  4284,
    4287,  4290,  4293,  4296,  4299,  4304,  4305,  4310,  4312,  4313,
    4314,  4315,  4316,  4317,  4318,  4319,  4320,  4321,  4322,  4323,
    4324,  4325,  4326,  4327,  4328,  4329,  4331,  4333,  4334,  4335,
    4337,  4339,  4340,  4341,  4342,  4343,  4344,  4345,  4346,  4347,
    4348,  4349,  4350
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
  "CROSS", "NEXT", "SEQUENCE", "RESTART", "BOTH", "COLLATION", "COMMENT", 
  "BIT_LENGTH", "CHAR_LENGTH", "CHARACTER_LENGTH", "LEADING", "KW_LOWER", 
  "OCTET_LENGTH", "TRAILING", "TRIM", "RETURNING", "KW_IGNORE", "LIMBO", 
  "UNDO", "REQUESTS", "TIMEOUT", "'='", "'<'", "'>'", "'+'", "'-'", "'*'", 
  "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", "'.'", "'?'", 
  "top", "statement", "grant", "table_noise", "privileges", 
  "privilege_list", "proc_privileges", "privilege", "grant_option", 
  "role_admin_option", "simple_proc_name", "revoke", "rev_grant_option", 
  "rev_admin_option", "non_role_grantee_list", "grantee_list", "grantee", 
  "user_grantee_list", "user_grantee", "role_name_list", "role_name", 
  "role_grantee_list", "role_grantee", "declare", "declare_clause", 
  "udf_decl_clause", "udf_data_type", "arg_desc_list1", "arg_desc_list", 
  "arg_desc", "param_mechanism", "return_value1", "return_value", 
  "return_mechanism", "filter_decl_clause", "blob_filter_subtype", 
  "create", "create_clause", "recreate", "recreate_clause", "replace", 
  "replace_clause", "exception_clause", "rexception_clause", 
  "replace_exception_clause", "alter_exception_clause", "unique_opt", 
  "index_definition", "shadow_clause", "manual_auto", "conditional", 
  "first_file_length", "sec_shadow_files", "db_file_list", 
  "domain_clause", "as_opt", "domain_default", "domain_default_opt", 
  "domain_constraint_clause", "domain_constraint_list", 
  "domain_constraint_def", "domain_constraint", "null_constraint", 
  "check_constraint", "generator_clause", "role_clause", "db_clause", 
  "equals", "db_name", "db_initial_desc1", "db_initial_desc", 
  "db_initial_option", "db_rem_desc1", "db_rem_desc", "db_rem_option", 
  "db_file", "file1", "file_desc1", "file_desc", "file_clause", 
  "file_clause_noise", "page_noise", "table_clause", "rtable_clause", 
  "external_file", "table_elements", "table_element", "column_def", 
  "def_computed", "computed_by", "data_type_or_domain", "collate_clause", 
  "column_def_name", "simple_column_def_name", "data_type_descriptor", 
  "init_data_type", "default_value", "column_constraint_clause", 
  "column_constraint_list", "column_constraint_def", "column_constraint", 
  "table_constraint_definition", "constraint_name_opt", 
  "table_constraint", "unique_constraint", "primary_constraint", 
  "referential_constraint", "constraint_index_opt", 
  "referential_trigger_action", "update_rule", "delete_rule", 
  "referential_action", "procedure_clause", "rprocedure_clause", 
  "replace_procedure_clause", "alter_procedure_clause", 
  "input_parameters", "output_parameters", "input_proc_parameters", 
  "input_proc_parameter", "output_proc_parameters", "proc_parameter", 
  "default_par_opt", "local_declaration_list", "local_declarations", 
  "local_declaration", "local_declaration_item", "var_declaration_item", 
  "var_decl_opt", "var_init_opt", "cursor_declaration_item", "proc_block", 
  "full_proc_block", "full_proc_block_body", "proc_statements", 
  "proc_statement", "simple_proc_statement", "complex_proc_statement", 
  "excp_statement", "raise_statement", "exec_sql", "for_select", 
  "for_exec_into", "exec_into", "if_then_else", "post_event", 
  "event_argument_opt", "singleton_select", "variable", "variable_list", 
  "while", "label_opt", "breakleave", "cursor_def", 
  "excp_hndl_statements", "excp_hndl_statement", "errors", "err", 
  "cursor_statement", "open_cursor", "close_cursor", "fetch_cursor", 
  "fetch_opt", "exec_procedure", "proc_inputs", "proc_outputs_opt", 
  "exec_block", "block_input_params", "block_parameters", 
  "block_parameter", "view_clause", "rview_clause", "begin_string", 
  "begin_trigger", "end_trigger", "end_default_opt", "check_opt", 
  "trigger_clause", "rtrigger_clause", "replace_trigger_clause", 
  "trigger_active", "trigger_type", "trigger_type_prefix", 
  "trigger_type_suffix", "trigger_position", "trigger_action", "alter", 
  "alter_clause", "alter_domain_ops", "alter_domain_op", "alter_ops", 
  "alter_op", "alter_column_name", "keyword_or_column", "col_opt", 
  "alter_data_type_or_domain", "alter_col_name", "drop_behaviour", 
  "alter_index_clause", "alter_sequence_clause", "alter_udf_clause", 
  "entry_op", "module_op", "init_alter_db", "alter_db", "db_alter_clause", 
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
  "tra_misc_options", "tra_timeout", "tbl_reserve_options", "lock_type", 
  "lock_mode", "restr_list", "restr_option", "table_lock", "table_list", 
  "set_statistics", "comment", "ddl_type0", "ddl_type1", "ddl_type2", 
  "ddl_subname", "ddl_desc", "select", "for_update_clause", 
  "for_update_list", "lock_clause", "select_expr", "column_select", 
  "column_singleton", "select_expr_body", "query_term", "query_spec", 
  "begin_limit", "end_limit", "begin_first", "end_first", "limit_clause", 
  "first_clause", "skip_clause", "distinct_clause", "select_list", 
  "select_items", "select_item", "as_noise", "from_clause", "from_list", 
  "table_reference", "table_primary", "derived_table", "correlation_name", 
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
  "returning_clause", "cursor_clause", "assignments", "assignment", 
  "exec_udf", "blob_io", "filter_clause_io", "blob_subtype_value_io", 
  "blob_subtype_io", "segment_clause_io", "segment_length_io", 
  "column_parens_opt", "column_parens", "column_list", 
  "ins_column_parens_opt", "ins_column_parens", "ins_column_list", 
  "column_name", "simple_column_name", "update_column_name", 
  "search_condition", "bracable_search_condition", 
  "simple_search_condition", "predicate", "comparison_predicate", 
  "quantified_predicate", "some", "distinct_predicate", 
  "between_predicate", "like_predicate", "in_predicate", 
  "containing_predicate", "starting_predicate", "exists_predicate", 
  "singular_predicate", "null_predicate", "trigger_action_predicate", 
  "special_trigger_action_predicate", "in_predicate_value", 
  "table_subquery", "value", "datetime_value_expression", 
  "sec_precision_opt", "array_element", "value_list", "constant", 
  "u_numeric_constant", "u_constant", "parameter", "current_user", 
  "current_role", "internal_info", "sql_string", "signed_short_integer", 
  "nonneg_short_integer", "neg_short_integer", "pos_short_integer", 
  "unsigned_short_integer", "signed_long_integer", "long_integer", 
  "function", "aggregate_function", "numeric_value_function", 
  "extract_expression", "length_expression", "bit_length_expression", 
  "char_length_expression", "octet_length_expression", 
  "string_value_function", "substring_function", "string_length_opt", 
  "trim_function", "trim_specification", "udf", "cast_specification", 
  "case_expression", "case_abbreviation", "case_specification", 
  "simple_case", "simple_when_clause", "searched_case", 
  "searched_when_clause", "when_operand", "case_operand", "case_result", 
  "next_value_expression", "timestamp_part", "all_noise", 
  "distinct_noise", "null_value", "symbol_UDF_name", 
  "symbol_blob_subtype_name", "symbol_character_set_name", 
  "symbol_collation_name", "symbol_column_name", "symbol_constraint_name", 
  "symbol_cursor_name", "symbol_domain_name", "symbol_exception_name", 
  "symbol_filter_name", "symbol_gdscode_name", "symbol_generator_name", 
  "symbol_index_name", "symbol_item_alias_name", "symbol_label_name", 
  "symbol_ddl_name", "symbol_procedure_name", "symbol_role_name", 
  "symbol_table_alias_name", "symbol_table_name", "symbol_trigger_name", 
  "symbol_user_name", "symbol_variable_name", "symbol_view_name", 
  "symbol_savepoint_name", "valid_symbol_name", "non_reserved_word", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   300,   300,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   302,   302,   302,   303,   303,   304,
     304,   304,   305,   305,   306,   307,   307,   307,   307,   307,
     308,   308,   309,   309,   310,   311,   311,   311,   312,   312,
     313,   313,   314,   314,   315,   315,   315,   315,   316,   316,
     316,   316,   317,   317,   318,   318,   318,   319,   319,   320,
     321,   321,   322,   322,   323,   324,   324,   325,   326,   326,
     326,   327,   327,   327,   328,   328,   329,   330,   330,   330,
     330,   331,   331,   332,   332,   333,   333,   333,   333,   333,
     334,   335,   335,   336,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   338,   339,   339,   339,
     339,   339,   340,   341,   341,   341,   342,   343,   344,   345,
     346,   346,   347,   347,   347,   348,   349,   349,   349,   350,
     350,   351,   351,   352,   352,   353,   353,   354,   355,   355,
     356,   357,   357,   358,   358,   359,   359,   360,   361,   361,
     362,   363,   364,   365,   366,   367,   367,   368,   369,   369,
     370,   370,   371,   371,   371,   371,   371,   372,   372,   373,
     373,   374,   374,   374,   375,   376,   377,   377,   378,   378,
     379,   379,   380,   380,   380,   381,   381,   381,   382,   383,
     384,   384,   384,   385,   385,   386,   386,   387,   387,   387,
     388,   389,   389,   390,   390,   391,   391,   392,   393,   394,
     395,   396,   396,   396,   396,   396,   396,   397,   397,   398,
     398,   399,   400,   400,   400,   400,   400,   401,   402,   402,
     403,   403,   403,   403,   404,   405,   406,   407,   407,   408,
     408,   408,   408,   408,   409,   410,   411,   411,   411,   411,
     412,   413,   414,   415,   416,   416,   417,   417,   418,   418,
     419,   420,   420,   421,   422,   422,   422,   423,   423,   424,
     424,   425,   426,   426,   427,   428,   428,   429,   429,   429,
     430,   431,   431,   432,   433,   433,   433,   434,   434,   435,
     435,   436,   436,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   436,   436,   436,   436,   437,   437,   437,
     437,   438,   438,   439,   440,   441,   442,   443,   444,   444,
     445,   446,   447,   448,   449,   449,   449,   449,   450,   451,
     451,   452,   452,   452,   453,   453,   454,   454,   455,   456,
     456,   457,   457,   457,   457,   458,   458,   458,   459,   460,
     461,   462,   463,   464,   464,   464,   465,   465,   465,   466,
     467,   467,   468,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   476,   477,   478,   479,   480,   480,   480,   481,
     482,   482,   483,   483,   483,   483,   483,   483,   483,   483,
     483,   483,   483,   483,   483,   483,   483,   484,   484,   485,
     486,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     488,   488,   489,   489,   489,   489,   489,   489,   489,   490,
     490,   491,   491,   491,   491,   491,   491,   491,   491,   491,
     492,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   494,   494,   495,   495,   496,   497,   497,   497,
     498,   498,   499,   499,   499,   500,   501,   501,   502,   502,
     503,   504,   504,   505,   505,   505,   505,   505,   506,   507,
     507,   508,   508,   509,   510,   510,   510,   510,   510,   510,
     510,   510,   510,   510,   510,   510,   510,   511,   511,   512,
     512,   513,   513,   514,   514,   515,   515,   516,   516,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   518,   518,
     519,   519,   519,   519,   520,   520,   521,   521,   521,   522,
     522,   523,   523,   523,   524,   524,   524,   525,   525,   525,
     526,   526,   527,   527,   527,   528,   528,   529,   529,   529,
     530,   530,   531,   531,   531,   531,   532,   532,   533,   533,
     533,   534,   534,   534,   535,   535,   535,   536,   537,   538,
     538,   539,   540,   540,   541,   542,   543,   543,   544,   544,
     545,   545,   546,   547,   547,   548,   548,   549,   549,   549,
     549,   549,   549,   550,   550,   551,   551,   552,   552,   553,
     553,   553,   554,   554,   554,   555,   555,   555,   556,   556,
     556,   557,   558,   559,   559,   559,   560,   560,   561,   561,
     562,   563,   563,   564,   564,   565,   566,   566,   566,   567,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   569,   569,   570,   571,   571,   572,
     573,   573,   574,   574,   575,   575,   576,   577,   578,   579,
     579,   579,   580,   581,   582,   583,   584,   585,   586,   586,
     586,   586,   587,   587,   587,   588,   588,   588,   589,   589,
     590,   590,   591,   591,   592,   592,   593,   593,   594,   595,
     595,   596,   596,   597,   597,   597,   598,   599,   599,   600,
     600,   601,   601,   602,   602,   602,   603,   604,   605,   606,
     606,   607,   608,   609,   609,   610,   610,   611,   611,   612,
     613,   613,   613,   613,   613,   614,   614,   615,   615,   616,
     616,   617,   618,   618,   619,   619,   620,   620,   621,   622,
     622,   622,   622,   622,   623,   623,   624,   624,   625,   625,
     626,   626,   626,   627,   627,   628,   628,   629,   629,   630,
     630,   631,   632,   632,   632,   633,   633,   634,   634,   635,
     635,   635,   636,   636,   637,   637,   638,   639,   640,   640,
     641,   642,   643,   643,   643,   644,   645,   645,   646,   647,
     648,   648,   649,   649,   649,   650,   650,   651,   652,   652,
     653,   653,   654,   654,   655,   656,   656,   657,   657,   658,
     659,   659,   660,   660,   660,   661,   662,   662,   663,   663,
     663,   663,   663,   664,   664,   664,   664,   664,   664,   665,
     665,   665,   666,   666,   666,   666,   666,   666,   666,   666,
     666,   666,   666,   667,   667,   667,   667,   667,   667,   667,
     667,   668,   668,   668,   668,   668,   668,   668,   668,   668,
     668,   668,   668,   668,   668,   668,   668,   669,   669,   670,
     670,   671,   671,   672,   672,   672,   672,   673,   673,   674,
     674,   675,   675,   675,   675,   676,   677,   678,   678,   679,
     679,   679,   680,   680,   680,   681,   681,   682,   683,   683,
     683,   683,   683,   683,   683,   683,   683,   683,   683,   683,
     683,   683,   683,   683,   683,   683,   683,   683,   683,   683,
     683,   683,   683,   683,   683,   683,   684,   684,   684,   685,
     685,   686,   687,   687,   688,   688,   689,   689,   689,   689,
     689,   690,   690,   690,   690,   690,   691,   692,   692,   693,
     694,   694,   694,   694,   694,   695,   695,   696,   696,   697,
     698,   699,   700,   701,   701,   702,   703,   703,   703,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     705,   705,   706,   707,   707,   707,   708,   709,   709,   710,
     711,   711,   711,   711,   712,   713,   713,   714,   714,   714,
     714,   715,   715,   715,   716,   716,   717,   718,   718,   719,
     719,   719,   720,   720,   721,   721,   722,   722,   723,   723,
     724,   724,   725,   726,   727,   728,   728,   729,   729,   729,
     729,   729,   729,   729,   729,   730,   730,   731,   731,   732,
     733,   734,   735,   736,   737,   738,   739,   740,   741,   742,
     743,   744,   745,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   758,   759,   759,   759,
     759,   759,   759,   759,   759,   759,   759,   759,   759,   759,
     759,   759,   759,   759,   759,   759,   759,   759,   759,   759,
     759,   759,   759,   759,   759,   759,   759,   759,   759,   759,
     759,   759,   759
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
       2,     2,     4,     2,     2,     2,     2,     2,     2,     2,
       1,     0,     1,     1,     6,     6,     1,     1,     0,     0,
       1,     0,     4,     0,     1,     1,     2,     8,     1,     0,
       3,     1,     0,     1,     0,     1,     2,     1,     1,     1,
       2,     6,     1,     1,     3,     0,     1,     1,     0,     1,
       1,     2,     3,     4,     2,     2,     3,     0,     1,     1,
       2,     1,     4,     3,     3,     1,     0,     1,     1,     2,
       3,     4,     0,     1,     2,     0,     1,     1,     5,     5,
       3,     2,     0,     1,     3,     1,     1,     6,     3,     2,
       6,     2,     1,     2,     2,     2,     0,     1,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       2,     2,     1,     1,     5,     2,     3,     2,     2,     0,
       1,     1,     1,     1,     3,     4,     8,     4,     0,     1,
       1,     2,     2,     0,     3,     3,     1,     2,     2,     2,
       8,     8,     8,     8,     3,     0,     4,     0,     1,     3,
       5,     1,     3,     2,     3,     3,     0,     1,     0,     1,
       2,     4,     1,     1,     3,     1,     0,     2,     2,     0,
       6,     1,     1,     3,     1,     2,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     1,     3,     8,     9,     5,     8,     6,
       3,     0,     3,     2,     1,     1,     3,     3,     7,     2,
       0,     1,     1,     2,     3,     0,     1,     2,     4,     1,
       3,     2,     2,     2,     1,     1,     1,     1,     2,     2,
       5,     0,     5,     1,     3,     0,     2,     4,     0,     7,
       3,     0,     1,     3,     3,     7,     7,     0,     0,     0,
       0,     3,     0,     8,     8,     8,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     5,     5,     5,     5,     5,     5,     2,     0,     4,
       2,     2,     3,     2,     2,     3,     3,     2,     2,     3,
       1,     2,     3,     3,     2,     2,     2,     2,     3,     1,
       3,     3,     3,     2,     2,     4,     4,     4,     5,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     0,
       2,     2,     4,     4,     5,     3,     2,     0,     2,     0,
       0,     1,     2,     2,     4,     3,     2,     2,     7,     1,
       0,     1,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     2,     2,     2,     2,     1,     1,     1,
       1,     4,     5,     1,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     6,     5,     3,     0,     2,     2,     0,     3,
       0,     4,     1,     5,     4,     1,     4,     1,     2,     2,
       1,     1,     1,     2,     2,     2,     2,     0,     3,     5,
       1,     1,     2,     3,     1,     2,     3,     0,     1,     1,
       1,     5,     5,     6,     1,     1,     1,     2,     4,     1,
       0,     5,     1,     0,     3,     3,     1,     0,     2,     0,
       1,     0,     3,     1,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     2,     3,     1,     1,
       3,     3,     1,     2,     3,     1,     2,     0,     3,     2,
       2,     3,     2,     1,     1,     0,     1,     1,     1,     3,
       2,     3,     0,     1,     3,     4,     5,     6,     7,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     2,     1,     1,     3,
       3,     0,     2,     0,     2,     0,     3,     3,     3,     1,
       4,     4,     1,     9,     0,     0,     0,     0,     3,     2,
       1,     0,     3,     5,     3,     2,     5,     2,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     0,     2,     1,
       3,     1,     1,     1,     1,     3,     6,     1,     0,     3,
       0,     1,     3,     1,     1,     1,     4,     5,     5,     1,
       1,     2,     4,     4,     2,     3,     0,     1,     2,     1,
       1,     2,     2,     2,     0,     1,     0,     3,     0,     1,
       3,     1,     2,     0,     2,     0,     2,     0,     4,     1,
       2,     1,     1,     0,     1,     3,     2,     1,     1,     2,
       1,     4,     3,     1,     3,     4,     0,     3,     0,     1,
       3,     3,     1,     1,     0,     1,     1,     4,     0,     2,
       4,     0,     9,     5,     1,     1,     7,     4,     1,     1,
       8,     5,     2,     4,     0,     4,     1,     3,     3,     1,
       7,     7,     5,     3,     0,     1,     1,     1,     2,     0,
       1,     1,     1,     0,     3,     1,     3,     1,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     2,
       1,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     1,     1,     5,
       6,     5,     6,     3,     4,     5,     6,     3,     4,     3,
       4,     3,     4,     4,     5,     4,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     2,     2,     3,
       0,     4,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     4,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       1,     1,     6,     1,     1,     1,     4,     4,     4,     4,
       1,     1,     4,     4,     7,     2,     0,     7,     6,     6,
       4,     1,     1,     1,     4,     3,     6,     1,     1,     6,
       8,     6,     1,     1,     4,     6,     4,     5,     3,     5,
       4,     5,     1,     1,     1,     4,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
       0,     0,   607,   131,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   607,   701,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   589,    19,   594,   595,   596,     6,    18,
     588,   590,     5,    20,   681,   788,   689,   692,    12,     9,
     804,   805,    22,   808,   809,     4,   500,     0,     0,     0,
       0,     0,     0,     0,     0,   410,   606,   609,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,   103,   794,   989,     0,    23,   987,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,     0,   371,    29,    37,    34,
      36,   833,    35,   833,  1095,  1097,  1098,  1099,  1100,  1101,
    1102,  1121,  1122,  1103,  1105,  1104,  1110,  1106,  1111,  1107,
    1109,  1112,  1108,  1113,  1114,  1115,  1117,  1118,  1116,  1119,
    1120,  1124,  1123,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,     0,    31,     0,    32,     0,    67,    69,  1087,  1096,
       0,     0,     0,     0,     0,     0,     0,   609,     0,     0,
    1066,   695,   700,     0,     0,   614,     0,   747,   749,  1089,
       0,     0,     0,     0,     0,   116,   597,  1094,     0,     0,
       2,     0,   685,     0,  1068,   801,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   470,   464,
     465,   463,   466,   467,   468,   469,   471,   473,   476,   472,
     474,   475,   477,   478,   479,   480,   481,     0,   440,   441,
     411,     0,  1078,     0,   417,     0,  1082,   414,   265,  1086,
       0,   749,   413,   388,  1090,   418,     0,  1081,   611,   604,
     985,     0,   112,   168,   167,   113,   149,   217,   845,  1074,
     104,     0,   110,   162,     0,   106,   265,   114,   991,   138,
     107,   202,   108,     0,   109,   833,  1093,   115,   163,   111,
     792,   793,     0,   990,   988,     0,    75,     0,  1079,   765,
     521,  1077,   514,     0,   520,   525,   515,   516,   523,   517,
     518,   519,   524,   526,   365,     0,   267,    30,     0,    39,
     832,    38,    28,     0,     0,     0,     0,     0,   838,     0,
       0,     0,     0,     0,     0,   603,   605,   995,     0,   976,
     694,   694,   695,   707,   705,  1065,   708,     0,   709,   699,
     695,     0,     0,     0,     0,     0,     0,   632,   625,     0,
       0,     0,   612,   613,   615,   617,   618,   619,   628,   629,
     620,   621,   622,     0,   748,  1088,   121,     0,   117,   265,
     118,   202,   120,     0,   119,   833,   600,     0,   659,   660,
     667,     0,   666,   668,   670,   675,   663,   661,   664,   662,
     671,   674,   669,   673,     0,     0,     0,   683,     0,   679,
       0,     0,  1067,     0,     0,   686,     0,     0,     0,     0,
     415,   501,     0,     0,     0,     0,   220,   416,   420,   129,
    1070,   419,   497,   490,   491,     0,   267,   239,   482,     0,
     412,   429,     0,   386,   387,   510,     0,   610,   608,   986,
     165,   165,     0,     0,     0,   177,   169,   170,   148,     0,
     126,     0,     0,     0,   122,   267,   137,   136,   139,     0,
       0,     0,     0,     0,    76,   220,     0,     0,   767,   807,
     522,     0,     0,     0,     0,   951,   982,     0,  1069,     0,
     953,     0,     0,   983,     0,   977,   968,   967,   966,  1095,
       0,     0,     0,   956,   960,   960,   969,   970,   978,   979,
    1105,   980,   981,     0,  1110,  1106,   984,  1119,  1124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   933,
     368,   928,   842,   962,   954,   929,   363,   971,   931,   932,
     948,   949,   950,   972,   930,   996,   997,  1010,  1011,  1013,
    1014,  1015,   998,  1020,  1021,   937,   934,   935,  1037,  1038,
    1042,  1043,   936,   955,     0,     0,  1074,     0,     0,     0,
     372,   218,     0,     0,     0,   835,    27,     0,    33,     0,
       0,    43,    70,    72,  1091,    68,     0,     0,     0,   837,
       0,    48,    50,    28,     0,     0,   602,     0,     0,     0,
     704,   702,     0,   711,     0,   710,   712,   717,   698,     0,
     655,     0,     0,   626,   637,   623,   637,   624,   642,   648,
     652,   653,   633,     0,   640,   639,   616,   765,   816,   846,
       0,     0,   127,   267,     0,     0,     0,   599,   598,   672,
     665,     0,     0,  1085,     0,     0,   680,   684,   787,   789,
     794,   691,   690,   799,   185,     0,   503,   145,     0,   506,
       0,   507,   502,   378,     0,   424,   426,   425,   378,   379,
     427,     0,   421,     0,   499,     0,     0,   268,     0,     0,
     433,     0,   434,     0,   483,     0,   489,     0,     0,     0,
     486,  1074,   391,   390,   509,     0,   408,     0,   166,     0,
       0,   175,     0,   174,     0,     0,   164,   178,   179,   181,
     171,   558,   570,   545,   580,   548,   571,   581,     0,   587,
     549,     0,   577,     0,   572,   584,   544,   567,   546,   547,
     542,   378,   527,   528,   529,   537,   543,   530,   539,   560,
       0,   565,   562,   540,   577,   541,   125,     0,   123,   265,
     124,     0,     0,   140,     0,     0,   201,   239,   388,   377,
       0,   220,     0,    82,    84,     0,     0,   102,   101,  1071,
       0,     0,     0,     0,   919,   920,   921,     0,   764,   850,
     859,   862,   868,   863,   864,   865,   866,   870,   871,   869,
     872,   867,   848,     0,   773,   788,  1066,     0,  1066,   973,
       0,     0,  1066,  1066,  1066,     0,   974,   975,     0,   957,
     958,     0,     0,  1053,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   939,   938,     0,   788,
     962,     0,   333,  1092,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,   537,   560,     0,
       0,   370,     0,   278,     0,   834,     0,     0,    44,    73,
       0,     0,    26,   824,     0,   840,     0,   803,   824,     0,
       0,    47,   601,     0,   694,     0,     0,   765,     0,   716,
       0,   592,     0,   591,   993,     0,   627,   638,     0,   635,
     631,   630,     0,   645,     0,   650,   634,   641,     0,   767,
     811,     0,     0,     0,   239,   388,   377,   678,   656,   677,
       0,     0,     0,   682,     0,   798,     0,     0,   146,   186,
     505,     0,   423,     0,   422,   428,   496,     0,   495,   378,
       0,   264,   377,   238,  1075,   212,   209,     0,   152,   378,
     527,   377,     0,     0,     0,   243,   237,   240,   241,   242,
     432,   488,   487,   431,   430,     0,     0,     0,     0,     0,
     394,   392,   393,   389,     0,   378,   493,     0,   492,   195,
     172,   176,     0,     0,   180,     0,     0,   555,   568,   569,
     585,     0,   582,   587,     0,   575,   573,   574,   152,     0,
       0,     0,   538,     0,     0,     0,     0,   576,   128,   267,
       0,   377,   141,   200,     0,   203,   205,   206,     0,     0,
       0,     0,   220,   220,    79,     0,    87,    78,     0,     0,
       0,     0,   861,   849,     0,     0,   922,   923,   924,     0,
     853,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   771,   772,   766,     0,   801,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,     0,     0,
       0,     0,     0,  1048,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1031,  1033,  1032,     0,
       0,   947,   801,   946,   364,     0,   334,   366,   335,     0,
       0,   942,  1073,   941,   940,   943,   944,   945,   963,  1035,
       0,   952,   844,   843,   374,   373,     0,   271,   286,     0,
     277,   279,   836,     0,     0,     0,    71,     0,   829,     0,
     839,     0,   829,     0,     0,   703,   706,     0,   718,   719,
     722,   724,   721,   733,   734,   735,   723,   746,     0,   758,
     713,   715,  1083,   593,   994,   636,   649,   644,   643,     0,
     654,   817,   788,   818,   847,   377,     0,     0,     0,   657,
     676,     0,   790,   696,   791,   800,   504,   165,   192,   184,
     187,   188,     0,     0,   150,   226,   221,   964,   222,   223,
     224,   225,   498,   276,   269,   278,   211,   378,   151,   380,
     213,   208,   214,     0,     0,   248,   436,   439,   379,   437,
     484,   485,   435,     0,     0,     0,   407,   512,   494,   196,
     197,   173,     0,   183,   556,   557,   992,     0,     0,     0,
     560,     0,   583,     0,     0,   380,     0,   533,   535,     0,
       0,     0,     0,     0,     0,     0,   388,   278,   165,   143,
     239,   198,   408,   382,     0,    83,     0,   220,     0,    91,
       0,    85,     0,     0,    90,    86,     0,   815,  1076,     0,
       0,   854,     0,     0,   860,     0,     0,   852,   851,     0,
     909,     0,   898,   897,     0,   876,     0,   907,   925,     0,
     917,     0,     0,     0,   877,   903,     0,     0,   880,     0,
       0,     0,     0,     0,     0,     0,   878,     0,     0,   879,
       0,   911,     0,     0,   873,     0,     0,   874,     0,     0,
     875,   770,   773,   806,     0,     0,   220,     0,   999,     0,
       0,  1022,     0,     0,     0,     0,     0,     0,     0,   959,
       0,     0,  1054,     0,     0,  1052,     0,     0,  1044,     0,
       0,     0,     0,  1055,  1016,  1017,  1018,  1023,  1019,     0,
    1030,     0,     0,   688,     0,     0,     0,   961,  1034,     0,
     266,   285,     0,   340,   369,   280,     0,     0,     0,     0,
       0,  1101,    41,    52,    54,    53,    62,    64,    41,    42,
       0,     0,     0,   821,   841,     0,   820,     0,     0,     0,
     754,   721,     0,   756,   750,   756,   754,   756,     0,     0,
       0,   717,     0,   763,   646,   647,   651,   801,   278,   199,
     408,   382,   658,     0,     0,   193,     0,   189,     0,   965,
     378,   378,   379,     0,     0,   227,     0,   248,   794,   244,
     438,   399,   400,   396,   395,   398,   397,   378,   511,   379,
     182,  1072,     0,     0,   551,     0,   550,   586,     0,   578,
     154,     0,   531,     0,   559,   560,   566,   564,     0,   561,
     377,     0,     0,     0,   135,   144,   204,     0,     0,   379,
     105,     0,   133,   132,    94,     0,     0,    95,     0,    88,
      89,     0,   915,   916,   856,   855,   858,   857,     0,     0,
       0,     0,   788,     0,     0,     0,   918,     0,     0,     0,
       0,     0,     0,   910,   908,   904,     0,   912,     0,     0,
       0,     0,   913,     0,     0,     0,     0,     0,     0,   777,
       0,   774,     0,   778,  1005,  1004,     0,     0,  1001,  1000,
       0,  1009,  1008,  1007,  1006,  1003,  1002,     0,  1026,  1050,
    1049,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   337,   336,   272,     0,     0,   282,   283,     0,
    1074,   323,     0,   316,     0,     0,     0,   315,  1103,  1108,
       0,     0,   361,   297,   292,     0,   340,   291,     0,   300,
     310,   311,   307,   319,   320,   308,   317,   312,   305,   318,
       0,   314,   313,   355,   357,   356,   306,     0,   302,   304,
     303,   301,   309,   819,     0,  1074,    66,    58,    59,    65,
      60,    61,     0,    24,     0,     0,    25,     0,   825,   826,
     827,   823,   828,   831,   830,   814,    45,    46,   717,   725,
     720,   755,   753,   751,     0,   752,     0,     0,     0,     0,
       0,     0,   767,   810,     0,     0,   379,   795,   796,   697,
     195,   194,   190,   379,     0,     0,   270,   379,   379,   216,
     228,   229,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,   278,   508,   553,     0,   554,     0,     0,   216,
     153,   155,   157,   158,   159,   534,   536,   532,   563,   278,
     408,   379,   195,   379,     0,   375,   378,    92,     0,     0,
      98,    93,   560,     0,   901,     0,     0,   927,   801,   926,
     899,     0,     0,     0,   905,     0,     0,     0,     0,   914,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,   773,     0,   780,     0,   776,   779,  1036,   219,  1056,
    1012,     0,     0,  1051,  1046,  1045,     0,  1039,  1041,     0,
    1028,  1029,     0,   289,   281,     0,   321,     0,     0,   331,
     343,  1084,   358,   359,     0,   293,     0,   298,   295,   346,
     299,     0,     0,     0,   339,     0,    55,    56,    57,    63,
       0,     0,   802,   728,     0,   736,   754,   745,   743,   757,
     759,   761,   762,   693,   379,   379,   376,   797,   191,   161,
     274,   275,   263,     0,     0,   207,   230,     0,     0,   248,
     232,   233,   231,   833,     0,   405,   406,   402,   401,   404,
     403,     0,   552,   579,   160,   147,   156,     0,     0,   260,
     142,   383,   381,     0,     0,    96,    97,    80,     0,   884,
     892,   687,   900,   885,   893,   888,   896,   902,   906,   886,
     894,   887,   895,   881,   889,   882,   890,   883,   891,   775,
       0,   786,  1025,  1024,  1047,     0,  1027,     0,     0,   284,
       0,   322,   324,     0,   330,     0,   354,     0,     0,     0,
       0,   349,   347,     0,     0,     0,   332,    40,   822,   812,
     730,   727,   737,     0,     0,   738,   739,   740,     0,   261,
     384,   210,   215,   248,   833,   235,   253,   247,   409,   379,
     379,   379,    77,    99,   100,     0,   783,     0,   782,  1040,
     287,   288,     0,     0,     0,     0,   353,   352,  1080,   351,
     340,     0,     0,     0,     0,     0,     0,   726,   741,     0,
     760,   236,   253,     0,   248,   249,   250,   262,   385,     0,
     781,     0,     0,     0,   327,   340,   360,   348,   350,     0,
     345,     0,   813,     0,   731,     0,   248,     0,     0,   246,
       0,   252,     0,   251,   134,   784,     0,   290,   329,     0,
       0,     0,   340,     0,   729,   742,   234,     0,     0,   256,
     255,   254,   785,   340,     0,     0,   340,   338,   732,   259,
     257,   258,   328,   340,   344,   325,   326,     0,     0,     0
};

static const short yydefgoto[] =
{
    2027,    21,    22,   577,   151,   152,   153,   154,  1633,   862,
     857,    23,   165,   166,  1392,  1393,  1394,  1395,  1396,   155,
     156,   581,   582,    24,    89,   474,  1016,   762,   763,   764,
    1275,  1268,  1269,  1721,   296,   766,    25,    81,    26,   185,
      27,   464,   270,   376,   746,   240,    82,  1490,   277,   468,
     754,  1259,  1484,   656,   265,   459,  1208,  1209,  1699,  1700,
    1701,  1702,  1703,  1704,   272,   287,   262,   699,   263,   455,
     456,   457,   706,   707,   708,   657,   658,  1189,  1190,  1191,
    1436,  1231,   280,   380,   470,  1004,  1005,  1006,   936,   937,
     938,  1825,   681,   567,  1546,   765,  1194,  1679,  1680,  1681,
    1832,  1007,   683,   946,   947,   948,   949,  1449,  1964,  1965,
    1966,  2010,   275,   378,   748,   247,   436,   573,   676,   677,
    1126,   568,  1442,  1129,  1130,  1131,  1576,  1577,  1382,  1889,
    1578,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,
    1602,  1603,  1604,  1605,  1606,  1607,  1894,  1608,   529,  1107,
    1609,  1610,  1611,  2001,  1788,  1789,  1900,  1901,  1612,  1613,
    1614,  1615,  1784,  1616,   530,   835,    29,   316,   569,   570,
     284,   384,  1009,   921,   924,  1445,  1489,   282,   382,   750,
     445,   694,   695,   963,   965,  1458,    30,    65,   427,   428,
     440,   441,   237,   238,   442,  1219,   689,   953,   244,   255,
     431,   674,   928,   196,   420,   421,   252,   696,  1459,    31,
     104,   731,   732,   733,  1246,  1247,   734,   847,   736,   737,
    1240,   977,   992,   738,   848,   740,   741,   742,   743,   985,
     744,   745,   982,    32,    33,    34,    35,    36,   638,    37,
     597,    38,    39,    67,   259,   448,    40,   362,   363,   364,
     365,   366,   367,   368,   369,   890,   370,   371,   372,  1169,
    1426,   618,   619,   895,   620,    41,    42,   404,   405,   406,
     912,   908,  1617,   192,   646,   409,    44,  1511,   828,    45,
      46,    47,   600,   349,  1433,  1817,   170,   171,   172,   347,
     604,   605,   606,   880,   877,  1148,  1149,  1150,  1151,  1910,
    1957,  1983,  1152,  1153,  1154,  1155,  1915,  1916,  1917,  1156,
    1421,   176,   177,  1419,  1652,  1423,  1809,  1810,  1662,   478,
     795,  1539,  1054,  1540,  1541,  1542,  1755,  1935,  1938,   195,
     648,   649,   292,  1669,  1184,   415,  1618,  1619,    50,    51,
    1620,    53,    54,  1802,   479,   627,  1621,  1622,    55,  1138,
    1637,  1638,  1403,  1642,   319,   320,   574,   588,   589,   864,
     531,   532,   630,   778,  1029,   779,   780,   781,   782,  1294,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
    1031,  1297,  1298,   793,   534,   809,   535,   536,  1196,   537,
     538,   539,   540,   541,   542,   543,  1640,    86,   294,   279,
    1238,  1248,   884,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,  1762,   554,  1100,   555,   556,   557,   558,
     559,   560,  1086,   561,   814,  1356,   815,  1353,   562,  1078,
     348,   413,   563,   564,   768,  1460,  1111,   268,   933,  1277,
     300,   241,   297,  1947,   273,  1936,  1161,  1624,   642,  1157,
     157,   565,   251,   253,  1397,   832,   285,   186,   566,   159
};

static const short yypact[] =
{
    9043,  1581,    33,  1711,    27,   664,   312,  1976,    93,  8160,
     571,   366,  1601,    33,   547,   137,  7109,   403,  7109,   193,
     292,   217,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   534,   230,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  9327,  7109,   508,
    7109,  7109,  7109,  7109,  7109,-32768,-32768,   510,   691,  7109,
    7109,  7109,   705,  7109,   596,  7109,  7109,-32768,  7109,  7109,
    7109,-32768,   749,-32768,   626,-32768,-32768,   665,  7109,-32768,
    7109,  7109,  7109,   690,  7109,  7109,  7109,  7109,   596,  7109,
    7109,  7109,  7109,  7109,-32768,  7109,   545,   740,-32768,-32768,
  -32768,   558,-32768,   558,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   735,   584,   762,-32768,    38,-32768,-32768,-32768,-32768,
    7109,  7109,  7109,   776,   806,  1785,  7109,   567,   267,   301,
     617,   682,-32768,  7109,   856,  1666,   782,-32768,  7109,-32768,
    7109,  7109,  7109,  7109,  7109,-32768,-32768,-32768,  7109,  8868,
  -32768,   767,   750,   934,   661,   701,   920,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   166,-32768,-32768,
  -32768,   691,-32768,   766,-32768,   473,-32768,-32768,   686,-32768,
     885,-32768,-32768,   542,-32768,-32768,   753,-32768,   822,-32768,
  -32768,   795,-32768,   562,-32768,-32768,  1012,-32768,-32768,-32768,
  -32768,   691,-32768,-32768,   501,-32768,   686,-32768,-32768,   354,
  -32768,   970,-32768,   974,-32768,   558,-32768,-32768,-32768,-32768,
  -32768,-32768,   961,-32768,-32768,   766,-32768,   965,-32768,   866,
  -32768,-32768,-32768,   766,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  7777,  7109,   926,-32768,  7109,-32768,
  -32768,-32768,   924,   601,   963,  4941,  7109,  1025,   818,  1053,
    1068,  1072,  1014,  1018,    99,   907,-32768,-32768,  8048,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  5880,-32768,-32768,
  -32768,   985,  7109,  1059,   177,   168,  7109,   991,-32768,   888,
     890,   893,-32768,  1666,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  7109,-32768,-32768,-32768,   691,-32768,   686,
  -32768,   970,-32768,  1114,-32768,   558,  1054,  1020,-32768,-32768,
  -32768,  1115,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1098,  7109,  7109,  1062,   941,-32768,
    8048,  1037,-32768,  1037,  8048,-32768,    76,   931,   938,   940,
     920,-32768,   791,   472,  1166,  7109,-32768,   166,-32768,-32768,
  -32768,-32768,  1160,-32768,-32768,  7109,   926,  3008,  1006,  3291,
     922,-32768,  9327,-32768,-32768,   656,  1027,-32768,-32768,-32768,
     933,   933,   691,  1108,   691,   235,   562,-32768,-32768,  9169,
  -32768,  7109,  7109,  7109,-32768,   926,-32768,-32768,  1195,   409,
     929,  7109,  1215,  7109,-32768,    41,  6567,  4525,  1095,-32768,
  -32768,   943,   944,   945,  1032,-32768,-32768,   946,-32768,   948,
  -32768,   952,   953,-32768,   964,-32768,-32768,-32768,-32768,   966,
     967,  1033,  1035,-32768,   968,   968,-32768,-32768,-32768,-32768,
     969,-32768,-32768,  6151,   971,   973,-32768,   979,  1159,   982,
     983,   984,   994,  1002,  1004,  8048,  8048,  6422,  7109,-32768,
    1125,  1003,-32768,   744,-32768,-32768,  1022,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1011,  1017,  1023,  9169,  1044,   435,
  -32768,-32768,  1042,  1253,   574,-32768,-32768,  7109,-32768,  7109,
    7109,    67,-32768,-32768,-32768,-32768,  7109,  7109,   581,-32768,
    7109,-32768,-32768,   924,  1175,  4941,-32768,  7109,  6422,   234,
  -32768,-32768,  8048,-32768,  1273,  1045,-32768,   190,-32768,   317,
  -32768,   646,  1061,-32768,   118,-32768,   118,-32768,  1055,-32768,
     149,-32768,  1180,   596,-32768,-32768,-32768,    89,-32768,-32768,
    1064,  1052,-32768,   926,  1056,  7109,  1341,-32768,-32768,-32768,
  -32768,   456,  1269,-32768,  1060,  7109,-32768,-32768,  1065,-32768,
     245,-32768,-32768,   623,-32768,  1275,  1282,-32768,   691,-32768,
    1285,-32768,-32768,-32768,  1337,-32768,-32768,-32768,-32768,-32768,
  -32768,  9169,-32768,   691,  1258,  9169,   604,-32768,  1366,  7109,
  -32768,  8750,-32768,   485,-32768,  7109,   694,   885,  1209,   131,
    1248,  1212,-32768,-32768,-32768,   291,  1252,   378,-32768,  1194,
     596,-32768,   691,-32768,  1368,  1303,-32768,   235,-32768,-32768,
  -32768,    58,  1219,-32768,-32768,-32768,  1220,-32768,  1262,  1113,
  -32768,  1317,  1116,   572,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1123,-32768,-32768,-32768,   124,
    1117,  1118,    51,-32768,  1116,-32768,-32768,   691,-32768,   686,
  -32768,  1351,  1410,-32768,   691,   691,-32768,  3008,   542,-32768,
    1300,-32768,  1274,  1133,-32768,  3979,  1299,-32768,-32768,-32768,
    1307,  1136,  4796,  1137,  1409,  1836,  2305,  4254,   232,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  2588,   711,  1309,   704,  8048,   143,-32768,
    7109,  8048,   719,   728,   729,  1662,-32768,-32768,   596,-32768,
  -32768,  8048,  5067,   744,   188,  1244,  8048,  8048,  5067,  1375,
    8048,  8048,  8048,  8048,  8048,  5338,   179,   179,  1144,   230,
     415,   613,-32768,-32768,  5212,-32768,  8048,  7109,  8048,  8048,
    8048,  8048,  8048,  8048,  6693,  4399,-32768,-32768,  1418,  1142,
    7109,-32768,  7109,  1401,  7109,-32768,  1271,  1272,-32768,-32768,
    1246,  4941,-32768,  1391,   635,-32768,  1157,-32768,  1391,  7109,
    7109,  1161,-32768,   415,-32768,   464,  5754,  1264,  8048,-32768,
    7109,-32768,   606,-32768,-32768,   195,-32768,-32768,  1266,-32768,
  -32768,-32768,  7109,   470,  7109,-32768,-32768,-32768,  7109,  1095,
  -32768,  8048,  7109,  1450,  3008,   542,-32768,-32768,-32768,-32768,
     456,  7109,  1376,  1171,  8048,  1224,  8048,   691,-32768,   480,
  -32768,  1173,-32768,  3624,-32768,-32768,-32768,   691,-32768,-32768,
    7109,-32768,-32768,-32768,-32768,  1448,-32768,  1176,  1166,-32768,
    1441,-32768,  1385,  1387,   558,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  7109,  1431,  1166,  8954,   596,
    1353,  1354,  1361,-32768,   596,-32768,-32768,   641,-32768,   921,
  -32768,-32768,  1333,   691,-32768,  6567,   269,  1336,-32768,-32768,
  -32768,   596,-32768,  1113,   111,-32768,-32768,-32768,  1166,   111,
    1335,   111,-32768,   596,   596,  1200,   596,-32768,-32768,   926,
    7109,-32768,  1400,-32768,   662,-32768,-32768,-32768,   656,  1037,
    7109,   667,    45,-32768,-32768,  1206,   351,-32768,  6567,  7109,
    1037,  5609,-32768,-32768,  1037,  4796,-32768,-32768,-32768,   129,
  -32768,   262,  2026,  5067,  5067,  8048,  8048,  2002,  1207,   643,
    2285,  8048,  2568,   531,  2851,  3134,  6964,  3417,  3700,  3983,
  -32768,-32768,  1394,-32768,  1210,   701,  8048,  8048,   222,  8048,
    1211,  8048,  1216,   736,  8048,  8048,  8048,  8048,  8048,  8048,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1444,  1217,
     717,   173,  8048,-32768,  5067,  8048,   328,   265,   396,   147,
    7109,   760,   783,   811,   859,   880,-32768,-32768,-32768,   432,
    7235,-32768,   701,-32768,-32768,  5483,-32768,  1221,-32768,  1218,
     -51,-32768,-32768,  1481,   179,   179,   214,   214,   744,-32768,
     676,-32768,-32768,-32768,-32768,-32768,   723,-32768,  1330,  1505,
    1401,-32768,-32768,  9153,  9153,  1399,-32768,   211,  1419,  7109,
  -32768,  8048,  1419,  1467,  1472,-32768,-32768,  4670,  1247,   429,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1243,  5067,  1470,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    87,
  -32768,-32768,  1309,   744,-32768,-32768,   726,   656,  1037,-32768,
  -32768,   456,-32768,-32768,-32768,   744,-32768,   933,  1536,-32768,
     480,-32768,  5067,   790,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   117,-32768,  1401,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   558,   558,  1312,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   209,   170,   450,-32768,  1542,-32768,-32768,
  -32768,-32768,  7109,-32768,-32768,-32768,-32768,    27,   752,  1395,
    1418,  1267,-32768,  1194,   765,-32768,   512,-32768,  1263,  7109,
     580,  1268,  1276,   596,  1277,  1550,   542,  1401,   933,  1282,
    3008,-32768,  1252,  1371,  2725,-32768,   596,  1432,  1522,-32768,
    3979,-32768,   596,   420,-32768,-32768,  1525,-32768,-32768,  1284,
    1289,-32768,  5067,  5067,-32768,  5067,  5067,-32768,  1571,   160,
     744,  1292,-32768,-32768,  1293,   744,  6422,-32768,-32768,  1526,
  -32768,   519,  1295,  1297,   744,  1421,  1304,  1305,   744,  8048,
    8048,  1207,  8048,  7506,  1306,  1315,   744,  1316,  1318,   744,
    8048,   744,  1319,  1324,   744,  1326,  1327,   744,  1328,  1329,
     744,-32768,  9241,-32768,   939,   960,-32768,   980,-32768,  1030,
    8048,-32768,  1051,  1066,  1074,  1107,  1127,  1168,  8048,-32768,
    8048,  8048,   744,  1545,   185,   744,  1420,  8048,-32768,  8048,
    8048,  8048,  8048,-32768,-32768,-32768,-32768,-32768,-32768,  8048,
  -32768,  8048,   831,-32768,   784,  5483,  6838,-32768,-32768,  7109,
  -32768,-32768,  7109,  8411,-32768,-32768,  7109,  7109,  7109,  7109,
    7109,  7109,  1408,  1331,-32768,  1342,-32768,-32768,  1408,-32768,
     407,   407,    84,-32768,-32768,   799,-32768,  9153,  9153,  1345,
     218,  1347,  5754,  1483,-32768,  1483,   801,  1483,  1558,  1559,
    8048,  1424,  1608,  1562,-32768,-32768,-32768,   701,  1401,-32768,
    1252,  1371,-32768,   559,  1194,  1516,  1194,-32768,   133,-32768,
  -32768,-32768,-32768,  1505,  8048,   181,  1504,  1312,   749,-32768,
  -32768,  1530,  1531,  1534,  1537,  1540,  1547,-32768,-32768,-32768,
  -32768,-32768,  1367,    27,-32768,  1474,-32768,-32768,   111,-32768,
     515,   111,-32768,   111,-32768,  1418,-32768,-32768,  1381,-32768,
  -32768,   656,  1505,  1194,-32768,  1282,-32768,  1542,  1654,-32768,
  -32768,  1389,-32768,  1171,-32768,  1392,   691,   157,  1396,-32768,
  -32768,   691,-32768,-32768,-32768,  1571,-32768,  1571,  8048,  1037,
    1037,  1397,   230,   816,  8048,  1622,-32768,  1037,  1037,  8048,
    1037,  1037,   196,   744,-32768,  1442,  8048,   744,  1037,  1037,
    1037,  1037,   744,  1037,  1037,  1037,  1037,  1037,  1037,-32768,
    1398,  1411,   618,  7109,-32768,-32768,  1406,  9169,-32768,-32768,
    1193,-32768,-32768,-32768,-32768,-32768,-32768,  1255,   688,-32768,
  -32768,  8048,  8048,  1638,  1528,  1279,   820,  1230,  1343,  1364,
    8048,-32768,-32768,-32768,-32768,  9169,  1413,-32768,-32768,  1670,
    1676,  7109,   120,-32768,  1427,  1631,  8048,-32768,  1423,  6026,
    7109,  7109,-32768,-32768,-32768,  1682,  8295,-32768,  1426,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     136,-32768,-32768,-32768,-32768,-32768,-32768,  1652,-32768,-32768,
  -32768,-32768,-32768,-32768,  1440,   555,-32768,-32768,-32768,-32768,
  -32768,-32768,  1673,-32768,  9153,  9153,-32768,  1566,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1466,-32768,-32768,  1737,-32768,
     429,-32768,-32768,-32768,  1664,-32768,  5754,  5754,   823,  7109,
    8048,  5067,  1095,-32768,  1505,  1542,-32768,-32768,-32768,-32768,
     921,-32768,-32768,-32768,  3624,  3624,-32768,-32768,   744,  1728,
     181,-32768,   203,  7109,-32768,  1675,  1575,  1680,  1582,  1717,
    1684,  1721,  1401,-32768,-32768,  1473,-32768,  1477,  1674,  1728,
     515,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1401,
    1252,-32768,   921,-32768,  1648,-32768,-32768,-32768,  1663,    98,
  -32768,-32768,  1418,  1665,   744,  1479,  1490,-32768,   701,-32768,
     744,  8048,  1492,  1493,   744,  1495,  1499,  8048,  8048,   744,
    1507,  1508,  1513,  1514,  1517,  1519,  1523,  1524,  1527,  1529,
  -32768,  9241,  1484,-32768,  7109,-32768,-32768,-32768,-32768,-32768,
  -32768,  8048,  1532,-32768,-32768,-32768,  8048,-32768,-32768,  8048,
  -32768,-32768,  1379,   186,-32768,  1759,  8048,  8048,  5067,   744,
  -32768,-32768,-32768,-32768,  7109,-32768,   189,-32768,  1633,-32768,
  -32768,   153,  1539,  5483,-32768,  1700,-32768,-32768,-32768,-32768,
     407,  8048,-32768,  7109,  5754,-32768,   718,-32768,-32768,  1535,
  -32768,   744,   232,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1541,  7109,-32768,-32768,  1747,  7109,  1312,
  -32768,-32768,-32768,   558,  7109,-32768,-32768,-32768,-32768,-32768,
  -32768,  1505,-32768,-32768,-32768,-32768,-32768,  1505,  1542,-32768,
  -32768,-32768,-32768,  8048,   691,-32768,  1634,-32768,   691,-32768,
  -32768,-32768,   744,-32768,-32768,-32768,-32768,   744,   744,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    7109,  1760,   744,-32768,-32768,  1390,-32768,  3624,  3624,-32768,
    1549,   744,  1449,   135,-32768,  1758,-32768,  7109,  7109,    27,
     112,-32768,-32768,  1597,  1762,  5067,  1221,-32768,-32768,   102,
    1552,-32768,-32768,  5067,  1554,-32768,-32768,-32768,  8048,-32768,
  -32768,-32768,-32768,  1312,   558,-32768,  1727,-32768,-32768,-32768,
  -32768,   744,-32768,-32768,-32768,  1556,  1563,  1564,-32768,-32768,
  -32768,-32768,  1037,  5483,  1683,  5483,-32768,-32768,-32768,-32768,
    8546,   189,  8048,  5483,   146,  5483,  7109,-32768,   232,  7109,
  -32768,-32768,  1727,   187,  1312,  1738,  1739,-32768,-32768,  1568,
  -32768,  7109,  7109,  1569,  1221,  8546,  1221,-32768,-32768,  1509,
     121,  1819,  1221,   834,-32768,   845,  1312,   446,   446,-32768,
    1825,-32768,  1690,-32768,-32768,-32768,  1586,-32768,  1831,  5483,
    1846,  1837,  8546,  7109,-32768,-32768,-32768,  1685,   526,-32768,
  -32768,-32768,-32768,  8546,   159,  7109,  8546,-32768,-32768,-32768,
  -32768,-32768,-32768,  8546,-32768,-32768,-32768,  1885,  1891,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1301,  1730,-32768,  1731,  1570,   494,-32768,
    1029,-32768,-32768,-32768,  -973,-32768,  -645,-32768,  -478,  1734,
    1576,  1310,  1046,-32768,-32768,-32768,   636,-32768,  1147,   899,
  -32768,-32768,   647,-32768,-32768,   895,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   657,-32768,-32768,  -318,   927,-32768,-32768,
     220,-32768,   236,  -400,  1841,-32768,-32768,  -410,-32768,-32768,
  -32768,  1468,-32768,-32768,  1222,  -423,-32768,-32768,-32768,   732,
  -32768, -1060,-32768,-32768,  1544,  1019,   668,  1494,   987,   669,
  -32768,   237,   -57,  -315,-32768,  -381, -1399,-32768,-32768,   254,
  -32768,  1498, -1263,-32768,-32768,-32768,-32768, -1372,   -24,   -22,
     -23,   -42,-32768,-32768,-32768,-32768,  -151,  -337,-32768,  1021,
  -32768,  -744,-32768, -1065,-32768,   819,-32768,-32768,-32768,-32768,
  -32768, -1477, -1079,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -811, -1027,
  -32768,-32768,-32768,-32768,-32768,   169,-32768,    -3,-32768,-32768,
  -32768,-32768,-32768,  1958,-32768,-32768,-32768,-32768,-32768,  1109,
  -32768,-32768,  -802,  -610,  -998,   715,   530,-32768,-32768,-32768,
    -640,  -874,-32768,-32768, -1138, -1352,-32768,-32768,-32768,  1543,
  -32768,  1281,  1538,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1551,-32768,-32768,-32768,-32768,
  -32768,  -643,  -533,-32768,   978,   502,  -653,  -438,-32768,-32768,
  -32768,-32768, -1123,-32768,  -434,-32768,-32768,-32768,-32768,  1228,
  -32768,-32768,   995,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1964,  1812,-32768,-32768,-32768,-32768,  1620,
  -32768,-32768,-32768,  1373,-32768,  1369,-32768,-32768,-32768,-32768,
  -32768,-32768,  1094,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  -763,     1,-32768,-32768,-32768,  -504,   549,-32768,  -331,
     537,-32768,  -234,   520,-32768,-32768,-32768,-32768,  1816,-32768,
  -32768,-32768,  1110, -1256,-32768,-32768, -1032, -1455,-32768,-32768,
  -32768,-32768,   842,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1900,   -59,   575,  -274,-32768,-32768,    74,-32768,  -441,
    -842,  1201,-32768,   246,-32768,   453,-32768,  -806,-32768,  -700,
  -32768,  1084,  -604,-32768,-32768,  -952,  1999,  2000,-32768,-32768,
    2001,-32768,-32768,-32768,  1382,-32768,  -268,-32768,-32768,  1134,
   -1274,-32768,   870,-32768,  -108,  -808,  -627,-32768,-32768,-32768,
    -791,   -49,  -528,  -782,-32768,  -614,-32768,-32768,-32768,   679,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -616,
  -32768,   706,-32768,   244,  -890,  1511,-32768,  -503,-32768,   825,
    -888,  -158,  -876,  -860,  -851,   -54,    -2,    -1,-32768,   -71,
   -1180,  -553,  -140,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768, -1285,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   660,-32768, -1225,-32768,-32768,
     141,-32768,  -849,   444,  1048,   777,   201,  -735,  1344, -1342,
  -32768,   -66,  1933,-32768,   -27,   -52, -1740,   439,  -290,     4,
     -60,  -165,   -10,   -61,  -294,-32768,   -84,   -55,    -9,-32768
};


#define	YYLAST		9610


static const short yytable[] =
{
     158,    43,    85,   250,   271,   321,   178,   179,   245,   187,
     340,   343,   266,   374,   264,   283,   281,   311,   913,   288,
     267,   735,   665,  1106,   831,   739,   302,   308,   341,   344,
    1081,   583,   709,  1195,   846,  1197,  1089,   256,   939,   310,
    1579,   700,   312,  1108,   306,   671,   915,  1198,   239,   242,
    1384,   246,   249,   179,   254,   257,   883,  1172,   923,   865,
     269,   242,   257,  1199,   249,   248,   179,   254,   305,   286,
     158,   257,  1200,   278,  1201,  1684,   313,   276,  1374,   298,
     178,   179,   301,   242,   867,   298,   257,   246,   249,   309,
     179,   254,   286,   158,   257,  1055,   249,   278,  1623,   678,
     385,   307,   328,  1333,  1178,   628,   669,   601,  1127,   314,
    1123,   327,  1017,   329,   377,  1410,   644,  1466,  1008,  1787,
     675,   988,   383,   381,  1487,   465,  1559,  1641,   752,  1102,
    1205,  2000,  1563,   386,  1262,  1713,  1215,  1282,   925,  1212,
    1443,  1033,   929,  1033,   968,   990,   351,  1179,   940,   345,
    1373,   269,   179,   269,  1033,  1033,  1023,   158,  1022,  1950,
    1440,  1398,   595,  1030,   257,  1659,   654,  1174,  1508,   375,
     422,   242,   249,   179,   254,   286,  1719,   472,  1266,   187,
     956,  1033,  1682,   837,  1955,   379,   899,   429,   838,  1059,
     -81,   612,  1482,  1033,   614,  1855,   829,  1791,  1896,  1257,
     879,  1805,   837,  -239,  1737,   173,  2023,   838,   631,  1903,
     893,   679,   325,   837,  1453,   423,  1984,   460,   838,   837,
    1450,   614,  1644,    83,   838,   663,    66,   975,   633,  1887,
    1424,  1987,  1336,   918,   105,   888,   995,   837,  1082,  1083,
    1033,   843,   838,   735,  1897,   837,  1377,   739,  1782,  1783,
     838,  1287,  1288,  -714,  1283,  1898,   290,   837,  1034,   860,
    1034,   105,   838,  2018,   922,  1177,   571,   829,   837,   575,
    1285,  1034,  1034,   838,  1400,  1820,  1821,   636,   704,   477,
    1236,  1425,  1413,   945,   709,  1696,   859,   957,   837,  1451,
     291,   615,  1665,   838,  1106,  1414,   903,   621,  1034,  -239,
     610,   583,  1354,  1430,   889,   174,   269,   337,    14,   269,
    1034,  1623,   175,  1815,  1108,    84,   584,   158,  1415,  1203,
    -239,  1698,   424,   632,   629,   654,  -239,  1792,  1856,  1210,
     326,   655,  1416,  1110,   761,   960,  1763,  1764,  1267,   958,
     425,  1120,  1827,   246,   996,   616,  1351,   179,  1828,  1912,
    1454,   976,  1707,   106,  1899,  1227,   193,  1034,  1561,   861,
    -239,  1720,   617,  1664,  1677,   613,  1777,  1988,   466,  1417,
    1273,   961,   616,  1428,   426,    90,   670,  1084,  1357,  1358,
    1650,   898,  1829,   339,   162,  1401,   571,  1286,   267,  1452,
     686,   326,  1803,   690,   843,   747,   643,   643,   701,  1243,
     703,  1441,   751,  1711,  1951,  1446,  1447,  1022,   194,  1281,
    1438,  1022,   758,  1375,   189,   756,   269,  1682,  1432,   837,
     991,   760,   631,  1284,   838,  1220,   269,  1673,   269,  1944,
     269,  1244,  1060,   691,  1646,  1647,  1159,   188,   837,  1362,
    1981,   894,  1895,   838,  1676,  1274,   829,   839,   840,   841,
     842,  1375,   242,   249,   254,   837,  1492,  1925,   180,   467,
     838,  1693,   179,   337,   246,  1236,   749,   769,   841,   842,
    1888,   962,  1427,  1977,   767,  1663,   433,   839,   840,   841,
     842,  1418,  -714,   839,   840,   841,   842,   837,  1940,  1941,
     705,  1715,   838,  1413,  1455,  1369,  1930,   337,  1998,   755,
    1504,  1505,   666,  1506,  1507,  1263,  1414,   663,   190,   839,
     840,   841,   842,   337,  -754,   667,  1279,  1359,   856,   833,
    1280,   839,   840,   841,   842,  2017,  1908,   863,   874,  1415,
    1456,   868,   839,   840,   841,   842,  2022,   663,   629,  2025,
     881,  1884,   872,  1416,   181,   443,  2026,   942,  1309,   434,
     907,  1961,   839,   840,   841,   842,   461,  1360,   533,   969,
     338,  1237,  1310,  2007,  1572,  1515,   339,   583,   179,  2020,
     249,   584,  1848,   243,   337,   182,   905,   179,  1187,   183,
    1417,   179,   599,   858,  1573,  1814,   584,   909,   187,   160,
     184,   607,  1989,   986,   342,   191,   575,  1444,   999,  1857,
     339,   966,  2008,    83,   919,   882,  1311,  1710,   260,   261,
    1818,  1404,  1167,  1516,  2006,   675,  1481,  1017,   444,   926,
    2021,  1180,   897,   345,   943,  1806,   179,  1841,  1168,   970,
    1171,  1270,   941,  1698,  1312,  1574,   269,  1493,  1405,  1218,
    1145,  1123,   462,  1409,  1847,   108,   837,  1188,   971,  2009,
    1499,   838,  1850,   161,   650,   260,   261,   258,   653,   987,
     450,   692,  1255,   346,   944,  1623,   967,   411,  1816,  1109,
     934,   693,   269,  2024,  1431,  1819,   934,   463,  1709,  1822,
    1823,   110,  1500,   839,   840,   841,   842,   432,  1361,  1299,
    1623,  1124,  1418,   998,   451,    84,   452,  1752,  1313,   278,
    1002,  1003,   839,   840,   841,   842,   339,   412,   267,  1103,
     345,   837,   274,  1849,   258,  1851,   838,  1623,   453,   839,
     840,   841,   842,    87,    88,   345,  1370,   850,  1623,   851,
     295,  1623,  1753,   631,   345,   345,    14,  1300,  1623,   475,
     837,   335,  1164,   454,  1754,   838,   111,   480,   269,  1761,
    1056,   839,   840,   841,   842,   303,   112,   813,  1146,   837,
     290,  1301,  1928,   866,   838,  1064,  1906,   837,  1929,   826,
     827,   830,   838,  1062,  1066,  1068,  1861,  1434,   168,   169,
    1350,   113,  1413,   837,  1195,  1195,  1197,  1197,   838,   885,
    1667,   257,    83,  1513,   291,  1414,  1050,   916,  1198,  1198,
    1668,   571,   337,   571,  1471,  1132,   837,  1079,   357,  1472,
    1143,   838,  1728,   663,  1199,  1199,  1919,  1920,  1415,  1051,
    1813,   664,   293,  1200,  1200,  1201,  1201,  1164,  1112,  1163,
    1674,  1675,  1416,   621,   837,  1170,   269,   337,   315,   838,
    1913,   269,   873,   269,  1685,   269,   875,  1692,  1483,   629,
   -1084,   318,   584, -1088,   837,   267,   909,   322,  1566,   838,
     179,   249,   602,  1186,  1228,  1413,   854,   249,   855,  1417,
     608,  1162,  1471,  1202,   858,  1052,   323,  1475,  1414,  1812,
     317,   571,   837,   179,   324,   179,  1270,   838,  1222,   437,
     260,   261,   438,   269,  1570,   269,   930,   951,   931,   952,
     330,  1415,   643,   837,  1758,   843,  1216,  1104,   838,  1221,
     839,   840,   841,   842,   169,  1697,  1974,  1658,  1976,  1233,
    1706,   269,  1251,  1252,   416,  1254,  1980,  1139,  1982,  1140,
     331,  1967,  1968,  1969,   439,   352,   417,  1057,   373,  1061,
    1109,  1256,   408,  1065,  1067,  1069,   269,   407,   651,   269,
     652,  1264,  1417,   410,  1260,  1547,  1261,  1914,   278,  1013,
     414,  1265,   837,  1226,   430,  1512,   769,   838,   843,   418,
    1378,   419,  2014,  1234,   631,   839,   840,   841,   842,   435,
    1241,  1418,  1106,   837,   447,   496,   497,   498,   838,  1796,
    1798,   179,   278,   278,   449,   278,  1893,  1195,  1195,  1197,
    1197,   179,  1108,   837,   839,   840,   841,   842,   838,   769,
    1278,  1198,  1198,   506,   507,  1379,   767,  1380,  1260,   446,
    1429,  1032,   458,   839,   840,   841,   842,  1199,  1199,   469,
    1341,   839,   840,   841,   842,   471,  1200,  1200,  1201,  1201,
     473,  1058,  1773,   476,  1463,  1063,  1464,   839,   840,   841,
     842,  1229,  1230,   837,  1364,  1080,   477,  1468,   838,  1469,
    1087,  1088,   918,  1363,  1091,  1092,  1093,  1094,  1095,  1099,
     839,   840,   841,   842,   837,   572,  1375,  1365,  1571,   838,
     533,   257,  1113,  1114,  1115,  1116,  1117,  1118,   533,   837,
     629,   843,  1626,  1645,   838,  1629,   576,   837,   839,   840,
     841,   842,   838,  1164,   579,  1366,  1853,   586,   843,   735,
    1729,   587,   843,   739,  1768,   843,   590,  1807,   839,   840,
     841,   842,   607,  1954,   584,   584,  2003,   909,  2004,   591,
     837,  1958,  1106,   592,  1106,   838,   593,   854,   249,  2005,
     594,  1653,  1106,  1655,  1106,  1173,   839,   840,   841,   842,
     837,   596,  1108,  1367,  1108,   838,  1797,  1799,   650,   609,
    1185,   611,  1108,   622,  1108,  1995,  1996,   839,   840,   841,
     842,   623,   624,   625,  1368,   635,   639,   637,  1512,  1512,
     640,   641,  1478,   645,   647,   659,  1512,  1512,  1106,  1512,
    1512,   837,    14,   660,   661,  1494,   838,  1512,  1512,  1512,
    1512,  1498,  1512,  1512,  1512,  1512,  1512,  1512,  1108,   668,
    1109,   267,   673,   684,   687,   575,   837,   698,   631,   697,
     702,   838,   757,  1461,   753,   759,   839,   840,   841,   842,
     794,   799,   806,  1544,   807,  1462,   796,   797,   798,   800,
    1461,   801,  1639,  1639,  1643,   802,   803,   839,   840,   841,
     842,   269,   278,   837,  1545,   269,   819,   804,   838, -1070,
     805,   808,   811,   853,   816,   278,   817,   839,   840,   841,
     842,   278,   818,   834,  1548,   820,   821,   822,   837,  1289,
    1290,  1295,  1831,   838,  1304,  1305,  1308,   823,  1316,  1319,
    1321,  1324,  1327,  1330,  1670,   824,  1672,   825,  1909,   836,
    1334,  1335,   837,  1337,   844,  1339,  1630,   838,  1342,  1343,
    1344,  1345,  1346,  1347,   843,   845,   870,   839,   840,   841,
     842, -1088,  1543,   179,  1549,  1575,  1352,  1628,   849,  1355,
     571,  1631,  1985,   267,   629,   852,   876,   878,   839,   840,
     841,   842,   887,  1712,  1372,  1551,   896,   892,   901,   904,
     902,   906,   910,   839,   840,   841,   842,   914,   911,   663,
    1552,   839,   840,   841,   842,   917,   837,   269,  1553,   927,
     269,   838,   654,  1580,  1625,   920,   932,   584,   249,   254,
     584,   286,   158,   955,   959,   533,  -441,   837,   964,   972,
     337,  1627,   838,   973,   839,   840,   841,   842,   584,   584,
     980,  1554,   837,   249,   978,   979,   981,   838,   983,   984,
     993,   994,  1000,   837,   839,   840,   841,   842,   838,   989,
    1001,  1555,  1010,  1012,  -744,  1013, -1113,  1018,  1019,  1020,
    1024,   631, -1113,  1085,   879,   193,  1090, -1113,  1101,   990,
   -1113,   339,  1718,  1128,   837,  1133,  1134,  1723,  1135,   838,
    1141,  1137,  1165,   861,  1158,   839,   840,   841,   842,  1181,
    1175,  1695,  1556,   854,  1183,   837,  1192,  1206,   935,  1207,
     838,  1213,   837,  1214,  1217,  1519, -1113,   838,  1223,  1224,
     839,   840,   841,   842, -1113,  -744,  1225,  1759,  -744,  1232,
    1239,  1249, -1113,  1253,  1808,  -744,  1738,  -744,  1258,  1272,
    1296,  -744,  1331,  1332,   837,  1338,  -744,  1348,  1340,  -744,
   -1113,  1349, -1113,  1375,  1381,  1776,  1376,   839,   840,   841,
     842,  1383,  1769,  1399,  -744, -1113,  1402, -1113, -1113, -1113,
    1407,  1943,   837,  1543,   179,  1408,  1420,   838,  -744,  1412,
     533,  1422,   839,   840,   841,   842,  -744,   629,  1435,  1760,
    -744,  1448,  1457,  1522,  1523,  1465,  1525,  1527,  1473,  -744,
    1480,  1467,  1476,  1488,  1532,  1266,   839,   840,   841,   842,
    1477,  1479,   242,  1767,  1496,  -744, -1113,  1501,  1502,  1033,
    1781,  1278,  1278,  1503,  1550,  1509,  1510,  1625,  1517,  1514,
    1518,  1999,  1557,  1562,  1558,  1352,  1560,  1520,  1521,  1528,
    1632,  1352,  -744,  1355,  1565,   533,  1567,   -49,  1529,  1530,
    1651,  1531,  1533,  1568,  -744,  1569,  -744,  1534,    56,  1535,
    1536,  1537,  1538,  1634,  1833,   584,   584,  1660,  1109,    57,
     839,   840,   841,   842,  1635,  1661,    58,  1770,  1911,  1648,
      59,  1649,  1639,  1656,  1657,   -49,  1671,   249,   249,  1683,
     375,   839,   840,   841,   842,  1686,  1687,   -49,  1771,  1688,
      60,  1694,  1689,  -744,   533,  1690,   839,   840,   841,   842,
    1236,   163,  1691,  1886,   179,  1708,  1714,   839,   840,   841,
     842,   -49,  1716,  -744,  1939,  1731,  1717,  -744,  1678,  1765,
    1722,  1727,  1750, -1113, -1113, -1113, -1113, -1113, -1113, -1113,
    1757,  1766,  1881,  1751,  1774, -1113,  1775, -1113,   839,   840,
     841,   842, -1076,   161,  -341,  -744,  -744,  1790,  -744,  1303,
    1778,  1307,    61,  1315,  1318,  1926,  1323,  1326,  1329,   839,
     840,   841,   842,  1785,  1793,  1794,   839,   840,   841,   842,
    1800,  1543,   179,  1795,  1801,   246,   -49,   879,    68,  1804,
     353,  1824,  1724,    62,  1834,  1835,   -49,    63,  1730,    69,
    1836,  1838,  1837,  1734,  1839,  1840,    70,  1842,  1844,  1924,
    1739,  1843,  1852,  1859,  1854,  1278,  1858,  1880,  1109,    71,
    1109,   -49,  1927,   354,  1860,   631,  1863,  1864,  1109,  1865,
    1109,   107,  1904,  1866,   375,   249,   839,   840,   841,   842,
    1932,  1869,  1870,   164,  1934,  1352,  1352,  1871,  1872,   355,
     631,  1873,   356,  1874,  1772,  1112,  1962,  1875,  1876,   179,
    1890,  1877,  1786,  1878,  1907,   246,  1883,  1918,   357,   108,
    1779,  1946,  1905,  1923,  1109,  1921,    72,   631,  1933,  1937,
    1945,   109,  1942,  1952,  1953,  1956,    64,  1959,   631,  1963,
    1970,   631,    73, -1114,   358,  1971,  1975,  1972,   631, -1114,
    1990,  1992,  1994,  1997, -1114,   110,  2002, -1114,    74,  1987,
    1988,   246,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    2012,  2013,  2015,    75,  2016,  2028,  2019,    76,   242,  1948,
      77,  2029,  1636,   578,   869,   332,   333,  1949,    78,  1144,
     334,   629,   585, -1114,  1811,   871,  1497,  1136,  1011,   359,
     575, -1114,  1271,  1276,  1495,  1245,  1485,    79,  1830, -1114,
    1846,   289,  1437,  1176,   710,   634,   629,  1211,  1486,   974,
     111,   680,   360,  1491,  1826,   682,  1845, -1114,  1986, -1114,
     112,  1625,  1991,  1973,  1993,   361,  2011,  1162,  1978,  1385,
     269,  1204, -1114,   629, -1114, -1114, -1114,  1902,    28,  1125,
    1470,  1666,   246,   246,   629,   113,  1625,   629,   954,  1250,
     672,   662,   997,  1705,   629,  1862,    80,   167,  1242,   336,
     688,  1867,  1868,   626,   886,   891,  1166,   350,  1160,  1411,
     299,  1654,  1960,  1625,  1162,  1053,  1756,  1879,  1182,    48,
      49,    52,  1142, -1114,  1625,  1882,  1278,  1625,  1291,   900,
    1352,  1292,  1406,  1885,  1625,   481,   810,  1524,  1439,  1564,
    1891,  1892,   482,  1235,    91,  1922,  1474,   304,  1780,   950,
       0,    92,     0,     0,   483,    93,    94,     0,     0,     0,
     484,   485,     0,  1035,    95,   533,     0,     0,     0,   837,
       0,     0,     0,     0,   838,    96,     0,  1036,  1725,  1726,
       0,     0,     0,     0,     0,     0,  1732,  1733,   486,  1735,
    1736,   487,     0,     0,     0,     0,     0,  1740,  1741,  1742,
    1743,     0,  1744,  1745,  1746,  1747,  1748,  1749,     0,     0,
       0,     0,     0,  1037,     0,     0,   488,  1931,   489,   490,
       0,  1038,     0,     0,     0,     0,     0,     0,   491,  1039,
       0,     0,   492,     0,     0,     0,     0,    97,     0,     0,
   -1114, -1114, -1114, -1114, -1114, -1114, -1114,  1040,     0,  1041,
       0,     0, -1114,    98, -1114,     0,     0,     0,     0,     0,
       0,     0,  1042,     0,  1043,  1044,  1045,     0,    99,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1811,   101,     0,  1293,     0,   493,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,   102,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1046,     0,     0,  1979,   496,   497,   498,
     499,   260,   261,   115,     0,   116,   117,   118,   119,     0,
     120,   500,     0,     0,     0,     0,     0,     0,   121,   122,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   123,
     510,     0,   125,     0,     0,   511,   512,     0,   513,   514,
     515,   103,   128,   129,   516,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   517,   140,     0,   518,   142,   143,     0,
     144,     0,   519,   520,   521,     0,   522,   523,     0,   524,
     145,   146,   147,   148,   149,   150,     0,     0,     0,   525,
     526,  1302,     0,     0,  1292,   598,     0,   528,   481,     0,
       0,   339,     0,     0,     0,   482,     0,     0,     0,     0,
    1047,  1048,  1049,   839,   840,   841,   842,   483,     0,     0,
    1103,     0, -1115,   484,   485,     0,     0,     0, -1115,     0,
       0,     0,     0, -1115,     0,     0, -1115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,     0,     0,   487,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0, -1115,     0,     0,     0,     0,     0,     0,   488,
   -1115,   489,   490,     0,     0,     0,     0,     0, -1115,     0,
       0,   491,     0,     0,     0,   492,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0, -1115,     0, -1115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0, -1115,     0, -1115, -1115, -1115,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1293,     0,
     493,     0,     0,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0, -1115,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,   499,   260,   261,   115,     0,   116,   117,
     118,   119,     0,   120,   500,     0,     0,     0,     0,     0,
       0,   121,   122,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   123,   510,     0,   125,     0,     0,   511,   512,
       0,   513,   514,   515,     0,   128,   129,   516,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   517,   140,     0,   518,
     142,   143,     0,   144,     0,   519,   520,   521,     0,   522,
     523,     0,   524,   145,   146,   147,   148,   149,   150,     0,
       0,     0,   525,   526,  1306,     0,     0,  1292,   598,     0,
     528,   481,     0,     0,   339,     0,     0,     0,   482, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115,     0,     0,     0,     0,
     483, -1115,     0, -1115,     0,  1035,   484,   485,     0,     0,
       0,   837,     0,     0,     0,     0,   838,     0,     0,  1036,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,     0,     0,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1037,     0,     0,     0,     0,
       0,     0,   488,  1038,   489,   490,     0,     0,     0,     0,
       0,  1039,     0,     0,   491,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1040,
       0,  1041,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1042,     0,  1043,  1044,  1045,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1293,     0,   493,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,   935,     0,     0,  1046,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   517,
     140,     0,   518,   142,   143,     0,   144,     0,   519,   520,
     521,     0,   522,   523,     0,   524,   145,   146,   147,   148,
     149,   150,     0,     0,     0,   525,   526,  1314,     0,     0,
    1292,   598,     0,   528,   481,     0,     0,   339,     0,     0,
       0,   482,  1047,  1048,  1049,   839,   840,   841,   842,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,     0,     0,
     487,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   488,     0,   489,   490,     0,
       0,     0,   123,   124,     0,   125,     0,   491,     0,     0,
       0,   492,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,   146,   147,   148,   149,   150,     0,
       0,     0,     0,     0,  1293,     0,   493,     0,   318,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,   679,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,   499,
     260,   261,   115,     0,   116,   117,   118,   119,     0,   120,
     500,     0,     0,     0,     0,     0,     0,   121,   122,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   123,   510,
       0,   125,     0,     0,   511,   512,     0,   513,   514,   515,
       0,   128,   129,   516,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   517,   140,     0,   518,   142,   143,     0,   144,
       0,   519,   520,   521,     0,   522,   523,     0,   524,   145,
     146,   147,   148,   149,   150,     0,     0,     0,   525,   526,
    1317,     0,     0,  1292,   598,     0,   528,   481,     0,     0,
     339,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,     0,     0,   487,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   488,     0,
     489,   490,     0,     0,     0,   123,   124,     0,   125,     0,
     491,     0,     0,     0,   492,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,     0,     0,     0,     0,     0,  1293,     0,   493,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   685,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   260,   261,   115,     0,   116,   117,   118,
     119,     0,   120,   500,     0,     0,     0,     0,     0,     0,
     121,   122,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   123,   510,     0,   125,     0,     0,   511,   512,     0,
     513,   514,   515,     0,   128,   129,   516,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   517,   140,     0,   518,   142,
     143,     0,   144,     0,   519,   520,   521,     0,   522,   523,
       0,   524,   145,   146,   147,   148,   149,   150,     0,     0,
       0,   525,   526,  1322,     0,     0,  1292,   598,     0,   528,
     481,     0,     0,   339,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
       0,     0,     0,     0,     0,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,     0,     0,   487,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   488,     0,   489,   490,     0,     0,     0,   123,   124,
       0,   125,     0,   491,     0,     0,     0,   492,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
     146,   147,   148,   149,   150,     0,     0,     0,     0,     0,
    1293,     0,   493,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,   499,   260,   261,   115,     0,
     116,   117,   118,   119,     0,   120,   500,     0,     0,     0,
       0,     0,     0,   121,   122,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   123,   510,     0,   125,     0,     0,
     511,   512,     0,   513,   514,   515,     0,   128,   129,   516,
     130,     0,   484,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   517,   140,
       0,   518,   142,   143,     0,   144,     0,   519,   520,   521,
     486,   522,   523,     0,   524,   145,   146,   147,   148,   149,
     150,     0,     0,     0,   525,   526,  1325,     0,     0,  1292,
     598,     0,   528,   481,     0,     0,   339,     0,   488,     0,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,     0,     0,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,   488,     0,   489,   490,     0,     0,
       0,     0,     0,     0,     0,   495,   491,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,     0,   260,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,   502,   503,   504,   505,   506,   507,   508,
     509,     0,     0,     0,     0,     0,     0,   511,   512,     0,
       0,     0,     0,  1293,     0,   493,   516,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,   499,   260,
     261,   115,     0,   116,   117,   118,   119,     0,   120,   500,
       0,     0,  1193,     0,     0,     0,   121,   122,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   123,   510,     0,
     125,     0,     0,   511,   512,     0,   513,   514,   515,     0,
     128,   129,   516,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   517,   140,     0,   518,   142,   143,     0,   144,     0,
     519,   520,   521,     0,   522,   523,     0,   524,   145,   146,
     147,   148,   149,   150,     0,     0,     0,   525,   526,  1328,
       0,     0,  1292,   598,     0,   528,   481,  1014,     0,   339,
     712,     0,     0,   482,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1015,     0,   483,     0,   713,     0,     0,
     714,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
       0,     0,   487,     0,     0,     0,     0,     0,     0,     0,
     715,     0,     0,     0,     0,     0,   716,   717,   718,     0,
     719,   720,   721,     0,   722,     0,     0,   488,     0,   489,
     490,     0,     0,     0,     0,     0,     0,     0,     0,   491,
       0,     0,   723,   492,   724,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,     0,     0,     0,     0,  1293,     0,   493,     0,
       0,     0,     0,     0,   494,     0,     0,     0,     0,     0,
       0,     0,   727,     0,   495,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,   499,   260,   261,   115,     0,   116,   117,   118,   119,
       0,   120,   500,     0,     0,     0,     0,   728,   729,   121,
     122,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     123,   510,     0,   125,   730,     0,   511,   512,     0,   513,
     514,   515,     0,   128,   129,   516,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   517,   140,     0,   518,   142,   143,
       0,   144,     0,   519,   520,   521,     0,   522,   523,     0,
     524,   145,   146,   147,   148,   149,   150,   481,     0,     0,
     525,   526,     0,     0,   482,     0,   598,     0,   528,     0,
       0,     0,   339,     0,     0,     0,   483,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,     0,     0,     0,     0,     0,     0,     0,     0,
     486,     0,     0,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   488,     0,
     489,   490,     0,     0,     0,     0,     0,     0,     0,     0,
     491,     0,     0,     0,   492,     0,     0,     0,     0,     0,
       0,     0,  1025,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,   773,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,  1121,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   260,   261,   115,     0,   116,   117,   118,
     119,     0,   120,   500,     0,     0,     0,     0,     0,     0,
     121,   122,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   123,   510,     0,   125,     0,     0,   511,   512,     0,
     513,   514,   515,     0,   128,   129,   516,   130,     0,     0,
     131,   132,   133,   134,   135,  1026,  1027,  1028,   136,   137,
       0,     0,     0,     0,   138,   517,   140,     0,   518,   142,
     143,     0,   144,     0,   519,   520,   521,     0,   522,   523,
       0,   524,   145,   146,   147,   148,   149,   150,   481,     0,
       0,   525,   526,     0,     0,   482,     0,   777,     0,   528,
       0,     0,     0,   339,     0,     0,     0,   483,     0,     0,
     770,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   771,     0,     0,     0,     0,     0,     0,     0,
       0,   486,     0,     0,   487,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   488,
       0,   489,   490,     0,     0,     0,   123,   124,     0,   125,
       0,   491,     0,     0,     0,   492,   126,   127,     0,   128,
     129,     0,   130,   772,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,   773,     0,     0,     0,  1122,     0,
     493,     0,     0,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,   499,   260,   261,   115,     0,   116,   117,
     118,   119,     0,   120,   500,     0,     0,     0,     0,     0,
       0,   121,   122,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   123,   510,     0,   125,     0,     0,   511,   512,
       0,   513,   514,   515,     0,   128,   129,   516,   130,     0,
       0,   131,   132,   774,   775,   776,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   517,   140,     0,   518,
     142,   143,     0,   144,     0,   519,   520,   521,     0,   522,
     523,     0,   524,   145,   146,   147,   148,   149,   150,   481,
       0,     0,   525,   526,     0,     0,   482,     0,   777,     0,
     528,     0,     0,     0,   339,    14,     0,     0,   483,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   771,     0,     0,     0,     0,     0,     0,
       0,     0,   486,     0,     0,   487,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
     488,     0,   489,   490,     0,     0,     0,   123,   124,     0,
     125,     0,   491,     0,     0,     0,   492,   126,   127,     0,
     128,   129,     0,   130,  1021,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   146,
     147,   148,   149,   150,     0,   773,     0,     0,     0,     0,
       0,   493,     0,  1147,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   496,   497,   498,   499,   260,   261,   115,     0,   116,
     117,   118,   119,     0,   120,   500,     0,     0,     0,     0,
       0,     0,   121,   122,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   123,   510,     0,   125,     0,     0,   511,
     512,     0,   513,   514,   515,     0,   128,   129,   516,   130,
       0,     0,   131,   132,   774,   775,   776,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   517,   140,     0,
     518,   142,   143,     0,   144,     0,   519,   520,   521,     0,
     522,   523,     0,   524,   145,   146,   147,   148,   149,   150,
     481,     0,     0,   525,   526,     0,     0,   482,     0,   777,
       0,   528,     0,     0,     0,   339,     0,     0,     0,   483,
       0,     0,     0,     0,     0,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   580,     0,   771,     0,     0,     0,     0,     0,
       0,     0,     0,   486,     0,     0,   487,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   488,     0,   489,   490,     0,     0,     0,   123,   124,
       0,   125,     0,   491,     0,     0,     0,   492,   126,   127,
       0,   128,   129,     0,   130,   772,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
     146,   147,   148,   149,   150,     0,   773,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,   499,   260,   261,   115,     0,
     116,   117,   118,   119,     0,   120,   500,     0,     0,     0,
       0,     0,     0,   121,   122,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   123,   510,     0,   125,     0,     0,
     511,   512,     0,   513,   514,   515,     0,   128,   129,   516,
     130,     0,     0,   131,   132,   774,   775,   776,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   517,   140,
       0,   518,   142,   143,     0,   144,     0,   519,   520,   521,
       0,   522,   523,     0,   524,   145,   146,   147,   148,   149,
     150,   481,     0,     0,   525,   526,     0,     0,   482,     0,
     777,     0,   528,     0,     0,     0,   339,     0,     0,     0,
     483,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,     0,     0,   487,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   488,     0,   489,   490,     0,     0,     0,   123,
     124,     0,   125,     0,   491,     0,     0,     0,   492,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,   146,   147,   148,   149,   150,     0,     0,     0,     0,
       0,     0,     0,   493,     0,  1105,     0,   528,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   517,
     140,     0,   518,   142,   143,  1096,   144,     0,   519,   520,
     521,  1097,   522,   523,  1098,   524,   145,   146,   147,   148,
     149,   150,   481,     0,     0,   525,   526,     0,     0,   482,
       0,   598,     0,   528,     0,     0,     0,   339,     0,     0,
       0,   483,     0,     0,     0,     0,     0,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,     0,     0,
       0,     0,     0,     0,     0,   486,     0,     0,   487,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   488,     0,   489,   490,     0,     0,     0,
     123,   124,     0,   125,     0,   491,     0,     0,     0,   492,
     126,   127,     0,   128,   129,     0,   130,  1021,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,   146,   147,   148,   149,   150,     0,   773,     0,
       0,     0,     0,     0,   493,     0,     0,     0,   528,     0,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,   499,   260,   261,
     115,     0,   116,   117,   118,   119,     0,   120,   500,     0,
       0,     0,     0,     0,     0,   121,   122,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   123,   510,     0,   125,
       0,     0,   511,   512,     0,   513,   514,   515,     0,   128,
     129,   516,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     517,   140,     0,   518,   142,   143,     0,   144,     0,   519,
     520,   521,     0,   522,   523,     0,   524,   145,   146,   147,
     148,   149,   150,   481,     0,     0,   525,   526,     0,     0,
     482,     0,   777,     0,   528,     0,     0,     0,   339,     0,
       0,     0,   483,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,     0,     0,   487,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   488,     0,   489,   490,     0,     0,
       0,   123,   124,     0,   125,     0,   491,     0,     0,     0,
     492,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   146,   147,   148,   149,   150,     0,     0,
       0,     0,     0,     0,     0,   493,     0,  1147,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,   499,   260,
     261,   115,     0,   116,   117,   118,   119,     0,   120,   500,
       0,     0,     0,     0,     0,     0,   121,   122,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   123,   510,     0,
     125,     0,     0,   511,   512,     0,   513,   514,   515,     0,
     128,   129,   516,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   517,   140,     0,   518,   142,   143,     0,   144,     0,
     519,   520,   521,     0,   522,   523,     0,   524,   145,   146,
     147,   148,   149,   150,   481,     0,     0,   525,   526,   603,
       0,   482,     0,   598,     0,   528,     0,     0,     0,   339,
       0,     0,     0,   483,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,     0,     0,
     487,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,   488,     0,   489,   490,     0,
       0,     0,     0,   123,   124,     0,   125,   491,     0,     0,
       0,   492,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,   146,   147,   148,   149,   150,
       0,     0,     0,     0,     0,     0,   493,  -342,     0,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,     0,     0,
     812,     0,     0,     0,     0,     0,   496,   497,   498,   499,
     260,   261,   115,     0,   116,   117,   118,   119,     0,   120,
     500,     0,     0,     0,     0,     0,     0,   121,   122,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   123,   510,
       0,   125,     0,     0,   511,   512,     0,   513,   514,   515,
       0,   128,   129,   516,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   517,   140,     0,   518,   142,   143,     0,   144,
       0,   519,   520,   521,     0,   522,   523,     0,   524,   145,
     146,   147,   148,   149,   150,   481,     0,     0,   525,   526,
       0,     0,   482,     0,   598,     0,   528,     0,     0,     0,
     339,     0,     0,     0,   483,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,     0,
       0,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,     0,   489,   490,
       0,     0,     0,     0,     0,     0,     0,     0,   491,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
     499,   260,   261,   115,     0,   116,   117,   118,   119,     0,
     120,   500,     0,     0,     0,     0,     0,     0,   121,   122,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   123,
     510,     0,   125,     0,     0,   511,   512,     0,   513,   514,
     515,     0,   128,   129,   516,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   517,   140,     0,   518,   142,   143,     0,
     144,     0,   519,   520,   521,     0,   522,   523,     0,   524,
     145,   146,   147,   148,   149,   150,   481,     0,     0,   525,
     526,     0,     0,   482,     0,   598,     0,   528,     0,     0,
       0,   339,     0,     0,     0,   483,     0,     0,     0,     0,
       0,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
       0,     0,   487,    83,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   488,     0,   489,
     490,     0,     0,     0,   123,   124,     0,   125,     0,   491,
       0,     0,     0,   492,   126,   127,     0,   128,   129,     0,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,   146,   147,   148,   149,
     150,     0,     0,     0,     0,    84,     0,     0,   493,     0,
       0,     0,     0,     0,   494,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   497,
     498,   499,   260,   261,   115,     0,   116,   117,   118,   119,
       0,   120,   500,     0,     0,     0,     0,     0,     0,   121,
     122,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     123,   510,     0,   125,     0,     0,   511,   512,     0,   513,
     514,   515,     0,   128,   129,   516,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   517,   140,     0,   518,   142,   143,
       0,   144,     0,   519,   520,   521,     0,   522,   523,     0,
     524,   145,   146,   147,   148,   149,   150,   481,     0,     0,
     525,   526,     0,     0,   482,     0,   598,  1119,   528,     0,
       0,     0,   339,     0,     0,     0,   483,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,     0,     0,   487,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   488,     0,
     489,   490,     0,     0,     0,   123,   124,     0,   125,     0,
     491,     0,     0,     0,   492,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,     0,     0,     0,     0,     0,  1122,     0,   493,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1320,     0,     0,   496,
     497,   498,   499,   260,   261,   115,     0,   116,   117,   118,
     119,     0,   120,   500,     0,     0,     0,     0,     0,     0,
     121,   122,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   123,   510,     0,   125,     0,     0,   511,   512,     0,
     513,   514,   515,     0,   128,   129,   516,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   517,   140,     0,   518,   142,
     143,     0,   144,     0,   519,   520,   521,     0,   522,   523,
       0,   524,   145,   146,   147,   148,   149,   150,   481,     0,
       0,   525,   526,     0,     0,   482,     0,   598,     0,   528,
       0,     0,     0,   339,     0,     0,     0,   483,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1371,     0,
       0,   486,     0,     0,   487,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   488,
       0,   489,   490,     0,     0,     0,   123,   124,     0,   125,
       0,   491,     0,     0,     0,   492,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,   499,   260,   261,   115,     0,   116,   117,
     118,   119,     0,   120,   500,     0,     0,     0,     0,     0,
       0,   121,   122,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   123,   510,     0,   125,     0,     0,   511,   512,
       0,   513,   514,   515,     0,   128,   129,   516,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   517,   140,     0,   518,
     142,   143,     0,   144,     0,   519,   520,   521,     0,   522,
     523,     0,   524,   145,   146,   147,   148,   149,   150,   481,
       0,     0,   525,   526,     0,     0,   482,     0,   598,     0,
     528,     0,     0,     0,   339,     0,     0,     0,   483,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,     0,     0,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     488,     0,   489,   490,     0,     0,     0,     0,     0,     0,
       0,     0,   491,     0,     0,     0,   492,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1526,     0,
       0,   496,   497,   498,   499,   260,   261,   115,     0,   116,
     117,   118,   119,     0,   120,   500,     0,     0,     0,     0,
       0,     0,   121,   122,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   123,   510,     0,   125,     0,     0,   511,
     512,     0,   513,   514,   515,     0,   128,   129,   516,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   517,   140,     0,
     518,   142,   143,     0,   144,     0,   519,   520,   521,     0,
     522,   523,     0,   524,   145,   146,   147,   148,   149,   150,
     481,     0,     0,   525,   526,     0,     0,   482,     0,   598,
       0,   528,     0,     0,     0,   339,     0,     0,     0,   483,
       0,     0,     0,     0,     0,   484,   485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,     0,     0,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,     0,   489,   490,     0,     0,     0,     0,     0,
       0,     0,     0,   491,     0,     0,     0,   492,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   497,   498,   499,   260,   261,   115,     0,
     116,   117,   118,   119,     0,   120,   500,     0,     0,     0,
       0,     0,     0,   121,   122,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   123,   510,     0,   125,     0,     0,
     511,   512,     0,   513,   514,   515,     0,   128,   129,   516,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   517,   140,
       0,   518,   142,   143,     0,   144,     0,   519,   520,   521,
       0,   522,   523,     0,   524,   145,   146,   147,   148,   149,
     150,   481,     0,     0,   525,   526,     0,     0,   482,     0,
     527,     0,   528,     0,     0,     0,   339,     0,     0,     0,
     483,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,     0,     0,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,     0,   489,   490,     0,     0,     0,     0,
       0,     0,     0,     0,   491,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,   493,     0,     0,   109,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,   111,     0,     0,   138,   517,
     140,  1383,   518,   142,   143,   112,   144,     0,   519,   520,
     521,     0,   522,   523,     0,   524,   145,   146,   147,   148,
     149,   150,     0,     0,     0,   525,   526,     0,     0,     6,
     113,   598,     0,   528,     0,     0,  -294,   339,     0,     0,
    1581,  1582,     0,  1583,     0,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,  1584,
       0,     0,     0,     0,     0,  1585,   121,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   123,   124,     0,
     125,     0,     0,     0,     0,     0,     0,   126,   127,     0,
     128,   129,     0,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,  1383,   144,     0,
       0,     0,  1586,     0,     0,     0,     0,     0,   145,   146,
     147,   148,   149,   150,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     6,     0,     0,     0,     0,
       0,     0,  -296,     0,     0,  1587,  1581,  1582,     0,  1583,
       0,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,     0,     0,     0,  1786,  1584,     0,     0,     0,     0,
       0,  1585,     0,   499,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1588,   124,     0,   125,     0,     0,     0,     0,
       0,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,  1589,   133,   134,   135,     0,     0,  1586,   136,
     137,  1590,  1591,  1592,     0,   138,   139,   140,     0,   141,
     142,   143,  1383,   144,     0,     0,    14,     0,     0,     0,
       0,     0,     0,   145,   146,   147,   148,   149,   150,     0,
       0,  1587,     0,     0,     0,     0,     0,     0,     0,     0,
       6,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1581,  1582,     0,  1583,     0,     0,     0,     0,   499,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
    1584,     0,     0,     0,     0,     0,  1585,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1588,   124,
       0,   125,     0,     0,     0,     0,     0,     0,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,  1589,   133,
     134,   135,     0,     0,     0,   136,   137,  1590,  1591,  1592,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,  1586,     0,     0,     0,     0,     0,   145,
     146,   147,   148,   149,   150,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1587,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,     0,     0,   711,     0,
       0,   712,     0,  1588,   124,     0,   125,   935,     0,     0,
       0,     0,     0,   126,   127,     0,   128,   129,   713,   130,
       0,   714,   131,  1589,   133,   134,   135,     0,     0,     0,
     136,   137,  1590,  1591,  1592,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,   146,   147,   148,   149,   150,
       0,   715,     0,     0,     0,     0,     0,   716,   717,   718,
       0,   719,   720,   721,     0,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,     0,   724,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   387,
       0,     0,     0,     0,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   388,     0,     0,     0,     0,
       0,   726,     0,     0,     0,     0,   389,     0,     0,     0,
       0,     0,     0,   390,     0,     0,     0,   391,   392,     0,
       0,     0,     0,   727,     0,     0,   393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,   728,   729,
       0,     0,   711,     0,     0,   712,     0,   123,   124,     0,
     125,     0,     0,     0,     0,   730,     0,   126,   127,     0,
     128,   129,   713,   130,     0,   714,   131,   132,   133,   134,
     135,   395,     0,     0,   136,   137,     0,     0,     0,   396,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   146,
     147,   148,   149,   150,     0,   715,     0,     0,     0,     0,
     397,   716,   717,   718,   398,   719,   720,   721,     0,   722,
       1,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,     2,   724,
       0,     0,     0,     0,   400,   401,     3,     0,     0,     0,
       0,     0,     0,     4,     0,     5,     0,     6,     0,     0,
       0,     0,     7,     0,     0,     0,     0,     0,   725,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,   726,     0,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,   403,   727,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,   728,   729,     0,     0,     0,     0,     0,     0,
       0,   123,   124,     0,   125,     0,    11,   711,     0,   730,
     712,   126,   127,    12,   128,   129,    13,   130,    14,    15,
     131,   132,   133,   134,   135,     0,     0,   713,   136,   137,
     714,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,    16,  1386,     0,     0,     0,     0,     0,
       0,     0,   145,   146,   147,   148,   149,   150,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     715,     0,     0,     0,     0,     0,   716,   717,   718,     0,
     719,   720,   721,     0,   722,     0,     0,     0,     0,     0,
       0,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   723,     0,   724,     0,     0,    18,    19,     0,
       0,     0,     0,     0,  1387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1050,     0,     0,  1388,
     726,     0,     0,     0,  1389,     0,     0,     0,     0,     0,
    1390,     0,     0,     0,     0,     0,     0,     0,     0,  1051,
       0,   114,   727,     0,   115,     0,   116,   117,   118,  1391,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,     0,     0,     0,   728,   729,     0,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,   730,  1052,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,   146,   147,   148,   149,   150,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   123,   124,
       0,   125,     0,     0,     0,     0,     0,     0,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
     146,   147,   148,   149,   150,   114,     0,     0,   115,   197,
     116,   117,   118,   119,   198,   120,   199,   200,   201,   202,
     203,   204,   205,   121,   122,   206,   207,   208,   209,   210,
       0,     0,   211,   212,   123,   124,   213,   125,     0,     0,
     214,   215,   216,   217,   126,   127,   218,   128,   129,   219,
     130,   220,   221,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,   222,   223,   224,   225,   138,   139,   140,
     226,   141,   142,   143,   227,   144,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   145,   146,   147,   148,   149,
     150
};

static const short yycheck[] =
{
       9,     0,     4,    62,    70,   113,    16,    16,    60,    18,
     168,   169,    69,   178,    68,    76,    75,   101,   645,    79,
      69,   459,   422,   834,   527,   459,    92,    98,   168,   169,
     812,   325,   455,   923,   567,   923,   818,    64,   681,   100,
    1382,   451,   102,   834,    96,   426,   650,   923,    57,    58,
    1129,    60,    61,    62,    63,    64,   609,   899,   668,   587,
      69,    70,    71,   923,    73,    61,    75,    76,    95,    78,
      79,    80,   923,    74,   923,  1447,   103,    73,  1105,    88,
      90,    90,    91,    92,   588,    94,    95,    96,    97,    99,
      99,   100,   101,   102,   103,   795,   105,    98,  1383,   436,
     184,    97,   161,  1055,   906,   373,   424,   341,   852,   105,
     845,   160,   765,   162,   180,  1147,   406,  1240,   758,  1596,
     435,   731,   183,   182,  1262,   276,  1351,  1401,   465,   829,
     932,    10,  1357,   188,  1008,  1487,   944,     8,   671,   941,
    1205,     8,   675,     8,   697,    21,   173,   910,   681,     6,
    1102,   160,   161,   162,     8,     8,   772,   166,   772,    47,
      43,  1134,    63,   777,   173,  1421,    90,   902,     8,   178,
       4,   180,   181,   182,   183,   184,    19,   285,   133,   188,
      49,     8,  1445,    23,    82,   181,   627,   241,    28,    46,
     149,    14,  1257,     8,    26,    97,   527,    61,     9,  1001,
      10,  1656,    23,    22,     8,    68,    47,    28,   373,    56,
      61,    30,   174,    23,    44,    49,  1956,   271,    28,    23,
    1218,    26,  1402,   196,    28,    22,   193,   169,   379,    43,
     143,    44,    10,   656,   141,   117,   185,    23,    50,    51,
       8,   292,    28,   681,    55,    23,   297,   681,  1590,  1591,
      28,  1033,  1034,    63,   125,    66,    11,    23,   125,   192,
     125,   141,    28,  2003,   664,   905,   315,   598,    23,   318,
       8,   125,   125,    28,    63,  1674,  1675,   385,    43,   190,
     196,   194,    64,   683,   707,  1465,   580,   156,    23,    80,
      45,   123,  1430,    28,  1105,    77,   633,   356,   125,   118,
     352,   595,  1084,  1177,   186,   168,   315,   196,   155,   318,
     125,  1596,   175,  1665,  1105,   288,   325,   326,   100,   929,
     139,   118,   156,   377,   373,    90,   145,   191,   230,   939,
     292,   255,   114,   836,   293,    44,  1561,  1562,   293,   208,
     174,   844,   139,   352,   293,   177,   173,   356,   145,  1804,
     180,   293,  1475,   260,   165,   965,   126,   125,   173,   292,
     179,   204,   194,  1428,  1443,   188,   246,   180,    14,   151,
      19,    80,   177,  1175,   208,    63,   425,   189,    50,    51,
    1412,   292,   179,   299,    18,   174,   435,   125,   437,   180,
     439,   292,  1648,   442,   292,   461,   405,   406,   452,   288,
     454,   284,   463,  1482,   292,  1213,  1214,  1021,   178,  1025,
    1192,  1025,   471,   292,   122,   469,   425,  1680,  1181,    23,
     296,   473,   587,   294,    28,   958,   435,   294,   437,   294,
     439,   984,   289,   442,  1407,  1408,   877,   244,    23,   292,
     294,   292,  1784,    28,  1442,    94,   777,   287,   288,   289,
     290,   292,   461,   462,   463,    23,  1264,  1829,    55,   105,
      28,  1459,   471,   196,   473,   196,   462,   476,   289,   290,
     284,   180,  1172,  1950,   476,  1427,     3,   287,   288,   289,
     290,   263,   292,   287,   288,   289,   290,    23,  1887,  1888,
     255,  1489,    28,    64,    44,    63,  1848,   196,  1975,    90,
    1282,  1283,    30,  1285,  1286,  1009,    77,    22,   291,   287,
     288,   289,   290,   196,    85,    43,  1020,   189,   577,   528,
    1024,   287,   288,   289,   290,  2002,  1800,   586,   294,   100,
      80,   590,   287,   288,   289,   290,  2013,    22,   587,  2016,
     223,  1766,   597,   114,   141,     3,  2023,    62,    17,    76,
      94,  1923,   287,   288,   289,   290,    55,   292,   314,   699,
     293,   292,    31,   117,  1375,    46,   299,   861,   577,    43,
     579,   580,  1710,    65,   196,   172,   635,   586,    98,   176,
     151,   590,   338,   579,  1375,  1664,   595,   641,   597,    18,
     187,   347,  1964,    21,   293,    61,   645,  1207,   749,  1722,
     299,   223,   156,   196,   658,   288,    75,  1481,   199,   200,
    1670,  1139,   142,    94,  1986,   930,  1256,  1270,    76,   673,
      94,   911,   623,     6,   139,  1657,   635,  1692,   158,   700,
     898,  1012,   681,   118,   103,  1379,   645,  1264,  1141,   957,
     874,  1376,   141,  1147,  1709,    44,    23,   167,   702,   203,
     230,    28,  1712,    82,   410,   199,   200,   147,   414,    87,
      98,     5,   999,    46,   179,  1950,   288,     6,  1666,   834,
     679,    15,   681,  2015,  1178,  1673,   685,   176,  1480,  1677,
    1678,    80,   262,   287,   288,   289,   290,   243,   292,    46,
    1975,   849,   263,   747,   132,   288,   134,    79,   167,   700,
     754,   755,   287,   288,   289,   290,   299,    46,   757,   294,
       6,    23,     7,  1711,   147,  1713,    28,  2002,   156,   287,
     288,   289,   290,    59,    60,     6,   294,   292,  2013,   294,
      65,  2016,   114,   898,     6,     6,   155,    94,  2023,   295,
      23,   174,   882,   181,   126,    28,   145,   303,   757,    61,
      46,   287,   288,   289,   290,    65,   155,   513,   294,    23,
      11,   118,  1841,   182,    28,    46,  1793,    23,  1847,   525,
     526,   527,    28,   800,    46,    46,  1728,  1187,   231,   232,
      63,   180,    64,    23,  1674,  1675,  1674,  1675,    28,   143,
     231,   800,   196,  1296,    45,    77,    85,   174,  1674,  1675,
     241,   850,   196,   852,   292,   854,    23,   808,   162,   297,
     869,    28,  1512,    22,  1674,  1675,  1814,  1815,   100,   108,
    1662,    30,   196,  1674,  1675,  1674,  1675,   967,   837,   223,
    1440,  1441,   114,   892,    23,   894,   845,   196,   293,    28,
     122,   850,   598,   852,  1448,   854,   602,  1457,  1258,   898,
     295,   293,   861,   298,    23,   904,   910,   122,  1361,    28,
     869,   870,   342,   917,   223,    64,   292,   876,   294,   151,
     350,   880,   292,   927,   870,   164,   292,   297,    77,  1661,
     140,   930,    23,   892,   122,   894,  1267,    28,   959,     4,
     199,   200,     7,   902,    63,   904,   292,   203,   294,   205,
     124,   100,   911,    23,  1547,   292,   955,   294,    28,   958,
     287,   288,   289,   290,   232,  1468,  1943,  1420,  1945,   973,
    1473,   930,   993,   994,     4,   996,  1953,   292,  1955,   294,
     124,  1929,  1930,  1931,    49,    79,    16,   796,   156,   798,
    1105,  1000,   192,   802,   803,   804,   955,   180,   411,   958,
     413,  1010,   151,    19,   292,  1336,   294,   239,   959,   292,
     259,   294,    23,   964,   198,  1296,   975,    28,   292,    49,
     294,    51,  1999,   975,  1139,   287,   288,   289,   290,   293,
     981,   263,  1793,    23,   162,   195,   196,   197,    28,  1634,
    1635,  1000,   993,   994,   199,   996,  1778,  1887,  1888,  1887,
    1888,  1010,  1793,    23,   287,   288,   289,   290,    28,  1018,
    1019,  1887,  1888,   223,   224,   292,  1018,   294,   292,   266,
     294,   777,    10,   287,   288,   289,   290,  1887,  1888,    59,
     294,   287,   288,   289,   290,    61,  1887,  1888,  1887,  1888,
      79,   797,  1575,    78,   292,   801,   294,   287,   288,   289,
     290,   130,   131,    23,   294,   811,   190,   292,    28,   294,
     816,   817,  1485,  1090,   820,   821,   822,   823,   824,   825,
     287,   288,   289,   290,    23,   149,   292,   294,   294,    28,
     836,  1090,   838,   839,   840,   841,   842,   843,   844,    23,
    1139,   292,  1386,   294,    28,  1389,   172,    23,   287,   288,
     289,   290,    28,  1243,   141,   294,  1716,    82,   292,  1547,
     294,   293,   292,  1547,   294,   292,    63,   294,   287,   288,
     289,   290,   878,  1905,  1133,  1134,   292,  1181,   294,    61,
      23,  1913,  1943,    61,  1945,    28,   122,   292,  1147,   294,
     122,  1415,  1953,  1417,  1955,   901,   287,   288,   289,   290,
      23,   244,  1943,   294,  1945,    28,  1634,  1635,   914,   174,
     916,   102,  1953,   172,  1955,  1971,  1972,   287,   288,   289,
     290,   283,   282,   280,   294,    61,   156,   123,  1509,  1510,
      65,    83,  1253,   121,   243,   254,  1517,  1518,  1999,  1520,
    1521,    23,   155,   255,   254,  1266,    28,  1528,  1529,  1530,
    1531,  1272,  1533,  1534,  1535,  1536,  1537,  1538,  1999,    43,
    1375,  1260,    52,   207,   292,  1264,    23,   284,  1383,   192,
     112,    28,   293,  1232,    29,    10,   287,   288,   289,   290,
     135,   199,   199,   294,   199,  1237,   293,   293,   293,   293,
    1249,   293,  1400,  1401,  1402,   293,   293,   287,   288,   289,
     290,  1260,  1253,    23,   294,  1264,    97,   293,    28,   293,
     293,   293,   293,    10,   293,  1266,   293,   287,   288,   289,
     290,  1272,   293,   148,   294,   293,   293,   293,    23,  1035,
    1036,  1037,  1682,    28,  1040,  1041,  1042,   293,  1044,  1045,
    1046,  1047,  1048,  1049,  1434,   293,  1436,   293,  1801,   296,
    1056,  1057,    23,  1059,   293,  1061,  1390,    28,  1064,  1065,
    1066,  1067,  1068,  1069,   292,   298,   141,   287,   288,   289,
     290,   298,  1332,  1332,   294,  1382,  1082,  1388,   284,  1085,
    1379,  1391,  1959,  1382,  1383,   293,    63,   292,   287,   288,
     289,   290,   281,  1483,  1100,   294,   166,   292,   284,   293,
     298,    10,    83,   287,   288,   289,   290,   292,   298,    22,
     294,   287,   288,   289,   290,    90,    23,  1376,   294,   111,
    1379,    28,    90,  1382,  1383,    90,    10,  1386,  1387,  1388,
    1389,  1390,  1391,   174,   136,  1141,   174,    23,   136,    21,
     196,  1387,    28,    90,   287,   288,   289,   290,  1407,  1408,
     138,   294,    23,  1412,   185,   185,   293,    28,    91,   293,
     293,   293,    61,    23,   287,   288,   289,   290,    28,   296,
      10,   294,   122,   149,     0,   292,    17,   128,   121,   293,
     293,  1596,    23,   189,    10,   126,    61,    28,   294,    21,
      31,   299,  1496,    42,    23,   174,   174,  1501,   202,    28,
     293,    60,   186,   292,   190,   287,   288,   289,   290,    83,
      10,  1463,   294,   292,   240,    23,   293,    19,    27,   293,
      28,    86,    23,    86,    43,    54,    67,    28,   125,   125,
     287,   288,   289,   290,    75,    61,   125,   294,    64,   156,
     154,   156,    83,   293,  1659,    71,    54,    73,    98,   293,
     293,    77,   108,   293,    23,   294,    82,    63,   292,    85,
     101,   294,   103,   292,   184,  1581,   298,   287,   288,   289,
     290,    16,   292,   124,   100,   116,   107,   118,   119,   120,
      63,    82,    23,  1543,  1543,    63,   293,    28,   114,   292,
    1296,    71,   287,   288,   289,   290,   122,  1596,    12,   294,
     126,   239,    10,  1309,  1310,   160,  1312,  1313,   295,   135,
      10,   294,   294,   192,  1320,   133,   287,   288,   289,   290,
     294,   294,  1581,   294,    52,   151,   167,    52,   294,     8,
    1589,  1590,  1591,   294,  1340,   293,   293,  1596,   293,    63,
     293,    82,  1348,   173,  1350,  1351,    51,   293,   293,   293,
     192,  1357,   178,  1359,  1360,  1361,  1362,     6,   293,   293,
     127,   293,   293,  1369,   190,  1371,   192,   293,    37,   293,
     293,   293,   293,   292,  1683,  1634,  1635,    19,  1793,    48,
     287,   288,   289,   290,   292,    73,    55,   294,  1803,   294,
      59,   294,  1800,    85,    85,    44,   130,  1656,  1657,   145,
    1659,   287,   288,   289,   290,   125,   125,    56,   294,   125,
      79,   294,   125,   239,  1420,   125,   287,   288,   289,   290,
     196,    70,   125,   294,  1683,   294,    22,   287,   288,   289,
     290,    80,   293,   259,   294,    63,   294,   263,  1444,    51,
     294,   294,   294,   284,   285,   286,   287,   288,   289,   290,
     294,   173,  1754,   292,   291,   296,    36,   298,   287,   288,
     289,   290,    36,    82,   291,   291,   292,   291,   294,  1040,
     293,  1042,   141,  1044,  1045,  1833,  1047,  1048,  1049,   287,
     288,   289,   290,    51,    82,   295,   287,   288,   289,   290,
     174,  1751,  1751,    70,   278,  1754,   145,    10,    37,    85,
      84,    23,  1508,   172,    79,   180,   155,   176,  1514,    48,
      80,    44,   180,  1519,    80,    44,    55,   294,    94,  1828,
    1526,   294,   124,   294,   111,  1784,   111,   293,  1943,    68,
    1945,   180,  1834,   117,   294,  1950,   294,   294,  1953,   294,
    1955,     6,  1791,   294,  1803,  1804,   287,   288,   289,   290,
    1854,   294,   294,   202,  1858,  1561,  1562,   294,   294,   143,
    1975,   294,   146,   294,  1570,  1824,  1924,   294,   294,  1828,
      61,   294,   189,   294,   124,  1834,   294,   292,   162,    44,
    1586,  1897,   293,    86,  1999,   294,   125,  2002,   204,    79,
      82,    56,   293,   246,    82,   293,   265,   293,  2013,   122,
     294,  2016,   141,    17,   188,   292,   173,   293,  2023,    23,
     122,   122,   294,   294,    28,    80,    47,    31,   157,    44,
     180,  1880,   210,   211,   212,   213,   214,   215,   216,   217,
     294,    50,    36,   172,    47,     0,   201,   176,  1897,  1898,
     179,     0,  1398,   323,   593,   165,   165,  1899,   187,   870,
     166,  1950,   326,    67,  1660,   595,  1270,   861,   761,   243,
    1959,    75,  1013,  1018,  1267,   988,  1259,   206,  1682,    83,
    1700,    80,  1190,   904,   456,   381,  1975,   940,  1260,   707,
     145,   437,   266,  1264,  1680,   437,  1699,   101,  1962,   103,
     155,  1950,  1965,  1942,  1966,   279,  1988,  1956,  1951,  1130,
    1959,   930,   116,  2002,   118,   119,   120,  1788,     0,   850,
    1245,  1431,  1971,  1972,  2013,   180,  1975,  2016,   687,   991,
     427,   420,   744,  1471,  2023,  1731,   265,    13,   983,   167,
     442,  1737,  1738,   363,   611,   616,   892,   171,   878,  1147,
      90,  1416,  1918,  2002,  2003,   794,  1543,  1751,   914,     0,
       0,     0,   868,   167,  2013,  1761,  2015,  2016,     6,   627,
    1766,     9,  1142,  1769,  2023,    13,   505,  1311,  1193,  1359,
    1776,  1777,    20,   975,    48,  1824,  1249,    94,  1589,   685,
      -1,    55,    -1,    -1,    32,    59,    60,    -1,    -1,    -1,
      38,    39,    -1,    17,    68,  1801,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    79,    -1,    31,  1509,  1510,
      -1,    -1,    -1,    -1,    -1,    -1,  1517,  1518,    66,  1520,
    1521,    69,    -1,    -1,    -1,    -1,    -1,  1528,  1529,  1530,
    1531,    -1,  1533,  1534,  1535,  1536,  1537,  1538,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    94,  1853,    96,    97,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,   106,    83,
      -1,    -1,   110,    -1,    -1,    -1,    -1,   141,    -1,    -1,
     284,   285,   286,   287,   288,   289,   290,   101,    -1,   103,
      -1,    -1,   296,   157,   298,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,   119,   120,    -1,   172,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1918,   187,    -1,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,  1952,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,   265,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,   287,
     288,     6,    -1,    -1,     9,   293,    -1,   295,    13,    -1,
      -1,   299,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
     284,   285,   286,   287,   288,   289,   290,    32,    -1,    -1,
     294,    -1,    17,    38,    39,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      75,    96,    97,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,   278,   279,   280,   281,   282,   283,    -1,
      -1,    -1,   287,   288,     6,    -1,    -1,     9,   293,    -1,
     295,    13,    -1,    -1,   299,    -1,    -1,    -1,    20,   284,
     285,   286,   287,   288,   289,   290,    -1,    -1,    -1,    -1,
      32,   296,    -1,   298,    -1,    17,    38,    39,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    75,    96,    97,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    27,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,   287,   288,     6,    -1,    -1,
       9,   293,    -1,   295,    13,    -1,    -1,   299,    -1,    -1,
      -1,    20,   284,   285,   286,   287,   288,   289,   290,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,   293,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,    -1,   277,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,   287,   288,
       6,    -1,    -1,     9,   293,    -1,   295,    13,    -1,    -1,
     299,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,   287,   288,     6,    -1,    -1,     9,   293,    -1,   295,
      13,    -1,    -1,   299,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    38,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      66,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,    -1,   287,   288,     6,    -1,    -1,     9,
     293,    -1,   295,    13,    -1,    -1,   299,    -1,    94,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,   219,   220,   221,   222,   223,   224,   225,
     226,    -1,    -1,    -1,    -1,    -1,    -1,   233,   234,    -1,
      -1,    -1,    -1,   163,    -1,   165,   242,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,   288,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,   279,
     280,   281,   282,   283,    -1,    -1,    -1,   287,   288,     6,
      -1,    -1,     9,   293,    -1,   295,    13,    18,    -1,   299,
      21,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    32,    -1,    38,    -1,    -1,
      41,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,   113,   110,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,   218,   219,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,   235,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    -1,
     277,   278,   279,   280,   281,   282,   283,    13,    -1,    -1,
     287,   288,    -1,    -1,    20,    -1,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    13,    -1,
      -1,   287,   288,    -1,    -1,    20,    -1,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,   118,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,   159,    -1,    -1,    -1,   289,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,   278,   279,   280,   281,   282,   283,    13,
      -1,    -1,   287,   288,    -1,    -1,    20,    -1,   293,    -1,
     295,    -1,    -1,    -1,   299,   155,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,   282,   283,    -1,   159,    -1,    -1,    -1,    -1,
      -1,   165,    -1,   293,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    -1,   277,   278,   279,   280,   281,   282,   283,
      13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,   118,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,   159,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   293,    -1,   295,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,   267,   268,    -1,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,
      -1,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,   279,   280,   281,   282,   283,    -1,   159,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,   295,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,    -1,   277,   278,   279,   280,
     281,   282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,
      20,    -1,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
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
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,   293,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,   279,
     280,   281,   282,   283,    13,    -1,    -1,   287,   288,   289,
      -1,    20,    -1,   293,    -1,   295,    -1,    -1,    -1,   299,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,   106,    -1,    -1,
      -1,   110,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,   283,
      -1,    -1,    -1,    -1,    -1,    -1,   165,   291,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,    -1,   277,   278,
     279,   280,   281,   282,   283,    13,    -1,    -1,   287,   288,
      -1,    -1,    20,    -1,   293,    -1,   295,    -1,    -1,    -1,
     299,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
     278,   279,   280,   281,   282,   283,    13,    -1,    -1,   287,
     288,    -1,    -1,    20,    -1,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,   196,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    -1,
     277,   278,   279,   280,   281,   282,   283,    13,    -1,    -1,
     287,   288,    -1,    -1,    20,    -1,   293,   294,   295,    -1,
      -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,    -1,    -1,   289,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    13,    -1,
      -1,   287,   288,    -1,    -1,    20,    -1,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,   278,   279,   280,   281,   282,   283,    13,
      -1,    -1,   287,   288,    -1,    -1,    20,    -1,   293,    -1,
     295,    -1,    -1,    -1,   299,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    -1,   277,   278,   279,   280,   281,   282,   283,
      13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,   293,
      -1,   295,    -1,    -1,    -1,   299,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,
     293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    56,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,   145,    -1,    -1,   260,   261,
     262,    16,   264,   265,   266,   155,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,   287,   288,    -1,    -1,    44,
     180,   293,    -1,   295,    -1,    -1,    51,   299,    -1,    -1,
      55,    56,    -1,    58,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    74,
      -1,    -1,    -1,    -1,    -1,    80,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    16,   268,    -1,
      -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,   170,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    74,    -1,    -1,    -1,    -1,
      -1,    80,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,   137,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,    -1,   264,
     265,   266,    16,   268,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    -1,    58,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      74,    -1,    -1,    -1,    -1,    -1,    80,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    18,    -1,
      -1,    21,    -1,   227,   228,    -1,   230,    27,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    38,   243,
      -1,    41,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,   283,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
      -1,    -1,    18,    -1,    -1,    21,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,   235,    -1,   237,   238,    -1,
     240,   241,    38,   243,    -1,    41,   246,   247,   248,   249,
     250,   133,    -1,    -1,   254,   255,    -1,    -1,    -1,   141,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,   282,   283,    -1,    81,    -1,    -1,    -1,    -1,
     172,    87,    88,    89,   176,    91,    92,    93,    -1,    95,
       7,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    25,   115,
      -1,    -1,    -1,    -1,   206,   207,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    42,    -1,    44,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,   144,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   265,    -1,    -1,   268,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   143,    18,    -1,   235,
      21,   237,   238,   150,   240,   241,   153,   243,   155,   156,
     246,   247,   248,   249,   250,    -1,    -1,    38,   254,   255,
      41,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,   180,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,   115,    -1,    -1,   244,   245,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,   176,
     161,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   198,   183,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,   218,   219,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,   235,   164,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,   279,   280,   281,   282,   283,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,   198,    -1,    -1,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,    -1,   225,   226,   227,   228,   229,   230,    -1,    -1,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283
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
{ yyval = yyvsp[0]; }
    break;
case 126:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 127:
{ yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 128:
{ yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 129:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 130:
{ yyval = make_node (nod_unique, 0, NULL); }
    break;
case 131:
{ yyval = NULL; }
    break;
case 132:
{ yyval = make_list (yyvsp[0]); }
    break;
case 134:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 135:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 136:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 137:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 138:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 139:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 140:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 141:
{ yyval = (dsql_nod*) 0;}
    break;
case 142:
{ yyval = yyvsp[-1]; }
    break;
case 143:
{ yyval = NULL; }
    break;
case 146:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 147:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 148:
{ yyval = NULL; }
    break;
case 149:
{ yyval = NULL; }
    break;
case 150:
{ yyval = yyvsp[0]; }
    break;
case 152:
{ yyval = NULL; }
    break;
case 154:
{ yyval = NULL; }
    break;
case 156:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 157:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 160:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 161:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count,
					NULL, NULL, yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 162:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
    break;
case 163:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
    break;
case 164:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 167:
{ yyval = (dsql_nod*) yyvsp[0]; }
    break;
case 168:
{yyval = NULL;}
    break;
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 172:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 173:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 174:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 175:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 176:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 177:
{yyval = NULL;}
    break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 182:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 183:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 184:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 185:
{ lex.g_file  = make_file();}
    break;
case 190:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 191:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
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
{ yyval = NULL; }
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
{ yyval = NULL; }
    break;
case 217:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 218:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 219:
{ yyval = yyvsp[-1]; }
    break;
case 220:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
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
case 234:
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
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;
case 250:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;
case 251:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;
case 252:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;
case 253:
{ yyval = NULL;}
    break;
case 254:
{ yyval = yyvsp[0];}
    break;
case 255:
{ yyval = yyvsp[0];}
    break;
case 256:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;
case 257:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;
case 258:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;
case 260:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 261:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 262:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 263:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 264:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 265:
{ yyval = NULL; }
    break;
case 266:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 267:
{ yyval = NULL; }
    break;
case 269:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 270:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-4], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 273:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;
case 274:
{ yyval = yyvsp[0]; }
    break;
case 275:
{ yyval = yyvsp[0]; }
    break;
case 276:
{ yyval = NULL; }
    break;
case 277:
{ yyval = make_list (yyvsp[0]); }
    break;
case 278:
{ yyval = NULL; }
    break;
case 280:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 281:
{ yyval = yyvsp[-1]; }
    break;
case 284:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;
case 285:
{ yyval = NULL; }
    break;
case 286:
{ yyval = NULL; }
    break;
case 287:
{ yyval = yyvsp[0]; }
    break;
case 288:
{ yyval = yyvsp[0]; }
    break;
case 289:
{ yyval = NULL; }
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
case 315:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 316:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 321:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
    break;
case 322:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
    break;
case 323:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 324:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
    break;
case 325:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 326:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 327:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
    break;
case 328:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 329:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 330:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
    break;
case 331:
{ yyval = NULL; }
    break;
case 332:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
    break;
case 333:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;
case 336:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 338:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 339:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 340:
{ yyval = NULL; }
    break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 342:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 343:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
    break;
case 344:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 345:
{ yyval = NULL; }
    break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 348:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 351:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;
case 352:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;
case 353:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;
case 354:
{ yyval = make_node (nod_default, 1, NULL); }
    break;
case 358:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 359:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 360:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 361:
{ yyval = NULL; }
    break;
case 362:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 363:
{ yyval = make_list (yyvsp[0]); }
    break;
case 364:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 365:
{ yyval = NULL; }
    break;
case 366:
{ yyval = make_list (yyvsp[0]); }
    break;
case 367:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 368:
{ yyval = NULL; }
    break;
case 369:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 370:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 371:
{ yyval = NULL; }
    break;
case 373:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 374:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 375:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 376:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 377:
{ lex.beginning = lex_position(); }
    break;
case 378:
{ lex.beginning = lex.last_token; }
    break;
case 379:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 380:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning, 
					(DSQL_yychar < 0 ? lex_position() : lex.last_token) - lex.beginning); 
			}
    break;
case 381:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 382:
{ yyval = 0; }
    break;
case 383:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 384:
{ yyval = make_node (nod_redef_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 385:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_mod_index, (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 418:
{ yyval = yyvsp[0]; }
    break;
case 419:
{ yyval = yyvsp[0]; }
    break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 422:
{ yyval = make_node (nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 436:
{ yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 437:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
    break;
case 438:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
    break;
case 439:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
    break;
case 440:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 482:
{ yyval = NULL; }
    break;
case 483:
{ yyval = NULL; }
    break;
case 484:
{ yyval = NULL; }
    break;
case 485:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
					yyvsp[0], NULL, NULL, NULL, NULL); }
    break;
case 486:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 487:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 488:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 489:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 490:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 491:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 492:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 493:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 494:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 495:
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 496:
{ yyval = yyvsp[0]; }
    break;
case 497:
{ yyval = NULL; }
    break;
case 498:
{ yyval = yyvsp[0]; }
    break;
case 499:
{ yyval = NULL; }
    break;
case 500:
{ yyval = NULL; }
    break;
case 502:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 503:
{ yyval = yyvsp[0]; }
    break;
case 504:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 505:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 506:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 507:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 508:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 510:
{ yyval = NULL; }
    break;
case 512:
{ yyval = NULL; }
    break;
case 513:
{ yyval = yyvsp[0]; }
    break;
case 514:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 515:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 516:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 517:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 518:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 519:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 520:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 521:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 522:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 523:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 524:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 525:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 526:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 531:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 532:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 534:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 535:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 536:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 542:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 545:
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
case 546:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
    break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 553:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 554:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 555:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 556:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 557:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 558:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 559:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 563:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 566:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 575:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 576:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 577:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 578:
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
case 579:
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
case 582:
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
case 583:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 584:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 585:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 586:
{ yyval = yyvsp[-1]; }
    break;
case 587:
{ yyval = 0; }
    break;
case 591:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 592:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 593:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 597:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 598:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 599:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 600:
{ yyval = 0; }
    break;
case 601:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 604:
{ yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
    break;
case 605:
{ yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
    break;
case 608:
{ yyval = make_node (nod_retain, 0, NULL); }
    break;
case 609:
{ yyval = NULL; }
    break;
case 611:
{ yyval = NULL; }
    break;
case 612:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 614:
{ yyval = NULL; }
    break;
case 616:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 623:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 624:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 625:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 626:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 627:
{ yyval = yyvsp[0];}
    break;
case 629:
{ yyval = yyvsp[0];}
    break;
case 630:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 631:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 632:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 633:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 634:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 635:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 636:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 637:
{ yyval = 0; }
    break;
case 638:
{ yyval = make_flag_node(nod_tra_misc, NOD_NO_AUTO_UNDO, 0, NULL); }
    break;
case 639:
{ yyval = make_flag_node(nod_tra_misc, NOD_IGNORE_LIMBO, 0, NULL); }
    break;
case 640:
{ yyval = make_flag_node(nod_tra_misc, NOD_RESTART_REQUESTS, 0, NULL); }
    break;
case 641:
{ yyval = make_node(nod_lock_timeout, 1, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 642:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 643:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 644:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 645:
{ yyval = (dsql_nod*) 0; }
    break;
case 646:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 647:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 649:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 650:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 651:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 652:
{ yyval = 0; }
    break;
case 654:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 655:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 656:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;
case 657:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 658:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;
case 659:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;
case 660:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;
case 661:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 662:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;
case 663:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 664:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;
case 665:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;
case 666:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;
case 667:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;
case 668:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 669:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 670:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;
case 671:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;
case 672:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;
case 673:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;
case 674:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 675:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 676:
{ yyval = yyvsp[0]; }
    break;
case 678:
{ yyval = NULL; }
    break;
case 679:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 680:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 681:
{ yyval = NULL; }
    break;
case 682:
{ yyval = yyvsp[0]; }
    break;
case 683:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 684:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 685:
{ yyval = NULL; }
    break;
case 686:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 687:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 688:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 690:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 691:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 693:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 694:
{ lex.limit_clause = true; }
    break;
case 695:
{ lex.limit_clause = false; }
    break;
case 696:
{ lex.first_detection = true; }
    break;
case 697:
{ lex.first_detection = false; }
    break;
case 698:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 699:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 700:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 701:
{ yyval = 0; }
    break;
case 702:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 703:
{ yyval = yyvsp[-2]; }
    break;
case 704:
{ yyval = yyvsp[-1]; }
    break;
case 705:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 706:
{ yyval = yyvsp[-1]; }
    break;
case 707:
{ yyval = yyvsp[0]; }
    break;
case 708:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 709:
{ yyval = 0; }
    break;
case 710:
{ yyval = make_list (yyvsp[0]); }
    break;
case 711:
{ yyval = 0; }
    break;
case 713:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 715:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 718:
{ yyval = make_list (yyvsp[0]); }
    break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 725:
{ yyval = yyvsp[-1]; }
    break;
case 726:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 728:
{ yyval = NULL; }
    break;
case 729:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 730:
{ yyval = NULL; }
    break;
case 732:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 736:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 737:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 738:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 741:
{ yyval = yyvsp[0]; }
    break;
case 742:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 743:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 744:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 745:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 746:
{ yyval = NULL; }
    break;
case 748:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 749:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 750:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 751:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 752:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 753:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 754:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 757:
{ yyval = make_list (yyvsp[0]); }
    break;
case 758:
{ yyval = NULL; }
    break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 762:
{ yyval = yyvsp[0]; }
    break;
case 763:
{ yyval = NULL; }
    break;
case 764:
{ yyval = yyvsp[0]; }
    break;
case 765:
{ yyval = NULL; }
    break;
case 766:
{ yyval = yyvsp[0]; }
    break;
case 767:
{ yyval = NULL; }
    break;
case 768:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 769:
{ yyval = 0; }
    break;
case 770:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 771:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 772:
{ yyval = 0; }
    break;
case 773:
{ yyval = 0; }
    break;
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 776:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 779:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 780:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 781:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 782:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 785:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 786:
{ yyval = 0; }
    break;
case 787:
{ yyval = make_list (yyvsp[0]); }
    break;
case 788:
{ yyval = 0; }
    break;
case 790:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 792:
{ yyval = 0; }
    break;
case 793:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 794:
{ yyval = 0; }
    break;
case 795:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 796:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 797:
{ yyval = yyvsp[-1]; }
    break;
case 798:
{ yyval = 0; }
    break;
case 799:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 801:
{ yyval = NULL; }
    break;
case 802:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
    break;
case 806:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 807:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 811:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
    break;
case 813:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
    break;
case 814:
{ yyval = NULL; }
    break;
case 815:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 817:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 819:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 820:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 824:
{ yyval = NULL; }
    break;
case 827:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 828:
{ yyval = yyvsp[0]; }
    break;
case 829:
{ yyval = NULL; }
    break;
case 830:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 833:
{ yyval = NULL; }
    break;
case 834:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 838:
{ yyval = NULL; }
    break;
case 839:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 841:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 843:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 844:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 845:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 854:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 856:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 857:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 858:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 860:
{ yyval = yyvsp[-1]; }
    break;
case 861:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 873:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 874:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 875:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 876:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 877:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 878:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 879:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 880:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 881:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 882:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 883:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 884:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 885:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 886:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 887:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 888:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 889:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 890:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 891:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 892:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 893:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 894:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 895:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 896:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 900:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 901:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 902:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 903:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 904:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 905:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 906:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 907:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 908:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 909:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 910:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 911:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 912:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 913:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 914:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 915:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 916:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 917:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 918:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 919:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 920:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 921:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 922:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 923:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 924:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 926:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 927:
{ yyval = yyvsp[-1]; }
    break;
case 938:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 939:
{ yyval = yyvsp[0]; }
    break;
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 941:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 942:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 943:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 944:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 945:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 946:
{ yyval = yyvsp[-1]; }
    break;
case 947:
{ yyval = yyvsp[-1]; }
    break;
case 951:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 952:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 953:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 956:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
    break;
case 957:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 1, yyvsp[0]);
			}
    break;
case 958:
{ yyval = make_node (nod_current_timestamp, 1, yyvsp[0]); }
    break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 960:
{ yyval = NULL; }
    break;
case 961:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 963:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 965:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 966:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 967:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 968:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 969:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 970:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 972:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 973:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
    break;
case 974:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
    break;
case 975:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 976:
{ yyval = make_parameter (); }
    break;
case 977:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 978:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 979:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 980:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 981:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 982:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 983:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 984:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 985:
{ yyval = yyvsp[0]; }
    break;
case 986:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 988:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 989:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 990:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 991:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 992:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 994:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 995:
{ yyval = yyvsp[0];}
    break;
case 999:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 1000:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 1001:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 1002:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 1003:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 1004:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 1005:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 1006:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 1007:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 1008:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 1009:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 1012:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 1016:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1017:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1018:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1019:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1022:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 1023:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;
case 1024:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 1025:
{ yyval = yyvsp[0]; }
    break;
case 1026:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 1027:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;
case 1028:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
    break;
case 1029:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;
case 1030:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
    break;
case 1031:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
    break;
case 1032:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
    break;
case 1033:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
    break;
case 1034:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1035:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 1036:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 1039:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 1040:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 1041:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1044:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1045:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1046:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1047:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1048:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1049:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1050:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1051:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1055:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
    break;
case 1056:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1057:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1058:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1059:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1060:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1061:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1062:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1063:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1064:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1069:
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

		const dsql_nod* old = node;
		node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		node->nod_line = (USHORT) lex.lines_bk;
		node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
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
			isc_arg_gds, isc_command_end_err2,	/* Unexpected end of command */
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1),
			0);
	else
	{
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			/* Token unknown - line %d, column %d */
			isc_arg_gds, isc_dsql_token_unk_err,
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Show the token */
			isc_arg_gds, isc_random,
			isc_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
			0);
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
