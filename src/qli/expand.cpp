/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		expand.cpp
 *	DESCRIPTION:	Expand syntax tree -- first phase of compiler
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
#include <string.h>
#include "../qli/dtr.h"
#include "../qli/parse.h"
#include "../qli/compile.h"
#include "../qli/exe.h"
#include "../qli/report.h"
#include "../qli/all_proto.h"
#include "../qli/comma_proto.h"
#include "../qli/compi_proto.h"
#include "../qli/err_proto.h"
#include "../qli/expan_proto.h"
#include "../qli/help_proto.h"
#include "../qli/meta_proto.h"
#include "../qli/show_proto.h"

extern USHORT QLI_columns, QLI_lines;

#define MAKE_NODE(type, count)	make_node (type, count)

static bool compare_names(NAM, SYM);
static bool compare_symbols(SYM, SYM);
static SYM copy_symbol(SYM);
static void declare_global(QLI_FLD, SYN);
static SYN decompile_field(QLI_FLD, QLI_CTX);
static NAM decompile_symbol(SYM);
static QLI_NOD expand_assignment(SYN, LLS, LLS);
static QLI_NOD expand_any(SYN, LLS);
static QLI_NOD expand_boolean(SYN, LLS);
static void expand_control_break(BRK *, LLS);
static void expand_distinct(QLI_NOD, QLI_NOD);
static void expand_edit_string(QLI_NOD, ITM);
static QLI_NOD expand_erase(SYN, LLS, LLS);
static QLI_NOD expand_expression(SYN, LLS);
static QLI_NOD expand_field(SYN, LLS, SYN);
static QLI_NOD expand_for(SYN, LLS, LLS);
static QLI_NOD expand_function(SYN, LLS);
static QLI_NOD expand_group_by(SYN, LLS, QLI_CTX);
static QLI_NOD expand_modify(SYN, LLS, LLS);
static QLI_NOD expand_output(SYN, LLS, PRT *);
static QLI_NOD expand_print(SYN, LLS, LLS);
static ITM expand_print_item(SYN, LLS);
static QLI_NOD expand_print_list(SYN, LLS);
static QLI_NOD expand_report(SYN, LLS, LLS);
static QLI_NOD expand_restructure(SYN, LLS, LLS);
static QLI_NOD expand_rse(SYN, LLS *);
static QLI_NOD expand_sort(SYN, LLS, QLI_NOD);
static QLI_NOD expand_statement(SYN, LLS, LLS);
static QLI_NOD expand_store(SYN, LLS, LLS);
static void expand_values(SYN, LLS);
static QLI_CTX find_context(NAM, LLS);
static int generate_fields(QLI_CTX, LLS, SYN);
static int generate_items(SYN, LLS, LLS, QLI_NOD);
static bool global_agg(SYN, SYN);
static bool invalid_nod_field(QLI_NOD, QLI_NOD);
static bool invalid_syn_field(SYN, SYN);
static QLI_NOD make_and(QLI_NOD, QLI_NOD);
static QLI_NOD make_assignment(QLI_NOD, QLI_NOD, LLS);
static QLI_NOD make_field(QLI_FLD, QLI_CTX);
static QLI_NOD make_list(LLS);
static QLI_NOD make_node(NOD_T, USHORT);
static QLI_NOD negate(QLI_NOD);
static QLI_NOD possible_literal(SYN, LLS, bool);
static QLI_NOD post_map(QLI_NOD, QLI_CTX);
static QLI_FLD resolve(SYN, LLS, QLI_CTX *);
static void resolve_really(QLI_FLD, SYN);

static LLS global_output_stack;


QLI_NOD EXP_expand( SYN node)
{
/**************************************
 *
 *	E X P _ e x p a n d
 *
 **************************************
 *
 * Functional description
 *	Expand a syntax tree into something richer and more complete.
 *
 **************************************/
	switch (node->syn_type) {
	case nod_commit:
	case nod_prepare:
	case nod_rollback:
		CMD_transaction(node);
		return NULL;

	case nod_copy_proc:
		CMD_copy_procedure(node);
		return NULL;

	case nod_declare:
		declare_global((QLI_FLD) node->syn_arg[0], node->syn_arg[1]);
		return NULL;

	case nod_define:
		CMD_define_procedure(node);
		return NULL;

	case nod_delete_proc:
		CMD_delete_proc(node);
		return NULL;

	case nod_def_database:
	case nod_sql_database:
		MET_ready(node, TRUE);
		return NULL;

	case nod_def_field:
		MET_define_field((DBB) node->syn_arg[0], (QLI_FLD) node->syn_arg[1]);
		return NULL;

	case nod_def_index:
		MET_define_index(node);
		return NULL;

	case nod_def_relation:
		MET_define_relation((QLI_REL)node->syn_arg[0], (QLI_REL) node->syn_arg[1]);
		return NULL;

	case nod_del_relation:
		MET_delete_relation((QLI_REL)node->syn_arg[0]);
		return NULL;

	case nod_del_field:
		MET_delete_field((DBB)node->syn_arg[0], (NAM) node->syn_arg[1]);
		return NULL;

	case nod_del_index:
		MET_delete_index((DBB)node->syn_arg[0], (NAM) node->syn_arg[1]);
		return NULL;

	case nod_del_database:
		MET_delete_database((DBB)node->syn_arg[0]);
		return NULL;

	case nod_edit_proc:
		CMD_edit_proc(node);
		return NULL;
	case nod_extract:
		node->syn_arg[1] = (SYN) expand_output(node->syn_arg[1], 0, 0);
		CMD_extract(node);
		return NULL;

	case nod_finish:
		CMD_finish(node);
		return NULL;

	case nod_help:
		HELP_help(node);
		return NULL;

	case nod_mod_field:
		MET_modify_field((DBB)node->syn_arg[0], (QLI_FLD) node->syn_arg[1]);
		return NULL;

	case nod_mod_relation:
		MET_modify_relation((QLI_REL) node->syn_arg[0], (QLI_FLD) node->syn_arg[1]);
		return NULL;

	case nod_mod_index:
		MET_modify_index(node);
		return NULL;

	case nod_ready:
		MET_ready(node, FALSE);
		return NULL;

	case nod_rename_proc:
		CMD_rename_proc(node);
		return NULL;

	case nod_set:
		CMD_set(node);
		return NULL;

	case nod_show:
		SHOW_stuff(node);
		return NULL;

	case nod_shell:
		CMD_shell(node);
		return NULL;

	case nod_sql_grant:
		MET_sql_grant(node);
		return NULL;

	case nod_sql_revoke:
		MET_sql_revoke(node);
		return NULL;

	case nod_sql_cr_table:
		MET_define_sql_relation((QLI_REL) node->syn_arg[0]);
		return NULL;

/****
    case nod_sql_cr_view:
	MET_sql_cr_view (node);
	GEN_release();
	return NULL;
****/

	case nod_sql_al_table:
		MET_sql_alter_table((QLI_REL) node->syn_arg[0], (QLI_FLD) node->syn_arg[1]);
		return NULL;
	} // end switch, no default case for error

// If there are any variables, make up a context now

	LLS right, left;
	global_output_stack = right = left = NULL;

	if (QLI_variables) {
		QLI_CTX context = (QLI_CTX) ALLOCD(type_ctx);
		context->ctx_type = CTX_VARIABLE;
		context->ctx_variable = QLI_variables;
		LLS_PUSH(context, &right);
		LLS_PUSH(context, &left);
	}

	QLI_NOD expanded = expand_statement(node, right, left);
	if (!expanded)
		return NULL;

	while (global_output_stack) {
		QLI_NOD output = (QLI_NOD) LLS_POP(&global_output_stack);
		output->nod_arg[e_out_statement] = expanded;
		expanded = output;
	}

	return expanded;
}


static bool compare_names( NAM name, SYM symbol)
{
/**************************************
 *
 *	c o m p a r e _ n a m e s
 *
 **************************************
 *
 * Functional description
 *	Compare a name node to a symbol.  If they are equal, return true.
 *
 **************************************/
	USHORT l;

	if (!symbol || (l = name->nam_length) != symbol->sym_length)
		return false;

	if (l)
	{
		const TEXT* p = symbol->sym_string;
		const TEXT* q = name->nam_string;

		do {
			if (*p++ != *q++)
				return false;
		} while (--l);
	}

	return true;
}


static bool compare_symbols( SYM symbol1, SYM symbol2)
{
/**************************************
 *
 *	c o m p a r e _ s y m b o l s
 *
 **************************************
 *
 * Functional description
 *	Compare two symbols (either may be 0).
 *
 **************************************/
	if (!symbol1 || !symbol2)
		return false;

	USHORT l = symbol1->sym_length;
	if (l != symbol2->sym_length)
		return false;

	if (l)
	{
		const TEXT* p = symbol1->sym_string;
		const TEXT* q = symbol2->sym_string;

		do {
			if (*p++ != *q++)
				return false;
		} while (--l);
	}

	return true;
}


static SYM copy_symbol( SYM old)
{
/**************************************
 *
 *	c o p y _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Copy a symbol into the permanent pool.
 *
 **************************************/
	SYM new_sym = (SYM) ALLOCPV(type_sym, old->sym_length);
	new_sym->sym_length = old->sym_length;
	new_sym->sym_type = old->sym_type;
	new_sym->sym_string = new_sym->sym_name;
	strcpy(new_sym->sym_name, old->sym_name);

	return new_sym;
}


static void declare_global( QLI_FLD variable, SYN field_node)
{
/**************************************
 *
 *	d e c l a r e _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Copy a variable block into the permanent pool as a field.
 *	Resolve BASED_ON references.
 *	Allocate all strings off the field block.
 *
 **************************************/
	QLI_FLD field, *ptr;

// If it's based_on, flesh it out & check datatype.

	if (field_node) {
		if (field_node->syn_type == nod_index)
			field_node = field_node->syn_arg[s_idx_field];
		resolve_really(variable, field_node);
		if (variable->fld_dtype == dtype_blob)
			IBERROR(137);		// Msg137 variables may not be based on blob fields.
	}

// Get rid of any other variables of the same name

	for (ptr = &QLI_variables; field = *ptr; ptr = &field->fld_next)
		if (!strcmp
			(field->fld_name->sym_string, variable->fld_name->sym_string))
		{
			*ptr = field->fld_next;
			ALLQ_release((FRB) field->fld_name);
			if (field->fld_query_name)
				ALLQ_release((FRB) field->fld_query_name);
			ALLQ_release((FRB) field);
			break;
		}

// Next, copy temporary field block into permanent pool.  Fold edit_string
//   query_header into main block to save space and complexity.

	const TEXT* q;
	USHORT l = variable->fld_length;
	if (q = variable->fld_edit_string)
		l += strlen(q);
	if (q = variable->fld_query_header)
		l += strlen(q);

	QLI_FLD new_fld = (QLI_FLD) ALLOCPV(type_fld, l);
	new_fld->fld_name = copy_symbol(variable->fld_name);
	new_fld->fld_dtype = variable->fld_dtype;
	new_fld->fld_length = variable->fld_length;
	new_fld->fld_scale = variable->fld_scale;
	new_fld->fld_sub_type = variable->fld_sub_type;
	new_fld->fld_sub_type_missing = variable->fld_sub_type_missing;
	new_fld->fld_flags = variable->fld_flags | FLD_missing;

// Copy query_name, edit string, query header

	TEXT* p = (TEXT*) new_fld->fld_data + new_fld->fld_length;
	if (q = variable->fld_edit_string) {
		new_fld->fld_edit_string = p;
		while (*p++ = *q++);
	}
	if (variable->fld_query_name)
		new_fld->fld_query_name = copy_symbol(variable->fld_query_name);
	if (q = variable->fld_query_header) {
		new_fld->fld_query_header = p;
		while (*p++ = *q++);
	}

// Link new variable into variable chain

	new_fld->fld_next = QLI_variables;
	QLI_variables = new_fld;
}


static SYN decompile_field( QLI_FLD field, QLI_CTX context)
{
/**************************************
 *
 *	d e c o m p i l e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Take a perfectly good, completely compiled
 *	field block and regress to a SYN node and
 *	and a NAM block.
 *	(Needed to support SQL idiocies)
 *
 **************************************/
	int args = (context) ? 2 : 1;

	SYN node = (SYN) ALLOCDV(type_syn, args);
	node->syn_type = nod_field;
	node->syn_count = args;

	NAM name = decompile_symbol(field->fld_name);
	node->syn_arg[0] = (SYN) name;

	if (context) {
		node->syn_arg[1] = node->syn_arg[0];
		if (context->ctx_symbol)
			name = decompile_symbol(context->ctx_symbol);
		else
			name = decompile_symbol(context->ctx_relation->rel_symbol);
		node->syn_arg[0] = (SYN) name;
	}

	return node;
}


static NAM decompile_symbol( SYM symbol)
{
/**************************************
 *
 *	d e c o m p i l e _ s y m b o l
 *
 **************************************
 *
 * Functional description
 *	Turn a symbol back into a name
 *	(Needed to support SQL idiocies)
 *
 **************************************/
	NAM name;
	int l;
	TEXT *p, *q, c;

	l = symbol->sym_length;

	name = (NAM) ALLOCDV(type_nam, l);
	name->nam_length = l;
	name->nam_symbol = symbol;

	p = name->nam_string;
	q = symbol->sym_string;

	if (l)
		do {
			c = *q++;
			*p++ = UPPER(c);

		} while (--l);

	return name;
}


static QLI_NOD expand_assignment( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ a s s i g n m e n t
 *
 **************************************
 *
 * Functional description
 *	Expand an assigment statement.  All in all, not too tough.
 *
 **************************************/
	QLI_NOD from, to;

	QLI_NOD node = MAKE_NODE(input->syn_type, e_asn_count);
	node->nod_arg[e_asn_to] = to =
		expand_expression(input->syn_arg[s_asn_to], left);
	node->nod_arg[e_asn_from] = from =
		expand_expression(input->syn_arg[s_asn_from], right);

	if (to->nod_type == nod_field || to->nod_type == nod_variable) {
		QLI_FLD field = (QLI_FLD) to->nod_arg[e_fld_field];
		if (field->fld_flags & FLD_computed) {
			ERRQ_print_error(138, field->fld_name->sym_string, NULL, NULL,
							 NULL, NULL);
			// Msg138 can't do assignment to computed field
		}
		if (from->nod_type == nod_prompt)
			from->nod_arg[e_prm_field] = to->nod_arg[e_fld_field];
		if (field->fld_validation)
			node->nod_arg[e_asn_valid] =
				expand_expression(field->fld_validation, left);
	}

	if (!node->nod_arg[e_asn_valid])
		--node->nod_count;

	return node;
}


static QLI_NOD expand_any( SYN input, LLS stack)
{
/**************************************
 *
 *	e x p a n d _ a n y
 *
 **************************************
 *
 * Functional description
 *	Expand an any expression.  This would be trivial were it not
 *	for a funny SQL case when an expression needs to be checked
 *	for existence.
 *
 **************************************/
	QLI_NOD rse;

	QLI_NOD node = MAKE_NODE(input->syn_type, e_any_count);
	node->nod_count = 0;
	node->nod_arg[e_any_rse] = rse = expand_rse(input->syn_arg[0], &stack);

	if (input->syn_count >= 2 && input->syn_arg[1]) {
		QLI_NOD boolean = MAKE_NODE(nod_missing, 1);
		boolean->nod_arg[0] = expand_expression(input->syn_arg[1], stack);
		QLI_NOD negation = MAKE_NODE(nod_not, 1);
		negation->nod_arg[0] = boolean;
		rse->nod_arg[e_rse_boolean] =
			make_and(rse->nod_arg[e_rse_boolean], negation);
	}

	return node;
}


static QLI_NOD expand_boolean( SYN input, LLS stack)
{
/**************************************
 *
 *	e x p a n d _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	QLI_NOD value;
	SSHORT i;

// Make node and process arguments

	QLI_NOD node = MAKE_NODE(input->syn_type, input->syn_count);
	QLI_NOD* ptr = node->nod_arg;
	*ptr++ = value = expand_expression(input->syn_arg[0], stack);

	for (i = 1; i < input->syn_count; i++, ptr++)
		if (!(*ptr = possible_literal(input->syn_arg[i], stack, true)))
			*ptr = expand_expression(input->syn_arg[i], stack);

// Try to match any prompts against fields to determine prompt length

	if (value->nod_type != nod_field)
		return node;

	QLI_FLD field = (QLI_FLD) value->nod_arg[e_fld_field];
	ptr = &node->nod_arg[1];

	for (i = 1; i < node->nod_count; i++, ptr++)
		if ((*ptr)->nod_type == nod_prompt)
			(*ptr)->nod_arg[e_prm_field] = (QLI_NOD) field;

	return node;
}


static void expand_control_break( BRK* ptr, LLS right)
{
/**************************************
 *
 *	e x p a n d _ c o n t r o l _ b r e a k
 *
 **************************************
 *
 * Functional description
 *	Work on a report writer control break.  This is called recursively
 *	to handle multiple breaks.
 *
 **************************************/
	BRK control;

	BRK list = NULL;

	while (control = *ptr) {
		*ptr = control->brk_next;
		control->brk_next = list;
		list = control;
		if (control->brk_field)
			control->brk_field =
				(SYN) expand_expression(control->brk_field, right);
		if (control->brk_line)
			control->brk_line =
				(SYN) expand_print_list(control->brk_line, right);
	}

	*ptr = list;
}


static void expand_distinct( QLI_NOD rse, QLI_NOD node)
{
/**************************************
 *
 *	e x p a n d _ d i s t i n c t
 *
 **************************************
 *
 * Functional description
 *	We have run into a distinct count.  Add a reduced
 *	clause to it's parent.
 *
 **************************************/
	QLI_NOD list;

	if (rse->nod_arg[e_rse_reduced])
		return;

	LLS stack = NULL;
	LLS_PUSH(node, &stack);
	LLS_PUSH(0, &stack);
	rse->nod_arg[e_rse_reduced] = list = make_list(stack);
	list->nod_count = 1;
}


static void expand_edit_string( QLI_NOD node, ITM item)
{
/**************************************
 *
 *	e x p a n d _ e d i t _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Default edit_string and query_header.
 *
 **************************************/
	FUN function;
	QLI_MAP map;

	switch (node->nod_type) {
	case nod_min:
	case nod_rpt_min:
	case nod_agg_min:
		if (!item->itm_query_header)
			item->itm_query_header = "MIN";

	case nod_total:
	case nod_running_total:
	case nod_rpt_total:
	case nod_agg_total:
		if (!item->itm_query_header)
			item->itm_query_header = "TOTAL";

	case nod_average:
	case nod_rpt_average:
	case nod_agg_average:
		if (!item->itm_query_header)
			item->itm_query_header = "AVG";

	case nod_max:
	case nod_rpt_max:
	case nod_agg_max:
		if (!item->itm_query_header)
			item->itm_query_header = "MAX";
		expand_edit_string(node->nod_arg[e_stt_value], item);
		return;

	case nod_count:
	case nod_running_count:
	case nod_rpt_count:
	case nod_agg_count:
		if (!item->itm_edit_string)
			item->itm_edit_string = "ZZZ,ZZZ,ZZ9";
		if (!item->itm_query_header)
			item->itm_query_header = "COUNT";
		break;

	case nod_map:
		map = (QLI_MAP) node->nod_arg[e_map_map];
		expand_edit_string(map->map_node, item);
		return;

	case nod_field:
	case nod_variable:
		break;

	case nod_function:
		function = (FUN) node->nod_arg[e_fun_function];
		if (!item->itm_query_header)
			item->itm_query_header = function->fun_symbol->sym_string;
		return;

	default:
		return;
	}

// Handle fields

	QLI_FLD field = (QLI_FLD) node->nod_arg[e_fld_field];

	if (!item->itm_edit_string)
		item->itm_edit_string = field->fld_edit_string;

	if (!item->itm_query_header)
		if (!(item->itm_query_header = field->fld_query_header))
			item->itm_query_header = field->fld_name->sym_string;
}


static QLI_NOD expand_erase( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	QLI_NOD loop = NULL;
	USHORT count = 0;

// If there is an rse, make up a FOR loop

	if (input->syn_arg[s_era_rse]) {
		loop = MAKE_NODE(nod_for, e_for_count);
		loop->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_era_rse],
											  &right);
	}

// Loop thru contexts counting them.

	QLI_CTX context = NULL;
	for (LLS contexts = right; contexts; contexts = contexts->lls_next) {
		context = (QLI_CTX) contexts->lls_object;
		if (context->ctx_variable)
			continue;
		count++;
		if (context->ctx_rse)
			break;
	}

	if (count == 0)
		IBERROR(139);			// Msg139 no context for ERASE
	else if (count > 1)
		IBERROR(140);			// Msg140 can't erase from a join

// Make up node big enough to hold fixed fields plus all contexts

	QLI_NOD node = MAKE_NODE(nod_erase, e_era_count);
	node->nod_arg[e_era_context] = (QLI_NOD) context;

	if (!loop)
		return node;

	loop->nod_arg[e_for_statement] = node;

	return loop;
}


static QLI_NOD expand_expression( SYN input, LLS stack)
{
/**************************************
 *
 *	e x p a n d _ e x p r e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Expand an expression.
 *
 **************************************/
	QLI_NOD node;
	CON constant;
	QLI_CTX context;
	NAM name;
	SYN value;

	switch (input->syn_type) {
	case nod_field:
		return expand_field(input, stack, 0);

	case nod_null:
	case nod_user_name:
		return MAKE_NODE(input->syn_type, 0);

	case nod_any:
	case nod_unique:
		return expand_any(input, stack);

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_average:
	case nod_total:
	case nod_from:

	case nod_rpt_max:
	case nod_rpt_min:
	case nod_rpt_count:
	case nod_rpt_average:
	case nod_rpt_total:

	case nod_running_total:
	case nod_running_count:
		node = MAKE_NODE(input->syn_type, e_stt_count);
		if (value = input->syn_arg[s_stt_rse])
			node->nod_arg[e_stt_rse] = expand_rse(value, &stack);
		if (value = input->syn_arg[s_stt_value])
			node->nod_arg[e_stt_value] = expand_expression(value, stack);
		if (value = input->syn_arg[s_stt_default])
			node->nod_arg[e_stt_default] = expand_expression(value, stack);
		if (input->syn_arg[s_prt_distinct] && node->nod_arg[e_stt_rse]
			&& node->nod_arg[e_stt_value])
			expand_distinct(node->nod_arg[e_stt_rse],
							node->nod_arg[e_stt_value]);
// count2 next 2 lines go
		if (input->syn_type == nod_count)
			node->nod_arg[e_stt_value] = 0;
		return node;

	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_count:
	case nod_agg_average:
	case nod_agg_total:
		node = MAKE_NODE(input->syn_type, e_stt_count);
		for (; stack; stack = stack->lls_next) {
			context = (QLI_CTX) stack->lls_object;
			if (context->ctx_type == CTX_AGGREGATE)
				break;
		}
		if (!stack)
			ERRQ_print_error(454, NULL, NULL, NULL, NULL, NULL);
			// could not resolve context for aggregate
/* count2
	if (value = input->syn_arg [s_stt_value])
	    {
	    node->nod_arg [e_stt_value] = expand_expression (value, stack->lls_next);
	    if (input->syn_arg [s_prt_distinct])
		expand_distinct (context->ctx_sub_rse, node->nod_arg [e_stt_value]);
	    }
*/
		if ((value = input->syn_arg[s_stt_value]) &&
			(input->syn_arg[s_prt_distinct]
			 || (input->syn_type != nod_agg_count))) {
			node->nod_arg[e_stt_value] =
				expand_expression(value, stack->lls_next);
			if (input->syn_arg[s_prt_distinct]
				|| (input->syn_type == nod_agg_count && context->ctx_sub_rse))
				expand_distinct(context->ctx_sub_rse,
								node->nod_arg[e_stt_value]);
		}
		return post_map(node, context);

	case nod_index:
		value = input->syn_arg[s_idx_field];
		if (value->syn_type != nod_field)
			IBERROR(466);		// Msg466 Only fields may be subscripted
		return expand_field(value, stack, input->syn_arg[s_idx_subs]);

	case nod_list:
	case nod_upcase:

	case nod_and:
	case nod_or:
	case nod_not:
	case nod_missing:
	case nod_add:
	case nod_subtract:
	case nod_multiply:
	case nod_divide:
	case nod_negate:
	case nod_concatenate:
	case nod_substr:
		break;

	case nod_eql:
	case nod_neq:
	case nod_gtr:
	case nod_geq:
	case nod_leq:
	case nod_lss:
	case nod_between:
	case nod_matches:
	case nod_sleuth:
	case nod_like:
	case nod_starts:
	case nod_containing:
		return expand_boolean(input, stack);

	case nod_edit_blob:
		node = MAKE_NODE(input->syn_type, e_edt_count);
		node->nod_count = 0;
		if (input->syn_arg[0]) {
			node->nod_count = 1;
			node->nod_arg[0] = expand_expression(input->syn_arg[0], stack);
		}
		return node;

	case nod_format:
		node = MAKE_NODE(input->syn_type, e_fmt_count);
		node->nod_count = 1;
		node->nod_arg[e_fmt_value] =
			expand_expression(input->syn_arg[s_fmt_value], stack);
		node->nod_arg[e_fmt_edit] = (QLI_NOD) input->syn_arg[s_fmt_edit];
		return node;

	case nod_function:
		return expand_function(input, stack);

	case nod_constant:
		node = MAKE_NODE(input->syn_type, 0);
		constant = (CON) input->syn_arg[0];
		node->nod_desc = constant->con_desc;
		return node;

	case nod_prompt:
		node = MAKE_NODE(input->syn_type, e_prm_count);
		node->nod_arg[e_prm_prompt] = (QLI_NOD) input->syn_arg[0];
		return node;

	case nod_star:
		name = (NAM) input->syn_arg[0];
		ERRQ_print_error(141, name->nam_string, NULL, NULL, NULL, NULL);
		// Msg141 can't be used when a single element is required

	default:
		BUGCHECK(135);			// Msg135 expand_expression: not yet implemented
	}

	node = MAKE_NODE(input->syn_type, input->syn_count);
	QLI_NOD* ptr = node->nod_arg;

	for (SSHORT i = 0; i < input->syn_count; i++)
		*ptr++ = expand_expression(input->syn_arg[i], stack);

	return node;
}


static QLI_NOD expand_field( SYN input, LLS stack, SYN subs)
{
/**************************************
 *
 *	e x p a n d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Expand a field reference.  Error if it can't be resolved.
 *	If the field belongs to a group by SQL expression, make
 *	sure it goes there.
 *
 **************************************/
	QLI_CTX context;

	QLI_FLD field = resolve(input, stack, &context);
	if (!field || (subs && (context->ctx_variable)))
	{
	    TEXT s[160];
		TEXT* p = s;
		const TEXT* const limit = p + sizeof(s) - 1;
		for (USHORT i = 0; i < input->syn_count; i++) {
			NAM name = (NAM) input->syn_arg[i];
			const TEXT* q = name->nam_string;
			USHORT l = name->nam_length;
			if (p < limit) {
				if (l)
					do {
						*p++ = *q++;
					} while (--l && p < limit);
				*p++ = '.';
			}
		}
		*--p = 0;
		if (field)
			ERRQ_print_error(467, s, NULL, NULL, NULL, NULL);
			// Msg467 "%s" is not a field and so may not be subscripted
		else
			ERRQ_print_error(142, s, NULL, NULL, NULL, NULL);
			// Msg142 "%s" is undefined or used out of context
	}

	QLI_NOD node = make_field(field, context);
	if (subs)
		node->nod_arg[e_fld_subs] = expand_expression(subs, stack);

	QLI_CTX parent = NULL;
	LLS save_stack;
	for (save_stack = stack; stack; stack = stack->lls_next) {
		parent = (QLI_CTX) stack->lls_object;
		if (parent->ctx_type == CTX_AGGREGATE)
			break;
	}

	if (!parent)
		return node;
	else if (context->ctx_parent != parent) {
		/* The parent context may be hidden because we are part of
		   a stream context.  Check out this possibility. */

		for (; save_stack; save_stack = save_stack->lls_next) {
			QLI_CTX stream_context = (QLI_CTX) save_stack->lls_object;
			if (stream_context->ctx_type != CTX_STREAM ||
				stream_context->ctx_stream->nod_type != nod_rse)
				continue;

			QLI_CTX* ptr =
				(QLI_CTX*) stream_context->ctx_stream->nod_arg + e_rse_count;
			QLI_CTX* const end = ptr + stream_context->ctx_stream->nod_count;
			for (; ptr < end; ptr++)
				if (*ptr == context)
					break;
			if (ptr < end && stream_context->ctx_parent == parent)
				break;
		}

		if (!save_stack)
			return node;
	}

	return post_map(node, parent);
}


static QLI_NOD expand_for( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ f o r
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	QLI_NOD node = MAKE_NODE(input->syn_type, e_for_count);
	node->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_for_rse], &right);
	node->nod_arg[e_for_statement] =
		expand_statement(input->syn_arg[s_for_statement], right, left);

	return node;
}


static QLI_NOD expand_function( SYN input, LLS stack)
{
/**************************************
 *
 *	e x p a n d _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Expand a functionn reference.
 *	For a field or expression reference
 *	tied to a relation in a database
 *	use only that database.  For a variable
 *      reference, use any database that matches.
 *
 **************************************/
	SYM symbol;
	FUN function;
	QLI_CTX context;
	DBB database;

	QLI_NOD node = MAKE_NODE(input->syn_type, e_fun_count);
	node->nod_count = 1;
	if (stack && (context = (QLI_CTX) stack->lls_object)
		&& (context->ctx_type == CTX_RELATION)) {
		if (context->ctx_primary)
			context = context->ctx_primary;
		database = context->ctx_relation->rel_database;
		for (symbol = (SYM) input->syn_arg[s_fun_function]; symbol;
			 symbol = symbol->sym_homonym)
		{
			if (symbol->sym_type == SYM_function) {
				function = (FUN) symbol->sym_object;
				if (function->fun_database == database)
					break;
			}
		}
	}
	else
		for (database = QLI_databases; database;
			 database = database->dbb_next)
		{
			for (symbol = (SYM) input->syn_arg[s_fun_function]; symbol;
				 symbol = symbol->sym_homonym)
			{
				if (symbol->sym_type == SYM_function) {
					function = (FUN) symbol->sym_object;
					if (function->fun_database == database)
						break;
				}
			}
			if (symbol)
				break;
		}


	if (!symbol) {
		symbol = (SYM) input->syn_arg[s_fun_function];
		ERRQ_error(412, symbol->sym_string, database->dbb_filename, NULL,
				   NULL, NULL);
	}

	node->nod_arg[e_fun_function] = (QLI_NOD) function;

	node->nod_arg[e_fun_args] =
		expand_expression(input->syn_arg[s_fun_args], stack);

	return node;
}


static QLI_NOD expand_group_by( SYN input, LLS stack, QLI_CTX context)
{
/**************************************
 *
 *	e x p a n d _ g r o u p _ b y
 *
 **************************************
 *
 * Functional description
 *	Expand a GROUP BY clause.
 *
 **************************************/
	SYN *ptr, *end;

	QLI_NOD node = MAKE_NODE(input->syn_type, input->syn_count);
	QLI_NOD* ptr2 = node->nod_arg;

	for (ptr = input->syn_arg, end = ptr + input->syn_count; ptr < end;
		 ptr++, ptr2++)
	{
		*ptr2 = expand_expression(*ptr, stack);
		post_map(*ptr2, context);
	}

	return node;
}


static QLI_NOD expand_modify( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	LLS contexts;

	QLI_NOD loop = NULL;
	USHORT count = 0;

// If there is an rse, make up a FOR loop

	if (input->syn_arg[s_mod_rse]) {
		loop = MAKE_NODE(nod_for, e_for_count);
		loop->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_mod_rse],
											  &right);
	}

// Loop thru contexts counting them.

	for (contexts = right; contexts; contexts = contexts->lls_next) {
		QLI_CTX context = (QLI_CTX) contexts->lls_object;
		if (context->ctx_variable)
			continue;
		count++;
		if (context->ctx_rse)
			break;
	}

	if (!count)
		IBERROR(148);			// Msg148 no context for modify

// Make up node big enough to hold fixed fields plus all contexts

	QLI_NOD node = MAKE_NODE(nod_modify, (int) e_mod_count + count);
	node->nod_count = count;
	QLI_NOD* ptr = &node->nod_arg[e_mod_count];

// Loop thru contexts augmenting left context

	for (contexts = right; contexts; contexts = contexts->lls_next) {
		QLI_CTX context = (QLI_CTX) contexts->lls_object;
		if (context->ctx_variable)
			continue;
		QLI_CTX new_context = (QLI_CTX) ALLOCD(type_ctx);
		*ptr++ = (QLI_NOD) new_context;
		new_context->ctx_type = CTX_RELATION;
		new_context->ctx_source = context;
		new_context->ctx_symbol = context->ctx_symbol;
		new_context->ctx_relation = context->ctx_relation;
		LLS_PUSH(new_context, &left);
		if (context->ctx_rse)
			break;
	}

// Process sub-statement, list of fields, or, sigh, none of the above

	SYN syn_list;
	if (input->syn_arg[s_mod_statement])
		node->nod_arg[e_mod_statement] =
			expand_statement(input->syn_arg[s_mod_statement], right, left);
	else if (syn_list = input->syn_arg[s_mod_list]) {
		QLI_NOD list = MAKE_NODE(nod_list, syn_list->syn_count);
		node->nod_arg[e_mod_statement] = list;

		ptr = list->nod_arg;
		SYN* syn_ptr = syn_list->syn_arg;
		for (USHORT i = 0; i < syn_list->syn_count; i++, syn_ptr++)
		{
			*ptr++ = make_assignment(expand_expression((SYN) *syn_ptr, left),
									(QLI_NOD) *syn_ptr, right);
		}
	}
	else
		IBERROR(149);			// Msg149 field list required for modify

	if (!loop)
		return node;

	loop->nod_arg[e_for_statement] = node;

	return loop;
}


static QLI_NOD expand_output( SYN input, LLS right, PRT * print)
{
/**************************************
 *
 *	e x p a n d _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Handle the presence (or absence) of an output specification clause.
 *
 **************************************/
	if (print)
		*print = (PRT) ALLOCD(type_prt);

	if (!input)
		return NULL;

	QLI_NOD output = MAKE_NODE(nod_output, e_out_count);
	LLS_PUSH(output, &global_output_stack);

    QLI_NOD node = possible_literal(input->syn_arg[s_out_file], right, false);
	if (!node)
		node = expand_expression(input->syn_arg[s_out_file], right);

	output->nod_arg[e_out_file] = node;
	output->nod_arg[e_out_pipe] = (QLI_NOD) input->syn_arg[s_out_pipe];

	if (print)
		output->nod_arg[e_out_print] = (QLI_NOD) * print;

	return output;
}


static QLI_NOD expand_print( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	SYN syn_rse = input->syn_arg[s_prt_rse];

	QLI_NOD loop = NULL;
	LLS items = NULL;
	QLI_NOD rse = NULL;
	USHORT count = 0;
	LLS new_right = right;

// If an output file or pipe is present, make up an output node

	PRT print;
	expand_output(input->syn_arg[s_prt_output], right, &print);

// If a record select expression is present, expand it and build a FOR
// statement.

	if (syn_rse) {
		loop = MAKE_NODE(nod_for, e_for_count);
		loop->nod_arg[e_for_rse] = rse = expand_rse(syn_rse, &new_right);
	}

/* If there were any print items, process them now.  Look first for things that
   look like items, but are actually lists.  If there aren't items of any kind,
   pick up all fields in the relations from the record selection expression. */

	SYN syn_item, *sub, *end;
	SYN syn_list = input->syn_arg[s_prt_list];
	if (syn_list)
		for (sub = syn_list->syn_arg, end = sub + syn_list->syn_count;
			 sub < end; sub++)
		{
			if (((*sub)->syn_type == nod_print_item)
				&& (syn_item = (*sub)->syn_arg[s_itm_value])
				&& (syn_item->syn_type == nod_star))
			{
				count += generate_items(syn_item, new_right, (LLS) &items, rse);
			}
			else {
				LLS_PUSH(expand_print_item(*sub, new_right), &items);
				count++;
			}
		}
	else if (syn_rse && (syn_list = syn_rse->syn_arg[s_rse_reduced]))
		for (sub = syn_list->syn_arg, end = sub + syn_list->syn_count;
			 sub < end; sub += 2)
		{
			ITM item = (ITM) ALLOCD(type_itm);
			item->itm_type = item_value;
			item->itm_value = expand_expression(*sub, new_right);
			expand_edit_string(item->itm_value, item);
			LLS_PUSH(item, &items);
			count++;
		}
	else
		for (; new_right; new_right = new_right->lls_next) {
			QLI_CTX context = (QLI_CTX) new_right->lls_object;
			QLI_REL relation = context->ctx_relation;
			if (!relation || context->ctx_sub_rse)
				continue;
			for (QLI_FLD field = relation->rel_fields; field;
				field = field->fld_next)
			{
				if (
					(field->fld_system_flag
					 && field->fld_system_flag != relation->rel_system_flag)
					|| field->fld_flags & FLD_array)
				{
					continue;
				}
				QLI_NOD node = make_field(field, context);
				if (rse && rse->nod_arg[e_rse_group_by] &&
					invalid_nod_field(node, rse->nod_arg[e_rse_group_by]))
					continue;
				ITM item = (ITM) ALLOCD(type_itm);
				item->itm_type = item_value;
				item->itm_value = make_field(field, context);
				expand_edit_string(item->itm_value, item);
				LLS_PUSH(item, &items);
				count++;
			}
			if (rse = context->ctx_rse)
				break;
		}

/* If no print object showed up, complain! */

	if (!count)
		IBERROR(150);			// Msg150 No items in print list

/* Build new print statement.  Unlike the syntax node, the print statement
   has only print items in it. */

	QLI_NOD node = MAKE_NODE(input->syn_type, e_prt_count);
	QLI_NOD list = make_list(items);
	node->nod_arg[e_prt_list] = list;
	node->nod_arg[e_prt_output] = (QLI_NOD) print;

// If DISTINCT was requested, make up a reduced list.

	if (rse && input->syn_arg[s_prt_distinct]) {
		QLI_NOD reduced = MAKE_NODE(nod_list, list->nod_count * 2);
		reduced->nod_count = 0;
		QLI_NOD* ptr = reduced->nod_arg;
		for (USHORT i = 0; i < list->nod_count; i++) {
			ITM item = (ITM) list->nod_arg[i];
			if (item->itm_value) {
				*ptr++ = item->itm_value;
				ptr++;
				reduced->nod_count++;
			}
		}
		if (reduced->nod_count)
			rse->nod_arg[e_rse_reduced] = reduced;
	}

// If a FOR loop was generated, splice it in here.

	if (loop) {
		loop->nod_arg[e_for_statement] = node;
		node = loop;
		if (input->syn_arg[s_prt_order])
			rse->nod_arg[e_rse_sort] =
				expand_sort(input->syn_arg[s_prt_order], new_right, list);
	}

	return node;
}


static ITM expand_print_item( SYN syn_item, LLS right)
{
/**************************************
 *
 *	e x p a n d _ p r i n t _ i t e m
 *
 **************************************
 *
 * Functional description
 *	Expand a print item.  A print item can either be a value or a format
 *	specifier.
 *
 **************************************/
	SYN syn_expr;
	QLI_NOD node;

	ITM item = (ITM) ALLOCD(type_itm);

	switch (syn_item->syn_type) {
	case nod_print_item:
		item->itm_type = item_value;
		syn_expr = syn_item->syn_arg[s_itm_value];
		node = item->itm_value = expand_expression(syn_expr, right);
		item->itm_edit_string = (TEXT *) syn_item->syn_arg[s_itm_edit_string];
		item->itm_query_header = (TEXT *) syn_item->syn_arg[s_itm_header];
		expand_edit_string(node, item);
		return item;

	case nod_column:
		item->itm_type = item_column;
		break;

	case nod_tab:
		item->itm_type = item_tab;
		break;

	case nod_space:
		item->itm_type = item_space;
		break;

	case nod_skip:
		item->itm_type = item_skip;
		break;

	case nod_new_page:
		item->itm_type = item_new_page;
		break;

	case nod_column_header:
		item->itm_type = item_column_header;
		break;

	case nod_report_header:
		item->itm_type = item_report_header;
		break;

	}

	item->itm_count = (int) syn_item->syn_arg[0];
	return item;
}


static QLI_NOD expand_print_list( SYN input, LLS stack)
{
/**************************************
 *
 *	e x p a n d _ p r i n t _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Expand a print list.
 *
 **************************************/
	LLS items = NULL;
	SYN *ptr, *end;

	for (ptr = input->syn_arg, end = ptr + input->syn_count; ptr < end; ptr++)
		LLS_PUSH(expand_print_item(*ptr, stack), &items);

	return make_list(items);
}


static QLI_NOD expand_report( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ r e p o r t
 *
 **************************************
 *
 * Functional description
 *	Expand a report specification.
 *
 **************************************/
	PRT print;

// Start by processing record selection expression

	expand_output(input->syn_arg[s_prt_output], right, &print);
	RPT report = print->prt_report = (RPT) input->syn_arg[s_prt_list];

	if (!(print->prt_lines_per_page = report->rpt_lines))
		print->prt_lines_per_page = QLI_lines;

	if (!report->rpt_columns)
		report->rpt_columns = QLI_columns;

	QLI_NOD loop = MAKE_NODE(nod_report_loop, e_for_count);
	loop->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_prt_rse], &right);
	QLI_NOD node = MAKE_NODE(nod_report, e_prt_count);
	loop->nod_arg[e_for_statement] = node;

	node->nod_arg[e_prt_list] = (QLI_NOD) report;
	node->nod_arg[e_prt_output] = (QLI_NOD) print;

// Process clauses where they exist

	expand_control_break(&report->rpt_top_rpt, right);
	expand_control_break(&report->rpt_top_page, right);
	expand_control_break(&report->rpt_top_breaks, right);

	SYN sub = (SYN) report->rpt_detail_line;
	if (sub)
		report->rpt_detail_line = expand_print_list(sub, right);

	expand_control_break(&report->rpt_bottom_breaks, right);
	expand_control_break(&report->rpt_bottom_page, right);
	expand_control_break(&report->rpt_bottom_rpt, right);

	return loop;
}


static QLI_NOD expand_restructure( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ r e s t r u c t u r e
 *
 **************************************
 *
 * Functional description
 *	Transform a restructure statement into a FOR <rse> STORE.
 *
 **************************************/

// Make a FOR loop to drive the restructure

	QLI_NOD loop = MAKE_NODE(nod_for, e_for_count);
	loop->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_asn_from], &right);

// Make a STORE node.

	QLI_NOD node = MAKE_NODE(nod_store, e_sto_count);
	loop->nod_arg[e_for_statement] = node;
	SYN rel_node = input->syn_arg[s_asn_to];
	QLI_CTX context = (QLI_CTX) ALLOCD(type_ctx);
	node->nod_arg[e_sto_context] = (QLI_NOD) context;
	context->ctx_type = CTX_RELATION;
	context->ctx_rse = (QLI_NOD) - 1;
	QLI_REL relation = context->ctx_relation =
		(QLI_REL) rel_node->syn_arg[s_rel_relation];

// If we don't already know about the relation, find out now.

	if (!(relation->rel_flags & REL_fields))
		MET_fields(relation);

/* Match fields in target relation against fields in the input rse.  Fields
   may match on either name or query name. */

	LLS stack = NULL;

	for (QLI_FLD field = relation->rel_fields; field; field = field->fld_next)
		if (!(field->fld_flags & FLD_computed))
		{
			for (LLS search = right; search; search = search->lls_next) {
				QLI_CTX ctx = (QLI_CTX) search->lls_object;

				// First look for an exact field name match

				QLI_FLD fld;
				for (fld = ctx->ctx_relation->rel_fields; fld;
					 fld = fld->fld_next)
				{
					if (compare_symbols(field->fld_name, fld->fld_name))
						break;
				}
				// Next try, target field name matching source query name

				if (!fld)
					for (fld = ctx->ctx_relation->rel_fields; fld;
						 fld = fld->fld_next)
					{
						if (compare_symbols(field->fld_name, fld->fld_query_name))
							break;
					}
				// If nothing yet, look for any old match

				if (!fld)
					for (fld = ctx->ctx_relation->rel_fields; fld;
						 fld = fld->fld_next)
					{
						 if (compare_symbols(field-> fld_query_name,
											 fld->fld_name)
							|| compare_symbols(field->fld_query_name,
											   fld->fld_query_name))
						{
								break;
						}
					}

				if (fld) {
					QLI_NOD assignment = MAKE_NODE(nod_assign, e_asn_count);
					assignment->nod_count = e_asn_count - 1;
					assignment->nod_arg[e_asn_to] =
						make_field(field, context);
					assignment->nod_arg[e_asn_from] = make_field(fld, ctx);
					LLS_PUSH(assignment, &stack);
					goto found_field;
				}

				if (ctx->ctx_rse)
					break;
			}
			found_field:;
		}

	node->nod_arg[e_sto_statement] = make_list(stack);

	return loop;
}


static QLI_NOD expand_rse( SYN input, LLS * stack)
{
/**************************************
 *
 *	e x p a n d _ r s e
 *
 **************************************
 *
 * Functional description
 *	Expand a record selection expression, returning an updated context
 *	stack.
 *
 **************************************/
	LLS old_stack, new_stack;
	old_stack = new_stack = *stack;
	QLI_NOD boolean = NULL;
	QLI_NOD node = MAKE_NODE(input->syn_type, (int) e_rse_count + input->syn_count);
	node->nod_count = input->syn_count;
	QLI_NOD* ptr2 = &node->nod_arg[e_rse_count];

/* Decide whether or not this is a GROUP BY, real or imagined
   If it is, disallow normal field type references */

	QLI_CTX parent_context = NULL;
	QLI_NOD parent_rse = NULL;

	if (input->syn_arg[s_rse_group_by] || input->syn_arg[s_rse_having])
		parent_context = (QLI_CTX) ALLOCD(type_ctx);
	SYN list = input->syn_arg[s_rse_list];
	if (list) {
		for (USHORT i = 0; i < list->syn_count; i++) {
			SYN value = list->syn_arg[i];
			SYN field = value->syn_arg[e_itm_value];
			if (!field)
				continue;
			if (global_agg(field, input->syn_arg[s_rse_group_by])) {
				if (!parent_context)
					parent_context = (QLI_CTX) ALLOCD(type_ctx);
			}
			else if (parent_context)
				if (invalid_syn_field(field, input->syn_arg[s_rse_group_by]))
					IBERROR(451);
		}
	}

	if (parent_context) {
		parent_context->ctx_type = CTX_AGGREGATE;
		parent_rse = MAKE_NODE(nod_rse, e_rse_count + 1);
		parent_rse->nod_count = 1;
		parent_rse->nod_arg[e_rse_count] = (QLI_NOD) parent_context;
		parent_context->ctx_sub_rse = node;
	}

// Process the FIRST clause before the context gets augmented

	if (input->syn_arg[s_rse_first])
		node->nod_arg[e_rse_first] =
			expand_expression(input->syn_arg[e_rse_first], old_stack);

// Process relations

	SYN* ptr = input->syn_arg + s_rse_count;

	for (USHORT i = 0; i < input->syn_count; i++) {
		SYN rel_node = *ptr++;
		SYN over = *ptr++;
		QLI_CTX context = (QLI_CTX) ALLOCD(type_ctx);
		*ptr2++ = (QLI_NOD) context;
		if (i == 0)
			context->ctx_rse = node;
		if (rel_node->syn_type == nod_rse) {
			context->ctx_type = CTX_STREAM;
			context->ctx_stream = expand_rse(rel_node, &new_stack);
		}
		else {
			context->ctx_type = CTX_RELATION;
			QLI_REL relation = context->ctx_relation =
				(QLI_REL) rel_node->syn_arg[s_rel_relation];
			if (!(relation->rel_flags & REL_fields))
				MET_fields(relation);
			SYM symbol = context->ctx_symbol =
				(SYM) rel_node->syn_arg[s_rel_context];
			if (symbol)
				symbol->sym_object = (BLK) context;
			if (over) {
				LLS short_stack = NULL;
				LLS_PUSH(context, &short_stack);
				for (USHORT j = 0; j < over->syn_count; j++) {
					syn* field = over->syn_arg[j];
					QLI_NOD eql_node = MAKE_NODE(nod_eql, 2);
					eql_node->nod_arg[0] =
						expand_expression(field, short_stack);
					eql_node->nod_arg[1] =
						expand_expression(field, new_stack);
					boolean = make_and(eql_node, boolean);
				}
				LLS_POP(&short_stack);
			}
		}
		LLS_PUSH(context, &new_stack);
	}

// Handle explicit boolean

	if (input->syn_arg[e_rse_boolean])
		boolean = make_and(boolean,
						   expand_expression(input->syn_arg[e_rse_boolean],
											 new_stack));

/* Handle implicit boolean from SQL xxx IN (yyy FROM relation) */

	if (input->syn_arg[s_rse_outer]) {
		QLI_NOD eql_node = MAKE_NODE((enum nod_t)(int)input->syn_arg[s_rse_op], 2);
		eql_node->nod_arg[0] =
			expand_expression(input->syn_arg[s_rse_outer], old_stack);
		eql_node->nod_arg[1] =
			expand_expression(input->syn_arg[s_rse_inner], new_stack);
		if (input->syn_arg[s_rse_all_flag])
			eql_node = negate(eql_node);
		boolean = make_and(eql_node, boolean);
	}

	node->nod_arg[e_rse_boolean] = boolean;

	if (input->syn_arg[s_rse_sort]) {
		QLI_NOD temp = expand_sort(input->syn_arg[e_rse_sort], new_stack, 0);
		if (parent_rse)
			parent_rse->nod_arg[e_rse_sort] = temp;
		else
			node->nod_arg[e_rse_sort] = temp;
	}
#ifdef PC_ENGINE
	else if (input->syn_arg[s_rse_index])
		node->nod_arg[e_rse_index] = (QLI_NOD) input->syn_arg[s_rse_index];
#endif

	if (input->syn_arg[s_rse_reduced])
		node->nod_arg[e_rse_reduced] =
			expand_sort(input->syn_arg[e_rse_reduced], new_stack, 0);

	if (input->syn_arg[s_rse_group_by])
		parent_rse->nod_arg[e_rse_group_by] =
			expand_group_by(input->syn_arg[s_rse_group_by], new_stack,
							parent_context);

	node->nod_arg[e_rse_join_type] = (QLI_NOD) input->syn_arg[s_rse_join_type];

// If there is a parent context, set it up here

	*stack = new_stack;

	if (!parent_context)
		return node;

    QLI_NOD *end;
    QLI_CTX context = NULL;
	for (ptr2 = node->nod_arg + e_rse_count, end = ptr2 + node->nod_count;
		 ptr2 < end; ptr2++)
	{
		context = (QLI_CTX) *ptr2;
		context->ctx_parent = parent_context;
	}

	if (!(parent_context->ctx_relation = context->ctx_relation))
		parent_context->ctx_stream = context->ctx_stream;
	LLS_PUSH(parent_context, stack);

	if (input->syn_arg[s_rse_having])
		parent_rse->nod_arg[e_rse_having] =
			expand_expression(input->syn_arg[s_rse_having], *stack);

	return parent_rse;
}


static QLI_NOD expand_sort( SYN input, LLS stack, QLI_NOD list)
{
/**************************************
 *
 *	e x p a n d _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Expand a sort or reduced clause.  This is more than a little
 *	kludgy.  For pure undiscipled, pragmatic reasons, the count for
 *	a sort/ reduced clause for a syntax node is twice the number of
 *	actual keys.  For node nodes, however, the count is the accurate
 *	number of keys.  So be careful.
 *
 **************************************/
	QLI_NOD node = MAKE_NODE(nod_list, input->syn_count);
	node->nod_count = input->syn_count / 2;
	QLI_NOD* ptr = node->nod_arg;
	SYN* syn_ptr = input->syn_arg;

	for (USHORT i = 0; i < node->nod_count; i++) {
		SYN expr = *syn_ptr++;
		if (expr->syn_type == nod_position) {
			// FIXME: isn't plain 'unsigned long' better here?
			// On 64bit platforms long is 64bit as well as pointer
			const ULONG position = (ULONG) expr->syn_arg[0];
			if (!list || !position || position > list->nod_count)
				IBERROR(152);	// Msg152 invalid ORDER BY ordinal
			ITM item = (ITM) list->nod_arg[position - 1];
			*ptr++ = item->itm_value;
		}
		else
			*ptr++ = expand_expression(expr, stack);
		*ptr++ = (QLI_NOD) * syn_ptr++;
	}

	return node;
}


static QLI_NOD expand_statement( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Expand a statement.
 *
 **************************************/
	SYN *syn_ptr, syn_node, field_node;
	QLI_CTX context;
	QLI_NOD node, (*routine) (SYN, LLS, LLS);
	LLS stack;
	USHORT i;

	switch (input->syn_type) {
	case nod_abort:
		node = MAKE_NODE(input->syn_type, input->syn_count);
		if (input->syn_arg[0])
			node->nod_arg[0] = expand_expression(input->syn_arg[0], right);
		return node;

	case nod_assign:
		routine = expand_assignment;
		break;

	case nod_commit_retaining:
		node = MAKE_NODE(input->syn_type, input->syn_count);
		for (i = 0; i < input->syn_count; i++)
			node->nod_arg[i] = (QLI_NOD) input->syn_arg[i];
		return node;

	case nod_erase:
		routine = expand_erase;
		break;
	case nod_for:
		routine = expand_for;
		break;

	case nod_if:
		node = MAKE_NODE(input->syn_type, input->syn_count);
		node->nod_arg[e_if_boolean] =
			expand_expression(input->syn_arg[s_if_boolean], right);
		node->nod_arg[e_if_true] =
			expand_statement(input->syn_arg[s_if_true], right, left);
		if (input->syn_arg[s_if_false])
			node->nod_arg[e_if_false] =
				expand_statement(input->syn_arg[s_if_false], right, left);
		else
			node->nod_count = 2;
		return node;

	case nod_modify:
		routine = expand_modify;
		break;

	case nod_print:
	case nod_list_fields:
		routine = expand_print;
		break;

	case nod_report:
		routine = expand_report;
		break;

	case nod_restructure:
		routine = expand_restructure;
		break;

	case nod_store:
		routine = expand_store;
		break;

	case nod_repeat:
		node = MAKE_NODE(input->syn_type, input->syn_count);
		node->nod_arg[e_rpt_value] =
			expand_expression(input->syn_arg[s_rpt_value], left);
		node->nod_arg[e_rpt_statement] =
			expand_statement(input->syn_arg[s_rpt_statement], right, left);
		return node;

	case nod_list:
		syn_ptr = input->syn_arg;
		stack = NULL;
		for (i = 0; i < input->syn_count; i++) {
			syn_node = *syn_ptr++;
			if (syn_node->syn_type == nod_declare) {
				context = (QLI_CTX) ALLOCD(type_ctx);
				context->ctx_type = CTX_VARIABLE;
				if (field_node = syn_node->syn_arg[1]) {
					if (field_node->syn_type == nod_index)
						field_node = field_node->syn_arg[s_idx_field];
					resolve_really((QLI_FLD) syn_node->syn_arg[0], field_node);
				}
				context->ctx_variable = (QLI_FLD) syn_node->syn_arg[0];
				LLS_PUSH(context, &right);
				LLS_PUSH(context, &left);
			}
			else if (node = expand_statement(syn_node, right, left))
				LLS_PUSH(node, &stack);
		}
		return make_list(stack);

	case nod_declare:
		return NULL;

	default:
		BUGCHECK(136);			// Msg136 expand_statement: not yet implemented
	}

	return (*routine) (input, right, left);
}


static QLI_NOD expand_store( SYN input, LLS right, LLS left)
{
/**************************************
 *
 *	e x p a n d _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Process, yea expand, on a mere STORE statement.  Make us
 *	something neat if nothing looks obvious.
 *
 **************************************/
	QLI_NOD loop = NULL;

// If there is an rse, make up a FOR loop

	if (input->syn_arg[s_sto_rse]) {
		loop = MAKE_NODE(nod_for, e_for_count);
		loop->nod_arg[e_for_rse] = expand_rse(input->syn_arg[s_sto_rse],
											  &right);
	}

	QLI_NOD node = MAKE_NODE(input->syn_type, e_sto_count);

	SYN rel_node = input->syn_arg[s_sto_relation];
	QLI_CTX context = (QLI_CTX) ALLOCD(type_ctx);
	node->nod_arg[e_sto_context] = (QLI_NOD) context;
	context->ctx_type = CTX_RELATION;
	context->ctx_rse = (QLI_NOD) - 1;
	QLI_REL relation = context->ctx_relation =
		(QLI_REL) rel_node->syn_arg[s_rel_relation];

	if (!(relation->rel_flags & REL_fields))
		MET_fields(relation);

	SYM symbol = context->ctx_symbol = (SYM) rel_node->syn_arg[s_rel_context];
	if (symbol)
		symbol->sym_object = (BLK) context;

	LLS_PUSH(context, &left);

//  If there are field and value lists, process them

	if (input->syn_arg[s_sto_values]) {
		if (!input->syn_arg[s_sto_fields]) {
			LLS stack = NULL;
			for (QLI_FLD field = relation->rel_fields; field;
				field = field->fld_next)
			{
				LLS_PUSH(decompile_field(field, 0), &stack);
			}
			input->syn_arg[s_sto_fields] = (SYN) stack;
		}
		expand_values(input, right);
	}

/* Process sub-statement.  If there isn't one, make up a series of
   assignments. */

	if (input->syn_arg[s_sto_statement]) {
		QLI_CTX secondary = (QLI_CTX) ALLOCD(type_ctx);
		secondary->ctx_type = CTX_RELATION;
		secondary->ctx_primary = context;
		LLS_PUSH(secondary, &right);
		node->nod_arg[e_sto_statement] =
			expand_statement(input->syn_arg[s_sto_statement], right, left);
	}
	else {
		LLS stack = NULL;
		for (QLI_FLD field = relation->rel_fields; field;
			field = field->fld_next)
		{
			if (field->fld_flags & FLD_computed)
				continue;
			if (
				(field->fld_system_flag
				 && field->fld_system_flag != relation->rel_system_flag)
				|| field->fld_flags & FLD_array)
				continue;
			QLI_NOD assignment = make_assignment(make_field(field, context), 0, 0);
			LLS_PUSH(assignment, &stack);
		}
		node->nod_arg[e_sto_statement] = make_list(stack);
	}

	if (!loop)
		return node;

	loop->nod_arg[e_for_statement] = node;

	return loop;
}


static void expand_values( SYN input, LLS right)
{
/**************************************
 *
 *	e x p a n d _ v a l u e s
 *
 **************************************
 *
 * Functional description
 *	We've got a grungy SQL insert, and we have
 *	to make the value list match the field list.
 *	On the way in, we got the right number of
 *	fields.  Now all that's needed is the values
 *	and matching the two lists, and generating
 *	assignments.  If the input is from a select,
 *	things may be harder, and if there are wild cards
 *	things will be harder still.  Wild cards come in
 *	two flavors * and <context>.*.  The first is
 *	a nod_prompt, the second a nod_star.
 *
 **************************************/
	SSHORT field_count = 0, value_count = 0;

// fields have already been checked and expanded.  Just count them

	LLS fields = (LLS) input->syn_arg[s_sto_fields];
	LLS stack;
	for (stack = fields; stack; stack = stack->lls_next)
		field_count++;

// We're going to want the values in the order listed in the command

	LLS values = (LLS) input->syn_arg[s_sto_values];
	for (; values; LLS_PUSH(LLS_POP(&values), &stack));

// now go through, count, and expand where needed

	while (stack) {
		SYN value = (SYN) LLS_POP(&stack);
		if (input->syn_arg[s_sto_rse] && value->syn_type == nod_prompt) {
			if (value->syn_arg[0] == 0) {
				LLS temp = NULL;
				for (; right; right = right->lls_next)
					LLS_PUSH(right->lls_object, &temp);
				while (temp) {
					QLI_CTX context = (QLI_CTX) LLS_POP(&temp);
					value_count +=
						generate_fields(context, (LLS) &values,
										input->syn_arg[s_sto_rse]);
				}
			}
			else
				IBERROR(542);	// this was a prompting expression.  won't do at all
		}
		else if (input->syn_arg[s_sto_rse] && (value->syn_type == nod_star)) {
			QLI_CTX context = find_context((NAM) value->syn_arg[0], right);
			if (!context)
				IBERROR(154);	// Msg154 unrecognized context
			value_count +=
				generate_fields(context, (LLS) &values, input->syn_arg[s_sto_rse]);
		}
		else {
			LLS_PUSH(value, &values);
			value_count++;
		}
	}

// Make assignments from values to fields

	if (field_count != value_count)
		IBERROR(189);
		// Msg189 the number of values do not match the number of fields

	SYN list = (SYN) ALLOCDV(type_syn, value_count);
	list->syn_type = nod_list;
	list->syn_count = value_count;
	input->syn_arg[s_sto_statement] = list;
	SYN* ptr = list->syn_arg + value_count;

	while (values) {
	    SYN assignment = (SYN) ALLOCDV(type_syn, s_asn_count);
		*--ptr = assignment;
		assignment->syn_type = nod_assign;
		assignment->syn_count = s_asn_count;
		assignment->syn_arg[s_asn_to] = (SYN) LLS_POP(&fields);
		assignment->syn_arg[s_asn_from] = (SYN) LLS_POP(&values);
	}
}


static QLI_CTX find_context( NAM name, LLS contexts)
{
/**************************************
 *
 *	f i n d _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	We've got a context name and we need to return
 *	the context block implicated.
 *
 **************************************/
	QLI_CTX context;

	for (; contexts; contexts = contexts->lls_next) {
		context = (QLI_CTX) contexts->lls_object;
		QLI_REL relation = context->ctx_relation;
		if (compare_names(name, relation->rel_symbol))
			break;
		if (compare_names(name, context->ctx_symbol))
			break;
	}
	return (contexts) ? context : NULL;
}


static int generate_fields( QLI_CTX context, LLS values, SYN rse)
{
/**************************************
 *
 *	g e n e r a t e _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Expand an asterisk expression, which
 *	could be <relation>.* or <alias>.* or <context>.*
 *	into a list of non-expanded field blocks for
 *	input to a store or update.
 *
 **************************************/
	if (context->ctx_type == CTX_AGGREGATE)
		return 0;
	SYN group_list = rse->syn_arg[s_rse_group_by];
	QLI_REL relation = context->ctx_relation;
	int count = 0;

	for (QLI_FLD field = relation->rel_fields; field; field = field->fld_next) {
		if (
			(field->fld_system_flag
			 && field->fld_system_flag != relation->rel_system_flag)
			|| field->fld_flags & FLD_array)
		{
			continue;
		}
		SYN value = decompile_field(field, context);
		if (group_list && invalid_syn_field(value, group_list))
			continue;
		LLS_PUSH(value, (LLS*) values);
		count++;
	}

	return count;
}


static int generate_items( SYN symbol, LLS right, LLS items, QLI_NOD rse)
{
/**************************************
 *
 *	g e n e r a t e _ i t e m s
 *
 **************************************
 *
 * Functional description
 *	Expand an asterisk expression, which
 *	could be <relation>.* or <alias>.* or <context>.*
 *	into a list of reasonable print items.
 *
 *      If the original request included a group by,
 *	include only the grouping fields.
 *
 **************************************/
	int count = 0;
	QLI_NOD group_list = (rse) ? rse->nod_arg[e_rse_group_by] : NULL;

// first identify the relation or context

	NAM name;
	if (symbol->syn_count == 1)
		name = (NAM) symbol->syn_arg[0];
	else
		IBERROR(153);
		// Msg153 asterisk expressions require exactly one qualifying context

	QLI_CTX context = find_context(name, right);
	if (!context)
		IBERROR(154);			// Msg154 unrecognized context

	QLI_REL relation = context->ctx_relation;

	for (QLI_FLD field = relation->rel_fields; field; field = field->fld_next) {
		if (
			(field->fld_system_flag
			 && field->fld_system_flag != relation->rel_system_flag)
			|| field->fld_flags & FLD_array)
			continue;
		QLI_NOD node = make_field(field, context);
		if (group_list && invalid_nod_field(node, group_list))
			continue;
		ITM item = (ITM) ALLOCD(type_itm);
		item->itm_type = item_value;
		item->itm_value = make_field(field, context);
		expand_edit_string(item->itm_value, item);
		LLS_PUSH(item, (LLS*) items);
		count++;
	}

	return count;
}


static bool global_agg( SYN item, SYN group_list)
{
/**************************************
 *
 *	g l o b a l _ a g g
 *
 **************************************
 *
 * Functional description
 *	We've got a print list item that may contain
 *	a sql global aggregate.  If it does, we're
 *	going to make the whole thing a degenerate
 *	group by.  Anyway.  Look for aggregates buried
 *	deep within printable things.
 *
 *	This recurses.  If it finds a mixture of normal
 *	and aggregates it complains.
 *
 **************************************/
	SYN *ptr, *end;
	bool normal_field = false;
	bool aggregate = false;

	switch (item->syn_type) {
	case nod_agg_average:
	case nod_agg_max:
	case nod_agg_min:
	case nod_agg_total:
	case nod_agg_count:
	case nod_running_total:
	case nod_running_count:
		return true;

	case nod_upcase:
	case nod_add:
	case nod_subtract:
	case nod_multiply:
	case nod_divide:
	case nod_negate:
	case nod_concatenate:
	case nod_substr:
		{
			for (ptr = item->syn_arg, end = ptr + item->syn_count; ptr < end;
				 ptr++)
			{
				if ((*ptr)->syn_type == nod_constant)
					continue;
				if (global_agg(*ptr, group_list))
					aggregate = true;
				else if (!group_list || invalid_syn_field(*ptr, group_list))
					normal_field = true;
			}
		}

	default:
		break;
	}

	if (normal_field && aggregate)
		IBERROR(451);

	return aggregate;
}


static bool invalid_nod_field( QLI_NOD node, QLI_NOD list)
{
/**************************************
 *
 *	i n v a l i d _ n o d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 *	Validate that an expanded field / context
 *	pair is in a specified list.  Thus is used
 *	in one instance to check that a simple field selected
 *	through a grouping rse is a grouping field -
 *	thus a valid field reference.
 *
 **************************************/
	QLI_NOD *ptr, *end;
	bool invalid = false;

	if (!list)
		return true;

	if (node->nod_type == nod_field) {
		QLI_FLD field = (QLI_FLD) node->nod_arg[e_fld_field];
		QLI_CTX context = (QLI_CTX) node->nod_arg[e_fld_context];
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end; ptr++)
			if (field == (QLI_FLD) (*ptr)->nod_arg[e_fld_field]
				&& context == (QLI_CTX) (*ptr)->nod_arg[e_fld_context])
			{
				return false;
			}
		return true;
	}
	else
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
		{
			switch ((*ptr)->nod_type) {
			case nod_field:
			case nod_add:
			case nod_subtract:
			case nod_multiply:
			case nod_divide:
			case nod_negate:
			case nod_concatenate:
			case nod_substr:
			case nod_format:
			case nod_choice:
			case nod_function:
			case nod_upcase:
				invalid |= invalid_nod_field(*ptr, list);
			}
		}

	return invalid;
}


static bool invalid_syn_field( SYN syn_node, SYN list)
{
/**************************************
 *
 *	i n v a l i d _ s y n _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Make sure an unexpanded simple field selected
 *	through a grouping rse is a grouping field -
 *	thus a valid field reference.  For the sake of
 *      argument, we'll match qualified to unqualified
 *	reference, but qualified reference must match
 *	completely.
 *
 *	One more thought.  If this miserable thing is
 *	a wild card, let it through and expand it
 *	correctly later.
 *
 **************************************/
	SYN *ptr, *end;
	bool invalid = false;

	if (syn_node->syn_type == nod_star)
		return false;

	if (!list)
		return true;

	if (syn_node->syn_type == nod_field) {
		NAM fctx = NULL;
		NAM fname = (NAM) syn_node->syn_arg[0];
		if (syn_node->syn_count == 2) {
			fctx = fname;
			fname = (NAM) syn_node->syn_arg[1];
		}

		for (SSHORT count = list->syn_count; count;) {
			NAM gctx = NULL;
			SYN element = list->syn_arg[--count];
			NAM gname = (NAM) element->syn_arg[0];
			if (element->syn_count == 2) {
				gctx = gname;
				gname = (NAM) element->syn_arg[1];
			}
			if (!strcmp(fname->nam_string, gname->nam_string))
				if (!gctx || !fctx
					|| !strcmp(fctx->nam_string, gctx->nam_string))
				{
					return false;
				}
		}
		return true;
	}
	else
		for (ptr = syn_node->syn_arg, end = ptr + syn_node->syn_count;
			 ptr < end; ptr++)
		{
			switch ((*ptr)->syn_type) {
			case nod_field:
			case nod_add:
			case nod_subtract:
			case nod_multiply:
			case nod_divide:
			case nod_negate:
			case nod_concatenate:
			case nod_substr:
			case nod_format:
			case nod_choice:
			case nod_function:
			case nod_upcase:
				invalid |= invalid_syn_field(*ptr, list);
			}
		}

	return invalid;
}


static QLI_NOD make_and( QLI_NOD expr1, QLI_NOD expr2)
{
/**************************************
 *
 *	m a k e _ a n d
 *
 **************************************
 *
 * Functional description
 *	Combine two expressions, each possible null, into at most
 *	a single boolean.
 *
 **************************************/
	if (!expr1)
		return expr2;

	if (!expr2)
		return expr1;

	QLI_NOD node = MAKE_NODE(nod_and, 2);
	node->nod_arg[0] = expr1;
	node->nod_arg[1] = expr2;

	return node;
}


static QLI_NOD make_assignment( QLI_NOD target, QLI_NOD initial, LLS right)
{
/**************************************
 *
 *	m a k e _ a s s i g n m e n t
 *
 **************************************
 *
 * Functional description
 *	Generate a prompt and assignment to a field.
 *
 **************************************/
	QLI_NOD prompt;

	QLI_FLD field = (QLI_FLD) target->nod_arg[e_fld_field];
	LLS stack = NULL;
	LLS_PUSH(target->nod_arg[e_fld_context], &stack);

	if (field->fld_dtype == dtype_blob) {
		prompt = MAKE_NODE(nod_edit_blob, e_edt_count);
		prompt->nod_count = 0;
		prompt->nod_arg[e_edt_name] = (QLI_NOD) field->fld_name->sym_string;
		if (initial) {
			prompt->nod_count = 1;
			prompt->nod_arg[e_edt_input] = expand_expression((SYN) initial, right);
		}
	}
	else {
		prompt = MAKE_NODE(nod_prompt, e_prm_count);
		prompt->nod_arg[e_prm_prompt] = (QLI_NOD) field->fld_name->sym_string;
		prompt->nod_arg[e_prm_field] = (QLI_NOD) field;
	}

	QLI_NOD assignment = MAKE_NODE(nod_assign, e_asn_count);
	assignment->nod_arg[e_asn_to] = target;
	assignment->nod_arg[e_asn_from] = prompt;

	if (field->fld_validation)
		assignment->nod_arg[e_asn_valid] =
			expand_expression(field->fld_validation, stack);
	else
		--assignment->nod_count;

	LLS_POP(&stack);

	return assignment;
}


static QLI_NOD make_field( QLI_FLD field, QLI_CTX context)
{
/**************************************
 *
 *	m a k e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Make a field block.  Not too tough.
 *
 **************************************/
	QLI_NOD node = MAKE_NODE(nod_field, e_fld_count);
	node->nod_count = 0;
	node->nod_arg[e_fld_field] = (QLI_NOD) field;
	node->nod_arg[e_fld_context] = (QLI_NOD) context;

	if (context->ctx_variable)
		node->nod_type = nod_variable;

	return node;
}


static QLI_NOD make_list( LLS stack)
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
	LLS temp = stack;
	USHORT count = 0;

	while (temp) {
		count++;
		temp = temp->lls_next;
	}

	QLI_NOD node = MAKE_NODE(nod_list, count);
	QLI_NOD* ptr = &node->nod_arg[count];

	while (stack)
		*--ptr = (QLI_NOD) LLS_POP(&stack);

	return node;
}


static QLI_NOD make_node( NOD_T type, USHORT count)
{
/**************************************
 *
 *	m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Allocate a node and fill in some basic stuff.
 *
 **************************************/
	QLI_NOD node = (QLI_NOD) ALLOCDV(type_nod, count);
	node->nod_type = type;
	node->nod_count = count;

	return node;
}


static QLI_NOD negate( QLI_NOD expr)
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
	QLI_NOD node = MAKE_NODE(nod_not, 1);
	node->nod_arg[0] = expr;

	return node;
}


static QLI_NOD possible_literal(SYN input,
								LLS stack,
								bool upper_flag)
{
/**************************************
 *
 *	p o s s i b l e _ l i t e r a l
 *
 **************************************
 *
 * Functional description
 *	Check to see if a value node is an unresolved name.  If so,
 *	transform it into a constant expression.  This is used to
 *	correct "informalities" in relational expressions.
 *
 **************************************/
	QLI_CTX context;

/* If the value isn't a field, is qualified, or can be resolved,
   it doesn't qualify for conversion.  Return NULL. */

	if (input->syn_type != nod_field ||
		input->syn_count != 1 || resolve(input, stack, &context))
		return NULL;

	NAM name = (NAM) input->syn_arg[0];
	USHORT l = name->nam_length;
	CON constant = (CON) ALLOCDV(type_con, l);
	constant->con_desc.dsc_dtype = dtype_text;
	constant->con_desc.dsc_length = l;
	TEXT* p = (TEXT *) constant->con_data;
	constant->con_desc.dsc_address = (UCHAR *) p;
	const TEXT* q = name->nam_string;

	if (upper_flag) {
		if (l)
			do {
				const TEXT c = *q++;
				*p++ = UPPER(c);
			} while (--l);
	}
	else if (l)
		do {
			const TEXT c = *q++;
			*p++ = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;

		} while (--l);

	QLI_NOD node = MAKE_NODE(nod_constant, 0);
	node->nod_desc = constant->con_desc;

	return node;
}


static QLI_NOD post_map( QLI_NOD node, QLI_CTX context)
{
/**************************************
 *
 *	p o s t _ m a p
 *
 **************************************
 *
 * Functional description
 *	Post an item to a map for a context.
 *
 **************************************/
	QLI_MAP map;

// Check to see if the item has already been posted

	for (map = context->ctx_map; map; map = map->map_next)
		if (CMP_node_match(node, map->map_node))
			break;

	if (!map) {
		map = (QLI_MAP) ALLOCD(type_map);
		map->map_next = context->ctx_map;
		context->ctx_map = map;
		map->map_node = node;
	}

	QLI_NOD new_node = MAKE_NODE(nod_map, e_map_count);
	new_node->nod_count = 0;
	new_node->nod_arg[e_map_context] = (QLI_NOD) context;
	new_node->nod_arg[e_map_map] = (QLI_NOD) map;
	new_node->nod_desc = node->nod_desc;

	return new_node;
}


static QLI_FLD resolve( SYN node, LLS stack, QLI_CTX * out_context)
{
/**************************************
 *
 *	r e s o l v e
 *
 **************************************
 *
 * Functional description
 *	Resolve a field node against a context stack.  Return both the
 *	field block (by value)  and the corresponding context block (by
 *	reference).  Return NULL if field can't be resolved.
 *
 **************************************/
	QLI_REL relation;
	QLI_FLD field;

/* Look thru context stack looking for a context that will resolve
   all name segments.  If the context is a secondary context, require
   that the context name be given explicitly (used for special STORE
   context). */

	NAM* base = (NAM *) node->syn_arg;

	for (; stack; stack = stack->lls_next) {
		QLI_CTX context = (QLI_CTX) stack->lls_object;
		*out_context = context;
		NAM* ptr = base + node->syn_count;
		NAM name = *--ptr;

		switch (context->ctx_type) {
		case CTX_VARIABLE:
			if (ptr == base)
				for (field = context->ctx_variable; field;
					 field = field->fld_next)
				{
					if (compare_names(name, field->fld_name)
						|| compare_names(name, field->fld_query_name))
					{
						return field;
					}
				}
			break;

		case CTX_RELATION:
			if (context->ctx_primary) {
				*out_context = context = context->ctx_primary;
				if (!compare_names((NAM) node->syn_arg[0], context->ctx_symbol))
					break;
			}
			relation = context->ctx_relation;

			for (field = relation->rel_fields; field; field = field->fld_next)
				if (compare_names(name, field->fld_name) ||
					compare_names(name, field->fld_query_name))
				{
					if (ptr == base)
						return field;
					name = *--ptr;

					if (compare_names(name, relation->rel_symbol))
						if (ptr == base)
							return field;
						else
							name = *--ptr;

					if (compare_names(name, context->ctx_symbol))
						if (ptr == base)
							return field;
					break;
				}
			break;
		}
	}

// We didn't resolve all name segments.  Let somebody else worry about it.

	return NULL;
}



static void resolve_really( QLI_FLD variable, SYN field_node)
{
/**************************************
 *
 *	r e s o l v e _ r e a l l y
 *
 **************************************
 *
 * Functional description
 *	Resolve a field reference entirely.
 *
 **************************************/
	SYM symbol;
	QLI_REL relation;

/* For ease, break down the syntax block.
   It should contain at least one name; two names are a  potential ambiguity:
   check for a dbb (<db>.<glo_fld>), then for a rel (<rel>.<fld>). */

	USHORT offset = field_node->syn_count;
	NAM fld_name = (NAM) field_node->syn_arg[--offset];

	NAM rel_name = NULL;
	NAM db_name = NULL;
	if (offset) {
		rel_name = (NAM) field_node->syn_arg[--offset];
		if (offset)
			db_name = (NAM) field_node->syn_arg[--offset];
	}

    bool resolved = false;
    bool local = false;
    QLI_FLD field = NULL;

	if (field_node->syn_count == 1)
		resolved = (MET_declare(0, variable, fld_name));
	else if (field_node->syn_count == 2) {
		for (symbol = rel_name->nam_symbol; symbol;
			 symbol = symbol->sym_homonym)
		{
			if (symbol->sym_type == SYM_database) {
				DBB dbb = (DBB) symbol->sym_object;
				resolved = (MET_declare(dbb, variable, fld_name));
				break;			// should be only one db in homonym list
			}
		}

		if (!resolved) {
			for (DBB dbb = QLI_databases; dbb && !resolved; dbb = dbb->dbb_next)
				for (symbol = rel_name->nam_symbol; symbol;
					 symbol = symbol->sym_homonym)
				{
					if (symbol->sym_type == SYM_relation
						&& (relation = (QLI_REL) symbol->sym_object)
						&& relation->rel_database == dbb)
					{
						if (!relation->rel_fields)
							MET_fields(relation);
						for (field = relation->rel_fields; field;
							 field = field->fld_next)
						{
							resolved = local = compare_names(fld_name,
														   field->fld_name);
							if (resolved)
								break;
						}
						break;	// should be only one rel in homonym list for each db
					}
				}
		}
	}
	else {
		relation = variable->fld_relation;
		if (!relation->rel_fields)
			MET_fields(relation);
		for (field = relation->rel_fields; field; field = field->fld_next)
		{
			resolved = local = compare_names(fld_name, field->fld_name);
			if (resolved)
				break;
		}
	}

	if (!resolved)
		IBERROR(155);
		// Msg155 field referenced in BASED ON can not be resolved against readied databases

	if (local) {
		variable->fld_dtype = field->fld_dtype;
		variable->fld_length = field->fld_length;
		variable->fld_scale = field->fld_scale;
		variable->fld_sub_type = field->fld_sub_type;
		variable->fld_sub_type_missing = field->fld_sub_type_missing;
		if (!variable->fld_edit_string)
			variable->fld_edit_string = field->fld_edit_string;
		if (!variable->fld_query_header)
			variable->fld_query_header = field->fld_query_header;
		if (!variable->fld_query_name)
			variable->fld_query_name = field->fld_query_name;
	}
}



