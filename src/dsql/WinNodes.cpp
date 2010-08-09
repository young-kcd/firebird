/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Adriano dos Santos Fernandes <adrianosf@gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../dsql/WinNodes.h"
#include "../dsql/make_proto.h"
#include "../dsql/pass1_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/recsrc/RecordSource.h"

using namespace Firebird;
using namespace Jrd;

#include "gen/blrtable.h"

namespace Jrd {


static RegisterNode<WinFuncNode> regWinFuncNode(blr_agg_function);

WinFuncNode::Factory* WinFuncNode::factories = NULL;

WinFuncNode::WinFuncNode(MemoryPool& pool, const AggInfo& aAggInfo, dsql_nod* aArg)
	: AggNode(pool, aAggInfo, false, false, aArg)
{
}

DmlNode* WinFuncNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	MetaName name;
	PAR_name(csb, name);

	WinFuncNode* node = NULL;

	for (const Factory* factory = factories; factory; factory = factory->next)
	{
		if (name == factory->name)
		{
			node = factory->newInstance(pool);
			break;
		}
	}

	if (!node)
		PAR_error(csb, Arg::Gds(isc_funnotdef) << name);

	UCHAR count = csb->csb_blr_reader.getByte();

	if (count != node->jrdChildNodes.getCount())
		PAR_error(csb, Arg::Gds(isc_funmismat) << name);

	if (count != 0)
	{
		NestConst<jrd_nod>** arg = node->jrdChildNodes.begin();
		do
		{
			**arg++ = PAR_parse_node(tdbb, csb, VALUE);
		} while (--count);
	}

	return node;
}


//--------------------


static WinFuncNode::Register<DenseRankWinNode> denseRankWinInfo("DENSE_RANK");

DenseRankWinNode::DenseRankWinNode(MemoryPool& pool)
	: WinFuncNode(pool, denseRankWinInfo)
{
	fb_assert(dsqlChildNodes.getCount() == 1 && jrdChildNodes.getCount() == 1);
	dsqlChildNodes.clear();
	jrdChildNodes.clear();
}

void DenseRankWinNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->makeInt64(0);
}

void DenseRankWinNode::getDesc(thread_db* tdbb, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* DenseRankWinNode::copy(thread_db* tdbb, NodeCopier& /*copier*/)
{
	DenseRankWinNode* node = FB_NEW(*tdbb->getDefaultPool()) DenseRankWinNode(*tdbb->getDefaultPool());
	return node;
}

void DenseRankWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	impure->make_int64(0, 0);
}

void DenseRankWinNode::aggPass(thread_db* /*tdbb*/, jrd_req* /*request*/, dsc* /*desc*/) const
{
}

dsc* DenseRankWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	++impure->vlu_misc.vlu_int64;
	return &impure->vlu_desc;
}

AggNode* DenseRankWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) DenseRankWinNode(getPool());
}


//--------------------


static WinFuncNode::Register<RankWinNode> rankWinInfo("RANK");

RankWinNode::RankWinNode(MemoryPool& pool)
	: WinFuncNode(pool, rankWinInfo),
	  tempImpure(0)
{
	fb_assert(dsqlChildNodes.getCount() == 1 && jrdChildNodes.getCount() == 1);
	dsqlChildNodes.clear();
	jrdChildNodes.clear();
}

void RankWinNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->makeInt64(0);
}

void RankWinNode::getDesc(thread_db* tdbb, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* RankWinNode::copy(thread_db* tdbb, NodeCopier& /*copier*/)
{
	RankWinNode* node = FB_NEW(*tdbb->getDefaultPool()) RankWinNode(*tdbb->getDefaultPool());
	return node;
}

ExprNode* RankWinNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	tempImpure = CMP_impure(csb, sizeof(impure_value_ex));
	return AggNode::pass2(tdbb, csb);
}

void RankWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	impure->make_int64(1, 0);
	impure->vlux_count = 0;
}

void RankWinNode::aggPass(thread_db* tdbb, jrd_req* request, dsc* /*desc*/) const
{
	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	++impure->vlux_count;
}

dsc* RankWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);

	dsc temp;
	temp.makeInt64(0, &impure->vlu_misc.vlu_int64);

	impure_value_ex* impureTemp = request->getImpure<impure_value_ex>(tempImpure);
	EVL_make_value(tdbb, &temp, impureTemp);

	impure->vlu_misc.vlu_int64 += impure->vlux_count;
	impure->vlux_count = 0;

	return &impureTemp->vlu_desc;
}

AggNode* RankWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) RankWinNode(getPool());
}


//--------------------


static WinFuncNode::Register<RowNumberWinNode> rowNumberWinInfo("ROW_NUMBER");

RowNumberWinNode::RowNumberWinNode(MemoryPool& pool)
	: WinFuncNode(pool, rowNumberWinInfo)
{
	fb_assert(dsqlChildNodes.getCount() == 1 && jrdChildNodes.getCount() == 1);
	dsqlChildNodes.clear();
	jrdChildNodes.clear();
}

void RowNumberWinNode::make(dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->makeInt64(0);
}

void RowNumberWinNode::getDesc(thread_db* tdbb, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* RowNumberWinNode::copy(thread_db* tdbb, NodeCopier& /*copier*/)
{
	RowNumberWinNode* node = FB_NEW(*tdbb->getDefaultPool()) RowNumberWinNode(*tdbb->getDefaultPool());
	return node;
}

void RowNumberWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	impure->make_int64(0, 0);
}

void RowNumberWinNode::aggPass(thread_db* /*tdbb*/, jrd_req* /*request*/, dsc* /*desc*/) const
{
}

dsc* RowNumberWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	return &impure->vlu_desc;
}

dsc* RowNumberWinNode::winPass(thread_db* tdbb, jrd_req* request, SlidingWindow* /*window*/) const
{
	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	++impure->vlu_misc.vlu_int64;
	return &impure->vlu_desc;
}

AggNode* RowNumberWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) RowNumberWinNode(getPool());
}


//--------------------


// A direction of -1 is LAG, and 1 is LEAD.
LagLeadWinNode::LagLeadWinNode(MemoryPool& pool, const AggInfo& aAggInfo, int aDirection,
			dsql_nod* aArg, dsql_nod* aRows, dsql_nod* aOutExpr)
	: WinFuncNode(pool, aAggInfo, aArg),
	  direction(aDirection),
	  dsqlRows(aRows),
	  dsqlOutExpr(aOutExpr),
	  rows(NULL),
	  outExpr(NULL)
{
	fb_assert(direction == -1 || direction == 1);

	addChildNode(dsqlRows, rows);
	addChildNode(dsqlOutExpr, outExpr);
}

void LagLeadWinNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	MAKE_desc(dsqlScratch, desc, dsqlArg, nullReplacement);
	desc->setNullable(true);
}

void LagLeadWinNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	CMP_get_desc(tdbb, csb, arg, desc);
}

void LagLeadWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = request->getImpure<impure_value_ex>(node->nod_impure);
	impure->make_int64(0, 0);
}

void LagLeadWinNode::aggPass(thread_db* /*tdbb*/, jrd_req* /*request*/, dsc* /*desc*/) const
{
}

dsc* LagLeadWinNode::aggExecute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
{
	return NULL;
}

dsc* LagLeadWinNode::winPass(thread_db* tdbb, jrd_req* request, SlidingWindow* window) const
{
	dsc* desc = EVL_expr(tdbb, rows);
	SINT64 records;

	if (!desc || (request->req_flags & req_null) || (records = MOV_get_int64(desc, 0)) < 0)
	{
		status_exception::raise(Arg::Gds(isc_sysf_argnmustbe_nonneg) <<
			Arg::Num(2) << Arg::Str(aggInfo.name));
	}

	if (!window->move(records * direction))
	{
		window->move(0);	// Come back to our row because outExpr may reference columns.

		desc = EVL_expr(tdbb, outExpr);
		if (!desc || (request->req_flags & req_null))
			return NULL;

		return desc;
	}

	desc = EVL_expr(tdbb, arg);
	if (!desc || (request->req_flags & req_null))
		return NULL;

	return desc;
}


//--------------------


static WinFuncNode::Register<LagWinNode> lagWinInfo("LAG");

LagWinNode::LagWinNode(MemoryPool& pool, dsql_nod* aArg, dsql_nod* aRows, dsql_nod* aOutExpr)
	: LagLeadWinNode(pool, lagWinInfo, -1, aArg, aRows, aOutExpr)
{
}

ExprNode* LagWinNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	LagWinNode* node = FB_NEW(*tdbb->getDefaultPool()) LagWinNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	node->rows = copier.copy(tdbb, rows);
	node->outExpr = copier.copy(tdbb, outExpr);
	return node;
}

AggNode* LagWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) LagWinNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg),
		PASS1_node(dsqlScratch, dsqlRows),
		PASS1_node(dsqlScratch, dsqlOutExpr));
}


//--------------------


static WinFuncNode::Register<LeadWinNode> leadWinInfo("LEAD");

LeadWinNode::LeadWinNode(MemoryPool& pool, dsql_nod* aArg, dsql_nod* aRows, dsql_nod* aOutExpr)
	: LagLeadWinNode(pool, leadWinInfo, 1, aArg, aRows, aOutExpr)
{
}

ExprNode* LeadWinNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	LeadWinNode* node = FB_NEW(*tdbb->getDefaultPool()) LeadWinNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	node->rows = copier.copy(tdbb, rows);
	node->outExpr = copier.copy(tdbb, outExpr);
	return node;
}

AggNode* LeadWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) LeadWinNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg),
		PASS1_node(dsqlScratch, dsqlRows),
		PASS1_node(dsqlScratch, dsqlOutExpr));
}


}	// namespace Jrd
