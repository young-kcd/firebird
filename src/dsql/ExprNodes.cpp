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
#include <math.h>
#include <ctype.h>
#include "../common/common.h"
#include "../common/classes/FpeControl.h"
#include "../common/classes/VaryStr.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/BoolNodes.h"
#include "../dsql/StmtNodes.h"
#include "../dsql/node.h"
#include "../jrd/align.h"
#include "../jrd/blr.h"
#include "../common/quad.h"
#include "../jrd/tra.h"
#include "../jrd/Function.h"
#include "../jrd/SysFunction.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/Optimizer.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/dpm_proto.h"
#include "../common/dsc_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
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

namespace Jrd {


static const SINT64 MAX_INT64_LIMIT = MAX_SINT64 / 10;
static const SINT64 MIN_INT64_LIMIT = MIN_SINT64 / 10;
static const SINT64 SECONDS_PER_DAY = 24 * 60 * 60;
static const SINT64 ISC_TICKS_PER_DAY = SECONDS_PER_DAY * ISC_TIME_SECONDS_PRECISION;
static const SCHAR DIALECT_3_TIMESTAMP_SCALE = -9;
static const SCHAR DIALECT_1_TIMESTAMP_SCALE = 0;

static bool couldBeDate(const dsc desc);
static SINT64 getDayFraction(const dsc* d);
static SINT64 getTimeStampToIscTicks(const dsc* d);
static bool isDateAndTime(const dsc& d1, const dsc& d2);


//--------------------


void NodeRef::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	internalPass2(tdbb, csb);

	ExprNode* node = getExpr();

	// Bind values of invariant nodes to top-level RSE (if present)
	if (node && (node->nodFlags & ExprNode::FLAG_INVARIANT))
	{
		if (csb->csb_current_nodes.hasData())
		{
			RseOrExprNode& topRseNode = csb->csb_current_nodes[0];
			fb_assert(topRseNode.rseNode);

			if (!topRseNode.rseNode->rse_invariants)
			{
				topRseNode.rseNode->rse_invariants =
					FB_NEW(*tdbb->getDefaultPool()) VarInvariantArray(*tdbb->getDefaultPool());
			}

			topRseNode.rseNode->rse_invariants->add(node->impureOffset);
		}
	}
}


//--------------------


ExprNode* ExprNode::fromLegacy(const dsql_nod* node)
{
	return node && node->nod_type == Dsql::nod_class_exprnode ?
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

bool ExprNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other) /*const*/
{
	if (other->type != type)
		return false;

	size_t count = jrdChildNodes.getCount();
	if (other->jrdChildNodes.getCount() != count)
		return false;

	NodeRef** j = other->jrdChildNodes.begin();

	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i, ++j)
	{
		if (!**i && !**j)
			continue;

		if (!**i || !**j || !(*i)->getExpr()->sameAs(tdbb, csb, (*j)->getExpr()))
			return false;
	}

	return true;
}

bool ExprNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i && !(*i)->getExpr()->computable(csb, stream, allowOnlyCurrentStream))
			return false;
	}

	return true;
}

void ExprNode::findDependentFromStreams(const OptimizerRetrieval* optRet, SortedStreamList* streamList)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i)
			(*i)->getExpr()->findDependentFromStreams(optRet, streamList);
	}
}

ExprNode* ExprNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i)
			(*i)->pass1(tdbb, csb);
	}

	return this;
}

ExprNode* ExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i)
			(*i)->pass2(tdbb, csb);
	}

	return this;
}

// Check if expression could return NULL or expression can turn NULL into a true/false.
bool ExprNode::jrdPossibleUnknownFinder()
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i && (*i)->getExpr()->jrdPossibleUnknownFinder())
			return true;
	}

	return false;
}

// Search if somewhere in the expression the given stream is used.
bool ExprNode::jrdStreamFinder(USHORT findStream)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i && (*i)->getExpr()->jrdStreamFinder(findStream))
			return true;
	}

	return false;
}

// Return all streams referenced by the expression.
void ExprNode::jrdStreamsCollector(SortedStreamList& streamList)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i)
			(*i)->getExpr()->jrdStreamsCollector(streamList);
	}
}

// Verify if this node is allowed in an unmapped boolean.
bool ExprNode::jrdUnmappableNode(const MapNode* mapNode, UCHAR shellStream)
{
	for (NodeRef** i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
	{
		if (**i && !(*i)->getExpr()->jrdUnmappableNode(mapNode, shellStream))
			return false;
	}

	return true;
}


//--------------------


static RegisterNode<ArithmeticNode> regArithmeticNodeAdd(blr_add);
static RegisterNode<ArithmeticNode> regArithmeticNodeSubtract(blr_subtract);
static RegisterNode<ArithmeticNode> regArithmeticNodeMultiply(blr_multiply);
static RegisterNode<ArithmeticNode> regArithmeticNodeDivide(blr_divide);

ArithmeticNode::ArithmeticNode(MemoryPool& pool, UCHAR aBlrOp, bool aDialect1,
			dsql_nod* aArg1, dsql_nod* aArg2)
	: TypedNode<ValueExprNode, ExprNode::TYPE_ARITHMETIC>(pool),
	  blrOp(aBlrOp),
	  dialect1(aDialect1),
	  label(pool),
	  dsqlArg1(aArg1),
	  dsqlArg2(aArg2),
	  arg1(NULL),
	  arg2(NULL)
{
	switch (blrOp)
	{
		case blr_add:
			dsqlCompatDialectVerb = "add";
			break;

		case blr_subtract:
			dsqlCompatDialectVerb = "subtract";
			break;

		case blr_multiply:
			dsqlCompatDialectVerb = "multiply";
			break;

		case blr_divide:
			dsqlCompatDialectVerb = "divide";
			break;

		default:
			fb_assert(false);
	}

	label = dsqlCompatDialectVerb;
	label.upper();

	addChildNode(dsqlArg1, arg1);
	addChildNode(dsqlArg2, arg2);
}

DmlNode* ArithmeticNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	ArithmeticNode* node = FB_NEW(pool) ArithmeticNode(
		pool, blrOp, (csb->csb_g_flags & csb_blr_version4));
	node->arg1 = PAR_parse_value(tdbb, csb);
	node->arg2 = PAR_parse_value(tdbb, csb);
	return node;
}

void ArithmeticNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("ArithmeticNode %s (%d)", label.c_str(), (dialect1 ? 1 : 3));
	ExprNode::print(text, nodes);
}

void ArithmeticNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = label;
}

bool ArithmeticNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg1, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlArg2, node, forceVarChar);
}

void ArithmeticNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

void ArithmeticNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1, desc2;

	MAKE_desc(dsqlScratch, &desc1, dsqlArg1);
	MAKE_desc(dsqlScratch, &desc2, dsqlArg2);

	if (desc1.isNull())
	{
		desc1 = desc2;
		desc1.setNull();
	}

	if (desc2.isNull())
	{
		desc2 = desc1;
		desc2.setNull();
	}

	if (ExprNode::is<NullNode>(dsqlArg1) && ExprNode::is<NullNode>(dsqlArg2))
	{
		// NULL + NULL = NULL of INT
		desc->makeLong(0);
		desc->setNullable(true);
	}
	else if (dialect1)
		makeDialect1(desc, desc1, desc2);
	else
		makeDialect3(desc, desc1, desc2);
}

void ArithmeticNode::makeDialect1(dsc* desc, dsc& desc1, dsc& desc2)
{
	USHORT dtype, dtype1, dtype2;

	switch (blrOp)
	{
		case blr_add:
		case blr_subtract:
			dtype1 = desc1.dsc_dtype;
			if (dtype_int64 == dtype1 || DTYPE_IS_TEXT(dtype1))
				dtype1 = dtype_double;

			dtype2 = desc2.dsc_dtype;
			if (dtype_int64 == dtype2 || DTYPE_IS_TEXT(dtype2))
				dtype2 = dtype_double;

			dtype = MAX(dtype1, dtype2);

			if (DTYPE_IS_BLOB(dtype))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;

			switch (dtype)
			{
				case dtype_sql_time:
				case dtype_sql_date:
					// CVC: I don't see how this case can happen since dialect 1 doesn't accept
					// DATE or TIME
					// Forbid <date/time> +- <string>
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
					{
						ERRD_post(Arg::Gds(isc_expression_eval_err) <<
									Arg::Gds(isc_dsql_nodateortime_pm_string));
					}
					// fall into

				case dtype_timestamp:

					// Allow <timestamp> +- <string> (historical)
					if (couldBeDate(desc1) && couldBeDate(desc2))
					{
						if (blrOp == blr_subtract)
						{
							// <any date> - <any date>

							// Legal permutations are:
							// <timestamp> - <timestamp>
							// <timestamp> - <date>
							// <date> - <date>
							// <date> - <timestamp>
							// <time> - <time>
							// <timestamp> - <string>
							// <string> - <timestamp>
							// <string> - <string>

							if (DTYPE_IS_TEXT(desc1.dsc_dtype))
								dtype = dtype_timestamp;
							else if (DTYPE_IS_TEXT(desc2.dsc_dtype))
								dtype = dtype_timestamp;
							else if (desc1.dsc_dtype == desc2.dsc_dtype)
								dtype = desc1.dsc_dtype;
							else if (desc1.dsc_dtype == dtype_timestamp &&
								desc2.dsc_dtype == dtype_sql_date)
							{
								dtype = dtype_timestamp;
							}
							else if (desc2.dsc_dtype == dtype_timestamp &&
								desc1.dsc_dtype == dtype_sql_date)
							{
								dtype = dtype_timestamp;
							}
							else
							{
								ERRD_post(Arg::Gds(isc_expression_eval_err) <<
										  Arg::Gds(isc_dsql_invalid_datetime_subtract));
							}

							if (dtype == dtype_sql_date)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = sizeof(SLONG);
								desc->dsc_scale = 0;
							}
							else if (dtype == dtype_sql_time)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = sizeof(SLONG);
								desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
								desc->dsc_sub_type = dsc_num_type_numeric;
							}
							else
							{
								fb_assert(dtype == dtype_timestamp);
								desc->dsc_dtype = dtype_double;
								desc->dsc_length = sizeof(double);
								desc->dsc_scale = 0;
							}
						}
						else if (isDateAndTime(desc1, desc2))
						{
							// <date> + <time>
							// <time> + <date>
							desc->dsc_dtype = dtype_timestamp;
							desc->dsc_length = type_lengths[dtype_timestamp];
							desc->dsc_scale = 0;
						}
						else
						{
							// <date> + <date>
							// <time> + <time>
							// CVC: Hard to see it, since we are in dialect 1.
							ERRD_post(Arg::Gds(isc_expression_eval_err) <<
									  Arg::Gds(isc_dsql_invalid_dateortime_add));
						}
					}
					else if (DTYPE_IS_DATE(desc1.dsc_dtype) || blrOp == blr_add)
					{
						// <date> +/- <non-date>
						// <non-date> + <date>
						desc->dsc_dtype = desc1.dsc_dtype;
						if (!DTYPE_IS_DATE(desc->dsc_dtype))
							desc->dsc_dtype = desc2.dsc_dtype;
						fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
					}
					else
					{
						// <non-date> - <date>
						fb_assert(blrOp == blr_subtract);
						ERRD_post(Arg::Gds(isc_expression_eval_err) <<
								  Arg::Gds(isc_dsql_invalid_type_minus_date));
					}
					break;

				case dtype_varying:
				case dtype_cstring:
				case dtype_text:
				case dtype_double:
				case dtype_real:
					desc->dsc_dtype = dtype_double;
					desc->dsc_sub_type = 0;
					desc->dsc_scale = 0;
					desc->dsc_length = sizeof(double);
					break;

				default:
					desc->dsc_dtype = dtype_long;
					desc->dsc_sub_type = 0;
					desc->dsc_length = sizeof(SLONG);
					desc->dsc_scale = MIN(NUMERIC_SCALE(desc1), NUMERIC_SCALE(desc2));
					break;
			}

			break;

		case blr_multiply:
			// Arrays and blobs can never partipate in multiplication
			if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			dtype = DSC_multiply_blr4_result[desc1.dsc_dtype][desc2.dsc_dtype];
			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;

			switch (dtype)
			{
				case dtype_double:
					desc->dsc_dtype = dtype_double;
					desc->dsc_sub_type = 0;
					desc->dsc_scale = 0;
					desc->dsc_length = sizeof(double);
					break;

				case dtype_long:
					desc->dsc_dtype = dtype_long;
					desc->dsc_sub_type = 0;
					desc->dsc_length = sizeof(SLONG);
					desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					break;

				default:
					ERRD_post(Arg::Gds(isc_expression_eval_err) <<
							  Arg::Gds(isc_dsql_invalid_type_multip_dial1));
			}

			break;

		case blr_divide:
			// Arrays and blobs can never partipate in division
			if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			dtype1 = desc1.dsc_dtype;
			if (dtype_int64 == dtype1 || DTYPE_IS_TEXT(dtype1))
				dtype1 = dtype_double;

			dtype2 = desc2.dsc_dtype;
			if (dtype_int64 == dtype2 || DTYPE_IS_TEXT(dtype2))
				dtype2 = dtype_double;

			dtype = MAX(dtype1, dtype2);

			if (!DTYPE_IS_NUMERIC(dtype))
			{
				ERRD_post(Arg::Gds(isc_expression_eval_err) <<
						  Arg::Gds(isc_dsql_mustuse_numeric_div_dial1));
			}

			desc->dsc_dtype = dtype_double;
			desc->dsc_length = sizeof(double);
			desc->dsc_scale = 0;
			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
			break;
	}
}

void ArithmeticNode::makeDialect3(dsc* desc, dsc& desc1, dsc& desc2)
{
	USHORT dtype, dtype1, dtype2;

	switch (blrOp)
	{
		case blr_add:
		case blr_subtract:
			dtype1 = desc1.dsc_dtype;
			dtype2 = desc2.dsc_dtype;

			// Arrays and blobs can never partipate in addition/subtraction
			if (DTYPE_IS_BLOB(dtype1) || DTYPE_IS_BLOB(dtype2))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			// In Dialect 2 or 3, strings can never partipate in addition / sub
			// (use a specific cast instead)
			if (DTYPE_IS_TEXT(dtype1) || DTYPE_IS_TEXT(dtype2))
			{
				ERRD_post(Arg::Gds(isc_expression_eval_err) <<
						  Arg::Gds(isc_dsql_nostring_addsub_dial3));
			}

			// Determine the TYPE of arithmetic to perform, store it
			// in dtype.  Note:  this is different from the result of
			// the operation, as <timestamp>-<timestamp> uses
			// <timestamp> arithmetic, but returns a <double>
			if (DTYPE_IS_EXACT(dtype1) && DTYPE_IS_EXACT(dtype2))
				dtype = dtype_int64;
			else if (DTYPE_IS_NUMERIC(dtype1) && DTYPE_IS_NUMERIC(dtype2))
			{
				fb_assert(DTYPE_IS_APPROX(dtype1) || DTYPE_IS_APPROX(dtype2));
				dtype = dtype_double;
			}
			else
			{
				// mixed numeric and non-numeric:

				// The MAX(dtype) rule doesn't apply with dtype_int64

				if (dtype_int64 == dtype1)
					dtype1 = dtype_double;
				if (dtype_int64 == dtype2)
					dtype2 = dtype_double;

				dtype = MAX(dtype1, dtype2);
			}

			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;

			switch (dtype)
			{
				case dtype_sql_time:
				case dtype_sql_date:
				case dtype_timestamp:
					if ((DTYPE_IS_DATE(dtype1) || dtype1 == dtype_unknown) &&
						(DTYPE_IS_DATE(dtype2) || dtype2 == dtype_unknown))
					{
						if (blrOp == blr_subtract)
						{
							// <any date> - <any date>
							// Legal permutations are:
							// <timestamp> - <timestamp>
							// <timestamp> - <date>
							// <date> - <date>
							// <date> - <timestamp>
							// <time> - <time>

							if (dtype1 == dtype2)
								dtype = dtype1;
							else if (dtype1 == dtype_timestamp && dtype2 == dtype_sql_date)
								dtype = dtype_timestamp;
							else if (dtype2 == dtype_timestamp && dtype1 == dtype_sql_date)
								dtype = dtype_timestamp;
							else
							{
								ERRD_post(Arg::Gds(isc_expression_eval_err) <<
										  Arg::Gds(isc_dsql_invalid_datetime_subtract));
							}

							if (dtype == dtype_sql_date)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = sizeof(SLONG);
								desc->dsc_scale = 0;
							}
							else if (dtype == dtype_sql_time)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = sizeof(SLONG);
								desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
								desc->dsc_sub_type = dsc_num_type_numeric;
							}
							else
							{
								fb_assert(dtype == dtype_timestamp);
								desc->dsc_dtype = dtype_int64;
								desc->dsc_length = sizeof(SINT64);
								desc->dsc_scale = -9;
								desc->dsc_sub_type = dsc_num_type_numeric;
							}
						}
						else if (isDateAndTime(desc1, desc2))
						{
							// <date> + <time>
							// <time> + <date>
							desc->dsc_dtype = dtype_timestamp;
							desc->dsc_length = type_lengths[dtype_timestamp];
							desc->dsc_scale = 0;
						}
						else
						{
							// <date> + <date>
							// <time> + <time>
							ERRD_post(Arg::Gds(isc_expression_eval_err) <<
									  Arg::Gds(isc_dsql_invalid_dateortime_add));
						}
					}
					else if (DTYPE_IS_DATE(desc1.dsc_dtype) || blrOp == blr_add)
					{
						// <date> +/- <non-date>
						// <non-date> + <date>
						desc->dsc_dtype = desc1.dsc_dtype;
						if (!DTYPE_IS_DATE(desc->dsc_dtype))
							desc->dsc_dtype = desc2.dsc_dtype;
						fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
					}
					else
					{
						// <non-date> - <date>
						fb_assert(blrOp == blr_subtract);
						ERRD_post(Arg::Gds(isc_expression_eval_err) <<
								  Arg::Gds(isc_dsql_invalid_type_minus_date));
					}
					break;

				case dtype_varying:
				case dtype_cstring:
				case dtype_text:
				case dtype_double:
				case dtype_real:
					desc->dsc_dtype = dtype_double;
					desc->dsc_sub_type = 0;
					desc->dsc_scale = 0;
					desc->dsc_length = sizeof(double);
					break;

				case dtype_short:
				case dtype_long:
				case dtype_int64:
					desc->dsc_dtype = dtype_int64;
					desc->dsc_sub_type = 0;
					desc->dsc_length = sizeof(SINT64);

					// The result type is int64 because both operands are
					// exact numeric: hence we don't need the NUMERIC_SCALE
					// macro here.
					fb_assert(desc1.dsc_dtype == dtype_unknown || DTYPE_IS_EXACT(desc1.dsc_dtype));
					fb_assert(desc2.dsc_dtype == dtype_unknown || DTYPE_IS_EXACT(desc2.dsc_dtype));

					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					break;

				default:
					// a type which cannot participate in an add or subtract
					ERRD_post(Arg::Gds(isc_expression_eval_err) <<
							  Arg::Gds(isc_dsql_invalid_type_addsub_dial3));
			}

			break;

		case blr_multiply:
			// In Dialect 2 or 3, strings can never partipate in multiplication
			// (use a specific cast instead)
			if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_expression_eval_err) <<
						  Arg::Gds(isc_dsql_nostring_multip_dial3));
			}

			// Arrays and blobs can never partipate in multiplication
			if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			dtype = DSC_multiply_result[desc1.dsc_dtype][desc2.dsc_dtype];
			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;

			switch (dtype)
			{
				case dtype_double:
					desc->dsc_dtype = dtype_double;
					desc->dsc_sub_type = 0;
					desc->dsc_scale = 0;
					desc->dsc_length = sizeof(double);
					break;

				case dtype_int64:
					desc->dsc_dtype = dtype_int64;
					desc->dsc_sub_type = 0;
					desc->dsc_length = sizeof(SINT64);
					desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					break;

				default:
					ERRD_post(Arg::Gds(isc_expression_eval_err) <<
							  Arg::Gds(isc_dsql_invalid_type_multip_dial3));
			}

			break;

		case blr_divide:
			// In Dialect 2 or 3, strings can never partipate in division
			// (use a specific cast instead)
			if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_expression_eval_err) <<
						  Arg::Gds(isc_dsql_nostring_div_dial3));
			}

			// Arrays and blobs can never partipate in division
			if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_no_blob_array));
			}

			dtype = DSC_multiply_result[desc1.dsc_dtype][desc2.dsc_dtype];
			desc->dsc_dtype = static_cast<UCHAR>(dtype);
			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;

			switch (dtype)
			{
				case dtype_int64:
					desc->dsc_length = sizeof(SINT64);
					desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					break;

				case dtype_double:
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					break;

				default:
					ERRD_post(Arg::Gds(isc_expression_eval_err) <<
							  Arg::Gds(isc_dsql_invalid_type_div_dial3));
			}

			break;
	}
}

void ArithmeticNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	dsc desc1, desc2;

	arg1->getDesc(tdbb, csb, &desc1);
	arg2->getDesc(tdbb, csb, &desc2);

	if (desc1.isNull())
	{
		desc1 = desc2;
		desc1.setNull();
	}

	if (desc2.isNull())
	{
		desc2 = desc1;
		desc2.setNull();
	}

	if (dialect1)
		getDescDialect1(tdbb, desc, desc1, desc2);
	else
		getDescDialect3(tdbb, desc, desc1, desc2);
}

void ArithmeticNode::getDescDialect1(thread_db* /*tdbb*/, dsc* desc, dsc& desc1, dsc& desc2)
{
	USHORT dtype;

	switch (blrOp)
	{
		case blr_add:
		case blr_subtract:
		{
			/* 92/05/29 DAVES - don't understand why this is done for ONLY
				dtype_text (eg: not dtype_cstring or dtype_varying) Doesn't
				appear to hurt.

				94/04/04 DAVES - NOW I understand it!  QLI will pass floating
				point values to the engine as text.  All other numeric constants
				it turns into either integers or longs (with scale). */

			USHORT dtype1 = desc1.dsc_dtype;
			if (dtype_int64 == dtype1)
				dtype1 = dtype_double;

			USHORT dtype2 = desc2.dsc_dtype;
			if (dtype_int64 == dtype2)
				dtype2 = dtype_double;

			if (dtype1 == dtype_text || dtype2 == dtype_text)
				dtype = MAX(MAX(dtype1, dtype2), (UCHAR) DEFAULT_DOUBLE);
			else
				dtype = MAX(dtype1, dtype2);

			switch (dtype)
			{
				case dtype_short:
					desc->dsc_dtype = dtype_long;
					desc->dsc_length = sizeof(SLONG);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);

					nodScale = desc->dsc_scale;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_sql_date:
				case dtype_sql_time:
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						ERR_post(Arg::Gds(isc_expression_eval_err));
					// fall into

				case dtype_timestamp:
					nodFlags |= FLAG_DATE;

					fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_DATE(desc2.dsc_dtype));

					if (couldBeDate(desc1) && couldBeDate(desc2))
					{
						if (blrOp == blr_subtract)
						{
							// <any date> - <any date>

							/* Legal permutations are:
								<timestamp> - <timestamp>
								<timestamp> - <date>
								<date> - <date>
								<date> - <timestamp>
								<time> - <time>
								<timestamp> - <string>
								<string> - <timestamp>
								<string> - <string>   */

							if (DTYPE_IS_TEXT(dtype1))
								dtype = dtype_timestamp;
							else if (DTYPE_IS_TEXT(dtype2))
								dtype = dtype_timestamp;
							else if (dtype1 == dtype2)
								dtype = dtype1;
							else if (dtype1 == dtype_timestamp && dtype2 == dtype_sql_date)
								dtype = dtype_timestamp;
							else if (dtype2 == dtype_timestamp && dtype1 == dtype_sql_date)
								dtype = dtype_timestamp;
							else
								ERR_post(Arg::Gds(isc_expression_eval_err));

							if (dtype == dtype_sql_date)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = 0;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
							else if (dtype == dtype_sql_time)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
							else
							{
								fb_assert(dtype == dtype_timestamp);
								desc->dsc_dtype = DEFAULT_DOUBLE;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = 0;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
						}
						else if (isDateAndTime(desc1, desc2))
						{
							// <date> + <time>
							// <time> + <date>
							desc->dsc_dtype = dtype_timestamp;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else
						{
							// <date> + <date>
							ERR_post(Arg::Gds(isc_expression_eval_err));
						}
					}
					else if (DTYPE_IS_DATE(desc1.dsc_dtype) || blrOp == blr_add)
					{
						// <date> +/- <non-date> || <non-date> + <date>
						desc->dsc_dtype = desc1.dsc_dtype;
						if (!DTYPE_IS_DATE(desc->dsc_dtype))
							desc->dsc_dtype = desc2.dsc_dtype;

						fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
					{
						// <non-date> - <date>
						ERR_post(Arg::Gds(isc_expression_eval_err));
					}
					return;

				case dtype_text:
				case dtype_cstring:
				case dtype_varying:
				case dtype_long:
				case dtype_real:
				case dtype_double:
					nodFlags |= FLAG_DOUBLE;
					desc->dsc_dtype = DEFAULT_DOUBLE;
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_unknown:
					desc->dsc_dtype = dtype_unknown;
					desc->dsc_length = 0;
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_quad:
					nodFlags |= FLAG_QUAD;
					desc->dsc_dtype = dtype_quad;
					desc->dsc_length = sizeof(SQUAD);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					nodScale = desc->dsc_scale;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
#ifdef NATIVE_QUAD
					return;
#endif
				default:
					fb_assert(false);
					// FALLINTO

				case dtype_blob:
				case dtype_array:
					break;
			}

			break;
		}

		case blr_multiply:
			dtype = DSC_multiply_blr4_result[desc1.dsc_dtype][desc2.dsc_dtype];

			switch (dtype)
			{
				case dtype_long:
					desc->dsc_dtype = dtype_long;
					desc->dsc_length = sizeof(SLONG);
					desc->dsc_scale = nodScale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_double:
					nodFlags |= FLAG_DOUBLE;
					desc->dsc_dtype = DEFAULT_DOUBLE;
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_unknown:
					desc->dsc_dtype = dtype_unknown;
					desc->dsc_length = 0;
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				default:
					fb_assert(false);
					// FALLINTO

				case DTYPE_CANNOT:
					// break to error reporting code
					break;
			}

			break;

		case blr_divide:
			// for compatibility with older versions of the product, we accept
			// text types for division in blr_version4 (dialect <= 1) only
			if (!(DTYPE_IS_NUMERIC(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc1.dsc_dtype)))
			{
				if (desc1.dsc_dtype != dtype_unknown)
					break;	// error, dtype not supported by arithmetic
			}

			if (!(DTYPE_IS_NUMERIC(desc2.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype)))
			{
				if (desc2.dsc_dtype != dtype_unknown)
					break;	// error, dtype not supported by arithmetic
			}

			desc->dsc_dtype = DEFAULT_DOUBLE;
			desc->dsc_length = sizeof(double);
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;
	}

	if (dtype == dtype_quad)
		IBERROR(224);	// msg 224 quad word arithmetic not supported

	ERR_post(Arg::Gds(isc_datype_notsup));	// data type not supported for arithmetic
}

void ArithmeticNode::getDescDialect3(thread_db* /*tdbb*/, dsc* desc, dsc& desc1, dsc& desc2)
{
	USHORT dtype;

	switch (blrOp)
	{
		case blr_add:
		case blr_subtract:
		{
			USHORT dtype1 = desc1.dsc_dtype;
			USHORT dtype2 = desc2.dsc_dtype;

			// In Dialect 2 or 3, strings can never partipate in addition / sub
			// (use a specific cast instead)
			if (DTYPE_IS_TEXT(dtype1) || DTYPE_IS_TEXT(dtype2))
				ERR_post(Arg::Gds(isc_expression_eval_err));

			// Because dtype_int64 > dtype_double, we cannot just use the MAX macro to set
			// the result dtype. The rule is that two exact numeric operands yield an int64
			// result, while an approximate numeric and anything yield a double result.

			if (DTYPE_IS_EXACT(desc1.dsc_dtype) && DTYPE_IS_EXACT(desc2.dsc_dtype))
				dtype = dtype_int64;
			else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype) && DTYPE_IS_NUMERIC(desc2.dsc_dtype))
				dtype = dtype_double;
			else
			{
				// mixed numeric and non-numeric:

				fb_assert(couldBeDate(desc1) || couldBeDate(desc2));

				// the MAX(dtype) rule doesn't apply with dtype_int64

				if (dtype_int64 == dtype1)
					dtype1 = dtype_double;

				if (dtype_int64 == dtype2)
					dtype2 = dtype_double;

				dtype = MAX(dtype1, dtype2);
			}

			switch (dtype)
			{
				case dtype_timestamp:
				case dtype_sql_date:
				case dtype_sql_time:
					nodFlags |= FLAG_DATE;

					fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_DATE(desc2.dsc_dtype));

					if ((DTYPE_IS_DATE(dtype1) || dtype1 == dtype_unknown) &&
						(DTYPE_IS_DATE(dtype2) || dtype2 == dtype_unknown))
					{
						if (blrOp == blr_subtract)
						{
							// <any date> - <any date>

							/* Legal permutations are:
							   <timestamp> - <timestamp>
							   <timestamp> - <date>
							   <date> - <date>
							   <date> - <timestamp>
							   <time> - <time> */

							if (dtype1 == dtype_unknown)
								dtype1 = dtype2;
							else if (dtype2 == dtype_unknown)
								dtype2 = dtype1;

							if (dtype1 == dtype2)
								dtype = dtype1;
							else if ((dtype1 == dtype_timestamp) && (dtype2 == dtype_sql_date))
								dtype = dtype_timestamp;
							else if ((dtype2 == dtype_timestamp) && (dtype1 == dtype_sql_date))
								dtype = dtype_timestamp;
							else
								ERR_post(Arg::Gds(isc_expression_eval_err));

							if (dtype == dtype_sql_date)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = 0;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
							else if (dtype == dtype_sql_time)
							{
								desc->dsc_dtype = dtype_long;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
							else
							{
								fb_assert(dtype == dtype_timestamp || dtype == dtype_unknown);
								desc->dsc_dtype = DEFAULT_DOUBLE;
								desc->dsc_length = type_lengths[desc->dsc_dtype];
								desc->dsc_scale = 0;
								desc->dsc_sub_type = 0;
								desc->dsc_flags = 0;
							}
						}
						else if (isDateAndTime(desc1, desc2))
						{
							// <date> + <time>
							// <time> + <date>
							desc->dsc_dtype = dtype_timestamp;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else
						{
							// <date> + <date>
							ERR_post(Arg::Gds(isc_expression_eval_err));
						}
					}
					else if (DTYPE_IS_DATE(desc1.dsc_dtype) || blrOp == blr_add)
					{
						// <date> +/- <non-date> || <non-date> + <date>
						desc->dsc_dtype = desc1.dsc_dtype;
						if (!DTYPE_IS_DATE(desc->dsc_dtype))
							desc->dsc_dtype = desc2.dsc_dtype;
						fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
					{
						// <non-date> - <date>
						ERR_post(Arg::Gds(isc_expression_eval_err));
					}
					return;

				case dtype_text:
				case dtype_cstring:
				case dtype_varying:
				case dtype_real:
				case dtype_double:
					nodFlags |= FLAG_DOUBLE;
					desc->dsc_dtype = DEFAULT_DOUBLE;
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_short:
				case dtype_long:
				case dtype_int64:
					desc->dsc_dtype = dtype_int64;
					desc->dsc_length = sizeof(SINT64);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					nodScale = desc->dsc_scale;
					desc->dsc_sub_type = MAX(desc1.dsc_sub_type, desc2.dsc_sub_type);
					desc->dsc_flags = 0;
					return;

				case dtype_unknown:
					desc->dsc_dtype = dtype_unknown;
					desc->dsc_length = 0;
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_quad:
					nodFlags |= FLAG_QUAD;
					desc->dsc_dtype = dtype_quad;
					desc->dsc_length = sizeof(SQUAD);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					nodScale = desc->dsc_scale;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
#ifdef NATIVE_QUAD
					return;
#endif

				default:
					fb_assert(false);
					// FALLINTO

				case dtype_blob:
				case dtype_array:
					break;
			}

			break;
		}

		case blr_multiply:
		case blr_divide:
			dtype = DSC_multiply_result[desc1.dsc_dtype][desc2.dsc_dtype];

			switch (dtype)
			{
				case dtype_double:
					nodFlags |= FLAG_DOUBLE;
					desc->dsc_dtype = DEFAULT_DOUBLE;
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_int64:
					desc->dsc_dtype = dtype_int64;
					desc->dsc_length = sizeof(SINT64);
					desc->dsc_scale = nodScale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					desc->dsc_sub_type = MAX(desc1.dsc_sub_type, desc2.dsc_sub_type);
					desc->dsc_flags = 0;
					return;

				case dtype_unknown:
					desc->dsc_dtype = dtype_unknown;
					desc->dsc_length = 0;
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				default:
					fb_assert(false);
					// FALLINTO

				case DTYPE_CANNOT:
					// break to error reporting code
					break;
			}

			break;
	}

	if (dtype == dtype_quad)
		IBERROR(224);	// msg 224 quad word arithmetic not supported

	ERR_post(Arg::Gds(isc_datype_notsup));	// data type not supported for arithmetic
}

ValueExprNode* ArithmeticNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ArithmeticNode* node = FB_NEW(*tdbb->getDefaultPool()) ArithmeticNode(*tdbb->getDefaultPool(),
		blrOp, dialect1);
	node->nodScale = nodScale;
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);
	return node;
}

bool ArithmeticNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const ArithmeticNode* o = other->as<ArithmeticNode>();
	fb_assert(o);

	return dialect1 == o->dialect1 && blrOp == o->blrOp;
}

bool ArithmeticNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	ArithmeticNode* otherNode = other->as<ArithmeticNode>();

	if (!otherNode || blrOp != otherNode->blrOp || dialect1 != otherNode->dialect1)
		return false;

	if (arg1->sameAs(tdbb, csb, otherNode->arg1) && arg2->sameAs(tdbb, csb, otherNode->arg2))
		return true;

	if (blrOp == blr_add || blrOp == blr_multiply)
	{
		// A + B is equivalent to B + A, ditto for A * B and B * A.
		// Note: If one expression is A + B + C, but the other is B + C + A we won't
		// necessarily match them.
		if (arg1->sameAs(tdbb, csb, otherNode->arg2) && arg2->sameAs(tdbb, csb, otherNode->arg1))
			return true;
	}

	return false;
}

ValueExprNode* ArithmeticNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* ArithmeticNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);

	request->req_flags &= ~req_null;

	// Evaluate arguments.  If either is null, result is null, but in
	// any case, evaluate both, since some expressions may later depend
	// on mappings which are developed here

	const dsc* desc1 = EVL_expr(tdbb, request, arg1);
	const ULONG flags = request->req_flags;
	request->req_flags &= ~req_null;

	const dsc* desc2 = EVL_expr(tdbb, request, arg2);

	// restore saved NULL state

	if (flags & req_null)
		request->req_flags |= req_null;

	if (request->req_flags & req_null)
		return NULL;

	EVL_make_value(tdbb, desc1, impure);

	if (dialect1)	// dialect-1 semantics
	{
		switch (blrOp)
		{
			case blr_add:
			case blr_subtract:
				return add(desc2, impure, this, blrOp);

			case blr_divide:
			{
				const double divisor = MOV_get_double(desc2);

				if (divisor == 0)
				{
					ERR_post(Arg::Gds(isc_arith_except) <<
							 Arg::Gds(isc_exception_float_divide_by_zero));
				}

				impure->vlu_misc.vlu_double = MOV_get_double(desc1) / divisor;

				if (isinf(impure->vlu_misc.vlu_double))
				{
					ERR_post(Arg::Gds(isc_arith_except) <<
							 Arg::Gds(isc_exception_float_overflow));
				}

				impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
				impure->vlu_desc.dsc_length = sizeof(double);
				impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc;

				return &impure->vlu_desc;
			}

			case blr_multiply:
				return multiply(desc2, impure);
		}
	}
	else	// with dialect-3 semantics
	{
		switch (blrOp)
		{
			case blr_add:
			case blr_subtract:
				return add2(desc2, impure, this, blrOp);

			case blr_multiply:
				return multiply2(desc2, impure);

			case blr_divide:
				return divide2(desc2, impure);
		}
	}

	BUGCHECK(232);	// msg 232 EVL_expr: invalid operation
	return NULL;
}

// Add (or subtract) the contents of a descriptor to value block, with dialect-1 semantics.
 // This function can be removed when dialect-3 becomes the lowest supported dialect. (Version 7.0?)
dsc* ArithmeticNode::add(const dsc* desc, impure_value* value, const ValueExprNode* node, UCHAR blrOp)
{
	const ArithmeticNode* arithmeticNode = node->as<ArithmeticNode>();
	const SubQueryNode* subQueryNode = node->as<SubQueryNode>();

	fb_assert(
		(arithmeticNode && arithmeticNode->dialect1 &&
			(arithmeticNode->blrOp == blr_add || arithmeticNode->blrOp == blr_subtract)) ||
		node->is<AggNode>() ||
		(subQueryNode && (subQueryNode->blrOp == blr_total || subQueryNode->blrOp == blr_average)));

	dsc* const result = &value->vlu_desc;

	// Handle date arithmetic

	if (node->nodFlags & FLAG_DATE)
	{
		fb_assert(arithmeticNode);
		return arithmeticNode->addDateTime(desc, value);
	}

	// Handle floating arithmetic

	if (node->nodFlags & FLAG_DOUBLE)
	{
		const double d1 = MOV_get_double(desc);
		const double d2 = MOV_get_double(&value->vlu_desc);

		value->vlu_misc.vlu_double = (blrOp == blr_subtract) ? d2 - d1 : d1 + d2;

		if (isinf(value->vlu_misc.vlu_double))
			ERR_post(Arg::Gds(isc_arith_except) << Arg::Gds(isc_exception_float_overflow));

		result->dsc_dtype = DEFAULT_DOUBLE;
		result->dsc_length = sizeof(double);
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;

		return result;
	}

	// Handle (oh, ugh) quad arithmetic

	if (node->nodFlags & FLAG_QUAD)
	{
		const SQUAD q1 = MOV_get_quad(desc, node->nodScale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, node->nodScale);

		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nodScale;
		value->vlu_misc.vlu_quad = (blrOp == blr_subtract) ?
			QUAD_SUBTRACT(q2, q1, ERR_post) : QUAD_ADD(q1, q2, ERR_post);
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return result;
	}

	// Everything else defaults to longword

	// CVC: Maybe we should upgrade the sum to double if it doesn't fit?
	// This is what was done for multiplicaton in dialect 1.

	const SLONG l1 = MOV_get_long(desc, node->nodScale);
	const SINT64 l2 = MOV_get_long(&value->vlu_desc, node->nodScale);
	SINT64 rc = (blrOp == blr_subtract) ? l2 - l1 : l2 + l1;

	if (rc < MIN_SLONG || rc > MAX_SLONG)
		ERR_post(Arg::Gds(isc_exception_integer_overflow));

	value->make_long(rc, node->nodScale);

	return result;
}

// Add (or subtract) the contents of a descriptor to value block, with dialect-3 semantics, as in
// the blr_add, blr_subtract, and blr_agg_total verbs following a blr_version5.
dsc* ArithmeticNode::add2(const dsc* desc, impure_value* value, const ValueExprNode* node, UCHAR blrOp)
{
	const ArithmeticNode* arithmeticNode = node->as<ArithmeticNode>();

	fb_assert(
		(arithmeticNode && !arithmeticNode->dialect1 &&
			(arithmeticNode->blrOp == blr_add || arithmeticNode->blrOp == blr_subtract)) ||
		node->is<AggNode>());

	dsc* result = &value->vlu_desc;

	// Handle date arithmetic

	if (node->nodFlags & FLAG_DATE)
	{
		fb_assert(arithmeticNode);
		return arithmeticNode->addDateTime(desc, value);
	}

	// Handle floating arithmetic

	if (node->nodFlags & FLAG_DOUBLE)
	{
		const double d1 = MOV_get_double(desc);
		const double d2 = MOV_get_double(&value->vlu_desc);

		value->vlu_misc.vlu_double = (blrOp == blr_subtract) ? d2 - d1 : d1 + d2;

		if (isinf(value->vlu_misc.vlu_double))
			ERR_post(Arg::Gds(isc_arith_except) << Arg::Gds(isc_exception_float_overflow));

		result->dsc_dtype = DEFAULT_DOUBLE;
		result->dsc_length = sizeof(double);
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;

		return result;
	}

	// Handle (oh, ugh) quad arithmetic

	if (node->nodFlags & FLAG_QUAD)
	{
		const SQUAD q1 = MOV_get_quad(desc, node->nodScale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, node->nodScale);

		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nodScale;
		value->vlu_misc.vlu_quad = (blrOp == blr_subtract) ?
			QUAD_SUBTRACT(q2, q1, ERR_post) : QUAD_ADD(q1, q2, ERR_post);
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return result;
	}

	// Everything else defaults to int64

	SINT64 i1 = MOV_get_int64(desc, node->nodScale);
	const SINT64 i2 = MOV_get_int64(&value->vlu_desc, node->nodScale);

	result->dsc_dtype = dtype_int64;
	result->dsc_length = sizeof(SINT64);
	result->dsc_scale = node->nodScale;
	value->vlu_misc.vlu_int64 = (blrOp == blr_subtract) ? i2 - i1 : i1 + i2;
	result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;
	result->dsc_sub_type = MAX(desc->dsc_sub_type, value->vlu_desc.dsc_sub_type);

	/* If the operands of an addition have the same sign, and their sum has
	the opposite sign, then overflow occurred.  If the two addends have
	opposite signs, then the result will lie between the two addends, and
	overflow cannot occur.
	If this is a subtraction, note that we invert the sign bit, rather than
	negating the argument, so that subtraction of MIN_SINT64, which is
	unchanged by negation, will be correctly treated like the addition of
	a positive number for the purposes of this test.

	Test cases for a Gedankenexperiment, considering the sign bits of the
	operands and result after the inversion below:                L  Rt  Sum

		MIN_SINT64 - MIN_SINT64 ==          0, with no overflow  1   0   0
	   -MAX_SINT64 - MIN_SINT64 ==          1, with no overflow  1   0   0
		1          - MIN_SINT64 == overflow                      0   0   1
	   -1          - MIN_SINT64 == MAX_SINT64, no overflow       1   0   0
	*/

	if (blrOp == blr_subtract)
		i1 ^= MIN_SINT64;		// invert the sign bit

	if ((i1 ^ i2) >= 0 && (i1 ^ value->vlu_misc.vlu_int64) < 0)
		ERR_post(Arg::Gds(isc_exception_integer_overflow));

	return result;
}

// Multiply two numbers, with SQL dialect-1 semantics.
// This function can be removed when dialect-3 becomes the lowest supported dialect. (Version 7.0?)
dsc* ArithmeticNode::multiply(const dsc* desc, impure_value* value) const
{
	DEV_BLKCHK(node, type_nod);

	// Handle floating arithmetic

	if (nodFlags & FLAG_DOUBLE)
	{
		const double d1 = MOV_get_double(desc);
		const double d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = d1 * d2;

		if (isinf(value->vlu_misc.vlu_double))
		{
			ERR_post(Arg::Gds(isc_arith_except) <<
					 Arg::Gds(isc_exception_float_overflow));
		}

		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;

		return &value->vlu_desc;
	}

	// Handle (oh, ugh) quad arithmetic

	if (nodFlags & FLAG_QUAD)
	{
		const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
		const SQUAD q1 = MOV_get_quad(desc, nodScale - scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, scale);
		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = nodScale;
		value->vlu_misc.vlu_quad = QUAD_MULTIPLY(q1, q2, ERR_post);
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return &value->vlu_desc;
	}

	// Everything else defaults to longword

	/* CVC: With so many problems cropping with dialect 1 and multiplication,
			I decided to close this Pandora box by incurring in INT64 performance
			overhead (if noticeable) and try to get the best result. When I read it,
			this function didn't bother even to check for overflow! */

#define FIREBIRD_AVOID_DIALECT1_OVERFLOW
	// SLONG l1, l2;
	//{
	const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
	const SINT64 i1 = MOV_get_long(desc, nodScale - scale);
	const SINT64 i2 = MOV_get_long(&value->vlu_desc, scale);
	value->vlu_desc.dsc_dtype = dtype_long;
	value->vlu_desc.dsc_length = sizeof(SLONG);
	value->vlu_desc.dsc_scale = nodScale;
	const SINT64 rc = i1 * i2;
	if (rc < MIN_SLONG || rc > MAX_SLONG)
	{
#ifdef FIREBIRD_AVOID_DIALECT1_OVERFLOW
		value->vlu_misc.vlu_int64 = rc;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;
		value->vlu_desc.dsc_dtype = dtype_int64;
		value->vlu_desc.dsc_length = sizeof(SINT64);
		value->vlu_misc.vlu_double = MOV_get_double(&value->vlu_desc);
		/* This is the Borland solution instead of the five lines above.
		d1 = MOV_get_double (desc);
		d2 = MOV_get_double (&value->vlu_desc);
		value->vlu_misc.vlu_double = d1 * d2; */
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;
#else
		ERR_post(Arg::Gds(isc_exception_integer_overflow));
#endif
	}
	else
	{
		value->vlu_misc.vlu_long = (SLONG) rc; // l1 * l2;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_long;
	}
	//}

	return &value->vlu_desc;
}

// Multiply two numbers, with dialect-3 semantics, implementing blr_version5 ... blr_multiply.
dsc* ArithmeticNode::multiply2(const dsc* desc, impure_value* value) const
{
	DEV_BLKCHK(node, type_nod);

	// Handle floating arithmetic

	if (nodFlags & FLAG_DOUBLE)
	{
		const double d1 = MOV_get_double(desc);
		const double d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = d1 * d2;

		if (isinf(value->vlu_misc.vlu_double))
		{
			ERR_post(Arg::Gds(isc_arith_except) <<
					 Arg::Gds(isc_exception_float_overflow));
		}

		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;

		return &value->vlu_desc;
	}

	// Handle (oh, ugh) quad arithmetic

	if (nodFlags & FLAG_QUAD)
	{
		const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
		const SQUAD q1 = MOV_get_quad(desc, nodScale - scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, scale);

		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = nodScale;
		value->vlu_misc.vlu_quad = QUAD_MULTIPLY(q1, q2, ERR_post);
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return &value->vlu_desc;
	}

	// Everything else defaults to int64

	const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
	const SINT64 i1 = MOV_get_int64(desc, nodScale - scale);
	const SINT64 i2 = MOV_get_int64(&value->vlu_desc, scale);

	/*
	We need to report an overflow if
	   (i1 * i2 < MIN_SINT64) || (i1 * i2 > MAX_SINT64)
	which is equivalent to
	   (i1 < MIN_SINT64 / i2) || (i1 > MAX_SINT64 / i2)

	Unfortunately, a trial division to see whether the multiplication will
	overflow is expensive: fortunately, we only need perform one division and
	test for one of the two cases, depending on whether the factors have the
	same or opposite signs.

	Unfortunately, in C it is unspecified which way division rounds
	when one or both arguments are negative.  (ldiv() is guaranteed to
	round towards 0, but the standard does not yet require an lldiv()
	or whatever for 64-bit operands.  This makes the problem messy.
	We use FB_UINT64s for the checking, thus ensuring that our division rounds
	down.  This means that we have to check the sign of the product first
	in order to know whether the maximum abs(i1*i2) is MAX_SINT64 or
	(MAX_SINT64+1).

	Of course, if a factor is 0, the product will also be 0, and we don't
	need a trial-division to be sure the multiply won't overflow.
	*/

	const FB_UINT64 u1 = (i1 >= 0) ? i1 : -i1;	// abs(i1)
	const FB_UINT64 u2 = (i2 >= 0) ? i2 : -i2;	// abs(i2)
	// largest product
	const FB_UINT64 u_limit = ((i1 ^ i2) >= 0) ? MAX_SINT64 : (FB_UINT64) MAX_SINT64 + 1;

	if ((u1 != 0) && ((u_limit / u1) < u2)) {
		ERR_post(Arg::Gds(isc_exception_integer_overflow));
	}

	value->vlu_desc.dsc_dtype = dtype_int64;
	value->vlu_desc.dsc_length = sizeof(SINT64);
	value->vlu_desc.dsc_scale = nodScale;
	value->vlu_misc.vlu_int64 = i1 * i2;
	value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;

	return &value->vlu_desc;
}

// Divide two numbers, with SQL dialect-3 semantics, as in the blr_version5 ... blr_divide or
// blr_version5 ... blr_average ....
dsc* ArithmeticNode::divide2(const dsc* desc, impure_value* value) const
{
	DEV_BLKCHK(node, type_nod);

	// Handle floating arithmetic

	if (nodFlags & FLAG_DOUBLE)
	{
		const double d2 = MOV_get_double(desc);
		if (d2 == 0.0)
		{
			ERR_post(Arg::Gds(isc_arith_except) <<
					 Arg::Gds(isc_exception_float_divide_by_zero));
		}
		const double d1 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = d1 / d2;
		if (isinf(value->vlu_misc.vlu_double))
		{
			ERR_post(Arg::Gds(isc_arith_except) <<
					 Arg::Gds(isc_exception_float_overflow));
		}
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;
		return &value->vlu_desc;
	}

	// Everything else defaults to int64

	/*
	 * In the SQL standard, the precision and scale of the quotient of exact
	 * numeric dividend and divisor are implementation-defined: we have defined
	 * the precision as 18 (in other words, an SINT64), and the scale as the
	 * sum of the scales of the two operands.  To make this work, we have to
	 * multiply by pow(10, -2* (scale of divisor)).
	 *
	 * To see this, consider the operation n1 / n2, and represent the numbers
	 * by ordered pairs (v1, s1) and (v2, s2), representing respectively the
	 * integer value and the scale of each operation, so that
	 *     n1 = v1 * pow(10, s1), and
	 *     n2 = v2 * pow(10, s2)
	 * Then the quotient is ...
	 *
	 *     v1 * pow(10,s1)
	 *     ----------------- = (v1/v2) * pow(10, s1-s2)
	 *     v2 * pow(10,s2)
	 *
	 * But we want the scale of the result to be (s1+s2), not (s1-s2)
	 * so we need to multiply by 1 in the form
	 *         pow(10, -2 * s2) * pow(20, 2 * s2)
	 * which, after regrouping, gives us ...
	 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, 2*s2) * pow(10, s1-s2)
	 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, 2*s2 + s1 - s2)
	 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, s1 + s2)
	 * or in our ordered-pair notation,
	 *      ( v1 * pow(10, -2*s2) / v2, s1 + s2 )
	 *
	 * To maximize the amount of information in the result, we scale up
	 * the dividend as far as we can without causing overflow, then we perform
	 * the division, then do any additional required scaling.
	 *
	 * Who'da thunk that 9th-grade algebra would prove so useful.
	 *                                      -- Chris Jewell, December 1998
	 */
	SINT64 i2 = MOV_get_int64(desc, desc->dsc_scale);
	if (i2 == 0)
	{
		ERR_post(Arg::Gds(isc_arith_except) <<
				 Arg::Gds(isc_exception_integer_divide_by_zero));
	}

	SINT64 i1 = MOV_get_int64(&value->vlu_desc, nodScale - desc->dsc_scale);

	// MIN_SINT64 / -1 = (MAX_SINT64 + 1), which overflows in SINT64.
	if ((i1 == MIN_SINT64) && (i2 == -1))
		ERR_post(Arg::Gds(isc_exception_integer_overflow));

	// Scale the dividend by as many of the needed powers of 10 as possible
	// without causing an overflow.
	int addl_scale = 2 * desc->dsc_scale;
	if (i1 >= 0)
	{
		while ((addl_scale < 0) && (i1 <= MAX_INT64_LIMIT))
		{
			i1 *= 10;
			++addl_scale;
		}
	}
	else
	{
		while ((addl_scale < 0) && (i1 >= MIN_INT64_LIMIT))
		{
			i1 *= 10;
			++addl_scale;
		}
	}

	// If we couldn't use up all the additional scaling by multiplying the
	// dividend by 10, but there are trailing zeroes in the divisor, we can
	// get the same effect by dividing the divisor by 10 instead.
	while ((addl_scale < 0) && (0 == (i2 % 10)))
	{
		i2 /= 10;
		++addl_scale;
	}

	value->vlu_desc.dsc_dtype = dtype_int64;
	value->vlu_desc.dsc_length = sizeof(SINT64);
	value->vlu_desc.dsc_scale = nodScale;
	value->vlu_misc.vlu_int64 = i1 / i2;
	value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;

	// If we couldn't do all the required scaling beforehand without causing
	// an overflow, do the rest of it now.  If we get an overflow now, then
	// the result is really too big to store in a properly-scaled SINT64,
	// so report the error. For example, MAX_SINT64 / 1.00 overflows.
	if (value->vlu_misc.vlu_int64 >= 0)
	{
		while ((addl_scale < 0) && (value->vlu_misc.vlu_int64 <= MAX_INT64_LIMIT))
		{
			value->vlu_misc.vlu_int64 *= 10;
			addl_scale++;
		}
	}
	else
	{
		while ((addl_scale < 0) && (value->vlu_misc.vlu_int64 >= MIN_INT64_LIMIT))
		{
			value->vlu_misc.vlu_int64 *= 10;
			addl_scale++;
		}
	}

	if (addl_scale < 0)
	{
		ERR_post(Arg::Gds(isc_arith_except) <<
				 Arg::Gds(isc_numeric_out_of_range));
	}

	return &value->vlu_desc;
}

// Vector out to one of the actual datetime addition routines.
dsc* ArithmeticNode::addDateTime(const dsc* desc, impure_value* value) const
{
	BYTE dtype;					// Which addition routine to use?

	fb_assert(nodFlags & FLAG_DATE);

	// Value is the LHS of the operand.  desc is the RHS

	if (blrOp == blr_add)
		dtype = DSC_add_result[value->vlu_desc.dsc_dtype][desc->dsc_dtype];
	else
	{
		fb_assert(blrOp == blr_subtract);
		dtype = DSC_sub_result[value->vlu_desc.dsc_dtype][desc->dsc_dtype];

		/* Is this a <date type> - <date type> construct?
		   chose the proper routine to do the subtract from the
		   LHS of expression
		   Thus:   <TIME> - <TIMESTAMP> uses TIME arithmetic
		   <DATE> - <TIMESTAMP> uses DATE arithmetic
		   <TIMESTAMP> - <DATE> uses TIMESTAMP arithmetic */
		if (DTYPE_IS_NUMERIC(dtype))
			dtype = value->vlu_desc.dsc_dtype;

		// Handle historical <timestamp> = <string> - <value> case
		if (!DTYPE_IS_DATE(dtype) &&
			(DTYPE_IS_TEXT(value->vlu_desc.dsc_dtype) || DTYPE_IS_TEXT(desc->dsc_dtype)))
		{
			dtype = dtype_timestamp;
		}
	}

	switch (dtype)
	{
		case dtype_sql_time:
			return addSqlTime(desc, value);

		case dtype_sql_date:
			return addSqlDate(desc, value);

		case DTYPE_CANNOT:
			ERR_post(Arg::Gds(isc_expression_eval_err) << Arg::Gds(isc_invalid_type_datetime_op));
			break;

		case dtype_timestamp:
		default:
			// This needs to handle a dtype_sql_date + dtype_sql_time
			// For historical reasons prior to V6 - handle any types for timestamp arithmetic
			return addTimeStamp(desc, value);
	}

	return NULL;
}

// Perform date arithmetic.
// DATE -   DATE	   Result is SLONG
// DATE +/- NUMERIC   Numeric is interpreted as days DECIMAL(*,0).
// NUMERIC +/- TIME   Numeric is interpreted as days DECIMAL(*,0).
dsc* ArithmeticNode::addSqlDate(const dsc* desc, impure_value* value) const
{
	DEV_BLKCHK(node, type_nod);
	fb_assert(blrOp == blr_add || blrOp == blr_subtract);

	dsc* result = &value->vlu_desc;

	fb_assert(value->vlu_desc.dsc_dtype == dtype_sql_date || desc->dsc_dtype == dtype_sql_date);

	SINT64 d1;
	// Coerce operand1 to a count of days
	bool op1_is_date = false;
	if (value->vlu_desc.dsc_dtype == dtype_sql_date)
	{
		d1 = *((GDS_DATE*) value->vlu_desc.dsc_address);
		op1_is_date = true;
	}
	else
		d1 = MOV_get_int64(&value->vlu_desc, 0);

	SINT64 d2;
	// Coerce operand2 to a count of days
	bool op2_is_date = false;
	if (desc->dsc_dtype == dtype_sql_date)
	{
		d2 = *((GDS_DATE*) desc->dsc_address);
		op2_is_date = true;
	}
	else
		d2 = MOV_get_int64(desc, 0);

	if (blrOp == blr_subtract && op1_is_date && op2_is_date)
	{
		d2 = d1 - d2;
		value->make_int64(d2);
		return result;
	}

	fb_assert(op1_is_date || op2_is_date);
	fb_assert(!(op1_is_date && op2_is_date));

	// Perform the operation

	if (blrOp == blr_subtract)
	{
		fb_assert(op1_is_date);
		d2 = d1 - d2;
	}
	else
		d2 = d1 + d2;

	value->vlu_misc.vlu_sql_date = d2;

	if (!TimeStamp::isValidDate(value->vlu_misc.vlu_sql_date))
		ERR_post(Arg::Gds(isc_date_range_exceeded));

	result->dsc_dtype = dtype_sql_date;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_sql_date;
	return result;

}

// Perform time arithmetic.
// TIME - TIME			Result is SLONG, scale -4
// TIME +/- NUMERIC		Numeric is interpreted as seconds DECIMAL(*,4).
// NUMERIC +/- TIME		Numeric is interpreted as seconds DECIMAL(*,4).
dsc* ArithmeticNode::addSqlTime(const dsc* desc, impure_value* value) const
{
	DEV_BLKCHK(node, type_nod);
	fb_assert(blrOp == blr_add || blrOp == blr_subtract);

	dsc* result = &value->vlu_desc;

	fb_assert(value->vlu_desc.dsc_dtype == dtype_sql_time || desc->dsc_dtype == dtype_sql_time);

	SINT64 d1;
	// Coerce operand1 to a count of seconds
	bool op1_is_time = false;
	if (value->vlu_desc.dsc_dtype == dtype_sql_time)
	{
		d1 = *(GDS_TIME*) value->vlu_desc.dsc_address;
		op1_is_time = true;
		fb_assert(d1 >= 0 && d1 < ISC_TICKS_PER_DAY);
	}
	else
		d1 = MOV_get_int64(&value->vlu_desc, ISC_TIME_SECONDS_PRECISION_SCALE);

	SINT64 d2;
	// Coerce operand2 to a count of seconds
	bool op2_is_time = false;
	if (desc->dsc_dtype == dtype_sql_time)
	{
		d2 = *(GDS_TIME*) desc->dsc_address;
		op2_is_time = true;
		fb_assert(d2 >= 0 && d2 < ISC_TICKS_PER_DAY);
	}
	else
		d2 = MOV_get_int64(desc, ISC_TIME_SECONDS_PRECISION_SCALE);

	if (blrOp == blr_subtract && op1_is_time && op2_is_time)
	{
		d2 = d1 - d2;
		// Overflow cannot occur as the range of supported TIME values
		// is less than the range of INTEGER
		value->vlu_misc.vlu_long = d2;
		result->dsc_dtype = dtype_long;
		result->dsc_length = sizeof(SLONG);
		result->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_long;
		return result;
	}

	fb_assert(op1_is_time || op2_is_time);
	fb_assert(!(op1_is_time && op2_is_time));

	// Perform the operation

	if (blrOp == blr_subtract)
	{
		fb_assert(op1_is_time);
		d2 = d1 - d2;
	}
	else
		d2 = d1 + d2;

	// Make sure to use modulo 24 hour arithmetic

	// Make the result positive
	while (d2 < 0)
		d2 += (ISC_TICKS_PER_DAY);

	// And make it in the range of values for a day
	d2 %= (ISC_TICKS_PER_DAY);

	fb_assert(d2 >= 0 && d2 < ISC_TICKS_PER_DAY);

	value->vlu_misc.vlu_sql_time = d2;

	result->dsc_dtype = dtype_sql_time;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_sql_time;
	return result;
}

// Perform date&time arithmetic.
// TIMESTAMP - TIMESTAMP	Result is INT64
// TIMESTAMP +/- NUMERIC   Numeric is interpreted as days DECIMAL(*,*).
// NUMERIC +/- TIMESTAMP   Numeric is interpreted as days DECIMAL(*,*).
// DATE + TIME
// TIME + DATE
dsc* ArithmeticNode::addTimeStamp(const dsc* desc, impure_value* value) const
{
	fb_assert(blrOp == blr_add || blrOp == blr_subtract);

	SINT64 d1, d2;

	dsc* result = &value->vlu_desc;

	// Operand 1 is Value -- Operand 2 is desc

	if (value->vlu_desc.dsc_dtype == dtype_sql_date)
	{
		// DATE + TIME
		if (desc->dsc_dtype == dtype_sql_time && blrOp == blr_add)
		{
			value->vlu_misc.vlu_timestamp.timestamp_date = value->vlu_misc.vlu_sql_date;
			value->vlu_misc.vlu_timestamp.timestamp_time = *(GDS_TIME*) desc->dsc_address;
		}
		else
			ERR_post(Arg::Gds(isc_expression_eval_err) << Arg::Gds(isc_onlycan_add_timetodate));
	}
	else if (desc->dsc_dtype == dtype_sql_date)
	{
		// TIME + DATE
		if (value->vlu_desc.dsc_dtype == dtype_sql_time && blrOp == blr_add)
		{
			value->vlu_misc.vlu_timestamp.timestamp_time = value->vlu_misc.vlu_sql_time;
			value->vlu_misc.vlu_timestamp.timestamp_date = *(GDS_DATE*) desc->dsc_address;
		}
		else
			ERR_post(Arg::Gds(isc_expression_eval_err) << Arg::Gds(isc_onlycan_add_datetotime));
	}
	else
	{
		/* For historical reasons (behavior prior to V6),
		there are times we will do timestamp arithmetic without a
		timestamp being involved.
		In such an event we need to convert a text type to a timestamp when
		we don't already have one.
		We assume any text string must represent a timestamp value. */

		/* If we're subtracting, and the 2nd operand is a timestamp, or
		something that looks & smells like it could be a timestamp, then
		we must be doing <timestamp> - <timestamp> subtraction.
		Notes that this COULD be as strange as <string> - <string>, but
		because FLAG_DATE is set in the nod_flags we know we're supposed
		to use some form of date arithmetic */

		if (blrOp == blr_subtract &&
			(desc->dsc_dtype == dtype_timestamp || DTYPE_IS_TEXT(desc->dsc_dtype)))
		{
			/* Handle cases of
			   <string>    - <string>
			   <string>    - <timestamp>
			   <timestamp> - <string>
			   <timestamp> - <timestamp>
			   in which cases we assume the string represents a timestamp value */

			// If the first operand couldn't represent a timestamp, bomb out

			if (!(value->vlu_desc.dsc_dtype == dtype_timestamp ||
					DTYPE_IS_TEXT(value->vlu_desc.dsc_dtype)))
			{
				ERR_post(Arg::Gds(isc_expression_eval_err) << Arg::Gds(isc_onlycansub_tstampfromtstamp));
			}

			d1 = getTimeStampToIscTicks(&value->vlu_desc);
			d2 = getTimeStampToIscTicks(desc);

			d2 = d1 - d2;

			if (!dialect1)
			{
				/* multiply by 100,000 so that we can have the result as decimal (18,9)
				 * We have 10 ^-4; to convert this to 10^-9 we need to multiply by
				 * 100,000. Of course all this is true only because we are dividing
				 * by SECONDS_PER_DAY
				 * now divide by the number of seconds per day, this will give us the
				 * result as a int64 of type decimal (18, 9) in days instead of
				 * seconds.
				 *
				 * But SECONDS_PER_DAY has 2 trailing zeroes (because it is 24 * 60 *
				 * 60), so instead of calculating (X * 100000) / SECONDS_PER_DAY,
				 * use (X * (100000 / 100)) / (SECONDS_PER_DAY / 100), which can be
				 * simplified to (X * 1000) / (SECONDS_PER_DAY / 100)
				 * Since the largest possible difference in timestamps is about 3E11
				 * seconds or 3E15 isc_ticks, the product won't exceed approximately
				 * 3E18, which fits into an INT64.
				 */
				// 09-Apr-2004, Nickolay Samofatov. Adjust number before division to
				// make sure we don't lose a tick as a result of remainder truncation

				if (d2 >= 0)
					d2 = (d2 * 1000 + (SECONDS_PER_DAY / 200)) / (SINT64) (SECONDS_PER_DAY / 100);
				else
					d2 = (d2 * 1000 - (SECONDS_PER_DAY / 200)) / (SINT64) (SECONDS_PER_DAY / 100);

				value->vlu_misc.vlu_int64 = d2;
				result->dsc_dtype = dtype_int64;
				result->dsc_length = sizeof(SINT64);
				result->dsc_scale = DIALECT_3_TIMESTAMP_SCALE;
				result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;

				return result;
			}

			// This is dialect 1 subtraction returning double as before
			value->vlu_misc.vlu_double = (double) d2 / ((double) ISC_TICKS_PER_DAY);
			result->dsc_dtype = dtype_double;
			result->dsc_length = sizeof(double);
			result->dsc_scale = DIALECT_1_TIMESTAMP_SCALE;
			result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;

			return result;
		}

		/* From here we know our result must be a <timestamp>.  The only
		legal cases are:
		<timestamp> +/-  <numeric>
		<numeric>   +    <timestamp>
		However, the FLAG_DATE flag might have been set on any type of
		nod_add / nod_subtract equation -- so we must detect any invalid
		operands.   Any <string> value is assumed to be convertable to
		a timestamp */

		// Coerce operand1 to a count of microseconds

		bool op1_is_timestamp = false;

		if (value->vlu_desc.dsc_dtype == dtype_timestamp ||
			(DTYPE_IS_TEXT(value->vlu_desc.dsc_dtype)))
		{
			op1_is_timestamp = true;
		}

		// Coerce operand2 to a count of microseconds

		bool op2_is_timestamp = false;

		if ((desc->dsc_dtype == dtype_timestamp) || (DTYPE_IS_TEXT(desc->dsc_dtype)))
			op2_is_timestamp = true;

		// Exactly one of the operands must be a timestamp or
		// convertable into a timestamp, otherwise it's one of
		//    <numeric>   +/- <numeric>
		// or <timestamp> +/- <timestamp>
		// or <string>    +/- <string>
		// which are errors

		if (op1_is_timestamp == op2_is_timestamp)
			ERR_post(Arg::Gds(isc_expression_eval_err) << Arg::Gds(isc_onlyoneop_mustbe_tstamp));

		if (op1_is_timestamp)
		{
			d1 = getTimeStampToIscTicks(&value->vlu_desc);
			d2 = getDayFraction(desc);
		}
		else
		{
			fb_assert(blrOp == blr_add);
			fb_assert(op2_is_timestamp);
			d1 = getDayFraction(&value->vlu_desc);
			d2 = getTimeStampToIscTicks(desc);
		}

		// Perform the operation

		if (blrOp == blr_subtract)
		{
			fb_assert(op1_is_timestamp);
			d2 = d1 - d2;
		}
		else
			d2 = d1 + d2;

		// Convert the count of microseconds back to a date / time format

		value->vlu_misc.vlu_timestamp.timestamp_date = d2 / (ISC_TICKS_PER_DAY);
		value->vlu_misc.vlu_timestamp.timestamp_time = (d2 % ISC_TICKS_PER_DAY);

		// Make sure the TIME portion is non-negative

		if ((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time < 0)
		{
			value->vlu_misc.vlu_timestamp.timestamp_time =
				((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time) + ISC_TICKS_PER_DAY;
			value->vlu_misc.vlu_timestamp.timestamp_date -= 1;
		}

		if (!TimeStamp::isValidTimeStamp(value->vlu_misc.vlu_timestamp))
			ERR_post(Arg::Gds(isc_datetime_range_exceeded));
	}

	fb_assert(value->vlu_misc.vlu_timestamp.timestamp_time >= 0 &&
		value->vlu_misc.vlu_timestamp.timestamp_time < ISC_TICKS_PER_DAY);

	result->dsc_dtype = dtype_timestamp;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_timestamp;

	return result;
}

ValueExprNode* ArithmeticNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) ArithmeticNode(getPool(), blrOp, dialect1,
		PASS1_node(dsqlScratch, dsqlArg1), PASS1_node(dsqlScratch, dsqlArg2));
}


//--------------------


static RegisterNode<BoolAsValueNode> regBoolAsValueNode(blr_bool_as_value);

BoolAsValueNode::BoolAsValueNode(MemoryPool& pool, dsql_nod* aBoolean)
	: TypedNode<ValueExprNode, ExprNode::TYPE_BOOL_AS_VALUE>(pool),
	  dsqlBoolean(aBoolean),
	  boolean(NULL)
{
	addChildNode(dsqlBoolean, boolean);
}

DmlNode* BoolAsValueNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	BoolAsValueNode* node = FB_NEW(pool) BoolAsValueNode(pool);
	node->boolean = PAR_parse_boolean(tdbb, csb);
	return node;
}

void BoolAsValueNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "BoolAsValueNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* BoolAsValueNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	BoolAsValueNode* node = FB_NEW(getPool()) BoolAsValueNode(getPool(),
		PASS1_node(dsqlScratch, dsqlBoolean));

	return node;
}

void BoolAsValueNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_bool_as_value);
	GEN_expr(dsqlScratch, dsqlBoolean);
}

void BoolAsValueNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	desc->makeBoolean();
	desc->setNullable(true);
}

void BoolAsValueNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->makeBoolean();
	desc->setNullable(true);
}

ValueExprNode* BoolAsValueNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	BoolAsValueNode* node = FB_NEW(*tdbb->getDefaultPool()) BoolAsValueNode(*tdbb->getDefaultPool());
	node->boolean = copier.copy(tdbb, boolean);
	return node;
}

ValueExprNode* BoolAsValueNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* BoolAsValueNode::execute(thread_db* tdbb, jrd_req* request) const
{
	UCHAR booleanVal = (UCHAR) boolean->execute(tdbb, request);

	if (request->req_flags & req_null)
		return NULL;

	impure_value* impure = request->getImpure<impure_value>(impureOffset);

	dsc desc;
	desc.makeBoolean(&booleanVal);
	EVL_make_value(tdbb, &desc, impure);

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<CastNode> regCastNode(blr_cast);

CastNode::CastNode(MemoryPool& pool, dsql_nod* aDsqlSource, dsql_fld* aDsqlField)
	: TypedNode<ValueExprNode, ExprNode::TYPE_CAST>(pool),
	  dsqlAlias("CAST"),
	  dsqlSource(aDsqlSource),
	  dsqlField(aDsqlField),
	  source(NULL),
	  format(NULL),
	  itemInfo(NULL)
{
	castDesc.clear();
	addChildNode(dsqlSource, source);
}

// Parse a datatype cast.
DmlNode* CastNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	CastNode* node = FB_NEW(pool) CastNode(pool);

	Format* format = Format::newFormat(*tdbb->getDefaultPool(), 1);
	node->format = format;

	dsc* desc = &format->fmt_desc[0];
	ItemInfo itemInfo;
	PAR_desc(tdbb, csb, desc, &itemInfo);
	format->fmt_length = desc->dsc_length;

	node->source = PAR_parse_value(tdbb, csb);

	if (itemInfo.isSpecial())
		node->itemInfo = FB_NEW(*tdbb->getDefaultPool()) ItemInfo(*tdbb->getDefaultPool(), itemInfo);

	if (itemInfo.explicitCollation)
	{
		CompilerScratch::Dependency dependency(obj_collation);
		dependency.number = INTL_TEXT_TYPE(*desc);
		csb->csb_dependencies.push(dependency);
	}

	return node;
}

void CastNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("CastNode");
	ExprNode::print(text, nodes);
}

ValueExprNode* CastNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	CastNode* node = FB_NEW(getPool()) CastNode(getPool());
	node->dsqlAlias = dsqlAlias;
	node->dsqlSource = PASS1_node(dsqlScratch, dsqlSource);
	node->dsqlField = dsqlField;

	DDL_resolve_intl_type(dsqlScratch, node->dsqlField, NULL);
	node->setParameterType(dsqlScratch, NULL, false);

	MAKE_desc_from_field(&node->castDesc, node->dsqlField);
	MAKE_desc(dsqlScratch, &node->dsqlSource->nod_desc, node->dsqlSource);

	node->castDesc.dsc_flags = node->dsqlSource->nod_desc.dsc_flags & DSC_nullable;

	return node;
}

void CastNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = dsqlAlias;
}

bool CastNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* /*node*/, bool /*forceVarChar*/)
{
	// ASF: Attention: CastNode::dsqlPass calls us with NULL node.

	ParameterNode* paramNode = ExprNode::as<ParameterNode>(dsqlSource);

	if (paramNode)
	{
		dsql_par* parameter = paramNode->dsqlParameter;

		if (parameter)
		{
			parameter->par_node = dsqlSource;
			MAKE_desc_from_field(&parameter->par_desc, dsqlField);
			if (!dsqlField->fld_full_domain)
				parameter->par_desc.setNullable(true);
			return true;
		}
	}

	return false;
}

// Generate BLR for a data-type cast operation.
void CastNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_cast);
	dsqlScratch->putDtype(dsqlField, true);
	GEN_expr(dsqlScratch, dsqlSource);
}

void CastNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	*desc = castDesc;
}

void CastNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	// ASF: Commented out code here appears correct and makes the expression
	// "1 + NULLIF(NULL, 0)" (failing since v2.1) to work. While this is natural as others
	// nodes calling getDesc on sub nodes, it's causing some problem with contexts of
	// views.

	dsc desc1;
	////source->getDesc(tdbb, csb, &desc1);

	*desc = format->fmt_desc[0];

	if ((desc->dsc_dtype <= dtype_any_text && !desc->dsc_length) ||
		(desc->dsc_dtype == dtype_varying && desc->dsc_length <= sizeof(USHORT)))
	{
		// Remove this call if enable the one above.
		source->getDesc(tdbb, csb, &desc1);

		desc->dsc_length = DSC_string_length(&desc1);

		if (desc->dsc_dtype == dtype_cstring)
			desc->dsc_length++;
		else if (desc->dsc_dtype == dtype_varying)
			desc->dsc_length += sizeof(USHORT);
	}

	////if (desc1.isNull())
	////	desc->setNull();
}

ValueExprNode* CastNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	CastNode* node = FB_NEW(getPool()) CastNode(getPool());

	node->source = copier.copy(tdbb, source);
	node->format = format;
	node->itemInfo = itemInfo;

	return node;
}

bool CastNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const CastNode* o = other->as<CastNode>();
	fb_assert(o);

	return dsqlField == o->dsqlField;
}

bool CastNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	FieldNode* fieldNode = other->as<FieldNode>();

	dsc desc1, desc2;
	getDesc(tdbb, csb, &desc1);

	if (fieldNode)
	{
		fieldNode->getDesc(tdbb, csb, &desc2);

		if (DSC_EQUIV(&desc1, &desc2, true) && source->sameAs(tdbb, csb, fieldNode))
			return true;
	}

	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	CastNode* o = other->as<CastNode>();
	fb_assert(o);

	o->getDesc(tdbb, csb, &desc2);

	return DSC_EQUIV(&desc1, &desc2, true) && source->sameAs(tdbb, csb, o->source);
}

ValueExprNode* CastNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass1(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);

	const USHORT ttype = INTL_TEXT_TYPE(desc);

	// Are we using a collation?
	if (TTYPE_TO_COLLATION(ttype) != 0)
	{
		CMP_post_resource(&csb->csb_resources, INTL_texttype_lookup(tdbb, ttype),
			Resource::rsc_collation, ttype);
	}

	return this;
}

ValueExprNode* CastNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Cast from one datatype to another.
dsc* CastNode::execute(thread_db* tdbb, jrd_req* request) const
{
	dsc* value = EVL_expr(tdbb, request, source);
	value = request->req_flags & req_null ? NULL : value;

	impure_value* impure = request->getImpure<impure_value>(impureOffset);

	impure->vlu_desc = format->fmt_desc[0];
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc;

	if (DTYPE_IS_TEXT(impure->vlu_desc.dsc_dtype))
	{
		USHORT length = DSC_string_length(&impure->vlu_desc);

		if (length <= 0 && value)
		{
			// cast is a subtype cast only

			length = DSC_string_length(value);

			if (impure->vlu_desc.dsc_dtype == dtype_cstring)
				++length;	// for NULL byte
			else if (impure->vlu_desc.dsc_dtype == dtype_varying)
				length += sizeof(USHORT);

			impure->vlu_desc.dsc_length = length;
		}

		length = impure->vlu_desc.dsc_length;

		// Allocate a string block of sufficient size.

		VaryingString* string = impure->vlu_string;

		if (string && string->str_length < length)
		{
			delete string;
			string = NULL;
		}

		if (!string)
		{
			string = impure->vlu_string = FB_NEW_RPT(*tdbb->getDefaultPool(), length) VaryingString();
			string->str_length = length;
		}

		impure->vlu_desc.dsc_address = string->str_data;
	}

	EVL_validate(tdbb, Item(Item::TYPE_CAST), itemInfo,
		value, value == NULL || (value->dsc_flags & DSC_null));

	if (!value)
		return NULL;

	dsc desc;
	char* text;
	UCHAR tempByte;

	if (value->dsc_dtype == dtype_boolean &&
		(DTYPE_IS_TEXT(impure->vlu_desc.dsc_dtype) || DTYPE_IS_BLOB(impure->vlu_desc.dsc_dtype)))
	{
		text = const_cast<char*>(MOV_get_boolean(value) ? "TRUE" : "FALSE");
		desc.makeText(strlen(text), CS_ASCII, reinterpret_cast<UCHAR*>(text));
		value = &desc;
	}
	else if (impure->vlu_desc.dsc_dtype == dtype_boolean &&
		(DTYPE_IS_TEXT(value->dsc_dtype) || DTYPE_IS_BLOB(value->dsc_dtype)))
	{
		desc.makeBoolean(&tempByte);

		MoveBuffer buffer;
		UCHAR* address;
		int len = MOV_make_string2(tdbb, value, CS_ASCII, &address, buffer);

		// Remove heading and trailing spaces.

		while (len > 0 && isspace(*address))
		{
			++address;
			--len;
		}

		while (len > 0 && isspace(address[len - 1]))
			--len;

		if (len == 4 && fb_utils::strnicmp(reinterpret_cast<char*>(address), "TRUE", len) == 0)
		{
			tempByte = '\1';
			value = &desc;
		}
		else if (len == 5 && fb_utils::strnicmp(reinterpret_cast<char*>(address), "FALSE", len) == 0)
		{
			tempByte = '\0';
			value = &desc;
		}
	}

	if (DTYPE_IS_BLOB(value->dsc_dtype) || DTYPE_IS_BLOB(impure->vlu_desc.dsc_dtype))
		BLB_move(tdbb, value, &impure->vlu_desc, NULL);
	else
		MOV_move(tdbb, value, &impure->vlu_desc);

	if (impure->vlu_desc.dsc_dtype == dtype_text)
		INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<CoalesceNode> regCoalesceNode(blr_coalesce);

DmlNode* CoalesceNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	CoalesceNode* node = FB_NEW(pool) CoalesceNode(pool);
	node->args = PAR_args(tdbb, csb);
	return node;
}

void CoalesceNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "CoalesceNode\n";
	nodes.add(dsqlArgs);
	ExprNode::print(text, nodes);
}

ValueExprNode* CoalesceNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	CoalesceNode* node = FB_NEW(getPool()) CoalesceNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArgs));

	AutoPtr<dsql_nod> nod(MAKE_node(Dsql::nod_class_exprnode, 1));
	nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(node);
	node->setParameterType(dsqlScratch, nod, false);

	return node;
}

void CoalesceNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "COALESCE";
}

bool CoalesceNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	// Set descriptor for output node.
	MAKE_desc(dsqlScratch, &node->nod_desc, node);

	bool ret = false;
	dsql_nod* const* ptr = dsqlArgs->nod_arg;

	for (const dsql_nod* const* const end = ptr + dsqlArgs->nod_count; ptr != end; ++ptr)
		ret |= PASS1_set_parameter_type(dsqlScratch, *ptr, node, false);

	return ret;
}

void CoalesceNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_coalesce);
	dsqlScratch->appendUChar(dsqlArgs->nod_count);

	dsql_nod* const* ptr = dsqlArgs->nod_arg;

	for (const dsql_nod* const* const end = ptr + dsqlArgs->nod_count; ptr != end; ++ptr)
		GEN_expr(dsqlScratch, *ptr);
}

void CoalesceNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc_from_list(dsqlScratch, desc, dsqlArgs, "COALESCE");
}

void CoalesceNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	Array<dsc> descs;
	descs.resize(args->args.getCount());

	unsigned i = 0;
	Array<const dsc*> descPtrs;
	descPtrs.resize(args->args.getCount());

	for (NestConst<ValueExprNode>* p = args->args.begin(); p != args->args.end(); ++p, ++i)
	{
		(*p)->getDesc(tdbb, csb, &descs[i]);
		descPtrs[i] = &descs[i];
	}

	DataTypeUtil(tdbb).makeFromList(desc, "COALESCE", descPtrs.getCount(), descPtrs.begin());
}

ValueExprNode* CoalesceNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	CoalesceNode* node = FB_NEW(*tdbb->getDefaultPool()) CoalesceNode(*tdbb->getDefaultPool());
	node->args = copier.copy(tdbb, args);
	return node;
}

ValueExprNode* CoalesceNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* CoalesceNode::execute(thread_db* tdbb, jrd_req* request) const
{
	const NestConst<ValueExprNode>* ptr = args->args.begin();
	const NestConst<ValueExprNode>* end = args->args.end();

	for (; ptr != end; ++ptr)
	{
		dsc* desc = EVL_expr(tdbb, request, *ptr);

		if (desc && !(request->req_flags & req_null))
			return desc;
	}

	return NULL;
}


//--------------------


static RegisterNode<ConcatenateNode> regConcatenateNode(blr_concatenate);

ConcatenateNode::ConcatenateNode(MemoryPool& pool, dsql_nod* aArg1, dsql_nod* aArg2)
	: TypedNode<ValueExprNode, ExprNode::TYPE_CONCATENATE>(pool),
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
	node->arg1 = PAR_parse_value(tdbb, csb);
	node->arg2 = PAR_parse_value(tdbb, csb);
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
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg1, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlArg2, node, forceVarChar);
}

void ConcatenateNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_concatenate);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

void ConcatenateNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1, desc2;

	MAKE_desc(dsqlScratch, &desc1, dsqlArg1);
	MAKE_desc(dsqlScratch, &desc2, dsqlArg2);

	if (desc1.isNull())
	{
		desc1.makeText(0, desc2.getTextType());
		desc1.setNull();
	}

	if (desc2.isNull())
	{
		desc2.makeText(0, desc1.getTextType());
		desc2.setNull();
	}

	DSqlDataTypeUtil(dsqlScratch).makeConcatenate(desc, &desc1, &desc2);
}

void ConcatenateNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	dsc desc1, desc2;

	arg1->getDesc(tdbb, csb, &desc1);
	arg2->getDesc(tdbb, csb, &desc2);

	DataTypeUtil(tdbb).makeConcatenate(desc, &desc1, &desc2);
}

ValueExprNode* ConcatenateNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ConcatenateNode* node = FB_NEW(*tdbb->getDefaultPool()) ConcatenateNode(*tdbb->getDefaultPool());
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);
	return node;
}

ValueExprNode* ConcatenateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* ConcatenateNode::execute(thread_db* tdbb, jrd_req* request) const
{
	const dsc* value1 = EVL_expr(tdbb, request, arg1);
	const ULONG flags = request->req_flags;
	const dsc* value2 = EVL_expr(tdbb, request, arg2);

	// restore saved NULL state

	if (flags & req_null)
		request->req_flags |= req_null;

	if (request->req_flags & req_null)
		return NULL;

	impure_value* impure = request->getImpure<impure_value>(impureOffset);
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

ValueExprNode* ConcatenateNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) ConcatenateNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg1), PASS1_node(dsqlScratch, dsqlArg2));
}


//--------------------


static RegisterNode<CurrentDateNode> regCurrentDateNode(blr_current_date);

DmlNode* CurrentDateNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* /*csb*/,
								UCHAR /*blrOp*/)
{
	return FB_NEW(pool) CurrentDateNode(pool);
}

void CurrentDateNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("CurrentDateNode");
	ExprNode::print(text, nodes);
}

void CurrentDateNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CURRENT_DATE";
}

void CurrentDateNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_current_date);
}

void CurrentDateNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_date;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentDateNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_date;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentDateNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentDateNode(*tdbb->getDefaultPool());
}

ValueExprNode* CurrentDateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* CurrentDateNode::execute(thread_db* /*tdbb*/, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	// Use the request timestamp.
	fb_assert(!request->req_timestamp.isEmpty());
	ISC_TIMESTAMP encTimes = request->req_timestamp.value();

	memset(&impure->vlu_desc, 0, sizeof(impure->vlu_desc));
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_timestamp;

	impure->vlu_desc.dsc_dtype = dtype_sql_date;
	impure->vlu_desc.dsc_length = type_lengths[dtype_sql_date];
	*(ULONG*) impure->vlu_desc.dsc_address = encTimes.timestamp_date;

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<CurrentTimeNode> regCurrentTimeNode(blr_current_time);
static RegisterNode<CurrentTimeNode> regCurrentTimeNode2(blr_current_time2);

DmlNode* CurrentTimeNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	unsigned precision = DEFAULT_TIME_PRECISION;

	fb_assert(blrOp == blr_current_time || blrOp == blr_current_time2);

	if (blrOp == blr_current_time2)
	{
		precision = csb->csb_blr_reader.getByte();

		if (precision > MAX_TIME_PRECISION)
			ERR_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));
	}

	return FB_NEW(pool) CurrentTimeNode(pool, precision);
}

void CurrentTimeNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("CurrentTimeNode (%d)", precision);
	ExprNode::print(text, nodes);
}

void CurrentTimeNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CURRENT_TIME";
}

void CurrentTimeNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (precision == DEFAULT_TIME_PRECISION)
		dsqlScratch->appendUChar(blr_current_time);
	else
	{
		dsqlScratch->appendUChar(blr_current_time2);
		dsqlScratch->appendUChar(precision);
	}
}

void CurrentTimeNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_time;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentTimeNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_time;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentTimeNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentTimeNode(*tdbb->getDefaultPool(), precision);
}

ValueExprNode* CurrentTimeNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

ValueExprNode* CurrentTimeNode::dsqlPass(DsqlCompilerScratch* /*dsqlScratch*/)
{
	if (precision > MAX_TIME_PRECISION)
		ERRD_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));

	return this;
}

dsc* CurrentTimeNode::execute(thread_db* /*tdbb*/, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	// Use the request timestamp.
	fb_assert(!request->req_timestamp.isEmpty());
	ISC_TIMESTAMP encTimes = request->req_timestamp.value();

	memset(&impure->vlu_desc, 0, sizeof(impure->vlu_desc));
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_timestamp;

	TimeStamp::round_time(encTimes.timestamp_time, precision);

	impure->vlu_desc.dsc_dtype = dtype_sql_time;
	impure->vlu_desc.dsc_length = type_lengths[dtype_sql_time];
	*(ULONG*) impure->vlu_desc.dsc_address = encTimes.timestamp_time;

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<CurrentTimeStampNode> regCurrentTimeStampNode(blr_current_timestamp);
static RegisterNode<CurrentTimeStampNode> regCurrentTimeStampNode2(blr_current_timestamp2);

DmlNode* CurrentTimeStampNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* csb,
	UCHAR blrOp)
{
	unsigned precision = DEFAULT_TIMESTAMP_PRECISION;

	fb_assert(blrOp == blr_current_timestamp || blrOp == blr_current_timestamp2);

	if (blrOp == blr_current_timestamp2)
	{
		precision = csb->csb_blr_reader.getByte();

		if (precision > MAX_TIME_PRECISION)
			ERR_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));
	}

	return FB_NEW(pool) CurrentTimeStampNode(pool, precision);
}

void CurrentTimeStampNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("CurrentTimeStampNode (%d)", precision);
	ExprNode::print(text, nodes);
}

void CurrentTimeStampNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CURRENT_TIMESTAMP";
}

void CurrentTimeStampNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (precision == DEFAULT_TIMESTAMP_PRECISION)
		dsqlScratch->appendUChar(blr_current_timestamp);
	else
	{
		dsqlScratch->appendUChar(blr_current_timestamp2);
		dsqlScratch->appendUChar(precision);
	}
}

void CurrentTimeStampNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	desc->dsc_dtype = dtype_timestamp;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentTimeStampNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->dsc_dtype = dtype_timestamp;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentTimeStampNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentTimeStampNode(*tdbb->getDefaultPool(), precision);
}

ValueExprNode* CurrentTimeStampNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

ValueExprNode* CurrentTimeStampNode::dsqlPass(DsqlCompilerScratch* /*dsqlScratch*/)
{
	if (precision > MAX_TIME_PRECISION)
		ERRD_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));

	return this;
}

dsc* CurrentTimeStampNode::execute(thread_db* /*tdbb*/, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	// Use the request timestamp.
	fb_assert(!request->req_timestamp.isEmpty());
	ISC_TIMESTAMP encTimes = request->req_timestamp.value();

	memset(&impure->vlu_desc, 0, sizeof(impure->vlu_desc));
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_timestamp;

	TimeStamp::round_time(encTimes.timestamp_time, precision);

	impure->vlu_desc.dsc_dtype = dtype_timestamp;
	impure->vlu_desc.dsc_length = type_lengths[dtype_timestamp];
	*((ISC_TIMESTAMP*) impure->vlu_desc.dsc_address) = encTimes;

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<CurrentRoleNode> regCurrentRoleNode(blr_current_role);

DmlNode* CurrentRoleNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* /*csb*/,
								UCHAR /*blrOp*/)
{
	return FB_NEW(pool) CurrentRoleNode(pool);
}

void CurrentRoleNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "CurrentRoleNode";
	ExprNode::print(text, nodes);
}

void CurrentRoleNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "ROLE";
}

void CurrentRoleNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_current_role);
}

void CurrentRoleNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	desc->dsc_dtype = dtype_varying;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length =
		(USERNAME_LENGTH / METADATA_BYTES_PER_CHAR) *
		METD_get_charset_bpc(dsqlScratch->getTransaction(), ttype_metadata) + sizeof(USHORT);
}

void CurrentRoleNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->dsc_dtype = dtype_text;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length = USERNAME_LENGTH * METADATA_BYTES_PER_CHAR;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
}

ValueExprNode* CurrentRoleNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentRoleNode(*tdbb->getDefaultPool());
}

ValueExprNode* CurrentRoleNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// CVC: Current role will get a validated role; IE one that exists.
dsc* CurrentRoleNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	impure->vlu_desc.dsc_dtype = dtype_text;
	impure->vlu_desc.dsc_sub_type = 0;
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.setTextType(ttype_metadata);
	const char* curRole = NULL;

	if (tdbb->getAttachment()->att_user)
	{
		curRole = tdbb->getAttachment()->att_user->usr_sql_role_name.c_str();
		impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(const_cast<char*>(curRole));
	}

	if (curRole)
		impure->vlu_desc.dsc_length = strlen(curRole);
	else
		impure->vlu_desc.dsc_length = 0;

	return &impure->vlu_desc;
}

ValueExprNode* CurrentRoleNode::dsqlPass(DsqlCompilerScratch* /*dsqlScratch*/)
{
	return FB_NEW(getPool()) CurrentRoleNode(getPool());
}


//--------------------


static RegisterNode<CurrentUserNode> regCurrentUserNode(blr_user_name);

DmlNode* CurrentUserNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* /*csb*/,
								UCHAR /*blrOp*/)
{
	return FB_NEW(pool) CurrentUserNode(pool);
}

void CurrentUserNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "CurrentUserNode";
	ExprNode::print(text, nodes);
}

void CurrentUserNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "USER";
}

void CurrentUserNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_user_name);
}

void CurrentUserNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	desc->dsc_dtype = dtype_varying;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length =
		(USERNAME_LENGTH / METADATA_BYTES_PER_CHAR) *
		METD_get_charset_bpc(dsqlScratch->getTransaction(), ttype_metadata) + sizeof(USHORT);
}

void CurrentUserNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->dsc_dtype = dtype_text;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length = USERNAME_LENGTH * METADATA_BYTES_PER_CHAR;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
}

ValueExprNode* CurrentUserNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentUserNode(*tdbb->getDefaultPool());
}

ValueExprNode* CurrentUserNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* CurrentUserNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	impure->vlu_desc.dsc_dtype = dtype_text;
	impure->vlu_desc.dsc_sub_type = 0;
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.setTextType(ttype_metadata);
	const char* curUser = NULL;

	if (tdbb->getAttachment()->att_user)
	{
		curUser = tdbb->getAttachment()->att_user->usr_user_name.c_str();
		impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(const_cast<char*>(curUser));
	}

	if (curUser)
		impure->vlu_desc.dsc_length = strlen(curUser);
	else
		impure->vlu_desc.dsc_length = 0;

	return &impure->vlu_desc;
}

ValueExprNode* CurrentUserNode::dsqlPass(DsqlCompilerScratch* /*dsqlScratch*/)
{
	return FB_NEW(getPool()) CurrentUserNode(getPool());
}


//--------------------


static RegisterNode<DecodeNode> regDecodeNode(blr_decode);

DmlNode* DecodeNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	DecodeNode* node = FB_NEW(pool) DecodeNode(pool);
	node->test = PAR_parse_value(tdbb, csb);
	node->conditions = PAR_args(tdbb, csb);
	node->values = PAR_args(tdbb, csb);
	return node;
}

void DecodeNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "DecodeNode\n";
	nodes.add(dsqlTest);
	nodes.add(dsqlConditions);
	nodes.add(dsqlValues);
	ExprNode::print(text, nodes);
}

ValueExprNode* DecodeNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	DecodeNode* node = FB_NEW(getPool()) DecodeNode(getPool(), PASS1_node(dsqlScratch, dsqlTest),
		PASS1_node(dsqlScratch, dsqlConditions), PASS1_node(dsqlScratch, dsqlValues));
	node->label = label;

	AutoPtr<dsql_nod> nod(MAKE_node(Dsql::nod_class_exprnode, 1));
	nod->nod_arg[0] = reinterpret_cast<dsql_nod*>(node);
	node->setParameterType(dsqlScratch, nod, false);

	return node;
}

void DecodeNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = label;
}

bool DecodeNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	// Check if there is a parameter in the test/conditions.
	bool setParameters = ExprNode::is<ParameterNode>(dsqlTest);

	if (!setParameters)
	{
		dsql_nod** ptr = dsqlConditions->nod_arg;

		for (const dsql_nod* const* const end = ptr + dsqlConditions->nod_count; ptr != end; ++ptr)
		{
			if (ExprNode::is<ParameterNode>(*ptr))
			{
				setParameters = true;
				break;
			}
		}
	}

	if (setParameters)
	{
		// Build list to make describe information for the test and conditions expressions.
		AutoPtr<dsql_nod> node1(MAKE_node(Dsql::nod_list, dsqlConditions->nod_count + 1));
		unsigned i = 0;

		node1->nod_arg[i++] = dsqlTest;
		dsql_nod** ptr = dsqlConditions->nod_arg;

		for (const dsql_nod* const* const end = ptr + dsqlConditions->nod_count; ptr != end; ++ptr, ++i)
			node1->nod_arg[i] = *ptr;

		MAKE_desc_from_list(dsqlScratch, &node1->nod_desc, node1, label.c_str());

		if (!node1->nod_desc.isUnknown())
		{
			// Set parameter describe information.
			PASS1_set_parameter_type(dsqlScratch, dsqlTest, node1, false);

			dsql_nod** ptr = dsqlConditions->nod_arg;

			for (const dsql_nod* const* const end = ptr + dsqlConditions->nod_count; ptr != end; ++ptr)
				PASS1_set_parameter_type(dsqlScratch, *ptr, node1, false);
		}
	}

	// Set descriptor for output node.
	MAKE_desc(dsqlScratch, &node->nod_desc, node);

	bool ret = false;
	dsql_nod* const* ptr = dsqlValues->nod_arg;

	for (const dsql_nod* const* const end = ptr + dsqlValues->nod_count; ptr != end; ++ptr)
		ret |= PASS1_set_parameter_type(dsqlScratch, *ptr, node, false);

	return ret;
}

void DecodeNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_decode);
	GEN_expr(dsqlScratch, dsqlTest);

	dsqlScratch->appendUChar(dsqlConditions->nod_count);

	dsql_nod* const* ptr = dsqlConditions->nod_arg;
	for (const dsql_nod* const* const end = ptr + dsqlConditions->nod_count; ptr != end; ++ptr)
		GEN_expr(dsqlScratch, *ptr);

	dsqlScratch->appendUChar(dsqlValues->nod_count);

	ptr = dsqlValues->nod_arg;
	for (const dsql_nod* const* const end = ptr + dsqlValues->nod_count; ptr != end; ++ptr)
		GEN_expr(dsqlScratch, *ptr);
}

void DecodeNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc_from_list(dsqlScratch, desc, dsqlValues, label.c_str());
	desc->setNullable(true);
}

void DecodeNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	Array<dsc> descs;
	descs.resize(values->args.getCount());

	unsigned i = 0;
	Array<const dsc*> descPtrs;
	descPtrs.resize(values->args.getCount());

	for (NestConst<ValueExprNode>* p = values->args.begin(); p != values->args.end(); ++p, ++i)
	{
		(*p)->getDesc(tdbb, csb, &descs[i]);
		descPtrs[i] = &descs[i];
	}

	DataTypeUtil(tdbb).makeFromList(desc, label.c_str(), descPtrs.getCount(), descPtrs.begin());

	desc->setNullable(true);
}

ValueExprNode* DecodeNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	DecodeNode* node = FB_NEW(*tdbb->getDefaultPool()) DecodeNode(*tdbb->getDefaultPool());
	node->test = copier.copy(tdbb, test);
	node->conditions = copier.copy(tdbb, conditions);
	node->values = copier.copy(tdbb, values);
	return node;
}

ValueExprNode* DecodeNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* DecodeNode::execute(thread_db* tdbb, jrd_req* request) const
{
	dsc* testDesc = EVL_expr(tdbb, request, test);

	// The comparisons are done with "equal" operator semantics, so if the test value is
	// NULL we have nothing to compare.
	if (testDesc && !(request->req_flags & req_null))
	{
		const NestConst<ValueExprNode>* conditionsPtr = conditions->args.begin();
		const NestConst<ValueExprNode>* conditionsEnd = conditions->args.end();
		const NestConst<ValueExprNode>* valuesPtr = values->args.begin();

		for (; conditionsPtr != conditionsEnd; ++conditionsPtr, ++valuesPtr)
		{
			dsc* desc = EVL_expr(tdbb, request, *conditionsPtr);

			if (desc && !(request->req_flags & req_null) && MOV_compare(testDesc, desc) == 0)
				return EVL_expr(tdbb, request, *valuesPtr);
		}
	}

	if (values->args.getCount() > conditions->args.getCount())
		return EVL_expr(tdbb, request, values->args.back());

	return NULL;
}


//--------------------


static RegisterNode<DerivedExprNode> regDerivedExprNode(blr_derived_expr);

DmlNode* DerivedExprNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	DerivedExprNode* node = FB_NEW(pool) DerivedExprNode(pool);

	const UCHAR streamCount = csb->csb_blr_reader.getByte();

	for (UCHAR i = 0; i < streamCount; ++i)
	{
		const USHORT n = csb->csb_blr_reader.getByte();
		node->streamList.add(csb->csb_rpt[n].csb_stream);
	}

	node->arg = PAR_parse_value(tdbb, csb);

	return node;
}

bool DerivedExprNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	if (!arg->computable(csb, stream, allowOnlyCurrentStream))
		return false;

	SortedStreamList argStreams;
	arg->jrdStreamsCollector(argStreams);

	for (USHORT* i = streamList.begin(); i != streamList.end(); ++i)
	{
		const USHORT n = *i;

		if (argStreams.exist(n))
		{
			// We've already checked computability of the argument,
			// so any stream it refers to is known to be active.
			continue;
		}

		if (allowOnlyCurrentStream)
		{
			if (n != stream && !(csb->csb_rpt[n].csb_flags & csb_sub_stream))
				return false;
		}
		else
		{
			if (n == stream)
				return false;
		}

		if (!(csb->csb_rpt[n].csb_flags & csb_active))
			return false;
	}

	return true;
}

void DerivedExprNode::findDependentFromStreams(const OptimizerRetrieval* optRet,
	SortedStreamList* streamList)
{
	arg->findDependentFromStreams(optRet, streamList);

	for (USHORT* i = this->streamList.begin(); i != this->streamList.end(); ++i)
	{
		int derivedStream = *i;

		if (derivedStream != optRet->stream &&
			(optRet->csb->csb_rpt[derivedStream].csb_flags & csb_active))
		{
			if (!streamList->exist(derivedStream))
				streamList->add(derivedStream);
		}
	}
}

void DerivedExprNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	arg->getDesc(tdbb, csb, desc);
}

ValueExprNode* DerivedExprNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	DerivedExprNode* node = FB_NEW(*tdbb->getDefaultPool()) DerivedExprNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	node->streamList = streamList;

	if (copier.remap)
	{
#ifdef CMP_DEBUG
		csb->dump("remap nod_derived_expr:\n");
		for (USHORT* i = node->streamList.begin(); i != node->streamList.end(); ++i)
			csb->dump("\t%d: %d -> %d\n", i, *i, copier.remap[*i]);
#endif

		for (USHORT* i = node->streamList.begin(); i != node->streamList.end(); ++i)
			*i = copier.remap[*i];
	}

	return node;
}

ValueExprNode* DerivedExprNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNodeStack stack;

#ifdef CMP_DEBUG
	csb->dump("expand nod_derived_expr:");
	for (USHORT* i = streamList.begin(); i != streamList.end(); ++i)
		csb->dump(" %d", *i);
	csb->dump("\n");
#endif

	for (USHORT* i = streamList.begin(); i != streamList.end(); ++i)
	{
		CMP_mark_variant(csb, *i);
		CMP_expand_view_nodes(tdbb, csb, *i, stack, blr_dbkey, true);
	}

	streamList.clear();

#ifdef CMP_DEBUG
	for (ExprValueNodeStack::iterator i(stack); i.hasData(); ++i)
		csb->dump(" %d", i.object()->as<RecordKeyNode>()->recStream);
	csb->dump("\n");
#endif

	for (ValueExprNodeStack::iterator i(stack); i.hasData(); ++i)
		streamList.add(i.object()->as<RecordKeyNode>()->recStream);

	return ValueExprNode::pass1(tdbb, csb);
}

ValueExprNode* DerivedExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* DerivedExprNode::execute(thread_db* tdbb, jrd_req* request) const
{
	dsc* value = NULL;

	for (const USHORT* i = streamList.begin(); i != streamList.end(); ++i)
	{
		if (request->req_rpb[*i].rpb_number.isValid())
		{
			value = EVL_expr(tdbb, request, arg);

			if (request->req_flags & req_null)
				value = NULL;

			break;
		}
	}

	return value;
}


//--------------------


void DomainValidationNode::print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const
{
	text.printf("DomainValidationNode");
	ExprNode::print(text, nodes);
}

ValueExprNode* DomainValidationNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	if (dsqlScratch->domainValue.isUnknown())
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_dsql_domain_err));
	}

	DomainValidationNode* node = FB_NEW(getPool()) DomainValidationNode(getPool());
	node->domDesc = dsqlScratch->domainValue;

	return node;
}

void DomainValidationNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_fid);
	dsqlScratch->appendUChar(0);		// context
	dsqlScratch->appendUShort(0);		// field id
}

void DomainValidationNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	*desc = domDesc;
}

void DomainValidationNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	*desc = domDesc;
}

ValueExprNode* DomainValidationNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	DomainValidationNode* node =
		FB_NEW(*tdbb->getDefaultPool()) DomainValidationNode(*tdbb->getDefaultPool());
	node->domDesc = domDesc;
	return node;
}

ValueExprNode* DomainValidationNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* DomainValidationNode::execute(thread_db* /*tdbb*/, jrd_req* request) const
{
	if (request->req_domain_validation == NULL ||
		(request->req_domain_validation->dsc_flags & DSC_null))
	{
		return NULL;
	}

	return request->req_domain_validation;
}


//--------------------


static RegisterNode<ExtractNode> regExtractNode(blr_extract);

ExtractNode::ExtractNode(MemoryPool& pool, UCHAR aBlrSubOp, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_EXTRACT>(pool),
	  blrSubOp(aBlrSubOp),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* ExtractNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	UCHAR blrSubOp = csb->csb_blr_reader.getByte();

	ExtractNode* node = FB_NEW(pool) ExtractNode(pool, blrSubOp);
	node->arg = PAR_parse_value(tdbb, csb);
	return node;
}

void ExtractNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("ExtractNode (%d)", blrSubOp);
	ExprNode::print(text, nodes);
}

ValueExprNode* ExtractNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	// Figure out the data type of the sub parameter, and make
	// sure the requested type of information can be extracted.

	dsql_nod* sub1 = PASS1_node(dsqlScratch, dsqlArg);
	MAKE_desc(dsqlScratch, &sub1->nod_desc, sub1);

	switch (blrSubOp)
	{
		case blr_extract_year:
		case blr_extract_month:
		case blr_extract_day:
		case blr_extract_weekday:
		case blr_extract_yearday:
		case blr_extract_week:
			if (!ExprNode::is<NullNode>(sub1) &&
				sub1->nod_desc.dsc_dtype != dtype_sql_date &&
				sub1->nod_desc.dsc_dtype != dtype_timestamp)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-105) <<
						  Arg::Gds(isc_extract_input_mismatch));
			}
			break;

		case blr_extract_hour:
		case blr_extract_minute:
		case blr_extract_second:
		case blr_extract_millisecond:
			if (!ExprNode::is<NullNode>(sub1) &&
				sub1->nod_desc.dsc_dtype != dtype_sql_time &&
				sub1->nod_desc.dsc_dtype != dtype_timestamp)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-105) <<
						  Arg::Gds(isc_extract_input_mismatch));
			}
			break;

		default:
			fb_assert(false);
			break;
	}

	return FB_NEW(getPool()) ExtractNode(getPool(), blrSubOp, sub1);
}

void ExtractNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "EXTRACT";
}

bool ExtractNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void ExtractNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_extract);
	dsqlScratch->appendUChar(blrSubOp);
	GEN_expr(dsqlScratch, dsqlArg);
}

void ExtractNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1;
	MAKE_desc(dsqlScratch, &desc1, dsqlArg);

	switch (blrSubOp)
	{
		case blr_extract_second:
			// QUADDATE - maybe this should be DECIMAL(6,4)
			desc->makeLong(ISC_TIME_SECONDS_PRECISION_SCALE);
			break;

		case blr_extract_millisecond:
			desc->makeLong(ISC_TIME_SECONDS_PRECISION_SCALE + 3);
			break;

		default:
			desc->makeShort(0);
			break;
	}

	desc->setNullable(desc1.isNullable());
}

void ExtractNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	switch (blrSubOp)
	{
		case blr_extract_second:
			// QUADDATE - SECOND returns a float, or scaled!
			desc->makeLong(ISC_TIME_SECONDS_PRECISION_SCALE);
			break;

		case blr_extract_millisecond:
			desc->makeLong(ISC_TIME_SECONDS_PRECISION_SCALE + 3);
			break;

		default:
			desc->makeShort(0);
			break;
	}
}

ValueExprNode* ExtractNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ExtractNode* node = FB_NEW(*tdbb->getDefaultPool()) ExtractNode(*tdbb->getDefaultPool(), blrSubOp);
	node->arg = copier.copy(tdbb, arg);
	return node;
}

bool ExtractNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const ExtractNode* o = other->as<ExtractNode>();
	fb_assert(o);

	return blrSubOp == o->blrSubOp;
}

bool ExtractNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	ExtractNode* o = other->as<ExtractNode>();
	fb_assert(o);

	return blrSubOp == o->blrSubOp;
}

ValueExprNode* ExtractNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Handles EXTRACT(part FROM date/time/timestamp)
dsc* ExtractNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	const dsc* value = EVL_expr(tdbb, request, arg);

	if (!value || (request->req_flags & req_null))
		return NULL;

	impure->vlu_desc.makeShort(0, &impure->vlu_misc.vlu_short);

	tm times = {0};
	int fractions;

	switch (value->dsc_dtype)
	{
		case dtype_sql_time:
			switch (blrSubOp)
			{
				case blr_extract_hour:
				case blr_extract_minute:
				case blr_extract_second:
				case blr_extract_millisecond:
					TimeStamp::decode_time(*(GDS_TIME*) value->dsc_address,
						&times.tm_hour, &times.tm_min, &times.tm_sec, &fractions);
					break;

				default:
					ERR_post(Arg::Gds(isc_expression_eval_err) <<
							 Arg::Gds(isc_invalid_extractpart_time));
			}
			break;

		case dtype_sql_date:
			switch (blrSubOp)
			{
				case blr_extract_hour:
				case blr_extract_minute:
				case blr_extract_second:
				case blr_extract_millisecond:
					ERR_post(Arg::Gds(isc_expression_eval_err) <<
							 Arg::Gds(isc_invalid_extractpart_date));
					break;

				default:
					TimeStamp::decode_date(*(GDS_DATE*) value->dsc_address, &times);
			}
			break;

		case dtype_timestamp:
			TimeStamp::decode_timestamp(*(GDS_TIMESTAMP*) value->dsc_address, &times, &fractions);
			break;

		default:
			ERR_post(Arg::Gds(isc_expression_eval_err) <<
					 Arg::Gds(isc_invalidarg_extract));
			break;
	}

	USHORT part;

	switch (blrSubOp)
	{
		case blr_extract_year:
			part = times.tm_year + 1900;
			break;

		case blr_extract_month:
			part = times.tm_mon + 1;
			break;

		case blr_extract_day:
			part = times.tm_mday;
			break;

		case blr_extract_hour:
			part = times.tm_hour;
			break;

		case blr_extract_minute:
			part = times.tm_min;
			break;

		case blr_extract_second:
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(ULONG);
			impure->vlu_desc.dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
			impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(&impure->vlu_misc.vlu_long);
			*(ULONG*) impure->vlu_desc.dsc_address = times.tm_sec * ISC_TIME_SECONDS_PRECISION + fractions;
			return &impure->vlu_desc;

		case blr_extract_millisecond:
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(ULONG);
			impure->vlu_desc.dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE + 3;
			impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(&impure->vlu_misc.vlu_long);
			(*(ULONG*) impure->vlu_desc.dsc_address) = fractions;
			return &impure->vlu_desc;

		case blr_extract_week:
		{
			// Algorithm for Converting Gregorian Dates to ISO 8601 Week Date by Rick McCarty, 1999
			// http://personal.ecu.edu/mccartyr/ISOwdALG.txt

			const int y = times.tm_year + 1900;
			const int dayOfYearNumber = times.tm_yday + 1;

			// Find the jan1Weekday for y (Monday=1, Sunday=7)
			const int yy = (y - 1) % 100;
			const int c = (y - 1) - yy;
			const int g = yy + yy / 4;
			const int jan1Weekday = 1 + (((((c / 100) % 4) * 5) + g) % 7);

			// Find the weekday for y m d
			const int h = dayOfYearNumber + (jan1Weekday - 1);
			const int weekday = 1 + ((h - 1) % 7);

			// Find if y m d falls in yearNumber y-1, weekNumber 52 or 53
			int yearNumber, weekNumber;

			if ((dayOfYearNumber <= (8 - jan1Weekday)) && (jan1Weekday > 4))
			{
				yearNumber = y - 1;
				weekNumber = ((jan1Weekday == 5) || ((jan1Weekday == 6) &&
					TimeStamp::isLeapYear(yearNumber))) ? 53 : 52;
			}
			else
			{
				yearNumber = y;

				// Find if y m d falls in yearNumber y+1, weekNumber 1
				int i = TimeStamp::isLeapYear(y) ? 366 : 365;

				if ((i - dayOfYearNumber) < (4 - weekday))
				{
					yearNumber = y + 1;
					weekNumber = 1;
				}
			}

			// Find if y m d falls in yearNumber y, weekNumber 1 through 53
			if (yearNumber == y)
			{
				int j = dayOfYearNumber + (7 - weekday) + (jan1Weekday - 1);
				weekNumber = j / 7;
				if (jan1Weekday > 4)
					weekNumber--;
			}

			part = weekNumber;
			break;
		}

		case blr_extract_yearday:
			part = times.tm_yday;
			break;

		case blr_extract_weekday:
			part = times.tm_wday;
			break;

		default:
			fb_assert(false);
			part = 0;
	}

	*(USHORT*) impure->vlu_desc.dsc_address = part;

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<FieldNode> regFieldNodeFid(blr_fid);
static RegisterNode<FieldNode> regFieldNodeField(blr_field);

FieldNode::FieldNode(MemoryPool& pool, dsql_ctx* context, dsql_fld* field, dsql_nod* indices)
	: TypedNode<ValueExprNode, ExprNode::TYPE_FIELD>(pool),
	  dsqlContext(context),
	  dsqlField(field),
	  dsqlIndices(indices),
	  byId(false),
	  fieldStream(0),
	  fieldId(0),
	  format(NULL)
{
	dsqlDesc.clear();
}

FieldNode::FieldNode(MemoryPool& pool, USHORT stream, USHORT id, bool aById)
	: TypedNode<ValueExprNode, ExprNode::TYPE_FIELD>(pool),
	  dsqlContext(NULL),
	  dsqlField(NULL),
	  dsqlIndices(NULL),
	  byId(aById),
	  fieldStream(stream),
	  fieldId(id),
	  format(NULL)
{
	dsqlDesc.clear();
}

// Parse a field.
DmlNode* FieldNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	const USHORT context = csb->csb_blr_reader.getByte();

	// check if this is a VALUE of domain's check constraint
	if (!csb->csb_domain_validation.isEmpty() && context == 0 &&
		(blrOp == blr_fid || blrOp == blr_field))
	{
		if (blrOp == blr_fid)
		{
#ifdef DEV_BUILD
			SSHORT id =
#endif
				csb->csb_blr_reader.getWord();
			fb_assert(id == 0);
		}
		else
		{
			MetaName name;
			PAR_name(csb, name);
		}

		DomainValidationNode* node = FB_NEW(pool) DomainValidationNode(pool);

		MET_get_domain(tdbb, csb->csb_domain_validation, &node->domDesc, NULL);

		return node;
	}

	if (context >= csb->csb_rpt.getCount())/* ||
		!(csb->csb_rpt[context].csb_flags & csb_used) )

		dimitr:	commented out to support system triggers implementing
				WITH CHECK OPTION. They reference the relation stream (#2)
				directly, without a DSQL context. It breaks the layering,
				but we must support legacy BLR.
		*/
	{
		PAR_error(csb, Arg::Gds(isc_ctxnotdef));
	}

	MetaName name;
	SSHORT id;
	const USHORT stream = csb->csb_rpt[context].csb_stream;
	SSHORT flags = 0;
	bool is_column = false;
	bool byId = false;

	if (blrOp == blr_fid)
	{
		id = csb->csb_blr_reader.getWord();
		byId = true;
		is_column = true;
	}
	else if (blrOp == blr_field)
	{
		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
		const jrd_prc* procedure = tail->csb_procedure;

		// make sure procedure has been scanned before using it

		if (procedure &&
			(!(procedure->prc_flags & PRC_scanned) ||
				(procedure->prc_flags & PRC_being_scanned) ||
				(procedure->prc_flags & PRC_being_altered)))
		{
			const jrd_prc* scan_proc = MET_procedure(tdbb, procedure->getId(), false, 0);

			if (scan_proc != procedure)
				procedure = NULL;
		}

		if (procedure)
		{
			PAR_name(csb, name);

			if ((id = PAR_find_proc_field(procedure, name)) == -1)
			{
				PAR_error(csb, Arg::Gds(isc_fldnotdef2) <<
					Arg::Str(name) << Arg::Str(procedure->getName().toString()));
			}
		}
		else
		{
			jrd_rel* relation = tail->csb_relation;
			if (!relation)
				PAR_error(csb, Arg::Gds(isc_ctxnotdef));

			// make sure relation has been scanned before using it

			if (!(relation->rel_flags & REL_scanned) || (relation->rel_flags & REL_being_scanned))
				MET_scan_relation(tdbb, relation);

			PAR_name(csb, name);

			if ((id = MET_lookup_field(tdbb, relation, name)) < 0)
			{
				if (csb->csb_g_flags & csb_validation)
				{
					id = 0;
					byId = true;
					is_column = true;
				}
				else
				{
					if (relation->rel_flags & REL_system)
						return FB_NEW(pool) NullNode(pool);

 					if (tdbb->getAttachment()->att_flags & ATT_gbak_attachment)
					{
						PAR_warning(Arg::Warning(isc_fldnotdef) << Arg::Str(name) <<
																   Arg::Str(relation->rel_name));
					}
					else if (!(relation->rel_flags & REL_deleted))
					{
						PAR_error(csb, Arg::Gds(isc_fldnotdef) << Arg::Str(name) <<
																  Arg::Str(relation->rel_name));
					}
					else
						PAR_error(csb, Arg::Gds(isc_ctxnotdef));
				}
			}
		}
	}

	// check for dependencies -- if a field name was given,
	// use it because when restoring the database the field
	// id's may not be valid yet

	if (csb->csb_g_flags & csb_get_dependencies)
	{
		if (blrOp == blr_fid)
			PAR_dependency(tdbb, csb, stream, id, "");
		else
			PAR_dependency(tdbb, csb, stream, id, name);
	}

	if (is_column)
	{
		jrd_rel* const temp_rel = csb->csb_rpt[stream].csb_relation;

		if (temp_rel)
		{
			if (id >= (int) temp_rel->rel_fields->count() || !(*temp_rel->rel_fields)[id])
			{
				if (temp_rel->rel_flags & REL_system)
					return FB_NEW(pool) NullNode(pool);
			}
		}
	}

	return PAR_gen_field(tdbb, stream, id, byId);
}

void FieldNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "FieldNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* FieldNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	// AB: nod_field is an already passed node.
	// This could be done in expand_select_list.
	return this;
}

bool FieldNode::dsqlAggregateFinder(AggregateFinder& visitor)
{
	if (visitor.deepestLevel < dsqlContext->ctx_scope_level)
		visitor.deepestLevel = dsqlContext->ctx_scope_level;

	return false;
}

bool FieldNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	return false;
}

bool FieldNode::dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
{
	// Wouldn't it be better to call an error from this point where return is true?
	// Then we could give the fieldname that's making the trouble.

	// If we come here then this field is used inside a aggregate-function. The
	// ctx_scope_level gives the info how deep the context is inside the statement.

	// If the context-scope-level from this field is lower or the same as the scope-level
	// from the given context then it is an invalid field.
	if (dsqlContext->ctx_scope_level == visitor.context->ctx_scope_level)
	{
		// Return true (invalid) if this field isn't inside the GROUP BY clause, that
		// should already been seen in the match_node test in that routine start.
		return true;
	}

	return false;
}

bool FieldNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return false;
}

bool FieldNode::dsqlFieldFinder(FieldFinder& visitor)
{
	visitor.field = true;

	switch (visitor.matchType)
	{
		case FIELD_MATCH_TYPE_EQUAL:
			return dsqlContext->ctx_scope_level == visitor.checkScopeLevel;

		case FIELD_MATCH_TYPE_LOWER:
			return dsqlContext->ctx_scope_level < visitor.checkScopeLevel;

		case FIELD_MATCH_TYPE_LOWER_EQUAL:
			return dsqlContext->ctx_scope_level <= visitor.checkScopeLevel;

		///case FIELD_MATCH_TYPE_HIGHER:
		///	return dsqlContext->ctx_scope_level > visitor.checkScopeLevel;

		///case FIELD_MATCH_TYPE_HIGHER_EQUAL:
		///	return dsqlContext->ctx_scope_level >= visitor.checkScopeLevel;

		default:
			fb_assert(false);
	}

	return false;
}

bool FieldNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	if (dsqlContext->ctx_scope_level == visitor.context->ctx_scope_level)
	{
		visitor.replaceNode(PASS1_post_map(visitor.dsqlScratch, this, visitor.context,
			visitor.partitionNode, visitor.orderNode));
	}

	return false;
}

void FieldNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = dsqlField->fld_name.c_str();

	if (dsqlContext->ctx_relation)
	{
		parameter->par_rel_name = dsqlContext->ctx_relation->rel_name.c_str();
		parameter->par_owner_name = dsqlContext->ctx_relation->rel_owner.c_str();
	}
	else if (dsqlContext->ctx_procedure)
	{
		parameter->par_rel_name = dsqlContext->ctx_procedure->prc_name.identifier.c_str();
		parameter->par_owner_name = dsqlContext->ctx_procedure->prc_owner.c_str();
	}

	parameter->par_rel_alias = dsqlContext->ctx_alias.c_str();
}

// Generate blr for a field - field id's are preferred but not for trigger or view blr.
void FieldNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	// For older clients - generate an error should they try and
	// access data types which did not exist in the older dialect.
	if (dsqlScratch->clientDialect <= SQL_DIALECT_V5)
	{
		switch (dsqlField->fld_dtype)
		{
			case dtype_sql_date:
			case dtype_sql_time:
			case dtype_int64:
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-804) <<
						  Arg::Gds(isc_dsql_datatype_err) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) <<
								Arg::Num(dsqlScratch->clientDialect) <<
								Arg::Str(DSC_dtype_tostring(static_cast<UCHAR>(dsqlField->fld_dtype))));
				break;

			default:
				// No special action for other data types
				break;
		}
	}

	if (dsqlIndices)
		dsqlScratch->appendUChar(blr_index);

	if (DDL_ids(dsqlScratch))
	{
		dsqlScratch->appendUChar(blr_fid);
		GEN_stuff_context(dsqlScratch, dsqlContext);
		dsqlScratch->appendUShort(dsqlField->fld_id);
	}
	else
	{
		dsqlScratch->appendUChar(blr_field);
		GEN_stuff_context(dsqlScratch, dsqlContext);
		dsqlScratch->appendMetaString(dsqlField->fld_name.c_str());
	}

	if (dsqlIndices)
	{
		dsqlScratch->appendUChar(dsqlIndices->nod_count);
		dsql_nod** ptr = dsqlIndices->nod_arg;

		for (const dsql_nod* const* end = ptr + dsqlIndices->nod_count; ptr != end; ++ptr)
			GEN_expr(dsqlScratch, *ptr);
	}
}

void FieldNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	if (dsqlDesc.dsc_dtype)
		*desc = dsqlDesc;
	else
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-203) <<
				  Arg::Gds(isc_dsql_field_ref));
	}
}

bool FieldNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const FieldNode* o = other->as<FieldNode>();
	fb_assert(o);

	if (dsqlField != o->dsqlField || dsqlContext != o->dsqlContext)
		return false;

	if (dsqlIndices || o->dsqlIndices)
		return PASS1_node_match(dsqlIndices, o->dsqlIndices, ignoreMapCast);

	return true;
}

bool FieldNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other) /*const*/
{
	CastNode* castNode = other->as<CastNode>();

	if (castNode)
	{
		dsc desc1, desc2;
		getDesc(tdbb, csb, &desc1);
		castNode->getDesc(tdbb, csb, &desc2);

		if (DSC_EQUIV(&desc1, &desc2, true) && sameAs(tdbb, csb, castNode->source))
			return true;
	}

	const FieldNode* o = other->as<FieldNode>();

	if (o)
	{
		CompilerScratch::csb_repeat* const tail = &csb->csb_rpt[fieldStream];
		CompilerScratch::csb_repeat* const other_tail = &csb->csb_rpt[o->fieldStream];

		return fieldId == o->fieldId &&
			(fieldStream == o->fieldStream ||
			(tail->csb_relation && other_tail->csb_relation &&
				tail->csb_relation->rel_id == other_tail->csb_relation->rel_id) ||
			(tail->csb_procedure && other_tail->csb_procedure &&
				tail->csb_procedure->getId() == other_tail->csb_procedure->getId()));
	}

	return false;
}

bool FieldNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	if (allowOnlyCurrentStream)
	{
		if (fieldStream != stream && !(csb->csb_rpt[fieldStream].csb_flags & csb_sub_stream))
			return false;
	}
	else
	{
		if (fieldStream == stream)
			return false;
	}

	return csb->csb_rpt[fieldStream].csb_flags & csb_active;
}

void FieldNode::findDependentFromStreams(const OptimizerRetrieval* optRet, SortedStreamList* streamList)
{
	// dimitr: OLD/NEW contexts shouldn't create any stream dependencies.

	if (fieldStream != optRet->stream &&
		(optRet->csb->csb_rpt[fieldStream].csb_flags & csb_active) &&
		!(optRet->csb->csb_rpt[fieldStream].csb_flags & csb_trigger))
	{
		if (!streamList->exist(fieldStream))
			streamList->add(fieldStream);
	}
}

void FieldNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	const Format* const format = CMP_format(tdbb, csb, fieldStream);

	if (fieldId >= format->fmt_count)
	{
		desc->clear();
	}
	else
	{
		*desc = format->fmt_desc[fieldId];

		// Fix UNICODE_FSS wrong length used in system tables.
		jrd_rel* relation = csb->csb_rpt[fieldStream].csb_relation;

		if (relation && (relation->rel_flags & REL_system) &&
			desc->isText() && desc->getCharSet() == CS_UNICODE_FSS)
		{
			USHORT adjust = 0;

			if (desc->dsc_dtype == dtype_varying)
				adjust = sizeof(USHORT);
			else if (desc->dsc_dtype == dtype_cstring)
				adjust = 1;

			desc->dsc_length -= adjust;
			desc->dsc_length *= 3;
			desc->dsc_length += adjust;
		}
	}
}

ValueExprNode* FieldNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	USHORT fldId = copier.getFieldId(this);
	USHORT stream = fieldStream;

	fldId = copier.remapField(stream, fldId);

	if (copier.remap)
	{
#ifdef CMP_DEBUG
		csb->dump("remap nod_field: %d -> %d\n", stream, copier.remap[stream]);
#endif
		stream = copier.remap[stream];
	}

	return PAR_gen_field(tdbb, stream, fldId, byId);
}

ValueExprNode* FieldNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	USHORT stream = fieldStream;

	CMP_mark_variant(csb, stream);

	CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
	jrd_rel* relation = tail->csb_relation;
	jrd_fld* field;

	if (!relation || !(field = MET_get_field(relation, fieldId)))
		return ValueExprNode::pass1(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);

	const USHORT ttype = INTL_TEXT_TYPE(desc);

	// Are we using a collation?
	if (TTYPE_TO_COLLATION(ttype) != 0)
	{
		Collation* collation = NULL;

		try
		{
			ThreadStatusGuard local_status(tdbb);
			collation = INTL_texttype_lookup(tdbb, ttype);
		}
		catch (Exception&)
		{
			// ASF: Swallow the exception if we fail to load the collation here.
			// This allows us to backup databases when the collation isn't available.
			if (!(tdbb->getAttachment()->att_flags & ATT_gbak_attachment))
				throw;
		}

		if (collation)
			CMP_post_resource(&csb->csb_resources, collation, Resource::rsc_collation, ttype);
	}

	// if this is a modify or store, check REFERENCES access to any foreign keys

	/* CVC: This is against the SQL standard. REFERENCES should be enforced only at the
		time the FK is defined in DDL, not when a DML is going to be executed.
	if (((tail->csb_flags & csb_modify) || (tail->csb_flags & csb_store)) &&
		!(relation->rel_view_rse || relation->rel_file))
	{
		IDX_check_access(tdbb, csb, tail->csb_view, relation);
	}
	*/

	// posting the required privilege access to the current relation and field

	// If this is in a "validate_subtree" then we must not
	// post access checks to the table and the fields in the table.
	// If any node of the parse tree is a nod_validate type node,
	// the nodes in the subtree are involved in a validation
	// clause only, the subtree is a validate_subtree in our notation.

	const SLONG viewId = tail->csb_view ?
		tail->csb_view->rel_id : (csb->csb_view ? csb->csb_view->rel_id : 0);

	if (tail->csb_flags & csb_modify)
	{
		if (!csb->csb_validate_expr)
		{
			CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
				SCL_sql_update, SCL_object_table, relation->rel_name);
			CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
				SCL_sql_update, SCL_object_column, field->fld_name, relation->rel_name);
		}
	}
	else if (tail->csb_flags & csb_erase)
	{
		CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
			SCL_sql_delete, SCL_object_table, relation->rel_name);
	}
	else if (tail->csb_flags & csb_store)
	{
		CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
			SCL_sql_insert, SCL_object_table, relation->rel_name);
		CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
			SCL_sql_insert, SCL_object_column, field->fld_name, relation->rel_name);
	}
	else
	{
		CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
			SCL_read, SCL_object_table, relation->rel_name);
		CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
			SCL_read, SCL_object_column, field->fld_name, relation->rel_name);
	}

	ValueExprNode* sub;

	if (!(sub = field->fld_computation) && !(sub = field->fld_source))
	{

		if (!relation->rel_view_rse)
			return ValueExprNode::pass1(tdbb, csb);

		// Msg 364 "cannot access column %s in view %s"
		ERR_post(Arg::Gds(isc_no_field_access) << Arg::Str(field->fld_name) <<
												  Arg::Str(relation->rel_name));
	}

	// The previous test below is an apparent temporary fix
	// put in by Root & Harrison in Summer/Fall 1991.
	// Old Code:
	// if (tail->csb_flags & (csb_view_update | csb_trigger))
	//   return ValueExprNode::pass1(tdbb, csb);
	// If the field is a computed field - we'll go on and make
	// the substitution.
	// Comment 1994-August-08 David Schnepper

	if (tail->csb_flags & (csb_view_update | csb_trigger))
	{
		// dimitr:	added an extra check for views, because we don't
		//			want their old/new contexts to be substituted
		if (relation->rel_view_rse || !field->fld_computation)
			return ValueExprNode::pass1(tdbb, csb);
	}

	UCHAR local_map[JrdStatement::MAP_LENGTH];
	UCHAR* map = tail->csb_map;

	if (!map)
	{
		map = local_map;
		fb_assert(stream + 2u <= MAX_STREAMS);
		local_map[0] = (UCHAR) stream;
		map[1] = stream + 1;
		map[2] = stream + 2;
	}

	AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
		(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

	sub = NodeCopier::copy(tdbb, csb, sub, map);

	if (relation->rel_view_rse)
	{
		// dimitr:	if we reference view columns, we need to pass them
		//			as belonging to a view (in order to compute the access
		//			permissions properly).
		AutoSetRestore<jrd_rel*> autoView(&csb->csb_view, relation);
		AutoSetRestore<USHORT> autoViewStream(&csb->csb_view_stream, stream);

		// ASF: If the view field doesn't reference an item of a stream, evaluate it
		// based on the view dbkey - CORE-1245.
		RecordKeyNode* recNode;

		if (!sub->is<FieldNode>() &&
			(!(recNode = sub->as<RecordKeyNode>()) || recNode->blrOp != blr_dbkey))
		{
			ValueExprNodeStack stack;
			CMP_expand_view_nodes(tdbb, csb, stream, stack, blr_dbkey, true);
			const UCHAR streamCount = (UCHAR) stack.getCount();

			if (streamCount)
			{
				DerivedExprNode* derivedNode =
					FB_NEW(*tdbb->getDefaultPool()) DerivedExprNode(*tdbb->getDefaultPool());
				derivedNode->arg = sub;

				for (ValueExprNodeStack::iterator i(stack); i.hasData(); ++i)
					derivedNode->streamList.add(i.object()->as<RecordKeyNode>()->recStream);

				sub = derivedNode;
			}
		}

		doPass1(tdbb, csb, &sub);	// note: scope of AutoSetRestore
	}
	else
		doPass1(tdbb, csb, &sub);

	return sub;
}

ValueExprNode* FieldNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	// SMB_SET uses ULONG, not USHORT
	SBM_SET(tdbb->getDefaultPool(), &csb->csb_rpt[fieldStream].csb_fields, fieldId);

	if (csb->csb_rpt[fieldStream].csb_relation || csb->csb_rpt[fieldStream].csb_procedure)
		format = CMP_format(tdbb, csb, fieldStream);

	impureOffset = CMP_impure(csb, sizeof(impure_value_ex));

	return this;
}

dsc* FieldNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);

	record_param& rpb = request->req_rpb[fieldStream];
	Record* record = rpb.rpb_record;
	jrd_rel* relation = rpb.rpb_relation;

	// In order to "map a null to a default" value (in EVL_field()), the relation block is referenced.
	// Reference: Bug 10116, 10424

	if (!EVL_field(relation, record, fieldId, &impure->vlu_desc))
		return NULL;

	// ASF: CORE-1432 - If the record is not on the latest format, upgrade it.
	// AP: for fields that are missing in original format use record's one.
	if (format &&
		record->rec_format->fmt_version != format->fmt_version &&
		fieldId < format->fmt_desc.getCount() &&
		!DSC_EQUIV(&impure->vlu_desc, &format->fmt_desc[fieldId], true))
	{
		dsc desc = impure->vlu_desc;
		impure->vlu_desc = format->fmt_desc[fieldId];

		if (impure->vlu_desc.isText())
		{
			// Allocate a string block of sufficient size.
			VaryingString* string = impure->vlu_string;

			if (string && string->str_length < impure->vlu_desc.dsc_length)
			{
				delete string;
				string = NULL;
			}

			if (!string)
			{
				string = impure->vlu_string = FB_NEW_RPT(*tdbb->getDefaultPool(),
					impure->vlu_desc.dsc_length) VaryingString();
				string->str_length = impure->vlu_desc.dsc_length;
			}

			impure->vlu_desc.dsc_address = string->str_data;
		}
		else
			impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc;

		MOV_move(tdbb, &desc, &impure->vlu_desc);
	}

	if (!relation || !(relation->rel_flags & REL_system))
	{
		if (impure->vlu_desc.dsc_dtype == dtype_text)
			INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);
	}

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<GenIdNode> regGenIdNode(blr_gen_id);

GenIdNode::GenIdNode(MemoryPool& pool, bool aDialect1, const MetaName& aName, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_GEN_ID>(pool),
	  dialect1(aDialect1),
	  name(pool, aName),
	  dsqlArg(aArg),
	  arg(NULL),
	  id(0)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* GenIdNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	MetaName name;
	PAR_name(csb, name);

	const SLONG id = MET_lookup_generator(tdbb, name);
	if (id < 0)
		PAR_error(csb, Arg::Gds(isc_gennotdef) << Arg::Str(name));

	if (csb->csb_g_flags & csb_get_dependencies)
	{
		CompilerScratch::Dependency dependency(obj_generator);
		dependency.number = id;
		csb->csb_dependencies.push(dependency);
	}

	GenIdNode* node = FB_NEW(pool) GenIdNode(pool, (csb->csb_g_flags & csb_blr_version4), name);
	node->id = id;
	node->arg = PAR_parse_value(tdbb, csb);

	return node;
}

void GenIdNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("GenIdNode %s (%d)", name.c_str(), (dialect1 ? 1 : 3));
	ExprNode::print(text, nodes);
}

ValueExprNode* GenIdNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) GenIdNode(getPool(), dialect1, name,
		PASS1_node(dsqlScratch, dsqlArg));
}

void GenIdNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "GEN_ID";
}

bool GenIdNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar) const
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void GenIdNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_gen_id);
	dsqlScratch->appendNullString(name.c_str());
	GEN_expr(dsqlScratch, dsqlArg);
}

void GenIdNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1;
	MAKE_desc(dsqlScratch, &desc1, dsqlArg);

	if (dialect1)
		desc->makeLong(0);
	else
		desc->makeInt64(0);

	desc->setNullable(true);
}

void GenIdNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	if (dialect1)
		desc->makeLong(0);
	else
		desc->makeInt64(0);
}

ValueExprNode* GenIdNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	GenIdNode* node = FB_NEW(*tdbb->getDefaultPool()) GenIdNode(*tdbb->getDefaultPool(),
		dialect1, name);
	node->id = id;
	node->arg = copier.copy(tdbb, arg);
	return node;
}

bool GenIdNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const GenIdNode* o = other->as<GenIdNode>();
	fb_assert(o);

	return dialect1 == o->dialect1 && name == o->name;
}

bool GenIdNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	GenIdNode* otherNode = other->as<GenIdNode>();
	return otherNode && dialect1 == otherNode->dialect1 && id == otherNode->id;
}

ValueExprNode* GenIdNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* GenIdNode::execute(thread_db* tdbb, jrd_req* request) const
{
	request->req_flags &= ~req_null;

	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	const dsc* value = EVL_expr(tdbb, request, arg);

	if (request->req_flags & req_null)
		return NULL;

	if (dialect1)
	{
		SLONG temp = (SLONG) DPM_gen_id(tdbb, id, false, MOV_get_int64(value, 0));
		impure->make_long(temp);
	}
	else
	{
		SINT64 temp = DPM_gen_id(tdbb, id, false, MOV_get_int64(value, 0));
		impure->make_int64(temp);
	}

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<InternalInfoNode> regInternalInfoNode(blr_internal_info);

// CVC: If this list changes, gpre will need to be updated
const InternalInfoNode::InfoAttr InternalInfoNode::INFO_TYPE_ATTRIBUTES[MAX_INFO_TYPE] =
{
	{"<UNKNOWN>", 0},
	{"CURRENT_CONNECTION", 0},
	{"CURRENT_TRANSACTION", 0},
	{"GDSCODE", DsqlCompilerScratch::FLAG_BLOCK},
	{"SQLCODE", DsqlCompilerScratch::FLAG_BLOCK},
	{"ROW_COUNT", DsqlCompilerScratch::FLAG_BLOCK},
	{"INSERTING/UPDATING/DELETING", DsqlCompilerScratch::FLAG_TRIGGER},
	{"SQLSTATE", DsqlCompilerScratch::FLAG_BLOCK}
};

InternalInfoNode::InternalInfoNode(MemoryPool& pool, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_INTERNAL_INFO>(pool),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* InternalInfoNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	InternalInfoNode* node = FB_NEW(pool) InternalInfoNode(pool);

	const UCHAR* blrOffset = csb->csb_blr_reader.getPos();

	node->arg = PAR_parse_value(tdbb, csb);

	LiteralNode* literal = node->arg->as<LiteralNode>();

	if (!literal || literal->litDesc.dsc_dtype != dtype_long)
	{
		csb->csb_blr_reader.setPos(blrOffset + 1);	// PAR_syntax_error seeks 1 backward.
        PAR_syntax_error(csb, "integer literal");
	}

	return node;
}

void InternalInfoNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "InternalInfoNode";
	ExprNode::print(text, nodes);
}

void InternalInfoNode::setParameterName(dsql_par* parameter) const
{
	SLONG infoType = ExprNode::as<LiteralNode>(dsqlArg)->getSlong();
	parameter->par_name = parameter->par_alias = INFO_TYPE_ATTRIBUTES[infoType].alias;
}

void InternalInfoNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_internal_info);
	GEN_expr(dsqlScratch, dsqlArg);
}

void InternalInfoNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	InfoType infoType = static_cast<InfoType>(ExprNode::as<LiteralNode>(dsqlArg)->getSlong());

	if (infoType == INFO_TYPE_SQLSTATE)
		desc->makeText(FB_SQLSTATE_LENGTH, ttype_ascii);
	else
		desc->makeLong(0);
}

void InternalInfoNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	fb_assert(arg->is<LiteralNode>());

	dsc argDesc;
	arg->getDesc(tdbb, csb, &argDesc);
	fb_assert(argDesc.dsc_dtype == dtype_long);

	InfoType infoType = static_cast<InfoType>(*reinterpret_cast<SLONG*>(argDesc.dsc_address));

	if (infoType == INFO_TYPE_SQLSTATE)
		desc->makeText(FB_SQLSTATE_LENGTH, ttype_ascii);
	else
		desc->makeLong(0);
}

ValueExprNode* InternalInfoNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	InternalInfoNode* node = FB_NEW(*tdbb->getDefaultPool()) InternalInfoNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	return node;
}

ValueExprNode* InternalInfoNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Return a given element of the internal engine data.
dsc* InternalInfoNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	const dsc* value = EVL_expr(tdbb, request, arg);
	if (request->req_flags & req_null)
		return NULL;

	fb_assert(value->dsc_dtype == dtype_long);
	InfoType infoType = static_cast<InfoType>(*reinterpret_cast<SLONG*>(value->dsc_address));

	if (infoType == INFO_TYPE_SQLSTATE)
	{
		FB_SQLSTATE_STRING sqlstate;
		request->req_last_xcp.as_sqlstate(sqlstate);

		dsc desc;
		desc.makeText(FB_SQLSTATE_LENGTH, ttype_ascii, (UCHAR*) sqlstate);
		EVL_make_value(tdbb, &desc, impure);

		return &impure->vlu_desc;
	}

	SLONG result = 0;

	switch (infoType)
	{
		case INFO_TYPE_CONNECTION_ID:
			result = PAG_attachment_id(tdbb);
			break;
		case INFO_TYPE_TRANSACTION_ID:
			result = tdbb->getTransaction()->tra_number;
			break;
		case INFO_TYPE_GDSCODE:
			result = request->req_last_xcp.as_gdscode();
			break;
		case INFO_TYPE_SQLCODE:
			result = request->req_last_xcp.as_sqlcode();
			break;
		case INFO_TYPE_ROWS_AFFECTED:
			result = request->req_records_affected.getCount();
			break;
		case INFO_TYPE_TRIGGER_ACTION:
			result = request->req_trigger_action;
			break;
		default:
			BUGCHECK(232);	// msg 232 EVL_expr: invalid operation
	}

	dsc desc;
	desc.makeLong(0, &result);
	EVL_make_value(tdbb, &desc, impure);

	return &impure->vlu_desc;
}

ValueExprNode* InternalInfoNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	SLONG infoType = ExprNode::as<LiteralNode>(dsqlArg)->getSlong();
	const InfoAttr& attr = INFO_TYPE_ATTRIBUTES[infoType];

	if (attr.mask && !(dsqlScratch->flags & attr.mask))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
			// Token unknown
			Arg::Gds(isc_token_err) <<
			Arg::Gds(isc_random) << attr.alias);
	}

	return FB_NEW(getPool()) InternalInfoNode(getPool(), PASS1_node(dsqlScratch, dsqlArg));
}


//--------------------


static RegisterNode<LiteralNode> regLiteralNode(blr_literal);

LiteralNode::LiteralNode(MemoryPool& pool)
	: TypedNode<ValueExprNode, ExprNode::TYPE_LITERAL>(pool),
	  dsqlStr(NULL)
{
	litDesc.clear();
}

// Parse a literal value.
DmlNode* LiteralNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	LiteralNode* node = FB_NEW(pool) LiteralNode(pool);

	PAR_desc(tdbb, csb, &node->litDesc);

	UCHAR* p = FB_NEW(csb->csb_pool) UCHAR[node->litDesc.dsc_length];
	node->litDesc.dsc_address = p;
	node->litDesc.dsc_flags = 0;
	const UCHAR* q = csb->csb_blr_reader.getPos();
	USHORT l = node->litDesc.dsc_length;

	switch (node->litDesc.dsc_dtype)
	{
		case dtype_short:
			l = 2;
			*(SSHORT*) p = (SSHORT) gds__vax_integer(q, l);
			break;

		case dtype_long:
		case dtype_sql_date:
		case dtype_sql_time:
			l = 4;
			*(SLONG*) p = gds__vax_integer(q, l);
			break;

		case dtype_timestamp:
			l = 8;
			*(SLONG*) p = gds__vax_integer(q, 4);
			p += 4;
			q += 4;
			*(SLONG*) p = gds__vax_integer(q, 4);
			break;

		case dtype_int64:
			l = sizeof(SINT64);
			*(SINT64*) p = isc_portable_integer(q, l);
			break;

		case dtype_double:
		{
			SSHORT scale;
			UCHAR dtype;

			// The double literal could potentially be used for any numeric literal - the value is
			// passed as if it were a text string. Convert the numeric string to its binary value
			// (int64, long or double as appropriate).

			l = csb->csb_blr_reader.getWord();
			q = csb->csb_blr_reader.getPos();
			dtype = CVT_get_numeric(q, l, &scale, (double*) p);
			node->litDesc.dsc_dtype = dtype;

			switch (dtype)
			{
				case dtype_double:
					node->litDesc.dsc_length = sizeof(double);
					break;
				case dtype_long:
					node->litDesc.dsc_length = sizeof(SLONG);
					node->litDesc.dsc_scale = (SCHAR) scale;
					break;
				default:
					node->litDesc.dsc_length = sizeof(SINT64);
					node->litDesc.dsc_scale = (SCHAR) scale;
			}
			break;
		}

		case dtype_text:
			memcpy(p, q, l);
			break;

		case dtype_boolean:
			l = 1;
			*p = *q;
			break;

		default:
			fb_assert(FALSE);
	}

	csb->csb_blr_reader.seekForward(l);

	return node;
}

// Generate BLR for a constant.
void LiteralNode::genConstant(DsqlCompilerScratch* dsqlScratch, const dsc* desc, bool negateValue)
{
	SLONG value;
	SINT64 i64value;

	dsqlScratch->appendUChar(blr_literal);

	const UCHAR* p = desc->dsc_address;

	switch (desc->dsc_dtype)
	{
		case dtype_short:
			GEN_descriptor(dsqlScratch, desc, true);
			value = *(SSHORT*) p;
			if (negateValue)
				value = -value;
			dsqlScratch->appendUShort(value);
			break;

		case dtype_long:
			GEN_descriptor(dsqlScratch, desc, true);
			value = *(SLONG*) p;
			if (negateValue)
				value = -value;
			dsqlScratch->appendUShort(value);
			dsqlScratch->appendUShort(value >> 16);
			break;

		case dtype_sql_time:
		case dtype_sql_date:
			GEN_descriptor(dsqlScratch, desc, true);
			value = *(SLONG*) p;
			dsqlScratch->appendUShort(value);
			dsqlScratch->appendUShort(value >> 16);
			break;

		case dtype_double:
		{
			// this is used for approximate/large numeric literal
			// which is transmitted to the engine as a string.

			GEN_descriptor(dsqlScratch, desc, true);
			// Length of string literal, cast because it could be > 127 bytes.
			const USHORT l = (USHORT)(UCHAR) desc->dsc_scale;

			if (negateValue)
			{
				dsqlScratch->appendUShort(l + 1);
				dsqlScratch->appendUChar('-');
			}
			else
				dsqlScratch->appendUShort(l);

			if (l)
				dsqlScratch->appendBytes(p, l);

			break;
		}

		case dtype_int64:
			i64value = *(SINT64*) p;

			if (negateValue)
				i64value = -i64value;
			else if (i64value == MIN_SINT64)
			{
				// UH OH!
				// yylex correctly recognized the digits as the most-negative
				// possible INT64 value, but unfortunately, there was no
				// preceding '-' (a fact which the lexer could not know).
				// The value is too big for a positive INT64 value, and it
				// didn't contain an exponent so it's not a valid DOUBLE
				// PRECISION literal either, so we have to bounce it.

				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_arith_except) <<
						  Arg::Gds(isc_numeric_out_of_range));
			}

			// We and the lexer both agree that this is an SINT64 constant,
			// and if the value needed to be negated, it already has been.
			// If the value will fit into a 32-bit signed integer, generate
			// it that way, else as an INT64.

			if ((i64value >= (SINT64) MIN_SLONG) && (i64value <= (SINT64) MAX_SLONG))
			{
				dsqlScratch->appendUChar(blr_long);
				dsqlScratch->appendUChar(desc->dsc_scale);
				dsqlScratch->appendUShort(i64value);
				dsqlScratch->appendUShort(i64value >> 16);
			}
			else
			{
				dsqlScratch->appendUChar(blr_int64);
				dsqlScratch->appendUChar(desc->dsc_scale);
				dsqlScratch->appendUShort(i64value);
				dsqlScratch->appendUShort(i64value >> 16);
				dsqlScratch->appendUShort(i64value >> 32);
				dsqlScratch->appendUShort(i64value >> 48);
			}
			break;

		case dtype_quad:
		case dtype_blob:
		case dtype_array:
		case dtype_timestamp:
			GEN_descriptor(dsqlScratch, desc, true);
			value = *(SLONG*) p;
			dsqlScratch->appendUShort(value);
			dsqlScratch->appendUShort(value >> 16);
			value = *(SLONG*) (p + 4);
			dsqlScratch->appendUShort(value);
			dsqlScratch->appendUShort(value >> 16);
			break;

		case dtype_text:
		{
			const USHORT length = desc->dsc_length;

			GEN_descriptor(dsqlScratch, desc, true);
			if (length)
				dsqlScratch->appendBytes(p, length);

			break;
		}

		case dtype_boolean:
			GEN_descriptor(dsqlScratch, desc, false);
			dsqlScratch->appendUChar(*p != 0);
			break;

		default:
			// gen_constant: datatype not understood
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-103) <<
					  Arg::Gds(isc_dsql_constant_err));
	}
}

void LiteralNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("LiteralNode");
	ExprNode::print(text, nodes);
}

// Turn an international string reference into internal subtype ID.
ValueExprNode* LiteralNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	thread_db* tdbb = JRD_get_thread_data();

	if (dsqlScratch->inOuterJoin)
		litDesc.dsc_flags = DSC_nullable;

	if (litDesc.dsc_dtype > dtype_any_text)
		return this;

	LiteralNode* constant = FB_NEW(getPool()) LiteralNode(getPool());
	constant->dsqlStr = dsqlStr;
	constant->litDesc = litDesc;

	const dsql_str* string = constant->dsqlStr;

	if (string && string->str_charset)
	{
		const dsql_intlsym* resolved = METD_get_charset(dsqlScratch->getTransaction(),
			strlen(string->str_charset), string->str_charset);

		if (!resolved)
		{
			// character set name is not defined
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-504) <<
					  Arg::Gds(isc_charset_not_found) << Arg::Str(string->str_charset));
		}

		constant->litDesc.setTextType(resolved->intlsym_ttype);
	}
	else
	{
		const MetaName charSetName = METD_get_charset_name(
			dsqlScratch->getTransaction(), constant->litDesc.getCharSet());

		const dsql_intlsym* sym = METD_get_charset(dsqlScratch->getTransaction(),
			charSetName.length(), charSetName.c_str());
		fb_assert(sym);

		if (sym)
			constant->litDesc.setTextType(sym->intlsym_ttype);
	}

	USHORT adjust = 0;

	if (constant->litDesc.dsc_dtype == dtype_varying)
		adjust = sizeof(USHORT);
	else if (constant->litDesc.dsc_dtype == dtype_cstring)
		adjust = 1;

	constant->litDesc.dsc_length -= adjust;

	CharSet* charSet = INTL_charset_lookup(tdbb, INTL_GET_CHARSET(&constant->litDesc));

	if (!charSet->wellFormed(string->str_length, constant->litDesc.dsc_address, NULL))
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  Arg::Gds(isc_malformed_string));
	}
	else
	{
		constant->litDesc.dsc_length =
			charSet->length(string->str_length, constant->litDesc.dsc_address, true) *
			charSet->maxBytesPerChar();
	}

	constant->litDesc.dsc_length += adjust;

	return constant;
}

void LiteralNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CONSTANT";
}

bool LiteralNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return false;
}

void LiteralNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (litDesc.dsc_dtype == dtype_text)
		litDesc.dsc_length = dsqlStr->str_length;

	genConstant(dsqlScratch, &litDesc, false);
}

void LiteralNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	*desc = litDesc;
}

void LiteralNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	*desc = litDesc;

	// ASF: I expect only dtype_text could occur here.
	// But I'll treat all string types for sure.
	if (DTYPE_IS_TEXT(desc->dsc_dtype))
	{
		const UCHAR* p;
		USHORT adjust = 0;

		if (desc->dsc_dtype == dtype_varying)
		{
			p = desc->dsc_address + sizeof(USHORT);
			adjust = sizeof(USHORT);
		}
		else
		{
			p = desc->dsc_address;

			if (desc->dsc_dtype == dtype_cstring)
				adjust = 1;
		}

		// Do the same thing which DSQL does.
		// Increase descriptor size to evaluate dependent expressions correctly.
		CharSet* cs = INTL_charset_lookup(tdbb, desc->getCharSet());
		desc->dsc_length = (cs->length(desc->dsc_length - adjust, p, true) *
			cs->maxBytesPerChar()) + adjust;
	}
}

ValueExprNode* LiteralNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	LiteralNode* node = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());
	node->litDesc = litDesc;

	UCHAR* p = FB_NEW(*tdbb->getDefaultPool()) UCHAR[node->litDesc.dsc_length];
	node->litDesc.dsc_address = p;

	memcpy(p, litDesc.dsc_address, litDesc.dsc_length);

	return node;
}

bool LiteralNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const LiteralNode* o = other->as<LiteralNode>();
	fb_assert(o);

	if (litDesc.dsc_dtype != o->litDesc.dsc_dtype || litDesc.dsc_length != o->litDesc.dsc_length ||
		litDesc.dsc_scale != o->litDesc.dsc_scale)
	{
		return false;
	}

	return memcmp(litDesc.dsc_address, o->litDesc.dsc_address, litDesc.dsc_length) == 0;
}

bool LiteralNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	LiteralNode* o = other->as<LiteralNode>();
	fb_assert(o);

	dsc desc1, desc2;

	getDesc(tdbb, csb, &desc1);
	o->getDesc(tdbb, csb, &desc2);

	return DSC_EQUIV(&desc1, &desc2, true) &&
		memcmp(desc1.dsc_address, desc2.dsc_address, desc1.dsc_length) == 0;
}

ValueExprNode* LiteralNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* LiteralNode::execute(thread_db* tdbb, jrd_req* request) const
{
	return const_cast<dsc*>(&litDesc);
}


//--------------------


void DsqlAliasNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("DsqlAliasNode");
	ExprNode::print(text, nodes);
}

ValueExprNode* DsqlAliasNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	DsqlAliasNode* node = FB_NEW(getPool()) DsqlAliasNode(getPool(), name,
		PASS1_node(dsqlScratch, value));
	MAKE_desc(dsqlScratch, &node->value->nod_desc, node->value);
	return node;
}

void DsqlAliasNode::setParameterName(dsql_par* parameter) const
{
	const ValueExprNode* exprNode;

	if ((exprNode = ExprNode::as<RecordKeyNode>(value)) || (exprNode = ExprNode::as<FieldNode>(value)))
		exprNode->setParameterName(parameter);

	parameter->par_alias = name;
}

void DsqlAliasNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	GEN_expr(dsqlScratch, value);
}

void DsqlAliasNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, value);
}


//--------------------


DsqlMapNode::DsqlMapNode(MemoryPool& pool, dsql_ctx* aContext, dsql_map* aMap)
	: TypedNode<ValueExprNode, ExprNode::TYPE_MAP>(pool),
	  context(aContext),
	  map(aMap)
{
}

void DsqlMapNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("DsqlMapNode");
	ExprNode::print(text, nodes);
}

ValueExprNode* DsqlMapNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) DsqlMapNode(getPool(), context, map);
}

bool DsqlMapNode::dsqlAggregateFinder(AggregateFinder& visitor)
{
	if (visitor.window)
		return false;

	if (context->ctx_scope_level == visitor.dsqlScratch->scopeLevel)
		return true;

	return visitor.visit(&map->map_node);
}

bool DsqlMapNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	return visitor.visit(&map->map_node);
}

bool DsqlMapNode::dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
{
	// If that map is of the current scopeLevel, we prevent the visiting of the aggregate
	// expression. This is because a field embedded in an aggregate function is valid even
	// not being in the group by list. Examples:
	//   select count(n) from table group by m
	//   select count(n) from table

	AutoSetRestore<bool> autoInsideOwnMap(&visitor.insideOwnMap,
		context->ctx_scope_level == visitor.context->ctx_scope_level);

	// If the context scope is greater than our own, someone should have already inspected
	// nested aggregates, so set insideHigherMap to true.

	AutoSetRestore<bool> autoInsideHigherMap(&visitor.insideHigherMap,
		context->ctx_scope_level > visitor.context->ctx_scope_level);

	return visitor.visit(&map->map_node);
}

bool DsqlMapNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return false;
}

bool DsqlMapNode::dsqlFieldFinder(FieldFinder& visitor)
{
	return visitor.visit(&map->map_node);
}

bool DsqlMapNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	if (context->ctx_scope_level != visitor.context->ctx_scope_level)
	{
		AutoSetRestore<USHORT> autoCurrentLevel(&visitor.currentLevel, context->ctx_scope_level);
		visitor.visit(&map->map_node);
	}

	if (visitor.window && context->ctx_scope_level == visitor.context->ctx_scope_level)
	{
		visitor.replaceNode(PASS1_post_map(visitor.dsqlScratch, this,
			visitor.context, visitor.partitionNode, visitor.orderNode));
	}

	return false;
}

void DsqlMapNode::setParameterName(dsql_par* parameter) const
{
	const dsql_nod* nestNode = map->map_node;
	const DsqlMapNode* mapNode;

	while ((mapNode = ExprNode::as<DsqlMapNode>(nestNode)))
	{
		// Skip all the DsqlMapNodes.
		nestNode = mapNode->map->map_node;
	}

	const char* nameAlias = NULL;
	const FieldNode* fieldNode = NULL;
	const dsql_nod* alias;

	const AggNode* aggNode;
	const DsqlAliasNode* aliasNode;
	const LiteralNode* literalNode;
	const DerivedFieldNode* derivedField;

	if ((aggNode = ExprNode::as<AggNode>(nestNode)))
		aggNode->setParameterName(parameter);
	else if ((aliasNode = ExprNode::as<DsqlAliasNode>(nestNode)))
	{
		parameter->par_alias = aliasNode->name;
		alias = aliasNode->value;
		fieldNode = ExprNode::as<FieldNode>(alias);
	}
	else if ((literalNode = ExprNode::as<LiteralNode>(nestNode)))
		literalNode->setParameterName(parameter);
	else if (ExprNode::is<RecordKeyNode>(nestNode))
		nameAlias = DB_KEY_NAME;
	else if ((derivedField = ExprNode::as<DerivedFieldNode>(nestNode)))
	{
		parameter->par_alias = derivedField->name;
		alias = derivedField->dsqlValue;
		fieldNode = ExprNode::as<FieldNode>(alias);
	}
	else if ((fieldNode = ExprNode::as<FieldNode>(nestNode)))
		nameAlias = fieldNode->dsqlField->fld_name.c_str();

	const dsql_ctx* context = NULL;
	const dsql_fld* field;

	if (fieldNode)
	{
		context = fieldNode->dsqlContext;
		field = fieldNode->dsqlField;
		parameter->par_name = field->fld_name.c_str();
	}

	if (nameAlias)
		parameter->par_name = parameter->par_alias = nameAlias;

	if (context)
	{
		if (context->ctx_relation)
		{
			parameter->par_rel_name = context->ctx_relation->rel_name.c_str();
			parameter->par_owner_name = context->ctx_relation->rel_owner.c_str();
		}
		else if (context->ctx_procedure)
		{
			parameter->par_rel_name = context->ctx_procedure->prc_name.identifier.c_str();
			parameter->par_owner_name = context->ctx_procedure->prc_owner.c_str();
		}

		parameter->par_rel_alias = context->ctx_alias.c_str();
	}
}

void DsqlMapNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_fid);

	if (map->map_partition)
		dsqlScratch->appendUChar(map->map_partition->context);
	else
		GEN_stuff_context(dsqlScratch, context);

	dsqlScratch->appendUShort(map->map_position);
}

void DsqlMapNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, map->map_node);

	// ASF: We should mark nod_agg_count as nullable when it's in an outer join - CORE-2660.
	if (context->ctx_flags & CTX_outer_join)
		desc->setNullable(true);
}

bool DsqlMapNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	const DsqlMapNode* o = other->as<DsqlMapNode>();
	return o && PASS1_node_match(map->map_node, o->map->map_node, ignoreMapCast);
}


//--------------------


DerivedFieldNode::DerivedFieldNode(MemoryPool& pool, const MetaName& aName, USHORT aScope,
			dsql_nod* aDsqlValue)
	: TypedNode<ValueExprNode, ExprNode::TYPE_DERIVED_FIELD>(pool),
	  name(aName),
	  scope(aScope),
	  dsqlValue(aDsqlValue),
	  context(NULL)
{
	addChildNode(dsqlValue);
}

void DerivedFieldNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("DerivedFieldNode");
	ExprNode::print(text, nodes);
}

ValueExprNode* DerivedFieldNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return this;
}

bool DerivedFieldNode::dsqlAggregateFinder(AggregateFinder& visitor)
{
	// This is a derived table, so don't look further, but don't forget to check for the
	// deepest scope level.

	if (visitor.deepestLevel < scope)
		visitor.deepestLevel = scope;

	return false;
}

bool DerivedFieldNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	return false;
}

bool DerivedFieldNode::dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
{
	if (scope == visitor.context->ctx_scope_level)
		return true;

	if (visitor.context->ctx_scope_level < scope)
		return visitor.visit(&dsqlValue);

	return false;
}

bool DerivedFieldNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return false;
}

bool DerivedFieldNode::dsqlFieldFinder(FieldFinder& visitor)
{
	// This is a "virtual" field
	visitor.field = true;

	const USHORT dfScopeLevel = scope;

	switch (visitor.matchType)
	{
		case FIELD_MATCH_TYPE_EQUAL:
			return dfScopeLevel == visitor.checkScopeLevel;

		case FIELD_MATCH_TYPE_LOWER:
			return dfScopeLevel < visitor.checkScopeLevel;

		case FIELD_MATCH_TYPE_LOWER_EQUAL:
			return dfScopeLevel <= visitor.checkScopeLevel;

		///case FIELD_MATCH_TYPE_HIGHER:
		///	return dfScopeLevel > visitor.checkScopeLevel;

		///case FIELD_MATCH_TYPE_HIGHER_EQUAL:
		///	return dfScopeLevel >= visitor.checkScopeLevel;

		default:
			fb_assert(false);
	}

	return false;
}

bool DerivedFieldNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	// If we got a field from a derived table we should not remap anything
	// deeper in the alias, but this "virtual" field should be mapped to
	// the given context (of course only if we're in the same scope-level).

	if (scope == visitor.context->ctx_scope_level)
	{
		visitor.replaceNode(PASS1_post_map(visitor.dsqlScratch, this,
			visitor.context, visitor.partitionNode, visitor.orderNode));
	}
	else if (visitor.context->ctx_scope_level < scope)
		visitor.visit(&dsqlValue);

	return false;
}

void DerivedFieldNode::setParameterName(dsql_par* parameter) const
{
	const dsql_ctx* context = NULL;
	const FieldNode* fieldNode;
	const RecordKeyNode* dbKeyNode;

	if ((fieldNode = ExprNode::as<FieldNode>(dsqlValue)))
	{
		parameter->par_name = fieldNode->dsqlField->fld_name.c_str();
		context = fieldNode->dsqlContext;
	}
	else if ((dbKeyNode = ExprNode::as<RecordKeyNode>(dsqlValue)))
		dbKeyNode->setParameterName(parameter);

	parameter->par_alias = name;

	if (context)
	{
		if (context->ctx_relation)
		{
			parameter->par_rel_name = context->ctx_relation->rel_name.c_str();
			parameter->par_owner_name = context->ctx_relation->rel_owner.c_str();
		}
		else if (context->ctx_procedure)
		{
			parameter->par_rel_name = context->ctx_procedure->prc_name.identifier.c_str();
			parameter->par_owner_name = context->ctx_procedure->prc_owner.c_str();
		}

		parameter->par_rel_alias = context->ctx_alias.c_str();
	}
}

void DerivedFieldNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	// ASF: If we are not referencing a field, we should evaluate the expression based on
	// a set (ORed) of contexts. If any of them are in a valid position the expression is
	// evaluated, otherwise a NULL will be returned. This is fix for CORE-1246.

	if (!ExprNode::is<FieldNode>(dsqlValue) &&
		!ExprNode::is<DerivedFieldNode>(dsqlValue) &&
		!ExprNode::is<RecordKeyNode>(dsqlValue) &&
		!ExprNode::is<DsqlMapNode>(dsqlValue))
	{
		if (context->ctx_main_derived_contexts.hasData())
		{
			if (context->ctx_main_derived_contexts.getCount() > MAX_UCHAR)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
						  Arg::Gds(isc_imp_exc) <<
						  Arg::Gds(isc_ctx_too_big));
			}

			dsqlScratch->appendUChar(blr_derived_expr);
			dsqlScratch->appendUChar(context->ctx_main_derived_contexts.getCount());

			for (DsqlContextStack::const_iterator stack(context->ctx_main_derived_contexts);
				 stack.hasData(); ++stack)
			{
				fb_assert(stack.object()->ctx_context <= MAX_UCHAR);
				dsqlScratch->appendUChar(stack.object()->ctx_context);
			}
		}
	}

	GEN_expr(dsqlScratch, dsqlValue);
}

void DerivedFieldNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, dsqlValue);
}


//--------------------


static RegisterNode<NegateNode> regNegateNode(blr_negate);

NegateNode::NegateNode(MemoryPool& pool, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_NEGATE>(pool),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* NegateNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	NegateNode* node = FB_NEW(pool) NegateNode(pool);
	node->arg = PAR_parse_value(tdbb, csb);
	return node;
}

void NegateNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "NegateNode";
	ExprNode::print(text, nodes);
}

void NegateNode::setParameterName(dsql_par* parameter) const
{
	// CVC: For this to be a thorough check, we need to recurse over all nodes.
	// This means we should separate the code that sets aliases from
	// the rest of the functionality here in MAKE_parameter_names().
	// Otherwise, we need to test here for most of the other node types.
	// However, we need to be recursive only if we agree things like -gen_id()
	// should be given the GEN_ID alias, too.
	int level = 0;
	const dsql_nod* innerNode = dsqlArg;
	const NegateNode* innerNegateNode;

	while ((innerNegateNode = ExprNode::as<NegateNode>(innerNode)))
	{
		innerNode = innerNegateNode->dsqlArg;
		++level;
	}

	switch (innerNode->nod_type)
	{
		case Dsql::nod_class_exprnode:
		{
			if (ExprNode::is<NullNode>(innerNode) || ExprNode::is<LiteralNode>(innerNode))
				parameter->par_name = parameter->par_alias = "CONSTANT";
			else if (!level)
			{
				const ArithmeticNode* arithmeticNode = ExprNode::as<ArithmeticNode>(innerNode);

				if (arithmeticNode && (
					/*arithmeticNode->blrOp == blr_add ||
					arithmeticNode->blrOp == blr_subtract ||*/
					arithmeticNode->blrOp == blr_multiply ||
					arithmeticNode->blrOp == blr_divide))
				{
					parameter->par_name = parameter->par_alias = arithmeticNode->label.c_str();
				}
			}

			break;
		}
	}
}

bool NegateNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void NegateNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	LiteralNode* literal = ExprNode::as<LiteralNode>(dsqlArg);

	if (literal && DTYPE_IS_NUMERIC(literal->litDesc.dsc_dtype))
		LiteralNode::genConstant(dsqlScratch, &literal->litDesc, true);
	else
	{
		dsqlScratch->appendUChar(blr_negate);
		GEN_expr(dsqlScratch, dsqlArg);
	}
}

void NegateNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, dsqlArg);

	if (ExprNode::is<NullNode>(dsqlArg))
	{
		// -NULL = NULL of INT
		desc->makeLong(0);
		desc->setNullable(true);
	}
	else
	{
		// In Dialect 2 or 3, a string can never partipate in negation
		// (use a specific cast instead)
		if (DTYPE_IS_TEXT(desc->dsc_dtype))
		{
			if (dsqlScratch->clientDialect >= SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_expression_eval_err) <<
						  Arg::Gds(isc_dsql_nostring_neg_dial3));
			}

			desc->dsc_dtype = dtype_double;
			desc->dsc_length = sizeof(double);
		}
		else if (DTYPE_IS_BLOB(desc->dsc_dtype))	// Forbid blobs and arrays
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
					  Arg::Gds(isc_dsql_no_blob_array));
		}
		else if (!DTYPE_IS_NUMERIC(desc->dsc_dtype))	// Forbid other not numeric datatypes
		{
			ERRD_post(Arg::Gds(isc_expression_eval_err) <<
					  Arg::Gds(isc_dsql_invalid_type_neg));
		}
	}
}

void NegateNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	arg->getDesc(tdbb, csb, desc);
	nodFlags = arg->nodFlags & (FLAG_DOUBLE | FLAG_QUAD);
}

ValueExprNode* NegateNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	NegateNode* node = FB_NEW(*tdbb->getDefaultPool()) NegateNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	return node;
}

ValueExprNode* NegateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* NegateNode::execute(thread_db* tdbb, jrd_req* request) const
{
	request->req_flags &= ~req_null;

	const dsc* desc = EVL_expr(tdbb, request, arg);
	if (request->req_flags & req_null)
		return NULL;

	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	EVL_make_value(tdbb, desc, impure);

	switch (impure->vlu_desc.dsc_dtype)
	{
		case dtype_short:
			if (impure->vlu_misc.vlu_short == MIN_SSHORT)
				ERR_post(Arg::Gds(isc_exception_integer_overflow));
			impure->vlu_misc.vlu_short = -impure->vlu_misc.vlu_short;
			break;

		case dtype_long:
			if (impure->vlu_misc.vlu_long == MIN_SLONG)
				ERR_post(Arg::Gds(isc_exception_integer_overflow));
			impure->vlu_misc.vlu_long = -impure->vlu_misc.vlu_long;
			break;

		case dtype_real:
			impure->vlu_misc.vlu_float = -impure->vlu_misc.vlu_float;
			break;

		case DEFAULT_DOUBLE:
			impure->vlu_misc.vlu_double = -impure->vlu_misc.vlu_double;
			break;

		case dtype_quad:
			impure->vlu_misc.vlu_quad =
				QUAD_NEGATE(impure->vlu_misc.vlu_quad, ERR_post);
			break;

		case dtype_int64:
			if (impure->vlu_misc.vlu_int64 == MIN_SINT64)
				ERR_post(Arg::Gds(isc_exception_integer_overflow));
			impure->vlu_misc.vlu_int64 = -impure->vlu_misc.vlu_int64;
			break;

		default:
			impure->vlu_misc.vlu_double = -MOV_get_double(&impure->vlu_desc);
			impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
			impure->vlu_desc.dsc_length = sizeof(double);
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_double;
	}

	return &impure->vlu_desc;
}

ValueExprNode* NegateNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) NegateNode(getPool(), PASS1_node(dsqlScratch, dsqlArg));
}


//--------------------


static RegisterNode<NullNode> regNullNode(blr_null);

DmlNode* NullNode::parse(thread_db* /*tdbb*/, MemoryPool& pool, CompilerScratch* /*csb*/,
	UCHAR /*blrOp*/)
{
	return FB_NEW(pool) NullNode(pool);
}

void NullNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("NullNode");
	ExprNode::print(text, nodes);
}

void NullNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CONSTANT";
}

void NullNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_null);
}

void NullNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	// This occurs when SQL statement specifies a literal NULL, eg:
	//  SELECT NULL FROM TABLE1;
	// As we don't have a <dtype_null, SQL_NULL> datatype pairing,
	// we don't know how to map this NULL to a host-language
	// datatype.  Therefore we now describe it as a
	// CHAR(1) CHARACTER SET NONE type.
	// No value will ever be sent back, as the value of the select
	// will be NULL - this is only for purposes of DESCRIBING
	// the statement.  Note that this mapping could be done in dsql.cpp
	// as part of the DESCRIBE statement - but I suspect other areas
	// of the code would break if this is declared dtype_unknown.
	//
	// ASF: We have SQL_NULL now, but don't use it here.

	desc->makeNullString();
}

void NullNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* desc)
{
	desc->makeLong(0);
	desc->setNull();
}

ValueExprNode* NullNode::copy(thread_db* tdbb, NodeCopier& /*copier*/) const
{
	return FB_NEW(*tdbb->getDefaultPool()) NullNode(*tdbb->getDefaultPool());
}

ValueExprNode* NullNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* NullNode::execute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
{
	return NULL;
}


//--------------------


OverNode::OverNode(MemoryPool& pool, dsql_nod* aAggExpr, dsql_nod* aPartition, dsql_nod* aOrder)
	: TypedNode<ValueExprNode, ExprNode::TYPE_OVER>(pool),
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

	AggregateFinder aggFinder(visitor.dsqlScratch, false);
	aggFinder.deepestLevel = visitor.dsqlScratch->scopeLevel;
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
		else if (visitor.dsqlScratch->scopeLevel == aggFinder.deepestLevel)
		{
			visitor.replaceNode(PASS1_post_map(visitor.dsqlScratch, copy, visitor.context,
				visitor.partitionNode, visitor.orderNode));
		}
	}

	return false;
}

void OverNode::setParameterName(dsql_par* parameter) const
{
	MAKE_parameter_names(parameter, dsqlAggExpr);
}

void OverNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	GEN_expr(dsqlScratch, dsqlAggExpr);
}

void OverNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, dsqlAggExpr);
	desc->setNullable(true);
}

void OverNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* /*desc*/)
{
	fb_assert(false);
}

ValueExprNode* OverNode::copy(thread_db* /*tdbb*/, NodeCopier& /*copier*/) const
{
	fb_assert(false);
	return NULL;
}

dsc* OverNode::execute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
{
	fb_assert(false);
	return NULL;
}

ValueExprNode* OverNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) OverNode(getPool(),
		PASS1_node(dsqlScratch, dsqlAggExpr),
		PASS1_node(dsqlScratch, dsqlPartition),
		PASS1_node(dsqlScratch, dsqlOrder));
}


//--------------------


static RegisterNode<ParameterNode> regParameterNode(blr_parameter);
static RegisterNode<ParameterNode> regParameterNode2(blr_parameter2);
static RegisterNode<ParameterNode> regParameterNode3(blr_parameter3);

ParameterNode::ParameterNode(MemoryPool& pool)
	: TypedNode<ValueExprNode, ExprNode::TYPE_PARAMETER>(pool),
	  dsqlParameterIndex(0),
	  dsqlParameter(NULL),
	  message(NULL),
	  argNumber(0),
	  argFlag(NULL),
	  argIndicator(NULL),
	  argInfo(NULL)
{
	addChildNode(argFlag);
	addChildNode(argIndicator);
}

DmlNode* ParameterNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	MessageNode* message = NULL;
	USHORT n = csb->csb_blr_reader.getByte();

	if (n >= csb->csb_rpt.getCount() || !(message = csb->csb_rpt[n].csb_message))
		PAR_error(csb, Arg::Gds(isc_badmsgnum));

	ParameterNode* node = FB_NEW(pool) ParameterNode(pool);

	node->message = message;
	node->argNumber = csb->csb_blr_reader.getWord();

	const Format* format = message->format;

	if (node->argNumber >= format->fmt_count)
		PAR_error(csb, Arg::Gds(isc_badparnum));

	if (blrOp != blr_parameter)
	{
		ParameterNode* flagNode = FB_NEW(pool) ParameterNode(pool);
		flagNode->message = message;
		flagNode->argNumber = csb->csb_blr_reader.getWord();

		if (flagNode->argNumber >= format->fmt_count)
			PAR_error(csb, Arg::Gds(isc_badparnum));

		node->argFlag = flagNode;
	}

	if (blrOp == blr_parameter3)
	{
		ParameterNode* indicatorNode = FB_NEW(pool) ParameterNode(pool);
		indicatorNode->message = message;
		indicatorNode->argNumber = csb->csb_blr_reader.getWord();

		if (indicatorNode->argNumber >= format->fmt_count)
			PAR_error(csb, Arg::Gds(isc_badparnum));

		node->argIndicator = indicatorNode;
	}

	return node;
}

void ParameterNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ParameterNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* ParameterNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	if (dsqlScratch->isPsql())
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
					Arg::Gds(isc_dsql_command_err));
	}

	dsql_msg* tempMsg = dsqlParameter ?
		dsqlParameter->par_message : dsqlScratch->getStatement()->getSendMsg();

	ParameterNode* node = FB_NEW(getPool()) ParameterNode(getPool());
	node->dsqlParameter = MAKE_parameter(tempMsg, true, true, dsqlParameterIndex, NULL);
	node->dsqlParameterIndex = dsqlParameterIndex;

	return node;
}

bool ParameterNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	thread_db* tdbb = JRD_get_thread_data();

	const dsc oldDesc = dsqlParameter->par_desc;

	if (!node)
		dsqlParameter->par_desc.makeNullString();
	else
	{
		MAKE_desc(dsqlScratch, &dsqlParameter->par_desc, node);

		if (tdbb->getCharSet() != CS_NONE && tdbb->getCharSet() != CS_BINARY)
		{
			const USHORT fromCharSet = dsqlParameter->par_desc.getCharSet();
			const USHORT toCharSet = (fromCharSet == CS_NONE || fromCharSet == CS_BINARY) ?
				fromCharSet : tdbb->getCharSet();

			if (dsqlParameter->par_desc.dsc_dtype <= dtype_any_text)
			{
				int diff = 0;

				switch (dsqlParameter->par_desc.dsc_dtype)
				{
					case dtype_varying:
						diff = sizeof(USHORT);
						break;
					case dtype_cstring:
						diff = 1;
						break;
				}

				dsqlParameter->par_desc.dsc_length -= diff;

				if (toCharSet != fromCharSet)
				{
					const USHORT fromCharSetBPC = METD_get_charset_bpc(
						dsqlScratch->getTransaction(), fromCharSet);
					const USHORT toCharSetBPC = METD_get_charset_bpc(
						dsqlScratch->getTransaction(), toCharSet);

					dsqlParameter->par_desc.setTextType(INTL_CS_COLL_TO_TTYPE(toCharSet,
						(fromCharSet == toCharSet ? INTL_GET_COLLATE(&dsqlParameter->par_desc) : 0)));

					dsqlParameter->par_desc.dsc_length = UTLD_char_length_to_byte_length(
						dsqlParameter->par_desc.dsc_length / fromCharSetBPC, toCharSetBPC);
				}

				dsqlParameter->par_desc.dsc_length += diff;
			}
			else if (dsqlParameter->par_desc.dsc_dtype == dtype_blob &&
				dsqlParameter->par_desc.dsc_sub_type == isc_blob_text &&
				fromCharSet != CS_NONE && fromCharSet != CS_BINARY)
			{
				dsqlParameter->par_desc.setTextType(toCharSet);
			}
		}
	}

	if (!dsqlParameter)
	{
		dsqlParameter = MAKE_parameter(dsqlScratch->getStatement()->getSendMsg(), true, true,
			dsqlParameterIndex, NULL);
		dsqlParameterIndex = dsqlParameter->par_index;
	}

	// In case of RETURNING in MERGE and UPDATE OR INSERT, a single parameter is used in
	// more than one place. So we save it to use below.
	const bool hasOldDesc = dsqlParameter->par_node != NULL;

	dsqlParameter->par_node = MAKE_node(Dsql::nod_class_exprnode, 1);
	dsqlParameter->par_node->nod_arg[0] = reinterpret_cast<dsql_nod*>(this);

	// Parameters should receive precisely the data that the user
	// passes in.  Therefore for text strings lets use varying
	// strings to insure that we don't add trailing blanks.

	// However, there are situations this leads to problems - so
	// we use the forceVarChar parameter to prevent this
	// datatype assumption from occuring.

	if (forceVarChar)
	{
		if (dsqlParameter->par_desc.dsc_dtype == dtype_text)
		{
			dsqlParameter->par_desc.dsc_dtype = dtype_varying;
			// The error msgs is inaccurate, but causing dsc_length
			// to be outsise range can be worse.
			if (dsqlParameter->par_desc.dsc_length > MAX_COLUMN_SIZE - sizeof(USHORT))
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-204) <<
							//Arg::Gds(isc_dsql_datatype_err)
							Arg::Gds(isc_imp_exc));
							//Arg::Gds(isc_field_name) << Arg::Str(parameter->par_name)
			}

			dsqlParameter->par_desc.dsc_length += sizeof(USHORT);
		}
		else if (!dsqlParameter->par_desc.isText() && !dsqlParameter->par_desc.isBlob())
		{
			const USHORT toCharSetBPC = METD_get_charset_bpc(
				dsqlScratch->getTransaction(), tdbb->getCharSet());

			// The LIKE & similar parameters must be varchar type
			// strings - so force this parameter to be varchar
			// and take a guess at a good length for it.
			dsqlParameter->par_desc.dsc_dtype = dtype_varying;
			dsqlParameter->par_desc.dsc_length = LIKE_PARAM_LEN * toCharSetBPC + sizeof(USHORT);
			dsqlParameter->par_desc.dsc_sub_type = 0;
			dsqlParameter->par_desc.dsc_scale = 0;
			dsqlParameter->par_desc.setTextType(tdbb->getCharSet());
		}
	}

	if (hasOldDesc)
	{
		dsc thisDesc = dsqlParameter->par_desc;
		const dsc* args[] = {&oldDesc, &thisDesc};
		DSqlDataTypeUtil(dsqlScratch).makeFromList(&dsqlParameter->par_desc,
			dsqlParameter->par_name.c_str(), 2, args);
	}

	return true;
}

void ParameterNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	GEN_parameter(dsqlScratch, dsqlParameter);
}

void ParameterNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	// We don't actually know the datatype of a parameter -
	// we have to guess it based on the context that the
	// parameter appears in. (This is done is pass1.c::set_parameter_type())
	// However, a parameter can appear as part of an expression.
	// As MAKE_desc is used for both determination of parameter
	// types and for expression type checking, we just continue.

	if (dsqlParameter->par_desc.dsc_dtype)
		*desc = dsqlParameter->par_desc;
}

bool ParameterNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	const ParameterNode* o = other->as<ParameterNode>();

	return o && dsqlParameter->par_index == o->dsqlParameter->par_index;
}

void ParameterNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	*desc = message->format->fmt_desc[argNumber];
}

ValueExprNode* ParameterNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ParameterNode* node = FB_NEW(*tdbb->getDefaultPool()) ParameterNode(*tdbb->getDefaultPool());
	node->argNumber = argNumber;

	// dimitr:	IMPORTANT!!!
	// nod_message copying must be done in the only place
	// (the nod_procedure code). Hence we don't call
	// copy() here to keep argument->nod_arg[e_arg_message]
	// and procedure->nod_arg[e_prc_in_msg] in sync. The
	// message is passed to copy() as a parameter. If the
	// passed message is NULL, it means nod_argument is
	// cloned outside nod_procedure (e.g. in the optimizer)
	// and we must keep the input message.
	// ASF: We should only use "message" if its number matches the number
	// in nod_argument. If it doesn't, it may be an input parameter cloned
	// in RseBoolNode::convertNeqAllToNotAny - see CORE-3094.

	if (copier.message && copier.message->messageNumber == message->messageNumber)
		node->message = copier.message;
	else
		node->message = message;

	node->argFlag = copier.copy(tdbb, argFlag);
	node->argIndicator = copier.copy(tdbb, argIndicator);

	return node;
}

ValueExprNode* ParameterNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	argInfo = CMP_pass2_validation(tdbb, csb,
		Item(Item::TYPE_PARAMETER, message->messageNumber, argNumber));

	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, (nodFlags & FLAG_VALUE) ? sizeof(impure_value_ex) : sizeof(dsc));

	return this;
}

dsc* ParameterNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	const dsc* desc;

	if (argFlag)
	{
		desc = EVL_expr(tdbb, request, argFlag);
		if (MOV_get_long(desc, 0))
			request->req_flags |= req_null;
	}

	desc = &message->format->fmt_desc[argNumber];

	impure->vlu_desc.dsc_address = request->getImpure<UCHAR>(
		message->impureOffset + (IPTR) desc->dsc_address);
	impure->vlu_desc.dsc_dtype = desc->dsc_dtype;
	impure->vlu_desc.dsc_length = desc->dsc_length;
	impure->vlu_desc.dsc_scale = desc->dsc_scale;
	impure->vlu_desc.dsc_sub_type = desc->dsc_sub_type;

	if (impure->vlu_desc.dsc_dtype == dtype_text)
		INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);

	USHORT* impure_flags = request->getImpure<USHORT>(
		message->impureFlags + (sizeof(USHORT) * argNumber));

	if (!(*impure_flags & VLU_checked))
	{
		if (argInfo)
		{
			EVL_validate(tdbb, Item(Item::TYPE_PARAMETER, message->messageNumber, argNumber),
				argInfo, &impure->vlu_desc, request->req_flags & req_null);
		}

		*impure_flags |= VLU_checked;
	}

	return (request->req_flags & req_null) ? NULL : &impure->vlu_desc;
}


//--------------------


static RegisterNode<RecordKeyNode> regRecordKeyNodeDbKey(blr_dbkey);
static RegisterNode<RecordKeyNode> regRecordKeyNodeRecordVersion(blr_record_version);

RecordKeyNode::RecordKeyNode(MemoryPool& pool, UCHAR aBlrOp, const MetaName& aDsqlQualifier)
	: TypedNode<ValueExprNode, ExprNode::TYPE_RECORD_KEY>(pool),
	  blrOp(aBlrOp),
	  dsqlQualifier(pool, aDsqlQualifier),
	  dsqlRelation(NULL),
	  recStream(0),
	  aggregate(false)
{
	fb_assert(blrOp == blr_dbkey || blrOp == blr_record_version);
	addChildNode(dsqlRelation);
}

DmlNode* RecordKeyNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	RecordKeyNode* node = FB_NEW(pool) RecordKeyNode(pool, blrOp);

	node->recStream = csb->csb_blr_reader.getByte();

	if (node->recStream >= csb->csb_rpt.getCount() || !(csb->csb_rpt[node->recStream].csb_flags & csb_used))
		PAR_error(csb, Arg::Gds(isc_ctxnotdef));

	node->recStream = csb->csb_rpt[node->recStream].csb_stream;

	return node;
}

void RecordKeyNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("RecordKeyNode (%s)", (blrOp == blr_dbkey ? "dbkey" : "record_version"));
	ExprNode::print(text, nodes);
}

// Resolve a dbkey to an available context.
ValueExprNode* RecordKeyNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	thread_db* tdbb = JRD_get_thread_data();

	if (dsqlQualifier.isEmpty())
	{
		DsqlContextStack contexts;

		for (DsqlContextStack::iterator stack(*dsqlScratch->context); stack.hasData(); ++stack)
		{
			dsql_ctx* context = stack.object();
			if (context->ctx_scope_level != dsqlScratch->scopeLevel)
				continue;

			if (context->ctx_relation)
				contexts.push(context);
		}

		if (contexts.hasData())
		{
			dsql_ctx* context = contexts.object();

			if (!context->ctx_relation)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
						  Arg::Gds(isc_dsql_dbkey_from_non_table));
			}

			if (context->ctx_flags & CTX_null)
				return FB_NEW(*tdbb->getDefaultPool()) NullNode(*tdbb->getDefaultPool());

			PASS1_ambiguity_check(dsqlScratch, MAKE_cstring("RDB$DB_KEY"), contexts);

			RelationSourceNode* relNode = FB_NEW(getPool()) RelationSourceNode(getPool());
			relNode->dsqlContext = context;

			RecordKeyNode* node = FB_NEW(getPool()) RecordKeyNode(getPool(), blrOp);
			node->dsqlRelation = MAKE_node(Dsql::nod_class_exprnode, 1);
			node->dsqlRelation->nod_arg[0] = reinterpret_cast<dsql_nod*>(relNode);

			return node;
		}
	}
	else
	{
		const bool cfgRlxAlias = Config::getRelaxedAliasChecking();
		bool rlxAlias = false;

		for (;;)
		{
			for (DsqlContextStack::iterator stack(*dsqlScratch->context); stack.hasData(); ++stack)
			{
				dsql_ctx* context = stack.object();

				if ((!context->ctx_relation ||
						context->ctx_relation->rel_name != dsqlQualifier ||
						!rlxAlias && context->ctx_internal_alias.hasData()) &&
					(context->ctx_internal_alias.isEmpty() ||
						strcmp(dsqlQualifier.c_str(), context->ctx_internal_alias.c_str()) != 0))
				{
					continue;
				}

				if (!context->ctx_relation)
				{
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
							  Arg::Gds(isc_dsql_dbkey_from_non_table));
				}

				if (context->ctx_flags & CTX_null)
					return FB_NEW(*tdbb->getDefaultPool()) NullNode(*tdbb->getDefaultPool());

				RelationSourceNode* relNode = FB_NEW(getPool()) RelationSourceNode(getPool());
				relNode->dsqlContext = context;

				RecordKeyNode* node = FB_NEW(getPool()) RecordKeyNode(getPool(), blrOp);
				node->dsqlRelation = MAKE_node(Dsql::nod_class_exprnode, 1);
				node->dsqlRelation->nod_arg[0] = reinterpret_cast<dsql_nod*>(relNode);

				return node;
			}

			if (rlxAlias == cfgRlxAlias)
				break;

			rlxAlias = cfgRlxAlias;
		}
	}

	//// FIXME: pass the node to report line/column of the error.
	// Field unresolved.
	PASS1_field_unknown((dsqlQualifier.hasData() ? dsqlQualifier.c_str() : NULL), DB_KEY_NAME, NULL);

	return NULL;
}

bool RecordKeyNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	return false;
}

bool RecordKeyNode::dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
{
	RelationSourceNode* relNode;

	if (dsqlRelation && (relNode = ExprNode::as<RelationSourceNode>(dsqlRelation)))
	{
		if (relNode->dsqlContext &&
			relNode->dsqlContext->ctx_scope_level == visitor.context->ctx_scope_level)
		{
			return true;
		}
	}

	return false;
}

bool RecordKeyNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return false;
}

bool RecordKeyNode::dsqlFieldFinder(FieldFinder& visitor)
{
	return false;
}

bool RecordKeyNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	visitor.replaceNode(PASS1_post_map(visitor.dsqlScratch, this,
		visitor.context, visitor.partitionNode, visitor.orderNode));
	return false;
}

void RecordKeyNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = DB_KEY_NAME;

	dsql_ctx* context = (dsql_ctx*) dsqlRelation->nod_arg[0];

	if (context)
	{
		if (context->ctx_relation)
		{
			parameter->par_rel_name = context->ctx_relation->rel_name.c_str();
			parameter->par_owner_name = context->ctx_relation->rel_owner.c_str();
		}
		else if (context->ctx_procedure)
		{
			parameter->par_rel_name = context->ctx_procedure->prc_name.identifier.c_str();
			parameter->par_owner_name = context->ctx_procedure->prc_owner.c_str();
		}

		parameter->par_rel_alias = context->ctx_alias.c_str();
	}
}

void RecordKeyNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsql_ctx* context = ExprNode::as<RelationSourceNode>(dsqlRelation)->dsqlContext;
	dsqlScratch->appendUChar(blrOp);
	GEN_stuff_context(dsqlScratch, context);
}

void RecordKeyNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	fb_assert(blrOp == blr_dbkey);

	// Fix for bug 10072 check that the target is a relation
	RelationSourceNode* relNode = ExprNode::as<RelationSourceNode>(dsqlRelation);
	dsql_rel* relation;

	if (relNode && (relation = relNode->dsqlContext->ctx_relation))
	{
		desc->dsc_dtype = dtype_text;

		if (relation->rel_flags & REL_creating)
			desc->dsc_length = 8;
		else
			desc->dsc_length = relation->rel_dbkey_length;

		desc->dsc_flags = DSC_nullable;
		desc->dsc_ttype() = ttype_binary;
	}
	else
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
				  Arg::Gds(isc_dsql_dbkey_from_non_table));
	}
}

bool RecordKeyNode::jrdStreamFinder(USHORT findStream)
{
	return recStream == findStream;
}

void RecordKeyNode::jrdStreamsCollector(SortedStreamList& streamList)
{
	if (!streamList.exist(recStream))
		streamList.add(recStream);
}

bool RecordKeyNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	if (allowOnlyCurrentStream)
	{
		if (recStream != stream && !(csb->csb_rpt[recStream].csb_flags & csb_sub_stream))
			return false;
	}
	else
	{
		if (recStream == stream)
			return false;
	}

	return csb->csb_rpt[recStream].csb_flags & csb_active;
}

void RecordKeyNode::findDependentFromStreams(const OptimizerRetrieval* optRet, SortedStreamList* streamList)
{
	if (recStream != optRet->stream && (optRet->csb->csb_rpt[recStream].csb_flags & csb_active))
	{
		if (!streamList->exist(recStream))
			streamList->add(recStream);
	}
}

void RecordKeyNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	if (blrOp == blr_dbkey)
	{
		desc->dsc_dtype = dtype_dbkey;
		desc->dsc_length = type_lengths[dtype_dbkey];
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
	}
	else if (blrOp == blr_record_version)
	{
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype() = ttype_binary;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
	}
}

ValueExprNode* RecordKeyNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	RecordKeyNode* node = FB_NEW(*tdbb->getDefaultPool()) RecordKeyNode(*tdbb->getDefaultPool(), blrOp);
	node->recStream = recStream;
	node->aggregate = aggregate;

	if (copier.remap)
	{
#ifdef CMP_DEBUG
		csb->dump("remap RecordKeyNode: %d -> %d\n", node->recStream, copier.remap[node->recStream]);
#endif
		node->recStream = copier.remap[node->recStream];
	}

	return node;
}

bool RecordKeyNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const RecordKeyNode* o = other->as<RecordKeyNode>();
	fb_assert(o);

	return blrOp == o->blrOp;
}

bool RecordKeyNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	RecordKeyNode* o = other->as<RecordKeyNode>();
	fb_assert(o);

	CompilerScratch::csb_repeat* const tail = &csb->csb_rpt[recStream];
	CompilerScratch::csb_repeat* const other_tail = &csb->csb_rpt[o->recStream];

	return blrOp == o->blrOp &&
		(recStream == o->recStream ||
		(tail->csb_relation && other_tail->csb_relation &&
			tail->csb_relation->rel_id == other_tail->csb_relation->rel_id) ||
		(tail->csb_procedure && other_tail->csb_procedure &&
			tail->csb_procedure->getId() == other_tail->csb_procedure->getId()));
}

ValueExprNode* RecordKeyNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass1(tdbb, csb);

	CMP_mark_variant(csb, recStream);

	if (!csb->csb_rpt[recStream].csb_map)
		return this;

	ValueExprNodeStack stack;
	CMP_expand_view_nodes(tdbb, csb, recStream, stack, blrOp, false);

#ifdef CMP_DEBUG
	csb->dump("expand RecordKeyNode: %d\n", recStream);
#endif

	if (stack.hasData())
	{
		const size_t stackCount = stack.getCount();

		// If that is a DB_KEY of a view, it's possible (in case of
		// outer joins) that some sub-stream have a NULL DB_KEY.
		// In this case, we build a COALESCE(DB_KEY, _OCTETS x"0000000000000000"),
		// for the concatenation of sub DB_KEYs not result in NULL.
		if (blrOp == blr_dbkey && stackCount > 1)
		{
			ValueExprNodeStack stack2;

			for (ValueExprNodeStack::iterator i(stack); i.hasData(); ++i)
			{
#ifdef CMP_DEBUG
				csb->dump(" %d", ExprNode::as<RecordKeyNode>(i.object())->recStream);
#endif

				ValueIfNode* valueIfNode = FB_NEW(csb->csb_pool) ValueIfNode(csb->csb_pool);

				MissingBoolNode* missingNode = FB_NEW(csb->csb_pool) MissingBoolNode(csb->csb_pool);
				missingNode->arg = i.object();

				NotBoolNode* notNode = FB_NEW(csb->csb_pool) NotBoolNode(csb->csb_pool);
				notNode->arg = missingNode;

				// build an IF (RDB$DB_KEY IS NOT NULL)
				valueIfNode->condition = notNode;

				valueIfNode->trueValue = i.object();	// THEN

				LiteralNode* literal = FB_NEW(csb->csb_pool) LiteralNode(csb->csb_pool);
				literal->litDesc.dsc_dtype = dtype_text;
				literal->litDesc.dsc_ttype() = CS_BINARY;
				literal->litDesc.dsc_scale = 0;
				literal->litDesc.dsc_length = 8;
				literal->litDesc.dsc_address = reinterpret_cast<UCHAR*>(
					const_cast<char*>("\0\0\0\0\0\0\0\0"));	// safe const_cast

				valueIfNode->falseValue = literal;

				stack2.push(valueIfNode);
			}

			stack.clear();

			// stack2 is in reverse order, pushing everything in stack
			// will correct the order.
			for (ValueExprNodeStack::iterator i2(stack2); i2.hasData(); ++i2)
				stack.push(i2.object());
		}

		ValueExprNode* node = catenateNodes(tdbb, stack);

		if (blrOp == blr_dbkey && stackCount > 1)
		{
			// ASF: If the view is in null state (with outer joins) we need to transform
			// the view RDB$KEY to NULL. (CORE-1245)

			ValueIfNode* valueIfNode = FB_NEW(csb->csb_pool) ValueIfNode(csb->csb_pool);

			ComparativeBoolNode* eqlNode = FB_NEW(csb->csb_pool) ComparativeBoolNode(
				csb->csb_pool, blr_eql);

			// build an IF (RDB$DB_KEY = '')
			valueIfNode->condition = eqlNode;

			eqlNode->arg1 = NodeCopier::copy(tdbb, csb, node, NULL);

			LiteralNode* literal = FB_NEW(csb->csb_pool) LiteralNode(csb->csb_pool);
			literal->litDesc.dsc_dtype = dtype_text;
			literal->litDesc.dsc_ttype() = CS_BINARY;
			literal->litDesc.dsc_scale = 0;
			literal->litDesc.dsc_length = 0;
			literal->litDesc.dsc_address = reinterpret_cast<UCHAR*>(
				const_cast<char*>(""));	// safe const_cast

			eqlNode->arg2 = literal;

			// THEN: NULL
			valueIfNode->trueValue = FB_NEW(csb->csb_pool) NullNode(csb->csb_pool);

			// ELSE: RDB$DB_KEY
			valueIfNode->falseValue = node;

			node = valueIfNode;
		}

#ifdef CMP_DEBUG
		csb->dump("\n");
#endif

		return node;
	}

#ifdef CMP_DEBUG
	csb->dump("\n");
#endif

	// The user is asking for the dbkey/record version of an aggregate.
	// Humor him with a key filled with zeros.

	RecordKeyNode* node = FB_NEW(*tdbb->getDefaultPool()) RecordKeyNode(*tdbb->getDefaultPool(), blrOp);
	node->recStream = recStream;
	node->aggregate = true;

	return node;
}

ValueExprNode* RecordKeyNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* RecordKeyNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	const record_param* rpb = &request->req_rpb[recStream];

	if (blrOp == blr_dbkey)
	{
		// Make up a dbkey for a record stream. A dbkey is expressed as an 8 byte character string.

		const jrd_rel* relation = rpb->rpb_relation;

		// If it doesn't point to a valid record, return NULL
		if (!rpb->rpb_number.isValid() || !relation)
		{
			request->req_flags |= req_null;
			return NULL;
		}

		// Format dbkey as vector of relation id, record number

		// Initialize first 32 bits of DB_KEY
		impure->vlu_misc.vlu_dbkey[0] = 0;

		// Now, put relation ID into first 16 bits of DB_KEY
		// We do not assign it as SLONG because of big-endian machines.
		*(USHORT*) impure->vlu_misc.vlu_dbkey = relation->rel_id;

		// Encode 40-bit record number. Before that, increment the value
		// because users expect the numbering to start with one.
		RecordNumber temp(rpb->rpb_number.getValue() + 1);
		temp.bid_encode(reinterpret_cast<RecordNumber::Packed*>(impure->vlu_misc.vlu_dbkey));

		// Initialize descriptor

		impure->vlu_desc.dsc_address = (UCHAR*) impure->vlu_misc.vlu_dbkey;
		impure->vlu_desc.dsc_dtype = dtype_dbkey;
		impure->vlu_desc.dsc_length = type_lengths[dtype_dbkey];
		impure->vlu_desc.dsc_ttype() = ttype_binary;
	}
	else if (blrOp == blr_record_version)
	{
		// Make up a record version for a record stream. The tid of the record will be used.
		// This will be returned as a 4 byte character string.

		// If the current transaction has updated the record, the record version
		// coming in from DSQL will have the original transaction # (or current
		// transaction if the current transaction updated the record in a different
		// request.).  In these cases, mark the request so that the boolean
		// to check equality of record version will be forced to evaluate to true.

		if (request->req_transaction->tra_number == rpb->rpb_transaction_nr)
			request->req_flags |= req_same_tx_upd;
		else
		{
			// If the transaction is a commit retain, check if the record was
			// last updated in one of its own prior transactions

			if (request->req_transaction->tra_commit_sub_trans)
			{
				if (request->req_transaction->tra_commit_sub_trans->test(rpb->rpb_transaction_nr))
					 request->req_flags |= req_same_tx_upd;
			}
		}

		// Initialize descriptor

		impure->vlu_misc.vlu_long = rpb->rpb_transaction_nr;
		impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_long;
		impure->vlu_desc.dsc_dtype = dtype_text;
		impure->vlu_desc.dsc_length = 4;
		impure->vlu_desc.dsc_ttype() = ttype_binary;
	}

	return &impure->vlu_desc;
}

// Take a stack of nodes and turn them into a tree of concatenations.
ValueExprNode* RecordKeyNode::catenateNodes(thread_db* tdbb, ValueExprNodeStack& stack)
{
	SET_TDBB(tdbb);

	ValueExprNode* node1 = stack.pop();

	if (stack.isEmpty())
		return node1;

	ConcatenateNode* concatNode = FB_NEW(*tdbb->getDefaultPool()) ConcatenateNode(
		*tdbb->getDefaultPool());
	concatNode->arg1 = node1;
	concatNode->arg2 = catenateNodes(tdbb, stack);

	return concatNode;
}


//--------------------


static RegisterNode<ScalarNode> regScalarNode1(blr_index);

DmlNode* ScalarNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	ScalarNode* node = FB_NEW(pool) ScalarNode(pool);
	node->field = PAR_parse_value(tdbb, csb);
	node->subscripts = PAR_args(tdbb, csb);
	return node;
}

void ScalarNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* csb, dsc* desc)
{
	FieldNode* fieldNode = field->as<FieldNode>();
	fb_assert(fieldNode);

	jrd_rel* relation = csb->csb_rpt[fieldNode->fieldStream].csb_relation;
	const jrd_fld* field = MET_get_field(relation, fieldNode->fieldId);
	const ArrayField* array;

	if (!field || !(array = field->fld_array))
		IBERROR(223);	// msg 223 argument of scalar operation must be an array

	*desc = array->arr_desc.iad_rpt[0].iad_desc;

	if (array->arr_desc.iad_dimensions > MAX_ARRAY_DIMENSIONS)
		IBERROR(306); // Found array data type with more than 16 dimensions
}

ValueExprNode* ScalarNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ScalarNode* node = FB_NEW(*tdbb->getDefaultPool()) ScalarNode(*tdbb->getDefaultPool());
	node->field = copier.copy(tdbb, field);
	node->subscripts = copier.copy(tdbb, subscripts);
	return node;
}

ValueExprNode* ScalarNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Evaluate a scalar item from an array.
dsc* ScalarNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	const dsc* desc = EVL_expr(tdbb, request, field);

	if (request->req_flags & req_null)
		return NULL;

	if (desc->dsc_dtype != dtype_array)
		IBERROR(261);	// msg 261 scalar operator used on field which is not an array

	if (subscripts->args.getCount() > MAX_ARRAY_DIMENSIONS)
		ERR_post(Arg::Gds(isc_array_max_dimensions) << Arg::Num(MAX_ARRAY_DIMENSIONS));

	SLONG numSubscripts[MAX_ARRAY_DIMENSIONS];
	int iter = 0;
	const NestConst<ValueExprNode>* ptr = subscripts->args.begin();

	for (const NestConst<ValueExprNode>* const end = subscripts->args.end(); ptr != end;)
	{
		const dsc* temp = EVL_expr(tdbb, request, *ptr++);

		if (temp && !(request->req_flags & req_null))
			numSubscripts[iter++] = MOV_get_long(temp, 0);
		else
			return NULL;
	}

	BLB_scalar(tdbb, request->req_transaction, reinterpret_cast<bid*>(desc->dsc_address),
		subscripts->args.getCount(), numSubscripts, impure);

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<StmtExprNode> regStmtExprNode(blr_stmt_expr);

DmlNode* StmtExprNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	StmtExprNode* node = FB_NEW(pool) StmtExprNode(pool);

	node->stmt = PAR_parse_stmt(tdbb, csb);
	node->expr = PAR_parse_value(tdbb, csb);

	// Avoid blr_stmt_expr in a BLR expression header.
	if (!node->stmt->is<AssignmentNode>())
		return node->expr;

	return node;
}

void StmtExprNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	fb_assert(false);

	expr->getDesc(tdbb, csb, desc);
}

ValueExprNode* StmtExprNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	fb_assert(false);

	StmtExprNode* node = FB_NEW(*tdbb->getDefaultPool()) StmtExprNode(*tdbb->getDefaultPool());
	node->stmt = copier.copy(tdbb, stmt);
	node->expr = copier.copy(tdbb, expr);
	return node;
}

ValueExprNode* StmtExprNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(false);

	doPass1(tdbb, csb, stmt.getAddress());
	return ValueExprNode::pass1(tdbb, csb);
}

ValueExprNode* StmtExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	fb_assert(false);

	StmtNode::doPass2(tdbb, csb, stmt.getAddress(), NULL);

	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* StmtExprNode::execute(thread_db* tdbb, jrd_req* request) const
{
	fb_assert(false);

	EXE_looper(tdbb, request, stmt.getObject(), true);

	dsc* desc = EVL_expr(tdbb, request, expr);

	return (request->req_flags & req_null) ? NULL : desc;
}


//--------------------


static RegisterNode<StrCaseNode> regStrCaseNodeLower(blr_lowcase);
static RegisterNode<StrCaseNode> regStrCaseNodeUpper(blr_upcase);

StrCaseNode::StrCaseNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_STR_CASE>(pool),
	  blrOp(aBlrOp),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* StrCaseNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	StrCaseNode* node = FB_NEW(pool) StrCaseNode(pool, blrOp);
	node->arg = PAR_parse_value(tdbb, csb);
	return node;
}

void StrCaseNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("StrCaseNode (%s)", (blrOp == blr_lowcase ? "lower" : "upper"));
	ExprNode::print(text, nodes);
}

ValueExprNode* StrCaseNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) StrCaseNode(getPool(), blrOp, PASS1_node(dsqlScratch, dsqlArg));
}

void StrCaseNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = (blrOp == blr_lowcase ? "LOWER" : "UPPER");
}

bool StrCaseNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void StrCaseNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	GEN_expr(dsqlScratch, dsqlArg);
}

void StrCaseNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1;
	MAKE_desc(dsqlScratch, &desc1, dsqlArg);

	if (desc1.dsc_dtype <= dtype_any_text || desc1.dsc_dtype == dtype_blob)
	{
		*desc = desc1;
		return;
	}

	desc->dsc_dtype = dtype_varying;
	desc->dsc_scale = 0;
	desc->dsc_ttype() = ttype_ascii;
	desc->dsc_length = sizeof(USHORT) + DSC_string_length(&desc1);
	desc->dsc_flags = desc1.dsc_flags & DSC_nullable;
}

void StrCaseNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	arg->getDesc(tdbb, csb, desc);

	if (desc->dsc_dtype > dtype_varying && desc->dsc_dtype != dtype_blob)
	{
		desc->dsc_length = DSC_convert_to_text_length(desc->dsc_dtype);
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype() = ttype_ascii;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
	}
}

ValueExprNode* StrCaseNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	StrCaseNode* node = FB_NEW(*tdbb->getDefaultPool()) StrCaseNode(*tdbb->getDefaultPool(), blrOp);
	node->arg = copier.copy(tdbb, arg);
	return node;
}

bool StrCaseNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const StrCaseNode* o = other->as<StrCaseNode>();
	fb_assert(o);

	return blrOp == o->blrOp;
}

bool StrCaseNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	StrCaseNode* otherNode = other->as<StrCaseNode>();
	fb_assert(otherNode);

	return blrOp == otherNode->blrOp;
}

ValueExprNode* StrCaseNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Low/up case a string.
dsc* StrCaseNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	const dsc* value = EVL_expr(tdbb, request, arg);

	if (request->req_flags & req_null)
		return NULL;

	TextType* textType = INTL_texttype_lookup(tdbb, value->getTextType());
	ULONG (TextType::*intlFunction)(ULONG, const UCHAR*, ULONG, UCHAR*) =
		(blrOp == blr_lowcase ? &TextType::str_to_lower : &TextType::str_to_upper);

	if (value->isBlob())
	{
		EVL_make_value(tdbb, value, impure);

		if (value->dsc_sub_type != isc_blob_text)
			return &impure->vlu_desc;

		CharSet* charSet = textType->getCharSet();

		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address));

		HalfStaticArray<UCHAR, BUFFER_SMALL> buffer;

		if (charSet->isMultiByte())
			buffer.getBuffer(blob->blb_length);	// alloc space to put entire blob in memory

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction,
			&impure->vlu_misc.vlu_bid);

		while (!(blob->blb_flags & BLB_eof))
		{
			SLONG len = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCapacity(), false);

			if (len)
			{
				len = (textType->*intlFunction)(len, buffer.begin(), len, buffer.begin());
				BLB_put_data(tdbb, newBlob, buffer.begin(), len);
			}
		}

		BLB_close(tdbb, newBlob);
		BLB_close(tdbb, blob);
	}
	else
	{
		UCHAR* ptr;
		VaryStr<32> temp;
		USHORT ttype;

		dsc desc;
		desc.dsc_length = MOV_get_string_ptr(value, &ttype, &ptr, &temp, sizeof(temp));
		desc.dsc_dtype = dtype_text;
		desc.dsc_address = NULL;
		desc.setTextType(ttype);
		EVL_make_value(tdbb, &desc, impure);

		ULONG len = (textType->*intlFunction)(desc.dsc_length,
			ptr, desc.dsc_length, impure->vlu_desc.dsc_address);

		if (len == INTL_BAD_STR_LENGTH)
			status_exception::raise(Arg::Gds(isc_arith_except));

		impure->vlu_desc.dsc_length = (USHORT) len;
	}

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<StrLenNode> regStrLenNode(blr_strlen);

StrLenNode::StrLenNode(MemoryPool& pool, UCHAR aBlrSubOp, dsql_nod* aArg)
	: TypedNode<ValueExprNode, ExprNode::TYPE_STR_LEN>(pool),
	  blrSubOp(aBlrSubOp),
	  dsqlArg(aArg),
	  arg(NULL)
{
	addChildNode(dsqlArg, arg);
}

DmlNode* StrLenNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	UCHAR blrSubOp = csb->csb_blr_reader.getByte();

	StrLenNode* node = FB_NEW(pool) StrLenNode(pool, blrSubOp);
	node->arg = PAR_parse_value(tdbb, csb);
	return node;
}

void StrLenNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text.printf("StrLenNode (%d)", blrSubOp);
	ExprNode::print(text, nodes);
}

ValueExprNode* StrLenNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) StrLenNode(getPool(), blrSubOp, PASS1_node(dsqlScratch, dsqlArg));
}

void StrLenNode::setParameterName(dsql_par* parameter) const
{
	const char* alias;

	switch (blrSubOp)
	{
		case blr_strlen_bit:
			alias = "BIT_LENGTH";
			break;

		case blr_strlen_char:
			alias = "CHAR_LENGTH";
			break;

		case blr_strlen_octet:
			alias = "OCTET_LENGTH";
			break;

		default:
			alias = "";
			fb_assert(false);
			break;
	}

	parameter->par_name = parameter->par_alias = alias;
}

bool StrLenNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void StrLenNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_strlen);
	dsqlScratch->appendUChar(blrSubOp);
	GEN_expr(dsqlScratch, dsqlArg);
}

void StrLenNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1;
	MAKE_desc(dsqlScratch, &desc1, dsqlArg);

	desc->makeLong(0);
	desc->setNullable(desc1.isNullable());
}

void StrLenNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->makeLong(0);
}

ValueExprNode* StrLenNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	StrLenNode* node = FB_NEW(*tdbb->getDefaultPool()) StrLenNode(*tdbb->getDefaultPool(), blrSubOp);
	node->arg = copier.copy(tdbb, arg);
	return node;
}

bool StrLenNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const StrLenNode* o = other->as<StrLenNode>();
	fb_assert(o);

	return blrSubOp == o->blrSubOp;
}

bool StrLenNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	StrLenNode* o = other->as<StrLenNode>();
	fb_assert(o);

	return blrSubOp == o->blrSubOp;
}

ValueExprNode* StrLenNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Handles BIT_LENGTH(s), OCTET_LENGTH(s) and CHAR[ACTER]_LENGTH(s)
dsc* StrLenNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	const dsc* value = EVL_expr(tdbb, request, arg);

	impure->vlu_desc.makeLong(0, &impure->vlu_misc.vlu_long);

	if (!value || (request->req_flags & req_null))
		return NULL;

	ULONG length;

	if (value->isBlob())
	{
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(value->dsc_address));

		switch (blrSubOp)
		{
			case blr_strlen_bit:
			{
				FB_UINT64 l = (FB_UINT64) blob->blb_length * 8;
				if (l > MAX_SINT64)
				{
					ERR_post(Arg::Gds(isc_arith_except) <<
							 Arg::Gds(isc_numeric_out_of_range));
				}

				length = l;
				break;
			}

			case blr_strlen_octet:
				length = blob->blb_length;
				break;

			case blr_strlen_char:
			{
				CharSet* charSet = INTL_charset_lookup(tdbb, value->dsc_blob_ttype());

				if (charSet->isMultiByte())
				{
					HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;

					length = BLB_get_data(tdbb, blob, buffer.getBuffer(blob->blb_length),
						blob->blb_length, false);
					length = charSet->length(length, buffer.begin(), true);
				}
				else
					length = blob->blb_length / charSet->maxBytesPerChar();

				break;
			}

			default:
				fb_assert(false);
				length = 0;
		}

		*(ULONG*) impure->vlu_desc.dsc_address = length;

		BLB_close(tdbb, blob);

		return &impure->vlu_desc;
	}

	VaryStr<32> temp;
	USHORT ttype;
	UCHAR* p;

	length = MOV_get_string_ptr(value, &ttype, &p, &temp, sizeof(temp));

	switch (blrSubOp)
	{
		case blr_strlen_bit:
		{
			FB_UINT64 l = (FB_UINT64) length * 8;
			if (l > MAX_SINT64)
			{
				ERR_post(Arg::Gds(isc_arith_except) <<
						 Arg::Gds(isc_numeric_out_of_range));
			}

			length = l;
			break;
		}

		case blr_strlen_octet:
			break;

		case blr_strlen_char:
		{
			CharSet* charSet = INTL_charset_lookup(tdbb, ttype);
			length = charSet->length(length, p, true);
			break;
		}

		default:
			fb_assert(false);
			length = 0;
	}

	*(ULONG*) impure->vlu_desc.dsc_address = length;

	return &impure->vlu_desc;
}


//--------------------


// Only blr_via is generated by DSQL.
static RegisterNode<SubQueryNode> regSubQueryNodeVia(blr_via);
static RegisterNode<SubQueryNode> regSubQueryNodeFrom(blr_from);
static RegisterNode<SubQueryNode> regSubQueryNodeAverage(blr_average);
static RegisterNode<SubQueryNode> regSubQueryNodeCount(blr_count);
static RegisterNode<SubQueryNode> regSubQueryNodeMaximum(blr_maximum);
static RegisterNode<SubQueryNode> regSubQueryNodeMinimum(blr_minimum);
static RegisterNode<SubQueryNode> regSubQueryNodeTotal(blr_total);

SubQueryNode::SubQueryNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aDsqlRse,
			dsql_nod* aValue1, dsql_nod* aValue2)
	: TypedNode<ValueExprNode, ExprNode::TYPE_SUBQUERY>(pool),
	  blrOp(aBlrOp),
	  dsqlRse(aDsqlRse),
	  dsqlValue1(aValue1),
	  dsqlValue2(aValue2),
	  value1(NULL),
	  value2(NULL),
	  rsb(NULL)
{
	addChildNode(dsqlRse, rse);
	addChildNode(dsqlValue1, value1);
	addChildNode(dsqlValue2, value2);
}

DmlNode* SubQueryNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	// We treat blr_from as blr_via after parse.
	SubQueryNode* node = FB_NEW(pool) SubQueryNode(pool, (blrOp == blr_from ? blr_via : blrOp));

	node->rse = PAR_rse(tdbb, csb);

	if (blrOp != blr_count)
		node->value1 = PAR_parse_value(tdbb, csb);

	if (blrOp == blr_via)
		node->value2 = PAR_parse_value(tdbb, csb);

	return node;
}

void SubQueryNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SubQueryNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* SubQueryNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	// This node is created after dsqlPass and should never be called.
	fb_assert(false);
	return this;
}

void SubQueryNode::setParameterName(dsql_par* parameter) const
{
	MAKE_parameter_names(parameter, dsqlValue1);
}

void SubQueryNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blrOp);
	GEN_expr(dsqlScratch, dsqlRse);
	GEN_expr(dsqlScratch, dsqlValue1);
	GEN_expr(dsqlScratch, dsqlValue2);
}

void SubQueryNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, dsqlValue1);

	// Set the descriptor flag as nullable. The select expression may or may not return this row
	// based on the WHERE clause. Setting this flag warns the client to expect null values.
	// (bug 10379)
	desc->dsc_flags |= DSC_nullable;
}

bool SubQueryNode::dsqlAggregateFinder(AggregateFinder& visitor)
{
	return !visitor.ignoreSubSelects && visitor.visit(&dsqlRse);
}

bool SubQueryNode::dsqlAggregate2Finder(Aggregate2Finder& visitor)
{
	return visitor.visit(&dsqlRse);	// Pass only the rse.
}

bool SubQueryNode::dsqlSubSelectFinder(SubSelectFinder& visitor)
{
	return true;
}

bool SubQueryNode::dsqlFieldFinder(FieldFinder& visitor)
{
	return visitor.visit(&dsqlRse);	// Pass only the rse.
}

bool SubQueryNode::dsqlFieldRemapper(FieldRemapper& visitor)
{
	visitor.visit(&dsqlRse);
	dsqlValue1 = ExprNode::as<RseNode>(dsqlRse)->dsqlSelectList->nod_arg[0];
	return false;
}

bool SubQueryNode::jrdStreamFinder(USHORT findStream)
{
	if (rse && rse->jrdStreamFinder(findStream))
		return true;

	if (value1 && value1->jrdStreamFinder(findStream))
		return true;

	return false;
}

void SubQueryNode::jrdStreamsCollector(SortedStreamList& streamList)
{
	if (rse)
		rse->jrdStreamsCollector(streamList);

	if (value1)
		value1->jrdStreamsCollector(streamList);
}

bool SubQueryNode::computable(CompilerScratch* csb, SSHORT stream,
	bool allowOnlyCurrentStream, ValueExprNode* /*value*/)
{
	if (value2 && !value2->computable(csb, stream, allowOnlyCurrentStream))
		return false;

	return rse->computable(csb, stream, allowOnlyCurrentStream, value1);
}

void SubQueryNode::findDependentFromStreams(const OptimizerRetrieval* optRet, SortedStreamList* streamList)
{
	if (value2)
		value2->findDependentFromStreams(optRet, streamList);

	rse->findDependentFromStreams(optRet, streamList);

	// Check value expression, if any.
	if (value1)
		value1->findDependentFromStreams(optRet, streamList);
}

void SubQueryNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	if (blrOp == blr_count)
		desc->makeLong(0);
	else if (value1)
		value1->getDesc(tdbb, csb, desc);

	if (blrOp == blr_average)
	{
		if (!(DTYPE_IS_NUMERIC(desc->dsc_dtype) || DTYPE_IS_TEXT(desc->dsc_dtype)))
		{
			if (desc->dsc_dtype != dtype_unknown)
				return;
		}

		desc->dsc_dtype = DEFAULT_DOUBLE;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
	}
	else if (blrOp == blr_total)
	{
		const USHORT dtype = desc->dsc_dtype;

		switch (dtype)
		{
			case dtype_short:
				desc->dsc_dtype = dtype_long;
				desc->dsc_length = sizeof(SLONG);
				nodScale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_unknown:
				desc->dsc_dtype = dtype_unknown;
				desc->dsc_length = 0;
				nodScale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_long:
			case dtype_int64:
			case dtype_real:
			case dtype_double:
			case dtype_text:
			case dtype_cstring:
			case dtype_varying:
				desc->dsc_dtype = DEFAULT_DOUBLE;
				desc->dsc_length = sizeof(double);
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				nodFlags |= FLAG_DOUBLE;
				return;

			case dtype_quad:
				desc->dsc_dtype = dtype_quad;
				desc->dsc_length = sizeof(SQUAD);
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				nodScale = desc->dsc_scale;
				nodFlags |= FLAG_QUAD;
#ifdef NATIVE_QUAD
				return;
#endif

			default:
				fb_assert(false);
				// fall into

			case dtype_sql_time:
			case dtype_sql_date:
			case dtype_timestamp:
			case dtype_blob:
			case dtype_array:
			case dtype_dbkey:
				// break to error reporting code
				break;
		}

		if (dtype == dtype_quad)
			IBERROR(224);	// msg 224 quad word arithmetic not supported

		ERR_post(Arg::Gds(isc_datype_notsup));	// data type not supported for arithmetic
	}
}

ValueExprNode* SubQueryNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	SubQueryNode* node = FB_NEW(*tdbb->getDefaultPool()) SubQueryNode(*tdbb->getDefaultPool(), blrOp);
	node->nodScale = nodScale;
	node->rse = copier.copy(tdbb, rse);
	node->value1 = copier.copy(tdbb, value1);
	node->value2 = copier.copy(tdbb, value2);
	return node;
}

bool SubQueryNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	return false;
}

ValueExprNode* SubQueryNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	rse->ignoreDbKey(tdbb, csb);
	doPass1(tdbb, csb, rse.getAddress());

	csb->csb_current_nodes.push(rse.getObject());

	doPass1(tdbb, csb, value1.getAddress());
	doPass1(tdbb, csb, value2.getAddress());

	csb->csb_current_nodes.pop();

	return this;
}

ValueExprNode* SubQueryNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	if (!rse)
		ERR_post(Arg::Gds(isc_wish_list));

	if (!(rse->flags & RseNode::FLAG_VARIANT))
	{
		nodFlags |= FLAG_INVARIANT;
		csb->csb_invariants.push(&impureOffset);
	}

	rse->pass2Rse(tdbb, csb);

	ValueExprNode::pass2(tdbb, csb);

	impureOffset = CMP_impure(csb, sizeof(impure_value_ex));

	if (blrOp == blr_average)
		nodFlags |= FLAG_DOUBLE;
	else if (blrOp == blr_total)
	{
		dsc desc;
		getDesc(tdbb, csb, &desc);
	}

	// Bind values of invariant nodes to top-level RSE (if present).
	if ((nodFlags & FLAG_INVARIANT) && csb->csb_current_nodes.hasData())
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

	// Finish up processing of record selection expressions.

	rsb = CMP_post_rse(tdbb, csb, rse);
	csb->csb_fors.add(rsb);

	return this;
}

// Evaluate a subquery expression.
dsc* SubQueryNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	dsc* desc = &impure->vlu_desc;
	USHORT* invariant_flags = NULL;

	if (nodFlags & FLAG_INVARIANT)
	{
		invariant_flags = &impure->vlu_flags;

		if (*invariant_flags & VLU_computed)
		{
			// An invariant node has already been computed.

			if (*invariant_flags & VLU_null)
				request->req_flags |= req_null;
			else
				request->req_flags &= ~req_null;

			return desc;
		}
	}

	impure->vlu_misc.vlu_long = 0;
	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_length = sizeof(SLONG);
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_long;

	ULONG flag = req_null;

	try
	{
		rsb->open(tdbb);

		SLONG count = 0;
		double d;

		// Handle each variety separately
		switch (blrOp)
		{
			case blr_count:
				flag = 0;
				while (rsb->getRecord(tdbb))
					++impure->vlu_misc.vlu_long;
				break;

			case blr_minimum:
			case blr_maximum:
				while (rsb->getRecord(tdbb))
				{
					dsc* value = EVL_expr(tdbb, request, value1);
					if (request->req_flags & req_null)
						continue;

					int result;

					if (flag || ((result = MOV_compare(value, desc)) < 0 && blrOp == blr_minimum) ||
						(blrOp != blr_minimum && result > 0))
					{
						flag = 0;
						EVL_make_value(tdbb, value, impure);
					}
				}
				break;

			case blr_average:	// total or average with dialect-1 semantics
			case blr_total:
				while (rsb->getRecord(tdbb))
				{
					desc = EVL_expr(tdbb, request, value1);
					if (request->req_flags & req_null)
						continue;

					// Note: if the field being SUMed or AVERAGEd is short or long,
					// impure will stay long, and the first add() will
					// set the correct scale; if it is approximate numeric,
					// the first add() will convert impure to double.
					ArithmeticNode::add(desc, impure, this, blr_add);

					++count;
				}

				desc = &impure->vlu_desc;

				if (blrOp == blr_total)
				{
					flag = 0;
					break;
				}

				if (!count)
					break;

				d = MOV_get_double(&impure->vlu_desc);
				impure->vlu_misc.vlu_double = d / count;
				impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
				impure->vlu_desc.dsc_length = sizeof(double);
				impure->vlu_desc.dsc_scale = 0;
				flag = 0;
				break;

			case blr_via:
				if (rsb->getRecord(tdbb))
					desc = EVL_expr(tdbb, request, value1);
				else
				{
					if (value2)
						desc = EVL_expr(tdbb, request, value2);
					else
						ERR_post(Arg::Gds(isc_from_no_match));
				}

				flag = request->req_flags;
				break;

			default:
				BUGCHECK(233);	// msg 233 eval_statistical: invalid operation
		}
	}
	catch (const Exception&)
	{
		// Close stream, ignoring any error during it to keep the original error.
		try
		{
			rsb->close(tdbb);
			request->req_flags &= ~req_null;
			request->req_flags |= flag;
		}
		catch (const Exception&)
		{
		}

		throw;
	}

	// Close stream and return value.

	rsb->close(tdbb);
	request->req_flags &= ~req_null;
	request->req_flags |= flag;

	// If this is an invariant node, save the return value. If the descriptor does not point to the
	// impure area for this node then point this node's descriptor to the correct place;
	// Copy the whole structure to be absolutely sure.

	if (nodFlags & FLAG_INVARIANT)
	{
		*invariant_flags |= VLU_computed;

		if (request->req_flags & req_null)
			*invariant_flags |= VLU_null;
		if (desc && (desc != &impure->vlu_desc))
			impure->vlu_desc = *desc;
	}

	return (request->req_flags & req_null) ? NULL : desc;
}


//--------------------


static RegisterNode<SubstringNode> regSubstringNode(blr_substring);

SubstringNode::SubstringNode(MemoryPool& pool, dsql_nod* aExpr, dsql_nod* aStart, dsql_nod* aLength)
	: TypedNode<ValueExprNode, ExprNode::TYPE_SUBSTRING>(pool),
	  dsqlExpr(aExpr),
	  dsqlStart(aStart),
	  dsqlLength(aLength),
	  expr(NULL),
	  start(NULL),
	  length(NULL)
{
	addChildNode(dsqlExpr, expr);
	addChildNode(dsqlStart, start);
	addChildNode(dsqlLength, length);
}

DmlNode* SubstringNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
	UCHAR /*blrOp*/)
{
	SubstringNode* node = FB_NEW(pool) SubstringNode(pool);
	node->expr = PAR_parse_value(tdbb, csb);
	node->start = PAR_parse_value(tdbb, csb);
	node->length = PAR_parse_value(tdbb, csb);
	return node;
}

void SubstringNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "SubstringNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* SubstringNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	SubstringNode* node = FB_NEW(getPool()) SubstringNode(getPool(),
		PASS1_node(dsqlScratch, dsqlExpr),
		PASS1_node(dsqlScratch, dsqlStart),
		PASS1_node(dsqlScratch, dsqlLength));

	return node;
}

void SubstringNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "SUBSTRING";
}

bool SubstringNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlExpr, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlStart, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlLength, node, forceVarChar);
}

void SubstringNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_substring);
	GEN_expr(dsqlScratch, dsqlExpr);
	GEN_expr(dsqlScratch, dsqlStart);
	GEN_expr(dsqlScratch, dsqlLength);
}

void SubstringNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1, desc2, desc3;

	MAKE_desc(dsqlScratch, &desc1, dsqlExpr);
	MAKE_desc(dsqlScratch, &desc2, dsqlStart);
	MAKE_desc(dsqlScratch, &desc3, dsqlLength);

	DSqlDataTypeUtil(dsqlScratch).makeSubstr(desc, &desc1, &desc2, &desc3);
}

void SubstringNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	DSC desc0, desc1, desc2, desc3;

	expr->getDesc(tdbb, csb, &desc0);

	ValueExprNode* offsetNode = start;
	ValueExprNode* decrementNode = NULL;
	ArithmeticNode* arithmeticNode = offsetNode->as<ArithmeticNode>();

	// ASF: This code is very strange. The DSQL node is created as dialect 1, but only the dialect
	// 3 is verified here. Also, this task seems unnecessary here, as it must be done during
	// execution anyway.

	if (arithmeticNode && arithmeticNode->blrOp == blr_subtract && !arithmeticNode->dialect1)
	{
		// This node is created by the DSQL layer, but the system BLR code bypasses it and uses
		// zero-based string offsets instead.
		decrementNode = arithmeticNode->arg2;
		decrementNode->getDesc(tdbb, csb, &desc3);
		offsetNode = arithmeticNode->arg1;
	}

	offsetNode->getDesc(tdbb, csb, &desc1);
	length->getDesc(tdbb, csb, &desc2);

	DataTypeUtil(tdbb).makeSubstr(desc, &desc0, &desc1, &desc2);

	if (desc1.dsc_flags & DSC_null || desc2.dsc_flags & DSC_null)
		desc->dsc_flags |= DSC_null;
	else
	{
		if (offsetNode->is<LiteralNode>() && desc1.dsc_dtype == dtype_long)
		{
			SLONG offset = MOV_get_long(&desc1, 0);

			if (decrementNode && decrementNode->is<LiteralNode>() && desc3.dsc_dtype == dtype_long)
				offset -= MOV_get_long(&desc3, 0);

			if (offset < 0)
				ERR_post(Arg::Gds(isc_bad_substring_offset) << Arg::Num(offset + 1));
		}

		if (length->is<LiteralNode>() && desc2.dsc_dtype == dtype_long)
		{
			const SLONG len = MOV_get_long(&desc2, 0);

			if (len < 0)
				ERR_post(Arg::Gds(isc_bad_substring_length) << Arg::Num(len));
		}
	}
}

ValueExprNode* SubstringNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	SubstringNode* node = FB_NEW(*tdbb->getDefaultPool()) SubstringNode(
		*tdbb->getDefaultPool());
	node->expr = copier.copy(tdbb, expr);
	node->start = copier.copy(tdbb, start);
	node->length = copier.copy(tdbb, length);
	return node;
}

ValueExprNode* SubstringNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* SubstringNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(impureOffset);

	// Run all expression arguments.

	const dsc* exprDesc = EVL_expr(tdbb, request, expr);
	exprDesc = (request->req_flags & req_null) ? NULL : exprDesc;

	const dsc* startDesc = EVL_expr(tdbb, request, start);
	startDesc = (request->req_flags & req_null) ? NULL : startDesc;

	const dsc* lengthDesc = EVL_expr(tdbb, request, length);
	lengthDesc = (request->req_flags & req_null) ? NULL : lengthDesc;

	if (exprDesc && startDesc && lengthDesc)
		return perform(tdbb, impure, exprDesc, startDesc, lengthDesc);

	// If any of them is NULL, return NULL.
	return NULL;
}

dsc* SubstringNode::perform(thread_db* tdbb, impure_value* impure, const dsc* valueDsc,
	const dsc* startDsc, const dsc* lengthDsc)
{
	const SLONG sStart = MOV_get_long(startDsc, 0);
	const SLONG sLength = MOV_get_long(lengthDsc, 0);

	if (sStart < 0)
		status_exception::raise(Arg::Gds(isc_bad_substring_offset) << Arg::Num(sStart + 1));
	else if (sLength < 0)
		status_exception::raise(Arg::Gds(isc_bad_substring_length) << Arg::Num(sLength));

	dsc desc;
	DataTypeUtil(tdbb).makeSubstr(&desc, valueDsc, startDsc, lengthDsc);

	ULONG start = (ULONG) sStart;
	ULONG length = (ULONG) sLength;

	if (desc.isText() && length > MAX_COLUMN_SIZE)
		length = MAX_COLUMN_SIZE;

	ULONG dataLen;

	if (valueDsc->isBlob())
	{
		// Source string is a blob, things get interesting.

		fb_assert(desc.dsc_dtype == dtype_blob);

		desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_bid;

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction, &impure->vlu_misc.vlu_bid);

		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
			reinterpret_cast<bid*>(valueDsc->dsc_address));

		HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;
		CharSet* charSet = INTL_charset_lookup(tdbb, valueDsc->getCharSet());

		const FB_UINT64 byte_offset = FB_UINT64(start) * charSet->maxBytesPerChar();
		const FB_UINT64 byte_length = FB_UINT64(length) * charSet->maxBytesPerChar();

		if (charSet->isMultiByte())
		{
			buffer.getBuffer(MIN(blob->blb_length, byte_offset + byte_length));
			dataLen = BLB_get_data(tdbb, blob, buffer.begin(), buffer.getCount(), false);

			HalfStaticArray<UCHAR, BUFFER_LARGE> buffer2;
			buffer2.getBuffer(dataLen);

			dataLen = charSet->substring(dataLen, buffer.begin(),
				buffer2.getCapacity(), buffer2.begin(), start, length);
			BLB_put_data(tdbb, newBlob, buffer2.begin(), dataLen);
		}
		else if (byte_offset < blob->blb_length)
		{
			start = byte_offset;
			length = MIN(blob->blb_length, byte_length);

			while (!(blob->blb_flags & BLB_eof) && start)
			{
				// Both cases are the same for now. Let's see if we can optimize in the future.
				ULONG l1 = BLB_get_data(tdbb, blob, buffer.begin(),
					MIN(buffer.getCapacity(), start), false);
				start -= l1;
			}

			while (!(blob->blb_flags & BLB_eof) && length)
			{
				dataLen = BLB_get_data(tdbb, blob, buffer.begin(),
					MIN(length, buffer.getCapacity()), false);
				length -= dataLen;

				BLB_put_data(tdbb, newBlob, buffer.begin(), dataLen);
			}
		}

		BLB_close(tdbb, blob);
		BLB_close(tdbb, newBlob);

		EVL_make_value(tdbb, &desc, impure);
	}
	else
	{
		fb_assert(desc.isText());

		desc.dsc_dtype = dtype_text;

		// CVC: I didn't bother to define a larger buffer because:
		//		- Native types when converted to string don't reach 31 bytes plus terminator.
		//		- String types do not need and do not use the buffer ("temp") to be pulled.
		//		- The types that can cause an error() issued inside the low level MOV/CVT
		//		routines because the "temp" is not enough are blob and array but at this time
		//		they aren't accepted, so they will cause error() to be called anyway.
		VaryStr<32> temp;
		USHORT ttype;
		desc.dsc_length = MOV_get_string_ptr(valueDsc, &ttype, &desc.dsc_address,
			&temp, sizeof(temp));
		desc.setTextType(ttype);

		// CVC: Why bother? If the start is greater or equal than the length in bytes,
		// it's impossible that the start be less than the length in an international charset.
		if (start >= desc.dsc_length || !length)
		{
			desc.dsc_length = 0;
			EVL_make_value(tdbb, &desc, impure);
		}
		// CVC: God save the king if the engine doesn't protect itself against buffer overruns,
		//		because intl.h defines UNICODE as the type of most system relations' string fields.
		//		Also, the field charset can come as 127 (dynamic) when it comes from system triggers,
		//		but it's resolved by INTL_obj_lookup() to UNICODE_FSS in the cases I observed. Here I cannot
		//		distinguish between user calls and system calls. Unlike the original ASCII substring(),
		//		this one will get correctly the amount of UNICODE characters requested.
		else if (ttype == ttype_ascii || ttype == ttype_none || ttype == ttype_binary)
		{
			/* Redundant.
			if (start >= desc.dsc_length)
				desc.dsc_length = 0;
			else */
			desc.dsc_address += start;
			desc.dsc_length -= start;
			if (length < desc.dsc_length)
				desc.dsc_length = length;
			EVL_make_value(tdbb, &desc, impure);
		}
		else
		{
			// CVC: ATTENTION:
			// I couldn't find an appropriate message for this failure among current registered
			// messages, so I will return empty.
			// Finally I decided to use arithmetic exception or numeric overflow.
			const UCHAR* p = desc.dsc_address;
			const USHORT pcount = desc.dsc_length;

			CharSet* charSet = INTL_charset_lookup(tdbb, desc.getCharSet());

			desc.dsc_address = NULL;
			const ULONG totLen = MIN(MAX_COLUMN_SIZE, length * charSet->maxBytesPerChar());
			desc.dsc_length = totLen;
			EVL_make_value(tdbb, &desc, impure);

			dataLen = charSet->substring(pcount, p, totLen,
				impure->vlu_desc.dsc_address, start, length);
			impure->vlu_desc.dsc_length = static_cast<USHORT>(dataLen);
		}
	}

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<SubstringSimilarNode> regSubstringSimilarNode(blr_substring_similar);

SubstringSimilarNode::SubstringSimilarNode(MemoryPool& pool, dsql_nod* aExpr, dsql_nod* aPattern,
			dsql_nod* aEscape)
	: TypedNode<ValueExprNode, ExprNode::TYPE_SUBSTRING_SIMILAR>(pool),
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
	node->expr = PAR_parse_value(tdbb, csb);
	node->pattern = PAR_parse_value(tdbb, csb);
	node->escape = PAR_parse_value(tdbb, csb);
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
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlExpr, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlPattern, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlEscape, node, forceVarChar);
}

void SubstringSimilarNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_substring_similar);
	GEN_expr(dsqlScratch, dsqlExpr);
	GEN_expr(dsqlScratch, dsqlPattern);
	GEN_expr(dsqlScratch, dsqlEscape);
}

void SubstringSimilarNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	MAKE_desc(dsqlScratch, desc, dsqlExpr);
	desc->setNullable(true);
}

void SubstringSimilarNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	expr->getDesc(tdbb, csb, desc);

	dsc tempDesc;
	pattern->getDesc(tdbb, csb, &tempDesc);
	escape->getDesc(tdbb, csb, &tempDesc);
}

ValueExprNode* SubstringSimilarNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	SubstringSimilarNode* node = FB_NEW(*tdbb->getDefaultPool()) SubstringSimilarNode(
		*tdbb->getDefaultPool());
	node->expr = copier.copy(tdbb, expr);
	node->pattern = copier.copy(tdbb, pattern);
	node->escape = copier.copy(tdbb, escape);
	return node;
}

ValueExprNode* SubstringSimilarNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	doPass1(tdbb, csb, expr.getAddress());

	// We need to take care of invariantness expressions to be able to pre-compile the pattern.
	nodFlags |= FLAG_INVARIANT;
	csb->csb_current_nodes.push(this);

	doPass1(tdbb, csb, pattern.getAddress());
	doPass1(tdbb, csb, escape.getAddress());

	csb->csb_current_nodes.pop();

	// If there is no top-level RSE present and patterns are not constant, unmark node as invariant
	// because it may be dependent on data or variables.
	if ((nodFlags & FLAG_INVARIANT) && (!pattern->is<LiteralNode>() || !escape->is<LiteralNode>()))
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

	return this;
}

ValueExprNode* SubstringSimilarNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	if (nodFlags & FLAG_INVARIANT)
		csb->csb_invariants.push(&impureOffset);

	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* SubstringSimilarNode::execute(thread_db* tdbb, jrd_req* request) const
{
	// Run all expression arguments.

	const dsc* exprDesc = EVL_expr(tdbb, request, expr);
	exprDesc = (request->req_flags & req_null) ? NULL : exprDesc;

	const dsc* patternDesc = EVL_expr(tdbb, request, pattern);
	patternDesc = (request->req_flags & req_null) ? NULL : patternDesc;

	const dsc* escapeDesc = EVL_expr(tdbb, request, escape);
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

	// Verify the correctness of the escape character.
	if (escapeLen == 0 || charSet->length(escapeLen, escapeStr, true) != 1)
		ERR_post(Arg::Gds(isc_escape_invalid));

	impure_value* impure = request->getImpure<impure_value>(impureOffset);

	AutoPtr<BaseSubstringSimilarMatcher> autoEvaluator;	// deallocate non-invariant evaluator
	BaseSubstringSimilarMatcher* evaluator;

	if (nodFlags & FLAG_INVARIANT)
	{
		if (!(impure->vlu_flags & VLU_computed))
		{
			delete impure->vlu_misc.vlu_invariant;

			impure->vlu_misc.vlu_invariant = evaluator = collation->createSubstringSimilarMatcher(
				*tdbb->getDefaultPool(), patternStr, patternLen, escapeStr, escapeLen);

			impure->vlu_flags |= VLU_computed;
		}
		else
		{
			evaluator = static_cast<BaseSubstringSimilarMatcher*>(impure->vlu_misc.vlu_invariant);
			evaluator->reset();
		}
	}
	else
	{
		autoEvaluator = evaluator = collation->createSubstringSimilarMatcher(*tdbb->getDefaultPool(),
			patternStr, patternLen, escapeStr, escapeLen);
	}

	evaluator->process(exprStr, exprLen);

	if (evaluator->result())
	{
		// Get the byte bounds of the matched substring.
		unsigned start = 0;
		unsigned length = 0;
		evaluator->getResultInfo(&start, &length);

		dsc desc;
		desc.makeText((USHORT) exprLen, textType);
		EVL_make_value(tdbb, &desc, impure);

		// And return it.
		memcpy(impure->vlu_desc.dsc_address, exprStr + start, length);
		impure->vlu_desc.dsc_length = length;

		return &impure->vlu_desc;
	}
	else
		return NULL;	// No match. Return NULL.
}

ValueExprNode* SubstringSimilarNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	SubstringSimilarNode* node = FB_NEW(getPool()) SubstringSimilarNode(getPool(),
		PASS1_node(dsqlScratch, dsqlExpr),
		PASS1_node(dsqlScratch, dsqlPattern),
		PASS1_node(dsqlScratch, dsqlEscape));

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
	: TypedNode<ValueExprNode, ExprNode::TYPE_SYSFUNC_CALL>(pool),
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

	node->args = PAR_args(tdbb, csb);

	return node;
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

void SysFuncCallNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_sys_function);
	dsqlScratch->appendMetaString(function->name.c_str());
	dsqlScratch->appendUChar(dsqlArgs->nod_count);

	dsql_nod* const* ptr = dsqlArgs->nod_arg;
	for (const dsql_nod* const* const end = ptr + dsqlArgs->nod_count; ptr < end; ptr++)
		GEN_expr(dsqlScratch, *ptr);
}

void SysFuncCallNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	Array<const dsc*> argsArray;

	for (dsql_nod** p = dsqlArgs->nod_arg; p < dsqlArgs->nod_arg + dsqlArgs->nod_count; ++p)
	{
		MAKE_desc(dsqlScratch, &(*p)->nod_desc, *p);
		argsArray.add(&(*p)->nod_desc);
	}

	DSqlDataTypeUtil(dsqlScratch).makeSysFunction(desc, name.c_str(),
		argsArray.getCount(), argsArray.begin());
}

void SysFuncCallNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	Array<const dsc*> argsArray;

	for (NestConst<ValueExprNode>* p = args->args.begin(); p != args->args.end(); ++p)
	{
		dsc* targetDesc = FB_NEW(*tdbb->getDefaultPool()) dsc();
		argsArray.push(targetDesc);
		(*p)->getDesc(tdbb, csb, targetDesc);

		// dsc_address is verified in makeFunc to get literals. If the node is not a
		// literal, set it to NULL, to prevent wrong interpretation of offsets as
		// pointers - CORE-2612.
		if (!(*p)->is<LiteralNode>())
			targetDesc->dsc_address = NULL;
	}

	DataTypeUtil dataTypeUtil(tdbb);
	function->makeFunc(&dataTypeUtil, function, desc, argsArray.getCount(), argsArray.begin());

	for (const dsc** pArgs = argsArray.begin(); pArgs != argsArray.end(); ++pArgs)
		delete *pArgs;
}

ValueExprNode* SysFuncCallNode::copy(thread_db* tdbb, NodeCopier& copier) const
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

bool SysFuncCallNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	SysFuncCallNode* otherNode = other->as<SysFuncCallNode>();
	fb_assert(otherNode);

	return function && function == otherNode->function;
}

ValueExprNode* SysFuncCallNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	function->checkArgsMismatch(args->args.getCount());

	return this;
}

dsc* SysFuncCallNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(impureOffset);
	return function->evlFunc(tdbb, function, args->args, impure);
}

ValueExprNode* SysFuncCallNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	QualifiedName qualifName(name);

	if (!dsqlSpecialSyntax && METD_get_function(dsqlScratch->getTransaction(), dsqlScratch, qualifName))
	{
		UdfCallNode* node = FB_NEW(getPool()) UdfCallNode(getPool(), qualifName, dsqlArgs);
		return node->dsqlPass(dsqlScratch);
	}

	SysFuncCallNode* node = FB_NEW(getPool()) SysFuncCallNode(getPool(), name,
		PASS1_node(dsqlScratch, dsqlArgs));
	node->dsqlSpecialSyntax = dsqlSpecialSyntax;

	node->function = SysFunction::lookup(name);

	if (node->function && node->function->setParamsFunc)
	{
		Array<dsc*> argsArray;
		dsql_nod* inArgs = node->dsqlArgs;

		for (unsigned int i = 0; i < inArgs->nod_count; ++i)
		{
			dsql_nod* p = inArgs->nod_arg[i];
			MAKE_desc(dsqlScratch, &p->nod_desc, p);
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


static RegisterNode<TrimNode> regTrimNode(blr_trim);

TrimNode::TrimNode(MemoryPool& pool, UCHAR aWhere, dsql_nod* aValue, dsql_nod* aTrimChars)
	: TypedNode<ValueExprNode, ExprNode::TYPE_TRIM>(pool),
	  where(aWhere),
	  dsqlValue(aValue),
	  dsqlTrimChars(aTrimChars),
	  value(NULL),
	  trimChars(NULL)
{
	addChildNode(dsqlValue, value);
	addChildNode(dsqlTrimChars, trimChars);
}

DmlNode* TrimNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	UCHAR where = csb->csb_blr_reader.getByte();
	UCHAR what = csb->csb_blr_reader.getByte();

	TrimNode* node = FB_NEW(pool) TrimNode(pool, where);

	if (what == blr_trim_characters)
		node->trimChars = PAR_parse_value(tdbb, csb);

	node->value = PAR_parse_value(tdbb, csb);

	return node;
}

void TrimNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "TrimNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* TrimNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	TrimNode* node = FB_NEW(getPool()) TrimNode(getPool(), where,
		PASS1_node(dsqlScratch, dsqlValue), PASS1_node(dsqlScratch, dsqlTrimChars));

	// Try to force trimChars to be same type as value: TRIM(? FROM FIELD)
	PASS1_set_parameter_type(dsqlScratch, node->dsqlTrimChars, node->dsqlValue, false);

	return node;
}

void TrimNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "TRIM";
}

bool TrimNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlValue, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlTrimChars, node, forceVarChar);
}

void TrimNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_trim);
	dsqlScratch->appendUChar(where);

	if (dsqlTrimChars)
	{
		dsqlScratch->appendUChar(blr_trim_characters);
		GEN_expr(dsqlScratch, dsqlTrimChars);
	}
	else
		dsqlScratch->appendUChar(blr_trim_spaces);

	GEN_expr(dsqlScratch, dsqlValue);
}

void TrimNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	dsc desc1, desc2;

	MAKE_desc(dsqlScratch, &desc1, dsqlValue);

	if (dsqlTrimChars)
		MAKE_desc(dsqlScratch, &desc2, dsqlTrimChars);
	else
		desc2.dsc_flags = 0;

	if (desc1.dsc_dtype == dtype_blob)
	{
		*desc = desc1;
		desc->dsc_flags |= (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
	}
	else if (desc1.dsc_dtype <= dtype_any_text)
	{
		*desc = desc1;
		desc->dsc_dtype = dtype_varying;
		desc->dsc_length = sizeof(USHORT) + DSC_string_length(&desc1);
		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
	}
	else
	{
		desc->dsc_dtype = dtype_varying;
		desc->dsc_scale = 0;
		desc->dsc_ttype() = ttype_ascii;
		desc->dsc_length = sizeof(USHORT) + DSC_string_length(&desc1);
		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
	}
}

void TrimNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	value->getDesc(tdbb, csb, desc);

	if (trimChars)
	{
		dsc desc1;
		trimChars->getDesc(tdbb, csb, &desc1);
		desc->dsc_flags |= desc1.dsc_flags & DSC_null;
	}

	if (desc->dsc_dtype != dtype_blob)
	{
		USHORT length = DSC_string_length(desc);

		if (!DTYPE_IS_TEXT(desc->dsc_dtype))
		{
			desc->dsc_ttype() = ttype_ascii;
			desc->dsc_scale = 0;
		}

		desc->dsc_dtype = dtype_varying;
		desc->dsc_length = length + sizeof(USHORT);
	}
}

ValueExprNode* TrimNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	TrimNode* node = FB_NEW(*tdbb->getDefaultPool()) TrimNode(*tdbb->getDefaultPool(), where);
	node->value = copier.copy(tdbb, value);
	if (trimChars)
		node->trimChars = copier.copy(tdbb, trimChars);
	return node;
}

bool TrimNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const TrimNode* o = other->as<TrimNode>();
	fb_assert(o);

	return where == o->where;
}

bool TrimNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	TrimNode* o = other->as<TrimNode>();
	fb_assert(o);

	return where == o->where;
}

ValueExprNode* TrimNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Perform trim function = TRIM([where what FROM] string).
dsc* TrimNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(impureOffset);
	request->req_flags &= ~req_null;

	dsc* trimCharsDesc = (trimChars ? EVL_expr(tdbb, request, trimChars) : NULL);
	if (request->req_flags & req_null)
		return NULL;

	dsc* valueDesc = EVL_expr(tdbb, request, value);
	if (request->req_flags & req_null)
		return NULL;

	USHORT ttype = INTL_TEXT_TYPE(*valueDesc);
	TextType* tt = INTL_texttype_lookup(tdbb, ttype);
	CharSet* cs = tt->getCharSet();

	const UCHAR* charactersAddress;
	MoveBuffer charactersBuffer;
	USHORT charactersLength;

	if (trimCharsDesc)
	{
		UCHAR* tempAddress = NULL;

		if (trimCharsDesc->isBlob())
		{
			UCharBuffer bpb;
			CharSet* charsCharSet;

			if (trimCharsDesc->getBlobSubType() == isc_blob_text)
			{
				BLB_gen_bpb_from_descs(trimCharsDesc, valueDesc, bpb);
				charsCharSet = INTL_charset_lookup(tdbb, trimCharsDesc->getCharSet());
			}
			else
				charsCharSet = cs;

			blb* blob = BLB_open2(tdbb, request->req_transaction,
				reinterpret_cast<bid*>(trimCharsDesc->dsc_address), bpb.getCount(), bpb.begin());

			// Go simple way and always read entire blob in memory.

			unsigned maxLen = blob->blb_length / charsCharSet->minBytesPerChar() *
				cs->maxBytesPerChar();

			tempAddress = charactersBuffer.getBuffer(maxLen);
			charactersLength = BLB_get_data(tdbb, blob, tempAddress, maxLen, true);
		}
		else
		{
			charactersLength = MOV_make_string2(tdbb, trimCharsDesc, ttype,
				&tempAddress, charactersBuffer);
		}

		charactersAddress = tempAddress;
	}
	else
	{
		charactersLength = tt->getCharSet()->getSpaceLength();
		charactersAddress = tt->getCharSet()->getSpace();
	}

	HalfStaticArray<UCHAR, BUFFER_SMALL> charactersCanonical;
	charactersCanonical.getBuffer(charactersLength / tt->getCharSet()->minBytesPerChar() *
		tt->getCanonicalWidth());
	const SLONG charactersCanonicalLen = tt->canonical(charactersLength, charactersAddress,
		charactersCanonical.getCount(), charactersCanonical.begin()) * tt->getCanonicalWidth();

	MoveBuffer valueBuffer;
	UCHAR* valueAddress;
	ULONG valueLength;

	if (valueDesc->isBlob())
	{
		// Source string is a blob, things get interesting.
		blb* blob = BLB_open(tdbb, request->req_transaction,
			reinterpret_cast<bid*>(valueDesc->dsc_address));

		// It's very difficult (and probably not very efficient) to trim a blob in chunks.
		// So go simple way and always read entire blob in memory.
		valueAddress = valueBuffer.getBuffer(blob->blb_length);
		valueLength = BLB_get_data(tdbb, blob, valueAddress, blob->blb_length, true);
	}
	else
		valueLength = MOV_make_string2(tdbb, valueDesc, ttype, &valueAddress, valueBuffer);

	HalfStaticArray<UCHAR, BUFFER_SMALL> valueCanonical;
	valueCanonical.getBuffer(valueLength / cs->minBytesPerChar() * tt->getCanonicalWidth());
	const SLONG valueCanonicalLen = tt->canonical(valueLength, valueAddress,
		valueCanonical.getCount(), valueCanonical.begin()) * tt->getCanonicalWidth();

	SLONG offsetLead = 0;
	SLONG offsetTrail = valueCanonicalLen;

	// CVC: Avoid endless loop with zero length trim chars.
	if (charactersCanonicalLen)
	{
		if (where == blr_trim_both || where == blr_trim_leading)
		{
			// CVC: Prevent surprises with offsetLead < valueCanonicalLen; it may fail.
			for (; offsetLead + charactersCanonicalLen <= valueCanonicalLen;
				 offsetLead += charactersCanonicalLen)
			{
				if (memcmp(charactersCanonical.begin(), &valueCanonical[offsetLead],
						charactersCanonicalLen) != 0)
				{
					break;
				}
			}
		}

		if (where == blr_trim_both || where == blr_trim_trailing)
		{
			for (; offsetTrail - charactersCanonicalLen >= offsetLead;
				 offsetTrail -= charactersCanonicalLen)
			{
				if (memcmp(charactersCanonical.begin(),
						&valueCanonical[offsetTrail - charactersCanonicalLen],
						charactersCanonicalLen) != 0)
				{
					break;
				}
			}
		}
	}

	if (valueDesc->isBlob())
	{
		// We have valueCanonical already allocated.
		// Use it to get the substring that will be written to the new blob.
		const ULONG len = cs->substring(valueLength, valueAddress,
			valueCanonical.getCapacity(), valueCanonical.begin(),
			offsetLead / tt->getCanonicalWidth(),
			(offsetTrail - offsetLead) / tt->getCanonicalWidth());

		EVL_make_value(tdbb, valueDesc, impure);

		blb* newBlob = BLB_create(tdbb, tdbb->getRequest()->req_transaction, &impure->vlu_misc.vlu_bid);
		BLB_put_data(tdbb, newBlob, valueCanonical.begin(), len);
		BLB_close(tdbb, newBlob);
	}
	else
	{
		dsc desc;
		desc.makeText(valueLength, ttype);
		EVL_make_value(tdbb, &desc, impure);

		impure->vlu_desc.dsc_length = cs->substring(valueLength, valueAddress,
			impure->vlu_desc.dsc_length, impure->vlu_desc.dsc_address,
			offsetLead / tt->getCanonicalWidth(),
			(offsetTrail - offsetLead) / tt->getCanonicalWidth());
	}

	return &impure->vlu_desc;
}


//--------------------


static RegisterNode<UdfCallNode> regUdfCallNode1(blr_function);
static RegisterNode<UdfCallNode> regUdfCallNode2(blr_function2);

UdfCallNode::UdfCallNode(MemoryPool& pool, const QualifiedName& aName, dsql_nod* aArgs)
	: TypedNode<ValueExprNode, ExprNode::TYPE_UDF_CALL>(pool),
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
		if (!node->function->isUndefined() && !node->function->isImplemented())
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
		CompilerScratch::Dependency dependency(obj_udf);
		dependency.function = node->function;
		csb->csb_dependencies.push(dependency);
	}

	return node;
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

void UdfCallNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
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

void UdfCallNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
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

ValueExprNode* UdfCallNode::copy(thread_db* tdbb, NodeCopier& copier) const
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

bool UdfCallNode::sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other)
{
	if (!ExprNode::sameAs(tdbb, csb, other))
		return false;

	UdfCallNode* otherNode = other->as<UdfCallNode>();
	fb_assert(otherNode);

	return function && function == otherNode->function;
}

ValueExprNode* UdfCallNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass1(tdbb, csb);

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

ValueExprNode* UdfCallNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = function->allocateImpure(csb);

	return this;
}

dsc* UdfCallNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* impure = request->getImpure<impure_value>(impureOffset);
	const bool invariant = (nodFlags & FLAG_INVARIANT);
	return function->execute(tdbb, args->args, impure, invariant);
}

ValueExprNode* UdfCallNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	UdfCallNode* node = FB_NEW(getPool()) UdfCallNode(getPool(), name,
		PASS1_node(dsqlScratch, dsqlArgs));
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


//--------------------


static RegisterNode<ValueIfNode> regValueIfNode(blr_value_if);

ValueIfNode::ValueIfNode(MemoryPool& pool, dsql_nod* aCondition, dsql_nod* aTrueValue,
			dsql_nod* aFalseValue)
	: TypedNode<ValueExprNode, ExprNode::TYPE_VALUE_IF>(pool),
	  dsqlCondition(aCondition),
	  dsqlTrueValue(aTrueValue),
	  dsqlFalseValue(aFalseValue),
	  condition(NULL),
	  trueValue(NULL),
	  falseValue(NULL)
{
	addChildNode(dsqlCondition, condition);
	addChildNode(dsqlTrueValue, trueValue);
	addChildNode(dsqlFalseValue, falseValue);
}

DmlNode* ValueIfNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
{
	ValueIfNode* node = FB_NEW(pool) ValueIfNode(pool);
	node->condition = PAR_parse_boolean(tdbb, csb);
	node->trueValue = PAR_parse_value(tdbb, csb);
	node->falseValue = PAR_parse_value(tdbb, csb);

	// Get rid of blr_stmt_expr expressions.

	MissingBoolNode* missing = node->condition->as<MissingBoolNode>();
	if (!missing)
		return node;

	StmtExprNode* missingCond = missing->arg->as<StmtExprNode>();
	if (!missingCond)
		return node;

	CompoundStmtNode* stmt = missingCond->stmt->as<CompoundStmtNode>();
	DeclareVariableNode* declStmt = NULL;
	AssignmentNode* assignStmt;

	if (stmt)
	{
		if (stmt->statements.getCount() != 2 ||
			!(declStmt = stmt->statements[0]->as<DeclareVariableNode>()) ||
			!(assignStmt = stmt->statements[1]->as<AssignmentNode>()))
		{
			return node;
		}
	}
	else if (!(assignStmt = missingCond->stmt->as<AssignmentNode>()))
		return node;

	VariableNode* var = node->falseValue->as<VariableNode>();
	VariableNode* var2 = assignStmt->asgnTo->as<VariableNode>();

	if (!var || !var2 || var->varId != var2->varId || (declStmt && declStmt->varId != var->varId))
		return node;

	CoalesceNode* coalesceNode = FB_NEW(pool) CoalesceNode(pool);
	coalesceNode->args = FB_NEW(pool) ValueListNode(pool, 2);
	coalesceNode->args->args[0] = assignStmt->asgnFrom;
	coalesceNode->args->args[1] = node->trueValue;

	return coalesceNode;
}

void ValueIfNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "ValueIfNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* ValueIfNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	ValueIfNode* node = FB_NEW(getPool()) ValueIfNode(getPool(),
		PASS1_node(dsqlScratch, dsqlCondition),
		PASS1_node(dsqlScratch, dsqlTrueValue),
		PASS1_node(dsqlScratch, dsqlFalseValue));

	PASS1_set_parameter_type(dsqlScratch, node->dsqlTrueValue, node->dsqlFalseValue, false);
	PASS1_set_parameter_type(dsqlScratch, node->dsqlFalseValue, node->dsqlTrueValue, false);

	return node;
}

void ValueIfNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = "CASE";
}

bool ValueIfNode::setParameterType(DsqlCompilerScratch* dsqlScratch,
	dsql_nod* node, bool forceVarChar)
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlTrueValue, node, forceVarChar) |
		PASS1_set_parameter_type(dsqlScratch, dsqlFalseValue, node, forceVarChar);
}

void ValueIfNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_value_if);
	GEN_expr(dsqlScratch, dsqlCondition);
	GEN_expr(dsqlScratch, dsqlTrueValue);
	GEN_expr(dsqlScratch, dsqlFalseValue);
}

void ValueIfNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
{
	Array<const dsc*> args;

	MAKE_desc(dsqlScratch, &dsqlTrueValue->nod_desc, dsqlTrueValue);
	args.add(&dsqlTrueValue->nod_desc);

	MAKE_desc(dsqlScratch, &dsqlFalseValue->nod_desc, dsqlFalseValue);
	args.add(&dsqlFalseValue->nod_desc);

	DSqlDataTypeUtil(dsqlScratch).makeFromList(desc, "CASE", args.getCount(), args.begin());
}

void ValueIfNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	ValueExprNode* val = trueValue->is<NullNode>() ? falseValue : trueValue;
	val->getDesc(tdbb, csb, desc);
}

ValueExprNode* ValueIfNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ValueIfNode* node = FB_NEW(*tdbb->getDefaultPool()) ValueIfNode(*tdbb->getDefaultPool());
	node->condition = copier.copy(tdbb, condition);
	node->trueValue = copier.copy(tdbb, trueValue);
	node->falseValue = copier.copy(tdbb, falseValue);
	return node;
}

ValueExprNode* ValueIfNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	impureOffset = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* ValueIfNode::execute(thread_db* tdbb, jrd_req* request) const
{
	return EVL_expr(tdbb, request, (condition->execute(tdbb, request) ? trueValue : falseValue));
}


//--------------------


ValueListNode::ValueListNode(MemoryPool& pool, unsigned count)
	: TypedNode<ValueExprNode, ExprNode::TYPE_VALUE_LIST>(pool),
	  args(pool)
{
	args.resize(count);

	for (unsigned i = 0; i < count; ++i)
		addChildNode((args[i] = NULL));
}

ValueListNode* ValueListNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	ValueListNode* node = FB_NEW(*tdbb->getDefaultPool()) ValueListNode(*tdbb->getDefaultPool(),
		args.getCount());

	NestConst<ValueExprNode>* j = node->args.begin();

	for (const NestConst<ValueExprNode>* i = args.begin(); i != args.end(); ++i, ++j)
		*j = copier.copy(tdbb, *i);

	return node;
}


//--------------------


static RegisterNode<VariableNode> regVariableNode(blr_variable);

VariableNode::VariableNode(MemoryPool& pool)
	: TypedNode<ValueExprNode, ExprNode::TYPE_VARIABLE>(pool),
	  dsqlVar(NULL),
	  varId(0),
	  varDecl(NULL),
	  varInfo(NULL)
{
}

DmlNode* VariableNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
{
	USHORT n = csb->csb_blr_reader.getWord();
	vec<DeclareVariableNode*>* vector = csb->csb_variables;

	if (!vector || n >= vector->count())
		PAR_syntax_error(csb, "variable identifier");

	VariableNode* node = FB_NEW(pool) VariableNode(pool);
	node->varId = n;

	return node;
}

void VariableNode::print(string& text, Array<dsql_nod*>& nodes) const
{
	text = "VariableNode";
	ExprNode::print(text, nodes);
}

ValueExprNode* VariableNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	VariableNode* node = FB_NEW(getPool()) VariableNode(getPool());
	node->dsqlVar = dsqlVar;

	return node;
}

void VariableNode::setParameterName(dsql_par* parameter) const
{
	parameter->par_name = parameter->par_alias = dsqlVar->field->fld_name.c_str();
}

void VariableNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (dsqlVar->type == dsql_var::TYPE_INPUT)
	{
		dsqlScratch->appendUChar(blr_parameter2);
		dsqlScratch->appendUChar(dsqlVar->msgNumber);
		dsqlScratch->appendUShort(dsqlVar->msgItem);
		dsqlScratch->appendUShort(dsqlVar->msgItem + 1);
	}
	else
	{
		dsqlScratch->appendUChar(blr_variable);
		dsqlScratch->appendUShort(dsqlVar->number);
	}
}

void VariableNode::make(DsqlCompilerScratch* /*dsqlScratch*/, dsc* desc)
{
	*desc = dsqlVar->desc;
}

bool VariableNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	const VariableNode* o = other->as<VariableNode>();
	if (!o)
		return false;

	if (dsqlVar->name != o->dsqlVar->name ||
		dsqlVar->field != o->dsqlVar->field ||
		dsqlVar->number != o->dsqlVar->number ||
		dsqlVar->msgItem != o->dsqlVar->msgItem ||
		dsqlVar->msgNumber != o->dsqlVar->msgNumber)
	{
		return false;
	}

	return true;
}

void VariableNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	*desc = varDecl->varDesc;
}

ValueExprNode* VariableNode::copy(thread_db* tdbb, NodeCopier& copier) const
{
	VariableNode* node = FB_NEW(*tdbb->getDefaultPool()) VariableNode(*tdbb->getDefaultPool());
	node->varId = copier.csb->csb_remap_variable + varId;
	node->varDecl = varDecl;
	node->varInfo = varInfo;

	return node;
}

ValueExprNode* VariableNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	ValueExprNode::pass1(tdbb, csb);

	vec<DeclareVariableNode*>* vector = csb->csb_variables;

	if (!vector || varId >= vector->count() || !(varDecl = (*vector)[varId]))
		PAR_syntax_error(csb, "variable identifier");

	return this;
}

ValueExprNode* VariableNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	varInfo = CMP_pass2_validation(tdbb, csb, Item(Item::TYPE_VARIABLE, varId));

	ValueExprNode::pass2(tdbb, csb);

	impureOffset = CMP_impure(csb, (nodFlags & FLAG_VALUE) ? sizeof(impure_value_ex) : sizeof(dsc));

	return this;
}

dsc* VariableNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(impureOffset);
	impure_value* impure2 = request->getImpure<impure_value>(varDecl->impureOffset);

	request->req_flags &= ~req_null;

	if (impure2->vlu_desc.dsc_flags & DSC_null)
		request->req_flags |= req_null;

	impure->vlu_desc = impure2->vlu_desc;

	if (impure->vlu_desc.dsc_dtype == dtype_text)
		INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);

	if (!(impure2->vlu_flags & VLU_checked))
	{
		if (varInfo)
		{
			EVL_validate(tdbb, Item(Item::TYPE_VARIABLE, varId), varInfo,
				&impure->vlu_desc, (impure->vlu_desc.dsc_flags & DSC_null));
		}

		impure2->vlu_flags |= VLU_checked;
	}

	return (request->req_flags & req_null) ? NULL : &impure->vlu_desc;
}


//--------------------


// Firebird provides transparent conversion from string to date in
// contexts where it makes sense.  This macro checks a descriptor to
// see if it is something that *could* represent a date value

static bool couldBeDate(const dsc desc)
{
	return DTYPE_IS_DATE(desc.dsc_dtype) || desc.dsc_dtype <= dtype_any_text;
}

// Take the input number, assume it represents a fractional count of days.
// Convert it to a count of microseconds.
static SINT64 getDayFraction(const dsc* d)
{
	dsc result;
	double result_days;

	result.dsc_dtype = dtype_double;
	result.dsc_scale = 0;
	result.dsc_flags = 0;
	result.dsc_sub_type = 0;
	result.dsc_length = sizeof(double);
	result.dsc_address = reinterpret_cast<UCHAR*>(&result_days);

	// Convert the input number to a double
	CVT_move(d, &result);

	// There's likely some loss of precision here due to rounding of number

	// 08-Apr-2004, Nickolay Samofatov. Loss of precision manifested itself as bad
	// result returned by the following query:
	//
	// select (cast('01.01.2004 10:01:00' as timestamp)
	//   -cast('01.01.2004 10:00:00' as timestamp))
	//   +cast('01.01.2004 10:00:00' as timestamp) from rdb$database
	//
	// Let's use llrint where it is supported and offset number for other platforms
	// in hope that compiler rounding mode doesn't get in.

#ifdef HAVE_LLRINT
	return llrint(result_days * ISC_TICKS_PER_DAY);
#else
	const double eps = 0.49999999999999;
	if (result_days >= 0)
		return (SINT64)(result_days * ISC_TICKS_PER_DAY + eps);

	return (SINT64) (result_days * ISC_TICKS_PER_DAY - eps);
#endif
}

// Take the input value, which is either a timestamp or a string representing a timestamp.
// Convert it to a timestamp, and then return that timestamp as a count of isc_ticks since the base
// date and time in MJD time arithmetic.
// ISC_TICKS or isc_ticks are actually deci - milli seconds or tenthousandth of seconds per day.
// This is derived from the ISC_TIME_SECONDS_PRECISION.
static SINT64 getTimeStampToIscTicks(const dsc* d)
{
	dsc result;
	GDS_TIMESTAMP result_timestamp;

	result.dsc_dtype = dtype_timestamp;
	result.dsc_scale = 0;
	result.dsc_flags = 0;
	result.dsc_sub_type = 0;
	result.dsc_length = sizeof(GDS_TIMESTAMP);
	result.dsc_address = reinterpret_cast<UCHAR*>(&result_timestamp);

	CVT_move(d, &result);

	return ((SINT64) result_timestamp.timestamp_date) * ISC_TICKS_PER_DAY +
		(SINT64) result_timestamp.timestamp_time;
}

// One of d1, d2 is time, the other is date
static bool isDateAndTime(const dsc& d1, const dsc& d2)
{
	return ((d1.dsc_dtype == dtype_sql_time && d2.dsc_dtype == dtype_sql_date) ||
		(d2.dsc_dtype == dtype_sql_time && d1.dsc_dtype == dtype_sql_date));
}


}	// namespace Jrd
