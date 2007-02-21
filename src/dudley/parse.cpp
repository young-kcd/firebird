/*
 *	PROGRAM:	JRD data definition language parser
 *	MODULE:		parse.c
 *	DESCRIPTION:	Statement parser
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#define PARSER_MAIN
#include "../jrd/gds.h"
#include "../jrd/flags.h"
#include "../dudley/ddl.h"
#include "../dudley/parse.h"
#include "../jrd/acl.h"
#include "../intl/kanji.h"
#include "../wal/wal.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/exe_proto.h"
#include "../dudley/expr_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/lex_proto.h"
#include "../dudley/parse_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"

#define PROMPT			"GDEF> "
#define CONTINUATION		"CON>  "
#define GDS_NAME_LEN		31

#define MAX_DIMENSION		16
#ifdef FLINT_CACHE
#define MIN_CACHE_BUFFERS	250
#define DEF_CACHE_BUFFERS	1000
#endif

#define STUFF(c)	*p++ = c
#define STUFF_WORD(c)	{STUFF (c); STUFF (c >> 8);}
#define STUFF_LONG(c)	{STUFF (c); STUFF (c >> 8); STUFF (c >> 16); STUFF (c >> 24); }

typedef enum {
	obj_database,
	obj_relation,
	obj_view,
	obj_field,
	obj_index,
	obj_security_class,
	obj_trigger,
	obj_file,
	obj_none,
	obj_function,
	obj_type,
	obj_filter,
	obj_shadow,
	obj_generator,
} OBJ_T;

/* this table translates between the two types of types */

static TRG_T trig_table[] = {
	trg_type_none,
	trg_type_none,
	trg_store,
	trg_post_store,
	trg_modify,
	trg_post_modify,
	trg_type_none,
	trg_type_none,
	trg_pre_erase,
	trg_erase
};

jmp_buf parse_env;

extern TEXT *DDL_prompt;

static BOOLEAN check_filename(SYM, USHORT);
static SYM copy_symbol(SYM);
static DUDLEY_FLD create_global_field(DUDLEY_FLD);
#ifdef FLINT_CACHE
static FIL define_cache(void);
#endif
static void define_database(enum act_t);
static void define_field(void);
static FIL define_file(void);
static void define_filter(void);
static void define_function(void);
static void define_generator(void);
static void define_index(void);
static FIL define_log_file(USHORT);
static void define_old_trigger(void);
static void define_relation(void);
static void define_security_class(void);
static void define_shadow(void);
static void define_trigger(void);
static void define_type(void);
static void define_view(void);
static void drop_filter(void);
static void drop_function(void);
static void drop_gfield(void);
static void drop_index(void);
static void drop_relation(void);
static void drop_security_class(void);
static void drop_shadow(void);
static void drop_trigger(void);
static void drop_type(void);
static void end_text(TXT);
static SYM gen_trigger_name(TRG_T, DUDLEY_REL);
static int get_system_flag(void);
static void get_trigger_attributes(int *, int *, int *);
static void grant_user_privilege(void);
static DUDLEY_CTX lookup_context(SYM, LLS);
static DUDLEY_FLD lookup_global_field(DUDLEY_FLD);
static ACT make_action(enum act_t, DBB);
static ACT make_computed_field(DUDLEY_FLD);
static DUDLEY_CTX make_context(TEXT *, DUDLEY_REL);
static ACT make_global_field(DUDLEY_FLD);
static void mod_old_trigger(void);
static void modify_field(void);
static void modify_index(void);
static void modify_relation(void);
static void modify_security_class(void);
static void modify_trigger(void);
static void modify_trigger_action(DUDLEY_TRG, DUDLEY_REL);
static void modify_type(void);
static void modify_view(void);
static BOOLEAN parse_action(void);
static void parse_array(DUDLEY_FLD);
static TXT parse_description(void);
static void parse_end(void);
static DUDLEY_FLD parse_field(DUDLEY_FLD);
static void parse_field_clauses(DUDLEY_FLD);
static void parse_field_dtype(DUDLEY_FLD);
static void parse_field_subtype(DUDLEY_FLD);
static FUNCARG parse_function_arg(FUNC, USHORT *);
static SCE parse_identifier(void);
static OBJ_T parse_object(void);
static int parse_page_size(void);
static SLONG parse_privileges(void);
static void revoke_user_privilege(void);
static SLONG score_entry(SCE);
static DUDLEY_NOD set_generator(void);
static void sort_out_attributes(DUDLEY_TRG, SLONG, SLONG, SLONG);
static TXT start_text(void);
static void validate_field(DUDLEY_FLD);

static LLS local_context;


void PARSE_actions(void)
{
/**************************************
 *
 *	P A R S E_ a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Parse actions in a loop until end of file.
 *
 **************************************/

	parse_action();
	while (!DDL_eof && DDL_errors < 20) {
		if ((!database || !database->dbb_handle) && (!DDL_drop_database)) {
			DDL_err(111, NULL, NULL, NULL, NULL, NULL);	/* msg 111: no database declared */
			if (database) {
				gds__free(database);
				database = NULL;
			}
			if (DDL_interactive)
				parse_action();
			else {
				DDL_err(112, NULL, NULL, NULL, NULL, NULL);	/* msg 112: ceasing processing */
				break;
			}
		}
		else {
			DDL_drop_database = FALSE;
			parse_action();
		}
	}
}


void PARSE_error( USHORT number, TEXT * arg1, TEXT * arg2)
{
/**************************************
 *
 *	P A R S E _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Generate a syntax error.
 *
 **************************************/

	DDL_err(number, arg1, arg2, NULL, NULL, NULL);
	Firebird::status_exception::raise(TRUE);
}


FUNC PARSE_function( int flag)
{
/**************************************
 *
 *	P A R S E _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Get the function block associated with the current token.  Also
 *	advance the token.  Create a new function requested.  
 *
 **************************************/
	SYM symbol;
	FUNC function;

	if (DDL_token.tok_type != tok_ident)
		PARSE_error(113, DDL_token.tok_string, 0);	/* msg 113: expected function, encountered \"%s\" */

	symbol = HSH_typed_lookup(DDL_token.tok_string, DDL_token.tok_length,
							  SYM_function);

	if (symbol && (function = (FUNC) symbol->sym_object) &&
		function->func_database == database) {
		LEX_token();
		return function;
	}

	if (flag)
		PARSE_error(114, DDL_token.tok_string, 0);	/* msg 114: expected function, encountered \"%s\" */
	else {
		function = (FUNC) DDL_alloc(FUNC_LEN);
		function->func_name = symbol = PARSE_symbol(tok_ident);
		symbol->sym_type = SYM_function;
		symbol->sym_object = (DUDLEY_CTX) function;
		HSH_insert(symbol);

		if (!(function->func_database = database))
			PARSE_error(111, 0, 0);	/* msg 111: no database declared */
	}
	return function;
}


enum kwwords PARSE_keyword(void)
{
/**************************************
 *
 *	P A R S E _ k e y w o r d
 *
 **************************************
 *
 * Functional description
 *	Get a real token and return the keyword number.
 *
 **************************************/
	SYM symbol;

	LEX_real();

	for (symbol = DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword)
			return (enum kwwords) symbol->sym_keyword;

	return KW_none;
}


DUDLEY_NOD PARSE_make_list(LLS stack)
{
/**************************************
 *
 *	P A R S E _ m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Dump a stack of junk into a list node.  Best count
 *	them first.
 *
 **************************************/
	DUDLEY_NOD node, *ptr;
	LLS temp;
	USHORT count;

	temp = stack;
	count = 0;

	while (temp) {
		count++;
		temp = temp->lls_next;
	}

	node = SYNTAX_NODE(nod_list, count);
	ptr = &node->nod_arg[count];

	while (stack)
		*--ptr = LLS_POP(&stack);

	return node;
}


DUDLEY_NOD PARSE_make_node(enum nod_t type, USHORT count)
{
/**************************************
 *
 *	P A R S E _ m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Allocate and initialize a syntax node of given type.
 *
 **************************************/
	DUDLEY_NOD node;

	node = (DUDLEY_NOD) DDL_alloc(NOD_LEN(count));
	node->nod_type = type;
	node->nod_count = count;

	return node;
}


int PARSE_match( enum kwwords keyword)
{
/**************************************
 *
 *	P A R S E _ m a t c h
 *
 **************************************
 *
 * Functional description
 *	Test the current token for a particular keyword.  If so, advanced
 *	the token stream.
 *
 **************************************/
	SYM symbol;

	if (KEYWORD(keyword)) {
		LEX_token();
		return TRUE;
	}

	for (symbol = DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword &&
			symbol->sym_keyword == (int) keyword) {
			LEX_token();
			return TRUE;
		}

	return FALSE;
}


int PARSE_number(void)
{
/**************************************
 *
 *	P A R S E _ n u m b e r
 *
 **************************************
 *
 * Functional description
 *	Parse the next token as a number and return its value.
 *
 **************************************/
	SSHORT negate;
	int number;

	negate = MATCH(KW_MINUS);

	if (DDL_token.tok_type != tok_number)
		PARSE_error(115, DDL_token.tok_string, 0);	/* msg 115: expected number, encountered \"%s\" */

	number = atoi(DDL_token.tok_string);
	LEX_token();

	if (negate)
		return -number;

	return number;
}


DUDLEY_REL PARSE_relation(void)
{
/**************************************
 *
 *	P A R S E _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Get the relation block associated with the current token.
 *	If the relatio was defined before, use the old definition.
 *	If not, create a new one.  Somebody else will decide whether
 *	this one is worth keeping.
 *
 **************************************/
	SYM symbol;
	DUDLEY_REL relation;

	if (DDL_token.tok_type != tok_ident)
		PARSE_error(116, DDL_token.tok_string, 0);	/* msg 116: expected relation name, encountered \"%s\" */

	symbol = HSH_typed_lookup(DDL_token.tok_string, DDL_token.tok_length,
							  SYM_relation);
	if (symbol && (relation = (DUDLEY_REL) symbol->sym_object) &&
		relation->rel_database == database) {
		LEX_token();
		return relation;
	}

	relation = (DUDLEY_REL) DDL_alloc(REL_LEN);
	relation->rel_name = symbol = PARSE_symbol(tok_ident);
	symbol->sym_type = SYM_relation;
	symbol->sym_object = (DUDLEY_CTX) relation;

	if (!(relation->rel_database = database))
		PARSE_error(111, 0, 0);	/* msg 111: no database declared */

	relation->rel_next = database->dbb_relations;
	database->dbb_relations = relation;

	return relation;
}


SYM PARSE_symbol(enum tok_t type)
{
/**************************************
 *
 *	P A R S E _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Swallow the current token as a symbol.
 *
 **************************************/
	SYM symbol;
	TEXT c, *p, *q;
	USHORT l;

	if (DDL_token.tok_type != type)
		switch (type) {
		case tok_ident:
			PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		case tok_quoted:
			PARSE_error(118, DDL_token.tok_string, 0);	/* msg 118: expected quoted string, encountered \"%s\" */
		default:
			PARSE_error(119, DDL_token.tok_string, 0);	/* msg 119: expected symbol, encountered \"%s\" */
		}
	l = DDL_token.tok_length;
	q = DDL_token.tok_string;

	if (type == tok_quoted) {
		if (l < 2)
			PARSE_error(118, DDL_token.tok_string, 0);	/* msg 118: expected quoted string, encountered \"%s\" */
		q++;
		l -= 2;
	}

	symbol = (SYM) DDL_alloc(SYM_LEN + l);
	symbol->sym_length = l;
	p = symbol->sym_string = symbol->sym_name;

	if (l)
		if (type == tok_ident)
			do {
				c = *q++;
				*p++ = UPPER(c);
			}
			while (--l);
		else
			do
				*p++ = *q++;
			while (--l);

	LEX_token();

	return symbol;
}


static BOOLEAN check_filename( SYM name, USHORT decnet_flag)
{
/**************************************
 *
 *	c h e c k _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *	Make sure that a file path doesn't contain an
 *	inet node name.
 *
 **************************************/
	USHORT l;
	TEXT file_name[256], *p, *q;

	if (!(l = name->sym_length))
		return TRUE;
	l = MIN(l, sizeof(file_name) - 1);
	for (p = file_name, q = name->sym_string; l--; *p++ = *q++);
	*p = 0;

	for (p = file_name; *p; p++)
		if (p[0] == ':' && p[1] == ':')
			if (!decnet_flag)
				return FALSE;
			else {
				for (p = file_name; *p;)
					if (*p++ == '^')
						return FALSE;
				return TRUE;
			}

	return !ISC_check_if_remote(file_name, FALSE);
}


static SYM copy_symbol( SYM old_name)
{
/**************************************
 *
 *	c o p y _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *    Create a new symbol block for
 *    the same symbol as an existing block.
 *    This seems dumber than it is, because
 *    the implicit creation of global fields
 *    from local definitions depends on it as
 *    does the implicit invocation of same-named
 *    global fields.
 *
 *    We'll just leave the type blank for now. 
 *
 **************************************/
	SYM new_name;

	new_name = (SYM) DDL_alloc(SYM_LEN + old_name->sym_length);
	new_name->sym_length = old_name->sym_length;
	new_name->sym_string = new_name->sym_name;
	strcpy(new_name->sym_name, old_name->sym_name);

	return new_name;
}


static DUDLEY_FLD create_global_field( DUDLEY_FLD local_field)
{
/**************************************
 *
 *	c r e a t e _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Copy a field block so we can
 *	have separate blocks for global
 *	and local fields, even when the
 *	global field is defined implicitly.
 *
 *	This creates a new field and symbol
 *	block.
 *
 **************************************/
	DUDLEY_FLD global_field;
	SYM old_name, new_name;

	global_field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
	global_field->fld_dtype = local_field->fld_dtype;
	global_field->fld_length = local_field->fld_length;
	global_field->fld_scale = local_field->fld_scale;
	global_field->fld_segment_length = local_field->fld_segment_length;
	global_field->fld_sub_type = local_field->fld_sub_type;
	global_field->fld_has_sub_type = local_field->fld_has_sub_type;
	global_field->fld_dimension = local_field->fld_dimension;
	global_field->fld_system = local_field->fld_system;
	global_field->fld_database = local_field->fld_database;
	global_field->fld_query_name = local_field->fld_query_name;
	global_field->fld_query_header = local_field->fld_query_header;
	global_field->fld_edit_string = local_field->fld_edit_string;
	global_field->fld_computed = local_field->fld_computed;
	global_field->fld_missing = local_field->fld_missing;
	global_field->fld_default = local_field->fld_default;
	global_field->fld_validation = local_field->fld_validation;
	global_field->fld_description = local_field->fld_description;
	global_field->fld_compute_src = local_field->fld_compute_src;
	global_field->fld_valid_src = local_field->fld_valid_src;
	global_field->fld_ranges = local_field->fld_ranges;

	if (local_field->fld_source)
		global_field->fld_name = local_field->fld_source;
	else {
		old_name = local_field->fld_name;
		global_field->fld_name = new_name = copy_symbol(old_name);
		new_name->sym_type = SYM_global;
		new_name->sym_object = (DUDLEY_CTX) global_field;
		local_field->fld_source = new_name;
	}

	global_field->fld_flags = local_field->fld_flags & ~fld_local;
	local_field->fld_source_field = global_field;
	local_field->fld_flags |= fld_local;

	return global_field;
}

#ifdef FLINT_CACHE
static FIL define_cache(void)
{
/**************************************
 *
 *	d e f i n e _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	Add a shared cache to an existing database.
 *
 **************************************/
	FIL file;

	file = (FIL) DDL_alloc(sizeof(struct fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, FALSE))
		PARSE_error(322, 0, 0);	/* msg 322: a node name is not permitted in a shared cache file name */

	if (MATCH(KW_LENGTH)) {
		if ((file->fil_length = PARSE_number()) < MIN_CACHE_BUFFERS)
			PARSE_error(339, (TEXT *) MIN_CACHE_BUFFERS, 0);	/* msg 339: minimum of %d cache pages required */
		MATCH(KW_PAGES);
	}
	else
		file->fil_length = DEF_CACHE_BUFFERS;	/* default cache buffers */

	return file;
}
#endif

static void define_database( enum act_t action_type)
{
/**************************************
 *
 *	d e f i n e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Parse the tail of a DEFINE, MODIFY, or DELETE DATABASE statement.
 *
 **************************************/
	FIL file;
	SYM symbol;

	if (database)
		DDL_error_abort(0, 120, NULL, NULL, NULL, NULL, NULL);	/* msg 120: GDEF processes only one database at a time */

	database = (DBB) DDL_alloc(DBB_LEN);
	database->dbb_name = PARSE_symbol(tok_quoted);
	DB_file_name = database->dbb_name->sym_name;
	database->dbb_grp_cmt_wait = -1;	/* Initialized for default value */

/* parse options for the database parameter block */

	while (TRUE) {
		if (MATCH(KW_LENGTH)) {
			database->dbb_length = PARSE_number();
			MATCH(KW_PAGES);
		}
		else if (MATCH(KW_USER)) {
			symbol = PARSE_symbol(tok_quoted);
			DDL_default_user = symbol->sym_name;
		}
		else if (MATCH(KW_PASSWORD)) {
			symbol = PARSE_symbol(tok_quoted);
			DDL_default_password = symbol->sym_name;
		}
		else
			break;
	}

	if (action_type == act_d_database) {
		if (MATCH(KW_CASCADE))
			database->dbb_flags |= DBB_cascade;
		EXE_drop_database(database);
		DDL_drop_database = TRUE;
		return;
	}

/* parse add/drop items */

	while (TRUE) {
		MATCH(KW_ADD);
		if (KEYWORD(KW_DESCRIPTION))
			database->dbb_description = parse_description();
		else if (MATCH(KW_SECURITY_CLASS))
			database->dbb_security_class = PARSE_symbol(tok_ident);
		else if (MATCH(KW_DROP)) {
			if (MATCH(KW_DESCRIP))
				database->dbb_flags |= DBB_null_description;
			else if (MATCH(KW_SECURITY_CLASS))
				database->dbb_flags |= DBB_null_security_class;
			else if (MATCH(KW_LOG_FILE)) {
				if ((database->dbb_flags & DBB_log_default)
					|| (database->dbb_logfiles)) PARSE_error(337, 0, 0);

				database->dbb_flags |= DBB_drop_log;
			}
			else if (MATCH(KW_CASCADE))
				database->dbb_flags |= DBB_cascade;
#ifdef FLINT_CACHE
			else if (MATCH(KW_CACHE))
				database->dbb_flags |= DBB_drop_cache;
#endif /* FLINT_CACHE */
			else
				PARSE_error(121, 0, 0);	/* msg 121 only SECURITY_CLASS, DESCRIPTION and CACHE can be dropped */
		}
		else if (MATCH(KW_FILE)) {
			file = define_file();
			file->fil_next = database->dbb_files;
			database->dbb_files = file;
		}
		else if (MATCH(KW_PAGE_SIZE)) {
			if (action_type == act_m_database)
				PARSE_error(122, 0, 0);	/* msg 122: PAGE_SIZE can not be modified */
			database->dbb_page_size = parse_page_size();
		}
		else if (MATCH(KW_CHECK_POINT_LEN)) {
			MATCH(KW_EQUALS);
			database->dbb_chkptlen = PARSE_number();
		}
		else if (MATCH(KW_NUM_LOG_BUFS)) {
			MATCH(KW_EQUALS);
			database->dbb_numbufs = PARSE_number();
		}
		else if (MATCH(KW_LOG_BUF_SIZE)) {
			MATCH(KW_EQUALS);
			database->dbb_bufsize = PARSE_number();
		}
		else if (MATCH(KW_GROUP_COMMIT_WAIT)) {
			MATCH(KW_EQUALS);
			database->dbb_grp_cmt_wait = PARSE_number();
		}
		else if (MATCH(KW_LOG_FILE)) {
			if ((database->dbb_flags & DBB_log_default)
				|| (database->dbb_logfiles)) PARSE_error(338, 0, 0);

			if (database->dbb_flags & DBB_drop_log)
				PARSE_error(337, 0, 0);

			if (MATCH(KW_LEFT_PAREN)) {
				database->dbb_flags |= DBB_log_preallocated;
				while (TRUE) {
					file = define_log_file(DBB_log_preallocated);
					file->fil_next = database->dbb_logfiles;
					database->dbb_logfiles = file;
					if (!MATCH(KW_COMMA))
						break;
				}

				if (!MATCH(KW_RIGHT_PAREN))
					PARSE_error(341, DDL_token.tok_string, 0);	/* msg 341: expected comma or ')', encountered \"%s\" */

				if (MATCH(KW_OVERFLOW))
					database->dbb_overflow = define_log_file(DBB_log_serial);
				else
					PARSE_error(340, 0, 0);
			}
			else if (MATCH(KW_BASE_NAME)) {
				database->dbb_flags |= DBB_log_serial;
				database->dbb_logfiles = define_log_file(DBB_log_serial);
			}
			else
				database->dbb_flags |= DBB_log_default;
		}
#ifdef FLINT_CACHE
		else if (MATCH(KW_CACHE))
			database->dbb_cache_file = define_cache();
#endif /* FLINT_CACHE */
		else
			break;
	}

	parse_end();

	if (action_type == act_c_database) {
		database->dbb_flags |= DBB_create_database;
		EXE_create_database(database);
	}
	else {
		EXE_modify_database(database);
	}

	make_action(action_type, database);
}


static void define_field(void)
{
/**************************************
 *
 *	d e f i n e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FIELD statement.
 *
 **************************************/
	DUDLEY_FLD field;

	field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
	parse_field(field);
	field->fld_database = database;

	if (!field->fld_dtype)
		PARSE_error(123, 0, 0);	/* msg 123: data type required for global field */

	if (field->fld_security_class)
		PARSE_error(124, 0, 0);	/* msg 124: Security class can appear only on local field references */

	parse_end();

	make_global_field(field);
}


static FIL define_file(void)
{
/**************************************
 *
 *	d e f i n e _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a new file to an existing database.
 *
 **************************************/
	FIL file;

	file = (FIL) DDL_alloc(sizeof(struct fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, FALSE))
		PARSE_error(297, 0, 0);	/* msg 297: A node name is not permitted in a shadow or secondary file name */

	while (TRUE) {
		if (MATCH(KW_LENGTH)) {
			file->fil_length = PARSE_number();
			MATCH(KW_PAGES);
		}
		else if (MATCH(KW_STARTS)) {
			MATCH(KW_AT);
			MATCH(KW_PAGE);
			file->fil_start = PARSE_number();
		}
		else
			break;
	}

	return file;
}


static void define_filter(void)
{
/**************************************
 *
 *	d e f i n e _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FILTER statement.
 *
 **************************************/
	FILTER filter;

	if (DDL_token.tok_type != tok_ident)
		PARSE_error(126, DDL_token.tok_string, 0);	/* msg 126: expected filter name, encountered \"%s\" */

	filter = (FILTER) DDL_alloc(sizeof(struct filter));
	filter->filter_name = PARSE_symbol(tok_ident);

	while (TRUE) {
		if (KEYWORD(KW_DESCRIPTION))
			filter->filter_description = parse_description();
		else if (MATCH(KW_INPUT_TYPE))
			filter->filter_input_sub_type = PARSE_number();
		else if (MATCH(KW_OUTPUT_TYPE))
			filter->filter_output_sub_type = PARSE_number();
		else if (MATCH(KW_FUNCTION_MODULE_NAME))
			filter->filter_module_name = PARSE_symbol(tok_quoted);
		else if (MATCH(KW_FUNCTION_ENTRY_POINT))
			filter->filter_entry_point = PARSE_symbol(tok_quoted);
		else
			break;
	}

	if (!filter->filter_entry_point)
		PARSE_error(127, 0, 0);	/* msg 127: Filter entry point must be specified */

#if (defined WIN_NT)
	if (!filter->filter_module_name)
		PARSE_error(128, 0, 0);	/* msg 128: Filter module name must be specified */
#endif

	parse_end();
	make_action(act_a_filter, (DBB) filter);
}


static void define_function(void)
{
/**************************************
 *
 *	d e f i n e _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE FUNCTION statement.
 *
 **************************************/
	FUNC function;
	FUNCARG function_arg;
	SSHORT position;

	function = PARSE_function(FALSE);

	while (TRUE) {
		if (KEYWORD(KW_DESCRIPTION))
			function->func_description = parse_description();
		else if (MATCH(KW_FUNCTION_MODULE_NAME))
			function->func_module_name = PARSE_symbol(tok_quoted);
		else if (MATCH(KW_FUNCTION_ENTRY_POINT))
			function->func_entry_point = PARSE_symbol(tok_quoted);
		else if (MATCH(KW_QUERY_NAME))
			function->func_query_name = PARSE_symbol(tok_ident);
		else
			break;
	}

	if (!function->func_entry_point)
		PARSE_error(130, 0, 0);	/* msg 130: Function entry point must be specified */

#if (defined WIN_NT)
	if (!function->func_module_name)
		PARSE_error(131, 0, 0);	/* msg 131: Function module name must be specified */
#endif

/* Gobble function arguments */

	position = 1;

	while (TRUE) {
		if (KEYWORD(KW_SEMI))
			break;
		function_arg = parse_function_arg(function, (USHORT*) &position);
		function_arg->funcarg_funcname = function->func_name;
		make_action(act_a_function_arg, (DBB) function_arg);
		if (!MATCH(KW_COMMA))
			break;
	}

	if (!KEYWORD(KW_SEMI))
		PARSE_error(132, DDL_token.tok_string, 0);	/* msg 132: expected comma or semi-colon, encountered \"%s\" */

	make_action(act_a_function, (DBB) function);
}


static void define_generator(void)
{
/**************************************
 *
 *	d e f i n e _ g e n e r a t o r
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE GENERATOR statement.
 *
 **************************************/
	SYM symbol;

	if (DDL_token.tok_type != tok_ident)
		PARSE_error(274, DDL_token.tok_string, 0);	/* msg 274: expected generator name, encountered \"%s\" */

	symbol = PARSE_symbol(tok_ident);
	parse_end();
	make_action(act_a_generator, (DBB) symbol);
}


static void define_index(void)
{
/**************************************
 *
 *	d e f i n e _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Define a new index.
 *
 **************************************/
	LLS stack;
	DUDLEY_IDX index;
	SYM index_name, rel_name, *ptr;
	TXT description;
	SSHORT count, unique, inactive, descending;

	index_name = PARSE_symbol(tok_ident);
	MATCH(KW_FOR);
	rel_name = PARSE_symbol(tok_ident);

	unique = inactive = descending = FALSE;
	description = NULL;

	while (TRUE) {
		if (MATCH(KW_DUPLICATES))
			unique = FALSE;
		else if (MATCH(KW_UNIQUE))
			unique = TRUE;
		else if (KEYWORD(KW_DESCRIPTION))
			description = parse_description();
		else if (MATCH(KW_ACTIVE))
			inactive = FALSE;
		else if (MATCH(KW_INACTIVE))
			inactive = TRUE;
		else if (MATCH(KW_ASCENDING))
			descending = IDX_type_none;
		else if (MATCH(KW_DESCENDING))
			descending = IDX_type_descend;
		else
			break;
	}

	count = 0;

	stack = NULL;

	while (TRUE) {
		LLS_PUSH(PARSE_symbol(tok_ident), &stack);
		count++;
		if (!MATCH(KW_COMMA))
			break;
	}

	if (!KEYWORD(KW_SEMI))
		PARSE_error(135, DDL_token.tok_string, 0);	/* msg 135: expected comma or semi-colon, encountered \"%s\" */
	index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(count));
	index->idx_count = count;
	index->idx_name = index_name;
	index->idx_relation = rel_name;
	index->idx_unique = unique;
	index->idx_description = description;
	index->idx_inactive = inactive;
	index->idx_type = descending;

	ptr = &index->idx_field[count];

	while (stack)
		*--ptr = (SYM) LLS_POP(&stack);

	make_action(act_a_index, (DBB) index);
}


static FIL define_log_file( USHORT log_type)
{
/**************************************
 *
 *	d e f i n e _ l o g _ f i l e
 *
 **************************************
 *
 * Functional description
 *	define a log file 
 *
 **************************************/
	FIL file;

	file = (FIL) DDL_alloc(sizeof(struct fil));
	file->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(file->fil_name, FALSE))
		PARSE_error(297, 0, 0);	/* msg 297: A node name is not permitted in a shadow or secondary file name */

	while (TRUE) {
		if (MATCH(KW_SIZE)) {
			MATCH(KW_EQUALS);
			file->fil_length = PARSE_number();
		}
		else if (MATCH(KW_RAW_PARTITIONS)) {
			if (log_type != DBB_log_preallocated)
				PARSE_error(332, 0, 0);	/* msg 332: Partitions not supported in series of log file specification */
			MATCH(KW_EQUALS);
			file->fil_partitions = PARSE_number();
			file->fil_raw = LOG_raw;
		}
		else
			break;
	}

/* Check for the specified length of the log file */

	if (file->fil_partitions) {
		if (!file->fil_length)
			PARSE_error(335, file->fil_name->sym_string, 0);	/* msg 335: Total length of the partitioned
																   log <logname> must be specified */
		if (PARTITION_SIZE(OneK * file->fil_length, file->fil_partitions) <
			(OneK * MIN_LOG_LENGTH))
			PARSE_error(334, file->fil_name->sym_string, 0);
		/* msg 334: log partition size too small for <logname> */
	}
	else {
		if ((file->fil_length) && (file->fil_length < MIN_LOG_LENGTH))
			PARSE_error(336, (TEXT *) MIN_LOG_LENGTH, 0);
		/* msg 336: Minimum log length should be MIN_LOG_LENGTH Kbytes */
	}

	return file;
}


static void define_old_trigger(void)
{
/**************************************
 *
 *	d e f i n e _ o l d _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse old style trigger definition
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_TRG trigger;
	SYM name;

	trigger = NULL;
	relation = PARSE_relation();

	while (!MATCH(KW_END_TRIGGER)) {
		trigger = (DUDLEY_TRG) DDL_alloc(TRG_LEN);
		trigger->trg_relation = relation;

		if (MATCH(KW_STORE))
			trigger->trg_type = trg_store;
		else if (MATCH(KW_MODIFY))
			trigger->trg_type = trg_modify;
		else if (MATCH(KW_ERASE))
			trigger->trg_type = trg_erase;
		else
			PARSE_error(136, DDL_token.tok_string, 0);	/* msg 136: expected STORE, MODIFY, ERASE, END_TRIGGER, encountered \"%s\" */
		MATCH(KW_COLON);

		name = gen_trigger_name(trigger->trg_type, relation);

		trigger->trg_name = name;
		trigger->trg_source = start_text();
		trigger->trg_statement = EXPR_statement();
		end_text(trigger->trg_source);

		make_action(act_a_trigger, (DBB) trigger);
	}

	parse_end();

	if (trigger) {
		name->sym_type = SYM_trigger;
		name->sym_object = (DUDLEY_CTX) trigger;
		HSH_insert(name);
	}
}


static void define_relation(void)
{
/**************************************
 *
 *	d e f i n e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_FLD field, global;
	SSHORT position;
	ACT rel_actions, action;

	relation = PARSE_relation();
	if (!(relation->rel_flags & rel_marked_for_delete) &&
		((relation->rel_flags & rel_marked_for_creation)
		 || EXE_relation(relation))) PARSE_error(137,
												 relation->rel_name->sym_string, 0);	/* msg 137: relation %s already exists  */

	if (MATCH(KW_EXTERNAL_FILE)) {
		relation->rel_filename = PARSE_symbol(tok_quoted);
		if (!check_filename(relation->rel_filename, TRUE))
			PARSE_error(298, 0, 0);	/* msg 298: A non-Decnet node name is not permitted in an external file name */
	}

	rel_actions = action = make_action(act_a_relation, (DBB) relation);
	action->act_flags |= ACT_ignore;
	position = 1;

	while (TRUE)
		if (KEYWORD(KW_DESCRIPTION))
			relation->rel_description = parse_description();
		else if (MATCH(KW_SECURITY_CLASS))
			relation->rel_security_class = PARSE_symbol(tok_ident);
		else if (MATCH(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
		}
		else
			break;

/* Gobble fields */

	while (TRUE) {
		MATCH(KW_ADD);
		MATCH(KW_FIELD);
		field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
		parse_field(field);
		field->fld_relation = relation;
		field->fld_database = database;
		if (field->fld_computed)
			make_computed_field(field);
		else if (!(field->fld_flags & fld_local))
			make_global_field(create_global_field(field));
		else if (global = lookup_global_field(field)) {
			field->fld_dtype = global->fld_dtype;
			field->fld_length = global->fld_length;
			field->fld_scale = global->fld_scale;
			field->fld_segment_length = global->fld_segment_length;
			field->fld_sub_type = global->fld_sub_type;
			field->fld_has_sub_type = global->fld_has_sub_type;
		}
		if (field->fld_flags & fld_explicit_position)
			position = field->fld_position + 1;
		else
			field->fld_position = position++;
		field->fld_flags |= fld_explicit_position;
		field->fld_name->sym_type = SYM_field;
		HSH_insert(field->fld_name);
		action = make_action(act_a_field, (DBB) field);
		action->act_flags |= ACT_ignore;
		if (!MATCH(KW_COMMA))
			break;
	}

	if (!KEYWORD(KW_SEMI))
		PARSE_error(138, DDL_token.tok_string, 0);	/* msg 138: expected comma or semi-colon, encountered \"%s\" */

/* We started off by assuming that the relation and field actions that
   have been defined would have to be ignored.  Everything has gone well so
   turn them on. */

	while (TRUE) {
		action->act_flags &= ~ACT_ignore;
		if (action == rel_actions)
			break;
		action = action->act_next;
	}

	HSH_insert(relation->rel_name);
	relation->rel_flags &= ~rel_marked_for_delete;
	relation->rel_flags |= rel_marked_for_creation;
}


static void define_security_class(void)
{
/**************************************
 *
 *	d e f i n e _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Parse an access control list.
 *
 **************************************/
	SCL class_;
	SCE element, *next;

	class_ = (SCL) DDL_alloc(sizeof(struct scl));
	class_->scl_name = PARSE_symbol(tok_ident);
	if (KEYWORD(KW_DESCRIPTION))
		class_->scl_description = parse_description();

/* Pick up entries. Use the users's order */

	while (TRUE) {
		if (!(element = parse_identifier()))
			return;
		for (next = &class_->scl_entries; *next; next = &(*next)->sce_next);
		*next = element;
		element->sce_privileges = parse_privileges();
		if (!MATCH(KW_COMMA))
			break;
	}

	parse_end();

	make_action(act_a_security, (DBB) class_);
}


static void define_shadow(void)
{
/**************************************
 *
 *	d e f i n e _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Define a shadow file to the database.
 *	Parse it as a set of normal file additions, 
 *	setting the shadow number on all files.
 *
 **************************************/
	FIL shadow, file;
	int number;

	shadow = (FIL) DDL_alloc(sizeof(struct fil));

/* get the shadow number, defaulting to 1 */

	if (DDL_token.tok_type != tok_number)
		number = 1;
	else {
		number = PARSE_number();
		if (number < 1)
			PARSE_error(139, 0, 0);	/* msg 139: shadow number must be a positive integer */
	}

/* match the keywords MANUAL or AUTO to imply whether the shadow 
   should be automatically deleted when something goes awry */

	if (MATCH(KW_MANUAL))
		shadow->fil_manual = 1;
	else
		MATCH(KW_AUTO);

	if (MATCH(KW_CONDITIONAL))
		shadow->fil_conditional = 1;

	shadow->fil_name = PARSE_symbol(tok_quoted);
	if (!check_filename(shadow->fil_name, FALSE))
		PARSE_error(297, 0, 0);	/* msg 297: A node name is not permitted in a shadow or secondary file name */

	if (MATCH(KW_LENGTH)) {
		shadow->fil_length = PARSE_number();
		MATCH(KW_PAGES);
	}

	while (TRUE) {
		if (MATCH(KW_FILE)) {
			file = define_file();
			file->fil_next = shadow;
			shadow = file;
		}
		else
			break;
	}

	parse_end();

	for (file = shadow; file; file = file->fil_next)
		file->fil_shadow_number = number;

	make_action(act_a_shadow, (DBB) shadow);
}


static void define_trigger(void)
{
/**************************************
 *
 *	d e f i n e _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse new trigger definition
 *      syntax, being a little loose about
 *	what comes when.
 *
 **************************************/
	DUDLEY_TRG trigger;
	TRGMSG trigmsg;
	int flags, trg_state, trg_sequence;
	USHORT action, end;

	if (MATCH(KW_FOR)) {
		define_old_trigger();
		return;
	}

	trigger = (DUDLEY_TRG) DDL_alloc(TRG_LEN);
	trigger->trg_name = PARSE_symbol(tok_ident);

	MATCH(KW_FOR);

	trigger->trg_relation = PARSE_relation();

	flags = trg_state = trg_sequence = 0;
	get_trigger_attributes(&flags, &trg_state, &trg_sequence);
	trigger->trg_type = trig_table[trg_state & ~trig_inact];
	trigger->trg_inactive = (trg_state & trig_inact);
	trigger->trg_sequence = trg_sequence;

	if (!(int) trigger->trg_type)	/* still none */
		PARSE_error(141, DDL_token.tok_string, 0);	/* msg 141: expected STORE, MODIFY, ERASE, encountered \"%s\" */

	action = end = FALSE;

	while (!KEYWORD(KW_SEMI)) {
		if (KEYWORD(KW_DESCRIPTION))
			trigger->trg_description = parse_description();
		else if (MATCH(KW_END_TRIGGER))
			action = end = TRUE;
		else if (!action) {
			trigger->trg_source = start_text();
			trigger->trg_statement = EXPR_statement();
			end_text(trigger->trg_source);
			action = TRUE;
		}
		else if (MATCH(KW_MESSAGE)) {
			trigmsg = (TRGMSG) DDL_alloc(sizeof(struct trgmsg));
			trigmsg->trgmsg_trg_name = trigger->trg_name;
			trigmsg->trgmsg_number = PARSE_number();
			if (trigmsg->trgmsg_number > 255)
				PARSE_error(142, (TEXT *) (trigmsg->trgmsg_number), 0);	/* msg 142: message number %d exceeds 255 */
			MATCH(KW_COLON);
			trigmsg->trgmsg_text = PARSE_symbol(tok_quoted);
			make_action(act_a_trigger_msg, (DBB) trigmsg);
			MATCH(KW_COMMA);
		}
		else {
			/* if none of the other cases were true, we must be stuck on a bum token */
			if (end)
				PARSE_error(304, DDL_token.tok_string, 0);	/* msg 304: expected message keyword, encountered \"%s\" */
			else if (!action)
				PARSE_error(305, DDL_token.tok_string, 0);	/* msg 305: expected trigger action, encountered \"%s\" */
			else
				PARSE_error(306, DDL_token.tok_string, 0);	/* msg 306: expected end_trigger or description keyword, encountered \"%s\" */
			break;
		}
	}

	parse_end();

	make_action(act_a_trigger, (DBB) trigger);
	trigger->trg_name->sym_type = SYM_trigger;
	trigger->trg_name->sym_object = (DUDLEY_CTX) trigger;
	HSH_insert(trigger->trg_name);
}


static void define_type(void)
{
/**************************************
 *
 *	d e f i n e _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a type for a field.
 *
 **************************************/
	SYM fldname;
	TYP fldtype;

	MATCH(KW_FOR);
	fldname = PARSE_symbol(tok_ident);

	while (TRUE) {
		fldtype = (TYP) DDL_alloc(sizeof(struct typ));
		fldtype->typ_field_name = fldname;
		fldtype->typ_name = PARSE_symbol(tok_ident);
		MATCH(KW_COLON);
		fldtype->typ_type = PARSE_number();
		if (KEYWORD(KW_DESCRIPTION))
			fldtype->typ_description = parse_description();
		make_action(act_a_type, (DBB) fldtype);
		if (!MATCH(KW_COMMA))
			break;
	}

	parse_end();
}


static void define_view(void)
{
/**************************************
 *
 *	d e f i n e _ v i e w
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE VIEW statement.
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_FLD field, *ptr;
	SYM symbol;
	SSHORT position;
	DUDLEY_CTX context, my_context;
	LLS contexts;
	ACT rel_actions, action;

/* Pick up relation name */

	relation = PARSE_relation();
	if (!(relation->rel_flags & rel_marked_for_delete) &&
		((relation->rel_flags & rel_marked_for_creation)
		 || EXE_relation(relation))) PARSE_error(300,
												 relation->rel_name->sym_string, 0);	/* msg 300: relation %s already exists  */

	MATCH(KW_OF);

/* Parse record selection expression */

	contexts = NULL;
	relation->rel_view_source = start_text();
	relation->rel_rse = EXPR_rse(TRUE);
	end_text(relation->rel_view_source);

/* add my context to the context stack */

	contexts = (LLS) relation->rel_rse->nod_arg[s_rse_contexts];
	my_context = make_context(0, relation);
	LLS_PUSH(my_context, &contexts);
	relation->rel_rse->nod_arg[s_rse_contexts] = (DUDLEY_NOD) contexts;

	rel_actions = action = make_action(act_a_relation, (DBB) relation);
	action->act_flags |= ACT_ignore;

/* Pick up various fields and clauses */

	while (TRUE) {
		if (KEYWORD(KW_DESCRIPTION))
			relation->rel_description = parse_description();
		else if (MATCH(KW_SECURITY_CLASS))
			relation->rel_security_class = PARSE_symbol(tok_ident);
		else if (MATCH(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
		}
		else
			break;
	}

/* Gobble fields */

	position = 1;
	ptr = &relation->rel_fields;

	while (TRUE) {
		MATCH(KW_ADD);
		MATCH(KW_FIELD);
		field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
		field->fld_flags |= fld_local;
		field->fld_relation = relation;
		*ptr = field;
		ptr = &field->fld_next;
		field->fld_name = symbol = PARSE_symbol(tok_ident);
		if (context = lookup_context(symbol, contexts)) {
			if (!MATCH(KW_DOT))
				PARSE_error(144, DDL_token.tok_string, 0);	/* msg 144: expected period, encountered \"%s\" */
			field->fld_name = field->fld_base = PARSE_symbol(tok_ident);
		}
		else {
			if (MATCH(KW_FROM)) {
				symbol = PARSE_symbol(tok_ident);
				if (!(context = lookup_context(symbol, contexts)))
					PARSE_error(145, DDL_token.tok_string, 0);
				/* msg 145: expected qualified field name, encountered \"%s\" */
				if (!MATCH(KW_DOT))
					PARSE_error(146, DDL_token.tok_string, 0);
				/* msg 146: expected period, encountered \"%s\" */
				field->fld_base = PARSE_symbol(tok_ident);
			}
			else {
				parse_field_dtype(field);
				if (field->fld_dtype == blr_cstring)
					PARSE_error(147, 0, 0);	/* msg 147: datatype cstring not supported for fields  */

				if (MATCH(KW_COMPUTED)) {
					MATCH(KW_BY);
					if (!(MATCH(KW_LEFT_PAREN)))
						PARSE_error(148, 0, 0);	/* msg 148: computed by expression must be parenthesized  */
					field->fld_compute_src = start_text();
					field->fld_computed = EXPR_value(0, 0);
					end_text(field->fld_compute_src);
					if (!(MATCH(KW_RIGHT_PAREN)))
						PARSE_error(149, 0, 0);	/* msg 149: unmatched parenthesis */
					context = my_context;
				}
				else
					PARSE_error(150, DDL_token.tok_string, 0);
				/* msg 150: expected FROM, COMPUTED, or qualified field, encountered \"%s\" */
			}
		}
		field->fld_context = context;
		parse_field_clauses(field);
		if (field->fld_computed)
			make_computed_field(field);
		if (field->fld_flags & fld_explicit_position)
			position = field->fld_position + 1;
		else
			field->fld_position = position++;
		field->fld_flags |= fld_explicit_position;
		field->fld_name->sym_type = SYM_field;
		field->fld_name->sym_object = (DUDLEY_CTX) field;
		HSH_insert(field->fld_name);
		if (!MATCH(KW_COMMA))
			break;
	}

	parse_end();

/* We started off by assuming that the relation and field actions that
   have been defined would have to be ignored.  Everything has gone well so
   turn them on. */

	while (TRUE) {
		action->act_flags &= ~ACT_ignore;
		if (action == rel_actions)
			break;
		action = action->act_next;
	}

	HSH_insert(relation->rel_name);
	relation->rel_flags &= ~rel_marked_for_delete;
	relation->rel_flags |= rel_marked_for_creation;
}


static void drop_filter(void)
{
/**************************************
 *
 *	d r o p _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) filter statement.
 *
 **************************************/
	FILTER filter;

	filter = (FILTER) DDL_alloc(sizeof(struct filter));
	filter->filter_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_filter, (DBB) filter);
}


static void drop_function(void)
{
/**************************************
 *
 *	d r o p _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) function statement.
 *
 **************************************/
	FUNC function;

	function = (FUNC) DDL_alloc(FUNC_LEN);
	function->func_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_function, (DBB) function);
}


static void drop_gfield(void)
{
/**************************************
 *
 *	d r o p _ g f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) field statement.
 *
 **************************************/
	DUDLEY_FLD field;

	field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
	field->fld_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_gfield, (DBB) field);
}


static void drop_index(void)
{
/**************************************
 *
 *	d r o p _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Parse DROP INDEX statement.
 *
 **************************************/
	DUDLEY_IDX index;

	index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(0));
	index->idx_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_index, (DBB) index);
}


static void drop_relation(void)
{
/**************************************
 *
 *	d r o p _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse DROP RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation;

	relation = PARSE_relation();
	parse_end();

	if (relation->rel_flags & rel_marked_for_creation)
		PARSE_error(303, 0, 0);	/* msg 303: A relation or view may not be defined and then deleted in a single execution of GDEF */

	relation->rel_flags |= rel_marked_for_delete;

	make_action(act_d_relation, (DBB) relation);
}


static void drop_security_class(void)
{
/**************************************
 *
 *	d r o p _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Parse DROP SECURITY_CLASS statement.
 *
 **************************************/
	SCL class_;

	class_ = (SCL) DDL_alloc(sizeof(struct scl));
	class_->scl_name = PARSE_symbol(tok_ident);
	parse_end();

	make_action(act_d_security, (DBB) class_);
}


static void drop_shadow(void)
{
/**************************************
 *
 *	d r o p _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Parse DROP SHADOW statement.
 *
 **************************************/
	int number;

	number = PARSE_number();
	parse_end();

	make_action(act_d_shadow, (DBB) number);
}


static void drop_trigger(void)
{
/**************************************
 *
 *	d r o p _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse trigger deletion
 *      (old and new trigger syntax).
 *
 **************************************/
	SYM name;
	SSHORT old_style;
	DUDLEY_REL relation;
	DUDLEY_TRG trigger;

	if (MATCH(KW_FOR)) {
		relation = PARSE_relation();
		old_style = TRUE;
	}
	else {
		old_style = FALSE;
		name = PARSE_symbol(tok_ident);
	}

	if (old_style) {
		while (!MATCH(KW_END_TRIGGER)) {
			trigger = (DUDLEY_TRG) DDL_alloc(TRG_LEN);
			if (MATCH(KW_STORE))
				trigger->trg_type = trg_store;
			else if (MATCH(KW_MODIFY))
				trigger->trg_type = trg_modify;
			else if (MATCH(KW_ERASE))
				trigger->trg_type = trg_erase;
			else
				PARSE_error(153, DDL_token.tok_string, 0);
			/* msg 153: "expected STORE, MODIFY, ERASE, or END_TRIGGER, encountered \"%s\" */
			trigger->trg_name = name =
				gen_trigger_name(trigger->trg_type, relation);
			trigger->trg_relation = relation;
			make_action(act_d_trigger, (DBB) trigger);
		}
	}
	else {
		trigger = (DUDLEY_TRG) DDL_alloc(TRG_LEN);
		trigger->trg_name = name;
		make_action(act_d_trigger, (DBB) trigger);
	}

	parse_end();
}


static void drop_type(void)
{
/**************************************
 *
 *	d r o p _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse the DROP (DELETE) type statement.
 *
 **************************************/
	SYM fldname;
	TYP fldtype;

	MATCH(KW_FOR);
	if (MATCH(KW_ALL)) {
		fldtype = (TYP) DDL_alloc(sizeof(struct typ));
		fldtype->typ_field_name = PARSE_symbol(tok_ident);
		fldtype->typ_name->sym_length = 3;
		strncpy(fldtype->typ_name->sym_string, "ALL", 3);
		make_action(act_d_type, (DBB) fldtype);
	}
	else {
		fldname = PARSE_symbol(tok_ident);
		while (TRUE) {
			fldtype = (TYP) DDL_alloc(sizeof(struct typ));
			fldtype->typ_field_name = fldname;
			fldtype->typ_name = PARSE_symbol(tok_ident);
			make_action(act_d_type,(DBB)  fldtype);
			if (!MATCH(KW_COMMA))
				break;
		}
	}
	parse_end();
}


static void end_text( TXT text)
{
/**************************************
 *
 *	e n d _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Mark end of a text description.
 *
 **************************************/

	text->txt_length =
		DDL_token.tok_position - DDL_token.tok_length - text->txt_position;

#if (defined WIN_NT)
/* the length of the text field should subtract out the 
   line feeds, since they are automatically filtered out
   when reading from a file */

	text->txt_length -= (DDL_line - text->txt_start_line);
#endif
}


static SYM gen_trigger_name( TRG_T type, DUDLEY_REL relation)
{
/**************************************
 *
 *	g e n _ t r i g g e r _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Generate a trigger name for an old style trigger.  
 *
 **************************************/
	TEXT *p, *q, *end;
	SYM symbol;

	symbol = (SYM) DDL_alloc(SYM_LEN + GDS_NAME_LEN);
	symbol->sym_string = symbol->sym_name;
	p = symbol->sym_string;
	end = p + GDS_NAME_LEN;

/* Start by copying relation name */

	q = relation->rel_name->sym_string;

	while (*q && p < end)
		*p++ = *q++;

	switch (type) {
	case trg_store:
		q = "$STORE";
		break;

	case trg_modify:
		q = "$MODIFY";
		break;

	case trg_erase:
		q = "$ERASE";
		break;

	default:
		DDL_err(156, NULL, NULL, NULL, NULL, NULL);	/* msg 156: gen_trigger_name: invalid trigger type */
	}

	while (*q && p < end)
		*p++ = *q++;

	*p = 0;

	symbol->sym_length = p - symbol->sym_string;
	return symbol;
}


static int get_system_flag(void)
{
/**************************************
 *
 *	g e t _ s y s t e m _ f l a g
 *
 **************************************
 *
 * Functional description
 *	Return the (signed) numeric system flag value.
 *
 **************************************/
	SSHORT number;

	number = PARSE_number();

	if (number == 1)
		PARSE_error(157, 0, 0);	/* msg 157: System flag value of 1 is reserved for system relations */

	return number;
}


static void get_trigger_attributes( int *flags, int *type, int *sequence)
{
/**************************************
 *
 *	g e t _ t r i g g e r _ a t t r i b u t e s
 *
 **************************************
 *
 * Functional description
 *
 *	trigger attributes include [[PRE | POST]
 *	{STORE | MODIFY | ERASE } [SEQUENCE]].
 *	All are optional on a new style modify,
 *	PRE and POST are optional in new style definitions.
 *	For STORE & MODIFY PRE is the default.
 *	For ERASE, POST is the default.
 *	
 **************************************/

	if (MATCH(KW_INACTIVE)) {
		*flags |= trg_mflag_onoff;
		*type |= trig_inact;
	}
	else if (MATCH(KW_ACTIVE))
		*flags |= trg_mflag_onoff;

	*flags |= trg_mflag_order;
	if (MATCH(KW_PRE_STORE))
		*type |= trig_sto;
	else if (MATCH(KW_PRE_MODIFY))
		*type |= trig_mod;
	else if (MATCH(KW_PRE_ERASE))
		*type |= trig_era;
	else if (!(MATCH(KW_PRE))) {
		*type |= trig_post;
		if (MATCH(KW_POST_STORE))
			*type |= trig_sto;
		else if (MATCH(KW_POST_MODIFY))
			*type |= trig_mod;
		else if (MATCH(KW_POST_ERASE))
			*type |= trig_era;
		else if (!(MATCH(KW_POST))) {
			*type &= ~trig_post;
			*flags &= ~trg_mflag_order;
		}
	}

	if (!(*type & ~(trig_inact | trig_post))) {
		if (MATCH(KW_STORE))
			*type |= trig_sto;
		else if (MATCH(KW_MODIFY))
			*type |= trig_mod;
		else if (MATCH(KW_ERASE)) {
			*type |= trig_era;
			if (!(*flags & trg_mflag_order))
				*type |= trig_post;
		}
	}


	if ((!MATCH(KW_COLON)) && ((*flags & trg_mflag_order) ||
							   (*type & (trig_sto | trig_mod | trig_era)))) {
		*sequence = PARSE_number();
		*flags |= trg_mflag_seqnum;
		MATCH(KW_COLON);
	}

	if ((*type & ~trig_inact) || (*flags & trg_mflag_order))
		*flags |= trg_mflag_type;
}


static void grant_user_privilege(void)
{
/**************************************
 *
 *	g r a n t _ u s e r _ p r i v i l e g e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL grant statement.
 *
 **************************************/
	USERPRIV upriv;
	UPFE upf;
	USRE usr;

	upriv = (USERPRIV) DDL_alloc(sizeof(struct userpriv));
	upriv->userpriv_flags = 0;

	while (TRUE) {
		/* ALL is translated to mean four individual privileges */

		if (MATCH(KW_ALL)) {
			/* optional keyword following ALL */

			MATCH(KW_PRIVILEGES);
			upriv->userpriv_flags |= USERPRIV_select;
			upriv->userpriv_flags |= USERPRIV_delete;
			upriv->userpriv_flags |= USERPRIV_insert;
			upriv->userpriv_flags |= USERPRIV_update;
			if (!MATCH(KW_ON))
				PARSE_error(159, DDL_token.tok_string, 0);	/* msg 159: expected ON, encountered \"%s\" */

			break;
		}
		else if (MATCH(KW_SELECT))
			upriv->userpriv_flags |= USERPRIV_select;
		else if (MATCH(KW_DELETE))
			upriv->userpriv_flags |= USERPRIV_delete;
		else if (MATCH(KW_INSERT))
			upriv->userpriv_flags |= USERPRIV_insert;
		else if (MATCH(KW_UPDATE)) {
			/* look for a field list for the update privilege */

			upriv->userpriv_flags |= USERPRIV_update;
			if (MATCH(KW_ON))
				break;
			if (MATCH(KW_COMMA))
				continue;
			if (!MATCH(KW_LEFT_PAREN))
				PARSE_error(313, DDL_token.tok_string, 0);	/* msg 313: expected ON or '(', encountered "%s" */

			do {
				if (KEYWORD(KW_SELECT) || KEYWORD(KW_INSERT) ||
					KEYWORD(KW_DELETE) || KEYWORD(KW_UPDATE))
					break;
				upf = (UPFE) DDL_alloc(sizeof(struct upfe));
				upf->upfe_fldname = PARSE_symbol(tok_ident);
				upf->upfe_next = upriv->userpriv_upflist;
				upriv->userpriv_upflist = upf;
			} while (MATCH(KW_COMMA));

			if (!MATCH(KW_RIGHT_PAREN))
				PARSE_error(314, DDL_token.tok_string, 0);	/* msg 314: expected ')', encountered "%s" */

			continue;
		}
		if (!MATCH(KW_COMMA)) {
			if (!MATCH(KW_ON))
				PARSE_error(159, DDL_token.tok_string, 0);	/* msg 159: expected ON, encountered \"%s\" */
			break;
		}
	}

	if (!upriv->userpriv_flags)
		PARSE_error(160, 0, 0);	/* msg 160: GRANT privilege was not specified */

	upriv->userpriv_relation = PARSE_symbol(tok_ident);
	if (!MATCH(KW_TO))
		PARSE_error(161, DDL_token.tok_string, 0);	/* msg 161: expected TO, encountered "%s" */

/* get the userlist */

	while (TRUE) {
		usr = (USRE) DDL_alloc(sizeof(struct usre));
		usr->usre_name = PARSE_symbol(tok_ident);
		usr->usre_next = upriv->userpriv_userlist;
		upriv->userpriv_userlist = usr;
		if (!MATCH(KW_COMMA))
			break;
	}

/* check for the optional WITH GRANT OPTION specification */

	if (MATCH(KW_WITH)) {
		if (!MATCH(KW_GRANT))
			PARSE_error(162, DDL_token.tok_string, 0);	/* msg 162:expected GRANT, encountered \"%s\" */
		if (!MATCH(KW_OPTION))
			PARSE_error(163, DDL_token.tok_string, 0);	/* msg 163: expected OPTION, encountered \"%s\" */
		upriv->userpriv_flags |= USERPRIV_grant;
	}

	parse_end();

	make_action(act_grant, (DBB) upriv);
}


static DUDLEY_CTX lookup_context( SYM symbol, LLS contexts)
{
/**************************************
 *
 *	l o o k u p _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Search the context stack.  If a context name is given, consider
 *	only named contexts; otherwise return the first unnamed context.
 *	In either case, if nothing matches, return NULL.
 *
 **************************************/
	DUDLEY_CTX context;
	SYM name;

/* If no name is given, look for a nameless one. */

	if (!symbol) {
		for (; contexts; contexts = contexts->lls_next) {
			context = (DUDLEY_CTX) contexts->lls_object;
			if (!context->ctx_name && !context->ctx_view_rse)
				return context;
		}
		return NULL;
	}

/* Other search by name */

	for (; contexts; contexts = contexts->lls_next) {
		context = (DUDLEY_CTX) contexts->lls_object;
		if ((name = context->ctx_name) &&
			!strcmp(name->sym_string, symbol->sym_string)) return context;
	}

	return NULL;
}


static DUDLEY_FLD lookup_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	l o o k u p _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYM symbol, name;

/* Find symbol */

	name = (field->fld_source) ? field->fld_source : field->fld_name;

	if (symbol = HSH_typed_lookup(name->sym_string, name->sym_length,
								  SYM_global))
			return (DUDLEY_FLD) symbol->sym_object;

	PARSE_error(230, name->sym_string, 0);	/* msg 230: global field %s isn't defined */

	return NULL;
}


static ACT make_action( enum act_t type, DBB object)
{
/**************************************
 *
 *	m a k e _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Allocate and link an action block.
 *
 **************************************/
	ACT action;

	action = (ACT) DDL_alloc(ACT_LEN);
	action->act_type = type;
	action->act_next = DDL_actions;
	action->act_object = object;
	action->act_line = DDL_line;
	DDL_actions = action;

	return action;
}


static ACT make_computed_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	m a k e _ c o m p u t e d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	DUDLEY_FLD computed;
	SYM symbol;
	TEXT s[64];
	USHORT i, l;

/* Start by generating a unique name */

	for (i = 1;; i++) {
		sprintf(s, "RDB$%d", i);
		l = strlen(s);
		if (!HSH_lookup(s, l))
			break;
	}

/* Make up new symbol block */

	field->fld_source = symbol = (SYM) DDL_alloc(SYM_LEN + l);
	symbol->sym_type = SYM_global;
	symbol->sym_length = l;
	symbol->sym_string = symbol->sym_name;
	strcpy(symbol->sym_name, s);

	field->fld_source_field = computed = create_global_field(field);
	symbol->sym_object = (DUDLEY_CTX) computed;
	return make_global_field(computed);
}


static DUDLEY_CTX make_context( TEXT * string, DUDLEY_REL relation)
{
/**************************************
 *
 *	m a k e _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Make context for trigger.
 *
 **************************************/
	DUDLEY_CTX context;
	SYM symbol;

	context = (DUDLEY_CTX) DDL_alloc(CTX_LEN);
	context->ctx_relation = relation;

	if (string) {
		context->ctx_name = symbol = (SYM) DDL_alloc(SYM_LEN);
		symbol->sym_object = context;
		symbol->sym_string = string;
		symbol->sym_length = strlen(string);
		symbol->sym_type = SYM_context;
	}

	return context;
}


static ACT make_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	m a k e _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYM symbol;

/* Make sure symbol is unique */

	symbol = field->fld_name;
	symbol->sym_object = (DUDLEY_CTX) field;
	symbol->sym_type = SYM_global;

	if (symbol = HSH_typed_lookup(symbol->sym_string,
								  symbol->sym_length,
								  SYM_global))
			PARSE_error(164, symbol->sym_string, 0);	/* msg 164: global field %s already exists */

	HSH_insert(field->fld_name);

	return make_action(act_a_gfield, (DBB) field);
}


static void mod_old_trigger(void)
{
/**************************************
 *
 *	m o d _ o l d  _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse and old style modify trigger.
 *      (first part was done earlier)
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_TRG trigger;
	int flags, type, sequence;
	SYM name, symbol;

	relation = PARSE_relation();

	while (!MATCH(KW_END_TRIGGER)) {
		flags = type = sequence = 0;
		get_trigger_attributes(&flags, &type, &sequence);
		if (!type)
			PARSE_error(165, DDL_token.tok_string, 0);	/* msg 165: expected STORE, MODIFY, ERASE, END_TRIGGER, encountered \"%s\"  */
		trigger = (DUDLEY_TRG) DDL_alloc(TRG_LEN);
		trigger->trg_relation = relation;
		trigger->trg_mflag = flags & ~trg_mflag_order;
		if (trigger->trg_mflag & trg_mflag_type)
			trigger->trg_type = trig_table[type & ~trig_inact];
		trigger->trg_name = name =
			gen_trigger_name(trigger->trg_type, relation);
		if (!
			(symbol =
			 HSH_typed_lookup(name->sym_string, name->sym_length,
							  SYM_trigger))) PARSE_error(166,
														 name->sym_string, 0);	/* msg 166: Trigger %s does not exist */
		modify_trigger_action(trigger, relation);
		make_action(act_m_trigger, (DBB) trigger);
	}
	parse_end();
}


static void modify_field(void)
{
/**************************************
 *
 *	m o d i f y _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY FIELD statement.
 *
 **************************************/
	SYM symbol;
	DUDLEY_FLD field;

	if (!local_context)
		LLS_PUSH(DDL_alloc(CTX_LEN), &local_context);

/* Lookup global field */

	for (symbol = DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_global)
			break;

	if (!symbol)
		PARSE_error(167, DDL_token.tok_string, 0);	/*msg 167: expected global field name, encountered \"%s\" */
	field = (DUDLEY_FLD) symbol->sym_object;
	LEX_token();
	field->fld_flags |= fld_modify;

	parse_field_dtype(field);
	if (field->fld_dtype == blr_cstring)
		PARSE_error(168, 0, 0);	/*msg 168: datatype cstring not supported for fields */
	parse_field_clauses(field);

	if (field->fld_computed)
		PARSE_error(169, 0, 0);	/* msg 169: A computed expression can not be changed or added */

	if (field->fld_security_class)
		PARSE_error(170, 0, 0);	/* msg 170: Security class can appear only on local field references */

	parse_end();

	make_action(act_m_gfield, (DBB) field);
}


static void modify_index(void)
{
/**************************************
 *
 *	m o d i f y _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Alter an index.  Does not currently
 *	allow adding or dropping fields from
 *	an index
 *
 **************************************/
	DUDLEY_IDX index;

	index = (DUDLEY_IDX) DDL_alloc(IDX_LEN(0));
	index->idx_name = PARSE_symbol(tok_ident);

	while (TRUE) {
		if (MATCH(KW_DUPLICATES)) {
			index->idx_unique = FALSE;
			index->idx_flags |= IDX_unique_flag;
		}
		else if (MATCH(KW_UNIQUE)) {
			index->idx_unique = TRUE;
			index->idx_flags |= IDX_unique_flag;
		}
		else if (MATCH(KW_ACTIVE)) {
			index->idx_inactive = FALSE;
			index->idx_flags |= IDX_active_flag;
		}
		else if (MATCH(KW_INACTIVE)) {
			index->idx_inactive = TRUE;
			index->idx_flags |= IDX_active_flag;
		}
		else if (MATCH(KW_ASCENDING)) {
			index->idx_type = IDX_type_none;
			index->idx_flags |= IDX_type_flag;
		}
		else if (MATCH(KW_DESCENDING)) {
			index->idx_type = IDX_type_descend;
			index->idx_flags |= IDX_type_flag;
		}
		else if (KEYWORD(KW_DESCRIPTION)) {
			index->idx_description = parse_description();
		}
		else if (MATCH(KW_DROP)) {
			if (MATCH(KW_DESCRIP))
				index->idx_flags |= IDX_null_description;
			else
				PARSE_error(172, DDL_token.tok_string, 0);	/*msg 172: expected DESCRIPTION, encountered \"%s\" */
		}
		else if (MATCH(KW_STATISTICS))
			index->idx_flags |= IDX_statistics_flag;
		else
			break;
	}

	make_action(act_m_index, (DBB) index);
}


static void modify_relation(void)
{
/**************************************
 *
 *	m o d i f y _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY RELATION statement.
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_FLD field, global;
	TEXT modify_relation;

	relation = PARSE_relation();
	make_action(act_m_relation, (DBB) relation);

	if (MATCH(KW_EXTERNAL_FILE)) {
		relation->rel_filename = PARSE_symbol(tok_quoted);
		if (!check_filename(relation->rel_filename, TRUE))
			PARSE_error(298, 0, 0);	/* msg 298: A non-Decnet node name is not permitted in an external file name */
	}

	modify_relation = FALSE;

	while (TRUE) {
		if (KEYWORD(KW_DESCRIPTION)) {
			relation->rel_description = parse_description();
			modify_relation = TRUE;
		}
		else if (MATCH(KW_SECURITY_CLASS))
		 {
			modify_relation = TRUE;
			relation->rel_security_class = PARSE_symbol(tok_ident);
		}
		else if (MATCH(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
			modify_relation = TRUE;
		}
		else
			break;
	}

/* Act on field actions */

	if (!KEYWORD(KW_SEMI))
		while (TRUE) {
			if (MATCH(KW_ADD)) {
				MATCH(KW_FIELD);
				{
					field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
					parse_field(field);
					field->fld_relation = relation;
					field->fld_database = database;
					if (field->fld_computed)
						make_computed_field(field);
					else if (!(field->fld_flags & fld_local))
						make_global_field(create_global_field(field));
					else if (global = lookup_global_field(field)) {
						field->fld_dtype = global->fld_dtype;
						field->fld_length = global->fld_length;
						field->fld_scale = global->fld_scale;
						field->fld_segment_length =
							global->fld_segment_length;
						field->fld_sub_type = global->fld_sub_type;
						field->fld_has_sub_type = global->fld_has_sub_type;
					}
					if (!(field->fld_flags & fld_explicit_position))
						field->fld_position = ++relation->rel_field_position;
					field->fld_name->sym_type = SYM_field;
					HSH_insert(field->fld_name);
					make_action(act_a_field, (DBB) field);
				}
			}
			else if (MATCH(KW_MODIFY)) {
				MATCH(KW_FIELD);
				field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
				field->fld_flags |= (fld_modify | fld_local);
				parse_field(field);
				field->fld_relation = relation;
				field->fld_database = database;
				if (field->fld_computed)
					PARSE_error(173, 0, 0);	/* msg 173: A computed expression can not be changed or added */
				make_action(act_m_field, (DBB) field);
			}
			else if (MATCH(KW_DROP)) {
				if (MATCH(KW_SECURITY_CLASS)) {
					relation->rel_flags |= rel_null_security_class;
					modify_relation = TRUE;
					MATCH(KW_COMMA);
					if (KEYWORD(KW_SEMI))
						break;
					else
						continue;
				}
				else if (MATCH(KW_DESCRIP)) {
					modify_relation = TRUE;
					relation->rel_flags |= rel_null_description;
					MATCH(KW_COMMA);
					if (KEYWORD(KW_SEMI))
						break;
					else
						continue;
				}
				else {
					MATCH(KW_FIELD);
					field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
					field->fld_flags |= fld_local;
					field->fld_relation = relation;
					field->fld_database = database;
					field->fld_name = PARSE_symbol(tok_ident);
					make_action(act_d_field, (DBB) field);
				}
			}
			else
				PARSE_error(174, DDL_token.tok_string, 0);	/* msg 174: expected field action, encountered \"%s\" */
			if (!MATCH(KW_COMMA))
				break;
		}

	parse_end();

	if (modify_relation)
		relation->rel_flags |= rel_marked_for_modify;
}


static void modify_security_class(void)
{
/**************************************
 *
 *	m o d i f y _ s e c u r i t y _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	Modify an existing security class.
 *
 **************************************/
	SCL class_;
	SCE element, *next;
	USHORT score;

/* return error msg for now until fully coded */

	PARSE_error(247, 0, 0);		/* msg 247: action not implemented yet */

	class_ = (SCL) DDL_alloc(sizeof(struct scl));
	class_->scl_name = PARSE_symbol(tok_ident);
	if (KEYWORD(KW_DESCRIPTION))
		class_->scl_description = parse_description();

	while (TRUE) {
		if (!(element = parse_identifier()))
			return;
		score = score_entry(element);
		for (next = &class_->scl_entries;; next = &(*next)->sce_next)
			if (!*next || score > score_entry(*next)) {
				element->sce_next = *next;
				*next = element;
				break;
			}
		element->sce_privileges = parse_privileges();
		if (!MATCH(KW_COMMA))
			break;
	}

	parse_end();
	make_action(act_m_security, (DBB) class_);
}


static void modify_trigger(void)
{
/**************************************
 *
 *	m o d i f y _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Parse new trigger modification
 *      (go elsewhere if this is an old trigger).
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_TRG trigger;
	TRGMSG trigmsg;
	TRGMSG_T msg_type;
	SLONG flags, type, sequence;
	SYM name;
	SCHAR action, end;

	action = end = FALSE;
	msg_type = trgmsg_none;

	if (MATCH(KW_FOR)) {		/* modify trigger for ... is the old syntax */
		mod_old_trigger();
		return;
	}

/* lookup the trigger name, complain and quit if it's unknown */

	for (name = DDL_token.tok_symbol;
		 name && (name->sym_type != SYM_trigger); name = name->sym_homonym);
	if (!name)
		PARSE_error(176, DDL_token.tok_string, 0);	/* msg 176: expected trigger name, encountered \"%s\" */
	trigger = (DUDLEY_TRG) name->sym_object;
	LEX_token();

/* in case somebody compulsive specifies the relation name */

	if (MATCH(KW_FOR)) {
		relation = PARSE_relation();
		if (relation != trigger->trg_relation)
			PARSE_error(177, 0, 0);	/* msg 177: Unsuccesful attempt to modify trigger relation */
	}
	else
		relation = trigger->trg_relation;

	flags = type = sequence = 0;
	get_trigger_attributes((int*) &flags, (int*) &type, (int*) &sequence);

	while (!KEYWORD(KW_SEMI)) {
		if ((MATCH(KW_MESSAGE)) || (MATCH(KW_MSGADD)) ||
			(MATCH(KW_MSGMODIFY))) msg_type = trgmsg_modify;
		else if ((MATCH(KW_MSGDROP)) || (MATCH(KW_DROP))) {
			MATCH(KW_MESSAGE);
			msg_type = trgmsg_drop;
		}
		if (msg_type) {
			trigmsg = (TRGMSG) DDL_alloc(sizeof(struct trgmsg));
			trigmsg->trgmsg_trg_name = trigger->trg_name;
			trigmsg->trgmsg_number = PARSE_number();
			if (trigmsg->trgmsg_number > 255)
				PARSE_error(178, (TEXT *) trigmsg->trgmsg_number, 0);	/* msg 178: message number %d exceeds 255 */
			if (msg_type == trgmsg_drop)
				make_action(act_d_trigger_msg, (DBB) trigmsg);
			else if (msg_type == trgmsg_modify) {
				MATCH(KW_COLON);
				trigmsg->trgmsg_text = PARSE_symbol(tok_quoted);
				make_action(act_m_trigger_msg, (DBB) trigmsg);
			}
			MATCH(KW_COMMA);
			msg_type = trgmsg_none;
		}
		else if (MATCH(KW_END_TRIGGER))
			end = TRUE;
		else if (KEYWORD(KW_DESCRIPTION))
			trigger->trg_description = parse_description();
		else if (!action && !end) {
			modify_trigger_action(trigger, relation);
			action = TRUE;
		}
		else
			PARSE_error(179, DDL_token.tok_string, 0);
		/* msg 179: expected message modification keyword, encountered \"%s\" */
	}
	if (flags || type || sequence)
		sort_out_attributes(trigger, flags, type, sequence);

	make_action(act_m_trigger, (DBB) trigger);
}


static void modify_trigger_action( DUDLEY_TRG trigger, DUDLEY_REL relation)
{
/**************************************
 *
 *	m o d i f y _ t r i g g e r _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a trigger statement.
 *
 **************************************/

	trigger->trg_source = start_text();

	trigger->trg_statement = EXPR_statement();
	end_text(trigger->trg_source);
}


static void modify_type(void)
{
/**************************************
 *
 *	m o d i f y _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a modify of an existing type value for a field.
 *
 **************************************/
	SYM fldname;
	TYP fldtype;

	MATCH(KW_FOR);
	fldname = PARSE_symbol(tok_ident);
	while (TRUE) {
		fldtype = (TYP) DDL_alloc(sizeof(struct typ));
		fldtype->typ_field_name = fldname;
		fldtype->typ_name = PARSE_symbol(tok_ident);
		MATCH(KW_COLON);
		fldtype->typ_type = PARSE_number();
		if (KEYWORD(KW_DESCRIPTION))
			fldtype->typ_description = parse_description();
		make_action(act_m_type, (DBB) fldtype);
		if (!MATCH(KW_COMMA))
			break;
	}
	parse_end();
}


static void modify_view(void)
{
/**************************************
 *
 *	m o d i f y _ v i e w
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY VIEW  statement.
 *
 **************************************/
	DUDLEY_REL relation;
	DUDLEY_FLD field;
	USHORT view_modify;

	relation = PARSE_relation();
	make_action(act_m_relation, (DBB) relation);
	view_modify = FALSE;

	while (TRUE) {
		if (KEYWORD(KW_DESCRIPTION)) {
			relation->rel_description = parse_description();
			view_modify = TRUE;
		}
		else if (MATCH(KW_SECURITY_CLASS)) {
			relation->rel_security_class = PARSE_symbol(tok_ident);
			view_modify = TRUE;
		}
		else if (MATCH(KW_SYSTEM_FLAG)) {
			relation->rel_system = get_system_flag();
			relation->rel_flags |= rel_explicit_system;
			view_modify = TRUE;
		}
		else
			break;
	}

/* Act on field actions */

	if (!KEYWORD(KW_SEMI))
		while (TRUE) {
			if (MATCH(KW_MODIFY)) {
				MATCH(KW_FIELD);
				field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
				field->fld_flags |= (fld_modify | fld_local);
				parse_field(field);
				field->fld_relation = relation;
				field->fld_database = database;
				if (field->fld_computed)
					PARSE_error(181, 0, 0);	/* msg 181: A computed expression can not be changed or added */
				make_action(act_m_field, (DBB) field);
			}
			else if (MATCH(KW_DROP)) {
				if (MATCH(KW_DESCRIP)) {
					view_modify = TRUE;
					relation->rel_flags |= rel_null_description;

					if (KEYWORD(KW_SEMI))
						break;
					else
						continue;
				}
				else if (MATCH(KW_SECURITY_CLASS)) {
					view_modify = TRUE;
					relation->rel_flags |= rel_null_security_class;

					if (KEYWORD(KW_SEMI))
						break;
					else
						continue;
				}
				else {
					MATCH(KW_FIELD);
					field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
					field->fld_flags |= fld_local;
					field->fld_relation = relation;
					field->fld_database = database;
					field->fld_name = PARSE_symbol(tok_ident);
					make_action(act_d_field, (DBB) field);
				}
			}
			else
				PARSE_error(182, DDL_token.tok_string, 0);
			/* msg 182: expected drop/modify of field or security class, encountered \"%s\" */
			if (!MATCH(KW_COMMA))
				break;
		}

	if (view_modify)
		relation->rel_flags |= rel_marked_for_modify;
	parse_end();
}


static BOOLEAN parse_action(void)
{
/**************************************
 *
 *	p a r s e _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a single action.  If an token flush is required, return TRUE.
 *
 **************************************/

/* Set up an environment to catch syntax errors.  If one occurs, scan looking
   for semi-colon to continue processing. */

	try {

	DDL_prompt = PROMPT;
	LEX_token();
	DDL_prompt = CONTINUATION;
	if (DDL_eof)
		return TRUE;

	if (MATCH(KW_DEFINE))
		switch (parse_object()) {
		case obj_database:
			define_database(act_c_database);
			return TRUE;
		case obj_relation:
			define_relation();
			return TRUE;
		case obj_view:
			define_view();
			return TRUE;
		case obj_field:
			define_field();
			return TRUE;
		case obj_index:
			define_index();
			return TRUE;
		case obj_security_class:
			define_security_class();
			return TRUE;
		case obj_trigger:
			define_trigger();
			return TRUE;
		case obj_file:
			define_file();
			return TRUE;
		case obj_function:
			define_function();
			return TRUE;
		case obj_type:
			define_type();
			return TRUE;
		case obj_filter:
			define_filter();
			return TRUE;
		case obj_shadow:
			define_shadow();
			return TRUE;
		case obj_generator:
			define_generator();
			return TRUE;

		default:
			if (database) {
				define_relation();
				return TRUE;
			}
			PARSE_error(183, DDL_token.tok_string, 0);	/* msg 183: expected object for DEFINE, encountered \"%s\" */
		}
	else if (MATCH(KW_MODIFY))
		switch (parse_object()) {
		case obj_database:
			define_database(act_m_database);
			return TRUE;
		case obj_relation:
			modify_relation();
			return TRUE;
		case obj_view:
			modify_view();
			return TRUE;
		case obj_field:
			modify_field();
			return TRUE;
		case obj_index:
			modify_index();
			return TRUE;
		case obj_security_class:
			modify_security_class();
			return TRUE;
		case obj_trigger:
			modify_trigger();
			return TRUE;
		case obj_function:
			PARSE_error(233, 0, 0);	/* msg 233: action not implemented yet */
		case obj_type:
			modify_type();
			return TRUE;
		case obj_filter:
			PARSE_error(231, 0, 0);	/* msg 231: action not implemented yet */
		case obj_shadow:
			PARSE_error(232, 0, 0);	/* msg 232: action not implemented yet */
		default:
			PARSE_error(184, DDL_token.tok_string, 0);
			/* msg 184: expected object for MODIFY, encountered \"%s\" */
		}
	else if (MATCH(KW_DROP))
		switch (parse_object()) {
		case obj_database:
			define_database(act_d_database);
			return TRUE;
		case obj_relation:
		case obj_view:
			drop_relation();
			return TRUE;
		case obj_field:
			drop_gfield();
			return TRUE;
		case obj_index:
			drop_index();
			return TRUE;
		case obj_security_class:
			drop_security_class();
			return TRUE;
		case obj_trigger:
			drop_trigger();
			return TRUE;
		case obj_function:
			drop_function();
			return TRUE;
		case obj_type:
			drop_type();
			return TRUE;
		case obj_filter:
			drop_filter();
			return TRUE;
		case obj_shadow:
			drop_shadow();
			return TRUE;
		default:
			PARSE_error(185, DDL_token.tok_string, 0);
			/* msg 185: expected object for DROP, encountered \"%s\" */
		}
	else if (MATCH(KW_GRANT)) {
		grant_user_privilege();
		return TRUE;
	}
	else if (MATCH(KW_REVOKE)) {
		revoke_user_privilege();
		return TRUE;
	}
	else if (MATCH(KW_SET_GENERATOR)) {
		set_generator();
		return TRUE;
	}
	else if (MATCH(KW_SET)) {
		if (!MATCH(KW_GENERATOR))
			PARSE_error(318, DDL_token.tok_string, 0);	/* msg 318: expected GENERATOR, encountered \"%s\" */
		set_generator();
		return TRUE;
	}
	else if (DDL_interactive && KEYWORD(KW_EXIT)) {
		DDL_eof = TRUE;
		return FALSE;
	}
	else if (DDL_interactive && KEYWORD(KW_QUIT)) {
		DDL_quit = DDL_eof = TRUE;
		return FALSE;
	}

	PARSE_error(186, DDL_token.tok_string, 0);	/* msg 186: expected command, encountered \"%s\" */
	return FALSE;

	}	// try
	catch (const std::exception&) {
		if (DDL_interactive)
			LEX_flush();
		else
			while (!DDL_eof && !KEYWORD(KW_SEMI))
				LEX_token();
		return TRUE;
	}
}


static void parse_array( DUDLEY_FLD field)
{
/************************************** 
 *
 *	p a r s e _ a r r a y 
 *
 **************************************
 *
 * Functional description
 *	Parse the multi-dimensional array specification.
 *
 **************************************/
	SLONG n, *range, *end, *ptr, ranges[2 * MAX_DIMENSION];

	if (!MATCH(KW_LEFT_PAREN))
		return;

/* Pick up ranges */

	for (range = ranges;;) {
		++field->fld_dimension;
		range[0] = 1;
		range[1] = PARSE_number();

		if (MATCH(KW_COLON)) {
			range[0] = range[1];
			range[1] = PARSE_number();
		}

		if ((range[1] - range[0]) <= 0)
			PARSE_error(188, 0, 0);	/* msg 188: array size must be positive */

		range += 2;

		if (MATCH(KW_RIGHT_PAREN))
			break;

		if (!MATCH(KW_COMMA))
			PARSE_error(189, DDL_token.tok_string, 0);	/* msg 189: expected comma, encountered \"%s\" */
	}

/* Allocate and copy block to hold ranges */

	n = field->fld_dimension * sizeof(SLONG) * 2;
	field->fld_ranges = ptr = (SLONG *) DDL_alloc(n);

	for (end = range, range = ranges; range < end;)
		*ptr++ = *range++;
}


static TXT parse_description(void)
{
/**************************************
 *
 *	p a r s e _ d e s c r i p t i o n
 *
 **************************************
 *
 * Functional description
 *	Create a text block to hold the pointer and length 
 *	of the description of a metadata item.
 *
 **************************************/
	TXT description;

	DDL_description = TRUE;
	description = start_text();
	description->txt_position = DDL_token.tok_position;

	while (!DDL_eof && (!KEYWORD(KW_END_DESCRIPTION)))
		LEX_token();

	if (DDL_eof)
		return NULL;

	end_text(description);
	MATCH(KW_END_DESCRIPTION);
	DDL_description = FALSE;

	return description;
}


static void parse_end(void)
{
/**************************************
 *
 *	p a r s e _ e n d
 *
 **************************************
 *
 * Functional description
 *	Parse the end of command.  Better be a semi-colon.
 *
 **************************************/

	if (!KEYWORD(KW_SEMI))
		PARSE_error(190, DDL_token.tok_string, 0);	/* msg 190: expected semi-colon, encountered \"%s\" */
}


static DUDLEY_FLD parse_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/

	field->fld_name = PARSE_symbol(tok_ident);
	field->fld_name->sym_object = (DUDLEY_CTX) field;

	if (MATCH(KW_BASED)) {
		MATCH(KW_ON);
		field->fld_source = PARSE_symbol(tok_ident);
		field->fld_flags |= fld_local;
	}
	else {
		parse_field_dtype(field);
		if (field->fld_dtype == blr_cstring)
			PARSE_error(191, 0, 0);	/* msg 191: datatype cstring not supported for fields */
	}

	if (!field->fld_dtype)
		field->fld_flags |= fld_local;

	parse_field_clauses(field);

	if (!KEYWORD(KW_COMMA) && !KEYWORD(KW_SEMI))
		PARSE_error(192, DDL_token.tok_string, 0);	/* msg 192: expected field clause, encountered \"%s\" */

	validate_field(field);

	return field;
}



static void parse_field_clauses( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ c l a u s e s
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/
	int n;
	LLS stack;

/* Pick up purely optional clauses */

	while (TRUE)
		switch (PARSE_keyword()) {
		case KW_POSITION:
			LEX_token();
			field->fld_position = PARSE_number();
			field->fld_flags |= fld_explicit_position;
			break;

		case KW_SECURITY_CLASS:
			LEX_token();
			field->fld_security_class = PARSE_symbol(tok_ident);
			break;

		case KW_DROP:
			LEX_token();
			if (MATCH(KW_SECURITY_CLASS))
				field->fld_flags |= fld_null_security_class;
			else if (MATCH(KW_VALID_IF))
				field->fld_flags |= fld_null_validation;
			else if (MATCH(KW_DESCRIP))
				field->fld_flags |= fld_null_description;
			else if (MATCH(KW_QUERY_NAME))
				field->fld_flags |= fld_null_query_name;
			else if (MATCH(KW_QUERY_HEADER))
				field->fld_flags |= fld_null_query_header;
			else if (MATCH(KW_EDIT_STRING))
				field->fld_flags |= fld_null_edit_string;
			else if (MATCH(KW_MISSING)) {
				MATCH(KW_VALUE);
				field->fld_flags |= fld_null_missing_value;
			}
			else
				PARSE_error(193, DDL_token.tok_string, 0);
			/* msg 193: expected DESCRIPTION, EDIT_STRING, MISSING VALUE, SECURITY_CLASS or VALID_IF", encountered \"%s\" */
			break;

		case KW_QUERY_NAME:
			LEX_token();
			MATCH(KW_IS);
			field->fld_query_name = PARSE_symbol(tok_ident);
			break;

		case KW_EDIT_STRING:
			LEX_token();
			field->fld_edit_string = PARSE_symbol(tok_quoted);
			break;

		case KW_QUERY_HEADER:
			LEX_token();
			MATCH(KW_IS);
			stack = NULL;
			for (;;) {
				LLS_PUSH(PARSE_symbol(tok_quoted), &stack);
				if (!MATCH(KW_SLASH))
					break;
			}
			field->fld_query_header = PARSE_make_list(stack);
			break;

		case KW_COMPUTED:
			LEX_token();
			MATCH(KW_BY);
			if (!(MATCH(KW_LEFT_PAREN)))
				PARSE_error(194, 0, 0);	/* msg 194: computed by expression must be parenthesized */
			field->fld_compute_src = start_text();
			field->fld_computed = EXPR_value(0, 0);
			end_text(field->fld_compute_src);
			if (!(MATCH(KW_RIGHT_PAREN)))
				PARSE_error(195, 0, 0);	/* msg 195: unmatched parenthesis */
			break;

		case KW_MISSING:
			LEX_token();
			MATCH(KW_VALUE);
			MATCH(KW_IS);
			field->fld_missing = EXPR_value(0, 0);
			break;

		case KW_VALID_IF:
			LEX_token();
			MATCH(KW_IF);
			if (!(MATCH(KW_LEFT_PAREN)))
				PARSE_error(196, 0, 0);	/* msg 196: validation expression must be parenthesized */
			field->fld_valid_src = start_text();
			field->fld_validation = EXPR_boolean(0);
			end_text(field->fld_valid_src);
			if (!(MATCH(KW_RIGHT_PAREN)))
				PARSE_error(195, 0, 0);	/* msg 195: unmatched parenthesis */
			break;

		case KW_SEGMENT_LENGTH:
			LEX_token();
			MATCH(KW_IS);
			field->fld_segment_length = n = PARSE_number();
			if (n <= 0)
				PARSE_error(197, 0, 0);	/* msg 197: segment length must be positive */
			break;

		case KW_SUB_TYPE:
			LEX_token();
			parse_field_subtype(field);
			break;

		case KW_DEFAULT:
			LEX_token();
			MATCH(KW_VALUE);
			MATCH(KW_IS);
			field->fld_default = EXPR_value(0, 0);
			break;

		case KW_DESCRIPTION:
			field->fld_description = parse_description();
			break;

		case KW_SYSTEM_FLAG:
			LEX_token();
			field->fld_system = get_system_flag();
			field->fld_flags |= fld_explicit_system;
			break;

		default:
			return;
		}
}


static void parse_field_dtype( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ d t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a field definition, complete with data type and clauses.
 *
 **************************************/
	int n;

	switch (PARSE_keyword()) {
	case KW_CHAR:
		field->fld_dtype = blr_text;
		break;

	case KW_VARYING:
		field->fld_dtype = blr_varying;
		break;

	case KW_CSTRING:
		field->fld_dtype = blr_cstring;
		break;

	case KW_SHORT:
		field->fld_dtype = blr_short;
		field->fld_length = sizeof(SSHORT);
		break;

	case KW_LONG:
		field->fld_dtype = blr_long;
		field->fld_length = sizeof(SLONG);
		break;

	case KW_QUAD:
		field->fld_dtype = blr_quad;
		field->fld_length = sizeof(GDS__QUAD);
		break;

	case KW_FLOAT:
		field->fld_dtype = blr_float;
		field->fld_length = sizeof(float);
		break;

	case KW_DOUBLE:
		field->fld_dtype = blr_double;
		field->fld_length = sizeof(double);
		break;

	case KW_DATE:
		field->fld_dtype = blr_timestamp;
		field->fld_length = sizeof(GDS__QUAD);
		break;

	case KW_BLOB:
		field->fld_dtype = blr_blob;
		field->fld_length = sizeof(GDS__QUAD);
		break;

	default:
		return;
	}

	LEX_token();

	if (field->fld_dtype == blr_text ||
		field->fld_dtype == blr_varying || field->fld_dtype == blr_cstring) {
		if (!MATCH(KW_L_BRCKET) && !MATCH(KW_LT))
			PARSE_error(200, DDL_token.tok_string, 0);	/* msg 200: expected \"[\", encountered \"%s\" */
		field->fld_length = n = PARSE_number();
		if (n <= 0)
			PARSE_error(201, 0, 0);	/* msg 201: character field length must be positive */
		if (!MATCH(KW_R_BRCKET) && !MATCH(KW_GT))
			PARSE_error(202, DDL_token.tok_string, 0);	/* msg 202: expected \"]\", encountered \"%s\" */

		if (MATCH(KW_SUB_TYPE))
			parse_field_subtype(field);

	}

	if (field->fld_dtype != blr_blob)
		parse_array(field);

	field->fld_scale = 0;

	if ((field->fld_dtype == blr_short ||
		 field->fld_dtype == blr_long || field->fld_dtype == blr_quad))
		if (MATCH(KW_SCALE))
			field->fld_scale = PARSE_number();
}


static void parse_field_subtype( DUDLEY_FLD field)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ s u b t y p e
 *
 **************************************
 *
 * Functional description
 *	Match a datatype sub_type clause.  Set the (fld_has_sub_type) flag
 *	so we know this type is a type set by the user.
 *
 **************************************/

	MATCH(KW_IS);
	if (MATCH(KW_TEXT) || MATCH(KW_FIXED))
		field->fld_sub_type = 1;
	else if (MATCH(KW_BLR))
		field->fld_sub_type = 2;
	else if (MATCH(KW_ACL))
		field->fld_sub_type = 3;
	else if (PARSE_keyword() == KW_MINUS)
		field->fld_sub_type = PARSE_number();
	else if (DDL_token.tok_type != tok_number)
		PARSE_error(198, DDL_token.tok_string, 0);	/* msg 198: expected field sub_type, encountered \"%s\" */
	else
		field->fld_sub_type = PARSE_number();

	field->fld_has_sub_type = TRUE;
}


static FUNCARG parse_function_arg( FUNC function, USHORT * position)
{
/**************************************
 *
 *	p a r s e _ f u n c t i o n _ a r g
 *
 **************************************
 *
 * Functional description
 *	Parse a function argument definition, complete with data type.
 *
 **************************************/
	FUNCARG func_arg;
	DUDLEY_FLD field;

	func_arg = (FUNCARG) DDL_alloc(FUNCARG_LEN);
	field = (DUDLEY_FLD) DDL_alloc(FLD_LEN);
	parse_field_dtype(field);
	func_arg->funcarg_dtype = field->fld_dtype;
	func_arg->funcarg_scale = field->fld_scale;
	func_arg->funcarg_length = field->fld_length;
	func_arg->funcarg_sub_type = field->fld_sub_type;
	func_arg->funcarg_has_sub_type = field->fld_has_sub_type;
	func_arg->funcarg_position = (*position)++;

	MATCH(KW_BY);
	LEX_token();

	switch (PARSE_keyword()) {
	case KW_VALUE:
		LEX_token();
		func_arg->funcarg_mechanism = FUNCARG_mechanism_value;
		if (field->fld_dtype == blr_text ||
			field->fld_dtype == blr_varying ||
			field->fld_dtype == blr_cstring ||
			field->fld_dtype == blr_blob || field->fld_dtype == blr_timestamp)
			PARSE_error(203, 0, 0);	/* msg 203: argument mode by value not allowed for this data type */
		break;

	case KW_REFERENCE:
		LEX_token();
		if (field->fld_dtype == blr_blob)
			func_arg->funcarg_mechanism = FUNCARG_mechanism_blob_struc;
		else
			func_arg->funcarg_mechanism = FUNCARG_mechanism_reference;
		break;

	case KW_SCALAR_ARRAY_DESCRIPTOR:
		LEX_token();
		func_arg->funcarg_mechanism = FUNCARG_mechanism_sc_array_desc;
		break;

	default:
		PARSE_error(204, 0, 0);	/* msg 204: argument mode is by value, or by reference */
	}

/* (kw_comma or kw_semi) here means this argument is not a 
   return_value or a return_argument in which case it had
   better not be passed by value */

	if (KEYWORD(KW_COMMA) || KEYWORD(KW_SEMI)) {
		if (func_arg->funcarg_mechanism == FUNCARG_mechanism_value)
			PARSE_error(293, 0, 0);	/* msg 293: argument mode 'by value' requires a return mode */
	}
	else {
		if (func_arg->funcarg_mechanism == FUNCARG_mechanism_sc_array_desc)
			PARSE_error(295, 0, 0);	/* msg 295: "Functions can't return arrays." */
		switch (PARSE_keyword()) {
		case KW_RETURN_VALUE:
			--(*position);
			LEX_token();
			function->func_return_arg = func_arg->funcarg_position = 0;
			function->func_return = func_arg;
			break;

		case KW_RETURN_ARGUMENT:
			LEX_token();
			if (func_arg->funcarg_mechanism == FUNCARG_mechanism_value)
				PARSE_error(292, 0, 0);	/* msg 292: argument mode of a return_argument must be 'by reference' */
			function->func_return_arg = func_arg->funcarg_position;
			function->func_return = func_arg;
			break;

		default:
			PARSE_error(205, 0, 0);	/* msg 205: return mode must be return_value or return_argument */
		}
	}

	if (*position > 11)
		PARSE_error(310, 0, 0);	/* msg 310: UDF is limited to 10 parameters */

	return func_arg;
}


static SCE parse_identifier(void)
{
/**************************************
 *
 *	p a r s e _ i d e n t i f i e r
 *
 **************************************
 *
 * Functional description
 *	Parse the hard part of an access control element.
 *
 **************************************/
	TEXT *idents[10], **s, **end, **s1, strings[256], *p, *q;
	SCE element;

	p = strings;
	for (s = idents, end = s + 10; s < end; s++)
		*s = NULL;

/* Get explicit identifiers, if any */

	switch (DDL_token.tok_keyword) {
	case (KW_VIEW):
		LEX_token();
		if (DDL_token.tok_type != tok_ident)
			PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_view] = p;
		q = DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		break;

	case (KW_USER):
		LEX_token();
		if (DDL_token.tok_type != tok_ident)
			PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_user] = p;
		q = DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		if (MATCH(KW_GROUP)) {
			if (DDL_token.tok_type != tok_ident)
				PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
			idents[id_group] = p;
			q = DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		break;

	case (KW_GROUP):
		LEX_token();
		if (DDL_token.tok_type != tok_ident)
			PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		idents[id_group] = p;
		q = DDL_token.tok_string;
		while (*p++ = *q++);
		LEX_token();
		if (MATCH(KW_USER)) {
			if (DDL_token.tok_type != tok_ident)
				PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
			idents[id_user] = p;
			q = DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		break;

	case (KW_L_BRCKET):
	case (KW_LT):
		LEX_token();
		if (!MATCH(KW_ASTERISK)) {
			if (DDL_token.tok_type != tok_number)
				PARSE_error(206, DDL_token.tok_string, 0);	/* msg 206: expected number, encountered \"%s\" */
			idents[id_group] = p;
			q = DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		if (!MATCH(KW_COMMA))
			PARSE_error(207, DDL_token.tok_string, 0);	/* msg 207: expected comma between group and user ids, encountered \"%s\" */
		if (!MATCH(KW_ASTERISK)) {
			if (DDL_token.tok_type != tok_number)
				PARSE_error(206, DDL_token.tok_string, 0);	/* msg 206: expected number, encountered \"%s\" */
			idents[id_user] = p;
			q = DDL_token.tok_string;
			while (*p++ = *q++);
			LEX_token();
		}
		if (!(MATCH(KW_R_BRCKET) || MATCH(KW_GT)))
			PARSE_error(208, DDL_token.tok_string, 0);	/* msg 208: expected trailing bracket, encountered \"%s\" */

		break;
	}

/* Build access control element */

	element = (SCE) DDL_alloc(sizeof(struct sce) + (p - strings));
	p = (TEXT *) element->sce_strings;

	for (s = idents, end = s + 10, s1 = (TEXT **) element->sce_idents;
		 s < end; s++, s1++)
		if (q = *s) {
			*s1 = p;
			while (*p++ = *q++);
		}

	return element;
}


static OBJ_T parse_object(void)
{
/**************************************
 *
 *	p a r s e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Parse on object name returning what we found.
 *
 **************************************/

	if (MATCH(KW_DATABASE))
		return obj_database;
	else if (!database || !database->dbb_handle)
		PARSE_error(209, 0, 0);	/* msg 209: no database declared */

	if (MATCH(KW_RELATION))
		return obj_relation;

	if (MATCH(KW_FIELD))
		return obj_field;

	if (MATCH(KW_INDEX))
		return obj_index;

	if (MATCH(KW_VIEW))
		return obj_view;

	if (MATCH(KW_SECURITY_CLASS))
		return obj_security_class;

	if (MATCH(KW_TRIGGER))
		return obj_trigger;

	if (MATCH(KW_FILE))
		return obj_file;

	if (MATCH(KW_FUNCTION))
		return obj_function;

	if (MATCH(KW_TYPES))
		return obj_type;

	if (MATCH(KW_FILTER))
		return obj_filter;

	if (MATCH(KW_SHADOW))
		return obj_shadow;

	if (MATCH(KW_GENERATOR))
		return obj_generator;

	return obj_none;
}


static int parse_page_size(void)
{
/**************************************
 *
 *	p a r s e _ p a g e _ s i z e
 *
 **************************************
 *
 * Functional description
 *	parse the page_size clause of a
 *	define database statement
 *	
 *
 **************************************/
	int n1, n2;

	MATCH(KW_EQUALS);
	n2 = n1 = PARSE_number();

	if (n1 <= 1024)
		n2 = 1024;
	else if (n1 <= 2048)
		n2 = 2048;
	else if (n1 <= 4096)
		n2 = 4096;
	else if (n1 <= 8192)
		n2 = 8192;
	else
		PARSE_error(210, (TEXT *) n1, (TEXT *) MAX_PAGE_LEN);
	/* msg 210: PAGE_SIZE specified (%d) longer than limit of %d bytes */
	if (n1 != n2)
		DDL_msg_put(211, (TEXT *) n1, (TEXT *) n2, NULL, NULL, NULL);
	/* msg 211: PAGE_SIZE specified (%d) was rounded up to %d bytes\n */

	return n2;
}


static SLONG parse_privileges(void)
{
/**************************************
 *
 *	p a r s e _ p r i v i l e g e s
 *
 **************************************
 *
 * Functional description
 *	Parse an access control list.
 *
 **************************************/
	TEXT *p, c;
	SLONG privileges;

	privileges = 0;

	if (!MATCH(KW_MINUS)) {
		if (DDL_token.tok_type != tok_ident)
			PARSE_error(117, DDL_token.tok_string, 0);	/* msg 117: expected identifier, encountered \"%s\" */
		p = DDL_token.tok_string;
		while (c = *p++)
			switch (UPPER(c)) {
			case 'P':
				privileges |= 1 << priv_protect;
				break;

			case 'G':
				privileges |= 1 << priv_grant;
				break;

			case 'D':
				privileges |= 1 << priv_delete;
				break;

			case 'R':
				privileges |= 1 << priv_read;
				break;

			case 'W':
				privileges |= 1 << priv_write;
				break;

			case 'C':
				privileges |= 1 << priv_control;
				break;

			default:
				PARSE_error(212, (TEXT *) p[-1], 0);
				/* msg 212: Unrecognized privilege \"%c\" or unrecognized identifier */
			}
		LEX_token();
	}

	return privileges;
}


static void revoke_user_privilege(void)
{
/**************************************
 *
 *	r e v o k e _ u s e r _ p r i v i l e g e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL grant statement.
 *
 **************************************/
	UPFE upf;
	USERPRIV upriv;
	USRE usr;

	upriv = (USERPRIV) DDL_alloc(sizeof(struct userpriv));

	while (TRUE) {
		if (MATCH(KW_ALL)) {
			/* optional keyword following ALL */

			MATCH(KW_PRIVILEGES);
			upriv->userpriv_flags |= USERPRIV_select;
			upriv->userpriv_flags |= USERPRIV_delete;
			upriv->userpriv_flags |= USERPRIV_insert;
			upriv->userpriv_flags |= USERPRIV_update;
		}
		else if (MATCH(KW_SELECT))
			upriv->userpriv_flags |= USERPRIV_select;
		else if (MATCH(KW_DELETE))
			upriv->userpriv_flags |= USERPRIV_delete;
		else if (MATCH(KW_INSERT))
			upriv->userpriv_flags |= USERPRIV_insert;
		else if (MATCH(KW_UPDATE)) {
			/* revoke update privilege applies to all fields in the grant
			   update list */

			upriv->userpriv_flags |= USERPRIV_update;
			if (MATCH(KW_ON))
				break;
			if (MATCH(KW_COMMA))
				continue;
			if (!MATCH(KW_LEFT_PAREN))
				PARSE_error(315, DDL_token.tok_string, 0);	/* msg 315: expected ON or '(', encountered "%s" */

			do {
				if (KEYWORD(KW_SELECT) || KEYWORD(KW_INSERT) ||
					KEYWORD(KW_DELETE) || KEYWORD(KW_UPDATE))
					break;
				upf = (UPFE) DDL_alloc(sizeof(struct upfe));
				upf->upfe_fldname = PARSE_symbol(tok_ident);
				upf->upfe_next = upriv->userpriv_upflist;
				upriv->userpriv_upflist = upf;
			} while (MATCH(KW_COMMA));

			if (!MATCH(KW_RIGHT_PAREN))
				PARSE_error(316, DDL_token.tok_string, 0);	/* msg 316: expected ')', encountered "%s" */

			continue;
		}
		if (!MATCH(KW_COMMA)) {
			if (!MATCH(KW_ON))
				PARSE_error(214, DDL_token.tok_string, 0);	/* msg 214: expected ON, encountered \"%s\" */
			break;
		}
	}							/* while (TRUE) */

	if (!upriv->userpriv_flags)
		PARSE_error(215, 0, 0);	/* msg 215: REVOKE privilege was not specified */

	upriv->userpriv_relation = PARSE_symbol(tok_ident);
	if (!MATCH(KW_FROM))
		PARSE_error(216, DDL_token.tok_string, 0);	/* msg 216: expected FROM, encountered \"%s\" */

/* get the userlist */
	while (TRUE) {
		usr = (USRE) DDL_alloc(sizeof(struct usre));
		usr->usre_name = PARSE_symbol(tok_ident);
		usr->usre_next = upriv->userpriv_userlist;
		upriv->userpriv_userlist = usr;
		if (!MATCH(KW_COMMA))
			break;
	}

	parse_end();
	make_action(act_revoke, (DBB) upriv);
}


static SLONG score_entry( SCE element)
{
/**************************************
 *
 *	s c o r e _ e n t r y
 *
 **************************************
 *
 * Functional description
 *	Compute a value to determine placement of an 
 *	access control element in an Apollo access 
 *	control list.
 *
 **************************************/
	SLONG score;
	TEXT **ptr, **end;

	score = 0;
	if (element->sce_idents[id_view])
		score++;

	for (ptr = (TEXT **) element->sce_idents, end = ptr + id_max; ptr < end;
		 ptr++) {
		score <<= 1;
		if (*ptr)
			score |= 1;
	}

	return score;
}


static DUDLEY_NOD set_generator(void)
{
/**************************************
 *
 *	s e t _ g e n e r a t o r
 *
 **************************************
 *
 * Functional description
 *      get the name and new value for generator
 **************************************/
	DUDLEY_NOD node;

	if (DDL_token.tok_type != tok_ident)
		PARSE_error(274, DDL_token.tok_string, 0);	/* msg 274: expected generator name, encountered \"%s\" */

	node = PARSE_make_node(nod_set_generator, 2);
	node->nod_count = 1;
	node->nod_arg[1] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
	MATCH(KW_TO);
	node->nod_arg[0] = EXPR_value(0, 0);

	parse_end();
	make_action(act_s_generator, (DBB) node);
	return 0;
}


static void sort_out_attributes(
								DUDLEY_TRG trigger,
								SLONG flags, SLONG type, SLONG sequence)
{
/**************************************
 *
 *	s o r t _ o u t _ a t t r i b u t e s
 *
 **************************************
 *
 * Functional description
 *	Somebody is being very cute and changing
 *	attributes of a trigger.  We're going to do
 *	it, but we'd better remember the other, unchanged
 *	attributes so we don't make a big mess.
 *
 **************************************/

	trigger->trg_mflag = flags & ~trg_mflag_order;

	if (flags & trg_mflag_type)
		switch (trigger->trg_type) {
		case trg_store:
		case trg_post_store:
			if (!(type & (trig_mod | trig_era)))
				type |= trig_sto;
			else if (!trigger->trg_statement && (type & trig_era))
				PARSE_error(217, 0, 0);	/* msg 217: Attempt change trigger type from STORE to ERASE */
			break;
		case trg_modify:
		case trg_post_modify:
			if (!(type & (trig_sto | trig_era)))
				type |= trig_mod;
			else if (!trigger->trg_statement)
				if (type & trig_sto)
					PARSE_error(218, 0, 0);	/* msg 218: Attempt change trigger type from MODIFY to STORE */
				else
					PARSE_error(219, 0, 0);	/* msg 219: Attempt change trigger type from MODIFY to ERASE */
			break;
		case trg_pre_erase:
		case trg_erase:
			if (!(type & (trig_sto | trig_mod)))
				type |= trig_era;
			else if (!trigger->trg_statement && (type & trig_sto))
				PARSE_error(220, 0, 0);	/* msg 220: Attempt to change trigger type from ERASE to STORE */
		}

	if ((!(flags & trg_mflag_order)) && (flags & trg_mflag_type))
		switch (trigger->trg_type) {
		case trg_erase:
		case trg_post_modify:
		case trg_post_store:
			type |= trig_post;
			break;
		case trg_store:
		case trg_modify:
		case trg_pre_erase:
			type &= ~trig_post;
		}

	if (trigger->trg_mflag & trg_mflag_type)
		trigger->trg_type = trig_table[type & ~trig_inact];

	if (trigger->trg_mflag & trg_mflag_seqnum)
		trigger->trg_sequence = sequence;

	if (trigger->trg_mflag & trg_mflag_onoff)
		trigger->trg_inactive = type & trig_inact;
}


static TXT start_text(void)
{
/**************************************
 *
 *	s t a r t _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Make the current position to save description text.
 *
 **************************************/
	TXT text;

	text = (TXT) DDL_alloc(TXT_LEN);
	text->txt_position = DDL_token.tok_position - DDL_token.tok_length;
	text->txt_start_line = DDL_line;

	return text;
}


static void validate_field( DUDLEY_FLD field)
{
/**************************************
 *
 *     v a l i d a t e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Validate that the field clauses make sense
 *	together.
 *
 **************************************/
	TEXT option[128];

	*option = 0;

	if (field->fld_flags & fld_local) {
		if (field->fld_validation)
			gds__msg_format(0, DDL_MSG_FAC, 221, sizeof(option), option, NULL,
							NULL, NULL, NULL, NULL);
		/* msg 221 /'valid if/' */
		if (field->fld_missing)
			gds__msg_format(0, DDL_MSG_FAC, 222, sizeof(option), option, NULL,
							NULL, NULL, NULL, NULL);
		/* msg 222: missing value */
		if ((field->fld_dtype) && !(field->fld_computed))
			gds__msg_format(0, DDL_MSG_FAC, 223, sizeof(option), option, NULL,
							NULL, NULL, NULL, NULL);
		/* msg 223: data type */
		if ((field->fld_has_sub_type) && !(field->fld_computed))
			gds__msg_format(0, DDL_MSG_FAC, 224, sizeof(option), option, NULL,
							NULL, NULL, NULL, NULL);
		/* msg 224: sub type */
		if ((field->fld_segment_length) && !(field->fld_computed))
			gds__msg_format(0, DDL_MSG_FAC, 225, sizeof(option), option, NULL,
							NULL, NULL, NULL, NULL);
		/* msg 225: segment_length */
		if (*option)
			PARSE_error(226, option, 0);	/* msg 226: %s is a global, not local, attribute */
		return;
	}

	if (field->fld_computed && !(field->fld_dtype))
		PARSE_error(227, 0, 0);	/* msg 227: computed fields need datatypes */

	if (field->fld_flags & fld_modify)
		return;

	if ((field->fld_has_sub_type) &&
		(field->fld_dtype != blr_blob) &&
		(field->fld_dtype != blr_text) && (field->fld_dtype != blr_varying))
		PARSE_error(228, 0, 0);	/* msg 228: subtypes are valid only for blobs and text */

	if (field->fld_segment_length && (field->fld_dtype != blr_blob))
		PARSE_error(229, 0, 0);	/* msg 229: segment length is valid only for blobs */

	if ((field->fld_dtype == blr_blob) && !(field->fld_segment_length))
		field->fld_segment_length = 80;
}
