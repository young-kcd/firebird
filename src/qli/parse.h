/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		par.h
 *	DESCRIPTION:	Parser definitions
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

#ifndef _QLI_PARSE_H_
#define _QLI_PARSE_H_

#define MAXSYMLEN	256

#define MATCH(kw) 	PAR_match (kw)

/* Keywords */

typedef enum kwwords {
    KW_none = 0,
#include "../qli/symbols.h"
    KW_continuation
} KWWORDS;

/* Token block, used to hold a lexical token. */

typedef enum tok_t {
    tok_ident, 
    tok_number, 
    tok_quoted, 
    tok_punct,
    tok_eol,
    tok_eof
} TOK_T;

typedef struct tok {
    struct blk	tok_header;
    TOK_T 	tok_type;	/* type of token */
    struct sym	*tok_symbol;	/* hash block if recognized */
    KWWORDS	tok_keyword;	/* keyword number, if recognized */
    SLONG	tok_position;	/* byte number in input stream */
    USHORT	tok_length;
    struct tok	*tok_next;
    struct tok	*tok_prior;
    TEXT	tok_string [2];
} *TOK ; 

/* Input line control */

enum line_t {
    line_stdin,
    line_blob,
    line_file,
    line_string,
    line_edit
};

typedef struct line {
    struct blk	line_header;
    struct line	*line_next;
    struct dbb	*line_database;
    USHORT	line_size;
    USHORT	line_length;
    TEXT	*line_ptr;
    SLONG	line_position;
    FRBRD	*line_source;			/* File or blob handle */
    enum line_t	line_type;
    TEXT	line_data [256];
    TEXT	line_source_name [2];
} *LINE;

#ifdef PARSER_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

EXTERN TOK	QLI_token;
EXTERN LINE	QLI_line;                  
EXTERN TEXT	*QLI_prompt;

#undef EXTERN

#endif /* _QLI_PARSE_H_ */
