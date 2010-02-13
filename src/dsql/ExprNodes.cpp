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
#include "../dsql/pass1_proto.h"
#include "../dsql/utld_proto.h"
#include "../jrd/DataTypeUtil.h"

using namespace Firebird;
using namespace Jrd;

#include "gen/blrtable.h"

namespace Jrd {


ExprNode* ExprNode::fromLegacy(dsql_nod* node)
{
	return node->nod_type == Dsql::nod_class_exprnode ?
		reinterpret_cast<ExprNode*>(node->nod_arg[0]) : NULL;
}

ExprNode* ExprNode::fromLegacy(jrd_nod* node)
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
	stuff(dsqlScratch->getStatement(), blr_concatenate);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

void ConcatenateNode::make(dsc* desc, dsql_nod* nullReplacement)
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

	impure_value* impure = (impure_value*) ((SCHAR*) request + node->nod_impure);
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
	bool wereWindow = visitor.window;
	AutoSetRestore<bool> autoWindow(&visitor.window, false);

	if (!wereWindow)
	{
		AggNode* aggNode = ExprNode::as<AggNode>(dsqlAggExpr);
		fb_assert(aggNode);

		aggregate |= visitor.visit(&aggNode->dsqlArg);
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

bool OverNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return false;
}

bool OverNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
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

	dsql_nod*& aggExpr = aggNode->dsqlArg;

	if (Aggregate2Finder::find(visitor.context->ctx_scope_level, FIELD_MATCH_TYPE_EQUAL,
			true, aggExpr))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_dsql_agg_nested_err));
	}

	AggregateFinder aggFinder(dsqlScratch, false);
	aggFinder.deepestLevel = dsqlScratch->scopeLevel;
	aggFinder.currentLevel = visitor.currentLevel;

	if (aggFinder.visit(&copy))
	{
		if (!visitor.window)
		{
			if (copy->nod_count)
			{
				AutoSetRestore<dsql_nod*> autoPartitionNode2(&visitor.partitionNode, NULL);
				AutoSetRestore<dsql_nod*> autoOrderNode2(&visitor.orderNode, NULL);

				visitor.visit(&aggExpr);
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

void OverNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	fb_assert(false);
}

ExprNode* OverNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	fb_assert(false);
	return NULL;
}

dsc* OverNode::execute(thread_db* tdbb, jrd_req* request) const
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


}	// namespace Jrd
