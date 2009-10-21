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
#include "../jrd/exe_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/tra_proto.h"
#include "../dsql/ddl_proto.h"
#include "../jrd/vio_proto.h"
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


static RegisterNode<InAutonomousTransactionNode> regInAutonomousTransactionNode(blr_auto_trans);


DmlNode* InAutonomousTransactionNode::parse(thread_db* tdbb, MemoryPool& pool,
		CompilerScratch* csb)
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
	text = "in autonomous transaction";
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
	text = "execute block";
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

	SSHORT inputs = 0, outputs = 0, locals = 0;
	dsql_nod* parameters;

	// now do the input parameters
	if (parameters = legacyParameters)
	{
		SSHORT position = 0;

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
		SSHORT position = 0;
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
		SSHORT	position = 0;
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


ExecBlockNode* ExecBlockNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(false);
	return this;
}


ExecBlockNode* ExecBlockNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(false);
	return this;
}


jrd_nod* ExecBlockNode::execute(thread_db* tdbb, jrd_req* request)
{
	fb_assert(false);
	return NULL;
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


}	// namespace Jrd
