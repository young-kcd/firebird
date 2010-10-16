/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         evl.cpp
 *      DESCRIPTION:    Expression evaluation
 *
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
 */

/*
 * Modified by: Patrick J. P. Griffin
 * Date: 11/24/2000
 * Problem:   select count(0)+1 from rdb$relations where 0=1; returns 0
 *            In the EVL_group processing, the internal assigment for
 *            the literal in the computation is being done on every
 *            statement fetch, so if there are no statements fetched
 *            then the internal field never gets set.
 * Change:    Added an assignment process for the literal
 *            before the first fetch.
 *
 * Modified by: Neil McCalden
 * Date: 05 Jan 2001
 * Problem:   Firebird bug: 127375
 *            Group by on a calculated expression would cause segv
 *            when it encountered a NULL value as the calculation
 *            was trying reference a null pointer.
 * Change:    Test the null flag before trying to expand the value.
 *
 * 2001.6.17 Claudio Valderrama: Fix the annoying behavior that causes silent
 *	overflow in dialect 1. If you define the macro FIREBIRD_AVOID_DIALECT1_OVERFLOW
 *	it will work with double should an overflow happen. Otherwise, an error will be
 *	issued to the user if the overflow happens. The multiplication is done using
 *	SINT64 quantities. I had the impression that casting this SINT64 result to double
 *	when we detect overflow was faster than achieving the failed full multiplication
 *	with double operands again. Usage will tell the truth.
 *	For now, the aforementioned macro is enabled.
 * 2001.6.18 Claudio Valderrama: substring() is working with international charsets,
 *	thanks to Dave Schnepper's directions.
 * 2002.2.15 Claudio Valderrama: divide2() should not mangle negative values.
 * 2002.04.16 Paul Beach HP10 Port - (UCHAR*) desc.dsc_address = p; modified for HP
 *	Compiler
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 * 2003.08.10 Claudio Valderrama: Fix SF bug# 784121.
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <string.h>
#include <math.h>
#include "../common/common.h"
#include "../jrd/ibase.h"
#include "../dsql/Nodes.h"
#include "../dsql/ExprNodes.h"
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../jrd/req.h"
#include "../jrd/exe.h"
#include "../jrd/sbm.h"
#include "../jrd/blb.h"
#include "gen/iberror.h"
#include "../jrd/scl.h"
#include "../jrd/lck.h"
#include "../jrd/lls.h"
#include "../jrd/intl.h"
#include "../jrd/intl_classes.h"
#include "../jrd/rse.h"
#include "../common/quad.h"
#include "../jrd/sort.h"
#include "../jrd/blr.h"
#include "../jrd/tra.h"
#include "../common/gdsassert.h"
#include "../common/classes/auto.h"
#include "../common/classes/timestamp.h"
#include "../common/classes/VaryStr.h"
#include "../jrd/blb_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/dpm_proto.h"
#include "../common/dsc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/scl_proto.h"
#include "../yvalve/gds_proto.h"
#include "../jrd/align.h"
#include "../jrd/met_proto.h"
#include "../common/config/config.h"
#include "../jrd/SysFunction.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/recsrc/Cursor.h"
#include "../common/classes/Aligner.h"
#include "../jrd/Function.h"

using namespace Jrd;
using namespace Firebird;

static dsc* cast(thread_db*, dsc*, const jrd_nod*, impure_value*);
static dsc* dbkey(thread_db*, const jrd_nod*, impure_value*);
static dsc* eval_statistical(thread_db*, const jrd_nod*, impure_value*);
static dsc* extract(thread_db*, const jrd_nod*, impure_value*);
static dsc* record_version(thread_db*, const jrd_nod*, impure_value*);
static dsc* scalar(thread_db*, const jrd_nod*, impure_value*);
static dsc* string_length(thread_db*, const jrd_nod*, impure_value*);
static dsc* substring(thread_db*, impure_value*, dsc*, const dsc*, const dsc*);


dsc* EVL_assign_to(thread_db* tdbb, const jrd_nod* node)
{
/**************************************
 *
 *      E V L _ a s s i g n _ t o
 *
 **************************************
 *
 * Functional description
 *      Evaluate the descriptor of the
 *      destination node of an assignment.
 *
 **************************************/

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	jrd_req* request = tdbb->getRequest();
	impure_value* impure = request->getImpure<impure_value>(node->nod_impure);

	// The only nodes that can be assigned to are: argument, field and variable.

	int arg_number;
	const dsc* desc;
	const Format* format;
	const jrd_nod* message;
	Record* record;
	const ParameterNode* paramNode;

	if ((paramNode = ExprNode::as<ParameterNode>(node)))
	{
		message = paramNode->message;
		format = (Format*) message->nod_arg[e_msg_format];
		arg_number = paramNode->argNumber;
		desc = &format->fmt_desc[arg_number];

		impure->vlu_desc.dsc_address = request->getImpure<UCHAR>(
			message->nod_impure + (IPTR) desc->dsc_address);
		impure->vlu_desc.dsc_dtype = desc->dsc_dtype;
		impure->vlu_desc.dsc_length = desc->dsc_length;
		impure->vlu_desc.dsc_scale = desc->dsc_scale;
		impure->vlu_desc.dsc_sub_type = desc->dsc_sub_type;

		if (DTYPE_IS_TEXT(desc->dsc_dtype) &&
			((INTL_TTYPE(desc) == ttype_dynamic) || (INTL_GET_CHARSET(desc) == CS_dynamic)))
		{
			// Value is a text value, we're assigning it back to the user
			// process, user process has not specified a subtype, user
			// process specified dynamic translation and the dsc isn't from
			// a 3.3 type request (blr_cstring2 instead of blr_cstring) so
			// convert the charset to the declared charset of the process.

			impure->vlu_desc.setTextType(tdbb->getCharSet());
		}

		return &impure->vlu_desc;
	}

	switch (node->nod_type)
	{
	case nod_field:
		record = request->req_rpb[(int) (IPTR) node->nod_arg[e_fld_stream]].rpb_record;
		if (!EVL_field(0, record, (USHORT)(IPTR) node->nod_arg[e_fld_id], &impure->vlu_desc))
		{
			// The below condition means that EVL_field() returned
			// a read-only dummy value which cannot be assigned to.
			// The usual reason is a field being unexpectedly dropped.
			if (impure->vlu_desc.dsc_address &&
				!(impure->vlu_desc.dsc_flags & DSC_null))
			{
				ERR_post(Arg::Gds(isc_field_disappeared));
			}
		}
		if (!impure->vlu_desc.dsc_address)
			ERR_post(Arg::Gds(isc_read_only_field));
		return &impure->vlu_desc;

	case nod_null:
		return NULL;

	case nod_variable:
		// Calculate descriptor
		node = node->nod_arg[e_var_variable];
		impure = request->getImpure<impure_value>(node->nod_impure);
		return &impure->vlu_desc;

	default:
		BUGCHECK(229);			// msg 229 EVL_assign_to: invalid operation
	}

	return NULL;
}


RecordBitmap** EVL_bitmap(thread_db* tdbb, const InversionNode* node, RecordBitmap* bitmap_and)
{
/**************************************
 *
 *      E V L _ b i t m a p
 *
 **************************************
 *
 * Functional description
 *      Evaluate bitmap valued expression.
 *
 **************************************/

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	if (--tdbb->tdbb_quantum < 0)
		JRD_reschedule(tdbb, 0, true);

	switch (node->type)
	{
	case InversionNode::TYPE_AND:
		{
			RecordBitmap** bitmap = EVL_bitmap(tdbb, node->node1, bitmap_and);
			if (!(*bitmap) || !(*bitmap)->getFirst())
				return bitmap;

			return EVL_bitmap(tdbb, node->node2, *bitmap);
		}

	case InversionNode::TYPE_OR:
		return RecordBitmap::bit_or(
			EVL_bitmap(tdbb, node->node1, bitmap_and),
			EVL_bitmap(tdbb, node->node2, bitmap_and));

	case InversionNode::TYPE_IN:
		{
			RecordBitmap** inv_bitmap = EVL_bitmap(tdbb, node->node1, bitmap_and);
			BTR_evaluate(tdbb, node->node2->retrieval, inv_bitmap, bitmap_and);
			return inv_bitmap;
		}

	case InversionNode::TYPE_DBKEY:
		{
			impure_inversion* impure = tdbb->getRequest()->getImpure<impure_inversion>(node->impure);
			RecordBitmap::reset(impure->inv_bitmap);
			const dsc* desc = EVL_expr(tdbb, node->value);

			if (!(tdbb->getRequest()->req_flags & req_null) &&
				desc->dsc_length == sizeof(RecordNumber::Packed))
			{
				const USHORT id = node->id;
				Aligner<RecordNumber::Packed> alignedNumbers(desc->dsc_address, desc->dsc_length);
				const RecordNumber::Packed* numbers = alignedNumbers;
				RecordNumber rel_dbkey;
				rel_dbkey.bid_decode(&numbers[id]);
				// Decrement the value in order to switch back to the zero based numbering
				// (from the user point of view the DB_KEY numbering starts from one)
				rel_dbkey.decrement();
				if (!bitmap_and || bitmap_and->test(rel_dbkey.getValue()))
					RBM_SET(tdbb->getDefaultPool(), &impure->inv_bitmap, rel_dbkey.getValue());
			}

			return &impure->inv_bitmap;
		}

	case InversionNode::TYPE_INDEX:
		{
			impure_inversion* impure = tdbb->getRequest()->getImpure<impure_inversion>(node->impure);
			RecordBitmap::reset(impure->inv_bitmap);
			BTR_evaluate(tdbb, node->retrieval, &impure->inv_bitmap, bitmap_and);
			return &impure->inv_bitmap;
		}

	default:
		BUGCHECK(230);			// msg 230 EVL_bitmap: invalid operation
	}

	return NULL;
}


bool EVL_boolean(thread_db* tdbb, const jrd_nod* node)
{
/**************************************
 *
 *      E V L _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *      Evaluate a boolean.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	jrd_req* request = tdbb->getRequest();

	switch (node->nod_type)
	{
		case nod_class_exprnode_jrd:
			return reinterpret_cast<BoolExprNode*>(node->nod_arg[0])->execute(tdbb, request);

		case nod_stmt_expr:
			EXE_looper(tdbb, request, node);
			return EVL_boolean(tdbb, node->nod_arg[e_stmt_expr_expr]);

		default:
			BUGCHECK(231);			// msg 231 EVL_boolean: invalid operation
	}

	return false;
}


dsc* EVL_expr(thread_db* tdbb, const jrd_nod* node)
{
/**************************************
 *
 *      E V L _ e x p r
 *
 **************************************
 *
 * Functional description
 *      Evaluate a value expression.
 *
 **************************************/
	DEV_BLKCHK(node, type_nod);

	if (!node)
		BUGCHECK(303);			// msg 303 Invalid expression for evaluation

	SET_TDBB(tdbb);

	if (--tdbb->tdbb_quantum < 0)
		JRD_reschedule(tdbb, 0, true);

	jrd_req* const request = tdbb->getRequest();
	impure_value* const impure = request->getImpure<impure_value>(node->nod_impure);
	request->req_flags &= ~req_null;

	// Do a preliminary screen for either simple nodes or nodes that are special cased elsewhere

	switch (node->nod_type)
	{
	case nod_class_exprnode_jrd:
		{
			const ValueExprNode* exprNode = reinterpret_cast<const ValueExprNode*>(node->nod_arg[0]);
			dsc* desc = exprNode->execute(tdbb, request);

			if (desc)
				request->req_flags &= ~req_null;
			else
				request->req_flags |= req_null;

			return desc;
		}

	case nod_dbkey:
		return dbkey(tdbb, node, impure);

	case nod_rec_version:
		return record_version(tdbb, node, impure);

	case nod_field:
		{
			const USHORT id = (USHORT)(IPTR) node->nod_arg[e_fld_id];
			record_param& rpb = request->req_rpb[(USHORT)(IPTR) node->nod_arg[e_fld_stream]];
			Record* record = rpb.rpb_record;
			jrd_rel* relation = rpb.rpb_relation;
			// In order to "map a null to a default" value (in EVL_field()),
			// the relation block is referenced.
			// Reference: Bug 10116, 10424

			if (!EVL_field(relation, record, id, &impure->vlu_desc))
				request->req_flags |= req_null;
			else
			{
				const Format* compileFormat = (Format*) node->nod_arg[e_fld_format];

				// ASF: CORE-1432 - If the the record is not on the latest format, upgrade it.
				// AP: for fields that are missing in original format use record's one.
				if (compileFormat &&
					record->rec_format->fmt_version != compileFormat->fmt_version &&
					id < compileFormat->fmt_desc.getCount() &&
					!DSC_EQUIV(&impure->vlu_desc, &compileFormat->fmt_desc[id], true))
				{
					dsc desc = impure->vlu_desc;
					impure->vlu_desc = compileFormat->fmt_desc[id];

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
			}

			if (!relation || !(relation->rel_flags & REL_system))
			{
				if (impure->vlu_desc.dsc_dtype == dtype_text)
					INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);
			}

			return &impure->vlu_desc;
		}

	case nod_derived_expr:
		{
			const UCHAR streamCount = (UCHAR)(IPTR) node->nod_arg[e_derived_expr_stream_count];
			const USHORT* streamList = (USHORT*) node->nod_arg[e_derived_expr_stream_list];

			for (UCHAR i = 0; i < streamCount; ++i)
			{
				if (request->req_rpb[streamList[i]].rpb_number.isValid())
					return EVL_expr(tdbb, node->nod_arg[e_derived_expr_expr]);
			}

			request->req_flags |= req_null;
			return NULL;
		}

	case nod_literal:
		return &((Literal*) node)->lit_desc;

	case nod_null:
		request->req_flags |= req_null;
		return NULL;

	case nod_extract:
		return extract(tdbb, node, impure);

	case nod_strlen:
		return string_length(tdbb, node, impure);

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_average:
	case nod_average2:
	case nod_total:
	case nod_from:
		return eval_statistical(tdbb, node, impure);

	case nod_scalar:
		return scalar(tdbb, node, impure);

	case nod_variable:
		{
			const jrd_nod* node2 = node->nod_arg[e_var_variable];

			impure_value* impure2 = request->getImpure<impure_value>(node2->nod_impure);
			if (impure2->vlu_desc.dsc_flags & DSC_null)
				request->req_flags |= req_null;

			impure->vlu_desc = impure2->vlu_desc;

			if (impure->vlu_desc.dsc_dtype == dtype_text)
				INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);

			if (!(impure2->vlu_flags & VLU_checked))
			{
				if (node->nod_arg[e_var_info])
				{
					EVL_validate(tdbb, Item(Item::TYPE_VARIABLE, (IPTR) node->nod_arg[e_var_id]),
						reinterpret_cast<const ItemInfo*>(node->nod_arg[e_var_info]),
						&impure->vlu_desc, impure->vlu_desc.dsc_flags & DSC_null);
				}
				impure2->vlu_flags |= VLU_checked;
			}

			return &impure->vlu_desc;
		}

	case nod_domain_validation:
		if (request->req_domain_validation == NULL ||
			(request->req_domain_validation->dsc_flags & DSC_null))
		{
			request->req_flags |= req_null;
		}
		return request->req_domain_validation;

	default:   // Shut up some compiler warnings
		break;

	case nod_stmt_expr:
		EXE_looper(tdbb, request, node);
		return EVL_expr(tdbb, node->nod_arg[e_stmt_expr_expr]);
	}

	// Evaluate arguments

	dsc* values[3];

	if (node->nod_count)
	{
		fb_assert(node->nod_count <= 3);
		dsc** v = values;
		const jrd_nod* const* ptr = node->nod_arg;
		for (const jrd_nod* const* const end = ptr + node->nod_count; ptr < end;)
		{
			*v++ = EVL_expr(tdbb, *ptr++);

			if (request->req_flags & req_null)
			{
				// ASF: CAST target type may be constrained
				if (node->nod_type == nod_cast)
					*(v - 1) = NULL;
				else
					return NULL;
			}
		}
	}

	switch (node->nod_type)
	{
	case nod_substr:
		return substring(tdbb, impure, values[0], values[1], values[2]);

	case nod_cast:
		return cast(tdbb, values[0], node, impure);

	default:
		BUGCHECK(232);		// msg 232 EVL_expr: invalid operation
	}

	return NULL;
}


bool EVL_field(jrd_rel* relation, Record* record, USHORT id, dsc* desc)
{
/**************************************
 *
 *      E V L _ f i e l d
 *
 **************************************
 *
 * Functional description
 *      Evaluate a field by filling out a descriptor.
 *
 **************************************/

	DEV_BLKCHK(record, type_rec);

	if (!record)
	{
		// ASF: Usage of ERR_warning with Arg::Gds (instead of Arg::Warning) is correct here.
		// Maybe not all code paths are prepared for throwing an exception here,
		// but it will leave the engine as an error (when testing for req_warning).
		ERR_warning(Arg::Gds(isc_no_cur_rec));
		return false;
	}

	const Format* format = record->rec_format;

	if (format && id < format->fmt_count) {
		*desc = format->fmt_desc[id];
	}

/*
	dimitr: fixed bug SF #562417

	AFAIU, there was an assumption here that if a field descriptor is not
	filled, then a field doesn't exist. This is not true, because in fact
	an empty string has dsc_length = 0, and being part of an aggregate it
	becomes nod_field with zero length, hence we had NULL as a result.

	if (!format || id >= format->fmt_count || !desc->dsc_length)
*/
	if (!format || id >= format->fmt_count || !desc->dsc_dtype)
	{
		/* Map a non-existent field to a default value, if available.
		 * This enables automatic format upgrade for data rows.
		 * Handle Outer Joins and such specially!
		 * Reference: Bug 10424, 10116
		 */

		// rec_format == NULL indicates we're performing a
		// join-to-null operation for outer joins

		if (record && record->rec_format && relation)
		{
			thread_db* tdbb = JRD_get_thread_data();

			while (format &&
				(id >= format->fmt_defaults.getCount() ||
				 format->fmt_defaults[id].vlu_desc.isUnknown()))
			{
				if (format->fmt_version >= relation->rel_current_format->fmt_version)
				{
					format = NULL;
					break;
				}

				format = MET_format(tdbb, relation, format->fmt_version + 1);
			}

			return format && !(*desc = format->fmt_defaults[id].vlu_desc).isUnknown();
		}

		desc->dsc_dtype = dtype_text;
		desc->dsc_length = 1;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_ttype() = ttype_ascii;
		desc->dsc_address = (UCHAR*) " ";
		return false;
	}

	// If the offset of the field is 0, the field can't possible exist

	if (!desc->dsc_address) {
		return false;
	}

	desc->dsc_address = record->rec_data + (IPTR) desc->dsc_address;

	if (TEST_NULL(record, id))
	{
		desc->dsc_flags |= DSC_null;
		return false;
	}

	desc->dsc_flags &= ~DSC_null;
	return true;
}


void EVL_make_value(thread_db* tdbb, const dsc* desc, impure_value* value)
{
/**************************************
 *
 *      E V L _ m a k e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *      Make a value block reflect the value of a descriptor.
 *
 **************************************/
	SET_TDBB(tdbb);

	// Handle the fixed length data types first.  They're easy.

	const dsc from = *desc;
	value->vlu_desc = *desc;
	value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc;

	switch (from.dsc_dtype)
	{
	case dtype_short:
		value->vlu_misc.vlu_short = *((SSHORT *) from.dsc_address);
		return;

	case dtype_long:
	case dtype_real:
	case dtype_sql_time:
	case dtype_sql_date:
		value->vlu_misc.vlu_long = *((SLONG *) from.dsc_address);
		return;

	case dtype_int64:
		value->vlu_misc.vlu_int64 = *((SINT64 *) from.dsc_address);
		return;

	case dtype_double:
		value->vlu_misc.vlu_double = *((double *) from.dsc_address);
		return;

	case dtype_timestamp:
	case dtype_quad:
		value->vlu_misc.vlu_dbkey[0] = ((SLONG *) from.dsc_address)[0];
		value->vlu_misc.vlu_dbkey[1] = ((SLONG *) from.dsc_address)[1];
		return;

	case dtype_text:
	case dtype_varying:
	case dtype_cstring:
	case dtype_dbkey:
		break;

	case dtype_blob:
		value->vlu_misc.vlu_bid = *(bid*)from.dsc_address;
		return;

	default:
		fb_assert(false);
		break;
	}

	VaryStr<128> temp;
	UCHAR* address;
	USHORT ttype;

	// Get string.  If necessary, get_string will convert the string into a
	// temporary buffer.  Since this will always be the result of a conversion,
	// this isn't a serious problem.

	const USHORT length = MOV_get_string_ptr(&from, &ttype, &address, &temp, sizeof(temp));

	// Allocate a string block of sufficient size.
	VaryingString* string = value->vlu_string;
	if (string && string->str_length < length)
	{
		delete string;
		string = NULL;
	}
	if (!string)
	{
		string = value->vlu_string = FB_NEW_RPT(*tdbb->getDefaultPool(), length) VaryingString();
		string->str_length = length;
	}

	value->vlu_desc.dsc_length = length;
	UCHAR* target = string->str_data;
	value->vlu_desc.dsc_address = target;
	value->vlu_desc.dsc_sub_type = 0;
	value->vlu_desc.dsc_scale = 0;
	if (from.dsc_dtype == dtype_dbkey)
	{
		value->vlu_desc.dsc_dtype = dtype_dbkey;
	}
	else
	{
		value->vlu_desc.dsc_dtype = dtype_text;
		value->vlu_desc.setTextType(ttype);
	}

	if (address && length && target != address)
		memcpy(target, address, length);
}


void EVL_validate(thread_db* tdbb, const Item& item, const ItemInfo* itemInfo, dsc* desc, bool null)
{
/**************************************
 *
 *	E V L _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Validate argument/variable for not null and check constraint
 *
 **************************************/
	if (itemInfo == NULL)
		return;

	jrd_req* request = tdbb->getRequest();
	bool err = false;

	if (null && !itemInfo->nullable)
		err = true;

	const char* value = NULL_STRING_MARK;
	VaryStr<128> temp;

	MapFieldInfo::ValueType fieldInfo;
	if (!err && itemInfo->fullDomain &&
		request->getStatement()->mapFieldInfo.get(itemInfo->field, fieldInfo) &&
		fieldInfo.validation)
	{
		if (desc && null)
			desc->dsc_flags |= DSC_null;

		const bool desc_is_null = !desc || (desc->dsc_flags & DSC_null);

		request->req_domain_validation = desc;
		const USHORT flags = request->req_flags;

		if (!EVL_boolean(tdbb, fieldInfo.validation) && !(request->req_flags & req_null))
		{
			const USHORT length = desc_is_null ? 0 :
				MOV_make_string(desc, ttype_dynamic, &value, &temp, sizeof(temp) - 1);

			if (desc_is_null)
				value = NULL_STRING_MARK;
			else if (!length)
				value = "";
			else
				const_cast<char*>(value)[length] = 0;	// safe cast - data is on our local stack

			err = true;
		}

		request->req_flags = flags;
	}

	Firebird::string s;

	if (err)
	{
		ISC_STATUS status = isc_not_valid_for_var;
		const char* arg;

		if (item.type == Item::TYPE_CAST)
		{
			status = isc_not_valid_for;
			arg = "CAST";
		}
		else
		{
			if (itemInfo->name.isEmpty())
			{
				int index = item.index + 1;

				status = isc_not_valid_for;

				if (item.type == Item::TYPE_VARIABLE)
				{
					const jrd_prc* procedure = request->getStatement()->procedure;

					if (procedure)
					{
						if (index <= int(procedure->prc_output_fields.getCount()))
							s.printf("output parameter number %d", index);
						else
						{
							s.printf("variable number %d",
								index - int(procedure->prc_output_fields.getCount()));
						}
					}
					else
						s.printf("variable number %d", index);
				}
				else if (item.type == Item::TYPE_PARAMETER && item.subType == 0)
					s.printf("input parameter number %d", (index - 1) / 2 + 1);
				else if (item.type == Item::TYPE_PARAMETER && item.subType == 1)
					s.printf("output parameter number %d", index);

				if (s.isEmpty())
					arg = UNKNOWN_STRING_MARK;
				else
					arg = s.c_str();
			}
			else
				arg = itemInfo->name.c_str();
		}

		ERR_post(Arg::Gds(status) << Arg::Str(arg) << Arg::Str(value));
	}
}


static dsc* cast(thread_db* tdbb, dsc* value, const jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *      c a s t
 *
 **************************************
 *
 * Functional description
 *      Cast from one datatype to another.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	// value is present; make the conversion

	const Format* format = (Format*) node->nod_arg[e_cast_fmt];
	impure->vlu_desc = format->fmt_desc[0];
	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc;
	if (DTYPE_IS_TEXT(impure->vlu_desc.dsc_dtype))
	{
		USHORT length = DSC_string_length(&impure->vlu_desc);
		if (length <= 0 && value)
		{
			// cast is a subtype cast only

			length = DSC_string_length(value);
			if (impure->vlu_desc.dsc_dtype == dtype_cstring)
				length++;		// for NULL byte
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

	EVL_validate(tdbb, Item(Item::TYPE_CAST), (ItemInfo*) node->nod_arg[e_cast_iteminfo],
		value, value == NULL || (value->dsc_flags & DSC_null));

	if (value == NULL)
	{
		tdbb->getRequest()->req_flags |= req_null;
		return NULL;
	}

	if (DTYPE_IS_BLOB(value->dsc_dtype) || DTYPE_IS_BLOB(impure->vlu_desc.dsc_dtype))
		BLB_move(tdbb, value, &impure->vlu_desc, NULL);
	else
		MOV_move(tdbb, value, &impure->vlu_desc);

	if (impure->vlu_desc.dsc_dtype == dtype_text)
		INTL_adjust_text_descriptor(tdbb, &impure->vlu_desc);

	return &impure->vlu_desc;
}


static dsc* dbkey(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *      d b k e y       ( J R D )
 *
 **************************************
 *
 * Functional description
 *      Make up a dbkey for a record stream.  A dbkey is expressed
 *      as an 8 byte character string.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	// Get request, record parameter block, and relation for stream

	jrd_req* request = tdbb->getRequest();
	const record_param* rpb = &request->req_rpb[(int) (IPTR) node->nod_arg[0]];

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

	impure->vlu_desc.dsc_address = (UCHAR *) impure->vlu_misc.vlu_dbkey;
	impure->vlu_desc.dsc_dtype = dtype_dbkey;
	impure->vlu_desc.dsc_length = type_lengths[dtype_dbkey];
	impure->vlu_desc.dsc_ttype() = ttype_binary;

	return &impure->vlu_desc;
}


static dsc* eval_statistical(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *      e v a l _ s t a t i s t i c a l
 *
 **************************************
 *
 * Functional description
 *      Evaluate a statistical expression.
 *
 **************************************/
	USHORT* invariant_flags;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	// Get started by opening stream

	jrd_req* request = tdbb->getRequest();
	dsc* desc = &impure->vlu_desc;

	if (node->nod_flags & nod_invariant)
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

	if (nod_average2 == node->nod_type)
	{
		impure->vlu_misc.vlu_int64 = 0;
		impure->vlu_desc.dsc_dtype = dtype_int64;
		impure->vlu_desc.dsc_length = sizeof(SINT64);
		impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_int64;
		impure->vlu_desc.dsc_scale = 0;
	}
	else
	{
		impure->vlu_misc.vlu_long = 0;
		impure->vlu_desc.dsc_dtype = dtype_long;
		impure->vlu_desc.dsc_length = sizeof(SLONG);
		impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_long;
	}

	const RecordSource* const rsb = reinterpret_cast<const RecordSource*>(node->nod_arg[e_stat_rsb]);
	rsb->open(tdbb);

	SLONG count = 0;
	ULONG flag = req_null;
	double d;

	try
	{
		// Handle each variety separately

		switch (node->nod_type)
		{
		case nod_count:
			flag = 0;
			while (rsb->getRecord(tdbb))
			{
				++impure->vlu_misc.vlu_long;
			}
			break;

		case nod_min:
		case nod_max:
			while (rsb->getRecord(tdbb))
			{
				dsc* value = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
				if (request->req_flags & req_null) {
					continue;
				}
				int result;
				if (flag || ((result = MOV_compare(value, desc)) < 0 && node->nod_type == nod_min) ||
					(node->nod_type != nod_min && result > 0))
				{
					flag = 0;
					EVL_make_value(tdbb, value, impure);
				}
			}
			break;

		case nod_from:
			if (rsb->getRecord(tdbb))
			{
				desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
			}
			else
			{
				if (node->nod_arg[e_stat_default])
					desc = EVL_expr(tdbb, node->nod_arg[e_stat_default]);
				else
					ERR_post(Arg::Gds(isc_from_no_match));
			}
			flag = request->req_flags;
			break;

		case nod_average:			// total or average with dialect-1 semantics
		case nod_total:
			while (rsb->getRecord(tdbb))
			{
				desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
				if (request->req_flags & req_null) {
					continue;
				}
				// Note: if the field being SUMed or AVERAGEd is short or long,
				// impure will stay long, and the first add() will
				// set the correct scale; if it is approximate numeric,
				// the first add() will convert impure to double.
				ArithmeticNode::add(desc, impure, node, blr_add);

				count++;
			}
			desc = &impure->vlu_desc;
			if (node->nod_type == nod_total)
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

		case nod_average2:			// average with dialect-3 semantics
			while (rsb->getRecord(tdbb))
			{
				desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
				if (request->req_flags & req_null)
					continue;
				// Note: if the field being SUMed or AVERAGEd is exact
				// numeric, impure will stay int64, and the first add() will
				// set the correct scale; if it is approximate numeric,
				// the first add() will convert impure to double.
				ArithmeticNode::add(desc, impure, node, blr_add);
				count++;
			}
			desc = &impure->vlu_desc;
			if (!count)
				break;
			// We know the sum, but we want the average.  To get it, divide
			// the sum by the count.  Since count is exact, dividing an int64
			// sum by count should leave an int64 average, while dividing a
			// double sum by count should leave a double average.
			if (dtype_int64 == impure->vlu_desc.dsc_dtype)
				impure->vlu_misc.vlu_int64 /= count;
			else
				impure->vlu_misc.vlu_double /= count;
			flag = 0;
			break;

		default:
			BUGCHECK(233);			// msg 233 eval_statistical: invalid operation
		}
	}
	catch (const Firebird::Exception&)
	{
		// close stream
		// ignore any error during it to keep original
		try
		{
			rsb->close(tdbb);
			request->req_flags &= ~req_null;
			request->req_flags |= flag;
		}
		catch (const Firebird::Exception&)
		{} // no-op

		throw;
	}

	// Close stream and return value

	rsb->close(tdbb);
	request->req_flags &= ~req_null;
	request->req_flags |= flag;

	// If this is an invariant node, save the return value.  If the
	// descriptor does not point to the impure area for this node then
	// point this node's descriptor to the correct place; copy the whole
	// structure to be absolutely sure

	if (node->nod_flags & nod_invariant)
	{
		*invariant_flags |= VLU_computed;
		if (request->req_flags & req_null)
			*invariant_flags |= VLU_null;
		if (desc && (desc != &impure->vlu_desc))
			impure->vlu_desc = *desc;
	}

	return desc;
}


// *************
// e x t r a c t
// *************
// Handles EXTRACT(part FROM date/time/timestamp)
static dsc* extract(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	const ULONG extract_part = (IPTR) node->nod_arg[e_extract_part];
	const dsc* value = EVL_expr(tdbb, node->nod_arg[e_extract_value]);

	impure->vlu_desc.dsc_dtype = dtype_short;
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(&impure->vlu_misc.vlu_short);
	impure->vlu_desc.dsc_length = sizeof(SSHORT);

	jrd_req* request = tdbb->getRequest();
	// CVC: Borland used special signaling for nulls in outer joins.
	if (!value || (request->req_flags & req_null))
	{
		request->req_flags |= req_null;
		impure->vlu_misc.vlu_short = 0;
		return &impure->vlu_desc;
	}

	tm times = {0};
	int fractions;

	switch (value->dsc_dtype)
	{
	case dtype_sql_time:
		switch (extract_part)
		{
		case blr_extract_hour:
		case blr_extract_minute:
		case blr_extract_second:
		case blr_extract_millisecond:
			Firebird::TimeStamp::decode_time(*(GDS_TIME*) value->dsc_address,
										 &times.tm_hour, &times.tm_min, &times.tm_sec, &fractions);
			break;
		default:
			ERR_post(Arg::Gds(isc_expression_eval_err) <<
						Arg::Gds(isc_invalid_extractpart_time));
		}
		break;

	case dtype_sql_date:
		switch (extract_part)
		{
		case blr_extract_hour:
		case blr_extract_minute:
		case blr_extract_second:
		case blr_extract_millisecond:
			ERR_post(Arg::Gds(isc_expression_eval_err) <<
						Arg::Gds(isc_invalid_extractpart_date));
			break;
		default:
			Firebird::TimeStamp::decode_date(*(GDS_DATE*) value->dsc_address, &times);
		}
		break;

	case dtype_timestamp:
		Firebird::TimeStamp::decode_timestamp(*(GDS_TIMESTAMP*) value->dsc_address,
											  &times, &fractions);
		break;

	default:
		ERR_post(Arg::Gds(isc_expression_eval_err) <<
					Arg::Gds(isc_invalidarg_extract));
		break;
	}

	USHORT part;
	switch (extract_part)
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
					Firebird::TimeStamp::isLeapYear(yearNumber))) ? 53 : 52;
			}
			else
			{
				yearNumber = y;

				// Find if y m d falls in yearNumber y+1, weekNumber 1
				int i = Firebird::TimeStamp::isLeapYear(y) ? 366 : 365;

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
		}
		break;

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


static dsc* record_version(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *      r e c o r d _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *      Make up a record version for a record stream.
 *      The tid of the record will be used.  This will be returned
 *      as a 4 byte character string.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	// Get request, record parameter block for stream

	jrd_req* request = tdbb->getRequest();
	const record_param* rpb = &request->req_rpb[(int) (IPTR) node->nod_arg[0]];

	/* If the current transaction has updated the record, the record version
	 * coming in from DSQL will have the original transaction # (or current
	 * transaction if the current transaction updated the record in a different
	 * request.).  In these cases, mark the request so that the boolean
	 * to check equality of record version will be forced to evaluate to true.
	 */

	if (tdbb->getRequest()->req_transaction->tra_number == rpb->rpb_transaction_nr)
		request->req_flags |= req_same_tx_upd;
	else
	{
		// If the transaction is a commit retain, check if the record was
		// last updated in one of its own prior transactions

		if (request->req_transaction->tra_commit_sub_trans)
		{
			if (request->req_transaction->tra_commit_sub_trans->test(rpb->rpb_transaction_nr))
			{
				 request->req_flags |= req_same_tx_upd;
			}
		}
	}

	// Initialize descriptor

	impure->vlu_misc.vlu_long = rpb->rpb_transaction_nr;
	impure->vlu_desc.dsc_address = (UCHAR*) &impure->vlu_misc.vlu_long;
	impure->vlu_desc.dsc_dtype = dtype_text;
	impure->vlu_desc.dsc_length = 4;
	impure->vlu_desc.dsc_ttype() = ttype_binary;

	return &impure->vlu_desc;
}


static dsc* scalar(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *      s c a l a r
 *
 **************************************
 *
 * Functional description
 *      Evaluate a scalar item from an array.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	const dsc* desc = EVL_expr(tdbb, node->nod_arg[e_scl_field]);
	jrd_req* request = tdbb->getRequest();

	if (request->req_flags & req_null)
		return NULL;

	if (desc->dsc_dtype != dtype_array)
		IBERROR(261);			// msg 261 scalar operator used on field which is not an array

	const jrd_nod* list = node->nod_arg[e_scl_subscripts];
	if (list->nod_count > MAX_ARRAY_DIMENSIONS)
		ERR_post(Arg::Gds(isc_array_max_dimensions) << Arg::Num(MAX_ARRAY_DIMENSIONS));

	SLONG subscripts[MAX_ARRAY_DIMENSIONS];
	int iter = 0;
	const jrd_nod* const* ptr = list->nod_arg;
	for (const jrd_nod* const* const end = ptr + list->nod_count; ptr < end;)
	{
		const dsc* temp = EVL_expr(tdbb, *ptr++);
		if (temp && !(request->req_flags & req_null))
			subscripts[iter++] = MOV_get_long(temp, 0);
		else
			return NULL;
	}

	BLB_scalar(tdbb,
			   request->req_transaction,
			   reinterpret_cast<bid*>(desc->dsc_address),
			   list->nod_count,
			   subscripts, impure);
			   // It was subscripts, reinterpret_cast<impure_value*>(&impure->vlu_desc));
			   // but vlu_desc is the first member of impure_value and impure is already
			   // of type impure_value*, so this cast seems nonsense.

	return &impure->vlu_desc;
}


// *************************
// s t r i n g _ l e n g t h
// *************************
// Handles BIT_LENGTH(s), OCTET_LENGTH(s) and CHAR[ACTER]_LENGTH(s)
static dsc* string_length(thread_db* tdbb, const jrd_nod* node, impure_value* impure)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	const ULONG length_type = (IPTR) node->nod_arg[e_strlen_type];
	const dsc* value = EVL_expr(tdbb, node->nod_arg[e_strlen_value]);

	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.dsc_address = reinterpret_cast<UCHAR*>(&impure->vlu_misc.vlu_long);
	impure->vlu_desc.dsc_length = sizeof(ULONG);

	jrd_req* request = tdbb->getRequest();
	// CVC: Borland used special signaling for nulls in outer joins.
	if (!value || (request->req_flags & req_null))
	{
		request->req_flags |= req_null;
		impure->vlu_misc.vlu_long = 0;
		return &impure->vlu_desc;
	}

	ULONG length;

	if (value->isBlob())
	{
		blb* blob = BLB_open(tdbb, tdbb->getRequest()->req_transaction,
							 reinterpret_cast<bid*>(value->dsc_address));

		switch (length_type)
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
				}
				break;

			case blr_strlen_octet:
				length = blob->blb_length;
				break;

			case blr_strlen_char:
			{
				CharSet* charSet = INTL_charset_lookup(tdbb, value->dsc_blob_ttype());

				if (charSet->isMultiByte())
				{
					Firebird::HalfStaticArray<UCHAR, BUFFER_LARGE> buffer;

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

	switch (length_type)
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
			}
			break;

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


static dsc* substring(thread_db* tdbb, impure_value* impure,
					  dsc* value, const dsc* offset_value, const dsc* length_value)
{
/**************************************
 *
 *      s u b s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Perform substring function.
 *
 **************************************/
	return SysFunction::substring(tdbb, impure, value, offset_value, length_value);
}
