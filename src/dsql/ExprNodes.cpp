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
#include "../jrd/common.h"
#include "../common/classes/FpeControl.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/node.h"
#include "../jrd/align.h"
#include "../jrd/blr.h"
#include "../jrd/quad.h"
#include "../jrd/tra.h"
#include "../jrd/Function.h"
#include "../jrd/SysFunction.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/intl_proto.h"
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

#include "gen/blrtable.h"

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


ExprNode* ExprNode::fromLegacy(const dsql_nod* node)
{
	return node && node->nod_type == Dsql::nod_class_exprnode ?
		reinterpret_cast<ExprNode*>(node->nod_arg[0]) : NULL;
}

const ExprNode* ExprNode::fromLegacy(const jrd_nod* node)
{
	return node && node->nod_type == nod_class_exprnode_jrd ?
		reinterpret_cast<const ExprNode*>(node->nod_arg[0]) : NULL;
}

ExprNode* ExprNode::fromLegacy(jrd_nod* node)
{
	return node && node->nod_type == nod_class_exprnode_jrd ?
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
	for (NestConst<NestConst<jrd_nod> >* i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		**i = CMP_pass1(tdbb, csb, **i);
	return this;
}

ExprNode* ExprNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	for (NestConst<NestConst<jrd_nod> >* i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		**i = CMP_pass2(tdbb, csb, **i, node);
	return this;
}

bool ExprNode::jrdVisit(JrdNodeVisitor& visitor)
{
	bool ret = false;

	for (NestConst<NestConst<jrd_nod> >* i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		ret |= visitor.visit(**i);

	return ret;
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
	node->arg1 = PAR_parse_node(tdbb, csb, VALUE);
	node->arg2 = PAR_parse_node(tdbb, csb, VALUE);
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
	dsql_nod* node, bool forceVarChar) const
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

void ArithmeticNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
{
	dsc desc1, desc2;

	MAKE_desc(dsqlScratch, &desc1, dsqlArg1, dsqlArg2);
	MAKE_desc(dsqlScratch, &desc2, dsqlArg2, dsqlArg1);

	if (dsqlArg1->nod_type == Dsql::nod_null && dsqlArg2->nod_type == Dsql::nod_null)
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

	CMP_get_desc(tdbb, csb, arg1, &desc1);
	CMP_get_desc(tdbb, csb, arg2, &desc2);

	if (dialect1)
		getDescDialect1(tdbb, desc, desc1, desc2);
	else
		getDescDialect3(tdbb, desc, desc1, desc2);
}

void ArithmeticNode::getDescDialect1(thread_db* tdbb, dsc* desc, dsc& desc1, dsc& desc2)
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

					node->nod_scale = desc->dsc_scale;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_sql_date:
				case dtype_sql_time:
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						ERR_post(Arg::Gds(isc_expression_eval_err));
					// fall into

				case dtype_timestamp:
					node->nod_flags |= nod_date;

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
					node->nod_flags |= nod_double;
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
					node->nod_flags |= nod_quad;
					desc->dsc_dtype = dtype_quad;
					desc->dsc_length = sizeof(SQUAD);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					node->nod_scale = desc->dsc_scale;
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
					desc->dsc_scale = node->nod_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_double:
					node->nod_flags |= nod_double;
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
			break;
	}
}

void ArithmeticNode::getDescDialect3(thread_db* tdbb, dsc* desc, dsc& desc1, dsc& desc2)
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
					node->nod_flags |= nod_date;

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
					node->nod_flags |= nod_double;
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
					node->nod_scale = desc->dsc_scale;
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
					node->nod_flags |= nod_quad;
					desc->dsc_dtype = dtype_quad;
					desc->dsc_length = sizeof(SQUAD);
					if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
						desc->dsc_scale = 0;
					else
						desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
					node->nod_scale = desc->dsc_scale;
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
					node->nod_flags |= nod_double;
					desc->dsc_dtype = DEFAULT_DOUBLE;
					desc->dsc_length = sizeof(double);
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
					return;

				case dtype_int64:
					desc->dsc_dtype = dtype_int64;
					desc->dsc_length = sizeof(SINT64);
					desc->dsc_scale = node->nod_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
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
}

ValueExprNode* ArithmeticNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	ArithmeticNode* node = FB_NEW(*tdbb->getDefaultPool()) ArithmeticNode(*tdbb->getDefaultPool(),
		blrOp, dialect1);
	node->arg1 = copier.copy(tdbb, arg1);
	node->arg2 = copier.copy(tdbb, arg2);
	return node;
}

bool ArithmeticNode::dsqlMatch(const ExprNode* other, bool ignoreMapCast) const
{
	if (!ExprNode::dsqlMatch(other, ignoreMapCast))
		return false;

	const ArithmeticNode* o = other->as<ArithmeticNode>();
	fb_assert(o)

	return dialect1 == o->dialect1 && blrOp == o->blrOp;
}

ExprNode* ArithmeticNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* ArithmeticNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);

	request->req_flags &= ~req_null;

	// Evaluate arguments.  If either is null, result is null, but in
	// any case, evaluate both, since some expressions may later depend
	// on mappings which are developed here

	const dsc* desc1 = EVL_expr(tdbb, arg1);
	const ULONG flags = request->req_flags;
	request->req_flags &= ~req_null;

	const dsc* desc2 = EVL_expr(tdbb, arg2);

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
				return add(desc2, impure, node, blrOp);

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
				return add2(desc2, impure, node, blrOp);

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
dsc* ArithmeticNode::add(const dsc* desc, impure_value* value, const jrd_nod* node, UCHAR blrOp)
{
	DEV_BLKCHK(node, type_nod);

	const ArithmeticNode* arithmeticNode = ExprNode::as<ArithmeticNode>(node);

	fb_assert(
		(arithmeticNode && arithmeticNode->dialect1 &&
			(arithmeticNode->blrOp == blr_add || arithmeticNode->blrOp == blr_subtract)) ||
		ExprNode::is<AggNode>(node) || node->nod_type == nod_total || node->nod_type == nod_average);

	dsc* const result = &value->vlu_desc;

	// Handle date arithmetic

	if (node->nod_flags & nod_date)
	{
		fb_assert(arithmeticNode);
		return arithmeticNode->addDateTime(desc, value);
	}

	// Handle floating arithmetic

	if (node->nod_flags & nod_double)
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

	if (node->nod_flags & nod_quad)
	{
		const SQUAD q1 = MOV_get_quad(desc, node->nod_scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, node->nod_scale);

		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad = (blrOp == blr_subtract) ?
			QUAD_SUBTRACT(q2, q1, ERR_post) : QUAD_ADD(q1, q2, ERR_post);
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return result;
	}

	// Everything else defaults to longword

	// CVC: Maybe we should upgrade the sum to double if it doesn't fit?
	// This is what was done for multiplicaton in dialect 1.

	const SLONG l1 = MOV_get_long(desc, node->nod_scale);
	const SINT64 l2 = MOV_get_long(&value->vlu_desc, node->nod_scale);
	SINT64 rc = (blrOp == blr_subtract) ? l2 - l1 : l2 + l1;

	if (rc < MIN_SLONG || rc > MAX_SLONG)
		ERR_post(Arg::Gds(isc_exception_integer_overflow));

	value->make_long(rc, node->nod_scale);

	return result;
}

// Add (or subtract) the contents of a descriptor to value block, with dialect-3 semantics, as in
// the blr_add, blr_subtract, and blr_agg_total verbs following a blr_version5.
dsc* ArithmeticNode::add2(const dsc* desc, impure_value* value, const jrd_nod* node, UCHAR blrOp)
{
	DEV_BLKCHK(node, type_nod);

	const ArithmeticNode* arithmeticNode = ExprNode::as<ArithmeticNode>(node);

	fb_assert(
		(arithmeticNode && !arithmeticNode->dialect1 &&
			(arithmeticNode->blrOp == blr_add || arithmeticNode->blrOp == blr_subtract)) ||
		ExprNode::is<AggNode>(node) || node->nod_type == nod_average2);

	dsc* result = &value->vlu_desc;

	// Handle date arithmetic

	if (node->nod_flags & nod_date)
	{
		fb_assert(arithmeticNode);
		return arithmeticNode->addDateTime(desc, value);
	}

	// Handle floating arithmetic

	if (node->nod_flags & nod_double)
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

	if (node->nod_flags & nod_quad)
	{
		const SQUAD q1 = MOV_get_quad(desc, node->nod_scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, node->nod_scale);

		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad = (blrOp == blr_subtract) ?
			QUAD_SUBTRACT(q2, q1, ERR_post) : QUAD_ADD(q1, q2, ERR_post);
		result->dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return result;
	}

	// Everything else defaults to int64

	SINT64 i1 = MOV_get_int64(desc, node->nod_scale);
	const SINT64 i2 = MOV_get_int64(&value->vlu_desc, node->nod_scale);

	result->dsc_dtype = dtype_int64;
	result->dsc_length = sizeof(SINT64);
	result->dsc_scale = node->nod_scale;
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

	if (node->nod_flags & nod_double)
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

	if (node->nod_flags & nod_quad)
	{
		const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
		const SQUAD q1 = MOV_get_quad(desc, node->nod_scale - scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, scale);
		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = node->nod_scale;
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
	const SINT64 i1 = MOV_get_long(desc, node->nod_scale - scale);
	const SINT64 i2 = MOV_get_long(&value->vlu_desc, scale);
	value->vlu_desc.dsc_dtype = dtype_long;
	value->vlu_desc.dsc_length = sizeof(SLONG);
	value->vlu_desc.dsc_scale = node->nod_scale;
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

	if (node->nod_flags & nod_double)
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

	if (node->nod_flags & nod_quad)
	{
		const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
		const SQUAD q1 = MOV_get_quad(desc, node->nod_scale - scale);
		const SQUAD q2 = MOV_get_quad(&value->vlu_desc, scale);

		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad = QUAD_MULTIPLY(q1, q2, ERR_post);
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_quad;

		return &value->vlu_desc;
	}

	// Everything else defaults to int64

	const SSHORT scale = NUMERIC_SCALE(value->vlu_desc);
	const SINT64 i1 = MOV_get_int64(desc, node->nod_scale - scale);
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
	value->vlu_desc.dsc_scale = node->nod_scale;
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

	if (node->nod_flags & nod_double)
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

	SINT64 i1 = MOV_get_int64(&value->vlu_desc, node->nod_scale - desc->dsc_scale);

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
	value->vlu_desc.dsc_scale = node->nod_scale;
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

	fb_assert(node->nod_flags & nod_date);

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
		because nod_date is set in the nod_flags we know we're supposed
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
		However, the nod_date flag might have been set on any type of
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

		if (!TimeStamp::isValidTimeStamp(value->vlu_misc.vlu_timestamp))
			ERR_post(Arg::Gds(isc_datetime_range_exceeded));

		// Make sure the TIME portion is non-negative

		if ((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time < 0)
		{
			value->vlu_misc.vlu_timestamp.timestamp_time =
				((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time) + ISC_TICKS_PER_DAY;
			value->vlu_misc.vlu_timestamp.timestamp_date -= 1;
		}
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

void ConcatenateNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_concatenate);
	GEN_expr(dsqlScratch, dsqlArg1);
	GEN_expr(dsqlScratch, dsqlArg2);
}

void ConcatenateNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
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

ValueExprNode* ConcatenateNode::copy(thread_db* tdbb, NodeCopier& copier)
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

ValueExprNode* ConcatenateNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) ConcatenateNode(getPool(),
		PASS1_node(dsqlScratch, dsqlArg1), PASS1_node(dsqlScratch, dsqlArg2));
}


//--------------------


static RegisterNode<CurrentDateNode> regCurrentDateNode(blr_current_date);

DmlNode* CurrentDateNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
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

void CurrentDateNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->dsc_dtype = dtype_sql_date;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentDateNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_date;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentDateNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentDateNode(*tdbb->getDefaultPool());
}

ExprNode* CurrentDateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* CurrentDateNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

DmlNode* CurrentTimeNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp)
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

void CurrentTimeNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->dsc_dtype = dtype_sql_time;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentTimeNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->dsc_dtype = dtype_sql_time;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentTimeNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentTimeNode(*tdbb->getDefaultPool(), precision);
}

ExprNode* CurrentTimeNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

ValueExprNode* CurrentTimeNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	if (precision > MAX_TIME_PRECISION)
		ERRD_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));

	return this;
}

dsc* CurrentTimeNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

DmlNode* CurrentTimeStampNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
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

void CurrentTimeStampNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
{
	desc->dsc_dtype = dtype_timestamp;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

void CurrentTimeStampNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->dsc_dtype = dtype_timestamp;
	desc->dsc_sub_type = 0;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_length = type_lengths[desc->dsc_dtype];
}

ValueExprNode* CurrentTimeStampNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentTimeStampNode(*tdbb->getDefaultPool(), precision);
}

ExprNode* CurrentTimeStampNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

ValueExprNode* CurrentTimeStampNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	if (precision > MAX_TIME_PRECISION)
		ERRD_post(Arg::Gds(isc_invalid_time_precision) << Arg::Num(MAX_TIME_PRECISION));

	return this;
}

dsc* CurrentTimeStampNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

DmlNode* CurrentRoleNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
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

void CurrentRoleNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
{
	desc->dsc_dtype = dtype_varying;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length =
		(USERNAME_LENGTH / METADATA_BYTES_PER_CHAR) *
		METD_get_charset_bpc(dsqlScratch->getTransaction(), ttype_metadata) + sizeof(USHORT);
}

void CurrentRoleNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->dsc_dtype = dtype_text;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length = USERNAME_LENGTH * METADATA_BYTES_PER_CHAR;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
}

ValueExprNode* CurrentRoleNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentRoleNode(*tdbb->getDefaultPool());
}

ExprNode* CurrentRoleNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// CVC: Current role will get a validated role; IE one that exists.
dsc* CurrentRoleNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

ValueExprNode* CurrentRoleNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) CurrentRoleNode(getPool());
}


//--------------------


static RegisterNode<CurrentUserNode> regCurrentUserNode(blr_user_name);

DmlNode* CurrentUserNode::parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR /*blrOp*/)
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

void CurrentUserNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
{
	desc->dsc_dtype = dtype_varying;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length =
		(USERNAME_LENGTH / METADATA_BYTES_PER_CHAR) *
		METD_get_charset_bpc(dsqlScratch->getTransaction(), ttype_metadata) + sizeof(USHORT);
}

void CurrentUserNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	desc->dsc_dtype = dtype_text;
	desc->dsc_ttype() = ttype_metadata;
	desc->dsc_length = USERNAME_LENGTH * METADATA_BYTES_PER_CHAR;
	desc->dsc_scale = 0;
	desc->dsc_flags = 0;
}

ValueExprNode* CurrentUserNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	return FB_NEW(*tdbb->getDefaultPool()) CurrentUserNode(*tdbb->getDefaultPool());
}

ExprNode* CurrentUserNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* CurrentUserNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

ValueExprNode* CurrentUserNode::dsqlPass(DsqlCompilerScratch* dsqlScratch)
{
	return FB_NEW(getPool()) CurrentUserNode(getPool());
}


//--------------------


static RegisterNode<InternalInfoNode> regInternalInfoNode(blr_internal_info);

const InternalInfoNode::InfoAttr InternalInfoNode::INFO_TYPE_ATTRIBUTES[MAX_INFO_TYPE] = {
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

	node->arg = PAR_parse_node(tdbb, csb, VALUE);

	if (node->arg->nod_type != nod_literal ||
		static_cast<Literal*>(static_cast<jrd_node_base*>(
			node->arg.getObject()))->lit_desc.dsc_dtype != dtype_long)
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
	SLONG infoType = dsqlArg->getSlong();
	parameter->par_name = parameter->par_alias = INFO_TYPE_ATTRIBUTES[infoType].alias;
}

void InternalInfoNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_internal_info);
	GEN_expr(dsqlScratch, dsqlArg);
}

void InternalInfoNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
{
	InfoType infoType = static_cast<InfoType>(dsqlArg->getSlong());

	if (infoType == INFO_TYPE_SQLSTATE)
		desc->makeText(FB_SQLSTATE_LENGTH, ttype_ascii);
	else
		desc->makeLong(0);
}

void InternalInfoNode::getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc)
{
	fb_assert(arg->nod_type == nod_literal);

	dsc argDesc;
	CMP_get_desc(tdbb, csb, arg, &argDesc);
	fb_assert(argDesc.dsc_dtype == dtype_long);

	InfoType infoType = static_cast<InfoType>(*reinterpret_cast<SLONG*>(argDesc.dsc_address));

	if (infoType == INFO_TYPE_SQLSTATE)
		desc->makeText(FB_SQLSTATE_LENGTH, ttype_ascii);
	else
		desc->makeLong(0);
}

ValueExprNode* InternalInfoNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	InternalInfoNode* node = FB_NEW(*tdbb->getDefaultPool()) InternalInfoNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	return node;
}

ExprNode* InternalInfoNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

// Return a given element of the internal engine data.
dsc* InternalInfoNode::execute(thread_db* tdbb, jrd_req* request) const
{
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
	request->req_flags &= ~req_null;

	const dsc* value = EVL_expr(tdbb, arg);
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
	SLONG infoType = dsqlArg->getSlong();
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
	node->arg = PAR_parse_node(tdbb, csb, VALUE);
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
		case Dsql::nod_constant:
		case Dsql::nod_null:
			parameter->par_name = parameter->par_alias = "CONSTANT";
			break;

		case Dsql::nod_class_exprnode:
		{
			if (!level)
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
	dsql_nod* node, bool forceVarChar) const
{
	return PASS1_set_parameter_type(dsqlScratch, dsqlArg, node, forceVarChar);
}

void NegateNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	if (dsqlArg->nod_type == Dsql::nod_constant && DTYPE_IS_NUMERIC(dsqlArg->nod_desc.dsc_dtype))
		GEN_constant(dsqlScratch, dsqlArg, true);
	else
	{
		dsqlScratch->appendUChar(blr_negate);
		GEN_expr(dsqlScratch, dsqlArg);
	}
}

void NegateNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
{
	MAKE_desc(dsqlScratch, desc, dsqlArg, nullReplacement);

	if (dsqlArg->nod_type == Dsql::nod_null)
	{
		// NULL + NULL = NULL of INT
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
	CMP_get_desc(tdbb, csb, arg, desc);
	node->nod_flags = arg->nod_flags & (nod_double | nod_quad);
}

ValueExprNode* NegateNode::copy(thread_db* tdbb, NodeCopier& copier)
{
	NegateNode* node = FB_NEW(*tdbb->getDefaultPool()) NegateNode(*tdbb->getDefaultPool());
	node->arg = copier.copy(tdbb, arg);
	return node;
}

ExprNode* NegateNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::pass2(tdbb, csb);

	dsc desc;
	getDesc(tdbb, csb, &desc);
	node->nod_impure = CMP_impure(csb, sizeof(impure_value));

	return this;
}

dsc* NegateNode::execute(thread_db* tdbb, jrd_req* request) const
{
	request->req_flags &= ~req_null;

	const dsc* desc = EVL_expr(tdbb, arg);
	if (request->req_flags & req_null)
		return NULL;

	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
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

void OverNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
{
	MAKE_desc(dsqlScratch, desc, dsqlAggExpr, nullReplacement);
	desc->setNullable(true);
}

void OverNode::getDesc(thread_db* /*tdbb*/, CompilerScratch* /*csb*/, dsc* /*desc*/)
{
	fb_assert(false);
}

ValueExprNode* OverNode::copy(thread_db* /*tdbb*/, NodeCopier& /*copier*/)
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

void SubstringSimilarNode::genBlr(DsqlCompilerScratch* dsqlScratch)
{
	dsqlScratch->appendUChar(blr_substring_similar);
	GEN_expr(dsqlScratch, dsqlExpr);
	GEN_expr(dsqlScratch, dsqlPattern);
	GEN_expr(dsqlScratch, dsqlEscape);
}

void SubstringSimilarNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* nullReplacement)
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

ValueExprNode* SubstringSimilarNode::copy(thread_db* tdbb, NodeCopier& copier)
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
	csb->csb_current_nodes.push(node.getObject());

	pattern = CMP_pass1(tdbb, csb, pattern);
	escape = CMP_pass1(tdbb, csb, escape);

	csb->csb_current_nodes.pop();

	// If there is no top-level RSE present and patterns are not constant, unmark node as invariant
	// because it may be dependent on data or variables.
	if ((node->nod_flags & nod_invariant) &&
		(pattern->nod_type != nod_literal || escape->nod_type != nod_literal))
	{
		const LegacyNodeOrRseNode* ctx_node, *end;
		for (ctx_node = csb->csb_current_nodes.begin(), end = csb->csb_current_nodes.end();
			 ctx_node != end; ++ctx_node)
		{
			if (ctx_node->rseNode)
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

	node->args = PAR_args(tdbb, csb, VALUE);

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

void SysFuncCallNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
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

ValueExprNode* SysFuncCallNode::copy(thread_db* tdbb, NodeCopier& copier)
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

void UdfCallNode::make(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* /*nullReplacement*/)
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

ValueExprNode* UdfCallNode::copy(thread_db* tdbb, NodeCopier& copier)
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
	ExprNode::pass1(tdbb, csb);

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

// One of d1,d2 is time, the other is date
static bool isDateAndTime(const dsc& d1, const dsc& d2)
{
	return ((d1.dsc_dtype == dtype_sql_time && d2.dsc_dtype == dtype_sql_date) ||
		(d2.dsc_dtype == dtype_sql_time && d1.dsc_dtype == dtype_sql_date));
}


}	// namespace Jrd
