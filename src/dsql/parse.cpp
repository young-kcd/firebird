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
	const TEXT* stop_trigger;
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



#define	YYFINAL		2030
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
    2961,  2963,  2968,  2970,  2974,  2976,  2979,  2981,  2983,  2985,
    2987,  2989,  2991,  2993,  2996,  2999,  3002,  3004,  3006,  3008,
    3010,  3012,  3014,  3016,  3018,  3020,  3022,  3025,  3027,  3030,
    3032,  3034,  3036,  3038,  3040,  3043,  3045,  3047,  3049,  3051,
    3056,  3062,  3068,  3074,  3080,  3086,  3092,  3098,  3104,  3110,
    3116,  3118,  3120,  3127,  3129,  3131,  3133,  3138,  3143,  3148,
    3153,  3155,  3157,  3162,  3167,  3175,  3178,  3179,  3187,  3194,
    3201,  3206,  3208,  3210,  3212,  3217,  3221,  3228,  3230,  3232,
    3239,  3248,  3255,  3257,  3259,  3264,  3271,  3276,  3282,  3286,
    3292,  3297,  3303,  3305,  3307,  3309,  3314,  3321,  3323,  3325,
    3327,  3329,  3331,  3333,  3335,  3337,  3339,  3340,  3342,  3343,
    3345,  3347,  3349,  3351,  3353,  3355,  3357,  3359,  3361,  3363,
    3365,  3367,  3369,  3371,  3373,  3375,  3377,  3379,  3381,  3383,
    3385,  3387,  3389,  3391,  3393,  3395,  3397,  3399,  3401,  3403,
    3405,  3407,  3409,  3411,  3413,  3415,  3417,  3419,  3421,  3423,
    3425,  3427,  3429,  3431,  3433,  3435,  3437,  3439,  3441,  3443,
    3445,  3447,  3449,  3451,  3453,  3455,  3457,  3459,  3461,  3463,
    3465,  3467,  3469
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
     473,   357,   474,   358,   391,     0,    10,     0,     0,    43,
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
       0,   401,     0,   392,   390,   357,   474,   397,   391,     0,
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
       0,   475,     0,   660,   296,   687,   297,     0,   683,     0,
     687,   292,   683,     0,   690,     0,   288,   689,     0,   197,
       0,   196,     0,   195,     0,   223,     0,   224,     0,   689,
       0,   695,     0,    38,   199,     0,   218,   199,     0,   219,
     199,     0,   299,     0,   181,     0,   225,     0,   226,     0,
     233,     0,   234,     0,    66,     0,   165,     0,   242,     0,
     199,     0,   200,   199,     0,   697,     0,   288,   698,     0,
     196,     0,   196,     0,   697,     0,   196,     0,   702,     0,
     288,   702,     0,   196,     0,   704,     0,   705,     0,   711,
       0,    32,   293,   289,   294,     0,    32,   293,   730,   683,
     294,     0,    32,   293,    46,   683,   294,     0,   171,   293,
     730,   683,   294,     0,   171,   293,    46,   683,   294,     0,
      13,   293,   730,   683,   294,     0,    13,   293,    46,   683,
     294,     0,   110,   293,   730,   683,   294,     0,   110,   293,
      46,   683,   294,     0,   106,   293,   730,   683,   294,     0,
     106,   293,    46,   683,   294,     0,   706,     0,   707,     0,
     209,   293,   729,    63,   683,   294,     0,   708,     0,   709,
       0,   710,     0,   270,   293,   683,   294,     0,   271,   293,
     683,   294,     0,   272,   293,   683,   294,     0,   275,   293,
     683,   294,     0,   712,     0,   714,     0,    96,   293,   683,
     294,     0,   274,   293,   683,   294,     0,   228,   293,   683,
      63,   683,   713,   294,     0,    61,   683,     0,     0,   277,
     293,   715,   683,    63,   683,   294,     0,   277,   293,   683,
      63,   683,   294,     0,   277,   293,   715,    63,   683,   294,
       0,   277,   293,   683,   294,     0,   267,     0,   276,     0,
     273,     0,   733,   293,   687,   294,     0,   733,   293,   294,
       0,    20,   293,   683,    10,   394,   294,     0,   719,     0,
     720,     0,   237,   293,   683,   292,   683,   294,     0,   261,
     293,   663,   292,   683,   292,   683,   294,     0,   238,   293,
     683,   292,   687,   294,     0,   721,     0,   723,     0,   236,
     726,   722,    51,     0,   236,   726,   722,    50,   727,    51,
       0,   189,   725,   173,   727,     0,   722,   189,   725,   173,
     727,     0,   236,   724,    51,     0,   236,   724,    50,   727,
      51,     0,   189,   663,   173,   727,     0,   724,   189,   663,
     173,   727,     0,   683,     0,   683,     0,   683,     0,   264,
      97,    61,   744,     0,    69,   293,   744,   292,   683,   294,
       0,   210,     0,   211,     0,   212,     0,   213,     0,   214,
       0,   215,     0,   216,     0,   217,     0,     6,     0,     0,
      46,     0,     0,    94,     0,   198,     0,   758,     0,   758,
       0,   758,     0,   758,     0,   758,     0,   758,     0,   758,
       0,   758,     0,   758,     0,   758,     0,   758,     0,   758,
       0,   758,     0,   758,     0,   758,     0,   758,     0,   758,
       0,   758,     0,   758,     0,   758,     0,   758,     0,   758,
       0,   758,     0,   758,     0,   198,     0,   759,     0,   201,
       0,   203,     0,   204,     0,   205,     0,   206,     0,   208,
       0,   227,     0,   230,     0,   228,     0,   238,     0,   241,
       0,   247,     0,   243,     0,   237,     0,   240,     0,   246,
       0,   248,     0,   249,     0,   250,     0,   260,     0,   254,
       0,   255,     0,   261,     0,   262,     0,   216,     0,   217,
       0,   265,     0,   264,     0,   266,     0,   268,     0,   278,
       0,   279,     0,   280,     0,   281,     0,   282,     0,   283,
       0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   557,   559,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   591,   595,   599,   604,   605,   608,
     610,   612,   616,   617,   621,   625,   627,   629,   631,   633,
     637,   639,   643,   645,   649,   656,   660,   664,   669,   671,
     675,   677,   681,   682,   685,   686,   688,   690,   694,   696,
     698,   700,   704,   705,   712,   714,   716,   720,   721,   725,
     729,   730,   734,   736,   743,   747,   749,   754,   760,   761,
     763,   769,   771,   772,   776,   777,   783,   788,   790,   792,
     794,   798,   799,   803,   806,   811,   813,   815,   817,   820,
     825,   831,   833,   840,   844,   846,   849,   851,   853,   855,
     857,   859,   861,   863,   865,   867,   876,   880,   882,   884,
     886,   892,   896,   900,   902,   908,   915,   920,   925,   930,
     938,   940,   944,   946,   947,   953,   959,   961,   963,   967,
     969,   973,   975,   979,   981,   984,   985,   992,  1010,  1012,
    1016,  1020,  1021,  1025,  1026,  1030,  1031,  1035,  1039,  1040,
    1043,  1047,  1055,  1062,  1127,  1132,  1133,  1136,  1140,  1142,
    1145,  1146,  1150,  1152,  1154,  1156,  1158,  1162,  1164,  1167,
    1168,  1172,  1173,  1175,  1179,  1185,  1189,  1190,  1193,  1194,
    1197,  1199,  1203,  1204,  1205,  1208,  1209,  1210,  1216,  1221,
    1226,  1228,  1230,  1234,  1235,  1239,  1240,  1247,  1251,  1255,
    1264,  1270,  1271,  1274,  1276,  1281,  1283,  1288,  1294,  1300,
    1304,  1310,  1311,  1312,  1313,  1314,  1315,  1318,  1320,  1323,
    1324,  1328,  1333,  1334,  1335,  1340,  1342,  1350,  1354,  1356,
    1360,  1361,  1362,  1363,  1366,  1370,  1374,  1381,  1388,  1393,
    1396,  1398,  1400,  1402,  1406,  1409,  1413,  1416,  1419,  1422,
    1431,  1442,  1452,  1462,  1472,  1474,  1478,  1480,  1484,  1485,
    1489,  1495,  1496,  1500,  1505,  1507,  1509,  1513,  1515,  1519,
    1520,  1524,  1528,  1529,  1532,  1537,  1539,  1543,  1545,  1547,
    1551,  1556,  1557,  1560,  1564,  1566,  1568,  1572,  1573,  1577,
    1578,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1594,  1595,  1597,  1601,  1602,  1603,
    1604,  1607,  1609,  1613,  1617,  1621,  1626,  1630,  1634,  1636,
    1640,  1644,  1650,  1655,  1660,  1661,  1662,  1664,  1668,  1672,
    1674,  1678,  1680,  1682,  1687,  1690,  1694,  1695,  1699,  1704,
    1705,  1709,  1711,  1713,  1715,  1719,  1720,  1721,  1724,  1728,
    1732,  1736,  1779,  1784,  1786,  1788,  1792,  1794,  1796,  1802,
    1810,  1812,  1816,  1817,  1821,  1827,  1834,  1857,  1867,  1871,
    1879,  1884,  1887,  1895,  1905,  1915,  1925,  1927,  1929,  1933,
    1937,  1939,  1943,  1945,  1947,  1949,  1951,  1953,  1955,  1957,
    1959,  1961,  1963,  1965,  1967,  1969,  1971,  1975,  1977,  1981,
    1987,  1991,  1993,  2000,  2002,  2004,  2007,  2010,  2012,  2014,
    2018,  2019,  2023,  2025,  2027,  2029,  2031,  2033,  2035,  2039,
    2040,  2044,  2046,  2048,  2050,  2056,  2059,  2061,  2063,  2066,
    2071,  2079,  2080,  2081,  2082,  2083,  2084,  2085,  2086,  2087,
    2088,  2089,  2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,
    2098,  2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,
    2108,  2109,  2110,  2111,  2112,  2113,  2114,  2116,  2117,  2118,
    2120,  2121,  2124,  2126,  2130,  2132,  2137,  2143,  2145,  2147,
    2151,  2153,  2157,  2160,  2163,  2168,  2172,  2174,  2178,  2180,
    2187,  2191,  2192,  2196,  2198,  2200,  2202,  2204,  2211,  2221,
    2222,  2226,  2227,  2233,  2237,  2239,  2241,  2243,  2245,  2247,
    2249,  2251,  2253,  2255,  2257,  2259,  2261,  2268,  2269,  2272,
    2273,  2276,  2281,  2288,  2289,  2293,  2302,  2308,  2309,  2312,
    2313,  2314,  2315,  2332,  2337,  2342,  2361,  2378,  2385,  2386,
    2393,  2398,  2405,  2412,  2421,  2425,  2431,  2435,  2439,  2445,
    2449,  2456,  2462,  2468,  2476,  2481,  2486,  2493,  2494,  2495,
    2498,  2499,  2502,  2503,  2504,  2511,  2515,  2526,  2532,  2586,
    2648,  2649,  2656,  2669,  2674,  2679,  2686,  2688,  2695,  2696,
    2697,  2701,  2704,  2707,  2715,  2716,  2717,  2720,  2724,  2728,
    2730,  2734,  2738,  2739,  2742,  2746,  2750,  2751,  2754,  2756,
    2760,  2761,  2765,  2769,  2770,  2774,  2775,  2779,  2780,  2781,
    2782,  2783,  2784,  2787,  2789,  2793,  2795,  2799,  2801,  2804,
    2806,  2808,  2812,  2814,  2816,  2820,  2822,  2824,  2828,  2830,
    2832,  2836,  2840,  2844,  2846,  2848,  2852,  2854,  2858,  2859,
    2863,  2867,  2869,  2873,  2874,  2879,  2883,  2885,  2887,  2891,
    2895,  2897,  2899,  2901,  2903,  2905,  2907,  2909,  2911,  2913,
    2915,  2917,  2919,  2921,  2929,  2931,  2935,  2939,  2940,  2947,
    2951,  2953,  2957,  2959,  2963,  2965,  2972,  2976,  2981,  2986,
    2987,  2989,  2993,  2996,  3008,  3012,  3016,  3020,  3024,  3026,
    3028,  3030,  3034,  3036,  3038,  3042,  3044,  3046,  3050,  3052,
    3056,  3058,  3062,  3063,  3067,  3068,  3072,  3073,  3078,  3082,
    3083,  3087,  3088,  3091,  3092,  3093,  3098,  3103,  3104,  3108,
    3110,  3114,  3115,  3119,  3120,  3121,  3124,  3129,  3134,  3138,
    3139,  3142,  3146,  3150,  3153,  3158,  3160,  3164,  3165,  3170,
    3175,  3177,  3179,  3181,  3183,  3187,  3188,  3194,  3196,  3200,
    3201,  3207,  3210,  3212,  3216,  3218,  3225,  3227,  3231,  3235,
    3237,  3239,  3245,  3247,  3251,  3252,  3256,  3258,  3261,  3262,
    3266,  3268,  3270,  3274,  3275,  3279,  3281,  3287,  3289,  3293,
    3294,  3298,  3302,  3304,  3306,  3310,  3312,  3316,  3318,  3324,
    3327,  3335,  3342,  3346,  3354,  3355,  3358,  3364,  3372,  3373,
    3376,  3382,  3388,  3391,  3394,  3398,  3405,  3406,  3410,  3414,
    3421,  3423,  3427,  3429,  3431,  3435,  3436,  3439,  3443,  3445,
    3449,  3451,  3457,  3458,  3462,  3466,  3467,  3472,  3473,  3477,
    3481,  3482,  3487,  3488,  3491,  3496,  3501,  3504,  3511,  3512,
    3514,  3515,  3517,  3521,  3522,  3524,  3526,  3530,  3532,  3536,
    3537,  3539,  3543,  3544,  3545,  3546,  3547,  3548,  3549,  3550,
    3551,  3552,  3553,  3558,  3560,  3562,  3564,  3566,  3568,  3570,
    3572,  3578,  3580,  3582,  3584,  3586,  3588,  3590,  3592,  3594,
    3596,  3598,  3600,  3602,  3604,  3606,  3608,  3612,  3613,  3619,
    3621,  3625,  3627,  3632,  3634,  3636,  3638,  3643,  3645,  3649,
    3651,  3655,  3657,  3659,  3661,  3665,  3669,  3673,  3675,  3679,
    3684,  3689,  3696,  3701,  3706,  3715,  3716,  3720,  3727,  3728,
    3729,  3730,  3731,  3732,  3733,  3734,  3735,  3736,  3737,  3739,
    3741,  3748,  3750,  3752,  3759,  3766,  3773,  3775,  3777,  3778,
    3779,  3780,  3782,  3784,  3788,  3789,  3792,  3808,  3824,  3828,
    3830,  3834,  3838,  3839,  3843,  3844,  3848,  3850,  3852,  3854,
    3856,  3860,  3861,  3863,  3879,  3895,  3899,  3903,  3905,  3909,
    3913,  3916,  3919,  3922,  3925,  3930,  3932,  3937,  3938,  3942,
    3949,  3956,  3963,  3970,  3971,  3975,  3982,  3983,  3984,  3987,
    3989,  3991,  3994,  4001,  4010,  4017,  4026,  4028,  4030,  4032,
    4036,  4037,  4040,  4044,  4045,  4046,  4049,  4054,  4057,  4062,
    4067,  4068,  4069,  4071,  4075,  4084,  4086,  4090,  4092,  4095,
    4097,  4102,  4104,  4106,  4110,  4112,  4116,  4122,  4123,  4126,
    4130,  4133,  4137,  4138,  4141,  4143,  4147,  4149,  4153,  4155,
    4159,  4161,  4165,  4168,  4171,  4176,  4185,  4195,  4197,  4199,
    4201,  4203,  4205,  4207,  4209,  4213,  4214,  4217,  4218,  4221,
    4229,  4232,  4235,  4238,  4241,  4244,  4247,  4250,  4253,  4256,
    4259,  4262,  4265,  4268,  4271,  4274,  4277,  4280,  4283,  4286,
    4289,  4292,  4295,  4298,  4301,  4306,  4307,  4312,  4314,  4315,
    4316,  4317,  4318,  4319,  4320,  4321,  4322,  4323,  4324,  4325,
    4326,  4327,  4328,  4329,  4330,  4331,  4333,  4335,  4336,  4337,
    4339,  4341,  4342,  4343,  4344,  4345,  4346,  4347,  4348,  4349,
    4350,  4351,  4352
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
  "begin_trigger", "end_trigger", "stop_trigger", "check_opt", 
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
       1,     4,     1,     3,     1,     2,     1,     1,     1,     1,
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
       0,     0,     0,   956,   380,   380,   969,   970,   978,   979,
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
       0,     0,  1066,  1066,  1066,     0,   974,   975,     0,   960,
     957,   958,     0,     0,  1053,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   939,   938,     0,
     788,   962,     0,   333,  1092,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,   537,   560,
       0,     0,   370,     0,   278,     0,   834,     0,     0,    44,
      73,     0,     0,    26,   824,     0,   840,     0,   803,   824,
       0,     0,    47,   601,     0,   694,     0,     0,   765,     0,
     716,     0,   592,     0,   591,   993,     0,   627,   638,     0,
     635,   631,   630,     0,   645,     0,   650,   634,   641,     0,
     767,   811,     0,     0,     0,   239,   388,   377,   678,   656,
     677,     0,     0,     0,   682,     0,   798,     0,     0,   146,
     186,   505,     0,   423,     0,   422,   428,   496,     0,   495,
     378,     0,   264,   377,   238,  1075,   212,   209,     0,   152,
     378,   527,   377,     0,     0,     0,   243,   237,   240,   241,
     242,   432,   488,   487,   431,   430,     0,     0,     0,     0,
       0,   394,   392,   393,   389,     0,   378,   493,     0,   492,
     195,   172,   176,     0,     0,   180,     0,     0,   555,   568,
     569,   585,     0,   582,   587,     0,   575,   573,   574,   152,
       0,     0,     0,   538,     0,     0,     0,     0,   576,   128,
     267,     0,   377,   141,   200,     0,   203,   205,   206,     0,
       0,     0,     0,   220,   220,    79,     0,    87,    78,     0,
       0,     0,     0,   861,   849,     0,     0,   922,   923,   924,
       0,   853,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   771,   772,   766,     0,   801,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,     0,
       0,     0,     0,     0,  1048,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1031,  1033,  1032,
       0,     0,   947,   801,   946,   364,     0,   334,   366,   335,
       0,     0,   942,  1073,   941,   940,   943,   944,   945,   963,
    1035,     0,   952,   844,   843,   374,   373,     0,   271,   286,
       0,   277,   279,   836,     0,     0,     0,    71,     0,   829,
       0,   839,     0,   829,     0,     0,   703,   706,     0,   718,
     719,   722,   724,   721,   733,   734,   735,   723,   746,     0,
     758,   713,   715,  1083,   593,   994,   636,   649,   644,   643,
       0,   654,   817,   788,   818,   847,   377,     0,     0,     0,
     657,   676,     0,   790,   696,   791,   800,   504,   165,   192,
     184,   187,   188,     0,     0,   150,   226,   221,   964,   222,
     223,   224,   225,   498,   276,   269,   278,   211,   378,   151,
     379,   213,   208,   214,     0,     0,   248,   436,   439,   379,
     437,   484,   485,   435,     0,     0,     0,   407,   512,   494,
     196,   197,   173,     0,   183,   556,   557,   992,     0,     0,
       0,   560,     0,   583,     0,     0,   379,     0,   533,   535,
       0,     0,     0,     0,     0,     0,     0,   388,   278,   165,
     143,   239,   198,   408,   382,     0,    83,     0,   220,     0,
      91,     0,    85,     0,     0,    90,    86,     0,   815,  1076,
       0,     0,   854,     0,     0,   860,     0,     0,   852,   851,
       0,   909,     0,   898,   897,     0,   876,     0,   907,   925,
       0,   917,     0,     0,     0,   877,   903,     0,     0,   880,
       0,     0,     0,     0,     0,     0,     0,   878,     0,     0,
     879,     0,   911,     0,     0,   873,     0,     0,   874,     0,
       0,   875,   770,   773,   806,     0,     0,   220,     0,   999,
       0,     0,  1022,     0,     0,     0,     0,     0,     0,     0,
     959,     0,     0,  1054,     0,     0,  1052,     0,     0,  1044,
       0,     0,     0,     0,  1055,  1016,  1017,  1018,  1023,  1019,
       0,  1030,     0,     0,   688,     0,     0,     0,   961,  1034,
       0,   266,   285,     0,   340,   369,   280,     0,     0,     0,
       0,     0,  1101,    41,    52,    54,    53,    62,    64,    41,
      42,     0,     0,     0,   821,   841,     0,   820,     0,     0,
       0,   754,   721,     0,   756,   750,   756,   754,   756,     0,
       0,     0,   717,     0,   763,   646,   647,   651,   801,   278,
     199,   408,   382,   658,     0,     0,   193,     0,   189,     0,
     965,   378,   378,   379,     0,     0,   227,     0,   248,   794,
     244,   438,   399,   400,   396,   395,   398,   397,   378,   511,
     379,   182,  1072,     0,     0,   551,     0,   550,   586,     0,
     578,   154,     0,   531,     0,   559,   560,   566,   564,     0,
     561,   377,     0,     0,     0,   135,   144,   204,     0,     0,
     379,   105,     0,   133,   132,    94,     0,     0,    95,     0,
      88,    89,     0,   915,   916,   856,   855,   858,   857,     0,
       0,     0,     0,   788,     0,     0,     0,   918,     0,     0,
       0,     0,     0,     0,   910,   908,   904,     0,   912,     0,
       0,     0,     0,   913,     0,     0,     0,     0,     0,     0,
     777,     0,   774,     0,   778,  1005,  1004,     0,     0,  1001,
    1000,     0,  1009,  1008,  1007,  1006,  1003,  1002,     0,  1026,
    1050,  1049,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   367,   337,   336,   272,     0,     0,   282,   283,
       0,  1074,   323,     0,   316,     0,     0,     0,   315,  1103,
    1108,     0,     0,   361,   297,   292,     0,   340,   291,     0,
     300,   310,   311,   307,   319,   320,   308,   317,   312,   305,
     318,     0,   314,   313,   355,   357,   356,   306,     0,   302,
     304,   303,   301,   309,   819,     0,  1074,    66,    58,    59,
      65,    60,    61,     0,    24,     0,     0,    25,     0,   825,
     826,   827,   823,   828,   831,   830,   814,    45,    46,   717,
     725,   720,   755,   753,   751,     0,   752,     0,     0,     0,
       0,     0,     0,   767,   810,     0,     0,   379,   795,   796,
     697,   195,   194,   190,   379,     0,     0,   270,   379,   379,
     216,   228,   229,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,   278,   508,   553,     0,   554,     0,     0,
     216,   153,   155,   157,   158,   159,   534,   536,   532,   563,
     278,   408,   379,   195,   379,     0,   375,   378,    92,     0,
       0,    98,    93,   560,     0,   901,     0,     0,   927,   801,
     926,   899,     0,     0,     0,   905,     0,     0,     0,     0,
     914,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   768,   773,     0,   780,     0,   776,   779,  1036,   219,
    1056,  1012,     0,     0,  1051,  1046,  1045,     0,  1039,  1041,
       0,  1028,  1029,     0,   289,   281,     0,   321,     0,     0,
     331,   343,  1084,   358,   359,     0,   293,     0,   298,   295,
     346,   299,     0,     0,     0,   339,     0,    55,    56,    57,
      63,     0,     0,   802,   728,     0,   736,   754,   745,   743,
     757,   759,   761,   762,   693,   379,   379,   376,   797,   191,
     161,   274,   275,   263,     0,     0,   207,   230,     0,     0,
     248,   232,   233,   231,   833,     0,   405,   406,   402,   401,
     404,   403,     0,   552,   579,   160,   147,   156,     0,     0,
     260,   142,   383,   381,     0,     0,    96,    97,    80,     0,
     884,   892,   687,   900,   885,   893,   888,   896,   902,   906,
     886,   894,   887,   895,   881,   889,   882,   890,   883,   891,
     775,     0,   786,  1025,  1024,  1047,     0,  1027,     0,     0,
     284,     0,   322,   324,     0,   330,     0,   354,     0,     0,
       0,     0,   349,   347,     0,     0,     0,   332,    40,   822,
     812,   730,   727,   737,     0,     0,   738,   739,   740,     0,
     261,   384,   210,   215,   248,   833,   235,   253,   247,   409,
     379,   379,   379,    77,    99,   100,     0,   783,     0,   782,
    1040,   287,   288,     0,     0,     0,     0,   353,   352,  1080,
     351,   340,     0,     0,     0,     0,     0,     0,   726,   741,
       0,   760,   236,   253,     0,   248,   249,   250,   262,   385,
       0,   781,     0,     0,     0,   327,   340,   360,   348,   350,
       0,   345,     0,   813,     0,   731,     0,   248,     0,     0,
     246,     0,   252,     0,   251,   134,   784,     0,   290,   329,
       0,     0,     0,   340,     0,   729,   742,   234,     0,     0,
     256,   255,   254,   785,   340,     0,     0,   340,   338,   732,
     259,   257,   258,   328,   340,   344,   325,   326,     0,     0,
       0
};

static const short yydefgoto[] =
{
    2028,    21,    22,   577,   151,   152,   153,   154,  1634,   863,
     858,    23,   165,   166,  1393,  1394,  1395,  1396,  1397,   155,
     156,   581,   582,    24,    89,   474,  1017,   762,   763,   764,
    1276,  1269,  1270,  1722,   296,   766,    25,    81,    26,   185,
      27,   464,   270,   376,   746,   240,    82,  1491,   277,   468,
     754,  1260,  1485,   656,   265,   459,  1209,  1210,  1700,  1701,
    1702,  1703,  1704,  1705,   272,   287,   262,   699,   263,   455,
     456,   457,   706,   707,   708,   657,   658,  1190,  1191,  1192,
    1437,  1232,   280,   380,   470,  1005,  1006,  1007,   937,   938,
     939,  1826,   681,   567,  1547,   765,  1195,  1680,  1681,  1682,
    1833,  1008,   683,   947,   948,   949,   950,  1450,  1965,  1966,
    1967,  2011,   275,   378,   748,   247,   436,   573,   676,   677,
    1127,   568,  1443,  1130,  1131,  1132,  1577,  1578,  1383,  1890,
    1579,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,  1606,  1607,  1608,  1895,  1609,   529,  1108,
    1610,  1611,  1612,  2002,  1789,  1790,  1901,  1902,  1613,  1614,
    1615,  1616,  1785,  1617,   530,   836,    29,   316,   569,   570,
     284,   384,  1010,   922,   925,   809,  1490,   282,   382,   750,
     445,   694,   695,   964,   966,  1459,    30,    65,   427,   428,
     440,   441,   237,   238,   442,  1220,   689,   954,   244,   255,
     431,   674,   929,   196,   420,   421,   252,   696,  1460,    31,
     104,   731,   732,   733,  1247,  1248,   734,   848,   736,   737,
    1241,   978,   993,   738,   849,   740,   741,   742,   743,   986,
     744,   745,   983,    32,    33,    34,    35,    36,   638,    37,
     597,    38,    39,    67,   259,   448,    40,   362,   363,   364,
     365,   366,   367,   368,   369,   891,   370,   371,   372,  1170,
    1427,   618,   619,   896,   620,    41,    42,   404,   405,   406,
     913,   909,  1618,   192,   646,   409,    44,  1512,   829,    45,
      46,    47,   600,   349,  1434,  1818,   170,   171,   172,   347,
     604,   605,   606,   881,   878,  1149,  1150,  1151,  1152,  1911,
    1958,  1984,  1153,  1154,  1155,  1156,  1916,  1917,  1918,  1157,
    1422,   176,   177,  1420,  1653,  1424,  1810,  1811,  1663,   478,
     795,  1540,  1055,  1541,  1542,  1543,  1756,  1936,  1939,   195,
     648,   649,   292,  1670,  1185,   415,  1619,  1620,    50,    51,
    1621,    53,    54,  1803,   479,   627,  1622,  1623,    55,  1139,
    1638,  1639,  1404,  1643,   319,   320,   574,   588,   589,   865,
     531,   532,   630,   778,  1030,   779,   780,   781,   782,  1295,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
    1032,  1298,  1299,   793,   534,   810,   535,   536,  1197,   537,
     538,   539,   540,   541,   542,   543,  1641,    86,   294,   279,
    1239,  1249,   885,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,  1763,   554,  1101,   555,   556,   557,   558,
     559,   560,  1087,   561,   815,  1357,   816,  1354,   562,  1079,
     348,   413,   563,   564,   768,  1461,  1112,   268,   934,  1278,
     300,   241,   297,  1948,   273,  1937,  1162,  1625,   642,  1158,
     157,   565,   251,   253,  1398,   833,   285,   186,   566,   159
};

static const short yypact[] =
{
    2701,   324,    46,  1634,   226,   214,   412,  1706,   120,  8384,
     421,   279,  1635,    46,   741,   405,  7062,   542,  7062,   292,
     481,   259,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   556,   519,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  9463,  7062,   602,
    7062,  7062,  7062,  7062,  7062,-32768,-32768,   600,   573,  7062,
    7062,  7062,   672,  7062,   570,  7062,  7062,-32768,  7062,  7062,
    7062,-32768,   502,-32768,   620,-32768,-32768,   760,  7062,-32768,
    7062,  7062,  7062,   789,  7062,  7062,  7062,  7062,   570,  7062,
    7062,  7062,  7062,  7062,-32768,  7062,   596,   729,-32768,-32768,
  -32768,   604,-32768,   604,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   790,   631,   805,-32768,    29,-32768,-32768,-32768,-32768,
    7062,  7062,  7062,   818,   828,   232,  7062,   588,   414,   432,
     656,   699,-32768,  7062,   884,  1718,   813,-32768,  7062,-32768,
    7062,  7062,  7062,  7062,  7062,-32768,-32768,-32768,  7062,  2663,
  -32768,   796,   799,   977,   705,   740,   814,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   356,-32768,-32768,
  -32768,   573,-32768,   827,-32768,   627,-32768,-32768,   742,-32768,
     748,-32768,-32768,   647,-32768,-32768,   768,-32768,   879,-32768,
  -32768,   839,-32768,   929,-32768,-32768,  1032,-32768,-32768,-32768,
  -32768,   573,-32768,-32768,   187,-32768,   742,-32768,-32768,   607,
  -32768,  1001,-32768,  1007,-32768,   604,-32768,-32768,-32768,-32768,
  -32768,-32768,   993,-32768,-32768,   827,-32768,   999,-32768,   891,
  -32768,-32768,-32768,   827,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  8001,  7062,   937,-32768,  7062,-32768,
  -32768,-32768,   943,   184,   946,  4894,  7062,  1041,   834,  1066,
    1084,  1091,  1031,  1040,    99,   932,-32768,-32768,  8272,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  6104,-32768,-32768,
  -32768,  1010,  7062,  1073,   162,   142,  7062,  1016,-32768,   919,
     907,   938,-32768,  1718,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  7062,-32768,-32768,-32768,   573,-32768,   742,
  -32768,  1001,-32768,  1138,-32768,   604,  1104,  1072,-32768,-32768,
  -32768,  1166,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1151,  7062,  7062,  1114,   994,-32768,
    8272,  1083,-32768,  1083,  8272,-32768,    98,   985,   992,   995,
     814,-32768,   747,   798,  1207,  7062,-32768,   356,-32768,-32768,
  -32768,-32768,  1201,-32768,-32768,  7062,   937,  3232,  1048,  3515,
     971,-32768,  9463,-32768,-32768,   673,  1070,-32768,-32768,-32768,
     981,   981,   573,  1155,   573,   194,   929,-32768,-32768,  2734,
  -32768,  7062,  7062,  7062,-32768,   937,-32768,-32768,  1239,   442,
     982,  7062,  1260,  7062,-32768,    73,  6249,  4749,  1141,-32768,
  -32768,   987,   991,   997,  1094,-32768,-32768,  1003,-32768,  1005,
  -32768,  1008,  1013,-32768,  1014,-32768,-32768,-32768,-32768,  1019,
    1021,  1101,  1120,-32768,  1028,  1028,-32768,-32768,-32768,-32768,
    1029,-32768,-32768,  6375,  1033,  1034,-32768,  1036,  1180,  1045,
    1046,  1047,  1049,  1053,  1056,  8272,  8272,  6646,  7062,-32768,
    1177,  1054,-32768,  1378,-32768,-32768,  1011,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1059,  1055,  1057,  2734,  1074,   654,
  -32768,-32768,  1061,  1346,   657,-32768,-32768,  7062,-32768,  7062,
    7062,    76,-32768,-32768,-32768,-32768,  7062,  7062,   703,-32768,
    7062,-32768,-32768,   943,  1222,  4894,-32768,  7062,  6646,   228,
  -32768,-32768,  8272,-32768,  1301,  1075,-32768,   171,-32768,   288,
  -32768,   397,  1085,-32768,   129,-32768,   129,-32768,  1077,-32768,
     125,-32768,  1199,   570,-32768,-32768,-32768,    43,-32768,-32768,
    1086,  1087,-32768,   937,  1080,  7062,  1366,-32768,-32768,-32768,
  -32768,   362,  1303,-32768,  1092,  7062,-32768,-32768,  1100,-32768,
     202,-32768,-32768,   756,-32768,  1294,  1299,-32768,   573,-32768,
    1304,-32768,-32768,-32768,  1376,-32768,-32768,-32768,-32768,-32768,
  -32768,  2734,-32768,   573,  1285,  2734,   663,-32768,  1397,  7062,
  -32768,  8974,-32768,   578,-32768,  7062,   759,   748,  1235,   166,
    1275,  1240,-32768,-32768,-32768,   201,  1276,   361,-32768,  1217,
     570,-32768,   573,-32768,  1398,  1330,-32768,   194,-32768,-32768,
  -32768,    38,  1241,-32768,-32768,-32768,  1242,-32768,  1286,  1132,
  -32768,  1338,  1137,   220,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1135,-32768,-32768,-32768,   117,
    1140,  1144,    55,-32768,  1137,-32768,-32768,   573,-32768,   742,
  -32768,  1373,  1432,-32768,   573,   573,-32768,  3232,   647,-32768,
    1322,-32768,  1296,  1154,-32768,  3920,  1326,-32768,-32768,-32768,
    1335,  1162,  5020,  1165,  1320,  1374,  1483,  4478,   351,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1918,   625,  1333,   728,  8272,   143,-32768,
    7062,  8272,   739,   762,   765,  1379,-32768,-32768,   570,-32768,
  -32768,-32768,  8272,  5291,  1378,   173,  1280,  8272,  8272,  5291,
    1403,  8272,  8272,  8272,  8272,  8272,  5562,   157,   157,  1176,
     519,   264,   674,-32768,-32768,  5165,-32768,  8272,  7062,  8272,
    8272,  8272,  8272,  8272,  8272,  6917,  4623,-32768,-32768,  1450,
    1174,  7062,-32768,  7062,  1434,  7062,-32768,  1300,  1309,-32768,
  -32768,  1283,  4894,-32768,  1418,   685,-32768,  1191,-32768,  1418,
    7062,  7062,  1196,-32768,   264,-32768,   366,  5707,  1305,  8272,
  -32768,  7062,-32768,   677,-32768,-32768,   167,-32768,-32768,  1310,
  -32768,-32768,-32768,  7062,   538,  7062,-32768,-32768,-32768,  7062,
    1141,-32768,  8272,  7062,  1479,  3232,   647,-32768,-32768,-32768,
  -32768,   362,  7062,  1408,  1209,  8272,  1257,  8272,   573,-32768,
     485,-32768,  1206,-32768,  1945,-32768,-32768,-32768,   573,-32768,
  -32768,  7062,-32768,-32768,-32768,-32768,  1484,-32768,  1211,  1207,
  -32768,  1475,-32768,  1419,  1421,   604,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  7062,  1465,  1207,  9178,
     570,  1384,  1385,  1387,-32768,   570,-32768,-32768,   679,-32768,
     878,-32768,-32768,  1357,   573,-32768,  6249,    48,  1361,-32768,
  -32768,-32768,   570,-32768,  1132,   462,-32768,-32768,-32768,  1207,
     462,  1362,   462,-32768,   570,   570,  1224,   570,-32768,-32768,
     937,  7062,-32768,  1422,-32768,   688,-32768,-32768,-32768,   673,
    1083,  7062,   730,    59,-32768,-32768,  1228,   447,-32768,  6249,
    7062,  1083,  5833,-32768,-32768,  1083,  5020,-32768,-32768,-32768,
     133,-32768,   400,  1802,  5291,  5291,  8272,  8272,  2115,  1229,
     645,  2411,  8272,  2792,   548,  3075,  3358,  7188,  3641,  3924,
    4207,-32768,-32768,  1415,-32768,  1231,   740,  8272,  8272,   191,
    8272,  1236,  8272,  1237,   471,  8272,  8272,  8272,  8272,  8272,
    8272,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1468,
    1243,   304,   176,  8272,-32768,  5291,  8272,   181,   454,   698,
     132,  7062,   511,   532,   546,   671,   726,-32768,-32768,-32768,
     239,  7459,-32768,   740,-32768,-32768,  5436,-32768,  1244,-32768,
    1246,   -12,-32768,-32768,  1510,   157,   157,   876,   876,  1378,
  -32768,   800,-32768,-32768,-32768,-32768,-32768,   804,-32768,  1354,
    1523,  1434,-32768,-32768,  9288,  9288,  1416,-32768,   180,  1435,
    7062,-32768,  8272,  1435,  1482,  1486,-32768,-32768,  4081,  1254,
    1078,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1258,  5291,
    1476,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     491,-32768,-32768,  1333,  1378,-32768,-32768,   807,   673,  1083,
  -32768,-32768,   362,-32768,-32768,-32768,  1378,-32768,   981,  1545,
  -32768,   485,-32768,  5291,   835,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   101,-32768,  1434,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   604,   604,  1328,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   340,   185,   801,-32768,  1549,-32768,
  -32768,-32768,-32768,  7062,-32768,-32768,-32768,-32768,   226,   843,
    1405,  1450,  1281,-32768,  1217,   846,-32768,   616,-32768,  1279,
    7062,   617,  1291,  1293,   570,  1306,  1587,   647,  1434,   981,
    1299,  3232,-32768,  1276,  1406,  2949,-32768,   570,  1478,  1560,
  -32768,  3920,-32768,   570,    42,-32768,-32768,  1561,-32768,-32768,
    1323,  1331,-32768,  5291,  5291,-32768,  5291,  5291,-32768,  1606,
     182,  1378,  1327,-32768,-32768,  1336,  1378,  6646,-32768,-32768,
    1567,-32768,   224,  1340,  1341,  1378,  1163,  1342,  1347,  1378,
    8272,  8272,  1229,  8272,  7730,  1351,  1352,  1378,  1353,  1363,
    1378,  8272,  1378,  1364,  1380,  1378,  1381,  1383,  1378,  1388,
    1390,  1378,-32768,  9377,-32768,   815,   824,-32768,   860,-32768,
     870,  8272,-32768,   883,   906,   970,   984,   998,  1128,  8272,
  -32768,  8272,  8272,  1378,  1580,   200,  1378,  1474,  8272,-32768,
    8272,  8272,  8272,  8272,-32768,-32768,-32768,-32768,-32768,-32768,
    8272,-32768,  8272,   832,-32768,   873,  5436,  6791,-32768,-32768,
    7062,-32768,-32768,  7062,  8635,-32768,-32768,  7062,  7062,  7062,
    7062,  7062,  7062,  1477,  1386,-32768,  1392,-32768,-32768,  1477,
  -32768,   431,   431,   246,-32768,-32768,   874,-32768,  9288,  9288,
    1391,  1097,  1393,  5707,  1550,-32768,  1550,   676,  1550,  1595,
    1601,  8272,  1169,  1669,  1617,-32768,-32768,-32768,   740,  1434,
  -32768,  1276,  1406,-32768,   576,  1217,  1562,  1217,-32768,   135,
  -32768,-32768,-32768,-32768,  1523,  8272,   545,  1548,  1328,   502,
  -32768,-32768,  1569,  1570,  1571,  1572,  1573,  1574,-32768,-32768,
  -32768,-32768,-32768,  1407,   226,-32768,  1504,-32768,-32768,   462,
  -32768,   379,   462,-32768,   462,-32768,  1450,-32768,-32768,  1410,
  -32768,-32768,   673,  1523,  1217,-32768,  1299,-32768,  1549,  1685,
  -32768,-32768,  1417,-32768,  1209,-32768,  1414,   573,   147,  1420,
  -32768,-32768,   573,-32768,-32768,-32768,  1606,-32768,  1606,  8272,
    1083,  1083,  1423,   519,   911,  8272,  1646,-32768,  1083,  1083,
    8272,  1083,  1083,   198,  1378,-32768,  1290,  8272,  1378,  1083,
    1083,  1083,  1083,  1378,  1083,  1083,  1083,  1083,  1083,  1083,
  -32768,  1425,  1424,   857,  7062,-32768,-32768,  1426,  2734,-32768,
  -32768,  1178,-32768,-32768,-32768,-32768,-32768,-32768,  1192,  1349,
  -32768,-32768,  8272,  8272,  1660,  1539,  1238,   915,   786,  1266,
    1274,  8272,-32768,-32768,-32768,-32768,  2734,  1430,-32768,-32768,
    1677,  1686,  7062,   138,-32768,  1431,  1641,  8272,-32768,  1436,
    5979,  7062,  7062,-32768,-32768,-32768,  1674,  8519,-32768,  1437,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   155,-32768,-32768,-32768,-32768,-32768,-32768,  1647,-32768,
  -32768,-32768,-32768,-32768,-32768,  1438,   592,-32768,-32768,-32768,
  -32768,-32768,-32768,  1664,-32768,  9288,  9288,-32768,  1563,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1457,-32768,-32768,  1726,
  -32768,  1078,-32768,-32768,-32768,  1653,-32768,  5707,  5707,   916,
    7062,  8272,  5291,  1141,-32768,  1523,  1549,-32768,-32768,-32768,
  -32768,   878,-32768,-32768,-32768,  1945,  1945,-32768,-32768,  1378,
    1716,   545,-32768,   526,  7062,-32768,  1661,  1564,  1665,  1568,
    1697,  1667,  1705,  1434,-32768,-32768,  1456,-32768,  1458,  1657,
    1716,   379,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1434,  1276,-32768,   878,-32768,  1629,-32768,-32768,-32768,  1644,
     103,-32768,-32768,  1450,  1645,  1378,  1463,  1464,-32768,   740,
  -32768,  1378,  8272,  1466,  1469,  1378,  1470,  1492,  8272,  8272,
    1378,  1493,  1494,  1503,  1505,  1506,  1514,  1515,  1517,  1518,
    1524,-32768,  9377,  1485,-32768,  7062,-32768,-32768,-32768,-32768,
  -32768,-32768,  8272,  1528,-32768,-32768,-32768,  8272,-32768,-32768,
    8272,-32768,-32768,  1282,   118,-32768,  1701,  8272,  8272,  5291,
    1378,-32768,-32768,-32768,-32768,  7062,-32768,   193,-32768,  1588,
  -32768,-32768,   153,  1511,  5436,-32768,  1699,-32768,-32768,-32768,
  -32768,   431,  8272,-32768,  7062,  5707,-32768,   547,-32768,-32768,
    1532,-32768,  1378,   351,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1533,  7062,-32768,-32768,  1712,  7062,
    1328,-32768,-32768,-32768,   604,  7062,-32768,-32768,-32768,-32768,
  -32768,-32768,  1523,-32768,-32768,-32768,-32768,-32768,  1523,  1549,
  -32768,-32768,-32768,-32768,  8272,   573,-32768,  1624,-32768,   573,
  -32768,-32768,-32768,  1378,-32768,-32768,-32768,-32768,  1378,  1378,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  7062,  1750,  1378,-32768,-32768,  1334,-32768,  1945,  1945,
  -32768,  1538,  1378,   893,   146,-32768,  1752,-32768,  7062,  7062,
     226,   131,-32768,-32768,  1590,  1757,  5291,  1244,-32768,-32768,
      83,  1551,-32768,-32768,  5291,  1552,-32768,-32768,-32768,  8272,
  -32768,-32768,-32768,-32768,  1328,   604,-32768,  1720,-32768,-32768,
  -32768,-32768,  1378,-32768,-32768,-32768,  1554,  1557,  1553,-32768,
  -32768,-32768,-32768,  1083,  5436,  1670,  5436,-32768,-32768,-32768,
  -32768,  8770,   193,  8272,  5436,   150,  5436,  7062,-32768,   351,
    7062,-32768,-32768,  1720,   234,  1328,  1728,  1731,-32768,-32768,
    1565,-32768,  7062,  7062,  1566,  1244,  8770,  1244,-32768,-32768,
    1365,   126,  1807,  1244,   927,-32768,   931,  1328,   572,   572,
  -32768,  1812,-32768,  1678,-32768,-32768,-32768,  1576,-32768,  1815,
    5436,  1821,  1819,  8770,  7062,-32768,-32768,-32768,  1666,   638,
  -32768,-32768,-32768,-32768,  8770,   151,  7062,  8770,-32768,-32768,
  -32768,-32768,-32768,-32768,  8770,-32768,-32768,-32768,  1868,  1871,
  -32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1288,  1708,-32768,  1709,  1556,   476,-32768,
    1012,-32768,-32768,-32768,  -972,-32768,  -757,-32768,  -632,  1710,
    1558,  1292,  1024,-32768,-32768,-32768,   621,-32768,  1127,   877,
  -32768,-32768,   626,-32768,-32768,   881,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   635,-32768,-32768,  -349,   908,-32768,-32768,
     195,-32768,   218,  -401,  1824,-32768,-32768,  -408,-32768,-32768,
  -32768,  1451,-32768,-32768,  1202,  -421,-32768,-32768,-32768,   717,
  -32768, -1036,-32768,-32768,  1529,  1009,   652,  1480,   974,   651,
  -32768,   219,   -50,  -311,-32768,  -367, -1365,-32768,-32768,   242,
  -32768,  1487, -1299,-32768,-32768,-32768,-32768, -1380,   -38,   -39,
     -40,   -59,-32768,-32768,-32768,-32768,  -159,  -334,-32768,  1000,
  -32768,  -779,-32768, -1088,-32768,   802,-32768,-32768,-32768,-32768,
  -32768, -1468, -1052,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -813, -1011,
  -32768,-32768,-32768,-32768,-32768,   140,-32768,   -20,-32768,-32768,
  -32768,-32768,-32768,  1934,-32768,-32768,-32768,-32768,-32768,  1088,
  -32768,-32768,  -807,  -610,  -992,-32768,   504,-32768,-32768,-32768,
    -651,  -887,-32768,-32768, -1133, -1340,-32768,-32768,-32768,  1513,
  -32768,  1250,  1496,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1525,-32768,-32768,-32768,-32768,
  -32768,  -643,  -525,-32768,   951,   475,  -657,  -432,-32768,-32768,
  -32768,-32768, -1121,-32768,  -431,-32768,-32768,-32768,-32768,  1208,
  -32768,-32768,   966,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1940,  1788,-32768,-32768,-32768,-32768,  1593,
  -32768,-32768,-32768,  1348,-32768,  1344,-32768,-32768,-32768,-32768,
  -32768,-32768,  1064,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  -744,     1,-32768,-32768,-32768,  -515,   541,-32768,  -482,
     682,-32768,  -237,   428,-32768,-32768,-32768,-32768,  1787,-32768,
  -32768,-32768,  1093, -1240,-32768,-32768, -1042, -1460,-32768,-32768,
  -32768,-32768,   822,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1872,    22,   549,  -310,-32768,-32768,    54,-32768,  -468,
    -860,  1182,-32768,   222,-32768,   433,-32768,  -954,-32768,  -718,
  -32768,  1063,  -600,-32768,-32768,  -993,  1979,  1980,-32768,-32768,
    1981,-32768,-32768,-32768,  1359,-32768,  -274,-32768,-32768,  1118,
   -1270,-32768,   845,-32768,  -108,  -808,  -628,-32768,-32768,-32768,
    -810,   -49,  -522,  -782,-32768,  -581,-32768,-32768,-32768,    21,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -627,
  -32768,   678,-32768,  1205,  -894,  1491,-32768,  -503,-32768,   795,
    -888,  -158,  -883,  -809,  -805,   -54,    -2,   -41,-32768,   -69,
   -1109,  -537,  -156,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768, -1308,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   632,-32768, -1225,-32768,-32768,
     513,-32768,  -799,   461,  1015,   749,   175,  -748,  1313, -1278,
  -32768,   -66,  1909,-32768,    39,   -52, -1721,   419,  -297,   -58,
     -56,  -162,   -10,   -44,  -290,-32768,   -83,   -60,    -9,-32768
};


#define	YYLAST		9746


static const short yytable[] =
{
     158,    43,    85,   248,   271,   321,   178,   179,   245,   187,
     340,   343,   341,   344,   264,   276,   374,   914,   311,   266,
     267,   665,  1107,   288,   832,  1109,   302,   735,   739,   308,
    1196,  1082,   283,   278,   709,   583,  1198,  1090,   940,   307,
    1173,  1199,   847,   700,   306,   830,   312,   314,   239,   242,
     916,   246,   249,   179,   254,   257,   310,   278,   924,   671,
     269,   242,   257,  1334,   249,   866,   179,   254,  1685,   286,
     158,   257,   884,   868,  1128,   669,  1624,  1056,  1385,   298,
     178,   179,   301,   242,   250,   298,   257,   246,   249,   309,
     179,   254,   286,   158,   257,  1375,   249,   281,  1124,   628,
    1179,   385,   678,   256,   601,  1580,  1411,  1009,  1018,   644,
    1374,   327,  1103,   329,   377,  1200,   830,   465,  1444,  1201,
    1467,   989,  1263,   379,   675,  1202,  1206,  1560,   386,  1788,
    1488,   752,  1642,  1564,   305,  1213,  2001,  1216,   991,   383,
    1034,  1283,   313,  1034,  1441,  1024,   926,  1683,  1714,   345,
     930,   269,   179,   269,  1034,  1175,   941,   158,  1034,   900,
     969,  1888,   595,  1399,   257,  1956,  1720,  1180,   614,   375,
    1483,   242,   249,   179,   254,   286,   612,   472,  1951,   187,
     838,   880,  1660,   328,  1034,   839,   894,   429,   654,  1060,
    1509,  1023,  1267,   614,   838,  1258,  1031,  1806,  2024,   839,
    1856,  1337,  1897,   325,   381,   838,  1738,   976,  1034,  1904,
     839,   631,   351,   290,   838,   957,  1792,   460,  1446,   839,
     633,   838,   -81,  1083,  1084,   838,   839,  1451,   108,  1454,
     839,  1358,  1359,   477,  -714,   919,  1985,   704,   107,    66,
     996,   987,   461,  1401,  1237,   961,   889,   291,  1898,   735,
     739,   838,  1288,  1289,  1471,  1178,   839,  1035,  1284,  1899,
    1035,   105,   838,   923,   110,   615,   571,   839,   861,   575,
    1516,  1035,  1500,    87,    88,  1035,   108,   636,  1988,   105,
     844,   962,   946,  2019,   654,  1378,   709,   838,   109,  1624,
     860,  1431,   839,  1107,  1645,   830,  1109,   162,  1666,   904,
     610,  1035,  1370,  1355,  1501,   583,   269,   988,    14,   269,
    1821,  1822,   110,  1783,  1784,   890,   584,   158,  1517,   616,
    1204,   326,   958,   632,   629,  1035,  1816,   838,   462,   111,
    1211,   977,   839,  1857,  1111,   899,   617,  1764,  1765,   112,
    1238,  1665,  1121,   246,   616,  1913,  1793,   179,   997,  1352,
     613,  1721,  1268,   655,  1402,  1708,  1228,  1697,  1900,  1034,
     422,    56,  1085,   463,   113,  1455,   761,  1351,   862,  1429,
    1360,  1651,    57,  1562,   959,   844,   670,   111,   621,    58,
     106,   963,  1683,    59,  1778,  1442,   571,   112,   267,   838,
     686,   326,  1678,   690,   839,   747,   643,   643,   701,  1282,
     703,   663,  1889,    60,   749,   423,  1447,  1448,  1286,  1804,
    1160,  1439,   113,   992,  1989,   756,   269,   895,  1376,   751,
    1452,   760,    83,  1952,  1363,   631,   269,  1285,   269,  1674,
     269,  1712,  1061,   691,  1221,  1664,  1647,  1648,  1433,   160,
    1945,  1023,  1237,  1376,  1982,  1023,   842,   843,  1245,   705,
    1926,  1677,   242,   249,   254,  1428,   908,  1493,   840,   841,
     842,   843,   179,  -714,   246,    61,  1274,   769,  1694,   840,
     841,   842,   843,   173,   767,    90,  1035,   838,   840,   841,
     842,   843,   839,  1978,   337,   840,   841,   842,   843,   840,
     841,   842,   843,   758,   838,  1264,    62,  1699,  1716,   839,
      63,  1505,  1506,   161,  1507,  1508,  1280,  1896,  1999,  1931,
    1281,   882,   424,   290,    84,   840,   841,   842,   843,   834,
    1453,   859,   875,  1941,  1942,  1287,   840,   841,   842,   843,
     425,  1909,   755,  1371,   838,  2018,   188,   873,   629,   839,
     886,  1275,  1885,   970,  1962,   339,  2023,   291,   663,  2026,
     190,   840,   841,   842,   843,   838,  2027,   337,  1104,   357,
     839,   260,   261,  1573,   426,  1310,  1574,  -239,   179,   838,
     249,   584,   583,   174,   839,   679,   883,   179,  1849,  1311,
     175,   179,   898,  1188,   967,  1990,   584,   910,   187,    64,
    1000,   840,   841,   842,   843,  1711,   575,   180,  1445,   857,
     663,  1575,  1858,   189,   920,  1842,  1482,  2007,   864,  1219,
     337,  1414,   869,  1815,  1018,  1181,  1807,   191,  1405,   927,
     675,   466,  1848,  1312,  1415,  1172,   179,    83,   337,  1124,
     433,   971,   942,  1410,  1425,  1819,   269,  1494,  1146,  1406,
     943,   260,   261,  1624,  1699,   193,  1271,  1416,   972,   968,
     443,  1313,  1189,   840,   841,   842,   843,   906,   337,   278,
    1147,  1417,   345,  -239,  1432,  1828,  1256,   243,  1624,  1914,
     935,  1829,   269,  1110,  1710,  1817,   935,  1851,   692,   274,
    1168,  2021,  1820,   181,  -239,  1426,  1823,  1824,   693,  2008,
    -239,  1300,  1125,   999,   838,  1624,  1169,   194,  1418,   839,
    1003,  1004,   346,   434,   432,  1830,  1624,   338,   267,  1624,
    1051,   411,   467,   339,   182,  1314,  1624,   944,   183,    84,
    1850,   838,  1852,   444,  -239,   342,   839,  1165,  2009,   184,
     339,   339,  2022,  1052,   345,   258,  1862,   631,  2025,  1301,
    1414,   840,   841,   842,   843,   345,  1361,   258,   269,   838,
    1244,   412,   437,  1415,   839,   438,   475,   945,   840,   841,
     842,   843,   335,  1302,   480,  1342,    83,  1080,   345,   663,
     602,   345,   260,   261,  1057,  2010,  1416,   664,   608,   838,
    1435,  1196,  1196,  1907,   839,  1065,  1915,  1198,  1198,  1053,
    1929,   257,  1199,  1199,  1514,  1729,  1930,   439,   840,   841,
     842,   843,   571,  1814,   571,  1365,  1133,  1668,  1067,   838,
    1419,  1069,  1165,   859,   839,  1513,   293,  1669,   416,   840,
     841,   842,   843,  1920,  1921,   295,  1366,  1418,   666,  1113,
     417,  1675,  1676,   840,   841,   842,   843,   269,   838,  1063,
    1367,   667,   269,   839,   269,  1456,   269,   838,  1693,  1686,
     629,  1484,   839,   584,   303,   838,   267,   910,    14,  1567,
     839,   179,   249,   418,  1187,   419,  1200,  1200,   249,   317,
    1201,  1201,  1163,   337,  1203,   337,  1202,  1202,  1797,  1799,
    1813,  1457,   571,   838,   179,   867,   179, -1084,   839,   315,
   -1088,  1223,  1144,   838,   269,  1571,   269,   318,   839,   838,
    1164,  1271,  1229,   643,   839,  1759,   838,  1217,  1472,  1472,
    1222,   839,   322,  1473,  1476,   621,   838,  1171,  1659,   278,
    1234,   839,   269,   323,  1227,  1252,  1253,   324,  1255,   838,
     917,   169,  1698,  1975,   839,  1977,  1753,  1707,  1968,  1969,
    1970,  1242,   330,  1981,  1110,  1983,   851,   269,   852,   855,
     269,   856,   331,   278,   278,   931,   278,   932,   840,   841,
     842,   843,   952,   352,   953,  1368,   844,   769,  1105,   373,
    1548,  1754,   168,   169,  1235,  1944,   407,  1140,   631,  1141,
    1261,  1107,  1262,  1755,  1109,   840,   841,   842,   843,  2015,
    1362,   408,   179,   838,  1196,  1196,   410,  1894,   839,   414,
    1198,  1198,   179,  1798,  1800,  1199,  1199,   838,  1230,  1231,
     769,  1279,   839,   840,   841,   842,   843,   767,  1996,  1997,
    1369,   838,  1014,  1257,  1266,   430,   839,   450,  1513,  1513,
     496,   497,   498,  1265,   446,   435,  1513,  1513,   449,  1513,
    1513,   447,   458,   840,   841,   842,   843,  1513,  1513,  1513,
    1513,  1774,  1513,  1513,  1513,  1513,  1513,  1513,   506,   507,
     469,   451,  1304,   452,  1308,   919,  1316,  1319,   471,  1324,
    1327,  1330,   473,   840,   841,   842,   843,   476,  1770,  1200,
    1200,   477,   257,  1201,  1201,   453,   572,   579,  1165,  1202,
    1202,   629,   844,   651,  1379,   652,  1380,  1627,  1381,  1261,
    1630,  1430,   840,   841,   842,   843,  1654,  1854,  1656,  1545,
     454,   840,   841,   842,   843,   576,   735,   739,  1546,   840,
     841,   842,   843,   586,  1955,   584,   584,   587,   910,   590,
    1364,  1107,  1959,  1107,  1109,  1464,  1109,  1465,  1469,   249,
    1470,  1107,  1414,  1107,  1109,   591,  1109,   840,   841,   842,
     843,   838,   592,   593,  1549,  1415,   839,   840,   841,   842,
     843,  1414,   594,  -754,  1550,  1376,   844,  1572,  1646,  -744,
     840,   841,   842,   843,  1415,   611,   596,  1552,  1416,   880,
     840,   841,   842,   843,   609,  1479,   838,  1107,   622,   624,
    1109,   839,  1417,   840,   841,   842,   843,  1416,  1495,   635,
    1553,   838,   623,   844,  1499,  1730,   839,   844,   844,  1769,
    1808,  1417,   267,   278,  1110,   838,   575,  1520,   625,  2004,
     839,  2005,   631,   855,  1462,  2006,   278,   637,   639,  1418,
    -744,   640,   278,  -744,   641,   645,  1463,   647,    14,   659,
    -744,  1462,  -744,  1640,  1640,  1644,  -744,   660,  1418,   661,
     668,  -744,   269,   673,  -744,   684,   269,   840,   841,   842,
     843,   838,   697,   687,  1554,   698,   839,   702,   753,  -744,
     759,   840,   841,   842,   843,   757,   794,   820,  1555,  1671,
     796,  1673,  1832,  -744,   797,   840,   841,   842,   843,   838,
     798,  -744,  1556,   799,   839,  -744,   800,   838,   801,  1910,
     806,   802,   839,   844,  -744,   838,   803,   804,  1631,  1058,
     839,  1062, -1070,   838,   805,  1066,  1068,  1070,   839,   807,
    -744,   808,   812,  1544,   179,   835,   817,   818,  1713,   819,
    1628,   571,  1986,  1576,   267,   629,  1632, -1113,   821,   822,
     823,  1419,   824, -1113,  1739,  1629,   825,  -744, -1113,   826,
     837, -1113,   845,   846,   853, -1088,   854,   838,   850,  -744,
    1419,  -744,   839,   871,   877,   897,   888,   879,   269,   893,
     902,   269,   838,   905,  1581,  1626,   907,   839,   584,   249,
     254,   584,   286,   158,   918,   903,   911, -1113,   838,   654,
     912, -1114,   915,   839,   921, -1113,   928, -1114,   663,   584,
     584,   838, -1114, -1113,   249, -1114,   839,   933,  -744,   956,
    1762,   960,   965,   337,  -441,   840,   841,   842,   843,   973,
     974, -1113,  1557, -1113,   981,   982,   979,   980,  -744,   984,
     985,   990,  -744,   994,  1001,   631, -1113,   995, -1113, -1113,
   -1113, -1114,  1002,  1719,  1011,  1013,  1014,  2000,  1724, -1114,
     840,   841,   842,   843,  1019,  1021,  1020, -1114,  1025,   193,
    -744,  -744,  1696,  -744,  1091,   840,   841,   842,   843,  1086,
    1102,   991,  1760,   339,  1134, -1114,  1129, -1114,  1138,   840,
     841,   842,   843,  1135,  1142,  1136,  1761, -1113,   862,  1176,
   -1114,  1182, -1114, -1114, -1114,  1159,  1166,  1184,  1809,  1193,
   -1115,   855,   936,  1207,  1208,  1214, -1115,  1215,  1218,  1224,
    1225, -1115,  1226,  1233, -1115,  1240,  1777,  1254,  1250,   533,
    1259,  1273,  1297,  1332,  1333,   840,   841,   842,   843,  1341,
    1339,  1349,  1768,   838,  1544,   179,  1376,  1350,  1382,  1384,
    1400, -1114,  1403,   599,  1377,  1408,  1413,  1423,   629,  1409,
   -1115,  1421,   607,   840,   841,   842,   843,  1436, -1115,  1458,
    1771,   840,   841,   842,   843,  1466, -1115,  1449,  1772,   840,
     841,   842,   843,   242,  1474,  1468,  1887,   840,   841,   842,
     843,  1782,  1279,  1279, -1115,  1477, -1115,  1478,  1626,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1481,  1489, -1115,
    1480, -1115, -1115, -1115, -1113, -1113, -1113, -1113, -1113, -1113,
   -1113,  1267,  1497,  1502,  1034,   650, -1113,  1503, -1113,   653,
    1510,   840,   841,   842,   843,  1504,   584,   584,  1940,  1511,
    1515,  1561,  1110,  1518,  1519,  1521,   840,   841,   842,   843,
    1522,   -49,  1912,  1640,  1529,  1530,  1531,  1563,   249,   249,
   -1115,   375,   840,   841,   842,   843,  1532,  1534, -1114, -1114,
   -1114, -1114, -1114, -1114, -1114,   840,   841,   842,   843,  1633,
   -1114,    68, -1114,  1535,  1536,   179,  1537,  1652,  1635,   -49,
    1657,  1538,    69,  1539,  1636,  1649,  1658,  1650,  1661,    70,
    1662,   -49,  1672,  1684,  1687,  1688,  1689,  1690,  1691,  1692,
    1237,  1695,    71,  1882,  1709,   163,  1834,  1715,  1718,  1732,
    1717,  1766,  1767,  1776,  1723,   -49,  1752,  1728,   814,  1751,
    1758,  1775, -1076,   161,  1779,  1786,  1927,  -341,  1791,  1794,
     827,   828,   831,  1795,  1796,  1802,   880,  1801,  1805,  1825,
    1835,  1839,  1544,   179,  1836,  1837,   246,  1840,  1838,  1841,
    1843,  1845,  1844,  1853,    91,  1855,  1859,  1860,  1861,    72,
    1864,    92,  1891,  1865,  1866,    93,    94, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115,    95,    73,  1279,  1787,  1881, -1115,
     -49, -1115,  1110,  1928,  1110,    96,  1867,  1870,  1871,   631,
     -49,    74,  1110,  1905,  1110,   375,   249,  1872,  1924,  1873,
    1874,  1933,   353,   874,  1906,  1935,    75,   876,  1875,  1876,
      76,  1877,  1878,    77,   631,   -49,  1113,  1963,  1879,  1036,
     179,    78,  1884,  1908,  1919,   838,   246,  1922,  1934,  1938,
     839,  1943,  1947,  1037,  1946,   354,  1953,   164,  1110,  1954,
      79,   631,  1964,  1976,  1957,  1960,  1973,    97,  1971,  1972,
    1991,  1925,   631,  1993,  2003,   631,  1988,  2016,  1989,  1995,
    1998,   355,   631,    98,   356,  2014,  2017,  2020,  2029,  1038,
    2013,  2030,   246,   332,   333,  1637,   334,  1039,    99,   578,
     357,   870,   100,  1145,   585,  1040,  1137,   872,  1012,   242,
    1949,  1272,  1498,   101,  1496,  1486,  1847,  1246,  1950,    80,
    1277,  1831,   629,  1041,   289,  1042,   358,   710,  1438,   975,
     634,   575,   102,  1487,  1177,  1212,  1492,   680,  1043,  1846,
    1044,  1045,  1046,  1827,   682,  1987,  1992,   629,  1994,  1903,
    2012,  1205,  1979,  1386,    28,  1036,  1667,   955,   688,  1126,
     672,   838,  1626,  1251,  1974,   662,   839,  1706,  1163,  1037,
    1243,   269,   998,   167,   629,   336,   626,  1167,   350,   887,
     892,   359,   299,   246,   246,   629,  1655,  1626,   629,  1047,
    1412,   103,  1161,  1961,  1880,   629,  1054,  1757,  1183,    48,
      49,    52,  1033,   484,   360,  1038,   901,  1143,  1407,  1440,
    1525,  1236,  1565,  1039,  1626,  1163,   811,   361,   951,  1475,
    1923,  1040,  1059,   304,     0,  1626,  1064,  1279,  1626,  1781,
       0,   486,     0,     0,     0,  1626,     0,  1081,     0,  1041,
       0,  1042,  1088,  1089,     0,     0,  1092,  1093,  1094,  1095,
    1096,  1100,     0,     0,  1043,     0,  1044,  1045,  1046,   488,
       0,     0,   533,     0,  1114,  1115,  1116,  1117,  1118,  1119,
     533,  1726,  1727,     0,     0,     0,     0,     0,     0,  1733,
    1734,     0,  1736,  1737,     0,     0,     0,     0,     0,     0,
    1741,  1742,  1743,  1744,     0,  1745,  1746,  1747,  1748,  1749,
    1750,     0,     0,     0,   607,  1047,  1048,  1049,  1050,   840,
     841,   842,   843,     0,     0,     0,  1104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1174,     0,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     650,  1292,  1186,     0,  1293,     0,   495,     0,   481,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
     496,   497,   498,     0,   260,   261,     0,   483,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,   503,   504,   505,   506,   507,
     508,   509,     0,     0,     0,     0,     0,     0,   511,   512,
       0,   486,     0,     0,   487,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1048,  1049,  1050,   840,   841,   842,   843,   488,
       0,   489,   490,     0,     0,     0,     0,     0,     0,     0,
       0,   491,     0,     0,     0,   492,     0,     0,     0,     0,
       0,     0,     0,  1194,     0,     0,     0,     0,     0,     0,
       0,  1290,  1291,  1296,     0,     0,  1305,  1306,  1309,     0,
    1317,  1320,  1322,  1325,  1328,  1331,     0,     0,     0,     0,
       0,     0,  1335,  1336,     0,  1338,     0,  1340,     0,     0,
    1343,  1344,  1345,  1346,  1347,  1348,     0,     0,  1294,     0,
     493,     0,     0,     0,     0,     0,   494,     0,  1353,     0,
       0,  1356,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1373,     0,     0,     0,
     496,   497,   498,   499,   260,   261,   115,     0,   116,   117,
     118,   119,     0,   120,   500,     0,     0,     0,     0,     0,
       0,   121,   122,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   123,   510,     0,   125,     0,   533,   511,   512,
       0,   513,   514,   515,     0,   128,   129,   516,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   517,   140,     0,   518,
     142,   143,     0,   144,     0,   519,   520,   521,     0,   522,
     523,     0,   524,   145,   146,   147,   148,   149,   150,     0,
       0,     0,   525,   526,     0,     0,     0,     0,   598,     0,
     528,     0,     0,     0,   339,     0,     0,  1303,     0,     0,
    1293,     0,     0,     0,   481,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,     0,     0,
     487,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   533,     0,     0,   488,     0,   489,   490,     0,
       0,     0,     0,     0,     0,  1523,  1524,   491,  1526,  1528,
       0,   492,     0,     0,     0,     0,  1533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1551,     0,     0,     0,
       0,     0,     0,     0,  1558,     0,  1559,  1353,     0,     0,
       0,     0,     0,  1353,     0,  1356,  1566,   533,  1568,     0,
       0,     0,     0,     0,  1294,  1569,   493,  1570,     0,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,   499,
     260,   261,   115,     0,   116,   117,   118,   119,     0,   120,
     500,     0,     0,     0,     0,     0,   533,   121,   122,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   123,   510,
       0,   125,     0,     0,   511,   512,     0,   513,   514,   515,
    1679,   128,   129,   516,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   517,   140,     0,   518,   142,   143,     0,   144,
       0,   519,   520,   521,   387,   522,   523,     0,   524,   145,
     146,   147,   148,   149,   150,     0,     0,     0,   525,   526,
     388,     0,     0,     0,   598,     0,   528,     0,     1,     0,
     339,   389,     0,     0,  1725,     0,     0,     0,   390,     0,
    1731,     0,   391,   392,     0,  1735,     2,     0,     0,     0,
       0,   393,  1740,     0,     3,     0,     0,     0,     0,     0,
       0,     4,   394,     5,     0,     6,     0,     0,     0,     0,
       7,     0,   711,     0,     0,   712,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1353,  1353,     0,
       0,     9,   713,     0,     0,   714,  1773,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1780,     0,     0,     0,   395,     0,  1307,     0,
       0,  1293,     0,     0,   396,   481,     0,     0,     0,     0,
       0,     0,   482,     0,     0,   715,     0,     0,     0,     0,
       0,   716,   717,   718,   483,   719,   720,   721,     0,   722,
     484,   485,     0,     0,     0,   397,     0,     0,     0,   398,
       0,     0,     0,     0,    11,     0,     0,   723,     0,   724,
     399,    12,     0,     0,    13,     0,    14,    15,   486,     0,
       0,   487,     0,     0,     0,     0,  1812,     0,     0,   400,
     401,     0,     0,     0,     0,     0,     0,     0,   725,     0,
       0,    16,     0,     0,     0,     0,   488,     0,   489,   490,
       0,     0,     0,     0,     0,   726,     0,     0,   491,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   727,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
      17,   403,     0,     0,     0,     0,     0,  1863,     0,     0,
       0,     0,     0,  1868,  1869,    18,    19,     0,     0,     0,
       0,     0,   728,   729,     0,  1294,     0,   493,     0,     0,
       0,     0,     0,   494,     0,     0,     0,  1883,     0,   730,
      20,     0,  1353,   495,     0,  1886,   936,     0,     0,     0,
       0,     0,  1892,  1893,     0,     0,     0,   496,   497,   498,
     499,   260,   261,   115,     0,   116,   117,   118,   119,     0,
     120,   500,     0,     0,     0,     0,     0,   533,   121,   122,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   123,
     510,     0,   125,     0,     0,   511,   512,     0,   513,   514,
     515,     0,   128,   129,   516,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   517,   140,     0,   518,   142,   143,  1932,
     144,     0,   519,   520,   521,     0,   522,   523,     0,   524,
     145,   146,   147,   148,   149,   150,     0,     0,     0,   525,
     526,  1315,     0,     0,  1293,   598,     0,   528,   481,     0,
       0,   339,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1812,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,     0,     0,   487,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,  1980,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   488,
       0,   489,   490,     0,     0,     0,   123,   124,     0,   125,
       0,   491,     0,     0,     0,   492,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,     0,     0,     0,     0,  1294,     0,
     493,     0,   318,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   679,     0,     0,     0,     0,     0,     0,     0,
     496,   497,   498,   499,   260,   261,   115,     0,   116,   117,
     118,   119,     0,   120,   500,     0,     0,     0,     0,     0,
       0,   121,   122,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   123,   510,     0,   125,     0,     0,   511,   512,
       0,   513,   514,   515,     0,   128,   129,   516,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   517,   140,     0,   518,
     142,   143,     0,   144,     0,   519,   520,   521,     0,   522,
     523,     0,   524,   145,   146,   147,   148,   149,   150,     0,
       0,     0,   525,   526,  1318,     0,     0,  1293,   598,     0,
     528,   481,     0,     0,   339,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,  1294,     0,   493,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,   685,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   517,
     140,     0,   518,   142,   143,     0,   144,     0,   519,   520,
     521,     0,   522,   523,     0,   524,   145,   146,   147,   148,
     149,   150,     0,     0,     0,   525,   526,  1323,     0,     0,
    1293,   598,     0,   528,   481,     0,     0,   339,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,  1294,     0,   493,     0,     0,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,     0,     0,
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
    1326,     0,     0,  1293,   598,     0,   528,   481,  1015,     0,
     339,   712,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1016,     0,   483,     0,   713,     0,
       0,   714,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,     0,     0,   487,     0,     0,     0,     0,     0,     0,
       0,   715,     0,     0,     0,     0,     0,   716,   717,   718,
       0,   719,   720,   721,     0,   722,     0,     0,   488,     0,
     489,   490,     0,     0,     0,     0,     0,     0,     0,     0,
     491,     0,     0,   723,   492,   724,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,     0,     0,     0,     0,     0,  1294,     0,   493,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,   727,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   260,   261,   115,     0,   116,   117,   118,
     119,     0,   120,   500,     0,     0,     0,     0,   728,   729,
     121,   122,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   123,   510,     0,   125,   730,     0,   511,   512,     0,
     513,   514,   515,     0,   128,   129,   516,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   517,   140,     0,   518,   142,
     143,     0,   144,     0,   519,   520,   521,     0,   522,   523,
       0,   524,   145,   146,   147,   148,   149,   150,     0,     0,
       0,   525,   526,  1329,     0,     0,  1293,   598,     0,   528,
     481,     0,     0,   339,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,   483,
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
    1294,     0,   493,     0,  1148,     0,     0,     0,   494,     0,
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
     598,     0,   528,     0,     0,     0,   339,     0,     0,     0,
     483,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   771,     0,     0,     0,     0,
       0,     0,     0,     0,   486,     0,     0,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,     0,   489,   490,     0,     0,     0,     0,
       0,     0,     0,     0,   491,     0,     0,     0,   492,     0,
       0,     0,     0,     0,     0,     0,  1026,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,   773,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,  1122,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,  1027,
    1028,  1029,   136,   137,     0,     0,     0,     0,   138,   517,
     140,     0,   518,   142,   143,     0,   144,     0,   519,   520,
     521,     0,   522,   523,     0,   524,   145,   146,   147,   148,
     149,   150,   481,     0,     0,   525,   526,     0,     0,   482,
       0,   777,     0,   528,     0,     0,     0,   339,     0,     0,
       0,   483,     0,     0,   770,     0,     0,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   771,     0,     0,     0,
       0,     0,     0,     0,     0,   486,     0,     0,   487,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   488,     0,   489,   490,     0,     0,     0,
     123,   124,     0,   125,     0,   491,     0,     0,     0,   492,
     126,   127,     0,   128,   129,     0,   130,   772,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,   146,   147,   148,   149,   150,     0,   773,     0,
       0,     0,  1123,     0,   493,     0,     0,     0,     0,     0,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   496,   497,   498,   499,   260,   261,
     115,     0,   116,   117,   118,   119,     0,   120,   500,     0,
       0,     0,     0,     0,     0,   121,   122,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   123,   510,     0,   125,
       0,     0,   511,   512,     0,   513,   514,   515,     0,   128,
     129,   516,   130,     0,     0,   131,   132,   774,   775,   776,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     517,   140,     0,   518,   142,   143,     0,   144,     0,   519,
     520,   521,     0,   522,   523,     0,   524,   145,   146,   147,
     148,   149,   150,   481,     0,     0,   525,   526,     0,     0,
     482,     0,   777,     0,   528,     0,     0,     0,   339,     0,
       0,     0,   483,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,   771,     0,     0,
       0,     0,     0,     0,     0,     0,   486,     0,     0,   487,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,   488,     0,   489,   490,     0,     0,
       0,   123,   124,     0,   125,     0,   491,     0,     0,     0,
     492,   126,   127,     0,   128,   129,     0,   130,  1022,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   146,   147,   148,   149,   150,     0,   773,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   496,   497,   498,   499,   260,
     261,   115,     0,   116,   117,   118,   119,     0,   120,   500,
       0,     0,     0,     0,     0,     0,   121,   122,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   123,   510,     0,
     125,     0,     0,   511,   512,     0,   513,   514,   515,     0,
     128,   129,   516,   130,     0,     0,   131,   132,   774,   775,
     776,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   517,   140,     0,   518,   142,   143,     0,   144,     0,
     519,   520,   521,     0,   522,   523,     0,   524,   145,   146,
     147,   148,   149,   150,   481,     0,     0,   525,   526,     0,
       0,   482,     0,   777,     0,   528,     0,     0,     0,   339,
       0,     0,     0,   483,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   771,     0,
       0,     0,     0,     0,     0,     0,     0,   486,     0,     0,
     487,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   488,     0,   489,   490,     0,
       0,     0,   123,   124,     0,   125,     0,   491,     0,     0,
       0,   492,   126,   127,     0,   128,   129,     0,   130,   772,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,   146,   147,   148,   149,   150,     0,
     773,     0,     0,     0,     0,     0,   493,     0,  1106,     0,
     528,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,   499,
     260,   261,   115,     0,   116,   117,   118,   119,     0,   120,
     500,     0,     0,     0,     0,     0,     0,   121,   122,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   123,   510,
       0,   125,     0,     0,   511,   512,     0,   513,   514,   515,
       0,   128,   129,   516,   130,     0,     0,   131,   132,   774,
     775,   776,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   517,   140,     0,   518,   142,   143,     0,   144,
       0,   519,   520,   521,     0,   522,   523,     0,   524,   145,
     146,   147,   148,   149,   150,   481,     0,     0,   525,   526,
       0,     0,   482,     0,   777,     0,   528,     0,     0,     0,
     339,     0,     0,     0,   483,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,     0,
       0,   487,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,   488,     0,   489,   490,
       0,     0,     0,   123,   124,     0,   125,     0,   491,     0,
       0,     0,   492,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,   146,   147,   148,   149,   150,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,   528,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   496,   497,   498,
     499,   260,   261,   115,     0,   116,   117,   118,   119,     0,
     120,   500,     0,     0,     0,     0,     0,     0,   121,   122,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   123,
     510,     0,   125,     0,     0,   511,   512,     0,   513,   514,
     515,     0,   128,   129,   516,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   517,   140,     0,   518,   142,   143,  1097,
     144,     0,   519,   520,   521,  1098,   522,   523,  1099,   524,
     145,   146,   147,   148,   149,   150,   481,     0,     0,   525,
     526,     0,     0,   482,     0,   598,     0,   528,     0,     0,
       0,   339,     0,     0,     0,   483,     0,     0,     0,     0,
       0,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     771,     0,     0,     0,     0,     0,     0,     0,     0,   486,
       0,     0,   487,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   488,     0,   489,
     490,     0,     0,     0,   123,   124,     0,   125,     0,   491,
       0,     0,     0,   492,   126,   127,     0,   128,   129,     0,
     130,  1022,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,   146,   147,   148,   149,
     150,     0,   773,     0,     0,     0,     0,     0,   493,     0,
    1148,     0,     0,     0,   494,     0,     0,     0,     0,     0,
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
     525,   526,     0,     0,   482,     0,   777,     0,   528,     0,
       0,     0,   339,     0,     0,     0,   483,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,     0,     0,   487,     0,     0,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,   488,     0,
     489,   490,     0,     0,     0,     0,   123,   124,     0,   125,
     491,     0,     0,     0,   492,     0,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,     0,     0,     0,     0,     0,   493,
    -342,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   496,
     497,   498,   499,   260,   261,   115,     0,   116,   117,   118,
     119,     0,   120,   500,     0,     0,     0,     0,     0,     0,
     121,   122,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   123,   510,     0,   125,     0,     0,   511,   512,     0,
     513,   514,   515,     0,   128,   129,   516,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   517,   140,     0,   518,   142,
     143,     0,   144,     0,   519,   520,   521,     0,   522,   523,
       0,   524,   145,   146,   147,   148,   149,   150,   481,     0,
       0,   525,   526,   603,     0,   482,     0,   598,     0,   528,
       0,     0,     0,   339,     0,     0,     0,   483,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,     0,     0,   487,    83,     0,   114,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   488,
       0,   489,   490,     0,     0,     0,   123,   124,     0,   125,
       0,   491,     0,     0,     0,   492,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,     0,     0,     0,    84,     0,     0,
     493,     0,     0,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,     0,     0,   813,     0,     0,     0,     0,     0,
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
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
    1123,     0,   493,     0,     0,     0,     0,     0,   494,     0,
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
     598,  1120,   528,     0,     0,     0,   339,     0,     0,     0,
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
       0,     0,     0,   493,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1321,     0,     0,   496,   497,   498,   499,   260,   261,   115,
       0,   116,   117,   118,   119,     0,   120,   500,     0,     0,
       0,     0,     0,     0,   121,   122,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   123,   510,     0,   125,     0,
       0,   511,   512,     0,   513,   514,   515,     0,   128,   129,
     516,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   517,
     140,     0,   518,   142,   143,     0,   144,     0,   519,   520,
     521,     0,   522,   523,     0,   524,   145,   146,   147,   148,
     149,   150,   481,     0,     0,   525,   526,     0,     0,   482,
       0,   598,     0,   528,     0,     0,     0,   339,     0,     0,
       0,   483,     0,     0,     0,     0,     0,   484,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1372,     0,     0,   486,     0,     0,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,   489,   490,     0,     0,     0,
       0,     0,     0,     0,     0,   491,     0,     0,     0,   492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
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
     482,     0,   598,     0,   528,     0,     0,     0,   339,     0,
       0,     0,   483,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,     0,     0,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   488,     0,   489,   490,     0,     0,
       0,     0,     0,     0,     0,     0,   491,     0,     0,     0,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1527,     0,     0,   496,   497,   498,   499,   260,
     261,   115,     0,   116,   117,   118,   119,     0,   120,   500,
       0,     0,     0,     0,     0,     0,   121,   122,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   123,   510,     0,
     125,     0,     0,   511,   512,     0,   513,   514,   515,     0,
     128,   129,   516,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   517,   140,     0,   518,   142,   143,     0,   144,     0,
     519,   520,   521,     0,   522,   523,     0,   524,   145,   146,
     147,   148,   149,   150,   481,     0,     0,   525,   526,     0,
       0,   482,     0,   598,     0,   528,     0,     0,     0,   339,
       0,     0,     0,   483,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   486,     0,     0,
     487,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,     0,   489,   490,     0,
       0,     0,     0,     0,     0,     0,     0,   491,     0,     0,
       0,   492,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   496,   497,   498,   499,
     260,   261,   115,     0,   116,   117,   118,   119,     0,   120,
     500,     0,     0,     0,     0,     0,     0,   121,   122,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   123,   510,
       0,   125,     0,     0,   511,   512,     0,   513,   514,   515,
       0,   128,   129,   516,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   517,   140,     0,   518,   142,   143,     0,   144,
       0,   519,   520,   521,     0,   522,   523,     0,   524,   145,
     146,   147,   148,   149,   150,   481,     0,     0,   525,   526,
       0,     0,   482,     0,   527,     0,   528,     0,     0,     0,
     339,     0,     0,     0,   483,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,     0,
       0,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,     0,   489,   490,
       0,     0,     0,     0,     0,     0,     0,     0,   491,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
     109,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,     0,     0,   496,   497,   498,
     499,   260,   261,   115,     0,   116,   117,   118,   119,     0,
     120,   500,     0,     0,     0,     0,     0,     0,   121,   122,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   123,
     510,     0,   125,     0,     0,   511,   512,     0,   513,   514,
     515,     0,   128,   129,   516,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,   111,
       0,     0,   138,   517,   140,  1384,   518,   142,   143,   112,
     144,     0,   519,   520,   521,     0,   522,   523,     0,   524,
     145,   146,   147,   148,   149,   150,     0,     0,     0,   525,
     526,     0,     0,     6,   113,   598,     0,   528,     0,     0,
    -294,   339,     0,     0,  1582,  1583,     0,  1584,     0,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,  1585,     0,     0,     0,     0,     0,  1586,
     121,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   123,   124,     0,   125,     0,     0,     0,     0,     0,
       0,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,  1384,   144,     0,     0,     0,  1587,     0,     0,     0,
       0,     0,   145,   146,   147,   148,   149,   150,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     6,
       0,     0,     0,     0,     0,     0,  -296,     0,     0,  1588,
    1582,  1583,     0,  1584,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,  1787,  1585,
       0,     0,     0,     0,     0,  1586,     0,   499,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1589,   124,     0,   125,
       0,     0,     0,     0,     0,     0,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,  1590,   133,   134,   135,
       0,     0,  1587,   136,   137,  1591,  1592,  1593,     0,   138,
     139,   140,     0,   141,   142,   143,  1384,   144,     0,     0,
      14,     0,     0,     0,     0,     0,     0,   145,   146,   147,
     148,   149,   150,     0,     0,  1588,     0,     0,     0,     0,
       0,     0,     0,     0,     6,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1582,  1583,     0,  1584,     0,
       0,     0,     0,   499,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,  1585,     0,     0,     0,     0,     0,
    1586,   121,   122,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1589,   124,     0,   125,     0,     0,     0,     0,
       0,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,  1590,   133,   134,   135,     0,     0,     0,   136,
     137,  1591,  1592,  1593,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,  1587,     0,     0,
       0,     0,     0,   145,   146,   147,   148,   149,   150,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1588,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
       0,     0,   711,     0,     0,   712,     0,  1589,   124,     0,
     125,   936,     0,     0,     0,     0,     0,   126,   127,     0,
     128,   129,   713,   130,     0,   714,   131,  1590,   133,   134,
     135,     0,     0,     0,   136,   137,  1591,  1592,  1593,     0,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   146,
     147,   148,   149,   150,     0,   715,     0,     0,     0,     0,
       0,   716,   717,   718,     0,   719,   720,   721,     0,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   723,     0,   724,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   726,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   727,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,   728,   729,     0,     0,   711,     0,     0,   712,
       0,   123,   124,     0,   125,     0,     0,     0,     0,   730,
       0,   126,   127,     0,   128,   129,   713,   130,     0,   714,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   146,   147,   148,   149,   150,     0,   715,
       0,     0,     0,     0,     0,   716,   717,   718,     0,   719,
     720,   721,     0,   722,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   723,     0,   724,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   726,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1387,
       0,   727,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   728,   729,     0,     0,
       0,     0,     0,     0,     0,   123,   124,     0,   125,     0,
       0,     0,     0,   730,     0,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,  1388,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,  1051,     0,  1389,     0,     0,     0,     0,  1390,
       0,     0,     0,     0,     0,  1391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1052,   114,     0,     0,   115,
       0,   116,   117,   118,  1392,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   123,   124,     0,   125,     0,
       0,     0,     0,     0,     0,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,  1053,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,   147,   148,
     149,   150,     0,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   123,   124,     0,   125,     0,     0,
       0,     0,     0,     0,   126,   127,     0,   128,   129,     0,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,   146,   147,   148,   149,
     150,   114,     0,     0,   115,   197,   116,   117,   118,   119,
     198,   120,   199,   200,   201,   202,   203,   204,   205,   121,
     122,   206,   207,   208,   209,   210,     0,     0,   211,   212,
     123,   124,   213,   125,     0,     0,   214,   215,   216,   217,
     126,   127,   218,   128,   129,   219,   130,   220,   221,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,   222,
     223,   224,   225,   138,   139,   140,   226,   141,   142,   143,
     227,   144,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   145,   146,   147,   148,   149,   150
};

static const short yycheck[] =
{
       9,     0,     4,    61,    70,   113,    16,    16,    60,    18,
     168,   169,   168,   169,    68,    73,   178,   645,   101,    69,
      69,   422,   835,    79,   527,   835,    92,   459,   459,    98,
     924,   813,    76,    74,   455,   325,   924,   819,   681,    97,
     900,   924,   567,   451,    96,   527,   102,   105,    57,    58,
     650,    60,    61,    62,    63,    64,   100,    98,   668,   426,
      69,    70,    71,  1056,    73,   587,    75,    76,  1448,    78,
      79,    80,   609,   588,   853,   424,  1384,   795,  1130,    88,
      90,    90,    91,    92,    62,    94,    95,    96,    97,    99,
      99,   100,   101,   102,   103,  1106,   105,    75,   846,   373,
     907,   184,   436,    64,   341,  1383,  1148,   758,   765,   406,
    1103,   160,   830,   162,   180,   924,   598,   276,  1206,   924,
    1241,   731,  1009,   181,   435,   924,   933,  1352,   188,  1597,
    1263,   465,  1402,  1358,    95,   942,    10,   945,    21,   183,
       8,     8,   103,     8,    43,   772,   671,  1446,  1488,     6,
     675,   160,   161,   162,     8,   903,   681,   166,     8,   627,
     697,    43,    63,  1135,   173,    82,    19,   911,    26,   178,
    1258,   180,   181,   182,   183,   184,    14,   285,    47,   188,
      23,    10,  1422,   161,     8,    28,    61,   241,    90,    46,
       8,   772,   133,    26,    23,  1002,   777,  1657,    47,    28,
      97,    10,     9,   174,   182,    23,     8,   169,     8,    56,
      28,   373,   173,    11,    23,    49,    61,   271,  1210,    28,
     379,    23,   149,    50,    51,    23,    28,  1219,    44,    44,
      28,    50,    51,   190,    63,   656,  1957,    43,     6,   193,
     185,    21,    55,    63,   196,    44,   117,    45,    55,   681,
     681,    23,  1034,  1035,  1246,   906,    28,   125,   125,    66,
     125,   141,    23,   664,    80,   123,   315,    28,   192,   318,
      46,   125,   230,    59,    60,   125,    44,   385,    44,   141,
     292,    80,   683,  2004,    90,   297,   707,    23,    56,  1597,
     580,  1178,    28,  1106,  1403,   777,  1106,    18,  1431,   633,
     352,   125,    63,  1085,   262,   595,   315,    87,   155,   318,
    1675,  1676,    80,  1591,  1592,   186,   325,   326,    94,   177,
     930,   292,   156,   377,   373,   125,  1666,    23,   141,   145,
     940,   293,    28,   230,   837,   292,   194,  1562,  1563,   155,
     292,  1429,   845,   352,   177,  1805,   191,   356,   293,   173,
     188,   204,   293,   255,   174,  1476,   966,  1466,   165,     8,
       4,    37,   189,   176,   180,   180,   293,    63,   292,  1176,
     189,  1413,    48,   173,   208,   292,   425,   145,   356,    55,
     260,   180,  1681,    59,   246,   284,   435,   155,   437,    23,
     439,   292,  1444,   442,    28,   461,   405,   406,   452,  1026,
     454,    22,   284,    79,   462,    49,  1214,  1215,     8,  1649,
     878,  1193,   180,   296,   180,   469,   425,   292,   292,   463,
      80,   473,   196,   292,   292,   587,   435,   294,   437,   294,
     439,  1483,   289,   442,   959,  1428,  1408,  1409,  1182,    18,
     294,  1022,   196,   292,   294,  1026,   289,   290,   985,   255,
    1830,  1443,   461,   462,   463,  1173,    94,  1265,   287,   288,
     289,   290,   471,   292,   473,   141,    19,   476,  1460,   287,
     288,   289,   290,    68,   476,    63,   125,    23,   287,   288,
     289,   290,    28,  1951,   196,   287,   288,   289,   290,   287,
     288,   289,   290,   471,    23,  1010,   172,   118,  1490,    28,
     176,  1283,  1284,    82,  1286,  1287,  1021,  1785,  1976,  1849,
    1025,   223,   156,    11,   288,   287,   288,   289,   290,   528,
     180,   579,   294,  1888,  1889,   125,   287,   288,   289,   290,
     174,  1801,    90,   294,    23,  2003,   244,   597,   587,    28,
     143,    94,  1767,   699,  1924,   299,  2014,    45,    22,  2017,
     291,   287,   288,   289,   290,    23,  2024,   196,   294,   162,
      28,   199,   200,  1376,   208,    17,  1376,    22,   577,    23,
     579,   580,   862,   168,    28,    30,   288,   586,  1711,    31,
     175,   590,   623,    98,   223,  1965,   595,   641,   597,   265,
     749,   287,   288,   289,   290,  1482,   645,    55,  1208,   577,
      22,  1380,  1723,   122,   658,  1693,  1257,  1987,   586,   958,
     196,    64,   590,  1665,  1271,   912,  1658,    61,  1140,   673,
     931,    14,  1710,    75,    77,   899,   635,   196,   196,  1377,
       3,   700,   681,  1148,   143,  1671,   645,  1265,   875,  1142,
      62,   199,   200,  1951,   118,   126,  1013,   100,   702,   288,
       3,   103,   167,   287,   288,   289,   290,   635,   196,   700,
     294,   114,     6,   118,  1179,   139,  1000,    65,  1976,   122,
     679,   145,   681,   835,  1481,  1667,   685,  1713,     5,     7,
     142,    43,  1674,   141,   139,   194,  1678,  1679,    15,   117,
     145,    46,   850,   747,    23,  2003,   158,   178,   151,    28,
     754,   755,    46,    76,   243,   179,  2014,   293,   757,  2017,
      85,     6,   105,   299,   172,   167,  2024,   139,   176,   288,
    1712,    23,  1714,    76,   179,   293,    28,   883,   156,   187,
     299,   299,    94,   108,     6,   147,  1729,   899,  2016,    94,
      64,   287,   288,   289,   290,     6,   292,   147,   757,    23,
     288,    46,     4,    77,    28,     7,   295,   179,   287,   288,
     289,   290,   174,   118,   303,   294,   196,   808,     6,    22,
     342,     6,   199,   200,    46,   203,   100,    30,   350,    23,
    1188,  1675,  1676,  1794,    28,    46,   239,  1675,  1676,   164,
    1842,   800,  1675,  1676,  1297,  1513,  1848,    49,   287,   288,
     289,   290,   851,  1663,   853,   294,   855,   231,    46,    23,
     263,    46,   968,   871,    28,  1297,   196,   241,     4,   287,
     288,   289,   290,  1815,  1816,    65,   294,   151,    30,   838,
      16,  1441,  1442,   287,   288,   289,   290,   846,    23,   800,
     294,    43,   851,    28,   853,    44,   855,    23,  1458,  1449,
     899,  1259,    28,   862,    65,    23,   905,   911,   155,  1362,
      28,   870,   871,    49,   918,    51,  1675,  1676,   877,   140,
    1675,  1676,   881,   196,   928,   196,  1675,  1676,  1635,  1636,
    1662,    80,   931,    23,   893,   182,   895,   295,    28,   293,
     298,   960,   870,    23,   903,    63,   905,   293,    28,    23,
     223,  1268,   223,   912,    28,  1548,    23,   956,   292,   292,
     959,    28,   122,   297,   297,   893,    23,   895,  1421,   960,
     974,    28,   931,   292,   965,   994,   995,   122,   997,    23,
     174,   232,  1469,  1944,    28,  1946,    79,  1474,  1930,  1931,
    1932,   982,   124,  1954,  1106,  1956,   292,   956,   294,   292,
     959,   294,   124,   994,   995,   292,   997,   294,   287,   288,
     289,   290,   203,    79,   205,   294,   292,   976,   294,   156,
    1337,   114,   231,   232,   976,    82,   180,   292,  1140,   294,
     292,  1794,   294,   126,  1794,   287,   288,   289,   290,  2000,
     292,   192,  1001,    23,  1888,  1889,    19,  1779,    28,   259,
    1888,  1889,  1011,  1635,  1636,  1888,  1889,    23,   130,   131,
    1019,  1020,    28,   287,   288,   289,   290,  1019,  1972,  1973,
     294,    23,   292,  1001,   294,   198,    28,    98,  1510,  1511,
     195,   196,   197,  1011,   266,   293,  1518,  1519,   199,  1521,
    1522,   162,    10,   287,   288,   289,   290,  1529,  1530,  1531,
    1532,  1576,  1534,  1535,  1536,  1537,  1538,  1539,   223,   224,
      59,   132,  1041,   134,  1043,  1486,  1045,  1046,    61,  1048,
    1049,  1050,    79,   287,   288,   289,   290,    78,   292,  1888,
    1889,   190,  1091,  1888,  1889,   156,   149,   141,  1244,  1888,
    1889,  1140,   292,   411,   294,   413,   292,  1387,   294,   292,
    1390,   294,   287,   288,   289,   290,  1416,  1717,  1418,   294,
     181,   287,   288,   289,   290,   172,  1548,  1548,   294,   287,
     288,   289,   290,    82,  1906,  1134,  1135,   293,  1182,    63,
    1091,  1944,  1914,  1946,  1944,   292,  1946,   294,   292,  1148,
     294,  1954,    64,  1956,  1954,    61,  1956,   287,   288,   289,
     290,    23,    61,   122,   294,    77,    28,   287,   288,   289,
     290,    64,   122,    85,   294,   292,   292,   294,   294,     0,
     287,   288,   289,   290,    77,   102,   244,   294,   100,    10,
     287,   288,   289,   290,   174,  1254,    23,  2000,   172,   282,
    2000,    28,   114,   287,   288,   289,   290,   100,  1267,    61,
     294,    23,   283,   292,  1273,   294,    28,   292,   292,   294,
     294,   114,  1261,  1254,  1376,    23,  1265,    54,   280,   292,
      28,   294,  1384,   292,  1233,   294,  1267,   123,   156,   151,
      61,    65,  1273,    64,    83,   121,  1238,   243,   155,   254,
      71,  1250,    73,  1401,  1402,  1403,    77,   255,   151,   254,
      43,    82,  1261,    52,    85,   207,  1265,   287,   288,   289,
     290,    23,   192,   292,   294,   284,    28,   112,    29,   100,
      10,   287,   288,   289,   290,   293,   135,    97,   294,  1435,
     293,  1437,  1683,   114,   293,   287,   288,   289,   290,    23,
     293,   122,   294,   199,    28,   126,   293,    23,   293,  1802,
     199,   293,    28,   292,   135,    23,   293,   293,  1391,   796,
      28,   798,   293,    23,   293,   802,   803,   804,    28,   199,
     151,   293,   293,  1333,  1333,   148,   293,   293,  1484,   293,
    1388,  1380,  1960,  1383,  1383,  1384,  1392,    17,   293,   293,
     293,   263,   293,    23,    54,  1389,   293,   178,    28,   293,
     296,    31,   293,   298,   293,   298,    10,    23,   284,   190,
     263,   192,    28,   141,    63,   166,   281,   292,  1377,   292,
     284,  1380,    23,   293,  1383,  1384,    10,    28,  1387,  1388,
    1389,  1390,  1391,  1392,    90,   298,    83,    67,    23,    90,
     298,    17,   292,    28,    90,    75,   111,    23,    22,  1408,
    1409,    23,    28,    83,  1413,    31,    28,    10,   239,   174,
      61,   136,   136,   196,   174,   287,   288,   289,   290,    21,
      90,   101,   294,   103,   138,   293,   185,   185,   259,    91,
     293,   296,   263,   293,    61,  1597,   116,   293,   118,   119,
     120,    67,    10,  1497,   122,   149,   292,    82,  1502,    75,
     287,   288,   289,   290,   128,   293,   121,    83,   293,   126,
     291,   292,  1464,   294,    61,   287,   288,   289,   290,   189,
     294,    21,   294,   299,   174,   101,    42,   103,    60,   287,
     288,   289,   290,   174,   293,   202,   294,   167,   292,    10,
     116,    83,   118,   119,   120,   190,   186,   240,  1660,   293,
      17,   292,    27,    19,   293,    86,    23,    86,    43,   125,
     125,    28,   125,   156,    31,   154,  1582,   293,   156,   314,
      98,   293,   293,   108,   293,   287,   288,   289,   290,   292,
     294,    63,   294,    23,  1544,  1544,   292,   294,   184,    16,
     124,   167,   107,   338,   298,    63,   292,    71,  1597,    63,
      67,   293,   347,   287,   288,   289,   290,    12,    75,    10,
     294,   287,   288,   289,   290,   160,    83,   239,   294,   287,
     288,   289,   290,  1582,   295,   294,   294,   287,   288,   289,
     290,  1590,  1591,  1592,   101,   294,   103,   294,  1597,   210,
     211,   212,   213,   214,   215,   216,   217,    10,   192,   116,
     294,   118,   119,   120,   284,   285,   286,   287,   288,   289,
     290,   133,    52,    52,     8,   410,   296,   294,   298,   414,
     293,   287,   288,   289,   290,   294,  1635,  1636,   294,   293,
      63,    51,  1794,   293,   293,   293,   287,   288,   289,   290,
     293,     6,  1804,  1801,   293,   293,   293,   173,  1657,  1658,
     167,  1660,   287,   288,   289,   290,   293,   293,   284,   285,
     286,   287,   288,   289,   290,   287,   288,   289,   290,   192,
     296,    37,   298,   293,   293,  1684,   293,   127,   292,    44,
      85,   293,    48,   293,   292,   294,    85,   294,    19,    55,
      73,    56,   130,   145,   125,   125,   125,   125,   125,   125,
     196,   294,    68,  1755,   294,    70,  1684,    22,   294,    63,
     293,    51,   173,    36,   294,    80,   292,   294,   513,   294,
     294,   291,    36,    82,   293,    51,  1834,   291,   291,    82,
     525,   526,   527,   295,    70,   278,    10,   174,    85,    23,
      79,    44,  1752,  1752,   180,    80,  1755,    80,   180,    44,
     294,    94,   294,   124,    48,   111,   111,   294,   294,   125,
     294,    55,    61,   294,   294,    59,    60,   284,   285,   286,
     287,   288,   289,   290,    68,   141,  1785,   189,   293,   296,
     145,   298,  1944,  1835,  1946,    79,   294,   294,   294,  1951,
     155,   157,  1954,  1792,  1956,  1804,  1805,   294,    86,   294,
     294,  1855,    84,   598,   293,  1859,   172,   602,   294,   294,
     176,   294,   294,   179,  1976,   180,  1825,  1925,   294,    17,
    1829,   187,   294,   124,   292,    23,  1835,   294,   204,    79,
      28,   293,  1898,    31,    82,   117,   246,   202,  2000,    82,
     206,  2003,   122,   173,   293,   293,   293,   141,   294,   292,
     122,  1829,  2014,   122,    47,  2017,    44,    36,   180,   294,
     294,   143,  2024,   157,   146,    50,    47,   201,     0,    67,
     294,     0,  1881,   165,   165,  1399,   166,    75,   172,   323,
     162,   593,   176,   871,   326,    83,   862,   595,   761,  1898,
    1899,  1014,  1271,   187,  1268,  1260,  1701,   989,  1900,   265,
    1019,  1683,  1951,   101,    80,   103,   188,   456,  1191,   707,
     381,  1960,   206,  1261,   905,   941,  1265,   437,   116,  1700,
     118,   119,   120,  1681,   437,  1963,  1966,  1976,  1967,  1789,
    1989,   931,  1952,  1131,     0,    17,  1432,   687,   442,   851,
     427,    23,  1951,   992,  1943,   420,    28,  1472,  1957,    31,
     984,  1960,   744,    13,  2003,   167,   363,   893,   171,   611,
     616,   243,    90,  1972,  1973,  2014,  1417,  1976,  2017,   167,
    1148,   265,   879,  1919,  1752,  2024,   794,  1544,   915,     0,
       0,     0,   777,    38,   266,    67,   627,   869,  1143,  1194,
    1312,   976,  1360,    75,  2003,  2004,   505,   279,   685,  1250,
    1825,    83,   797,    94,    -1,  2014,   801,  2016,  2017,  1590,
      -1,    66,    -1,    -1,    -1,  2024,    -1,   812,    -1,   101,
      -1,   103,   817,   818,    -1,    -1,   821,   822,   823,   824,
     825,   826,    -1,    -1,   116,    -1,   118,   119,   120,    94,
      -1,    -1,   837,    -1,   839,   840,   841,   842,   843,   844,
     845,  1510,  1511,    -1,    -1,    -1,    -1,    -1,    -1,  1518,
    1519,    -1,  1521,  1522,    -1,    -1,    -1,    -1,    -1,    -1,
    1529,  1530,  1531,  1532,    -1,  1534,  1535,  1536,  1537,  1538,
    1539,    -1,    -1,    -1,   879,   167,   284,   285,   286,   287,
     288,   289,   290,    -1,    -1,    -1,   294,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   902,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     915,     6,   917,    -1,     9,    -1,   181,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,   200,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,   219,   220,   221,   222,   223,   224,
     225,   226,    -1,    -1,    -1,    -1,    -1,    -1,   233,   234,
      -1,    66,    -1,    -1,    69,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,   285,   286,   287,   288,   289,   290,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1036,  1037,  1038,    -1,    -1,  1041,  1042,  1043,    -1,
    1045,  1046,  1047,  1048,  1049,  1050,    -1,    -1,    -1,    -1,
      -1,    -1,  1057,  1058,    -1,  1060,    -1,  1062,    -1,    -1,
    1065,  1066,  1067,  1068,  1069,  1070,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,  1083,    -1,
      -1,  1086,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1101,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,  1142,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,   278,   279,   280,   281,   282,   283,    -1,
      -1,    -1,   287,   288,    -1,    -1,    -1,    -1,   293,    -1,
     295,    -1,    -1,    -1,   299,    -1,    -1,     6,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1297,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,  1310,  1311,   106,  1313,  1314,
      -1,   110,    -1,    -1,    -1,    -1,  1321,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1341,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1349,    -1,  1351,  1352,    -1,    -1,
      -1,    -1,    -1,  1358,    -1,  1360,  1361,  1362,  1363,    -1,
      -1,    -1,    -1,    -1,   163,  1370,   165,  1372,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,  1421,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
    1445,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    21,   274,   275,    -1,   277,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,   287,   288,
      37,    -1,    -1,    -1,   293,    -1,   295,    -1,     7,    -1,
     299,    48,    -1,    -1,  1509,    -1,    -1,    -1,    55,    -1,
    1515,    -1,    59,    60,    -1,  1520,    25,    -1,    -1,    -1,
      -1,    68,  1527,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    79,    42,    -1,    44,    -1,    -1,    -1,    -1,
      49,    -1,    18,    -1,    -1,    21,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1562,  1563,    -1,
      -1,    70,    38,    -1,    -1,    41,  1571,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1587,    -1,    -1,    -1,   133,    -1,     6,    -1,
      -1,     9,    -1,    -1,   141,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    32,    91,    92,    93,    -1,    95,
      38,    39,    -1,    -1,    -1,   172,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,   143,    -1,    -1,   113,    -1,   115,
     187,   150,    -1,    -1,   153,    -1,   155,   156,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,  1661,    -1,    -1,   206,
     207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,
     229,   268,    -1,    -1,    -1,    -1,    -1,  1732,    -1,    -1,
      -1,    -1,    -1,  1738,  1739,   244,   245,    -1,    -1,    -1,
      -1,    -1,   218,   219,    -1,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,  1762,    -1,   235,
     269,    -1,  1767,   181,    -1,  1770,    27,    -1,    -1,    -1,
      -1,    -1,  1777,  1778,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,  1802,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,  1854,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,   287,
     288,     6,    -1,    -1,     9,   293,    -1,   295,    13,    -1,
      -1,   299,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1919,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,  1953,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,   293,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     295,    13,    -1,    -1,   299,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
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
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
       6,    -1,    -1,     9,   293,    -1,   295,    13,    18,    -1,
     299,    21,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    32,    -1,    38,    -1,
      -1,    41,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,   113,   110,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,   218,   219,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,   235,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,   287,   288,     6,    -1,    -1,     9,   293,    -1,   295,
      13,    -1,    -1,   299,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    32,
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
     163,    -1,   165,    -1,   293,    -1,    -1,    -1,   171,    -1,
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
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,
      -1,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    39,    -1,
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
      -1,    -1,   289,    -1,   165,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   181,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,   118,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
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
     280,   281,   282,   283,    13,    -1,    -1,   287,   288,    -1,
      -1,    20,    -1,   293,    -1,   295,    -1,    -1,    -1,   299,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,   118,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,   293,    -1,
     295,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
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
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,   295,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,   267,
     268,    -1,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,    13,    -1,    -1,   287,
     288,    -1,    -1,    20,    -1,   293,    -1,   295,    -1,    -1,
      -1,   299,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     293,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
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
     287,   288,    -1,    -1,    20,    -1,   293,    -1,   295,    -1,
      -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
     106,    -1,    -1,    -1,   110,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     291,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    13,    -1,
      -1,   287,   288,   289,    -1,    20,    -1,   293,    -1,   295,
      -1,    -1,    -1,   299,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,   196,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,   288,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
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
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
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
     289,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
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
     293,   294,   295,    -1,    -1,    -1,   299,    -1,    -1,    -1,
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
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,
      -1,   293,    -1,   295,    -1,    -1,    -1,   299,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    -1,    -1,    69,    -1,
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
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,   279,
     280,   281,   282,   283,    13,    -1,    -1,   287,   288,    -1,
      -1,    20,    -1,   293,    -1,   295,    -1,    -1,    -1,   299,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
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
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      56,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,   145,
      -1,    -1,   260,   261,   262,    16,   264,   265,   266,   155,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,   287,
     288,    -1,    -1,    44,   180,   293,    -1,   295,    -1,    -1,
      51,   299,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    74,    -1,    -1,    -1,    -1,    -1,    80,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    16,   268,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,   170,
      55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    74,
      -1,    -1,    -1,    -1,    -1,    80,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,   137,   254,   255,   256,   257,   258,    -1,   260,
     261,   262,    -1,   264,   265,   266,    16,   268,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    74,    -1,    -1,    -1,    -1,    -1,
      80,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    21,    -1,   227,   228,    -1,
     230,    27,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,
     240,   241,    38,   243,    -1,    41,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,   256,   257,   258,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,   282,   283,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,    -1,    -1,    18,    -1,    -1,    21,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,   237,   238,    -1,   240,   241,    38,   243,    -1,    41,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,   141,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    85,    -1,   176,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,   164,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,   198,    -1,    -1,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,    -1,   225,   226,
     227,   228,   229,   230,    -1,    -1,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283
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
{ lex.beginning = lex.last_token; lex.stop_trigger = 0; }
    break;
case 379:
{
				yyval = (dsql_nod*) MAKE_string(lex.beginning, 
					(lex.stop_trigger ? lex.stop_trigger : lex_position()) - lex.beginning); 
				lex.stop_trigger = 0; 
			}
    break;
case 380:
{ lex.stop_trigger = lex.last_token; }
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
