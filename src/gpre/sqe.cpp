//____________________________________________________________
//  
//		PROGRAM:	C Preprocessor
//		MODULE:		sqe.cpp
//		DESCRIPTION:	SQL expression parser
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  $Id: sqe.cpp,v 1.9 2003-03-03 08:36:03 brodsom Exp $
//  Revision 1.3  2000/11/16 15:54:29  fsg
//  Added new switch -verbose to gpre that will dump
//  parsed lines to stderr
//  
//  Fixed gpre bug in handling row names in WHERE clauses
//  that are reserved words now (DATE etc)
//  (this caused gpre to dump core when parsing tan.e)
//  
//  Fixed gpre bug in handling lower case table aliases
//  in WHERE clauses for sql dialect 2 and 3.
//  (cause a core dump in a test case from C.R. Zamana)
//  
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  
//
//____________________________________________________________
//
//	$Id: sqe.cpp,v 1.9 2003-03-03 08:36:03 brodsom Exp $
//
#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../gpre/gpre.h"
#include "../gpre/parse.h"
#include "../gpre/cme_proto.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/exp_proto.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../gpre/sqe_proto.h"
#include "../gpre/sql_proto.h"


extern "C" {


#define ERROR_LENGTH	256

struct scope {
	struct gpre_ctx *req_contexts;
	USHORT req_scope_level;		/* scope level for SQL subquery parsing */
	USHORT req_in_aggregate;	/* now processing value expr for aggr */
	USHORT req_in_select_list;	/* processing select list */
	USHORT req_in_where_clause;	/* processing where clause */
	USHORT req_in_having_clause;	/* processing having clause */
	USHORT req_in_order_by_clause;	/* processing order by clause */
	USHORT req_in_subselect;	/* processing a subselect clause */
};

extern ACT cur_routine;
extern struct tok prior_token;

static BOOLEAN compare_expr(GPRE_NOD, GPRE_NOD);
static GPRE_NOD copy_fields(GPRE_NOD, MAP);
static GPRE_NOD explode_asterisk(GPRE_NOD, int, RSE);
static GPRE_NOD explode_asterisk_all(GPRE_NOD, int, RSE, BOOLEAN);
static GPRE_FLD get_ref(GPRE_NOD);
static GPRE_NOD implicit_any(GPRE_REQ, GPRE_NOD, enum nod_t, enum nod_t);
static GPRE_NOD merge(GPRE_NOD, GPRE_NOD);
static GPRE_NOD merge_fields(GPRE_NOD, GPRE_NOD, int, BOOLEAN);
static GPRE_NOD negate(GPRE_NOD);
static void pair(GPRE_NOD, GPRE_NOD);
static GPRE_CTX par_alias_list(GPRE_REQ, GPRE_NOD);
static GPRE_CTX par_alias(GPRE_REQ, TEXT *);
static GPRE_NOD par_and(GPRE_REQ, USHORT *);
static GPRE_REL par_base_table(GPRE_REQ, GPRE_REL, TEXT *);
static GPRE_NOD par_collate(GPRE_REQ, GPRE_NOD);
static GPRE_NOD par_in(GPRE_REQ, GPRE_NOD);
static GPRE_CTX par_joined_relation(GPRE_REQ, GPRE_CTX);
static GPRE_CTX par_join_clause(GPRE_REQ, GPRE_CTX);
static NOD_T par_join_type(void);
static GPRE_NOD par_multiply(GPRE_REQ, BOOLEAN, USHORT *, USHORT *);
static GPRE_NOD par_not(GPRE_REQ, USHORT *);
static void par_order(GPRE_REQ, RSE, SSHORT, USHORT);
static GPRE_NOD par_plan(GPRE_REQ);
static GPRE_NOD par_plan_item(GPRE_REQ, BOOLEAN);
static GPRE_NOD par_primitive_value(GPRE_REQ, BOOLEAN, USHORT *, USHORT *);
static GPRE_NOD par_relational(GPRE_REQ, USHORT *);
static RSE par_rse(GPRE_REQ, GPRE_NOD, BOOLEAN);
static RSE par_select(GPRE_REQ, RSE);
static GPRE_NOD par_stat(GPRE_REQ);
static GPRE_NOD par_subscript(GPRE_REQ);
static void par_terminating_parens(USHORT *, USHORT *);
static GPRE_NOD par_udf(GPRE_REQ);
static GPRE_NOD par_udf_or_field(GPRE_REQ, BOOLEAN);
static GPRE_NOD par_udf_or_field_with_collate(GPRE_REQ, BOOLEAN);
static GPRE_NOD post_fields(GPRE_NOD, MAP);
static GPRE_NOD post_map(GPRE_NOD, MAP);
static GPRE_NOD post_select_list(GPRE_NOD, MAP);
static void pop_scope(GPRE_REQ, struct scope *);
static void push_scope(GPRE_REQ, struct scope *);
static GPRE_FLD resolve(GPRE_NOD, GPRE_CTX, GPRE_CTX *, ACT *);
static GPRE_CTX resolve_asterisk(TOK, RSE);
static void set_ref(GPRE_NOD, GPRE_FLD);
static char *upcase_string(char *);
static BOOLEAN validate_references(GPRE_NOD, GPRE_NOD);
static void dialect1_bad_type(USHORT);




typedef struct ops {
	enum nod_t rel_op;
	enum kwwords rel_kw;
	enum nod_t rel_negation;
} OPS;

static OPS rel_ops[] = {
	{ nod_eq, KW_EQ, nod_ne },
	{ nod_eq, KW_EQUALS, nod_ne },
	{ nod_ne, KW_NE, nod_eq },
	{ nod_gt, KW_GT, nod_le },
	{ nod_ge, KW_GE, nod_lt },
	{ nod_le, KW_LE, nod_gt },
	{ nod_lt, KW_LT, nod_ge },
	{ nod_containing, KW_CONTAINING, nod_any },
	{ nod_starting, KW_STARTING, nod_any },
	{ nod_matches, KW_MATCHES, nod_any },
	{ nod_any, KW_none, nod_any },
	{ nod_ansi_any, KW_none, nod_ansi_any },
	{ nod_ansi_all, KW_none, nod_ansi_all }};
#ifdef NOT_USED_OR_REPLACED
static OPS scalar_stat_ops[] = {
	{ nod_count, KW_COUNT, nod_any },
	{ nod_max, KW_MAX, nod_any },
	{ nod_min, KW_MIN, nod_any },
	{ nod_total, KW_TOTAL, nod_any },
	{ nod_total, KW_SUM, nod_any },
	{ nod_average, KW_AVERAGE, nod_any },
	{ nod_via, KW_none, nod_any}};
#endif
static OPS stat_ops[] = {
	{ nod_agg_count, KW_COUNT, nod_any },
	{ nod_agg_max, KW_MAX, nod_any },
	{ nod_agg_min, KW_MIN, nod_any },
	{ nod_agg_total, KW_TOTAL, nod_any },
	{ nod_agg_total, KW_SUM, nod_any },
	{ nod_agg_average, KW_AVERAGE, nod_any },
	{ nod_any, KW_none, nod_any },
	{ nod_ansi_any, KW_none, nod_ansi_any },
	{ nod_ansi_all, KW_none, nod_ansi_all }};

static NOD_T relationals[] = {
	nod_eq, nod_ne, nod_gt, nod_ge, nod_le, nod_lt, nod_containing,
	nod_starting, nod_matches, nod_any, nod_missing, nod_between, nod_like,
	nod_and, nod_or, nod_not, nod_ansi_any, nod_ansi_all, (NOD_T) 0
};


//____________________________________________________________
//  
//		Parse an OR boolean expression.
//  

GPRE_NOD SQE_boolean( GPRE_REQ request, USHORT * paren_count)
{
	GPRE_NOD expr1;
	USHORT local_count;

	assert_IS_REQ(request);

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	expr1 = par_and(request, paren_count);

	if (!MATCH(KW_OR) && !MATCH(KW_OR1)) {
		par_terminating_parens(paren_count, &local_count);
		return expr1;
	}

	expr1 = MSC_binary(nod_or, expr1, SQE_boolean(request, paren_count));
	par_terminating_parens(paren_count, &local_count);

	return expr1;
}


//____________________________________________________________
//  
//		Parse a reference to a relation name
//		and generate a context for it.
//  

GPRE_CTX SQE_context(GPRE_REQ request)
{
	SYM symbol;
	GPRE_CTX context, conflict;
	GPRE_PRC procedure;
	USHORT local_count;
	SCHAR r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1];
	SCHAR s[ERROR_LENGTH];
	GPRE_FLD field;
	GPRE_NOD *input;

	assert_IS_REQ(request);

	context = MAKE_CONTEXT(request);
	SQL_relation_name(r_name, db_name, owner_name);

	if (!(context->ctx_relation =
		  SQL_relation(request, r_name, db_name, owner_name, FALSE))) {
		/* check for a procedure */
		if (procedure = context->ctx_procedure =
			SQL_procedure(request, r_name, db_name, owner_name, FALSE)) {
			if (procedure->prc_inputs) {
				if (!MATCH(KW_LEFT_PAREN))
					SYNTAX_ERROR("( <procedure input parameters> )");
				/* parse input references */
				context->ctx_prc_inputs = SQE_list(SQE_value, request, FALSE);
				local_count = 1;
				par_terminating_parens(&local_count, &local_count);
				if (procedure->prc_in_count !=
					context->ctx_prc_inputs->
					nod_count)
PAR_error("count of input values doesn't match count of parameters");
				for (input = context->ctx_prc_inputs->nod_arg, field =
					 procedure->prc_inputs; field;
					 input++, field =
					 field->fld_next) SQE_post_field(*input, field);
			}
		}
		else {
			if (owner_name[0])
				sprintf(s, "table %s.%s not defined", owner_name, r_name);
			else
				sprintf(s, "table %s not defined", r_name);
			PAR_error(s);
		}

	}

//  If the next token is recognized as a keyword, it can't be a SQL "alias".
//  It may, however, be an "end of line" token.  If so, trade it in on the
//  next "real" token. 

	if ((symbol = token.tok_symbol) && symbol->sym_type == SYM_keyword) {
		if (!token.tok_length)
			CPR_token();
		return context;
	}

//  we have what we assume to be an alias; check to make sure that 
//  it does not conflict with any relation, procedure or context names
//  at the same scoping level in this query 

	for (conflict = request->req_contexts; conflict;
		 conflict = conflict->ctx_next) if ((symbol = conflict->ctx_symbol)
											&& (symbol->sym_type ==
												SYM_relation
												|| symbol->sym_type ==
												SYM_context
												|| symbol->sym_type ==
												SYM_procedure)
											&&
											(!strcmp
											 (symbol->sym_string,
											  token.tok_string))
											&& (conflict->ctx_scope_level ==
												request->
												req_scope_level)) break;

	if (conflict) {
		SCHAR *error_type;
		if (symbol->sym_type == SYM_relation)
			error_type = "table";
		else if (symbol->sym_type == SYM_procedure)
			error_type = "procedure";
		else
			error_type = "context";

		sprintf(s, "context %s conflicts with a %s in the same statement",
				token.tok_string, error_type);
		PAR_error(s);
	}

	symbol = MSC_symbol(SYM_context, token.tok_string, token.tok_length, 0);
	symbol->sym_object = context;
	context->ctx_symbol = symbol;
	context->ctx_alias = symbol->sym_name;
	HSH_insert(symbol);

	ADVANCE_TOKEN;

	return context;
}


//____________________________________________________________
//  
//		Parse an item is a select list.  This is particularly nasty
//		since neither the relations nor context variables have been
//		processed yet.  So, rather than generating a simple field
//		reference, make up a more or less dummy block containing
//		a pointer to a field system and possible a qualifier symbol.
//		this will be turned into a reference later.
//  

GPRE_NOD SQE_field(GPRE_REQ request, BOOLEAN aster_ok)
{
	GPRE_NOD node, tail;
	SYM symbol, temp_symbol;
	GPRE_CTX context;
	REF reference;
	GPRE_REL relation;
	GPRE_PRC procedure;
	TOK f_token;
	LLS upper_dim, lower_dim;
	int count = 0;
	GPRE_REQ slice_req;
	SLC slice;
	ACT action;
	TEXT s[ERROR_LENGTH];
	struct tok hold_token;
	slc::slc_repeat * tail_ptr;

	assert_IS_REQ(request);

	upper_dim = lower_dim = NULL;
	hold_token.tok_type = tok_t(0);

	if (aster_ok && MATCH(KW_ASTERISK)) {
		node = MAKE_NODE(nod_asterisk, 1);
		return node;
	}

//  if the token isn't an identifier, complain 

	SQL_resolve_identifier("<column name>", s);

//  For domains we can't be resolving tokens to field names
//  in the CHECK constraint. 

	if (request &&
		request->req_type == REQ_ddl &&
		(action = request->req_actions) &&
		(action->act_type == ACT_create_domain ||
		 action->act_type == ACT_alter_domain)) {
		sprintf(s, "Illegal use of identifier: %s in domain constraint",
				token.tok_string);
		PAR_error(s);
	}


//  Note: We *always* want to make a defered name block - to handle
//  scoping of alias names in subselects properly, when we haven't
//  seen the list of relations (& aliases for them).  This occurs
//  during the select list, but by the time we see the having, group,
//  order, or where we do know all aliases and should be able to
//  precisely match.
//  Note that the case of request == NULL should never
//  occur, and request->req_contexts == NULL should only
//  occur for the very first select list in a request.
//  1994-October-03 David Schnepper 
//  

//  if the request is null, make a defered name block 

	if (!request || !request->req_contexts || request->req_in_select_list) {
		node = MAKE_NODE(nod_defered, 3);
		node->nod_count = 0;
		f_token = (TOK) ALLOC(TOK_LEN);
		node->nod_arg[0] = (GPRE_NOD) f_token;
		f_token->tok_length = token.tok_length;
		SQL_resolve_identifier("<identifier>", f_token->tok_string);
		CPR_token();

		if (MATCH(KW_DOT)) {
			if ((int) token.tok_keyword == (int) KW_ASTERISK) {
				if (aster_ok)
					node->nod_type = nod_asterisk;
				else
					PAR_error("* not allowed");
			}
			else {
				node->nod_arg[1] = node->nod_arg[0];
				f_token = (TOK) ALLOC(TOK_LEN);
				node->nod_arg[0] = (GPRE_NOD) f_token;
				f_token->tok_length = token.tok_length;
				SQL_resolve_identifier("<identifier>", f_token->tok_string);
			}
			CPR_token();
		}
		if (MATCH(KW_L_BRCKET)) {
			/* We have a complete array or an array slice here */

			if (!MATCH(KW_R_BRCKET)) {
				slice_req = MAKE_REQUEST(REQ_slice);
				do {
					count++;
					tail = par_subscript(slice_req);
					PUSH(tail, &lower_dim);
					if (MATCH(KW_COLON)) {
						/* if (!MATCH (KW_DOT))
						   SYNTAX_ERROR ("<period>"); */
						tail = par_subscript(slice_req);
						PUSH(tail, &upper_dim);
					}
					else
						PUSH(tail, &upper_dim);
				} while (MATCH(KW_COMMA));

				if (!MATCH(KW_R_BRCKET))
					SYNTAX_ERROR("<right bracket>");
				slice_req->req_slice = slice = (SLC) ALLOC(SLC_LEN(count));
				tail_ptr = &slice->slc_rpt[count];
				slice->slc_dimensions = count;
				slice->slc_parent_request = request;
				while (lower_dim) {
					--tail_ptr;
					tail_ptr->slc_lower = POP(&lower_dim);
					tail_ptr->slc_upper = POP(&upper_dim);
				}
				node->nod_arg[2] = (GPRE_NOD) slice_req;

				/* added this to assign the correct nod_count 
				   The nod type is converted to nod_field in SQE_resolve()
				   The nod_count is check to confirm if the array slice
				   has been initialized in cmd.c
				 */
				node->nod_count = 3;
			}
			else {
				slice_req = (GPRE_REQ) ALLOC(REQ_LEN);
				slice_req->req_type = REQ_slice;
				node->nod_arg[2] = (GPRE_NOD) slice_req;
			}
		}

		return node;
	}

	reference = (REF) ALLOC(REF_LEN);
	node = MSC_unary(nod_field, (GPRE_NOD) reference);

	if (symbol = token.tok_symbol) {
		/* if there is a homonym which is a context, use the context;
		   otherwise we may match with a relation or procedure which 
		   is not in the request, resulting in a bogus error */

		if (symbol->sym_type != SYM_field) {
			for (temp_symbol = symbol; temp_symbol;
				 temp_symbol = temp_symbol->sym_homonym) {
				if (temp_symbol->sym_type == SYM_context) {
					symbol = temp_symbol;
					break;
				}
				else if (temp_symbol->sym_type == SYM_relation) {
					symbol = temp_symbol;
					continue;
				}
				else if (temp_symbol->sym_type == SYM_procedure) {
					if (symbol->sym_type == SYM_relation)
						continue;
					else {
						symbol = temp_symbol;
						continue;
					}
				}
			}

			if (symbol->sym_type == SYM_context) {
				context = symbol->sym_object;
				CPR_token();
				if (!MATCH(KW_DOT))
					SYNTAX_ERROR("<period> in qualified column");
				if (context->ctx_request != request)
					PAR_error("context not part of this request");
				SQL_resolve_identifier("<Column Name>", s);
				if (!
					(reference->ref_field =
					 MET_context_field(context, token.tok_string))) {
					sprintf(s, "column \"%s\" not in context",
							token.tok_string);
					PAR_error(s);
				}
				if (SQL_DIALECT_V5 == sw_sql_dialect) {
					USHORT field_dtype;
					field_dtype = reference->ref_field->fld_dtype;
					if ((dtype_sql_date == field_dtype) ||
						(dtype_sql_time == field_dtype) ||
						(dtype_int64 == field_dtype)) {
						dialect1_bad_type(field_dtype);
					}
				}
				reference->ref_context = context;
				CPR_token();
				return node;
			}
			else if (symbol->sym_type == SYM_relation) {
				relation = (GPRE_REL) symbol->sym_object;
				if (relation->rel_database != request->req_database)
					PAR_error("table not in appropriate database");

				CPR_token();

				/* if we do not see a KW_DOT, perhaps what we think is a relation
				   ** is actually a column with the same name as the relation in
				   ** the HSH table.  if so...skip down to the end of the routine
				   ** and find out if we do have such a column. but first, since
				   ** we just did a CPR_token, we have to move prior_token into
				   ** current token, and hold the current token for later.
				 */

				if (!MATCH(KW_DOT)) {
					hold_token = token;
					token = prior_token;
					token.tok_symbol = 0;
				}
				else {
		/** We've got the column name. resolve it **/
					SQL_resolve_identifier("<Columnn Name>", s);
					for (context = request->req_contexts; context;
						 context = context->ctx_next)
							if (context->ctx_relation == relation &&
								(reference->ref_field =
								 MET_field(context->ctx_relation,
										   token.tok_string))) {
							if (SQL_DIALECT_V5 == sw_sql_dialect) {
								USHORT field_dtype;
								field_dtype = reference->ref_field->fld_dtype;
								if ((dtype_sql_date == field_dtype) ||
									(dtype_sql_time == field_dtype) ||
									(dtype_int64 == field_dtype)) {
									dialect1_bad_type(field_dtype);
								}
							}
							reference->ref_context = context;
							CPR_token();
							if (reference->ref_field->fld_array_info) {
								node =
									EXP_array(request, reference->ref_field,
											  TRUE, TRUE);
								node->nod_arg[0] = (GPRE_NOD) reference;
							}
							return node;
						}

					sprintf(s, "column \"%s\" not in context",
							token.tok_string);
					PAR_error(s);
				}
			}
			else if (symbol->sym_type == SYM_procedure) {
				procedure = (GPRE_PRC) symbol->sym_object;
				if (procedure->prc_database != request->req_database)
					PAR_error("procedure not in appropriate database");
				CPR_token();
				if (!MATCH(KW_DOT))
					SYNTAX_ERROR("<period> in qualified column");
				SQL_resolve_identifier("<Column Name>", s);
				for (context = request->req_contexts; context;
					 context = context->ctx_next)
						if (context->ctx_procedure == procedure &&
							(reference->ref_field =
							 MET_context_field(context, token.tok_string))) {
						if (SQL_DIALECT_V5 == sw_sql_dialect) {
							USHORT field_dtype;
							field_dtype = reference->ref_field->fld_dtype;
							if ((dtype_sql_date == field_dtype) ||
								(dtype_sql_time == field_dtype) ||
								(dtype_int64 == field_dtype)) {
								dialect1_bad_type(field_dtype);
							}
						}
						reference->ref_context = context;
						if (reference->ref_field->fld_array_info) {
							node =
								EXP_array(request, reference->ref_field, TRUE,
										  TRUE);
							node->nod_arg[0] = (GPRE_NOD) reference;
						}
						CPR_token();
						return node;
					}

				sprintf(s, "column \"%s\" not in context", token.tok_string);
				PAR_error(s);
			}
		}
	}

//  Hmmm.  So it wasn't a qualified field.  Try any field. 

	SQL_resolve_identifier("<Column Name>", s);
	for (context = request->req_contexts; context;
		 context = context->ctx_next) if (reference->ref_field =
										  MET_context_field(context,
															token.
															tok_string)) {
			if (SQL_DIALECT_V5 == sw_sql_dialect) {
				USHORT field_dtype;
				field_dtype = reference->ref_field->fld_dtype;
				if ((dtype_sql_date == field_dtype) ||
					(dtype_sql_time == field_dtype) ||
					(dtype_int64 == field_dtype)) {
					dialect1_bad_type(field_dtype);
				}
			}
			reference->ref_context = context;

			/* if we skipped down from the SYM_relation case above, we need to
			   ** switch token and prior_token back to their original values to
			   ** continue.
			 */
			if (hold_token.tok_type != 0) {
				prior_token = token;
				token = hold_token;
			}
			else
				CPR_token();
			if (reference->ref_field->fld_array_info) {
				node = EXP_array(request, reference->ref_field, TRUE, TRUE);
				node->nod_arg[0] = (GPRE_NOD) reference;
			}
			if (request->req_map)
				return post_map(node, request->req_map);
			return node;
		}

	SYNTAX_ERROR("<column name>");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Parse a list of "things", separated by commas.  Return the
//		whole mess in a list node.
//  

GPRE_NOD SQE_list(pfn_SQE_list_cb routine, GPRE_REQ request, BOOLEAN aster_ok)
{
	LLS stack;
	GPRE_NOD list, *ptr;
	int count;

	assert_IS_REQ(request);

	stack = NULL;
	count = 0;

	do {
		count++;
		PUSH((*routine) (request, aster_ok, 0, 0), &stack);
	}
	while (MATCH(KW_COMMA));

	list = MAKE_NODE(nod_list, (SSHORT) count);
	ptr = &list->nod_arg[count];

	while (stack)
		*--ptr = (GPRE_NOD) POP(&stack);

	return list;
}


//____________________________________________________________
//  
//		Parse procedure input parameters which are constants or
//		host variable reference and, perhaps, a missing
//		flag reference, which may be prefaced by the noiseword,
//       "INDICATOR".
//  

REF SQE_parameter(GPRE_REQ request, BOOLEAN aster_ok)
{
	REF reference;
	SYM symbol;
	SCHAR *string, *s;
	int sign;

	assert_IS_REQ(request);

	if (token.tok_type == tok_number) {
		reference = (REF) ALLOC(REF_LEN);
		string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}
	if ((QUOTED(token.tok_type) && sw_sql_dialect == 1) ||
		SINGLE_QUOTED(token.tok_type)) {
	/** 
    Since we have stripped the quotes, it is time now to put it back
    so that the host language will interpret it correctly as a string 
    literal.
    ***/
		reference = (REF) ALLOC(REF_LEN);
		string = (TEXT *) ALLOC(token.tok_length + 3);
		string[0] = '\"';
		COPY(token.tok_string, token.tok_length, string + 1);
		string[token.tok_length + 1] = '\"';
		string[token.tok_length + 2] = 0;
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}
	if (KEYWORD(KW_PLUS) || KEYWORD(KW_MINUS)) {
		if (KEYWORD(KW_MINUS))
			sign = 1;
		else
			sign = 0;
		CPR_token();
		if (token.tok_type != tok_number)
			SYNTAX_ERROR("<host variable> or <constant>");
		reference = (REF) ALLOC(REF_LEN);
		s = string = (TEXT *) ALLOC(token.tok_length + 1 + sign);
		if (sign)
			*s++ = '-';
		COPY(token.tok_string, token.tok_length, s);
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}

	if (!MATCH(KW_COLON))
		SYNTAX_ERROR("<host variable> or <constant>");

	if (token.tok_type != tok_ident)
		SYNTAX_ERROR("<host variable> or <constant>");

	reference = (REF) ALLOC(REF_LEN);

	for (symbol = token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_variable) {
			reference->ref_field = (GPRE_FLD) symbol->sym_object;
			break;
		}

	reference->ref_value = PAR_native_value(FALSE, FALSE);

	MATCH(KW_INDICATOR);

	if (MATCH(KW_COLON))
		reference->ref_null_value = PAR_native_value(FALSE, FALSE);

	return reference;
}


//____________________________________________________________
//  
//		Given an expression node, for values that don't have a 
//		field, post the given field.
//		Procedure called from EXP_array to post the "subscript field".
//  

void SQE_post_field( GPRE_NOD input, GPRE_FLD field)
{
	GPRE_NOD *ptr, *end;
	REF reference;
	GPRE_NOD node;
	MEL element;

	if (!input || !field)
		return;

	assert_IS_NOD(input);

	switch (input->nod_type) {
	case nod_value:
		{
			reference = (REF) input->nod_arg[0];
			if (!reference->ref_field) {
				/* We're guessing that hostvar reference->ref_value matches
				 * the datatype of field->fld_dtype
				 */
				reference->ref_field = field;
			}
			return;
		}

	case nod_field:
	case nod_literal:
	case nod_defered:
	case nod_array:
		return;

	case nod_map_ref:
		element = (MEL) input->nod_arg[0];
		node = element->mel_expr;
		SQE_post_field(node, field);
		return;

	default:
		for (ptr = input->nod_arg, end = ptr + input->nod_count;
			 ptr < end; ptr++)
			SQE_post_field(*ptr, field);
		return;
	}
}


//____________________________________________________________
//  
//		Post an external reference to a request.  If the expression
//		in question already exists, re-use it.  If there isn't a field,
//		generate a pseudo-field to hold datatype information.  If there
//		isn't a context, well, there isn't a context.
//  

REF SQE_post_reference(GPRE_REQ request, GPRE_FLD field, GPRE_CTX context, GPRE_NOD node)
{
	REF reference;

	assert_IS_REQ(request);
	assert_IS_NOD(node);

//  If the beast is already a field reference, get component parts 

	if (node && node->nod_type == nod_field) {
		reference = (REF) node->nod_arg[0];
		field = reference->ref_field;
		context = reference->ref_context;
	}

//  See if there is already a reference to this guy.  If so, return it. 

	for (reference = request->req_references; reference;
		 reference = reference->ref_next)
			if (
				(reference->ref_expr
				 && compare_expr(node, reference->ref_expr))
				|| (!reference->ref_expr && field == reference->ref_field
					&& context == reference->ref_context))
			return reference;

//  If there isn't a field given, make one up 

	if (!field) {
		field = (GPRE_FLD) ALLOC(FLD_LEN);
		CME_get_dtype(node, field);
		if (field->fld_dtype && (field->fld_dtype <= dtype_any_text))
			field->fld_flags |= FLD_text;
	}

//  No reference -- make one 

	reference = (REF) ALLOC(REF_LEN);
	reference->ref_context = context;
	reference->ref_field = field;
	reference->ref_expr = node;

	reference->ref_next = request->req_references;
	request->req_references = reference;

	return reference;
}


//____________________________________________________________
//  
//		Resolve a kludgy field node build by par_s_item into
//		a bona fide field reference.  If a request
//		is supplied, resolve the reference to any context available
//		in the request.  Otherwise resolve the field to a given
//		record selection expression.
//  
//		If the expression contains a global aggregate, return TRUE,
//		otherwise FALSE.
//  

BOOLEAN SQE_resolve(GPRE_NOD node, GPRE_REQ request, RSE rse)
{
	REF reference;
	GPRE_CTX context;
	GPRE_FLD field;
	GPRE_NOD *ptr, *end, node_arg;
	TOK f_token, q_token;
	SSHORT i;
	BOOLEAN result;
	SCHAR s[ERROR_LENGTH];
	ACT slice_action = 0;

	assert_IS_REQ(request);
	assert_IS_NOD(node);

	result = FALSE;

	switch (node->nod_type) {
	case nod_plus:
	case nod_minus:
	case nod_times:
	case nod_divide:
	case nod_negate:
	case nod_upcase:
	case nod_concatenate:
	case nod_cast:
		ptr = node->nod_arg;
		end = ptr + node->nod_count;
		for (; ptr < end; ptr++)
			result |= SQE_resolve(*ptr, request, rse);
		return result;

	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
	case nod_agg_average:
	case nod_agg_count:
		if (node->nod_arg[0]) {
			SQE_resolve(node->nod_arg[0], request, rse);
			node_arg = node->nod_arg[0];
			reference = (REF) node_arg->nod_arg[0];
			if (node_arg->nod_type == nod_field && reference &&
				reference->ref_field && reference->ref_field->fld_array_info)
				PAR_error
					("Array columns not permitted in aggregate functions");
		}
		return TRUE;

	case nod_udf:
		if (node->nod_arg[0]) {
			ptr = node->nod_arg[0]->nod_arg;
			end = ptr + node->nod_arg[0]->nod_count;
			for (; ptr < end; ptr++)
				result |= SQE_resolve(*ptr, request, rse);
		}
		return result;

	case nod_gen_id:
		return SQE_resolve(node->nod_arg[0], request, rse);

// ** Begin date/time/timestamp support *
	case nod_extract:
		result |= SQE_resolve(node->nod_arg[1], request, rse);
		return result;
// ** End date/time/timestamp support *

	case nod_defered:
		break;

	default:
		return FALSE;
	}

	f_token = (TOK) node->nod_arg[0];
	f_token->tok_symbol = HSH_lookup(f_token->tok_string);
	if (q_token = (TOK) node->nod_arg[1])
		q_token->tok_symbol = HSH_lookup(q_token->tok_string);

	field = NULL;

	if (request)
		for (context = request->req_contexts; context;
			 context = context->ctx_next) {
			if (!context->ctx_stream
				&& (field = resolve(node, context, 0, &slice_action))) break;
		}
	else
		for (i = 0; i < rse->rse_count; i++) {
			if (field =
				resolve(node, rse->rse_context[i], &context,
						&slice_action)) break;
		}

	if (!field) {
		if (q_token)
			sprintf(s, "column \"%s.%s\" cannot be resolved",
					q_token->tok_string, f_token->tok_string);
		else
			sprintf(s, "column \"%s\" cannot be resolved",
					f_token->tok_string);
		PAR_error(s);
	}

//  Make sure that a dialect-1 program isn't trying to select a
//  dialect-3-only field type. 
	if ((SQL_DIALECT_V5 == sw_sql_dialect) &&
		((dtype_sql_date == field->fld_dtype) ||
		 (dtype_sql_time == field->fld_dtype) ||
		 (dtype_int64 == field->fld_dtype)))
			dialect1_bad_type(field->fld_dtype);

	reference = (REF) ALLOC(REF_LEN);
	reference->ref_field = field;
	reference->ref_context = context;
	reference->ref_slice = (SLC) slice_action;

//  donot reinit if this is a nod_deffered type 
	if (node->nod_type != nod_defered)
		node->nod_count = 0;


	node->nod_type = nod_field;
	node->nod_arg[0] = (GPRE_NOD) reference;

	return FALSE;
}


//____________________________________________________________
//  
//		Parse a SELECT (sans keyword) expression.
//  

RSE SQE_select(GPRE_REQ request, USHORT view_flag)
{
	RSE select, rse1, rse2;
	GPRE_NOD node;
	LLS context_stack;
	GPRE_CTX context;
	MAP map, old_map;
	BOOLEAN have_union;

	assert_IS_REQ(request);

	context_stack = NULL;
	select = rse1 = rse2 = NULL;
	old_map = request->req_map;
	have_union = FALSE;

//  Get components of union.  Most likely there isn't one, so this is
//  probably wasted work.  

	select = rse1 = par_select(request, NULL);

//  "Look for ... the UNION label ... " 
	while (MATCH(KW_UNION)) {
		BOOLEAN union_all;

		have_union = TRUE;
		union_all = FALSE;
		if (MATCH(KW_ALL))
			union_all = TRUE;
		if (!MATCH(KW_SELECT))
			SYNTAX_ERROR("SELECT");

		PUSH((GPRE_NOD) request->req_contexts, &context_stack);
		request->req_contexts = NULL;
		request->req_map = NULL;
		rse2 = par_select(request, rse1);

		/* We've got a bona fide union.  Make a union node to hold sub-rse
		   and then a new rse to point to it. */

		select = (RSE) ALLOC(RSE_LEN(1));
		select->rse_context[0] = context = MAKE_CONTEXT(request);
		select->rse_union = node = MAKE_NODE(nod_union, 2);
		node->nod_arg[0] = (GPRE_NOD) rse1;
		node->nod_arg[1] = (GPRE_NOD) rse2;

		rse1->rse_map = map = (MAP) ALLOC(sizeof(struct map));
		map->map_context = context;
		select->rse_fields = post_select_list(rse1->rse_fields, map);

		rse2->rse_map = map = (MAP) ALLOC(sizeof(struct map));
		map->map_context = context;
		(void) post_select_list(rse2->rse_fields, map);

		select->rse_into = rse1->rse_into;
		if (!union_all)
			select->rse_reduced = select->rse_fields;

		/* Result of this UNION might be the left side of the NEXT UNION */
		rse1 = select;
	}

//  Restore the context lists that were forgotten 
//  <context> holds the most recently allocated context, which is
//  already linked into the request block 

	while (context_stack) {
		while (context->ctx_next)
			context = context->ctx_next;
		context->ctx_next = (GPRE_CTX) POP(&context_stack);
	}

//  Pick up any dangling ORDER clause 

	++request->req_in_order_by_clause;
	par_order(request, select, have_union, view_flag);
	--request->req_in_order_by_clause;
	request->req_map = old_map;

	return select;
}


//____________________________________________________________
//  
//		Parse either of the low precedence operators + and -.
//  

GPRE_NOD SQE_value(GPRE_REQ request,
			  BOOLEAN aster_ok, USHORT * paren_count, USHORT * bool_flag)
{
	GPRE_NOD node, arg;
	nod_t operator_;
	USHORT local_count, local_flag;

	assert_IS_REQ(request);

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}
	if (!bool_flag) {
		local_flag = FALSE;
		bool_flag = &local_flag;
	}

	MATCH(KW_PLUS);
	node = par_multiply(request, aster_ok, paren_count, bool_flag);
	assert_IS_NOD(node);
	if (node->nod_type == nod_asterisk) {
		par_terminating_parens(paren_count, &local_count);
		return node;
	}

	while (TRUE) {
		if (MATCH(KW_PLUS))
			operator_ = nod_plus;
		else if (MATCH(KW_MINUS))
			operator_ = nod_minus;
		else if (MATCH(KW_OR1))
			operator_ = nod_concatenate;
		else {
			par_terminating_parens(paren_count, &local_count);
			return node;
		}
		arg = node;
		node =
			MSC_binary(operator_, arg,
					   par_multiply(request, FALSE, paren_count, bool_flag));
	}
}


//____________________________________________________________
//  
//		Parse either a literal NULL expression or a value
//		expression.
//  

GPRE_NOD SQE_value_or_null(GPRE_REQ request,
					  BOOLEAN aster_ok,
					  USHORT * paren_count, USHORT * bool_flag)
{
	if (MATCH(KW_NULL)) {
		return MAKE_NODE(nod_null, 0);
	}
	else
		return SQE_value(request, aster_ok, paren_count, bool_flag);
}


//____________________________________________________________
//  
//		Parse host variable reference and, perhaps, a missing
//		flag reference, which may be prefaced by the noiseword,
//       "INDICATOR".
//  

REF SQE_variable(GPRE_REQ request, BOOLEAN aster_ok)
{
	REF reference;
	SYM symbol;

	assert_IS_REQ(request);

	if (!MATCH(KW_COLON))
		SYNTAX_ERROR("<colon>");

	if (QUOTED(token.tok_type))
		SYNTAX_ERROR("<host variable>");

	reference = (REF) ALLOC(REF_LEN);

	for (symbol = token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_variable) {
			reference->ref_field = (GPRE_FLD) symbol->sym_object;
			break;
		}

	reference->ref_value = PAR_native_value(FALSE, FALSE);

	MATCH(KW_INDICATOR);

	if (MATCH(KW_COLON))
		reference->ref_null_value = PAR_native_value(FALSE, FALSE);

	return reference;
}


//____________________________________________________________
//  
//		Compare two expressions symbollically.  If they're the same,
//		return TRUE, otherwise FALSE.
//  

static BOOLEAN compare_expr( GPRE_NOD node1, GPRE_NOD node2)
{
	REF ref1, ref2;

	assert_IS_NOD(node1);
	assert_IS_NOD(node2);

	if (node1->nod_type != node2->nod_type)
		return FALSE;

	switch (node1->nod_type) {
	case nod_field:
		ref1 = (REF) node1->nod_arg[0];
		ref2 = (REF) node2->nod_arg[0];
		if (ref1->ref_context != ref2->ref_context ||
			ref1->ref_field != ref2->ref_field ||
			ref1->ref_master != ref2->ref_master) return FALSE;
		return TRUE;

	case nod_map_ref:
		if (node1->nod_arg[0] != node2->nod_arg[0])
			return FALSE;
		return TRUE;

	case nod_udf:
	case nod_gen_id:
		if (node1->nod_arg[0] != node2->nod_arg[0] ||
			node1->nod_arg[1] != node2->nod_arg[1])
			return FALSE;
		return TRUE;

	default:
		return FALSE;
	}
}


//____________________________________________________________
//  
//		Copy a field list for a SELECT against an artificial context.
//  

static GPRE_NOD copy_fields( GPRE_NOD fields, MAP map)
{
	GPRE_NOD list;
	USHORT i;

	assert_IS_NOD(fields);

	list = MAKE_NODE(nod_list, fields->nod_count);

	for (i = 0; i < fields->nod_count; i++)
		list->nod_arg[i] = post_fields(fields->nod_arg[i], map);

	return list;
}


//____________________________________________________________
//  
//		Expand an '*' in a field list to the corresponding fields.
//  

static GPRE_NOD explode_asterisk( GPRE_NOD fields, int n, RSE rse)
{
	GPRE_CTX context;
	GPRE_NOD node;
	TOK q_token;
	TEXT s[ERROR_LENGTH];

	assert_IS_NOD(fields);

	node = fields->nod_arg[n];
	if (q_token = (TOK) node->nod_arg[0]) {
		/* expand for single relation */

		if (context = resolve_asterisk(q_token, rse))
			fields = merge_fields(fields, MET_fields(context), n, TRUE);
		else {
			sprintf(s, "columns \"%s.*\" cannot be resolved",
					q_token->tok_string);
			PAR_error(s);
		}
	}
	else {
		/* expand for all relations in context list */

		fields = explode_asterisk_all(fields, n, rse, TRUE);
	}

	return fields;
}


//____________________________________________________________
//  
//		Expand an '*' for all relations
//		in the context list.
//  

static GPRE_NOD explode_asterisk_all( GPRE_NOD fields, int n, RSE rse, BOOLEAN replace)
{
	GPRE_CTX context;
	int i, old_count;

	assert_IS_NOD(fields);

	for (i = 0; i < rse->rse_count; i++) {
		context = rse->rse_context[i];
		old_count = fields->nod_count;
		if (context->ctx_stream)
			fields =
				explode_asterisk_all(fields, n, context->ctx_stream, replace);
		else
			fields = merge_fields(fields, MET_fields(context), n, replace);
		n += fields->nod_count - old_count;
		replace = FALSE;
	}

	return fields;
}


//____________________________________________________________
//  
//		Get an element of an expression to act as a reference
//		field for determining the data type of a host variable.
//  

static GPRE_FLD get_ref( GPRE_NOD expr)
{
	REF ref;
	GPRE_NOD *ptr, *end, node;
	GPRE_FLD field;
	MEL element;

	assert_IS_NOD(expr);

	if (expr->nod_type == nod_via || expr->nod_type == nod_cast) {
		field = (GPRE_FLD) ALLOC(FLD_LEN);
		CME_get_dtype(expr, field);
		if (field->fld_dtype && (field->fld_dtype <= dtype_any_text))
			field->fld_flags |= FLD_text;
		return field;
	}

	switch (expr->nod_type) {
	case nod_field:
		ref = (REF) expr->nod_arg[0];
		return ref->ref_field;


	case nod_array:
		ref = (REF) expr->nod_arg[0];
		return ref->ref_field->fld_array;

	case nod_agg_count:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
	case nod_agg_average:
	case nod_plus:
	case nod_minus:
	case nod_times:
	case nod_divide:
	case nod_negate:
	case nod_upcase:
	case nod_concatenate:
		for (ptr = expr->nod_arg, end = ptr + expr->nod_count; ptr < end;
			 ptr++)
			if (field = get_ref(*ptr))
				return field;
		break;

// ** Begin date/time/timestamp support *
	case nod_extract:
		if (field = get_ref(expr->nod_arg[1]))
			return field;
		break;
// ** End date/time/timestamp support *
	case nod_map_ref:
		element = (MEL) expr->nod_arg[0];
		node = element->mel_expr;
		return get_ref(node);
	}

	return 0;
}


//____________________________________________________________
//  
//		Finish off processing an implicit ANY clause.  Assume that the word
//		"select" has already been recognized.  If the outer thing is a group
//		by, so we're looking at a "having", re-resolve the input value to the
//		map.
//  

static GPRE_NOD implicit_any(
						GPRE_REQ request,
						GPRE_NOD value, enum nod_t comparison, enum nod_t any_all)
{
	GPRE_NOD value2, node, node2, field_list;
	RSE rse, sub;
	GPRE_CTX original;
	BOOLEAN distinct;
	struct scope previous_scope;

	assert_IS_REQ(request);
	assert_IS_NOD(value);

	original = request->req_contexts;

	request->req_in_subselect++;
	push_scope(request, &previous_scope);

	if (!(original->ctx_relation || original->ctx_procedure) &&
		request->req_map) value = post_fields(value, request->req_map);

//  Handle the ALL and DISTINCT options 
	distinct = (!MATCH(KW_ALL) && MATCH(KW_DISTINCT)) ? TRUE : FALSE;

	request->req_in_select_list++;
	value2 = SQE_value(request, FALSE, 0, 0);
	request->req_in_select_list--;

	field_list = MAKE_NODE(nod_list, 1);
	field_list->nod_arg[0] = value2;

	rse = par_rse(request, field_list, distinct);
	value2 = rse->rse_fields->nod_arg[0];

	if (sub = rse->rse_aggregate) {
		if (validate_references(value2, sub->rse_group_by))
			PAR_error
				("simple column reference not allowed in aggregate context");
		if (sub->rse_group_by) {
			node = MSC_binary(comparison, value, value2);
			pair(node->nod_arg[0], node->nod_arg[1]);
			rse->rse_boolean = merge(rse->rse_boolean, node);
			if (any_all == nod_ansi_all)
				node = MAKE_NODE(nod_ansi_all, 1);
			else if (!(request->req_database->dbb_flags & DBB_v3))
				node = MAKE_NODE(nod_ansi_any, 1);
			else
				node = MAKE_NODE(nod_any, 1);
			node->nod_count = 0;
			node->nod_arg[0] = (GPRE_NOD) rse;
		}
		else {
			node2 = MAKE_NODE(nod_via, 3);
			node2->nod_count = 0;
			node2->nod_arg[0] = (GPRE_NOD) rse;
			node2->nod_arg[2] = MAKE_NODE(nod_null, 0);
			node2->nod_arg[1] = value2;
			node = MSC_binary(comparison, value, node2);
			pair(node->nod_arg[0], node->nod_arg[1]);
		}
	}
	else {
		node = MSC_binary(comparison, value, value2);
		pair(node->nod_arg[0], node->nod_arg[1]);
		rse->rse_boolean = merge(rse->rse_boolean, node);
		if (any_all == nod_ansi_all)
			node = MAKE_NODE(nod_ansi_all, 1);
		else if (!(request->req_database->dbb_flags & DBB_v3))
			node = MAKE_NODE(nod_ansi_any, 1);
		else
			node = MAKE_NODE(nod_any, 1);
		node->nod_count = 0;
		node->nod_arg[0] = (GPRE_NOD) rse;
	}

	EXP_rse_cleanup(rse);

	pop_scope(request, &previous_scope);
	request->req_in_subselect--;

	return node;
}


//____________________________________________________________
//  
//		Merge two (possibly null) booleans into a single conjunct.
//  

static GPRE_NOD merge( GPRE_NOD expr1, GPRE_NOD expr2)
{

	if (!expr1)
		return expr2;

	if (!expr2)
		return expr1;

	assert_IS_NOD(expr1);
	assert_IS_NOD(expr1);

	return MSC_binary(nod_and, expr1, expr2);
}


//____________________________________________________________
//  
//		Merge 2 field lists
//		  2nd list is added over nth entry in 1st list
//		  if replace is TRUE, otherwise it is added
//		  after the nth entry.
//  

static GPRE_NOD merge_fields( GPRE_NOD fields_1, GPRE_NOD fields_2, int n, BOOLEAN replace)
{
	GPRE_NOD fields;
	int i, count, offset;

	assert_IS_NOD(fields_1);
	assert_IS_NOD(fields_2);

	count = fields_1->nod_count + fields_2->nod_count;
	if (replace)
		count--;
	fields = MAKE_NODE(nod_list, (SSHORT) count);

	count = n;
	if (!replace)
		count++;
	for (i = 0; i < count; i++)
		fields->nod_arg[i] = fields_1->nod_arg[i];

	for (i = 0; i < fields_2->nod_count; i++)
		fields->nod_arg[i + count] = fields_2->nod_arg[i];

	offset = 0;
	if (replace) {
		count++;
		offset = 1;
	}

	for (i = count; i < fields_1->nod_count; i++)
		fields->nod_arg[i + fields_2->nod_count - offset] =
			fields_1->nod_arg[i];

	return fields;
}


//____________________________________________________________
//  
//		Construct negation of expression.
//  

static GPRE_NOD negate( GPRE_NOD expr)
{

	assert_IS_NOD(expr);

	return MSC_unary(nod_not, expr);
}


//____________________________________________________________
//  
//		Given two value expressions associated in a relational
//		expression, see if one is a field reference and the other
//		is a host language variable..  If so, match the field to the
//		host language variable.
//  

static void pair( GPRE_NOD expr1, GPRE_NOD expr2)
{
	GPRE_FLD field, temp;

	assert_IS_NOD(expr1);
	assert_IS_NOD(expr2);

//  Verify that an array field without subscripts is not
//  being used inappropriately 

	if (expr1 && expr2) {
		if (expr1->nod_type == nod_array && !expr1->nod_arg[1])
			PAR_error("Invalid array column reference");
		if (expr2->nod_type == nod_array && !expr2->nod_arg[1])
			PAR_error("Invalid array column reference");
	}

	field = 0;
	if (expr2)
		field = get_ref(expr2);
	if (!field)
		field = get_ref(expr1);

	if (!field)
		return;

	set_ref(expr1, field);

	if (!expr2)
		return;

	if (temp = get_ref(expr1))
		field = temp;

	set_ref(expr2, field);
}


//____________________________________________________________
//  
//		The passed alias list fully specifies a relation.
//		The first alias represents a relation specified in
//		the from list at this scope levels.  Subsequent
//		contexts, if there are any, represent base relations
//		in a view stack.  They are used to fully specify a 
//		base relation of a view.  The aliases used in the 
//		view stack are those used in the view definition.
//  

static GPRE_CTX par_alias_list( GPRE_REQ request, GPRE_NOD alias_list)
{
	GPRE_CTX context, new_context;
	GPRE_REL relation;
	GPRE_NOD *arg, *end;
	USHORT alias_length;
	TEXT *p, *q, *alias;
	SCHAR error_string[ERROR_LENGTH];

	assert_IS_REQ(request);
	assert_IS_NOD(alias_list);

	arg = alias_list->nod_arg;
	end = alias_list->nod_arg + alias_list->nod_count;

//  check the first alias in the list with the relations
//  in the current context for a match 

	if (context = par_alias(request, (TEXT *) * arg)) {
		if (alias_list->nod_count == 1)
			return context;
		relation = context->ctx_relation;
	}

//  if the first alias didn't specify a table in the context stack, 
//  look through all contexts to find one which might be a view with
//  a base table having a matching table name or alias 

	if (!context)
		for (context = request->req_contexts; context;
			 context = context->ctx_next) {
			if (context->ctx_scope_level != request->req_scope_level)
				continue;
			if (!context->ctx_relation)
				continue;
			if (relation =
				par_base_table(request, context->ctx_relation,
							   (TEXT *) * arg)) break;
		}

	if (!context) {
		sprintf(error_string,
				"there is no alias or table named %s at this scope level",
				(TEXT *) * arg);
		PAR_error(error_string);
	}

//  find the base table using the specified alias list, skipping the first one
//  since we already matched it to the context 

	for (arg++; arg < end; arg++)
		if (!(relation = par_base_table(request, relation, (TEXT *) * arg)))
			break;

	if (!relation) {
		sprintf(error_string,
				"there is no alias or table named %s at this scope level",
				(TEXT *) * arg);
		PAR_error(error_string);
	}

//  make up a dummy context to hold the resultant relation 

	new_context = (GPRE_CTX) ALLOC(CTX_LEN);
	new_context->ctx_request = request;
	new_context->ctx_internal = context->ctx_internal;
	new_context->ctx_relation = relation;

//  concatenate all the contexts to form the alias name;
//  calculate the length leaving room for spaces and a null 

	alias_length = alias_list->nod_count;
	for (arg = alias_list->nod_arg; arg < end; arg++)
		alias_length += strlen((TEXT *) * arg);

	alias = (TEXT *) ALLOC(alias_length);

	p = new_context->ctx_alias = alias;
	for (arg = alias_list->nod_arg; arg < end; arg++) {
		for (q = (TEXT *) * arg; *q;)
			*p++ = *q++;
		*p++ = ' ';
	}

	p[-1] = 0;

	return new_context;
}


//____________________________________________________________
//  
//		The passed relation or alias represents 
//		a context which was previously specified
//		in the from list.  Find and return the 
//		proper context.
//  

static GPRE_CTX par_alias( GPRE_REQ request, TEXT * alias)
{
	GPRE_CTX context, relation_context = NULL;
	SCHAR error_string[ERROR_LENGTH];
	TEXT *p, *q;

	assert_IS_REQ(request);

//  look through all contexts at this scope level
//  to find one that has a relation name or alias
//  name which matches the identifier passed 

	for (context = request->req_contexts; context;
		 context = context->ctx_next) {
		if (context->ctx_scope_level != request->req_scope_level)
			continue;

		/* check for matching alias */

		if (context->ctx_alias) {
			for (p = context->ctx_alias, q = alias; *p && *q; p++, q++)
				if (UPPER(*p) != UPPER(*q))
					break;
			if (!*p && !*q)
				return context;
		}

		/* check for matching relation name; aliases take priority so
		   save the context in case there is an alias of the same name;
		   also to check that there is no self-join in the query */

		if (context->ctx_relation &&
			!strcmp(context->ctx_relation->rel_symbol->sym_string, alias)) {
			if (relation_context) {
				sprintf(error_string,
						"the table %s is referenced twice; use aliases to differentiate",
						alias);
				PAR_error(error_string);
			}
			relation_context = context;
		}
	}

	return relation_context;
}


//____________________________________________________________
//  
//		Check if the relation in the passed context
//		has a base table which matches the passed alias.
//  

static GPRE_REL par_base_table( GPRE_REQ request, GPRE_REL relation, TEXT * alias)
{

	assert_IS_REQ(request);

	return MET_get_view_relation(request, relation->rel_symbol->sym_string,
								 alias, 0);
}


//____________________________________________________________
//  
//		Parse an AND boolean expression.
//  

static GPRE_NOD par_and( GPRE_REQ request, USHORT * paren_count)
{
	GPRE_NOD expr1;

	assert_IS_REQ(request);

	expr1 = par_not(request, paren_count);

	if (!MATCH(KW_AND))
		return expr1;

	return merge(expr1, par_and(request, paren_count));
}


//____________________________________________________________
//  
//  

static GPRE_NOD par_collate( GPRE_REQ request, GPRE_NOD arg)
{
	GPRE_FLD field;
	GPRE_NOD node;

	assert_IS_REQ(request);
	assert_IS_NOD(arg);

	node = MAKE_NODE(nod_cast, 2);
	node->nod_count = 1;
	node->nod_arg[0] = arg;
	field = (GPRE_FLD) ALLOC(FLD_LEN);
	node->nod_arg[1] = (GPRE_NOD) field;
	CME_get_dtype(arg, field);
	if (field->fld_dtype > dtype_any_text) {
		/* cast expression to VARYING with implementation-defined */
		/* maximum length */

		field->fld_dtype = dtype_varying;
		field->fld_char_length = 30;
		field->fld_length = 0;	/* calculated by SQL_adjust_field_dtype */
		field->fld_scale = 0;
		field->fld_sub_type = 0;
	}
	else if (field->fld_sub_type) {
		field->fld_character_set = MET_get_text_subtype(field->fld_sub_type);
	}
	SQL_par_field_collate(request, field);
	SQL_adjust_field_dtype(field);

	return node;
}


//____________________________________________________________
//  
//		Parse a SQL "IN" expression.  This comes in two flavors:
//  
//			<value> IN (<value_comma_list>)
//			<value> IN (SELECT <column> <from_nonsense>)
//  

static GPRE_NOD par_in( GPRE_REQ request, GPRE_NOD value)
{
	GPRE_NOD value2, node;
	REF ref1, ref2;
	SCHAR s[ERROR_LENGTH];

	assert_IS_REQ(request);
	assert_IS_NOD(value);

	EXP_left_paren(0);

//  If the next token isn't SELECT, we must have the comma list flavor. 

	if (MATCH(KW_SELECT))
		node = implicit_any(request, value, nod_eq, nod_ansi_any);
	else {
		node = NULL;
		while (TRUE) {
			value2 = par_primitive_value(request, FALSE, 0, 0);
			if (value2->nod_type == nod_value) {
				ref2 = (REF) value2->nod_arg[0];
				if (value->nod_type == nod_field) {
					ref1 = (REF) value->nod_arg[0];
					ref2->ref_field = ref1->ref_field;
				}
				else {
					sprintf(s, "datatype of %s can not be determined",
							token.tok_string);
					PAR_error(s);
				}
			}
			if (!node)
				node = MSC_binary(nod_eq, value, value2);
			else
				node =
					MSC_binary(nod_or, node,
							   MSC_binary(nod_eq, value, value2));

			if (!(MATCH(KW_COMMA)))
				break;
		}
	}

	if (!EXP_match_paren())
		return NULL;

	return node;
}


//____________________________________________________________
//  
//		Parse a join relation clause.
//  

static GPRE_CTX par_joined_relation( GPRE_REQ request, GPRE_CTX prior_context)
{
	GPRE_CTX context1;

	assert_IS_REQ(request);

	if (MATCH(KW_LEFT_PAREN)) {
		context1 = par_joined_relation(request, NULL);
		EXP_match_paren();
	}
	else if (!(context1 = SQE_context(request)))
		return NULL;

	return par_join_clause(request, context1);
}


//____________________________________________________________
//  
//		Parse a join relation clause.
//  

static GPRE_CTX par_join_clause( GPRE_REQ request, GPRE_CTX context1)
{
	GPRE_CTX context2;
	NOD_T join_type;
	GPRE_NOD node;
	RSE rse;

	assert_IS_REQ(request);

	join_type = par_join_type();
	if (join_type == (NOD_T) 0)
		return context1;

	if (!(context2 = par_joined_relation(request, context1)))
		SYNTAX_ERROR("<joined table clause>");

	if (!MATCH(KW_ON))
		SYNTAX_ERROR("ON");

	node = SQE_boolean(request, NULL);

	rse = (RSE) ALLOC(RSE_LEN(2));
	rse->rse_count = 2;
	rse->rse_context[0] = context1;
	rse->rse_context[1] = context2;
	rse->rse_boolean = node;
	rse->rse_join_type = join_type;

	context1 = MAKE_CONTEXT(request);
	context1->ctx_stream = rse;

	return par_join_clause(request, context1);
}


//____________________________________________________________
//  
//		Parse a join type.
//  

static NOD_T par_join_type(void)
{
	NOD_T operator_;

	if (MATCH(KW_INNER))
		operator_ = nod_join_inner;
	else if (MATCH(KW_LEFT))
		operator_ = nod_join_left;
	else if (MATCH(KW_RIGHT))
		operator_ = nod_join_right;
	else if (MATCH(KW_FULL))
		operator_ = nod_join_full;
	else if (MATCH(KW_JOIN))
		return nod_join_inner;
	else
		return (NOD_T) 0;

	if (operator_ != nod_join_inner)
		MATCH(KW_OUTER);

	if (!MATCH(KW_JOIN))
		SYNTAX_ERROR("JOIN");

	return operator_;
}


//____________________________________________________________
//  
//		Parse either of the high precedence operators * and /.
//  

static GPRE_NOD par_multiply(
						GPRE_REQ request,
						BOOLEAN aster_ok,
						USHORT * paren_count, USHORT * bool_flag)
{
	GPRE_NOD node, arg;
	enum nod_t operator_;

	assert_IS_REQ(request);
	node = par_primitive_value(request, aster_ok, paren_count, bool_flag);
	if (node->nod_type == nod_asterisk)
		return node;

	if (KEYWORD(KW_COLLATE))
		return par_collate(request, node);

	while (TRUE) {
		if (MATCH(KW_ASTERISK))
			operator_ = nod_times;
		else if (MATCH(KW_SLASH))
			operator_ = nod_divide;
		else
			return node;
		arg = node;
		node =
			MSC_binary(operator_, arg,
					   par_primitive_value(request, FALSE, paren_count,
										   bool_flag));
	}
}


//____________________________________________________________
//  
//		Parse an NOT boolean expression.
//  

static GPRE_NOD par_not( GPRE_REQ request, USHORT * paren_count)
{
	RSE rse;
	GPRE_NOD node, expr, field;
	enum nod_t type;
	struct scope saved_scope;

	assert_IS_REQ(request);

	if (MATCH(KW_NOT))
		return negate(par_not(request, paren_count));

	type = (enum nod_t) 0;

	if (MATCH(KW_EXISTS))
		type = nod_any;
	else if (MATCH(KW_SINGULAR))
		type = nod_unique;
	if (type == nod_any || type == nod_unique) {
		push_scope(request, &saved_scope);

		EXP_left_paren(0);
		if (!MATCH(KW_SELECT))
			SYNTAX_ERROR("SELECT");

		request->req_in_select_list++;
		if (MATCH(KW_ASTERISK))
			field = NULL;
		else if (!(field = par_udf(request)))
			field = SQE_field(request, FALSE);
		request->req_in_select_list--;

		node = MAKE_NODE(type, 1);
		node->nod_count = 0;
		rse = par_rse(request, 0, FALSE);
		node->nod_arg[0] = (GPRE_NOD) rse;
		if (field) {
			SQE_resolve(field, 0, rse);
			expr = MSC_unary(nod_missing, field);
			rse->rse_boolean = merge(negate(expr), rse->rse_boolean);
		}
		EXP_rse_cleanup((RSE) node->nod_arg[0]);
		pop_scope(request, &saved_scope);
		EXP_match_paren();
		return node;
	}

	return par_relational(request, paren_count);
}


//____________________________________________________________
//  
//		Parse ORDER clause of SELECT expression.  This is 
//		particularly difficult since the ORDER clause can
//		refer to fields by position.
//  

static void par_order(
					  GPRE_REQ request,
					  RSE select, SSHORT union_f, USHORT view_flag)
{
	GPRE_NOD sort, *ptr, values;
	LLS items, directions;
	MAP map;
	int count, direction;
	USHORT i;

	assert_IS_REQ(request);

//  This doesn't really belong here, but it's convenient.  Parse the
//  SQL "FOR UPDATE OF ..." clause.  Just eat it and ignore it. 

	if (MATCH(KW_FOR)) {
		MATCH(KW_UPDATE);
		MATCH(KW_OF);
		do
			CPR_token();
		while (MATCH(KW_COMMA));
	}

	if (!MATCH(KW_ORDER))
		return;
	if (view_flag)
		PAR_error("sort clause not allowed in a view definition");

	MATCH(KW_BY);
	items = directions = NULL;
	count = direction = 0;
	values = select->rse_fields;

	while (TRUE) {
		direction = FALSE;
		if (token.tok_type == tok_number) {
			i = EXP_USHORT_ordinal(FALSE);
			if (i < 1 || i > values->nod_count)
				SYNTAX_ERROR("<ordinal column position>");
			sort = values->nod_arg[i - 1];
			ADVANCE_TOKEN;
			if (KEYWORD(KW_COLLATE))
				sort = par_collate(request, sort);
		}
		else {
			if (union_f)
				SYNTAX_ERROR("<column position in union>");
			sort = SQE_value(request, FALSE, 0, 0);
			if (request && (map = request->req_map))
				sort = post_map(sort, map);
		}
		if (MATCH(KW_ASCENDING))
			direction = FALSE;
		else if (MATCH(KW_DESCENDING))
			direction = TRUE;
		count++;
		PUSH((GPRE_NOD) direction, &directions);
		PUSH(sort, &items);
		if (!MATCH(KW_COMMA))
			break;
	}

	select->rse_sort = sort = MAKE_NODE(nod_sort, (SSHORT) (count * 2));
	sort->nod_count = count;
	ptr = sort->nod_arg + count * 2;

	while (items) {
		*--ptr = (GPRE_NOD) POP(&items);
		*--ptr = (GPRE_NOD) POP(&directions);
	}
}


//____________________________________________________________
//  
//		Allow the user to specify the access plan
//		for a query as part of a select expression.
//  

static GPRE_NOD par_plan( GPRE_REQ request)
{
	NOD_T operator_;
	GPRE_NOD plan_expression;

	assert_IS_REQ(request);

//  parse the join type 

	if (MATCH(KW_JOIN))
		operator_ = nod_join;
	else if (MATCH(KW_MERGE))
		operator_ = nod_merge;
	else if (MATCH(KW_SORT) && MATCH(KW_MERGE))
		operator_ = nod_merge;
	else
		operator_ = nod_join;

//  make up the plan expression node 

	plan_expression = MAKE_NODE(nod_plan_expr, 2);

	if (operator_ != nod_join)
		plan_expression->nod_arg[0] = MAKE_NODE(operator_, 0);

//  parse the plan items at this level 

	EXP_left_paren(0);

#pragma FB_COMPILER_MESSAGE("Fix! Wrong function ptr type!")
//
// Please search for "reinterpret_cast<pfn_SQE_list_cb>",
// there are more in this file.
//
	plan_expression->nod_arg[1] =
		SQE_list(reinterpret_cast < pfn_SQE_list_cb > (par_plan_item),
				 request, FALSE);

	if (!EXP_match_paren())
		return NULL;
	return plan_expression;
}


//____________________________________________________________
//  
//		Parse an individual plan item for an 
//		access plan.
//  

static GPRE_NOD par_plan_item( GPRE_REQ request, BOOLEAN aster_ok)
{
	LLS stack = NULL;
	int count;
	GPRE_NOD plan_item, alias_list, access_type, index_list, *ptr;
	GPRE_CTX context;

	assert_IS_REQ(request);

//  check for a plan expression 

	if (KEYWORD(KW_JOIN) ||
		KEYWORD(KW_SORT) ||
		KEYWORD(KW_MERGE) || KEYWORD(KW_LEFT_PAREN)) return par_plan(request);

//  parse the list of one or more table names or
//  aliases (more than one is used when there is
//  a need to differentiate base tables of a view) 

	for (count = 0; token.tok_type == tok_ident; count++) {
		if (KEYWORD(KW_NATURAL) || KEYWORD(KW_ORDER) || KEYWORD(KW_INDEX))
			break;

		PUSH((GPRE_NOD) upcase_string(token.tok_string), &stack);
		ADVANCE_TOKEN;
	}

	if (!count)
		SYNTAX_ERROR("<table name> or <alias>");

	alias_list = MAKE_NODE(nod_list, (SSHORT) count);
	for (ptr = &alias_list->nod_arg[count]; stack;)
		*--ptr = (GPRE_NOD) POP(&stack);

//  lookup the contexts for the aliases 

	context = par_alias_list(request, alias_list);

//  parse the access type 

	if (KEYWORD(KW_NATURAL)) {
		access_type = MAKE_NODE(nod_natural, 0);
		ADVANCE_TOKEN;
	}
	else if (KEYWORD(KW_ORDER)) {
		access_type = MAKE_NODE(nod_index_order, 1);
		access_type->nod_count = 0;
		ADVANCE_TOKEN;

		if (token.tok_type != tok_ident)
			SYNTAX_ERROR("<index name>");
		access_type->nod_arg[0] = (GPRE_NOD) upcase_string(token.tok_string);
		ADVANCE_TOKEN;
	}
	else if (KEYWORD(KW_INDEX)) {
		access_type = MAKE_NODE(nod_index, 1);
		access_type->nod_count = 0;
		ADVANCE_TOKEN;

		EXP_left_paren(0);

		stack = NULL;
		for (count = 0; token.tok_type == tok_ident;) {
			PUSH((GPRE_NOD) upcase_string(token.tok_string), &stack);
			ADVANCE_TOKEN;

			count++;

			if (!MATCH(KW_COMMA))
				break;
		}
		if (!count)
			SYNTAX_ERROR("<table name> or <alias>");

		access_type->nod_arg[0] = index_list =
			MAKE_NODE(nod_list, (SSHORT) count);
		for (ptr = &index_list->nod_arg[count]; stack;)
			*--ptr = (GPRE_NOD) POP(&stack);

		if (!EXP_match_paren())
			return NULL;
	}
	else {
		SYNTAX_ERROR("NATURAL, ORDER, or INDEX");
	}


//  generate the plan item node 

	plan_item = MAKE_NODE(nod_plan_item, 3);
	plan_item->nod_count = 2;
	plan_item->nod_arg[0] = alias_list;
	plan_item->nod_arg[1] = access_type;
	plan_item->nod_arg[2] = (GPRE_NOD) context;

	return plan_item;
}


//____________________________________________________________
//  
//		Parse a value expression.  The value could be any of the
//		following:
//  
//			"quoted string"
//			_CHARSET"quoted string"
//			123
//			+1.234E-3
//			field
//			relation.field
//			context.field
//			user defined function
//  

static GPRE_NOD par_primitive_value(
							   GPRE_REQ request,
							   BOOLEAN aster_ok,
							   USHORT * paren_count, USHORT * bool_flag)
{
	GPRE_NOD node, node_arg;
	REF reference;
	OPS *op;
	MAP map;
	USHORT distinct, local_count;
	ACT action;
	KWWORDS kw_word;

	assert_IS_REQ(request);

	node = FALSE;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	if (MATCH(KW_SELECT))
		return par_stat(request);

	if (MATCH(KW_MINUS))
		return MSC_unary(nod_negate,
						 par_primitive_value(request, FALSE, paren_count, 0));

	MATCH(KW_PLUS);

	if (MATCH(KW_USER)) {
		return MAKE_NODE(nod_user_name, 0);
	}

	if (MATCH(KW_VALUE)) {
		/* If request is NULL we must be processing a subquery - and 
		 * without the request to refer to we're kinda hosed
		 */
		if (!request)
			PAR_error("VALUE cannot be used in this context");
		action = request->req_actions;
		if (request->req_type != REQ_ddl ||
			!action ||
			!(action->act_type == ACT_create_domain ||
			  action->act_type == ACT_alter_domain))
				PAR_error("VALUE cannot be used in this context");

		return MAKE_NODE(nod_dom_value, 0);
	}

	if (MATCH(KW_LEFT_PAREN)) {
		(*paren_count)++;
		if (bool_flag && *bool_flag)
			node = SQE_boolean(request, paren_count);
		else
			node = SQE_value(request, FALSE, paren_count, bool_flag);
		EXP_match_paren();
		(*paren_count)--;
		return node;
	}

//  Check for an aggregate statistical expression.  If we already have a
//  map defined for the request, we're part of either HAVING or a trailing
//  ORDER clause.  In this case, post only the complete expression, and not
//  the sub-expressions. 

	for (op = stat_ops; (int) op->rel_kw != (int) KW_none; op++) {
		MATCH(KW_ALL);
		if (MATCH(op->rel_kw)) {
			if (request && (request->req_in_aggregate ||
							!(request->req_in_select_list ||
							  request->req_in_having_clause ||
							  request->req_in_order_by_clause)))
				/* either nested aggregate, or not part of a select
				   list, having clause, or order by clause (in any subquery)
				 */
				PAR_error("Invalid aggregate reference");

			node = MAKE_NODE(op->rel_op, 2);
			node->nod_count = 1;
			EXP_left_paren("left parenthesis in statistical function");
			distinct = MATCH(KW_DISTINCT);
			if (request) {
				map = request->req_map;
				request->req_map = NULL;
				++request->req_in_aggregate;
			}
			if (node->nod_type == nod_agg_count && MATCH(KW_ASTERISK))
				node->nod_count = 0;
			else {
				node->nod_arg[0] = SQE_value(request, FALSE, 0, 0);
				/* Disallow arrays as arguments to aggregate functions  */
				node_arg = node->nod_arg[0];
				if (node_arg && node_arg->nod_type == nod_array)
					PAR_error
						("Array columns not permitted in aggregate functions");
			}

			if (distinct)
				node->nod_arg[1] = node->nod_arg[0];
			EXP_match_paren();
			if (request) {
				if (map)
					node = post_map(node, map);
				request->req_map = map;
				--request->req_in_aggregate;
			}
			return node;
		}
	}

//  If it's a number or a quoted string, it's a literal 

	if (token.tok_type == tok_number ||
		(QUOTED(token.tok_type) && sw_sql_dialect == 1) ||
		SINGLE_QUOTED(token.tok_type)) {
		node = EXP_literal();
		return node;
	}

//  moved this timestamp support down some lines, because it caused
//  gpre to segfault when it was done here.
//  FSG 15.Nov.2000
//  


//  If the next token is a colon, it is a variable reference 

	if ((int) token.tok_keyword == (int) KW_COLON) {
		if (!request) {
			/* We must be processing a subquery - and without the request to
			 * post the :hostvar to we can't continue.
			 * (core dump when de-refer of NULL request)
			 */
			PAR_error(":hostvar reference not supported in this context");
			return NULL;
		}
		reference = SQE_variable(request, FALSE);
		node = MSC_unary(nod_value, (GPRE_NOD) reference);
		reference->ref_next = request->req_values;
		request->req_values = reference;
		return node;
	}



//  Must be a field or a udf.  If there is a map, post the field to it. 
	node = par_udf_or_field(request, aster_ok);

//  
//if (request && (map = request->req_map))
//   return post_map (node, map);
//  

	if (!node)
//  I don't know what it's good for, but let's try it anyway if we haven't found 
//  anything that makes sense until now 
	{
// ** Begin date/time/timestamp support *
		kw_word = token.tok_keyword;

		if (MATCH(KW_DATE) || MATCH(KW_TIME) || MATCH(KW_TIMESTAMP)) {
			token.tok_keyword = kw_word;
			node = EXP_literal();
			return node;
		}

// ** End date/time/timestamp support *

	}

	return node;
}


//____________________________________________________________
//  
//		Parse relational expression.
//  

static GPRE_NOD par_relational( GPRE_REQ request, USHORT * paren_count)
{
	GPRE_NOD node, expr1, expr2;
	REF ref_value;
	OPS *op;
	int negation;
	USHORT local_flag;
	NOD_T *relational_ops;

	assert_IS_REQ(request);

	local_flag = TRUE;

	negation = FALSE;
	expr1 = SQE_value(request, FALSE, paren_count, &local_flag);
	if (KEYWORD(KW_RIGHT_PAREN))
		return expr1;
	if (KEYWORD(KW_SEMI_COLON))
		for (relational_ops = relationals; *relational_ops != (NOD_T) 0;
			 relational_ops++)
			if (expr1->nod_type == *relational_ops)
				return expr1;

	if (MATCH(KW_NOT))
		negation = TRUE;

//  Check for one of the binary operators 

	if (MATCH(KW_IN))
		node = par_in(request, expr1);
	else if (MATCH(KW_BETWEEN)) {
		node = MAKE_NODE(nod_between, 3);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = SQE_value(request, FALSE, 0, 0);
		MATCH(KW_AND);
		node->nod_arg[2] = SQE_value(request, FALSE, 0, 0);
		pair(node->nod_arg[0], node->nod_arg[1]);
		pair(node->nod_arg[0], node->nod_arg[2]);
	}
	else if (MATCH(KW_LIKE)) {
		node = MAKE_NODE(nod_like, 3);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = SQE_value(request, FALSE, 0, 0);
		pair(node->nod_arg[0], node->nod_arg[1]);
		if (MATCH(KW_ESCAPE)) {
			node->nod_arg[2] = expr2 = SQE_value(request, FALSE, 0, 0);
			if (expr2->nod_type == nod_value) {
				ref_value = (REF) expr2->nod_arg[0];
				ref_value->ref_field =
					MET_make_field("like_escape_character", dtype_text, 2,
								   FALSE);
			}
		}
		else
			node->nod_count = 2;
	}
	else if (MATCH(KW_IS)) {
		if (MATCH(KW_NOT))
			negation = !negation;
		if (!MATCH(KW_NULL))
			SYNTAX_ERROR("NULL");
		if (expr1->nod_type == nod_array)
			expr1->nod_type = nod_field;
		node = MSC_unary(nod_missing, expr1);
	}
	else {
		node = NULL;
		for (op = rel_ops; (int) op->rel_kw != (int) KW_none; op++)
			if (MATCH(op->rel_kw))
				break;
		if ((int) op->rel_kw == (int) KW_none) {
			for (relational_ops = relationals; *relational_ops != (NOD_T) 0;
				 relational_ops++)

				if (expr1->nod_type == *relational_ops)
					return expr1;
			SYNTAX_ERROR("<relational operator>");
		}
		if ((int) op->rel_kw == (int) KW_STARTING)
			MATCH(KW_WITH);
		if (MATCH(KW_ANY)) {
			if (!MATCH(KW_LEFT_PAREN) || !MATCH(KW_SELECT))
				SYNTAX_ERROR("<select clause> for ANY");
			node = implicit_any(request, expr1, op->rel_op, nod_any);
			EXP_match_paren();
		}
		else if (MATCH(KW_ALL)) {
			if (!MATCH(KW_LEFT_PAREN) || !MATCH(KW_SELECT))
				SYNTAX_ERROR("<select clause> for ALL");
			if (op->rel_negation == nod_any
				|| op->rel_negation == nod_ansi_any
				|| op->rel_negation ==
				nod_ansi_all) SYNTAX_ERROR("<relational operator> for ALL");
			node = implicit_any(request, expr1, op->rel_op, nod_ansi_all);
			EXP_match_paren();
		}
		else {
			node =
				MSC_binary(op->rel_op, expr1,
						   SQE_value(request, FALSE, 0, 0));
			pair(node->nod_arg[0], node->nod_arg[1]);
		}
	}

	return (negation) ? negate(node) : node;
}


//____________________________________________________________
//  
//		Parse the SQL equivalent of a record selection expression --
//		FROM, WHERE, and ORDER clauses.  A field list may or may not
//		be present.
//  

static RSE par_rse( GPRE_REQ request, GPRE_NOD fields, BOOLEAN distinct)
{
	GPRE_CTX context;
	MAP map;
	GPRE_NOD *ptr, *end, node;
	RSE select, sub_rse;
	BOOLEAN aggregate;
	int count, i, old_count;
	LLS stack;

	assert_IS_REQ(request);
	assert_IS_NOD(fields);

//  Get list and count of relations 

	if (!MATCH(KW_FROM))
		SYNTAX_ERROR("FROM");

	stack = NULL;
	count = 0;

	do
		if (context = par_joined_relation(request, NULL)) {
			PUSH((GPRE_NOD) context, &stack);
			count++;
		}
		else
			return NULL;
	while (MATCH(KW_COMMA));

//  Now allocate a record select expression
//  block for the beast and fill in what we already know.  

	select = (RSE) ALLOC(RSE_LEN(count));
	select->rse_count = count;

	while (count--)
		select->rse_context[count] = (GPRE_CTX) POP(&stack);

//  If a field list has been presented, resolve references now 

	aggregate = FALSE;

	if (fields) {
		for (count = fields->nod_count, ptr = fields->nod_arg, i = 0;
			 i < count; i++) {
			node = *(ptr + i);
			if (node->nod_type == nod_asterisk) {
				old_count = count;
				fields = explode_asterisk(fields, i, select);
				count = fields->nod_count;
				i += count - old_count;
				ptr = fields->nod_arg;
			}
			else {
				aggregate |= SQE_resolve(node, 0, select);
				pair(node, 0);
				switch (node->nod_type) {
				case nod_agg_count:
				case nod_agg_max:
				case nod_agg_min:
				case nod_agg_average:
				case nod_agg_total:
					if ((node->nod_arg[1]) &&
						(request->req_database->dbb_flags & DBB_v3))
						select->rse_reduced =
							MSC_unary(nod_sort, node->nod_arg[1]);
					break;
				}
			}
		}
	}
	select->rse_fields = fields;
	if (distinct)
		select->rse_reduced = fields;

//  Handle a boolean, if present 

	if (MATCH(KW_WITH)) {
		++request->req_in_where_clause;
		select->rse_boolean = SQE_boolean(request, 0);
		--request->req_in_where_clause;
	}

	if (MATCH(KW_GROUP)) {
		MATCH(KW_BY);
		select->rse_group_by =
			SQE_list(reinterpret_cast < pfn_SQE_list_cb >
					 (par_udf_or_field_with_collate), request, FALSE);
		for (ptr = select->rse_group_by->nod_arg, end =
			 ptr + select->rse_group_by->nod_count; ptr < end; ptr++) {
			if ((*ptr)->nod_type == nod_array)
				PAR_error("Array columns not permitted in GROUP BY clause");
		}
	}

	if (select->rse_group_by || aggregate) {
		if (validate_references(select->rse_fields, select->rse_group_by))
			PAR_error
				("simple column reference not allowed in aggregate context");
		sub_rse = select;
		sub_rse->rse_map = map = (MAP) ALLOC(sizeof(struct map));
		if (select->rse_group_by)
			request->req_map = map;
		map->map_context = MAKE_CONTEXT(request);
		select = (RSE) ALLOC(RSE_LEN(0));
		select->rse_aggregate = sub_rse;

		if (fields)
			select->rse_fields = copy_fields(sub_rse->rse_fields, map);

		if (MATCH(KW_HAVING)) {
			++request->req_in_having_clause;
			select->rse_boolean = SQE_boolean(request, 0);
			--request->req_in_having_clause;
			if (validate_references
				(select->rse_boolean,
				 sub_rse->
				 rse_group_by))
PAR_error("simple column reference in HAVING must be referenced in GROUP BY");
		}
	}

//  parse a user-specified access plan 

	if (MATCH(KW_PLAN))
		select->rse_plan = par_plan(request);

	return select;
}


//____________________________________________________________
//  
//		Parse a SELECT (sans keyword) expression (except UNION).  This
//		is called exclusively by SQE_select, which handles unions.  Note:
//		if "union_rse" is non-null, we are a subsequent SELECT in a union.
//		In this case, check datatypes of the field against the rse field
//		list.
//  

static RSE par_select( GPRE_REQ request, RSE union_rse)
{
	RSE select;
	GPRE_NOD s_list, into_list;
	BOOLEAN distinct;

	assert_IS_REQ(request);

//  Handle the ALL and DISTINCT options 

	distinct = (!MATCH(KW_ALL) && MATCH(KW_DISTINCT)) ? TRUE : FALSE;

//  Make select list out of select items 

	++request->req_in_select_list;
	s_list = SQE_list(SQE_value_or_null, request, TRUE);
	--request->req_in_select_list;

//  If this is not a declare cursor statement and an INTO list is present,
//  parse it. 

	if (!(request->req_flags & REQ_sql_declare_cursor))
		into_list =
			(MATCH(KW_INTO)) ? SQE_list(reinterpret_cast < pfn_SQE_list_cb >
										(SQE_variable), request,
										FALSE) : NULL;
	else
		into_list = NULL;

	select = par_rse(request, s_list, distinct);
	if (select->rse_into = into_list)
		select->rse_flags |= RSE_singleton;

	if (union_rse && s_list->nod_count != union_rse->rse_fields->nod_count)
		PAR_error("select lists for UNION don't match");

	return select;
}


//____________________________________________________________
//  
//		Parse a dumb SQL scalar statistical expression.  Somebody else
//		has already eaten the SELECT on the front.
//  

static GPRE_NOD par_stat( GPRE_REQ request)
{
	GPRE_NOD node, field_list;
	GPRE_NOD item;
	RSE select;
	BOOLEAN distinct;
	struct scope previous_scope;

	assert_IS_REQ(request);

	request->req_in_subselect++;
	push_scope(request, &previous_scope);

	distinct = (!MATCH(KW_ALL) && MATCH(KW_DISTINCT)) ? TRUE : FALSE;

	request->req_in_select_list++;
	if (!(item = par_udf(request)))
		item = SQE_value(request, FALSE, 0, 0);
	request->req_in_select_list--;

	field_list = MAKE_NODE(nod_list, 1);
	field_list->nod_arg[0] = item;
	select = par_rse(request, field_list, distinct);
	select->rse_flags |= RSE_singleton;

	item = select->rse_fields->nod_arg[0];

	node = MAKE_NODE(nod_via, 3);
	node->nod_count = 0;
	node->nod_arg[0] = (GPRE_NOD) select;
	node->nod_arg[2] = MAKE_NODE(nod_null, 0);
	node->nod_arg[1] = item;

	EXP_rse_cleanup(select);
	pop_scope(request, &previous_scope);
	request->req_in_subselect--;

	return node;
}


//____________________________________________________________
//  
//       Parse a subscript value.  
//  

static GPRE_NOD par_subscript( GPRE_REQ request)
{
	GPRE_NOD node;
	REF reference;
	SCHAR *string;

	assert_IS_REQ(request);

	reference = (REF) ALLOC(REF_LEN);
	node = MSC_unary(nod_value, (GPRE_NOD) reference);

//  Special case literals 

	if (token.tok_type == tok_number) {
		node->nod_type = nod_literal;
		reference->ref_value = string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		ADVANCE_TOKEN;
		return node;
	}

	if (!MATCH(KW_COLON))
		SYNTAX_ERROR("<colon>");

	reference->ref_value = PAR_native_value(FALSE, FALSE);

	if (request) {
		reference->ref_next = request->req_values;
		request->req_values = reference;
	}

	return node;
}


//____________________________________________________________
//  
//		Match several trailing parentheses.
//  

static void par_terminating_parens(
								   USHORT * paren_count, USHORT * local_count)
{

	if (*paren_count && paren_count == local_count)
		do
			EXP_match_paren();
		while (--(*paren_count));
}


//____________________________________________________________
//  
//		Parse a user defined function.  If the current token isn't one,
//		return NULL.  Otherwise try to parse one.  If things go badly,
//		complain bitterly.
//  

static GPRE_NOD par_udf( GPRE_REQ request)
{
	GPRE_NOD node;
	GPRE_NOD *input;
	UDF udf, tmp_udf;
	USHORT local_count;
	GPRE_FLD field;
	SCHAR s[ERROR_LENGTH];
	DBB db;
	TEXT *gen_name;

	if (!request)
		return NULL;

	assert_IS_REQ(request);

//  Check for user defined functions 
// ** resolve only if an identifier *
	if ((QUOTED(token.tok_type)) || token.tok_type == tok_ident)
		SQL_resolve_identifier("<Udf Name>", s);
	if (request->req_database)
		udf = MET_get_udf(request->req_database, token.tok_string);
	else {
		/* no database was specified, check the metadata for all the databases
		   for the existence of the udf */

		udf = NULL;
		for (db = isc_databases; db; db = db->dbb_next)
			if (tmp_udf = MET_get_udf(db, token.tok_string))
				if (udf) {
					/* udf was found in more than one database */
					sprintf(s, "UDF %s is ambiguous", token.tok_string);
					PAR_error(s);
				}
				else {
					udf = tmp_udf;
					request->req_database = db;
				}
	}

	if (udf) {
		if ((SQL_DIALECT_V5 == sw_sql_dialect) &&
			((dtype_sql_date == udf->udf_dtype) ||
			 (dtype_sql_time == udf->udf_dtype) ||
			 (dtype_int64 == udf->udf_dtype)))
				dialect1_bad_type(udf->udf_dtype);

		node = MAKE_NODE(nod_udf, 2);
		node->nod_count = 1;
		node->nod_arg[1] = (GPRE_NOD) udf;
		ADVANCE_TOKEN;
		EXP_left_paren(0);
		if (!(token.tok_keyword == KW_RIGHT_PAREN)) {
			/* parse udf parameter references */
			node->nod_arg[0] = SQE_list(SQE_value, request, FALSE);

			if (udf->udf_args != node->nod_arg[0]->nod_count)
				PAR_error("count of UDF parameters doesn't match definition");

			/* Match parameter types to the declared parameters */
			for (input = node->nod_arg[0]->nod_arg,
				 field = udf->udf_inputs;
				 field;
				 input++, field = field->fld_next)
			 SQE_post_field(*input, field);
		}
		else {
			node->nod_arg[0] = (GPRE_NOD) 0;
			node->nod_count = 0;
		}
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

	if (!request)
		return NULL;

//  Check for GEN_ID () 
	if (MATCH(KW_GEN_ID)) {
		gen_name = (TEXT *) ALLOC(NAME_SIZE);
		node = MAKE_NODE(nod_gen_id, 2);
		node->nod_count = 1;
		EXP_left_paren(0);
		SQL_resolve_identifier("<Generator Name>", gen_name);
		node->nod_arg[1] = (GPRE_NOD) gen_name;
		ADVANCE_TOKEN;
		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR("<comma>");
		node->nod_arg[0] = SQE_value(request, FALSE, 0, 0);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

//  Check for DATE constants 
// ** Begin date/time/timesamp *
	if (MATCH(KW_CURRENT_DATE))
		return MAKE_NODE(nod_current_date, 0);
	else if (MATCH(KW_CURRENT_TIME))
		return MAKE_NODE(nod_current_time, 0);
	else if (MATCH(KW_CURRENT_TIMESTAMP))
		return MAKE_NODE(nod_current_timestamp, 0);

//  End date/time/timesamp *

//  Check for SQL II defined functions 

// ** Begin date/time/timesamp *
	if (MATCH(KW_EXTRACT)) {
		KWWORDS kw_word;
		node = MAKE_NODE(nod_extract, 2);
		EXP_left_paren(0);
		kw_word = token.tok_keyword;
		if (MATCH(KW_YEAR) || MATCH(KW_MONTH) || MATCH(KW_DAY) ||
			MATCH(KW_HOUR) || MATCH(KW_MINUTE) || MATCH(KW_SECOND) ||
			MATCH(KW_WEEKDAY) || MATCH(KW_YEARDAY)) {
			node->nod_arg[0] = (struct gpre_nod *) kw_word;
			if (!MATCH(KW_FROM))
				SYNTAX_ERROR("FROM");
		}
		else
			SYNTAX_ERROR("valid extract part");
		node->nod_arg[1] = SQE_value(request, FALSE, 0, 0);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

//  End date/time/timesamp *

	if (MATCH(KW_UPPER)) {
		node = MAKE_NODE(nod_upcase, 1);
		EXP_left_paren(0);
		node->nod_arg[0] = SQE_value(request, FALSE, 0, 0);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

	if (MATCH(KW_CAST)) {
		node = MAKE_NODE(nod_cast, 2);
		node->nod_count = 1;
		EXP_left_paren(0);
		node->nod_arg[0] = SQE_value_or_null(request, FALSE, 0, 0);
		if (!MATCH(KW_AS))
			SYNTAX_ERROR("AS");
		field = (GPRE_FLD) ALLOC(FLD_LEN);
		node->nod_arg[1] = (GPRE_NOD) field;
		SQL_par_field_dtype(request, field, FALSE);
		SQL_par_field_collate(request, field);
		SQL_adjust_field_dtype(field);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

	return NULL;
}


//____________________________________________________________
//  
//		Parse a user defined function or a field name.
//  

static GPRE_NOD par_udf_or_field( GPRE_REQ request, BOOLEAN aster_ok)
{
	GPRE_NOD node;

	assert_IS_REQ(request);

	if (!(node = par_udf(request)))
		node = SQE_field(request, aster_ok);

	return node;
}


//____________________________________________________________
//  
//		Parse a user defined function or a field name.
//		Allow the collate clause to follow.
//  

static GPRE_NOD par_udf_or_field_with_collate( GPRE_REQ request, BOOLEAN aster_ok)
{
	GPRE_NOD node;

	assert_IS_REQ(request);

	node = par_udf_or_field(request, aster_ok);
	if (KEYWORD(KW_COLLATE))
		node = par_collate(request, node);

	return node;
}


//____________________________________________________________
//  
//		Post a field or aggregate to a map.  This is used to references
//		to aggregates and unions.  Return a reference to the map (rather
//		than the expression itself).  Post only the aggregates and fields,
//		not the computations around them.
//  

static GPRE_NOD post_fields( GPRE_NOD node, MAP map)
{
	GPRE_NOD *ptr, *end;

	assert_IS_NOD(node);

	switch (node->nod_type) {
		/* Removed during fix to BUG_8021 - this would post a literal to
		 * the map record for each literal used in an expression - which 
		 * would result in unneccesary data movement as the literal is more
		 * easily experssed in the assignment portion of the mapping select
		 * operation.
		 case nod_literal:
		 * 1995-Jul-10 David Schnepper 
		 */
	case nod_field:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_average:
	case nod_agg_total:
	case nod_agg_count:
	case nod_map_ref:
		return post_map(node, map);

	case nod_udf:
	case nod_gen_id:
		node->nod_arg[0] = post_fields(node->nod_arg[0], map);
		break;

	case nod_list:
	case nod_upcase:
	case nod_concatenate:
	case nod_cast:
	case nod_plus:
	case nod_minus:
	case nod_times:
	case nod_divide:
	case nod_negate:
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
			*ptr = post_fields(*ptr, map);
		break;
// ** Begin date/time/timestamp support *
	case nod_extract:
		node->nod_arg[1] = post_fields(node->nod_arg[1], map);
		break;
// ** End date/time/timestamp support *
	}

	return node;
}


//____________________________________________________________
//  
//		Post a value expression to a map.  This is used to references
//		to aggregates and unions.  Return a reference to the map (rather
//		than the expression itself).
//  

static GPRE_NOD post_map( GPRE_NOD node, MAP map)
{
	MEL element;

	assert_IS_NOD(node);

//  Search existing map for equivalent expression.  If we find one,
//  return a reference to it. 

	if (node->nod_type == nod_map_ref) {
		element = (MEL) node->nod_arg[0];
		if (element->mel_context == map->map_context)
			return node;
	}

	for (element = map->map_elements; element; element = element->mel_next)
		if (compare_expr(node, element->mel_expr))
			return MSC_unary(nod_map_ref, (GPRE_NOD) element);

//  We need to make up a new map reference 

	element = (MEL) ALLOC(sizeof(struct mel));
	element->mel_next = map->map_elements;
	map->map_elements = element;
	element->mel_position = map->map_count++;
	element->mel_expr = node;
	element->mel_context = map->map_context;

//  Make up a reference to the map element 

	return MSC_unary(nod_map_ref, (GPRE_NOD) element);
}


//____________________________________________________________
//  
//		Copy a selection list to the map generated for a UNION
//		construct.  Note at this level we want the full expression
//		selected posted, not just the portions that come from the
//		stream.  Thus CAST and other operations will be passed into
//		a UNION.  See BUG_8021 & BUG_8000 for examples.
//  

static GPRE_NOD post_select_list( GPRE_NOD fields, MAP map)
{
	GPRE_NOD list;
	USHORT i;

	assert_IS_NOD(fields);

	list = MAKE_NODE(nod_list, fields->nod_count);

	for (i = 0; i < fields->nod_count; i++)
		list->nod_arg[i] = post_map(fields->nod_arg[i], map);

	return list;
}


//____________________________________________________________
//  
//		Restore saved scoping information to the request block
//  

static void pop_scope( GPRE_REQ request, struct scope *save_scope)
{
	assert_IS_REQ(request);

	request->req_contexts = save_scope->req_contexts;
	request->req_scope_level = save_scope->req_scope_level;
	request->req_in_aggregate = save_scope->req_in_aggregate;
	request->req_in_select_list = save_scope->req_in_select_list;
	request->req_in_where_clause = save_scope->req_in_where_clause;
	request->req_in_having_clause = save_scope->req_in_having_clause;
	request->req_in_order_by_clause = save_scope->req_in_order_by_clause;
}


//____________________________________________________________
//  
//		Save scoping information from the request block
//  

static void push_scope( GPRE_REQ request, struct scope *save_scope)
{
	assert_IS_REQ(request);

	save_scope->req_contexts = request->req_contexts;
	save_scope->req_scope_level = request->req_scope_level;
	save_scope->req_in_aggregate = request->req_in_aggregate;
	save_scope->req_in_select_list = request->req_in_select_list;
	save_scope->req_in_where_clause = request->req_in_where_clause;
	save_scope->req_in_having_clause = request->req_in_having_clause;
	save_scope->req_in_order_by_clause = request->req_in_order_by_clause;
	save_scope->req_in_subselect = request->req_in_subselect;
	request->req_scope_level++;
	request->req_in_aggregate = 0;
	request->req_in_select_list = 0;
	request->req_in_where_clause = 0;
	request->req_in_having_clause = 0;
	request->req_in_order_by_clause = 0;
	request->req_in_subselect = 0;
}


//____________________________________________________________
//  
//		Attempt to resolve a field in a context.  If successful, return
//		the field.  Otherwise return NULL.  Let somebody else worry about
//		errors.
//  

static GPRE_FLD resolve(
				   GPRE_NOD node,
				   GPRE_CTX context, GPRE_CTX * found_context, ACT * slice_action)
{
	SYM symbol, temp_symbol;
	GPRE_FLD field;
	TOK f_token, q_token;
	RSE rs_stream;
	SSHORT i;
	GPRE_REQ slice_req;
	SLC slice;
	ARY ary_info;
	ACT action;

	assert_IS_NOD(node);

	if (rs_stream = context->ctx_stream) {
		for (i = 0; i < rs_stream->rse_count; i++)
			if (field =
				resolve(node, rs_stream->rse_context[i], found_context,
						slice_action)) return field;

		return NULL;
	}

	f_token = (TOK) node->nod_arg[0];
	q_token = (TOK) node->nod_arg[1];

	if (!(context->ctx_relation || context->ctx_procedure))
		return NULL;

//  Handle unqualified fields first for simplicity 

	if (!q_token)
		field = MET_context_field(context, f_token->tok_string);
	else {
		/* Now search alternatives for the qualifier */

		symbol = HSH_lookup(q_token->tok_string);

		/* This caused gpre to dump core if there are lower case 
		   table aliases in a where clause used with dialect 2 or 3 

		   if ( (symbol == NULL)&& (sw_case || sw_sql_dialect == SQL_DIALECT_V5))
		   symbol = HSH_lookup2 (q_token->tok_string);
		 */

		/* So I replaced it with the following, don't know
		   why we don't do a HSH_lookup2 in any case, but so it may be.
		   FSG 16.Nov.2000
		 */
		if ((symbol == NULL))
			symbol = HSH_lookup2(q_token->tok_string);


		for (temp_symbol = symbol; temp_symbol;
			 temp_symbol = temp_symbol->sym_homonym) {
			if (temp_symbol->sym_type == SYM_context) {
				symbol = temp_symbol;
				break;
			}
			else if (temp_symbol->sym_type == SYM_relation) {
				symbol = temp_symbol;
				continue;
			}
			else if (temp_symbol->sym_type == SYM_procedure) {
				if (symbol->sym_type == SYM_relation)
					continue;
				else
					symbol = temp_symbol;
			}
		}


		field = NULL;
		if (symbol->sym_type == SYM_relation) {
			if ((GPRE_REL) symbol->sym_object == context->ctx_relation)
				field = MET_field(context->ctx_relation, f_token->tok_string);
		}
		else if (symbol->sym_type == SYM_procedure) {
			if ((GPRE_PRC) symbol->sym_object == context->ctx_procedure)
				field = MET_context_field(context, f_token->tok_string);
		}
		else if (symbol->sym_type == SYM_context
				 && symbol->sym_object == context) field =
				MET_context_field(context, f_token->tok_string);
	}


	if (field && found_context)
		*found_context = context;

//  Check for valid array field  
//  Check dimensions 
//  Set remaining fields for slice 
	if ((slice_req = (GPRE_REQ) node->nod_arg[2]) &&
		(slice = slice_req->req_slice) && slice_action) {
		slice = slice_req->req_slice;
		if (!(ary_info = field->fld_array_info))
			SYNTAX_ERROR("<array column>");
		if (ary_info->ary_dimension_count != slice->slc_dimensions)
			PAR_error("subscript count mismatch");
		slice->slc_field = field;
		slice->slc_parent_request = context->ctx_request;

		/* The action type maybe ACT_get_slice or ACT_put_slice 
		   set as a place holder */

		action = MAKE_ACTION(slice_req, ACT_get_slice);
		action->act_object = (REF) slice;
		*slice_action = action;
	}
	else if ((slice_req = (GPRE_REQ) node->nod_arg[2]) && slice_action) {
		/* The action type maybe ACT_get_slice or ACT_put_slice
		   set as a place holder */

		action = MAKE_ACTION(slice_req, ACT_get_slice);
		*slice_action = action;
	}

	return field;
}


//____________________________________________________________
//  
//		Attempt to resolve an asterisk in a context.
//		If successful, return the context.  Otherwise return NULL.
//  

static GPRE_CTX resolve_asterisk( TOK q_token, RSE rse)
{
	GPRE_CTX context;
	RSE rs_stream;
	SYM symbol;
	int i;

	for (i = 0; i < rse->rse_count; i++) {
		context = rse->rse_context[i];
		if (rs_stream = context->ctx_stream) {
			if (context = resolve_asterisk(q_token, rs_stream))
				return context;
			continue;
		}
		symbol = HSH_lookup(q_token->tok_string);
		for (; symbol; symbol = symbol->sym_homonym)
			if (symbol->sym_type == SYM_relation &&
				(GPRE_REL) symbol->sym_object == context->ctx_relation)
				return context;
			else if (symbol->sym_type == SYM_procedure &&
					 (GPRE_PRC) symbol->sym_object == context->ctx_procedure)
				return context;
			else if (symbol->sym_type == SYM_context &&
					 (GPRE_CTX) symbol->sym_object == context)
				return context;
	}

	return NULL;
}


//____________________________________________________________
//  
//		Set field reference for any host variables in expr to field_ref.
//  

static void set_ref( GPRE_NOD expr, GPRE_FLD field_ref)
{
	GPRE_NOD *ptr, *end;
	REF ref;

	assert_IS_NOD(expr);

	ref = (REF) expr->nod_arg[0];
	switch (expr->nod_type) {
	case nod_value:
		ref->ref_field = field_ref;
		break;

	case nod_agg_count:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
	case nod_agg_average:
	case nod_plus:
	case nod_minus:
	case nod_times:
	case nod_divide:
	case nod_negate:
	case nod_upcase:
	case nod_concatenate:
	case nod_cast:
		for (ptr = expr->nod_arg, end = ptr + expr->nod_count; ptr < end;
			 ptr++)
			set_ref(*ptr, field_ref);
		break;
// ** Begin date/time/timestamp support *
	case nod_extract:
		set_ref(expr->nod_arg[1], field_ref);
		break;
// ** End date/time/timestamp support *
	}
}


//____________________________________________________________
//  
//		Return the uppercase version of
//		the input string.
//  

static char *upcase_string( char *p)
{
	char c, *q, *s;
	USHORT l;

	l = 0;
	s = (char *) ALLOC(strlen(p) + 1);
	q = s;

	while ((c = *p++) && (++l <= NAME_SIZE)) {
		*q++ = UPPER7(c);
	}
	*q = 0;

	return s;
}


//____________________________________________________________
//  
//		validate that top level field references
//		in a select with a group by, real or imagined,
//		resolve to grouping fields.  Ignore constants
//		and aggregates.	  If there's no group_by list,
//		then it's an imaginary group by (a top level
//		aggregation, and nothing can be referenced
//		directly.	
//  

static BOOLEAN validate_references( GPRE_NOD fields, GPRE_NOD group_by)
{
	GPRE_NOD *ptr, *end, node;
	REF fref, gref;
	RSE any;
	BOOLEAN invalid, context_match;
	MEL element;

	assert_IS_NOD(fields);
	assert_IS_NOD(group_by);

	if (!fields)
		return FALSE;

	invalid = FALSE;
	if (fields->nod_type == nod_field) {
		if (!group_by)
			return TRUE;
		fref = (REF) fields->nod_arg[0];

		context_match = FALSE;
		for (ptr = group_by->nod_arg, end = ptr + group_by->nod_count;
			 ptr < end; ptr++) {
			gref = (REF) (*ptr)->nod_arg[0];
			if (gref->ref_context == fref->ref_context) {
				if (gref->ref_field == fref->ref_field)
					return FALSE;
				context_match = TRUE;
			}
		}
		return context_match;
	}

	if (fields->nod_type == nod_agg_count ||
		fields->nod_type == nod_agg_max ||
		fields->nod_type == nod_agg_min ||
		fields->nod_type == nod_agg_total ||
		fields->nod_type == nod_agg_average ||
		fields->nod_type == nod_aggregate) return FALSE;

	if (fields->nod_type == nod_any || fields->nod_type == nod_ansi_any ||
		fields->nod_type == nod_ansi_all) {
		any = (RSE) fields->nod_arg[0];
		return validate_references(any->rse_boolean, group_by);
	}

	if ((fields->nod_type == nod_gen_id) || (fields->nod_type == nod_udf))
		return validate_references(fields->nod_arg[0], group_by);

	for (ptr = fields->nod_arg, end = ptr + fields->nod_count; ptr < end;
		 ptr++)
		switch ((*ptr)->nod_type) {
		case nod_map_ref:
			element = (MEL) (*ptr)->nod_arg[0];
			node = element->mel_expr;
			if (node->nod_type != nod_agg_count &&
				node->nod_type != nod_agg_max &&
				node->nod_type != nod_agg_min &&
				node->nod_type != nod_agg_total &&
				node->nod_type != nod_agg_average &&
				node->nod_type != nod_aggregate)
					invalid |= validate_references(node, group_by);
			break;

		case nod_field:
		case nod_plus:
		case nod_minus:
		case nod_times:
		case nod_negate:
		case nod_divide:
		case nod_and:
		case nod_like:
		case nod_missing:
		case nod_not:
		case nod_or:
		case nod_eq:
		case nod_ne:
		case nod_gt:
		case nod_ge:
		case nod_le:
		case nod_lt:
		case nod_upcase:
		case nod_concatenate:
		case nod_cast:
			invalid |= validate_references(*ptr, group_by);
			break;
		}

	return invalid;
}


static void dialect1_bad_type(USHORT field_dtype)
{
	char buffer[200];
	char *s;

	switch (field_dtype) {
	case dtype_sql_date:
		s = "SQL DATE";
		break;
	case dtype_sql_time:
		s = "SQL TIME";
		break;
	case dtype_int64:
		s = "64-bit numeric";
		break;
	}
	sprintf(buffer,
			"Client SQL dialect 1 does not support reference to the %s datatype",
			s);
	PAR_error(buffer);
}


} // extern "C"
