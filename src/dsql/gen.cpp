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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <string.h>
#include <stdio.h>
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/DdlNodes.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/StmtNodes.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/ibase.h"
#include "../jrd/align.h"
#include "../jrd/constants.h"
#include "../jrd/intl.h"
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/utld_proto.h"
#include "../jrd/thread_proto.h"
#include "../common/dsc_proto.h"
#include "../yvalve/why_proto.h"
#include "gen/iberror.h"
#include "../common/StatusArg.h"

using namespace Jrd;
using namespace Dsql;
using namespace Firebird;

static void gen_plan(DsqlCompilerScratch*, const dsql_nod*);
static void gen_union(DsqlCompilerScratch*, const dsql_nod*);


void GEN_hidden_variables(DsqlCompilerScratch* dsqlScratch)
{
/**************************************
 *
 *	G E N _ h i d d e n _ v a r i a b l e s
 *
 **************************************
 *
 * Function
 *	Emit BLR for hidden variables.
 *
 **************************************/
	if (dsqlScratch->hiddenVariables.isEmpty())
		return;

	for (Array<dsql_var*>::const_iterator i = dsqlScratch->hiddenVariables.begin();
		 i != dsqlScratch->hiddenVariables.end();
		 ++i)
	{
		const dsql_var* var = *i;
		dsqlScratch->appendUChar(blr_dcl_variable);
		dsqlScratch->appendUShort(var->number);
		GEN_descriptor(dsqlScratch, &var->desc, true);
	}

	// Clear it for GEN_expr not regenerate them.
	dsqlScratch->hiddenVariables.clear();
}


/**

 	GEN_expr

    @brief	Generate blr for an arbitrary expression.


    @param dsqlScratch
    @param node

 **/
void GEN_expr(DsqlCompilerScratch* dsqlScratch, dsql_nod* node)
{
	if (node->nod_type == nod_class_exprnode)
	{
		ExprNode* exprNode = reinterpret_cast<ExprNode*>(node->nod_arg[0]);

		if (exprNode->is<RseNode>())
		{
			GEN_rse(dsqlScratch, node);
			return;
		}

		exprNode->genBlr(dsqlScratch);

		// Check whether the node we just processed is for a dialect 3
		// operation which gives a different result than the corresponding
		// operation in dialect 1. If it is, and if the client dialect is 2,
		// issue a warning about the difference.

		// ASF: Shouldn't we check nod_gen_id2 too?

		if (exprNode->dsqlCompatDialectVerb &&
			dsqlScratch->clientDialect == SQL_DIALECT_V6_TRANSITION)
		{
			dsc desc;
			MAKE_desc(dsqlScratch, &desc, node);

			if (desc.dsc_dtype == dtype_int64)
			{
				ERRD_post_warning(
					Arg::Warning(isc_dsql_dialect_warning_expr) <<
					Arg::Str(exprNode->dsqlCompatDialectVerb));
			}
		}
	}
	else
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_internal_err) <<
				  // expression evaluation not supported
				  Arg::Gds(isc_expression_eval_err) <<
				  Arg::Gds(isc_dsql_eval_unknode) << Arg::Num(node->nod_type));
	}
}

/**

 	GEN_port

    @brief	Generate a port from a message.  Feel free to rearrange the
 	order of parameters.


    @param dsqlScratch
    @param message

 **/
void GEN_port(DsqlCompilerScratch* dsqlScratch, dsql_msg* message)
{
	thread_db* tdbb = JRD_get_thread_data();

	dsqlScratch->appendUChar(blr_message);
	dsqlScratch->appendUChar(message->msg_number);
	dsqlScratch->appendUShort(message->msg_parameter);

	ULONG offset = 0;

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		dsql_par* parameter = message->msg_parameters[i];

		parameter->par_parameter = (USHORT) i;

		const USHORT fromCharSet = parameter->par_desc.getCharSet();
		const USHORT toCharSet = (fromCharSet == CS_NONE || fromCharSet == CS_BINARY) ?
			fromCharSet : tdbb->getCharSet();

		if (parameter->par_desc.dsc_dtype <= dtype_any_text &&
			tdbb->getCharSet() != CS_NONE && tdbb->getCharSet() != CS_BINARY)
		{
			USHORT adjust = 0;
			if (parameter->par_desc.dsc_dtype == dtype_varying)
				adjust = sizeof(USHORT);
			else if (parameter->par_desc.dsc_dtype == dtype_cstring)
				adjust = 1;

			parameter->par_desc.dsc_length -= adjust;

			const USHORT fromCharSetBPC = METD_get_charset_bpc(dsqlScratch->getTransaction(), fromCharSet);
			const USHORT toCharSetBPC = METD_get_charset_bpc(dsqlScratch->getTransaction(), toCharSet);

			parameter->par_desc.setTextType(INTL_CS_COLL_TO_TTYPE(toCharSet,
				(fromCharSet == toCharSet ? INTL_GET_COLLATE(&parameter->par_desc) : 0)));

			parameter->par_desc.dsc_length = UTLD_char_length_to_byte_length(
				parameter->par_desc.dsc_length / fromCharSetBPC, toCharSetBPC, adjust);

			parameter->par_desc.dsc_length += adjust;
		}
		else if (parameter->par_desc.dsc_dtype == dtype_blob &&
			parameter->par_desc.dsc_sub_type == isc_blob_text &&
			tdbb->getCharSet() != CS_NONE && tdbb->getCharSet() != CS_BINARY)
		{
			if (fromCharSet != toCharSet)
				parameter->par_desc.setTextType(toCharSet);
		}

		// For older clients - generate an error should they try and
		// access data types which did not exist in the older dialect
		if (dsqlScratch->clientDialect <= SQL_DIALECT_V5)
		{
			switch (parameter->par_desc.dsc_dtype)
			{
				// In V6.0 - older clients, which we distinguish by
				// their use of SQL DIALECT 0 or 1, are forbidden
				// from selecting values of new datatypes
				case dtype_sql_date:
				case dtype_sql_time:
				case dtype_int64:
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
							  Arg::Gds(isc_dsql_datatype_err) <<
							  Arg::Gds(isc_sql_dialect_datatype_unsupport) <<
									Arg::Num(dsqlScratch->clientDialect) <<
									Arg::Str(DSC_dtype_tostring(parameter->par_desc.dsc_dtype)));
					break;
				default:
					// No special action for other data types
					break;
			}
		}

		if (parameter->par_desc.dsc_dtype == dtype_text && parameter->par_index != 0)
		{
			// We should convert par_desc from text to varying so the user can receive it with
			// correct length when requesting it as varying. See CORE-2606.
			// But we flag it to describe as text.
			parameter->par_is_text = true;
			parameter->par_desc.dsc_dtype = dtype_varying;
			parameter->par_desc.dsc_length += sizeof(USHORT);
		}

		const USHORT align = type_alignments[parameter->par_desc.dsc_dtype];
		if (align)
			offset = FB_ALIGN(offset, align);
		parameter->par_desc.dsc_address = (UCHAR*)(IPTR) offset;
		offset += parameter->par_desc.dsc_length;
		GEN_descriptor(dsqlScratch, &parameter->par_desc, true);
	}

	if (offset > MAX_MESSAGE_SIZE)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
				  Arg::Gds(isc_imp_exc) <<
				  Arg::Gds(isc_blktoobig));
	}

	message->msg_length = offset;

	dsqlScratch->ports.add(message);
}


// Generate complete blr for a dsqlScratch.
void GEN_request(DsqlCompilerScratch* scratch, DmlNode* node)
{
	DsqlCompiledStatement* statement = scratch->getStatement();

	if (statement->getBlrVersion() == 4)
		scratch->appendUChar(blr_version4);
	else
		scratch->appendUChar(blr_version5);

	if (statement->getType() == DsqlCompiledStatement::TYPE_SAVEPOINT)
	{
		// Do not generate BEGIN..END block around savepoint statement
		// to avoid breaking of savepoint logic
		statement->setSendMsg(NULL);
		statement->setReceiveMsg(NULL);
		node->genBlr(scratch);
	}
	else
	{
		const bool block = statement->getType() == DsqlCompiledStatement::TYPE_EXEC_BLOCK ||
			statement->getType() == DsqlCompiledStatement::TYPE_SELECT_BLOCK;

		// To parse sub-routines messages, they must not have that begin...end pair.
		// And since it appears to be unnecessary for execute block too, do not generate them.
		if (!block)
			scratch->appendUChar(blr_begin);

		GEN_hidden_variables(scratch);

		switch (statement->getType())
		{
		case DsqlCompiledStatement::TYPE_SELECT:
		case DsqlCompiledStatement::TYPE_SELECT_UPD:
		case DsqlCompiledStatement::TYPE_EXEC_BLOCK:
		case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
			node->genBlr(scratch);
			break;
		default:
			{
				dsql_msg* message = statement->getSendMsg();
				if (!message->msg_parameter)
					statement->setSendMsg(NULL);
				else
				{
					GEN_port(scratch, message);
					scratch->appendUChar(blr_receive);
					scratch->appendUChar(message->msg_number);
				}
				message = statement->getReceiveMsg();
				if (!message->msg_parameter)
					statement->setReceiveMsg(NULL);
				else
					GEN_port(scratch, message);
				node->genBlr(scratch);
			}
		}

		if (!block)
			scratch->appendUChar(blr_end);
	}

	scratch->appendUChar(blr_eoc);
}


/**

 	GEN_descriptor

    @brief	Generate a blr descriptor from an internal descriptor.


    @param dsqlScratch
    @param desc
    @param texttype

 **/
void GEN_descriptor( DsqlCompilerScratch* dsqlScratch, const dsc* desc, bool texttype)
{
	switch (desc->dsc_dtype)
	{
	case dtype_text:
		if (texttype || desc->dsc_ttype() == ttype_binary || desc->dsc_ttype() == ttype_none)
		{
			dsqlScratch->appendUChar(blr_text2);
			dsqlScratch->appendUShort(desc->dsc_ttype());
		}
		else
		{
			dsqlScratch->appendUChar(blr_text2);	// automatic transliteration
			dsqlScratch->appendUShort(ttype_dynamic);
		}

		dsqlScratch->appendUShort(desc->dsc_length);
		break;

	case dtype_varying:
		if (texttype || desc->dsc_ttype() == ttype_binary || desc->dsc_ttype() == ttype_none)
		{
			dsqlScratch->appendUChar(blr_varying2);
			dsqlScratch->appendUShort(desc->dsc_ttype());
		}
		else
		{
			dsqlScratch->appendUChar(blr_varying2);	// automatic transliteration
			dsqlScratch->appendUShort(ttype_dynamic);
		}
		dsqlScratch->appendUShort(desc->dsc_length - sizeof(USHORT));
		break;

	case dtype_short:
		dsqlScratch->appendUChar(blr_short);
		dsqlScratch->appendUChar(desc->dsc_scale);
		break;

	case dtype_long:
		dsqlScratch->appendUChar(blr_long);
		dsqlScratch->appendUChar(desc->dsc_scale);
		break;

	case dtype_quad:
		dsqlScratch->appendUChar(blr_quad);
		dsqlScratch->appendUChar(desc->dsc_scale);
		break;

	case dtype_int64:
		dsqlScratch->appendUChar(blr_int64);
		dsqlScratch->appendUChar(desc->dsc_scale);
		break;

	case dtype_real:
		dsqlScratch->appendUChar(blr_float);
		break;

	case dtype_double:
		dsqlScratch->appendUChar(blr_double);
		break;

	case dtype_sql_date:
		dsqlScratch->appendUChar(blr_sql_date);
		break;

	case dtype_sql_time:
		dsqlScratch->appendUChar(blr_sql_time);
		break;

	case dtype_timestamp:
		dsqlScratch->appendUChar(blr_timestamp);
		break;

	case dtype_array:
		dsqlScratch->appendUChar(blr_quad);
		dsqlScratch->appendUChar(0);
		break;

	case dtype_blob:
		dsqlScratch->appendUChar(blr_blob2);
		dsqlScratch->appendUShort(desc->dsc_sub_type);
		dsqlScratch->appendUShort(desc->getTextType());
		break;

	case dtype_boolean:
		dsqlScratch->appendUChar(blr_bool);
		break;

	default:
		// don't understand dtype
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_datatype_err));
	}
}


// Generate a parameter reference.
void GEN_parameter( DsqlCompilerScratch* dsqlScratch, const dsql_par* parameter)
{
	const dsql_msg* message = parameter->par_message;

	const dsql_par* null = parameter->par_null;
	if (null != NULL)
	{
		dsqlScratch->appendUChar(blr_parameter2);
		dsqlScratch->appendUChar(message->msg_number);
		dsqlScratch->appendUShort(parameter->par_parameter);
		dsqlScratch->appendUShort(null->par_parameter);
		return;
	}

	dsqlScratch->appendUChar(blr_parameter);
	dsqlScratch->appendUChar(message->msg_number);
	dsqlScratch->appendUShort(parameter->par_parameter);
}



/**

 	gen_plan

    @brief	Generate blr for an access plan expression.


    @param dsqlScratch
    @param plan_expression

 **/
static void gen_plan( DsqlCompilerScratch* dsqlScratch, const dsql_nod* plan_expression)
{
	// stuff the join type

	const dsql_nod* list = plan_expression->nod_arg[0];
	if (list->nod_count > 1)
	{
		dsqlScratch->appendUChar(blr_join);
		dsqlScratch->appendUChar(list->nod_count);
	}

	// stuff one or more plan items

	const dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
	{
		const dsql_nod* node = *ptr;
		if (node->nod_type == nod_plan_expr)
		{
			gen_plan(dsqlScratch, node);
			continue;
		}

		// if we're here, it must be a nod_plan_item

		dsqlScratch->appendUChar(blr_retrieve);

		// stuff the relation--the relation id itself is redundant except
		// when there is a need to differentiate the base tables of views

		/*const*/ dsql_nod* arg = node->nod_arg[0];
		reinterpret_cast<RecordSourceNode*>(arg->nod_arg[0])->genBlr(dsqlScratch);

		// now stuff the access method for this stream
		const dsql_str* index_string;

		arg = node->nod_arg[1];
		switch (arg->nod_type)
		{
		case nod_natural:
			dsqlScratch->appendUChar(blr_sequential);
			break;

		case nod_index_order:
			dsqlScratch->appendUChar(blr_navigational);
			index_string = (dsql_str*) arg->nod_arg[0];
			dsqlScratch->appendNullString(index_string->str_data);
			if (!arg->nod_arg[1])
				break;
			// dimitr: FALL INTO, if the plan item is ORDER ... INDEX (...)

		case nod_index:
			{
				dsqlScratch->appendUChar(blr_indices);
				arg = (arg->nod_type == nod_index) ? arg->nod_arg[0] : arg->nod_arg[1];
				dsqlScratch->appendUChar(arg->nod_count);
				const dsql_nod* const* ptr2 = arg->nod_arg;
				for (const dsql_nod* const* const end2 = ptr2 + arg->nod_count; ptr2 < end2; ptr2++)
				{
					index_string = (dsql_str*) * ptr2;
					dsqlScratch->appendNullString(index_string->str_data);
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

 	GEN_rse

    @brief	Generate a record selection expression.


    @param dsqlScratch
    @param rse

 **/
void GEN_rse( DsqlCompilerScratch* dsqlScratch, const dsql_nod* rseNod)
{
	const RseNode* rse = ExprNode::as<RseNode>(rseNod);

	if (rse->dsqlFlags & RecordSourceNode::DFLAG_SINGLETON)
		dsqlScratch->appendUChar(blr_singular);

	if (rse->dsqlExplicitJoin)
	{
		dsqlScratch->appendUChar(blr_rs_stream);
		fb_assert(rse->dsqlStreams->nod_count == 2);
	}
	else
		dsqlScratch->appendUChar(blr_rse);

	dsql_nod* list = rse->dsqlStreams;

	// Handle source streams

	if (ExprNode::is<UnionSourceNode>(list))
	{
		dsqlScratch->appendUChar(1);
		gen_union(dsqlScratch, rseNod);
	}
	else if (list->nod_type == nod_list)
	{
		dsqlScratch->appendUChar(list->nod_count);
		dsql_nod* const* ptr = list->nod_arg;
		for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
		{
			dsql_nod* node = *ptr;
			switch (node->nod_type)
			{
			case nod_class_exprnode:
				GEN_expr(dsqlScratch, node);
				break;
			}
		}
	}
	else
	{
		dsqlScratch->appendUChar(1);
		GEN_expr(dsqlScratch, list);
	}

	if (rse->flags & RseNode::FLAG_WRITELOCK)
		dsqlScratch->appendUChar(blr_writelock);

	dsql_nod* node;

	if ((node = rse->dsqlFirst))
	{
		dsqlScratch->appendUChar(blr_first);
		GEN_expr(dsqlScratch, node);
	}

	if ((node = rse->dsqlSkip))
	{
		dsqlScratch->appendUChar(blr_skip);
		GEN_expr(dsqlScratch, node);
	}

	if (rse->rse_jointype != blr_inner)
	{
		dsqlScratch->appendUChar(blr_join_type);
		dsqlScratch->appendUChar(rse->rse_jointype);
	}

	if ((node = rse->dsqlWhere))
	{
		dsqlScratch->appendUChar(blr_boolean);
		GEN_expr(dsqlScratch, node);
	}

	if ((list = rse->dsqlOrder))
		GEN_sort(dsqlScratch, list);

	if ((list = rse->dsqlDistinct))
	{
		dsqlScratch->appendUChar(blr_project);
		dsqlScratch->appendUChar(list->nod_count);

		dsql_nod** ptr = list->nod_arg;

		for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
			GEN_expr(dsqlScratch, *ptr);
	}

	// if the user specified an access plan to use, add it here

	if ((node = rse->dsqlPlan) != NULL)
	{
		dsqlScratch->appendUChar(blr_plan);
		gen_plan(dsqlScratch, node);
	}

	dsqlScratch->appendUChar(blr_end);
}


// Generate a sort clause.
void GEN_sort(DsqlCompilerScratch* dsqlScratch, dsql_nod* list)
{
	dsqlScratch->appendUChar(blr_sort);
	dsqlScratch->appendUChar(list->nod_count);

	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
	{
		const OrderNode* orderNode = ExprNode::as<OrderNode>(*ptr);

		switch (orderNode->nullsPlacement)
		{
			case OrderNode::NULLS_FIRST:
				dsqlScratch->appendUChar(blr_nullsfirst);
				break;
			case OrderNode::NULLS_LAST:
				dsqlScratch->appendUChar(blr_nullslast);
				break;
		}

		dsqlScratch->appendUChar((orderNode->descending ? blr_descending : blr_ascending));
		GEN_expr(dsqlScratch, orderNode->dsqlValue);
	}
}


/**

 	gen_union

    @brief	Generate a union of substreams.


    @param dsqlScratch
    @param union_node

 **/
static void gen_union( DsqlCompilerScratch* dsqlScratch, const dsql_nod* union_node)
{
	const RseNode* unionRse = ExprNode::as<RseNode>(union_node);
	const UnionSourceNode* unionSource = ExprNode::as<UnionSourceNode>(unionRse->dsqlStreams);

	if (unionSource->recursive)
		dsqlScratch->appendUChar(blr_recurse);
	else
		dsqlScratch->appendUChar(blr_union);

	// Obtain the context for UNION from the first dsql_map* node
	dsql_nod* items = unionRse->dsqlSelectList;
	dsql_nod* map_item = items->nod_arg[0];

	// AB: First item could be a virtual field generated by derived table.
	DerivedFieldNode* derivedField = ExprNode::as<DerivedFieldNode>(map_item);

	if (derivedField)
		map_item = derivedField->dsqlValue;

	dsql_ctx* union_context = ExprNode::as<DsqlMapNode>(map_item)->context;
	GEN_stuff_context(dsqlScratch, union_context);
	// secondary context number must be present once in generated blr
	union_context->ctx_flags &= ~CTX_recursive;

	dsql_nod* streams = unionSource->dsqlClauses;
	dsqlScratch->appendUChar(streams->nod_count);	// number of substreams

	dsql_nod** ptr = streams->nod_arg;
	for (const dsql_nod* const* const end = ptr + streams->nod_count; ptr < end; ptr++)
	{
		dsql_nod* sub_rse = *ptr;
		GEN_rse(dsqlScratch, sub_rse);
		items = ExprNode::as<RseNode>(sub_rse)->dsqlSelectList;
		dsqlScratch->appendUChar(blr_map);
		dsqlScratch->appendUShort(items->nod_count);
		USHORT count = 0;
		dsql_nod** iptr = items->nod_arg;
		for (const dsql_nod* const* const iend = iptr + items->nod_count; iptr < iend; iptr++)
		{
			dsqlScratch->appendUShort(count);
			GEN_expr(dsqlScratch, *iptr);
			count++;
		}
	}
}


// Write a context number into the BLR buffer. Check for possible overflow.
void GEN_stuff_context(DsqlCompilerScratch* dsqlScratch, const dsql_ctx* context)
{
	if (context->ctx_context > MAX_UCHAR)
		ERRD_post(Arg::Gds(isc_too_many_contexts));

	dsqlScratch->appendUChar(context->ctx_context);

	if (context->ctx_flags & CTX_recursive)
	{
		if (context->ctx_recursive > MAX_UCHAR)
			ERRD_post(Arg::Gds(isc_too_many_contexts));

		dsqlScratch->appendUChar(context->ctx_recursive);
	}
}
