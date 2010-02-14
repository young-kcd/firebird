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
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"

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

DmlNode* WinFuncNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	MetaName name;
	PAR_name(csb, name);

	UCHAR count = csb->csb_blr_reader.getByte();

	fb_assert(count == 0);	// Arguments not yet supported here.
	if (count != 0)
		PAR_error(csb, Arg::Gds(isc_funmismat) << name);

	for (const Factory* factory = factories; factory; factory = factory->next)
	{
		if (name == factory->name)
			return factory->newInstance(pool);
	}

	PAR_error(csb, Arg::Gds(isc_funmismat) << name);
	return NULL;	// silence
}


//--------------------


static WinFuncNode::Register<DenseRankWinNode> denseRankWinInfo("DENSE_RANK");

DenseRankWinNode::DenseRankWinNode(MemoryPool& pool)
	: WinFuncNode(pool, denseRankWinInfo)
{
}

void DenseRankWinNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	desc->makeInt64(0);
}

void DenseRankWinNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* DenseRankWinNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	DenseRankWinNode* node = FB_NEW(*tdbb->getDefaultPool()) DenseRankWinNode(*tdbb->getDefaultPool());
	return node;
}

void DenseRankWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	impure->make_int64(0, 0);
}

void DenseRankWinNode::aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const
{
}

dsc* DenseRankWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
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
}

void RankWinNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	desc->makeInt64(0);
}

void RankWinNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* RankWinNode::copy(thread_db* tdbb, NodeCopier& copier) const
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

	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	impure->make_int64(1, 0);
	impure->vlux_count = 0;
}

void RankWinNode::aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const
{
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	++impure->vlux_count;
}

dsc* RankWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);

	dsc temp;
	temp.makeInt64(0, &impure->vlu_misc.vlu_int64);

	impure_value_ex* impureTemp = (impure_value_ex*) ((SCHAR*) request + tempImpure);
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
}

void RowNumberWinNode::make(dsc* desc, dsql_nod* nullReplacement)
{
	desc->makeInt64(0);
}

void RowNumberWinNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->makeInt64(0);
}

ExprNode* RowNumberWinNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	RowNumberWinNode* node = FB_NEW(*tdbb->getDefaultPool()) RowNumberWinNode(*tdbb->getDefaultPool());
	return node;
}

void RowNumberWinNode::aggInit(thread_db* tdbb, jrd_req* request) const
{
	AggNode::aggInit(tdbb, request);

	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	impure->make_int64(0, 0);
}

void RowNumberWinNode::aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const
{
}

dsc* RowNumberWinNode::aggExecute(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	return &impure->vlu_desc;
}

dsc* RowNumberWinNode::winPass(thread_db* tdbb, jrd_req* request) const
{
	impure_value_ex* impure = (impure_value_ex*) ((SCHAR*) request + node->nod_impure);
	++impure->vlu_misc.vlu_int64;
	return &impure->vlu_desc;
}

AggNode* RowNumberWinNode::dsqlCopy() const
{
	return FB_NEW(getPool()) RowNumberWinNode(getPool());
}


}	// namespace Jrd
