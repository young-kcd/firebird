
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



#define	YYFINAL		1891
#define	YYFLAG		-32768
#define	YYNTBASE	280

#define YYTRANSLATE(x) ((unsigned)(x) <= 517 ? yytranslate[x] : 712)

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
   952,   956,   961,   964,   971,   976,   985,   995,  1002,  1011,
  1018,  1023,  1024,  1029,  1032,  1034,  1038,  1039,  1042,  1047,
  1048,  1050,  1052,  1056,  1060,  1068,  1071,  1072,  1075,  1078,
  1082,  1086,  1087,  1089,  1092,  1097,  1099,  1103,  1106,  1109,
  1112,  1114,  1116,  1118,  1120,  1123,  1126,  1132,  1133,  1138,
  1146,  1150,  1151,  1153,  1157,  1161,  1169,  1177,  1178,  1179,
  1180,  1181,  1185,  1186,  1196,  1206,  1208,  1210,  1211,  1214,
  1216,  1218,  1220,  1222,  1224,  1228,  1232,  1236,  1240,  1244,
  1248,  1254,  1260,  1266,  1272,  1278,  1284,  1287,  1288,  1293,
  1296,  1300,  1304,  1307,  1310,  1314,  1318,  1321,  1325,  1332,
  1334,  1337,  1342,  1346,  1349,  1352,  1355,  1358,  1362,  1364,
  1368,  1372,  1376,  1379,  1382,  1387,  1392,  1398,  1400,  1402,
  1404,  1406,  1408,  1410,  1412,  1414,  1416,  1418,  1420,  1422,
  1424,  1426,  1428,  1430,  1432,  1434,  1436,  1438,  1440,  1442,
  1444,  1446,  1448,  1450,  1452,  1454,  1456,  1459,  1462,  1465,
  1467,  1469,  1471,  1472,  1475,  1478,  1479,  1481,  1484,  1487,
  1492,  1496,  1499,  1502,  1510,  1512,  1513,  1515,  1516,  1519,
  1522,  1525,  1528,  1531,  1534,  1537,  1540,  1543,  1547,  1550,
  1553,  1556,  1558,  1560,  1562,  1564,  1569,  1575,  1577,  1581,
  1583,  1587,  1589,  1592,  1594,  1596,  1598,  1600,  1602,  1604,
  1606,  1608,  1610,  1612,  1614,  1619,  1624,  1631,  1637,  1641,
  1642,  1645,  1648,  1649,  1653,  1654,  1659,  1661,  1667,  1672,
  1674,  1679,  1681,  1684,  1687,  1689,  1691,  1693,  1696,  1699,
  1702,  1705,  1706,  1710,  1716,  1718,  1720,  1723,  1727,  1729,
  1732,  1736,  1737,  1739,  1741,  1743,  1749,  1755,  1762,  1764,
  1766,  1768,  1771,  1776,  1778,  1779,  1785,  1787,  1788,  1792,
  1795,  1797,  1798,  1801,  1802,  1804,  1805,  1809,  1811,  1812,
  1814,  1817,  1819,  1821,  1823,  1825,  1828,  1831,  1833,  1836,
  1840,  1842,  1844,  1848,  1852,  1854,  1857,  1861,  1863,  1866,
  1867,  1870,  1872,  1874,  1875,  1877,  1879,  1881,  1885,  1888,
  1892,  1893,  1895,  1899,  1904,  1908,  1912,  1913,  1916,  1917,
  1920,  1921,  1925,  1929,  1933,  1935,  1940,  1945,  1947,  1957,
  1958,  1959,  1960,  1961,  1965,  1968,  1970,  1971,  1975,  1981,
  1985,  1988,  1994,  1997,  1999,  2001,  2003,  2005,  2007,  2011,
  2013,  2017,  2019,  2020,  2023,  2025,  2029,  2031,  2033,  2035,
  2037,  2041,  2048,  2050,  2051,  2055,  2056,  2058,  2062,  2064,
  2066,  2068,  2073,  2079,  2085,  2087,  2089,  2092,  2097,  2102,
  2105,  2109,  2110,  2112,  2115,  2117,  2119,  2122,  2125,  2128,
  2129,  2131,  2132,  2136,  2137,  2139,  2143,  2145,  2148,  2149,
  2152,  2153,  2156,  2157,  2162,  2164,  2167,  2169,  2171,  2172,
  2174,  2178,  2181,  2183,  2185,  2188,  2190,  2195,  2199,  2201,
  2205,  2210,  2211,  2215,  2216,  2218,  2222,  2226,  2228,  2230,
  2231,  2233,  2235,  2240,  2241,  2244,  2249,  2250,  2259,  2265,
  2267,  2269,  2277,  2282,  2287,  2289,  2291,  2300,  2306,  2308,
  2312,  2316,  2324,  2332,  2338,  2342,  2343,  2345,  2347,  2349,
  2352,  2353,  2355,  2357,  2359,  2360,  2364,  2366,  2370,  2372,
  2373,  2377,  2379,  2383,  2385,  2389,  2393,  2395,  2397,  2401,
  2403,  2406,  2408,  2412,  2416,  2418,  2421,  2425,  2429,  2433,
  2437,  2439,  2443,  2446,  2448,  2450,  2452,  2454,  2456,  2458,
  2460,  2462,  2464,  2466,  2468,  2472,  2476,  2480,  2484,  2488,
  2492,  2496,  2500,  2507,  2514,  2521,  2528,  2535,  2542,  2549,
  2556,  2563,  2570,  2577,  2584,  2591,  2598,  2605,  2612,  2614,
  2616,  2622,  2629,  2635,  2642,  2646,  2651,  2657,  2664,  2668,
  2673,  2677,  2682,  2686,  2691,  2696,  2702,  2707,  2712,  2716,
  2721,  2723,  2725,  2727,  2729,  2731,  2733,  2735,  2739,  2743,
  2745,  2747,  2749,  2751,  2753,  2755,  2757,  2759,  2761,  2764,
  2767,  2771,  2775,  2779,  2783,  2787,  2791,  2795,  2799,  2801,
  2803,  2805,  2807,  2811,  2813,  2815,  2817,  2819,  2821,  2823,
  2828,  2830,  2834,  2836,  2839,  2841,  2843,  2845,  2847,  2849,
  2851,  2853,  2856,  2859,  2862,  2864,  2866,  2868,  2870,  2872,
  2874,  2876,  2878,  2880,  2882,  2885,  2887,  2890,  2892,  2894,
  2896,  2898,  2900,  2903,  2905,  2907,  2909,  2911,  2913,  2918,
  2924,  2930,  2936,  2942,  2948,  2954,  2960,  2966,  2972,  2978,
  2985,  2987,  2994,  2996,  3001,  3009,  3012,  3013,  3018,  3022,
  3029,  3031,  3033,  3040,  3049,  3056,  3058,  3060,  3065,  3072,
  3077,  3083,  3087,  3093,  3098,  3104,  3106,  3108,  3110,  3112,
  3114,  3116,  3118,  3120,  3122,  3124,  3126,  3128,  3129,  3131,
  3132,  3134,  3136,  3138,  3140,  3142,  3144,  3146,  3148,  3150,
  3152,  3154,  3156,  3158,  3160,  3162,  3164,  3166,  3168,  3170,
  3172,  3174,  3176,  3178,  3180,  3182,  3184,  3186,  3188,  3190,
  3192,  3194,  3196,  3198,  3200,  3202,  3204,  3206,  3208,  3210,
  3212,  3214,  3216,  3218,  3220,  3222,  3224
};

static const short yyrhs[] = {   281,
     0,   281,   271,     0,   459,     0,   609,     0,   512,     0,
   314,     0,   301,     0,   600,     0,   480,     0,   282,     0,
   599,     0,   439,     0,   440,     0,   316,     0,   318,     0,
   291,     0,   513,     0,   506,     0,   535,     0,   504,     0,
   604,     0,    40,   655,     0,    70,   284,   122,   283,   174,
   295,   288,     0,    70,   286,   122,   141,   290,   174,   295,
   288,     0,    70,   284,   122,   283,   174,   293,     0,    70,
   286,   122,   141,   290,   174,   293,     0,    70,   297,   174,
   299,   289,     0,   575,     0,   172,   575,     0,     6,     0,
     6,   140,     0,   285,     0,   287,     0,   285,   272,   287,
     0,    56,     0,   155,     0,    80,     0,    44,     0,   180,
   614,     0,   145,   614,     0,   192,    70,   124,     0,     0,
   192,   202,   124,     0,     0,   701,     0,   150,   292,   284,
   122,   283,    63,   295,     0,   150,   292,   286,   122,   141,
   290,    63,   295,     0,   150,   284,   122,   283,    63,   295,
     0,   150,   286,   122,   141,   290,    63,   295,     0,   150,
   284,   122,   283,    63,   293,     0,   150,   286,   122,   141,
   290,    63,   293,     0,   150,   297,    63,   299,     0,    70,
   124,    61,     0,   294,     0,   293,   272,   294,     0,   293,
   272,   296,     0,   295,   272,   294,     0,   141,   701,     0,
   176,   705,     0,   187,   708,     0,   206,   702,     0,   296,
     0,   295,   272,   296,     0,   706,     0,   181,   706,     0,
    71,   706,     0,   298,     0,   297,   272,   298,     0,   702,
     0,   300,     0,   299,   272,   300,     0,   706,     0,   181,
   706,     0,    42,   302,     0,    60,   312,     0,    59,    65,
   303,     0,   686,   305,   149,   309,    52,   654,   111,   654,
     0,   487,     0,    18,     0,    34,   273,   658,   274,   493,
     0,     0,   306,     0,   273,   306,   274,     0,   307,     0,
   306,   272,   307,     0,   368,   304,   308,     0,     0,    19,
   230,     0,    19,   262,     0,    94,     0,   310,     0,   273,
   310,   274,     0,   368,   304,   311,     0,   133,   658,     0,
     0,    19,    97,     0,    19,   230,     0,   204,     0,    19,
   230,   204,     0,   695,    78,   313,   128,   313,    52,   654,
   111,   654,     0,   687,     0,   655,     0,    33,   315,     0,
    55,   694,   654,     0,   320,   595,    79,   698,   122,   575,
   321,     0,   141,   387,     0,   172,   355,     0,   176,   451,
     0,   187,   444,     0,    68,   337,     0,    37,   339,     0,
    48,   328,     0,   157,   322,     0,   206,   338,     0,   229,
   317,     0,   141,   388,     0,   172,   356,     0,   187,   445,
     0,    33,   125,     7,   319,     0,   141,   389,     0,   176,
   452,     0,   179,     0,     0,   616,     0,   615,     0,   362,
   273,   448,   643,   449,   274,     0,   658,   323,   324,   654,
   325,   326,     0,   105,     0,    14,     0,     0,     0,    29,
     0,     0,    98,   340,   661,   354,     0,     0,   327,     0,
   348,     0,   327,   348,     0,   365,   329,   482,   448,   330,
   449,   331,   364,     0,    10,     0,     0,    43,   448,   370,
     0,     0,     0,   332,     0,   333,     0,   332,   333,     0,
   334,     0,   335,     0,   336,     0,   118,    94,     0,   448,
    22,   273,   623,   274,   449,     0,   697,     0,   702,     0,
   341,   342,   345,     0,     0,   264,     0,   654,     0,     0,
   343,     0,   344,     0,   343,   344,     0,   132,   340,   658,
     0,    98,   340,   661,   354,     0,   181,   654,     0,   134,
   654,     0,   156,   112,   654,     0,     0,   346,     0,   347,
     0,   346,   347,     0,   348,     0,    43,    21,   156,   688,
     0,   255,    90,   654,     0,   349,   654,   350,     0,    90,
     0,     0,   351,     0,   352,     0,   351,   352,     0,   167,
   353,   661,     0,    98,   340,   661,   354,     0,     0,    12,
     0,    12,   130,     0,     0,   130,     0,   131,     0,   575,
   357,   273,   358,   274,     0,   575,   357,   273,   358,   274,
     0,    59,    90,   654,     0,    59,   654,     0,     0,   359,
     0,   358,   272,   359,     0,   360,     0,   375,     0,   365,
   363,   369,   449,   371,   364,     0,   365,   483,   361,     0,
   365,   361,     0,   362,   273,   448,   643,   449,   274,     0,
    27,    19,     0,    27,     0,   482,   448,     0,   621,   446,
     0,    23,   689,     0,     0,   621,     0,   621,     0,   368,
   482,     0,     0,    43,   370,     0,     0,   647,     0,   651,
     0,   652,     0,   653,     0,   685,     0,   644,     0,     0,
   372,     0,   373,     0,   372,   373,     0,   376,   374,     0,
   118,    94,     0,   145,   575,   614,   383,   381,     0,   382,
     0,   179,   381,     0,   139,    86,   381,     0,   376,   377,
     0,    30,   691,     0,     0,   378,     0,   379,     0,   380,
     0,   382,     0,   179,   615,   381,     0,   139,    86,   615,
   381,     0,    62,    86,   615,   145,   575,   614,   383,   381,
     0,   239,   595,    79,   698,     0,     0,   448,    22,   273,
   623,   274,   449,     0,   384,     0,   385,     0,   385,   384,
     0,   384,   385,     0,     0,   122,   180,   386,     0,   122,
    44,   386,     0,   203,     0,   156,    43,     0,   156,    94,
     0,   117,   201,     0,   701,   391,   392,    10,   446,   398,
   407,   449,     0,   701,   391,   392,    10,   446,   398,   407,
   449,     0,   701,   391,   392,    10,   446,   398,   407,   449,
     0,   701,   391,   392,    10,   446,   398,   407,   449,     0,
   273,   393,   274,     0,     0,   149,   273,   395,   274,     0,
     0,   394,     0,   393,   272,   394,     0,   366,   483,   397,
   449,     0,   396,     0,   395,   272,   396,     0,   366,   483,
     0,    43,   446,   370,     0,   264,   446,   370,     0,   446,
     0,   399,     0,     0,   400,     0,   399,   400,     0,    42,
   403,   401,   271,     0,   402,     0,   405,     0,   365,   483,
   404,     0,   184,     0,     0,   264,   370,     0,   369,     0,
   692,    36,    61,   273,   535,   274,     0,   410,     0,   407,
     0,    16,   408,    51,     0,   409,     0,   409,   430,     0,
     0,   406,     0,   409,   406,     0,   608,   271,     0,   600,
   271,     0,   411,     0,   412,     0,   413,     0,   414,     0,
   415,     0,   418,     0,   599,   271,     0,   419,     0,   421,
     0,   604,   271,     0,   426,     0,   416,     0,   417,     0,
   170,   271,     0,    58,   271,     0,   428,     0,   434,   271,
     0,    55,   694,   271,     0,    55,   694,   643,   271,     0,
    55,   271,     0,    56,   141,   701,   423,   424,   271,     0,
    56,   246,   643,   271,     0,   427,    61,   535,    82,   425,
   429,    47,   406,     0,   427,    61,    56,   246,   643,    82,
   425,    47,   406,     0,    56,   246,   643,    82,   425,   271,
     0,    74,   273,   623,   274,   173,   406,    50,   406,     0,
    74,   273,   623,   274,   173,   406,     0,   137,   643,   420,
   271,     0,     0,   535,    82,   425,   271,     0,   275,   707,
     0,   646,     0,   273,   646,   274,     0,     0,   148,   425,
     0,   148,   273,   425,   274,     0,     0,   422,     0,   620,
     0,   425,   272,   620,     0,   425,   272,   422,     0,   427,
   191,   273,   623,   274,    47,   406,     0,   700,   275,     0,
     0,   227,   271,     0,   247,   271,     0,   247,   700,   271,
     0,    10,    36,   692,     0,     0,   431,     0,   430,   431,
     0,   189,   432,    47,   406,     0,   433,     0,   432,   272,
   433,     0,   165,   655,     0,    66,   696,     0,    55,   694,
     0,     9,     0,   435,     0,   437,     0,   436,     0,   256,
   692,     0,   257,   692,     0,   258,   438,   692,    82,   425,
     0,     0,    56,   141,   701,   423,     0,    56,   260,   441,
   392,    10,   398,   407,     0,   273,   442,   274,     0,     0,
   443,     0,   442,   272,   443,     0,   396,   264,   650,     0,
   708,   614,    10,   446,   539,   450,   447,     0,   708,   614,
    10,   446,   539,   450,   447,     0,     0,     0,     0,     0,
   192,    22,   124,     0,     0,   705,    61,   575,   453,   454,
   457,   448,   458,   449,     0,   705,    61,   575,   453,   454,
   457,   448,   458,   449,     0,     3,     0,    76,     0,     0,
   455,   456,     0,    15,     0,     5,     0,    80,     0,   180,
     0,    44,     0,    80,   125,   180,     0,    80,   125,    44,
     0,   180,   125,    80,     0,   180,   125,    44,     0,    44,
   125,    80,     0,    44,   125,   180,     0,    80,   125,   180,
   125,    44,     0,    80,   125,    44,   125,   180,     0,   180,
   125,    80,   125,    44,     0,   180,   125,    44,   125,    80,
     0,    44,   125,    80,   125,   180,     0,    44,   125,   180,
   125,    80,     0,   136,   656,     0,     0,    10,   448,   398,
   407,     0,     7,   460,     0,    55,   694,   654,     0,   172,
   575,   465,     0,   176,   477,     0,   141,   390,     0,    37,
   474,   475,     0,    48,   467,   463,     0,    79,   473,     0,
    43,   448,   370,     0,    22,   448,   273,   623,   274,   449,
     0,   464,     0,   463,   464,     0,   156,   448,   461,   449,
     0,     4,    30,   462,     0,     4,   336,     0,    49,    43,
     0,    49,    30,     0,   174,   621,     0,   208,   368,   483,
     0,   466,     0,   465,   272,   466,     0,    49,   621,   472,
     0,    49,    30,   691,     0,     4,   360,     0,     4,   375,
     0,   469,   621,   136,   658,     0,   469,   467,   174,   621,
     0,   469,   471,   208,   470,   449,     0,   468,     0,   710,
     0,   207,     0,   208,     0,   209,     0,   210,     0,   211,
     0,   212,     0,   213,     0,   214,     0,   215,     0,   218,
     0,   219,     0,   220,     0,   221,     0,   222,     0,   225,
     0,   226,     0,   233,     0,   234,     0,   242,     0,   244,
     0,   256,     0,   257,     0,   258,     0,   259,     0,   239,
     0,   263,     0,     7,     0,     7,   207,     0,   483,   448,
     0,   621,   446,     0,   621,     0,   205,     0,   203,     0,
     0,   698,     3,     0,   698,    76,     0,     0,   476,     0,
   475,   476,     0,     4,   327,     0,     4,   255,    90,   654,
     0,    49,   255,    90,     0,    16,   254,     0,    51,   254,
     0,   705,   453,   478,   457,   448,   479,   449,     0,   454,
     0,     0,   458,     0,     0,    49,   481,     0,    55,   694,
     0,    79,   698,     0,   141,   701,     0,   172,   704,     0,
   176,   705,     0,   187,   708,     0,    60,   695,     0,    48,
   693,     0,    59,    65,   686,     0,   157,   658,     0,   206,
   702,     0,    68,   697,     0,   483,     0,   484,     0,   487,
     0,   490,     0,   488,   276,   485,   277,     0,   495,   276,
   485,   277,   493,     0,   486,     0,   485,   272,   486,     0,
   660,     0,   660,   275,   660,     0,   488,     0,   495,   493,
     0,   494,     0,   499,     0,   502,     0,   235,     0,   489,
     0,   161,     0,    38,     0,   218,     0,   219,     0,    81,
     0,    92,     0,    18,   492,   491,   493,     0,    18,   273,
   659,   274,     0,    18,   273,   659,   272,   655,   274,     0,
    18,   273,   272,   655,   274,     0,   154,   160,   659,     0,
     0,   169,   655,     0,   169,   687,     0,     0,    21,   156,
   688,     0,     0,   498,   273,   658,   274,     0,   498,     0,
   498,   185,   273,   658,   274,     0,   497,   273,   658,   274,
     0,   497,     0,   496,   273,   658,   274,     0,   183,     0,
    21,   185,     0,    87,   185,     0,    21,     0,    87,     0,
   115,     0,   113,    21,     0,   113,    87,     0,    95,   500,
     0,   501,   500,     0,     0,   273,   660,   274,     0,   273,
   660,   272,   660,   274,     0,    41,     0,    88,     0,    91,
   503,     0,    93,    91,   503,     0,   144,     0,    89,   138,
     0,   273,   656,   274,     0,     0,   517,     0,   505,     0,
   534,     0,   156,    68,   697,   174,   660,     0,   156,    68,
   697,   174,   223,     0,   156,    68,   697,   174,   268,   223,
     0,   507,     0,   508,     0,   510,     0,   244,   709,     0,
   245,   244,   709,   509,     0,   123,     0,     0,   153,   514,
   174,   511,   709,     0,   244,     0,     0,    25,   514,   515,
     0,   153,   514,     0,   193,     0,     0,   147,   516,     0,
     0,   162,     0,     0,   156,   175,   518,     0,   519,     0,
     0,   520,     0,   519,   520,     0,   521,     0,   522,     0,
   523,     0,   527,     0,   143,   123,     0,   143,   194,     0,
   188,     0,   117,   188,     0,    84,   102,   524,     0,   524,
     0,   525,     0,   143,   177,   526,     0,   143,    26,   526,
     0,   162,     0,   162,   172,     0,   162,   172,   166,     0,
   186,     0,   117,   186,     0,     0,   146,   530,     0,   158,
     0,   142,     0,     0,   143,     0,   194,     0,   531,     0,
   530,   272,   531,     0,   533,   532,     0,    61,   528,   529,
     0,     0,   575,     0,   533,   272,   575,     0,   156,   168,
    79,   698,     0,   539,   536,   538,     0,    61,   180,   537,
     0,     0,   121,   616,     0,     0,   192,   243,     0,     0,
   542,   592,   598,     0,   542,   592,   598,     0,   542,   592,
   598,     0,   543,     0,   542,   178,   684,   543,     0,   542,
   178,     6,   543,     0,   544,     0,   155,   549,   552,   553,
   557,   582,   578,   581,   583,     0,     0,     0,     0,     0,
   550,   551,   546,     0,   550,   546,     0,   551,     0,     0,
   231,   661,   545,     0,   231,   273,   643,   274,   545,     0,
   231,   650,   545,     0,   232,   661,     0,   232,   273,   546,
   643,   274,     0,   232,   650,     0,    46,     0,   683,     0,
   554,     0,   269,     0,   555,     0,   554,   272,   555,     0,
   643,     0,   643,   556,   699,     0,    10,     0,     0,    63,
   558,     0,   559,     0,   558,   272,   559,     0,   565,     0,
   560,     0,   572,     0,   561,     0,   273,   565,   274,     0,
   273,   539,   274,   556,   562,   563,     0,   703,     0,     0,
   273,   564,   274,     0,     0,   699,     0,   564,   272,   699,
     0,   566,     0,   567,     0,   568,     0,   559,   263,    85,
   560,     0,   559,   114,   576,    85,   560,     0,   559,   576,
    85,   559,   569,     0,   570,     0,   571,     0,   122,   623,
     0,   239,   273,   616,   274,     0,   701,   573,   556,   703,
     0,   701,   573,     0,   273,   646,   274,     0,     0,   575,
     0,   704,   703,     0,   704,     0,    77,     0,   100,   577,
     0,   151,   577,     0,    64,   577,     0,     0,   127,     0,
     0,    71,    19,   579,     0,     0,   580,     0,   579,   272,
   580,     0,   643,     0,    73,   623,     0,     0,   190,   623,
     0,     0,   135,   584,     0,     0,   585,   273,   586,   274,
     0,    85,     0,   164,   108,     0,   108,     0,   164,     0,
     0,   587,     0,   587,   272,   586,     0,   588,   589,     0,
   584,     0,   704,     0,   704,   588,     0,   114,     0,    79,
   273,   590,   274,     0,   126,   698,   591,     0,   698,     0,
   698,   272,   590,     0,    79,   273,   590,   274,     0,     0,
   126,    19,   593,     0,     0,   594,     0,   593,   272,   594,
     0,   643,   595,   597,     0,    11,     0,    45,     0,     0,
   231,     0,   241,     0,   240,   547,   596,   548,     0,     0,
   259,   643,     0,   259,   643,   174,   643,     0,     0,    80,
    82,   575,   617,   182,   273,   646,   274,     0,    80,    82,
   575,   617,   539,     0,   601,     0,   602,     0,    44,    63,
   574,   582,   583,   592,   598,     0,    44,    63,   574,   603,
     0,   190,    35,   121,   692,     0,   605,     0,   606,     0,
   180,   574,   156,   607,   582,   583,   592,   598,     0,   180,
   574,   156,   607,   603,     0,   608,     0,   607,   272,   608,
     0,   622,   264,   643,     0,   143,    18,   621,    63,   575,
   610,   612,     0,    80,    18,   621,    82,   575,   610,   612,
     0,    60,    63,   611,   174,   611,     0,    60,   174,   611,
     0,     0,   492,     0,   650,     0,   655,     0,   107,   613,
     0,     0,   659,     0,   650,     0,   615,     0,     0,   273,
   616,   274,     0,   621,     0,   616,   272,   621,     0,   618,
     0,     0,   273,   619,   274,     0,   622,     0,   619,   272,
   622,     0,   621,     0,   703,   278,   690,     0,   703,   278,
   269,     0,   690,     0,   621,     0,   703,   278,   690,     0,
   639,     0,   118,   639,     0,   625,     0,   623,   125,   623,
     0,   623,     8,   623,     0,   625,     0,   118,   639,     0,
   624,   125,   623,     0,   624,     8,   623,     0,   640,   125,
   623,     0,   640,     8,   623,     0,   626,     0,   273,   624,
   274,     0,   118,   625,     0,   627,     0,   630,     0,   631,
     0,   632,     0,   633,     0,   638,     0,   628,     0,   636,
     0,   634,     0,   635,     0,   637,     0,   643,   264,   643,
     0,   643,   265,   643,     0,   643,   266,   643,     0,   643,
    67,   643,     0,   643,   101,   643,     0,   643,   119,   643,
     0,   643,   120,   643,     0,   643,   116,   643,     0,   643,
   264,     6,   273,   540,   274,     0,   643,   265,     6,   273,
   540,   274,     0,   643,   266,     6,   273,   540,   274,     0,
   643,    67,     6,   273,   540,   274,     0,   643,   101,     6,
   273,   540,   274,     0,   643,   119,     6,   273,   540,   274,
     0,   643,   120,     6,   273,   540,   274,     0,   643,   116,
     6,   273,   540,   274,     0,   643,   264,   629,   273,   540,
   274,     0,   643,   265,   629,   273,   540,   274,     0,   643,
   266,   629,   273,   540,   274,     0,   643,    67,   629,   273,
   540,   274,     0,   643,   101,   629,   273,   540,   274,     0,
   643,   119,   629,   273,   540,   274,     0,   643,   120,   629,
   273,   540,   274,     0,   643,   116,   629,   273,   540,   274,
     0,   163,     0,     9,     0,   643,    83,    46,    63,   643,
     0,   643,    83,   118,    46,    63,   643,     0,   643,    17,
   643,     8,   643,     0,   643,   118,    17,   643,     8,   643,
     0,   643,   103,   643,     0,   643,   118,   103,   643,     0,
   643,   103,   643,    54,   643,     0,   643,   118,   103,   643,
    54,   643,     0,   643,    75,   641,     0,   643,   118,    75,
   641,     0,   643,    31,   643,     0,   643,   118,    31,   643,
     0,   643,   167,   643,     0,   643,   118,   167,   643,     0,
   643,   167,   192,   643,     0,   643,   118,   167,   192,   643,
     0,    57,   273,   539,   274,     0,   159,   273,   539,   274,
     0,   643,    83,    94,     0,   643,    83,   118,    94,     0,
   248,     0,   249,     0,   250,     0,   251,     0,   252,     0,
   253,     0,   642,     0,   273,   646,   274,     0,   273,   540,
   274,     0,   620,     0,   645,     0,   662,     0,   649,     0,
   650,     0,   422,     0,   671,     0,   672,     0,   670,     0,
   268,   643,     0,   267,   643,     0,   643,   267,   643,     0,
   643,    28,   643,     0,   643,    23,   689,     0,   643,   268,
   643,     0,   643,   269,   643,     0,   643,   270,   643,     0,
   273,   643,   274,     0,   273,   541,   274,     0,   651,     0,
   652,     0,   653,     0,    39,     0,   703,   278,    39,     0,
    97,     0,   644,     0,   685,     0,   220,     0,   221,     0,
   222,     0,   620,   276,   646,   277,     0,   643,     0,   646,
   272,   643,     0,   649,     0,   268,   648,     0,   197,     0,
   196,     0,   195,     0,   223,     0,   224,     0,   648,     0,
   654,     0,    38,   199,     0,   218,   199,     0,   219,   199,
     0,   279,     0,   181,     0,   225,     0,   226,     0,   233,
     0,   234,     0,    66,     0,   165,     0,   242,     0,   199,
     0,   200,   199,     0,   656,     0,   268,   657,     0,   196,
     0,   196,     0,   656,     0,   196,     0,   661,     0,   268,
   661,     0,   196,     0,   663,     0,   664,     0,   665,     0,
   667,     0,    32,   273,   269,   274,     0,    32,   273,   683,
   643,   274,     0,    32,   273,    46,   643,   274,     0,   171,
   273,   683,   643,   274,     0,   171,   273,    46,   643,   274,
     0,    13,   273,   683,   643,   274,     0,    13,   273,    46,
   643,   274,     0,   110,   273,   683,   643,   274,     0,   110,
   273,    46,   643,   274,     0,   106,   273,   683,   643,   274,
     0,   106,   273,    46,   643,   274,     0,    69,   273,   697,
   272,   643,   274,     0,   666,     0,   209,   273,   682,    63,
   643,   274,     0,   668,     0,    96,   273,   643,   274,     0,
   228,   273,   643,    63,   643,   669,   274,     0,    61,   643,
     0,     0,   686,   273,   646,   274,     0,   686,   273,   274,
     0,    20,   273,   643,    10,   367,   274,     0,   673,     0,
   674,     0,   237,   273,   643,   272,   643,   274,     0,   261,
   273,   623,   272,   643,   272,   643,   274,     0,   238,   273,
   643,   272,   646,   274,     0,   675,     0,   677,     0,   236,
   680,   676,    51,     0,   236,   680,   676,    50,   681,    51,
     0,   189,   679,   173,   681,     0,   676,   189,   679,   173,
   681,     0,   236,   678,    51,     0,   236,   678,    50,   681,
    51,     0,   189,   623,   173,   681,     0,   678,   189,   623,
   173,   681,     0,   643,     0,   643,     0,   643,     0,   210,
     0,   211,     0,   212,     0,   213,     0,   214,     0,   215,
     0,   216,     0,   217,     0,     6,     0,     0,    46,     0,
     0,    94,     0,   198,     0,   710,     0,   710,     0,   710,
     0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
     0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
     0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
     0,   710,     0,   710,     0,   710,     0,   710,     0,   710,
     0,   198,     0,   711,     0,   227,     0,   230,     0,   228,
     0,   238,     0,   241,     0,   247,     0,   243,     0,   237,
     0,   240,     0,   246,     0,   248,     0,   249,     0,   250,
     0,   260,     0,   254,     0,   255,     0,   261,     0,   262,
     0,   216,     0,   217,     0
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
  1624,  1626,  1630,  1634,  1639,  1643,  1648,  1652,  1656,  1658,
  1662,  1666,  1672,  1677,  1682,  1684,  1686,  1690,  1692,  1694,
  1698,  1699,  1700,  1702,  1706,  1710,  1712,  1716,  1718,  1720,
  1725,  1728,  1732,  1733,  1737,  1742,  1743,  1747,  1749,  1751,
  1753,  1757,  1758,  1759,  1762,  1766,  1770,  1774,  1816,  1823,
  1831,  1833,  1837,  1838,  1842,  1848,  1855,  1878,  1882,  1888,
  1892,  1898,  1902,  1910,  1921,  1932,  1934,  1936,  1940,  1944,
  1946,  1950,  1952,  1954,  1956,  1958,  1960,  1962,  1964,  1966,
  1968,  1970,  1972,  1974,  1976,  1978,  1982,  1984,  1988,  1994,
  1998,  2001,  2008,  2010,  2012,  2015,  2018,  2023,  2027,  2033,
  2034,  2038,  2047,  2051,  2053,  2055,  2057,  2059,  2063,  2064,
  2068,  2070,  2072,  2074,  2080,  2083,  2085,  2089,  2097,  2098,
  2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,
  2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,
  2119,  2120,  2121,  2122,  2123,  2126,  2128,  2132,  2134,  2139,
  2145,  2147,  2149,  2153,  2155,  2162,  2168,  2169,  2173,  2187,
  2189,  2191,  2193,  2207,  2217,  2218,  2222,  2223,  2229,  2233,
  2235,  2237,  2239,  2241,  2243,  2245,  2247,  2249,  2251,  2253,
  2255,  2262,  2263,  2266,  2267,  2270,  2275,  2282,  2283,  2287,
  2296,  2302,  2303,  2306,  2307,  2308,  2309,  2326,  2331,  2336,
  2355,  2372,  2379,  2380,  2387,  2391,  2397,  2403,  2411,  2415,
  2421,  2425,  2429,  2435,  2439,  2446,  2452,  2458,  2466,  2471,
  2476,  2483,  2484,  2485,  2488,  2489,  2492,  2493,  2494,  2501,
  2505,  2516,  2522,  2576,  2638,  2639,  2646,  2659,  2664,  2669,
  2676,  2678,  2685,  2686,  2687,  2691,  2696,  2701,  2712,  2713,
  2714,  2717,  2721,  2725,  2727,  2731,  2735,  2736,  2739,  2743,
  2747,  2748,  2751,  2753,  2757,  2758,  2762,  2766,  2767,  2771,
  2772,  2776,  2777,  2778,  2779,  2782,  2784,  2788,  2790,  2794,
  2796,  2799,  2801,  2803,  2807,  2809,  2811,  2815,  2817,  2819,
  2823,  2827,  2829,  2831,  2835,  2837,  2841,  2842,  2846,  2850,
  2852,  2856,  2857,  2862,  2870,  2874,  2876,  2880,  2882,  2886,
  2888,  2895,  2899,  2904,  2909,  2911,  2913,  2917,  2920,  2932,
  2936,  2940,  2944,  2948,  2950,  2952,  2954,  2958,  2960,  2962,
  2966,  2968,  2970,  2974,  2976,  2980,  2982,  2986,  2987,  2991,
  2992,  2996,  2997,  3002,  3006,  3007,  3011,  3012,  3015,  3016,
  3017,  3022,  3027,  3028,  3032,  3034,  3038,  3039,  3043,  3044,
  3045,  3048,  3053,  3058,  3062,  3063,  3066,  3070,  3074,  3077,
  3082,  3084,  3088,  3089,  3094,  3099,  3101,  3103,  3105,  3107,
  3111,  3112,  3118,  3120,  3124,  3125,  3131,  3134,  3136,  3140,
  3142,  3149,  3151,  3155,  3159,  3161,  3163,  3169,  3171,  3175,
  3176,  3180,  3182,  3185,  3186,  3190,  3192,  3194,  3198,  3199,
  3203,  3205,  3211,  3213,  3217,  3218,  3222,  3226,  3228,  3230,
  3234,  3236,  3240,  3242,  3248,  3251,  3259,  3266,  3269,  3276,
  3277,  3280,  3284,  3288,  3295,  3296,  3299,  3304,  3309,  3310,
  3314,  3321,  3323,  3327,  3329,  3331,  3334,  3335,  3338,  3342,
  3344,  3347,  3349,  3355,  3356,  3360,  3364,  3365,  3370,  3371,
  3375,  3379,  3380,  3385,  3386,  3389,  3394,  3399,  3402,  3409,
  3410,  3412,  3413,  3415,  3419,  3420,  3422,  3424,  3428,  3430,
  3434,  3435,  3437,  3441,  3442,  3443,  3444,  3445,  3446,  3447,
  3448,  3449,  3450,  3451,  3456,  3458,  3460,  3462,  3464,  3466,
  3468,  3470,  3476,  3478,  3480,  3482,  3484,  3486,  3488,  3490,
  3492,  3494,  3496,  3498,  3500,  3502,  3504,  3506,  3510,  3511,
  3517,  3519,  3523,  3525,  3530,  3532,  3534,  3536,  3541,  3543,
  3547,  3549,  3553,  3555,  3557,  3559,  3563,  3567,  3571,  3573,
  3577,  3582,  3587,  3594,  3599,  3604,  3613,  3614,  3618,  3625,
  3626,  3627,  3628,  3629,  3630,  3631,  3632,  3633,  3634,  3636,
  3638,  3645,  3647,  3649,  3656,  3663,  3670,  3672,  3674,  3675,
  3676,  3677,  3679,  3681,  3685,  3686,  3689,  3705,  3721,  3725,
  3729,  3730,  3734,  3735,  3739,  3741,  3743,  3745,  3747,  3751,
  3752,  3754,  3770,  3786,  3790,  3794,  3796,  3800,  3804,  3807,
  3810,  3813,  3816,  3821,  3823,  3828,  3829,  3833,  3840,  3847,
  3854,  3861,  3862,  3866,  3872,  3873,  3874,  3875,  3878,  3880,
  3882,  3885,  3892,  3901,  3908,  3917,  3919,  3921,  3923,  3929,
  3938,  3941,  3945,  3946,  3950,  3959,  3961,  3965,  3967,  3971,
  3977,  3978,  3981,  3985,  3988,  3992,  3993,  3996,  3998,  4002,
  4004,  4008,  4010,  4014,  4016,  4020,  4023,  4026,  4029,  4031,
  4033,  4035,  4037,  4039,  4041,  4043,  4047,  4048,  4051,  4052,
  4055,  4063,  4066,  4069,  4072,  4075,  4078,  4081,  4084,  4087,
  4090,  4093,  4096,  4099,  4102,  4105,  4108,  4111,  4114,  4117,
  4120,  4123,  4126,  4129,  4132,  4137,  4138,  4143,  4145,  4146,
  4147,  4148,  4149,  4150,  4151,  4152,  4153,  4154,  4155,  4156,
  4158,  4159,  4160,  4161,  4162,  4163,  4164
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
"assignments","assignment","blob","filter_clause","blob_subtype_value","segment_clause",
"segment_length","column_parens_opt","column_parens","column_list","ins_column_parens_opt",
"ins_column_parens","ins_column_list","column_name","simple_column_name","update_column_name",
"search_condition","bracable_search_condition","simple_search_condition","predicate",
"comparison_predicate","quantified_predicate","some","distinct_predicate","between_predicate",
"like_predicate","in_predicate","containing_predicate","starting_predicate",
"exists_predicate","singular_predicate","null_predicate","trigger_action_predicate",
"special_trigger_action_predicate","in_predicate_value","table_subquery","value",
"datetime_value_expression","array_element","value_list","constant","u_numeric_constant",
"u_constant","parameter","current_user","current_role","internal_info","sql_string",
"signed_short_integer","nonneg_short_integer","neg_short_integer","pos_short_integer",
"unsigned_short_integer","signed_long_integer","long_integer","function","aggregate_function",
"generate_value_function","numeric_value_function","extract_expression","string_value_function",
"substring_function","string_length_opt","udf","cast_specification","case_expression",
"case_abbreviation","case_specification","simple_case","simple_when_clause",
"searched_case","searched_when_clause","when_operand","case_operand","case_result",
"timestamp_part","all_noise","distinct_noise","null_value","symbol_UDF_name",
"symbol_blob_subtype_name","symbol_character_set_name","symbol_collation_name",
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
   411,   411,   412,   413,   414,   415,   416,   417,   418,   418,
   419,   420,   421,   422,   423,   423,   423,   424,   424,   424,
   425,   425,   425,   425,   426,   427,   427,   428,   428,   428,
   429,   429,   430,   430,   431,   432,   432,   433,   433,   433,
   433,   434,   434,   434,   435,   436,   437,   438,   439,   440,
   441,   441,   442,   442,   443,   444,   445,   446,   447,   448,
   449,   450,   450,   451,   452,   453,   453,   453,   454,   455,
   455,   456,   456,   456,   456,   456,   456,   456,   456,   456,
   456,   456,   456,   456,   456,   456,   457,   457,   458,   459,
   460,   460,   460,   460,   460,   460,   460,   461,   462,   463,
   463,   464,   464,   464,   464,   464,   464,   464,   465,   465,
   466,   466,   466,   466,   466,   466,   466,   467,   468,   468,
   468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
   468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
   468,   468,   468,   468,   468,   469,   469,   470,   470,   471,
   472,   472,   472,   473,   473,   474,   475,   475,   476,   476,
   476,   476,   476,   477,   478,   478,   479,   479,   480,   481,
   481,   481,   481,   481,   481,   481,   481,   481,   481,   481,
   481,   482,   482,   483,   483,   484,   484,   485,   485,   486,
   486,   487,   487,   488,   488,   488,   488,   488,   488,   488,
   488,   488,   489,   489,   490,   490,   490,   490,   491,   491,
   492,   492,   492,   493,   493,   494,   494,   494,   495,   495,
   495,   496,   496,   496,   497,   497,   498,   498,   498,   499,
   499,   500,   500,   500,   501,   501,   502,   502,   502,   502,
   503,   503,   504,   504,   504,   505,   505,   505,   506,   506,
   506,   507,   508,   509,   509,   510,   511,   511,   512,   513,
   514,   514,   515,   515,   516,   516,   517,   518,   518,   519,
   519,   520,   520,   520,   520,   521,   521,   522,   522,   523,
   523,   524,   524,   524,   525,   525,   525,   526,   526,   526,
   527,   528,   528,   528,   529,   529,   530,   530,   531,   532,
   532,   533,   533,   534,   535,   536,   536,   537,   537,   538,
   538,   539,   540,   541,   542,   542,   542,   543,   544,   545,
   546,   547,   548,   549,   549,   549,   549,   550,   550,   550,
   551,   551,   551,   552,   552,   553,   553,   554,   554,   555,
   555,   556,   556,   557,   558,   558,   559,   559,   560,   560,
   560,   561,   562,   562,   563,   563,   564,   564,   565,   565,
   565,   566,   567,   568,   569,   569,   570,   571,   572,   572,
   573,   573,   574,   574,   575,   576,   576,   576,   576,   576,
   577,   577,   578,   578,   579,   579,   580,   581,   581,   582,
   582,   583,   583,   584,   585,   585,   585,   585,   585,   586,
   586,   587,   587,   588,   588,   589,   589,   589,   590,   590,
   591,   591,   592,   592,   593,   593,   594,   595,   595,   595,
   596,   596,   597,   597,   598,   598,   598,   599,   599,   600,
   600,   601,   602,   603,   604,   604,   605,   606,   607,   607,
   608,   609,   609,   610,   610,   610,   611,   611,   492,   612,
   612,   613,   613,   614,   614,   615,   616,   616,   617,   617,
   618,   619,   619,   620,   620,   620,   621,   622,   622,   623,
   623,   623,   623,   623,   624,   624,   624,   624,   624,   624,
   625,   625,   625,   626,   626,   626,   626,   626,   626,   626,
   626,   626,   626,   626,   627,   627,   627,   627,   627,   627,
   627,   627,   628,   628,   628,   628,   628,   628,   628,   628,
   628,   628,   628,   628,   628,   628,   628,   628,   629,   629,
   630,   630,   631,   631,   632,   632,   632,   632,   633,   633,
   634,   634,   635,   635,   635,   635,   636,   637,   638,   638,
   639,   639,   639,   640,   640,   640,   641,   641,   642,   643,
   643,   643,   643,   643,   643,   643,   643,   643,   643,   643,
   643,   643,   643,   643,   643,   643,   643,   643,   643,   643,
   643,   643,   643,   643,   643,   643,   644,   644,   644,   645,
   646,   646,   647,   647,   648,   648,   648,   648,   648,   649,
   649,   649,   649,   649,   650,   651,   651,   652,   653,   653,
   653,   653,   653,   654,   654,   655,   655,   656,   657,   658,
   659,   660,   660,   661,   662,   662,   662,   662,   663,   663,
   663,   663,   663,   663,   663,   663,   663,   663,   663,   664,
   665,   666,   667,   667,   668,   669,   669,   670,   670,   671,
   672,   672,   673,   673,   673,   674,   674,   675,   675,   676,
   676,   677,   677,   678,   678,   679,   680,   681,   682,   682,
   682,   682,   682,   682,   682,   682,   683,   683,   684,   684,
   685,   686,   687,   688,   689,   690,   691,   692,   693,   694,
   695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
   705,   706,   707,   708,   709,   710,   710,   711,   711,   711,
   711,   711,   711,   711,   711,   711,   711,   711,   711,   711,
   711,   711,   711,   711,   711,   711,   711
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
     3,     4,     2,     6,     4,     8,     9,     6,     8,     6,
     4,     0,     4,     2,     1,     3,     0,     2,     4,     0,
     1,     1,     3,     3,     7,     2,     0,     2,     2,     3,
     3,     0,     1,     2,     4,     1,     3,     2,     2,     2,
     1,     1,     1,     1,     2,     2,     5,     0,     4,     7,
     3,     0,     1,     3,     3,     7,     7,     0,     0,     0,
     0,     3,     0,     9,     9,     1,     1,     0,     2,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
     5,     5,     5,     5,     5,     5,     2,     0,     4,     2,
     3,     3,     2,     2,     3,     3,     2,     3,     6,     1,
     2,     4,     3,     2,     2,     2,     2,     3,     1,     3,
     3,     3,     2,     2,     4,     4,     5,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     2,     2,     2,     1,
     1,     1,     0,     2,     2,     0,     1,     2,     2,     4,
     3,     2,     2,     7,     1,     0,     1,     0,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     3,     2,     2,
     2,     1,     1,     1,     1,     4,     5,     1,     3,     1,
     3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     4,     4,     6,     5,     3,     0,
     2,     2,     0,     3,     0,     4,     1,     5,     4,     1,
     4,     1,     2,     2,     1,     1,     1,     2,     2,     2,
     2,     0,     3,     5,     1,     1,     2,     3,     1,     2,
     3,     0,     1,     1,     1,     5,     5,     6,     1,     1,
     1,     2,     4,     1,     0,     5,     1,     0,     3,     2,
     1,     0,     2,     0,     1,     0,     3,     1,     0,     1,
     2,     1,     1,     1,     1,     2,     2,     1,     2,     3,
     1,     1,     3,     3,     1,     2,     3,     1,     2,     0,
     2,     1,     1,     0,     1,     1,     1,     3,     2,     3,
     0,     1,     3,     4,     3,     3,     0,     2,     0,     2,
     0,     3,     3,     3,     1,     4,     4,     1,     9,     0,
     0,     0,     0,     3,     2,     1,     0,     3,     5,     3,
     2,     5,     2,     1,     1,     1,     1,     1,     3,     1,
     3,     1,     0,     2,     1,     3,     1,     1,     1,     1,
     3,     6,     1,     0,     3,     0,     1,     3,     1,     1,
     1,     4,     5,     5,     1,     1,     2,     4,     4,     2,
     3,     0,     1,     2,     1,     1,     2,     2,     2,     0,
     1,     0,     3,     0,     1,     3,     1,     2,     0,     2,
     0,     2,     0,     4,     1,     2,     1,     1,     0,     1,
     3,     2,     1,     1,     2,     1,     4,     3,     1,     3,
     4,     0,     3,     0,     1,     3,     3,     1,     1,     0,
     1,     1,     4,     0,     2,     4,     0,     8,     5,     1,
     1,     7,     4,     4,     1,     1,     8,     5,     1,     3,
     3,     7,     7,     5,     3,     0,     1,     1,     1,     2,
     0,     1,     1,     1,     0,     3,     1,     3,     1,     0,
     3,     1,     3,     1,     3,     3,     1,     1,     3,     1,
     2,     1,     3,     3,     1,     2,     3,     3,     3,     3,
     1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
     3,     3,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     6,     1,     1,
     5,     6,     5,     6,     3,     4,     5,     6,     3,     4,
     3,     4,     3,     4,     4,     5,     4,     4,     3,     4,
     1,     1,     1,     1,     1,     1,     1,     3,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
     3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
     1,     1,     3,     1,     1,     1,     1,     1,     1,     4,
     1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     2,     1,     2,     1,     1,     1,
     1,     1,     2,     1,     1,     1,     1,     1,     4,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,     6,
     1,     6,     1,     4,     7,     2,     0,     4,     3,     6,
     1,     1,     6,     8,     6,     1,     1,     4,     6,     4,
     5,     3,     5,     4,     5,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     0,     1,     0,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,   572,   123,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   572,   637,     0,     0,     0,     0,     0,     1,
    10,    16,     7,     6,    14,    15,    12,    13,     3,     9,
    20,   554,    18,   559,   560,   561,     5,    17,   553,   555,
    19,   617,   724,   625,   628,    11,     8,   740,   741,    21,
   745,   746,     4,   466,     0,     0,     0,     0,     0,     0,
   400,   571,   574,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   122,     0,     0,   103,   730,   918,     0,    22,
   916,     0,     0,    74,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   479,     0,   362,
    30,    38,    35,    37,   765,    36,   765,  1006,  1026,  1027,
  1008,  1010,  1009,  1015,  1011,  1016,  1012,  1014,  1017,  1013,
  1018,  1019,  1020,  1022,  1023,  1021,  1024,  1025,     0,    32,
     0,    33,     0,    67,    69,   998,  1007,     0,     0,     0,
     0,     0,     0,     0,     0,   570,     0,     0,   978,   631,
   636,     0,     0,   579,     0,   683,   685,  1000,     0,     0,
     0,   115,   562,  1005,     0,     2,     0,   621,     0,   980,
   737,     0,   430,   431,   432,   433,   434,   435,   436,   437,
   438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
   454,   448,   449,   450,   451,   452,   453,   455,     0,   428,
   429,     0,   990,   407,     0,   994,   404,   259,   997,     0,
   685,   403,   378,  1001,   576,   569,   914,     0,   111,   159,
   158,   112,   141,   208,   777,   986,     0,   110,   153,   993,
     0,   106,   259,   113,   920,   130,   107,   193,   108,     0,
   109,   765,  1004,   114,   154,   728,   729,     0,   919,   917,
     0,    75,     0,   991,   701,   487,   989,   480,     0,   486,
   491,   481,   482,   489,   483,   484,   485,   490,   327,     0,
   261,    31,     0,    40,   764,    39,     0,     0,     0,     0,
     0,     0,   770,     0,     0,     0,     0,     0,     0,     0,
   568,   924,     0,   905,   630,   630,   631,   643,   641,   977,
   644,     0,   645,   635,   631,     0,     0,     0,     0,     0,
     0,   595,   588,   577,   578,   580,   582,   583,   584,   591,
   592,   585,     0,   684,   999,   116,   259,   117,   193,   118,
   765,   565,   619,     0,   615,     0,     0,   979,     0,     0,
   622,     0,     0,     0,     0,   405,   467,   370,     0,   370,
     0,   211,   406,   410,   401,   464,   465,     0,   261,   232,
   456,     0,   402,   419,     0,   376,   377,   476,   575,   573,
   915,   156,   156,     0,     0,     0,   168,   160,   161,   140,
     0,   104,     0,     0,   119,   261,   129,   128,   131,     0,
     0,     0,     0,     0,   982,    76,   211,     0,     0,   703,
   743,   488,     0,     0,     0,     0,   882,   911,     0,   981,
     0,   884,     0,     0,   912,     0,   906,   897,   896,   895,
  1006,     0,     0,     0,   887,   888,   889,   898,   899,   907,
   908,  1010,   909,   910,     0,  1015,  1011,   913,  1024,     0,
     0,     0,     0,   865,   359,   860,   774,   891,   885,   861,
   325,   900,   863,   864,   879,   880,   881,   901,   862,   925,
   926,   927,   941,   928,   943,   868,   866,   867,   951,   952,
   956,   957,   886,     0,     0,   986,     0,     0,     0,   363,
   209,     0,     0,     0,   767,     0,     0,    28,    34,     0,
     0,    44,    70,    72,  1002,    68,     0,     0,     0,   769,
     0,    53,     0,     0,     0,     0,    52,   567,     0,     0,
     0,   640,   638,     0,   647,     0,   646,   648,   653,   634,
     0,   614,     0,   589,   600,   586,   600,   587,   601,   607,
   611,   612,   596,   581,   701,   749,   778,     0,     0,   261,
     0,     0,   564,   563,     0,   616,   620,   723,   725,   730,
   627,   626,   735,   176,     0,   469,   137,     0,   472,     0,
   473,   468,     0,   414,     0,   416,   415,     0,   417,     0,
   411,     0,     0,   262,     0,     0,   423,     0,   424,   370,
   457,     0,   463,     0,     0,     0,   460,   986,   381,   380,
   475,     0,   398,   157,     0,     0,   166,     0,   165,     0,
     0,   155,   169,   170,   172,   162,   523,   535,   510,   545,
   513,   536,   546,     0,   552,   514,     0,   542,     0,   537,
   549,   509,   532,   511,   512,   507,   370,   492,   493,   494,
   502,   508,   495,   504,   525,     0,   530,   527,   505,   542,
   506,   120,   259,   121,     0,     0,   132,     0,     0,   192,
   232,   378,   368,     0,   211,     0,    82,    84,     0,     0,
   102,   101,   983,     0,     0,     0,     0,   851,   852,   853,
     0,   700,   782,   791,   794,   800,   795,   796,   797,   798,
   802,   803,   801,   804,   799,   780,     0,   709,   724,   978,
     0,   978,   902,     0,     0,   978,   978,   978,     0,   903,
   904,     0,     0,   967,     0,     0,     0,     0,     0,   870,
   869,     0,   724,   891,     0,   324,  1003,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   267,   502,   525,
     0,     0,   361,     0,   272,     0,   766,    29,     0,     0,
    45,    73,     0,     0,    27,   756,     0,   772,     0,   739,
   756,     0,     0,     0,     0,   566,     0,   630,     0,     0,
   701,     0,   652,     0,   557,     0,   556,   922,     0,   590,
     0,   598,   594,   593,     0,   604,     0,   609,   597,     0,
   703,   748,     0,     0,     0,   232,   368,   618,     0,   734,
     0,     0,   138,   177,   471,   370,   413,     0,   370,   371,
   418,   368,     0,   258,   368,   231,   987,   203,   200,     0,
   213,   370,   492,   368,     0,     0,     0,   230,   233,   234,
   235,   236,     0,   422,   462,   461,   421,   420,     0,     0,
     0,   384,   382,   383,   379,     0,   370,   186,   163,   167,
     0,     0,   171,     0,     0,   520,   759,   533,   534,   550,
     0,   547,   552,     0,   540,   538,   539,   143,     0,     0,
     0,   503,     0,     0,     0,     0,   541,   261,     0,   368,
   133,   191,     0,   194,   196,   197,     0,     0,     0,     0,
   211,   211,    79,     0,    87,    78,     0,     0,     0,     0,
   793,   781,     0,     0,   854,   855,   856,     0,   785,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   705,   707,
   708,   702,     0,   737,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   969,   970,
   971,   972,   973,   974,   975,   976,     0,     0,     0,     0,
   962,     0,     0,     0,     0,     0,     0,   878,   737,   877,
   326,     0,   873,   985,   872,   871,   874,   875,   876,   892,
   949,     0,   883,   776,   775,   365,   364,     0,   265,   280,
     0,   271,   273,   768,     0,     0,     0,     0,     0,     0,
    25,    54,    42,    62,    64,     0,     0,    71,     0,   761,
     0,   771,     0,   761,    50,    48,     0,     0,     0,   639,
   642,     0,   654,   655,   658,   660,   657,   669,   670,   671,
   659,   682,     0,   694,   649,   651,   995,   558,   923,   599,
   608,   603,   602,     0,   613,   750,   724,   751,   779,   368,
     0,     0,   726,   632,   727,   736,   470,   156,   183,   175,
   178,   179,     0,     0,     0,   412,   368,   368,   371,   270,
   263,   272,   202,   370,     0,   371,   204,   199,   205,     0,
     0,   241,     0,   426,   371,   370,   368,   425,     0,     0,
     0,   397,   478,   187,   188,   164,     0,   174,   521,   522,
   921,     0,     0,     0,   525,     0,   548,     0,     0,   370,
   371,     0,   498,   500,     0,     0,     0,     0,     0,     0,
     0,   378,   272,   156,   135,   232,   189,   398,   373,     0,
    83,     0,   211,     0,    91,     0,    85,     0,     0,    90,
    86,     0,   744,   988,     0,     0,   786,     0,     0,   792,
     0,     0,   784,   783,     0,   841,     0,   830,   829,     0,
   808,     0,   839,   857,     0,   849,     0,     0,     0,   809,
   835,     0,     0,   812,     0,     0,     0,     0,     0,     0,
     0,   810,     0,     0,   811,     0,   843,     0,     0,   805,
     0,     0,   806,     0,     0,   807,   706,   709,   742,     0,
     0,   211,     0,   929,     0,     0,   944,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   968,     0,     0,   966,
     0,     0,   958,     0,     0,     0,     0,   624,   890,   948,
     0,   260,   279,     0,   337,   360,   274,    66,    58,    59,
    65,    60,    61,     0,     0,     0,    23,    26,    42,    43,
   523,   523,     0,   753,   773,     0,   752,    51,    49,    46,
     0,     0,   690,   657,     0,   692,   686,   692,   690,   692,
     0,     0,     0,   680,     0,   699,   605,   606,   610,   737,
   272,   190,   373,     0,     0,   184,     0,   180,     0,     0,
     0,   408,   219,   214,   893,   215,   216,   217,   218,     0,
     0,   264,     0,     0,   212,   220,     0,   241,   730,   237,
     0,   427,   458,   459,   389,   390,   386,   385,   388,   387,
   370,   477,   371,   173,   984,     0,     0,   516,     0,   515,
   551,     0,   543,     0,   144,     0,   496,     0,   524,   525,
   531,   529,     0,   526,   368,     0,     0,     0,   127,   136,
   195,   370,     0,   369,   105,     0,   125,   124,    94,     0,
     0,    95,     0,    88,    89,     0,   847,   848,   788,   787,
   790,   789,     0,     0,     0,     0,   724,     0,     0,     0,
   850,     0,     0,     0,     0,     0,     0,   842,   840,   836,
     0,   844,     0,     0,     0,     0,   845,     0,     0,     0,
     0,     0,     0,   713,     0,   710,     0,   714,   935,   934,
     0,     0,   931,   930,     0,   939,   938,   937,   936,   933,
   932,     0,   947,   964,   963,     0,     0,     0,     0,     0,
     0,     0,   266,     0,     0,   276,   277,     0,   986,     0,
     0,     0,     0,     0,     0,     0,  1008,  1013,     0,     0,
   358,   290,   285,     0,   337,   284,   294,   295,   296,   297,
   298,   305,   306,   299,   301,   302,   304,     0,   309,     0,
   352,   354,   353,     0,     0,     0,     0,     0,     0,   986,
    55,    56,     0,    57,    63,    24,   757,     0,   758,   755,
   760,   763,   762,   738,     0,    47,   653,   661,   656,   691,
   689,   687,     0,   688,     0,     0,     0,     0,     0,     0,
   703,   747,     0,   369,   731,   732,   633,   186,   185,   181,
     0,   371,   894,   268,   269,   371,   371,   207,   221,   222,
   370,     0,   238,     0,     0,     0,     0,     0,     0,     0,
     0,   272,   474,   518,     0,   519,     0,   142,     0,   207,
   145,   146,   148,   149,   150,   499,   501,   497,   528,   272,
   398,   371,   186,     0,     0,   366,   370,    92,     0,     0,
    98,    93,   525,     0,   833,     0,     0,   859,   737,   858,
   831,     0,     0,     0,   837,     0,     0,     0,     0,   846,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   704,   709,     0,   716,     0,   712,   715,   950,   210,   940,
   942,     0,     0,   965,   960,   959,     0,   953,   955,     0,
   213,   275,     0,   313,     0,     0,     0,   308,     0,   322,
   307,   338,   339,     0,   996,   355,   356,     0,   286,     0,
   291,   288,   343,     0,     0,   310,     0,   300,   293,   303,
   292,   336,    41,   523,   664,     0,   672,   690,   681,   679,
   693,   695,   697,   698,   629,   371,   367,   733,   182,   371,
   152,   257,     0,     0,   198,   223,     0,     0,     0,   241,
   224,   227,   765,     0,   371,   395,   396,   392,   391,   394,
   393,     0,   517,   544,   151,   139,   147,     0,   370,   254,
   134,   371,   372,     0,     0,    96,    97,    80,     0,   816,
   824,   623,   832,   817,   825,   820,   828,   834,   838,   818,
   826,   819,   827,   813,   821,   814,   822,   815,   823,   711,
     0,   722,   946,   945,   961,     0,     0,   282,   278,     0,
   311,     0,   327,     0,     0,     0,   340,     0,   351,     0,
     0,     0,     0,   346,   344,     0,     0,     0,   331,     0,
   332,     0,   754,   666,   663,   673,     0,     0,   674,   675,
   676,     0,   255,   409,   201,   206,   225,   241,   765,   228,
   247,   240,   242,   399,   371,     0,   374,   371,    77,    99,
   100,     0,   719,     0,   718,   954,   281,     0,   312,   330,
     0,   315,     0,   321,     0,   350,   349,   992,   348,   337,
     0,     0,     0,     0,   323,     0,     0,     0,   662,   677,
     0,   696,   229,   247,     0,   241,   243,   244,   256,   371,
     0,   717,     0,     0,     0,     0,     0,     0,   337,   357,
   345,   347,     0,   342,     0,   334,   333,     0,   667,     0,
   241,     0,     0,   239,     0,   246,     0,   245,   375,   126,
   720,     0,   283,     0,   328,   314,   318,   320,     0,     0,
     0,   337,     0,   665,   678,   226,     0,     0,   250,   249,
   248,   721,     0,   337,     0,     0,   337,   335,   668,   253,
   251,   252,   329,   319,   337,   341,   316,   317,     0,     0,
     0
};

static const short yydefgoto[] = {  1889,
    20,    21,   487,   129,   130,   131,   132,  1237,   745,   740,
    22,   144,   991,   992,   993,   994,   133,   134,   492,   493,
    23,    84,   396,   885,   656,   657,   658,  1131,  1124,  1125,
  1562,   252,   660,    24,    76,    25,   162,    26,   385,    77,
  1345,   234,   389,   648,  1115,  1339,   556,   222,   381,  1101,
  1540,  1541,  1542,  1543,  1544,  1545,   228,   244,   219,   595,
   220,   377,   378,   379,   602,   603,   604,   557,   558,  1050,
  1051,  1052,  1277,  1086,   237,   328,   391,   873,   874,   875,
   809,   810,   811,  1665,   578,   477,  1401,   659,  1066,  1282,
  1518,  1519,  1520,  1671,   876,   580,   818,   819,   820,   821,
  1300,   822,  1816,  1817,  1818,  1870,   232,   326,   642,   207,
   359,   483,   573,   574,   978,   478,  1059,   981,   982,   983,
  1425,  1426,  1224,  1729,  1427,  1442,  1443,  1444,  1445,  1446,
  1447,  1448,  1449,  1450,  1451,  1452,  1453,  1454,  1455,  1736,
  1456,   444,   445,  1827,  1750,  1457,  1458,  1459,  1861,  1632,
  1633,  1743,  1744,  1460,  1461,  1462,  1463,  1628,    27,    28,
   271,   479,   480,   241,   330,   878,  1556,   565,  1056,  1344,
   239,   644,   368,   591,   592,   835,   837,  1312,    29,    61,
   800,   797,   353,   354,   363,   364,   199,   200,   365,  1075,
   586,   827,   204,   172,   346,   347,   212,   593,  1313,    30,
    98,   627,   628,   629,  1102,  1103,   630,   729,   632,   633,
  1095,  1477,   862,   634,   730,   636,   637,   638,   639,   855,
   640,   641,   852,    31,    32,    33,    34,    35,   544,    36,
   509,    37,    38,    63,   216,   370,    39,   314,   315,   316,
   317,   318,   319,   320,   321,   773,   322,  1034,  1269,   529,
   530,   778,   531,    40,  1464,   168,   546,   335,    42,  1366,
   712,    43,    44,    45,   512,   304,  1274,  1658,   149,   150,
   151,   302,   516,   517,   518,   764,   761,  1013,  1014,  1015,
  1016,  1754,  1809,  1838,  1017,  1018,  1019,  1020,  1759,  1760,
  1761,  1021,  1264,   155,   488,  1262,  1491,  1266,  1651,  1652,
  1501,   400,   689,  1394,   923,  1395,  1396,  1397,  1596,  1782,
  1785,   171,   548,   549,   248,  1507,  1045,   341,  1465,  1466,
    48,    49,   401,  1467,    51,    52,   535,  1468,    53,  1000,
  1478,  1244,  1481,   274,   275,   484,   499,   500,   747,   446,
   447,   538,   672,   898,   673,   674,   675,   676,  1150,   677,
   678,   679,   680,   681,   682,   683,   684,   685,   686,   900,
  1153,  1154,   687,   449,   450,   451,  1284,   452,   453,   454,
   455,   456,   457,   458,   847,    81,   250,   236,  1093,  1104,
   768,   459,   460,   461,   462,   463,   464,   465,  1603,   466,
   467,   468,   469,   470,   471,   954,   472,   705,  1211,   706,
  1208,   947,   303,   339,   473,   474,   662,  1314,   963,   225,
   806,  1133,   256,   202,   253,  1797,   229,  1783,  1026,  1469,
  1022,   135,   475,   211,   213,   995,   716,   242,   163,   476,
   137
};

static const short yypact[] = {  2124,
   533,    42,  1833,   473,   942,   178,  1976,    59,  2090,   561,
   232,   678,    42,   571,   507,  5437,   685,  5437,    21,   218,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   465,    96,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  7219,  5437,  5437,  5437,  5437,  5437,
-32768,-32768,   357,   723,  5437,  5437,  5437,   504,  5437,   352,
  5437,  5437,-32768,  5437,  5437,-32768,   729,-32768,   404,-32768,
-32768,   605,  5437,-32768,  5437,  5437,  5437,   618,  5437,  5437,
  5437,  5437,   352,  5437,  5437,  5437,  5437,-32768,  5437,   421,
   617,-32768,-32768,-32768,   487,-32768,   487,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   659,   556,
   710,-32768,   311,-32768,-32768,-32768,-32768,  5437,  5437,  5437,
   716,   725,   734,   953,   177,   714,   358,   575,   766,   666,
-32768,  5437,   856,   396,   758,-32768,  5437,-32768,  5437,  5437,
  5437,-32768,-32768,-32768,  5437,-32768,   761,   755,   944,   819,
   759,   740,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   644,-32768,
-32768,   723,-32768,-32768,   211,-32768,-32768,   702,-32768,   224,
-32768,-32768,   530,-32768,   880,-32768,-32768,   830,-32768,   802,
-32768,-32768,  1026,-32768,-32768,-32768,   723,-32768,-32768,-32768,
   486,-32768,   702,-32768,-32768,   201,-32768,   990,-32768,  1010,
-32768,   487,-32768,-32768,-32768,-32768,-32768,   975,-32768,-32768,
   876,-32768,  1002,-32768,   914,-32768,-32768,-32768,   876,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6603,  5437,
   936,-32768,  5437,-32768,-32768,-32768,  3823,   373,   959,  4068,
  5437,  1023,   836,  1047,  1058,  3823,   988,  1016,  1027,  4068,
   901,-32768,  6822,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  5070,-32768,-32768,-32768,   989,  5437,  1060,   978,   146,
  5437,   999,-32768,-32768,   396,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  5437,-32768,-32768,-32768,   702,-32768,   990,-32768,
   487,  1052,  1057,   943,-32768,  6822,  1030,-32768,  1030,  6822,
-32768,    70,   934,   945,   948,   740,-32768,  1169,   492,-32768,
  5437,-32768,   644,-32768,-32768,-32768,-32768,  5437,   936,   417,
   996,  2804,   932,-32768,  7219,-32768,-32768,   497,-32768,-32768,
-32768,   946,   946,   723,  1102,   723,   149,   802,-32768,-32768,
  3321,-32768,  5437,  5437,-32768,   936,-32768,-32768,  1188,   512,
   947,  5437,  1214,  5437,-32768,-32768,   186,  5382,  4194,  1088,
-32768,-32768,   955,   956,   957,  1032,-32768,-32768,   964,-32768,
   967,-32768,   970,   971,-32768,   972,-32768,-32768,-32768,-32768,
   981,   982,  1039,  1059,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   984,-32768,-32768,  5289,   987,   991,-32768,   997,  6822,
  6822,  5508,  5437,-32768,-32768,   995,-32768,  1264,-32768,-32768,
  1003,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1006,   985,  1004,  3321,  1005,   680,-32768,
-32768,  1008,  1274,   700,-32768,  5437,  1111,-32768,-32768,  5437,
  5437,    27,-32768,-32768,-32768,-32768,  5437,  5437,   720,-32768,
  5437,-32768,  1226,  5437,  3823,  1149,  1022,-32768,  5437,  5508,
   432,-32768,-32768,  6822,-32768,  1237,  1036,-32768,   160,-32768,
   524,-32768,    91,-32768,    99,-32768,    99,-32768,  1038,-32768,
    97,-32768,  1135,-32768,   415,-32768,-32768,  1048,  1024,   936,
  1043,  1309,-32768,-32768,  5437,-32768,-32768,  1049,-32768,   371,
-32768,-32768,   294,-32768,  1230,  1233,-32768,   723,-32768,  1234,
-32768,-32768,  1303,-32768,  1304,-32768,-32768,  1285,-32768,  3321,
-32768,  3321,   712,-32768,  1320,  5437,-32768,  7128,-32768,   558,
-32768,  5437,   805,   224,  1157,  1126,  1206,  1171,-32768,-32768,
-32768,   450,  1208,-32768,  1151,   352,-32768,   723,-32768,  1328,
  1262,-32768,   149,-32768,-32768,-32768,   520,  1165,-32768,-32768,
-32768,  1175,-32768,  1224,  1091,-32768,  1275,  1096,   427,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1098,-32768,-32768,-32768,   148,  1109,  1114,    22,-32768,  1096,
-32768,-32768,   702,-32768,  1314,  1378,-32768,   723,   723,-32768,
   417,   530,-32768,  1270,-32768,  1245,  1130,-32768,  3276,  1280,
-32768,-32768,-32768,  1282,  1136,  4413,  1143,   983,  1089,  1260,
  3949,   209,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  2288,   568,  1284,   835,
  6822,   140,-32768,  5437,  6822,   863,   867,   905,  1925,-32768,
-32768,  6822,  4632,  1264,   440,  1228,  6822,  6822,  4632,   151,
   151,  1144,    96,   576,   733,-32768,-32768,  6822,  5437,  6822,
  6822,  6822,  6822,  6822,  6822,  5727,  2864,-32768,-32768,  1399,
  1146,  5437,-32768,  5437,  1380,  5437,-32768,-32768,  3367,  1249,
-32768,-32768,  1222,  4068,-32768,  1366,   743,-32768,  1159,-32768,
  1366,  3367,  1379,  1382,  5437,-32768,   576,-32768,   623,  4725,
  1251,  6822,-32768,  5437,-32768,   707,-32768,-32768,   194,-32768,
  1257,-32768,-32768,-32768,  5437,   105,  5437,-32768,-32768,  5437,
  1088,-32768,  6822,  5437,  1436,   417,-32768,  1176,  6822,  1207,
  6822,   723,-32768,   389,-32768,-32768,-32768,  1181,-32768,-32768,
-32768,   144,  5437,-32768,-32768,-32768,-32768,  1430,-32768,  1189,
  1413,-32768,  1437,-32768,  1384,  1385,   487,-32768,-32768,-32768,
-32768,-32768,  1450,-32768,-32768,-32768,-32768,-32768,  5437,  7310,
   352,  1348,  1349,  1350,-32768,   352,-32768,   882,-32768,-32768,
  1321,   723,-32768,  5382,   260,  1324,-32768,-32768,-32768,-32768,
   352,-32768,  1091,   528,-32768,-32768,-32768,  1441,   528,  1323,
   528,-32768,   352,   352,  1215,   352,-32768,   936,  5437,-32768,
  1392,-32768,   750,-32768,-32768,-32768,   497,  1030,  5437,   751,
    66,-32768,-32768,  1218,   446,-32768,  5382,  5437,  1030,  4851,
-32768,-32768,  1030,  4413,-32768,-32768,-32768,   124,-32768,   237,
  1477,  4632,  4632,  6822,  6822,  1851,  1219,   721,  2216,  6822,
  2473,   452,  2730,  2980,  5946,  3230,  3480,  3730,-32768,-32768,
  1387,-32768,  1223,   759,  6822,  6822,   282,  6822,  1225,  6822,
  1229,   675,  6822,  6822,  6822,  6822,  6822,  6822,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  1434,   327,   187,  6822,
-32768,  4632,  6822,   447,   594,  1069,   119,-32768,   759,-32768,
-32768,   233,-32768,-32768,  1475,   151,   151,   663,   663,  1264,
-32768,   760,-32768,-32768,-32768,-32768,-32768,   781,-32768,  1318,
  1488,  1380,-32768,-32768,  5437,  5437,  5437,  5437,  5437,  5437,
  1238,-32768,   317,-32768,-32768,  3367,  1388,-32768,   127,  1402,
  5437,-32768,  6822,  1402,  1238,  1239,  3367,  3614,  1443,-32768,
-32768,  3560,  1242,  1524,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1248,  4632,  1444,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    90,-32768,-32768,  1284,  1264,-32768,-32768,
   787,  1030,-32768,-32768,-32768,  1264,-32768,   946,  1510,-32768,
   389,-32768,  1267,  4632,  1539,-32768,-32768,-32768,-32768,-32768,
-32768,  1380,-32768,-32768,  1539,-32768,-32768,-32768,-32768,   487,
   487,  1298,  1268,-32768,-32768,-32768,-32768,-32768,   128,   271,
   775,-32768,  1513,-32768,-32768,-32768,  5437,-32768,-32768,-32768,
-32768,   473,   844,  1383,  1399,  1271,-32768,  1151,   853,-32768,
-32768,   684,-32768,  1273,  5437,   692,  1277,  1283,   352,  1291,
  1548,   530,  1380,   946,  1233,   417,-32768,  1208,  1357,  2003,
-32768,   352,  1426,  1514,-32768,  3276,-32768,   352,   451,-32768,
-32768,  1515,-32768,-32768,  1295,  1297,-32768,  4632,  4632,-32768,
  4632,  4632,-32768,  1560,   170,  1264,  1300,-32768,-32768,  1301,
  1264,  5508,-32768,-32768,  1512,-32768,   627,  1306,  1308,  1264,
  1286,  1311,  1312,  1264,  6822,  6822,  1219,  6822,  6165,  1316,
  1317,  1264,  1319,  1326,  1264,  6822,  1264,  1327,  1329,  1264,
  1331,  1333,  1264,  1335,  1337,  1264,-32768,  2659,-32768,   722,
   854,-32768,   873,-32768,   884,  6822,-32768,   927,   965,   998,
  1029,  1037,  1103,  6822,  6822,  6822,  1264,  1531,   195,  1264,
  1414,  6822,-32768,  6822,  6822,  6822,  6822,-32768,-32768,-32768,
  5437,-32768,-32768,  5437,  6924,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  3367,  1541,  3367,-32768,  1238,   317,-32768,
   511,   511,     9,-32768,-32768,   858,-32768,  1238,  1239,  1322,
  3614,  1338,   753,  1339,  4725,  1487,-32768,  1487,   631,  1487,
  1530,  1535,  6822,  1738,  1597,  1550,-32768,-32768,-32768,   759,
  1380,-32768,  1357,   498,  1151,  1496,  1151,-32768,  4632,   126,
   771,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1539,
  1539,-32768,  1488,  6822,-32768,   792,  1482,  1298,   729,-32768,
  4632,-32768,-32768,-32768,  1503,  1506,  1509,  1516,  1517,  1520,
-32768,-32768,-32768,-32768,-32768,  1372,   473,-32768,  1451,-32768,
-32768,   528,-32768,  1539,   182,   528,-32768,   528,-32768,  1399,
-32768,-32768,  1375,-32768,-32768,   497,  1488,  1151,-32768,  1233,
-32768,-32768,  1618,-32768,-32768,  1377,-32768,  1176,-32768,  1389,
   723,   154,  1393,-32768,-32768,   723,-32768,-32768,-32768,  1560,
-32768,  1560,  6822,  1030,  1030,  1395,    96,   862,  6822,  1593,
-32768,  1030,  1030,  6822,  1030,  1030,   174,  1264,-32768,  1294,
  6822,  1264,  1030,  1030,  1030,  1030,  1264,  1030,  1030,  1030,
  1030,  1030,  1030,-32768,  1396,  1386,   757,  5437,-32768,-32768,
  1397,  3321,-32768,-32768,  1116,-32768,-32768,-32768,-32768,-32768,
-32768,  1137,  1213,-32768,-32768,  6822,  6822,  1608,  1489,  1145,
   874,  1183,-32768,  3321,  1401,-32768,-32768,  1630,  1637,  4944,
   362,  1405,  1406,  1600,  6822,  1412,  1416,  5163,  5437,  5437,
-32768,-32768,-32768,  1639,  2605,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   120,-32768,  1420,
-32768,-32768,-32768,  1610,  1422,  1423,  1425,  1427,  1424,   687,
-32768,-32768,  1573,-32768,-32768,-32768,-32768,  1529,-32768,-32768,
-32768,-32768,-32768,-32768,  3614,  1322,  1695,-32768,  1524,-32768,
-32768,-32768,  1621,-32768,  4725,  4725,   883,  5437,  6822,  4632,
  1088,-32768,  1488,-32768,-32768,-32768,-32768,   882,-32768,-32768,
   131,-32768,-32768,-32768,-32768,-32768,  1264,  1685,   792,-32768,
   741,  5437,-32768,  1642,   137,  1543,  1629,  1544,  1682,  1647,
  1684,  1380,-32768,-32768,  1455,-32768,  1456,-32768,  1643,  1685,
   182,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1380,
  1208,-32768,   882,  1513,  1625,-32768,-32768,-32768,  1641,    94,
-32768,-32768,  1399,  1655,  1264,  1476,  1479,-32768,   759,-32768,
  1264,  6822,  1480,  1494,  1264,  1495,  1497,  6822,  6822,  1264,
  1500,  1501,  1504,  1505,  1518,  1519,  1521,  1522,  1523,  1526,
-32768,  2659,  1507,-32768,  5437,-32768,-32768,-32768,-32768,-32768,
-32768,  6822,  1527,-32768,-32768,-32768,  6822,-32768,-32768,  6822,
   169,-32768,  1709,-32768,  6384,  5437,  6822,-32768,  4632,  1264,
-32768,-32768,-32768,  1532,-32768,-32768,-32768,  5437,-32768,   212,
-32768,  1588,-32768,   468,  1533,-32768,  4506,-32768,-32768,-32768,
-32768,-32768,-32768,   511,  5437,  4725,-32768,   393,-32768,-32768,
  1511,-32768,  1264,   209,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  1534,  5437,-32768,-32768,  1692,  1702,  5437,  1298,
-32768,-32768,   487,  5437,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1488,-32768,-32768,-32768,-32768,-32768,  1488,-32768,-32768,
-32768,-32768,-32768,  6822,   723,-32768,  1586,-32768,   723,-32768,
-32768,-32768,  1264,-32768,-32768,-32768,-32768,  1264,  1264,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  5437,  1715,  1264,-32768,-32768,  1170,  1539,-32768,-32768,  1536,
-32768,  1190,  6603,  1198,   141,  1540,-32768,  1722,-32768,  5437,
  5437,   473,   138,-32768,-32768,  1559,  1728,  4632,-32768,   791,
-32768,  1538,-32768,  1546,-32768,-32768,  4632,  1549,-32768,-32768,
-32768,  6822,-32768,-32768,-32768,-32768,-32768,  1298,   487,-32768,
  1690,-32768,-32768,-32768,-32768,  1513,-32768,  1264,-32768,-32768,
-32768,  1547,  1545,  1552,-32768,-32768,-32768,  1030,-32768,  1665,
  4506,-32768,  1653,-32768,  4506,-32768,-32768,-32768,-32768,  7058,
   212,  6822,  4506,   145,-32768,  4506,  5601,  5437,-32768,   209,
  5437,-32768,-32768,  1690,   351,  1298,  1705,  1706,-32768,-32768,
  1555,-32768,  5437,  5437,  1562,  4287,  1566,   796,  7058,  1567,
-32768,-32768,  1250,   125,  1791,-32768,-32768,   895,-32768,   902,
  1298,   101,   101,-32768,  1796,-32768,  1666,-32768,-32768,-32768,
-32768,  1581,-32768,  4506,  1567,-32768,-32768,  1806,  4506,  1822,
  1812,  7058,  5437,-32768,-32768,-32768,  1660,   443,-32768,-32768,
-32768,-32768,   910,  7058,   159,  5437,  7058,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  7058,-32768,-32768,-32768,  1862,  1863,
-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,   -59,   165,-32768,   263,  1591,   633,-32768,  -341,
-32768,-32768,  -629,  -157,  -642, -1040,  1854,  1592,  1584,  1131,
-32768,-32768,-32768,   752,-32768,  1227,  1009,-32768,-32768,   754,
-32768,-32768,   992,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   765,-32768,-32768,-32768,
-32768,-32768,   343,-32768,-32768,  1537,-32768,-32768,-32768,  -334,
-32768,-32768,-32768,  1508,-32768,-32768,  1289,  -355,-32768,-32768,
-32768,   842,-32768,  -849,-32768,-32768,  1558,  1108,   779,  1553,
  1083,   780,-32768,   365,   -54,  -244,-32768,  -324,   301, -1031,
-32768,-32768,   395,-32768,  1556, -1175,-32768,-32768,-32768,-32768,
 -1192,   394,   108,   106,   115,    82,-32768,-32768,-32768,-32768,
  -118,  -250,-32768,  1132,-32768,  -634,-32768,  -958,-32768,   951,
-32768,-32768,-32768,-32768,-32768, -1284,  -912,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  -256,   204,-32768,  -173,-32768,-32768,-32768,-32768,-32768,
   302,-32768,   139,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  1209,-32768,-32768,  -511,   434,  -338,  -891,   669,
-32768,-32768,  -541,  -764,-32768,-32768, -1010, -1392,-32768,-32768,
-32768,-32768,-32768,  1596,-32768,  1355,  1579,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  1604,-32768,-32768,-32768,-32768,
-32768,  -551,  -454,-32768,  1090,   626,  -558,  -367,-32768,-32768,
-32768,  1346,  -983,-32768,  -365,-32768,-32768,-32768,-32768,  1330,
-32768,-32768,  1106,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1943,-32768,-32768,-32768,-32768,-32768,  1645,
-32768,-32768,-32768,  1439,-32768,  1440,-32768,-32768,-32768,-32768,
  1194,-32768,-32768,-32768,     1,-32768,-32768,-32768,  -426,   459,
-32768,  -345,   798,-32768,  -193,   -73,-32768,-32768,-32768,-32768,
  1813,-32768,-32768,-32768,  1202, -1139,-32768,-32768,  -914, -1243,
-32768,-32768,-32768,-32768,   960,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1886,    20,   718,   -69,-32768,-32768,   220,
-32768,  -397,  -740,  1292,-32768,   381,-32768,   585,-32768,  -729,
-32768,  -614,-32768,  1200,  -512,-32768,-32768,  -852,  1991,  1994,
-32768,-32768,  1460,  1997,-32768,-32768,-32768,  -203,-32768,  1252,
 -1125,  1000,-32768,  -101,  -684,  -535,-32768,-32768,-32768,  -174,
   -44,  -403,  -666,-32768,  -428,-32768,-32768,-32768,   482,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -524,-32768,
   834,-32768,   993,  -994,-32768,  -389,-32768,   726,  -963,  -128,
  -918,  -911,  -898,   -38,    -2,   -53,-32768,   -58,  -591,  -491,
  -116,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   788,-32768,
 -1084,-32768,   391,-32768,  -889,    -5,  1162,   903,   346,  -714,
  1429, -1157,-32768,   -62,  1924,-32768,   -26,   -49, -1583,   577,
   -40,   -42,  -152,    -1,   -27,  -236,-32768,   -72,   -25,    -9,
-32768
};


#define	YYLAST		7572


static const short yytable[] = {   136,
    41,    80,   136,   227,   324,   276,   158,   205,   164,   788,
   223,   568,   975,   631,   157,   635,   235,   208,   295,   298,
   224,   605,   728,   267,   258,   221,   812,   570,   233,   767,
   296,   299,   245,  1295,   264,   156,   949,   790,   596,   235,
  1037,   262,   957,   494,   240,   201,   203,   206,   209,   158,
   214,   263,   715,   494,   268,   226,   203,   230,   269,   209,
  1283,   158,   214,   261,   243,   136,  1428,   266,  1226,  1039,
  1283,  1189,   750,   254,   924,   158,   257,   203,   210,   254,
   230,   206,   209,   157,   158,   214,   243,   136,   331,   209,
   238,  1285,   265,   282,   748,   284,   713,  1253,   959,   979,
   886,  1285,   513,  1293,   156,  1523,  1218,  1342,   575,  1006,
   877,  1320,  1118,   572,   386,   801,  1480,   802,   327,   536,
  1521,  1414,  1005,   813,  1498,   306,   902,  1418,   226,   158,
   226,  1138,  1072,   902,  1860,   646,  1286,   781,   902,   332,
   393,   892,   230,  1287,   902,   300,  1286,   325,   902,   209,
   158,   243,   902,  1287,  1337,   164,  1288,   776,   283,   554,
  1631,  1692,   753,   355,   713,  1289,  1288,  1292,   860,   763,
   539,   525,  1560,   719,  1296,  1289,   142,  1363,   720,   329,
  1634,  1578,   719,  1302,  1800,   928,  1057,   720,   382,  1241,
  1696,   600,   719,  1472,   902,  1475,   719,   720,  1122,    99,
   793,   720,   902,  -370,  1091,  1885,   865,  1305,   540,  1325,
   631,  1065,   635,   356,   387,   771,   902,  1867,   743,   525,
  1739,   169,  -650,   514,  1839,   481,   503,   360,   485,   542,
   361,   520,  1267,   769,    62,  1143,  1144,   891,   554,   290,
    85,   823,   899,   903,  1141,   397,  1032,   605,  1139,   140,
   903,  1647,   312,   402,   742,   903,  1868,   522,  1514,  1515,
   226,   903,  1033,   226,   165,   903,  1740,   158,   526,   903,
   495,   136,   362,   170,   143,  1042,   158,  1741,   537,  1879,
   495,  1626,  1627,  1268,   772,  1209,   357,   294,   858,   785,
  1060,  1192,  1538,  1062,   866,  1283,  1283,   206,   744,  1539,
  1242,   158,  1069,  1869,   719,   388,   569,  1306,   288,   720,
  1635,   903,  1503,   481,  1307,   224,   719,   583,   100,   903,
   587,   720,   527,  1697,   555,   713,  1285,  1285,   962,  1283,
   532,  1604,  1605,   903,   -81,   597,   972,   599,  1123,   528,
  1489,   226,   643,  1521,   654,   539,  1548,  1645,   226,   719,
   226,   650,   226,  1239,   720,   588,   645,  1561,  1113,  1206,
  1285,  1142,  1099,  1024,  1249,  1250,  1238,  1416,   777,  1137,
   527,  1286,  1286,   209,   214,  1076,  1742,  1248,  1287,  1287,
  1516,   246,   158,  1820,   206,  1297,  1298,  1280,   663,  1205,
  1217,  1288,  1288,   719,  1842,   661,  1806,  1140,   720,  1512,
  1289,  1289,  1756,   601,  1660,  1286,   289,  1058,   929,  1801,
  1675,   652,  1287,  1009,  1793,   247,   102,  1502,  1835,   723,
   724,  1533,  1270,   861,  1552,  1288,   721,   722,   723,   724,
  1806,  -650,  1727,   717,  1289,  1347,   721,   722,   723,   724,
   721,   722,   723,   724,  1475,   754,   576,   856,   281,   741,
  1308,  1119,   104,   537,   719,  1091,  1256,  1053,   655,   720,
  1055,   891,  1135,   741,  1129,   891,  1136,   791,  1165,  1257,
  1738,  1359,  1360,  1067,  1361,  1362,   158,  1770,   838,   308,
   209,   495,  1166,   756,   280,  1881,  1048,   158,   166,   950,
   951,   158,  1258,   832,   209,   158,  1212,  1213,  1083,   164,
   485,   589,  1616,   215,   725,   738,  1259,   494,  1235,  1219,
   231,   590,   309,   857,  1757,  1831,   746,   105,  1753,   794,
   751,   566,  1725,  1746,   868,   167,  1167,   106,  1271,   833,
  1843,  1092,   366,   814,   567,   226,  1882,   839,   310,  1130,
  1689,   311,   235,  1260,  1858,  1290,  1291,    78,   721,   722,
   723,   724,   107,   292,  1168,  1049,  1126,   312,   572,   840,
   721,   722,   723,   724,  1010,  1304,   807,   886,   226,    54,
  1336,  1551,   807,  1682,   152,  1813,  1036,  1878,   138,  1698,
    55,  1648,   281,   313,  1348,  1252,  1423,    56,  1236,  1884,
  1656,  1688,  1887,   721,   722,   723,   724,  1245,   719,   249,
  1888,   649,   976,   720,   399,   367,   224,  1617,  1486,   871,
   872,    57,  1511,  1246,   108,  1273,   719,  1111,  1169,   815,
  1661,   720,    14,  1844,  1662,  1663,   383,   539,   952,   834,
   293,  1758,   109,   110,  1525,  1214,   294,   721,   722,   723,
   724,   226,   139,   111,   112,   719,   113,   348,  1866,  1029,
   720,  1483,   919,   114,   115,  1261,   116,   117,  1659,   118,
  1690,   384,   119,   120,   121,   122,   123,   931,    78,   251,
   124,   125,  1370,    58,   153,   920,   126,   127,   128,   844,
  1354,   154,   259,   101,   230,   719,   780,   481,   844,   481,
   720,   984,   349,   270,  1256,  1787,   816,   719,   721,   722,
   723,   724,   720,  1691,    59,   758,    78,  1257,    60,   964,
   217,   218,  1355,  1275,   741,    78,  1702,   226,  1886,   292,
  1371,   102,   226,   292,   226,  1294,   226,  1536,  1505,   495,
  1258,   921,  1283,   103,   495,   537,   817,  1303,  1506,   246,
    79,   224,   495,   342,   719,   209,   765,   141,  1228,   720,
   209,  1231,  1569,  1047,  1027,   343,   272,   104,   481,   273,
  1655,  1324,  1368,  1285,  1763,   158,  1155,   158,  1764,  1774,
   292,   300,  1078,   247,   226,  1775,   226,   235,    79,  1338,
   277,  1260,  1082,  1773,  1074,  1077,  1524,    79,   344,   294,
   345,   766,   845,   226,   532,  1098,  1035,  1096,  1126,   350,
  1777,   147,   148,  1088,  1107,  1108,  1367,  1110,  1286,   235,
   235,   301,   235,  -232,  1156,  1287,  1256,   351,  1309,   226,
   226,   576,   105,  1550,   337,   159,  1421,   278,  1288,  1257,
  1537,   279,   106,  1654,   663,  1593,  1547,  1289,  1157,   285,
   300,  1089,   721,   722,   723,   724,   286,   297,   539,   960,
  1599,   352,  1258,   294,  1310,   287,   160,   107,  1667,   158,
   721,   722,   723,   724,   338,  1215,  1259,  1402,   300,   158,
  1594,   161,   300,  1497,    14,   108,   719,   663,  1134,  1668,
   925,   720,  1595,  1819,   661,  1669,  1821,   291,  1112,   721,
   722,   723,   724,   109,   110,   719,  1011,   148,  1120,   372,
   720,   749,   292,  1260,   111,   112,   719,   113,   933,  -232,
   300,   720,   935,   323,   114,   115,  1232,   116,   117,  1670,
   118,   217,   218,   119,   120,   121,   122,   123,  1849,  1028,
  -232,   124,   125,   373,   307,   374,  -232,   126,   127,   128,
   333,   721,   722,   723,   724,  1229,   334,  1233,  1197,   719,
   937,   732,  1735,   733,   720,  1326,   537,   375,   101,  1230,
  1327,  -996,   336,  1326,  -999,   418,   419,   420,  1330,  1611,
  -232,   736,  1532,   737,   358,   495,   209,   214,   495,   243,
   136,  1029,   376,   803,   793,   804,   495,   719,   721,   722,
   723,   724,   720,   428,   429,  1399,   102,   495,   495, -1018,
    82,    83,   209,  1554,   725, -1018,   961,   825,   103,   826,
 -1018,  1084,  1085, -1018,  1001,  1261,  1002,   340,  1367,  1367,
   719,  1116,   882,  1117,  1121,   720,  1367,  1367,   371,  1367,
  1367,   725,   104,  1220,   631,   380,   635,  1367,  1367,  1367,
  1367,   369,  1367,  1367,  1367,  1367,  1367,  1367,   390, -1018,
  1333,   719,  1221,   394,  1222,   235,   720, -1018,  1116,   719,
  1272,  1805,  1806,  1349,   720, -1018,  1857,  1806,   235,  1353,
   392,   224,   539,   395,   235,   485,  1471,  1315,  1474,   398,
   926,  1804,   930, -1018,   482, -1018,   934,   936,   938,  1316,
  1810,   719,   975,  1851,  1852,  1315,   720,   105, -1018,   490,
 -1018, -1018, -1018,   399,   497, -1019,   226,   106,   498,   501,
   226, -1019,  1479,  1479,  1482,  1317, -1019,  1318,   502, -1019,
   721,   722,   723,   724,  1322,   719,  1323,  1400,   504,   725,
   720,  1484,   107,   725,   551,  1570,   552,   505,   719,   721,
   722,   723,   724,   720,   508,   725,  1403,  1609,   506, -1018,
   721,   722,   723,   724,   725, -1019,  1649,  1404,  1508,   719,
  1510,   523,   521, -1019,   720,   524,  1863,   719,  1864,  1424,
   533, -1019,   720,   736,   543,  1865,   481,   545,   158,   224,
   537,  1806,   823,  1883,    14,   547,  1398,   559,  1492, -1019,
  1494, -1019,   719,   721,   722,   723,   724,   720,   563,   560,
  1406,   561,   581,   584, -1019,   719, -1019, -1019, -1019,   594,
   720,   226,   719,   598,  1429,  1470,   647,   720,  1694,   651,
   719,  1553,   688,   653,   495,   720,   495,   690,   691,   692,
   693,   721,   722,   723,   724,   719,   694,   700,  1407,   695,
   720,   495,   696,   697,   698,   209, -1018, -1018, -1018, -1018,
 -1018, -1018, -1018,  -982,   699, -1019,   702,   701, -1018,   707,
 -1018,   448,   727,   708,   721,   722,   723,   724,   731,   709,
   718,  1408,   719,  1602,   725,  1840, -1020,   720,   726,  1791,
   734,  -999, -1020,   735,   739,   511,   719, -1020,   752,   755,
 -1020,   720,   539,   744,   519,   721,   722,   723,   724,   760,
   779,   784,  1409,   721,   722,   723,   724,   762,   719,   775,
  1410,   783,  1559,   720,  1535,   786,   719,  1564,   787,   792,
   789,   720,   554,   795,   796,   798, -1020,   799,   550,   805,
   829,  1859,   553,   830, -1020,   721,   722,   723,   724,  1374,
  1216,   831, -1020,   836,  -429,  1650,   292,  1579,   841,   848,
  1776,   842, -1019, -1019, -1019, -1019, -1019, -1019, -1019,   849,
 -1020,   850, -1020,   851, -1019,   853, -1019,  1615,   854,   721,
   722,   723,   724,   859,   869, -1020,  1411, -1020, -1020, -1020,
  1749,   863,   721,   722,   723,   724,   864,   870,   158,  1600,
  1159,   879,  1163,   881,  1171,  1174,  1398,  1179,  1182,  1185,
   537,   882,   888,   721,   722,   723,   724,   887,   889,   169,
  1601,   721,   722,   723,   724,   893,   953,   958,  1608,   860,
   203,   980,   996,   997,   294,   999, -1020,   704,  1625,  1134,
  1134,  1003,   710,   711,   714,  1470,   721,   722,   723,   724,
  1023,  1007,  1030,  1786,  1008,  1040,  1044,   736,  1063,   721,
   722,   723,   724,  1054,  1610,  1065,   721,   722,   723,   724,
  1789,  1064,  1751,   808,   721,   722,   723,   724,  1792,  1070,
  1071,  1073,  1079,  1080,  1081,   495,  1087,  1094,  1105,   721,
   722,   723,   724,  1100,  1752,   209,   209,  1109,   325,  1114,
  1128,  1152,  1755,   904,  1187,  1188,  1204,   719,  1194,   719,
  1196,  1223,   757,  1225,   720,  1251,   759,   905,  1243,  1234,
  1236,  1240,   158,  1255,  1265,  1479,   721,   722,   723,   724,
  1263,  1276,  1311, -1020, -1020, -1020, -1020, -1020, -1020, -1020,
   721,   722,   723,   724,  1749, -1020,  1299, -1020,  1749,  1279,
  1301,  1673,  1319,   906,  1321,  1722,  1749,  1328,  1343,  1836,
  1331,   907,   721,   722,   723,   724,  1332,  1335,  1122,   908,
   721,   722,   723,   724,  1334,  1351,  1356,   902,  1357,  1749,
  1358,  1771,  1364,  1365,  1369,  1733,   406,   909,  1372,   910,
  1373,  1415,   158,  1375,  1376,   206,  1417,  1256,  1383,  1384,
  1398,  1385,   911,  1485,   912,   913,   914,  1749,  1386,  1388,
  1257,  1389,  1749,  1390,   408,  1391,   209,  1392,  -690,  1393,
  1473,  1487,  1488,  1490,  1495,  1499,  1751,  1828,  1134,  1496,
  1751,  1830,  1500,  1258,  1772,  1509,  1522,  1526,  1751,  1834,
  1527,  1837,   410,  1528,  1747,   325,   209,  1259,  1752,  1555,
  1529,  1530,  1752,   915,  1531,  1534,  1091,   539,  1549,  1557,
  1752,  1751,  1855,  1752,   964,  1572,  1779,  1592,  1606,   158,
  1781,  1607,  1558,   901,   206,  1613,  1563,  1814,  1568,  1591,
  1598,  1612,  -988,  1752,  1260,  1618,   539,  1796,  1619,  1751,
  1873,   139,  1621,   927,  1751,  1875,  1622,   932,  1769,  1629,
  1636,  1637,  1638,  1639,   948,  1640,  1643,  1641,  1642,   955,
   956,  1752,  1644,   415,   763,  1646,  1752,  1664,  1677,   539,
   448,   206,   965,   966,   967,   968,   969,   970,   448,   417,
  1674,   539,  1676,  1678,   539,  1679,  1680,  1681,  1683,  1684,
   203,  1798,   539,   418,   419,   420,  1685,   217,   218,  1799,
   916,   917,   918,   721,   722,   723,   724,   763,  1693,  1700,
   960,  1695,  1701,  1704,   519,   537,   423,   424,   425,   426,
   427,   428,   429,   430,   431,  1699,   485,  1705,  1706,  1730,
  1707,   433,   434,  1710,  1711,  1038,  1630,  1712,  1713,  1721,
   438,   550,  1762,  1046,   537,  1767,  1261,  1768,  1825,  1780,
  1470,  1714,  1715,  1784,  1716,  1717,  1718,   226,  1027,  1719,
  1724,   226,  1737,  1795,  1802,  1748,  1281,  1765,  1788,  1803,
  1794,  1815,  1826,   206,   206,  1807,  1823,   537,  1808,  1470,
  1822,  1811,  1566,  1567,  1824,  1829,  1845,  1847,  1850,   537,
  1573,  1574,   537,  1576,  1577,  1853,  1856,  1862,  1806,  1842,
   537,  1581,  1582,  1583,  1584,  1843,  1585,  1586,  1587,  1588,
  1589,  1590,  1470,  1027,  1872,  1874,  1147,  1876,  1877,  1148,
  1880,  1890,  1891,   403,  1470,   145,  1134,  1470,   489,    64,
   404,  1476,   496,   507,   998,  1470,  1350,  1352,  1132,  1340,
    65,   880,   405,  1687,   564,   606,   541,    66,   406,   407,
  1127,   843,  1278,  1041,  1341,  1068,  1145,  1146,  1151,  1346,
    67,  1160,  1161,  1164,  1686,  1172,  1175,  1177,  1180,  1183,
  1186,  1728,   577,  1666,  1672,   579,   408,  1190,  1191,   409,
  1193,  1841,  1195,  1848,  1871,  1198,  1199,  1200,  1201,  1202,
  1203,  1846,  1227,  1745,  1061,  -653,  1790,  1657,   828,  1832,
   977,  1504,  1207,   585,   410,  1210,   411,   412,   571,   562,
  1106,  1546,   846,  -653,  -653,   146,   413,    68,  1097,   534,
   414,   770,   305,  1025,  -653,  -653,   774,  -653,  1031,   867,
   255,  1254,  1720,    69,  -653,  -653,  1493,  -653,  -653,   922,
  -653,  1812,  1597,  -653,  -653,  -653,  -653,  -653,  1043,    70,
    46,  -653,  -653,    47,   782,   448,    50,  -653,  -653,  -653,
  1379,  1419,  1004,  1247,    71,  1090,  1513,  1329,    72,  1766,
   824,    73,   260,  1149,  1624,   415,     0,     0,     0,    74,
     0,   416,     0,    86,     0,     0,     0,     0,     0,   808,
    87,   417,     0,     0,    88,    89,     0,     0,    75,     0,
     0,     0,     0,    90,     0,   418,   419,   420,   421,   217,
   218,     0,     0,     0,    91,     0,     0,     0,     0,   422,
     0,     0,     0,     0,     0,     0,   109,   110,   423,   424,
   425,   426,   427,   428,   429,   430,   431,   111,   432,     0,
   113,     0,     0,   433,   434,     0,   435,   436,   437,     0,
   116,   117,   438,   118,     0,   101,   119,   120,   121,   122,
   123,     0,     0,     0,   124,   125,     0,     0,     0,     0,
   126,   439,   128,     0,     0,     0,    92,   440,   441,     0,
     0,     0,     0,   510,     0,   443,     0,     0,     0,   294,
     1,     0,    93,   102,   939,   940,   941,   942,   943,   944,
   945,   946,     0,     0,   448,   103,     0,    94,     2,     0,
     0,    95,     0,     0,     0,     0,     3,  1377,  1378,     0,
  1380,  1382,    96,     4,     0,     5,     0,     6,  1387,   104,
     0,     0,     7,     0,     0,     0,     0,     0,     0,     8,
     0,    97,     0,     0,     0,     0,     0,     0,  1405,     0,
     0,     0,     0,     9,     0,     0,  1412,  1413,  1207,     0,
   108,     0,     0,    10,  1207,     0,  1210,  1420,   448,  1422,
     0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     0,  1158,     0,     0,  1148,     0,     0,     0,   403,   111,
   112,     0,   113,     0,   105,   404,     0,     0,     0,   114,
   115,     0,   116,   117,   106,   118,     0,   405,   119,   120,
   121,   122,   123,   406,   407,   448,   124,   125,     0,     0,
     0,     0,   126,   127,   128,     0,    11,     0,     0,   107,
     0,     0,     0,    12,     0,   273,    13,     0,    14,    15,
     0,   408,     0,     0,   409,     0,  1517,   108,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    16,   904,   109,   110,     0,     0,   410,
   719,   411,   412,     0,     0,   720,   111,   112,   905,   113,
     0,   413,     0,     0,     0,   414,   114,   115,     0,   116,
   117,     0,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,     0,     0,     0,     0,   126,
   127,   128,    17,     0,   906,  1565,     0,     0,     0,     0,
     0,  1571,   907,     0,     0,     0,  1575,    18,    19,     0,
   908,     0,     0,  1580,     0,     0,     0,     0,  1149,     0,
   415,     0,     0,     0,     0,     0,   416,     0,   909,     0,
   910,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,   911,     0,   912,   913,   914,  1207,  1207,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,     0,     0,   422,     0,     0,  1620,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,     0,   433,   434,
     0,   435,   436,   437,   915,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,     0,     0,     0,     0,   126,   439,   128,  1162,     0,
     0,  1148,   440,   441,     0,   403,     0,     0,   510,     0,
   443,  1653,   404,     0,   294,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
   406,   407,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   408,     0,
     0,   409,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   916,   917,   918,   721,   722,   723,   724,     0,     0,
     0,     0,     0,     0,  1703,     0,   410,     0,   411,   412,
  1708,  1709,     0,     0,     0,     0,     0,     0,   413,     0,
     0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1723,     0,     0,     0,     0,  1207,
     0,     0,  1726,     0,     0,     0,     0,  1732,     0,  1734,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  1225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,  1149,     0,   415,     0,     0,
     0,     0,     0,   416,     0,     0,     0,     0,     6,     0,
     0,     0,     0,   417,     0,  -287,     0,     0,     0,  1430,
  1431,     0,  1432,     0,     0,     0,     0,   418,   419,   420,
   421,   217,   218,     0,     0,     0,     0,     0,  1433,     0,
     0,   422,     0,     0,  1434,     0,  1778,     0,   109,   110,
   423,   424,   425,   426,   427,   428,   429,   430,   431,   111,
   432,     0,   113,     0,     0,   433,   434,     0,   435,   436,
   437,     0,   116,   117,   438,   118,     0,     0,   119,   120,
   121,   122,   123,     0,     0,   448,   124,   125,     0,     0,
     0,     0,   126,   439,   128,  1170,     0,     0,  1148,   440,
   441,  1435,   403,   919,     0,   510,     0,   443,     0,   404,
     0,   294,     0,     0,  1653,     0,     0,     0,     0,    14,
     0,   405,     0,     0,     0,     0,   920,   406,   407,     0,
     0,     0,     0,     0,  1436,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    16,     0,     0,     0,     0,     0,
     0,     0,     0,  1630,  1833,   408,     0,     0,   409,     0,
     0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   109,   110,   921,   410,     0,   411,   412,     0,     0,     0,
     0,  1437,   112,   582,   113,   413,     0,     0,     0,   414,
     0,   114,   115,     0,   116,   117,     0,   118,     0,     0,
   119,  1438,   121,   122,   123,     0,   108,     0,   124,   125,
  1439,  1440,  1441,     0,   126,   127,   128,     0,     0,     0,
     0,     0,     0,     0,   109,   110,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   111,   112,     0,   113,     0,
     0,     0,  1149,     0,   415,   114,   115,     0,   116,   117,
   416,   118,   973,     0,   119,   120,   121,   122,   123,     0,
   417,     0,   124,   125,     0,     0,     0,     0,   126,   127,
   128,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,     0,   433,   434,     0,   435,   436,   437,     0,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,  1173,     0,     0,  1148,   126,
   439,   128,   403,     0,     0,     0,   440,   441,     0,   404,
     0,   108,   510,     0,   443,     0,     0,     0,   294,     0,
     0,   405,     0,     0,     0,     0,     0,   406,   407,   109,
   110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   111,   112,     0,   113,     0,     0,     0,     0,     0,     0,
   114,   115,     0,   116,   117,   408,   118,     0,   409,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,     0,
     0,   108,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,     0,   410,     0,   411,   412,     0,     0,   109,
   110,     0,     0,     0,     0,   413,     0,     0,     0,   414,
   111,   112,     0,   113,     0,     0,     0,     0,     0,     0,
   114,   115,     0,   116,   117,     0,   118,     0,     0,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,     0,
     0,     0,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,   974,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  1149,     0,   415,     0,     0,     0,     0,     0,
   416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   417,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,     0,   433,   434,     0,   435,   436,   437,     0,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,  1178,     0,     0,  1148,   126,
   439,   128,   403,     0,     0,     0,   440,   441,     0,   404,
     0,     0,   510,     0,   443,     0,     0,     0,   294,     0,
     0,   405,     0,     0,     0,     0,     0,   406,   407,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   883,     0,   408,   608,     0,   409,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   884,
     0,     0,     0,   609,     0,     0,   610,     0,     0,     0,
     0,     0,     0,   410,     0,   411,   412,     0,     0,     0,
     0,     0,     0,     0,     0,   413,     0,     0,   607,   414,
     0,   608,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   611,     0,   609,     0,
     0,   610,   612,   613,   614,     0,   615,   616,   617,     0,
   618,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   619,     0,
   620,     0,  1149,     0,   415,     0,     0,     0,     0,     0,
   416,   611,     0,     0,     0,     0,     0,   612,   613,   614,
   417,   615,   616,   617,     0,   618,     0,     0,     0,   621,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,   619,     0,   620,   622,   985,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,   623,   113,
     0,     0,   433,   434,   621,   435,   436,   437,     0,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
     0,   622,     0,   124,   125,  1181,     0,     0,  1148,   126,
   439,   128,   403,   624,   625,     0,   440,   441,     0,   404,
     0,     0,   510,   623,   443,     0,     0,   986,   294,     0,
   626,   405,     0,     0,     0,     0,     0,   406,   407,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   624,   625,
     0,     0,   987,     0,     0,   408,     0,   988,   409,     0,
     0,     0,     0,   989,     0,   626,     0,     0,     0,     0,
     0,     0,     0,     0,   108,     0,     0,     0,     0,     0,
     0,     0,   990,   410,     0,   411,   412,     0,     0,     0,
     0,     0,   109,   110,     0,   413,     0,     0,     0,   414,
     0,     0,     0,   111,   112,     0,   113,     0,     0,     0,
     0,     0,     0,   114,   115,     0,   116,   117,     0,   118,
     0,     0,   119,   120,   121,   122,   123,     0,     0,     0,
   124,   125,     0,     0,     0,     0,   126,   127,   128,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  1149,     0,   415,     0,     0,     0,     0,     0,
   416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   417,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,   985,     0,     0,     0,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,     0,   433,   434,    14,   435,   436,   437,     0,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,  1184,     0,     0,  1148,   126,
   439,   128,   403,     0,     0,     0,   440,   441,     0,   404,
     0,     0,   510,     0,   443,     0,     0,   108,   294,     0,
     0,   405,     0,     0,     0,     0,     0,   406,   407,     0,
     0,     0,     0,     0,     0,   109,   110,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   111,   112,     0,   113,
     0,     0,     0,     0,   988,   408,   114,   115,   409,   116,
   117,     0,   118,     0,     0,   119,   120,   121,   122,   123,
     0,   108,     0,   124,   125,     0,     0,     0,     0,   126,
   127,   128,     0,   410,     0,   411,   412,     0,     0,   109,
   110,     0,  1012,     0,     0,   413,     0,     0,     0,   414,
   111,   112,     0,   113,     0,     0,     0,     0,     0,     0,
   114,   115,     0,   116,   117,     0,   118,     0,     0,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,     0,
     0,     0,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,  1149,     0,   415,     0,     0,     0,     0,     0,
   416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   417,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   418,   419,   420,   421,   217,   218,
     0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
     0,     0,     0,     0,     0,   109,   110,   423,   424,   425,
   426,   427,   428,   429,   430,   431,   111,   432,     0,   113,
     0,   403,   433,   434,     0,   435,   436,   437,   404,   116,
   117,   438,   118,     0,     0,   119,   120,   121,   122,   123,
   405,     0,     0,   124,   125,     0,   406,   407,     0,   126,
   439,   128,     0,     0,   486,     0,   440,   441,     0,     0,
     0,     0,   510,     0,   443,   665,     0,     0,   294,     0,
     0,     0,     0,     0,   408,     0,     0,   409,     0,     0,
   108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     0,     0,   410,     0,   411,   412,     0,     0,     0,   111,
   112,     0,   113,     0,   413,     0,     0,     0,   414,   114,
   115,     0,   116,   117,     0,   118,   894,     0,   119,   120,
   121,   122,   123,     0,     0,     0,   124,   125,     0,     0,
     0,     0,   126,   127,   128,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    14,     0,     0,     0,   667,     0,     0,
     0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   418,   419,   420,   421,   217,   218,     0,
     0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
     0,     0,     0,     0,   109,   110,   423,   424,   425,   426,
   427,   428,   429,   430,   431,   111,   432,     0,   113,     0,
     0,   433,   434,     0,   435,   436,   437,     0,   116,   117,
   438,   118,     0,     0,   119,   120,   121,   122,   123,   895,
   896,   897,   124,   125,     0,     0,   403,     0,   126,   439,
   128,     0,     0,   404,     0,   440,   441,     0,     0,     0,
     0,   671,     0,   443,     0,   405,     0,   294,   664,     0,
     0,   406,   407,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   491,     0,
   665,     0,     0,     0,     0,     0,     0,     0,     0,   408,
     0,     0,   409,     0,     0,   108,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   109,   110,     0,     0,   410,     0,   411,
   412,     0,     0,     0,   111,   112,     0,   113,     0,   413,
     0,     0,     0,   414,   114,   115,     0,   116,   117,     0,
   118,   666,     0,   119,   120,   121,   122,   123,     0,     0,
     0,   124,   125,     0,     0,     0,     0,   126,   127,   128,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   667,     0,     0,     0,     0,     0,   415,     0,
     0,     0,     0,     0,   416,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   418,   419,
   420,   421,   217,   218,     0,     0,     0,     0,     0,     0,
     0,     0,   422,     0,     0,     0,     0,     0,     0,   109,
   110,   423,   424,   425,   426,   427,   428,   429,   430,   431,
   111,   432,     0,   113,     0,   403,   433,   434,     0,   435,
   436,   437,   404,   116,   117,   438,   118,     0,     0,   119,
   120,   668,   669,   670,   405,     0,     0,   124,   125,     0,
   406,   407,     0,   126,   439,   128,     0,     0,     0,     0,
   440,   441,     0,     0,     0,     0,   671,     0,   443,   665,
     0,     0,   294,     0,     0,     0,     0,     0,   408,     0,
     0,   409,     0,     0,   108,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   109,   110,     0,     0,   410,     0,   411,   412,
     0,     0,     0,   111,   112,     0,   113,     0,   413,     0,
     0,     0,   414,   114,   115,     0,   116,   117,     0,   118,
   890,     0,   119,   120,   121,   122,   123,     0,     0,     0,
   124,   125,     0,     0,     0,     0,   126,   127,   128,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,  1854,
     0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   667,     0,     0,     0,     0,     0,   415,     0,     0,
     0,     0,     0,   416,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   417,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   418,   419,   420,
   421,   217,   218,     0,     0,     0,     0,     0,     0,     0,
     0,   422,     0,     0,     0,     0,     0,     0,   109,   110,
   423,   424,   425,   426,   427,   428,   429,   430,   431,   111,
   432,     0,   113,     0,   403,   433,   434,     0,   435,   436,
   437,   404,   116,   117,   438,   118,     0,     0,   119,   120,
   668,   669,   670,   405,     0,     0,   124,   125,     0,   406,
   407,     0,   126,   439,   128,     0,     0,     0,     0,   440,
   441,     0,     0,     0,     0,   671,     0,   443,   665,     0,
     0,   294,     0,     0,     0,     0,     0,   408,     0,     0,
   409,     0,     0,   108,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   109,   110,     0,     0,   410,     0,   411,   412,     0,
     0,     0,   111,   112,     0,   113,     0,   413,     0,     0,
     0,   414,   114,   115,     0,   116,   117,     0,   118,   666,
     0,   119,   120,   121,   122,   123,     0,     0,     0,   124,
   125,     0,     0,     0,     0,   126,   127,   128,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   667,     0,     0,     0,     0,     0,   415,     0,     0,     0,
     0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   417,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   418,   419,   420,   421,
   217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
   422,     0,     0,     0,     0,     0,     0,   109,   110,   423,
   424,   425,   426,   427,   428,   429,   430,   431,   111,   432,
     0,   113,     0,   403,   433,   434,     0,   435,   436,   437,
   404,   116,   117,   438,   118,     0,     0,   119,   120,   668,
   669,   670,   405,     0,     0,   124,   125,     0,   406,   407,
     0,   126,   439,   128,     0,     0,     0,     0,   440,   441,
     0,     0,     0,     0,   671,     0,   443,   665,     0,     0,
   294,     0,     0,     0,     0,     0,   408,     0,     0,   409,
     0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   109,   110,     0,     0,   410,     0,   411,   412,     0,     0,
     0,   111,   112,     0,   113,     0,   413,     0,     0,     0,
   414,   114,   115,     0,   116,   117,     0,   118,   890,     0,
   119,   120,   121,   122,   123,     0,     0,     0,   124,   125,
     0,     0,     0,     0,   126,   127,   128,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,  1012,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   667,
     0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
     0,   416,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   417,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   418,   419,   420,   421,   217,
   218,     0,     0,     0,     0,     0,     0,     0,     0,   422,
     0,     0,     0,     0,     0,     0,   109,   110,   423,   424,
   425,   426,   427,   428,   429,   430,   431,   111,   432,     0,
   113,     0,   403,   433,   434,     0,   435,   436,   437,   404,
   116,   117,   438,   118,     0,     0,   119,   120,   121,   122,
   123,   405,     0,     0,   124,   125,     0,   406,   407,     0,
   126,   439,   128,     0,     0,     0,     0,   440,   441,     0,
     0,     0,     0,   671,     0,   443,     0,     0,     0,   294,
     0,     0,     0,     0,     0,   408,     0,     0,   409,     0,
     0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
   110,     0,     0,   410,     0,   411,   412,     0,     0,     0,
   111,   112,     0,   113,     0,   413,     0,     0,     0,   414,
   114,   115,     0,   116,   117,     0,   118,     0,     0,   119,
   120,   121,   122,   123,     0,     0,     0,   124,   125,     0,
     0,     0,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,     0,     0,  1614,     0,     0,     0,     0,     0,
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
   439,   128,     0,     0,     0,     0,   440,   441,   515,     0,
     0,     0,   510,     0,   443,     0,     0,     0,   294,     0,
     0,     0,     0,     0,   408,     0,     0,   409,     0,     0,
   108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     0,     0,   410,     0,   411,   412,     0,     0,     0,   111,
   112,     0,   113,     0,   413,     0,     0,     0,   414,   114,
   115,     0,   116,   117,     0,   118,     0,     0,   119,   120,
   121,   122,   123,     0,     0,     0,   124,   125,     0,     0,
     0,     0,   126,   127,   128,     0,     0,     0,     0,     0,
     0,     0,     0,  1623,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     0,     0,     0,     0,     0,     0,     0,   703,     0,     0,
     0,     0,     0,   418,   419,   420,   421,   217,   218,     0,
     0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
     0,     0,     0,     0,   109,   110,   423,   424,   425,   426,
   427,   428,   429,   430,   431,   111,   432,     0,   113,     0,
   403,   433,   434,     0,   435,   436,   437,   404,   116,   117,
   438,   118,     0,     0,   119,   120,   121,   122,   123,   405,
     0,     0,   124,   125,     0,   406,   407,     0,   126,   439,
   128,     0,     0,     0,     0,   440,   441,     0,     0,     0,
     0,   510,     0,   443,     0,     0,     0,   294,     0,     0,
     0,     0,     0,   408,     0,     0,   409,    78,     0,   108,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   109,   110,     0,
     0,   410,     0,   411,   412,     0,     0,     0,   111,   112,
     0,   113,     0,   413,     0,     0,     0,   414,   114,   115,
     0,   116,   117,     0,   118,     0,     0,   119,   120,   121,
   122,   123,     0,     0,   108,   124,   125,     0,     0,     0,
     0,   126,   127,   128,     0,     0,     0,     0,     0,    79,
     0,     0,   109,   110,     0,     0,     0,     0,     0,     0,
     0,     0,    14,   111,   112,     0,   113,     0,     0,     0,
     0,     0,   415,   114,   115,     0,   116,   117,   416,   118,
     0,     0,   119,   120,   121,   122,   123,     0,   417,     0,
   124,   125,     0,     0,     0,     0,   126,   127,   128,     0,
     0,     0,   418,   419,   420,   421,   217,   218,     0,     0,
     0,     0,     0,     0,     0,     0,   422,     0,     0,     0,
     0,     0,     0,   109,   110,   423,   424,   425,   426,   427,
   428,   429,   430,   431,   111,   432,     0,   113,     0,   403,
   433,   434,     0,   435,   436,   437,   404,   116,   117,   438,
   118,     0,     0,   119,   120,   121,   122,   123,   405,     0,
     0,   124,   125,     0,   406,   407,     0,   126,   439,   128,
     0,     0,     0,     0,   440,   441,     0,     0,     0,     0,
   510,     0,   443,     0,     0,     0,   294,     0,     0,     0,
     0,     0,   408,     0,     0,   409,     0,     0,   108,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   109,   110,     0,     0,
   410,     0,   411,   412,     0,     0,     0,   111,   112,     0,
   113,     0,   413,     0,     0,     0,   414,   114,   115,     0,
   116,   117,     0,   118,     0,     0,   119,   120,   121,   122,
   123,     0,     0,     0,   124,   125,     0,     0,     0,     0,
   126,   127,   128,     0,     0,     0,     0,     0,     0,   974,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     0,     0,     0,   440,   441,     0,     0,     0,     0,   510,
   971,   443,     0,     0,     0,   294,     0,     0,     0,     0,
     0,   408,     0,     0,   409,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   410,
     0,   411,   412,     0,     0,     0,     0,     0,     0,     0,
     0,   413,     0,     0,     0,   414,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   415,     0,     0,     0,     0,     0,   416,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,  1176,     0,     0,
   418,   419,   420,   421,   217,   218,     0,     0,     0,     0,
     0,     0,     0,     0,   422,     0,     0,     0,     0,     0,
     0,   109,   110,   423,   424,   425,   426,   427,   428,   429,
   430,   431,   111,   432,     0,   113,     0,   403,   433,   434,
     0,   435,   436,   437,   404,   116,   117,   438,   118,     0,
     0,   119,   120,   121,   122,   123,   405,     0,     0,   124,
   125,     0,   406,   407,     0,   126,   439,   128,     0,     0,
     0,     0,   440,   441,     0,     0,     0,     0,   510,     0,
   443,     0,     0,     0,   294,     0,     0,     0,     0,     0,
   408,     0,     0,   409,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   410,     0,
   411,   412,     0,     0,     0,     0,     0,     0,     0,     0,
   413,     0,     0,     0,   414,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     0,     0,     0,     0,     0,   416,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   417,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,  1381,     0,     0,   418,
   419,   420,   421,   217,   218,     0,     0,     0,     0,     0,
     0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
   109,   110,   423,   424,   425,   426,   427,   428,   429,   430,
   431,   111,   432,     0,   113,     0,   403,   433,   434,     0,
   435,   436,   437,   404,   116,   117,   438,   118,     0,     0,
   119,   120,   121,   122,   123,   405,     0,     0,   124,   125,
     0,   406,   407,     0,   126,   439,   128,     0,     0,     0,
     0,   440,   441,     0,     0,     0,     0,   510,     0,   443,
     0,     0,     0,   294,     0,     0,     0,     0,     0,   408,
     0,     0,   409,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   410,     0,   411,
   412,     0,     0,     0,     0,     0,     0,     0,     0,   413,
     0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
   440,   441,     0,     0,  1731,     0,   510,     0,   443,     0,
     0,     0,   294,     0,     0,     0,     0,     0,   408,     0,
     0,   409,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   410,     0,   411,   412,
     0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
     0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   415,     0,     0,
     0,     0,     0,   416,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   417,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   418,   419,   420,
   421,   217,   218,     0,     0,     0,     0,     0,     0,     0,
     0,   422,     0,     0,     0,     0,     0,     0,   109,   110,
   423,   424,   425,   426,   427,   428,   429,   430,   431,   111,
   432,     0,   113,     0,   403,   433,   434,     0,   435,   436,
   437,   404,   116,   117,   438,   118,     0,     0,   119,   120,
   121,   122,   123,   405,     0,     0,   124,   125,     0,   406,
   407,     0,   126,   439,   128,     0,     0,     0,     0,   440,
   441,     0,     0,     0,     0,   442,     0,   443,     0,     0,
     0,   294,     0,     0,     0,     0,     0,   408,     0,     0,
   409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   410,     0,   411,   412,     0,
     0,     0,     0,     0,     0,     0,     0,   413,     0,     0,
     0,   414,     0,     0,     0,     0,     0,     0,     0,  1225,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     6,     0,     0,
     0,     0,     0,     0,  -289,     0,     0,     0,  1430,  1431,
     0,  1432,     0,     0,     0,     0,   415,     0,     0,     0,
     0,     0,   416,     0,     0,     0,     0,  1433,     0,     0,
     0,     0,   417,  1434,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   418,   419,   420,   421,
   217,   218,     0,     0,     0,     0,     0,     0,     0,     0,
   422,     0,     0,     0,     0,     0,     0,   109,   110,   423,
   424,   425,   426,   427,   428,   429,   430,   431,   111,   432,
     0,   113,     0,     0,   433,   434,     0,   435,   436,   437,
  1435,   116,   117,   438,   118,     0,     0,   119,   120,   121,
   122,   123,     0,  1225,     0,   124,   125,     0,    14,     0,
     0,   126,   439,   128,     0,     0,     0,     0,   440,   441,
     0,     0,     0,  1436,   510,     0,   443,     0,     0,     0,
   294,     6,     0,    16,     0,     0,     0,     0,     0,     0,
     0,     0,  1430,  1431,     0,  1432,     0,     0,     0,     0,
     0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     0,  1433,     0,     0,     0,     0,     0,  1434,     0,   109,
   110,     0,     0,     0,     0,   607,     0,     0,   608,     0,
  1437,   112,     0,   113,   808,     0,     0,     0,     0,     0,
   114,   115,     0,   116,   117,   609,   118,     0,   610,   119,
  1438,   121,   122,   123,     0,     0,     0,   124,   125,  1439,
  1440,  1441,     0,   126,   127,   128,     0,     0,     0,     0,
     0,     0,     0,     0,  1435,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   611,     0,
     0,     0,    14,     0,   612,   613,   614,     0,   615,   616,
   617,     0,   618,     0,     0,     0,     0,  1436,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
   619,     0,   620,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   108,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   621,     0,   109,   110,     0,     0,     0,     0,     0,
     0,     0,     0,     0,  1437,   112,     0,   113,   622,     0,
     0,     0,     0,     0,   114,   115,     0,   116,   117,     0,
   118,     0,     0,   119,  1438,   121,   122,   123,     0,     0,
   623,   124,   125,  1439,  1440,  1441,     0,   126,   127,   128,
     0,     0,     0,     0,     0,   108,     0,   607,     0,     0,
   608,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   109,   110,   624,   625,   609,     0,     0,
   610,     0,     0,     0,   111,   112,     0,   113,     0,     0,
     0,     0,   626,     0,   114,   115,     0,   116,   117,     0,
   118,     0,     0,   119,   120,   121,   122,   123,     0,     0,
     0,   124,   125,     0,     0,     0,     0,   126,   127,   128,
   611,     0,     0,     0,     0,     0,   612,   613,   614,     0,
   615,   616,   617,     0,   618,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
     0,     0,   619,     0,   620,   173,   174,   175,   176,   177,
   178,   179,   180,   181,   109,   110,   182,   183,   184,   185,
   186,     0,     0,   187,   188,   111,   112,     0,   113,     0,
     0,   189,   190,   621,     0,   114,   115,   191,   116,   117,
   192,   118,   193,     0,   119,   120,   121,   122,   123,     0,
   622,     0,   124,   125,   194,   195,   196,   197,   126,   127,
   128,   198,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   623,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   109,   110,   624,   625,     0,
     0,     0,     0,     0,     0,     0,   111,   112,     0,   113,
     0,     0,     0,     0,   626,     0,   114,   115,     0,   116,
   117,     0,   118,     0,     0,   119,   120,   121,   122,   123,
     0,     0,     0,   124,   125,     0,     0,     0,     0,   126,
   127,   128
};

static const short yycheck[] = {     9,
     0,     4,    12,    66,   157,   107,    16,    57,    18,   545,
    65,   350,   727,   381,    16,   381,    70,    58,   147,   148,
    65,   377,   477,    96,    87,    64,   578,   352,    69,   521,
   147,   148,    75,  1065,    93,    16,   703,   550,   373,    93,
   781,    91,   709,   280,    72,    55,    56,    57,    58,    59,
    60,    92,   442,   290,    97,    65,    66,    67,    99,    69,
  1055,    71,    72,    90,    74,    75,  1224,    95,   981,   784,
  1065,   924,   499,    83,   689,    85,    86,    87,    59,    89,
    90,    91,    92,    85,    94,    95,    96,    97,   161,    99,
    71,  1055,    94,   138,   498,   140,   442,  1012,   713,   734,
   659,  1065,   296,  1062,    85,  1298,   959,  1118,   359,   752,
   652,  1095,   877,   358,   233,   570,  1242,   572,   159,   323,
  1296,  1206,   752,   578,  1264,   152,     8,  1212,   138,   139,
   140,     8,   817,     8,    10,   386,  1055,   535,     8,   165,
   242,   666,   152,  1055,     8,     6,  1065,   157,     8,   159,
   160,   161,     8,  1065,  1113,   165,  1055,    61,   139,    90,
  1445,  1554,   504,   202,   510,  1055,  1065,  1059,    21,    10,
   323,    26,    19,    23,  1066,  1065,    12,     8,    28,   160,
    61,     8,    23,  1075,    47,    46,    43,    28,   227,    63,
    97,    43,    23,  1234,     8,  1236,    23,    28,   133,   141,
   556,    28,     8,    22,   196,    47,   185,    80,   327,  1101,
   578,    43,   578,     3,    14,   117,     8,   117,   192,    26,
     9,   126,    63,   297,  1808,   270,   286,     4,   273,   331,
     7,   305,   143,   143,   193,   902,   903,   666,    90,    63,
    63,   580,   671,   125,     8,   251,   142,   603,   125,    18,
   125,  1495,   162,   259,   491,   125,   156,   307,  1290,  1291,
   270,   125,   158,   273,   244,   125,    55,   277,   123,   125,
   280,   281,    49,   178,    12,   787,   286,    66,   323,  1863,
   290,  1439,  1440,   194,   186,   952,    76,   279,   627,   540,
   802,    10,  1324,   805,   273,  1290,  1291,   307,   272,   118,
   174,   311,   814,   203,    23,   105,   351,   180,   144,    28,
   191,   125,  1271,   358,    44,   360,    23,   362,   260,   125,
   365,    28,   177,   230,   255,   671,  1290,  1291,   718,  1324,
   311,  1416,  1417,   125,   149,   374,   726,   376,   273,   194,
  1255,   351,   383,  1519,   394,   498,  1330,  1487,   358,    23,
   360,   390,   362,   996,    28,   365,   384,   204,   870,   173,
  1324,   125,   854,   761,  1007,  1008,   996,   173,   272,   894,
   177,  1290,  1291,   383,   384,   830,   165,  1007,  1290,  1291,
  1293,    11,   392,  1776,   394,  1070,  1071,  1054,   398,    63,
   272,  1290,  1291,    23,    44,   398,   272,   274,    28,   274,
  1290,  1291,  1646,   255,   274,  1324,   144,   264,   269,   272,
   274,   392,  1324,   755,   274,    45,    44,  1270,   274,   269,
   270,  1313,  1037,   276,  1337,  1324,   267,   268,   269,   270,
   272,   272,   264,   443,  1324,  1120,   267,   268,   269,   270,
   267,   268,   269,   270,  1485,   505,    30,    21,   272,   490,
   180,   878,    80,   498,    23,   196,    64,   796,   273,    28,
   799,   890,   889,   504,    19,   894,   893,   174,    17,    77,
  1628,  1138,  1139,   812,  1141,  1142,   486,  1670,   595,    84,
   490,   491,    31,   509,   174,    43,    98,   497,   271,    50,
    51,   501,   100,    44,   504,   505,    50,    51,   837,   509,
   545,     5,   141,   147,   272,   486,   114,   744,   192,   277,
     7,    15,   117,    87,   122,  1800,   497,   145,  1644,   558,
   501,    30,  1607,    56,   643,    61,    75,   155,  1040,    80,
   180,   272,     3,   578,    43,   545,    94,   596,   143,    94,
  1551,   146,   596,   151,  1829,  1057,  1058,   196,   267,   268,
   269,   270,   180,   196,   103,   167,   881,   162,   803,   598,
   267,   268,   269,   270,   758,  1077,   576,  1126,   578,    37,
  1112,  1336,   582,  1532,    68,  1768,   780,  1862,    18,  1563,
    48,  1496,   272,   188,  1120,  1012,  1221,    55,   272,  1874,
  1503,  1550,  1877,   267,   268,   269,   270,  1001,    23,   196,
  1885,    90,   731,    28,   190,    76,   651,   246,  1251,   648,
   649,    79,  1279,  1003,   198,  1042,    23,   868,   167,    62,
  1512,    28,   155,  1816,  1516,  1517,   141,   780,   189,   180,
   273,   239,   216,   217,  1301,   189,   279,   267,   268,   269,
   270,   651,    82,   227,   228,    23,   230,     4,  1841,   766,
    28,  1243,    85,   237,   238,   263,   240,   241,  1508,   243,
  1552,   176,   246,   247,   248,   249,   250,   694,   196,    65,
   254,   255,    46,   141,   168,   108,   260,   261,   262,   169,
   230,   175,    65,     6,   694,    23,   272,   732,   169,   734,
    28,   736,    49,   273,    64,  1727,   139,    23,   267,   268,
   269,   270,    28,  1553,   172,   274,   196,    77,   176,   719,
   199,   200,   262,  1048,   755,   196,  1569,   727,  1876,   196,
    94,    44,   732,   196,   734,  1064,   736,  1319,   231,   739,
   100,   164,  1727,    56,   744,   780,   179,  1076,   241,    11,
   268,   786,   752,     4,    23,   755,   223,    70,   985,    28,
   760,   988,  1367,   792,   764,    16,   140,    80,   803,   273,
  1501,  1100,  1152,  1727,  1656,   775,    46,   777,  1660,  1682,
   196,     6,   831,    45,   784,  1688,   786,   831,   268,  1114,
   122,   151,   836,  1675,   829,   830,  1299,   268,    49,   279,
    51,   268,   273,   803,   775,   268,   777,   851,  1123,   156,
  1692,   231,   232,   842,   863,   864,  1152,   866,  1727,   863,
   864,    46,   866,    22,    94,  1727,    64,   174,    44,   829,
   830,    30,   145,  1335,     6,   141,  1216,   272,  1727,    77,
  1322,   122,   155,  1500,   844,    79,  1328,  1727,   118,   124,
     6,   844,   267,   268,   269,   270,   122,   273,  1001,   274,
  1402,   208,   100,   279,    80,   122,   172,   180,   118,   869,
   267,   268,   269,   270,    46,   272,   114,  1192,     6,   879,
   114,   187,     6,  1263,   155,   198,    23,   887,   888,   139,
    46,    28,   126,  1775,   887,   145,  1778,   174,   869,   267,
   268,   269,   270,   216,   217,    23,   274,   232,   879,    98,
    28,   182,   196,   151,   227,   228,    23,   230,    46,   118,
     6,    28,    46,   156,   237,   238,   989,   240,   241,   179,
   243,   199,   200,   246,   247,   248,   249,   250,  1820,   223,
   139,   254,   255,   132,    79,   134,   145,   260,   261,   262,
   180,   267,   268,   269,   270,   986,   192,   990,   274,    23,
    46,   272,  1619,   274,    28,   272,  1001,   156,     6,   987,
   277,   275,    19,   272,   278,   195,   196,   197,   277,  1424,
   179,   272,  1311,   274,   273,   985,   986,   987,   988,   989,
   990,  1098,   181,   272,  1340,   274,   996,    23,   267,   268,
   269,   270,    28,   223,   224,   274,    44,  1007,  1008,    17,
    59,    60,  1012,  1342,   272,    23,   274,   203,    56,   205,
    28,   130,   131,    31,   272,   263,   274,   259,  1364,  1365,
    23,   272,   272,   274,   274,    28,  1372,  1373,   199,  1375,
  1376,   272,    80,   274,  1402,    10,  1402,  1383,  1384,  1385,
  1386,   162,  1388,  1389,  1390,  1391,  1392,  1393,    59,    67,
  1109,    23,   272,    79,   274,  1109,    28,    75,   272,    23,
   274,   271,   272,  1122,    28,    83,   271,   272,  1122,  1128,
    61,  1116,  1225,   198,  1128,  1120,  1234,  1087,  1236,    78,
   690,  1748,   692,   101,   149,   103,   696,   697,   698,  1092,
  1757,    23,  1807,  1823,  1824,  1105,    28,   145,   116,   141,
   118,   119,   120,   190,    82,    17,  1116,   155,   273,    63,
  1120,    23,  1241,  1242,  1243,   272,    28,   274,    61,    31,
   267,   268,   269,   270,   272,    23,   274,   274,   141,   272,
    28,   274,   180,   272,   337,   274,   339,   122,    23,   267,
   268,   269,   270,    28,   244,   272,   274,   274,   122,   167,
   267,   268,   269,   270,   272,    67,   274,   274,  1275,    23,
  1277,   102,   174,    75,    28,   188,   272,    23,   274,  1224,
   172,    83,    28,   272,   123,   274,  1221,   121,  1188,  1224,
  1225,   272,  1521,   274,   155,   243,  1188,   254,  1258,   101,
  1260,   103,    23,   267,   268,   269,   270,    28,    30,   255,
   274,   254,   207,   272,   116,    23,   118,   119,   120,   264,
    28,  1221,    23,   112,  1224,  1225,    29,    28,  1557,   273,
    23,  1338,   135,    10,  1234,    28,  1236,   273,   273,   273,
   199,   267,   268,   269,   270,    23,   273,   199,   274,   273,
    28,  1251,   273,   273,   273,  1255,   264,   265,   266,   267,
   268,   269,   270,   273,   273,   167,   273,   199,   276,   273,
   278,   269,   278,   273,   267,   268,   269,   270,   264,   273,
   276,   274,    23,    61,   272,  1811,    17,    28,   273,    82,
   273,   278,    23,    10,   174,   293,    23,    28,    63,   141,
    31,    28,  1445,   272,   302,   267,   268,   269,   270,    63,
   166,   278,   274,   267,   268,   269,   270,   272,    23,   272,
   274,   264,  1351,    28,  1317,   273,    23,  1356,    10,    90,
   272,    28,    90,    90,    22,    22,    67,    43,   336,    10,
   174,    82,   340,   208,    75,   267,   268,   269,   270,    54,
   272,   136,    83,   136,   174,  1498,   196,    54,    21,   185,
  1689,    90,   264,   265,   266,   267,   268,   269,   270,   185,
   101,   138,   103,   273,   276,    91,   278,  1430,   273,   267,
   268,   269,   270,   276,    61,   116,   274,   118,   119,   120,
  1637,   273,   267,   268,   269,   270,   273,    10,  1398,   274,
   909,   122,   911,   149,   913,   914,  1398,   916,   917,   918,
  1445,   272,   121,   267,   268,   269,   270,   128,   273,   126,
   274,   267,   268,   269,   270,   273,   189,   274,   274,    21,
  1430,    42,   174,   202,   279,    60,   167,   435,  1438,  1439,
  1440,   273,   440,   441,   442,  1445,   267,   268,   269,   270,
   190,    63,   186,   274,    63,    10,   240,   272,    19,   267,
   268,   269,   270,   273,   272,    43,   267,   268,   269,   270,
   271,   273,  1637,    27,   267,   268,   269,   270,   271,    86,
    86,    22,   125,   125,   125,  1485,   156,   154,   156,   267,
   268,   269,   270,    43,  1637,  1495,  1496,   273,  1498,    98,
   273,   273,  1645,    17,   108,   273,    63,    23,   274,    23,
   272,   184,   510,    16,    28,    63,   514,    31,   107,   272,
   272,   124,  1522,   272,    71,  1644,   267,   268,   269,   270,
   273,    12,    10,   264,   265,   266,   267,   268,   269,   270,
   267,   268,   269,   270,  1791,   276,   239,   278,  1795,   273,
   273,  1522,   160,    67,   274,  1595,  1803,   275,   192,  1806,
   274,    75,   267,   268,   269,   270,   274,    10,   133,    83,
   267,   268,   269,   270,   274,    52,    52,     8,   274,  1826,
   274,  1673,   273,   273,    63,  1616,    38,   101,   273,   103,
   273,    51,  1592,   273,   273,  1595,   173,    64,   273,   273,
  1592,   273,   116,   272,   118,   119,   120,  1854,   273,   273,
    77,   273,  1859,   273,    66,   273,  1616,   273,    85,   273,
    70,   274,   274,   127,    85,    19,  1791,  1791,  1628,    85,
  1795,  1795,    73,   100,  1674,   130,   145,   125,  1803,  1803,
   125,  1806,    94,   125,  1634,  1645,  1646,   114,  1791,    22,
   125,   125,  1795,   167,   125,   274,   196,  1800,   274,   273,
  1803,  1826,  1826,  1806,  1664,    63,  1695,   272,    51,  1669,
  1699,   173,   274,   671,  1674,    36,   274,  1769,   274,   274,
   274,   271,    36,  1826,   151,   271,  1829,  1740,   273,  1854,
  1854,    82,   271,   691,  1859,  1859,   271,   695,  1669,    51,
   271,    82,   271,   271,   702,   271,   124,   271,   275,   707,
   708,  1854,   174,   165,    10,    85,  1859,    23,    80,  1862,
   718,  1721,   720,   721,   722,   723,   724,   725,   726,   181,
    79,  1874,   180,   180,  1877,    44,    80,    44,   274,   274,
  1740,  1741,  1885,   195,   196,   197,    94,   199,   200,  1742,
   264,   265,   266,   267,   268,   269,   270,    10,   124,   274,
   274,   111,   274,   274,   762,  1800,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   111,  1811,   274,   274,    61,
   274,   233,   234,   274,   274,   783,   189,   274,   274,   273,
   242,   789,   272,   791,  1829,    94,   263,    86,  1788,   204,
  1800,   274,   274,    79,   274,   274,   274,  1807,  1808,   274,
   274,  1811,   271,    82,   246,   273,   268,   274,   273,    82,
   271,   122,   148,  1823,  1824,   278,   272,  1862,   273,  1829,
   274,   273,  1364,  1365,   273,   173,   122,   122,   274,  1874,
  1372,  1373,  1877,  1375,  1376,   274,   271,    47,   272,    44,
  1885,  1383,  1384,  1385,  1386,   180,  1388,  1389,  1390,  1391,
  1392,  1393,  1862,  1863,   274,    50,     6,    36,    47,     9,
   201,     0,     0,    13,  1874,    12,  1876,  1877,   278,    37,
    20,  1239,   281,   290,   744,  1885,  1123,  1126,   887,  1115,
    48,   655,    32,  1541,   348,   378,   329,    55,    38,    39,
   882,   603,  1051,   786,  1116,   813,   904,   905,   906,  1120,
    68,   909,   910,   911,  1540,   913,   914,   915,   916,   917,
   918,  1611,   360,  1519,  1521,   360,    66,   925,   926,    69,
   928,  1814,   930,  1818,  1843,   933,   934,   935,   936,   937,
   938,  1817,   982,  1632,   803,   198,  1733,  1504,   584,  1801,
   732,  1273,   950,   365,    94,   953,    96,    97,   353,   346,
   861,  1326,   607,   216,   217,    13,   106,   125,   853,   315,
   110,   523,   150,   762,   227,   228,   527,   230,   775,   640,
    85,  1012,  1592,   141,   237,   238,  1259,   240,   241,   688,
   243,  1762,  1398,   246,   247,   248,   249,   250,   789,   157,
     0,   254,   255,     0,   535,  1003,     0,   260,   261,   262,
  1167,  1214,   751,  1004,   172,   844,  1281,  1105,   176,  1664,
   582,   179,    89,   163,  1438,   165,    -1,    -1,    -1,   187,
    -1,   171,    -1,    48,    -1,    -1,    -1,    -1,    -1,    27,
    55,   181,    -1,    -1,    59,    60,    -1,    -1,   206,    -1,
    -1,    -1,    -1,    68,    -1,   195,   196,   197,   198,   199,
   200,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,   209,
    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
   220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
   240,   241,   242,   243,    -1,     6,   246,   247,   248,   249,
   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
   260,   261,   262,    -1,    -1,    -1,   141,   267,   268,    -1,
    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,
     7,    -1,   157,    44,   210,   211,   212,   213,   214,   215,
   216,   217,    -1,    -1,  1152,    56,    -1,   172,    25,    -1,
    -1,   176,    -1,    -1,    -1,    -1,    33,  1165,  1166,    -1,
  1168,  1169,   187,    40,    -1,    42,    -1,    44,  1176,    80,
    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    56,
    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,  1196,    -1,
    -1,    -1,    -1,    70,    -1,    -1,  1204,  1205,  1206,    -1,
   198,    -1,    -1,    80,  1212,    -1,  1214,  1215,  1216,  1217,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,   227,
   228,    -1,   230,    -1,   145,    20,    -1,    -1,    -1,   237,
   238,    -1,   240,   241,   155,   243,    -1,    32,   246,   247,
   248,   249,   250,    38,    39,  1263,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,    -1,   143,    -1,    -1,   180,
    -1,    -1,    -1,   150,    -1,   273,   153,    -1,   155,   156,
    -1,    66,    -1,    -1,    69,    -1,  1294,   198,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   180,    17,   216,   217,    -1,    -1,    94,
    23,    96,    97,    -1,    -1,    28,   227,   228,    31,   230,
    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
   261,   262,   229,    -1,    67,  1363,    -1,    -1,    -1,    -1,
    -1,  1369,    75,    -1,    -1,    -1,  1374,   244,   245,    -1,
    83,    -1,    -1,  1381,    -1,    -1,    -1,    -1,   163,    -1,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,   101,    -1,
   103,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,   116,    -1,   118,   119,   120,  1416,  1417,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,  1435,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
    -1,   236,   237,   238,   167,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,    -1,    -1,    -1,    -1,   260,   261,   262,     6,    -1,
    -1,     9,   267,   268,    -1,    13,    -1,    -1,   273,    -1,
   275,  1499,    20,    -1,   279,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   264,   265,   266,   267,   268,   269,   270,    -1,    -1,
    -1,    -1,    -1,    -1,  1572,    -1,    94,    -1,    96,    97,
  1578,  1579,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,  1602,    -1,    -1,    -1,    -1,  1607,
    -1,    -1,  1610,    -1,    -1,    -1,    -1,  1615,    -1,  1617,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,
    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    44,    -1,
    -1,    -1,    -1,   181,    -1,    51,    -1,    -1,    -1,    55,
    56,    -1,    58,    -1,    -1,    -1,    -1,   195,   196,   197,
   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    74,    -1,
    -1,   209,    -1,    -1,    80,    -1,  1694,    -1,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
   248,   249,   250,    -1,    -1,  1733,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,     6,    -1,    -1,     9,   267,
   268,   137,    13,    85,    -1,   273,    -1,   275,    -1,    20,
    -1,   279,    -1,    -1,  1762,    -1,    -1,    -1,    -1,   155,
    -1,    32,    -1,    -1,    -1,    -1,   108,    38,    39,    -1,
    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   189,  1802,    66,    -1,    -1,    69,    -1,
    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,   164,    94,    -1,    96,    97,    -1,    -1,    -1,
    -1,   227,   228,    30,   230,   106,    -1,    -1,    -1,   110,
    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
   246,   247,   248,   249,   250,    -1,   198,    -1,   254,   255,
   256,   257,   258,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
    -1,    -1,   163,    -1,   165,   237,   238,    -1,   240,   241,
   171,   243,    39,    -1,   246,   247,   248,   249,   250,    -1,
   181,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
   262,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,   260,
   261,   262,    13,    -1,    -1,    -1,   267,   268,    -1,    20,
    -1,   198,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,
    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,   216,
   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
   237,   238,    -1,   240,   241,    66,   243,    -1,    69,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
    -1,   198,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,   216,
   217,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,   260,
   261,   262,    13,    -1,    -1,    -1,   267,   268,    -1,    20,
    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,
    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    18,    -1,    66,    21,    -1,    69,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,    -1,
    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    18,   110,
    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    38,    -1,
    -1,    41,    87,    88,    89,    -1,    91,    92,    93,    -1,
    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
   115,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
   171,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
   181,    91,    92,    93,    -1,    95,    -1,    -1,    -1,   144,
    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,   113,    -1,   115,   161,    71,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,   183,   230,
    -1,    -1,   233,   234,   144,   236,   237,   238,    -1,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,   161,    -1,   254,   255,     6,    -1,    -1,     9,   260,
   261,   262,    13,   218,   219,    -1,   267,   268,    -1,    20,
    -1,    -1,   273,   183,   275,    -1,    -1,   141,   279,    -1,
   235,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,   219,
    -1,    -1,   176,    -1,    -1,    66,    -1,   181,    69,    -1,
    -1,    -1,    -1,   187,    -1,   235,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   206,    94,    -1,    96,    97,    -1,    -1,    -1,
    -1,    -1,   216,   217,    -1,   106,    -1,    -1,    -1,   110,
    -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    -1,   233,   234,   155,   236,   237,   238,    -1,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,     6,    -1,    -1,     9,   260,
   261,   262,    13,    -1,    -1,    -1,   267,   268,    -1,    20,
    -1,    -1,   273,    -1,   275,    -1,    -1,   198,   279,    -1,
    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
    -1,    -1,    -1,    -1,   181,    66,   237,   238,    69,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,   198,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
   261,   262,    -1,    94,    -1,    96,    97,    -1,    -1,   216,
   217,    -1,   273,    -1,    -1,   106,    -1,    -1,    -1,   110,
   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
   221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
   241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
   261,   262,    -1,    -1,   172,    -1,   267,   268,    -1,    -1,
    -1,    -1,   273,    -1,   275,    57,    -1,    -1,   279,    -1,
    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,   247,
   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,    -1,
    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
   242,   243,    -1,    -1,   246,   247,   248,   249,   250,   251,
   252,   253,   254,   255,    -1,    -1,    13,    -1,   260,   261,
   262,    -1,    -1,    20,    -1,   267,   268,    -1,    -1,    -1,
    -1,   273,    -1,   275,    -1,    32,    -1,   279,    35,    -1,
    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
   243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
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
   267,   268,    -1,    -1,    -1,    -1,   273,    -1,   275,    57,
    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,
    -1,    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,
    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,
    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   273,
    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,
   238,    20,   240,   241,   242,   243,    -1,    -1,   246,   247,
   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,
    39,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,
   268,    -1,    -1,    -1,    -1,   273,    -1,   275,    57,    -1,
    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
    69,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
    -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,   118,
    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
   219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
    20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,
    -1,    -1,    -1,    -1,   273,    -1,   275,    57,    -1,    -1,
   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
   110,   237,   238,    -1,   240,   241,    -1,   243,   118,    -1,
   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
    -1,    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   273,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,    -1,
    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,
    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
    -1,    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,
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
   261,   262,    -1,    -1,    -1,    -1,   267,   268,   269,    -1,
    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,
    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
    -1,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
   222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
    13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
   242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
   262,    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,
    -1,   273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,
    -1,    -1,    -1,    66,    -1,    -1,    69,   196,    -1,   198,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
   249,   250,    -1,    -1,   198,   254,   255,    -1,    -1,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,    -1,   268,
    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   155,   227,   228,    -1,   230,    -1,    -1,    -1,
    -1,    -1,   165,   237,   238,    -1,   240,   241,   171,   243,
    -1,    -1,   246,   247,   248,   249,   250,    -1,   181,    -1,
   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
    -1,    -1,   195,   196,   197,   198,   199,   200,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,
    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
   223,   224,   225,   226,   227,   228,    -1,   230,    -1,    13,
   233,   234,    -1,   236,   237,   238,    20,   240,   241,   242,
   243,    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,
    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,   262,
    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,
   273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
   260,   261,   262,    -1,    -1,    -1,    -1,    -1,    -1,   269,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
   274,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,
   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,
    -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
    -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,    -1,
    -1,    -1,   267,   268,    -1,    -1,    -1,    -1,   273,    -1,
   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,
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
    -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,   195,
   196,   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,
   216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
   226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
   236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
   246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
    -1,    38,    39,    -1,   260,   261,   262,    -1,    -1,    -1,
    -1,   267,   268,    -1,    -1,    -1,    -1,   273,    -1,   275,
    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,
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
   197,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
   217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,
   237,   238,    20,   240,   241,   242,   243,    -1,    -1,   246,
   247,   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,
    38,    39,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
   267,   268,    -1,    -1,   271,    -1,   273,    -1,   275,    -1,
    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,
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
   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,
   238,    20,   240,   241,   242,   243,    -1,    -1,   246,   247,
   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,
    39,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,
   268,    -1,    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,
    -1,   279,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
    -1,    58,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
    -1,    -1,   171,    -1,    -1,    -1,    -1,    74,    -1,    -1,
    -1,    -1,   181,    80,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
   219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
   137,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
   249,   250,    -1,    16,    -1,   254,   255,    -1,   155,    -1,
    -1,   260,   261,   262,    -1,    -1,    -1,    -1,   267,   268,
    -1,    -1,    -1,   170,   273,    -1,   275,    -1,    -1,    -1,
   279,    44,    -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,    -1,
    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    74,    -1,    -1,    -1,    -1,    -1,    80,    -1,   216,
   217,    -1,    -1,    -1,    -1,    18,    -1,    -1,    21,    -1,
   227,   228,    -1,   230,    27,    -1,    -1,    -1,    -1,    -1,
   237,   238,    -1,   240,   241,    38,   243,    -1,    41,   246,
   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
   257,   258,    -1,   260,   261,   262,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
    -1,    -1,   155,    -1,    87,    88,    89,    -1,    91,    92,
    93,    -1,    95,    -1,    -1,    -1,    -1,   170,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   144,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   227,   228,    -1,   230,   161,    -1,
    -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
   183,   254,   255,   256,   257,   258,    -1,   260,   261,   262,
    -1,    -1,    -1,    -1,    -1,   198,    -1,    18,    -1,    -1,
    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   216,   217,   218,   219,    38,    -1,    -1,
    41,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,
    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,    -1,
   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
    81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
    91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,    -1,
    -1,    -1,   113,    -1,   115,   207,   208,   209,   210,   211,
   212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
   222,    -1,    -1,   225,   226,   227,   228,    -1,   230,    -1,
    -1,   233,   234,   144,    -1,   237,   238,   239,   240,   241,
   242,   243,   244,    -1,   246,   247,   248,   249,   250,    -1,
   161,    -1,   254,   255,   256,   257,   258,   259,   260,   261,
   262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
    -1,    -1,    -1,    -1,   235,    -1,   237,   238,    -1,   240,
   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
   261,   262
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
case 311:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); ;
    break;}
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); ;
    break;}
case 314:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); ;
    break;}
case 315:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); ;
    break;}
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); ;
    break;}
case 317:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); ;
    break;}
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); ;
    break;}
case 319:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); ;
    break;}
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); ;
    break;}
case 321:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); ;
    break;}
case 322:
{ yyval = NULL; ;
    break;}
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); ;
    break;}
case 324:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); ;
    break;}
case 325:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 326:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 327:
{ yyval = NULL; ;
    break;}
case 328:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 329:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 330:
{ yyval = NULL; ;
    break;}
case 333:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 335:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); ;
    break;}
case 336:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); ;
    break;}
case 337:
{ yyval = NULL; ;
    break;}
case 338:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); ;
    break;}
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); ;
    break;}
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); ;
    break;}
case 341:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); ;
    break;}
case 342:
{ yyval = NULL; ;
    break;}
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 345:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); ;
    break;}
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 348:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); ;
    break;}
case 349:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); ;
    break;}
case 350:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); ;
    break;}
case 351:
{ yyval = make_node (nod_default, 1, NULL); ;
    break;}
case 355:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); ;
    break;}
case 356:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); ;
    break;}
case 357:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); ;
    break;}
case 358:
{ yyval = NULL; ;
    break;}
case 359:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); ;
    break;}
case 360:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); ;
    break;}
case 361:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 362:
{ yyval = NULL; ;
    break;}
case 364:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 365:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); ;
    break;}
case 366:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 367:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 368:
{ lex.beginning = lex_position(); ;
    break;}
case 369:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);;
    break;}
case 370:
{ lex.beginning = lex.last_token; ;
    break;}
case 371:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); ;
    break;}
case 372:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); ;
    break;}
case 373:
{ yyval = 0; ;
    break;}
case 374:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 375:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 376:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 377:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 378:
{ yyval = NULL; ;
    break;}
case 379:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); ;
    break;}
case 380:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); ;
    break;}
case 381:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); ;
    break;}
case 382:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); ;
    break;}
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); ;
    break;}
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); ;
    break;}
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); ;
    break;}
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); ;
    break;}
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); ;
    break;}
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); ;
    break;}
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); ;
    break;}
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); ;
    break;}
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); ;
    break;}
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); ;
    break;}
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); ;
    break;}
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); ;
    break;}
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); ;
    break;}
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); ;
    break;}
case 397:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 398:
{ yyval = NULL; ;
    break;}
case 399:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); ;
    break;}
case 400:
{ yyval = yyvsp[0]; ;
    break;}
case 401:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); ;
    break;}
case 402:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); ;
    break;}
case 403:
{ yyval = yyvsp[0]; ;
    break;}
case 404:
{ yyval = yyvsp[0]; ;
    break;}
case 405:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); ;
    break;}
case 406:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); ;
    break;}
case 407:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); ;
    break;}
case 408:
{ yyval = yyvsp[0]; ;
    break;}
case 409:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); ;
    break;}
case 411:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 412:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); ;
    break;}
case 413:
{ yyval = yyvsp[0]; ;
    break;}
case 414:
{ yyval = yyvsp[0]; ;
    break;}
case 415:
{yyval = make_node (nod_del_default, (int) 0, NULL); ;
    break;}
case 416:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); ;
    break;}
case 417:
{ yyval = yyvsp[0]; ;
    break;}
case 418:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); ;
    break;}
case 420:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 421:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 422:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);;
    break;}
case 423:
{ yyval = yyvsp[0]; ;
    break;}
case 424:
{ yyval = yyvsp[0]; ;
    break;}
case 425:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); ;
    break;}
case 426:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 427:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); ;
    break;}
case 428:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); ;
    break;}
case 456:
{ yyval = NULL; ;
    break;}
case 457:
{ yyval = NULL; ;
    break;}
case 458:
{ yyval = NULL; ;
    break;}
case 459:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); ;
    break;}
case 460:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; ;
    break;}
case 461:
{ yyval = make_node (nod_restrict, 0, NULL); ;
    break;}
case 462:
{ yyval = make_node (nod_cascade, 0, NULL); ;
    break;}
case 463:
{ yyval = make_node (nod_restrict, 0, NULL); ;
    break;}
case 464:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); ;
    break;}
case 465:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); ;
    break;}
case 466:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; ;
    break;}
case 468:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 469:
{ yyval = yyvsp[0]; ;
    break;}
case 470:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); ;
    break;}
case 471:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); ;
    break;}
case 472:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); ;
    break;}
case 473:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); ;
    break;}
case 474:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 476:
{ yyval = NULL; ;
    break;}
case 478:
{ yyval = NULL; ;
    break;}
case 479:
{ yyval = yyvsp[0]; ;
    break;}
case 480:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); ;
    break;}
case 481:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); ;
    break;}
case 482:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); ;
    break;}
case 483:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); ;
    break;}
case 484:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); ;
    break;}
case 485:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); ;
    break;}
case 486:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); ;
    break;}
case 487:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); ;
    break;}
case 488:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); ;
    break;}
case 489:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); ;
    break;}
case 490:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); ;
    break;}
case 491:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); ;
    break;}
case 496:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; ;
    break;}
case 497:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; ;
    break;}
case 499:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 500:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); ;
    break;}
case 501:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); ;
    break;}
case 507:
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
case 508:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			;
    break;}
case 509:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			;
    break;}
case 510:
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
case 511:
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
case 512:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			;
    break;}
case 515:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			;
    break;}
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			;
    break;}
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			;
    break;}
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			;
    break;}
case 519:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	;
    break;}
case 520:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	;
    break;}
case 521:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			;
    break;}
case 522:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			;
    break;}
case 523:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			;
    break;}
case 524:
{
			lex.g_field->fld_character_set = yyvsp[0];
			;
    break;}
case 526:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 527:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 528:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			;
    break;}
case 529:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			;
    break;}
case 530:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			;
    break;}
case 531:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			;
    break;}
case 540:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			;
    break;}
case 541:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			;
    break;}
case 542:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				;
    break;}
case 543:
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
case 544:
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
case 547:
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
case 548:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			;
    break;}
case 549:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			;
    break;}
case 550:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			;
    break;}
case 551:
{ yyval = yyvsp[-1]; ;
    break;}
case 552:
{ yyval = 0; ;
    break;}
case 556:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			;
    break;}
case 557:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			;
    break;}
case 558:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			;
    break;}
case 562:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); ;
    break;}
case 563:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 564:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 565:
{ yyval = 0; ;
    break;}
case 566:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); ;
    break;}
case 569:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); ;
    break;}
case 570:
{ yyval = make_node (nod_rollback, 0, NULL); ;
    break;}
case 573:
{ yyval = make_node (nod_commit_retain, 0, NULL); ;
    break;}
case 574:
{ yyval = NULL; ;
    break;}
case 576:
{ yyval = NULL; ;
    break;}
case 577:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); ;
    break;}
case 579:
{ yyval = NULL; ;
    break;}
case 581:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 586:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); ;
    break;}
case 587:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); ;
    break;}
case 588:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); ;
    break;}
case 589:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); ;
    break;}
case 590:
{ yyval = yyvsp[0];;
    break;}
case 592:
{ yyval = yyvsp[0];;
    break;}
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); ;
    break;}
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); ;
    break;}
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); ;
    break;}
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); ;
    break;}
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); ;
    break;}
case 598:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); ;
    break;}
case 599:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); ;
    break;}
case 600:
{ yyval = 0; ;
    break;}
case 601:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); ;
    break;}
case 602:
{ yyval = (dsql_nod*) NOD_SHARED; ;
    break;}
case 603:
{ yyval = (dsql_nod*) NOD_PROTECTED ; ;
    break;}
case 604:
{ yyval = (dsql_nod*) 0; ;
    break;}
case 605:
{ yyval = (dsql_nod*) NOD_READ; ;
    break;}
case 606:
{ yyval = (dsql_nod*) NOD_WRITE; ;
    break;}
case 608:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 609:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); ;
    break;}
case 610:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); ;
    break;}
case 611:
{ yyval = 0; ;
    break;}
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 614:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); ;
    break;}
case 615:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 616:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); ;
    break;}
case 617:
{ yyval = NULL; ;
    break;}
case 618:
{ yyval = yyvsp[0]; ;
    break;}
case 619:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 620:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 621:
{ yyval = NULL; ;
    break;}
case 622:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 623:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 624:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 625:
{ yyval = make_node (nod_list, 1, yyvsp[0]); ;
    break;}
case 626:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 627:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 629:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 630:
{ lex.limit_clause = true; ;
    break;}
case 631:
{ lex.limit_clause = false; ;
    break;}
case 632:
{ lex.first_detection = true; ;
    break;}
case 633:
{ lex.first_detection = false; ;
    break;}
case 634:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); ;
    break;}
case 635:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); ;
    break;}
case 636:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); ;
    break;}
case 637:
{ yyval = 0; ;
    break;}
case 638:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); ;
    break;}
case 639:
{ yyval = yyvsp[-2]; ;
    break;}
case 640:
{ yyval = yyvsp[-1]; ;
    break;}
case 641:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 642:
{ yyval = yyvsp[-1]; ;
    break;}
case 643:
{ yyval = yyvsp[0]; ;
    break;}
case 644:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 645:
{ yyval = 0; ;
    break;}
case 646:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 647:
{ yyval = 0; ;
    break;}
case 649:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 651:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 654:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 656:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 661:
{ yyval = yyvsp[-1]; ;
    break;}
case 662:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); ;
    break;}
case 664:
{ yyval = NULL; ;
    break;}
case 665:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 666:
{ yyval = NULL; ;
    break;}
case 668:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 672:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); ;
    break;}
case 673:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); ;
    break;}
case 674:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); ;
    break;}
case 677:
{ yyval = yyvsp[0]; ;
    break;}
case 678:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 679:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); ;
    break;}
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); ;
    break;}
case 681:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 682:
{ yyval = NULL; ;
    break;}
case 684:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); ;
    break;}
case 685:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); ;
    break;}
case 686:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); ;
    break;}
case 687:
{ yyval = make_node (nod_join_left, (int) 0, NULL); ;
    break;}
case 688:
{ yyval = make_node (nod_join_right, (int) 0, NULL); ;
    break;}
case 689:
{ yyval = make_node (nod_join_full, (int) 0, NULL); ;
    break;}
case 690:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); ;
    break;}
case 693:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 694:
{ yyval = NULL; ;
    break;}
case 696:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 698:
{ yyval = yyvsp[0]; ;
    break;}
case 699:
{ yyval = NULL; ;
    break;}
case 700:
{ yyval = yyvsp[0]; ;
    break;}
case 701:
{ yyval = NULL; ;
    break;}
case 702:
{ yyval = yyvsp[0]; ;
    break;}
case 703:
{ yyval = NULL; ;
    break;}
case 704:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); ;
    break;}
case 705:
{ yyval = 0; ;
    break;}
case 706:
{ yyval = make_node (nod_merge, (int) 0, NULL); ;
    break;}
case 707:
{ yyval = make_node (nod_merge, (int) 0, NULL); ;
    break;}
case 708:
{ yyval = 0; ;
    break;}
case 709:
{ yyval = 0; ;
    break;}
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 712:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); ;
    break;}
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 716:
{ yyval = make_node (nod_natural, (int) 0, NULL); ;
    break;}
case 717:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); ;
    break;}
case 718:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); ;
    break;}
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 721:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 722:
{ yyval = 0; ;
    break;}
case 723:
{ yyval = make_list (yyvsp[0]); ;
    break;}
case 724:
{ yyval = 0; ;
    break;}
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 727:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 728:
{ yyval = 0; ;
    break;}
case 729:
{ yyval = make_node (nod_flag, 0, NULL); ;
    break;}
case 730:
{ yyval = 0; ;
    break;}
case 731:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); ;
    break;}
case 732:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); ;
    break;}
case 733:
{ yyval = yyvsp[-1]; ;
    break;}
case 734:
{ yyval = 0; ;
    break;}
case 735:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); ;
    break;}
case 736:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); ;
    break;}
case 737:
{ yyval = NULL; ;
    break;}
case 738:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); ;
    break;}
case 739:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); ;
    break;}
case 742:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 743:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 744:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); ;
    break;}
case 747:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); ;
    break;}
case 748:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); ;
    break;}
case 750:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 751:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); ;
    break;}
case 752:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 753:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); ;
    break;}
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 755:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); ;
    break;}
case 759:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 760:
{ yyval = yyvsp[0]; ;
    break;}
case 762:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 765:
{ yyval = NULL; ;
    break;}
case 766:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 770:
{ yyval = NULL; ;
    break;}
case 771:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 773:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 775:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); ;
    break;}
case 776:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); ;
    break;}
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); ;
    break;}
case 779:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); ;
    break;}
case 781:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 783:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 784:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 787:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 788:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 789:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 790:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 792:
{ yyval = yyvsp[-1]; ;
    break;}
case 793:
{ yyval = make_node (nod_not, 1, yyvsp[0]); ;
    break;}
case 805:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 806:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 807:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 808:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 809:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 810:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 811:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 812:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 813:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 814:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 815:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 816:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 817:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 818:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 819:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 820:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 821:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 822:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 823:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 824:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 825:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 826:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 827:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 828:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); ;
    break;}
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); ;
    break;}
case 832:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); ;
    break;}
case 833:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); ;
    break;}
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); ;
    break;}
case 835:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 837:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); ;
    break;}
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); ;
    break;}
case 839:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 841:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 843:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); ;
    break;}
case 845:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); ;
    break;}
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); ;
    break;}
case 847:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); ;
    break;}
case 848:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); ;
    break;}
case 849:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); ;
    break;}
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); ;
    break;}
case 851:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); ;
    break;}
case 852:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); ;
    break;}
case 853:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); ;
    break;}
case 854:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); ;
    break;}
case 855:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); ;
    break;}
case 856:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); ;
    break;}
case 858:
{ yyval = make_list (yyvsp[-1]); ;
    break;}
case 859:
{ yyval = yyvsp[-1]; ;
    break;}
case 869:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); ;
    break;}
case 870:
{ yyval = yyvsp[0]; ;
    break;}
case 871:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 872:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 873:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); ;
    break;}
case 874:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 875:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 876:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			;
    break;}
case 877:
{ yyval = yyvsp[-1]; ;
    break;}
case 878:
{ yyval = yyvsp[-1]; ;
    break;}
case 882:
{ yyval = make_node (nod_dbkey, 1, NULL); ;
    break;}
case 883:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); ;
    break;}
case 884:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						;
    break;}
case 887:
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
case 888:
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
case 889:
{ yyval = make_node (nod_current_timestamp, 0, NULL); ;
    break;}
case 890:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); ;
    break;}
case 892:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 894:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); ;
    break;}
case 895:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); ;
    break;}
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); ;
    break;}
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); ;
    break;}
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); ;
    break;}
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); ;
    break;}
case 901:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); ;
    break;}
case 902:
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
case 903:
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
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); ;
    break;}
case 905:
{ yyval = make_parameter (); ;
    break;}
case 906:
{ yyval = make_node (nod_user_name, 0, NULL); ;
    break;}
case 907:
{ yyval = make_node (nod_user_name, 0, NULL); ;
    break;}
case 908:
{ yyval = make_node (nod_current_role, 0, NULL); ;
    break;}
case 909:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); ;
    break;}
case 910:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); ;
    break;}
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); ;
    break;}
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); ;
    break;}
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); ;
    break;}
case 914:
{ yyval = yyvsp[0]; ;
    break;}
case 915:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; ;
    break;}
case 917:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; ;
    break;}
case 918:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 919:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 920:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];;
    break;}
case 921:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];;
    break;}
case 923:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; ;
    break;}
case 924:
{ yyval = yyvsp[0];;
    break;}
case 929:
{ yyval = make_node (nod_agg_count, 0, NULL); ;
    break;}
case 930:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); ;
    break;}
case 931:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); ;
    break;}
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			;
    break;}
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			;
    break;}
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			;
    break;}
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			;
    break;}
case 936:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); ;
    break;}
case 937:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); ;
    break;}
case 938:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); ;
    break;}
case 939:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); ;
    break;}
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			;
    break;}
case 942:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 944:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); ;
    break;}
case 945:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); ;
    break;}
case 946:
{ yyval = yyvsp[0]; ;
    break;}
case 947:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); ;
    break;}
case 948:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 949:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); ;
    break;}
case 950:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); ;
    break;}
case 953:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); ;
    break;}
case 954:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 955:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); ;
    break;}
case 958:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); ;
    break;}
case 959:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 960:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 961:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); ;
    break;}
case 962:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); ;
    break;}
case 963:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); ;
    break;}
case 964:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); ;
    break;}
case 965:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); ;
    break;}
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); ;
    break;}
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); ;
    break;}
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); ;
    break;}
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); ;
    break;}
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); ;
    break;}
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); ;
    break;}
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); ;
    break;}
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); ;
    break;}
case 981:
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

