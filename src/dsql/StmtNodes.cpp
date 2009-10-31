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
#include "../dsql/StmtNodes.h"
#include "../dsql/node.h"
#include "../jrd/jrd.h"
#include "../jrd/blr.h"
#include "../jrd/exe.h"
#include "../jrd/tra.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dfw_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
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


template <typename T>
class RegisterNode
{
public:
	explicit RegisterNode(UCHAR blr)
	{
		PAR_register(blr, T::parse);
	}
};


//--------------------


DmlNode* DmlNode::pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode)
{
	node = aNode;
	return pass2(tdbb, csb);
}


//--------------------


static RegisterNode<IfNode> regIfNode(blr_if);


DmlNode* IfNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
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
	IfNode* node = FB_NEW(getPool()) IfNode(getPool());
	node->compiledStatement = compiledStatement;
	node->dsqlCondition = PASS1_node(compiledStatement, dsqlCondition);
	node->dsqlTrueAction = PASS1_statement(compiledStatement, dsqlTrueAction);
	node->dsqlFalseAction = PASS1_statement(compiledStatement, dsqlFalseAction);

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
	stuff(compiledStatement, blr_if);
	GEN_expr(compiledStatement, dsqlCondition);
	GEN_statement(compiledStatement, dsqlTrueAction);
	if (dsqlFalseAction)
		GEN_statement(compiledStatement, dsqlFalseAction);
	else
		stuff(compiledStatement, blr_end);
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


jrd_nod* IfNode::execute(thread_db* tdbb, jrd_req* request)
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
	UCHAR blrOp)
{
	InAutonomousTransactionNode* node = FB_NEW(pool) InAutonomousTransactionNode(pool);

	if (csb->csb_blr_reader.getByte() != 0)	// Reserved for future improvements. Should be 0 for now.
		PAR_syntax_error(csb, "0");

	node->action = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


InAutonomousTransactionNode* InAutonomousTransactionNode::internalDsqlPass()
{
	const bool autoTrans = compiledStatement->req_flags & REQ_in_auto_trans_block;
	compiledStatement->req_flags |= REQ_in_auto_trans_block;

	InAutonomousTransactionNode* node = FB_NEW(getPool()) InAutonomousTransactionNode(getPool());
	node->compiledStatement = compiledStatement;
	node->dsqlAction = PASS1_statement(compiledStatement, dsqlAction);

	if (!autoTrans)
		compiledStatement->req_flags &= ~REQ_in_auto_trans_block;

	return node;
}


void InAutonomousTransactionNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "InAutonomousTransactionNode";
	nodes.add(dsqlAction);
}


void InAutonomousTransactionNode::genBlr()
{
	stuff(compiledStatement, blr_auto_trans);
	stuff(compiledStatement, 0);	// to extend syntax in the future
	GEN_statement(compiledStatement, dsqlAction);
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


jrd_nod* InAutonomousTransactionNode::execute(thread_db* tdbb, jrd_req* request)
{
	SLONG* savNumber = (SLONG*) ((char*) request + savNumberOffset);

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

		if (!(tdbb->getAttachment()->att_flags & ATT_no_db_triggers))
		{
			// run ON TRANSACTION START triggers
			EXE_execute_db_triggers(tdbb, request->req_transaction, jrd_req::req_trigger_trans_start);
		}

		return action;
	}

	jrd_tra* transaction = request->req_transaction;
	fb_assert(transaction);
	fb_assert(transaction != tdbb->getDatabase()->dbb_sys_trans);

	switch (request->req_operation)
	{
	case jrd_req::req_return:
		if (!(tdbb->getAttachment()->att_flags & ATT_no_db_triggers))
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
				if (!(tdbb->getAttachment()->att_flags & ATT_no_db_triggers))
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

			if (!(tdbb->getAttachment()->att_flags & ATT_no_db_triggers))
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
	compiledStatement->blockNode = this;

	if (legacyReturns && legacyReturns->nod_count)
		compiledStatement->req_type = REQ_SELECT_BLOCK;
	else
		compiledStatement->req_type = REQ_EXEC_BLOCK;

	compiledStatement->req_flags |= REQ_block;

	ExecBlockNode* node = FB_NEW(getPool()) ExecBlockNode(getPool());
	node->compiledStatement = compiledStatement;

	node->legacyParameters = PASS1_node_psql(compiledStatement, legacyParameters, false);
	node->legacyReturns = legacyReturns;

	node->localDeclList = localDeclList;
	node->body = body;

	const size_t count = node->legacyParameters ? node->legacyParameters->nod_count : 0 +
		node->legacyReturns ? node->legacyReturns->nod_count : 0 +
		node->localDeclList ? node->localDeclList->nod_count : 0;

	if (count)
	{
		StrArray names(*getDefaultMemoryPool(), count);

		PASS1_check_unique_fields_names(names, node->legacyParameters);
		PASS1_check_unique_fields_names(names, node->legacyReturns);
		PASS1_check_unique_fields_names(names, node->localDeclList);
	}

	return node;
}


void ExecBlockNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecBlockNode";
	nodes.add(legacyParameters);
	nodes.add(legacyReturns);
	nodes.add(localDeclList);
	nodes.add(body);
}


void ExecBlockNode::genBlr()
{
	// Update blockNode, because we have a reference to the original unprocessed node.
	compiledStatement->blockNode = this;

	compiledStatement->begin_debug();

	USHORT inputs = 0, outputs = 0, locals = 0;
	dsql_nod* parameters;

	// now do the input parameters
	if (parameters = legacyParameters)
	{
		USHORT position = 0;

		dsql_nod** ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ptr++)
		{
			dsql_nod* parameter = (*ptr)->nod_arg[Dsql::e_prm_val_fld];
			dsql_fld* field = (dsql_fld*) parameter->nod_arg[Dsql::e_dfl_field];
			// parameter = (*ptr)->nod_arg[Dsql::e_prm_val_val]; USELESS

			DDL_resolve_intl_type(compiledStatement, field,
				reinterpret_cast<const dsql_str*>(parameter->nod_arg[Dsql::e_dfl_collate]));

			*ptr = MAKE_variable(field, field->fld_name.c_str(), VAR_input, 0,
								 (USHORT) (2 * position), locals);
			// ASF: do not increment locals here - CORE-2341
			position++;
		}
		inputs = position;
	}

	// now do the output parameters
	if (parameters = legacyReturns)
	{
		USHORT position = 0;
		dsql_nod** ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ++ptr)
		{
			dsql_fld* field = (dsql_fld*) (*ptr)->nod_arg[Dsql::e_dfl_field];

			DDL_resolve_intl_type(compiledStatement, field,
				reinterpret_cast<const dsql_str*>((*ptr)->nod_arg[Dsql::e_dfl_collate]));

			*ptr = MAKE_variable(field, field->fld_name.c_str(), VAR_output, 1,
								 (USHORT) (2 * position), locals++);
			position++;
		}
		outputs = position;
	}

	compiledStatement->append_uchar(blr_begin);

	if (inputs) {
		compiledStatement->req_send->msg_parameters =
			revertParametersOrder(compiledStatement->req_send->msg_parameters, NULL);
		GEN_port(compiledStatement, compiledStatement->req_send);
	}
	else
		compiledStatement->req_send = NULL;

	if (outputs)
	{
		USHORT position = 0;
		parameters = legacyReturns;

		dsql_nod** ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ptr++)
		{
			dsql_par* param = MAKE_parameter(compiledStatement->req_receive, true, true, ++position, *ptr);
			param->par_node = *ptr;
			MAKE_desc(compiledStatement, &param->par_desc, *ptr, NULL);
			param->par_desc.dsc_flags |= DSC_nullable;
		}
	}

	// Set up parameter to handle EOF
	dsql_par* param = MAKE_parameter(compiledStatement->req_receive, false, false, 0, NULL);
	compiledStatement->req_eof = param;
	param->par_desc.dsc_dtype = dtype_short;
	param->par_desc.dsc_scale = 0;
	param->par_desc.dsc_length = sizeof(SSHORT);

	compiledStatement->req_receive->msg_parameters =
		revertParametersOrder(compiledStatement->req_receive->msg_parameters, NULL);
	GEN_port(compiledStatement, compiledStatement->req_receive);

	if (inputs) {
		compiledStatement->append_uchar(blr_receive);
		compiledStatement->append_uchar(0);
	}

	compiledStatement->append_uchar(blr_begin);

	if (parameters = legacyParameters)
	{
		dsql_nod** ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ptr++)
		{
			const dsql_nod* parameter = *ptr;
			const dsql_var* variable = (dsql_var*) parameter->nod_arg[Dsql::e_var_variable];
			const dsql_fld* field = variable->var_field;

			if (field->fld_full_domain || field->fld_not_nullable)
			{
				// ASF: Validation of execute block input parameters is different than procedure
				// parameters, because we can't generate messages using the domains due to the
				// connection charset influence. So to validate, we cast them and assign to null.
				compiledStatement->append_uchar(blr_assignment);
				compiledStatement->append_uchar(blr_cast);
				DDL_put_field_dtype(compiledStatement, field, true);
				compiledStatement->append_uchar(blr_parameter2);
				compiledStatement->append_uchar(0);
				compiledStatement->append_ushort(variable->var_msg_item);
				compiledStatement->append_ushort(variable->var_msg_item + 1);
				compiledStatement->append_uchar(blr_null);
			}
		}
	}

	if (outputs)
	{
		parameters = legacyReturns;
		dsql_nod** ptr = parameters->nod_arg;
		for (const dsql_nod* const* const end = ptr + parameters->nod_count; ptr < end; ptr++)
		{
			dsql_nod* parameter = *ptr;
			dsql_var* variable = (dsql_var*) parameter->nod_arg[Dsql::e_var_variable];
			DDL_put_local_variable(compiledStatement, variable, 0, NULL);
		}
	}

	compiledStatement->setPsql(true);

	DDL_put_local_variables(compiledStatement, localDeclList, locals);

	compiledStatement->req_loop_level = 0;

	dsql_nod* stmtNode = PASS1_statement(compiledStatement, body);
	GEN_hidden_variables(compiledStatement, false);

	compiledStatement->append_uchar(blr_stall);
	// Put a label before body of procedure, so that
	// any exit statement can get out
	compiledStatement->append_uchar(blr_label);
	compiledStatement->append_uchar(0);
	GEN_statement(compiledStatement, stmtNode);
	if (outputs)
		compiledStatement->req_type = REQ_SELECT_BLOCK;
	else
		compiledStatement->req_type = REQ_EXEC_BLOCK;

	compiledStatement->append_uchar(blr_end);
	GEN_return(compiledStatement, legacyReturns, true);
	compiledStatement->append_uchar(blr_end);

	compiledStatement->end_debug();
}


void ExecBlockNode::genReturn()
{
	GEN_return(compiledStatement, legacyReturns, false);
}


dsql_nod* ExecBlockNode::resolveVariable(const dsql_str* varName)
{
	// try to resolve variable name against input and output parameters and local variables

	dsql_nod* varNode;

	if (localDeclList)
	{
		if (varNode = PASS1_resolve_variable_name(localDeclList, varName))
			return varNode;
	}

	if (legacyParameters)
	{
		if (varNode = PASS1_resolve_variable_name(legacyParameters, varName))
			return varNode;
	}

	if (legacyReturns)
	{
		if (varNode = PASS1_resolve_variable_name(legacyReturns, varName))
			return varNode;
	}

	return NULL;
}


// Revert parameters order for EXECUTE BLOCK statement
dsql_par* ExecBlockNode::revertParametersOrder(dsql_par* parameter, dsql_par* prev)
{
	dsql_par* result;

	if (parameter->par_next)
		result = revertParametersOrder(parameter->par_next, parameter);
	else
		result = parameter;

	parameter->par_next = prev;

	return result;
}


//--------------------


void ExitNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ExitNode";
}


void ExitNode::genBlr()
{
	stuff(compiledStatement, blr_leave);
	stuff(compiledStatement, 0);
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
	node->compiledStatement = compiledStatement;

	node->dsqlEvent = PASS1_node(compiledStatement, dsqlEvent);
	node->dsqlArgument = PASS1_node(compiledStatement, dsqlArgument);

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
		stuff(compiledStatement, blr_post_arg);
		GEN_expr(compiledStatement, dsqlEvent);
		GEN_expr(compiledStatement, dsqlArgument);
	}
	else
	{
		stuff(compiledStatement, blr_post);
		GEN_expr(compiledStatement, dsqlEvent);
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


jrd_nod* PostEventNode::execute(thread_db* tdbb, jrd_req* request)
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


DmlNode* SavepointNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	SavepointNode* node = FB_NEW(pool) SavepointNode(pool);

	node->command = (Command) csb->csb_blr_reader.getByte();
	PAR_name(csb, node->name);

	return node;
}


SavepointNode* SavepointNode::internalDsqlPass()
{
	// ASF: It should never enter in this IF, because the grammar does not allow it.
	if (compiledStatement->req_flags & REQ_block) // blocks, procedures and triggers
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
			Arg::Gds(isc_random) <<
			Arg::Str(cmd));
	}

	compiledStatement->req_type = REQ_SAVEPOINT;

	return this;
}


void SavepointNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SavepointNode";
}


void SavepointNode::genBlr()
{
	stuff(compiledStatement, blr_user_savepoint);
	stuff(compiledStatement, (UCHAR) command);
	stuff_cstring(compiledStatement, name.c_str());
}


SavepointNode* SavepointNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	return this;
}


SavepointNode* SavepointNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	return this;
}


jrd_nod* SavepointNode::execute(thread_db* tdbb, jrd_req* request)
{
	Database* dbb = request->req_attachment->att_database;
	jrd_tra* transaction = request->req_transaction;

	if (request->req_operation == jrd_req::req_evaluate && transaction != dbb->dbb_sys_trans)
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


DmlNode* SuspendNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	SuspendNode* node = FB_NEW(pool) SuspendNode(pool);

	USHORT n = csb->csb_blr_reader.getByte();
	node->message = csb->csb_rpt[n].csb_message;
	node->statement = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


SuspendNode* SuspendNode::internalDsqlPass()
{
	if (compiledStatement->req_flags & REQ_trigger)	// triggers only
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown
				  Arg::Gds(isc_token_err) <<
				  Arg::Gds(isc_random) << Arg::Str("SUSPEND"));
	}

	if (compiledStatement->req_flags & REQ_in_auto_trans_block)	// autonomous transaction
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_unsupported_in_auto_trans) << Arg::Str("SUSPEND"));
	}

	compiledStatement->req_flags |= REQ_selectable;

	blockNode = compiledStatement->blockNode;

	return this;
}


void SuspendNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SuspendNode";
}


void SuspendNode::genBlr()
{
	if (blockNode)
		blockNode->genReturn();
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
jrd_nod* SuspendNode::execute(thread_db* tdbb, jrd_req* request)
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


}	// namespace Jrd
