/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		gen.cpp
 *	DESCRIPTION:	Routines to generate BLR.
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
 * Contributor(s): ______________________________________
 * 2001.6.21 Claudio Valderrama: BREAK and SUBSTRING.
 * 2001.07.28 John Bellardo:  Added code to generate blr_skip.
 * 2002.07.30 Arno Brinkman:  Added code, procedures to generate COALESCE, CASE
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL
 * 2004.01.16 Vlad Horsun: Added support for default parameters and 
 *   EXECUTE BLOCK statement
 */

#include "firebird.h"
#include <string.h>
#include <stdio.h>
#include "../dsql/dsql.h"
#include "../jrd/ibase.h"
#include "../jrd/align.h"
#include "../jrd/intl.h"
#include "../dsql/alld_proto.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/misc_func.h"
#include "../jrd/thd.h"
#include "../jrd/dsc_proto.h"
#include "gen/iberror.h"

static void gen_aggregate(dsql_req*, const dsql_nod*);
static void gen_cast(dsql_req*, const dsql_nod*);
static void gen_coalesce(dsql_req*, const dsql_nod*);
static void gen_constant(dsql_req*, dsc*, bool);
static void gen_descriptor(dsql_req*, const dsc*, bool);
static void gen_error_condition(dsql_req*, const dsql_nod*);
static void gen_field(dsql_req*, const dsql_ctx*, const dsql_fld*, dsql_nod*);
static void gen_for_select(dsql_req*, dsql_nod*);
static void gen_gen_id(dsql_req*, const dsql_nod*);
static void gen_join_rse(dsql_req*, const dsql_nod*);
static void gen_map(dsql_req*, dsql_map*);
static void gen_parameter(dsql_req*, const dsql_par*);
static void gen_plan(dsql_req*, const dsql_nod*);
static void gen_relation(dsql_req*, dsql_ctx*);
static void gen_rse(dsql_req*, const dsql_nod*);
static void gen_searched_case(dsql_req*, const dsql_nod*);
static void gen_select(dsql_req*, dsql_nod*);
static void gen_simple_case(dsql_req*, const dsql_nod*);
static void gen_sort(dsql_req*, dsql_nod*);
static void gen_table_lock(dsql_req*, const dsql_nod*, USHORT);
static void gen_udf(dsql_req*, const dsql_nod*);
static void gen_union(dsql_req*, const dsql_nod*);
static void stuff_cstring(dsql_req*, const char*);
static void stuff_word(dsql_req*, USHORT);

static const SCHAR db_key_name[] = "DB_KEY";

// STUFF is defined in dsql.h for use in common with ddl.c 

// The following are passed as the third argument to gen_constant 
const bool NEGATE_VALUE = true;
const bool USE_VALUE    = false;


/**
  
 	GEN_expr
  
    @brief	Generate blr for an arbitrary expression.
 

    @param request
    @param node

 **/
void GEN_expr( dsql_req* request, dsql_nod* node)
{
	UCHAR blr_operator;
	dsql_nod* ddl_node;
	dsql_ctx* context;
	dsql_map* map;
	dsql_var* variable;

	switch (node->nod_type) {
	case nod_alias:
		GEN_expr(request, node->nod_arg[e_alias_value]);
		return;

	case nod_aggregate:
		gen_aggregate(request, node);
		return;

	case nod_constant:
		gen_constant(request, &node->nod_desc, USE_VALUE);
		return;

	case nod_derived_field:
		GEN_expr(request, node->nod_arg[e_derived_field_value]);
		return;

	case nod_extract:
		stuff(request, blr_extract);
		stuff(request, *(SLONG *) node->nod_arg[e_extract_part]->nod_desc.dsc_address);
		GEN_expr(request, node->nod_arg[e_extract_value]);
		return;

	case nod_dbkey:
		node = node->nod_arg[0];
		context = (dsql_ctx*) node->nod_arg[e_rel_context];
		stuff(request, blr_dbkey);
		stuff(request, context->ctx_context);
		return;

	case nod_rec_version:
		node = node->nod_arg[0];
		context = (dsql_ctx*) node->nod_arg[e_rel_context];
		stuff(request, blr_record_version);
		stuff(request, context->ctx_context);
		return;

	case nod_dom_value:
		if ((request->req_type != REQ_DDL) ||
			!(ddl_node = request->req_ddl_node) ||
			!(ddl_node->nod_type == nod_def_domain ||
			  ddl_node->nod_type == nod_mod_domain))
		{
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 901,
						  isc_arg_gds, isc_dsql_domain_err, 0);
		}
		stuff(request, blr_fid);
		stuff(request, 0);				// Context   
		stuff_word(request, 0);			// Field id  
		return;

	case nod_field:
		gen_field(request,
				  (dsql_ctx*) node->nod_arg[e_fld_context],
				  (dsql_fld*) node->nod_arg[e_fld_field],
				  node->nod_arg[e_fld_indices]);
		return;

	case nod_user_name:
		stuff(request, blr_user_name);
		return;

	case nod_current_time:
		stuff(request, blr_current_time);
		return;

	case nod_current_timestamp:
		stuff(request, blr_current_timestamp);
		return;

	case nod_current_date:
		stuff(request, blr_current_date);
		return;

    case nod_current_role:
        stuff(request, blr_current_role);
        return;

	case nod_udf:
		gen_udf(request, node);
		return;

	case nod_variable:
		variable = (dsql_var*) node->nod_arg[e_var_variable];
		if (variable->var_flags & VAR_input) {
			stuff(request, blr_parameter2);
			stuff(request, variable->var_msg_number);
			stuff_word(request, variable->var_msg_item);
			stuff_word(request, variable->var_msg_item + 1);
		}
		else {
			stuff(request, blr_variable);
			stuff_word(request, variable->var_variable_number);
		}
		return;

	case nod_join:
		gen_join_rse(request, node);
		return;

	case nod_map:
		map = (dsql_map*) node->nod_arg[e_map_map];
		context = (dsql_ctx*) node->nod_arg[e_map_context];
		stuff(request, blr_fid);
		stuff(request, context->ctx_context);
		stuff_word(request, map->map_position);
		return;

	case nod_parameter:
		gen_parameter(request, (dsql_par*) node->nod_arg[e_par_parameter]);
		return;

	case nod_relation:
		gen_relation(request, (dsql_ctx*) node->nod_arg[e_rel_context]);
		return;

	case nod_rse:
		gen_rse(request, node);
		return;

	case nod_derived_table:
		gen_rse(request, node->nod_arg[e_derived_table_rse]);
		return;

	case nod_exists:
		stuff(request, blr_any);
		gen_rse(request, node->nod_arg[0]);
		return;

	case nod_singular:
		stuff(request, blr_unique);
		gen_rse(request, node->nod_arg[0]);
		return;

	case nod_agg_count:
		if (!(request->req_dbb->dbb_flags & DBB_v3)) {
			if (node->nod_count)
				blr_operator = (node->nod_flags & NOD_AGG_DISTINCT) ?
					blr_agg_count_distinct : blr_agg_count2;
			else
				blr_operator = blr_agg_count;
		}
		else
			blr_operator = blr_agg_count;
		break;

	case nod_agg_min:
		blr_operator = blr_agg_min;
		break;
	case nod_agg_max:
		blr_operator = blr_agg_max;
		break;

	case nod_agg_average:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			blr_operator = (node->nod_flags & NOD_AGG_DISTINCT) ?
				blr_agg_average_distinct : blr_agg_average;
		else
			blr_operator = blr_agg_average;
		break;

	case nod_agg_total:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			blr_operator = (node->nod_flags & NOD_AGG_DISTINCT) ?
				blr_agg_total_distinct : blr_agg_total;
		else
			blr_operator = blr_agg_total;
		break;

	case nod_agg_average2:
		blr_operator = (node->nod_flags & NOD_AGG_DISTINCT) ?
			blr_agg_average_distinct : blr_agg_average;
		break;

	case nod_agg_total2:
		blr_operator = (node->nod_flags & NOD_AGG_DISTINCT) ?
			blr_agg_total_distinct : blr_agg_total;
		break;

	case nod_and:
		blr_operator = blr_and;
		break;
	case nod_or:
		blr_operator = blr_or;
		break;
	case nod_not:
		blr_operator = blr_not;
		break;
	case nod_eql_all:
	case nod_eql_any:
	case nod_eql:
		blr_operator = blr_eql;
		break;
	case nod_equiv:
		blr_operator = blr_equiv;
		break;
	case nod_neq_all:
	case nod_neq_any:
	case nod_neq:
		blr_operator = blr_neq;
		break;
	case nod_gtr_all:
	case nod_gtr_any:
	case nod_gtr:
		blr_operator = blr_gtr;
		break;
	case nod_leq_all:
	case nod_leq_any:
	case nod_leq:
		blr_operator = blr_leq;
		break;
	case nod_geq_all:
	case nod_geq_any:
	case nod_geq:
		blr_operator = blr_geq;
		break;
	case nod_lss_all:
	case nod_lss_any:
	case nod_lss:
		blr_operator = blr_lss;
		break;
	case nod_between:
		blr_operator = blr_between;
		break;
	case nod_containing:
		blr_operator = blr_containing;
		break;
	case nod_starting:
		blr_operator = blr_starting;
		break;
	case nod_missing:
		blr_operator = blr_missing;
		break;

	case nod_like:
		blr_operator = (node->nod_count == 2) ? blr_like : blr_ansi_like;
		break;

	case nod_add:
		blr_operator = blr_add;
		break;
	case nod_subtract:
		blr_operator = blr_subtract;
		break;
	case nod_multiply:
		blr_operator = blr_multiply;
		break;

	case nod_negate:
		{
			dsql_nod* child = node->nod_arg[0];
			if (child->nod_type == nod_constant &&
				DTYPE_IS_NUMERIC(child->nod_desc.dsc_dtype)) {
				gen_constant(request, &child->nod_desc, NEGATE_VALUE);
				return;
			}
		}
		blr_operator = blr_negate;
		break;

	case nod_divide:
		blr_operator = blr_divide;
		break;
	case nod_add2:
		blr_operator = blr_add;
		break;
	case nod_subtract2:
		blr_operator = blr_subtract;
		break;
	case nod_multiply2:
		blr_operator = blr_multiply;
		break;
	case nod_divide2:
		blr_operator = blr_divide;
		break;
	case nod_concatenate:
		blr_operator = blr_concatenate;
		break;
	case nod_null:
		blr_operator = blr_null;
		break;
	case nod_any:
		blr_operator = blr_any;
		break;
	case nod_ansi_any:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			blr_operator = blr_ansi_any;
		else
			blr_operator = blr_any;
		break;
	case nod_ansi_all:
		blr_operator = blr_ansi_all;
		break;
	case nod_via:
		blr_operator = blr_via;
		break;

	case nod_internal_info:
		blr_operator = blr_internal_info;
		break;
	case nod_upcase:
		blr_operator = blr_upcase;
		break;
	case nod_substr:	
        blr_operator = blr_substring;
        break;
	case nod_cast:
		gen_cast(request, node);
		return;
	case nod_gen_id:
	case nod_gen_id2:
		gen_gen_id(request, node);
		return;
    case nod_coalesce: 
		gen_coalesce(request, node);
		return;
    case nod_simple_case: 
		gen_simple_case(request, node);
		return;
    case nod_searched_case: 
		gen_searched_case(request, node);
		return;
	case nod_average:
	case nod_count:
	case nod_from:
	case nod_max:
	case nod_min:
	case nod_total:
		switch (node->nod_type) {
		case nod_average:
			blr_operator = blr_average;
			break;
		case nod_count:
			blr_operator = blr_count;
/* count2
		blr_operator = node->nod_arg[0]->nod_arg[e_rse_items] ? blr_count2 : blr_count;
*/
			break;
		case nod_from:
			blr_operator = blr_from;
			break;
		case nod_max:
			blr_operator = blr_maximum;
			break;
		case nod_min:
			blr_operator = blr_minimum;
			break;
		case nod_total:
			blr_operator = blr_total;
			break;

		default:
			break;
		}

		stuff(request, blr_operator);
		gen_rse(request, node->nod_arg[0]);
		if (blr_operator != blr_count)
			GEN_expr(request, node->nod_arg[0]->nod_arg[e_rse_items]);
		return;

	default:
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 901,
				  isc_arg_gds, isc_dsql_internal_err,
				  isc_arg_gds, isc_expression_eval_err,
				  // expression evaluation not supported 
				  0);
	}

	stuff(request, blr_operator);

	dsql_nod* const* ptr = node->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->nod_count;
		ptr < end; ptr++)
	{
		GEN_expr(request, *ptr);
	}

/* Check whether the node we just processed is for a dialect 3
   operation which gives a different result than the corresponding
   operation in dialect 1.  If it is, and if the client dialect is 2,
   issue a warning about the difference. */

	if (node->nod_type == nod_add2 ||
		node->nod_type == nod_subtract2 ||
		node->nod_type == nod_multiply2 ||
		node->nod_type == nod_divide2 ||
		node->nod_type == nod_agg_total2 ||
		node->nod_type == nod_agg_average2) 
	{
		dsc desc;
		const char* s = 0;
		char message_buf[8];

		MAKE_desc(&desc, node, NULL);
		if ((node->nod_flags & NOD_COMP_DIALECT) &&
			(request->req_client_dialect == SQL_DIALECT_V6_TRANSITION)) 
		{
			switch (node->nod_type) {
			case nod_add2:
				s = "add";
				break;
			case nod_subtract2:
				s = "subtract";
				break;
			case nod_multiply2:
				s = "multiply";
				break;
			case nod_divide2:
				s = "divide";
				break;
			case nod_agg_total2:
				s = "sum";
				break;
			case nod_agg_average2:
				s = "avg";
				break;
			default:
				sprintf(message_buf, "blr %d", (int) blr_operator);
				s = message_buf;
			}
			ERRD_post_warning(isc_dsql_dialect_warning_expr,
							  isc_arg_string, s, isc_arg_end);
		}
	}

}

/**
  
 	GEN_port
  
    @brief	Generate a port from a message.  Feel free to rearrange the
 	order of parameters.
 

    @param request
    @param message

 **/
void GEN_port( dsql_req* request, dsql_msg* message)
{
	tsql* tdsql = DSQL_get_thread_data();

//	if (request->req_blr_string) {
		stuff(request, blr_message);
		stuff(request, message->msg_number);
		stuff_word(request, message->msg_parameter);
//	}

    dsql_par* parameter;

	USHORT number = 0;
	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
	{
		parameter->par_parameter = number++;

		/* For older clients - generate an error should they try and
		   access data types which did not exist in the older dialect */
		if (request->req_client_dialect <= SQL_DIALECT_V5)
			switch (parameter->par_desc.dsc_dtype) {

				/* In V6.0 - older clients, which we distinguish by
				   their use of SQL DIALECT 0 or 1, are forbidden
				   from selecting values of new datatypes */
				case dtype_sql_date:
				case dtype_sql_time:
				case dtype_int64:
					ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
							  isc_arg_gds, isc_dsql_datatype_err,
							  isc_arg_gds, isc_sql_dialect_datatype_unsupport,
							  isc_arg_number, request->req_client_dialect,
							  isc_arg_string,
							  DSC_dtype_tostring(parameter->par_desc.dsc_dtype),
							  0);
					break;
				default:
					// No special action for other data types 
					break;
			}
		const USHORT align = type_alignments[parameter->par_desc.dsc_dtype];
		if (align)
			message->msg_length = FB_ALIGN(message->msg_length, align);
		parameter->par_desc.dsc_address = (UCHAR*)(IPTR)message->msg_length;
		// CVC: No check for overflow here? Should be < 64K
		message->msg_length += parameter->par_desc.dsc_length;
//		if (request->req_blr_string)
			gen_descriptor(request, &parameter->par_desc, false);
	}

// Allocate buffer for message 
	// CVC: again, final possibility of having overflow! Should be < 64K
	const USHORT new_len = message->msg_length + DOUBLE_ALIGN - 1;
	dsql_str* buffer = FB_NEW_RPT(*tdsql->getDefaultPool(), new_len) dsql_str;
	message->msg_buffer =
		(UCHAR *) FB_ALIGN((U_IPTR) buffer->str_data, DOUBLE_ALIGN);

// Relocate parameter descriptors to point direction into message buffer 

	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
	{
		parameter->par_desc.dsc_address = message->msg_buffer + 
			(IPTR)parameter->par_desc.dsc_address;
	}
}


/**
  
 	GEN_request
  
    @brief	Generate complete blr for a request.
 		       

    @param request
    @param node

 **/
void GEN_request( dsql_req* request, dsql_nod* node)
{
	if (request->req_type == REQ_DDL) {
		DDL_generate(request, node);
		return;
	}

	if (request->req_flags & REQ_blr_version4)
		stuff(request, blr_version4);
	else
		stuff(request, blr_version5);
		
	if (request->req_type == REQ_SAVEPOINT) 
	{
		// Do not generate BEGIN..END block around savepoint statement
		// to avoid breaking of savepoint logic
		request->req_send = NULL;
		request->req_receive = NULL;
		GEN_statement(request, node);
	} 
	else 
	{	
	
		stuff(request, blr_begin);

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT)
		{
      	  gen_select(request, node);
  		}
		else if (request->req_type == REQ_EXEC_BLOCK ||
				 request->req_type == REQ_SELECT_BLOCK )
		{
			GEN_statement(request, node);
  		}
		else {
			dsql_msg* message = request->req_send;
			if (!message->msg_parameter)
				request->req_send = NULL;
			else {
				GEN_port(request, message);
				if (request->req_type != REQ_EXEC_PROCEDURE) {
					stuff(request, blr_receive);
					stuff(request, message->msg_number);
				}
			}
			message = request->req_receive;
			if (!message->msg_parameter)
				request->req_receive = NULL;
			else
				GEN_port(request, message);
			GEN_statement(request, node);
		}		
		stuff(request, blr_end);
	}
	
	stuff(request, blr_eoc);
}


/**
  
 	GEN_start_transaction
  
    @brief	Generate tpb for set transaction.  Use blr string of request.
 	If a value is not specified, default is not STUFF'ed, let the
 	engine handle it.
 	Do not allow an option to be specified more than once.
 

    @param request
    @param tran_node

 **/
void GEN_start_transaction( dsql_req* request, const dsql_nod* tran_node)
{
	SSHORT count = tran_node->nod_count;

	if (!count)
		return;

	const dsql_nod* node = tran_node->nod_arg[0];

	if (!node)
		return;

/* find out isolation level - if specified. This is required for
 * specifying the correct lock level in reserving clause. */

	USHORT lock_level = isc_tpb_shared;

	if (count = node->nod_count) {
		while (count--) {
			const dsql_nod* ptr = node->nod_arg[count];

			if ((!ptr) || (ptr->nod_type != nod_isolation))
				continue;

			lock_level = (ptr->nod_flags & NOD_CONSISTENCY) ?
				isc_tpb_protected : isc_tpb_shared;
		}
	}


   	bool sw_access = false, sw_wait = false, sw_isolation = false,
		sw_reserve = false;

// Stuff some version info.
	if (count = node->nod_count)
		stuff(request, isc_tpb_version1);

	while (count--) {
		const dsql_nod* ptr = node->nod_arg[count];

		if (!ptr)
			continue;

		switch (ptr->nod_type) {
		case nod_access:
			if (sw_access)
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
						  isc_arg_gds, isc_dsql_dup_option, 0);

			sw_access = true;
			if (ptr->nod_flags & NOD_READ_ONLY)
				stuff(request, isc_tpb_read);
			else
				stuff(request, isc_tpb_write);
			break;

		case nod_wait:
			if (sw_wait)
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
						  isc_arg_gds, isc_dsql_dup_option, 0);

			sw_wait = true;
			if (ptr->nod_flags & NOD_NO_WAIT)
				stuff(request, isc_tpb_nowait);
			else
				stuff(request, isc_tpb_wait);
			break;

		case nod_isolation:
			if (sw_isolation)
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
						  isc_arg_gds, isc_dsql_dup_option, 0);

			sw_isolation = true;

			if (ptr->nod_flags & NOD_CONCURRENCY)
				stuff(request, isc_tpb_concurrency);
			else if (ptr->nod_flags & NOD_CONSISTENCY)
				stuff(request, isc_tpb_consistency);
			else {
				stuff(request, isc_tpb_read_committed);

				if ((ptr->nod_count) && (ptr->nod_arg[0]) &&
					(ptr->nod_arg[0]->nod_type == nod_version))
				{
					if (ptr->nod_arg[0]->nod_flags & NOD_VERSION)
						stuff(request, isc_tpb_rec_version);
					else
						stuff(request, isc_tpb_no_rec_version);
				}
				else
					stuff(request, isc_tpb_no_rec_version);
			}

			break;

		case nod_reserve:
			{
				if (sw_reserve)
					ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
							  isc_arg_gds, isc_dsql_dup_option, 0);

				sw_reserve = true;
				const dsql_nod* reserve = ptr->nod_arg[0];

				if (reserve) {
					const dsql_nod* const* temp = reserve->nod_arg;
					for (const dsql_nod* const* end = temp + reserve->nod_count;
						 temp < end; temp++)
					{
						gen_table_lock(request, *temp, lock_level);
					}
				}
				break;
			}

		default:
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
					  isc_arg_gds, isc_dsql_tran_err, 0);
		}
	}
}


/**
  
 	GEN_statement
  
    @brief	Generate blr for an arbitrary expression.
 

    @param request
    @param node

 **/
void GEN_statement( dsql_req* request, dsql_nod* node)
{
	dsql_nod* temp;
	dsql_nod** ptr;
	const dsql_nod* const* end;
	dsql_ctx* context;
	dsql_msg* message;
	dsql_str* name;
	dsql_str* string;
	ULONG id_length;

	switch (node->nod_type) {
	case nod_assign:
		stuff(request, blr_assignment);
		GEN_expr(request, node->nod_arg[0]);
		GEN_expr(request, node->nod_arg[1]);
		return;

	case nod_block:
		stuff(request, blr_block);
		GEN_statement(request, node->nod_arg[e_blk_action]);
		if (node->nod_count > 1) {
			temp = node->nod_arg[e_blk_errs];
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
			{
				GEN_statement(request, *ptr);
			}
		}
		stuff(request, blr_end);
		return;

	case nod_erase:
		if ((temp = node->nod_arg[e_era_rse]) != NULL) {
			stuff(request, blr_for);
			GEN_expr(request, temp);
		}
		temp = node->nod_arg[e_era_relation];
		context = (dsql_ctx*) temp->nod_arg[e_rel_context];
		stuff(request, blr_erase);
		stuff(request, context->ctx_context);
		return;

	case nod_erase_current:
		stuff(request, blr_erase);
		context = (dsql_ctx*) node->nod_arg[e_erc_context];
		stuff(request, context->ctx_context);
		return;

	case nod_exec_block:
		DDL_gen_block(request, node);
		return;

	case nod_exec_procedure:
		if (request->req_type == REQ_EXEC_PROCEDURE) {
			if (message = request->req_receive) {
				stuff(request, blr_begin);
				stuff(request, blr_send);
				stuff(request, message->msg_number);
			}
		}
		else
			message = NULL;
		stuff(request, blr_exec_proc);
		name = (dsql_str*) node->nod_arg[e_exe_procedure];
		stuff_cstring(request, name->str_data);

		if (temp = node->nod_arg[e_exe_inputs]) {
			stuff_word(request, temp->nod_count);
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
 			{
 				GEN_expr(request, *ptr);
 			}
		}
		else
			stuff_word(request, 0);
		if (temp = node->nod_arg[e_exe_outputs]) {
			stuff_word(request, temp->nod_count);
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
			{
				GEN_expr(request, *ptr);
			}
		}
		else
			stuff_word(request, 0);
		if (message)
			stuff(request, blr_end);
		return;

	case nod_for_select:
		gen_for_select(request, node);
		return;

	case nod_set_generator:
	case nod_set_generator2:
		stuff(request, blr_set_generator);
		string = (dsql_str*) node->nod_arg[e_gen_id_name];
		stuff_cstring(request, string->str_data);
		GEN_expr(request, node->nod_arg[e_gen_id_value]);
		return;

	case nod_if:
		stuff(request, blr_if);
		GEN_expr(request, node->nod_arg[e_if_condition]);
		GEN_statement(request, node->nod_arg[e_if_true]);
		if (node->nod_arg[e_if_false])
			GEN_statement(request, node->nod_arg[e_if_false]);
		else
			stuff(request, blr_end);
		return;

	case nod_list:
		stuff(request, blr_begin);
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
		{
			GEN_statement(request, *ptr);
		}
		stuff(request, blr_end);
		return;

	case nod_modify:
		if ((temp = node->nod_arg[e_mod_rse]) != NULL) {
			stuff(request, blr_for);
			GEN_expr(request, temp);
		}
		stuff(request, blr_modify);
		temp = node->nod_arg[e_mod_source];
		context = (dsql_ctx*) temp->nod_arg[e_rel_context];
		stuff(request, context->ctx_context);
		temp = node->nod_arg[e_mod_update];
		context = (dsql_ctx*) temp->nod_arg[e_rel_context];
		stuff(request, context->ctx_context);
		GEN_statement(request, node->nod_arg[e_mod_statement]);
		return;

	case nod_modify_current:
		stuff(request, blr_modify);
		context = (dsql_ctx*) node->nod_arg[e_mdc_context];
		stuff(request, context->ctx_context);
		temp = node->nod_arg[e_mdc_update];
		context = (dsql_ctx*) temp->nod_arg[e_rel_context];
		stuff(request, context->ctx_context);
		GEN_statement(request, node->nod_arg[e_mdc_statement]);
		return;

	case nod_on_error:
		stuff(request, blr_error_handler);
		temp = node->nod_arg[e_err_errs];
		stuff_word(request, temp->nod_count);
		for (ptr = temp->nod_arg, end = ptr + temp->nod_count; ptr < end;
			 ptr++)
		{
			gen_error_condition(request, *ptr);
		}
		GEN_statement(request, node->nod_arg[e_err_action]);
		return;

	case nod_post:
		if ( (temp = node->nod_arg[e_pst_argument]) ) {
			stuff(request, blr_post_arg);
			GEN_expr(request, node->nod_arg[e_pst_event]);
			GEN_expr(request, temp);
		}
		else {
			stuff(request, blr_post);
			GEN_expr(request, node->nod_arg[e_pst_event]);
		}
		return;

	case nod_exec_sql:
		stuff(request, blr_exec_sql);
		GEN_expr(request, node->nod_arg[e_exec_sql_stmnt]);
		return;

	case nod_exec_into:
		if (node->nod_arg[e_exec_into_block]) {
			stuff(request, blr_label);
			stuff(request, (int) (IPTR) node->nod_arg[e_exec_into_label]->nod_arg[e_label_number]);
		}
		stuff(request, blr_exec_into);
		temp = node->nod_arg[e_exec_into_list];
		stuff_word(request, temp->nod_count);
		GEN_expr(request, node->nod_arg[e_exec_into_stmnt]);
		if (node->nod_arg[e_exec_into_block]) {
			stuff(request, 0); // Non-singleton
			GEN_statement(request, node->nod_arg[e_exec_into_block]);
		}
		else
			stuff(request, 1); // Singleton
		for (ptr = temp->nod_arg, end = ptr + temp->nod_count; 
				ptr < end; ptr++)
		{
			GEN_expr(request, *ptr);
		}
		return;
	
	case nod_return:
		if (temp = node->nod_arg[e_rtn_procedure])
		{
			if (temp->nod_type == nod_exec_block)
				GEN_return(request, temp->nod_arg[e_exe_blk_outputs], false);
			else 
				GEN_return(request, temp->nod_arg[e_prc_outputs], false);
		}
		return;

	case nod_exit:
		stuff(request, blr_leave);
		stuff(request, 0);
		return;

    case nod_breakleave:
        stuff(request, blr_leave);
        stuff(request, (int) (IPTR) node->nod_arg[e_breakleave_label]->nod_arg[e_label_number]);
        return;

	case nod_store:
		if ((temp = node->nod_arg[e_sto_rse]) != NULL) {
			stuff(request, blr_for);
			GEN_expr(request, temp);
		}
		stuff(request, blr_store);
		GEN_expr(request, node->nod_arg[e_sto_relation]);
		GEN_statement(request, node->nod_arg[e_sto_statement]);
		return;

	case nod_abort:
		stuff(request, blr_leave);
		stuff(request, (int) (IPTR) node->nod_arg[e_abrt_number]);
		return;

	case nod_start_savepoint:
		stuff(request, blr_start_savepoint);
		return;

	case nod_end_savepoint:
		stuff(request, blr_end_savepoint);
		return;

	case nod_user_savepoint:
		stuff(request, blr_user_savepoint);
		stuff(request, blr_savepoint_set);
		stuff_cstring(request, ((dsql_str*)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_release_savepoint:
		stuff(request, blr_user_savepoint);
		if (node->nod_arg[1]) {
			stuff(request, blr_savepoint_release_single);
		}
		else {
			stuff(request, blr_savepoint_release);
		}
		stuff_cstring(request, ((dsql_str*)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_undo_savepoint:
		stuff(request, blr_user_savepoint);
		stuff(request, blr_savepoint_undo);
		stuff_cstring(request, ((dsql_str*)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_exception_stmt:
		stuff(request, blr_abort);
		string = (dsql_str*) node->nod_arg[e_xcps_name];
		temp = node->nod_arg[e_xcps_msg];
		/* if exception name is undefined,
		   it means we have re-initiate semantics here,
		   so blr_raise verb should be generated */
		if (!string)
		{
			stuff(request, blr_raise);
			return;
		}
		/* if exception value is defined,
		   it means we have user-defined exception message here,
		   so blr_exception_msg verb should be generated */
		if (temp)
		{
			stuff(request, blr_exception_msg);
		}
		/* otherwise go usual way,
		   i.e. generate blr_exception */
		else
		{
			stuff(request, blr_exception);
		}
		if (!(string->str_flags & STR_delimited_id))
		{
			id_length = string->str_length;
			for (TEXT* p = reinterpret_cast<char*>(string->str_data); *p;
				 id_length--)
			{
				*p = UPPER(*p);
				*p++;
			}
		}
		stuff_cstring(request, string->str_data);
		/* if exception value is defined,
		   generate appropriate BLR verbs */
		if (temp)
		{
			GEN_expr(request, temp);
		}
		return;

	case nod_while:
		stuff(request, blr_label);
		stuff(request, (int) (IPTR) node->nod_arg[e_while_label]->nod_arg[e_label_number]);
		stuff(request, blr_loop);
		stuff(request, blr_begin);
		stuff(request, blr_if);
		GEN_expr(request, node->nod_arg[e_while_cond]);
		GEN_statement(request, node->nod_arg[e_while_action]);
		stuff(request, blr_leave);
		stuff(request, (int) (IPTR) node->nod_arg[e_while_label]->nod_arg[e_label_number]);
		stuff(request, blr_end);
		return;

	case nod_sqlcode:
	case nod_gdscode:
		stuff(request, blr_abort);
		gen_error_condition(request, node);
		return;

	case nod_cursor:
		stuff(request, blr_dcl_cursor);
		stuff_word(request, (int) (IPTR) node->nod_arg[e_cur_number]);
		GEN_expr(request, node->nod_arg[e_cur_rse]);
		return;

	case nod_cursor_open:
	case nod_cursor_close:
	case nod_cursor_fetch:
		{
		// op-code
		stuff(request, blr_cursor_stmt);
		if (node->nod_type == nod_cursor_open)
			stuff(request, blr_cursor_open);
		else if (node->nod_type == nod_cursor_close)
			stuff(request, blr_cursor_close);
		else
			stuff(request, blr_cursor_fetch);
		// cursor reference
		dsql_nod* cursor = node->nod_arg[e_cur_stmt_id];
		stuff_word(request, (int) (IPTR) cursor->nod_arg[e_cur_number]);
		// preliminary navigation
		dsql_nod* seek = node->nod_arg[e_cur_stmt_seek];
		if (seek) {
			stuff(request, blr_seek);
			GEN_expr(request, seek->nod_arg[0]);
			GEN_expr(request, seek->nod_arg[1]);
		}
		// assignment
		dsql_nod* list_into = node->nod_arg[e_cur_stmt_into];
		if (list_into) {
			dsql_nod* list = cursor->nod_arg[e_cur_rse]->nod_arg[e_rse_items];
			if (list->nod_count != list_into->nod_count)
				ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 313,
						  isc_arg_gds, isc_dsql_count_mismatch, 0);
			stuff(request, blr_begin);
			ptr = list->nod_arg;
			end = ptr + list->nod_count;
			dsql_nod** ptr_to = list_into->nod_arg;
			while (ptr < end) {
				stuff(request, blr_assignment);
				GEN_expr(request, *ptr++);
				GEN_expr(request, *ptr_to++);
			}
			stuff(request, blr_end);
		}
		}
		return;

	default:
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 901,
				  isc_arg_gds, isc_dsql_internal_err,
				  isc_arg_gds, isc_node_err, // gen.c: node not supported
				  0);
	}
}


/**
  
 	gen_aggregate
  
    @brief	Generate blr for a relation reference.
 

    @param 
    @param 

 **/
static void gen_aggregate( dsql_req* request, const dsql_nod* node)
{
	const dsql_ctx* context = (dsql_ctx*) node->nod_arg[e_agg_context];
	stuff(request, blr_aggregate);
	stuff(request, context->ctx_context);
	gen_rse(request, node->nod_arg[e_agg_rse]);

// Handle GROUP BY clause 

	stuff(request, blr_group_by);

	dsql_nod* list = node->nod_arg[e_agg_group];
	if (list != NULL) {
		stuff(request, list->nod_count);
		dsql_nod** ptr = list->nod_arg;
		for (const dsql_nod* const* end = ptr + list->nod_count; ptr < end;
			 ptr++)
		{
			GEN_expr(request, *ptr);
		}
	}
	else
		stuff(request, 0);

// Generate value map 

	gen_map(request, context->ctx_map);
}


/**
  
 gen_cast
  
    @brief      Generate BLR for a data-type cast operation
 

    @param request
    @param node

 **/
static void gen_cast( dsql_req* request, const dsql_nod* node)
{
	stuff(request, blr_cast);
	const dsql_fld* field = (dsql_fld*) node->nod_arg[e_cast_target];
	DDL_put_field_dtype(request, field, true);
	GEN_expr(request, node->nod_arg[e_cast_source]);
}


/**
  
 gen_coalesce
  
    @brief      Generate BLR for coalesce function

	Generate the blr values, begin with a cast and then :

	blr_value_if
	blr_missing
	blr for expression 1
		blr_value_if
		blr_missing
		blr for expression n
		blr_null
	blr for expression n-1

    @param request
    @param node

 **/
static void gen_coalesce( dsql_req* request, const dsql_nod* node)
{
	// blr_value_if is used for building the coalesce function
	dsql_nod* list = node->nod_arg[0];
	stuff(request, blr_cast);
	gen_descriptor(request, &node->nod_desc, true);
	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count;
		ptr < end; ptr++)
	{
		// IF (expression IS NULL) THEN
		stuff(request, blr_value_if);
		stuff(request, blr_missing);
		GEN_expr(request, *ptr);
	}
	// Return values
	list = node->nod_arg[1];
	const dsql_nod* const* const begin = list->nod_arg;
	ptr = list->nod_arg + list->nod_count;
	// if all expressions are NULL return NULL
	stuff(request, blr_null);
	for (ptr--; ptr >= begin; ptr--)
	{
		GEN_expr(request, *ptr);
	}
}


/**
  
 	gen_constant
  
    @brief	Generate BLR for a constant.
 

    @param request
    @param desc
    @param negate_value

 **/
static void gen_constant( dsql_req* request, dsc* desc, bool negate_value)
{
	SLONG value;
	SINT64 i64value;

	DSC tmp_desc;

	stuff(request, blr_literal);

	if ((desc->dsc_dtype == dtype_double)
		&& (request->req_dbb->dbb_flags & DBB_v3))
		// v3 doesn't understand blr_double literal, generate blr_text instead 
	{
		tmp_desc = *desc;
		tmp_desc.dsc_dtype = dtype_text;
		tmp_desc.dsc_length = desc->dsc_scale;	// length of string literal 
		tmp_desc.dsc_scale = 0;
		desc = &tmp_desc;
	}

	USHORT l = desc->dsc_length;
	const UCHAR* p = desc->dsc_address;

	switch (desc->dsc_dtype) {
	case dtype_short:
		gen_descriptor(request, desc, true);
		value = *(SSHORT *) p;
		if (negate_value)
			value = -value;
		stuff_word(request, value);
		break;

	case dtype_long:
		gen_descriptor(request, desc, true);
		value = *(SLONG *) p;
		if (negate_value)
			value = -value;
		stuff_word(request, value);
		stuff_word(request, value >> 16);
		break;

	case dtype_sql_time:
	case dtype_sql_date:
		gen_descriptor(request, desc, true);
		value = *(SLONG *) p;
		stuff_word(request, value);
		stuff_word(request, value >> 16);
		break;

	case dtype_double:
		/* this is used for approximate/large numeric literal
		   which is transmitted to the engine as a string.
		 */
		gen_descriptor(request, desc, true);
		l = (USHORT) desc->dsc_scale;	// length of string literal 
		if (negate_value) {
			stuff_word(request, l + 1);
			stuff(request, '-');
		}
		else {
			stuff_word(request, l);
		}

		if (l)
			do {
				stuff(request, *p++);
			} while (--l);
		break;

	case dtype_int64:
		i64value = *(SINT64 *) p;

		if (negate_value)
			i64value = -i64value;
		else if (i64value == MIN_SINT64) {
			/* UH OH!
			 * yylex correctly recognized the digits as the most-negative
			 * possible INT64 value, but unfortunately, there was no
			 * preceding '-' (a fact which the lexer could not know).
			 * The value is too big for a positive INT64 value, and it
			 * didn't contain an exponent so it's not a valid DOUBLE
			 * PRECISION literal either, so we have to bounce it.
			 */
			ERRD_post(isc_sqlerr,
					  isc_arg_number, (SLONG) - 104,
					  isc_arg_gds, isc_arith_except, 0);
		}

		/* We and the lexer both agree that this is an SINT64 constant,
		   * and if the value needed to be negated, it already has been.
		   * If the value will fit into a 32-bit signed integer, generate
		   * it that way, else as an INT64.
		 */

		if ((i64value >= (SINT64) MIN_SLONG) &&
			(i64value <= (SINT64) MAX_SLONG))
		{
			stuff(request, blr_long);
			stuff(request, desc->dsc_scale);
			stuff_word(request, i64value);
			stuff_word(request, i64value >> 16);
			break;
		}
		else {
			stuff(request, blr_int64);
			stuff(request, desc->dsc_scale);
			stuff_word(request, i64value);
			stuff_word(request, i64value >> 16);
			stuff_word(request, i64value >> 32);
			stuff_word(request, i64value >> 48);
		}
		break;

	case dtype_quad:
	case dtype_blob:
	case dtype_array:
	case dtype_timestamp:
		gen_descriptor(request, desc, true);
		value = *(SLONG *) p;
		stuff_word(request, value);
		stuff_word(request, value >> 16);
		value = *(SLONG *) (p + 4);
		stuff_word(request, value);
		stuff_word(request, value >> 16);
		break;

	case dtype_text:
		gen_descriptor(request, desc, true);
		if (l)
			do {
				stuff(request, *p++);
			} while (--l);
		break;

	default:
		// gen_constant: datatype not understood 
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 103,
				  isc_arg_gds, isc_dsql_constant_err, 0);
	}
}


/**
  
 	gen_descriptor
  
    @brief	Generate a blr descriptor from an internal descriptor.
 

    @param request
    @param desc
    @param texttype

 **/
static void gen_descriptor( dsql_req* request, const dsc* desc, bool texttype)
{
	switch (desc->dsc_dtype) {
	case dtype_text:
		if (request->req_dbb->dbb_flags & DBB_v3)
			stuff(request, blr_text);
		else if (texttype || desc->dsc_ttype() == ttype_binary) {
			stuff(request, blr_text2);
			stuff_word(request, desc->dsc_ttype());
		}
		else {
			stuff(request, blr_text2);	// automatic transliteration 
			stuff_word(request, ttype_dynamic);
		}

		stuff_word(request, desc->dsc_length);
		break;

	case dtype_varying:
		if (request->req_dbb->dbb_flags & DBB_v3)
			stuff(request, blr_varying);
		else if (texttype || desc->dsc_ttype() == ttype_binary) {
			stuff(request, blr_varying2);
			stuff_word(request, desc->dsc_ttype());
		}
		else {
			stuff(request, blr_varying2);	// automatic transliteration 
			stuff_word(request, ttype_dynamic);
		}
		stuff_word(request, desc->dsc_length - sizeof(USHORT));
		break;

	case dtype_short:
		stuff(request, blr_short);
		stuff(request, desc->dsc_scale);
		break;

	case dtype_long:
		stuff(request, blr_long);
		stuff(request, desc->dsc_scale);
		break;

	case dtype_quad:
		stuff(request, blr_quad);
		stuff(request, desc->dsc_scale);
		break;

	case dtype_int64:
		stuff(request, blr_int64);
		stuff(request, desc->dsc_scale);
		break;

	case dtype_real:
		stuff(request, blr_float);
		break;

	case dtype_double:
		stuff(request, blr_double);
		break;

	case dtype_sql_date:
		stuff(request, blr_sql_date);
		break;

	case dtype_sql_time:
		stuff(request, blr_sql_time);
		break;

	case dtype_timestamp:
		stuff(request, blr_timestamp);
		break;

	case dtype_blob:
	case dtype_array:
		stuff(request, blr_quad);
		stuff(request, 0);
		break;

	default:
		// don't understand dtype 
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				  isc_arg_gds, isc_dsql_datatype_err, 0);
	}
}


/**
  
 	gen_error_condition
  
    @brief	Generate blr for an error condtion
 

    @param request
    @param node

 **/
static void gen_error_condition( dsql_req* request, const dsql_nod* node)
{
	const dsql_str* string;

	switch (node->nod_type) {
	case nod_sqlcode:
		stuff(request, blr_sql_code);
		stuff_word(request, (USHORT)(IPTR) node->nod_arg[0]);
		return;

	case nod_gdscode:
		stuff(request, blr_gds_code);
		string = (dsql_str*) node->nod_arg[0];
		stuff_cstring(request, string->str_data);
		return;

	case nod_exception:
		stuff(request, blr_exception);
		string = (dsql_str*) node->nod_arg[0];
		stuff_cstring(request, string->str_data);
		return;

	case nod_default:
		stuff(request, blr_default_code);
		return;

	default:
		fb_assert(false);
		return;
	}
}


/**
  
 	gen_field
  
    @brief	Generate blr for a field - field id's
 	are preferred but not for trigger or view blr.
 

    @param request
    @param context
    @param field
    @param indices

 **/
static void gen_field( dsql_req* request, const dsql_ctx* context,
	const dsql_fld* field, dsql_nod* indices)
{
/* For older clients - generate an error should they try and
 *    access data types which did not exist in the older dialect */
	if (request->req_client_dialect <= SQL_DIALECT_V5) {
		switch (field->fld_dtype) {
		case dtype_sql_date:
		case dtype_sql_time:
		case dtype_int64:
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
					  isc_arg_gds, isc_dsql_datatype_err,
					  isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					  isc_arg_number, request->req_client_dialect,
					  isc_arg_string,
					  DSC_dtype_tostring(static_cast < UCHAR >
										 (field->fld_dtype)), 0);
			break;
		default:
			// No special action for other data types 
			break;
		}
	}

	if (indices)
		stuff(request, blr_index);

	if (DDL_ids(request)) {
		stuff(request, blr_fid);
		stuff(request, context->ctx_context);
		stuff_word(request, field->fld_id);
	}
	else {
		stuff(request, blr_field);
		stuff(request, context->ctx_context);
		stuff_cstring(request, field->fld_name);
	}

	if (indices) {
		stuff(request, indices->nod_count);
		dsql_nod** ptr = indices->nod_arg;
		for (const dsql_nod* const* end = ptr + indices->nod_count;
			 ptr < end; ptr++)
		{
			GEN_expr(request, *ptr);
		}
	}
}


/**
  
 	gen_for_select
  
    @brief	Generate BLR for a SELECT statement.
 

    @param request
    @param for_select

 **/
static void gen_for_select( dsql_req* request, dsql_nod* for_select)
{
	dsql_nod* rse = for_select->nod_arg[e_flp_select];

    /* CVC: Only put a label if this is not singular; otherwise,
       what loop is the user trying to abandon? */
    if (for_select->nod_arg[e_flp_action]) {
        stuff(request, blr_label);
        stuff(request, (int) (IPTR) for_select->nod_arg[e_flp_label]->nod_arg[e_label_number]);
    }

// Generate FOR loop 

	stuff(request, blr_for);

	if (!for_select->nod_arg[e_flp_action] &&
		!(request->req_dbb->dbb_flags & DBB_v3))
	{
		stuff(request, blr_singular);
	}
	gen_rse(request, rse);
	stuff(request, blr_begin);

// Build body of FOR loop 

	// Handle write locks 
	dsql_nod* streams = rse->nod_arg[e_rse_streams];
	dsql_ctx* context = NULL;

	if (!rse->nod_arg[e_rse_reduced] && streams->nod_count == 1) {
		dsql_nod* item = streams->nod_arg[0];
		if (item && (item->nod_type == nod_relation))
			context = (dsql_ctx*) item->nod_arg[e_rel_context];
	}
	
	dsql_nod* list = rse->nod_arg[e_rse_items];
	dsql_nod* list_to = for_select->nod_arg[e_flp_into];
	if (list->nod_count != list_to->nod_count)
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 313,
				  isc_arg_gds, isc_dsql_count_mismatch, 0);
	dsql_nod** ptr = list->nod_arg;
	dsql_nod** ptr_to = list_to->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end;
		ptr++, ptr_to++) 
	{
		stuff(request, blr_assignment);
		GEN_expr(request, *ptr);
		GEN_expr(request, *ptr_to);
	}
	if (for_select->nod_arg[e_flp_action])
		GEN_statement(request, for_select->nod_arg[e_flp_action]);
	stuff(request, blr_end);
}


/**
  
 gen_gen_id
  
    @brief      Generate BLR for gen_id
 

    @param request
    @param node

 **/
static void gen_gen_id( dsql_req* request, const dsql_nod* node)
{
	stuff(request, blr_gen_id);
	const dsql_str* string = (dsql_str*) node->nod_arg[e_gen_id_name];
	stuff_cstring(request, string->str_data);
	GEN_expr(request, node->nod_arg[e_gen_id_value]);
}


/**
  
 	gen_join_rse
  
    @brief	Generate a record selection expression
 	with an explicit join type.
 

    @param request
    @param rse

 **/
static void gen_join_rse( dsql_req* request, const dsql_nod* rse)
{
	stuff(request, blr_rs_stream);
	stuff(request, 2);

	GEN_expr(request, rse->nod_arg[e_join_left_rel]);
	GEN_expr(request, rse->nod_arg[e_join_rght_rel]);

	const dsql_nod* node = rse->nod_arg[e_join_type];
	if (node->nod_type != nod_join_inner) {
		stuff(request, blr_join_type);
		if (node->nod_type == nod_join_left)
			stuff(request, blr_left);
		else if (node->nod_type == nod_join_right)
			stuff(request, blr_right);
		else
			stuff(request, blr_full);
	}

	if (rse->nod_arg[e_join_boolean])
	{
		stuff(request, blr_boolean);
		GEN_expr(request, rse->nod_arg[e_join_boolean]);
	}

	stuff(request, blr_end);
}


/**
  
 	gen_map
  
    @brief	Generate a value map for a record selection expression.
 

    @param request
    @param map

 **/
static void gen_map( dsql_req* request, dsql_map* map)
{
	USHORT count = 0;
	dsql_map* temp;
	for (temp = map; temp; temp = temp->map_next)
		temp->map_position = count++;

	stuff(request, blr_map);
	stuff_word(request, count);

	for (temp = map; temp; temp = temp->map_next) {
		stuff_word(request, temp->map_position);
		GEN_expr(request, temp->map_node);
	}
}


/**
  
 	gen_parameter
  
    @brief	Generate a parameter reference.
 

    @param request
    @param parameter

 **/
static void gen_parameter( dsql_req* request, const dsql_par* parameter)
{
	const dsql_msg* message = parameter->par_message;

	const dsql_par* null = parameter->par_null;
	if (null != NULL) {
		stuff(request, blr_parameter2);
		stuff(request, message->msg_number);
		stuff_word(request, parameter->par_parameter);
		stuff_word(request, null->par_parameter);
		return;
	}

	stuff(request, blr_parameter);
	stuff(request, message->msg_number);
	stuff_word(request, parameter->par_parameter);
}



/**
  
 	gen_plan
  
    @brief	Generate blr for an access plan expression.
 

    @param request
    @param plan_expression

 **/
static void gen_plan( dsql_req* request, const dsql_nod* plan_expression)
{
// stuff the join type 

	const dsql_nod* list = plan_expression->nod_arg[1];
	if (list->nod_count > 1) {
		if (plan_expression->nod_arg[0])
			stuff(request, blr_merge);
		else
			stuff(request, blr_join);
		stuff(request, list->nod_count);
	}

// stuff one or more plan items 

	const dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; 
		ptr++) 
	{
		const dsql_nod* node = *ptr;
		if (node->nod_type == nod_plan_expr) {
			gen_plan(request, node);
			continue;
		}

		// if we're here, it must be a nod_plan_item 

		stuff(request, blr_retrieve);

		/* stuff the relation--the relation id itself is redundant except 
		   when there is a need to differentiate the base tables of views */

		const dsql_nod* arg = node->nod_arg[0];
		gen_relation(request, (dsql_ctx*) arg->nod_arg[e_rel_context]);

		// now stuff the access method for this stream 
		const dsql_str* index_string;

		arg = node->nod_arg[1];
		switch (arg->nod_type) {
		case nod_natural:
			stuff(request, blr_sequential);
			break;

		case nod_index_order:
			stuff(request, blr_navigational);
			index_string = (dsql_str*) arg->nod_arg[0];
			stuff_cstring(request, index_string->str_data);
			if (!arg->nod_arg[1])
				break;
			// dimitr: FALL INTO, if the plan item is ORDER ... INDEX (...)

		case nod_index:
			{
				stuff(request, blr_indices);
				arg = (arg->nod_type == nod_index) ?
					arg->nod_arg[0] : arg->nod_arg[1];
				stuff(request, arg->nod_count);
				const dsql_nod* const* ptr2 = arg->nod_arg;
				for (const dsql_nod* const* const end2 = ptr2 + arg->nod_count;
					 ptr2 < end2; ptr2++) 
				{
					index_string = (dsql_str*) * ptr2;
					stuff_cstring(request, index_string->str_data);
				}
				break;
			}

		default:
			fb_assert(false);
			break;
		}

	}
}



/**
  
 	gen_relation
  
    @brief	Generate blr for a relation reference.
 

    @param request
    @param context

 **/
static void gen_relation( dsql_req* request, dsql_ctx* context)
{
	const dsql_rel* relation = context->ctx_relation;
	const dsql_prc* procedure = context->ctx_procedure;

// if this is a trigger or procedure , don't want relation id used 
	if (relation) {
		if (DDL_ids(request)) {
			if (context->ctx_alias)
				stuff(request, blr_rid2);
			else
				stuff(request, blr_rid);
			stuff_word(request, relation->rel_id);
		}
		else {
			if (context->ctx_alias)
				stuff(request, blr_relation2);
			else
				stuff(request, blr_relation);
			stuff_cstring(request, relation->rel_name);
		}

		if (context->ctx_alias)
			stuff_cstring(request, context->ctx_alias);
		stuff(request, context->ctx_context);
	}
	else if (procedure) {
		if (DDL_ids(request)) {
			stuff(request, blr_pid);
			stuff_word(request, procedure->prc_id);
		}
		else {
			stuff(request, blr_procedure);
			stuff_cstring(request, procedure->prc_name);
		}
		stuff(request, context->ctx_context);

		dsql_nod* inputs = context->ctx_proc_inputs;
		if (inputs) {
			stuff_word(request, inputs->nod_count);

			dsql_nod* const* ptr = inputs->nod_arg;
			for (const dsql_nod* const* const end = ptr + inputs->nod_count;
				 ptr < end; ptr++)
			{
 				GEN_expr(request, *ptr);
			}
 		}
		else
			stuff_word(request, 0);
	}
}


/**
  
 	gen_return
  
    @brief	Generate blr for a procedure return.
 

    @param request
    @param procedure
    @param eos_flag

 **/
void GEN_return( dsql_req* request, const dsql_nod* parameters, bool eos_flag)
{
	if (!eos_flag)
		stuff(request, blr_begin);
	stuff(request, blr_send);
	stuff(request, 1);
	stuff(request, blr_begin);

	USHORT outputs = 0;
	if (parameters) {
		const dsql_nod* const* ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count;
			 ptr < end; ptr++)
		{
			outputs++;
			const dsql_nod* parameter = *ptr;
			const dsql_var* variable = (dsql_var*) parameter->nod_arg[e_var_variable];
			stuff(request, blr_assignment);
			stuff(request, blr_variable);
			stuff_word(request, variable->var_variable_number);
			stuff(request, blr_parameter2);
			stuff(request, variable->var_msg_number);
			stuff_word(request, variable->var_msg_item);
			stuff_word(request, variable->var_msg_item + 1);
		}
	}
	stuff(request, blr_assignment);
	stuff(request, blr_literal);
	stuff(request, blr_short);
	stuff(request, 0);
	if (eos_flag)
		stuff_word(request, 0);
	else
		stuff_word(request, 1);
	stuff(request, blr_parameter);
	stuff(request, 1);
	stuff_word(request, 2 * outputs);
	stuff(request, blr_end);
	if (!eos_flag) {
		stuff(request, blr_stall);
		stuff(request, blr_end);
	}
}


/**
  
 	gen_rse
  
    @brief	Generate a record selection expression.
 

    @param request
    @param rse

 **/
static void gen_rse( dsql_req* request, const dsql_nod* rse)
{
	if ((rse->nod_flags & NOD_SELECT_EXPR_SINGLETON)
		&& !(request->req_dbb->dbb_flags & DBB_v3))
	{
			stuff(request, blr_singular);
	}

	stuff(request, blr_rse);

	dsql_nod* list = rse->nod_arg[e_rse_streams];

// Handle source streams

	if (list->nod_type == nod_union) {
		stuff(request, 1);
		gen_union(request, rse);
	}
	else if (list->nod_type == nod_list) {
		stuff(request, list->nod_count);
		dsql_nod* const* ptr = list->nod_arg;
		for (const dsql_nod* const* const end = ptr + list->nod_count;
			ptr < end; ptr++)
		{
			dsql_nod* node = *ptr;
			if (node->nod_type == nod_relation ||
				node->nod_type == nod_aggregate ||
				node->nod_type == nod_join)
			{
				GEN_expr(request, node);
			}
			else if (node->nod_type == nod_derived_table) {
				GEN_expr(request, node->nod_arg[e_derived_table_rse]);
			}
		}
	}
	else {
		stuff(request, 1);
		GEN_expr(request, list);
	}

	if (rse->nod_arg[e_rse_lock])
		stuff(request, blr_writelock);

	dsql_nod* node;

	if ((node = rse->nod_arg[e_rse_first]) != NULL) {
		stuff(request, blr_first);
		GEN_expr(request, node);
	}

    if ((node = rse->nod_arg[e_rse_skip]) != NULL) {
        stuff(request, blr_skip);
        GEN_expr (request, node);
    }

	if ((node = rse->nod_arg[e_rse_boolean]) != NULL) {
		stuff(request, blr_boolean);
		GEN_expr(request, node);
	}

	if ((list = rse->nod_arg[e_rse_sort]) != NULL)
		gen_sort(request, list);

	if ((list = rse->nod_arg[e_rse_reduced]) != NULL) {
		stuff(request, blr_project);
		stuff(request, list->nod_count);
		dsql_nod** ptr = list->nod_arg;
		for (const dsql_nod* const* const end = ptr + list->nod_count;
			ptr < end; ptr++)
		{
			GEN_expr(request, *ptr);
		}
	}

// if the user specified an access plan to use, add it here 

	if ((node = rse->nod_arg[e_rse_plan]) != NULL) {
		stuff(request, blr_plan);
		gen_plan(request, node);
	}

#ifdef SCROLLABLE_CURSORS
/* generate a statement to be executed if the user scrolls 
   in a direction other than forward; a message is sent outside 
   the normal send/receive protocol to specify the direction 
   and offset to scroll; note that we do this only on a SELECT 
   type statement and only when talking to a 4.1 engine or greater */

	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5)
	{
		stuff(request, blr_receive);
		stuff(request, request->req_async->msg_number);
		stuff(request, blr_seek);
		const dsql_par* parameter = request->req_async->msg_parameters;
		gen_parameter(request, parameter->par_next);
		gen_parameter(request, parameter);
	}
#endif

	stuff(request, blr_end);
}


/**
  
 gen_searched_case
  
    @brief      Generate BLR for CASE function (searched)
 

    @param request
    @param node

 **/
static void gen_searched_case( dsql_req* request, const dsql_nod* node)
{
	// blr_value_if is used for building the case expression

	stuff(request, blr_cast);
	gen_descriptor(request, &node->nod_desc, true);
	const SSHORT count =
		node->nod_arg[e_searched_case_search_conditions]->nod_count;
	dsql_nod* boolean_list = node->nod_arg[e_searched_case_search_conditions];
	dsql_nod* results_list = node->nod_arg[e_searched_case_results];
	dsql_nod* const* bptr = boolean_list->nod_arg;
	dsql_nod* const* rptr = results_list->nod_arg;
	for (const dsql_nod* const* const end = bptr + count; bptr < end;
		bptr++, rptr++)
	{
		stuff(request, blr_value_if);
		GEN_expr(request, *bptr);
		GEN_expr(request, *rptr);
	}
	// else_result
	GEN_expr(request, node->nod_arg[e_searched_case_results]->nod_arg[count]);
}


/**
  
 	gen_select
  
    @brief	Generate BLR for a SELECT statement.
 

    @param request
    @param rse

 **/
static void gen_select( dsql_req* request, dsql_nod* rse)
{
	const dsql_fld* field;
	const dsql_rel* relation;
	dsql_ctx* context;
	
	SSHORT constant;
	dsc constant_desc;
	constant_desc.dsc_dtype = dtype_short;
	constant_desc.dsc_scale = 0;
	constant_desc.dsc_sub_type = 0;
	constant_desc.dsc_flags = 0;
	constant_desc.dsc_length = sizeof(SSHORT);
	constant_desc.dsc_address = (UCHAR*) & constant;

// Set up parameter for things in the select list 
	dsql_nod* list = rse->nod_arg[e_rse_items];
	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end;
		ptr++) 
	{
		dsql_nod* item = *ptr;
		dsql_par* parameter = MAKE_parameter(request->req_receive, true, true, 0);
		parameter->par_node = item;
		MAKE_desc(&parameter->par_desc, item, NULL);
		const char* name_alias = NULL;

		switch (item->nod_type) {
		case nod_field: {
			field = (dsql_fld*) item->nod_arg[e_fld_field];
			name_alias = field->fld_name;
			context = (dsql_ctx*) item->nod_arg[e_fld_context];
			if (context->ctx_relation) {
				parameter->par_rel_name = context->ctx_relation->rel_name;
				parameter->par_owner_name = context->ctx_relation->rel_owner;
			}
			else if (context->ctx_procedure) {
				parameter->par_rel_name = context->ctx_procedure->prc_name;
				parameter->par_owner_name = context->ctx_procedure->prc_owner;
			}
			break;
			}
		case nod_dbkey: {
			parameter->par_name = parameter->par_alias = db_key_name;
			context = (dsql_ctx*) item->nod_arg[0]->nod_arg[0];
			parameter->par_rel_name = context->ctx_relation->rel_name;
			parameter->par_owner_name = context->ctx_relation->rel_owner;
			break;
			}
		case nod_alias: {
			const dsql_str* string = (dsql_str*) item->nod_arg[e_alias_alias];
			parameter->par_alias = reinterpret_cast<const TEXT*>(string->str_data);
			dsql_nod* alias = item->nod_arg[e_alias_value];
			if (alias->nod_type == nod_field) {
				field = (dsql_fld*) alias->nod_arg[e_fld_field];
				parameter->par_name = field->fld_name;
				context = (dsql_ctx*) alias->nod_arg[e_fld_context];
				if (context->ctx_relation) {
					parameter->par_rel_name = context->ctx_relation->rel_name;
					parameter->par_owner_name =
						context->ctx_relation->rel_owner;
				}
				else if (context->ctx_procedure) {
					parameter->par_rel_name =
						context->ctx_procedure->prc_name;
					parameter->par_owner_name =
						context->ctx_procedure->prc_owner;
				}
			}
			else if (alias->nod_type == nod_dbkey) {
				parameter->par_name = db_key_name;
				context = (dsql_ctx*) alias->nod_arg[0]->nod_arg[0];
				parameter->par_rel_name = context->ctx_relation->rel_name;
				parameter->par_owner_name = context->ctx_relation->rel_owner;
			}
			break;
			}
		case nod_derived_field: {
			const dsql_str* string = (dsql_str*) item->nod_arg[e_derived_field_name];
			parameter->par_alias = reinterpret_cast<const TEXT*>(string->str_data);
			dsql_nod* alias = item->nod_arg[e_derived_field_value];
			if (alias->nod_type == nod_field) {
				field = (dsql_fld*) alias->nod_arg[e_fld_field];
				parameter->par_name = field->fld_name;
				context = (dsql_ctx*) alias->nod_arg[e_fld_context];
				if (context->ctx_relation) {
					parameter->par_rel_name = context->ctx_relation->rel_name;
					parameter->par_owner_name =
						context->ctx_relation->rel_owner;
				}
				else if (context->ctx_procedure) {
					parameter->par_rel_name =
						context->ctx_procedure->prc_name;
					parameter->par_owner_name =
						context->ctx_procedure->prc_owner;
				}
			}
			else if (alias->nod_type == nod_dbkey) {
				parameter->par_name = db_key_name;
				context = (dsql_ctx*) alias->nod_arg[0]->nod_arg[0];
				parameter->par_rel_name = context->ctx_relation->rel_name;
				parameter->par_owner_name = context->ctx_relation->rel_owner;
			}
			break;
			}
		case nod_map: {
			const dsql_map* map = (dsql_map*) item->nod_arg[e_map_map];
			const dsql_nod* map_node = map->map_node;
			while (map_node->nod_type == nod_map) {
				// skip all the nod_map nodes
				map = (dsql_map*) map_node->nod_arg[e_map_map];
				map_node = map->map_node;
			}
			switch (map_node->nod_type) {
			case nod_field: {
				field = (dsql_fld*) map_node->nod_arg[e_fld_field];
				name_alias = field->fld_name;
				break;
				}
			case nod_alias: {
				const dsql_str* string = (dsql_str*) map_node->nod_arg[e_alias_alias];
				parameter->par_alias = reinterpret_cast<const TEXT*>(string->str_data);
				dsql_nod* alias = map_node->nod_arg[e_alias_value];
				if (alias->nod_type == nod_field) {
					field = (dsql_fld*) alias->nod_arg[e_fld_field];
					parameter->par_name = field->fld_name;
				}
				break;
				}
			case nod_derived_field: {
				const dsql_str* string = (dsql_str*) map_node->nod_arg[e_derived_field_name];
				parameter->par_alias = reinterpret_cast<const TEXT*>(string->str_data);
				dsql_nod* alias = map_node->nod_arg[e_derived_field_value];
				if (alias->nod_type == nod_field) {
					field = (dsql_fld*) alias->nod_arg[e_fld_field];
					parameter->par_name = field->fld_name;
				}
				break;
				}

			case nod_agg_count:
				name_alias = "COUNT";
				break;
			case nod_agg_total:
				name_alias = "SUM";
				break;
			case nod_agg_average:
				name_alias = "AVG";
				break;
			case nod_agg_total2:
				name_alias = "SUM";
				break;
			case nod_agg_average2:
				name_alias = "AVG";
				break;
			case nod_agg_min:
				name_alias = "MIN";
				break;
			case nod_agg_max:
				name_alias = "MAX";
				break;
			} // switch(map_node->nod_type)
			break;
			} // case nod_map
		case nod_udf: 
			{
			dsql_udf* userFunc = (dsql_udf*) item->nod_arg[0];
			name_alias = userFunc->udf_name;
			break;
			}
		case nod_gen_id:
			name_alias	= "GEN_ID";
			break;
		case nod_gen_id2:
			name_alias	= "GEN_ID";
			break;
		case nod_user_name:
			name_alias	= "USER";
			break;
		case nod_current_role:
			name_alias	= "ROLE";
			break;
		case nod_internal_info:
			{
			internal_info_id id =
				*reinterpret_cast<internal_info_id*>(item->nod_arg[0]->nod_desc.dsc_address);
			name_alias = InternalInfo::getAlias(id);
			break;
			}
		case nod_concatenate:
			name_alias = "CONCATENATION";
			break;
		case nod_substr:
			name_alias = "SUBSTRING";
			break;
		case nod_cast:
			name_alias	= "CAST";
			break;
		case nod_upcase:
			name_alias	= "UPPER";
			break;
		case nod_current_date:
			name_alias = "CURRENT_DATE";
			break;
		case nod_current_time:
			name_alias = "CURRENT_TIME";
			break;
		case nod_current_timestamp:
			name_alias = "CURRENT_TIMESTAMP";
			break;
		case nod_extract:
			name_alias = "EXTRACT";
			break;
		case nod_searched_case:
		case nod_simple_case:
			name_alias = "CASE";
			break;
		case nod_coalesce:
			name_alias = "COALESCE";
			break;
		} // end switch(item->nod_type)
		if (name_alias)
			parameter->par_name = parameter->par_alias = name_alias;
	} // for (ptr = list->nod_arg

// Set up parameter to handle EOF 

	{
		dsql_par* parameter_eof = MAKE_parameter(request->req_receive, false, false, 0);
		request->req_eof = parameter_eof;
		parameter_eof->par_desc.dsc_dtype = dtype_short;
		parameter_eof->par_desc.dsc_scale = 0;
		parameter_eof->par_desc.dsc_length = sizeof(SSHORT);
	}

// Save DBKEYs for possible update later

	list = rse->nod_arg[e_rse_streams];

	if (!rse->nod_arg[e_rse_reduced]) {
		ptr = list->nod_arg;
		for (const dsql_nod* const* const end2 = ptr + list->nod_count; 
			ptr < end2; ptr++) 
		{
			dsql_nod* item = *ptr;
			if (item && item->nod_type == nod_relation) {
				context = (dsql_ctx*) item->nod_arg[e_rel_context];
				if (relation = context->ctx_relation) {
					// Set up dbkey
					dsql_par* parameter =
						MAKE_parameter(request->req_receive, false, false, 0);
					parameter->par_dbkey_ctx = context;
					parameter->par_desc.dsc_dtype = dtype_text;
					parameter->par_desc.dsc_ttype() = ttype_binary;
					parameter->par_desc.dsc_length =
						relation->rel_dbkey_length;

					// Set up record version - for post v33 databases

					if (!(request->req_dbb->dbb_flags & DBB_v3)) {
						parameter =
							MAKE_parameter(request->req_receive, false,
										   false, 0);
						parameter->par_rec_version_ctx = context;
						parameter->par_desc.dsc_dtype = dtype_text;
						parameter->par_desc.dsc_ttype() = ttype_binary;
						parameter->par_desc.dsc_length =
							relation->rel_dbkey_length / 2;
					}
				}
			}
		}
	}

#ifdef SCROLLABLE_CURSORS
/* define the parameters for the scrolling message--offset and direction, 
   in that order to make it easier to generate the request */

	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5) 
	{
		dsql_par* parameter = MAKE_parameter(request->req_async, false, false, 0);
		parameter->par_desc.dsc_dtype = dtype_short;
		parameter->par_desc.dsc_length = sizeof(USHORT);
		parameter->par_desc.dsc_scale = 0;
		parameter->par_desc.dsc_flags = 0;
		parameter->par_desc.dsc_sub_type = 0;

		parameter = MAKE_parameter(request->req_async, false, false, 0);
		parameter->par_desc.dsc_dtype = dtype_long;
		parameter->par_desc.dsc_length = sizeof(ULONG);
		parameter->par_desc.dsc_scale = 0;
		parameter->par_desc.dsc_flags = 0;
		parameter->par_desc.dsc_sub_type = 0;
	}
#endif

// Generate definitions for the messages 

	GEN_port(request, request->req_receive);
	dsql_msg* message = request->req_send;
	if (message->msg_parameter)
		GEN_port(request, message);
	else
		request->req_send = NULL;
#ifdef SCROLLABLE_CURSORS
	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5)
			GEN_port(request, request->req_async);
#endif

// If there is a send message, build a RECEIVE

	if ((message = request->req_send) != NULL) {
		stuff(request, blr_receive);
		stuff(request, message->msg_number);
	}

// Generate FOR loop

	message = request->req_receive;

	stuff(request, blr_for);
	if (!(request->req_dbb->dbb_flags & DBB_v3))
		stuff(request, blr_stall);
	gen_rse(request, rse);	

	stuff(request, blr_send);
	stuff(request, message->msg_number);
	stuff(request, blr_begin);

// Build body of FOR loop

	// Add invalid usage here
	
	stuff(request, blr_assignment);
	constant = 1;
	gen_constant(request, &constant_desc, USE_VALUE);
	gen_parameter(request, request->req_eof);

	for (dsql_par* parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
	{
		if (parameter->par_node) {
			stuff(request, blr_assignment);
			GEN_expr(request, parameter->par_node);
			gen_parameter(request, parameter);
		}
		if (context = parameter->par_dbkey_ctx) {
			stuff(request, blr_assignment);
			stuff(request, blr_dbkey);
			stuff(request, context->ctx_context);
			gen_parameter(request, parameter);
		}
		if (context = parameter->par_rec_version_ctx) {
			stuff(request, blr_assignment);
			stuff(request, blr_record_version);
			stuff(request, context->ctx_context);
			gen_parameter(request, parameter);
		}
	}

	stuff(request, blr_end);
	stuff(request, blr_send);
	stuff(request, message->msg_number);
	stuff(request, blr_assignment);
	constant = 0;
	gen_constant(request, &constant_desc, USE_VALUE);
	gen_parameter(request, request->req_eof);
}


/**
  
 gen_simple_case
  
    @brief      Generate BLR for CASE function (simple)
 

    @param request
    @param node

 **/
static void gen_simple_case( dsql_req* request, const dsql_nod* node)
{
	// blr_value_if is used for building the case expression 

	stuff(request, blr_cast);
	gen_descriptor(request, &node->nod_desc, true);
	SSHORT count = node->nod_arg[e_simple_case_when_operands]->nod_count;
	dsql_nod* when_list = node->nod_arg[e_simple_case_when_operands];
	dsql_nod* results_list = node->nod_arg[e_simple_case_results];

	dsql_nod* const* wptr = when_list->nod_arg;
	dsql_nod* const* rptr = results_list->nod_arg;
	for (const dsql_nod* const* const end = wptr + count; wptr < end;
		wptr++, rptr++)
	{
		stuff(request, blr_value_if);
		stuff(request, blr_eql);
		GEN_expr(request, node->nod_arg[e_simple_case_case_operand]);
		GEN_expr(request, *wptr);
		GEN_expr(request, *rptr);
	}
	// else_result 
	GEN_expr(request, node->nod_arg[e_simple_case_results]->nod_arg[count]); 
}


/**
  
 	gen_sort
  
    @brief	Generate a sort clause.
 

    @param request
    @param list

 **/
static void gen_sort( dsql_req* request, dsql_nod* list)
{
	stuff(request, blr_sort);
	stuff(request, list->nod_count);

	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end;
		ptr++)
	{
		dsql_nod* nulls_placement = (*ptr)->nod_arg[e_order_nulls];
		if (nulls_placement) {
			switch ((IPTR)nulls_placement->nod_arg[0]) {
				case NOD_NULLS_FIRST:
					stuff(request, blr_nullsfirst);
					break;
				case NOD_NULLS_LAST:
					stuff(request, blr_nullslast);
					break;
			}
		}
		if ((*ptr)->nod_arg[e_order_flag])
			stuff(request, blr_descending);
		else
			stuff(request, blr_ascending);
		GEN_expr(request, (*ptr)->nod_arg[e_order_field]);
	}
}


/**
  
 	gen_table_lock
  
    @brief	Generate tpb for table lock.
 	If lock level is specified, it overrrides the transaction lock level.
 

    @param request
    @param tbl_lock
    @param lock_level

 **/
static void gen_table_lock( dsql_req* request, const dsql_nod* tbl_lock,
	USHORT lock_level)
{
	if ((!tbl_lock) || (tbl_lock->nod_type != nod_table_lock))
		return;

	const dsql_nod* tbl_names = tbl_lock->nod_arg[e_lock_tables];
	SSHORT flags = 0;

	if (tbl_lock->nod_arg[e_lock_mode])
		flags = tbl_lock->nod_arg[e_lock_mode]->nod_flags;

	if (flags & NOD_PROTECTED)
		lock_level = isc_tpb_protected;
	else if (flags & NOD_SHARED)
		lock_level = isc_tpb_shared;

	const USHORT lock_mode = (flags & NOD_WRITE) ? 
		isc_tpb_lock_write : isc_tpb_lock_read;

    const dsql_nod* const* ptr = tbl_names->nod_arg;
	for (const dsql_nod* const* const end = ptr + tbl_names->nod_count;
		 ptr < end; ptr++)
	{
		if ((*ptr)->nod_type != nod_relation_name)
			continue;

		stuff(request, lock_mode);

		// stuff table name 
		const dsql_str* temp = (dsql_str*) ((*ptr)->nod_arg[e_rln_name]);
		stuff_cstring(request, reinterpret_cast<const char*>(temp->str_data));

		stuff(request, lock_level);
	}
}


/**
  
 	gen_udf
  
    @brief	Generate a user defined function.
 

    @param request
    @param node

 **/
static void gen_udf( dsql_req* request, const dsql_nod* node)
{
	const dsql_udf* userFunc = (dsql_udf*) node->nod_arg[0];
	stuff(request, blr_function);
	stuff_cstring(request, userFunc->udf_name);

	const dsql_nod* list;
	if ((node->nod_count == 2) && (list = node->nod_arg[1])) {
		stuff(request, list->nod_count);
		dsql_nod* const* ptr = list->nod_arg;
		for (const dsql_nod* const* const end = ptr + list->nod_count;
			ptr < end; ptr++)
		{
			GEN_expr(request, *ptr);
		}
	}
	else
		stuff(request, 0);
}


/**
  
 	gen_union
  
    @brief	Generate a union of substreams.
 

    @param request
    @param union_node

 **/
static void gen_union( dsql_req* request, const dsql_nod* union_node)
{
	stuff(request, blr_union);

// Obtain the context for UNION from the first dsql_map* node
	dsql_nod* items = union_node->nod_arg[e_rse_items];
	dsql_nod* map_item = items->nod_arg[0];
	// AB: First item could be a virtual field generated by derived table.
	if (map_item->nod_type == nod_derived_field) {
		map_item = map_item->nod_arg[e_alias_value]; 
	}
	dsql_ctx* union_context = (dsql_ctx*) map_item->nod_arg[e_map_context];
	stuff(request, union_context->ctx_context);

	dsql_nod* streams = union_node->nod_arg[e_rse_streams];
	stuff(request, streams->nod_count);	// number of substreams 

    dsql_nod** ptr = streams->nod_arg;
	for (const dsql_nod* const* const end = ptr + streams->nod_count; ptr < end;
		 ptr++)
	{
		dsql_nod* sub_rse = *ptr;
		gen_rse(request, sub_rse);
		items = sub_rse->nod_arg[e_rse_items];
		stuff(request, blr_map);
		stuff_word(request, items->nod_count);
		USHORT count = 0;
		dsql_nod** iptr = items->nod_arg;
		for (const dsql_nod* const* const iend = iptr + items->nod_count;
			 iptr < iend; iptr++)
		{
			stuff_word(request, count);
			GEN_expr(request, *iptr);
			count++;
		}
	}
}


/**
  
 	stuff_cstring
  
    @brief	Write out a string with one byte of length.
 

    @param request
    @param string

 **/
static void stuff_cstring( dsql_req* request, const char* string)
{
	UCHAR c;

	stuff(request, strlen(string));
	while ((c = *string++))
		stuff(request, c);
}


/**
  
 	stuff_word
  
    @brief	Cram a word into the blr buffer.  If the buffer is getting
 	ready to overflow, expand it.
 

    @param request
    @param word

 **/
static void stuff_word( dsql_req* request, USHORT word)
{

	stuff(request, word);
	stuff(request, word >> 8);
}

