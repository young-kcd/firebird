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
#include "../common/common.h"
#include "../common/classes/BaseStream.h"
#include "../common/classes/MsgPrint.h"
#include "../common/classes/VaryStr.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/StmtNodes.h"
#include "../dsql/node.h"
#include "../jrd/blr.h"
#include "../jrd/tra.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/VirtualTable.h"
#include "../jrd/extds/ExtDS.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/recsrc/Cursor.h"
#include "../jrd/trace/TraceManager.h"
#include "../jrd/trace/TraceJrdHelpers.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dfw_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/idx_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/tra_proto.h"
#include "../dsql/ddl_proto.h"
#include "../jrd/vio_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/pass1_proto.h"

using namespace Firebird;
using namespace Jrd;


namespace
{
	// Node copier that remaps the field id 0 of stream 0 to a given field id.
	class RemapFieldNodeCopier : public NodeCopier
	{
	public:
		RemapFieldNodeCopier(CompilerScratch* aCsb, UCHAR* aRemap, USHORT aFldId)
			: NodeCopier(aCsb, aRemap),
			  fldId(aFldId)
		{
		}

	protected:
		virtual USHORT getFieldId(FieldNode* field)
		{
			if (field->byId && field->fieldId == 0 && field->fieldStream == 0)
				return fldId;

			return NodeCopier::getFieldId(field);
		}

	private:
		USHORT fldId;
	};
}


namespace Jrd {


static void cleanupRpb(thread_db* tdbb, record_param* rpb);
static void makeValidation(thread_db* tdbb, CompilerScratch* csb, USHORT stream,
	Array<ValidateInfo>& validations);
static StmtNode* pass1ExpandView(thread_db* tdbb, CompilerScratch* csb, USHORT orgStream,
	USHORT newStream, bool remap);
static RelationSourceNode* pass1Update(thread_db* tdbb, CompilerScratch* csb, jrd_rel* relation,
	const trig_vec* trigger, USHORT stream, USHORT updateStream, SecurityClass::flags_t priv,
	jrd_rel* view, USHORT viewStream);
static void pass1Validations(thread_db* tdbb, CompilerScratch* csb, Array<ValidateInfo>& validations);
static void postTriggerAccess(CompilerScratch* csb, jrd_rel* ownerRelation,
	ExternalAccess::exa_act operation, jrd_rel* view);
static void preModifyEraseTriggers(thread_db* tdbb, trig_vec** trigs,
	StmtNode::WhichTrigger whichTrig, record_param* rpb, record_param* rec, jrd_req::req_ta op);
static void validateExpressions(thread_db* tdbb, const Array<ValidateInfo>& validations);


//--------------------


StmtNode* StmtNode::fromLegacy(const dsql_nod* node)
{
	return node && node->nod_type == Dsql::nod_class_stmtnode ?
		reinterpret_cast<StmtNode*>(node->nod_arg[0]) : NULL;
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

void SavepointEncloseNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_begin);
	dsqlScratch->appendUChar(blr_start_savepoint);
	stmt->genBlr(dsqlScratch);
	dsqlScratch->appendUChar(blr_end_savepoint);
	dsqlScratch->appendUChar(blr_end);
}


//--------------------


static RegisterNode<AssignmentNode> regAssignmentNode(blr_assignment);

DmlNode* AssignmentNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	AssignmentNode* node = FB_NEW(pool) AssignmentNode(pool);
	node->asgnFrom = PAR_parse_value(tdbb, csb);
	node->asgnTo = PAR_parse_value(tdbb, csb);
	return node;
}

AssignmentNode* AssignmentNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void AssignmentNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "AssignmentNode";
}

void AssignmentNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

AssignmentNode* AssignmentNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	AssignmentNode* node = FB_NEW(*tdbb->getDefaultPool()) AssignmentNode(*tdbb->getDefaultPool());
	node->asgnFrom = copier.copy(tdbb, asgnFrom);
	node->asgnTo = copier.copy(tdbb, asgnTo);
	node->missing = copier.copy(tdbb, missing);
	node->missing2 = copier.copy(tdbb, missing2);
	return node;
}

AssignmentNode* AssignmentNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode* sub = asgnFrom;
	FieldNode* fieldNode;
	USHORT stream;
	CompilerScratch::csb_repeat* tail;

	if ((fieldNode = sub->as<FieldNode>()))
	{
		stream = fieldNode->fieldStream;
		jrd_fld* field = MET_get_field(csb->csb_rpt[stream].csb_relation, fieldNode->fieldId);

		if (field)
			missing2 = field->fld_missing_value;
	}

	sub = asgnTo;

	if ((fieldNode = sub->as<FieldNode>()))
	{
		stream = fieldNode->fieldStream;
		tail = &csb->csb_rpt[stream];
		jrd_fld* field = MET_get_field(tail->csb_relation, fieldNode->fieldId);

		if (field && field->fld_missing_value)
			missing = field->fld_missing_value;
	}

	doPass1(tdbb, csb, asgnFrom.getAddress());
	doPass1(tdbb, csb, asgnTo.getAddress());
	doPass1(tdbb, csb, missing.getAddress());
	// ASF: No idea why we do not call pass1 for missing2.

	// Perform any post-processing here.

	sub = asgnTo;

	if ((fieldNode = sub->as<FieldNode>()))
	{
		stream = fieldNode->fieldStream;
		tail = &csb->csb_rpt[stream];

		// Assignments to the OLD context are prohibited for all trigger types.
		if ((tail->csb_flags & csb_trigger) && stream == 0)
			ERR_post(Arg::Gds(isc_read_only_field));

		// Assignments to the NEW context are prohibited for post-action triggers.
		if ((tail->csb_flags & csb_trigger) && stream == 1 &&
			(csb->csb_g_flags & csb_post_trigger))
		{
			ERR_post(Arg::Gds(isc_read_only_field));
		}
	}
	else if (!(sub->is<ParameterNode>() || sub->is<VariableNode>() || sub->is<NullNode>()))
		ERR_post(Arg::Gds(isc_read_only_field));

	return this;
}

AssignmentNode* AssignmentNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, asgnFrom.getAddress());
	ExprNode::doPass2(tdbb, csb, asgnTo.getAddress());
	ExprNode::doPass2(tdbb, csb, missing.getAddress());
	ExprNode::doPass2(tdbb, csb, missing2.getAddress());
	return this;
}

const StmtNode* AssignmentNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
		EXE_assignment(tdbb, this);

	return parentStmt;
}


//--------------------


static RegisterNode<BlockNode> regBlockNode(blr_block);

DmlNode* BlockNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	BlockNode* node = FB_NEW(pool) BlockNode(pool);
	node->action = PAR_parse_stmt(tdbb, csb);

	StmtNodeStack stack;

	while (csb->csb_blr_reader.peekByte() != blr_end)
		stack.push(PAR_parse_stmt(tdbb, csb));

	csb->csb_blr_reader.getByte();	// skip blr_end

	node->handlers = PAR_make_list(tdbb, stack);

	return node;
}

BlockNode* BlockNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void BlockNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "BlockNode";
}

void BlockNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

BlockNode* BlockNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, action.getAddress());
	doPass1(tdbb, csb, handlers.getAddress());
	return this;
}

BlockNode* BlockNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, action.getAddress(), this);
	doPass2(tdbb, csb, handlers.getAddress(), this);

	impureOffset = CMP_impure(csb, sizeof(SLONG));

	return this;
}

const StmtNode* BlockNode::execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const
{
	jrd_tra* transaction = request->req_transaction;
	jrd_tra* sysTransaction = request->req_attachment->getSysTransaction();
	SLONG count;

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			if (transaction != sysTransaction)
			{
				VIO_start_save_point(tdbb, transaction);
				const Savepoint* save_point = transaction->tra_save_point;
				count = save_point->sav_number;
				*request->getImpure<SLONG>(impureOffset) = count;
			}
			return action;

		case jrd_req::req_unwind:
		{
			if (request->req_flags & (req_leave | req_continue_loop))
			{
				// Although the req_operation is set to req_unwind,
				// it's not an error case if req_leave/req_continue_loop bit is set.
				// req_leave/req_continue_loop bit indicates that we hit an EXIT or
				// BREAK/LEAVE/CONTINUE statement in the SP/trigger code.
				// Do not perform the error handling stuff.

				if (transaction != sysTransaction)
				{
					count = *request->getImpure<SLONG>(impureOffset);

					for (const Savepoint* save_point = transaction->tra_save_point;
						 save_point && count <= save_point->sav_number;
						 save_point = transaction->tra_save_point)
					{
						EXE_verb_cleanup(tdbb, transaction);
					}
				}

				return parentStmt;
			}

			if (transaction != sysTransaction)
			{
				count = *request->getImpure<SLONG>(impureOffset);

				// Since there occurred an error (req_unwind), undo all savepoints
				// up to, but not including, the savepoint of this block.  The
				// savepoint of this block will be dealt with below.

				for (const Savepoint* save_point = transaction->tra_save_point;
					 save_point && count < save_point->sav_number;
					 save_point = transaction->tra_save_point)
				{
					++transaction->tra_save_point->sav_verb_count;
					EXE_verb_cleanup(tdbb, transaction);
				}
			}

			const StmtNode* temp;

			if (handlers)
			{
				temp = parentStmt;
				const NestConst<StmtNode>* ptr = handlers->statements.begin();

				for (const NestConst<StmtNode>* const end = handlers->statements.end();
					 ptr != end;
					 ++ptr)
				{
					const ErrorHandlerNode* handlerNode = (*ptr)->as<ErrorHandlerNode>();
					const PsqlException* xcpNode = handlerNode->conditions;

					if (testAndFixupError(tdbb, request, xcpNode))
					{
						request->req_operation = jrd_req::req_evaluate;
						temp = handlerNode->action;
						exeState->errorPending = false;

						// On entering looper exeState->oldRequest etc. are saved.
						// On recursive calling we will loose the actual old
						// request for that invocation of looper. Avoid this.

						{
							Jrd::ContextPoolHolder contextLooper(tdbb, exeState->oldPool);
							tdbb->setRequest(exeState->oldRequest);
							fb_assert(request->req_caller == exeState->oldRequest);
							request->req_caller = NULL;

							// Save the previous state of req_error_handler
							// bit. We need to restore it later. This is
							// necessary if the error handler is deeply nested.

							const ULONG prev_req_error_handler =
								request->req_flags & req_error_handler;
							request->req_flags |= req_error_handler;
							temp = EXE_looper(tdbb, request, temp);
							request->req_flags &= ~req_error_handler;
							request->req_flags |= prev_req_error_handler;

							// Note: Previously the above call "temp = looper (tdbb, request, temp);"
							// never returned back till the tree was executed completely. Now that
							// the looper has changed its behaviour such that it returns back after
							// handling error. This makes it necessary that the jmpbuf be reset
							// so that looper can proceede with the processing of execution tree.
							// If this is not done then anymore errors will take the engine out of
							// looper there by abruptly terminating the processing.

							exeState->catchDisabled = false;
							tdbb->setRequest(request);
							fb_assert(request->req_caller == NULL);
							request->req_caller = exeState->oldRequest;
						}

						// The error is dealt with by the application, cleanup
						// this block's savepoint.

						if (transaction != sysTransaction)
						{
							for (const Savepoint* save_point = transaction->tra_save_point;
								 save_point && count <= save_point->sav_number;
								 save_point = transaction->tra_save_point)
							{
								EXE_verb_cleanup(tdbb, transaction);
							}
						}
					}
				}
			}
			else
				temp = parentStmt;

			// If the application didn't have an error handler, then
			// the error will still be pending.  Undo the block by
			// using its savepoint.

			if (exeState->errorPending && transaction != sysTransaction)
			{
				for (const Savepoint* save_point = transaction->tra_save_point;
					 save_point && count <= save_point->sav_number;
					 save_point = transaction->tra_save_point)
				{
					++transaction->tra_save_point->sav_verb_count;
					EXE_verb_cleanup(tdbb, transaction);
				}
			}

			return temp;
		}

		case jrd_req::req_return:
			if (transaction != sysTransaction)
			{
				count = *request->getImpure<SLONG>(impureOffset);

				for (const Savepoint* save_point = transaction->tra_save_point;
					 save_point && count <= save_point->sav_number;
					 save_point = transaction->tra_save_point)
				{
					EXE_verb_cleanup(tdbb, transaction);
				}
			}

		default:
			return parentStmt;
	}

	fb_assert(false);
	return NULL;
}

// Test for match of current state with list of error conditions. Fix type and code of the exception.
bool BlockNode::testAndFixupError(thread_db* tdbb, jrd_req* request, const PsqlException* conditions)
{
	if (tdbb->tdbb_flags & TDBB_sys_error)
		return false;

	ISC_STATUS* statusVector = tdbb->tdbb_status_vector;
	const SSHORT sqlcode = gds__sqlcode(statusVector);

	bool found = false;

	for (USHORT i = 0; i < conditions->xcp_count; i++)
	{
		switch (conditions->xcp_rpt[i].xcp_type)
		{
			case xcp_sql_code:
				if (sqlcode == conditions->xcp_rpt[i].xcp_code)
					found = true;
				break;

			case xcp_gds_code:
				if (statusVector[1] == conditions->xcp_rpt[i].xcp_code)
					found = true;
				break;

			case xcp_xcp_code:
				// Look at set_error() routine to understand how the
				// exception ID info is encoded inside the status vector.
				if ((statusVector[1] == isc_except) &&
					(statusVector[3] == conditions->xcp_rpt[i].xcp_code))
				{
					found = true;
				}

				break;

			case xcp_default:
				found = true;
				break;

			default:
				fb_assert(false);
		}

		if (found)
		{
			request->req_last_xcp.init(statusVector);
			fb_utils::init_status(statusVector);
			break;
		}
    }

	return found;
}


//--------------------


static RegisterNode<CompoundStmtNode> regCompoundStmtNode(blr_begin);

DmlNode* CompoundStmtNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	CompoundStmtNode* node = FB_NEW(pool) CompoundStmtNode(pool);

	while (csb->csb_blr_reader.peekByte() != blr_end)
		node->statements.add(PAR_parse_stmt(tdbb, csb));

	csb->csb_blr_reader.getByte();	// skip blr_end

	return node;
}

CompoundStmtNode* CompoundStmtNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void CompoundStmtNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "CompoundStmtNode";
}

void CompoundStmtNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

CompoundStmtNode* CompoundStmtNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	CompoundStmtNode* node = FB_NEW(*tdbb->getDefaultPool()) CompoundStmtNode(*tdbb->getDefaultPool());
	node->onlyAssignments = onlyAssignments;

	NestConst<StmtNode>* j = node->statements.getBuffer(statements.getCount());

	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i, ++j)
		*j = copier.copy(tdbb, *i);

	return node;
}

CompoundStmtNode* CompoundStmtNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
		doPass1(tdbb, csb, i->getAddress());
	return this;
}

CompoundStmtNode* CompoundStmtNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
		doPass2(tdbb, csb, i->getAddress(), this);

	impureOffset = CMP_impure(csb, sizeof(impure_state));

	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
	{
		if (!StmtNode::is<AssignmentNode>(i->getObject()))
			return this;
	}

	onlyAssignments = true;

	return this;
}

const StmtNode* CompoundStmtNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	const NestConst<StmtNode>* end = statements.end();

	if (onlyAssignments)
	{
		if (request->req_operation == jrd_req::req_evaluate)
		{
			for (const NestConst<StmtNode>* i = statements.begin(); i != end; ++i)
			{
				const StmtNode* stmt = i->getObject();

				if (stmt->hasLineColumn)
				{
					request->req_src_line = stmt->line;
					request->req_src_column = stmt->column;
				}

				EXE_assignment(tdbb, static_cast<const AssignmentNode*>(stmt));
			}

			request->req_operation = jrd_req::req_return;
		}

		return parentStmt;
	}

	impure_state* impure = request->getImpure<impure_state>(impureOffset);

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			impure->sta_state = 0;
			// fall into

		case jrd_req::req_return:
		case jrd_req::req_sync:
			if (impure->sta_state < int(statements.getCount()))
			{
				request->req_operation = jrd_req::req_evaluate;
				return statements[impure->sta_state++];
			}
			request->req_operation = jrd_req::req_return;
			// fall into

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<ContinueLeaveNode> regContinueLeaveNodeContinue(blr_continue_loop);
static RegisterNode<ContinueLeaveNode> regContinueLeaveNodeLeave(blr_leave);

DmlNode* ContinueLeaveNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	ContinueLeaveNode* node = FB_NEW(pool) ContinueLeaveNode(pool, blrOp);
	node->labelNumber = csb->csb_blr_reader.getByte();
	return node;
}

ContinueLeaveNode* ContinueLeaveNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ContinueLeaveNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ContinueLeaveNode";
}

void ContinueLeaveNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

const StmtNode* ContinueLeaveNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	request->req_operation = jrd_req::req_unwind;
	request->req_label = labelNumber;
	request->req_flags |= (blrOp == blr_continue_loop ? req_continue_loop : req_leave);
	return parentStmt;
}


//--------------------


static RegisterNode<CursorStmtNode> regCursorStmtNode(blr_cursor_stmt);

DmlNode* CursorStmtNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	CursorStmtNode* node = FB_NEW(pool) CursorStmtNode(pool);

	node->cursorOp = csb->csb_blr_reader.getByte();
	node->cursorNumber = csb->csb_blr_reader.getWord();

	switch (node->cursorOp)
	{
		case blr_cursor_open:
		case blr_cursor_close:
			break;

		case blr_cursor_fetch_scroll:
			node->scrollOp = csb->csb_blr_reader.getByte();
			node->scrollExpr = PAR_parse_value(tdbb, csb);
			// fall into

		case blr_cursor_fetch:
			csb->csb_g_flags |= csb_reuse_context;
			node->intoStmt = PAR_parse_stmt(tdbb, csb);
			csb->csb_g_flags &= ~csb_reuse_context;
			break;

		default:
			PAR_syntax_error(csb, "cursor operation clause");
	}

	return node;
}

CursorStmtNode* CursorStmtNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void CursorStmtNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "CursorStmtNode";
}

void CursorStmtNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

CursorStmtNode* CursorStmtNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, scrollExpr.getAddress());
	doPass1(tdbb, csb, intoStmt.getAddress());
	return this;
}

CursorStmtNode* CursorStmtNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, scrollExpr.getAddress());
	doPass2(tdbb, csb, intoStmt.getAddress(), this);
	return this;
}

const StmtNode* CursorStmtNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	fb_assert(cursorNumber < request->req_cursors.getCount());
	const Cursor* const cursor = request->req_cursors[cursorNumber];
	bool fetched = false;

	switch (cursorOp)
	{
		case blr_cursor_open:
			if (request->req_operation == jrd_req::req_evaluate)
			{
				cursor->open(tdbb);
				request->req_operation = jrd_req::req_return;
			}
			return parentStmt;

		case blr_cursor_close:
			if (request->req_operation == jrd_req::req_evaluate)
			{
				cursor->close(tdbb);
				request->req_operation = jrd_req::req_return;
			}
			return parentStmt;

		case blr_cursor_fetch:
		case blr_cursor_fetch_scroll:
			switch (request->req_operation)
			{
				case jrd_req::req_evaluate:
					request->req_records_affected.clear();

					if (cursorOp == blr_cursor_fetch)
						fetched = cursor->fetchNext(tdbb);
					else
					{
						fb_assert(cursorOp == blr_cursor_fetch_scroll);

						const dsc* desc = EVL_expr(tdbb, request, scrollExpr);
						const bool unknown = !desc || (request->req_flags & req_null);
						const SINT64 offset = unknown ? 0 : MOV_get_int64(desc, 0);

						switch (scrollOp)
						{
							case blr_scroll_forward:
								fetched = cursor->fetchNext(tdbb);
								break;
							case blr_scroll_backward:
								fetched = cursor->fetchPrior(tdbb);
								break;
							case blr_scroll_bof:
								fetched = cursor->fetchFirst(tdbb);
								break;
							case blr_scroll_eof:
								fetched = cursor->fetchLast(tdbb);
								break;
							case blr_scroll_absolute:
								fetched = unknown ? false : cursor->fetchAbsolute(tdbb, offset);
								break;
							case blr_scroll_relative:
								fetched = unknown ? false : cursor->fetchRelative(tdbb, offset);
								break;
							default:
								fb_assert(false);
								fetched = false;
						}
					}

					if (fetched)
					{
						request->req_operation = jrd_req::req_evaluate;
						return intoStmt;
					}

					request->req_operation = jrd_req::req_return;

				default:
					return parentStmt;
			}
			break;
	}

	fb_assert(false);
	return NULL;
}


//--------------------


static RegisterNode<DeclareCursorNode> regDeclareCursorNode(blr_dcl_cursor);

DmlNode* DeclareCursorNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	DeclareCursorNode* node = FB_NEW(pool) DeclareCursorNode(pool);

	node->cursorNumber = csb->csb_blr_reader.getWord();
	node->rse = PAR_rse(tdbb, csb);

	USHORT count = csb->csb_blr_reader.getWord();
	node->refs = PAR_args(tdbb, csb, count, count);

	return node;
}

DeclareCursorNode* DeclareCursorNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void DeclareCursorNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "DeclareCursorNode";
}

void DeclareCursorNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

DeclareCursorNode* DeclareCursorNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, rse.getAddress());
	doPass1(tdbb, csb, refs.getAddress());
	return this;
}

DeclareCursorNode* DeclareCursorNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	rse->pass2Rse(tdbb, csb);

	ExprNode::doPass2(tdbb, csb, rse.getAddress());
	ExprNode::doPass2(tdbb, csb, refs.getAddress());

	// Finish up processing of record selection expressions.

	RecordSource* const rsb = CMP_post_rse(tdbb, csb, rse.getObject());
	csb->csb_fors.add(rsb);

	cursor = FB_NEW(*tdbb->getDefaultPool()) Cursor(csb, rsb, rse->rse_invariants,
		(rse->flags & RseNode::FLAG_SCROLLABLE));

	return this;
}

const StmtNode* DeclareCursorNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		// Set up the cursors array...
		if (cursorNumber >= request->req_cursors.getCount())
			request->req_cursors.grow(cursorNumber + 1);

		// And store cursor there.
		request->req_cursors[cursorNumber] = cursor;
		request->req_operation = jrd_req::req_return;
	}

	return parentStmt;
}


//--------------------


static RegisterNode<DeclareVariableNode> regDeclareVariableNode(blr_dcl_variable);

DmlNode* DeclareVariableNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	DeclareVariableNode* node = FB_NEW(pool) DeclareVariableNode(pool);

	node->varId = csb->csb_blr_reader.getWord();

	ItemInfo itemInfo;
	PAR_desc(tdbb, csb, &node->varDesc, &itemInfo);

	csb->csb_variables = vec<DeclareVariableNode*>::newVector(
		*tdbb->getDefaultPool(), csb->csb_variables, node->varId + 1);

	if (itemInfo.isSpecial())
	{
		csb->csb_dbg_info.varIndexToName.get(node->varId, itemInfo.name);
		csb->csb_map_item_info.put(Item(Item::TYPE_VARIABLE, node->varId), itemInfo);
	}

	if (itemInfo.explicitCollation)
	{
		CompilerScratch::Dependency dependency(obj_collation);
		dependency.number = INTL_TEXT_TYPE(node->varDesc);
		csb->csb_dependencies.push(dependency);
	}

	return node;
}

DeclareVariableNode* DeclareVariableNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void DeclareVariableNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "DeclareVariableNode";
}

void DeclareVariableNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

DeclareVariableNode* DeclareVariableNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (copier.csb->csb_remap_variable == 0)
		return this;

	DeclareVariableNode* node = FB_NEW(*tdbb->getDefaultPool()) DeclareVariableNode(*tdbb->getDefaultPool());
	node->varId = varId + copier.csb->csb_remap_variable;
	node->varDesc = varDesc;

	copier.csb->csb_variables = vec<DeclareVariableNode*>::newVector(*tdbb->getDefaultPool(),
		copier.csb->csb_variables, node->varId + 1);

	return node;
}

DeclareVariableNode* DeclareVariableNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	vec<DeclareVariableNode*>* vector = csb->csb_variables = vec<DeclareVariableNode*>::newVector(
		*tdbb->getDefaultPool(), csb->csb_variables, varId + 1);
	fb_assert(!(*vector)[varId]);
	(*vector)[varId] = this;

	return this;
}

DeclareVariableNode* DeclareVariableNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	impureOffset = CMP_impure(csb, sizeof(impure_value));
	return this;
}

const StmtNode* DeclareVariableNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	impure_value* variable = request->getImpure<impure_value>(impureOffset);
	variable->vlu_desc = varDesc;
	variable->vlu_desc.dsc_flags = 0;

	if (variable->vlu_desc.dsc_dtype <= dtype_varying)
	{
		if (!variable->vlu_string)
		{
			const USHORT len = variable->vlu_desc.dsc_length;
			variable->vlu_string = FB_NEW_RPT(*tdbb->getDefaultPool(), len) VaryingString();
			variable->vlu_string->str_length = len;
		}

		variable->vlu_desc.dsc_address = variable->vlu_string->str_data;
	}
	else
		variable->vlu_desc.dsc_address = (UCHAR*) &variable->vlu_misc;

	request->req_operation = jrd_req::req_return;

	return parentStmt;
}


//--------------------


static RegisterNode<EraseNode> regEraseNode(blr_erase);

DmlNode* EraseNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	USHORT n = csb->csb_blr_reader.getByte();

	if (n >= csb->csb_rpt.getCount() || !(csb->csb_rpt[n].csb_flags & csb_used))
		PAR_error(csb, Arg::Gds(isc_ctxnotdef));

	EraseNode* node = FB_NEW(pool) EraseNode(pool);
	node->stream = csb->csb_rpt[n].csb_stream;

	return node;
}

EraseNode* EraseNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void EraseNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "EraseNode";
}

void EraseNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

EraseNode* EraseNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	pass1Erase(tdbb, csb, this);

	doPass1(tdbb, csb, statement.getAddress());
	doPass1(tdbb, csb, subStatement.getAddress());

	return this;
}

// Checkout an erase statement. If it references a view, and is kosher, fix it up.
void EraseNode::pass1Erase(thread_db* tdbb, CompilerScratch* csb, EraseNode* node)
{
	// If updateable views with triggers are involved, there maybe a recursive call to be ignored.

	if (node->subStatement)
		return;

	// To support nested views, loop until we hit a table or a view with user-defined triggers
	// (which means no update).

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parentStream = 0;

	for (;;)
	{
		USHORT newStream = node->stream;
		const USHORT stream = newStream;

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_erase;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = relation->rel_view_rse ? relation : view;

		if (!parent)
			parent = tail->csb_view;

		postTriggerAccess(csb, relation, ExternalAccess::exa_delete, view);

		// Check out delete. If this is a delete thru a view, verify the view by checking for read
		// access on the base table. If field-level select privileges are implemented, this needs
		// to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_delete;

		if (parent)
			priv |= SCL_read;

		const trig_vec* trigger = relation->rel_pre_erase ?
			relation->rel_pre_erase : relation->rel_post_erase;

		// If we have a view with triggers, let's expand it.

		if (relation->rel_view_rse && trigger)
		{
			newStream = csb->nextStream();
			node->stream = newStream;
			CMP_csb_element(csb, newStream)->csb_relation = relation;

			node->statement = pass1ExpandView(tdbb, csb, stream, newStream, false);
		}

		// Get the source relation, either a table or yet another view.

		RelationSourceNode* source = pass1Update(tdbb, csb, relation, trigger, stream,
			newStream, priv, parent, parentStream);

		if (!source)
			return;	// no source means we're done

		parent = relation;
		parentStream = stream;

		// Remap the source stream.

		UCHAR* map = csb->csb_rpt[stream].csb_map;

		if (trigger)
		{
			// ASF: This code is responsible to make view's WITH CHECK OPTION to work as constraints.
			// I don't see how it could run for delete statements under normal conditions.

			// Set up the new target stream.

			EraseNode* viewNode = FB_NEW(*tdbb->getDefaultPool()) EraseNode(*tdbb->getDefaultPool());
			viewNode->stream = node->stream;

			node->subStatement = viewNode;

			// Substitute the original delete node with the newly created one.
			node = viewNode;
		}
		else
		{
			// This relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view).
			csb->csb_rpt[newStream].csb_flags &= ~csb_view_update;
		}

		// Let's reset the target stream.
		newStream = source->getStream();
		node->stream = map[newStream];
	}
}

EraseNode* EraseNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	doPass2(tdbb, csb, subStatement.getAddress(), this);

	impureOffset = CMP_impure(csb, sizeof(SLONG));
	csb->csb_rpt[stream].csb_flags |= csb_update;

	return this;
}

const StmtNode* EraseNode::execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const
{
	const StmtNode* retNode;

	if (request->req_operation == jrd_req::req_unwind)
		retNode = parentStmt;
	else if (request->req_operation == jrd_req::req_return && subStatement)
	{
		if (!exeState->topNode)
		{
			exeState->topNode = this;
			exeState->whichEraseTrig = PRE_TRIG;
		}

		exeState->prevNode = this;
		retNode = erase(tdbb, request, exeState->whichEraseTrig);

		if (exeState->whichEraseTrig == PRE_TRIG)
		{
			retNode = subStatement;
			fb_assert(retNode->parentStmt == this);
			///retNode->parentStmt = exeState->prevNode;
		}

		if (exeState->topNode == this && exeState->whichEraseTrig == POST_TRIG)
		{
			exeState->topNode = NULL;
			exeState->whichEraseTrig = ALL_TRIGS;
		}
		else
			request->req_operation = jrd_req::req_evaluate;
	}
	else
	{
		exeState->prevNode = this;
		retNode = erase(tdbb, request, ALL_TRIGS);

		if (!subStatement && exeState->whichEraseTrig == PRE_TRIG)
			exeState->whichEraseTrig = POST_TRIG;
	}

	return retNode;
}

// Perform erase operation.
const StmtNode* EraseNode::erase(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const
{
	Jrd::Attachment* attachment = tdbb->getAttachment();
	jrd_tra* transaction = request->req_transaction;
	record_param* rpb = &request->req_rpb[stream];
	jrd_rel* relation = rpb->rpb_relation;

	if (rpb->rpb_number.isBof() || (!relation->rel_view_rse && !rpb->rpb_number.isValid()))
		ERR_post(Arg::Gds(isc_no_cur_rec));

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
		{
			request->req_records_affected.bumpModified(false);

			if (!statement)
				break;

			const Format* format = MET_current(tdbb, rpb->rpb_relation);
			Record* record = VIO_record(tdbb, rpb, format, tdbb->getDefaultPool());

			rpb->rpb_address = record->rec_data;
			rpb->rpb_length = format->fmt_length;
			rpb->rpb_format_number = format->fmt_version;

			return statement;
		}

		case jrd_req::req_return:
			break;

		default:
			return parentStmt;
	}

	request->req_operation = jrd_req::req_return;
	RLCK_reserve_relation(tdbb, transaction, relation, true);

	// If the stream was sorted, the various fields in the rpb are probably junk.
	// Just to make sure that everything is cool, refetch and release the record.

	if (rpb->rpb_stream_flags & RPB_s_refetch)
	{
		VIO_refetch_record(tdbb, rpb, transaction);
		rpb->rpb_stream_flags &= ~RPB_s_refetch;
	}

	if (transaction != attachment->getSysTransaction())
		++transaction->tra_save_point->sav_verb_count;

	// Handle pre-operation trigger.
	preModifyEraseTriggers(tdbb, &relation->rel_pre_erase, whichTrig, rpb, NULL,
		jrd_req::req_trigger_delete);

	if (relation->rel_file)
		EXT_erase(rpb, transaction);
	else if (relation->isVirtual())
		VirtualTable::erase(tdbb, rpb);
	else if (!relation->rel_view_rse)
		VIO_erase(tdbb, rpb, transaction);

	// Handle post operation trigger.
	if (relation->rel_post_erase && whichTrig != PRE_TRIG)
	{
		EXE_execute_triggers(tdbb, &relation->rel_post_erase, rpb, NULL,
			jrd_req::req_trigger_delete, POST_TRIG);
	}

	// Call IDX_erase (which checks constraints) after all post erase triggers have fired.
	// This is required for cascading referential integrity, which can be implemented as
	// post_erase triggers.

	if (!relation->rel_file && !relation->rel_view_rse && !relation->isVirtual())
	{
		jrd_rel* badRelation = NULL;
		USHORT badIndex;

		const idx_e errorCode = IDX_erase(tdbb, rpb, transaction, &badRelation, &badIndex);

		if (errorCode)
			ERR_duplicate_error(errorCode, badRelation, badIndex);
	}

	// CVC: Increment the counter only if we called VIO/EXT_erase() and we were successful.
	if (!(request->req_view_flags & req_first_erase_return))
	{
		request->req_view_flags |= req_first_erase_return;
		if (relation->rel_view_rse)
			request->req_top_view_erase = relation;
	}

	if (relation == request->req_top_view_erase)
	{
		if (whichTrig == ALL_TRIGS || whichTrig == POST_TRIG)
		{
			request->req_records_deleted++;
			request->req_records_affected.bumpModified(true);
		}
	}
	else if (relation->rel_file || !relation->rel_view_rse)
	{
		request->req_records_deleted++;
		request->req_records_affected.bumpModified(true);
	}

	if (transaction != attachment->getSysTransaction())
		--transaction->tra_save_point->sav_verb_count;

	rpb->rpb_number.setValid(false);

	return parentStmt;
}


//--------------------


static RegisterNode<ErrorHandlerNode> regErrorHandlerNode(blr_error_handler);

DmlNode* ErrorHandlerNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	ErrorHandlerNode* node = FB_NEW(pool) ErrorHandlerNode(pool);

	const USHORT n = csb->csb_blr_reader.getWord();
	node->conditions = FB_NEW_RPT(*tdbb->getDefaultPool(), n) PsqlException();
	node->conditions->xcp_count = n;

	for (unsigned i = 0; i < n; i++)
	{
		const USHORT codeType = csb->csb_blr_reader.getByte();
		xcp_repeat& item = node->conditions->xcp_rpt[i];

		switch (codeType)
		{
			case blr_sql_code:
				item.xcp_type = xcp_sql_code;
				item.xcp_code = (SSHORT) csb->csb_blr_reader.getWord();
				break;

			case blr_gds_code:
			{
				string name;
				item.xcp_type = xcp_gds_code;
				PAR_name(csb, name);
				name.lower();
				SLONG codeNumber = PAR_symbol_to_gdscode(name);
				if (codeNumber)
					item.xcp_code = codeNumber;
				else
					PAR_error(csb, Arg::Gds(isc_codnotdef) << Arg::Str(name));
				break;
			}

			case blr_exception:
			{
				MetaName name;
				item.xcp_type = xcp_xcp_code;
				PAR_name(csb, name);
				if (!(item.xcp_code = MET_lookup_exception_number(tdbb, name)))
					PAR_error(csb, Arg::Gds(isc_xcpnotdef) << Arg::Str(name));

				CompilerScratch::Dependency dependency(obj_exception);
				dependency.number = item.xcp_code;
				csb->csb_dependencies.push(dependency);
				break;
			}

			case blr_default_code:
				item.xcp_type = xcp_default;
				item.xcp_code = 0;
				break;

			default:
				fb_assert(FALSE);
				break;
		}
	}

	node->action = PAR_parse_stmt(tdbb, csb);

	return node;
}

ErrorHandlerNode* ErrorHandlerNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ErrorHandlerNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ErrorHandlerNode";
}

void ErrorHandlerNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

ErrorHandlerNode* ErrorHandlerNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, action.getAddress());
	return this;
}

ErrorHandlerNode* ErrorHandlerNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, action.getAddress(), this);
	return this;
}

const StmtNode* ErrorHandlerNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* exeState) const
{
	if ((request->req_flags & req_error_handler) && !exeState->errorPending)
	{
		fb_assert(request->req_caller == exeState->oldRequest);
		request->req_caller = NULL;
		return this;
	}

	const StmtNode* retNode = parentStmt;
	retNode = retNode->parentStmt;

	if (request->req_operation == jrd_req::req_unwind)
		retNode = retNode->parentStmt;

	request->req_last_xcp.clear();

	return retNode;
}


//--------------------


static RegisterNode<ExecProcedureNode> regExecProcedureNodeProc(blr_exec_proc);
static RegisterNode<ExecProcedureNode> regExecProcedureNodeProc2(blr_exec_proc2);
static RegisterNode<ExecProcedureNode> regExecProcedureNodePid(blr_exec_pid);

// Parse an execute procedure reference.
DmlNode* ExecProcedureNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	SET_TDBB(tdbb);

	jrd_prc* procedure = NULL;
	QualifiedName name;

	if (blrOp == blr_exec_pid)
	{
		const USHORT pid = csb->csb_blr_reader.getWord();
		if (!(procedure = MET_lookup_procedure_id(tdbb, pid, false, false, 0)))
			name.identifier.printf("id %d", pid);
	}
	else
	{
		if (blrOp == blr_exec_proc2)
			PAR_name(csb, name.package);
		PAR_name(csb, name.identifier);
		procedure = MET_lookup_procedure(tdbb, name, false);
	}

	if (!procedure)
		PAR_error(csb, Arg::Gds(isc_prcnotdef) << Arg::Str(name.toString()));

	ExecProcedureNode* node = FB_NEW(pool) ExecProcedureNode(pool);
	node->procedure = procedure;

	PAR_procedure_parms(tdbb, csb, procedure, node->inputMessage.getAddress(),
		node->inputSources.getAddress(), node->inputTargets.getAddress(), true);
	PAR_procedure_parms(tdbb, csb, procedure, node->outputMessage.getAddress(),
		node->outputSources.getAddress(), node->outputTargets.getAddress(), false);

	CompilerScratch::Dependency dependency(obj_procedure);
	dependency.procedure = procedure;
	csb->csb_dependencies.push(dependency);

	return node;
}

ExecProcedureNode* ExecProcedureNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ExecProcedureNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecProcedureNode";
}

void ExecProcedureNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

ExecProcedureNode* ExecProcedureNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	// Post access to procedure
	CMP_post_procedure_access(tdbb, csb, procedure);
	CMP_post_resource(&csb->csb_resources, procedure, Resource::rsc_procedure, procedure->getId());

	doPass1(tdbb, csb, inputSources.getAddress());
	doPass1(tdbb, csb, inputTargets.getAddress());
	doPass1(tdbb, csb, inputMessage.getAddress());
	doPass1(tdbb, csb, outputSources.getAddress());
	doPass1(tdbb, csb, outputTargets.getAddress());
	doPass1(tdbb, csb, outputMessage.getAddress());

	return this;
}

ExecProcedureNode* ExecProcedureNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, inputSources.getAddress());
	ExprNode::doPass2(tdbb, csb, inputTargets.getAddress());
	doPass2(tdbb, csb, inputMessage.getAddress(), this);
	ExprNode::doPass2(tdbb, csb, outputSources.getAddress());
	ExprNode::doPass2(tdbb, csb, outputTargets.getAddress());
	doPass2(tdbb, csb, outputMessage.getAddress(), this);
	return this;
}

const StmtNode* ExecProcedureNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_unwind)
		return parentStmt;

	executeProcedure(tdbb, request);

	request->req_operation = jrd_req::req_return;
	return parentStmt;
}

// Execute a stored procedure. Begin by assigning the input parameters.
// End by assigning the output parameters.
void ExecProcedureNode::executeProcedure(thread_db* tdbb, jrd_req* request) const
{
	Jrd::Attachment* attachment = tdbb->getAttachment();

	if (inputSources)
	{
		const NestConst<ValueExprNode>* const sourceEnd = inputSources->args.end();
		const NestConst<ValueExprNode>* sourcePtr = inputSources->args.begin();
		const NestConst<ValueExprNode>* targetPtr = inputTargets->args.begin();

		for (; sourcePtr != sourceEnd; ++sourcePtr, ++targetPtr)
			EXE_assignment(tdbb, *sourcePtr, *targetPtr);
	}

	ULONG inMsgLength = 0;
	UCHAR* inMsg = NULL;

	if (inputMessage)
	{
		inMsgLength = inputMessage->format->fmt_length;
		inMsg = request->getImpure<UCHAR>(inputMessage->impureOffset);
	}

	const Format* format = NULL;
	ULONG outMsgLength = 0;
	UCHAR* outMsg = NULL;

	if (outputMessage)
	{
		format = outputMessage->format;
		outMsgLength = format->fmt_length;
		outMsg = request->getImpure<UCHAR>(outputMessage->impureOffset);
	}

	jrd_req* procRequest = procedure->getStatement()->findRequest(tdbb);

	// trace procedure execution start
	TraceProcExecute trace(tdbb, procRequest, request, inputTargets);

	Array<UCHAR> temp_buffer;

	if (!outputMessage)
	{
		format = procedure->prc_output_msg->format;
		outMsgLength = format->fmt_length;
		outMsg = temp_buffer.getBuffer(outMsgLength + FB_DOUBLE_ALIGN - 1);
		outMsg = (UCHAR*) FB_ALIGN((U_IPTR) outMsg, FB_DOUBLE_ALIGN);
	}

	// Catch errors so we can unwind cleanly.

	try
	{
		Jrd::ContextPoolHolder context(tdbb, procRequest->req_pool);	// Save the old pool.

		jrd_tra* transaction = request->req_transaction;
		const SLONG savePointNumber = transaction->tra_save_point ?
			transaction->tra_save_point->sav_number : 0;

		procRequest->req_timestamp = request->req_timestamp;

		EXE_start(tdbb, procRequest, transaction);

		if (inputMessage)
			EXE_send(tdbb, procRequest, 0, inMsgLength, inMsg);

		EXE_receive(tdbb, procRequest, 1, outMsgLength, outMsg);

		// Clean up all savepoints started during execution of the procedure.

		if (transaction != attachment->getSysTransaction())
		{
			for (const Savepoint* savePoint = transaction->tra_save_point;
				 savePoint && savePointNumber < savePoint->sav_number;
				 savePoint = transaction->tra_save_point)
			{
				VIO_verb_cleanup(tdbb, transaction);
			}
		}
	}
	catch (const Exception& ex)
	{
		const bool noPriv = (ex.stuff_exception(tdbb->tdbb_status_vector) == isc_no_priv);
		trace.finish(false, noPriv ? res_unauthorized : res_failed);

		tdbb->setRequest(request);
		EXE_unwind(tdbb, procRequest);
		procRequest->req_attachment = NULL;
		procRequest->req_flags &= ~(req_in_use | req_proc_fetch);
		procRequest->req_timestamp.invalidate();
		throw;
	}

	// trace procedure execution finish
	trace.finish(false, res_successful);

	EXE_unwind(tdbb, procRequest);
	tdbb->setRequest(request);

	if (outputSources)
	{
		const NestConst<ValueExprNode>* const sourceEnd = outputSources->args.end();
		const NestConst<ValueExprNode>* sourcePtr = outputSources->args.begin();
		const NestConst<ValueExprNode>* targetPtr = outputTargets->args.begin();

		for (; sourcePtr != sourceEnd; ++sourcePtr, ++targetPtr)
			EXE_assignment(tdbb, *sourcePtr, *targetPtr);
	}

	procRequest->req_attachment = NULL;
	procRequest->req_flags &= ~(req_in_use | req_proc_fetch);
	procRequest->req_timestamp.invalidate();
}


//--------------------


static RegisterNode<ExecStatementNode> regExecStatementSql(blr_exec_sql);
static RegisterNode<ExecStatementNode> regExecStatementInto(blr_exec_into);
static RegisterNode<ExecStatementNode> regExecStatementStmt(blr_exec_stmt);

DmlNode* ExecStatementNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	ExecStatementNode* node = FB_NEW(pool) ExecStatementNode(pool);

	switch (blrOp)
	{
		case blr_exec_sql:
			node->sql = PAR_parse_value(tdbb, csb);
			break;

		case blr_exec_into:
		{
			const unsigned outputs = csb->csb_blr_reader.getWord();

			node->sql = PAR_parse_value(tdbb, csb);

			if (csb->csb_blr_reader.getByte() == 0)	// not singleton flag
				node->innerStmt = PAR_parse_stmt(tdbb, csb);

			node->outputs = PAR_args(tdbb, csb, outputs, outputs);
			break;
		}

		case blr_exec_stmt:
		{
			unsigned inputs = 0;
			unsigned outputs = 0;

			while (true)
			{
				const UCHAR code = csb->csb_blr_reader.getByte();

				switch (code)
				{
					case blr_exec_stmt_inputs:
						inputs = csb->csb_blr_reader.getWord();
						break;

					case blr_exec_stmt_outputs:
						outputs = csb->csb_blr_reader.getWord();
						break;

					case blr_exec_stmt_sql:
						node->sql = PAR_parse_value(tdbb, csb);
						break;

					case blr_exec_stmt_proc_block:
						node->innerStmt = PAR_parse_stmt(tdbb, csb);
						break;

					case blr_exec_stmt_data_src:
						node->dataSource = PAR_parse_value(tdbb, csb);
						break;

					case blr_exec_stmt_user:
						node->userName = PAR_parse_value(tdbb, csb);
						break;

					case blr_exec_stmt_pwd:
						node->password = PAR_parse_value(tdbb, csb);
						break;

					case blr_exec_stmt_role:
						node->role = PAR_parse_value(tdbb, csb);
						break;

					case blr_exec_stmt_tran:
						PAR_syntax_error(csb, "external transaction parameters");
						break;

					case blr_exec_stmt_tran_clone:
						node->traScope = static_cast<EDS::TraScope>(csb->csb_blr_reader.getByte());
						break;

					case blr_exec_stmt_privs:
						node->useCallerPrivs = true;
						break;

					case blr_exec_stmt_in_params:
					case blr_exec_stmt_in_params2:
					{
						node->inputs = FB_NEW(pool) ValueListNode(pool, inputs);
						NestConst<ValueExprNode>* const end = node->inputs->args.end();

						for (NestConst<ValueExprNode>* ptr = node->inputs->args.begin();
							 ptr != end;
							 ++ptr)
						{
							if (code == blr_exec_stmt_in_params2)
							{
								string name;

								if (PAR_name(csb, name))
								{
									MemoryPool& pool = csb->csb_pool;

									if (!node->inputNames)
										node->inputNames = FB_NEW (pool) EDS::ParamNames(pool);

									string* newName = FB_NEW (pool) string(pool, name);
									node->inputNames->add(newName);
								}
							}

							*ptr = PAR_parse_value(tdbb, csb);
						}

						break;
					}

					case blr_exec_stmt_out_params:
						node->outputs = PAR_args(tdbb, csb, outputs, outputs);
						break;

					case blr_end:
						break;

					default:
						PAR_syntax_error(csb, "unknown EXECUTE STATEMENT option");
				}

				if (code == blr_end)
					break;
			}

			break;
		}

		default:
			fb_assert(false);
	}

	return node;
}

ExecStatementNode* ExecStatementNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ExecStatementNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecStatementNode";
}

void ExecStatementNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

ExecStatementNode* ExecStatementNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, sql.getAddress());
	doPass1(tdbb, csb, dataSource.getAddress());
	doPass1(tdbb, csb, userName.getAddress());
	doPass1(tdbb, csb, password.getAddress());
	doPass1(tdbb, csb, role.getAddress());
	doPass1(tdbb, csb, innerStmt.getAddress());
	doPass1(tdbb, csb, inputs.getAddress());
	doPass1(tdbb, csb, outputs.getAddress());
	return this;
}

ExecStatementNode* ExecStatementNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, sql.getAddress());
	ExprNode::doPass2(tdbb, csb, dataSource.getAddress());
	ExprNode::doPass2(tdbb, csb, userName.getAddress());
	ExprNode::doPass2(tdbb, csb, password.getAddress());
	ExprNode::doPass2(tdbb, csb, role.getAddress());
	doPass2(tdbb, csb, innerStmt.getAddress(), this);
	ExprNode::doPass2(tdbb, csb, inputs.getAddress());
	ExprNode::doPass2(tdbb, csb, outputs.getAddress());

	impureOffset = CMP_impure(csb, sizeof(EDS::Statement*));

	return this;
}

const StmtNode* ExecStatementNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	EDS::Statement** stmtPtr = request->getImpure<EDS::Statement*>(impureOffset);
	EDS::Statement* stmt = *stmtPtr;

	if (request->req_operation == jrd_req::req_evaluate)
	{
		fb_assert(!*stmtPtr);

		string sSql;
		getString(tdbb, request, sql, sSql);

		string sDataSrc;
		getString(tdbb, request, dataSource, sDataSrc);

		string sUser;
		getString(tdbb, request, userName, sUser);

		string sPwd;
		getString(tdbb, request, password, sPwd);

		string sRole;
		getString(tdbb, request, role, sRole);

		EDS::Connection* conn = EDS::Manager::getConnection(tdbb, sDataSrc, sUser, sPwd, sRole, traScope);

		stmt = conn->createStatement(sSql);

		EDS::Transaction* tran = EDS::Transaction::getTransaction(tdbb, stmt->getConnection(), traScope);

		stmt->bindToRequest(request, stmtPtr);
		stmt->setCallerPrivileges(useCallerPrivs);

		const string* const* inpNames = inputNames ? inputNames->begin() : NULL;
		stmt->prepare(tdbb, tran, sSql, inputNames != NULL);

		if (stmt->isSelectable())
			stmt->open(tdbb, tran, inpNames, inputs, !innerStmt);
		else
			stmt->execute(tdbb, tran, inpNames, inputs, outputs);

		request->req_operation = jrd_req::req_return;
	}  // jrd_req::req_evaluate

	if (request->req_operation == jrd_req::req_return || request->req_operation == jrd_req::req_sync)
	{
		fb_assert(stmt);

		if (stmt->isSelectable())
		{
			if (stmt->fetch(tdbb, outputs))
			{
				request->req_operation = jrd_req::req_evaluate;
				return innerStmt;
			}

			request->req_operation = jrd_req::req_return;
		}
	}

	if (request->req_operation == jrd_req::req_unwind)
	{
		const LabelNode* label = StmtNode::as<LabelNode>(parentStmt.getObject());

		if (label && request->req_label == label->labelNumber &&
			(request->req_flags & req_continue_loop))
		{
			request->req_flags &= ~req_continue_loop;
			request->req_operation = jrd_req::req_sync;
			return this;
		}
	}

	if (stmt)
		stmt->close(tdbb);

	return parentStmt;
}

void ExecStatementNode::getString(thread_db* tdbb, jrd_req* request, const ValueExprNode* node,
	string& str) const
{
	MoveBuffer buffer;
	UCHAR* p = NULL;
	int len = 0;
	const dsc* dsc = node ? EVL_expr(tdbb, request, node) : NULL;

	if (dsc && !(request->req_flags & req_null))
		len = MOV_make_string2(tdbb, dsc, dsc->getTextType(), &p, buffer);

	str = string((char*) p, len);
	str.trim();
}


//--------------------


static RegisterNode<IfNode> regIfNode(blr_if);

DmlNode* IfNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	IfNode* node = FB_NEW(pool) IfNode(pool);

	node->condition = PAR_parse_boolean(tdbb, csb);
	node->trueAction = PAR_parse_stmt(tdbb, csb);

	if (csb->csb_blr_reader.peekByte() == blr_end)
		csb->csb_blr_reader.getByte(); // skip blr_end
	else
		node->falseAction = PAR_parse_stmt(tdbb, csb);

	return node;
}

IfNode* IfNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	IfNode* node = FB_NEW(getPool()) IfNode(getPool());
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

void IfNode::genBlr(DsqlCompilerScratch* dsqlScratch)
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
	doPass1(tdbb, csb, condition.getAddress());
	doPass1(tdbb, csb, trueAction.getAddress());
	doPass1(tdbb, csb, falseAction.getAddress());
	return this;
}

IfNode* IfNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, condition.getAddress());
	doPass2(tdbb, csb, trueAction.getAddress(), this);
	doPass2(tdbb, csb, falseAction.getAddress(), this);
	return this;
}

const StmtNode* IfNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		if (condition->execute(tdbb, request))
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

	return parentStmt;
}


//--------------------


static RegisterNode<InAutonomousTransactionNode> regInAutonomousTransactionNode(blr_auto_trans);

DmlNode* InAutonomousTransactionNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	InAutonomousTransactionNode* node = FB_NEW(pool) InAutonomousTransactionNode(pool);

	if (csb->csb_blr_reader.getByte() != 0)	// Reserved for future improvements. Should be 0 for now.
		PAR_syntax_error(csb, "0");

	node->action = PAR_parse_stmt(tdbb, csb);

	return node;
}

InAutonomousTransactionNode* InAutonomousTransactionNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	const bool autoTrans = dsqlScratch->flags & DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;
	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;

	InAutonomousTransactionNode* node = FB_NEW(getPool()) InAutonomousTransactionNode(getPool());
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

void InAutonomousTransactionNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_auto_trans);
	dsqlScratch->appendUChar(0);	// to extend syntax in the future
	GEN_statement(dsqlScratch, dsqlAction);
}

InAutonomousTransactionNode* InAutonomousTransactionNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, action.getAddress());
	return this;
}

InAutonomousTransactionNode* InAutonomousTransactionNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	savNumberOffset = CMP_impure(csb, sizeof(SLONG));
	doPass2(tdbb, csb, action.getAddress(), this);
	return this;
}

const StmtNode* InAutonomousTransactionNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
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

	return parentStmt;
}


//--------------------


static RegisterNode<InitVariableNode> regInitVariableNode(blr_init_variable);

DmlNode* InitVariableNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	InitVariableNode* node = FB_NEW(pool) InitVariableNode(pool);
	node->varId = csb->csb_blr_reader.getWord();

	vec<DeclareVariableNode*>* vector = csb->csb_variables;

	if (!vector || node->varId >= vector->count())
		PAR_syntax_error(csb, "variable identifier");

	return node;
}

InitVariableNode* InitVariableNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void InitVariableNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "InitVariableNode";
}

void InitVariableNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

InitVariableNode* InitVariableNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	if (copier.csb->csb_remap_variable == 0)
		return this;

	InitVariableNode* node = FB_NEW(*tdbb->getDefaultPool()) InitVariableNode(*tdbb->getDefaultPool());
	node->varId = varId + copier.csb->csb_remap_variable;
	node->varDecl = varDecl;
	node->varInfo = varInfo;
	return node;
}

InitVariableNode* InitVariableNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	vec<DeclareVariableNode*>* vector = csb->csb_variables;

	if (!vector || varId >= vector->count() || !(varDecl = (*vector)[varId]))
		PAR_syntax_error(csb, "variable identifier");

	return this;
}

InitVariableNode* InitVariableNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	varInfo = CMP_pass2_validation(tdbb, csb, Item(Item::TYPE_VARIABLE, varId));
	return this;
}

const StmtNode* InitVariableNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		if (varInfo)
		{
			dsc* toDesc = &request->getImpure<impure_value>(varDecl->impureOffset)->vlu_desc;
			toDesc->dsc_flags |= DSC_null;

			MapFieldInfo::ValueType fieldInfo;

			if (varInfo->fullDomain &&
				request->getStatement()->mapFieldInfo.get(varInfo->field, fieldInfo) &&
				fieldInfo.defaultValue)
			{
				dsc* value = EVL_expr(tdbb, request, fieldInfo.defaultValue);

				if (value && !(request->req_flags & req_null))
				{
					toDesc->dsc_flags &= ~DSC_null;
					MOV_move(tdbb, value, toDesc);
				}
			}
		}

		request->req_operation = jrd_req::req_return;
	}

	return parentStmt;
}


//--------------------


ExecBlockNode* ExecBlockNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	if (returns.hasData())
		statement->setType(DsqlCompiledStatement::TYPE_SELECT_BLOCK);
	else
		statement->setType(DsqlCompiledStatement::TYPE_EXEC_BLOCK);

	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_BLOCK;

	ExecBlockNode* node = FB_NEW(getPool()) ExecBlockNode(getPool());

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

void ExecBlockNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->beginDebug();

	// now do the input parameters
	for (size_t i = 0; i < parameters.getCount(); ++i)
	{
		ParameterClause& parameter = parameters[i];

		VariableNode* var = MAKE_variable(parameter.legacyField,
			parameter.name.c_str(), VAR_input, 0, (USHORT) (2 * i), 0);

		dsqlScratch->variables.add(var);
	}

	const unsigned returnsPos = dsqlScratch->variables.getCount();

	// now do the output parameters
	for (size_t i = 0; i < returns.getCount(); ++i)
	{
		ParameterClause& parameter = returns[i];

		VariableNode* var = MAKE_variable(parameter.legacyField,
			parameter.name.c_str(), VAR_output, 1, (USHORT) (2 * i), i);

		dsqlScratch->variables.add(var);
		dsqlScratch->outputVariables.add(var);
	}

	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	dsqlScratch->appendUChar(blr_begin);

	if (parameters.hasData())
	{
		revertParametersOrder(statement->getSendMsg()->msg_parameters);
		GEN_port(dsqlScratch, statement->getSendMsg());
	}
	else
		statement->setSendMsg(NULL);

	for (Array<VariableNode*>::const_iterator i = dsqlScratch->outputVariables.begin();
		 i != dsqlScratch->outputVariables.end();
		 ++i)
	{
		dsql_nod* varNod = MAKE_node(Dsql::nod_class_exprnode, 1);
		varNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(*i);

		dsql_par* param = MAKE_parameter(statement->getReceiveMsg(), true, true,
			(i - dsqlScratch->outputVariables.begin()) + 1, varNod);
		param->par_node = varNod;
		MAKE_desc(dsqlScratch, &param->par_desc, varNod);
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
		const VariableNode* parameter = dsqlScratch->variables[i];
		const dsql_var* variable = parameter->dsqlVar;
		const dsql_fld* field = variable->var_field;

		if (field->fld_full_domain || field->fld_not_nullable)
		{
			// ASF: Validation of execute block input parameters is different than procedure
			// parameters, because we can't generate messages using the domains due to the
			// connection charset influence. So to validate, we cast them and assign to null.
			dsqlScratch->appendUChar(blr_assignment);
			dsqlScratch->appendUChar(blr_cast);
			dsqlScratch->putDtype(field, true);
			dsqlScratch->appendUChar(blr_parameter2);
			dsqlScratch->appendUChar(0);
			dsqlScratch->appendUShort(variable->var_msg_item);
			dsqlScratch->appendUShort(variable->var_msg_item + 1);
			dsqlScratch->appendUChar(blr_null);
		}
	}

	for (Array<VariableNode*>::const_iterator i = dsqlScratch->outputVariables.begin();
		 i != dsqlScratch->outputVariables.end();
		 ++i)
	{
		dsqlScratch->putLocalVariable((*i)->dsqlVar, 0, NULL);
	}

	dsqlScratch->setPsql(true);

	dsqlScratch->putLocalVariables(localDeclList, USHORT(returns.getCount()));

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
	dsqlScratch->genReturn(true);
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

					CompilerScratch::Dependency dependency(obj_exception);
					dependency.number = item.xcp_code;
					csb->csb_dependencies.push(dependency);
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
		node->parameters = PAR_args(tdbb, csb, count, count);
	}
	else if (type == blr_exception_msg)
		node->messageExpr = PAR_parse_value(tdbb, csb);

	return node;
}

StmtNode* ExceptionNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	ExceptionNode* node = FB_NEW(getPool()) ExceptionNode(getPool());
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

void ExceptionNode::genBlr(DsqlCompilerScratch* dsqlScratch)
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
	doPass1(tdbb, csb, messageExpr.getAddress());
	doPass1(tdbb, csb, parameters.getAddress());
	return this;
}

ExceptionNode* ExceptionNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, messageExpr.getAddress());
	ExprNode::doPass2(tdbb, csb, parameters.getAddress());
	return this;
}

const StmtNode* ExceptionNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
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

	return parentStmt;
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
		dsc* desc = EVL_expr(tdbb, request, messageExpr);

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
				for (const NestConst<ValueExprNode>* parameter = parameters->args.begin();
					 parameter != parameters->args.end(); ++parameter)
				{
					const dsc* value = EVL_expr(tdbb, request, *parameter);

					if (!value || (request->req_flags & req_null))
						paramsStr.push(NULL_STRING_MARK);
					else
						paramsStr.push(MOV_make_string2(tdbb, value, ttype_metadata));
				}

				// And add the values to the args and status vector only after they are all created
				// and will not move in paramsStr.

				MsgFormat::SafeArg arg;
				for (size_t i = 0; i < parameters->args.getCount(); ++i)
					arg << paramsStr[i].c_str();

				MsgFormat::StringRefStream stream(formattedMsg);
				MsgFormat::MsgPrint(stream, s, arg, true);

				status << formattedMsg;

				for (size_t i = 0; i < parameters->args.getCount(); ++i)
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

void ExitNode::genBlr(DsqlCompilerScratch* dsqlScratch)
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
		node->stall = PAR_parse_stmt(tdbb, csb);

	if (csb->csb_blr_reader.peekByte() == (UCHAR) blr_rse ||
		csb->csb_blr_reader.peekByte() == (UCHAR) blr_singular ||
		csb->csb_blr_reader.peekByte() == (UCHAR) blr_scrollable)
	{
		node->rse = PAR_rse(tdbb, csb);
	}
	else
		node->rse = PAR_rse(tdbb, csb, blrOp);

	node->statement = PAR_parse_stmt(tdbb, csb);

	return node;
}

StmtNode* ForNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	ForNode* node = FB_NEW(getPool()) ForNode(getPool());

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

void ForNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	// CVC: Only put a label if this is not singular; otherwise,
	// what loop is the user trying to abandon?
	if (dsqlAction)
	{
		dsqlScratch->appendUChar(blr_label);
		dsqlScratch->appendUChar((int)(IPTR) dsqlLabel->nod_arg[Dsql::e_label_number]);
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
	doPass1(tdbb, csb, stall.getAddress());
	doPass1(tdbb, csb, rse.getAddress());
	doPass1(tdbb, csb, statement.getAddress());
	return this;
}

StmtNode* ForNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	rse->pass2Rse(tdbb, csb);

	doPass2(tdbb, csb, stall.getAddress(), this);
	ExprNode::doPass2(tdbb, csb, rse.getAddress());
	doPass2(tdbb, csb, statement.getAddress(), this);

	// Finish up processing of record selection expressions.

	RecordSource* const rsb = CMP_post_rse(tdbb, csb, rse.getObject());
	csb->csb_fors.add(rsb);

	cursor = FB_NEW(*tdbb->getDefaultPool()) Cursor(csb, rsb, rse->rse_invariants,
		(rse->flags & RseNode::FLAG_SCROLLABLE));

	return this;
}

const StmtNode* ForNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
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
			const LabelNode* label = StmtNode::as<LabelNode>(parentStmt.getObject());

			if (label && request->req_label == label->labelNumber &&
				(request->req_flags & req_continue_loop))
			{
				request->req_flags &= ~req_continue_loop;
				request->req_operation = jrd_req::req_sync;
				return this;
			}

			// fall into
		}

		default:
			cursor->close(tdbb);
			return parentStmt;
	}

	fb_assert(false); // unreachable code
	return NULL;
}


//--------------------


static RegisterNode<HandlerNode> regHandlerNode(blr_handler);

DmlNode* HandlerNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	HandlerNode* node = FB_NEW(pool) HandlerNode(pool);
	node->statement = PAR_parse_stmt(tdbb, csb);
	return node;
}

HandlerNode* HandlerNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void HandlerNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "HandlerNode";
}

void HandlerNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

HandlerNode* HandlerNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, statement.getAddress());
	return this;
}

HandlerNode* HandlerNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	return this;
}

const StmtNode* HandlerNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			return statement;

		case jrd_req::req_unwind:
			if (!request->req_label)
				request->req_operation = jrd_req::req_return;

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<LabelNode> regLabelNode(blr_label);

DmlNode* LabelNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	LabelNode* node = FB_NEW(pool) LabelNode(pool);

	node->labelNumber = csb->csb_blr_reader.getByte();
	node->statement = PAR_parse_stmt(tdbb, csb);

	return node;
}

LabelNode* LabelNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void LabelNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "LabelNode";
}

void LabelNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

LabelNode* LabelNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, statement.getAddress());
	return this;
}

LabelNode* LabelNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	return this;
}

const StmtNode* LabelNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* exeState) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			return statement;

		case jrd_req::req_unwind:
			fb_assert(!(request->req_flags & req_continue_loop));

			if (request->req_label == labelNumber &&
				(request->req_flags & (req_leave | req_error_handler)))
			{
				request->req_flags &= ~req_leave;
				request->req_operation = jrd_req::req_return;
			}
			// fall into

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<LoopNode> regLoopNode(blr_loop);

DmlNode* LoopNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	LoopNode* node = FB_NEW(pool) LoopNode(pool);
	node->statement = PAR_parse_stmt(tdbb, csb);
	return node;
}

LoopNode* LoopNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void LoopNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "LoopNode";
}

void LoopNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

LoopNode* LoopNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, statement.getAddress());
	return this;
}

LoopNode* LoopNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	return this;
}

const StmtNode* LoopNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
		case jrd_req::req_return:
			request->req_operation = jrd_req::req_evaluate;
			return statement;

		case jrd_req::req_unwind:
		{
			const LabelNode* label = StmtNode::as<LabelNode>(parentStmt.getObject());

			if (label && request->req_label == label->labelNumber &&
				(request->req_flags & req_continue_loop))
			{
				request->req_flags &= ~req_continue_loop;
				request->req_operation = jrd_req::req_evaluate;
				return statement;
			}
			// fall into
		}

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<MessageNode> regMessageNode(blr_message);

// Parse a message declaration, including operator byte.
DmlNode* MessageNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	MessageNode* node = FB_NEW(pool) MessageNode(pool);

	// Get message number, register it in the compiler scratch block, and
	// allocate a node to represent the message.

	USHORT n = csb->csb_blr_reader.getByte();
	CompilerScratch::csb_repeat* tail = CMP_csb_element(csb, n);

	tail->csb_message = node;
	node->messageNumber = n;

	if (n > csb->csb_msg_number)
		csb->csb_msg_number = n;

	// Get the number of parameters in the message and prepare to fill out the format block.

	n = csb->csb_blr_reader.getWord();
	node->format = Format::newFormat(*tdbb->getDefaultPool(), n);
	ULONG offset = 0;

	Format::fmt_desc_iterator desc, end;
	USHORT index = 0;

	for (desc = node->format->fmt_desc.begin(), end = desc + n; desc < end; ++desc, ++index)
	{
		ItemInfo itemInfo;
		const USHORT alignment = PAR_desc(tdbb, csb, &*desc, &itemInfo);
		if (alignment)
			offset = FB_ALIGN(offset, alignment);
		desc->dsc_address = (UCHAR *) (IPTR) offset;
		offset += desc->dsc_length;

		// ASF: Odd indexes are the nullable flag.
		// So we only check even indexes, which is the actual parameter.
		if (itemInfo.isSpecial() && index % 2 == 0)
		{
			csb->csb_dbg_info.argInfoToName.get(
				Firebird::ArgumentInfo(csb->csb_msg_number, index / 2), itemInfo.name);

			csb->csb_map_item_info.put(Item(Item::TYPE_PARAMETER, csb->csb_msg_number, index),
				itemInfo);
		}
	}

	if (offset > MAX_FORMAT_SIZE)
		PAR_error(csb, Arg::Gds(isc_imp_exc) << Arg::Gds(isc_blktoobig));

	node->format->fmt_length = (USHORT) offset;

	return node;
}

MessageNode* MessageNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void MessageNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "MessageNode";
}

void MessageNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

MessageNode* MessageNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	MessageNode* node = FB_NEW(*tdbb->getDefaultPool()) MessageNode(*tdbb->getDefaultPool());
	node->messageNumber = messageNumber;
	node->format = format;
	node->impureFlags = impureFlags;
	return node;
}

MessageNode* MessageNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	return this;
}

MessageNode* MessageNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(format);

	impureOffset = CMP_impure(csb, FB_ALIGN(format->fmt_length, 2));
	impureFlags = CMP_impure(csb, sizeof(USHORT) * format->fmt_count);

	return this;
}

const StmtNode* MessageNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		USHORT* flags = request->getImpure<USHORT>(impureFlags);
		memset(flags, 0, sizeof(USHORT) * format->fmt_count);
		request->req_operation = jrd_req::req_return;
	}

	return parentStmt;
}


//--------------------


static RegisterNode<ModifyNode> regModifyNode(blr_modify);
static RegisterNode<ModifyNode> regModifyNode2(blr_modify2);

// Parse a modify statement.
DmlNode* ModifyNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	// Parse the original and new contexts.

	USHORT context = (unsigned int) csb->csb_blr_reader.getByte();

	if (context >= csb->csb_rpt.getCount() || !(csb->csb_rpt[context].csb_flags & csb_used))
		PAR_error(csb, Arg::Gds(isc_ctxnotdef));

	const SSHORT orgStream = csb->csb_rpt[context].csb_stream;
	const SSHORT newStream = csb->nextStream(false);

	if (newStream >= MAX_STREAMS)
		PAR_error(csb, Arg::Gds(isc_too_many_contexts));

	context = csb->csb_blr_reader.getByte();

	// Make sure the compiler scratch block is big enough to hold everything.

	CompilerScratch::csb_repeat* tail = CMP_csb_element(csb, context);
	tail->csb_stream = (UCHAR) newStream;
	tail->csb_flags |= csb_used;

	tail = CMP_csb_element(csb, newStream);
	tail->csb_relation = csb->csb_rpt[orgStream].csb_relation;

	// Make the node and parse the sub-expression.

	ModifyNode* node = FB_NEW(pool) ModifyNode(pool);
	node->orgStream = orgStream;
	node->newStream = newStream;
	node->statement = PAR_parse_stmt(tdbb, csb);

	if (blrOp == blr_modify2)
		node->statement2 = PAR_parse_stmt(tdbb, csb);

	return node;
}

ModifyNode* ModifyNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ModifyNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ModifyNode";
}

void ModifyNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

ModifyNode* ModifyNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	pass1Modify(tdbb, csb, this);

	doPass1(tdbb, csb, statement.getAddress());
	doPass1(tdbb, csb, statement2.getAddress());
	doPass1(tdbb, csb, subMod.getAddress());
	pass1Validations(tdbb, csb, validations);
	doPass1(tdbb, csb, mapView.getAddress());

	return this;
}

// Process a source for a modify statement. This can get a little tricky if the relation is a view.
void ModifyNode::pass1Modify(thread_db* tdbb, CompilerScratch* csb, ModifyNode* node)
{
	// If updateable views with triggers are involved, there maybe a recursive call to be ignored.

	if (node->subMod)
		return;

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parentStream = 0;

	// To support nested views, loop until we hit a table or a view with user-defined triggers
	// (which means no update).

	for (;;)
	{
		USHORT stream = node->orgStream;
		USHORT newStream = node->newStream;

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[newStream];
		tail->csb_flags |= csb_modify;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = relation->rel_view_rse ? relation : view;

		if (!parent)
			parent = tail->csb_view;

		postTriggerAccess(csb, relation, ExternalAccess::exa_update, view);

		// Check out update. If this is an update thru a view, verify the view by checking for read
		// access on the base table. If field-level select privileges are implemented, this needs
		// to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_update;

		if (parent)
			priv |= SCL_read;

		const trig_vec* trigger = (relation->rel_pre_modify) ?
			relation->rel_pre_modify : relation->rel_post_modify;

		// If we have a view with triggers, let's expand it.

		if (relation->rel_view_rse && trigger)
			node->mapView = pass1ExpandView(tdbb, csb, stream, newStream, false);

		// Get the source relation, either a table or yet another view.

		RelationSourceNode* source = pass1Update(tdbb, csb, relation, trigger, stream,
			newStream, priv, parent, parentStream);

		if (!source)
		{
			// No source means we're done.
			if (!relation->rel_view_rse)
			{
				// Apply validation constraints.
				makeValidation(tdbb, csb, newStream, node->validations);
			}

			return;
		}

		parent = relation;
		parentStream = stream;

		// Remap the source stream.

		UCHAR* map = csb->csb_rpt[stream].csb_map;

		stream = source->getStream();
		stream = map[stream];

		// Copy the view source.

		map = CMP_alloc_map(tdbb, csb, node->newStream);
		NodeCopier copier(csb, map);
		source = source->copy(tdbb, copier);

		if (trigger)
		{
			// ASF: This code is responsible to make view's WITH CHECK OPTION to work as constraints.

			// Set up the new target stream.

			const USHORT viewStream = newStream;
			newStream = source->getStream();
			fb_assert(newStream <= MAX_STREAMS);
			map[viewStream] = newStream;

			ModifyNode* viewNode = FB_NEW(*tdbb->getDefaultPool()) ModifyNode(*tdbb->getDefaultPool());
			viewNode->statement = pass1ExpandView(tdbb, csb, viewStream, newStream, true);

			node->subMod = viewNode;

			node = viewNode;
		}
		else
		{
			// This relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view).
			csb->csb_rpt[newStream].csb_flags &= ~csb_view_update;
		}

		// Let's reset streams to represent the mapped source and target.
		node->orgStream = stream;
		node->newStream = source->getStream();
	}
}

ModifyNode* ModifyNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	// AB: Mark the streams involved with UPDATE statements active.
	// So that the optimizer can use indices for eventually used sub-selects.
	csb->csb_rpt[orgStream].csb_flags |= csb_active;
	csb->csb_rpt[newStream].csb_flags |= csb_active;

	doPass2(tdbb, csb, statement.getAddress(), this);
	doPass2(tdbb, csb, statement2.getAddress(), this);
	doPass2(tdbb, csb, subMod.getAddress(), this);

	for (Array<ValidateInfo>::iterator i = validations.begin(); i != validations.end(); ++i)
	{
		doPass2(tdbb, csb, i->stmt.getAddress(), this);
		ExprNode::doPass2(tdbb, csb, i->boolean.getAddress());
		ExprNode::doPass2(tdbb, csb, i->value.getAddress());
	}

	doPass2(tdbb, csb, mapView.getAddress(), this);

	// AB: Remove the previous flags
	csb->csb_rpt[orgStream].csb_flags &= ~csb_active;
	csb->csb_rpt[newStream].csb_flags &= ~csb_active;

	csb->csb_rpt[orgStream].csb_flags |= csb_update;

	const Format* format = CMP_format(tdbb, csb, orgStream);
	Format::fmt_desc_const_iterator desc = format->fmt_desc.begin();

	for (ULONG id = 0; id < format->fmt_count; ++id, ++desc)
	{
		if (desc->dsc_dtype)
			SBM_SET(tdbb->getDefaultPool(), &csb->csb_rpt[orgStream].csb_fields, id);
	}

	impureOffset = CMP_impure(csb, sizeof(impure_state));

	return this;
}

const StmtNode* ModifyNode::execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const
{
	impure_state* impure = request->getImpure<impure_state>(impureOffset);
	const StmtNode* retNode;

	if (request->req_operation == jrd_req::req_unwind)
		return parentStmt;
	else if (request->req_operation == jrd_req::req_return && !impure->sta_state && subMod)
	{
		if (!exeState->topNode)
		{
			exeState->topNode = this;
			exeState->whichModTrig = PRE_TRIG;
		}

		exeState->prevNode = this;
		retNode = modify(tdbb, request, exeState->whichModTrig);

		if (exeState->whichModTrig == PRE_TRIG)
		{
			retNode = subMod;
			fb_assert(retNode->parentStmt == exeState->prevNode);
			///retNode->nod_parent = exeState->prevNode;
		}

		if (exeState->topNode == exeState->prevNode && exeState->whichModTrig == POST_TRIG)
		{
			exeState->topNode = NULL;
			exeState->whichModTrig = ALL_TRIGS;
		}
		else
			request->req_operation = jrd_req::req_evaluate;
	}
	else
	{
		exeState->prevNode = this;
		retNode = modify(tdbb, request, ALL_TRIGS);

		if (!subMod && exeState->whichModTrig == PRE_TRIG)
			exeState->whichModTrig = POST_TRIG;
	}

	return retNode;
}

// Execute a MODIFY statement.
const StmtNode* ModifyNode::modify(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const
{
	Jrd::Attachment* attachment = tdbb->getAttachment();
	jrd_tra* transaction = request->req_transaction;
	impure_state* impure = request->getImpure<impure_state>(impureOffset);

	record_param* orgRpb = &request->req_rpb[orgStream];
	jrd_rel* relation = orgRpb->rpb_relation;

	if (orgRpb->rpb_number.isBof() || (!relation->rel_view_rse && !orgRpb->rpb_number.isValid()))
		ERR_post(Arg::Gds(isc_no_cur_rec));

	record_param* newRpb = &request->req_rpb[newStream];

	// If the stream was sorted, the various fields in the rpb are
	// probably junk.  Just to make sure that everything is cool,
	// refetch and release the record.

	if (orgRpb->rpb_stream_flags & RPB_s_refetch)
	{
		VIO_refetch_record(tdbb, orgRpb, transaction);
		orgRpb->rpb_stream_flags &= ~RPB_s_refetch;
	}

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			request->req_records_affected.bumpModified(false);
			break;

		case jrd_req::req_return:
			if (impure->sta_state == 1)
			{
				impure->sta_state = 0;
				Record* orgRecord = orgRpb->rpb_record;
				const Record* newRecord = newRpb->rpb_record;
				memcpy(orgRecord->rec_data, newRecord->rec_data, newRecord->rec_length);
				request->req_operation = jrd_req::req_evaluate;
				return statement;
			}

			if (impure->sta_state == 0)
			{
				// CVC: This call made here to clear the record in each NULL field and
				// varchar field whose tail may contain garbage.
				cleanupRpb(tdbb, newRpb);

				if (transaction != attachment->getSysTransaction())
					++transaction->tra_save_point->sav_verb_count;

				preModifyEraseTriggers(tdbb, &relation->rel_pre_modify, whichTrig, orgRpb, newRpb,
					jrd_req::req_trigger_update);

				if (validations.hasData())
					validateExpressions(tdbb, validations);

				if (relation->rel_file)
					EXT_modify(orgRpb, newRpb, transaction);
				else if (relation->isVirtual())
					VirtualTable::modify(tdbb, orgRpb, newRpb);
				else if (!relation->rel_view_rse)
				{
					USHORT badIndex;
					jrd_rel* badRelation = NULL;

					VIO_modify(tdbb, orgRpb, newRpb, transaction);
					const idx_e errorCode = IDX_modify(tdbb, orgRpb, newRpb, transaction,
						&badRelation, &badIndex);

					if (errorCode)
						ERR_duplicate_error(errorCode, badRelation, badIndex);
				}

				newRpb->rpb_number = orgRpb->rpb_number;
				newRpb->rpb_number.setValid(true);

				if (relation->rel_post_modify && whichTrig != PRE_TRIG)
				{
					EXE_execute_triggers(tdbb, &relation->rel_post_modify, orgRpb, newRpb,
						jrd_req::req_trigger_update, POST_TRIG);
				}

				// Now call IDX_modify_check_constrints after all post modify triggers
				// have fired.  This is required for cascading referential integrity,
				// which can be implemented as post_erase triggers.

				if (!relation->rel_file && !relation->rel_view_rse && !relation->isVirtual())
				{
					USHORT badIndex;
					jrd_rel* badRelation = NULL;

					const idx_e errorCode = IDX_modify_check_constraints(tdbb, orgRpb, newRpb,
						transaction, &badRelation, &badIndex);

					if (errorCode)
						ERR_duplicate_error(errorCode, badRelation, badIndex);
				}

				if (transaction != attachment->getSysTransaction())
					--transaction->tra_save_point->sav_verb_count;

				// CVC: Increment the counter only if we called VIO/EXT_modify() and
				// we were successful.
				if (!(request->req_view_flags & req_first_modify_return))
				{
					request->req_view_flags |= req_first_modify_return;

					if (relation->rel_view_rse)
						request->req_top_view_modify = relation;
				}

				if (relation == request->req_top_view_modify)
				{
					if (whichTrig == ALL_TRIGS || whichTrig == POST_TRIG)
					{
						request->req_records_updated++;
						request->req_records_affected.bumpModified(true);
					}
				}
				else if (relation->rel_file || !relation->rel_view_rse)
				{
					request->req_records_updated++;
					request->req_records_affected.bumpModified(true);
				}

				if (statement2)
				{
					impure->sta_state = 2;
					request->req_operation = jrd_req::req_evaluate;
					return statement2;
				}
			}

			if (whichTrig != PRE_TRIG)
			{
				Record* orgRecord = orgRpb->rpb_record;
				orgRpb->rpb_record = newRpb->rpb_record;
				newRpb->rpb_record = orgRecord;
			}

		default:
			return parentStmt;
	}

	impure->sta_state = 0;
	RLCK_reserve_relation(tdbb, transaction, relation, true);

	// Fall thru on evaluate to set up for modify before executing sub-statement.
	// This involves finding the appropriate format, making sure a record block
	// exists for the stream and is big enough, and copying fields from the
	// original record to the new record.

	const Format* newFormat = MET_current(tdbb, newRpb->rpb_relation);
	Record* newRecord = VIO_record(tdbb, newRpb, newFormat, tdbb->getDefaultPool());
	newRpb->rpb_address = newRecord->rec_data;
	newRpb->rpb_length = newFormat->fmt_length;
	newRpb->rpb_format_number = newFormat->fmt_version;

	const Format* orgFormat;
	Record* orgRecord = orgRpb->rpb_record;

	if (!orgRecord)
	{
		orgRecord = VIO_record(tdbb, orgRpb, newFormat, tdbb->getDefaultPool());
		orgFormat = orgRecord->rec_format;
		orgRpb->rpb_address = orgRecord->rec_data;
		orgRpb->rpb_length = orgFormat->fmt_length;
		orgRpb->rpb_format_number = orgFormat->fmt_version;
	}
	else
		orgFormat = orgRecord->rec_format;

	// Copy the original record to the new record.  If the format hasn't changed,
	// this is a simple move.  If the format has changed, each field must be
	// fetched and moved separately, remembering to set the missing flag.

	if (newFormat->fmt_version == orgFormat->fmt_version)
		memcpy(newRpb->rpb_address, orgRecord->rec_data, newRpb->rpb_length);
	else
	{
		DSC orgDesc, newDesc;

		for (SSHORT i = 0; i < newFormat->fmt_count; i++)
		{
			// In order to "map a null to a default" value (in EVL_field()),
			// the relation block is referenced.
			// Reference: Bug 10116, 10424
			CLEAR_NULL(newRecord, i);

			if (EVL_field(newRpb->rpb_relation, newRecord, i, &newDesc))
			{
				if (EVL_field(orgRpb->rpb_relation, orgRecord, i, &orgDesc))
					MOV_move(tdbb, &orgDesc, &newDesc);
				else
				{
					SET_NULL(newRecord, i);

					if (newDesc.dsc_dtype)
					{
						UCHAR* p = newDesc.dsc_address;
						USHORT n = newDesc.dsc_length;
						memset(p, 0, n);
					}
				}	// if (orgRecord)
			}		// if (newRecord)
		}			// for (fmt_count)
	}

	newRpb->rpb_number = orgRpb->rpb_number;
	newRpb->rpb_number.setValid(true);

	if (mapView)
	{
		impure->sta_state = 1;
		return mapView;
	}

	return statement;
}


//--------------------


static RegisterNode<PostEventNode> regPostEventNode1(blr_post);
static RegisterNode<PostEventNode> regPostEventNode2(blr_post_arg);

DmlNode* PostEventNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	PostEventNode* node = FB_NEW(pool) PostEventNode(pool);

	node->event = PAR_parse_value(tdbb, csb);
	if (blrOp == blr_post_arg)
		node->argument = PAR_parse_value(tdbb, csb);

	return node;
}

PostEventNode* PostEventNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	PostEventNode* node = FB_NEW(getPool()) PostEventNode(getPool());

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

void PostEventNode::genBlr(DsqlCompilerScratch* dsqlScratch)
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
	doPass1(tdbb, csb, event.getAddress());
	doPass1(tdbb, csb, argument.getAddress());
	return this;
}

PostEventNode* PostEventNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, event.getAddress());
	ExprNode::doPass2(tdbb, csb, argument.getAddress());
	return this;
}

const StmtNode* PostEventNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	jrd_tra* transaction = request->req_transaction;

	DeferredWork* work = DFW_post_work(transaction, dfw_post_event,
		EVL_expr(tdbb, request, event), 0);

	if (argument)
		DFW_post_work_arg(transaction, work, EVL_expr(tdbb, request, argument), 0);

	// For an autocommit transaction, events can be posted without any updates.

	if (transaction->tra_flags & TRA_autocommit)
		transaction->tra_flags |= TRA_perform_autocommit;

	if (request->req_operation == jrd_req::req_evaluate)
		request->req_operation = jrd_req::req_return;

	return parentStmt;
}


//--------------------


static RegisterNode<ReceiveNode> regReceiveNode(blr_receive);

DmlNode* ReceiveNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	ReceiveNode* node = FB_NEW(pool) ReceiveNode(pool);

	USHORT n = csb->csb_blr_reader.getByte();
	node->message = csb->csb_rpt[n].csb_message;
	node->statement = PAR_parse_stmt(tdbb, csb);

	return node;
}

ReceiveNode* ReceiveNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void ReceiveNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ReceiveNode";
}

void ReceiveNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

ReceiveNode* ReceiveNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, statement.getAddress());
	// Do not call message pass1.
	return this;
}

ReceiveNode* ReceiveNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	// Do not call message pass2.
	return this;
}

// Execute a RECEIVE statement. This can be entered either with "req_evaluate" (ordinary receive
// statement) or "req_proceed" (select statement).
// In the latter case, the statement isn't every formalled evaluated.
const StmtNode* ReceiveNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			request->req_operation = jrd_req::req_receive;
			request->req_message = message;
			request->req_flags |= req_stall;
			return this;

		case jrd_req::req_proceed:
			request->req_operation = jrd_req::req_evaluate;
			return statement;

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<StoreNode> regStoreNode(blr_store);
static RegisterNode<StoreNode> regStoreNode2(blr_store2);

// Parse a store statement.
DmlNode* StoreNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	StoreNode* node = FB_NEW(pool) StoreNode(pool);

	const UCHAR* blrPos = csb->csb_blr_reader.getPos();

	node->relationSource = PAR_parseRecordSource(tdbb, csb)->as<RelationSourceNode>();

	if (!node->relationSource)
	{
		csb->csb_blr_reader.setPos(blrPos);
		PAR_syntax_error(csb, "relation source");
	}

	node->statement = PAR_parse_stmt(tdbb, csb);

	if (blrOp == blr_store2)
		node->statement2 = PAR_parse_stmt(tdbb, csb);

	return node;
}

StoreNode* StoreNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void StoreNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "StoreNode";
}

void StoreNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

StoreNode* StoreNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	if (pass1Store(tdbb, csb, this))
		makeDefaults(tdbb, csb);

	doPass1(tdbb, csb, statement.getAddress());
	doPass1(tdbb, csb, statement2.getAddress());
	doPass1(tdbb, csb, subStore.getAddress());
	pass1Validations(tdbb, csb, validations);

	return this;
}

// Process a source for a store statement. This can get a little tricky if the relation is a view.
bool StoreNode::pass1Store(thread_db* tdbb, CompilerScratch* csb, StoreNode* node)
{
	// If updateable views with triggers are involved, there may be a recursive call to be ignored.

	if (node->subStore)
		return false;

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parentStream = 0;

	// To support nested views, loop until we hit a table or a view with user-defined triggers
	// (which means no update).

	for (;;)
	{
		RelationSourceNode* relSource = node->relationSource;
		const USHORT stream = relSource->getStream();

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_store;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = relation->rel_view_rse ? relation : view;

		if (!parent)
			parent = tail->csb_view;

		postTriggerAccess(csb, relation, ExternalAccess::exa_insert, view);

		const trig_vec* trigger = relation->rel_pre_store ?
			relation->rel_pre_store : relation->rel_post_store;

		// Check out insert. If this is an insert thru a view, verify the view by checking for read
		// access on the base table. If field-level select privileges are implemented, this needs
		// to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_insert;

		if (parent)
			priv |= SCL_read;

		// Get the source relation, either a table or yet another view.

		relSource = pass1Update(tdbb, csb, relation, trigger, stream, stream, priv,
			parent, parentStream);

		if (!relSource)
		{
			CMP_post_resource(&csb->csb_resources, relation, Resource::rsc_relation, relation->rel_id);

			if (!relation->rel_view_rse)
			{
				// Apply validation constraints.
				makeValidation(tdbb, csb, stream, node->validations);
			}

			return true;
		}

		parent = relation;
		parentStream = stream;

		UCHAR* map = CMP_alloc_map(tdbb, csb, stream);
		NodeCopier copier(csb, map);

		if (trigger)
		{
			// ASF: This code is responsible to make view's WITH CHECK OPTION to work as constraints.

			CMP_post_resource(&csb->csb_resources, relation, Resource::rsc_relation, relation->rel_id);

			// Set up the new target stream.

			relSource = relSource->copy(tdbb, copier);

			const USHORT newStream = relSource->getStream();
			StoreNode* viewNode = FB_NEW(*tdbb->getDefaultPool()) StoreNode(*tdbb->getDefaultPool());

			viewNode->relationSource = relSource;
			viewNode->statement = pass1ExpandView(tdbb, csb, stream, newStream, true);

			node->subStore = viewNode;

			// Substitute the original update node with the newly created one.
			node = viewNode;
		}
		else
		{
			// This relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view).
			csb->csb_rpt[stream].csb_flags &= ~csb_view_update;
			node->relationSource = relSource->copy(tdbb, copier);
		}
	}
}

// Build an default value assignments.
void StoreNode::makeDefaults(thread_db* tdbb, CompilerScratch* csb)
{
	USHORT stream = relationSource->getStream();
	jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return;

	UCHAR localMap[JrdStatement::MAP_LENGTH];
	UCHAR* map = csb->csb_rpt[stream].csb_map;

	if (!map)
	{
		map = localMap;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR relevant, too?
		map[0] = (UCHAR) stream;
		map[1] = 1;
		map[2] = 2;
	}

	StmtNodeStack stack;

	USHORT fieldId = 0;
	vec<jrd_fld*>::iterator ptr1 = vector->begin();

	for (const vec<jrd_fld*>::const_iterator end = vector->end(); ptr1 < end; ++ptr1, ++fieldId)
	{
		ValueExprNode* value;

		if (!*ptr1 || !((*ptr1)->fld_generator_name.hasData() || (value = (*ptr1)->fld_default_value)))
			continue;

		CompoundStmtNode* compoundNode = StmtNode::as<CompoundStmtNode>(statement.getObject());
		fb_assert(compoundNode);

		if (compoundNode)
		{
			bool inList = false;

			for (size_t i = 0; i < compoundNode->statements.getCount(); ++i)
			{
				const AssignmentNode* assign = StmtNode::as<AssignmentNode>(
					compoundNode->statements[i].getObject());
				fb_assert(assign);

				if (assign)
				{
					const FieldNode* fieldNode = assign->asgnTo->as<FieldNode>();
					fb_assert(fieldNode);

					if (fieldNode && fieldNode->fieldStream == stream && fieldNode->fieldId == fieldId)
					{
						inList = true;
						break;
					}
				}
			}

			if (inList)
				continue;

			AssignmentNode* assign = FB_NEW(*tdbb->getDefaultPool()) AssignmentNode(
				*tdbb->getDefaultPool());
			assign->asgnTo = PAR_gen_field(tdbb, stream, fieldId);

			stack.push(assign);

			if ((*ptr1)->fld_generator_name.hasData())
			{
				// Make a gen_id(<generator name>, 1) expression.

				LiteralNode* literal = FB_NEW(csb->csb_pool) LiteralNode(csb->csb_pool);
				SLONG* increment = FB_NEW(csb->csb_pool) SLONG(1);
				literal->litDesc.makeLong(0, increment);

				GenIdNode* genNode = FB_NEW(csb->csb_pool) GenIdNode(csb->csb_pool,
					(csb->csb_g_flags & csb_blr_version4), (*ptr1)->fld_generator_name);
				genNode->id = MET_lookup_generator(tdbb, (*ptr1)->fld_generator_name.c_str());
				genNode->arg = literal;

				assign->asgnFrom = genNode;
			}
			else //if (value)
			{
				// Clone the field default value.
				assign->asgnFrom = RemapFieldNodeCopier(csb, map, fieldId).copy(tdbb, value);
			}
		}
	}

	if (stack.isEmpty())
		return;

	// We have some default - add the original statement and make a list out of the whole mess.
	stack.push(statement);
	statement = PAR_make_list(tdbb, stack);
}

StoreNode* StoreNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	// AB: Mark the streams involved with INSERT statements active.
	// So that the optimizer can use indices for eventually used sub-selects.

	USHORT stream = relationSource->getStream();
	csb->csb_rpt[stream].csb_flags |= csb_active;

	doPass2(tdbb, csb, statement.getAddress(), this);
	doPass2(tdbb, csb, statement2.getAddress(), this);
	doPass2(tdbb, csb, subStore.getAddress(), this);

	for (Array<ValidateInfo>::iterator i = validations.begin(); i != validations.end(); ++i)
	{
		doPass2(tdbb, csb, i->stmt.getAddress(), this);
		ExprNode::doPass2(tdbb, csb, i->boolean.getAddress());
		ExprNode::doPass2(tdbb, csb, i->value.getAddress());
	}

	// AB: Remove the previous flags
	csb->csb_rpt[stream].csb_flags &= ~csb_active;

	impureOffset = CMP_impure(csb, sizeof(impure_state));

	return this;
}

const StmtNode* StoreNode::execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const
{
	impure_state* impure = request->getImpure<impure_state>(impureOffset);
	const StmtNode* retNode;

	if (request->req_operation == jrd_req::req_return && !impure->sta_state && subStore)
	{
		if (!exeState->topNode)
		{
			exeState->topNode = this;
			exeState->whichStoTrig = PRE_TRIG;
		}

		exeState->prevNode = this;
		retNode = store(tdbb, request, exeState->whichStoTrig);

		if (exeState->whichStoTrig == PRE_TRIG)
		{
			retNode = subStore;
			fb_assert(retNode->parentStmt == exeState->prevNode);
			///retNode->nod_parent = exeState->prevNode;
		}

		if (exeState->topNode == exeState->prevNode && exeState->whichStoTrig == POST_TRIG)
		{
			exeState->topNode = NULL;
			exeState->whichStoTrig = ALL_TRIGS;
		}
		else
			request->req_operation = jrd_req::req_evaluate;
	}
	else
	{
		exeState->prevNode = this;
		retNode = store(tdbb, request, ALL_TRIGS);

		if (!subStore && exeState->whichStoTrig == PRE_TRIG)
			exeState->whichStoTrig = POST_TRIG;
	}

	return retNode;
}

// Execute a STORE statement.
const StmtNode* StoreNode::store(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const
{
	Jrd::Attachment* attachment = tdbb->getAttachment();
	jrd_tra* transaction = request->req_transaction;
	impure_state* impure = request->getImpure<impure_state>(impureOffset);

	const USHORT stream = relationSource->getStream();
	record_param* rpb = &request->req_rpb[stream];
	jrd_rel* relation = rpb->rpb_relation;

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			if (request->req_records_affected.isReadOnly() && !request->req_records_affected.hasCursor())
				request->req_records_affected.clear();

			request->req_records_affected.bumpModified(false);
			impure->sta_state = 0;
			RLCK_reserve_relation(tdbb, transaction, relation, true);
			break;

		case jrd_req::req_return:
			if (impure->sta_state)
				return parentStmt;

			if (transaction != attachment->getSysTransaction())
				++transaction->tra_save_point->sav_verb_count;

			if (relation->rel_pre_store && whichTrig != POST_TRIG)
			{
				EXE_execute_triggers(tdbb, &relation->rel_pre_store, NULL, rpb,
					jrd_req::req_trigger_insert, PRE_TRIG);
			}

			if (validations.hasData())
				validateExpressions(tdbb, validations);

			// For optimum on-disk record compression, zero all unassigned
			// fields. In addition, zero the tail of assigned varying fields
			// so that previous remnants don't defeat compression efficiency.

			// CVC: The code that was here was moved to its own routine: cleanupRpb()
			// and replaced by the call shown below.

			cleanupRpb(tdbb, rpb);

			if (relation->rel_file)
				EXT_store(tdbb, rpb);
			else if (relation->isVirtual())
				VirtualTable::store(tdbb, rpb);
			else if (!relation->rel_view_rse)
			{
				USHORT badIndex;
				jrd_rel* badRelation = NULL;

				VIO_store(tdbb, rpb, transaction);
				const idx_e errorCode = IDX_store(tdbb, rpb, transaction, &badRelation, &badIndex);

				if (errorCode)
					ERR_duplicate_error(errorCode, badRelation, badIndex);
			}

			rpb->rpb_number.setValid(true);

			if (relation->rel_post_store && whichTrig != PRE_TRIG)
			{
				EXE_execute_triggers(tdbb, &relation->rel_post_store, NULL, rpb,
					jrd_req::req_trigger_insert, POST_TRIG);
			}

			// CVC: Increment the counter only if we called VIO/EXT_store() and we were successful.
			if (!(request->req_view_flags & req_first_store_return))
			{
				request->req_view_flags |= req_first_store_return;
				if (relation->rel_view_rse)
					request->req_top_view_store = relation;
			}

			if (relation == request->req_top_view_store)
			{
				if (whichTrig == ALL_TRIGS || whichTrig == POST_TRIG)
				{
					request->req_records_inserted++;
					request->req_records_affected.bumpModified(true);
				}
			}
			else if (relation->rel_file || !relation->rel_view_rse)
			{
				request->req_records_inserted++;
				request->req_records_affected.bumpModified(true);
			}

			if (transaction != attachment->getSysTransaction())
				--transaction->tra_save_point->sav_verb_count;

			if (statement2)
			{
				impure->sta_state = 1;
				request->req_operation = jrd_req::req_evaluate;
				return statement2;
			}

		default:
			return parentStmt;
	}

	// Fall thru on evaluate to set up for store before executing sub-statement.
	// This involves finding the appropriate format, making sure a record block
	// exists for the stream and is big enough, and initialize all null flags
	// to "missing."

	const Format* format = MET_current(tdbb, relation);
	Record* record = VIO_record(tdbb, rpb, format, tdbb->getDefaultPool());

	rpb->rpb_address = record->rec_data;
	rpb->rpb_length = format->fmt_length;
	rpb->rpb_format_number = format->fmt_version;

	// CVC: This small block added by Ann Harrison to
	// start with a clean empty buffer and so avoid getting
	// new record buffer with misleading information. Fixes
	// bug with incorrect blob sharing during insertion in
	// a stored procedure.

	memset(record->rec_data, 0, rpb->rpb_length);

	// Initialize all fields to missing

	SSHORT n = (format->fmt_count + 7) >> 3;
	if (n)
		memset(record->rec_data, 0xFF, n);

	return statement;
}


//--------------------


static RegisterNode<UserSavepointNode> regUserSavepointNode(blr_user_savepoint);

DmlNode* UserSavepointNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	UserSavepointNode* node = FB_NEW(pool) UserSavepointNode(pool);

	node->command = (Command) csb->csb_blr_reader.getByte();
	PAR_name(csb, node->name);

	return node;
}

UserSavepointNode* UserSavepointNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
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

void UserSavepointNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "UserSavepointNode";
}

void UserSavepointNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_user_savepoint);
	dsqlScratch->appendUChar((UCHAR) command);
	dsqlScratch->appendNullString(name.c_str());
}

UserSavepointNode* UserSavepointNode::pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
{
	return this;
}

UserSavepointNode* UserSavepointNode::pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
{
	return this;
}

const StmtNode* UserSavepointNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
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

	return parentStmt;
}


//--------------------


static RegisterNode<SelectNode> regSelectNode(blr_select);

DmlNode* SelectNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	SelectNode* node = FB_NEW(pool) SelectNode(pool);

	while (csb->csb_blr_reader.peekByte() != blr_end)
	{
		if (csb->csb_blr_reader.peekByte() != blr_receive)
			PAR_syntax_error(csb, "blr_receive");
		node->statements.add(PAR_parse_stmt(tdbb, csb));
	}

	csb->csb_blr_reader.getByte();	// skip blr_end

	return node;
}

SelectNode* SelectNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void SelectNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SelectNode";
}

void SelectNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

SelectNode* SelectNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
		doPass1(tdbb, csb, i->getAddress());
	return this;
}

SelectNode* SelectNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
		doPass2(tdbb, csb, i->getAddress(), this);
	return this;
}

// Execute a SELECT statement. This is more than a little obscure.
// We first set up the SELECT statement as the "message" and stall on receive (waiting for user send).
// EXE_send will then loop thru the sub-statements of select looking for the appropriate RECEIVE
// statement. When (or if) it finds it, it will set it up the next statement to be executed.
// The RECEIVE, then, will be entered with the operation "req_proceed".
const StmtNode* SelectNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			request->req_message = this;
			request->req_operation = jrd_req::req_receive;
			request->req_flags |= req_stall;
			return this;

		default:
			return parentStmt;
	}
}


//--------------------


static RegisterNode<SetGeneratorNode> regSetGeneratorNode(blr_set_generator);

DmlNode* SetGeneratorNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	MetaName name;
	PAR_name(csb, name);

	SetGeneratorNode* node = FB_NEW(pool) SetGeneratorNode(pool);

	node->genId = MET_lookup_generator(tdbb, name.c_str());
	if (node->genId < 0)
		PAR_error(csb, Arg::Gds(isc_gennotdef) << Arg::Str(name));

	node->value = PAR_parse_value(tdbb, csb);

	return node;
}

SetGeneratorNode* SetGeneratorNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void SetGeneratorNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SetGeneratorNode";
}

void SetGeneratorNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

SetGeneratorNode* SetGeneratorNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, value.getAddress());
	return this;
}

SetGeneratorNode* SetGeneratorNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, value.getAddress());
	return this;
}

const StmtNode* SetGeneratorNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	if (request->req_operation == jrd_req::req_evaluate)
	{
		jrd_tra* transaction = request->req_transaction;

		MetaName genName;
		MET_lookup_generator_id(tdbb, genId, genName);

		DdlNode::executeDdlTrigger(tdbb, transaction, DdlNode::DTW_BEFORE,
			DDL_TRIGGER_ALTER_SEQUENCE, genName, *request->getStatement()->sqlText);

		dsc* desc = EVL_expr(tdbb, request, value);
		DPM_gen_id(tdbb, genId, true, MOV_get_int64(desc, 0));

		DdlNode::executeDdlTrigger(tdbb, transaction, DdlNode::DTW_AFTER,
			DDL_TRIGGER_ALTER_SEQUENCE, genName, *request->getStatement()->sqlText);

		request->req_operation = jrd_req::req_return;
	}

	return parentStmt;
}


//--------------------


static RegisterNode<StallNode> regStallNode(blr_stall);

DmlNode* StallNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* /*csb*/, UCHAR /*blrOp*/)
{
	StallNode* node = FB_NEW(pool) StallNode(pool);
	return node;
}

StallNode* StallNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void StallNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "StallNode";
}

void StallNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

StallNode* StallNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	return this;
}

StallNode* StallNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	return this;
}

// Execute a stall statement. This is like a blr_receive, except that there is no need for a
// gds__send () from the user (i.e. EXE_send () in the engine).
// A gds__receive () will unblock the user.
const StmtNode* StallNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_sync:
			return parentStmt;

		case jrd_req::req_proceed:
			request->req_operation = jrd_req::req_return;
			return parentStmt;

		default:
			request->req_message = this;
			request->req_operation = jrd_req::req_return;
			request->req_flags |= req_stall;
			return this;
	}
}


//--------------------


static RegisterNode<SuspendNode> regSuspendNode(blr_send);

DmlNode* SuspendNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	SuspendNode* node = FB_NEW(pool) SuspendNode(pool);

	USHORT n = csb->csb_blr_reader.getByte();
	node->message = csb->csb_rpt[n].csb_message;
	node->statement = PAR_parse_stmt(tdbb, csb);

	return node;
}

SuspendNode* SuspendNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
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

	return this;
}

void SuspendNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SuspendNode";
}

void SuspendNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->genReturn();
}

SuspendNode* SuspendNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, statement.getAddress());
	return this;
}

SuspendNode* SuspendNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	doPass2(tdbb, csb, statement.getAddress(), this);
	return this;
}

// Execute a SEND statement.
const StmtNode* SuspendNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
	case jrd_req::req_evaluate:
		return statement;

	case jrd_req::req_return:
		request->req_operation = jrd_req::req_send;
		request->req_message = message;
		request->req_flags |= req_stall;
		return this;

	case jrd_req::req_proceed:
		request->req_operation = jrd_req::req_return;
		return parentStmt;

	default:
		return parentStmt;
	}
}


//--------------------


ReturnNode* ReturnNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
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
	node->value = PASS1_node(dsqlScratch, value);

	return node;
}

void ReturnNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ReturnNode";
	nodes.add(value);
}

void ReturnNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_begin);
	dsqlScratch->appendUChar(blr_assignment);
	GEN_expr(dsqlScratch, value);
	dsqlScratch->appendUChar(blr_variable);
	dsqlScratch->appendUShort(0);
	dsqlScratch->genReturn();
	dsqlScratch->appendUChar(blr_leave);
	dsqlScratch->appendUChar(0);
	dsqlScratch->appendUChar(blr_end);
}


//--------------------


static RegisterNode<SavePointNode> regSavePointNodeStart(blr_start_savepoint);
static RegisterNode<SavePointNode> regSavePointNodeEnd(blr_end_savepoint);

DmlNode* SavePointNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	SavePointNode* node = FB_NEW(pool) SavePointNode(pool, blrOp);
	return node;
}

SavePointNode* SavePointNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

void SavePointNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "SavePointNode";
}

void SavePointNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}

const StmtNode* SavePointNode::execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const
{
	jrd_tra* transaction = request->req_transaction;
	jrd_tra* sysTransaction = request->req_attachment->getSysTransaction();

	switch (blrOp)
	{
		case blr_start_savepoint:
			if (request->req_operation == jrd_req::req_evaluate)
			{
				// Start a save point.
				if (transaction != sysTransaction)
					VIO_start_save_point(tdbb, transaction);
			}
			break;

		case blr_end_savepoint:
			if (request->req_operation == jrd_req::req_evaluate ||
				request->req_operation == jrd_req::req_unwind)
			{
				// If any requested modify/delete/insert ops have completed, forget them.
				if (transaction != sysTransaction)
				{
					// If an error is still pending when the savepoint is supposed to end, then the
					// application didn't handle the error and the savepoint should be undone.
					if (exeState->errorPending)
						++transaction->tra_save_point->sav_verb_count;
					EXE_verb_cleanup(tdbb, transaction);
				}
			}
			break;
	}

	request->req_operation = jrd_req::req_return;
	return parentStmt;
}


//--------------------


// Perform cleaning of rpb, zeroing unassigned fields and the impure tail of varying fields that
// we don't want to carry when the RLE algorithm is applied.
static void cleanupRpb(thread_db* tdbb, record_param* rpb)
{
	Record* record = rpb->rpb_record;
	const Format* format = record->rec_format;

	SET_TDBB(tdbb); // Is it necessary?

	/*
    Starting from the format, walk through its
    array of descriptors.  If the descriptor has
    no address, its a computed field and we shouldn't
    try to fix it.  Get a pointer to the actual data
    and see if that field is null by indexing into
    the null flags between the record header and the
    record data.
	*/

	for (USHORT n = 0; n < format->fmt_count; n++)
	{
		const dsc* desc = &format->fmt_desc[n];

		if (!desc->dsc_address)
			continue;

		UCHAR* const p = record->rec_data + (IPTR) desc->dsc_address;

		if (TEST_NULL(record, n))
		{
			USHORT length = desc->dsc_length;

			if (length)
				memset(p, 0, length);
		}
		else if (desc->dsc_dtype == dtype_varying)
		{
			vary* varying = reinterpret_cast<vary*>(p);
			USHORT length = desc->dsc_length - sizeof(USHORT);

			if (length > varying->vary_length)
			{
				char* trail = varying->vary_string + varying->vary_length;
				length -= varying->vary_length;
				memset(trail, 0, length);
			}
		}
	}
}

// Build a validation list for a relation, if appropriate.
static void makeValidation(thread_db* tdbb, CompilerScratch* csb, USHORT stream,
	Array<ValidateInfo>& validations)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return;

	UCHAR local_map[JrdStatement::MAP_LENGTH];
	UCHAR* map = csb->csb_rpt[stream].csb_map;
	if (!map)
	{
		map = local_map;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR still relevant for the bitmap?
		map[0] = (UCHAR) stream;
	}

	USHORT fieldId = 0;
	vec<jrd_fld*>::iterator ptr1 = vector->begin();

	for (const vec<jrd_fld*>::const_iterator end = vector->end(); ptr1 < end; ++ptr1, ++fieldId)
	{
		BoolExprNode* validation;
		StmtNode* validationStmt;

		if (*ptr1 && (validation = (*ptr1)->fld_validation))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			RemapFieldNodeCopier copier(csb, map, fieldId);

			if ((validationStmt = (*ptr1)->fld_validation_stmt))
				validationStmt = copier.copy(tdbb, validationStmt);

			validation = copier.copy(tdbb, validation);

			ValidateInfo validate;
			validate.stmt = validationStmt;
			validate.boolean = validation;
			validate.value = PAR_gen_field(tdbb, stream, fieldId);
			validations.add(validate);
		}

		if (*ptr1 && (validation = (*ptr1)->fld_not_null))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			RemapFieldNodeCopier copier(csb, map, fieldId);

			if ((validationStmt = (*ptr1)->fld_not_null_stmt))
				validationStmt = copier.copy(tdbb, validationStmt);

			validation = copier.copy(tdbb, validation);

			ValidateInfo validate;
			validate.stmt = validationStmt;
			validate.boolean = validation;
			validate.value = PAR_gen_field(tdbb, stream, fieldId);
			validations.add(validate);
		}
	}
}

// Process a view update performed by a trigger.
static StmtNode* pass1ExpandView(thread_db* tdbb, CompilerScratch* csb, USHORT orgStream,
	USHORT newStream, bool remap)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	StmtNodeStack stack;
	jrd_rel* relation = csb->csb_rpt[orgStream].csb_relation;
	vec<jrd_fld*>* fields = relation->rel_fields;

	dsc desc;
	USHORT id = 0, newId = 0;
	vec<jrd_fld*>::iterator ptr = fields->begin();

	for (const vec<jrd_fld*>::const_iterator end = fields->end(); ptr < end; ++ptr, ++id)
	{
		if (*ptr)
		{
			if (remap)
			{
				const jrd_fld* field = MET_get_field(relation, id);

				if (field->fld_source)
					newId = field->fld_source->as<FieldNode>()->fieldId;
				else
					newId = id;
			}
			else
				newId = id;

			ValueExprNode* node = PAR_gen_field(tdbb, newStream, newId);
			node->getDesc(tdbb, csb, &desc);

			if (!desc.dsc_address)
			{
				delete node;
				continue;
			}

			AssignmentNode* assign = FB_NEW(*tdbb->getDefaultPool()) AssignmentNode(
				*tdbb->getDefaultPool());
			assign->asgnTo = node;
			assign->asgnFrom = PAR_gen_field(tdbb, orgStream, id);

			stack.push(assign);
		}
	}

	return PAR_make_list(tdbb, stack);
}

// Check out a prospective update to a relation. If it fails security check, bounce it.
// If it's a view update, make sure the view is updatable, and return the view source for redirection.
// If it's a simple relation, return NULL.
static RelationSourceNode* pass1Update(thread_db* tdbb, CompilerScratch* csb, jrd_rel* relation,
	const trig_vec* trigger, USHORT stream, USHORT updateStream, SecurityClass::flags_t priv,
	jrd_rel* view, USHORT viewStream)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(relation, type_rel);
	DEV_BLKCHK(view, type_rel);

	// unless this is an internal request, check access permission

	CMP_post_access(tdbb, csb, relation->rel_security_name, (view ? view->rel_id : 0),
		priv, SCL_object_table, relation->rel_name);

	// ensure that the view is set for the input streams,
	// so that access to views can be checked at the field level

	fb_assert(viewStream <= MAX_STREAMS);
	CMP_csb_element(csb, stream)->csb_view = view;
	CMP_csb_element(csb, stream)->csb_view_stream = viewStream;
	CMP_csb_element(csb, updateStream)->csb_view = view;
	CMP_csb_element(csb, updateStream)->csb_view_stream = viewStream;

	// if we're not a view, everything's cool

	RseNode* rse = relation->rel_view_rse;
	if (!rse)
		return NULL;

	// a view with triggers is always updatable

	if (trigger)
	{
		bool userTriggers = false;

		for (size_t i = 0; i < trigger->getCount(); i++)
		{
			if (!(*trigger)[i].sys_trigger)
			{
				userTriggers = true;
				break;
			}
		}

		if (userTriggers)
		{
			csb->csb_rpt[updateStream].csb_flags |= csb_view_update;
			return NULL;
		}
	}

	// we've got a view without triggers, let's check whether it's updateable

	if (rse->rse_relations.getCount() != 1 || rse->rse_projection || rse->rse_sorted ||
		rse->rse_relations[0]->type != RelationSourceNode::TYPE)
	{
		ERR_post(Arg::Gds(isc_read_only_view) << Arg::Str(relation->rel_name));
	}

	// for an updateable view, return the view source

	csb->csb_rpt[updateStream].csb_flags |= csb_view_update;

	return static_cast<RelationSourceNode*>(rse->rse_relations[0].getObject());
}

// The csb->csb_validate_expr becomes true if an ancestor of the current node (the one being
// passed in) in the parse tree is a validation. "Ancestor" does not include the current node
// being passed in as an argument.
// If we are in a "validate subtree" (as determined by the csb->csb_validate_expr), we must not
// post update access to the fields involved in the validation clause.
// (See the call for CMP_post_access in this function.)
static void pass1Validations(thread_db* tdbb, CompilerScratch* csb, Array<ValidateInfo>& validations)
{
	AutoSetRestore<bool> autoValidateExpr(&csb->csb_validate_expr, true);

	for (Array<ValidateInfo>::iterator i = validations.begin(); i != validations.end(); ++i)
	{
		DmlNode::doPass1(tdbb, csb, i->stmt.getAddress());
		DmlNode::doPass1(tdbb, csb, i->boolean.getAddress());
		DmlNode::doPass1(tdbb, csb, i->value.getAddress());
	}
}

// Inherit access to triggers to be fired.
//
// When we detect that a trigger could be fired by a request,
// then we add the access list for that trigger to the access
// list for this request.  That way, when we check access for
// the request we also check access for any other objects that
// could be fired off by the request.
//
// Note that when we add the access item, we specify that
//    Trigger X needs access to resource Y.
// In the access list we parse here, if there is no "accessor"
// name then the trigger must access it directly.  If there is
// an "accessor" name, then something accessed by this trigger
// must require the access.
//
// CVC: The third parameter is the owner of the triggers vector
// and was added to avoid triggers posting access checks to
// their base tables, since it's nonsense and causes weird
// messages about false REFERENCES right failures.
static void postTriggerAccess(CompilerScratch* csb, jrd_rel* ownerRelation,
	ExternalAccess::exa_act operation, jrd_rel* view)
{
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	// allow all access to internal requests
	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
		return;

	// Post trigger access
	ExternalAccess temp(operation, ownerRelation->rel_id, view ? view->rel_id : 0);
	size_t i;

	if (!csb->csb_external.find(temp, i))
		csb->csb_external.insert(i, temp);
}

// Perform operation's pre-triggers, storing active rpb in chain.
static void preModifyEraseTriggers(thread_db* tdbb, trig_vec** trigs,
	StmtNode::WhichTrigger whichTrig, record_param* rpb, record_param* rec, jrd_req::req_ta op)
{
	if (!tdbb->getTransaction()->tra_rpblist)
	{
		tdbb->getTransaction()->tra_rpblist =
			FB_NEW(*tdbb->getTransaction()->tra_pool) traRpbList(*tdbb->getTransaction()->tra_pool);
	}

	const int rpblevel = tdbb->getTransaction()->tra_rpblist->PushRpb(rpb);

	if (*trigs && whichTrig != StmtNode::POST_TRIG)
	{
		try
		{
			EXE_execute_triggers(tdbb, trigs, rpb, rec, op, StmtNode::PRE_TRIG);
		}
		catch (const Exception&)
		{
			tdbb->getTransaction()->tra_rpblist->PopRpb(rpb, rpblevel);
			throw;
		}
	}

	tdbb->getTransaction()->tra_rpblist->PopRpb(rpb, rpblevel);
}

// Execute a list of validation expressions.
static void validateExpressions(thread_db* tdbb, const Array<ValidateInfo>& validations)
{
	SET_TDBB(tdbb);

	Array<ValidateInfo>::const_iterator end = validations.end();

	for (Array<ValidateInfo>::const_iterator i = validations.begin(); i != end; ++i)
	{
		jrd_req* request = tdbb->getRequest();

		if (i->stmt)
			EXE_looper(tdbb, request, i->stmt.getObject(), true);

		if (!i->boolean->execute(tdbb, request) && !(request->req_flags & req_null))
		{
			// Validation error -- report result
			const char* value;
			VaryStr<128> temp;

			const dsc* desc = EVL_expr(tdbb, request, i->value);
			const USHORT length = (desc && !(request->req_flags & req_null)) ?
				MOV_make_string(desc, ttype_dynamic, &value, &temp, sizeof(temp) - 1) : 0;

			if (!desc || (request->req_flags & req_null))
				value = NULL_STRING_MARK;
			else if (!length)
				value = "";
			else
				const_cast<char*>(value)[length] = 0;	// safe cast - data is actually on the stack

			const TEXT*	name = NULL;
			const FieldNode* fieldNode = i->value->as<FieldNode>();

			if (fieldNode)
			{
				const jrd_rel* relation = request->req_rpb[fieldNode->fieldStream].rpb_relation;
				const vec<jrd_fld*>* vector = relation->rel_fields;
				const jrd_fld* field;

				if (vector && fieldNode->fieldId < vector->count() &&
					(field = (*vector)[fieldNode->fieldId]))
				{
					name = field->fld_name.c_str();
				}
			}

			if (!name)
				name = UNKNOWN_STRING_MARK;

			ERR_post(Arg::Gds(isc_not_valid) << Arg::Str(name) << Arg::Str(value));
		}
	}
}


}	// namespace Jrd
