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



#define	YYFINAL		1918
#define	YYFLAG		-32768
#define	YYNTBASE	283

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 520 ? yytranslate[x] : 723)

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
     300,   302,   304,   307,   310,   318,   321,   324,   327,   330,
     333,   336,   339,   342,   345,   348,   351,   354,   357,   360,
     363,   368,   371,   374,   377,   380,   383,   386,   389,   391,
     392,   394,   396,   403,   410,   412,   414,   415,   416,   418,
     419,   424,   425,   427,   429,   432,   441,   443,   444,   448,
     449,   450,   452,   454,   457,   459,   461,   463,   466,   473,
     475,   477,   481,   482,   484,   486,   487,   489,   491,   494,
     498,   503,   506,   509,   513,   514,   516,   518,   521,   523,
     528,   532,   536,   538,   539,   541,   543,   546,   550,   555,
     556,   558,   561,   562,   564,   566,   572,   578,   582,   585,
     586,   588,   592,   594,   596,   603,   607,   610,   617,   620,
     622,   625,   628,   631,   632,   634,   636,   639,   640,   643,
     644,   646,   648,   650,   652,   654,   656,   657,   659,   661,
     664,   667,   670,   676,   678,   681,   685,   688,   691,   692,
     694,   696,   698,   700,   704,   709,   718,   723,   724,   731,
     733,   735,   738,   741,   742,   746,   750,   752,   755,   758,
     761,   770,   779,   788,   797,   801,   802,   807,   808,   810,
     814,   819,   821,   825,   828,   832,   836,   838,   840,   841,
     843,   846,   851,   853,   855,   859,   861,   862,   865,   867,
     874,   876,   878,   882,   884,   887,   888,   890,   893,   896,
     899,   902,   905,   908,   911,   914,   917,   920,   923,   926,
     929,   932,   935,   938,   941,   943,   945,   947,   949,   952,
     956,   958,   962,   971,   981,   987,   996,  1003,  1007,  1008,
    1012,  1015,  1017,  1019,  1023,  1027,  1035,  1038,  1039,  1041,
    1043,  1046,  1050,  1051,  1053,  1056,  1061,  1063,  1067,  1070,
    1073,  1076,  1078,  1080,  1082,  1084,  1087,  1090,  1096,  1097,
    1103,  1105,  1109,  1110,  1113,  1118,  1119,  1127,  1131,  1132,
    1134,  1138,  1142,  1150,  1158,  1159,  1160,  1161,  1162,  1166,
    1167,  1177,  1187,  1189,  1191,  1192,  1195,  1197,  1199,  1201,
    1203,  1205,  1209,  1213,  1217,  1221,  1225,  1229,  1235,  1241,
    1247,  1253,  1259,  1265,  1268,  1269,  1274,  1277,  1280,  1284,
    1287,  1290,  1294,  1298,  1301,  1304,  1308,  1315,  1317,  1320,
    1325,  1329,  1332,  1335,  1338,  1341,  1345,  1347,  1351,  1355,
    1359,  1362,  1365,  1370,  1375,  1381,  1383,  1385,  1387,  1389,
    1391,  1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,
    1411,  1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,
    1431,  1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,  1450,
    1453,  1456,  1458,  1460,  1462,  1463,  1466,  1469,  1474,  1479,
    1485,  1486,  1488,  1491,  1494,  1499,  1503,  1506,  1509,  1517,
    1519,  1520,  1522,  1523,  1526,  1529,  1532,  1535,  1538,  1541,
    1544,  1547,  1550,  1554,  1557,  1560,  1563,  1566,  1568,  1570,
    1572,  1574,  1579,  1585,  1587,  1591,  1593,  1597,  1599,  1602,
    1604,  1606,  1608,  1610,  1612,  1614,  1616,  1618,  1620,  1622,
    1624,  1629,  1634,  1641,  1647,  1651,  1652,  1655,  1658,  1659,
    1663,  1664,  1669,  1671,  1677,  1682,  1684,  1689,  1691,  1694,
    1697,  1699,  1701,  1703,  1706,  1709,  1712,  1715,  1716,  1720,
    1726,  1728,  1730,  1733,  1737,  1739,  1742,  1746,  1747,  1749,
    1751,  1753,  1759,  1765,  1772,  1774,  1776,  1778,  1781,  1786,
    1788,  1789,  1795,  1797,  1798,  1802,  1805,  1807,  1808,  1811,
    1812,  1814,  1815,  1819,  1821,  1822,  1824,  1827,  1829,  1831,
    1833,  1835,  1838,  1841,  1843,  1846,  1850,  1852,  1854,  1858,
    1862,  1864,  1867,  1871,  1873,  1876,  1877,  1880,  1882,  1884,
    1885,  1887,  1889,  1891,  1895,  1898,  1902,  1903,  1905,  1909,
    1914,  1918,  1922,  1923,  1926,  1927,  1930,  1931,  1935,  1939,
    1943,  1945,  1950,  1955,  1957,  1967,  1968,  1969,  1970,  1971,
    1975,  1978,  1980,  1981,  1985,  1991,  1995,  1998,  2004,  2007,
    2009,  2011,  2013,  2015,  2017,  2021,  2023,  2027,  2029,  2030,
    2033,  2035,  2039,  2041,  2043,  2045,  2047,  2051,  2058,  2060,
    2061,  2065,  2066,  2068,  2072,  2074,  2076,  2078,  2083,  2089,
    2095,  2097,  2099,  2102,  2107,  2112,  2115,  2119,  2120,  2122,
    2125,  2127,  2129,  2132,  2135,  2138,  2139,  2141,  2142,  2146,
    2147,  2149,  2153,  2155,  2158,  2159,  2162,  2163,  2166,  2167,
    2172,  2174,  2177,  2179,  2181,  2182,  2184,  2188,  2191,  2193,
    2195,  2198,  2200,  2205,  2209,  2211,  2215,  2220,  2221,  2225,
    2226,  2228,  2232,  2236,  2238,  2240,  2241,  2243,  2245,  2250,
    2251,  2254,  2259,  2260,  2269,  2275,  2277,  2279,  2287,  2292,
    2294,  2296,  2305,  2311,  2316,  2318,  2322,  2326,  2328,  2336,
    2344,  2350,  2354,  2355,  2357,  2359,  2361,  2364,  2365,  2367,
    2369,  2371,  2372,  2376,  2378,  2382,  2384,  2385,  2389,  2391,
    2395,  2397,  2401,  2405,  2407,  2409,  2413,  2415,  2418,  2420,
    2424,  2428,  2430,  2433,  2437,  2441,  2445,  2449,  2451,  2455,
    2458,  2460,  2462,  2464,  2466,  2468,  2470,  2472,  2474,  2476,
    2478,  2480,  2484,  2488,  2492,  2496,  2500,  2504,  2508,  2512,
    2519,  2526,  2533,  2540,  2547,  2554,  2561,  2568,  2575,  2582,
    2589,  2596,  2603,  2610,  2617,  2624,  2626,  2628,  2634,  2641,
    2647,  2654,  2658,  2663,  2669,  2676,  2680,  2685,  2689,  2694,
    2698,  2703,  2708,  2714,  2719,  2724,  2728,  2733,  2735,  2737,
    2739,  2741,  2743,  2745,  2747,  2751,  2755,  2757,  2759,  2761,
    2763,  2765,  2767,  2769,  2771,  2773,  2775,  2778,  2781,  2785,
    2789,  2793,  2797,  2801,  2805,  2809,  2813,  2815,  2817,  2819,
    2821,  2825,  2827,  2829,  2831,  2833,  2835,  2837,  2842,  2844,
    2848,  2850,  2853,  2855,  2857,  2859,  2861,  2863,  2865,  2867,
    2870,  2873,  2876,  2878,  2880,  2882,  2884,  2886,  2888,  2890,
    2892,  2894,  2896,  2899,  2901,  2904,  2906,  2908,  2910,  2912,
    2914,  2917,  2919,  2921,  2923,  2925,  2930,  2936,  2942,  2948,
    2954,  2960,  2966,  2972,  2978,  2984,  2990,  2992,  2999,  3001,
    3006,  3014,  3017,  3018,  3023,  3027,  3034,  3036,  3038,  3045,
    3054,  3061,  3063,  3065,  3070,  3077,  3082,  3088,  3092,  3098,
    3103,  3109,  3111,  3113,  3115,  3120,  3127,  3129,  3131,  3133,
    3135,  3137,  3139,  3141,  3143,  3145,  3146,  3148,  3149,  3151,
    3153,  3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,
    3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,
    3193,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229,  3231,
    3233,  3235,  3237,  3239,  3241,  3243,  3245,  3247,  3249,  3251,
    3253,  3255,  3257,  3259
};
static const short yyrhs[] =
{
     284,     0,   284,   274,     0,   467,     0,   619,     0,   521,
       0,   319,     0,   306,     0,   609,     0,   489,     0,   285,
       0,   608,     0,   445,     0,   448,     0,   321,     0,   323,
       0,   294,     0,   522,     0,   515,     0,   544,     0,   513,
       0,   612,     0,    40,   666,     0,    70,   287,   122,   286,
     584,   174,   297,   291,     0,    70,   289,   122,   141,   293,
     174,   297,   291,     0,    70,   302,   174,   304,   292,     0,
     172,     0,     0,     6,     0,     6,   140,     0,   288,     0,
     290,     0,   288,   275,   290,     0,    56,     0,   155,     0,
      80,     0,    44,     0,   180,   625,     0,   145,   625,     0,
     192,    70,   124,     0,     0,   192,   202,   124,     0,     0,
     712,     0,   150,   295,   287,   122,   286,   584,    63,   297,
       0,   150,   295,   289,   122,   141,   293,    63,   297,     0,
     150,   296,   302,    63,   304,     0,    70,   124,    61,     0,
       0,   202,   124,    61,     0,     0,   298,     0,   300,     0,
     299,     0,   298,   275,   299,     0,   298,   275,   301,     0,
     300,   275,   299,     0,   141,   712,     0,   176,   716,     0,
     187,   719,     0,   206,   713,     0,   301,     0,   300,   275,
     301,     0,   717,     0,   181,   717,     0,    71,   717,     0,
     303,     0,   302,   275,   303,     0,   713,     0,   305,     0,
     304,   275,   305,     0,   717,     0,   181,   717,     0,    42,
     307,     0,    60,   317,     0,    59,    65,   308,     0,   697,
     310,   149,   314,    52,   665,   111,   665,     0,   496,     0,
      18,     0,    34,   276,   669,   277,   502,     0,     0,   311,
       0,   276,   311,   277,     0,   312,     0,   311,   275,   312,
       0,   377,   309,   313,     0,     0,    19,   230,     0,    19,
     262,     0,    94,     0,   315,     0,   276,   315,   277,     0,
     377,   309,   316,     0,   133,   669,     0,     0,    19,    97,
       0,    19,   230,     0,   204,     0,    19,   230,   204,     0,
     706,    78,   318,   128,   318,    52,   665,   111,   665,     0,
     698,     0,   666,     0,    33,   320,     0,    55,   325,     0,
     329,   604,    79,   709,   122,   584,   330,     0,   141,   396,
       0,   172,   364,     0,   176,   459,     0,   187,   452,     0,
      68,   346,     0,   265,   346,     0,    37,   348,     0,    48,
     337,     0,   157,   331,     0,   206,   347,     0,   229,   322,
       0,   141,   397,     0,   172,   365,     0,   187,   453,     0,
      55,   326,     0,    33,   125,     7,   324,     0,   141,   398,
       0,   176,   460,     0,    55,   327,     0,   705,   665,     0,
     705,   665,     0,   705,   665,     0,   705,   665,     0,   179,
       0,     0,   627,     0,   626,     0,   371,   276,   456,   654,
     457,   277,     0,   669,   332,   333,   665,   334,   335,     0,
     105,     0,    14,     0,     0,     0,    29,     0,     0,    98,
     349,   672,   363,     0,     0,   336,     0,   357,     0,   336,
     357,     0,   374,   338,   491,   456,   339,   457,   340,   373,
       0,    10,     0,     0,    43,   456,   379,     0,     0,     0,
     341,     0,   342,     0,   341,   342,     0,   343,     0,   344,
       0,   345,     0,   118,    94,     0,   456,    22,   276,   634,
     277,   457,     0,   708,     0,   713,     0,   350,   351,   354,
       0,     0,   267,     0,   665,     0,     0,   352,     0,   353,
       0,   352,   353,     0,   132,   349,   669,     0,    98,   349,
     672,   363,     0,   181,   665,     0,   134,   665,     0,   156,
     112,   665,     0,     0,   355,     0,   356,     0,   355,   356,
       0,   357,     0,    43,    21,   156,   699,     0,   255,    90,
     665,     0,   358,   665,   359,     0,    90,     0,     0,   360,
       0,   361,     0,   360,   361,     0,   167,   362,   672,     0,
      98,   349,   672,   363,     0,     0,    12,     0,    12,   130,
       0,     0,   130,     0,   131,     0,   584,   366,   276,   367,
     277,     0,   584,   366,   276,   367,   277,     0,    59,    90,
     665,     0,    59,   665,     0,     0,   368,     0,   367,   275,
     368,     0,   369,     0,   384,     0,   374,   372,   339,   457,
     380,   373,     0,   374,   492,   370,     0,   374,   370,     0,
     371,   276,   456,   654,   457,   277,     0,    27,    19,     0,
      27,     0,   491,   456,     0,   632,   454,     0,    23,   700,
       0,     0,   632,     0,   632,     0,   377,   491,     0,     0,
      43,   379,     0,     0,   658,     0,   662,     0,   663,     0,
     664,     0,   696,     0,   655,     0,     0,   381,     0,   382,
       0,   381,   382,     0,   385,   383,     0,   118,    94,     0,
     145,   584,   625,   392,   390,     0,   391,     0,   179,   390,
       0,   139,    86,   390,     0,   385,   386,     0,    30,   702,
       0,     0,   387,     0,   388,     0,   389,     0,   391,     0,
     179,   626,   390,     0,   139,    86,   626,   390,     0,    62,
      86,   626,   145,   584,   625,   392,   390,     0,   239,   604,
      79,   709,     0,     0,   456,    22,   276,   634,   277,   457,
       0,   393,     0,   394,     0,   394,   393,     0,   393,   394,
       0,     0,   122,   180,   395,     0,   122,    44,   395,     0,
     203,     0,   156,    43,     0,   156,    94,     0,   117,   201,
       0,   712,   400,   401,    10,   454,   407,   416,   457,     0,
     712,   400,   401,    10,   454,   407,   416,   457,     0,   712,
     400,   401,    10,   454,   407,   416,   457,     0,   712,   400,
     401,    10,   454,   407,   416,   457,     0,   276,   402,   277,
       0,     0,   149,   276,   404,   277,     0,     0,   403,     0,
     402,   275,   403,     0,   375,   492,   406,   457,     0,   405,
       0,   404,   275,   405,     0,   375,   492,     0,    43,   454,
     379,     0,   267,   454,   379,     0,   454,     0,   408,     0,
       0,   409,     0,   408,   409,     0,    42,   412,   410,   274,
       0,   411,     0,   414,     0,   374,   492,   413,     0,   184,
       0,     0,   267,   379,     0,   378,     0,   703,    36,    61,
     276,   544,   277,     0,   419,     0,   416,     0,    16,   417,
      51,     0,   418,     0,   418,   436,     0,     0,   415,     0,
     418,   415,     0,   617,   274,     0,   608,   274,     0,   612,
     274,     0,   609,   274,     0,   429,   274,     0,   445,   274,
       0,   422,   274,     0,   425,   274,     0,   618,   274,     0,
     420,   274,     0,   421,   274,     0,   427,   274,     0,   440,
     274,     0,   434,   274,     0,   170,   274,     0,    58,   274,
       0,   426,     0,   432,     0,   423,     0,   424,     0,    55,
     705,     0,    55,   705,   654,     0,    55,     0,    56,   246,
     654,     0,   433,    61,   544,    82,   431,   435,    47,   415,
       0,   433,    61,    56,   246,   654,    82,   431,    47,   415,
       0,    56,   246,   654,    82,   431,     0,    74,   276,   634,
     277,   173,   415,    50,   415,     0,    74,   276,   634,   277,
     173,   415,     0,   137,   654,   428,     0,     0,   544,    82,
     431,     0,   278,   718,     0,   430,     0,   631,     0,   431,
     275,   631,     0,   431,   275,   430,     0,   433,   191,   276,
     634,   277,    47,   415,     0,   711,   278,     0,     0,   227,
       0,   247,     0,   247,   711,     0,    10,    36,   703,     0,
       0,   437,     0,   436,   437,     0,   189,   438,    47,   415,
       0,   439,     0,   438,   275,   439,     0,   165,   666,     0,
      66,   707,     0,    55,   705,     0,     9,     0,   441,     0,
     443,     0,   442,     0,   256,   703,     0,   257,   703,     0,
     258,   444,   703,    82,   431,     0,     0,    56,   141,   712,
     446,   447,     0,   657,     0,   276,   657,   277,     0,     0,
     148,   431,     0,   148,   276,   431,   277,     0,     0,    56,
     260,   449,   401,    10,   407,   416,     0,   276,   450,   277,
       0,     0,   451,     0,   450,   275,   451,     0,   405,   267,
     661,     0,   719,   625,    10,   454,   548,   458,   455,     0,
     719,   625,    10,   454,   548,   458,   455,     0,     0,     0,
       0,     0,   192,    22,   124,     0,     0,   716,    61,   584,
     461,   462,   465,   456,   466,   457,     0,   716,    61,   584,
     461,   462,   465,   456,   466,   457,     0,     3,     0,    76,
       0,     0,   463,   464,     0,    15,     0,     5,     0,    80,
       0,   180,     0,    44,     0,    80,   125,   180,     0,    80,
     125,    44,     0,   180,   125,    80,     0,   180,   125,    44,
       0,    44,   125,    80,     0,    44,   125,   180,     0,    80,
     125,   180,   125,    44,     0,    80,   125,    44,   125,   180,
       0,   180,   125,    80,   125,    44,     0,   180,   125,    44,
     125,    80,     0,    44,   125,    80,   125,   180,     0,    44,
     125,   180,   125,    80,     0,   136,   667,     0,     0,    10,
     456,   407,   416,     0,     7,   468,     0,    55,   328,     0,
     172,   584,   473,     0,   176,   486,     0,   141,   399,     0,
      37,   483,   484,     0,    48,   475,   471,     0,    79,   481,
       0,   265,   482,     0,    43,   456,   379,     0,    22,   456,
     276,   634,   277,   457,     0,   472,     0,   471,   472,     0,
     156,   456,   469,   457,     0,     4,    30,   470,     0,     4,
     345,     0,    49,    43,     0,    49,    30,     0,   174,   632,
       0,   208,   377,   492,     0,   474,     0,   473,   275,   474,
       0,    49,   632,   480,     0,    49,    30,   702,     0,     4,
     369,     0,     4,   384,     0,   477,   632,   136,   669,     0,
     477,   475,   174,   632,     0,   477,   479,   208,   478,   457,
       0,   476,     0,   721,     0,   202,     0,   207,     0,   209,
       0,   210,     0,   211,     0,   212,     0,   213,     0,   214,
       0,   215,     0,   218,     0,   219,     0,   220,     0,   221,
       0,   222,     0,   225,     0,   226,     0,   229,     0,   233,
       0,   234,     0,   235,     0,   236,     0,   245,     0,   242,
       0,   244,     0,   256,     0,   257,     0,   258,     0,   259,
       0,   239,     0,   263,     0,     7,     0,     7,   207,     0,
     492,   456,     0,   632,   454,     0,   632,     0,   205,     0,
     203,     0,     0,   709,     3,     0,   709,    76,     0,   708,
     266,   192,   671,     0,   708,   266,   192,   223,     0,   708,
     266,   192,   271,   223,     0,     0,   485,     0,   484,   485,
       0,     4,   336,     0,     4,   255,    90,   665,     0,    49,
     255,    90,     0,    16,   254,     0,    51,   254,     0,   716,
     461,   487,   465,   456,   488,   457,     0,   462,     0,     0,
     466,     0,     0,    49,   490,     0,    55,   705,     0,    79,
     709,     0,   141,   712,     0,   172,   715,     0,   176,   716,
       0,   187,   719,     0,    60,   706,     0,    48,   704,     0,
      59,    65,   697,     0,   157,   669,     0,   206,   713,     0,
      68,   708,     0,   265,   708,     0,   492,     0,   493,     0,
     496,     0,   499,     0,   497,   279,   494,   280,     0,   504,
     279,   494,   280,   502,     0,   495,     0,   494,   275,   495,
       0,   671,     0,   671,   278,   671,     0,   497,     0,   504,
     502,     0,   503,     0,   508,     0,   511,     0,   235,     0,
     498,     0,   161,     0,    38,     0,   218,     0,   219,     0,
      81,     0,    92,     0,    18,   501,   500,   502,     0,    18,
     276,   670,   277,     0,    18,   276,   670,   275,   666,   277,
       0,    18,   276,   275,   666,   277,     0,   154,   160,   670,
       0,     0,   169,   666,     0,   169,   698,     0,     0,    21,
     156,   699,     0,     0,   507,   276,   669,   277,     0,   507,
       0,   507,   185,   276,   669,   277,     0,   506,   276,   669,
     277,     0,   506,     0,   505,   276,   669,   277,     0,   183,
       0,    21,   185,     0,    87,   185,     0,    21,     0,    87,
       0,   115,     0,   113,    21,     0,   113,    87,     0,    95,
     509,     0,   510,   509,     0,     0,   276,   671,   277,     0,
     276,   671,   275,   671,   277,     0,    41,     0,    88,     0,
      91,   512,     0,    93,    91,   512,     0,   144,     0,    89,
     138,     0,   276,   667,   277,     0,     0,   526,     0,   514,
       0,   543,     0,   156,    68,   708,   174,   671,     0,   156,
      68,   708,   174,   223,     0,   156,    68,   708,   174,   271,
     223,     0,   516,     0,   517,     0,   519,     0,   244,   720,
       0,   245,   244,   720,   518,     0,   123,     0,     0,   153,
     523,   174,   520,   720,     0,   244,     0,     0,    25,   523,
     524,     0,   153,   523,     0,   193,     0,     0,   147,   525,
       0,     0,   162,     0,     0,   156,   175,   527,     0,   528,
       0,     0,   529,     0,   528,   529,     0,   530,     0,   531,
       0,   532,     0,   536,     0,   143,   123,     0,   143,   194,
       0,   188,     0,   117,   188,     0,    84,   102,   533,     0,
     533,     0,   534,     0,   143,   177,   535,     0,   143,    26,
     535,     0,   162,     0,   162,   172,     0,   162,   172,   166,
       0,   186,     0,   117,   186,     0,     0,   146,   539,     0,
     158,     0,   142,     0,     0,   143,     0,   194,     0,   540,
       0,   539,   275,   540,     0,   542,   541,     0,    61,   537,
     538,     0,     0,   584,     0,   542,   275,   584,     0,   156,
     168,    79,   709,     0,   548,   545,   547,     0,    61,   180,
     546,     0,     0,   121,   627,     0,     0,   192,   243,     0,
       0,   551,   601,   607,     0,   551,   601,   607,     0,   551,
     601,   607,     0,   552,     0,   551,   178,   695,   552,     0,
     551,   178,     6,   552,     0,   553,     0,   155,   558,   561,
     562,   566,   591,   587,   590,   592,     0,     0,     0,     0,
       0,   559,   560,   555,     0,   559,   555,     0,   560,     0,
       0,   231,   672,   554,     0,   231,   276,   654,   277,   554,
       0,   231,   661,   554,     0,   232,   672,     0,   232,   276,
     555,   654,   277,     0,   232,   661,     0,    46,     0,   694,
       0,   563,     0,   272,     0,   564,     0,   563,   275,   564,
       0,   654,     0,   654,   565,   710,     0,    10,     0,     0,
      63,   567,     0,   568,     0,   567,   275,   568,     0,   574,
       0,   569,     0,   581,     0,   570,     0,   276,   574,   277,
       0,   276,   548,   277,   565,   571,   572,     0,   714,     0,
       0,   276,   573,   277,     0,     0,   710,     0,   573,   275,
     710,     0,   575,     0,   576,     0,   577,     0,   568,   263,
      85,   569,     0,   568,   114,   585,    85,   569,     0,   568,
     585,    85,   568,   578,     0,   579,     0,   580,     0,   122,
     634,     0,   239,   276,   627,   277,     0,   712,   582,   565,
     714,     0,   712,   582,     0,   276,   657,   277,     0,     0,
     584,     0,   715,   714,     0,   715,     0,    77,     0,   100,
     586,     0,   151,   586,     0,    64,   586,     0,     0,   127,
       0,     0,    71,    19,   588,     0,     0,   589,     0,   588,
     275,   589,     0,   654,     0,    73,   634,     0,     0,   190,
     634,     0,     0,   135,   593,     0,     0,   594,   276,   595,
     277,     0,    85,     0,   164,   108,     0,   108,     0,   164,
       0,     0,   596,     0,   596,   275,   595,     0,   597,   598,
       0,   593,     0,   715,     0,   715,   597,     0,   114,     0,
      79,   276,   599,   277,     0,   126,   709,   600,     0,   709,
       0,   709,   275,   599,     0,    79,   276,   599,   277,     0,
       0,   126,    19,   602,     0,     0,   603,     0,   602,   275,
     603,     0,   654,   604,   606,     0,    11,     0,    45,     0,
       0,   231,     0,   241,     0,   240,   556,   605,   557,     0,
       0,   259,   654,     0,   259,   654,   174,   654,     0,     0,
      80,    82,   584,   628,   182,   276,   657,   277,     0,    80,
      82,   584,   628,   548,     0,   610,     0,   611,     0,    44,
      63,   583,   591,   592,   601,   607,     0,    44,    63,   583,
     615,     0,   613,     0,   614,     0,   180,   583,   156,   616,
     591,   592,   601,   607,     0,   180,   583,   156,   616,   615,
       0,   190,    35,   121,   703,     0,   617,     0,   616,   275,
     617,     0,   633,   267,   654,     0,   680,     0,   143,    18,
     632,    63,   584,   620,   623,     0,    80,    18,   632,    82,
     584,   620,   623,     0,    60,    63,   621,   174,   621,     0,
      60,   174,   621,     0,     0,   622,     0,   661,     0,   666,
       0,   107,   624,     0,     0,   670,     0,   661,     0,   626,
       0,     0,   276,   627,   277,     0,   632,     0,   627,   275,
     632,     0,   629,     0,     0,   276,   630,   277,     0,   633,
       0,   630,   275,   633,     0,   632,     0,   714,   281,   701,
       0,   714,   281,   272,     0,   701,     0,   632,     0,   714,
     281,   701,     0,   650,     0,   118,   650,     0,   636,     0,
     634,   125,   634,     0,   634,     8,   634,     0,   636,     0,
     118,   650,     0,   635,   125,   634,     0,   635,     8,   634,
       0,   651,   125,   634,     0,   651,     8,   634,     0,   637,
       0,   276,   635,   277,     0,   118,   636,     0,   638,     0,
     641,     0,   642,     0,   643,     0,   644,     0,   649,     0,
     639,     0,   647,     0,   645,     0,   646,     0,   648,     0,
     654,   267,   654,     0,   654,   268,   654,     0,   654,   269,
     654,     0,   654,    67,   654,     0,   654,   101,   654,     0,
     654,   119,   654,     0,   654,   120,   654,     0,   654,   116,
     654,     0,   654,   267,     6,   276,   549,   277,     0,   654,
     268,     6,   276,   549,   277,     0,   654,   269,     6,   276,
     549,   277,     0,   654,    67,     6,   276,   549,   277,     0,
     654,   101,     6,   276,   549,   277,     0,   654,   119,     6,
     276,   549,   277,     0,   654,   120,     6,   276,   549,   277,
       0,   654,   116,     6,   276,   549,   277,     0,   654,   267,
     640,   276,   549,   277,     0,   654,   268,   640,   276,   549,
     277,     0,   654,   269,   640,   276,   549,   277,     0,   654,
      67,   640,   276,   549,   277,     0,   654,   101,   640,   276,
     549,   277,     0,   654,   119,   640,   276,   549,   277,     0,
     654,   120,   640,   276,   549,   277,     0,   654,   116,   640,
     276,   549,   277,     0,   163,     0,     9,     0,   654,    83,
      46,    63,   654,     0,   654,    83,   118,    46,    63,   654,
       0,   654,    17,   654,     8,   654,     0,   654,   118,    17,
     654,     8,   654,     0,   654,   103,   654,     0,   654,   118,
     103,   654,     0,   654,   103,   654,    54,   654,     0,   654,
     118,   103,   654,    54,   654,     0,   654,    75,   652,     0,
     654,   118,    75,   652,     0,   654,    31,   654,     0,   654,
     118,    31,   654,     0,   654,   167,   654,     0,   654,   118,
     167,   654,     0,   654,   167,   192,   654,     0,   654,   118,
     167,   192,   654,     0,    57,   276,   548,   277,     0,   159,
     276,   548,   277,     0,   654,    83,    94,     0,   654,    83,
     118,    94,     0,   248,     0,   249,     0,   250,     0,   251,
       0,   252,     0,   253,     0,   653,     0,   276,   657,   277,
       0,   276,   549,   277,     0,   631,     0,   656,     0,   673,
       0,   660,     0,   661,     0,   430,     0,   681,     0,   682,
       0,   692,     0,   680,     0,   271,   654,     0,   270,   654,
       0,   654,   270,   654,     0,   654,    28,   654,     0,   654,
      23,   700,     0,   654,   271,   654,     0,   654,   272,   654,
       0,   654,   273,   654,     0,   276,   654,   277,     0,   276,
     550,   277,     0,   662,     0,   663,     0,   664,     0,    39,
       0,   714,   281,    39,     0,    97,     0,   655,     0,   696,
       0,   220,     0,   221,     0,   222,     0,   631,   279,   657,
     280,     0,   654,     0,   657,   275,   654,     0,   660,     0,
     271,   659,     0,   197,     0,   196,     0,   195,     0,   223,
       0,   224,     0,   659,     0,   665,     0,    38,   199,     0,
     218,   199,     0,   219,   199,     0,   282,     0,   181,     0,
     225,     0,   226,     0,   233,     0,   234,     0,    66,     0,
     165,     0,   242,     0,   199,     0,   200,   199,     0,   667,
       0,   271,   668,     0,   196,     0,   196,     0,   667,     0,
     196,     0,   672,     0,   271,   672,     0,   196,     0,   674,
       0,   675,     0,   677,     0,    32,   276,   272,   277,     0,
      32,   276,   694,   654,   277,     0,    32,   276,    46,   654,
     277,     0,   171,   276,   694,   654,   277,     0,   171,   276,
      46,   654,   277,     0,    13,   276,   694,   654,   277,     0,
      13,   276,    46,   654,   277,     0,   110,   276,   694,   654,
     277,     0,   110,   276,    46,   654,   277,     0,   106,   276,
     694,   654,   277,     0,   106,   276,    46,   654,   277,     0,
     676,     0,   209,   276,   693,    63,   654,   277,     0,   678,
       0,    96,   276,   654,   277,     0,   228,   276,   654,    63,
     654,   679,   277,     0,    61,   654,     0,     0,   697,   276,
     657,   277,     0,   697,   276,   277,     0,    20,   276,   654,
      10,   376,   277,     0,   683,     0,   684,     0,   237,   276,
     654,   275,   654,   277,     0,   261,   276,   634,   275,   654,
     275,   654,   277,     0,   238,   276,   654,   275,   657,   277,
       0,   685,     0,   687,     0,   236,   690,   686,    51,     0,
     236,   690,   686,    50,   691,    51,     0,   189,   689,   173,
     691,     0,   686,   189,   689,   173,   691,     0,   236,   688,
      51,     0,   236,   688,    50,   691,    51,     0,   189,   634,
     173,   691,     0,   688,   189,   634,   173,   691,     0,   654,
       0,   654,     0,   654,     0,   264,    97,    61,   708,     0,
      69,   276,   708,   275,   654,   277,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   216,
       0,   217,     0,     6,     0,     0,    46,     0,     0,    94,
       0,   198,     0,   721,     0,   721,     0,   721,     0,   721,
       0,   721,     0,   721,     0,   721,     0,   721,     0,   721,
       0,   721,     0,   721,     0,   721,     0,   721,     0,   721,
       0,   721,     0,   721,     0,   721,     0,   721,     0,   721,
       0,   721,     0,   721,     0,   721,     0,   721,     0,   198,
       0,   722,     0,   201,     0,   203,     0,   204,     0,   205,
       0,   206,     0,   208,     0,   227,     0,   230,     0,   228,
       0,   238,     0,   241,     0,   247,     0,   243,     0,   237,
       0,   240,     0,   246,     0,   248,     0,   249,     0,   250,
       0,   260,     0,   254,     0,   255,     0,   261,     0,   262,
       0,   216,     0,   217,     0,   265,     0,   264,     0,   266,
       0
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
     815,   817,   824,   828,   830,   833,   835,   837,   839,   841,
     843,   845,   847,   849,   851,   856,   860,   862,   864,   872,
     876,   880,   882,   888,   895,   900,   905,   910,   918,   920,
     924,   926,   927,   933,   939,   941,   943,   947,   949,   953,
     955,   959,   961,   964,   965,   972,   990,   992,   996,   998,
    1002,  1004,  1007,  1008,  1011,  1016,  1017,  1020,  1024,  1033,
    1040,  1048,  1053,  1054,  1057,  1063,  1065,  1068,  1069,  1073,
    1075,  1077,  1079,  1081,  1085,  1087,  1090,  1091,  1095,  1100,
    1102,  1151,  1208,  1212,  1213,  1216,  1217,  1220,  1222,  1226,
    1227,  1228,  1231,  1232,  1233,  1239,  1244,  1249,  1251,  1253,
    1257,  1258,  1262,  1263,  1270,  1274,  1278,  1287,  1293,  1294,
    1297,  1299,  1304,  1306,  1311,  1317,  1323,  1327,  1333,  1335,
    1339,  1340,  1341,  1342,  1343,  1344,  1347,  1349,  1352,  1353,
    1357,  1362,  1364,  1369,  1370,  1372,  1380,  1384,  1386,  1389,
    1390,  1391,  1392,  1395,  1399,  1403,  1410,  1417,  1422,  1428,
    1431,  1433,  1435,  1437,  1441,  1444,  1448,  1451,  1454,  1457,
    1466,  1477,  1487,  1497,  1507,  1509,  1513,  1515,  1519,  1520,
    1524,  1530,  1531,  1535,  1540,  1542,  1544,  1548,  1550,  1554,
    1555,  1559,  1563,  1564,  1567,  1572,  1574,  1578,  1580,  1584,
    1589,  1590,  1593,  1597,  1599,  1601,  1605,  1606,  1610,  1611,
    1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1626,  1628,  1629,  1630,  1631,  1634,  1636,
    1640,  1644,  1648,  1653,  1657,  1661,  1663,  1667,  1671,  1677,
    1682,  1687,  1688,  1689,  1691,  1695,  1699,  1701,  1705,  1707,
    1709,  1714,  1717,  1721,  1722,  1726,  1731,  1732,  1736,  1738,
    1740,  1742,  1746,  1747,  1748,  1751,  1755,  1759,  1763,  1806,
    1811,  1813,  1815,  1819,  1821,  1823,  1829,  1837,  1839,  1843,
    1844,  1848,  1854,  1861,  1884,  1888,  1894,  1898,  1904,  1908,
    1916,  1927,  1938,  1940,  1942,  1946,  1950,  1952,  1956,  1958,
    1960,  1962,  1964,  1966,  1968,  1970,  1972,  1974,  1976,  1978,
    1980,  1982,  1984,  1988,  1990,  1994,  2000,  2004,  2006,  2013,
    2015,  2017,  2020,  2023,  2026,  2030,  2034,  2040,  2041,  2045,
    2054,  2058,  2060,  2062,  2064,  2066,  2070,  2071,  2075,  2077,
    2079,  2081,  2087,  2090,  2092,  2096,  2104,  2105,  2106,  2107,
    2108,  2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,
    2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,
    2128,  2129,  2130,  2131,  2132,  2133,  2134,  2137,  2139,  2143,
    2145,  2150,  2156,  2158,  2160,  2164,  2166,  2170,  2173,  2176,
    2184,  2190,  2191,  2195,  2209,  2211,  2213,  2215,  2229,  2239,
    2240,  2244,  2245,  2251,  2255,  2257,  2259,  2261,  2263,  2265,
    2267,  2269,  2271,  2273,  2275,  2277,  2279,  2286,  2287,  2290,
    2291,  2294,  2299,  2306,  2307,  2311,  2320,  2326,  2327,  2330,
    2331,  2332,  2333,  2350,  2355,  2360,  2379,  2396,  2403,  2404,
    2411,  2416,  2423,  2430,  2439,  2443,  2449,  2453,  2457,  2463,
    2467,  2474,  2480,  2486,  2494,  2499,  2504,  2511,  2512,  2513,
    2516,  2517,  2520,  2521,  2522,  2529,  2533,  2544,  2550,  2604,
    2666,  2667,  2674,  2687,  2692,  2697,  2704,  2706,  2713,  2714,
    2715,  2719,  2722,  2725,  2733,  2734,  2735,  2738,  2742,  2746,
    2748,  2752,  2756,  2757,  2760,  2764,  2768,  2769,  2772,  2774,
    2778,  2779,  2783,  2787,  2788,  2792,  2793,  2797,  2798,  2799,
    2800,  2803,  2805,  2809,  2811,  2815,  2817,  2820,  2822,  2824,
    2828,  2830,  2832,  2836,  2838,  2840,  2844,  2848,  2850,  2852,
    2856,  2858,  2862,  2863,  2867,  2871,  2873,  2877,  2878,  2883,
    2890,  2894,  2896,  2900,  2902,  2906,  2908,  2915,  2919,  2924,
    2929,  2930,  2932,  2936,  2939,  2951,  2955,  2959,  2963,  2967,
    2969,  2971,  2973,  2977,  2979,  2981,  2985,  2987,  2989,  2993,
    2995,  2999,  3001,  3005,  3006,  3010,  3011,  3015,  3016,  3021,
    3025,  3026,  3030,  3031,  3034,  3035,  3036,  3041,  3046,  3047,
    3051,  3053,  3057,  3058,  3062,  3063,  3064,  3067,  3072,  3077,
    3081,  3082,  3085,  3089,  3093,  3096,  3101,  3103,  3107,  3108,
    3113,  3118,  3120,  3122,  3124,  3126,  3130,  3131,  3137,  3139,
    3143,  3144,  3150,  3153,  3155,  3159,  3161,  3168,  3170,  3174,
    3178,  3180,  3182,  3188,  3190,  3194,  3195,  3199,  3201,  3204,
    3205,  3209,  3211,  3213,  3217,  3218,  3222,  3224,  3230,  3232,
    3236,  3237,  3241,  3245,  3247,  3249,  3253,  3255,  3259,  3261,
    3267,  3270,  3278,  3285,  3288,  3295,  3296,  3299,  3303,  3310,
    3311,  3314,  3319,  3325,  3332,  3333,  3337,  3341,  3348,  3350,
    3354,  3356,  3358,  3362,  3363,  3366,  3370,  3372,  3376,  3378,
    3384,  3385,  3389,  3393,  3394,  3399,  3400,  3404,  3408,  3409,
    3414,  3415,  3418,  3423,  3428,  3431,  3438,  3439,  3441,  3442,
    3444,  3448,  3449,  3451,  3453,  3457,  3459,  3463,  3464,  3466,
    3470,  3471,  3472,  3473,  3474,  3475,  3476,  3477,  3478,  3479,
    3480,  3485,  3487,  3489,  3491,  3493,  3495,  3497,  3499,  3505,
    3507,  3509,  3511,  3513,  3515,  3517,  3519,  3521,  3523,  3525,
    3527,  3529,  3531,  3533,  3535,  3539,  3540,  3546,  3548,  3552,
    3554,  3559,  3561,  3563,  3565,  3570,  3572,  3576,  3578,  3582,
    3584,  3586,  3588,  3592,  3596,  3600,  3602,  3606,  3611,  3616,
    3623,  3628,  3633,  3642,  3643,  3647,  3654,  3655,  3656,  3657,
    3658,  3659,  3660,  3661,  3662,  3663,  3664,  3666,  3668,  3675,
    3677,  3679,  3686,  3693,  3700,  3702,  3704,  3705,  3706,  3707,
    3709,  3711,  3715,  3716,  3719,  3735,  3751,  3755,  3759,  3760,
    3764,  3765,  3769,  3771,  3773,  3775,  3777,  3781,  3782,  3784,
    3800,  3816,  3820,  3824,  3826,  3830,  3834,  3837,  3840,  3843,
    3846,  3851,  3853,  3858,  3859,  3863,  3870,  3877,  3884,  3891,
    3892,  3896,  3903,  3904,  3905,  3908,  3910,  3912,  3915,  3922,
    3931,  3938,  3947,  3949,  3951,  3953,  3957,  3960,  3964,  3965,
    3969,  3978,  3980,  3984,  3986,  3990,  3996,  3997,  4000,  4004,
    4007,  4011,  4012,  4015,  4017,  4021,  4023,  4027,  4029,  4033,
    4035,  4039,  4042,  4045,  4050,  4053,  4063,  4065,  4067,  4069,
    4071,  4073,  4075,  4077,  4081,  4082,  4085,  4086,  4089,  4097,
    4100,  4103,  4106,  4109,  4112,  4115,  4118,  4121,  4124,  4127,
    4130,  4133,  4136,  4139,  4142,  4145,  4148,  4151,  4154,  4157,
    4160,  4163,  4166,  4171,  4172,  4177,  4179,  4180,  4181,  4182,
    4183,  4184,  4185,  4186,  4187,  4188,  4189,  4190,  4191,  4192,
    4193,  4194,  4195,  4196,  4198,  4199,  4200,  4201,  4202,  4203,
    4204,  4205,  4206,  4207
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
  "replace_clause", "exception_clause", "rexception_clause", 
  "replace_exception_clause", "alter_exception_clause", "unique_opt", 
  "index_definition", "shadow_clause", "manual_auto", "conditional", 
  "first_file_length", "sec_shadow_files", "db_file_list", 
  "domain_clause", "as_opt", "domain_default_opt", 
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
  "proc_statement", "excp_statement", "raise_statement", "exec_sql", 
  "for_select", "for_exec_into", "exec_into", "if_then_else", 
  "post_event", "event_argument_opt", "singleton_select", "variable", 
  "variable_list", "while", "label_opt", "breakleave", "cursor_def", 
  "excp_hndl_statements", "excp_hndl_statement", "errors", "err", 
  "cursor_statement", "open_cursor", "close_cursor", "fetch_cursor", 
  "fetch_opt", "exec_procedure", "proc_inputs", "proc_outputs_opt", 
  "exec_block", "block_input_params", "block_parameters", 
  "block_parameter", "view_clause", "rview_clause", "begin_string", 
  "end_string", "begin_trigger", "end_trigger", "check_opt", 
  "def_trigger_clause", "replace_trigger_clause", "trigger_active", 
  "trigger_type", "trigger_type_prefix", "trigger_type_suffix", 
  "trigger_position", "trigger_action", "alter", "alter_clause", 
  "domain_default_opt2", "domain_check_constraint2", "alter_domain_ops", 
  "alter_domain_op", "alter_ops", "alter_op", "alter_column_name", 
  "keyword_or_column", "col_opt", "alter_data_type_or_domain", 
  "alter_col_name", "drop_behaviour", "alter_index_clause", 
  "alter_sequence_clause", "init_alter_db", "alter_db", "db_alter_clause", 
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
  "delete_positioned", "update", "update_searched", "update_positioned", 
  "cursor_clause", "assignments", "assignment", "exec_udf", "blob_io", 
  "filter_clause_io", "blob_subtype_value_io", "blob_subtype_io", 
  "segment_clause_io", "segment_length_io", "column_parens_opt", 
  "column_parens", "column_list", "ins_column_parens_opt", 
  "ins_column_parens", "ins_column_list", "column_name", 
  "simple_column_name", "update_column_name", "search_condition", 
  "bracable_search_condition", "simple_search_condition", "predicate", 
  "comparison_predicate", "quantified_predicate", "some", 
  "distinct_predicate", "between_predicate", "like_predicate", 
  "in_predicate", "containing_predicate", "starting_predicate", 
  "exists_predicate", "singular_predicate", "null_predicate", 
  "trigger_action_predicate", "special_trigger_action_predicate", 
  "in_predicate_value", "table_subquery", "value", 
  "datetime_value_expression", "array_element", "value_list", "constant", 
  "u_numeric_constant", "u_constant", "parameter", "current_user", 
  "current_role", "internal_info", "sql_string", "signed_short_integer", 
  "nonneg_short_integer", "neg_short_integer", "pos_short_integer", 
  "unsigned_short_integer", "signed_long_integer", "long_integer", 
  "function", "aggregate_function", "numeric_value_function", 
  "extract_expression", "string_value_function", "substring_function", 
  "string_length_opt", "udf", "cast_specification", "case_expression", 
  "case_abbreviation", "case_specification", "simple_case", 
  "simple_when_clause", "searched_case", "searched_when_clause", 
  "when_operand", "case_operand", "case_result", "next_value_expression", 
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
     320,   320,   320,   320,   320,   321,   322,   322,   322,   322,
     323,   324,   324,   324,   325,   326,   327,   328,   329,   329,
     330,   330,   330,   331,   332,   332,   332,   333,   333,   334,
     334,   335,   335,   336,   336,   337,   338,   338,   339,   339,
     340,   340,   341,   341,   342,   343,   343,   344,   345,   346,
     347,   348,   349,   349,   350,   351,   351,   352,   352,   353,
     353,   353,   353,   353,   354,   354,   355,   355,   356,   356,
     356,   357,   358,   359,   359,   360,   360,   361,   361,   362,
     362,   362,   363,   363,   363,   364,   365,   366,   366,   366,
     367,   367,   368,   368,   369,   369,   369,   370,   371,   371,
     372,   372,   373,   373,   374,   375,   376,   377,   378,   378,
     379,   379,   379,   379,   379,   379,   380,   380,   381,   381,
     382,   383,   383,   383,   383,   383,   384,   385,   385,   386,
     386,   386,   386,   387,   388,   389,   390,   390,   391,   392,
     392,   392,   392,   392,   393,   394,   395,   395,   395,   395,
     396,   397,   398,   399,   400,   400,   401,   401,   402,   402,
     403,   404,   404,   405,   406,   406,   406,   407,   407,   408,
     408,   409,   410,   410,   411,   412,   412,   413,   413,   414,
     415,   415,   416,   417,   417,   417,   418,   418,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   420,   420,
     421,   422,   423,   424,   425,   426,   426,   427,   428,   429,
     430,   431,   431,   431,   431,   432,   433,   433,   434,   434,
     434,   435,   435,   436,   436,   437,   438,   438,   439,   439,
     439,   439,   440,   440,   440,   441,   442,   443,   444,   445,
     446,   446,   446,   447,   447,   447,   448,   449,   449,   450,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   458,
     459,   460,   461,   461,   461,   462,   463,   463,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   465,   465,   466,   467,   468,   468,   468,
     468,   468,   468,   468,   468,   469,   470,   471,   471,   472,
     472,   472,   472,   472,   472,   472,   473,   473,   474,   474,
     474,   474,   474,   474,   474,   475,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   477,   477,   478,
     478,   479,   480,   480,   480,   481,   481,   482,   482,   482,
     483,   484,   484,   485,   485,   485,   485,   485,   486,   487,
     487,   488,   488,   489,   490,   490,   490,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   490,   491,   491,   492,
     492,   493,   493,   494,   494,   495,   495,   496,   496,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   498,   498,
     499,   499,   499,   499,   500,   500,   501,   501,   501,   502,
     502,   503,   503,   503,   504,   504,   504,   505,   505,   505,
     506,   506,   507,   507,   507,   508,   508,   509,   509,   509,
     510,   510,   511,   511,   511,   511,   512,   512,   513,   513,
     513,   514,   514,   514,   515,   515,   515,   516,   517,   518,
     518,   519,   520,   520,   521,   522,   523,   523,   524,   524,
     525,   525,   526,   527,   527,   528,   528,   529,   529,   529,
     529,   530,   530,   531,   531,   532,   532,   533,   533,   533,
     534,   534,   534,   535,   535,   535,   536,   537,   537,   537,
     538,   538,   539,   539,   540,   541,   541,   542,   542,   543,
     544,   545,   545,   546,   546,   547,   547,   548,   549,   550,
     551,   551,   551,   552,   553,   554,   555,   556,   557,   558,
     558,   558,   558,   559,   559,   559,   560,   560,   560,   561,
     561,   562,   562,   563,   563,   564,   564,   565,   565,   566,
     567,   567,   568,   568,   569,   569,   569,   570,   571,   571,
     572,   572,   573,   573,   574,   574,   574,   575,   576,   577,
     578,   578,   579,   580,   581,   581,   582,   582,   583,   583,
     584,   585,   585,   585,   585,   585,   586,   586,   587,   587,
     588,   588,   589,   590,   590,   591,   591,   592,   592,   593,
     594,   594,   594,   594,   594,   595,   595,   596,   596,   597,
     597,   598,   598,   598,   599,   599,   600,   600,   601,   601,
     602,   602,   603,   604,   604,   604,   605,   605,   606,   606,
     607,   607,   607,   608,   608,   609,   609,   610,   611,   612,
     612,   613,   614,   615,   616,   616,   617,   618,   619,   619,
     620,   620,   620,   621,   621,   622,   623,   623,   624,   624,
     625,   625,   626,   627,   627,   628,   628,   629,   630,   630,
     631,   631,   631,   632,   633,   633,   634,   634,   634,   634,
     634,   635,   635,   635,   635,   635,   635,   636,   636,   636,
     637,   637,   637,   637,   637,   637,   637,   637,   637,   637,
     637,   638,   638,   638,   638,   638,   638,   638,   638,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   639,   639,
     639,   639,   639,   639,   639,   640,   640,   641,   641,   642,
     642,   643,   643,   643,   643,   644,   644,   645,   645,   646,
     646,   646,   646,   647,   648,   649,   649,   650,   650,   650,
     651,   651,   651,   652,   652,   653,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   654,   654,   654,   654,
     654,   654,   654,   654,   655,   655,   655,   656,   657,   657,
     658,   658,   659,   659,   659,   659,   659,   660,   660,   660,
     660,   660,   661,   662,   662,   663,   664,   664,   664,   664,
     664,   665,   665,   666,   666,   667,   668,   669,   670,   671,
     671,   672,   673,   673,   673,   674,   674,   674,   674,   674,
     674,   674,   674,   674,   674,   674,   675,   676,   677,   677,
     678,   679,   679,   680,   680,   681,   682,   682,   683,   683,
     683,   684,   684,   685,   685,   686,   686,   687,   687,   688,
     688,   689,   690,   691,   692,   692,   693,   693,   693,   693,
     693,   693,   693,   693,   694,   694,   695,   695,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   721,   722,   722,   722,   722,   722,
     722,   722,   722,   722,   722,   722,   722,   722,   722,   722,
     722,   722,   722,   722,   722,   722,   722,   722,   722,   722,
     722,   722,   722,   722
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
       1,     1,     2,     2,     7,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       4,     2,     2,     2,     2,     2,     2,     2,     1,     0,
       1,     1,     6,     6,     1,     1,     0,     0,     1,     0,
       4,     0,     1,     1,     2,     8,     1,     0,     3,     0,
       0,     1,     1,     2,     1,     1,     1,     2,     6,     1,
       1,     3,     0,     1,     1,     0,     1,     1,     2,     3,
       4,     2,     2,     3,     0,     1,     1,     2,     1,     4,
       3,     3,     1,     0,     1,     1,     2,     3,     4,     0,
       1,     2,     0,     1,     1,     5,     5,     3,     2,     0,
       1,     3,     1,     1,     6,     3,     2,     6,     2,     1,
       2,     2,     2,     0,     1,     1,     2,     0,     2,     0,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       2,     2,     5,     1,     2,     3,     2,     2,     0,     1,
       1,     1,     1,     3,     4,     8,     4,     0,     6,     1,
       1,     2,     2,     0,     3,     3,     1,     2,     2,     2,
       8,     8,     8,     8,     3,     0,     4,     0,     1,     3,
       4,     1,     3,     2,     3,     3,     1,     1,     0,     1,
       2,     4,     1,     1,     3,     1,     0,     2,     1,     6,
       1,     1,     3,     1,     2,     0,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     2,     3,
       1,     3,     8,     9,     5,     8,     6,     3,     0,     3,
       2,     1,     1,     3,     3,     7,     2,     0,     1,     1,
       2,     3,     0,     1,     2,     4,     1,     3,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     5,     0,     5,
       1,     3,     0,     2,     4,     0,     7,     3,     0,     1,
       3,     3,     7,     7,     0,     0,     0,     0,     3,     0,
       9,     9,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     5,     5,     5,
       5,     5,     5,     2,     0,     4,     2,     2,     3,     2,
       2,     3,     3,     2,     2,     3,     6,     1,     2,     4,
       3,     2,     2,     2,     2,     3,     1,     3,     3,     3,
       2,     2,     4,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     0,     2,     2,     4,     4,     5,
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
       0,     5,     1,     0,     3,     2,     1,     0,     2,     0,
       1,     0,     3,     1,     0,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     2,     3,     1,     1,     3,     3,
       1,     2,     3,     1,     2,     0,     2,     1,     1,     0,
       1,     1,     1,     3,     2,     3,     0,     1,     3,     4,
       3,     3,     0,     2,     0,     2,     0,     3,     3,     3,
       1,     4,     4,     1,     9,     0,     0,     0,     0,     3,
       2,     1,     0,     3,     5,     3,     2,     5,     2,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     0,     2,
       1,     3,     1,     1,     1,     1,     3,     6,     1,     0,
       3,     0,     1,     3,     1,     1,     1,     4,     5,     5,
       1,     1,     2,     4,     4,     2,     3,     0,     1,     2,
       1,     1,     2,     2,     2,     0,     1,     0,     3,     0,
       1,     3,     1,     2,     0,     2,     0,     2,     0,     4,
       1,     2,     1,     1,     0,     1,     3,     2,     1,     1,
       2,     1,     4,     3,     1,     3,     4,     0,     3,     0,
       1,     3,     3,     1,     1,     0,     1,     1,     4,     0,
       2,     4,     0,     8,     5,     1,     1,     7,     4,     1,
       1,     8,     5,     4,     1,     3,     3,     1,     7,     7,
       5,     3,     0,     1,     1,     1,     2,     0,     1,     1,
       1,     0,     3,     1,     3,     1,     0,     3,     1,     3,
       1,     3,     3,     1,     1,     3,     1,     2,     1,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     1,     1,     5,     6,     5,
       6,     3,     4,     5,     6,     3,     4,     3,     4,     3,
       4,     4,     5,     4,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     1,     6,     1,     4,
       7,     2,     0,     4,     3,     6,     1,     1,     6,     8,
       6,     1,     1,     4,     6,     4,     5,     3,     5,     4,
       5,     1,     1,     1,     4,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   587,   129,     0,     0,     0,     0,     0,     0,
       0,     0,    50,   587,   652,     0,     0,     0,     0,     0,
       1,    10,    16,     7,     6,    14,    15,    12,    13,     3,
       9,    20,   569,    18,   574,   575,   576,     5,    17,   568,
     570,    19,   632,   739,   640,   643,    11,     8,   755,   756,
      21,   759,   760,     4,   480,     0,     0,     0,     0,     0,
       0,     0,   406,   586,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   128,     0,     0,     0,   102,   745,
     935,     0,    22,   933,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   493,     0,   368,    28,    36,    33,    35,   781,    34,
     781,  1023,  1025,  1026,  1027,  1028,  1029,  1030,  1049,  1050,
    1031,  1033,  1032,  1038,  1034,  1039,  1035,  1037,  1040,  1036,
    1041,  1042,  1043,  1045,  1046,  1044,  1047,  1048,  1052,  1051,
    1053,     0,    30,     0,    31,     0,    66,    68,  1015,  1024,
       0,     0,     0,     0,     0,     0,     0,   585,     0,     0,
     995,   646,   651,     0,     0,   594,     0,   698,   700,  1017,
       0,     0,     0,     0,   115,   577,  1022,     0,     2,     0,
     636,     0,   997,   752,     0,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   465,   459,   460,   458,
     461,   462,   463,   464,   466,     0,   435,   436,   407,     0,
    1007,   413,     0,  1011,   410,   265,  1014,     0,   700,   409,
     384,  1018,   414,     0,  1010,   591,   584,   931,     0,   111,
     165,   164,   112,   147,   214,   793,  1003,   103,     0,   109,
     159,     0,   105,   265,   113,   937,   136,   106,   199,   107,
       0,   108,   781,  1021,   114,   160,   110,   743,   744,     0,
     936,   934,     0,    74,     0,  1008,   716,   501,  1006,   494,
       0,   500,   505,   495,   496,   503,   497,   498,   499,   504,
     506,   362,     0,   267,    29,     0,    38,   780,    37,    27,
       0,     0,     0,     0,     0,   786,     0,     0,     0,     0,
       0,     0,   583,   941,     0,   922,   645,   645,   646,   658,
     656,   994,   659,     0,   660,   650,   646,     0,     0,     0,
       0,     0,     0,   610,   603,   592,   593,   595,   597,   598,
     599,   606,   607,   600,     0,   699,  1016,   119,     0,   116,
     265,   117,   199,   118,   781,   580,   634,     0,   630,     0,
       0,   996,     0,     0,   637,     0,     0,     0,     0,   411,
     481,   376,     0,   376,     0,   217,   412,   417,   127,   475,
     476,     0,   267,   238,   467,     0,   408,   426,     0,   382,
     383,   490,     0,   590,   588,   932,   162,   162,     0,     0,
       0,   174,   166,   167,   146,     0,   124,     0,     0,     0,
     120,   267,   135,   134,   137,     0,     0,     0,     0,     0,
     999,    75,   217,     0,     0,   718,   758,   502,     0,     0,
       0,     0,   899,   928,     0,   998,     0,   901,     0,     0,
     929,     0,   923,   914,   913,   912,  1023,     0,     0,     0,
     904,   905,   906,   915,   916,   924,   925,  1033,   926,   927,
       0,  1038,  1034,   930,  1047,  1052,     0,     0,     0,     0,
     881,   365,   876,   790,   908,   902,   877,   360,   917,   879,
     880,   896,   897,   898,   918,   878,   942,   943,   956,   944,
     958,   885,   882,   883,   966,   967,   971,   972,   884,   903,
       0,     0,  1003,     0,     0,     0,   369,   215,     0,     0,
       0,   783,    26,     0,    32,     0,     0,    42,    69,    71,
    1019,    67,     0,     0,     0,   785,     0,    47,    49,    27,
       0,     0,   582,     0,     0,     0,   655,   653,     0,   662,
       0,   661,   663,   668,   649,     0,   629,     0,   604,   615,
     601,   615,   602,   616,   622,   626,   627,   611,   596,   716,
     764,   794,     0,     0,   125,   267,     0,     0,   579,   578,
       0,   631,   635,   738,   740,   745,   642,   641,   750,   182,
       0,   483,   143,     0,   486,     0,   487,   482,     0,   421,
       0,   423,   422,     0,   424,     0,   418,     0,     0,   268,
       0,     0,   430,     0,   431,   376,   468,     0,   474,     0,
       0,     0,   471,  1003,   387,   386,   489,     0,   404,     0,
     163,     0,     0,   172,     0,   171,     0,     0,   161,   175,
     176,   178,   168,   538,   550,   525,   560,   528,   551,   561,
       0,   567,   529,     0,   557,     0,   552,   564,   524,   547,
     526,   527,   522,   376,   507,   508,   509,   517,   523,   510,
     519,   540,     0,   545,   542,   520,   557,   521,   123,     0,
     121,   265,   122,     0,     0,   138,     0,     0,   198,   238,
     384,   374,     0,   217,     0,    81,    83,     0,     0,   101,
     100,  1000,     0,     0,     0,     0,   867,   868,   869,     0,
     715,   798,   807,   810,   816,   811,   812,   813,   814,   818,
     819,   817,   820,   815,   796,     0,   724,   739,   995,     0,
     995,   919,     0,     0,   995,   995,   995,     0,   920,   921,
       0,     0,   982,     0,     0,     0,     0,     0,     0,   887,
     886,     0,   739,   908,     0,   330,  1020,     0,   359,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   273,
     517,   540,     0,     0,   367,     0,   278,     0,   782,     0,
       0,    43,    72,     0,     0,    25,   772,     0,   788,     0,
     754,   772,     0,     0,    46,   581,     0,   645,     0,     0,
     716,     0,   667,     0,   572,     0,   571,   939,     0,   605,
       0,   613,   609,   608,     0,   619,     0,   624,   612,     0,
     718,   762,     0,     0,     0,   238,   374,   633,     0,   749,
       0,     0,   144,   183,   485,   376,   420,     0,   376,   377,
     425,   374,     0,   264,   374,   237,  1004,   209,   206,     0,
     149,   376,   507,   374,     0,     0,     0,   236,   239,   240,
     241,   242,     0,   429,   473,   472,   428,   427,     0,     0,
       0,   390,   388,   389,   385,     0,   376,   478,     0,   477,
     192,   169,   173,     0,     0,   177,     0,     0,   535,   548,
     549,   565,     0,   562,   567,     0,   555,   553,   554,   149,
       0,     0,     0,   518,     0,     0,     0,     0,   556,   126,
     267,     0,   374,   139,   197,     0,   200,   202,   203,     0,
       0,     0,     0,   217,   217,    78,     0,    86,    77,     0,
       0,     0,     0,   809,   797,     0,     0,   870,   871,   872,
       0,   801,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   720,   722,   723,   717,     0,   752,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   986,   987,   988,   989,   990,   991,   992,   993,     0,
       0,     0,     0,   977,     0,     0,     0,     0,     0,     0,
       0,   895,   752,   894,   361,     0,   331,   363,   332,     0,
       0,   890,  1002,   889,   888,   891,   892,   893,   909,   964,
       0,   900,   792,   791,   371,   370,     0,   271,   286,     0,
     277,   279,   784,     0,     0,     0,    70,     0,   777,     0,
     787,     0,   777,     0,     0,   654,   657,     0,   669,   670,
     673,   675,   672,   684,   685,   686,   674,   697,     0,   709,
     664,   666,  1012,   573,   940,   614,   623,   618,   617,     0,
     628,   765,   739,   766,   795,   374,     0,     0,   741,   647,
     742,   751,   484,   162,   189,   181,   184,   185,     0,     0,
       0,   419,   374,   374,   377,   276,   269,   278,   208,   376,
     376,   377,   210,   205,   211,     0,     0,   247,     0,   433,
     377,   376,   374,   432,     0,     0,     0,   403,   492,   479,
     193,   194,   170,     0,   180,   536,   537,   938,     0,     0,
       0,   540,     0,   563,     0,     0,   377,     0,   513,   515,
       0,     0,     0,     0,     0,     0,     0,   384,   278,   162,
     141,   238,   195,   404,   379,     0,    82,     0,   217,     0,
      90,     0,    84,     0,     0,    89,    85,     0,   763,  1005,
       0,     0,   802,     0,     0,   808,     0,     0,   800,   799,
       0,   857,     0,   846,   845,     0,   824,     0,   855,   873,
       0,   865,     0,     0,     0,   825,   851,     0,     0,   828,
       0,     0,     0,     0,     0,     0,     0,   826,     0,     0,
     827,     0,   859,     0,     0,   821,     0,     0,   822,     0,
       0,   823,   721,   724,   757,     0,     0,   217,     0,   945,
       0,     0,   959,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   983,     0,     0,   981,     0,     0,   973,     0,
       0,     0,     0,   984,   639,     0,     0,     0,   907,   963,
       0,   266,   285,     0,   337,   366,   280,     0,     0,     0,
       0,     0,  1029,    40,    51,    53,    52,    61,    63,    40,
      41,     0,     0,     0,   769,   789,     0,   768,     0,     0,
       0,   705,   672,     0,   707,   701,   707,   705,   707,     0,
       0,     0,   668,     0,   714,   620,   621,   625,   752,   278,
     196,   379,     0,     0,   190,     0,   186,     0,     0,     0,
     415,   225,   220,   910,   221,   222,   223,   224,     0,     0,
     270,     0,     0,     0,   226,     0,   247,   745,   243,     0,
     434,   469,   470,   395,   396,   392,   391,   394,   393,   376,
     491,   377,   179,  1001,     0,     0,   531,     0,   530,   566,
       0,   558,   150,     0,   511,     0,   539,   540,   546,   544,
       0,   541,   374,     0,     0,     0,   133,   142,   201,   376,
       0,   375,   104,     0,   131,   130,    93,     0,     0,    94,
       0,    87,    88,     0,   863,   864,   804,   803,   806,   805,
       0,     0,     0,     0,   739,     0,     0,     0,   866,     0,
       0,     0,     0,     0,     0,   858,   856,   852,     0,   860,
       0,     0,     0,     0,   861,     0,     0,     0,     0,     0,
       0,   728,     0,   725,     0,   729,   951,   950,     0,     0,
     947,   946,     0,   955,   954,   953,   952,   949,   948,     0,
     962,   979,   978,     0,     0,     0,     0,     0,     0,     0,
     364,   334,   333,   272,     0,     0,   282,   283,     0,  1003,
     320,     0,     0,     0,     0,     0,     0,  1031,  1036,     0,
       0,   358,   296,   291,     0,   337,   290,     0,     0,     0,
     316,   317,     0,   314,     0,     0,   315,     0,     0,     0,
     352,   354,   353,     0,     0,     0,     0,     0,     0,     0,
     767,     0,  1003,    65,    57,    58,    64,    59,    60,     0,
      23,     0,     0,    24,     0,   773,   774,   775,   771,   776,
     779,   778,   753,    44,    45,   668,   676,   671,   706,   704,
     702,     0,   703,     0,     0,     0,     0,     0,     0,   718,
     761,     0,   375,   746,   747,   648,   192,   191,   187,     0,
     377,   911,   274,   275,   377,   377,   148,   213,   227,   228,
     376,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,   278,   488,   533,     0,   534,     0,     0,   213,   151,
     152,   154,   155,   156,   514,   516,   512,   543,   278,   404,
     377,   192,     0,     0,   372,   376,    91,     0,     0,    97,
      92,   540,     0,   849,     0,     0,   875,   752,   874,   847,
       0,     0,     0,   853,     0,     0,     0,     0,   862,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   719,
     724,     0,   731,     0,   727,   730,   965,   216,   985,   957,
       0,     0,   980,   975,   974,     0,   968,   970,     0,   219,
     281,     0,   318,     0,   313,     0,   328,   312,   340,  1013,
     355,   356,     0,   292,     0,   297,   294,   343,   307,   308,
     304,   305,   309,   302,     0,     0,   311,   310,   303,     0,
     299,   301,   300,   298,   306,   336,     0,    54,    55,    56,
      62,     0,   679,     0,   687,   705,   696,   694,   708,   710,
     712,   713,   644,   377,   373,   748,   188,   377,   158,   263,
       0,     0,   204,   229,     0,     0,     0,   247,   230,   233,
     781,     0,   377,   401,   402,   398,   397,   400,   399,     0,
     532,   559,   157,   145,   153,     0,   376,   260,   140,   377,
     378,     0,     0,    95,    96,    79,     0,   832,   840,   638,
     848,   833,   841,   836,   844,   850,   854,   834,   842,   835,
     843,   829,   837,   830,   838,   831,   839,   726,     0,   737,
     961,   960,   976,     0,     0,     0,   288,   284,     0,   319,
     321,     0,   327,     0,   351,     0,     0,     0,     0,   346,
     344,     0,     0,     0,   329,    39,   770,   681,   678,   688,
       0,     0,   689,   690,   691,     0,   261,   416,   207,   212,
     231,   247,   781,   234,   253,   246,   248,   405,   377,     0,
     380,   377,    76,    98,    99,     0,   734,     0,   733,   969,
     218,   287,     0,     0,     0,     0,   350,   349,  1009,   348,
     337,     0,     0,     0,     0,     0,   677,   692,     0,   711,
     235,   253,     0,   247,   249,   250,   262,   377,     0,   732,
       0,     0,     0,   324,   337,   357,   345,   347,     0,   342,
       0,     0,   682,     0,   247,     0,     0,   245,     0,   252,
       0,   251,   381,   132,   735,     0,   289,   326,     0,     0,
       0,   337,     0,   680,   693,   232,     0,     0,   256,   255,
     254,   736,   337,     0,     0,   337,   335,   683,   259,   257,
     258,   325,   337,   341,   322,   323,     0,     0,     0
};

static const short yydefgoto[] =
{
    1916,    20,    21,   513,   141,   142,   143,   144,  1510,   775,
     770,    22,   155,   156,  1263,  1264,  1265,  1266,  1267,   145,
     146,   517,   518,    23,    86,   421,   917,   684,   685,   686,
    1156,  1149,  1150,  1600,   273,   688,    24,    78,    25,   174,
      26,   410,   247,   347,   668,   218,    79,  1372,   254,   414,
     676,  1140,  1366,   581,   242,   405,  1091,  1578,  1579,  1580,
    1581,  1582,  1583,   249,   264,   239,   621,   240,   401,   402,
     403,   628,   629,   630,   582,   583,  1075,  1076,  1077,  1305,
    1112,   257,   351,   416,   905,   906,   907,   838,   839,   840,
    1712,   603,   503,  1428,   687,  1776,  1310,  1557,  1558,  1559,
    1718,   908,   605,   847,   848,   849,   850,  1328,   851,  1853,
    1854,  1855,  1899,   252,   349,   670,   224,   382,   509,   598,
     599,  1016,   504,  1084,  1019,  1020,  1021,  1455,  1456,  1253,
    1777,  1457,  1472,  1473,  1474,  1475,  1476,  1477,  1478,  1479,
    1480,  1481,  1482,  1483,  1484,  1782,  1485,   470,   997,  1486,
    1487,  1488,  1890,  1666,  1667,  1788,  1789,  1489,  1490,  1491,
    1492,  1662,  1493,   471,   748,    28,   293,   505,   506,   261,
     353,   910,  1594,   590,  1081,  1371,   259,   672,   391,   616,
     617,   864,   866,  1340,    29,    62,   829,   826,   376,   377,
     386,   387,   215,   216,   388,  1100,   611,   856,   221,   232,
     184,   369,   370,   229,   618,  1341,    30,   101,   653,   654,
     655,  1127,  1128,   656,   760,   658,   659,  1121,   878,   893,
     660,   761,   662,   663,   664,   665,   886,   666,   667,   883,
      31,    32,    33,    34,    35,   569,    36,   533,    37,    38,
      64,   236,   394,    39,   335,   336,   337,   338,   339,   340,
     341,   342,   802,   343,  1059,  1297,   553,   554,   807,   555,
      40,  1494,   180,   571,   358,    42,  1393,   741,    43,    44,
      45,   536,   325,  1302,  1705,   160,   161,   162,   323,   540,
     541,   542,   793,   790,  1038,  1039,  1040,  1041,  1797,  1846,
    1871,  1042,  1043,  1044,  1045,  1802,  1803,  1804,  1046,  1292,
     166,   167,  1290,  1529,  1294,  1698,  1699,  1539,   425,   717,
    1421,   955,  1422,  1423,  1424,  1634,  1825,  1828,   183,   573,
     574,   269,  1545,  1070,   364,  1495,  1496,    48,    49,  1497,
      51,    52,   426,   559,  1498,  1499,    53,  1028,  1514,  1515,
    1274,  1519,   296,   297,   510,   524,   525,   777,   472,   473,
     562,   700,   930,   701,   702,   703,   704,  1175,   705,   706,
     707,   708,   709,   710,   711,   712,   713,   714,   932,  1178,
    1179,   715,   475,   476,   477,  1312,   478,   479,   480,   481,
     482,   483,   484,  1517,    83,   271,   256,  1119,  1129,   797,
     485,   486,   487,   488,   489,   490,  1641,   491,   492,   493,
     494,   495,   496,   986,   497,   733,  1236,   734,  1233,   498,
     979,   324,   362,   499,   500,   690,  1342,  1001,   245,   835,
    1158,   277,   219,   274,  1837,   250,  1826,  1051,  1501,  1047,
     147,   501,   228,   230,  1268,   745,   262,   175,   502,   149
};

static const short yypact[] =
{
    2524,   154,    56,  1823,   298,   740,   200,  1546,    57,  5363,
     202,   254,  1442,    56,   687,   485,  6419,   495,  6419,    53,
       4,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   449,   120,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  7823,  6419,  6419,  6419,  6419,
    6419,  6419,-32768,-32768,   382,   745,  6419,  6419,  6419,   537,
    6419,   451,  6419,  6419,-32768,  6419,  6419,  6419,-32768,   231,
  -32768,   465,-32768,-32768,   670,  6419,-32768,  6419,  6419,  6419,
     682,  6419,  6419,  6419,  6419,   451,  6419,  6419,  6419,  6419,
    6419,-32768,  6419,   405,   639,-32768,-32768,-32768,   548,-32768,
     548,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   742,   609,   765,-32768,    49,-32768,-32768,-32768,-32768,
    6419,  6419,  6419,   773,   802,   477,  6419,   758,   355,   364,
     653,   703,-32768,  6419,   862,   673,   793,-32768,  6419,-32768,
    6419,  6419,  6419,  6419,-32768,-32768,-32768,  6419,-32768,   776,
     780,   956,   677,   743,   828,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   478,-32768,-32768,-32768,   745,
  -32768,-32768,   482,-32768,-32768,   713,-32768,   253,-32768,-32768,
     648,-32768,-32768,   746,-32768,   854,-32768,-32768,   830,-32768,
     662,-32768,-32768,  1027,-32768,-32768,-32768,-32768,   745,-32768,
  -32768,   444,-32768,   713,-32768,-32768,   198,-32768,   981,-32768,
     980,-32768,   548,-32768,-32768,-32768,-32768,-32768,-32768,   972,
  -32768,-32768,   859,-32768,   987,-32768,   883,-32768,-32768,-32768,
     859,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  6764,  6419,   939,-32768,  6419,-32768,-32768,-32768,   919,
     499,   951,  4343,  6419,  1011,   819,  1035,  1038,  1040,   984,
     988,   137,   867,-32768,  6983,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  5450,-32768,-32768,-32768,   929,  6419,  1013,
     931,   315,  6419,   948,-32768,-32768,   673,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  6419,-32768,-32768,-32768,   745,-32768,
     713,-32768,   981,-32768,   548,  1004,  1018,   891,-32768,  6983,
     991,-32768,   991,  6983,-32768,   111,   893,   895,   897,   828,
  -32768,  1124,   701,-32768,  6419,-32768,   478,-32768,-32768,-32768,
  -32768,  6419,   939,  2784,   949,  3037,   880,-32768,  7823,-32768,
  -32768,   594,   968,-32768,-32768,-32768,   894,   894,   745,  1054,
     745,   209,   662,-32768,-32768,  1725,-32768,  6419,  6419,  6419,
  -32768,   939,-32768,-32768,  1139,   371,   896,  6419,  1159,  6419,
  -32768,-32768,    85,  5981,  4469,  1041,-32768,-32768,   899,   903,
     904,   971,-32768,-32768,   905,-32768,   906,-32768,   908,   909,
  -32768,   913,-32768,-32768,-32768,-32768,   914,   917,   996,   997,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   923,-32768,-32768,
    5669,   926,   933,-32768,   934,  1108,  6983,  6983,  5888,  6419,
  -32768,  1059,   935,-32768,  1164,-32768,-32768,   945,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     950,   940,   943,  1725,   970,   511,-32768,-32768,   964,  1245,
     570,-32768,-32768,  6419,-32768,  6419,  6419,   287,-32768,-32768,
  -32768,-32768,  6419,  6419,   655,-32768,  6419,-32768,-32768,   919,
    1115,  4343,-32768,  6419,  5888,   225,-32768,-32768,  6983,-32768,
    1195,   989,-32768,   156,-32768,   277,-32768,   115,-32768,   121,
  -32768,   121,-32768,   992,-32768,    96,-32768,  1100,-32768,   233,
  -32768,-32768,  1008,   990,-32768,   939,  1001,  1271,-32768,-32768,
    6419,-32768,-32768,  1009,-32768,   402,-32768,-32768,  1129,-32768,
    1197,  1198,-32768,   745,-32768,  1200,-32768,-32768,  1263,-32768,
    1269,-32768,-32768,  1250,-32768,  1725,-32768,  1725,   656,-32768,
    1285,  6419,-32768,  7563,-32768,   407,-32768,  6419,   749,   253,
    1122,  1091,  1165,  1128,-32768,-32768,-32768,   151,  1169,   472,
  -32768,  1111,   451,-32768,   745,-32768,  1288,  1216,-32768,   209,
  -32768,-32768,-32768,    37,  1125,-32768,-32768,-32768,  1130,-32768,
    1175,  1045,-32768,  1225,  1048,   600,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1046,-32768,-32768,
  -32768,   128,  1053,  1058,   308,-32768,  1048,-32768,-32768,   745,
  -32768,   713,-32768,  1256,  1329,-32768,   745,   745,-32768,  2784,
     648,-32768,  1220,-32768,  1192,  1075,-32768,  3691,  1221,-32768,
  -32768,-32768,  1231,  1078,  4723,  1083,  1160,  1237,  1291,  4215,
     184,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1706,   554,  1238,   700,  6983,
     114,-32768,  6419,  6983,   709,   732,   736,  1257,-32768,-32768,
    6983,  4977,  1164,   125,  1176,  6983,  6983,  4977,  1307,   171,
     171,  1092,   120,   341,   680,-32768,-32768,  4597,-32768,  6983,
    6419,  6983,  6983,  6983,  6983,  6983,  6983,  6107,  3303,-32768,
  -32768,  1349,  1089,  6419,-32768,  6419,  1331,  6419,-32768,  1201,
    1202,-32768,-32768,  1177,  4343,-32768,  1318,   684,-32768,  1109,
  -32768,  1318,  6419,  6419,  1113,-32768,   341,-32768,   431,  5116,
    1194,  6983,-32768,  6419,-32768,   602,-32768,-32768,   124,-32768,
    1203,-32768,-32768,-32768,  6419,   568,  6419,-32768,-32768,  6419,
    1041,-32768,  6983,  6419,  1376,  2784,-32768,  1116,  6983,  1153,
    6983,   745,-32768,   545,-32768,-32768,-32768,  1114,-32768,-32768,
  -32768,   173,  6419,-32768,-32768,-32768,-32768,  1378,-32768,  1119,
    1360,-32768,  1379,-32768,  1319,  1322,   548,-32768,-32768,-32768,
  -32768,-32768,  1390,-32768,-32768,-32768,-32768,-32768,  6419,  7754,
     451,  1289,  1290,  1292,-32768,   451,-32768,-32768,   633,-32768,
     832,-32768,-32768,  1262,   745,-32768,  5981,   316,  1259,-32768,
  -32768,-32768,   451,-32768,  1045,   330,-32768,-32768,-32768,  1360,
     330,  1270,   330,-32768,   451,   451,  1162,   451,-32768,-32768,
     939,  6419,-32768,  1342,-32768,   693,-32768,-32768,-32768,   594,
     991,  6419,   715,    71,-32768,-32768,  1166,   513,-32768,  5981,
    6419,   991,  5231,-32768,-32768,   991,  4723,-32768,-32768,-32768,
     118,-32768,   267,  1595,  4977,  4977,  6983,  6983,  1987,  1167,
     603,  2280,  6983,  2631,   708,  2897,  3163,  6326,  3429,  3695,
    3961,-32768,-32768,  1336,-32768,  1170,   743,  6983,  6983,   204,
    6983,  1168,  6983,  1172,   635,  6983,  6983,  6983,  6983,  6983,
    6983,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1386,
    1183,   164,  6983,-32768,  4977,  6983,   178,   582,   738,   116,
    6419,-32768,   743,-32768,-32768,  4851,-32768,  1182,-32768,  1184,
     583,-32768,-32768,  1427,   171,   171,   845,   845,  1164,-32768,
     719,-32768,-32768,-32768,-32768,-32768,   720,-32768,  1268,  1447,
    1331,-32768,-32768,  3835,  3835,  1340,-32768,   147,  1372,  6419,
  -32768,  6983,  1372,  1417,  1424,-32768,-32768,  4089,  1213,   401,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1217,  4977,  1425,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   325,
  -32768,-32768,  1238,  1164,-32768,-32768,   729,   991,-32768,-32768,
  -32768,  1164,-32768,   894,  1483,-32768,   545,-32768,  1223,  4977,
    1716,-32768,-32768,-32768,-32768,-32768,-32768,  1331,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   548,   548,  1258,  1224,-32768,
  -32768,-32768,-32768,-32768,   408,   153,   201,-32768,  1491,-32768,
  -32768,-32768,-32768,  6419,-32768,-32768,-32768,-32768,   298,   730,
    1351,  1349,  1236,-32768,  1111,   750,-32768,   616,-32768,  1239,
    6419,   618,  1243,  1244,   451,  1246,  1504,   648,  1331,   894,
    1198,  2784,-32768,  1169,  1334,  1800,-32768,   451,  1396,  1479,
  -32768,  3691,-32768,   451,   515,-32768,-32768,  1480,-32768,-32768,
    1265,  1272,-32768,  4977,  4977,-32768,  4977,  4977,-32768,  1525,
     162,  1164,  1264,-32768,-32768,  1267,  1164,  5888,-32768,-32768,
    1472,-32768,   191,  1274,  1275,  1164,  1205,  1277,  1279,  1164,
    6983,  6983,  1167,  6983,  6545,  1280,  1281,  1164,  1293,  1297,
    1164,  6983,  1164,  1298,  1299,  1164,  1300,  1302,  1164,  1303,
    1304,  1164,-32768,  5553,-32768,   643,   657,-32768,   666,-32768,
     694,  6983,-32768,   706,   747,   798,   858,   871,   946,  6983,
    6983,  6983,  1164,  1494,   272,  1164,  1373,  6983,-32768,  6983,
    6983,  6983,  6983,-32768,-32768,   755,  4851,  6200,-32768,-32768,
    6419,-32768,-32768,  6419,  7300,-32768,-32768,  6419,  6419,  6419,
    6419,  6419,  6419,  1355,  1273,-32768,  1296,-32768,-32768,  1355,
  -32768,   394,   394,   322,-32768,-32768,   761,-32768,  3835,  3835,
    1305,   542,  1306,  5116,  1454,-32768,  1454,   650,  1454,  1500,
    1501,  6983,  1023,  1569,  1516,-32768,-32768,-32768,   743,  1331,
  -32768,  1334,   356,  1111,  1460,  1111,-32768,  4977,   119,   727,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1716,  1716,
  -32768,  1447,  6983,  1716,   551,  1448,  1258,   231,-32768,  4977,
  -32768,-32768,-32768,  1467,  1470,  1471,  1473,  1475,  1478,-32768,
  -32768,-32768,-32768,-32768,  1325,   298,-32768,  1408,-32768,-32768,
     330,-32768,   393,   330,-32768,   330,-32768,  1349,-32768,-32768,
    1330,-32768,-32768,   594,  1447,  1111,-32768,  1198,-32768,-32768,
    1586,-32768,-32768,  1333,-32768,  1116,-32768,  1338,   745,   150,
    1343,-32768,-32768,   745,-32768,-32768,-32768,  1525,-32768,  1525,
    6983,   991,   991,  1350,   120,   772,  6983,  1547,-32768,   991,
     991,  6983,   991,   991,   185,  1164,-32768,  1219,  6983,  1164,
     991,   991,   991,   991,  1164,   991,   991,   991,   991,   991,
     991,-32768,  1353,  1344,   769,  6419,-32768,-32768,  1356,  1725,
  -32768,-32768,   959,-32768,-32768,-32768,-32768,-32768,-32768,  1074,
    1211,-32768,-32768,  6983,  6983,  1560,  1440,  1090,   775,  1150,
  -32768,-32768,-32768,-32768,  1725,  1358,-32768,-32768,  1581,  1598,
    6419,   329,  1361,  1364,  1554,  6983,  1367,  1368,  5762,  6419,
    6419,-32768,-32768,-32768,  1592,  7211,-32768,  1371,  1374,  1375,
  -32768,-32768,  1377,-32768,  1380,  1381,-32768,   169,  1382,  1383,
  -32768,-32768,-32768,  1384,  1564,  1385,  1387,  1389,  1391,  1392,
  -32768,  1369,   560,-32768,-32768,-32768,-32768,-32768,-32768,  1580,
  -32768,  3835,  3835,-32768,  1486,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1642,-32768,   401,-32768,-32768,
  -32768,  1579,-32768,  5116,  5116,   789,  6419,  6983,  4977,  1041,
  -32768,  1447,-32768,-32768,-32768,-32768,   832,-32768,-32768,   126,
  -32768,-32768,-32768,-32768,-32768,  1164,-32768,  1644,   551,-32768,
     697,  6419,-32768,  1589,   129,  1493,  1597,  1502,  1636,  1601,
    1641,  1331,-32768,-32768,  1409,-32768,  1411,  1599,  1644,   393,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1331,  1169,
  -32768,   832,  1491,  1565,-32768,-32768,-32768,  1584,    92,-32768,
  -32768,  1349,  1588,  1164,  1414,  1415,-32768,   743,-32768,  1164,
    6983,  1420,  1429,  1164,  1433,  1443,  6983,  6983,  1164,  1449,
    1451,  1453,  1455,  1458,  1459,  1462,  1465,  1468,  1474,-32768,
    5553,  1428,-32768,  6419,-32768,-32768,-32768,-32768,-32768,-32768,
    6983,  1476,-32768,-32768,-32768,  6983,-32768,-32768,  6983,   174,
  -32768,  1640,  6983,  6983,-32768,  4977,  1164,-32768,-32768,-32768,
  -32768,-32768,  6419,-32768,   283,-32768,  1528,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   179,  1481,-32768,-32768,-32768,  4851,
  -32768,-32768,-32768,-32768,-32768,-32768,  1607,-32768,-32768,-32768,
  -32768,   394,  6419,  5116,-32768,  1477,-32768,-32768,  1444,-32768,
    1164,   184,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1484,  6419,-32768,-32768,  1631,  1658,  6419,  1258,-32768,-32768,
     548,  6419,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1447,
  -32768,-32768,-32768,-32768,-32768,  1447,-32768,-32768,-32768,-32768,
  -32768,  6983,   745,-32768,  1543,-32768,   745,-32768,-32768,-32768,
    1164,-32768,-32768,-32768,-32768,  1164,  1164,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6419,  1670,
    1164,-32768,-32768,  1110,  1716,  1716,-32768,-32768,  1482,  1164,
    1266,   140,-32768,  1668,-32768,  6419,  6419,   298,    78,-32768,
  -32768,  1509,  1674,  4977,  1182,-32768,-32768,  1488,-32768,-32768,
    4977,  1489,-32768,-32768,-32768,  6983,-32768,-32768,-32768,-32768,
  -32768,  1258,   548,-32768,  1638,-32768,-32768,-32768,-32768,  1491,
  -32768,  1164,-32768,-32768,-32768,  1490,  1495,  1492,-32768,-32768,
  -32768,-32768,   991,  4851,  1596,  4851,-32768,-32768,-32768,-32768,
    7465,   283,  6983,  4851,   143,  6419,-32768,   184,  6419,-32768,
  -32768,  1638,   159,  1258,  1649,  1650,-32768,-32768,  1497,-32768,
    6419,  6419,  1498,  1182,  7465,  1182,-32768,-32768,  1295,   134,
    1731,   797,-32768,   801,  1258,   374,   374,-32768,  1735,-32768,
    1600,-32768,-32768,-32768,-32768,  1506,-32768,  1736,  4851,  1751,
    1741,  7465,  6419,-32768,-32768,-32768,  1590,   473,-32768,-32768,
  -32768,-32768,  7465,   105,  6419,  7465,-32768,-32768,-32768,-32768,
  -32768,-32768,  7465,-32768,-32768,-32768,  1790,  1793,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1276,  1639,-32768,  1643,  1496,   526,-32768,
    1014,-32768,-32768,-32768,  -842,-32768,  -526,-32768,  -513,  1645,
    1499,  1284,  1025,-32768,-32768,-32768,   649,-32768,  1120,   890,
  -32768,-32768,   671,-32768,-32768,   902,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   683,-32768,-32768,   941,-32768,-32768,   249,
  -32768,-32768,  1461,  1752,-32768,-32768,  -354,-32768,-32768,-32768,
    1432,-32768,-32768,  1208,  -374,-32768,-32768,-32768,   766,-32768,
   -1287,-32768,-32768,  1505,  1026,   702,  1464,  1002,   705,-32768,
     270,   -50,  -267,-32768,  -339,-32768, -1145,-32768,-32768,   295,
  -32768,  1515, -1136,-32768,-32768,-32768,-32768, -1271,   285,     3,
      19,     5,   -18,-32768,-32768,-32768,-32768,  -135,  -265,-32768,
    1029,-32768,  -705,-32768,  -955,-32768,   855,-32768,-32768,-32768,
  -32768,-32768, -1264,  -948,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -726,  -963,-32768,
  -32768,-32768,-32768,-32768,   210,-32768,    36,-32768,-32768,-32768,
  -32768,-32768,  1879,-32768,-32768,-32768,-32768,-32768,  1117,-32768,
  -32768,  -678,   342,  -338,  -818,   584,-32768,-32768,  -573,  -797,
  -32768,-32768, -1033, -1367,-32768,-32768,-32768,-32768,-32768,  1511,
  -32768,  1283,  1512,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1519,-32768,-32768,-32768,-32768,-32768,  -569,  -474,
  -32768,  1007,   536,  -612,  -385,-32768,-32768,-32768,-32768, -1047,
  -32768,  -381,-32768,-32768,-32768,-32768,  1248,-32768,-32768,  1020,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1889,-32768,-32768,-32768,-32768,-32768,  1573,-32768,-32768,-32768,
    1363,-32768,  1354,-32768,-32768,-32768,-32768,  1126,-32768,-32768,
  -32768,     1,-32768,-32768,-32768,  -459,   911,-32768,  -357,   717,
  -32768,  -245,   -62,-32768,-32768,-32768,-32768,  1756,-32768,-32768,
  -32768,  1127, -1157,-32768,-32768,  -938, -1402,-32768,-32768,-32768,
  -32768,   882,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1834,   -42,   642,  -205,-32768,-32768,   127,-32768,  -388,  -768,
    1215,-32768,   303,-32768,   520,-32768,  -839,-32768,  -661,-32768,
    1133,  -538,-32768,-32768,  -887,  1946,  1947,-32768,-32768,  1952,
  -32768,-32768,  1394,-32768,  -241,-32768,-32768,  1173, -1150,-32768,
     924,-32768,  -104,  -706,  -565,-32768,-32768,-32768,  -721,   -56,
    -470,  -629,-32768,  -473,-32768,-32768,-32768,   387,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -508,-32768,   763,
  -32768,  1171, -1007,-32768,  -429,-32768,   652,  -983,  -147,  -967,
    -961,  -941,   -52,    -2,     6,-32768,   -67,  -784,  -504,  -140,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768, -1214,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   718,-32768, -1050,-32768,
  -32768,   222,-32768,  -935,   610,  1087,   829,   255,  -658,  1362,
   -1199,-32768,   -64,  1874,-32768,   -30,   -49, -1605,   500,    34,
     -54,  -164,    -1,   -59,  -269,-32768,   -75,   -61,    -9,-32768
};


#define	YYLAST		8089


static const short yytable[] =
{
     148,    41,    82,   248,   345,   817,   298,   169,   222,   176,
     244,   316,   319,   241,   260,   168,   243,   227,   317,   320,
     657,   996,   265,   288,   661,   279,   998,   631,   285,   759,
     258,   233,  1245,   519,   841,   593,   595,   819,   287,   744,
    1500,   796,  1062,   622,   283,   289,   217,   220,   223,   226,
     169,   231,   234,   778,  1458,  1562,   956,   246,   220,   234,
    1017,   226,   282,   169,   231,   780,   263,   148,   234,  1214,
     290,  1255,   537,  1311,  1348,   918,   275,   255,   169,   278,
     220,   992,   275,   234,   223,   226,   168,   169,   231,   263,
     148,   234,   225,   226,   304,   286,   306,  1313,   354,  1281,
    1013,   255,   981,   560,   253,  1244,   348,   909,   989,   305,
    1369,   742,  1143,  1314,   597,   869,   355,   600,   411,  1315,
     321,   830,  1518,   831,   934,  1840,  1163,   934,   284,   842,
     352,  1694,  1321,   327,   934,  1536,   291,   934,  1067,  1316,
    1097,   246,   169,   246,  1889,  1317,   674,   148,   934,   891,
     549,   934,  1912,  1085,   234,  1064,  1087,   805,   418,   346,
     960,   220,   226,   169,   263,  1094,   792,   378,   176,  1598,
    1390,   810,   934,  1552,  1553,   982,   983,   742,  1556,   750,
     563,  1441,  1269,  1364,   751,   750,   924,  1445,  1560,  1743,
     751,    54,   934,  1616,   750,   861,   406,  1335,   102,   751,
     531,   579,    55,  1875,  1147,   350,   876,   822,   750,    56,
    1271,  1665,   412,   751,  1217,   565,  1082,  1774,   657,  -665,
     150,   923,   661,   302,  1138,  1739,   931,   750,  1237,  1238,
    1674,   862,   751,    57,   -80,  1791,   507,  1397,   800,   511,
    1872,   935,   267,  1164,   935,  1337,   181,   772,   750,    63,
     567,   935,   626,   751,   935,   631,   538,   383,   798,  1706,
     384,  1500,   519,    87,   544,   935,  1320,   852,   935,   996,
    1660,  1661,   152,  1324,   998,  1166,   268,   333,   178,   546,
     934,  1338,  1330,   246,   151,  1398,   246,  1907,   561,   935,
     556,  1799,  1784,   520,   148,    58,   564,   177,   182,   579,
     814,   551,   385,   413,  1738,  1168,  1169,   801,  1352,   935,
    1586,  1311,  1311,   877,   984,   889,  1311,   103,   594,   223,
    1000,  1272,  1744,   169,   303,   507,    59,   244,  1010,   608,
      60,   863,   612,  1336,    14,  1313,  1313,  1231,  1785,  1876,
    1313,   549,   742,   669,  1541,  1527,   623,  1148,   625,  1786,
     673,  1314,  1314,  1841,  1599,  1234,  1314,  1315,  1315,   563,
    1675,   683,  1315,   678,   750,   246,   580,  1239,  1692,   751,
     682,   806,   246,  1554,   246,   680,   246,  1316,  1316,   613,
    1246,  1125,  1316,  1317,  1317,  1101,   961,  1299,  1317,  1325,
    1326,  1242,  1167,  1642,  1643,  1165,  1550,   935,   220,   226,
     231,  1298,  1049,  1707,  1318,  1319,  1722,   892,   169,  1246,
     223,  1540,   303,   267,   691,  -376,  1590,  1834,  1162,    61,
    1870,   689,  1560,   424,  1332,   750,   752,   753,   754,   755,
     751,  -665,   752,   753,   754,   755,  1523,  1524,   550,  1374,
    1083,  1775,   671,   754,   755,  1443,  1813,   268,  1787,   923,
    1308,  1144,  1857,   923,   750,   752,   753,   754,   755,   751,
     746,   677,  1160,  1783,   627,  1284,  1161,   561,  1295,   844,
     102,   769,   785,   313,   752,   753,   754,   755,  1285,   773,
     776,   870,   371,   104,   781,   379,  -705,  1078,  1333,  1521,
    1080,  1896,   551,   896,    80,   752,   753,   754,   755,   407,
     794,  1286,   787,  1092,   169,   519,   226,   520,   809,   552,
     179,  1577,  1117,   169,   511,  1287,  1909,   169,  1117,  1296,
    1451,   105,   520,  1572,   176,  1452,   313,   372,  1108,   235,
    1897,   823,  1154,   106,  1386,  1387,   900,  1388,  1389,   918,
    1850,  1796,  1035,   105,   251,  1453,   845,   843,   795,   771,
     170,   313,  1288,   163,  1745,   871,  1736,   107,   380,  1275,
     313,   246,   774,  1575,  1363,   597,  1589,  1910,  1061,    81,
     237,   238,   872,  -238,  1151,  1653,  1866,  1898,  1280,   107,
    1375,   601,  1877,   999,   897,   408,   846,  1543,  1334,  1013,
      80,  1118,   836,  1703,   246,  1772,  1695,  1544,   836,   614,
    1887,  1124,  1276,  1895,   315,   750,  1284,  1155,  1301,   615,
     751,   752,   753,   754,   755,  1014,  1729,   899,   993,  1285,
     409,   887,   108,   244,   903,   904,  1500,  1906,   255,  1830,
    1831,   314,   109,  1735,   373,  1136,   171,   315,  1911,   951,
     318,  1914,  1286,  1073,   108,   563,   315,    80,  1915,  1180,
    1500,   389,   374,   164,   109,  1054,  1287,   110,   750,   321,
     165,   270,   952,   751,  1289,    81,   750,   172,   313,  -238,
     246,   751,   752,   753,   754,   755,   315,  1500,  1549,   110,
     750,   292,   173,   360,  1588,   751,   375,   888,  1500,   750,
    -238,  1500,   963,  1288,   751,   867,  -238,  1181,  1500,   322,
    1564,   752,   753,   754,   755,  1913,   321,   507,  1036,   507,
    1057,  1022,  1074,   234,  1284,   321,  1794,   750,   953,  1303,
    1749,  1182,   751,   361,   390,  1190,  1058,  1285,  1054,   750,
    -238,   591,  1708,  1607,   751,   272,  1709,  1710,   321,  1191,
    1033,  1002,   321,   868,   592,  1381,   957,   280,  1395,   246,
    1286,  1322,  1323,   561,   246,   965,   246,   329,   246,   244,
     396,   750,   556,  1331,  1060,   520,   751,  1311,  1311,  1072,
     750,  1702,  1737,   169,   226,   751,   507,  1382,   967,   294,
     226,  1817,   969,  1192,  1052,  1365,   763,  1818,   764,  1563,
     330,  1313,  1313,  1103,   397,   169,   398,   169,   313,    84,
      85,  1288,  1099,  1102,   246,  1289,   246,  1314,  1314,  1151,
      14,  1193,  1448,  1315,  1315,  1714,   331,   771,   399,   332,
    1394,   750,  1114,   246,   295,  1053,   751,  1132,  1133,   313,
    1135,   999,   365,  1316,  1316,   333,  1715,   779, -1013,  1317,
    1317, -1016,  1716,   400,   366,   767,  1576,   768,  1631,   246,
     246,  1585,   752,   753,   754,   755,  1109,  1240,   756,  1137,
    1637,   334,  1535,  1248,   299,   563,   255,   691,   750,  1145,
    1863,  1107,  1865,   751,  1115,  1194,  1717,   367,  1429,   368,
    1869,   750,   422,  1632,   300,  1806,   751,   301,  1122,  1807,
     427,  1353,   169,  1353,   750,  1633,  1354,   307,  1357,   751,
     255,   255,   169,   255,  1816,   752,   753,   754,   755,  1701,
     691,  1159,  1222,   752,   753,   754,   755,   689,   158,   159,
    1426,  1820,   443,   444,   445,  1903,   308,   752,   753,   754,
     755,   832,   312,   833,  1427,   159,   752,   753,   754,   755,
     958,   328,   962,  1430,   237,   238,   966,   968,   970,   344,
     453,   454,   854,   996,   855,   756,   356,   994,   998,  1029,
    1243,  1030,  1110,  1111,   752,   753,   754,   755,  1141,   750,
    1142,  1431,   357,   561,   751,   359,   752,   753,   754,   755,
    1649,   234,   750,  1433,  1054,  1687,  1689,   751,  1503,   381,
     914,  1506,  1146,   822,   756,  1250,  1249,  1251,  1688,  1690,
    1856,  1571,   363,  1858,  1141,  1345,  1300,  1346,   752,   753,
     754,   755,   392,  1241,   520,   520,   393,   752,   753,   754,
     755,  1884,  1885,  -695,  1434,  1350,  1781,  1351,   226,   395,
    1246,  1592,  1450,   792,  1394,  1394,   756,   404,  1522,  1882,
     415,   417,  1394,  1394,   657,  1394,  1394,   756,   661,  1608,
     756,   419,  1647,  1394,  1394,  1394,  1394,   420,  1394,  1394,
    1394,  1394,  1394,  1394,   756,   423,  1696,  1360,   752,   753,
     754,   755,  1892,   424,  1893,  1435,   767,   576,  1894,   577,
    1376,  1530,   999,  1532,  -695,   244,  1380,  -695,   508,   511,
     563,   512,   515,   522,  -695,   523,  -695,   750,   526,   527,
    -695,   528,   751,   545,  1343,  -695,   529,   996,  -695,   996,
     530,   532,   998,   750,   998,   547,  1344,   996,   751,   548,
     557,  1343,   998,  -695,  1516,  1516,  1520,   568,   752,   753,
     754,   755,   246,   750,   572,  1436,   246,  -695,   751,   570,
     255,   752,   753,   754,   755,  -695,    14,   584,  1437,  -695,
     585,   586,   750,   255,   588,   609,   606,   751,  -695,   255,
     619,   620,   996,  1546,  1844,  1548,   624,   998,   675,   681,
     721,  1847,   679,   750,  -695,   718,   716, -1041,   751,   719,
     720,   722,   723, -1041,   724,   725,  1507,   750, -1041,   726,
    -999, -1041,   751,   727,   507,   728,   729,   244,   561,   730,
    1505,  -695,   735,  1454,   169,   738,   750,   747,  1508,   736,
     737,   751,  1425,  -695,   749,  -695,   752,   753,   754,   755,
     756,   758,   852,  1438, -1016,  1591,   757, -1041,   750,   752,
     753,   754,   755,   751,   750, -1041,  1638,   762,   246,   751,
     765,   246,   750, -1041,  1459,  1502,  1230,   751,   520,   226,
     231,   520,   263,   148, -1042,   766,   783,  1741,   789,  1401,
   -1042, -1041,  -695, -1041,   791, -1042,   808,   804, -1042,   520,
     520,   813,  1640,  1617,   226,   812, -1041,   815, -1041, -1041,
   -1041,   816,  -695,  1873,   818,   825,  -695,   821,   579,   750,
     824,   827,  1504,   828,   751,   834,   858,  -695,  -695,   859,
    -695,   860,  -436,   820, -1042,   865,   874,   313, -1043,   873,
     879,   563, -1042,   881, -1043,   880,   884,   901,   750, -1043,
   -1042,   882, -1043,   751,   885,   890,  1597, -1041,  1184,   894,
    1188,  1602,  1196,  1199,   895,  1204,  1207,  1210, -1042,   902,
   -1042,   913,   911,  1574,   752,   753,   754,   755,  1833,   919,
     914,  1639,   920, -1042,   921, -1042, -1042, -1042, -1043,   925,
     752,   753,   754,   755,   181,   985, -1043,  1646,   990,   991,
     891,   315,  1697,  1018, -1043,  1023,  1024,  1888,  1027,  1025,
     752,   753,   754,   755,  1048,  1031,  1065,  1829,   774,  1055,
    1079,   767, -1043,  1069, -1043,  1089,  1652,  1088,  1819,   752,
     753,   754,   755,  1090, -1042,  1095,   837, -1043,  1096, -1043,
   -1043, -1043,  1098,  1120,  1104,  1105,   169,  1106,  1113,   561,
     752,   753,   754,   755,  1425,  1648,  1130, -1041, -1041, -1041,
   -1041, -1041, -1041, -1041,   752,   753,   754,   755,  1134, -1041,
    1139, -1041,  1153,  1177,  1212,  1219,  1213,  1221,   -48,  1229,
     750,   220,  1252,   752,   753,   754,   755,  1246, -1043,  1659,
    1159,  1159,   474,  1254,  1270,  1247,  1502,   971,   972,   973,
     974,   975,   976,   977,   978,   752,   753,   754,   755,  1273,
    1278,   752,   753,   754,   755,   535,   -48,  1279,  1283,   752,
     753,   754,   755,  1291,   543,  1304,  1293,  1327,   -48,  1307,
    1329,  1339,   520,   520, -1042, -1042, -1042, -1042, -1042, -1042,
   -1042,  1347,   153,  1349,  1362,   999, -1042,  1355, -1042,  1720,
    1358,  1359,   -48,  1361,   226,   226,  1370,   346,  1798,  1147,
     575,  1378,  1383,   934,   578,  1396,   752,   753,   754,   755,
    1391,  1284,  1384,  1392,  1516,  1442,  1444,  1509,  1511,  1385,
    1399,  1400,   169,  1402,  1285,  1403,  1410,  1411, -1043, -1043,
   -1043, -1043, -1043, -1043, -1043,   752,   753,   754,   755,  1412,
   -1043,  1512, -1043,  1413,  1415,  1416,  1417,  1286,  1418,  1419,
    1420,  1528,  1525,  1526,  1769,  1533,  1534,   -48,  1537,  1538,
    1547,  1287,  1565,  1561,    88,  1566,  1567,   -48,  1568,  1800,
    1569,    89,  1573,  1570,  1117,    90,    91,  1587,  1593,  1595,
    1610,  1644,   936,  1645,    92,  1596,  1814,  1651,   750,  1630,
    1601,   169,   -48,   751,   223,    93,   937,  1606,  1288,  1425,
    1629,   732,  1650,  1636, -1005,  1654,   151,   739,   740,   743,
    1655,  1657,  -338,  1663,   154,  1668,  1679,  1685,  1669,  1670,
    1686,  1671,   792,  1159,  1672,  1673,  1676,  1677,  1678,  1680,
    1691,  1681,   938,  1682,  1693,  1683,  1684,  1711,  1721,   999,
     939,   999,  1815,  1723,  1812,  1792,   563,  1724,   940,   999,
    1726,  1727,  1725,   346,   226,  1728,  1730,    94,  1731,  1740,
    1822,  1747,  1748,  1732,  1824,  1742,   941,  1751,   942,  1746,
     563,  1778,  1002,    95,  1768,   786,  1752,   169,  1851,   788,
    1753,   943,   223,   944,   945,   946,  1801,  1664,    96,  1805,
    1754,  1836,    97,   936,   999,  1810,  1757,   563,  1758,   750,
    1759,  1795,  1760,    98,   751,  1761,  1762,   937,   563,  1763,
    1289,   563,  1764,   633,  1811,  1765,   634,  1823,   563,  1827,
    1835,  1766,    99,  1771,   431,  1842,  1843,  1793,  1832,   223,
    1852,  1808,   947,   635,  1845,  1848,   636,  1859,  1861,  1864,
    1860,  1878,  1880,   938,  1883,  1886,   220,  1838,  1891,  1875,
    1876,   939,   433,  1901,   561,  1839,  1902,  1904,  1905,   940,
    1917,  1908,   511,  1918,   309,  1513,   514,  1034,   310,  1026,
    1379,   311,   521,   912,  1152,   782,   637,   941,   561,   942,
     435,   100,   638,   639,   640,   784,   641,   642,   643,  1377,
     644,  1157,   943,  1367,   944,   945,   946,   837,  1734,   266,
    1126,  1502,   589,  1862,   632,   561,  1052,   875,   645,   246,
     646,  1066,  1306,  1368,  1093,  1719,   561,   602,  1733,   561,
    1373,   223,   223,  1713,  1874,  1502,   561,   566,  1900,  1879,
      65,  1086,   948,   949,   950,   752,   753,   754,   755,   647,
     933,    66,   993,   947,  1881,  1256,  1790,  1867,    67,    27,
    1015,   440,  1502,  1052,  1704,  1542,   648,   596,   587,  1584,
     959,    68,   857,  1502,   964,  1159,  1502,   442,   604,  1131,
     610,   980,   157,  1502,  1123,   803,   987,   988,   649,   558,
     799,   443,   444,   445,   898,   237,   238,   326,  1050,  1282,
     474,   276,  1003,  1004,  1005,  1006,  1007,  1008,   474,  1531,
    1056,   954,  1849,  1767,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   650,   651,  1635,    46,    47,    69,   458,
     459,  1068,    50,   811,  1032,  1406,  1277,  1446,   463,  1356,
     652,  1551,   543,  1116,    70,   281,  1809,     0,  1658,   853,
       0,     0,     0,   948,   949,   950,   752,   753,   754,   755,
      71,     0,     0,  1063,     0,     0,     0,  1309,     0,   575,
       0,  1071,     0,  1172,     0,    72,  1173,     0,   111,    73,
     428,   112,    74,   113,   114,   115,   116,   429,   117,     0,
      75,     0,     0,     0,     0,     0,   118,   119,     0,   430,
       0,     0,     0,     0,     0,   431,   432,   120,   121,    76,
     122,     0,     0,     0,     0,     0,     0,   123,   124,     0,
     125,   126,     0,   127,     0,     0,   128,   129,   130,   131,
     132,     0,     0,   433,   133,   134,   434,     0,     0,     0,
     135,   136,   137,     0,   138,   139,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,     0,
       0,   435,     0,   436,   437,     0,     0,     0,    77,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1170,  1171,  1176,
       0,     0,  1185,  1186,  1189,     0,  1197,  1200,  1202,  1205,
    1208,  1211,     0,     0,     0,     0,     0,     0,  1215,  1216,
       0,  1218,     0,  1220,     0,     0,  1223,  1224,  1225,  1226,
    1227,  1228,     0,     0,     0,     0,     0,     0,     0,     0,
    1174,     0,   440,  1232,     0,     0,  1235,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   237,   238,   112,     0,
     113,   114,   115,   116,     0,   117,   447,     0,     0,     0,
       0,     0,   474,   118,   119,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   120,   457,     0,   122,     0,     0,
     458,   459,     0,   460,   461,   462,     0,   125,   126,   463,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   464,   137,
       0,   465,   139,   140,     0,     0,     0,   466,   467,     0,
       0,     0,     0,   534,     0,   469,     0,     0,     0,   315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1183,     0,     0,  1173,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
     429,     0,  1604,  1605,     0,     0,     0,     0,     0,     0,
    1611,  1612,   430,  1614,  1615,     0,     0,     0,   431,   432,
       0,  1619,  1620,  1621,  1622,     0,  1623,  1624,  1625,  1626,
    1627,  1628,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,   474,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1404,  1405,     0,  1407,  1409,     0,     0,     0,     0,
       0,     0,  1414,     0,   435,     0,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,     0,     0,     0,
     439,     0,  1432,     0,     0,     0,     0,     0,     0,     0,
    1439,  1440,  1232,     0,     0,     0,     0,     0,  1232,     0,
    1235,  1447,   474,  1449,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1174,     0,   440,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   442,   474,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   237,
     238,   112,     0,   113,   114,   115,   116,     0,   117,   447,
       0,     0,     0,  1555,     0,     0,   118,   119,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   120,   457,     0,
     122,     0,     0,   458,   459,     0,   460,   461,   462,     0,
     125,   126,   463,   127,     0,     0,   128,   129,   130,   131,
     132,     1,     0,     0,   133,   134,     0,     0,     0,     0,
     135,   464,   137,     0,   465,   139,   140,     0,     0,     2,
     466,   467,     0,     0,     0,     0,   534,     3,   469,     0,
       0,  1603,   315,     0,     4,     0,     5,  1609,     6,     0,
       0,     0,  1613,     7,     0,     0,     0,     0,     0,  1618,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1232,  1232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1656,  1187,     0,     0,
    1173,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,    11,     0,   431,
     432,     0,     0,     0,    12,     0,     0,    13,     0,    14,
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
     434,     0,     0,     0,    16,     0,     0,     0,  1700,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1750,     0,     0,     0,     0,     0,  1755,  1756,     0,
       0,     0,     0,     0,  1174,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,  1770,   442,     0,   601,     0,  1232,     0,     0,  1773,
       0,     0,     0,  1779,  1780,     0,   443,   444,   445,   446,
     237,   238,   112,     0,   113,   114,   115,   116,     0,   117,
     447,     0,     0,     0,     0,     0,     0,   118,   119,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   120,   457,
       0,   122,     0,     0,   458,   459,     0,   460,   461,   462,
       0,   125,   126,   463,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   464,   137,     0,   465,   139,   140,     0,     0,
       0,   466,   467,  1195,     0,     0,  1173,   534,     0,   469,
     428,     0,  1821,   315,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   431,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1700,     0,     0,     0,
       0,     0,   111,     0,     0,   112,     0,   113,   114,   115,
     116,   435,   117,   436,   437,     0,     0,     0,     0,     0,
     118,   119,     0,   438,     0,     0,     0,   439,     0,     0,
       0,   120,   121,  1868,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,   125,   126,     0,   127,     0,     0,
     128,   129,   130,   131,   132,     0,     0,     0,   133,   134,
       0,     0,     0,     0,   135,   136,   137,     0,   138,   139,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1174,     0,   440,     0,     0,     0,     0,   607,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   237,   238,   112,     0,
     113,   114,   115,   116,     0,   117,   447,     0,     0,     0,
       0,     0,     0,   118,   119,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   120,   457,     0,   122,     0,     0,
     458,   459,     0,   460,   461,   462,     0,   125,   126,   463,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   464,   137,
       0,   465,   139,   140,     0,     0,     0,   466,   467,  1198,
       0,     0,  1173,   534,     0,   469,   428,     0,     0,   315,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,   434,     0,     0,   111,     0,     0,   112,     0,
     113,   114,   115,   116,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   118,   119,     0,     0,   435,     0,   436,
     437,     0,     0,     0,   120,   121,     0,   122,     0,   438,
       0,     0,     0,   439,   123,   124,     0,   125,   126,     0,
     127,     0,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   136,   137,
       0,   138,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1174,     0,   440,     0,
       0,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,  1011,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   237,   238,   112,     0,   113,   114,   115,   116,
       0,   117,   447,     0,     0,     0,     0,     0,     0,   118,
     119,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     120,   457,     0,   122,     0,     0,   458,   459,     0,   460,
     461,   462,     0,   125,   126,   463,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   464,   137,     0,   465,   139,   140,
       0,     0,     0,   466,   467,  1203,     0,     0,  1173,   534,
       0,   469,   428,     0,     0,   315,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,   431,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,   434,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,   435,     0,   436,   437,     0,     0,     0,
     120,   121,     0,   122,     0,   438,     0,     0,     0,   439,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,  1012,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1174,     0,   440,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   237,   238,
     112,     0,   113,   114,   115,   116,     0,   117,   447,     0,
       0,     0,     0,     0,     0,   118,   119,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   120,   457,     0,   122,
       0,     0,   458,   459,     0,   460,   461,   462,     0,   125,
     126,   463,   127,     0,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     464,   137,     0,   465,   139,   140,     0,     0,     0,   466,
     467,  1206,     0,     0,  1173,   534,     0,   469,   428,   915,
       0,   315,   634,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   916,     0,   430,     0,   635,
       0,     0,   636,   431,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,   434,     0,     0,     0,     0,     0,
       0,     0,   637,     0,     0,     0,     0,     0,   638,   639,
     640,     0,   641,   642,   643,     0,   644,     0,     0,   435,
       0,   436,   437,     0,     0,     0,     0,     0,     0,     0,
       0,   438,     0,     0,   645,   439,   646,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   648,     0,     0,     0,     0,     0,  1174,     0,
     440,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,   649,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   237,   238,   112,     0,   113,   114,
     115,   116,     0,   117,   447,     0,  1257,     0,     0,   650,
     651,   118,   119,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   120,   457,     0,   122,   652,     0,   458,   459,
       0,   460,   461,   462,     0,   125,   126,   463,   127,     0,
       0,   128,   129,   130,   131,   132,     0,     0,     0,   133,
     134,     0,     0,     0,     0,   135,   464,   137,     0,   465,
     139,   140,     0,     0,     0,   466,   467,  1209,     0,     0,
    1173,   534,     0,   469,   428,     0,  1258,   315,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1259,     0,     0,     0,     0,  1260,     0,     0,     0,
       0,     0,  1261,     0,     0,     0,     0,   433,     0,     0,
     434,     0,     0,   111,     0,     0,   112,     0,   113,   114,
     115,  1262,     0,   117,     0,     0,     0,     0,     0,     0,
       0,   118,   119,     0,     0,   435,     0,   436,   437,     0,
       0,     0,   120,   121,     0,   122,     0,   438,     0,     0,
       0,   439,   123,   124,     0,   125,   126,     0,   127,     0,
       0,   128,   129,   130,   131,   132,     0,     0,     0,   133,
     134,     0,     0,     0,     0,   135,   136,   137,     0,   138,
     139,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1174,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     237,   238,   112,     0,   113,   114,   115,   116,     0,   117,
     447,     0,     0,     0,     0,     0,     0,   118,   119,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   120,   457,
       0,   122,     0,     0,   458,   459,     0,   460,   461,   462,
       0,   125,   126,   463,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   464,   137,     0,   465,   139,   140,   428,     0,
       0,   466,   467,     0,     0,   429,     0,   534,     0,   469,
       0,     0,     0,   315,    14,     0,     0,   430,     0,     0,
       0,     0,     0,   431,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   693,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,   434,     0,     0,   111,     0,     0,
     112,     0,   113,   114,   115,   116,     0,   117,     0,     0,
       0,     0,     0,     0,     0,   118,   119,     0,     0,   435,
       0,   436,   437,     0,     0,     0,   120,   121,     0,   122,
       0,   438,     0,     0,     0,   439,   123,   124,     0,   125,
     126,     0,   127,   926,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     136,   137,     0,   138,   139,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1037,     0,     0,     0,     0,
      14,     0,     0,     0,   695,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     443,   444,   445,   446,   237,   238,   112,     0,   113,   114,
     115,   116,     0,   117,   447,     0,     0,     0,     0,     0,
       0,   118,   119,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   120,   457,     0,   122,     0,     0,   458,   459,
       0,   460,   461,   462,     0,   125,   126,   463,   127,     0,
       0,   128,   129,   130,   131,   132,   927,   928,   929,   133,
     134,     0,     0,     0,     0,   135,   464,   137,     0,   465,
     139,   140,   428,     0,     0,   466,   467,     0,     0,   429,
       0,   699,     0,   469,     0,     0,     0,   315,     0,     0,
       0,   430,     0,     0,   692,     0,     0,   431,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,     0,   693,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,   434,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,   435,     0,   436,   437,     0,     0,     0,
     120,   121,     0,   122,     0,   438,     0,     0,     0,   439,
     123,   124,     0,   125,   126,     0,   127,   694,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   695,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   237,   238,
     112,     0,   113,   114,   115,   116,     0,   117,   447,     0,
       0,     0,     0,     0,     0,   118,   119,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   120,   457,     0,   122,
       0,     0,   458,   459,     0,   460,   461,   462,     0,   125,
     126,   463,   127,     0,     0,   128,   129,   696,   697,   698,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     464,   137,     0,   465,   139,   140,   428,     0,     0,   466,
     467,     0,     0,   429,     0,   699,     0,   469,     0,     0,
       0,   315,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   431,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     693,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,   434,     0,     0,   111,     0,     0,   112,     0,
     113,   114,   115,   116,     0,   117,     0,     0,     0,     0,
       0,     0,     0,   118,   119,     0,     0,   435,     0,   436,
     437,     0,     0,     0,   120,   121,     0,   122,     0,   438,
       0,     0,     0,   439,   123,   124,     0,   125,   126,     0,
     127,   922,     0,   128,   129,   130,   131,   132,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   136,   137,
       0,   138,   139,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   995,     0,   469,     0,     0,     0,     0,
       0,     0,   695,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   237,   238,   112,     0,   113,   114,   115,   116,
       0,   117,   447,     0,     0,     0,     0,     0,     0,   118,
     119,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     120,   457,     0,   122,     0,     0,   458,   459,     0,   460,
     461,   462,     0,   125,   126,   463,   127,     0,     0,   128,
     129,   696,   697,   698,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   464,   137,     0,   465,   139,   140,
     428,     0,     0,   466,   467,     0,     0,   429,     0,   699,
       0,   469,     0,     0,     0,   315,     0,     0,     0,   430,
       0,     0,     0,     0,     0,   431,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   693,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,   434,     0,     0,   111,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   118,   119,     0,
       0,   435,     0,   436,   437,     0,     0,     0,   120,   121,
       0,   122,     0,   438,     0,     0,     0,   439,   123,   124,
       0,   125,   126,     0,   127,   694,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   136,   137,     0,   138,   139,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,   695,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   237,   238,   112,     0,
     113,   114,   115,   116,     0,   117,   447,     0,     0,     0,
       0,     0,     0,   118,   119,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   120,   457,     0,   122,     0,     0,
     458,   459,     0,   460,   461,   462,     0,   125,   126,   463,
     127,     0,     0,   128,   129,   696,   697,   698,     0,     0,
       0,   133,   134,     0,     0,     0,     0,   135,   464,   137,
       0,   465,   139,   140,   428,     0,     0,   466,   467,     0,
       0,   429,     0,   699,     0,   469,     0,     0,     0,   315,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   693,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,   112,     0,   113,
     114,   115,   116,     0,   117,   435,     0,   436,   437,     0,
       0,     0,   118,   119,     0,     0,     0,   438,     0,     0,
       0,   439,     0,   120,   121,     0,   122,     0,     0,   922,
       0,     0,     0,   123,   124,     0,   125,   126,     0,   127,
       0,     0,   128,   129,   130,   131,   132,     0,     0,   104,
     133,   134,     0,     0,     0,     0,   135,   136,   137,     0,
     138,   139,   140,     0,     0,     0,     0,     0,     0,     0,
     695,     0,  1037,     0,     0,     0,   440,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,   105,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,   443,   444,   445,   446,
     237,   238,   112,     0,   113,   114,   115,   116,     0,   117,
     447,     0,     0,   107,     0,     0,     0,   118,   119,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   120,   457,
       0,   122,     0,   428,   458,   459,     0,   460,   461,   462,
     429,   125,   126,   463,   127,     0,     0,   128,   129,   130,
     131,   132,   430,     0,     0,   133,   134,     0,   431,   432,
       0,   135,   464,   137,     0,   465,   139,   140,     0,     0,
       0,   466,   467,     0,     0,     0,     0,   699,   108,   469,
       0,     0,     0,   315,     0,     0,   433,     0,   109,   434,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,   435,     0,   436,   437,     0,     0,
       0,     0,     0,     0,     0,     0,   438,     0,     0,     0,
     439,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,     0,     0,     0,     0,     0,     0,   118,
     119,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     120,   121,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,   440,     0,   133,   134,     0,
       0,   441,     0,   135,   136,   137,     0,   138,   139,   140,
       0,   442,     0,     0,     0,     0,     0,     0,   951,     0,
       0,     0,     0,     0,     0,   443,   444,   445,   446,   237,
     238,   112,     0,   113,   114,   115,   116,     0,   117,   447,
       0,   952,     0,     0,     0,     0,   118,   119,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   120,   457,     0,
     122,     0,   428,   458,   459,     0,   460,   461,   462,   429,
     125,   126,   463,   127,     0,     0,   128,   129,   130,   131,
     132,   430,     0,     0,   133,   134,     0,   431,   432,     0,
     135,   464,   137,     0,   465,   139,   140,   953,     0,     0,
     466,   467,   539,     0,     0,     0,   534,     0,   469,     0,
       0,     0,   315,     0,     0,   433,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,     0,   435,     0,   436,   437,     0,     0,   118,
     119,     0,     0,     0,     0,   438,     0,     0,     0,   439,
     120,   121,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   731,     0,
       0,     0,     0,     0,   443,   444,   445,   446,   237,   238,
     112,     0,   113,   114,   115,   116,     0,   117,   447,     0,
       0,     0,     0,     0,     0,   118,   119,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   120,   457,     0,   122,
       0,   428,   458,   459,     0,   460,   461,   462,   429,   125,
     126,   463,   127,     0,     0,   128,   129,   130,   131,   132,
     430,     0,     0,   133,   134,     0,   431,   432,     0,   135,
     464,   137,     0,   465,   139,   140,     0,     0,     0,   466,
     467,     0,     0,     0,     0,   534,     0,   469,     0,     0,
       0,   315,     0,     0,   433,     0,     0,   434,     0,     0,
     111,     0,     0,   112,     0,   113,   114,   115,   116,     0,
     117,     0,     0,     0,     0,     0,     0,     0,   118,   119,
       0,     0,   435,     0,   436,   437,     0,     0,     0,   120,
     121,     0,   122,     0,   438,     0,     0,     0,   439,   123,
     124,     0,   125,   126,     0,   127,     0,     0,   128,   129,
     130,   131,   132,     0,     0,     0,   133,   134,     0,     0,
       0,     0,   135,   136,   137,     0,   138,   139,   140,     0,
       0,     0,     0,     0,     0,     0,  -339,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   446,   237,   238,   112,
       0,   113,   114,   115,   116,     0,   117,   447,     0,     0,
       0,     0,     0,     0,   118,   119,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   120,   457,     0,   122,     0,
     428,   458,   459,     0,   460,   461,   462,   429,   125,   126,
     463,   127,     0,     0,   128,   129,   130,   131,   132,   430,
       0,     0,   133,   134,     0,   431,   432,     0,   135,   464,
     137,     0,   465,   139,   140,     0,     0,     0,   466,   467,
       0,     0,     0,     0,   534,     0,   469,     0,     0,     0,
     315,     0,     0,   433,     0,     0,   434,    80,     0,   111,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   118,   119,     0,
       0,   435,     0,   436,   437,     0,     0,     0,   120,   121,
       0,   122,     0,   438,     0,     0,     0,   439,   123,   124,
       0,   125,   126,     0,   127,     0,     0,   128,   129,   130,
     131,   132,     0,     0,     0,   133,   134,     0,     0,     0,
       0,   135,   136,   137,     0,   138,   139,   140,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   237,   238,   112,     0,
     113,   114,   115,   116,     0,   117,   447,     0,     0,     0,
       0,     0,     0,   118,   119,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   120,   457,     0,   122,     0,   428,
     458,   459,     0,   460,   461,   462,   429,   125,   126,   463,
     127,     0,     0,   128,   129,   130,   131,   132,   430,     0,
       0,   133,   134,     0,   431,   432,     0,   135,   464,   137,
       0,   465,   139,   140,     0,     0,     0,   466,   467,     0,
       0,     0,     0,   534,  1009,   469,     0,     0,     0,   315,
       0,     0,   433,     0,     0,   434,     0,     0,   111,     0,
       0,   112,     0,   113,   114,   115,   116,     0,   117,     0,
       0,     0,     0,     0,     0,     0,   118,   119,     0,     0,
     435,     0,   436,   437,     0,     0,     0,   120,   121,     0,
     122,     0,   438,     0,     0,     0,   439,   123,   124,     0,
     125,   126,     0,   127,     0,     0,   128,   129,   130,   131,
     132,     0,     0,     0,   133,   134,     0,     0,     0,     0,
     135,   136,   137,     0,   138,   139,   140,     0,     0,     0,
       0,     0,  1012,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1201,     0,
       0,   443,   444,   445,   446,   237,   238,   112,     0,   113,
     114,   115,   116,     0,   117,   447,     0,     0,     0,     0,
       0,     0,   118,   119,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   120,   457,     0,   122,     0,   428,   458,
     459,     0,   460,   461,   462,   429,   125,   126,   463,   127,
       0,     0,   128,   129,   130,   131,   132,   430,     0,     0,
     133,   134,     0,   431,   432,     0,   135,   464,   137,     0,
     465,   139,   140,     0,     0,     0,   466,   467,     0,     0,
       0,     0,   534,     0,   469,     0,     0,     0,   315,     0,
       0,   433,     0,     0,   434,     0,     0,   111,     0,     0,
     112,     0,   113,   114,   115,   116,     0,   117,     0,     0,
       0,     0,     0,     0,     0,   118,   119,     0,     0,   435,
       0,   436,   437,     0,     0,     0,   120,   121,     0,   122,
       0,   438,     0,     0,     0,   439,   123,   124,     0,   125,
     126,     0,   127,     0,     0,   128,   129,   130,   131,   132,
       0,     0,     0,   133,   134,     0,     0,     0,     0,   135,
     136,   137,     0,   138,   139,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1408,     0,     0,
     443,   444,   445,   446,   237,   238,   112,     0,   113,   114,
     115,   116,     0,   117,   447,     0,     0,     0,     0,     0,
       0,   118,   119,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   120,   457,     0,   122,     0,   428,   458,   459,
       0,   460,   461,   462,   429,   125,   126,   463,   127,     0,
       0,   128,   129,   130,   131,   132,   430,     0,     0,   133,
     134,     0,   431,   432,     0,   135,   464,   137,     0,   465,
     139,   140,     0,     0,     0,   466,   467,     0,     0,     0,
       0,   534,     0,   469,     0,     0,     0,   315,     0,     0,
     433,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
     436,   437,     0,     0,     0,     0,     0,     0,     0,     0,
     438,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   443,
     444,   445,   446,   237,   238,   112,     0,   113,   114,   115,
     116,     0,   117,   447,     0,     0,     0,     0,     0,     0,
     118,   119,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   120,   457,     0,   122,     0,   428,   458,   459,     0,
     460,   461,   462,   429,   125,   126,   463,   127,     0,     0,
     128,   129,   130,   131,   132,   430,     0,     0,   133,   134,
       0,   431,   432,     0,   135,   464,   137,     0,   465,   139,
     140,     0,     0,     0,   466,   467,     0,     0,     0,     0,
     468,     0,   469,     0,     0,     0,   315,     0,     0,   433,
       0,     0,   434,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,   444,
     445,   446,   237,   238,   112,     0,   113,   114,   115,   116,
       0,   117,   447,     0,     0,     0,     0,     0,     0,   118,
     119,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     120,   457,     0,   122,     0,     0,   458,   459,     0,   460,
     461,   462,     0,   125,   126,   463,   127,  1254,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   464,   137,     0,   465,   139,   140,
       0,     0,     0,   466,   467,     6,     0,     0,     0,   534,
       0,   469,  -293,     0,     0,   315,  1460,  1461,     0,  1462,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1463,     0,     0,     0,     0,
       0,  1464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     0,     0,     0,  1465,     0,
       0,  -295,     0,     0,     0,  1460,  1461,     0,  1462,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,  1463,     0,     0,     0,     0,     0,
    1464,  1466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,     0,     0,
    1664,     0,     0,     0,     0,     0,     0,     0,     0,   446,
       0,     0,   112,     0,   113,   114,   115,   116,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   118,   119,     0,
       0,     0,     0,     0,     0,     0,     0,  1465,  1467,   121,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
       0,   125,   126,     0,   127,    14,     0,   128,  1468,   130,
     131,   132,     0,     0,     0,   133,   134,  1469,  1470,  1471,
    1466,   135,   136,   137,     0,   138,   139,   140,     0,     0,
      16,  1254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   446,     0,
       0,   112,     0,   113,   114,   115,   116,     0,   117,     6,
       0,     0,     0,     0,     0,     0,   118,   119,     0,     0,
    1460,  1461,     0,  1462,     0,     0,     0,  1467,   121,     0,
     122,     0,     0,     0,     0,     0,     0,   123,   124,  1463,
     125,   126,     0,   127,     0,  1464,   128,  1468,   130,   131,
     132,     0,     0,     0,   133,   134,  1469,  1470,  1471,     0,
     135,   136,   137,     0,   138,   139,   140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   633,     0,     0,   634,     0,     0,     0,     0,     0,
     837,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   635,  1465,     0,   636,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1466,     0,     0,     0,     0,
       0,     0,     0,     0,   637,    16,     0,     0,     0,     0,
     638,   639,   640,     0,   641,   642,   643,     0,   644,     0,
       0,     0,     0,   446,     0,     0,   112,     0,   113,   114,
     115,   116,     0,   117,     0,     0,   645,     0,   646,     0,
       0,   118,   119,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1467,   121,     0,   122,     0,     0,     0,     0,
       0,     0,   123,   124,     0,   125,   126,   647,   127,     0,
       0,   128,  1468,   130,   131,   132,     0,     0,     0,   133,
     134,  1469,  1470,  1471,   648,   135,   136,   137,     0,   138,
     139,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   649,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,   112,     0,   113,   114,   115,   116,
       0,   117,   633,     0,     0,   634,     0,     0,     0,   118,
     119,   650,   651,     0,     0,     0,     0,     0,     0,     0,
     120,   121,   635,   122,     0,   636,     0,     0,   652,     0,
     123,   124,     0,   125,   126,     0,   127,     0,     0,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,     0,
       0,     0,     0,   135,   136,   137,     0,   138,   139,   140,
       0,     0,     0,     0,     0,   637,     0,     0,     0,     0,
       0,   638,   639,   640,     0,   641,   642,   643,     0,   644,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   645,     0,   646,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   647,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   648,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   649,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,   112,     0,   113,   114,   115,
     116,     0,   117,     0,     0,     0,     0,     0,     0,     0,
     118,   119,   650,   651,     0,     0,     0,     0,     0,     0,
       0,   120,   121,     0,   122,     0,     0,     0,     0,   652,
       0,   123,   124,     0,   125,   126,     0,   127,     0,     0,
     128,   129,   130,   131,   132,     0,     0,     0,   133,   134,
       0,     0,     0,     0,   135,   136,   137,     0,   138,   139,
     140,   111,     0,     0,   112,   185,   113,   114,   115,   116,
     186,   117,   187,   188,   189,   190,   191,   192,   193,   118,
     119,   194,   195,   196,   197,   198,     0,     0,   199,   200,
     120,   121,   201,   122,     0,     0,   202,   203,   204,   205,
     123,   124,   206,   125,   126,   207,   127,   208,   209,   128,
     129,   130,   131,   132,     0,     0,     0,   133,   134,   210,
     211,   212,   213,   135,   136,   137,   214,   138,   139,   140
};

static const short yycheck[] =
{
       9,     0,     4,    67,   168,   570,   110,    16,    57,    18,
      66,   158,   159,    65,    73,    16,    66,    59,   158,   159,
     405,   747,    76,    98,   405,    89,   747,   401,    95,   503,
      72,    61,   995,   302,   603,   373,   375,   575,    97,   468,
    1254,   545,   810,   397,    93,    99,    55,    56,    57,    58,
      59,    60,    61,   523,  1253,  1326,   717,    66,    67,    68,
     765,    70,    92,    72,    73,   524,    75,    76,    77,   956,
     100,  1019,   317,  1080,  1121,   687,    85,    71,    87,    88,
      89,   742,    91,    92,    93,    94,    87,    96,    97,    98,
      99,   100,    58,   102,   150,    96,   152,  1080,   173,  1037,
     758,    95,   731,   344,    70,   992,   170,   680,   737,   151,
    1143,   468,   909,  1080,   381,   619,   177,   382,   253,  1080,
       6,   595,  1272,   597,     8,    47,     8,     8,    94,   603,
     172,  1533,  1087,   163,     8,  1292,   102,     8,   816,  1080,
     846,   150,   151,   152,    10,  1080,   411,   156,     8,    21,
      26,     8,    47,   831,   163,   813,   834,    61,   262,   168,
      46,   170,   171,   172,   173,   843,    10,   219,   177,    19,
       8,   559,     8,  1318,  1319,    50,    51,   534,  1323,    23,
     344,  1231,  1024,  1138,    28,    23,   694,  1237,  1324,    97,
      28,    37,     8,     8,    23,    44,   248,    44,   141,    28,
      63,    90,    48,    44,   133,   171,   169,   581,    23,    55,
      63,  1475,    14,    28,    10,   350,    43,    43,   603,    63,
      18,   694,   603,   174,   902,  1592,   699,    23,    50,    51,
      61,    80,    28,    79,   149,    56,   292,    46,   117,   295,
    1845,   125,    11,   125,   125,    44,   126,   516,    23,   193,
     354,   125,    43,    28,   125,   629,   318,     4,   143,  1546,
       7,  1475,   531,    63,   326,   125,  1084,   605,   125,   995,
    1469,  1470,    18,  1091,   995,     8,    45,   162,   274,   328,
       8,    80,  1100,   292,    82,    94,   295,  1892,   344,   125,
     332,  1693,     9,   302,   303,   141,   348,   244,   178,    90,
     565,   177,    49,   105,  1591,   934,   935,   186,  1126,   125,
    1357,  1318,  1319,   276,   189,   653,  1323,   260,   374,   328,
     749,   174,   230,   332,   275,   381,   172,   383,   757,   385,
     176,   180,   388,   180,   155,  1318,  1319,   173,    55,   180,
    1323,    26,   699,   407,  1299,  1283,   398,   276,   400,    66,
     409,  1318,  1319,   275,   204,   984,  1323,  1318,  1319,   523,
     191,   276,  1323,   415,    23,   374,   255,   189,  1525,    28,
     419,   275,   381,  1321,   383,   417,   385,  1318,  1319,   388,
     275,   885,  1323,  1318,  1319,   859,   272,  1065,  1323,  1095,
    1096,   275,   125,  1443,  1444,   277,   277,   125,   407,   408,
     409,  1062,   790,   277,  1082,  1083,   277,   279,   417,   275,
     419,  1298,   275,    11,   423,    22,  1364,   277,   926,   265,
     277,   423,  1558,   190,  1102,    23,   270,   271,   272,   273,
      28,   275,   270,   271,   272,   273,  1278,  1279,   123,  1145,
     267,   267,   408,   272,   273,   173,  1717,    45,   165,   922,
    1079,   910,  1819,   926,    23,   270,   271,   272,   273,    28,
     469,    90,   921,  1662,   255,    64,   925,   523,   143,    62,
     141,   513,   533,   196,   270,   271,   272,   273,    77,   192,
     522,   621,     4,     6,   526,     3,    85,   825,    80,  1273,
     828,   117,   177,   185,   196,   270,   271,   272,   273,    55,
     223,   100,   277,   841,   513,   774,   515,   516,   275,   194,
      61,   118,   196,   522,   570,   114,    43,   526,   196,   194,
    1246,    44,   531,  1341,   533,  1246,   196,    49,   866,   147,
     156,   583,    19,    56,  1163,  1164,   671,  1166,  1167,  1151,
    1811,  1691,   787,    44,     7,  1250,   139,   603,   271,   515,
      55,   196,   151,    68,  1601,   622,  1589,    80,    76,  1029,
     196,   570,   275,  1347,  1137,   832,  1363,    94,   809,   271,
     199,   200,   624,    22,   913,   246,  1840,   203,  1037,    80,
    1145,    30,  1853,   747,   276,   141,   179,   231,   180,  1247,
     196,   275,   601,  1541,   603,  1645,  1534,   241,   607,     5,
    1864,   271,  1031,  1874,   282,    23,    64,    94,  1067,    15,
      28,   270,   271,   272,   273,   762,  1571,   669,   277,    77,
     176,    21,   145,   679,   676,   677,  1840,  1891,   622,  1774,
    1775,   276,   155,  1588,   156,   900,   141,   282,  1902,    85,
     276,  1905,   100,    98,   145,   809,   282,   196,  1912,    46,
    1864,     3,   174,   168,   155,   795,   114,   180,    23,     6,
     175,   196,   108,    28,   263,   271,    23,   172,   196,   118,
     679,    28,   270,   271,   272,   273,   282,  1891,  1307,   180,
      23,   276,   187,     6,  1362,    28,   208,    87,  1902,    23,
     139,  1905,   722,   151,    28,   223,   145,    94,  1912,    46,
    1329,   270,   271,   272,   273,  1904,     6,   763,   277,   765,
     142,   767,   167,   722,    64,     6,  1679,    23,   164,  1073,
    1607,   118,    28,    46,    76,    17,   158,    77,   868,    23,
     179,    30,  1550,  1394,    28,    65,  1554,  1555,     6,    31,
     782,   750,     6,   271,    43,   230,    46,    65,  1177,   758,
     100,  1089,  1090,   809,   763,    46,   765,    84,   767,   815,
      98,    23,   804,  1101,   806,   774,    28,  1774,  1775,   821,
      23,  1539,  1590,   782,   783,    28,   832,   262,    46,   140,
     789,  1729,    46,    75,   793,  1139,   275,  1735,   277,  1327,
     117,  1774,  1775,   860,   132,   804,   134,   806,   196,    59,
      60,   151,   858,   859,   813,   263,   815,  1774,  1775,  1148,
     155,   103,  1241,  1774,  1775,   118,   143,   783,   156,   146,
    1177,    23,   874,   832,   276,   223,    28,   894,   895,   196,
     897,   995,     4,  1774,  1775,   162,   139,   182,   278,  1774,
    1775,   281,   145,   181,    16,   275,  1350,   277,    79,   858,
     859,  1355,   270,   271,   272,   273,   223,   275,   275,   901,
    1429,   188,  1291,   280,   122,  1029,   860,   876,    23,   911,
    1833,   865,  1835,    28,   876,   167,   179,    49,  1217,    51,
    1843,    23,   272,   114,   275,  1703,    28,   122,   882,  1707,
     280,   275,   901,   275,    23,   126,   280,   124,   280,    28,
     894,   895,   911,   897,  1722,   270,   271,   272,   273,  1538,
     919,   920,   277,   270,   271,   272,   273,   919,   231,   232,
     277,  1739,   195,   196,   197,  1888,   124,   270,   271,   272,
     273,   275,   174,   277,   277,   232,   270,   271,   272,   273,
     718,    79,   720,   277,   199,   200,   724,   725,   726,   156,
     223,   224,   203,  1679,   205,   275,   180,   277,  1679,   275,
     990,   277,   130,   131,   270,   271,   272,   273,   275,    23,
     277,   277,   192,  1029,    28,    19,   270,   271,   272,   273,
    1454,   990,    23,   277,  1124,  1511,  1512,    28,  1257,   276,
     275,  1260,   277,  1367,   275,   275,   277,   277,  1511,  1512,
    1818,  1339,   259,  1821,   275,   275,   277,   277,   270,   271,
     272,   273,   266,   275,  1023,  1024,   162,   270,   271,   272,
     273,  1860,  1861,     0,   277,   275,  1655,   277,  1037,   199,
     275,  1369,   277,    10,  1391,  1392,   275,    10,   277,  1857,
      59,    61,  1399,  1400,  1429,  1402,  1403,   275,  1429,   277,
     275,    79,   277,  1410,  1411,  1412,  1413,   198,  1415,  1416,
    1417,  1418,  1419,  1420,   275,    78,   277,  1134,   270,   271,
     272,   273,   275,   190,   277,   277,   275,   360,   277,   362,
    1147,  1286,  1246,  1288,    61,  1141,  1153,    64,   149,  1145,
    1254,   172,   141,    82,    71,   276,    73,    23,    63,    61,
      77,    61,    28,   174,  1113,    82,   122,  1833,    85,  1835,
     122,   244,  1833,    23,  1835,   102,  1118,  1843,    28,   188,
     172,  1130,  1843,   100,  1271,  1272,  1273,   123,   270,   271,
     272,   273,  1141,    23,   243,   277,  1145,   114,    28,   121,
    1134,   270,   271,   272,   273,   122,   155,   254,   277,   126,
     255,   254,    23,  1147,    30,   275,   207,    28,   135,  1153,
     192,   267,  1888,  1303,  1793,  1305,   112,  1888,    29,    10,
     199,  1800,   276,    23,   151,   276,   135,    17,    28,   276,
     276,   276,   276,    23,   276,   276,  1261,    23,    28,   276,
     276,    31,    28,   276,  1250,   199,   199,  1253,  1254,   276,
    1259,   178,   276,  1253,  1213,    97,    23,   148,  1262,   276,
     276,    28,  1213,   190,   279,   192,   270,   271,   272,   273,
     275,   281,  1560,   277,   281,  1365,   276,    67,    23,   270,
     271,   272,   273,    28,    23,    75,   277,   267,  1247,    28,
     276,  1250,    23,    83,  1253,  1254,    63,    28,  1257,  1258,
    1259,  1260,  1261,  1262,    17,    10,   141,  1595,    63,    54,
      23,   101,   239,   103,   275,    28,   166,   275,    31,  1278,
    1279,   281,    61,    54,  1283,   267,   116,   276,   118,   119,
     120,    10,   259,  1848,   275,    22,   263,    90,    90,    23,
      90,    22,  1258,    43,    28,    10,   174,   274,   275,   208,
     277,   136,   174,   174,    67,   136,    90,   196,    17,    21,
     185,  1475,    75,   138,    23,   185,    91,    61,    23,    28,
      83,   276,    31,    28,   276,   279,  1378,   167,   941,   276,
     943,  1383,   945,   946,   276,   948,   949,   950,   101,    10,
     103,   149,   122,  1345,   270,   271,   272,   273,    82,   128,
     275,   277,   121,   116,   276,   118,   119,   120,    67,   276,
     270,   271,   272,   273,   126,   189,    75,   277,    61,   277,
      21,   282,  1536,    42,    83,   174,   174,    82,    60,   202,
     270,   271,   272,   273,   190,   276,    10,   277,   275,   186,
     276,   275,   101,   240,   103,   276,  1460,    19,  1736,   270,
     271,   272,   273,    43,   167,    86,    27,   116,    86,   118,
     119,   120,    22,   154,   125,   125,  1425,   125,   156,  1475,
     270,   271,   272,   273,  1425,   275,   156,   267,   268,   269,
     270,   271,   272,   273,   270,   271,   272,   273,   276,   279,
      98,   281,   276,   276,   108,   277,   276,   275,     6,    63,
      23,  1460,   184,   270,   271,   272,   273,   275,   167,  1468,
    1469,  1470,   291,    16,   124,   281,  1475,   210,   211,   212,
     213,   214,   215,   216,   217,   270,   271,   272,   273,   107,
      63,   270,   271,   272,   273,   314,    44,    63,   275,   270,
     271,   272,   273,   276,   323,    12,    71,   239,    56,   276,
     276,    10,  1511,  1512,   267,   268,   269,   270,   271,   272,
     273,   160,    70,   277,    10,  1679,   279,   278,   281,  1561,
     277,   277,    80,   277,  1533,  1534,   192,  1536,  1692,   133,
     359,    52,    52,     8,   363,    63,   270,   271,   272,   273,
     276,    64,   277,   276,  1691,    51,   173,   192,   275,   277,
     276,   276,  1561,   276,    77,   276,   276,   276,   267,   268,
     269,   270,   271,   272,   273,   270,   271,   272,   273,   276,
     279,   275,   281,   276,   276,   276,   276,   100,   276,   276,
     276,   127,   277,   277,  1633,    85,    85,   145,    19,    73,
     130,   114,   125,   145,    48,   125,   125,   155,   125,   122,
     125,    55,   277,   125,   196,    59,    60,   277,    22,   276,
      63,    51,    17,   173,    68,   277,  1720,    36,    23,   275,
     277,  1630,   180,    28,  1633,    79,    31,   277,   151,  1630,
     277,   460,   274,   277,    36,   274,    82,   466,   467,   468,
     276,   274,   274,    51,   202,   274,    82,   278,   274,   274,
      70,   274,    10,  1662,   274,   274,   274,   274,   274,   274,
     174,   274,    67,   274,    85,   274,   274,    23,    79,  1833,
      75,  1835,  1721,   180,  1716,  1674,  1840,    80,    83,  1843,
      44,    80,   180,  1692,  1693,    44,   277,   141,   277,   124,
    1742,   277,   277,    94,  1746,   111,   101,   277,   103,   111,
    1864,    61,  1711,   157,   276,   534,   277,  1716,  1812,   538,
     277,   116,  1721,   118,   119,   120,   239,   189,   172,   275,
     277,  1785,   176,    17,  1888,    94,   277,  1891,   277,    23,
     277,   124,   277,   187,    28,   277,   277,    31,  1902,   277,
     263,  1905,   277,    18,    86,   277,    21,   204,  1912,    79,
      82,   277,   206,   277,    38,   246,    82,   276,   276,  1768,
     122,   277,   167,    38,   276,   276,    41,   277,   276,   173,
     275,   122,   122,    67,   277,   277,  1785,  1786,    47,    44,
     180,    75,    66,   277,  1840,  1787,    50,    36,    47,    83,
       0,   201,  1848,     0,   155,  1269,   300,   783,   155,   774,
    1151,   156,   303,   683,   914,   529,    81,   101,  1864,   103,
      94,   265,    87,    88,    89,   531,    91,    92,    93,  1148,
      95,   919,   116,  1140,   118,   119,   120,    27,  1579,    77,
     889,  1840,   371,  1832,   402,  1891,  1845,   629,   113,  1848,
     115,   815,  1076,  1141,   842,  1560,  1902,   383,  1578,  1905,
    1145,  1860,  1861,  1558,  1851,  1864,  1912,   352,  1876,  1854,
      37,   832,   267,   268,   269,   270,   271,   272,   273,   144,
     699,    48,   277,   167,  1855,  1020,  1666,  1841,    55,     0,
     763,   165,  1891,  1892,  1542,  1301,   161,   376,   369,  1353,
     719,    68,   609,  1902,   723,  1904,  1905,   181,   383,   892,
     388,   730,    13,  1912,   884,   551,   735,   736,   183,   336,
     547,   195,   196,   197,   666,   199,   200,   161,   791,  1037,
     749,    87,   751,   752,   753,   754,   755,   756,   757,  1287,
     804,   716,  1805,  1630,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   218,   219,  1425,     0,     0,   125,   233,
     234,   818,     0,   559,   781,  1192,  1032,  1239,   242,  1130,
     235,  1309,   791,   876,   141,    91,  1711,    -1,  1468,   607,
      -1,    -1,    -1,   267,   268,   269,   270,   271,   272,   273,
     157,    -1,    -1,   812,    -1,    -1,    -1,   271,    -1,   818,
      -1,   820,    -1,     6,    -1,   172,     9,    -1,   198,   176,
      13,   201,   179,   203,   204,   205,   206,    20,   208,    -1,
     187,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,   227,   228,   206,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    66,   254,   255,    69,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   265,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   936,   937,   938,
      -1,    -1,   941,   942,   943,    -1,   945,   946,   947,   948,
     949,   950,    -1,    -1,    -1,    -1,    -1,    -1,   957,   958,
      -1,   960,    -1,   962,    -1,    -1,   965,   966,   967,   968,
     969,   970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,   982,    -1,    -1,   985,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,  1031,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,    -1,    -1,   270,   271,    -1,
      -1,    -1,    -1,   276,    -1,   278,    -1,    -1,    -1,   282,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,  1391,  1392,    -1,    -1,    -1,    -1,    -1,    -1,
    1399,  1400,    32,  1402,  1403,    -1,    -1,    -1,    38,    39,
      -1,  1410,  1411,  1412,  1413,    -1,  1415,  1416,  1417,  1418,
    1419,  1420,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,  1177,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1190,  1191,    -1,  1193,  1194,    -1,    -1,    -1,    -1,
      -1,    -1,  1201,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,  1221,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1229,  1230,  1231,    -1,    -1,    -1,    -1,    -1,  1237,    -1,
    1239,  1240,  1241,  1242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,  1291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,  1322,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,     7,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    25,
     270,   271,    -1,    -1,    -1,    -1,   276,    33,   278,    -1,
      -1,  1390,   282,    -1,    40,    -1,    42,  1396,    44,    -1,
      -1,    -1,  1401,    49,    -1,    -1,    -1,    -1,    -1,  1408,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1443,  1444,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1465,     6,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,   143,    -1,    38,
      39,    -1,    -1,    -1,   150,    -1,    -1,   153,    -1,   155,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,   180,    -1,    -1,    -1,  1537,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   244,   245,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1610,    -1,    -1,    -1,    -1,    -1,  1616,  1617,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1640,   181,    -1,    30,    -1,  1645,    -1,    -1,  1648,
      -1,    -1,    -1,  1652,  1653,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,   270,   271,     6,    -1,    -1,     9,   276,    -1,   278,
      13,    -1,  1741,   282,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1805,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    94,   208,    96,    97,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,   227,   228,  1842,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     205,   206,    -1,   208,   209,    -1,    71,    -1,    -1,   218,
     219,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,   235,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,   270,   271,     6,    -1,    -1,
       9,   276,    -1,   278,    13,    -1,   141,   282,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,   270,
     271,    -1,    -1,    -1,    -1,   276,    -1,   278,    -1,    -1,
      -1,   282,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
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
     282,    -1,    -1,    66,    -1,    -1,    69,   196,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,    -1,    -1,   270,   271,    -1,    -1,    -1,    -1,
     276,    -1,   278,    -1,    -1,    -1,   282,    -1,    -1,    66,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    16,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,   270,   271,    44,    -1,    -1,    -1,   276,
      -1,   278,    51,    -1,    -1,   282,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,   137,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      80,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   227,   228,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,   155,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
     170,   260,   261,   262,    -1,   264,   265,   266,    -1,    -1,
     180,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    44,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      55,    56,    -1,    58,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    74,
     240,   241,    -1,   243,    -1,    80,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,   256,   257,   258,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,   137,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,   180,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,   113,    -1,   115,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,   144,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   256,   257,   258,   161,   260,   261,   262,    -1,   264,
     265,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    18,    -1,    -1,    21,    -1,    -1,    -1,   216,
     217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    38,   230,    -1,    41,    -1,    -1,   235,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
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
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,   198,    -1,    -1,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,    -1,   225,   226,
     227,   228,   229,   230,    -1,    -1,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266
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
{ yyval = yyvsp[0]; }
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
{ yyval = yyvsp[0]; }
    break;
case 123:
{ yyval = yyvsp[0]; }
    break;
case 124:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 125:
{ yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 126:
{ yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 127:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;
case 128:
{ yyval = make_node (nod_unique, 0, NULL); }
    break;
case 129:
{ yyval = NULL; }
    break;
case 130:
{ yyval = make_list (yyvsp[0]); }
    break;
case 132:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 133:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 134:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 135:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 136:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 137:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 138:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 139:
{ yyval = (dsql_nod*) 0;}
    break;
case 140:
{ yyval = yyvsp[-1]; }
    break;
case 141:
{ yyval = NULL; }
    break;
case 144:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 145:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 146:
{ yyval = NULL; }
    break;
case 147:
{ yyval = NULL; }
    break;
case 148:
{ yyval = yyvsp[0]; }
    break;
case 149:
{ yyval = NULL; }
    break;
case 150:
{ yyval = NULL; }
    break;
case 153:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 154:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 157:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 158:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 159:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
    break;
case 160:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
    break;
case 161:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 164:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
    break;
case 165:
{yyval = NULL;}
    break;
case 168:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 169:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 170:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 171:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 172:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 173:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 174:
{yyval = NULL;}
    break;
case 177:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 179:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 180:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 181:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 182:
{ lex.g_file  = make_file();}
    break;
case 187:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 188:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;
case 195:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 196:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 197:
{ yyval = yyvsp[0]; }
    break;
case 198:
{ yyval = yyvsp[0]; }
    break;
case 199:
{ yyval = NULL; }
    break;
case 201:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 204:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 205:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 206:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 207:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 210:
{ yyval = NULL; }
    break;
case 211:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 212:
{ yyval = yyvsp[0]; }
    break;
case 213:
{ yyval = NULL; }
    break;
case 214:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 215:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 216:
{ yyval = yyvsp[-1]; }
    break;
case 217:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 218:
{ yyval = yyvsp[0]; }
    break;
case 219:
{ yyval = NULL; }
    break;
case 226:
{ yyval = NULL; }
    break;
case 229:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 231:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 232:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 234:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 235:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 236:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 237:
{ yyval = yyvsp[0]; }
    break;
case 238:
{ yyval = NULL ;}
    break;
case 243:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 244:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 245:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 246:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 247:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 248:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
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
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
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
{ yyval = (dsql_nod*) NULL; }
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
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 292:
{ yyval = yyvsp[-1]; }
    break;
case 293:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 294:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 295:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 297:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 312:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 313:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 318:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
    break;
case 319:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
    break;
case 320:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 321:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
    break;
case 322:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 323:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
    break;
case 325:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 327:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
    break;
case 328:
{ yyval = NULL; }
    break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
    break;
case 330:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
    break;
case 341:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
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
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 360:
{ yyval = make_list (yyvsp[0]); }
    break;
case 361:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 362:
{ yyval = NULL; }
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
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 367:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 368:
{ yyval = NULL; }
    break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 371:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 372:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 373:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 374:
{ lex.beginning = lex_position(); }
    break;
case 375:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 376:
{ lex.beginning = lex.last_token; }
    break;
case 377:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;
case 378:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 379:
{ yyval = 0; }
    break;
case 380:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 381:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 383:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 384:
{ yyval = NULL; }
    break;
case 385:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;
case 387:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;
case 403:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 404:
{ yyval = NULL; }
    break;
case 405:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;
case 406:
{ yyval = yyvsp[0]; }
    break;
case 407:
{ yyval = yyvsp[0]; }
    break;
case 408:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 409:
{ yyval = yyvsp[0]; }
    break;
case 410:
{ yyval = yyvsp[0]; }
    break;
case 411:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;
case 412:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;
case 413:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;
case 414:
{ yyval = yyvsp[0]; }
    break;
case 415:
{ yyval = yyvsp[0]; }
    break;
case 416:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 418:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 419:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 420:
{ yyval = yyvsp[0]; }
    break;
case 421:
{ yyval = yyvsp[0]; }
    break;
case 422:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 424:
{ yyval = yyvsp[0]; }
    break;
case 425:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 427:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 428:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 429:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 430:
{ yyval = yyvsp[0]; }
    break;
case 431:
{ yyval = yyvsp[0]; }
    break;
case 432:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 433:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 434:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;
case 435:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 467:
{ yyval = NULL; }
    break;
case 468:
{ yyval = NULL; }
    break;
case 469:
{ yyval = NULL; }
    break;
case 470:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 471:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 472:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 473:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 474:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 475:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 476:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 477:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 478:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 479:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 480:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;
case 482:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 483:
{ yyval = yyvsp[0]; }
    break;
case 484:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 485:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 486:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 487:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 488:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 490:
{ yyval = NULL; }
    break;
case 492:
{ yyval = NULL; }
    break;
case 493:
{ yyval = yyvsp[0]; }
    break;
case 494:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 495:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 496:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 497:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 498:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 499:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 500:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 501:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 502:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 503:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 505:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 506:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 511:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 512:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 514:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 515:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 516:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 522:
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
case 523:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 525:
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
case 526:
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
case 527:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 534:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 535:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 536:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 537:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 538:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 539:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 555:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 556:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 557:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 558:
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
case 559:
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
case 562:
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
case 563:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 566:
{ yyval = yyvsp[-1]; }
    break;
case 567:
{ yyval = 0; }
    break;
case 571:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 572:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 573:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 577:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 578:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 579:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 580:
{ yyval = 0; }
    break;
case 581:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 584:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 585:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 588:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
    break;
case 589:
{ yyval = NULL; }
    break;
case 591:
{ yyval = NULL; }
    break;
case 592:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 594:
{ yyval = NULL; }
    break;
case 596:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 601:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 602:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 603:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 604:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 605:
{ yyval = yyvsp[0];}
    break;
case 607:
{ yyval = yyvsp[0];}
    break;
case 608:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 609:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 610:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 613:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 614:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 615:
{ yyval = 0; }
    break;
case 616:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 617:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 618:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 619:
{ yyval = (dsql_nod*) 0; }
    break;
case 620:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 621:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 623:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 624:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 625:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 626:
{ yyval = 0; }
    break;
case 628:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 629:
{yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 630:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 631:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 632:
{ yyval = NULL; }
    break;
case 633:
{ yyval = yyvsp[0]; }
    break;
case 634:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 635:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 636:
{ yyval = NULL; }
    break;
case 637:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 638:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 639:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 641:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 642:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 644:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 645:
{ lex.limit_clause = true; }
    break;
case 646:
{ lex.limit_clause = false; }
    break;
case 647:
{ lex.first_detection = true; }
    break;
case 648:
{ lex.first_detection = false; }
    break;
case 649:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 650:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 651:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 652:
{ yyval = 0; }
    break;
case 653:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 654:
{ yyval = yyvsp[-2]; }
    break;
case 655:
{ yyval = yyvsp[-1]; }
    break;
case 656:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 657:
{ yyval = yyvsp[-1]; }
    break;
case 658:
{ yyval = yyvsp[0]; }
    break;
case 659:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 660:
{ yyval = 0; }
    break;
case 661:
{ yyval = make_list (yyvsp[0]); }
    break;
case 662:
{ yyval = 0; }
    break;
case 664:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 666:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 669:
{ yyval = make_list (yyvsp[0]); }
    break;
case 671:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 676:
{ yyval = yyvsp[-1]; }
    break;
case 677:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 679:
{ yyval = NULL; }
    break;
case 680:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 681:
{ yyval = NULL; }
    break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 687:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 688:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 689:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 692:
{ yyval = yyvsp[0]; }
    break;
case 693:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 694:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 695:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 696:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 697:
{ yyval = NULL; }
    break;
case 699:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 700:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 701:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 702:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 703:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 704:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 705:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 708:
{ yyval = make_list (yyvsp[0]); }
    break;
case 709:
{ yyval = NULL; }
    break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 713:
{ yyval = yyvsp[0]; }
    break;
case 714:
{ yyval = NULL; }
    break;
case 715:
{ yyval = yyvsp[0]; }
    break;
case 716:
{ yyval = NULL; }
    break;
case 717:
{ yyval = yyvsp[0]; }
    break;
case 718:
{ yyval = NULL; }
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
{ yyval = make_list (yyvsp[0]); }
    break;
case 739:
{ yyval = 0; }
    break;
case 741:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 743:
{ yyval = 0; }
    break;
case 744:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 745:
{ yyval = 0; }
    break;
case 746:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 747:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 748:
{ yyval = yyvsp[-1]; }
    break;
case 749:
{ yyval = 0; }
    break;
case 750:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 751:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 752:
{ yyval = NULL; }
    break;
case 753:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 754:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 757:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 758:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 761:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 762:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 763:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 765:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 766:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 767:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 768:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 771:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 772:
{ yyval = NULL; }
    break;
case 775:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 776:
{ yyval = yyvsp[0]; }
    break;
case 777:
{ yyval = NULL; }
    break;
case 778:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
    break;
case 787:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 789:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 791:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 792:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 793:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 795:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 799:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 800:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 802:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 804:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 808:
{ yyval = yyvsp[-1]; }
    break;
case 809:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 821:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 822:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 823:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 824:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 825:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 826:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 827:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 828:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 829:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 830:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 831:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 832:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 833:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 835:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 836:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 837:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 838:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 839:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 840:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 841:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 842:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 843:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 844:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 848:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 851:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 853:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 854:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 855:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 856:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 857:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 858:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 859:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 860:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 861:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 862:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 863:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 864:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 865:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 866:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 867:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 868:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 869:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 870:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 871:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 872:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 874:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 875:
{ yyval = yyvsp[-1]; }
    break;
case 886:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 887:
{ yyval = yyvsp[0]; }
    break;
case 888:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 889:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 890:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 891:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 892:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 893:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 894:
{ yyval = yyvsp[-1]; }
    break;
case 895:
{ yyval = yyvsp[-1]; }
    break;
case 899:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 900:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 901:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 904:
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
case 905:
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
case 906:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 907:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 909:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 911:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 912:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 913:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 914:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 918:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 919:
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
case 920:
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
case 921:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 922:
{ yyval = make_parameter (); }
    break;
case 923:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 924:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 925:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 926:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 927:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 928:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 929:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 930:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 931:
{ yyval = yyvsp[0]; }
    break;
case 932:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 934:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 935:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 936:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 937:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 938:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 940:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 941:
{ yyval = yyvsp[0];}
    break;
case 945:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 946:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 947:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 948:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 949:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 950:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 951:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 952:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 953:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 954:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 955:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 957:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 959:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 960:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 961:
{ yyval = yyvsp[0]; }
    break;
case 962:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 963:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 964:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 965:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 968:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 969:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 970:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 973:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 974:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 975:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 976:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 977:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 978:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 979:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 980:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 984:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 985:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 989:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 990:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 991:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 992:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 993:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 998:
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

