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



#define	YYFINAL		1904
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
    1434,  1437,  1440,  1442,  1444,  1446,  1447,  1450,  1453,  1458,
    1463,  1469,  1470,  1472,  1475,  1478,  1483,  1487,  1490,  1493,
    1501,  1503,  1504,  1506,  1507,  1510,  1513,  1516,  1519,  1522,
    1525,  1528,  1531,  1534,  1538,  1541,  1544,  1547,  1550,  1552,
    1554,  1556,  1558,  1563,  1569,  1571,  1575,  1577,  1581,  1583,
    1586,  1588,  1590,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1613,  1618,  1625,  1631,  1635,  1636,  1639,  1642,
    1643,  1647,  1648,  1653,  1655,  1661,  1666,  1668,  1673,  1675,
    1678,  1681,  1683,  1685,  1687,  1690,  1693,  1696,  1699,  1700,
    1704,  1710,  1712,  1714,  1717,  1721,  1723,  1726,  1730,  1731,
    1733,  1735,  1737,  1743,  1749,  1756,  1758,  1760,  1762,  1765,
    1770,  1772,  1773,  1779,  1781,  1782,  1786,  1789,  1791,  1792,
    1795,  1796,  1798,  1799,  1803,  1805,  1806,  1808,  1811,  1813,
    1815,  1817,  1819,  1822,  1825,  1827,  1830,  1834,  1836,  1838,
    1842,  1846,  1848,  1851,  1855,  1857,  1860,  1861,  1864,  1866,
    1868,  1869,  1871,  1873,  1875,  1879,  1882,  1886,  1887,  1889,
    1893,  1898,  1902,  1906,  1907,  1910,  1911,  1914,  1915,  1919,
    1923,  1927,  1929,  1934,  1939,  1941,  1951,  1952,  1953,  1954,
    1955,  1959,  1962,  1964,  1965,  1969,  1975,  1979,  1982,  1988,
    1991,  1993,  1995,  1997,  1999,  2001,  2005,  2007,  2011,  2013,
    2014,  2017,  2019,  2023,  2025,  2027,  2029,  2031,  2035,  2042,
    2044,  2045,  2049,  2050,  2052,  2056,  2058,  2060,  2062,  2067,
    2073,  2079,  2081,  2083,  2086,  2091,  2096,  2099,  2103,  2104,
    2106,  2109,  2111,  2113,  2116,  2119,  2122,  2123,  2125,  2126,
    2130,  2131,  2133,  2137,  2139,  2142,  2143,  2146,  2147,  2150,
    2151,  2156,  2158,  2161,  2163,  2165,  2166,  2168,  2172,  2175,
    2177,  2179,  2182,  2184,  2189,  2193,  2195,  2199,  2204,  2205,
    2209,  2210,  2212,  2216,  2220,  2222,  2224,  2225,  2227,  2229,
    2234,  2235,  2238,  2243,  2244,  2253,  2259,  2261,  2263,  2271,
    2276,  2281,  2283,  2285,  2294,  2300,  2302,  2306,  2310,  2312,
    2320,  2328,  2334,  2338,  2339,  2341,  2343,  2345,  2348,  2349,
    2351,  2353,  2355,  2356,  2360,  2362,  2366,  2368,  2369,  2373,
    2375,  2379,  2381,  2385,  2389,  2391,  2393,  2397,  2399,  2402,
    2404,  2408,  2412,  2414,  2417,  2421,  2425,  2429,  2433,  2435,
    2439,  2442,  2444,  2446,  2448,  2450,  2452,  2454,  2456,  2458,
    2460,  2462,  2464,  2468,  2472,  2476,  2480,  2484,  2488,  2492,
    2496,  2503,  2510,  2517,  2524,  2531,  2538,  2545,  2552,  2559,
    2566,  2573,  2580,  2587,  2594,  2601,  2608,  2610,  2612,  2618,
    2625,  2631,  2638,  2642,  2647,  2653,  2660,  2664,  2669,  2673,
    2678,  2682,  2687,  2692,  2698,  2703,  2708,  2712,  2717,  2719,
    2721,  2723,  2725,  2727,  2729,  2731,  2735,  2739,  2741,  2743,
    2745,  2747,  2749,  2751,  2753,  2755,  2757,  2759,  2762,  2765,
    2769,  2773,  2777,  2781,  2785,  2789,  2793,  2797,  2799,  2801,
    2803,  2805,  2809,  2811,  2813,  2815,  2817,  2819,  2821,  2826,
    2828,  2832,  2834,  2837,  2839,  2841,  2843,  2845,  2847,  2849,
    2851,  2854,  2857,  2860,  2862,  2864,  2866,  2868,  2870,  2872,
    2874,  2876,  2878,  2880,  2883,  2885,  2888,  2890,  2892,  2894,
    2896,  2898,  2901,  2903,  2905,  2907,  2909,  2914,  2920,  2926,
    2932,  2938,  2944,  2950,  2956,  2962,  2968,  2974,  2976,  2983,
    2985,  2990,  2998,  3001,  3002,  3007,  3011,  3018,  3020,  3022,
    3029,  3038,  3045,  3047,  3049,  3054,  3061,  3066,  3072,  3076,
    3082,  3087,  3093,  3095,  3097,  3099,  3104,  3111,  3113,  3115,
    3117,  3119,  3121,  3123,  3125,  3127,  3129,  3130,  3132,  3133,
    3135,  3137,  3139,  3141,  3143,  3145,  3147,  3149,  3151,  3153,
    3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,  3173,
    3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,  3193,
    3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,  3213,
    3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229,  3231
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
     475,   454,     0,   473,     0,   717,     0,   207,     0,   208,
       0,   209,     0,   210,     0,   211,     0,   212,     0,   213,
       0,   214,     0,   215,     0,   218,     0,   219,     0,   220,
       0,   221,     0,   222,     0,   225,     0,   226,     0,   233,
       0,   234,     0,   242,     0,   244,     0,   256,     0,   257,
       0,   258,     0,   259,     0,   239,     0,   263,     0,     7,
       0,     7,   207,     0,   489,   453,     0,   628,   451,     0,
     628,     0,   205,     0,   203,     0,     0,   705,     3,     0,
     705,    76,     0,   704,   266,   192,   667,     0,   704,   266,
     192,   223,     0,   704,   266,   192,   271,   223,     0,     0,
     482,     0,   481,   482,     0,     4,   332,     0,     4,   255,
      90,   661,     0,    49,   255,    90,     0,    16,   254,     0,
      51,   254,     0,   712,   458,   484,   462,   453,   485,   454,
       0,   459,     0,     0,   463,     0,     0,    49,   487,     0,
      55,   701,     0,    79,   705,     0,   141,   708,     0,   172,
     711,     0,   176,   712,     0,   187,   715,     0,    60,   702,
       0,    48,   700,     0,    59,    65,   693,     0,   157,   665,
       0,   206,   709,     0,    68,   704,     0,   265,   704,     0,
     489,     0,   490,     0,   493,     0,   496,     0,   494,   279,
     491,   280,     0,   501,   279,   491,   280,   499,     0,   492,
       0,   491,   275,   492,     0,   667,     0,   667,   278,   667,
       0,   494,     0,   501,   499,     0,   500,     0,   505,     0,
     508,     0,   235,     0,   495,     0,   161,     0,    38,     0,
     218,     0,   219,     0,    81,     0,    92,     0,    18,   498,
     497,   499,     0,    18,   276,   666,   277,     0,    18,   276,
     666,   275,   662,   277,     0,    18,   276,   275,   662,   277,
       0,   154,   160,   666,     0,     0,   169,   662,     0,   169,
     694,     0,     0,    21,   156,   695,     0,     0,   504,   276,
     665,   277,     0,   504,     0,   504,   185,   276,   665,   277,
       0,   503,   276,   665,   277,     0,   503,     0,   502,   276,
     665,   277,     0,   183,     0,    21,   185,     0,    87,   185,
       0,    21,     0,    87,     0,   115,     0,   113,    21,     0,
     113,    87,     0,    95,   506,     0,   507,   506,     0,     0,
     276,   667,   277,     0,   276,   667,   275,   667,   277,     0,
      41,     0,    88,     0,    91,   509,     0,    93,    91,   509,
       0,   144,     0,    89,   138,     0,   276,   663,   277,     0,
       0,   523,     0,   511,     0,   540,     0,   156,    68,   704,
     174,   667,     0,   156,    68,   704,   174,   223,     0,   156,
      68,   704,   174,   271,   223,     0,   513,     0,   514,     0,
     516,     0,   244,   716,     0,   245,   244,   716,   515,     0,
     123,     0,     0,   153,   520,   174,   517,   716,     0,   244,
       0,     0,    25,   520,   521,     0,   153,   520,     0,   193,
       0,     0,   147,   522,     0,     0,   162,     0,     0,   156,
     175,   524,     0,   525,     0,     0,   526,     0,   525,   526,
       0,   527,     0,   528,     0,   529,     0,   533,     0,   143,
     123,     0,   143,   194,     0,   188,     0,   117,   188,     0,
      84,   102,   530,     0,   530,     0,   531,     0,   143,   177,
     532,     0,   143,    26,   532,     0,   162,     0,   162,   172,
       0,   162,   172,   166,     0,   186,     0,   117,   186,     0,
       0,   146,   536,     0,   158,     0,   142,     0,     0,   143,
       0,   194,     0,   537,     0,   536,   275,   537,     0,   539,
     538,     0,    61,   534,   535,     0,     0,   581,     0,   539,
     275,   581,     0,   156,   168,    79,   705,     0,   545,   542,
     544,     0,    61,   180,   543,     0,     0,   121,   623,     0,
       0,   192,   243,     0,     0,   548,   598,   604,     0,   548,
     598,   604,     0,   548,   598,   604,     0,   549,     0,   548,
     178,   691,   549,     0,   548,   178,     6,   549,     0,   550,
       0,   155,   555,   558,   559,   563,   588,   584,   587,   589,
       0,     0,     0,     0,     0,   556,   557,   552,     0,   556,
     552,     0,   557,     0,     0,   231,   668,   551,     0,   231,
     276,   650,   277,   551,     0,   231,   657,   551,     0,   232,
     668,     0,   232,   276,   552,   650,   277,     0,   232,   657,
       0,    46,     0,   690,     0,   560,     0,   272,     0,   561,
       0,   560,   275,   561,     0,   650,     0,   650,   562,   706,
       0,    10,     0,     0,    63,   564,     0,   565,     0,   564,
     275,   565,     0,   571,     0,   566,     0,   578,     0,   567,
       0,   276,   571,   277,     0,   276,   545,   277,   562,   568,
     569,     0,   710,     0,     0,   276,   570,   277,     0,     0,
     706,     0,   570,   275,   706,     0,   572,     0,   573,     0,
     574,     0,   565,   263,    85,   566,     0,   565,   114,   582,
      85,   566,     0,   565,   582,    85,   565,   575,     0,   576,
       0,   577,     0,   122,   630,     0,   239,   276,   623,   277,
       0,   708,   579,   562,   710,     0,   708,   579,     0,   276,
     653,   277,     0,     0,   581,     0,   711,   710,     0,   711,
       0,    77,     0,   100,   583,     0,   151,   583,     0,    64,
     583,     0,     0,   127,     0,     0,    71,    19,   585,     0,
       0,   586,     0,   585,   275,   586,     0,   650,     0,    73,
     630,     0,     0,   190,   630,     0,     0,   135,   590,     0,
       0,   591,   276,   592,   277,     0,    85,     0,   164,   108,
       0,   108,     0,   164,     0,     0,   593,     0,   593,   275,
     592,     0,   594,   595,     0,   590,     0,   711,     0,   711,
     594,     0,   114,     0,    79,   276,   596,   277,     0,   126,
     705,   597,     0,   705,     0,   705,   275,   596,     0,    79,
     276,   596,   277,     0,     0,   126,    19,   599,     0,     0,
     600,     0,   599,   275,   600,     0,   650,   601,   603,     0,
      11,     0,    45,     0,     0,   231,     0,   241,     0,   240,
     553,   602,   554,     0,     0,   259,   650,     0,   259,   650,
     174,   650,     0,     0,    80,    82,   581,   624,   182,   276,
     653,   277,     0,    80,    82,   581,   624,   545,     0,   607,
       0,   608,     0,    44,    63,   580,   588,   589,   598,   604,
       0,    44,    63,   580,   609,     0,   190,    35,   121,   699,
       0,   611,     0,   612,     0,   180,   580,   156,   613,   588,
     589,   598,   604,     0,   180,   580,   156,   613,   609,     0,
     614,     0,   613,   275,   614,     0,   629,   267,   650,     0,
     676,     0,   143,    18,   628,    63,   581,   617,   619,     0,
      80,    18,   628,    82,   581,   617,   619,     0,    60,    63,
     618,   174,   618,     0,    60,   174,   618,     0,     0,   498,
       0,   657,     0,   662,     0,   107,   620,     0,     0,   666,
       0,   657,     0,   622,     0,     0,   276,   623,   277,     0,
     628,     0,   623,   275,   628,     0,   625,     0,     0,   276,
     626,   277,     0,   629,     0,   626,   275,   629,     0,   628,
       0,   710,   281,   697,     0,   710,   281,   272,     0,   697,
       0,   628,     0,   710,   281,   697,     0,   646,     0,   118,
     646,     0,   632,     0,   630,   125,   630,     0,   630,     8,
     630,     0,   632,     0,   118,   646,     0,   631,   125,   630,
       0,   631,     8,   630,     0,   647,   125,   630,     0,   647,
       8,   630,     0,   633,     0,   276,   631,   277,     0,   118,
     632,     0,   634,     0,   637,     0,   638,     0,   639,     0,
     640,     0,   645,     0,   635,     0,   643,     0,   641,     0,
     642,     0,   644,     0,   650,   267,   650,     0,   650,   268,
     650,     0,   650,   269,   650,     0,   650,    67,   650,     0,
     650,   101,   650,     0,   650,   119,   650,     0,   650,   120,
     650,     0,   650,   116,   650,     0,   650,   267,     6,   276,
     546,   277,     0,   650,   268,     6,   276,   546,   277,     0,
     650,   269,     6,   276,   546,   277,     0,   650,    67,     6,
     276,   546,   277,     0,   650,   101,     6,   276,   546,   277,
       0,   650,   119,     6,   276,   546,   277,     0,   650,   120,
       6,   276,   546,   277,     0,   650,   116,     6,   276,   546,
     277,     0,   650,   267,   636,   276,   546,   277,     0,   650,
     268,   636,   276,   546,   277,     0,   650,   269,   636,   276,
     546,   277,     0,   650,    67,   636,   276,   546,   277,     0,
     650,   101,   636,   276,   546,   277,     0,   650,   119,   636,
     276,   546,   277,     0,   650,   120,   636,   276,   546,   277,
       0,   650,   116,   636,   276,   546,   277,     0,   163,     0,
       9,     0,   650,    83,    46,    63,   650,     0,   650,    83,
     118,    46,    63,   650,     0,   650,    17,   650,     8,   650,
       0,   650,   118,    17,   650,     8,   650,     0,   650,   103,
     650,     0,   650,   118,   103,   650,     0,   650,   103,   650,
      54,   650,     0,   650,   118,   103,   650,    54,   650,     0,
     650,    75,   648,     0,   650,   118,    75,   648,     0,   650,
      31,   650,     0,   650,   118,    31,   650,     0,   650,   167,
     650,     0,   650,   118,   167,   650,     0,   650,   167,   192,
     650,     0,   650,   118,   167,   192,   650,     0,    57,   276,
     545,   277,     0,   159,   276,   545,   277,     0,   650,    83,
      94,     0,   650,    83,   118,    94,     0,   248,     0,   249,
       0,   250,     0,   251,     0,   252,     0,   253,     0,   649,
       0,   276,   653,   277,     0,   276,   546,   277,     0,   627,
       0,   652,     0,   669,     0,   656,     0,   657,     0,   427,
       0,   677,     0,   678,     0,   688,     0,   676,     0,   271,
     650,     0,   270,   650,     0,   650,   270,   650,     0,   650,
      28,   650,     0,   650,    23,   696,     0,   650,   271,   650,
       0,   650,   272,   650,     0,   650,   273,   650,     0,   276,
     650,   277,     0,   276,   547,   277,     0,   658,     0,   659,
       0,   660,     0,    39,     0,   710,   281,    39,     0,    97,
       0,   651,     0,   692,     0,   220,     0,   221,     0,   222,
       0,   627,   279,   653,   280,     0,   650,     0,   653,   275,
     650,     0,   656,     0,   271,   655,     0,   197,     0,   196,
       0,   195,     0,   223,     0,   224,     0,   655,     0,   661,
       0,    38,   199,     0,   218,   199,     0,   219,   199,     0,
     282,     0,   181,     0,   225,     0,   226,     0,   233,     0,
     234,     0,    66,     0,   165,     0,   242,     0,   199,     0,
     200,   199,     0,   663,     0,   271,   664,     0,   196,     0,
     196,     0,   663,     0,   196,     0,   668,     0,   271,   668,
       0,   196,     0,   670,     0,   671,     0,   673,     0,    32,
     276,   272,   277,     0,    32,   276,   690,   650,   277,     0,
      32,   276,    46,   650,   277,     0,   171,   276,   690,   650,
     277,     0,   171,   276,    46,   650,   277,     0,    13,   276,
     690,   650,   277,     0,    13,   276,    46,   650,   277,     0,
     110,   276,   690,   650,   277,     0,   110,   276,    46,   650,
     277,     0,   106,   276,   690,   650,   277,     0,   106,   276,
      46,   650,   277,     0,   672,     0,   209,   276,   689,    63,
     650,   277,     0,   674,     0,    96,   276,   650,   277,     0,
     228,   276,   650,    63,   650,   675,   277,     0,    61,   650,
       0,     0,   693,   276,   653,   277,     0,   693,   276,   277,
       0,    20,   276,   650,    10,   372,   277,     0,   679,     0,
     680,     0,   237,   276,   650,   275,   650,   277,     0,   261,
     276,   630,   275,   650,   275,   650,   277,     0,   238,   276,
     650,   275,   653,   277,     0,   681,     0,   683,     0,   236,
     686,   682,    51,     0,   236,   686,   682,    50,   687,    51,
       0,   189,   685,   173,   687,     0,   682,   189,   685,   173,
     687,     0,   236,   684,    51,     0,   236,   684,    50,   687,
      51,     0,   189,   630,   173,   687,     0,   684,   189,   630,
     173,   687,     0,   650,     0,   650,     0,   650,     0,   264,
      97,    61,   704,     0,    69,   276,   704,   275,   650,   277,
       0,   210,     0,   211,     0,   212,     0,   213,     0,   214,
       0,   215,     0,   216,     0,   217,     0,     6,     0,     0,
      46,     0,     0,    94,     0,   198,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   717,     0,   717,     0,   717,     0,   717,
       0,   717,     0,   198,     0,   718,     0,   227,     0,   230,
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
    2102,  2103,  2104,  2105,  2106,  2107,  2108,  2109,  2112,  2114,
    2118,  2120,  2125,  2131,  2133,  2135,  2139,  2141,  2145,  2148,
    2151,  2159,  2165,  2166,  2170,  2184,  2186,  2188,  2190,  2204,
    2214,  2215,  2219,  2220,  2226,  2230,  2232,  2234,  2236,  2238,
    2240,  2242,  2244,  2246,  2248,  2250,  2252,  2254,  2261,  2262,
    2265,  2266,  2269,  2274,  2281,  2282,  2286,  2295,  2301,  2302,
    2305,  2306,  2307,  2308,  2325,  2330,  2335,  2354,  2371,  2378,
    2379,  2386,  2390,  2396,  2402,  2410,  2414,  2420,  2424,  2428,
    2434,  2438,  2445,  2451,  2457,  2465,  2470,  2475,  2482,  2483,
    2484,  2487,  2488,  2491,  2492,  2493,  2500,  2504,  2515,  2521,
    2575,  2637,  2638,  2645,  2658,  2663,  2668,  2675,  2677,  2684,
    2685,  2686,  2690,  2693,  2696,  2704,  2705,  2706,  2709,  2713,
    2717,  2719,  2723,  2727,  2728,  2731,  2735,  2739,  2740,  2743,
    2745,  2749,  2750,  2754,  2758,  2759,  2763,  2764,  2768,  2769,
    2770,  2771,  2774,  2776,  2780,  2782,  2786,  2788,  2791,  2793,
    2795,  2799,  2801,  2803,  2807,  2809,  2811,  2815,  2819,  2821,
    2823,  2827,  2829,  2833,  2834,  2838,  2842,  2844,  2848,  2849,
    2854,  2862,  2866,  2868,  2872,  2874,  2878,  2880,  2887,  2891,
    2896,  2901,  2903,  2905,  2909,  2912,  2924,  2928,  2932,  2936,
    2940,  2942,  2944,  2946,  2950,  2952,  2954,  2958,  2960,  2962,
    2966,  2968,  2972,  2974,  2978,  2979,  2983,  2984,  2988,  2989,
    2994,  2998,  2999,  3003,  3004,  3007,  3008,  3009,  3014,  3019,
    3020,  3024,  3026,  3030,  3031,  3035,  3036,  3037,  3040,  3045,
    3050,  3054,  3055,  3058,  3062,  3066,  3069,  3074,  3076,  3080,
    3081,  3086,  3091,  3093,  3095,  3097,  3099,  3103,  3104,  3110,
    3112,  3116,  3117,  3123,  3126,  3128,  3132,  3134,  3141,  3143,
    3147,  3151,  3153,  3155,  3161,  3163,  3167,  3168,  3172,  3174,
    3177,  3178,  3182,  3184,  3186,  3190,  3191,  3195,  3197,  3203,
    3205,  3209,  3210,  3214,  3218,  3220,  3222,  3226,  3228,  3232,
    3234,  3240,  3243,  3251,  3258,  3261,  3268,  3269,  3272,  3276,
    3280,  3287,  3288,  3291,  3296,  3301,  3302,  3306,  3310,  3317,
    3319,  3323,  3325,  3327,  3330,  3331,  3334,  3338,  3340,  3343,
    3345,  3351,  3352,  3356,  3360,  3361,  3366,  3367,  3371,  3375,
    3376,  3381,  3382,  3385,  3390,  3395,  3398,  3405,  3406,  3408,
    3409,  3411,  3415,  3416,  3418,  3420,  3424,  3426,  3430,  3431,
    3433,  3437,  3438,  3439,  3440,  3441,  3442,  3443,  3444,  3445,
    3446,  3447,  3452,  3454,  3456,  3458,  3460,  3462,  3464,  3466,
    3472,  3474,  3476,  3478,  3480,  3482,  3484,  3486,  3488,  3490,
    3492,  3494,  3496,  3498,  3500,  3502,  3506,  3507,  3513,  3515,
    3519,  3521,  3526,  3528,  3530,  3532,  3537,  3539,  3543,  3545,
    3549,  3551,  3553,  3555,  3559,  3563,  3567,  3569,  3573,  3578,
    3583,  3590,  3595,  3600,  3609,  3610,  3614,  3621,  3622,  3623,
    3624,  3625,  3626,  3627,  3628,  3629,  3630,  3631,  3633,  3635,
    3642,  3644,  3646,  3653,  3660,  3667,  3669,  3671,  3672,  3673,
    3674,  3676,  3678,  3682,  3683,  3686,  3702,  3718,  3722,  3726,
    3727,  3731,  3732,  3736,  3738,  3740,  3742,  3744,  3748,  3749,
    3751,  3767,  3783,  3787,  3791,  3793,  3797,  3801,  3804,  3807,
    3810,  3813,  3818,  3820,  3825,  3826,  3830,  3837,  3844,  3851,
    3858,  3859,  3863,  3870,  3871,  3872,  3875,  3877,  3879,  3882,
    3889,  3898,  3905,  3914,  3916,  3918,  3920,  3924,  3927,  3931,
    3932,  3936,  3945,  3947,  3951,  3953,  3957,  3963,  3964,  3967,
    3971,  3974,  3978,  3979,  3982,  3984,  3988,  3990,  3994,  3996,
    4000,  4002,  4006,  4009,  4012,  4017,  4020,  4030,  4032,  4034,
    4036,  4038,  4040,  4042,  4044,  4048,  4049,  4052,  4053,  4056,
    4064,  4067,  4070,  4073,  4076,  4079,  4082,  4085,  4088,  4091,
    4094,  4097,  4100,  4103,  4106,  4109,  4112,  4115,  4118,  4121,
    4124,  4127,  4130,  4133,  4138,  4139,  4144,  4146,  4147,  4148,
    4149,  4150,  4151,  4152,  4153,  4154,  4155,  4156,  4157,  4159,
    4160,  4161,  4162,  4163,  4164,  4165,  4166,  4167,  4168
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
     473,   473,   473,   473,   473,   473,   473,   473,   474,   474,
     475,   475,   476,   477,   477,   477,   478,   478,   479,   479,
     479,   480,   481,   481,   482,   482,   482,   482,   482,   483,
     484,   484,   485,   485,   486,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   488,   488,
     489,   489,   490,   490,   491,   491,   492,   492,   493,   493,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   495,
     495,   496,   496,   496,   496,   497,   497,   498,   498,   498,
     499,   499,   500,   500,   500,   501,   501,   501,   502,   502,
     502,   503,   503,   504,   504,   504,   505,   505,   506,   506,
     506,   507,   507,   508,   508,   508,   508,   509,   509,   510,
     510,   510,   511,   511,   511,   512,   512,   512,   513,   514,
     515,   515,   516,   517,   517,   518,   519,   520,   520,   521,
     521,   522,   522,   523,   524,   524,   525,   525,   526,   526,
     526,   526,   527,   527,   528,   528,   529,   529,   530,   530,
     530,   531,   531,   531,   532,   532,   532,   533,   534,   534,
     534,   535,   535,   536,   536,   537,   538,   538,   539,   539,
     540,   541,   542,   542,   543,   543,   544,   544,   545,   546,
     547,   548,   548,   548,   549,   550,   551,   552,   553,   554,
     555,   555,   555,   555,   556,   556,   556,   557,   557,   557,
     558,   558,   559,   559,   560,   560,   561,   561,   562,   562,
     563,   564,   564,   565,   565,   566,   566,   566,   567,   568,
     568,   569,   569,   570,   570,   571,   571,   571,   572,   573,
     574,   575,   575,   576,   577,   578,   578,   579,   579,   580,
     580,   581,   582,   582,   582,   582,   582,   583,   583,   584,
     584,   585,   585,   586,   587,   587,   588,   588,   589,   589,
     590,   591,   591,   591,   591,   591,   592,   592,   593,   593,
     594,   594,   595,   595,   595,   596,   596,   597,   597,   598,
     598,   599,   599,   600,   601,   601,   601,   602,   602,   603,
     603,   604,   604,   604,   605,   605,   606,   606,   607,   608,
     609,   610,   610,   611,   612,   613,   613,   614,   615,   616,
     616,   617,   617,   617,   618,   618,   498,   619,   619,   620,
     620,   621,   621,   622,   623,   623,   624,   624,   625,   626,
     626,   627,   627,   627,   628,   629,   629,   630,   630,   630,
     630,   630,   631,   631,   631,   631,   631,   631,   632,   632,
     632,   633,   633,   633,   633,   633,   633,   633,   633,   633,
     633,   633,   634,   634,   634,   634,   634,   634,   634,   634,
     635,   635,   635,   635,   635,   635,   635,   635,   635,   635,
     635,   635,   635,   635,   635,   635,   636,   636,   637,   637,
     638,   638,   639,   639,   639,   639,   640,   640,   641,   641,
     642,   642,   642,   642,   643,   644,   645,   645,   646,   646,
     646,   647,   647,   647,   648,   648,   649,   650,   650,   650,
     650,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   650,   650,   650,   651,   651,   651,   652,   653,
     653,   654,   654,   655,   655,   655,   655,   655,   656,   656,
     656,   656,   656,   657,   658,   658,   659,   660,   660,   660,
     660,   660,   661,   661,   662,   662,   663,   664,   665,   666,
     667,   667,   668,   669,   669,   669,   670,   670,   670,   670,
     670,   670,   670,   670,   670,   670,   670,   671,   672,   673,
     673,   674,   675,   675,   676,   676,   677,   678,   678,   679,
     679,   679,   680,   680,   681,   681,   682,   682,   683,   683,
     684,   684,   685,   686,   687,   688,   688,   689,   689,   689,
     689,   689,   689,   689,   689,   690,   690,   691,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   714,   715,   716,   717,   717,   718,   718,   718,   718,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     0,     2,     2,     4,     4,
       5,     0,     1,     2,     2,     4,     3,     2,     2,     7,
       1,     0,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     2,     2,     2,     1,     1,
       1,     1,     4,     5,     1,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     6,     5,     3,     0,     2,     2,     0,
       3,     0,     4,     1,     5,     4,     1,     4,     1,     2,
       2,     1,     1,     1,     2,     2,     2,     2,     0,     3,
       5,     1,     1,     2,     3,     1,     2,     3,     0,     1,
       1,     1,     5,     5,     6,     1,     1,     1,     2,     4,
       1,     0,     5,     1,     0,     3,     2,     1,     0,     2,
       0,     1,     0,     3,     1,     0,     1,     2,     1,     1,
       1,     1,     2,     2,     1,     2,     3,     1,     1,     3,
       3,     1,     2,     3,     1,     2,     0,     2,     1,     1,
       0,     1,     1,     1,     3,     2,     3,     0,     1,     3,
       4,     3,     3,     0,     2,     0,     2,     0,     3,     3,
       3,     1,     4,     4,     1,     9,     0,     0,     0,     0,
       3,     2,     1,     0,     3,     5,     3,     2,     5,     2,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     0,
       2,     1,     3,     1,     1,     1,     1,     3,     6,     1,
       0,     3,     0,     1,     3,     1,     1,     1,     4,     5,
       5,     1,     1,     2,     4,     4,     2,     3,     0,     1,
       2,     1,     1,     2,     2,     2,     0,     1,     0,     3,
       0,     1,     3,     1,     2,     0,     2,     0,     2,     0,
       4,     1,     2,     1,     1,     0,     1,     3,     2,     1,
       1,     2,     1,     4,     3,     1,     3,     4,     0,     3,
       0,     1,     3,     3,     1,     1,     0,     1,     1,     4,
       0,     2,     4,     0,     8,     5,     1,     1,     7,     4,
       4,     1,     1,     8,     5,     1,     3,     3,     1,     7,
       7,     5,     3,     0,     1,     1,     1,     2,     0,     1,
       1,     1,     0,     3,     1,     3,     1,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     2,     1,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     1,     1,     5,     6,
       5,     6,     3,     4,     5,     6,     3,     4,     3,     4,
       3,     4,     4,     5,     4,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     4,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     1,     6,     1,
       4,     7,     2,     0,     4,     3,     6,     1,     1,     6,
       8,     6,     1,     1,     4,     6,     4,     5,     3,     5,
       4,     5,     1,     1,     1,     4,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     1,
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
       0,     0,   578,   123,     0,     0,     0,     0,     0,     0,
       0,     0,    50,   578,   643,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   560,    18,   565,   566,   567,     5,    17,   559,
     561,    19,   623,   730,   631,   634,    11,     8,   746,   747,
      21,   751,   752,     4,   471,     0,     0,     0,     0,     0,
       0,     0,   401,   577,   580,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   122,     0,     0,     0,   102,   736,
     926,     0,    22,   924,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   484,     0,   363,    28,    36,    33,    35,   772,    34,
     772,  1014,  1034,  1035,  1016,  1018,  1017,  1023,  1019,  1024,
    1020,  1022,  1025,  1021,  1026,  1027,  1028,  1030,  1031,  1029,
    1032,  1033,  1037,  1036,  1038,     0,    30,     0,    31,     0,
      66,    68,  1006,  1015,     0,     0,     0,     0,     0,     0,
       0,   576,     0,     0,   986,   637,   642,     0,     0,   585,
       0,   689,   691,  1008,     0,     0,     0,   115,   568,  1013,
       0,     2,     0,   627,     0,   988,   743,     0,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   456,   450,   451,   452,
     453,   454,   455,   457,     0,   430,   431,     0,   998,   408,
       0,  1002,   405,   259,  1005,     0,   691,   404,   379,  1009,
     409,     0,  1001,   582,   575,   922,     0,   111,   159,   158,
     112,   141,   208,   784,   994,     0,   109,   153,     0,   105,
     259,   113,   928,   130,   106,   193,   107,     0,   108,   772,
    1012,   114,   154,   110,   734,   735,     0,   927,   925,     0,
      74,     0,   999,   707,   492,   997,   485,     0,   491,   496,
     486,   487,   494,   488,   489,   490,   495,   497,   328,     0,
     261,    29,     0,    38,   771,    37,    27,     0,     0,     0,
       0,     0,   777,     0,     0,     0,     0,     0,     0,   574,
     932,     0,   913,   636,   636,   637,   649,   647,   985,   650,
       0,   651,   641,   637,     0,     0,     0,     0,     0,     0,
     601,   594,   583,   584,   586,   588,   589,   590,   597,   598,
     591,     0,   690,  1007,   116,   259,   117,   193,   118,   772,
     571,   625,     0,   621,     0,     0,   987,     0,     0,   628,
       0,     0,     0,     0,   406,   472,   371,     0,   371,     0,
     211,   407,   412,   402,   466,   467,     0,   261,   232,   458,
       0,   403,   421,     0,   377,   378,   481,     0,   581,   579,
     923,   156,   156,     0,     0,     0,   168,   160,   161,   140,
       0,   103,     0,     0,   119,   261,   129,   128,   131,     0,
       0,     0,     0,     0,   990,    75,   211,     0,     0,   709,
     749,   493,     0,     0,     0,     0,   890,   919,     0,   989,
       0,   892,     0,     0,   920,     0,   914,   905,   904,   903,
    1014,     0,     0,     0,   895,   896,   897,   906,   907,   915,
     916,  1018,   917,   918,     0,  1023,  1019,   921,  1032,  1037,
       0,     0,     0,     0,   872,   360,   867,   781,   899,   893,
     868,   326,   908,   870,   871,   887,   888,   889,   909,   869,
     933,   934,   947,   935,   949,   876,   873,   874,   957,   958,
     962,   963,   875,   894,     0,     0,   994,     0,     0,     0,
     364,   209,     0,     0,     0,   774,    26,     0,    32,     0,
       0,    42,    69,    71,  1010,    67,     0,     0,     0,   776,
       0,    47,    49,    27,     0,     0,   573,     0,     0,     0,
     646,   644,     0,   653,     0,   652,   654,   659,   640,     0,
     620,     0,   595,   606,   592,   606,   593,   607,   613,   617,
     618,   602,   587,   707,   755,   785,     0,     0,   261,     0,
       0,   570,   569,     0,   622,   626,   729,   731,   736,   633,
     632,   741,   176,     0,   474,   137,     0,   477,     0,   478,
     473,     0,   416,     0,   418,   417,     0,   419,     0,   413,
       0,     0,   262,     0,     0,   425,     0,   426,   371,   459,
       0,   465,     0,     0,     0,   462,   994,   382,   381,   480,
       0,   399,     0,   157,     0,     0,   166,     0,   165,     0,
       0,   155,   169,   170,   172,   162,   529,   541,   516,   551,
     519,   542,   552,     0,   558,   520,     0,   548,     0,   543,
     555,   515,   538,   517,   518,   513,   371,   498,   499,   500,
     508,   514,   501,   510,   531,     0,   536,   533,   511,   548,
     512,   120,   259,   121,     0,     0,   132,     0,     0,   192,
     232,   379,   369,     0,   211,     0,    81,    83,     0,     0,
     101,   100,   991,     0,     0,     0,     0,   858,   859,   860,
       0,   706,   789,   798,   801,   807,   802,   803,   804,   805,
     809,   810,   808,   811,   806,   787,     0,   715,   730,   986,
       0,   986,   910,     0,     0,   986,   986,   986,     0,   911,
     912,     0,     0,   973,     0,     0,     0,     0,     0,     0,
     878,   877,     0,   730,   899,     0,   325,  1011,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   508,
     531,     0,     0,   362,     0,   272,     0,   773,     0,     0,
      43,    72,     0,     0,    25,   763,     0,   779,     0,   745,
     763,     0,     0,    46,   572,     0,   636,     0,     0,   707,
       0,   658,     0,   563,     0,   562,   930,     0,   596,     0,
     604,   600,   599,     0,   610,     0,   615,   603,     0,   709,
     754,     0,     0,     0,   232,   369,   624,     0,   740,     0,
       0,   138,   177,   476,   371,   415,     0,   371,   372,   420,
     369,     0,   258,   369,   231,   995,   203,   200,     0,   143,
     371,   498,   369,     0,     0,     0,   230,   233,   234,   235,
     236,     0,   424,   464,   463,   423,   422,     0,     0,     0,
     385,   383,   384,   380,     0,   371,   469,     0,   468,   186,
     163,   167,     0,     0,   171,     0,     0,   526,   766,   539,
     540,   556,     0,   553,   558,     0,   546,   544,   545,   143,
       0,     0,     0,   509,     0,     0,     0,     0,   547,   261,
       0,   369,   133,   191,     0,   194,   196,   197,     0,     0,
       0,     0,   211,   211,    78,     0,    86,    77,     0,     0,
       0,     0,   800,   788,     0,     0,   861,   862,   863,     0,
     792,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     711,   713,   714,   708,     0,   743,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     977,   978,   979,   980,   981,   982,   983,   984,     0,     0,
       0,     0,   968,     0,     0,     0,     0,     0,     0,     0,
     886,   743,   885,   327,     0,   881,   993,   880,   879,   882,
     883,   884,   900,   955,     0,   891,   783,   782,   366,   365,
       0,   265,   280,     0,   271,   273,   775,     0,     0,     0,
      70,     0,   768,     0,   778,     0,   768,     0,     0,   645,
     648,     0,   660,   661,   664,   666,   663,   675,   676,   677,
     665,   688,     0,   700,   655,   657,  1003,   564,   931,   605,
     614,   609,   608,     0,   619,   756,   730,   757,   786,   369,
       0,     0,   732,   638,   733,   742,   475,   156,   183,   175,
     178,   179,     0,     0,     0,   414,   369,   369,   372,   270,
     263,   272,   202,   371,   371,   372,   204,   199,   205,     0,
       0,   241,     0,   428,   372,   371,   369,   427,     0,     0,
       0,   398,   483,   470,   187,   188,   164,     0,   174,   527,
     528,   929,     0,     0,     0,   531,     0,   554,     0,     0,
     372,     0,   504,   506,     0,     0,     0,     0,     0,     0,
       0,   379,   272,   156,   135,   232,   189,   399,   374,     0,
      82,     0,   211,     0,    90,     0,    84,     0,     0,    89,
      85,     0,   750,   996,     0,     0,   793,     0,     0,   799,
       0,     0,   791,   790,     0,   848,     0,   837,   836,     0,
     815,     0,   846,   864,     0,   856,     0,     0,     0,   816,
     842,     0,     0,   819,     0,     0,     0,     0,     0,     0,
       0,   817,     0,     0,   818,     0,   850,     0,     0,   812,
       0,     0,   813,     0,     0,   814,   712,   715,   748,     0,
       0,   211,     0,   936,     0,     0,   950,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   974,     0,     0,   972,
       0,     0,   964,     0,     0,     0,     0,   975,   630,   898,
     954,     0,   260,   279,     0,   338,   361,   274,     0,     0,
       0,     0,     0,     0,    40,    51,    53,    52,    61,    63,
      40,    41,   529,   529,     0,   760,   780,     0,   759,     0,
       0,     0,   696,   663,     0,   698,   692,   698,   696,   698,
       0,     0,     0,   659,     0,   705,   611,   612,   616,   743,
     272,   190,   374,     0,     0,   184,     0,   180,     0,     0,
       0,   410,   219,   214,   901,   215,   216,   217,   218,     0,
       0,   264,     0,     0,     0,   220,     0,   241,   736,   237,
       0,   429,   460,   461,   390,   391,   387,   386,   389,   388,
     371,   482,   372,   173,   992,     0,     0,   522,     0,   521,
     557,     0,   549,   144,     0,   502,     0,   530,   531,   537,
     535,     0,   532,   369,     0,     0,     0,   127,   136,   195,
     371,     0,   370,   104,     0,   125,   124,    93,     0,     0,
      94,     0,    87,    88,     0,   854,   855,   795,   794,   797,
     796,     0,     0,     0,     0,   730,     0,     0,     0,   857,
       0,     0,     0,     0,     0,     0,   849,   847,   843,     0,
     851,     0,     0,     0,     0,   852,     0,     0,     0,     0,
       0,     0,   719,     0,   716,     0,   720,   942,   941,     0,
       0,   938,   937,     0,   946,   945,   944,   943,   940,   939,
       0,   953,   970,   969,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,   276,   277,     0,   994,     0,     0,
       0,     0,     0,     0,     0,  1016,  1021,     0,     0,   359,
     290,   285,     0,   338,   284,   294,   295,   296,   297,   298,
     305,   306,   299,   301,   302,   304,     0,   309,     0,   353,
     355,   354,     0,     0,     0,     0,     0,     0,   758,     0,
     994,    65,    57,    58,    64,    59,    60,     0,    23,     0,
       0,    24,   764,     0,   765,   762,   767,   770,   769,   744,
      44,    45,   659,   667,   662,   697,   695,   693,     0,   694,
       0,     0,     0,     0,     0,     0,   709,   753,     0,   370,
     737,   738,   639,   186,   185,   181,     0,   372,   902,   268,
     269,   372,   372,   142,   207,   221,   222,   371,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   479,
     524,     0,   525,     0,     0,   207,   145,   146,   148,   149,
     150,   505,   507,   503,   534,   272,   399,   372,   186,     0,
       0,   367,   371,    91,     0,     0,    97,    92,   531,     0,
     840,     0,     0,   866,   743,   865,   838,     0,     0,     0,
     844,     0,     0,     0,     0,   853,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   710,   715,     0,   722,
       0,   718,   721,   956,   210,   976,   948,     0,     0,   971,
     966,   965,     0,   959,   961,     0,   213,   275,     0,   314,
       0,     0,     0,   308,     0,   323,   307,   339,   340,     0,
    1004,   356,   357,     0,   286,     0,   291,   288,   344,     0,
       0,   310,     0,   300,   293,   303,   292,   311,   337,     0,
      54,    55,    56,    62,   529,   670,     0,   678,   696,   687,
     685,   699,   701,   703,   704,   635,   372,   368,   739,   182,
     372,   152,   257,     0,     0,   198,   223,     0,     0,     0,
     241,   224,   227,   772,     0,   372,   396,   397,   393,   392,
     395,   394,     0,   523,   550,   151,   139,   147,     0,   371,
     254,   134,   372,   373,     0,     0,    95,    96,    79,     0,
     823,   831,   629,   839,   824,   832,   827,   835,   841,   845,
     825,   833,   826,   834,   820,   828,   821,   829,   822,   830,
     717,     0,   728,   952,   951,   967,     0,     0,     0,   282,
     278,     0,   312,     0,   328,     0,     0,     0,   341,     0,
     352,     0,     0,     0,     0,   347,   345,     0,     0,     0,
     332,     0,   333,     0,    39,   761,   672,   669,   679,     0,
       0,   680,   681,   682,     0,   255,   411,   201,   206,   225,
     241,   772,   228,   247,   240,   242,   400,   372,     0,   375,
     372,    76,    98,    99,     0,   725,     0,   724,   960,   212,
     281,     0,   313,   331,     0,   316,     0,   322,     0,   351,
     350,  1000,   349,   338,     0,     0,     0,     0,   324,     0,
       0,     0,   668,   683,     0,   702,   229,   247,     0,   241,
     243,   244,   256,   372,     0,   723,     0,     0,     0,     0,
       0,     0,   338,   358,   346,   348,     0,   343,     0,   335,
     334,     0,   673,     0,   241,     0,     0,   239,     0,   246,
       0,   245,   376,   126,   726,     0,   283,     0,   329,   315,
     319,   321,     0,     0,     0,   338,     0,   671,   684,   226,
       0,     0,   250,   249,   248,   727,     0,   338,     0,     0,
     338,   336,   674,   253,   251,   252,   330,   320,   338,   342,
     317,   318,     0,     0,     0
};

static const short yydefgoto[] =
{
    1902,    20,    21,   497,   135,   136,   137,   138,  1478,   754,
     749,    22,   149,   150,  1234,  1235,  1236,  1237,  1238,   139,
     140,   501,   502,    23,    86,   405,   896,   665,   666,   667,
    1130,  1123,  1124,  1567,   260,   669,    24,    78,    25,   167,
      26,   394,    79,  1343,   241,   398,   657,  1114,  1337,   564,
     230,   390,  1065,  1545,  1546,  1547,  1548,  1549,  1550,   236,
     251,   227,   604,   228,   386,   387,   388,   611,   612,   613,
     565,   566,  1049,  1050,  1051,  1276,  1086,   244,   336,   400,
     884,   885,   886,   817,   818,   819,  1675,   586,   487,  1399,
     668,  1739,  1281,  1524,  1525,  1526,  1681,   887,   588,   826,
     827,   828,   829,  1299,   830,  1829,  1830,  1831,  1883,   239,
     334,   651,   212,   367,   493,   581,   582,   990,   488,  1058,
     993,   994,   995,  1423,  1424,  1224,  1740,  1425,  1440,  1441,
    1442,  1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1747,  1454,   454,   455,  1840,  1761,  1455,  1456,
    1457,  1874,  1637,  1638,  1754,  1755,  1458,  1459,  1460,  1461,
    1633,    27,    28,   280,   489,   490,   248,   338,   889,  1561,
     573,  1055,  1342,   246,   653,   376,   599,   600,   843,   845,
    1311,    29,    62,   808,   805,   361,   362,   371,   372,   204,
     205,   373,  1074,   594,   835,   209,   220,   177,   354,   355,
     217,   601,  1312,    30,   101,   636,   637,   638,  1101,  1102,
     639,   739,   641,   642,  1095,  1482,   873,   643,   740,   645,
     646,   647,   648,   866,   649,   650,   863,    31,    32,    33,
      34,    35,   552,    36,   517,    37,    38,    64,   224,   379,
      39,   322,   323,   324,   325,   326,   327,   328,   329,   781,
     330,  1033,  1268,   537,   538,   786,   539,    40,  1462,   173,
     554,   343,    42,  1364,   722,    43,    44,    45,   520,   312,
    1273,  1668,   154,   155,   156,   310,   524,   525,   526,   772,
     769,  1012,  1013,  1014,  1015,  1766,  1822,  1851,  1016,  1017,
    1018,  1019,  1771,  1772,  1773,  1020,  1263,   160,   161,  1261,
    1496,  1265,  1661,  1662,  1506,   409,   698,  1392,   934,  1393,
    1394,  1395,  1601,  1794,  1797,   176,   556,   557,   256,  1512,
    1044,   349,  1463,  1464,    48,    49,   410,  1465,    51,    52,
     543,  1466,  1467,    53,  1002,  1483,  1245,  1486,   283,   284,
     494,   508,   509,   756,   456,   457,   546,   681,   909,   682,
     683,   684,   685,  1149,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   911,  1152,  1153,   696,   459,   460,
     461,  1283,   462,   463,   464,   465,   466,   467,   468,   858,
      83,   258,   243,  1093,  1103,   776,   469,   470,   471,   472,
     473,   474,  1608,   475,   476,   477,   478,   479,   480,   965,
     481,   714,  1210,   715,  1207,   482,   958,   311,   347,   483,
     484,   671,  1313,   975,   233,   814,  1132,   264,   207,   261,
    1810,   237,  1795,  1025,  1469,  1021,   141,   485,   216,   218,
    1239,   726,   249,   168,   486,   143
};

static const short yypact[] =
{
    1564,   456,    76,  3026,   286,   197,   406,   481,    78,  2134,
     220,   485,  1581,    76,   435,   471,  5659,   521,  5659,   361,
     345,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   575,   320,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  7704,  5659,  5659,  5659,  5659,
    5659,  5659,-32768,-32768,   508,   531,  5659,  5659,  5659,   706,
    5659,   546,  5659,  5659,-32768,  5659,  5659,  5659,-32768,   705,
  -32768,   553,-32768,-32768,   703,  5659,-32768,  5659,  5659,  5659,
     707,  5659,  5659,  5659,  5659,   546,  5659,  5659,  5659,  5659,
    5659,-32768,  5659,   514,   656,-32768,-32768,-32768,   525,-32768,
     525,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   697,   562,   740,-32768,    37,
  -32768,-32768,-32768,-32768,  5659,  5659,  5659,   792,   798,  1460,
    5659,   702,    27,   239,   449,   679,-32768,  5659,   808,  1055,
     765,-32768,  5659,-32768,  5659,  5659,  5659,-32768,-32768,-32768,
    5659,-32768,   768,   759,   937,   470,   712,   762,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   292,-32768,-32768,   531,-32768,-32768,
     474,-32768,-32768,   710,-32768,   623,-32768,-32768,   597,-32768,
  -32768,   713,-32768,   816,-32768,-32768,   783,-32768,   863,-32768,
  -32768,   973,-32768,-32768,-32768,   531,-32768,-32768,   606,-32768,
     710,-32768,-32768,   206,-32768,   928,-32768,   929,-32768,   525,
  -32768,-32768,-32768,-32768,-32768,-32768,   924,-32768,-32768,   812,
  -32768,   938,-32768,   827,-32768,-32768,-32768,   812,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6880,  5659,
     872,-32768,  5659,-32768,-32768,-32768,   854,  1265,   889,  3986,
    5659,   959,   770,   984,  1002,  1013,   955,   962,   114,   847,
  -32768,  7099,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5347,-32768,-32768,-32768,   930,  5659,  1005,   920,   399,  5659,
     945,-32768,-32768,  1055,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  5659,-32768,-32768,-32768,   710,-32768,   928,-32768,   525,
     995,  1003,   882,-32768,  7099,   974,-32768,   974,  7099,-32768,
      89,   879,   891,   883,   762,-32768,  1121,   216,-32768,  5659,
  -32768,   292,-32768,-32768,-32768,-32768,  5659,   872,  1988,   946,
    2607,   881,-32768,  7704,-32768,-32768,   249,   967,-32768,-32768,
  -32768,   893,   893,   531,  1050,   531,   210,   863,-32768,-32768,
    3588,-32768,  5659,  5659,-32768,   872,-32768,-32768,  1135,   396,
     901,  5659,  1170,  5659,-32768,-32768,    39,  5002,  4366,  1047,
  -32768,-32768,   907,   911,   914,   994,-32768,-32768,   919,-32768,
     921,-32768,   934,   935,-32768,   956,-32768,-32768,-32768,-32768,
     960,   961,  1001,  1006,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   963,-32768,-32768,  5566,   966,   975,-32768,   981,  1136,
    7099,  7099,  5785,  5659,-32768,-32768,   979,-32768,  1203,-32768,
  -32768,   978,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   983,   954,   980,  3588,  1004,   591,
  -32768,-32768,   997,  1266,   605,-32768,-32768,  5659,-32768,  5659,
    5659,   244,-32768,-32768,-32768,-32768,  5659,  5659,    90,-32768,
    5659,-32768,-32768,   854,  1138,  3986,-32768,  5659,  5785,   295,
  -32768,-32768,  7099,-32768,  1214,  1017,-32768,   170,-32768,   425,
  -32768,    98,-32768,   473,-32768,   473,-32768,  1018,-32768,    70,
  -32768,  1114,-32768,     7,-32768,-32768,  1021,  1014,   872,  1029,
    1284,-32768,-32768,  5659,-32768,-32768,  1027,-32768,   411,-32768,
  -32768,  1112,-32768,  1217,  1218,-32768,   531,-32768,  1220,-32768,
  -32768,  1292,-32768,  1294,-32768,-32768,  1274,-32768,  3588,-32768,
    3588,   640,-32768,  1313,  5659,-32768,  7585,-32768,   156,-32768,
    5659,   764,   623,  1150,  1118,  1191,  1156,-32768,-32768,-32768,
     142,  1195,   441,-32768,  1141,   546,-32768,   531,-32768,  1311,
    1249,-32768,   210,-32768,-32768,-32768,   316,  1161,-32768,-32768,
  -32768,  1163,-32768,  1212,  1075,-32768,  1261,  1078,   200,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1083,-32768,-32768,-32768,    94,  1089,  1094,    67,-32768,  1078,
  -32768,-32768,   710,-32768,  1303,  1361,-32768,   531,   531,-32768,
    1988,   597,-32768,  1255,-32768,  1230,  1111,-32768,  3322,  1262,
  -32768,-32768,-32768,  1268,  1115,  4620,  1116,  1088,  1171,  1305,
    4112,   188,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2548,   120,  1267,   612,
    7099,   160,-32768,  5659,  7099,   655,   665,   692,  1687,-32768,
  -32768,  7099,  4874,  1203,   112,  1207,  7099,  7099,  4874,  1337,
     187,   187,  1127,   320,   452,   691,-32768,-32768,  7099,  5659,
    7099,  7099,  7099,  7099,  7099,  7099,  6004,  2668,-32768,-32768,
    1384,  1125,  5659,-32768,  5659,  1367,  5659,-32768,  1237,  1242,
  -32768,-32768,  1224,  3986,-32768,  1372,   695,-32768,  1159,-32768,
    1372,  5659,  5659,  1162,-32768,   452,-32768,   626,  4240,  1246,
    7099,-32768,  5659,-32768,   607,-32768,-32768,   175,-32768,  1260,
  -32768,-32768,-32768,  5659,   683,  5659,-32768,-32768,  5659,  1047,
  -32768,  7099,  5659,  1437,  1988,-32768,  1173,  7099,  1209,  7099,
     531,-32768,   542,-32768,-32768,-32768,  1175,-32768,-32768,-32768,
     104,  5659,-32768,-32768,-32768,-32768,  1435,-32768,  1179,  1421,
  -32768,  1438,-32768,  1381,  1383,   525,-32768,-32768,-32768,-32768,
  -32768,  1455,-32768,-32768,-32768,-32768,-32768,  5659,  7644,   546,
    1353,  1354,  1355,-32768,   546,-32768,-32768,   609,-32768,   677,
  -32768,-32768,  1328,   531,-32768,  5002,   256,  1331,-32768,-32768,
  -32768,-32768,   546,-32768,  1075,   298,-32768,-32768,-32768,  1421,
     298,  1332,   298,-32768,   546,   546,  1211,   546,-32768,   872,
    5659,-32768,  1391,-32768,   716,-32768,-32768,-32768,   249,   974,
    5659,   717,    21,-32768,-32768,  1221,   491,-32768,  5002,  5659,
     974,  5128,-32768,-32768,   974,  4620,-32768,-32768,-32768,   111,
  -32768,   324,  1790,  4874,  4874,  7099,  7099,  1911,  1222,   694,
    2260,  7099,  2528,   614,  2794,  3060,  6223,  3326,  3592,  3858,
  -32768,-32768,  1382,-32768,  1223,   712,  7099,  7099,   353,  7099,
    1216,  7099,  1225,   637,  7099,  7099,  7099,  7099,  7099,  7099,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1439,  1129,
     163,  7099,-32768,  4874,  7099,   132,   551,   977,   115,  5659,
  -32768,   712,-32768,-32768,   209,-32768,-32768,  1482,   187,   187,
     761,   761,  1203,-32768,   724,-32768,-32768,-32768,-32768,-32768,
     729,-32768,  1322,  1491,  1367,-32768,-32768,  3732,  3732,  1385,
  -32768,   235,  1401,  5659,-32768,  7099,  1401,  1447,  1450,-32768,
  -32768,  2884,  1239,  1418,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1241,  4874,  1444,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   458,-32768,-32768,  1267,  1203,-32768,-32768,
     732,   974,-32768,-32768,-32768,  1203,-32768,   893,  1509,-32768,
     542,-32768,  1247,  4874,  2461,-32768,-32768,-32768,-32768,-32768,
  -32768,  1367,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   525,
     525,  1287,  1254,-32768,-32768,-32768,-32768,-32768,   127,   173,
     564,-32768,  1523,-32768,-32768,-32768,-32768,  5659,-32768,-32768,
  -32768,-32768,   286,   787,  1374,  1384,  1259,-32768,  1141,   793,
  -32768,   563,-32768,  1263,  5659,   594,  1269,  1272,   546,  1273,
    1527,   597,  1367,   893,  1218,  1988,-32768,  1195,  1346,  1698,
  -32768,   546,  1406,  1492,-32768,  3322,-32768,   546,   537,-32768,
  -32768,  1493,-32768,-32768,  1275,  1276,-32768,  4874,  4874,-32768,
    4874,  4874,-32768,  1535,   201,  1203,  1278,-32768,-32768,  1279,
    1203,  5785,-32768,-32768,  1494,-32768,   185,  1280,  1282,  1203,
     339,  1306,  1307,  1203,  7099,  7099,  1222,  7099,  6442,  1309,
    1314,  1203,  1316,  1318,  1203,  7099,  1203,  1319,  1323,  1203,
    1324,  1326,  1203,  1327,  1333,  1203,-32768,  5231,-32768,   663,
     672,-32768,   682,-32768,   704,  7099,-32768,   741,   752,   850,
     877,   896,   957,  7099,  7099,  7099,  1203,  1517,   204,  1203,
    1387,  7099,-32768,  7099,  7099,  7099,  7099,-32768,-32768,-32768,
  -32768,  5659,-32768,-32768,  5659,  7328,-32768,-32768,  5659,  5659,
    5659,  5659,  5659,  5659,  1378,  1335,-32768,  1336,-32768,-32768,
    1378,-32768,   392,   392,   251,-32768,-32768,   810,-32768,  3732,
    3732,  1304,  1317,  1321,  4240,  1466,-32768,  1466,   592,  1466,
    1522,  1529,  7099,   806,  1597,  1544,-32768,-32768,-32768,   712,
    1367,-32768,  1346,    24,  1141,  1488,  1141,-32768,  4874,   118,
     689,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  2461,
    2461,-32768,  1491,  7099,  2461,   186,  1474,  1287,   705,-32768,
    4874,-32768,-32768,-32768,  1496,  1498,  1501,  1502,  1504,  1506,
  -32768,-32768,-32768,-32768,-32768,  1356,   286,-32768,  1436,-32768,
  -32768,   298,-32768,   267,   298,-32768,   298,-32768,  1384,-32768,
  -32768,  1359,-32768,-32768,   249,  1491,  1141,-32768,  1218,-32768,
  -32768,  1616,-32768,-32768,  1363,-32768,  1173,-32768,  1366,   531,
     123,  1371,-32768,-32768,   531,-32768,-32768,-32768,  1535,-32768,
    1535,  7099,   974,   974,  1373,   320,   813,  7099,  1590,-32768,
     974,   974,  7099,   974,   974,   255,  1203,-32768,  1190,  7099,
    1203,   974,   974,   974,   974,  1203,   974,   974,   974,   974,
     974,   974,-32768,  1379,  1388,   571,  5659,-32768,-32768,  1390,
    3588,-32768,-32768,   992,-32768,-32768,-32768,-32768,-32768,-32768,
    1026,  1142,-32768,-32768,  7099,  7099,  1607,  1495,  1048,   819,
    1103,-32768,  3588,  1395,-32768,-32768,  1628,  1635,  4494,    48,
    1398,  1397,  1592,  7099,  1402,  1405,  4748,  5659,  5659,-32768,
  -32768,-32768,  1631,  7230,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   129,-32768,  1410,-32768,
  -32768,-32768,  1603,  1412,  1413,  1414,  1416,  1417,-32768,  1415,
     669,-32768,-32768,-32768,-32768,-32768,-32768,  1624,-32768,  3732,
    3732,-32768,-32768,  1521,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1686,-32768,  1418,-32768,-32768,-32768,  1612,-32768,
    4240,  4240,   822,  5659,  7099,  4874,  1047,-32768,  1491,-32768,
  -32768,-32768,-32768,   677,-32768,-32768,   119,-32768,-32768,-32768,
  -32768,-32768,  1203,-32768,  1675,   186,-32768,   715,  5659,-32768,
    1620,   136,  1524,  1621,  1525,  1658,  1623,  1662,  1367,-32768,
  -32768,  1432,-32768,  1433,  1617,  1675,   267,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1367,  1195,-32768,   677,  1523,
    1588,-32768,-32768,-32768,  1604,   107,-32768,-32768,  1384,  1610,
    1203,  1441,  1446,-32768,   712,-32768,  1203,  7099,  1453,  1456,
    1203,  1457,  1458,  7099,  7099,  1203,  1461,  1463,  1464,  1468,
    1472,  1477,  1485,  1486,  1487,  1489,-32768,  5231,  1448,-32768,
    5659,-32768,-32768,-32768,-32768,-32768,-32768,  7099,  1497,-32768,
  -32768,-32768,  7099,-32768,-32768,  7099,   141,-32768,  1666,-32768,
    6661,  5659,  7099,-32768,  4874,  1203,-32768,-32768,-32768,  1507,
  -32768,-32768,-32768,  5659,-32768,   523,-32768,  1543,-32768,   323,
    1510,-32768,  3480,-32768,-32768,-32768,-32768,-32768,-32768,  1613,
  -32768,-32768,-32768,-32768,   392,  5659,  4240,-32768,  1578,-32768,
  -32768,  1490,-32768,  1203,   188,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1505,  5659,-32768,-32768,  1677,  1683,  5659,
    1287,-32768,-32768,   525,  5659,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1491,-32768,-32768,-32768,-32768,-32768,  1491,-32768,
  -32768,-32768,-32768,-32768,  7099,   531,-32768,  1583,-32768,   531,
  -32768,-32768,-32768,  1203,-32768,-32768,-32768,-32768,  1203,  1203,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  5659,  1706,  1203,-32768,-32768,  1070,  2461,  2461,-32768,
  -32768,  1512,-32768,   405,  6880,   763,   150,  1515,-32768,  1708,
  -32768,  5659,  5659,   286,   131,-32768,-32768,  1545,  1712,  4874,
  -32768,   581,-32768,  1516,-32768,-32768,  1520,-32768,-32768,  4874,
    1530,-32768,-32768,-32768,  7099,-32768,-32768,-32768,-32768,-32768,
    1287,   525,-32768,  1676,-32768,-32768,-32768,-32768,  1523,-32768,
    1203,-32768,-32768,-32768,  1528,  1526,  1534,-32768,-32768,-32768,
  -32768,   974,-32768,  1651,  3480,-32768,  1627,-32768,  3480,-32768,
  -32768,-32768,-32768,  7381,   523,  7099,  3480,   152,-32768,  3480,
    5440,  5659,-32768,   188,  5659,-32768,-32768,  1676,   246,  1287,
    1692,  1694,-32768,-32768,  1542,-32768,  5659,  5659,  1546,  3214,
    1548,   618,  7381,  1549,-32768,-32768,  1186,   139,  1773,-32768,
  -32768,   837,-32768,   838,  1287,   487,   487,-32768,  1781,-32768,
    1646,-32768,-32768,-32768,-32768,  1552,-32768,  3480,  1549,-32768,
  -32768,  1780,  3480,  1795,  1785,  7381,  5659,-32768,-32768,-32768,
    1633,   541,-32768,-32768,-32768,-32768,   853,  7381,   148,  5659,
    7381,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  7381,-32768,
  -32768,-32768,  1837,  1838,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1329,  1690,-32768,  1691,  1556,   604,-32768,
    1084,-32768,-32768,-32768,  -881,-32768,  -578,-32768,  -541,  1695,
    1558,  1334,  1098,-32768,-32768,-32768,   727,-32768,  1189,   968,
  -32768,-32768,   733,-32768,-32768,   958,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   745,
  -32768,-32768,   985,-32768,-32768,   314,-32768,-32768,  1508,  1786,
  -32768,-32768,  -348,-32768,-32768,-32768,  1475,-32768,-32768,  1256,
    -365,-32768,-32768,-32768,   820,-32768, -1265,-32768,-32768,  1532,
    1077,   757,  1511,  1053,   756,-32768,   331,   -50,  -264,-32768,
    -330,-32768, -1121,-32768,-32768,   352,-32768,  1514, -1165,-32768,
  -32768,-32768,-32768, -1256,   356,    57,    54,    56,    31,-32768,
  -32768,-32768,-32768,  -135,  -297,-32768,  1079,-32768,  -673,-32768,
    -996,-32768,   894,-32768,-32768,-32768,-32768,-32768, -1304,  -953,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  -504,   151,-32768,  -538,-32768,-32768,
  -32768,-32768,-32768,   257,-32768,    91,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1165,-32768,-32768,  -619,   383,
    -302,  -915,   639,-32768,-32768,  -558,  -778,-32768,-32768,  -993,
   -1430,-32768,-32768,-32768,-32768,-32768,  1551,-32768,  1330,  1540,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1562,
  -32768,-32768,-32768,-32768,-32768,  -553,  -458,-32768,  1046,   595,
    -595,  -373,-32768,-32768,-32768,  1338, -1014,-32768,  -372,-32768,
  -32768,-32768,-32768,  1281,-32768,-32768,  1057,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1910,-32768,-32768,
  -32768,-32768,-32768,  1606,-32768,-32768,-32768,  1396,-32768,  1399,
  -32768,-32768,-32768,-32768,  1149,-32768,-32768,-32768,     2,-32768,
  -32768,-32768,  -408,   641,-32768,  -331,   797,-32768,  -232,   -55,
  -32768,-32768,-32768,-32768,  1778,-32768,-32768,-32768,  1167, -1093,
  -32768,-32768,  -949, -1348,-32768,-32768,-32768,-32768,   931,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1853,   -27,   693,
    -114,-32768,-32768,   181,-32768,  -397,  -746,  1264,-32768,   359,
  -32768,   569,-32768,  -910,-32768,  -624,-32768,  1169,  -521,-32768,
  -32768,  -838,  1967,  1968,-32768,-32768,  1426,  1970,-32768,-32768,
  -32768,  -230,-32768,-32768,  1213, -1134,   965,-32768,  -104,  -651,
    -539,-32768,-32768,-32768,  -386,   -40,  -452,  -687,-32768,  -443,
  -32768,-32768,-32768,   600,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  -550,-32768,   809,-32768,   831,  -943,-32768,
    -358,-32768,   696,  -903,  -142,  -889,  -887,  -879,   -43,    -3,
     -35,-32768,   -60, -1019,  -490,  -140,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1172,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   766,-32768, -1098,-32768,-32768,   224,-32768,  -873,
     240,  1117,   874,   307,  -717,  1393, -1170,-32768,   -62,  1893,
  -32768,   -23,   -49, -1591,   549,    38,   -57,  -159,    -1,   -69,
    -266,-32768,   -74,   -56,    -9,-32768
};


#define	YYLAST		7970


static const short yytable[] =
{
     142,    82,    41,   332,   247,   235,   285,   163,   210,   169,
     303,   306,   304,   307,   796,   162,   231,   640,   644,   252,
     987,   614,   229,   503,   275,   960,   232,   266,   274,   738,
     578,   968,   215,   820,   605,   272,   242,   798,   221,   775,
    1226,  1529,   276,  1036,   270,   245,   206,   208,   211,   214,
     163,   219,   222,  1468,  1426,   757,   576,   234,   208,   222,
     242,   214,  1252,   163,   219,  1292,   250,   142,   222,   269,
     583,   991,   521,   897,   935,  1038,   262,   277,   163,   265,
     208,  1319,   262,   222,   211,   214,   162,   163,   219,   250,
     142,   222,   339,   214,   725,   273,   213,  1188,   655,   971,
     759,   544,   580,   888,   291,   395,   293,  1412,   240,  1485,
    1117,  1282,   848,  1416,   340,   871,  1335,  1240,   292,  1137,
     809,   723,   810,   913,  1340,   903,   913,   913,   821,  1702,
    1527,   784,   271,  1218,   314,   234,   163,   234,   337,  1636,
     278,   142,  1565,  1291,   913,   402,   789,  1056,   222,  1873,
    1295,  1284,  1657,   333,  1121,   214,   163,   250,   913,  1301,
     913,   169,   961,   962,   363,  1285,   308,  1286,  1519,  1520,
    1503,   913,   547,  1523,  1071,  1287,  1041,   515,  1813,   562,
     771,  1288,  1211,  1212,  1737,  1323,   840,   723,   -80,  1621,
    1639,  1059,   391,   729,  1061,  1898,   913,   408,   730,   801,
     548,   533,   335,  1068,  1706,   930,   939,  1304,  -232,  1361,
     729,   289,   913,   640,   644,   730,   584,  1306,   823,   102,
     396,   867,   841,   300,   729,  1488,  1142,  1143,   931,   730,
    1852,  1368,   902,  -656,   751,   550,  1138,   910,   144,   491,
     914,   777,   495,   914,   914,    14,   574,   614,  1669,   503,
     522,   793,   876,   609,   597,  1510,    84,    85,   528,   575,
     320,   914,  1112,  1583,   598,  1511,   530,  1631,  1632,    63,
     234,  1468,   758,   234,  1508,   914,  1208,   914,   729,  1369,
     504,   142,   788,   730,   932,  1892,   831,   868,   914,  -371,
    1855,   545,   540,  1701,  1622,   824,   356,  1122,  1242,  1542,
     562,   963,   145,   301,  -232,  1494,   211,  1305,  1768,   302,
     163,   397,   290,   914,  1553,   664,  1609,  1610,   729,   577,
    1640,  1213,   842,   730,   654,  -232,   491,  1566,   232,   914,
     591,  -232,  1140,   595,   869,   825,  1205,  1707,   103,  1521,
     606,   357,   608,   877,   563,   785,  1282,  1282,   547,   723,
     234,  1282,   535,  1307,   663,  1136,   659,   234,  1833,   234,
    1527,   234,   729,  1191,   596,  -232,  1279,   730,  1490,  1491,
     974,  1057,  1023,   872,   661,  1099,   729,  1414,   984,  1757,
    1075,   730,  1557,   214,   219,  1544,  1284,  1284,  1139,   290,
    1216,  1284,   163,  1372,   211,  1517,  1670,  1539,   672,  1655,
    1285,  1285,  1286,  1286,   670,  1285,  1814,  1286,  1738,  1243,
    1287,  1287,  1269,  1685,  1819,  1287,  1288,  1288,  1296,  1297,
    1270,  1288,   254,  1819,  1782,   533,  1856,  1806,   729,  1848,
     652,  1507,   940,   730,   729,   300,   752,  1289,  1290,   730,
     731,   732,   733,   734,   727,  -656,   174,  1091,   358,  1141,
    1357,  1358,  1091,  1359,  1360,   308,   255,  1303,   902,   733,
     734,   764,   902,  1749,   849,   610,   359,   545,  1345,    87,
     748,   731,   732,   733,   734,   729,   345,   364,    14,   755,
     730,  1118,    80,   760,   735,   855,   658,   503,   163,  1219,
     214,   504,  1134,    54,   300,   309,  1135,   163,   175,   406,
     360,   163,  1052,   146,    55,  1054,   504,   411,   169,  1844,
    1128,    56,    80,   495,  1735,   305,   346,   879,  1066,   753,
    1765,   302,   534,   802,  1826,   731,   732,   733,   734,    88,
     897,  1092,  1750,   302,  1009,    57,    89,   750,  1871,   157,
      90,    91,  1692,  1082,   234,   850,   822,   580,  1421,    92,
     365,  1246,  1658,  1334,  1708,  1666,  1556,    81,  1035,  1698,
      93,   855,  1125,  1699,   851,   731,   732,   733,   734,  1098,
     242,  1891,   766,  1857,   729,   815,   535,   234,  1751,   730,
    1346,   815,  1110,  1897,  1894,  1129,  1900,    81,    80,  1752,
     779,  1516,   856,   536,  1901,   225,   226,    58,  1879,   988,
     374,  1266,  1671,  1251,  1880,   170,  1672,  1673,  1308,   731,
     732,   733,   734,  1531,   882,   883,  1799,  1800,   308,   171,
     232,   300,    94,   731,   732,   733,   734,   368,    59,   547,
     369,  1164,    60,  1272,  1028,  1895,   172,   300,    95,   158,
    1047,  1468,  1700,  1881,  1309,  1165,   159,  1247,   773,   729,
    1598,   234,  1267,    96,   730,   223,  1255,    97,   936,   780,
     729,   308,   164,    81,   846,   730,   152,   153,    98,  1256,
    1468,   308,   370,   375,   302,   731,   732,   733,   734,  1802,
     942,   731,   732,   733,   734,  1599,   729,    99,  1753,  1166,
    1882,   730,  1257,   165,   222,   729,   774,  1600,   308,  1274,
     730,   944,   491,  1468,   491,   729,   996,  1028,   166,  1048,
     730,   946,   847,   238,  1555,  1468,   254,  1167,  1468,  1899,
     976,    61,   731,   732,   733,   734,  1468,   729,   234,   972,
     225,   226,   730,   234,  1007,   234,  1712,   234,   948,  1786,
    1154,  1574,    80,  1259,   504,  1787,   100,   392,   545,   257,
     255,  1775,   163,   214,   232,  1776,   540,  1046,  1034,   214,
    1665,  1293,  1294,  1026,   729,  1336,   350,  1352,   259,   730,
    1785,   491,   267,  1302,   163,   729,   163,  1530,   351,  1077,
     730,  1168,   393,   234,   729,   234,   729,  1789,  1155,   730,
     279,   730,  1125,  1366,  1282,  1282,   281,  1073,  1076,  1353,
     750,   282,   234,   300,   242,   300,  -686,  1084,  1085,  1081,
    1088,   352,  1156,   353,  1106,  1107,   771,  1109,  1664,   286,
    1365,   731,   732,   733,   734,  1031,  1214,  1096,   234,   234,
    1027,  1543,  1083,  1677,  1284,  1284,  1552,   287,  1324,   242,
     242,  1032,   242,  1325,   547,  1804,   672,  1604,  1285,  1285,
    1286,  1286,  1089,  1111,  1678,  1818,  1819,  1419,  1287,  1287,
    1679,  1400,   288,  1119,  1288,  1288,   742,  -686,   743,  1324,
    -686,   163,  1832,   729,  1328,  1834,   299,  -686,   730,  -686,
     746,   163,   747,  -686,   427,   428,   429,   315,  -686,   672,
    1133,  -686,  1870,  1819,  1680,   670,   731,   732,   733,   734,
     729,  1650,  1652,  1010,  1502,   730,  -686,   731,   732,   733,
     734,   153,   437,   438,  1196,   811,   294,   812,  1862,   729,
    -686,   331,   295,   937,   730,   941,  1864,  1865,  -686,   945,
     947,   949,  -686,   731,   732,   733,   734,  1746,  1651,  1653,
    1397,  -686,   731,   732,   733,   734,  1217, -1004,   341,  1398,
   -1007,   342,   731,   732,   733,   734,   344,  -686,  1028,  1401,
     222,   381,  1471,   545,  1616,  1474,   735,   833,   973,   834,
    1003,   348,  1004,   801,   731,   732,   733,   734,   378,   377,
     729,  1402,   380,   389,  -686,   730,   366,   399,   504,   504,
     401,  1115,   893,  1116,  1120,   382,  -686,   383,  -686,   735,
     729,  1220,   214,   403,  1221,   730,  1222,  1115,  1538,  1271,
     404,   731,   732,   733,   734,   729,   407,   408,  1404,   384,
     730,   492,   731,   732,   733,   734,   496,   640,   644,  1405,
     499,  1365,  1365,   731,   732,   733,   734,  1805,  1559,  1365,
    1365,   506,  1365,  1365,   385,  -686,   507,   510,  1331,   729,
    1365,  1365,  1365,  1365,   730,  1365,  1365,  1365,  1365,  1365,
    1365,  1347,  1316,   511,  1317,  -686,   547,  1351,  1321,  -686,
    1322,   729,  1817,   242,   512,   232,   730,   513,  1314,   495,
    -686,  -686,  1823,  -686,   514,   735,   242,  1489,   735,  1315,
    1575,   516,   242,   729,   735,  1314,  1614,   735,   730,  1659,
    1484,  1484,  1487,   987,   529, -1026,   234,   531,   532,   458,
     234, -1026,  1876,   746,  1877,  1878, -1026,   541,   551, -1026,
     731,   732,   733,   734,   553,   555,   729,  1406,  1819,    14,
    1896,   730,   519,   567,  1513,   729,  1515,   569,  1760,   316,
     730,   527,   559,  1497,   560,  1499,   568,   731,   732,   733,
     734,   571,   729,   589,  1407, -1026,   592,   730,  1475,   602,
     603,  1473,   607, -1026,   656,   729,   731,   732,   733,   734,
     730, -1026,   317,  1408,  1422,   558,  1476,   660,   163,   561,
     662,   491,   697,   699,   232,   545,  1396,   700, -1027, -1026,
     701, -1026,  1204,   702, -1027,   703,  1558,   704,   318, -1027,
     709,   319, -1027,  1607, -1026,   710, -1026, -1026, -1026,   729,
     705,   706,   234,   729,   730,  1427,  1470,   320,   730,   504,
     214,   219,   504,   250,   142,   831,   729,   731,   732,   733,
     734,   730,   707,   719,  1409,   737,  -990,   708, -1027,   711,
     504,   504,   716,   321,  1584,   214, -1027,   731,   732,   733,
     734,   717,  1215,   735, -1027, -1026,  1762,   718,   728,   736,
    1704, -1007,   731,   732,   733,   734,  1841,  1472,  1872,  1605,
    1843,   741, -1027,   744, -1027,   713,   745,   768,  1847,   762,
     787,   720,   721,   724,   547,  1853,   799, -1027,   791, -1027,
   -1027, -1027,   770,   783,   795,   792,   731,   732,   733,   734,
    1760,  1868,   797,  1606,  1760,   794,  1564,   800,   562,   105,
     803,  1569,  1760,  1541,   804,  1849,   806,   807,   731,   732,
     733,   734, -1028,   813,   837,  1613,   838,   839, -1028,  1886,
    -431,   844,   852, -1028,  1888,  1760, -1028,   300, -1027,   853,
     731,   732,   733,   734,  1660,   107,   859,  1798,   860,   765,
     861,   862,   864,   767,   865, -1026, -1026, -1026, -1026, -1026,
   -1026, -1026,   870,  1760,   880,   874,  1620, -1026,  1760, -1026,
     875,   881, -1028,   731,   732,   733,   734,   890,  1615,   892,
   -1028,  1255,   731,   732,   733,   734,   893,   163, -1028,   899,
     898,   900,   904,   174,  1256,  1396,   964,  1788,   969,   731,
     732,   733,   734,   545,   970,   871, -1028,   302, -1028,   992,
     108,   997,   731,   732,   733,   734,   998,  1257,  1762,   208,
     109, -1028,  1762, -1028, -1028, -1028,   999,  1630,  1133,  1133,
    1762,  1258,  1001,  1850,  1470,  1005,  1022,   753, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027,   110,  1029,  1039,   746,  1043,
   -1027,  1053, -1027,  1762,  1062,  1063,   731,   732,   733,   734,
     731,   732,   733,   734,  1064,   816,   104,  1069,  1259,  1070,
     504,   504, -1028,   731,   732,   733,   734,  1072,  1078,  1079,
    1080,  1762,  1255,  1763,  1087,  1094,  1762,  1108,  1104,  1113,
    1186,   214,   214,  1193,   333,  1256,  1767,  1127,  1151,  1187,
    1195,  1683,  1203,  -696,   105,   729,  1223,  1225,  1244,  1241,
    1249,   912,  1484,  1250,  1254,  1264,   106,  1262,  1257,   163,
    1158,  1275,  1162,  1278,  1170,  1173,  1298,  1178,  1181,  1184,
    1300,   938,  1258,  1310,  1318,   943,  1320,  1333,  1341,  1121,
     107,  1326,   959,   913,  1349,  1354,  1329,   966,   967,  1330,
    1332,  1732,  1355,  1356,  1362,  1363,  1370,  1367,  1371,   458,
    1415,   977,   978,   979,   980,   981,   982,   458,  1413,  1259,
    1477,     1, -1028, -1028, -1028, -1028, -1028, -1028, -1028,  1783,
    1260,  1492,  1373,  1374, -1028,  1381, -1028,   -48,   163,     2,
    1382,   211,  1383,  1495,  1384,  1386,  1396,     3,  1493,  1387,
    1388,   527,  1389,  1390,     4,   108,     5,  1500,     6,  1391,
    1479,  1480,   214,     7,  1501,   109,  1504,  1505,  1514,  1528,
       8,  1532,  1037,  1533,  1133,   -48,  1534,  1535,   558,  1536,
    1045,  1537,  1091,  1540,     9,  1784,  1554,   -48,  1560,  1562,
     110,  1758,  1255,  1563,    10,  1763,   333,   214,  1568,  1763,
    1573,   147,  1781,  1577,   547,  1256,  1596,  1763,  1611,  1744,
    1763,   -48,  1791,  1597,  1618,   976,  1793,  1603,  1612,  1617,
     163,  -996,  1623,  1624,   145,   211,  1626,  1827,  1257,  1627,
    1763,  1260,  1634,   547,  1641,  1642,  1643,  1644,  1645,  1809,
    1646,  1647,  1258,  1648,  1649,  1654,   771,  1656,  1674,  1684,
    1769,  1687,  1689,  1690,  1686,  1688,  1691,    11,  1763,  1693,
    1694,  1695,  1703,  1763,    12,  1705,   547,    13,  1710,    14,
      15,  1709,   211,  1711,  1731,   816,   -48,  1741,   547,  1259,
    1714,   547,  1635,  1715,  1716,  1717,   -48,  1764,  1720,   547,
    1721,  1722,   208,  1811,    16,  1723,  1144,  1145,  1150,  1724,
    1812,  1159,  1160,  1163,  1725,  1171,  1174,  1176,  1179,  1182,
    1185,   -48,  1726,  1727,  1728,  1774,  1729,  1189,  1190,  1780,
    1192,  1779,  1194,   545,  1734,  1197,  1198,  1199,  1200,  1201,
    1202,  1748,  1777,   148,   495,  1796,  1759,  1792,  1801,  1807,
    1808,  1815,  1206,    17,  1816,  1209,  1821,  1820,  1828,  1839,
    1842,  1836,   545,  1838,  1470,  1835,  1824,   915,    18,    19,
    1837,   234,  1026,   729,  1858,   234,  1860,  1770,   730,  1863,
    1875,   916,  1869,  1866,  1819,  1855,  1856,   211,   211,  1885,
    1887,  1889,  1890,  1470,  1893,   545,   458,  1903,  1904,   296,
     297,  1260,   761,   498,  1481,   298,  1008,   545,   505,   763,
     545,  1000,  1350,   891,  1100,  1348,  1131,   917,   545,  1338,
    1697,  1126,   615,   253,   572,   918,  1470,  1026,   854,   549,
    1277,  1040,  1339,   919,  1067,  1344,  1696,  1676,  1470,   585,
    1133,  1470,   587,  1682,  1854,  1861,  1859,  1884,  1227,  1470,
    1060,   920,  1667,   921,  1756,  1803,   111,   950,   951,   952,
     953,   954,   955,   956,   957,  1845,   922,   989,   923,   924,
     925,  1509,   579,   593,   112,   113,   570,  1146,  1105,  1551,
    1147,  1097,   836,   151,   412,   114,   115,   778,   116,   542,
     878,   413,  1030,   313,   782,   117,   118,  1024,   119,   120,
     263,   121,  1253,   414,   122,   123,   124,   125,   126,   415,
     416,  1498,   127,   128,   857,  1825,  1730,   926,   129,   130,
     131,   933,   132,   133,   134,  1602,  1042,    46,    47,   790,
      50,  1248,  1090,  1006,   282,  1377,  1518,   417,  1327,  1417,
     418,  1778,   458,   832,   268,  1629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1375,  1376,     0,  1378,  1380,
       0,     0,     0,  1571,  1572,   419,  1385,   420,   421,     0,
       0,  1578,  1579,     0,  1581,  1582,     0,   422,   584,     0,
       0,   423,  1586,  1587,  1588,  1589,  1403,  1590,  1591,  1592,
    1593,  1594,  1595,     0,  1410,  1411,  1206,     0,     0,     0,
       0,     0,  1206,     0,  1209,  1418,   458,  1420,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   927,   928,   929,
     731,   732,   733,   734,     0,     0,     0,   972,     0,     0,
       0,     0,     0,     0,  1148,     0,   424,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   426,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,   428,   429,   430,
     225,   226,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,  1522,     0,     0,   112,   113,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   114,   441,
     104,   116,     0,     0,   442,   443,     0,   444,   445,   446,
       0,   119,   120,   447,   121,     0,     0,   122,   123,   124,
     125,   126,     0,     0,     0,   127,   128,     0,     0,     0,
       0,   129,   448,   131,     0,   449,   133,   134,   105,     0,
       0,   450,   451,     0,     0,     0,   111,   518,     0,   453,
     106,     0,  1570,   302,     0,     0,     0,     0,  1576,     0,
       0,     0,     0,  1580,   112,   113,     0,     0,     0,     0,
    1585,     0,     0,     0,   107,   114,   115,     0,   116,     0,
       0,     0,     0,     0,     0,   117,   118,     0,   119,   120,
       0,   121,     0,     0,   122,   123,   124,   125,   126,     0,
       0,     0,   127,   128,     0,  1206,  1206,     0,   129,   130,
     131,     0,   132,   133,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1625,     0,  1157,     0,     0,  1147,
       0,     0,     0,   412,     0,     0,     0,     0,     0,   108,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   109,
       0,     0,   414,     0,     0,     0,     0,     0,   415,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,     0,     0,   418,
       0,     0,   111,     0,     0,  1663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,   113,     0,     0,   419,     0,   420,   421,     0,     0,
       0,   114,   115,     0,   116,     0,   422,     0,     0,     0,
     423,   117,   118,     0,   119,   120,     0,   121,     0,     0,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
       0,     0,     0,     0,   129,   130,   131,     0,   132,   133,
     134,     0,     0,     0,     0,     0,     0,     0,  1713,     0,
       0,     0,     0,     0,  1718,  1719,     0,     0,     0,     0,
       0,     0,     0,  1148,     0,   424,     0,     0,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,  1733,     0,
       0,   426,     0,  1206,     0,     0,  1736,     0,     0,     0,
       0,  1743,     0,  1745,     0,   427,   428,   429,   430,   225,
     226,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   112,   113,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   114,   441,     0,
     116,     0,     0,   442,   443,     0,   444,   445,   446,   415,
     119,   120,   447,   121,     0,     0,   122,   123,   124,   125,
     126,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     129,   448,   131,     0,   449,   133,   134,   417,     0,     0,
     450,   451,     0,     0,  1161,  1790,   518,  1147,   453,     0,
       0,   412,   302,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,   419,     0,     0,     0,     0,
     414,     0,     0,     0,     0,   915,   415,   416,     0,     0,
       0,   729,     0,     0,     0,   458,   730,     0,     0,   916,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,     0,     0,   418,     0,     0,
       0,     0,     0,     0,     0,  1663,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   917,     0,     0,     0,     0,
       0,     0,   419,   918,   420,   421,   424,     0,     0,     0,
       0,   919,     0,     0,   422,     0,     0,   590,   423,     0,
       0,     0,   426,     0,     0,     0,  1846,     0,     0,   920,
       0,   921,     0,     0,     0,     0,   427,   428,   429,     0,
     225,   226,     0,     0,   922,     0,   923,   924,   925,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,   434,   435,   436,   437,   438,   439,   440,     0,     0,
       0,  1148,     0,   424,   442,   443,     0,     0,     0,   425,
       0,     0,     0,   447,     0,     0,     0,   985,     0,   426,
       0,     0,     0,     0,     0,   926,     0,     0,     0,     0,
       0,     0,     0,   427,   428,   429,   430,   225,   226,     0,
       0,     0,  1280,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   112,   113,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   114,   441,     0,   116,     0,
       0,   442,   443,     0,   444,   445,   446,     0,   119,   120,
     447,   121,     0,     0,   122,   123,   124,   125,   126,     0,
       0,     0,   127,   128,     0,     0,     0,     0,   129,   448,
     131,     0,   449,   133,   134,     0,     0,     0,   450,   451,
    1169,     0,     0,  1147,   518,   111,   453,   412,     0,     0,
     302,     0,     0,     0,   413,   927,   928,   929,   731,   732,
     733,   734,     0,   112,   113,     0,   414,     0,     0,     0,
       0,     0,   415,   416,   114,   115,     0,   116,     0,     0,
       0,     0,     0,     0,   117,   118,     0,   119,   120,     0,
     121,     0,     0,   122,   123,   124,   125,   126,     0,     0,
     417,   127,   128,   418,     0,     0,   111,   129,   130,   131,
       0,   132,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,   113,     0,     0,   419,     0,
     420,   421,     0,     0,     0,   114,   115,     0,   116,     0,
     422,     0,     0,     0,   423,   117,   118,     0,   119,   120,
       0,   121,     0,     0,   122,   123,   124,   125,   126,     0,
       0,     0,   127,   128,     0,     0,     0,     0,   129,   130,
     131,     0,   132,   133,   134,     0,     0,     0,     0,     0,
     986,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1148,     0,   424,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     428,   429,   430,   225,   226,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     112,   113,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   114,   441,     0,   116,     0,     0,   442,   443,     0,
     444,   445,   446,     0,   119,   120,   447,   121,     0,    14,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
       0,     0,     0,     0,   129,   448,   131,     0,   449,   133,
     134,     0,     0,    65,   450,   451,  1172,     0,     0,  1147,
     518,     0,   453,   412,    66,     0,   302,     0,     0,     0,
     413,    67,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   414,     0,    68,     0,     0,     0,   415,   416,
     112,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,     0,   116,     0,     0,     0,     0,     0,
       0,   117,   118,     0,   119,   120,   417,   121,     0,   418,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
       0,     0,     0,     0,   129,   130,   131,     0,   132,   133,
     134,    69,     0,     0,   419,     0,   420,   421,     0,     0,
    1011,     0,     0,     0,     0,     0,   422,    70,     0,     0,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    72,     0,
       0,     0,    73,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1148,     0,   424,     0,     0,     0,     0,
       0,   425,    76,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,   428,   429,   430,   225,
     226,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   112,   113,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   114,   441,     0,
     116,    77,     0,   442,   443,     0,   444,   445,   446,     0,
     119,   120,   447,   121,     0,     0,   122,   123,   124,   125,
     126,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     129,   448,   131,     0,   449,   133,   134,     0,     0,     0,
     450,   451,  1177,     0,     0,  1147,   518,     0,   453,   412,
     894,     0,   302,   617,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   895,     0,   414,     0,
     618,     0,     0,   619,   415,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   417,     0,     0,   418,     0,     0,     0,     0,
       0,     0,     0,   620,     0,     0,     0,     0,     0,   621,
     622,   623,   111,   624,   625,   626,     0,   627,     0,     0,
     419,     0,   420,   421,     0,     0,     0,     0,     0,     0,
     112,   113,   422,     0,     0,   628,   423,   629,     0,     0,
       0,   114,   115,     0,   116,     0,     0,     0,     0,     0,
       0,   117,   118,     0,   119,   120,     0,   121,     0,     0,
     122,   123,   124,   125,   126,     0,   630,     0,   127,   128,
       0,     0,     0,     0,   129,   130,   131,     0,   132,   133,
     134,     0,     0,   631,     0,     0,     0,     0,     0,  1148,
    1867,   424,   453,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,   632,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,   428,   429,   430,   225,   226,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
     633,   634,   112,   113,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   114,   441,     0,   116,   635,     0,   442,
     443,     0,   444,   445,   446,     0,   119,   120,   447,   121,
       0,     0,   122,   123,   124,   125,   126,     0,     0,     0,
     127,   128,     0,     0,     0,     0,   129,   448,   131,     0,
     449,   133,   134,     0,     0,     0,   450,   451,  1180,     0,
       0,  1147,   518,     0,   453,   412,   616,     0,   302,   617,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,   618,     0,     0,   619,
     415,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,     0,
       0,   418,     0,     0,     0,     0,     0,     0,     0,   620,
       0,     0,     0,     0,     0,   621,   622,   623,   111,   624,
     625,   626,     0,   627,     0,     0,   419,     0,   420,   421,
       0,     0,     0,     0,     0,     0,   112,   113,   422,     0,
       0,   628,   423,   629,     0,     0,     0,   114,   115,     0,
     116,     0,     0,     0,     0,     0,     0,   117,   118,     0,
     119,   120,     0,   121,     0,     0,   122,   123,   124,   125,
     126,     0,   630,     0,   127,   128,     0,     0,     0,     0,
     129,   130,   131,     0,   132,   133,   134,     0,     0,   631,
       0,     0,     0,     0,     0,  1148,     0,   424,   453,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,     0,
       0,   632,     0,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,   428,   429,
     430,   225,   226,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,  1228,     0,     0,   633,   634,   112,   113,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   114,
     441,     0,   116,   635,     0,   442,   443,     0,   444,   445,
     446,     0,   119,   120,   447,   121,     0,     0,   122,   123,
     124,   125,   126,     0,     0,     0,   127,   128,     0,     0,
       0,     0,   129,   448,   131,     0,   449,   133,   134,     0,
       0,     0,   450,   451,  1183,     0,     0,  1147,   518,     0,
     453,   412,     0,  1229,   302,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,     0,     0,     0,   415,   416,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1230,     0,
       0,     0,     0,  1231,     0,     0,     0,     0,     0,  1232,
       0,     0,     0,     0,   417,     0,     0,   418,     0,     0,
     111,     0,     0,     0,     0,     0,     0,     0,  1233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   112,   113,
       0,     0,   419,     0,   420,   421,     0,     0,     0,   114,
     115,     0,   116,     0,   422,     0,     0,     0,   423,   117,
     118,     0,   119,   120,     0,   121,     0,     0,   122,   123,
     124,   125,   126,     0,     0,     0,   127,   128,     0,     0,
       0,     0,   129,   130,   131,     0,   132,   133,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1148,     0,   424,     0,     0,     0,     0,     0,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,   428,   429,   430,   225,   226,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   112,   113,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   114,   441,     0,   116,     0,
       0,   442,   443,     0,   444,   445,   446,     0,   119,   120,
     447,   121,     0,     0,   122,   123,   124,   125,   126,     0,
       0,     0,   127,   128,     0,     0,     0,     0,   129,   448,
     131,     0,   449,   133,   134,   412,     0,     0,   450,   451,
       0,     0,   413,     0,   518,     0,   453,     0,     0,     0,
     302,     0,     0,     0,   414,     0,     0,     0,     0,     0,
     415,   416,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,   674,
       0,     0,     0,     0,     0,     0,     0,     0,   417,     0,
       0,   418,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   113,     0,     0,   419,     0,   420,   421,
       0,     0,     0,   114,   115,     0,   116,     0,   422,     0,
       0,     0,   423,   117,   118,     0,   119,   120,     0,   121,
     905,     0,   122,   123,   124,   125,   126,     0,     0,     0,
     127,   128,     0,     0,     0,     0,   129,   130,   131,     0,
     132,   133,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,     0,     0,
       0,   676,     0,     0,     0,     0,     0,   424,     0,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,   428,   429,
     430,   225,   226,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   112,   113,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   114,
     441,     0,   116,     0,     0,   442,   443,     0,   444,   445,
     446,     0,   119,   120,   447,   121,     0,     0,   122,   123,
     124,   125,   126,   906,   907,   908,   127,   128,     0,     0,
       0,     0,   129,   448,   131,     0,   449,   133,   134,   412,
       0,     0,   450,   451,     0,     0,   413,     0,   680,     0,
     453,     0,     0,     0,   302,     0,     0,     0,   414,     0,
       0,   673,     0,     0,   415,   416,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   674,     0,     0,     0,     0,     0,     0,
       0,     0,   417,     0,     0,   418,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,   113,     0,     0,
     419,     0,   420,   421,     0,     0,     0,   114,   115,     0,
     116,     0,   422,     0,     0,     0,   423,   117,   118,     0,
     119,   120,     0,   121,   675,     0,   122,   123,   124,   125,
     126,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     129,   130,   131,     0,   132,   133,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1011,     0,     0,     0,
       0,     0,     0,     0,     0,   676,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,   428,   429,   430,   225,   226,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,   112,   113,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   114,   441,     0,   116,     0,     0,   442,
     443,     0,   444,   445,   446,     0,   119,   120,   447,   121,
       0,     0,   122,   123,   677,   678,   679,     0,     0,     0,
     127,   128,     0,     0,     0,     0,   129,   448,   131,     0,
     449,   133,   134,   412,     0,     0,   450,   451,     0,     0,
     413,     0,   680,     0,   453,     0,     0,     0,   302,     0,
       0,     0,   414,     0,     0,     0,     0,     0,   415,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   674,     0,     0,
       0,     0,     0,     0,     0,     0,   417,     0,     0,   418,
       0,     0,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,   113,     0,     0,   419,     0,   420,   421,     0,     0,
       0,   114,   115,     0,   116,     0,   422,     0,     0,     0,
     423,   117,   118,     0,   119,   120,     0,   121,   901,     0,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
       0,     0,     0,     0,   129,   130,   131,     0,   132,   133,
     134,     0,     0,     0,     0,     0,     0,     0,  1619,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   676,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,   428,   429,   430,   225,
     226,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   112,   113,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   114,   441,     0,
     116,     0,     0,   442,   443,     0,   444,   445,   446,     0,
     119,   120,   447,   121,     0,     0,   122,   123,   677,   678,
     679,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     129,   448,   131,     0,   449,   133,   134,   412,     0,     0,
     450,   451,     0,     0,   413,     0,   680,     0,   453,     0,
       0,     0,   302,     0,     0,     0,   414,     0,     0,     0,
       0,     0,   415,   416,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   674,     0,     0,     0,     0,     0,     0,     0,     0,
     417,     0,     0,   418,     0,     0,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,   113,     0,     0,   419,     0,
     420,   421,     0,     0,     0,   114,   115,     0,   116,     0,
     422,     0,     0,     0,   423,   117,   118,     0,   119,   120,
       0,   121,   675,     0,   122,   123,   124,   125,   126,     0,
       0,     0,   127,   128,     0,     0,     0,     0,   129,   130,
     131,     0,   132,   133,   134,     0,     0,     0,     0,     0,
       0,     0,  1628,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   676,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     428,   429,   430,   225,   226,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     112,   113,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   114,   441,     0,   116,     0,     0,   442,   443,     0,
     444,   445,   446,     0,   119,   120,   447,   121,     0,     0,
     122,   123,   677,   678,   679,     0,     0,     0,   127,   128,
       0,     0,     0,     0,   129,   448,   131,     0,   449,   133,
     134,   412,     0,     0,   450,   451,     0,     0,   413,     0,
     680,     0,   453,     0,     0,     0,   302,     0,     0,     0,
     414,     0,     0,     0,     0,     0,   415,   416,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   674,     0,     0,     0,     0,
       0,     0,     0,     0,   417,     0,     0,   418,    80,     0,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   112,   113,
       0,     0,   419,     0,   420,   421,     0,     0,     0,   114,
     115,     0,   116,     0,   422,     0,     0,     0,   423,   117,
     118,     0,   119,   120,     0,   121,   901,     0,   122,   123,
     124,   125,   126,     0,     0,     0,   127,   128,     0,     0,
       0,     0,   129,   130,   131,     0,   132,   133,   134,     0,
       0,     0,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   676,     0,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,   930,     0,     0,     0,
       0,     0,     0,   427,   428,   429,   430,   225,   226,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,   931,
       0,     0,     0,     0,   112,   113,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   114,   441,     0,   116,     0,
     412,   442,   443,     0,   444,   445,   446,   413,   119,   120,
     447,   121,     0,     0,   122,   123,   124,   125,   126,   414,
       0,     0,   127,   128,     0,   415,   416,     0,   129,   448,
     131,     0,   449,   133,   134,   932,     0,     0,   450,   451,
       0,     0,     0,     0,   680,     0,   453,     0,     0,     0,
     302,     0,     0,   417,     0,     0,   418,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   419,     0,   420,   421,     0,     0,   112,   113,     0,
       0,     0,     0,   422,     0,     0,     0,   423,   114,   115,
       0,   116,     0,     0,     0,     0,     0,     0,   117,   118,
       0,   119,   120,     0,   121,     0,     0,   122,   123,   124,
     125,   126,     0,     0,     0,   127,   128,     0,     0,     0,
       0,   129,   130,   131,     0,   132,   133,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   427,   428,   429,   430,   225,   226,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,   112,   113,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   114,   441,     0,   116,     0,   412,
     442,   443,     0,   444,   445,   446,   413,   119,   120,   447,
     121,     0,     0,   122,   123,   124,   125,   126,   414,     0,
       0,   127,   128,     0,   415,   416,     0,   129,   448,   131,
       0,   449,   133,   134,     0,     0,     0,   450,   451,   523,
       0,     0,     0,   518,     0,   453,     0,     0,     0,   302,
       0,     0,   417,     0,     0,   418,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,   113,     0,     0,
     419,     0,   420,   421,     0,     0,     0,   114,   115,     0,
     116,     0,   422,     0,     0,     0,   423,   117,   118,     0,
     119,   120,     0,   121,     0,     0,   122,   123,   124,   125,
     126,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     129,   130,   131,     0,   132,   133,   134,     0,     0,     0,
       0,     0,   986,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,   712,     0,     0,     0,     0,
       0,   427,   428,   429,   430,   225,   226,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,   112,   113,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   114,   441,     0,   116,     0,   412,   442,
     443,     0,   444,   445,   446,   413,   119,   120,   447,   121,
       0,     0,   122,   123,   124,   125,   126,   414,     0,     0,
     127,   128,     0,   415,   416,     0,   129,   448,   131,     0,
     449,   133,   134,     0,     0,     0,   450,   451,     0,     0,
       0,     0,   518,     0,   453,     0,     0,     0,   302,     0,
       0,   417,     0,     0,   418,     0,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,   113,     0,     0,   419,
       0,   420,   421,     0,     0,     0,   114,   115,     0,   116,
       0,   422,     0,     0,     0,   423,   117,   118,     0,   119,
     120,     0,   121,     0,     0,   122,   123,   124,   125,   126,
       0,     0,     0,   127,   128,     0,     0,     0,     0,   129,
     130,   131,     0,   132,   133,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     424,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,   428,   429,   430,   225,   226,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,   112,   113,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   114,   441,     0,   116,     0,   412,   442,   443,
       0,   444,   445,   446,   413,   119,   120,   447,   121,     0,
       0,   122,   123,   124,   125,   126,   414,     0,     0,   127,
     128,     0,   415,   416,     0,   129,   448,   131,     0,   449,
     133,   134,     0,     0,     0,   450,   451,     0,     0,     0,
       0,   518,     0,   453,     0,     0,     0,   302,     0,     0,
     417,     0,     0,   418,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   419,     0,
     420,   421,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,     0,     0,   423,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   427,
     428,   429,   430,   225,   226,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     112,   113,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   114,   441,     0,   116,     0,   412,   442,   443,     0,
     444,   445,   446,   413,   119,   120,   447,   121,     0,     0,
     122,   123,   124,   125,   126,   414,     0,     0,   127,   128,
       0,   415,   416,     0,   129,   448,   131,     0,   449,   133,
     134,     0,     0,     0,   450,   451,     0,     0,     0,     0,
     518,   983,   453,     0,     0,     0,   302,     0,     0,   417,
       0,     0,   418,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   419,     0,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   424,     0,
       0,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1175,     0,     0,   427,   428,
     429,   430,   225,   226,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   112,
     113,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     114,   441,     0,   116,     0,   412,   442,   443,     0,   444,
     445,   446,   413,   119,   120,   447,   121,     0,     0,   122,
     123,   124,   125,   126,   414,     0,     0,   127,   128,     0,
     415,   416,     0,   129,   448,   131,     0,   449,   133,   134,
       0,     0,     0,   450,   451,     0,     0,     0,     0,   518,
       0,   453,     0,     0,     0,   302,     0,     0,   417,     0,
       0,   418,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   419,     0,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
       0,     0,   423,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   424,     0,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1379,     0,     0,   427,   428,   429,
     430,   225,   226,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   112,   113,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   114,
     441,     0,   116,     0,   412,   442,   443,     0,   444,   445,
     446,   413,   119,   120,   447,   121,     0,     0,   122,   123,
     124,   125,   126,   414,     0,     0,   127,   128,     0,   415,
     416,     0,   129,   448,   131,     0,   449,   133,   134,     0,
       0,     0,   450,   451,     0,     0,     0,     0,   518,     0,
     453,     0,     0,     0,   302,     0,     0,   417,     0,     0,
     418,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   419,     0,   420,   421,     0,
       0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
       0,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,   428,   429,   430,
     225,   226,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   112,   113,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   114,   441,
       0,   116,     0,   412,   442,   443,     0,   444,   445,   446,
     413,   119,   120,   447,   121,     0,     0,   122,   123,   124,
     125,   126,   414,     0,     0,   127,   128,     0,   415,   416,
       0,   129,   448,   131,     0,   449,   133,   134,     0,     0,
       0,   450,   451,     0,     0,  1742,     0,   518,     0,   453,
       0,     0,     0,   302,     0,     0,   417,     0,     0,   418,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   419,     0,   420,   421,     0,     0,
       0,     0,     0,     0,     0,     0,   422,     0,     0,     0,
     423,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,   428,   429,   430,   225,
     226,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,   112,   113,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   114,   441,     0,
     116,     0,   412,   442,   443,     0,   444,   445,   446,   413,
     119,   120,   447,   121,     0,     0,   122,   123,   124,   125,
     126,   414,     0,     0,   127,   128,     0,   415,   416,     0,
     129,   448,   131,     0,   449,   133,   134,     0,     0,     0,
     450,   451,     0,     0,     0,     0,   452,     0,   453,     0,
       0,     0,   302,     0,     0,   417,     0,     0,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   419,     0,   420,   421,     0,     0,     0,
       0,     0,     0,     0,     0,   422,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,     0,
     425,     0,     0,     0,     6,     0,     0,     0,     0,     0,
     426,  -287,     0,     0,     0,  1428,  1429,     0,  1430,     0,
       0,     0,     0,     0,   427,   428,   429,   430,   225,   226,
       0,     0,     0,     0,  1431,     0,     0,     0,   431,     0,
    1432,     0,     0,     0,     0,   112,   113,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   114,   441,     0,   116,
       0,     0,   442,   443,     0,   444,   445,   446,     0,   119,
     120,   447,   121,     0,  1225,   122,   123,   124,   125,   126,
       0,     0,     0,   127,   128,     0,     0,     0,     0,   129,
     448,   131,     0,   449,   133,   134,     0,  1433,     0,   450,
     451,     0,     6,     0,     0,   518,     0,   453,     0,  -289,
       0,   302,     0,  1428,  1429,    14,  1430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1225,     0,     0,
    1434,     0,  1431,     0,     0,     0,     0,     0,  1432,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,  1635,
       0,     0,     0,     0,     0,     6,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,  1428,  1429,     0,  1430,
       0,     0,     0,     0,     0,     0,   112,   113,     0,     0,
       0,     0,     0,     0,     0,  1431,     0,  1435,   115,     0,
     116,  1432,     0,     0,     0,  1433,     0,   117,   118,     0,
     119,   120,     0,   121,     0,     0,   122,  1436,   124,   125,
     126,     0,     0,    14,   127,   128,  1437,  1438,  1439,     0,
     129,   130,   131,     0,   132,   133,   134,     0,  1434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1433,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,   112,   113,     0,     0,     0,     0,
       0,  1434,     0,     0,     0,  1435,   115,     0,   116,     0,
       0,    16,     0,     0,     0,   117,   118,     0,   119,   120,
       0,   121,     0,     0,   122,  1436,   124,   125,   126,   430,
       0,     0,   127,   128,  1437,  1438,  1439,     0,   129,   130,
     131,     0,   132,   133,   134,     0,     0,   112,   113,     0,
       0,     0,     0,   616,     0,     0,   617,     0,  1435,   115,
       0,   116,   816,     0,     0,     0,     0,     0,   117,   118,
       0,   119,   120,   618,   121,     0,   619,   122,  1436,   124,
     125,   126,     0,     0,     0,   127,   128,  1437,  1438,  1439,
       0,   129,   130,   131,     0,   132,   133,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   616,     0,     0,   617,   620,     0,     0,     0,
       0,     0,   621,   622,   623,     0,   624,   625,   626,     0,
     627,     0,   618,     0,     0,   619,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   628,     0,
     629,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   620,     0,     0,     0,   630,
       0,   621,   622,   623,     0,   624,   625,   626,     0,   627,
       0,     0,     0,     0,     0,     0,   631,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   628,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,   632,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,   630,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,   113,   633,   634,   631,     0,     0,     0,     0,
       0,     0,   114,   115,     0,   116,     0,     0,     0,     0,
     635,     0,   117,   118,     0,   119,   120,   632,   121,     0,
       0,   122,   123,   124,   125,   126,     0,     0,     0,   127,
     128,     0,   111,     0,     0,   129,   130,   131,     0,   132,
     133,   134,     0,     0,     0,     0,     0,     0,     0,     0,
     112,   113,   633,   634,     0,     0,     0,     0,     0,     0,
       0,   114,   115,     0,   116,     0,     0,     0,     0,   635,
       0,   117,   118,     0,   119,   120,     0,   121,     0,     0,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
       0,     0,   111,     0,   129,   130,   131,     0,   132,   133,
     134,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     112,   113,   187,   188,   189,   190,   191,     0,     0,   192,
     193,   114,   115,     0,   116,     0,     0,   194,   195,     0,
       0,   117,   118,   196,   119,   120,   197,   121,   198,     0,
     122,   123,   124,   125,   126,     0,     0,     0,   127,   128,
     199,   200,   201,   202,   129,   130,   131,   203,   132,   133,
     134
};

static const short yycheck[] =
{
       9,     4,     0,   162,    73,    67,   110,    16,    57,    18,
     152,   153,   152,   153,   553,    16,    66,   390,   390,    76,
     737,   386,    65,   289,    98,   712,    66,    89,    97,   487,
     360,   718,    59,   586,   382,    95,    71,   558,    61,   529,
     993,  1297,    99,   789,    93,    72,    55,    56,    57,    58,
      59,    60,    61,  1225,  1224,   507,   358,    66,    67,    68,
      95,    70,  1011,    72,    73,  1061,    75,    76,    77,    92,
     367,   744,   304,   668,   698,   792,    85,   100,    87,    88,
      89,  1095,    91,    92,    93,    94,    87,    96,    97,    98,
      99,   100,   166,   102,   452,    96,    58,   935,   395,   723,
     508,   331,   366,   661,   144,   240,   146,  1205,    70,  1243,
     888,  1054,   602,  1211,   170,    21,  1112,   998,   145,     8,
     578,   452,   580,     8,  1117,   675,     8,     8,   586,  1559,
    1295,    61,    94,   971,   157,   144,   145,   146,   165,  1443,
     102,   150,    19,  1058,     8,   249,   543,    43,   157,    10,
    1065,  1054,  1500,   162,   133,   164,   165,   166,     8,  1074,
       8,   170,    50,    51,   207,  1054,     6,  1054,  1289,  1290,
    1263,     8,   331,  1294,   825,  1054,   795,    63,    47,    90,
      10,  1054,    50,    51,    43,  1100,    44,   518,   149,   141,
      61,   810,   235,    23,   813,    47,     8,   190,    28,   564,
     335,    26,   164,   822,    97,    85,    46,    80,    22,     8,
      23,   174,     8,   586,   586,    28,    30,    44,    62,   141,
      14,    21,    80,   196,    23,  1244,   913,   914,   108,    28,
    1821,    46,   675,    63,   500,   339,   125,   680,    18,   279,
     125,   143,   282,   125,   125,   155,    30,   612,  1513,   515,
     305,   548,   185,    43,     5,   231,    59,    60,   313,    43,
     162,   125,   881,     8,    15,   241,   315,  1437,  1438,   193,
     279,  1443,   182,   282,  1270,   125,   963,   125,    23,    94,
     289,   290,   275,    28,   164,  1876,   588,    87,   125,    22,
      44,   331,   319,  1558,   246,   139,     4,   276,    63,  1318,
      90,   189,    82,   276,   118,  1254,   315,   180,  1656,   282,
     319,   105,   275,   125,  1328,   276,  1414,  1415,    23,   359,
     191,   189,   180,    28,   393,   139,   366,   204,   368,   125,
     370,   145,     8,   373,   636,   179,   173,   230,   260,  1292,
     383,    49,   385,   276,   255,   275,  1289,  1290,   507,   680,
     359,  1294,   177,   180,   403,   905,   399,   366,  1788,   368,
    1525,   370,    23,    10,   373,   179,  1053,    28,  1249,  1250,
     728,   267,   769,   279,   401,   865,    23,   173,   736,    56,
     838,    28,  1335,   392,   393,   118,  1289,  1290,   277,   275,
     275,  1294,   401,    54,   403,   277,   277,  1312,   407,  1492,
    1289,  1290,  1289,  1290,   407,  1294,   275,  1294,   267,   174,
    1289,  1290,  1036,   277,   275,  1294,  1289,  1290,  1069,  1070,
    1039,  1294,    11,   275,  1680,    26,   180,   277,    23,   277,
     392,  1269,   272,    28,    23,   196,   192,  1056,  1057,    28,
     270,   271,   272,   273,   453,   275,   126,   196,   156,   125,
    1137,  1138,   196,  1140,  1141,     6,    45,  1076,   901,   272,
     273,   517,   905,  1633,   604,   255,   174,   507,  1119,    63,
     497,   270,   271,   272,   273,    23,     6,     3,   155,   506,
      28,   889,   196,   510,   275,   169,    90,   753,   497,   280,
     499,   500,   900,    37,   196,    46,   904,   506,   178,   259,
     208,   510,   804,    18,    48,   807,   515,   267,   517,  1813,
      19,    55,   196,   553,  1612,   276,    46,   652,   820,   275,
    1654,   282,   123,   566,  1780,   270,   271,   272,   273,    48,
    1125,   275,     9,   282,   766,    79,    55,   499,  1842,    68,
      59,    60,  1538,   845,   553,   605,   586,   811,  1221,    68,
      76,  1003,  1501,  1111,  1568,  1508,  1334,   271,   788,  1555,
      79,   169,   892,  1556,   607,   270,   271,   272,   273,   271,
     605,  1875,   277,  1829,    23,   584,   177,   586,    55,    28,
    1119,   590,   879,  1887,    43,    94,  1890,   271,   196,    66,
     117,  1278,   276,   194,  1898,   199,   200,   141,  1854,   741,
       3,   143,  1517,  1011,   117,   244,  1521,  1522,    44,   270,
     271,   272,   273,  1300,   657,   658,  1737,  1738,     6,   274,
     660,   196,   141,   270,   271,   272,   273,     4,   172,   788,
       7,    17,   176,  1041,   774,    94,    61,   196,   157,   168,
      98,  1813,  1557,   156,    80,    31,   175,  1005,   223,    23,
      79,   660,   194,   172,    28,   147,    64,   176,    46,   186,
      23,     6,   141,   271,   223,    28,   231,   232,   187,    77,
    1842,     6,    49,    76,   282,   270,   271,   272,   273,   274,
     703,   270,   271,   272,   273,   114,    23,   206,   165,    75,
     203,    28,   100,   172,   703,    23,   271,   126,     6,  1047,
      28,    46,   742,  1875,   744,    23,   746,   847,   187,   167,
      28,    46,   271,     7,  1333,  1887,    11,   103,  1890,  1889,
     729,   265,   270,   271,   272,   273,  1898,    23,   737,   277,
     199,   200,    28,   742,   761,   744,  1574,   746,    46,  1692,
      46,  1365,   196,   151,   753,  1698,   265,   141,   788,   196,
      45,  1666,   761,   762,   794,  1670,   783,   800,   785,   768,
    1506,  1063,  1064,   772,    23,  1113,     4,   230,    65,    28,
    1685,   811,    65,  1075,   783,    23,   785,  1298,    16,   839,
      28,   167,   176,   792,    23,   794,    23,  1702,    94,    28,
     276,    28,  1122,  1151,  1737,  1738,   140,   837,   838,   262,
     762,   276,   811,   196,   839,   196,     0,   130,   131,   844,
     853,    49,   118,    51,   874,   875,    10,   877,  1505,   122,
    1151,   270,   271,   272,   273,   142,   275,   862,   837,   838,
     223,  1321,   223,   118,  1737,  1738,  1326,   275,   275,   874,
     875,   158,   877,   280,  1003,    82,   855,  1400,  1737,  1738,
    1737,  1738,   855,   880,   139,   274,   275,  1215,  1737,  1738,
     145,  1191,   122,   890,  1737,  1738,   275,    61,   277,   275,
      64,   880,  1787,    23,   280,  1790,   174,    71,    28,    73,
     275,   890,   277,    77,   195,   196,   197,    79,    82,   898,
     899,    85,   274,   275,   179,   898,   270,   271,   272,   273,
      23,  1479,  1480,   277,  1262,    28,   100,   270,   271,   272,
     273,   232,   223,   224,   277,   275,   124,   277,  1833,    23,
     114,   156,   124,   699,    28,   701,  1836,  1837,   122,   705,
     706,   707,   126,   270,   271,   272,   273,  1624,  1479,  1480,
     277,   135,   270,   271,   272,   273,   969,   278,   180,   277,
     281,   192,   270,   271,   272,   273,    19,   151,  1098,   277,
     969,    98,  1228,  1003,  1422,  1231,   275,   203,   277,   205,
     275,   259,   277,  1338,   270,   271,   272,   273,   162,   266,
      23,   277,   199,    10,   178,    28,   276,    59,   997,   998,
      61,   275,   275,   277,   277,   132,   190,   134,   192,   275,
      23,   277,  1011,    79,   275,    28,   277,   275,  1310,   277,
     198,   270,   271,   272,   273,    23,    78,   190,   277,   156,
      28,   149,   270,   271,   272,   273,   172,  1400,  1400,   277,
     141,  1362,  1363,   270,   271,   272,   273,   274,  1340,  1370,
    1371,    82,  1373,  1374,   181,   239,   276,    63,  1108,    23,
    1381,  1382,  1383,  1384,    28,  1386,  1387,  1388,  1389,  1390,
    1391,  1121,   275,    61,   277,   259,  1225,  1127,   275,   263,
     277,    23,  1759,  1108,    61,  1115,    28,   122,  1087,  1119,
     274,   275,  1769,   277,   122,   275,  1121,   277,   275,  1092,
     277,   244,  1127,    23,   275,  1104,   277,   275,    28,   277,
    1242,  1243,  1244,  1820,   174,    17,  1115,   102,   188,   278,
    1119,    23,   275,   275,   277,   277,    28,   172,   123,    31,
     270,   271,   272,   273,   121,   243,    23,   277,   275,   155,
     277,    28,   301,   254,  1274,    23,  1276,   254,  1642,    84,
      28,   310,   345,  1257,   347,  1259,   255,   270,   271,   272,
     273,    30,    23,   207,   277,    67,   275,    28,  1232,   192,
     267,  1230,   112,    75,    29,    23,   270,   271,   272,   273,
      28,    83,   117,   277,  1224,   344,  1233,   276,  1187,   348,
      10,  1221,   135,   276,  1224,  1225,  1187,   276,    17,   101,
     276,   103,    63,   199,    23,   276,  1336,   276,   143,    28,
     199,   146,    31,    61,   116,   199,   118,   119,   120,    23,
     276,   276,  1221,    23,    28,  1224,  1225,   162,    28,  1228,
    1229,  1230,  1231,  1232,  1233,  1527,    23,   270,   271,   272,
     273,    28,   276,    97,   277,   281,   276,   276,    67,   276,
    1249,  1250,   276,   188,    54,  1254,    75,   270,   271,   272,
     273,   276,   275,   275,    83,   167,  1642,   276,   279,   276,
    1562,   281,   270,   271,   272,   273,  1804,  1229,    82,   277,
    1808,   267,   101,   276,   103,   444,    10,    63,  1816,   141,
     166,   450,   451,   452,  1443,  1824,   174,   116,   267,   118,
     119,   120,   275,   275,    10,   281,   270,   271,   272,   273,
    1804,  1839,   275,   277,  1808,   276,  1349,    90,    90,    44,
      90,  1354,  1816,  1316,    22,  1819,    22,    43,   270,   271,
     272,   273,    17,    10,   174,   277,   208,   136,    23,  1867,
     174,   136,    21,    28,  1872,  1839,    31,   196,   167,    90,
     270,   271,   272,   273,  1503,    80,   185,   277,   185,   518,
     138,   276,    91,   522,   276,   267,   268,   269,   270,   271,
     272,   273,   279,  1867,    61,   276,  1428,   279,  1872,   281,
     276,    10,    67,   270,   271,   272,   273,   122,   275,   149,
      75,    64,   270,   271,   272,   273,   275,  1396,    83,   121,
     128,   276,   276,   126,    77,  1396,   189,  1699,    61,   270,
     271,   272,   273,  1443,   277,    21,   101,   282,   103,    42,
     145,   174,   270,   271,   272,   273,   174,   100,  1804,  1428,
     155,   116,  1808,   118,   119,   120,   202,  1436,  1437,  1438,
    1816,   114,    60,  1819,  1443,   276,   190,   275,   267,   268,
     269,   270,   271,   272,   273,   180,   186,    10,   275,   240,
     279,   276,   281,  1839,    19,   276,   270,   271,   272,   273,
     270,   271,   272,   273,    43,    27,     6,    86,   151,    86,
    1479,  1480,   167,   270,   271,   272,   273,    22,   125,   125,
     125,  1867,    64,  1642,   156,   154,  1872,   276,   156,    98,
     108,  1500,  1501,   277,  1503,    77,  1655,   276,   276,   276,
     275,  1528,    63,    85,    44,    23,   184,    16,   107,   124,
      63,   680,  1654,    63,   275,    71,    56,   276,   100,  1528,
     920,    12,   922,   276,   924,   925,   239,   927,   928,   929,
     276,   700,   114,    10,   160,   704,   277,    10,   192,   133,
      80,   278,   711,     8,    52,    52,   277,   716,   717,   277,
     277,  1600,   277,   277,   276,   276,   276,    63,   276,   728,
     173,   730,   731,   732,   733,   734,   735,   736,    51,   151,
     192,     7,   267,   268,   269,   270,   271,   272,   273,  1683,
     263,   277,   276,   276,   279,   276,   281,     6,  1597,    25,
     276,  1600,   276,   127,   276,   276,  1597,    33,   277,   276,
     276,   770,   276,   276,    40,   145,    42,    85,    44,   276,
     275,   275,  1621,    49,    85,   155,    19,    73,   130,   145,
      56,   125,   791,   125,  1633,    44,   125,   125,   797,   125,
     799,   125,   196,   277,    70,  1684,   277,    56,    22,   276,
     180,  1639,    64,   277,    80,  1804,  1655,  1656,   277,  1808,
     277,    70,  1679,    63,  1813,    77,   277,  1816,    51,  1621,
    1819,    80,  1705,   275,    36,  1674,  1709,   277,   173,   274,
    1679,    36,   274,   276,    82,  1684,   274,  1781,   100,   274,
    1839,   263,    51,  1842,   274,    82,   274,   274,   274,  1751,
     274,   274,   114,   278,    70,   174,    10,    85,    23,    79,
     122,    80,    44,    80,   180,   180,    44,   143,  1867,   277,
     277,    94,   124,  1872,   150,   111,  1875,   153,   277,   155,
     156,   111,  1731,   277,   276,    27,   145,    61,  1887,   151,
     277,  1890,   189,   277,   277,   277,   155,   124,   277,  1898,
     277,   277,  1751,  1752,   180,   277,   915,   916,   917,   277,
    1753,   920,   921,   922,   277,   924,   925,   926,   927,   928,
     929,   180,   277,   277,   277,   275,   277,   936,   937,    86,
     939,    94,   941,  1813,   277,   944,   945,   946,   947,   948,
     949,   274,   277,   202,  1824,    79,   276,   204,   276,   274,
      82,   246,   961,   229,    82,   964,   276,   281,   122,   148,
     173,   275,  1842,  1801,  1813,   277,   276,    17,   244,   245,
     276,  1820,  1821,    23,   122,  1824,   122,   239,    28,   277,
      47,    31,   274,   277,   275,    44,   180,  1836,  1837,   277,
      50,    36,    47,  1842,   201,  1875,  1005,     0,     0,   149,
     149,   263,   513,   287,  1240,   150,   762,  1887,   290,   515,
    1890,   753,  1125,   664,   869,  1122,   898,    67,  1898,  1114,
    1546,   893,   387,    77,   356,    75,  1875,  1876,   612,   337,
    1050,   794,  1115,    83,   821,  1119,  1545,  1525,  1887,   368,
    1889,  1890,   368,  1527,  1827,  1831,  1830,  1856,   994,  1898,
     811,   101,  1509,   103,  1637,  1744,   198,   210,   211,   212,
     213,   214,   215,   216,   217,  1814,   116,   742,   118,   119,
     120,  1272,   361,   373,   216,   217,   354,     6,   872,  1324,
       9,   864,   592,    13,    13,   227,   228,   531,   230,   323,
     649,    20,   783,   155,   535,   237,   238,   770,   240,   241,
      87,   243,  1011,    32,   246,   247,   248,   249,   250,    38,
      39,  1258,   254,   255,   616,  1774,  1597,   167,   260,   261,
     262,   697,   264,   265,   266,  1396,   797,     0,     0,   543,
       0,  1006,   855,   760,   276,  1166,  1280,    66,  1104,  1213,
      69,  1674,  1151,   590,    91,  1436,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1164,  1165,    -1,  1167,  1168,
      -1,    -1,    -1,  1362,  1363,    94,  1175,    96,    97,    -1,
      -1,  1370,  1371,    -1,  1373,  1374,    -1,   106,    30,    -1,
      -1,   110,  1381,  1382,  1383,  1384,  1195,  1386,  1387,  1388,
    1389,  1390,  1391,    -1,  1203,  1204,  1205,    -1,    -1,    -1,
      -1,    -1,  1211,    -1,  1213,  1214,  1215,  1216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   268,   269,
     270,   271,   272,   273,    -1,    -1,    -1,   277,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,  1262,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,    -1,  1293,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
       6,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    44,    -1,
      -1,   270,   271,    -1,    -1,    -1,   198,   276,    -1,   278,
      56,    -1,  1361,   282,    -1,    -1,    -1,    -1,  1367,    -1,
      -1,    -1,    -1,  1372,   216,   217,    -1,    -1,    -1,    -1,
    1379,    -1,    -1,    -1,    80,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,  1414,  1415,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1433,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,   145,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,  1504,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1577,    -1,
      -1,    -1,    -1,    -1,  1583,  1584,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,  1607,    -1,
      -1,   181,    -1,  1612,    -1,    -1,  1615,    -1,    -1,    -1,
      -1,  1620,    -1,  1622,    -1,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    38,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    66,    -1,    -1,
     270,   271,    -1,    -1,     6,  1704,   276,     9,   278,    -1,
      -1,    13,   282,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    17,    38,    39,    -1,    -1,
      -1,    23,    -1,    -1,    -1,  1744,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1774,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    75,    96,    97,   165,    -1,    -1,    -1,
      -1,    83,    -1,    -1,   106,    -1,    -1,    30,   110,    -1,
      -1,    -1,   181,    -1,    -1,    -1,  1815,    -1,    -1,   101,
      -1,   103,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,   200,    -1,    -1,   116,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
     219,   220,   221,   222,   223,   224,   225,   226,    -1,    -1,
      -1,   163,    -1,   165,   233,   234,    -1,    -1,    -1,   171,
      -1,    -1,    -1,   242,    -1,    -1,    -1,    39,    -1,   181,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,   271,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,
       6,    -1,    -1,     9,   276,   198,   278,    13,    -1,    -1,
     282,    -1,    -1,    -1,    20,   267,   268,   269,   270,   271,
     272,   273,    -1,   216,   217,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      66,   254,   255,    69,    -1,    -1,   198,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,   155,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    37,   270,   271,     6,    -1,    -1,     9,
     276,    -1,   278,    13,    48,    -1,   282,    -1,    -1,    -1,
      20,    55,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    68,    -1,    -1,    -1,    38,    39,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    66,   243,    -1,    69,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,   125,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
     276,    -1,    -1,    -1,    -1,    -1,   106,   141,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,   176,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,   265,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
     270,   271,     6,    -1,    -1,     9,   276,    -1,   278,    13,
      18,    -1,   282,    21,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    32,    -1,
      38,    -1,    -1,    41,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,   198,    91,    92,    93,    -1,    95,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   106,    -1,    -1,   113,   110,   115,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,   144,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,   163,
     276,   165,   278,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
     218,   219,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,   235,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,   270,   271,     6,    -1,
      -1,     9,   276,    -1,   278,    13,    18,    -1,   282,    21,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    38,    -1,    -1,    41,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,   198,    91,
      92,    93,    -1,    95,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   106,    -1,
      -1,   113,   110,   115,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,   144,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,   163,    -1,   165,   278,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    71,    -1,    -1,   218,   219,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,   235,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,   270,   271,     6,    -1,    -1,     9,   276,    -1,
     278,    13,    -1,   141,   282,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    13,    -1,    -1,   270,   271,
      -1,    -1,    20,    -1,   276,    -1,   278,    -1,    -1,    -1,
     282,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
     118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    13,
      -1,    -1,   270,   271,    -1,    -1,    20,    -1,   276,    -1,
     278,    -1,    -1,    -1,   282,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    13,    -1,    -1,   270,   271,    -1,    -1,
      20,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,   118,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    13,    -1,    -1,
     270,   271,    -1,    -1,    20,    -1,   276,    -1,   278,    -1,
      -1,    -1,   282,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,   118,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    13,    -1,    -1,   270,   271,    -1,    -1,    20,    -1,
     276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,   196,    -1,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,   118,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,   108,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
      -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
     262,    -1,   264,   265,   266,   164,    -1,    -1,   270,   271,
      -1,    -1,    -1,    -1,   276,    -1,   278,    -1,    -1,    -1,
     282,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    13,
     233,   234,    -1,   236,   237,   238,    20,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,
      -1,   254,   255,    -1,    38,    39,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,   272,
      -1,    -1,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
     234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
     254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,
      -1,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,   270,   271,    -1,    -1,    -1,
      -1,   276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,    -1,    -1,    -1,
     276,   277,   278,    -1,    -1,    -1,   282,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,
     197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
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
      -1,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,
     198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,
     238,    20,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,
      39,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,   270,   271,    -1,    -1,    -1,    -1,   276,    -1,
     278,    -1,    -1,    -1,   282,    -1,    -1,    66,    -1,    -1,
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
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,   270,   271,    -1,    -1,   274,    -1,   276,    -1,   278,
      -1,    -1,    -1,   282,    -1,    -1,    66,    -1,    -1,    69,
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
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
     270,   271,    -1,    -1,    -1,    -1,   276,    -1,   278,    -1,
      -1,    -1,   282,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
     181,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,   209,    -1,
      80,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    16,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   137,    -1,   270,
     271,    -1,    44,    -1,    -1,   276,    -1,   278,    -1,    51,
      -1,   282,    -1,    55,    56,   155,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
     170,    -1,    74,    -1,    -1,    -1,    -1,    -1,    80,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,   227,   228,    -1,
     230,    80,    -1,    -1,    -1,   137,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,   155,   254,   255,   256,   257,   258,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,   180,    -1,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,   198,
      -1,    -1,   254,   255,   256,   257,   258,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    21,    -1,   227,   228,
      -1,   230,    27,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    38,   243,    -1,    41,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    21,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,   144,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
     235,    -1,   237,   238,    -1,   240,   241,   183,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,   198,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,   198,    -1,   260,   261,   262,    -1,   264,   265,
     266,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,    -1,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
      -1,   237,   238,   239,   240,   241,   242,   243,   244,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266
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
case 458:
{ yyval = NULL; }
    break;
case 459:
{ yyval = NULL; }
    break;
case 460:
{ yyval = NULL; }
    break;
case 461:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 462:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 463:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 464:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 465:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 466:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 467:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 468:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 469:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 470:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 471:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 473:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 474:
{ yyval = yyvsp[0]; }
    break;
case 475:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 476:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 477:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 478:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 479:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 481:
{ yyval = NULL; }
    break;
case 483:
{ yyval = NULL; }
    break;
case 484:
{ yyval = yyvsp[0]; }
    break;
case 485:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 486:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 487:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 488:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 489:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 490:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 491:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 492:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 493:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 494:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 503:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 505:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 506:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 507:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 513:
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
case 514:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 516:
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
case 517:
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
case 518:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 526:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 527:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 528:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 529:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 530:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 546:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 547:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 548:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 549:
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
case 550:
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
case 553:
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
case 554:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 557:
{ yyval = yyvsp[-1]; }
    break;
case 558:
{ yyval = 0; }
    break;
case 562:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 563:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 564:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 568:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 569:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 570:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 571:
{ yyval = 0; }
    break;
case 572:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 575:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 576:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 579:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 580:
{ yyval = NULL; }
    break;
case 582:
{ yyval = NULL; }
    break;
case 583:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 585:
{ yyval = NULL; }
    break;
case 587:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 592:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 593:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 594:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 595:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 596:
{ yyval = yyvsp[0];}
    break;
case 598:
{ yyval = yyvsp[0];}
    break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 602:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 603:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 604:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 605:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 606:
{ yyval = 0; }
    break;
case 607:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 608:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 609:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 610:
{ yyval = (dsql_nod*) 0; }
    break;
case 611:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 612:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 614:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 615:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 616:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 617:
{ yyval = 0; }
    break;
case 619:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 620:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;
case 621:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 622:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;
case 623:
{ yyval = NULL; }
    break;
case 624:
{ yyval = yyvsp[0]; }
    break;
case 625:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 627:
{ yyval = NULL; }
    break;
case 628:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 629:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 630:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 631:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;
case 632:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 633:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 635:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 636:
{ lex.limit_clause = true; }
    break;
case 637:
{ lex.limit_clause = false; }
    break;
case 638:
{ lex.first_detection = true; }
    break;
case 639:
{ lex.first_detection = false; }
    break;
case 640:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 641:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 642:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 643:
{ yyval = 0; }
    break;
case 644:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 645:
{ yyval = yyvsp[-2]; }
    break;
case 646:
{ yyval = yyvsp[-1]; }
    break;
case 647:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 648:
{ yyval = yyvsp[-1]; }
    break;
case 649:
{ yyval = yyvsp[0]; }
    break;
case 650:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 651:
{ yyval = 0; }
    break;
case 652:
{ yyval = make_list (yyvsp[0]); }
    break;
case 653:
{ yyval = 0; }
    break;
case 655:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 657:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 660:
{ yyval = make_list (yyvsp[0]); }
    break;
case 662:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 667:
{ yyval = yyvsp[-1]; }
    break;
case 668:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 670:
{ yyval = NULL; }
    break;
case 671:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 672:
{ yyval = NULL; }
    break;
case 674:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 679:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 680:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 683:
{ yyval = yyvsp[0]; }
    break;
case 684:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 685:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 686:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 687:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 688:
{ yyval = NULL; }
    break;
case 690:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 691:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 692:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 693:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 695:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 696:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 699:
{ yyval = make_list (yyvsp[0]); }
    break;
case 700:
{ yyval = NULL; }
    break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 704:
{ yyval = yyvsp[0]; }
    break;
case 705:
{ yyval = NULL; }
    break;
case 706:
{ yyval = yyvsp[0]; }
    break;
case 707:
{ yyval = NULL; }
    break;
case 708:
{ yyval = yyvsp[0]; }
    break;
case 709:
{ yyval = NULL; }
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
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 727:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 728:
{ yyval = 0; }
    break;
case 729:
{ yyval = make_list (yyvsp[0]); }
    break;
case 730:
{ yyval = 0; }
    break;
case 732:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 733:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 734:
{ yyval = 0; }
    break;
case 735:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 736:
{ yyval = 0; }
    break;
case 737:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 738:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 739:
{ yyval = yyvsp[-1]; }
    break;
case 740:
{ yyval = 0; }
    break;
case 741:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 743:
{ yyval = NULL; }
    break;
case 744:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 745:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 748:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 749:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 750:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 753:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 754:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 757:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 758:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 759:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 760:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 762:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 766:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 767:
{ yyval = yyvsp[0]; }
    break;
case 769:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 772:
{ yyval = NULL; }
    break;
case 773:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 777:
{ yyval = NULL; }
    break;
case 778:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 780:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 782:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 783:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 788:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 790:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 799:
{ yyval = yyvsp[-1]; }
    break;
case 800:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 812:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 813:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 815:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 817:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 818:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 819:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 820:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 821:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 822:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 823:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 825:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 826:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 827:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 828:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 829:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 830:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 831:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 832:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 839:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 842:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 844:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 846:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 848:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 850:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 852:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 854:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 855:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 856:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 859:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 860:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 861:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 863:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 865:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 866:
{ yyval = yyvsp[-1]; }
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
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
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
case 895:
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
case 896:
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
case 897:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 898:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 900:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 902:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 905:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 909:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 910:
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
case 911:
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
case 912:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 913:
{ yyval = make_parameter (); }
    break;
case 914:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 915:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 916:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 918:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 919:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 920:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 921:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 922:
{ yyval = yyvsp[0]; }
    break;
case 923:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 925:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 926:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 927:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 928:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 929:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 931:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 932:
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
case 948:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 950:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 951:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 952:
{ yyval = yyvsp[0]; }
    break;
case 953:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 954:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 955:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 956:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 959:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 960:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
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
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 976:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 979:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 989:
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

