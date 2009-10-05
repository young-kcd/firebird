/*
 *	PROGRAM:	Language Preprocessor
 *	MODULE:		parse.h
 *	DESCRIPTION:	Common parser definitions
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

#ifndef _GPRE_PARSE_H_
#define _GPRE_PARSE_H_

typedef enum kwwords {
	KW_none = 0,
#include "../gpre/words.h"
	KW_max
} KWWORDS;

#include "../gpre/gpre.h"

#define MATCH(keyword) 		MSC_match(keyword)
#define KEYWORD(kw)		((int) token.tok_keyword == (int) kw)
#define ADVANCE_TOKEN		PAR_get_token ()
#define SYNTAX_ERROR		CPR_s_error

/* Token block, used to hold a lexical token. */

enum tok_t {
	tok_ident,
	tok_number,
	tok_quoted,
	tok_punct,
	tok_introducer,
	tok_dblquoted
};

typedef struct tok {
	enum tok_t tok_type;		/* type of token */
	struct sym *tok_symbol;		/* hash block if recognized */
	KWWORDS tok_keyword;		/* keyword number, if recognized */
	SLONG tok_position;			/* byte number in input stream */
	USHORT tok_length;
	USHORT tok_white_space;
	SCHAR tok_string[MAXSYMLEN];
	USHORT tok_first;			/* first token in a statement */
	struct sym *tok_charset;	/* Character set of token */
} *TOK;

#define TOK_LEN sizeof (struct tok)

#ifdef PARSER_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

EXTERN struct tok token;

#undef EXTERN

#endif /* _GPRE_PARSE_H_ */
