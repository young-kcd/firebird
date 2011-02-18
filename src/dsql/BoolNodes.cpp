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
 * Adriano dos Santos Fernandes - refactored from pass1.cpp, gen.cpp, cmp.cpp, par.cpp and evl.cpp
 */

#include "firebird.h"
#include "../common/common.h"
#include "../common/classes/VaryStr.h"
#include "../dsql/BoolNodes.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/node.h"
#include "../jrd/align.h"
#include "../jrd/blr.h"
#include "../common/quad.h"
#include "../jrd/tra.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/Optimizer.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"
#include "../dsql/ddl_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/pass1_proto.h"

using namespace Firebird;
using namespace Jrd;

namespace Jrd {


// Maximum members in "IN" list. For eg. SELECT * FROM T WHERE F IN (1, 2, 3, ...)
// Bug 10061, bsriram - 19-Apr-1999
static const int MAX_MEMBER_LIST = 1500;

static const int TEMP_LENGTH = 128;


//--------------------


namespace
{
	// Copy sub expressions (including subqueries).
	class SubExprNodeCopier : public NodeCopier
	{
	public:
		explicit SubExprNodeCopier(CompilerScratch* aCsb)
			: NodeCopier(aCsb, localMap)
		{
			// Initialize the map so all streams initially resolve to the original number. As soon
			// copy creates new streams, the map are being overwritten.
			for (unsigned i = 0; i < JrdStatement::MAP_LENGTH; ++i)
				localMap[i] = i;
		}

	private:
		UCHAR localMap[JrdStatement::MAP_LENGTH];
	};
}	// namespace


//--------------------


bool BoolExprNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	if (nodFlags & FLAG_DEOPTIMIZE)
		return false;

	return ExprNode::computable(csb, stream, allowOnlyCurrentStream);
}

BoolExprNode* BoolExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	pass2Boolean1(tdbb, csb);
	ExprNode::pass2(tdbb, csb);
	pass2Boolean2(tdbb, csb);

	if (nodFlags & FLAG_INVARIANT)
	{
		// Bind values of invariant nodes to top-level RSE (if present)

		if (csb->csb_current_nodes.hasData())
		{
			RseOrExprNode& topRseNode = csb->csb_current_nodes[0];
			fb_assert(topRseNode.rseNode);

			if (!topRseNode.rseNode->rse_invariants)
			{
				topRseNode.rseNode->rse_invariants =
					FB_NEW(*tdbb->getDefaultPool()) VarInvariantArray(*tdbb->getDefaultPool());
			}

			topRseNode.rseNode->rse_invariants->add(impureOffset);
		}
	}

	return this;
}


//--------------------


static RegisterBoolNode<BinaryBoolNode> regBinaryBoolNodeAnd(blr_and);
static RegisterBoolNode<BinaryBoolNode> regBinaryBoolNodeOr(blr_or);

BinaryBoolNode::BinaryBoolNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aArg1, dsql_nod* aArg2)
	: TypedNode<BoolExprNode, ExprNode::TYPE_BINARY_BOOL>(pool),
	  blrOp(aBlrOp),
	  dsqlArg1(aArg1),
	  dsqlArg2(aArg2),
	  arg1(NULL),
	  arg2(NULL)
{
	addChildNode(dsqlArg1, arg1);
	addChildNode(dsqlArg2, arg2);
}

DmlNode* BinaryBoolNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	BinaryBoolNode* node = FB_NEW(pool) BinaryBoolNode(pool, blrOp);
	node->arg1 = PAR_parse_boolean(tdbb, csb);
	node->arg2 = PAR_parse_boolean(tdbb, csb);
	return node;
}

void BinaryBoolNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("BinaryBoolNode (%d)", blrOp);
	BoolExprNode::print(text, nodes);
}

BoolExprNode* BinaryBoolNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) BinaryBoolNode(getPool(), blrOp,
		PASS1_node(dsqlScratch, dsqlArg1), PASS1_node(dsqlScratch, dsqlArg2));
}

void BinaryBoolNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

bool BinaryBoolNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!BoolExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const BinaryBoolNode* o = other->as<BinaryBoolNode>();
	fb_assert(o)

	return blrOp == o->blrOp;
}

bool BinaryBoolNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	BinaryBoolNode* otherNode = other->as<BinaryBoolNode>();

	if (!otherNode || blrOp != otherNode->blrOp)
		return false;

	if (arg1->sameAs(tdbb, csb, otherNode->arg1) &&
		arg2->sameAs(tdbb, csb, otherNode->arg2))
	{
		return true;
	}

	// A AND B is equivalent to B AND A, ditto for A OR B and B OR A.
	return arg1->sameAs(tdbb, csb, otherNode->arg2) &&
		arg2->sameAs(tdbb, csb, otherNode->arg1);
}

BoolExprNode* BinaryBoolNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	BinaryBoolNode* node = FB_NEW(*tdbb->getDefaultPool()) BinaryBoolNode(*tdbb->getDefaultPool(),
		blrOp);
	node->nodFlags = nodFlags;
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);
	return node;
}

bool BinaryBoolNode::execute(thread_db* tdbb, jrd_req* request) const
{
	switch (blrOp)
	{
		case blr_and:
			return executeAnd(tdbb, request);

		case blr_or:
			return executeOr(tdbb, request);
	}

	fb_assert(false);
	return false;
}

bool BinaryBoolNode::executeAnd(thread_db* tdbb, jrd_req* request) const
{
	// If either operand is false, then the result is false;
	// If both are true, the result is true;
	// Otherwise, the result is NULL.
	//
	// op 1            op 2            result
	// ----            ----            ------
	// F               F                F
	// F               T                F
	// F               N                F
	// T               F                F
	// T               T                T
	// T               N                N
	// N               F                F
	// N               T                N
	// N               N                N

	const bool value1 = arg1->execute(tdbb, request);

	// Save null state and get other operand.
	const USHORT firstnull = request->req_flags & req_null;
	request->req_flags &= ~req_null;

	if (!value1 && !firstnull)
	{
		// First term is false, why the whole expression is false.
		// NULL flag is already turned off a few lines above.
		return false;
	}

	const bool value2 = arg2->execute(tdbb, request);
	const USHORT secondnull = request->req_flags & req_null;
	request->req_flags &= ~req_null;

	if (!value2 && !secondnull)
		return false;	// at least one operand was false

	if (value1 && value2)
		return true;	// both true

	// otherwise, return null
	request->req_flags |= req_null;
	return false;
}

bool BinaryBoolNode::executeOr(thread_db* tdbb, jrd_req* request) const
{
	// If either operand is true, then the result is true;
	// If both are false, the result is false;
	// Otherwise, the result is NULL.
	//
	// op 1            op 2            result
	// ----            ----            ------
	// F               F                F
	// F               T                T
	// F               N                N
	// T               F                T
	// T               T                T
	// T               N                T
	// N               F                N
	// N               T                T
	// N               N                N

	const bool value1 = arg1->execute(tdbb, request);

	const ULONG flags = request->req_flags;
	request->req_flags &= ~req_null;

	if (value1)
	{
		// First term is true, why the whole expression is true.
		// NULL flag is already turned off a few lines above.
		return true;
	}

	const bool value2 = arg2->execute(tdbb, request);

	if (value1 || value2)
	{
		request->req_flags &= ~req_null;
		return true;
	}

	// restore saved NULL state

	if (flags & req_null)
		request->req_flags |= req_null;

	return false;
}


//--------------------


static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeEql(blr_eql);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeGeq(blr_geq);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeGtr(blr_gtr);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeLeq(blr_leq);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeLss(blr_lss);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeNeq(blr_neq);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeEquiv(blr_equiv);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeBetween(blr_between);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeLike(blr_like);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeAnsiLike(blr_ansi_like);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeContaining(blr_containing);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeStarting(blr_starting);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeSimilar(blr_similar);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeMatching(blr_matching);
static RegisterBoolNode<ComparativeBoolNode> regComparativeBoolNodeMatching2(blr_matching2);	// sleuth

ComparativeBoolNode::ComparativeBoolNode(MemoryPool& pool, UCHAR aBlrOp,
			dsql_nod* aArg1, dsql_nod* aArg2, dsql_nod* aArg3)
	: TypedNode<BoolExprNode, ExprNode::TYPE_COMPARATIVE_BOOL>(pool),
	  blrOp(aBlrOp),
	  dsqlArg1(aArg1),
	  dsqlArg2(aArg2),
	  dsqlArg3(aArg3),
	  dsqlFlag(DFLAG_NONE),
	  dsqlWasValue(false),
	  arg1(NULL),
	  arg2(NULL),
	  arg3(NULL)
{
	addChildNode(dsqlArg1, arg1);
	addChildNode(dsqlArg2, arg2);
	addChildNode(dsqlArg3, arg3);
}

DmlNode* ComparativeBoolNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	ComparativeBoolNode* node = FB_NEW(pool) ComparativeBoolNode(pool, blrOp);

	node->arg1 = PAR_parse_value(tdbb, csb);
	node->arg2 = PAR_parse_value(tdbb, csb);

	if (blrOp == blr_between || blrOp == blr_ansi_like || blrOp == blr_matching2)
	{
		if (blrOp == blr_ansi_like)
			node->blrOp = blr_like;

		node->arg3 = PAR_parse_value(tdbb, csb);
	}
	else if (blrOp == blr_similar)
	{
		if (csb->csb_blr_reader.getByte() != 0)
			node->arg3 = PAR_parse_value(tdbb, csb);	// escape
	}

	return node;
}

void ComparativeBoolNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("ComparativeBoolNode (%d)", blrOp);
	BoolExprNode::print(text, nodes);
}

BoolExprNode* ComparativeBoolNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	dsql_nod* procArg1 = dsqlArg1;
	dsql_nod* procArg2 = dsqlArg2;
	dsql_nod* procArg3 = dsqlArg3;

	// Make INSERTING/UPDATING/DELETING in booleans to read the trigger action.

	if (dsqlWasValue && procArg1->nod_type == Dsql::nod_field_name)
	{
		const char* fieldName = ((dsql_str*) procArg1->nod_arg[Dsql::e_fln_name])->str_data;

		static const char* const NAMES[] = {
			"INSERTING",
			"UPDATING",
			"DELETING"
		};

		for (size_t i = 0; i < FB_NELEM(NAMES); ++i)
		{
			if (strcmp(fieldName, NAMES[i]) == 0)
			{
				thread_db* tdbb = JRD_get_thread_data();

				InternalInfoNode* infoNode = FB_NEW(*tdbb->getDefaultPool()) InternalInfoNode(
					*tdbb->getDefaultPool(),
					MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_TRIGGER_ACTION)));

				procArg1 = MAKE_node(Dsql::nod_class_exprnode, 1);
				procArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(infoNode);

				procArg2 = MAKE_const_slong(i + 1);
			}
		}
	}

	switch (blrOp)
	{
		case blr_eql:
		case blr_neq:
		case blr_gtr:
		case blr_geq:
		case blr_lss:
		case blr_leq:
			if (procArg2->nod_type == Dsql::nod_list)
			{
				int listItemCount = 0;
				BoolExprNode* resultNode = NULL;
				dsql_nod** ptr = procArg2->nod_arg;

				for (const dsql_nod* const* const end = ptr + procArg2->nod_count;
					 ptr != end; ++listItemCount, ++ptr)
				{
					if (listItemCount >= MAX_MEMBER_LIST)
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
								  Arg::Gds(isc_imp_exc) <<
								  Arg::Gds(isc_dsql_too_many_values) << Arg::Num(MAX_MEMBER_LIST));
					}

					DEV_BLKCHK(*ptr, dsql_type_nod);

					ComparativeBoolNode* temp = FB_NEW(getPool()) ComparativeBoolNode(getPool(),
						blrOp, procArg1, *ptr);

					if (resultNode)
					{
						dsql_nod* tempNod = MAKE_node(Dsql::nod_class_exprnode, 1);
						tempNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(temp);

						dsql_nod* resultNod = MAKE_node(Dsql::nod_class_exprnode, 1);
						resultNod->nod_arg[0] = reinterpret_cast<dsql_nod*>(resultNode);

						BinaryBoolNode* binaryNode = FB_NEW(getPool()) BinaryBoolNode(getPool(),
							blr_or, resultNod, tempNod);

						resultNode = binaryNode;
					}
					else
						resultNode = temp;
				}

				return resultNode->dsqlPass(dsqlScratch);
			}

			if (procArg2->nod_type == Dsql::nod_select_expr &&
				!(procArg2->nod_flags & NOD_SELECT_EXPR_SINGLETON))
			{
				UCHAR newBlrOp = blr_any;

				if (dsqlFlag == DFLAG_ANSI_ANY)
					newBlrOp = blr_ansi_any;
				else if (dsqlFlag == DFLAG_ANSI_ALL)
					newBlrOp = blr_ansi_all;

				return createRseNode(dsqlScratch, newBlrOp);
			}

			break;
	}

	ComparativeBoolNode* node = FB_NEW(getPool()) ComparativeBoolNode(getPool(), blrOp,
		PASS1_node(dsqlScratch, procArg1), PASS1_node(dsqlScratch, procArg2),
		PASS1_node(dsqlScratch, procArg3));

	switch (blrOp)
	{
		case blr_eql:
		case blr_neq:
		case blr_gtr:
		case blr_geq:
		case blr_lss:
		case blr_leq:
		case blr_equiv:
		case blr_between:
		{
			// Try to force arg1 to be same type as arg2 eg: ? = FIELD case
			PASS1_set_parameter_type(dsqlScratch, node->dsqlArg1, node->dsqlArg2, false);

			// Try to force arg2 to be same type as arg1 eg: FIELD = ? case
			// Try even when the above call succeeded, because "arg2" may
			// have arg-expressions that should be resolved.
			PASS1_set_parameter_type(dsqlScratch, node->dsqlArg2, node->dsqlArg1, false);

			// X BETWEEN Y AND ? case
			if (!PASS1_set_parameter_type(dsqlScratch, node->dsqlArg3, node->dsqlArg1, false))
			{
				// ? BETWEEN Y AND ? case
				PASS1_set_parameter_type(dsqlScratch, node->dsqlArg3, node->dsqlArg2, false);
			}

			dsc desc1, desc2;

			MAKE_desc(dsqlScratch, &desc1, node->dsqlArg1);
			MAKE_desc(dsqlScratch, &desc2, node->dsqlArg2);

			if ((desc1.dsc_dtype == dtype_boolean || desc2.dsc_dtype == dtype_boolean) &&
				!(desc1.isNull() || desc2.isNull()) && desc1.dsc_dtype != desc2.dsc_dtype)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					Arg::Gds(isc_invalid_boolean_usage));
			}

			break;
		}

		case blr_containing:
		case blr_like:
		case blr_similar:
		case blr_starting:
			// Try to force arg1 to be same type as arg2 eg: ? LIKE FIELD case
			PASS1_set_parameter_type(dsqlScratch, node->dsqlArg1, node->dsqlArg2, true);

			// Try to force arg2 same type as arg 1 eg: FIELD LIKE ? case
			// Try even when the above call succeeded, because "arg2" may
			// have arg-expressions that should be resolved.
			PASS1_set_parameter_type(dsqlScratch, node->dsqlArg2, node->dsqlArg1, true);

			// X LIKE Y ESCAPE ? case
			PASS1_set_parameter_type(dsqlScratch, node->dsqlArg3, node->dsqlArg2, true);
	}

	return node;
}

void ComparativeBoolNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp == blr_like && dsqlArg3 ? blr_ansi_like : blrOp);

	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);

	if (blrOp == blr_similar)
		dsqlScratch->appendUChar(dsqlArg3 ? 1 : 0);

	if (dsqlArg3)
		GEN_expr(dsqlScratch, dsqlArg3);
}

bool ComparativeBoolNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!BoolExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const ComparativeBoolNode* o = other->as<ComparativeBoolNode>();
	fb_assert(o)

	return dsqlFlag == o->dsqlFlag && blrOp == o->blrOp;
}


bool ComparativeBoolNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	ComparativeBoolNode* otherNode = other->as<ComparativeBoolNode>();

	if (!otherNode || blrOp != otherNode->blrOp)
		return false;

	bool matching = arg1->sameAs(tdbb, csb, otherNode->arg1) &&
		arg2->sameAs(tdbb, csb, otherNode->arg2);

	if (matching)
	{
		matching = !arg3 == !otherNode->arg3 && (!arg3 || arg3->sameAs(tdbb, csb, otherNode->arg3));

		if (matching)
			return true;
	}

	// TODO match A > B to B <= A, etc

	if (blrOp == blr_eql || blrOp == blr_equiv || blrOp == blr_neq)
	{
		// A = B is equivalent to B = A, etc.
		if (arg1->sameAs(tdbb, csb, otherNode->arg2) && arg2->sameAs(tdbb, csb, otherNode->arg1))
			return true;
	}

	return false;
}


BoolExprNode* ComparativeBoolNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ComparativeBoolNode* node = FB_NEW(*tdbb->getDefaultPool()) ComparativeBoolNode(
		*tdbb->getDefaultPool(), blrOp);
	node->nodFlags = nodFlags;
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);

	if (arg3)
		node->arg3 = copier.copy(tdbb, arg3);

	return node;
}

BoolExprNode* ComparativeBoolNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	bool invariantCheck = false;

	switch (blrOp)
	{
		case blr_like:
		case blr_similar:
		case blr_containing:
		case blr_starting:
			invariantCheck = true;
			break;
	}

	doPass1(tdbb, csb, arg1.getAddress());

	if (invariantCheck)
	{
		// We need to take care of invariantness expressions to be able to pre-compile the pattern.
		nodFlags |= FLAG_INVARIANT;
		csb->csb_current_nodes.push(this);
	}

	doPass1(tdbb, csb, arg2.getAddress());
	doPass1(tdbb, csb, arg3.getAddress());

	if (invariantCheck)
	{
		csb->csb_current_nodes.pop();

		// If there is no top-level RSE present and patterns are not constant, unmark node as invariant
		// because it may be dependent on data or variables.
		if ((nodFlags & FLAG_INVARIANT) &&
			(!arg2->is<LiteralNode>() || (arg3 && !arg3->is<LiteralNode>())))
		{
			const RseOrExprNode* ctx_node, *end;

			for (ctx_node = csb->csb_current_nodes.begin(), end = csb->csb_current_nodes.end();
				 ctx_node != end; ++ctx_node)
			{
				if (ctx_node->rseNode)
					break;
			}

			if (ctx_node >= end)
				nodFlags &= ~FLAG_INVARIANT;
		}
	}

	return this;
}

void ComparativeBoolNode::pass2Boolean1(thread_db* /*tdbb*/, CompilerScratch* csb)
{
	if (nodFlags & FLAG_INVARIANT)
		csb->csb_invariants.push(&impureOffset);
}

void ComparativeBoolNode::pass2Boolean2(thread_db* tdbb, CompilerScratch* csb)
{
	RecordKeyNode* keyNode;

	if (arg3)
	{
		if ((keyNode = arg3->as<RecordKeyNode>()) && keyNode->aggregate)
			ERR_post(Arg::Gds(isc_bad_dbkey));

		dsc descriptor_c;
		arg1->getDesc(tdbb, csb, &descriptor_c);

		if (DTYPE_IS_DATE(descriptor_c.dsc_dtype))
		{
			arg1->nodFlags |= FLAG_DATE;
			arg2->nodFlags |= FLAG_DATE;
		}
	}

	if (((keyNode = arg1->as<RecordKeyNode>()) && keyNode->aggregate) ||
		((keyNode = arg2->as<RecordKeyNode>()) && keyNode->aggregate))
	{
		ERR_post(Arg::Gds(isc_bad_dbkey));
	}

	dsc descriptor_a, descriptor_b;
	arg1->getDesc(tdbb, csb, &descriptor_a);
	arg2->getDesc(tdbb, csb, &descriptor_b);

	if (DTYPE_IS_DATE(descriptor_a.dsc_dtype))
		arg2->nodFlags |= FLAG_DATE;
	else if (DTYPE_IS_DATE(descriptor_b.dsc_dtype))
		arg1->nodFlags |= FLAG_DATE;

	if (nodFlags & FLAG_INVARIANT)
	{
		// This may currently happen for nod_like, nod_contains and nod_similar
		impureOffset = CMP_impure(csb, sizeof(impure_value));
	}
}

bool ComparativeBoolNode::execute(thread_db* tdbb, jrd_req* request) const
{
	dsc* desc[2] = {NULL, NULL};
	SSHORT comparison;
	bool computed_invariant = false;

	request->req_flags &= ~req_same_tx_upd;
	SSHORT force_equal = 0;

	// Evaluate arguments.  If either is null, result is null, but in
	// any case, evaluate both, since some expressions may later depend
	// on mappings which are developed here

	desc[0] = EVL_expr(tdbb, request, arg1);

	const ULONG flags = request->req_flags;
	request->req_flags &= ~req_null;
	force_equal |= request->req_flags & req_same_tx_upd;

	// Currently only nod_like, nod_contains, nod_starts and nod_similar may be marked invariant
	if (nodFlags & FLAG_INVARIANT)
	{
		impure_value* impure = request->getImpure<impure_value>(impureOffset);

		// Check that data type of operand is still the same.
		// It may change due to multiple formats present in stream
		// System tables are the good example of such streams -
		// data coming from ini.epp has ASCII ttype, user data is UNICODE_FSS
		//
		// Note that value descriptor may be NULL pointer if value is SQL NULL
		if ((impure->vlu_flags & VLU_computed) && desc[0] &&
			(impure->vlu_desc.dsc_dtype != desc[0]->dsc_dtype ||
			 impure->vlu_desc.dsc_sub_type != desc[0]->dsc_sub_type ||
			 impure->vlu_desc.dsc_scale != desc[0]->dsc_scale))
		{
			impure->vlu_flags &= ~VLU_computed;
		}

		if (impure->vlu_flags & VLU_computed)
		{
			if (impure->vlu_flags & VLU_null)
				request->req_flags |= req_null;
			else
				computed_invariant = true;
		}
		else
		{
			desc[1] = EVL_expr(tdbb, request, arg2);

			if (request->req_flags & req_null)
			{
				impure->vlu_flags |= VLU_computed;
				impure->vlu_flags |= VLU_null;
			}
			else
			{
				impure->vlu_flags &= ~VLU_null;

				// Search object depends on operand data type.
				// Thus save data type which we use to compute invariant
				if (desc[0])
				{
					impure->vlu_desc.dsc_dtype = desc[0]->dsc_dtype;
					impure->vlu_desc.dsc_sub_type = desc[0]->dsc_sub_type;
					impure->vlu_desc.dsc_scale = desc[0]->dsc_scale;
				}
				else
				{
					// Indicate we do not know type of expression.
					// This code will force pattern recompile for the next non-null value
					impure->vlu_desc.dsc_dtype = 0;
					impure->vlu_desc.dsc_sub_type = 0;
					impure->vlu_desc.dsc_scale = 0;
				}
			}
		}
	}
	else
		desc[1] = EVL_expr(tdbb, request, arg2);

	// An equivalence operator evaluates to true when both operands
	// are NULL and behaves like an equality operator otherwise.
	// Note that this operator never sets req_null flag

	if (blrOp == blr_equiv)
	{
		if ((flags & req_null) && (request->req_flags & req_null))
		{
			request->req_flags &= ~req_null;
			return true;
		}

		if ((flags & req_null) || (request->req_flags & req_null))
		{
			request->req_flags &= ~req_null;
			return false;
		}
	}

	// If either of expressions above returned NULL set req_null flag
	// and return false

	if (flags & req_null)
		request->req_flags |= req_null;

	if (request->req_flags & req_null)
		return false;

	force_equal |= request->req_flags & req_same_tx_upd;

	switch (blrOp)
	{
		case blr_eql:
		case blr_equiv:
		case blr_gtr:
		case blr_geq:
		case blr_lss:
		case blr_leq:
		case blr_neq:
		case blr_between:
			comparison = MOV_compare(desc[0], desc[1]);
	}

	// If we are checking equality of record_version
	// and same transaction updated the record, force equality.

	const RecordKeyNode* recVersionNode = arg1->as<RecordKeyNode>();

	if (recVersionNode && recVersionNode->blrOp == blr_record_version && force_equal)
		comparison = 0;

	request->req_flags &= ~(req_null | req_same_tx_upd);

	switch (blrOp)
	{
		case blr_eql:
		case blr_equiv:
			return comparison == 0;

		case blr_gtr:
			return comparison > 0;

		case blr_geq:
			return comparison >= 0;

		case blr_lss:
			return comparison < 0;

		case blr_leq:
			return comparison <= 0;

		case blr_neq:
			return comparison != 0;

		case blr_between:
			desc[1] = EVL_expr(tdbb, request, arg3);
			if (request->req_flags & req_null)
				return false;
			return comparison >= 0 && MOV_compare(desc[0], desc[1]) <= 0;

		case blr_containing:
		case blr_starting:
		case blr_matching:
		case blr_like:
		case blr_similar:
			return stringBoolean(tdbb, request, desc[0], desc[1], computed_invariant);

		case blr_matching2:
			return sleuth(tdbb, request, desc[0], desc[1]);
	}

	return false;
}

// Perform one of the complex string functions CONTAINING, MATCHES, or STARTS WITH.
bool ComparativeBoolNode::stringBoolean(thread_db* tdbb, jrd_req* request, dsc* desc1,
	dsc* desc2, bool computed_invariant) const
{
	UCHAR* p1 = NULL;
	UCHAR* p2 = NULL;
	SLONG l2 = 0;
	USHORT type1;
	MoveBuffer match_str;

	SET_TDBB(tdbb);

	if (!desc1->isBlob())
	{
		// Source is not a blob, do a simple search

		// Get text type of data string

		type1 = INTL_TEXT_TYPE(*desc1);

		// Get address and length of search string - convert to datatype of data

		if (!computed_invariant)
			l2 = MOV_make_string2(tdbb, desc2, type1, &p2, match_str, false);

		VaryStr<256> temp1;
		USHORT xtype1;
		const USHORT l1 = MOV_get_string_ptr(desc1, &xtype1, &p1, &temp1, sizeof(temp1));

		fb_assert(xtype1 == type1);

		return stringFunction(tdbb, request, l1, p1, l2, p2, type1, computed_invariant);
	}

	// Source string is a blob, things get interesting

	HalfStaticArray<UCHAR, BUFFER_SMALL> buffer;

	if (desc1->dsc_sub_type == isc_blob_text)
		type1 = desc1->dsc_blob_ttype();	// pick up character set and collation of blob
	else
		type1 = ttype_none;	// Do byte matching

	Collation* obj = INTL_texttype_lookup(tdbb, type1);
	CharSet* charset = obj->getCharSet();

	// Get address and length of search string - make it string if necessary
	// but don't transliterate character set if the source blob is binary
	if (!computed_invariant)
	{
		l2 = MOV_make_string2(tdbb, desc2, type1, &p2, match_str, false);
	}

	blb* blob =	BLB_open(tdbb, request->req_transaction, reinterpret_cast<bid*>(desc1->dsc_address));

	if (charset->isMultiByte() &&
		(blrOp != blr_starting || !(obj->getFlags() & TEXTTYPE_DIRECT_MATCH)))
	{
		buffer.getBuffer(blob->blb_length);		// alloc space to put entire blob in memory
	}

	// Performs the string_function on each segment of the blob until
	// a positive result is obtained

	bool ret_val = false;

	switch (blrOp)
	{
		case blr_like:
		case blr_similar:
		{
			VaryStr<TEMP_LENGTH> temp3;
			const UCHAR* escape_str = NULL;
			USHORT escape_length = 0;

			// ensure 3rd argument (escape char) is in operation text type
			if (arg3 && !computed_invariant)
			{
				// Convert ESCAPE to operation character set
				dsc* desc = EVL_expr(tdbb, request, arg3);

				if (request->req_flags & req_null)
				{
					if (nodFlags & FLAG_INVARIANT)
					{
						impure_value* impure = request->getImpure<impure_value>(impureOffset);
						impure->vlu_flags |= VLU_computed;
						impure->vlu_flags |= VLU_null;
					}
					ret_val = false;
					break;
				}

				escape_length = MOV_make_string(desc, type1,
					reinterpret_cast<const char**>(&escape_str), &temp3, sizeof(temp3));

				if (!escape_length || charset->length(escape_length, escape_str, true) != 1)
				{
					// If characters left, or null byte character, return error
					BLB_close(tdbb, blob);
					ERR_post(Arg::Gds(isc_escape_invalid));
				}

				USHORT escape[2] = {0, 0};

				charset->getConvToUnicode().convert(escape_length, escape_str, sizeof(escape), escape);
				if (!escape[0])
				{
					// If or null byte character, return error
					BLB_close(tdbb, blob);
					ERR_post(Arg::Gds(isc_escape_invalid));
				}
			}

			PatternMatcher* evaluator;

			if (nodFlags & FLAG_INVARIANT)
			{
				impure_value* impure = request->getImpure<impure_value>(impureOffset);

				if (!(impure->vlu_flags & VLU_computed))
				{
					delete impure->vlu_misc.vlu_invariant;
					impure->vlu_flags |= VLU_computed;

					if (blrOp == blr_like)
					{
						impure->vlu_misc.vlu_invariant = evaluator = obj->createLikeMatcher(
							*tdbb->getDefaultPool(), p2, l2, escape_str, escape_length);
					}
					else	// nod_similar
					{
						impure->vlu_misc.vlu_invariant = evaluator = obj->createSimilarToMatcher(
							*tdbb->getDefaultPool(), p2, l2, escape_str, escape_length);
					}
				}
				else
				{
					evaluator = impure->vlu_misc.vlu_invariant;
					evaluator->reset();
				}
			}
			else if (blrOp == blr_like)
			{
				evaluator = obj->createLikeMatcher(*tdbb->getDefaultPool(),
					p2, l2, escape_str, escape_length);
			}
			else	// nod_similar
			{
				evaluator = obj->createSimilarToMatcher(*tdbb->getDefaultPool(),
					p2, l2, escape_str, escape_length);
			}

			while (!(blob->blb_flags & BLB_eof))
			{
				const SLONG l1 = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCapacity(), false);
				if (!evaluator->process(buffer.begin(), l1))
					break;
			}

			ret_val = evaluator->result();

			if (!(nodFlags & FLAG_INVARIANT))
				delete evaluator;

			break;
		}

		case blr_containing:
		case blr_starting:
		{
			PatternMatcher* evaluator;

			if (nodFlags & FLAG_INVARIANT)
			{
				impure_value* impure = request->getImpure<impure_value>(impureOffset);
				if (!(impure->vlu_flags & VLU_computed))
				{
					delete impure->vlu_misc.vlu_invariant;

					if (blrOp == blr_containing)
					{
						impure->vlu_misc.vlu_invariant = evaluator =
							obj->createContainsMatcher(*tdbb->getDefaultPool(), p2, l2);
					}
					else	// nod_starts
					{
						impure->vlu_misc.vlu_invariant = evaluator =
							obj->createStartsMatcher(*tdbb->getDefaultPool(), p2, l2);
					}

					impure->vlu_flags |= VLU_computed;
				}
				else
				{
					evaluator = impure->vlu_misc.vlu_invariant;
					evaluator->reset();
				}
			}
			else
			{
				if (blrOp == blr_containing)
					evaluator = obj->createContainsMatcher(*tdbb->getDefaultPool(), p2, l2);
				else	// nod_starts
					evaluator = obj->createStartsMatcher(*tdbb->getDefaultPool(), p2, l2);
			}

			while (!(blob->blb_flags & BLB_eof))
			{
				const SLONG l1 = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCapacity(), false);
				if (!evaluator->process(buffer.begin(), l1))
					break;
			}

			ret_val = evaluator->result();

			if (!(nodFlags & FLAG_INVARIANT))
				delete evaluator;

			break;
		}
	}

	BLB_close(tdbb, blob);

	return ret_val;
}

// Perform one of the pattern matching string functions.
bool ComparativeBoolNode::stringFunction(thread_db* tdbb, jrd_req* request,
	SLONG l1, const UCHAR* p1, SLONG l2, const UCHAR* p2, USHORT ttype,
	bool computed_invariant) const
{
	SET_TDBB(tdbb);

	Collation* obj = INTL_texttype_lookup(tdbb, ttype);
	CharSet* charset = obj->getCharSet();

	// Handle contains and starts
	if (blrOp == blr_containing || blrOp == blr_starting)
	{
		if (nodFlags & FLAG_INVARIANT)
		{
			impure_value* impure = request->getImpure<impure_value>(impureOffset);
			PatternMatcher* evaluator;
			if (!(impure->vlu_flags & VLU_computed))
			{
				delete impure->vlu_misc.vlu_invariant;

				if (blrOp == blr_containing)
				{
					impure->vlu_misc.vlu_invariant = evaluator =
						obj->createContainsMatcher(*tdbb->getDefaultPool(), p2, l2);
				}
				else
				{
					// nod_starts
					impure->vlu_misc.vlu_invariant = evaluator =
						obj->createStartsMatcher(*tdbb->getDefaultPool(), p2, l2);
				}

				impure->vlu_flags |= VLU_computed;
			}
			else
			{
				evaluator = impure->vlu_misc.vlu_invariant;
				evaluator->reset();
			}

			evaluator->process(p1, l1);
			return evaluator->result();
		}

		if (blrOp == blr_containing)
			return obj->contains(*tdbb->getDefaultPool(), p1, l1, p2, l2);

		// nod_starts
		return obj->starts(*tdbb->getDefaultPool(), p1, l1, p2, l2);
	}

	// Handle LIKE and SIMILAR
	if (blrOp == blr_like || blrOp == blr_similar)
	{
		VaryStr<TEMP_LENGTH> temp3;
		const UCHAR* escape_str = NULL;
		USHORT escape_length = 0;
		// ensure 3rd argument (escape char) is in operation text type
		if (arg3 && !computed_invariant)
		{
			// Convert ESCAPE to operation character set
			dsc* desc = EVL_expr(tdbb, request, arg3);
			if (request->req_flags & req_null)
			{
				if (nodFlags & FLAG_INVARIANT)
				{
					impure_value* impure = request->getImpure<impure_value>(impureOffset);
					impure->vlu_flags |= VLU_computed;
					impure->vlu_flags |= VLU_null;
				}
				return false;
			}

			escape_length = MOV_make_string(desc, ttype,
				reinterpret_cast<const char**>(&escape_str), &temp3, sizeof(temp3));

			if (!escape_length || charset->length(escape_length, escape_str, true) != 1)
			{
				// If characters left, or null byte character, return error
				ERR_post(Arg::Gds(isc_escape_invalid));
			}

			USHORT escape[2] = {0, 0};

			charset->getConvToUnicode().convert(escape_length, escape_str, sizeof(escape), escape);

			if (!escape[0])
			{
				// If or null byte character, return error
				ERR_post(Arg::Gds(isc_escape_invalid));
			}
		}

		if (nodFlags & FLAG_INVARIANT)
		{
			impure_value* impure = request->getImpure<impure_value>(impureOffset);
			PatternMatcher* evaluator;

			if (!(impure->vlu_flags & VLU_computed))
			{
				delete impure->vlu_misc.vlu_invariant;
				impure->vlu_flags |= VLU_computed;

				if (blrOp == blr_like)
				{
					impure->vlu_misc.vlu_invariant = evaluator = obj->createLikeMatcher(
						*tdbb->getDefaultPool(), p2, l2, escape_str, escape_length);
				}
				else	// nod_similar
				{
					impure->vlu_misc.vlu_invariant = evaluator = obj->createSimilarToMatcher(
						*tdbb->getDefaultPool(), p2, l2, escape_str, escape_length);
				}
			}
			else
			{
				evaluator = impure->vlu_misc.vlu_invariant;
				evaluator->reset();
			}

			evaluator->process(p1, l1);

			return evaluator->result();
		}

		if (blrOp == blr_like)
			return obj->like(*tdbb->getDefaultPool(), p1, l1, p2, l2, escape_str, escape_length);

		// nod_similar
		return obj->similarTo(*tdbb->getDefaultPool(), p1, l1, p2, l2, escape_str, escape_length);
	}

	// Handle MATCHES
	return obj->matches(*tdbb->getDefaultPool(), p1, l1, p2, l2);
}

// Execute SLEUTH operator.
bool ComparativeBoolNode::sleuth(thread_db* tdbb, jrd_req* request, const dsc* desc1,
	const dsc* desc2) const
{
	SET_TDBB(tdbb);

	// Choose interpretation for the operation

 	USHORT ttype;
	if (desc1->isBlob())
	{
		if (desc1->dsc_sub_type == isc_blob_text)
			ttype = desc1->dsc_blob_ttype();	// Load blob character set and collation
		else
			ttype = INTL_TTYPE(desc2);
	}
	else
		ttype = INTL_TTYPE(desc1);

	Collation* obj = INTL_texttype_lookup(tdbb, ttype);

	// Get operator definition string (control string)

	dsc* desc3 = EVL_expr(tdbb, request, arg3);

	UCHAR* p1;
	MoveBuffer sleuth_str;
	USHORT l1 = MOV_make_string2(tdbb, desc3, ttype, &p1, sleuth_str);
	// Get address and length of search string
	UCHAR* p2;
	MoveBuffer match_str;
	USHORT l2 = MOV_make_string2(tdbb, desc2, ttype, &p2, match_str);

	// Merge search and control strings
	UCHAR control[BUFFER_SMALL];
	SLONG control_length = obj->sleuthMerge(*tdbb->getDefaultPool(), p2, l2, p1, l1, control); //, BUFFER_SMALL);

	// Note: resulting string from sleuthMerge is either USHORT or UCHAR
	// and never Multibyte (see note in EVL_mb_sleuthCheck)
	bool ret_val;
	MoveBuffer data_str;
	if (!desc1->isBlob())
	{
		// Source is not a blob, do a simple search

		l1 = MOV_make_string2(tdbb, desc1, ttype, &p1, data_str);
		ret_val = obj->sleuthCheck(*tdbb->getDefaultPool(), 0, p1, l1, control, control_length);
	}
	else
	{
		// Source string is a blob, things get interesting

		blb* blob = BLB_open(tdbb, request->req_transaction,
			reinterpret_cast<bid*>(desc1->dsc_address));

		UCHAR buffer[BUFFER_LARGE];
		ret_val = false;

		while (!(blob->blb_flags & BLB_eof))
		{
			l1 = BLB_get_segment(tdbb, blob, buffer, sizeof(buffer));
			if (obj->sleuthCheck(*tdbb->getDefaultPool(), 0, buffer, l1, control, control_length))
			{
				ret_val = true;
				break;
			}
		}

		BLB_close(tdbb, blob);
	}

	return ret_val;
}

BoolExprNode* ComparativeBoolNode::createRseNode(DsqlCompilerScratch* dsqlScratch, UCHAR rseBlrOp)
{
	PASS1_set_parameter_type(dsqlScratch, dsqlArg1, dsqlArg2, false);

	// Create a derived table representing our subquery.
	dsql_nod* dt = MAKE_node(Dsql::nod_select_expr, Dsql::e_sel_count);
	// Ignore validation for colum names that must exist for "user" derived tables.
	dt->nod_flags |= NOD_SELECT_EXPR_DT_IGNORE_COLUMN_CHECK | NOD_SELECT_EXPR_DERIVED;
	dt->nod_arg[Dsql::e_sel_query_spec] = dsqlArg2;
	dsql_nod* from = MAKE_node(Dsql::nod_list, 1);
	from->nod_arg[0] = dt;

	RseNode* querySpec = FB_NEW(getPool()) RseNode(getPool());
	querySpec->dsqlFrom = from;

	dsql_nod* select_expr = MAKE_node(Dsql::nod_select_expr, Dsql::e_sel_count);
	select_expr->nod_arg[Dsql::e_sel_query_spec] = MAKE_node(Dsql::nod_class_exprnode, 1);
	select_expr->nod_arg[Dsql::e_sel_query_spec]->nod_arg[0] = reinterpret_cast<dsql_nod*>(querySpec);

	const DsqlContextStack::iterator base(*dsqlScratch->context);
	const DsqlContextStack::iterator baseDT(dsqlScratch->derivedContext);
	const DsqlContextStack::iterator baseUnion(dsqlScratch->unionContext);

	dsql_nod* rseNod = PASS1_rse(dsqlScratch, select_expr, NULL);
	RseNode* rse = ExprNode::as<RseNode>(rseNod);
	fb_assert(rse);

	// Create a conjunct to be injected.

	ComparativeBoolNode* cmpNode = FB_NEW(getPool()) ComparativeBoolNode(getPool(), blrOp,
		PASS1_node_psql(dsqlScratch, dsqlArg1, false), rse->dsqlSelectList->nod_arg[0]);

	dsql_nod* temp = MAKE_node(Dsql::nod_class_exprnode, 1);
	temp->nod_arg[0] = reinterpret_cast<dsql_nod*>(cmpNode);
	rse->dsqlWhere = temp;

	// Create output node.
	RseBoolNode* rseBoolNode = FB_NEW(getPool()) RseBoolNode(getPool(), rseBlrOp, rseNod);

	// Finish off by cleaning up contexts
	dsqlScratch->unionContext.clear(baseUnion);
	dsqlScratch->derivedContext.clear(baseDT);
	dsqlScratch->context->clear(base);

	return rseBoolNode;
}


//--------------------


static RegisterBoolNode<MissingBoolNode> regMissingBoolNode(blr_missing);

MissingBoolNode::MissingBoolNode(MemoryPool& pool, dsql_nod* aArg, bool aDsqlUnknown)
	: TypedNode<BoolExprNode, ExprNode::TYPE_MISSING_BOOL>(pool),
	  dsqlArg(aArg),
	  dsqlUnknown(aDsqlUnknown),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* MissingBoolNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	MissingBoolNode* node = FB_NEW(pool) MissingBoolNode(pool);
	node->arg = PAR_parse_value(tdbb, csb);
	return node;
}

void MissingBoolNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "MissingBoolNode";
	BoolExprNode::print(text, nodes);
}

BoolExprNode* MissingBoolNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	MissingBoolNode* node = FB_NEW(getPool()) MissingBoolNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg));

	PASS1_set_parameter_type(dsqlScratch, node->dsqlArg, NULL, false);

	dsc desc;
	MAKE_desc(dsqlScratch, &desc, node->dsqlArg);

	if (dsqlUnknown && desc.dsc_dtype != dtype_boolean && !desc.isNull())
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
			Arg::Gds(isc_invalid_boolean_usage));
	}

	return node;
}

void MissingBoolNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_missing);
	GEN_expr(dsqlScratch, dsqlArg);
}

BoolExprNode* MissingBoolNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	MissingBoolNode* node = FB_NEW(*tdbb->getDefaultPool()) MissingBoolNode(
		*tdbb->getDefaultPool());
	node->nodFlags = nodFlags;
	node->arg = copier.copy(tdbb, arg);
	return node;
}

BoolExprNode* MissingBoolNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	return BoolExprNode::pass1(tdbb, csb);
}

void MissingBoolNode::pass2Boolean2(thread_db* tdbb, CompilerScratch* csb)
{
	RecordKeyNode* keyNode;

	if ((keyNode = arg->as<RecordKeyNode>()) && keyNode->aggregate)
		ERR_post(Arg::Gds(isc_bad_dbkey));

	// check for syntax errors in the calculation
	dsc descriptor_a;
	arg->getDesc(tdbb, csb, &descriptor_a);
}

bool MissingBoolNode::execute(thread_db* tdbb, jrd_req* request) const
{
	EVL_expr(tdbb, request, arg);

	if (request->req_flags & req_null)
	{
		request->req_flags &= ~req_null;
		return true;
	}

	return false;
}


//--------------------


static RegisterBoolNode<NotBoolNode> regNotBoolNode(blr_not);

NotBoolNode::NotBoolNode(MemoryPool& pool, dsql_nod* aArg)
	: TypedNode<BoolExprNode, ExprNode::TYPE_NOT_BOOL>(pool),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* NotBoolNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	NotBoolNode* node = FB_NEW(pool) NotBoolNode(pool);
	node->arg = PAR_parse_boolean(tdbb, csb);
	return node;
}

void NotBoolNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "NotBoolNode";
	BoolExprNode::print(text, nodes);
}

BoolExprNode* NotBoolNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return process(dsqlScratch, true);
}

void NotBoolNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_not);
	GEN_expr(dsqlScratch, dsqlArg);
}

BoolExprNode* NotBoolNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	NotBoolNode* node = FB_NEW(*tdbb->getDefaultPool()) NotBoolNode(*tdbb->getDefaultPool());
	node->nodFlags = nodFlags;
	node->arg = copier.copy(tdbb, arg);
	return node;
}

BoolExprNode* NotBoolNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	RseBoolNode* rseBoolean = arg->as<RseBoolNode>();

	if (rseBoolean)
	{
		if (rseBoolean->blrOp == blr_ansi_any)
			rseBoolean->nodFlags |= FLAG_DEOPTIMIZE | FLAG_ANSI_NOT;
		else if (rseBoolean->blrOp == blr_ansi_all)
			rseBoolean->nodFlags |= FLAG_ANSI_NOT;
	}

	return BoolExprNode::pass1(tdbb, csb);
}

bool NotBoolNode::execute(thread_db* tdbb, jrd_req* request) const
{
	bool value = arg->execute(tdbb, request);

	if (request->req_flags & req_null)
		return false;

	return !value;
}

// Replace NOT with an appropriately inverted condition, if possible.
// Get rid of redundant nested NOT predicates.
BoolExprNode* NotBoolNode::process(DsqlCompilerScratch* dsqlScratch, bool invert)
{
	fb_assert(dsqlArg->nod_type == Dsql::nod_class_exprnode);
	BoolExprNode* boolArg = reinterpret_cast<BoolExprNode*>(dsqlArg->nod_arg[0]);

	NotBoolNode* notArg = ExprNode::as<NotBoolNode>(dsqlArg);

	if (notArg)
	{
		// Recurse until different node is found (every even call means no inversion required).
		return notArg->process(dsqlScratch, !invert);
	}

	if (!invert)
		return boolArg->dsqlPass(dsqlScratch);

	ComparativeBoolNode* cmpArg = ExprNode::as<ComparativeBoolNode>(dsqlArg);
	BinaryBoolNode* binArg = ExprNode::as<BinaryBoolNode>(dsqlArg);

	// Do not handle special case: <value> NOT IN <list>

	if (cmpArg)
	{
		// Invert the given boolean.
		switch (cmpArg->blrOp)
		{
			case blr_eql:
			case blr_neq:
			case blr_lss:
			case blr_gtr:
			case blr_leq:
			case blr_geq:
			{
				UCHAR newBlrOp;

				switch (cmpArg->blrOp)
				{
					case blr_eql:
						newBlrOp = blr_neq;
						break;
					case blr_neq:
						newBlrOp = blr_eql;
						break;
					case blr_lss:
						newBlrOp = blr_geq;
						break;
					case blr_gtr:
						newBlrOp = blr_leq;
						break;
					case blr_leq:
						newBlrOp = blr_gtr;
						break;
					case blr_geq:
						newBlrOp = blr_lss;
						break;
					default:
						fb_assert(false);
						break;
				}

				ComparativeBoolNode* node = FB_NEW(getPool()) ComparativeBoolNode(
					getPool(), newBlrOp, cmpArg->dsqlArg1, cmpArg->dsqlArg2);
				node->dsqlWasValue = cmpArg->dsqlWasValue;

				if (cmpArg->dsqlFlag == ComparativeBoolNode::DFLAG_ANSI_ANY)
					node->dsqlFlag = ComparativeBoolNode::DFLAG_ANSI_ALL;
				else if (cmpArg->dsqlFlag == ComparativeBoolNode::DFLAG_ANSI_ALL)
					node->dsqlFlag = ComparativeBoolNode::DFLAG_ANSI_ANY;

				return node->dsqlPass(dsqlScratch);
			}

			case blr_between:
			{
				BinaryBoolNode* node = FB_NEW(getPool()) BinaryBoolNode(getPool(), blr_or,
					MAKE_node(Dsql::nod_class_exprnode, 1), MAKE_node(Dsql::nod_class_exprnode, 1));

				ComparativeBoolNode* cmpNode = FB_NEW(getPool()) ComparativeBoolNode(getPool(),
					blr_lss, cmpArg->dsqlArg1, cmpArg->dsqlArg2);
				node->dsqlArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(cmpNode);

				cmpNode = FB_NEW(getPool()) ComparativeBoolNode(getPool(),
					blr_gtr, cmpArg->dsqlArg1, cmpArg->dsqlArg3);
				node->dsqlArg2->nod_arg[0] = reinterpret_cast<dsql_nod*>(cmpNode);

				return node->dsqlPass(dsqlScratch);
			}
		}
	}
	else if (binArg)
	{
		switch (binArg->blrOp)
		{
			case blr_and:
			case blr_or:
			{
				UCHAR newBlrOp = binArg->blrOp == blr_and ? blr_or : blr_and;

				BinaryBoolNode* node = FB_NEW(getPool()) BinaryBoolNode(getPool(), newBlrOp,
					MAKE_node(Dsql::nod_class_exprnode, 1), MAKE_node(Dsql::nod_class_exprnode, 1));

				NotBoolNode* notNode = FB_NEW(getPool()) NotBoolNode(getPool(), binArg->dsqlArg1);
				node->dsqlArg1->nod_arg[0] = reinterpret_cast<dsql_nod*>(notNode);

				notNode = FB_NEW(getPool()) NotBoolNode(getPool(), binArg->dsqlArg2);
				node->dsqlArg2->nod_arg[0] = reinterpret_cast<dsql_nod*>(notNode);

				return node->dsqlPass(dsqlScratch);
			}
		}
	}

	// No inversion is possible, so just recreate the input node
	// and return immediately to avoid infinite recursion later.

	return FB_NEW(getPool()) NotBoolNode(getPool(), PASS1_node(dsqlScratch, dsqlArg));
}


//--------------------


static RegisterBoolNode<RseBoolNode> regRseBoolNodeAny(blr_any);
static RegisterBoolNode<RseBoolNode> regRseBoolNodeUnique(blr_unique);
static RegisterBoolNode<RseBoolNode> regRseBoolNodeAnsiAny(blr_ansi_any);
static RegisterBoolNode<RseBoolNode> regRseBoolNodeAnsiAll(blr_ansi_all);
static RegisterBoolNode<RseBoolNode> regRseBoolNodeExists(blr_exists);	// ASF: Where is this handled?

RseBoolNode::RseBoolNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aDsqlRse)
	: TypedNode<BoolExprNode, ExprNode::TYPE_RSE_BOOL>(pool),
	  blrOp(aBlrOp),
	  dsqlRse(aDsqlRse),
	  rse(NULL),
	  rsb(NULL)
{
	addChildNode(dsqlRse, rse);
}

DmlNode* RseBoolNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	RseBoolNode* node = FB_NEW(pool) RseBoolNode(pool, blrOp);
	node->rse = PAR_rse(tdbb, csb);
	return node;
}

void RseBoolNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("RseBoolNode (%d)", blrOp);
	BoolExprNode::print(text, nodes);
}

BoolExprNode* RseBoolNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	const DsqlContextStack::iterator base(*dsqlScratch->context);

	RseBoolNode* node = FB_NEW(getPool()) RseBoolNode(getPool(), blrOp,
		PASS1_rse(dsqlScratch, dsqlRse, NULL));

	// Finish off by cleaning up contexts
	dsqlScratch->context->clear(base);

	return node;
}

void RseBoolNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	GEN_rse(dsqlScratch, dsqlRse);
}

bool RseBoolNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!BoolExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const RseBoolNode* o = other->as<RseBoolNode>();
	fb_assert(o);

	return blrOp == o->blrOp;
}

bool RseBoolNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!BoolExprNode::sameAs(tdbb, csb, other))
		return false;

	RseBoolNode* otherNode = other->as<RseBoolNode>();
	fb_assert(otherNode);

	return blrOp == otherNode->blrOp;
}

BoolExprNode* RseBoolNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	RseBoolNode* node = FB_NEW(*tdbb->getDefaultPool()) RseBoolNode(
		*tdbb->getDefaultPool(), blrOp);
	node->nodFlags = nodFlags;
	node->rse = copier.copy(tdbb, rse);

	return node;
}

BoolExprNode* RseBoolNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	switch (blrOp)
	{
		case blr_ansi_all:
		{
			BoolExprNode* newNode = convertNeqAllToNotAny(tdbb, csb);
			if (newNode)
			{
				doPass1(tdbb, csb, &newNode);
				return newNode;
			}

			nodFlags |= FLAG_DEOPTIMIZE;
		}
		// fall into

		case blr_ansi_any:
			if (nodFlags & FLAG_DEOPTIMIZE)
			{
				nodFlags &= ~FLAG_DEOPTIMIZE;

				// Deoptimize the conjunct, not the ALL node itself
				BoolExprNode* boolean = rse->rse_boolean;

				if (boolean)
				{
					BinaryBoolNode* binaryNode = boolean->as<BinaryBoolNode>();
					if (binaryNode && binaryNode->blrOp == blr_and)
						boolean = binaryNode->arg2;

					// Deoptimize the injected boolean of a quantified predicate
					// when it's necessary. ALL predicate does not require an index scan.
					// This fixes bug SF #543106.
					boolean->nodFlags |= FLAG_DEOPTIMIZE;
				}
			}
			// fall into

		case blr_any:
		case blr_exists:
		case blr_unique:
			rse->ignoreDbKey(tdbb, csb);
			break;
	}

	return BoolExprNode::pass1(tdbb, csb);
}

void RseBoolNode::pass2Boolean1(thread_db* tdbb, CompilerScratch* csb)
{
	if (!(rse->flags & RseNode::FLAG_VARIANT))
	{
		nodFlags |= FLAG_INVARIANT;
		csb->csb_invariants.push(&impureOffset);
	}

	rse->pass2Rse(tdbb, csb);
}

void RseBoolNode::pass2Boolean2(thread_db* tdbb, CompilerScratch* csb)
{
	if (nodFlags & FLAG_INVARIANT)
		impureOffset = CMP_impure(csb, sizeof(impure_value));

	rsb = CMP_post_rse(tdbb, csb, rse);

	// for ansi ANY clauses (and ALL's, which are negated ANY's)
	// the unoptimized boolean expression must be used, since the
	// processing of these clauses is order dependant (see FilteredStream.cpp)

	if (blrOp == blr_ansi_any || blrOp == blr_ansi_all)
	{
		const bool ansiAny = blrOp == blr_ansi_any;
		const bool ansiNot = nodFlags & FLAG_ANSI_NOT;
		FilteredStream* const filter = static_cast<FilteredStream*>(rsb.getObject());
		filter->setAnyBoolean(rse->rse_boolean, ansiAny, ansiNot);
	}

	csb->csb_fors.add(rsb);
}

bool RseBoolNode::execute(thread_db* tdbb, jrd_req* request) const
{
	USHORT* invariant_flags;
	impure_value* impure;

	if (nodFlags & FLAG_INVARIANT)
	{
		impure = request->getImpure<impure_value>(impureOffset);
		invariant_flags = &impure->vlu_flags;

		if (*invariant_flags & VLU_computed)
		{
			// An invariant node has already been computed.

			if (blrOp == blr_ansi_any && (*invariant_flags & VLU_null))
				request->req_flags |= req_null;
			else
				request->req_flags &= ~req_null;

			return impure->vlu_misc.vlu_short != 0;
		}
	}

	rsb->open(tdbb);
	bool value = rsb->getRecord(tdbb);

	if (blrOp == blr_unique && value)
		value = !rsb->getRecord(tdbb);

	rsb->close(tdbb);

	if (blrOp == blr_any || blrOp == blr_unique)
		request->req_flags &= ~req_null;

	// If this is an invariant node, save the return value.

	if (nodFlags & FLAG_INVARIANT)
	{
		*invariant_flags |= VLU_computed;

		if ((blrOp == blr_ansi_any || blrOp == blr_ansi_all) && (request->req_flags & req_null))
			*invariant_flags |= VLU_null;

		impure->vlu_misc.vlu_short = value ? TRUE : FALSE;
	}

	return value;
}

// Try to convert nodes of expression:
//   select ... from <t1>
//     where <x> not in (select <y> from <t2>)
//   (and its variants that uses the same BLR: {NOT (a = ANY b)} and {a <> ALL b})
// to:
//   select ... from <t1>
//     where not ((x is null and exists (select 1 from <t2>)) or
//                exists (select <y> from <t2> where <y> = <x> or <y> is null))
//
// Because the second form can use indexes.
// Returns NULL when not converted, and a new node to be processed when converted.
BoolExprNode* RseBoolNode::convertNeqAllToNotAny(thread_db* tdbb, CompilerScratch* csb)
{
	SET_TDBB(tdbb);

	fb_assert(blrOp == blr_ansi_all);

	RseNode* outerRse = rse;	// blr_ansi_all rse
	ComparativeBoolNode* outerRseNeq;

	if (!outerRse || outerRse->type != RseNode::TYPE || outerRse->rse_relations.getCount() != 1 ||
		!outerRse->rse_boolean ||
		!(outerRseNeq = outerRse->rse_boolean->as<ComparativeBoolNode>()) ||
		outerRseNeq->blrOp != blr_neq)
	{
		return NULL;
	}

	RseNode* innerRse = static_cast<RseNode*>(outerRse->rse_relations[0].getObject());	// user rse

	// If the rse is different than we expected, do nothing. Do nothing also if it uses FIRST or
	// SKIP, as we can't inject booleans there without changing the behavior.
	if (!innerRse || innerRse->type != RseNode::TYPE || innerRse->rse_first || innerRse->rse_skip)
		return NULL;

	NotBoolNode* newNode = FB_NEW(csb->csb_pool) NotBoolNode(csb->csb_pool);

	BinaryBoolNode* orNode = FB_NEW(csb->csb_pool) BinaryBoolNode(csb->csb_pool, blr_or);

	newNode->arg = orNode;

	BinaryBoolNode* andNode = FB_NEW(csb->csb_pool) BinaryBoolNode(csb->csb_pool, blr_and);

	orNode->arg1 = andNode;

	MissingBoolNode* missNode = FB_NEW(csb->csb_pool) MissingBoolNode(csb->csb_pool);
	missNode->arg = outerRseNeq->arg1;

	andNode->arg1 = missNode;

	RseBoolNode* rseBoolNode = FB_NEW(csb->csb_pool) RseBoolNode(csb->csb_pool, blr_any);
	rseBoolNode->rse = innerRse;

	andNode->arg2 = rseBoolNode;

	RseNode* newInnerRse = innerRse->clone();
	newInnerRse->ignoreDbKey(tdbb, csb);

	rseBoolNode = FB_NEW(csb->csb_pool) RseBoolNode(csb->csb_pool, blr_any);
	rseBoolNode->rse = newInnerRse;

	orNode->arg2 = rseBoolNode;

	BinaryBoolNode* boolean = FB_NEW(csb->csb_pool) BinaryBoolNode(csb->csb_pool, blr_or);

	missNode = FB_NEW(csb->csb_pool) MissingBoolNode(csb->csb_pool);
	missNode->arg = outerRseNeq->arg2;

	boolean->arg1 = missNode;

	boolean->arg2 = outerRse->rse_boolean;
	outerRseNeq->blrOp = blr_eql;

	// If there was a boolean on the stream, append (AND) the new one
	if (newInnerRse->rse_boolean)
	{
		andNode = FB_NEW(csb->csb_pool) BinaryBoolNode(csb->csb_pool, blr_and);

		andNode->arg1 = newInnerRse->rse_boolean;
		andNode->arg2 = boolean;
		boolean = andNode;
	}

	newInnerRse->rse_boolean = boolean;

	SubExprNodeCopier copier(csb);

	return copier.copy(tdbb, static_cast<BoolExprNode*>(newNode));
}


}	// namespace Jrd
