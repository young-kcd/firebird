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


struct scope {
	gpre_ctx* req_contexts;
	USHORT req_scope_level;		// scope level for SQL subquery parsing 
	USHORT req_in_aggregate;	// now processing value expr for aggr 
	USHORT req_in_select_list;	// processing select list 
	USHORT req_in_where_clause;	// processing where clause 
	USHORT req_in_having_clause;	// processing having clause 
	USHORT req_in_order_by_clause;	// processing order by clause 
	USHORT req_in_subselect;	// processing a subselect clause 
};

static bool compare_expr(GPRE_NOD, GPRE_NOD);
static GPRE_NOD copy_fields(GPRE_NOD, map*);
static GPRE_NOD explode_asterisk(GPRE_NOD, int, gpre_rse*);
static GPRE_NOD explode_asterisk_all(GPRE_NOD, int, gpre_rse*, bool);
static gpre_fld* get_ref(GPRE_NOD);
static GPRE_NOD implicit_any(gpre_req*, GPRE_NOD, enum nod_t, enum nod_t);
static GPRE_NOD merge(GPRE_NOD, GPRE_NOD);
static GPRE_NOD merge_fields(GPRE_NOD, GPRE_NOD, int, bool);
static GPRE_NOD negate(GPRE_NOD);
static void pair(GPRE_NOD, GPRE_NOD);
static gpre_ctx* par_alias_list(gpre_req*, GPRE_NOD);
static gpre_ctx* par_alias(gpre_req*, const TEXT*);
static GPRE_NOD par_and(gpre_req*, USHORT *);
static gpre_rel* par_base_table(gpre_req*, const gpre_rel*, const TEXT*);
static GPRE_NOD par_collate(gpre_req*, GPRE_NOD);
static GPRE_NOD par_in(gpre_req*, GPRE_NOD);
static gpre_ctx* par_joined_relation(gpre_req*, gpre_ctx*);
static gpre_ctx* par_join_clause(gpre_req*, gpre_ctx*);
static NOD_T par_join_type(void);
static GPRE_NOD par_multiply(gpre_req*, bool, USHORT *, bool *);
static GPRE_NOD par_not(gpre_req*, USHORT *);
static void par_order(gpre_req*, gpre_rse*, bool, bool);
static GPRE_NOD par_plan(gpre_req*);
static GPRE_NOD par_plan_item(gpre_req*, bool, USHORT *, bool *);
static GPRE_NOD par_primitive_value(gpre_req*, bool, USHORT *, bool *);
static GPRE_NOD par_relational(gpre_req*, USHORT *);
static gpre_rse* par_rse(gpre_req*, GPRE_NOD, bool);
static gpre_rse* par_select(gpre_req*, gpre_rse*);
static GPRE_NOD par_stat(gpre_req*);
static GPRE_NOD par_subscript(gpre_req*);
static void par_terminating_parens(USHORT *, USHORT *);
static GPRE_NOD par_udf(gpre_req*);
static GPRE_NOD par_udf_or_field(gpre_req*, bool);
static GPRE_NOD par_udf_or_field_with_collate(gpre_req*, bool, USHORT *, bool *);
static GPRE_NOD post_fields(GPRE_NOD, map*);
static GPRE_NOD post_map(GPRE_NOD, MAP);
static GPRE_NOD post_select_list(GPRE_NOD, map*);
static void pop_scope(gpre_req*, scope*);
static void push_scope(gpre_req*, scope*);
static gpre_fld* resolve(GPRE_NOD, gpre_ctx*, gpre_ctx**, act**);
static gpre_ctx* resolve_asterisk(TOK, gpre_rse*);
static void set_ref(GPRE_NOD, gpre_fld*);
static char* upcase_string(const char*);
static bool validate_references(GPRE_NOD, GPRE_NOD);




struct ops {
	enum nod_t rel_op;
	enum kwwords rel_kw;
	enum nod_t rel_negation;
};

static const ops rel_ops[] =
{
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
	{ nod_ansi_all, KW_none, nod_ansi_all }
};

#ifdef NOT_USED_OR_REPLACED
static const ops scalar_stat_ops[] = {
	{ nod_count, KW_COUNT, nod_any },
	{ nod_max, KW_MAX, nod_any },
	{ nod_min, KW_MIN, nod_any },
	{ nod_total, KW_TOTAL, nod_any },
	{ nod_total, KW_SUM, nod_any },
	{ nod_average, KW_AVERAGE, nod_any },
	{ nod_via, KW_none, nod_any}
};
#endif

static const ops stat_ops[] = {
	{ nod_agg_count, KW_COUNT, nod_any },
	{ nod_agg_max, KW_MAX, nod_any },
	{ nod_agg_min, KW_MIN, nod_any },
	{ nod_agg_total, KW_TOTAL, nod_any },
	{ nod_agg_total, KW_SUM, nod_any },
	{ nod_agg_average, KW_AVERAGE, nod_any },
	{ nod_any, KW_none, nod_any },
	{ nod_ansi_any, KW_none, nod_ansi_any },
	{ nod_ansi_all, KW_none, nod_ansi_all }
};

static const NOD_T relationals[] = {
	nod_eq, nod_ne, nod_gt, nod_ge, nod_le, nod_lt, nod_containing,
	nod_starting, nod_matches, nod_any, nod_missing, nod_between, nod_like,
	nod_and, nod_or, nod_not, nod_ansi_any, nod_ansi_all, (NOD_T) 0
};


//____________________________________________________________
//  
//		Parse an OR boolean expression.
//  

GPRE_NOD SQE_boolean( gpre_req* request, USHORT * paren_count)
{
	USHORT local_count;

	assert_IS_REQ(request);

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	gpre_nod* expr1 = par_and(request, paren_count);

	if (!MSC_match(KW_OR) && !MSC_match(KW_OR1)) {
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

gpre_ctx* SQE_context(gpre_req* request)
{
	SCHAR r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1];
	SCHAR s[ERROR_LENGTH];

	assert_IS_REQ(request);

	gpre_ctx* context = MSC_context(request);
	SQL_relation_name(r_name, db_name, owner_name);

	if (!(context->ctx_relation =
		  SQL_relation(request, r_name, db_name, owner_name, false)))
	{
		// check for a procedure 
		gpre_prc* procedure = context->ctx_procedure =
			SQL_procedure(request, r_name, db_name, owner_name, false);
		if (procedure)
		{
			if (procedure->prc_inputs) {
				if (!MSC_match(KW_LEFT_PAREN))
					CPR_s_error("( <procedure input parameters> )");
				// parse input references 
				context->ctx_prc_inputs = SQE_list(SQE_value, request, false);
				USHORT local_count = 1;
				par_terminating_parens(&local_count, &local_count);
				if (procedure->prc_in_count !=
					context->ctx_prc_inputs->nod_count)
				{
					PAR_error("count of input values doesn't match count of parameters");
				}
				gpre_nod** input = context->ctx_prc_inputs->nod_arg;
				for (gpre_fld* field = procedure->prc_inputs; field;
					input++, field = field->fld_next)
				{
					SQE_post_field(*input, field);
				}
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

	gpre_sym* symbol = gpreGlob.token_global.tok_symbol;
	if (symbol && symbol->sym_type == SYM_keyword) {
		if (!gpreGlob.token_global.tok_length)
			CPR_token();
		return context;
	}

//  we have what we assume to be an alias; check to make sure that 
//  it does not conflict with any relation, procedure or context names
//  at the same scoping level in this query 

	gpre_ctx* conflict;
	for (conflict = request->req_contexts; conflict;
		 conflict = conflict->ctx_next)
	{
		if ((symbol = conflict->ctx_symbol) 
			&& (symbol->sym_type == SYM_relation
				|| symbol->sym_type == SYM_context
				|| symbol->sym_type == SYM_procedure)
			&& (!strcmp(symbol->sym_string, gpreGlob.token_global.tok_string))
			&& (conflict->ctx_scope_level == request-> req_scope_level))
		{
			break;
		}
	}

	if (conflict) {
		const char* error_type;
		if (symbol->sym_type == SYM_relation)
			error_type = "table";
		else if (symbol->sym_type == SYM_procedure)
			error_type = "procedure";
		else
			error_type = "context";

		sprintf(s, "context %s conflicts with a %s in the same statement",
				gpreGlob.token_global.tok_string, error_type);
		PAR_error(s);
	}

	symbol = MSC_symbol(SYM_context, gpreGlob.token_global.tok_string, gpreGlob.token_global.tok_length, 0);
	symbol->sym_object = context;
	context->ctx_symbol = symbol;
	context->ctx_alias = symbol->sym_name;
	HSH_insert(symbol);

	PAR_get_token();

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

GPRE_NOD SQE_field(gpre_req* request,
				   bool aster_ok)
{
	GPRE_NOD node;
	gpre_ctx* context;
	int count = 0;
	gpre_req* slice_req;
	TEXT s[ERROR_LENGTH];

	assert_IS_REQ(request);

	gpre_lls* upper_dim = NULL;
	gpre_lls* lower_dim = NULL;
	tok hold_token;
	hold_token.tok_type = tok_t(0);

	if (aster_ok && MSC_match(KW_ASTERISK)) {
		node = MSC_node(nod_asterisk, 1);
		return node;
	}

//  if the token isn't an identifier, complain 

	SQL_resolve_identifier("<column name>", s);

//  For domains we can't be resolving tokens to field names
//  in the CHECK constraint. 

	act* action;
	if (request &&
		request->req_type == REQ_ddl &&
		(action = request->req_actions) &&
		(action->act_type == ACT_create_domain ||
		 action->act_type == ACT_alter_domain))
	{
		sprintf(s, "Illegal use of identifier: %s in domain constraint",
				gpreGlob.token_global.tok_string);
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
		node = MSC_node(nod_defered, 3);
		node->nod_count = 0;
		TOK f_token = (TOK) MSC_alloc(TOK_LEN);
		node->nod_arg[0] = (GPRE_NOD) f_token;
		f_token->tok_length = gpreGlob.token_global.tok_length;
		SQL_resolve_identifier("<identifier>", f_token->tok_string);
		CPR_token();

		if (MSC_match(KW_DOT)) {
			if ((int) gpreGlob.token_global.tok_keyword == (int) KW_ASTERISK) {
				if (aster_ok)
					node->nod_type = nod_asterisk;
				else
					PAR_error("* not allowed");
			}
			else {
				node->nod_arg[1] = node->nod_arg[0];
				f_token = (TOK) MSC_alloc(TOK_LEN);
				node->nod_arg[0] = (GPRE_NOD) f_token;
				f_token->tok_length = gpreGlob.token_global.tok_length;
				SQL_resolve_identifier("<identifier>", f_token->tok_string);
			}
			CPR_token();
		}
		if (MSC_match(KW_L_BRCKET)) {
			// We have a complete array or an array slice here 

			if (!MSC_match(KW_R_BRCKET)) {
				slice_req = MSC_request(REQ_slice);
				do {
					count++;
					gpre_nod* tail = par_subscript(slice_req);
					MSC_push(tail, &lower_dim);
					if (MSC_match(KW_COLON)) {
						// if (!MSC_match (KW_DOT))
						// CPR_s_error ("<period>");
						tail = par_subscript(slice_req);
						MSC_push(tail, &upper_dim);
					}
					else
						MSC_push(tail, &upper_dim);
				} while (MSC_match(KW_COMMA));

				if (!MSC_match(KW_R_BRCKET))
					CPR_s_error("<right bracket>");
				slc* slice = (slc*) MSC_alloc(SLC_LEN(count));
				slice_req->req_slice = slice;
				slc::slc_repeat* tail_ptr = &slice->slc_rpt[count];
				slice->slc_dimensions = count;
				slice->slc_parent_request = request;
				while (lower_dim) {
					--tail_ptr;
					tail_ptr->slc_lower = MSC_pop(&lower_dim);
					tail_ptr->slc_upper = MSC_pop(&upper_dim);
				}
				node->nod_arg[2] = (GPRE_NOD) slice_req;

				/* added this to assign the correct nod_count 
				   The nod type is converted to nod_field in SQE_resolve()
				   The nod_count is check to confirm if the array slice
				   has been initialized in cmd.cpp
				 */
				node->nod_count = 3;
			}
			else {
				slice_req = (gpre_req*) MSC_alloc(REQ_LEN);
				slice_req->req_type = REQ_slice;
				node->nod_arg[2] = (GPRE_NOD) slice_req;
			}
		}

		return node;
	}

	ref* reference = (REF) MSC_alloc(REF_LEN);
	node = MSC_unary(nod_field, (GPRE_NOD) reference);

	gpre_sym* symbol = gpreGlob.token_global.tok_symbol;
	if (symbol) {
		/* if there is a homonym which is a context, use the context;
		   otherwise we may match with a relation or procedure which 
		   is not in the request, resulting in a bogus error */

		if (symbol->sym_type != SYM_field) {
			for (gpre_sym* temp_symbol = symbol; temp_symbol;
				 temp_symbol = temp_symbol->sym_homonym)
			{
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
				if (!MSC_match(KW_DOT))
					CPR_s_error("<period> in qualified column");
				if (context->ctx_request != request)
					PAR_error("context not part of this request");
				SQL_resolve_identifier("<Column Name>", s);
				if (!
					(reference->ref_field =
					 MET_context_field(context, gpreGlob.token_global.tok_string)))
				{
					sprintf(s, "column \"%s\" not in context",
							gpreGlob.token_global.tok_string);
					PAR_error(s);
				}
				if (SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) {
					const USHORT field_dtype = reference->ref_field->fld_dtype;
					if ((dtype_sql_date == field_dtype) ||
						(dtype_sql_time == field_dtype) ||
						(dtype_int64 == field_dtype))
					{
						SQL_dialect1_bad_type(field_dtype);
					}
				}
				reference->ref_context = context;
				CPR_token();
				return node;
			}
			else if (symbol->sym_type == SYM_relation) {
				const gpre_rel* relation = (gpre_rel*) symbol->sym_object;
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

				if (!MSC_match(KW_DOT)) {
					hold_token = gpreGlob.token_global;
					gpreGlob.token_global = gpreGlob.prior_token;
					gpreGlob.token_global.tok_symbol = 0;
				}
				else {
		/** We've got the column name. resolve it **/
					SQL_resolve_identifier("<Columnn Name>", s);
					for (context = request->req_contexts; context;
						 context = context->ctx_next)
					{
						if (context->ctx_relation == relation &&
								(reference->ref_field =
								 MET_field(context->ctx_relation,
										   gpreGlob.token_global.tok_string)))
						{
							if (SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) {
								const USHORT field_dtype = reference->ref_field->fld_dtype;
								if ((dtype_sql_date == field_dtype) ||
									(dtype_sql_time == field_dtype) ||
									(dtype_int64 == field_dtype))
								{
									SQL_dialect1_bad_type(field_dtype);
								}
							}
							reference->ref_context = context;
							CPR_token();
							if (reference->ref_field->fld_array_info) {
								node =
									EXP_array(request, reference->ref_field,
										true, true);
								node->nod_arg[0] = (GPRE_NOD) reference;
							}
							return node;
						}
					}

					sprintf(s, "column \"%s\" not in context",
							gpreGlob.token_global.tok_string);
					PAR_error(s);
				}
			}
			else if (symbol->sym_type == SYM_procedure) {
				const gpre_prc* procedure = (gpre_prc*) symbol->sym_object;
				if (procedure->prc_database != request->req_database)
					PAR_error("procedure not in appropriate database");
				CPR_token();
				if (!MSC_match(KW_DOT))
					CPR_s_error("<period> in qualified column");
				SQL_resolve_identifier("<Column Name>", s);
				for (context = request->req_contexts; context;
					 context = context->ctx_next)
				{
					if (context->ctx_procedure == procedure &&
							(reference->ref_field =
							 MET_context_field(context, gpreGlob.token_global.tok_string)))
					{
						if (SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) {
							const USHORT field_dtype = reference->ref_field->fld_dtype;
							if ((dtype_sql_date == field_dtype) ||
								(dtype_sql_time == field_dtype) ||
								(dtype_int64 == field_dtype))
							{
								SQL_dialect1_bad_type(field_dtype);
							}
						}
						reference->ref_context = context;
						if (reference->ref_field->fld_array_info) {
							node =
								EXP_array(request, reference->ref_field, true,
										  true);
							node->nod_arg[0] = (GPRE_NOD) reference;
						}
						CPR_token();
						return node;
					}
				}

				sprintf(s, "column \"%s\" not in context", gpreGlob.token_global.tok_string);
				PAR_error(s);
			}
		}
	}

//  Hmmm.  So it wasn't a qualified field.  Try any field. 

	SQL_resolve_identifier("<Column Name>", s);
	for (context = request->req_contexts; context;
		 context = context->ctx_next)
	{
		if (reference->ref_field = MET_context_field(context,
													 gpreGlob.token_global.tok_string))
		{
			if (SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) {
				const USHORT field_dtype = reference->ref_field->fld_dtype;
				if ((dtype_sql_date == field_dtype) ||
					(dtype_sql_time == field_dtype) ||
					(dtype_int64 == field_dtype))
				{
					SQL_dialect1_bad_type(field_dtype);
				}
			}
			reference->ref_context = context;

			/* if we skipped down from the SYM_relation case above, we need to
			   ** switch token and prior_token back to their original values to
			   ** continue.
			 */
			if (hold_token.tok_type != 0) {
				gpreGlob.prior_token = gpreGlob.token_global;
				gpreGlob.token_global = hold_token;
			}
			else
				CPR_token();
			if (reference->ref_field->fld_array_info) {
				node = EXP_array(request, reference->ref_field, true, true);
				node->nod_arg[0] = (GPRE_NOD) reference;
			}
			if (request->req_map)
				return post_map(node, request->req_map);
			return node;
		}
	}

	CPR_s_error("<column name>");
	return NULL;				// silence compiler 
}


//____________________________________________________________
//  
//		Parse a list of "things", separated by commas.  Return the
//		whole mess in a list node.
//  

GPRE_NOD SQE_list(pfn_SQE_list_cb routine,
				  gpre_req* request,
				  bool aster_ok)
{
	assert_IS_REQ(request);

	gpre_lls* stack = NULL;
	int count = 0;

	do {
		count++;
		MSC_push((*routine) (request, aster_ok, NULL, NULL), &stack);
	} while (MSC_match(KW_COMMA));

	gpre_nod* list = MSC_node(nod_list, (SSHORT) count);
	gpre_nod** ptr = &list->nod_arg[count];

	while (stack)
		*--ptr = (GPRE_NOD) MSC_pop(&stack);

	return list;
}


//____________________________________________________________
//  
//		Parse procedure input parameters which are constants or
//		host variable reference and, perhaps, a missing
//		flag reference, which may be prefaced by the noiseword,
//       "INDICATOR".
//  

REF SQE_parameter(gpre_req* request,
				  bool aster_ok)
{
	REF reference;
	SCHAR* string;

	assert_IS_REQ(request);

	if (gpreGlob.token_global.tok_type == tok_number) {
		reference = (REF) MSC_alloc(REF_LEN);
		string = (TEXT *) MSC_alloc(gpreGlob.token_global.tok_length + 1);
		MSC_copy(gpreGlob.token_global.tok_string, gpreGlob.token_global.tok_length, string);
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}

	if ((isQuoted(gpreGlob.token_global.tok_type) && gpreGlob.sw_sql_dialect == 1) ||
		gpreGlob.token_global.tok_type == tok_sglquoted)
	{
	/** 
    Since we have stripped the quotes, it is time now to put it back
    so that the host language will interpret it correctly as a string 
    literal.
    ***/
		reference = (REF) MSC_alloc(REF_LEN);
		string = (TEXT *) MSC_alloc(gpreGlob.token_global.tok_length + 3);
		string[0] = '\"';
		MSC_copy(gpreGlob.token_global.tok_string, gpreGlob.token_global.tok_length, string + 1);
		string[gpreGlob.token_global.tok_length + 1] = '\"';
		string[gpreGlob.token_global.tok_length + 2] = 0;
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}

	if (gpreGlob.token_global.tok_keyword == KW_PLUS || gpreGlob.token_global.tok_keyword == KW_MINUS)
	{
		int sign;
		if (gpreGlob.token_global.tok_keyword == KW_MINUS)
			sign = 1;
		else
			sign = 0;
		CPR_token();
		if (gpreGlob.token_global.tok_type != tok_number)
			CPR_s_error("<host variable> or <constant>");
		reference = (REF) MSC_alloc(REF_LEN);
		char* s = string = (TEXT *) MSC_alloc(gpreGlob.token_global.tok_length + 1 + sign);
		if (sign)
			*s++ = '-';
		MSC_copy(gpreGlob.token_global.tok_string, gpreGlob.token_global.tok_length, s);
		reference->ref_value = string;
		reference->ref_flags |= REF_literal;
		CPR_token();
		return reference;
	}

	if (!MSC_match(KW_COLON))
		CPR_s_error("<host variable> or <constant>");

	if (gpreGlob.token_global.tok_type != tok_ident)
		CPR_s_error("<host variable> or <constant>");

	reference = (REF) MSC_alloc(REF_LEN);

	for (gpre_sym* symbol = gpreGlob.token_global.tok_symbol; symbol;
		symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_variable) {
			reference->ref_field = (gpre_fld*) symbol->sym_object;
			break;
		}
	}

	reference->ref_value = PAR_native_value(false, false);

	MSC_match(KW_INDICATOR);

	if (MSC_match(KW_COLON))
		reference->ref_null_value = PAR_native_value(false, false);

	return reference;
}


//____________________________________________________________
//  
//		Given an expression node, for values that don't have a 
//		field, post the given field.
//		Procedure called from EXP_array to post the "subscript field".
//  

void SQE_post_field( GPRE_NOD input, gpre_fld* field)
{
	if (!input || !field)
		return;

	assert_IS_NOD(input);

	switch (input->nod_type)
	{
	case nod_value:
		{
			ref* reference = (REF) input->nod_arg[0];
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
		{
			mel* element = (MEL) input->nod_arg[0];
			gpre_nod* node = element->mel_expr;
			SQE_post_field(node, field);
			return;
		}

	default:
		{
			gpre_nod** ptr = input->nod_arg;
			for (const gpre_nod* const* const end = ptr + input->nod_count;
				ptr < end; ptr++)
			{
				SQE_post_field(*ptr, field);
			}
			return;
		}
	}
}


//____________________________________________________________
//  
//		Post an external reference to a request.  If the expression
//		in question already exists, re-use it.  If there isn't a field,
//		generate a pseudo-field to hold datatype information.  If there
//		isn't a context, well, there isn't a context.
//  

REF SQE_post_reference(gpre_req* request, gpre_fld* field, gpre_ctx* context,
	GPRE_NOD node)
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
	{
			if ((reference->ref_expr && compare_expr(node, reference->ref_expr))
				|| (!reference->ref_expr && field == reference->ref_field
					&& context == reference->ref_context))
		{
			return reference;
		}
	}

//  If there isn't a field given, make one up 

	if (!field) {
		field = (gpre_fld*) MSC_alloc(FLD_LEN);
		CME_get_dtype(node, field);
		if (field->fld_dtype && (field->fld_dtype <= dtype_any_text))
			field->fld_flags |= FLD_text;
	}

//  No reference -- make one 

	reference = (REF) MSC_alloc(REF_LEN);
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
//		If the expression contains a global aggregate, return true,
//		otherwise false.
//  

bool SQE_resolve(GPRE_NOD node,
				 gpre_req* request,
				 gpre_rse* selection)
{
	bool result = false;
	SCHAR s[ERROR_LENGTH];
	act* slice_action = 0;

	assert_IS_REQ(request);
	assert_IS_NOD(node);

	switch (node->nod_type) {
	case nod_plus:
	case nod_minus:
	case nod_times:
	case nod_divide:
	case nod_negate:
	case nod_upcase:
	case nod_concatenate:
	case nod_cast:
		{
			gpre_nod** ptr = node->nod_arg;
			const gpre_nod* const* const end = ptr + node->nod_count;
			for (; ptr < end; ptr++)
				result |= SQE_resolve(*ptr, request, selection);
			return result;
		}

	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
	case nod_agg_average:
	case nod_agg_count:
		if (node->nod_arg[0]) {
			SQE_resolve(node->nod_arg[0], request, selection);
			gpre_nod* node_arg = node->nod_arg[0];
			const ref* reference = (REF) node_arg->nod_arg[0];
			if (node_arg->nod_type == nod_field && reference &&
				reference->ref_field && reference->ref_field->fld_array_info)
			{
				PAR_error
					("Array columns not permitted in aggregate functions");
			}
		}
		return true;

	case nod_udf:
		if (node->nod_arg[0]) {
			gpre_nod** ptr = node->nod_arg[0]->nod_arg;
			const gpre_nod* const* const end = ptr + node->nod_arg[0]->nod_count;
			for (; ptr < end; ptr++)
				result |= SQE_resolve(*ptr, request, selection);
		}
		return result;

	case nod_gen_id:
		return SQE_resolve(node->nod_arg[0], request, selection);

// ** Begin date/time/timestamp support *
	case nod_extract:
		result |= SQE_resolve(node->nod_arg[1], request, selection);
		return result;
// ** End date/time/timestamp support *

	case nod_defered:
		break;

	default:
		return false;
	}

	TOK f_token = (TOK) node->nod_arg[0];
	f_token->tok_symbol = HSH_lookup(f_token->tok_string);
	TOK q_token = (TOK) node->nod_arg[1];
	if (q_token)
		q_token->tok_symbol = HSH_lookup(q_token->tok_string);

	gpre_fld* field = NULL;

	gpre_ctx* context;
	if (request)
		for (context = request->req_contexts; context;
			 context = context->ctx_next)
		{
			if (!context->ctx_stream
				&& (field = resolve(node, context, 0, &slice_action))) break;
		}
	else
		for (SSHORT i = 0; i < selection->rse_count; i++) {
			if (field = resolve(node, selection->rse_context[i], &context, &slice_action))
				break;
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
	if ((SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) &&
		((dtype_sql_date == field->fld_dtype) ||
		 (dtype_sql_time == field->fld_dtype) ||
		 (dtype_int64 == field->fld_dtype)))
	{
		SQL_dialect1_bad_type(field->fld_dtype);
	}

	ref* reference = (REF) MSC_alloc(REF_LEN);
	reference->ref_field = field;
	reference->ref_context = context;
	reference->ref_slice = (slc*) slice_action;

//  donot reinit if this is a nod_deffered type 
	if (node->nod_type != nod_defered)
		node->nod_count = 0;


	node->nod_type = nod_field;
	node->nod_arg[0] = (GPRE_NOD) reference;

	return false;
}


//____________________________________________________________
//  
//		Parse a SELECT (sans keyword) expression.
//  

gpre_rse* SQE_select(gpre_req* request,
			   bool view_flag)
{
	gpre_lls* context_stack = NULL;
	gpre_ctx* context = 0;
	bool have_union = false;

	assert_IS_REQ(request);

	map* const old_map = request->req_map;

//  Get components of union.  Most likely there isn't one, so this is
//  probably wasted work.  

	gpre_rse* select = NULL;
	gpre_rse* rse1 = NULL;
	select = rse1 = par_select(request, NULL);

//  "Look for ... the UNION label ... " 
	while (MSC_match(KW_UNION)) {

		have_union = true;
		const bool union_all = MSC_match(KW_ALL);
		if (!MSC_match(KW_SELECT))
			CPR_s_error("SELECT");

		MSC_push((GPRE_NOD) request->req_contexts, &context_stack);
		request->req_contexts = NULL;
		request->req_map = NULL;
		gpre_rse* rse2 = par_select(request, rse1);

		/* We've got a bona fide union.  Make a union node to hold sub-rse
		   and then a new rse to point to it. */

		select = (gpre_rse*) MSC_alloc(RSE_LEN(1));
		select->rse_context[0] = context = MSC_context(request);
		gpre_nod* node = MSC_node(nod_union, 2);
		select->rse_union = node;
		node->nod_arg[0] = (GPRE_NOD) rse1;
		node->nod_arg[1] = (GPRE_NOD) rse2;

		map* new_map = (map*) MSC_alloc(sizeof(map));
		rse1->rse_map = new_map;
		new_map->map_context = context;
		select->rse_fields = post_select_list(rse1->rse_fields, new_map);

		rse2->rse_map = new_map = (map*) MSC_alloc(sizeof(map));
		new_map->map_context = context;
		post_select_list(rse2->rse_fields, new_map);

		select->rse_into = rse1->rse_into;
		if (!union_all)
			select->rse_reduced = select->rse_fields;

		// Result of this UNION might be the left side of the NEXT UNION 
		rse1 = select;
	}

//  Restore the context lists that were forgotten 
//  <context> holds the most recently allocated context, which is
//  already linked into the request block 

	while (context_stack) {
		while (context->ctx_next)
			context = context->ctx_next;
		context->ctx_next = (gpre_ctx*) MSC_pop(&context_stack);
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

GPRE_NOD SQE_value(gpre_req* request,
				   bool aster_ok,
				   USHORT * paren_count,
				   bool * bool_flag)
{
	USHORT local_count;
	bool local_flag;

	assert_IS_REQ(request);

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}
	if (!bool_flag) {
		local_flag = false;
		bool_flag = &local_flag;
	}

	MSC_match(KW_PLUS);
	gpre_nod* node = par_multiply(request, aster_ok, paren_count, bool_flag);
	assert_IS_NOD(node);
	if (node->nod_type == nod_asterisk) {
		par_terminating_parens(paren_count, &local_count);
		return node;
	}

	nod_t nod_type;
	while (true) {
		if (MSC_match(KW_PLUS))
			nod_type = nod_plus;
		else if (MSC_match(KW_MINUS))
			nod_type = nod_minus;
		else if (MSC_match(KW_OR1))
			nod_type = nod_concatenate;
		else {
			par_terminating_parens(paren_count, &local_count);
			return node;
		}
		gpre_nod* arg = node;
		node = MSC_binary(nod_type, arg,
						  par_multiply(request, false, paren_count, bool_flag));
	}
}


//____________________________________________________________
//  
//		Parse either a literal NULL expression or a value
//		expression.
//  

GPRE_NOD SQE_value_or_null(gpre_req* request,
						   bool aster_ok,
						   USHORT * paren_count,
						   bool * bool_flag)
{
	if (MSC_match(KW_NULL)) {
		return MSC_node(nod_null, 0);
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

GPRE_NOD SQE_variable(gpre_req* request,
					  bool aster_ok,
					  USHORT * paren_count,
					  bool * bool_flag)
{
	assert_IS_REQ(request);

	if (!MSC_match(KW_COLON))
		CPR_s_error("<colon>");

	if (isQuoted(gpreGlob.token_global.tok_type))
		CPR_s_error("<host variable>");

	ref* reference = (REF) MSC_alloc(REF_LEN);

	for (gpre_sym* symbol = gpreGlob.token_global.tok_symbol; symbol;
		symbol = symbol->sym_homonym)
	{
		if (symbol->sym_type == SYM_variable) {
			reference->ref_field = (gpre_fld*) symbol->sym_object;
			break;
		}
	}

	reference->ref_value = PAR_native_value(false, false);

	MSC_match(KW_INDICATOR);

	if (MSC_match(KW_COLON))
		reference->ref_null_value = PAR_native_value(false, false);

	return (GPRE_NOD) reference;
}


//____________________________________________________________
//  
//		Compare two expressions symbollically.  If they're the same,
//		return TRUE, otherwise FALSE.
//  

static bool compare_expr(GPRE_NOD node1,
						 GPRE_NOD node2)
{
	REF ref1, ref2;

	assert_IS_NOD(node1);
	assert_IS_NOD(node2);

	if (node1->nod_type != node2->nod_type)
		return false;

	switch (node1->nod_type) {
	case nod_field:
		ref1 = (REF) node1->nod_arg[0];
		ref2 = (REF) node2->nod_arg[0];
		if (ref1->ref_context != ref2->ref_context ||
			ref1->ref_field != ref2->ref_field ||
			ref1->ref_master != ref2->ref_master) 
		{
			return false;
		}
		return true;

	case nod_map_ref:
		if (node1->nod_arg[0] != node2->nod_arg[0])
			return false;
		return true;

	case nod_udf:
	case nod_gen_id:
		if (node1->nod_arg[0] != node2->nod_arg[0] ||
			node1->nod_arg[1] != node2->nod_arg[1])
		{
			return false;
		}
		return true;

	default:
		return false;
	}
}


//____________________________________________________________
//  
//		Copy a field list for a SELECT against an artificial context.
//  

static GPRE_NOD copy_fields( GPRE_NOD fields, map* fields_map)
{
	assert_IS_NOD(fields);

	gpre_nod* list = MSC_node(nod_list, fields->nod_count);

	for (USHORT i = 0; i < fields->nod_count; i++)
		list->nod_arg[i] = post_fields(fields->nod_arg[i], fields_map);

	return list;
}


//____________________________________________________________
//  
//		Expand an '*' in a field list to the corresponding fields.
//  

static GPRE_NOD explode_asterisk( GPRE_NOD fields, int n, gpre_rse* selection)
{
	TEXT s[ERROR_LENGTH];

	assert_IS_NOD(fields);

	gpre_nod* node = fields->nod_arg[n];
	TOK q_token = (TOK) node->nod_arg[0];
	if (q_token) {
		// expand for single relation 
		gpre_ctx* context = resolve_asterisk(q_token, selection);
		if (context)
			fields = merge_fields(fields, MET_fields(context), n, true);
		else {
			sprintf(s, "columns \"%s.*\" cannot be resolved",
					q_token->tok_string);
			PAR_error(s);
		}
	}
	else {
		// expand for all relations in context list 

		fields = explode_asterisk_all(fields, n, selection, true);
	}

	return fields;
}


//____________________________________________________________
//  
//		Expand an '*' for all relations
//		in the context list.
//  

static GPRE_NOD explode_asterisk_all(GPRE_NOD fields,
									 int n,
									 gpre_rse* selection,
									 bool replace)
{
	assert_IS_NOD(fields);

	for (int i = 0; i < selection->rse_count; i++) {
		gpre_ctx* context = selection->rse_context[i];
		const int old_count = fields->nod_count;
		if (context->ctx_stream)
			fields = explode_asterisk_all(fields, n, context->ctx_stream,
										  replace);
		else
			fields = merge_fields(fields, MET_fields(context), n, replace);
		n += fields->nod_count - old_count;
		replace = false;
	}

	return fields;
}


//____________________________________________________________
//  
//		Get an element of an expression to act as a reference
//		field for determining the data type of a host variable.
//  

static gpre_fld* get_ref( GPRE_NOD expr)
{
	gpre_fld* field;

	assert_IS_NOD(expr);

	if (expr->nod_type == nod_via || expr->nod_type == nod_cast) {
		field = (gpre_fld*) MSC_alloc(FLD_LEN);
		CME_get_dtype(expr, field);
		if (field->fld_dtype && (field->fld_dtype <= dtype_any_text))
			field->fld_flags |= FLD_text;
		return field;
	}

	ref* reference;
	
	switch (expr->nod_type) {
	case nod_field:
		reference = (ref*) expr->nod_arg[0];
		return reference->ref_field;


	case nod_array:
		reference = (ref*) expr->nod_arg[0];
		return reference->ref_field->fld_array;

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
		{
			gpre_nod** ptr = expr->nod_arg;
			for (const gpre_nod* const* const end = ptr + expr->nod_count;
				ptr < end; ptr++)
			{
				if (field = get_ref(*ptr))
					return field;
			}
			break;
		}

// ** Begin date/time/timestamp support *
	case nod_extract:
		if (field = get_ref(expr->nod_arg[1]))
			return field;
		break;
// ** End date/time/timestamp support *
	case nod_map_ref:
		{
			mel* element = (MEL) expr->nod_arg[0];
			gpre_nod* node = element->mel_expr;
			return get_ref(node);
		}
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

static GPRE_NOD implicit_any(gpre_req* request,
							 GPRE_NOD value,
							 enum nod_t comparison,
							 enum nod_t any_all)
{
	GPRE_NOD node;
	scope previous_scope;

	assert_IS_REQ(request);
	assert_IS_NOD(value);

	gpre_ctx* original = request->req_contexts;

	request->req_in_subselect++;
	push_scope(request, &previous_scope);

	if (!(original->ctx_relation || original->ctx_procedure) &&	request->req_map)
		value = post_fields(value, request->req_map);

//  Handle the ALL and DISTINCT options 
	const bool distinct = (!MSC_match(KW_ALL) && MSC_match(KW_DISTINCT));

	request->req_in_select_list++;
	gpre_nod* value2 = SQE_value(request, false, NULL, NULL);
	request->req_in_select_list--;

	gpre_nod* field_list = MSC_node(nod_list, 1);
	field_list->nod_arg[0] = value2;

	gpre_rse* selection = par_rse(request, field_list, distinct);
	value2 = selection->rse_fields->nod_arg[0];

	gpre_rse* sub = selection->rse_aggregate;
	if (sub) {
		if (validate_references(value2, sub->rse_group_by))
			PAR_error
				("simple column reference not allowed in aggregate context");
		if (sub->rse_group_by) {
			node = MSC_binary(comparison, value, value2);
			pair(node->nod_arg[0], node->nod_arg[1]);
			selection->rse_boolean = merge(selection->rse_boolean, node);
			if (any_all == nod_ansi_all)
				node = MSC_node(nod_ansi_all, 1);
			else if (!(request->req_database->dbb_flags & DBB_v3))
				node = MSC_node(nod_ansi_any, 1);
			else
				node = MSC_node(nod_any, 1);
			node->nod_count = 0;
			node->nod_arg[0] = (GPRE_NOD) selection;
		}
		else {
			gpre_nod* node2 = MSC_node(nod_via, 3);
			node2->nod_count = 0;
			node2->nod_arg[0] = (GPRE_NOD) selection;
			node2->nod_arg[2] = MSC_node(nod_null, 0);
			node2->nod_arg[1] = value2;
			node = MSC_binary(comparison, value, node2);
			pair(node->nod_arg[0], node->nod_arg[1]);
		}
	}
	else {
		node = MSC_binary(comparison, value, value2);
		pair(node->nod_arg[0], node->nod_arg[1]);
		selection->rse_boolean = merge(selection->rse_boolean, node);
		if (any_all == nod_ansi_all)
			node = MSC_node(nod_ansi_all, 1);
		else if (!(request->req_database->dbb_flags & DBB_v3))
			node = MSC_node(nod_ansi_any, 1);
		else
			node = MSC_node(nod_any, 1);
		node->nod_count = 0;
		node->nod_arg[0] = (GPRE_NOD) selection;
	}

	EXP_rse_cleanup(selection);

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

static GPRE_NOD merge_fields(GPRE_NOD fields_1,
							 GPRE_NOD fields_2,
							 int n,
							 bool replace)
{
	int i;

	assert_IS_NOD(fields_1);
	assert_IS_NOD(fields_2);

	int count = fields_1->nod_count + fields_2->nod_count;
	if (replace)
		count--;
	gpre_nod* fields = MSC_node(nod_list, (SSHORT) count);

	count = n;
	if (!replace)
		count++;
	for (i = 0; i < count; i++)
		fields->nod_arg[i] = fields_1->nod_arg[i];

	for (i = 0; i < fields_2->nod_count; i++)
		fields->nod_arg[i + count] = fields_2->nod_arg[i];

	int offset = 0;
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

	gpre_fld* field = 0;
	if (expr2)
		field = get_ref(expr2);
	if (!field)
		field = get_ref(expr1);

	if (!field)
		return;

	set_ref(expr1, field);

	if (!expr2)
		return;

	gpre_fld* temp = get_ref(expr1);
	if (temp)
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

static gpre_ctx* par_alias_list( gpre_req* request, GPRE_NOD alias_list)
{
	SCHAR error_string[ERROR_LENGTH];

	assert_IS_REQ(request);
	assert_IS_NOD(alias_list);

	gpre_nod** arg = alias_list->nod_arg;
	const gpre_nod* const* const end = alias_list->nod_arg + alias_list->nod_count;

//  check the first alias in the list with the relations
//  in the current context for a match 

	gpre_rel* relation = 0; // unreliable test many lines below without initializing.
	gpre_ctx* context = par_alias(request, (const TEXT*) * arg);
	if (context) {
		if (alias_list->nod_count == 1)
			return context;
		relation = context->ctx_relation;
	}

//  if the first alias didn't specify a table in the context stack, 
//  look through all contexts to find one which might be a view with
//  a base table having a matching table name or alias 

	if (!context)
		for (context = request->req_contexts; context;
			 context = context->ctx_next)
		{
			if (context->ctx_scope_level != request->req_scope_level)
				continue;
			if (!context->ctx_relation)
				continue;
			if (relation =
				par_base_table(request, context->ctx_relation,
							   (const TEXT*) * arg))
			{
				break;
			}
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
		if (!(relation = par_base_table(request, relation, (const TEXT*) * arg)))
			break;

	if (!relation) {
		sprintf(error_string,
				"there is no alias or table named %s at this scope level",
				(TEXT *) * arg);
		PAR_error(error_string);
	}

//  make up a dummy context to hold the resultant relation 

	gpre_ctx* new_context = (gpre_ctx*) MSC_alloc(CTX_LEN);
	new_context->ctx_request = request;
	new_context->ctx_internal = context->ctx_internal;
	new_context->ctx_relation = relation;

//  concatenate all the contexts to form the alias name;
//  calculate the length leaving room for spaces and a null 

	USHORT alias_length = alias_list->nod_count;
	for (arg = alias_list->nod_arg; arg < end; arg++)
		alias_length += strlen((TEXT *) * arg);

	TEXT* alias = (TEXT *) MSC_alloc(alias_length);

	// CVC: Warning: Using space as separator may conflict with dialect 3's embedded blanks;
	// but gpre is not much worried about dialects... except in this file!
	TEXT* p = new_context->ctx_alias = alias;
	for (arg = alias_list->nod_arg; arg < end; arg++) {
		for (const TEXT* q = (TEXT *) * arg; *q;)
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

static gpre_ctx* par_alias( gpre_req* request, const TEXT* alias)
{
	SCHAR error_string[ERROR_LENGTH];

	assert_IS_REQ(request);

//  look through all contexts at this scope level
//  to find one that has a relation name or alias
//  name which matches the identifier passed 

	gpre_ctx* relation_context = NULL;
	for (gpre_ctx* context = request->req_contexts; context;
		 context = context->ctx_next)
	{
		if (context->ctx_scope_level != request->req_scope_level)
			continue;

		// check for matching alias 

		if (context->ctx_alias) {
			const TEXT* p = context->ctx_alias;
			const TEXT* q = alias;
			for (; *p && *q; p++, q++)
				if (UPPER(*p) != UPPER(*q))
					break;
			if (!*p && !*q)
				return context;
		}

		/* check for matching relation name; aliases take priority so
		   save the context in case there is an alias of the same name;
		   also to check that there is no self-join in the query */

		if (context->ctx_relation &&
			!strcmp(context->ctx_relation->rel_symbol->sym_string, alias))
		{
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

static gpre_rel* par_base_table( gpre_req* request, const gpre_rel* relation,
	const TEXT* alias)
{

	assert_IS_REQ(request);

	return MET_get_view_relation(request, relation->rel_symbol->sym_string,
								 alias, 0);
}


//____________________________________________________________
//  
//		Parse an AND boolean expression.
//  

static GPRE_NOD par_and( gpre_req* request, USHORT * paren_count)
{
	assert_IS_REQ(request);

	gpre_nod* expr1 = par_not(request, paren_count);

	if (!MSC_match(KW_AND))
		return expr1;

	return merge(expr1, par_and(request, paren_count));
}


//____________________________________________________________
//  
//  

static GPRE_NOD par_collate( gpre_req* request, GPRE_NOD arg)
{
	assert_IS_REQ(request);
	assert_IS_NOD(arg);

	gpre_nod* node = MSC_node(nod_cast, 2);
	node->nod_count = 1;
	node->nod_arg[0] = arg;
	gpre_fld* field = (gpre_fld*) MSC_alloc(FLD_LEN);
	node->nod_arg[1] = (GPRE_NOD) field;
	CME_get_dtype(arg, field);
	if (field->fld_dtype > dtype_any_text) {
		// cast expression to VARYING with implementation-defined 
		// maximum length 

		field->fld_dtype = dtype_varying;
		field->fld_char_length = 30;
		field->fld_length = 0;	// calculated by SQL_adjust_field_dtype 
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

static GPRE_NOD par_in( gpre_req* request, GPRE_NOD value)
{
	GPRE_NOD node;
	SCHAR s[ERROR_LENGTH];

	assert_IS_REQ(request);
	assert_IS_NOD(value);

	EXP_left_paren(0);

//  If the next token isn't SELECT, we must have the comma list flavor. 

	if (MSC_match(KW_SELECT))
		node = implicit_any(request, value, nod_eq, nod_ansi_any);
	else {
		node = NULL;
		while (true) {
			gpre_nod* value2 = par_primitive_value(request, false, 0, NULL);
			if (value2->nod_type == nod_value) {
				ref* ref2 = (REF) value2->nod_arg[0];
				if (value->nod_type == nod_field) {
					ref* ref1 = (REF) value->nod_arg[0];
					ref2->ref_field = ref1->ref_field;
				}
				else {
					sprintf(s, "datatype of %s can not be determined",
							gpreGlob.token_global.tok_string);
					PAR_error(s);
				}
			}
			if (!node)
				node = MSC_binary(nod_eq, value, value2);
			else
				node =
					MSC_binary(nod_or, node,
							   MSC_binary(nod_eq, value, value2));

			if (!(MSC_match(KW_COMMA)))
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

static gpre_ctx* par_joined_relation( gpre_req* request, gpre_ctx* prior_context)
{
	gpre_ctx* context1;

	assert_IS_REQ(request);

	if (MSC_match(KW_LEFT_PAREN)) {
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

static gpre_ctx* par_join_clause( gpre_req* request, gpre_ctx* context1)
{
	assert_IS_REQ(request);

	const NOD_T join_type = par_join_type();
	if (join_type == (NOD_T) 0)
		return context1;

	gpre_ctx* context2 = par_joined_relation(request, context1);
	if (!context2)
		CPR_s_error("<joined table clause>");

	if (!MSC_match(KW_ON))
		CPR_s_error("ON");

	gpre_nod* node = SQE_boolean(request, NULL);

	gpre_rse* selection = (gpre_rse*) MSC_alloc(RSE_LEN(2));
	selection->rse_count = 2;
	selection->rse_context[0] = context1;
	selection->rse_context[1] = context2;
	selection->rse_boolean = node;
	selection->rse_join_type = join_type;

	context1 = MSC_context(request);
	context1->ctx_stream = selection;

	return par_join_clause(request, context1);
}


//____________________________________________________________
//  
//		Parse a join type.
//  

static NOD_T par_join_type(void)
{
	NOD_T nod_type;

	if (MSC_match(KW_INNER))
		nod_type = nod_join_inner;
	else if (MSC_match(KW_LEFT))
		nod_type = nod_join_left;
	else if (MSC_match(KW_RIGHT))
		nod_type = nod_join_right;
	else if (MSC_match(KW_FULL))
		nod_type = nod_join_full;
	else if (MSC_match(KW_JOIN))
		return nod_join_inner;
	else
		return (NOD_T) 0;

	if (nod_type != nod_join_inner)
		MSC_match(KW_OUTER);

	if (!MSC_match(KW_JOIN))
		CPR_s_error("JOIN");

	return nod_type;
}


//____________________________________________________________
//  
//		Parse either of the high precedence operators * and /.
//  

static GPRE_NOD par_multiply(gpre_req* request,
							 bool aster_ok,
							 USHORT * paren_count,
							 bool * bool_flag)
{
	assert_IS_REQ(request);
	gpre_nod* node = par_primitive_value(request, aster_ok, paren_count, bool_flag);
	if (node->nod_type == nod_asterisk)
		return node;

	if (gpreGlob.token_global.tok_keyword == KW_COLLATE)
		return par_collate(request, node);

	GPRE_NOD arg;
	enum nod_t nod_type;
	while (true) {
		if (MSC_match(KW_ASTERISK))
			nod_type = nod_times;
		else if (MSC_match(KW_SLASH))
			nod_type = nod_divide;
		else
			return node;
		arg = node;
		node =
			MSC_binary(nod_type, arg,
					   par_primitive_value(request, false, paren_count,
										   bool_flag));
	}
}


//____________________________________________________________
//  
//		Parse an NOT boolean expression.
//  

static GPRE_NOD par_not( gpre_req* request, USHORT * paren_count)
{
	GPRE_NOD node, field;

	assert_IS_REQ(request);

	if (MSC_match(KW_NOT))
		return negate(par_not(request, paren_count));

	enum nod_t type = (enum nod_t) 0;

	if (MSC_match(KW_EXISTS))
		type = nod_any;
	else if (MSC_match(KW_SINGULAR))
		type = nod_unique;
	if (type == nod_any || type == nod_unique) {
		scope saved_scope;
		push_scope(request, &saved_scope);

		EXP_left_paren(0);
		if (!MSC_match(KW_SELECT))
			CPR_s_error("SELECT");

		request->req_in_select_list++;
		if (MSC_match(KW_ASTERISK))
			field = NULL;
		else if (!(field = par_udf(request)))
			field = SQE_field(request, false);
		request->req_in_select_list--;

		node = MSC_node(type, 1);
		node->nod_count = 0;
		gpre_rse* selection = par_rse(request, 0, false);
		node->nod_arg[0] = (GPRE_NOD) selection;
		if (field) {
			SQE_resolve(field, 0, selection);
			gpre_nod* expr = MSC_unary(nod_missing, field);
			selection->rse_boolean = merge(negate(expr), selection->rse_boolean);
		}
		EXP_rse_cleanup((gpre_rse*) node->nod_arg[0]);
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

static void par_order(gpre_req* request,
					  gpre_rse* select,
					  bool union_f,
					  bool view_flag)
{
	GPRE_NOD sort;
	map* request_map;
	USHORT i;

	assert_IS_REQ(request);

//  This doesn't really belong here, but it's convenient.  Parse the
//  SQL "FOR UPDATE OF ..." clause.  Just eat it and ignore it. 

	if (MSC_match(KW_FOR)) {
		MSC_match(KW_UPDATE);
		MSC_match(KW_OF);
		do {
			CPR_token();
		} while (MSC_match(KW_COMMA));
	}

	if (!MSC_match(KW_ORDER))
		return;
	if (view_flag)
		PAR_error("sort clause not allowed in a view definition");

	MSC_match(KW_BY);
	gpre_lls* items = NULL;
	gpre_lls* directions = NULL;
	int count = 0, direction = 0;
	gpre_nod* values = select->rse_fields;

	while (true) {
		direction = FALSE;
		if (gpreGlob.token_global.tok_type == tok_number) {
			i = EXP_USHORT_ordinal(false);
			if (i < 1 || i > values->nod_count)
				CPR_s_error("<ordinal column position>");
			sort = values->nod_arg[i - 1];
			PAR_get_token();
			if (gpreGlob.token_global.tok_keyword == KW_COLLATE)
				sort = par_collate(request, sort);
		}
		else {
			if (union_f)
				CPR_s_error("<column position in union>");
			sort = SQE_value(request, false, NULL, NULL);
			if (request && (request_map = request->req_map))
				sort = post_map(sort, request_map);
		}
		if (MSC_match(KW_ASCENDING))
			direction = FALSE;
		else if (MSC_match(KW_DESCENDING))
			direction = TRUE;
		count++;
		MSC_push((GPRE_NOD)(IPTR)direction, &directions);
		MSC_push(sort, &items);
		if (!MSC_match(KW_COMMA))
			break;
	}

	select->rse_sort = sort = MSC_node(nod_sort, (SSHORT) (count * 2));
	sort->nod_count = count;
	gpre_nod** ptr = sort->nod_arg + count * 2;

	while (items) {
		*--ptr = (GPRE_NOD) MSC_pop(&items);
		*--ptr = (GPRE_NOD) MSC_pop(&directions);
	}
}


//____________________________________________________________
//  
//		Allow the user to specify the access plan
//		for a query as part of a select expression.
//  

static GPRE_NOD par_plan( gpre_req* request)
{
	assert_IS_REQ(request);

//  parse the join type 

	NOD_T nod_type;
	if (MSC_match(KW_JOIN))
		nod_type = nod_join;
	else if (MSC_match(KW_MERGE))
		nod_type = nod_merge;
	else if (MSC_match(KW_SORT) && MSC_match(KW_MERGE))
		nod_type = nod_merge;
	else
		nod_type = nod_join;

//  make up the plan expression node 

	gpre_nod* plan_expression = MSC_node(nod_plan_expr, 2);

	if (nod_type != nod_join)
		plan_expression->nod_arg[0] = MSC_node(nod_type, 0);

//  parse the plan items at this level 

	EXP_left_paren(0);

	plan_expression->nod_arg[1] = SQE_list(par_plan_item, request, false);

	if (!EXP_match_paren())
		return NULL;
	return plan_expression;
}


//____________________________________________________________
//  
//		Parse an individual plan item for an 
//		access plan.
//  

static GPRE_NOD par_plan_item(gpre_req* request,
							  bool aster_ok,
							  USHORT * paren_count,
							  bool * bool_flag)
{
	assert_IS_REQ(request);

//  check for a plan expression 

	if (gpreGlob.token_global.tok_keyword == KW_JOIN || gpreGlob.token_global.tok_keyword == KW_SORT ||
		gpreGlob.token_global.tok_keyword == KW_MERGE || gpreGlob.token_global.tok_keyword == KW_LEFT_PAREN)
	{
		return par_plan(request);
	}

//  parse the list of one or more table names or
//  aliases (more than one is used when there is
//  a need to differentiate base tables of a view) 

	int count;
	gpre_lls* stack = NULL;
	for (count = 0; gpreGlob.token_global.tok_type == tok_ident; count++) {
		if (gpreGlob.token_global.tok_keyword == KW_NATURAL ||
			gpreGlob.token_global.tok_keyword == KW_ORDER || 
			gpreGlob.token_global.tok_keyword == KW_INDEX)
		{
			break;
		}

		MSC_push((GPRE_NOD) upcase_string(gpreGlob.token_global.tok_string), &stack);
		PAR_get_token();
	}

	if (!count)
		CPR_s_error("<table name> or <alias>");

	gpre_nod** ptr;

	gpre_nod* alias_list = MSC_node(nod_list, (SSHORT) count);
	for (ptr = &alias_list->nod_arg[count]; stack;)
		*--ptr = (GPRE_NOD) MSC_pop(&stack);

//  lookup the contexts for the aliases 

	gpre_ctx* context = par_alias_list(request, alias_list);

//  parse the access type 

	gpre_nod* access_type;
	if (gpreGlob.token_global.tok_keyword == KW_NATURAL) {
		access_type = MSC_node(nod_natural, 0);
		PAR_get_token();
	}
	else if (gpreGlob.token_global.tok_keyword == KW_ORDER) {
		access_type = MSC_node(nod_index_order, 1);
		access_type->nod_count = 0;
		PAR_get_token();

		if (gpreGlob.token_global.tok_type != tok_ident)
			CPR_s_error("<index name>");
		access_type->nod_arg[0] = (GPRE_NOD) upcase_string(gpreGlob.token_global.tok_string);
		PAR_get_token();
	}
	else if (gpreGlob.token_global.tok_keyword == KW_INDEX) {
		access_type = MSC_node(nod_index, 1);
		access_type->nod_count = 0;
		PAR_get_token();

		EXP_left_paren(0);

		stack = NULL;
		for (count = 0; gpreGlob.token_global.tok_type == tok_ident;) {
			MSC_push((GPRE_NOD) upcase_string(gpreGlob.token_global.tok_string), &stack);
			PAR_get_token();

			count++;

			if (!MSC_match(KW_COMMA))
				break;
		}
		if (!count)
			CPR_s_error("<table name> or <alias>");

		gpre_nod* index_list = MSC_node(nod_list, (SSHORT) count);
		access_type->nod_arg[0] = index_list;
		for (ptr = &index_list->nod_arg[count]; stack;)
			*--ptr = (GPRE_NOD) MSC_pop(&stack);

		if (!EXP_match_paren())
			return NULL;
	}
	else {
		CPR_s_error("NATURAL, ORDER, or INDEX");
	}


//  generate the plan item node 

	gpre_nod* plan_item = MSC_node(nod_plan_item, 3);
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

static GPRE_NOD par_primitive_value(gpre_req* request,
									bool aster_ok,
									USHORT * paren_count,
									bool * bool_flag)
{
	USHORT local_count;

	assert_IS_REQ(request);

	gpre_nod* node = 0;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	if (MSC_match(KW_SELECT))
		return par_stat(request);

	if (MSC_match(KW_MINUS))
		return MSC_unary(nod_negate,
						 par_primitive_value(request, false, paren_count, false));

	MSC_match(KW_PLUS);

	if (MSC_match(KW_USER)) {
		return MSC_node(nod_user_name, 0);
	}

	if (MSC_match(KW_VALUE)) {
		/* If request is NULL we must be processing a subquery - and 
		 * without the request to refer to we're kinda hosed
		 */
		if (!request)
			PAR_error("VALUE cannot be used in this context");
		const act* action = request->req_actions;
		if (request->req_type != REQ_ddl ||
			!action ||
			!(action->act_type == ACT_create_domain ||
			  action->act_type == ACT_alter_domain))
		{
			PAR_error("VALUE cannot be used in this context");
		}

		return MSC_node(nod_dom_value, 0);
	}

	if (MSC_match(KW_LEFT_PAREN)) {
		(*paren_count)++;
		if (bool_flag && *bool_flag)
			node = SQE_boolean(request, paren_count);
		else
			node = SQE_value(request, false, paren_count, bool_flag);
		EXP_match_paren();
		(*paren_count)--;
		return node;
	}

//  Check for an aggregate statistical expression.  If we already have a
//  map defined for the request, we're part of either HAVING or a trailing
//  ORDER clause.  In this case, post only the complete expression, and not
//  the sub-expressions. 

	map* tmp_map = 0;
	for (const ops *op = stat_ops; (int) op->rel_kw != (int) KW_none; op++) {
		MSC_match(KW_ALL);
		if (MSC_match(op->rel_kw)) {
			if (request && (request->req_in_aggregate ||
							!(request->req_in_select_list ||
							  request->req_in_having_clause ||
							  request->req_in_order_by_clause)))
			{
				/* either nested aggregate, or not part of a select
				   list, having clause, or order by clause (in any subquery)
				 */
				PAR_error("Invalid aggregate reference");
			}

			node = MSC_node(op->rel_op, 2);
			node->nod_count = 1;
			EXP_left_paren("left parenthesis in statistical function");
			const bool distinct = MSC_match(KW_DISTINCT);
			if (request) {
				tmp_map = request->req_map;
				request->req_map = NULL;
				++request->req_in_aggregate;
			}
			if (node->nod_type == nod_agg_count && MSC_match(KW_ASTERISK))
				node->nod_count = 0;
			else {
				node->nod_arg[0] = SQE_value(request, false, NULL, NULL);
				// Disallow arrays as arguments to aggregate functions  
				const gpre_nod* node_arg = node->nod_arg[0];
				if (node_arg && node_arg->nod_type == nod_array)
					PAR_error
						("Array columns not permitted in aggregate functions");
			}

			if (distinct)
				node->nod_arg[1] = node->nod_arg[0];
			EXP_match_paren();
			if (request) {
				if (tmp_map)
					node = post_map(node, tmp_map);
				request->req_map = tmp_map;
				--request->req_in_aggregate;
			}
			return node;
		}
	}

//  If it's a number or a quoted string, it's a literal 

	if (gpreGlob.token_global.tok_type == tok_number ||
		(isQuoted(gpreGlob.token_global.tok_type) && gpreGlob.sw_sql_dialect == 1) ||
		gpreGlob.token_global.tok_type == tok_sglquoted)
	{
		node = EXP_literal();
		return node;
	}

//  moved this timestamp support down some lines, because it caused
//  gpre to segfault when it was done here.
//  FSG 15.Nov.2000
//  


//  If the next token is a colon, it is a variable reference 

	if ((int) gpreGlob.token_global.tok_keyword == (int) KW_COLON) {
		if (!request) {
			/* We must be processing a subquery - and without the request to
			 * post the :hostvar to we can't continue.
			 * (core dump when de-refer of NULL request)
			 */
			PAR_error(":hostvar reference not supported in this context");
			return NULL;
		}
		ref* reference = (REF) SQE_variable(request, false, NULL, NULL);
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
		const KWWORDS kw_word = gpreGlob.token_global.tok_keyword;

		if (MSC_match(KW_DATE) || MSC_match(KW_TIME) || MSC_match(KW_TIMESTAMP)) {
			gpreGlob.token_global.tok_keyword = kw_word;
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

static GPRE_NOD par_relational(gpre_req* request,
							   USHORT * paren_count)
{
	GPRE_NOD node;
	bool negation = false;
	bool local_flag  =true;

	assert_IS_REQ(request);

	gpre_nod* expr1 = SQE_value(request, false, paren_count, &local_flag);
	if (gpreGlob.token_global.tok_keyword == KW_RIGHT_PAREN)
		return expr1;
	if (gpreGlob.token_global.tok_keyword == KW_SEMI_COLON)
	{
		for (const NOD_T* relational_ops = relationals; *relational_ops != (NOD_T) 0;
			 relational_ops++)
		{
			if (expr1->nod_type == *relational_ops)
				return expr1;
		}
	}

	if (MSC_match(KW_NOT))
		negation = true;

//  Check for one of the binary operators 

	if (MSC_match(KW_IN))
		node = par_in(request, expr1);
	else if (MSC_match(KW_BETWEEN)) {
		node = MSC_node(nod_between, 3);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = SQE_value(request, false, NULL, NULL);
		MSC_match(KW_AND);
		node->nod_arg[2] = SQE_value(request, false, NULL, NULL);
		pair(node->nod_arg[0], node->nod_arg[1]);
		pair(node->nod_arg[0], node->nod_arg[2]);
	}
	else if (MSC_match(KW_LIKE)) {
		node = MSC_node(nod_like, 3);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = SQE_value(request, false, NULL, NULL);
		pair(node->nod_arg[0], node->nod_arg[1]);
		if (MSC_match(KW_ESCAPE)) {
			gpre_nod* expr2 = SQE_value(request, false, NULL, NULL);
			node->nod_arg[2] = expr2;
			if (expr2->nod_type == nod_value) {
				ref* ref_value = (REF) expr2->nod_arg[0];
				ref_value->ref_field = MET_make_field("like_escape_character",
													  dtype_text, 2, false);
			}
		}
		else
			node->nod_count = 2;
	}
	else if (MSC_match(KW_IS)) {
		if (MSC_match(KW_NOT))
			negation = !negation;
		if (!MSC_match(KW_NULL))
			CPR_s_error("NULL");
		if (expr1->nod_type == nod_array)
			expr1->nod_type = nod_field;
		node = MSC_unary(nod_missing, expr1);
	}
	else {
		node = NULL;
		const ops* op;
		for (op = rel_ops; (int) op->rel_kw != (int) KW_none; op++)
			if (MSC_match(op->rel_kw))
				break;
		if ((int) op->rel_kw == (int) KW_none) {
			for (const NOD_T* relational_ops = relationals;
				*relational_ops != (NOD_T) 0; relational_ops++)
			{
				if (expr1->nod_type == *relational_ops)
					return expr1;
			}
			CPR_s_error("<relational operator>");
		}
		if ((int) op->rel_kw == (int) KW_STARTING)
			MSC_match(KW_WITH);
		if (MSC_match(KW_ANY)) {
			if (!MSC_match(KW_LEFT_PAREN) || !MSC_match(KW_SELECT))
				CPR_s_error("<select clause> for ANY");
			node = implicit_any(request, expr1, op->rel_op, nod_any);
			EXP_match_paren();
		}
		else if (MSC_match(KW_ALL)) {
			if (!MSC_match(KW_LEFT_PAREN) || !MSC_match(KW_SELECT))
				CPR_s_error("<select clause> for ALL");
			if (op->rel_negation == nod_any
				|| op->rel_negation == nod_ansi_any
				|| op->rel_negation == nod_ansi_all)
			{
				CPR_s_error("<relational operator> for ALL");
			}
			node = implicit_any(request, expr1, op->rel_op, nod_ansi_all);
			EXP_match_paren();
		}
		else {
			node = MSC_binary(op->rel_op, expr1,
							  SQE_value(request, false, NULL, NULL));
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

static gpre_rse* par_rse(gpre_req* request,
				   GPRE_NOD fields,
				   bool distinct)
{
	gpre_lls* stack = NULL;

	assert_IS_REQ(request);
	assert_IS_NOD(fields);

//  Get list and count of relations 

	if (!MSC_match(KW_FROM))
		CPR_s_error("FROM");

	int count = 0;
	gpre_ctx* context;
	do {
		if (context = par_joined_relation(request, NULL)) {
			MSC_push((GPRE_NOD) context, &stack);
			count++;
		}
		else
			return NULL;
	} while (MSC_match(KW_COMMA));

//  Now allocate a record select expression
//  block for the beast and fill in what we already know.  

	gpre_rse* select = (gpre_rse*) MSC_alloc(RSE_LEN(count));
	select->rse_count = count;

	while (count--)
		select->rse_context[count] = (gpre_ctx*) MSC_pop(&stack);

//  If a field list has been presented, resolve references now 

	bool aggregate = false;

	if (fields) {
		gpre_nod** ptr = fields->nod_arg;
		count = fields->nod_count;
		for (int i = 0; i < count; i++)
		{
			gpre_nod* node = *(ptr + i);
			if (node->nod_type == nod_asterisk) {
				const int old_count = count;
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

	if (MSC_match(KW_WITH)) {
		++request->req_in_where_clause;
		select->rse_boolean = SQE_boolean(request, 0);
		--request->req_in_where_clause;
	}

	if (MSC_match(KW_GROUP)) {
		MSC_match(KW_BY);
		select->rse_group_by =
			SQE_list(par_udf_or_field_with_collate, request, false);
		gpre_nod** ptr = select->rse_group_by->nod_arg;
		for (const gpre_nod* const* const end =
			 ptr + select->rse_group_by->nod_count; ptr < end; ptr++)
		{
			if ((*ptr)->nod_type == nod_array)
				PAR_error("Array columns not permitted in GROUP BY clause");
		}
	}

	if (select->rse_group_by || aggregate) {
		if (validate_references(select->rse_fields, select->rse_group_by))
			PAR_error
				("simple column reference not allowed in aggregate context");
		gpre_rse* sub_rse = select;
		map* subselect_map = (map*) MSC_alloc(sizeof(map));
		sub_rse->rse_map = subselect_map;
		if (select->rse_group_by)
			request->req_map = subselect_map;
		subselect_map->map_context = MSC_context(request);
		select = (gpre_rse*) MSC_alloc(RSE_LEN(0));
		select->rse_aggregate = sub_rse;

		if (fields)
			select->rse_fields = copy_fields(sub_rse->rse_fields, subselect_map);

		if (MSC_match(KW_HAVING)) {
			++request->req_in_having_clause;
			select->rse_boolean = SQE_boolean(request, 0);
			--request->req_in_having_clause;
			if (validate_references(select->rse_boolean,
				 sub_rse->rse_group_by))
			{
				PAR_error("simple column reference in HAVING must be referenced in GROUP BY");
			}
		}
	}

//  parse a user-specified access plan 

	if (MSC_match(KW_PLAN))
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

static gpre_rse* par_select( gpre_req* request, gpre_rse* union_rse)
{
	assert_IS_REQ(request);

//  Handle the ALL and DISTINCT options 

	const bool distinct = (!MSC_match(KW_ALL) && MSC_match(KW_DISTINCT));

//  Make select list out of select items 

	++request->req_in_select_list;
	gpre_nod* s_list = SQE_list(SQE_value_or_null, request, true);
	--request->req_in_select_list;

//  If this is not a declare cursor statement and an INTO list is present,
//  parse it. 

	gpre_nod* into_list;
	if (!(request->req_flags & REQ_sql_declare_cursor))
		into_list = (MSC_match(KW_INTO)) ? SQE_list(SQE_variable, request,
										false) : NULL;
	else
		into_list = NULL;

	gpre_rse* select = par_rse(request, s_list, distinct);
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

static GPRE_NOD par_stat( gpre_req* request)
{
	assert_IS_REQ(request);

	request->req_in_subselect++;
	scope previous_scope;
	push_scope(request, &previous_scope);

	const bool distinct = (!MSC_match(KW_ALL) && MSC_match(KW_DISTINCT));

	request->req_in_select_list++;
	gpre_nod* item = par_udf(request);
	if (!item)
		item = SQE_value(request, false, NULL, NULL);
	request->req_in_select_list--;

	gpre_nod* field_list = MSC_node(nod_list, 1);
	field_list->nod_arg[0] = item;
	gpre_rse* select = par_rse(request, field_list, distinct);
	select->rse_flags |= RSE_singleton;

	item = select->rse_fields->nod_arg[0];

	gpre_nod* node = MSC_node(nod_via, 3);
	node->nod_count = 0;
	node->nod_arg[0] = (GPRE_NOD) select;
	node->nod_arg[2] = MSC_node(nod_null, 0);
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

static GPRE_NOD par_subscript( gpre_req* request)
{
	assert_IS_REQ(request);

	ref* reference = (REF) MSC_alloc(REF_LEN);
	gpre_nod* node = MSC_unary(nod_value, (GPRE_NOD) reference);

//  Special case literals 

	if (gpreGlob.token_global.tok_type == tok_number) {
		node->nod_type = nod_literal;
		char* string = (TEXT *) MSC_alloc(gpreGlob.token_global.tok_length + 1);
		reference->ref_value = string;
		MSC_copy(gpreGlob.token_global.tok_string, gpreGlob.token_global.tok_length, string);
		PAR_get_token();
		return node;
	}

	if (!MSC_match(KW_COLON))
		CPR_s_error("<colon>");

	reference->ref_value = PAR_native_value(false, false);

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
	// Suspicious condition.
	if (*paren_count && paren_count == local_count)
		do {
			EXP_match_paren();
		} while (--(*paren_count));
}


//____________________________________________________________
//  
//		Parse a user defined function.  If the current token isn't one,
//		return NULL.  Otherwise try to parse one.  If things go badly,
//		complain bitterly.
//  

static GPRE_NOD par_udf( gpre_req* request)
{
	GPRE_NOD node;
	USHORT local_count;
	SCHAR s[ERROR_LENGTH];

	if (!request)
		return NULL;

	assert_IS_REQ(request);

//  Check for user defined functions 
// ** resolve only if an identifier *
	if ((isQuoted(gpreGlob.token_global.tok_type)) || gpreGlob.token_global.tok_type == tok_ident)
		SQL_resolve_identifier("<Udf Name>", s);

	udf* an_udf;
	if (request->req_database)
		an_udf = MET_get_udf(request->req_database, gpreGlob.token_global.tok_string);
	else {
		/* no database was specified, check the metadata for all the databases
		   for the existence of the udf */

		an_udf = NULL;
		for (DBB db = gpreGlob.isc_databases; db; db = db->dbb_next)
		{
			udf* tmp_udf = MET_get_udf(db, gpreGlob.token_global.tok_string);
			if (tmp_udf)
				if (an_udf) {
					// udf was found in more than one database 
					sprintf(s, "UDF %s is ambiguous", gpreGlob.token_global.tok_string);
					PAR_error(s);
				}
				else {
					an_udf = tmp_udf;
					request->req_database = db;
				}
		}
	}

	if (an_udf) {
		if ((SQL_DIALECT_V5 == gpreGlob.sw_sql_dialect) &&
			((dtype_sql_date == an_udf->udf_dtype) ||
			 (dtype_sql_time == an_udf->udf_dtype) ||
			 (dtype_int64 == an_udf->udf_dtype)))
		{
			SQL_dialect1_bad_type(an_udf->udf_dtype);
		}

		node = MSC_node(nod_udf, 2);
		node->nod_count = 1;
		node->nod_arg[1] = (GPRE_NOD) an_udf;
		PAR_get_token();
		EXP_left_paren(0);
		if (!(gpreGlob.token_global.tok_keyword == KW_RIGHT_PAREN)) {
			// parse udf parameter references 
			node->nod_arg[0] = SQE_list(SQE_value, request, false);

			if (an_udf->udf_args != node->nod_arg[0]->nod_count)
				PAR_error("count of UDF parameters doesn't match definition");

			// Match parameter types to the declared parameters 
			gpre_nod** input = node->nod_arg[0]->nod_arg;
			for (gpre_fld* field = an_udf->udf_inputs;
				 field;
				 input++, field = field->fld_next)
			{
				SQE_post_field(*input, field);
			}
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
	if (MSC_match(KW_GEN_ID)) {
		TEXT* gen_name = (TEXT *) MSC_alloc(NAME_SIZE);
		node = MSC_node(nod_gen_id, 2);
		node->nod_count = 1;
		EXP_left_paren(0);
		SQL_resolve_identifier("<Generator Name>", gen_name);
		node->nod_arg[1] = (GPRE_NOD) gen_name;
		PAR_get_token();
		if (!MSC_match(KW_COMMA))
			CPR_s_error("<comma>");
		node->nod_arg[0] = SQE_value(request, false, NULL, NULL);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

//  Check for DATE constants 
// ** Begin date/time/timesamp *
	if (MSC_match(KW_CURRENT_DATE))
		return MSC_node(nod_current_date, 0);
	else if (MSC_match(KW_CURRENT_TIME))
		return MSC_node(nod_current_time, 0);
	else if (MSC_match(KW_CURRENT_TIMESTAMP))
		return MSC_node(nod_current_timestamp, 0);

//  End date/time/timesamp *

//  Check for SQL II defined functions 

// ** Begin date/time/timesamp *
	if (MSC_match(KW_EXTRACT)) {
		node = MSC_node(nod_extract, 2);
		EXP_left_paren(0);
		KWWORDS kw_word = gpreGlob.token_global.tok_keyword;
		if (MSC_match(KW_YEAR) || MSC_match(KW_MONTH) || MSC_match(KW_DAY) ||
			MSC_match(KW_HOUR) || MSC_match(KW_MINUTE) || MSC_match(KW_SECOND) ||
			MSC_match(KW_WEEKDAY) || MSC_match(KW_YEARDAY))
		{
			node->nod_arg[0] = (gpre_nod*) kw_word;
			if (!MSC_match(KW_FROM))
				CPR_s_error("FROM");
		}
		else
			CPR_s_error("valid extract part");
		node->nod_arg[1] = SQE_value(request, false, NULL, NULL);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

//  End date/time/timesamp *

	if (MSC_match(KW_UPPER)) {
		node = MSC_node(nod_upcase, 1);
		EXP_left_paren(0);
		node->nod_arg[0] = SQE_value(request, false, NULL, NULL);
		local_count = 1;
		par_terminating_parens(&local_count, &local_count);
		return node;
	}

	if (MSC_match(KW_CAST)) {
		node = MSC_node(nod_cast, 2);
		node->nod_count = 1;
		EXP_left_paren(0);
		node->nod_arg[0] = SQE_value_or_null(request, false, 0, 0);
		if (!MSC_match(KW_AS))
			CPR_s_error("AS");
		gpre_fld* field = (gpre_fld*) MSC_alloc(FLD_LEN);
		node->nod_arg[1] = (GPRE_NOD) field;
		SQL_par_field_dtype(request, field, false);
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

static GPRE_NOD par_udf_or_field(gpre_req* request,
								 bool aster_ok)
{
	assert_IS_REQ(request);

	gpre_nod* node = par_udf(request);
	if (!node)
		node = SQE_field(request, aster_ok);

	return node;
}


//____________________________________________________________
//  
//		Parse a user defined function or a field name.
//		Allow the collate clause to follow.
//  

static GPRE_NOD par_udf_or_field_with_collate(gpre_req* request,
											  bool aster_ok,
											  USHORT * paren_count,
											  bool * bool_flag)

{
	assert_IS_REQ(request);

	gpre_nod* node = par_udf_or_field(request, aster_ok);
	if (gpreGlob.token_global.tok_keyword == KW_COLLATE)
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

static GPRE_NOD post_fields( GPRE_NOD node, map* to_map)
{
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
		return post_map(node, to_map);

	case nod_udf:
	case nod_gen_id:
		node->nod_arg[0] = post_fields(node->nod_arg[0], to_map);
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
		{
			gpre_nod** ptr = node->nod_arg;
			for (const gpre_nod* const* const end = ptr + node->nod_count;
				ptr < end; ptr++)
			{
				*ptr = post_fields(*ptr, to_map);
			}
			break;
		}
// ** Begin date/time/timestamp support *
	case nod_extract:
		node->nod_arg[1] = post_fields(node->nod_arg[1], to_map);
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

static GPRE_NOD post_map( GPRE_NOD node, map* to_map)
{
	MEL element;

	assert_IS_NOD(node);

//  Search existing map for equivalent expression.  If we find one,
//  return a reference to it. 

	if (node->nod_type == nod_map_ref) {
		element = (MEL) node->nod_arg[0];
		if (element->mel_context == to_map->map_context)
			return node;
	}

	for (element = to_map->map_elements; element; element = element->mel_next)
		if (compare_expr(node, element->mel_expr))
			return MSC_unary(nod_map_ref, (GPRE_NOD) element);

//  We need to make up a new map reference 

	element = (MEL) MSC_alloc(sizeof(mel));
	element->mel_next = to_map->map_elements;
	to_map->map_elements = element;
	element->mel_position = to_map->map_count++;
	element->mel_expr = node;
	element->mel_context = to_map->map_context;

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

static GPRE_NOD post_select_list( GPRE_NOD fields, map* to_map)
{
	assert_IS_NOD(fields);

	gpre_nod* list = MSC_node(nod_list, fields->nod_count);

	for (USHORT i = 0; i < fields->nod_count; i++)
		list->nod_arg[i] = post_map(fields->nod_arg[i], to_map);

	return list;
}


//____________________________________________________________
//  
//		Restore saved scoping information to the request block
//  

static void pop_scope(gpre_req* request, scope* save_scope)
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

static void push_scope(gpre_req* request, scope* save_scope)
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

static gpre_fld* resolve(
				   GPRE_NOD node,
				   gpre_ctx* context, gpre_ctx** found_context, act** slice_action)
{
	gpre_fld* field;

	assert_IS_NOD(node);

	gpre_rse* rs_stream = context->ctx_stream;
	if (rs_stream) {
		for (SSHORT i = 0; i < rs_stream->rse_count; i++)
			if (field =
				resolve(node, rs_stream->rse_context[i], found_context,
						slice_action))
			{
				return field;
			}

		return NULL;
	}

	TOK f_token = (TOK) node->nod_arg[0];
	TOK q_token = (TOK) node->nod_arg[1];

	if (!(context->ctx_relation || context->ctx_procedure))
		return NULL;

//  Handle unqualified fields first for simplicity 

	if (!q_token)
		field = MET_context_field(context, f_token->tok_string);
	else {
		// Now search alternatives for the qualifier 

		gpre_sym* symbol = HSH_lookup(q_token->tok_string);

		/* This caused gpre to dump core if there are lower case 
		   table aliases in a where clause used with dialect 2 or 3 

		   if ( (symbol == NULL) && (sw_case || gpreGlob.sw_sql_dialect == SQL_DIALECT_V5))
		   symbol = HSH_lookup2 (q_token->tok_string);
		 */

		/* So I replaced it with the following, don't know
		   why we don't do a HSH_lookup2 in any case, but so it may be.
		   FSG 16.Nov.2000
		 */
		if ((symbol == NULL))
			symbol = HSH_lookup2(q_token->tok_string);


		for (gpre_sym* temp_symbol = symbol; temp_symbol;
			 temp_symbol = temp_symbol->sym_homonym)
		{
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
			if ((gpre_rel*) symbol->sym_object == context->ctx_relation)
				field = MET_field(context->ctx_relation, f_token->tok_string);
		}
		else if (symbol->sym_type == SYM_procedure) {
			if ((gpre_prc*) symbol->sym_object == context->ctx_procedure)
				field = MET_context_field(context, f_token->tok_string);
		}
		else if (symbol->sym_type == SYM_context && symbol->sym_object == context)
			field = MET_context_field(context, f_token->tok_string);
	}


	if (field && found_context)
		*found_context = context;

//  Check for valid array field  
//  Check dimensions 
//  Set remaining fields for slice 
	slc* slice;
	gpre_req* slice_req = (gpre_req*) node->nod_arg[2];
	if (slice_req && (slice = slice_req->req_slice) && slice_action)
	{
		slice = slice_req->req_slice;
		ary* ary_info = field->fld_array_info;
		if (!ary_info)
			CPR_s_error("<array column>");
		if (ary_info->ary_dimension_count != slice->slc_dimensions)
			PAR_error("subscript count mismatch");
		slice->slc_field = field;
		slice->slc_parent_request = context->ctx_request;

		/* The action type maybe ACT_get_slice or ACT_put_slice 
		   set as a place holder */

		act* action = MSC_action(slice_req, ACT_get_slice);
		action->act_object = (REF) slice;
		*slice_action = action;
	}
	else if ((slice_req = (gpre_req*) node->nod_arg[2]) && slice_action) {
		/* The action type maybe ACT_get_slice or ACT_put_slice
		   set as a place holder */

		act* action = MSC_action(slice_req, ACT_get_slice);
		*slice_action = action;
	}

	return field;
}


//____________________________________________________________
//  
//		Attempt to resolve an asterisk in a context.
//		If successful, return the context.  Otherwise return NULL.
//  

static gpre_ctx* resolve_asterisk( TOK q_token, gpre_rse* selection)
{
	for (int i = 0; i < selection->rse_count; i++) {
		gpre_ctx* context = selection->rse_context[i];
		gpre_rse* rs_stream = context->ctx_stream;
		if (rs_stream) {
			if (context = resolve_asterisk(q_token, rs_stream))
				return context;
			continue;
		}
		gpre_sym* symbol = HSH_lookup(q_token->tok_string);
		for (; symbol; symbol = symbol->sym_homonym)
			if (symbol->sym_type == SYM_relation &&
				(gpre_rel*) symbol->sym_object == context->ctx_relation)
			{
				return context;
			}
			else if (symbol->sym_type == SYM_procedure &&
					 (gpre_prc*) symbol->sym_object == context->ctx_procedure)
			{
				return context;
			}
			else if (symbol->sym_type == SYM_context &&
					 (gpre_ctx*) symbol->sym_object == context)
			{
				return context;
			}
	}

	return NULL;
}


//____________________________________________________________
//  
//		Set field reference for any host variables in expr to field_ref.
//  

static void set_ref( GPRE_NOD expr, gpre_fld* field_ref)
{
	assert_IS_NOD(expr);

	ref* re = (REF) expr->nod_arg[0];
	switch (expr->nod_type) {
	case nod_value:
		re->ref_field = field_ref;
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
		{
            gpre_nod** ptr = expr->nod_arg;
			for (const gpre_nod* const* const end = ptr + expr->nod_count;
				ptr < end; ptr++)
			{
				set_ref(*ptr, field_ref);
			}
			break;
		}
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

static char* upcase_string(const char* p)
{
	USHORT l = 0;
	char* const s = (char *) MSC_alloc(strlen(p) + 1);
	char* q = s;

	char c;
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

static bool validate_references(GPRE_NOD fields,
								GPRE_NOD group_by)
{
	assert_IS_NOD(fields);
	assert_IS_NOD(group_by);

	if (!fields)
		return false;

	if (fields->nod_type == nod_field) {
		if (!group_by)
			return true;
		ref* fref = (REF) fields->nod_arg[0];

		bool context_match = false;
		gpre_nod** ptr = group_by->nod_arg;
		for (const gpre_nod* const* const end = ptr + group_by->nod_count;
			 ptr < end; ptr++)
		{
			ref* gref = (REF) (*ptr)->nod_arg[0];
			if (gref->ref_context == fref->ref_context) {
				if (gref->ref_field == fref->ref_field)
					return false;
				context_match = true;
			}
		}
		return context_match;
	}

	if (fields->nod_type == nod_agg_count ||
		fields->nod_type == nod_agg_max ||
		fields->nod_type == nod_agg_min ||
		fields->nod_type == nod_agg_total ||
		fields->nod_type == nod_agg_average ||
		fields->nod_type == nod_aggregate)
	{
		return false;
	}

	if (fields->nod_type == nod_any || fields->nod_type == nod_ansi_any ||
		fields->nod_type == nod_ansi_all) 
	{
		gpre_rse* any = (gpre_rse*) fields->nod_arg[0];
		return validate_references(any->rse_boolean, group_by);
	}

	if ((fields->nod_type == nod_gen_id) || (fields->nod_type == nod_udf))
		return validate_references(fields->nod_arg[0], group_by);

	bool invalid = false;
	gpre_nod** ptr = fields->nod_arg;
	for (const gpre_nod* const* const end = ptr + fields->nod_count; ptr < end;
		 ptr++)
	{
		switch ((*ptr)->nod_type) {
		case nod_map_ref:
			{
				mel* element = (MEL) (*ptr)->nod_arg[0];
				gpre_nod* node = element->mel_expr;
				if (node->nod_type != nod_agg_count &&
					node->nod_type != nod_agg_max &&
					node->nod_type != nod_agg_min &&
					node->nod_type != nod_agg_total &&
					node->nod_type != nod_agg_average &&
					node->nod_type != nod_aggregate)
				{
					invalid |= validate_references(node, group_by);
				}
				break;
			}

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
	}

	return invalid;
}

