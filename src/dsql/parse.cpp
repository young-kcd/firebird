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



#define	YYFINAL		1914
#define	YYFLAG		-32768
#define	YYNTBASE	283

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 520 ? yytranslate[x] : 719)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     276,   277,   272,   270,   275,   271,   281,   273,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   278,   274,
     268,   267,   269,   282,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   279,     2,   280,     2,     2,     2,     2,     2,     2,
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
     266
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    43,    46,    55,    64,    70,    72,    73,    75,
      78,    80,    82,    86,    88,    90,    92,    94,    97,   100,
     104,   105,   109,   110,   112,   121,   130,   136,   140,   141,
     145,   146,   148,   150,   152,   156,   160,   164,   167,   170,
     173,   176,   178,   182,   184,   187,   190,   192,   196,   198,
     200,   204,   206,   209,   212,   215,   219,   228,   230,   232,
     238,   239,   241,   245,   247,   251,   255,   256,   259,   262,
     264,   266,   270,   274,   277,   278,   281,   284,   286,   290,
     300,   302,   304,   307,   311,   319,   322,   325,   328,   331,
     334,   337,   340,   343,   346,   349,   352,   355,   358,   361,
     366,   369,   372,   374,   375,   377,   379,   386,   393,   395,
     397,   398,   399,   401,   402,   407,   408,   410,   412,   415,
     424,   426,   427,   431,   432,   433,   435,   437,   440,   442,
     444,   446,   449,   456,   458,   460,   464,   465,   467,   469,
     470,   472,   474,   477,   481,   486,   489,   492,   496,   497,
     499,   501,   504,   506,   511,   515,   519,   521,   522,   524,
     526,   529,   533,   538,   539,   541,   544,   545,   547,   549,
     555,   561,   565,   568,   569,   571,   575,   577,   579,   586,
     590,   593,   600,   603,   605,   608,   611,   614,   615,   617,
     619,   622,   623,   626,   627,   629,   631,   633,   635,   637,
     639,   640,   642,   644,   647,   650,   653,   659,   661,   664,
     668,   671,   674,   675,   677,   679,   681,   683,   687,   692,
     701,   706,   707,   714,   716,   718,   721,   724,   725,   729,
     733,   735,   738,   741,   744,   753,   762,   771,   780,   784,
     785,   790,   791,   793,   797,   802,   804,   808,   811,   815,
     819,   821,   823,   824,   826,   829,   834,   836,   838,   842,
     844,   845,   848,   850,   857,   859,   861,   865,   867,   870,
     871,   873,   876,   879,   882,   884,   886,   888,   890,   892,
     894,   897,   899,   901,   904,   906,   908,   910,   913,   916,
     918,   921,   924,   928,   933,   936,   943,   948,   957,   967,
     974,   983,   990,   995,   996,  1001,  1004,  1006,  1010,  1011,
    1014,  1019,  1020,  1022,  1024,  1028,  1032,  1040,  1043,  1044,
    1047,  1050,  1054,  1058,  1059,  1061,  1064,  1069,  1071,  1075,
    1078,  1081,  1084,  1086,  1088,  1090,  1092,  1095,  1098,  1104,
    1105,  1110,  1118,  1122,  1123,  1125,  1129,  1133,  1141,  1149,
    1150,  1151,  1152,  1153,  1157,  1158,  1168,  1178,  1180,  1182,
    1183,  1186,  1188,  1190,  1192,  1194,  1196,  1200,  1204,  1208,
    1212,  1216,  1220,  1226,  1232,  1238,  1244,  1250,  1256,  1259,
    1260,  1265,  1268,  1272,  1276,  1279,  1282,  1286,  1290,  1293,
    1296,  1300,  1307,  1309,  1312,  1317,  1321,  1324,  1327,  1330,
    1333,  1337,  1339,  1343,  1347,  1351,  1354,  1357,  1362,  1367,
    1373,  1375,  1377,  1379,  1381,  1383,  1385,  1387,  1389,  1391,
    1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,
    1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,  1431,
    1433,  1435,  1437,  1439,  1442,  1445,  1448,  1450,  1452,  1454,
    1455,  1458,  1461,  1466,  1471,  1477,  1478,  1480,  1483,  1486,
    1491,  1495,  1498,  1501,  1509,  1511,  1512,  1514,  1515,  1518,
    1521,  1524,  1527,  1530,  1533,  1536,  1539,  1542,  1546,  1549,
    1552,  1555,  1558,  1560,  1562,  1564,  1566,  1571,  1577,  1579,
    1583,  1585,  1589,  1591,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1612,  1614,  1616,  1621,  1626,  1633,  1639,
    1643,  1644,  1647,  1650,  1651,  1655,  1656,  1661,  1663,  1669,
    1674,  1676,  1681,  1683,  1686,  1689,  1691,  1693,  1695,  1698,
    1701,  1704,  1707,  1708,  1712,  1718,  1720,  1722,  1725,  1729,
    1731,  1734,  1738,  1739,  1741,  1743,  1745,  1751,  1757,  1764,
    1766,  1768,  1770,  1773,  1778,  1780,  1781,  1787,  1789,  1790,
    1794,  1797,  1799,  1800,  1803,  1804,  1806,  1807,  1811,  1813,
    1814,  1816,  1819,  1821,  1823,  1825,  1827,  1830,  1833,  1835,
    1838,  1842,  1844,  1846,  1850,  1854,  1856,  1859,  1863,  1865,
    1868,  1869,  1872,  1874,  1876,  1877,  1879,  1881,  1883,  1887,
    1890,  1894,  1895,  1897,  1901,  1906,  1910,  1914,  1915,  1918,
    1919,  1922,  1923,  1927,  1931,  1935,  1937,  1942,  1947,  1949,
    1959,  1960,  1961,  1962,  1963,  1967,  1970,  1972,  1973,  1977,
    1983,  1987,  1990,  1996,  1999,  2001,  2003,  2005,  2007,  2009,
    2013,  2015,  2019,  2021,  2022,  2025,  2027,  2031,  2033,  2035,
    2037,  2039,  2043,  2050,  2052,  2053,  2057,  2058,  2060,  2064,
    2066,  2068,  2070,  2075,  2081,  2087,  2089,  2091,  2094,  2099,
    2104,  2107,  2111,  2112,  2114,  2117,  2119,  2121,  2124,  2127,
    2130,  2131,  2133,  2134,  2138,  2139,  2141,  2145,  2147,  2150,
    2151,  2154,  2155,  2158,  2159,  2164,  2166,  2169,  2171,  2173,
    2174,  2176,  2180,  2183,  2185,  2187,  2190,  2192,  2197,  2201,
    2203,  2207,  2212,  2213,  2217,  2218,  2220,  2224,  2228,  2230,
    2232,  2233,  2235,  2237,  2242,  2243,  2246,  2251,  2252,  2261,
    2267,  2269,  2271,  2279,  2284,  2289,  2291,  2293,  2302,  2308,
    2310,  2314,  2318,  2320,  2328,  2336,  2342,  2346,  2347,  2349,
    2351,  2353,  2356,  2357,  2359,  2361,  2363,  2364,  2368,  2370,
    2374,  2376,  2377,  2381,  2383,  2387,  2389,  2393,  2397,  2399,
    2401,  2405,  2407,  2410,  2412,  2416,  2420,  2422,  2425,  2429,
    2433,  2437,  2441,  2443,  2447,  2450,  2452,  2454,  2456,  2458,
    2460,  2462,  2464,  2466,  2468,  2470,  2472,  2476,  2480,  2484,
    2488,  2492,  2496,  2500,  2504,  2511,  2518,  2525,  2532,  2539,
    2546,  2553,  2560,  2567,  2574,  2581,  2588,  2595,  2602,  2609,
    2616,  2618,  2620,  2626,  2633,  2639,  2646,  2650,  2655,  2661,
    2668,  2672,  2677,  2681,  2686,  2690,  2695,  2700,  2706,  2711,
    2716,  2720,  2725,  2727,  2729,  2731,  2733,  2735,  2737,  2739,
    2743,  2747,  2749,  2751,  2753,  2755,  2757,  2759,  2761,  2763,
    2765,  2767,  2770,  2773,  2777,  2781,  2785,  2789,  2793,  2797,
    2801,  2805,  2807,  2809,  2811,  2813,  2817,  2819,  2821,  2823,
    2825,  2827,  2829,  2834,  2836,  2840,  2842,  2845,  2847,  2849,
    2851,  2853,  2855,  2857,  2859,  2862,  2865,  2868,  2870,  2872,
    2874,  2876,  2878,  2880,  2882,  2884,  2886,  2888,  2891,  2893,
    2896,  2898,  2900,  2902,  2904,  2906,  2909,  2911,  2913,  2915,
    2917,  2922,  2928,  2934,  2940,  2946,  2952,  2958,  2964,  2970,
    2976,  2982,  2984,  2991,  2993,  2998,  3006,  3009,  3010,  3015,
    3019,  3026,  3028,  3030,  3037,  3046,  3053,  3055,  3057,  3062,
    3069,  3074,  3080,  3084,  3090,  3095,  3101,  3103,  3105,  3107,
    3112,  3119,  3121,  3123,  3125,  3127,  3129,  3131,  3133,  3135,
    3137,  3138,  3140,  3141,  3143,  3145,  3147,  3149,  3151,  3153,
    3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,  3173,
    3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,  3193,
    3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,  3213,
    3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229,  3231,  3233,
    3235,  3237,  3239,  3241,  3243,  3245,  3247,  3249,  3251
};
static const short yyrhs[] =
{
     284,     0,   284,   274,     0,   464,     0,   616,     0,   518,
       0,   319,     0,   306,     0,   606,     0,   486,     0,   285,
       0,   605,     0,   444,     0,   445,     0,   321,     0,   323,
       0,   294,     0,   519,     0,   512,     0,   541,     0,   510,
       0,   610,     0,    40,   662,     0,    70,   287,   122,   286,
     581,   174,   297,   291,     0,    70,   289,   122,   141,   293,
     174,   297,   291,     0,    70,   302,   174,   304,   292,     0,
     172,     0,     0,     6,     0,     6,   140,     0,   288,     0,
     290,     0,   288,   275,   290,     0,    56,     0,   155,     0,
      80,     0,    44,     0,   180,   621,     0,   145,   621,     0,
     192,    70,   124,     0,     0,   192,   202,   124,     0,     0,
     708,     0,   150,   295,   287,   122,   286,   581,    63,   297,
       0,   150,   295,   289,   122,   141,   293,    63,   297,     0,
     150,   296,   302,    63,   304,     0,    70,   124,    61,     0,
       0,   202,   124,    61,     0,     0,   298,     0,   300,     0,
     299,     0,   298,   275,   299,     0,   298,   275,   301,     0,
     300,   275,   299,     0,   141,   708,     0,   176,   712,     0,
     187,   715,     0,   206,   709,     0,   301,     0,   300,   275,
     301,     0,   713,     0,   181,   713,     0,    71,   713,     0,
     303,     0,   302,   275,   303,     0,   709,     0,   305,     0,
     304,   275,   305,     0,   713,     0,   181,   713,     0,    42,
     307,     0,    60,   317,     0,    59,    65,   308,     0,   693,
     310,   149,   314,    52,   661,   111,   661,     0,   493,     0,
      18,     0,    34,   276,   665,   277,   499,     0,     0,   311,
       0,   276,   311,   277,     0,   312,     0,   311,   275,   312,
       0,   373,   309,   313,     0,     0,    19,   230,     0,    19,
     262,     0,    94,     0,   315,     0,   276,   315,   277,     0,
     373,   309,   316,     0,   133,   665,     0,     0,    19,    97,
       0,    19,   230,     0,   204,     0,    19,   230,   204,     0,
     702,    78,   318,   128,   318,    52,   661,   111,   661,     0,
     694,     0,   662,     0,    33,   320,     0,    55,   701,   661,
       0,   325,   601,    79,   705,   122,   581,   326,     0,   141,
     392,     0,   172,   360,     0,   176,   456,     0,   187,   449,
       0,    68,   342,     0,   265,   342,     0,    37,   344,     0,
      48,   333,     0,   157,   327,     0,   206,   343,     0,   229,
     322,     0,   141,   393,     0,   172,   361,     0,   187,   450,
       0,    33,   125,     7,   324,     0,   141,   394,     0,   176,
     457,     0,   179,     0,     0,   623,     0,   622,     0,   367,
     276,   453,   650,   454,   277,     0,   665,   328,   329,   661,
     330,   331,     0,   105,     0,    14,     0,     0,     0,    29,
       0,     0,    98,   345,   668,   359,     0,     0,   332,     0,
     353,     0,   332,   353,     0,   370,   334,   488,   453,   335,
     454,   336,   369,     0,    10,     0,     0,    43,   453,   375,
       0,     0,     0,   337,     0,   338,     0,   337,   338,     0,
     339,     0,   340,     0,   341,     0,   118,    94,     0,   453,
      22,   276,   630,   277,   454,     0,   704,     0,   709,     0,
     346,   347,   350,     0,     0,   267,     0,   661,     0,     0,
     348,     0,   349,     0,   348,   349,     0,   132,   345,   665,
       0,    98,   345,   668,   359,     0,   181,   661,     0,   134,
     661,     0,   156,   112,   661,     0,     0,   351,     0,   352,
       0,   351,   352,     0,   353,     0,    43,    21,   156,   695,
       0,   255,    90,   661,     0,   354,   661,   355,     0,    90,
       0,     0,   356,     0,   357,     0,   356,   357,     0,   167,
     358,   668,     0,    98,   345,   668,   359,     0,     0,    12,
       0,    12,   130,     0,     0,   130,     0,   131,     0,   581,
     362,   276,   363,   277,     0,   581,   362,   276,   363,   277,
       0,    59,    90,   661,     0,    59,   661,     0,     0,   364,
       0,   363,   275,   364,     0,   365,     0,   380,     0,   370,
     368,   335,   454,   376,   369,     0,   370,   489,   366,     0,
     370,   366,     0,   367,   276,   453,   650,   454,   277,     0,
      27,    19,     0,    27,     0,   488,   453,     0,   628,   451,
       0,    23,   696,     0,     0,   628,     0,   628,     0,   373,
     488,     0,     0,    43,   375,     0,     0,   654,     0,   658,
       0,   659,     0,   660,     0,   692,     0,   651,     0,     0,
     377,     0,   378,     0,   377,   378,     0,   381,   379,     0,
     118,    94,     0,   145,   581,   621,   388,   386,     0,   387,
       0,   179,   386,     0,   139,    86,   386,     0,   381,   382,
       0,    30,   698,     0,     0,   383,     0,   384,     0,   385,
       0,   387,     0,   179,   622,   386,     0,   139,    86,   622,
     386,     0,    62,    86,   622,   145,   581,   621,   388,   386,
       0,   239,   601,    79,   705,     0,     0,   453,    22,   276,
     630,   277,   454,     0,   389,     0,   390,     0,   390,   389,
       0,   389,   390,     0,     0,   122,   180,   391,     0,   122,
      44,   391,     0,   203,     0,   156,    43,     0,   156,    94,
       0,   117,   201,     0,   708,   396,   397,    10,   451,   403,
     412,   454,     0,   708,   396,   397,    10,   451,   403,   412,
     454,     0,   708,   396,   397,    10,   451,   403,   412,   454,
       0,   708,   396,   397,    10,   451,   403,   412,   454,     0,
     276,   398,   277,     0,     0,   149,   276,   400,   277,     0,
       0,   399,     0,   398,   275,   399,     0,   371,   489,   402,
     454,     0,   401,     0,   400,   275,   401,     0,   371,   489,
       0,    43,   451,   375,     0,   267,   451,   375,     0,   451,
       0,   404,     0,     0,   405,     0,   404,   405,     0,    42,
     408,   406,   274,     0,   407,     0,   410,     0,   370,   489,
     409,     0,   184,     0,     0,   267,   375,     0,   374,     0,
     699,    36,    61,   276,   541,   277,     0,   415,     0,   412,
       0,    16,   413,    51,     0,   414,     0,   414,   435,     0,
       0,   411,     0,   414,   411,     0,   614,   274,     0,   606,
     274,     0,   416,     0,   417,     0,   418,     0,   419,     0,
     420,     0,   423,     0,   605,   274,     0,   424,     0,   426,
       0,   610,   274,     0,   431,     0,   421,     0,   422,     0,
     170,   274,     0,    58,   274,     0,   433,     0,   439,   274,
       0,   615,   274,     0,    55,   701,   274,     0,    55,   701,
     650,   274,     0,    55,   274,     0,    56,   141,   708,   428,
     429,   274,     0,    56,   246,   650,   274,     0,   432,    61,
     541,    82,   430,   434,    47,   411,     0,   432,    61,    56,
     246,   650,    82,   430,    47,   411,     0,    56,   246,   650,
      82,   430,   274,     0,    74,   276,   630,   277,   173,   411,
      50,   411,     0,    74,   276,   630,   277,   173,   411,     0,
     137,   650,   425,   274,     0,     0,   541,    82,   430,   274,
       0,   278,   714,     0,   653,     0,   276,   653,   277,     0,
       0,   148,   430,     0,   148,   276,   430,   277,     0,     0,
     427,     0,   627,     0,   430,   275,   627,     0,   430,   275,
     427,     0,   432,   191,   276,   630,   277,    47,   411,     0,
     707,   278,     0,     0,   227,   274,     0,   247,   274,     0,
     247,   707,   274,     0,    10,    36,   699,     0,     0,   436,
       0,   435,   436,     0,   189,   437,    47,   411,     0,   438,
       0,   437,   275,   438,     0,   165,   662,     0,    66,   703,
       0,    55,   701,     0,     9,     0,   440,     0,   442,     0,
     441,     0,   256,   699,     0,   257,   699,     0,   258,   443,
     699,    82,   430,     0,     0,    56,   141,   708,   428,     0,
      56,   260,   446,   397,    10,   403,   412,     0,   276,   447,
     277,     0,     0,   448,     0,   447,   275,   448,     0,   401,
     267,   657,     0,   715,   621,    10,   451,   545,   455,   452,
       0,   715,   621,    10,   451,   545,   455,   452,     0,     0,
       0,     0,     0,   192,    22,   124,     0,     0,   712,    61,
     581,   458,   459,   462,   453,   463,   454,     0,   712,    61,
     581,   458,   459,   462,   453,   463,   454,     0,     3,     0,
      76,     0,     0,   460,   461,     0,    15,     0,     5,     0,
      80,     0,   180,     0,    44,     0,    80,   125,   180,     0,
      80,   125,    44,     0,   180,   125,    80,     0,   180,   125,
      44,     0,    44,   125,    80,     0,    44,   125,   180,     0,
      80,   125,   180,   125,    44,     0,    80,   125,    44,   125,
     180,     0,   180,   125,    80,   125,    44,     0,   180,   125,
      44,   125,    80,     0,    44,   125,    80,   125,   180,     0,
      44,   125,   180,   125,    80,     0,   136,   663,     0,     0,
      10,   453,   403,   412,     0,     7,   465,     0,    55,   701,
     661,     0,   172,   581,   470,     0,   176,   483,     0,   141,
     395,     0,    37,   480,   481,     0,    48,   472,   468,     0,
      79,   478,     0,   265,   479,     0,    43,   453,   375,     0,
      22,   453,   276,   630,   277,   454,     0,   469,     0,   468,
     469,     0,   156,   453,   466,   454,     0,     4,    30,   467,
       0,     4,   341,     0,    49,    43,     0,    49,    30,     0,
     174,   628,     0,   208,   373,   489,     0,   471,     0,   470,
     275,   471,     0,    49,   628,   477,     0,    49,    30,   698,
       0,     4,   365,     0,     4,   380,     0,   474,   628,   136,
     665,     0,   474,   472,   174,   628,     0,   474,   476,   208,
     475,   454,     0,   473,     0,   717,     0,   202,     0,   207,
       0,   209,     0,   210,     0,   211,     0,   212,     0,   213,
       0,   214,     0,   215,     0,   218,     0,   219,     0,   220,
       0,   221,     0,   222,     0,   225,     0,   226,     0,   229,
       0,   233,     0,   234,     0,   235,     0,   236,     0,   245,
       0,   242,     0,   244,     0,   256,     0,   257,     0,   258,
       0,   259,     0,   239,     0,   263,     0,     7,     0,     7,
     207,     0,   489,   453,     0,   628,   451,     0,   628,     0,
     205,     0,   203,     0,     0,   705,     3,     0,   705,    76,
       0,   704,   266,   192,   667,     0,   704,   266,   192,   223,
       0,   704,   266,   192,   271,   223,     0,     0,   482,     0,
     481,   482,     0,     4,   332,     0,     4,   255,    90,   661,
       0,    49,   255,    90,     0,    16,   254,     0,    51,   254,
       0,   712,   458,   484,   462,   453,   485,   454,     0,   459,
       0,     0,   463,     0,     0,    49,   487,     0,    55,   701,
       0,    79,   705,     0,   141,   708,     0,   172,   711,     0,
     176,   712,     0,   187,   715,     0,    60,   702,     0,    48,
     700,     0,    59,    65,   693,     0,   157,   665,     0,   206,
     709,     0,    68,   704,     0,   265,   704,     0,   489,     0,
     490,     0,   493,     0,   496,     0,   494,   279,   491,   280,
       0,   501,   279,   491,   280,   499,     0,   492,     0,   491,
     275,   492,     0,   667,     0,   667,   278,   667,     0,   494,
       0,   501,   499,     0,   500,     0,   505,     0,   508,     0,
     235,     0,   495,     0,   161,     0,    38,     0,   218,     0,
     219,     0,    81,     0,    92,     0,    18,   498,   497,   499,
       0,    18,   276,   666,   277,     0,    18,   276,   666,   275,
     662,   277,     0,    18,   276,   275,   662,   277,     0,   154,
     160,   666,     0,     0,   169,   662,     0,   169,   694,     0,
       0,    21,   156,   695,     0,     0,   504,   276,   665,   277,
       0,   504,     0,   504,   185,   276,   665,   277,     0,   503,
     276,   665,   277,     0,   503,     0,   502,   276,   665,   277,
       0,   183,     0,    21,   185,     0,    87,   185,     0,    21,
       0,    87,     0,   115,     0,   113,    21,     0,   113,    87,
       0,    95,   506,     0,   507,   506,     0,     0,   276,   667,
     277,     0,   276,   667,   275,   667,   277,     0,    41,     0,
      88,     0,    91,   509,     0,    93,    91,   509,     0,   144,
       0,    89,   138,     0,   276,   663,   277,     0,     0,   523,
       0,   511,     0,   540,     0,   156,    68,   704,   174,   667,
       0,   156,    68,   704,   174,   223,     0,   156,    68,   704,
     174,   271,   223,     0,   513,     0,   514,     0,   516,     0,
     244,   716,     0,   245,   244,   716,   515,     0,   123,     0,
       0,   153,   520,   174,   517,   716,     0,   244,     0,     0,
      25,   520,   521,     0,   153,   520,     0,   193,     0,     0,
     147,   522,     0,     0,   162,     0,     0,   156,   175,   524,
       0,   525,     0,     0,   526,     0,   525,   526,     0,   527,
       0,   528,     0,   529,     0,   533,     0,   143,   123,     0,
     143,   194,     0,   188,     0,   117,   188,     0,    84,   102,
     530,     0,   530,     0,   531,     0,   143,   177,   532,     0,
     143,    26,   532,     0,   162,     0,   162,   172,     0,   162,
     172,   166,     0,   186,     0,   117,   186,     0,     0,   146,
     536,     0,   158,     0,   142,     0,     0,   143,     0,   194,
       0,   537,     0,   536,   275,   537,     0,   539,   538,     0,
      61,   534,   535,     0,     0,   581,     0,   539,   275,   581,
       0,   156,   168,    79,   705,     0,   545,   542,   544,     0,
      61,   180,   543,     0,     0,   121,   623,     0,     0,   192,
     243,     0,     0,   548,   598,   604,     0,   548,   598,   604,
       0,   548,   598,   604,     0,   549,     0,   548,   178,   691,
     549,     0,   548,   178,     6,   549,     0,   550,     0,   155,
     555,   558,   559,   563,   588,   584,   587,   589,     0,     0,
       0,     0,     0,   556,   557,   552,     0,   556,   552,     0,
     557,     0,     0,   231,   668,   551,     0,   231,   276,   650,
     277,   551,     0,   231,   657,   551,     0,   232,   668,     0,
     232,   276,   552,   650,   277,     0,   232,   657,     0,    46,
       0,   690,     0,   560,     0,   272,     0,   561,     0,   560,
     275,   561,     0,   650,     0,   650,   562,   706,     0,    10,
       0,     0,    63,   564,     0,   565,     0,   564,   275,   565,
       0,   571,     0,   566,     0,   578,     0,   567,     0,   276,
     571,   277,     0,   276,   545,   277,   562,   568,   569,     0,
     710,     0,     0,   276,   570,   277,     0,     0,   706,     0,
     570,   275,   706,     0,   572,     0,   573,     0,   574,     0,
     565,   263,    85,   566,     0,   565,   114,   582,    85,   566,
       0,   565,   582,    85,   565,   575,     0,   576,     0,   577,
       0,   122,   630,     0,   239,   276,   623,   277,     0,   708,
     579,   562,   710,     0,   708,   579,     0,   276,   653,   277,
       0,     0,   581,     0,   711,   710,     0,   711,     0,    77,
       0,   100,   583,     0,   151,   583,     0,    64,   583,     0,
       0,   127,     0,     0,    71,    19,   585,     0,     0,   586,
       0,   585,   275,   586,     0,   650,     0,    73,   630,     0,
       0,   190,   630,     0,     0,   135,   590,     0,     0,   591,
     276,   592,   277,     0,    85,     0,   164,   108,     0,   108,
       0,   164,     0,     0,   593,     0,   593,   275,   592,     0,
     594,   595,     0,   590,     0,   711,     0,   711,   594,     0,
     114,     0,    79,   276,   596,   277,     0,   126,   705,   597,
       0,   705,     0,   705,   275,   596,     0,    79,   276,   596,
     277,     0,     0,   126,    19,   599,     0,     0,   600,     0,
     599,   275,   600,     0,   650,   601,   603,     0,    11,     0,
      45,     0,     0,   231,     0,   241,     0,   240,   553,   602,
     554,     0,     0,   259,   650,     0,   259,   650,   174,   650,
       0,     0,    80,    82,   581,   624,   182,   276,   653,   277,
       0,    80,    82,   581,   624,   545,     0,   607,     0,   608,
       0,    44,    63,   580,   588,   589,   598,   604,     0,    44,
      63,   580,   609,     0,   190,    35,   121,   699,     0,   611,
       0,   612,     0,   180,   580,   156,   613,   588,   589,   598,
     604,     0,   180,   580,   156,   613,   609,     0,   614,     0,
     613,   275,   614,     0,   629,   267,   650,     0,   676,     0,
     143,    18,   628,    63,   581,   617,   619,     0,    80,    18,
     628,    82,   581,   617,   619,     0,    60,    63,   618,   174,
     618,     0,    60,   174,   618,     0,     0,   498,     0,   657,
       0,   662,     0,   107,   620,     0,     0,   666,     0,   657,
       0,   622,     0,     0,   276,   623,   277,     0,   628,     0,
     623,   275,   628,     0,   625,     0,     0,   276,   626,   277,
       0,   629,     0,   626,   275,   629,     0,   628,     0,   710,
     281,   697,     0,   710,   281,   272,     0,   697,     0,   628,
       0,   710,   281,   697,     0,   646,     0,   118,   646,     0,
     632,     0,   630,   125,   630,     0,   630,     8,   630,     0,
     632,     0,   118,   646,     0,   631,   125,   630,     0,   631,
       8,   630,     0,   647,   125,   630,     0,   647,     8,   630,
       0,   633,     0,   276,   631,   277,     0,   118,   632,     0,
     634,     0,   637,     0,   638,     0,   639,     0,   640,     0,
     645,     0,   635,     0,   643,     0,   641,     0,   642,     0,
     644,     0,   650,   267,   650,     0,   650,   268,   650,     0,
     650,   269,   650,     0,   650,    67,   650,     0,   650,   101,
     650,     0,   650,   119,   650,     0,   650,   120,   650,     0,
     650,   116,   650,     0,   650,   267,     6,   276,   546,   277,
       0,   650,   268,     6,   276,   546,   277,     0,   650,   269,
       6,   276,   546,   277,     0,   650,    67,     6,   276,   546,
     277,     0,   650,   101,     6,   276,   546,   277,     0,   650,
     119,     6,   276,   546,   277,     0,   650,   120,     6,   276,
     546,   277,     0,   650,   116,     6,   276,   546,   277,     0,
     650,   267,   636,   276,   546,   277,     0,   650,   268,   636,
     276,   546,   277,     0,   650,   269,   636,   276,   546,   277,
       0,   650,    67,   636,   276,   546,   277,     0,   650,   101,
     636,   276,   546,   277,     0,   650,   119,   636,   276,   546,
     277,     0,   650,   120,   636,   276,   546,   277,     0,   650,
     116,   636,   276,   546,   277,     0,   163,     0,     9,     0,
     650,    83,    46,    63,   650,     0,   650,    83,   118,    46,
      63,   650,     0,   650,    17,   650,     8,   650,     0,   650,
     118,    17,   650,     8,   650,     0,   650,   103,   650,     0,
     650,   118,   103,   650,     0,   650,   103,   650,    54,   650,
       0,   650,   118,   103,   650,    54,   650,     0,   650,    75,
     648,     0,   650,   118,    75,   648,     0,   650,    31,   650,
       0,   650,   118,    31,   650,     0,   650,   167,   650,     0,
     650,   118,   167,   650,     0,   650,   167,   192,   650,     0,
     650,   118,   167,   192,   650,     0,    57,   276,   545,   277,
       0,   159,   276,   545,   277,     0,   650,    83,    94,     0,
     650,    83,   118,    94,     0,   248,     0,   249,     0,   250,
       0,   251,     0,   252,     0,   253,     0,   649,     0,   276,
     653,   277,     0,   276,   546,   277,     0,   627,     0,   652,
       0,   669,     0,   656,     0,   657,     0,   427,     0,   677,
       0,   678,     0,   688,     0,   676,     0,   271,   650,     0,
     270,   650,     0,   650,   270,   650,     0,   650,    28,   650,
       0,   650,    23,   696,     0,   650,   271,   650,     0,   650,
     272,   650,     0,   650,   273,   650,     0,   276,   650,   277,
       0,   276,   547,   277,     0,   658,     0,   659,     0,   660,
       0,    39,     0,   710,   281,    39,     0,    97,     0,   651,
       0,   692,     0,   220,     0,   221,     0,   222,     0,   627,
     279,   653,   280,     0,   650,     0,   653,   275,   650,     0,
     656,     0,   271,   655,     0,   197,     0,   196,     0,   195,
       0,   223,     0,   224,     0,   655,     0,   661,     0,    38,
     199,     0,   218,   199,     0,   219,   199,     0,   282,     0,
     181,     0,   225,     0,   226,     0,   233,     0,   234,     0,
      66,     0,   165,     0,   242,     0,   199,     0,   200,   199,
       0,   663,     0,   271,   664,     0,   196,     0,   196,     0,
     663,     0,   196,     0,   668,     0,   271,   668,     0,   196,
       0,   670,     0,   671,     0,   673,     0,    32,   276,   272,
     277,     0,    32,   276,   690,   650,   277,     0,    32,   276,
      46,   650,   277,     0,   171,   276,   690,   650,   277,     0,
     171,   276,    46,   650,   277,     0,    13,   276,   690,   650,
     277,     0,    13,   276,    46,   650,   277,     0,   110,   276,
     690,   650,   277,     0,   110,   276,    46,   650,   277,     0,
     106,   276,   690,   650,   277,     0,   106,   276,    46,   650,
     277,     0,   672,     0,   209,   276,   689,    63,   650,   277,
       0,   674,     0,    96,   276,   650,   277,     0,   228,   276,
     650,    63,   650,   675,   277,     0,    61,   650,     0,     0,
     693,   276,   653,   277,     0,   693,   276,   277,     0,    20,
     276,   650,    10,   372,   277,     0,   679,     0,   680,     0,
     237,   276,   650,   275,   650,   277,     0,   261,   276,   630,
     275,   650,   275,   650,   277,     0,   238,   276,   650,   275,
     653,   277,     0,   681,     0,   683,     0,   236,   686,   682,
      51,     0,   236,   686,   682,    50,   687,    51,     0,   189,
     685,   173,   687,     0,   682,   189,   685,   173,   687,     0,
     236,   684,    51,     0,   236,   684,    50,   687,    51,     0,
     189,   630,   173,   687,     0,   684,   189,   630,   173,   687,
       0,   650,     0,   650,     0,   650,     0,   264,    97,    61,
     704,     0,    69,   276,   704,   275,   650,   277,     0,   210,
       0,   211,     0,   212,     0,   213,     0,   214,     0,   215,
       0,   216,     0,   217,     0,     6,     0,     0,    46,     0,
       0,    94,     0,   198,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   198,     0,   718,     0,   201,     0,   203,     0,   204,
       0,   205,     0,   206,     0,   208,     0,   227,     0,   230,
       0,   228,     0,   238,     0,   241,     0,   247,     0,   243,
       0,   237,     0,   240,     0,   246,     0,   248,     0,   249,
       0,   250,     0,   260,     0,   254,     0,   255,     0,   261,
       0,   262,     0,   216,     0,   217,     0,   265,     0,   264,
       0,   266,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   542,   544,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   575,   579,   583,   588,   589,   592,   594,
     596,   600,   601,   605,   609,   611,   613,   615,   617,   621,
     623,   627,   629,   633,   640,   644,   648,   653,   655,   659,
     661,   665,   666,   669,   670,   672,   674,   678,   680,   682,
     684,   688,   689,   696,   698,   700,   704,   705,   709,   713,
     714,   718,   720,   727,   731,   733,   738,   744,   745,   747,
     753,   755,   756,   760,   761,   767,   772,   774,   776,   778,
     782,   783,   787,   790,   795,   797,   799,   801,   804,   809,
     815,   817,   824,   828,   831,   834,   836,   838,   840,   842,
     844,   846,   848,   850,   852,   857,   861,   863,   865,   875,
     879,   881,   892,   894,   898,   900,   901,   907,   913,   915,
     917,   921,   923,   927,   929,   933,   935,   938,   939,   946,
     964,   966,   970,   972,   976,   978,   981,   982,   985,   990,
     991,   994,   998,  1007,  1014,  1022,  1027,  1028,  1031,  1037,
    1039,  1042,  1043,  1047,  1049,  1051,  1053,  1055,  1059,  1061,
    1064,  1065,  1069,  1074,  1076,  1125,  1182,  1186,  1187,  1190,
    1191,  1194,  1196,  1200,  1201,  1202,  1205,  1206,  1207,  1213,
    1218,  1223,  1225,  1227,  1231,  1232,  1236,  1237,  1244,  1248,
    1252,  1261,  1267,  1268,  1271,  1273,  1278,  1280,  1285,  1291,
    1297,  1301,  1307,  1309,  1313,  1314,  1315,  1316,  1317,  1318,
    1321,  1323,  1326,  1327,  1331,  1336,  1338,  1343,  1344,  1346,
    1354,  1358,  1360,  1363,  1364,  1365,  1366,  1369,  1373,  1377,
    1384,  1391,  1396,  1402,  1405,  1407,  1409,  1411,  1415,  1418,
    1422,  1425,  1428,  1431,  1440,  1451,  1461,  1471,  1481,  1483,
    1487,  1489,  1493,  1494,  1498,  1504,  1505,  1509,  1514,  1516,
    1518,  1522,  1524,  1528,  1529,  1533,  1537,  1538,  1541,  1546,
    1548,  1552,  1554,  1558,  1563,  1564,  1567,  1571,  1573,  1575,
    1579,  1580,  1584,  1585,  1586,  1587,  1588,  1589,  1590,  1591,
    1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1601,  1603,
    1604,  1605,  1608,  1610,  1614,  1618,  1623,  1627,  1632,  1636,
    1640,  1642,  1646,  1650,  1656,  1661,  1666,  1668,  1670,  1674,
    1676,  1678,  1682,  1683,  1684,  1686,  1690,  1694,  1696,  1700,
    1702,  1704,  1709,  1712,  1716,  1717,  1721,  1726,  1727,  1731,
    1733,  1735,  1737,  1741,  1742,  1743,  1746,  1750,  1754,  1758,
    1800,  1807,  1815,  1817,  1821,  1822,  1826,  1832,  1839,  1862,
    1866,  1872,  1876,  1882,  1886,  1894,  1905,  1916,  1918,  1920,
    1924,  1928,  1930,  1934,  1936,  1938,  1940,  1942,  1944,  1946,
    1948,  1950,  1952,  1954,  1956,  1958,  1960,  1962,  1966,  1968,
    1972,  1978,  1982,  1985,  1992,  1994,  1996,  1999,  2002,  2005,
    2009,  2013,  2019,  2020,  2024,  2033,  2037,  2039,  2041,  2043,
    2045,  2049,  2050,  2054,  2056,  2058,  2060,  2066,  2069,  2071,
    2075,  2083,  2084,  2085,  2086,  2087,  2088,  2089,  2090,  2091,
    2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,  2100,  2101,
    2102,  2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,
    2112,  2113,  2116,  2118,  2122,  2124,  2129,  2135,  2137,  2139,
    2143,  2145,  2149,  2152,  2155,  2163,  2169,  2170,  2174,  2188,
    2190,  2192,  2194,  2208,  2218,  2219,  2223,  2224,  2230,  2234,
    2236,  2238,  2240,  2242,  2244,  2246,  2248,  2250,  2252,  2254,
    2256,  2258,  2265,  2266,  2269,  2270,  2273,  2278,  2285,  2286,
    2290,  2299,  2305,  2306,  2309,  2310,  2311,  2312,  2329,  2334,
    2339,  2358,  2375,  2382,  2383,  2390,  2395,  2402,  2409,  2418,
    2422,  2428,  2432,  2436,  2442,  2446,  2453,  2459,  2465,  2473,
    2478,  2483,  2490,  2491,  2492,  2495,  2496,  2499,  2500,  2501,
    2508,  2512,  2523,  2529,  2583,  2645,  2646,  2653,  2666,  2671,
    2676,  2683,  2685,  2692,  2693,  2694,  2698,  2701,  2704,  2712,
    2713,  2714,  2717,  2721,  2725,  2727,  2731,  2735,  2736,  2739,
    2743,  2747,  2748,  2751,  2753,  2757,  2758,  2762,  2766,  2767,
    2771,  2772,  2776,  2777,  2778,  2779,  2782,  2784,  2788,  2790,
    2794,  2796,  2799,  2801,  2803,  2807,  2809,  2811,  2815,  2817,
    2819,  2823,  2827,  2829,  2831,  2835,  2837,  2841,  2842,  2846,
    2850,  2852,  2856,  2857,  2862,  2870,  2874,  2876,  2880,  2882,
    2886,  2888,  2895,  2899,  2904,  2909,  2910,  2912,  2916,  2919,
    2931,  2935,  2939,  2943,  2947,  2949,  2951,  2953,  2957,  2959,
    2961,  2965,  2967,  2969,  2973,  2975,  2979,  2981,  2985,  2986,
    2990,  2991,  2995,  2996,  3001,  3005,  3006,  3010,  3011,  3014,
    3015,  3016,  3021,  3026,  3027,  3031,  3033,  3037,  3038,  3042,
    3043,  3044,  3047,  3052,  3057,  3061,  3062,  3065,  3069,  3073,
    3076,  3081,  3083,  3087,  3088,  3093,  3098,  3100,  3102,  3104,
    3106,  3110,  3111,  3117,  3119,  3123,  3124,  3130,  3133,  3135,
    3139,  3141,  3148,  3150,  3154,  3158,  3160,  3162,  3168,  3170,
    3174,  3175,  3179,  3181,  3184,  3185,  3189,  3191,  3193,  3197,
    3198,  3202,  3204,  3210,  3212,  3216,  3217,  3221,  3225,  3227,
    3229,  3233,  3235,  3239,  3241,  3247,  3250,  3258,  3265,  3268,
    3275,  3276,  3279,  3283,  3287,  3294,  3295,  3298,  3303,  3308,
    3309,  3313,  3317,  3324,  3326,  3330,  3332,  3334,  3337,  3338,
    3341,  3345,  3347,  3350,  3352,  3358,  3359,  3363,  3367,  3368,
    3373,  3374,  3378,  3382,  3383,  3388,  3389,  3392,  3397,  3402,
    3405,  3412,  3413,  3415,  3416,  3418,  3422,  3423,  3425,  3427,
    3431,  3433,  3437,  3438,  3440,  3444,  3445,  3446,  3447,  3448,
    3449,  3450,  3451,  3452,  3453,  3454,  3459,  3461,  3463,  3465,
    3467,  3469,  3471,  3473,  3479,  3481,  3483,  3485,  3487,  3489,
    3491,  3493,  3495,  3497,  3499,  3501,  3503,  3505,  3507,  3509,
    3513,  3514,  3520,  3522,  3526,  3528,  3533,  3535,  3537,  3539,
    3544,  3546,  3550,  3552,  3556,  3558,  3560,  3562,  3566,  3570,
    3574,  3576,  3580,  3585,  3590,  3597,  3602,  3607,  3616,  3617,
    3621,  3628,  3629,  3630,  3631,  3632,  3633,  3634,  3635,  3636,
    3637,  3638,  3640,  3642,  3649,  3651,  3653,  3660,  3667,  3674,
    3676,  3678,  3679,  3680,  3681,  3683,  3685,  3689,  3690,  3693,
    3709,  3725,  3729,  3733,  3734,  3738,  3739,  3743,  3745,  3747,
    3749,  3751,  3755,  3756,  3758,  3774,  3790,  3794,  3798,  3800,
    3804,  3808,  3811,  3814,  3817,  3820,  3825,  3827,  3832,  3833,
    3837,  3844,  3851,  3858,  3865,  3866,  3870,  3877,  3878,  3879,
    3882,  3884,  3886,  3889,  3896,  3905,  3912,  3921,  3923,  3925,
    3927,  3931,  3934,  3938,  3939,  3943,  3952,  3954,  3958,  3960,
    3964,  3970,  3971,  3974,  3978,  3981,  3985,  3986,  3989,  3991,
    3995,  3997,  4001,  4003,  4007,  4009,  4013,  4016,  4019,  4024,
    4027,  4037,  4039,  4041,  4043,  4045,  4047,  4049,  4051,  4055,
    4056,  4059,  4060,  4063,  4071,  4074,  4077,  4080,  4083,  4086,
    4089,  4092,  4095,  4098,  4101,  4104,  4107,  4110,  4113,  4116,
    4119,  4122,  4125,  4128,  4131,  4134,  4137,  4140,  4145,  4146,
    4151,  4153,  4154,  4155,  4156,  4157,  4158,  4159,  4160,  4161,
    4162,  4163,  4164,  4165,  4166,  4167,  4168,  4169,  4170,  4172,
    4173,  4174,  4175,  4176,  4177,  4178,  4179,  4180,  4181
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
  "CROSS", "NEXT", "SEQUENCE", "RESTART", "'='", "'<'", "'>'", "'+'", 
  "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", 
  "'.'", "'?'", "top", "statement", "grant", "table_noise", "privileges", 
  "privilege_list", "proc_privileges", "privilege", "grant_option", 
  "role_admin_option", "simple_proc_name", "revoke", "rev_grant_option", 
  "rev_admin_option", "non_role_grantee_list", "grantee_list", "grantee", 
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
  "begin_string", "end_string", "begin_trigger", "end_trigger", 
  "check_opt", "def_trigger_clause", "replace_trigger_clause", 
  "trigger_active", "trigger_type", "trigger_type_prefix", 
  "trigger_type_suffix", "trigger_position", "trigger_action", "alter", 
  "alter_clause", "domain_default_opt2", "domain_check_constraint2", 
  "alter_domain_ops", "alter_domain_op", "alter_ops", "alter_op", 
  "alter_column_name", "keyword_or_column", "col_opt", 
  "alter_data_type_or_domain", "alter_col_name", "drop_behaviour", 
  "alter_index_clause", "alter_sequence_clause", "init_alter_db", 
  "alter_db", "db_alter_clause", "alter_trigger_clause", 
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
  "for_update_clause", "for_update_list", "lock_clause", "select_expr", 
  "column_select", "column_singleton", "select_expr_body", "query_term", 
  "query_spec", "begin_limit", "end_limit", "begin_first", "end_first", 
  "limit_clause", "first_clause", "skip_clause", "distinct_clause", 
  "select_list", "select_items", "select_item", "as_noise", "from_clause", 
  "from_list", "table_reference", "table_primary", "derived_table", 
  "correlation_name", "derived_column_list", "alias_list", "joined_table", 
  "cross_join", "natural_join", "qualified_join", "join_specification", 
  "join_condition", "named_columns_join", "table_proc", 
  "table_proc_inputs", "table_name", "simple_table_name", "join_type", 
  "outer_noise", "group_clause", "group_by_list", "group_by_item", 
  "having_clause", "where_clause", "plan_clause", "plan_expression", 
  "plan_type", "plan_item_list", "plan_item", "table_or_alias_list", 
  "access_type", "index_list", "extra_indices_opt", "order_clause", 
  "order_list", "order_item", "order_direction", "nulls_placement", 
  "nulls_clause", "rows_clause", "insert", "delete", "delete_searched", 
  "delete_positioned", "cursor_clause", "update", "update_searched", 
  "update_positioned", "assignments", "assignment", "exec_udf", "blob", 
  "filter_clause", "blob_subtype_value", "segment_clause", 
  "segment_length", "column_parens_opt", "column_parens", "column_list", 
  "ins_column_parens_opt", "ins_column_parens", "ins_column_list", 
  "column_name", "simple_column_name", "update_column_name", 
  "search_condition", "bracable_search_condition", 
  "simple_search_condition", "predicate", "comparison_predicate", 
  "quantified_predicate", "some", "distinct_predicate", 
  "between_predicate", "like_predicate", "in_predicate", 
  "containing_predicate", "starting_predicate", "exists_predicate", 
  "singular_predicate", "null_predicate", "trigger_action_predicate", 
  "special_trigger_action_predicate", "in_predicate_value", 
  "table_subquery", "value", "datetime_value_expression", "array_element", 
  "value_list", "constant", "u_numeric_constant", "u_constant", 
  "parameter", "current_user", "current_role", "internal_info", 
  "sql_string", "signed_short_integer", "nonneg_short_integer", 
  "neg_short_integer", "pos_short_integer", "unsigned_short_integer", 
  "signed_long_integer", "long_integer", "function", "aggregate_function", 
  "numeric_value_function", "extract_expression", "string_value_function", 
  "substring_function", "string_length_opt", "udf", "cast_specification", 
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
  "symbol_procedure_name", "symbol_role_name", "symbol_table_alias_name", 
  "symbol_table_name", "symbol_trigger_name", "symbol_user_name", 
  "symbol_variable_name", "symbol_view_name", "symbol_savepoint_name", 
  "valid_symbol_name", "non_reserved_word", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   283,   283,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   285,   285,   285,   286,   286,   287,   287,
     287,   288,   288,   289,   290,   290,   290,   290,   290,   291,
     291,   292,   292,   293,   294,   294,   294,   295,   295,   296,
     296,   297,   297,   298,   298,   298,   298,   299,   299,   299,
     299,   300,   300,   301,   301,   301,   302,   302,   303,   304,
     304,   305,   305,   306,   307,   307,   308,   309,   309,   309,
     310,   310,   310,   311,   311,   312,   313,   313,   313,   313,
     314,   314,   315,   315,   316,   316,   316,   316,   316,   317,
     318,   318,   319,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   321,   322,   322,   322,   323,
     324,   324,   325,   325,   326,   326,   326,   327,   328,   328,
     328,   329,   329,   330,   330,   331,   331,   332,   332,   333,
     334,   334,   335,   335,   336,   336,   337,   337,   338,   339,
     339,   340,   341,   342,   343,   344,   345,   345,   346,   347,
     347,   348,   348,   349,   349,   349,   349,   349,   350,   350,
     351,   351,   352,   352,   352,   353,   354,   355,   355,   356,
     356,   357,   357,   358,   358,   358,   359,   359,   359,   360,
     361,   362,   362,   362,   363,   363,   364,   364,   365,   365,
     365,   366,   367,   367,   368,   368,   369,   369,   370,   371,
     372,   373,   374,   374,   375,   375,   375,   375,   375,   375,
     376,   376,   377,   377,   378,   379,   379,   379,   379,   379,
     380,   381,   381,   382,   382,   382,   382,   383,   384,   385,
     386,   386,   387,   388,   388,   388,   388,   388,   389,   390,
     391,   391,   391,   391,   392,   393,   394,   395,   396,   396,
     397,   397,   398,   398,   399,   400,   400,   401,   402,   402,
     402,   403,   403,   404,   404,   405,   406,   406,   407,   408,
     408,   409,   409,   410,   411,   411,   412,   413,   413,   413,
     414,   414,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   416,   416,   417,   418,   419,   420,   421,   422,
     423,   423,   424,   425,   426,   427,   428,   428,   428,   429,
     429,   429,   430,   430,   430,   430,   431,   432,   432,   433,
     433,   433,   434,   434,   435,   435,   436,   437,   437,   438,
     438,   438,   438,   439,   439,   439,   440,   441,   442,   443,
     444,   445,   446,   446,   447,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   455,   456,   457,   458,   458,   458,
     459,   460,   460,   461,   461,   461,   461,   461,   461,   461,
     461,   461,   461,   461,   461,   461,   461,   461,   462,   462,
     463,   464,   465,   465,   465,   465,   465,   465,   465,   465,
     466,   467,   468,   468,   469,   469,   469,   469,   469,   469,
     469,   470,   470,   471,   471,   471,   471,   471,   471,   471,
     472,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   474,   474,   475,   475,   476,   477,   477,   477,
     478,   478,   479,   479,   479,   480,   481,   481,   482,   482,
     482,   482,   482,   483,   484,   484,   485,   485,   486,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   488,   488,   489,   489,   490,   490,   491,   491,
     492,   492,   493,   493,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   495,   495,   496,   496,   496,   496,   497,
     497,   498,   498,   498,   499,   499,   500,   500,   500,   501,
     501,   501,   502,   502,   502,   503,   503,   504,   504,   504,
     505,   505,   506,   506,   506,   507,   507,   508,   508,   508,
     508,   509,   509,   510,   510,   510,   511,   511,   511,   512,
     512,   512,   513,   514,   515,   515,   516,   517,   517,   518,
     519,   520,   520,   521,   521,   522,   522,   523,   524,   524,
     525,   525,   526,   526,   526,   526,   527,   527,   528,   528,
     529,   529,   530,   530,   530,   531,   531,   531,   532,   532,
     532,   533,   534,   534,   534,   535,   535,   536,   536,   537,
     538,   538,   539,   539,   540,   541,   542,   542,   543,   543,
     544,   544,   545,   546,   547,   548,   548,   548,   549,   550,
     551,   552,   553,   554,   555,   555,   555,   555,   556,   556,
     556,   557,   557,   557,   558,   558,   559,   559,   560,   560,
     561,   561,   562,   562,   563,   564,   564,   565,   565,   566,
     566,   566,   567,   568,   568,   569,   569,   570,   570,   571,
     571,   571,   572,   573,   574,   575,   575,   576,   577,   578,
     578,   579,   579,   580,   580,   581,   582,   582,   582,   582,
     582,   583,   583,   584,   584,   585,   585,   586,   587,   587,
     588,   588,   589,   589,   590,   591,   591,   591,   591,   591,
     592,   592,   593,   593,   594,   594,   595,   595,   595,   596,
     596,   597,   597,   598,   598,   599,   599,   600,   601,   601,
     601,   602,   602,   603,   603,   604,   604,   604,   605,   605,
     606,   606,   607,   608,   609,   610,   610,   611,   612,   613,
     613,   614,   615,   616,   616,   617,   617,   617,   618,   618,
     498,   619,   619,   620,   620,   621,   621,   622,   623,   623,
     624,   624,   625,   626,   626,   627,   627,   627,   628,   629,
     629,   630,   630,   630,   630,   630,   631,   631,   631,   631,
     631,   631,   632,   632,   632,   633,   633,   633,   633,   633,
     633,   633,   633,   633,   633,   633,   634,   634,   634,   634,
     634,   634,   634,   634,   635,   635,   635,   635,   635,   635,
     635,   635,   635,   635,   635,   635,   635,   635,   635,   635,
     636,   636,   637,   637,   638,   638,   639,   639,   639,   639,
     640,   640,   641,   641,   642,   642,   642,   642,   643,   644,
     645,   645,   646,   646,   646,   647,   647,   647,   648,   648,
     649,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   650,   650,   650,   650,   650,   650,   650,   651,
     651,   651,   652,   653,   653,   654,   654,   655,   655,   655,
     655,   655,   656,   656,   656,   656,   656,   657,   658,   658,
     659,   660,   660,   660,   660,   660,   661,   661,   662,   662,
     663,   664,   665,   666,   667,   667,   668,   669,   669,   669,
     670,   670,   670,   670,   670,   670,   670,   670,   670,   670,
     670,   671,   672,   673,   673,   674,   675,   675,   676,   676,
     677,   678,   678,   679,   679,   679,   680,   680,   681,   681,
     682,   682,   683,   683,   684,   684,   685,   686,   687,   688,
     688,   689,   689,   689,   689,   689,   689,   689,   689,   690,
     690,   691,   691,   692,   693,   694,   695,   696,   697,   698,
     699,   700,   701,   702,   703,   704,   705,   706,   707,   708,
     709,   710,   711,   712,   713,   714,   715,   716,   717,   717,
     718,   718,   718,   718,   718,   718,   718,   718,   718,   718,
     718,   718,   718,   718,   718,   718,   718,   718,   718,   718,
     718,   718,   718,   718,   718,   718,   718,   718,   718
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     8,     8,     5,     1,     0,     1,     2,
       1,     1,     3,     1,     1,     1,     1,     2,     2,     3,
       0,     3,     0,     1,     8,     8,     5,     3,     0,     3,
       0,     1,     1,     1,     3,     3,     3,     2,     2,     2,
       2,     1,     3,     1,     2,     2,     1,     3,     1,     1,
       3,     1,     2,     2,     2,     3,     8,     1,     1,     5,
       0,     1,     3,     1,     3,     3,     0,     2,     2,     1,
       1,     3,     3,     2,     0,     2,     2,     1,     3,     9,
       1,     1,     2,     3,     7,     2,     2,     2,     2,     2,
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
       2,     2,     3,     4,     2,     6,     4,     8,     9,     6,
       8,     6,     4,     0,     4,     2,     1,     3,     0,     2,
       4,     0,     1,     1,     3,     3,     7,     2,     0,     2,
       2,     3,     3,     0,     1,     2,     4,     1,     3,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     5,     0,
       4,     7,     3,     0,     1,     3,     3,     7,     7,     0,
       0,     0,     0,     3,     0,     9,     9,     1,     1,     0,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     5,     5,     5,     5,     5,     5,     2,     0,
       4,     2,     3,     3,     2,     2,     3,     3,     2,     2,
       3,     6,     1,     2,     4,     3,     2,     2,     2,     2,
       3,     1,     3,     3,     3,     2,     2,     4,     4,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     0,
       2,     2,     4,     4,     5,     0,     1,     2,     2,     4,
       3,     2,     2,     7,     1,     0,     1,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     2,     2,
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
       3,     0,     1,     3,     4,     3,     3,     0,     2,     0,
       2,     0,     3,     3,     3,     1,     4,     4,     1,     9,
       0,     0,     0,     0,     3,     2,     1,     0,     3,     5,
       3,     2,     5,     2,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     0,     2,     1,     3,     1,     1,     1,
       1,     3,     6,     1,     0,     3,     0,     1,     3,     1,
       1,     1,     4,     5,     5,     1,     1,     2,     4,     4,
       2,     3,     0,     1,     2,     1,     1,     2,     2,     2,
       0,     1,     0,     3,     0,     1,     3,     1,     2,     0,
       2,     0,     2,     0,     4,     1,     2,     1,     1,     0,
       1,     3,     2,     1,     1,     2,     1,     4,     3,     1,
       3,     4,     0,     3,     0,     1,     3,     3,     1,     1,
       0,     1,     1,     4,     0,     2,     4,     0,     8,     5,
       1,     1,     7,     4,     4,     1,     1,     8,     5,     1,
       3,     3,     1,     7,     7,     5,     3,     0,     1,     1,
       1,     2,     0,     1,     1,     1,     0,     3,     1,     3,
       1,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     2,     1,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       1,     1,     5,     6,     5,     6,     3,     4,     5,     6,
       3,     4,     3,     4,     3,     4,     4,     5,     4,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       4,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     1,     6,     1,     4,     7,     2,     0,     4,     3,
       6,     1,     1,     6,     8,     6,     1,     1,     4,     6,
       4,     5,     3,     5,     4,     5,     1,     1,     1,     4,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   582,   123,     0,     0,     0,     0,     0,     0,
       0,     0,    50,   582,   647,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   564,    18,   569,   570,   571,     5,    17,   563,
     565,    19,   627,   734,   635,   638,    11,     8,   750,   751,
      21,   755,   756,     4,   475,     0,     0,     0,     0,     0,
       0,     0,   401,   581,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,     0,     0,   102,   740,
     930,     0,    22,   928,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,     0,   363,    28,    36,    33,    35,   776,    34,
     776,  1018,  1020,  1021,  1022,  1023,  1024,  1025,  1044,  1045,
    1026,  1028,  1027,  1033,  1029,  1034,  1030,  1032,  1035,  1031,
    1036,  1037,  1038,  1040,  1041,  1039,  1042,  1043,  1047,  1046,
    1048,     0,    30,     0,    31,     0,    66,    68,  1010,  1019,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     990,   641,   646,     0,     0,   589,     0,   693,   695,  1012,
       0,     0,     0,   115,   572,  1017,     0,     2,     0,   631,
       0,   992,   747,     0,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   460,   454,   455,   453,   456,
     457,   458,   459,   461,     0,   430,   431,     0,  1002,   408,
       0,  1006,   405,   259,  1009,     0,   695,   404,   379,  1013,
     409,     0,  1005,   586,   579,   926,     0,   111,   159,   158,
     112,   141,   208,   788,   998,     0,   109,   153,     0,   105,
     259,   113,   932,   130,   106,   193,   107,     0,   108,   776,
    1016,   114,   154,   110,   738,   739,     0,   931,   929,     0,
      74,     0,  1003,   711,   496,  1001,   489,     0,   495,   500,
     490,   491,   498,   492,   493,   494,   499,   501,   328,     0,
     261,    29,     0,    38,   775,    37,    27,     0,     0,     0,
       0,     0,   781,     0,     0,     0,     0,     0,     0,   578,
     936,     0,   917,   640,   640,   641,   653,   651,   989,   654,
       0,   655,   645,   641,     0,     0,     0,     0,     0,     0,
     605,   598,   587,   588,   590,   592,   593,   594,   601,   602,
     595,     0,   694,  1011,   116,   259,   117,   193,   118,   776,
     575,   629,     0,   625,     0,     0,   991,     0,     0,   632,
       0,     0,     0,     0,   406,   476,   371,     0,   371,     0,
     211,   407,   412,   402,   470,   471,     0,   261,   232,   462,
       0,   403,   421,     0,   377,   378,   485,     0,   585,   583,
     927,   156,   156,     0,     0,     0,   168,   160,   161,   140,
       0,   103,     0,     0,   119,   261,   129,   128,   131,     0,
       0,     0,     0,     0,   994,    75,   211,     0,     0,   713,
     753,   497,     0,     0,     0,     0,   894,   923,     0,   993,
       0,   896,     0,     0,   924,     0,   918,   909,   908,   907,
    1018,     0,     0,     0,   899,   900,   901,   910,   911,   919,
     920,  1028,   921,   922,     0,  1033,  1029,   925,  1042,  1047,
       0,     0,     0,     0,   876,   360,   871,   785,   903,   897,
     872,   326,   912,   874,   875,   891,   892,   893,   913,   873,
     937,   938,   951,   939,   953,   880,   877,   878,   961,   962,
     966,   967,   879,   898,     0,     0,   998,     0,     0,     0,
     364,   209,     0,     0,     0,   778,    26,     0,    32,     0,
       0,    42,    69,    71,  1014,    67,     0,     0,     0,   780,
       0,    47,    49,    27,     0,     0,   577,     0,     0,     0,
     650,   648,     0,   657,     0,   656,   658,   663,   644,     0,
     624,     0,   599,   610,   596,   610,   597,   611,   617,   621,
     622,   606,   591,   711,   759,   789,     0,     0,   261,     0,
       0,   574,   573,     0,   626,   630,   733,   735,   740,   637,
     636,   745,   176,     0,   478,   137,     0,   481,     0,   482,
     477,     0,   416,     0,   418,   417,     0,   419,     0,   413,
       0,     0,   262,     0,     0,   425,     0,   426,   371,   463,
       0,   469,     0,     0,     0,   466,   998,   382,   381,   484,
       0,   399,     0,   157,     0,     0,   166,     0,   165,     0,
       0,   155,   169,   170,   172,   162,   533,   545,   520,   555,
     523,   546,   556,     0,   562,   524,     0,   552,     0,   547,
     559,   519,   542,   521,   522,   517,   371,   502,   503,   504,
     512,   518,   505,   514,   535,     0,   540,   537,   515,   552,
     516,   120,   259,   121,     0,     0,   132,     0,     0,   192,
     232,   379,   369,     0,   211,     0,    81,    83,     0,     0,
     101,   100,   995,     0,     0,     0,     0,   862,   863,   864,
       0,   710,   793,   802,   805,   811,   806,   807,   808,   809,
     813,   814,   812,   815,   810,   791,     0,   719,   734,   990,
       0,   990,   914,     0,     0,   990,   990,   990,     0,   915,
     916,     0,     0,   977,     0,     0,     0,     0,     0,     0,
     882,   881,     0,   734,   903,     0,   325,  1015,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   512,
     535,     0,     0,   362,     0,   272,     0,   777,     0,     0,
      43,    72,     0,     0,    25,   767,     0,   783,     0,   749,
     767,     0,     0,    46,   576,     0,   640,     0,     0,   711,
       0,   662,     0,   567,     0,   566,   934,     0,   600,     0,
     608,   604,   603,     0,   614,     0,   619,   607,     0,   713,
     758,     0,     0,     0,   232,   369,   628,     0,   744,     0,
       0,   138,   177,   480,   371,   415,     0,   371,   372,   420,
     369,     0,   258,   369,   231,   999,   203,   200,     0,   143,
     371,   502,   369,     0,     0,     0,   230,   233,   234,   235,
     236,     0,   424,   468,   467,   423,   422,     0,     0,     0,
     385,   383,   384,   380,     0,   371,   473,     0,   472,   186,
     163,   167,     0,     0,   171,     0,     0,   530,   770,   543,
     544,   560,     0,   557,   562,     0,   550,   548,   549,   143,
       0,     0,     0,   513,     0,     0,     0,     0,   551,   261,
       0,   369,   133,   191,     0,   194,   196,   197,     0,     0,
       0,     0,   211,   211,    78,     0,    86,    77,     0,     0,
       0,     0,   804,   792,     0,     0,   865,   866,   867,     0,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     715,   717,   718,   712,     0,   747,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     981,   982,   983,   984,   985,   986,   987,   988,     0,     0,
       0,     0,   972,     0,     0,     0,     0,     0,     0,     0,
     890,   747,   889,   327,     0,   885,   997,   884,   883,   886,
     887,   888,   904,   959,     0,   895,   787,   786,   366,   365,
       0,   265,   280,     0,   271,   273,   779,     0,     0,     0,
      70,     0,   772,     0,   782,     0,   772,     0,     0,   649,
     652,     0,   664,   665,   668,   670,   667,   679,   680,   681,
     669,   692,     0,   704,   659,   661,  1007,   568,   935,   609,
     618,   613,   612,     0,   623,   760,   734,   761,   790,   369,
       0,     0,   736,   642,   737,   746,   479,   156,   183,   175,
     178,   179,     0,     0,     0,   414,   369,   369,   372,   270,
     263,   272,   202,   371,   371,   372,   204,   199,   205,     0,
       0,   241,     0,   428,   372,   371,   369,   427,     0,     0,
       0,   398,   487,   474,   187,   188,   164,     0,   174,   531,
     532,   933,     0,     0,     0,   535,     0,   558,     0,     0,
     372,     0,   508,   510,     0,     0,     0,     0,     0,     0,
       0,   379,   272,   156,   135,   232,   189,   399,   374,     0,
      82,     0,   211,     0,    90,     0,    84,     0,     0,    89,
      85,     0,   754,  1000,     0,     0,   797,     0,     0,   803,
       0,     0,   795,   794,     0,   852,     0,   841,   840,     0,
     819,     0,   850,   868,     0,   860,     0,     0,     0,   820,
     846,     0,     0,   823,     0,     0,     0,     0,     0,     0,
       0,   821,     0,     0,   822,     0,   854,     0,     0,   816,
       0,     0,   817,     0,     0,   818,   716,   719,   752,     0,
       0,   211,     0,   940,     0,     0,   954,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   978,     0,     0,   976,
       0,     0,   968,     0,     0,     0,     0,   979,   634,   902,
     958,     0,   260,   279,     0,   338,   361,   274,     0,     0,
       0,     0,     0,  1024,    40,    51,    53,    52,    61,    63,
      40,    41,   533,   533,     0,   764,   784,     0,   763,     0,
       0,     0,   700,   667,     0,   702,   696,   702,   700,   702,
       0,     0,     0,   663,     0,   709,   615,   616,   620,   747,
     272,   190,   374,     0,     0,   184,     0,   180,     0,     0,
       0,   410,   219,   214,   905,   215,   216,   217,   218,     0,
       0,   264,     0,     0,     0,   220,     0,   241,   740,   237,
       0,   429,   464,   465,   390,   391,   387,   386,   389,   388,
     371,   486,   372,   173,   996,     0,     0,   526,     0,   525,
     561,     0,   553,   144,     0,   506,     0,   534,   535,   541,
     539,     0,   536,   369,     0,     0,     0,   127,   136,   195,
     371,     0,   370,   104,     0,   125,   124,    93,     0,     0,
      94,     0,    87,    88,     0,   858,   859,   799,   798,   801,
     800,     0,     0,     0,     0,   734,     0,     0,     0,   861,
       0,     0,     0,     0,     0,     0,   853,   851,   847,     0,
     855,     0,     0,     0,     0,   856,     0,     0,     0,     0,
       0,     0,   723,     0,   720,     0,   724,   946,   945,     0,
       0,   942,   941,     0,   950,   949,   948,   947,   944,   943,
       0,   957,   974,   973,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,   276,   277,     0,   998,     0,     0,
       0,     0,     0,     0,     0,  1026,  1031,     0,     0,   359,
     290,   285,     0,   338,   284,   294,   295,   296,   297,   298,
     305,   306,   299,   301,   302,   304,     0,   309,     0,   353,
     355,   354,     0,     0,     0,     0,     0,     0,   762,     0,
     998,    65,    57,    58,    64,    59,    60,     0,    23,     0,
       0,    24,   768,     0,   769,   766,   771,   774,   773,   748,
      44,    45,   663,   671,   666,   701,   699,   697,     0,   698,
       0,     0,     0,     0,     0,     0,   713,   757,     0,   370,
     741,   742,   643,   186,   185,   181,     0,   372,   906,   268,
     269,   372,   372,   142,   207,   221,   222,   371,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   483,
     528,     0,   529,     0,     0,   207,   145,   146,   148,   149,
     150,   509,   511,   507,   538,   272,   399,   372,   186,     0,
       0,   367,   371,    91,     0,     0,    97,    92,   535,     0,
     844,     0,     0,   870,   747,   869,   842,     0,     0,     0,
     848,     0,     0,     0,     0,   857,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   714,   719,     0,   726,
       0,   722,   725,   960,   210,   980,   952,     0,     0,   975,
     970,   969,     0,   963,   965,     0,   213,   275,     0,   314,
       0,     0,     0,   308,     0,   323,   307,   339,   340,     0,
    1008,   356,   357,     0,   286,     0,   291,   288,   344,     0,
       0,   310,     0,   300,   293,   303,   292,   311,   337,     0,
      54,    55,    56,    62,   533,   674,     0,   682,   700,   691,
     689,   703,   705,   707,   708,   639,   372,   368,   743,   182,
     372,   152,   257,     0,     0,   198,   223,     0,     0,     0,
     241,   224,   227,   776,     0,   372,   396,   397,   393,   392,
     395,   394,     0,   527,   554,   151,   139,   147,     0,   371,
     254,   134,   372,   373,     0,     0,    95,    96,    79,     0,
     827,   835,   633,   843,   828,   836,   831,   839,   845,   849,
     829,   837,   830,   838,   824,   832,   825,   833,   826,   834,
     721,     0,   732,   956,   955,   971,     0,     0,     0,   282,
     278,     0,   312,     0,   328,     0,     0,     0,   341,     0,
     352,     0,     0,     0,     0,   347,   345,     0,     0,     0,
     332,     0,   333,     0,    39,   765,   676,   673,   683,     0,
       0,   684,   685,   686,     0,   255,   411,   201,   206,   225,
     241,   776,   228,   247,   240,   242,   400,   372,     0,   375,
     372,    76,    98,    99,     0,   729,     0,   728,   964,   212,
     281,     0,   313,   331,     0,   316,     0,   322,     0,   351,
     350,  1004,   349,   338,     0,     0,     0,     0,   324,     0,
       0,     0,   672,   687,     0,   706,   229,   247,     0,   241,
     243,   244,   256,   372,     0,   727,     0,     0,     0,     0,
       0,     0,   338,   358,   346,   348,     0,   343,     0,   335,
     334,     0,   677,     0,   241,     0,     0,   239,     0,   246,
       0,   245,   376,   126,   730,     0,   283,     0,   329,   315,
     319,   321,     0,     0,     0,   338,     0,   675,   688,   226,
       0,     0,   250,   249,   248,   731,     0,   338,     0,     0,
     338,   336,   678,   253,   251,   252,   330,   320,   338,   342,
     317,   318,     0,     0,     0
};

static const short yydefgoto[] =
{
    1912,    20,    21,   507,   141,   142,   143,   144,  1488,   764,
     759,    22,   155,   156,  1244,  1245,  1246,  1247,  1248,   145,
     146,   511,   512,    23,    86,   415,   906,   675,   676,   677,
    1140,  1133,  1134,  1577,   270,   679,    24,    78,    25,   173,
      26,   404,    79,  1353,   251,   408,   667,  1124,  1347,   574,
     240,   400,  1075,  1555,  1556,  1557,  1558,  1559,  1560,   246,
     261,   237,   614,   238,   396,   397,   398,   621,   622,   623,
     575,   576,  1059,  1060,  1061,  1286,  1096,   254,   346,   410,
     894,   895,   896,   827,   828,   829,  1685,   596,   497,  1409,
     678,  1749,  1291,  1534,  1535,  1536,  1691,   897,   598,   836,
     837,   838,   839,  1309,   840,  1839,  1840,  1841,  1893,   249,
     344,   661,   222,   377,   503,   591,   592,  1000,   498,  1068,
    1003,  1004,  1005,  1433,  1434,  1234,  1750,  1435,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,
    1462,  1463,  1757,  1464,   464,   465,  1850,  1771,  1465,  1466,
    1467,  1884,  1647,  1648,  1764,  1765,  1468,  1469,  1470,  1471,
    1643,    27,    28,   290,   499,   500,   258,   348,   899,  1571,
     583,  1065,  1352,   256,   663,   386,   609,   610,   853,   855,
    1321,    29,    62,   818,   815,   371,   372,   381,   382,   214,
     215,   383,  1084,   604,   845,   219,   230,   183,   364,   365,
     227,   611,  1322,    30,   101,   646,   647,   648,  1111,  1112,
     649,   749,   651,   652,  1105,  1492,   883,   653,   750,   655,
     656,   657,   658,   876,   659,   660,   873,    31,    32,    33,
      34,    35,   562,    36,   527,    37,    38,    64,   234,   389,
      39,   332,   333,   334,   335,   336,   337,   338,   339,   791,
     340,  1043,  1278,   547,   548,   796,   549,    40,  1472,   179,
     564,   353,    42,  1374,   732,    43,    44,    45,   530,   322,
    1283,  1678,   160,   161,   162,   320,   534,   535,   536,   782,
     779,  1022,  1023,  1024,  1025,  1776,  1832,  1861,  1026,  1027,
    1028,  1029,  1781,  1782,  1783,  1030,  1273,   166,   167,  1271,
    1506,  1275,  1671,  1672,  1516,   419,   708,  1402,   944,  1403,
    1404,  1405,  1611,  1804,  1807,   182,   566,   567,   266,  1522,
    1054,   359,  1473,  1474,    48,    49,   420,  1475,    51,    52,
     553,  1476,  1477,    53,  1012,  1493,  1255,  1496,   293,   294,
     504,   518,   519,   766,   466,   467,   556,   691,   919,   692,
     693,   694,   695,  1159,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   921,  1162,  1163,   706,   469,   470,
     471,  1293,   472,   473,   474,   475,   476,   477,   478,   868,
      83,   268,   253,  1103,  1113,   786,   479,   480,   481,   482,
     483,   484,  1618,   485,   486,   487,   488,   489,   490,   975,
     491,   724,  1220,   725,  1217,   492,   968,   321,   357,   493,
     494,   681,  1323,   985,   243,   824,  1142,   274,   217,   271,
    1820,   247,  1805,  1035,  1479,  1031,   147,   495,   226,   228,
    1249,   736,   259,   174,   496,   149
};

static const short yypact[] =
{
    7359,  1461,    49,  1440,   460,   859,   197,   146,    39,  5233,
     559,   254,   785,    49,   691,   431,  6727,   531,  6727,   266,
     289,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   556,   482,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  7912,  6727,  6727,  6727,  6727,
    6727,  6727,-32768,-32768,   489,   748,  6727,  6727,  6727,   637,
    6727,   444,  6727,  6727,-32768,  6727,  6727,  6727,-32768,   795,
  -32768,   462,-32768,-32768,   692,  6727,-32768,  6727,  6727,  6727,
     727,  6727,  6727,  6727,  6727,   444,  6727,  6727,  6727,  6727,
    6727,-32768,  6727,   528,   685,-32768,-32768,-32768,   572,-32768,
     572,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   735,   585,   750,-32768,    10,-32768,-32768,-32768,-32768,
    6727,  6727,  6727,   782,   789,   791,  6727,   701,   441,   450,
     218,   719,-32768,  6727,   875,  1198,   808,-32768,  6727,-32768,
    6727,  6727,  6727,-32768,-32768,-32768,  6727,-32768,   777,   776,
     955,   486,   722,   227,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   282,-32768,-32768,   748,-32768,-32768,
     229,-32768,-32768,   761,-32768,   781,-32768,-32768,   452,-32768,
  -32768,   747,-32768,   839,-32768,-32768,   843,-32768,   541,-32768,
  -32768,   978,-32768,-32768,-32768,   748,-32768,-32768,   584,-32768,
     761,-32768,-32768,   172,-32768,   934,-32768,   998,-32768,   572,
  -32768,-32768,-32768,-32768,-32768,-32768,   990,-32768,-32768,   879,
  -32768,  1009,-32768,   894,-32768,-32768,-32768,   879,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6853,  6727,
     945,-32768,  6727,-32768,-32768,-32768,   932,  1172,   972,  4213,
    6727,  1036,   845,  1062,  1070,  1072,  1023,  1025,    52,   895,
  -32768,  7072,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5320,-32768,-32768,-32768,   977,  6727,  1050,   970,   415,  6727,
     987,-32768,-32768,  1198,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  6727,-32768,-32768,-32768,   761,-32768,   934,-32768,   572,
    1037,  1046,   925,-32768,  7072,  1014,-32768,  1014,  7072,-32768,
      66,   917,   919,   918,   227,-32768,  1146,   223,-32768,  6727,
  -32768,   282,-32768,-32768,-32768,-32768,  6727,   945,  2651,   971,
    2907,   908,-32768,  7912,-32768,-32768,   831,   992,-32768,-32768,
  -32768,   939,   939,   748,  1080,   748,   133,   541,-32768,-32768,
    3827,-32768,  6727,  6727,-32768,   945,-32768,-32768,  1168,   427,
     935,  6727,  1202,  6727,-32768,-32768,    80,  6289,  4339,  1083,
  -32768,-32768,   943,   944,   951,  1016,-32768,-32768,   959,-32768,
     982,-32768,   983,   985,-32768,   986,-32768,-32768,-32768,-32768,
     988,   994,  1022,  1047,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   999,-32768,-32768,  5539,  1008,  1019,-32768,  1021,  1151,
    7072,  7072,  5758,  6727,-32768,-32768,   974,-32768,   507,-32768,
  -32768,  1024,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1028,  1032,  1039,  3827,  1042,   567,
  -32768,-32768,  1030,  1253,   609,-32768,-32768,  6727,-32768,  6727,
    6727,    19,-32768,-32768,-32768,-32768,  6727,  6727,    89,-32768,
    6727,-32768,-32768,   932,  1180,  4213,-32768,  6727,  5758,   393,
  -32768,-32768,  7072,-32768,  1259,  1051,-32768,   198,-32768,   256,
  -32768,    95,-32768,    98,-32768,    98,-32768,  1053,-32768,   155,
  -32768,  1159,-32768,    20,-32768,-32768,  1064,  1056,   945,  1066,
    1329,-32768,-32768,  6727,-32768,-32768,  1068,-32768,   340,-32768,
  -32768,  1006,-32768,  1257,  1260,-32768,   748,-32768,  1261,-32768,
  -32768,  1332,-32768,  1333,-32768,-32768,  1319,-32768,  3827,-32768,
    3827,   615,-32768,  1353,  6727,-32768,  7652,-32768,   416,-32768,
    6727,   706,   781,  1191,  1161,  1231,  1196,-32768,-32768,-32768,
     248,  1236,   478,-32768,  1177,   444,-32768,   748,-32768,  1354,
    1287,-32768,   133,-32768,-32768,-32768,   379,  1193,-32768,-32768,
  -32768,  1208,-32768,  1247,  1111,-32768,  1303,  1119,   466,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1117,-32768,-32768,-32768,   107,  1122,  1123,    28,-32768,  1119,
  -32768,-32768,   761,-32768,  1339,  1391,-32768,   748,   748,-32768,
    2651,   452,-32768,  1281,-32768,  1258,  1131,-32768,  3561,  1280,
  -32768,-32768,-32768,  1288,  1134,  4593,  1135,  1182,  1263,  1301,
    4085,   449,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1937,   560,  1286,   653,
    7072,   129,-32768,  6727,  7072,   682,   700,   788,  1337,-32768,
  -32768,  7072,  4847,   507,   141,  1226,  7072,  7072,  4847,  1355,
     137,   137,  1145,   482,   408,   662,-32768,-32768,  7072,  6727,
    7072,  7072,  7072,  7072,  7072,  7072,  5977,  3173,-32768,-32768,
    1402,  1149,  6727,-32768,  6727,  1386,  6727,-32768,  1271,  1272,
  -32768,-32768,  1245,  4213,-32768,  1388,   675,-32768,  1184,-32768,
    1388,  6727,  6727,  1187,-32768,   408,-32768,   421,  4986,  1274,
    7072,-32768,  6727,-32768,   605,-32768,-32768,   228,-32768,  1279,
  -32768,-32768,-32768,  6727,   647,  6727,-32768,-32768,  6727,  1083,
  -32768,  7072,  6727,  1456,  2651,-32768,  1192,  7072,  1229,  7072,
     748,-32768,   345,-32768,-32768,-32768,  1199,-32768,-32768,-32768,
      70,  6727,-32768,-32768,-32768,-32768,  1455,-32768,  1203,  1433,
  -32768,  1462,-32768,  1397,  1405,   572,-32768,-32768,-32768,-32768,
  -32768,  1472,-32768,-32768,-32768,-32768,-32768,  6727,  7843,   444,
    1371,  1372,  1374,-32768,   444,-32768,-32768,   631,-32768,   798,
  -32768,-32768,  1344,   748,-32768,  6289,   290,  1349,-32768,-32768,
  -32768,-32768,   444,-32768,  1111,   471,-32768,-32768,-32768,  1433,
     471,  1350,   471,-32768,   444,   444,  1234,   444,-32768,   945,
    6727,-32768,  1409,-32768,   702,-32768,-32768,-32768,   831,  1014,
    6727,   703,    37,-32768,-32768,  1239,   510,-32768,  6289,  6727,
    1014,  5101,-32768,-32768,  1014,  4593,-32768,-32768,-32768,   121,
  -32768,   457,  1648,  4847,  4847,  7072,  7072,  1865,  1241,   715,
    2150,  7072,  2499,   540,  2767,  3033,  6196,  3299,  3565,  3831,
  -32768,-32768,  1404,-32768,  1243,   722,  7072,  7072,   175,  7072,
    1246,  7072,  1249,   597,  7072,  7072,  7072,  7072,  7072,  7072,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1463,  1154,
     185,  7072,-32768,  4847,  7072,   177,   865,  1017,   150,  6727,
  -32768,   722,-32768,-32768,   239,-32768,-32768,  1502,   137,   137,
     838,   838,   507,-32768,   717,-32768,-32768,-32768,-32768,-32768,
     725,-32768,  1343,  1512,  1386,-32768,-32768,  5642,  5642,  1413,
  -32768,   126,  1431,  6727,-32768,  7072,  1431,  1476,  1480,-32768,
  -32768,  3959,  1270,   232,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1283,  4847,  1484,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   492,-32768,-32768,  1286,   507,-32768,-32768,
     729,  1014,-32768,-32768,-32768,   507,-32768,   939,  1544,-32768,
     345,-32768,  1284,  4847,  2244,-32768,-32768,-32768,-32768,-32768,
  -32768,  1386,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   572,
     572,  1318,  1285,-32768,-32768,-32768,-32768,-32768,   423,   211,
     589,-32768,  1552,-32768,-32768,-32768,-32768,  6727,-32768,-32768,
  -32768,-32768,   460,   745,  1403,  1402,  1289,-32768,  1177,   766,
  -32768,   628,-32768,  1297,  6727,   635,  1290,  1299,   444,  1300,
    1554,   452,  1386,   939,  1260,  2651,-32768,  1236,  1387,  2374,
  -32768,   444,  1445,  1531,-32768,  3561,-32768,   444,     3,-32768,
  -32768,  1532,-32768,-32768,  1308,  1309,-32768,  4847,  4847,-32768,
    4847,  4847,-32768,  1579,   189,   507,  1314,-32768,-32768,  1315,
     507,  5758,-32768,-32768,  1530,-32768,   176,  1320,  1323,   507,
    1170,  1324,  1327,   507,  7072,  7072,  1241,  7072,  6415,  1328,
    1331,   507,  1334,  1335,   507,  7072,   507,  1338,  1341,   507,
    1342,  1345,   507,  1347,  1348,   507,-32768,  5423,-32768,   606,
     625,-32768,   654,-32768,   672,  7072,-32768,   712,   744,   794,
     857,   884,   893,  7072,  7072,  7072,   507,  1543,   315,   507,
    1422,  7072,-32768,  7072,  7072,  7072,  7072,-32768,-32768,-32768,
  -32768,  6727,-32768,-32768,  6727,  7389,-32768,-32768,  6727,  6727,
    6727,  6727,  6727,  6727,  1416,  1330,-32768,  1340,-32768,-32768,
    1416,-32768,   312,   312,    43,-32768,-32768,   801,-32768,  5642,
    5642,  1336,   787,  1351,  4986,  1482,-32768,  1482,   473,  1482,
    1535,  1540,  7072,   997,  1607,  1556,-32768,-32768,-32768,   722,
    1386,-32768,  1387,   663,  1177,  1500,  1177,-32768,  4847,   122,
     737,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2244,
    2244,-32768,  1512,  7072,  2244,   357,  1486,  1318,   795,-32768,
    4847,-32768,-32768,-32768,  1507,  1511,  1513,  1514,  1515,  1516,
  -32768,-32768,-32768,-32768,-32768,  1366,   460,-32768,  1448,-32768,
  -32768,   471,-32768,   484,   471,-32768,   471,-32768,  1402,-32768,
  -32768,  1368,-32768,-32768,   831,  1512,  1177,-32768,  1260,-32768,
  -32768,  1625,-32768,-32768,  1373,-32768,  1192,-32768,  1375,   748,
     145,  1376,-32768,-32768,   748,-32768,-32768,-32768,  1579,-32768,
    1579,  7072,  1014,  1014,  1378,   482,   815,  7072,  1585,-32768,
    1014,  1014,  7072,  1014,  1014,   212,   507,-32768,  1200,  7072,
     507,  1014,  1014,  1014,  1014,   507,  1014,  1014,  1014,  1014,
    1014,  1014,-32768,  1382,  1385,   673,  6727,-32768,-32768,  1384,
    3827,-32768,-32768,   930,-32768,-32768,-32768,-32768,-32768,-32768,
     968,  1186,-32768,-32768,  7072,  7072,  1599,  1491,   996,   818,
    1086,-32768,  3827,  1393,-32768,-32768,  1632,  1636,  5851,   333,
    1399,  1401,  1596,  7072,  1407,  1410,  6070,  6727,  6727,-32768,
  -32768,-32768,  1635,  7300,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   135,-32768,  1414,-32768,
  -32768,-32768,  1613,  1424,  1425,  1426,  1438,  1439,-32768,  1418,
     470,-32768,-32768,-32768,-32768,-32768,-32768,  1644,-32768,  5642,
    5642,-32768,-32768,  1542,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1708,-32768,   232,-32768,-32768,-32768,  1634,-32768,
    4986,  4986,   823,  6727,  7072,  4847,  1083,-32768,  1512,-32768,
  -32768,-32768,-32768,   798,-32768,-32768,   123,-32768,-32768,-32768,
  -32768,-32768,   507,-32768,  1697,   357,-32768,   796,  6727,-32768,
    1642,   138,  1547,  1649,  1548,  1680,  1650,  1689,  1386,-32768,
  -32768,  1457,-32768,  1458,  1645,  1697,   484,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1386,  1236,-32768,   798,  1552,
    1612,-32768,-32768,-32768,  1627,   370,-32768,-32768,  1402,  1630,
     507,  1465,  1466,-32768,   722,-32768,   507,  7072,  1467,  1468,
     507,  1469,  1470,  7072,  7072,   507,  1477,  1478,  1479,  1481,
    1485,  1488,  1492,  1494,  1495,  1496,-32768,  5423,  1483,-32768,
    6727,-32768,-32768,-32768,-32768,-32768,-32768,  7072,  1498,-32768,
  -32768,-32768,  7072,-32768,-32768,  7072,   130,-32768,  1696,-32768,
    6634,  6727,  7072,-32768,  4847,   507,-32768,-32768,-32768,  1487,
  -32768,-32768,-32768,  6727,-32768,   454,-32768,  1574,-32768,   459,
    1501,-32768,  4721,-32768,-32768,-32768,-32768,-32768,-32768,  1652,
  -32768,-32768,-32768,-32768,   312,  6727,  4986,-32768,   619,-32768,
  -32768,  1503,-32768,   507,   449,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1505,  6727,-32768,-32768,  1685,  1694,  6727,
    1318,-32768,-32768,   572,  6727,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1512,-32768,-32768,-32768,-32768,-32768,  1512,-32768,
  -32768,-32768,-32768,-32768,  7072,   748,-32768,  1580,-32768,   748,
  -32768,-32768,-32768,   507,-32768,-32768,-32768,-32768,   507,   507,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  6727,  1706,   507,-32768,-32768,  1063,  2244,  2244,-32768,
  -32768,  1517,-32768,  1118,  6853,  1162,   142,  1518,-32768,  1707,
  -32768,  6727,  6727,   460,   152,-32768,-32768,  1545,  1713,  4847,
  -32768,   751,-32768,  1509,-32768,-32768,  1520,-32768,-32768,  4847,
    1521,-32768,-32768,-32768,  7072,-32768,-32768,-32768,-32768,-32768,
    1318,   572,-32768,  1666,-32768,-32768,-32768,-32768,  1552,-32768,
     507,-32768,-32768,-32768,  1522,  1533,  1534,-32768,-32768,-32768,
  -32768,  1014,-32768,  1659,  4721,-32768,  1638,-32768,  4721,-32768,
  -32768,-32768,-32768,  7554,   454,  7072,  4721,   149,-32768,  4721,
    6508,  6727,-32768,   449,  6727,-32768,-32768,  1666,   271,  1318,
    1687,  1695,-32768,-32768,  1539,-32768,  6727,  6727,  1541,  4467,
    1546,   756,  7554,  1549,-32768,-32768,  1214,   117,  1772,-32768,
  -32768,   826,-32768,   835,  1318,   428,   428,-32768,  1779,-32768,
    1646,-32768,-32768,-32768,-32768,  1550,-32768,  4721,  1549,-32768,
  -32768,  1778,  4721,  1793,  1783,  7554,  6727,-32768,-32768,-32768,
    1631,   633,-32768,-32768,-32768,-32768,   840,  7554,   160,  6727,
    7554,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  7554,-32768,
  -32768,-32768,  1831,  1833,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1311,  1681,-32768,  1684,  1538,   591,-32768,
    1074,-32768,-32768,-32768,  -837,-32768,  -417,-32768,  -409,  1686,
    1551,  1322,  1081,-32768,-32768,-32768,   713,-32768,  1175,   947,
  -32768,-32768,   720,-32768,-32768,   946,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   731,
  -32768,-32768,   979,-32768,-32768,   297,-32768,-32768,  1490,  1782,
  -32768,-32768,  -349,-32768,-32768,-32768,  1473,-32768,-32768,  1240,
    -372,-32768,-32768,-32768,   803,-32768, -1287,-32768,-32768,  1519,
    1061,   739,  1497,  1041,   752,-32768,   318,   -55,  -254,-32768,
    -342,-32768, -1151,-32768,-32768,   344,-32768,  1504, -1118,-32768,
  -32768,-32768,-32768, -1232,   343,    46,    45,    44,    21,-32768,
  -32768,-32768,-32768,  -136,  -296,-32768,  1071,-32768,  -689,-32768,
   -1001,-32768,   885,-32768,-32768,-32768,-32768,-32768, -1319,  -949,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768, -1228,   139,-32768,  -859,-32768,-32768,
  -32768,-32768,-32768,   247,-32768,    71,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1144,-32768,-32768,  -654,   381,
    -364,  -880,   616,-32768,-32768,  -570,  -795,-32768,-32768, -1023,
   -1436,-32768,-32768,-32768,-32768,-32768,  1536,-32768,  1304,  1525,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1537,
  -32768,-32768,-32768,-32768,-32768,  -567,  -470,-32768,  1020,   571,
    -573,  -378,-32768,-32768,-32768,  1296, -1032,-32768,  -377,-32768,
  -32768,-32768,-32768,  1250,-32768,-32768,  1038,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1897,-32768,-32768,
  -32768,-32768,-32768,  1578,-32768,-32768,-32768,  1383,-32768,  1369,
  -32768,-32768,-32768,-32768,  1120,-32768,-32768,-32768,     2,-32768,
  -32768,-32768,  -481,   310,-32768,  -345,   705,-32768,  -217,   182,
  -32768,-32768,-32768,-32768,  1762,-32768,-32768,-32768,  1147, -1121,
  -32768,-32768,  -950, -1342,-32768,-32768,-32768,-32768,   905,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1841,   -27,   661,
    -143,-32768,-32768,   148,-32768,  -414,  -766,  1223,-32768,   326,
  -32768,   529,-32768,  -697,-32768,  -664,-32768,  1129,  -537,-32768,
  -32768,  -845,  1938,  1939,-32768,-32768,  1389,  1940,-32768,-32768,
  -32768,  -230,-32768,-32768,  1167, -1130,   927,-32768,  -104,  -690,
    -553,-32768,-32768,-32768,  -336,   -40,  -461,  -626,-32768,  -508,
  -32768,-32768,-32768,   373,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -495,-32768,   765,-32768,  1060, -1011,-32768,
    -427,-32768,   655,  -992,  -146,  -965,  -945,  -927,   -45,    -3,
     -35,-32768,   -74,  -710,  -505,  -142,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1195,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   721,-32768, -1083,-32768,-32768,   294,-32768,  -924,
     -18,  1082,   832,   264,  -708,  1352, -1179,-32768,   -64,  1858,
  -32768,    16,   -52, -1215,   504,     4,   -57,  -160,    -1,   -59,
    -269,-32768,   -80,   -51,    -9,-32768
};


#define	YYLAST		8178


static const short yytable[] =
{
     148,    82,    41,   245,   586,   220,   295,   169,   342,   175,
     806,   241,   313,   316,   257,   168,   314,   317,   285,   262,
     239,   282,   650,   654,   624,   276,   242,   748,   588,   830,
     513,   808,   225,  1046,   785,   735,   252,   769,   284,   997,
    1478,   280,   286,   615,   945,   255,   216,   218,   221,   224,
     169,   229,   232,  1292,  1236,  1436,   767,   244,   218,   232,
     252,   224,   223,   169,   229,  1001,   260,   148,   232,   981,
    1302,  1262,  1294,  1329,   250,  1539,   272,   231,   169,   275,
     218,   593,   272,   232,   221,   224,   168,   169,   229,   260,
     148,   232,   349,   224,  1048,   283,   970,   531,   281,  1295,
    1198,   898,   978,  1127,  1350,   907,   288,   858,   279,   665,
     301,   554,   303,  1066,   405,   525,   287,   733,   819,  1296,
     820,  1345,   590,  1495,   302,   350,   831,  1883,   881,  1147,
     923,   923,  1422,  1712,  1646,   318,  1228,  1297,  1426,   799,
    1298,   244,   169,   244,   347,  1081,   923,   148,  1529,  1530,
     923,  1051,  1513,  1533,   232,   412,   572,   923,   923,   343,
     739,   224,   169,   260,  1575,   740,  1069,   175,  1667,  1071,
    1131,  1250,   373,  1747,   345,   949,   619,   912,  1078,   324,
     102,   557,   920,   733,   299,  1201,   406,  1537,  1301,  1252,
     913,   971,   972,   923,    88,  1305,  1649,  1371,   739,  1823,
     401,    89,   811,   740,  1311,    90,    91,  1908,   781,   558,
     418,   762,   739,   886,    92,   789,   794,   740,   650,   654,
    1593,   739,  1378,   572,   318,    93,   740,  1221,  1222,   -80,
    1333,   360,   374,  1362,   841,   739,  1679,  1122,   787,  1101,
     740,   761,    63,   361,    14,   560,  1148,   924,   924,   501,
     624,   416,   505,   584,   543,  1316,   513,   330,  1478,   421,
      87,  -660,   803,   924,   319,  1363,   585,   924,  1641,  1642,
    1379,   768,   152,   540,   924,   924,   362,   407,   363,  1518,
     244,  1711,   879,   244,   790,   300,   366,    94,  1292,  1292,
     514,   148,   850,  1292,   763,   798,  1265,  1152,  1153,   103,
    1253,   555,   550,    95,   887,   375,  1563,  1294,  1294,  1266,
     924,   984,  1294,  1132,  1504,  1865,   221,  -700,    96,   994,
     169,   573,    97,   923,  1778,   312,  1650,   300,   851,   587,
     973,   367,  1267,    98,  1295,  1295,   501,  1067,   242,  1295,
     601,  1619,  1620,   605,   664,   733,  1268,  1218,   616,  1576,
     618,   264,    99,  1531,  1296,  1296,   674,   557,  1215,  1296,
     244,   673,  1843,   739,   669,  1033,  1223,   244,   740,   244,
    1109,   244,  1297,  1297,   606,  1298,  1298,  1297,  1085,  -232,
    1298,  1665,  1279,  1269,   671,   265,   882,   594,   620,  1306,
    1307,  1317,  1829,   224,   229,  1280,  1567,  1748,  1149,  1527,
    1680,   950,   169,   912,   221,   545,   662,   912,   682,   743,
     744,   100,  1299,  1300,   680,  1695,   739,  1537,  1128,  1816,
    1146,   740,  1500,  1501,  1770,  1226,  1858,  1824,   852,  1144,
     795,   739,  1313,  1145,  1517,  1829,   740,  1289,   368,  1355,
     924,   543,  1549,  1057,   739,   741,   742,   743,   744,   740,
    1062,  1866,   310,  1064,   737,   384,   369,   923,  1792,   741,
     742,   743,   744,  1760,  1759,  1150,  1076,  1716,   741,   742,
     743,   744,   859,  -660,  1631,  -232,   774,   555,   833,   783,
     758,   865,   741,   742,   743,   744,  1101,   877,  1424,   765,
     370,  1092,   355,   770,   513,  1270,  -232,   532,   169,   163,
     224,   514,  -232,  1314,  1854,   538,  -371,   169,    80,  1761,
     176,   169,  1058,   760,   745,  1767,   514,   668,   175,  1229,
    1762,  1367,  1368,   505,  1369,  1370,   889,   784,   385,  1138,
     739,   812,   356,  1881,  1775,   740,  -232,  1265,   544,  1745,
    1261,   860,  1431,  1709,  1498,  1890,  1718,  1702,   865,  1566,
    1266,  1344,  1256,   878,   244,   834,   832,  1174,  1836,  1019,
    1135,  1668,   907,   177,  1708,  1102,  1901,   590,  1045,  1676,
    1282,  1175,   861,  1267,   924,    80,  1356,   150,  1907,  1632,
     252,  1910,  1151,    81,  1891,   825,  1770,   244,  1257,  1911,
    1770,   825,   545,  1120,   312,   835,  1809,  1810,  1770,   164,
    1717,  1859,  1554,  1315,  1139,   998,   165,  1867,   180,   546,
     741,   742,   743,   744,    14,  1176,  1862,   178,  1552,  1763,
     739,  1770,   892,   893,  1269,   740,   235,   236,  1478,   739,
     242,  1892,  1889,  1318,   740,  1276,   233,   310,   557,   391,
      80,   151,  1038,  1177,   248,   940,   310,  1681,   739,  1770,
      81,  1682,  1683,   740,  1770,   866,    80,  1478,   267,   318,
     181,   244,  1526,   741,   742,   743,   744,   310,   941,  1319,
     776,  1902,   170,   392,   310,   393,  1904,   739,   741,   742,
     743,   744,   740,  1265,  1541,   982,  1277,  1710,   318,  1565,
    1478,   741,   742,   743,   744,   739,  1266,   394,  1020,   946,
     740,   856,  1478,   171,   232,  1478,   318,  1178,  1284,  1303,
    1304,  1584,   501,  1478,   501,  1038,  1006,   311,   172,  1267,
    1909,  1312,   395,   312,   942,   402,   315,  1905,   954,   952,
     986,    81,   312,  1268,  1376,   739,  1292,  1292,   244,  1722,
     740,  1779,  1108,   244,  1017,   244,   956,   244, -1008,   857,
    1675, -1011,  1608,  1796,   514,  1294,  1294,   269,   555,  1797,
     403,  1164,   169,   224,   242,  1056,   550,   739,  1044,   224,
    1269,  1540,   740,  1036,  1346,  1087,   760,   741,   742,   743,
     744,   501,  1295,  1295,   169,   378,   169,  1609,   379,  1041,
    1135,   -48,   277,   244,   318,   244,  1785,   104,  1429,  1610,
    1786,   310,  1296,  1296,   289,  1042,   264,  1083,  1086,  1165,
    1116,  1117,   244,  1119,   252,  1795,  1375,   739,  1098,  1091,
    1297,  1297,   740,  1298,  1298,   291,  1553,   310,  1037,   -48,
     380,  1562,  1799,  1166,   958,   105,   607,  1106,   244,   244,
     265,   -48,   752,  1614,   753,  1512,   608,   106,   292,   252,
     252,  1265,   252,   557,  1093,   153,   682,   296,  1780,  1410,
     297,   739,  1099,  1121,  1266,   -48,   740,   741,   742,   743,
     744,   107,   298,  1129,  1206,   309,   741,   742,   743,   744,
     739,   169,  1270,  1407,   756,   740,   757,  1267,   739,  1674,
     821,   169,   822,   740,  1520,   741,   742,   743,   744,   682,
    1143,  1268,  1408,  1334,  1521,   680,   304,   739,  1335,   843,
    1334,   844,   740,   305,  1687,  1338,   739,  1842,    84,    85,
    1844,   740,   158,   159,   741,   742,   743,   744,  1094,  1095,
     -48,  1411,   437,   438,   439,  1688,   108,   745,  1269,   983,
     -48,  1689,   741,   742,   743,   744,   109,   235,   236,  1412,
    1013,   159,  1014,   739,   325,  1851,  1548,   351,   740,  1853,
     447,   448,  1626,  1872,   341,   -48,  1038,  1857,   352,  1481,
     232,   110,  1484,   555,   354,  1690,   811,  1125,   903,  1126,
    1130,   358,   741,   742,   743,   744,  1569,   154,   399,  1414,
    1878,   739,   745,   409,  1230,  1227,   740,  -690,   514,   514,
    1231,   388,  1232,   947,  1125,   951,  1281,   781,  1756,   955,
     957,   959,   224,   387,   741,   742,   743,   744,  1896,   739,
    1326,  1415,  1327,  1898,   740,  1828,  1829,  1375,  1375,   739,
    1880,  1829,   650,   654,   740,  1375,  1375,   376,  1375,  1375,
     739,  1331,   390,  1332,  1341,   740,  1375,  1375,  1375,  1375,
    1270,  1375,  1375,  1375,  1375,  1375,  1375,  1357,  -690,   411,
     569,  -690,   570,  1361,   741,   742,   743,   744,  -690,   413,
    -690,  1416,  1660,  1662,  -690,   557,   745,   414,  1499,  -690,
    1661,  1663,  -690,   252,   418,   242,   739,   417,  1324,   505,
     745,   740,  1585,   745,   502,  1624,   252,  -690,   745,  1325,
    1669,  1886,   252,  1887,   506,  1324,  1494,  1494,  1497,   739,
     756,  -690,  1888,   509,   740,  1829,   244,  1906,   516,  -690,
     244,   517,   997,  -690,  1507,   520,  1509,   741,   742,   743,
     744,   521,  -690,   522,  1417,   741,   742,   743,   744,   526,
    1224,   739,  1523,  1827,  1525,   523,   740,   524,  -690,  1874,
    1875,   539,   541,  1833,   741,   742,   743,   744,   542,   551,
     561,  1418,  1485,   741,   742,   743,   744,   563,   565,    14,
    1419,   577,   579,   841,   578,  -690,   581,   739,   599,  1432,
     809,  1483,   740,   602,   612,   739,  1486,  -690,   169,  -690,
     740,   501,   617,   739,   242,   555,  1406,   666,   740, -1036,
     741,   742,   743,   744,  1568, -1036,   613,  1615,  1714,   739,
   -1036,   670,   672, -1036,   740,   712,   105,  1214,   707,   709,
     710,   719,   244,   739,  1382,  1437,  1480,   711,   740,   514,
     224,   229,   514,   260,   148,   713,  -690,   739,   741,   742,
     743,   744,   740,  1482,  1814,  1616,   720,  1617,   729, -1036,
     514,   514,   107,   738,  1594,   224,  -690, -1036,   714,   715,
    -690,   716,   717,   755,  -994, -1036,   741,   742,   743,   744,
     718,  -690,  -690,  1623,  -690,   721,   741,   742,   743,   744,
   -1037,  1863,   326, -1036,   726, -1036, -1037,   741,   742,   743,
     744, -1037,  1225,   557, -1037,   727,  1882,   728, -1036,   745,
   -1036, -1036, -1036,  1168,   746,  1172,   754,  1180,  1183,   751,
    1188,  1191,  1194,   747,  1574,   327,  1772,   108, -1038,  1579,
   -1011,   772,   778,  1551, -1038,   797,   780,   109,   793, -1038,
   -1037,   801, -1038,   741,   742,   743,   744,   802, -1037,   805,
    1808,   328,   804,   807,   329,  1798, -1037,   810,   468, -1036,
     572,   813,   110,  1670,   814,   816,   741,   742,   743,   744,
     330,  1625,   817,   823, -1037,   847, -1037,   849, -1038,   848,
    -431,   529,   854,   310,  1630,   862, -1038,   863,   869, -1037,
     537, -1037, -1037, -1037, -1038,   871,   331,   872,   741,   742,
     743,   744,  1812,   870,   874,   875,   880,   169,   884,   885,
     890,   891, -1038,   900, -1038,  1406,   903,   902,   908,   909,
     910,   914,   180,   555,   568,   974,   979, -1038,   571, -1038,
   -1038, -1038,   980,   881,   741,   742,   743,   744,  1002,   218,
   -1037,   312,   741,   742,   743,   744,  1815,  1640,  1143,  1143,
     741,   742,   743,   744,  1480,  1007,  1008,  1009,  1011, -1036,
   -1036, -1036, -1036, -1036, -1036, -1036,   741,   742,   743,   744,
    1015, -1036,   763, -1036,  1032,  1039,  1049,   756, -1038,  1053,
     741,   742,   743,   744,  1072,  1063,  1074,    65,  1772,  1073,
     514,   514,  1772,  1079,   741,   742,   743,   744,    66,   826,
    1772,  1080,  1773,  1860,  1082,    67,  1088,  1089,    54,  1090,
    1097,   224,   224,  1104,   343,  1777,  1114,  1123,    68,    55,
    1118,  1693,  1196,  1772,   723,  1137,    56,  1161,  1494,  1197,
     730,   731,   734,  1203,  1205,   739,  1213,  1233,  1235,   169,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037,  1251,  1254,  1259,
      57,  1772, -1037,  1260, -1037,  1264,  1772,   960,   961,   962,
     963,   964,   965,   966,   967,  1274,  1285,  1308,  1742,  1272,
    1288,  1310,  1320,  1328,  1343,    69,  1330,  1339, -1038, -1038,
   -1038, -1038, -1038, -1038, -1038,  1336,  1340,  1342,  1131,  1351,
   -1038,    70, -1038,  1359,  1364,  1365,  1366,   923,   775,  1793,
    1372,  1373,   777,  1377,  1423,  1425,  1380,    71,   169,  1381,
    1383,   221,    58,  1384,  1391,  1489,  1406,  1392,  1487,  1505,
    1393,  1394,    72,  1502,  1396,  1490,    73,  1397,  1398,    74,
    1510,  1399,   224,  1400,  1401,  1511,  1514,    75,  1503,  1515,
    1524,  1538,  1542,    59,  1143,  1754,  1543,    60,  1544,  1545,
    1546,  1547,  1794,  1550,  1101,  1564,    76,  1570,  1587,  1572,
    1621,  1768,  1573,  1578,  1773,  1583,   343,   224,  1773,  1606,
    1607,  1613,  1791,   557,  1622,   925,  1773,  1627,  1628,  1773,
    1801,   739, -1000,  1633,  1803,   986,   740,  1634,   151,   926,
     169,  1636,  1581,  1582,  1637,   221,  1644,  1837,  1651,  1773,
    1588,  1589,   557,  1591,  1592,  1652,  1658,  1819,  1653,  1654,
    1655,  1596,  1597,  1598,  1599,    77,  1600,  1601,  1602,  1603,
    1604,  1605,  1656,  1657,  1659,   927,  1664,  1773,   781,  1666,
    1684,  1694,  1773,   928,  1699,   557,    61,  1696,  1698,  1697,
    1700,   929,   221,  1701,  1703,  1704,  1713,   557,  1715,  1705,
     557,  1719,  1720,  1721,  1724,  1725,  1726,  1727,   557,   930,
     922,   931,   218,  1821,  1730,  1731,  1732,  1751,  1733,  1741,
    1822,  1758,  1734,  1645,   932,  1735,   933,   934,   935,  1736,
     948,  1737,  1738,  1739,   953,  1744,  1774,  1769,  1784,  1789,
    1790,   969,  1787,   555,  1802,  1806,   976,   977,  1838,  1818,
    1830,  1825,  1817,  1811,   505,  1826,  1831,  1834,   468,  1845,
     987,   988,   989,   990,   991,   992,   468,  1849,  1846,  1868,
    1847,  1852,   555,  1848,  1480,   936,  1873,  1870,  1876,  1885,
    1879,   244,  1036,  1865,  1829,   244,  1866,  1895,  1897,  1899,
    1900,  1913,  1903,  1914,   771,   508,   306,   221,   221,   307,
     537,  1491,   308,  1480,  1010,   555,  1018,   773,  1360,   901,
    1136,   515,  1358,  1707,  1141,  1348,   582,   555,  1110,   263,
     555,  1047,   864,  1287,  1349,  1050,   559,   568,   555,  1055,
     625,  1156,  1077,  1706,  1157,   595,  1480,  1036,   422,  1686,
    1692,  1354,   597,  1864,  1869,   423,  1871,  1894,  1480,  1237,
    1143,  1480,  1070,  1813,  1766,  1855,   999,   424,  1519,  1480,
    1677,   580,  1115,   425,   426,  1561,   846,   589,   603,   888,
     157,   552,  1107,  1040,   792,   937,   938,   939,   741,   742,
     743,   744,   867,   323,   788,   982,  1263,  1034,   273,  1508,
     943,   427,  1835,  1740,   428,  1612,  1052,  1016,    46,    47,
      50,  1387,   800,  1258,  1427,  1528,  1337,  1100,  1788,   278,
    1639,     0,   842,     0,   925,     0,     0,     0,     0,   429,
     739,   430,   431,     0,     0,   740,     0,     0,   926,     0,
       0,   432,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1154,  1155,  1160,     0,     0,
    1169,  1170,  1173,     0,  1181,  1184,  1186,  1189,  1192,  1195,
       0,     0,     0,     0,   927,     0,  1199,  1200,     0,  1202,
       0,  1204,   928,     0,  1207,  1208,  1209,  1210,  1211,  1212,
     929,     0,     0,     0,     0,     0,     0,     0,  1158,     0,
     434,  1216,     0,     0,  1219,     0,   435,     0,   930,     0,
     931,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,   932,     0,   933,   934,   935,     0,     0,
     437,   438,   439,   440,   235,   236,   112,     0,   113,   114,
     115,   116,     0,   117,   441,   468,     0,     0,     0,     0,
       0,   118,   119,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   120,   451,     0,   122,     0,     0,   452,   453,
       0,   454,   455,   456,   936,   125,   126,   457,   127,     0,
       0,   128,   129,   130,   131,   132,     0,     0,     0,   133,
     134,     0,     0,     0,     0,   135,   458,   137,     0,   459,
     139,   140,     0,     0,     0,   460,   461,     0,     0,     0,
       0,   528,     0,   463,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,  1167,     0,     0,  1157,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,     0,   425,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   937,   938,   939,   741,   742,   743,
     744,     0,     0,     0,     0,     0,   427,     0,     0,   428,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1385,  1386,     0,  1388,  1390,     0,
       0,     0,     0,     0,   429,  1395,   430,   431,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,     0,
     433,     0,     0,     0,     0,  1413,     0,     0,     0,     0,
       0,     0,     0,  1420,  1421,  1216,     0,     0,     0,     0,
       0,  1216,   425,  1219,  1428,   468,  1430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,  1158,     0,   434,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   468,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,   437,   438,   439,   440,   235,
     236,   112,     0,   113,   114,   115,   116,     0,   117,   441,
       0,     0,     0,  1532,     0,     0,   118,   119,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   120,   451,     0,
     122,     0,     0,   452,   453,     0,   454,   455,   456,     0,
     125,   126,   457,   127,     0,     0,   128,   129,   130,   131,
     132,   826,     0,     0,   133,   134,     0,     0,     0,   434,
     135,   458,   137,     0,   459,   139,   140,     0,     0,     0,
     460,   461,     0,     0,     0,   436,   528,     0,   463,     0,
       0,  1580,   312,     0,     0,     0,     0,  1586,     0,   437,
     438,   439,  1590,   235,   236,     0,     0,     0,     0,  1595,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,     0,     0,     0,     0,     0,     0,   452,   453,     0,
       0,     0,     0,     0,  1216,  1216,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1635,     0,  1171,     0,     0,  1157,     0,
       0,     0,   422,     0,     0,  1290,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,   428,     0,
       0,     0,   111,     0,  1673,   112,     0,   113,   114,   115,
     116,     0,   117,     0,     0,     0,     0,     0,     0,     0,
     118,   119,     0,   429,     0,   430,   431,     0,     0,     0,
       0,   120,   121,     0,   122,   432,     0,     0,     0,   433,
       0,   123,   124,     0,   125,   126,     0,   127,     0,     0,
     128,   129,   130,   131,   132,     0,     0,     0,   133,   134,
       0,     0,     0,     0,   135,   136,   137,     0,   138,   139,
     140,     0,     0,     0,     0,     0,     0,  1723,     0,     0,
     292,     0,     0,  1728,  1729,     0,     0,     0,     0,     0,
       0,     0,  1158,     0,   434,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,  1743,     0,     0,
     436,   594,  1216,     0,     0,  1746,     0,     0,     0,     0,
    1753,     0,  1755,     0,   437,   438,   439,   440,   235,   236,
     112,     0,   113,   114,   115,   116,     0,   117,   441,     0,
       0,     0,     0,     0,     0,   118,   119,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   120,   451,     0,   122,
       0,     0,   452,   453,     0,   454,   455,   456,     0,   125,
     126,   457,   127,     0,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     458,   137,     0,   459,   139,   140,     0,     0,     0,   460,
     461,     0,     0,  1179,  1800,   528,  1157,   463,     0,     0,
     422,   312,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,   425,   426,     0,     0,     0,
       0,     0,     0,     0,   468,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,  1673,     0,     0,     0,     0,   111,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,   429,     0,   430,   431,     0,     0,   118,   119,     0,
       0,     0,     0,   432,     0,     0,     0,   433,   120,   121,
       0,   122,     0,     0,     0,  1856,     0,     0,   123,   124,
       0,   125,   126,     0,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   136,   137,     0,   138,   139,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1158,     0,   434,     0,     0,     0,     0,   600,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   439,   440,   235,   236,   112,     0,
     113,   114,   115,   116,     0,   117,   441,     0,     0,     0,
       0,     0,     0,   118,   119,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   120,   451,     0,   122,     0,     0,
     452,   453,     0,   454,   455,   456,     0,   125,   126,   457,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   458,   137,
       0,   459,   139,   140,     0,     0,     0,   460,   461,  1182,
       0,     0,  1157,   528,     0,   463,   422,     0,     0,   312,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,   425,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   111,     0,     0,   112,     0,
     113,   114,   115,   116,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   118,   119,     0,     0,   429,     0,   430,
     431,     0,     0,     0,   120,   121,     0,   122,     0,   432,
       0,     0,     0,   433,   123,   124,     0,   125,   126,     0,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   136,   137,
       0,   138,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1158,     0,   434,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   995,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
     439,   440,   235,   236,   112,     0,   113,   114,   115,   116,
       0,   117,   441,     0,     0,     0,     0,     0,     0,   118,
     119,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     120,   451,     0,   122,     0,     0,   452,   453,     0,   454,
     455,   456,     0,   125,   126,   457,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   458,   137,     0,   459,   139,   140,
       0,     0,     0,   460,   461,  1187,     0,     0,  1157,   528,
       0,   463,   422,     0,     0,   312,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,   428,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,   429,     0,   430,   431,     0,     0,     0,
     120,   121,     0,   122,     0,   432,     0,     0,     0,   433,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,   996,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1158,     0,   434,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,   439,   440,   235,   236,
     112,     0,   113,   114,   115,   116,     0,   117,   441,     0,
       0,     0,     0,     0,     0,   118,   119,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   120,   451,     0,   122,
       0,     0,   452,   453,     0,   454,   455,   456,     0,   125,
     126,   457,   127,     0,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     458,   137,     0,   459,   139,   140,     0,     0,     0,   460,
     461,  1190,     0,     0,  1157,   528,     0,   463,   422,   904,
       0,   312,   627,     0,     0,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   905,     0,   424,     0,   628,
       0,     0,   629,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,   630,     0,     0,     0,     0,     0,   631,   632,
     633,     0,   634,   635,   636,     0,   637,     0,     0,   429,
       0,   430,   431,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   638,   433,   639,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   640,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   641,     0,     0,     0,     0,     0,  1158,     0,
     434,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,   642,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,   439,   440,   235,   236,   112,     0,   113,   114,
     115,   116,     0,   117,   441,     0,     0,     0,     0,   643,
     644,   118,   119,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   120,   451,     0,   122,   645,     0,   452,   453,
       0,   454,   455,   456,     0,   125,   126,   457,   127,     0,
       0,   128,   129,   130,   131,   132,     0,     0,     0,   133,
     134,     0,     0,     0,     0,   135,   458,   137,     0,   459,
     139,   140,     0,     0,     0,   460,   461,  1193,     0,     0,
    1157,   528,     0,   463,   422,   626,     0,   312,   627,     0,
       0,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,   628,     0,     0,   629,   425,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,   630,     0,
       0,     0,     0,     0,   631,   632,   633,     0,   634,   635,
     636,     0,   637,     0,     0,   429,     0,   430,   431,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,     0,
     638,   433,   639,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   641,     0,
       0,     0,     0,     0,  1158,     0,   434,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
     642,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,   439,   440,
     235,   236,   112,     0,   113,   114,   115,   116,     0,   117,
     441,     0,     0,     0,     0,   643,   644,   118,   119,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   120,   451,
       0,   122,   645,     0,   452,   453,     0,   454,   455,   456,
       0,   125,   126,   457,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   458,   137,     0,   459,   139,   140,   422,     0,
       0,   460,   461,     0,     0,   423,     0,   528,     0,   463,
       0,     0,     0,   312,    14,     0,     0,   424,     0,     0,
       0,     0,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   684,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,   428,     0,     0,   111,     0,     0,
     112,     0,   113,   114,   115,   116,     0,   117,     0,     0,
       0,     0,     0,     0,     0,   118,   119,     0,     0,   429,
       0,   430,   431,     0,     0,     0,   120,   121,     0,   122,
       0,   432,     0,     0,     0,   433,   123,   124,     0,   125,
     126,     0,   127,   915,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     136,   137,     0,   138,   139,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1021,     0,     0,     0,     0,
      14,     0,     0,     0,   686,     0,     0,     0,     0,     0,
     434,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,   439,   440,   235,   236,   112,     0,   113,   114,
     115,   116,     0,   117,   441,     0,     0,     0,     0,     0,
       0,   118,   119,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   120,   451,     0,   122,     0,     0,   452,   453,
       0,   454,   455,   456,     0,   125,   126,   457,   127,     0,
       0,   128,   129,   130,   131,   132,   916,   917,   918,   133,
     134,     0,     0,     0,     0,   135,   458,   137,     0,   459,
     139,   140,   422,     0,     0,   460,   461,     0,     0,   423,
       0,   690,     0,   463,     0,     0,     0,   312,     0,     0,
       0,   424,     0,     0,   683,     0,     0,   425,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,     0,   684,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,   428,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,   429,     0,   430,   431,     0,     0,     0,
     120,   121,     0,   122,     0,   432,     0,     0,     0,   433,
     123,   124,     0,   125,   126,     0,   127,   685,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   686,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,   439,   440,   235,   236,
     112,     0,   113,   114,   115,   116,     0,   117,   441,     0,
       0,     0,     0,     0,     0,   118,   119,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   120,   451,     0,   122,
       0,     0,   452,   453,     0,   454,   455,   456,     0,   125,
     126,   457,   127,     0,     0,   128,   129,   687,   688,   689,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     458,   137,     0,   459,   139,   140,   422,     0,     0,   460,
     461,     0,     0,   423,     0,   690,     0,   463,     0,     0,
       0,   312,     0,     0,     0,   424,     0,     0,     0,     0,
       0,   425,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     684,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,   428,     0,     0,   111,     0,     0,   112,     0,
     113,   114,   115,   116,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   118,   119,     0,     0,   429,     0,   430,
     431,     0,     0,     0,   120,   121,     0,   122,     0,   432,
       0,     0,     0,   433,   123,   124,     0,   125,   126,     0,
     127,   911,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   136,   137,
       0,   138,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1877,     0,   463,     0,     0,     0,     0,
       0,     0,   686,     0,     0,     0,     0,     0,   434,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
     439,   440,   235,   236,   112,     0,   113,   114,   115,   116,
       0,   117,   441,     0,     0,     0,     0,     0,     0,   118,
     119,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     120,   451,     0,   122,     0,     0,   452,   453,     0,   454,
     455,   456,     0,   125,   126,   457,   127,     0,     0,   128,
     129,   687,   688,   689,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   458,   137,     0,   459,   139,   140,
     422,     0,     0,   460,   461,     0,     0,   423,     0,   690,
       0,   463,     0,     0,     0,   312,     0,     0,     0,   424,
       0,     0,     0,     0,     0,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   684,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,   428,     0,     0,   111,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   118,   119,     0,
       0,   429,     0,   430,   431,     0,     0,     0,   120,   121,
       0,   122,     0,   432,     0,     0,     0,   433,   123,   124,
       0,   125,   126,     0,   127,   685,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   136,   137,     0,   138,   139,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,   686,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   439,   440,   235,   236,   112,     0,
     113,   114,   115,   116,     0,   117,   441,     0,     0,     0,
       0,     0,     0,   118,   119,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   120,   451,     0,   122,     0,     0,
     452,   453,     0,   454,   455,   456,     0,   125,   126,   457,
     127,     0,     0,   128,   129,   687,   688,   689,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   458,   137,
       0,   459,   139,   140,   422,     0,     0,   460,   461,     0,
       0,   423,     0,   690,     0,   463,     0,     0,     0,   312,
       0,     0,     0,   424,     0,     0,     0,     0,     0,   425,
     426,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   684,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,   112,     0,   113,
     114,   115,   116,     0,   117,   429,     0,   430,   431,     0,
       0,     0,   118,   119,     0,     0,     0,   432,     0,     0,
       0,   433,     0,   120,   121,     0,   122,     0,     0,   911,
       0,     0,     0,   123,   124,     0,   125,   126,     0,   127,
       0,     0,   128,   129,   130,   131,   132,     0,     0,   104,
     133,   134,     0,     0,     0,     0,   135,   136,   137,     0,
     138,   139,   140,     0,     0,     0,     0,     0,     0,     0,
     686,     0,  1021,     0,     0,     0,   434,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,   105,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,   437,   438,   439,   440,
     235,   236,   112,     0,   113,   114,   115,   116,     0,   117,
     441,     0,     0,   107,     0,     0,     0,   118,   119,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   120,   451,
       0,   122,     0,   422,   452,   453,     0,   454,   455,   456,
     423,   125,   126,   457,   127,     0,     0,   128,   129,   130,
     131,   132,   424,     0,     0,   133,   134,     0,   425,   426,
       0,   135,   458,   137,     0,   459,   139,   140,     0,     0,
       0,   460,   461,     0,     0,     0,     0,   690,   108,   463,
       0,     0,     0,   312,     0,     0,   427,     0,   109,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,   429,     0,   430,   431,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,     0,
     433,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     120,   121,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,   434,     0,   133,   134,     0,
       0,   435,     0,   135,   136,   137,     0,   138,   139,   140,
       0,   436,     0,     0,     0,     0,     0,     0,   940,     0,
       0,     0,     0,     0,     0,   437,   438,   439,   440,   235,
     236,   112,     0,   113,   114,   115,   116,     0,   117,   441,
       0,   941,     0,     0,     0,     0,   118,   119,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   120,   451,     0,
     122,     0,   422,   452,   453,     0,   454,   455,   456,   423,
     125,   126,   457,   127,     0,     0,   128,   129,   130,   131,
     132,   424,     0,     0,   133,   134,     0,   425,   426,     0,
     135,   458,   137,     0,   459,   139,   140,   942,     0,     0,
     460,   461,   533,     0,     0,     0,   528,     0,   463,     0,
       0,     0,   312,     0,     0,   427,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,   429,     0,   430,   431,     0,     0,   118,
     119,     0,     0,     0,     0,   432,     0,     0,     0,   433,
     120,   121,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,     0,     0,
     435,     0,     0,  1238,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,   722,     0,
       0,     0,     0,     0,   437,   438,   439,   440,   235,   236,
     112,     0,   113,   114,   115,   116,     0,   117,   441,     0,
       0,     0,     0,     0,     0,   118,   119,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   120,   451,     0,   122,
       0,   422,   452,   453,     0,   454,   455,   456,   423,   125,
     126,   457,   127,  1239,     0,   128,   129,   130,   131,   132,
     424,     0,     0,   133,   134,     0,   425,   426,     0,   135,
     458,   137,     0,   459,   139,   140,     0,     0,     0,   460,
     461,     0,     0,     0,     0,   528,     0,   463,  1240,     0,
       0,   312,     0,  1241,   427,     0,     0,   428,     0,  1242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,   112,     0,   113,   114,   115,  1243,     0,
     117,     0,   429,     0,   430,   431,     0,     0,   118,   119,
       0,     0,     0,     0,   432,     0,     0,     0,   433,   120,
     121,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,   125,   126,     0,   127,     0,     0,   128,   129,
     130,   131,   132,     0,     0,     0,   133,   134,     0,     0,
       0,     0,   135,   136,   137,     0,   138,   139,   140,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,   439,   440,   235,   236,   112,
       0,   113,   114,   115,   116,     0,   117,   441,     0,     0,
       0,     0,     0,     0,   118,   119,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   120,   451,     0,   122,     0,
     422,   452,   453,     0,   454,   455,   456,   423,   125,   126,
     457,   127,     0,     0,   128,   129,   130,   131,   132,   424,
       0,     0,   133,   134,     0,   425,   426,     0,   135,   458,
     137,     0,   459,   139,   140,     0,     0,     0,   460,   461,
       0,     0,     0,     0,   528,     0,   463,     0,     0,     0,
     312,     0,     0,   427,     0,     0,   428,     0,     0,   111,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   118,   119,     0,
       0,   429,     0,   430,   431,     0,     0,     0,   120,   121,
       0,   122,     0,   432,     0,     0,     0,   433,   123,   124,
       0,   125,   126,     0,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   136,   137,     0,   138,   139,   140,     0,     0,
       0,     0,     0,     0,     0,  1629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   439,   440,   235,   236,   112,     0,
     113,   114,   115,   116,     0,   117,   441,     0,     0,     0,
       0,     0,     0,   118,   119,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   120,   451,     0,   122,     0,   422,
     452,   453,     0,   454,   455,   456,   423,   125,   126,   457,
     127,     0,     0,   128,   129,   130,   131,   132,   424,     0,
       0,   133,   134,     0,   425,   426,     0,   135,   458,   137,
       0,   459,   139,   140,     0,     0,     0,   460,   461,     0,
       0,     0,     0,   528,   993,   463,     0,     0,     0,   312,
       0,     0,   427,     0,     0,   428,     0,     0,   111,     0,
       0,   112,     0,   113,   114,   115,   116,     0,   117,     0,
       0,     0,     0,     0,     0,     0,   118,   119,     0,     0,
     429,     0,   430,   431,     0,     0,     0,   120,   121,     0,
     122,     0,   432,     0,     0,     0,   433,   123,   124,     0,
     125,   126,     0,   127,     0,     0,   128,   129,   130,   131,
     132,     0,     0,     0,   133,   134,     0,     0,     0,     0,
     135,   136,   137,     0,   138,   139,   140,     0,     0,     0,
       0,     0,     0,     0,  1638,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1185,     0,
       0,   437,   438,   439,   440,   235,   236,   112,     0,   113,
     114,   115,   116,     0,   117,   441,     0,     0,     0,     0,
       0,     0,   118,   119,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   120,   451,     0,   122,     0,   422,   452,
     453,     0,   454,   455,   456,   423,   125,   126,   457,   127,
       0,     0,   128,   129,   130,   131,   132,   424,     0,     0,
     133,   134,     0,   425,   426,     0,   135,   458,   137,     0,
     459,   139,   140,     0,     0,     0,   460,   461,     0,     0,
       0,     0,   528,     0,   463,     0,     0,     0,   312,     0,
       0,   427,     0,     0,   428,    80,     0,   111,     0,     0,
     112,     0,   113,   114,   115,   116,     0,   117,     0,     0,
       0,     0,     0,     0,     0,   118,   119,     0,     0,   429,
       0,   430,   431,     0,     0,     0,   120,   121,     0,   122,
       0,   432,     0,     0,     0,   433,   123,   124,     0,   125,
     126,     0,   127,     0,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     136,   137,     0,   138,   139,   140,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1389,     0,     0,
     437,   438,   439,   440,   235,   236,   112,     0,   113,   114,
     115,   116,     0,   117,   441,     0,     0,     0,     0,     0,
       0,   118,   119,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   120,   451,     0,   122,     0,   422,   452,   453,
       0,   454,   455,   456,   423,   125,   126,   457,   127,     0,
       0,   128,   129,   130,   131,   132,   424,     0,     0,   133,
     134,     0,   425,   426,     0,   135,   458,   137,     0,   459,
     139,   140,     0,     0,     0,   460,   461,     0,     0,     0,
       0,   528,     0,   463,     0,     0,     0,   312,     0,     0,
     427,     0,     0,   428,     0,     0,   111,     0,     0,   112,
       0,   113,   114,   115,   116,     0,   117,     0,     0,     0,
       0,     0,     0,     0,   118,   119,     0,     0,   429,     0,
     430,   431,     0,     0,     0,   120,   121,     0,   122,     0,
     432,     0,     0,     0,   433,   123,   124,     0,   125,   126,
       0,   127,     0,     0,   128,   129,   130,   131,   132,     0,
       0,     0,   133,   134,     0,     0,     0,     0,   135,   136,
     137,     0,   138,   139,   140,     0,     0,     0,     0,     0,
     996,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,   439,   440,   235,   236,   112,     0,   113,   114,   115,
     116,     0,   117,   441,     0,     0,     0,     0,     0,     0,
     118,   119,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   120,   451,     0,   122,     0,   422,   452,   453,     0,
     454,   455,   456,   423,   125,   126,   457,   127,     0,     0,
     128,   129,   130,   131,   132,   424,     0,     0,   133,   134,
       0,   425,   426,     0,   135,   458,   137,     0,   459,   139,
     140,     0,     0,     0,   460,   461,     0,     0,  1752,     0,
     528,     0,   463,     0,     0,     0,   312,     0,     0,   427,
       0,     0,   428,     0,     0,   111,     0,     0,   112,     0,
     113,   114,   115,   116,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   118,   119,     0,     0,   429,     0,   430,
     431,     0,     0,     0,   120,   121,     0,   122,     0,   432,
       0,     0,     0,   433,   123,   124,     0,   125,   126,     0,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   136,   137,
       0,   138,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
     439,   440,   235,   236,   112,     0,   113,   114,   115,   116,
       0,   117,   441,     0,     0,     0,     0,     0,     0,   118,
     119,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     120,   451,     0,   122,     0,   422,   452,   453,     0,   454,
     455,   456,   423,   125,   126,   457,   127,     0,     0,   128,
     129,   130,   131,   132,   424,     0,     0,   133,   134,     0,
     425,   426,     0,   135,   458,   137,     0,   459,   139,   140,
       0,     0,     0,   460,   461,     0,     0,     0,     0,   462,
       0,   463,     0,     0,     0,   312,     0,     0,   427,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,   430,   431,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,   439,
     440,   235,   236,   112,     0,   113,   114,   115,   116,     0,
     117,   441,     0,     0,     0,     0,     0,     0,   118,   119,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   120,
     451,     0,   122,     0,     0,   452,   453,     0,   454,   455,
     456,     0,   125,   126,   457,   127,  1235,     0,   128,   129,
     130,   131,   132,     0,     0,     0,   133,   134,     0,     0,
       0,     0,   135,   458,   137,     0,   459,   139,   140,     0,
       0,     0,   460,   461,     6,     0,     0,     0,   528,     0,
     463,  -287,     0,     0,   312,  1438,  1439,     0,  1440,     0,
       0,     0,     0,     0,     0,     0,     1,     0,     0,     0,
       0,     0,     0,     0,  1441,     0,     0,     0,     0,     0,
    1442,     0,     0,     0,     2,     0,     0,     0,     0,     0,
       0,     0,     3,     0,     0,     0,     0,     0,     0,     4,
       0,     5,     0,     6,     0,  1235,     0,     0,     7,     0,
       0,     0,     0,     0,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,     0,     0,     6,     0,     0,     0,  1443,     0,    10,
    -289,     0,     0,     0,  1438,  1439,     0,  1440,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,  1441,     0,     0,     0,     0,     0,  1442,
    1444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,  1645,
       0,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,   112,    11,   113,   114,   115,   116,     0,   117,    12,
       0,     0,    13,     0,    14,    15,   118,   119,     0,     0,
       0,     0,     0,     0,     0,     0,  1443,  1445,   121,     0,
     122,     0,     0,     0,     0,     0,     0,   123,   124,    16,
     125,   126,     0,   127,    14,     0,   128,  1446,   130,   131,
     132,     0,     0,     0,   133,   134,  1447,  1448,  1449,  1444,
     135,   136,   137,     0,   138,   139,   140,     0,     0,    16,
    1235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,    17,     0,
     112,     0,   113,   114,   115,   116,     0,   117,     6,     0,
       0,     0,     0,    18,    19,   118,   119,     0,     0,  1438,
    1439,     0,  1440,     0,     0,     0,  1445,   121,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,  1441,   125,
     126,     0,   127,     0,  1442,   128,  1446,   130,   131,   132,
       0,     0,     0,   133,   134,  1447,  1448,  1449,     0,   135,
     136,   137,     0,   138,   139,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     626,     0,     0,   627,     0,     0,     0,     0,     0,   826,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     628,  1443,     0,   629,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1444,     0,     0,     0,     0,     0,
       0,     0,     0,   630,    16,     0,     0,     0,     0,   631,
     632,   633,     0,   634,   635,   636,     0,   637,     0,     0,
       0,     0,   440,     0,     0,   112,     0,   113,   114,   115,
     116,     0,   117,     0,     0,   638,     0,   639,     0,     0,
     118,   119,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1445,   121,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,   125,   126,   640,   127,     0,     0,
     128,  1446,   130,   131,   132,     0,     0,     0,   133,   134,
    1447,  1448,  1449,   641,   135,   136,   137,     0,   138,   139,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   642,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,   112,     0,   113,   114,   115,   116,     0,
     117,   626,     0,     0,   627,     0,     0,     0,   118,   119,
     643,   644,     0,     0,     0,     0,     0,     0,     0,   120,
     121,   628,   122,     0,   629,     0,     0,   645,     0,   123,
     124,     0,   125,   126,     0,   127,     0,     0,   128,   129,
     130,   131,   132,     0,     0,     0,   133,   134,     0,     0,
       0,     0,   135,   136,   137,     0,   138,   139,   140,     0,
       0,     0,     0,     0,   630,     0,     0,     0,     0,     0,
     631,   632,   633,     0,   634,   635,   636,     0,   637,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   638,     0,   639,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   642,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,   643,   644,     0,     0,     0,     0,     0,     0,     0,
     120,   121,     0,   122,     0,     0,     0,     0,   645,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
     111,     0,     0,   112,   184,   113,   114,   115,   116,   185,
     117,   186,   187,   188,   189,   190,   191,   192,   118,   119,
     193,   194,   195,   196,   197,     0,     0,   198,   199,   120,
     121,   200,   122,     0,     0,   201,   202,   203,   204,   123,
     124,   205,   125,   126,   206,   127,   207,   208,   128,   129,
     130,   131,   132,     0,     0,     0,   133,   134,   209,   210,
     211,   212,   135,   136,   137,   213,   138,   139,   140
};

static const short yycheck[] =
{
       9,     4,     0,    67,   368,    57,   110,    16,   168,    18,
     563,    66,   158,   159,    73,    16,   158,   159,    98,    76,
      65,    95,   400,   400,   396,    89,    66,   497,   370,   596,
     299,   568,    59,   799,   539,   462,    71,   518,    97,   747,
    1235,    93,    99,   392,   708,    72,    55,    56,    57,    58,
      59,    60,    61,  1064,  1003,  1234,   517,    66,    67,    68,
      95,    70,    58,    72,    73,   754,    75,    76,    77,   733,
    1071,  1021,  1064,  1105,    70,  1307,    85,    61,    87,    88,
      89,   377,    91,    92,    93,    94,    87,    96,    97,    98,
      99,   100,   172,   102,   802,    96,   722,   314,    94,  1064,
     945,   671,   728,   898,  1127,   678,   102,   612,    92,   405,
     150,   341,   152,    43,   250,    63,   100,   462,   588,  1064,
     590,  1122,   376,  1253,   151,   176,   596,    10,    21,     8,
       8,     8,  1215,  1569,  1453,     6,   981,  1064,  1221,   553,
    1064,   150,   151,   152,   171,   835,     8,   156,  1299,  1300,
       8,   805,  1273,  1304,   163,   259,    90,     8,     8,   168,
      23,   170,   171,   172,    19,    28,   820,   176,  1510,   823,
     133,  1008,   217,    43,   170,    46,    43,   685,   832,   163,
     141,   341,   690,   528,   174,    10,    14,  1305,  1068,    63,
     685,    50,    51,     8,    48,  1075,    61,     8,    23,    47,
     245,    55,   574,    28,  1084,    59,    60,    47,    10,   345,
     190,   192,    23,   185,    68,   117,    61,    28,   596,   596,
       8,    23,    46,    90,     6,    79,    28,    50,    51,   149,
    1110,     4,     3,   230,   598,    23,  1523,   891,   143,   196,
      28,   510,   193,    16,   155,   349,   125,   125,   125,   289,
     622,   269,   292,    30,    26,    44,   525,   162,  1453,   277,
      63,    63,   558,   125,    46,   262,    43,   125,  1447,  1448,
      94,   182,    18,   325,   125,   125,    49,   105,    51,  1280,
     289,  1568,   646,   292,   186,   275,     4,   141,  1299,  1300,
     299,   300,    44,  1304,   275,   275,    64,   923,   924,   260,
     174,   341,   329,   157,   276,    76,  1338,  1299,  1300,    77,
     125,   738,  1304,   276,  1264,    44,   325,    85,   172,   746,
     329,   255,   176,     8,  1666,   282,   191,   275,    80,   369,
     189,    49,   100,   187,  1299,  1300,   376,   267,   378,  1304,
     380,  1424,  1425,   383,   403,   690,   114,   973,   393,   204,
     395,    11,   206,  1302,  1299,  1300,   276,   517,   173,  1304,
     369,   413,  1798,    23,   409,   779,   189,   376,    28,   378,
     875,   380,  1299,  1300,   383,  1299,  1300,  1304,   848,    22,
    1304,  1502,  1046,   151,   411,    45,   279,    30,   255,  1079,
    1080,   180,   275,   402,   403,  1049,  1345,   267,   277,   277,
     277,   272,   411,   911,   413,   177,   402,   915,   417,   272,
     273,   265,  1066,  1067,   417,   277,    23,  1535,   899,   277,
     915,    28,  1259,  1260,  1652,   275,   277,   275,   180,   910,
     275,    23,  1086,   914,  1279,   275,    28,  1063,   156,  1129,
     125,    26,  1322,    98,    23,   270,   271,   272,   273,    28,
     814,   180,   196,   817,   463,     3,   174,     8,  1690,   270,
     271,   272,   273,     9,  1643,     8,   830,    97,   270,   271,
     272,   273,   614,   275,   141,   118,   527,   517,    62,   223,
     507,   169,   270,   271,   272,   273,   196,    21,   173,   516,
     208,   855,     6,   520,   763,   263,   139,   315,   507,    68,
     509,   510,   145,    80,  1823,   323,    22,   516,   196,    55,
     244,   520,   167,   509,   275,    56,   525,    90,   527,   280,
      66,  1147,  1148,   563,  1150,  1151,   662,   271,    76,    19,
      23,   576,    46,  1852,  1664,    28,   179,    64,   123,  1622,
    1021,   615,  1231,  1566,  1254,   117,  1578,  1548,   169,  1344,
      77,  1121,  1013,    87,   563,   139,   596,    17,  1790,   776,
     902,  1511,  1135,   274,  1565,   275,  1885,   821,   798,  1518,
    1051,    31,   617,   100,   125,   196,  1129,    18,  1897,   246,
     615,  1900,   125,   271,   156,   594,  1814,   596,  1015,  1908,
    1818,   600,   177,   889,   282,   179,  1747,  1748,  1826,   168,
     230,  1829,   118,   180,    94,   751,   175,  1839,   126,   194,
     270,   271,   272,   273,   155,    75,  1831,    61,  1328,   165,
      23,  1849,   667,   668,   151,    28,   199,   200,  1823,    23,
     670,   203,  1864,    44,    28,   143,   147,   196,   798,    98,
     196,    82,   784,   103,     7,    85,   196,  1527,    23,  1877,
     271,  1531,  1532,    28,  1882,   276,   196,  1852,   196,     6,
     178,   670,  1288,   270,   271,   272,   273,   196,   108,    80,
     277,  1886,   141,   132,   196,   134,    43,    23,   270,   271,
     272,   273,    28,    64,  1310,   277,   194,  1567,     6,  1343,
    1885,   270,   271,   272,   273,    23,    77,   156,   277,    46,
      28,   223,  1897,   172,   713,  1900,     6,   167,  1057,  1073,
    1074,  1375,   752,  1908,   754,   857,   756,   276,   187,   100,
    1899,  1085,   181,   282,   164,   141,   276,    94,    46,   713,
     739,   271,   282,   114,  1161,    23,  1747,  1748,   747,  1584,
      28,   122,   271,   752,   771,   754,    46,   756,   278,   271,
    1516,   281,    79,  1702,   763,  1747,  1748,    65,   798,  1708,
     176,    46,   771,   772,   804,   810,   793,    23,   795,   778,
     151,  1308,    28,   782,  1123,   849,   772,   270,   271,   272,
     273,   821,  1747,  1748,   793,     4,   795,   114,     7,   142,
    1132,     6,    65,   802,     6,   804,  1676,     6,  1225,   126,
    1680,   196,  1747,  1748,   276,   158,    11,   847,   848,    94,
     884,   885,   821,   887,   849,  1695,  1161,    23,   863,   854,
    1747,  1748,    28,  1747,  1748,   140,  1331,   196,   223,    44,
      49,  1336,  1712,   118,    46,    44,     5,   872,   847,   848,
      45,    56,   275,  1410,   277,  1272,    15,    56,   276,   884,
     885,    64,   887,  1013,   223,    70,   865,   122,   239,  1201,
     275,    23,   865,   890,    77,    80,    28,   270,   271,   272,
     273,    80,   122,   900,   277,   174,   270,   271,   272,   273,
      23,   890,   263,   277,   275,    28,   277,   100,    23,  1515,
     275,   900,   277,    28,   231,   270,   271,   272,   273,   908,
     909,   114,   277,   275,   241,   908,   124,    23,   280,   203,
     275,   205,    28,   124,   118,   280,    23,  1797,    59,    60,
    1800,    28,   231,   232,   270,   271,   272,   273,   130,   131,
     145,   277,   195,   196,   197,   139,   145,   275,   151,   277,
     155,   145,   270,   271,   272,   273,   155,   199,   200,   277,
     275,   232,   277,    23,    79,  1814,  1320,   180,    28,  1818,
     223,   224,  1432,  1843,   156,   180,  1108,  1826,   192,  1238,
     979,   180,  1241,  1013,    19,   179,  1348,   275,   275,   277,
     277,   259,   270,   271,   272,   273,  1350,   202,    10,   277,
    1849,    23,   275,    59,   277,   979,    28,     0,  1007,  1008,
     275,   162,   277,   709,   275,   711,   277,    10,  1634,   715,
     716,   717,  1021,   266,   270,   271,   272,   273,  1877,    23,
     275,   277,   277,  1882,    28,   274,   275,  1372,  1373,    23,
     274,   275,  1410,  1410,    28,  1380,  1381,   276,  1383,  1384,
      23,   275,   199,   277,  1118,    28,  1391,  1392,  1393,  1394,
     263,  1396,  1397,  1398,  1399,  1400,  1401,  1131,    61,    61,
     355,    64,   357,  1137,   270,   271,   272,   273,    71,    79,
      73,   277,  1489,  1490,    77,  1235,   275,   198,   277,    82,
    1489,  1490,    85,  1118,   190,  1125,    23,    78,  1097,  1129,
     275,    28,   277,   275,   149,   277,  1131,   100,   275,  1102,
     277,   275,  1137,   277,   172,  1114,  1252,  1253,  1254,    23,
     275,   114,   277,   141,    28,   275,  1125,   277,    82,   122,
    1129,   276,  1830,   126,  1267,    63,  1269,   270,   271,   272,
     273,    61,   135,    61,   277,   270,   271,   272,   273,   244,
     275,    23,  1284,  1769,  1286,   122,    28,   122,   151,  1846,
    1847,   174,   102,  1779,   270,   271,   272,   273,   188,   172,
     123,   277,  1242,   270,   271,   272,   273,   121,   243,   155,
     277,   254,   254,  1537,   255,   178,    30,    23,   207,  1234,
     174,  1240,    28,   275,   192,    23,  1243,   190,  1197,   192,
      28,  1231,   112,    23,  1234,  1235,  1197,    29,    28,    17,
     270,   271,   272,   273,  1346,    23,   267,   277,  1572,    23,
      28,   276,    10,    31,    28,   199,    44,    63,   135,   276,
     276,   199,  1231,    23,    54,  1234,  1235,   276,    28,  1238,
    1239,  1240,  1241,  1242,  1243,   276,   239,    23,   270,   271,
     272,   273,    28,  1239,    82,   277,   199,    61,    97,    67,
    1259,  1260,    80,   279,    54,  1264,   259,    75,   276,   276,
     263,   276,   276,    10,   276,    83,   270,   271,   272,   273,
     276,   274,   275,   277,   277,   276,   270,   271,   272,   273,
      17,  1834,    84,   101,   276,   103,    23,   270,   271,   272,
     273,    28,   275,  1453,    31,   276,    82,   276,   116,   275,
     118,   119,   120,   930,   276,   932,   276,   934,   935,   267,
     937,   938,   939,   281,  1359,   117,  1652,   145,    17,  1364,
     281,   141,    63,  1326,    23,   166,   275,   155,   275,    28,
      67,   267,    31,   270,   271,   272,   273,   281,    75,    10,
     277,   143,   276,   275,   146,  1709,    83,    90,   288,   167,
      90,    90,   180,  1513,    22,    22,   270,   271,   272,   273,
     162,   275,    43,    10,   101,   174,   103,   136,    67,   208,
     174,   311,   136,   196,  1438,    21,    75,    90,   185,   116,
     320,   118,   119,   120,    83,   138,   188,   276,   270,   271,
     272,   273,   274,   185,    91,   276,   279,  1406,   276,   276,
      61,    10,   101,   122,   103,  1406,   275,   149,   128,   121,
     276,   276,   126,  1453,   354,   189,    61,   116,   358,   118,
     119,   120,   277,    21,   270,   271,   272,   273,    42,  1438,
     167,   282,   270,   271,   272,   273,   274,  1446,  1447,  1448,
     270,   271,   272,   273,  1453,   174,   174,   202,    60,   267,
     268,   269,   270,   271,   272,   273,   270,   271,   272,   273,
     276,   279,   275,   281,   190,   186,    10,   275,   167,   240,
     270,   271,   272,   273,    19,   276,    43,    37,  1814,   276,
    1489,  1490,  1818,    86,   270,   271,   272,   273,    48,    27,
    1826,    86,  1652,  1829,    22,    55,   125,   125,    37,   125,
     156,  1510,  1511,   154,  1513,  1665,   156,    98,    68,    48,
     276,  1538,   108,  1849,   454,   276,    55,   276,  1664,   276,
     460,   461,   462,   277,   275,    23,    63,   184,    16,  1538,
     267,   268,   269,   270,   271,   272,   273,   124,   107,    63,
      79,  1877,   279,    63,   281,   275,  1882,   210,   211,   212,
     213,   214,   215,   216,   217,    71,    12,   239,  1610,   276,
     276,   276,    10,   160,    10,   125,   277,   277,   267,   268,
     269,   270,   271,   272,   273,   278,   277,   277,   133,   192,
     279,   141,   281,    52,    52,   277,   277,     8,   528,  1693,
     276,   276,   532,    63,    51,   173,   276,   157,  1607,   276,
     276,  1610,   141,   276,   276,   275,  1607,   276,   192,   127,
     276,   276,   172,   277,   276,   275,   176,   276,   276,   179,
      85,   276,  1631,   276,   276,    85,    19,   187,   277,    73,
     130,   145,   125,   172,  1643,  1631,   125,   176,   125,   125,
     125,   125,  1694,   277,   196,   277,   206,    22,    63,   276,
      51,  1649,   277,   277,  1814,   277,  1665,  1666,  1818,   277,
     275,   277,  1689,  1823,   173,    17,  1826,   274,    36,  1829,
    1715,    23,    36,   274,  1719,  1684,    28,   276,    82,    31,
    1689,   274,  1372,  1373,   274,  1694,    51,  1791,   274,  1849,
    1380,  1381,  1852,  1383,  1384,    82,   278,  1761,   274,   274,
     274,  1391,  1392,  1393,  1394,   265,  1396,  1397,  1398,  1399,
    1400,  1401,   274,   274,    70,    67,   174,  1877,    10,    85,
      23,    79,  1882,    75,    44,  1885,   265,   180,   180,    80,
      80,    83,  1741,    44,   277,   277,   124,  1897,   111,    94,
    1900,   111,   277,   277,   277,   277,   277,   277,  1908,   101,
     690,   103,  1761,  1762,   277,   277,   277,    61,   277,   276,
    1763,   274,   277,   189,   116,   277,   118,   119,   120,   277,
     710,   277,   277,   277,   714,   277,   124,   276,   275,    94,
      86,   721,   277,  1823,   204,    79,   726,   727,   122,    82,
     281,   246,   274,   276,  1834,    82,   276,   276,   738,   277,
     740,   741,   742,   743,   744,   745,   746,   148,   275,   122,
     276,   173,  1852,  1811,  1823,   167,   277,   122,   277,    47,
     274,  1830,  1831,    44,   275,  1834,   180,   277,    50,    36,
      47,     0,   201,     0,   523,   297,   155,  1846,  1847,   155,
     780,  1250,   156,  1852,   763,  1885,   772,   525,  1135,   674,
     903,   300,  1132,  1556,   908,  1124,   366,  1897,   879,    77,
    1900,   801,   622,  1060,  1125,   804,   347,   807,  1908,   809,
     397,     6,   831,  1555,     9,   378,  1885,  1886,    13,  1535,
    1537,  1129,   378,  1837,  1840,    20,  1841,  1866,  1897,  1004,
    1899,  1900,   821,  1754,  1647,  1824,   752,    32,  1282,  1908,
    1519,   364,   882,    38,    39,  1334,   602,   371,   383,   659,
      13,   333,   874,   793,   545,   267,   268,   269,   270,   271,
     272,   273,   626,   161,   541,   277,  1021,   780,    87,  1268,
     707,    66,  1784,  1607,    69,  1406,   807,   770,     0,     0,
       0,  1176,   553,  1016,  1223,  1290,  1114,   865,  1684,    91,
    1446,    -1,   600,    -1,    17,    -1,    -1,    -1,    -1,    94,
      23,    96,    97,    -1,    -1,    28,    -1,    -1,    31,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   925,   926,   927,    -1,    -1,
     930,   931,   932,    -1,   934,   935,   936,   937,   938,   939,
      -1,    -1,    -1,    -1,    67,    -1,   946,   947,    -1,   949,
      -1,   951,    75,    -1,   954,   955,   956,   957,   958,   959,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,   971,    -1,    -1,   974,    -1,   171,    -1,   101,    -1,
     103,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,   119,   120,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,  1015,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,   167,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,    -1,
      -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,   268,   269,   270,   271,   272,
     273,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,  1161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1174,  1175,    -1,  1177,  1178,    -1,
      -1,    -1,    -1,    -1,    94,  1185,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,  1205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1213,  1214,  1215,    -1,    -1,    -1,    -1,
      -1,  1221,    38,  1223,  1224,  1225,  1226,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,  1272,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,  1303,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    27,    -1,    -1,   254,   255,    -1,    -1,    -1,   165,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,    -1,    -1,   181,   276,    -1,   278,    -1,
      -1,  1371,   282,    -1,    -1,    -1,    -1,  1377,    -1,   195,
     196,   197,  1382,   199,   200,    -1,    -1,    -1,    -1,  1389,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,   219,   220,   221,   222,   223,   224,   225,
     226,    -1,    -1,    -1,    -1,    -1,    -1,   233,   234,    -1,
      -1,    -1,    -1,    -1,  1424,  1425,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1443,    -1,     6,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,   271,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,   198,    -1,  1514,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,   106,    -1,    -1,    -1,   110,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,  1587,    -1,    -1,
     276,    -1,    -1,  1593,  1594,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,  1617,    -1,    -1,
     181,    30,  1622,    -1,    -1,  1625,    -1,    -1,    -1,    -1,
    1630,    -1,  1632,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,    -1,     6,  1714,   276,     9,   278,    -1,    -1,
      13,   282,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1754,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1784,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,   228,
      -1,   230,    -1,    -1,    -1,  1825,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    30,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,     6,
      -1,    -1,     9,   276,    -1,   278,    13,    -1,    -1,   282,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,   270,   271,     6,    -1,    -1,     9,   276,
      -1,   278,    13,    -1,    -1,   282,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,    -1,    -1,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,
     271,     6,    -1,    -1,     9,   276,    -1,   278,    13,    18,
      -1,   282,    21,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    32,    -1,    38,
      -1,    -1,    41,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,   113,   110,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,   218,
     219,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,   235,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,   270,   271,     6,    -1,    -1,
       9,   276,    -1,   278,    13,    18,    -1,   282,    21,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    38,    -1,    -1,    41,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
     113,   110,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,   218,   219,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,   235,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    13,    -1,
      -1,   270,   271,    -1,    -1,    20,    -1,   276,    -1,   278,
      -1,    -1,    -1,   282,   155,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,   118,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    13,    -1,    -1,   270,   271,    -1,    -1,    20,
      -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
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
     261,   262,    -1,   264,   265,   266,    13,    -1,    -1,   270,
     271,    -1,    -1,    20,    -1,   276,    -1,   278,    -1,    -1,
      -1,   282,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
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
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   276,    -1,   278,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
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
      13,    -1,    -1,   270,   271,    -1,    -1,    20,    -1,   276,
      -1,   278,    -1,    -1,    -1,   282,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,   118,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
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
      -1,   264,   265,   266,    13,    -1,    -1,   270,   271,    -1,
      -1,    20,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    94,    -1,    96,    97,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   227,   228,    -1,   230,    -1,    -1,   118,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,     6,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,   276,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    80,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,    -1,    -1,    -1,   276,   145,   278,
      -1,    -1,    -1,   282,    -1,    -1,    66,    -1,   155,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,   165,    -1,   254,   255,    -1,
      -1,   171,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,   108,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,   164,    -1,    -1,
     270,   271,   272,    -1,    -1,    -1,   276,    -1,   278,    -1,
      -1,    -1,   282,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,   141,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,    -1,    -1,    -1,   276,    -1,   278,   176,    -1,
      -1,   282,    -1,   181,    66,    -1,    -1,    69,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,
     228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
      -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,
      -1,    -1,    -1,    -1,   276,    -1,   278,    -1,    -1,    -1,
     282,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    13,
     233,   234,    -1,   236,   237,   238,    20,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,
      -1,   254,   255,    -1,    38,    39,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
      -1,    -1,    -1,   276,   277,   278,    -1,    -1,    -1,   282,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
     234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
     254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,
      -1,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,
      -1,    66,    -1,    -1,    69,   196,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
     271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,    -1,
      -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,    -1,   274,    -1,
     276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,
     237,   238,    20,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,
      38,    39,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    -1,    -1,    -1,    -1,   276,
      -1,   278,    -1,    -1,    -1,   282,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    16,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    44,    -1,    -1,    -1,   276,    -1,
     278,    51,    -1,    -1,   282,    55,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    42,    -1,    44,    -1,    16,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    44,    -1,    -1,    -1,   137,    -1,    80,
      51,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    80,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,   143,   203,   204,   205,   206,    -1,   208,   150,
      -1,    -1,   153,    -1,   155,   156,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,   180,
     240,   241,    -1,   243,   155,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,   256,   257,   258,   170,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,   180,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   229,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    44,    -1,
      -1,    -1,    -1,   244,   245,   216,   217,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    74,   240,
     241,    -1,   243,    -1,    80,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,   256,   257,   258,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,   137,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,   180,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,   113,    -1,   115,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,   144,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,   161,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    18,    -1,    -1,    21,    -1,    -1,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,
     228,    38,   230,    -1,    41,    -1,    -1,   235,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
     198,    -1,    -1,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,    -1,    -1,   225,   226,   227,
     228,   229,   230,    -1,    -1,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266
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
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
    break;
case 28:
{ yyval = make_node (nod_all, (int) 0, NULL); }
    break;
case 29:
{ yyval = make_node (nod_all, (int) 0, NULL); }
    break;
case 30:
{ yyval = make_list (yyvsp[0]); }
    break;
case 32:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 33:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
    break;
case 34:
{ yyval = make_node (nod_select, (int) 0, NULL); }
    break;
case 35:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
    break;
case 36:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
    break;
case 37:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
    break;
case 38:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
    break;
case 39:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
    break;
case 40:
{ yyval = NULL; }
    break;
case 41:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;
case 42:
{ yyval = NULL; }
    break;
case 43:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
    break;
case 44:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
    break;
case 45:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
    break;
case 46:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					make_list(yyvsp[-2]), make_list(yyvsp[0]), NULL, yyvsp[-3]); }
    break;
case 47:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
    break;
case 48:
{ yyval = NULL; }
    break;
case 49:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;
case 50:
{ yyval = NULL; }
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
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
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
							  yyvsp[-2], yyvsp[0]); }
    break;
case 86:
{ yyval = NULL; }
    break;
case 87:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
    break;
case 88:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); }
    break;
case 89:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); }
    break;
case 91:
{ yyval = yyvsp[-1]; }
    break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
    break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
    break;
case 94:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); }
    break;
case 95:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); }
    break;
case 96:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
    break;
case 97:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); }
    break;
case 98:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); }
    break;
case 99:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 100:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 101:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 102:
{ yyval = yyvsp[0]; }
    break;
case 103:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 104:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
    break;
case 154:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
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
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
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
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 314:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 315:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;
case 316:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;
case 317:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 319:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 321:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 322:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;
case 323:
{ yyval = NULL; }
    break;
case 324:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
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
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;
case 337:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;
case 338:
{ yyval = NULL; }
    break;
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;
case 342:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 343:
{ yyval = NULL; }
    break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 346:
{ yyval = make_node (nod_on_error, (int) e_err_count,
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
case 356:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 357:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;
case 358:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;
case 359:
{ yyval = NULL; }
    break;
case 360:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 361:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 362:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 363:
{ yyval = NULL; }
    break;
case 365:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 366:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 367:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 368:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 369:
{ lex.beginning = lex_position(); }
    break;
case 370:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 371:
{ lex.beginning = lex.last_token; }
    break;
case 372:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
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
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 377:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 378:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 379:
{ yyval = NULL; }
    break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 399:
{ yyval = NULL; }
    break;
case 400:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
    break;
case 411:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 413:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 414:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 415:
{ yyval = yyvsp[0]; }
    break;
case 416:
{ yyval = yyvsp[0]; }
    break;
case 417:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 418:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 419:
{ yyval = yyvsp[0]; }
    break;
case 420:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 422:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 423:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 424:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 425:
{ yyval = yyvsp[0]; }
    break;
case 426:
{ yyval = yyvsp[0]; }
    break;
case 427:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 428:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 429:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 430:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 462:
{ yyval = NULL; }
    break;
case 463:
{ yyval = NULL; }
    break;
case 464:
{ yyval = NULL; }
    break;
case 465:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 466:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 467:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 468:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 469:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 470:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 471:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 472:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 473:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 474:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 475:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 477:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 478:
{ yyval = yyvsp[0]; }
    break;
case 479:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 480:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 481:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 482:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 483:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 485:
{ yyval = NULL; }
    break;
case 487:
{ yyval = NULL; }
    break;
case 488:
{ yyval = yyvsp[0]; }
    break;
case 489:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 490:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 491:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 492:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 494:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
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
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
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
				}
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
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 567:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 568:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
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
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 626:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 627:
{ yyval = NULL; }
    break;
case 628:
{ yyval = yyvsp[0]; }
    break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 630:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 631:
{ yyval = NULL; }
    break;
case 632:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 633:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 634:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 636:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 637:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 639:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 640:
{ lex.limit_clause = true; }
    break;
case 641:
{ lex.limit_clause = false; }
    break;
case 642:
{ lex.first_detection = true; }
    break;
case 643:
{ lex.first_detection = false; }
    break;
case 644:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 645:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 646:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 647:
{ yyval = 0; }
    break;
case 648:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 649:
{ yyval = yyvsp[-2]; }
    break;
case 650:
{ yyval = yyvsp[-1]; }
    break;
case 651:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 652:
{ yyval = yyvsp[-1]; }
    break;
case 653:
{ yyval = yyvsp[0]; }
    break;
case 654:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 655:
{ yyval = 0; }
    break;
case 656:
{ yyval = make_list (yyvsp[0]); }
    break;
case 657:
{ yyval = 0; }
    break;
case 659:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 661:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 664:
{ yyval = make_list (yyvsp[0]); }
    break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 671:
{ yyval = yyvsp[-1]; }
    break;
case 672:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 674:
{ yyval = NULL; }
    break;
case 675:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 676:
{ yyval = NULL; }
    break;
case 678:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 682:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 683:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 684:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 687:
{ yyval = yyvsp[0]; }
    break;
case 688:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 689:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 690:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 691:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 692:
{ yyval = NULL; }
    break;
case 694:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 695:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 696:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 697:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 698:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 699:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 703:
{ yyval = make_list (yyvsp[0]); }
    break;
case 704:
{ yyval = NULL; }
    break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 708:
{ yyval = yyvsp[0]; }
    break;
case 709:
{ yyval = NULL; }
    break;
case 710:
{ yyval = yyvsp[0]; }
    break;
case 711:
{ yyval = NULL; }
    break;
case 712:
{ yyval = yyvsp[0]; }
    break;
case 713:
{ yyval = NULL; }
    break;
case 714:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 715:
{ yyval = 0; }
    break;
case 716:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 717:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 718:
{ yyval = 0; }
    break;
case 719:
{ yyval = 0; }
    break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 722:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 726:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 727:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 728:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 731:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 732:
{ yyval = 0; }
    break;
case 733:
{ yyval = make_list (yyvsp[0]); }
    break;
case 734:
{ yyval = 0; }
    break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 737:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 738:
{ yyval = 0; }
    break;
case 739:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 740:
{ yyval = 0; }
    break;
case 741:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 742:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 743:
{ yyval = yyvsp[-1]; }
    break;
case 744:
{ yyval = 0; }
    break;
case 745:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 746:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 747:
{ yyval = NULL; }
    break;
case 748:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 749:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 752:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 753:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 754:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 757:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 758:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 762:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 763:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 764:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 765:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 766:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 770:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 771:
{ yyval = yyvsp[0]; }
    break;
case 773:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 776:
{ yyval = NULL; }
    break;
case 777:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 779:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 781:
{ yyval = NULL; }
    break;
case 782:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 784:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 787:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 788:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 798:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 801:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 803:
{ yyval = yyvsp[-1]; }
    break;
case 804:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 817:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 819:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 825:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 826:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 828:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 829:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 830:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 831:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 832:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 837:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 843:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 844:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 846:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 848:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 850:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 852:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 854:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 856:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 858:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 859:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 860:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 861:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 863:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 864:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 865:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 866:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 867:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 869:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 870:
{ yyval = yyvsp[-1]; }
    break;
case 881:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 882:
{ yyval = yyvsp[0]; }
    break;
case 883:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 884:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 885:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 886:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 887:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 888:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 889:
{ yyval = yyvsp[-1]; }
    break;
case 890:
{ yyval = yyvsp[-1]; }
    break;
case 894:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 895:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 896:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
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
			yyval = make_node (nod_current_date, 0, NULL);
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
    break;
case 901:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 902:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 904:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 906:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 908:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 909:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 910:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 911:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 913:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 914:
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
case 915:
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
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 917:
{ yyval = make_parameter (); }
    break;
case 918:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 919:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 920:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 921:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 922:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 923:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 924:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 925:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 926:
{ yyval = yyvsp[0]; }
    break;
case 927:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 929:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 930:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 931:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 932:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 933:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 935:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 936:
{ yyval = yyvsp[0];}
    break;
case 940:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 941:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 942:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 943:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 944:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 945:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 946:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 947:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 948:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 949:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 950:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 952:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 954:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 955:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 956:
{ yyval = yyvsp[0]; }
    break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 958:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 959:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 960:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 963:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 964:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 965:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 968:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 969:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 971:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 972:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 973:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 974:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 975:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 979:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 980:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 985:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 993:
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

		dsql_nod* old = node;
		node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
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

