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
#include "../jrd/common.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/node.h"
#include "../jrd/blr.h"
#include "../jrd/tra.h"
#include "../jrd/Function.h"
#include "../jrd/SysFunction.h"
#include "../jrd/recsrc/RecordSource.h"
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
#include "../dsql/metd_proto.h"
#include "../dsql/pass1_proto.h"
#include "../dsql/utld_proto.h"
#include "../dsql/DSqlDataTypeUtil.h"
#include "../jrd/DataTypeUtil.h"

using namespace Firebird;
using namespace Jrd;

#include "gen/blrtable.h"

namespace Jrd {


ExprNode* ExprNode::fromLegacy(const dsql_nod* node)
{
	return node->nod_type == Dsql::nod_class_exprnode ?
		reinterpret_cast<ExprNode*>(node->nod_arg[0]) : NULL;
}

ExprNode* ExprNode::fromLegacy(const jrd_nod* node)
{
	return node->nod_type == nod_class_exprnode_jrd ?
		reinterpret_cast<ExprNode*>(node->nod_arg[0]) : NULL;
}

bool ExprNode::dsqlVisit(ConstDsqlNodeVisitor& visitor)
{
	bool ret = false;

	for (dsql_nod*** i = dsqlChildNodes.begin(); i != dsqlChildNodes.end(); ++i)
		ret |= visitor.visit(*i);

	return ret;
}

bool ExprNode::dsqlVisit(NonConstDsqlNodeVisitor& visitor)
{
	bool ret = false;

	for (dsql_nod*** i = dsqlChildNodes.begin(); i != dsqlChildNodes.end(); ++i)
		ret |= visitor.visit(*i);

	return ret;
}

bool ExprNode::isArrayOrBlob(DsqlCompilerScratch* dsqlScratch) const
{
	for (const dsql_nod* const* const* i = dsqlChildNodes.begin(); i != dsqlChildNodes.end(); ++i)
	{
		if (DDL_is_array_or_blob(dsqlScratch, **i))
			return true;
	}

	return false;
}

void ExprNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ExprNode"; // needed or not?
	for (dsql_nod* const* const* i = dsqlChildNodes.begin(); i != dsqlChildNodes.end(); ++i)
		nodes.add(**i);
}

bool ExprNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (other->type != type)
		return false;

	size_t count = dsqlChildNodes.getCount();
	if (other->dsqlChildNodes.getCount() != count)
		return false;

	const dsql_nod* const* const* j = other->dsqlChildNodes.begin();
	for (const dsql_nod* const* const* i = dsqlChildNodes.begin(); i != dsqlChildNodes.end(); ++i, ++j)
	{
		if (!PASS1_node_match(**i, **j, ignoreMapCast))
			return false;
	}

	return true;
}

ExprNode* ExprNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	for (jrd_nod*** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		**i = CMP_pass1(tdbb, csb, **i);
	return this;
}

ExprNode* ExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	for (jrd_nod*** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		**i = CMP_pass2(tdbb, csb, **i, node);
	return this;
}

bool ExprNode::jrdVisit(JrdNodeVisitor& visitor)
{
	bool ret = false;

	for (jrd_nod*** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		ret |= visitor.visit(**i);

	return ret;
}


//--------------------


static RegisterNode<ConcatenateNode> regConcatenateNode(blr_concatenate);

ConcatenateNode::ConcatenateNode(MemoryPool& pool, dsql_nod* aArg1, dsql_nod* aArg2)
	: TypedNode<ExprNode, ExprNode::TYPE_CONCATENATE>(pool),
	  dsqlArg1(aArg1),
	  dsqlArg2(aArg2),
	  arg1(NULL),
	  arg2(NULL)
{
	addChildNode(dsqlArg1, arg1);
	addChildNode(dsqlArg2, arg2);
}

DmlNode* ConcatenateNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	ConcatenateNode* node = FB_NEW(pool) ConcatenateNode(pool);
	node->arg1 = PAR_parse_node(tdbb, csb, VALUE);
	node->arg2 = PAR_parse_node(tdbb, csb, VALUE);
	return node;
}

void ConcatenateNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ConcatenateNode";
	ExprNode::print(text, nodes);
}

void ConcatenateNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CONCATENATION";
}

bool ConcatenateNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar) const
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg1, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlArg2, node, forceVarChar);
}

void ConcatenateNode::genBlr()
{
	dsqlScratch->appendUChar(blr_concatenate);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

void ConcatenateNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	dsc desc1, desc2;

	MAKE_desc(dsqlScratch, &desc1, dsqlArg1, dsqlArg2);
	MAKE_desc(dsqlScratch, &desc2, dsqlArg2, dsqlArg1);

	DSqlDataTypeUtil(dsqlScratch).makeConcatenate(desc, &desc1, &desc2);
}

void ConcatenateNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	dsc desc1, desc2;

	CMP_get_desc(tdbb, csb, arg1, &desc1);
	CMP_get_desc(tdbb, csb, arg2, &desc2);

	DataTypeUtil(tdbb).makeConcatenate(desc, &desc1, &desc2);
}

ExprNode* ConcatenateNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ConcatenateNode* node = FB_NEW(*tdbb->getDefaultPool()) ConcatenateNode(*tdbb->getDefaultPool());
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);
	return node;
}

ExprNode* ConcatenateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* ConcatenateNode::execute(thread_db* tdbb, jrd_req* request) const
{
	const dsc* value1 = EVL_expr(tdbb, arg1);
	const ULONG flags = request->req_flags;
	const dsc* value2 = EVL_expr(tdbb, arg2);

	// restore saved NULL state

	if (flags & req_null)
		request->req_flags |= req_null;

	if (request->req_flags & req_null)
		return NULL;

	impure_value* impure = request->getImpure<impure_value>(node->nod_impure);
	dsc desc;

	if (value1->dsc_dtype == dtype_dbkey && value2->dsc_dtype == dtype_dbkey)
	{
		if ((ULONG) value1->dsc_length + (ULONG) value2->dsc_length > MAX_COLUMN_SIZE - sizeof(USHORT))
		{
			ERR_post(Arg::Gds(isc_concat_overflow));
			return NULL;
		}

		desc.dsc_dtype = dtype_dbkey;
		desc.dsc_length = value1->dsc_length + value2->dsc_length;
		desc.dsc_address = NULL;

		VaryingString* string = NULL;
		if (value1->dsc_address == impure->vlu_desc.dsc_address ||
			value2->dsc_address == impure->vlu_desc.dsc_address)
		{
			string = impure->vlu_string;
			impure->vlu_string = NULL;
		}

		EVL_make_value(tdbb, &desc, impure);
		UCHAR* p = impure->vlu_desc.dsc_address;

		memcpy(p, value1->dsc_address, value1->dsc_length);
		p += value1->dsc_length;
		memcpy(p, value2->dsc_address, value2->dsc_length);

		delete string;

		return &impure->vlu_desc;
	}

	DataTypeUtil(tdbb).makeConcatenate(&desc, value1, value2);

	// Both values are present; build the concatenation

	MoveBuffer temp1;
	UCHAR* address1 = NULL;
	USHORT length1 = 0;

	if (!value1->isBlob())
		length1 = MOV_make_string2(tdbb, value1, desc.getTextType(), &address1, temp1);

	MoveBuffer temp2;
	UCHAR* address2 = NULL;
	USHORT length2 = 0;

	if (!value2->isBlob())
		length2 = MOV_make_string2(tdbb, value2, desc.getTextType(), &address2, temp2);

	if (address1 && address2)
	{
		fb_assert(desc.dsc_dtype == dtype_varying);

		if ((ULONG) length1 + (ULONG) length2 > MAX_COLUMN_SIZE - sizeof(USHORT))
		{
			ERR_post(Arg::Gds(isc_concat_overflow));
			return NULL;
		}

		desc.dsc_dtype = dtype_text;
		desc.dsc_length = length1 + length2;
		desc.dsc_address = NULL;

		VaryingString* string = NULL;
		if (value1->dsc_address == impure->vlu_desc.dsc_address ||
			value2->dsc_address == impure->vlu_desc.dsc_address)
		{
			string = impure->vlu_string;
			impure->vlu_string = NULL;
		}

		EVL_make_value(tdbb, &desc, impure);
		UCHAR* p = impure->vlu_desc.dsc_address;

		if (length1)
		{
			memcpy(p, address1, length1);
			p += length1;
		}

		if (length2)
			memcpy(p, address2, length2);

		delete string;
	}
	else
	{
		fb_assert(desc.isBlob());

		desc.dsc_address = (UCHAR*)&impure->vlu_misc.vlu_bid;

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction,
			&impure->vlu_misc.vlu_bid);

		HalfStaticArray<UCHAR, BUFFER_SMALL> buffer;

		if (address1)
			BLB_put_data(tdbb, newBlob, address1, length1);	// first value is not a blob
		else
		{
			UCharBuffer bpb;
			BLB_gen_bpb_from_descs(value1, &desc, bpb);

			blb* blob = BLB_open2(tdbb, tdbb->getRequest()->req_transaction,
				reinterpret_cast<bid*>(value1->dsc_address), bpb.getCount(), bpb.begin());

			while (!(blob->blb_flags & BLB_eof))
			{
				SLONG len = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCapacity(), false);

				if (len)
					BLB_put_data(tdbb, newBlob, buffer.begin(), len);
			}

			BLB_close(tdbb, blob);
		}

		if (address2)
			BLB_put_data(tdbb, newBlob, address2, length2);	// second value is not a blob
		else
		{
			UCharBuffer bpb;
			BLB_gen_bpb_from_descs(value2, &desc, bpb);

			blb* blob = BLB_open2(tdbb, tdbb->getRequest()->req_transaction,
				reinterpret_cast<bid*>(value2->dsc_address), bpb.getCount(), bpb.begin());

			while (!(blob->blb_flags & BLB_eof))
			{
				SLONG len = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCapacity(), false);

				if (len)
					BLB_put_data(tdbb, newBlob, buffer.begin(), len);
			}

			BLB_close(tdbb, blob);
		}

		BLB_close(tdbb, newBlob);

		EVL_make_value(tdbb, &desc, impure);
	}

	return &impure->vlu_desc;
}

ExprNode* ConcatenateNode::internalDsqlPass()
{
	ConcatenateNode* node = FB_NEW(getPool()) ConcatenateNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg1), PASS1_node(dsqlScratch, dsqlArg2));
	node->dsqlScratch = dsqlScratch;

	return node;
}


//--------------------


OverNode::OverNode(MemoryPool& pool, dsql_nod* aAggExpr, dsql_nod* aPartition, dsql_nod* aOrder)
	: TypedNode<ExprNode, ExprNode::TYPE_OVER>(pool),
	  dsqlAggExpr(aAggExpr),
	  dsqlPartition(aPartition),
	  dsqlOrder(aOrder)
{
	addChildNode(dsqlAggExpr);
	addChildNode(dsqlPartition);
	addChildNode(dsqlOrder);
}

void OverNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "OverNode";
	ExprNode::print(text, nodes);
}

bool OverNode::dsqlAggregateFinder(AggregateFinder& visitor)
{
	bool aggregate = false;
	const bool wereWindow = visitor.window;
	AutoSetRestore<bool> autoWindow(&visitor.window, false);

	if (!wereWindow)
	{
		AggNode* aggNode = ExprNode::as<AggNode>(dsqlAggExpr);
		fb_assert(aggNode);

		Array<dsql_nod**>& exprChildren = aggNode->dsqlChildNodes;
		for (dsql_nod*** i = exprChildren.begin(); i != exprChildren.end(); ++i)
			aggregate |= visitor.visit(*i);
	}
	else
		aggregate |= visitor.visit(&dsqlAggExpr);

	aggregate |= visitor.visit(&dsqlPartition);
	aggregate |= visitor.visit(&dsqlOrder);

	return aggregate;
}

bool OverNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	bool found = false;

	{	// scope
		AutoSetRestore<bool> autoWindowOnly(&visitor.windowOnly, false);
		found |= visitor.visit(&dsqlAggExpr);
	}

	found |= visitor.visit(&dsqlPartition);
	found |= visitor.visit(&dsqlOrder);

	return found;
}

bool OverNode::dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
{
	bool invalid = false;

	// It's allowed to use an aggregate function of our context inside window functions.
	AutoSetRestore<bool> autoInsideHigherMap(&visitor.insideHigherMap, true);

	invalid |= visitor.visit(&dsqlAggExpr);
	invalid |= visitor.visit(&dsqlPartition);
	invalid |= visitor.visit(&dsqlOrder);

	return invalid;
}

bool OverNode::dsqlSubSelectFinder(SubSelectFinder& /*visitor*/)
{
	return false;
}

bool OverNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	// Save the values to restore them in the end.
	AutoSetRestore<dsql_nod*> autoPartitionNode(&visitor.partitionNode, visitor.partitionNode);
	AutoSetRestore<dsql_nod*> autoOrderNode(&visitor.orderNode, visitor.orderNode);

	if (dsqlPartition)
	{
		if (Aggregate2Finder::find(visitor.context->ctx_scope_level, FIELD_MATCH_TYPE_EQUAL,
				true, dsqlPartition))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_agg_nested_err));
		}

		visitor.partitionNode = dsqlPartition;
	}

	if (dsqlOrder)
	{
		if (Aggregate2Finder::find(visitor.context->ctx_scope_level, FIELD_MATCH_TYPE_EQUAL,
				true, dsqlOrder))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_agg_nested_err));
		}

		visitor.orderNode = dsqlOrder;
	}

	dsql_nod* const copy = dsqlAggExpr;

	AggNode* aggNode = ExprNode::as<AggNode>(copy);
	fb_assert(aggNode);

	Array<dsql_nod**>& exprChildren = aggNode->dsqlChildNodes;

	for (dsql_nod*** i = exprChildren.begin(); i != exprChildren.end(); ++i)
	{
		if (Aggregate2Finder::find(visitor.context->ctx_scope_level, FIELD_MATCH_TYPE_EQUAL,
				true, **i))
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					  Arg::Gds(isc_dsql_agg_nested_err));
		}
	}

	AggregateFinder aggFinder(dsqlScratch, false);
	aggFinder.deepestLevel = dsqlScratch->scopeLevel;
	aggFinder.currentLevel = visitor.currentLevel;

	if (aggFinder.visit(&copy))
	{
		if (!visitor.window)
		{
			{	// scope
				AutoSetRestore<dsql_nod*> autoPartitionNode2(&visitor.partitionNode, NULL);
				AutoSetRestore<dsql_nod*> autoOrderNode2(&visitor.orderNode, NULL);

				Array<dsql_nod**>& exprChildren = aggNode->dsqlChildNodes;
				for (dsql_nod*** i = exprChildren.begin(); i != exprChildren.end(); ++i)
					visitor.visit(*i);
			}

			if (dsqlPartition)
			{
				for (unsigned i = 0; i < dsqlPartition->nod_count; ++i)
				{
					AutoSetRestore<dsql_nod*> autoPartitionNode2(&visitor.partitionNode, NULL);
					AutoSetRestore<dsql_nod*> autoOrderNode2(&visitor.orderNode, NULL);

					visitor.visit(&dsqlPartition->nod_arg[i]);
				}
			}

			if (dsqlOrder)
			{
				for (unsigned i = 0; i < dsqlOrder->nod_count; ++i)
				{
					AutoSetRestore<dsql_nod*> autoPartitionNode(&visitor.partitionNode, NULL);
					AutoSetRestore<dsql_nod*> autoOrderNode(&visitor.orderNode, NULL);

					visitor.visit(&dsqlOrder->nod_arg[i]);
				}
			}
		}
		else if (dsqlScratch->scopeLevel == aggFinder.deepestLevel)
		{
			visitor.replaceNode(PASS1_post_map(dsqlScratch, copy, visitor.context,
				visitor.partitionNode, visitor.orderNode));
		}
	}

	return false;
}

void OverNode::setParameterName(dsql_par* parameter) const
{
	MAKE_parameter_names(parameter, dsqlAggExpr);
}

void OverNode::genBlr()
{
	GEN_expr(dsqlScratch, dsqlAggExpr);
}

void OverNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	MAKE_desc(dsqlScratch, desc, dsqlAggExpr, nullReplacement);
	desc->setNullable(true);
}

void OverNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* /*desc*/)
{
	fb_assert(false);
}

ExprNode* OverNode::copy(thread_db* /*tdbb*/, NodeCopier& /*copier*/) const
{
	fb_assert(false);
	return NULL;
}

dsc* OverNode::execute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
{
	fb_assert(false);
	return NULL;
}

ExprNode* OverNode::internalDsqlPass()
{
	OverNode* node = FB_NEW(getPool()) OverNode(getPool(),
		PASS1_node(dsqlScratch, dsqlAggExpr),
		PASS1_node(dsqlScratch, dsqlPartition),
		PASS1_node(dsqlScratch, dsqlOrder));
	node->dsqlScratch = dsqlScratch;

	return node;
}


//--------------------


static RegisterNode<SubstringSimilarNode> regSubstringSimilarNode(blr_substring_similar);

SubstringSimilarNode::SubstringSimilarNode(MemoryPool& pool, dsql_nod* aExpr, dsql_nod* aPattern,
			dsql_nod* aEscape)
	: TypedNode<ExprNode, ExprNode::TYPE_SUBSTRING_SIMILAR>(pool),
	  dsqlExpr(aExpr),
	  dsqlPattern(aPattern),
	  dsqlEscape(aEscape),
	  expr(NULL),
	  pattern(NULL),
	  escape(NULL)
{
	addChildNode(dsqlExpr, expr);
	addChildNode(dsqlPattern, pattern);
	addChildNode(dsqlEscape, escape);
}

DmlNode* SubstringSimilarNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	SubstringSimilarNode* node = FB_NEW(pool) SubstringSimilarNode(pool);
	node->expr = PAR_parse_node(tdbb, csb, VALUE);
	node->pattern = PAR_parse_node(tdbb, csb, VALUE);
	node->escape = PAR_parse_node(tdbb, csb, VALUE);
	return node;
}

void SubstringSimilarNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SubstringSimilarNode";
	ExprNode::print(text, nodes);
}

void SubstringSimilarNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "SUBSTRING";
}

bool SubstringSimilarNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar) const
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlExpr, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlPattern, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlEscape, node, forceVarChar);
}

void SubstringSimilarNode::genBlr()
{
	dsqlScratch->appendUChar(blr_substring_similar);
	GEN_expr(dsqlScratch, dsqlExpr);
	GEN_expr(dsqlScratch, dsqlPattern);
	GEN_expr(dsqlScratch, dsqlEscape);
}

void SubstringSimilarNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	MAKE_desc(dsqlScratch, desc, dsqlExpr, nullReplacement);
	desc->setNullable(true);
}

void SubstringSimilarNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	CMP_get_desc(tdbb, csb, expr, desc);

	dsc tempDesc;
	CMP_get_desc(tdbb, csb, pattern, &tempDesc);
	CMP_get_desc(tdbb, csb, escape, &tempDesc);
}

ExprNode* SubstringSimilarNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	SubstringSimilarNode* node = FB_NEW(*tdbb->getDefaultPool()) SubstringSimilarNode(
		*tdbb->getDefaultPool());
	node->expr = copier.copy(tdbb, expr);
	node->pattern = copier.copy(tdbb, pattern);
	node->escape = copier.copy(tdbb, escape);
	return node;
}

ExprNode* SubstringSimilarNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	expr = CMP_pass1(tdbb, csb, expr);

	// We need to take care of invariantness expressions to be able to pre-compile the pattern.
	node->nod_flags |= nod_invariant;
	csb->csb_current_nodes.push(node);

	pattern = CMP_pass1(tdbb, csb, pattern);
	escape = CMP_pass1(tdbb, csb, escape);

	csb->csb_current_nodes.pop();

	// If there is no top-level RSE present and patterns are not constant, unmark node as invariant
	// because it may be dependent on data or variables.
	if ((node->nod_flags & nod_invariant) &&
		(pattern->nod_type != nod_literal || escape->nod_type != nod_literal))
	{
		jrd_node_base **ctx_node, **end;
		for (ctx_node = csb->csb_current_nodes.begin(), end = csb->csb_current_nodes.end();
			 ctx_node < end; ctx_node++)
		{
			if ((*ctx_node)->nod_type == nod_rse)
				break;
		}

		if (ctx_node >= end)
			node->nod_flags &= ~nod_invariant;
	}

	return this;
}

ExprNode* SubstringSimilarNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	if (node->nod_flags & nod_invariant)
		csb->csb_invariants.push(node);

	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* SubstringSimilarNode::execute(thread_db* tdbb, jrd_req* request) const
{
	// Run all expression arguments.

	const dsc* exprDesc = EVL_expr(tdbb, expr);
	exprDesc = (request->req_flags & req_null) ? NULL : exprDesc;

	const dsc* patternDesc = EVL_expr(tdbb, pattern);
	patternDesc = (request->req_flags & req_null) ? NULL : patternDesc;

	const dsc* escapeDesc = EVL_expr(tdbb, escape);
	escapeDesc = (request->req_flags & req_null) ? NULL : escapeDesc;

	// If any of them is NULL, return NULL.
	if (!exprDesc || !patternDesc || !escapeDesc)
		return NULL;

	USHORT textType = exprDesc->getTextType();
	Collation* collation = INTL_texttype_lookup(tdbb, textType);
	CharSet* charSet = collation->getCharSet();

	MoveBuffer exprBuffer;
	UCHAR* exprStr;
	int exprLen = MOV_make_string2(tdbb, exprDesc, textType, &exprStr, exprBuffer);

	MoveBuffer patternBuffer;
	UCHAR* patternStr;
	int patternLen = MOV_make_string2(tdbb, patternDesc, textType, &patternStr, patternBuffer);

	MoveBuffer escapeBuffer;
	UCHAR* escapeStr;
	int escapeLen = MOV_make_string2(tdbb, escapeDesc, textType, &escapeStr, escapeBuffer);

	impure_value* impure = request->getImpure<impure_value>(node->nod_impure);

	AutoPtr<BaseSimilarToMatcher> autoEvaluator;	// deallocate non-invariant evaluator
	BaseSimilarToMatcher* evaluator;

	if (node->nod_flags & nod_invariant)
	{
		if (!(impure->vlu_flags & VLU_computed))
		{
			delete impure->vlu_misc.vlu_invariant;

			impure->vlu_misc.vlu_invariant = evaluator = collation->createSimilarToMatcher(
				*tdbb->getDefaultPool(), patternStr, patternLen, escapeStr, escapeLen, true);

			impure->vlu_flags |= VLU_computed;
		}
		else
		{
			evaluator = static_cast<BaseSimilarToMatcher*>(impure->vlu_misc.vlu_invariant);
			evaluator->reset();
		}
	}
	else
	{
		autoEvaluator = evaluator = collation->createSimilarToMatcher(*tdbb->getDefaultPool(),
			patternStr, patternLen, escapeStr, escapeLen, true);
	}

	evaluator->process(exprStr, exprLen);

	if (evaluator->result())
	{
		// Get the bounds of the matched substring.
		unsigned start = 0;
		unsigned length = 0;
		evaluator->getBranchInfo(1, &start, &length);

		dsc desc;
		desc.makeText((USHORT) exprLen, textType);
		EVL_make_value(tdbb, &desc, impure);

		// And return it.
		impure->vlu_desc.dsc_length = charSet->substring(exprLen, exprStr,
			impure->vlu_desc.dsc_length, impure->vlu_desc.dsc_address, start, length);

		return &impure->vlu_desc;
	}
	else
		return NULL;	// No match. Return NULL.
}

ExprNode* SubstringSimilarNode::internalDsqlPass()
{
	SubstringSimilarNode* node = FB_NEW(getPool()) SubstringSimilarNode(getPool(),
		PASS1_node(dsqlScratch, dsqlExpr),
		PASS1_node(dsqlScratch, dsqlPattern),
		PASS1_node(dsqlScratch, dsqlEscape));
	node->dsqlScratch = dsqlScratch;

	// ? SIMILAR FIELD case.
	PASS1_set_parameter_type(dsqlScratch, node->dsqlExpr, node->dsqlPattern, true);

	// FIELD SIMILAR ? case.
	PASS1_set_parameter_type(dsqlScratch, node->dsqlPattern, node->dsqlExpr, true);

	// X SIMILAR Y ESCAPE ? case.
	PASS1_set_parameter_type(dsqlScratch, node->dsqlEscape, node->dsqlPattern, true);

	return node;
}


//--------------------


static RegisterNode<SysFuncCallNode> regSysFuncCallNode(blr_sys_function);

SysFuncCallNode::SysFuncCallNode(MemoryPool& pool, const MetaName& aName, dsql_nod* aArgs)
	: TypedNode<ExprNode, ExprNode::TYPE_SYSFUNC_CALL>(pool),
	  name(pool, aName),
	  dsqlArgs(aArgs),
	  dsqlSpecialSyntax(false),
	  args(NULL),
	  function(NULL)
{
	addChildNode(dsqlArgs, args);
}

DmlNode* SysFuncCallNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	MetaName name;
	const USHORT count = PAR_name(csb, name);

	SysFuncCallNode* node = FB_NEW(pool) SysFuncCallNode(pool, name);
	node->function = SysFunction::lookup(name);

	if (!node->function)
	{
		csb->csb_blr_reader.seekBackward(count);
		PAR_error(csb, Arg::Gds(isc_funnotdef) << Arg::Str(name));
	}

	node->args = PAR_args(tdbb, csb, VALUE);

	return node;
}

bool SysFuncCallNode::isArrayOrBlob(DsqlCompilerScratch* dsqlScratch) const
{
	Array<const dsc*> argsArray;

	for (dsql_nod** p = dsqlArgs->nod_arg; p < dsqlArgs->nod_arg + dsqlArgs->nod_count; ++p)
	{
		MAKE_desc(dsqlScratch, &(*p)->nod_desc, *p, NULL);
		argsArray.add(&(*p)->nod_desc);
	}

	dsc desc;
	DSqlDataTypeUtil(dsqlScratch).makeSysFunction(&desc, name.c_str(),
		argsArray.getCount(), argsArray.begin());

	return desc.isBlob();
}

void SysFuncCallNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SysFuncCallNode\n\tname: " + string(name.c_str());
	ExprNode::print(text, nodes);
}

void SysFuncCallNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = name;
}

void SysFuncCallNode::genBlr()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	dsqlScratch->appendUChar(blr_sys_function);
	dsqlScratch->appendMetaString(function->name.c_str());
	dsqlScratch->appendUChar(dsqlArgs->nod_count);

	dsql_nod* const* ptr = dsqlArgs->nod_arg;
	for (const dsql_nod* const* const end = ptr + dsqlArgs->nod_count; ptr < end; ptr++)
		GEN_expr(dsqlScratch, *ptr);
}

void SysFuncCallNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	Array<const dsc*> argsArray;

	for (dsql_nod** p = dsqlArgs->nod_arg; p < dsqlArgs->nod_arg + dsqlArgs->nod_count; ++p)
	{
		MAKE_desc(dsqlScratch, &(*p)->nod_desc, *p, NULL);
		argsArray.add(&(*p)->nod_desc);
	}

	DSqlDataTypeUtil(dsqlScratch).makeSysFunction(desc, name.c_str(),
		argsArray.getCount(), argsArray.begin());
}

void SysFuncCallNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	fb_assert(args->nod_type == nod_list);

	Array<const dsc*> argsArray;

	for (jrd_nod** p = args->nod_arg; p < args->nod_arg + args->nod_count; ++p)
	{
		dsc* targetDesc = FB_NEW(*tdbb->getDefaultPool()) dsc();
		argsArray.push(targetDesc);
		CMP_get_desc(tdbb, csb, *p, targetDesc);

		// dsc_address is verified in makeFunc to get literals. If the node is not a
		// literal, set it to NULL, to prevent wrong interpretation of offsets as
		// pointers - CORE-2612.
		if ((*p)->nod_type != nod_literal)
			targetDesc->dsc_address = NULL;
	}

	DataTypeUtil dataTypeUtil(tdbb);
	function->makeFunc(&dataTypeUtil, function, desc, argsArray.getCount(), argsArray.begin());

	for (const dsc** pArgs = argsArray.begin(); pArgs != argsArray.end(); ++pArgs)
		delete *pArgs;
}

ExprNode* SysFuncCallNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	SysFuncCallNode* node = FB_NEW(*tdbb->getDefaultPool()) SysFuncCallNode(
		*tdbb->getDefaultPool(), name);
	node->args = copier.copy(tdbb, args);
	node->function = function;
	return node;
}

bool SysFuncCallNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const SysFuncCallNode* otherNode = other->as<SysFuncCallNode>();

	return name == otherNode->name;
}

ExprNode* SysFuncCallNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	function->checkArgsMismatch(args->nod_count);

	return this;
}

dsc* SysFuncCallNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(node->nod_impure);
	return function->evlFunc(tdbb, function, args, impure);
}

ExprNode* SysFuncCallNode::internalDsqlPass()
{
	QualifiedName qualifName(name);

	if (!dsqlSpecialSyntax && METD_get_function(dsqlScratch->getTransaction(), dsqlScratch, qualifName))
	{
		UdfCallNode* node = FB_NEW(getPool()) UdfCallNode(getPool(), qualifName, dsqlArgs);
		return static_cast<ExprNode*>(node->dsqlPass(dsqlScratch));
	}

	SysFuncCallNode* node = FB_NEW(getPool()) SysFuncCallNode(getPool(), name,
		PASS1_node(dsqlScratch, dsqlArgs));
	node->dsqlScratch = dsqlScratch;
	node->dsqlSpecialSyntax = dsqlSpecialSyntax;

	node->function = SysFunction::lookup(name);

	if (node->function && node->function->setParamsFunc)
	{
		Array<dsc*> argsArray;
		dsql_nod* inArgs = node->dsqlArgs;

		for (unsigned int i = 0; i < inArgs->nod_count; ++i)
		{
			dsql_nod* p = inArgs->nod_arg[i];
			MAKE_desc(dsqlScratch, &p->nod_desc, p, p);
			argsArray.add(&p->nod_desc);
		}

		DSqlDataTypeUtil dataTypeUtil(dsqlScratch);
		node->function->setParamsFunc(&dataTypeUtil, node->function,
			argsArray.getCount(), argsArray.begin());

		for (unsigned int i = 0; i < inArgs->nod_count; ++i)
		{
			dsql_nod* p = inArgs->nod_arg[i];
			PASS1_set_parameter_type(dsqlScratch, p, p, false);
		}
	}

	return node;
}


//--------------------


static RegisterNode<UdfCallNode> regUdfCallNode1(blr_function);
static RegisterNode<UdfCallNode> regUdfCallNode2(blr_function2);

UdfCallNode::UdfCallNode(MemoryPool& pool, const QualifiedName& aName, dsql_nod* aArgs)
	: TypedNode<ExprNode, ExprNode::TYPE_UDF_CALL>(pool),
	  name(pool, aName),
	  dsqlArgs(aArgs),
	  args(NULL),
	  function(NULL),
	  dsqlFunction(NULL)
{
	addChildNode(dsqlArgs, args);
}

DmlNode* UdfCallNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR blrOp)
{
	const UCHAR* savePos = csb->csb_blr_reader.getPos();

	QualifiedName name;
	USHORT count = 0;

	if (blrOp == blr_function2)
		count = PAR_name(csb, name.package);

	count += PAR_name(csb, name.identifier);

	UdfCallNode* node = FB_NEW(pool) UdfCallNode(pool, name);

	if (blrOp == blr_function &&
		(name.identifier == "RDB$GET_CONTEXT" || name.identifier == "RDB$SET_CONTEXT"))
	{
		csb->csb_blr_reader.setPos(savePos);
		return SysFuncCallNode::parse(tdbb, pool, csb, blr_sys_function);
	}

	node->function = Function::lookup(tdbb, name, false);

	if (node->function)
	{
		if (!node->function->isUndefined() && !node->function->fun_entrypoint &&
			!node->function->fun_external && !node->function->getStatement())
		{
			if (tdbb->getAttachment()->att_flags & ATT_gbak_attachment)
			{
				PAR_warning(Arg::Warning(isc_funnotdef) << Arg::Str(name.toString()) <<
							Arg::Warning(isc_modnotfound));
			}
			else
			{
				csb->csb_blr_reader.seekBackward(count);
				PAR_error(csb, Arg::Gds(isc_funnotdef) << Arg::Str(name.toString()) <<
						   Arg::Gds(isc_modnotfound));
			}
		}
	}
	else
	{
		csb->csb_blr_reader.seekBackward(count);
		PAR_error(csb, Arg::Gds(isc_funnotdef) << Arg::Str(name.toString()));
	}

	node->args = node->function->parseArgs(tdbb, csb);

    // CVC: I will track ufds only if a proc is not being dropped.
    if (csb->csb_g_flags & csb_get_dependencies)
    {
        jrd_nod* dep_node = PAR_make_node(tdbb, e_dep_length);
        dep_node->nod_type = nod_dependency;
        dep_node->nod_arg [e_dep_object] = (jrd_nod*) node->function;
        dep_node->nod_arg [e_dep_object_type] = (jrd_nod*)(IPTR) obj_udf;
        csb->csb_dependencies.push(dep_node);
    }

	return node;
}

bool UdfCallNode::isArrayOrBlob(DsqlCompilerScratch* /*dsqlScratch*/) const
{
	// Parameters to UDF don't need checking, a blob or array can be passed.
	return dsqlFunction->udf_dtype == dtype_blob || dsqlFunction->udf_dtype == dtype_array;
}

void UdfCallNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "UdfCallNode\n\tname: " + name.toString();
	ExprNode::print(text, nodes);
}

void UdfCallNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = dsqlFunction->udf_name.identifier;
}

void UdfCallNode::genBlr()
{
	DsqlCompiledStatement* statement = dsqlScratch->getStatement();

	if (dsqlFunction->udf_name.package.isEmpty())
		dsqlScratch->appendUChar(blr_function);
	else
	{
		dsqlScratch->appendUChar(blr_function2);
		dsqlScratch->appendMetaString(dsqlFunction->udf_name.package.c_str());
	}

	dsqlScratch->appendMetaString(dsqlFunction->udf_name.identifier.c_str());
	dsqlScratch->appendUChar(dsqlArgs->nod_count);

	dsql_nod* const* ptr = dsqlArgs->nod_arg;
	for (const dsql_nod* const* const end = ptr + dsqlArgs->nod_count; ptr < end; ptr++)
		GEN_expr(dsqlScratch, *ptr);
}

void UdfCallNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->dsc_dtype = static_cast<UCHAR>(dsqlFunction->udf_dtype);
	desc->dsc_length = dsqlFunction->udf_length;
	desc->dsc_scale = static_cast<SCHAR>(dsqlFunction->udf_scale);
	// CVC: Setting flags to zero obviously impeded DSQL to acknowledge
	// the fact that any UDF can return NULL simply returning a NULL
	// pointer.
	desc->setNullable(true);

	if (desc->dsc_dtype <= dtype_any_text)
		desc->dsc_ttype() = dsqlFunction->udf_character_set_id;
	else
		desc->dsc_ttype() = dsqlFunction->udf_sub_type;
}

void UdfCallNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	// Null value for the function indicates that the function was not
	// looked up during parsing the BLR. This is true if the function
	// referenced in the procedure BLR was dropped before dropping the
	// procedure itself. Ignore the case because we are currently trying
	// to drop the procedure.
	// For normal requests, function would never be null. We would have
	// created a valid block while parsing.
	if (function)
		*desc = function->fun_args[function->fun_return_arg].fun_parameter->prm_desc;
	else
		desc->clear();
}

ExprNode* UdfCallNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	UdfCallNode* node = FB_NEW(*tdbb->getDefaultPool()) UdfCallNode(*tdbb->getDefaultPool(), name);
	node->args = copier.copy(tdbb, args);
	node->function = function;
	return node;
}

bool UdfCallNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const UdfCallNode* otherNode = other->as<UdfCallNode>();

	return name == otherNode->name;
}

ExprNode* UdfCallNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	if (!(csb->csb_g_flags & (csb_internal | csb_ignore_perm)))
	{
		const TEXT* secName = function->getSecurityName().nullStr();

		if (function->getName().package.isEmpty())
		{
			CMP_post_access(tdbb, csb, secName, 0, SCL_execute, SCL_object_function,
				function->getName().identifier.c_str());
		}
		else
		{
			CMP_post_access(tdbb, csb, secName, 0, SCL_execute, SCL_object_package,
				function->getName().package.c_str());
		}

		ExternalAccess temp(ExternalAccess::exa_function, function->getId());
		size_t idx;
		if (!csb->csb_external.find(temp, idx))
			csb->csb_external.insert(idx, temp);
	}

	CMP_post_resource(&csb->csb_resources, function, Resource::rsc_function, function->getId());

	return this;
}

ExprNode* UdfCallNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = function->allocateImpure(csb);

	return this;
}

dsc* UdfCallNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(node->nod_impure);
	return function->execute(tdbb, args, impure);
}

ExprNode* UdfCallNode::internalDsqlPass()
{
	UdfCallNode* node = FB_NEW(getPool()) UdfCallNode(getPool(), name,
		PASS1_node(dsqlScratch, dsqlArgs));
	node->dsqlScratch = dsqlScratch;
	node->dsqlFunction = METD_get_function(dsqlScratch->getTransaction(), dsqlScratch, name);

	if (!node->dsqlFunction)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
				  Arg::Gds(isc_dsql_function_err) <<
				  Arg::Gds(isc_random) << Arg::Str(name.toString()));
	}

	dsql_nod** ptr = node->dsqlArgs->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->dsqlArgs->nod_count; ptr < end; ptr++)
	{
		unsigned pos = ptr - node->dsqlArgs->nod_arg;

		if (pos < node->dsqlFunction->udf_arguments.getCount())
		{
			dsql_nod temp;
			temp.nod_desc = node->dsqlFunction->udf_arguments[pos];
			PASS1_set_parameter_type(dsqlScratch, *ptr, &temp, false);
		}
		else
		{
			// We should complain here in the future! The parameter is
			// out of bounds or the function doesn't declare input params.
		}
	}

	return node;
}


}	// namespace Jrd
