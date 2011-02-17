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

static void gen_error_condition(DsqlCompilerScratch*, const dsql_nod*);
static void gen_plan(DsqlCompilerScratch*, const dsql_nod*);
static void gen_select(DsqlCompilerScratch*, dsql_nod*);
static void gen_statement(DsqlCompilerScratch*, const dsql_nod*);
static void gen_table_lock(DsqlCompilerScratch*, const dsql_nod*, USHORT);
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
	switch (node->nod_type)
	{
	case nod_class_exprnode:
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

			return;
		}

	case nod_assign:
		dsqlScratch->appendUChar(blr_assignment);
		GEN_expr(dsqlScratch, node->nod_arg[0]);
		GEN_expr(dsqlScratch, node->nod_arg[1]);
		return;

	default:
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

			parameter->par_desc.dsc_length =
				UTLD_char_length_to_byte_length(parameter->par_desc.dsc_length / fromCharSetBPC, toCharSetBPC);

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

		const USHORT align = type_alignments[parameter->par_desc.dsc_dtype];
		if (align)
			offset = FB_ALIGN(offset, align);
		parameter->par_desc.dsc_address = (UCHAR*)(IPTR) offset;
		offset += parameter->par_desc.dsc_length;
		GEN_descriptor(dsqlScratch, &parameter->par_desc, false);
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


/**

 	GEN_request

    @brief	Generate complete blr for a dsqlScratch.


    @param dsqlScratch
    @param node

 **/
void GEN_request(DsqlCompilerScratch* scratch, dsql_nod* node)
{
	DsqlCompiledStatement* statement = scratch->getStatement();

	if (statement->getType() == DsqlCompiledStatement::TYPE_CREATE_DB ||
		statement->getType() == DsqlCompiledStatement::TYPE_DDL)
	{
		DDL_generate(scratch, node);
		return;
	}

	if (statement->getFlags() & DsqlCompiledStatement::FLAG_BLR_VERSION4)
		scratch->appendUChar(blr_version4);
	else
		scratch->appendUChar(blr_version5);

	if (statement->getType() == DsqlCompiledStatement::TYPE_SAVEPOINT)
	{
		// Do not generate BEGIN..END block around savepoint statement
		// to avoid breaking of savepoint logic
		statement->setSendMsg(NULL);
		statement->setReceiveMsg(NULL);
		GEN_statement(scratch, node);
	}
	else
	{
		scratch->appendUChar(blr_begin);

		GEN_hidden_variables(scratch);

		switch (statement->getType())
		{
		case DsqlCompiledStatement::TYPE_SELECT:
		case DsqlCompiledStatement::TYPE_SELECT_UPD:
			gen_select(scratch, node);
			break;
		case DsqlCompiledStatement::TYPE_EXEC_BLOCK:
		case DsqlCompiledStatement::TYPE_SELECT_BLOCK:
			GEN_statement(scratch, node);
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
				GEN_statement(scratch, node);
			}
		}
		scratch->appendUChar(blr_end);
	}

	scratch->appendUChar(blr_eoc);
}


/**

 	GEN_start_transaction

    @brief	Generate tpb for set transaction.  Use blr string of dsqlScratch.
 	If a value is not specified, default is not STUFF'ed, let the
 	engine handle it.
 	Do not allow an option to be specified more than once.


    @param dsqlScratch
    @param tran_node

 **/
void GEN_start_transaction( DsqlCompilerScratch* dsqlScratch, const dsql_nod* tran_node)
{
	SSHORT count = tran_node->nod_count;

	if (!count)
		return;

	const dsql_nod* node = tran_node->nod_arg[0];

	if (!node)
		return;

	// Find out isolation level - if specified. This is required for
	// specifying the correct lock level in reserving clause.

	USHORT lock_level = isc_tpb_shared;

	if (count = node->nod_count)
	{
		while (count--)
		{
			const dsql_nod* ptr = node->nod_arg[count];

			if (!ptr || ptr->nod_type != nod_isolation)
				continue;

			lock_level = (ptr->nod_flags & NOD_CONSISTENCY) ? isc_tpb_protected : isc_tpb_shared;
		}
	}


   	bool sw_access = false, sw_wait = false, sw_isolation = false,
		sw_reserve = false, sw_lock_timeout = false;
	int misc_flags = 0;

	// Stuff some version info.
	if (count = node->nod_count)
		dsqlScratch->appendUChar(isc_tpb_version1);

	while (count--)
	{
		const dsql_nod* ptr = node->nod_arg[count];

		if (!ptr)
			continue;

		switch (ptr->nod_type)
		{
		case nod_access:
			if (sw_access)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_dup_option));

			sw_access = true;
			if (ptr->nod_flags & NOD_READ_ONLY)
				dsqlScratch->appendUChar(isc_tpb_read);
			else
				dsqlScratch->appendUChar(isc_tpb_write);
			break;

		case nod_wait:
			if (sw_wait)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_dup_option));

			sw_wait = true;
			if (ptr->nod_flags & NOD_NO_WAIT)
				dsqlScratch->appendUChar(isc_tpb_nowait);
			else
				dsqlScratch->appendUChar(isc_tpb_wait);
			break;

		case nod_isolation:
			if (sw_isolation)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_dup_option));

			sw_isolation = true;

			if (ptr->nod_flags & NOD_CONCURRENCY)
				dsqlScratch->appendUChar(isc_tpb_concurrency);
			else if (ptr->nod_flags & NOD_CONSISTENCY)
				dsqlScratch->appendUChar(isc_tpb_consistency);
			else
			{
				dsqlScratch->appendUChar(isc_tpb_read_committed);

				if (ptr->nod_count && ptr->nod_arg[0] && ptr->nod_arg[0]->nod_type == nod_version)
				{
					if (ptr->nod_arg[0]->nod_flags & NOD_VERSION)
						dsqlScratch->appendUChar(isc_tpb_rec_version);
					else
						dsqlScratch->appendUChar(isc_tpb_no_rec_version);
				}
				else
					dsqlScratch->appendUChar(isc_tpb_no_rec_version);
			}
			break;

		case nod_reserve:
			{
				if (sw_reserve)
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_dsql_dup_option));

				sw_reserve = true;
				const dsql_nod* reserve = ptr->nod_arg[0];

				if (reserve)
				{
					const dsql_nod* const* temp = reserve->nod_arg;
					for (const dsql_nod* const* end = temp + reserve->nod_count; temp < end; temp++)
					{
						gen_table_lock(dsqlScratch, *temp, lock_level);
					}
				}
			}
			break;

		case nod_tra_misc:
			if (misc_flags & ptr->nod_flags)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_dup_option));

			misc_flags |= ptr->nod_flags;
			if (ptr->nod_flags & NOD_NO_AUTO_UNDO)
				dsqlScratch->appendUChar(isc_tpb_no_auto_undo);
			else if (ptr->nod_flags & NOD_IGNORE_LIMBO)
				dsqlScratch->appendUChar(isc_tpb_ignore_limbo);
			else if (ptr->nod_flags & NOD_RESTART_REQUESTS)
				dsqlScratch->appendUChar(isc_tpb_restart_requests);
			break;

		case nod_lock_timeout:
			if (sw_lock_timeout)
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_dsql_dup_option));

			sw_lock_timeout = true;
			if (ptr->nod_count == 1 && ExprNode::is<LiteralNode>(ptr->nod_arg[0]))
			{
				const int lck_timeout = (int) ExprNode::as<LiteralNode>(ptr->nod_arg[0])->getSlong();
				dsqlScratch->appendUChar(isc_tpb_lock_timeout);
				dsqlScratch->appendUChar(2);
				dsqlScratch->appendUShort(lck_timeout);
			}
			break;

		default:
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_tran_err));
		}
	}

	// Store DYN data in the statement.
	dsqlScratch->getStatement()->setDdlData(dsqlScratch->getBlrData());
}


/**

 	GEN_statement

    @brief	Generate blr for an arbitrary expression.


    @param dsqlScratch
    @param node

 **/
void GEN_statement( DsqlCompilerScratch* dsqlScratch, dsql_nod* node)
{
	dsql_nod* temp;
	dsql_nod** ptr;
	const dsql_nod* const* end;
	dsql_str* string;

	switch (node->nod_type)
	{
	case nod_assign:
		dsqlScratch->appendUChar(blr_assignment);
		GEN_expr(dsqlScratch, node->nod_arg[0]);
		GEN_expr(dsqlScratch, node->nod_arg[1]);
		return;

	case nod_block:
		dsqlScratch->appendUChar(blr_block);
		GEN_statement(dsqlScratch, node->nod_arg[e_blk_action]);
		if (node->nod_count > 1)
		{
			temp = node->nod_arg[e_blk_errs];
			for (ptr = temp->nod_arg, end = ptr + temp->nod_count; ptr < end; ptr++)
			{
				GEN_statement(dsqlScratch, *ptr);
			}
		}
		dsqlScratch->appendUChar(blr_end);
		return;

	case nod_class_stmtnode:
		{
			DmlNode* dmlNode = reinterpret_cast<DmlNode*>(node->nod_arg[0]);
			dmlNode->genBlr(dsqlScratch);
		}
		return;

	case nod_set_generator:
	case nod_set_generator2:
		dsqlScratch->appendUChar(blr_set_generator);
		string = (dsql_str*) node->nod_arg[e_gen_id_name];
		dsqlScratch->appendNullString(string->str_data);
		GEN_expr(dsqlScratch, node->nod_arg[e_gen_id_value]);
		return;

	case nod_list:
		if (!(node->nod_flags & NOD_SIMPLE_LIST))
			dsqlScratch->appendUChar(blr_begin);
		for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		{
			GEN_statement(dsqlScratch, *ptr);
		}
		if (!(node->nod_flags & NOD_SIMPLE_LIST))
			dsqlScratch->appendUChar(blr_end);
		return;

	case nod_erase:
	case nod_erase_current:
	case nod_modify:
	case nod_modify_current:
	case nod_store:
		gen_statement(dsqlScratch, node);
		return;

	case nod_on_error:
		dsqlScratch->appendUChar(blr_error_handler);
		temp = node->nod_arg[e_err_errs];
		dsqlScratch->appendUShort(temp->nod_count);
		for (ptr = temp->nod_arg, end = ptr + temp->nod_count; ptr < end; ptr++)
		{
			gen_error_condition(dsqlScratch, *ptr);
		}
		GEN_statement(dsqlScratch, node->nod_arg[e_err_action]);
		return;

	case nod_start_savepoint:
		dsqlScratch->appendUChar(blr_start_savepoint);
		return;

	case nod_end_savepoint:
		dsqlScratch->appendUChar(blr_end_savepoint);
		return;

	case nod_while:
		dsqlScratch->appendUChar(blr_label);
		dsqlScratch->appendUChar((int)(IPTR) node->nod_arg[e_while_label]->nod_arg[e_label_number]);
		dsqlScratch->appendUChar(blr_loop);
		dsqlScratch->appendUChar(blr_begin);
		dsqlScratch->appendUChar(blr_if);
		GEN_expr(dsqlScratch, node->nod_arg[e_while_cond]);
		GEN_statement(dsqlScratch, node->nod_arg[e_while_action]);
		dsqlScratch->appendUChar(blr_leave);
		dsqlScratch->appendUChar((int)(IPTR) node->nod_arg[e_while_label]->nod_arg[e_label_number]);
		dsqlScratch->appendUChar(blr_end);
		return;

	case nod_sqlcode:
	case nod_gdscode:
		dsqlScratch->appendUChar(blr_abort);
		gen_error_condition(dsqlScratch, node);
		return;

	case nod_cursor:
		dsqlScratch->appendUChar(blr_dcl_cursor);
		dsqlScratch->appendUShort((int)(IPTR) node->nod_arg[e_cur_number]);

		if (node->nod_arg[e_cur_scroll])
			dsqlScratch->appendUChar(blr_scrollable);

		GEN_rse(dsqlScratch, node->nod_arg[e_cur_rse]);
		temp = ExprNode::as<RseNode>(node->nod_arg[e_cur_rse])->dsqlSelectList;
		dsqlScratch->appendUShort(temp->nod_count);
		ptr = temp->nod_arg;
		end = ptr + temp->nod_count;

		while (ptr < end)
			GEN_expr(dsqlScratch, *ptr++);

		return;

	case nod_src_info:
		dsqlScratch->putDebugSrcInfo(node->nod_line, node->nod_column);
		GEN_statement(dsqlScratch, node->nod_arg[e_src_info_stmt]);
		return;

	default:
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_internal_err) <<
				  // gen.c: node not supported
				  Arg::Gds(isc_node_err));
	}
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


/**

 	gen_error_condition

    @brief	Generate blr for an error condtion


    @param dsqlScratch
    @param node

 **/
static void gen_error_condition( DsqlCompilerScratch* dsqlScratch, const dsql_nod* node)
{
	const dsql_str* string;

	switch (node->nod_type)
	{
	case nod_sqlcode:
		dsqlScratch->appendUChar(blr_sql_code);
		dsqlScratch->appendUShort((USHORT)(IPTR) node->nod_arg[0]);
		return;

	case nod_gdscode:
		dsqlScratch->appendUChar(blr_gds_code);
		string = (dsql_str*) node->nod_arg[0];
		dsqlScratch->appendNullString(string->str_data);
		return;

	case nod_exception:
		dsqlScratch->appendUChar(blr_exception);
		string = (dsql_str*) node->nod_arg[0];
		dsqlScratch->appendNullString(string->str_data);
		return;

	case nod_default:
		dsqlScratch->appendUChar(blr_default_code);
		return;

	default:
		fb_assert(false);
		return;
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

	if (rseNod->nod_flags & NOD_SELECT_EXPR_SINGLETON)
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


/**

 	gen_select

    @brief	Generate BLR for a SELECT statement.


    @param dsqlScratch
    @param rse

 **/
static void gen_select(DsqlCompilerScratch* dsqlScratch, dsql_nod* rseNod)
{
	dsql_ctx* context;

	RseNode* const rse = ExprNode::as<RseNode>(rseNod);
	fb_assert(rse);

	DsqlCompiledStatement* const statement = dsqlScratch->getStatement();

	// Set up parameter for things in the select list
	const dsql_nod* list = rse->dsqlSelectList;
	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
	{
		dsql_par* parameter = MAKE_parameter(statement->getReceiveMsg(), true, true, 0, *ptr);
		parameter->par_node = *ptr;
		MAKE_desc(dsqlScratch, &parameter->par_desc, *ptr);
	}

	// Set up parameter to handle EOF

	dsql_par* const parameter_eof =
		MAKE_parameter(statement->getReceiveMsg(), false, false, 0, NULL);
	statement->setEof(parameter_eof);
	parameter_eof->par_desc.dsc_dtype = dtype_short;
	parameter_eof->par_desc.dsc_scale = 0;
	parameter_eof->par_desc.dsc_length = sizeof(SSHORT);

	// Save DBKEYs for possible update later

	GenericMap<NonPooled<dsql_par*, dsql_ctx*> > paramContexts(*getDefaultMemoryPool());

	if (!rse->dsqlDistinct)
	{
		list = rse->dsqlStreams;
		dsql_nod* const* ptr2 = list->nod_arg;
		for (const dsql_nod* const* const end2 = ptr2 + list->nod_count; ptr2 < end2; ptr2++)
		{
			dsql_nod* const item = *ptr2;
			RelationSourceNode* relNode;

			if (item && (relNode = ExprNode::as<RelationSourceNode>(item)))
			{
				context = relNode->dsqlContext;
				const dsql_rel* const relation = context->ctx_relation;

				if (relation)
				{
					// Set up dbkey
					dsql_par* parameter =
						MAKE_parameter(statement->getReceiveMsg(), false, false, 0, NULL);

					parameter->par_dbkey_relname = relation->rel_name;
					paramContexts.put(parameter, context);

					parameter->par_desc.dsc_dtype = dtype_text;
					parameter->par_desc.dsc_ttype() = ttype_binary;
					parameter->par_desc.dsc_length = relation->rel_dbkey_length;

					// Set up record version - for post v33 databases

					parameter = MAKE_parameter(statement->getReceiveMsg(), false, false, 0, NULL);
					parameter->par_rec_version_relname = relation->rel_name;
					paramContexts.put(parameter, context);

					parameter->par_desc.dsc_dtype = dtype_text;
					parameter->par_desc.dsc_ttype() = ttype_binary;
					parameter->par_desc.dsc_length = relation->rel_dbkey_length / 2;
				}
			}
		}
	}

	// Generate definitions for the messages

	GEN_port(dsqlScratch, statement->getReceiveMsg());
	dsql_msg* message = statement->getSendMsg();
	if (message->msg_parameter)
		GEN_port(dsqlScratch, message);
	else
		statement->setSendMsg(NULL);

	// If there is a send message, build a RECEIVE

	if ((message = statement->getSendMsg()) != NULL)
	{
		dsqlScratch->appendUChar(blr_receive);
		dsqlScratch->appendUChar(message->msg_number);
	}

	// Generate FOR loop

	message = statement->getReceiveMsg();

	dsqlScratch->appendUChar(blr_for);
	dsqlScratch->appendUChar(blr_stall);
	GEN_rse(dsqlScratch, rseNod);

	dsqlScratch->appendUChar(blr_send);
	dsqlScratch->appendUChar(message->msg_number);
	dsqlScratch->appendUChar(blr_begin);

	// Build body of FOR loop

	SSHORT constant;
	dsc constant_desc;
	constant_desc.makeShort(0, &constant);

	// Add invalid usage here

	dsqlScratch->appendUChar(blr_assignment);
	constant = 1;
	LiteralNode::genConstant(dsqlScratch, &constant_desc, false);
	GEN_parameter(dsqlScratch, statement->getEof());

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		dsql_par* const parameter = message->msg_parameters[i];

		if (parameter->par_node)
		{
			dsqlScratch->appendUChar(blr_assignment);
			GEN_expr(dsqlScratch, parameter->par_node);
			GEN_parameter(dsqlScratch, parameter);
		}

		if (parameter->par_dbkey_relname.hasData() && paramContexts.get(parameter, context))
		{
			dsqlScratch->appendUChar(blr_assignment);
			dsqlScratch->appendUChar(blr_dbkey);
			GEN_stuff_context(dsqlScratch, context);
			GEN_parameter(dsqlScratch, parameter);
		}

		if (parameter->par_rec_version_relname.hasData() && paramContexts.get(parameter, context))
		{
			dsqlScratch->appendUChar(blr_assignment);
			dsqlScratch->appendUChar(blr_record_version);
			GEN_stuff_context(dsqlScratch, context);
			GEN_parameter(dsqlScratch, parameter);
		}
	}

	dsqlScratch->appendUChar(blr_end);
	dsqlScratch->appendUChar(blr_send);
	dsqlScratch->appendUChar(message->msg_number);
	dsqlScratch->appendUChar(blr_assignment);
	constant = 0;
	LiteralNode::genConstant(dsqlScratch, &constant_desc, false);
	GEN_parameter(dsqlScratch, statement->getEof());
}


// Generate a sort clause.
void GEN_sort(DsqlCompilerScratch* dsqlScratch, dsql_nod* list)
{
	dsqlScratch->appendUChar(blr_sort);
	dsqlScratch->appendUChar(list->nod_count);

	dsql_nod* const* ptr = list->nod_arg;
	for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++)
	{
		dsql_nod* nulls_placement = (*ptr)->nod_arg[e_order_nulls];

		if (nulls_placement)
		{
			switch (ExprNode::as<LiteralNode>(nulls_placement)->getSlong())
			{
				case NOD_NULLS_FIRST:
					dsqlScratch->appendUChar(blr_nullsfirst);
					break;
				case NOD_NULLS_LAST:
					dsqlScratch->appendUChar(blr_nullslast);
					break;
			}
		}

		if ((*ptr)->nod_arg[e_order_flag])
			dsqlScratch->appendUChar(blr_descending);
		else
			dsqlScratch->appendUChar(blr_ascending);

		GEN_expr(dsqlScratch, (*ptr)->nod_arg[e_order_field]);
	}
}


/**

 	gen_statement

    @brief	Generate BLR for DML statements.


    @param dsqlScratch
    @param node

 **/
static void gen_statement(DsqlCompilerScratch* dsqlScratch, const dsql_nod* node)
{
	dsql_nod* rse = NULL;
	const dsql_msg* message = NULL;
	bool innerSend = dsqlScratch->flags & DsqlCompilerScratch::FLAG_UPDATE_OR_INSERT;
	const bool merge = dsqlScratch->flags & DsqlCompilerScratch::FLAG_MERGE;

	switch (node->nod_type)
	{
	case nod_store:
		rse = node->nod_arg[e_sto_rse];
		break;
	case nod_modify:
		rse = node->nod_arg[e_mod_rse];
		break;
	case nod_erase:
		rse = node->nod_arg[e_era_rse];
		break;
	default:
		innerSend = true;
		break;
	}

	if (dsqlScratch->getStatement()->getType() == DsqlCompiledStatement::TYPE_EXEC_PROCEDURE &&
		!innerSend && !merge)
	{
		if ((message = dsqlScratch->getStatement()->getReceiveMsg()))
		{
			dsqlScratch->appendUChar(blr_send);
			dsqlScratch->appendUChar(message->msg_number);
		}
	}

	if (rse)
	{
		dsqlScratch->appendUChar(blr_for);
		GEN_expr(dsqlScratch, rse);
	}

	if (dsqlScratch->getStatement()->getType() == DsqlCompiledStatement::TYPE_EXEC_PROCEDURE)
	{
		if ((message = dsqlScratch->getStatement()->getReceiveMsg()))
		{
			dsqlScratch->appendUChar(blr_begin);

			if (innerSend && !merge)
			{
				dsqlScratch->appendUChar(blr_send);
				dsqlScratch->appendUChar(message->msg_number);
			}
		}
	}

	dsql_nod* temp;
	const dsql_ctx* context;

	switch (node->nod_type)
	{
	case nod_store:
		dsqlScratch->appendUChar(node->nod_arg[e_sto_return] ? blr_store2 : blr_store);
		GEN_expr(dsqlScratch, node->nod_arg[e_sto_relation]);
		GEN_statement(dsqlScratch, node->nod_arg[e_sto_statement]);
		if (node->nod_arg[e_sto_return])
			GEN_statement(dsqlScratch, node->nod_arg[e_sto_return]);
		break;

	case nod_modify:
		dsqlScratch->appendUChar(node->nod_arg[e_mod_return] ? blr_modify2 : blr_modify);
		temp = node->nod_arg[e_mod_source];
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
		GEN_stuff_context(dsqlScratch, context);
		temp = node->nod_arg[e_mod_update];
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
		GEN_stuff_context(dsqlScratch, context);
		GEN_statement(dsqlScratch, node->nod_arg[e_mod_statement]);
		if (node->nod_arg[e_mod_return])
			GEN_statement(dsqlScratch, node->nod_arg[e_mod_return]);
		break;

	case nod_modify_current:
		dsqlScratch->appendUChar(node->nod_arg[e_mdc_return] ? blr_modify2 : blr_modify);
		context = (dsql_ctx*) node->nod_arg[e_mdc_context];
		GEN_stuff_context(dsqlScratch, context);
		temp = node->nod_arg[e_mdc_update];
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
		GEN_stuff_context(dsqlScratch, context);
		GEN_statement(dsqlScratch, node->nod_arg[e_mdc_statement]);
		if (node->nod_arg[e_mdc_return])
			GEN_statement(dsqlScratch, node->nod_arg[e_mdc_return]);
		break;

	case nod_erase:
		temp = node->nod_arg[e_era_relation];
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
		if (node->nod_arg[e_era_return])
		{
			dsqlScratch->appendUChar(blr_begin);
			GEN_statement(dsqlScratch, node->nod_arg[e_era_return]);
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
			dsqlScratch->appendUChar(blr_end);
		}
		else
		{
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
		}
		break;

	case nod_erase_current:
		context = (dsql_ctx*) node->nod_arg[e_erc_context];
		if (node->nod_arg[e_erc_return])
		{
			dsqlScratch->appendUChar(blr_begin);
			GEN_statement(dsqlScratch, node->nod_arg[e_erc_return]);
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
			dsqlScratch->appendUChar(blr_end);
		}
		else
		{
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
		}
		break;

	default:
		fb_assert(false);
	}

	if (message)
		dsqlScratch->appendUChar(blr_end);
}


/**

 	gen_table_lock

    @brief	Generate tpb for table lock.
 	If lock level is specified, it overrrides the transaction lock level.


    @param dsqlScratch
    @param tbl_lock
    @param lock_level

 **/
static void gen_table_lock( DsqlCompilerScratch* dsqlScratch, const dsql_nod* tbl_lock, USHORT lock_level)
{
	if (!tbl_lock || tbl_lock->nod_type != nod_table_lock)
		return;

	const dsql_nod* tbl_names = tbl_lock->nod_arg[e_lock_tables];
	SSHORT flags = 0;

	if (tbl_lock->nod_arg[e_lock_mode])
		flags = tbl_lock->nod_arg[e_lock_mode]->nod_flags;

	if (flags & NOD_PROTECTED)
		lock_level = isc_tpb_protected;
	else if (flags & NOD_SHARED)
		lock_level = isc_tpb_shared;

	const USHORT lock_mode = (flags & NOD_WRITE) ? isc_tpb_lock_write : isc_tpb_lock_read;

	const dsql_nod* const* ptr = tbl_names->nod_arg;
	for (const dsql_nod* const* const end = ptr + tbl_names->nod_count; ptr < end; ptr++)
	{
		const RelationSourceNode* relNode = ExprNode::as<RelationSourceNode>(*ptr);

		if (!relNode)
			continue;

		dsqlScratch->appendUChar(lock_mode);

		// stuff table name
		dsqlScratch->appendNullString(relNode->dsqlName.c_str());

		dsqlScratch->appendUChar(lock_level);
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

	if (unionRse->dsqlStreams->nod_flags & NOD_UNION_RECURSIVE)
		dsqlScratch->appendUChar(blr_recurse);
	else
		dsqlScratch->appendUChar(blr_union);

	// Obtain the context for UNION from the first dsql_map* node
	dsql_nod* items = unionRse->dsqlSelectList;
	dsql_nod* map_item = items->nod_arg[0];

	// AB: First item could be a virtual field generated by derived table.
	DerivedFieldNode* derivedField;

	if ((derivedField = ExprNode::as<DerivedFieldNode>(map_item)))
		map_item = derivedField->dsqlValue;

	dsql_ctx* union_context = ExprNode::as<DsqlMapNode>(map_item)->context;
	GEN_stuff_context(dsqlScratch, union_context);
	// secondary context number must be present once in generated blr
	union_context->ctx_flags &= ~CTX_recursive;

	dsql_nod* streams = ExprNode::as<UnionSourceNode>(unionRse->dsqlStreams)->dsqlClauses;
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
