/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		Optimizer.cpp
 *	DESCRIPTION:	Optimizer
 *
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
 *  The Original Code was created by Arno Brinkman
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Arno Brinkman <firebird@abvisie.nl>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"

#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../jrd/btr.h"
#include "../jrd/rse.h"
#include "../jrd/ods.h"
#include "../jrd/Optimizer.h"

#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/par_proto.h"


#ifdef WIN_NT
#undef min
#define min _cpp_min
#undef max
#define max _cpp_max
#include <xutility>
#endif


namespace Jrd {

bool computable(CompilerScratch* csb, jrd_nod* node, SSHORT stream, 
					   bool idx_use, bool allowOnlyCurrentStream)
{
/**************************************
 *
 *	c o m p u t a b l e
 *
 **************************************
 *
 * Functional description
 *	See if node is presently computable.  
 *	Note that a field is not computable
 *	with respect to its own stream.
 *
 * There are two different uses of computable(). 
 * (a) idx_use == false: when an unused conjunct is to be picked for making
 *     into a boolean and in making a db_key. 
 *     In this case, a node is said to be computable, if all the streams 
 *     involved in that node are csb_active. The csb_active flag 
 *     defines all the streams available in the current scope of the
 *     query.
 * (b) idx_use == true: to determine if we can use an
 *     index on the conjunct we have already chosen.
 *     In order to use an index on a conjunct, it is required that the
 *     all the streams involved in the conjunct are currently active
 *     or have been already processed before and made into rivers.
 *     Because, here we want to differentiate between streams we have
 *     not yet worked on and those that we have worked on or are currently
 *     working on.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// Recurse thru interesting sub-nodes
	jrd_nod** ptr = node->nod_arg;

	if (node->nod_type == nod_procedure) {
		return false;
	}

	if (node->nod_type == nod_union) {
		jrd_nod* clauses = node->nod_arg[e_uni_clauses];
		ptr = clauses->nod_arg;
		for (const jrd_nod* const* const end = ptr + clauses->nod_count;
			ptr < end; ptr += 2)
		{
			if (!computable(csb, *ptr, stream, idx_use, allowOnlyCurrentStream)) {
				return false;
			}
		}
	} 
	else {
		for (const jrd_nod* const* const end = ptr + node->nod_count;
			ptr < end; ptr++) 
		{
			if (!computable(csb, *ptr, stream, idx_use, allowOnlyCurrentStream)) {
				return false;
			}
		}
	}

	RecordSelExpr* rse;
	jrd_nod* sub;
	jrd_nod* value;
	USHORT n;
	
	switch (node->nod_type) {
	case nod_field:

		n = (USHORT)(IPTR) node->nod_arg[e_fld_stream];
		if (allowOnlyCurrentStream) {
			if (n != stream) {
				return false;
			}
		}
		else {
			if (n == stream) {
				return false;
			}
		}
		return csb->csb_rpt[n].csb_flags & csb_active;

	case nod_rec_version:
	case nod_dbkey:

		n = (USHORT)(IPTR) node->nod_arg[0];
		if (allowOnlyCurrentStream) {
			if (n != stream) {
				return false;
			}
		}
		else {
			if (n == stream) {
				return false;
			}
		}
		return csb->csb_rpt[n].csb_flags & csb_active;

	case nod_min:
	case nod_max:
	case nod_average:
	case nod_total:
	case nod_count:
	case nod_from:
		if ((sub = node->nod_arg[e_stat_default]) &&
			!computable(csb, sub, stream, idx_use, allowOnlyCurrentStream))
		{
			return false;
		}
		rse = (RecordSelExpr*) node->nod_arg[e_stat_rse];
		value = node->nod_arg[e_stat_value];
		break;

	case nod_rse:
		rse = (RecordSelExpr*) node;
		value = NULL;
		break;

	case nod_aggregate:
		rse = (RecordSelExpr*) node->nod_arg[e_agg_rse];
		rse->rse_sorted = node->nod_arg[e_agg_group];
		value = NULL;
		break;

	default:
		return true;
	}

	// Node is a record selection expression.
	bool result = true;

	if ((sub = rse->rse_first) && !computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) {
		return false;
	}

    if ((sub = rse->rse_skip) && !computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) {
        return false;
	}
    
	// Set sub-streams of rse active
	const jrd_nod* const* end;

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end; ptr++) {
		if ((*ptr)->nod_type != nod_rse) {
			n = (USHORT)(IPTR) (*ptr)->nod_arg[STREAM_INDEX((*ptr))];
			csb->csb_rpt[n].csb_flags |= csb_active;
		}
	}

	// Check sub-stream
	if (((sub = rse->rse_boolean)    && !computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) ||
	    ((sub = rse->rse_sorted)     && !computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)) ||
	    ((sub = rse->rse_projection) && !computable(csb, sub, stream, idx_use, allowOnlyCurrentStream)))
	{
		result = false;
	}

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count;
		((ptr < end) && (result)); ptr++)
	{
		if ((*ptr)->nod_type != nod_rse) {
			if (!computable(csb, (*ptr), stream, idx_use, allowOnlyCurrentStream)) {
				result = false;
			}
		}
	}

	// Check value expression, if any
	if (result && value && !computable(csb, value, stream, idx_use, allowOnlyCurrentStream)) {
		result = false;
	}

	// Reset streams inactive
	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end;
		 ptr++)
	{
		if ((*ptr)->nod_type != nod_rse)
		{
			n = (USHORT)(IPTR) (*ptr)->nod_arg[STREAM_INDEX((*ptr))];
			csb->csb_rpt[n].csb_flags &= ~csb_active;
		}
	}

	return result;
}

#ifdef EXPRESSION_INDICES

// Try to merge this function with node_equality() into 1 function.

bool expression_equal(thread_db* tdbb, OptimizerBlk* opt,
							 const index_desc* idx, jrd_nod* node,
							 USHORT stream)
{
/**************************************
 *
 *      e x p r e s s i o n _ e q u a l
 *
 **************************************
 *
 * Functional description
 *  Wrapper for expression_equal2().
 *
 **************************************/
	DEV_BLKCHK(node, type_nod);

	SET_TDBB(tdbb);

	if (idx && idx->idx_expression_request && idx->idx_expression)
	{
		fb_assert(idx->idx_flags & idx_expressn);
		fb_assert(idx->idx_expression_request->req_caller == NULL);
		idx->idx_expression_request->req_caller = tdbb->tdbb_request;
		tdbb->tdbb_request = idx->idx_expression_request;
		bool result = false;
		{
			Jrd::ContextPoolHolder context(tdbb, tdbb->tdbb_request->req_pool);

			result = expression_equal2(tdbb, opt, idx->idx_expression,
										node, stream);
		}
		tdbb->tdbb_request = idx->idx_expression_request->req_caller;
		idx->idx_expression_request->req_caller = NULL;

		return result;
	}
	else
		return false;
}


bool expression_equal2(thread_db* tdbb, OptimizerBlk* opt,
							  jrd_nod* node1, jrd_nod* node2,
							  USHORT stream)
{
/**************************************
 *
 *      e x p r e s s i o n _ e q u a l 2
 *
 **************************************
 *
 * Functional description
 *      Determine if two expression trees are the same for 
 *	the purposes of matching one half of a boolean expression 
 *	to an index.
 *
 **************************************/
	DEV_BLKCHK(node1, type_nod);
	DEV_BLKCHK(node2, type_nod);

	SET_TDBB(tdbb);

	if (!node1 || !node2)
	{
		BUGCHECK(303);	// msg 303 Invalid expression for evaluation.
	}

	if (node1->nod_type != node2->nod_type)
	{
		dsc dsc1, dsc2; 
		dsc *desc1 = &dsc1, *desc2 = &dsc2; 

		if (node1->nod_type == nod_cast && node2->nod_type == nod_field)
		{
			CMP_get_desc(tdbb, opt->opt_csb, node1, desc1);
			CMP_get_desc(tdbb, opt->opt_csb, node2, desc2);

			if (DSC_EQUIV(desc1, desc2) && 
				expression_equal2(tdbb, opt, node1->nod_arg[e_cast_source],
								  node2, stream))
			{
				return true;
			}
		}

		if (node1->nod_type == nod_field && node2->nod_type == nod_cast)
		{
			CMP_get_desc(tdbb, opt->opt_csb, node1, desc1);
			CMP_get_desc(tdbb, opt->opt_csb, node2, desc2);

			if (DSC_EQUIV(desc1, desc2) &&
				expression_equal2(tdbb, opt, node1,
								  node2->nod_arg[e_cast_source], stream))
			{
				return true;
			}
		}

		return false;
	}

	switch (node1->nod_type) {
		case nod_add:
		case nod_multiply:
		case nod_add2:
		case nod_multiply2:
		case nod_equiv:
	    case nod_eql:
		case nod_neq:
	    case nod_and:
		case nod_or:
			// A+B is equivalent to B+A, ditto A*B==B*A
			// Note: If one expression is A+B+C, but the other is B+C+A we won't
			// necessarily match them.
			if (expression_equal2(tdbb, opt, node1->nod_arg[0],
								  node2->nod_arg[1], stream) &&
				expression_equal2(tdbb, opt, node1->nod_arg[1],
								  node2->nod_arg[0], stream))
			{
				return true;
			}
			// Fall into ...
		case nod_subtract:
		case nod_divide:
		case nod_subtract2:
		case nod_divide2:
		case nod_concatenate:

		// TODO match A > B to B <= A, etc
	    case nod_gtr:
		case nod_geq:
		case nod_leq:
		case nod_lss:
			if (expression_equal2(tdbb, opt, node1->nod_arg[0],
								  node2->nod_arg[0], stream) &&
				expression_equal2(tdbb, opt, node1->nod_arg[1],
								  node2->nod_arg[1], stream))
			{
				return true;
			}
			break;

		case nod_rec_version:
		case nod_dbkey:
			if (node1->nod_arg[0] == node2->nod_arg[0])
			{
				return true;
			}
			break;

		case nod_field:
			{
				const USHORT fld_stream = (USHORT)(IPTR) node2->nod_arg[e_fld_stream];
				if ((node1->nod_arg[e_fld_id] == node2->nod_arg[e_fld_id]) &&
					(fld_stream == stream))
				{
					/*
					dsc dsc1, dsc2; 
					dsc *desc1 = &dsc1, *desc2 = &dsc2; 

					CMP_get_desc(tdbb, opt->opt_csb, node1, desc1);
					CMP_get_desc(tdbb, opt->opt_csb, node2, desc2);

					if (DSC_GET_COLLATE(desc1) == DSC_GET_COLLATE(desc2))
					*/	
					return true;
				}
			}
			break;

		case nod_function:
			if (node1->nod_arg[e_fun_function] &&
				(node1->nod_arg[e_fun_function] == node2->nod_arg[e_fun_function]) &&
				expression_equal2(tdbb, opt, node1->nod_arg[e_fun_args],
								  node2->nod_arg[e_fun_args], stream)) 
			{
				return true;
			}
			break;

		case nod_literal:
			{
				const dsc* desc1 = EVL_expr(tdbb, node1);
				const dsc* desc2 = EVL_expr(tdbb, node2);
				if (desc1 && desc2 && !MOV_compare(desc1, desc2))
				{
					return true;
				}
			}
			break;

		case nod_null:
		case nod_user_name:
		case nod_current_role:
		case nod_current_time:
		case nod_current_date:
		case nod_current_timestamp:
			return true;

		case nod_between:
		case nod_like:
		case nod_missing:
		case nod_any:
		case nod_ansi_any:
		case nod_ansi_all:
		case nod_not:
		case nod_unique:

		case nod_value_if:
		case nod_substr:
			{
				if (node1->nod_count != node2->nod_count)
				{
					return false;
				}
				for (int i = 0; i < node1->nod_count; ++i)
				{
					if (!expression_equal2(tdbb, opt, node1->nod_arg[i],
										   node2->nod_arg[i], stream))
					{
						return false;
					}
				}
				return true;
			}
			break;

		case nod_gen_id:
		case nod_gen_id2:
			if (node1->nod_arg[e_gen_id] == node2->nod_arg[e_gen_id])
			{
				return true;
			}
			break;

		case nod_negate:
		case nod_internal_info:
			if (expression_equal2(tdbb, opt, node1->nod_arg[0],
								  node2->nod_arg[0], stream))
			{
				return true;
			}
			break;

		case nod_upcase:
			if (expression_equal2(tdbb, opt, node1->nod_arg[0],
								  node2->nod_arg[0], stream))
			{
				/*
				dsc dsc1, dsc2; 
				dsc *desc1 = &dsc1, *desc2 = &dsc2; 

				CMP_get_desc(tdbb, opt->opt_csb, node1, desc1);
				CMP_get_desc(tdbb, opt->opt_csb, node2, desc2);

				if (DSC_GET_COLLATE(desc1) == DSC_GET_COLLATE(desc2))
				*/
				return true;
			}
			break;

		case nod_cast:
			{
				dsc dsc1, dsc2; 
				dsc *desc1 = &dsc1, *desc2 = &dsc2; 

				CMP_get_desc(tdbb, opt->opt_csb, node1, desc1);
				CMP_get_desc(tdbb, opt->opt_csb, node2, desc2);

				if (DSC_EQUIV(desc1, desc2) &&
					expression_equal2(tdbb, opt, node1->nod_arg[e_cast_source],
									  node2->nod_arg[e_cast_source], stream)) 
				{
					return true;
				}
			}
			break;

		case nod_extract:
			if (node1->nod_arg[e_extract_part] == node2->nod_arg[e_extract_part] &&
				expression_equal2(tdbb, opt, node1->nod_arg[e_extract_value],
								  node2->nod_arg[e_extract_value], stream)) 
			{
				return true;
			}
			break;

	    case nod_list:
			{
				jrd_nod** ptr1 = node1->nod_arg;
				jrd_nod** ptr2 = node2->nod_arg;

				if (node1->nod_count != node2->nod_count)
				{
					return false;
				}

				ULONG count = node1->nod_count;

				while (count--)
				{
					if (!expression_equal2(tdbb, opt, *ptr1++, *ptr2++, stream))
					{
						return false;
					}
				}
			
				return true;
		    }

		// AB: New nodes has to be added

		default:
			break;
	}

	return false;
}
#endif


double getRelationCardinality(thread_db* tdbb, jrd_rel* relation, const Format* format)
{
/**************************************
 *
 *	g e t R e l a t i o n C a r d i n a l i t y
 *
 **************************************
 *
 * Functional description
 *	Return the estimated cardinality for
 *  the given relation.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	if (relation->rel_file) {
		return (double) 10000;
	}
	else {
		//csb_tail->csb_cardinality =
		//	(float) DPM_data_pages(tdbb, relation) *
		//	        dbb->dbb_page_size / format->fmt_length;

		// Estimated number of total records for this relation, 
		// we assume that the records are compressed to 60%
		// Every record has also a header and a jump section (13 + 4)
		USHORT minRecordSize = sizeof(Ods::data_page::dpg_repeat) + RHD_SIZE;
		if (!(dbb->dbb_flags & DBB_no_reserve)) {
			minRecordSize += RHDF_SIZE;
		}

		SLONG dataPages = DPM_data_pages(tdbb, relation);
		if (dataPages == 1) {
			vcl* vector = relation->rel_pages;
			if (vector) {
				WIN window(-1);
				window.win_page = (*vector)[0];
				Ods::pointer_page* ppage = 
					(Ods::pointer_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_pointer);
				const SLONG* page = ppage->ppg_page;
				Ods::data_page* dpage =
					(Ods::data_page*) CCH_HANDOFF(tdbb, &window, *page, LCK_read, pag_data);
				CCH_RELEASE(tdbb, &window);
				return dpage->dpg_count;
			}
		}

		return (double) dataPages * (dbb->dbb_page_size - DPG_SIZE) / 
			(minRecordSize + (format->fmt_length * 0.5));
	}
}


jrd_nod* make_binary_node(NOD_T type, jrd_nod* arg1, jrd_nod* arg2, bool flag)
{
/**************************************
 *
 *	m a k e _ b i n a r y _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a binary node.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	DEV_BLKCHK(arg1, type_nod);
	DEV_BLKCHK(arg2, type_nod);
	jrd_nod* node = PAR_make_node(tdbb, 2);
	node->nod_type = type;
	node->nod_arg[0] = arg1;
	node->nod_arg[1] = arg2;
	if (flag) {
		node->nod_flags |= nod_comparison;
	}
	return node;
}


str* make_alias(thread_db* tdbb, CompilerScratch* csb, 
				CompilerScratch::csb_repeat* base_tail)
{
/**************************************
 *
 *	m a k e _ a l i a s
 *
 **************************************
 *
 * Functional description
 *	Make an alias string suitable for printing
 *	as part of the plan.  For views, this means
 *	multiple aliases to distinguish the base 
 *	table.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);
	SET_TDBB(tdbb);
	if (!base_tail->csb_view && !base_tail->csb_alias)
		return NULL;
		
	const CompilerScratch::csb_repeat* csb_tail;
	// calculate the length of the alias by going up through
	// the view stack to find the lengths of all aliases;
	// adjust for spaces and a null terminator
	USHORT alias_length = 0;
	for (csb_tail = base_tail;;
		 csb_tail = &csb->csb_rpt[csb_tail->csb_view_stream])
	{
		if (csb_tail->csb_alias)
			alias_length += csb_tail->csb_alias->length();
		else {
			alias_length +=
				(!(csb_tail->csb_relation) || !(csb_tail->csb_relation->rel_name))
					? 0 : strlen(csb_tail->csb_relation->rel_name);
		}
		alias_length++;
		if (!csb_tail->csb_view)
			break;
	}

	// allocate a string block to hold the concatenated alias
	str* alias = FB_NEW_RPT(*tdbb->getDefaultPool(), alias_length) str();
	alias->str_length = alias_length - 1;
	// now concatenate the individual aliases into the string block, 
	// beginning at the end and copying back to the beginning
	TEXT* p = (TEXT *) alias->str_data + alias->str_length;
	*p-- = 0;
	for (csb_tail = base_tail;;
		 csb_tail = &csb->csb_rpt[csb_tail->csb_view_stream])
	{
		const TEXT* q;
		if (csb_tail->csb_alias)
			q = (TEXT *) csb_tail->csb_alias->c_str();
		else {
			q = (!(csb_tail->csb_relation) || !(csb_tail->csb_relation->rel_name))
				? NULL : csb_tail->csb_relation->rel_name;
		}
		// go to the end of the alias and copy it backwards
		if (q) {
			for (alias_length = 0; *q; alias_length++)
				q++;
			while (alias_length--)
				*p-- = *--q;
		}

		if (!csb_tail->csb_view)
			break;
		*p-- = ' ';
	}

	return alias;
}


USHORT nav_rsb_size(RecordSource* rsb, USHORT key_length, USHORT size)
{
/**************************************
 *
 *	n a v _ r s b _ s i z e
 *
 **************************************
 *
 * Functional description
 *	Calculate the size of a navigational rsb.  
 *
 **************************************/
	DEV_BLKCHK(rsb, type_rsb);
#ifdef SCROLLABLE_CURSORS
/* allocate extra impure area to hold the current key, 
   plus an upper and lower bound key value, for a total 
   of three times the key length for the index */
	size += sizeof(struct irsb_nav) + 3 * key_length;
#else
	size += sizeof(struct irsb_nav) + 2 * key_length;
#endif
	size = FB_ALIGN(size, ALIGNMENT);
/* make room for an idx structure to describe the index
   that was used to generate this rsb */
	if (rsb->rsb_type == rsb_navigate)
		rsb->rsb_arg[RSB_NAV_idx_offset] = (RecordSource*) (IPTR) size;
	size += sizeof(index_desc);
	return size;
}


IndexScratchSegment::IndexScratchSegment(MemoryPool& p) :
	matches(p), lowerValue(NULL), upperValue(NULL), 
	scope(0), scanType(segmentScanNone)
{
/**************************************
 *
 *	I n d e x S c r a t c h S e g m e n t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
}

IndexScratchSegment::IndexScratchSegment(MemoryPool& p, IndexScratchSegment* segment) :
	matches(p)
{
/**************************************
 *
 *	I n d e x S c r a t c h S e g m e n t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	lowerValue = segment->lowerValue;
	upperValue = segment->upperValue;
	scope = segment->scope;
	scanType = segment->scanType;

	for (int i = 0; i < segment->matches.getCount(); i++) {
		matches.add(segment->matches[i]);
	}	
}

IndexScratch::IndexScratch(MemoryPool& p, index_desc* idx) :
	segments(p), selectivity(1.0), candidate(false), scopeCandidate(false),
	lowerCount(0), upperCount(0), nonFullMatchedSegments(0), idx(idx)
{
/**************************************
 *
 *	I n d e x S c r a t c h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	// Allocate needed segments
	segments.grow(idx->idx_count);

	IndexScratchSegment** segment = segments.begin();
	for (int i = 0; i < segments.getCount(); i++) {
		segment[i] = FB_NEW(p) IndexScratchSegment(p);
	}	
}

IndexScratch::IndexScratch(MemoryPool& p, IndexScratch* scratch) :
	segments(p)
{
/**************************************
 *
 *	I n d e x S c r a t c h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	selectivity = scratch->selectivity;
	candidate = scratch->candidate;
	scopeCandidate = scratch->scopeCandidate;
	lowerCount = scratch->lowerCount;
	upperCount = scratch->upperCount;
	nonFullMatchedSegments = scratch->nonFullMatchedSegments;
	idx = scratch->idx;

	// Allocate needed segments
	segments.grow(scratch->segments.getCount());

	IndexScratchSegment** scratchSegment = scratch->segments.begin();
	IndexScratchSegment** segment = segments.begin();
	for (int i = 0; i < segments.getCount(); i++) {
		segment[i] = FB_NEW(p) IndexScratchSegment(p, scratchSegment[i]);
	}	
}

IndexScratch::~IndexScratch()
{
/**************************************
 *
 *	~I n d e x S c r a t c h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IndexScratchSegment** segment = segments.begin();
	for (int i = 0; i < segments.getCount(); i++) {
		delete segment[i];
	}	
}

InversionCandidate::InversionCandidate(MemoryPool& p) :
	selectivity(1.0), indexes(0), nonFullMatchedSegments(MAX_INDEX_SEGMENTS + 1),
	matchedSegments(0), boolean(NULL), inversion(NULL), scratch(NULL), 
	used(false), unique(false), matches(p), dependentFromStreams(p)
{
/**************************************
 *
 *	I n v e r s i o  n C a n d i d a t e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
}

OptimizerRetrieval::OptimizerRetrieval(MemoryPool& p, OptimizerBlk* opt, 
									   SSHORT streamNumber, bool outer, 
									   bool inner, jrd_nod** sortNode) :
	pool(p), indexScratches(p), inversionCandidates(p), tdbb(NULL),
	createIndexScanNodes(false), alias(NULL), setConjunctionsMatched(false)
{
/**************************************
 *
 *	O p t i m i z e r R e t r i e v a l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SET_TDBB(tdbb);
	this->database = tdbb->tdbb_database;
	this->stream = streamNumber;
	this->optimizer = opt;
	this->csb = this->optimizer->opt_csb;
	this->innerFlag = inner;
	this->outerFlag = outer;
	this->sort = sortNode;
	CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[this->stream];
	relation = csb_tail->csb_relation;

	// Allocate needed indexScratches
	indexScratches.grow(csb_tail->csb_indices);

	IndexScratch** indexScratch = indexScratches.begin();
	index_desc* idx = csb_tail->csb_idx->items;
	for (int i = 0; i < csb_tail->csb_indices; ++i, ++idx) {
		indexScratch[i] = FB_NEW(p) IndexScratch(p, idx);
	}

	inversionCandidates.shrink(0);
}

OptimizerRetrieval::~OptimizerRetrieval()
{
/**************************************
 *
 *	~O p t i m i z e r R e t r i e v a l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IndexScratch** indexScratch = indexScratches.begin();
	int i = 0;
	for (; i < indexScratches.getCount() ; ++i) {
		delete indexScratch[i];
	}
	
	InversionCandidate** invCandidate = inversionCandidates.begin();
	i = 0;
	for (; i < inversionCandidates.getCount() ; ++i) {
		delete inversionCandidates[i];
	}
}

jrd_nod* OptimizerRetrieval::composeInversion(jrd_nod* node1, jrd_nod* node2, 
											  NOD_T node_type) const
{
/**************************************
 *
 *	c o m p o s e I n v e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Melt two inversions together by the
 *  type given in node_type.
 *
 **************************************/

	if (!node2) {
		return node1;
	}

	if (!node1) {
		return node2;
	}

	if (node_type == nod_bit_or) {
		if ((node1->nod_type == nod_index) && 
			(node2->nod_type == nod_index) &&
			(reinterpret_cast<IndexRetrieval*>(node1->nod_arg[e_idx_retrieval])->irb_index == 
				reinterpret_cast<IndexRetrieval*>(node2->nod_arg[e_idx_retrieval])->irb_index))
		{
			node_type = nod_bit_in;
		}
		else if ((node1->nod_type == nod_bit_in) &&
			(node2->nod_type == nod_index) &&
			(reinterpret_cast<IndexRetrieval*>(node1->nod_arg[1]->nod_arg[e_idx_retrieval])->irb_index ==
				reinterpret_cast<IndexRetrieval*>(node2->nod_arg[e_idx_retrieval])->irb_index))
		{
			node_type = nod_bit_in;
		}
	}

	return make_binary_node(node_type, node1, node2, false);
}

void OptimizerRetrieval::findDependentFromStreams(jrd_nod* node, 
	SortedStreamList* streamList) const
{
/**************************************
 *
 *	f i n d D e p e n d e n t F r o m S t r e a m s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	// Recurse thru interesting sub-nodes
	jrd_nod** ptr = node->nod_arg;

	if (node->nod_type == nod_procedure) {
		return;
	}

	if (node->nod_type == nod_union) {
		jrd_nod* clauses = node->nod_arg[e_uni_clauses];
		ptr = clauses->nod_arg;
		for (const jrd_nod* const* const end = ptr + clauses->nod_count; 
			ptr < end; ptr += 2)
		{
			findDependentFromStreams(*ptr, streamList);
		}
	} 
	else {
		for (const jrd_nod* const* const end = ptr + node->nod_count;
			ptr < end; ptr++) 
		{
			findDependentFromStreams(*ptr, streamList);
		}
	}

	RecordSelExpr* rse;
	jrd_nod* sub;
	jrd_nod* value;
	
	switch (node->nod_type) {
		case nod_field:
		{
			int fieldStream = (USHORT)(IPTR) node->nod_arg[e_fld_stream];
			if (fieldStream != stream) {
				size_t pos;
				if (!streamList->find(fieldStream, pos)) {
					streamList->add(fieldStream);
				}
			}
			return;
		}

		case nod_rec_version:
		case nod_dbkey:
		{
			int keyStream = (USHORT)(IPTR) node->nod_arg[0];
			if (keyStream != stream) {
				size_t pos;
				if (!streamList->find(keyStream, pos)) {
					streamList->add(keyStream);
				}
			}
			return;
		}

		case nod_min:
		case nod_max:
		case nod_average:
		case nod_total:
		case nod_count:
		case nod_from:
			if (sub = node->nod_arg[e_stat_default]) {
				findDependentFromStreams(sub, streamList);
			}
			rse = (RecordSelExpr*) node->nod_arg[e_stat_rse];
			value = node->nod_arg[e_stat_value];
			break;

		case nod_rse:
			rse = (RecordSelExpr*) node;
			value = NULL;
			break;

		case nod_aggregate:
			rse = (RecordSelExpr*) node->nod_arg[e_agg_rse];
			rse->rse_sorted = node->nod_arg[e_agg_group];
			value = NULL;
			break;

		default:
			return;
	}

	// Node is a record selection expression.
	if (sub = rse->rse_first) {
		findDependentFromStreams(sub, streamList);
	}

    if (sub = rse->rse_skip) {
        findDependentFromStreams(sub, streamList);
	}
    
	if (sub = rse->rse_boolean) {
		findDependentFromStreams(sub, streamList);
	}

	if (sub = rse->rse_sorted) {
		findDependentFromStreams(sub, streamList);
	}

	if (sub = rse->rse_projection) {
		findDependentFromStreams(sub, streamList);
	}

	const jrd_nod* const* end;
	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end; ptr++) {
		if ((*ptr)->nod_type != nod_rse) {
			findDependentFromStreams(*ptr, streamList);
		}
	}

	// Check value expression, if any
	if (value) {
		findDependentFromStreams(value, streamList);
	}

	return;
}

str* OptimizerRetrieval::getAlias()
{
/**************************************
 *
 *	g e t A l i a s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if (!alias) {
		CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[this->stream];
		alias = make_alias(tdbb, csb, csb_tail);
	}
	return alias;
}

InversionCandidate* OptimizerRetrieval::generateInversion(RecordSource** rsb)
{
/**************************************
 *
 *	g e n e r a t e I n v e r s i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if ((!relation) || (relation->rel_file)) {
		return NULL;
	}

	// It's recalculated later.
	const OptimizerBlk::opt_conjunct* opt_end =
		optimizer->opt_conjuncts.begin() + 
		(innerFlag ? optimizer->opt_base_missing_conjuncts : 
			optimizer->opt_conjuncts.getCount());

	InversionCandidateList inversions;
	inversions.shrink(0);

	// First, handle "AND" comparisons (all nodes except nod_or)
	OptimizerBlk::opt_conjunct* tail = optimizer->opt_conjuncts.begin();
	if (outerFlag) {
		tail += optimizer->opt_base_parent_conjuncts;
	}
	for (; tail < opt_end; tail++) {
		if (!(tail->opt_conjunct_flags & opt_conjunct_used) &&
			tail->opt_conjunct_node && (tail->opt_conjunct_node->nod_type != nod_or)) 
		{
			matchOnIndexes(&indexScratches, tail->opt_conjunct_node, 1);
		}
	}
	getInversionCandidates(&inversions, &indexScratches, 1);

	if (sort && rsb) {
		*rsb = generateNavigation();
	}

	// Second, handle "OR" comparisons
	InversionCandidate* invCandidate = NULL;
	tail = optimizer->opt_conjuncts.begin();
	if (outerFlag) {
		tail += optimizer->opt_base_parent_conjuncts;
	}
	for (; tail < opt_end; tail++) {
		if (!(tail->opt_conjunct_flags & opt_conjunct_used) &&
			tail->opt_conjunct_node && (tail->opt_conjunct_node->nod_type == nod_or)) 
		{
			invCandidate = matchOnIndexes(&indexScratches, tail->opt_conjunct_node, 1);
			if (invCandidate) {
				invCandidate->boolean = tail->opt_conjunct_node;
				inversions.add(invCandidate);
			}
		}
	}

	invCandidate = makeInversion(&inversions);

	// Add the streams where this stream is depending on.
	if (invCandidate) {
		for (int i = 0; i < invCandidate->matches.getCount(); i++) {
			findDependentFromStreams(invCandidate->matches[i], 
				&invCandidate->dependentFromStreams);
		}
	}

	if (invCandidate && setConjunctionsMatched) {
		Firebird::SortedArray<jrd_nod*> matches;
		// AB: Putting a unsorted array in a sorted array directly by join isn't
		// very safe at the moment, but in our case Array holds a sorted list.
		// However SortedArray class should be updated to handle join right!
		matches.join(invCandidate->matches);
		size_t pos;
		tail = optimizer->opt_conjuncts.begin();
		for (; tail < opt_end; tail++) {
			if (!(tail->opt_conjunct_flags & opt_conjunct_used)) {
				if (matches.find(tail->opt_conjunct_node, pos)) {
					tail->opt_conjunct_flags |= opt_conjunct_matched;
				}
			}
		}
	}

	// Clean up inversion list
	InversionCandidate** inversion = inversions.begin();
	for (int i = 0; i < inversions.getCount(); i++) {
		delete inversion[i];
	}

	return invCandidate;
}

RecordSource* OptimizerRetrieval::generateNavigation()
{
/**************************************
 *
 *	g e n e r a t e N a v i g a t i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	// not sure the significance of this magic number; if it's meant to 
	// signify that we shouldn't navigate on a system table, our catalog 
	// has grown beyond 16 tables--it doesn't seem like a problem 
	// to allow navigating through system tables, so I won't bump the 
	// number up, but I'll leave it at 16 for safety's sake--deej
	if (!sort || !(*sort) || (relation->rel_id <= 16)) {
		return NULL;
	}

	jrd_nod* sortPtr = *sort;
	IndexScratch** indexScratch = indexScratches.begin();
	int i = 0;
	for (; i < indexScratches.getCount() ; ++i) {

		index_desc* idx = indexScratch[i]->idx;
		
		// Check sort order against index.  If they don't match, give up and
		// go home.  Also don't bother if we have a non-unique index.
		// This is because null values aren't placed in a "good" spot in
		// the index in versions prior to V3.2.

		// if the number of fields in the sort is greater than the number of 
		// fields in the index, the index will not be used to optimize the    
		// sort--note that in the case where the first field is unique, this 
		// could be optimized, since the sort will be performed correctly by 
		// navigating on a unique index on the first field--deej
		if (sortPtr->nod_count > idx->idx_count) {
			continue;
		}

		// if the user-specified access plan for this request didn't
		// mention this index, forget it
		if ((idx->idx_runtime_flags & idx_plan_dont_use) &&
			!(idx->idx_runtime_flags & idx_plan_navigate)) 
		{
			continue;
		}

		// check to see if the fields in the sort match the fields in the index 
		// in the exact same order--we used to check for ascending/descending prior 
		// to SCROLLABLE_CURSORS, but now descending sorts can use ascending indices 
		// and vice versa.

		bool usableIndex = true;
		index_desc::idx_repeat* idx_tail = idx->idx_rpt;
		jrd_nod** ptr = sortPtr->nod_arg;
		for (const jrd_nod* const* const end = ptr + sortPtr->nod_count; ptr < end;
			ptr++, idx_tail++)
		{
			jrd_nod* node = *ptr;
			if (node->nod_type != nod_field
				|| (USHORT)(IPTR) node->nod_arg[e_fld_stream] != stream
				|| (USHORT)(IPTR) node->nod_arg[e_fld_id] != idx_tail->idx_field
				// for ODS11 default nulls placement always may be matched to index
				|| (database->dbb_ods_version >= ODS_VERSION11 && (
				(reinterpret_cast<IPTR>(ptr[2 * sortPtr->nod_count]) == rse_nulls_first && ptr[sortPtr->nod_count])
				|| (reinterpret_cast<IPTR>(ptr[2 * sortPtr->nod_count]) == rse_nulls_last && !ptr[sortPtr->nod_count])))
				// for ODS10 and earlier indices always placed nulls at the end of dataset
				|| (database->dbb_ods_version < ODS_VERSION11 && 
				reinterpret_cast<IPTR>(ptr[2 * sortPtr->nod_count]) == rse_nulls_first)
				|| (ptr[sortPtr->nod_count]
					&& !(idx->idx_flags & idx_descending))
				|| (!ptr[sortPtr->nod_count]
					&& (idx->idx_flags & idx_descending)) )
			{
				usableIndex = false;
				break;
			}
		}

		if (!usableIndex) {
			// We can't use this index, try next one.
			continue;
		}

		// Looks like we can do a navigational walk.  Flag that
		// we have used this index for navigation, and allocate
		// a navigational rsb for it.
		*sort = NULL;
		idx->idx_runtime_flags |= idx_navigate;
		//return gen_nav_rsb(tdbb, opt, stream, relation, alias, idx);

		USHORT key_length = ROUNDUP(BTR_key_length(relation, idx), sizeof(SLONG));
		RecordSource* rsb = FB_NEW_RPT(*tdbb->getDefaultPool(), RSB_NAV_count) RecordSource();
		rsb->rsb_type = rsb_navigate;
		rsb->rsb_relation = relation;
		rsb->rsb_stream = (UCHAR) stream;
		rsb->rsb_alias = getAlias();
		rsb->rsb_arg[RSB_NAV_index] = (RecordSource*) makeIndexScanNode(indexScratch[i]);
		rsb->rsb_arg[RSB_NAV_key_length] = (RecordSource*) (IPTR) key_length;

		// if this is a blr_stream, adjust the allocated impure area 
		// to be based on the maximum key size so that the index may be
		// reset at any time to another index of larger key length
		// without adjusting the impure area offsets
		if (optimizer->opt_g_flags & opt_g_stream) {
			key_length = MAX_KEY;
		}
		const USHORT size = nav_rsb_size(rsb, key_length, 0);
		rsb->rsb_impure = CMP_impure(optimizer->opt_csb, size);
		return rsb;
	}

	return NULL;
}

InversionCandidate* OptimizerRetrieval::getCost()
{
/**************************************
 *
 *	g e t C o s t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	createIndexScanNodes = false;
	setConjunctionsMatched = false;
	InversionCandidate* inversion = generateInversion(NULL);
	if (inversion) {
		return inversion;
	}
	else {
		// No index will be used
		InversionCandidate* invCandidate = FB_NEW(pool) InversionCandidate(pool);
		invCandidate->indexes = 0;
		invCandidate->selectivity = 1.0;
		return invCandidate;
	}
}

InversionCandidate* OptimizerRetrieval::getInversion(RecordSource** rsb)
{
/**************************************
 *
 *	g e t I n v e r s i o n
 *
 **************************************
 *
 * Functional description
 * Return an inversionCandidate which 
 * contains a created inversion when an
 * index could be used.
 * This function should always return 
 * an InversionCandidate;
 *
 **************************************/
	createIndexScanNodes = true;
	setConjunctionsMatched = true;
	InversionCandidate* inversion = generateInversion(rsb);
	if (inversion) {
		return inversion;
	}
	else {
		// No index will be used
		InversionCandidate* invCandidate = FB_NEW(pool) InversionCandidate(pool);
		invCandidate->indexes = 0;
		invCandidate->selectivity = 1.0;
		return invCandidate;
	}
}

bool OptimizerRetrieval::getInversionCandidates(InversionCandidateList* inversions, 
		IndexScratchList* indexScratches, USHORT scope) const
{
/**************************************
 *
 *	g e t I n v e r s i o n C a n d i d a t e s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	// Walk through indexes to calculate selectivity / candidate
	Firebird::Array<jrd_nod*> matches;
	IndexScratch** scratch = indexScratches->begin();
	int i = 0;
	for (i = 0; i < indexScratches->getCount(); i++) {
		scratch[i]->scopeCandidate = false;
		scratch[i]->lowerCount = 0;
		scratch[i]->upperCount = 0;
		scratch[i]->nonFullMatchedSegments = MAX_INDEX_SEGMENTS + 1;
		if (scratch[i]->candidate) {
			matches.clear();
			scratch[i]->selectivity = 1.0;
			for (int j = 0; j < scratch[i]->idx->idx_count; j++) {
				IndexScratchSegment* segment = scratch[i]->segments[j];
				if (segment->scope == scope) {
					scratch[i]->scopeCandidate = true;
				}
				// Check if this is the last usable segment
				if (((segment->scanType == segmentScanEqual) ||
					(segment->scanType == segmentScanEquivalent) ||
					(segment->scanType == segmentScanMissing))) 
				{
					// This is a perfect usable segment thus update root selectivity
					scratch[i]->lowerCount++;
					scratch[i]->upperCount++;
					scratch[i]->selectivity = scratch[i]->idx->idx_rpt[j].idx_selectivity;
					scratch[i]->nonFullMatchedSegments = scratch[i]->idx->idx_count - (j + 1);
					// Add matches for this segment to the main matches list
					matches.join(segment->matches);

					if (((j + 1) == scratch[i]->idx->idx_count) && 
						(scratch[i]->idx->idx_flags & idx_unique) &&
						(scratch[i]->scopeCandidate)) 
					{
						// We have found a full equal matching index and it's unique,
						// so we can stop looking further, because this is the best
						// one we can get.
						InversionCandidate* invCandidate = FB_NEW(pool) InversionCandidate(pool);
						invCandidate->unique = true;
						invCandidate->selectivity = scratch[i]->selectivity;
						invCandidate->nonFullMatchedSegments = 0;
						invCandidate->matchedSegments = 
							std::max(scratch[i]->lowerCount, scratch[i]->upperCount);
						invCandidate->indexes = 1;
						invCandidate->scratch = scratch[i];
						invCandidate->matches.join(matches);
						inversions->add(invCandidate);
						// There's no chance that this can be better.
						return true;
					}
				}
				else 
				{
					// This is our last segment that we can use, estimate the selectivity
					double selectivity = scratch[i]->selectivity;
					switch (segment->scanType) {
						case segmentScanBetween:
							scratch[i]->lowerCount++;
							scratch[i]->upperCount++;
							selectivity = selectivity -
								((selectivity - scratch[i]->idx->idx_rpt[j].idx_selectivity) / 2);
							break;

						case segmentScanLess:
							scratch[i]->upperCount++;
							selectivity = selectivity -
								((selectivity - scratch[i]->idx->idx_rpt[j].idx_selectivity) / 5);
							break;

						case segmentScanGreater:
							scratch[i]->lowerCount++;
							selectivity = selectivity -
								((selectivity - scratch[i]->idx->idx_rpt[j].idx_selectivity) / 5);
							break;

						case segmentScanStarting:
							scratch[i]->lowerCount++;
							scratch[i]->upperCount++;
							selectivity = selectivity -
								((selectivity - scratch[i]->idx->idx_rpt[j].idx_selectivity) / 10);
							break;

						default:
							break;
					}
					// The selectivity can never be worser than the previous segment
					scratch[i]->selectivity = selectivity;
					if (segment->scanType != segmentScanNone) {
						matches.join(segment->matches);
						scratch[i]->nonFullMatchedSegments = scratch[i]->idx->idx_count - j;
					}
					break;
				}
			}
			if (scratch[i]->scopeCandidate) {
				InversionCandidate* invCandidate = FB_NEW(pool) InversionCandidate(pool);
				invCandidate->selectivity = scratch[i]->selectivity;
				invCandidate->nonFullMatchedSegments = scratch[i]->nonFullMatchedSegments;
				invCandidate->matchedSegments = 
					std::max(scratch[i]->lowerCount, scratch[i]->upperCount);
				invCandidate->indexes = 1;
				invCandidate->scratch = scratch[i];
				invCandidate->matches.join(matches);
				inversions->add(invCandidate);
			}
		}
	}

	return (inversions->getCount() >= 1);
}

jrd_nod* OptimizerRetrieval::makeIndexNode(const index_desc* idx) const
{
/**************************************
 *
 *	m a k e I n d e x N o d e
 *
 **************************************
 *
 * Functional description
 *	Make an index node and an index retrieval block.
 *
 **************************************/

	// check whether this is during a compile or during
	// a SET INDEX operation
	if (csb) {
		CMP_post_resource(&csb->csb_resources,
						  relation, Resource::rsc_index,
						  idx->idx_id);
	}
	else {
		CMP_post_resource(&tdbb->tdbb_request->req_resources,
						  relation, Resource::rsc_index,
						  idx->idx_id);
	}

	jrd_nod* node = PAR_make_node(tdbb, e_idx_length);
	node->nod_type = nod_index;
	node->nod_count = 0;

	IndexRetrieval* retrieval = FB_NEW_RPT(pool, idx->idx_count * 2) IndexRetrieval();
	node->nod_arg[e_idx_retrieval] = (jrd_nod*) retrieval;
	retrieval->irb_index = idx->idx_id;
	MOVE_FAST(idx, &retrieval->irb_desc, sizeof(retrieval->irb_desc));
	if (csb) {
		node->nod_impure = CMP_impure(csb, sizeof(impure_inversion));
	}
	return node;
}

jrd_nod* OptimizerRetrieval::makeIndexScanNode(IndexScratch* indexScratch) const
{
/**************************************
 *
 *	m a k e I n d e x S c a n N o d e
 *
 **************************************
 *
 * Functional description
 *	Build node for index scan.
 *
 **************************************/

	if (!createIndexScanNodes) {
		return NULL;
	}

	// Allocate both a index retrieval node and block.
	index_desc* idx = indexScratch->idx;
	jrd_nod* node = makeIndexNode(idx);
	IndexRetrieval* retrieval = (IndexRetrieval*) node->nod_arg[e_idx_retrieval];
	retrieval->irb_relation = relation;

	// Pick up lower bound segment values
	jrd_nod** lower = retrieval->irb_value;
	jrd_nod** upper = retrieval->irb_value + idx->idx_count;
	retrieval->irb_lower_count = indexScratch->lowerCount;
	retrieval->irb_upper_count = indexScratch->upperCount;

	if (idx->idx_flags & idx_descending) {
		// switch upper/lower information
		upper = retrieval->irb_value;
		lower = retrieval->irb_value + idx->idx_count;
		retrieval->irb_lower_count = indexScratch->upperCount;
		retrieval->irb_upper_count = indexScratch->lowerCount;
		retrieval->irb_generic |= irb_descending;
	}

	int i = 0;
	bool ignoreNullsOnScan = true;
	IndexScratchSegment** segment = indexScratch->segments.begin();
	for (i = 0; i < MAX(indexScratch->lowerCount, indexScratch->upperCount); i++) {
		if (segment[i]->scanType == segmentScanMissing) {
			jrd_nod* value = PAR_make_node(tdbb, 0);
			value->nod_type = nod_null;
			*lower++ = *upper++ = value;
			ignoreNullsOnScan = false;
		}
		else {
			if (i < indexScratch->lowerCount) {
				*lower++ = segment[i]->lowerValue;
			}
			if (i < indexScratch->upperCount) {
				*upper++ = segment[i]->upperValue;
			}
		}
		if (segment[i]->scanType == segmentScanEquivalent) {
			ignoreNullsOnScan = false;
		}
	}

	i = std::max(indexScratch->lowerCount, indexScratch->upperCount) - 1;
	if ((i >= 0) && (segment[i]->scanType == segmentScanStarting)) {
		retrieval->irb_generic |= irb_starting;
	}

	// This index is never used for IS NULL, thus we can ignore NULLs
	// already at index scan. But this rule doesn't apply to nod_equiv
	// which requires NULLs to be found in the index.
	// A second exception is when this index is used for navigation.
	if (ignoreNullsOnScan && !(idx->idx_runtime_flags & idx_navigate)) {
		retrieval->irb_generic |= irb_ignore_null_value_key;
	}

	// Check to see if this is really an equality retrieval
	if (retrieval->irb_lower_count == retrieval->irb_upper_count) {
		retrieval->irb_generic |= irb_equality;
		segment = indexScratch->segments.begin();
		for (i = 0; i < retrieval->irb_lower_count; i++) {
			if (segment[i]->lowerValue != segment[i]->upperValue) {
				retrieval->irb_generic &= ~irb_equality;
				break;
			}
		}
	}

	// If we are matching less than the full index, this is a partial match
	if (idx->idx_flags & idx_descending) {
		if (retrieval->irb_lower_count < idx->idx_count) {
			retrieval->irb_generic |= irb_partial;
		}
	}
	else {
		if (retrieval->irb_upper_count < idx->idx_count) {
			retrieval->irb_generic |= irb_partial;
		}
	}

	// mark the index as utilized for the purposes of this compile
	idx->idx_runtime_flags |= idx_used;

	return node;
}

InversionCandidate* OptimizerRetrieval::makeInversion(InversionCandidateList* inversions)
	const
{
/**************************************
 *
 *	m a k e I n v e r s i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (inversions->getCount() < 1) {
		return NULL;
	}

	InversionCandidate* invCandidate = NULL;
	int i = 0;
	double totalSelectivity = 1; // worst selectivity
	InversionCandidate** inversion = inversions->begin();
	for (i = 0; i < inversions->getCount(); i++) {
		inversion[i]->used = false;
	}

	// The matches returned in this inversion are always sorted.
	Firebird::SortedArray<jrd_nod*> matches;

	for (i = 0; i < inversions->getCount(); i++) {

		// Initialize vars before walking through candidates
		double bestSelectivity = 1; // worst selectivity
		int bestNonFullMatchedSegments = MAX_INDEX_SEGMENTS + 1; // worst non matching segments
		int bestPosition = -1;
		int bestIndexes = 65535; // More as 65535 indexes in a inversion doesn't sound as optimized
		int bestMatchedSegments = 0;
		bool restartLoop = false;

		for (int currentPosition = 0; currentPosition < inversions->getCount(); currentPosition++) {

			if (!inversion[currentPosition]->used) {

				// If this is a unique full equal matched inversion we're done, so 
				// we can make the inversion and return it. 
				if (inversion[currentPosition]->unique) {
					if (!invCandidate) {
						invCandidate = FB_NEW(pool) InversionCandidate(pool);
					}
					if (!inversion[currentPosition]->inversion && inversion[currentPosition]->scratch) {
						invCandidate->inversion = makeIndexScanNode(inversion[currentPosition]->scratch);
					}
					else {
						invCandidate->inversion = inversion[currentPosition]->inversion;
					}
					invCandidate->selectivity = inversion[currentPosition]->selectivity;
					invCandidate->indexes = inversion[currentPosition]->indexes;
					invCandidate->nonFullMatchedSegments = 0;
					invCandidate->matchedSegments = inversion[currentPosition]->matchedSegments;
					matches.clear();
					for (int j = 0; j < inversion[currentPosition]->matches.getCount(); j++) {
						size_t pos;
						if (!matches.find(inversion[currentPosition]->matches[j], pos)) {
							matches.add(inversion[currentPosition]->matches[j]);
						}
					}
					invCandidate->matches.join(matches);
					return invCandidate;
				}
							
				// Look if a match is already used by previous matches
				bool anyMatchAlreadyUsed = false;
				for (int j = 0; j < inversion[currentPosition]->matches.getCount(); j++) {
					size_t pos;
					if (matches.find(inversion[currentPosition]->matches[j], pos)) {
						anyMatchAlreadyUsed = true;
						break;
					}
				}

				if (anyMatchAlreadyUsed) {
					inversion[currentPosition]->used = true;
					// If a match on this index was already used by another 
					// index, add also the other matches from this index.
					for (int j = 0; j < inversion[currentPosition]->matches.getCount(); j++) {
						size_t pos;
						if (!matches.find(inversion[currentPosition]->matches[j], pos)) {
							matches.add(inversion[currentPosition]->matches[j]);
						}
					}
					// Restart loop, because other indexes could als be excluded now.
					restartLoop = true;
					break;
				}				

				// Check if selectivity is almost the same
				double diffSelectivity = (bestSelectivity / 
					inversion[currentPosition]->selectivity);
				if ((diffSelectivity >= 0.98) && (diffSelectivity <= 1.02)) {
					// If the "same" selectivity then compare with the nr of unmatched segments, 
					// how many indexes and matched segments.
					int compareSelectivity = (inversion[currentPosition]->indexes - bestIndexes);
					if (compareSelectivity == 0) {
						compareSelectivity =
							(inversion[currentPosition]->nonFullMatchedSegments - bestNonFullMatchedSegments);
						if (compareSelectivity == 0) {
							compareSelectivity = 
								(inversion[currentPosition]->matchedSegments - bestMatchedSegments);
						}
					}
					if (compareSelectivity < 0) {
						bestPosition = currentPosition;
						bestSelectivity = inversion[currentPosition]->selectivity;
						bestNonFullMatchedSegments = 
							inversion[currentPosition]->nonFullMatchedSegments;
						bestMatchedSegments = inversion[currentPosition]->matchedSegments;
						bestIndexes = inversion[currentPosition]->indexes;
					}
				}
				else if (inversion[currentPosition]->selectivity < bestSelectivity) {
					bestPosition = currentPosition;
					bestSelectivity = inversion[currentPosition]->selectivity;
					bestNonFullMatchedSegments = 
						inversion[currentPosition]->nonFullMatchedSegments;
					bestMatchedSegments = inversion[currentPosition]->matchedSegments;
					bestIndexes = inversion[currentPosition]->indexes;
				}
			}
		}
		
		if (restartLoop) {
			continue;
		}

		// If we have a candidate which is interesting build the inversion
		// else we're done.
		if (bestPosition >= 0) {
			//if (bestSelectivity < (totalSelectivity * SELECTIVITY_THRESHOLD_FACTOR)) {
			if (bestSelectivity < (totalSelectivity * 5)) {

				// Estimate new selectivity
				// Assign selectivity for the formula
				double bestSel = bestSelectivity;
				double worstSel = totalSelectivity;
				if (bestSelectivity > totalSelectivity) {
					worstSel = bestSelectivity;
					bestSel = totalSelectivity;
				}
				totalSelectivity = (bestSel + (((worstSel - bestSel) / (1 - bestSel)) * bestSel)) / 2;

				// Exclude index from next pass
				inversion[bestPosition]->used = true;

				if (!invCandidate) {
					invCandidate = FB_NEW(pool) InversionCandidate(pool);
					if (!inversion[bestPosition]->inversion && inversion[bestPosition]->scratch) {
						invCandidate->inversion = makeIndexScanNode(inversion[bestPosition]->scratch);
					}
					else {
						invCandidate->inversion = inversion[bestPosition]->inversion;
					}
					invCandidate->selectivity = inversion[bestPosition]->selectivity;
					invCandidate->indexes = inversion[bestPosition]->indexes;
					invCandidate->nonFullMatchedSegments = 0;
					invCandidate->matchedSegments = inversion[bestPosition]->matchedSegments;
					for (int j = 0; j < inversion[bestPosition]->matches.getCount(); j++) {
						size_t pos;
						if (!matches.find(inversion[bestPosition]->matches[j], pos)) {
							matches.add(inversion[bestPosition]->matches[j]);
						}
					}
					if (inversion[bestPosition]->boolean) {
						size_t pos;
						if (!matches.find(inversion[bestPosition]->boolean, pos)) {
							matches.add(inversion[bestPosition]->boolean);
						}
					}
				}
				else {
					if (!inversion[bestPosition]->inversion && inversion[bestPosition]->scratch) {
						invCandidate->inversion = composeInversion(invCandidate->inversion,
							makeIndexScanNode(inversion[bestPosition]->scratch), nod_bit_and);
					}
					else {
						invCandidate->inversion = composeInversion(invCandidate->inversion,
							inversion[bestPosition]->inversion, nod_bit_and);
					}
					invCandidate->selectivity = totalSelectivity;
					invCandidate->indexes += inversion[bestPosition]->indexes;
					invCandidate->nonFullMatchedSegments = 0;
					invCandidate->matchedSegments = 
						std::max(inversion[bestPosition]->matchedSegments, invCandidate->matchedSegments);
					for (int j = 0; j < inversion[bestPosition]->matches.getCount(); j++) {
						size_t pos;
						if (!matches.find(inversion[bestPosition]->matches[j], pos)) {
	                        matches.add(inversion[bestPosition]->matches[j]);
						}
					}
					if (inversion[bestPosition]->boolean) {
						size_t pos;
						if (!matches.find(inversion[bestPosition]->boolean, pos)) {
							matches.add(inversion[bestPosition]->boolean);
						}
					}
				}
			}
			else {
				// We're done
				break;
			}
		}
		else {
			break;
		}
	}

	if (invCandidate && matches.getCount()) {
		invCandidate->matches.join(matches);
	}

	return invCandidate;
}

bool OptimizerRetrieval::matchBoolean(IndexScratch* indexScratch, 
	jrd_nod* boolean, USHORT scope) const
{
/**************************************
 *
 *	m a t c h B o o l e a n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	bool forward = true;

	jrd_nod* match = boolean->nod_arg[0];
	jrd_nod* value = (boolean->nod_count < 2) ? NULL : boolean->nod_arg[1];

#ifdef EXPRESSION_INDICES
	if (indexScratch->idx->idx_flags & idx_expressn)
	{
		// see if one side or the other is matchable to the index expression

	    fb_assert(indexScratch->idx->idx_expression != NULL);

		if (!expression_equal(tdbb, optimizer, indexScratch->idx, match, stream) ||
			(value && !computable(optimizer->opt_csb, value, stream, true, false)))
		{
			if (value &&
				expression_equal(tdbb, optimizer, indexScratch->idx, value, stream) &&
				computable(optimizer->opt_csb, match, stream, true, false))
			{
				match = boolean->nod_arg[1];
				value = boolean->nod_arg[0];
				// AB: Shouldn't the boolean forward be set to false here?
				//forward = false;
			}
			else
				return false;
		}
	}
	else {
#endif
		// If left side is not a field, swap sides. 
		// If left side is still not a field, give up

		if (match->nod_type != nod_field ||
			(USHORT)(IPTR) match->nod_arg[e_fld_stream] != stream ||
			(value && !computable(optimizer->opt_csb, value, stream, true, false)))
		{
			match = value;
			value = boolean->nod_arg[0];
			if (!match ||
				match->nod_type != nod_field ||
				(USHORT)(IPTR) match->nod_arg[e_fld_stream] != stream ||
				!computable(optimizer->opt_csb, value, stream, true, false))
			{
				return false;
			}
			forward = false;
		}
#ifdef EXPRESSION_INDICES
	}
#endif

	// match the field to an index, if possible, and save the value to be matched 
	// as either the lower or upper bound for retrieval, or both

	int count = 0;
	IndexScratchSegment** segment = indexScratch->segments.begin();
	for (int i = 0; i < indexScratch->idx->idx_count; i++) {

		if (
#ifdef EXPRESSION_INDICES
			(indexScratch->idx->idx_flags & idx_expressn) ||
#endif
			 (USHORT)(IPTR) match->nod_arg[e_fld_id] == indexScratch->idx->idx_rpt[i].idx_field)
		{

			switch (boolean->nod_type) {

				case nod_between:
					if (!forward || 
						!computable(optimizer->opt_csb, boolean->nod_arg[2],
						            stream, true, false))
					{
						return false;
					}
					segment[i]->matches.add(boolean);
					// AB: If we have already an exact match don't 
					// override it with worser matches.
					if (!((segment[i]->scanType == segmentScanEqual) ||
						(segment[i]->scanType == segmentScanEquivalent))) 
					{
						segment[i]->lowerValue = value;
						segment[i]->upperValue = boolean->nod_arg[2];
						segment[i]->scanType = segmentScanBetween;
					}
					break;

				case nod_equiv:
					segment[i]->matches.add(boolean);
					// AB: If we have already an exact match don't 
					// override it with worser matches.
					if (!(segment[i]->scanType == segmentScanEqual)) {
						segment[i]->lowerValue = segment[i]->upperValue = value;
						segment[i]->scanType = segmentScanEquivalent;
					}
					break;

				case nod_eql:
					segment[i]->matches.add(boolean);
					segment[i]->lowerValue = segment[i]->upperValue = value;
					segment[i]->scanType = segmentScanEqual;
					break;

				case nod_gtr:
				case nod_geq:
					segment[i]->matches.add(boolean);
					if (!((segment[i]->scanType == segmentScanEqual) ||
						(segment[i]->scanType == segmentScanEquivalent) ||
						(segment[i]->scanType == segmentScanBetween))) 
					{
						if (forward) {
							segment[i]->lowerValue = value;
							if (segment[i]->scanType == segmentScanLess) {
                        		segment[i]->scanType = segmentScanBetween;
							}
							else {
                        		segment[i]->scanType = segmentScanGreater;
							}
						}
						else {
							segment[i]->upperValue = value;
							if (segment[i]->scanType == segmentScanGreater) {
								segment[i]->scanType = segmentScanBetween;
							}
							else {
								segment[i]->scanType = segmentScanLess;
							}
						}
					}
					break;

				case nod_lss:
				case nod_leq:
					segment[i]->matches.add(boolean);
					if (!((segment[i]->scanType == segmentScanEqual) ||
						(segment[i]->scanType == segmentScanEquivalent) ||
						(segment[i]->scanType == segmentScanBetween))) 
					{
						if (forward) {
							segment[i]->upperValue = value;
							if (segment[i]->scanType == segmentScanGreater) {
								segment[i]->scanType = segmentScanBetween;
							}
							else {
								segment[i]->scanType = segmentScanLess;
							}
						}
						else {
							segment[i]->lowerValue = value;
							if (segment[i]->scanType == segmentScanLess) {
                        		segment[i]->scanType = segmentScanBetween;
							}
							else {
                        		segment[i]->scanType = segmentScanGreater;
							}
						}
					}
					break;


				case nod_starts:
					// Check if validate for using index
					if (!forward || !validateStarts(indexScratch, boolean, i)) {
						return false;
					}
					segment[i]->matches.add(boolean);
					if (!((segment[i]->scanType == segmentScanEqual) ||
						(segment[i]->scanType == segmentScanEquivalent))) 
					{
						segment[i]->lowerValue = segment[i]->upperValue = value;
						segment[i]->scanType = segmentScanStarting;
					}
					break;

				case nod_missing:
					segment[i]->matches.add(boolean);
					if (!((segment[i]->scanType == segmentScanEqual) ||
						(segment[i]->scanType == segmentScanEquivalent))) 
					{
						segment[i]->lowerValue = segment[i]->upperValue = value;
						segment[i]->scanType = segmentScanMissing;
					}
					break;


				default:		// If no known boolean type is found return 0
					return false;
			}

			// A match could be made
			if (segment[i]->scope < scope) {
				segment[i]->scope = scope;
			}

			++count;

			if (i == 0) {
				// If this is the first segment, then this index is a candidate.
				indexScratch->candidate = true;
			}

		}
	}

	return (count >= 1);
}

InversionCandidate* OptimizerRetrieval::matchOnIndexes(
	IndexScratchList* indexScratches, jrd_nod* boolean, USHORT scope) const
{
/**************************************
 *
 *	m a t c h O n I n d e x e s
 *
 **************************************
 *
 * Functional description
 *  Try to match boolean on every index.
 *  If the boolean is an "OR" node then a
 *  inversion candidate could be returned.
 *
 **************************************/
	DEV_BLKCHK(boolean, type_nod);


	// Handle the "OR" case up front
	if (boolean->nod_type == nod_or)
	{
		InversionCandidateList inversions;
		inversions.shrink(0);

		// Make list for index matches 
		IndexScratchList indexOrScratches;
		// Copy information from caller
		IndexScratch** indexScratch = indexScratches->begin();
		int i = 0;		
		for (; i < indexScratches->getCount(); i++) {
			indexOrScratches.add(FB_NEW(pool) IndexScratch(pool, indexScratch[i]));
		}
		// We use a scope variable to see on how 
		// deep we are in a nested or conjunction.
		scope++;

		InversionCandidate* invCandidate1 = 
			matchOnIndexes(&indexOrScratches, boolean->nod_arg[0], scope);
		if (invCandidate1) {
			inversions.add(invCandidate1);
		}
		// Get usable inversions based on indexOrScratches and scope
		if (boolean->nod_arg[0]->nod_type != nod_or) {
			getInversionCandidates(&inversions, &indexOrScratches, scope);
		}

		invCandidate1 = makeInversion(&inversions);
		if (!invCandidate1) {
			return NULL;
		}

		// Clear list to remove previously matched conjunctions
		indexOrScratches.clear();
		// Copy information from caller
		indexScratch = indexScratches->begin();
		i = 0;		
		for (; i < indexScratches->getCount(); i++) {
			indexOrScratches.add(FB_NEW(pool) IndexScratch(pool, indexScratch[i]));
		}
		// Clear inversion list
		inversions.clear();

		InversionCandidate* invCandidate2 = 
			matchOnIndexes(&indexOrScratches, boolean->nod_arg[1], scope);
		if (invCandidate2) {
			inversions.add(invCandidate2);
		}
		// Make inversion based on indexOrScratches and scope
		if (boolean->nod_arg[1]->nod_type != nod_or) {
			getInversionCandidates(&inversions, &indexOrScratches, scope);
		}
		invCandidate2 = makeInversion(&inversions);

		if (invCandidate2) {
			InversionCandidate* invCandidate = FB_NEW(pool) InversionCandidate(pool);
			invCandidate->inversion = 
				composeInversion(invCandidate1->inversion, invCandidate2->inversion, nod_bit_or);
			invCandidate->selectivity = invCandidate1->selectivity + invCandidate2->selectivity;
			invCandidate->indexes = invCandidate1->indexes + invCandidate2->indexes;
			invCandidate->nonFullMatchedSegments = 0;
			invCandidate->matchedSegments = 
				std::min(invCandidate1->matchedSegments, invCandidate2->matchedSegments);

			// Add matches conjunctions that exists in both left and right inversion
			if ((invCandidate1->matches.getCount()) && (invCandidate2->matches.getCount())) {
				Firebird::SortedArray<jrd_nod*> matches;
				int j;
				for (j = 0; j < invCandidate1->matches.getCount(); j++) {
					matches.add(invCandidate1->matches[j]);
				}				
				for (j = 0; j < invCandidate2->matches.getCount(); j++) {
					size_t pos;
					if (matches.find(invCandidate2->matches[j], pos)) {
						invCandidate->matches.add(invCandidate2->matches[j]);
					}					
				}				
			}
			return invCandidate;
		}
		return NULL;
	}

	if (boolean->nod_type == nod_and) {
		// Recursivly call this procedure for every boolean
		// and finally get candidate inversions.
		// Normally we come here from within a nod_or conjunction.
		InversionCandidateList inversions;
		inversions.shrink(0);

		InversionCandidate* invCandidate = 
			matchOnIndexes(indexScratches, boolean->nod_arg[0], scope);
		if (invCandidate) {
			inversions.add(invCandidate);
		}
		invCandidate = matchOnIndexes(indexScratches, boolean->nod_arg[1], scope);
		if (invCandidate) {
			inversions.add(invCandidate);
		}
		return makeInversion(&inversions);
	}

	// Walk through indexes
	IndexScratch** indexScratch = indexScratches->begin();
	for (int i = 0; i < indexScratches->getCount(); i++) {
		// Try to match the boolean against a index.
		if (!(indexScratch[i]->idx->idx_runtime_flags & idx_plan_dont_use)) {
			matchBoolean(indexScratch[i], boolean, scope); 
		}
	}
	return NULL;
}


bool OptimizerRetrieval::validateStarts(IndexScratch* indexScratch,
										jrd_nod* boolean, USHORT segment) const
{
/**************************************
 *
 *	v a l i d a t e S t a r t s
 *
 **************************************
 *
 * Functional description
 *  Check if the boolean is valid for
 *  using it against the given index segment.
 *
 **************************************/
	if (boolean->nod_type != nod_starts) {
		return false;
	}

	jrd_nod* field = boolean->nod_arg[0];
	jrd_nod* value = boolean->nod_arg[1];

#ifdef EXPRESSION_INDICES
	if (indexScratch->idx->idx_flags & idx_expressn) {

		// AB: What if the expression contains a number/float etc.. and
		// we use starting with against it? Is that allowed?
		fb_assert(indexScratch->idx->idx_expression != NULL);
		if (!(expression_equal(tdbb, optimizer, indexScratch->idx, field, stream) || 
			(value && !computable(optimizer->opt_csb, value, stream, true, false))))
		{
			// AB: Can we swap de left and right sides by a starting with?
			// X STARTING WITH 'a' that is never the same as 'a' STARTING WITH X
			if (value &&
				expression_equal(tdbb, optimizer, indexScratch->idx, value, stream) && 
				computable(optimizer->opt_csb, field, stream, true, false))
			{
				field = value; 
				value = boolean->nod_arg[0]; 
			}
			else {
				return false;
			}
		}
	}
	else
#endif
	{
		if (field->nod_type != nod_field) {
			// dimitr:	any idea how we can use an index in this case?
			//			The code below produced wrong results.
			// AB: I don't think that it would be effective, because
			// this must include many matches (think about empty string)
			return false;
			/*
			if (value->nod_type != nod_field)
				return NULL;
			field = value;
			value = boolean->nod_arg[0];
			*/
		}

		// Every string starts with an empty string so
		// don't bother using an index in that case.
		if (value->nod_type == nod_literal) {
			const dsc* literal_desc = &((Literal*) value)->lit_desc;
			if ((literal_desc->dsc_dtype == dtype_text &&
				literal_desc->dsc_length == 0) ||
				(literal_desc->dsc_dtype == dtype_varying &&
				literal_desc->dsc_length == sizeof(USHORT)))
			{
				return false;
			}
		}

		// AB: Check if the index-segment is usable for using starts.
		// Thus it should be of type string, etc...
		if ((USHORT)(IPTR) field->nod_arg[e_fld_stream] != stream ||
			(USHORT)(IPTR) field->nod_arg[e_fld_id] != indexScratch->idx->idx_rpt[segment].idx_field
			|| !(indexScratch->idx->idx_rpt[segment].idx_itype == idx_string
				|| indexScratch->idx->idx_rpt[segment].idx_itype == idx_byte_array
				|| indexScratch->idx->idx_rpt[segment].idx_itype == idx_metadata
				|| indexScratch->idx->idx_rpt[segment].idx_itype >= idx_first_intl_string)
			|| !computable(optimizer->opt_csb, value, stream, false, false))
		{
			return false;
		}
	}

	return true;
}

IndexRelationship::IndexRelationship() :
	stream(0), unique(false), cost(0)
{
/**************************************
 *
 *	I n d e x R e l a t i on s h i p
 *
 **************************************
 *
 *  Initialize
 *
 **************************************/
}


InnerJoinStreamInfo::InnerJoinStreamInfo(MemoryPool& p) :
	stream(0), baseUnique(false), baseCost(0), baseIndexes(0), 
	baseConjunctionMatches(0), used(false),
	indexedRelationships(p), previousExpectedStreams(p)
{
/**************************************
 *
 *	I n n e r J o i n S t r e a m I n f o
 *
 **************************************
 *
 *  Initialize
 *
 **************************************/
	indexedRelationships.shrink(0);
	previousExpectedStreams.shrink(0);
}

bool InnerJoinStreamInfo::independent() const
{
/**************************************
 *
 *	i n d e p e n d e n t
 *
 **************************************
 *
 *  Return true if this stream can't be 
 *  used by other streams and it can't
 *  use index retrieval based on other
 *  streams.
 *
 **************************************/
	return (indexedRelationships.getCount() == 0) &&
		(previousExpectedStreams.getCount() == 0);
}


OptimizerInnerJoin::OptimizerInnerJoin(MemoryPool& p, OptimizerBlk* opt, UCHAR*	streams,
		RiverStack& river_stack, jrd_nod** sort_clause, 
		jrd_nod** project_clause, jrd_nod* plan_clause) :
	pool(p), tdbb(NULL), innerStreams(p)
{
/**************************************
 *
 *	O p t i m i z e r I n n e r J o i n 
 *
 **************************************
 *
 *  Initialize
 *
 **************************************/
	SET_TDBB(tdbb);
	this->database = tdbb->tdbb_database;
	this->optimizer = opt;
	this->csb = this->optimizer->opt_csb;
	this->sort = sort_clause;
	this->project = project_clause;
	this->plan = plan_clause;
	this->remainingStreams = 0;

	innerStreams.grow(streams[0]);
	InnerJoinStreamInfo** innerStream = innerStreams.begin();
	for (int i = 0; i < innerStreams.getCount() ; i++) {
		innerStream[i] = FB_NEW(p) InnerJoinStreamInfo(p);
		innerStream[i]->stream = streams[i + 1];
	}

	calculateCardinalities();
	calculateStreamInfo();
}

OptimizerInnerJoin::~OptimizerInnerJoin()
{
/**************************************
 *
 *	~O p t i m i z e r I n n e r J o i n 
 *
 **************************************
 *
 *  Finish with giving back memory.
 *
 **************************************/

	for (int i = 0; i < innerStreams.getCount(); i++) {
		for (int j = 0; j < innerStreams[i]->indexedRelationships.getCount(); j++) {
			delete innerStreams[i]->indexedRelationships[j];
		}
		innerStreams[i]->indexedRelationships.clear();
		delete innerStreams[i];
	}
	innerStreams.clear();
}

void OptimizerInnerJoin::calculateCardinalities()
{
/**************************************
 *
 *	c a l c u l a t e C a r d i n a l i t i e s
 *
 **************************************
 *
 *  Get the cardinality for every stream.
 *
 **************************************/

	for (int i = 0; i < innerStreams.getCount(); i++) {		
		CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[innerStreams[i]->stream];
		fb_assert(csb_tail);
		jrd_rel* relation = csb_tail->csb_relation;
		fb_assert(relation);
		const Format* format = CMP_format(tdbb, csb, (USHORT)innerStreams[i]->stream);
		fb_assert(format);
		csb_tail->csb_cardinality = getRelationCardinality(tdbb, relation, format);
	}

}

void OptimizerInnerJoin::calculateStreamInfo()
{
/**************************************
 *
 *	c a l c u l a t e S t r e a m I n f o
 *
 **************************************
 *
 *  Calculate the needed information for
 *  all streams.
 *
 **************************************/

	int i = 0;
	// First get the base cost without any relation to an other inner join stream.
	for (i = 0; i < innerStreams.getCount(); i++) {		
		CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[innerStreams[i]->stream];
		csb_tail->csb_flags |= csb_active;

		OptimizerRetrieval* optimizerRetrieval = FB_NEW(pool) 
			OptimizerRetrieval(pool, optimizer, innerStreams[i]->stream, false, false, NULL);
		InversionCandidate* candidate = optimizerRetrieval->getCost();
		innerStreams[i]->baseCost = candidate->selectivity * csb_tail->csb_cardinality;
		innerStreams[i]->baseIndexes = candidate->indexes;
		innerStreams[i]->baseUnique = candidate->unique;
		innerStreams[i]->baseConjunctionMatches = candidate->matches.getCount();
		delete candidate;
		delete optimizerRetrieval;

		csb_tail->csb_flags &= ~csb_active;
	}

	for (i = 0; i < innerStreams.getCount(); i++) {
		CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[innerStreams[i]->stream];
		csb_tail->csb_flags |= csb_active;

		// Find streams that have a indexed relationship to this 
		// stream and add the information.
		for (int j = 0; j < innerStreams.getCount(); j++) {		
			if (innerStreams[j]->stream != innerStreams[i]->stream) {
				getIndexedRelationship(innerStreams[i], innerStreams[j]);
			}
		}

		csb_tail->csb_flags &= ~csb_active;
	}

	// Sort the streams based on independecy and cost.
	// Except when a PLAN was forced.
	if (!plan && (innerStreams.getCount() > 1)) {
		StreamInfoList tempStreams(pool);

		for (i = 0; i < innerStreams.getCount(); i++) {
			int index = 0;
			for (; index < tempStreams.getCount(); index++) {
				// First those streams which can't be used by other streams
				// or can't depend on a stream.
				if (innerStreams[i]->independent() && !tempStreams[index]->independent()) {
					break;
				}
				// Next those with the lowest previous expected streams
				int compare = innerStreams[i]->previousExpectedStreams.getCount() -
					tempStreams[index]->previousExpectedStreams.getCount();
				if (compare < 0) {
					break;
				}
				if (compare == 0) {
					// Next those with the cheapest base cost
					if (innerStreams[i]->baseCost < tempStreams[index]->baseCost) {
						break;
					}
				}
			}
			tempStreams.insert(index, innerStreams[i]);
		}
		
		// Finally update the innerStreams with the sorted streams
		innerStreams.clear();
		innerStreams.join(tempStreams);
	}
}

bool OptimizerInnerJoin::cheaperRelationship(IndexRelationship* checkRelationship,
		IndexRelationship* withRelationship) const
{
/**************************************
 *
 *	c h e a p e r R e l a t i o n s h i p
 *
 **************************************
 *
 *  Return true if checking relationship
 *	is cheaper as withRelationship.
 *
 **************************************/
	double compareValue = checkRelationship->cost / withRelationship->cost;
	if ((compareValue >= 0.98) && (compareValue <= 1.02)) {
		// cost is nearly the same, now check on cardinality.
		if (checkRelationship->cardinality < withRelationship->cardinality) {
			return true;
		}
	}
	else if (checkRelationship->cost < withRelationship->cost) {
		return true;
	}
	return false;
}

bool OptimizerInnerJoin::estimateCost(USHORT stream, double *cost, 
	double *resulting_cardinality) const
{
/**************************************
 *
 *	e s t i m a t e C o s t
 *
 **************************************
 *
 *  Estimate the cost for the stream.
 *
 **************************************/
	const CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[stream];
	double cardinality = csb_tail->csb_cardinality;

	// Create the optimizer retrieval generation class and calculate
	// which indexes will be used and the total estimated 
	// selectivity will be returned.
	OptimizerRetrieval* optimizerRetrieval = FB_NEW(pool) 
		OptimizerRetrieval(pool, optimizer, stream, false, false, NULL);
	// I'm allowed to apply delete to this const object, is this MS extension? Let gcc decide.
	const InversionCandidate* candidate = optimizerRetrieval->getCost();
	double selectivity = candidate->selectivity;
	if (candidate->indexes) {
		const double nodesPerPage = ((double)database->dbb_page_size / 10);
		const double indexCost = 2 + ((cardinality * selectivity) / nodesPerPage);
		*cost = (cardinality * selectivity) + (candidate->indexes * indexCost);
	}
	else {
		*cost = cardinality;
	}

	cardinality *= selectivity;

	if (candidate->unique) {
		*resulting_cardinality = cardinality;
	}
	else {
		*resulting_cardinality = MAX(cardinality, 1.0);
	}

	const bool useIndexRetrieval = (candidate->indexes >= 1);
	delete candidate;
	delete optimizerRetrieval;

	return useIndexRetrieval;
}

int OptimizerInnerJoin::findJoinOrder()
{
/**************************************
 *
 *	f i n d J o i n O r d e r
 *
 **************************************
 *
 *  Find the best order out of the streams.
 *  First return a stream if it can't use
 *  a index based on a previous stream and
 *  it can't be used by another stream.
 *  Next loop through the remaining streams
 *  and find the best order.
 *
 **************************************/

	optimizer->opt_best_count = 0;

	int i = 0;
	remainingStreams = 0;
	for (i = 0; i < innerStreams.getCount(); i++) {
		if (!innerStreams[i]->used) {
			remainingStreams++;
			if (innerStreams[i]->independent()) {
				optimizer->opt_streams[0].opt_best_stream = innerStreams[i]->stream;
				optimizer->opt_best_count = 1;
			}
		}
	}

	if (optimizer->opt_best_count == 0) {
		IndexedRelationships indexedRelationships(pool);
		for (i = 0; i < innerStreams.getCount(); i++) {
			if (!innerStreams[i]->used) {
				indexedRelationships.clear();
				findBestOrder(0, innerStreams[i], &indexedRelationships, (double) 0, (double) 1);

				if (plan) {
					// If a explicit PLAN was specified we should be ready;
					break;
				}
#ifdef OPT_DEBUG
				// Debug
				printProcessList(&indexedRelationships, innerStreams[i]->stream);
#endif
			}
		}
	}

	// Mark streams as used
	for (i = 0; i < optimizer->opt_best_count; i++) {
		InnerJoinStreamInfo* streamInfo = 
			getStreamInfo(optimizer->opt_streams[i].opt_best_stream);
		streamInfo->used = true;
	}

	return optimizer->opt_best_count;
}

void OptimizerInnerJoin::findBestOrder(int position, InnerJoinStreamInfo* stream, 
	IndexedRelationships* processList, double cost, double cardinality)
{
/**************************************
 *
 *	f i n d B e s t O r d e r
 *
 **************************************
 *  Make different combinations to find 
 *  out the join order. 
 *  For every position we start with the
 *  stream that has the best selectivity
 *  for that position. If we've have 
 *  used up all our streams after that
 *  we assume we're done.
 *
 **************************************/

	fb_assert(processList);

	// do some initializations.
	csb->csb_rpt[stream->stream].csb_flags |= csb_active;
	optimizer->opt_streams[position].opt_stream_number = stream->stream;
	position++;
	const OptimizerBlk::opt_stream* order_end = optimizer->opt_streams.begin() + position;

	// Save the various flag bits from the optimizer block to reset its
	// state after each test.
	Firebird::HalfStaticArray<bool, OPT_STATIC_ITEMS> streamFlags(pool);
	streamFlags.grow(innerStreams.getCount());
	size_t i;
	for (i = 0; i < streamFlags.getCount(); i++) {
		streamFlags[i] = innerStreams[i]->used;
	}

	// Compute delta and total estimate cost to fetch this stream.
	double position_cost, position_cardinality, new_cost = 0, new_cardinality = 0;

	if (!plan) {
		estimateCost(stream->stream, &position_cost, &position_cardinality);
		new_cost = cost + cardinality * position_cost;
		new_cardinality = position_cardinality * cardinality;
	}

	optimizer->opt_combinations++;
	// If the partial order is either longer than any previous partial order,
	// or the same length and cheap, save order as "best".
	if (position > optimizer->opt_best_count ||
		(position == optimizer->opt_best_count && new_cost < optimizer->opt_best_cost))
	{
		optimizer->opt_best_count = position;
		optimizer->opt_best_cost = new_cost;
		for (OptimizerBlk::opt_stream* tail = optimizer->opt_streams.begin(); tail < order_end; tail++) {
			tail->opt_best_stream = tail->opt_stream_number;
		}
	}

#ifdef OPT_DEBUG
	// Debug information
	printFoundOrder(position, new_cost, new_cardinality);
#endif

	// mark this stream as "used" in the sense that it is already included 
	// in this particular proposed stream ordering.
	stream->used = true;
	bool done = false;

	// if we've used up all the streams there's no reason to go any further.
	if (position == remainingStreams) {
		done = true;
	}
	// If we know a combination with all streams used and the 
	// current cost is higher as the one from the best we're done.
	if ((optimizer->opt_best_count == remainingStreams) &&
		(optimizer->opt_best_cost < new_cost))
	{
		done = true;
	}

	if (!done && !plan) {
		// Add these relations to the processing list
		int j = 0;
		for (j = 0; j < stream->indexedRelationships.getCount(); j++) {
			IndexRelationship* relationship = stream->indexedRelationships[j];
			InnerJoinStreamInfo* relationStreamInfo = 
				getStreamInfo(relationship->stream);
			if (!relationStreamInfo->used) {
				bool found = false;
				IndexRelationship** relationships = processList->begin();
				for (int index = 0; index < processList->getCount(); index++) {
					if (relationStreamInfo->stream == relationships[index]->stream) {
						// If the cost of this relationship is cheaper then remove the
						// old realtionship and add this one.
						if (cheaperRelationship(relationship, relationships[index])) {
							processList->remove(index);
							break;
						}
						else {
							found = true;
							break;
						}
					}
				}
				if (!found) {
					// Add relationship sorted on cost (cheapest as first)
					IndexRelationship** relationships = processList->begin();
					int index = 0;
					for (; index < processList->getCount(); index++) {
						if (cheaperRelationship(relationship, relationships[index])) {
							break;
						}
					}
					processList->insert(index, relationship);
				}		
			}
		}

		IndexRelationship** nextRelationship = processList->begin();
		for (j = 0; j < processList->getCount(); j++) {
			InnerJoinStreamInfo* relationStreamInfo = 
				getStreamInfo(nextRelationship[j]->stream);
			if (!relationStreamInfo->used) {
				findBestOrder(position, relationStreamInfo, processList, 
					new_cost, new_cardinality);
				break;
			}
		}
	}

	if (plan) {
		// If a explicit PLAN was specific pick the next relation.
		// The order in innerStreams is expected to be exactly the order as 
		// specified in the explicit PLAN.
		for (int j = 0; j < innerStreams.getCount(); j++) {
			InnerJoinStreamInfo* nextStream = innerStreams[j];
			if (!nextStream->used) {
				findBestOrder(position, nextStream, processList, new_cost, new_cardinality);
				break;
			}
		}
	}

	// Clean up from any changes made for compute the cost for this stream
	csb->csb_rpt[stream->stream].csb_flags &= ~csb_active;
	for (i = 0; i < streamFlags.getCount(); i++) {
		innerStreams[i]->used = streamFlags[i];
	}
}

void OptimizerInnerJoin::getIndexedRelationship(InnerJoinStreamInfo* baseStream, 
	InnerJoinStreamInfo* testStream)
{
/**************************************
 *
 *	g e t I n d e x e d R e l a t i o n s h i p
 *
 **************************************
 *
 *  Check if the testStream can use a index
 *  when the baseStream is active. If so
 *  then we create a indexRelationship
 *  and fill it with the needed information.
 *  The reference is added to the baseStream
 *  and the baseStream is added as previous 
 *  expected stream to the testStream.
 *
 **************************************/

	CompilerScratch::csb_repeat* csb_tail = &csb->csb_rpt[testStream->stream];
	csb_tail->csb_flags |= csb_active;

	OptimizerRetrieval* optimizerRetrieval = FB_NEW(pool) 
		OptimizerRetrieval(pool, optimizer, testStream->stream, false, false, NULL);
	InversionCandidate* candidate = optimizerRetrieval->getCost();
	double const cost = candidate->selectivity * csb_tail->csb_cardinality;

	size_t pos;
	if (candidate->dependentFromStreams.find(baseStream->stream, pos)) {
		// If we could use more conjunctions on the testing stream with the base stream
		// active as without the base stream then the test stream has a indexed
		// relationship with the base stream.
		IndexRelationship* indexRelationship = FB_NEW(pool) IndexRelationship();
		indexRelationship->stream = testStream->stream;
		indexRelationship->unique = candidate->unique;
		indexRelationship->cost = cost;
		indexRelationship->cardinality = csb_tail->csb_cardinality;

		// indexRelationship are kept sorted on cost and unique in the indexRelations array.
		// The unqiue and cheapest indexed relatioships are on the first position.
		int index = 0;
		for (; index < baseStream->indexedRelationships.getCount(); index++) {
			if (cheaperRelationship(indexRelationship, baseStream->indexedRelationships[index])) {
				break;
			}
		}
		baseStream->indexedRelationships.insert(index, indexRelationship);
		testStream->previousExpectedStreams.add(baseStream->stream);
	}
	delete candidate;
	delete optimizerRetrieval;

	csb_tail->csb_flags &= ~csb_active;
}

InnerJoinStreamInfo* OptimizerInnerJoin::getStreamInfo(int stream)
{
/**************************************
 *
 *	g e t S t r e a m I n f o
 *
 **************************************
 *
 *  Return stream information based on
 *  the stream number.
 *
 **************************************/

	for (int i = 0; i < innerStreams.getCount(); i++) {
		if (innerStreams[i]->stream == stream) {
			return innerStreams[i];
		}
	}

	// We should never come here
	fb_assert(false);
	return NULL;
}

#ifdef OPT_DEBUG
void OptimizerInnerJoin::printFoundOrder(int position, double cost, double cardinality) const
{
/**************************************
 *
 *	p r i n t F o u n d O r d e r
 *
 **************************************
 *
 *  Dump currently passed streams to a
 *  debug file.
 *
 **************************************/

	FILE *opt_debug_file = fopen(OPTIMIZER_DEBUG_FILE, "a");
	fprintf(opt_debug_file, "order pos. %2.2d, streams: ", position);
	const OptimizerBlk::opt_stream* tail = optimizer->opt_streams.begin();
	const OptimizerBlk::opt_stream* const order_end = tail + position;
	for (; tail < order_end; tail++) {
		fprintf(opt_debug_file, "%2.2d - ", tail->opt_stream_number);
	}
	fprintf(opt_debug_file, "\tcardinality: %10.2f\tcost: %10.2f\n", cardinality, cost);
	fclose(opt_debug_file);
}

void OptimizerInnerJoin::printProcessList(const IndexedRelationships* processList,
	int stream) const
{
/**************************************
 *
 *	p r i n t P r o c e s s L i s t
 *
 **************************************
 *
 *  Dump the processlist to a debug file.
 *
 **************************************/

	FILE *opt_debug_file = fopen(OPTIMIZER_DEBUG_FILE, "a");
	fprintf(opt_debug_file, "processlist, basestream %2.2d, relationships: \n", stream);
	const IndexRelationship* const* relationships = processList->begin();
	for (int i = 0; i < processList->getCount(); i++) {
		fprintf(opt_debug_file, "\t\t%2.2d (%10.2f)\n", relationships[i]->stream, relationships[i]->cost);
	}
	fclose(opt_debug_file);
}
#endif


} // namespace
