
/*  A Bison parser, made from c:\fb2_old\src\dsql\parse.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ACTIVE	257
#define	ADD	258
#define	AFTER	259
#define	ALL	260
#define	ALTER	261
#define	AND	262
#define	ANY	263
#define	AS	264
#define	ASC	265
#define	AT	266
#define	AVG	267
#define	AUTO	268
#define	BEFORE	269
#define	BEGIN	270
#define	BETWEEN	271
#define	BLOB	272
#define	BY	273
#define	CAST	274
#define	CHARACTER	275
#define	CHECK	276
#define	COLLATE	277
#define	COMMA	278
#define	COMMIT	279
#define	COMMITTED	280
#define	COMPUTED	281
#define	CONCATENATE	282
#define	CONDITIONAL	283
#define	CONSTRAINT	284
#define	CONTAINING	285
#define	COUNT	286
#define	CREATE	287
#define	CSTRING	288
#define	CURRENT	289
#define	CURSOR	290
#define	DATABASE	291
#define	DATE	292
#define	DB_KEY	293
#define	KW_DEBUG	294
#define	DECIMAL	295
#define	DECLARE	296
#define	DEFAULT	297
#define	KW_DELETE	298
#define	DESC	299
#define	DISTINCT	300
#define	DO	301
#define	DOMAIN	302
#define	DROP	303
#define	ELSE	304
#define	END	305
#define	ENTRY_POINT	306
#define	EQL	307
#define	ESCAPE	308
#define	EXCEPTION	309
#define	EXECUTE	310
#define	EXISTS	311
#define	EXIT	312
#define	EXTERNAL	313
#define	FILTER	314
#define	FOR	315
#define	FOREIGN	316
#define	FROM	317
#define	FULL	318
#define	FUNCTION	319
#define	GDSCODE	320
#define	GEQ	321
#define	GENERATOR	322
#define	GEN_ID	323
#define	GRANT	324
#define	GROUP	325
#define	GTR	326
#define	HAVING	327
#define	IF	328
#define	KW_IN	329
#define	INACTIVE	330
#define	INNER	331
#define	INPUT_TYPE	332
#define	INDEX	333
#define	INSERT	334
#define	INTEGER	335
#define	INTO	336
#define	IS	337
#define	ISOLATION	338
#define	JOIN	339
#define	KEY	340
#define	KW_CHAR	341
#define	KW_DEC	342
#define	KW_DOUBLE	343
#define	KW_FILE	344
#define	KW_FLOAT	345
#define	KW_INT	346
#define	KW_LONG	347
#define	KW_NULL	348
#define	KW_NUMERIC	349
#define	KW_UPPER	350
#define	KW_VALUE	351
#define	LENGTH	352
#define	LPAREN	353
#define	LEFT	354
#define	LEQ	355
#define	LEVEL	356
#define	LIKE	357
#define	LSS	358
#define	MANUAL	359
#define	MAXIMUM	360
#define	MAX_SEGMENT	361
#define	MERGE	362
#define	MESSAGE	363
#define	MINIMUM	364
#define	MODULE_NAME	365
#define	NAMES	366
#define	NATIONAL	367
#define	NATURAL	368
#define	NCHAR	369
#define	NEQ	370
#define	NO	371
#define	NOT	372
#define	NOT_GTR	373
#define	NOT_LSS	374
#define	OF	375
#define	ON	376
#define	ONLY	377
#define	OPTION	378
#define	OR	379
#define	ORDER	380
#define	OUTER	381
#define	OUTPUT_TYPE	382
#define	OVERFLOW	383
#define	PAGE	384
#define	PAGES	385
#define	KW_PAGE_SIZE	386
#define	PARAMETER	387
#define	PASSWORD	388
#define	PLAN	389
#define	POSITION	390
#define	POST_EVENT	391
#define	PRECISION	392
#define	PRIMARY	393
#define	PRIVILEGES	394
#define	PROCEDURE	395
#define	PROTECTED	396
#define	READ	397
#define	REAL	398
#define	REFERENCES	399
#define	RESERVING	400
#define	RETAIN	401
#define	RETURNING_VALUES	402
#define	RETURNS	403
#define	REVOKE	404
#define	RIGHT	405
#define	RPAREN	406
#define	ROLLBACK	407
#define	SEGMENT	408
#define	SELECT	409
#define	SET	410
#define	SHADOW	411
#define	KW_SHARED	412
#define	SINGULAR	413
#define	KW_SIZE	414
#define	SMALLINT	415
#define	SNAPSHOT	416
#define	SOME	417
#define	SORT	418
#define	SQLCODE	419
#define	STABILITY	420
#define	STARTING	421
#define	STATISTICS	422
#define	SUB_TYPE	423
#define	SUSPEND	424
#define	SUM	425
#define	TABLE	426
#define	THEN	427
#define	TO	428
#define	TRANSACTION	429
#define	TRIGGER	430
#define	UNCOMMITTED	431
#define	UNION	432
#define	UNIQUE	433
#define	UPDATE	434
#define	USER	435
#define	VALUES	436
#define	VARCHAR	437
#define	VARIABLE	438
#define	VARYING	439
#define	VERSION	440
#define	VIEW	441
#define	WAIT	442
#define	WHEN	443
#define	WHERE	444
#define	WHILE	445
#define	WITH	446
#define	WORK	447
#define	WRITE	448
#define	FLOAT_NUMBER	449
#define	NUMBER	450
#define	NUMERIC	451
#define	SYMBOL	452
#define	STRING	453
#define	INTRODUCER	454
#define	ACTION	455
#define	ADMIN	456
#define	CASCADE	457
#define	FREE_IT	458
#define	RESTRICT	459
#define	ROLE	460
#define	COLUMN	461
#define	TYPE	462
#define	EXTRACT	463
#define	YEAR	464
#define	MONTH	465
#define	DAY	466
#define	HOUR	467
#define	MINUTE	468
#define	SECOND	469
#define	WEEKDAY	470
#define	YEARDAY	471
#define	TIME	472
#define	TIMESTAMP	473
#define	CURRENT_DATE	474
#define	CURRENT_TIME	475
#define	CURRENT_TIMESTAMP	476
#define	NUMBER64BIT	477
#define	SCALEDINT	478
#define	CURRENT_USER	479
#define	CURRENT_ROLE	480
#define	KW_BREAK	481
#define	SUBSTRING	482
#define	RECREATE	483
#define	KW_DESCRIPTOR	484
#define	FIRST	485
#define	SKIP	486
#define	CURRENT_CONNECTION	487
#define	CURRENT_TRANSACTION	488
#define	BIGINT	489
#define	CASE	490
#define	NULLIF	491
#define	COALESCE	492
#define	USING	493
#define	NULLS	494
#define	LAST	495
#define	ROW_COUNT	496
#define	LOCK	497
#define	SAVEPOINT	498
#define	RELEASE	499
#define	STATEMENT	500
#define	LEAVE	501
#define	INSERTING	502
#define	UPDATING	503
#define	DELETING	504
#define	KW_INSERTING	505
#define	KW_UPDATING	506
#define	KW_DELETING	507
#define	BACKUP	508
#define	KW_DIFFERENCE	509
#define	OPEN	510
#define	CLOSE	511
#define	FETCH	512
#define	ROWS	513
#define	BLOCK	514
#define	IIF	515
#define	SCALAR_ARRAY	516
#define	CROSS	517


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
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		1894
#define	YYFLAG		-32768
#define	YYNTBASE	280

#define YYTRANSLATE(x) ((unsigned)(x) <= 517 ? yytranslate[x] : 713)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,   273,
   274,   269,   267,   272,   268,   278,   270,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,   275,   271,   265,
   264,   266,   279,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
   276,     2,   277,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
   127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
   177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
   187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
   197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
   207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
   217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
   227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
   237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
   247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
   257,   258,   259,   260,   261,   262,   263
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
    41,    43,    46,    54,    63,    70,    78,    84,    86,    89,
    91,    94,    96,    98,   102,   104,   106,   108,   110,   113,
   116,   120,   121,   125,   126,   128,   136,   145,   152,   160,
   167,   175,   180,   184,   186,   190,   194,   198,   201,   204,
   207,   210,   212,   216,   218,   221,   224,   226,   230,   232,
   234,   238,   240,   243,   246,   249,   253,   262,   264,   266,
   272,   273,   275,   279,   281,   285,   289,   290,   293,   296,
   298,   300,   304,   308,   311,   312,   315,   318,   320,   324,
   334,   336,   338,   341,   345,   353,   356,   359,   362,   365,
   368,   371,   374,   377,   380,   383,   386,   389,   392,   397,
   400,   403,   405,   406,   408,   410,   417,   424,   426,   428,
   429,   430,   432,   433,   438,   439,   441,   443,   446,   455,
   457,   458,   462,   463,   464,   466,   468,   471,   473,   475,
   477,   480,   487,   489,   491,   495,   496,   498,   500,   501,
   503,   505,   508,   512,   517,   520,   523,   527,   528,   530,
   532,   535,   537,   542,   546,   550,   552,   553,   555,   557,
   560,   564,   569,   570,   572,   575,   576,   578,   580,   586,
   592,   596,   599,   600,   602,   606,   608,   610,   617,   621,
   624,   631,   634,   636,   639,   642,   645,   646,   648,   650,
   653,   654,   657,   658,   660,   662,   664,   666,   668,   670,
   671,   673,   675,   678,   681,   684,   690,   692,   695,   699,
   702,   705,   706,   708,   710,   712,   714,   718,   723,   732,
   737,   738,   745,   747,   749,   752,   755,   756,   760,   764,
   766,   769,   772,   775,   784,   793,   802,   811,   815,   816,
   821,   822,   824,   828,   833,   835,   839,   842,   846,   850,
   852,   854,   855,   857,   860,   865,   867,   869,   873,   875,
   876,   879,   881,   888,   890,   892,   896,   898,   901,   902,
   904,   907,   910,   913,   915,   917,   919,   921,   923,   925,
   928,   930,   932,   935,   937,   939,   941,   944,   947,   949,
   952,   955,   959,   964,   967,   974,   979,   988,   998,  1005,
  1014,  1021,  1026,  1027,  1032,  1035,  1037,  1041,  1042,  1045,
  1050,  1051,  1053,  1055,  1059,  1063,  1071,  1074,  1075,  1078,
  1081,  1085,  1089,  1090,  1092,  1095,  1100,  1102,  1106,  1109,
  1112,  1115,  1117,  1119,  1121,  1123,  1126,  1129,  1135,  1136,
  1141,  1149,  1153,  1154,  1156,  1160,  1164,  1172,  1180,  1181,
  1182,  1183,  1184,  1188,  1189,  1199,  1209,  1211,  1213,  1214,
  1217,  1219,  1221,  1223,  1225,  1227,  1231,  1235,  1239,  1243,
  1247,  1251,  1257,  1263,  1269,  1275,  1281,  1287,  1290,  1291,
  1296,  1299,  1303,  1307,  1310,  1313,  1317,  1321,  1324,  1328,
  1335,  1337,  1340,  1345,  1349,  1352,  1355,  1358,  1361,  1365,
  1367,  1371,  1375,  1379,  1382,  1385,  1390,  1395,  1401,  1403,
  1405,  1407,  1409,  1411,  1413,  1415,  1417,  1419,  1421,  1423,
  1425,  1427,  1429,  1431,  1433,  1435,  1437,  1439,  1441,  1443,
  1445,  1447,  1449,  1451,  1453,  1455,  1457,  1459,  1462,  1465,
  1468,  1470,  1472,  1474,  1475,  1478,  1481,  1482,  1484,  1487,
  1490,  1495,  1499,  1502,  1505,  1513,  1515,  1516,  1518,  1519,
  1522,  1525,  1528,  1531,  1534,  1537,  1540,  1543,  1546,  1550,
  1553,  1556,  1559,  1561,  1563,  1565,  1567,  1572,  1578,  1580,
  1584,  1586,  1590,  1592,  1595,  1597,  1599,  1601,  1603,  1605,
  1607,  1609,  1611,  1613,  1615,  1617,  1622,  1627,  1634,  1640,
  1644,  1645,  1648,  1651,  1652,  1656,  1657,  1662,  1664,  1670,
  1675,  1677,  1682,  1684,  1687,  1690,  1692,  1694,  1696,  1699,
  1702,  1705,  1708,  1709,  1713,  1719,  1721,  1723,  1726,  1730,
  1732,  1735,  1739,  1740,  1742,  1744,  1746,  1752,  1758,  1765,
  1767,  1769,  1771,  1774,  1779,  1781,  1782,  1788,  1790,  1791,
  1795,  1798,  1800,  1801,  1804,  1805,  1807,  1808,  1812,  1814,
  1815,  1817,  1820,  1822,  1824,  1826,  1828,  1831,  1834,  1836,
  1839,  1843,  1845,  1847,  1851,  1855,  1857,  1860,  1864,  1866,
  1869,  1870,  1873,  1875,  1877,  1878,  1880,  1882,  1884,  1888,
  1891,  1895,  1896,  1898,  1902,  1907,  1911,  1915,  1916,  1919,
  1920,  1923,  1924,  1928,  1932,  1936,  1938,  1943,  1948,  1950,
  1960,  1961,  1962,  1963,  1964,  1968,  1971,  1973,  1974,  1978,
  1984,  1988,  1991,  1997,  2000,  2002,  2004,  2006,  2008,  2010,
  2014,  2016,  2020,  2022,  2023,  2026,  2028,  2032,  2034,  2036,
  2038,  2040,  2044,  2051,  2053,  2054,  2058,  2059,  2061,  2065,
  2067,  2069,  2071,  2076,  2082,  2088,  2090,  2092,  2095,  2100,
  2105,  2108,  2112,  2113,  2115,  2118,  2120,  2122,  2125,  2128,
  2131,  2132,  2134,  2135,  2139,  2140,  2142,  2146,  2148,  2151,
  2152,  2155,  2156,  2159,  2160,  2165,  2167,  2170,  2172,  2174,
  2175,  2177,  2181,  2184,  2186,  2188,  2191,  2193,  2198,  2202,
  2204,  2208,  2213,  2214,  2218,  2219,  2221,  2225,  2229,  2231,
  2233,  2234,  2236,  2238,  2243,  2244,  2247,  2252,  2253,  2262,
  2268,  2270,  2272,  2280,  2285,  2290,  2292,  2294,  2303,  2309,
  2311,  2315,  2319,  2321,  2329,  2337,  2343,  2347,  2348,  2350,
  2352,  2354,  2357,  2358,  2360,  2362,  2364,  2365,  2369,  2371,
  2375,  2377,  2378,  2382,  2384,  2388,  2390,  2394,  2398,  2400,
  2402,  2406,  2408,  2411,  2413,  2417,  2421,  2423,  2426,  2430,
  2434,  2438,  2442,  2444,  2448,  2451,  2453,  2455,  2457,  2459,
  2461,  2463,  2465,  2467,  2469,  2471,  2473,  2477,  2481,  2485,
  2489,  2493,  2497,  2501,  2505,  2512,  2519,  2526,  2533,  2540,
  2547,  2554,  2561,  2568,  2575,  2582,  2589,  2596,  2603,  2610,
  2617,  2619,  2621,  2627,  2634,  2640,  2647,  2651,  2656,  2662,
  2669,  2673,  2678,  2682,  2687,  2691,  2696,  2701,  2707,  2712,
  2717,  2721,  2726,  2728,  2730,  2732,  2734,  2736,  2738,  2740,
  2744,  2748,  2750,  2752,  2754,  2756,  2758,  2760,  2762,  2764,
  2766,  2769,  2772,  2776,  2780,  2784,  2788,  2792,  2796,  2800,
  2804,  2806,  2808,  2810,  2812,  2816,  2818,  2820,  2822,  2824,
  2826,  2828,  2833,  2835,  2839,  2841,  2844,  2846,  2848,  2850,
  2852,  2854,  2856,  2858,  2861,  2864,  2867,  2869,  2871,  2873,
  2875,  2877,  2879,  2881,  2883,  2885,  2887,  2890,  2892,  2895,
  2897,  2899,  2901,  2903,  2905,  2908,  2910,  2912,  2914,  2916,
  2918,  2923,  2929,  2935,  2941,  2947,  2953,  2959,  2965,  2971,
  2977,  2983,  2990,  2992,  2999,  3001,  3006,  3014,  3017,  3018,
  3023,  3027,  3034,  3036,  3038,  3045,  3054,  3061,  3063,  3065,
  3070,  3077,  3082,  3088,  3092,  3098,  3103,  3109,  3111,  3113,
  3115,  3117,  3119,  3121,  3123,  3125,  3127,  3129,  3131,  3133,
  3134,  3136,  3137,  3139,  3141,  3143,  3145,  3147,  3149,  3151,
  3153,  3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,
  3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,
  3193,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
  3213,  3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229
};

static const short yyrhs[] = {   281,
     0,   281,   271,     0,   459,     0,   610,     0,   512,     0,
   314,     0,   301,     0,   600,     0,   480,     0,   282,     0,
   599,     0,   439,     0,   440,     0,   316,     0,   318,     0,
   291,     0,   513,     0,   506,     0,   535,     0,   504,     0,
   604,     0,    40,   656,     0,    70,   284,   122,   283,   174,
   295,   288,     0,    70,   286,   122,   141,   290,   174,   295,
   288,     0,    70,   284,   122,   283,   174,   293,     0,    70,
   286,   122,   141,   290,   174,   293,     0,    70,   297,   174,
   299,   289,     0,   575,     0,   172,   575,     0,     6,     0,
     6,   140,     0,   285,     0,   287,     0,   285,   272,   287,
     0,    56,     0,   155,     0,    80,     0,    44,     0,   180,
   615,     0,   145,   615,     0,   192,    70,   124,     0,     0,
   192,   202,   124,     0,     0,   702,     0,   150,   292,   284,
   122,   283,    63,   295,     0,   150,   292,   286,   122,   141,
   290,    63,   295,     0,   150,   284,   122,   283,    63,   295,
     0,   150,   286,   122,   141,   290,    63,   295,     0,   150,
   284,   122,   283,    63,   293,     0,   150,   286,   122,   141,
   290,    63,   293,     0,   150,   297,    63,   299,     0,    70,
   124,    61,     0,   294,     0,   293,   272,   294,     0,   293,
   272,   296,     0,   295,   272,   294,     0,   141,   702,     0,
   176,   706,     0,   187,   709,     0,   206,   703,     0,   296,
     0,   295,   272,   296,     0,   707,     0,   181,   707,     0,
    71,   707,     0,   298,     0,   297,   272,   298,     0,   703,
     0,   300,     0,   299,   272,   300,     0,   707,     0,   181,
   707,     0,    42,   302,     0,    60,   312,     0,    59,    65,
   303,     0,   687,   305,   149,   309,    52,   655,   111,   655,
     0,   487,     0,    18,     0,    34,   273,   659,   274,   493,
     0,     0,   306,     0,   273,   306,   274,     0,   307,     0,
   306,   272,   307,     0,   368,   304,   308,     0,     0,    19,
   230,     0,    19,   262,     0,    94,     0,   310,     0,   273,
   310,   274,     0,   368,   304,   311,     0,   133,   659,     0,
     0,    19,    97,     0,    19,   230,     0,   204,     0,    19,
   230,   204,     0,   696,    78,   313,   128,   313,    52,   655,
   111,   655,     0,   688,     0,   656,     0,    33,   315,     0,
    55,   695,   655,     0,   320,   595,    79,   699,   122,   575,
   321,     0,   141,   387,     0,   172,   355,     0,   176,   451,
     0,   187,   444,     0,    68,   337,     0,    37,   339,     0,
    48,   328,     0,   157,   322,     0,   206,   338,     0,   229,
   317,     0,   141,   388,     0,   172,   356,     0,   187,   445,
     0,    33,   125,     7,   319,     0,   141,   389,     0,   176,
   452,     0,   179,     0,     0,   617,     0,   616,     0,   362,
   273,   448,   644,   449,   274,     0,   659,   323,   324,   655,
   325,   326,     0,   105,     0,    14,     0,     0,     0,    29,
     0,     0,    98,   340,   662,   354,     0,     0,   327,     0,
   348,     0,   327,   348,     0,   365,   329,   482,   448,   330,
   449,   331,   364,     0,    10,     0,     0,    43,   448,   370,
     0,     0,     0,   332,     0,   333,     0,   332,   333,     0,
   334,     0,   335,     0,   336,     0,   118,    94,     0,   448,
    22,   273,   624,   274,   449,     0,   698,     0,   703,     0,
   341,   342,   345,     0,     0,   264,     0,   655,     0,     0,
   343,     0,   344,     0,   343,   344,     0,   132,   340,   659,
     0,    98,   340,   662,   354,     0,   181,   655,     0,   134,
   655,     0,   156,   112,   655,     0,     0,   346,     0,   347,
     0,   346,   347,     0,   348,     0,    43,    21,   156,   689,
     0,   255,    90,   655,     0,   349,   655,   350,     0,    90,
     0,     0,   351,     0,   352,     0,   351,   352,     0,   167,
   353,   662,     0,    98,   340,   662,   354,     0,     0,    12,
     0,    12,   130,     0,     0,   130,     0,   131,     0,   575,
   357,   273,   358,   274,     0,   575,   357,   273,   358,   274,
     0,    59,    90,   655,     0,    59,   655,     0,     0,   359,
     0,   358,   272,   359,     0,   360,     0,   375,     0,   365,
   363,   369,   449,   371,   364,     0,   365,   483,   361,     0,
   365,   361,     0,   362,   273,   448,   644,   449,   274,     0,
    27,    19,     0,    27,     0,   482,   448,     0,   622,   446,
     0,    23,   690,     0,     0,   622,     0,   622,     0,   368,
   482,     0,     0,    43,   370,     0,     0,   648,     0,   652,
     0,   653,     0,   654,     0,   686,     0,   645,     0,     0,
   372,     0,   373,     0,   372,   373,     0,   376,   374,     0,
   118,    94,     0,   145,   575,   615,   383,   381,     0,   382,
     0,   179,   381,     0,   139,    86,   381,     0,   376,   377,
     0,    30,   692,     0,     0,   378,     0,   379,     0,   380,
     0,   382,     0,   179,   616,   381,     0,   139,    86,   616,
   381,     0,    62,    86,   616,   145,   575,   615,   383,   381,
     0,   239,   595,    79,   699,     0,     0,   448,    22,   273,
   624,   274,   449,     0,   384,     0,   385,     0,   385,   384,
     0,   384,   385,     0,     0,   122,   180,   386,     0,   122,
    44,   386,     0,   203,     0,   156,    43,     0,   156,    94,
     0,   117,   201,     0,   702,   391,   392,    10,   446,   398,
   407,   449,     0,   702,   391,   392,    10,   446,   398,   407,
   449,     0,   702,   391,   392,    10,   446,   398,   407,   449,
     0,   702,   391,   392,    10,   446,   398,   407,   449,     0,
   273,   393,   274,     0,     0,   149,   273,   395,   274,     0,
     0,   394,     0,   393,   272,   394,     0,   366,   483,   397,
   449,     0,   396,     0,   395,   272,   396,     0,   366,   483,
     0,    43,   446,   370,     0,   264,   446,   370,     0,   446,
     0,   399,     0,     0,   400,     0,   399,   400,     0,    42,
   403,   401,   271,     0,   402,     0,   405,     0,   365,   483,
   404,     0,   184,     0,     0,   264,   370,     0,   369,     0,
   693,    36,    61,   273,   535,   274,     0,   410,     0,   407,
     0,    16,   408,    51,     0,   409,     0,   409,   430,     0,
     0,   406,     0,   409,   406,     0,   608,   271,     0,   600,
   271,     0,   411,     0,   412,     0,   413,     0,   414,     0,
   415,     0,   418,     0,   599,   271,     0,   419,     0,   421,
     0,   604,   271,     0,   426,     0,   416,     0,   417,     0,
   170,   271,     0,    58,   271,     0,   428,     0,   434,   271,
     0,   609,   271,     0,    55,   695,   271,     0,    55,   695,
   644,   271,     0,    55,   271,     0,    56,   141,   702,   423,
   424,   271,     0,    56,   246,   644,   271,     0,   427,    61,
   535,    82,   425,   429,    47,   406,     0,   427,    61,    56,
   246,   644,    82,   425,    47,   406,     0,    56,   246,   644,
    82,   425,   271,     0,    74,   273,   624,   274,   173,   406,
    50,   406,     0,    74,   273,   624,   274,   173,   406,     0,
   137,   644,   420,   271,     0,     0,   535,    82,   425,   271,
     0,   275,   708,     0,   647,     0,   273,   647,   274,     0,
     0,   148,   425,     0,   148,   273,   425,   274,     0,     0,
   422,     0,   621,     0,   425,   272,   621,     0,   425,   272,
   422,     0,   427,   191,   273,   624,   274,    47,   406,     0,
   701,   275,     0,     0,   227,   271,     0,   247,   271,     0,
   247,   701,   271,     0,    10,    36,   693,     0,     0,   431,
     0,   430,   431,     0,   189,   432,    47,   406,     0,   433,
     0,   432,   272,   433,     0,   165,   656,     0,    66,   697,
     0,    55,   695,     0,     9,     0,   435,     0,   437,     0,
   436,     0,   256,   693,     0,   257,   693,     0,   258,   438,
   693,    82,   425,     0,     0,    56,   141,   702,   423,     0,
    56,   260,   441,   392,    10,   398,   407,     0,   273,   442,
   274,     0,     0,   443,     0,   442,   272,   443,     0,   396,
   264,   651,     0,   709,   615,    10,   446,   539,   450,   447,
     0,   709,   615,    10,   446,   539,   450,   447,     0,     0,
     0,     0,     0,   192,    22,   124,     0,     0,   706,    61,
   575,   453,   454,   457,   448,   458,   449,     0,   706,    61,
   575,   453,   454,   457,   448,   458,   449,     0,     3,     0,
    76,     0,     0,   455,   456,     0,    15,     0,     5,     0,
    80,     0,   180,     0,    44,     0,    80,   125,   180,     0,
    80,   125,    44,     0,   180,   125,    80,     0,   180,   125,
    44,     0,    44,   125,    80,     0,    44,   125,   180,     0,
    80,   125,   180,   125,    44,     0,    80,   125,    44,   125,
   180,     0,   180,   125,    80,   125,    44,     0,   180,   125,
    44,   125,    80,     0,    44,   125,    80,   125,   180,     0,
    44,   125,   180,   125,    80,     0,   136,   657,     0,     0,
    10,   448,   398,   407,     0,     7,   460,     0,    55,   695,
   655,     0,   172,   575,   465,     0,   176,   477,     0,   141,
   390,     0,    37,   474,   475,     0,    48,   467,   463,     0,
    79,   473,     0,    43,   448,   370,     0,    22,   448,   273,
   624,   274,   449,     0,   464,     0,   463,   464,     0,   156,
   448,   461,   449,     0,     4,    30,   462,     0,     4,   336,
     0,    49,    43,     0,    49,    30,     0,   174,   622,     0,
   208,   368,   483,     0,   466,     0,   465,   272,   466,     0,
    49,   622,   472,     0,    49,    30,   692,     0,     4,   360,
     0,     4,   375,     0,   469,   622,   136,   659,     0,   469,
   467,   174,   622,     0,   469,   471,   208,   470,   449,     0,
   468,     0,   711,     0,   207,     0,   208,     0,   209,     0,
   210,     0,   211,     0,   212,     0,   213,     0,   214,     0,
   215,     0,   218,     0,   219,     0,   220,     0,   221,     0,
   222,     0,   225,     0,   226,     0,   233,     0,   234,     0,
   242,     0,   244,     0,   256,     0,   257,     0,   258,     0,
   259,     0,   239,     0,   263,     0,     7,     0,     7,   207,
     0,   483,   448,     0,   622,   446,     0,   622,     0,   205,
     0,   203,     0,     0,   699,     3,     0,   699,    76,     0,
     0,   476,     0,   475,   476,     0,     4,   327,     0,     4,
   255,    90,   655,     0,    49,   255,    90,     0,    16,   254,
     0,    51,   254,     0,   706,   453,   478,   457,   448,   479,
   449,     0,   454,     0,     0,   458,     0,     0,    49,   481,
     0,    55,   695,     0,    79,   699,     0,   141,   702,     0,
   172,   705,     0,   176,   706,     0,   187,   709,     0,    60,
   696,     0,    48,   694,     0,    59,    65,   687,     0,   157,
   659,     0,   206,   703,     0,    68,   698,     0,   483,     0,
   484,     0,   487,     0,   490,     0,   488,   276,   485,   277,
     0,   495,   276,   485,   277,   493,     0,   486,     0,   485,
   272,   486,     0,   661,     0,   661,   275,   661,     0,   488,
     0,   495,   493,     0,   494,     0,   499,     0,   502,     0,
   235,     0,   489,     0,   161,     0,    38,     0,   218,     0,
   219,     0,    81,     0,    92,     0,    18,   492,   491,   493,
     0,    18,   273,   660,   274,     0,    18,   273,   660,   272,
   656,   274,     0,    18,   273,   272,   656,   274,     0,   154,
   160,   660,     0,     0,   169,   656,     0,   169,   688,     0,
     0,    21,   156,   689,     0,     0,   498,   273,   659,   274,
     0,   498,     0,   498,   185,   273,   659,   274,     0,   497,
   273,   659,   274,     0,   497,     0,   496,   273,   659,   274,
     0,   183,     0,    21,   185,     0,    87,   185,     0,    21,
     0,    87,     0,   115,     0,   113,    21,     0,   113,    87,
     0,    95,   500,     0,   501,   500,     0,     0,   273,   661,
   274,     0,   273,   661,   272,   661,   274,     0,    41,     0,
    88,     0,    91,   503,     0,    93,    91,   503,     0,   144,
     0,    89,   138,     0,   273,   657,   274,     0,     0,   517,
     0,   505,     0,   534,     0,   156,    68,   698,   174,   661,
     0,   156,    68,   698,   174,   223,     0,   156,    68,   698,
   174,   268,   223,     0,   507,     0,   508,     0,   510,     0,
   244,   710,     0,   245,   244,   710,   509,     0,   123,     0,
     0,   153,   514,   174,   511,   710,     0,   244,     0,     0,
    25,   514,   515,     0,   153,   514,     0,   193,     0,     0,
   147,   516,     0,     0,   162,     0,     0,   156,   175,   518,
     0,   519,     0,     0,   520,     0,   519,   520,     0,   521,
     0,   522,     0,   523,     0,   527,     0,   143,   123,     0,
   143,   194,     0,   188,     0,   117,   188,     0,    84,   102,
   524,     0,   524,     0,   525,     0,   143,   177,   526,     0,
   143,    26,   526,     0,   162,     0,   162,   172,     0,   162,
   172,   166,     0,   186,     0,   117,   186,     0,     0,   146,
   530,     0,   158,     0,   142,     0,     0,   143,     0,   194,
     0,   531,     0,   530,   272,   531,     0,   533,   532,     0,
    61,   528,   529,     0,     0,   575,     0,   533,   272,   575,
     0,   156,   168,    79,   699,     0,   539,   536,   538,     0,
    61,   180,   537,     0,     0,   121,   617,     0,     0,   192,
   243,     0,     0,   542,   592,   598,     0,   542,   592,   598,
     0,   542,   592,   598,     0,   543,     0,   542,   178,   685,
   543,     0,   542,   178,     6,   543,     0,   544,     0,   155,
   549,   552,   553,   557,   582,   578,   581,   583,     0,     0,
     0,     0,     0,   550,   551,   546,     0,   550,   546,     0,
   551,     0,     0,   231,   662,   545,     0,   231,   273,   644,
   274,   545,     0,   231,   651,   545,     0,   232,   662,     0,
   232,   273,   546,   644,   274,     0,   232,   651,     0,    46,
     0,   684,     0,   554,     0,   269,     0,   555,     0,   554,
   272,   555,     0,   644,     0,   644,   556,   700,     0,    10,
     0,     0,    63,   558,     0,   559,     0,   558,   272,   559,
     0,   565,     0,   560,     0,   572,     0,   561,     0,   273,
   565,   274,     0,   273,   539,   274,   556,   562,   563,     0,
   704,     0,     0,   273,   564,   274,     0,     0,   700,     0,
   564,   272,   700,     0,   566,     0,   567,     0,   568,     0,
   559,   263,    85,   560,     0,   559,   114,   576,    85,   560,
     0,   559,   576,    85,   559,   569,     0,   570,     0,   571,
     0,   122,   624,     0,   239,   273,   617,   274,     0,   702,
   573,   556,   704,     0,   702,   573,     0,   273,   647,   274,
     0,     0,   575,     0,   705,   704,     0,   705,     0,    77,
     0,   100,   577,     0,   151,   577,     0,    64,   577,     0,
     0,   127,     0,     0,    71,    19,   579,     0,     0,   580,
     0,   579,   272,   580,     0,   644,     0,    73,   624,     0,
     0,   190,   624,     0,     0,   135,   584,     0,     0,   585,
   273,   586,   274,     0,    85,     0,   164,   108,     0,   108,
     0,   164,     0,     0,   587,     0,   587,   272,   586,     0,
   588,   589,     0,   584,     0,   705,     0,   705,   588,     0,
   114,     0,    79,   273,   590,   274,     0,   126,   699,   591,
     0,   699,     0,   699,   272,   590,     0,    79,   273,   590,
   274,     0,     0,   126,    19,   593,     0,     0,   594,     0,
   593,   272,   594,     0,   644,   595,   597,     0,    11,     0,
    45,     0,     0,   231,     0,   241,     0,   240,   547,   596,
   548,     0,     0,   259,   644,     0,   259,   644,   174,   644,
     0,     0,    80,    82,   575,   618,   182,   273,   647,   274,
     0,    80,    82,   575,   618,   539,     0,   601,     0,   602,
     0,    44,    63,   574,   582,   583,   592,   598,     0,    44,
    63,   574,   603,     0,   190,    35,   121,   693,     0,   605,
     0,   606,     0,   180,   574,   156,   607,   582,   583,   592,
   598,     0,   180,   574,   156,   607,   603,     0,   608,     0,
   607,   272,   608,     0,   623,   264,   644,     0,   671,     0,
   143,    18,   622,    63,   575,   611,   613,     0,    80,    18,
   622,    82,   575,   611,   613,     0,    60,    63,   612,   174,
   612,     0,    60,   174,   612,     0,     0,   492,     0,   651,
     0,   656,     0,   107,   614,     0,     0,   660,     0,   651,
     0,   616,     0,     0,   273,   617,   274,     0,   622,     0,
   617,   272,   622,     0,   619,     0,     0,   273,   620,   274,
     0,   623,     0,   620,   272,   623,     0,   622,     0,   704,
   278,   691,     0,   704,   278,   269,     0,   691,     0,   622,
     0,   704,   278,   691,     0,   640,     0,   118,   640,     0,
   626,     0,   624,   125,   624,     0,   624,     8,   624,     0,
   626,     0,   118,   640,     0,   625,   125,   624,     0,   625,
     8,   624,     0,   641,   125,   624,     0,   641,     8,   624,
     0,   627,     0,   273,   625,   274,     0,   118,   626,     0,
   628,     0,   631,     0,   632,     0,   633,     0,   634,     0,
   639,     0,   629,     0,   637,     0,   635,     0,   636,     0,
   638,     0,   644,   264,   644,     0,   644,   265,   644,     0,
   644,   266,   644,     0,   644,    67,   644,     0,   644,   101,
   644,     0,   644,   119,   644,     0,   644,   120,   644,     0,
   644,   116,   644,     0,   644,   264,     6,   273,   540,   274,
     0,   644,   265,     6,   273,   540,   274,     0,   644,   266,
     6,   273,   540,   274,     0,   644,    67,     6,   273,   540,
   274,     0,   644,   101,     6,   273,   540,   274,     0,   644,
   119,     6,   273,   540,   274,     0,   644,   120,     6,   273,
   540,   274,     0,   644,   116,     6,   273,   540,   274,     0,
   644,   264,   630,   273,   540,   274,     0,   644,   265,   630,
   273,   540,   274,     0,   644,   266,   630,   273,   540,   274,
     0,   644,    67,   630,   273,   540,   274,     0,   644,   101,
   630,   273,   540,   274,     0,   644,   119,   630,   273,   540,
   274,     0,   644,   120,   630,   273,   540,   274,     0,   644,
   116,   630,   273,   540,   274,     0,   163,     0,     9,     0,
   644,    83,    46,    63,   644,     0,   644,    83,   118,    46,
    63,   644,     0,   644,    17,   644,     8,   644,     0,   644,
   118,    17,   644,     8,   644,     0,   644,   103,   644,     0,
   644,   118,   103,   644,     0,   644,   103,   644,    54,   644,
     0,   644,   118,   103,   644,    54,   644,     0,   644,    75,
   642,     0,   644,   118,    75,   642,     0,   644,    31,   644,
     0,   644,   118,    31,   644,     0,   644,   167,   644,     0,
   644,   118,   167,   644,     0,   644,   167,   192,   644,     0,
   644,   118,   167,   192,   644,     0,    57,   273,   539,   274,
     0,   159,   273,   539,   274,     0,   644,    83,    94,     0,
   644,    83,   118,    94,     0,   248,     0,   249,     0,   250,
     0,   251,     0,   252,     0,   253,     0,   643,     0,   273,
   647,   274,     0,   273,   540,   274,     0,   621,     0,   646,
     0,   663,     0,   650,     0,   651,     0,   422,     0,   672,
     0,   673,     0,   671,     0,   268,   644,     0,   267,   644,
     0,   644,   267,   644,     0,   644,    28,   644,     0,   644,
    23,   690,     0,   644,   268,   644,     0,   644,   269,   644,
     0,   644,   270,   644,     0,   273,   644,   274,     0,   273,
   541,   274,     0,   652,     0,   653,     0,   654,     0,    39,
     0,   704,   278,    39,     0,    97,     0,   645,     0,   686,
     0,   220,     0,   221,     0,   222,     0,   621,   276,   647,
   277,     0,   644,     0,   647,   272,   644,     0,   650,     0,
   268,   649,     0,   197,     0,   196,     0,   195,     0,   223,
     0,   224,     0,   649,     0,   655,     0,    38,   199,     0,
   218,   199,     0,   219,   199,     0,   279,     0,   181,     0,
   225,     0,   226,     0,   233,     0,   234,     0,    66,     0,
   165,     0,   242,     0,   199,     0,   200,   199,     0,   657,
     0,   268,   658,     0,   196,     0,   196,     0,   657,     0,
   196,     0,   662,     0,   268,   662,     0,   196,     0,   664,
     0,   665,     0,   666,     0,   668,     0,    32,   273,   269,
   274,     0,    32,   273,   684,   644,   274,     0,    32,   273,
    46,   644,   274,     0,   171,   273,   684,   644,   274,     0,
   171,   273,    46,   644,   274,     0,    13,   273,   684,   644,
   274,     0,    13,   273,    46,   644,   274,     0,   110,   273,
   684,   644,   274,     0,   110,   273,    46,   644,   274,     0,
   106,   273,   684,   644,   274,     0,   106,   273,    46,   644,
   274,     0,    69,   273,   698,   272,   644,   274,     0,   667,
     0,   209,   273,   683,    63,   644,   274,     0,   669,     0,
    96,   273,   644,   274,     0,   228,   273,   644,    63,   644,
   670,   274,     0,    61,   644,     0,     0,   687,   273,   647,
   274,     0,   687,   273,   274,     0,    20,   273,   644,    10,
   367,   274,     0,   674,     0,   675,     0,   237,   273,   644,
   272,   644,   274,     0,   261,   273,   624,   272,   644,   272,
   644,   274,     0,   238,   273,   644,   272,   647,   274,     0,
   676,     0,   678,     0,   236,   681,   677,    51,     0,   236,
   681,   677,    50,   682,    51,     0,   189,   680,   173,   682,
     0,   677,   189,   680,   173,   682,     0,   236,   679,    51,
     0,   236,   679,    50,   682,    51,     0,   189,   624,   173,
   682,     0,   679,   189,   624,   173,   682,     0,   644,     0,
   644,     0,   644,     0,   210,     0,   211,     0,   212,     0,
   213,     0,   214,     0,   215,     0,   216,     0,   217,     0,
     6,     0,     0,    46,     0,     0,    94,     0,   198,     0,
   711,     0,   711,     0,   711,     0,   711,     0,   711,     0,
   711,     0,   711,     0,   711,     0,   711,     0,   711,     0,
   711,     0,   711,     0,   711,     0,   711,     0,   711,     0,
   711,     0,   711,     0,   711,     0,   711,     0,   711,     0,
   711,     0,   711,     0,   711,     0,   198,     0,   712,     0,
   227,     0,   230,     0,   228,     0,   238,     0,   241,     0,
   247,     0,   243,     0,   237,     0,   240,     0,   246,     0,
   248,     0,   249,     0,   250,     0,   260,     0,   254,     0,
   255,     0,   261,     0,   262,     0,   216,     0,   217,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   539,   541,   545,   546,   547,   548,   549,   550,   551,   552,
   553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
   563,   564,   572,   576,   580,   584,   588,   593,   594,   598,
   600,   602,   606,   607,   611,   615,   617,   619,   621,   623,
   627,   629,   633,   635,   639,   646,   651,   656,   661,   666,
   671,   676,   682,   686,   687,   689,   691,   695,   697,   699,
   701,   705,   706,   713,   715,   717,   721,   722,   726,   730,
   731,   735,   737,   744,   748,   750,   755,   761,   762,   764,
   770,   772,   773,   777,   778,   784,   789,   791,   793,   795,
   799,   800,   804,   807,   812,   814,   816,   818,   821,   826,
   832,   834,   841,   845,   848,   851,   853,   855,   857,   859,
   861,   863,   865,   867,   872,   876,   878,   880,   890,   894,
   896,   907,   909,   913,   915,   916,   922,   928,   930,   932,
   936,   938,   942,   944,   948,   950,   953,   954,   961,   979,
   981,   985,   987,   991,   993,   996,   997,  1000,  1005,  1006,
  1009,  1013,  1022,  1030,  1039,  1044,  1045,  1048,  1054,  1056,
  1059,  1060,  1064,  1066,  1068,  1070,  1072,  1076,  1078,  1081,
  1082,  1086,  1091,  1093,  1142,  1199,  1203,  1204,  1207,  1208,
  1211,  1213,  1217,  1218,  1219,  1222,  1223,  1224,  1230,  1235,
  1240,  1242,  1244,  1248,  1249,  1253,  1254,  1261,  1265,  1269,
  1278,  1284,  1285,  1288,  1290,  1295,  1297,  1302,  1308,  1314,
  1318,  1324,  1326,  1330,  1331,  1332,  1333,  1334,  1335,  1338,
  1340,  1343,  1344,  1348,  1353,  1355,  1360,  1361,  1363,  1371,
  1375,  1377,  1380,  1381,  1382,  1383,  1386,  1390,  1394,  1401,
  1408,  1413,  1419,  1422,  1424,  1426,  1428,  1432,  1435,  1439,
  1442,  1445,  1448,  1457,  1468,  1478,  1488,  1498,  1500,  1504,
  1506,  1510,  1511,  1515,  1521,  1522,  1526,  1531,  1533,  1535,
  1539,  1541,  1545,  1546,  1550,  1554,  1555,  1558,  1563,  1565,
  1569,  1571,  1575,  1580,  1581,  1584,  1588,  1590,  1592,  1596,
  1597,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,
  1610,  1611,  1612,  1613,  1614,  1615,  1616,  1618,  1620,  1621,
  1622,  1625,  1627,  1631,  1635,  1640,  1644,  1649,  1653,  1657,
  1659,  1663,  1667,  1673,  1678,  1683,  1685,  1687,  1691,  1693,
  1695,  1699,  1700,  1701,  1703,  1707,  1711,  1713,  1717,  1719,
  1721,  1726,  1729,  1733,  1734,  1738,  1743,  1744,  1748,  1750,
  1752,  1754,  1758,  1759,  1760,  1763,  1767,  1771,  1775,  1817,
  1824,  1832,  1834,  1838,  1839,  1843,  1849,  1856,  1879,  1883,
  1889,  1893,  1899,  1903,  1911,  1922,  1933,  1935,  1937,  1941,
  1945,  1947,  1951,  1953,  1955,  1957,  1959,  1961,  1963,  1965,
  1967,  1969,  1971,  1973,  1975,  1977,  1979,  1983,  1985,  1989,
  1995,  1999,  2002,  2009,  2011,  2013,  2016,  2019,  2024,  2028,
  2034,  2035,  2039,  2048,  2052,  2054,  2056,  2058,  2060,  2064,
  2065,  2069,  2071,  2073,  2075,  2081,  2084,  2086,  2090,  2098,
  2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,
  2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,
  2119,  2120,  2121,  2122,  2123,  2124,  2127,  2129,  2133,  2135,
  2140,  2146,  2148,  2150,  2154,  2156,  2163,  2169,  2170,  2174,
  2188,  2190,  2192,  2194,  2208,  2218,  2219,  2223,  2224,  2230,
  2234,  2236,  2238,  2240,  2242,  2244,  2246,  2248,  2250,  2252,
  2254,  2256,  2263,  2264,  2267,  2268,  2271,  2276,  2283,  2284,
  2288,  2297,  2303,  2304,  2307,  2308,  2309,  2310,  2327,  2332,
  2337,  2356,  2373,  2380,  2381,  2388,  2392,  2398,  2404,  2412,
  2416,  2422,  2426,  2430,  2436,  2440,  2447,  2453,  2459,  2467,
  2472,  2477,  2484,  2485,  2486,  2489,  2490,  2493,  2494,  2495,
  2502,  2506,  2517,  2523,  2577,  2639,  2640,  2647,  2660,  2665,
  2670,  2677,  2679,  2686,  2687,  2688,  2692,  2697,  2702,  2713,
  2714,  2715,  2718,  2722,  2726,  2728,  2732,  2736,  2737,  2740,
  2744,  2748,  2749,  2752,  2754,  2758,  2759,  2763,  2767,  2768,
  2772,  2773,  2777,  2778,  2779,  2780,  2783,  2785,  2789,  2791,
  2795,  2797,  2800,  2802,  2804,  2808,  2810,  2812,  2816,  2818,
  2820,  2824,  2828,  2830,  2832,  2836,  2838,  2842,  2843,  2847,
  2851,  2853,  2857,  2858,  2863,  2871,  2875,  2877,  2881,  2883,
  2887,  2889,  2896,  2900,  2905,  2910,  2912,  2914,  2918,  2921,
  2933,  2937,  2941,  2945,  2949,  2951,  2953,  2955,  2959,  2961,
  2963,  2967,  2969,  2971,  2975,  2977,  2981,  2983,  2987,  2988,
  2992,  2993,  2997,  2998,  3003,  3007,  3008,  3012,  3013,  3016,
  3017,  3018,  3023,  3028,  3029,  3033,  3035,  3039,  3040,  3044,
  3045,  3046,  3049,  3054,  3059,  3063,  3064,  3067,  3071,  3075,
  3078,  3083,  3085,  3089,  3090,  3095,  3100,  3102,  3104,  3106,
  3108,  3112,  3113,  3119,  3121,  3125,  3126,  3132,  3135,  3137,
  3141,  3143,  3150,  3152,  3156,  3160,  3162,  3164,  3170,  3172,
  3176,  3177,  3181,  3183,  3186,  3187,  3191,  3193,  3195,  3199,
  3200,  3204,  3206,  3212,  3214,  3218,  3219,  3223,  3227,  3229,
  3231,  3235,  3237,  3241,  3243,  3249,  3252,  3260,  3267,  3270,
  3277,  3278,  3281,  3285,  3289,  3296,  3297,  3300,  3305,  3310,
  3311,  3315,  3319,  3326,  3328,  3332,  3334,  3336,  3339,  3340,
  3343,  3347,  3349,  3352,  3354,  3360,  3361,  3365,  3369,  3370,
  3375,  3376,  3380,  3384,  3385,  3390,  3391,  3394,  3399,  3404,
  3407,  3414,  3415,  3417,  3418,  3420,  3424,  3425,  3427,  3429,
  3433,  3435,  3439,  3440,  3442,  3446,  3447,  3448,  3449,  3450,
  3451,  3452,  3453,  3454,  3455,  3456,  3461,  3463,  3465,  3467,
  3469,  3471,  3473,  3475,  3481,  3483,  3485,  3487,  3489,  3491,
  3493,  3495,  3497,  3499,  3501,  3503,  3505,  3507,  3509,  3511,
  3515,  3516,  3522,  3524,  3528,  3530,  3535,  3537,  3539,  3541,
  3546,  3548,  3552,  3554,  3558,  3560,  3562,  3564,  3568,  3572,
  3576,  3578,  3582,  3587,  3592,  3599,  3604,  3609,  3618,  3619,
  3623,  3630,  3631,  3632,  3633,  3634,  3635,  3636,  3637,  3638,
  3639,  3641,  3643,  3650,  3652,  3654,  3661,  3668,  3675,  3677,
  3679,  3680,  3681,  3682,  3684,  3686,  3690,  3691,  3694,  3710,
  3726,  3730,  3734,  3735,  3739,  3740,  3744,  3746,  3748,  3750,
  3752,  3756,  3757,  3759,  3775,  3791,  3795,  3799,  3801,  3805,
  3809,  3812,  3815,  3818,  3821,  3826,  3828,  3833,  3834,  3838,
  3845,  3852,  3859,  3866,  3867,  3871,  3877,  3878,  3879,  3880,
  3883,  3885,  3887,  3890,  3897,  3906,  3913,  3922,  3924,  3926,
  3928,  3934,  3943,  3946,  3950,  3951,  3955,  3964,  3966,  3970,
  3972,  3976,  3982,  3983,  3986,  3990,  3993,  3997,  3998,  4001,
  4003,  4007,  4009,  4013,  4015,  4019,  4021,  4025,  4028,  4031,
  4034,  4036,  4038,  4040,  4042,  4044,  4046,  4048,  4052,  4053,
  4056,  4057,  4060,  4068,  4071,  4074,  4077,  4080,  4083,  4086,
  4089,  4092,  4095,  4098,  4101,  4104,  4107,  4110,  4113,  4116,
  4119,  4122,  4125,  4128,  4131,  4134,  4137,  4142,  4143,  4148,
  4150,  4151,  4152,  4153,  4154,  4155,  4156,  4157,  4158,  4159,
  4160,  4161,  4163,  4164,  4165,  4166,  4167,  4168,  4169
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ACTIVE",
"ADD","AFTER","ALL","ALTER","AND","ANY","AS","ASC","AT","AVG","AUTO","BEFORE",
"BEGIN","BETWEEN","BLOB","BY","CAST","CHARACTER","CHECK","COLLATE","COMMA","COMMIT",
"COMMITTED","COMPUTED","CONCATENATE","CONDITIONAL","CONSTRAINT","CONTAINING",
"COUNT","CREATE","CSTRING","CURRENT","CURSOR","DATABASE","DATE","DB_KEY","KW_DEBUG",
"DECIMAL","DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO","DOMAIN","DROP",
"ELSE","END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE","EXISTS","EXIT",
"EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION","GDSCODE","GEQ",
"GENERATOR","GEN_ID","GRANT","GROUP","GTR","HAVING","IF","KW_IN","INACTIVE",
"INNER","INPUT_TYPE","INDEX","INSERT","INTEGER","INTO","IS","ISOLATION","JOIN",
"KEY","KW_CHAR","KW_DEC","KW_DOUBLE","KW_FILE","KW_FLOAT","KW_INT","KW_LONG",
"KW_NULL","KW_NUMERIC","KW_UPPER","KW_VALUE","LENGTH","LPAREN","LEFT","LEQ",
"LEVEL","LIKE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE","MINIMUM",
"MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT","NOT_GTR",
"NOT_LSS","OF","ON","ONLY","OPTION","OR","ORDER","OUTER","OUTPUT_TYPE","OVERFLOW",
"PAGE","PAGES","KW_PAGE_SIZE","PARAMETER","PASSWORD","PLAN","POSITION","POST_EVENT",
"PRECISION","PRIMARY","PRIVILEGES","PROCEDURE","PROTECTED","READ","REAL","REFERENCES",
"RESERVING","RETAIN","RETURNING_VALUES","RETURNS","REVOKE","RIGHT","RPAREN",
"ROLLBACK","SEGMENT","SELECT","SET","SHADOW","KW_SHARED","SINGULAR","KW_SIZE",
"SMALLINT","SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING","STATISTICS",
"SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION","TRIGGER","UNCOMMITTED",
"UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR","VARIABLE","VARYING","VERSION",
"VIEW","WAIT","WHEN","WHERE","WHILE","WITH","WORK","WRITE","FLOAT_NUMBER","NUMBER",
"NUMERIC","SYMBOL","STRING","INTRODUCER","ACTION","ADMIN","CASCADE","FREE_IT",
"RESTRICT","ROLE","COLUMN","TYPE","EXTRACT","YEAR","MONTH","DAY","HOUR","MINUTE",
"SECOND","WEEKDAY","YEARDAY","TIME","TIMESTAMP","CURRENT_DATE","CURRENT_TIME",
"CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT","CURRENT_USER","CURRENT_ROLE",
"KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR","FIRST","SKIP","CURRENT_CONNECTION",
"CURRENT_TRANSACTION","BIGINT","CASE","NULLIF","COALESCE","USING","NULLS","LAST",
"ROW_COUNT","LOCK","SAVEPOINT","RELEASE","STATEMENT","LEAVE","INSERTING","UPDATING",
"DELETING","KW_INSERTING","KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE",
"OPEN","CLOSE","FETCH","ROWS","BLOCK","IIF","SCALAR_ARRAY","CROSS","'='","'<'",
"'>'","'+'","'-'","'*'","'/'","';'","','","'('","')'","':'","'['","']'","'.'",
"'?'","top","statement","grant","prot_table_name","privileges","privilege_list",
"proc_privileges","privilege","grant_option","role_admin_option","simple_proc_name",
"revoke","rev_grant_option","grantee_list","grantee","user_grantee_list","user_grantee",
"role_name_list","role_name","role_grantee_list","role_grantee","declare","declare_clause",
"udf_decl_clause","udf_data_type","arg_desc_list1","arg_desc_list","arg_desc",
"param_mechanism","return_value1","return_value","return_mechanism","filter_decl_clause",
"blob_filter_subtype","create","create_clause","recreate","recreate_clause",
"replace","replace_clause","unique_opt","index_definition","shadow_clause","manual_auto",
"conditional","first_file_length","sec_shadow_files","db_file_list","domain_clause",
"as_opt","domain_default_opt","domain_constraint_clause","domain_constraint_list",
"domain_constraint_def","domain_constraint","null_constraint","domain_check_constraint",
"generator_clause","role_clause","db_clause","equals","db_name","db_initial_desc1",
"db_initial_desc","db_initial_option","db_rem_desc1","db_rem_desc","db_rem_option",
"db_file","file1","file_desc1","file_desc","file_clause","file_clause_noise",
"page_noise","table_clause","rtable_clause","external_file","table_elements",
"table_element","column_def","def_computed","computed_by","data_type_or_domain",
"collate_clause","column_def_name","simple_column_def_name","data_type_descriptor",
"init_data_type","default_opt","default_value","column_constraint_clause","column_constraint_list",
"column_constraint_def","column_constraint","table_constraint_definition","constraint_name_opt",
"table_constraint","unique_constraint","primary_constraint","referential_constraint",
"constraint_index_opt","check_constraint","referential_trigger_action","update_rule",
"delete_rule","referential_action","procedure_clause","rprocedure_clause","replace_procedure_clause",
"alter_procedure_clause","input_parameters","output_parameters","input_proc_parameters",
"input_proc_parameter","output_proc_parameters","proc_parameter","default_par_opt",
"local_declaration_list","local_declarations","local_declaration","local_declaration_item",
"var_declaration_item","var_decl_opt","var_init_opt","cursor_declaration_item",
"proc_block","full_proc_block","full_proc_block_body","proc_statements","proc_statement",
"excp_statement","raise_statement","exec_procedure","exec_sql","for_select",
"for_exec_into","exec_into","if_then_else","post_event","event_argument_opt",
"singleton_select","variable","proc_inputs","proc_outputs","variable_list","while",
"label_opt","breakleave","cursor_def","excp_hndl_statements","excp_hndl_statement",
"errors","err","cursor_statement","open_cursor","close_cursor","fetch_cursor",
"fetch_opt","invoke_procedure","invoke_block","block_input_params","block_parameters",
"block_parameter","view_clause","rview_clause","begin_string","end_string","begin_trigger",
"end_trigger","check_opt","def_trigger_clause","replace_trigger_clause","trigger_active",
"trigger_type","trigger_type_prefix","trigger_type_suffix","trigger_position",
"trigger_action","alter","alter_clause","domain_default_opt2","domain_check_constraint2",
"alter_domain_ops","alter_domain_op","alter_ops","alter_op","alter_column_name",
"keyword_or_column","col_opt","alter_data_type_or_domain","alter_col_name","drop_behaviour",
"alter_index_clause","init_alter_db","alter_db","db_alter_clause","alter_trigger_clause",
"new_trigger_type","new_trigger_action","drop","drop_clause","data_type","non_array_type",
"array_type","array_spec","array_range","simple_type","non_charset_simple_type",
"integer_keyword","blob_type","blob_segsize","blob_subtype","charset_clause",
"national_character_type","character_type","varying_keyword","character_keyword",
"national_character_keyword","numeric_type","prec_scale","decimal_keyword","float_type",
"precision_opt","set","set_generator","savepoint","set_savepoint","release_savepoint",
"release_only_opt","undo_savepoint","optional_savepoint","commit","rollback",
"optional_work","optional_retain","opt_snapshot","set_transaction","tran_opt_list_m",
"tran_opt_list","tran_opt","access_mode","lock_wait","isolation_mode","iso_mode",
"snap_shot","version_mode","tbl_reserve_options","lock_type","lock_mode","restr_list",
"restr_option","table_lock","table_list","set_statistics","select","for_update_clause",
"for_update_list","lock_clause","select_expr","column_select","column_singleton",
"select_expr_body","query_term","query_spec","begin_limit","end_limit","begin_first",
"end_first","limit_clause","first_clause","skip_clause","distinct_clause","select_list",
"select_items","select_item","as_noise","from_clause","from_list","table_reference",
"table_primary","derived_table","correlation_name","derived_column_list","alias_list",
"joined_table","cross_join","natural_join","qualified_join","join_specification",
"join_condition","named_columns_join","table_proc","table_proc_inputs","table_name",
"simple_table_name","join_type","outer_noise","group_clause","group_by_list",
"group_by_item","having_clause","where_clause","plan_clause","plan_expression",
"plan_type","plan_item_list","plan_item","table_or_alias_list","access_type",
"index_list","extra_indices_opt","order_clause","order_list","order_item","order_direction",
"nulls_placement","nulls_clause","rows_clause","insert","delete","delete_searched",
"delete_positioned","cursor_clause","update","update_searched","update_positioned",
"assignments","assignment","exec_udf","blob","filter_clause","blob_subtype_value",
"segment_clause","segment_length","column_parens_opt","column_parens","column_list",
"ins_column_parens_opt","ins_column_parens","ins_column_list","column_name",
"simple_column_name","update_column_name","search_condition","bracable_search_condition",
"simple_search_condition","predicate","comparison_predicate","quantified_predicate",
"some","distinct_predicate","between_predicate","like_predicate","in_predicate",
"containing_predicate","starting_predicate","exists_predicate","singular_predicate",
"null_predicate","trigger_action_predicate","special_trigger_action_predicate",
"in_predicate_value","table_subquery","value","datetime_value_expression","array_element",
"value_list","constant","u_numeric_constant","u_constant","parameter","current_user",
"current_role","internal_info","sql_string","signed_short_integer","nonneg_short_integer",
"neg_short_integer","pos_short_integer","unsigned_short_integer","signed_long_integer",
"long_integer","function","aggregate_function","generate_value_function","numeric_value_function",
"extract_expression","string_value_function","substring_function","string_length_opt",
"udf","cast_specification","case_expression","case_abbreviation","case_specification",
"simple_case","simple_when_clause","searched_case","searched_when_clause","when_operand",
"case_operand","case_result","timestamp_part","all_noise","distinct_noise","null_value",
"symbol_UDF_name","symbol_blob_subtype_name","symbol_character_set_name","symbol_collation_name",
"symbol_column_name","symbol_constraint_name","symbol_cursor_name","symbol_domain_name",
"symbol_exception_name","symbol_filter_name","symbol_gdscode_name","symbol_generator_name",
"symbol_index_name","symbol_item_alias_name","symbol_label_name","symbol_procedure_name",
"symbol_role_name","symbol_table_alias_name","symbol_table_name","symbol_trigger_name",
"symbol_user_name","symbol_variable_name","symbol_view_name","symbol_savepoint_name",
"valid_symbol_name","non_reserved_word", NULL
};
#endif

static const short yyr1[] = {     0,
   280,   280,   281,   281,   281,   281,   281,   281,   281,   281,
   281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
   281,   281,   282,   282,   282,   282,   282,   283,   283,   284,
   284,   284,   285,   285,   286,   287,   287,   287,   287,   287,
   288,   288,   289,   289,   290,   291,   291,   291,   291,   291,
   291,   291,   292,   293,   293,   293,   293,   294,   294,   294,
   294,   295,   295,   296,   296,   296,   297,   297,   298,   299,
   299,   300,   300,   301,   302,   302,   303,   304,   304,   304,
   305,   305,   305,   306,   306,   307,   308,   308,   308,   308,
   309,   309,   310,   310,   311,   311,   311,   311,   311,   312,
   313,   313,   314,   315,   315,   315,   315,   315,   315,   315,
   315,   315,   315,   315,   316,   317,   317,   317,   318,   319,
   319,   320,   320,   321,   321,   321,   322,   323,   323,   323,
   324,   324,   325,   325,   326,   326,   327,   327,   328,   329,
   329,   330,   330,   331,   331,   332,   332,   333,   334,   334,
   335,   336,   337,   338,   339,   340,   340,   341,   342,   342,
   343,   343,   344,   344,   344,   344,   344,   345,   345,   346,
   346,   347,   347,   347,   348,   349,   350,   350,   351,   351,
   352,   352,   353,   353,   353,   354,   354,   354,   355,   356,
   357,   357,   357,   358,   358,   359,   359,   360,   360,   360,
   361,   362,   362,   363,   363,   364,   364,   365,   366,   367,
   368,   369,   369,   370,   370,   370,   370,   370,   370,   371,
   371,   372,   372,   373,   374,   374,   374,   374,   374,   375,
   376,   376,   377,   377,   377,   377,   378,   379,   380,   381,
   381,   382,   383,   383,   383,   383,   383,   384,   385,   386,
   386,   386,   386,   387,   388,   389,   390,   391,   391,   392,
   392,   393,   393,   394,   395,   395,   396,   397,   397,   397,
   398,   398,   399,   399,   400,   401,   401,   402,   403,   403,
   404,   404,   405,   406,   406,   407,   408,   408,   408,   409,
   409,   410,   410,   410,   410,   410,   410,   410,   410,   410,
   410,   410,   410,   410,   410,   410,   410,   410,   410,   410,
   410,   411,   411,   412,   413,   414,   415,   416,   417,   418,
   418,   419,   420,   421,   422,   423,   423,   423,   424,   424,
   424,   425,   425,   425,   425,   426,   427,   427,   428,   428,
   428,   429,   429,   430,   430,   431,   432,   432,   433,   433,
   433,   433,   434,   434,   434,   435,   436,   437,   438,   439,
   440,   441,   441,   442,   442,   443,   444,   445,   446,   447,
   448,   449,   450,   450,   451,   452,   453,   453,   453,   454,
   455,   455,   456,   456,   456,   456,   456,   456,   456,   456,
   456,   456,   456,   456,   456,   456,   456,   457,   457,   458,
   459,   460,   460,   460,   460,   460,   460,   460,   461,   462,
   463,   463,   464,   464,   464,   464,   464,   464,   464,   465,
   465,   466,   466,   466,   466,   466,   466,   466,   467,   468,
   468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
   468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
   468,   468,   468,   468,   468,   468,   469,   469,   470,   470,
   471,   472,   472,   472,   473,   473,   474,   475,   475,   476,
   476,   476,   476,   476,   477,   478,   478,   479,   479,   480,
   481,   481,   481,   481,   481,   481,   481,   481,   481,   481,
   481,   481,   482,   482,   483,   483,   484,   484,   485,   485,
   486,   486,   487,   487,   488,   488,   488,   488,   488,   488,
   488,   488,   488,   489,   489,   490,   490,   490,   490,   491,
   491,   492,   492,   492,   493,   493,   494,   494,   494,   495,
   495,   495,   496,   496,   496,   497,   497,   498,   498,   498,
   499,   499,   500,   500,   500,   501,   501,   502,   502,   502,
   502,   503,   503,   504,   504,   504,   505,   505,   505,   506,
   506,   506,   507,   508,   509,   509,   510,   511,   511,   512,
   513,   514,   514,   515,   515,   516,   516,   517,   518,   518,
   519,   519,   520,   520,   520,   520,   521,   521,   522,   522,
   523,   523,   524,   524,   524,   525,   525,   525,   526,   526,
   526,   527,   528,   528,   528,   529,   529,   530,   530,   531,
   532,   532,   533,   533,   534,   535,   536,   536,   537,   537,
   538,   538,   539,   540,   541,   542,   542,   542,   543,   544,
   545,   546,   547,   548,   549,   549,   549,   549,   550,   550,
   550,   551,   551,   551,   552,   552,   553,   553,   554,   554,
   555,   555,   556,   556,   557,   558,   558,   559,   559,   560,
   560,   560,   561,   562,   562,   563,   563,   564,   564,   565,
   565,   565,   566,   567,   568,   569,   569,   570,   571,   572,
   572,   573,   573,   574,   574,   575,   576,   576,   576,   576,
   576,   577,   577,   578,   578,   579,   579,   580,   581,   581,
   582,   582,   583,   583,   584,   585,   585,   585,   585,   585,
   586,   586,   587,   587,   588,   588,   589,   589,   589,   590,
   590,   591,   591,   592,   592,   593,   593,   594,   595,   595,
   595,   596,   596,   597,   597,   598,   598,   598,   599,   599,
   600,   600,   601,   602,   603,   604,   604,   605,   606,   607,
   607,   608,   609,   610,   610,   611,   611,   611,   612,   612,
   492,   613,   613,   614,   614,   615,   615,   616,   617,   617,
   618,   618,   619,   620,   620,   621,   621,   621,   622,   623,
   623,   624,   624,   624,   624,   624,   625,   625,   625,   625,
   625,   625,   626,   626,   626,   627,   627,   627,   627,   627,
   627,   627,   627,   627,   627,   627,   628,   628,   628,   628,
   628,   628,   628,   628,   629,   629,   629,   629,   629,   629,
   629,   629,   629,   629,   629,   629,   629,   629,   629,   629,
   630,   630,   631,   631,   632,   632,   633,   633,   633,   633,
   634,   634,   635,   635,   636,   636,   636,   636,   637,   638,
   639,   639,   640,   640,   640,   641,   641,   641,   642,   642,
   643,   644,   644,   644,   644,   644,   644,   644,   644,   644,
   644,   644,   644,   644,   644,   644,   644,   644,   644,   644,
   644,   644,   644,   644,   644,   644,   644,   644,   645,   645,
   645,   646,   647,   647,   648,   648,   649,   649,   649,   649,
   649,   650,   650,   650,   650,   650,   651,   652,   652,   653,
   654,   654,   654,   654,   654,   655,   655,   656,   656,   657,
   658,   659,   660,   661,   661,   662,   663,   663,   663,   663,
   664,   664,   664,   664,   664,   664,   664,   664,   664,   664,
   664,   665,   666,   667,   668,   668,   669,   670,   670,   671,
   671,   672,   673,   673,   674,   674,   674,   675,   675,   676,
   676,   677,   677,   678,   678,   679,   679,   680,   681,   682,
   683,   683,   683,   683,   683,   683,   683,   683,   684,   684,
   685,   685,   686,   687,   688,   689,   690,   691,   692,   693,
   694,   695,   696,   697,   698,   699,   700,   701,   702,   703,
   704,   705,   706,   707,   708,   709,   710,   711,   711,   712,
   712,   712,   712,   712,   712,   712,   712,   712,   712,   712,
   712,   712,   712,   712,   712,   712,   712,   712,   712
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     2,     7,     8,     6,     7,     5,     1,     2,     1,
     2,     1,     1,     3,     1,     1,     1,     1,     2,     2,
     3,     0,     3,     0,     1,     7,     8,     6,     7,     6,
     7,     4,     3,     1,     3,     3,     3,     2,     2,     2,
     2,     1,     3,     1,     2,     2,     1,     3,     1,     1,
     3,     1,     2,     2,     2,     3,     8,     1,     1,     5,
     0,     1,     3,     1,     3,     3,     0,     2,     2,     1,
     1,     3,     3,     2,     0,     2,     2,     1,     3,     9,
     1,     1,     2,     3,     7,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     4,     2,
     2,     1,     0,     1,     1,     6,     6,     1,     1,     0,
     0,     1,     0,     4,     0,     1,     1,     2,     8,     1,
     0,     3,     0,     0,     1,     1,     2,     1,     1,     1,
     2,     6,     1,     1,     3,     0,     1,     1,     0,     1,
     1,     2,     3,     4,     2,     2,     3,     0,     1,     1,
     2,     1,     4,     3,     3,     1,     0,     1,     1,     2,
     3,     4,     0,     1,     2,     0,     1,     1,     5,     5,
     3,     2,     0,     1,     3,     1,     1,     6,     3,     2,
     6,     2,     1,     2,     2,     2,     0,     1,     1,     2,
     0,     2,     0,     1,     1,     1,     1,     1,     1,     0,
     1,     1,     2,     2,     2,     5,     1,     2,     3,     2,
     2,     0,     1,     1,     1,     1,     3,     4,     8,     4,
     0,     6,     1,     1,     2,     2,     0,     3,     3,     1,
     2,     2,     2,     8,     8,     8,     8,     3,     0,     4,
     0,     1,     3,     4,     1,     3,     2,     3,     3,     1,
     1,     0,     1,     2,     4,     1,     1,     3,     1,     0,
     2,     1,     6,     1,     1,     3,     1,     2,     0,     1,
     2,     2,     2,     1,     1,     1,     1,     1,     1,     2,
     1,     1,     2,     1,     1,     1,     2,     2,     1,     2,
     2,     3,     4,     2,     6,     4,     8,     9,     6,     8,
     6,     4,     0,     4,     2,     1,     3,     0,     2,     4,
     0,     1,     1,     3,     3,     7,     2,     0,     2,     2,
     3,     3,     0,     1,     2,     4,     1,     3,     2,     2,
     2,     1,     1,     1,     1,     2,     2,     5,     0,     4,
     7,     3,     0,     1,     3,     3,     7,     7,     0,     0,
     0,     0,     3,     0,     9,     9,     1,     1,     0,     2,
     1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
     3,     5,     5,     5,     5,     5,     5,     2,     0,     4,
     2,     3,     3,     2,     2,     3,     3,     2,     3,     6,
     1,     2,     4,     3,     2,     2,     2,     2,     3,     1,
     3,     3,     3,     2,     2,     4,     4,     5,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
     1,     1,     1,     0,     2,     2,     0,     1,     2,     2,
     4,     3,     2,     2,     7,     1,     0,     1,     0,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
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
     2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
     1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
     1,     4,     1,     3,     1,     2,     1,     1,     1,     1,
     1,     1,     1,     2,     2,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
     1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
     4,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     6,     1,     6,     1,     4,     7,     2,     0,     4,
     3,     6,     1,     1,     6,     8,     6,     1,     1,     4,
     6,     4,     5,     3,     5,     4,     5,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
     1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,   573,   123,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   573,   638,     0,     0,     0,     0,     0,     1,
    10,    16,     7,     6,    14,    15,    12,    13,     3,     9,
    20,   555,    18,   560,   561,   562,     5,    17,   554,   556,
    19,   618,   725,   626,   629,    11,     8,   741,   742,    21,
   746,   747,     4,   467,     0,     0,     0,     0,     0,     0,
   401,   572,   575,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   122,     0,     0,   103,   731,   920,     0,    22,
   918,     0,     0,    74,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   480,     0,   363,
    30,    38,    35,    37,   767,    36,   767,  1008,  1028,  1029,
  1010,  1012,  1011,  1017,  1013,  1018,  1014,  1016,  1019,  1015,
  1020,  1021,  1022,  1024,  1025,  1023,  1026,  1027,     0,    32,
     0,    33,     0,    67,    69,  1000,  1009,     0,     0,     0,
     0,     0,     0,     0,     0,   571,     0,     0,   980,   632,
   637,     0,     0,   580,     0,   684,   686,  1002,     0,     0,
     0,   115,   563,  1007,     0,     2,     0,   622,     0,   982,
   738,     0,   431,   432,   433,   434,   435,   436,   437,   438,
   439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
   455,   449,   450,   451,   452,   453,   454,   456,     0,   429,
   430,     0,   992,   408,     0,   996,   405,   259,   999,     0,
   686,   404,   379,  1003,   577,   570,   916,     0,   111,   159,
   158,   112,   141,   208,   779,   988,     0,   110,   153,   995,
     0,   106,   259,   113,   922,   130,   107,   193,   108,     0,
   109,   767,  1006,   114,   154,   729,   730,     0,   921,   919,
     0,    75,     0,   993,   702,   488,   991,   481,     0,   487,
   492,   482,   483,   490,   484,   485,   486,   491,   328,     0,
   261,    31,     0,    40,   766,    39,     0,     0,     0,     0,
     0,     0,   772,     0,     0,     0,     0,     0,     0,     0,
   569,   926,     0,   907,   631,   631,   632,   644,   642,   979,
   645,     0,   646,   636,   632,     0,     0,     0,     0,     0,
     0,   596,   589,   578,   579,   581,   583,   584,   585,   592,
   593,   586,     0,   685,  1001,   116,   259,   117,   193,   118,
   767,   566,   620,     0,   616,     0,     0,   981,     0,     0,
   623,     0,     0,     0,     0,   406,   468,   371,     0,   371,
     0,   211,   407,   411,   402,   465,   466,     0,   261,   232,
   457,     0,   403,   420,     0,   377,   378,   477,   576,   574,
   917,   156,   156,     0,     0,     0,   168,   160,   161,   140,
     0,   104,     0,     0,   119,   261,   129,   128,   131,     0,
     0,     0,     0,     0,   984,    76,   211,     0,     0,   704,
   744,   489,     0,     0,     0,     0,   884,   913,     0,   983,
     0,   886,     0,     0,   914,     0,   908,   899,   898,   897,
  1008,     0,     0,     0,   889,   890,   891,   900,   901,   909,
   910,  1012,   911,   912,     0,  1017,  1013,   915,  1026,     0,
     0,     0,     0,   867,   360,   862,   776,   893,   887,   863,
   326,   902,   865,   866,   881,   882,   883,   903,   864,   927,
   928,   929,   943,   930,   945,   870,   868,   869,   953,   954,
   958,   959,   888,     0,     0,   988,     0,     0,     0,   364,
   209,     0,     0,     0,   769,     0,     0,    28,    34,     0,
     0,    44,    70,    72,  1004,    68,     0,     0,     0,   771,
     0,    53,     0,     0,     0,     0,    52,   568,     0,     0,
     0,   641,   639,     0,   648,     0,   647,   649,   654,   635,
     0,   615,     0,   590,   601,   587,   601,   588,   602,   608,
   612,   613,   597,   582,   702,   750,   780,     0,     0,   261,
     0,     0,   565,   564,     0,   617,   621,   724,   726,   731,
   628,   627,   736,   176,     0,   470,   137,     0,   473,     0,
   474,   469,     0,   415,     0,   417,   416,     0,   418,     0,
   412,     0,     0,   262,     0,     0,   424,     0,   425,   371,
   458,     0,   464,     0,     0,     0,   461,   988,   382,   381,
   476,     0,   399,   157,     0,     0,   166,     0,   165,     0,
     0,   155,   169,   170,   172,   162,   524,   536,   511,   546,
   514,   537,   547,     0,   553,   515,     0,   543,     0,   538,
   550,   510,   533,   512,   513,   508,   371,   493,   494,   495,
   503,   509,   496,   505,   526,     0,   531,   528,   506,   543,
   507,   120,   259,   121,     0,     0,   132,     0,     0,   192,
   232,   379,   369,     0,   211,     0,    82,    84,     0,     0,
   102,   101,   985,     0,     0,     0,     0,   853,   854,   855,
     0,   701,   784,   793,   796,   802,   797,   798,   799,   800,
   804,   805,   803,   806,   801,   782,     0,   710,   725,   980,
     0,   980,   904,     0,     0,   980,   980,   980,     0,   905,
   906,     0,     0,   969,     0,     0,     0,     0,     0,   872,
   871,     0,   725,   893,     0,   325,  1005,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   267,   503,   526,
     0,     0,   362,     0,   272,     0,   768,    29,     0,     0,
    45,    73,     0,     0,    27,   758,     0,   774,     0,   740,
   758,     0,     0,     0,     0,   567,     0,   631,     0,     0,
   702,     0,   653,     0,   558,     0,   557,   924,     0,   591,
     0,   599,   595,   594,     0,   605,     0,   610,   598,     0,
   704,   749,     0,     0,     0,   232,   369,   619,     0,   735,
     0,     0,   138,   177,   472,   371,   414,     0,   371,   372,
   419,   369,     0,   258,   369,   231,   989,   203,   200,     0,
   213,   371,   493,   369,     0,     0,     0,   230,   233,   234,
   235,   236,     0,   423,   463,   462,   422,   421,     0,     0,
     0,   385,   383,   384,   380,     0,   371,   186,   163,   167,
     0,     0,   171,     0,     0,   521,   761,   534,   535,   551,
     0,   548,   553,     0,   541,   539,   540,   143,     0,     0,
     0,   504,     0,     0,     0,     0,   542,   261,     0,   369,
   133,   191,     0,   194,   196,   197,     0,     0,     0,     0,
   211,   211,    79,     0,    87,    78,     0,     0,     0,     0,
   795,   783,     0,     0,   856,   857,   858,     0,   787,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   706,   708,
   709,   703,     0,   738,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   971,   972,
   973,   974,   975,   976,   977,   978,     0,     0,     0,     0,
   964,     0,     0,     0,     0,     0,     0,   880,   738,   879,
   327,     0,   875,   987,   874,   873,   876,   877,   878,   894,
   951,     0,   885,   778,   777,   366,   365,     0,   265,   280,
     0,   271,   273,   770,     0,     0,     0,     0,     0,     0,
    25,    54,    42,    62,    64,     0,     0,    71,     0,   763,
     0,   773,     0,   763,    50,    48,     0,     0,     0,   640,
   643,     0,   655,   656,   659,   661,   658,   670,   671,   672,
   660,   683,     0,   695,   650,   652,   997,   559,   925,   600,
   609,   604,   603,     0,   614,   751,   725,   752,   781,   369,
     0,     0,   727,   633,   728,   737,   471,   156,   183,   175,
   178,   179,     0,     0,     0,   413,   369,   369,   372,   270,
   263,   272,   202,   371,     0,   372,   204,   199,   205,     0,
     0,   241,     0,   427,   372,   371,   369,   426,     0,     0,
     0,   398,   479,   187,   188,   164,     0,   174,   522,   523,
   923,     0,     0,     0,   526,     0,   549,     0,     0,   371,
   372,     0,   499,   501,     0,     0,     0,     0,     0,     0,
     0,   379,   272,   156,   135,   232,   189,   399,   374,     0,
    83,     0,   211,     0,    91,     0,    85,     0,     0,    90,
    86,     0,   745,   990,     0,     0,   788,     0,     0,   794,
     0,     0,   786,   785,     0,   843,     0,   832,   831,     0,
   810,     0,   841,   859,     0,   851,     0,     0,     0,   811,
   837,     0,     0,   814,     0,     0,     0,     0,     0,     0,
     0,   812,     0,     0,   813,     0,   845,     0,     0,   807,
     0,     0,   808,     0,     0,   809,   707,   710,   743,     0,
     0,   211,     0,   931,     0,     0,   946,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   970,     0,     0,   968,
     0,     0,   960,     0,     0,     0,     0,   625,   892,   950,
     0,   260,   279,     0,   338,   361,   274,    66,    58,    59,
    65,    60,    61,     0,     0,     0,    23,    26,    42,    43,
   524,   524,     0,   755,   775,     0,   754,    51,    49,    46,
     0,     0,   691,   658,     0,   693,   687,   693,   691,   693,
     0,     0,     0,   681,     0,   700,   606,   607,   611,   738,
   272,   190,   374,     0,     0,   184,     0,   180,     0,     0,
     0,   409,   219,   214,   895,   215,   216,   217,   218,     0,
     0,   264,     0,     0,   212,   220,     0,   241,   731,   237,
     0,   428,   459,   460,   390,   391,   387,   386,   389,   388,
   371,   478,   372,   173,   986,     0,     0,   517,     0,   516,
   552,     0,   544,     0,   144,     0,   497,     0,   525,   526,
   532,   530,     0,   527,   369,     0,     0,     0,   127,   136,
   195,   371,     0,   370,   105,     0,   125,   124,    94,     0,
     0,    95,     0,    88,    89,     0,   849,   850,   790,   789,
   792,   791,     0,     0,     0,     0,   725,     0,     0,     0,
   852,     0,     0,     0,     0,     0,     0,   844,   842,   838,
     0,   846,     0,     0,     0,     0,   847,     0,     0,     0,
     0,     0,     0,   714,     0,   711,     0,   715,   937,   936,
     0,     0,   933,   932,     0,   941,   940,   939,   938,   935,
   934,     0,   949,   966,   965,     0,     0,     0,     0,     0,
     0,     0,   266,     0,     0,   276,   277,     0,   988,     0,
     0,     0,     0,     0,     0,     0,  1010,  1015,     0,     0,
   359,   290,   285,     0,   338,   284,   294,   295,   296,   297,
   298,   305,   306,   299,   301,   302,   304,     0,   309,     0,
   353,   355,   354,     0,     0,     0,     0,     0,     0,   753,
     0,   988,    55,    56,     0,    57,    63,    24,   759,     0,
   760,   757,   762,   765,   764,   739,     0,    47,   654,   662,
   657,   692,   690,   688,     0,   689,     0,     0,     0,     0,
     0,     0,   704,   748,     0,   370,   732,   733,   634,   186,
   185,   181,     0,   372,   896,   268,   269,   372,   372,   207,
   221,   222,   371,     0,   238,     0,     0,     0,     0,     0,
     0,     0,     0,   272,   475,   519,     0,   520,     0,   142,
     0,   207,   145,   146,   148,   149,   150,   500,   502,   498,
   529,   272,   399,   372,   186,     0,     0,   367,   371,    92,
     0,     0,    98,    93,   526,     0,   835,     0,     0,   861,
   738,   860,   833,     0,     0,     0,   839,     0,     0,     0,
     0,   848,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   705,   710,     0,   717,     0,   713,   716,   952,
   210,   942,   944,     0,     0,   967,   962,   961,     0,   955,
   957,     0,   213,   275,     0,   314,     0,     0,     0,   308,
     0,   323,   307,   339,   340,     0,   998,   356,   357,     0,
   286,     0,   291,   288,   344,     0,     0,   310,     0,   300,
   293,   303,   292,   311,   337,    41,   524,   665,     0,   673,
   691,   682,   680,   694,   696,   698,   699,   630,   372,   368,
   734,   182,   372,   152,   257,     0,     0,   198,   223,     0,
     0,     0,   241,   224,   227,   767,     0,   372,   396,   397,
   393,   392,   395,   394,     0,   518,   545,   151,   139,   147,
     0,   371,   254,   134,   372,   373,     0,     0,    96,    97,
    80,     0,   818,   826,   624,   834,   819,   827,   822,   830,
   836,   840,   820,   828,   821,   829,   815,   823,   816,   824,
   817,   825,   712,     0,   723,   948,   947,   963,     0,     0,
   282,   278,     0,   312,     0,   328,     0,     0,     0,   341,
     0,   352,     0,     0,     0,     0,   347,   345,     0,     0,
     0,   332,     0,   333,     0,   756,   667,   664,   674,     0,
     0,   675,   676,   677,     0,   255,   410,   201,   206,   225,
   241,   767,   228,   247,   240,   242,   400,   372,     0,   375,
   372,    77,    99,   100,     0,   720,     0,   719,   956,   281,
     0,   313,   331,     0,   316,     0,   322,     0,   351,   350,
   994,   349,   338,     0,     0,     0,     0,   324,     0,     0,
     0,   663,   678,     0,   697,   229,   247,     0,   241,   243,
   244,   256,   372,     0,   718,     0,     0,     0,     0,     0,
     0,   338,   358,   346,   348,     0,   343,     0,   335,   334,
     0,   668,     0,   241,     0,     0,   239,     0,   246,     0,
   245,   376,   126,   721,     0,   283,     0,   329,   315,   319,
   321,     0,     0,     0,   338,     0,   666,   679,   226,     0,
     0,   250,   249,   248,   722,     0,   338,     0,     0,   338,
   336,   669,   253,   251,   252,   330,   320,   338,   342,   317,
   318,     0,     0,     0
};

static const short yydefgoto[] = {  1892,
    20,    21,   487,   129,   130,   131,   132,  1237,   745,   740,
    22,   144,   991,   992,   993,   994,   133,   134,   492,   493,
    23,    84,   396,   885,   656,   657,   658,  1131,  1124,  1125,
  1564,   252,   660,    24,    76,    25,   162,    26,   385,    77,
  1345,   234,   389,   648,  1115,  1339,   556,   222,   381,  1101,
  1542,  1543,  1544,  1545,  1546,  1547,   228,   244,   219,   595,
   220,   377,   378,   379,   602,   603,   604,   557,   558,  1050,
  1051,  1052,  1277,  1086,   237,   328,   391,   873,   874,   875,
   809,   810,   811,  1668,   578,   477,  1401,   659,  1066,  1282,
  1520,  1521,  1522,  1674,   876,   580,   818,   819,   820,   821,
  1300,   822,  1819,  1820,  1821,  1873,   232,   326,   642,   207,
   359,   483,   573,   574,   978,   478,  1059,   981,   982,   983,
  1425,  1426,  1224,  1732,  1427,  1442,  1443,  1444,  1445,  1446,
  1447,  1448,  1449,  1450,  1451,  1452,  1453,  1454,  1455,  1739,
  1456,   444,   445,  1830,  1753,  1457,  1458,  1459,  1864,  1634,
  1635,  1746,  1747,  1460,  1461,  1462,  1463,  1630,    27,    28,
   271,   479,   480,   241,   330,   878,  1558,   565,  1056,  1344,
   239,   644,   368,   591,   592,   835,   837,  1312,    29,    61,
   800,   797,   353,   354,   363,   364,   199,   200,   365,  1075,
   586,   827,   204,   172,   346,   347,   212,   593,  1313,    30,
    98,   627,   628,   629,  1102,  1103,   630,   729,   632,   633,
  1095,  1479,   862,   634,   730,   636,   637,   638,   639,   855,
   640,   641,   852,    31,    32,    33,    34,    35,   544,    36,
   509,    37,    38,    63,   216,   370,    39,   314,   315,   316,
   317,   318,   319,   320,   321,   773,   322,  1034,  1269,   529,
   530,   778,   531,    40,  1464,   168,   546,   335,    42,  1366,
   712,    43,    44,    45,   512,   304,  1274,  1661,   149,   150,
   151,   302,   516,   517,   518,   764,   761,  1013,  1014,  1015,
  1016,  1757,  1812,  1841,  1017,  1018,  1019,  1020,  1762,  1763,
  1764,  1021,  1264,   155,   488,  1262,  1493,  1266,  1654,  1655,
  1503,   400,   689,  1394,   923,  1395,  1396,  1397,  1598,  1785,
  1788,   171,   548,   549,   248,  1509,  1045,   341,  1465,  1466,
    48,    49,   401,  1467,    51,    52,   535,  1468,  1469,    53,
  1000,  1480,  1244,  1483,   274,   275,   484,   499,   500,   747,
   446,   447,   538,   672,   898,   673,   674,   675,   676,  1150,
   677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
   900,  1153,  1154,   687,   449,   450,   451,  1284,   452,   453,
   454,   455,   456,   457,   458,   847,    81,   250,   236,  1093,
  1104,   768,   459,   460,   461,   462,   463,   464,   465,  1605,
   466,   467,   468,   469,   470,   471,   954,   472,   705,  1211,
   706,  1208,   947,   303,   339,   473,   474,   662,  1314,   963,
   225,   806,  1133,   256,   202,   253,  1800,   229,  1786,  1026,
  1471,  1022,   135,   475,   211,   213,   995,   716,   242,   163,
   476,   137
};

static const short yypact[] = {  2060,
   674,    30,  1728,   297,   861,   186,  1446,   362,  2243,   476,
   473,  1322,    30,   696,   126,  5632,   620,  5632,   345,   361,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   619,    95,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  7464,  5632,  5632,  5632,  5632,  5632,
-32768,-32768,   536,   732,  5632,  5632,  5632,   680,  5632,   543,
  5632,  5632,-32768,  5632,  5632,-32768,   501,-32768,   551,-32768,
-32768,   728,  5632,-32768,  5632,  5632,  5632,   763,  5632,  5632,
  5632,  5632,   543,  5632,  5632,  5632,  5632,-32768,  5632,   571,
   708,-32768,-32768,-32768,   584,-32768,   584,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   745,   600,
   761,-32768,    28,-32768,-32768,-32768,-32768,  5632,  5632,  5632,
   791,   829,   832,   651,    65,   785,   441,   455,   691,   736,
-32768,  5632,   896,   878,   833,-32768,  5632,-32768,  5632,  5632,
  5632,-32768,-32768,-32768,  5632,-32768,   813,   820,   978,   718,
   742,   839,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   306,-32768,
-32768,   732,-32768,-32768,   212,-32768,-32768,   762,-32768,   709,
-32768,-32768,   516,-32768,   853,-32768,-32768,   858,-32768,   807,
-32768,-32768,   995,-32768,-32768,-32768,   732,-32768,-32768,-32768,
   363,-32768,   762,-32768,-32768,   190,-32768,   973,-32768,   981,
-32768,   584,-32768,-32768,-32768,-32768,-32768,   984,-32768,-32768,
   870,-32768,   993,-32768,   884,-32768,-32768,-32768,   870,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6579,  5632,
   931,-32768,  5632,-32768,-32768,-32768,  4482,   541,   943,  3112,
  5632,  1010,   865,  1068,  1082,  4482,  1003,  1029,  1032,  3112,
   918,-32768,  6798,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  5046,-32768,-32768,-32768,   991,  5632,  1064,   988,   472,
  5632,  1011,-32768,-32768,   878,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  5632,-32768,-32768,-32768,   762,-32768,   973,-32768,
   584,  1066,  1074,   970,-32768,  6798,  1065,-32768,  1065,  6798,
-32768,    70,   974,   977,   980,   839,-32768,  1210,   784,-32768,
  5632,-32768,   306,-32768,-32768,-32768,-32768,  5632,   931,  2592,
  1034,  2780,   971,-32768,  7464,-32768,-32768,   824,-32768,-32768,
-32768,   992,   992,   732,  1133,   732,   170,   807,-32768,-32768,
  3502,-32768,  5632,  5632,-32768,   931,-32768,-32768,  1225,   464,
   982,  5632,  1251,  5632,-32768,-32768,   308,  5577,  4170,  1138,
-32768,-32768,  1001,  1004,  1005,  1069,-32768,-32768,  1008,-32768,
  1017,-32768,  1035,  1048,-32768,  1050,-32768,-32768,-32768,-32768,
  1052,  1053,  1103,  1108,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1054,-32768,-32768,  5265,  1057,  1058,-32768,  1072,  6798,
  6798,  5484,  5632,-32768,-32768,  1033,-32768,  1140,-32768,-32768,
  1071,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1073,  1051,  1063,  3502,  1078,   237,-32768,
-32768,  1077,  1337,   587,-32768,  5632,  1177,-32768,-32768,  5632,
  5632,    33,-32768,-32768,-32768,-32768,  5632,  5632,   589,-32768,
  5632,-32768,  1289,  5632,  4482,  1212,  1083,-32768,  5632,  5484,
   376,-32768,-32768,  6798,-32768,  1293,  1089,-32768,   217,-32768,
   585,-32768,   104,-32768,   554,-32768,   554,-32768,  1091,-32768,
    88,-32768,  1196,-32768,    19,-32768,-32768,  1100,  1087,   931,
  1096,  1362,-32768,-32768,  5632,-32768,-32768,  1101,-32768,   407,
-32768,-32768,   968,-32768,  1284,  1285,-32768,   732,-32768,  1286,
-32768,-32768,  1355,-32768,  1357,-32768,-32768,  1338,-32768,  3502,
-32768,  3502,   632,-32768,  1380,  5632,-32768,  7325,-32768,   454,
-32768,  5632,   692,   709,  1217,  1185,  1258,  1222,-32768,-32768,
-32768,   187,  1262,-32768,  1203,   543,-32768,   732,-32768,  1379,
  1314,-32768,   170,-32768,-32768,-32768,   368,  1220,-32768,-32768,
-32768,  1235,-32768,  1290,  1149,-32768,  1333,  1153,   197,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1151,-32768,-32768,-32768,   146,  1160,  1161,   305,-32768,  1153,
-32768,-32768,   762,-32768,  1374,  1428,-32768,   732,   732,-32768,
  2592,   516,-32768,  1323,-32768,  1291,  1178,-32768,  3297,  1321,
-32768,-32768,-32768,  1330,  1179,  4389,  1180,   891,   933,  1147,
  3925,   458,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  1644,   491,  1328,   727,
  6798,   117,-32768,  5632,  6798,   794,   806,   812,  1384,-32768,
-32768,  6798,  4608,  1140,   123,  1266,  6798,  6798,  4608,   147,
   147,  1183,    95,   432,   695,-32768,-32768,  6798,  5632,  6798,
  6798,  6798,  6798,  6798,  6798,  5703,  2840,-32768,-32768,  1437,
  1181,  5632,-32768,  5632,  1417,  5632,-32768,-32768,  3593,  1294,
-32768,-32768,  1268,  3112,-32768,  1414,   710,-32768,  1202,-32768,
  1414,  3593,  1416,  1429,  5632,-32768,   432,-32768,   556,  4975,
  1305,  6798,-32768,  5632,-32768,    47,-32768,-32768,   164,-32768,
  1312,-32768,-32768,-32768,  5632,    99,  5632,-32768,-32768,  5632,
  1138,-32768,  6798,  5632,  1489,  2592,-32768,  1228,  6798,  1264,
  6798,   732,-32768,   621,-32768,-32768,-32768,  1234,-32768,-32768,
-32768,   189,  5632,-32768,-32768,-32768,-32768,  1490,-32768,  1238,
  1465,-32768,  1485,-32768,  1427,  1430,   584,-32768,-32768,-32768,
-32768,-32768,  1495,-32768,-32768,-32768,-32768,-32768,  5632,  7406,
   543,  1393,  1394,  1396,-32768,   543,-32768,   841,-32768,-32768,
  1366,   732,-32768,  5577,   401,  1369,-32768,-32768,-32768,-32768,
   543,-32768,  1149,   444,-32768,-32768,-32768,  1481,   444,  1370,
   444,-32768,   543,   543,  1254,   543,-32768,   931,  5632,-32768,
  1434,-32768,   711,-32768,-32768,-32768,   824,  1065,  5632,   748,
    84,-32768,-32768,  1260,   450,-32768,  5577,  5632,  1065,  4827,
-32768,-32768,  1065,  4389,-32768,-32768,-32768,   113,-32768,   475,
  1216,  4608,  4608,  6798,  6798,  1822,  1263,   623,  2112,  6798,
  2456,   511,  2706,  2956,  5922,  3206,  3456,  3706,-32768,-32768,
  1426,-32768,  1267,   742,  6798,  6798,   255,  6798,  1269,  6798,
  1270,   595,  6798,  6798,  6798,  6798,  6798,  6798,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  1472,  1090,   340,  6798,
-32768,  4608,  6798,   420,   281,   854,   109,-32768,   742,-32768,
-32768,   594,-32768,-32768,  1523,   147,   147,   603,   603,  1140,
-32768,   773,-32768,-32768,-32768,-32768,-32768,   788,-32768,  1363,
  1535,  1417,-32768,-32768,  5632,  5632,  5632,  5632,  5632,  5632,
  1281,-32768,    58,-32768,-32768,  3593,  1440,-32768,   132,  1447,
  5632,-32768,  6798,  1447,  1281,  1295,  3593,  3799,  1498,-32768,
-32768,  4044,  1301,  1318,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1306,  4608,  1503,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    81,-32768,-32768,  1328,  1140,-32768,-32768,
   805,  1065,-32768,-32768,-32768,  1140,-32768,   992,  1554,-32768,
   621,-32768,  1307,  4608,  3065,-32768,-32768,-32768,-32768,-32768,
-32768,  1417,-32768,-32768,  3065,-32768,-32768,-32768,-32768,   584,
   584,  1347,  1316,-32768,-32768,-32768,-32768,-32768,   119,   140,
   202,-32768,  1580,-32768,-32768,-32768,  5632,-32768,-32768,-32768,
-32768,   297,   809,  1431,  1437,  1332,-32768,  1203,   814,-32768,
-32768,   609,-32768,  1317,  5632,   641,  1334,  1336,   543,  1339,
  1592,   516,  1417,   992,  1285,  2592,-32768,  1262,  1412,  1925,
-32768,   543,  1474,  1559,-32768,  3297,-32768,   543,   429,-32768,
-32768,  1560,-32768,-32768,  1340,  1341,-32768,  4608,  4608,-32768,
  4608,  4608,-32768,  1608,   169,  1140,  1344,-32768,-32768,  1346,
  1140,  5484,-32768,-32768,  1557,-32768,   182,  1350,  1351,  1140,
  1117,  1352,  1353,  1140,  6798,  6798,  1263,  6798,  6141,  1354,
  1356,  1140,  1359,  1361,  1140,  6798,  1140,  1365,  1368,  1140,
  1371,  1372,  1140,  1375,  1376,  1140,-32768,  4275,-32768,   606,
   624,-32768,   633,-32768,   642,  6798,-32768,   656,   666,   675,
   759,   990,  1002,  6798,  6798,  6798,  1140,  1579,   354,  1140,
  1458,  6798,-32768,  6798,  6798,  6798,  6798,-32768,-32768,-32768,
  5632,-32768,-32768,  5632,  7035,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  3593,  1565,  3593,-32768,  1281,    58,-32768,
   512,   512,    43,-32768,-32768,   817,-32768,  1281,  1295,  1364,
  3799,  1373,  1342,  1381,  4975,  1516,-32768,  1516,   553,  1516,
  1568,  1571,  6798,  2331,  1631,  1586,-32768,-32768,-32768,   742,
  1417,-32768,  1412,   610,  1203,  1532,  1203,-32768,  4608,   127,
  1056,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  3065,
  3065,-32768,  1535,  6798,-32768,  1358,  1520,  1347,   501,-32768,
  4608,-32768,-32768,-32768,  1541,  1544,  1545,  1548,  1549,  1551,
-32768,-32768,-32768,-32768,-32768,  1404,   297,-32768,  1483,-32768,
-32768,   444,-32768,  3065,   211,   444,-32768,   444,-32768,  1437,
-32768,-32768,  1408,-32768,-32768,   824,  1535,  1203,-32768,  1285,
-32768,-32768,  1661,-32768,-32768,  1411,-32768,  1228,-32768,  1413,
   732,   150,  1415,-32768,-32768,   732,-32768,-32768,-32768,  1608,
-32768,  1608,  6798,  1065,  1065,  1419,    95,   838,  6798,  1622,
-32768,  1065,  1065,  6798,  1065,  1065,   175,  1140,-32768,  1195,
  6798,  1140,  1065,  1065,  1065,  1065,  1140,  1065,  1065,  1065,
  1065,  1065,  1065,-32768,  1420,  1418,   579,  5632,-32768,-32768,
  1421,  3502,-32768,-32768,  1018,-32768,-32768,-32768,-32768,-32768,
-32768,  1027,  1261,-32768,-32768,  6798,  6798,  1635,  1515,  1036,
   867,   924,-32768,  3502,  1425,-32768,-32768,  1655,  1662,  5139,
    71,  1432,  1424,  1617,  6798,  1433,  1435,  5358,  5632,  5632,
-32768,-32768,-32768,  1649,  6900,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   235,-32768,  1436,
-32768,-32768,-32768,  1619,  1438,  1441,  1443,  1445,  1449,-32768,
  1442,   323,-32768,-32768,  1584,-32768,-32768,-32768,-32768,  1547,
-32768,-32768,-32768,-32768,-32768,-32768,  3799,  1364,  1708,-32768,
  1318,-32768,-32768,-32768,  1638,-32768,  4975,  4975,   876,  5632,
  6798,  4608,  1138,-32768,  1535,-32768,-32768,-32768,-32768,   841,
-32768,-32768,   131,-32768,-32768,-32768,-32768,-32768,  1140,  1701,
  1358,-32768,  1007,  5632,-32768,  1650,   134,  1550,  1651,  1552,
  1693,  1660,  1697,  1417,-32768,-32768,  1468,-32768,  1470,-32768,
  1652,  1701,   211,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1417,  1262,-32768,   841,  1580,  1624,-32768,-32768,-32768,
  1639,   272,-32768,-32768,  1437,  1647,  1140,  1487,  1492,-32768,
   742,-32768,  1140,  6798,  1493,  1494,  1140,  1497,  1499,  6798,
  6798,  1140,  1500,  1501,  1504,  1505,  1506,  1507,  1508,  1510,
  1511,  1512,-32768,  4275,  1496,-32768,  5632,-32768,-32768,-32768,
-32768,-32768,-32768,  6798,  1513,-32768,-32768,-32768,  6798,-32768,
-32768,  6798,   192,-32768,  1711,-32768,  6360,  5632,  6798,-32768,
  4608,  1140,-32768,-32768,-32768,  1518,-32768,-32768,-32768,  5632,
-32768,   242,-32768,  1588,-32768,   160,  1517,-32768,  4920,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   512,  5632,  4975,-32768,
  1198,-32768,-32768,  1519,-32768,  1140,   458,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  1521,  5632,-32768,-32768,  1703,
  1712,  5632,  1347,-32768,-32768,   584,  5632,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  1535,-32768,-32768,-32768,-32768,-32768,
  1535,-32768,-32768,-32768,-32768,-32768,  6798,   732,-32768,  1595,
-32768,   732,-32768,-32768,-32768,  1140,-32768,-32768,-32768,-32768,
  1140,  1140,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  5632,  1721,  1140,-32768,-32768,  1070,  3065,
-32768,-32768,  1530,-32768,   937,  6579,   262,   137,  1533,-32768,
  1724,-32768,  5632,  5632,   297,   138,-32768,-32768,  1561,  1726,
  4608,-32768,   767,-32768,  1531,-32768,  1537,-32768,-32768,  4608,
  1539,-32768,-32768,-32768,  6798,-32768,-32768,-32768,-32768,-32768,
  1347,   584,-32768,  1691,-32768,-32768,-32768,-32768,  1580,-32768,
  1140,-32768,-32768,-32768,  1542,  1543,  1546,-32768,-32768,-32768,
  1065,-32768,  1676,  4920,-32768,  1653,-32768,  4920,-32768,-32768,
-32768,-32768,  7143,   242,  6798,  4920,   149,-32768,  4920,  5796,
  5632,-32768,   458,  5632,-32768,-32768,  1691,   204,  1347,  1705,
  1707,-32768,-32768,  1556,-32768,  5632,  5632,  1558,  4701,  1563,
   843,  7143,  1553,-32768,-32768,  1288,   128,  1790,-32768,-32768,
   900,-32768,   910,  1347,   424,   424,-32768,  1794,-32768,  1659,
-32768,-32768,-32768,-32768,  1566,-32768,  4920,  1553,-32768,-32768,
  1791,  4920,  1807,  1798,  7143,  5632,-32768,-32768,-32768,  1645,
   467,-32768,-32768,-32768,-32768,   916,  7143,   161,  5632,  7143,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,  7143,-32768,-32768,
-32768,  1847,  1848,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,   -64,   179,-32768,   251,  1572,   612,-32768,  -379,
-32768,-32768,  -625,   -17,  -649, -1081,  1837,  1574,  1562,  1114,
-32768,-32768,-32768,   733,-32768,  1207,   983,-32768,-32768,   740,
-32768,-32768,   979,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   749,-32768,-32768,-32768,
-32768,-32768,   324,-32768,-32768,  1524,-32768,-32768,-32768,  -298,
-32768,-32768,-32768,  1502,-32768,-32768,  1271,  -350,-32768,-32768,
-32768,   822,-32768,  -907,-32768,-32768,  1555,  1092,   760,  1522,
  1062,   757,-32768,   339,   -51,  -258,-32768,  -324,   270,  -998,
-32768,-32768,   365,-32768,  1527, -1118,-32768,-32768,-32768,-32768,
 -1253,   366,    73,    72,    74,    46,-32768,-32768,-32768,-32768,
  -117,  -253,-32768,  1093,-32768,  -662,-32768,  -947,-32768,   913,
-32768,-32768,-32768,-32768,-32768, -1325,  -928,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768, -1190,   162,-32768,  -775,-32768,-32768,-32768,-32768,-32768,
   263,-32768,    97,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  1167,-32768,-32768,  -626,   396,  -338,  -894,   630,
-32768,-32768,  -541,  -764,-32768,-32768, -1006, -1409,-32768,-32768,
-32768,-32768,-32768,  1564,-32768,  1343,  1540,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  1575,-32768,-32768,-32768,-32768,
-32768,  -548,  -438,-32768,  1045,   596,  -564,  -359,-32768,-32768,
-32768,  1313, -1027,-32768,  -358,-32768,-32768,-32768,-32768,  1283,
-32768,-32768,  1076,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1911,-32768,-32768,-32768,-32768,-32768,  1610,
-32768,-32768,-32768,  1403,-32768,  1406,-32768,-32768,-32768,-32768,
  1155,-32768,-32768,-32768,     1,-32768,-32768,-32768,  -430,  1042,
-32768,  -256,   887,-32768,  -188,   200,-32768,-32768,-32768,-32768,
  1781,-32768,-32768,-32768,  1176, -1068,-32768,-32768,  -942, -1286,
-32768,-32768,-32768,-32768,   927,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1858,    20,   694,   -29,-32768,-32768,   185,
-32768,  -389,  -747,  1272,-32768,   357,-32768,   557,-32768,  -889,
-32768,  -591,-32768,  1165,  -509,-32768,-32768,  -823,  1958,  1962,
-32768,-32768,  1439,  1963,-32768,-32768,-32768,  -205,-32768,-32768,
  1219, -1132,   967,-32768,  -101,  -680,  -532,-32768,-32768,-32768,
  -792,   -44,  -427,  -666,-32768,  -466,-32768,-32768,-32768,   530,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -465,
-32768,   808,-32768,  1031,  -963,-32768,  -407,-32768,   697,  -958,
  -137,  -956,  -951,  -911,   -38,    -2,   -53,-32768,   -61,  -637,
  -490,  -128,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
 -1043,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   758,
-32768, -1019,-32768,   405,-32768,  -897,   403,  1129,   871,   310,
  -711,  1397, -1062,-32768,   -62,  1891,-32768,   -26,   -49, -1597,
   544,   -40,   -42,  -152,    -1,   -34,  -236,-32768,   -72,   -41,
    -9,-32768
};


#define	YYLAST		7727


static const short yytable[] = {   136,
    41,    80,   136,   227,   324,   276,   158,   205,   164,   295,
   298,   568,   788,   223,   157,   975,   235,   208,   296,   299,
   224,   631,   635,   267,   258,   221,   605,   570,   233,   812,
   767,   264,   245,  1037,   715,   156,   949,   240,   728,   235,
   790,   262,   957,   494,  1525,   201,   203,   206,   209,   158,
   214,   263,  1226,   494,   268,   226,   203,   230,   269,   209,
   266,   158,   214,   261,   243,   136,  1295,  1320,   750,  1253,
   748,   979,  1039,   254,   596,   158,   257,   203,   210,   254,
   230,   206,   209,   157,   158,   214,   243,   136,   331,   209,
   238,  1283,   265,   282,   886,   284,  1285,   924,  1286,   572,
  1189,  1283,  1006,  1287,   156,   575,  1285,   513,  1286,  1482,
   877,  1342,  1118,  1287,  1293,   386,   902,   536,   327,  1633,
  1138,   959,   300,   332,   753,   306,  1005,   290,   226,   158,
   226,   801,   646,   802,   902,  1218,  1072,  1863,   902,   813,
   393,   902,   230,  1288,   902,   781,  1695,   325,   776,   209,
   158,   243,  1474,  1288,  1477,   164,   902,  1289,   283,   554,
  1042,  1428,   928,   355,  1292,  1337,   860,  1289,  1562,   719,
   539,  1296,   950,   951,   720,  1060,  1363,  1523,  1062,   329,
  1302,  1470,  1580,  1307,  1803,   713,  1414,  1069,   382,   525,
   142,   719,  1418,   152,  1241,  1500,   720,   719,  1305,   891,
   892,   280,   720,   387,   899,   793,  1325,  1888,   399,   540,
  1650,  1618,   600,  1842,   356,  1749,  1122,   856,   631,   635,
   169,   503,    62,  1267,   743,   481,   763,  1370,   485,   542,
   832,  1057,  -371,   903,  1065,  1143,  1144,  1139,  1091,   719,
  1032,   823,   292,  1113,   720,  1309,   769,  1845,    85,  1235,
  1742,   903,   605,   713,   742,   903,  1033,   522,   903,   554,
   226,   903,   143,   226,  1192,   312,   833,   158,  1882,  1028,
   495,   136,   170,   903,  1268,  1371,   158,   719,   537,  -651,
   495,  1310,   720,   857,   719,  1209,   785,   357,   858,   720,
   780,  1516,  1517,   153,   388,  1636,  1743,   206,  1306,   281,
   154,   158,  1550,   719,   744,  1242,   569,  1744,   720,   348,
   962,   952,  1491,   481,    14,   224,  1619,   583,   972,  1308,
   587,   294,   288,  1505,   555,  1540,  1283,  1283,  1541,  1236,
   532,  1285,  1285,  1286,  1286,   597,   281,   599,  1287,  1287,
   527,   226,   643,  1794,   654,   539,  1239,   902,   226,   645,
   226,   650,   226,  1563,   349,   588,  1123,  1249,  1250,   777,
  1283,   902,  1759,  1099,  1518,  1285,   834,  1286,  1699,  1823,
  1238,  1024,  1287,   209,   214,  1009,  1628,  1629,  1288,  1288,
  1217,  1248,   158,  1846,   206,   929,  1140,  1280,   663,  1297,
  1298,  1076,  1289,  1289,   289,   661,  1606,  1607,   719,  1809,
  1514,  1470,  1523,   720,  1663,  1477,  1745,  1678,  1554,  1804,
  1796,   652,  1288,  1271,   713,   723,   724,   246,  1535,  1773,
  1648,   861,  1838,   891,   601,  1637,  1289,   891,  1137,   719,
  1290,  1291,  1809,   717,   720,   721,   722,   723,   724,  1347,
   754,   721,   722,   723,   724,  1270,  1504,  1119,  1752,   741,
  1304,   247,  1058,   537,   719,  1730,   -81,  1053,  1135,   720,
  1055,   350,  1136,   741,   903,   902,   838,   756,  1129,  1212,
  1213,  1359,  1360,  1067,  1361,  1362,   158,  1834,   903,   351,
   209,   495,  1141,   721,   722,   723,   724,   158,  -651,   865,
   140,   158,    78,   138,   209,   158,   514,   525,  1083,   164,
   485,  1700,    99,   383,   520,   738,  1861,   494,   732,  1884,
   733,   246,  1206,   352,  1756,   815,   746,  1816,   366,   794,
   751,   721,   722,   723,   724,   868,  1416,  1165,   721,   722,
   723,   724,  1795,   814,   839,   226,   844,  1701,   384,  1881,
  1870,  1166,   235,  1130,   572,   247,  1692,   721,   722,   723,
   724,  1887,  1215,   649,  1890,  1651,  1126,   139,  1423,   840,
  1885,   886,  1891,    78,    79,  1847,   807,  1741,   226,  1010,
  1336,  1553,   807,  1245,  1036,   919,  1659,   866,   719,  1871,
   655,  1252,   903,   720,   102,  1167,  1685,  1348,   165,  1728,
  1869,   367,   816,   976,   526,  1246,  1091,  -998,   920,  1142,
 -1001,  1488,  1662,  1752,  1691,  1485,   224,  1752,  1214,   871,
   872,  1273,  1513,  1168,  1111,  1752,  1256,   719,  1839,  1664,
   104,   100,   720,  1665,  1666,   719,  1872,   539,   719,  1257,
   720,   166,   817,   720,  1527,    79,   292,  1029,  1752,   292,
   845,   226,   721,   722,   723,   724,   719,  1694,   527,   758,
   292,   720,  1258,   397,   921,   719,   101,  1595,  1354,  1693,
   720,   402,   217,   218,   719,   528,  1752,   931,  1155,   720,
   771,  1752,  1092,   721,   722,   723,   724,  1169,   719,   167,
   844,  1538,   215,   720,   230,   105,   231,   481,   719,   481,
  1355,   984,  1596,   720,   102,   106,   300,   719,   721,   722,
   723,   724,   720,  1260,  1597,   960,   103,    78,  1552,   964,
    54,  1098,   360,   293,   741,   361,  1156,   226,  1048,   294,
   107,    55,   226,   337,   226,  1294,   226,   297,    56,   495,
   104,  1790,   300,   294,   495,   537,   301,  1303,    78,   772,
  1157,   224,   495,    14,  1368,   209,   249,  1705,  1228,  1275,
   209,  1231,    57,  1047,  1027,  1658,  1777,   362,   481,  1470,
   159,  1324,  1778,   338,  1766,   158,  1283,   158,  1767,  1078,
   749,  1285,   925,  1286,   226,  1571,   226,   235,  1287,    79,
   292,   719,  1082,  1776,  1074,  1077,   720,  1049,  1470,  1526,
   294,   160,   251,   226,   532,   105,  1035,  1096,  1126,   300,
  1780,  1107,  1108,  1088,  1110,   106,   161,   765,  1421,   235,
   235,   300,   235,   566,    58,  1338,  1889,   300,  1288,   226,
   226,  1470,   721,   722,   723,   724,   567,   259,   589,  1011,
   107,  1539,  1289,  1470,   663,  1657,  1470,  1549,   590,   933,
  1507,  1089,   342,   270,  1470,    59,  1754,   272,   539,    60,
  1508,   935,   766,  1601,   343,  1499,   273,   937,   736,   158,
   737,   721,   722,   723,   724,   725,   277,  1402,  1197,   158,
  1219,   278,   721,   722,   723,   724,   719,   663,  1134,  1399,
  1326,   720,   279,  1822,   661,  1327,  1824,   344,  1112,   345,
   721,   722,   723,   724,   825,  1367,   826,  1400,  1120,   721,
   722,   723,   724,   803,   372,   804,  1403, -1020,   721,   722,
   723,   724,  1326, -1020,   285,  1404,  1232,  1330, -1020,    82,
    83, -1020,   721,   722,   723,   724,   147,   148,  1852,  1406,
   217,   218,   721,   722,   723,   724,  1854,  1855,   373,  1407,
   374,   721,   722,   723,   724,  1229,   719,  1233,  1408, -1021,
   286,   720,  1230,   287,  1738, -1021,   537, -1020,   291,   719,
 -1021,   308,   375, -1021,   720, -1020,   725,   148,   961,  1029,
  1084,  1085,  1534, -1020,   307,   495,   209,   214,   495,   243,
   136,  1001,  1116,  1002,  1117,  1613,   495,   376,   323,   793,
   719, -1020,   333, -1020,   309,   720,   336,   495,   495, -1021,
   340,  1754,   209,  1556,   380,  1754, -1020, -1021, -1020, -1020,
 -1020,   334,   719,  1754,   369, -1021,  1840,   720,  1831,   882,
   310,  1121,  1833,   311,   719,   721,   722,   723,   724,   720,
  1837,   390,  1409, -1021,   358, -1021,  1754,  1808,  1809,   312,
   719,   392,   631,   635,   725,   720,  1220,  1333, -1021,   719,
 -1021, -1021, -1021,  1858,   720,   235,   371, -1020,   719,  1221,
  1349,  1222,   394,   720,  1754,   313,  1353,   395,   235,  1754,
   398,   224,   539,   399,   235,   485,  1116,  1315,  1272,   482,
  1317,  1876,  1318,   490,  1807,  1322,  1878,  1323,   725,  1316,
  1486,   497,   719,  1813,   926,  1315,   930,   720,   975, -1021,
   934,   936,   938,  1481,  1481,  1484,   226,  1367,  1367,   725,
   226,  1572,   719,  1860,  1809,  1367,  1367,   720,  1367,  1367,
   721,   722,   723,   724,  1670,  1216,  1367,  1367,  1367,  1367,
   501,  1367,  1367,  1367,  1367,  1367,  1367,   498,   725,   719,
  1611,   791,   502,   504,   720,  1671,  1510,   725,  1512,  1652,
   505,  1672,  1205,   506, -1020, -1020, -1020, -1020, -1020, -1020,
 -1020,   508,   719, -1022,   521,   523, -1020,   720, -1020, -1022,
  1374,  1866,  1424,  1867, -1022,   524,   481, -1022,   158,   224,
   537,   736,   533,  1868,   823,  1673,  1398,  1809,   543,  1886,
   721,   722,   723,   724,   545,  1612, -1021, -1021, -1021, -1021,
 -1021, -1021, -1021,   721,   722,   723,   724,  1792, -1021,  1555,
 -1021,   226,   547, -1022,  1429,  1472,  1473,   719,  1476,    14,
  1697, -1022,   720,   551,   495,   552,   495,   559,  1494, -1022,
  1496,   560,   904,   561,   721,   722,   723,   724,   719,   563,
   581,   495,   584,   720,   598,   209,   905, -1022,  1581, -1022,
   418,   419,   420,   647,   651,   594,   721,   722,   723,   724,
   653,  1256, -1022,  1410, -1022, -1022, -1022,   693,   721,   722,
   723,   724,   688,   690,  1257,  1411,   691,   692,   428,   429,
   694,  1843,   906,   719,   721,   722,   723,   724,   720,   695,
   907,  1602,   539,   721,   722,   723,   724,  1258,   908,   448,
  1603,   700,   721,   722,   723,   724,   701,   696,   718,  1610,
   719,  1259,  1561, -1022,  1537,   720,   909,  1566,   910,  1760,
   697,  1604,   698,   511,  -984,   699,   702,   101,   727,   707,
   708,   911,   519,   912,   913,   914,   721,   722,   723,   724,
 -1001,   731,   725,  1789,   709,   726,   735,  1653,  1260,   734,
   739,   752,   755,  1779,   744,   760,   721,   722,   723,   724,
   762,   779,   775,   783,   784,   102,   550,  1617,   786,  1862,
   553,   787,   789,   792,   554,   795,   796,   103,   798,  -232,
   799,  1256,   915,   721,   722,   723,   724,   576,   158,   805,
   829,   141,   830,   831,  1257,  -430,  1398,   836,   292,   841,
   537,   104,  -691,   842,   848,  1256,   721,   722,   723,   724,
 -1022, -1022, -1022, -1022, -1022, -1022, -1022,  1258,  1257,   849,
   203,   851, -1022,   853, -1022,   854,   859,   850,  1627,  1134,
  1134,  1259,   863,   864,   869,  1472,  1761,   870,  1159,   881,
  1163,  1258,  1171,  1174,   879,  1179,  1182,  1185,   887,   882,
   888,   889,   893,   169,   953,  1259,   958,   860,   980,   294,
  1261,   721,   722,   723,   724,   704,   105,   996,  1260,   997,
   710,   711,   714,   999,  1003,  -232,   106,   495,  1007,   916,
   917,   918,   721,   722,   723,   724,  1755,   209,   209,   960,
   325,  1008,  1260,    86,  1023,  1758,  -232,  1030,  1040,   736,
    87,   107,  -232,  1044,    88,    89,  1054,  1065,  1063,  1481,
  1064,   808,  1070,    90,   158,  1071,  1073,  1079,  1080,   108,
  1081,  1087,  1094,  1100,    91,  1105,  1109,   721,   722,   723,
   724,  1114,  1128,  1187,  1204,  1152,  -232,   109,   110,  1188,
   757,  1196,  1194,  1676,   759,   719,  1223,  1725,   111,   112,
  1225,   113,  1234,  1243,   721,   722,   723,   724,   114,   115,
  1251,   116,   117,  1240,   118,  1276,  1236,   119,   120,   121,
   122,   123,  1255,  1265,  1774,   124,   125,  1736,  1263,  1279,
  1261,   126,   127,   128,   158,  1299,    92,   206,  1301,  1311,
  1319,  1328,  1398,   939,   940,   941,   942,   943,   944,   945,
   946,  1335,    93,  1343,  1261,  1321,  1122,  1331,   209,  1332,
  1351,  1356,  1334,  1357,  1358,   902,  1364,    94,  1365,  1369,
  1134,    95,  1372,  1373,  1375,  1376,  1383,  1775,  1384,  1415,
  1417,  1385,    96,  1386,  1475,  1487,  1750,  1388,   325,   209,
  1389,  1755,  1492,  1390,  1391,  1755,  1489,  1392,  1393,  1501,
   539,    97,  1497,  1755,  1490,  1498,  1755,   964,  1502,  1782,
   904,  1511,   158,  1784,  1524,  1528,   719,   206,  1529,  1530,
  1817,   720,  1531,  1532,   905,  1533,  1755,  1536,  1091,   539,
  1799,  1551,  1557,  1559,  1574,  1608,  1560,  1609,  1565,  1594,
  1615,  1772,  1570,  1593,  1600,  1614,  1621,  -990,   139,  1631,
  1639,   901,  1620,  1623,  1755,  1624,  1638,  1646,  1640,  1755,
   906,  1641,   539,  1642,   206,  1643,  1645,   763,   907,  1644,
  1647,   927,  1649,  1667,   539,   932,   908,   539,  1677,  1679,
  1680,  1681,   948,   203,  1801,   539,  1682,   955,   956,  1683,
  1684,  1686,  1802,  1687,   909,  1688,   910,  1696,   448,  1698,
   965,   966,   967,   968,   969,   970,   448,  1702,   537,   911,
  1703,   912,   913,   914,    64,  1704,  1707,  1708,  1724,   485,
  1709,  1733,  1710,  1713,  1714,    65,  1632,  1715,  1716,  1717,
  1718,  1719,    66,  1720,  1721,  1722,  1727,   537,  1740,  1751,
  1765,  1828,   519,  1472,  1768,    67,  1770,  1771,  1783,  1787,
   226,  1027,  1791,  1797,   226,  1798,  1805,  1806,  1810,  1811,
   915,  1814,  1818,  1038,  1826,  1825,   206,   206,  1827,   550,
   537,  1046,  1472,  1829,  1809,  1832,  1848,  1147,  1850,  1853,
  1148,  1856,   537,  1859,   403,   537,  1865,  1845,  1846,  1875,
  1877,   404,  1879,   537,  1880,  1883,  1893,  1894,   145,   489,
  1478,   507,    68,   405,   496,  1472,  1027,   998,  1352,   406,
   407,   880,  1350,  1340,  1127,  1132,  1690,  1472,    69,  1134,
  1472,   564,  1278,   843,  1068,  1341,  1346,  1041,  1472,   606,
  1689,   577,  1731,   541,    70,  1669,   579,   408,  1675,  1844,
   409,  1874,  1851,  1849,  1227,  1061,  1748,  1793,   977,    71,
  1835,  1660,  1506,    72,   585,  1106,    73,   916,   917,   918,
   721,   722,   723,   724,    74,   410,   571,   411,   412,   846,
   562,  1548,   867,   146,   534,   770,   828,   413,  1097,  1031,
   305,   414,   774,    75,  1145,  1146,  1151,  1025,  1254,  1160,
  1161,  1164,   255,  1172,  1175,  1177,  1180,  1183,  1186,  1815,
  1723,   808,  1495,  1043,  1599,  1190,  1191,    46,  1193,   922,
  1195,    47,    50,  1198,  1199,  1200,  1201,  1202,  1203,  1004,
  1247,  1419,  1090,   782,  1379,  1329,  1769,  1515,   824,   260,
  1207,  1626,     0,  1210,  1149,     0,   415,     0,     0,     0,
     0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   417,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   418,   419,   420,   421,
   217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
   422,     0,     0,   448,     0,     0,     0,   109,   110,   423,
   424,   425,   426,   427,   428,   429,   430,   431,   111,   432,
     0,   113,     0,     0,   433,   434,     0,   435,   436,   437,
     0,   116,   117,   438,   118,     0,     1,   119,   120,   121,
   122,   123,     0,     0,     0,   124,   125,     0,     0,     0,
     0,   126,   439,   128,     2,     0,     0,     0,   440,   441,
     0,     0,     3,     0,   510,     0,   443,     0,     0,     4,
   294,     5,     0,     6,     0,     0,     0,     0,     7,     0,
     0,     0,     0,     0,     0,     8,     0,  1158,     0,     0,
  1148,     0,   108,     0,   403,     0,     0,     0,     0,     9,
     0,   404,     0,     0,     0,     0,     0,     0,     0,    10,
   109,   110,     0,   405,     0,     0,     0,     0,     0,   406,
   407,   111,   112,     0,   113,     0,     0,     0,     0,     0,
     0,   114,   115,     0,   116,   117,     0,   118,     0,     0,
   119,   120,   121,   122,   123,     0,     0,   408,   124,   125,
   409,     0,   448,     0,   126,   127,   128,     0,     0,     0,
     0,     0,     0,     0,     0,  1377,  1378,   273,  1380,  1382,
     0,     0,    11,     0,     0,   410,  1387,   411,   412,    12,
     0,     0,    13,     0,    14,    15,     0,   413,     0,     0,
     0,   414,     0,     0,     0,     0,  1405,     0,     0,     0,
     0,     0,     0,     0,  1412,  1413,  1207,     0,     0,    16,
     0,     0,  1207,     0,  1210,  1420,   448,  1422,   101,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1149,     0,   415,     0,     0,     0,
     0,     0,   416,     0,     0,     0,   102,     0,    17,     0,
     0,     0,   417,   448,     0,     0,     0,     0,   103,     0,
     0,     0,     0,    18,    19,     0,   418,   419,   420,   421,
   217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
   422,     0,   104,     0,  1519,     0,     0,   109,   110,   423,
   424,   425,   426,   427,   428,   429,   430,   431,   111,   432,
   763,   113,     0,     0,   433,   434,     0,   435,   436,   437,
     0,   116,   117,   438,   118,     0,     0,   119,   120,   121,
   122,   123,     0,     0,     0,   124,   125,     0,     0,     0,
     0,   126,   439,   128,     0,     0,     0,     0,   440,   441,
     0,     0,     0,     0,   510,     0,   443,   105,     0,     0,
   294,     0,     0,  1567,     0,     0,     0,   106,     0,  1573,
     0,     0,     0,     0,  1577,  1568,  1569,     0,     0,     0,
     0,  1582,     0,  1575,  1576,     0,  1578,  1579,     0,     0,
     0,     0,   107,     0,  1583,  1584,  1585,  1586,     0,  1587,
  1588,  1589,  1590,  1591,  1592,     0,     0,     0,     0,     0,
   108,     0,     0,     0,     0,     0,  1207,  1207,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     0,  1162,     0,     0,  1148,  1622,     0,     0,   403,   111,
   112,     0,   113,     0,     0,   404,     0,     0,     0,   114,
   115,     0,   116,   117,     0,   118,     0,   405,   119,   120,
   121,   122,   123,   406,   407,     0,   124,   125,     0,     0,
     0,     0,   126,   127,   128,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   408,     0,     0,   409,     0,     0,     0,  -654,     0,
     0,  1656,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,  -654,  -654,     0,   410,
     0,   411,   412,     0,     0,     0,     0,  -654,  -654,     0,
  -654,   413,     0,     0,     0,   414,     0,  -654,  -654,     0,
  -654,  -654,     0,  -654,     0,     0,  -654,  -654,  -654,  -654,
  -654,     0,     0,     0,  -654,  -654,     0,     0,     0,     0,
  -654,  -654,  -654,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1706,     0,     0,     0,     0,     0,
  1711,  1712,     0,     0,     0,     0,     0,     0,  1149,     0,
   415,   576,     0,     0,     0,     0,   416,     0,     0,     0,
     0,     0,     0,     0,  1726,     0,   417,     0,     0,  1207,
     0,     0,  1729,     0,     0,     0,     0,  1735,     0,  1737,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,     0,   433,   434,
     0,   435,   436,   437,     0,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,  1170,     0,     0,  1148,   126,   439,   128,   403,     0,
     0,     0,   440,   441,     0,   404,     0,  1781,   510,     0,
   443,     0,     0,     0,   294,     0,     0,   405,     0,     0,
     0,     0,     0,   406,   407,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   448,     0,     0,     0,
     0,   408,     0,     0,   409,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     0,     0,     0,     0,     0,  1656,     0,     0,     0,   410,
     0,   411,   412,     0,     0,     0,     0,   109,   110,   582,
     0,   413,     0,     0,     0,   414,     0,     0,   111,   112,
     0,   113,     0,     0,     0,     0,     0,     0,   114,   115,
     0,   116,   117,     0,   118,  1836,     0,   119,   120,   121,
   122,   123,     0,     0,     0,   124,   125,     0,     0,     0,
     0,   126,   127,   128,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,  1149,     0,
   415,     0,     0,     0,     0,     0,   416,     0,   973,     0,
     0,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,     0,   433,   434,
     0,   435,   436,   437,     0,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,  1173,     0,     0,  1148,   126,   439,   128,   403,     0,
     0,     0,   440,   441,     0,   404,     0,   108,   510,     0,
   443,     0,     0,     0,   294,     0,     0,   405,     0,     0,
     0,     0,     0,   406,   407,   109,   110,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   111,   112,     0,   113,
     0,     0,     0,     0,     0,     0,   114,   115,     0,   116,
   117,   408,   118,     0,   409,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,     0,     0,   108,     0,   126,
   127,   128,     0,     0,     0,     0,     0,     0,     0,   410,
     0,   411,   412,     0,     0,   109,   110,     0,     0,     0,
     0,   413,     0,     0,     0,   414,   111,   112,     0,   113,
     0,     0,     0,     0,     0,     0,   114,   115,     0,   116,
   117,     0,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,     0,     0,     0,     0,   126,
   127,   128,   406,     0,     0,     0,     0,     0,   974,     0,
     0,     0,     0,     0,     0,     0,     0,     0,  1149,     0,
   415,     0,     0,     0,     0,     0,   416,     0,     0,     0,
   408,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,   410,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,     0,   433,   434,
     0,   435,   436,   437,     0,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,  1178,     0,     0,  1148,   126,   439,   128,   403,     0,
     0,     0,   440,   441,     0,   404,     0,     0,   510,   415,
   443,     0,     0,     0,   294,     0,     0,   405,     0,     0,
     0,     0,     0,   406,   407,   417,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   418,
   419,   420,     0,   217,   218,     0,     0,     0,     0,     0,
     0,   408,     0,     0,   409,     0,     0,     0,     0,     0,
     0,     0,   423,   424,   425,   426,   427,   428,   429,   430,
   431,     0,   491,     0,     0,     0,     0,   433,   434,   410,
     0,   411,   412,     0,     0,     0,   438,     0,     0,   108,
     0,   413,     0,     0,   883,   414,     0,   608,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   109,   110,     0,
   884,     0,  1281,     0,   609,     0,     0,   610,   111,   112,
     0,   113,     0,     0,     0,     0,     0,     0,   114,   115,
     0,   116,   117,     0,   118,     0,     0,   119,   120,   121,
   122,   123,     0,     0,     0,   124,   125,     0,  1149,     0,
   415,   126,   127,   128,     0,     0,   416,   611,     0,     0,
     0,     0,     0,   612,   613,   614,   417,   615,   616,   617,
     0,   618,     0,     0,     0,     0,     0,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,     0,   619,
     0,   620,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,     0,   433,   434,
   621,   435,   436,   437,     0,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,   622,     0,   124,
   125,  1181,     0,     0,  1148,   126,   439,   128,   403,     0,
     0,     0,   440,   441,     0,   404,     0,     0,   510,   623,
   443,     0,     0,     0,   294,     0,     0,   405,     0,     0,
     0,     0,     0,   406,   407,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   624,   625,     0,     0,     0,   607,
     0,   408,   608,     0,   409,     0,     0,     0,     0,     0,
     0,   626,     0,     0,     0,     0,     0,     0,     0,   609,
     0,     0,   610,     0,     0,     0,     0,     0,     0,   410,
     0,   411,   412,     0,     0,     0,     0,     0,     0,     0,
     0,   413,     0,     0,     0,   414,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   611,     0,     0,     0,     0,     0,   612,   613,
   614,     0,   615,   616,   617,     0,   618,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   619,     0,   620,     0,  1149,     0,
   415,     0,     0,     0,     0,     0,   416,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,     0,     0,   621,     0,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,   622,   985,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,   623,   113,     0,     0,   433,   434,
     0,   435,   436,   437,     0,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,  1184,     0,     0,  1148,   126,   439,   128,   403,   624,
   625,     0,   440,   441,     0,   404,     0,     0,   510,     0,
   443,     0,     0,   986,   294,     0,   626,   405,     0,     0,
     0,     0,     0,   406,   407,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   987,     0,
     0,   408,     0,   988,   409,     0,     0,     0,     0,   989,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   108,     0,     0,     0,     0,     0,     0,     0,   990,   410,
     0,   411,   412,     0,     0,     0,     0,     0,   109,   110,
     0,   413,     0,     0,     0,   414,     0,     0,     0,   111,
   112,     0,   113,     0,     0,     0,     0,     0,     0,   114,
   115,     0,   116,   117,     0,   118,     0,     0,   119,   120,
   121,   122,   123,     0,     0,     0,   124,   125,     0,     0,
     0,     0,   126,   127,   128,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,  1149,   985,
   415,     0,     0,     0,     0,     0,   416,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,   403,   433,   434,
     0,   435,   436,   437,   404,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,   405,     0,     0,   124,
   125,     0,   406,   407,     0,   126,   439,   128,     0,     0,
     0,     0,   440,   441,     0,     0,     0,     0,   510,   988,
   443,   665,     0,     0,   294,     0,     0,     0,     0,     0,
   408,     0,     0,   409,     0,     0,   108,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   109,   110,     0,     0,   410,     0,
   411,   412,     0,     0,     0,   111,   112,     0,   113,     0,
   413,     0,     0,     0,   414,   114,   115,     0,   116,   117,
     0,   118,   894,     0,   119,   120,   121,   122,   123,     0,
     0,     0,   124,   125,     0,     0,     0,     0,   126,   127,
   128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
     0,     0,     0,   667,     0,     0,     0,     0,     0,   415,
     0,     0,     0,     0,     0,   416,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   417,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   418,
   419,   420,   421,   217,   218,     0,     0,     0,     0,     0,
     0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
   109,   110,   423,   424,   425,   426,   427,   428,   429,   430,
   431,   111,   432,     0,   113,     0,     0,   433,   434,     0,
   435,   436,   437,     0,   116,   117,   438,   118,     0,     0,
   119,   120,   121,   122,   123,   895,   896,   897,   124,   125,
     0,     0,   403,     0,   126,   439,   128,     0,     0,   404,
     0,   440,   441,     0,     0,     0,     0,   671,    14,   443,
     0,   405,     0,   294,   664,     0,     0,   406,   407,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   665,     0,     0,     0,
     0,     0,     0,     0,     0,   408,     0,     0,   409,     0,
     0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
   110,     0,     0,   410,     0,   411,   412,     0,     0,     0,
   111,   112,     0,   113,     0,   413,     0,     0,     0,   414,
   114,   115,     0,   116,   117,     0,   118,   666,     0,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,     0,
     0,     0,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,  1012,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   667,     0,
     0,     0,     0,     0,   415,     0,     0,     0,     0,     0,
   416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   417,     0,     0,     0,     0,     0,     0,     0,     0,   919,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     0,     0,   920,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,   403,   433,   434,     0,   435,   436,   437,   404,   116,
   117,   438,   118,     0,     0,   119,   120,   668,   669,   670,
   405,     0,     0,   124,   125,     0,   406,   407,     0,   126,
   439,   128,     0,     0,     0,     0,   440,   441,   921,     0,
     0,     0,   671,     0,   443,   665,     0,     0,   294,     0,
     0,     0,     0,     0,   408,     0,     0,   409,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   410,     0,   411,   412,     0,     0,     0,     0,
   109,   110,     0,     0,   413,     0,     0,     0,   414,     0,
     0,   111,   112,     0,   113,     0,   890,     0,     0,     0,
     0,   114,   115,     0,   116,   117,     0,   118,     0,     0,
   119,   120,   121,   122,   123,     0,     0,     0,   124,   125,
     0,     0,     0,     0,   126,   127,   128,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   667,     0,     0,
     0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   418,   419,   420,   421,   217,   218,     0,
     0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
     0,     0,     0,     0,   109,   110,   423,   424,   425,   426,
   427,   428,   429,   430,   431,   111,   432,     0,   113,     0,
   403,   433,   434,     0,   435,   436,   437,   404,   116,   117,
   438,   118,     0,     0,   119,   120,   668,   669,   670,   405,
     0,     0,   124,   125,     0,   406,   407,     0,   126,   439,
   128,     0,     0,   486,     0,   440,   441,     0,     0,     0,
     0,   671,     0,   443,   665,     0,     0,   294,     0,     0,
     0,     0,     0,   408,     0,     0,   409,     0,     0,   108,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   109,   110,     0,
     0,   410,     0,   411,   412,     0,     0,     0,   111,   112,
     0,   113,     0,   413,     0,     0,     0,   414,   114,   115,
     0,   116,   117,     0,   118,   666,     0,   119,   120,   121,
   122,   123,     0,     0,     0,   124,   125,     0,     0,     0,
     0,   126,   127,   128,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   667,     0,     0,     0,
     0,     0,   415,     0,     0,     0,     0,     0,   416,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   417,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   418,   419,   420,   421,   217,   218,     0,     0,
     0,     0,     0,     0,     0,     0,   422,     0,     0,     0,
     0,     0,     0,   109,   110,   423,   424,   425,   426,   427,
   428,   429,   430,   431,   111,   432,     0,   113,     0,   403,
   433,   434,     0,   435,   436,   437,   404,   116,   117,   438,
   118,     0,     0,   119,   120,   668,   669,   670,   405,     0,
     0,   124,   125,     0,   406,   407,     0,   126,   439,   128,
     0,     0,     0,     0,   440,   441,     0,     0,     0,     0,
   671,     0,   443,   665,     0,     0,   294,     0,     0,     0,
     0,     0,   408,     0,     0,   409,     0,     0,   108,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   109,   110,     0,     0,
   410,     0,   411,   412,     0,     0,     0,   111,   112,     0,
   113,     0,   413,     0,     0,     0,   414,   114,   115,     0,
   116,   117,     0,   118,   890,     0,   119,   120,   121,   122,
   123,     0,     0,     0,   124,   125,     0,     0,     0,     0,
   126,   127,   128,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,  1857,     0,   443,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   667,     0,     0,     0,     0,
     0,   415,     0,     0,     0,     0,     0,   416,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   417,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   418,   419,   420,   421,   217,   218,     0,     0,     0,
     0,     0,     0,     0,     0,   422,     0,     0,     0,     0,
     0,     0,   109,   110,   423,   424,   425,   426,   427,   428,
   429,   430,   431,   111,   432,     0,   113,     0,   403,   433,
   434,     0,   435,   436,   437,   404,   116,   117,   438,   118,
     0,     0,   119,   120,   121,   122,   123,   405,     0,     0,
   124,   125,     0,   406,   407,     0,   126,   439,   128,     0,
     0,     0,     0,   440,   441,     0,     0,     0,     0,   671,
     0,   443,     0,     0,     0,   294,     0,     0,     0,     0,
     0,   408,     0,     0,   409,     0,     0,   108,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   109,   110,     0,     0,   410,
     0,   411,   412,     0,     0,     0,   111,   112,     0,   113,
     0,   413,     0,     0,     0,   414,   114,   115,     0,   116,
   117,     0,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,   108,   124,   125,     0,     0,     0,     0,   126,
   127,   128,     0,     0,     0,     0,     0,     0,     0,     0,
   109,   110,     0,     0,   443,     0,     0,     0,     0,     0,
     0,   111,   112,     0,   113,     0,     0,     0,     0,     0,
   415,   114,   115,     0,   116,   117,   416,   118,     0,     0,
   119,   120,   121,   122,   123,     0,   417,     0,   124,   125,
     0,     0,     0,     0,   126,   127,   128,     0,     0,     0,
   418,   419,   420,   421,   217,   218,     0,  1012,     0,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,   403,   433,   434,
     0,   435,   436,   437,   404,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,   405,     0,     0,   124,
   125,     0,   406,   407,     0,   126,   439,   128,     0,     0,
     0,     0,   440,   441,   515,     0,     0,     0,   510,     0,
   443,     0,     0,     0,   294,     0,     0,     0,     0,     0,
   408,     0,     0,   409,     0,     0,   108,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   109,   110,     0,     0,   410,     0,
   411,   412,     0,     0,     0,   111,   112,     0,   113,     0,
   413,     0,     0,     0,   414,   114,   115,     0,   116,   117,
     0,   118,     0,     0,   119,   120,   121,   122,   123,     0,
     0,     0,   124,   125,     0,     0,     0,     0,   126,   127,
   128,     0,     0,     0,     0,     0,     0,     0,     0,  1616,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     0,     0,     0,     0,     0,   416,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   417,     0,     0,     0,     0,
     0,     0,     0,   703,     0,     0,     0,     0,     0,   418,
   419,   420,   421,   217,   218,     0,     0,     0,     0,     0,
     0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
   109,   110,   423,   424,   425,   426,   427,   428,   429,   430,
   431,   111,   432,     0,   113,     0,   403,   433,   434,     0,
   435,   436,   437,   404,   116,   117,   438,   118,     0,     0,
   119,   120,   121,   122,   123,   405,     0,     0,   124,   125,
     0,   406,   407,     0,   126,   439,   128,     0,     0,     0,
     0,   440,   441,     0,     0,     0,     0,   510,     0,   443,
     0,     0,     0,   294,     0,     0,     0,     0,     0,   408,
     0,     0,   409,     0,     0,   108,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   109,   110,     0,     0,   410,     0,   411,
   412,     0,     0,     0,   111,   112,     0,   113,     0,   413,
     0,     0,     0,   414,   114,   115,     0,   116,   117,     0,
   118,     0,     0,   119,   120,   121,   122,   123,     0,     0,
     0,   124,   125,     0,     0,     0,     0,   126,   127,   128,
     0,     0,     0,     0,     0,     0,     0,     0,  1625,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    14,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   415,     0,
     0,     0,     0,     0,   416,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   418,   419,
   420,   421,   217,   218,     0,     0,     0,     0,     0,     0,
     0,     0,   422,     0,     0,     0,     0,     0,     0,   109,
   110,   423,   424,   425,   426,   427,   428,   429,   430,   431,
   111,   432,     0,   113,     0,   403,   433,   434,     0,   435,
   436,   437,   404,   116,   117,   438,   118,     0,     0,   119,
   120,   121,   122,   123,   405,     0,     0,   124,   125,     0,
   406,   407,     0,   126,   439,   128,     0,     0,     0,     0,
   440,   441,     0,     0,     0,     0,   510,     0,   443,     0,
     0,     0,   294,     0,     0,     0,     0,     0,   408,     0,
     0,   409,    78,     0,   108,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   109,   110,     0,     0,   410,     0,   411,   412,
     0,     0,     0,   111,   112,     0,   113,     0,   413,     0,
     0,     0,   414,   114,   115,     0,   116,   117,     0,   118,
     0,     0,   119,   120,   121,   122,   123,     0,     0,   108,
   124,   125,     0,     0,     0,     0,   126,   127,   128,     0,
     0,     0,     0,     0,    79,     0,     0,   109,   110,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   111,   112,
     0,   113,     0,     0,     0,     0,     0,   415,   114,   115,
     0,   116,   117,   416,   118,     0,     0,   119,   120,   121,
   122,   123,     0,   417,     0,   124,   125,     0,     0,     0,
     0,   126,   127,   128,     0,     0,     0,   418,   419,   420,
   421,   217,   218,     0,     0,     0,     0,     0,     0,     0,
     0,   422,     0,     0,     0,     0,     0,     0,   109,   110,
   423,   424,   425,   426,   427,   428,   429,   430,   431,   111,
   432,     0,   113,     0,   403,   433,   434,     0,   435,   436,
   437,   404,   116,   117,   438,   118,     0,     0,   119,   120,
   121,   122,   123,   405,     0,     0,   124,   125,     0,   406,
   407,     0,   126,   439,   128,     0,     0,     0,     0,   440,
   441,     0,     0,     0,     0,   510,   971,   443,     0,     0,
     0,   294,     0,     0,     0,     0,     0,   408,     0,     0,
   409,     0,     0,   108,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   109,   110,     0,     0,   410,     0,   411,   412,     0,
     0,     0,   111,   112,     0,   113,     0,   413,     0,     0,
     0,   414,   114,   115,     0,   116,   117,     0,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,     0,     0,     0,     0,   126,   127,   128,     0,     0,
     0,     0,     0,     0,   974,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   415,     0,     0,     0,
     0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   417,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,  1176,     0,     0,   418,   419,   420,   421,
   217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
   422,     0,     0,     0,     0,     0,     0,   109,   110,   423,
   424,   425,   426,   427,   428,   429,   430,   431,   111,   432,
     0,   113,     0,   403,   433,   434,     0,   435,   436,   437,
   404,   116,   117,   438,   118,     0,     0,   119,   120,   121,
   122,   123,   405,     0,     0,   124,   125,     0,   406,   407,
     0,   126,   439,   128,     0,     0,     0,     0,   440,   441,
     0,     0,     0,     0,   510,     0,   443,     0,     0,     0,
   294,     0,     0,     0,     0,     0,   408,     0,     0,   409,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   410,     0,   411,   412,     0,     0,
     0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
   414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
     0,   416,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   417,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  1381,     0,     0,   418,   419,   420,   421,   217,
   218,     0,     0,     0,     0,     0,     0,     0,     0,   422,
     0,     0,     0,     0,     0,     0,   109,   110,   423,   424,
   425,   426,   427,   428,   429,   430,   431,   111,   432,     0,
   113,     0,   403,   433,   434,     0,   435,   436,   437,   404,
   116,   117,   438,   118,     0,     0,   119,   120,   121,   122,
   123,   405,     0,     0,   124,   125,     0,   406,   407,     0,
   126,   439,   128,     0,     0,     0,     0,   440,   441,     0,
     0,     0,     0,   510,     0,   443,     0,     0,     0,   294,
     0,     0,     0,     0,     0,   408,     0,     0,   409,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   410,     0,   411,   412,     0,     0,     0,
     0,     0,     0,     0,     0,   413,     0,     0,     0,   414,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   415,     0,     0,     0,     0,     0,
   416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   417,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,   403,   433,   434,     0,   435,   436,   437,   404,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
   405,     0,     0,   124,   125,     0,   406,   407,     0,   126,
   439,   128,     0,     0,     0,     0,   440,   441,     0,     0,
  1734,     0,   510,     0,   443,     0,     0,     0,   294,     0,
     0,     0,     0,     0,   408,     0,     0,   409,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   410,     0,   411,   412,     0,     0,     0,     0,
     0,     0,     0,     0,   413,     0,     0,     0,   414,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   418,   419,   420,   421,   217,   218,     0,
     0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
     0,     0,     0,     0,   109,   110,   423,   424,   425,   426,
   427,   428,   429,   430,   431,   111,   432,     0,   113,     0,
   403,   433,   434,     0,   435,   436,   437,   404,   116,   117,
   438,   118,     0,     0,   119,   120,   121,   122,   123,   405,
     0,     0,   124,   125,     0,   406,   407,     0,   126,   439,
   128,     0,     0,     0,     0,   440,   441,     0,     0,     0,
     0,   442,     0,   443,     0,     0,     0,   294,     0,     0,
     0,     0,     0,   408,     0,     0,   409,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   410,     0,   411,   412,     0,     0,     0,     0,     0,
     0,     0,     0,   413,     0,     0,     0,   414,     0,     0,
     0,     0,     0,     0,     0,  1225,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     6,     0,     0,     0,     0,     0,     0,
  -287,     0,     0,     0,  1430,  1431,     0,  1432,     0,     0,
     0,     0,   415,     0,     0,     0,     0,     0,   416,     0,
     0,     0,     0,  1433,     0,     0,     0,     0,   417,  1434,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   418,   419,   420,   421,   217,   218,     0,     0,
     0,     0,     0,     0,     0,     0,   422,     0,     0,     0,
     0,     0,     0,   109,   110,   423,   424,   425,   426,   427,
   428,   429,   430,   431,   111,   432,     0,   113,     0,     0,
   433,   434,     0,   435,   436,   437,  1435,   116,   117,   438,
   118,     0,     0,   119,   120,   121,   122,   123,     0,     0,
  1225,   124,   125,     0,    14,     0,     0,   126,   439,   128,
     0,     0,     0,     0,   440,   441,     0,     0,     0,  1436,
   510,     0,   443,     0,     0,     0,   294,     0,     6,    16,
     0,     0,     0,     0,     0,  -289,     0,     0,  1632,  1430,
  1431,     0,  1432,     0,     0,     0,     0,   421,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,  1433,     0,
     0,     0,     0,     0,  1434,   109,   110,     0,     0,     0,
     0,     0,     0,     0,     0,     0,  1437,   112,     0,   113,
     0,     0,     0,     0,     0,     0,   114,   115,     0,   116,
   117,     0,   118,     0,     0,   119,  1438,   121,   122,   123,
     0,     0,     0,   124,   125,  1439,  1440,  1441,  1225,   126,
   127,   128,     0,     0,     0,     0,     0,     0,     0,     0,
     0,  1435,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     6,     0,     0,    14,
     0,     0,     0,     0,     0,     0,     0,  1430,  1431,     0,
  1432,     0,     0,     0,  1436,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    16,     0,  1433,     0,     0,     0,
     0,     0,  1434,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   421,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   109,   110,     0,     0,     0,     0,     0,     0,     0,     0,
     0,  1437,   112,     0,   113,     0,     0,     0,     0,     0,
     0,   114,   115,     0,   116,   117,     0,   118,     0,  1435,
   119,  1438,   121,   122,   123,     0,     0,     0,   124,   125,
  1439,  1440,  1441,     0,   126,   127,   128,    14,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  1436,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    16,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   421,     0,   607,     0,     0,   608,     0,     0,     0,     0,
     0,   808,     0,     0,     0,     0,     0,     0,   109,   110,
     0,     0,   609,     0,     0,   610,     0,     0,     0,  1437,
   112,     0,   113,     0,     0,     0,     0,     0,     0,   114,
   115,     0,   116,   117,     0,   118,     0,     0,   119,  1438,
   121,   122,   123,     0,     0,     0,   124,   125,  1439,  1440,
  1441,     0,   126,   127,   128,   611,     0,     0,     0,     0,
     0,   612,   613,   614,     0,   615,   616,   617,     0,   618,
     0,     0,     0,   607,     0,     0,   608,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   619,     0,   620,
     0,     0,     0,   609,     0,     0,   610,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   621,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   622,   611,     0,     0,     0,
     0,     0,   612,   613,   614,     0,   615,   616,   617,     0,
   618,     0,     0,     0,     0,     0,     0,   623,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   619,     0,
   620,     0,   108,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   109,   110,   624,   625,     0,     0,     0,     0,     0,   621,
     0,   111,   112,     0,   113,     0,     0,     0,     0,   626,
     0,   114,   115,     0,   116,   117,   622,   118,     0,     0,
   119,   120,   121,   122,   123,     0,     0,     0,   124,   125,
     0,     0,     0,     0,   126,   127,   128,     0,   623,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   108,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   109,   110,   624,   625,     0,     0,     0,     0,     0,
     0,     0,   111,   112,     0,   113,     0,     0,     0,     0,
   626,     0,   114,   115,     0,   116,   117,     0,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,   108,     0,     0,     0,   126,   127,   128,     0,     0,
   173,   174,   175,   176,   177,   178,   179,   180,   181,   109,
   110,   182,   183,   184,   185,   186,     0,     0,   187,   188,
   111,   112,     0,   113,     0,     0,   189,   190,     0,     0,
   114,   115,   191,   116,   117,   192,   118,   193,     0,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,   194,
   195,   196,   197,   126,   127,   128,   198
};

static const short yycheck[] = {     9,
     0,     4,    12,    66,   157,   107,    16,    57,    18,   147,
   148,   350,   545,    65,    16,   727,    70,    58,   147,   148,
    65,   381,   381,    96,    87,    64,   377,   352,    69,   578,
   521,    93,    75,   781,   442,    16,   703,    72,   477,    93,
   550,    91,   709,   280,  1298,    55,    56,    57,    58,    59,
    60,    92,   981,   290,    97,    65,    66,    67,    99,    69,
    95,    71,    72,    90,    74,    75,  1065,  1095,   499,  1012,
   498,   734,   784,    83,   373,    85,    86,    87,    59,    89,
    90,    91,    92,    85,    94,    95,    96,    97,   161,    99,
    71,  1055,    94,   138,   659,   140,  1055,   689,  1055,   358,
   924,  1065,   752,  1055,    85,   359,  1065,   296,  1065,  1242,
   652,  1118,   877,  1065,  1062,   233,     8,   323,   159,  1445,
     8,   713,     6,   165,   504,   152,   752,    63,   138,   139,
   140,   570,   386,   572,     8,   959,   817,    10,     8,   578,
   242,     8,   152,  1055,     8,   535,  1556,   157,    61,   159,
   160,   161,  1234,  1065,  1236,   165,     8,  1055,   139,    90,
   787,  1224,    46,   202,  1059,  1113,    21,  1065,    19,    23,
   323,  1066,    50,    51,    28,   802,     8,  1296,   805,   160,
  1075,  1225,     8,    44,    47,   442,  1206,   814,   227,    26,
    12,    23,  1212,    68,    63,  1264,    28,    23,    80,   666,
   666,   174,    28,    14,   671,   556,  1101,    47,   190,   327,
  1497,   141,    43,  1811,     3,    56,   133,    21,   578,   578,
   126,   286,   193,   143,   192,   270,    10,    46,   273,   331,
    44,    43,    22,   125,    43,   902,   903,   125,   196,    23,
   142,   580,   196,   870,    28,    44,   143,    44,    63,   192,
     9,   125,   603,   510,   491,   125,   158,   307,   125,    90,
   270,   125,    12,   273,    10,   162,    80,   277,  1866,   223,
   280,   281,   178,   125,   194,    94,   286,    23,   323,    63,
   290,    80,    28,    87,    23,   952,   540,    76,   627,    28,
   272,  1290,  1291,   168,   105,    61,    55,   307,   180,   272,
   175,   311,  1330,    23,   272,   174,   351,    66,    28,     4,
   718,   189,  1255,   358,   155,   360,   246,   362,   726,   180,
   365,   279,   144,  1271,   255,  1324,  1290,  1291,   118,   272,
   311,  1290,  1291,  1290,  1291,   374,   272,   376,  1290,  1291,
   177,   351,   383,    82,   394,   498,   996,     8,   358,   384,
   360,   390,   362,   204,    49,   365,   273,  1007,  1008,   272,
  1324,     8,  1649,   854,  1293,  1324,   180,  1324,    97,  1779,
   996,   761,  1324,   383,   384,   755,  1439,  1440,  1290,  1291,
   272,  1007,   392,   180,   394,   269,   274,  1054,   398,  1070,
  1071,   830,  1290,  1291,   144,   398,  1416,  1417,    23,   272,
   274,  1445,  1521,    28,   274,  1487,   165,   274,  1337,   272,
   274,   392,  1324,  1040,   671,   269,   270,    11,  1313,  1673,
  1489,   276,   274,   890,   255,   191,  1324,   894,   894,    23,
  1057,  1058,   272,   443,    28,   267,   268,   269,   270,  1120,
   505,   267,   268,   269,   270,  1037,  1270,   878,  1639,   490,
  1077,    45,   264,   498,    23,   264,   149,   796,   889,    28,
   799,   156,   893,   504,   125,     8,   595,   509,    19,    50,
    51,  1138,  1139,   812,  1141,  1142,   486,  1803,   125,   174,
   490,   491,     8,   267,   268,   269,   270,   497,   272,   185,
    18,   501,   196,    18,   504,   505,   297,    26,   837,   509,
   545,   230,   141,   141,   305,   486,  1832,   744,   272,    43,
   274,    11,   173,   208,  1647,    62,   497,  1771,     3,   558,
   501,   267,   268,   269,   270,   643,   173,    17,   267,   268,
   269,   270,   271,   578,   596,   545,   169,  1565,   176,  1865,
   117,    31,   596,    94,   803,    45,  1553,   267,   268,   269,
   270,  1877,   272,    90,  1880,  1498,   881,    82,  1221,   598,
    94,  1126,  1888,   196,   268,  1819,   576,  1630,   578,   758,
  1112,  1336,   582,  1001,   780,    85,  1505,   273,    23,   156,
   273,  1012,   125,    28,    44,    75,  1534,  1120,   244,  1609,
  1844,    76,   139,   731,   123,  1003,   196,   275,   108,   125,
   278,  1251,  1510,  1794,  1552,  1243,   651,  1798,   189,   648,
   649,  1042,  1279,   103,   868,  1806,    64,    23,  1809,  1514,
    80,   260,    28,  1518,  1519,    23,   203,   780,    23,    77,
    28,   271,   179,    28,  1301,   268,   196,   766,  1829,   196,
   273,   651,   267,   268,   269,   270,    23,  1555,   177,   274,
   196,    28,   100,   251,   164,    23,     6,    79,   230,  1554,
    28,   259,   199,   200,    23,   194,  1857,   694,    46,    28,
   117,  1862,   272,   267,   268,   269,   270,   167,    23,    61,
   169,  1319,   147,    28,   694,   145,     7,   732,    23,   734,
   262,   736,   114,    28,    44,   155,     6,    23,   267,   268,
   269,   270,    28,   151,   126,   274,    56,   196,  1335,   719,
    37,   268,     4,   273,   755,     7,    94,   727,    98,   279,
   180,    48,   732,     6,   734,  1064,   736,   273,    55,   739,
    80,  1730,     6,   279,   744,   780,    46,  1076,   196,   186,
   118,   786,   752,   155,  1152,   755,   196,  1571,   985,  1048,
   760,   988,    79,   792,   764,  1503,  1685,    49,   803,  1803,
   141,  1100,  1691,    46,  1659,   775,  1730,   777,  1663,   831,
   182,  1730,    46,  1730,   784,  1367,   786,   831,  1730,   268,
   196,    23,   836,  1678,   829,   830,    28,   167,  1832,  1299,
   279,   172,    65,   803,   775,   145,   777,   851,  1123,     6,
  1695,   863,   864,   842,   866,   155,   187,   223,  1216,   863,
   864,     6,   866,    30,   141,  1114,  1879,     6,  1730,   829,
   830,  1865,   267,   268,   269,   270,    43,    65,     5,   274,
   180,  1322,  1730,  1877,   844,  1502,  1880,  1328,    15,    46,
   231,   844,     4,   273,  1888,   172,  1639,   140,  1001,   176,
   241,    46,   268,  1402,    16,  1263,   273,    46,   272,   869,
   274,   267,   268,   269,   270,   272,   122,  1192,   274,   879,
   277,   272,   267,   268,   269,   270,    23,   887,   888,   274,
   272,    28,   122,  1778,   887,   277,  1781,    49,   869,    51,
   267,   268,   269,   270,   203,  1152,   205,   274,   879,   267,
   268,   269,   270,   272,    98,   274,   274,    17,   267,   268,
   269,   270,   272,    23,   124,   274,   989,   277,    28,    59,
    60,    31,   267,   268,   269,   270,   231,   232,  1823,   274,
   199,   200,   267,   268,   269,   270,  1826,  1827,   132,   274,
   134,   267,   268,   269,   270,   986,    23,   990,   274,    17,
   122,    28,   987,   122,  1621,    23,  1001,    67,   174,    23,
    28,    84,   156,    31,    28,    75,   272,   232,   274,  1098,
   130,   131,  1311,    83,    79,   985,   986,   987,   988,   989,
   990,   272,   272,   274,   274,  1424,   996,   181,   156,  1340,
    23,   101,   180,   103,   117,    28,    19,  1007,  1008,    67,
   259,  1794,  1012,  1342,    10,  1798,   116,    75,   118,   119,
   120,   192,    23,  1806,   162,    83,  1809,    28,  1794,   272,
   143,   274,  1798,   146,    23,   267,   268,   269,   270,    28,
  1806,    59,   274,   101,   273,   103,  1829,   271,   272,   162,
    23,    61,  1402,  1402,   272,    28,   274,  1109,   116,    23,
   118,   119,   120,  1829,    28,  1109,   199,   167,    23,   272,
  1122,   274,    79,    28,  1857,   188,  1128,   198,  1122,  1862,
    78,  1116,  1225,   190,  1128,  1120,   272,  1087,   274,   149,
   272,  1857,   274,   141,  1751,   272,  1862,   274,   272,  1092,
   274,    82,    23,  1760,   690,  1105,   692,    28,  1810,   167,
   696,   697,   698,  1241,  1242,  1243,  1116,  1364,  1365,   272,
  1120,   274,    23,   271,   272,  1372,  1373,    28,  1375,  1376,
   267,   268,   269,   270,   118,   272,  1383,  1384,  1385,  1386,
    63,  1388,  1389,  1390,  1391,  1392,  1393,   273,   272,    23,
   274,   174,    61,   141,    28,   139,  1275,   272,  1277,   274,
   122,   145,    63,   122,   264,   265,   266,   267,   268,   269,
   270,   244,    23,    17,   174,   102,   276,    28,   278,    23,
    54,   272,  1224,   274,    28,   188,  1221,    31,  1188,  1224,
  1225,   272,   172,   274,  1523,   179,  1188,   272,   123,   274,
   267,   268,   269,   270,   121,   272,   264,   265,   266,   267,
   268,   269,   270,   267,   268,   269,   270,   271,   276,  1338,
   278,  1221,   243,    67,  1224,  1225,  1234,    23,  1236,   155,
  1559,    75,    28,   337,  1234,   339,  1236,   254,  1258,    83,
  1260,   255,    17,   254,   267,   268,   269,   270,    23,    30,
   207,  1251,   272,    28,   112,  1255,    31,   101,    54,   103,
   195,   196,   197,    29,   273,   264,   267,   268,   269,   270,
    10,    64,   116,   274,   118,   119,   120,   199,   267,   268,
   269,   270,   135,   273,    77,   274,   273,   273,   223,   224,
   273,  1814,    67,    23,   267,   268,   269,   270,    28,   273,
    75,   274,  1445,   267,   268,   269,   270,   100,    83,   269,
   274,   199,   267,   268,   269,   270,   199,   273,   276,   274,
    23,   114,  1351,   167,  1317,    28,   101,  1356,   103,   122,
   273,    61,   273,   293,   273,   273,   273,     6,   278,   273,
   273,   116,   302,   118,   119,   120,   267,   268,   269,   270,
   278,   264,   272,   274,   273,   273,    10,  1500,   151,   273,
   174,    63,   141,  1692,   272,    63,   267,   268,   269,   270,
   272,   166,   272,   264,   278,    44,   336,  1430,   273,    82,
   340,    10,   272,    90,    90,    90,    22,    56,    22,    22,
    43,    64,   167,   267,   268,   269,   270,    30,  1398,    10,
   174,    70,   208,   136,    77,   174,  1398,   136,   196,    21,
  1445,    80,    85,    90,   185,    64,   267,   268,   269,   270,
   264,   265,   266,   267,   268,   269,   270,   100,    77,   185,
  1430,   273,   276,    91,   278,   273,   276,   138,  1438,  1439,
  1440,   114,   273,   273,    61,  1445,   239,    10,   909,   149,
   911,   100,   913,   914,   122,   916,   917,   918,   128,   272,
   121,   273,   273,   126,   189,   114,   274,    21,    42,   279,
   263,   267,   268,   269,   270,   435,   145,   174,   151,   202,
   440,   441,   442,    60,   273,   118,   155,  1487,    63,   264,
   265,   266,   267,   268,   269,   270,  1639,  1497,  1498,   274,
  1500,    63,   151,    48,   190,  1648,   139,   186,    10,   272,
    55,   180,   145,   240,    59,    60,   273,    43,    19,  1647,
   273,    27,    86,    68,  1524,    86,    22,   125,   125,   198,
   125,   156,   154,    43,    79,   156,   273,   267,   268,   269,
   270,    98,   273,   108,    63,   273,   179,   216,   217,   273,
   510,   272,   274,  1524,   514,    23,   184,  1597,   227,   228,
    16,   230,   272,   107,   267,   268,   269,   270,   237,   238,
    63,   240,   241,   124,   243,    12,   272,   246,   247,   248,
   249,   250,   272,    71,  1676,   254,   255,  1618,   273,   273,
   263,   260,   261,   262,  1594,   239,   141,  1597,   273,    10,
   160,   275,  1594,   210,   211,   212,   213,   214,   215,   216,
   217,    10,   157,   192,   263,   274,   133,   274,  1618,   274,
    52,    52,   274,   274,   274,     8,   273,   172,   273,    63,
  1630,   176,   273,   273,   273,   273,   273,  1677,   273,    51,
   173,   273,   187,   273,    70,   272,  1636,   273,  1648,  1649,
   273,  1794,   127,   273,   273,  1798,   274,   273,   273,    19,
  1803,   206,    85,  1806,   274,    85,  1809,  1667,    73,  1698,
    17,   130,  1672,  1702,   145,   125,    23,  1677,   125,   125,
  1772,    28,   125,   125,    31,   125,  1829,   274,   196,  1832,
  1743,   274,    22,   273,    63,    51,   274,   173,   274,   272,
    36,  1672,   274,   274,   274,   271,   273,    36,    82,    51,
    82,   671,   271,   271,  1857,   271,   271,   124,   271,  1862,
    67,   271,  1865,   271,  1724,   271,   275,    10,    75,   271,
   174,   691,    85,    23,  1877,   695,    83,  1880,    79,   180,
    80,   180,   702,  1743,  1744,  1888,    44,   707,   708,    80,
    44,   274,  1745,   274,   101,    94,   103,   124,   718,   111,
   720,   721,   722,   723,   724,   725,   726,   111,  1803,   116,
   274,   118,   119,   120,    37,   274,   274,   274,   273,  1814,
   274,    61,   274,   274,   274,    48,   189,   274,   274,   274,
   274,   274,    55,   274,   274,   274,   274,  1832,   271,   273,
   272,  1791,   762,  1803,   274,    68,    94,    86,   204,    79,
  1810,  1811,   273,   271,  1814,    82,   246,    82,   278,   273,
   167,   273,   122,   783,   272,   274,  1826,  1827,   273,   789,
  1865,   791,  1832,   148,   272,   173,   122,     6,   122,   274,
     9,   274,  1877,   271,    13,  1880,    47,    44,   180,   274,
    50,    20,    36,  1888,    47,   201,     0,     0,    12,   278,
  1239,   290,   125,    32,   281,  1865,  1866,   744,  1126,    38,
    39,   655,  1123,  1115,   882,   887,  1543,  1877,   141,  1879,
  1880,   348,  1051,   603,   813,  1116,  1120,   786,  1888,   378,
  1542,   360,  1613,   329,   157,  1521,   360,    66,  1523,  1817,
    69,  1846,  1821,  1820,   982,   803,  1634,  1736,   732,   172,
  1804,  1506,  1273,   176,   365,   861,   179,   264,   265,   266,
   267,   268,   269,   270,   187,    94,   353,    96,    97,   607,
   346,  1326,   640,    13,   315,   523,   584,   106,   853,   775,
   150,   110,   527,   206,   904,   905,   906,   762,  1012,   909,
   910,   911,    85,   913,   914,   915,   916,   917,   918,  1765,
  1594,    27,  1259,   789,  1398,   925,   926,     0,   928,   688,
   930,     0,     0,   933,   934,   935,   936,   937,   938,   751,
  1004,  1214,   844,   535,  1167,  1105,  1667,  1281,   582,    89,
   950,  1438,    -1,   953,   163,    -1,   165,    -1,    -1,    -1,
    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   209,    -1,    -1,  1003,    -1,    -1,    -1,   216,   217,   218,
   219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
    -1,   240,   241,   242,   243,    -1,     7,   246,   247,   248,
   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    25,    -1,    -1,    -1,   267,   268,
    -1,    -1,    33,    -1,   273,    -1,   275,    -1,    -1,    40,
   279,    42,    -1,    44,    -1,    -1,    -1,    -1,    49,    -1,
    -1,    -1,    -1,    -1,    -1,    56,    -1,     6,    -1,    -1,
     9,    -1,   198,    -1,    13,    -1,    -1,    -1,    -1,    70,
    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
   216,   217,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
    39,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
   246,   247,   248,   249,   250,    -1,    -1,    66,   254,   255,
    69,    -1,  1152,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,  1165,  1166,   273,  1168,  1169,
    -1,    -1,   143,    -1,    -1,    94,  1176,    96,    97,   150,
    -1,    -1,   153,    -1,   155,   156,    -1,   106,    -1,    -1,
    -1,   110,    -1,    -1,    -1,    -1,  1196,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,  1204,  1205,  1206,    -1,    -1,   180,
    -1,    -1,  1212,    -1,  1214,  1215,  1216,  1217,     6,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
    -1,    -1,   171,    -1,    -1,    -1,    44,    -1,   229,    -1,
    -1,    -1,   181,  1263,    -1,    -1,    -1,    -1,    56,    -1,
    -1,    -1,    -1,   244,   245,    -1,   195,   196,   197,   198,
   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   209,    -1,    80,    -1,  1294,    -1,    -1,   216,   217,   218,
   219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
    10,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,
    -1,    -1,    -1,    -1,   273,    -1,   275,   145,    -1,    -1,
   279,    -1,    -1,  1363,    -1,    -1,    -1,   155,    -1,  1369,
    -1,    -1,    -1,    -1,  1374,  1364,  1365,    -1,    -1,    -1,
    -1,  1381,    -1,  1372,  1373,    -1,  1375,  1376,    -1,    -1,
    -1,    -1,   180,    -1,  1383,  1384,  1385,  1386,    -1,  1388,
  1389,  1390,  1391,  1392,  1393,    -1,    -1,    -1,    -1,    -1,
   198,    -1,    -1,    -1,    -1,    -1,  1416,  1417,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,     6,    -1,    -1,     9,  1435,    -1,    -1,    13,   227,
   228,    -1,   230,    -1,    -1,    20,    -1,    -1,    -1,   237,
   238,    -1,   240,   241,    -1,   243,    -1,    32,   246,   247,
   248,   249,   250,    38,    39,    -1,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,   198,    -1,
    -1,  1501,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    94,
    -1,    96,    97,    -1,    -1,    -1,    -1,   227,   228,    -1,
   230,   106,    -1,    -1,    -1,   110,    -1,   237,   238,    -1,
   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,  1574,    -1,    -1,    -1,    -1,    -1,
  1580,  1581,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
   165,    30,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,  1604,    -1,   181,    -1,    -1,  1609,
    -1,    -1,  1612,    -1,    -1,    -1,    -1,  1617,    -1,  1619,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
    -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,     6,    -1,    -1,     9,   260,   261,   262,    13,    -1,
    -1,    -1,   267,   268,    -1,    20,    -1,  1697,   273,    -1,
   275,    -1,    -1,    -1,   279,    -1,    -1,    32,    -1,    -1,
    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,  1736,    -1,    -1,    -1,
    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
    -1,    -1,    -1,    -1,    -1,  1765,    -1,    -1,    -1,    94,
    -1,    96,    97,    -1,    -1,    -1,    -1,   216,   217,    30,
    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   227,   228,
    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
    -1,   240,   241,    -1,   243,  1805,    -1,   246,   247,   248,
   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    39,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
    -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,     6,    -1,    -1,     9,   260,   261,   262,    13,    -1,
    -1,    -1,   267,   268,    -1,    20,    -1,   198,   273,    -1,
   275,    -1,    -1,    -1,   279,    -1,    -1,    32,    -1,    -1,
    -1,    -1,    -1,    38,    39,   216,   217,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
   241,    66,   243,    -1,    69,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,    -1,    -1,   198,    -1,   260,
   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
    -1,    96,    97,    -1,    -1,   216,   217,    -1,    -1,    -1,
    -1,   106,    -1,    -1,    -1,   110,   227,   228,    -1,   230,
    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
   261,   262,    38,    -1,    -1,    -1,    -1,    -1,   269,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
    66,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    94,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
    -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,     6,    -1,    -1,     9,   260,   261,   262,    13,    -1,
    -1,    -1,   267,   268,    -1,    20,    -1,    -1,   273,   165,
   275,    -1,    -1,    -1,   279,    -1,    -1,    32,    -1,    -1,
    -1,    -1,    -1,    38,    39,   181,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
   196,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,
    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   218,   219,   220,   221,   222,   223,   224,   225,
   226,    -1,   181,    -1,    -1,    -1,    -1,   233,   234,    94,
    -1,    96,    97,    -1,    -1,    -1,   242,    -1,    -1,   198,
    -1,   106,    -1,    -1,    18,   110,    -1,    21,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
    34,    -1,   268,    -1,    38,    -1,    -1,    41,   227,   228,
    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,
    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
   249,   250,    -1,    -1,    -1,   254,   255,    -1,   163,    -1,
   165,   260,   261,   262,    -1,    -1,   171,    81,    -1,    -1,
    -1,    -1,    -1,    87,    88,    89,   181,    91,    92,    93,
    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,   113,
    -1,   115,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
   144,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,   161,    -1,   254,
   255,     6,    -1,    -1,     9,   260,   261,   262,    13,    -1,
    -1,    -1,   267,   268,    -1,    20,    -1,    -1,   273,   183,
   275,    -1,    -1,    -1,   279,    -1,    -1,    32,    -1,    -1,
    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   218,   219,    -1,    -1,    -1,    18,
    -1,    66,    21,    -1,    69,    -1,    -1,    -1,    -1,    -1,
    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    94,
    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   163,    -1,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,   161,    71,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,   183,   230,    -1,    -1,   233,   234,
    -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,     6,    -1,    -1,     9,   260,   261,   262,    13,   218,
   219,    -1,   267,   268,    -1,    20,    -1,    -1,   273,    -1,
   275,    -1,    -1,   141,   279,    -1,   235,    32,    -1,    -1,
    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
    -1,    66,    -1,   181,    69,    -1,    -1,    -1,    -1,   187,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    94,
    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,   227,
   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,
   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    71,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
    -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,    -1,
    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,   273,   181,
   275,    57,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,
    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
    -1,   243,   118,    -1,   246,   247,   248,   249,   250,    -1,
    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,
    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
   236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
   246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
    -1,    -1,    13,    -1,   260,   261,   262,    -1,    -1,    20,
    -1,   267,   268,    -1,    -1,    -1,    -1,   273,   155,   275,
    -1,    32,    -1,   279,    35,    -1,    -1,    38,    39,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
   237,   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   273,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    -1,    -1,   108,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
   261,   262,    -1,    -1,    -1,    -1,   267,   268,   164,    -1,
    -1,    -1,   273,    -1,   275,    57,    -1,    -1,   279,    -1,
    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
   216,   217,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
    -1,   227,   228,    -1,   230,    -1,   118,    -1,    -1,    -1,
    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
    13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
   242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
   262,    -1,    -1,   172,    -1,   267,   268,    -1,    -1,    -1,
    -1,   273,    -1,   275,    57,    -1,    -1,   279,    -1,    -1,
    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
    -1,   240,   241,    -1,   243,   118,    -1,   246,   247,   248,
   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
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
    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,
   273,    -1,   275,    57,    -1,    -1,   279,    -1,    -1,    -1,
    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
   240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
    -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
   224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
   234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
   254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,   273,
    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,   198,   254,   255,    -1,    -1,    -1,    -1,   260,
   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,    -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,
    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
   165,   237,   238,    -1,   240,   241,   171,   243,    -1,    -1,
   246,   247,   248,   249,   250,    -1,   181,    -1,   254,   255,
    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,   273,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
    -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,    -1,
    -1,    -1,   267,   268,   269,    -1,    -1,    -1,   273,    -1,
   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,
    66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,   195,
   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
   226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
   236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
   246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
    -1,    38,    39,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,   267,   268,    -1,    -1,    -1,    -1,   273,    -1,   275,
    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,
    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
   217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,
   237,   238,    20,   240,   241,   242,   243,    -1,    -1,   246,
   247,   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,
    38,    39,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
   267,   268,    -1,    -1,    -1,    -1,   273,    -1,   275,    -1,
    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,
    -1,    69,   196,    -1,   198,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,   198,
   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
    -1,    -1,    -1,    -1,   268,    -1,    -1,   216,   217,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
    -1,   230,    -1,    -1,    -1,    -1,    -1,   165,   237,   238,
    -1,   240,   241,   171,   243,    -1,    -1,   246,   247,   248,
   249,   250,    -1,   181,    -1,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,   195,   196,   197,
   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,
   238,    20,   240,   241,   242,   243,    -1,    -1,   246,   247,
   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,
    39,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,
   268,    -1,    -1,    -1,    -1,   273,   274,   275,    -1,    -1,
    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,
    -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,   198,
   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
   219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
    20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,
    -1,    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,
   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
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
   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
   220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
   230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
   240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,    -1,
    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,
    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
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
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
   261,   262,    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,
   271,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,
    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
    13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
   242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
   262,    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,
    -1,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,
    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    51,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,
    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,   181,    80,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
   223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
   233,   234,    -1,   236,   237,   238,   137,   240,   241,   242,
   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
    16,   254,   255,    -1,   155,    -1,    -1,   260,   261,   262,
    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,   170,
   273,    -1,   275,    -1,    -1,    -1,   279,    -1,    44,   180,
    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,   189,    55,
    56,    -1,    58,    -1,    -1,    -1,    -1,   198,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
    -1,    -1,    -1,    -1,    80,   216,   217,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
    -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,   256,   257,   258,    16,   260,
   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,   155,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,
    58,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   180,    -1,    74,    -1,    -1,    -1,
    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,   137,
   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
   256,   257,   258,    -1,   260,   261,   262,   155,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   198,    -1,    18,    -1,    -1,    21,    -1,    -1,    -1,    -1,
    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,   227,
   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,   237,
   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,   257,
   258,    -1,   260,   261,   262,    81,    -1,    -1,    -1,    -1,
    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
    -1,    -1,    -1,    18,    -1,    -1,    21,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   161,    81,    -1,    -1,    -1,
    -1,    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,
    95,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
   115,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,   144,
    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,
    -1,   237,   238,    -1,   240,   241,   161,   243,    -1,    -1,
   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   183,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
   235,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,   198,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,
   207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
   217,   218,   219,   220,   221,   222,    -1,    -1,   225,   226,
   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,    -1,
   237,   238,   239,   240,   241,   242,   243,   244,    -1,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
   257,   258,   259,   260,   261,   262,   263
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(DSQL_yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (DSQL_yychar == YYEMPTY && yylen == 1)				\
    { DSQL_yychar = (token), yylval = (value);			\
      DSQL_yychar1 = YYTRANSLATE (DSQL_yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	DSQL_yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

static int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into dsql_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int dsql_yyparse (void *);
#else
int dsql_yyparse (void);
#endif
#endif

int
dsql_yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *DSQL_DSQL_yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int DSQL_yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	DSQL_yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *DSQL_yyss = DSQL_yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, DSQL_DSQL_yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, DSQL_DSQL_yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int DSQL_yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (lex.dsql_debug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  DSQL_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  DSQL_DSQL_yyssp = DSQL_yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++DSQL_DSQL_yyssp = yystate;

  if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *DSQL_yyss1 = DSQL_yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = DSQL_DSQL_yyssp - DSQL_yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &DSQL_yyss1, size * sizeof (*DSQL_DSQL_yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &DSQL_yyss1, size * sizeof (*DSQL_DSQL_yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      DSQL_yyss = DSQL_yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (DSQL_yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      DSQL_yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*DSQL_DSQL_yyssp));
      __yy_memcpy ((char *)DSQL_yyss, (char *)DSQL_yyss1,
		   size * (unsigned int) sizeof (*DSQL_DSQL_yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      DSQL_DSQL_yyssp = DSQL_yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (lex.dsql_debug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (lex.dsql_debug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
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
#if YYDEBUG != 0
      if (lex.dsql_debug)
	fprintf(stderr, "Reading a token: ");
#endif
      DSQL_yychar = YYLEX;
    }

  /* Convert token to internal form (in DSQL_yychar1) for indexing tables with */

  if (DSQL_yychar <= 0)		/* This means end of input. */
    {
      DSQL_yychar1 = 0;
      DSQL_yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (lex.dsql_debug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      DSQL_yychar1 = YYTRANSLATE(DSQL_yychar);

#if YYDEBUG != 0
      if (lex.dsql_debug)
	{
	  fprintf (stderr, "Next token is %d (%s", DSQL_yychar, yytname[DSQL_yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, DSQL_yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
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

#if YYDEBUG != 0
  if (lex.dsql_debug)
    fprintf(stderr, "Shifting token %d (%s), ", DSQL_yychar, yytname[DSQL_yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (DSQL_yychar != YYEOF)
    DSQL_yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (lex.dsql_debug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{ DSQL_parse = yyvsp[0]; ;
    break;}
case 2:
{ DSQL_parse = yyvsp[-1]; ;
    break;}
case 22:
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); ;
    break;}
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); ;
    break;}
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); ;
    break;}
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); ;
    break;}
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); ;
    break;}
case 27:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); ;
    break;}
case 29:
{ yyval = yyvsp[0]; ;
    break;}
case 30:
{ yyval = make_node (nod_all, (int) 0, NULL); ;
    break;}
case 31:
{ yyval = make_node (nod_all, (int) 0, NULL); ;
    break;}
case 32:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 34:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 35:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); ;
    break;}
case 36:
{ yyval = make_node (nod_select, (int) 0, NULL); ;
    break;}
case 37:
{ yyval = make_node (nod_insert, (int) 0, NULL); ;
    break;}
case 38:
{ yyval = make_node (nod_delete, (int) 0, NULL); ;
    break;}
case 39:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); ;
    break;}
case 40:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); ;
    break;}
case 41:
{ yyval = make_node (nod_grant, (int) 0, NULL); ;
    break;}
case 42:
{ yyval = 0; ;
    break;}
case 43:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); ;
    break;}
case 44:
{ yyval = 0; ;
    break;}
case 45:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); ;
    break;}
case 46:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); ;
    break;}
case 47:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); ;
    break;}
case 48:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); ;
    break;}
case 49:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); ;
    break;}
case 50:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); ;
    break;}
case 51:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); ;
    break;}
case 52:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); ;
    break;}
case 53:
{ yyval = make_node (nod_grant, (int) 0, NULL); ;
    break;}
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 57:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 58:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); ;
    break;}
case 59:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); ;
    break;}
case 60:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); ;
    break;}
case 61:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); ;
    break;}
case 63:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 64:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); ;
    break;}
case 65:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); ;
    break;}
case 66:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); ;
    break;}
case 68:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 69:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); ;
    break;}
case 71:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); ;
    break;}
case 73:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); ;
    break;}
case 74:
{ yyval = yyvsp[0];;
    break;}
case 75:
{ yyval = yyvsp[0]; ;
    break;}
case 76:
{ yyval = yyvsp[0]; ;
    break;}
case 77:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); ;
    break;}
case 79:
{ lex.g_field->fld_dtype = dtype_blob; ;
    break;}
case 80:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; ;
    break;}
case 81:
{ yyval = NULL; ;
    break;}
case 83:
{ yyval = yyvsp[-1]; ;
    break;}
case 85:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 86:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); ;
    break;}
case 87:
{ yyval = NULL; ;
    break;}
case 88:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); ;
    break;}
case 89:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); ;
    break;}
case 90:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); ;
    break;}
case 92:
{ yyval = yyvsp[-1]; ;
    break;}
case 93:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); ;
    break;}
case 94:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));;
    break;}
case 95:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); ;
    break;}
case 96:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); ;
    break;}
case 97:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); ;
    break;}
case 98:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); ;
    break;}
case 99:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); ;
    break;}
case 100:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); ;
    break;}
case 101:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); ;
    break;}
case 102:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 103:
{ yyval = yyvsp[0]; ;
    break;}
case 104:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); ;
    break;}
case 105:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); ;
    break;}
case 106:
{ yyval = yyvsp[0]; ;
    break;}
case 107:
{ yyval = yyvsp[0]; ;
    break;}
case 108:
{ yyval = yyvsp[0]; ;
    break;}
case 109:
{ yyval = yyvsp[0]; ;
    break;}
case 110:
{ yyval = yyvsp[0]; ;
    break;}
case 111:
{ yyval = yyvsp[0]; ;
    break;}
case 112:
{ yyval = yyvsp[0]; ;
    break;}
case 113:
{ yyval = yyvsp[0]; ;
    break;}
case 114:
{ yyval = yyvsp[0]; ;
    break;}
case 115:
{ yyval = yyvsp[0]; ;
    break;}
case 116:
{ yyval = yyvsp[0]; ;
    break;}
case 117:
{ yyval = yyvsp[0]; ;
    break;}
case 118:
{ yyval = yyvsp[0]; ;
    break;}
case 119:
{ yyval = yyvsp[0]; ;
    break;}
case 120:
{ yyval = yyvsp[0]; ;
    break;}
case 121:
{ yyval = yyvsp[0]; ;
    break;}
case 122:
{ yyval = make_node (nod_unique, 0, NULL); ;
    break;}
case 123:
{ yyval = NULL; ;
    break;}
case 124:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 126:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 127:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); ;
    break;}
case 128:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 129:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 130:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 131:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 132:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 133:
{ yyval = (dsql_nod*) 0;;
    break;}
case 134:
{ yyval = yyvsp[-1]; ;
    break;}
case 135:
{ yyval = NULL; ;
    break;}
case 138:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 139:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); ;
    break;}
case 140:
{ yyval = NULL; ;
    break;}
case 141:
{ yyval = NULL; ;
    break;}
case 142:
{ yyval = yyvsp[0]; ;
    break;}
case 143:
{ yyval = NULL; ;
    break;}
case 144:
{ yyval = NULL; ;
    break;}
case 147:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 148:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);;
    break;}
case 151:
{ yyval = make_node (nod_null, (int) 0, NULL); ;
    break;}
case 152:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); ;
    break;}
case 153:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); ;
    break;}
case 154:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); ;
    break;}
case 155:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));;
    break;}
case 158:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; ;
    break;}
case 159:
{yyval = NULL;;
    break;}
case 162:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 163:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);;
    break;}
case 164:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);;
    break;}
case 165:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);;
    break;}
case 166:
{ yyval = make_node (nod_password, 1, yyvsp[0]);;
    break;}
case 167:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);;
    break;}
case 168:
{yyval = NULL;;
    break;}
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 173:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);;
    break;}
case 174:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); ;
    break;}
case 175:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); ;
    break;}
case 176:
{ lex.g_file  = make_file();;
    break;}
case 181:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];;
    break;}
case 182:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];;
    break;}
case 189:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); ;
    break;}
case 190:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); ;
    break;}
case 191:
{ yyval = yyvsp[0]; ;
    break;}
case 192:
{ yyval = yyvsp[0]; ;
    break;}
case 193:
{ yyval = NULL; ;
    break;}
case 195:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 198:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); ;
    break;}
case 199:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 200:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 201:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 204:
{ yyval = NULL; ;
    break;}
case 205:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); ;
    break;}
case 206:
{ yyval = yyvsp[0]; ;
    break;}
case 207:
{ yyval = NULL; ;
    break;}
case 208:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; ;
    break;}
case 209:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; ;
    break;}
case 210:
{ yyval = yyvsp[-1]; ;
    break;}
case 211:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; ;
    break;}
case 212:
{ yyval = yyvsp[0]; ;
    break;}
case 213:
{ yyval = NULL; ;
    break;}
case 220:
{ yyval = NULL; ;
    break;}
case 223:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 224:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);;
    break;}
case 225:
{ yyval = make_node (nod_null, (int) 1, NULL); ;
    break;}
case 226:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 228:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); ;
    break;}
case 229:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); ;
    break;}
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);;
    break;}
case 231:
{ yyval = yyvsp[0]; ;
    break;}
case 232:
{ yyval = NULL ;;
    break;}
case 237:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 238:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); ;
    break;}
case 239:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 240:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); ;
    break;}
case 241:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); ;
    break;}
case 242:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); ;
    break;}
case 243:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);;
    break;}
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);;
    break;}
case 245:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); ;
    break;}
case 246:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);;
    break;}
case 247:
{ yyval = NULL;;
    break;}
case 248:
{ yyval = yyvsp[0];;
    break;}
case 249:
{ yyval = yyvsp[0];;
    break;}
case 250:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);;
    break;}
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);;
    break;}
case 252:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);;
    break;}
case 253:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);;
    break;}
case 254:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 255:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 256:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 257:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 258:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 259:
{ yyval = NULL; ;
    break;}
case 260:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 261:
{ yyval = NULL; ;
    break;}
case 263:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 264:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); ;
    break;}
case 266:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 267:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); ;
    break;}
case 268:
{ yyval = yyvsp[0]; ;
    break;}
case 269:
{ yyval = yyvsp[0]; ;
    break;}
case 270:
{ yyval = (dsql_nod*) NULL; ;
    break;}
case 271:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 272:
{ yyval = NULL; ;
    break;}
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 275:
{ yyval = yyvsp[-1]; ;
    break;}
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); ;
    break;}
case 279:
{ yyval = NULL; ;
    break;}
case 280:
{ yyval = NULL; ;
    break;}
case 281:
{ yyval = yyvsp[0]; ;
    break;}
case 282:
{ yyval = yyvsp[0]; ;
    break;}
case 283:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); ;
    break;}
case 286:
{ yyval = yyvsp[-1]; ;
    break;}
case 287:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); ;
    break;}
case 288:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); ;
    break;}
case 289:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);;
    break;}
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 307:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); ;
    break;}
case 308:
{ yyval = make_node (nod_exit, 0, NULL); ;
    break;}
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); ;
    break;}
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 314:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); ;
    break;}
case 315:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); ;
    break;}
case 316:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); ;
    break;}
case 317:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); ;
    break;}
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); ;
    break;}
case 319:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); ;
    break;}
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); ;
    break;}
case 321:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); ;
    break;}
case 322:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 323:
{ yyval = NULL; ;
    break;}
case 324:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); ;
    break;}
case 325:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); ;
    break;}
case 326:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 327:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 328:
{ yyval = NULL; ;
    break;}
case 329:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 330:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 331:
{ yyval = NULL; ;
    break;}
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 336:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); ;
    break;}
case 337:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); ;
    break;}
case 338:
{ yyval = NULL; ;
    break;}
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); ;
    break;}
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); ;
    break;}
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); ;
    break;}
case 342:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); ;
    break;}
case 343:
{ yyval = NULL; ;
    break;}
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 346:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); ;
    break;}
case 348:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 349:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); ;
    break;}
case 350:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); ;
    break;}
case 351:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); ;
    break;}
case 352:
{ yyval = make_node (nod_default, 1, NULL); ;
    break;}
case 356:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); ;
    break;}
case 357:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); ;
    break;}
case 358:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); ;
    break;}
case 359:
{ yyval = NULL; ;
    break;}
case 360:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); ;
    break;}
case 361:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); ;
    break;}
case 362:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 363:
{ yyval = NULL; ;
    break;}
case 365:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 366:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); ;
    break;}
case 367:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 368:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 369:
{ lex.beginning = lex_position(); ;
    break;}
case 370:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);;
    break;}
case 371:
{ lex.beginning = lex.last_token; ;
    break;}
case 372:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); ;
    break;}
case 373:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); ;
    break;}
case 374:
{ yyval = 0; ;
    break;}
case 375:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 377:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 378:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 379:
{ yyval = NULL; ;
    break;}
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); ;
    break;}
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); ;
    break;}
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); ;
    break;}
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); ;
    break;}
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); ;
    break;}
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); ;
    break;}
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); ;
    break;}
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); ;
    break;}
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); ;
    break;}
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); ;
    break;}
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); ;
    break;}
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); ;
    break;}
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); ;
    break;}
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); ;
    break;}
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); ;
    break;}
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); ;
    break;}
case 398:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 399:
{ yyval = NULL; ;
    break;}
case 400:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); ;
    break;}
case 401:
{ yyval = yyvsp[0]; ;
    break;}
case 402:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); ;
    break;}
case 403:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); ;
    break;}
case 404:
{ yyval = yyvsp[0]; ;
    break;}
case 405:
{ yyval = yyvsp[0]; ;
    break;}
case 406:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); ;
    break;}
case 407:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); ;
    break;}
case 408:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); ;
    break;}
case 409:
{ yyval = yyvsp[0]; ;
    break;}
case 410:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); ;
    break;}
case 412:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 413:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); ;
    break;}
case 414:
{ yyval = yyvsp[0]; ;
    break;}
case 415:
{ yyval = yyvsp[0]; ;
    break;}
case 416:
{yyval = make_node (nod_del_default, (int) 0, NULL); ;
    break;}
case 417:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); ;
    break;}
case 418:
{ yyval = yyvsp[0]; ;
    break;}
case 419:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); ;
    break;}
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 422:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);;
    break;}
case 424:
{ yyval = yyvsp[0]; ;
    break;}
case 425:
{ yyval = yyvsp[0]; ;
    break;}
case 426:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); ;
    break;}
case 427:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 428:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); ;
    break;}
case 429:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); ;
    break;}
case 457:
{ yyval = NULL; ;
    break;}
case 458:
{ yyval = NULL; ;
    break;}
case 459:
{ yyval = NULL; ;
    break;}
case 460:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); ;
    break;}
case 461:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; ;
    break;}
case 462:
{ yyval = make_node (nod_restrict, 0, NULL); ;
    break;}
case 463:
{ yyval = make_node (nod_cascade, 0, NULL); ;
    break;}
case 464:
{ yyval = make_node (nod_restrict, 0, NULL); ;
    break;}
case 465:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); ;
    break;}
case 466:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); ;
    break;}
case 467:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; ;
    break;}
case 469:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 470:
{ yyval = yyvsp[0]; ;
    break;}
case 471:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); ;
    break;}
case 472:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); ;
    break;}
case 473:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); ;
    break;}
case 474:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); ;
    break;}
case 475:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 477:
{ yyval = NULL; ;
    break;}
case 479:
{ yyval = NULL; ;
    break;}
case 480:
{ yyval = yyvsp[0]; ;
    break;}
case 481:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); ;
    break;}
case 482:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); ;
    break;}
case 483:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); ;
    break;}
case 484:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); ;
    break;}
case 485:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); ;
    break;}
case 486:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); ;
    break;}
case 487:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); ;
    break;}
case 488:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); ;
    break;}
case 489:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); ;
    break;}
case 490:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); ;
    break;}
case 491:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); ;
    break;}
case 492:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); ;
    break;}
case 497:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; ;
    break;}
case 498:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; ;
    break;}
case 500:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 501:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); ;
    break;}
case 502:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); ;
    break;}
case 508:
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
			;
    break;}
case 509:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			;
    break;}
case 510:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			;
    break;}
case 511:
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
			;
    break;}
case 512:
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
			;
    break;}
case 513:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			;
    break;}
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			;
    break;}
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			;
    break;}
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			;
    break;}
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			;
    break;}
case 520:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	;
    break;}
case 521:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	;
    break;}
case 522:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			;
    break;}
case 523:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			;
    break;}
case 524:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			;
    break;}
case 525:
{
			lex.g_field->fld_character_set = yyvsp[0];
			;
    break;}
case 527:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 529:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 530:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			;
    break;}
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			;
    break;}
case 532:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			;
    break;}
case 541:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			;
    break;}
case 542:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			;
    break;}
case 543:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				;
    break;}
case 544:
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
			;
    break;}
case 545:
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
			;
    break;}
case 548:
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
			;
    break;}
case 549:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			;
    break;}
case 550:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			;
    break;}
case 551:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			;
    break;}
case 552:
{ yyval = yyvsp[-1]; ;
    break;}
case 553:
{ yyval = 0; ;
    break;}
case 557:
{ 
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			;
    break;}
case 558:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			;
    break;}
case 559:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			;
    break;}
case 563:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); ;
    break;}
case 564:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 565:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 566:
{ yyval = 0; ;
    break;}
case 567:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); ;
    break;}
case 570:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); ;
    break;}
case 571:
{ yyval = make_node (nod_rollback, 0, NULL); ;
    break;}
case 574:
{ yyval = make_node (nod_commit_retain, 0, NULL); ;
    break;}
case 575:
{ yyval = NULL; ;
    break;}
case 577:
{ yyval = NULL; ;
    break;}
case 578:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); ;
    break;}
case 580:
{ yyval = NULL; ;
    break;}
case 582:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 587:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); ;
    break;}
case 588:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); ;
    break;}
case 589:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); ;
    break;}
case 590:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); ;
    break;}
case 591:
{ yyval = yyvsp[0];;
    break;}
case 593:
{ yyval = yyvsp[0];;
    break;}
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); ;
    break;}
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); ;
    break;}
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); ;
    break;}
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); ;
    break;}
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); ;
    break;}
case 599:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); ;
    break;}
case 600:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); ;
    break;}
case 601:
{ yyval = 0; ;
    break;}
case 602:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); ;
    break;}
case 603:
{ yyval = (dsql_nod*) NOD_SHARED; ;
    break;}
case 604:
{ yyval = (dsql_nod*) NOD_PROTECTED ; ;
    break;}
case 605:
{ yyval = (dsql_nod*) 0; ;
    break;}
case 606:
{ yyval = (dsql_nod*) NOD_READ; ;
    break;}
case 607:
{ yyval = (dsql_nod*) NOD_WRITE; ;
    break;}
case 609:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 610:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); ;
    break;}
case 611:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); ;
    break;}
case 612:
{ yyval = 0; ;
    break;}
case 614:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 615:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); ;
    break;}
case 616:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 617:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); ;
    break;}
case 618:
{ yyval = NULL; ;
    break;}
case 619:
{ yyval = yyvsp[0]; ;
    break;}
case 620:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 621:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 622:
{ yyval = NULL; ;
    break;}
case 623:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 624:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 625:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 626:
{ yyval = make_node (nod_list, 1, yyvsp[0]); ;
    break;}
case 627:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 628:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 630:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 631:
{ lex.limit_clause = true; ;
    break;}
case 632:
{ lex.limit_clause = false; ;
    break;}
case 633:
{ lex.first_detection = true; ;
    break;}
case 634:
{ lex.first_detection = false; ;
    break;}
case 635:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); ;
    break;}
case 636:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); ;
    break;}
case 637:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); ;
    break;}
case 638:
{ yyval = 0; ;
    break;}
case 639:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); ;
    break;}
case 640:
{ yyval = yyvsp[-2]; ;
    break;}
case 641:
{ yyval = yyvsp[-1]; ;
    break;}
case 642:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 643:
{ yyval = yyvsp[-1]; ;
    break;}
case 644:
{ yyval = yyvsp[0]; ;
    break;}
case 645:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 646:
{ yyval = 0; ;
    break;}
case 647:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 648:
{ yyval = 0; ;
    break;}
case 650:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 652:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 655:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 657:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 662:
{ yyval = yyvsp[-1]; ;
    break;}
case 663:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); ;
    break;}
case 665:
{ yyval = NULL; ;
    break;}
case 666:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 667:
{ yyval = NULL; ;
    break;}
case 669:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 673:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); ;
    break;}
case 674:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); ;
    break;}
case 675:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); ;
    break;}
case 678:
{ yyval = yyvsp[0]; ;
    break;}
case 679:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); ;
    break;}
case 681:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); ;
    break;}
case 682:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 683:
{ yyval = NULL; ;
    break;}
case 685:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); ;
    break;}
case 686:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); ;
    break;}
case 687:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); ;
    break;}
case 688:
{ yyval = make_node (nod_join_left, (int) 0, NULL); ;
    break;}
case 689:
{ yyval = make_node (nod_join_right, (int) 0, NULL); ;
    break;}
case 690:
{ yyval = make_node (nod_join_full, (int) 0, NULL); ;
    break;}
case 691:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); ;
    break;}
case 694:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 695:
{ yyval = NULL; ;
    break;}
case 697:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 699:
{ yyval = yyvsp[0]; ;
    break;}
case 700:
{ yyval = NULL; ;
    break;}
case 701:
{ yyval = yyvsp[0]; ;
    break;}
case 702:
{ yyval = NULL; ;
    break;}
case 703:
{ yyval = yyvsp[0]; ;
    break;}
case 704:
{ yyval = NULL; ;
    break;}
case 705:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); ;
    break;}
case 706:
{ yyval = 0; ;
    break;}
case 707:
{ yyval = make_node (nod_merge, (int) 0, NULL); ;
    break;}
case 708:
{ yyval = make_node (nod_merge, (int) 0, NULL); ;
    break;}
case 709:
{ yyval = 0; ;
    break;}
case 710:
{ yyval = 0; ;
    break;}
case 712:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 713:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); ;
    break;}
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 717:
{ yyval = make_node (nod_natural, (int) 0, NULL); ;
    break;}
case 718:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); ;
    break;}
case 719:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 722:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 723:
{ yyval = 0; ;
    break;}
case 724:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 725:
{ yyval = 0; ;
    break;}
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 728:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 729:
{ yyval = 0; ;
    break;}
case 730:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 731:
{ yyval = 0; ;
    break;}
case 732:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); ;
    break;}
case 733:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); ;
    break;}
case 734:
{ yyval = yyvsp[-1]; ;
    break;}
case 735:
{ yyval = 0; ;
    break;}
case 736:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); ;
    break;}
case 737:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); ;
    break;}
case 738:
{ yyval = NULL; ;
    break;}
case 739:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); ;
    break;}
case 740:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); ;
    break;}
case 743:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 744:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 745:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); ;
    break;}
case 748:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 749:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 752:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); ;
    break;}
case 753:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); ;
    break;}
case 754:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 755:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 757:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); ;
    break;}
case 761:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 762:
{ yyval = yyvsp[0]; ;
    break;}
case 764:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 767:
{ yyval = NULL; ;
    break;}
case 768:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 772:
{ yyval = NULL; ;
    break;}
case 773:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); ;
    break;}
case 778:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); ;
    break;}
case 779:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); ;
    break;}
case 781:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); ;
    break;}
case 783:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 785:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 786:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 788:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 789:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 790:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 791:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 792:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 794:
{ yyval = yyvsp[-1]; ;
    break;}
case 795:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 807:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 808:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 809:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 810:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 811:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 812:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 813:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 814:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 815:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 816:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 817:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 818:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 819:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 820:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 821:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 822:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 823:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 824:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 825:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 826:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 827:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 828:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 829:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 830:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); ;
    break;}
case 834:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); ;
    break;}
case 835:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); ;
    break;}
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); ;
    break;}
case 837:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 839:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); ;
    break;}
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); ;
    break;}
case 841:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 843:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 845:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 847:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 848:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); ;
    break;}
case 849:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); ;
    break;}
case 850:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); ;
    break;}
case 851:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); ;
    break;}
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); ;
    break;}
case 853:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); ;
    break;}
case 854:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); ;
    break;}
case 855:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); ;
    break;}
case 856:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); ;
    break;}
case 857:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); ;
    break;}
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); ;
    break;}
case 860:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 861:
{ yyval = yyvsp[-1]; ;
    break;}
case 871:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); ;
    break;}
case 872:
{ yyval = yyvsp[0]; ;
    break;}
case 873:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 874:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 875:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); ;
    break;}
case 876:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 877:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 878:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 879:
{ yyval = yyvsp[-1]; ;
    break;}
case 880:
{ yyval = yyvsp[-1]; ;
    break;}
case 884:
{ yyval = make_node (nod_dbkey, 1, NULL); ;
    break;}
case 885:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); ;
    break;}
case 886:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						;
    break;}
case 889:
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
			;
    break;}
case 890:
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
			;
    break;}
case 891:
{ yyval = make_node (nod_current_timestamp, 0, NULL); ;
    break;}
case 892:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); ;
    break;}
case 894:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 896:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); ;
    break;}
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); ;
    break;}
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); ;
    break;}
case 900:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); ;
    break;}
case 901:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); ;
    break;}
case 903:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); ;
    break;}
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			;
    break;}
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
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			;
    break;}
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); ;
    break;}
case 907:
{ yyval = make_parameter (); ;
    break;}
case 908:
{ yyval = make_node (nod_user_name, 0, NULL); ;
    break;}
case 909:
{ yyval = make_node (nod_user_name, 0, NULL); ;
    break;}
case 910:
{ yyval = make_node (nod_current_role, 0, NULL); ;
    break;}
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); ;
    break;}
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); ;
    break;}
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); ;
    break;}
case 914:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); ;
    break;}
case 915:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); ;
    break;}
case 916:
{ yyval = yyvsp[0]; ;
    break;}
case 917:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; ;
    break;}
case 919:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; ;
    break;}
case 920:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 921:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 922:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];;
    break;}
case 923:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 925:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; ;
    break;}
case 926:
{ yyval = yyvsp[0];;
    break;}
case 931:
{ yyval = make_node (nod_agg_count, 0, NULL); ;
    break;}
case 932:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); ;
    break;}
case 933:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); ;
    break;}
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			;
    break;}
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			;
    break;}
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			;
    break;}
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			;
    break;}
case 938:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); ;
    break;}
case 939:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); ;
    break;}
case 940:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); ;
    break;}
case 941:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); ;
    break;}
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			;
    break;}
case 944:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 946:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); ;
    break;}
case 947:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); ;
    break;}
case 948:
{ yyval = yyvsp[0]; ;
    break;}
case 949:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); ;
    break;}
case 950:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 951:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); ;
    break;}
case 952:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); ;
    break;}
case 955:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); ;
    break;}
case 956:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 957:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 960:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); ;
    break;}
case 961:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 962:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 963:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); ;
    break;}
case 964:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); ;
    break;}
case 965:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 966:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 967:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); ;
    break;}
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); ;
    break;}
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); ;
    break;}
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); ;
    break;}
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); ;
    break;}
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); ;
    break;}
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); ;
    break;}
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); ;
    break;}
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); ;
    break;}
case 983:
{ yyval = make_node (nod_null, 0, NULL); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  DSQL_DSQL_yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (lex.dsql_debug)
    {
      short *ssp1 = DSQL_yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != DSQL_DSQL_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *DSQL_DSQL_yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *DSQL_DSQL_yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (DSQL_yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (lex.dsql_debug)
	fprintf(stderr, "Discarding token %d (%s).\n", DSQL_yychar, yytname[DSQL_yychar1]);
#endif

      DSQL_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (DSQL_DSQL_yyssp == DSQL_yyss) YYABORT;
  yyvsp--;
  yystate = *--DSQL_DSQL_yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (lex.dsql_debug)
    {
      short *ssp1 = DSQL_yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != DSQL_DSQL_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

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

#if YYDEBUG != 0
  if (lex.dsql_debug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (DSQL_yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (DSQL_yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
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
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
					LexerState savedState = lex;
					int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
					int token = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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

