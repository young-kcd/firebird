/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		gen.c
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
 */
/*
$Id: gen.cpp,v 1.33.2.4 2005-06-16 09:44:39 arnobrinkman Exp $
*/

#include "firebird.h"
#include <string.h>
#include <stdio.h>
#include "../dsql/dsql.h"
#include "../jrd/gds.h"
#include "../jrd/align.h"
#include "../dsql/node.h"
#include "../jrd/intl.h"
#include "../dsql/sym.h"
#include "../dsql/alld_proto.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/misc_func.h"
#include "../jrd/thd_proto.h"
#include "../jrd/dsc_proto.h"
#include "gen/iberror.h"

ASSERT_FILENAME static void gen_aggregate(DSQL_REQ, DSQL_NOD);
static void gen_cast(DSQL_REQ, DSQL_NOD);
static void	gen_coalesce(DSQL_REQ, DSQL_NOD);
static void gen_constant(DSQL_REQ, DSC *, BOOLEAN);
static void gen_descriptor(DSQL_REQ, DSC *, USHORT);
static void gen_error_condition(DSQL_REQ, DSQL_NOD);
static void gen_field(DSQL_REQ, DSQL_CTX, DSQL_FLD, DSQL_NOD);
static void gen_for_select(DSQL_REQ, DSQL_NOD);
static void gen_gen_id(DSQL_REQ, DSQL_NOD);
static void gen_join_rse(DSQL_REQ, DSQL_NOD);
static void gen_map(DSQL_REQ, MAP);
static void gen_parameter(DSQL_REQ, PAR);
static void gen_plan(DSQL_REQ, DSQL_NOD);
static void gen_relation(DSQL_REQ, DSQL_CTX);
static void gen_rse(DSQL_REQ, DSQL_NOD);
static void	gen_searched_case(DSQL_REQ, DSQL_NOD);
static void gen_select(DSQL_REQ, DSQL_NOD);
static void	gen_simple_case(DSQL_REQ, DSQL_NOD);
static void gen_sort(DSQL_REQ, DSQL_NOD);
static void gen_table_lock(DSQL_REQ, DSQL_NOD, USHORT);
static void gen_udf(DSQL_REQ, DSQL_NOD);
static void gen_union(DSQL_REQ, DSQL_NOD);
static void stuff_cstring(DSQL_REQ, char *);
static void stuff_word(DSQL_REQ, USHORT);

static const SCHAR db_key_name[] = "DB_KEY";

/* STUFF is defined in dsql.h for use in common with ddl.c */

#define STUFF_WORD(word)	stuff_word (request, word)
#define STUFF_CSTRING(cstring)	stuff_cstring (request, (char*) (cstring))

/* The following are passed as the third argument to gen_constant */
#define NEGATE_VALUE TRUE
#define USE_VALUE    FALSE


/**

    GEN_expand_buffer

    @brief The blr buffer needs to be expanded.
    

    @param request
    @param byte

**/
UCHAR GEN_expand_buffer( DSQL_REQ request, UCHAR byte)
{
	TSQL tdsql = GET_THREAD_DATA;

	const ULONG length = request->req_blr_string->str_length + 2048;
	// AB: We must define a maximum length and post an error when exceeded else 
	// the server can crash with a huge SQL command.

	const bool bIsPermanentPool = MemoryPool::blk_pool(request->req_blr_string) == DSQL_permanent_pool;
	DsqlMemoryPool* pool = bIsPermanentPool ? DSQL_permanent_pool : tdsql->tsql_default;
	STR new_buffer = FB_NEW_RPT(*pool, length) str;
	new_buffer->str_length = length;

	// one huge pointer per line for LIBS
	// TMN: What does that mean???

	char*       p   = new_buffer->str_data;
	const char* q   = request->req_blr_string->str_data;
	BLOB_PTR*   end = request->req_blr;
	const size_t copy_length = (reinterpret_cast<char*>(end) - q);
	memcpy(p, q, copy_length);

	delete request->req_blr_string;
	request->req_blr_string = new_buffer;
	request->req_blr = reinterpret_cast<BLOB_PTR*>(p + copy_length);
	request->req_blr_yellow = reinterpret_cast<BLOB_PTR*>(new_buffer->str_data + length);

	return (*request->req_blr++ = byte);
}


/**
  
 	GEN_expr
  
    @brief	Generate blr for an arbitrary expression.
 

    @param request
    @param node

 **/
void GEN_expr( DSQL_REQ request, DSQL_NOD node)
{
	UCHAR operator_;
	DSQL_NOD *ptr, *end, ddl_node;
	DSQL_CTX context;
	MAP map;
	VAR variable;

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

	case nod_extract:
		STUFF(blr_extract);
		STUFF(*(SLONG *) node->nod_arg[e_extract_part]->nod_desc.dsc_address);
		GEN_expr(request, node->nod_arg[e_extract_value]);
		return;

	case nod_dbkey:
		node = node->nod_arg[0];
		context = (DSQL_CTX) node->nod_arg[e_rel_context];
		STUFF(blr_dbkey);
		STUFF(context->ctx_context);
		return;

	case nod_rec_version:
		node = node->nod_arg[0];
		context = (DSQL_CTX) node->nod_arg[e_rel_context];
		STUFF(blr_record_version);
		STUFF(context->ctx_context);
		return;

	case nod_dom_value:
		if ((request->req_type != REQ_DDL) ||
			!(ddl_node = request->req_ddl_node) ||
			!(ddl_node->nod_type == nod_def_domain ||
			  ddl_node->nod_type == nod_mod_domain))
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 901,
						  gds_arg_gds, gds_dsql_domain_err, 0);
		STUFF(blr_fid);
		STUFF(0);				/* Context   */
		STUFF_WORD(0);			/* Field id  */
		return;

	case nod_field:
		gen_field(request,
				  (DSQL_CTX) node->nod_arg[e_fld_context],
				  (DSQL_FLD) node->nod_arg[e_fld_field],
				  node->nod_arg[e_fld_indices]);
		return;

	case nod_user_name:
		STUFF(blr_user_name);
		return;

	case nod_current_time:
		STUFF(blr_current_time);
		return;

	case nod_current_timestamp:
		STUFF(blr_current_timestamp);
		return;

	case nod_current_date:
		STUFF(blr_current_date);
		return;

    case nod_current_role:
        STUFF (blr_current_role);
        return;

	case nod_udf:
		gen_udf(request, node);
		return;

	case nod_variable:
		variable = (VAR) node->nod_arg[e_var_variable];
		if (variable->var_flags & VAR_input) {
			STUFF(blr_parameter2);
			STUFF(variable->var_msg_number);
			STUFF_WORD(variable->var_msg_item);
			STUFF_WORD(variable->var_msg_item + 1);
		}
		else {
			STUFF(blr_variable);
			STUFF_WORD(variable->var_variable_number);
		}
		return;

	case nod_join:
		gen_join_rse(request, node);
		return;

	case nod_map:
		map = (MAP) node->nod_arg[e_map_map];
		context = (DSQL_CTX) node->nod_arg[e_map_context];
		STUFF(blr_fid);
		STUFF(context->ctx_context);
		STUFF_WORD(map->map_position);
		return;

	case nod_parameter:
		gen_parameter(request, (PAR) node->nod_arg[e_par_parameter]);
		return;

	case nod_relation:
		gen_relation(request, (DSQL_CTX) node->nod_arg[e_rel_context]);
		return;

	case nod_rse:
		gen_rse(request, node);
		return;

	case nod_exists:
		STUFF(blr_any);
		gen_rse(request, node->nod_arg[0]);
		return;

	case nod_singular:
		STUFF(blr_unique);
		gen_rse(request, node->nod_arg[0]);
		return;

	case nod_agg_count:
		if (!(request->req_dbb->dbb_flags & DBB_v3)) {
			if (node->nod_count)
				operator_ = (node->nod_flags & NOD_AGG_DISTINCT) ?
					blr_agg_count_distinct : blr_agg_count2;
			else
				operator_ = blr_agg_count;
		}
		else
			operator_ = blr_agg_count;
		break;

	case nod_agg_min:
		operator_ = blr_agg_min;
		break;
	case nod_agg_max:
		operator_ = blr_agg_max;
		break;

	case nod_agg_average:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			operator_ = (node->nod_flags & NOD_AGG_DISTINCT) ?
				blr_agg_average_distinct : blr_agg_average;
		else
			operator_ = blr_agg_average;
		break;

	case nod_agg_total:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			operator_ = (node->nod_flags & NOD_AGG_DISTINCT) ?
				blr_agg_total_distinct : blr_agg_total;
		else
			operator_ = blr_agg_total;
		break;

	case nod_agg_average2:
		operator_ = (node->nod_flags & NOD_AGG_DISTINCT) ?
			blr_agg_average_distinct : blr_agg_average;
		break;

	case nod_agg_total2:
		operator_ = (node->nod_flags & NOD_AGG_DISTINCT) ?
			blr_agg_total_distinct : blr_agg_total;
		break;

	case nod_and:
		operator_ = blr_and;
		break;
	case nod_or:
		operator_ = blr_or;
		break;
	case nod_not:
		operator_ = blr_not;
		break;
	case nod_eql_all:
	case nod_eql_any:
	case nod_eql:
		operator_ = blr_eql;
		break;
	case nod_neq_all:
	case nod_neq_any:
	case nod_neq:
		operator_ = blr_neq;
		break;
	case nod_gtr_all:
	case nod_gtr_any:
	case nod_gtr:
		operator_ = blr_gtr;
		break;
	case nod_leq_all:
	case nod_leq_any:
	case nod_leq:
		operator_ = blr_leq;
		break;
	case nod_geq_all:
	case nod_geq_any:
	case nod_geq:
		operator_ = blr_geq;
		break;
	case nod_lss_all:
	case nod_lss_any:
	case nod_lss:
		operator_ = blr_lss;
		break;
	case nod_between:
		operator_ = blr_between;
		break;
	case nod_containing:
		operator_ = blr_containing;
		break;
	case nod_starting:
		operator_ = blr_starting;
		break;
	case nod_missing:
		operator_ = blr_missing;
		break;

	case nod_like:
		operator_ = (node->nod_count == 2) ? blr_like : blr_ansi_like;
		break;

	case nod_add:
		operator_ = blr_add;
		break;
	case nod_subtract:
		operator_ = blr_subtract;
		break;
	case nod_multiply:
		operator_ = blr_multiply;
		break;

	case nod_negate:
		{
			DSQL_NOD child = node->nod_arg[0];
			if (child->nod_type == nod_constant &&
				DTYPE_IS_NUMERIC(child->nod_desc.dsc_dtype)) {
				gen_constant(request, &child->nod_desc, NEGATE_VALUE);
				return;
			}
		}
		operator_ = blr_negate;
		break;

	case nod_divide:
		operator_ = blr_divide;
		break;
	case nod_add2:
		operator_ = blr_add;
		break;
	case nod_subtract2:
		operator_ = blr_subtract;
		break;
	case nod_multiply2:
		operator_ = blr_multiply;
		break;
	case nod_divide2:
		operator_ = blr_divide;
		break;
	case nod_concatenate:
		operator_ = blr_concatenate;
		break;
	case nod_null:
		operator_ = blr_null;
		break;
	case nod_any:
		operator_ = blr_any;
		break;
	case nod_ansi_any:
		if (!(request->req_dbb->dbb_flags & DBB_v3))
			operator_ = blr_ansi_any;
		else
			operator_ = blr_any;
		break;
	case nod_ansi_all:
		operator_ = blr_ansi_all;
		break;
	case nod_via:
		operator_ = blr_via;
		break;

	case nod_internal_info:
		operator_ = blr_internal_info;
		break;
	case nod_upcase:
		operator_ = blr_upcase;
		break;
	case nod_substr:	
        operator_ = blr_substring;		
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
			operator_ = blr_average;
			break;
		case nod_count:
			operator_ = blr_count;
/* count2
		operator_ = node->nod_arg [0]->nod_arg [e_rse_items] ? blr_count2 : blr_count;
*/
			break;
		case nod_from:
			operator_ = blr_from;
			break;
		case nod_max:
			operator_ = blr_maximum;
			break;
		case nod_min:
			operator_ = blr_minimum;
			break;
		case nod_total:
			operator_ = blr_total;
			break;

		default:
			break;
		}

		STUFF(operator_);
		gen_rse(request, node->nod_arg[0]);
		if (operator_ != blr_count)
			GEN_expr(request, node->nod_arg[0]->nod_arg[e_rse_items]);
		return;

	default:
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 901,
				  gds_arg_gds, gds_dsql_internal_err,
				  gds_arg_gds, gds_expression_eval_err,
				  /* expression evaluation not supported */
				  0);
	}

	STUFF(operator_);

	for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		GEN_expr(request, *ptr);

/* Check whether the node we just processed is for a dialect 3
   operation which gives a different result than the corresponding
   operation in dialect 1.  If it is, and if the client dialect is 2,
   issue a warning about the difference. */

	if (node->nod_type == nod_add2 ||
		node->nod_type == nod_subtract2 ||
		node->nod_type == nod_multiply2 ||
		node->nod_type == nod_divide2 ||
		node->nod_type == nod_agg_total2 ||
		node->nod_type == nod_agg_average2) {
		DSC desc;
		char *s;
		char message_buf[8];

		MAKE_desc(&desc, node);
		if ((node->nod_flags & NOD_COMP_DIALECT) &&
			(request->req_client_dialect == SQL_DIALECT_V6_TRANSITION)) {
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
				sprintf(message_buf, "blr %d", operator_);
				s = message_buf;
			}
			ERRD_post_warning(isc_dsql_dialect_warning_expr,
							  gds_arg_string, s, gds_arg_end);
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
void GEN_port( DSQL_REQ request, DSQL_MSG message)
{
	STR buffer;
	PAR parameter;
	USHORT number, align;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	if (request->req_blr_string) {
		STUFF(blr_message);
		STUFF(message->msg_number);
		STUFF_WORD(message->msg_parameter);
	}

	for (parameter = message->msg_parameters, number = 0; parameter;
		 parameter = parameter->par_next) {
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
					ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
							  gds_arg_gds, gds_dsql_datatype_err,
							  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
							  gds_arg_number, request->req_client_dialect,
							  gds_arg_string,
							  DSC_dtype_tostring(parameter->par_desc.dsc_dtype),
							  0);
					break;
				default:
					/* No special action for other data types */
					break;
			};
		align = type_alignments[parameter->par_desc.dsc_dtype];
		if (align)
			message->msg_length = FB_ALIGN(message->msg_length, align);
		parameter->par_desc.dsc_address =
			(UCHAR *) (SLONG) message->msg_length;
		message->msg_length += parameter->par_desc.dsc_length;
		if (request->req_blr_string)
			gen_descriptor(request, &parameter->par_desc, FALSE);
	}

/* Allocate buffer for message */

	buffer = FB_NEW_RPT(*tdsql->tsql_default, message->msg_length + DOUBLE_ALIGN - 1) str;
	message->msg_buffer =
		(UCHAR *) FB_ALIGN((U_IPTR) buffer->str_data, DOUBLE_ALIGN);

/* Relocate parameter descriptors to point direction into message buffer */

	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next)
			parameter->par_desc.dsc_address =
			message->msg_buffer + (SLONG) parameter->par_desc.dsc_address;
}


/**
  
 	GEN_request
  
    @brief	Generate complete blr for a request.
 		       

    @param request
    @param node

 **/
void GEN_request( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_MSG message;

	if (request->req_type == REQ_DDL) {
		DDL_generate(request, node);
		return;
	}

	if (request->req_flags & REQ_blr_version4)
		STUFF(blr_version4);
	else
		STUFF(blr_version5);
		
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
	
		STUFF(blr_begin);

		if (request->req_type == REQ_SELECT ||
			request->req_type == REQ_SELECT_UPD ||
			request->req_type == REQ_EMBED_SELECT) {
      	  gen_select(request, node);
  		}
		else {
			message = request->req_send;
			if (!message->msg_parameter)
				request->req_send = NULL;
			else {
				GEN_port(request, message);
				if (request->req_type != REQ_EXEC_PROCEDURE) {
					STUFF(blr_receive);
					STUFF(message->msg_number);
				}
			}
			message = request->req_receive;
			if (!message->msg_parameter)
				request->req_receive = NULL;
			else
				GEN_port(request, message);
			GEN_statement(request, node);
		}		
		STUFF(blr_end);
	}
	
	STUFF(blr_eoc);
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
void GEN_start_transaction( DSQL_REQ request, DSQL_NOD tran_node)
{
	DSQL_NOD *temp, ptr, *end;
	SSHORT count;
	DSQL_NOD reserve, node;
	SSHORT sw_access, sw_wait, sw_isolation, sw_reserve;
	USHORT lock_level;

	count = tran_node->nod_count;

	if (!count)
		return;

	sw_access = sw_wait = sw_isolation = sw_reserve = 0;

	node = tran_node->nod_arg[0];

	if (!node)
		return;

/* find out isolation level - if specified. This is required for
 * specifying the correct lock level in reserving clause. */

	lock_level = gds_tpb_shared;

	if (count = node->nod_count) {
		while (count--) {
			ptr = node->nod_arg[count];

			if ((!ptr) || (ptr->nod_type != nod_isolation))
				continue;

			lock_level =
				(ptr->
				 nod_flags & NOD_CONSISTENCY) ? gds_tpb_protected :
				gds_tpb_shared;
		}
	}

/* Stuff some version info. */

	if (count = node->nod_count)
		STUFF(gds_tpb_version1);

	while (count--) {
		ptr = node->nod_arg[count];

		if (!ptr)
			continue;

		switch (ptr->nod_type) {
		case nod_access:
			if (sw_access)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, gds_dsql_dup_option, 0);

			sw_access = 1;
			if (ptr->nod_flags & NOD_READ_ONLY)
				STUFF(gds_tpb_read);
			else
				STUFF(gds_tpb_write);
			break;

		case nod_wait:
			if (sw_wait)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, gds_dsql_dup_option, 0);

			sw_wait = 1;
			if (ptr->nod_flags & NOD_NO_WAIT)
				STUFF(gds_tpb_nowait);
			else
				STUFF(gds_tpb_wait);
			break;

		case nod_isolation:
			if (sw_isolation)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, gds_dsql_dup_option, 0);

			sw_isolation = 1;

			if (ptr->nod_flags & NOD_CONCURRENCY)
				STUFF(gds_tpb_concurrency);
			else if (ptr->nod_flags & NOD_CONSISTENCY)
				STUFF(gds_tpb_consistency);
			else {
				STUFF(gds_tpb_read_committed);

				if ((ptr->nod_count) && (ptr->nod_arg[0]) &&
					(ptr->nod_arg[0]->nod_type == nod_version)) {
					if (ptr->nod_arg[0]->nod_flags & NOD_VERSION)
						STUFF(gds_tpb_rec_version);
					else
						STUFF(gds_tpb_no_rec_version);
				}
				else
					STUFF(gds_tpb_no_rec_version);
			}

			break;

		case nod_reserve:
			if (sw_reserve)
				ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
						  gds_arg_gds, gds_dsql_dup_option, 0);

			sw_reserve = 1;
			reserve = ptr->nod_arg[0];

			if (reserve)
				for (temp = reserve->nod_arg, end = temp + reserve->nod_count;
					 temp < end; temp++)
					gen_table_lock(request, *temp, lock_level);

			break;

		default:
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
					  gds_arg_gds, gds_dsql_tran_err, 0);
		}
	}
}


/**
  
 	GEN_statement
  
    @brief	Generate blr for an arbitrary expression.
 

    @param request
    @param node

 **/
void GEN_statement( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_NOD temp, *ptr, *end;
	DSQL_CTX context;
	DSQL_MSG message;
	STR name, string;
	TEXT *p;
	ULONG id_length;

	switch (node->nod_type) {
	case nod_assign:
		STUFF(blr_assignment);
		GEN_expr(request, node->nod_arg[0]);
		GEN_expr(request, node->nod_arg[1]);
		return;

	case nod_block:
		STUFF(blr_block);
		GEN_statement(request, node->nod_arg[e_blk_action]);
		if (node->nod_count > 1) {
			temp = node->nod_arg[e_blk_errs];
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
				GEN_statement(request, *ptr);
		}
		STUFF(blr_end);
		return;

	case nod_erase:
		if ((temp = node->nod_arg[e_era_rse]) != NULL) {
			STUFF(blr_for);
			GEN_expr(request, temp);
		}
		temp = node->nod_arg[e_era_relation];
		context = (DSQL_CTX) temp->nod_arg[e_rel_context];
		STUFF(blr_erase);
		STUFF(context->ctx_context);
		return;

	case nod_erase_current:
		STUFF(blr_erase);
		context = (DSQL_CTX) node->nod_arg[e_erc_context];
		STUFF(context->ctx_context);
		return;

	case nod_exec_procedure:
		if (request->req_type == REQ_EXEC_PROCEDURE) {
			if (message = request->req_receive) {
				STUFF(blr_begin);
				STUFF(blr_send);
				STUFF(message->msg_number);
			}
		}
		else
			message = NULL;
		STUFF(blr_exec_proc);
		name = (STR) node->nod_arg[e_exe_procedure];
		STUFF_CSTRING(name->str_data);
		if (temp = node->nod_arg[e_exe_inputs]) {
			STUFF_WORD(temp->nod_count);
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
				GEN_expr(request, *ptr);
		}
		else
			STUFF_WORD(0);
		if (temp = node->nod_arg[e_exe_outputs]) {
			STUFF_WORD(temp->nod_count);
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count;
				 ptr < end; ptr++)
				GEN_expr(request, *ptr);
		}
		else
			STUFF_WORD(0);
		if (message)
			STUFF(blr_end);
		return;

	case nod_for_select:
		gen_for_select(request, node);
		return;

	case nod_set_generator:
	case nod_set_generator2:
		STUFF(blr_set_generator);
		string = (STR) node->nod_arg[e_gen_id_name];
		STUFF_CSTRING(string->str_data);
		GEN_expr(request, node->nod_arg[e_gen_id_value]);
		return;

	case nod_if:
		STUFF(blr_if);
		GEN_expr(request, node->nod_arg[e_if_condition]);
		GEN_statement(request, node->nod_arg[e_if_true]);
		if (node->nod_arg[e_if_false])
			GEN_statement(request, node->nod_arg[e_if_false]);
		else
			STUFF(blr_end);
		return;

	case nod_list:
		STUFF(blr_begin);
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end;
			 ptr++)
			GEN_statement(request, *ptr);
		STUFF(blr_end);
		return;

	case nod_modify:
		if ((temp = node->nod_arg[e_mod_rse]) != NULL) {
			STUFF(blr_for);
			GEN_expr(request, temp);
		}
		STUFF(blr_modify);
		temp = node->nod_arg[e_mod_source];
		context = (DSQL_CTX) temp->nod_arg[e_rel_context];
		STUFF(context->ctx_context);
		temp = node->nod_arg[e_mod_update];
		context = (DSQL_CTX) temp->nod_arg[e_rel_context];
		STUFF(context->ctx_context);
		GEN_statement(request, node->nod_arg[e_mod_statement]);
		return;

	case nod_modify_current:
		STUFF(blr_modify);
		context = (DSQL_CTX) node->nod_arg[e_mdc_context];
		STUFF(context->ctx_context);
		temp = node->nod_arg[e_mdc_update];
		context = (DSQL_CTX) temp->nod_arg[e_rel_context];
		STUFF(context->ctx_context);
		GEN_statement(request, node->nod_arg[e_mdc_statement]);
		return;

	case nod_on_error:
		STUFF(blr_error_handler);
		temp = node->nod_arg[e_err_errs];
		STUFF_WORD(temp->nod_count);
		for (ptr = temp->nod_arg, end = ptr + temp->nod_count; ptr < end;
			 ptr++)
			gen_error_condition(request, *ptr);
		GEN_statement(request, node->nod_arg[e_err_action]);
		return;

	case nod_post:
		if ( (temp = node->nod_arg[e_pst_argument]) ) {
			STUFF(blr_post_arg);
			GEN_expr(request, node->nod_arg[e_pst_event]);
			GEN_expr(request, temp);
		}
		else {
			STUFF(blr_post);
			GEN_expr(request, node->nod_arg[e_pst_event]);
		}
		return;

	case nod_exec_sql:
		STUFF(blr_exec_sql);
		GEN_expr(request, node->nod_arg[e_exec_sql_stmnt]);
		return;

	case nod_exec_into:
		if (node->nod_arg[e_exec_into_block]) {
			STUFF(blr_label);
			STUFF((int)(SLONG) node->nod_arg[e_exec_into_number]);
		}
		STUFF(blr_exec_into);
		temp = node->nod_arg[e_exec_into_list];
		STUFF_WORD(temp->nod_count);
		GEN_expr(request, node->nod_arg[e_exec_into_stmnt]);
		if (node->nod_arg[e_exec_into_block]) {
			STUFF(0); // Non-singleton
			GEN_statement(request, node->nod_arg[e_exec_into_block]);
		}
		else
			STUFF(1); // Singleton
		for (ptr = temp->nod_arg, end = ptr + temp->nod_count; 
				ptr < end; ptr++)
			GEN_expr(request, *ptr);
		return;

	case nod_return:
		GEN_return(request, node->nod_arg[e_rtn_procedure], FALSE);
		return;

	case nod_exit:
		STUFF(blr_leave);
		STUFF(0);
		return;

    case nod_label:
        STUFF(blr_label);
        STUFF((int)(SLONG) node->nod_arg[e_label_number]);
        return;
	
    case nod_breakleave:
        STUFF(blr_leave);
        STUFF((int)(SLONG) node->nod_arg[e_breakleave_number]);
        return;

	case nod_store:
		if ((temp = node->nod_arg[e_sto_rse]) != NULL) {
			STUFF(blr_for);
			GEN_expr(request, temp);
		}
		STUFF(blr_store);
		GEN_expr(request, node->nod_arg[e_sto_relation]);
		GEN_statement(request, node->nod_arg[e_sto_statement]);
		return;

	case nod_abort:
		STUFF(blr_leave);
		STUFF((int)(SLONG) node->nod_arg[e_abrt_number]);
		return;

	case nod_start_savepoint:
		STUFF(blr_start_savepoint);
		return;

	case nod_end_savepoint:
		STUFF(blr_end_savepoint);
		return;

	case nod_user_savepoint:
		STUFF(blr_user_savepoint);
		STUFF(blr_savepoint_set);
		STUFF_CSTRING(((STR)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_release_savepoint:
		STUFF(blr_user_savepoint);
		if (node->nod_arg[1]) {
			STUFF(blr_savepoint_release_single);
		}
		else {
			STUFF(blr_savepoint_release);
		}
		STUFF_CSTRING(((STR)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_undo_savepoint:
		STUFF(blr_user_savepoint);
		STUFF(blr_savepoint_undo);
		STUFF_CSTRING(((STR)node->nod_arg[e_sav_name])->str_data);
		return;

	case nod_exception_stmt:
		STUFF(blr_abort);
		string = (STR) node->nod_arg[e_xcp_name];
		temp = node->nod_arg[e_xcp_msg];
		/* if exception name is undefined,
		   it means we have re-initiate semantics here,
		   so blr_raise verb should be generated */
		if (!string)
		{
			STUFF(blr_raise);
			return;
		}
		/* if exception value is defined,
		   it means we have user-defined exception message here,
		   so blr_exception_msg verb should be generated */
		if (temp)
		{
			STUFF(blr_exception_msg);
		}
		/* otherwise go usual way,
		   i.e. generate blr_exception */
		else
		{
			STUFF(blr_exception);
		}
		if (!(string->str_flags & STR_delimited_id))
		{
			id_length = string->str_length;
			for (p = reinterpret_cast<char*>(string->str_data); *p;
				 id_length--)
			{
				*p = UPPER(*p);
				*p++;
			}
		}
		STUFF_CSTRING(string->str_data);
		/* if exception value is defined,
		   generate appropriate BLR verbs */
		if (temp)
		{
			GEN_expr(request, temp);
		}
		return;

	case nod_while:
		STUFF(blr_label);
		STUFF((int)(SLONG) node->nod_arg[e_while_number]);
		STUFF(blr_loop);
		STUFF(blr_begin);
		STUFF(blr_if);
		GEN_expr(request, node->nod_arg[e_while_cond]);
		GEN_statement(request, node->nod_arg[e_while_action]);
		STUFF(blr_leave);
		STUFF((int)(SLONG) node->nod_arg[e_while_number]);
		STUFF(blr_end);
		return;

	case nod_sqlcode:
	case nod_gdscode:
		STUFF(blr_abort);
		gen_error_condition(request, node);
		return;

	default:
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 901,
				  gds_arg_gds, gds_dsql_internal_err, gds_arg_gds, gds_node_err,	/* gen.c: node not supported */
				  0);
	}
}


/**
  
 	gen_aggregate
  
    @brief	Generate blr for a relation reference.
 

    @param 
    @param 

 **/
static void gen_aggregate( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_NOD list, *ptr, *end;
	DSQL_CTX context;

	context = (DSQL_CTX) node->nod_arg[e_agg_context];
	STUFF(blr_aggregate);
	STUFF(context->ctx_context);
	gen_rse(request, node->nod_arg[e_agg_rse]);

/* Handle GROUP BY clause */

	STUFF(blr_group_by);

	if ((list = node->nod_arg[e_agg_group]) != NULL) {
		STUFF(list->nod_count);
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end;
			 ptr++)
			GEN_expr(request, *ptr);
	}
	else
		STUFF(0);

/* Generate value map */

	gen_map(request, context->ctx_map);
}


/**
  
 gen_cast
  
    @brief      Generate BLR for a data-type cast operation
 

    @param request
    @param node

 **/
static void gen_cast( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_FLD field;

	STUFF(blr_cast);
	field = (DSQL_FLD) node->nod_arg[e_cast_target];
	DDL_put_field_dtype(request, field, TRUE);
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
		blr for expression n-1
		expression n
	blr for expression n-1

    @param request
    @param node

 **/
static void gen_coalesce( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_NOD list, *ptr, *end;

	// blr_value_if is used for building the coalesce function
	list = node->nod_arg[0];
	STUFF(blr_cast);
	gen_descriptor(request, &node->nod_desc, TRUE);
	for (ptr = list->nod_arg, end = ptr + (list->nod_count - 1); ptr < end; ptr++)
	{
		// IF (expression IS NULL) THEN
		STUFF(blr_value_if);
		STUFF(blr_missing);
		GEN_expr(request, *ptr);
	}
	// Return values
	list = node->nod_arg[1];
	end = list->nod_arg;
	ptr = end + list->nod_count;
	// if all expressions are NULL return NULL
	for (ptr--; ptr >= end; ptr--)
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
static void gen_constant( DSQL_REQ request, DSC * desc, BOOLEAN negate_value)
{
	UCHAR *p;
	USHORT l;
	SLONG value;
	SINT64 i64value;

	DSC tmp_desc;

	STUFF(blr_literal);

	if ((desc->dsc_dtype == dtype_double)
		&& (request->req_dbb->dbb_flags & DBB_v3))
		/* v3 doesn't understand blr_double literal, generate blr_text instead */
	{
		tmp_desc = *desc;
		tmp_desc.dsc_dtype = dtype_text;
		tmp_desc.dsc_length = desc->dsc_scale;	/* length of string literal */
		tmp_desc.dsc_scale = 0;
		desc = &tmp_desc;
	}

	l = desc->dsc_length;
	p = desc->dsc_address;

	switch (desc->dsc_dtype) {
	case dtype_short:
		gen_descriptor(request, desc, TRUE);
		value = *(SSHORT *) p;
		if (negate_value)
			value = -value;
		STUFF_WORD(value);
		break;

	case dtype_long:
		gen_descriptor(request, desc, TRUE);
		value = *(SLONG *) p;
		if (negate_value)
			value = -value;
		STUFF_WORD(value);
		STUFF_WORD(value >> 16);
		break;

	case dtype_sql_time:
	case dtype_sql_date:
		gen_descriptor(request, desc, TRUE);
		value = *(SLONG *) p;
		STUFF_WORD(value);
		STUFF_WORD(value >> 16);
		break;

	case dtype_double:
		/* this is used for approximate/large numeric literal
		   which is transmitted to the engine as a string.
		 */
		gen_descriptor(request, desc, TRUE);
		l = (USHORT) desc->dsc_scale;	/* length of string literal */
		if (negate_value) {
			STUFF_WORD(l + 1);
			STUFF('-');
		}
		else {
			STUFF_WORD(l);
		}

		if (l)
			do
				STUFF(*p++);
			while (--l);
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
					  gds_arg_number, (SLONG) - 104,
					  gds_arg_gds, isc_arith_except, 0);
		}

		/* We and the lexer both agree that this is an SINT64 constant,
		   * and if the value needed to be negated, it already has been.
		   * If the value will fit into a 32-bit signed integer, generate
		   * it that way, else as an INT64.
		 */

		if ((i64value >= (SINT64) MIN_SLONG) &&
			(i64value <= (SINT64) MAX_SLONG)) {
			STUFF(blr_long);
			STUFF(desc->dsc_scale);
			STUFF_WORD(i64value);
			STUFF_WORD(i64value >> 16);
			break;
		}
		else {
			STUFF(blr_int64);
			STUFF(desc->dsc_scale);
			STUFF_WORD(i64value);
			STUFF_WORD(i64value >> 16);
			STUFF_WORD(i64value >> 32);
			STUFF_WORD(i64value >> 48);
		}
		break;

	case dtype_quad:
	case dtype_blob:
	case dtype_array:
	case dtype_timestamp:
		gen_descriptor(request, desc, TRUE);
		value = *(SLONG *) p;
		STUFF_WORD(value);
		STUFF_WORD(value >> 16);
		value = *(SLONG *) (p + 4);
		STUFF_WORD(value);
		STUFF_WORD(value >> 16);
		break;

	case dtype_text:
		gen_descriptor(request, desc, TRUE);
		if (l)
			do
				STUFF(*p++);
			while (--l);
		break;

	default:
		/* gen_constant: datatype not understood */
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 103,
				  gds_arg_gds, gds_dsql_constant_err, 0);
	}
}


/**
  
 	gen_descriptor
  
    @brief	Generate a blr descriptor from an internal descriptor.
 

    @param request
    @param desc
    @param texttype

 **/
static void gen_descriptor( DSQL_REQ request, DSC * desc, USHORT texttype)
{

	switch (desc->dsc_dtype) {
	case dtype_text:
		if (request->req_dbb->dbb_flags & DBB_v3)
			STUFF(blr_text);
		else if (texttype || desc->dsc_ttype == ttype_binary) {
			STUFF(blr_text2);
			STUFF_WORD(desc->dsc_ttype);
		}
		else {
			STUFF(blr_text2);	/* automatic transliteration */
			STUFF_WORD(ttype_dynamic);
		}

		STUFF_WORD(desc->dsc_length);
		break;

	case dtype_varying:
		if (request->req_dbb->dbb_flags & DBB_v3)
			STUFF(blr_varying);
		else if (texttype || desc->dsc_ttype == ttype_binary) {
			STUFF(blr_varying2);
			STUFF_WORD(desc->dsc_ttype);
		}
		else {
			STUFF(blr_varying2);	/* automatic transliteration */
			STUFF_WORD(ttype_dynamic);
		}
		STUFF_WORD(desc->dsc_length - sizeof(USHORT));
		break;

	case dtype_short:
		STUFF(blr_short);
		STUFF(desc->dsc_scale);
		break;

	case dtype_long:
		STUFF(blr_long);
		STUFF(desc->dsc_scale);
		break;

	case dtype_quad:
		STUFF(blr_quad);
		STUFF(desc->dsc_scale);
		break;

	case dtype_int64:
		STUFF(blr_int64);
		STUFF(desc->dsc_scale);
		break;

	case dtype_real:
		STUFF(blr_float);
		break;

	case dtype_double:
		STUFF(blr_double);
		break;

	case dtype_sql_date:
		STUFF(blr_sql_date);
		break;

	case dtype_sql_time:
		STUFF(blr_sql_time);
		break;

	case dtype_timestamp:
		STUFF(blr_timestamp);
		break;

	case dtype_blob:
	case dtype_array:
		STUFF(blr_quad);
		STUFF(0);
		break;

	default:
		/* don't understand dtype */
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				  gds_arg_gds, gds_dsql_datatype_err, 0);
	}
}


/**
  
 	gen_error_condition
  
    @brief	Generate blr for an error condtion
 

    @param request
    @param node

 **/
static void gen_error_condition( DSQL_REQ request, DSQL_NOD node)
{
	STR string;

	switch (node->nod_type) {
	case nod_sqlcode:
		STUFF(blr_sql_code);
		STUFF_WORD((USHORT)(ULONG) node->nod_arg[0]);
		return;

	case nod_gdscode:
		STUFF(blr_gds_code);
		string = (STR) node->nod_arg[0];
		STUFF_CSTRING(string->str_data);
		return;

	case nod_exception:
		STUFF(blr_exception);
		string = (STR) node->nod_arg[0];
		STUFF_CSTRING(string->str_data);
		return;

	case nod_default:
		STUFF(blr_default_code);
		return;

	default:
		assert(FALSE);
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
static void gen_field( DSQL_REQ request, DSQL_CTX context, DSQL_FLD field, DSQL_NOD indices)
{
	DSQL_NOD *ptr, *end;

/* For older clients - generate an error should they try and
 *    access data types which did not exist in the older dialect */
	if (request->req_client_dialect <= SQL_DIALECT_V5) {
		switch (field->fld_dtype) {
		case dtype_sql_date:
		case dtype_sql_time:
		case dtype_int64:
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
					  gds_arg_gds, gds_dsql_datatype_err,
					  gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					  gds_arg_number, request->req_client_dialect,
					  gds_arg_string,
					  DSC_dtype_tostring(static_cast < UCHAR >
										 (field->fld_dtype)), 0);
			break;
		default:
			/* No special action for other data types */
			break;
		}
	}



	if (indices)
		STUFF(blr_index);

	if (DDL_ids(request)) {
		STUFF(blr_fid);
		STUFF(context->ctx_context);
		STUFF_WORD(field->fld_id);
	}
	else {
		STUFF(blr_field);
		STUFF(context->ctx_context);
		STUFF_CSTRING(field->fld_name);
	}

	if (indices) {
		STUFF(indices->nod_count);
		for (ptr = indices->nod_arg, end = ptr + indices->nod_count;
			 ptr < end; ptr++)
			GEN_expr(request, *ptr);
	}
}


/**
  
 	gen_for_select
  
    @brief	Generate BLR for a SELECT statement.
 

    @param request
    @param for_select

 **/
static void gen_for_select( DSQL_REQ request, DSQL_NOD for_select)
{
	DSQL_NOD list, list_to, *ptr, *ptr_to, *end, rse;

	rse = for_select->nod_arg[e_flp_select];

    /* CVC: Only put a label if this is not singular; otherwise,
       what loop is the user trying to abandon? */
    if (for_select->nod_arg [e_flp_action]) {
        STUFF (blr_label);
        STUFF ((int) for_select->nod_arg [e_flp_number]);
    }

/* Generate FOR loop */

	STUFF(blr_for);

	if (!for_select->nod_arg[e_flp_action] &&
		!(request->req_dbb->dbb_flags & DBB_v3)) STUFF(blr_singular);
	gen_rse(request, rse);
	STUFF(blr_begin);

/* Build body of FOR loop */

	/* Handle write locks */
	DSQL_NOD streams = rse->nod_arg[e_rse_streams];
	DSQL_CTX context = NULL;

	if (!rse->nod_arg[e_rse_reduced] && streams->nod_count==1) {
		DSQL_NOD item = streams->nod_arg[0];
		if (item && (item->nod_type == nod_relation))
			context = (DSQL_CTX) item->nod_arg[e_rel_context];
	}
	
	list = rse->nod_arg[e_rse_items];
	list_to = for_select->nod_arg[e_flp_into];
	if (list->nod_count != list_to->nod_count)
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 313,
				  gds_arg_gds, gds_dsql_count_mismatch, 0);
	for (ptr = list->nod_arg, ptr_to = list_to->nod_arg,
		 end = ptr + list->nod_count; ptr < end; ptr++, ptr_to++) {
		STUFF(blr_assignment);
		GEN_expr(request, *ptr);
		GEN_expr(request, *ptr_to);
	}
	if (for_select->nod_arg[e_flp_action])
		GEN_statement(request, for_select->nod_arg[e_flp_action]);
	STUFF(blr_end);
}


/**
  
 gen_gen_id
  
    @brief      Generate BLR for gen_id
 

    @param request
    @param node

 **/
static void gen_gen_id( DSQL_REQ request, DSQL_NOD node)
{
	STR string;

	STUFF(blr_gen_id);
	string = (STR) node->nod_arg[e_gen_id_name];
	STUFF_CSTRING(string->str_data);
	GEN_expr(request, node->nod_arg[e_gen_id_value]);
}


/**
  
 	gen_join_rse
  
    @brief	Generate a record selection expression
 	with an explicit join type.
 

    @param request
    @param rse

 **/
static void gen_join_rse( DSQL_REQ request, DSQL_NOD rse)
{
	DSQL_NOD node;

	STUFF(blr_rs_stream);
	STUFF(2);

	GEN_expr(request, rse->nod_arg[e_join_left_rel]);
	GEN_expr(request, rse->nod_arg[e_join_rght_rel]);

	node = rse->nod_arg[e_join_type];
	if (node->nod_type != nod_join_inner) {
		STUFF(blr_join_type);
		if (node->nod_type == nod_join_left)
			STUFF(blr_left);
		else if (node->nod_type == nod_join_right)
			STUFF(blr_right);
		else
			STUFF(blr_full);
	}

	STUFF(blr_boolean);
	GEN_expr(request, rse->nod_arg[e_join_boolean]);

	STUFF(blr_end);
}


/**
  
 	gen_map
  
    @brief	Generate a value map for a record selection expression.
 

    @param request
    @param map

 **/
static void gen_map( DSQL_REQ request, MAP map)
{
	USHORT count;
	MAP temp;

	count = 0;
	for (temp = map; temp; temp = temp->map_next)
		temp->map_position = count++;

	STUFF(blr_map);
	STUFF_WORD(count);

	for (temp = map; temp; temp = temp->map_next) {
		STUFF_WORD(temp->map_position);
		GEN_expr(request, temp->map_node);
	}
}


/**
  
 	gen_parameter
  
    @brief	Generate a parameter reference.
 

    @param request
    @param parameter

 **/
static void gen_parameter( DSQL_REQ request, PAR parameter)
{
	DSQL_MSG message;
	PAR null;

	message = parameter->par_message;

	if ((null = parameter->par_null) != NULL) {
		STUFF(blr_parameter2);
		STUFF(message->msg_number);
		STUFF_WORD(parameter->par_parameter);
		STUFF_WORD(null->par_parameter);
		return;
	}

	STUFF(blr_parameter);
	STUFF(message->msg_number);
	STUFF_WORD(parameter->par_parameter);
}



/**
  
 	gen_plan
  
    @brief	Generate blr for an access plan expression.
 

    @param request
    @param plan_expression

 **/
static void gen_plan( DSQL_REQ request, DSQL_NOD plan_expression)
{
	DSQL_NOD list, node, arg, *ptr, *end, *ptr2, *end2;
	STR index_string;

/* stuff the join type */

	list = plan_expression->nod_arg[1];
	if (list->nod_count > 1) {
		if (node = plan_expression->nod_arg[0])
			STUFF(blr_merge);
		else
			STUFF(blr_join);
		STUFF(list->nod_count);
	}

/* stuff one or more plan items */

	for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end; ptr++) {
		node = *ptr;
		if (node->nod_type == nod_plan_expr) {
			gen_plan(request, node);
			continue;
		}

		/* if we're here, it must be a nod_plan_item */

		STUFF(blr_retrieve);

		/* stuff the relation--the relation id itself is redundant except 
		   when there is a need to differentiate the base tables of views */

		arg = node->nod_arg[0];
		gen_relation(request, (DSQL_CTX) arg->nod_arg[e_rel_context]);

		/* now stuff the access method for this stream */

		arg = node->nod_arg[1];
		switch (arg->nod_type) {
		case nod_natural:
			STUFF(blr_sequential);
			break;

		case nod_index_order:
			STUFF(blr_navigational);
			index_string = (STR) arg->nod_arg[0];
			STUFF_CSTRING(index_string->str_data);
			break;

		case nod_index:
			STUFF(blr_indices);
			arg = arg->nod_arg[0];
			STUFF(arg->nod_count);
			for (ptr2 = arg->nod_arg, end2 = ptr2 + arg->nod_count;
				 ptr2 < end2; ptr2++) {
				index_string = (STR) * ptr2;
				STUFF_CSTRING(index_string->str_data);
			}
			break;

		default:
			assert(FALSE);
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
static void gen_relation( DSQL_REQ request, DSQL_CTX context)
{
	DSQL_REL relation;
	DSQL_PRC procedure;
	DSQL_NOD inputs, *ptr, *end;

	relation = context->ctx_relation;
	procedure = context->ctx_procedure;

/* if this is a trigger or procedure , don't want relation id used */
	if (relation) {
		if (DDL_ids(request)) {
			if (context->ctx_alias)
				STUFF(blr_rid2);
			else
				STUFF(blr_rid);
			STUFF_WORD(relation->rel_id);
		}
		else {
			if (context->ctx_alias)
				STUFF(blr_relation2);
			else
				STUFF(blr_relation);
			STUFF_CSTRING(relation->rel_name);
		}

		if (context->ctx_alias)
			STUFF_CSTRING(context->ctx_alias);
		STUFF(context->ctx_context);
	}
	else {
		if (DDL_ids(request)) {
			STUFF(blr_pid);
			STUFF_WORD(procedure->prc_id);
		}
		else {
			STUFF(blr_procedure);
			STUFF_CSTRING(procedure->prc_name);
		}
		STUFF(context->ctx_context);
		STUFF_WORD(procedure->prc_in_count);
		if (inputs = context->ctx_proc_inputs)
			for (ptr = inputs->nod_arg, end = ptr + inputs->nod_count;
				 ptr < end; ptr++)
				GEN_expr(request, *ptr);
	}
}


/**
  
 	gen_return
  
    @brief	Generate blr for a procedure return.
 

    @param request
    @param procedure
    @param eos_flag

 **/
void GEN_return( DSQL_REQ request, DSQL_NOD procedure, BOOLEAN eos_flag)
{
	DSQL_NOD parameters, parameter, *ptr, *end;
	VAR variable;
	USHORT outputs;

	if (!procedure)
		return;

	if (!eos_flag)
		STUFF(blr_begin);
	STUFF(blr_send);
	STUFF(1);
	STUFF(blr_begin);
	outputs = 0;
	if (parameters = procedure->nod_arg[e_prc_outputs]) {
		for (ptr = parameters->nod_arg, end = ptr + parameters->nod_count;
			 ptr < end; ptr++) {
			outputs++;
			parameter = *ptr;
			variable = (VAR) parameter->nod_arg[e_var_variable];
			STUFF(blr_assignment);
			STUFF(blr_variable);
			STUFF_WORD(variable->var_variable_number);
			STUFF(blr_parameter2);
			STUFF(variable->var_msg_number);
			STUFF_WORD(variable->var_msg_item);
			STUFF_WORD(variable->var_msg_item + 1);
		}
	}
	STUFF(blr_assignment);
	STUFF(blr_literal);
	STUFF(blr_short);
	STUFF(0);
	if (eos_flag)
		STUFF_WORD(0);
	else
		STUFF_WORD(1);
	STUFF(blr_parameter);
	STUFF(1);
	STUFF_WORD(2 * outputs);
	STUFF(blr_end);
	if (!eos_flag) {
		STUFF(blr_stall);
		STUFF(blr_end);
	}
}


/**
  
 	gen_rse
  
    @brief	Generate a record selection expression.
 

    @param request
    @param rse

 **/
static void gen_rse( DSQL_REQ request, DSQL_NOD rse)
{
	DSQL_NOD node, list, *ptr, *end;

	if (rse->nod_arg[e_rse_singleton]
		&& !(request->req_dbb->dbb_flags & DBB_v3)) STUFF(blr_singular);

	STUFF(blr_rse);

	list = rse->nod_arg[e_rse_streams];

/* Handle source streams */

	if (list->nod_type == nod_union) {
		STUFF(1);
		gen_union(request, rse);
	}
	else if (list->nod_type == nod_list) {
		STUFF(list->nod_count);
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end;
			 ptr++) {
			node = *ptr;
			if (node->nod_type == nod_relation ||
				node->nod_type == nod_aggregate || node->nod_type == nod_join)
				GEN_expr(request, node);
		}
	}
	else {
		STUFF(1);
		GEN_expr(request, list);
	}

	if (rse->nod_arg[e_rse_lock])
		STUFF(blr_writelock);
	
	if ((node = rse->nod_arg[e_rse_first]) != NULL) {
		STUFF(blr_first);
		GEN_expr(request, node);
	}

    if ((node = rse->nod_arg [e_rse_skip]) != NULL) {
        STUFF (blr_skip);
        GEN_expr (request, node);
    }

	if ((node = rse->nod_arg[e_rse_boolean]) != NULL) {
		STUFF(blr_boolean);
		GEN_expr(request, node);
	}

	if ((list = rse->nod_arg[e_rse_sort]) != NULL)
		gen_sort(request, list);

	if ((list = rse->nod_arg[e_rse_reduced]) != NULL) {
		STUFF(blr_project);
		STUFF(list->nod_count);
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end;
			 ptr++)
			GEN_expr(request, *ptr);
	}

/* if the user specified an access plan to use, add it here */

	if ((node = rse->nod_arg[e_rse_plan]) != NULL) {
		STUFF(blr_plan);
		gen_plan(request, node);
	}

#ifdef SCROLLABLE_CURSORS
/* generate a statement to be executed if the user scrolls 
   in a direction other than forward; a message is sent outside 
   the normal send/receive protocol to specify the direction 
   and offset to scroll; note that we do this only on a SELECT 
   type statement and only when talking to a 4.1 engine or greater */

	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5) {
		STUFF(blr_receive);
		STUFF(request->req_async->msg_number);
		STUFF(blr_seek);
		PAR parameter = request->req_async->msg_parameters;
		gen_parameter(request, parameter->par_next);
		gen_parameter(request, parameter);
	}
#endif

	STUFF(blr_end);
}


/**
  
 gen_searched_case
  
    @brief      Generate BLR for CASE function (searched)
 

    @param request
    @param node

 **/
static void gen_searched_case( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_NOD boolean_list, results_list, *bptr, *rptr, *end;

	/* blr_value_if is used for building the case expression */

	STUFF(blr_cast);
	gen_descriptor(request, &node->nod_desc, TRUE);
	SSHORT count = node->nod_arg[e_searched_case_search_conditions]->nod_count;
	boolean_list = node->nod_arg[e_searched_case_search_conditions];
	results_list = node->nod_arg[e_searched_case_results];
	for (bptr = boolean_list->nod_arg, end = bptr + count, rptr = results_list->nod_arg; 
			bptr < end; bptr++, rptr++)
	{
		STUFF(blr_value_if);
		GEN_expr(request, *bptr);
		GEN_expr(request, *rptr);
	}
	/* else_result */
	GEN_expr(request, node->nod_arg[e_searched_case_results]->nod_arg[count]);
}


/**
  
 	gen_select
  
    @brief	Generate BLR for a SELECT statement.
 

    @param request
    @param rse

 **/
static void gen_select( DSQL_REQ request, DSQL_NOD rse)
{
	DSQL_NOD list, *ptr, *end, item, alias, map_node;
	PAR parameter;
	DSQL_MSG message;
	DSQL_FLD field;
	DSC constant_desc;
	DSQL_REL relation;
	UDF udf;
	DSQL_CTX context;
	STR string;
	SSHORT constant;
	MAP map;

	constant_desc.dsc_dtype = dtype_short;
	constant_desc.dsc_scale = 0;
	constant_desc.dsc_sub_type = 0;
	constant_desc.dsc_flags = 0;
	constant_desc.dsc_length = sizeof(SSHORT);
	constant_desc.dsc_address = (UCHAR *) & constant;

/* Set up parameter for things in the select list */

	list = rse->nod_arg[e_rse_items];
	for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end; ptr++) {
		item = *ptr;
		parameter = MAKE_parameter(request->req_receive, TRUE, TRUE, 0);
		parameter->par_node = item;
		MAKE_desc(&parameter->par_desc, item);
		if (item->nod_type == nod_field) {
			field = (DSQL_FLD) item->nod_arg[e_fld_field];
			parameter->par_name = parameter->par_alias = field->fld_name;
			context = (DSQL_CTX) item->nod_arg[e_fld_context];
			if (context->ctx_relation) {
				parameter->par_rel_name = context->ctx_relation->rel_name;
				parameter->par_owner_name = context->ctx_relation->rel_owner;
			}
			else if (context->ctx_procedure) {
				parameter->par_rel_name = context->ctx_procedure->prc_name;
				parameter->par_owner_name = context->ctx_procedure->prc_owner;
			}
		}
		else if (item->nod_type == nod_dbkey) {
			parameter->par_alias = const_cast < char *>(db_key_name);
			parameter->par_name = const_cast < char *>(db_key_name);
			context = (DSQL_CTX) item->nod_arg[0]->nod_arg[0];
			parameter->par_rel_name = context->ctx_relation->rel_name;
			parameter->par_owner_name = context->ctx_relation->rel_owner;
		}
		else if (item->nod_type == nod_alias) {
			string = (STR) item->nod_arg[e_alias_alias];
			parameter->par_alias = (TEXT *) string->str_data;
			alias = item->nod_arg[e_alias_value];
			if (alias->nod_type == nod_field) {
				field = (DSQL_FLD) alias->nod_arg[e_fld_field];
				parameter->par_name = field->fld_name;
				context = (DSQL_CTX) alias->nod_arg[e_fld_context];
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
				parameter->par_name = const_cast < char *>(db_key_name);
				context = (DSQL_CTX) alias->nod_arg[0]->nod_arg[0];
				parameter->par_rel_name = context->ctx_relation->rel_name;
				parameter->par_owner_name = context->ctx_relation->rel_owner;
			}
		}
		else if (item->nod_type == nod_map) {
			map = (MAP) item->nod_arg[e_map_map];
			map_node = map->map_node;
			while (map_node->nod_type == nod_map) {
				/* skip all the nod_map nodes */
				map = (MAP) map_node->nod_arg[e_map_map];
				map_node = map->map_node;
			}
			if (map_node->nod_type == nod_field) {
				field = (DSQL_FLD) map_node->nod_arg[e_fld_field];
				parameter->par_name = parameter->par_alias = field->fld_name;
			}
			else if (map_node->nod_type == nod_alias) {
				string = (STR) map_node->nod_arg[e_alias_alias];
				parameter->par_alias = (TEXT *) string->str_data;
				alias = map_node->nod_arg[e_alias_value];
				if (alias->nod_type == nod_field) {
					field = (DSQL_FLD) alias->nod_arg[e_fld_field];
					parameter->par_name = field->fld_name;
				}
			}
			else if (map_node->nod_type == nod_agg_count)
				parameter->par_name = parameter->par_alias = "COUNT";
			else if (map_node->nod_type == nod_agg_total)
				parameter->par_name = parameter->par_alias = "SUM";
			else if (map_node->nod_type == nod_agg_average)
				parameter->par_name = parameter->par_alias = "AVG";
			else if (map_node->nod_type == nod_agg_total2)
				parameter->par_name = parameter->par_alias = "SUM";
			else if (map_node->nod_type == nod_agg_average2)
				parameter->par_name = parameter->par_alias = "AVG";
			else if (map_node->nod_type == nod_agg_min)
				parameter->par_name = parameter->par_alias = "MIN";
			else if (map_node->nod_type == nod_agg_max)
				parameter->par_name = parameter->par_alias = "MAX";
		}
		else if (item->nod_type == nod_udf) {
			udf = (UDF) item->nod_arg[0];
			parameter->par_name = parameter->par_alias = udf->udf_name;
		}
		else if (item->nod_type == nod_gen_id)
			parameter->par_name = parameter->par_alias = "GEN_ID";
		else if (item->nod_type == nod_gen_id2)
			parameter->par_name = parameter->par_alias = "GEN_ID";
        else if (item->nod_type == nod_user_name)
            parameter->par_name = parameter->par_alias  = "USER";
        else if (item->nod_type == nod_current_role)
            parameter->par_name = parameter->par_alias  = "ROLE";
        else if (item->nod_type == nod_internal_info)
		{
			internal_info_id id =
				*reinterpret_cast<internal_info_id*>(item->nod_arg[0]->nod_desc.dsc_address);
			parameter->par_name = parameter->par_alias = InternalInfo::getAlias(id);
		}
        else if (item->nod_type == nod_substr) {
            /* CVC: SQL starts at 1 but C starts at zero. */
            /*DSQL_NOD node = item->nod_arg [e_substr_start];
              --(*(SLONG *) (node->nod_desc.dsc_address));
              FIXED IN PARSE.Y; here it doesn't catch expressions: Bug 450301. */
            parameter->par_name = parameter->par_alias  = "SUBSTRING";
        }
        else if (item->nod_type == nod_concatenate)
            parameter->par_name = parameter->par_alias	= "CONCATENATION";
        else if (item->nod_type == nod_cast)
            parameter->par_name = parameter->par_alias	= "CAST";
        else if (item->nod_type == nod_upcase)
            parameter->par_name = parameter->par_alias	= "UPPER";
        else if (item->nod_type == nod_current_date)
            parameter->par_name = parameter->par_alias	= "CURRENT_DATE";
        else if (item->nod_type == nod_current_time)
            parameter->par_name = parameter->par_alias	= "CURRENT_TIME";
        else if (item->nod_type == nod_current_timestamp)
            parameter->par_name = parameter->par_alias	= "CURRENT_TIMESTAMP";
        else if (item->nod_type == nod_extract)
            parameter->par_name = parameter->par_alias	= "EXTRACT";
        else if (item->nod_type == nod_searched_case)
            parameter->par_name = parameter->par_alias	= "CASE";
        else if (item->nod_type == nod_simple_case)
            parameter->par_name = parameter->par_alias	= "CASE";
        else if (item->nod_type == nod_coalesce)
            parameter->par_name = parameter->par_alias	= "COALESCE";
	}

/* Set up parameter to handle EOF */

	request->req_eof = parameter =
		MAKE_parameter(request->req_receive, FALSE, FALSE, 0);
	parameter->par_desc.dsc_dtype = dtype_short;
	parameter->par_desc.dsc_scale = 0;
	parameter->par_desc.dsc_length = sizeof(SSHORT);

/* Save DBKEYs for possible update later */

	list = rse->nod_arg[e_rse_streams];

	if (!rse->nod_arg[e_rse_reduced]) {
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end;
			 ptr++)
			if ((item = *ptr) && (item->nod_type == nod_relation)) {
				context = (DSQL_CTX) item->nod_arg[e_rel_context];
				if (relation = context->ctx_relation) {
					/* Set up dbkey */
					parameter =
						MAKE_parameter(request->req_receive, FALSE, FALSE, 0);
					parameter->par_dbkey_ctx = context;
					parameter->par_desc.dsc_dtype = dtype_text;
					parameter->par_desc.dsc_ttype = ttype_binary;
					parameter->par_desc.dsc_length =
						relation->rel_dbkey_length;

					/* Set up record version - for post v33 databases */

					if (!(request->req_dbb->dbb_flags & DBB_v3)) {
						parameter =
							MAKE_parameter(request->req_receive, FALSE,
										   FALSE, 0);
						parameter->par_rec_version_ctx = context;
						parameter->par_desc.dsc_dtype = dtype_text;
						parameter->par_desc.dsc_ttype = ttype_binary;
						parameter->par_desc.dsc_length =
							relation->rel_dbkey_length / 2;
					}
				}
			}
	}

#ifdef SCROLLABLE_CURSORS
/* define the parameters for the scrolling message--offset and direction, 
   in that order to make it easier to generate the request */

	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5) {
		parameter = MAKE_parameter(request->req_async, FALSE, FALSE, 0);
		parameter->par_desc.dsc_dtype = dtype_short;
		parameter->par_desc.dsc_length = sizeof(USHORT);
		parameter->par_desc.dsc_scale = 0;
		parameter->par_desc.dsc_flags = 0;
		parameter->par_desc.dsc_sub_type = 0;

		parameter = MAKE_parameter(request->req_async, FALSE, FALSE, 0);
		parameter->par_desc.dsc_dtype = dtype_long;
		parameter->par_desc.dsc_length = sizeof(ULONG);
		parameter->par_desc.dsc_scale = 0;
		parameter->par_desc.dsc_flags = 0;
		parameter->par_desc.dsc_sub_type = 0;
	}
#endif

/* Generate definitions for the messages */

	GEN_port(request, request->req_receive);
	message = request->req_send;
	if (message->msg_parameter)
		GEN_port(request, message);
	else
		request->req_send = NULL;
#ifdef SCROLLABLE_CURSORS
	if (request->req_type == REQ_SELECT &&
		request->req_dbb->dbb_base_level >= 5)
			GEN_port(request, request->req_async);
#endif

/* If there is a send message, build a RECEIVE */

	if ((message = request->req_send) != NULL) {
		STUFF(blr_receive);
		STUFF(message->msg_number);
	}

/* Generate FOR loop */

	message = request->req_receive;

	STUFF(blr_for);
	if (!(request->req_dbb->dbb_flags & DBB_v3))
		STUFF(blr_stall);
	gen_rse(request, rse);	
	
	STUFF(blr_send);
	STUFF(message->msg_number);
	STUFF(blr_begin);

/* Build body of FOR loop */

	/* Add invalid usage here */
	
	STUFF(blr_assignment);
	constant = 1;
	gen_constant(request, &constant_desc, USE_VALUE);
	gen_parameter(request, request->req_eof);

	for (parameter = message->msg_parameters; parameter;
		 parameter = parameter->par_next) {
		if (parameter->par_node) {
			STUFF(blr_assignment);
			GEN_expr(request, parameter->par_node);
			gen_parameter(request, parameter);
		}
		if (context = parameter->par_dbkey_ctx) {
			STUFF(blr_assignment);
			STUFF(blr_dbkey);
			STUFF(context->ctx_context);
			gen_parameter(request, parameter);
		}
		if (context = parameter->par_rec_version_ctx) {
			STUFF(blr_assignment);
			STUFF(blr_record_version);
			STUFF(context->ctx_context);
			gen_parameter(request, parameter);
		}
	}

	STUFF(blr_end);
	STUFF(blr_send);
	STUFF(message->msg_number);
	STUFF(blr_assignment);
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
static void gen_simple_case( DSQL_REQ request, DSQL_NOD node)
{
	DSQL_NOD when_list, results_list, *wptr, *rptr, *end;

	/* blr_value_if is used for building the case expression */

	STUFF(blr_cast);
	gen_descriptor(request, &node->nod_desc, TRUE);
	SSHORT count = node->nod_arg[e_simple_case_when_operands]->nod_count;
	when_list = node->nod_arg[e_simple_case_when_operands];
	results_list = node->nod_arg[e_simple_case_results];
	for (wptr = when_list->nod_arg, end = wptr + count, rptr = results_list->nod_arg; 
			wptr < end; wptr++, rptr++)
	{
		STUFF(blr_value_if);
		STUFF(blr_eql);
		GEN_expr(request, node->nod_arg[e_simple_case_case_operand]);
		GEN_expr(request, *wptr);
		GEN_expr(request, *rptr);
	}
	/* else_result */
	GEN_expr(request, node->nod_arg[e_simple_case_results]->nod_arg[count]); 
}


/**
  
 	gen_sort
  
    @brief	Generate a sort clause.
 

    @param request
    @param list

 **/
static void gen_sort( DSQL_REQ request, DSQL_NOD list)
{
	DSQL_NOD *ptr, *end;

	STUFF(blr_sort);
	STUFF(list->nod_count);

	for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end; ptr++) {
 		dsql_nod* nulls_placement = (*ptr)->nod_arg[e_order_nulls];
 		if (nulls_placement) {
 			switch ((SLONG)nulls_placement->nod_arg[0]) {
 				case NOD_NULLS_FIRST:
 					STUFF(blr_nullsfirst);
 					break;
 				case NOD_NULLS_LAST:
 					STUFF(blr_nullslast);
 					break;
 			}
 		}
		if ((*ptr)->nod_arg[e_order_flag])
			STUFF(blr_descending);
		else
			STUFF(blr_ascending);
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
static void gen_table_lock( DSQL_REQ request, DSQL_NOD tbl_lock, USHORT lock_level)
{
	DSQL_NOD tbl_names, *ptr, *end;
	STR temp;
	SSHORT flags;
	USHORT lock_mode;

	if ((!tbl_lock) || (tbl_lock->nod_type != nod_table_lock))
		return;

	tbl_names = tbl_lock->nod_arg[e_lock_tables];
	flags = 0;

	if (tbl_lock->nod_arg[e_lock_mode])
		flags = tbl_lock->nod_arg[e_lock_mode]->nod_flags;

	if (flags & NOD_PROTECTED)
		lock_level = gds_tpb_protected;
	else if (flags & NOD_SHARED)
		lock_level = gds_tpb_shared;

	lock_mode = (flags & NOD_WRITE) ? gds_tpb_lock_write : gds_tpb_lock_read;

	for (ptr = tbl_names->nod_arg, end = ptr + tbl_names->nod_count;
		 ptr < end; ptr++) {
		if ((*ptr)->nod_type != nod_relation_name)
			continue;

		STUFF(lock_mode);

		/* stuff table name */
		temp = (STR) ((*ptr)->nod_arg[e_rln_name]);
		stuff_cstring(request, reinterpret_cast < char *>(temp->str_data));

		STUFF(lock_level);
	}
}


/**
  
 	gen_udf
  
    @brief	Generate a user defined function.
 

    @param request
    @param node

 **/
static void gen_udf( DSQL_REQ request, DSQL_NOD node)
{
	UDF udf;
	DSQL_NOD list, *ptr, *end;

	udf = (UDF) node->nod_arg[0];
	STUFF(blr_function);
	STUFF_CSTRING(udf->udf_name);

	if ((node->nod_count == 2) && (list = node->nod_arg[1])) {
		STUFF(list->nod_count);
		for (ptr = list->nod_arg, end = ptr + list->nod_count; ptr < end;
			 ptr++)
			GEN_expr(request, *ptr);
	}
	else
		STUFF(0);
}


/**
  
 	gen_union
  
    @brief	Generate a union of substreams.
 

    @param request
    @param union_node

 **/
static void gen_union( DSQL_REQ request, DSQL_NOD union_node)
{
	DSQL_NOD sub_rse, *ptr, *end, streams;
	DSQL_NOD items, *iptr, *iend;
	USHORT count;
	DSQL_CTX union_context;

	STUFF(blr_union);

/* Obtain the context for UNION from the first MAP node   */
	items = union_node->nod_arg[e_rse_items];
	union_context = (DSQL_CTX) items->nod_arg[0]->nod_arg[e_map_context];
	STUFF(union_context->ctx_context);

	streams = union_node->nod_arg[e_rse_streams];
	STUFF(streams->nod_count);	/* number of substreams */

	for (ptr = streams->nod_arg, end = ptr + streams->nod_count; ptr < end;
		 ptr++) {
		sub_rse = *ptr;
		gen_rse(request, sub_rse);
		items = sub_rse->nod_arg[e_rse_items];
		STUFF(blr_map);
		STUFF_WORD(items->nod_count);
		count = 0;
		for (iptr = items->nod_arg, iend = iptr + items->nod_count;
			 iptr < iend; iptr++) {
			STUFF_WORD(count);
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
static void stuff_cstring( DSQL_REQ request, char *string)
{
	UCHAR c;

	STUFF(strlen(string));
	while ((c = *string++))
		STUFF(c);
}


/**
  
 	stuff_word
  
    @brief	Cram a word into the blr buffer.  If the buffer is getting
 	ready to overflow, expand it.
 

    @param request
    @param word

 **/
static void stuff_word( DSQL_REQ request, USHORT word)
{

	STUFF(word);
	STUFF(word >> 8);
}
