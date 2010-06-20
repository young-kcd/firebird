/*
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
 * Adriano dos Santos Fernandes - refactored from pass1.cpp, gen.cpp, cmp.cpp, par.cpp and exe.cpp
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../common/classes/BaseStream.h"
#include "../common/classes/MsgPrint.h"
#include "../dsql/StmtNodes.h"
#include "../dsql/node.h"
#include "../jrd/blr.h"
#include "../jrd/tra.h"
#include "../jrd/recsrc/Cursor.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dfw_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/tra_proto.h"
#include "../dsql/ddl_proto.h"
#include "../jrd/vio_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/pass1_proto.h"

using namespace Firebird;
using namespace Jrd;

#include "gen/blrtable.h"


namespace Jrd {


// Write out field data type.
// Taking special care to declare international text.
void BlockNode::putDtype(DsqlCompilerScratch* dsqlScratch, const dsql_fld* field, bool useSubType)
{
#ifdef DEV_BUILD
	// Check if the field describes a known datatype

	if (field->fld_dtype > FB_NELEM(blr_dtypes) || !blr_dtypes[field->fld_dtype])
	{
		SCHAR buffer[100];

		sprintf(buffer, "Invalid dtype %d in put_dtype", field->fld_dtype);
		ERRD_bugcheck(buffer);
	}
#endif

	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	if (field->fld_not_nullable)
		dsqlScratch->appendUChar(blr_not_nullable);

	if (field->fld_type_of_name.hasData())
	{
		if (field->fld_type_of_table)
		{
			if (field->fld_explicit_collation)
			{
				dsqlScratch->appendUChar(blr_column_name2);
				dsqlScratch->appendUChar(field->fld_full_domain ? blr_domain_full : blr_domain_type_of);
				dsqlScratch->appendMetaString(field->fld_type_of_table->str_data);
				dsqlScratch->appendMetaString(field->fld_type_of_name.c_str());
				dsqlScratch->appendUShort(field->fld_ttype);
			}
			else
			{
				dsqlScratch->appendUChar(blr_column_name);
				dsqlScratch->appendUChar(field->fld_full_domain ? blr_domain_full : blr_domain_type_of);
				dsqlScratch->appendMetaString(field->fld_type_of_table->str_data);
				dsqlScratch->appendMetaString(field->fld_type_of_name.c_str());
			}
		}
		else
		{
			if (field->fld_explicit_collation)
			{
				dsqlScratch->appendUChar(blr_domain_name2);
				dsqlScratch->appendUChar(field->fld_full_domain ? blr_domain_full : blr_domain_type_of);
				dsqlScratch->appendMetaString(field->fld_type_of_name.c_str());
				dsqlScratch->appendUShort(field->fld_ttype);
			}
			else
			{
				dsqlScratch->appendUChar(blr_domain_name);
				dsqlScratch->appendUChar(field->fld_full_domain ? blr_domain_full : blr_domain_type_of);
				dsqlScratch->appendMetaString(field->fld_type_of_name.c_str());
			}
		}

		return;
	}

	switch (field->fld_dtype)
	{
		case dtype_cstring:
		case dtype_text:
		case dtype_varying:
		case dtype_blob:
			if (!useSubType)
				dsqlScratch->appendUChar(blr_dtypes[field->fld_dtype]);
			else if (field->fld_dtype == dtype_varying)
			{
				dsqlScratch->appendUChar(blr_varying2);
				dsqlScratch->appendUShort(field->fld_ttype);
			}
			else if (field->fld_dtype == dtype_cstring)
			{
				dsqlScratch->appendUChar(blr_cstring2);
				dsqlScratch->appendUShort(field->fld_ttype);
			}
			else if (field->fld_dtype == dtype_blob)
			{
				dsqlScratch->appendUChar(blr_blob2);
				dsqlScratch->appendUShort(field->fld_sub_type);
				dsqlScratch->appendUShort(field->fld_ttype);
			}
			else
			{
				dsqlScratch->appendUChar(blr_text2);
				dsqlScratch->appendUShort(field->fld_ttype);
			}

			if (field->fld_dtype == dtype_varying)
				dsqlScratch->appendUShort(field->fld_length - sizeof(USHORT));
			else if (field->fld_dtype != dtype_blob)
				dsqlScratch->appendUShort(field->fld_length);
			break;

		default:
			dsqlScratch->appendUChar(blr_dtypes[field->fld_dtype]);
			if (DTYPE_IS_EXACT(field->fld_dtype) || (dtype_quad == field->fld_dtype))
				dsqlScratch->appendUChar(field->fld_scale);
			break;
	}
}

// Emit dyn for the local variables declared in a procedure or trigger.
void BlockNode::putLocalVariables(DsqlCompilerScratch* dsqlScratch, const dsql_nod* parameters,
	SSHORT locals)
{
	if (!parameters)
		return;

	dsql_nod* const* ptr = parameters->nod_arg;
	for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ptr++)
	{
		dsql_nod* parameter = *ptr;

		dsqlScratch->putDebugSrcInfo(parameter->nod_line, parameter->nod_column);

		if (parameter->nod_type == Dsql::nod_def_field)
		{
			dsql_fld* field = (dsql_fld*) parameter->nod_arg[Dsql::e_dfl_field];
			const dsql_nod* const* rest = ptr;
			while (++rest != end)
			{
				if ((*rest)->nod_type == Dsql::nod_def_field)
				{
					const dsql_fld* rest_field = (dsql_fld*) (*rest)->nod_arg[Dsql::e_dfl_field];
					if (field->fld_name == rest_field->fld_name)
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
								  Arg::Gds(isc_dsql_duplicate_spec) << Arg::Str(field->fld_name));
					}
				}
			}

			dsql_nod* var_node = MAKE_variable(field, field->fld_name.c_str(), VAR_local, 0, 0, locals);
			variables.add(var_node);

			dsql_var* variable = (dsql_var*) var_node->nod_arg[Dsql::e_var_variable];
			putLocalVariable(dsqlScratch, variable, parameter,
				reinterpret_cast<const dsql_str*>(parameter->nod_arg[Dsql::e_dfl_collate]));

			// Some field attributes are calculated inside
			// putLocalVariable(), so we reinitialize the
			// descriptor
			MAKE_desc_from_field(&var_node->nod_desc, field);

			locals++;
		}
		else if (parameter->nod_type == Dsql::nod_cursor)
		{
			PASS1_statement(dsqlScratch, parameter);
			GEN_statement(dsqlScratch, parameter);
		}
	}
}

// Write out local variable field data type.
void BlockNode::putLocalVariable(DsqlCompilerScratch* dsqlScratch, dsql_var* variable,
	dsql_nod* hostParam, const dsql_str* collationName)
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();
	dsql_fld* field = variable->var_field;

	dsqlScratch->appendUChar(blr_dcl_variable);
	dsqlScratch->appendUShort(variable->var_variable_number);
	DDL_resolve_intl_type(dsqlScratch, field, collationName);

	//const USHORT dtype = field->fld_dtype;

	putDtype(dsqlScratch, field, true);
	//field->fld_dtype = dtype;

	// Check for a default value, borrowed from define_domain
	dsql_nod* node = hostParam ? hostParam->nod_arg[Dsql::e_dfl_default] : NULL;

	if (node || (!field->fld_full_domain && !field->fld_not_nullable))
	{
		dsqlScratch->appendUChar(blr_assignment);

		if (node)
		{
			fb_assert(node->nod_type == Dsql::nod_def_default);
			PsqlChanger psqlChanger(dsqlScratch, false);
			node = PASS1_node(dsqlScratch, node->nod_arg[Dsql::e_dft_default]);
			GEN_expr(dsqlScratch, node);
		}
		else
			dsqlScratch->appendUChar(blr_null);	// Initialize variable to NULL

		dsqlScratch->appendUChar(blr_variable);
		dsqlScratch->appendUShort(variable->var_variable_number);
	}
	else
	{
		dsqlScratch->appendUChar(blr_init_variable);
		dsqlScratch->appendUShort(variable->var_variable_number);
	}

	if (variable->var_name[0])	// Not a function return value
		dsqlScratch->putDebugVariable(variable->var_variable_number, variable->var_name);

	++dsqlScratch->hiddenVarsNumber;
}

// Try to resolve variable name against parameters and local variables.
dsql_nod* BlockNode::resolveVariable(const dsql_str* varName)
{
	for (dsql_nod* const* i = variables.begin(); i != variables.end(); ++i)
	{
		dsql_nod* var_node = *i;
		fb_assert(var_node->nod_type == Dsql::nod_variable);

		if (var_node->nod_type == Dsql::nod_variable)
		{
			const dsql_var* variable = (dsql_var*) var_node->nod_arg[Dsql::e_var_variable];
			DEV_BLKCHK(variable, dsql_type_var);

			if (!strcmp(varName->str_data, variable->var_name))
				return var_node;
		}
	}

	return NULL;
}

// Generate BLR for a return.
void BlockNode::genReturn(DsqlCompilerScratch* dsqlScratch, bool eosFlag)
{
	if (hasEos && !eosFlag)
		dsqlScratch->appendUChar(blr_begin);

	dsqlScratch->appendUChar(blr_send);
	dsqlScratch->appendUChar(1);
	dsqlScratch->appendUChar(blr_begin);

	for (Array<dsql_nod*>::const_iterator i = outputVariables.begin(); i != outputVariables.end(); ++i)
	{
		const dsql_nod* parameter = *i;
		const dsql_var* variable = (dsql_var*) parameter->nod_arg[Dsql::e_var_variable];
		dsqlScratch->appendUChar(blr_assignment);
		dsqlScratch->appendUChar(blr_variable);
		dsqlScratch->appendUShort(variable->var_variable_number);
		dsqlScratch->appendUChar(blr_parameter2);
		dsqlScratch->appendUChar(variable->var_msg_number);
		dsqlScratch->appendUShort(variable->var_msg_item);
		dsqlScratch->appendUShort(variable->var_msg_item + 1);
	}

	if (hasEos)
	{
		dsqlScratch->appendUChar(blr_assignment);
		dsqlScratch->appendUChar(blr_literal);
		dsqlScratch->appendUChar(blr_short);
		dsqlScratch->appendUChar(0);
		dsqlScratch->appendUShort((eosFlag ? 0 : 1));
		dsqlScratch->appendUChar(blr_parameter);
		dsqlScratch->appendUChar(1);
		dsqlScratch->appendUShort(USHORT(2 * outputVariables.getCount()));
	}

	dsqlScratch->appendUChar(blr_end);

	if (hasEos && !eosFlag)
	{
		dsqlScratch->appendUChar(blr_stall);
		dsqlScratch->appendUChar(blr_end);
	}
}


//--------------------


DmlNode* DmlNode::pass1(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode)
{
	node = aNode;
	return pass1(tdbb, csb);
}

DmlNode* DmlNode::pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode)
{
	node = aNode;
	return pass2(tdbb, csb);
}


//--------------------


StmtNode* SavepointEncloseNode::make(MemoryPool& pool, DsqlCompilerScratch* dsqlScratch, StmtNode* node)
{
	if (dsqlScratch->errorHandlers)
	{
		node = FB_NEW(pool) SavepointEncloseNode(pool, node);
		node->dsqlPass(dsqlScratch);
	}

	return node;
}


void SavepointEncloseNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SavepointEncloseNode\n";
	string s;
	stmt->print(s, nodes);
	text += s;
}


void SavepointEncloseNode::genBlr()
{
	DsqlCompiledStatement* const statement = dsqlScratch->getStatement();

	dsqlScratch->appendUChar(blr_begin);
	dsqlScratch->appendUChar(blr_start_savepoint);
	stmt->genBlr();
	dsqlScratch->appendUChar(blr_end_savepoint);
	dsqlScratch->appendUChar(blr_end);
}


//--------------------


static RegisterNode<IfNode> regIfNode(blr_if);


DmlNode* IfNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	IfNode* node = FB_NEW(pool) IfNode(pool);

	node->condition = PAR_parse_node(tdbb, csb, TYPE_BOOL);
	node->trueAction = PAR_parse_node(tdbb, csb, STATEMENT);
	if (csb->csb_blr_reader.peekByte() == (UCHAR) blr_end)
		csb->csb_blr_reader.getByte(); // skip blr_end
	else
		node->falseAction = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


IfNode* IfNode::internalDsqlPass()
{
	IfNode* node = FB_NEW(getPool()) IfNode(getPool(), dsqlScratch);
	node->dsqlCondition = PASS1_node(dsqlScratch, dsqlCondition);
	node->dsqlTrueAction = PASS1_statement(dsqlScratch, dsqlTrueAction);
	node->dsqlFalseAction = PASS1_statement(dsqlScratch, dsqlFalseAction);
	return node;
}


void IfNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "IfNode";
	nodes.add(dsqlCondition);
	nodes.add(dsqlTrueAction);
	if (dsqlFalseAction)
		nodes.add(dsqlFalseAction);
}


void IfNode::genBlr()
{
	dsqlScratch->appendUChar(blr_if);
	GEN_expr(dsqlScratch, dsqlCondition);
	GEN_statement(dsqlScratch, dsqlTrueAction);
	if (dsqlFalseAction)
		GEN_statement(dsqlScratch, dsqlFalseAction);
	else
		dsqlScratch->appendUChar(blr_end);
}


IfNode* IfNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	condition = CMP_pass1(tdbb, csb, condition);
	trueAction = CMP_pass1(tdbb, csb, trueAction);
	falseAction = CMP_pass1(tdbb, csb, falseAction);
	return this;
}


IfNode* IfNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	condition = CMP_pass2(tdbb, csb, condition, node);
	trueAction = CMP_pass2(tdbb, csb, trueAction, node);
	falseAction = CMP_pass2(tdbb, csb, falseAction, node);
	return this;
}


jrd_nod* IfNode::execute(thread_db* tdbb, jrd_req* request) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		if (EVL_boolean(tdbb, condition))
		{
			request->req_operation = jrd_req::req_evaluate;
			return trueAction;
		}

		if (falseAction)
		{
			request->req_operation = jrd_req::req_evaluate;
			return falseAction;
		}

		request->req_operation = jrd_req::req_return;
	}

	return node->nod_parent;
}


//--------------------


static RegisterNode<InAutonomousTransactionNode> regInAutonomousTransactionNode(blr_auto_trans);


DmlNode* InAutonomousTransactionNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	InAutonomousTransactionNode* node = FB_NEW(pool) InAutonomousTransactionNode(pool);

	if (csb->csb_blr_reader.getByte() != 0)	// Reserved for future improvements. Should be 0 for now.
		PAR_syntax_error(csb, "0");

	node->action = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


InAutonomousTransactionNode* InAutonomousTransactionNode::internalDsqlPass()
{
	const bool autoTrans = dsqlScratch->flags & DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;
	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;

	InAutonomousTransactionNode* node = FB_NEW(getPool()) InAutonomousTransactionNode(getPool());
	node->dsqlScratch = dsqlScratch;
	node->dsqlAction = PASS1_statement(dsqlScratch, dsqlAction);

	if (!autoTrans)
		dsqlScratch->flags &= ~DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;

	return node;
}


void InAutonomousTransactionNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "InAutonomousTransactionNode";
	nodes.add(dsqlAction);
}


void InAutonomousTransactionNode::genBlr()
{
	dsqlScratch->appendUChar(blr_auto_trans);
	dsqlScratch->appendUChar(0);	// to extend syntax in the future
	GEN_statement(dsqlScratch, dsqlAction);
}


InAutonomousTransactionNode* InAutonomousTransactionNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	action = CMP_pass1(tdbb, csb, action);
	return this;
}


InAutonomousTransactionNode* InAutonomousTransactionNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	savNumberOffset = CMP_impure(csb, sizeof(SLONG));
	action = CMP_pass2(tdbb, csb, action, node);
	return this;
}


jrd_nod* InAutonomousTransactionNode::execute(thread_db* tdbb, jrd_req* request) const
{
	Jrd::Attachment* attachment = request->req_attachment;
	SLONG* savNumber = request->getImpure<SLONG>(savNumberOffset);

	if (request->req_operation == jrd_req::req_evaluate)
	{
		fb_assert(tdbb->getTransaction() == request->req_transaction);

		request->req_auto_trans.push(request->req_transaction);
		request->req_transaction = TRA_start(tdbb, request->req_transaction->tra_flags,
											 request->req_transaction->tra_lock_timeout,
											 request->req_transaction);
		tdbb->setTransaction(request->req_transaction);

		VIO_start_save_point(tdbb, request->req_transaction);
		*savNumber = request->req_transaction->tra_save_point->sav_number;

		if (!(attachment->att_flags & ATT_no_db_triggers))
		{
			// run ON TRANSACTION START triggers
			EXE_execute_db_triggers(tdbb, request->req_transaction, jrd_req::req_trigger_trans_start);
		}

		return action;
	}

	jrd_tra* transaction = request->req_transaction;
	fb_assert(transaction && transaction != attachment->getSysTransaction());

	switch (request->req_operation)
	{
	case jrd_req::req_return:
		if (!(attachment->att_flags & ATT_no_db_triggers))
		{
			// run ON TRANSACTION COMMIT triggers
			EXE_execute_db_triggers(tdbb, transaction, jrd_req::req_trigger_trans_commit);
		}

		if (transaction->tra_save_point &&
			!(transaction->tra_save_point->sav_flags & SAV_user) &&
			!transaction->tra_save_point->sav_verb_count)
		{
			VIO_verb_cleanup(tdbb, transaction);
		}

		{ // scope
			AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
				tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);
			TRA_commit(tdbb, transaction, false);
		} // end scope
		break;

	case jrd_req::req_unwind:
		if (request->req_flags & (req_leave | req_continue_loop))
		{
			try
			{
				if (!(attachment->att_flags & ATT_no_db_triggers))
				{
					// run ON TRANSACTION COMMIT triggers
					EXE_execute_db_triggers(tdbb, transaction,
											jrd_req::req_trigger_trans_commit);
				}

				if (transaction->tra_save_point &&
					!(transaction->tra_save_point->sav_flags & SAV_user) &&
					!transaction->tra_save_point->sav_verb_count)
				{
					VIO_verb_cleanup(tdbb, transaction);
				}

				AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
					tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);
				TRA_commit(tdbb, transaction, false);
			}
			catch (...)
			{
				request->req_flags &= ~(req_leave | req_continue_loop);
				throw;
			}
		}
		else
		{
			ThreadStatusGuard temp_status(tdbb);

			if (!(attachment->att_flags & ATT_no_db_triggers))
			{
				try
				{
					// run ON TRANSACTION ROLLBACK triggers
					EXE_execute_db_triggers(tdbb, transaction,
											jrd_req::req_trigger_trans_rollback);
				}
				catch (const Exception&)
				{
					if (tdbb->getDatabase()->dbb_flags & DBB_bugcheck)
					{
						throw;
					}
				}
			}

			try
			{
				AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
					tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);

				// undo all savepoints up to our one
				for (const Savepoint* save_point = transaction->tra_save_point;
					save_point && *savNumber <= save_point->sav_number;
					save_point = transaction->tra_save_point)
				{
					++transaction->tra_save_point->sav_verb_count;
					VIO_verb_cleanup(tdbb, transaction);
				}

				TRA_rollback(tdbb, transaction, false, false);
			}
			catch (const Exception&)
			{
				if (tdbb->getDatabase()->dbb_flags & DBB_bugcheck)
				{
					throw;
				}
			}
		}
		break;

	default:
		fb_assert(false);
	}

	request->req_transaction = request->req_auto_trans.pop();
	tdbb->setTransaction(request->req_transaction);

	return node->nod_parent;
}


//--------------------


ExecBlockNode* ExecBlockNode::internalDsqlPass()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	statement->setBlockNode(this);

	if (returns.hasData())
		statement->setType(DsqlCompiledStatement::TYPE_SELECT_BLOCK);
	else
		statement->setType(DsqlCompiledStatement::TYPE_EXEC_BLOCK);

	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_BLOCK;

	ExecBlockNode* node = FB_NEW(getPool()) ExecBlockNode(getPool());
	node->dsqlScratch = dsqlScratch;

	for (ParameterClause* param = parameters.begin(); param != parameters.end(); ++param)
	{
		PsqlChanger changer(dsqlScratch, false);

		node->parameters.add(*param);
		ParameterClause& newParam = node->parameters.back();

		newParam.legacyParameter = PASS1_node(dsqlScratch, newParam.legacyParameter);

		if (newParam.legacyDefault)
		{
			newParam.legacyDefault->nod_arg[Dsql::e_dft_default] =
				PASS1_node(dsqlScratch, newParam.legacyDefault->nod_arg[Dsql::e_dft_default]);
		}

		newParam.resolve(dsqlScratch);
		newParam.legacyField->fld_id = param - parameters.begin();

		{ // scope
			dsql_nod* temp = newParam.legacyParameter;
			DEV_BLKCHK(temp, dsql_type_nod);

			// Initialize this stack variable, and make it look like a node
			AutoPtr<dsql_nod> desc_node(FB_NEW_RPT(*getDefaultMemoryPool(), 0) dsql_nod);

			newParam.legacyField->fld_flags |= FLD_nullable;
			MAKE_desc_from_field(&(desc_node->nod_desc), newParam.legacyField);
			PASS1_set_parameter_type(dsqlScratch, temp, desc_node, false);
		} // end scope

		if (param != parameters.begin())
			node->parameters.end()[-2].legacyField->fld_next = newParam.legacyField;
	}

	node->returns = returns;

	for (size_t i = 0; i < node->returns.getCount(); ++i)
	{
		node->returns[i].resolve(dsqlScratch);
		node->returns[i].legacyField->fld_id = i;

		if (i != 0)
			node->returns[i - 1].legacyField->fld_next = node->returns[i].legacyField;
	}

	node->localDeclList = localDeclList;
	node->body = body;

	const size_t count = node->parameters.getCount() + node->returns.getCount() +
		(node->localDeclList ? node->localDeclList->nod_count : 0);

	if (count != 0)
	{
		StrArray names(*getDefaultMemoryPool(), count);

		// Hand-made PASS1_check_unique_fields_names for arrays of ParameterClause

		Array<ParameterClause> params(parameters);
		params.add(returns.begin(), returns.getCount());

		for (size_t i = 0; i < params.getCount(); ++i)
		{
			ParameterClause& parameter = params[i];

			size_t pos;
			if (!names.find(parameter.name.c_str(), pos))
				names.insert(pos, parameter.name.c_str());
			else
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
						  Arg::Gds(isc_dsql_duplicate_spec) << Arg::Str(parameter.name));
			}
		}

		PASS1_check_unique_fields_names(names, node->localDeclList);
	}

	return node;
}


void ExecBlockNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecBlockNode\n";

	text += "  Returns:\n";

	for (size_t i = 0; i < returns.getCount(); ++i)
	{
		const ParameterClause& parameter = returns[i];

		string s;
		parameter.print(s);
		text += "    " + s + "\n";
	}

	nodes.add(localDeclList);
	nodes.add(body);
}


void ExecBlockNode::genBlr()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	// Update blockNode, because we have a reference to the original unprocessed node.
	statement->setBlockNode(this);

	dsqlScratch->beginDebug();

	// now do the input parameters
	for (size_t i = 0; i < parameters.getCount(); ++i)
	{
		ParameterClause& parameter = parameters[i];

		dsql_nod* var = MAKE_variable(parameter.legacyField,
			parameter.name.c_str(), VAR_input, 0, (USHORT) (2 * i), 0);

		variables.add(var);
	}

	const unsigned returnsPos = variables.getCount();

	// now do the output parameters
	for (size_t i = 0; i < returns.getCount(); ++i)
	{
		ParameterClause& parameter = returns[i];

		dsql_nod* var = MAKE_variable(parameter.legacyField,
			parameter.name.c_str(), VAR_output, 1, (USHORT) (2 * i), i);

		variables.add(var);
		outputVariables.add(var);
	}

	dsqlScratch->appendUChar(blr_begin);

	if (parameters.hasData())
	{
		revertParametersOrder(statement->getSendMsg()->msg_parameters);
		GEN_port(dsqlScratch, statement->getSendMsg());
	}
	else
		statement->setSendMsg(NULL);

	for (Array<dsql_nod*>::const_iterator i = outputVariables.begin(); i != outputVariables.end(); ++i)
	{
		dsql_par* param = MAKE_parameter(statement->getReceiveMsg(), true, true,
			(i - outputVariables.begin()) + 1, *i);
		param->par_node = *i;
		MAKE_desc(dsqlScratch, &param->par_desc, *i, NULL);
		param->par_desc.dsc_flags |= DSC_nullable;
	}

	// Set up parameter to handle EOF
	dsql_par* param = MAKE_parameter(statement->getReceiveMsg(), false, false, 0, NULL);
	statement->setEof(param);
	param->par_desc.dsc_dtype = dtype_short;
	param->par_desc.dsc_scale = 0;
	param->par_desc.dsc_length = sizeof(SSHORT);

	revertParametersOrder(statement->getReceiveMsg()->msg_parameters);
	GEN_port(dsqlScratch, statement->getReceiveMsg());

	if (parameters.hasData())
	{
		dsqlScratch->appendUChar(blr_receive);
		dsqlScratch->appendUChar(0);
	}

	dsqlScratch->appendUChar(blr_begin);

	for (unsigned i = 0; i < returnsPos; ++i)
	{
		const dsql_nod* parameter = variables[i];
		const dsql_var* variable = (dsql_var*) parameter->nod_arg[Dsql::e_var_variable];
		const dsql_fld* field = variable->var_field;

		if (field->fld_full_domain || field->fld_not_nullable)
		{
			// ASF: Validation of execute block input parameters is different than procedure
			// parameters, because we can't generate messages using the domains due to the
			// connection charset influence. So to validate, we cast them and assign to null.
			dsqlScratch->appendUChar(blr_assignment);
			dsqlScratch->appendUChar(blr_cast);
			BlockNode::putDtype(dsqlScratch, field, true);
			dsqlScratch->appendUChar(blr_parameter2);
			dsqlScratch->appendUChar(0);
			dsqlScratch->appendUShort(variable->var_msg_item);
			dsqlScratch->appendUShort(variable->var_msg_item + 1);
			dsqlScratch->appendUChar(blr_null);
		}
	}

	for (Array<dsql_nod*>::const_iterator i = outputVariables.begin(); i != outputVariables.end(); ++i)
	{
		dsql_nod* parameter = *i;
		dsql_var* variable = (dsql_var*) parameter->nod_arg[Dsql::e_var_variable];
		putLocalVariable(dsqlScratch, variable, 0, NULL);
	}

	dsqlScratch->setPsql(true);

	putLocalVariables(dsqlScratch, localDeclList, USHORT(returns.getCount()));

	dsqlScratch->loopLevel = 0;

	dsql_nod* stmtNode = PASS1_statement(dsqlScratch, body);
	GEN_hidden_variables(dsqlScratch, false);

	dsqlScratch->appendUChar(blr_stall);
	// Put a label before body of procedure, so that
	// any exit statement can get out
	dsqlScratch->appendUChar(blr_label);
	dsqlScratch->appendUChar(0);
	GEN_statement(dsqlScratch, stmtNode);
	if (returns.hasData())
		statement->setType(DsqlCompiledStatement::TYPE_SELECT_BLOCK);
	else
		statement->setType(DsqlCompiledStatement::TYPE_EXEC_BLOCK);

	dsqlScratch->appendUChar(blr_end);
	genReturn(dsqlScratch, true);
	dsqlScratch->appendUChar(blr_end);

	dsqlScratch->endDebug();
}


// Revert parameters order for EXECUTE BLOCK statement
void ExecBlockNode::revertParametersOrder(Array<dsql_par*>& parameters)
{
	int start = 0;
	int end = int(parameters.getCount()) - 1;

	while (start < end)
	{
		dsql_par* temp = parameters[start];
		parameters[start] = parameters[end];
		parameters[end] = temp;
		++start;
		--end;
	}
}


//--------------------


static RegisterNode<ExceptionNode> regExceptionNode(blr_abort);


DmlNode* ExceptionNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	ExceptionNode* node = FB_NEW(pool) ExceptionNode(pool);
	const UCHAR type = csb->csb_blr_reader.peekByte();
	const USHORT codeType = csb->csb_blr_reader.getByte();

	// Don't create PsqlException if blr_raise is used.
	if (codeType != blr_raise)
	{
		node->exception = FB_NEW_RPT(pool, 1) PsqlException();
		node->exception->xcp_count = 1;
		xcp_repeat& item = node->exception->xcp_rpt[0];

		switch (codeType)
		{
			case blr_sql_code:
				item.xcp_type = xcp_sql_code;
				item.xcp_code = (SSHORT) csb->csb_blr_reader.getWord();
				break;

			case blr_gds_code:
				{
					string exName;
					item.xcp_type = xcp_gds_code;
					PAR_name(csb, exName);
					exName.lower();
					SLONG code_number = PAR_symbol_to_gdscode(exName);
					if (code_number)
						item.xcp_code = code_number;
					else
						PAR_error(csb, Arg::Gds(isc_codnotdef) << Arg::Str(exName));
				}
				break;

			case blr_exception:
			case blr_exception_msg:
			case blr_exception_params:
				{
					item.xcp_type = xcp_xcp_code;
					PAR_name(csb, node->name);
					if (!(item.xcp_code = MET_lookup_exception_number(tdbb, node->name)))
						PAR_error(csb, Arg::Gds(isc_xcpnotdef) << Arg::Str(node->name));
					jrd_nod* dep_node = PAR_make_node(tdbb, e_dep_length);
					dep_node->nod_type = nod_dependency;
					dep_node->nod_arg[e_dep_object] = (jrd_nod*)(IPTR) item.xcp_code;
					dep_node->nod_arg[e_dep_object_type] = (jrd_nod*)(IPTR) obj_exception;
					csb->csb_dependencies.push(dep_node);
				}
				break;

			default:
				fb_assert(false);
				break;
		}
	}

	if (type == blr_exception_params)
	{
		const USHORT count = csb->csb_blr_reader.getWord();

		node->parameters = PAR_make_node(tdbb, count);
		node->parameters->nod_type = nod_list;
		node->parameters->nod_count = count;

		for (unsigned i = 0; i < count; ++i)
			node->parameters->nod_arg[i] = PAR_parse_node(tdbb, csb, VALUE);
	}
	else if (type == blr_exception_msg)
		node->messageExpr = PAR_parse_node(tdbb, csb, VALUE);

	return node;
}


StmtNode* ExceptionNode::internalDsqlPass()
{
	ExceptionNode* node = FB_NEW(getPool()) ExceptionNode(getPool());
	node->dsqlScratch = dsqlScratch;
	node->name = name;
	node->dsqlMessageExpr = PASS1_node(dsqlScratch, dsqlMessageExpr);
	node->dsqlParameters = PASS1_node(dsqlScratch, dsqlParameters);

	return SavepointEncloseNode::make(getPool(), dsqlScratch, node);
}


void ExceptionNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("ExceptionNode: Name: %s", name.c_str());
	if (dsqlMessageExpr)
		nodes.add(dsqlMessageExpr);
}


void ExceptionNode::genBlr()
{
	dsqlScratch->appendUChar(blr_abort);

	// If exception name is undefined, it means we have re-initiate semantics here,
	// so blr_raise verb should be generated.
	if (name.isEmpty())
	{
		dsqlScratch->appendUChar(blr_raise);
		return;
	}

	// If exception value is defined, it means we have user-defined exception message
	// here, so blr_exception_msg verb should be generated.
	if (dsqlParameters)
		dsqlScratch->appendUChar(blr_exception_params);
	else if (dsqlMessageExpr)
		dsqlScratch->appendUChar(blr_exception_msg);
	else	// Otherwise go usual way, i.e. generate blr_exception.
		dsqlScratch->appendUChar(blr_exception);

	dsqlScratch->appendNullString(name.c_str());

	// If exception parameters or value is defined, generate appropriate BLR verbs.
	if (dsqlParameters)
	{
		dsqlScratch->appendUShort(dsqlParameters->nod_count);

		dsql_nod** ptr = dsqlParameters->nod_arg;
		const dsql_nod* const* end = ptr + dsqlParameters->nod_count;
		while (ptr < end)
			GEN_expr(dsqlScratch, *ptr++);
	}
	else if (dsqlMessageExpr)
		GEN_expr(dsqlScratch, dsqlMessageExpr);
}


ExceptionNode* ExceptionNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	messageExpr = CMP_pass1(tdbb, csb, messageExpr);
	parameters = CMP_pass1(tdbb, csb, parameters);
	return this;
}


ExceptionNode* ExceptionNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	messageExpr = CMP_pass2(tdbb, csb, messageExpr, node);
	parameters = CMP_pass2(tdbb, csb, parameters, node);
	return this;
}


jrd_nod* ExceptionNode::execute(thread_db* tdbb, jrd_req* request) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		if (exception)
		{
			// PsqlException is defined, so throw an exception.
			setError(tdbb);
		}
		else if (!request->req_last_xcp.success())
		{
			// PsqlException is undefined, but there was a known exception before,
			// so re-initiate it.
			setError(tdbb);
		}
		else
		{
			// PsqlException is undefined and there weren't any exceptions before,
			// so just do nothing.
			request->req_operation = jrd_req::req_return;
		}
	}

	return node->nod_parent;
}


// Set status vector according to specified error condition and jump to handle error accordingly.
void ExceptionNode::setError(thread_db* tdbb) const
{
	SET_TDBB(tdbb);

	jrd_req* request = tdbb->getRequest();

	if (!exception)
	{
		// Retrieve the status vector and punt.
		request->req_last_xcp.copyTo(tdbb->tdbb_status_vector);
		request->req_last_xcp.clear();
		ERR_punt();
	}

	MetaName exName;
	MetaName relationName;
	TEXT message[XCP_MESSAGE_LENGTH + 1];
	MoveBuffer temp;
	USHORT length = 0;

	if (messageExpr)
	{
		UCHAR* string = NULL;

		// Evaluate exception message and convert it to string.
		dsc* desc = EVL_expr(tdbb, messageExpr);
		if (desc && !(request->req_flags & req_null))
		{
			length = MOV_make_string2(tdbb, desc, CS_METADATA, &string, temp);
			length = MIN(length, sizeof(message) - 1);

			/* dimitr: or should we throw an error here, i.e.
					replace the above assignment with the following lines:

			 if (length > sizeof(message) - 1)
				ERR_post(Arg::Gds(isc_imp_exc) << Arg::Gds(isc_blktoobig));
			*/

			memcpy(message, string, length);
		}
		else
			length = 0;
	}

	message[length] = 0;

	SLONG xcpCode = exception->xcp_rpt[0].xcp_code;

	switch (exception->xcp_rpt[0].xcp_type)
	{
		case xcp_sql_code:
			ERR_post(Arg::Gds(isc_sqlerr) << Arg::Num(xcpCode));

		case xcp_gds_code:
			if (xcpCode == isc_check_constraint)
			{
				MET_lookup_cnstrt_for_trigger(tdbb, exName, relationName,
					request->getStatement()->triggerName);
				ERR_post(Arg::Gds(xcpCode) << Arg::Str(exName) << Arg::Str(relationName));
			}
			else
				ERR_post(Arg::Gds(xcpCode));

		case xcp_xcp_code:
		{
			string tempStr;
			const TEXT* s;

			// CVC: If we have the exception name, use it instead of the number.
			// Solves SF Bug #494981.
			MET_lookup_exception(tdbb, xcpCode, exName, &tempStr);

			if (message[0])
				s = message;
			else if (tempStr.hasData())
				s = tempStr.c_str();
			else
				s = NULL;

			Arg::StatusVector status;
			ISC_STATUS msgCode = parameters ? isc_formatted_exception : isc_random;

			if (s && exName.hasData())
			{
				status << Arg::Gds(isc_except) << Arg::Num(xcpCode) <<
						  Arg::Gds(isc_random) << Arg::Str(exName) <<
						  Arg::Gds(msgCode);
			}
			else if (s)
				status << Arg::Gds(isc_except) << Arg::Num(xcpCode) <<
						  Arg::Gds(msgCode);
			else if (exName.hasData())
			{
				ERR_post(Arg::Gds(isc_except) << Arg::Num(xcpCode) <<
						 Arg::Gds(isc_random) << Arg::Str(exName));
			}
			else
				ERR_post(Arg::Gds(isc_except) << Arg::Num(xcpCode));

			// Preallocate objects, because Arg::StatusVector store pointers.
			string formattedMsg;
			ObjectsArray<string> paramsStr;

			if (parameters)
			{
				for (unsigned i = 0; i < parameters->nod_count; ++i)
				{
					const dsc* value = EVL_expr(tdbb, parameters->nod_arg[i]);

					if (!value || (request->req_flags & req_null))
						paramsStr.push(NULL_STRING_MARK);
					else
						paramsStr.push(MOV_make_string2(tdbb, value, ttype_metadata));
				}

				// And add the values to the args and status vector only after they are all created
				// and will not move in paramsStr.

				MsgFormat::SafeArg arg;
				for (unsigned i = 0; i < parameters->nod_count; ++i)
					arg << paramsStr[i].c_str();

				MsgFormat::StringRefStream stream(formattedMsg);
				MsgFormat::MsgPrint(stream, s, arg, true);

				status << formattedMsg;

				for (unsigned i = 0; i < parameters->nod_count; ++i)
					status << paramsStr[i];
			}
			else
				status << s;	// add the exception text

			ERR_post(status);
		}

		default:
			fb_assert(false);
	}
}


//--------------------


void ExitNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ExitNode";
}


void ExitNode::genBlr()
{
	dsqlScratch->appendUChar(blr_leave);
	dsqlScratch->appendUChar(0);
}


//--------------------


static RegisterNode<ForNode> regForNode(blr_for);

DmlNode* ForNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	ForNode* node = FB_NEW(pool) ForNode(pool);

	if (csb->csb_blr_reader.peekByte() == (UCHAR) blr_stall)
		node->stall = PAR_parse_node(tdbb, csb, STATEMENT);

	if (csb->csb_blr_reader.peekByte() == (UCHAR) blr_rse ||
		csb->csb_blr_reader.peekByte() == (UCHAR) blr_singular ||
		csb->csb_blr_reader.peekByte() == (UCHAR) blr_scrollable)
	{
		node->rse = PAR_parse_node(tdbb, csb, TYPE_RSE);
	}
	else
		node->rse = PAR_rse(tdbb, csb, blrOp);

	node->statement = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}

StmtNode* ForNode::internalDsqlPass()
{
	ForNode* node = FB_NEW(getPool()) ForNode(getPool());
	node->dsqlScratch = dsqlScratch;

	node->dsqlCursor = dsqlCursor;
	node->dsqlSelect = PASS1_statement(dsqlScratch, dsqlSelect);

	if (dsqlCursor)
	{
		fb_assert(dsqlCursor->nod_flags > 0);
		PASS1_cursor_name(dsqlScratch, (dsql_str*) dsqlCursor->nod_arg[Dsql::e_cur_name],
			NOD_CURSOR_ALL, false);
		dsqlCursor->nod_arg[Dsql::e_cur_rse] = node->dsqlSelect;
		dsqlCursor->nod_arg[Dsql::e_cur_number] = (dsql_nod*) (IPTR) dsqlScratch->cursorNumber++;
		dsqlScratch->cursors.push(dsqlCursor);
	}

	if (dsqlInto)
	{
		node->dsqlInto = MAKE_node(dsqlInto->nod_type, dsqlInto->nod_count);
		const dsql_nod** ptr2 = const_cast<const dsql_nod**>(node->dsqlInto->nod_arg);
		dsql_nod** ptr = dsqlInto->nod_arg;
		for (const dsql_nod* const* const end = ptr + dsqlInto->nod_count; ptr < end; ptr++)
		{
			DEV_BLKCHK(*ptr, dsql_type_nod);
			*ptr2++ = PASS1_node(dsqlScratch, *ptr);
			DEV_BLKCHK(*(ptr2 - 1), dsql_type_nod);
		}
	}

	if (dsqlAction)
	{
		// CVC: Let's add the ability to BREAK the for_select same as the while,
		// but only if the command is FOR SELECT, otherwise we have singular SELECT
		dsqlScratch->loopLevel++;
		node->dsqlLabel = PASS1_label2(dsqlScratch, NULL, dsqlLabel);
		node->dsqlAction = PASS1_statement(dsqlScratch, dsqlAction);
		dsqlScratch->loopLevel--;
		dsqlScratch->labels.pop();
	}

	if (dsqlCursor)
	{
		dsqlScratch->cursorNumber--;
		dsqlScratch->cursors.pop();
	}

	return node;
}

void ForNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ForNode";
	nodes.add(dsqlSelect);
	nodes.add(dsqlInto);
	nodes.add(dsqlCursor);
	nodes.add(dsqlAction);
	nodes.add(dsqlLabel);
}

void ForNode::genBlr()
{
	// CVC: Only put a label if this is not singular; otherwise,
	// what loop is the user trying to abandon?
	if (dsqlAction)
	{
		dsqlScratch->appendUChar(blr_label);
		dsqlScratch->appendUChar((int) (IPTR) dsqlLabel->nod_arg[Dsql::e_label_number]);
	}

	// Generate FOR loop

	dsqlScratch->appendUChar(blr_for);

	if (!dsqlAction || dsqlForceSingular)
		dsqlScratch->appendUChar(blr_singular);

	GEN_rse(dsqlScratch, dsqlSelect);
	dsqlScratch->appendUChar(blr_begin);

	// Build body of FOR loop

	dsql_nod* list = dsqlSelect->nod_arg[Dsql::e_rse_items];

	if (dsqlInto)
	{
		if (list->nod_count != dsqlInto->nod_count)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-313) <<
					  Arg::Gds(isc_dsql_count_mismatch));
		}

		dsql_nod** ptr = list->nod_arg;
		dsql_nod** ptr_to = dsqlInto->nod_arg;

		for (const dsql_nod* const* const end = ptr + list->nod_count; ptr < end; ptr++, ptr_to++)
		{
			dsqlScratch->appendUChar(blr_assignment);
			GEN_expr(dsqlScratch, *ptr);
			GEN_expr(dsqlScratch, *ptr_to);
		}
	}

	if (dsqlAction)
		GEN_statement(dsqlScratch, dsqlAction);

	dsqlScratch->appendUChar(blr_end);
}

StmtNode* ForNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	stall = CMP_pass1(tdbb, csb, stall);
	rse = CMP_pass1(tdbb, csb, rse);
	statement = CMP_pass1(tdbb, csb, statement);
	return this;
}

StmtNode* ForNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	stall = CMP_pass2(tdbb, csb, stall, node);
	rse = CMP_pass2(tdbb, csb, rse, node);
	statement = CMP_pass2(tdbb, csb, statement, node);
	return this;
}

jrd_nod* ForNode::execute(thread_db* tdbb, jrd_req* request) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			cursor->open(tdbb);
			request->req_records_affected.clear();
			// fall into

		case jrd_req::req_return:
			if (stall)
				return stall;
			// fall into

		case jrd_req::req_sync:
			if (cursor->fetchNext(tdbb))
			{
				request->req_operation = jrd_req::req_evaluate;
				return statement;
			}
			request->req_operation = jrd_req::req_return;
			// fall into

		case jrd_req::req_unwind:
		{
			jrd_nod* parent = node->nod_parent;

			if (parent && parent->nod_type == nod_label &&
				request->req_label == (USHORT)(IPTR) parent->nod_arg[e_lbl_label] &&
				(request->req_flags & req_continue_loop))
			{
				request->req_flags &= ~req_continue_loop;
				request->req_operation = jrd_req::req_sync;
				return node;
			}

			// fall into
		}

		default:
			cursor->close(tdbb);
			return node->nod_parent;
	}

	fb_assert(false);
	return NULL;
}


//--------------------


static RegisterNode<PostEventNode> regPostEventNode1(blr_post);
static RegisterNode<PostEventNode> regPostEventNode2(blr_post_arg);


DmlNode* PostEventNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	PostEventNode* node = FB_NEW(pool) PostEventNode(pool);

	node->event = PAR_parse_node(tdbb, csb, VALUE);
	if (blrOp == blr_post_arg)
		node->argument = PAR_parse_node(tdbb, csb, VALUE);

	return node;
}


PostEventNode* PostEventNode::internalDsqlPass()
{
	PostEventNode* node = FB_NEW(getPool()) PostEventNode(getPool());
	node->dsqlScratch = dsqlScratch;

	node->dsqlEvent = PASS1_node(dsqlScratch, dsqlEvent);
	node->dsqlArgument = PASS1_node(dsqlScratch, dsqlArgument);

	return node;
}


void PostEventNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "PostEventNode";
	nodes.add(dsqlEvent);
	if (dsqlArgument)
		nodes.add(dsqlArgument);
}


void PostEventNode::genBlr()
{
	if (dsqlArgument)
	{
		dsqlScratch->appendUChar(blr_post_arg);
		GEN_expr(dsqlScratch, dsqlEvent);
		GEN_expr(dsqlScratch, dsqlArgument);
	}
	else
	{
		dsqlScratch->appendUChar(blr_post);
		GEN_expr(dsqlScratch, dsqlEvent);
	}
}


PostEventNode* PostEventNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	event = CMP_pass1(tdbb, csb, event);
	argument = CMP_pass1(tdbb, csb, argument);
	return this;
}


PostEventNode* PostEventNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	event = CMP_pass2(tdbb, csb, event, node);
	argument = CMP_pass2(tdbb, csb, argument, node);
	return this;
}


jrd_nod* PostEventNode::execute(thread_db* tdbb, jrd_req* request) const
{
	jrd_tra* transaction = request->req_transaction;

	DeferredWork* work = DFW_post_work(transaction, dfw_post_event, EVL_expr(tdbb, event), 0);
	if (argument)
		DFW_post_work_arg(transaction, work, EVL_expr(tdbb, argument), 0);

	// For an autocommit transaction, events can be posted without any updates.

	if (transaction->tra_flags & TRA_autocommit)
		transaction->tra_flags |= TRA_perform_autocommit;

	if (request->req_operation == jrd_req::req_evaluate)
		request->req_operation = jrd_req::req_return;

	return node->nod_parent;
}


//--------------------


static RegisterNode<SavepointNode> regSavepointNode(blr_user_savepoint);


DmlNode* SavepointNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	SavepointNode* node = FB_NEW(pool) SavepointNode(pool);

	node->command = (Command) csb->csb_blr_reader.getByte();
	PAR_name(csb, node->name);

	return node;
}


SavepointNode* SavepointNode::internalDsqlPass()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	// ASF: It should never enter in this IF, because the grammar does not allow it.
	if (dsqlScratch->flags & DsqlCompilerScratch::FLAG_BLOCK) // blocks, procedures and triggers
	{
		const char* cmd = NULL;

		switch (command)
		{
		//case CMD_NOTHING:
		case CMD_SET:
			cmd = "SAVEPOINT";
			break;
		case CMD_RELEASE:
			cmd = "RELEASE";
			break;
		case CMD_RELEASE_ONLY:
			cmd = "RELEASE ONLY";
			break;
		case CMD_ROLLBACK:
			cmd = "ROLLBACK";
			break;
		default:
			cmd = "UNKNOWN";
			fb_assert(false);
		}

		ERRD_post(
			Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
			// Token unknown
			Arg::Gds(isc_token_err) <<
			Arg::Gds(isc_random) << Arg::Str(cmd));
	}

	statement->setType(DsqlCompiledStatement::TYPE_SAVEPOINT);

	return this;
}


void SavepointNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SavepointNode";
}


void SavepointNode::genBlr()
{
	dsqlScratch->appendUChar(blr_user_savepoint);
	dsqlScratch->appendUChar((UCHAR) command);
	dsqlScratch->appendNullString(name.c_str());
}


SavepointNode* SavepointNode::pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
{
	return this;
}


SavepointNode* SavepointNode::pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
{
	return this;
}


jrd_nod* SavepointNode::execute(thread_db* tdbb, jrd_req* request) const
{
	jrd_tra* transaction = request->req_transaction;

	if (request->req_operation == jrd_req::req_evaluate &&
		transaction != request->req_attachment->getSysTransaction())
	{
		// Skip the savepoint created by EXE_start
		Savepoint* savepoint = transaction->tra_save_point->sav_next;
		Savepoint* previous = transaction->tra_save_point;

		// Find savepoint
		bool found = false;
		while (true)
		{
			if (!savepoint || !(savepoint->sav_flags & SAV_user))
				break;

			if (!strcmp(name.c_str(), savepoint->sav_name))
			{
				found = true;
				break;
			}

			previous = savepoint;
			savepoint = savepoint->sav_next;
		}

		if (!found && command != CMD_SET)
			ERR_post(Arg::Gds(isc_invalid_savepoint) << Arg::Str(name));

		switch (command)
		{
			case CMD_SET:
				// Release the savepoint
				if (found)
				{
					Savepoint* const current = transaction->tra_save_point;
					transaction->tra_save_point = savepoint;
					EXE_verb_cleanup(tdbb, transaction);
					previous->sav_next = transaction->tra_save_point;
					transaction->tra_save_point = current;
				}

				// Use the savepoint created by EXE_start
				transaction->tra_save_point->sav_flags |= SAV_user;
				strcpy(transaction->tra_save_point->sav_name, name.c_str());
				break;

			case CMD_RELEASE_ONLY:
			{
				// Release the savepoint
				Savepoint* const current = transaction->tra_save_point;
				transaction->tra_save_point = savepoint;
				EXE_verb_cleanup(tdbb, transaction);
				previous->sav_next = transaction->tra_save_point;
				transaction->tra_save_point = current;
				break;
			}

			case CMD_RELEASE:
			{
				const SLONG sav_number = savepoint->sav_number;

				// Release the savepoint and all subsequent ones
				while (transaction->tra_save_point &&
					transaction->tra_save_point->sav_number >= sav_number)
				{
					EXE_verb_cleanup(tdbb, transaction);
				}

				// Restore the savepoint initially created by EXE_start
				VIO_start_save_point(tdbb, transaction);
				break;
			}

			case CMD_ROLLBACK:
			{
				const SLONG sav_number = savepoint->sav_number;

				// Undo the savepoint
				while (transaction->tra_save_point &&
					transaction->tra_save_point->sav_number >= sav_number)
				{
					transaction->tra_save_point->sav_verb_count++;
					EXE_verb_cleanup(tdbb, transaction);
				}

				// Now set the savepoint again to allow to return to it later
				VIO_start_save_point(tdbb, transaction);
				transaction->tra_save_point->sav_flags |= SAV_user;
				strcpy(transaction->tra_save_point->sav_name, name.c_str());
				break;
			}

			default:
				BUGCHECK(232);
				break;
		}
	}

	request->req_operation = jrd_req::req_return;

	return node->nod_parent;
}


//--------------------


static RegisterNode<SuspendNode> regSuspendNode(blr_send);


DmlNode* SuspendNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	SuspendNode* node = FB_NEW(pool) SuspendNode(pool);

	USHORT n = csb->csb_blr_reader.getByte();
	node->message = csb->csb_rpt[n].csb_message;
	node->statement = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


SuspendNode* SuspendNode::internalDsqlPass()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	if (dsqlScratch->flags & (DsqlCompilerScratch::FLAG_TRIGGER | DsqlCompilerScratch::FLAG_FUNCTION))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown
				  Arg::Gds(isc_token_err) <<
				  Arg::Gds(isc_random) << Arg::Str("SUSPEND"));
	}

	if (dsqlScratch->flags & DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK)	// autonomous transaction
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_unsupported_in_auto_trans) << Arg::Str("SUSPEND"));
	}

	statement->addFlags(DsqlCompiledStatement::FLAG_SELECTABLE);

	blockNode = statement->getBlockNode();

	return this;
}


void SuspendNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SuspendNode";
}


void SuspendNode::genBlr()
{
	if (blockNode)
		blockNode->genReturn(dsqlScratch);
}


SuspendNode* SuspendNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	statement = CMP_pass1(tdbb, csb, statement);
	return this;
}


SuspendNode* SuspendNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	statement = CMP_pass2(tdbb, csb, statement, node);
	return this;
}


// Execute a SEND statement.
jrd_nod* SuspendNode::execute(thread_db* /*tdbb*/, jrd_req* request) const
{
	switch (request->req_operation)
	{
	case jrd_req::req_evaluate:
		return statement;

	case jrd_req::req_return:
		request->req_operation = jrd_req::req_send;
		request->req_message = message;
		request->req_flags |= req_stall;
		return node;

	case jrd_req::req_proceed:
		request->req_operation = jrd_req::req_return;
		return node->nod_parent;

	default:
		return node->nod_parent;
	}
}


//--------------------


ReturnNode* ReturnNode::internalDsqlPass()
{
	DsqlCompiledStatement* const statement = dsqlScratch->getStatement();

	if (!(dsqlScratch->flags & DsqlCompilerScratch::FLAG_FUNCTION))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown
				  Arg::Gds(isc_token_err) <<
				  Arg::Gds(isc_random) << Arg::Str("RETURN"));
	}

	if (dsqlScratch->flags & DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK)	// autonomous transaction
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_unsupported_in_auto_trans) << Arg::Str("RETURN"));
	}

	ReturnNode* node = FB_NEW(getPool()) ReturnNode(getPool());
	node->dsqlScratch = dsqlScratch;
	node->blockNode = statement->getBlockNode();
	node->value = PASS1_node(dsqlScratch, value);

	return node;
}


void ReturnNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ReturnNode";
	nodes.add(value);
}


void ReturnNode::genBlr()
{
	DsqlCompiledStatement* const statement = dsqlScratch->getStatement();

	dsqlScratch->appendUChar(blr_assignment);
	GEN_expr(dsqlScratch, value);
	dsqlScratch->appendUChar(blr_variable);
	dsqlScratch->appendUShort(0);

	blockNode->genReturn(dsqlScratch);

	dsqlScratch->appendUChar(blr_leave);
	dsqlScratch->appendUChar(0);
}


}	// namespace Jrd
