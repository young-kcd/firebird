/*
 *	PROGRAM:	JRD Command Oriented Query Language
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#define PARSER_MAIN
#include "../qli/dtr.h"
#include "../qli/exe.h"			/* This is only included to suppress a compiler warning */
#include "../qli/parse.h"
#include "../qli/compile.h"
#include "../qli/report.h"
#include "../qli/all_proto.h"
#include "../qli/err_proto.h"
#include "../qli/hsh_proto.h"
#include "../qli/lex_proto.h"
#include "../qli/mov_proto.h"
#include "../qli/parse_proto.h"
#include "../qli/proc_proto.h"

#define SYNTAX_NODE(type,count)	syntax_node (type, count)
#define KEYWORD(kw)		(QLI_token->tok_keyword == kw)
#define INT_CAST		(SYN) (IPTR)

static void check_end(void);
static void command_end(void);
static DBB get_dbb(SYM);
static SYN make_list(LLS);
static NAM make_name(void);
static CON make_numeric_constant(TEXT *, USHORT);
static TEXT *make_string(TEXT *, USHORT);
static SYN negate(SYN);
static KWWORDS next_keyword(void);
static SYN parse_abort(void);
static SYN parse_accept(void);
static SYN parse_add(USHORT *, USHORT *);
static SYN parse_and(USHORT *);
static SYN parse_assignment(void);
static SYN parse_boolean(USHORT *);
static SYN parse_copy(void);
static DBB parse_database(void);
static SYN parse_declare(void);
static SYN parse_define(void);
static SYN parse_def_index(void);
static SYN parse_def_relation(void);
static SYN parse_delete(void);
static SYN parse_drop(void);
static int parse_dtype(USHORT *, USHORT *);
static int parse_dtype_subtype(void);
static SYN parse_edit(void);
static TEXT *parse_edit_string(void);
static SYN parse_erase(void);
static SYN parse_extract(void);
static QLI_FLD parse_field(int);
static SYN parse_field_name(SYN *);
static SYN parse_for(void);
#ifdef PYXIS
static SYN parse_form(void);
#endif
static SYN parse_from(USHORT *, USHORT *);
static SYN parse_function(void);
static TEXT *parse_header(void);
static SYN parse_help(void);
static SYN parse_if(void);
static SYN parse_in(SYN, NOD_T, USHORT);
static SYN parse_insert(void);
static NOD_T parse_join_type(void);
static SYN parse_list_fields(void);
static CON parse_literal(void);
static SYN parse_matches(void);
static void parse_matching_paren(void);
static SYN parse_menu(void);
static SYN parse_modify(void);
static SYN parse_modify_index(void);
static SYN parse_modify_relation(void);
static SYN parse_multiply(USHORT *, USHORT *);
static NAM parse_name(void);
static SYN parse_not(USHORT *);
static int parse_ordinal(void);
static SYN parse_output(void);
static SYN parse_primitive_value(USHORT *, USHORT *);
static SYN parse_print_list(void);
static SYN parse_print(void);
static SYN parse_prompt(void);
static QFL parse_qualified_filter(void);
static QFN parse_qualified_function(void);
static QPR parse_qualified_procedure(void);
static QLI_REL parse_qualified_relation(void);
static SYN parse_ready(NOD_T);
static SYN parse_relational(USHORT *);
static SYN parse_relation(void);
static SYN parse_rename(void);
static SYN parse_repeat(void);
static SYN parse_report(void);
static SYN parse_rse(void);
static SYN parse_select(void);
static SYN parse_set(void);
static SYN parse_shell(void);
static SYN parse_show(void);
static SYN parse_sort(void);
static SYN parse_sql_alter(void);
static SYN parse_sql_create(void);
static int parse_sql_dtype(USHORT *, USHORT *);
static QLI_FLD parse_sql_field(void);
static SYN parse_sql_grant_revoke(USHORT);
static SYN parse_sql_index_create(USHORT, USHORT);
static SYN parse_sql_joined_relation(SYN);
static SYN parse_sql_join_clause(SYN);
static SYN parse_sql_table_create(void);
#ifdef NOT_USED_OR_REPLACED
static SYN parse_sql_view_create(void);
#endif
static SYN parse_sql_relation(void);
static SYN parse_sql_rse(void);
static SYN parse_sql_singleton_select(void);
static SYN parse_sql_subquery(void);
static SYN parse_statement(void);
static SYN parse_statistical(void);
static SYN parse_store(void);
static TEXT *parse_string(void);
static SYM parse_symbol(void);
static void parse_terminating_parens(USHORT *, USHORT *);
static SYN parse_transaction(NOD_T);
static SYN parse_udf_or_field(void);
static SYN parse_update(void);
static SYN parse_value(USHORT *, USHORT *);
static int potential_rse(void);
static QLI_REL resolve_relation(SYM, SYM);
static SYN syntax_node(NOD_T, USHORT);
static int test_end(void);

typedef struct {
	SLONG gds_quad_high;
	ULONG gds_quad_low;
} gds_quad;

/* 
The following flags are: 

	sql_flag	indicates whether we are parsing in a SQL environment.  
			The flag is used to turn off automatic end-of-command
			recognition.

	else_count	indicates the depth of if/then/else's

	sw_report 	indicates whether we're in a report statement

	sw_statement	indicates that we're actively parsing a statement/command

	sw_sql_view	indicates parsing a SQL VIEW, so restrict the select.
*/

static USHORT sql_flag, else_count, sw_report, sw_statement, sw_sql_view;
static SSHORT function_count;	/* indicates the depth of UDF calls */

static struct nod_types {
	KWWORDS nod_t_keyword;
	NOD_T nod_t_node;
	NOD_T nod_t_rpt_node;
	NOD_T nod_t_sql_node;
} statisticals[] = {
	{ KW_MAX, nod_max, nod_rpt_max, nod_agg_max },
	{ KW_MIN, nod_min, nod_rpt_min, nod_agg_min },
	{ KW_COUNT, nod_count, nod_rpt_count, nod_agg_count },
	{ KW_AVERAGE, nod_average, nod_rpt_average, nod_agg_average },
	{ KW_TOTAL, nod_total, nod_rpt_total, nod_agg_total },
	{ KW_none, nod_any, nod_any, nod_any }
};

static NOD_T relationals[] = {
	nod_eql, nod_neq, nod_leq, nod_lss, nod_gtr, nod_geq, nod_containing,
	nod_like, nod_starts, nod_missing, nod_between, nod_sleuth,
	nod_matches, nod_and, nod_or, nod_not, nod_any, nod_unique, (NOD_T) 0
};


SYN PARQ_parse(void)
{
/**************************************
 *
 *	P A R Q _ p a r s e
 *
 **************************************
 *
 * Functional description
 *	Parse a single statement or command.
 *
 **************************************/
	LLS stack;
	SYN node;

	sql_flag = else_count = sw_report = 0;
	sw_statement = TRUE;

	switch (next_keyword()) {
	case KW_COMMIT:
		return parse_transaction(nod_commit);

	case KW_COPY:
		return parse_copy();

	case KW_EXIT:
		return SYNTAX_NODE(nod_exit, 0);

	case KW_EXTRACT:
		return parse_extract();

	case KW_QUIT:
		return SYNTAX_NODE(nod_quit, 0);

	case KW_DELETE:
	case KW_DROP:
		if (node = parse_drop())
			return node;
		node = parse_delete();
		check_end();
		if (!MATCH(KW_THEN))
			return node;
		stack = NULL;
		LLS_PUSH(node, &stack);
		LLS_PUSH(parse_statement(), &stack);
		return make_list(stack);

	case KW_DEFINE:
		return parse_define();

	case KW_CREATE:
		return parse_sql_create();

	case KW_ALTER:
		return parse_sql_alter();

	case KW_EDIT:
		return parse_edit();

	case KW_FINISH:
		return parse_transaction(nod_finish);

	case KW_GRANT:
		return parse_sql_grant_revoke(nod_sql_grant);

	case KW_HELP:
		return parse_help();

	case KW_PREPARE:
		return parse_transaction(nod_prepare);

	case KW_READY:
		return parse_ready(nod_ready);

	case KW_RENAME:
		return parse_rename();

	case KW_REVOKE:
		return parse_sql_grant_revoke(nod_sql_revoke);

	case KW_ROLLBACK:
		return parse_transaction(nod_rollback);

	case KW_SET:
		return parse_set();

	case KW_SHELL:
		return parse_shell();

	case KW_SHOW:
		return parse_show();
	}

	return parse_statement();
}


int PAR_match( KWWORDS keyword)
{
/**************************************
 *
 *	P A R _ m a t c h
 *
 **************************************
 *
 * Functional description
 *	Test the current token for a particular keyword.
 *	If so, advance the token stream.
 *
 **************************************/
	SYM symbol;

	if (KEYWORD(keyword)) {
		PAR_token();
		return TRUE;
	}

	for (symbol = QLI_token->tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword &&
			symbol->sym_keyword == (int) keyword) {
			PAR_token();
			return TRUE;
		}

	return FALSE;
}


void PAR_real(void)
{
/**************************************
 *
 *	P A R _ r e a l
 *
 **************************************
 *
 * Functional description
 *	Get a real (not EOL) token.
 *
 *	If the token is an end of line, get the next token.
 *      If the next token is a colon, start reading the
 *      procedure.
 *
 **************************************/
	DBB database;

	while ((QLI_token->tok_type == tok_eol) || KEYWORD(KW_continuation))
		LEX_token();

	if (MATCH(KW_COLON)) {
		database = parse_database();
		PRO_invoke(database, QLI_token->tok_string);
	}
}


void PAR_real_token(void)
{
/**************************************
 *
 *	P A R _ r e a l _ t o k e n
 *
 **************************************
 *
 * Functional description
 *	Composition of PAR_token followed by PAR_real.
 *
 **************************************/

	PAR_token();
	PAR_real();
}


void PAR_token(void)
{
/**************************************
 *
 *	P A R _ t o k e n
 *
 **************************************
 *
 * Functional description
 *	Get the next token.  
 *	If it's a procedure invocation, handle it
 *      or complain and get rid of the evidence.
 *
 **************************************/
	DBB database;

	for (;;) {
		LEX_token();
		if (!(KEYWORD(KW_continuation)) &&
			!(sw_statement && QLI_semi && QLI_token->tok_type == tok_eol))
			break;
	}

	if (MATCH(KW_COLON)) {
		if (!QLI_databases) {
			ERRQ_error_format(159, NULL, NULL, NULL, NULL, NULL);	/* Msg159 no databases are ready */
			ERRQ_pending();
			LEX_token();
		}
		else {
			database = parse_database();
			PRO_invoke(database, QLI_token->tok_string);
		}
	}
}


static void check_end(void)
{
/**************************************
 *
 *	c h e c k _ e n d
 *
 **************************************
 *
 * Functional description
 *	Check for end of statement.  If it isn't complain bitterly.
 *
 **************************************/

	if (QLI_token->tok_type == tok_eol ||
		KEYWORD(KW_SEMI) ||
		KEYWORD(KW_THEN) || (else_count && KEYWORD(KW_ELSE))) {
		sw_statement = FALSE;
		return;
	}

	SYNTAX_ERROR(161);			/* Msg161 end of statement */
}


static void command_end(void)
{
/**************************************
 *
 *	c o m m a n d _ e n d
 *
 **************************************
 *
 * Functional description
 *	Check for end of command.  If it isn't complain bitterly.
 *
 **************************************/

	if (QLI_token->tok_type == tok_eol || KEYWORD(KW_SEMI)) {
		sw_statement = FALSE;
		return;
	}

	SYNTAX_ERROR(162);			/* Msg162 end of command */
}


static DBB get_dbb( SYM symbol)
{
/**************************************
 *
 *	g e t _ d b b
 *
 **************************************
 *
 * Functional description
 *	Find a database block from a symbol
 *	or its homonyms.
 *
 **************************************/

	for (; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_database)
			return (DBB) symbol->sym_object;

	return NULL;
}


static SYN make_list( LLS stack)
{
/**************************************
 *
 *	m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Dump a stack of junk into a list node.  Best count
 *	them first.
 *
 **************************************/
	SYN node, *ptr;
	LLS temp;
	USHORT count;

	temp = stack;
	count = 0;

	while (temp) {
		count++;
		temp = temp->lls_next;
	}

	node = SYNTAX_NODE(nod_list, count);
	ptr = &node->syn_arg[count];

	while (stack)
		*--ptr = (SYN) LLS_POP(&stack);

	return node;
}


static NAM make_name(void)
{
/**************************************
 *
 *	m a k e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Generate a unique name for something
 *	(like a database) that needs one.
 *
 **************************************/
	NAM name;
	SSHORT l, i;
	TEXT c, *p, *q, string[32];

	for (i = 0; i < 1000; i++) {
		sprintf(string, "QLI_%d", i);
		if (i < 10)
			l = 5;
		else
			l = (i < 100) ? 6 : 7;
		if (!(HSH_lookup(string, l)))
			break;
	}

	name = (NAM) ALLOCDV(type_nam, l);
	name->nam_length = l;
	p = name->nam_string;
	q = string;

	if (l)
		do {
			c = *q++;
			*p++ = UPPER(c);
		} while (--l);

	return name;
}


static CON make_numeric_constant( TEXT * string, USHORT length)
{
/**************************************
 *
 *	m a k e _ n u m e r i c _ c o n s t a n t
 *
 **************************************
 *
 * Functional description
 *	Build a constant block for a numeric
 *	constant.  Numeric constants are normally
 *	stored as long words, but especially large
 *	ones become text.  They ought to become
 *      double precision, one would think, but they'd
 *      have to be VAX style double precision which is
 *      more pain than gain.
 *
 **************************************/
	CON constant;
	TEXT *p;
	USHORT l;

	p = string;
	l = length;

/* If there are a reasonable number of digits, convert to binary */

	if (length < 9) {
		constant = (CON) ALLOCDV(type_con, sizeof(SLONG));
		constant->con_desc.dsc_dtype = dtype_long;
		constant->con_desc.dsc_length = sizeof(SLONG);
		constant->con_desc.dsc_address = constant->con_data;
		constant->con_desc.dsc_scale =
			MOVQ_decompose(string, length, (SLONG*) constant->con_data);
	}
	else {
		++length;
		constant = (CON) ALLOCDV(type_con, length);
		constant->con_desc.dsc_dtype = dtype_text;
		constant->con_desc.dsc_length = length;
		constant->con_desc.dsc_address = constant->con_data;
		p = (TEXT *) constant->con_desc.dsc_address;
		*p++ = '0';
		while (--length)
			*p++ = *string++;
	}

	return constant;
}


static TEXT *make_string( TEXT * address, USHORT length)
{
/**************************************
 *
 *	m a k e _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Copy a string into a temporary string block.
 *
 **************************************/
	STR string;
	TEXT *p;

	string = (STR) ALLOCDV(type_str, length);
	p = string->str_data;
	if (length)
		do
			*p++ = *address++;
		while (--length);

	return string->str_data;
}


static SYN negate( SYN expr)
{
/**************************************
 *
 *	n e g a t e
 *
 **************************************
 *
 * Functional description
 *	Build negation of expression.
 *
 **************************************/
	SYN node;

	node = SYNTAX_NODE(nod_not, 1);
	node->syn_arg[0] = expr;

	return node;
}


static KWWORDS next_keyword(void)
{
/**************************************
 *
 *	n e x t _ k e y w o r d
 *
 **************************************
 *
 * Functional description
 *	Get a real token and return the keyword number.
 *
 **************************************/
	SYM symbol;

	PAR_real();

	for (symbol = QLI_token->tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword)
			return (KWWORDS) symbol->sym_keyword;

	return KW_none;
}


static SYN parse_abort(void)
{
/**************************************
 *
 *	p a r s e _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Parse an ABORT statement.
 *
 **************************************/
	SYN node;

	PAR_token();
	node = SYNTAX_NODE(nod_abort, 1);

	if (KEYWORD(KW_SEMI))
		node->syn_count = 0;
	else
		node->syn_arg[0] = parse_value(0, 0);

	return node;
}


static SYN parse_accept(void)
{
/**************************************
 *
 *	p a r s e _ a c c e p t
 *
 **************************************
 *
 * Functional description
 *	Parse form update statement.
 *
 **************************************/
	IBERROR(484);				/* FORMs not supported */
	return 0;
}


static SYN parse_add( USHORT * paren_count, USHORT * bool_flag)
{
/**************************************
 *
 *	p a r s e _ a d d
 *
 **************************************
 *
 * Functional description
 *	Parse the lowest precedence operatrs, ADD and SUBTRACT.
 *
 **************************************/
	SYN node, arg;
	NOD_T operatr;

	node = parse_multiply(paren_count, bool_flag);

	while (TRUE) {
		if (MATCH(KW_PLUS))
			operatr = nod_add;
		else if (MATCH(KW_MINUS))
			operatr = nod_subtract;
		else
			return node;
		arg = node;
		node = SYNTAX_NODE(operatr, 2);
		node->syn_arg[0] = arg;
		node->syn_arg[1] = parse_multiply(paren_count, bool_flag);
	}
}


static SYN parse_and( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ a n d
 *
 **************************************
 *
 * Functional description
 *	Parse an AND expression.
 *
 **************************************/
	SYN expr, node;

	expr = parse_not(paren_count);

/*
while (*paren_count && KEYWORD (KW_RIGHT_PAREN))
    {
    parse_matching_paren();
    (*paren_count)--;
    }
*/

	if (!MATCH(KW_AND))
		return expr;

	node = SYNTAX_NODE(nod_and, 2);
	node->syn_arg[0] = expr;
	node->syn_arg[1] = parse_and(paren_count);

	return node;
}


static SYN parse_assignment(void)
{
/**************************************
 *
 *	p a r s e _ a s s i g n m e n t
 *
 **************************************
 *
 * Functional description
 *	Parse an assignment statement (or give an error).  The
 *	assignment statement can be either a simple assignment
 *	(field = value) or a restructure (relation = rse).  
 *	If the assignment operatr is missing, 
 *	generate an "expected statement" error.
 *
 **************************************/
	SYN node, field;
	QLI_REL relation;
	NAM name, name2;

	node = SYNTAX_NODE(nod_assign, s_asn_count);
	node->syn_arg[s_asn_to] = parse_field_name(&field);
	name = (NAM) field->syn_arg[0];

/* If the next token is an equals sign, the statement really is an
   assignment, and we're off the hook. */

	if (!MATCH(KW_EQUALS))
		ERRQ_print_error(156, name->nam_string, NULL, NULL, NULL, NULL);	/* Msg156 expected statement, encountered %s */

/* See if the "field name" is really a relation reference.  If so,
   turn the assignment into a restructure. */

	if (field->syn_count == 1)
		relation = resolve_relation(0, name->nam_symbol);
	else if (field->syn_count == 2 && name->nam_symbol) {
		name2 = (NAM) field->syn_arg[1];
		relation = resolve_relation(name->nam_symbol, name2->nam_symbol);
	}
	else
		relation = NULL;

	if (relation) {
		ALL_release((FRB) field);
		node->syn_type = nod_restructure;
		node->syn_arg[s_asn_to] = field =
			SYNTAX_NODE(nod_relation, s_rel_count);
		field->syn_arg[s_rel_relation] = (SYN) relation;
		node->syn_arg[s_asn_from] = parse_rse();
	}
	else
		node->syn_arg[s_asn_from] = parse_value(0, 0);

	return node;
}


static SYN parse_boolean( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *	Parse a general boolean expression.  By precedence, handle an OR
 *	here.
 *
 **************************************/
	SYN expr, node;
	USHORT local_count;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	expr = parse_and(paren_count);

/*
while (*paren_count && KEYWORD (KW_RIGHT_PAREN))
    {
    parse_matching_paren();
    (*paren_count)--;
    }
*/

	if (!MATCH(KW_OR)) {
		parse_terminating_parens(paren_count, &local_count);
		return expr;
	}

	node = SYNTAX_NODE(nod_or, 2);
	node->syn_arg[0] = expr;
	node->syn_arg[1] = parse_boolean(paren_count);
	parse_terminating_parens(paren_count, &local_count);

	return node;
}


static SYN parse_copy(void)
{
/**************************************
 *
 *	p a r s e _ c o p y
 *
 **************************************
 *
 * Functional description
 *	Parse a copy command, which copies
 *	one procedure to another.
 *
 **************************************/
	SYN node;

	PAR_real_token();

	if (MATCH(KW_PROCEDURE)) {
		node = SYNTAX_NODE(nod_copy_proc, 2);
		node->syn_arg[0] = (SYN) parse_qualified_procedure();
		MATCH(KW_TO);
		node->syn_arg[1] = (SYN) parse_qualified_procedure();
		return node;
	}

	ERRQ_print_error(157, QLI_token->tok_string, NULL, NULL, NULL, NULL);	/* Msg157 Expected PROCEDURE encountered %s */
	return NULL;
}


static DBB parse_database(void)
{
/**************************************
 *
 *	p a r s e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Pick up a database for a meta-data or
 *	procedure update.  Return NULL if the
 *	token is not a database name.
 *
 **************************************/
	DBB database;
	SYM db_symbol;


	if ((db_symbol = QLI_token->tok_symbol)
		&& db_symbol->sym_type == SYM_database) {
		database = (DBB) db_symbol->sym_object;
		PAR_real_token();
		if (!MATCH(KW_DOT))
			SYNTAX_ERROR(158);	/* Msg158 period in qualified name */
		PAR_real();
		return database;
	}

	if (!QLI_databases)
		IBERROR(159);			/* Msg159 no databases are ready */

	return NULL;
}


static SYN parse_declare(void)
{
/**************************************
 *
 *	p a r s e _ d e c l a r e
 *
 **************************************
 *
 * Functional description
 *	Parse a variable  declaration.
 *
 **************************************/
	SYN node, field_node;
	SYM name, query_name;
	QLI_FLD global_variable;
	USHORT dtype, length, scale;
	SSHORT sub_type, sub_type_missing;
	TEXT *edit_string, *query_header;
	QLI_REL relation;
	NAM db_name, rel_name;

	PAR_token();
	PAR_real();

	dtype = length = scale = 0;
	sub_type = 0;
	sub_type_missing = 1;
	field_node = NULL;
	relation = NULL;
	query_name = NULL;
	edit_string = query_header = NULL;
	name = parse_symbol();

/*if (global_flag) PAR_real();*/

	while (!KEYWORD(KW_SEMI) && !KEYWORD(KW_COMMA)) {
		PAR_real();
		switch (QLI_token->tok_keyword) {
		case KW_SHORT:
		case KW_LONG:
		case KW_FLOAT:
		case KW_DOUBLE:
		case KW_DATE:
		case KW_CHAR:
		case KW_VARYING:
			if (dtype)
				SYNTAX_ERROR(164);	/* Msg164 variable definition clause */
			dtype = parse_dtype(&length, &scale);
			break;

		case KW_BLOB:
			IBERROR(160);		/* Msg160 blob variables are not supported */
			break;

		case KW_SUB_TYPE:
			sub_type = parse_dtype_subtype();
			sub_type_missing = 0;
			break;

		case KW_EDIT_STRING:
			PAR_token();
			if (QLI_token->tok_type != tok_quoted)
				SYNTAX_ERROR(163);	/* Msg163 quoted edit string */
			edit_string =
				make_string(QLI_token->tok_string + 1,
							QLI_token->tok_length - 2);
			PAR_token();
			break;

		case KW_QUERY_NAME:
			PAR_token();
			MATCH(KW_IS);
			if (QLI_token->tok_type != tok_ident)
				SYNTAX_ERROR(199);	/* Msg199 identifier */
			query_name = parse_symbol();
			break;

		case KW_QUERY_HEADER:
			PAR_token();
			query_header = parse_header();
			break;

		case KW_BASED:
			PAR_token();
			MATCH(KW_ON);
			field_node = parse_field_name(0);
			break;

		default:
			SYNTAX_ERROR(164);	/* Msg164 variable definition clause */
			break;
		}
	}

	if (field_node && field_node->syn_count == 3) {
		db_name = (NAM) field_node->syn_arg[0];
		rel_name = (NAM) field_node->syn_arg[1];
		if (!db_name->nam_symbol)
			ERRQ_print_error(165, db_name->nam_string, NULL, NULL, NULL, NULL);	/* Msg165 %s is not a database */
		if (!
			(relation =
			 resolve_relation(db_name->nam_symbol,
							  rel_name->nam_symbol))) ERRQ_print_error(166,
																	   rel_name->
																	   nam_string,
																	   db_name->
																	   nam_string, NULL, NULL, NULL);	/* Msg166 %s is not a relation in database %s */
	}

	if (!dtype && !field_node)
		SYNTAX_ERROR(167);		/* Msg167 variable data type */
	if (field_node && (dtype || length || scale))
		IBERROR(168);			/* Msg168 no datatype may be specified for a variable based on a field */

	node = SYNTAX_NODE(nod_declare, 2);
	global_variable = (QLI_FLD) ALLOCDV(type_fld, length);
	node->syn_arg[0] = (SYN) global_variable;
	global_variable->fld_name = name;
	global_variable->fld_dtype = dtype;
	global_variable->fld_scale = scale;
	global_variable->fld_sub_type = sub_type;
	global_variable->fld_sub_type_missing = sub_type_missing;
	global_variable->fld_length = length;
	global_variable->fld_edit_string = edit_string;
	global_variable->fld_query_name = query_name;
	global_variable->fld_query_header = query_header;
	global_variable->fld_relation = relation;

	node->syn_arg[1] = field_node;

	check_end();

	return node;
}


static SYN parse_define(void)
{
/**************************************
 *
 *	p a r s e _ d e f i n e
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE command.  
 *	There are, of course, a whole class of define commands.  
 *
 **************************************/
	SYN node;

	PAR_real_token();

	if (MATCH(KW_PROCEDURE)) {
		PAR_real();
		node = SYNTAX_NODE(nod_define, 1);
		node->syn_arg[0] = (SYN) parse_qualified_procedure();
		return node;
	}

	if (MATCH(KW_FIELD)) {
		PAR_real();
		node = SYNTAX_NODE(nod_def_field, 2);
		node->syn_arg[0] = (SYN) parse_database();
		node->syn_arg[1] = (SYN) parse_field(TRUE);
		return node;
	}

	if (MATCH(KW_RELATION))
		return parse_def_relation();

	if (KEYWORD(KW_DATABASE))
		return parse_ready(nod_def_database);

	if (MATCH(KW_INDEX))
		return parse_def_index();

	SYNTAX_ERROR(169);			/* Msg169 object type for DEFINE */
	return NULL;
}


static SYN parse_def_index(void)
{
/**************************************
 *
 *	p a r s e _ d e f _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Parse a DEFINE INDEX command.  
 *
 **************************************/
	LLS stack;
	SYN node;

	PAR_real();
	node = SYNTAX_NODE(nod_def_index, s_dfi_count);
	node->syn_arg[s_dfi_name] = (SYN) parse_symbol();
	PAR_real();
	MATCH(KW_FOR);

	if (!(node->syn_arg[s_dfi_relation] = (SYN) parse_qualified_relation()))
		SYNTAX_ERROR(170);		/* Msg170 relation name */

	PAR_real();

	while (TRUE) {
		PAR_real();
		if (MATCH(KW_UNIQUE))
			node->syn_flags |= s_dfi_flag_unique;
		else if (MATCH(KW_DUPLICATE))
			node->syn_flags &= ~s_dfi_flag_unique;
		else if (MATCH(KW_ACTIVE))
			node->syn_flags &= ~s_dfi_flag_inactive;
		else if (MATCH(KW_INACTIVE))
			node->syn_flags |= s_dfi_flag_inactive;
		else if (MATCH(KW_DESCENDING))
			node->syn_flags |= s_dfi_flag_descending;
		else if (MATCH(KW_ASCENDING))
			node->syn_flags &= ~s_dfi_flag_descending;
		else
			break;
	}

	stack = NULL;
	for (;;) {
		LLS_PUSH(parse_name(), &stack);
		if (!MATCH(KW_COMMA))
			break;
	}
	node->syn_arg[s_dfi_fields] = make_list(stack);

	command_end();
	return node;
}


static SYN parse_def_relation(void)
{
/**************************************
 *
 *	p a r s e _ d e f _ r e l a t i o n 
 *
 **************************************
 *
 * Functional description 
 *	Parse a DEFINE RELATION command,
 *	which include the field definitions
 *	for a primitive relation definition
 *	or it may just reference another relatio
 *	whose field definitions we will copy.  
 *
 **************************************/
	QLI_REL relation;
	QLI_FLD field, *ptr;
	SYN node;

	PAR_real();
	node = SYNTAX_NODE(nod_def_relation, 2);
	relation = (QLI_REL) ALLOCD(type_rel);
	node->syn_arg[0] = (SYN) relation;
	relation->rel_database = parse_database();
	relation->rel_symbol = parse_symbol();
	PAR_real();

	if (MATCH(KW_BASED)) {
		PAR_real();
		MATCH(KW_ON);
		PAR_real();
		MATCH(KW_RELATION);
		PAR_real();
		relation = (QLI_REL) ALLOCD(type_rel);
		node->syn_arg[1] = (SYN) relation;
		relation->rel_database = parse_database();
		relation->rel_symbol = parse_symbol();
	}
	else {
		node->syn_arg[1] = NULL;
		ptr = &relation->rel_fields;
		for (;;) {
			MATCH(KW_ADD);
			PAR_real();
			MATCH(KW_FIELD);
			*ptr = field = parse_field(FALSE);
			ptr = &field->fld_next;
			if (KEYWORD(KW_SEMI))
				break;
			if (!MATCH(KW_COMMA))
				SYNTAX_ERROR(171);	/* Msg171 comma between field definitions */
		}
	}

	command_end();
	return node;
}


static SYN parse_delete(void)
{
/**************************************
 *
 *	p a r s e _ d e l e t e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL DELETE statement. 
 *	(DELETE PROCEDURE is parsed in parse_drop)
 *
 **************************************/
	SYN node, rse;

	++sql_flag;

	if (!MATCH(KW_FROM))
		SYNTAX_ERROR(172);		/* Msg172 FROM */

	node = SYNTAX_NODE(nod_erase, s_era_count);
	node->syn_arg[s_era_rse] = rse =
		SYNTAX_NODE(nod_rse, (int) s_rse_count + 2);
	rse->syn_count = 1;
	rse->syn_arg[s_rse_count] = parse_sql_relation();

/* Pick up boolean, if present */

	if (MATCH(KW_WITH))
		rse->syn_arg[s_rse_boolean] = parse_boolean(0);

	--sql_flag;

	return node;
}


static SYN parse_drop(void)
{
/**************************************
 *
 *	p a r s e _ d r o p
 *
 **************************************
 *
 * Functional description
 *	Parse a DDL DROP/DELETE command.  It it isn't one,
 *	just return NULL.
 *
 **************************************/
	SYN node;
	NOD_T type;
	DBB database;
	SSHORT l;
	TEXT *p, *q;

	PAR_real_token();

	if (MATCH(KW_RELATION) || MATCH(KW_VIEW) || MATCH(KW_TABLE)) {
		node = SYNTAX_NODE(nod_del_relation, 1);
		if (!(node->syn_arg[0] = (SYN) parse_qualified_relation()))
			SYNTAX_ERROR(173);	/* Msg173 relation or view name */
		return node;
	}

	switch (QLI_token->tok_keyword) {
	case KW_PROCEDURE:
		type = nod_delete_proc;
		break;

	case KW_INDEX:
		type = nod_del_index;
		break;

	case KW_FIELD:
		type = nod_del_field;
		break;

	case KW_DATABASE:
		LEX_filename();
		if (!(l = QLI_token->tok_length))
			ERRQ_error(429, NULL, NULL, NULL, NULL, NULL);	/* Msg429 database file name required on DROP DATABASE */
		q = QLI_token->tok_string;
		if (QLI_token->tok_type == tok_quoted) {
			l -= 2;
			q++;
		}
		database = (DBB) ALLOCDV(type_dbb, l);
		database->dbb_filename_length = l;
		p = database->dbb_filename;
		do
			*p++ = *q++;
		while (--l);
		PAR_token();

		/* parse an optional user name and password if given */

		for (;;) {
			if (MATCH(KW_USER))
				database->dbb_user = parse_literal();
			else if (MATCH(KW_PASSWORD))
				database->dbb_password = parse_literal();
			else
				break;
		}

		command_end();
		node = SYNTAX_NODE(nod_del_database, 1);
		node->syn_arg[0] = (SYN) database;
		return node;

	default:
		return NULL;
	}

	PAR_real_token();
	node = SYNTAX_NODE(type, 2);

	if (type == nod_delete_proc)
		node->syn_arg[0] = (SYN) parse_qualified_procedure();
	else {
		node->syn_arg[0] = (SYN) parse_database();
		node->syn_arg[1] = (SYN) parse_name();
	}

	return node;
}


static int parse_dtype( USHORT * length, USHORT * scale)
{
/**************************************
 *
 *	p a r s e _ d t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a datatype clause.
 *
 **************************************/
	KWWORDS keyword;
	USHORT dtype, l;

	keyword = QLI_token->tok_keyword;
	PAR_token();
	*scale = 0;

	switch (keyword) {
	case KW_SHORT:
		*length = sizeof(SSHORT);
		dtype = dtype_short;
		break;

	case KW_LONG:
		*length = sizeof(SLONG);
		dtype = dtype_long;
		break;

	case KW_FLOAT:
		*length = sizeof(float);
		return dtype_real;

	case KW_DOUBLE:
		*length = sizeof(double);
		return dtype_double;

	case KW_DATE:
		*length = sizeof(gds_quad);
		return dtype_timestamp;

	case KW_CHAR:
		dtype = dtype_text;
		break;

	case KW_VARYING:
		dtype = dtype_varying;
		break;

	case KW_BLOB:
		*length = sizeof(gds_quad);
		return dtype_blob;
	}

	if (dtype == dtype_short || dtype == dtype_long) {
		if (MATCH(KW_SCALE)) {
			l = (MATCH(KW_MINUS)) ? TRUE : FALSE;
			*scale = parse_ordinal();
			if (l)
				*scale = -(*scale);
		}
	}
	else if (dtype == dtype_text || dtype == dtype_varying) {
		if (!MATCH(KW_L_BRCKET) && !MATCH(KW_LT))
			SYNTAX_ERROR(174);	/* Msg174 "[" */

		l = parse_ordinal();
		if (dtype == dtype_varying)
			l += sizeof(SSHORT);
		*length = l;

		if (!MATCH(KW_R_BRCKET) && !MATCH(KW_GT))
			SYNTAX_ERROR(175);	/* Msg175 "]" */

	}

	return dtype;
}


static int parse_dtype_subtype(void)
{
/**************************************
 *
 *	p a r s e _ d t y p e _ s u b t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a sub-type definition, which can be any of
 *	SUB_TYPE {IS} [TEXT | FIXED | <n>]
 *
 *	Returns the numeric subtype value,
 *
 **************************************/
	int sign;

/* grab KW_SUB_TYPE */

	PAR_token();
	MATCH(KW_IS);
	if (MATCH(KW_TEXT) || MATCH(KW_FIXED))
		return 1;

	sign = (MATCH(KW_MINUS)) ? -1 : 1;

	return (sign * parse_ordinal());
}


static SYN parse_edit(void)
{
/**************************************
 *
 *	p a r s e _ e d i t 
 *
 **************************************
 *
 * Functional description
 *	Parse an edit statement which can
 *	be any of  EDIT <procedure_name>
 *	           EDIT <n> 
 *		   EDIT <*>
 *		   EDIT 
 *
 *
 **************************************/
	SYN node;
	NOD_T type;
	int l;
	LLS start, stop, statement_list;

	LEX_token();

/*
 * edit previous statements.  The top of the statment list
 * is this edit command, which we conveniently ignore.
 */

	if (KEYWORD(KW_SEMI) ||
		(QLI_token->tok_type == tok_number) || (KEYWORD(KW_ASTERISK))) {
		if (!(statement_list = LEX_statement_list()))
			IBERROR(176);		/* Msg176 No statements issued yet */

		if (MATCH(KW_ASTERISK))
			LEX_edit((SLONG) 0, (SLONG) statement_list->lls_object);
		else {
			if (KEYWORD(KW_SEMI))
				l = 1;
			else if (QLI_token->tok_type == tok_number)
				l = parse_ordinal();

			for (start = stop = statement_list;
				 l && start->lls_next; l--, start = start->lls_next);
			command_end();
			LEX_edit((SLONG) start->lls_object, (SLONG) stop->lls_object);
		}
	}
#ifdef PYXIS
	else if (MATCH(KW_FORM))
		IBERROR(484);			/* FORMs not supported */
#endif
	else {
		type = nod_edit_proc;
		node = SYNTAX_NODE(type, 2);
		node->syn_arg[0] = (SYN) parse_qualified_procedure();
		command_end();
		return node;
	}

	return NULL;
}


static TEXT *parse_edit_string(void)
{
/**************************************
 *
 *	p a r s e _ e d i t _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Look for and parse a clause of the form:
 *	    USING <edit_string>
 *
 **************************************/

	if (!KEYWORD(KW_USING))
		return NULL;

	LEX_edit_string();

	return parse_string();
}


static SYN parse_erase(void)
{
/**************************************
 *
 *	p a r s e _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Parse an ERASE statement.  Erase can be any of the
 *	following:
 *
 *	ERASE [ALL] [OF <rse>]
 *
 **************************************/
	SYN node;

	PAR_token();
	MATCH(KW_ALL);
	MATCH(KW_OF);
	node = SYNTAX_NODE(nod_erase, s_era_count);

	if (MATCH(KW_ALL) || potential_rse()) {
		MATCH(KW_OF);
		node->syn_arg[s_era_rse] = parse_rse();
	}

	return node;
}


static SYN parse_extract(void)
{
/**************************************
 *
 *	p a r s e _ e x t r a c t
 *
 **************************************
 *
 * Functional description
 *	Parse a procedure extract statement.  Syntax is:
 *
 *	EXTRACT [ON <file>] proc [, ...] [ON <file> ]
 *
 **************************************/
	LLS stack;
	SYN node;

	PAR_real_token();
	node = SYNTAX_NODE(nod_extract, 2);
	node->syn_arg[1] = parse_output();

	if (!MATCH(KW_ALL)) {
		stack = NULL;
		for (;;) {
			LLS_PUSH(parse_qualified_procedure(), &stack);
			if (!MATCH(KW_COMMA))
				break;
		}
		node->syn_arg[0] = make_list(stack);
	}

	if (!node->syn_arg[1] && !(node->syn_arg[1] = parse_output()))
		SYNTAX_ERROR(177);		/* Msg177 "ON or TO" */

	return node;
}


static QLI_FLD parse_field( int global_flag)
{
/**************************************
 *
 *	p a r s e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a field description.
 *
 **************************************/
	QLI_FLD field;
	SYM name, query_name, based_on;
	USHORT dtype, length, scale;
	SSHORT sub_type, sub_type_missing;
	TEXT *edit_string, *query_header;

	PAR_real();

	dtype = length = scale = 0;
	sub_type = 0;
	sub_type_missing = 1;
	query_name = based_on = NULL;
	edit_string = query_header = NULL;
	name = parse_symbol();

	if (global_flag)
		PAR_real();

	while (!KEYWORD(KW_SEMI) && !KEYWORD(KW_COMMA)) {
		PAR_real();
		switch (QLI_token->tok_keyword) {
		case KW_SHORT:
		case KW_LONG:
		case KW_FLOAT:
		case KW_DOUBLE:
		case KW_DATE:
		case KW_CHAR:
		case KW_VARYING:
		case KW_BLOB:
			if (dtype)
				SYNTAX_ERROR(179);	/* Msg179 field definition clause */
			dtype = parse_dtype(&length, &scale);
			break;

		case KW_SUB_TYPE:
			sub_type = parse_dtype_subtype();
			sub_type_missing = 0;
			break;

		case KW_EDIT_STRING:
			PAR_token();
			if (QLI_token->tok_type != tok_quoted)
				SYNTAX_ERROR(178);	/* Msg178 quoted edit string */
			edit_string =
				make_string(QLI_token->tok_string + 1,
							QLI_token->tok_length - 2);
			PAR_token();
			break;

		case KW_QUERY_NAME:
			PAR_token();
			MATCH(KW_IS);
			if (QLI_token->tok_type != tok_ident)
				SYNTAX_ERROR(199);	/* Msg199 identifier */
			query_name = parse_symbol();
			break;

		case KW_BASED:
			PAR_token();
			MATCH(KW_ON);
			based_on = parse_symbol();
			break;

		default:
			SYNTAX_ERROR(179);	/* Msg179 field definition clause */
			break;
		}
	}

	field = (QLI_FLD) ALLOCDV(type_fld, length);
	field->fld_name = name;
	field->fld_dtype = dtype;
	field->fld_scale = scale;
	field->fld_sub_type = sub_type;
	field->fld_sub_type_missing = sub_type_missing;
	field->fld_length = length;
	field->fld_edit_string = edit_string;
	field->fld_query_name = query_name;
	field->fld_query_header = query_header;
	if (!global_flag)
		field->fld_based = based_on;
	else if (based_on)
		IBERROR(180);			/* Msg180 global fields may not be based on other fields */

	return field;
}


static SYN parse_field_name( SYN * fld_ptr)
{
/**************************************
 *
 *	p a r s e _ f i e l d _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Parse a qualified field name, or
 *	qualified * expression.
 *
 **************************************/
	SYN field, node;
	LLS stack;

	stack = NULL;

	while (TRUE) {
		if (MATCH(KW_ASTERISK)) {
			if (!stack)
				SYNTAX_ERROR(181);	/* Msg181 field name or asterisk expression */
			field = make_list(stack);
			field->syn_type = nod_star;
			return field;
		}
		LLS_PUSH(parse_name(), &stack);
		if (!MATCH(KW_DOT))
			break;
	}

	field = make_list(stack);
	field->syn_type = nod_field;
	if (fld_ptr)
		*fld_ptr = field;
	if (!(MATCH(KW_L_BRCKET)))
		return field;

/* Parse an array reference */

	stack = NULL;
	for (;;) {
		LLS_PUSH(parse_value(0, 0), &stack);
		if (MATCH(KW_R_BRCKET))
			break;
		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR(183);	/* Msg183 comma */
	}

	node = SYNTAX_NODE(nod_index, s_idx_count);
	node->syn_arg[s_idx_field] = field;
	node->syn_arg[s_idx_subs] = make_list(stack);

	return node;
}


static SYN parse_for(void)
{
/**************************************
 *
 *	p a r s e _ f o r
 *
 **************************************
 *
 * Functional description
 *	Parse a FOR statement.
 *
 **************************************/
	SYN node;

	PAR_token();
#ifdef PYXIS
	if (MATCH(KW_FORM))
		IBERROR(484);			/* FORMs not supported */
#endif
	node = SYNTAX_NODE(nod_for, s_for_count);
	node->syn_arg[s_for_rse] = parse_rse();
	node->syn_arg[s_for_statement] = parse_statement();

	return node;
}

#ifdef PYXIS
static SYN parse_form(void)
{
/**************************************
 *
 *	p a r s e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Parse qualified form name, returning a form block.
 *
 **************************************/
	IBERROR(484);				/* FORMs not supported */
}
#endif

static SYN parse_from( USHORT * paren_count, USHORT * bool_flag)
{
/**************************************
 *
 *	p a r s e _ f r o m
 *
 **************************************
 *
 * Functional description
 *	Parse either an explicit or implicit FIRST ... FROM statement.
 *
 **************************************/
	SYN node, value;

	PAR_real();

	if (MATCH(KW_FIRST)) {
		value = parse_primitive_value(0, 0);
		PAR_real();
		if (!MATCH(KW_FROM))
			SYNTAX_ERROR(182);	/* Msg182 FROM rse clause */
	}
	else {
		value = parse_primitive_value(paren_count, bool_flag);
		if (sql_flag || !MATCH(KW_FROM))
			return value;
	}

	node = SYNTAX_NODE(nod_from, s_stt_count);
	node->syn_arg[s_stt_value] = value;
	node->syn_arg[s_stt_rse] = parse_rse();

	if (MATCH(KW_ELSE))
		node->syn_arg[s_stt_default] = parse_value(0, 0);

	return node;
}


static SYN parse_function(void)
{
/**************************************
 *
 *	p a r s e _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a function reference.
 *
 **************************************/
	SYN node;
	LLS stack;

	function_count++;
	node = SYNTAX_NODE(nod_function, s_fun_count);
	node->syn_arg[s_fun_function] = (SYN) QLI_token->tok_symbol;
	node->syn_count = 1;
	PAR_token();
	stack = NULL;

	if (MATCH(KW_LEFT_PAREN))
		for (;;) {
			LLS_PUSH(parse_value(0, 0), &stack);
			if (MATCH(KW_RIGHT_PAREN))
				break;
			if (!MATCH(KW_COMMA))
				SYNTAX_ERROR(183);	/* Msg183 comma */
		}

	node->syn_arg[s_fun_args] = make_list(stack);
	function_count--;

	return node;
}


static TEXT *parse_header(void)
{
/**************************************
 *
 *	p a r s e _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Parse and store headers of the form:
 *		"quoted_string" [/ "more_string"]...
 *		or even the non-header -
 *
 **************************************/
	TEXT *p, *q, header[1024];

	p = header;

	while (TRUE) {
		PAR_real();
		if ((QLI_token->tok_keyword != KW_MINUS) &&
			(QLI_token->tok_type != tok_quoted)) SYNTAX_ERROR(184);	/* Msg184 quoted header segment */
		q = QLI_token->tok_string;
		while (*q)
			*p++ = *q++;
		PAR_real_token();
		if (!MATCH(KW_SLASH))
			break;
	}

	return make_string(header, p - header);
}


static SYN parse_help(void)
{
/**************************************
 *
 *	p a r s e _ h e l p
 *
 **************************************
 *
 * Functional description
 *	Parse HELP statement.  Unreasonable, but the masses
 *	must be appeased.  Bread, circuses, help.
 *
 **************************************/
	SYN node;
	LLS stack;

	stack = NULL;
	PAR_token();

	while (!KEYWORD(KW_SEMI)) {
		LLS_PUSH(parse_name(), &stack);
		MATCH(KW_COMMA);
	}

	node = make_list(stack);
	node->syn_type = nod_help;
	command_end();

	return node;
}


static SYN parse_if(void)
{
/**************************************
 *
 *	p a r s e _ i f
 *
 **************************************
 *
 * Functional description
 *	Parse an IF THEN ELSE statement.
 *
 **************************************/
	SYN node;

	PAR_token();
	node = SYNTAX_NODE(nod_if, s_if_count);
	node->syn_arg[s_if_boolean] = parse_boolean(0);
	PAR_real();
	MATCH(KW_THEN);
	++else_count;
	node->syn_arg[s_if_true] = parse_statement();
	--else_count;

	if (MATCH(KW_ELSE))
		node->syn_arg[s_if_false] = parse_statement();

	return node;
}


static SYN parse_in( SYN value, NOD_T operatr, USHORT all_flag)
{
/**************************************
 *
 *	p a r s e _ i n
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL "IN" clause.  This can have two forms:
 *
 *		value IN (exp1, exp2...)
 *
 *		value IN (column <select expression>)
 *
 **************************************/
	SYN node, node2, value2, rse;

	PAR_real();

	if (!MATCH(KW_LEFT_PAREN))
		SYNTAX_ERROR(185);		/* Msg185 left parenthesis */

/* Time to chose between two forms of the expression */

	if (!MATCH(KW_SELECT)) {
		node = SYNTAX_NODE(operatr, 2);
		node->syn_arg[0] = value;
		node->syn_arg[1] = parse_primitive_value(0, 0);
		while (MATCH(KW_COMMA)) {
			node2 = node;
			node = SYNTAX_NODE(nod_or, 2);
			node->syn_arg[0] = node2;
			node->syn_arg[1] = node2 = SYNTAX_NODE(nod_eql, 2);
			node2->syn_arg[0] = value;
			node2->syn_arg[1] = parse_value(0, 0);
		}
		parse_matching_paren();
		return node;
	}

	value2 = parse_value(0, 0);

/* We have the "hard" -- an implicit ANY */

	rse = parse_sql_rse();
	parse_matching_paren();

	rse->syn_arg[s_rse_outer] = value;
	rse->syn_arg[s_rse_inner] = value2;
	rse->syn_arg[s_rse_op] = INT_CAST operatr;
	rse->syn_arg[s_rse_all_flag] = INT_CAST all_flag;

/* Finally, construct an ANY node */

	node = SYNTAX_NODE(nod_any, 1);
	node->syn_arg[0] = rse;

	return (all_flag) ? negate(node) : node;
}


static SYN parse_insert(void)
{
/**************************************
 *
 *	p a r s e _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Parse a STORE statement.
 *
 **************************************/
	SYN node;
	USHORT select_flag;
	LLS fields, values, distinct;

	++sql_flag;
	PAR_real_token();
	MATCH(KW_INTO);
	node = SYNTAX_NODE(nod_store, s_sto_count);

/* Pick up relation name for insert */

	node->syn_arg[s_sto_relation] = parse_sql_relation();

/* Pick up field list, provided one is present */

	distinct = fields = values = NULL;
	PAR_real();

	if (MATCH(KW_LEFT_PAREN))
		while (TRUE) {
			LLS_PUSH(parse_field_name(0), &fields);
			if (MATCH(KW_RIGHT_PAREN))
				break;
			if (!MATCH(KW_COMMA))
				SYNTAX_ERROR(186);	/* Msg186 comma or terminating right parenthesis */
		}

/* Pick up value list or SELECT statement */

	PAR_real();

	if (MATCH(KW_VALUES)) {
		select_flag = FALSE;
		if (!MATCH(KW_LEFT_PAREN))
			SYNTAX_ERROR(187);	/* Msg187 left parenthesis */
	}
	else if (MATCH(KW_SELECT))
		select_flag = TRUE;
	else
		SYNTAX_ERROR(188);		/* Msg188 VALUES list or SELECT clause */


	while (TRUE) {
		if (distinct || MATCH(KW_DISTINCT)) {
			LLS_PUSH(parse_value(0, 0), &distinct);
			LLS_PUSH(distinct->lls_object, &values);
			LLS_PUSH(0, &distinct);
		}
		else
			LLS_PUSH(parse_value(0, 0), &values);
		if (!MATCH(KW_COMMA))
			break;
	}

	if (select_flag)
		node->syn_arg[s_sto_rse] = parse_sql_rse();
	else
		parse_matching_paren();

	if (distinct)
		node->syn_arg[s_sto_rse]->syn_arg[s_rse_reduced] =
			make_list(distinct);

	node->syn_arg[s_sto_fields] = (SYN) fields;
	node->syn_arg[s_sto_values] = (SYN) values;

	--sql_flag;

	return node;
}


static NOD_T parse_join_type(void)
{
/**************************************
 *
 *	p a r s e _ j o i n _ t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a join type.
 *
 **************************************/
	NOD_T operatr;

	if (MATCH(KW_INNER))
		operatr = nod_join_inner;
	else if (MATCH(KW_LEFT))
		operatr = nod_join_left;
	else if (MATCH(KW_RIGHT))
		operatr = nod_join_right;
	else if (MATCH(KW_FULL))
		operatr = nod_join_full;
	else if (MATCH(KW_JOIN))
		return nod_join_inner;
	else
		return (NOD_T) 0;

	if (operatr != nod_join_inner)
		MATCH(KW_OUTER);

	if (!MATCH(KW_JOIN))
		SYNTAX_ERROR(489);		/* Msg489 JOIN */

	return operatr;
}


static SYN parse_list_fields(void)
{
/**************************************
 *
 *	p a r s e _ l i s t _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Parse a LIST statement.  LIST is like PRINT, but does vertical
 *	formatting.
 *
 **************************************/
	SYN node, item;
	LLS stack;

	node = SYNTAX_NODE(nod_list_fields, s_prt_count);
	PAR_token();

	if (!MATCH(KW_ALL) && MATCH(KW_DISTINCT))
		node->syn_arg[s_prt_distinct] = INT_CAST TRUE;

	if (node->syn_arg[s_prt_output] = parse_output())
		return node;

	if (test_end())
		return node;

/* If there is a potential record selection expression, there obviously
   can't be a print list.  Get the rse.  Otherwise, pick up the print
   list. */

	if (potential_rse())
		node->syn_arg[s_prt_rse] = parse_rse();
	else {
		if (!test_end()) {
			stack = NULL;
			for (;;) {
				item = SYNTAX_NODE(nod_print_item, s_itm_count);
				item->syn_arg[s_itm_value] = parse_value(0, 0);
				item->syn_arg[s_itm_edit_string] = (SYN) parse_edit_string();
				LLS_PUSH(item, &stack);
				if (!MATCH(KW_COMMA) && !MATCH(KW_AND))
					break;
				PAR_real();
				if (MATCH(KW_AND))
					PAR_real();
			}
			node->syn_arg[s_prt_list] = make_list(stack);
		}
		if (MATCH(KW_OF))
			node->syn_arg[s_prt_rse] = parse_rse();
	}

	node->syn_arg[s_prt_output] = parse_output();

	return node;
}


static CON parse_literal(void)
{
/**************************************
 *
 *	p a r s e _ l i t e r a l
 *
 **************************************
 *
 * Functional description
 *	Parse a literal expression.
 *
 **************************************/
	CON constant;
	USHORT l;
	UCHAR *p, *q;

	PAR_real();
	q = (UCHAR *) QLI_token->tok_string;
	l = QLI_token->tok_length;

	if (QLI_token->tok_type == tok_quoted) {
		q++;
		l -= 2;
		constant = (CON) ALLOCDV(type_con, l);
		constant->con_desc.dsc_dtype = dtype_text;
		p = constant->con_desc.dsc_address = constant->con_data;
		if (constant->con_desc.dsc_length = l)
			do
				*p++ = *q++;
			while (--l);
	}
	else if (QLI_token->tok_type == tok_number)
		constant = make_numeric_constant(QLI_token->tok_string,
										 QLI_token->tok_length);
	else
		SYNTAX_ERROR(190);		/* Msg190 value expression */

	PAR_token();

	return constant;
}


static SYN parse_matches(void)
{
/**************************************
 *
 *	p a r s e _ m a t c h e s
 *
 **************************************
 *
 * Functional description
 *	Parse a matching expressing, including
 *	the preset matching language template.
 *
 **************************************/
	SYN node, language;

	PAR_token();
	node = SYNTAX_NODE(nod_sleuth, 3);
	node->syn_arg[1] = parse_value(0, 0);
	if (MATCH(KW_USING))
		node->syn_arg[2] = parse_value(0, 0);
	else if (QLI_matching_language) {
		node->syn_arg[2] = language = SYNTAX_NODE(nod_constant, 1);
		language->syn_arg[0] = (SYN) QLI_matching_language;
	}
	else {
		node->syn_type = nod_matches;
		node->syn_count = 2;
	}

	return node;
}


static void parse_matching_paren(void)
{
/**************************************
 *
 *	p a r s e _ m a t c h i n g _ p a r e n
 *
 **************************************
 *
 * Functional description
 *	Check for a trailing (right) parenthesis.  Complain if
 *	its not there.
 *
 **************************************/

	PAR_real();

	if (MATCH(KW_RIGHT_PAREN))
		return;

	SYNTAX_ERROR(191);			/* Msg191 right parenthesis */
}


static SYN parse_menu(void)
{
/**************************************
 *
 *	p a r s e _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Parse a MENU statement.
 *
 **************************************/
	LLS labels, statements;
	SYN node;

	PAR_real_token();

	if (QLI_token->tok_type != tok_quoted)
		SYNTAX_ERROR(192);		/* Msg192 quoted string */

	node = SYNTAX_NODE(nod_menu, s_men_count);
	node->syn_arg[s_men_string] = parse_primitive_value(0, 0);
	labels = statements = NULL;
	PAR_real();

	while (!MATCH(KW_END)) {
		if (!MATCH(KW_ENTREE))
			SYNTAX_ERROR(193);	/* Msg193 ENTREE or END */
		if (QLI_token->tok_type != tok_quoted)
			SYNTAX_ERROR(194);	/* Msg194 quoted string */
		LLS_PUSH(parse_primitive_value(0, 0), &labels);
		MATCH(KW_COLON);
		LLS_PUSH(parse_statement(), &statements);
		PAR_real();
	}

	node->syn_arg[s_men_statements] = make_list(statements);
	node->syn_arg[s_men_labels] = make_list(labels);

	return node;
}


static SYN parse_modify(void)
{
/**************************************
 *
 *	p a r s e _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY statement.  Modify can be any of the
 *	following:
 *
 *	MODIFY [ALL] [<field> [, <field>]...] [OF <rse> ]
 *	MODIFY [ALL] USING <statement> [OF <rse>]
 *
 **************************************/
	SYN node;
	LLS stack;

	PAR_token();

/* If this is a meta-data change, handle it elsewhere */

	if (MATCH(KW_INDEX))
		return parse_modify_index();

	if (MATCH(KW_FIELD)) {
		node = SYNTAX_NODE(nod_mod_field, 1);
		node->syn_arg[0] = (SYN) parse_database();
		node->syn_arg[1] = (SYN) parse_field(TRUE);
		return node;
	}

	if (MATCH(KW_RELATION))
		return parse_modify_relation();

/* Not a meta-data modification, just a simple data modify */

	MATCH(KW_ALL);
	node = SYNTAX_NODE(nod_modify, s_mod_count);

	if (MATCH(KW_USING))
#if PYXIS
		if (MATCH(KW_FORM))
			IBERROR(484);		/* FORMs not supported */
		else
#endif
			node->syn_arg[s_mod_statement] = parse_statement();
	else if (!KEYWORD(KW_SEMI)) {
		stack = NULL;
		while (TRUE) {
			LLS_PUSH(parse_field_name(0), &stack);
			if (!MATCH(KW_COMMA))
				break;
		}
		node->syn_arg[s_mod_list] = make_list(stack);
	}

	if (MATCH(KW_OF))
		node->syn_arg[s_mod_rse] = parse_rse();

	return node;
}


static SYN parse_modify_index(void)
{
/**************************************
 *
 *	p a r s e _ m o d i f y _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY INDEX statement. 
 *
 **************************************/
	SYN node;

	node = SYNTAX_NODE(nod_mod_index, s_mfi_count);
	node->syn_arg[s_mfi_database] = (SYN) parse_database();
	node->syn_arg[s_mfi_name] = (SYN) parse_name();
	PAR_real();

	while (TRUE) {
		if (MATCH(KW_UNIQUE))
			node->syn_flags |= (s_dfi_flag_selectivity | s_dfi_flag_unique);
		else if (MATCH(KW_DUPLICATE)) {
			node->syn_flags |= s_dfi_flag_selectivity;
			node->syn_flags &= ~s_dfi_flag_unique;
		}
		else if (MATCH(KW_INACTIVE))
			node->syn_flags |= (s_dfi_flag_activity | s_dfi_flag_inactive);
		else if (MATCH(KW_ACTIVE)) {
			node->syn_flags |= s_dfi_flag_activity;
			node->syn_flags &= ~s_dfi_flag_inactive;
		}
		else if (MATCH(KW_DESCENDING))
			node->syn_flags |= (s_dfi_flag_order | s_dfi_flag_descending);
		else if (MATCH(KW_ASCENDING)) {
			node->syn_flags |= s_dfi_flag_order;
			node->syn_flags &= ~s_dfi_flag_inactive;
		}
		else if (MATCH(KW_STATISTICS))
			node->syn_flags |= s_dfi_flag_statistics;
		else
			break;
	}

	if (!node->syn_flags)
		SYNTAX_ERROR(195);		/* Msg195 index state option */

	command_end();

	return node;
}


static SYN parse_modify_relation(void)
{
/**************************************
 *
 *	p a r s e _ m o d i f y _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a MODIFY RELATION statement. 
 *
 **************************************/
	SYN node;
	QLI_REL relation;
	QLI_FLD field;

	node = SYNTAX_NODE(nod_mod_relation, 2);
	relation = parse_qualified_relation();
	node->syn_arg[0] = (SYN) relation;

	if (!relation)
		SYNTAX_ERROR(196);		/* Msg196 relation name */

	for (;;) {
		PAR_real();
		if (MATCH(KW_ADD)) {
			PAR_real();
			MATCH(KW_FIELD);
			field = parse_field(FALSE);
		}
		else if (MATCH(KW_MODIFY)) {
			PAR_real();
			MATCH(KW_FIELD);
			field = parse_field(FALSE);
			field->fld_flags |= FLD_modify;
		}
		else if (MATCH(KW_DROP)) {
			PAR_real();
			MATCH(KW_FIELD);
			field = parse_field(FALSE);
			field->fld_flags |= FLD_drop;
		}
		else
			SYNTAX_ERROR(197);	/* Msg197 ADD, MODIFY, or DROP */
		field->fld_next = (QLI_FLD) node->syn_arg[1];
		node->syn_arg[1] = (SYN) field;
		if (KEYWORD(KW_SEMI))
			break;
		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR(198);	/* Msg198 comma between field definitions */
	}

	command_end();

	return node;
}


static SYN parse_multiply( USHORT * paren_count, USHORT * bool_flag)
{
/**************************************
 *
 *	p a r s e _ m u l t i p l y
 *
 **************************************
 *
 * Functional description
 *	Parse the operatrs * and /.
 *
 **************************************/
	SYN node, arg;
	NOD_T operatr;

	node = parse_from(paren_count, bool_flag);

	while (TRUE) {
		if (MATCH(KW_ASTERISK))
			operatr = nod_multiply;
		else if (MATCH(KW_SLASH))
			operatr = nod_divide;
		else
			return node;
		arg = node;
		node = SYNTAX_NODE(operatr, 2);
		node->syn_arg[0] = arg;
		node->syn_arg[1] = parse_from(paren_count, bool_flag);
	}
}


static NAM parse_name(void)
{
/**************************************
 *
 *	p a r s e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Turn current token into a name and get the next token.
 *
 **************************************/
	NAM name;
	SSHORT l;
	TEXT c, *p, *q;

	PAR_real();

	if (QLI_token->tok_type != tok_ident)
		SYNTAX_ERROR(199);		/* Msg199 identifier */

	l = QLI_token->tok_length;
	name = (NAM) ALLOCDV(type_nam, l);
	name->nam_length = l;
	name->nam_symbol = QLI_token->tok_symbol;
	q = QLI_token->tok_string;
	p = name->nam_string;

	if (l)
		do {
			c = *q++;
			*p++ = UPPER(c);

		} while (--l);

	PAR_token();

	return name;
}


static SYN parse_not( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ n o t
 *
 **************************************
 *
 * Functional description
 *	Parse a prefix NOT expression.
 *
 **************************************/

	PAR_real();

	if (!MATCH(KW_NOT))
		return parse_relational(paren_count);

	return negate(parse_not(paren_count));
}


static int parse_ordinal(void)
{
/**************************************
 *
 *	p a r s e _ o r d i n a l
 *
 **************************************
 *
 * Functional description
 *	Pick up a simple number as a number.  This is 
 *	used for SKIP [n], SPACE [n], COL n, and SQL 
 *	positions.
 *
 **************************************/
	int n;

	PAR_real();

	if (QLI_token->tok_type != tok_number)
		SYNTAX_ERROR(200);		/* Msg200 positive number */

	n = atoi(QLI_token->tok_string);
	if (n < 0)
		SYNTAX_ERROR(200);		/* Msg200 positive number */
	PAR_token();

	return n;
}


static SYN parse_output(void)
{
/**************************************
 *
 *	p a r s e _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Parse an output clause the the absence thereof.
 *
 **************************************/
	SYN node;
	USHORT flag;

	if (MATCH(KW_ON))
		flag = FALSE;
	else if (MATCH(KW_TO))
		flag = TRUE;
	else
		return NULL;

	node = SYNTAX_NODE(nod_output, s_out_count);
	node->syn_arg[s_out_file] = parse_value(0, 0);
	node->syn_arg[s_out_pipe] = INT_CAST flag;

	return node;
}


static SYN parse_primitive_value( USHORT * paren_count, USHORT * bool_flag)
{
/**************************************
 *
 *	p a r s e _ p r i m i t i v e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Pick up a value expression.  This may be either a field reference
 *	or a constant value.
 *
 **************************************/
	SYM symbol;
	SYN node, sub;
	CON constant;
	UCHAR *p;
	USHORT local_count;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	PAR_real();

	switch (next_keyword()) {
	case KW_LEFT_PAREN:
		PAR_token();
		(*paren_count)++;
		if (bool_flag && *bool_flag)
			node = parse_boolean(paren_count);
		else
			node = parse_value(paren_count, bool_flag);
/*
	if (*paren_count && KEYWORD (KW_RIGHT_PAREN))
*/
		{
			parse_matching_paren();
			(*paren_count)--;
		}
		break;

	case KW_PLUS:
		PAR_token();
		return parse_primitive_value(paren_count, 0);

	case KW_MINUS:
		PAR_token();
		sub = parse_primitive_value(paren_count, 0);
		if (sub->syn_type == nod_constant) {
			constant = (CON) sub->syn_arg[0];
			p = constant->con_desc.dsc_address;
			switch (constant->con_desc.dsc_dtype) {
			case dtype_long:
				*(SLONG *) p = -*(SLONG *) p;
				return sub;
			case dtype_text:
				*p = '-';
				return sub;
			}
		}
		node = SYNTAX_NODE(nod_negate, 1);
		node->syn_arg[0] = sub;
		break;

	case KW_ASTERISK:
		node = parse_prompt();
		break;

	case KW_EDIT:
		PAR_token();
		node = SYNTAX_NODE(nod_edit_blob, 1);
		if (!KEYWORD(KW_SEMI))
			node->syn_arg[0] = parse_value(0, 0);
		break;

	case KW_FORMAT:
		PAR_token();
		node = SYNTAX_NODE(nod_format, s_fmt_count);
		node->syn_arg[s_fmt_value] = parse_value(0, 0);
		node->syn_arg[s_fmt_edit] = (SYN) parse_edit_string();
		break;

	case KW_NULL:
		PAR_token();
		node = SYNTAX_NODE(nod_null, 0);
		break;

	case KW_USER_NAME:
		PAR_token();
		node = SYNTAX_NODE(nod_user_name, 0);
		break;

	case KW_COUNT:
	case KW_MAX:
	case KW_MIN:
	case KW_AVERAGE:
	case KW_TOTAL:
		node = parse_statistical();
		break;

	case KW_RUNNING:
		if (function_count > 0)
			IBERROR(487);		/* Msg487 Invalid argument for UDF */
		PAR_real_token();
		node = SYNTAX_NODE(nod_running_total, s_stt_count);
		if (MATCH(KW_COUNT))
			node->syn_type = nod_running_count;
		else {
			MATCH(KW_TOTAL);
			node->syn_arg[s_stt_value] = parse_value(0, 0);
		}
		break;

	case KW_SELECT:
		node = parse_sql_subquery();
		break;

	default:
		if ((symbol = QLI_token->tok_symbol)
			&& symbol->sym_type == SYM_function) {
			node = parse_function();
			break;
		}
		if (QLI_token->tok_type == tok_ident) {
			node = parse_field_name(0);
			break;
		}
		node = SYNTAX_NODE(nod_constant, 1);
		node->syn_arg[0] = (SYN) parse_literal();
	}

	return node;
}


static SYN parse_print_list(void)
{
/**************************************
 *
 *	p a r s e _ p r i n t _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Pick up a print item.  The syntax of a print item is:
 *
 *	<value> [ '[ <query_header> '] ] [USING <edit_string> ]
 *
 **************************************/
	NOD_T op;
	SYN node;
	LLS stack;

	stack = NULL;

	while (TRUE) {
		if (MATCH(KW_SKIP))
			op = nod_skip;
		else if (MATCH(KW_SPACE))
			op = nod_space;
		else if (MATCH(KW_TAB))
			op = nod_tab;
		else if (MATCH(KW_COLUMN))
			op = nod_column;
		else if (MATCH(KW_NEW_PAGE))
			op = nod_new_page;
		else if (MATCH(KW_REPORT_HEADER))
			op = nod_report_header;
		else if (MATCH(KW_COLUMN_HEADER))
			op = nod_column_header;
		else {
			op = nod_print_item;
			node = SYNTAX_NODE(nod_print_item, s_itm_count);
			node->syn_arg[s_itm_value] = parse_value(0, 0);
			if (MATCH(KW_LEFT_PAREN)) {
				if (MATCH(KW_MINUS))
					node->syn_arg[s_itm_header] = (SYN) "-";
				else
					node->syn_arg[s_itm_header] = (SYN) parse_header();
				parse_matching_paren();
			}
			node->syn_arg[s_itm_edit_string] = (SYN) parse_edit_string();
		}
		if (op != nod_print_item) {
			node = SYNTAX_NODE(op, 1);
			node->syn_count = 0;
			node->syn_arg[0] = INT_CAST 1;
			if (op == nod_column || QLI_token->tok_type == tok_number)
				node->syn_arg[0] = INT_CAST parse_ordinal();
			if ((op == nod_skip) && ((int) node->syn_arg[0] < 1))
				SYNTAX_ERROR(478);	/* Msg478 number > 0 */
		}
		LLS_PUSH(node, &stack);
		if (!MATCH(KW_COMMA) && !MATCH(KW_AND))
			break;
		PAR_real();
		if (MATCH(KW_AND))
			PAR_real();
	}

	node = make_list(stack);

	return node;
}


static SYN parse_print(void)
{
/**************************************
 *
 *	p a r s e _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Parse the PRINT statement.  This is the richest and most general
 *	Datatrieve statement.  Hence this may get a bit long.  The syntax is:
 *
 *		[<item> [, <item>]] OF <rse>
 *	PRINT [ 			     ] [ON <file>]
 *		<rse>
 *
 **************************************/
	SYN node;

	node = SYNTAX_NODE(nod_print, s_prt_count);
	PAR_token();

	if (!MATCH(KW_ALL) && MATCH(KW_DISTINCT))
		node->syn_arg[s_prt_distinct] = INT_CAST TRUE;

	if (node->syn_arg[s_prt_output] = parse_output())
		return node;

	if (test_end())
		return node;

/* If there is a potential record selection expression, there obviously
   can't be a print list.  Get the rse.  Otherwise, pick up the print
   list. */

	if (potential_rse())
		node->syn_arg[s_prt_rse] = parse_rse();
	else if (!KEYWORD(KW_USING)) {
		if (!KEYWORD(KW_THEN) && !KEYWORD(KW_OF) && !KEYWORD(KW_ON))
			node->syn_arg[s_prt_list] = parse_print_list();
		if (MATCH(KW_OF))
			node->syn_arg[s_prt_rse] = parse_rse();
	}

	if (!node->syn_arg[s_prt_list] && MATCH(KW_USING)) {
		IBERROR(484);			/* FORMs not supported */
	}
	else
		node->syn_arg[s_prt_output] = parse_output();

	return node;
}


static SYN parse_prompt(void)
{
/**************************************
 *
 *	p a r s e _ p r o m p t
 *
 **************************************
 *
 * Functional description
 *	Parse a prompt expression.
 *
 **************************************/
	SYN node;
	USHORT l;
	TEXT *q;

	PAR_token();
	node = SYNTAX_NODE(nod_prompt, 1);

/* If there is a period, get the prompt string and make a string node */

	if (MATCH(KW_DOT)) {
		PAR_real();
		l = QLI_token->tok_length;
		q = QLI_token->tok_string;
		if (QLI_token->tok_type == tok_quoted) {
			q++;
			l -= 2;
		}
		node->syn_arg[0] = (SYN) make_string(q, l);
		PAR_token();
	}

	return node;
}


static QFL parse_qualified_filter(void)
{
/**************************************
 *
 *	p a r s e _ q u a l i f i e d _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	This token ought to be a filter, possibly qualified.
 *	Return a qualified filter block, containing the 
 *	filter name in a NAM block and the database in a
 *	DBB block if a database was specified.  Somebody
 *	else will decide what to do if the database was not
 *	specified.
 *
 **************************************/
	QFL filter;

	filter = (QFL) ALLOCD(type_qfl);
	filter->qfl_database = parse_database();
	filter->qfl_name = parse_name();
	return filter;
}



static QFN parse_qualified_function(void)
{
/**************************************
 *
 *	p a r s e _ q u a l i f i e d _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	This token ought to be a function, possibly qualified.
 *	Return a qualified function block, containing the 
 *	function name in a NAM block and the database in a
 *	DBB block if a database was specified.  Somebody
 *	else will decide what to do if the database was not
 *	specified.
 *
 **************************************/
	QFN func;

	func = (QFN) ALLOCD(type_qfn);
	func->qfn_database = parse_database();
	func->qfn_name = parse_name();
	return func;
}


static QPR parse_qualified_procedure(void)
{
/**************************************
 *
 *	p a r s e _ q u a l i f i e d _ p r o c e d u r e
 *
 **************************************
 *
 * Functional description
 *	This token ought to be a procedure, possibly qualified.
 *	Return a qualified procedure block, containing the 
 *	procedure name in a NAM block and the database in a
 *	DBB block if a database was specified.  Somebody
 *	else will decide what to do if the database was not
 *	specified.
 *
 **************************************/
	QPR proc;

	proc = (QPR) ALLOCD(type_qpr);
	proc->qpr_database = parse_database();
	proc->qpr_name = parse_name();
	return proc;
}


static QLI_REL parse_qualified_relation(void)
{
/**************************************
 *
 *	p a r s e _ q u a l i f i e d _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Check for a relation name, possible qualified.  If there
 *	is a relation name, parse it and return the relation.  If
 *	not, return NULL.  Produce a syntax error only if there is
 *	a partially qualified name.
 *
 **************************************/
	SYM db_symbol;
	QLI_REL relation;

	PAR_real();

/* If the next token is a database symbol, take it as a qualifier */

	if ((db_symbol = QLI_token->tok_symbol)
		&& db_symbol->sym_type == SYM_database) {
		PAR_real_token();
		if (!MATCH(KW_DOT))
			SYNTAX_ERROR(202);	/* Msg202 period in qualified relation name */
		PAR_real();
		if (relation = resolve_relation(db_symbol, QLI_token->tok_symbol)) {
			PAR_token();
			return relation;
		}
		ERRQ_print_error(203, QLI_token->tok_string, db_symbol->sym_string,
						 NULL, NULL, NULL);	/* Msg203 %s is not a relation in database %s */
	}

	if (relation = resolve_relation(0, QLI_token->tok_symbol))
		PAR_token();

	return relation;
}


static SYN parse_ready( NOD_T node_type)
{
/**************************************
 *
 *	p a r s e _ r e a d y
 *
 **************************************
 *
 * Functional description
 *	Parse a READY statement: 
 *
 *	READY <filename> [AS <symbol>] [,...];
 *
 **************************************/
	SYN node;
	DBB database;
	NAM name;
	LLS stack;
	SSHORT l;
	TEXT *p, *q;

	stack = NULL;

	while (TRUE) {
		LEX_filename();
		if (!(l = QLI_token->tok_length))
			ERRQ_error(204, NULL, NULL, NULL, NULL, NULL);	/* Msg204 database file name required on READY */
		q = QLI_token->tok_string;
		if (QLI_token->tok_type == tok_quoted) {
			l -= 2;
			q++;
		}
		database = (DBB) ALLOCDV(type_dbb, l);
		database->dbb_filename_length = l;
		p = database->dbb_filename;
		do
			*p++ = *q++;
		while (--l);
		PAR_token();

		if (node_type == nod_def_database || node_type == nod_ready) {
			if (MATCH(KW_AS)) {
				name = parse_name();
				database->dbb_symbol = (SYM) name;
				if (HSH_lookup(name->nam_string, name->nam_length))
					ERRQ_error(408, name->nam_string, NULL, NULL, NULL, NULL);	/* Database handle is not unique */
			}
			else
				database->dbb_symbol = (SYM) make_name();
		}
		else if (node_type == nod_sql_database) {
			if (MATCH(KW_PAGESIZE)) {
				if (database->dbb_pagesize)
					SYNTAX_ERROR(390);	/* Msg390 Multiple page size specifications */
				if (!MATCH(KW_EQUALS))
					SYNTAX_ERROR(396);	/* Msg396 = (equals) */
				database->dbb_pagesize = parse_ordinal();
			}
			database->dbb_symbol = (SYM) make_name();
		}

		for (;;) {
			if (MATCH(KW_USER))
				database->dbb_user = parse_literal();
			else if (MATCH(KW_PASSWORD))
				database->dbb_password = parse_literal();
			else
				break;
		}

		LLS_PUSH(database, &stack);
		if (!KEYWORD(KW_COMMA) || (node_type == nod_sql_database))
			break;
	}

	command_end();
	node = make_list(stack);
	node->syn_type = node_type;

	return node;
}


static SYN parse_relational( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ r e l a t i o n a l
 *
 **************************************
 *
 * Functional description
 *	Parse a relational expression.
 *
 **************************************/
	SYN node, expr1, or_node;
	USHORT negation;
	NOD_T operatr, *rel_ops;
	USHORT local_flag;

	local_flag = TRUE;

	if (MATCH(KW_ANY)) {
		node = SYNTAX_NODE(nod_any, 1);
		node->syn_arg[0] = parse_rse();
		return node;
	}

	operatr = (NOD_T) 0;
	if (MATCH(KW_EXISTS))
		operatr = nod_any;
	else if (MATCH(KW_SINGULAR))
		operatr = nod_unique;

	if (operatr != (NOD_T) 0) {
		PAR_real();
		if (MATCH(KW_LEFT_PAREN)) {
			PAR_real();
			if (MATCH(KW_SELECT)) {
				PAR_real();
				node = SYNTAX_NODE(operatr, 2);
				if (!MATCH(KW_ASTERISK))
					node->syn_arg[1] = parse_value(0, 0);
				node->syn_arg[0] = parse_sql_rse();
				parse_matching_paren();
				return node;
			}
		}
		if (operatr == nod_any)
			SYNTAX_ERROR(205);	/* Msg205 EXISTS (SELECT * <sql rse>) */
		else
			SYNTAX_ERROR(488);	/* Msg488 SINGULAR (SELECT * <sql rse>) */
	}

	if (MATCH(KW_UNIQUE)) {
		node = SYNTAX_NODE(nod_unique, 1);
		node->syn_arg[0] = parse_rse();
		return node;
	}

	expr1 = parse_value(paren_count, &local_flag);
	if (KEYWORD(KW_RIGHT_PAREN))
		return expr1;
	if (KEYWORD(KW_SEMI))
		for (rel_ops = relationals; *rel_ops != (NOD_T) 0; rel_ops++)
			if (expr1->syn_type == *rel_ops)
				return expr1;

	negation = FALSE;
	node = NULL;
	MATCH(KW_IS);
	PAR_real();

	if (MATCH(KW_NOT)) {
		negation = TRUE;
		PAR_real();
	}

	switch (next_keyword()) {
	case KW_IN:
		PAR_token();
		node = parse_in(expr1, nod_eql, FALSE);
		break;

	case KW_EQUALS:
	case KW_EQ:
		operatr = (negation) ? nod_neq : nod_eql;
		negation = FALSE;
		break;

	case KW_NE:
		operatr = (negation) ? nod_eql : nod_neq;
		negation = FALSE;
		break;

	case KW_GT:
		operatr = (negation) ? nod_leq : nod_gtr;
		negation = FALSE;
		break;

	case KW_GE:
		operatr = (negation) ? nod_lss : nod_geq;
		negation = FALSE;
		break;

	case KW_LE:
		operatr = (negation) ? nod_gtr : nod_leq;
		negation = FALSE;
		break;

	case KW_LT:
		operatr = (negation) ? nod_geq : nod_lss;
		negation = FALSE;
		break;

	case KW_CONTAINING:
		operatr = nod_containing;
		break;

	case KW_MATCHES:
		node = parse_matches();
		node->syn_arg[0] = expr1;
		operatr = node->syn_type;
		break;

	case KW_LIKE:
		PAR_token();
		node = SYNTAX_NODE(nod_like, 3);
		node->syn_arg[0] = expr1;
		node->syn_arg[1] = parse_value(0, 0);
		if (MATCH(KW_ESCAPE))
			node->syn_arg[2] = parse_value(0, 0);
		else
			node->syn_count = 2;
		break;


	case KW_STARTS:
		PAR_token();
		MATCH(KW_WITH);
		node = SYNTAX_NODE(nod_starts, 2);
		node->syn_arg[0] = expr1;
		node->syn_arg[1] = parse_value(0, 0);
		break;

	case KW_NULL:
	case KW_MISSING:
		PAR_token();
		node = SYNTAX_NODE(nod_missing, 1);
		node->syn_arg[0] = expr1;
		break;

	case KW_BETWEEN:
		PAR_token();
		node = SYNTAX_NODE(nod_between, 3);
		node->syn_arg[0] = expr1;
		node->syn_arg[1] = parse_value(0, 0);
		MATCH(KW_AND);
		node->syn_arg[2] = parse_value(0, 0);
		break;

	default:
		for (rel_ops = relationals; *rel_ops != (NOD_T) 0; rel_ops++)
			if (expr1->syn_type == *rel_ops)
				return expr1;
		SYNTAX_ERROR(206);		/* Msg206 relational operatr */
	}

/* If we haven't already built a node, it must be an ordinary binary operatr.
   Build it. */

	if (!node) {
		PAR_token();
		if (MATCH(KW_ANY))
			return parse_in(expr1, operatr, FALSE);
		if (MATCH(KW_ALL))
			return parse_in(expr1, operatr, TRUE);
		node = SYNTAX_NODE(operatr, 2);
		node->syn_arg[0] = expr1;
		node->syn_arg[1] = parse_value(paren_count, &local_flag);
	}

/* If a negation remains to be handled, zap the node under a NOT. */

	if (negation)
		node = negate(node);

/*  If the node isn't an equality, we've done.  Since equalities can be
    structured as implicit ORs, build them here. */

	if (operatr != nod_eql)
		return node;

/* We have an equality operation, which can take a number of values.  Generate
   implicit ORs */

	while (MATCH(KW_COMMA)) {
		PAR_real();
		MATCH(KW_OR);
		or_node = SYNTAX_NODE(nod_or, 2);
		or_node->syn_arg[0] = node;
		or_node->syn_arg[1] = node = SYNTAX_NODE(nod_eql, 2);
		node->syn_arg[0] = expr1;
		node->syn_arg[1] = parse_value(paren_count, &local_flag);
		node = or_node;
	}

	return node;
}


static SYN parse_relation(void)
{
/**************************************
 *
 *	p a r s e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a relation expression.  Syntax is:
 *
 *	[ <context_variable> IN ] <relation>
 *
 **************************************/
	SYN node;
	SYM context;

	node = SYNTAX_NODE(nod_relation, s_rel_count);

/* Token wasn't a relation name, maybe it's a context variable. */

	if (!(node->syn_arg[s_rel_relation] = (SYN) parse_qualified_relation())) {
		context = parse_symbol();
		node->syn_arg[s_rel_context] = (SYN) context;
		if (sql_flag || !MATCH(KW_IN)) {
			if (!QLI_databases)
				IBERROR(207);	/* Msg207 a database has not been readied */
			ERRQ_print_error(208, context->sym_string, NULL, NULL, NULL, NULL);	/* Msg208 expected \"relation_name\", encountered \"%s\ */
		}
		if (!
			(node->syn_arg[s_rel_relation] =
			 (SYN) parse_qualified_relation()))SYNTAX_ERROR(209);	/* Msg209 relation name */
	}

	return node;
}


static SYN parse_rename(void)
{
/**************************************
 *
 *	p a r s e _ r e n a m e
 *
 **************************************
 *
 * Functional description
 *	Parse a PROCEDURE rename statement.
 *
 **************************************/
	SYN node;

	PAR_real_token();

	if (!MATCH(KW_PROCEDURE))
		SYNTAX_ERROR(210);		/* Msg210 PROCEDURE */

	node = SYNTAX_NODE(nod_rename_proc, 2);
	node->syn_arg[0] = (SYN) parse_qualified_procedure();
	MATCH(KW_TO);
	node->syn_arg[1] = (SYN) parse_qualified_procedure();

	return node;
}


static SYN parse_repeat(void)
{
/**************************************
 *
 *	p a r s e _ r e p e a t
 *
 **************************************
 *
 * Functional description
 *	Parse REPEAT statement.
 *
 **************************************/
	SYN node;

	PAR_token();
	node = SYNTAX_NODE(nod_repeat, s_rpt_count);
	node->syn_arg[s_rpt_value] = parse_value(0, 0);
	node->syn_arg[s_rpt_statement] = parse_statement();

	return node;
}


static SYN parse_report(void)
{
/**************************************
 *
 *	p a r s e _ r e p o r t
 *
 **************************************
 *
 * Functional description
 *	Parse a report specification.
 *
 **************************************/
	SYN node, rse, flds, qli_fld, rse_fld;
	RPT report;
	BRK control, *ptr, tmpptr, tmpptr1;
	USHORT top, i, srt_syn, ctl_syn, syn_count;
	NAM name1, name2;

	++sw_report;
	PAR_token();
	report = (RPT) ALLOCD(type_rpt);
	node = SYNTAX_NODE(nod_report, s_prt_count);
	node->syn_arg[s_prt_list] = (SYN) report;

/* Pick up record select expression */

	rse = node->syn_arg[s_prt_rse] = parse_rse();
	node->syn_arg[s_prt_output] = parse_output();

/* Pick up report clauses */

	for (;;) {
		PAR_real();
		if (MATCH(KW_END_REPORT))
			break;
		switch (next_keyword()) {
		case KW_PRINT:
			PAR_token();
			report->rpt_detail_line = (QLI_NOD) parse_print_list();
			break;

		case KW_AT:
			PAR_token();
			if (MATCH(KW_TOP))
				top = TRUE;
			else if (MATCH(KW_BOTTOM))
				top = FALSE;
			else
				SYNTAX_ERROR(382);	/* Msg382 TOP or BOTTOM */
			MATCH(KW_OF);
			if (MATCH(KW_REPORT)) {
				control = (BRK) ALLOCD(type_brk);
				ptr = (top) ? &report->rpt_top_rpt : &report->rpt_bottom_rpt;
				control->brk_next = *ptr;
				*ptr = control;
				MATCH(KW_PRINT);
				control->brk_line = parse_print_list();
			}
			else if (MATCH(KW_PAGE)) {
				control = (BRK) ALLOCD(type_brk);
				ptr =
					(top) ? &report->rpt_top_page : &report->rpt_bottom_page;
				control->brk_next = *ptr;
				*ptr = control;
				MATCH(KW_PRINT);
				control->brk_line = parse_print_list();
			}
			else {
				ptr = (top) ? &report->rpt_top_breaks :
					&report->rpt_bottom_breaks;
				if (!*ptr) {
					/* control breaks should only be on sorted fields, set up list
					   of control breaks based on sorted fields and then add action (print)
					   items to that list. */
					if (!(flds = rse->syn_arg[s_rse_sort]))
						SYNTAX_ERROR(383);	/* Msg383 sort field */
					tmpptr = *ptr;
					for (i = 0; i < flds->syn_count; i += 2) {
						control = (BRK) ALLOCD(type_brk);
						control->brk_field = flds->syn_arg[i];
						control->brk_line = NULL;
						control->brk_statisticals = NULL;
						control->brk_next = tmpptr;
						tmpptr = control;
					}
					if (!top) {
						/* reverse the 'at bottom' control break list as the
						   lower control breaks should be performed prior to the higher ones. */
						for (tmpptr1 = tmpptr->brk_next, control = 0; tmpptr;) {
							tmpptr->brk_next = control;
							control = tmpptr;
							if (tmpptr = tmpptr1)
								tmpptr1 = tmpptr->brk_next;
						}
						tmpptr = control;
					}
					*ptr = tmpptr;
				}
				qli_fld = parse_field_name(0);
				for (control = *ptr; control; control = control->brk_next) {
					rse_fld = (SYN) control->brk_field;
					if (rse_fld->syn_type != qli_fld->syn_type)
						continue;
					/* if number of field qualifiers on sort field and control field
					   are not equal test match of rightmost set */
					syn_count = MIN(rse_fld->syn_count, qli_fld->syn_count);
					srt_syn = ctl_syn = 0;
					if (syn_count != rse_fld->syn_count)
						srt_syn = rse_fld->syn_count - syn_count;
					if (syn_count != qli_fld->syn_count)
						ctl_syn = qli_fld->syn_count - syn_count;
					for (i = 0; i < syn_count; i++) {
						name1 = (NAM) rse_fld->syn_arg[i + srt_syn];
						name2 = (NAM) qli_fld->syn_arg[i + ctl_syn];
						if (strcmp(name1->nam_string, name2->nam_string))
							break;
					}
					if (i == qli_fld->syn_count)
						break;
				}
				if (!control)
					SYNTAX_ERROR(383);	/* Msg383 sort field */
				MATCH(KW_PRINT);
				control->brk_field = qli_fld;
				control->brk_line = parse_print_list();
			}
			break;

		case KW_SET:
			PAR_token();
			if (MATCH(KW_COLUMNS)) {
				MATCH(KW_EQUALS);
				report->rpt_columns = parse_ordinal();
			}
			else if (MATCH(KW_LINES)) {
				MATCH(KW_EQUALS);
				report->rpt_lines = parse_ordinal();
			}
			else if (MATCH(KW_REPORT_NAME)) {
				MATCH(KW_EQUALS);
				report->rpt_name = (TEXT *) parse_header();
			}
			else
				SYNTAX_ERROR(212);	/* Msg212 report writer SET option */
			break;

		default:
			SYNTAX_ERROR(213);	/* Msg213 report item */
		}
		MATCH(KW_SEMI);
	}

	if (!node->syn_arg[s_prt_output])
		node->syn_arg[s_prt_output] = parse_output();

	check_end();
	--sw_report;

	return node;
}


static SYN parse_rse(void)
{
/**************************************
 *
 *	p a r s e _ r s e
 *
 **************************************
 *
 * Functional description
 *	Parse a record selection expression.
 *
 **************************************/
	SYN node, first, over, *ptr;
	LLS stack, field_stack;
	USHORT count, sw_with;

	count = sw_with = 0;
	first = NULL;
	stack = NULL;
	PAR_real();

	if (MATCH(KW_ALL))
		PAR_real();

	if (MATCH(KW_FIRST))
		first = parse_value(0, 0);

	while (TRUE) {
		count++;
		LLS_PUSH(parse_relation(), &stack);
		over = NULL;
		if (MATCH(KW_OVER)) {
			field_stack = NULL;
			while (TRUE) {
				LLS_PUSH(parse_field_name(0), &field_stack);
				if (!MATCH(KW_COMMA))
					break;
			}
			over = make_list(field_stack);
		}
		LLS_PUSH(over, &stack);
		if (!MATCH(KW_CROSS))
			break;
	}

	node = SYNTAX_NODE(nod_rse, (int) s_rse_count + 2 * count);
	node->syn_count = count;
	node->syn_arg[s_rse_first] = first;
	ptr = &node->syn_arg[(int) s_rse_count + 2 * count];

	while (stack)
		*--ptr = (SYN) LLS_POP(&stack);

/* Pick up various other clauses */

	while (TRUE) {
		if (MATCH(KW_WITH)) {
			if (!sw_with) {
				sw_with++;
				node->syn_arg[s_rse_boolean] = parse_boolean(0);
			}
			else
				IBERROR(384);	/* Msg384 Too many WITHs */
		}

		else if (MATCH(KW_SORTED)) {
			PAR_real();
			MATCH(KW_BY);
			node->syn_arg[s_rse_sort] = parse_sort();
		}

#ifdef PC_ENGINE
		else if (MATCH(KW_USING)) {
			PAR_real();
			MATCH(KW_INDEX);
			node->syn_arg[s_rse_index] = (SYN) parse_name();
		}
#endif

		else if (MATCH(KW_REDUCED)) {
			PAR_real();
			MATCH(KW_TO);
			node->syn_arg[s_rse_reduced] = parse_sort();
		}

		else if (MATCH(KW_GROUP)) {
			PAR_real();
			MATCH(KW_BY);
			stack = NULL;
			while (TRUE) {
				LLS_PUSH(parse_udf_or_field(), &stack);
				if (!MATCH(KW_COMMA))
					break;
			}
			node->syn_arg[s_rse_group_by] = make_list(stack);
		}
		else
			break;
	}

	return node;
}


static SYN parse_select(void)
{
/**************************************
 *
 *	p a r s e _ s e l e c t
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL select statement.
 *
 **************************************/
	SYN node, item, rse;
	LLS stack;

	++sql_flag;
	PAR_token();
	node = SYNTAX_NODE(nod_print, s_prt_count);

	if (!MATCH(KW_ALL) && MATCH(KW_DISTINCT))
		node->syn_arg[s_prt_distinct] = INT_CAST TRUE;

/* Get list of items */

	if (!MATCH(KW_ASTERISK)) {
		stack = NULL;
		while (TRUE) {
			item = SYNTAX_NODE(nod_print_item, s_itm_count);
			item->syn_arg[s_itm_value] = parse_value(0, 0);
			LLS_PUSH(item, &stack);
			if (!MATCH(KW_COMMA))
				break;
		}
		node->syn_arg[s_prt_list] = make_list(stack);
	}

	node->syn_arg[s_prt_rse] = rse = parse_sql_rse();
	rse->syn_arg[s_rse_list] = node->syn_arg[s_prt_list];

	if (MATCH(KW_ORDER)) {
		PAR_real();
		MATCH(KW_BY);
		node->syn_arg[s_prt_order] = parse_sort();
	}

	--sql_flag;

	return node;
}


static SYN parse_set(void)
{
/**************************************
 *
 *	p a r s e _ s e t
 *
 **************************************
 *
 * Functional description
 *	Parse a SET statement.
 *
 **************************************/
	SYN node;
	LLS stack;
	ENUM set_t sw;
	USHORT count;
	U_IPTR value;
	PAR_token();
	stack = NULL;
	count = 0;

	while (TRUE) {
		PAR_real();
		value = TRUE;
		if (MATCH(KW_NO)) {
			value = FALSE;
			PAR_real();
		}
		switch (QLI_token->tok_keyword) {
		case KW_BLR:
			sw = set_blr;
			PAR_token();
			break;

		case KW_STATISTICS:
			sw = set_statistics;
			PAR_token();
			break;

		case KW_COLUMNS:
			sw = set_columns;
			PAR_token();
			MATCH(KW_EQUALS);
			value = parse_ordinal();
			break;

		case KW_LINES:
			sw = set_lines;
			PAR_token();
			MATCH(KW_EQUALS);
			value = parse_ordinal();
			break;

		case KW_SEMICOLON:
			sw = set_semi;
			PAR_token();
			break;

		case KW_ECHO:
			sw = set_echo;
			PAR_token();
			break;
#ifdef PYXIS
		case KW_FORM:
		case KW_FORMS:
			IBERROR(484);		/* FORMs not supported */
			break;
#endif
		case KW_MATCHING_LANGUAGE:
			sw = set_matching_language;
			PAR_token();
			MATCH(KW_EQUALS);
			if (value)
				value = (U_IPTR) parse_literal();
			break;

		case KW_PASSWORD:
			sw = set_password;
			PAR_token();
			value = (U_IPTR) parse_literal();
			break;

		case KW_PROMPT:
			sw = set_prompt;
			PAR_token();
			value = (U_IPTR) parse_literal();
			break;

		case KW_CONT_PROMPT:
			sw = set_continuation;
			PAR_token();
			value = (U_IPTR) parse_literal();
			break;

		case KW_USER:
			sw = set_user;
			PAR_token();
			value = (U_IPTR) parse_literal();
			break;

		case KW_COUNT:
			sw = set_count;
			PAR_token();
			break;

		case KW_CHAR:
			sw = set_charset;
			PAR_token();
			MATCH(KW_SET);
			if (value) {		/* allow for NO */
				MATCH(KW_EQUALS);
				value = (U_IPTR) parse_name();
			}
			break;

		case KW_NAMES:
			sw = set_charset;
			PAR_token();
			if (value) {
				value = (U_IPTR) parse_name();
			}
			break;

#ifdef DEV_BUILD
		case KW_EXPLAIN:
			sw = set_explain;
			PAR_token();
			break;

		case KW_HEXOUT:
			sw = set_hex_output;
			PAR_token();
			break;
#endif

		default:
			SYNTAX_ERROR(214);	/* Msg214 set option */
		}
		LLS_PUSH(sw, &stack);
		LLS_PUSH(value, &stack);
		count++;
		if (!MATCH(KW_COMMA))
			break;
	}

	command_end();
	node = make_list(stack);
	node->syn_count = count;
	node->syn_type = nod_set;

	return node;
}


static SYN parse_shell(void)
{
/**************************************
 *
 *	p a r s e _ s h e l l
 *
 **************************************
 *
 * Functional description
 *	Parse SHELL command.
 *
 **************************************/
	SYN node;

	PAR_token();
	node = SYNTAX_NODE(nod_shell, 1);

	if (!KEYWORD(KW_SEMI))
		node->syn_arg[0] = (SYN) parse_literal();

	command_end();

	return node;
}


static SYN parse_show(void)
{
/**************************************
 *
 *	p a r s e _ s h o w
 *
 **************************************
 *
 * Functional description
 *	Parse a SHOW statement.  The first
 *	group are individual things (one 
 *	named relation, field, form, ...)
 *	
 *	the second group are sets of things
 *	and can be qualified with a FOR 
 *	[DATABASE] <handle>
 *
 **************************************/
	SYN node;
	QLI_REL relation;
	LLS stack;
	SYM symbol;
	ENUM show_t sw;
	BLK value;
	USHORT count;
	QPR proc;

	PAR_token();
	stack = NULL;
	count = 0;

	while (TRUE) {
		PAR_real();
		value = NULL;
		symbol = QLI_token->tok_symbol;
		if (MATCH(KW_ALL))
			sw = show_all;
		else if (MATCH(KW_MATCHING_LANGUAGE))
			sw = show_matching_language;
		else if (MATCH(KW_VERSION))
			sw = show_version;
		else if (MATCH(KW_RELATION)) {
			if (!(value = (BLK) parse_qualified_relation()))
				SYNTAX_ERROR(216);	/* Msg216 relation name */
			else
				sw = show_relation;
		}
		else if (MATCH(KW_FILTER)) {
			sw = show_filter;
			value = (BLK) parse_qualified_filter();
		}
		else if (MATCH(KW_FUNCTION)) {
			sw = show_function;
			value = (BLK) parse_qualified_function();
		}
		else if ((MATCH(KW_DATABASES)) || (MATCH(KW_READY)))
			sw = show_databases;
		else if (MATCH(KW_DATABASE)) {
			sw = show_database;
			if (value = (BLK) get_dbb(QLI_token->tok_symbol))
				PAR_token();
		}
		else if (MATCH(KW_FIELD)) {
			sw = show_field;
			value = (BLK) parse_field_name(0);
		}
		else if (MATCH(KW_PROCEDURE)) {
			sw = show_procedure;
			value = (BLK) parse_qualified_procedure();
		}
		else if (MATCH(KW_VARIABLE)) {
			sw = show_variable;
			value = (BLK) parse_name();
		}
		else if (MATCH(KW_VARIABLES))
			sw = show_variables;
		else if (MATCH(KW_FIELDS)) {
			if (MATCH(KW_FOR)) {
				if (MATCH(KW_DATABASE)) {
					if (value = (BLK) get_dbb(QLI_token->tok_symbol))
						PAR_token();
					else
						SYNTAX_ERROR(221);	/* Msg221 database name */
					sw = show_db_fields;
				}
				else {
					MATCH(KW_RELATION);
					if (!(value = (BLK) parse_qualified_relation()))
						SYNTAX_ERROR(218);	/* Msg218 relation name */
					else
						sw = show_relation;
				}
			}
			else
				sw = show_all;
		}
		else if (MATCH(KW_INDICES)) {
			sw = show_indices;
			if (MATCH(KW_FOR))
				if (MATCH(KW_DATABASE)) {
					if (value = (BLK) get_dbb(QLI_token->tok_symbol))
						PAR_token();
					else
						SYNTAX_ERROR(221);	/* Msg221 database name */
					sw = show_db_indices;
				}
				else if (!(value = (BLK) parse_qualified_relation()))
					SYNTAX_ERROR(220);	/* Msg220 relation name */
		}
		else if (MATCH(KW_SECURITY_CLASS)) {
			sw = show_security_class;
			value = (BLK) parse_name();
		}
		else if (MATCH(KW_TRIGGERS)) {
			sw = show_triggers;
			if (MATCH(KW_FOR)) {
				if (MATCH(KW_DATABASE)) {
					if (value = (BLK) get_dbb(QLI_token->tok_symbol))
						PAR_token();
					else
						SYNTAX_ERROR(221);	/* Msg221 database name */
				}
				else {
					MATCH(KW_RELATION);
					if (!(value = (BLK) parse_qualified_relation()))
						SYNTAX_ERROR(222);	/* Msg222 relation_name */
					sw = show_trigger;
				}
			}
		}
		else if (MATCH(KW_RELATIONS))
			sw = show_relations;
		else if (MATCH(KW_VIEWS))
			sw = show_views;
		else if (MATCH(KW_SECURITY_CLASSES))
			sw = show_security_classes;
		else if (MATCH(KW_SYSTEM)) {
			if (MATCH(KW_TRIGGERS))
				sw = show_system_triggers;
			else if (MATCH(KW_RELATIONS) ||
					 QLI_token->tok_type == tok_eol ||
					 KEYWORD(KW_SEMI) || KEYWORD(KW_FOR))
				sw = show_system_relations;
			else
				SYNTAX_ERROR(215);	/* Msg215 RELATIONS or TRIGGERS */
		}
		else if (MATCH(KW_PROCEDURES))
			sw = show_procedures;
		else if (MATCH(KW_FILTERS))
			sw = show_filters;
		else if (MATCH(KW_FUNCTIONS))
			sw = show_functions;
#ifdef PYXIS
		else if (MATCH(KW_FORMS)) {
			sw = show_forms;
		}
#endif
		else if (MATCH(KW_GLOBAL)) {
			PAR_real();
			if (MATCH(KW_FIELD)) {
				sw = show_global_field;
				value = (BLK) parse_field_name(0);
			}
			else if (MATCH(KW_FIELDS))
				sw = show_global_fields;
		}
		else if (symbol && symbol->sym_type == SYM_relation) {
			sw = show_relation;
			value = symbol->sym_object;
			PAR_token();
		}
		else if (value = (BLK) get_dbb(symbol)) {
			sw = show_database;
			PAR_token();
			if (MATCH(KW_DOT))
				if (MATCH(KW_RELATIONS))
					sw = show_relations;
				else if (MATCH(KW_FIELDS))
					sw = show_db_fields;
				else if (MATCH(KW_INDICES))
					sw = show_db_indices;
				else if (MATCH(KW_SECURITY_CLASS))
					sw = show_security_class;
				else if (MATCH(KW_TRIGGERS))
					sw = show_triggers;
				else if (MATCH(KW_VIEWS))
					sw = show_views;
				else if (MATCH(KW_SECURITY_CLASSES))
					sw = show_security_classes;
				else if (MATCH(KW_SYSTEM)) {
					if (MATCH(KW_TRIGGERS))
						sw = show_system_triggers;
					else if (MATCH(KW_RELATIONS) ||
							 QLI_token->tok_type == tok_eol ||
							 KEYWORD(KW_SEMI) || KEYWORD(KW_FOR))
						sw = show_system_relations;
					else
						SYNTAX_ERROR(215);	/* Msg215 RELATIONS or TRIGGERS */
				}
				else if (MATCH(KW_PROCEDURES))
					sw = show_procedures;
				else if (MATCH(KW_FILTERS))
					sw = show_filters;
				else if (MATCH(KW_FUNCTIONS))
					sw = show_functions;
#ifdef PYXIS
				else if (MATCH(KW_FORMS))
					sw = show_forms;
#endif
				else if (MATCH(KW_GLOBAL)) {
					PAR_real();
					if (MATCH(KW_FIELD)) {
						sw = show_global_field;
						value = (BLK) parse_field_name(0);
					}
					else if (MATCH(KW_FIELDS))
						sw = show_global_fields;
				}
				else if (relation =
						 resolve_relation(symbol, QLI_token->tok_symbol)) {
					sw = show_relation;
					value = relation->rel_symbol->sym_object;
					PAR_token();
				}
				else {
					sw = show_procedure;
					proc = (QPR) ALLOCD(type_qpr);
					proc->qpr_database = (DBB) value;
					proc->qpr_name = parse_name();
					value = (BLK) proc;
				}
		}
		else {
			sw = show_procedure;
			value = (BLK) parse_qualified_procedure();
		}

		LLS_PUSH(sw, &stack);
		if (!value && (sw == show_relations || sw == show_views ||
					   sw == show_security_classes
					   || sw == show_system_triggers
					   || sw == show_system_relations || sw == show_procedures
#ifdef PYXIS
					   || sw == show_forms 
#endif
					   || sw == show_filters
					   || sw == show_functions || sw == show_global_fields))
			if (MATCH(KW_FOR)) {
				MATCH(KW_DATABASE);
				if (value = (BLK) get_dbb(QLI_token->tok_symbol))
					PAR_token();
				else
					SYNTAX_ERROR(221);	/* Msg221 database name */
			}
		LLS_PUSH(value, &stack);
		count++;
		if (!MATCH(KW_COMMA))
			break;
	}

	command_end();
	node = make_list(stack);
	node->syn_count = count;
	node->syn_type = nod_show;

	return node;
}



static SYN parse_sort(void)
{
/**************************************
 *
 *	p a r s e _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Parse a sort list.
 *
 **************************************/
	SYN node, upcase;
	LLS stack;
	USHORT direction, sensitive;

	direction = sensitive = 0;
	stack = NULL;

	while (TRUE) {
		PAR_real();
		if (!sql_flag) {
			if (MATCH(KW_ASCENDING)) {
				direction = 0;
				continue;
			}
			else if (MATCH(KW_DESCENDING)) {
				direction = 1;
				continue;
			}
			else if (MATCH(KW_EXACTCASE)) {
				sensitive = 0;
				continue;
			}
			else if (MATCH(KW_ANYCASE)) {
				sensitive = 1;
				continue;
			}
		}
		if (sql_flag && QLI_token->tok_type == tok_number) {
			node = SYNTAX_NODE(nod_position, 1);
			node->syn_arg[0] = INT_CAST parse_ordinal();
		}
		else
			node = parse_value(0, 0);
		if (sensitive) {
			upcase = SYNTAX_NODE(nod_upcase, 1);
			upcase->syn_arg[0] = node;
			LLS_PUSH(upcase, &stack);
		}
		else
			LLS_PUSH(node, &stack);
		if (sql_flag)
			if (MATCH(KW_ASCENDING))
				direction = 0;
			else if (MATCH(KW_DESCENDING))
				direction = 1;
		LLS_PUSH((ULONG) direction, &stack);
		if (!MATCH(KW_COMMA))
			break;
	}

	return make_list(stack);
}


static SYN parse_sql_alter(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ a l t e r
 *
 **************************************
 *
 * Functional description
 *	Parse the leading clauses of a SQL ALTER statement.
 *
 **************************************/
	SYN node;
	QLI_REL relation;
	QLI_FLD field;

	PAR_real_token();

	if (!MATCH(KW_TABLE))
		SYNTAX_ERROR(407);		/* Msg407 TABLE */

	node = SYNTAX_NODE(nod_sql_al_table, 2);
	relation = parse_qualified_relation();
	node->syn_arg[0] = (SYN) relation;

	for (;;) {
		if (MATCH(KW_ADD)) {
			field = parse_sql_field();
			field->fld_flags |= FLD_add;
		}
		else if (MATCH(KW_DROP)) {
			field = parse_field(FALSE);
			field->fld_flags |= FLD_drop;
		}
		else
			SYNTAX_ERROR(405);	/* Msg405 ADD or DROP */

		field->fld_next = (QLI_FLD) node->syn_arg[1];
		node->syn_arg[1] = (SYN) field;

		if (!MATCH(KW_COMMA))
			break;
	}

	command_end();
	return node;

}



static SYN parse_sql_create(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Parse the leading clauses of a SQL CREATE statement.
 *
 **************************************/
	USHORT unique, descending;

	PAR_real_token();

	if (KEYWORD(KW_DATABASE))
		return parse_ready(nod_sql_database);

	if (KEYWORD(KW_UNIQUE) || KEYWORD(KW_ASCENDING)
		|| KEYWORD(KW_DESCENDING) || KEYWORD(KW_INDEX)) {
		unique = descending = FALSE;
		while (TRUE) {
			if (MATCH(KW_UNIQUE))
				unique = TRUE;
			else if (MATCH(KW_ASCENDING))
				descending = FALSE;
			else if (MATCH(KW_DESCENDING))
				descending = TRUE;
			else if (MATCH(KW_INDEX))
				return parse_sql_index_create(unique, descending);
			else
				SYNTAX_ERROR(389);	/* Msg389 INDEX */
		}
	}

	if (MATCH(KW_TABLE))
		return parse_sql_table_create();

#ifdef NOT_USED_OR_REPLACED
/***
if (MATCH (KW_VIEW))
    return parse_sql_view_create();
***/
#endif

	SYNTAX_ERROR(386);			/* Msg386 object type for CREATE */
	return NULL;
}


static int parse_sql_dtype( USHORT * length, USHORT * scale)
{
/**************************************
 *
 *	p a r s e _ s q l _ d t y p e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL datatype clause.
 *
 **************************************/
	KWWORDS keyword;
	USHORT dtype, l;

	keyword = QLI_token->tok_keyword;
	PAR_token();
	*scale = 0;
	*length = 1;

	switch (keyword) {
	case KW_DATE:
		*length = 8;
		return dtype_timestamp;

	case KW_CHAR:
		dtype = dtype_text;
		break;

	case KW_VARCHAR:
		dtype = dtype_varying;
		break;

	case KW_SMALLINT:
		*length = sizeof(SSHORT);
		return dtype_short;

	case KW_INTEGER:
		*length = sizeof(SLONG);
		return dtype_long;

	case KW_REAL:
	case KW_FLOAT:
		*length = sizeof(float);
		dtype = dtype_real;
		break;

	case KW_LONG:
		if (!MATCH(KW_FLOAT))
			SYNTAX_ERROR(388);	/* Msg388 "FLOAT" */
	case KW_DOUBLE:
	case KW_PRECISION:
		*length = sizeof(double);
		dtype = dtype_double;
		break;

	case KW_DECIMAL:
		*length = sizeof(SLONG);
		dtype = dtype_long;
		break;
	}

	if (dtype == dtype_long || dtype == dtype_real || dtype == dtype_double) {
		if (MATCH(KW_LEFT_PAREN)) {
			l = (MATCH(KW_MINUS)) ? TRUE : FALSE;
			*scale = parse_ordinal();
			if (l)
				*scale = -(*scale);
			parse_matching_paren();
		}
	}
	else if (dtype == dtype_text || dtype == dtype_varying) {
		if (MATCH(KW_LEFT_PAREN)) {
			l = parse_ordinal();
			if (dtype == dtype_varying)
				l += sizeof(SSHORT);
			*length = l;
			parse_matching_paren();
		}
	}

	return dtype;
}


static QLI_FLD parse_sql_field(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a field description.
 *
 **************************************/
	QLI_FLD field;
	SYM name;
	USHORT dtype, length, scale;

	PAR_real();

	dtype = length = scale = 0;
	name = parse_symbol();

	PAR_real();
	switch (QLI_token->tok_keyword) {
	case KW_DOUBLE:
		MATCH(KW_PRECISION);
	case KW_NUMERIC:
	case KW_REAL:
	case KW_DATE:
	case KW_CHAR:
	case KW_VARCHAR:
	case KW_SMALLINT:
	case KW_INTEGER:
	case KW_FLOAT:
	case KW_LONG:
	case KW_DECIMAL:
		dtype = parse_sql_dtype(&length, &scale);
		break;

	default:
		SYNTAX_ERROR(179);		/* Msg179 field definition clause */
		break;
	}

	field = (QLI_FLD) ALLOCDV(type_fld, length);
	field->fld_name = name;
	field->fld_dtype = dtype;
	field->fld_scale = scale;
	field->fld_length = length;

	if (MATCH(KW_NOT))
		if (MATCH(KW_NULL)) {
			field->fld_flags |= FLD_not_null;
		}
		else {
			SYNTAX_ERROR(393);	/* Msg393 NULL */
		}

	return field;
}


static SYN parse_sql_grant_revoke( USHORT type)
{
/**************************************
 *
 *	p a r s e _ s q l _ g r a n t _ r e v o k e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL GRANT/REVOKE statement.
 *
 **************************************/
	SYN node;
	LLS stack;
	USHORT privileges;

	PAR_real_token();
	node = SYNTAX_NODE((NOD_T) type, s_grant_count);
	stack = NULL;
	privileges = 0;

	if (MATCH(KW_ALL)) {
		MATCH(KW_PRIVILEGES);
		privileges |= PRV_all;
	}
	else
		while (TRUE) {
			PAR_real();
			if (MATCH(KW_SELECT)) {
				privileges |= PRV_select;
				continue;
			}
			else if (MATCH(KW_INSERT)) {
				privileges |= PRV_insert;
				continue;
			}
			else if (MATCH(KW_DELETE)) {
				privileges |= PRV_delete;
				continue;
			}
			else if (MATCH(KW_UPDATE)) {
				privileges |= PRV_update;

				if (MATCH(KW_COMMA))
					continue;

				if (KEYWORD(KW_ON))
					break;

				if (!MATCH(KW_LEFT_PAREN))
					SYNTAX_ERROR(187);	/* Msg187 left parenthesis */

				do {
					if (KEYWORD(KW_SELECT) || KEYWORD(KW_INSERT)
						|| KEYWORD(KW_DELETE) || KEYWORD(KW_UPDATE))
						break;
					PAR_real();
					LLS_PUSH(parse_name(), &stack);

				} while (MATCH(KW_COMMA));

				if (!MATCH(KW_RIGHT_PAREN))
					SYNTAX_ERROR(191);	/* Msg191 right parenthesis */

				continue;
			}

			if (!MATCH(KW_COMMA))
				break;
		}

	node->syn_arg[s_grant_fields] = make_list(stack);

	PAR_real();
	if (!MATCH(KW_ON))
		SYNTAX_ERROR(397);		/* Msg397 ON */

	PAR_real();
	if (!(node->syn_arg[s_grant_relation] = (SYN) parse_qualified_relation()))
		SYNTAX_ERROR(170);		/* Msg170 relation name */

	if (type == (USHORT) nod_sql_grant) {
		if (!MATCH(KW_TO))
			SYNTAX_ERROR(404);	/* Msg404 TO */
	}
	else {
		if (!MATCH(KW_FROM))
			SYNTAX_ERROR(403);	/* Msg403 FROM */
	}

	stack = NULL;

	while (TRUE) {
		PAR_real();
		LLS_PUSH(parse_name(), &stack);
		if (!MATCH(KW_COMMA))
			break;
	}

	node->syn_arg[s_grant_users] = make_list(stack);

	if (type == (USHORT) nod_sql_grant)
		if (MATCH(KW_WITH)) {
			PAR_real();
			if (!MATCH(KW_GRANT))
				SYNTAX_ERROR(401);	/* Msg401 GRANT */
			MATCH(KW_OPTION);
			privileges |= PRV_grant_option;
		}

	node->syn_arg[s_grant_privileges] = INT_CAST privileges;

	return node;
}


static SYN parse_sql_index_create( USHORT unique, USHORT descending)
{
/**************************************
 *
 *	p a r s e _ s q l _ i n d e x _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Parse the SQL CREATE INDEX statement.
 *
 **************************************/
	LLS stack;
	SYN node;

	PAR_real();
	node = SYNTAX_NODE(nod_def_index, s_dfi_count);

	if (unique)
		node->syn_flags |= s_dfi_flag_unique;
	if (descending)
		node->syn_flags |= s_dfi_flag_descending;

	node->syn_arg[s_dfi_name] = (SYN) parse_symbol();

	PAR_real();
	if (!MATCH(KW_ON))
		SYNTAX_ERROR(397);		/* Msg397 ON */

	if (!(node->syn_arg[s_dfi_relation] = (SYN) parse_qualified_relation()))
		SYNTAX_ERROR(170);		/* Msg170 relation name */

	PAR_real();

	if (!MATCH(KW_LEFT_PAREN))
		SYNTAX_ERROR(185);		/* Msg185 left parenthesis */

	stack = NULL;

	for (;;) {
		LLS_PUSH(parse_name(), &stack);
		if (MATCH(KW_RIGHT_PAREN))
			break;
		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR(171);	/* Msg171 comma between field definitions */
	}

	node->syn_arg[s_dfi_fields] = make_list(stack);

	command_end();

	return node;
}


static SYN parse_sql_joined_relation( SYN prior_context)
{
/**************************************
 *
 *	p a r s e _ s q l _ j o i n e d _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a join relation clause.
 *
 **************************************/
	SYN left;

	if (MATCH(KW_LEFT_PAREN)) {
		left = parse_sql_joined_relation(0);
		parse_matching_paren();
	}
	else if (!(left = parse_sql_relation()))
		return NULL;

	return parse_sql_join_clause(left);
}


static SYN parse_sql_join_clause( SYN left)
{
/**************************************
 *
 *	p a r s e _ s q l _ j o i n _ c l a u s e
 *
 **************************************
 *
 * Functional description
 *	Parse a join relation clause.
 *
 **************************************/
	SYN right, node;
	NOD_T join_type;

	join_type = parse_join_type();
	if (join_type == (NOD_T) 0)
		return left;

	if (!(right = parse_sql_joined_relation(left)))
		SYNTAX_ERROR(490);		/* Msg490 joined relation clause */

	if (!MATCH(KW_ON))
		SYNTAX_ERROR(492);		/* Msg492 ON */

	node = SYNTAX_NODE(nod_rse, (int) s_rse_count + 2 * 2);
	node->syn_count = 2;
	node->syn_arg[s_rse_count] = left;
	node->syn_arg[s_rse_count + 2] = right;
	node->syn_arg[s_rse_join_type] = (SYN) join_type;
	node->syn_arg[s_rse_boolean] = parse_boolean(0);

	return parse_sql_join_clause(node);
}


static SYN parse_sql_table_create(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ t a b l e _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Parse the SQL CREATE TABLE statement.
 *
 **************************************/
	QLI_REL relation;
	QLI_FLD field, *ptr;
	SYN node;

	PAR_real();
	node = SYNTAX_NODE(nod_sql_cr_table, 1);
	relation = (QLI_REL) ALLOCD(type_rel);
	node->syn_arg[0] = (SYN) relation;
	relation->rel_database = parse_database();
	relation->rel_symbol = parse_symbol();

	ptr = &relation->rel_fields;

	if (!MATCH(KW_LEFT_PAREN))
		SYNTAX_ERROR(185);		/* Msg185 left parenthesis */

	PAR_real();

	for (;;) {
		*ptr = field = parse_sql_field();
		ptr = &field->fld_next;
		if (MATCH(KW_RIGHT_PAREN))
			break;
		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR(171);	/* Msg171 comma between field definitions */
	}

	command_end();

	return node;
}

#ifdef NOT_USED_OR_REPLACED
static SYN parse_sql_view_create(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ v i e w _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Parse the SQL CREATE VIEW statement.
 *
 **************************************/
	SYN node;
	LLS stack;
	QLI_REL relation;

	PAR_real();

	sw_sql_view = TRUE;
	node = SYNTAX_NODE(nod_sql_cr_view, s_crv_count);
	stack = NULL;

	relation = (QLI_REL) ALLOCD(type_rel);
	node->syn_arg[s_crv_name] = (SYN) relation;
	relation->rel_database = parse_database();
	relation->rel_symbol = parse_symbol();

/* if field list is present parse it and create corresponding field blocks */

	if (MATCH(KW_LEFT_PAREN)) {
		for (;;) {
			LLS_PUSH(parse_name(), &stack);
			if (MATCH(KW_RIGHT_PAREN))
				break;
			if (!MATCH(KW_COMMA))
				SYNTAX_ERROR(171);	/* Msg171 comma between field definitions */
		}
	}

/* node->syn_arg [s_crv_fields] = make_list (stack); */

	if (!MATCH(KW_AS))
		SYNTAX_ERROR(394);		/* Msg394 As */

	if (!KEYWORD(KW_SELECT))
		SYNTAX_ERROR(395);		/* Msg395 Select */

	node->syn_arg[s_crv_rse] = parse_select();

	sw_sql_view = FALSE;

	return node;
}
#endif

static SYN parse_sql_relation(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL relation clause.
 *
 **************************************/
	SYN node;

	node = SYNTAX_NODE(nod_relation, s_rel_count);

	if (!(node->syn_arg[s_rel_relation] = (SYN) parse_qualified_relation()))
		SYNTAX_ERROR(223);		/* Msg223 relation name */

	if (!QLI_token->tok_symbol)
		node->syn_arg[s_rel_context] = (SYN) parse_symbol();

	return node;
}


static SYN parse_sql_rse(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ r s e
 *
 **************************************
 *
 * Functional description
 *	Parse the trailing clauses of a SQL SELECT statement.
 *
 **************************************/
	SYN node, *ptr;
	LLS stack;
	USHORT count;

	stack = NULL;
	count = 0;
	PAR_real();

	if (!MATCH(KW_FROM))
		SYNTAX_ERROR(224);		/* Msg224 FROM clause */

/* Parse FROM list of relations */

	while (TRUE) {
		count++;
		LLS_PUSH(parse_sql_joined_relation(0), &stack);
		if (!MATCH(KW_COMMA))
			break;
	}

/* Build a syntax node.  Since SQL doesn't support OVER, only every
   other slot will be used in the RSE. */

	node = SYNTAX_NODE(nod_rse, (int) s_rse_count + 2 * count);
	node->syn_count = count;
	ptr = &node->syn_arg[(int) s_rse_count + 2 * count];

	while (stack) {
		--ptr;
		*--ptr = (SYN) LLS_POP(&stack);
	}

	if (MATCH(KW_WITH))
		node->syn_arg[s_rse_boolean] = parse_boolean(0);

	if (MATCH(KW_GROUP)) {
		if (sw_sql_view)
			SYNTAX_ERROR(391);	/* Msg391 No group by in view def */
		PAR_real();
		MATCH(KW_BY);
		stack = NULL;
		while (TRUE) {
			LLS_PUSH(parse_udf_or_field(), &stack);
			if (!MATCH(KW_COMMA))
				break;
		}
		node->syn_arg[s_rse_group_by] = make_list(stack);
		if (MATCH(KW_HAVING))
			node->syn_arg[s_rse_having] = parse_boolean(0);
	}

	return node;
}


static SYN parse_sql_singleton_select(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ s i n g l e t o n _ s e l e c t
 *
 **************************************
 *
 * Functional description
 *	Finish parsing an SQL singleton select and
 *	turn it into a FIRST ... FROM --- not exactly
 *	kosher, but a start.
 *
 **************************************/
	SYN node, value;

	value = parse_primitive_value(0, 0);
	PAR_real();

	node = SYNTAX_NODE(nod_from, s_stt_count);
	node->syn_arg[s_stt_value] = value;

	node->syn_arg[s_stt_rse] = parse_sql_rse();
	--sql_flag;

	return node;
}


static SYN parse_sql_subquery(void)
{
/**************************************
 *
 *	p a r s e _ s q l _ s u b q u e r y
 *
 **************************************
 *
 * Functional description
 *	Parse an sql subquery that should
 *	return a single value.
 *
 **************************************/
	struct nod_types *types;
	SYN node;
	KWWORDS keyword;

	if (sw_sql_view)
		SYNTAX_ERROR(392);		/* Msg392 No aggregates in view def */

	PAR_token();

	keyword = next_keyword();
	++sql_flag;

	for (types = statisticals;; types++)
		if (types->nod_t_keyword == KW_none)
			return parse_sql_singleton_select();
		else if (types->nod_t_keyword == keyword)
			break;

	PAR_token();
	node = SYNTAX_NODE(types->nod_t_node, s_stt_count);

	MATCH(KW_LEFT_PAREN);

	if (node->syn_type != nod_count || !MATCH(KW_ASTERISK)) {
		if (MATCH(KW_DISTINCT))
			node->syn_arg[s_prt_distinct] = INT_CAST TRUE;
		node->syn_arg[s_stt_value] = parse_value(0, 0);
	}

	parse_matching_paren();

	node->syn_arg[s_stt_rse] = parse_sql_rse();
	--sql_flag;

	return node;
}


static SYN parse_statement(void)
{
/**************************************
 *
 *	p a r s e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Parse a statement.  (Set statement switch
 *	to true here as well as in PARQ_parse to
 *	avoid confusion with linked statements 
 *	e.g. THEN conjuncts )
 *
 **************************************/
	SYN node;
	LLS stack;

	PAR_real();
	sw_statement = TRUE;
	function_count = 0;

	switch (next_keyword()) {
	case KW_ABORT:
		node = parse_abort();
		break;

	case KW_ACCEPT:
		node = parse_accept();
		break;

	case KW_COMMIT:
		node = parse_transaction(nod_commit_retaining);
		break;

	case KW_DECLARE:
		node = parse_declare();
		break;

	case KW_DELETE:
		MATCH(KW_DELETE);
		node = parse_delete();
		break;

	case KW_ERASE:
		node = parse_erase();
		break;

	case KW_FOR:
		node = parse_for();
		break;

	case KW_IF:
		node = parse_if();
		break;

	case KW_INSERT:
		node = parse_insert();
		break;

	case KW_LIST:
		node = parse_list_fields();
		break;

	case KW_MENU:
		node = parse_menu();
		break;

	case KW_MODIFY:
		node = parse_modify();
		break;

	case KW_PRINT:
		node = parse_print();
		break;

	case KW_REPEAT:
		node = parse_repeat();
		break;

	case KW_REPORT:
		node = parse_report();
		break;

	case KW_SELECT:
		node = parse_select();
		break;

	case KW_STORE:
		node = parse_store();
		break;

	case KW_UPDATE:
		node = parse_update();
		break;

	case KW_BEGIN:
		stack = NULL;
		PAR_token();
		while (TRUE) {
			PAR_real();
			if (MATCH(KW_END))
				break;
			LLS_PUSH(parse_statement(), &stack);
			MATCH(KW_SEMI);
		}
		node = make_list(stack);
		break;

	default:
		node = parse_assignment();
	}

	check_end();

/* Check for the "THEN" connective.  If found, make a list of statements. */

	if (QLI_token->tok_type != tok_eol || (QLI_semi && !KEYWORD(KW_SEMI)))
		MATCH(KW_SEMI);

	if (!MATCH(KW_THEN))
		return node;

	stack = NULL;
	LLS_PUSH(node, &stack);
	LLS_PUSH(parse_statement(), &stack);

	return make_list(stack);
}


static SYN parse_statistical(void)
{
/**************************************
 *
 *	p a r s e _ s t a t i s t i c a l
 *
 **************************************
 *
 * Functional description
 *	Parse statistical expression.
 *
 **************************************/
	struct nod_types *types;
	SYN node;
	KWWORDS keyword;

	keyword = next_keyword();
	PAR_token();

	for (types = statisticals;; types++)
		if (types->nod_t_keyword == keyword)
			break;

/* Handle SQL statisticals a little differently */

	if (sql_flag) {
		node = SYNTAX_NODE(types->nod_t_sql_node, s_stt_count);
		if (!MATCH(KW_LEFT_PAREN))
			SYNTAX_ERROR(227);	/* Msg227 left parenthesis */
		if (node->syn_type != nod_agg_count || !MATCH(KW_ASTERISK)) {
			if (MATCH(KW_DISTINCT))
				node->syn_arg[s_prt_distinct] = INT_CAST TRUE;
			node->syn_arg[s_stt_value] = parse_value(0, 0);
		}
		parse_matching_paren();
		return node;
	}

/* Handle GDML statisticals */

	node = SYNTAX_NODE(types->nod_t_node, s_stt_count);

	if (node->syn_type != nod_count)
		node->syn_arg[s_stt_value] = parse_value(0, 0);

	if (!MATCH(KW_OF)) {
		if (sw_report) {
			if (function_count > 0)
				IBERROR(487);	/* Msg487 Invalid argument for UDF */
			node->syn_type = types->nod_t_rpt_node;
			return node;
		}
		PAR_real();
		if (!MATCH(KW_OF))
			SYNTAX_ERROR(228);	/* Msg 228 OF */
	}

	node->syn_arg[s_stt_rse] = parse_rse();

	return node;
}


static SYN parse_store(void)
{
/**************************************
 *
 *	p a r s e _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Parse a STORE statement.
 *
 **************************************/
	SYN node;

	PAR_token();
	node = SYNTAX_NODE(nod_store, s_sto_count);
	node->syn_arg[s_sto_relation] = parse_relation();

	if (test_end())
		return node;

	MATCH(KW_USING);

#ifdef PYXIS
	if (MATCH(KW_FORM))
		IBERROR(484);			/* FORMs not supported */
	else
#endif
		node->syn_arg[s_sto_statement] = parse_statement();

	return node;
}


static TEXT *parse_string(void)
{
/**************************************
 *
 *	p a r s e _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Save the current token as a string, advance to the next
 *	token, and return a pointer to the string.
 *
 **************************************/
	TEXT *string;

	string = make_string(QLI_token->tok_string, QLI_token->tok_length);
	PAR_token();

	return string;
}


static SYM parse_symbol(void)
{
/**************************************
 *
 *	p a r s e _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Parse the next token as a context symbol.
 *
 **************************************/
	SYM context;
	USHORT l;
	TEXT c, *p, *q;

	l = QLI_token->tok_length;
	context = (SYM) ALLOCDV(type_sym, l);
	context->sym_type = SYM_context;
	context->sym_length = l;
	q = QLI_token->tok_string;
	p = context->sym_string = context->sym_name;

	if (l)
		do {
			c = *q++;
			*p++ = UPPER(c);
		} while (--l);

	PAR_token();

	return context;
}


static void parse_terminating_parens( USHORT * paren_count, USHORT * local_count)
{
/**************************************
 *
 *	p a r s e _ t e r m i n a t i n g _ p a r e n s
 *
 **************************************
 *
 * Functional description
 *	Check for balancing parenthesis.  If missing, barf.
 *
 **************************************/

	if (*paren_count && paren_count == local_count)
		do
			parse_matching_paren();
		while (--(*paren_count));
}


static SYN parse_transaction( NOD_T node_type)
{
/**************************************
 *
 *	p a r s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse the FINISH, COMMIT, ROLLBACK,
 *	and PREPARE commands and the COMMIT statement.
 *
 **************************************/
	SYN node;
	LLS stack;
	SYM symbol;

	stack = NULL;
	PAR_token();

	if (!KEYWORD(KW_SEMI))
		while (TRUE) {
			for (symbol = QLI_token->tok_symbol; symbol;
				 symbol = symbol->sym_homonym)
					if (symbol->sym_type == SYM_database) break;
			if (!symbol)
				SYNTAX_ERROR(229);	/* Msg229 database name */
			LLS_PUSH(symbol->sym_object, &stack);
			PAR_token();
			if (!MATCH(KW_COMMA))
				break;
		}

	command_end();
	node = make_list(stack);
	node->syn_type = node_type;

	return node;
}


static SYN parse_udf_or_field(void)
{
/**************************************
 *
 *	p a r s e _ u d f _ o r _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a function or field reference.
 *
 **************************************/
	SYM symbol;

	if ((symbol = QLI_token->tok_symbol) && symbol->sym_type == SYM_function)
		return parse_function();

	return parse_field_name(0);
}


static SYN parse_update(void)
{
/**************************************
 *
 *	p a r s e _ u p d a t e
 *
 **************************************
 *
 * Functional description
 *	Parse a SQL UPDATE statement.
 *
 **************************************/
	SYN node, rse;
	LLS stack;

	++sql_flag;
	PAR_token();
	node = SYNTAX_NODE(nod_modify, s_mod_count);
	node->syn_arg[s_mod_rse] = rse =
		SYNTAX_NODE(nod_rse, (int) s_rse_count + 2);
	rse->syn_count = 1;
	rse->syn_arg[s_rse_count] = parse_sql_relation();

	if (!MATCH(KW_SET))
		SYNTAX_ERROR(230);		/* Msg230 SET */

/* Pick up assignments */

	stack = NULL;

	while (TRUE) {
		LLS_PUSH(parse_assignment(), &stack);
		if (!MATCH(KW_COMMA))
			break;
	}

/* Pick up boolean, if present */

	if (MATCH(KW_WITH))
		rse->syn_arg[s_rse_boolean] = parse_boolean(0);

	node->syn_arg[s_mod_statement] = make_list(stack);
	--sql_flag;

	return node;
}


static SYN parse_value( USHORT * paren_count, USHORT * bool_flag)
{
/**************************************
 *
 *	p a r s e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Parse a general value expression.  In practice, this means parse the
 *	lowest precedence operatr CONCATENATE.
 *
 **************************************/
	SYN node, arg;
	USHORT local_count, local_flag;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}
	if (!bool_flag) {
		local_flag = FALSE;
		bool_flag = &local_flag;
	}

	node = parse_add(paren_count, bool_flag);

	while (TRUE) {
		if (!MATCH(KW_BAR)) {
			parse_terminating_parens(paren_count, &local_count);
			return node;
		}
		arg = node;
		node = SYNTAX_NODE(nod_concatenate, 2);
		node->syn_arg[0] = arg;
		node->syn_arg[1] = parse_add(paren_count, bool_flag);
	}
}


static int potential_rse(void)
{
/**************************************
 *
 *	p o t e n t i a l _ r s e
 *
 **************************************
 *
 * Functional description
 *	Test to see if the current token is likely (sic!) to be part of
 *	a record selection expression.
 *
 **************************************/
	SYM symbol;

	for (symbol = QLI_token->tok_symbol; symbol; symbol = symbol->sym_homonym)
		if ((symbol->sym_type == SYM_keyword &&
			 symbol->sym_keyword == (int) KW_FIRST) ||
			symbol->sym_type == SYM_relation ||
			symbol->sym_type == SYM_database) return TRUE;

	return FALSE;
}


static QLI_REL resolve_relation( SYM db_symbol, SYM relation_symbol)
{
/**************************************
 *
 *	r e s o l v e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Given symbols for a database and a relation (either may be null),
 *	resolve the relation.  If the relation can't be resolved, return
 *	NULL (don't error!).
 *
 **************************************/
	QLI_REL relation;
	SYM temp;
	DBB dbb;

/* If we don't recognize the relation, punt. */

	if (!relation_symbol)
		return NULL;

/* If a database symbol is present, resolve the relation against the
   the given database. */

	if (db_symbol) {			/* && db_symbol->sym_type == SYM_database ?  */
		for (; db_symbol; db_symbol = db_symbol->sym_homonym)
			for (temp = relation_symbol; temp; temp = temp->sym_homonym)
				if (temp->sym_type == SYM_relation) {
					relation = (QLI_REL) temp->sym_object;
					if (relation->rel_database == (DBB) db_symbol->sym_object)
						return relation;
				}
		return NULL;
	}

/* No database qualifier, so search all databases. */

	for (dbb = QLI_databases; dbb; dbb = dbb->dbb_next)
		for (temp = relation_symbol; temp; temp = temp->sym_homonym)
			if (temp->sym_type == SYM_relation) {
				relation = (QLI_REL) temp->sym_object;
				if (relation->rel_database == dbb)
					return relation;
			}

	return NULL;
}


static SYN syntax_node( NOD_T type, USHORT count)
{
/**************************************
 *
 *	s y n t a x _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Allocate and initialize a syntax node of given type.
 *
 **************************************/
	SYN node;

	node = (SYN) ALLOCDV(type_syn, count);
	node->syn_type = type;
	node->syn_count = count;

	return node;
}


static int test_end(void)
{
/**************************************
 *
 *	t e s t _ e n d
 *
 **************************************
 *
 * Functional description
 *	Test for end of a statement.  In specific, test for one of
 *	THEN, ELSE, ON, or a semi-colon.
 *
 **************************************/

	if (KEYWORD(KW_THEN) ||
		KEYWORD(KW_ON) || KEYWORD(KW_ELSE) || KEYWORD(KW_SEMI)) return TRUE;

	return FALSE;
}
