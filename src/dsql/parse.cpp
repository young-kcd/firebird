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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "../include/iberror.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../include/jrd/gds.h"
#include "../jrd/flags.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/parse_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
#include "../wal/wal.h"

// fwd. decl. - NOTE! We can't currently include jrd/err_proto.h into
// this file to get the real declaration (try and you'll see why). :-(
extern "C" TEXT* DLL_EXPORT ERR_string(TEXT*, int);

ASSERT_FILENAME
extern "C" {
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
#define YYSTYPE		NOD
#if defined(DEBUG) || defined(_DEBUG)
#define YYDEBUG		1
#endif
static CONST UCHAR INTERNAL_FIELD_NAME[] = "DSQL internal";	/* NTX: placeholder */
static CONST UCHAR NULL_STRING[] = "";

#ifndef SHLIB_DEFS
NOD DSQL_parse;
#else
extern NOD DSQL_parse;
#endif

static FLD field;
static FIL file;
static NOD field_name;
static TEXT *beginning;
static SSHORT log_defined, cache_defined;


# define ACTIVE 257
# define ADD 258
# define AFTER 259
# define ALL 260
# define ALTER 261
# define AND 262
# define ANY 263
# define AS 264
# define ASC 265
# define AT 266
# define AVG 267
# define AUTO 268
# define BASENAME 269
# define BEFORE 270
# define BEGIN 271
# define BETWEEN 272
# define BLOB 273
# define BY 274
# define CACHE 275
# define CAST 276
# define CHARACTER 277
# define CHECK 278
# define CHECK_POINT_LEN 279
# define COLLATE 280
# define COLLATION 281
# define COMMA 282
# define COMMIT 283
# define COMMITTED 284
# define COMPUTED 285
# define CONCATENATE 286
# define CONDITIONAL 287
# define CONSTRAINT 288
# define CONTAINING 289
# define COUNT 290
# define CREATE 291
# define CSTRING 292
# define CURRENT 293
# define CURSOR 294
# define DATABASE 295
# define DATE 296
# define DB_KEY 297
#ifdef DEBUG
#undef DEBUG
#endif
# define DEBUG		298
# define DECIMAL	299
# define DECLARE	300
# define DEFAULT	301
# define DELETE		302
# define DESC		303
# define DISTINCT	304
# define DO 305
# define DOMAIN 306
# define DROP 307
# define ELSE 308
# define END 309
# define ENTRY_POINT 310
# define EQL 311
# define ESCAPE 312
# define EXCEPTION 313
# define EXECUTE 314
# define EXISTS 315
# define EXIT 316
# define EXTERNAL 317
# define FILTER 318
# define FOR 319
# define FOREIGN 320
# define FROM 321
# define FULL 322
# define FUNCTION 323
# define GDSCODE 324
# define GEQ 325
# define GENERATOR 326
# define GEN_ID 327
# define GRANT 328
# define GROUP 329
# define GROUP_COMMIT_WAIT 330
# define GTR 331
# define HAVING 332
# define IF 333
# define IN 334
# define INACTIVE 335
# define INNER 336
# define INPUT_TYPE 337
# define INDEX 338
# define INSERT 339
# define INTEGER 340
# define INTO 341
# define IS 342
# define ISOLATION 343
# define JOIN 344
# define KEY 345
# define KW_CHAR 346
# define KW_DEC 347
# define KW_DOUBLE 348
# define KW_FILE 349
# define KW_FLOAT 350
# define KW_INT 351
# define KW_LONG 352
# define KW_NULL 353
# define KW_NUMERIC 354
# define KW_UPPER 355
# define KW_VALUE 356
# define LENGTH 357
# define LOGFILE 358
# define LPAREN 359
# define LEFT 360
# define LEQ 361
# define LEVEL 362
# define LIKE 363
# define LOG_BUF_SIZE 364
# define LSS 365
# define MANUAL 366
# define MAXIMUM 367
# define MAX_SEGMENT 368
# define MERGE 369
# define MESSAGE 370
# define MINIMUM 371
# define MODULE_NAME 372
# define NAMES 373
# define NATIONAL 374
# define NATURAL 375
# define NCHAR 376
# define NEQ 377
# define NO 378
# define NOT 379
# define NOT_GTR 380
# define NOT_LSS 381
# define NUM_LOG_BUFS 382
# define OF 383
# define ON 384
# define ONLY 385
# define OPTION 386
# define OR 387
# define ORDER 388
# define OUTER 389
# define OUTPUT_TYPE 390
# define OVERFLOW 391
# define PAGE 392
# define PAGES 393
# define PAGE_SIZE 394
# define PARAMETER 395
# define PASSWORD 396
# define PLAN 397
# define POSITION 398
# define POST_EVENT 399
# define PRECISION 400
# define PRIMARY 401
# define PRIVILEGES 402
# define PROCEDURE 403
# define PROTECTED 404
# define RAW_PARTITIONS 405
# define READ 406
# define REAL 407
# define REFERENCES 408
# define RESERVING 409
# define RETAIN 410
# define RETURNING_VALUES 411
# define RETURNS 412
# define REVOKE 413
# define RIGHT 414
# define RPAREN 415
# define ROLLBACK 416
# define SEGMENT 417
# define SELECT 418
# define SET 419
# define SHADOW 420
# define SHARED 421
# define SINGULAR 422
# define SIZE 423
# define SMALLINT 424
# define SNAPSHOT 425
# define SOME 426
# define SORT 427
# define SQLCODE 428
# define STABILITY 429
# define STARTING 430
# define STATISTICS 431
# define SUB_TYPE 432
# define SUSPEND 433
# define SUM 434
# define TABLE 435
# define THEN 436
# define TO 437
# define TRANSACTION 438
# define TRIGGER 439
# define UNCOMMITTED 440
# define UNION 441
# define UNIQUE 442
# define UPDATE 443
# define USER 444
# define VALUES 445
# define VARCHAR 446
# define VARIABLE 447
# define VARYING 448
# define VERSION 449
# define VIEW 450
# define WAIT 451
# define WHEN 452
# define WHERE 453
# define WHILE 454
# define WITH 455
# define WORK 456
# define WRITE 457
# define FLOAT 458
# define NUMBER 459
# define NUMERIC 460
# define SYMBOL 461
# define STRING 462
# define INTRODUCER 463
# define ACTION 464
# define ADMIN 465
# define CASCADE 466
# define FREE_IT 467
# define RESTRICT 468
# define ROLE 469
# define COLUMN 470
# define TYPE 471
# define EXTRACT 472
# define YEAR 473
# define MONTH 474
# define DAY 475
# define HOUR 476
# define MINUTE 477
# define SECOND 478
# define WEEKDAY 479
# define YEARDAY 480
# define TIME 481
# define TIMESTAMP 482
# define CURRENT_DATE 483
# define CURRENT_TIME 484
# define CURRENT_TIMESTAMP 485
# define NUMBER64BIT 486
# define SCALEDINT 487

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif



#ifdef __cplusplus

#ifndef yyerror
static void yyerror(TEXT *);
#endif

#ifndef yylex
#ifdef __EXTERN_C__
extern "C" {
	int yylex(USHORT, USHORT, USHORT, BOOLEAN *);
}
#else
int yylex(USHORT, USHORT, USHORT, BOOLEAN *);
#endif
#endif
int dsql_yyparse(USHORT, USHORT, USHORT, BOOLEAN *);

#endif // __cplusplus


#define yyclearin DSQL_yychar = -1
#define yyerrok DSQL_yyerrflag = 0
extern int DSQL_yychar;
extern int DSQL_yyerrflag;
#ifndef YYSTYPE
#define YYSTYPE int
#endif
static YYSTYPE yylval;
static YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
static int yy_yys[YYMAXDEPTH], *yys = yy_yys;
static YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else /* user does initial allocation */
static int *yys;
static YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256



/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		lex.c
 *	DESCRIPTION:	Lexical routine
 *
 */

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
    {\
    if ((to - string) >= MAX_TOKEN_LEN)        \
	yyabandon (-104, isc_token_too_long); \
    }
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT *lex_position(void);
static BOOLEAN long_int(NOD, SLONG *);
static FLD make_field(NOD);
static FIL make_file(void);
static NOD make_list(NOD);
static NOD make_node(NOD_TYPE, int, ...);
static NOD make_flag_node(NOD_TYPE, SSHORT, int, ...);
static BOOLEAN short_int(NOD, SLONG *, SSHORT);
static void stack_nodes(NOD, LLS *);
static int yylex(USHORT, USHORT, USHORT, BOOLEAN *);
static void yyerror(TEXT *);
static void yyabandon(SSHORT, STATUS);
static void check_log_file_attrs();

static TEXT *ptr, *end, *last_token, *line_start;
static SSHORT lines, att_charset;

struct TOK {
	USHORT tok_ident;
	const char *tok_string;
	USHORT tok_version;
};


const TOK tokens[] = {
#include "../dsql/keywords.h"
	{0, 0, 0}
};


void LEX_dsql_init(void)
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

	for (const TOK * token = tokens; token->tok_string; ++token) {
		SYM symbol;
		STR str;

		symbol = (SYM) ALLOCPV(type_sym, 0);
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		str = (STR) ALLOCPV(type_str, symbol->sym_length);
		str->str_length = symbol->sym_length;
		strncpy(reinterpret_cast < char *>(str->str_data), symbol->sym_string,
				symbol->sym_length);
		symbol->sym_object = (void *) str;
		HSHD_insert(symbol);
	}
}


void LEX_string( TEXT * string, USHORT length, SSHORT character_set)
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

	line_start = ptr = string;
	end = string + length;
	lines = 1;
	att_charset = character_set;
}


static void check_log_file_attrs(void)
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

	if (file->fil_partitions) {
		if (!file->fil_length) {
			yyabandon(-261, isc_log_length_spec);
			/* Total length of a partitioned log must be specified */
		}

		if (PARTITION_SIZE(OneK * file->fil_length, file->fil_partitions) <
			(OneK * MIN_LOG_LENGTH)) {
			yyabandon(-239, isc_partition_too_small);
			/* Log partition size too small */
		}
	}
	else {
		if ((file->fil_length) && (file->fil_length < MIN_LOG_LENGTH)) {
			yyabandon(-239, isc_log_too_small);	/* Log size too small */
		}
	}
}


static TEXT *lex_position(void)
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

	return ptr;
}


static BOOLEAN long_int( NOD string, SLONG * long_value)
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
	UCHAR *p;

	for (p = ((STR) string)->str_data; classes[*p] & CHR_DIGIT; p++)
		if (!(classes[*p] & CHR_DIGIT))
			return FALSE;

	*long_value = atol((char *) ((STR) string)->str_data);

	return TRUE;
}


static FLD make_field( NOD field_name)
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
	FLD field;
	STR string;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	if (field_name == NULL) {
		field = (FLD) ALLOCDV(type_fld, sizeof(INTERNAL_FIELD_NAME));
		strcpy(field->fld_name, (TEXT *) INTERNAL_FIELD_NAME);
		return field;
	}
	string = (STR) field_name->nod_arg[1];
	field = (FLD) ALLOCDV(type_fld, strlen((SCHAR *) string->str_data));
	strcpy(field->fld_name, (TEXT *) string->str_data);

	return field;
}


static FIL make_file(void)
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
	FIL temp_file;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	temp_file = (FIL) ALLOCD(type_fil);

	return temp_file;
}


static NOD make_list( NOD node)
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
	NOD *ptr;
	LLS stack, temp;
	USHORT l;
	NOD old;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	stack = 0;
	stack_nodes(node, &stack);
	for (l = 0, temp = stack; temp; temp = temp->lls_next)
		l++;

	old = node;
	node = (NOD) ALLOCDV(type_nod, l);
	node->nod_count = l;
	node->nod_type = nod_list;
	node->nod_flags = old->nod_flags;
	ptr = node->nod_arg + node->nod_count;

	while (stack)
		*--ptr = (NOD) LLS_POP(&stack);

	return node;
}


static NOD make_node( NOD_TYPE type, int count, ...)
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
	NOD node, *p;
	va_list ptr;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	node = (NOD) ALLOCDV(type_nod, count);
	node->nod_type = type;
	node->nod_count = count;
	p = node->nod_arg;
	VA_START(ptr, count);

	while (--count >= 0)
		*p++ = va_arg(ptr, NOD);

	return node;
}


static NOD make_flag_node( NOD_TYPE type, SSHORT flag, int count, ...)
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
	NOD node, *p;
	va_list ptr;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	node = (NOD) ALLOCDV(type_nod, count);
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_count = count;
	p = node->nod_arg;
	VA_START(ptr, count);

	while (--count >= 0)
		*p++ = va_arg(ptr, NOD);

	return node;
}


static BOOLEAN short_int( NOD string, SLONG * long_value, SSHORT range)
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
	UCHAR *p;
	SCHAR buf[10];
	BOOLEAN return_value;

	if (((STR) string)->str_length > 5)
		return FALSE;

	for (p = ((STR) string)->str_data; classes[*p] & CHR_DIGIT; p++)
		if (!(classes[*p] & CHR_DIGIT))
			return FALSE;

/* there are 5 or fewer digits, it's value may still be greater
 * than 32767... */

	buf[0] = ((STR) string)->str_data[0];
	buf[1] = ((STR) string)->str_data[1];
	buf[2] = ((STR) string)->str_data[2];
	buf[3] = ((STR) string)->str_data[3];
	buf[4] = ((STR) string)->str_data[4];
	buf[5] = '\0';

	*long_value = atoi(buf);

	switch (range) {
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


static void stack_nodes( NOD node, LLS * stack)
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
	NOD *ptr, *end;
	NOD curr_node, next_node, start_chain, end_chain, save_link;

	if (node->nod_type != nod_list) {
		LLS_PUSH(node, stack);
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
	end_chain = (NOD) NULL;
	curr_node = node;
	next_node = node->nod_arg[0];
	while (curr_node->nod_count == 2 &&
		   curr_node->nod_arg[0]->nod_type == nod_list &&
		   curr_node->nod_arg[1]->nod_type != nod_list &&
		   next_node->nod_arg[0]->nod_type == nod_list &&
		   next_node->nod_arg[1]->nod_type != nod_list) {

		/* pattern was found so reverse the links and go to next node */

		save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
	}

/* see if any chain was found */

	if (end_chain) {

		/* first, handle the rest of the nodes */
		/* note that next_node still points to the first non-pattern node */

		stack_nodes(next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */

		curr_node = end_chain;
		while (TRUE) {
			LLS_PUSH(curr_node->nod_arg[1], stack);
			if (curr_node == start_chain)
				break;
			save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
		}
		return;
	}

	for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes(*ptr, stack);
}


static int yylex(
				 USHORT client_dialect,
				 USHORT db_dialect,
				 USHORT parser_version, BOOLEAN * stmt_ambiguous)
{
/**************************************
 *
 *	y y l e x
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *p, class_, string[MAX_TOKEN_LEN], *buffer, *buffer_end,
		*new_buffer;
	SYM sym;
	SSHORT c;
	USHORT buffer_len;

	STR delimited_id_str;

/* Find end of white space and skip comments */

	for (;;) {
		if (ptr >= end)
			return -1;

		c = *ptr++;

		if (c == '\n') {
			lines++;
			line_start = ptr + 1;
		}

		if ((c == '/') && (*ptr == '*')) {
			ptr++;
			while (ptr < end) {
				if ((c = *ptr++) == '*') {
					if (*ptr == '/')
						break;
				}
				if (c == '\n') {
					lines++;
					line_start = ptr + 1;
				}
			}
			if (ptr >= end)
				return -1;
			ptr++;
			continue;
		}

#if (! ( defined JPN_SJIS || defined JPN_EUC) )
		class_ = classes[c];
#else
		c = c & 0xff;
		class_ = (JPN1_CHAR(c) ? CHR_LETTER : classes[c]);
#endif /*JPN_SJIS || JPN_EUC */

		if (!(class_ & CHR_WHITE))
			break;
	}

/* Depending on class of token, parse token */

	last_token = ptr - 1;

	if (class_ & CHR_INTRODUCER) {
		/* The Introducer (_) is skipped, all other idents are copied
		 * to become the name of the character set
		 */
		p = string;
		for (; ptr < end && classes[*ptr] & CHR_IDENT; ptr++) {
			if (ptr >= end)
				return -1;
			CHECK_COPY_INCR(p, UPPER7(*ptr));
		}

		CHECK_BOUND(p);
		*p = 0;

		/* make a string value to hold the name, the name 
		 * is resolved in pass1_constant */

		yylval = (NOD) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
	}

/* parse a quoted string, being sure to look for double quotes */

	if (class_ & CHR_QUOTE) {
		buffer = string;
		buffer_len = sizeof(string);
		buffer_end = buffer + buffer_len - 1;
		for (p = buffer;; p++) {
			if (ptr >= end) {
				if (buffer != string)
					gds__free(buffer);
				return -1;
			}
			/* *ptr is quote - if next != quote we're at the end */
			if ((*ptr == c) && ((++ptr == end) || (*ptr != c)))
				break;
			if (p > buffer_end) {
				new_buffer = (UCHAR *) gds__alloc(2 * buffer_len);
				/* FREE: at outer block */
				if (!new_buffer) {	/* NOMEM: */
					if (buffer != string)
						gds__free(buffer);
					return -1;
				}
				memcpy(new_buffer, buffer, buffer_len);
				if (buffer != string)
					gds__free(buffer);
				buffer = new_buffer;
				p = buffer + buffer_len;
				buffer_len = 2 * buffer_len;
				buffer_end = buffer + buffer_len - 1;
			}
			*p = *ptr++;
		}
		if (c == '"') {
			*stmt_ambiguous = TRUE;	/* string delimited by double quotes could be
									   **   either a string constant or a SQL delimited
									   **   identifier, therefore marks the SQL
									   **   statement as ambiguous  */
			if (client_dialect == SQL_DIALECT_V6_TRANSITION) {
				if (buffer != string)
					gds__free(buffer);
				yyabandon(-104, isc_invalid_string_constant);
			}
			else if (client_dialect >= SQL_DIALECT_V6) {
				if ((p - buffer) >= MAX_TOKEN_LEN) {
					if (buffer != string)
						gds__free(buffer);
					yyabandon(-104, isc_token_too_long);
				}
				yylval = (NOD) MAKE_string(buffer, p - buffer);
				delimited_id_str = (STR) yylval;
				delimited_id_str->str_flags |= STR_delimited_id;
				if (buffer != string)
					gds__free(buffer);
				return SYMBOL;
			}
		}
		yylval = (NOD) MAKE_string(buffer, p - buffer);
		if (buffer != string)
			gds__free(buffer);
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

	if ((class_ & CHR_DIGIT) ||
		((c == '.') && (ptr < end) && (classes[*ptr] & CHR_DIGIT))) {
		/* The following variables are used to recognize kinds of numbers. */

		BOOLEAN have_error = FALSE;	/* syntax error or value too large */
		BOOLEAN have_digit = FALSE;	/* we've seen a digit              */
		BOOLEAN have_decimal = FALSE;	/* we've seen a '.'                */
		BOOLEAN have_exp = FALSE;	/* digit ... [eE]                  */
		BOOLEAN have_exp_sign = FALSE;	/* digit ... [eE] {+-]             */
		BOOLEAN have_exp_digit = FALSE;	/* digit ... [eE] ... digit        */
		UINT64 number = 0;
		UINT64 limit_by_10 = MAX_SINT64 / 10;

		for (--ptr; ptr < end; ptr++) {
			c = *ptr;
			if (have_exp_digit && (!(classes[c] & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				   the token. */
				break;
			else if (have_exp_sign && (!(classes[c] & CHR_DIGIT))) {
				/* only digits can be accepted after "1E-" */
				have_error = TRUE;
				break;
			}
			else if (have_exp) {
				/* We've seen e or E, but nothing beyond that. */
				if (('-' == c) || ('+' == c))
					have_exp_sign = TRUE;
				else if (classes[c] & CHR_DIGIT)
					/* We have a digit: we haven't seen a sign yet,
					   but it's too late now. */
					have_exp_digit = have_exp_sign = TRUE;
				else {
					/* end of the token */
					have_error = TRUE;
					break;
				}
			}
			else if ('.' == c) {
				if (!have_decimal)
					have_decimal = TRUE;
				else {
					have_error = TRUE;
					break;
				}
			}
			else if (classes[c] & CHR_DIGIT) {
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = TRUE;

				if (number >= limit_by_10)
					/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8')) {
						have_error = TRUE;
						break;
					}
				number = number * 10 + (c - '0');
			}
			else if ((('E' == c) || ('e' == c)) && have_digit)
				have_exp = TRUE;
			else
				/* Unexpected character: this is the end of the number. */
				break;
		}

		/* We're done scanning the characters: now return the right kind
		   of number token, if any fits the bill. */

		if (!have_error) {
			assert(have_digit);

			if (have_exp_digit) {
				yylval = (NOD) MAKE_string((UCHAR *) last_token,
										   ptr - last_token);
				return FLOAT;
			}
			else if (!have_exp) {

				/* We should return some kind (scaled-) integer type
				   except perhaps in dialect 1. */

				if (!have_decimal && (number <= MAX_SLONG)) {
					yylval = (NOD) number;
					return NUMBER;
				}
				else {
					/* We have either a decimal point with no exponent
					   or a string of digits whose value exceeds MAX_SLONG:
					   the returned type depends on the client dialect,
					   so warn of the difference if the client dialect is
					   SQL_DIALECT_V6_TRANSITION.
					 */

					if (SQL_DIALECT_V6_TRANSITION == client_dialect) {
						/* Issue a warning about the ambiguity of the numeric
						 * numeric literal.  There are multiple calls because
						 * the message text exceeds the 119-character limit
						 * of our message database.
						 */
						ERRD_post_warning(isc_dsql_warning_number_ambiguous,
										  gds_arg_string,
										  ERR_string(last_token,
													 ptr - last_token),
										  gds_arg_end);
						ERRD_post_warning(isc_dsql_warning_number_ambiguous1,
										  gds_arg_end);
					}

					yylval = (NOD) MAKE_string((UCHAR *) last_token,
											   ptr - last_token);

					if (client_dialect < SQL_DIALECT_V6_TRANSITION)
						return FLOAT;
					else if (have_decimal)
						return SCALEDINT;
					else
						return NUMBER64BIT;
				}
			}					/* else if (!have_exp) */
		}						/* if (!have_error) */

		/* we got some kind of error or overflow, so don't recognize this
		 * as a number: just pass it through to the next part of the lexer.
		 */
	}

/* Restore the status quo ante, before we started our unsuccessful
   attempt to recognize a number. */
	ptr = last_token;
	c = *ptr++;
/* We never touched class, so it doesn't need to be restored. */

/* end of number-recognition code */


	if (class_ & CHR_LETTER) {
#if (! ( defined JPN_SJIS || defined JPN_EUC) )
		p = string;
		CHECK_COPY_INCR(p, UPPER(c));
		for (; ptr < end && classes[*ptr] & CHR_IDENT; ptr++) {
			if (ptr >= end)
				return -1;
			CHECK_COPY_INCR(p, UPPER(*ptr));
		}
#else
		for (; ptr <= end; c = *ptr++, c = c & 0xff) {
			if (KANJI1(c)) {
				CHECK_COPY_INCR(p, UPPER(c));
				if (ptr >= end)
					break;

				c = *ptr++;
				c = c & 0xff;
				if (!KANJI2(c)) {
					ptr--;
					break;
				}
				else
					CHECK_COPY_INCR(p, c);
			}
			else {
#ifdef JPN_SJIS
				if ((SJIS_SINGLE(c)) || (classes[c] & CHR_IDENT))
#else
				if (classes[c] & CHR_IDENT)
#endif
				{
					CHECK_COPY_INCR(p, UPPER(c));
					if (ptr >= end)
						break;
				}
				else {
					ptr--;
					break;
				}
			}
		}
#endif /* JPN_SJIS || JPN_EUC */

		CHECK_BOUND(p);
		*p = 0;
		sym =
			HSHD_lookup(0, (TEXT *) string, p - string, SYM_keyword,
						parser_version);
		if (sym) {
			yylval = (NOD) sym->sym_object;
			return sym->sym_keyword;
		};
		yylval = (NOD) MAKE_string(string, p - string);
		return SYMBOL;
	}

/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end) {
		sym = HSHD_lookup(0, last_token, 2, SYM_keyword, parser_version);
		if (sym) {
			++ptr;
			return sym->sym_keyword;
		}
	}

/* Single character punctuation are simply passed on */

	return c;
}


static void yyerror(TEXT * error_string)
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
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104, gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
				  0);
	else {
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
				  gds_arg_gds, gds_dsql_token_unk_err,
				  gds_arg_number, (SLONG) lines,
				  gds_arg_number, (SLONG) (last_token - line_start),
				  /* Token unknown - line %d, char %d */
				  gds_arg_gds, gds_random,
				  gds_arg_cstring, (int) (ptr - last_token), last_token, 0);
	}
}


static void yyabandon( SSHORT sql_code, STATUS error_symbol)
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

	ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) sql_code,
			  gds_arg_gds, error_symbol, 0);
}
static const yytabelem yyexca[] = {
	-1, 1,
	0, -1,
	-2, 0,
	-1, 194,
	46, 836,
	-2, 824,
	-1, 299,
	40, 820,
	46, 836,
	-2, 824,
	-1, 363,
	412, 76,
	-2, 206,
	-1, 465,
	437, 399,
	-2, 824,
	-1, 637,
	344, 601,
	-2, 577,
	-1, 1042,
	344, 601,
	-2, 578,
	-1, 1069,
	278, 225,
	379, 225,
	401, 225,
	408, 225,
	442, 225,
	-2, 213,
	-1, 1114,
	278, 339,
	-2, 123,
	-1, 1200,
	278, 225,
	379, 225,
	401, 225,
	408, 225,
	442, 225,
	-2, 214,
	-1, 1231,
	278, 339,
	-2, 124,
	-1, 1407,
	344, 601,
	-2, 331,
	-1, 1484,
	344, 601,
	-2, 332,
};
# define YYNPROD 842
# define YYLAST 3728
static const yytabelem yyact[] = {

	278, 1511, 1303, 596, 991, 269, 1471, 1185, 131, 1438,
	1428, 1039, 266, 853, 1512, 271, 1491, 1407, 850, 1414,
	567, 1422, 1421, 197, 1423, 128, 400, 916, 1408, 1386,
	222, 646, 641, 420, 528, 1305, 1416, 617, 265, 44,
	1189, 119, 45, 1178, 292, 758, 1323, 15, 940, 1232,
	120, 383, 593, 521, 1095, 937, 497, 906, 459, 699,
	611, 1078, 1050, 1080, 1201, 460, 270, 947, 1417, 122,
	159, 193, 791, 946, 914, 259, 280, 917, 263, 722,
	1082, 480, 721, 191, 1027, 136, 973, 792, 168, 527,
	207, 530, 130, 399, 205, 637, 785, 173, 478, 164,
	572, 708, 789, 966, 204, 1112, 744, 175, 377, 902,
	693, 494, 179, 206, 186, 223, 39, 790, 133, 571,
	1096, 257, 945, 120, 793, 468, 923, 474, 732, 190,
	690, 518, 769, 225, 996, 638, 1306, 226, 412, 396,
	569, 169, 575, 515, 766, 315, 737, 1235, 305, 304,
	303, 618, 371, 616, 707, 318, 466, 685, 192, 684,
	801, 1251, 1515, 1494, 509, 196, 187, 188, 47, 297,
	194, 461, 795, 308, 456, 160, 306, 307, 840, 1518,
	562, 214, 795, 264, 1281, 177, 672, 673, 674, 675,
	676, 677, 678, 679, 450, 335, 209, 297, 328, 449,
	562, 208, 368, 373, 1493, 448, 1410, 534, 224, 1283,
	345, 554, 424, 339, 354, 161, 640, 165, 46, 694,
	374, 686, 46, 992, 129, 193, 1468, 121, 121, 235,
	540, 137, 385, 559, 647, 194, 1113, 378, 193, 258,
	296, 92, 180, 134, 374, 163, 618, 329, 386, 787,
	397, 1492, 687, 554, 46, 1472, 796, 193, 367, 183,
	386, 761, 425, 452, 103, 384, 46, 1282, 788, 419,
	416, 367, 540, 297, 552, 559, 296, 187, 188, 364,
	555, 560, 551, 100, 548, 553, 549, 794, 546, 212,
	202, 46, 797, 382, 414, 94, 948, 794, 954, 1109,
	915, 121, 310, 798, 374, 47, 127, 974, 558, 109,
	557, 60, 635, 193, 374, 402, 552, 1425, 409, 651,
	99, 621, 555, 560, 551, 191, 548, 553, 549, 341,
	546, 1037, 578, 842, 433, 434, 435, 632, 124, 408,
	774, 550, 764, 372, 194, 773, 182, 163, 1211, 1462,
	558, 465, 557, 394, 1250, 506, 372, 42, 539, 327,
	141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
	151, 152, 153, 154, 155, 156, 502, 330, 505, 1463,
	556, 715, 1038, 550, 503, 45, 511, 385, 510, 102,
	712, 1110, 620, 418, 393, 163, 524, 525, 162, 98,
	539, 113, 576, 1175, 523, 702, 408, 1220, 799, 425,
	574, 331, 219, 115, 925, 541, 542, 706, 713, 573,
	384, 403, 556, 576, 101, 1504, 415, 710, 516, 367,
	904, 629, 501, 1218, 338, 533, 1281, 423, 532, 1122,
	405, 262, 103, 950, 630, 1292, 847, 634, 382, 939,
	256, 595, 504, 694, 112, 656, 488, 541, 542, 92,
	1387, 1283, 239, 846, 42, 1432, 69, 1296, 85, 250,
	455, 614, 462, 952, 1293, 78, 496, 70, 476, 86,
	84, 938, 565, 264, 62, 322, 701, 644, 645, 500,
	648, 649, 650, 652, 689, 407, 1179, 68, 580, 293,
	79, 100, 782, 903, 249, 264, 669, 655, 1295, 193,
	720, 293, 406, 94, 652, 1431, 472, 703, 117, 1282,
	1462, 191, 714, 668, 1228, 116, 1430, 700, 724, 624,
	926, 711, 247, 589, 323, 252, 736, 236, 99, 533,
	1499, 251, 532, 728, 729, 730, 731, 167, 254, 691,
	401, 254, 1204, 1396, 747, 587, 281, 619, 45, 390,
	723, 753, 407, 369, 857, 80, 104, 618, 281, 220,
	772, 754, 194, 709, 64, 969, 705, 248, 854, 1186,
	967, 1213, 87, 193, 755, 941, 942, 779, 1215, 385,
	45, 71, 193, 193, 615, 802, 516, 81, 778, 163,
	194, 82, 855, 820, 378, 397, 65, 102, 777, 566,
	66, 1046, 83, 73, 821, 752, 291, 98, 692, 163,
	576, 67, 805, 1481, 749, 1045, 815, 121, 717, 194,
	1429, 88, 47, 193, 781, 804, 47, 137, 1044, 1478,
	72, 566, 101, 734, 1334, 419, 324, 919, 1253, 366,
	803, 984, 740, 566, 841, 760, 856, 748, 750, 232,
	103, 305, 304, 303, 786, 187, 188, 414, 47, 844,
	818, 822, 915, 305, 304, 303, 194, 187, 188, 1461,
	47, 1459, 938, 704, 294, 295, 300, 301, 302, 306,
	307, 809, 768, 1424, 264, 42, 294, 295, 741, 388,
	812, 306, 307, 372, 1454, 47, 810, 387, 230, 229,
	618, 784, 849, 915, 217, 216, 864, 819, 975, 866,
	867, 868, 869, 870, 871, 872, 873, 874, 875, 891,
	808, 800, 813, 897, 591, 814, 859, 816, 380, 385,
	490, 1453, 894, 579, 1236, 526, 402, 92, 883, 1345,
	765, 350, 1344, 1371, 1455, 1299, 561, 682, 1219, 924,
	1032, 857, 410, 993, 843, 896, 911, 912, 485, 848,
	908, 908, 920, 680, 360, 854, 858, 484, 658, 660,
	662, 664, 666, 930, 909, 918, 1033, 486, 349, 100,
	352, 489, 963, 670, 960, 817, 1346, 1376, 1513, 855,
	264, 94, 45, 1340, 884, 218, 1182, 771, 485, 348,
	921, 1378, 100, 353, 889, 1021, 887, 881, 928, 888,
	882, 989, 990, 1528, 913, 492, 99, 1485, 898, 979,
	53, 857, 1024, 1043, 471, 959, 953, 561, 351, 775,
	1436, 54, 1399, 491, 818, 854, 57, 899, 49, 99,
	1514, 340, 336, 856, 931, 255, 955, 956, 957, 910,
	818, 1048, 643, 361, 642, 892, 895, 929, 58, 855,
	1040, 851, 359, 55, 934, 1144, 316, 1023, 476, 389,
	949, 326, 213, 968, 994, 999, 1002, 1005, 1008, 1011,
	1014, 1017, 1020, 1286, 1026, 102, 1029, 1030, 1054, 964,
	1051, 203, 982, 970, 961, 98, 1022, 517, 776, 1069,
	985, 981, 332, 219, 987, 1337, 980, 983, 102, 977,
	978, 1074, 1052, 856, 1053, 986, 1091, 879, 98, 988,
	101, 193, 811, 807, 193, 969, 57, 588, 52, 1067,
	1068, 421, 1070, 378, 334, 114, 191, 321, 1042, 45,
	45, 1075, 623, 101, 317, 1034, 1035, 392, 1087, 1001,
	1004, 1007, 1010, 1013, 1016, 1019, 622, 627, 631, 817,
	50, 1114, 344, 1025, 51, 1125, 818, 1066, 45, 818,
	234, 1098, 76, 75, 818, 817, 841, 738, 343, 746,
	333, 1094, 1097, 1097, 139, 1105, 1137, 1121, 745, 1093,
	1136, 1115, 1535, 585, 1071, 1072, 1465, 1076, 1111, 157,
	312, 1084, 1524, 314, 1507, 1463, 944, 1135, 141, 142,
	143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
	153, 154, 155, 156, 385, 125, 1107, 586, 170, 1102,
	1106, 1104, 193, 125, 1187, 733, 1103, 1143, 490, 1146,
	1083, 657, 1129, 125, 1190, 1525, 1120, 564, 125, 311,
	786, 1134, 1142, 1140, 1145, 1139, 1141, 125, 968, 1147,
	519, 1138, 461, 495, 694, 499, 171, 1205, 1226, 123,
	1176, 1194, 1167, 1168, 428, 1170, 1171, 667, 498, 1149,
	427, 594, 1181, 430, 426, 1177, 429, 665, 431, 489,
	1285, 817, 663, 428, 817, 1206, 890, 430, 1169, 817,
	735, 661, 431, 757, 759, 751, 1208, 1041, 485, 886,
	584, 385, 237, 1212, 56, 1214, 1209, 484, 725, 470,
	1086, 1203, 1079, 492, 908, 1202, 1207, 876, 1480, 1191,
	469, 1223, 742, 716, 512, 1242, 533, 1216, 45, 532,
	356, 491, 818, 242, 920, 370, 1196, 520, 96, 1117,
	1117, 264, 1210, 756, 1224, 1240, 1227, 918, 95, 863,
	432, 386, 264, 1246, 522, 97, 507, 1516, 1409, 1221,
	298, 1247, 1248, 90, 633, 1118, 1239, 1229, 1442, 296,
	231, 1238, 921, 1237, 1433, 89, 1480, 1241, 1395, 1245,
	430, 426, 1394, 429, 1393, 431, 1391, 493, 1119, 1116,
	193, 1479, 193, 1288, 107, 1389, 723, 1434, 862, 201,
	118, 1143, 419, 1388, 191, 43, 106, 1270, 438, 357,
	479, 1297, 1051, 1273, 1274, 1538, 1301, 1523, 1480, 1480,
	1500, 1531, 628, 1500, 45, 1451, 1298, 1329, 199, 1278,
	1275, 1332, 1289, 1276, 1052, 1279, 1287, 1445, 1192, 703,
	908, 1193, 1294, 1326, 215, 1290, 1202, 1464, 477, 125,
	1325, 1300, 1328, 1327, 1130, 1330, 1284, 1131, 1148, 1331,
	582, 1339, 385, 1338, 1335, 193, 1333, 1097, 976, 228,
	1374, 430, 426, 238, 429, 1343, 431, 1190, 1347, 759,
	1375, 227, 458, 193, 210, 1385, 974, 878, 463, 430,
	426, 457, 429, 659, 431, 419, 1123, 1373, 852, 1124,
	1368, 1099, 386, 1088, 1100, 1372, 1089, 1381, 1366, 1380,
	783, 428, 1364, 1365, 1390, 1377, 1382, 427, 971, 1369,
	296, 783, 727, 1367, 625, 428, 120, 610, 592, 578,
	274, 427, 273, 932, 381, 583, 933, 726, 1503, 386,
	1401, 1404, 454, 1405, 422, 297, 1402, 1397, 386, 221,
	296, 1530, 421, 880, 1392, 806, 863, 296, 583, 1418,
	376, 297, 718, 1419, 363, 719, 1486, 391, 375, 1411,
	1412, 1482, 1403, 610, 365, 404, 274, 1450, 273, 1379,
	193, 1363, 1437, 1413, 1441, 1362, 581, 1361, 1420, 582,
	1360, 297, 1440, 1359, 1426, 1439, 296, 1358, 1357, 120,
	1356, 1446, 293, 1355, 1444, 1354, 1353, 1418, 1448, 1435,
	1352, 1443, 1351, 1350, 1349, 1348, 193, 193, 428, 1447,
	1449, 385, 1342, 1457, 427, 1460, 1477, 1458, 1475, 1440,
	1466, 1473, 1439, 45, 120, 1456, 1483, 1469, 1341, 1277,
	1418, 1252, 1446, 1484, 1249, 193, 1244, 1496, 1487, 1243,
	1490, 1488, 1195, 193, 1476, 1184, 1497, 419, 385, 1501,
	1495, 193, 1183, 1132, 1128, 1475, 1127, 1474, 1473, 1126,
	1047, 1508, 865, 1505, 120, 654, 1506, 1489, 1446, 1510,
	193, 193, 224, 1509, 1400, 193, 385, 1526, 193, 1517,
	1527, 1476, 1440, 1521, 1398, 1439, 1519, 419, 1529, 193,
	419, 1370, 1222, 1446, 1474, 193, 1028, 1533, 1532, 428,
	1534, 1440, 1536, 1537, 1439, 427, 193, 419, 1539, 1522,
	1165, 1164, 1163, 1162, 1179, 1161, 1160, 428, 419, 1159,
	1158, 1157, 1520, 427, 1254, 1255, 1256, 1257, 1258, 1259,
	1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269,
	1180, 1156, 610, 293, 284, 274, 1155, 273, 1154, 1272,
	1153, 1152, 1151, 287, 305, 304, 303, 1150, 187, 188,
	297, 1073, 972, 770, 958, 296, 922, 282, 885, 861,
	839, 838, 767, 293, 277, 763, 762, 294, 295, 671,
	293, 739, 306, 307, 386, 319, 570, 508, 453, 293,
	284, 91, 612, 430, 426, 447, 429, 446, 431, 287,
	445, 444, 296, 59, 289, 430, 426, 443, 429, 442,
	431, 441, 440, 282, 439, 126, 597, 272, 275, 293,
	277, 274, 108, 273, 268, 1198, 430, 426, 267, 429,
	436, 431, 288, 279, 1174, 110, 297, 281, 612, 1315,
	17, 296, 1312, 11, 286, 609, 281, 608, 285, 607,
	289, 1307, 8, 606, 605, 604, 599, 603, 1197, 430,
	426, 602, 429, 275, 431, 601, 274, 600, 273, 598,
	1092, 1064, 430, 426, 417, 429, 37, 431, 288, 279,
	36, 297, 26, 25, 430, 426, 296, 429, 927, 431,
	286, 1180, 1452, 1415, 285, 437, 1280, 1188, 1049, 613,
	860, 639, 599, 636, 260, 305, 304, 303, 590, 187,
	188, 283, 1063, 430, 426, 395, 429, 233, 431, 111,
	32, 276, 626, 413, 1036, 845, 411, 253, 294, 295,
	246, 245, 244, 306, 307, 305, 304, 303, 299, 187,
	188, 243, 305, 304, 303, 613, 187, 188, 241, 290,
	240, 305, 304, 303, 194, 187, 188, 283, 294, 295,
	300, 301, 302, 306, 307, 294, 295, 276, 337, 284,
	306, 307, 166, 35, 294, 295, 34, 33, 287, 306,
	307, 305, 304, 303, 299, 187, 188, 398, 547, 544,
	543, 545, 282, 780, 951, 290, 538, 537, 293, 277,
	536, 535, 531, 529, 294, 295, 300, 301, 302, 306,
	307, 211, 77, 1077, 467, 487, 140, 612, 893, 464,
	313, 683, 158, 325, 320, 1271, 138, 132, 309, 289,
	48, 428, 1406, 1336, 936, 293, 1225, 427, 1018, 1108,
	935, 998, 379, 428, 1467, 284, 1427, 451, 1502, 427,
	1498, 1470, 1316, 1314, 287, 1311, 1310, 288, 279, 1309,
	1384, 1302, 1304, 1173, 428, 1081, 473, 135, 282, 286,
	427, 698, 697, 285, 293, 277, 1062, 430, 426, 696,
	429, 599, 431, 1015, 695, 1291, 998, 1200, 1065, 475,
	284, 275, 281, 1199, 274, 688, 273, 428, 743, 287,
	342, 1085, 901, 427, 1166, 289, 1217, 1090, 907, 297,
	428, 900, 483, 282, 296, 905, 427, 482, 481, 293,
	277, 514, 428, 347, 613, 1061, 430, 426, 427, 429,
	513, 431, 346, 288, 279, 185, 283, 1234, 1060, 430,
	426, 1233, 429, 1231, 431, 286, 276, 1230, 943, 285,
	289, 428, 355, 275, 1133, 962, 274, 427, 273, 563,
	305, 304, 303, 299, 187, 188, 358, 198, 195, 189,
	184, 297, 181, 178, 290, 176, 296, 174, 288, 279,
	172, 1101, 63, 294, 295, 300, 301, 302, 306, 307,
	286, 61, 965, 568, 285, 362, 200, 305, 304, 303,
	74, 187, 188, 105, 997, 1059, 430, 426, 93, 429,
	577, 431, 283, 16, 14, 13, 12, 10, 9, 7,
	294, 295, 276, 6, 5, 306, 307, 1058, 430, 426,
	4, 429, 3, 431, 2, 1, 305, 304, 303, 299,
	187, 188, 1057, 430, 426, 0, 429, 0, 431, 997,
	290, 0, 1056, 430, 426, 0, 429, 283, 431, 294,
	295, 300, 301, 302, 306, 307, 0, 276, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 305, 304, 303, 299, 187, 188, 0, 0, 0,
	0, 0, 0, 0, 0, 290, 0, 0, 0, 0,
	0, 0, 0, 0, 294, 295, 300, 301, 302, 306,
	307, 1012, 0, 0, 998, 428, 0, 0, 284, 0,
	0, 427, 0, 0, 1055, 430, 426, 287, 429, 0,
	431, 275, 0, 0, 274, 0, 273, 0, 877, 430,
	426, 282, 429, 0, 431, 0, 0, 293, 277, 297,
	0, 0, 0, 0, 296, 0, 534, 0, 0, 0,
	554, 0, 0, 0, 428, 653, 430, 426, 0, 429,
	427, 431, 0, 1009, 0, 0, 998, 428, 289, 540,
	284, 0, 559, 427, 0, 0, 0, 0, 0, 287,
	0, 0, 0, 275, 0, 0, 274, 0, 273, 0,
	0, 0, 0, 282, 0, 0, 288, 279, 0, 293,
	277, 297, 0, 0, 0, 0, 296, 0, 286, 0,
	0, 0, 285, 552, 0, 0, 0, 0, 0, 555,
	560, 551, 0, 548, 553, 549, 0, 546, 0, 0,
	289, 0, 0, 0, 428, 0, 0, 0, 0, 0,
	427, 0, 0, 0, 0, 275, 0, 558, 274, 557,
	273, 0, 0, 0, 0, 0, 428, 0, 288, 279,
	0, 0, 427, 297, 0, 0, 0, 997, 296, 0,
	286, 428, 0, 0, 285, 283, 0, 427, 0, 0,
	550, 428, 0, 0, 0, 276, 0, 427, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 539, 0, 305,
	304, 303, 299, 187, 188, 0, 0, 0, 0, 0,
	0, 0, 0, 290, 0, 0, 0, 0, 0, 556,
	0, 0, 294, 295, 300, 301, 302, 306, 307, 997,
	0, 0, 0, 0, 163, 0, 0, 283, 0, 0,
	0, 1006, 0, 0, 998, 0, 0, 276, 284, 0,
	0, 0, 0, 428, 541, 542, 0, 287, 0, 427,
	0, 305, 304, 303, 299, 187, 188, 428, 0, 0,
	0, 282, 0, 427, 0, 290, 0, 293, 277, 0,
	0, 0, 0, 0, 294, 295, 300, 301, 302, 306,
	307, 0, 0, 0, 428, 0, 0, 0, 0, 0,
	427, 0, 0, 1003, 0, 0, 998, 0, 289, 0,
	284, 0, 0, 0, 0, 0, 0, 0, 0, 287,
	0, 0, 0, 275, 0, 0, 274, 0, 273, 0,
	0, 0, 0, 282, 0, 0, 288, 279, 0, 293,
	277, 297, 0, 0, 0, 0, 296, 0, 286, 0,
	0, 0, 285, 0, 0, 0, 0, 275, 0, 261,
	274, 1209, 273, 0, 0, 1000, 0, 0, 998, 0,
	289, 0, 284, 0, 0, 297, 0, 0, 0, 0,
	296, 287, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 38, 0, 0, 282, 0, 0, 288, 279,
	0, 293, 277, 1308, 1319, 0, 1318, 997, 0, 1320,
	286, 0, 0, 0, 285, 283, 0, 0, 0, 0,
	0, 0, 0, 1321, 0, 276, 0, 0, 0, 1322,
	0, 0, 289, 0, 0, 0, 0, 0, 0, 305,
	304, 303, 299, 187, 188, 0, 0, 0, 0, 0,
	0, 0, 0, 290, 0, 0, 0, 0, 0, 0,
	288, 279, 294, 295, 300, 301, 302, 306, 307, 997,
	0, 0, 286, 0, 0, 0, 285, 283, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 276, 0, 1313,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 305, 304, 303, 299, 187, 188, 0, 42, 0,
	0, 0, 0, 0, 0, 290, 0, 0, 0, 0,
	0, 0, 0, 1317, 294, 295, 300, 301, 302, 306,
	307, 997, 0, 41, 0, 275, 0, 0, 274, 283,
	273, 0, 0, 995, 1324, 0, 998, 0, 0, 276,
	284, 194, 0, 297, 0, 0, 0, 0, 296, 287,
	0, 0, 0, 305, 304, 303, 299, 187, 188, 0,
	0, 0, 0, 282, 0, 0, 0, 290, 0, 293,
	277, 0, 0, 0, 284, 0, 294, 295, 300, 301,
	302, 306, 307, 287, 0, 0, 0, 275, 430, 426,
	274, 429, 273, 431, 0, 0, 0, 282, 0, 0,
	289, 0, 0, 293, 277, 297, 824, 823, 825, 0,
	296, 0, 534, 0, 0, 0, 554, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 288, 279,
	0, 0, 0, 0, 289, 540, 0, 0, 559, 0,
	286, 0, 0, 0, 285, 0, 0, 0, 275, 0,
	0, 274, 0, 273, 0, 0, 0, 0, 0, 0,
	281, 0, 288, 279, 0, 0, 297, 0, 0, 0,
	0, 296, 0, 0, 286, 0, 0, 0, 285, 552,
	0, 0, 0, 0, 0, 555, 560, 551, 0, 548,
	553, 549, 0, 546, 0, 0, 0, 0, 0, 997,
	0, 0, 0, 0, 0, 0, 0, 283, 0, 0,
	0, 0, 0, 558, 0, 557, 0, 276, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 305, 304, 303, 299, 187, 188, 0, 0, 0,
	0, 283, 0, 0, 0, 290, 550, 0, 0, 0,
	0, 276, 284, 0, 294, 295, 300, 301, 302, 306,
	307, 287, 0, 539, 0, 305, 304, 303, 299, 187,
	188, 0, 0, 0, 0, 282, 0, 0, 0, 290,
	0, 293, 277, 0, 0, 556, 0, 0, 294, 295,
	300, 301, 302, 306, 307, 0, 0, 0, 0, 275,
	681, 0, 274, 0, 273, 0, 0, 0, 0, 0,
	0, 0, 289, 0, 284, 0, 0, 297, 831, 0,
	541, 542, 296, 287, 0, 0, 428, 0, 0, 0,
	0, 0, 427, 0, 0, 835, 0, 282, 281, 0,
	288, 279, 0, 293, 277, 0, 0, 0, 0, 0,
	0, 0, 286, 0, 0, 0, 285, 0, 0, 0,
	275, 0, 0, 274, 0, 273, 0, 0, 0, 0,
	0, 826, 0, 0, 289, 284, 0, 0, 297, 0,
	834, 0, 0, 296, 287, 0, 0, 0, 837, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 282, 0,
	0, 0, 288, 279, 293, 277, 0, 827, 0, 833,
	0, 0, 0, 0, 286, 0, 0, 0, 285, 283,
	0, 0, 0, 830, 0, 832, 828, 829, 0, 276,
	0, 0, 0, 0, 0, 289, 0, 0, 0, 0,
	0, 0, 0, 305, 304, 303, 299, 187, 188, 0,
	0, 0, 0, 0, 0, 0, 1209, 290, 0, 0,
	0, 0, 0, 288, 279, 0, 294, 295, 300, 301,
	302, 306, 307, 0, 0, 286, 836, 0, 0, 285,
	0, 283, 0, 0, 0, 0, 0, 38, 0, 0,
	0, 276, 0, 0, 1383, 0, 0, 0, 1308, 1319,
	0, 1318, 1172, 0, 1320, 305, 304, 303, 299, 187,
	188, 0, 0, 0, 0, 0, 0, 0, 1321, 290,
	0, 0, 0, 0, 1322, 0, 284, 0, 294, 295,
	300, 301, 302, 306, 307, 287, 0, 0, 0, 0,
	0, 275, 283, 0, 274, 0, 273, 0, 0, 282,
	0, 0, 276, 0, 0, 293, 277, 0, 0, 297,
	0, 0, 0, 1031, 296, 0, 305, 304, 303, 299,
	187, 188, 0, 0, 0, 0, 0, 0, 0, 0,
	290, 0, 0, 0, 1313, 0, 289, 284, 0, 294,
	295, 300, 301, 302, 306, 307, 287, 0, 0, 0,
	0, 0, 0, 42, 0, 0, 0, 0, 0, 0,
	282, 0, 0, 0, 288, 279, 293, 277, 1317, 0,
	0, 0, 0, 0, 0, 0, 286, 0, 41, 0,
	285, 0, 0, 0, 0, 0, 0, 1387, 0, 1324,
	0, 0, 0, 0, 0, 0, 194, 289, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 288, 279, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 286, 653, 430,
	426, 285, 429, 283, 431, 0, 0, 0, 0, 0,
	0, 0, 0, 276, 0, 0, 0, 824, 823, 825,
	0, 0, 0, 0, 0, 0, 0, 305, 304, 303,
	299, 187, 188, 0, 0, 0, 0, 0, 0, 0,
	0, 290, 0, 0, 0, 0, 0, 0, 437, 0,
	294, 295, 300, 301, 302, 306, 307, 0, 0, 0,
	0, 0, 0, 0, 283, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 276, 0, 0, 0, 284, 0,
	0, 0, 0, 0, 0, 0, 0, 287, 305, 304,
	303, 299, 187, 188, 0, 0, 19, 0, 0, 0,
	0, 282, 290, 0, 0, 0, 0, 293, 277, 0,
	0, 294, 295, 300, 301, 302, 306, 307, 22, 0,
	0, 0, 0, 0, 0, 0, 23, 0, 0, 0,
	0, 0, 0, 18, 0, 24, 0, 38, 289, 0,
	0, 0, 27, 0, 0, 0, 0, 0, 0, 29,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 28, 0, 0, 288, 279, 0, 0,
	0, 0, 0, 0, 21, 0, 0, 0, 286, 0,
	0, 0, 285, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 831,
	0, 0, 0, 0, 0, 0, 0, 428, 0, 0,
	0, 20, 0, 427, 0, 283, 835, 0, 30, 0,
	0, 31, 0, 42, 40, 276, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 305,
	304, 303, 299, 187, 188, 0, 0, 0, 41, 0,
	0, 0, 826, 290, 0, 0, 0, 0, 0, 0,
	0, 834, 294, 295, 300, 301, 302, 306, 307, 837,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 827, 0,
	833, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 830, 0, 832, 828, 829, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 836
};
static const yytabelem yypact[] = {

	3175, -10000000, 1166, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, 246, 535,
	851, 595, -145, 171, 665, -10000000, -10000000, 162, 199, 163,
	-19, -145, 13, -10000000, -10000000, -10000000, -10000000, -10000000, 624,
		-10000000,
	87, -234, 775, -10000000, -10000000, -10000000, -153, -10000000,
		-10000000, -237,
	-234, -218, -230, -10000000, 548, -286, -216, -234, -216, 137,
	-10000000, -10000000, -237, 773, -230, -234, -218, -219, -202, -296,
	-226, -154, -197, -10000000, -10000000, -171, 578, -10000000, -237, -286,
	-230, -234, -218, -219, -171, -172, 559, -154, -197, 331,
	330, 368, 167, 1325, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000,
	-10000000, 1462, 1462, -10000000, -230, 487, 325, 324, 869, 273,
	-10000000, 661, 277, 848, -234, -202, 126, 517, 31, -10000000,
	-222, -10000000, 2457, -10000000, -10000000, -10000000, -10000000,
		-10000000, -296, -10000000,
	752, -10000000, -10000000, 619, -10000000, -10000000, 1575, -10000000,
		227, -60,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, 655,
	-10000000, 623, -10000000, -10000000, 1462, 511, -10000000, 9, -296, 513,
	-10000000, -10000000, -10000000, 1575, -10000000, 671, -10000000, 653,
		-10000000, 1462,
	-10000000, -10000000, -10000000, -10000000, -10000000, 394, -10000000,
		-10000000, -248, -10000000,
	886, -10000000, -10000000, 1183, -10000000, -10000000, 506, -10000000,
		-10000000, -10000000,
	-10000000, 526, -10000000, -182, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000,
	-10000000, -10000000, -10000000, -182, -10000000, -10000000, -233, 160,
		-241, -197,
	-10000000, 510, -10000000, -10000000, -226, -10000000, 1314, 323, 315,
		-233,
	156, -241, 638, -10000000, -49, -10000000, 46, -291, -138, -16,
	-10000000, 126, -10000000, -10000000, -10000000, -10000000, -10000000, 55,
		-10000000, -133,
	400, -10000000, -234, -10000000, -9, -286, -226, -10000000, -10000000,
		620,
	1320, -10000000, -10000000, -52, 1249, -10000000, 1079, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, -10000000, 3151, 3151, 2970, -10000000, -10000000,
		1182, -10000000,
	-10000000, -10000000, 1604, 1602, 1601, 1599, 1597, 1591, 1590, 1587,
	1585, -10000000, -10000000, -257, -263, -268, -10000000, -198, 1578,
		-10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, 1318,
	-10000000, -114, -117, -110, -314, 870, -10000000, -10000000, 104, -216,
	227, -10000000, 419, 429, 461, -60, -10000000, -10000000, 785, 787,
	-216, -10000000, -10000000, -10000000, -234, -61, -234, -10000000,
		-10000000, -10000000,
	-286, 104, 1577, -185, -234, 880, 769, 394, -10000000, 1113,
	1113, -296, -296, 372, -10000000, 2489, -10000000, 138, 770, -10000000,
	-10000000, 209, -10000000, 1576, -10000000, -10000000, -18, -10000000,
		-234, -230,
	288, -10000000, -10000000, -217, -10000000, -10000000, -10000000, 1365,
		-10000000, -10000000,
	1311, 1569, -10000000, -10000000, -10000000, -10000000, -310, -233, 152,
		616,
	-230, 1305, -10000000, 351, -10000000, 1304, -10000000, 811, 811,
		-10000000,
	-10000000, -10000000, 1353, 108, -10000000, -10000000, -10000000, -57,
		-57, -10000000,
	123, 1300, -10000000, 923, -10000000, 2, -10000000, 293, -10000000, 1123,
	-141, 176, 2635, -10000000, -199, -10000000, 3151, 3151, -227, 3151,
	3151, 3151, 3151, 1065, 1065, 2154, 1454, 775, 158, 1009,
	807, 798, 793, 783, 2635, 3151, -202, -287, -10000000, -10000000,
	-10000000, -10000000, -10000000, 2909, 752, -309, -209, -10000000,
		-10000000, -66,
	85, -209, 178, -20, -317, -10000000, -10000000, 175, -10000000, 88,
	79, 879, 1575, 1341, -10000000, 2489, -10000000, -10000000, 459,
		-10000000,
	-10000000, -10000000, -10000000, -296, 1088, -10000000, -10000000, 1298,
		-10000000, 1113,
	1113, 1113, 1113, -10000000, 744, -10000000, -10000000, 832, -10000000,
		-10000000,
	-10000000, 2489, 669, 1571, -10000000, 669, 314, 878, -117, -296,
	-10000000, 619, -10000000, -10000000, 769, -10000000, -10000000,
		-10000000, -10000000, 838,
	-10000000, -154, -10000000, -213, -10000000, -10000000, -296, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, 1072, 1022, 221, -10000000, -10000000, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, -10000000, 1566, 1565, 302, 1562, 1562, 1553, 457,
	-10000000, 170, -10000000, -10000000, -103, -108, -10000000, -10000000,
		562, -10000000,
	-10000000, -10000000, -10000000, -296, -10000000, 208, 173, -10000000, 90,
		1286,
	-10000000, -10000000, -24, -147, -10000000, -29, -10000000, -10000000,
		-241, -305,
	-10000000, -10000000, -226, 1569, 1334, -10000000, 612, -230, -147, 611,
	-10000000, -226, -291, 773, -227, 773, 408, 334, -10000000, 1532,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, -10000000,
	1307, 2696, 1561, 1560, -10000000, -10000000, -308, -10000000, -10000000,
		-10000000,
	-10000000, -116, -10000000, -10000000, 122, -234, -10000000, -234, 42,
		-10000000,
	-10000000, -10000000, -226, 2635, 542, 1532, 1274, 242, -10000000,
		-10000000,
	176, 1559, -10000000, -10000000, 1065, 823, -10000000, -10000000, 1065,
		804,
	804, 1125, 1249, -10000000, -10000000, 3151, -10000000, 1451, 3151, 3151,
	3151, 3151, 3151, 3151, 3151, 3151, 3151, 3151, 873, 2127,
	1263, 606, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000,
	1332, -10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, 744, 789,
	-10000000, -10000000, -10000000, 1558, 845, -10000000, -10000000,
		-10000000, -10000000, -10000000,
	1462, 474, 469, 828, -10000000, -154, -216, 1913, -10000000, -154,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, -216,
	-10000000, -10000000, 837, -10000000, 73, -296, -296, 461, -213, -213,
	-154, -159, -10000000, 203, -10000000, 1556, -10000000, 391, 93, 2635,
	391, -234, -10000000, 1312, -10000000, -10000000, -10000000, -10000000,
		870, 63,
	-10000000, 30, -10000000, 193, -10000000, 715, 251, 251, -10000000, 24,
	56, 254, -154, -154, -154, 1554, -10000000, 251, -10000000, -10000000,
	-154, 1553, -10000000, -10000000, -10000000, -10000000, -10000000, 435,
		209, -10000000,
	-10000000, -10000000, 540, -10000000, 1297, -10000000, -10000000,
		-10000000, 1552, 263,
	1244, -10000000, -10000000, -10000000, -217, -217, -230, -218, -219, -147,
	-10000000, 265, -10000000, -10000000, -10000000, -10000000, -10000000,
		-157, 604, 1262,
	1244, -147, 1236, -10000000, -10000000, -10000000, -10000000, 1532, 1532,
		-238,
	-10000000, 722, 3297, 2423, 2245, 2183, 2121, 1943, 1881, 1653,
	1608, 3151, 543, 3151, 1486, 3151, 2758, 407, 46, 46,
	-10000000, -10000000, -10000000, -10000000, -10000000, -75, -10000000,
		-10000000, -10000000, -10000000,
	538, 843, 176, 489, -10000000, 249, 236, 222, 1449, 242,
	-222, 2635, -10000000, 3151, 1051, -10000000, 2113, 2041, 2031, 2016,
	1994, 1927, 1914, 1865, 1701, 1660, -10000000, -10000000, 3151, 3151,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, 1462, 1462,
	1551, -10000000, -10000000, -10000000, -10000000, -10000000, 868,
		-10000000, 750, -10000000,
	-10000000, 73, -10000000, 864, 1113, 1282, -10000000, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, -10000000,
	-10000000, -10000000, 1532, -10000000, 213, 177, 177, 1280, -10000000,
		-10000000,
	168, 750, -10000000, -117, 175, -156, -50, -10000000, 775, -225,
	-10000000, -10000000, -10000000, -10000000, -10000000, 1116, -10000000,
		1127, -213, 1115,
	-225, 837, 16, 1275, 246, 1448, 1445, 1443, -154, 1233,
	1442, -10000000, 459, 1113, 690, 686, -10000000, 180, -24, -154,
	-10000000, -10000000, -154, -10000000, -147, 547, -147, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, 263, 1244, -10000000, 1234, -157, 1262, 1244, 890,
	-10000000, -10000000, -10000000, -10000000, 1249, 1547, 1542, -10000000,
		-10000000, 1249,
	1541, 1540, 1249, 1538, 1536, 1249, 1531, 1511, 1249, 1510,
	1509, 1249, 1506, 1505, 1249, 1503, 1502, 1249, 1501, 1500,
	1672, 3151, 3151, 1486, 3151, 2697, 1581, -10000000, 1126, 1249,
	1249, 3151, -10000000, 453, 1441, 1434, -10000000, -10000000, -10000000,
		182,
	1532, -226, 242, 176, -10000000, -10000000, -10000000, -10000000,
		-10000000, 1217,
	-10000000, -10000000, 1249, 1249, -141, -10000000, -10000000, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, 1431, 2489, 1647,
		1614, 784,
	3151, -10000000, 144, 1532, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000,
	855, 750, -10000000, -99, -10000000, -213, 189, -213, 197, -296,
	10, 717, -10000000, -10000000, -10000000, -30, -10000000, -10000000,
		-10000000, -10000000,
	2635, -10000000, 1236, -10000000, 1482, 855, -10000000, -10000000,
		-10000000, 800,
	264, 2457, -10000000, -10000000, 365, 203, -10000000, 251, 251, 837,
	-10000000, -10000000, -159, -10000000, 246, 1428, -10000000, -10000000,
		-10000000, 1425,
	-10000000, 251, -10000000, -10000000, 459, -213, -296, -296, 1423, -113,
	-10000000, 1420, -10000000, -10000000, 262, -10000000, -10000000,
		-10000000, -157, 1234,
	78, 78, 78, 78, 78, 78, 78, 78, 78, 78,
	78, 78, 78, 78, 78, 78, 3151, 1593, 1267, -10000000,
	1249, 1249, 3151, 3151, 1209, 1418, -10000000, -10000000, -10000000, 775,
	-10000000, 1249, -10000000, -10000000, -10000000, -10000000, 92, 408,
		1232, -10000000,
	820, 509, -10000000, 2635, 542, -10000000, -10000000, -10000000,
		-10000000, 811,
	784, -10000000, 66, 1249, -234, 714, -10000000, 750, -10000000, 2230,
	-10000000, -226, -10000000, -10000000, 193, -296, -10000000, -10000000,
		1113, -10000000,
	177, -10000000, -10000000, -10000000, 868, -10000000, 258, -10000000, 63,
		594,
	811, 365, -10000000, -10000000, -10000000, -10000000, 450, -10000000,
		-10000000, -10000000,
	-10000000, -10000000, 1417, -10000000, -10000000, 1401, 193, 380, 377,
		-10000000,
	-10000000, 440, 837, -10000000, 1394, 1393, 1392, 1391, 1389, 1385,
	1384, 1382, 1379, 1377, 1376, 1372, 1369, 1366, 1364, 1360,
	1249, 3151, 3151, 1249, 1249, -10000000, 1277, -10000000, 3151, -10000000,
	1481, -10000000, 384, -10000000, -226, -227, 1532, -10000000, 538,
		-10000000,
	-10000000, -10000000, 444, -234, -10000000, -10000000, 466, 1358, 1462,
		-10000000,
	855, -10000000, 2835, -10000000, -10000000, -10000000, 1164, 1156, -237,
		-10000000,
	-10000000, -10000000, 1147, 3151, -10000000, 1145, -10000000, 1143, 1139,
		150,
	46, 1474, 505, 501, 1464, 2489, -10000000, -10000000, -213, -10000000,
	-10000000, 3151, -10000000, -10000000, -10000000, -10000000, -141, 166,
		-10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -296, -296, -10000000,
		-10000000, -10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, -10000000, -10000000,
		-10000000, -10000000, -10000000, -10000000,
	-10000000, -10000000, -10000000, -10000000, 1249, 1249, -10000000,
		-10000000, -10000000, 1051,
	-160, -10000000, -10000000, -10000000, 408, 182, -10000000, 1462,
		-10000000, -10000000,
	309, -10000000, -10000000, -10000000, 8, -10000000, -10000000, 202,
		-10000000, -10000000,
	1135, -10000000, 1158, -10000000, -10000000, -10000000, -230, 499, 1532,
		111,
	1532, 1129, -10000000, 1249, -10000000, 542, 1213, 242, -10000000,
		-10000000,
	166, -10000000, -10000000, -141, 1356, 1201, 366, -10000000, -234,
		-10000000,
	309, -10000000, 297, 295, 77, -10000000, -10000000, 962, -10000000, 246,
	-235, -237, -10000000, -10000000, -10000000, 215, 111, 598, 1152,
		-10000000,
	-10000000, 582, -10000000, 1350, 538, 166, 483, 1345, 242, 542,
	-10000000, -160, -10000000, -10000000, 1457, -286, -10000000, -10000000,
		-10000000, 713,
	-10000000, -94, -215, -215, 2230, 202, -10000000, -10000000, -10000000,
		-10000000,
	129, 1199, 1323, -10000000, -10000000, -10000000, -10000000, 1094, -11,
		-10000000,
	111, 709, -10000000, 182, 242, 166, -10000000, 538, -10000000, -286,
	-10000000, -10000000, -10000000, 497, -302, -10000000, -10000000,
		-10000000, 1118, 139,
	1323, 1196, 707, 761, 2230, -10000000, -10000000, 2230, -10000000, 439,
	182, 1330, 1197, -10000000, -10000000, -10000000, -10000000, 1195, 111,
		-10000000,
	-10000000, -10000000, -10000000, -10000000, 2230, -238, 694, -10000000,
		1532, -10000000,
	-10000000, -286, 1194, -10000000, -10000000, 2230, 408, -10000000,
		-10000000, -10000000
};
static const yytabelem yypgo[] = {

	0, 2065, 2064, 2062, 2060, 2054, 2053, 2049, 1681, 2048,
	2047, 1672, 2046, 2045, 2044, 46, 2043, 1669, 20, 1195,
	649, 102, 86, 1183, 142, 117, 1621, 1155, 2040, 41,
	2038, 1158, 30, 32, 2033, 87, 72, 118, 112, 124,
	1168, 1175, 152, 2030, 2026, 2025, 1180, 2023, 2022, 44,
	96, 91, 93, 45, 140, 119, 100, 103, 1219, 120,
	2021, 25, 2012, 141, 14, 2011, 2010, 2007, 2005, 2003,
	2002, 2000, 1999, 1998, 1997, 108, 115, 110, 56, 60,
	27, 1996, 1989, 1985, 1984, 53, 37, 48, 98, 81,
	58, 1982, 89, 1978, 1977, 52, 77, 1973, 49, 1971,
	1967, 147, 3, 346, 1965, 1962, 1960, 1953, 809, 1951,
	143, 907, 131, 105, 74, 1948, 1947, 1945, 57, 1942,
	1941, 1938, 1937, 1936, 1932, 109, 1931, 1930, 1928, 106,
	998, 989, 1925, 128, 1923, 34, 130, 158, 111, 31,
	12, 1919, 1918, 51, 38, 76, 1917, 64, 65, 1915,
	22, 59, 1914, 221, 1909, 1902, 1901, 21, 24, 16,
	155, 834, 63, 35, 1897, 1896, 127, 1895, 80, 2,
	1892, 1891, 1890, 136, 1889, 1886, 1885, 1883, 1882, 1881,
	1880, 9, 1878, 15, 1877, 6, 4, 29, 1876, 10,
	1874, 1872, 738, 66, 1870, 1869, 1866, 1864, 55, 69,
	75, 1863, 26, 18, 11, 7, 1862, 17, 28, 1178,
	13, 145, 125, 101, 61, 23, 1860, 1858, 1857, 1856,
	1854, 994, 1853, 1852, 881, 302, 1851, 1850, 1849, 1848,
	1846, 947, 1845, 1844, 1843, 1842, 8, 1841, 1833, 1832,
	82, 122, 79, 73, 67, 1831, 1830, 1827, 1826, 1824,
	1823, 1821, 1820, 1819, 144, 1818, 1817, 132, 1807, 1806,
	1803, 1633, 1802, 1798, 1780, 1778, 1153, 1771, 1762, 1761,
	1760, 541, 1757, 557, 1756, 1755, 1754, 138, 1753, 1752,
	1750, 1749, 1747, 116, 1745, 139, 1738, 33, 338, 1734,
	441, 78, 437, 1733, 95, 135, 1731, 1730, 0, 1728,
	62, 1727, 40, 68, 1726, 19, 1723, 36, 1722, 1,
	1718, 1713, 1712, 550, 1710, 1706, 1704, 146, 126, 54,
	1700, 398, 1699, 1697, 1695, 1691, 1687, 1685, 1684, 1683,
	1679, 1677, 1675, 403, 134, 84, 1664, 1660, 1658, 1654,
	5, 1647, 319, 616, 43, 1645, 1609
};
static const yytabelem yyr1[] = {

	0, 1, 1, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 10,
	10, 10, 10, 10, 20, 20, 19, 19, 19, 30,
	30, 23, 31, 31, 31, 31, 31, 22, 22, 28,
	28, 24, 13, 13, 13, 13, 13, 13, 13, 34,
	25, 25, 25, 25, 35, 35, 35, 21, 21, 36,
	36, 36, 26, 26, 40, 27, 27, 42, 42, 7,
	43, 43, 45, 50, 50, 50, 47, 47, 47, 54,
	54, 55, 48, 48, 57, 57, 57, 57, 44, 6,
	60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
	60, 62, 62, 65, 65, 65, 73, 81, 81, 81,
	82, 82, 83, 83, 84, 84, 88, 88, 72, 91,
	91, 93, 93, 94, 94, 97, 97, 98, 99, 99,
	100, 101, 70, 74, 71, 85, 85, 104, 105, 105,
	107, 107, 108, 108, 108, 108, 108, 106, 106, 109,
	109, 110, 110, 110, 110, 112, 112, 112, 112, 111,
	111, 116, 116, 115, 89, 117, 117, 118, 121, 122,
	122, 123, 119, 120, 120, 124, 124, 125, 125, 126,
	126, 126, 87, 87, 87, 67, 127, 127, 127, 128,
	128, 129, 129, 130, 130, 130, 136, 77, 77, 132,
	132, 95, 95, 90, 141, 142, 56, 133, 133, 96,
	96, 96, 96, 134, 134, 146, 146, 147, 149, 149,
	149, 149, 149, 131, 148, 148, 152, 152, 152, 152,
	154, 155, 156, 151, 150, 150, 150, 150, 150, 157,
	158, 159, 159, 159, 159, 66, 164, 160, 160, 161,
	161, 165, 165, 166, 162, 162, 167, 167, 168, 169,
	169, 163, 163, 171, 171, 170, 170, 170, 170, 170,
	170, 170, 170, 170, 170, 170, 170, 170, 174, 175,
	176, 176, 177, 183, 179, 179, 179, 180, 180, 180,
	185, 185, 185, 185, 185, 185, 185, 185, 181, 181,
	181, 181, 178, 182, 182, 172, 172, 187, 188, 188,
	189, 189, 189, 189, 12, 191, 191, 191, 192, 192,
	192, 192, 192, 192, 69, 194, 197, 197, 197, 198,
	201, 206, 206, 207, 207, 208, 208, 138, 196, 78,
	80, 195, 195, 68, 211, 211, 211, 212, 212, 212,
	212, 212, 212, 213, 213, 214, 3, 216, 216, 216,
	216, 216, 216, 216, 222, 222, 224, 224, 224, 224,
	224, 224, 224, 217, 217, 225, 225, 225, 225, 225,
	225, 225, 221, 230, 230, 230, 230, 230, 230, 230,
	230, 230, 230, 230, 230, 230, 230, 230, 230, 230,
	227, 227, 229, 229, 228, 226, 226, 226, 223, 223,
	219, 220, 220, 231, 231, 231, 231, 232, 232, 218,
	233, 233, 234, 234, 9, 235, 235, 235, 235, 235,
	235, 235, 235, 235, 235, 235, 92, 92, 135, 135,
	238, 238, 241, 241, 243, 243, 51, 51, 240, 240,
	240, 240, 240, 240, 240, 240, 248, 248, 239, 239,
	239, 239, 249, 249, 59, 59, 59, 53, 53, 245,
	245, 245, 242, 242, 242, 253, 253, 253, 252, 252,
	251, 251, 251, 246, 246, 256, 254, 254, 254, 255,
	255, 247, 247, 247, 247, 257, 257, 16, 16, 16,
	259, 259, 259, 5, 14, 261, 261, 262, 262, 263,
	263, 258, 264, 264, 265, 265, 266, 266, 266, 266,
	267, 267, 268, 268, 269, 269, 271, 271, 271, 272,
	272, 272, 273, 273, 273, 270, 275, 275, 275, 276,
	276, 274, 274, 277, 279, 279, 278, 278, 260, 15,
	280, 280, 280, 281, 281, 284, 284, 285, 285, 63,
	63, 63, 282, 282, 286, 286, 283, 199, 199, 200,
	200, 289, 289, 290, 290, 290, 287, 293, 293, 294,
	294, 295, 295, 296, 296, 297, 297, 299, 299, 300,
	300, 209, 209, 29, 210, 210, 210, 210, 210, 210,
	210, 210, 203, 203, 301, 301, 302, 302, 204, 204,
	202, 202, 205, 205, 303, 304, 304, 304, 304, 304,
	305, 305, 306, 306, 307, 307, 308, 308, 308, 309,
	309, 11, 11, 310, 310, 8, 8, 311, 312, 313,
	17, 17, 314, 315, 316, 316, 173, 291, 291, 4,
	4, 317, 317, 317, 319, 319, 59, 318, 318, 320,
	320, 32, 32, 76, 75, 75, 140, 140, 140, 137,
	102, 102, 102, 102, 322, 322, 322, 322, 322, 322,
	322, 322, 322, 322, 322, 323, 323, 323, 323, 323,
	323, 323, 323, 328, 328, 328, 328, 328, 328, 328,
	328, 328, 328, 328, 328, 328, 328, 328, 328, 334,
	334, 324, 324, 325, 325, 325, 325, 326, 326, 330,
	330, 331, 331, 331, 331, 329, 332, 327, 327, 335,
	335, 333, 337, 79, 79, 79, 79, 79, 79, 79,
	79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
	79, 79, 79, 79, 79, 145, 145, 145, 338, 342,
	342, 143, 143, 343, 343, 343, 343, 343, 340, 340,
	340, 340, 340, 336, 336, 336, 336, 336, 336, 193,
	344, 49, 49, 18, 18, 215, 345, 52, 114, 244,
	244, 86, 339, 339, 339, 339, 339, 339, 339, 339,
	339, 339, 339, 339, 339, 339, 339, 341, 341, 346,
	346, 346, 346, 346, 346, 346, 346, 288, 288, 144,
	46, 250, 113, 139, 321, 153, 186, 237, 61, 58,
	190, 103, 64, 292, 33, 41, 298, 236, 37, 39,
	184, 38
};
static const yytabelem yyr2[] = {

	0, 3, 5, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 5, 15,
	17, 13, 15, 11, 2, 5, 3, 5, 3, 2,
	7, 3, 3, 3, 3, 5, 5, 7, 1, 7,
	1, 3, 15, 17, 13, 15, 13, 15, 9, 7,
	2, 7, 7, 7, 5, 5, 5, 2, 7, 3,
	5, 5, 2, 7, 3, 2, 7, 3, 5, 5,
	5, 7, 17, 2, 3, 11, 1, 2, 7, 2,
	7, 5, 2, 7, 5, 7, 9, 5, 19, 5,
	7, 15, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 3, 1, 3, 2, 13, 13, 3, 3, 1,
	1, 3, 1, 9, 1, 2, 2, 5, 17, 3,
	1, 7, 1, 1, 2, 2, 5, 3, 2, 2,
	5, 13, 3, 3, 7, 0, 2, 3, 1, 2,
	2, 5, 7, 9, 5, 5, 7, 1, 2, 2,
	5, 2, 2, 2, 9, 7, 7, 7, 7, 3,
	3, 13, 7, 3, 7, 2, 7, 5, 3, 0,
	4, 7, 3, 0, 2, 2, 4, 7, 9, 0,
	2, 4, 0, 2, 2, 11, 7, 5, 1, 2,
	7, 2, 2, 13, 7, 5, 13, 4, 2, 5,
	5, 5, 1, 3, 3, 5, 1, 5, 1, 3,
	3, 3, 3, 1, 2, 2, 5, 5, 5, 9,
	2, 3, 5, 5, 5, 1, 2, 2, 2, 2,
	5, 7, 15, 13, 3, 3, 5, 5, 1, 7,
	7, 3, 5, 5, 5, 17, 17, 7, 1, 5,
	1, 2, 7, 5, 3, 1, 2, 5, 11, 2,
	2, 7, 9, 2, 5, 4, 4, 7, 2, 2,
	2, 4, 7, 2, 4, 2, 5, 5, 13, 15,
	17, 13, 9, 5, 3, 7, 1, 5, 9, 1,
	2, 2, 2, 2, 7, 7, 7, 7, 2, 2,
	7, 7, 13, 7, 1, 2, 5, 9, 2, 7,
	5, 5, 5, 3, 9, 3, 7, 1, 2, 2,
	2, 7, 7, 7, 15, 3, 3, 7, 9, 17,
	5, 2, 7, 2, 2, 13, 7, 1, 1, 1,
	1, 7, 1, 19, 3, 3, 1, 5, 5, 5,
	5, 5, 5, 5, 1, 9, 5, 7, 7, 5,
	5, 7, 7, 5, 2, 5, 9, 7, 5, 5,
	5, 5, 7, 2, 7, 7, 7, 5, 5, 9,
	9, 11, 3, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 5, 5, 5, 3, 3, 3, 1, 5, 5,
	1, 2, 5, 5, 5, 5, 5, 2, 7, 15,
	2, 1, 2, 1, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 7, 5, 5, 2, 2, 2, 2,
	9, 11, 2, 7, 3, 7, 2, 4, 2, 2,
	2, 3, 3, 3, 3, 3, 2, 2, 9, 9,
	13, 11, 7, 1, 5, 5, 1, 7, 0, 9,
	3, 11, 9, 3, 9, 2, 4, 4, 2, 2,
	2, 4, 4, 5, 5, 3, 1, 7, 11, 2,
	2, 5, 7, 3, 5, 7, 1, 2, 2, 2,
	11, 11, 13, 7, 5, 2, 0, 5, 1, 2,
	1, 7, 2, 1, 2, 5, 2, 2, 2, 2,
	5, 5, 3, 5, 7, 2, 3, 7, 7, 3,
	5, 7, 3, 5, 1, 5, 3, 3, 1, 3,
	3, 2, 7, 5, 7, 1, 2, 7, 9, 7,
	3, 7, 9, 7, 1, 2, 7, 7, 7, 3,
	3, 1, 7, 1, 5, 1, 17, 3, 3, 3,
	3, 2, 7, 2, 5, 7, 5, 2, 7, 2,
	2, 13, 7, 7, 5, 7, 1, 2, 7, 2,
	2, 2, 5, 3, 3, 3, 5, 3, 5, 3,
	5, 1, 7, 1, 2, 7, 2, 7, 5, 1,
	5, 1, 5, 1, 9, 3, 5, 3, 3, 1,
	2, 7, 5, 2, 2, 5, 3, 9, 5, 2,
	7, 17, 11, 2, 7, 2, 2, 9, 9, 9,
	2, 2, 11, 11, 2, 7, 7, 2, 2, 15,
	15, 11, 7, 0, 2, 2, 3, 5, 0, 3,
	2, 2, 1, 7, 2, 7, 2, 7, 7, 3,
	2, 7, 7, 5, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 2,
	2, 11, 13, 7, 9, 11, 13, 7, 9, 7,
	9, 7, 9, 9, 11, 9, 9, 7, 9, 7,
	7, 17, 17, 2, 2, 2, 2, 2, 2, 2,
	5, 5, 7, 7, 7, 7, 7, 7, 7, 7,
	3, 3, 7, 3, 3, 3, 3, 3, 9, 2,
	7, 2, 5, 3, 3, 3, 3, 3, 2, 3,
	5, 5, 5, 2, 2, 2, 7, 7, 7, 3,
	3, 3, 5, 2, 5, 3, 3, 3, 3, 2,
	5, 3, 9, 11, 11, 11, 11, 11, 11, 11,
	11, 11, 11, 13, 9, 13, 13, 9, 7, 3,
	3, 3, 3, 3, 3, 3, 3, 2, 0, 3,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2
};
static const yytabelem yychk[] = {

	-10000000, -1, -2, -3, -4, -5, -6, -7, -8, -9,
	-10, -11, -12, -13, -14, -15, -16, -17, 298, 261,
	406, 339, 283, 291, 300, -311, -312, 307, 328, 314,
	413, 416, -280, -258, -259, -260, -314, -315, 302, -283,
	419, 443, 418, 59, -18, -215, 45, 459, -216, 313,
	435, 439, 403, 295, 306, 338, 273, 341, 273, -261,
	456, -60, 313, -62, 403, 435, 439, 450, 326, 295,
	306, 420, 469, 442, -43, 318, 317, -235, 313, 338,
	403, 435, 439, 450, 318, 306, 317, 420, 469, -19,
	-23, -26, 260, -30, 314, -40, -31, -41, 418, 339,
	302, 443, 408, 461, 403, -34, -19, -23, -26, 328,
	-261, -281, 441, 388, 321, 326, 438, 431, -209, -29,
	-236, 461, -199, 304, -288, 260, -345, 459, -61, 461,
	-29, -236, -218, -37, 461, -164, -33, 461, -219, -221,
	-230, 470, 471, 472, 473, 474, 475, 476, 477, 478,
	479, 480, 481, 482, 483, 484, 485, 461, -223, -64,
	461, -137, -321, 461, -29, -137, -262, 410, -61, -63,
	265, 303, -66, -33, -67, -29, -68, -37, -69, -38,
	461, -70, -103, 461, -71, -104, -49, 462, 463, -72,
	-90, -140, -137, -298, 461, -73, -52, -215, -74, -41,
	-44, -58, 461, 323, -61, -64, -33, -236, -37, -38,
	-58, -237, 461, 323, -52, -41, 384, 384, 437, 44,
	402, 44, -32, -76, 40, -32, -33, -19, -23, 384,
	384, 321, 386, -282, 319, -283, 260, 274, -209, -103,
	-264, -265, -266, -267, -268, -269, -270, 406, 451, 378,
	343, -271, 409, -272, 425, 338, 419, -298, 461, -200,
	-289, 42, -290, -291, -79, -144, -140, -338, -339, -340,
	-193, -183, -341, 45, 43, 40, 444, 297, -298, 356,
	-145, 353, 290, 434, 267, 371, 367, 276, 355, 327,
	472, -343, -49, 296, 481, 482, 63, 58, -46, 461,
	483, 484, 485, 460, 459, 458, 486, 487, -49, -217,
	-225, 307, 258, -227, 261, -211, 257, 335, -160, 40,
	-220, -231, 258, 307, 419, -222, -224, 419, 258, 307,
	437, 471, 257, 335, 321, -32, 341, -263, 425, -49,
	338, -160, -127, 317, 319, -32, -105, -107, -108, 394,
	357, 444, 396, 419, 462, -91, 264, 46, -81, 366,
	268, 337, -45, -46, 461, -46, -20, -29, 435, 403,
	-27, -42, -39, 444, 461, -40, -31, -75, -140, -191,
	-192, 40, -193, -143, -144, -340, 45, 384, 384, -20,
	403, -27, 319, 443, -283, -284, -285, -140, -256, -52,
	-202, -313, 453, 437, -266, 385, 457, 440, 284, 451,
	362, -274, -277, -278, -29, 435, -64, -316, -173, -140,
	-287, 321, 44, -292, 264, 461, 43, 286, 280, 45,
	42, 47, 91, -79, -79, -79, -337, 418, 46, 40,
	40, 40, 40, 40, 40, 40, 40, 40, 462, 462,
	462, -184, 461, 40, 44, -137, 288, -130, -131, -90,
	-148, 288, -137, -221, -228, 461, 470, -233, -212, 270,
	259, -161, 412, -165, -166, -141, -137, -231, -88, -111,
	-89, -115, -116, -119, 358, 349, 358, -232, -112, 330,
	279, 382, 364, -224, -138, 288, -101, -78, 301, 288,
	-137, -56, -29, 445, -283, -29, -64, -161, 40, 349,
	-49, -29, 264, -106, -109, -110, -89, -111, -112, 301,
	-108, -85, 61, -85, -49, -49, 373, -92, -135, -238,
	-51, -239, -240, -242, 273, -245, -246, -247, -248, 424,
	296, 481, 482, -252, -253, -251, 354, -255, 350, 352,
	407, 348, 340, 351, 277, 346, 446, 376, 374, 299,
	347, -321, 42, -82, 287, -59, 432, -18, -47, -54,
	40, -55, -56, 437, -29, -24, -33, -28, 44, 455,
	-39, 41, 44, 44, -192, -343, -20, 403, 321, -24,
	-286, 383, 44, -95, 280, -95, -102, 293, -322, 379,
	-323, -324, -325, -326, -327, -328, -329, -330, -331, -332,
	40, -79, 315, 422, -244, 486, 45, -86, 459, -273,
	449, 378, -273, -271, 406, 44, -279, 44, 319, 429,
	-202, -313, 44, 61, -202, 453, -293, -294, -295, -296,
	40, -33, -290, -292, -79, -79, -139, 461, -79, -79,
	-79, -342, -79, 41, 41, -199, 297, 42, -288, 304,
	-288, 304, -288, 304, -288, 304, -288, 304, -291, -79,
	-103, -346, 473, 474, 475, 476, 477, 478, 479, 480,
	-342, 41, -225, -226, 468, 466, -153, 461, -132, -135,
	-136, -92, -137, -77, 285, -152, -154, -155, -156, -151,
	442, 401, 320, -78, -153, 398, 437, 471, -213, 398,
	339, 443, 302, 339, 443, 302, 264, -160, 41, 44,
	-135, -240, -242, -89, -49, 40, 269, 44, -85, -85,
	-85, -85, -133, 301, -101, 278, -135, -317, 318, 40,
	-317, 384, 264, -128, -129, -130, -131, -49, -211, -138,
	-110, 277, -52, -86, -49, -78, 91, 91, -53, 277,
	-59, 40, 40, 40, 40, 448, -254, 40, -254, -257,
	40, 350, 400, 448, 448, 277, 346, -49, 390, -18,
	-250, 461, 412, 44, -54, -50, -51, 273, 292, -21,
	-25, -36, -35, -39, 444, 329, 403, 439, 450, 437,
	-42, 465, -140, -193, -143, -144, 41, 321, -24, -21,
	-25, 321, -75, -285, -63, -139, -63, 387, 262, 383,
	-102, -102, -79, 61, 60, 62, 325, 361, 380, 381,
	377, 272, 379, 363, 334, 289, 430, 342, 40, 40,
	486, -86, 449, -277, -29, -275, 421, 404, -173, -291,
	-203, 329, 44, -210, 336, 360, 414, 322, -295, -294,
	-297, 40, 93, 44, -79, 41, -79, -79, -79, -79,
	-79, -79, -79, -79, -79, -79, 264, 41, 44, 321,
	41, -133, -136, -78, -138, 40, 274, -76, 345, 345,
	278, -215, -137, -229, -135, -137, -78, -215, -138, -166,
	-120, -124, -125, 430, 357, -117, -118, -121, -49, -118,
	-112, -86, -86, -52, -114, 459, -80, -96, -143, 444,
	-144, -145, 40, -318, 368, 321, 437, -310, -291, -318,
	-29, -138, 41, 44, -212, -194, -197, -198, 418, 419,
	-87, 392, 393, -93, 301, -241, -243, -244, 45, -241,
	419, -249, 417, -114, 44, -52, -52, -52, 40, -244,
	-215, -257, -83, 357, -59, -48, -57, 40, -56, 395,
	-55, 41, 40, -22, 44, 455, 44, -39, -39, -33,
	-37, -38, -21, -25, 386, -21, 321, -21, -25, -102,
	-102, -186, 461, 41, -79, 260, -334, 426, 263, -79,
	260, -334, -79, 260, -334, -79, 260, -334, -79, 260,
	-334, -79, 260, -334, -79, 260, -334, -79, 260, -334,
	-79, 272, 363, 334, 289, 430, -79, -335, 40, -79,
	-79, 455, 353, 379, -283, -283, -276, 406, 457, -204,
	332, 274, -294, 344, 389, 389, 389, 41, -298, -299,
	-300, -144, -79, -79, -287, 41, 41, 41, 41, 41,
	41, 41, 41, 41, 41, -142, -56, -79, -79, -80,
	-78, -76, -76, 40, -80, -78, -138, -234, -214, 264,
	-162, -167, -168, 300, -125, -126, 266, -85, 41, 44,
	-122, -102, -320, -114, -193, -319, -59, -193, -319, 41,
	44, -65, -75, -76, -77, -162, -129, -213, -195, 455,
	441, -199, -113, 461, -80, -78, 93, 44, 58, 93,
	-113, -53, 423, 41, 44, -18, 41, 41, 41, -52,
	41, 44, 41, -84, -88, -85, 310, 310, -57, -50,
	-52, -52, -35, -36, 328, -35, -36, -22, 44, -21,
	40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	40, 40, 40, 40, 40, 40, 262, -79, -79, -335,
	-79, -79, 455, 312, -336, -333, -143, -193, -344, 418,
	444, -79, 353, 41, 41, -205, 397, -102, -301, -302,
	-140, -294, 41, 44, -202, 41, -92, 41, 41, -134,
	-146, -147, -148, -79, 408, -102, -80, -78, -163, 271,
	-168, 447, -86, 392, -86, 391, -118, -123, 423, 41,
	437, -291, 40, -163, -78, -196, 278, -198, 260, -200,
	-94, -97, -98, -99, -100, -101, 379, -96, -243, -244,
	-53, -114, -18, 41, 41, -244, -86, -49, -49, 41,
	467, 274, 41, 386, -333, -333, -333, -333, -333, -333,
	-333, -333, -333, -333, -333, -333, -333, -333, -333, -333,
	-79, 262, 312, -79, -79, 41, 44, 41, -199, -303,
	-304, 344, 427, 369, 44, 280, 384, -300, -203, -95,
	-147, -149, 379, 408, -151, 442, 401, -80, -29, 41,
	-162, -80, -171, -169, -170, -163, -173, -8, 313, -174,
	-175, -176, -11, 399, -177, -17, -178, 433, 316, 314,
	319, 333, 339, -15, 454, -90, -87, -118, -85, -80,
	-319, -78, -80, -214, 386, -198, -201, 321, -95, -98,
	353, 41, 41, -87, 372, 372, 356, -53, 41, 41,
	41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	41, 41, 41, 41, -79, -79, -143, -193, -344, -79,
	40, 369, -302, -139, -102, -204, 353, -29, 345, 41,
	-32, -80, -163, 309, -172, -169, -187, 452, 59, 59,
	-61, 59, -79, 59, 59, 59, 403, -15, 40, 341,
	40, -135, -86, -79, -80, -202, -206, -207, -208, -209,
	40, -49, -49, -287, -305, -306, -307, -303, -236, -205,
	-32, -150, -157, -158, 384, 309, -187, -188, -189, 428,
	324, 313, 263, 59, 59, -33, 341, -102, -181, -183,
	-140, -102, 59, -80, -203, 44, -210, -208, -207, -202,
	41, 44, -308, 375, 338, 388, -307, -150, -158, 384,
	-157, 384, 443, 302, 305, 44, -18, -190, 461, -61,
	-179, -185, 40, -183, -143, -140, -144, -181, 41, 59,
	44, 41, 41, -204, -207, 344, 41, -203, -305, 40,
	-64, -159, 466, 419, 378, -159, -169, -189, -180, 411,
	44, -185, -182, 264, 436, -140, -183, 305, -205, -207,
	-204, -309, -64, 301, 353, 464, 59, -181, 40, -183,
	-143, -140, -144, 41, 305, 294, -169, -169, 384, -205,
	41, 44, -181, -169, -186, 308, -102, -309, 41, -169
};
static const yytabelem yydef[] = {

	0, -2, 1, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 0, 0,
	0, 0, 506, 102, 0, 635, 636, 0, 0, 0,
	0, 506, 554, 497, 498, 499, 640, 641, 0, 550,
	0, 0, 818, 2, 18, 783, 0, 785, 356, 0,
	0, 0, 0, 410, 0, 0, 0, 0, 0, 508,
	505, 89, 0, 561, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 101, 69, 0, 0, 424, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 26, 28, 31, 62, 29, 64, 32, 33,
	34, 662, 662, 835, 0, 0, 0, 0, 0, 0,
	504, 563, 0, 0, 0, 0, 513, 0, 0, 591,
	593, 837, 0, 567, 568, 817, 784, 786, 0, 828,
	0, 593, 359, 346, 838, 360, 248, 834, 0, 0,
	382, 383, 384, 385, 386, 387, 388, 389, 390, 391,
	392, 393, 394, 395, 396, 397, 398, 399, 363, 0,
	832, 0, 669, 824, 662, 0, 503, 510, 0, 0,
	559, 560, 92, 248, 93, 188, 94, 0, 95, 662,
	841, 96, 132, 831, 97, 138, 137, 781, 0, 98,
	120, 203, 666, 0, -2, 99, 109, 787, 100, 133,
	70, 0, 829, 0, 425, 426, 427, 428, 429, 430,
	431, 432, 827, 0, 434, 435, 0, 0, 0, 0,
	27, 0, 35, 661, 0, 36, 317, 0, 0, 0,
	0, 0, 0, 549, 0, 551, 0, 0, 611, 0,
	511, 512, 514, 516, 517, 518, 519, 0, 522, 0,
	0, 525, 0, 526, 529, 0, 0, 592, 836, 0,
	569, 570, 571, 573, 647, 648, 733, 734, 735, 736,
	737, 738, 739, 0, 0, 0, 750, 751, 0, 753,
	754, 819, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 768, 769, 0, 0, 0, 779, 0, 0, -2,
	755, 756, 757, 763, 764, 765, 766, 767, 357, 358,
	373, 0, 225, 0, 400, 421, 344, 345, 250, 0,
	361, 411, 0, 0, 0, 362, 364, 337, 339, 0,
	0, 206, 408, 409, 0, 0, 0, 507, 509, 90,
	0, 250, 0, 0, 0, 0, 147, 139, 140, 135,
	135, 0, 0, 0, 782, 0, 119, 0, 110, 107,
	108, 466, 71, -2, 820, 433, 0, 24, 0, 0,
	40, 65, 67, 0, 839, 63, 30, 0, 664, 314,
	315, 0, 318, 319, 320, 761, 0, 0, 0, 0,
	0, 48, 49, 565, 552, 553, 555, 202, 202, 485,
	637, 638, 0, 0, 515, 520, 521, 534, 534, 523,
	0, 535, 541, 545, 546, 530, 548, 611, 644, 0,
	611, 0, 0, 574, 0, 833, 0, 0, 0, 0,
	0, 0, 0, 740, 741, 0, 0, 818, 0, 818,
	818, 818, 818, 818, 0, 0, 0, 0, 770, 771,
	772, 283, 840, 0, 0, 407, 0, 377, 378, 0,
	339, 0, 404, 0, 0, -2, 401, 354, 420, 0,
	0, 0, 248, 0, 251, 0, 204, 412, 413, 416,
	116, 159, 160, 0, 163, 172, 414, 415, 417, 135,
	135, 135, 135, 365, 208, 339, 368, 0, 369, 370,
	371, 0, 653, 0, 632, 653, 0, 0, 225, 0,
	187, 346, 337, 134, 148, 149, 151, 152, 153, 0,
	141, 0, 136, 0, 144, 145, 0, 339, 436, 437,
	438, 439, 446, 468, 466, 448, 449, 450, 451, 452,
	453, 454, 455, 473, 0, 470, 486, 486, 496, 0,
	493, 0, 456, 457, 478, 479, 475, 480, 0, 489,
	490, 667, 668, 0, 111, 0, 0, 656, 0, 77,
	206, 79, 0, 0, 25, 0, 41, 23, 0, 0,
	68, 663, 0, 0, 0, 762, 0, 0, 0, 0,
	562, 0, 0, 561, 0, 561, 610, 0, 670, 0,
	674, 675, 676, 677, 678, 679, 680, 681, 682, 683,
	0, 0, 0, 0, 500, 501, 0, 789, 791, 527,
	532, 0, 528, 524, 0, 0, 543, 0, 538, 531,
	642, 643, 0, 0, 603, 0, 576, -2, 579, 580,
	0, 586, 572, 575, 742, 743, 744, 823, 745, 746,
	747, 0, 759, 748, 749, 0, 752, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 809, 810, 811, 812, 813, 814, 815, 816,
	0, 808, 374, 375, 405, 406, 376, 825, 208, 436,
	195, 339, 337, 0, 198, 223, 226, 227, 228, 229,
	0, 0, 0, 0, 224, 0, 0, 0, 339, 0,
	347, 349, 351, 348, 350, 352, 337, 249, 247, 0,
	253, 446, 468, 117, 173, 0, 0, 0, 0, 0,
	0, 0, 340, 0, 367, 0, 372, 658, 0, 0,
	658, 0, 337, 0, 189, 191, 192, 186, 0, 0,
	150, 0, 142, 182, 146, 122, 0, 0, 447, 0,
	463, 0, 0, 0, 0, 0, 483, 0, 484, 491,
	0, 496, 494, 476, 477, 481, 482, 112, 466, 464,
	465, 821, 206, 206, 0, 81, 73, 74, 0, 38,
	21, 57, 50, 59, 0, 0, 0, 0, 0, 0,
	66, 0, 665, 321, 322, 323, 316, 0, 0, 44,
	46, 0, 564, 556, 557, 201, 558, 0, 0, 0,
	673, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	502, 790, 533, 542, 547, 0, 536, 537, 645, 646,
	609, 0, 0, 0, 594, 595, 597, 599, 579, 601,
	584, 0, 758, 0, 0, 792, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 206, 804, 0, 0,
	807, 340, 194, 199, 200, 339, 197, 230, 0, 0,
	0, 379, 380, 340, 339, 337, 423, 353, 255, 252,
	164, 174, 175, 179, 135, 0, 165, 169, 168, 162,
	418, 155, 156, 157, 158, 788, 366, 207, 209, 210,
	211, 212, 0, 649, 0, 466, 466, 0, 633, 650,
	0, 255, 185, 225, 354, 342, 325, 326, 818, 0,
	143, 183, 184, 340, 339, 0, 442, 444, 0, 0,
	0, 468, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 492, 114, 135, 0, 0, 82, 206, 0, 0,
	80, 78, 0, 19, 0, 0, 0, 60, 61, 54,
	55, 56, 38, 22, 39, 42, 0, 45, 47, 671,
	672, 639, 826, 684, 685, 0, 0, 709, 710, 686,
	0, 0, 687, 0, 0, 688, 0, 0, 689, 0,
	0, 690, 0, 0, 691, 0, 0, 692, 0, 0,
	0, 0, 0, 0, 0, 0, 713, 717, 0, 719,
	721, 0, 727, 0, 0, 0, 544, 539, 540, 613,
	0, 0, -2, 0, 596, 598, 600, 582, 583, 0,
	587, 589, 590, 760, 611, 793, 794, 795, 796, 797,
	798, 799, 800, 801, 802, 0, 0, 0, 0, -2,
	0, 231, 0, 0, 381, 402, 403, 340, 422, 339,
	0, 254, 256, 0, 176, 0, 180, 0, 0, 0,
	167, 0, 657, 659, 660, 0, 654, 655, 652, 631,
	0, 91, 103, 104, 0, 0, 190, 339, 338, 0,
	0, 0, 154, 822, -2, 0, 440, 0, 0, 468,
	467, 458, 0, 459, 0, 0, 472, 474, 469, 0,
	487, 0, 495, 106, 115, 0, 0, 0, 0, 84,
	87, 0, 53, 58, 0, 51, 52, 20, 0, 43,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 714, 718,
	720, 722, 0, 0, 0, 0, 773, 774, 775, 818,
	780, 723, 728, 725, 726, 566, 619, 608, 602, 604,
	606, 601, 585, 0, 603, 803, 205, 805, 806, 202,
	-2, 215, 339, 340, 0, 0, 419, 255, 340, 0,
	257, 0, 177, 181, 182, 0, 166, 170, 135, 340,
	466, 634, 339, 340, 0, 324, 0, 327, 0, 0,
	202, -2, 125, 127, 128, 129, 0, 121, 443, 445,
	441, 462, 0, 461, 471, 0, 182, 0, 0, 83,
	85, 0, 468, 37, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	711, 0, 0, 724, 715, 729, 0, 730, 0, 612,
	0, 615, 618, 617, 0, 0, 0, 588, 609, 193,
	216, 217, 0, 0, 220, 221, 0, 0, 662, 340,
	0, 246, 0, 263, 259, 260, 0, 0, 0, 268,
	269, 270, 0, 0, 273, 0, 275, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 178, 161, 0, 131,
	651, 0, 245, 340, 341, 328, 611, 0, 118, 126,
	130, 460, 488, 113, 0, 0, 86, 75, 693, 701,
	694, 702, 695, 703, 696, 704, 697, 705, 698, 706,
	699, 707, 700, 708, 712, 716, 776, 777, 778, 0,
	619, 616, 605, 607, 581, 613, 218, 662, 222, 196,
	238, 233, 355, 261, 0, 264, 305, 0, 265, 266,
	0, 271, 0, 274, 276, 277, 0, 0, 0, 0,
	0, 0, 171, 340, 343, 603, 330, -2, 333, 334,
	0, 88, 72, 611, 0, 620, 0, 623, 624, 732,
	238, 232, 234, 235, 0, 262, 306, 0, 308, 0,
	0, 0, 313, 267, 272, 286, 0, 0, 0, 298,
	299, 0, 258, 0, 609, 0, 0, 333, 601, 603,
	614, 619, 622, 626, 0, 0, 625, 219, 237, 0,
	236, 0, 0, 0, 0, 0, 310, 311, 830, 312,
	289, 284, 0, 290, 291, 292, 293, 304, 0, 282,
	0, 0, 105, 613, -2, 0, 336, 609, 621, 0,
	628, 239, 241, 0, 0, 240, 307, 309, 0, 0,
	0, 0, 0, 0, 0, 300, 301, 0, 329, 601,
	613, 0, 629, 242, 243, 244, 278, 287, 0, 294,
	295, 296, 297, 285, 0, 0, 281, 302, 0, 731,
	627, 0, 0, 279, 303, 0, 335, 630, 288, 280
};
typedef struct
#ifdef __cplusplus
  yytoktype
#endif
{
	char *t_name;
	int t_val;
} yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0			/* don't allow debugging */
#endif

#if YYDEBUG

static CONST yytoktype yytoks[] = {
	"ACTIVE", 257,
	"ADD", 258,
	"AFTER", 259,
	"ALL", 260,
	"ALTER", 261,
	"AND", 262,
	"ANY", 263,
	"AS", 264,
	"ASC", 265,
	"AT", 266,
	"AVG", 267,
	"AUTO", 268,
	"BASENAME", 269,
	"BEFORE", 270,
	"BEGIN", 271,
	"BETWEEN", 272,
	"BLOB", 273,
	"BY", 274,
	"CACHE", 275,
	"CAST", 276,
	"CHARACTER", 277,
	"CHECK", 278,
	"CHECK_POINT_LEN", 279,
	"COLLATE", 280,
	"COLLATION", 281,
	"COMMA", 282,
	"COMMIT", 283,
	"COMMITTED", 284,
	"COMPUTED", 285,
	"CONCATENATE", 286,
	"CONDITIONAL", 287,
	"CONSTRAINT", 288,
	"CONTAINING", 289,
	"COUNT", 290,
	"CREATE", 291,
	"CSTRING", 292,
	"CURRENT", 293,
	"CURSOR", 294,
	"DATABASE", 295,
	"DATE", 296,
	"DB_KEY", 297,
	"DEBUG", 298,
	"DECIMAL", 299,
	"DECLARE", 300,
	"DEFAULT", 301,
	"DELETE", 302,
	"DESC", 303,
	"DISTINCT", 304,
	"DO", 305,
	"DOMAIN", 306,
	"DROP", 307,
	"ELSE", 308,
	"END", 309,
	"ENTRY_POINT", 310,
	"EQL", 311,
	"ESCAPE", 312,
	"EXCEPTION", 313,
	"EXECUTE", 314,
	"EXISTS", 315,
	"EXIT", 316,
	"EXTERNAL", 317,
	"FILTER", 318,
	"FOR", 319,
	"FOREIGN", 320,
	"FROM", 321,
	"FULL", 322,
	"FUNCTION", 323,
	"GDSCODE", 324,
	"GEQ", 325,
	"GENERATOR", 326,
	"GEN_ID", 327,
	"GRANT", 328,
	"GROUP", 329,
	"GROUP_COMMIT_WAIT", 330,
	"GTR", 331,
	"HAVING", 332,
	"IF", 333,
	"IN", 334,
	"INACTIVE", 335,
	"INNER", 336,
	"INPUT_TYPE", 337,
	"INDEX", 338,
	"INSERT", 339,
	"INTEGER", 340,
	"INTO", 341,
	"IS", 342,
	"ISOLATION", 343,
	"JOIN", 344,
	"KEY", 345,
	"KW_CHAR", 346,
	"KW_DEC", 347,
	"KW_DOUBLE", 348,
	"KW_FILE", 349,
	"KW_FLOAT", 350,
	"KW_INT", 351,
	"KW_LONG", 352,
	"KW_NULL", 353,
	"KW_NUMERIC", 354,
	"KW_UPPER", 355,
	"KW_VALUE", 356,
	"LENGTH", 357,
	"LOGFILE", 358,
	"LPAREN", 359,
	"LEFT", 360,
	"LEQ", 361,
	"LEVEL", 362,
	"LIKE", 363,
	"LOG_BUF_SIZE", 364,
	"LSS", 365,
	"MANUAL", 366,
	"MAXIMUM", 367,
	"MAX_SEGMENT", 368,
	"MERGE", 369,
	"MESSAGE", 370,
	"MINIMUM", 371,
	"MODULE_NAME", 372,
	"NAMES", 373,
	"NATIONAL", 374,
	"NATURAL", 375,
	"NCHAR", 376,
	"NEQ", 377,
	"NO", 378,
	"NOT", 379,
	"NOT_GTR", 380,
	"NOT_LSS", 381,
	"NUM_LOG_BUFS", 382,
	"OF", 383,
	"ON", 384,
	"ONLY", 385,
	"OPTION", 386,
	"OR", 387,
	"ORDER", 388,
	"OUTER", 389,
	"OUTPUT_TYPE", 390,
	"OVERFLOW", 391,
	"PAGE", 392,
	"PAGES", 393,
	"PAGE_SIZE", 394,
	"PARAMETER", 395,
	"PASSWORD", 396,
	"PLAN", 397,
	"POSITION", 398,
	"POST_EVENT", 399,
	"PRECISION", 400,
	"PRIMARY", 401,
	"PRIVILEGES", 402,
	"PROCEDURE", 403,
	"PROTECTED", 404,
	"RAW_PARTITIONS", 405,
	"READ", 406,
	"REAL", 407,
	"REFERENCES", 408,
	"RESERVING", 409,
	"RETAIN", 410,
	"RETURNING_VALUES", 411,
	"RETURNS", 412,
	"REVOKE", 413,
	"RIGHT", 414,
	"RPAREN", 415,
	"ROLLBACK", 416,
	"SEGMENT", 417,
	"SELECT", 418,
	"SET", 419,
	"SHADOW", 420,
	"SHARED", 421,
	"SINGULAR", 422,
	"SIZE", 423,
	"SMALLINT", 424,
	"SNAPSHOT", 425,
	"SOME", 426,
	"SORT", 427,
	"SQLCODE", 428,
	"STABILITY", 429,
	"STARTING", 430,
	"STATISTICS", 431,
	"SUB_TYPE", 432,
	"SUSPEND", 433,
	"SUM", 434,
	"TABLE", 435,
	"THEN", 436,
	"TO", 437,
	"TRANSACTION", 438,
	"TRIGGER", 439,
	"UNCOMMITTED", 440,
	"UNION", 441,
	"UNIQUE", 442,
	"UPDATE", 443,
	"USER", 444,
	"VALUES", 445,
	"VARCHAR", 446,
	"VARIABLE", 447,
	"VARYING", 448,
	"VERSION", 449,
	"VIEW", 450,
	"WAIT", 451,
	"WHEN", 452,
	"WHERE", 453,
	"WHILE", 454,
	"WITH", 455,
	"WORK", 456,
	"WRITE", 457,
	"FLOAT", 458,
	"NUMBER", 459,
	"NUMERIC", 460,
	"SYMBOL", 461,
	"STRING", 462,
	"INTRODUCER", 463,
	"ACTION", 464,
	"ADMIN", 465,
	"CASCADE", 466,
	"FREE_IT", 467,
	"RESTRICT", 468,
	"ROLE", 469,
	"COLUMN", 470,
	"TYPE", 471,
	"EXTRACT", 472,
	"YEAR", 473,
	"MONTH", 474,
	"DAY", 475,
	"HOUR", 476,
	"MINUTE", 477,
	"SECOND", 478,
	"WEEKDAY", 479,
	"YEARDAY", 480,
	"TIME", 481,
	"TIMESTAMP", 482,
	"CURRENT_DATE", 483,
	"CURRENT_TIME", 484,
	"CURRENT_TIMESTAMP", 485,
	"NUMBER64BIT", 486,
	"SCALEDINT", 487,
	"=", 61,
	"<", 60,
	">", 62,
	"+", 43,
	"-", 45,
	"*", 42,
	"/", 47,
	"-unknown-", -1				/* ends search */
};

static char *yyreds[] = {
	"-no such reduction-",
	"top : statement",
	"top : statement ';'",
	"statement : alter",
	"statement : blob",
	"statement : commit",
	"statement : create",
	"statement : declare",
	"statement : delete",
	"statement : drop",
	"statement : grant",
	"statement : insert",
	"statement : invoke_procedure",
	"statement : revoke",
	"statement : rollback",
	"statement : select",
	"statement : set",
	"statement : update",
	"statement : DEBUG signed_short_integer",
	"grant : GRANT privileges ON prot_table_name TO user_grantee_list grant_option",
	"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO user_grantee_list grant_option",
	"grant : GRANT privileges ON prot_table_name TO grantee_list",
	"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO grantee_list",
	"grant : GRANT role_name_list TO role_grantee_list role_admin_option",
	"prot_table_name : simple_table_name",
	"prot_table_name : TABLE simple_table_name",
	"privileges : ALL",
	"privileges : ALL PRIVILEGES",
	"privileges : privilege_list",
	"privilege_list : privilege",
	"privilege_list : privilege_list ',' privilege",
	"proc_privileges : EXECUTE",
	"privilege : SELECT",
	"privilege : INSERT",
	"privilege : DELETE",
	"privilege : UPDATE column_parens_opt",
	"privilege : REFERENCES column_parens_opt",
	"grant_option : WITH GRANT OPTION",
	"grant_option : /* empty */",
	"role_admin_option : WITH ADMIN OPTION",
	"role_admin_option : /* empty */",
	"simple_proc_name : symbol_procedure_name",
	"revoke : REVOKE rev_grant_option privileges ON prot_table_name FROM user_grantee_list",
	"revoke : REVOKE rev_grant_option proc_privileges ON PROCEDURE simple_proc_name FROM user_grantee_list",
	"revoke : REVOKE privileges ON prot_table_name FROM user_grantee_list",
	"revoke : REVOKE proc_privileges ON PROCEDURE simple_proc_name FROM user_grantee_list",
	"revoke : REVOKE privileges ON prot_table_name FROM grantee_list",
	"revoke : REVOKE proc_privileges ON PROCEDURE simple_proc_name FROM grantee_list",
	"revoke : REVOKE role_name_list FROM role_grantee_list",
	"rev_grant_option : GRANT OPTION FOR",
	"grantee_list : grantee",
	"grantee_list : grantee_list ',' grantee",
	"grantee_list : grantee_list ',' user_grantee",
	"grantee_list : user_grantee_list ',' grantee",
	"grantee : PROCEDURE symbol_procedure_name",
	"grantee : TRIGGER symbol_trigger_name",
	"grantee : VIEW symbol_view_name",
	"user_grantee_list : user_grantee",
	"user_grantee_list : user_grantee_list ',' user_grantee",
	"user_grantee : symbol_user_name",
	"user_grantee : USER symbol_user_name",
	"user_grantee : GROUP symbol_user_name",
	"role_name_list : role_name",
	"role_name_list : role_name_list ',' role_name",
	"role_name : symbol_role_name",
	"role_grantee_list : role_grantee",
	"role_grantee_list : role_grantee_list ',' role_grantee",
	"role_grantee : symbol_user_name",
	"role_grantee : USER symbol_user_name",
	"declare : DECLARE declare_clause",
	"declare_clause : FILTER filter_decl_clause",
	"declare_clause : EXTERNAL FUNCTION udf_decl_clause",
	"udf_decl_clause : symbol_UDF_name arg_desc_list1 RETURNS return_value1 ENTRY_POINT sql_string MODULE_NAME sql_string",
	"udf_data_type : simple_type",
	"udf_data_type : BLOB",
	"udf_data_type : CSTRING '(' pos_short_integer ')' charset_clause",
	"arg_desc_list1 : /* empty */",
	"arg_desc_list1 : arg_desc_list",
	"arg_desc_list1 : '(' arg_desc_list ')'",
	"arg_desc_list : arg_desc",
	"arg_desc_list : arg_desc_list ',' arg_desc",
	"arg_desc : init_data_type udf_data_type",
	"return_value1 : return_value",
	"return_value1 : '(' return_value ')'",
	"return_value : init_data_type udf_data_type",
	"return_value : init_data_type udf_data_type FREE_IT",
	"return_value : init_data_type udf_data_type BY KW_VALUE",
	"return_value : PARAMETER pos_short_integer",
	"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_subtype OUTPUT_TYPE blob_subtype ENTRY_POINT sql_string MODULE_NAME sql_string",
	"create : CREATE create_clause",
	"create_clause : EXCEPTION symbol_exception_name sql_string",
	"create_clause : unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition",
	"create_clause : PROCEDURE procedure_clause",
	"create_clause : TABLE table_clause",
	"create_clause : TRIGGER def_trigger_clause",
	"create_clause : VIEW view_clause",
	"create_clause : GENERATOR generator_clause",
	"create_clause : DATABASE db_clause",
	"create_clause : DOMAIN domain_clause",
	"create_clause : SHADOW shadow_clause",
	"create_clause : ROLE role_clause",
	"unique_opt : UNIQUE",
	"unique_opt : /* empty */",
	"index_definition : column_list",
	"index_definition : column_parens",
	"index_definition : computed_by '(' begin_trigger value end_trigger ')'",
	"shadow_clause : pos_short_integer manual_auto conditional sql_string first_file_length sec_shadow_files",
	"manual_auto : MANUAL",
	"manual_auto : AUTO",
	"manual_auto : /* empty */",
	"conditional : /* empty */",
	"conditional : CONDITIONAL",
	"first_file_length : /* empty */",
	"first_file_length : LENGTH equals long_integer page_noise",
	"sec_shadow_files : /* empty */",
	"sec_shadow_files : db_file_list",
	"db_file_list : db_file",
	"db_file_list : db_file_list db_file",
	"domain_clause : column_def_name as_opt data_type begin_trigger domain_default_opt end_trigger domain_constraint_clause collate_clause",
	"as_opt : AS",
	"as_opt : /* empty */",
	"domain_default_opt : DEFAULT begin_trigger default_value",
	"domain_default_opt : /* empty */",
	"domain_constraint_clause : /* empty */",
	"domain_constraint_clause : domain_constraint_list",
	"domain_constraint_list : domain_constraint_def",
	"domain_constraint_list : domain_constraint_list domain_constraint_def",
	"domain_constraint_def : domain_constraint",
	"domain_constraint : null_constraint",
	"domain_constraint : domain_check_constraint",
	"null_constraint : NOT KW_NULL",
	"domain_check_constraint : begin_trigger CHECK '(' search_condition ')' end_trigger",
	"generator_clause : symbol_generator_name",
	"role_clause : symbol_role_name",
	"db_clause : db_name db_initial_desc1 db_rem_desc1",
	"equals : /* empty */",
	"equals : '='",
	"db_name : sql_string",
	"db_initial_desc1 : /* empty */",
	"db_initial_desc1 : db_initial_desc",
	"db_initial_desc : db_initial_option",
	"db_initial_desc : db_initial_desc db_initial_option",
	"db_initial_option : PAGE_SIZE equals pos_short_integer",
	"db_initial_option : LENGTH equals long_integer page_noise",
	"db_initial_option : USER sql_string",
	"db_initial_option : PASSWORD sql_string",
	"db_initial_option : SET NAMES sql_string",
	"db_rem_desc1 : /* empty */",
	"db_rem_desc1 : db_rem_desc",
	"db_rem_desc : db_rem_option",
	"db_rem_desc : db_rem_desc db_rem_option",
	"db_rem_option : db_file",
	"db_rem_option : db_log",
	"db_rem_option : db_log_option",
	"db_rem_option : DEFAULT CHARACTER SET symbol_character_set_name",
	"db_log_option : GROUP_COMMIT_WAIT equals long_integer",
	"db_log_option : CHECK_POINT_LEN equals long_integer",
	"db_log_option : NUM_LOG_BUFS equals pos_short_integer",
	"db_log_option : LOG_BUF_SIZE equals unsigned_short_integer",
	"db_log : db_default_log_spec",
	"db_log : db_rem_log_spec",
	"db_rem_log_spec : LOGFILE '(' logfiles ')' OVERFLOW logfile_desc",
	"db_rem_log_spec : LOGFILE BASENAME logfile_desc",
	"db_default_log_spec : LOGFILE",
	"db_file : file1 sql_string file_desc1",
	"logfiles : logfile_desc",
	"logfiles : logfiles ',' logfile_desc",
	"logfile_desc : logfile_name logfile_attrs",
	"logfile_name : sql_string",
	"logfile_attrs : /* empty */",
	"logfile_attrs : logfile_attrs logfile_attr",
	"logfile_attr : SIZE equals long_integer",
	"file1 : KW_FILE",
	"file_desc1 : /* empty */",
	"file_desc1 : file_desc",
	"file_desc : file_clause",
	"file_desc : file_desc file_clause",
	"file_clause : STARTING file_clause_noise long_integer",
	"file_clause : LENGTH equals long_integer page_noise",
	"file_clause_noise : /* empty */",
	"file_clause_noise : AT",
	"file_clause_noise : AT PAGE",
	"page_noise : /* empty */",
	"page_noise : PAGE",
	"page_noise : PAGES",
	"table_clause : simple_table_name external_file '(' table_elements ')'",
	"external_file : EXTERNAL KW_FILE sql_string",
	"external_file : EXTERNAL sql_string",
	"external_file : /* empty */",
	"table_elements : table_element",
	"table_elements : table_elements ',' table_element",
	"table_element : column_def",
	"table_element : table_constraint_definition",
	"column_def : column_def_name data_type_or_domain default_opt end_trigger column_constraint_clause collate_clause",
	"column_def : column_def_name non_array_type def_computed",
	"column_def : column_def_name def_computed",
	"def_computed : computed_by '(' begin_trigger value end_trigger ')'",
	"computed_by : COMPUTED BY",
	"computed_by : COMPUTED",
	"data_type_or_domain : data_type begin_trigger",
	"data_type_or_domain : simple_column_name begin_string",
	"collate_clause : COLLATE symbol_collation_name",
	"collate_clause : /* empty */",
	"column_def_name : column_name",
	"simple_column_def_name : simple_column_name",
	"data_type_descriptor : init_data_type data_type",
	"init_data_type : /* empty */",
	"default_opt : DEFAULT default_value",
	"default_opt : /* empty */",
	"default_value : constant",
	"default_value : USER",
	"default_value : null_value",
	"default_value : datetime_value_expression",
	"column_constraint_clause : /* empty */",
	"column_constraint_clause : column_constraint_list",
	"column_constraint_list : column_constraint_def",
	"column_constraint_list : column_constraint_list column_constraint_def",
	"column_constraint_def : constraint_name_opt column_constraint",
	"column_constraint : NOT KW_NULL",
	"column_constraint : REFERENCES simple_table_name column_parens_opt referential_trigger_action",
	"column_constraint : check_constraint",
	"column_constraint : UNIQUE",
	"column_constraint : PRIMARY KEY",
	"table_constraint_definition : constraint_name_opt table_constraint",
	"constraint_name_opt : CONSTRAINT symbol_constraint_name",
	"constraint_name_opt : /* empty */",
	"table_constraint : unique_constraint",
	"table_constraint : primary_constraint",
	"table_constraint : referential_constraint",
	"table_constraint : check_constraint",
	"unique_constraint : UNIQUE column_parens",
	"primary_constraint : PRIMARY KEY column_parens",
	"referential_constraint : FOREIGN KEY column_parens REFERENCES simple_table_name column_parens_opt referential_trigger_action",
	"check_constraint : begin_trigger CHECK '(' search_condition ')' end_trigger",
	"referential_trigger_action : update_rule",
	"referential_trigger_action : delete_rule",
	"referential_trigger_action : delete_rule update_rule",
	"referential_trigger_action : update_rule delete_rule",
	"referential_trigger_action : /* empty */",
	"update_rule : ON UPDATE referential_action",
	"delete_rule : ON DELETE referential_action",
	"referential_action : CASCADE",
	"referential_action : SET DEFAULT",
	"referential_action : SET KW_NULL",
	"referential_action : NO ACTION",
	"procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
	"alter_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
	"input_parameters : '(' proc_parameters ')'",
	"input_parameters : /* empty */",
	"output_parameters : RETURNS input_parameters",
	"output_parameters : /* empty */",
	"proc_parameters : proc_parameter",
	"proc_parameters : proc_parameters ',' proc_parameter",
	"proc_parameter : simple_column_def_name non_array_type",
	"var_declaration_list : var_declarations",
	"var_declaration_list : /* empty */",
	"var_declarations : var_declaration",
	"var_declarations : var_declarations var_declaration",
	"var_declaration : DECLARE VARIABLE column_def_name non_array_type ';'",
	"proc_block : proc_statement",
	"proc_block : full_proc_block",
	"full_proc_block : BEGIN proc_statements END",
	"full_proc_block : BEGIN proc_statements excp_statements END",
	"proc_statements : proc_block",
	"proc_statements : proc_statements proc_block",
	"proc_statement : assignment ';'",
	"proc_statement : delete ';'",
	"proc_statement : EXCEPTION symbol_exception_name ';'",
	"proc_statement : exec_procedure",
	"proc_statement : for_select",
	"proc_statement : if_then_else",
	"proc_statement : insert ';'",
	"proc_statement : POST_EVENT value ';'",
	"proc_statement : singleton_select",
	"proc_statement : update ';'",
	"proc_statement : while",
	"proc_statement : SUSPEND ';'",
	"proc_statement : EXIT ';'",
	"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs ';'",
	"for_select : FOR select INTO variable_list cursor_def DO proc_block",
	"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
	"if_then_else : IF '(' search_condition ')' THEN proc_block",
	"singleton_select : select INTO variable_list ';'",
	"variable : ':' symbol_variable_name",
	"proc_inputs : var_const_list",
	"proc_inputs : '(' var_const_list ')'",
	"proc_inputs : /* empty */",
	"proc_outputs : RETURNING_VALUES variable_list",
	"proc_outputs : RETURNING_VALUES '(' variable_list ')'",
	"proc_outputs : /* empty */",
	"var_const_list : variable",
	"var_const_list : constant",
	"var_const_list : column_name",
	"var_const_list : null_value",
	"var_const_list : var_const_list ',' variable",
	"var_const_list : var_const_list ',' constant",
	"var_const_list : var_const_list ',' column_name",
	"var_const_list : var_const_list ',' null_value",
	"variable_list : variable",
	"variable_list : column_name",
	"variable_list : variable_list ',' column_name",
	"variable_list : variable_list ',' variable",
	"while : WHILE '(' search_condition ')' DO proc_block",
	"cursor_def : AS CURSOR symbol_cursor_name",
	"cursor_def : /* empty */",
	"excp_statements : excp_statement",
	"excp_statements : excp_statements excp_statement",
	"excp_statement : WHEN errors DO proc_block",
	"errors : err",
	"errors : errors ',' err",
	"err : SQLCODE signed_short_integer",
	"err : GDSCODE symbol_gdscode_name",
	"err : EXCEPTION symbol_exception_name",
	"err : ANY",
	"invoke_procedure : EXECUTE PROCEDURE symbol_procedure_name prc_inputs",
	"prc_inputs : prm_const_list",
	"prc_inputs : '(' prm_const_list ')'",
	"prc_inputs : /* empty */",
	"prm_const_list : parameter",
	"prm_const_list : constant",
	"prm_const_list : null_value",
	"prm_const_list : prm_const_list ',' parameter",
	"prm_const_list : prm_const_list ',' constant",
	"prm_const_list : prm_const_list ',' null_value",
	"view_clause : symbol_view_name column_parens_opt AS begin_string union_view check_opt end_string",
	"union_view : union_view_expr",
	"union_view_expr : select_view_expr",
	"union_view_expr : union_view_expr UNION select_view_expr",
	"union_view_expr : union_view_expr UNION ALL select_view_expr",
	"select_view_expr : SELECT distinct_clause select_list from_view_clause where_clause group_clause having_clause plan_clause",
	"from_view_clause : FROM from_view_list",
	"from_view_list : view_table",
	"from_view_list : from_view_list ',' view_table",
	"view_table : joined_view_table",
	"view_table : table_name",
	"joined_view_table : view_table join_type JOIN view_table ON search_condition",
	"joined_view_table : '(' joined_view_table ')'",
	"begin_string : /* empty */",
	"end_string : /* empty */",
	"begin_trigger : /* empty */",
	"end_trigger : /* empty */",
	"check_opt : WITH CHECK OPTION",
	"check_opt : /* empty */",
	"def_trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position begin_trigger trigger_action end_trigger",
	"trigger_active : ACTIVE",
	"trigger_active : INACTIVE",
	"trigger_active : /* empty */",
	"trigger_type : BEFORE INSERT",
	"trigger_type : AFTER INSERT",
	"trigger_type : BEFORE UPDATE",
	"trigger_type : AFTER UPDATE",
	"trigger_type : BEFORE DELETE",
	"trigger_type : AFTER DELETE",
	"trigger_position : POSITION nonneg_short_integer",
	"trigger_position : /* empty */",
	"trigger_action : AS begin_trigger var_declaration_list full_proc_block",
	"alter : ALTER alter_clause",
	"alter_clause : EXCEPTION symbol_exception_name sql_string",
	"alter_clause : TABLE simple_table_name alter_ops",
	"alter_clause : TRIGGER alter_trigger_clause",
	"alter_clause : PROCEDURE alter_procedure_clause",
	"alter_clause : DATABASE init_alter_db alter_db",
	"alter_clause : DOMAIN alter_column_name alter_domain_ops",
	"alter_clause : INDEX alter_index_clause",
	"alter_domain_ops : alter_domain_op",
	"alter_domain_ops : alter_domain_ops alter_domain_op",
	"alter_domain_op : SET begin_string default_opt end_trigger",
	"alter_domain_op : ADD CONSTRAINT domain_check_constraint",
	"alter_domain_op : ADD domain_check_constraint",
	"alter_domain_op : DROP DEFAULT",
	"alter_domain_op : DROP CONSTRAINT",
	"alter_domain_op : TO simple_column_name",
	"alter_domain_op : TYPE init_data_type non_array_type",
	"alter_ops : alter_op",
	"alter_ops : alter_ops ',' alter_op",
	"alter_op : DROP simple_column_name drop_behaviour",
	"alter_op : DROP CONSTRAINT symbol_constraint_name",
	"alter_op : ADD column_def",
	"alter_op : ADD table_constraint_definition",
	"alter_op : col_opt simple_column_name POSITION nonneg_short_integer",
	"alter_op : col_opt alter_column_name TO simple_column_name",
	"alter_op : col_opt alter_col_name TYPE alter_data_type_or_domain end_trigger",
	"alter_column_name : keyword_or_column",
	"keyword_or_column : COLUMN",
	"keyword_or_column : TYPE",
	"keyword_or_column : EXTRACT",
	"keyword_or_column : YEAR",
	"keyword_or_column : MONTH",
	"keyword_or_column : DAY",
	"keyword_or_column : HOUR",
	"keyword_or_column : MINUTE",
	"keyword_or_column : SECOND",
	"keyword_or_column : WEEKDAY",
	"keyword_or_column : YEARDAY",
	"keyword_or_column : TIME",
	"keyword_or_column : TIMESTAMP",
	"keyword_or_column : CURRENT_DATE",
	"keyword_or_column : CURRENT_TIME",
	"keyword_or_column : CURRENT_TIMESTAMP",
	"keyword_or_column : SYMBOL",
	"col_opt : ALTER",
	"col_opt : ALTER COLUMN",
	"alter_data_type_or_domain : non_array_type begin_trigger",
	"alter_data_type_or_domain : simple_column_name begin_string",
	"alter_col_name : simple_column_name",
	"drop_behaviour : RESTRICT",
	"drop_behaviour : CASCADE",
	"drop_behaviour : /* empty */",
	"alter_index_clause : symbol_index_name ACTIVE",
	"alter_index_clause : symbol_index_name INACTIVE",
	"init_alter_db : /* empty */",
	"alter_db : db_alter_clause",
	"alter_db : alter_db db_alter_clause",
	"db_alter_clause : ADD db_file_list",
	"db_alter_clause : DROP LOGFILE",
	"db_alter_clause : SET db_log_option_list",
	"db_alter_clause : ADD db_log",
	"db_log_option_list : db_log_option",
	"db_log_option_list : db_log_option_list ',' db_log_option",
	"alter_trigger_clause : symbol_trigger_name trigger_active new_trigger_type trigger_position begin_trigger new_trigger_action end_trigger",
	"new_trigger_type : trigger_type",
	"new_trigger_type : /* empty */",
	"new_trigger_action : trigger_action",
	"new_trigger_action : /* empty */",
	"drop : DROP drop_clause",
	"drop_clause : EXCEPTION symbol_exception_name",
	"drop_clause : INDEX symbol_index_name",
	"drop_clause : PROCEDURE symbol_procedure_name",
	"drop_clause : TABLE symbol_table_name",
	"drop_clause : TRIGGER symbol_trigger_name",
	"drop_clause : VIEW symbol_view_name",
	"drop_clause : FILTER symbol_filter_name",
	"drop_clause : DOMAIN symbol_domain_name",
	"drop_clause : EXTERNAL FUNCTION symbol_UDF_name",
	"drop_clause : SHADOW pos_short_integer",
	"drop_clause : ROLE symbol_role_name",
	"data_type : non_array_type",
	"data_type : array_type",
	"non_array_type : simple_type",
	"non_array_type : blob_type",
	"array_type : non_charset_simple_type '[' array_spec ']'",
	"array_type : character_type '[' array_spec ']' charset_clause",
	"array_spec : array_range",
	"array_spec : array_spec ',' array_range",
	"array_range : signed_long_integer",
	"array_range : signed_long_integer ':' signed_long_integer",
	"simple_type : non_charset_simple_type",
	"simple_type : character_type charset_clause",
	"non_charset_simple_type : national_character_type",
	"non_charset_simple_type : numeric_type",
	"non_charset_simple_type : float_type",
	"non_charset_simple_type : integer_keyword",
	"non_charset_simple_type : SMALLINT",
	"non_charset_simple_type : DATE",
	"non_charset_simple_type : TIME",
	"non_charset_simple_type : TIMESTAMP",
	"integer_keyword : INTEGER",
	"integer_keyword : KW_INT",
	"blob_type : BLOB blob_subtype blob_segsize charset_clause",
	"blob_type : BLOB '(' unsigned_short_integer ')'",
	"blob_type : BLOB '(' unsigned_short_integer ',' signed_short_integer ')'",
	"blob_type : BLOB '(' ',' signed_short_integer ')'",
	"blob_segsize : SEGMENT SIZE unsigned_short_integer",
	"blob_segsize : /* empty */",
	"blob_subtype : SUB_TYPE signed_short_integer",
	"blob_subtype : SUB_TYPE symbol_blob_subtype_name",
	"blob_subtype : /* empty */",
	"charset_clause : CHARACTER SET symbol_character_set_name",
	"charset_clause : /* empty */",
	"national_character_type : national_character_keyword '(' pos_short_integer ')'",
	"national_character_type : national_character_keyword",
	"national_character_type : national_character_keyword VARYING '(' pos_short_integer ')'",
	"character_type : character_keyword '(' pos_short_integer ')'",
	"character_type : character_keyword",
	"character_type : varying_keyword '(' pos_short_integer ')'",
	"varying_keyword : VARCHAR",
	"varying_keyword : CHARACTER VARYING",
	"varying_keyword : KW_CHAR VARYING",
	"character_keyword : CHARACTER",
	"character_keyword : KW_CHAR",
	"national_character_keyword : NCHAR",
	"national_character_keyword : NATIONAL CHARACTER",
	"national_character_keyword : NATIONAL KW_CHAR",
	"numeric_type : KW_NUMERIC prec_scale",
	"numeric_type : decimal_keyword prec_scale",
	"ordinal : pos_short_integer",
	"prec_scale : /* empty */",
	"prec_scale : '(' signed_long_integer ')'",
	"prec_scale : '(' signed_long_integer ',' signed_long_integer ')'",
	"decimal_keyword : DECIMAL",
	"decimal_keyword : KW_DEC",
	"float_type : KW_FLOAT precision_opt",
	"float_type : KW_LONG KW_FLOAT precision_opt",
	"float_type : REAL",
	"float_type : KW_DOUBLE PRECISION",
	"precision_opt : '(' nonneg_short_integer ')'",
	"precision_opt : /* empty */",
	"set : set_transaction",
	"set : set_generator",
	"set : set_statistics",
	"set_generator : SET GENERATOR symbol_generator_name TO signed_long_integer",
	"set_generator : SET GENERATOR symbol_generator_name TO NUMBER64BIT",
	"set_generator : SET GENERATOR symbol_generator_name TO '-' NUMBER64BIT",
	"commit : COMMIT optional_work optional_retain",
	"rollback : ROLLBACK optional_work",
	"optional_work : WORK",
	"optional_work : /* empty */",
	"optional_retain : RETAIN opt_snapshot",
	"optional_retain : /* empty */",
	"opt_snapshot : SNAPSHOT",
	"opt_snapshot : /* empty */",
	"set_transaction : SET TRANSACTION tran_opt_list_m",
	"tran_opt_list_m : tran_opt_list",
	"tran_opt_list_m : /* empty */",
	"tran_opt_list : tran_opt",
	"tran_opt_list : tran_opt_list tran_opt",
	"tran_opt : access_mode",
	"tran_opt : lock_wait",
	"tran_opt : isolation_mode",
	"tran_opt : tbl_reserve_options",
	"access_mode : READ ONLY",
	"access_mode : READ WRITE",
	"lock_wait : WAIT",
	"lock_wait : NO WAIT",
	"isolation_mode : ISOLATION LEVEL iso_mode",
	"isolation_mode : iso_mode",
	"iso_mode : snap_shot",
	"iso_mode : READ UNCOMMITTED version_mode",
	"iso_mode : READ COMMITTED version_mode",
	"snap_shot : SNAPSHOT",
	"snap_shot : SNAPSHOT TABLE",
	"snap_shot : SNAPSHOT TABLE STABILITY",
	"version_mode : VERSION",
	"version_mode : NO VERSION",
	"version_mode : /* empty */",
	"tbl_reserve_options : RESERVING restr_list",
	"lock_type : SHARED",
	"lock_type : PROTECTED",
	"lock_type : /* empty */",
	"lock_mode : READ",
	"lock_mode : WRITE",
	"restr_list : restr_option",
	"restr_list : restr_list ',' restr_option",
	"restr_option : table_list table_lock",
	"table_lock : FOR lock_type lock_mode",
	"table_lock : /* empty */",
	"table_list : simple_table_name",
	"table_list : table_list ',' simple_table_name",
	"set_statistics : SET STATISTICS INDEX symbol_index_name",
	"select : union_expr order_clause for_update_clause",
	"union_expr : select_expr",
	"union_expr : union_expr UNION select_expr",
	"union_expr : union_expr UNION ALL select_expr",
	"order_clause : ORDER BY order_list",
	"order_clause : /* empty */",
	"order_list : order_item",
	"order_list : order_list ',' order_item",
	"order_item : column_name collate_clause order_direction",
	"order_item : ordinal collate_clause order_direction",
	"order_direction : ASC",
	"order_direction : DESC",
	"order_direction : /* empty */",
	"for_update_clause : FOR UPDATE for_update_list",
	"for_update_clause : /* empty */",
	"for_update_list : OF column_list",
	"for_update_list : /* empty */",
	"select_expr : SELECT distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
	"distinct_clause : DISTINCT",
	"distinct_clause : all_noise",
	"select_list : select_items",
	"select_list : '*'",
	"select_items : select_item",
	"select_items : select_items ',' select_item",
	"select_item : rhs",
	"select_item : rhs symbol_item_alias_name",
	"select_item : rhs AS symbol_item_alias_name",
	"from_clause : FROM from_list",
	"from_list : table_reference",
	"from_list : from_list ',' table_reference",
	"table_reference : joined_table",
	"table_reference : table_proc",
	"joined_table : table_reference join_type JOIN table_reference ON search_condition",
	"joined_table : '(' joined_table ')'",
	"table_proc : symbol_procedure_name proc_table_inputs symbol_table_alias_name",
	"table_proc : symbol_procedure_name proc_table_inputs",
	"proc_table_inputs : '(' null_or_value_list ')'",
	"proc_table_inputs : /* empty */",
	"null_or_value_list : null_or_value",
	"null_or_value_list : null_or_value_list ',' null_or_value",
	"null_or_value : null_value",
	"null_or_value : value",
	"table_name : simple_table_name",
	"table_name : symbol_table_name symbol_table_alias_name",
	"simple_table_name : symbol_table_name",
	"join_type : INNER",
	"join_type : LEFT",
	"join_type : LEFT OUTER",
	"join_type : RIGHT",
	"join_type : RIGHT OUTER",
	"join_type : FULL",
	"join_type : FULL OUTER",
	"join_type : /* empty */",
	"group_clause : GROUP BY grp_column_list",
	"group_clause : /* empty */",
	"grp_column_list : grp_column_elem",
	"grp_column_list : grp_column_list ',' grp_column_elem",
	"grp_column_elem : column_name",
	"grp_column_elem : column_name COLLATE symbol_collation_name",
	"having_clause : HAVING search_condition",
	"having_clause : /* empty */",
	"where_clause : WHERE search_condition",
	"where_clause : /* empty */",
	"plan_clause : PLAN plan_expression",
	"plan_clause : /* empty */",
	"plan_expression : plan_type '(' plan_item_list ')'",
	"plan_type : JOIN",
	"plan_type : SORT MERGE",
	"plan_type : MERGE",
	"plan_type : SORT",
	"plan_type : /* empty */",
	"plan_item_list : plan_item",
	"plan_item_list : plan_item ',' plan_item_list",
	"plan_item : table_or_alias_list access_type",
	"plan_item : plan_expression",
	"table_or_alias_list : symbol_table_name",
	"table_or_alias_list : symbol_table_name table_or_alias_list",
	"access_type : NATURAL",
	"access_type : INDEX '(' index_list ')'",
	"access_type : ORDER symbol_index_name",
	"index_list : symbol_index_name",
	"index_list : symbol_index_name ',' index_list",
	"insert : INSERT INTO simple_table_name column_parens_opt VALUES '(' insert_value_list ')'",
	"insert : INSERT INTO simple_table_name column_parens_opt select_expr",
	"insert_value_list : rhs",
	"insert_value_list : insert_value_list ',' rhs",
	"delete : delete_searched",
	"delete : delete_positioned",
	"delete_searched : DELETE FROM table_name where_clause",
	"delete_positioned : DELETE FROM table_name cursor_clause",
	"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
	"update : update_searched",
	"update : update_positioned",
	"update_searched : UPDATE table_name SET assignments where_clause",
	"update_positioned : UPDATE table_name SET assignments cursor_clause",
	"assignments : assignment",
	"assignments : assignments ',' assignment",
	"assignment : column_name '=' rhs",
	"rhs : value",
	"rhs : null_value",
	"blob : READ BLOB simple_column_name FROM simple_table_name filter_clause segment_clause",
	"blob : INSERT BLOB simple_column_name INTO simple_table_name filter_clause segment_clause",
	"filter_clause : FILTER FROM blob_subtype_value TO blob_subtype_value",
	"filter_clause : FILTER TO blob_subtype_value",
	"filter_clause : /* empty */",
	"blob_subtype_value : blob_subtype",
	"blob_subtype_value : parameter",
	"blob_subtype : signed_short_integer",
	"segment_clause : MAX_SEGMENT segment_length",
	"segment_clause : /* empty */",
	"segment_length : unsigned_short_integer",
	"segment_length : parameter",
	"column_parens_opt : column_parens",
	"column_parens_opt : /* empty */",
	"column_parens : '(' column_list ')'",
	"column_list : column_name",
	"column_list : column_list ',' column_name",
	"column_name : simple_column_name",
	"column_name : symbol_table_alias_name '.' symbol_column_name",
	"column_name : symbol_table_alias_name '.' '*'",
	"simple_column_name : symbol_column_name",
	"search_condition : predicate",
	"search_condition : search_condition OR search_condition",
	"search_condition : search_condition AND search_condition",
	"search_condition : NOT search_condition",
	"predicate : comparison_predicate",
	"predicate : between_predicate",
	"predicate : like_predicate",
	"predicate : in_predicate",
	"predicate : null_predicate",
	"predicate : quantified_predicate",
	"predicate : exists_predicate",
	"predicate : containing_predicate",
	"predicate : starting_predicate",
	"predicate : unique_predicate",
	"predicate : '(' search_condition ')'",
	"comparison_predicate : value '=' value",
	"comparison_predicate : value '<' value",
	"comparison_predicate : value '>' value",
	"comparison_predicate : value GEQ value",
	"comparison_predicate : value LEQ value",
	"comparison_predicate : value NOT_GTR value",
	"comparison_predicate : value NOT_LSS value",
	"comparison_predicate : value NEQ value",
	"quantified_predicate : value '=' ALL '(' column_select ')'",
	"quantified_predicate : value '<' ALL '(' column_select ')'",
	"quantified_predicate : value '>' ALL '(' column_select ')'",
	"quantified_predicate : value GEQ ALL '(' column_select ')'",
	"quantified_predicate : value LEQ ALL '(' column_select ')'",
	"quantified_predicate : value NOT_GTR ALL '(' column_select ')'",
	"quantified_predicate : value NOT_LSS ALL '(' column_select ')'",
	"quantified_predicate : value NEQ ALL '(' column_select ')'",
	"quantified_predicate : value '=' some '(' column_select ')'",
	"quantified_predicate : value '<' some '(' column_select ')'",
	"quantified_predicate : value '>' some '(' column_select ')'",
	"quantified_predicate : value GEQ some '(' column_select ')'",
	"quantified_predicate : value LEQ some '(' column_select ')'",
	"quantified_predicate : value NOT_GTR some '(' column_select ')'",
	"quantified_predicate : value NOT_LSS some '(' column_select ')'",
	"quantified_predicate : value NEQ some '(' column_select ')'",
	"some : SOME",
	"some : ANY",
	"between_predicate : value BETWEEN value AND value",
	"between_predicate : value NOT BETWEEN value AND value",
	"like_predicate : value LIKE value",
	"like_predicate : value NOT LIKE value",
	"like_predicate : value LIKE value ESCAPE value",
	"like_predicate : value NOT LIKE value ESCAPE value",
	"in_predicate : value IN scalar_set",
	"in_predicate : value NOT IN scalar_set",
	"containing_predicate : value CONTAINING value",
	"containing_predicate : value NOT CONTAINING value",
	"starting_predicate : value STARTING value",
	"starting_predicate : value NOT STARTING value",
	"starting_predicate : value STARTING WITH value",
	"starting_predicate : value NOT STARTING WITH value",
	"exists_predicate : EXISTS '(' select_expr ')'",
	"unique_predicate : SINGULAR '(' select_expr ')'",
	"null_predicate : value IS KW_NULL",
	"null_predicate : value IS NOT KW_NULL",
	"scalar_set : '(' constant_list ')'",
	"scalar_set : '(' column_select ')'",
	"column_select : SELECT distinct_clause value from_clause where_clause group_clause having_clause plan_clause",
	"column_singleton : SELECT distinct_clause value from_clause where_clause group_clause having_clause plan_clause",
	"value : column_name",
	"value : array_element",
	"value : function",
	"value : u_constant",
	"value : parameter",
	"value : variable",
	"value : udf",
	"value : '-' value",
	"value : '+' value",
	"value : value '+' value",
	"value : value CONCATENATE value",
	"value : value COLLATE symbol_collation_name",
	"value : value '-' value",
	"value : value '*' value",
	"value : value '/' value",
	"value : '(' value ')'",
	"value : '(' column_singleton ')'",
	"value : USER",
	"value : DB_KEY",
	"value : symbol_table_alias_name '.' DB_KEY",
	"value : KW_VALUE",
	"value : datetime_value_expression",
	"datetime_value_expression : CURRENT_DATE",
	"datetime_value_expression : CURRENT_TIME",
	"datetime_value_expression : CURRENT_TIMESTAMP",
	"array_element : column_name '[' value_list ']'",
	"value_list : value",
	"value_list : value_list ',' value",
	"constant : u_constant",
	"constant : '-' u_numeric_constant",
	"u_numeric_constant : NUMERIC",
	"u_numeric_constant : NUMBER",
	"u_numeric_constant : FLOAT",
	"u_numeric_constant : NUMBER64BIT",
	"u_numeric_constant : SCALEDINT",
	"u_constant : u_numeric_constant",
	"u_constant : sql_string",
	"u_constant : DATE STRING",
	"u_constant : TIME STRING",
	"u_constant : TIMESTAMP STRING",
	"constant_list : constant",
	"constant_list : parameter",
	"constant_list : current_user",
	"constant_list : constant_list ',' constant",
	"constant_list : constant_list ',' parameter",
	"constant_list : constant_list ',' current_user",
	"parameter : '?'",
	"current_user : USER",
	"sql_string : STRING",
	"sql_string : INTRODUCER STRING",
	"signed_short_integer : nonneg_short_integer",
	"signed_short_integer : '-' neg_short_integer",
	"nonneg_short_integer : NUMBER",
	"neg_short_integer : NUMBER",
	"pos_short_integer : nonneg_short_integer",
	"unsigned_short_integer : NUMBER",
	"signed_long_integer : long_integer",
	"signed_long_integer : '-' long_integer",
	"long_integer : NUMBER",
	"function : COUNT '(' '*' ')'",
	"function : COUNT '(' all_noise value ')'",
	"function : COUNT '(' DISTINCT value ')'",
	"function : SUM '(' all_noise value ')'",
	"function : SUM '(' DISTINCT value ')'",
	"function : AVG '(' all_noise value ')'",
	"function : AVG '(' DISTINCT value ')'",
	"function : MINIMUM '(' all_noise value ')'",
	"function : MINIMUM '(' DISTINCT value ')'",
	"function : MAXIMUM '(' all_noise value ')'",
	"function : MAXIMUM '(' DISTINCT value ')'",
	"function : CAST '(' rhs AS data_type_descriptor ')'",
	"function : KW_UPPER '(' value ')'",
	"function : GEN_ID '(' symbol_generator_name ',' value ')'",
	"function : EXTRACT '(' timestamp_part FROM value ')'",
	"udf : symbol_UDF_name '(' value_list ')'",
	"udf : symbol_UDF_name '(' ')'",
	"timestamp_part : YEAR",
	"timestamp_part : MONTH",
	"timestamp_part : DAY",
	"timestamp_part : HOUR",
	"timestamp_part : MINUTE",
	"timestamp_part : SECOND",
	"timestamp_part : WEEKDAY",
	"timestamp_part : YEARDAY",
	"all_noise : ALL",
	"all_noise : /* empty */",
	"null_value : KW_NULL",
	"symbol_UDF_name : SYMBOL",
	"symbol_blob_subtype_name : SYMBOL",
	"symbol_character_set_name : SYMBOL",
	"symbol_collation_name : SYMBOL",
	"symbol_column_name : SYMBOL",
	"symbol_constraint_name : SYMBOL",
	"symbol_cursor_name : SYMBOL",
	"symbol_domain_name : SYMBOL",
	"symbol_exception_name : SYMBOL",
	"symbol_filter_name : SYMBOL",
	"symbol_gdscode_name : SYMBOL",
	"symbol_generator_name : SYMBOL",
	"symbol_index_name : SYMBOL",
	"symbol_item_alias_name : SYMBOL",
	"symbol_procedure_name : SYMBOL",
	"symbol_role_name : SYMBOL",
	"symbol_table_alias_name : SYMBOL",
	"symbol_table_name : SYMBOL",
	"symbol_trigger_name : SYMBOL",
	"symbol_user_name : SYMBOL",
	"symbol_variable_name : SYMBOL",
	"symbol_view_name : SYMBOL",
};
#endif /* YYDEBUG */
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.14	97/01/16 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( DSQL_yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	DSQL_yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!DSQL_yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1			/* make debugging available */
#endif

/*
** user known globals
*/
static int yydebug;				/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;				/* top of state stack */

static int yystate;				/* current state */
static int yytmp;				/* extra var (lasts between blocks) */

static int yynerrs;				/* number of errors */
#ifndef SHLIB_DEFS
int DSQL_yyerrflag;
#endif /* error recovery flag */
#ifndef SHLIB_DEFS
int DSQL_yychar;
#endif /* current input token number */



#ifdef YYNMBCHARS
#define YYLEX(client_dialect, db_dialect, parser_version, stmt_ambiguous)		yycvtok(yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous))
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
static int yycvtok(int i)
#else
static int yycvtok(i)
	 int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if (i & 0x60000000) {		/*Must convert to a token. */
		if (yymbchars[last].character < i) {
			return i;			/*Giving up */
		}
		while ((last >= first) && (first >= 0)) {	/*Binary search loop */
			mid = (first + last) / 2;
			j = yymbchars[mid].character;
			if (j == i) {		/*Found */
				return yymbchars[mid].tvalue;
			}
			else if (j < i) {
				first = mid + 1;
			}
			else {
				last = mid - 1;
			}
		}
		/*No entry in the table. */
		return i;				/* Giving up. */
	}
	else {						/* i is already a token. */
		return i;
	}
}
#else /*!YYNMBCHARS */
#define YYLEX(client_dialect, db_dialect, parser_version, stmt_ambiguous)		yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)
#endif /*!YYNMBCHARS */

/*
** dsql_yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int dsql_yyparse(USHORT client_dialect, USHORT db_dialect,
				 USHORT parser_version, BOOLEAN * stmt_ambiguous)
#else
int dsql_yyparse(USHORT client_dialect, USHORT db_dialect,
				 USHORT parser_version, BOOLEAN * stmt_ambiguous)
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__) {
	case 1:
		goto yyerrlab;
	case 2:
		goto yynewstate;
	}
#endif

	/*
	   ** Initialize externals - dsql_yyparse may be called more than once
	 */
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	DSQL_yyerrflag = 0;
	DSQL_yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0) {
		if ((yymaxdepth = YYEXPAND(0)) <= 0) {
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;	/* top of state stack */
		register int yy_state;	/* current state */
		register int yy_n;		/* internal state number info */
		goto yystack;			/* moved from 6 lines above to here to please C++ */

		/*
		   ** get globals into registers.
		   ** branch to here only if YYBACKUP was called.
		 */
	  yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		   ** get globals into registers.
		   ** either we just started, or we just finished a reduction
		 */
	  yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		   ** top of for (;;) loop while no reductions done
		 */
	  yy_stack:
		/*
		   ** put a state and value onto the stacks
		 */
#if YYDEBUG
		/*
		   ** if debugging, look up token value in list of value vs.
		   ** name pairs.  0 and negative (-1) are special values.
		   ** Note: linear search is used since time is not a real
		   ** consideration while debugging.
		 */
		if (yydebug) {
			register int yy_i;

			printf("State %d, token ", yy_state);
			if (DSQL_yychar == 0)
				printf("end-of-file\n");
			else if (DSQL_yychar < 0)
				printf("-none-\n");
			else {
				for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
					if (yytoks[yy_i].t_val == DSQL_yychar)
						break;
				}
				printf("%s\n", yytoks[yy_i].t_name);
			}
		}
#endif /* YYDEBUG */
		if (++yy_ps >= &yys[yymaxdepth]) {	/* room on stack? */
			/*
			   ** reallocate and recover.  Note that pointers
			   ** have to be reset, or bad things will happen
			 */
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH) {	/* first time growth */
				char *newyys = (char *) YYNEW(int);
				char *newyyv = (char *) YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0) {
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else {				/* not first time */

				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth) {	/* tables not expanded */
				yyerror("yacc stack overflow");
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		   ** we have a new state - find out what to do
		 */
	  yy_newstate:
		if ((yy_n = yypact[yy_state]) <= YYFLAG)
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		   ** if debugging, need to mark whether new token grabbed
		 */
		yytmp = DSQL_yychar < 0;
#endif
		if ((DSQL_yychar < 0)
			&&
			((DSQL_yychar
			  =
			  YYLEX(client_dialect, db_dialect, parser_version,
					stmt_ambiguous)) < 0)) DSQL_yychar = 0;	/* reached EOF */
#if YYDEBUG
		if (yydebug && yytmp) {
			register int yy_i;

			printf("Received token ");
			if (DSQL_yychar == 0)
				printf("end-of-file\n");
			else if (DSQL_yychar < 0)
				printf("-none-\n");
			else {
				for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
					if (yytoks[yy_i].t_val == DSQL_yychar)
						break;
				}
				printf("%s\n", yytoks[yy_i].t_name);
			}
		}
#endif /* YYDEBUG */
		if (((yy_n += DSQL_yychar) < 0) || (yy_n >= YYLAST))
			goto yydefault;
		if (yychk[yy_n = yyact[yy_n]] == DSQL_yychar) {	/*valid shift */
			DSQL_yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if (DSQL_yyerrflag > 0)
				DSQL_yyerrflag--;
			goto yy_stack;
		}

	  yydefault:
		if ((yy_n = yydef[yy_state]) == -2) {
#if YYDEBUG
			yytmp = DSQL_yychar < 0;
#endif
			if ((DSQL_yychar < 0)
				&&
				((DSQL_yychar
				  =
				  YYLEX(client_dialect, db_dialect, parser_version,
						stmt_ambiguous)) < 0)) DSQL_yychar = 0;	/* reached EOF */
#if YYDEBUG
			if (yydebug && yytmp) {
				register int yy_i;

				printf("Received token ");
				if (DSQL_yychar == 0)
					printf("end-of-file\n");
				else if (DSQL_yychar < 0)
					printf("-none-\n");
				else {
					for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
						if (yytoks[yy_i].t_val == DSQL_yychar) {
							break;
						}
					}
					printf("%s\n", yytoks[yy_i].t_name);
				}
			}
#endif /* YYDEBUG */
			/*
			   ** look through exception table
			 */
			{
				register const int *yyxi = yyexca;

				while ((*yyxi != -1) || (yyxi[1] != yy_state)) {
					yyxi += 2;
				}
				while ((*(yyxi += 2) >= 0) && (*yyxi != DSQL_yychar));
				if ((yy_n = yyxi[1]) < 0)
					YYACCEPT;
			}
		}

		/*
		   ** check for syntax error
		 */
		if (yy_n == 0) {		/* have an error */
			/* no worry about speed here! */
			switch (DSQL_yyerrflag) {
			case 0:			/* new error */
				yyerror("syntax error");
				goto skip_init;
			  yyerrlab:
				/*
				   ** get globals into registers.
				   ** we have a user generated syntax type error
				 */
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			  skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:			/* incompletely recovered error */
				/* try again... */
				DSQL_yyerrflag = 3;
				/*
				   ** find state where "error" is a legal
				   ** shift action
				 */
				while (yy_ps >= yys) {
					yy_n = yypact[*yy_ps] + YYERRCODE;
					if (yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE) {
						/*
						   ** simulate shift of "error"
						 */
						yy_state = yyact[yy_n];
						goto yy_stack;
					}
					/*
					   ** current state has no shift on
					   ** "error", pop stack
					 */
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if (yydebug)
						printf(_POP_, *yy_ps, yy_ps[-1]);
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				   ** there is no state on stack with "error" as
				   ** a valid shift.  give up.
				 */
				YYABORT;
			case 3:			/* no shift yet; eat a token */
#if YYDEBUG
				/*
				   ** if debugging, look up token in list of
				   ** pairs.  0 and negative shouldn't occur,
				   ** but since timing doesn't matter when
				   ** debugging, it doesn't hurt to leave the
				   ** tests here.
				 */
				if (yydebug) {
					register int yy_i;

					printf("Error recovery discards ");
					if (DSQL_yychar == 0)
						printf("token end-of-file\n");
					else if (DSQL_yychar < 0)
						printf("token -none-\n");
					else {
						for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
							if (yytoks[yy_i].t_val == DSQL_yychar) {
								break;
							}
						}
						printf("token %s\n", yytoks[yy_i].t_name);
					}
				}
#endif /* YYDEBUG */
				if (DSQL_yychar == 0)	/* reached EOF. quit */
					YYABORT;
				DSQL_yychar = -1;
				goto yy_newstate;
			}
		}						/* end if ( yy_n == 0 ) */
		/*
		   ** reduction by production yy_n
		   ** put stack tops, etc. so things right after switch
		 */
#if YYDEBUG
		/*
		   ** if debugging, print the string that is the user's
		   ** specification of the reduction which is just about
		   ** to be done.
		 */
		if (yydebug)
			printf("Reduce by (%d) \"%s\"\n", yy_n, yyreds[yy_n]);
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		   ** Look in goto table for next state
		   ** Sorry about using yy_state here as temporary
		   ** register variable, but why not, if it works...
		   ** If yyr2[ yy_n ] doesn't have the low order bit
		   ** set, then there is no action to be done for
		   ** this reduction.  So, no saving & unsaving of
		   ** registers done.  The only difference between the
		   ** code just after the if and the body of the if is
		   ** the goto yy_stack in the body.  This way the test
		   ** can be made before the choice of what to do is needed.
		 */
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[yy_n];

			if (!(yy_len & 01)) {
				yy_len >>= 1;
				yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
				yy_state = yypgo[yy_n = yyr1[yy_n]] + *(yy_ps -= yy_len) + 1;
				if (yy_state >= YYLAST ||
					yychk[yy_state = yyact[yy_state]] != -yy_n) {
					yy_state = yyact[yypgo[yy_n]];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
			yy_state = yypgo[yy_n = yyr1[yy_n]] + *(yy_ps -= yy_len) + 1;
			if (yy_state >= YYLAST ||
				yychk[yy_state = yyact[yy_state]] != -yy_n) {
				yy_state = yyact[yypgo[yy_n]];
			}
		}
		/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	   ** code supplied by user is placed in this switch
	 */
	switch (yytmp) {

	case 1:{
			DSQL_parse = yypvt[-0];
		}
		break;
	case 2:{
			DSQL_parse = yypvt[-1];
		}
		break;
	case 18:{
			DSQL_debug = (int) yypvt[-0];
			if (DSQL_debug > 10)
				yydebug = DSQL_debug;
			yyval = make_node(nod_null, (int) 0, NULL);
		} break;
	case 19:{
			yyval = make_node(nod_grant, (int) e_grant_count,
							  yypvt[-5], yypvt[-3], make_list(yypvt[-1]),
							  yypvt[-0]);
		} break;
	case 20:{
			yyval = make_node(nod_grant, (int) e_grant_count,
							  yypvt[-6], yypvt[-3], make_list(yypvt[-1]),
							  yypvt[-0]);
		} break;
	case 21:{
			yyval = make_node(nod_grant, (int) e_grant_count,
							  yypvt[-4], yypvt[-2], make_list(yypvt[-0]),
							  NULL);} break;
	case 22:{
			yyval =
				make_node(nod_grant, (int) e_grant_count, yypvt[-5],
						  yypvt[-2], make_list(yypvt[-0]), NULL);
		} break;
	case 23:{
			yyval = make_node(nod_grant, (int) e_grant_count,
							  make_list(yypvt[-3]), make_list(yypvt[-1]),
							  NULL, yypvt[-0]);
		} break;
	case 25:{
			yyval = yypvt[-0];
		}
		break;
	case 26:{
			yyval = make_node(nod_all, (int) 0, NULL);
		} break;
	case 27:{
			yyval = make_node(nod_all, (int) 0, NULL);
		} break;
	case 28:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 30:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 31:{
			yyval = make_list(make_node(nod_execute, (int) 0, NULL));
		} break;
	case 32:{
			yyval = make_node(nod_select, (int) 0, NULL);
		} break;
	case 33:{
			yyval = make_node(nod_insert, (int) 0, NULL);
		} break;
	case 34:{
			yyval = make_node(nod_delete, (int) 0, NULL);
		} break;
	case 35:{
			yyval = make_node(nod_update, (int) 1, yypvt[-0]);
		} break;
	case 36:{
			yyval = make_node(nod_references, (int) 1, yypvt[-0]);
		} break;
	case 37:{
			yyval = make_node(nod_grant, (int) 0, NULL);
		} break;
	case 38:{
			yyval = 0;
		}
		break;
	case 39:{
			yyval = make_node(nod_grant_admin, (int) 0, NULL);
		} break;
	case 40:{
			yyval = 0;
		}
		break;
	case 41:{
			yyval = make_node(nod_procedure_name, (int) 1, yypvt[-0]);
		} break;
	case 42:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-4], yypvt[-2],
							  make_list(yypvt[-0]), yypvt[-5]);
		} break;
	case 43:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-5], yypvt[-2],
							  make_list(yypvt[-0]), yypvt[-6]);
		} break;
	case 44:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-4], yypvt[-2],
							  make_list(yypvt[-0]), NULL);
		} break;
	case 45:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-5], yypvt[-2],
							  make_list(yypvt[-0]), NULL);
		} break;
	case 46:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-4], yypvt[-2],
							  make_list(yypvt[-0]), NULL);
		} break;
	case 47:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, yypvt[-5], yypvt[-2],
							  make_list(yypvt[-0]), NULL);
		} break;
	case 48:{
			yyval = make_node(nod_revoke,
							  (int) e_grant_count, make_list(yypvt[-2]),
							  make_list(yypvt[-0]), NULL, NULL);
		} break;
	case 49:{
			yyval = make_node(nod_grant, (int) 0, NULL);
		} break;
	case 51:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 52:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 53:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 54:{
			yyval = make_node(nod_proc_obj, (int) 1, yypvt[-0]);
		} break;
	case 55:{
			yyval = make_node(nod_trig_obj, (int) 1, yypvt[-0]);
		} break;
	case 56:{
			yyval = make_node(nod_view_obj, (int) 1, yypvt[-0]);
		} break;
	case 58:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 59:{
			yyval = make_node(nod_user_name, (int) 1, yypvt[-0]);
		} break;
	case 60:{
			yyval = make_node(nod_user_name, (int) 1, yypvt[-0]);
		} break;
	case 61:{
			yyval = make_node(nod_user_group, (int) 1, yypvt[-0]);
		} break;
	case 63:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 64:{
			yyval = make_node(nod_role_name, (int) 1, yypvt[-0]);
		} break;
	case 66:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 67:{
			yyval = make_node(nod_user_name, (int) 1, yypvt[-0]);
		} break;
	case 68:{
			yyval = make_node(nod_user_name, (int) 1, yypvt[-0]);
		} break;
	case 69:{
			yyval = yypvt[-0];
		}
		break;
	case 70:{
			yyval = yypvt[-0];
		}
		break;
	case 71:{
			yyval = yypvt[-0];
		}
		break;
	case 72:{
			yyval = make_node(nod_def_udf, (int) e_udf_count,
							  yypvt[-7], yypvt[-2], yypvt[-0],
							  make_list(yypvt[-6]), yypvt[-4]);
		} break;
	case 74:{
			field->fld_dtype = dtype_blob;
		}
		break;
	case 75:{
			field->fld_dtype = dtype_cstring;
			field->fld_character_length =
				static_cast < SSHORT > ((SLONG) yypvt[-2]);
		}
		break;
	case 76:{
			yyval = (NOD) NULL;
		}
		break;
	case 78:{
			yyval = yypvt[-1];
		}
		break;
	case 80:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 81:{
			yyval = yypvt[-1];
		}
		break;
	case 83:{
			yyval = yypvt[-1];
		}
		break;
	case 84:{
			yyval = make_node(nod_udf_return_value, (int) 2, yypvt[-1],
							  MAKE_constant((STR) FUN_reference,
											CONSTANT_SLONG));} break;
	case 85:{
			yyval =
				make_node(nod_udf_return_value, (int) 2, yypvt[-2],
						  MAKE_constant((STR) (-1 * FUN_reference),
										CONSTANT_SLONG));} break;
	case 86:{
			yyval =
				make_node(nod_udf_return_value, (int) 2, yypvt[-3],
						  MAKE_constant((STR) FUN_value, CONSTANT_SLONG));
		} break;
	case 87:{
			yyval = make_node(nod_udf_return_value, (int) 2,
							  (NOD) NULL, MAKE_constant((STR) yypvt[-0],
														CONSTANT_SLONG));}
			break;
	case 88:{
			yyval =
				make_node(nod_def_filter, (int) e_filter_count, yypvt[-8],
						  yypvt[-6], yypvt[-4], yypvt[-2], yypvt[-0]);
		} break;
	case 89:{
			yyval = yypvt[-0];
		}
		break;
	case 90:{
			yyval = make_node(nod_def_exception, (int) e_xcp_count,
							  yypvt[-1], yypvt[-0]);
		} break;
	case 91:{
			yyval = make_node(nod_def_index, (int) e_idx_count,
							  yypvt[-6], yypvt[-5], yypvt[-3], yypvt[-1],
							  yypvt[-0]);
		} break;
	case 92:{
			yyval = yypvt[-0];
		}
		break;
	case 93:{
			yyval = yypvt[-0];
		}
		break;
	case 94:{
			yyval = yypvt[-0];
		}
		break;
	case 95:{
			yyval = yypvt[-0];
		}
		break;
	case 96:{
			yyval = yypvt[-0];
		}
		break;
	case 97:{
			yyval = yypvt[-0];
		}
		break;
	case 98:{
			yyval = yypvt[-0];
		}
		break;
	case 99:{
			yyval = yypvt[-0];
		}
		break;
	case 100:{
			yyval = yypvt[-0];
		}
		break;
	case 101:{
			yyval = make_node(nod_unique, (int) 0, NULL);
		} break;
	case 102:{
			yyval = NULL;
		}
		break;
	case 103:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 105:{
			yyval = make_node(nod_def_computed, 2, yypvt[-2], yypvt[-1]);
		}
		break;
	case 106:{
			yyval = make_node(nod_def_shadow, (int) e_shadow_count,
							  yypvt[-5], yypvt[-4], yypvt[-3], yypvt[-2],
							  yypvt[-1], make_list(yypvt[-0]));
		} break;
	case 107:{
			yyval = MAKE_constant((STR) 1, CONSTANT_SLONG);
		}
		break;
	case 108:{
			yyval = MAKE_constant((STR) 0, CONSTANT_SLONG);
		}
		break;
	case 109:{
			yyval = MAKE_constant((STR) 0, CONSTANT_SLONG);
		}
		break;
	case 110:{
			yyval = MAKE_constant((STR) 0, CONSTANT_SLONG);
		}
		break;
	case 111:{
			yyval = MAKE_constant((STR) 1, CONSTANT_SLONG);
		}
		break;
	case 112:{
			yyval = (NOD) 0;
		}
		break;
	case 113:{
			yyval = yypvt[-1];
		}
		break;
	case 114:{
			yyval = (NOD) NULL;
		}
		break;
	case 117:{
			yyval = make_node(nod_list, (int) 2, yypvt[-1], yypvt[-0]);
		} break;
	case 118:{
			yyval = make_node(nod_def_domain, (int) e_dom_count,
							  yypvt[-7], yypvt[-3], yypvt[-2],
							  make_list(yypvt[-1]), yypvt[-0]);
		} break;
	case 119:{
			yyval = NULL;
		}
		break;
	case 120:{
			yyval = NULL;
		}
		break;
	case 121:{
			yyval = yypvt[-0];
		}
		break;
	case 122:{
			yyval = (NOD) NULL;
		}
		break;
	case 123:{
			yyval = (NOD) NULL;
		}
		break;
	case 126:{
			yyval = make_node(nod_list, (int) 2, yypvt[-1], yypvt[-0]);
		} break;
	case 127:{
			yyval = make_node(nod_rel_constraint, (int) 2, NULL, yypvt[-0]);
		} break;
	case 130:{
			yyval = make_node(nod_null, (int) 0, NULL);
		} break;
	case 131:{
			yyval = make_node(nod_def_constraint,
							  (int) e_cnstr_count, MAKE_string(NULL_STRING,
															   0), NULL, NULL,
							  NULL, yypvt[-2], NULL, yypvt[-0], NULL, NULL);
		} break;
	case 132:{
			yyval = make_node(nod_def_generator,
							  (int) e_gen_count, yypvt[-0]);} break;
	case 133:{
			yyval = make_node(nod_def_role, (int) 1, yypvt[-0]);
		} break;
	case 134:{
			yyval = make_node(nod_def_database, (int) e_cdb_count,
							  yypvt[-2], make_list(yypvt[-1]),
							  make_list(yypvt[-0]));} break;
	case 137:{
			log_defined = FALSE;
			cache_defined = FALSE;
			yyval = (NOD) yypvt[-0];
		}
		break;
	case 138:{
			yyval = (NOD) NULL;
		}
		break;
	case 141:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 142:{
			yyval = make_node(nod_page_size, 1, yypvt[-0]);
		}
		break;
	case 143:{
			yyval = make_node(nod_file_length, 1, yypvt[-1]);
		}
		break;
	case 144:{
			yyval = make_node(nod_user_name, 1, yypvt[-0]);
		}
		break;
	case 145:{
			yyval = make_node(nod_password, 1, yypvt[-0]);
		}
		break;
	case 146:{
			yyval = make_node(nod_lc_ctype, 1, yypvt[-0]);
		}
		break;
	case 147:{
			yyval = (NOD) NULL;
		}
		break;
	case 150:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 154:{
			yyval = make_node(nod_dfl_charset, 1, yypvt[-0]);
		}
		break;
	case 155:{
			yyval = make_node(nod_group_commit_wait, 1, yypvt[-0]);
		}
		break;
	case 156:{
			yyval = make_node(nod_check_point_len, 1, yypvt[-0]);
		}
		break;
	case 157:{
			yyval = make_node(nod_num_log_buffers, 1, yypvt[-0]);
		}
		break;
	case 158:{
			yyval = make_node(nod_log_buffer_size, 1, yypvt[-0]);
		}
		break;
	case 159:{
			if (log_defined)
				yyabandon(-260, isc_log_redef);	/* Log redefined */
			log_defined = TRUE;
			yyval = yypvt[-0];
		}
		break;
	case 160:{
			if (log_defined)
				yyabandon(-260, isc_log_redef);
			log_defined = TRUE;
			yyval = yypvt[-0];
		}
		break;
	case 161:{
			file->fil_flags |= LOG_serial | LOG_overflow;
			if (file->fil_partitions)
				yyabandon(-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			yyval = make_node(nod_list, 2, yypvt[-3], yypvt[-0]);
		}
		break;
	case 162:{
			file->fil_flags |= LOG_serial;
			if (file->fil_partitions)
				yyabandon(-261, isc_partition_not_supp);
			yyval = yypvt[-0];
		}
		break;
	case 163:{
			file = make_file();
			file->fil_flags = LOG_serial | LOG_default;
			yyval = make_node(nod_log_file_desc, (int) 1, (NOD) file);
		} break;
	case 164:{
			file->fil_name = (STR) yypvt[-1];
			yyval = (NOD) make_node(nod_file_desc, (int) 1, (NOD) file);
		} break;
	case 166:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 167:{
			check_log_file_attrs();
			yyval = (NOD) make_node(nod_log_file_desc, (int) 1, (NOD) file);
		} break;
	case 168:{
			file = make_file();
			file->fil_name = (STR) yypvt[-0];
		}
		break;
	case 171:{
			file->fil_length = (SLONG) yypvt[-0];
		}
		break;
	case 172:{
			file = make_file();
		}
		break;
	case 177:{
			file->fil_start = (SLONG) yypvt[-0];
		}
		break;
	case 178:{
			file->fil_length = (SLONG) yypvt[-1];
		}
		break;
	case 185:{
			yyval = make_node(nod_def_relation,
							  (int) e_drl_count, yypvt[-4],
							  make_list(yypvt[-1]), yypvt[-3]);
		} break;
	case 186:{
			yyval = yypvt[-0];
		}
		break;
	case 187:{
			yyval = yypvt[-0];
		}
		break;
	case 188:{
			yyval = (NOD) NULL;
		}
		break;
	case 190:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 193:{
			yyval = make_node(nod_def_field, (int) e_dfl_count,
							  yypvt[-5], yypvt[-3], yypvt[-2],
							  make_list(yypvt[-1]), yypvt[-0], yypvt[-4],
							  NULL);} break;
	case 194:{
			yyval =
				make_node(nod_def_field, (int) e_dfl_count, yypvt[-2], NULL,
						  NULL, NULL, NULL, NULL, yypvt[-0]);
		} break;
	case 195:{
			yyval = make_node(nod_def_field, (int) e_dfl_count,
							  yypvt[-1], NULL, NULL, NULL, NULL, NULL,
							  yypvt[-0]);} break;
	case 196:{
			field->fld_flags |= FLD_computed;
			yyval = make_node(nod_def_computed, 2, yypvt[-2], yypvt[-1]);
		}
		break;
	case 199:{
			yyval = NULL;
		}
		break;
	case 200:{
			yyval = make_node(nod_def_domain, (int) e_dom_count,
							  yypvt[-1], NULL, NULL, NULL, NULL);
		} break;
	case 201:{
			yyval = yypvt[-0];
		}
		break;
	case 202:{
			yyval = (NOD) NULL;
		}
		break;
	case 203:{
			field_name = yypvt[-0];
			field = make_field(yypvt[-0]);
			yyval = (NOD) field;
		}
		break;
	case 204:{
			field = make_field(yypvt[-0]);
			yyval = (NOD) field;
		}
		break;
	case 205:{
			yyval = yypvt[-1];
		}
		break;
	case 206:{
			field = make_field(NULL);
			yyval = (NOD) field;
		}
		break;
	case 207:{
			yyval = yypvt[-0];
		}
		break;
	case 208:{
			yyval = (NOD) NULL;
		}
		break;
	case 209:{
			yyval = yypvt[-0];
		}
		break;
	case 210:{
			yyval = make_node(nod_user_name, (int) 0, NULL);
		} break;
	case 211:{
			yyval = yypvt[-0];
		}
		break;
	case 212:{
			yyval = yypvt[-0];
		}
		break;
	case 213:{
			yyval = (NOD) NULL;
		}
		break;
	case 216:{
			yyval = make_node(nod_list, (int) 2, yypvt[-1], yypvt[-0]);
		} break;
	case 217:{
			yyval =
				make_node(nod_rel_constraint, (int) 2, yypvt[-1],
						  yypvt[-0]);} break;
	case 218:{
			yyval = make_node(nod_null, (int) 1, NULL);
		} break;
	case 219:{
			yyval = make_node(nod_foreign, e_for_count,
							  make_node(nod_list, (int) 1, field_name),
							  yypvt[-2], yypvt[-1], yypvt[-0]);
		} break;
	case 221:{
			yyval = make_node(nod_unique, (int) 0, NULL);
		} break;
	case 222:{
			yyval = make_node(nod_primary, (int) 0, NULL);
		} break;
	case 223:{
			yyval =
				make_node(nod_rel_constraint, (int) 2, yypvt[-1],
						  yypvt[-0]);} break;
	case 224:{
			yyval = yypvt[-0];
		}
		break;
	case 225:{
			yyval = NULL;
		}
		break;
	case 230:{
			yyval = make_node(nod_unique, 1, yypvt[-0]);
		}
		break;
	case 231:{
			yyval = make_node(nod_primary, e_pri_count, yypvt[-0]);
		}
		break;
	case 232:{
			yyval = make_node(nod_foreign, e_for_count, yypvt[-4], yypvt[-2],
							  yypvt[-1], yypvt[-0]);
		}
		break;
	case 233:{
			yyval = make_node(nod_def_constraint,
							  (int) e_cnstr_count, MAKE_string(NULL_STRING,
															   0), NULL, NULL,
							  NULL, yypvt[-2], NULL, yypvt[-0], NULL, NULL);
		} break;
	case 234:{
			yyval =
				make_node(nod_ref_upd_del, e_ref_upd_del_count, yypvt[-0],
						  NULL);} break;
	case 235:{
			yyval =
				make_node(nod_ref_upd_del, e_ref_upd_del_count, NULL,
						  yypvt[-0]);}
		break;
	case 236:{
			yyval =
				make_node(nod_ref_upd_del, e_ref_upd_del_count, yypvt[-0],
						  yypvt[-1]);}
		break;
	case 237:{
			yyval =
				make_node(nod_ref_upd_del, e_ref_upd_del_count, yypvt[-1],
						  yypvt[-0]);}
		break;
	case 238:{
			yyval = NULL;
		}
		break;
	case 239:{
			yyval = yypvt[-0];
		}
		break;
	case 240:{
			yyval = yypvt[-0];
		}
		break;
	case 241:{
			yyval = make_flag_node(nod_ref_trig_action,
								   REF_ACTION_CASCADE,
								   e_ref_trig_action_count, NULL);}
		break;
	case 242:{
			yyval = make_flag_node(nod_ref_trig_action,
								   REF_ACTION_SET_DEFAULT,
								   e_ref_trig_action_count, NULL);
		}
		break;
	case 243:{
			yyval = make_flag_node(nod_ref_trig_action,
								   REF_ACTION_SET_NULL,
								   e_ref_trig_action_count, NULL);}
		break;
	case 244:{
			yyval = make_flag_node(nod_ref_trig_action,
								   REF_ACTION_NONE, e_ref_trig_action_count,
								   NULL);}
		break;
	case 245:{
			yyval = make_node(nod_def_procedure,
							  (int) e_prc_count,
							  yypvt[-7], yypvt[-6], yypvt[-5], yypvt[-2],
							  yypvt[-1], yypvt[-0], NULL);
		} break;
	case 246:{
			yyval = make_node(nod_mod_procedure,
							  (int) e_prc_count,
							  yypvt[-7], yypvt[-6], yypvt[-5], yypvt[-2],
							  yypvt[-1], yypvt[-0], NULL);
		} break;
	case 247:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 248:{
			yyval = NULL;
		}
		break;
	case 249:{
			yyval = yypvt[-0];
		}
		break;
	case 250:{
			yyval = NULL;
		}
		break;
	case 252:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 253:{
			yyval = make_node(nod_def_field, (int) e_dfl_count,
							  yypvt[-1], NULL, NULL, NULL, NULL, NULL, NULL);
		} break;
	case 254:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 255:{
			yyval = NULL;
		}
		break;
	case 257:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 258:{
			yyval = make_node(nod_def_field, (int) e_dfl_count,
							  yypvt[-2], NULL, NULL, NULL, NULL, NULL, NULL);
		} break;
	case 261:{
			yyval = make_node(nod_block, e_blk_count,
							  make_list(yypvt[-1]), NULL);}
		break;
	case 262:{
			yyval = make_node(nod_block, e_blk_count,
							  make_list(yypvt[-2]), make_list(yypvt[-1]));
		}
		break;
	case 264:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 267:{
			yyval = make_node(nod_exception_stmt, 1, yypvt[-1]);
		}
		break;
	case 272:{
			yyval = make_node(nod_post, e_pst_count, yypvt[-1]);
		}
		break;
	case 276:{
			yyval = make_node(nod_return, e_rtn_count, NULL);
		}
		break;
	case 277:{
			yyval = make_node(nod_exit, 0, NULL);
		}
		break;
	case 278:{
			yyval = make_node(nod_exec_procedure, e_exe_count, yypvt[-3],
							  yypvt[-2], yypvt[-1]);
		}
		break;
	case 279:{
			yyval = make_node(nod_for_select, e_flp_count, yypvt[-5],
							  make_list(yypvt[-3]), yypvt[-2], yypvt[-0]);
		}
		break;
	case 280:{
			yyval =
				make_node(nod_if, e_if_count, yypvt[-5], yypvt[-2],
						  yypvt[-0]);} break;
	case 281:{
			yyval = make_node(nod_if, e_if_count, yypvt[-3], yypvt[-0], NULL);
		}
		break;
	case 282:{
			yyval = make_node(nod_for_select, e_flp_count, yypvt[-3],
							  make_list(yypvt[-1]), NULL, NULL);
		}
		break;
	case 283:{
			yyval = make_node(nod_var_name, (int) e_vrn_count, yypvt[-0]);
		} break;
	case 284:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 285:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 286:{
			yyval = NULL;
		}
		break;
	case 287:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 288:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 289:{
			yyval = NULL;
		}
		break;
	case 294:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 295:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 296:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 297:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 300:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 301:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 302:{
			yyval = make_node(nod_while, e_while_count,
							  yypvt[-3], yypvt[-0], NULL);}
		break;
	case 303:{
			yyval = make_node(nod_cursor, e_cur_count, yypvt[-0], NULL, NULL);
		}
		break;
	case 304:{
			yyval = NULL;
		}
		break;
	case 306:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 307:{
			yyval = make_node(nod_on_error, e_err_count,
							  make_list(yypvt[-2]), yypvt[-0]);
		}
		break;
	case 309:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 310:{
			yyval = make_node(nod_sqlcode, 1, yypvt[-0]);
		}
		break;
	case 311:{
			yyval = make_node(nod_gdscode, 1, yypvt[-0]);
		}
		break;
	case 312:{
			yyval = make_node(nod_exception, 1, yypvt[-0]);
		}
		break;
	case 313:{
			yyval = make_node(nod_default, 1, NULL);
		}
		break;
	case 314:{
			yyval = make_node(nod_exec_procedure, e_exe_count, yypvt[-1],
							  yypvt[-0], make_node(nod_all, (int) 0, NULL));
		} break;
	case 315:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 316:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 317:{
			yyval = NULL;
		}
		break;
	case 321:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 322:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 323:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 324:{
			yyval = make_node(nod_def_view, (int) e_view_count,
							  yypvt[-6], yypvt[-5], yypvt[-2], yypvt[-1],
							  yypvt[-0]);
		} break;
	case 325:{
			yyval = make_node(nod_select, (int) 2, yypvt[-0], NULL);
		} break;
	case 326:{
			yyval = make_node(nod_list, (int) 1, yypvt[-0]);
		} break;
	case 327:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 328:{
			yyval =
				make_flag_node(nod_list, NOD_UNION_ALL, 2, yypvt[-3],
							   yypvt[-0]);}
		break;
	case 329:{
			yyval = make_node(nod_select_expr, e_sel_count,
							  yypvt[-6], yypvt[-5], yypvt[-4], yypvt[-3],
							  yypvt[-2], yypvt[-1], yypvt[-0], NULL);
		}
		break;
	case 330:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 332:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 335:{
			yyval = make_node(nod_join, (int) e_join_count,
							  yypvt[-5], yypvt[-4], yypvt[-2], yypvt[-0]);
		} break;
	case 336:{
			yyval = yypvt[-1];
		}
		break;
	case 337:{
			beginning = lex_position();
		}
		break;
	case 338:{
			yyval = (NOD) MAKE_string((UCHAR *) beginning,
									  (lex_position() == end) ?
									  lex_position() -
									  beginning : last_token - beginning);
		}
		break;
	case 339:{
			beginning = last_token;
		}
		break;
	case 340:{
			yyval = (NOD) MAKE_string((UCHAR *) beginning,
									  lex_position() - beginning);}
		break;
	case 341:{
			yyval = make_node(nod_def_constraint, (int) e_cnstr_count,
							  MAKE_string(NULL_STRING, 0), NULL, NULL, NULL,
							  NULL, NULL, NULL, NULL, NULL);
		} break;
	case 342:{
			yyval = 0;
		}
		break;
	case 343:{
			yyval = make_node(nod_def_trigger, (int) e_trg_count,
							  yypvt[-8], yypvt[-6], yypvt[-5], yypvt[-4],
							  yypvt[-3], yypvt[-1], yypvt[-0], NULL);
		} break;
	case 344:{
			yyval = MAKE_constant((STR) 0, CONSTANT_SLONG);
		}
		break;
	case 345:{
			yyval = MAKE_constant((STR) 1, CONSTANT_SLONG);
		}
		break;
	case 346:{
			yyval = NULL;
		}
		break;
	case 347:{
			yyval = MAKE_constant((STR) 1, CONSTANT_SLONG);
		}
		break;
	case 348:{
			yyval = MAKE_constant((STR) 2, CONSTANT_SLONG);
		}
		break;
	case 349:{
			yyval = MAKE_constant((STR) 3, CONSTANT_SLONG);
		}
		break;
	case 350:{
			yyval = MAKE_constant((STR) 4, CONSTANT_SLONG);
		}
		break;
	case 351:{
			yyval = MAKE_constant((STR) 5, CONSTANT_SLONG);
		}
		break;
	case 352:{
			yyval = MAKE_constant((STR) 6, CONSTANT_SLONG);
		}
		break;
	case 353:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG);
		}
		break;
	case 354:{
			yyval = NULL;
		}
		break;
	case 355:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 356:{
			yyval = yypvt[-0];
		}
		break;
	case 357:{
			yyval = make_node(nod_mod_exception, (int) e_xcp_count,
							  yypvt[-1], yypvt[-0]);
		} break;
	case 358:{
			yyval = make_node(nod_mod_relation, (int) e_alt_count,
							  yypvt[-1], make_list(yypvt[-0]));
		} break;
	case 359:{
			yyval = yypvt[-0];
		}
		break;
	case 360:{
			yyval = yypvt[-0];
		}
		break;
	case 361:{
			yyval = make_node(nod_mod_database, (int) e_adb_count,
							  make_list(yypvt[-0]));
		} break;
	case 362:{
			yyval = make_node(nod_mod_domain, (int) e_alt_count,
							  yypvt[-1], make_list(yypvt[-0]));
		} break;
	case 363:{
			yyval = make_node(nod_mod_index,
							  (int) e_mod_idx_count, yypvt[-0]);} break;
	case 365:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 366:{
			yyval = make_node(nod_def_default, (int) e_dft_count,
							  yypvt[-1], yypvt[-0]);
		} break;
	case 367:{
			yyval = yypvt[-0];
		}
		break;
	case 368:{
			yyval = yypvt[-0];
		}
		break;
	case 369:{
			yyval = make_node(nod_del_default, (int) 0, NULL);
		} break;
	case 370:{
			yyval = make_node(nod_delete_rel_constraint, (int) 1, NULL);
		} break;
	case 371:{
			yyval = yypvt[-0];
		}
		break;
	case 372:{
			yyval = make_node(nod_mod_domain_type, 2, yypvt[-1]);
		}
		break;
	case 374:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 375:{
			yyval = make_node(nod_del_field, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 376:{
			yyval = make_node(nod_delete_rel_constraint, (int) 1, yypvt[-0]);
		} break;
	case 377:{
			yyval = yypvt[-0];
		}
		break;
	case 378:{
			yyval = yypvt[-0];
		}
		break;
	case 379:{
			yyval = make_node(nod_mod_field_pos, 2, yypvt[-2],
							  MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG));
		}
		break;
	case 380:{
			yyval = make_node(nod_mod_field_name, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 381:{
			yyval =
				make_node(nod_mod_field_type, 3, yypvt[-3], yypvt[-0],
						  yypvt[-1]);}
		break;
	case 382:{
			yyval = make_node(nod_field_name, (int) e_fln_count,
							  NULL, yypvt[-0]);} break;
	case 400:{
			yyval = NULL;
		}
		break;
	case 401:{
			yyval = NULL;
		}
		break;
	case 402:{
			yyval = NULL;
		}
		break;
	case 403:{
			yyval = make_node(nod_def_domain, (int) e_dom_count,
							  yypvt[-1], NULL, NULL, NULL, NULL);
		} break;
	case 404:{
			field_name = yypvt[-0];
			field = make_field(yypvt[-0]);
			yyval = (NOD) field;
		}
		break;
	case 405:{
			yyval = make_node(nod_restrict, 0, NULL);
		}
		break;
	case 406:{
			yyval = make_node(nod_cascade, 0, NULL);
		}
		break;
	case 407:{
			yyval = make_node(nod_restrict, 0, NULL);
		}
		break;
	case 408:{
			yyval = make_node(nod_idx_active, 1, yypvt[-1]);
		}
		break;
	case 409:{
			yyval = make_node(nod_idx_inactive, 1, yypvt[-1]);
		}
		break;
	case 410:{
			log_defined = FALSE;
			cache_defined = FALSE;
			yyval = (NOD) NULL;
		}
		break;
	case 412:{
			yyval = make_node(nod_list, (int) 2, yypvt[-1], yypvt[-0]);
		} break;
	case 413:{
			yyval = yypvt[-0];
		}
		break;
	case 414:{
			yyval = make_node(nod_drop_log, (int) 0, NULL);
		} break;
	case 415:{
			yyval = yypvt[-0];
		}
		break;
	case 416:{
			yyval = yypvt[-0];
		}
		break;
	case 418:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 419:{
			yyval = make_node(nod_mod_trigger, (int) e_trg_count,
							  yypvt[-6], NULL, yypvt[-5], yypvt[-4],
							  yypvt[-3], yypvt[-1], yypvt[-0], NULL);
		} break;
	case 421:{
			yyval = NULL;
		}
		break;
	case 423:{
			yyval = NULL;
		}
		break;
	case 424:{
			yyval = yypvt[-0];
		}
		break;
	case 425:{
			yyval = make_node(nod_del_exception, 1, yypvt[-0]);
		}
		break;
	case 426:{
			yyval = make_node(nod_del_index, (int) 1, yypvt[-0]);
		} break;
	case 427:{
			yyval = make_node(nod_del_procedure, (int) 1, yypvt[-0]);
		} break;
	case 428:{
			yyval = make_node(nod_del_relation, (int) 1, yypvt[-0]);
		} break;
	case 429:{
			yyval = make_node(nod_del_trigger, (int) 1, yypvt[-0]);
		} break;
	case 430:{
			yyval = make_node(nod_del_relation, (int) 1, yypvt[-0]);
		} break;
	case 431:{
			yyval = make_node(nod_del_filter, (int) 1, yypvt[-0]);
		} break;
	case 432:{
			yyval = make_node(nod_del_domain, (int) 1, yypvt[-0]);
		} break;
	case 433:{
			yyval = make_node(nod_del_udf, (int) 1, yypvt[-0]);
		} break;
	case 434:{
			yyval = make_node(nod_del_shadow, (int) 1, yypvt[-0]);
		} break;
	case 435:{
			yyval = make_node(nod_del_role, (int) 1, yypvt[-0]);
		} break;
	case 440:{
			field->fld_ranges = make_list(yypvt[-1]);
			field->fld_dimensions = field->fld_ranges->nod_count / 2;
			field->fld_element_dtype = field->fld_dtype;
			yyval = yypvt[-3];
		}
		break;
	case 441:{
			field->fld_ranges = make_list(yypvt[-2]);
			field->fld_dimensions = field->fld_ranges->nod_count / 2;
			field->fld_element_dtype = field->fld_dtype;
			yyval = yypvt[-4];
		}
		break;
	case 443:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 444:{
			if ((SLONG) yypvt[-0] < 1)
				yyval = make_node(nod_list, (int) 2,
								  MAKE_constant((STR) yypvt[-0],
												CONSTANT_SLONG),
								  MAKE_constant((STR) 1, CONSTANT_SLONG));
			else
				yyval =
					make_node(nod_list, (int) 2,
							  MAKE_constant((STR) 1, CONSTANT_SLONG),
							  MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG));
		} break;
	case 445:{
			yyval = make_node(nod_list, (int) 2,
							  MAKE_constant((STR) yypvt[-2], CONSTANT_SLONG),
							  MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG));
		} break;
	case 451:{
			field->fld_dtype = dtype_long;
			field->fld_length = sizeof(SLONG);
		}
		break;
	case 452:{
			field->fld_dtype = dtype_short;
			field->fld_length = sizeof(SSHORT);
		}
		break;
	case 453:{
			*stmt_ambiguous = TRUE;
			if (client_dialect <= SQL_DIALECT_V5) {
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
				ERRD_post_warning(isc_sqlwarn, gds_arg_number, (SLONG) 301,
								  isc_arg_warning, isc_dtype_renamed, 0);
				field->fld_dtype = dtype_timestamp;
				field->fld_length = sizeof(GDS_TIMESTAMP);
			}
			else if (client_dialect == SQL_DIALECT_V6_TRANSITION)
				yyabandon(-104, isc_transitional_date);
			else {
				field->fld_dtype = dtype_sql_date;
				field->fld_length = sizeof(ULONG);
			}
		}
		break;
	case 454:{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
						  gds_arg_number, client_dialect,
						  gds_arg_string, "TIME", 0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
						  gds_arg_number, db_dialect,
						  gds_arg_string, "TIME", 0);
			field->fld_dtype = dtype_sql_time;
			field->fld_length = sizeof(SLONG);
		}
		break;
	case 455:{
			field->fld_dtype = dtype_timestamp;
			field->fld_length = sizeof(GDS_TIMESTAMP);
		}
		break;
	case 458:{
			field->fld_dtype = dtype_blob;
		}
		break;
	case 459:{
			field->fld_dtype = dtype_blob;
			field->fld_seg_length =
				static_cast < USHORT > ((SLONG) yypvt[-1]);
			field->fld_sub_type = 0;
		}
		break;
	case 460:{
			field->fld_dtype = dtype_blob;
			field->fld_seg_length =
				static_cast < USHORT > ((SLONG) yypvt[-3]);
			field->fld_sub_type = static_cast < SSHORT > ((SLONG) yypvt[-1]);
		}
		break;
	case 461:{
			field->fld_dtype = dtype_blob;
			field->fld_seg_length = 80;
			field->fld_sub_type = static_cast < SSHORT > ((SLONG) yypvt[-1]);
		}
		break;
	case 462:{
			field->fld_seg_length =
				static_cast < USHORT > ((SLONG) yypvt[-0]);
		}
		break;
	case 463:{
			field->fld_seg_length = (SLONG) 80;
		}
		break;
	case 464:{
			field->fld_sub_type = static_cast < SSHORT > ((SLONG) yypvt[-0]);
		}
		break;
	case 465:{
			field->fld_sub_type_name = yypvt[-0];
		}
		break;
	case 466:{
			field->fld_sub_type = (SLONG) 0;
		}
		break;
	case 467:{
			field->fld_character_set = yypvt[-0];
		}
		break;
	case 469:{
			field->fld_dtype = dtype_text;
			field->fld_character_length =
				static_cast < USHORT > ((SLONG) yypvt[-1]);
			field->fld_flags |= FLD_national;
		}
		break;
	case 470:{
			field->fld_dtype = dtype_text;
			field->fld_character_length = 1;
			field->fld_flags |= FLD_national;
		}
		break;
	case 471:{
			field->fld_dtype = dtype_varying;
			field->fld_character_length =
				static_cast < USHORT > ((SLONG) yypvt[-1]);
			field->fld_flags |= FLD_national;
		}
		break;
	case 472:{
			field->fld_dtype = dtype_text;
			field->fld_character_length =
				static_cast < USHORT > ((SLONG) yypvt[-1]);
		}
		break;
	case 473:{
			field->fld_dtype = dtype_text;
			field->fld_character_length = 1;
		}
		break;
	case 474:{
			field->fld_dtype = dtype_varying;
			field->fld_character_length =
				static_cast < USHORT > ((SLONG) yypvt[-1]);
		}
		break;
	case 483:{
			field->fld_sub_type = dsc_num_type_numeric;
		}
		break;
	case 484:{
			field->fld_sub_type = dsc_num_type_decimal;
			if (field->fld_dtype == dtype_short) {
				field->fld_dtype = dtype_long;
				field->fld_length = sizeof(SLONG);
			};
		}
		break;
	case 485:{
			yyval = make_node(nod_position, 1, yypvt[-0]);
		}
		break;
	case 486:{
			field->fld_dtype = dtype_long;
			field->fld_length = sizeof(SLONG);
			field->fld_precision = 9;
		}
		break;
	case 487:{
			if (((SLONG) yypvt[-1] < 1) || ((SLONG) yypvt[-1] > 18))
				yyabandon(-842, isc_precision_err);
			/* Precision most be between 1 and 18. */
			if ((SLONG) yypvt[-1] > 9) {
				if (((client_dialect <= SQL_DIALECT_V5) &&
					 (db_dialect > SQL_DIALECT_V5)) ||
					((client_dialect > SQL_DIALECT_V5) &&
					 (db_dialect <= SQL_DIALECT_V5)))
						ERRD_post(gds_sqlerr,
								  gds_arg_number, (SLONG) - 817,
								  gds_arg_gds,
								  isc_ddl_not_allowed_by_db_sql_dial,
								  gds_arg_number, (SLONG) db_dialect, 0);
				if (client_dialect <= SQL_DIALECT_V5) {
					field->fld_dtype = dtype_double;
					field->fld_length = sizeof(double);
				}
				else {
					if (client_dialect == SQL_DIALECT_V6_TRANSITION) {
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous,
										  gds_arg_end);
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous1,
										  gds_arg_end);
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous2,
										  gds_arg_end);

					}
					field->fld_dtype = dtype_int64;
					field->fld_length = sizeof(SINT64);
				}
			}
			else if ((SLONG) yypvt[-1] < 5) {
				field->fld_dtype = dtype_short;
				field->fld_length = sizeof(SSHORT);
			}
			else {
				field->fld_dtype = dtype_long;
				field->fld_length = sizeof(SLONG);
			}
			field->fld_precision = static_cast < USHORT > ((SLONG) yypvt[-1]);
		}
		break;
	case 488:{
			if (((SLONG) yypvt[-3] < 1) || ((SLONG) yypvt[-3] > 18))
				yyabandon(-842, isc_precision_err);
			/* Precision should be between 1 and 18 */
			if (((SLONG) yypvt[-1] > (SLONG) yypvt[-3])
				|| ((SLONG) yypvt[-1] < 0))
				yyabandon(-842, isc_scale_nogt);
			/* Scale must be between 0 and precision */
			if ((SLONG) yypvt[-3] > 9) {
				if (((client_dialect <= SQL_DIALECT_V5) &&
					 (db_dialect > SQL_DIALECT_V5)) ||
					((client_dialect > SQL_DIALECT_V5) &&
					 (db_dialect <= SQL_DIALECT_V5)))
						ERRD_post(gds_sqlerr,
								  gds_arg_number, (SLONG) - 817,
								  gds_arg_gds,
								  isc_ddl_not_allowed_by_db_sql_dial,
								  gds_arg_number, (SLONG) db_dialect, 0);
				if (client_dialect <= SQL_DIALECT_V5) {
					field->fld_dtype = dtype_double;
					field->fld_length = sizeof(double);
				}
				else {
					if (client_dialect == SQL_DIALECT_V6_TRANSITION) {
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous,
										  gds_arg_end);
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous1,
										  gds_arg_end);
						ERRD_post_warning(isc_dsql_warn_precision_ambiguous2,
										  gds_arg_end);
					}
					/* client_dialect >= SQL_DIALECT_V6 */
					field->fld_dtype = dtype_int64;
					field->fld_length = sizeof(SINT64);
				}
			}
			else {
				if ((SLONG) yypvt[-3] < 5) {
					field->fld_dtype = dtype_short;
					field->fld_length = sizeof(SSHORT);
				}
				else {
					field->fld_dtype = dtype_long;
					field->fld_length = sizeof(SLONG);
				}
			}
			field->fld_precision = static_cast < USHORT > ((SLONG) yypvt[-3]);
			field->fld_scale = static_cast < SSHORT > (-(SLONG) yypvt[-1]);
		}
		break;
	case 491:{
			if ((SLONG) yypvt[-0] > 7) {
				field->fld_dtype = dtype_double;
				field->fld_length = sizeof(double);
			}
			else {
				field->fld_dtype = dtype_real;
				field->fld_length = sizeof(float);
			}
		} break;
	case 492:{
			field->fld_dtype = dtype_double;
			field->fld_length = sizeof(double);
		} break;
	case 493:{
			field->fld_dtype = dtype_real;
			field->fld_length = sizeof(float);
		} break;
	case 494:{
			field->fld_dtype = dtype_double;
			field->fld_length = sizeof(double);
		} break;
	case 495:{
			yyval = yypvt[-1];
		}
		break;
	case 496:{
			yyval = 0;
		}
		break;
	case 500:{
			yyval = make_node(nod_set_generator2, e_gen_id_count, yypvt[-2],
							  MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG));
		}
		break;
	case 501:{
			yyval = make_node(nod_set_generator2, e_gen_id_count, yypvt[-2],
							  MAKE_constant((STR) yypvt[-0],
											CONSTANT_SINT64));
		}
		break;
	case 502:{
			yyval = make_node(nod_set_generator2, e_gen_id_count, yypvt[-3],
							  make_node(nod_negate, 1,
										MAKE_constant((STR) yypvt[-0],
													  CONSTANT_SINT64)));
		}
		break;
	case 503:{
			yyval = make_node(nod_commit, 1, yypvt[-0]);
		}
		break;
	case 504:{
			yyval = make_node(nod_rollback, 0, NULL);
		}
		break;
	case 507:{
			yyval = make_node(nod_commit_retain, 0, NULL);
		}
		break;
	case 508:{
			yyval = (NOD) NULL;
		}
		break;
	case 510:{
			yyval = (NOD) NULL;
		}
		break;
	case 511:{
			yyval = make_node(nod_trans, 1, make_list(yypvt[-0]));
		}
		break;
	case 513:{
			yyval = (NOD) NULL;
		}
		break;
	case 515:{
			yyval = make_node(nod_list, (int) 2, yypvt[-1], yypvt[-0]);
		} break;
	case 520:{
			yyval = make_flag_node(nod_access, NOD_READ_ONLY, (int) 0, NULL);
		} break;
	case 521:{
			yyval = make_flag_node(nod_access, NOD_READ_WRITE, (int) 0, NULL);
		} break;
	case 522:{
			yyval = make_flag_node(nod_wait, NOD_WAIT, (int) 0, NULL);
		} break;
	case 523:{
			yyval = make_flag_node(nod_wait, NOD_NO_WAIT, (int) 0, NULL);
		} break;
	case 524:{
			yyval = yypvt[-0];
		}
		break;
	case 526:{
			yyval = yypvt[-0];
		}
		break;
	case 527:{
			yyval =
				make_flag_node(nod_isolation, NOD_READ_COMMITTED, 1,
							   yypvt[-0]);} break;
	case 528:{
			yyval =
				make_flag_node(nod_isolation, NOD_READ_COMMITTED, 1,
							   yypvt[-0]);} break;
	case 529:{
			yyval = make_flag_node(nod_isolation, NOD_CONCURRENCY, 0, NULL);
		}
		break;
	case 530:{
			yyval = make_flag_node(nod_isolation, NOD_CONSISTENCY, 0, NULL);
		}
		break;
	case 531:{
			yyval = make_flag_node(nod_isolation, NOD_CONSISTENCY, 0, NULL);
		}
		break;
	case 532:{
			yyval = make_flag_node(nod_version, NOD_VERSION, 0, NULL);
		}
		break;
	case 533:{
			yyval = make_flag_node(nod_version, NOD_NO_VERSION, 0, NULL);
		}
		break;
	case 534:{
			yyval = 0;
		}
		break;
	case 535:{
			yyval = make_node(nod_reserve, 1, make_list(yypvt[-0]));
		}
		break;
	case 536:{
			yyval = (NOD) NOD_SHARED;
		}
		break;
	case 537:{
			yyval = (NOD) NOD_PROTECTED;
		}
		break;
	case 538:{
			yyval = (NOD) 0;
		}
		break;
	case 539:{
			yyval = (NOD) NOD_READ;
		}
		break;
	case 540:{
			yyval = (NOD) NOD_WRITE;
		}
		break;
	case 542:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 543:{
			yyval =
				make_node(nod_table_lock, (int) 2, make_list(yypvt[-1]),
						  yypvt[-0]);} break;
	case 544:{
			yyval =
				make_flag_node(nod_lock_mode,
							   ((SSHORT) yypvt[-1] | (SSHORT) yypvt[-0]), 0,
							   NULL);}
		break;
	case 545:{
			yyval = 0;
		}
		break;
	case 547:{
			yyval = make_node(nod_list, (int) 2, yypvt[-2], yypvt[-0]);
		} break;
	case 548:{
			yyval = make_node(nod_set_statistics,
							  (int) e_stat_count, yypvt[-0]);} break;
	case 549:{
			yyval = make_node(nod_select, 3, yypvt[-2], yypvt[-1], yypvt[-0]);
		}
		break;
	case 550:{
			yyval = make_node(nod_list, 1, yypvt[-0]);
		}
		break;
	case 551:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 552:{
			yyval =
				make_flag_node(nod_list, NOD_UNION_ALL, 2, yypvt[-3],
							   yypvt[-0]);}
		break;
	case 553:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 554:{
			yyval = 0;
		}
		break;
	case 556:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 557:{
			yyval =
				make_node(nod_order, e_order_count, yypvt[-2], yypvt[-0],
						  yypvt[-1]);}
		break;
	case 558:{
			yyval =
				make_node(nod_order, e_order_count, yypvt[-2], yypvt[-0],
						  yypvt[-1]);}
		break;
	case 559:{
			yyval = 0;
		}
		break;
	case 560:{
			yyval = make_node(nod_flag, 0, NULL);
		}
		break;
	case 561:{
			yyval = 0;
		}
		break;
	case 562:{
			yyval = yypvt[-0];
		}
		break;
	case 563:{
			yyval = 0;
		}
		break;
	case 564:{
			yyval = yypvt[-0];
		}
		break;
	case 565:{
			yyval = make_node(nod_flag, 0, NULL);
		}
		break;
	case 566:{
			yyval = make_node(nod_select_expr, e_sel_count,
							  yypvt[-6], yypvt[-5], yypvt[-4], yypvt[-3],
							  yypvt[-2], yypvt[-1], yypvt[-0], NULL);
		}
		break;
	case 567:{
			yyval = make_node(nod_flag, 0, NULL);
		}
		break;
	case 568:{
			yyval = 0;
		}
		break;
	case 569:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 570:{
			yyval = 0;
		}
		break;
	case 572:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 574:{
			yyval = make_node(nod_alias, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 575:{
			yyval = make_node(nod_alias, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 576:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 578:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 581:{
			yyval =
				make_node(nod_join, (int) e_join_count, yypvt[-5], yypvt[-4],
						  yypvt[-2], yypvt[-0]);
		} break;
	case 582:{
			yyval = yypvt[-1];
		}
		break;
	case 583:{
			yyval = make_node(nod_rel_proc_name,
							  (int) e_rpn_count, yypvt[-2], yypvt[-0],
							  yypvt[-1]);} break;
	case 584:{
			yyval =
				make_node(nod_rel_proc_name, (int) e_rpn_count, yypvt[-1],
						  NULL, yypvt[-0]);
		} break;
	case 585:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 586:{
			yyval = NULL;
		}
		break;
	case 588:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 592:{
			yyval = make_node(nod_relation_name,
							  (int) e_rln_count, yypvt[-1], yypvt[-0]);
		} break;
	case 593:{
			yyval = make_node(nod_relation_name,
							  (int) e_rln_count, yypvt[-0], NULL);} break;
	case 594:{
			yyval = make_node(nod_join_inner, (int) 0, NULL);
		} break;
	case 595:{
			yyval = make_node(nod_join_left, (int) 0, NULL);
		} break;
	case 596:{
			yyval = make_node(nod_join_left, (int) 0, NULL);
		} break;
	case 597:{
			yyval = make_node(nod_join_right, (int) 0, NULL);
		} break;
	case 598:{
			yyval = make_node(nod_join_right, (int) 0, NULL);
		} break;
	case 599:{
			yyval = make_node(nod_join_full, (int) 0, NULL);
		} break;
	case 600:{
			yyval = make_node(nod_join_full, (int) 0, NULL);
		} break;
	case 601:{
			yyval = make_node(nod_join_inner, (int) 0, NULL);
		} break;
	case 602:{
			yyval = make_list(yypvt[-0]);
		}
		break;
	case 603:{
			yyval = 0;
		}
		break;
	case 605:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 607:{
			yyval =
				make_node(nod_collate, e_coll_count, (NOD) yypvt[-0],
						  yypvt[-2]);}
		break;
	case 608:{
			yyval = yypvt[-0];
		}
		break;
	case 609:{
			yyval = 0;
		}
		break;
	case 610:{
			yyval = yypvt[-0];
		}
		break;
	case 611:{
			yyval = 0;
		}
		break;
	case 612:{
			yyval = yypvt[-0];
		}
		break;
	case 613:{
			yyval = 0;
		}
		break;
	case 614:{
			yyval =
				make_node(nod_plan_expr, 2, yypvt[-3], make_list(yypvt[-1]));}
		break;
	case 615:{
			yyval = 0;
		}
		break;
	case 616:{
			yyval = make_node(nod_merge, (int) 0, NULL);
		} break;
	case 617:{
			yyval = make_node(nod_merge, (int) 0, NULL);
		} break;
	case 618:{
			yyval = 0;
		}
		break;
	case 619:{
			yyval = 0;
		}
		break;
	case 621:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 622:{
			yyval =
				make_node(nod_plan_item, 2, make_list(yypvt[-1]), yypvt[-0]);}
		break;
	case 625:{
			yyval = make_node(nod_list, 2, yypvt[-1], yypvt[-0]);
		}
		break;
	case 626:{
			yyval = make_node(nod_natural, (int) 0, NULL);
		} break;
	case 627:{
			yyval = make_node(nod_index, 1, make_list(yypvt[-1]));
		}
		break;
	case 628:{
			yyval = make_node(nod_index_order, 1, yypvt[-0]);
		}
		break;
	case 630:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 631:{
			yyval = make_node(nod_insert, e_ins_count,
							  yypvt[-5], make_list(yypvt[-4]),
							  make_list(yypvt[-1]), NULL);
		}
		break;
	case 632:{
			yyval =
				make_node(nod_insert, e_ins_count, yypvt[-2], yypvt[-1], NULL,
						  yypvt[-0]);
		}
		break;
	case 634:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 637:{
			yyval =
				make_node(nod_delete, e_del_count, yypvt[-1], yypvt[-0],
						  NULL);} break;
	case 638:{
			yyval =
				make_node(nod_delete, e_del_count, yypvt[-1], NULL,
						  yypvt[-0]);} break;
	case 639:{
			yyval = make_node(nod_cursor, e_cur_count, yypvt[-0], NULL, NULL);
		}
		break;
	case 642:{
			yyval = make_node(nod_update, e_upd_count,
							  yypvt[-3], make_list(yypvt[-1]), yypvt[-0],
							  NULL);}
		break;
	case 643:{
			yyval = make_node(nod_update, e_upd_count,
							  yypvt[-3], make_list(yypvt[-1]), NULL,
							  yypvt[-0]);}
		break;
	case 645:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 646:{
			yyval = make_node(nod_assign, 2, yypvt[-0], yypvt[-2]);
		}
		break;
	case 649:{
			yyval =
				make_node(nod_get_segment, e_blb_count, yypvt[-4], yypvt[-2],
						  yypvt[-1], yypvt[-0]);
		}
		break;
	case 650:{
			yyval =
				make_node(nod_put_segment, e_blb_count, yypvt[-4], yypvt[-2],
						  yypvt[-1], yypvt[-0]);
		}
		break;
	case 651:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 652:{
			yyval = make_node(nod_list, 2, NULL, yypvt[-0]);
		}
		break;
	case 656:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG);
		}
		break;
	case 657:{
			yyval = yypvt[-0];
		}
		break;
	case 659:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG);
		}
		break;
	case 662:{
			yyval = NULL;
		}
		break;
	case 663:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 665:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 667:{
			yyval = make_node(nod_field_name, (int) e_fln_count,
							  yypvt[-2], yypvt[-0]);
		} break;
	case 668:{
			yyval = make_node(nod_field_name, (int) e_fln_count,
							  yypvt[-2], NULL);} break;
	case 669:{
			yyval = make_node(nod_field_name, (int) e_fln_count,
							  NULL, yypvt[-0]);} break;
	case 671:{
			yyval = make_node(nod_or, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 672:{
			yyval = make_node(nod_and, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 673:{
			yyval = make_node(nod_not, 1, yypvt[-0]);
		}
		break;
	case 684:{
			yyval = yypvt[-1];
		}
		break;
	case 685:{
			yyval = make_node(nod_eql, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 686:{
			yyval = make_node(nod_lss, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 687:{
			yyval = make_node(nod_gtr, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 688:{
			yyval = make_node(nod_geq, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 689:{
			yyval = make_node(nod_leq, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 690:{
			yyval = make_node(nod_leq, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 691:{
			yyval = make_node(nod_geq, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 692:{
			yyval = make_node(nod_neq, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 693:{
			yyval = make_node(nod_eql_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 694:{
			yyval = make_node(nod_lss_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 695:{
			yyval = make_node(nod_gtr_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 696:{
			yyval = make_node(nod_geq_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 697:{
			yyval = make_node(nod_leq_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 698:{
			yyval = make_node(nod_leq_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 699:{
			yyval = make_node(nod_geq_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 700:{
			yyval = make_node(nod_neq_all, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 701:{
			yyval = make_node(nod_eql_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 702:{
			yyval = make_node(nod_lss_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 703:{
			yyval = make_node(nod_gtr_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 704:{
			yyval = make_node(nod_geq_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 705:{
			yyval = make_node(nod_leq_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 706:{
			yyval = make_node(nod_leq_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 707:{
			yyval = make_node(nod_geq_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 708:{
			yyval = make_node(nod_neq_any, 2, yypvt[-5], yypvt[-1]);
		}
		break;
	case 711:{
			yyval =
				make_node(nod_between, 3, yypvt[-4], yypvt[-2], yypvt[-0]);}
		break;
	case 712:{
			yyval = make_node(nod_not, 1, make_node(nod_between,
													3, yypvt[-5], yypvt[-2],
													yypvt[-0]));
		}
		break;
	case 713:{
			yyval = make_node(nod_like, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 714:{
			yyval =
				make_node(nod_not, 1,
						  make_node(nod_like, 2, yypvt[-3], yypvt[-0]));}
		break;
	case 715:{
			yyval = make_node(nod_like, 3, yypvt[-4], yypvt[-2], yypvt[-0]);
		}
		break;
	case 716:{
			yyval = make_node(nod_not, 1, make_node(nod_like,
													3, yypvt[-5], yypvt[-2],
													yypvt[-0]));
		}
		break;
	case 717:{
			yyval = make_node(nod_eql_any, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 718:{
			yyval =
				make_node(nod_not, 1,
						  make_node(nod_eql_any, 2, yypvt[-3], yypvt[-0]));}
		break;
	case 719:{
			yyval = make_node(nod_containing, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 720:{
			yyval =
				make_node(nod_not, 1,
						  make_node(nod_containing, 2, yypvt[-3], yypvt[-0]));
		}
		break;
	case 721:{
			yyval = make_node(nod_starting, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 722:{
			yyval =
				make_node(nod_not, 1,
						  make_node(nod_starting, 2, yypvt[-3], yypvt[-0]));
		}
		break;
	case 723:{
			yyval = make_node(nod_starting, 2, yypvt[-3], yypvt[-0]);
		}
		break;
	case 724:{
			yyval =
				make_node(nod_not, 1,
						  make_node(nod_starting, 2, yypvt[-4], yypvt[-0]));
		}
		break;
	case 725:{
			yyval = make_node(nod_exists, 1, yypvt[-1]);
		}
		break;
	case 726:{
			yyval = make_node(nod_singular, 1, yypvt[-1]);
		}
		break;
	case 727:{
			yyval = make_node(nod_missing, 1, yypvt[-2]);
		}
		break;
	case 728:{
			yyval =
				make_node(nod_not, 1, make_node(nod_missing, 1, yypvt[-3]));}
		break;
	case 729:{
			yyval = make_list(yypvt[-1]);
		}
		break;
	case 730:{
			yyval = yypvt[-1];
		}
		break;
	case 731:{
			yyval = make_node(nod_select_expr, e_sel_count,
							  yypvt[-6], make_list(yypvt[-5]), yypvt[-4],
							  yypvt[-3], yypvt[-2], yypvt[-1], yypvt[-0],
							  NULL);}
		break;
	case 732:{
			yyval = make_node(nod_select_expr, e_sel_count,
							  yypvt[-6], make_list(yypvt[-5]), yypvt[-4],
							  yypvt[-3], yypvt[-2], yypvt[-1], yypvt[-0],
							  MAKE_constant((STR) 1, CONSTANT_SLONG));
		}
		break;
	case 740:{
			yyval = make_node(nod_negate, 1, yypvt[-0]);
		}
		break;
	case 741:{
			yyval = yypvt[-0];
		}
		break;
	case 742:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_add2, 2, yypvt[-2], yypvt[-0]);
			else
				yyval = make_node(nod_add, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 743:{
			yyval = make_node(nod_concatenate, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 744:{
			yyval =
				make_node(nod_collate, e_coll_count, (NOD) yypvt[-0],
						  yypvt[-2]);}
		break;
	case 745:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_subtract2, 2, yypvt[-2], yypvt[-0]);
			else
				yyval = make_node(nod_subtract, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 746:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_multiply2, 2, yypvt[-2], yypvt[-0]);
			else
				yyval = make_node(nod_multiply, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 747:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_divide2, 2, yypvt[-2], yypvt[-0]);
			else
				yyval = make_node(nod_divide, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 748:{
			yyval = yypvt[-1];
		}
		break;
	case 749:{
			yyval = yypvt[-1];
		}
		break;
	case 750:{
			yyval = make_node(nod_user_name, 0, NULL);
		}
		break;
	case 751:{
			yyval = make_node(nod_dbkey, 1, NULL);
		}
		break;
	case 752:{
			yyval = make_node(nod_dbkey, 1, yypvt[-2]);
		}
		break;
	case 753:{
			yyval = make_node(nod_dom_value, 0, NULL);
		}
		break;
	case 754:{
			yyval = yypvt[-0];
		}
		break;
	case 755:{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
						  gds_arg_number, client_dialect,
						  gds_arg_string, "DATE", 0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
						  gds_arg_number, db_dialect,
						  gds_arg_string, "DATE", 0);
			yyval = make_node(nod_current_date, 0, NULL);
		}
		break;
	case 756:{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
						  gds_arg_number, client_dialect,
						  gds_arg_string, "TIME", 0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
						  gds_arg_number, db_dialect,
						  gds_arg_string, "TIME", 0);
			yyval = make_node(nod_current_time, 0, NULL);
		}
		break;
	case 757:{
			yyval = make_node(nod_current_timestamp, 0, NULL);
		}
		break;
	case 758:{
			yyval = make_node(nod_array, 2, yypvt[-3], make_list(yypvt[-1]));
		}
		break;
	case 760:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 762:{
			yyval = make_node(nod_negate, 1, yypvt[-0]);
		}
		break;
	case 763:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_STRING);
		}
		break;
	case 764:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SLONG);
		}
		break;
	case 765:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_DOUBLE);
		}
		break;
	case 766:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SINT64);
		}
		break;
	case 767:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_SINT64);
		}
		break;
	case 769:{
			yyval = MAKE_str_constant((STR) yypvt[-0], att_charset);
		}
		break;
	case 770:{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
						  gds_arg_number, client_dialect,
						  gds_arg_string, "DATE", 0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
						  gds_arg_number, db_dialect,
						  gds_arg_string, "DATE", 0);
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_DATE);
		}
		break;
	case 771:{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
						  gds_arg_number, client_dialect,
						  gds_arg_string, "TIME", 0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
						  gds_arg_number, db_dialect,
						  gds_arg_string, "TIME", 0);
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_TIME);
		}
		break;
	case 772:{
			yyval = MAKE_constant((STR) yypvt[-0], CONSTANT_TIMESTAMP);
		}
		break;
	case 776:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 777:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 778:{
			yyval = make_node(nod_list, 2, yypvt[-2], yypvt[-0]);
		}
		break;
	case 779:{
			yyval = make_node(nod_parameter, 0, NULL);
		}
		break;
	case 780:{
			yyval = make_node(nod_user_name, 0, NULL);
		}
		break;
	case 781:{
			yyval = yypvt[-0];
		}
		break;
	case 782:{
			((STR) yypvt[-0])->str_charset = (TEXT *) yypvt[-1];
			yyval = yypvt[-0];
		}
		break;
	case 784:{
			yyval = (NOD) - (SLONG) yypvt[-0];
		}
		break;
	case 785:{
			if ((SLONG) yypvt[-0] > SHRT_POS_MAX)
				yyabandon(-842, isc_expec_short);
			/* Short integer expected */
			yyval = yypvt[-0];
		}
		break;
	case 786:{
			if ((SLONG) yypvt[-0] > SHRT_NEG_MAX)
				yyabandon(-842, isc_expec_short);
			/* Short integer expected */
			yyval = yypvt[-0];
		}
		break;
	case 787:{
			if ((SLONG) yypvt[-0] == 0)
				yyabandon(-842, isc_expec_positive);
			/* Positive number expected */
			yyval = yypvt[-0];
		}
		break;
	case 788:{
			if ((SLONG) yypvt[-0] > SHRT_UNSIGNED_MAX)
				yyabandon(-842, isc_expec_ushort);
			/* Unsigned short integer expected */
			yyval = yypvt[-0];
		}
		break;
	case 790:{
			yyval = (NOD) - (SLONG) yypvt[-0];
		}
		break;
	case 791:{
			yyval = yypvt[-0];
		}
		break;
	case 792:{
			yyval = make_node(nod_agg_count, 0, NULL);
		}
		break;
	case 793:{
			yyval = make_node(nod_agg_count, 1, yypvt[-1]);
		}
		break;
	case 794:{
			yyval = make_flag_node(nod_agg_count,
								   NOD_AGG_DISTINCT, 1, yypvt[-1]);}
		break;
	case 795:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_agg_total2, 1, yypvt[-1]);
			else
				yyval = make_node(nod_agg_total, 1, yypvt[-1]);
		}
		break;
	case 796:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_flag_node(nod_agg_total2,
									   NOD_AGG_DISTINCT, 1, yypvt[-1]);
			else
				yyval = make_flag_node(nod_agg_total,
									   NOD_AGG_DISTINCT, 1, yypvt[-1]);
		}
		break;
	case 797:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_agg_average2, 1, yypvt[-1]);
			else
				yyval = make_node(nod_agg_average, 1, yypvt[-1]);
		}
		break;
	case 798:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_flag_node(nod_agg_average2,
									   NOD_AGG_DISTINCT, 1, yypvt[-1]);
			else
				yyval = make_flag_node(nod_agg_average,
									   NOD_AGG_DISTINCT, 1, yypvt[-1]);
		}
		break;
	case 799:{
			yyval = make_node(nod_agg_min, 1, yypvt[-1]);
		}
		break;
	case 800:{
			yyval = make_node(nod_agg_min, 1, yypvt[-1]);
		}
		break;
	case 801:{
			yyval = make_node(nod_agg_max, 1, yypvt[-1]);
		}
		break;
	case 802:{
			yyval = make_node(nod_agg_max, 1, yypvt[-1]);
		}
		break;
	case 803:{
			yyval = make_node(nod_cast, e_cast_count, yypvt[-1], yypvt[-3]);
		}
		break;
	case 804:{
			yyval = make_node(nod_upcase, 1, yypvt[-1]);
		}
		break;
	case 805:{
			if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				yyval = make_node(nod_gen_id2, 2, yypvt[-3], yypvt[-1]);
			else
				yyval = make_node(nod_gen_id, 2, yypvt[-3], yypvt[-1]);
		}
		break;
	case 806:{
			yyval =
				make_node(nod_extract, e_extract_count, yypvt[-3], yypvt[-1]);}
		break;
	case 807:{
			yyval = make_node(nod_udf, 2, yypvt[-3], yypvt[-1]);
		}
		break;
	case 808:{
			yyval = make_node(nod_udf, 1, yypvt[-2]);
		}
		break;
	case 809:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_year),
							  CONSTANT_SLONG);}
		break;
	case 810:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_month),
							  CONSTANT_SLONG);}
		break;
	case 811:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_day),
							  CONSTANT_SLONG);} break;
	case 812:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_hour),
							  CONSTANT_SLONG);}
		break;
	case 813:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_minute),
							  CONSTANT_SLONG);}
		break;
	case 814:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_second),
							  CONSTANT_SLONG);}
		break;
	case 815:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_weekday),
							  CONSTANT_SLONG);}
		break;
	case 816:{
			yyval =
				MAKE_constant(reinterpret_cast < str * >(blr_extract_yearday),
							  CONSTANT_SLONG);}
		break;
	case 819:{
			yyval = make_node(nod_null, 0, NULL);
		}
		break;
	}
	goto yystack;				/* reset registers in driver code */
}


} // extern "C"
