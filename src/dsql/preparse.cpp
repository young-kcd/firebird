/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		preparse.c
 *	DESCRIPTION:	Dynamic SQL pre parser / parser on client side. 
 *			This module will probably change to a YACC parser.
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

#include "firebird.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../dsql/chars.h"
#include "../dsql/prepa_proto.h"
#include "../dsql/utld_proto.h"
#include "../jrd/gds_proto.h"

#define PP_CREATE	0
#define PP_DATABASE	1
#define PP_SCHEMA	2
#define PP_PAGE_SIZE	3
#define PP_USER		4
#define PP_PASSWORD	5
#define PP_PAGESIZE	6
#define PP_LENGTH	7
#define PP_PAGES	8
#define PP_PAGE		9
#define PP_SET		10
#define PP_NAMES	11


#define MAX_TOKEN_SIZE	1024


extern "C" {


/* use STUFF_DPB in place of STUFF to avoid confusion with BLR STUFF
   macro defined in dsql.h */

#define STUFF_DPB(blr)	{*dpb++ = (SCHAR)(blr);}
#define STUFF_DPB_INT(blr)	{STUFF_DPB (blr); STUFF_DPB ((blr) >> 8); STUFF_DPB ((blr) >> 16); STUFF_DPB ((blr) >> 24);}

static void generate_error(ISC_STATUS *, SCHAR *, SSHORT, SSHORT);
static SSHORT get_next_token(SCHAR **, SCHAR *, SCHAR *, USHORT *);
static SSHORT get_token(ISC_STATUS *, SSHORT, BOOLEAN, SCHAR **, SCHAR *,
						SCHAR *, USHORT *);

typedef struct pp_table {
	SCHAR symbol[10];
	SSHORT length;
	SSHORT code;
} PP_TABLE;

static const PP_TABLE pp_symbols[] = {
	{"CREATE", 6, PP_CREATE},
	{"DATABASE", 8, PP_DATABASE},
	{"SCHEMA", 6, PP_SCHEMA},
	{"PAGE_SIZE", 9, PP_PAGE_SIZE},
	{"USER", 4, PP_USER},
	{"PASSWORD", 8, PP_PASSWORD},
	{"PAGESIZE", 8, PP_PAGESIZE},
	{"LENGTH", 6, PP_LENGTH},
	{"PAGES", 5, PP_PAGES},
	{"PAGE", 4, PP_PAGE},
	{"SET", 3, PP_SET},
	{"NAMES", 5, PP_NAMES},
	{"", 0, 0}
};

/* define the tokens */

#define NO_MORE_TOKENS		-1
#define TOKEN_TOO_LONG		-2
#define UNEXPECTED_END_OF_COMMAND -3
#define UNEXPECTED_TOKEN	-4
#define STRING			257
#define NUMERIC			258
#define SYMBOL			259


/**
  
 	PREPARSE_execute
  
    @brief

    @param user_status
    @param db_handle
    @param trans_handle
    @param stmt_length
    @param stmt
    @param stmt_eaten
    @param dialect

 **/
int DLL_EXPORT PREPARSE_execute(
								ISC_STATUS * user_status,
								FRBRD ** db_handle,
								FRBRD ** trans_handle,
USHORT stmt_length, SCHAR * stmt, BOOLEAN * stmt_eaten, USHORT dialect)
{
	TEXT file_name[MAX_TOKEN_SIZE + 1];
	SCHAR *token, *dpb_array, *dpb, *ch, *stmt_end;
	SLONG page_size = 0;
	USHORT dpb_len = 0, token_length;
	SSHORT i, l, result;
	BOOLEAN matched, get_out;
	ISC_STATUS_ARRAY temp_status;
	FRBRD *temp_db_handle = NULL;

	token = (SCHAR *) gds__alloc((SLONG) MAX_TOKEN_SIZE + 1);
/* FREE: by return(s) in this procedure */
	if (!token) {				/* NOMEM: */
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_virmemexh;
		user_status[2] = isc_arg_end;
		return FALSE;
	}

	if (!stmt_length)
		stmt_length = strlen(stmt);
	stmt_end = stmt + stmt_length;

	if (get_token(user_status, SYMBOL, FALSE, &stmt, stmt_end, token,
				  &token_length) ||
		token_length != pp_symbols[PP_CREATE].length ||
		strcmp(token, pp_symbols[PP_CREATE].symbol)) {
		gds__free((SLONG *) token);
		return FALSE;
	}

	if (get_token(user_status, SYMBOL, FALSE, &stmt, stmt_end, token,
				  &token_length) ||
		(token_length != pp_symbols[PP_DATABASE].length &&
		 token_length != pp_symbols[PP_SCHEMA].length) ||
		(strcmp(token, pp_symbols[PP_DATABASE].symbol) &&
		 strcmp(token, pp_symbols[PP_SCHEMA].symbol))) {
		gds__free((SLONG *) token);
		return FALSE;
	}

	if (get_token(user_status, STRING, FALSE, &stmt, stmt_end, token,
				  &token_length)) {
		gds__free((SLONG *) token);
		return TRUE;
	}

	strcpy(file_name, token);
	*stmt_eaten = FALSE;
	dpb = dpb_array = (SCHAR *) gds__alloc((SLONG) 2 * MAX_TOKEN_SIZE + 25);
/* FREE: by following return(s) in this procedure */
	if (!dpb_array) {			/* NOMEM: */
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_virmemexh;
		user_status[2] = isc_arg_end;
		gds__free((SLONG *) token);
		return TRUE;
	}

	STUFF_DPB(gds_dpb_version1);
	STUFF_DPB(isc_dpb_overwrite);
	STUFF_DPB(1);
	STUFF_DPB(0);
	STUFF_DPB(isc_dpb_sql_dialect);
	STUFF_DPB(4);
	STUFF_DPB_INT(dialect);
	do {
		result = get_next_token(&stmt, stmt_end, token, &token_length);
		if (result == NO_MORE_TOKENS) {
			*stmt_eaten = TRUE;
			break;
		}
		else if (result < 0)
			break;

		matched = FALSE;
		for (i = 3; pp_symbols[i].length && !matched; i++) {
			if (token_length == pp_symbols[i].length &&
				!strcmp(token, pp_symbols[i].symbol)) {

				get_out = FALSE;
				switch (pp_symbols[i].code) {
				case PP_PAGE_SIZE:
				case PP_PAGESIZE:
					if (get_token(user_status, '=', TRUE, &stmt, stmt_end,
								  token, &token_length) ||
						get_token(user_status, NUMERIC, FALSE, &stmt,
								  stmt_end, token, &token_length)) {
						get_out = TRUE;
						break;
					}
					page_size = atol(token);
					STUFF_DPB(gds_dpb_page_size);
					STUFF_DPB(4);
					STUFF_DPB_INT(page_size);
					matched = TRUE;
					break;

				case PP_USER:
					if (get_token(user_status, STRING, FALSE, &stmt, stmt_end,
								  token, &token_length)) {
						get_out = TRUE;
						break;
					}

					STUFF_DPB(gds_dpb_user_name);
					l = token_length;
					STUFF_DPB(l);
					ch = token;
					while (*ch)
						STUFF_DPB(*ch++);
					matched = TRUE;
					break;

				case PP_PASSWORD:
					if (get_token(user_status, STRING, FALSE, &stmt, stmt_end,
								  token, &token_length)) {
						get_out = TRUE;
						break;
					}

					STUFF_DPB(gds_dpb_password);
					l = token_length;
					STUFF_DPB(l);
					ch = token;
					while (*ch)
						STUFF_DPB(*ch++);
					matched = TRUE;
					break;

				case PP_SET:
					if (get_token(user_status, SYMBOL, FALSE, &stmt, stmt_end,
								  token, &token_length) ||
						token_length != pp_symbols[PP_NAMES].length ||
						strcmp(token, pp_symbols[PP_NAMES].symbol) ||
						get_token(user_status, STRING, FALSE, &stmt, stmt_end,
								  token, &token_length)) {
						get_out = TRUE;
						break;
					}

					STUFF_DPB(gds_dpb_lc_ctype);
					l = token_length;
					STUFF_DPB(l);
					ch = token;
					while (*ch)
						STUFF_DPB(*ch++);
					matched = TRUE;
					break;

				case PP_LENGTH:
					/* Skip a token for value */

					if (get_token(user_status, '=', TRUE, &stmt, stmt_end,
								  token, &token_length) ||
						get_token(user_status, NUMERIC, FALSE, &stmt,
								  stmt_end, token, &token_length)) {
						get_out = TRUE;
						break;
					}

					matched = TRUE;
					break;

				case PP_PAGE:
				case PP_PAGES:
					matched = TRUE;
					break;
				}

				if (get_out) {
					gds__free((SLONG *) dpb_array);
					gds__free((SLONG *) token);
					return TRUE;
				}
			}
		}

	} while (matched);

	dpb_len = dpb - dpb_array;

/* This code is because 3.3 server does not recognize isc_dpb_overwrite. */
	if (!isc_attach_database(user_status, 0,
							 file_name, GDS_REF(temp_db_handle),
							 dpb_len, dpb_array) ||
		user_status[1] != gds_io_error) {
		if (!user_status[1])
			isc_detach_database(temp_status, &temp_db_handle);
		if (!user_status[1] || user_status[1] == gds_bad_db_format) {
			user_status[0] = gds_arg_gds;
			user_status[1] = gds_io_error;
			user_status[2] = gds_arg_string;
			user_status[3] = (ISC_STATUS) "open";
			user_status[4] = gds_arg_string;
			user_status[5] = (ISC_STATUS) file_name;
			user_status[6] = gds_arg_gds;
			user_status[7] = gds_db_or_file_exists;
			user_status[8] = gds_arg_end;
			UTLD_save_status_strings(user_status);
		}
		if (dpb_array)
			gds__free((SLONG *) dpb_array);
		gds__free((SLONG *) token);
		return TRUE;
	}

	isc_create_database(user_status, 0, file_name,
						(db_handle), dpb_len,
						dpb_array, 0);

	if (dpb_array)
		gds__free((SLONG *) dpb_array);
	gds__free((SLONG *) token);

	return TRUE;
}


/**
  
 	generate_error
  
    @brief

    @param user_status
    @param token
    @param error
    @param result

 **/
static void generate_error(
						   ISC_STATUS * user_status,
						   SCHAR * token, SSHORT error, SSHORT result)
{
	TEXT err_string[MAX_TOKEN_SIZE + 3];
	SSHORT length;

	user_status[0] = gds_arg_gds;
	user_status[1] = gds_sqlerr;
	user_status[2] = gds_arg_number;
	user_status[3] = -104;
	user_status[4] = gds_arg_gds;

	switch (error) {
	case UNEXPECTED_END_OF_COMMAND:
		user_status[5] = gds_command_end_err;
		user_status[6] = gds_arg_end;
		break;

	case UNEXPECTED_TOKEN:
	case TOKEN_TOO_LONG:
		if (result) {
			err_string[0] = (TEXT) result;
			strcpy(err_string + 1, token);
			length = strlen(token);
			err_string[length + 1] = (TEXT) result;
			err_string[length + 2] = '\0';
		}
		else
			strcpy(err_string, token);
		user_status[5] = gds_token_err;
		user_status[6] = gds_arg_gds;
		user_status[7] = gds_random;
		user_status[8] = gds_arg_string;
		user_status[9] = (ISC_STATUS) err_string;
		user_status[10] = gds_arg_end;
		UTLD_save_status_strings(user_status);
		break;
	}
}


/**
  
 	get_next_token
  
    @brief

    @param stmt
    @param stmt_end
    @param token
    @param token_length

 **/
static SSHORT get_next_token(
							 SCHAR ** stmt,
							 SCHAR * stmt_end,
							 SCHAR * token, USHORT * token_length)
{
	SCHAR *s, *p, *token_end, *start_of_token;
	UCHAR c, class_;
	SSHORT length;

	*token_length = 0;
	s = *stmt;

	for (;;) {
		if (s >= stmt_end) {
			*stmt = s;
			return NO_MORE_TOKENS;
		}
		c = *s++;
		if (c == '/' && s < stmt_end && *s == '*') {
			s++;
			while (s < stmt_end) {
				c = *s++;
				if (c == '*' && s < stmt_end && *s == '/')
					break;
			}
			s++;
			continue;
		}
		class_ = classes[c];
		if (!(class_ & CHR_WHITE))
			break;
	}

/* At this point c contains character and class contains character class.
   s is pointing to next character. */

	start_of_token = s - 1;

/* In here we handle only 4 cases, STRING, INTEGER, arbitrary
   SYMBOL and single character punctuation. */

/* token can be up to MAX_TOKEN_SIZE and 1 is for null termination */

	p = token;
	token_end = token + MAX_TOKEN_SIZE + 1;
	if (class_ & CHR_QUOTE) {
		for (; p < token_end; p++) {
			if (s >= stmt_end)
				return UNEXPECTED_END_OF_COMMAND;

			/* *s is quote - if next != quote we're at the end */

			if ((*s == c) && ((++s == stmt_end) || (*s != c)))
				break;
			*p = *s++;
		}
		*stmt = s;
		if (p >= token_end) {
			/* '=' used as then there is no place for null termination */

			*token_length = MAX_TOKEN_SIZE;
			token[MAX_TOKEN_SIZE] = '\0';
			return TOKEN_TOO_LONG;
		}
		*token_length = (s - start_of_token) - 2;
		*p = '\0';
		return STRING;
	}

/* Is it an integer? */

	if (class_ & CHR_DIGIT) {
		for (; s < stmt_end && (classes[c = *s] & CHR_DIGIT); ++s);
		length = (s - start_of_token);
		*stmt = s;
		if (length > MAX_TOKEN_SIZE) {
			memcpy(token, start_of_token, MAX_TOKEN_SIZE);
			token[MAX_TOKEN_SIZE] = '\0';
			*token_length = MAX_TOKEN_SIZE;
			return TOKEN_TOO_LONG;
		}
		memcpy(token, start_of_token, length);
		token[length] = '\0';
		*token_length = length;
		return NUMERIC;
	}

/* Is is a symbol */

	if (class_ & CHR_LETTER) {
		*p++ = UPPER(c);
		for (; s < stmt_end && classes[*s] & CHR_IDENT && p < token_end; s++) {
			*p++ = UPPER(*s);
		}

		/* what happend to token size check. It went to hell */

		*stmt = s;
		if (p >= token_end) {
			*token_length = MAX_TOKEN_SIZE;
			token[MAX_TOKEN_SIZE] = '\0';
			return TOKEN_TOO_LONG;
		}
		*token_length = s - start_of_token;
		*p = 0;
		return SYMBOL;
	}

/* What remains at this point for us is the single character punctuation.*/

	*stmt = s;

	return (c == ';' ? NO_MORE_TOKENS : c);
}


/**
  
 	get_token
  
    @brief

    @param status
    @param token_type
    @param optional
    @param stmt
    @param stmt_end
    @param token
    @param token_length

 **/
static SSHORT get_token(
						ISC_STATUS * status,
						SSHORT token_type,
						BOOLEAN optional,
						SCHAR ** stmt,
SCHAR * stmt_end, SCHAR * token, USHORT * token_length)
{
	SSHORT result;
	SCHAR *temp_stmt;

	temp_stmt = *stmt;
	result = get_next_token(&temp_stmt, stmt_end, token, token_length);

	switch (result) {
	case NO_MORE_TOKENS:
		*stmt = temp_stmt;
		generate_error(status, token, UNEXPECTED_END_OF_COMMAND, 0);
		return FB_FAILURE;

	case UNEXPECTED_END_OF_COMMAND:
	case TOKEN_TOO_LONG:
		*stmt = temp_stmt;

		/* generate error here */

		generate_error(status, token, result, 0);
		return FB_FAILURE;
	}

/* Some token was found */

	if (result == token_type) {
		*stmt = temp_stmt;
		return FB_SUCCESS;
	}

	if (optional)
		return FB_SUCCESS;

/* generate error here and return failure; */

	*stmt = temp_stmt;
	generate_error(status, token, UNEXPECTED_TOKEN,
				   (result == STRING) ? *(temp_stmt - 1) : 0);
	return FB_FAILURE;
}


} // extern "C"
