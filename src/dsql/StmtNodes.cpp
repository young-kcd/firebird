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
#include "../dsql/BoolNodes.h"
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
#include "../dsql/metd_proto.h"
#include "../jrd/vio_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/pass1_proto.h"

using namespace Firebird;
using namespace Jrd;


namespace Jrd {

static dsql_nod* dsqlExplodeFields(dsql_rel*);
static dsql_par* dsqlFindDbKey(const dsql_req*, const dsql_nod*);
static dsql_par* dsqlFindRecordVersion(const dsql_req*, const dsql_nod*);
static const dsql_msg* dsqlGenDmlHeader(DsqlCompilerScratch*, dsql_nod*);
static dsql_ctx* dsqlGetContext(const dsql_nod* node);
static void dsqlGetContexts(DsqlContextStack& contexts, const dsql_nod* node);
static StmtNode* dsqlNullifyReturning(DsqlCompilerScratch*, StmtNode* input, bool returnList);
static void dsqlFieldAppearsOnce(const Array<dsql_nod*>&, const char*);
static Array<dsql_nod*>* dsqlPassArray(DsqlCompilerScratch*, Array<dsql_nod*>*);
static dsql_ctx* dsqlPassCursorContext(DsqlCompilerScratch*, const dsql_nod*, const dsql_nod*);
static dsql_nod* dsqlPassCursorReference(DsqlCompilerScratch*, const dsql_nod*, dsql_nod*);
static dsql_nod* dsqlPassHiddenVariable(DsqlCompilerScratch* dsqlScratch, dsql_nod* expr);
static StmtNode* dsqlProcessReturning(DsqlCompilerScratch*, ReturningClause*, StmtNode*);
static void dsqlSetParameterName(dsql_nod*, const dsql_nod*, const dsql_rel*);
static void dsqlSetParametersName(CompoundStmtNode*, const dsql_nod*);

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

}	// namespace Jrd


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
		virtual USHORT getFieldId(const FieldNode* field)
		{
			if (field->byId && field->fieldId == 0 && field->fieldStream == 0)
				return fldId;

			return NodeCopier::getFieldId(field);
		}

	private:
		USHORT fldId;
	};

	class ReturningProcessor
	{
	public:
		// Play with contexts for RETURNING purposes.
		// Its assumed that oldContext is already on the stack.
		// Changes oldContext name to "OLD".
		ReturningProcessor(DsqlCompilerScratch* aScratch, dsql_ctx* oldContext, dsql_ctx* modContext)
			: scratch(aScratch),
			  autoAlias(&oldContext->ctx_alias, OLD_CONTEXT),
			  autoInternalAlias(&oldContext->ctx_internal_alias, oldContext->ctx_alias),
			  autoFlags(&oldContext->ctx_flags, oldContext->ctx_flags | CTX_system | CTX_returning),
			  hasModContext(modContext != NULL)
		{
			// Clone the modify/old context and push with name "NEW" in a greater scope level.

			dsql_ctx* newContext = FB_NEW(scratch->getPool()) dsql_ctx(scratch->getPool());

			if (modContext)
			{
				// push the modify context in the same scope level
				scratch->context->push(modContext);
				*newContext = *modContext;
				newContext->ctx_flags |= CTX_system;
			}
			else
			{
				// This is NEW in the context of a DELETE. Mark it as NULL.
				*newContext = *oldContext;
				newContext->ctx_flags |= CTX_null;

				// Remove the system flag, so unqualified fields could be resolved to this context.
				oldContext->ctx_flags &= ~CTX_system;
			}

			newContext->ctx_alias = newContext->ctx_internal_alias =
				MAKE_cstring(NEW_CONTEXT)->str_data;
			newContext->ctx_flags |= CTX_returning;
			scratch->context->push(newContext);
		}

		~ReturningProcessor()
		{
			// Restore the context stack.
			scratch->context->pop();
			if (hasModContext)
				scratch->context->pop();
		}

		// Process the RETURNING clause.
		StmtNode* process(ReturningClause* node, StmtNode* stmtNode)
		{
			return dsqlProcessReturning(scratch, node, stmtNode);
		}

		// Clone a RETURNING node without create duplicate parameters.
		static StmtNode* clone(DsqlCompilerScratch* scratch, ReturningClause* unprocessed, StmtNode* processed)
		{
			if (!processed)
				return NULL;

			// nod_returning was already processed
			CompoundStmtNode* processedStmt = processed->as<CompoundStmtNode>();
			fb_assert(processed);

			// And we create a RETURNING node where the targets are already processed.
			CompoundStmtNode* newNode =
				FB_NEW(scratch->getPool()) CompoundStmtNode(scratch->getPool());

			dsql_nod** srcPtr = unprocessed->first->nod_arg;
			NestConst<StmtNode>* dstPtr = processedStmt->statements.begin();

			for (const dsql_nod* const* const end = srcPtr + unprocessed->first->nod_count;
				 srcPtr != end;
				 ++srcPtr, ++dstPtr)
			{
				AssignmentNode* temp = FB_NEW(scratch->getPool()) AssignmentNode(scratch->getPool());
				temp->dsqlAsgnFrom = *srcPtr;
				temp->dsqlAsgnTo = (*dstPtr)->as<AssignmentNode>()->dsqlAsgnTo;
				newNode->statements.add(temp);
			}

			return newNode;
		}

	private:
		DsqlCompilerScratch* scratch;
		AutoSetRestore<string> autoAlias;
		AutoSetRestore<string> autoInternalAlias;
		AutoSetRestore<USHORT> autoFlags;
		bool hasModContext;
	};
}	// namespace


//--------------------


namespace Jrd {


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

DmlNode* AssignmentNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	AssignmentNode* node = FB_NEW(pool) AssignmentNode(pool);
	node->asgnFrom = PAR_parse_value(tdbb, csb);
	node->asgnTo = PAR_parse_value(tdbb, csb);
	return node;
}

AssignmentNode* AssignmentNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	AssignmentNode* node = FB_NEW(getPool()) AssignmentNode(getPool());
	node->dsqlAsgnFrom = PASS1_node(dsqlScratch, dsqlAsgnFrom);
	node->dsqlAsgnTo = PASS1_node(dsqlScratch, dsqlAsgnTo);

	// Try to force dsqlAsgnFrom to be same type as dsqlAsgnTo eg: ? = FIELD case
	PASS1_set_parameter_type(dsqlScratch, node->dsqlAsgnFrom, node->dsqlAsgnTo, false);

	// Try to force dsqlAsgnTo to be same type as dsqlAsgnFrom eg: FIELD = ? case
	// Try even when the above call succeeded, because "dsqlAsgnTo" may
	// have sub-expressions that should be resolved.
	PASS1_set_parameter_type(dsqlScratch, node->dsqlAsgnTo, node->dsqlAsgnFrom, false);

	return node;
}

void AssignmentNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "AssignmentNode";
}

void AssignmentNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_assignment);
	GEN_expr(dsqlScratch, dsqlAsgnFrom);
	GEN_expr(dsqlScratch, dsqlAsgnTo);
}

AssignmentNode* AssignmentNode::copy(thread_db* tdbb, NodeCopier& copier) const
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

StmtNode* BlockNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	if (!handlers && !dsqlScratch->errorHandlers)
	{
		CompoundStmtNode* node = FB_NEW(getPool()) CompoundStmtNode(getPool());
		node->statements.add(action->dsqlPass(dsqlScratch));
		return node;
	}

	BlockNode* node = FB_NEW(getPool()) BlockNode(getPool());

	if (handlers)
		++dsqlScratch->errorHandlers;

	node->action = action->dsqlPass(dsqlScratch);

	if (handlers)
	{
		node->handlers = handlers->dsqlPass(dsqlScratch);
		--dsqlScratch->errorHandlers;
	}

	return node;
}

void BlockNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "BlockNode";
	// print handlers, too?
}

void BlockNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_block);
	action->genBlr(dsqlScratch);

	if (handlers)
	{
		const NestConst<StmtNode>* const end = handlers->statements.end();

		for (NestConst<StmtNode>* ptr = handlers->statements.begin(); ptr != end; ++ptr)
			(*ptr)->genBlr(dsqlScratch);
	}

	dsqlScratch->appendUChar(blr_end);
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
					const ExceptionArray& xcpNode = handlerNode->conditions;

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

							// Re-assign the transaction pointer, as the active transaction
							// could change in the meantime (inside the looper)
							transaction = request->req_transaction;

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
bool BlockNode::testAndFixupError(thread_db* tdbb, jrd_req* request, const ExceptionArray& conditions)
{
	if (tdbb->tdbb_flags & TDBB_sys_error)
		return false;

	ISC_STATUS* statusVector = tdbb->tdbb_status_vector;
	const SSHORT sqlcode = gds__sqlcode(statusVector);

	bool found = false;

	for (USHORT i = 0; i < conditions.getCount(); i++)
	{
		switch (conditions[i].type)
		{
			case ExceptionItem::SQL_CODE:
				if (sqlcode == conditions[i].code)
					found = true;
				break;

			case ExceptionItem::GDS_CODE:
				if (statusVector[1] == conditions[i].code)
					found = true;
				break;

			case ExceptionItem::XCP_CODE:
				// Look at set_error() routine to understand how the
				// exception ID info is encoded inside the status vector.
				if ((statusVector[1] == isc_except) &&
					(statusVector[3] == conditions[i].code))
				{
					found = true;
				}

				break;

			case ExceptionItem::XCP_DEFAULT:
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
	CompoundStmtNode* node = FB_NEW(getPool()) CompoundStmtNode(getPool());

	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
	{
		StmtNode* ptr = *i;
		ptr = ptr->dsqlPass(dsqlScratch);
		node->statements.add(ptr);
	}

	return node;
}

void CompoundStmtNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "CompoundStmtNode";
}

void CompoundStmtNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_begin);

	for (NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i)
		(*i)->genBlr(dsqlScratch);

	dsqlScratch->appendUChar(blr_end);
}

CompoundStmtNode* CompoundStmtNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	CompoundStmtNode* node = FB_NEW(*tdbb->getDefaultPool()) CompoundStmtNode(*tdbb->getDefaultPool());
	node->onlyAssignments = onlyAssignments;

	NestConst<StmtNode>* j = node->statements.getBuffer(statements.getCount());

	for (const NestConst<StmtNode>* i = statements.begin(); i != statements.end(); ++i, ++j)
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
	const char* cmd = blrOp == blr_continue_loop ? "CONTINUE" : "BREAK/LEAVE";

	if (!dsqlScratch->loopLevel)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
			// Token unknown
			Arg::Gds(isc_token_err) <<
			Arg::Gds(isc_random) << cmd);
	}

	dsqlLabel = PASS1_label(dsqlScratch, true, dsqlLabel);

	return this;
}

void ContinueLeaveNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ContinueLeaveNode";
}

void ContinueLeaveNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	dsqlScratch->appendUChar((int)(IPTR) dsqlLabel->nod_arg[Dsql::e_label_number]);
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
	CursorStmtNode* node = FB_NEW(pool) CursorStmtNode(pool, csb->csb_blr_reader.getByte());
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
	// Verify if we're in an autonomous transaction.
	if (dsqlScratch->flags & DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK)
	{
		const char* stmt = NULL;

		switch (cursorOp)
		{
			case blr_cursor_open:
				stmt = "OPEN CURSOR";
				break;

			case blr_cursor_close:
				stmt = "CLOSE CURSOR";
				break;

			case blr_cursor_fetch:
			case blr_cursor_fetch_scroll:
				stmt = "FETCH CURSOR";
				break;
		}

		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_unsupported_in_auto_trans) << Arg::Str(stmt));
	}

	// Resolve the cursor.
	cursorNumber = PASS1_cursor_name(dsqlScratch, dsqlName,
		DeclareCursorNode::CUR_TYPE_EXPLICIT, true)->cursorNumber;

	// Process a scroll node, if exists.
	if (dsqlScrollExpr)
		dsqlScrollExpr = PASS1_node(dsqlScratch, dsqlScrollExpr);

	// Process an assignment node, if exists.
	dsqlIntoStmt = dsqlPassArray(dsqlScratch, dsqlIntoStmt);

	return this;
}

void CursorStmtNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "CursorStmtNode";
}

void CursorStmtNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_cursor_stmt);
	dsqlScratch->appendUChar(cursorOp);	// open, close, fetch [scroll]
	dsqlScratch->appendUShort(cursorNumber);

	if (cursorOp == blr_cursor_fetch_scroll)
	{
		dsqlScratch->appendUChar(scrollOp);

		if (dsqlScrollExpr)
			GEN_expr(dsqlScratch, dsqlScrollExpr);
		else
			dsqlScratch->appendUChar(blr_null);
	}

	DeclareCursorNode* cursor = NULL;

	for (Array<DeclareCursorNode*>::iterator itr = dsqlScratch->cursors.begin();
		 itr != dsqlScratch->cursors.end();
		 ++itr)
	{
		if ((*itr)->cursorNumber == cursorNumber)
			cursor = *itr;
	}

	fb_assert(cursor);

	// Assignment.

	if (dsqlIntoStmt)
	{
		dsql_nod* list = ExprNode::as<RseNode>(cursor->dsqlRse)->dsqlSelectList;

		if (list->nod_count != dsqlIntoStmt->getCount())
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-313) <<
					  Arg::Gds(isc_dsql_count_mismatch));
		}

		dsqlScratch->appendUChar(blr_begin);

		dsql_nod** ptr = list->nod_arg;
		dsql_nod** end = ptr + list->nod_count;
		dsql_nod** ptr_to = dsqlIntoStmt->begin();

		while (ptr != end)
		{
			dsqlScratch->appendUChar(blr_assignment);
			GEN_expr(dsqlScratch, *ptr++);
			GEN_expr(dsqlScratch, *ptr_to++);
		}

		dsqlScratch->appendUChar(blr_end);
	}
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
	fb_assert(dsqlCursorType != CUR_TYPE_NONE);

	// Make sure the cursor doesn't exist.
	PASS1_cursor_name(dsqlScratch, dsqlName, CUR_TYPE_ALL, false);

	// Temporarily hide unnecessary contexts and process our RSE.
	DsqlContextStack* const baseContext = dsqlScratch->context;
	DsqlContextStack temp;
	dsqlScratch->context = &temp;
	const dsql_nod* select = dsqlRse;
	dsqlRse = PASS1_rse(dsqlScratch, select->nod_arg[Dsql::e_select_expr], select->nod_arg[Dsql::e_select_lock]);
	dsqlScratch->context->clear();
	dsqlScratch->context = baseContext;

	// Assign number and store in the dsqlScratch stack.
	cursorNumber = dsqlScratch->cursorNumber++;
	dsqlScratch->cursors.push(this);

	return this;
}

void DeclareCursorNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "DeclareCursorNode";
}

void DeclareCursorNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_dcl_cursor);
	dsqlScratch->appendUShort(cursorNumber);

	if (dsqlScroll)
		dsqlScratch->appendUChar(blr_scrollable);

	GEN_rse(dsqlScratch, dsqlRse);

	dsql_nod* temp = ExprNode::as<RseNode>(dsqlRse)->dsqlSelectList;
	dsql_nod** ptr = temp->nod_arg;
	dsql_nod** end = ptr + temp->nod_count;

	fb_assert(temp->nod_count < MAX_USHORT);
	dsqlScratch->appendUShort(temp->nod_count);

	while (ptr < end)
		GEN_expr(dsqlScratch, *ptr++);
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

DeclareVariableNode* DeclareVariableNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
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
	const USHORT n = csb->csb_blr_reader.getByte();

	if (n >= csb->csb_rpt.getCount() || !(csb->csb_rpt[n].csb_flags & csb_used))
		PAR_error(csb, Arg::Gds(isc_ctxnotdef));

	EraseNode* node = FB_NEW(pool) EraseNode(pool);
	node->stream = csb->csb_rpt[n].csb_stream;

	return node;
}

StmtNode* EraseNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	thread_db* tdbb = JRD_get_thread_data();

	const dsql_nod* cursor = dsqlCursor;
	dsql_nod* relation = dsqlRelation;

	EraseNode* node = FB_NEW(getPool()) EraseNode(getPool());

	if (cursor && dsqlScratch->isPsql())
	{
		node->dsqlContext = dsqlPassCursorContext(dsqlScratch, cursor, relation);
		node->statement = dsqlProcessReturning(dsqlScratch, dsqlReturning, statement);
		return SavepointEncloseNode::make(getPool(), dsqlScratch, node);
	}

	dsqlScratch->getStatement()->setType(
		cursor ? DsqlCompiledStatement::TYPE_DELETE_CURSOR : DsqlCompiledStatement::TYPE_DELETE);

	// Generate record selection expression.

	dsql_nod* rseNod;
	if (cursor)
		rseNod = dsqlPassCursorReference(dsqlScratch, cursor, relation);
	else
	{
		RseNode* rse = FB_NEW(getPool()) RseNode(getPool());

		rseNod = MAKE_node(Dsql::nod_class_exprnode, 1);
		rseNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(rse);

		dsql_nod* temp = rse->dsqlStreams = MAKE_node(Dsql::nod_list, 1);
		temp->nod_arg[0] = PASS1_node_psql(dsqlScratch, relation, false);

		if ((temp = dsqlBoolean))
			rse->dsqlWhere = PASS1_node_psql(dsqlScratch, temp, false);

		if ((temp = dsqlPlan))
			rse->dsqlPlan = PASS1_node_psql(dsqlScratch, temp, false);

		if ((temp = dsqlSort))
			rse->dsqlOrder = PASS1_sort(dsqlScratch, temp, NULL);

		if ((temp = dsqlRows))
		{
			PASS1_limit(dsqlScratch, temp->nod_arg[Dsql::e_rows_length],
				temp->nod_arg[Dsql::e_rows_skip], rse);
		}

		if (dsqlReturning || statement)
			rseNod->nod_flags |= NOD_SELECT_EXPR_SINGLETON;
	}

	node->dsqlRse = rseNod;
	node->dsqlRelation = ExprNode::as<RseNode>(rseNod)->dsqlStreams->nod_arg[0];

	node->statement = dsqlProcessReturning(dsqlScratch, dsqlReturning, statement);

	StmtNode* ret = dsqlNullifyReturning(dsqlScratch, node, true);

	dsqlScratch->context->pop();

	return SavepointEncloseNode::make(getPool(), dsqlScratch, ret);
}

void EraseNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "EraseNode";
}

void EraseNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	const dsql_msg* message = dsqlGenDmlHeader(dsqlScratch, dsqlRse);
	const dsql_ctx* context;

	if (dsqlContext)
	{
		context = dsqlContext;

		if (statement)
		{
			dsqlScratch->appendUChar(blr_begin);
			statement->genBlr(dsqlScratch);
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
			dsqlScratch->appendUChar(blr_end);
		}
		else
		{
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
		}
	}
	else
	{
		dsql_nod* temp = dsqlRelation;
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;

		if (statement)
		{
			dsqlScratch->appendUChar(blr_begin);
			statement->genBlr(dsqlScratch);
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
			dsqlScratch->appendUChar(blr_end);
		}
		else
		{
			dsqlScratch->appendUChar(blr_erase);
			GEN_stuff_context(dsqlScratch, context);
		}
	}

	if (message)
		dsqlScratch->appendUChar(blr_end);
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

	for (unsigned i = 0; i < n; i++)
	{
		const USHORT codeType = csb->csb_blr_reader.getByte();
		ExceptionItem& item = node->conditions.add();

		switch (codeType)
		{
			case blr_sql_code:
				item.type = ExceptionItem::SQL_CODE;
				item.code = (SSHORT) csb->csb_blr_reader.getWord();
				break;

			case blr_gds_code:
				item.type = ExceptionItem::GDS_CODE;
				PAR_name(csb, item.name);
				item.name.lower();
				if (!(item.code = PAR_symbol_to_gdscode(item.name)))
					PAR_error(csb, Arg::Gds(isc_codnotdef) << item.name);
				break;

			case blr_exception:
			{
				item.type = ExceptionItem::XCP_CODE;
				PAR_name(csb, item.name);
				if (!(item.code = MET_lookup_exception_number(tdbb, item.name)))
					PAR_error(csb, Arg::Gds(isc_xcpnotdef) << item.name);

				CompilerScratch::Dependency dependency(obj_exception);
				dependency.number = item.code;
				csb->csb_dependencies.push(dependency);
				break;
			}

			case blr_default_code:
				item.type = ExceptionItem::XCP_DEFAULT;
				item.code = 0;
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
	ErrorHandlerNode* node = FB_NEW(getPool()) ErrorHandlerNode(getPool());
	node->conditions = conditions;
	node->action = action->dsqlPass(dsqlScratch);
	return node;
}

void ErrorHandlerNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ErrorHandlerNode";
}

void ErrorHandlerNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_error_handler);
	fb_assert(conditions.getCount() < MAX_USHORT);
	dsqlScratch->appendUShort(USHORT(conditions.getCount()));

	for (ExceptionArray::iterator i = conditions.begin(); i != conditions.end(); ++i)
	{
		switch (i->type)
		{
			case ExceptionItem::SQL_CODE:
				dsqlScratch->appendUChar(blr_sql_code);
				dsqlScratch->appendUShort(i->code);
				break;

			case ExceptionItem::GDS_CODE:
				dsqlScratch->appendUChar(blr_gds_code);
				dsqlScratch->appendNullString(i->name.c_str());
				break;

			case ExceptionItem::XCP_CODE:
				dsqlScratch->appendUChar(blr_exception);
				dsqlScratch->appendNullString(i->name.c_str());
				break;

			case ExceptionItem::XCP_DEFAULT:
				dsqlScratch->appendUChar(blr_default_code);
				break;

			default:
				fb_assert(false);
				break;
		}
	}

	action->genBlr(dsqlScratch);
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
		exeState->exit = true;
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
	dsql_prc* procedure = METD_get_procedure(dsqlScratch->getTransaction(), dsqlScratch, dsqlName);

	if (!procedure)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
				  Arg::Gds(isc_dsql_procedure_err) <<
				  Arg::Gds(isc_random) <<
				  Arg::Str(dsqlName.toString()));
	}

	if (!dsqlScratch->isPsql())
	{
		dsqlScratch->procedure = procedure;
		dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_EXEC_PROCEDURE);
	}

	ExecProcedureNode* node = FB_NEW(getPool()) ExecProcedureNode(getPool(), dsqlName);

	if (node->dsqlName.package.isEmpty() && procedure->prc_name.package.hasData())
		node->dsqlName.package = procedure->prc_name.package;

	// Handle input parameters.

	const USHORT count = dsqlInputs ? dsqlInputs->nod_count : 0;
	if (count > procedure->prc_in_count || count < procedure->prc_in_count - procedure->prc_def_count)
		ERRD_post(Arg::Gds(isc_prcmismat) << Arg::Str(dsqlName.toString()));

	node->dsqlInputs = PASS1_node(dsqlScratch, dsqlInputs);

	if (count)
	{
		// Initialize this stack variable, and make it look like a node.
		AutoPtr<dsql_nod> desc_node(FB_NEW_RPT(*getDefaultMemoryPool(), 0) dsql_nod);

		dsql_nod* const* ptr = node->dsqlInputs->nod_arg;

		for (const dsql_fld* field = procedure->prc_inputs; *ptr; ++ptr, field = field->fld_next)
		{
			DEV_BLKCHK(field, dsql_type_fld);
			DEV_BLKCHK(*ptr, dsql_type_nod);
			MAKE_desc_from_field(&desc_node->nod_desc, field);
			// PASS1_set_parameter_type(*ptr, &desc_node, false);
			PASS1_set_parameter_type(dsqlScratch, *ptr, desc_node, false);
		}
	}

	// Handle output parameters.

	if (dsqlScratch->isPsql())
	{
		const USHORT outCount = dsqlOutputs ? dsqlOutputs->getCount() : 0;

		if (outCount != procedure->prc_out_count)
			ERRD_post(Arg::Gds(isc_prc_out_param_mismatch) << Arg::Str(dsqlName.toString()));

		node->dsqlOutputs = dsqlPassArray(dsqlScratch, dsqlOutputs);
	}
	else
	{
		if (dsqlOutputs)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  // Token unknown
					  Arg::Gds(isc_token_err) <<
					  Arg::Gds(isc_random) << Arg::Str("RETURNING_VALUES"));
		}

		node->dsqlOutputs = explodeOutputs(dsqlScratch, dsqlScratch->procedure);
	}

	return node;
}

// Generate a parameter list to correspond to procedure outputs.
Array<dsql_nod*>* ExecProcedureNode::explodeOutputs(DsqlCompilerScratch* dsqlScratch, const dsql_prc* procedure)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(procedure, dsql_type_prc);

	const USHORT count = procedure->prc_out_count;
	Array<dsql_nod*>* output = FB_NEW(getPool()) Array<dsql_nod*>(getPool(), count);

	for (const dsql_fld* field = procedure->prc_outputs; field; field = field->fld_next)
	{
		DEV_BLKCHK(field, dsql_type_fld);

		ParameterNode* paramNode = FB_NEW(getPool()) ParameterNode(getPool());

		dsql_nod* paramNod = MAKE_node(Dsql::nod_class_exprnode, 1);
		paramNod->nod_count = 0;
		paramNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(paramNode);

		output->add(paramNod);

		dsql_par* parameter = paramNode->dsqlParameter = MAKE_parameter(
			dsqlScratch->getStatement()->getReceiveMsg(), true, true, 0, NULL);
		paramNode->dsqlParameterIndex = parameter->par_index;

		MAKE_desc_from_field(&parameter->par_desc, field);
		parameter->par_name = parameter->par_alias = field->fld_name.c_str();
		parameter->par_rel_name = procedure->prc_name.identifier.c_str();
		parameter->par_owner_name = procedure->prc_owner.c_str();
	}

	return output;
}

void ExecProcedureNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecProcedureNode";
}

void ExecProcedureNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	const dsql_msg* message = NULL;

	if (dsqlScratch->getStatement()->getType() == DsqlCompiledStatement::TYPE_EXEC_PROCEDURE)
	{
		if ((message = dsqlScratch->getStatement()->getReceiveMsg()))
		{
			dsqlScratch->appendUChar(blr_begin);
			dsqlScratch->appendUChar(blr_send);
			dsqlScratch->appendUChar(message->msg_number);
		}
	}

	if (dsqlName.package.hasData())
	{
		dsqlScratch->appendUChar(blr_exec_proc2);
		dsqlScratch->appendMetaString(dsqlName.package.c_str());
	}
	else
		dsqlScratch->appendUChar(blr_exec_proc);

	dsqlScratch->appendMetaString(dsqlName.identifier.c_str());

	// Input parameters.
	if (dsqlInputs)
	{
		dsqlScratch->appendUShort(dsqlInputs->nod_count);
		dsql_nod** ptr = dsqlInputs->nod_arg;
		const dsql_nod* const* end = ptr + dsqlInputs->nod_count;

		while (ptr < end)
			GEN_expr(dsqlScratch, *ptr++);
	}
	else
		dsqlScratch->appendUShort(0);

	// Output parameters.
	if (dsqlOutputs)
	{
		dsqlScratch->appendUShort(dsqlOutputs->getCount());
		dsql_nod** ptr = dsqlOutputs->begin();

		for (const dsql_nod* const* end = dsqlOutputs->end(); ptr != end; ++ptr)
			GEN_expr(dsqlScratch, *ptr);
	}
	else
		dsqlScratch->appendUShort(0);

	if (message)
		dsqlScratch->appendUChar(blr_end);
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
	node->traScope = EDS::traCommon;

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

StmtNode* ExecStatementNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	ExecStatementNode* node = FB_NEW(getPool()) ExecStatementNode(getPool());

	node->dsqlSql = PASS1_node(dsqlScratch, dsqlSql);
	node->dsqlInputs = PASS1_node(dsqlScratch, dsqlInputs);
	node->inputNames = inputNames;

	// Check params names uniqueness, if present.

	if (node->inputNames)
	{
		const size_t count = node->inputNames->getCount();
		StrArray names(*getDefaultMemoryPool(), count);

		for (size_t i = 0; i != count; ++i)
		{
			const string* name = (*node->inputNames)[i];

			size_t pos;
			if (names.find(name->c_str(), pos))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
						  Arg::Gds(isc_dsql_duplicate_spec) << *name);
			}

			names.insert(pos, name->c_str());
		}
	}

	node->dsqlOutputs = dsqlPassArray(dsqlScratch, dsqlOutputs);

	if (innerStmt)
	{
		++dsqlScratch->loopLevel;
		node->dsqlLabel = PASS1_label(dsqlScratch, false, dsqlLabel);
		node->innerStmt = innerStmt->dsqlPass(dsqlScratch);
		--dsqlScratch->loopLevel;
		dsqlScratch->labels.pop();
	}

	// Process various optional arguments.

	node->dsqlDataSource = PASS1_node(dsqlScratch, dsqlDataSource);
	node->dsqlUserName = PASS1_node(dsqlScratch, dsqlUserName);
	node->dsqlPassword = PASS1_node(dsqlScratch, dsqlPassword);
	node->dsqlRole = PASS1_node(dsqlScratch, dsqlRole);
	node->traScope = traScope;
	node->useCallerPrivs = useCallerPrivs;

	return SavepointEncloseNode::make(getPool(), dsqlScratch, node);
}

void ExecStatementNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExecStatementNode";
}

void ExecStatementNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (innerStmt)
	{
		dsqlScratch->appendUChar(blr_label);
		dsqlScratch->appendUChar((int)(IPTR) dsqlLabel->nod_arg[Dsql::e_label_number]);
	}

	// If no new features of EXECUTE STATEMENT are used, lets generate old BLR.
	if (!dsqlDataSource && !dsqlUserName && !dsqlPassword && !dsqlRole && !useCallerPrivs &&
		!dsqlInputs && !traScope)
	{
		if (dsqlOutputs)
		{
			dsqlScratch->appendUChar(blr_exec_into);
			dsqlScratch->appendUShort(dsqlOutputs->getCount());

			GEN_expr(dsqlScratch, dsqlSql);

			if (innerStmt)
			{
				dsqlScratch->appendUChar(0); // Non-singleton.
				innerStmt->genBlr(dsqlScratch);
			}
			else
				dsqlScratch->appendUChar(1); // Singleton.

			for (size_t i = 0; i < dsqlOutputs->getCount(); ++i)
				GEN_expr(dsqlScratch, (*dsqlOutputs)[i]);
		}
		else
		{
			dsqlScratch->appendUChar(blr_exec_sql);
			GEN_expr(dsqlScratch, dsqlSql);
		}
	}
	else
	{
		dsqlScratch->appendUChar(blr_exec_stmt);

		// Counts of input and output parameters.
		if (dsqlInputs)
		{
			dsqlScratch->appendUChar(blr_exec_stmt_inputs);
			dsqlScratch->appendUShort(dsqlInputs->nod_count);
		}

		if (dsqlOutputs)
		{
			dsqlScratch->appendUChar(blr_exec_stmt_outputs);
			dsqlScratch->appendUShort(dsqlOutputs->getCount());
		}

		// Query expression.
		dsqlScratch->appendUChar(blr_exec_stmt_sql);
		GEN_expr(dsqlScratch, dsqlSql);

		// Proc block body.
		if (innerStmt)
		{
			dsqlScratch->appendUChar(blr_exec_stmt_proc_block);
			innerStmt->genBlr(dsqlScratch);
		}

		// External data source, user, password and role.
		genOptionalExpr(dsqlScratch, blr_exec_stmt_data_src, dsqlDataSource);
		genOptionalExpr(dsqlScratch, blr_exec_stmt_user, dsqlUserName);
		genOptionalExpr(dsqlScratch, blr_exec_stmt_pwd, dsqlPassword);
		genOptionalExpr(dsqlScratch, blr_exec_stmt_role, dsqlRole);

		// dsqlScratch's transaction behavior.
		if (traScope)
		{
			// Transaction parameters equal to current transaction.
			dsqlScratch->appendUChar(blr_exec_stmt_tran_clone);
			dsqlScratch->appendUChar(UCHAR(traScope));
		}

		// Inherit caller's privileges?
		if (useCallerPrivs)
			dsqlScratch->appendUChar(blr_exec_stmt_privs);

		// Inputs.
		if (dsqlInputs)
		{
			if (inputNames)
				dsqlScratch->appendUChar(blr_exec_stmt_in_params2);
			else
				dsqlScratch->appendUChar(blr_exec_stmt_in_params);

			dsql_nod* const* ptr = dsqlInputs->nod_arg;
			string* const* name = inputNames ? inputNames->begin() : NULL;

			for (const dsql_nod* const* end = ptr + dsqlInputs->nod_count; ptr != end; ++ptr, ++name)
			{
				if (inputNames)
					dsqlScratch->appendNullString((*name)->c_str());

				GEN_expr(dsqlScratch, *ptr);
			}
		}

		// Outputs.
		if (dsqlOutputs)
		{
			dsqlScratch->appendUChar(blr_exec_stmt_out_params);

			for (size_t i = 0; i < dsqlOutputs->getCount(); ++i)
				GEN_expr(dsqlScratch, (*dsqlOutputs)[i]);
		}

		dsqlScratch->appendUChar(blr_end);
	}
}

void ExecStatementNode::genOptionalExpr(DsqlCompilerScratch* dsqlScratch, const UCHAR code,
	dsql_nod* node)
{
	if (node)
	{
		dsqlScratch->appendUChar(code);
		GEN_expr(dsqlScratch, node);
	}
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
		getString(tdbb, request, sql, sSql, true);

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
	string& str, bool useAttCS) const
{
	MoveBuffer buffer;
	UCHAR* p = NULL;
	int len = 0;
	const dsc* dsc = node ? EVL_expr(tdbb, request, node) : NULL;

	if (dsc && !(request->req_flags & req_null))
	{
		const Jrd::Attachment* att = tdbb->getAttachment();
		len = MOV_make_string2(tdbb, dsc, (useAttCS ? att->att_charset : dsc->getTextType()),
			&p, buffer);
	}

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
	node->trueAction = trueAction->dsqlPass(dsqlScratch);
	if (falseAction)
		node->falseAction = falseAction->dsqlPass(dsqlScratch);
	return node;
}

void IfNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "IfNode";
	nodes.add(dsqlCondition);
}

void IfNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_if);
	GEN_expr(dsqlScratch, dsqlCondition);
	trueAction->genBlr(dsqlScratch);

	if (falseAction)
		falseAction->genBlr(dsqlScratch);
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
	node->action = action->dsqlPass(dsqlScratch);

	if (!autoTrans)
		dsqlScratch->flags &= ~DsqlCompilerScratch::FLAG_IN_AUTO_TRANS_BLOCK;

	return node;
}

void InAutonomousTransactionNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "InAutonomousTransactionNode";
}

void InAutonomousTransactionNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_auto_trans);
	dsqlScratch->appendUChar(0);	// to extend syntax in the future
	action->genBlr(dsqlScratch);
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

InitVariableNode* InitVariableNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
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
		(node->localDeclList ? node->localDeclList->statements.getCount() : 0);

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

	if (localDeclList)
	{
		string s;
		localDeclList->print(s, nodes);
		text += s + "\n";
	}
}

void ExecBlockNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	thread_db* tdbb = JRD_get_thread_data();

	dsqlScratch->beginDebug();

	// now do the input parameters
	for (size_t i = 0; i < parameters.getCount(); ++i)
	{
		ParameterClause& parameter = parameters[i];

		dsqlScratch->makeVariable(parameter.legacyField, parameter.name.c_str(),
			dsql_var::TYPE_INPUT, 0, (USHORT) (2 * i), 0);
	}

	const unsigned returnsPos = dsqlScratch->variables.getCount();

	// now do the output parameters
	for (size_t i = 0; i < returns.getCount(); ++i)
	{
		ParameterClause& parameter = returns[i];

		dsqlScratch->makeVariable(parameter.legacyField, parameter.name.c_str(),
			dsql_var::TYPE_OUTPUT, 1, (USHORT) (2 * i), i);
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

	for (Array<dsql_var*>::const_iterator i = dsqlScratch->outputVariables.begin();
		 i != dsqlScratch->outputVariables.end();
		 ++i)
	{
		VariableNode* varNode = FB_NEW(*tdbb->getDefaultPool()) VariableNode(*tdbb->getDefaultPool());
		varNode->dsqlVar = *i;

		dsql_nod* varNod = MAKE_node(Dsql::nod_class_exprnode, 1);
		varNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(varNode);

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
		const dsql_var* variable = dsqlScratch->variables[i];
		const dsql_fld* field = variable->field;

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
			dsqlScratch->appendUShort(variable->msgItem);
			dsqlScratch->appendUShort(variable->msgItem + 1);
			dsqlScratch->appendUChar(blr_null);
		}
	}

	for (Array<dsql_var*>::const_iterator i = dsqlScratch->outputVariables.begin();
		 i != dsqlScratch->outputVariables.end();
		 ++i)
	{
		dsqlScratch->putLocalVariable(*i, 0, NULL);
	}

	dsqlScratch->setPsql(true);

	dsqlScratch->putLocalVariables(localDeclList, USHORT(returns.getCount()));

	dsqlScratch->loopLevel = 0;

	StmtNode* stmtNode = body->dsqlPass(dsqlScratch);
	GEN_hidden_variables(dsqlScratch);

	dsqlScratch->appendUChar(blr_stall);
	// Put a label before body of procedure, so that
	// any exit statement can get out
	dsqlScratch->appendUChar(blr_label);
	dsqlScratch->appendUChar(0);

	stmtNode->genBlr(dsqlScratch);

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

	// Don't create ExceptionItem if blr_raise is used.
	if (codeType != blr_raise)
	{
		node->exception = FB_NEW(pool) ExceptionItem(pool);

		switch (codeType)
		{
			case blr_sql_code:
				node->exception->type = ExceptionItem::SQL_CODE;
				node->exception->code = (SSHORT) csb->csb_blr_reader.getWord();
				break;

			case blr_gds_code:
				node->exception->type = ExceptionItem::GDS_CODE;
				PAR_name(csb, node->exception->name);
				node->exception->name.lower();
				if (!(node->exception->code = PAR_symbol_to_gdscode(node->exception->name)))
					PAR_error(csb, Arg::Gds(isc_codnotdef) << node->exception->name);
				break;

			case blr_exception:
			case blr_exception_msg:
			case blr_exception_params:
				{
					node->exception->type = ExceptionItem::XCP_CODE;
					PAR_name(csb, node->exception->name);
					if (!(node->exception->code = MET_lookup_exception_number(tdbb, node->exception->name)))
						PAR_error(csb, Arg::Gds(isc_xcpnotdef) << node->exception->name);

					CompilerScratch::Dependency dependency(obj_exception);
					dependency.number = node->exception->code;
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
	if (exception)
		node->exception = FB_NEW(getPool()) ExceptionItem(getPool(), *exception);
	node->dsqlMessageExpr = PASS1_node(dsqlScratch, dsqlMessageExpr);
	node->dsqlParameters = PASS1_node(dsqlScratch, dsqlParameters);

	return SavepointEncloseNode::make(getPool(), dsqlScratch, node);
}

void ExceptionNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("ExceptionNode: Name: %s", (exception ? exception->name.c_str() : ""));
	if (dsqlMessageExpr)
		nodes.add(dsqlMessageExpr);
}

void ExceptionNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_abort);

	// If exception is NULL, it means we have re-initiate semantics here,
	// so blr_raise verb should be generated.
	if (!exception)
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
	else if (exception->type == ExceptionItem::GDS_CODE)
		dsqlScratch->appendUChar(blr_gds_code);
	else	// Otherwise go usual way, i.e. generate blr_exception.
		dsqlScratch->appendUChar(blr_exception);

	dsqlScratch->appendNullString(exception->name.c_str());

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

	SLONG xcpCode = exception->code;

	switch (exception->type)
	{
		case ExceptionItem::SQL_CODE:
			ERR_post(Arg::Gds(isc_sqlerr) << Arg::Num(xcpCode));

		case ExceptionItem::GDS_CODE:
			if (xcpCode == isc_check_constraint)
			{
				MET_lookup_cnstrt_for_trigger(tdbb, exName, relationName,
					request->getStatement()->triggerName);
				ERR_post(Arg::Gds(xcpCode) << Arg::Str(exName) << Arg::Str(relationName));
			}
			else
				ERR_post(Arg::Gds(xcpCode));

		case ExceptionItem::XCP_CODE:
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

ForNode* ForNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	ForNode* node = FB_NEW(getPool()) ForNode(getPool());

	node->dsqlCursor = dsqlCursor;
	node->dsqlSelect = PASS1_statement(dsqlScratch, dsqlSelect);

	if (dsqlCursor)
	{
		DeclareCursorNode* cursor = StmtNode::as<DeclareCursorNode>(dsqlCursor);
		fb_assert(cursor->dsqlCursorType != DeclareCursorNode::CUR_TYPE_NONE);
		PASS1_cursor_name(dsqlScratch, cursor->dsqlName, DeclareCursorNode::CUR_TYPE_ALL, false);
		cursor->dsqlRse = node->dsqlSelect;
		cursor->cursorNumber = dsqlScratch->cursorNumber++;
		dsqlScratch->cursors.push(cursor);
	}

	node->dsqlInto = dsqlPassArray(dsqlScratch, dsqlInto);

	if (statement)
	{
		// CVC: Let's add the ability to BREAK the for_select same as the while,
		// but only if the command is FOR SELECT, otherwise we have singular SELECT
		++dsqlScratch->loopLevel;
		node->dsqlLabel = PASS1_label(dsqlScratch, false, dsqlLabel);
		node->statement = statement->dsqlPass(dsqlScratch);
		--dsqlScratch->loopLevel;
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
	nodes.add(dsqlCursor);
	nodes.add(dsqlLabel);
}

void ForNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	// CVC: Only put a label if this is not singular; otherwise,
	// what loop is the user trying to abandon?
	if (statement)
	{
		dsqlScratch->appendUChar(blr_label);
		dsqlScratch->appendUChar((int)(IPTR) dsqlLabel->nod_arg[Dsql::e_label_number]);
	}

	// Generate FOR loop

	dsqlScratch->appendUChar(blr_for);

	if (!statement || dsqlForceSingular)
		dsqlScratch->appendUChar(blr_singular);

	GEN_rse(dsqlScratch, dsqlSelect);
	dsqlScratch->appendUChar(blr_begin);

	// Build body of FOR loop

	dsql_nod* list = ExprNode::as<RseNode>(dsqlSelect)->dsqlSelectList;

	if (dsqlInto)
	{
		if (list->nod_count != dsqlInto->getCount())
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-313) <<
					  Arg::Gds(isc_dsql_count_mismatch));
		}

		dsql_nod** ptr = list->nod_arg;
		dsql_nod** ptr_to = dsqlInto->begin();

		for (const dsql_nod* const* const end = ptr + list->nod_count; ptr != end; ++ptr, ++ptr_to)
		{
			dsqlScratch->appendUChar(blr_assignment);
			GEN_expr(dsqlScratch, *ptr);
			GEN_expr(dsqlScratch, *ptr_to);
		}
	}

	if (statement)
		statement->genBlr(dsqlScratch);

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

	impureOffset = CMP_impure(csb, sizeof(SLONG));

	return this;
}

const StmtNode* ForNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	jrd_tra* transaction = request->req_transaction;
	jrd_tra* sysTransaction = request->req_attachment->getSysTransaction();

	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
			*request->getImpure<SLONG>(impureOffset) = 0;
			if (transaction != sysTransaction &&
				transaction->tra_save_point && transaction->tra_save_point->sav_verb_actions)
			{
				VIO_start_save_point(tdbb, transaction);
				const Savepoint* save_point = transaction->tra_save_point;
				*request->getImpure<SLONG>(impureOffset) = save_point->sav_number;
			}
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
		{
			const SLONG sav_number = *request->getImpure<SLONG>(impureOffset);
			if (sav_number)
			{
				for (const Savepoint* save_point = transaction->tra_save_point;
					 save_point && sav_number <= save_point->sav_number;
					 save_point = transaction->tra_save_point)
				{
					EXE_verb_cleanup(tdbb, transaction);
				}
			}
			cursor->close(tdbb);
			return parentStmt;
		}
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

const StmtNode* LabelNode::execute(thread_db* /*tdbb*/, jrd_req* request, ExeState* /*exeState*/) const
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


void LineColumnNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("LineColumnNode: line %d, col %d", line, column);
}

LineColumnNode* LineColumnNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	statement = statement->dsqlPass(dsqlScratch);
	return this;
}

void LineColumnNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->putDebugSrcInfo(line, column);
	statement->genBlr(dsqlScratch);
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
	LoopNode* node = FB_NEW(getPool()) LoopNode(getPool());

	node->dsqlExpr = PASS1_node(dsqlScratch, dsqlExpr);

	// CVC: Loop numbers should be incremented before analyzing the body
	// to preserve nesting <==> increasing level number.
	++dsqlScratch->loopLevel;
	node->dsqlLabel = PASS1_label(dsqlScratch, false, dsqlLabel);
	node->statement = statement->dsqlPass(dsqlScratch);
	--dsqlScratch->loopLevel;
	dsqlScratch->labels.pop();

	return node;
}

void LoopNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "LoopNode";
}

void LoopNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_label);
	const IPTR lblNum = (IPTR) dsqlLabel->nod_arg[Dsql::e_label_number];
	fb_assert(lblNum < MAX_UCHAR);
	dsqlScratch->appendUChar((UCHAR) lblNum);
	dsqlScratch->appendUChar(blr_loop);
	dsqlScratch->appendUChar(blr_begin);
	dsqlScratch->appendUChar(blr_if);
	GEN_expr(dsqlScratch, dsqlExpr);
	statement->genBlr(dsqlScratch);
	dsqlScratch->appendUChar(blr_leave);
	dsqlScratch->appendUChar((UCHAR) lblNum);
	dsqlScratch->appendUChar(blr_end);
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


StmtNode* MergeNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	// Puts a blr_send before blr_for in DSQL statements.
	class MergeSendNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_MERGE_SEND>
	{
	public:
		MergeSendNode(MemoryPool& pool, StmtNode* aStmt)
			: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_MERGE_SEND>(pool),
			  stmt(aStmt)
		{
		}

	public:
		virtual void print(string& text, Array<dsql_nod*>& nodes) const
		{
			text = "MergeSendNode";
		}

		// Do not make dsqlPass to process 'stmt'. It's already processed.

		virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
		{
			dsql_msg* message = dsqlScratch->getStatement()->getReceiveMsg();

			if (!dsqlScratch->isPsql() && message)
			{
				dsqlScratch->appendUChar(blr_send);
				dsqlScratch->appendUChar(message->msg_number);
			}

			stmt->genBlr(dsqlScratch);
		}

	private:
		StmtNode* stmt;
	};

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	dsql_nod* source = dsqlUsing;		// USING
	dsql_nod* target = dsqlRelation;	// INTO
	dsql_nod* updDelCondition = dsqlWhenMatchedCondition;
	dsql_nod* insCondition = dsqlWhenNotMatchedCondition;

	// Build a join between USING and INTO tables.
	RseNode* join = FB_NEW(pool) RseNode(pool);
	join->dsqlExplicitJoin = true;
	join->dsqlFrom = MAKE_node(Dsql::nod_list, 2);

	join->dsqlFrom->nod_arg[0] = source;

	// Left join if WHEN NOT MATCHED is present.
	if (dsqlWhenNotMatchedPresent)
		join->rse_jointype = blr_left;

	join->dsqlFrom->nod_arg[1] = target;
	join->dsqlWhere = dsqlCondition;

	RseNode* querySpec = FB_NEW(pool) RseNode(pool);
	querySpec->dsqlFrom = MAKE_node(Dsql::nod_list, 1);
	querySpec->dsqlFrom->nod_arg[0] = MAKE_node(Dsql::nod_class_exprnode, 1);
	querySpec->dsqlFrom->nod_arg[0]->nod_arg[0] = reinterpret_cast<dsql_nod*>(join);

	dsql_nod* querySpecNod = MAKE_node(Dsql::nod_class_exprnode, 1);
	querySpecNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(querySpec);

	if (updDelCondition || insCondition)
	{
		const char* targetName = ExprNode::as<RelationSourceNode>(target)->alias.nullStr();
		if (!targetName)
			targetName = ExprNode::as<RelationSourceNode>(target)->dsqlName.c_str();

		if (dsqlWhenMatchedPresent)	// WHEN MATCHED
		{
			MissingBoolNode* missingNode = FB_NEW(pool) MissingBoolNode(
				pool, MAKE_node(Dsql::nod_class_exprnode, 1));
			missingNode->dsqlArg->nod_arg[0] = reinterpret_cast<dsql_nod*>(
				FB_NEW(pool) RecordKeyNode(pool, blr_dbkey, targetName));

			NotBoolNode* notNode = FB_NEW(pool) NotBoolNode(
				pool, MAKE_node(Dsql::nod_class_exprnode, 1));
			notNode->dsqlArg->nod_arg[0] = reinterpret_cast<dsql_nod*>(missingNode);

			querySpec->dsqlWhere = MAKE_node(Dsql::nod_class_exprnode, 1);
			querySpec->dsqlWhere->nod_arg[0] = reinterpret_cast<dsql_nod*>(notNode);
		}

		if (updDelCondition)
			querySpec->dsqlWhere = PASS1_compose(querySpec->dsqlWhere, updDelCondition, blr_and);

		dsql_nod* temp = NULL;

		if (dsqlWhenNotMatchedPresent)	// WHEN NOT MATCHED
		{
			MissingBoolNode* missingNode = FB_NEW(pool) MissingBoolNode(
				pool, MAKE_node(Dsql::nod_class_exprnode, 1));
			missingNode->dsqlArg->nod_arg[0] = reinterpret_cast<dsql_nod*>(
				FB_NEW(pool) RecordKeyNode(pool, blr_dbkey, targetName));

			temp = MAKE_node(Dsql::nod_class_exprnode, 1);
			temp->nod_arg[0] = reinterpret_cast<dsql_nod*>(missingNode);

			if (insCondition)
				temp = PASS1_compose(temp, insCondition, blr_and);

			querySpec->dsqlWhere = PASS1_compose(querySpec->dsqlWhere, temp, blr_or);
		}
	}

	dsql_nod* select_expr = MAKE_node(Dsql::nod_select_expr, Dsql::e_sel_count);
	select_expr->nod_arg[Dsql::e_sel_query_spec] = querySpecNod;

	dsql_nod* select = MAKE_node(Dsql::nod_select, Dsql::e_select_count);
	select->nod_arg[Dsql::e_select_expr] = select_expr;

	// build a FOR SELECT node
	ForNode* forNode = FB_NEW(pool) ForNode(pool);
	forNode->dsqlSelect = select;
	forNode->statement = FB_NEW(pool) CompoundStmtNode(pool);

	forNode = forNode->dsqlPass(dsqlScratch);

	if (dsqlReturning)
		forNode->dsqlForceSingular = true;

	// Get the already processed relations.
	source = ExprNode::as<RseNode>(ExprNode::as<RseNode>(
		forNode->dsqlSelect)->dsqlStreams->nod_arg[0])->dsqlStreams->nod_arg[0];
	target = ExprNode::as<RseNode>(ExprNode::as<RseNode>(
		forNode->dsqlSelect)->dsqlStreams->nod_arg[0])->dsqlStreams->nod_arg[1];

	DsqlContextStack usingCtxs;
	dsqlGetContexts(usingCtxs, source);

	StmtNode* update = NULL;
	StmtNode* matchedRet = NULL;
	StmtNode* nullRet = NULL;

	if (dsqlWhenMatchedPresent && dsqlWhenMatchedAssignments)
	{
		// Get the assignments of the UPDATE dsqlScratch.
		CompoundStmtNode* stmts = dsqlWhenMatchedAssignments;
		Array<dsql_nod*> org_values, new_values;

		// Separate the new and org values to process in correct contexts.
		for (size_t i = 0; i < stmts->statements.getCount(); ++i)
		{
			const AssignmentNode* const assign = stmts->statements[i]->as<AssignmentNode>();
			fb_assert(assign);
			org_values.add(assign->dsqlAsgnFrom);
			new_values.add(assign->dsqlAsgnTo);
		}

		// Build the MODIFY node.
		ModifyNode* modify = FB_NEW(pool) ModifyNode(pool);

		dsql_ctx* const old_context = dsqlGetContext(target);
		dsql_nod** ptr;

		modify->dsqlContext = old_context;

		++dsqlScratch->scopeLevel;	// Go to the same level of source and target contexts.

		for (DsqlContextStack::iterator itr(usingCtxs); itr.hasData(); ++itr)
			dsqlScratch->context->push(itr.object());	// push the USING contexts

		dsqlScratch->context->push(old_context);	// process old context values

		for (ptr = org_values.begin(); ptr != org_values.end(); ++ptr)
			*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);

		// And pop the contexts.
		dsqlScratch->context->pop();
		dsqlScratch->context->pop();
		--dsqlScratch->scopeLevel;

		// Process relation.
		modify->dsqlRelation = PASS1_relation(dsqlScratch, dsqlRelation);
		dsql_ctx* mod_context = dsqlGetContext(modify->dsqlRelation);

		// Process new context values.
		for (ptr = new_values.begin(); ptr != new_values.end(); ++ptr)
			*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);

		dsqlScratch->context->pop();

		if (dsqlReturning)
		{
			// Repush the source contexts.
			++dsqlScratch->scopeLevel;	// Go to the same level of source and target contexts.

			for (DsqlContextStack::iterator itr(usingCtxs); itr.hasData(); ++itr)
				dsqlScratch->context->push(itr.object());	// push the USING contexts

			dsqlScratch->context->push(old_context);	// process old context values

			mod_context->ctx_scope_level = old_context->ctx_scope_level;

			matchedRet = modify->statement2 = ReturningProcessor(
				dsqlScratch, old_context, mod_context).process(dsqlReturning, NULL);

			nullRet = dsqlNullifyReturning(dsqlScratch, modify, false);

			// And pop them.
			dsqlScratch->context->pop();
			dsqlScratch->context->pop();
			--dsqlScratch->scopeLevel;
		}

		// Recreate the list of assignments.

		CompoundStmtNode* assignStatements = FB_NEW(pool) CompoundStmtNode(pool);
		modify->statement = assignStatements;

		assignStatements->statements.resize(stmts->statements.getCount());

		for (size_t i = 0; i < assignStatements->statements.getCount(); ++i)
		{
			if (!PASS1_set_parameter_type(dsqlScratch, org_values[i], new_values[i], false))
				PASS1_set_parameter_type(dsqlScratch, new_values[i], org_values[i], false);

			AssignmentNode* assign = FB_NEW(pool) AssignmentNode(pool);
			assign->dsqlAsgnFrom = org_values[i];
			assign->dsqlAsgnTo = new_values[i];
			assignStatements->statements[i] = assign;
		}

		// We do not allow cases like UPDATE SET f1 = v1, f2 = v2, f1 = v3...
		dsqlFieldAppearsOnce(new_values, "MERGE");

		update = modify;
	}
	else if (dsqlWhenMatchedPresent && !dsqlWhenMatchedAssignments)
	{
		// build the DELETE node
		EraseNode* erase = FB_NEW(pool) EraseNode(pool);
		dsql_ctx* context = dsqlGetContext(target);
		erase->dsqlContext = context;

		if (dsqlReturning)
		{
			++dsqlScratch->scopeLevel;	// Go to the same level of source and target contexts.

			for (DsqlContextStack::iterator itr(usingCtxs); itr.hasData(); ++itr)
				dsqlScratch->context->push(itr.object());	// push the USING contexts

			dsqlScratch->context->push(context);	// process old context values

			matchedRet = erase->statement = ReturningProcessor(
				dsqlScratch, context, NULL).process(dsqlReturning, NULL);

			nullRet = dsqlNullifyReturning(dsqlScratch, erase, false);

			// And pop the contexts.
			dsqlScratch->context->pop();
			dsqlScratch->context->pop();
			--dsqlScratch->scopeLevel;
		}

		update = erase;
	}

	StmtNode* insert = NULL;

	if (dsqlWhenNotMatchedPresent)
	{
		++dsqlScratch->scopeLevel;	// Go to the same level of the source context.

		for (DsqlContextStack::iterator itr(usingCtxs); itr.hasData(); ++itr)
			dsqlScratch->context->push(itr.object());	// push the USING contexts

		// The INSERT relation should be processed in a higher level than the source context.
		++dsqlScratch->scopeLevel;

		// Build the INSERT node.
		StoreNode* store = FB_NEW(pool) StoreNode(pool);
		store->dsqlRelation = dsqlRelation;
		store->dsqlFields = dsqlWhenNotMatchedFields;
		store->dsqlValues = dsqlWhenNotMatchedValues;

		store = store->internalDsqlPass(dsqlScratch, false)->as<StoreNode>();
		fb_assert(store);

		// Restore the scope level.
		--dsqlScratch->scopeLevel;

		StmtNode* insRet = ReturningProcessor::clone(dsqlScratch, dsqlReturning, matchedRet);

		if (dsqlReturning)
		{
			dsql_ctx* const old_context = dsqlGetContext(target);
			dsqlScratch->context->push(old_context);

			dsql_ctx* context = dsqlGetContext(store->dsqlRelation);
			context->ctx_scope_level = old_context->ctx_scope_level;

			store->statement2 = ReturningProcessor(
				dsqlScratch, old_context, context).process(dsqlReturning, insRet);

			if (!matchedRet)
				nullRet = dsqlNullifyReturning(dsqlScratch, store, false);

			dsqlScratch->context->pop();
		}

		// Pop the USING context.
		dsqlScratch->context->pop();
		--dsqlScratch->scopeLevel;

		insert = store;
	}

	MissingBoolNode* missingNode = FB_NEW(pool) MissingBoolNode(
		pool, MAKE_node(Dsql::nod_class_exprnode, 1));

	RecordKeyNode* dbKeyNode = FB_NEW(pool) RecordKeyNode(pool, blr_dbkey);
	dbKeyNode->dsqlRelation = target;
	missingNode->dsqlArg->nod_arg[0] = reinterpret_cast<dsql_nod*>(dbKeyNode);

	// Build a IF (target.RDB$DB_KEY IS NULL).
	IfNode* action = FB_NEW(pool) IfNode(pool);

	action->dsqlCondition = MAKE_node(Dsql::nod_class_exprnode, 1);
	action->dsqlCondition->nod_arg[0] = reinterpret_cast<dsql_nod*>(missingNode);

	if (insert)
	{
		action->trueAction = insert;	// then INSERT
		action->falseAction = update;	// else UPDATE/DELETE
	}
	else
	{
		// Negate the condition -> IF (target.RDB$DB_KEY IS NOT NULL).

		NotBoolNode* notNode = FB_NEW(pool) NotBoolNode(pool, action->dsqlCondition);

		action->dsqlCondition = MAKE_node(Dsql::nod_class_exprnode, 1);
		action->dsqlCondition->nod_arg[0] = reinterpret_cast<dsql_nod*>(notNode);

		action->trueAction = update;	// then UPDATE/DELETE
	}

	if (!dsqlScratch->isPsql())
	{
		// Describe it as EXECUTE_PROCEDURE if RETURNING is present or as INSERT otherwise.
		if (dsqlReturning)
			dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_EXEC_PROCEDURE);
		else
			dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_INSERT);

		dsqlScratch->flags |= DsqlCompilerScratch::FLAG_MERGE;
	}

	// Insert the IF inside the FOR SELECT.
	forNode->statement = action;

	StmtNode* mergeStmt = forNode;

	// Setup the main node.

	if (nullRet)
	{
		CompoundStmtNode* temp = FB_NEW(pool) CompoundStmtNode(pool);
		temp->statements.add(nullRet);
		temp->statements.add(forNode);
		mergeStmt = temp;
	}

	StmtNode* sendNode = (FB_NEW(pool) MergeSendNode(pool, mergeStmt))->dsqlPass(dsqlScratch);

	return SavepointEncloseNode::make(getPool(), dsqlScratch, sendNode);
}

void MergeNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "MergeNode";
}

void MergeNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
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
		desc->dsc_address = (UCHAR*)(IPTR) offset;
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

	if (offset > MAX_MESSAGE_SIZE)
		PAR_error(csb, Arg::Gds(isc_imp_exc) << Arg::Gds(isc_blktoobig));

	node->format->fmt_length = offset;

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

MessageNode* MessageNode::copy(thread_db* tdbb, NodeCopier& copier) const
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

	const USHORT orgStream = csb->csb_rpt[context].csb_stream;
	const USHORT newStream = csb->nextStream(false);

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


StmtNode* ModifyNode::internalDsqlPass(DsqlCompilerScratch* dsqlScratch, bool updateOrInsert)
{
	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = getPool();

	const bool isUpdateSqlCompliant = !Config::getOldSetClauseSemantics();

	// Separate old and new context references.

	Array<dsql_nod*> org_values, new_values;

	CompoundStmtNode* assignments = statement->as<CompoundStmtNode>();
	fb_assert(assignments);

	for (size_t i = 0; i < assignments->statements.getCount(); ++i)
	{
		const AssignmentNode* const assign = assignments->statements[i]->as<AssignmentNode>();
		fb_assert(assign);
		org_values.add(assign->dsqlAsgnFrom);
		new_values.add(assign->dsqlAsgnTo);
	}

	dsql_nod* cursor = dsqlCursor;
	dsql_nod* relation = dsqlRelation;
	dsql_nod** ptr;

	ModifyNode* node = FB_NEW(pool) ModifyNode(pool);

	if (cursor && dsqlScratch->isPsql())
	{
		node->dsqlContext = dsqlPassCursorContext(dsqlScratch, cursor, relation);

		if (isUpdateSqlCompliant)
		{
			// Process old context values.
			dsqlScratch->context->push(node->dsqlContext);
			++dsqlScratch->scopeLevel;

			for (ptr = org_values.begin(); ptr != org_values.end(); ++ptr)
				*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);

			--dsqlScratch->scopeLevel;
			dsqlScratch->context->pop();
		}

		// Process relation.
		node->dsqlRelation = PASS1_node_psql(dsqlScratch, relation, false);

		if (!isUpdateSqlCompliant)
		{
			// Process old context values.
			for (ptr = org_values.begin(); ptr != org_values.end(); ++ptr)
				*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);
		}

		// Process new context values.
		for (ptr = new_values.begin(); ptr != new_values.end(); ++ptr)
			*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);

		node->statement2 = dsqlProcessReturning(dsqlScratch, dsqlReturning, statement2);

		dsqlScratch->context->pop();

		// Recreate list of assignments.

		CompoundStmtNode* assignStatements = FB_NEW(pool) CompoundStmtNode(pool);
		node->statement = assignStatements;

		assignStatements->statements.resize(assignments->statements.getCount());

		for (size_t i = 0; i < assignStatements->statements.getCount(); ++i)
		{
			AssignmentNode* assign = FB_NEW(pool) AssignmentNode(pool);
			assign->dsqlAsgnFrom = org_values[i];
			assign->dsqlAsgnTo = new_values[i];
			assignStatements->statements[i] = assign;
		}

		// We do not allow cases like UPDATE T SET f1 = v1, f2 = v2, f1 = v3...
		dsqlFieldAppearsOnce(new_values, "UPDATE");

		return node;
	}

	dsqlScratch->getStatement()->setType(
		cursor ? DsqlCompiledStatement::TYPE_UPDATE_CURSOR : DsqlCompiledStatement::TYPE_UPDATE);

	node->dsqlRelation = PASS1_node_psql(dsqlScratch, relation, false);
	dsql_ctx* mod_context = dsqlGetContext(node->dsqlRelation);

	if (!isUpdateSqlCompliant)
	{
		// Process old context values.
		for (ptr = org_values.begin(); ptr != org_values.end(); ++ptr)
			*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);
	}

	// Process new context values.
	for (ptr = new_values.begin(); ptr != new_values.end(); ++ptr)
		*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);

	dsqlScratch->context->pop();

	// Generate record selection expression

	dsql_nod* rseNod;

	if (cursor)
		rseNod = dsqlPassCursorReference(dsqlScratch, cursor, relation);
	else
	{
		RseNode* rse = FB_NEW(pool) RseNode(pool);

		rseNod = MAKE_node(Dsql::nod_class_exprnode, 1);
		rseNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(rse);
		rseNod->nod_flags = dsqlRseFlags;

		if (dsqlReturning || statement2)
			rseNod->nod_flags |= NOD_SELECT_EXPR_SINGLETON;

		dsql_nod* temp = MAKE_node(Dsql::nod_list, 1);
		rse->dsqlStreams = temp;
		temp->nod_arg[0] = PASS1_node_psql(dsqlScratch, relation, false);
		dsql_ctx* old_context = dsqlGetContext(temp->nod_arg[0]);

		if ((temp = dsqlBoolean))
			rse->dsqlWhere = PASS1_node_psql(dsqlScratch, temp, false);

		if ((temp = dsqlPlan))
			rse->dsqlPlan = PASS1_node_psql(dsqlScratch, temp, false);

		if ((temp = dsqlSort))
			rse->dsqlOrder = PASS1_sort(dsqlScratch, temp, NULL);

		if ((temp = dsqlRows))
		{
			PASS1_limit(dsqlScratch, temp->nod_arg[Dsql::e_rows_length],
				temp->nod_arg[Dsql::e_rows_skip], rse);
		}

		if (dsqlReturning || statement2)
		{
			node->statement2 = ReturningProcessor(
				dsqlScratch, old_context, mod_context).process(dsqlReturning, statement2);
		}
	}

	node->dsqlRse = rseNod;

	if (isUpdateSqlCompliant)
	{
		// Process old context values.
		for (ptr = org_values.begin(); ptr != org_values.end(); ++ptr)
			*ptr = PASS1_node_psql(dsqlScratch, *ptr, false);
	}

	dsqlScratch->context->pop();

	// Recreate list of assignments.

	CompoundStmtNode* assignStatements = FB_NEW(pool) CompoundStmtNode(pool);
	node->statement = assignStatements;

	assignStatements->statements.resize(assignments->statements.getCount());

	for (size_t j = 0; j < assignStatements->statements.getCount(); ++j)
	{
		dsql_nod* const sub1 = org_values[j];
		dsql_nod* const sub2 = new_values[j];

		if (!PASS1_set_parameter_type(dsqlScratch, sub1, sub2, false))
			PASS1_set_parameter_type(dsqlScratch, sub2, sub1, false);

		AssignmentNode* assign = FB_NEW(pool) AssignmentNode(pool);
		assign->dsqlAsgnFrom = sub1;
		assign->dsqlAsgnTo = sub2;
		assignStatements->statements[j] = assign;
	}

	// We do not allow cases like UPDATE T SET f1 = v1, f2 = v2, f1 = v3...
	dsqlFieldAppearsOnce(new_values, "UPDATE");

	dsqlSetParametersName(assignStatements, node->dsqlRelation);

	StmtNode* ret = node;
	if (!updateOrInsert)
		ret = dsqlNullifyReturning(dsqlScratch, node, true);

	return ret;
}

StmtNode* ModifyNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return SavepointEncloseNode::make(getPool(), dsqlScratch, internalDsqlPass(dsqlScratch, false));
}

void ModifyNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "ModifyNode";
}

void ModifyNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	const dsql_msg* message = dsqlGenDmlHeader(dsqlScratch, dsqlRse);

	dsqlScratch->appendUChar(statement2 ? blr_modify2 : blr_modify);

	const dsql_ctx* context;
	dsql_nod* temp;

	if (dsqlContext)
		context = dsqlContext;
	else
	{
		temp = ExprNode::as<RseNode>(dsqlRse)->dsqlStreams->nod_arg[0];
		context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
	}

	GEN_stuff_context(dsqlScratch, context);
	temp = dsqlRelation;
	context = ExprNode::as<RelationSourceNode>(temp)->dsqlContext;
	GEN_stuff_context(dsqlScratch, context);
	statement->genBlr(dsqlScratch);

	if (statement2)
		statement2->genBlr(dsqlScratch);

	if (message)
		dsqlScratch->appendUChar(blr_end);
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

	if (request->req_operation == jrd_req::req_return && !impure->sta_state && subMod)
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

	// Copy the original record to the new record.

	VIO_copy_record(tdbb, orgRpb, newRpb);

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

StmtNode* StoreNode::internalDsqlPass(DsqlCompilerScratch* dsqlScratch, bool updateOrInsert)
{
	thread_db* tdbb = JRD_get_thread_data();

	dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_INSERT);

	StoreNode* node = FB_NEW(getPool()) StoreNode(getPool());

	// Process SELECT expression, if present

	dsql_nod* values;
	dsql_nod* rse = dsqlRse;

	if (rse)
	{
		if (dsqlReturning || statement2)
			rse->nod_flags |= NOD_SELECT_EXPR_SINGLETON;

		node->dsqlRse = rse = PASS1_rse(dsqlScratch, rse, NULL);
		values = ExprNode::as<RseNode>(rse)->dsqlSelectList;
	}
	else
		values = PASS1_node_psql(dsqlScratch, dsqlValues, false);

	// Process relation

	dsql_nod* temp_rel = PASS1_relation(dsqlScratch, dsqlRelation);
	node->dsqlRelation = temp_rel;
	dsql_ctx* context = ExprNode::as<RelationSourceNode>(temp_rel)->dsqlContext;
	DEV_BLKCHK(context, dsql_type_ctx);
	dsql_rel* relation = context->ctx_relation;

	// If there isn't a field list, generate one

	dsql_nod* fields = dsqlFields;

	if (fields)
	{
		const dsql_nod* const old_fields = fields; // for error reporting.
		fields = PASS1_node_psql(dsqlScratch, fields, false);

		// We do not allow cases like INSERT INTO T (f1, f2, f1)...
		{	// scope
			Array<dsql_nod*> newValues;

			for (USHORT i = 0; i < fields->nod_count; ++i)
				newValues.add(fields->nod_arg[i]);

			dsqlFieldAppearsOnce(newValues, "INSERT");
		}	// scope

		// begin IBO hack
		// 02-May-2004, Nickolay Samofatov. Do not constify ptr further e.g. to
		// const dsql_nod* const* .... etc. It chokes GCC 3.4.0
		dsql_nod** ptr = fields->nod_arg;
		for (const dsql_nod* const* const end = ptr + fields->nod_count; ptr < end; ptr++)
		{
			DEV_BLKCHK (*ptr, dsql_type_nod);
			const dsql_nod* temp2 = *ptr;

			const dsql_ctx* tmp_ctx = NULL;
			const TEXT* tmp_name = NULL;
			const FieldNode* fieldNode;
			const DerivedFieldNode* derivedField;

			if ((fieldNode = ExprNode::as<FieldNode>(temp2)))
			{
				tmp_ctx = fieldNode->dsqlContext;
				if (fieldNode->dsqlField)
					tmp_name = fieldNode->dsqlField->fld_name.c_str();
			}
			else if ((derivedField = ExprNode::as<DerivedFieldNode>(temp2)))
			{
				tmp_ctx = derivedField->context;
				tmp_name = derivedField->name.nullStr();
			}

			if (tmp_ctx &&
				((tmp_ctx->ctx_relation && relation->rel_name != tmp_ctx->ctx_relation->rel_name) ||
				 tmp_ctx->ctx_context != context->ctx_context))
			{
				const dsql_rel* bad_rel = tmp_ctx->ctx_relation;
				// At this time, "fields" has been replaced by the processed list in
				// the same variable, so we refer again to dsqlFields.
				// CVC: After three years, made old_fields for that purpose.

				PASS1_field_unknown((bad_rel ? bad_rel->rel_name.c_str() : NULL),
					tmp_name, old_fields->nod_arg[ptr - fields->nod_arg]);
			}
		}
		// end IBO hack
	}
	else
		fields = PASS1_node_psql(dsqlScratch, dsqlExplodeFields(relation), false);

	// Match field fields and values

	CompoundStmtNode* assignStatements = FB_NEW(getPool()) CompoundStmtNode(getPool());
	node->statement = assignStatements;

	if (values)
	{
		if (fields->nod_count != values->nod_count)
		{
			// count of column list and value list don't match
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
					  Arg::Gds(isc_dsql_var_count_err));
		}

		dsql_nod** ptr = fields->nod_arg;
		dsql_nod** ptr2 = values->nod_arg;
		for (const dsql_nod* const* const end = ptr + fields->nod_count; ptr < end; ptr++, ptr2++)
		{
			DEV_BLKCHK(*ptr, dsql_type_nod);
			DEV_BLKCHK(*ptr2, dsql_type_nod);

			AssignmentNode* temp = FB_NEW(getPool()) AssignmentNode(getPool());
			temp->dsqlAsgnFrom = *ptr2;
			temp->dsqlAsgnTo = *ptr;
			assignStatements->statements.add(temp);

			PASS1_set_parameter_type(dsqlScratch, *ptr2, *ptr, false);
		}
	}

	if (updateOrInsert)
	{
		// Clone the insert context, push with name "OLD" in the same scope level and
		// marks it with CTX_null so all fields be resolved to NULL constant.
		dsql_ctx* old_context = FB_NEW(dsqlScratch->getPool()) dsql_ctx(dsqlScratch->getPool());
		*old_context = *context;
		old_context->ctx_alias = old_context->ctx_internal_alias = MAKE_cstring(OLD_CONTEXT)->str_data;
		old_context->ctx_flags |= CTX_system | CTX_null | CTX_returning;
		dsqlScratch->context->push(old_context);

		// clone the insert context and push with name "NEW" in a greater scope level
		dsql_ctx* new_context = FB_NEW(dsqlScratch->getPool()) dsql_ctx(dsqlScratch->getPool());
		*new_context = *context;
		new_context->ctx_scope_level = ++dsqlScratch->scopeLevel;
		new_context->ctx_alias = new_context->ctx_internal_alias = MAKE_cstring(NEW_CONTEXT)->str_data;
		new_context->ctx_flags |= CTX_system | CTX_returning;
		dsqlScratch->context->push(new_context);
	}

	node->statement2 = dsqlProcessReturning(dsqlScratch, dsqlReturning, statement2);

	if (updateOrInsert)
	{
		--dsqlScratch->scopeLevel;
		dsqlScratch->context->pop();
		dsqlScratch->context->pop();
	}

	dsqlSetParametersName(assignStatements, node->dsqlRelation);

	StmtNode* ret = node;
	if (!updateOrInsert)
		ret = dsqlNullifyReturning(dsqlScratch, node, true);

	dsqlScratch->context->pop();

	return ret;
}

StmtNode* StoreNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return SavepointEncloseNode::make(getPool(), dsqlScratch, internalDsqlPass(dsqlScratch, false));
}

void StoreNode::print(string& text, Array<dsql_nod*>& /*nodes*/) const
{
	text = "StoreNode";
}

void StoreNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	const dsql_msg* message = dsqlGenDmlHeader(dsqlScratch, dsqlRse);

	dsqlScratch->appendUChar(statement2 ? blr_store2 : blr_store);
	GEN_expr(dsqlScratch, dsqlRelation);
	statement->genBlr(dsqlScratch);

	if (statement2)
		statement2->genBlr(dsqlScratch);

	if (message)
		dsqlScratch->appendUChar(blr_end);
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

// Build a default value assignments.
void StoreNode::makeDefaults(thread_db* tdbb, CompilerScratch* csb)
{
	const USHORT stream = relationSource->getStream();
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
				genNode->id = MET_lookup_generator(tdbb, (*ptr1)->fld_generator_name);
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

	const USHORT stream = relationSource->getStream();
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

	SetGeneratorNode* node = FB_NEW(pool) SetGeneratorNode(pool, name);

	node->genId = MET_lookup_generator(tdbb, name);
	if (node->genId < 0)
		PAR_error(csb, Arg::Gds(isc_gennotdef) << Arg::Str(name));

	node->value = PAR_parse_value(tdbb, csb);

	return node;
}

SetGeneratorNode* SetGeneratorNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	SetGeneratorNode* node = FB_NEW(getPool()) SetGeneratorNode(getPool(), name,
		PASS1_node(dsqlScratch, dsqlValue));

	dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_SET_GENERATOR);

	return node;
}

void SetGeneratorNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SetGeneratorNode";
}

void SetGeneratorNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_set_generator);
	dsqlScratch->appendNullString(name.c_str());
	GEN_expr(dsqlScratch, dsqlValue);
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
const StmtNode* SuspendNode::execute(thread_db* tdbb, jrd_req* request, ExeState* /*exeState*/) const
{
	switch (request->req_operation)
	{
		case jrd_req::req_evaluate:
		{
			// ASF: If this is the send in the tail of a procedure and the procedure was called
			// with a SELECT, don't run all the send statements. It may make validations fail when
			// the procedure didn't return any rows. See CORE-2204.
			// But we should run the last assignment, as it's the one who make the procedure stop.

			if (!request->getStatement()->procedure || !(request->req_flags & req_proc_fetch))
				return statement;

			const CompoundStmtNode* list = parentStmt->as<CompoundStmtNode>();

			if (list && !list->parentStmt && list->statements[list->statements.getCount() - 1] == this)
			{
				list = statement->as<CompoundStmtNode>();

				if (list && list->onlyAssignments && list->statements.hasData())
				{
					// This is the assignment that sets the EOS parameter.
					const AssignmentNode* assign = static_cast<const AssignmentNode*>(
						list->statements[list->statements.getCount() - 1].getObject());
					EXE_assignment(tdbb, assign);
				}
				else
					return statement;
			}
			else
				return statement;

			// fall into
		}

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
	dsqlScratch->appendUChar(blrOp);
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


StmtNode* UpdateOrInsertNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = getPool();

	if (!dsqlScratch->isPsql())
		dsqlScratch->flags |= DsqlCompilerScratch::FLAG_UPDATE_OR_INSERT;

	const MetaName& relation_name = ExprNode::as<RelationSourceNode>(dsqlRelation)->dsqlName;
	MetaName base_name = relation_name;

	dsql_nod* values = dsqlValues;

	// Build the INSERT node.
	StoreNode* insert = FB_NEW(pool) StoreNode(pool);
	insert->dsqlRelation = dsqlRelation;
	insert->dsqlFields = dsqlFields;
	insert->dsqlValues = values;
	insert->dsqlReturning = dsqlReturning;
	insert = insert->internalDsqlPass(dsqlScratch, true)->as<StoreNode>();
	fb_assert(insert);

	dsql_ctx* context = ExprNode::as<RelationSourceNode>(insert->dsqlRelation)->dsqlContext;
	DEV_BLKCHK(context, dsql_type_ctx);

	dsql_rel* relation = context->ctx_relation;
	dsql_nod* fields = dsqlFields;

	// If a field list isn't present, build one using the same rules of INSERT INTO table VALUES ...
	if (!fields)
		fields = dsqlExplodeFields(relation);

	// Maintain a pair of view's field name / base field name.
	MetaNamePairMap view_fields;

	if ((relation->rel_flags & REL_view) && !dsqlMatching)
	{
		dsql_rel* base_rel = METD_get_view_base(dsqlScratch->getTransaction(), dsqlScratch,
			relation_name.c_str(), view_fields);

		// Get the base table name if there is only one.
		if (base_rel)
			base_name = base_rel->rel_name;
		else
			ERRD_post(Arg::Gds(isc_upd_ins_with_complex_view));
	}

	dsql_nod* matching = dsqlMatching;
	UCHAR equality_type;

	if (matching)
	{
		equality_type = blr_equiv;

		dsqlScratch->context->push(context);
		++dsqlScratch->scopeLevel;

		const dsql_nod* matching_fields = PASS1_node_psql(dsqlScratch, matching, false);

		--dsqlScratch->scopeLevel;
		dsqlScratch->context->pop();

		{	// scope
			Array<dsql_nod*> newValues;

			for (USHORT i = 0; i < matching_fields->nod_count; ++i)
				newValues.add(matching_fields->nod_arg[i]);

			dsqlFieldAppearsOnce(newValues, "UPDATE OR INSERT");
		}	// scope
	}
	else
	{
		equality_type = blr_eql;

		matching = METD_get_primary_key(dsqlScratch->getTransaction(), base_name.c_str());

		if (!matching)
			ERRD_post(Arg::Gds(isc_primary_key_required) << base_name);
	}

	// Build a boolean to use in the UPDATE dsqlScratch.
	dsql_nod* match = NULL;
	USHORT match_count = 0;

	CompoundStmtNode* list = FB_NEW(pool) CompoundStmtNode(pool);

	CompoundStmtNode* assignments = FB_NEW(pool) CompoundStmtNode(pool);
	dsql_nod** field_ptr = fields->nod_arg;
	dsql_nod** value_ptr = values->nod_arg;

	Array<NestConst<StmtNode> >& insertStatements =
		insert->statement->as<CompoundStmtNode>()->statements;

	for (const dsql_nod* const* const field_end = field_ptr + fields->nod_count;
		 field_ptr < field_end; field_ptr++, value_ptr++)
	{
		DEV_BLKCHK(*field_ptr, dsql_type_nod);
		DEV_BLKCHK(*value_ptr, dsql_type_nod);

		AssignmentNode* assign = FB_NEW(pool) AssignmentNode(pool);
		assign->dsqlAsgnFrom = *value_ptr;
		assign->dsqlAsgnTo = *field_ptr;
		assignments->statements.add(assign);

		fb_assert((*field_ptr)->nod_type == Dsql::nod_field_name);

		// When relation is a view and MATCHING was not specified, field_name
		// stores the base field name that is what we should find in the primary
		// key of base table.
		MetaName field_name;

		if ((relation->rel_flags & REL_view) && !dsqlMatching)
		{
			view_fields.get(
				MetaName(((dsql_str*) (*field_ptr)->nod_arg[Dsql::e_fln_name])->str_data),
				field_name);
		}
		else
			field_name = ((dsql_str*) (*field_ptr)->nod_arg[Dsql::e_fln_name])->str_data;

		if (field_name.hasData())
		{
			dsql_nod** matching_ptr = matching->nod_arg;

			for (const dsql_nod* const* const matching_end = matching_ptr + matching->nod_count;
				 matching_ptr < matching_end; matching_ptr++)
			{
				DEV_BLKCHK(*matching_ptr, dsql_type_nod);
				fb_assert((*matching_ptr)->nod_type == Dsql::nod_field_name);

				const MetaName
					testField(((dsql_str*)(*matching_ptr)->nod_arg[Dsql::e_fln_name])->str_data);

				if (testField == field_name)
				{
					++match_count;

					const size_t fieldPos = field_ptr - fields->nod_arg;
					dsql_nod*& expr = insertStatements[fieldPos]->as<AssignmentNode>()->dsqlAsgnFrom;
					dsql_nod* var = dsqlPassHiddenVariable(dsqlScratch, expr);

					if (var)
					{
						AssignmentNode* varAssign = FB_NEW(pool) AssignmentNode(pool);
						varAssign->dsqlAsgnFrom = expr;
						varAssign->dsqlAsgnTo = var;

						list->statements.add(varAssign);

						assign->dsqlAsgnFrom = expr = var;
					}
					else
						var = *value_ptr;

					ComparativeBoolNode* eqlNode = FB_NEW(pool) ComparativeBoolNode(pool,
						equality_type, *field_ptr, var);

					dsql_nod* eqlNod = MAKE_node(Dsql::nod_class_exprnode, 1);
					eqlNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(eqlNode);

					match = PASS1_compose(match, eqlNod, blr_and);
				}
			}
		}
	}

	// check if implicit or explicit MATCHING is valid
	if (match_count != matching->nod_count)
	{
		if (dsqlMatching)
			ERRD_post(Arg::Gds(isc_upd_ins_doesnt_match_matching));
		else
			ERRD_post(Arg::Gds(isc_upd_ins_doesnt_match_pk) << base_name);
	}

	// build the UPDATE node
	ModifyNode* update = FB_NEW(pool) ModifyNode(pool);
	update->dsqlRelation = dsqlRelation;
	update->statement = assignments;
	update->dsqlBoolean = match;

	if (dsqlReturning)
	{
		update->dsqlRseFlags = NOD_SELECT_EXPR_SINGLETON;
		update->statement2 = ReturningProcessor::clone(
			dsqlScratch, dsqlReturning, insert->statement2);
	}

	update = update->internalDsqlPass(dsqlScratch, true)->as<ModifyNode>();
	fb_assert(update);

	// test if ROW_COUNT = 0

	ComparativeBoolNode* eqlNode = FB_NEW(pool) ComparativeBoolNode(pool,
		blr_eql, MAKE_node(Dsql::nod_class_exprnode, 1), MAKE_const_slong(0));

	eqlNode->dsqlArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(FB_NEW(pool) InternalInfoNode(pool,
		MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_ROWS_AFFECTED))));

	dsql_nod* eqlNod = MAKE_node(Dsql::nod_class_exprnode, 1);
	eqlNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(eqlNode);

	const ULONG save_flags = dsqlScratch->flags;
	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_BLOCK;	// to compile ROW_COUNT
	eqlNod = PASS1_node(dsqlScratch, eqlNod);
	dsqlScratch->flags = save_flags;

	// If (ROW_COUNT = 0) then INSERT.
	IfNode* ifNode = FB_NEW(pool) IfNode(pool);
	ifNode->dsqlCondition = eqlNod;
	ifNode->trueAction = insert;

	// Build the temporary vars / UPDATE / IF nodes.

	list->statements.add(update);
	list->statements.add(ifNode);

	// If RETURNING is present, type is already DsqlCompiledStatement::TYPE_EXEC_PROCEDURE.
	if (!dsqlReturning)
		dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_INSERT);

	return SavepointEncloseNode::make(getPool(), dsqlScratch, list);
}

void UpdateOrInsertNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "UpdateOrInsertNode";
}

void UpdateOrInsertNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
}


//--------------------


// Generate a field list that correspond to table fields.
static dsql_nod* dsqlExplodeFields(dsql_rel* relation)
{
	DsqlNodStack stack;

	for (dsql_fld* field = relation->rel_fields; field; field = field->fld_next)
	{
		// CVC: Ann Harrison requested to skip COMPUTED fields in INSERT w/o field list.
		if (field->fld_flags & FLD_computed)
		{
			continue;
		}

		stack.push(MAKE_field_name(field->fld_name.c_str()));
	}

	return MAKE_list(stack);
}

// Find dbkey for named relation in statement's saved dbkeys.
static dsql_par* dsqlFindDbKey(const dsql_req* request, const dsql_nod* relation_name)
{
	DEV_BLKCHK(request, dsql_type_req);
	DEV_BLKCHK(relation_name, dsql_type_nod);

	const dsql_msg* message = request->getStatement()->getReceiveMsg();
	dsql_par* candidate = NULL;
	const MetaName& relName = ExprNode::as<RelationSourceNode>(relation_name)->dsqlName;

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		dsql_par* parameter = message->msg_parameters[i];

		if (parameter->par_dbkey_relname.hasData() && parameter->par_dbkey_relname == relName)
		{
			if (candidate)
				return NULL;

			candidate = parameter;
		}
	}

	return candidate;
}

// Find record version for relation in statement's saved record version.
static dsql_par* dsqlFindRecordVersion(const dsql_req* request, const dsql_nod* relation_name)
{
	DEV_BLKCHK(request, dsql_type_req);
	DEV_BLKCHK(relation_name, dsql_type_nod);

	const dsql_msg* message = request->getStatement()->getReceiveMsg();
	dsql_par* candidate = NULL;
	const MetaName& relName = ExprNode::as<RelationSourceNode>(relation_name)->dsqlName;

	for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
	{
		dsql_par* parameter = message->msg_parameters[i];

		if (parameter->par_rec_version_relname.hasData() &&
			parameter->par_rec_version_relname == relName)
		{
			if (candidate)
				return NULL;

			candidate = parameter;
		}
	}

	return candidate;
}

// Generate DML header for INSERT/UPDATE/DELETE.
static const dsql_msg* dsqlGenDmlHeader(DsqlCompilerScratch* dsqlScratch, dsql_nod* dsqlRse)
{
	const dsql_msg* message = NULL;
	const bool innerSend = !dsqlRse || (dsqlScratch->flags & DsqlCompilerScratch::FLAG_UPDATE_OR_INSERT);
	const bool merge = dsqlScratch->flags & DsqlCompilerScratch::FLAG_MERGE;

	if (dsqlScratch->getStatement()->getType() == DsqlCompiledStatement::TYPE_EXEC_PROCEDURE &&
		!innerSend && !merge)
	{
		if ((message = dsqlScratch->getStatement()->getReceiveMsg()))
		{
			dsqlScratch->appendUChar(blr_send);
			dsqlScratch->appendUChar(message->msg_number);
		}
	}

	if (dsqlRse)
	{
		dsqlScratch->appendUChar(blr_for);
		GEN_expr(dsqlScratch, dsqlRse);
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

	return message;
}

// Get the context of a relation, procedure or derived table.
static dsql_ctx* dsqlGetContext(const dsql_nod* node)
{
	const ProcedureSourceNode* procNode;
	const RelationSourceNode* relNode;
	const RseNode* rseNode;

	if ((procNode = ExprNode::as<ProcedureSourceNode>(node)))
		return procNode->dsqlContext;

	if ((relNode = ExprNode::as<RelationSourceNode>(node)))
		return relNode->dsqlContext;

	if ((rseNode = ExprNode::as<RseNode>(node)))
		return rseNode->dsqlContext;

	fb_assert(false);
	return NULL;
}

// Get the contexts of a relation, procedure, derived table or a list of joins.
static void dsqlGetContexts(DsqlContextStack& contexts, const dsql_nod* node)
{
	const ProcedureSourceNode* procNode;
	const RelationSourceNode* relNode;
	const RseNode* rseNode;

	if ((procNode = ExprNode::as<ProcedureSourceNode>(node)))
		contexts.push(procNode->dsqlContext);
	else if ((relNode = ExprNode::as<RelationSourceNode>(node)))
		contexts.push(relNode->dsqlContext);
	else if ((rseNode = ExprNode::as<RseNode>(node)))
	{
		if (rseNode->dsqlContext)	// derived table
			contexts.push(rseNode->dsqlContext);
		else	// joins
		{
			dsql_nod** ptr = rseNode->dsqlStreams->nod_arg;

			for (const dsql_nod* const* const end = ptr + rseNode->dsqlStreams->nod_count;
				 ptr != end;
				 ++ptr)
			{
				dsqlGetContexts(contexts, *ptr);
			}
		}
	}
	else
	{
		fb_assert(false);
	}
}

// Create a compound statement to initialize returning parameters.
static StmtNode* dsqlNullifyReturning(DsqlCompilerScratch* dsqlScratch, StmtNode* input, bool returnList)
{
	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	StmtNode* returning = NULL;
	EraseNode* eraseNode;
	ModifyNode* modifyNode;
	StoreNode* storeNode;

	if (eraseNode = input->as<EraseNode>())
		returning = eraseNode->statement;
	else if (modifyNode = input->as<ModifyNode>())
		returning = modifyNode->statement2;
	else if (storeNode = input->as<StoreNode>())
		returning = storeNode->statement2;
	else
	{
		fb_assert(false);
	}

	if (dsqlScratch->isPsql() || !returning)
		return returnList ? input : NULL;

	// If this is a RETURNING in DSQL, we need to initialize the output
	// parameters with NULL, to return in case of empty resultset.
	// Note: this may be changed in the future, i.e. return empty resultset
	// instead of NULLs. In this case, I suppose this function could be
	// completely removed.

	// nod_returning was already processed
	CompoundStmtNode* returningStmt = returning->as<CompoundStmtNode>();
	fb_assert(returningStmt);

	CompoundStmtNode* nullAssign = FB_NEW(pool) CompoundStmtNode(pool);

	NestConst<StmtNode>* ret_ptr = returningStmt->statements.begin();
	NestConst<StmtNode>* null_ptr = nullAssign->statements.getBuffer(returningStmt->statements.getCount());

	for (const NestConst<StmtNode>* const end = ret_ptr + returningStmt->statements.getCount();
		 ret_ptr != end;
		 ++ret_ptr, ++null_ptr)
	{
		AssignmentNode* assign = FB_NEW(pool) AssignmentNode(pool);
		assign->dsqlAsgnFrom = MAKE_node(Dsql::nod_class_exprnode, 1);
		assign->dsqlAsgnFrom->nod_arg[0] = reinterpret_cast<dsql_nod*>(FB_NEW(pool) NullNode(pool));
		assign->dsqlAsgnTo = (*ret_ptr)->as<AssignmentNode>()->dsqlAsgnTo;
		*null_ptr = assign;
	}

	// If asked for, return a compound statement with the initialization and the
	// original statement.
	if (returnList)
	{
		CompoundStmtNode* list = FB_NEW(pool) CompoundStmtNode(pool);
		list->statements.add(nullAssign);
		list->statements.add(input);
		return list;
	}

	return nullAssign;	// return the initialization statement.
}

// Check that a field is named only once in INSERT or UPDATE statements.
static void dsqlFieldAppearsOnce(const Array<dsql_nod*>& values, const char* command)
{
	for (size_t i = 0; i < values.getCount(); ++i)
	{
		const FieldNode* field1 = ExprNode::as<FieldNode>(values[i]);
		fb_assert(field1);

		const MetaName& name1 = field1->dsqlField->fld_name;

		for (size_t j = i + 1; j < values.getCount(); ++j)
		{
			const FieldNode* field2 = ExprNode::as<FieldNode>(values[j]);
			fb_assert(field2);

			const MetaName& name2 = field2->dsqlField->fld_name;

			if (name1 == name2)
			{
				string str = field1->dsqlContext->ctx_relation->rel_name.c_str();
				str += ".";
				str += name1.c_str();

				//// FIXME: line/column is not very accurate for MERGE ... INSERT.
				ERRD_post(
					Arg::Gds(isc_sqlerr) << Arg::Num(-206) <<
					Arg::Gds(isc_dsql_no_dup_name) << str << command <<
					Arg::Gds(isc_dsql_line_col_error) <<
						Arg::Num(values[j]->nod_line) << Arg::Num(values[j]->nod_column));
			}
		}
	}
}

static Array<dsql_nod*>* dsqlPassArray(DsqlCompilerScratch* dsqlScratch, Array<dsql_nod*>* input)
{
	if (!input)
		return NULL;

	MemoryPool& pool = dsqlScratch->getStatement()->getPool();
	Array<dsql_nod*>* output = FB_NEW(pool) Array<dsql_nod*>(pool, input->getCount());
	dsql_nod** ptr = input->begin();

	for (const dsql_nod* const* const end = input->end(); ptr != end; ++ptr)
		output->add(PASS1_node(dsqlScratch, *ptr));

	return output;
}

// Turn a cursor reference into a record selection expression.
static dsql_ctx* dsqlPassCursorContext( DsqlCompilerScratch* dsqlScratch, const dsql_nod* cursor,
	const dsql_nod* relation_name)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(cursor, dsql_type_nod);
	DEV_BLKCHK(relation_name, dsql_type_nod);

	const MetaName& relName = ExprNode::as<RelationSourceNode>(relation_name)->dsqlName;
	const MetaName& cursorName = StmtNode::as<DeclareCursorNode>(cursor)->dsqlName;

	// this function must throw an error if no cursor was found
	const DeclareCursorNode* node = PASS1_cursor_name(dsqlScratch, cursorName,
		DeclareCursorNode::CUR_TYPE_ALL, true);
	fb_assert(node);

	const RseNode* rse = ExprNode::as<RseNode>(node->dsqlRse);
	fb_assert(rse);

	if (rse->dsqlDistinct)
	{
		// cursor with DISTINCT is not updatable
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-510) <<
				  Arg::Gds(isc_dsql_cursor_update_err) << cursorName);
	}

	const dsql_nod* temp = rse->dsqlStreams;
	dsql_ctx* context = NULL;

	if (temp->nod_type != Dsql::nod_class_exprnode)
	{
		dsql_nod* const* ptr = temp->nod_arg;

		for (const dsql_nod* const* const end = ptr + temp->nod_count; ptr != end; ++ptr)
		{
			DEV_BLKCHK(*ptr, dsql_type_nod);
			dsql_nod* r_node = *ptr;
			RelationSourceNode* relNode = ExprNode::as<RelationSourceNode>(r_node);

			if (relNode)
			{
				dsql_ctx* candidate = relNode->dsqlContext;
				DEV_BLKCHK(candidate, dsql_type_ctx);
				const dsql_rel* relation = candidate->ctx_relation;

				if (relation->rel_name == relName)
				{
					if (context)
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
								  Arg::Gds(isc_dsql_cursor_err) <<
								  Arg::Gds(isc_dsql_cursor_rel_ambiguous) << Arg::Str(relName) <<
																			 cursorName);
					}
					else
						context = candidate;
				}
			}
			else if (ExprNode::as<AggregateSourceNode>(r_node))
			{
				// cursor with aggregation is not updatable
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-510) <<
						  Arg::Gds(isc_dsql_cursor_update_err) << cursorName);
			}
			// note that UnionSourceNode and joins will cause the error below,
			// as well as derived tables. Some cases deserve fixing in the future
		}
	}

	if (!context)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
				  Arg::Gds(isc_dsql_cursor_err) <<
				  Arg::Gds(isc_dsql_cursor_rel_not_found) << Arg::Str(relName) << cursorName);
	}

	return context;
}

// Turn a cursor reference into a record selection expression.
static dsql_nod* dsqlPassCursorReference( DsqlCompilerScratch* dsqlScratch, const dsql_nod* cursor,
	dsql_nod* relation_name)
{
	DEV_BLKCHK(dsqlScratch, dsql_type_req);
	DEV_BLKCHK(cursor, dsql_type_nod);
	DEV_BLKCHK(relation_name, dsql_type_nod);

	thread_db* tdbb = JRD_get_thread_data();
	MemoryPool& pool = *tdbb->getDefaultPool();

	// Lookup parent dsqlScratch

	const MetaName& cursorName = StmtNode::as<DeclareCursorNode>(cursor)->dsqlName;

	dsql_req* const* symbol = dsqlScratch->getAttachment()->dbb_cursors.get(cursorName.c_str());

	if (!symbol)
	{
		// cursor is not found
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
				  Arg::Gds(isc_dsql_cursor_err) <<
				  Arg::Gds(isc_dsql_cursor_not_found) << cursorName);
	}

	dsql_req* parent = *symbol;

	// Verify that the cursor is appropriate and updatable

	dsql_par* rv_source = dsqlFindRecordVersion(parent, relation_name);

	dsql_par* source;
	if (parent->getStatement()->getType() != DsqlCompiledStatement::TYPE_SELECT_UPD ||
		!rv_source || !(source = dsqlFindDbKey(parent, relation_name)))
	{
		// cursor is not updatable
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-510) <<
				  Arg::Gds(isc_dsql_cursor_update_err) << cursorName);
	}

	dsqlScratch->getStatement()->setParentRequest(parent);
	dsqlScratch->getStatement()->setParentDbKey(source);
	dsqlScratch->getStatement()->setParentRecVersion(rv_source);
	parent->cursors.add(dsqlScratch->getStatement());

	// Build record selection expression

	RseNode* rse = FB_NEW(pool) RseNode(pool);
	dsql_nod* temp = rse->dsqlStreams = MAKE_node(Dsql::nod_list, 1);
	dsql_nod* relation_node = PASS1_relation(dsqlScratch, relation_name);
	temp->nod_arg[0] = relation_node;

	ComparativeBoolNode* eqlNode = FB_NEW(pool) ComparativeBoolNode(pool,
		blr_eql, MAKE_node(Dsql::nod_class_exprnode, 1), MAKE_node(Dsql::nod_class_exprnode, 1));

	dsql_nod* node = MAKE_node(Dsql::nod_class_exprnode, 1);
	node->nod_arg[0] = reinterpret_cast<dsql_nod*>(eqlNode);

	rse->dsqlWhere = node;

	RecordKeyNode* dbKeyNode = FB_NEW(pool) RecordKeyNode(pool, blr_dbkey);
	dbKeyNode->dsqlRelation = relation_node;
	eqlNode->dsqlArg1->nod_count = 0;
	eqlNode->dsqlArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(dbKeyNode);

	dsql_par* parameter = MAKE_parameter(dsqlScratch->getStatement()->getSendMsg(),
		false, false, 0, NULL);
	dsqlScratch->getStatement()->setDbKey(parameter);

	ParameterNode* paramNode = FB_NEW(pool) ParameterNode(pool);
	eqlNode->dsqlArg2->nod_count = 0;
	eqlNode->dsqlArg2->nod_arg[0] = reinterpret_cast<dsql_nod*>(paramNode);
	paramNode->dsqlParameterIndex = parameter->par_index;
	paramNode->dsqlParameter = parameter;

	parameter->par_desc = source->par_desc;

	// record version will be set only for V4 - for the parent select cursor
	if (rv_source)
	{
		eqlNode = FB_NEW(pool) ComparativeBoolNode(pool,
			blr_eql, MAKE_node(Dsql::nod_class_exprnode, 1), MAKE_node(Dsql::nod_class_exprnode, 1));

		node = MAKE_node(Dsql::nod_class_exprnode, 1);
		node->nod_arg[0] = reinterpret_cast<dsql_nod*>(eqlNode);

		dbKeyNode = FB_NEW(pool) RecordKeyNode(pool, blr_record_version);
		dbKeyNode->dsqlRelation = relation_node;
		eqlNode->dsqlArg1->nod_count = 0;
		eqlNode->dsqlArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(dbKeyNode);

		parameter = MAKE_parameter(dsqlScratch->getStatement()->getSendMsg(),
			false, false, 0, NULL);
		dsqlScratch->getStatement()->setRecVersion(parameter);

		paramNode = FB_NEW(pool) ParameterNode(pool);
		eqlNode->dsqlArg2->nod_count = 0;
		eqlNode->dsqlArg2->nod_arg[0] = reinterpret_cast<dsql_nod*>(paramNode);
		paramNode->dsqlParameterIndex = parameter->par_index;
		paramNode->dsqlParameter = parameter;

		parameter->par_desc = rv_source->par_desc;

		rse->dsqlWhere = PASS1_compose(rse->dsqlWhere, node, blr_and);
	}

	dsql_nod* rseNod = MAKE_node(Dsql::nod_class_exprnode, 1);
	rseNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(rse);

	return rseNod;
}

// Create (if necessary) a hidden variable to store a temporary value.
static dsql_nod* dsqlPassHiddenVariable(DsqlCompilerScratch* dsqlScratch, dsql_nod* expr)
{
	thread_db* tdbb = JRD_get_thread_data();

	// For some node types, it's better to not create temporary value.
	if (expr->nod_type == Dsql::nod_class_exprnode)
	{
		switch (ExprNode::fromLegacy(expr)->type)
		{
			case ExprNode::TYPE_CURRENT_DATE:
			case ExprNode::TYPE_CURRENT_TIME:
			case ExprNode::TYPE_CURRENT_TIMESTAMP:
			case ExprNode::TYPE_CURRENT_ROLE:
			case ExprNode::TYPE_CURRENT_USER:
			case ExprNode::TYPE_FIELD:
			case ExprNode::TYPE_INTERNAL_INFO:
			case ExprNode::TYPE_LITERAL:
			case ExprNode::TYPE_NULL:
			case ExprNode::TYPE_PARAMETER:
			case ExprNode::TYPE_RECORD_KEY:
			case ExprNode::TYPE_VARIABLE:
				return NULL;
		}
	}

	VariableNode* varNode = FB_NEW(*tdbb->getDefaultPool()) VariableNode(*tdbb->getDefaultPool());
	varNode->dsqlVar = dsqlScratch->makeVariable(NULL, "", dsql_var::TYPE_HIDDEN,
		0, 0, dsqlScratch->hiddenVarsNumber++);

	dsql_nod* varNod = MAKE_node(Dsql::nod_class_exprnode, 1);
	varNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(varNode);

	MAKE_desc(dsqlScratch, &varNode->dsqlVar->desc, expr);
	varNod->nod_desc = varNode->dsqlVar->desc;

	return varNod;
}

// Compile a RETURNING clause (nod_returning or not).
static StmtNode* dsqlProcessReturning(DsqlCompilerScratch* dsqlScratch, ReturningClause* input,
	StmtNode* stmt)
{
	thread_db* tdbb = JRD_get_thread_data();

	if (stmt)
	{
		const bool isPsql = dsqlScratch->isPsql();

		PsqlChanger changer(dsqlScratch, false);
		stmt = stmt->dsqlPass(dsqlScratch);

		if (!isPsql)
			dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_EXEC_PROCEDURE);

		return stmt;
	}

	if (!input)
		return NULL;

	MemoryPool& pool = *tdbb->getDefaultPool();

	dsql_nod* const source = PASS1_node_psql(dsqlScratch, input->first, false);

	dsqlScratch->flags |= DsqlCompilerScratch::FLAG_RETURNING_INTO;
	Array<dsql_nod*>* target = dsqlPassArray(dsqlScratch, input->second);
	dsqlScratch->flags &= ~DsqlCompilerScratch::FLAG_RETURNING_INTO;

	if (!dsqlScratch->isPsql() && target)
	{
		// RETURNING INTO is not allowed syntax for DSQL
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown
				  Arg::Gds(isc_token_err) <<
				  Arg::Gds(isc_random) << Arg::Str("INTO"));
	}
	else if (dsqlScratch->isPsql() && !target)
	{
		// This trick because we don't copy lexer positions when copying lists.
		const dsql_nod* errSrc = input->first;
		fb_assert(errSrc->nod_type == Dsql::nod_list);
		// RETURNING without INTO is not allowed for PSQL
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  Arg::Gds(isc_command_end_err2) << Arg::Num(errSrc->nod_line) <<
													Arg::Num(errSrc->nod_column));
	}

	const int count = source->nod_count;
	fb_assert(count);

	CompoundStmtNode* node = FB_NEW(pool) CompoundStmtNode(pool);

	if (target)
	{
		// PSQL case
		fb_assert(dsqlScratch->isPsql());

		if (count != target->getCount())
		{
			// count of column list and value list don't match
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
					  Arg::Gds(isc_dsql_var_count_err));
		}

		dsql_nod** src = source->nod_arg;
		dsql_nod** dst = target->begin();

		for (const dsql_nod* const* const end = src + source->nod_count; src != end; ++src, ++dst)
		{
			AssignmentNode* temp = FB_NEW(pool) AssignmentNode(pool);
			temp->dsqlAsgnFrom = *src;
			temp->dsqlAsgnTo = *dst;

			node->statements.add(temp);
		}
	}
	else
	{
		// DSQL case
		fb_assert(!dsqlScratch->isPsql());

		dsql_nod** src = source->nod_arg;

		for (const dsql_nod* const* const end = src + source->nod_count; src != end; ++src)
		{
			dsql_par* parameter = MAKE_parameter(dsqlScratch->getStatement()->getReceiveMsg(),
				true, true, 0, *src);
			parameter->par_node = *src;
			MAKE_desc(dsqlScratch, &parameter->par_desc, *src);
			parameter->par_desc.dsc_flags |= DSC_nullable;

			ParameterNode* paramNode = FB_NEW(*tdbb->getDefaultPool()) ParameterNode(
				*tdbb->getDefaultPool());
			paramNode->dsqlParameterIndex = parameter->par_index;
			paramNode->dsqlParameter = parameter;

			dsql_nod* paramNod = MAKE_node(Dsql::nod_class_exprnode, 1);
			paramNod->nod_count = 0;
			paramNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(paramNode);

			AssignmentNode* temp = FB_NEW(pool) AssignmentNode(pool);
			temp->dsqlAsgnFrom = *src;
			temp->dsqlAsgnTo = paramNod;

			node->statements.add(temp);
		}
	}

	if (!dsqlScratch->isPsql())
		dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_EXEC_PROCEDURE);

	return node;
}

// Setup parameter name.
// This function was added as a part of array data type support for InterClient. It is called when
// either "insert" or "update" statements are parsed. If the statements have input parameters, then
// the parameter is assigned the name of the field it is being inserted (or updated). The same goes
// to the name of a relation.
// The names are assigned to the parameter only if the field is of array data type.
static void dsqlSetParameterName( dsql_nod* par_node, const dsql_nod* fld_node, const dsql_rel* relation)
{
	DEV_BLKCHK(par_node, dsql_type_nod);
	DEV_BLKCHK(fld_node, dsql_type_nod);
	DEV_BLKCHK(relation, dsql_type_dsql_rel);

	if (!par_node)
		return;

	const FieldNode* fieldNode = ExprNode::as<FieldNode>(fld_node);
	fb_assert(fieldNode);	// Could it be something else ???

	if (fieldNode->dsqlDesc.dsc_dtype != dtype_array)
		return;

	switch (par_node->nod_type)
	{
		case Dsql::nod_class_exprnode:
		{
			ExprNode* exprNode = reinterpret_cast<ExprNode*>(par_node->nod_arg[0]);

			switch (exprNode->type)
			{
				case ExprNode::TYPE_ARITHMETIC:
				case ExprNode::TYPE_CONCATENATE:
				case ExprNode::TYPE_EXTRACT:
				case ExprNode::TYPE_NEGATE:
				case ExprNode::TYPE_STR_CASE:
				case ExprNode::TYPE_STR_LEN:
				case ExprNode::TYPE_SUBSTRING:
				case ExprNode::TYPE_SUBSTRING_SIMILAR:
				case ExprNode::TYPE_TRIM:
					for (dsql_nod*** i = exprNode->dsqlChildNodes.begin();
						 i != exprNode->dsqlChildNodes.end(); ++i)
					{
						dsqlSetParameterName(**i, fld_node, relation);
					}
					break;

				case ExprNode::TYPE_PARAMETER:
				{
					ParameterNode* paramNode = exprNode->as<ParameterNode>();
					dsql_par* parameter = paramNode->dsqlParameter;
					parameter->par_name = fieldNode->dsqlField->fld_name.c_str();
					parameter->par_rel_name = relation->rel_name.c_str();
					break;
				}
			}
		}
		return;

		default:
			return;
	}
}

// Setup parameter parameters name.
static void dsqlSetParametersName(CompoundStmtNode* statements, const dsql_nod* rel_node)
{
	DEV_BLKCHK(rel_node, dsql_type_nod);

	const dsql_ctx* context = ExprNode::as<RelationSourceNode>(rel_node)->dsqlContext;
	DEV_BLKCHK(context, dsql_type_ctx);
	const dsql_rel* relation = context->ctx_relation;

	size_t count = statements->statements.getCount();
	NestConst<StmtNode>* ptr = statements->statements.begin();

	for (const NestConst<StmtNode>* const end = ptr + count; ptr != end; ++ptr)
	{
		const AssignmentNode* assign = (*ptr)->as<AssignmentNode>();

		if (assign)
			dsqlSetParameterName(assign->dsqlAsgnFrom, assign->dsqlAsgnTo, relation);
		else
		{
			fb_assert(false);
		}
	}
}

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
// CVC: The code no longer matches this comment.
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
