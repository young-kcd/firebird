/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cmp.cpp
 *	DESCRIPTION:	Request compiler
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
 *
 * 2001.07.28: John Bellardo: Added code to handle rse_skip.
 * 2001.07.17 Claudio Valderrama: Stop crash when parsing user-supplied SQL plan.
 * 2001.10.04 Claudio Valderrama: Fix annoying & invalid server complaint about
 *   triggers not having REFERENCES privilege over their owner table.
 * 2002.02.24 Claudio Valderrama: substring() should signal output as string even
 *   if source is blob and should check implementation limits on field lengths.
 * 2002.02.25 Claudio Valderrama: concatenate() should be a civilized function.
 *   This closes the heart of SF Bug #518282.
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <string.h>
#include <stdlib.h>				// abort
#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/align.h"
#include "../jrd/lls.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/scl.h"
#include "../jrd/tra.h"
#include "../jrd/lck.h"
#include "../jrd/irq.h"
#include "../jrd/drq.h"
#include "../jrd/intl.h"
#include "../jrd/btr.h"
#include "../jrd/sort.h"
#include "../jrd/gdsassert.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/idx_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/dbg_proto.h"	// DBG_supervisor
#include "../jrd/execute_statement.h"
#include "../jrd/Optimizer.h"

#include "../jrd/DataTypeUtil.h"
#include "../jrd/SysFunction.h"

// Pick up relation ids
#include "../jrd/ini.h"

#include "../common/classes/auto.h"
#include "../common/utils_proto.h"
#include "../dsql/Nodes.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/ValuesImpl.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/recsrc/Cursor.h"
#include "../jrd/Function.h"
#include "../jrd/misc_func_ids.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/StmtNodes.h"

using namespace Jrd;
using namespace Firebird;


// Firebird provides transparent conversion from string to date in
// contexts where it makes sense.  This macro checks a descriptor to
// see if it is something that *could* represent a date value

inline bool COULD_BE_DATE(const dsc desc)
{
	return ((DTYPE_IS_DATE(desc.dsc_dtype)) || (desc.dsc_dtype <= dtype_any_text));
}

// One of d1,d2 is time, the other is date
inline bool IS_DATE_AND_TIME(const dsc d1, const dsc d2)
{
	return (((d1.dsc_dtype == dtype_sql_time) && (d2.dsc_dtype == dtype_sql_date)) ||
	((d2.dsc_dtype == dtype_sql_time) && (d1.dsc_dtype == dtype_sql_date)));
}


namespace
{
	// Node copier for views.
	class ViewNodeCopier : public NodeCopier
	{
	public:
		ViewNodeCopier(CompilerScratch* aCsb, UCHAR* aRemap)
			: NodeCopier(aCsb, aRemap)
		{
		}

	protected:
		virtual bool remapArgument()
		{
			return true;
		}

		virtual USHORT remapField(USHORT stream, USHORT fldId)
		{
			jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
			jrd_fld* field = MET_get_field(relation, fldId);

			if (field->fld_source)
				fldId = (USHORT)(IPTR) field->fld_source->nod_arg[e_fld_id];

			return fldId;
		}
	};

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
		virtual USHORT getFieldId(jrd_nod* input)
		{
			if ((input->nod_flags & nod_id) && !input->nod_arg[e_fld_id] &&
				!input->nod_arg[e_fld_stream])
			{
				return fldId;
			}

			return NodeCopier::getFieldId(input);
		}

	private:
		USHORT fldId;
	};

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

		static jrd_nod* copy(thread_db* tdbb, CompilerScratch* csb, jrd_nod* input)
		{
			SubExprNodeCopier obj(csb);
			return static_cast<NodeCopier&>(obj).copy(tdbb, input);
		}

	private:
		UCHAR localMap[JrdStatement::MAP_LENGTH];
	};
}	// namespace

static jrd_nod* catenate_nodes(thread_db*, NodeStack&);
static jrd_nod* convertNeqAllToNotAny(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node);
static void expand_view_nodes(thread_db*, CompilerScratch*, USHORT, NodeStack&, nod_t, bool);
static jrd_nod* make_defaults(thread_db*, CompilerScratch*, USHORT, jrd_nod*);
static jrd_nod* make_validation(thread_db*, CompilerScratch*, USHORT);
static void mark_variant(CompilerScratch* csb, USHORT stream);
static void pass1_erase(thread_db*, CompilerScratch*, jrd_nod*);
static jrd_nod* pass1_expand_view(thread_db*, CompilerScratch*, USHORT, USHORT, bool);
static void pass1_modify(thread_db*, CompilerScratch*, jrd_nod*);
static bool pass1_store(thread_db*, CompilerScratch*, jrd_nod*);
static RelationSourceNode* pass1_update(thread_db*, CompilerScratch*, jrd_rel*, const trig_vec*, USHORT, USHORT,
	SecurityClass::flags_t, jrd_rel*, USHORT);
static jrd_nod* pass2_validation(thread_db*, CompilerScratch*, const Item&);
static RecordSource* post_rse(thread_db*, CompilerScratch*, RseNode*);
static void	post_trigger_access(CompilerScratch*, jrd_rel*, ExternalAccess::exa_act, jrd_rel*);

#ifdef CMP_DEBUG
#include <stdarg.h>
IMPLEMENT_TRACE_ROUTINE(cmp_trace, "CMP")
#endif


jrd_nod* CMP_clone_node(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	C M P _ c l o n e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Clone a node.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	return NodeCopier::copy(tdbb, csb, node, NULL);
}


jrd_nod* CMP_clone_node_opt(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	C M P _ c l o n e _ n o d e _ o p t
 *
 **************************************
 *
 * Functional description
 *	Clone a value node for the optimizer.  Make a copy of the node
 *	(if necessary) and assign impure space.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	if (node->nod_type == nod_argument) {
		return node;
	}

	jrd_nod* clone = NodeCopier::copy(tdbb, csb, node, NULL);
	CMP_pass2(tdbb, csb, clone, 0);

	return clone;
}


jrd_req* CMP_compile2(thread_db* tdbb, const UCHAR* blr, ULONG blr_length, bool internal_flag,
					  USHORT dbginfo_length, const UCHAR* dbginfo)
{
/**************************************
 *
 *	C M P _ c o m p i l e 2
 *
 **************************************
 *
 * Functional description
 *	Compile a BLR request.
 *
 **************************************/
	jrd_req* request = NULL;

	SET_TDBB(tdbb);
	Database* const dbb = tdbb->getDatabase();

	// 26.09.2002 Nickolay Samofatov: default memory pool will become statement pool
	// and will be freed by CMP_release
	MemoryPool* const new_pool = dbb->createPool();

	try
	{
		Jrd::ContextPoolHolder context(tdbb, new_pool);

		CompilerScratch* csb =
			PAR_parse(tdbb, blr, blr_length, internal_flag, dbginfo_length, dbginfo);

		request = JrdStatement::makeRequest(tdbb, csb, internal_flag);
		new_pool->setStatsGroup(request->req_memory_stats);

#ifdef CMP_DEBUG
		if (csb->csb_dump.hasData())
		{
			csb->dump("streams:\n");
			for (unsigned i = 0; i < csb->csb_n_stream; ++i)
			{
				const CompilerScratch::csb_repeat& s = csb->csb_rpt[i];
				csb->dump(
					"\t%2d - view_stream: %2d; alias: %s; relation: %s; procedure: %s; view: %s\n",
					i, s.csb_view_stream,
					(s.csb_alias ? s.csb_alias->c_str() : ""),
					(s.csb_relation ? s.csb_relation->rel_name.c_str() : ""),
					(s.csb_procedure ? s.csb_procedure->getName().toString().c_str() : ""),
					(s.csb_view ? s.csb_view->rel_name.c_str() : ""));
			}

			cmp_trace("\n%s\n", csb->csb_dump.c_str());
		}
#endif

		request->getStatement()->verifyAccess(tdbb);

		delete csb;
	}
	catch (const Firebird::Exception& ex)
	{
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		if (request)
			CMP_release(tdbb, request);
		else
			dbb->deletePool(new_pool);
		ERR_punt();
	}

	return request;
}


CompilerScratch::csb_repeat* CMP_csb_element(CompilerScratch* csb, USHORT element)
{
/**************************************
 *
 *	C M P _ c s b _ e l e m e n t
 *
 **************************************
 *
 * Functional description
 *	Find tail element of compiler scratch block.  If the csb isn't big
 *	enough, extend it.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);
	CompilerScratch::csb_repeat empty_item;
	while (element >= csb->csb_rpt.getCount()) {
		csb->csb_rpt.add(empty_item);
	}
	return &csb->csb_rpt[element];
}


void CMP_fini(thread_db* tdbb)
{
/**************************************
 *
 *	C M P _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of resource locks during shutdown.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* const dbb = tdbb->getDatabase();

	CMP_shutdown_database(tdbb);	// Shutdown shared database locks.

	// And release the system requests.

	for (JrdStatement** itr = dbb->dbb_internal.begin(); itr != dbb->dbb_internal.end(); ++itr)
	{
		if (*itr)
			(*itr)->release(tdbb);
	}

	for (JrdStatement** itr = dbb->dbb_dyn_req.begin(); itr != dbb->dbb_dyn_req.end(); ++itr)
	{
		if (*itr)
			(*itr)->release(tdbb);
	}
}


const Format* CMP_format(thread_db* tdbb, CompilerScratch* csb, USHORT stream)
{
/**************************************
 *
 *	C M P _ f o r m a t
 *
 **************************************
 *
 * Functional description
 *	Pick up a format for a stream.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];

	if (tail->csb_format) {
		return tail->csb_format;
	}

	if (tail->csb_relation) {
		return tail->csb_format = MET_current(tdbb, tail->csb_relation);
	}

	if (tail->csb_procedure) {
		return tail->csb_format = tail->csb_procedure->prc_format;
	}

	IBERROR(222);				// msg 222 bad blr - invalid stream
	return NULL;
}


void CMP_get_desc(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node, DSC* desc)
{
/**************************************
 *
 *	C M P _ g e t _ d e s c
 *
 **************************************
 *
 * Functional description
 *	Compute descriptor for value expression.
 *
 **************************************/
	USHORT dtype = dtype_unknown;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	switch (node->nod_type)
	{
	case nod_max:
	case nod_min:
	case nod_from:
		CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		return;

	case nod_total:
		if (node->nod_type == nod_total)
			CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		else
			CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		switch (dtype = desc->dsc_dtype)
		{
		case dtype_short:
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = sizeof(SLONG);
			node->nod_scale = desc->dsc_scale;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;

		case dtype_unknown:
			desc->dsc_dtype = dtype_unknown;
			desc->dsc_length = 0;
			node->nod_scale = 0;
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
			node->nod_flags |= nod_double;
			return;

		case dtype_quad:
			desc->dsc_dtype = dtype_quad;
			desc->dsc_length = sizeof(SQUAD);
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			node->nod_scale = desc->dsc_scale;
			node->nod_flags |= nod_quad;
#ifdef NATIVE_QUAD
			return;
#endif

		default:
			fb_assert(false);
			// FALLINTO
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:
		case dtype_blob:
		case dtype_array:
		case dtype_dbkey:
			// break to error reporting code
			break;
		}
		break;

	case nod_prot_mask:
	case nod_null:
	case nod_count:
	case nod_gen_id:
	case nod_lock_state:
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_field:
		{
			const USHORT stream = (USHORT) (IPTR) node->nod_arg[e_fld_stream];
			const USHORT id = (USHORT) (IPTR) node->nod_arg[e_fld_id];
			const Format* format = CMP_format(tdbb, csb, stream);

			if (id >= format->fmt_count)
			{
				desc->dsc_dtype = dtype_unknown;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
			}
			else
			{
				*desc = format->fmt_desc[id];

				// fix UNICODE_FSS wrong length used in system tables
				jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

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
			return;
		}

	case nod_scalar:
		{
			jrd_nod* sub = node->nod_arg[e_scl_field];
			jrd_rel* relation = csb->csb_rpt[(USHORT)(IPTR) sub->nod_arg[e_fld_stream]].csb_relation;
			const USHORT id = (USHORT)(IPTR) sub->nod_arg[e_fld_id];
			const jrd_fld* field = MET_get_field(relation, id);
			const ArrayField* array;
			if (!field || !(array = field->fld_array)) {
				IBERROR(223);	// msg 223 argument of scalar operation must be an array
			}
			*desc = array->arr_desc.iad_rpt[0].iad_desc;
			if (array->arr_desc.iad_dimensions > MAX_ARRAY_DIMENSIONS)
				IBERROR(306); // Found array data type with more than 16 dimensions
			return;
		}

	case nod_divide:
		{
			DSC desc1, desc2;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
			// for compatibility with older versions of the product, we accept
			// text types for division in blr_version4 (dialect <= 1) only
			if (!(DTYPE_IS_NUMERIC(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc1.dsc_dtype)))
			{
				if (desc1.dsc_dtype != dtype_unknown) {
					break;		// error, dtype not supported by arithmetic
				}
			}
			if (!(DTYPE_IS_NUMERIC(desc2.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype)))
			{
				if (desc2.dsc_dtype != dtype_unknown) {
					break;		// error, dtype not supported by arithmetic
				}
			}
		}
		desc->dsc_dtype = DEFAULT_DOUBLE;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_average:
		if (node->nod_type == nod_average) {
			CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		}
		if (!(DTYPE_IS_NUMERIC(desc->dsc_dtype) || DTYPE_IS_TEXT(desc->dsc_dtype)))
		{
			if (desc->dsc_dtype != dtype_unknown) {
				break;
			}
		}
		desc->dsc_dtype = DEFAULT_DOUBLE;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_add:
	case nod_subtract:
		{
			DSC desc1, desc2;

			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);

			/* 92/05/29 DAVES - don't understand why this is done for ONLY
			   dtype_text (eg: not dtype_cstring or dtype_varying) Doesn't
			   appear to hurt.

			   94/04/04 DAVES - NOW I understand it!  QLI will pass floating
			   point values to the engine as text.  All other numeric constants
			   it turns into either integers or longs (with scale). */

			USHORT dtype1 = desc1.dsc_dtype;
			if (dtype_int64 == dtype1) {
				dtype1 = dtype_double;
			}
			USHORT dtype2 = desc2.dsc_dtype;
			if (dtype_int64 == dtype2) {
				dtype2 = dtype_double;
			}

			if (dtype1 == dtype_text || dtype2 == dtype_text) {
				dtype = MAX(MAX(dtype1, dtype2), (UCHAR) DEFAULT_DOUBLE);
			}
			else {
				dtype = MAX(dtype1, dtype2);
			}

			switch (dtype)
			{
			case dtype_short:
				desc->dsc_dtype = dtype_long;
				desc->dsc_length = sizeof(SLONG);
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
				{
					desc->dsc_scale = 0;
				}
				else {
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				}
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_sql_date:
			case dtype_sql_time:
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_TEXT(desc2.dsc_dtype))
				{
					ERR_post(Arg::Gds(isc_expression_eval_err));
				}
				// FALL INTO

			case dtype_timestamp:
				node->nod_flags |= nod_date;

				fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_DATE(desc2.dsc_dtype));

				if (COULD_BE_DATE(desc1) && COULD_BE_DATE(desc2))
				{
					if (node->nod_type == nod_subtract)
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

						if (DTYPE_IS_TEXT(dtype1)) {
							dtype = dtype_timestamp;
						}
						else if (DTYPE_IS_TEXT(dtype2)) {
							dtype = dtype_timestamp;
						}
						else if (dtype1 == dtype2) {
							dtype = dtype1;
						}
						else if (dtype1 == dtype_timestamp && dtype2 == dtype_sql_date)
						{
							dtype = dtype_timestamp;
						}
						else if (dtype2 == dtype_timestamp && dtype1 == dtype_sql_date)
						{
							dtype = dtype_timestamp;
						}
						else {
							ERR_post(Arg::Gds(isc_expression_eval_err));
						}

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
					else if (IS_DATE_AND_TIME(desc1, desc2))
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
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) || (node->nod_type == nod_add))
				{
					// <date> +/- <non-date> || <non-date> + <date>
					desc->dsc_dtype = desc1.dsc_dtype;
					if (!DTYPE_IS_DATE(desc->dsc_dtype)) {
						desc->dsc_dtype = desc2.dsc_dtype;
					}
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
				{
					desc->dsc_scale = 0;
				}
				else {
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				}
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
		}
		break;

	case nod_gen_id2:
		desc->dsc_dtype = dtype_int64;
		desc->dsc_length = sizeof(SINT64);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_add2:
	case nod_subtract2:
		{
			DSC desc1, desc2;

			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
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
			{
				dtype = dtype_int64;
			}
			else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype) && DTYPE_IS_NUMERIC(desc2.dsc_dtype))
			{
				dtype = dtype_double;
			}
			else
			{
				// mixed numeric and non-numeric:

				fb_assert(COULD_BE_DATE(desc1) || COULD_BE_DATE(desc2));

				// the MAX(dtype) rule doesn't apply with dtype_int64

				if (dtype_int64 == dtype1) {
					dtype1 = dtype_double;
				}
				if (dtype_int64 == dtype2) {
					dtype2 = dtype_double;
				}

				dtype = MAX(dtype1, dtype2);
			}

			switch (dtype)
			{
			case dtype_timestamp:
			case dtype_sql_date:
			case dtype_sql_time:
				node->nod_flags |= nod_date;

				fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_DATE(desc2.dsc_dtype));

				if ((DTYPE_IS_DATE(dtype1) || (dtype1 == dtype_unknown)) &&
					(DTYPE_IS_DATE(dtype2) || (dtype2 == dtype_unknown)))
				{
					if (node->nod_type == nod_subtract2)
					{
						// <any date> - <any date>

						/* Legal permutations are:
						   <timestamp> - <timestamp>
						   <timestamp> - <date>
						   <date> - <date>
						   <date> - <timestamp>
						   <time> - <time> */

						if (dtype1 == dtype_unknown) {
							dtype1 = dtype2;
						}
						else if (dtype2 == dtype_unknown) {
							dtype2 = dtype1;
						}
						if (dtype1 == dtype2) {
							dtype = dtype1;
						}
						else if ((dtype1 == dtype_timestamp) && (dtype2 == dtype_sql_date))
						{
							dtype = dtype_timestamp;
						}
						else if ((dtype2 == dtype_timestamp) && (dtype1 == dtype_sql_date))
						{
							dtype = dtype_timestamp;
						}
						else {
							ERR_post(Arg::Gds(isc_expression_eval_err));
						}

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
					else if (IS_DATE_AND_TIME(desc1, desc2))
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
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) || (node->nod_type == nod_add2))

				{
					// <date> +/- <non-date> || <non-date> + <date>
					desc->dsc_dtype = desc1.dsc_dtype;
					if (!DTYPE_IS_DATE(desc->dsc_dtype)) {
						desc->dsc_dtype = desc2.dsc_dtype;
					}
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
				{
					desc->dsc_scale = 0;
				}
				else {
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				}
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
				{
					desc->dsc_scale = 0;
				}
				else {
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				}
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
		}
		break;

	case nod_multiply:
		{
			DSC desc1, desc2;

			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
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
		}
		break;

	case nod_multiply2:
	case nod_divide2:
		{
			DSC desc1, desc2;

			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
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
		}
		break;

	case nod_class_exprnode_jrd:
		{
			ExprNode* exprNode = reinterpret_cast<ExprNode*>(node->nod_arg[0]);
			exprNode->getDesc(tdbb, csb, desc);
			return;
		}

	case nod_upcase:
	case nod_lowcase:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		if (desc->dsc_dtype > dtype_varying && desc->dsc_dtype != dtype_blob)
		{
			desc->dsc_length = DSC_convert_to_text_length(desc->dsc_dtype);
			desc->dsc_dtype = dtype_text;
			desc->dsc_ttype() = ttype_ascii;
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		return;

	case nod_dbkey:
		desc->dsc_dtype = dtype_dbkey;
		desc->dsc_length = type_lengths[dtype_dbkey];
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_rec_version:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype() = ttype_binary;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_current_time:
		desc->dsc_dtype = dtype_sql_time;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_current_timestamp:
		desc->dsc_dtype = dtype_timestamp;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_current_date:
		desc->dsc_dtype = dtype_sql_date;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_user_name:
	case nod_current_role:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype() = ttype_metadata;
		desc->dsc_length = USERNAME_LENGTH * METADATA_BYTES_PER_CHAR;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_internal_info:
		{
			jrd_nod* const arg_node = node->nod_arg[0];
			fb_assert(arg_node->nod_type == nod_literal);

			dsc arg_desc;
			CMP_get_desc(tdbb, csb, arg_node, &arg_desc);
			fb_assert(arg_desc.dsc_dtype == dtype_long);

			const internal_info_id id = *reinterpret_cast<internal_info_id*>(arg_desc.dsc_address);

			if (id == internal_sqlstate)
			{
				desc->makeText(FB_SQLSTATE_LENGTH, ttype_ascii);
			}
			else
			{
				desc->makeLong(0);
			}
		}
		return;

	case nod_extract:
		switch ((IPTR) node->nod_arg[e_extract_part])
		{
			case blr_extract_second:
				// QUADDATE - SECOND returns a float, or scaled!
				desc->makeLong(ISC_TIME_SECONDS_PRECISION_SCALE);
				break;

			case blr_extract_millisecond:
				desc->makeLong(0);
				break;

			default:
				desc->makeShort(0);
				break;
		}
		return;

	case nod_strlen:
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(ULONG);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_negate:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		node->nod_flags = node->nod_arg[0]->nod_flags & (nod_double | nod_quad);
		return;

	case nod_literal:
		*desc = ((Literal*) node)->lit_desc;

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
		return;

	case nod_cast:
		{
			const Format* format = (Format*) node->nod_arg[e_cast_fmt];
			*desc = format->fmt_desc[0];
			if ((desc->dsc_dtype <= dtype_any_text && !desc->dsc_length) ||
				(desc->dsc_dtype == dtype_varying && desc->dsc_length <= sizeof(USHORT)))
			{
			    dsc desc1;
				CMP_get_desc(tdbb, csb, node->nod_arg[e_cast_source], &desc1);
				desc->dsc_length = DSC_string_length(&desc1);
				if (desc->dsc_dtype == dtype_cstring) {
					desc->dsc_length++;
				}
				else if (desc->dsc_dtype == dtype_varying) {
					desc->dsc_length += sizeof(USHORT);
				}
			}
			return;
		}

	case nod_argument:
		{
			const jrd_nod* message = node->nod_arg[e_arg_message];
			const Format* format = (Format*) message->nod_arg[e_msg_format];
			*desc = format->fmt_desc[(IPTR) node->nod_arg[e_arg_number]];
			return;
		}

	case nod_substr:
		{
			DSC desc0, desc1, desc2, desc3;

			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc0);

			jrd_nod* offset_node = node->nod_arg[1];
			jrd_nod* decrement_node = NULL;
			if (offset_node->nod_type == nod_subtract2)
			{
				// This node is created by the DSQL layer, but the
				// system BLR code bypasses it and uses zero-based
				// string offsets instead
				decrement_node = offset_node->nod_arg[1];
				CMP_get_desc(tdbb, csb, decrement_node, &desc3);
				offset_node = offset_node->nod_arg[0];
			}
			CMP_get_desc(tdbb, csb, offset_node, &desc1);

			jrd_nod* length_node = node->nod_arg[2];
			CMP_get_desc(tdbb, csb, length_node, &desc2);

			DataTypeUtil(tdbb).makeSubstr(desc, &desc0, &desc1, &desc2);

			if (desc1.dsc_flags & DSC_null || desc2.dsc_flags & DSC_null)
			{
				desc->dsc_flags |= DSC_null;
			}
			else
			{
				if (offset_node->nod_type == nod_literal && desc1.dsc_dtype == dtype_long)
				{
					SLONG offset = MOV_get_long(&desc1, 0);
					if (decrement_node && decrement_node->nod_type == nod_literal &&
						desc3.dsc_dtype == dtype_long)
					{
						offset -= MOV_get_long(&desc3, 0);
					}
					// error() is a local routine in par.cpp, so we use plain ERR_post
					if (offset < 0)
					{
						ERR_post(Arg::Gds(isc_bad_substring_offset) << Arg::Num(offset + 1));
					}
				}
				if (length_node->nod_type == nod_literal && desc2.dsc_dtype == dtype_long)
				{
					const SLONG length = MOV_get_long(&desc2, 0);
					// error() is a local routine in par.cpp, so we use plain ERR_post
					if (length < 0)
					{
						ERR_post(Arg::Gds(isc_bad_substring_length) << Arg::Num(length));
					}
				}
			}

			return;
		}

	case nod_trim:
		CMP_get_desc(tdbb, csb, node->nod_arg[e_trim_value], desc);

		if (node->nod_arg[e_trim_characters])
		{
			DSC desc1;
			CMP_get_desc(tdbb, csb, node->nod_arg[e_trim_characters], &desc1);
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

		return;

	case nod_variable:
		{
			const jrd_nod* value = node->nod_arg[e_var_variable];
			*desc = *(DSC*) (value->nod_arg + e_dcl_desc);
			return;
		}

	case nod_value_if:
		CMP_get_desc(tdbb, csb,
			node->nod_arg[1]->nod_type != nod_null ?
				node->nod_arg[1] : node->nod_arg[2],
			desc);
		return;

	case nod_domain_validation:
		*desc = *(DSC*) (node->nod_arg + e_domval_desc);
		return;

	case nod_stmt_expr:
		CMP_get_desc(tdbb, csb, node->nod_arg[e_stmt_expr_expr], desc);
		return;

	case nod_derived_expr:
		CMP_get_desc(tdbb, csb, node->nod_arg[e_derived_expr_expr], desc);
		return;

	default:
		fb_assert(false);
		break;
	}

	if (dtype == dtype_quad)
		IBERROR(224);				// msg 224 quad word arithmetic not supported

	ERR_post(Arg::Gds(isc_datype_notsup));	// data type not supported for arithmetic
}


IndexLock* CMP_get_index_lock(thread_db* tdbb, jrd_rel* relation, USHORT id)
{
/**************************************
 *
 *	C M P _ g e t _ i n d e x _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Get index lock block for index.  If one doesn't exist,
 *	make one.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();

	DEV_BLKCHK(relation, type_rel);

	if (relation->rel_id < (USHORT) rel_MAX) {
		return NULL;
	}

	// for to find an existing block

	for (IndexLock* index = relation->rel_index_locks; index; index = index->idl_next)
	{
		if (index->idl_id == id) {
			return index;
		}
	}

	IndexLock* index = FB_NEW(*dbb->dbb_permanent) IndexLock();
	index->idl_next = relation->rel_index_locks;
	relation->rel_index_locks = index;
	index->idl_relation = relation;
	index->idl_id = id;
	index->idl_count = 0;

	Lock* lock = FB_NEW_RPT(*dbb->dbb_permanent, 0) Lock;
	index->idl_lock = lock;
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_dbb = dbb;
	lock->lck_key.lck_long = (relation->rel_id << 16) | id;
	lock->lck_length = sizeof(lock->lck_key.lck_long);
	lock->lck_type = LCK_idx_exist;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);

	return index;
}


ULONG CMP_impure(CompilerScratch* csb, ULONG size)
{
/**************************************
 *
 *	C M P _ i m p u r e
 *
 **************************************
 *
 * Functional description
 *	Allocate space (offset) in request.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);

	if (!csb)
	{
		return 0;
	}

	const ULONG offset = FB_ALIGN(csb->csb_impure, FB_ALIGNMENT);

	if (offset + size > JrdStatement::MAX_REQUEST_SIZE)
	{
		IBERROR(226);	// msg 226: request size limit exceeded
	}

	csb->csb_impure = offset + size;

	return offset;
}


void CMP_post_access(thread_db* tdbb,
					 CompilerScratch* csb,
					 const Firebird::MetaName& security_name,
					 SLONG view_id,
					 SecurityClass::flags_t mask,
					 SLONG type_name,
					 const Firebird::MetaName& name,
					 const Firebird::MetaName& r_name)
{
/**************************************
 *
 *	C M P _ p o s t _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *	Post access to security class to request.
 *      We append the new security class to the existing list of
 *      security classes for that request.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	// allow all access to internal requests

	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
		return;

	SET_TDBB(tdbb);

	AccessItem access(security_name, view_id, name, type_name, mask, r_name);

	size_t i;

	if (csb->csb_access.find(access, i))
	{
		return;
	}

	csb->csb_access.insert(i, access);
}


void CMP_post_resource(	ResourceList* rsc_ptr, void* obj, Resource::rsc_s type, USHORT id)
{
/**************************************
 *
 *	C M P _ p o s t _ r e s o u r c e
 *
 **************************************
 *
 * Functional description
 *	Post a resource usage to the compiler scratch block.
 *
 **************************************/
	// Initialize resource block
	Resource resource(type, id, NULL, NULL, NULL);
	switch (type)
	{
	case Resource::rsc_relation:
	case Resource::rsc_index:
		resource.rsc_rel = (jrd_rel*) obj;
		break;
	case Resource::rsc_procedure:
		resource.rsc_prc = (jrd_prc*) obj;
		break;
	case Resource::rsc_collation:
		resource.rsc_coll = (Collation*) obj;
		break;
	case Resource::rsc_function:
		resource.rsc_fun = (Function*) obj;
		break;
	default:
		BUGCHECK(220);			// msg 220 unknown resource
		break;
	}

	// Add it into list if not present already
	size_t pos;
	if (!rsc_ptr->find(resource, pos))
		rsc_ptr->insert(pos, resource);
}


void CMP_decrement_prc_use_count(thread_db* tdbb, jrd_prc* procedure)
{
/*********************************************
 *
 *	C M P _ d e c r e m e n t _ p r c _ u s e _ c o u n t
 *
 *********************************************
 *
 * Functional description
 *	decrement the procedure's use count
 *
 *********************************************/
	// Actually, it's possible for procedures to have intermixed dependencies, so
	// this routine can be called for the procedure which is being freed itself.
	// Hence we should just silently ignore such a situation.

	if (!procedure->prc_use_count)
		return;

	if (procedure->prc_int_use_count > 0)
		procedure->prc_int_use_count--;

	--procedure->prc_use_count;

#ifdef DEBUG_PROCS
	{
		string buffer;
		buffer.printf(
			"Called from CMP_decrement():\n\t Decrementing use count of %s\n",
			procedure->getName()->toString().c_str());
		JRD_print_procedure_info(tdbb, buffer.c_str());
	}
#endif

	// Call recursively if and only if the use count is zero AND the procedure
	// in dbb_procedures is different than this procedure.
	// The procedure will be different than in dbb_procedures only if it is a
	// floating copy, i.e. an old copy or a deleted procedure.
	if ((procedure->prc_use_count == 0) &&
		( (*tdbb->getDatabase()->dbb_procedures)[procedure->getId()] != procedure))
	{
		if (procedure->getStatement())
		{
			procedure->getStatement()->release(tdbb);
			procedure->setStatement(NULL);
		}
		procedure->prc_flags &= ~PRC_being_altered;
		MET_remove_procedure(tdbb, procedure->getId(), procedure);
	}
}


void CMP_release(thread_db* tdbb, jrd_req* request)
{
/**************************************
 *
 *	C M P _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a request's statement.
 *
 **************************************/
	DEV_BLKCHK(request, type_req);
	request->getStatement()->release(tdbb);
}


void CMP_shutdown_database(thread_db* tdbb)
{
/**************************************
 *
 *	C M P _ s h u t d o w n _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Release compile-time locks for database.
 *	Since this can be called at AST level, don't
 *	release any data structures.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	DEV_BLKCHK(dbb, type_dbb);

	// go through relations and indices and release
	// all existence locks that might have been taken

	vec<jrd_rel*>* rvector = dbb->dbb_relations;
	if (rvector)
	{
		vec<jrd_rel*>::iterator ptr, end;
		for (ptr = rvector->begin(), end = rvector->end(); ptr < end; ++ptr)
		{
			jrd_rel* relation = *ptr;
			if (relation)
			{
				if (relation->rel_existence_lock)
				{
					LCK_release(tdbb, relation->rel_existence_lock);
					relation->rel_flags |= REL_check_existence;
					relation->rel_use_count = 0;
				}
				if (relation->rel_partners_lock)
				{
					LCK_release(tdbb, relation->rel_partners_lock);
					relation->rel_flags |= REL_check_partners;
				}
				if (relation->rel_rescan_lock)
				{
					LCK_release(tdbb, relation->rel_rescan_lock);
					relation->rel_flags &= ~REL_scanned;
				}
				for (IndexLock* index = relation->rel_index_locks; index; index = index->idl_next)
				{
					if (index->idl_lock)
					{
						index->idl_count = 0;
						LCK_release(tdbb, index->idl_lock);
					}
				}
			}
		}
	}

	// release all procedure existence locks that might have been taken

	vec<jrd_prc*>* pvector = dbb->dbb_procedures;
	if (pvector)
	{
		vec<jrd_prc*>::iterator pptr, pend;
		for (pptr = pvector->begin(), pend = pvector->end(); pptr < pend; ++pptr)
		{
			jrd_prc* procedure = *pptr;
			if (procedure)
			{
				if (procedure->prc_existence_lock)
				{
					LCK_release(tdbb, procedure->prc_existence_lock);
					procedure->prc_flags |= PRC_check_existence;
					procedure->prc_use_count = 0;
				}
			}
		}
	}

	// release all function existence locks that might have been taken

	for (Function** iter = dbb->dbb_functions.begin(); iter < dbb->dbb_functions.end(); ++iter)
	{
		Function* const function = *iter;

		if (function)
		{
			function->releaseLocks(tdbb);
		}
	}

	// release collation existence locks

	dbb->releaseIntlObjects();
}


UCHAR* CMP_alloc_map(thread_db* tdbb, CompilerScratch* csb, USHORT stream)
{
/**************************************
 *
 *	C M P _ a l l o c _ m a p
 *
 **************************************
 *
 * Functional description
 *	Allocate and initialize stream map for view processing.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);

	SET_TDBB(tdbb);

	fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR maybe?
	UCHAR* const p = FB_NEW(*tdbb->getDefaultPool()) UCHAR[JrdStatement::MAP_LENGTH];
	memset(p, 0, JrdStatement::MAP_LENGTH);
	p[0] = (UCHAR) stream;
	csb->csb_rpt[stream].csb_map = p;

	return p;
}


static jrd_nod* catenate_nodes(thread_db* tdbb, NodeStack& stack)
{
/**************************************
 *
 *	c a t e n a t e _ n o d e s
 *
 **************************************
 *
 * Functional description
 *	Take a stack of nodes
 *	and turn them into a tree of concatenations.
 *
 **************************************/
	SET_TDBB(tdbb);

	jrd_nod* node1 = stack.pop();

	if (stack.isEmpty())
		return node1;

	ConcatenateNode* concatNode = FB_NEW(*tdbb->getDefaultPool()) ConcatenateNode(
		*tdbb->getDefaultPool());
	concatNode->arg1 = node1;
	concatNode->arg2 = catenate_nodes(tdbb, stack);

	jrd_nod* node = PAR_make_node(tdbb, 1);
	node->nod_type = nod_class_exprnode_jrd;
	node->nod_count = 0;
	node->nod_arg[0] = (jrd_nod*) concatNode;

	return node;
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
static jrd_nod* convertNeqAllToNotAny(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	fb_assert(node->nod_type == nod_ansi_all);

	fb_assert(node->nod_arg[e_any_rse]->nod_type == nod_class_recsrcnode_jrd);
	RseNode* outerRse = reinterpret_cast<RseNode*>(node->nod_arg[e_any_rse]->nod_arg[0]);	// nod_ansi_all rse

	if (!outerRse || outerRse->type != RseNode::TYPE || outerRse->rse_relations.getCount() != 1 ||
		!outerRse->rse_boolean || outerRse->rse_boolean->nod_type != nod_neq)
	{
		return NULL;
	}

	RseNode* innerRse = static_cast<RseNode*>(outerRse->rse_relations[0].getObject());	// user rse

	// If the rse is different than we expected, do nothing. Do nothing also if it uses FIRST or
	// SKIP, as we can't inject booleans there without changing the behavior.
	if (!innerRse || innerRse->type != RseNode::TYPE || innerRse->rse_first || innerRse->rse_skip)
		return NULL;

	jrd_nod* newNode = PAR_make_node(tdbb, 1);
	newNode->nod_type = nod_not;
	newNode->nod_count = 1;

	newNode->nod_arg[0] = PAR_make_node(tdbb, 2);
	newNode->nod_arg[0]->nod_type = nod_or;
	newNode->nod_arg[0]->nod_count = 2;

	newNode->nod_arg[0]->nod_arg[0] = PAR_make_node(tdbb, 2);
	newNode->nod_arg[0]->nod_arg[0]->nod_type = nod_and;
	newNode->nod_arg[0]->nod_arg[0]->nod_count = 2;

	newNode->nod_arg[0]->nod_arg[0]->nod_arg[0] = PAR_make_node(tdbb, 2);
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[0]->nod_type = nod_missing;
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[0]->nod_count = 1;
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[0]->nod_arg[0] = outerRse->rse_boolean->nod_arg[0];

	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1] = PAR_make_node(tdbb, e_any_length);
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1]->nod_type = nod_any;
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1]->nod_count = 1;
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse] = PAR_make_node(tdbb, 1);
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse]->nod_type = nod_class_recsrcnode_jrd;
	newNode->nod_arg[0]->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse]->nod_arg[0] = (jrd_nod*) innerRse;

	RseNode* newInnerRse = innerRse->clone();

	newNode->nod_arg[0]->nod_arg[1] = PAR_make_node(tdbb, e_any_length);
	newNode->nod_arg[0]->nod_arg[1]->nod_type = nod_any;
	newNode->nod_arg[0]->nod_arg[1]->nod_count = 1;
	newNode->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse] = PAR_make_node(tdbb, 1);
	newNode->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse]->nod_type = nod_class_recsrcnode_jrd;
	newNode->nod_arg[0]->nod_arg[1]->nod_arg[e_any_rse]->nod_arg[0] = (jrd_nod*) newInnerRse;

	jrd_nod* boolean = PAR_make_node(tdbb, 2);
	boolean->nod_type = nod_or;
	boolean->nod_count = 2;

	boolean->nod_arg[0] = PAR_make_node(tdbb, 1);
	boolean->nod_arg[0]->nod_type = nod_missing;
	boolean->nod_arg[0]->nod_count = 1;
	boolean->nod_arg[0]->nod_arg[0] = outerRse->rse_boolean->nod_arg[1];

	boolean->nod_arg[1] = outerRse->rse_boolean;
	boolean->nod_arg[1]->nod_type = nod_eql;

	// If there was a boolean on the stream, append (AND) the new one
	if (newInnerRse->rse_boolean)
	{
		jrd_nod* temp = PAR_make_node(tdbb, 2);
		temp->nod_type = nod_and;
		temp->nod_count = 2;
		temp->nod_arg[0] = newInnerRse->rse_boolean;
		temp->nod_arg[1] = boolean;
		boolean = temp;
	}

	newInnerRse->rse_boolean = boolean;

	return SubExprNodeCopier::copy(tdbb, csb, newNode);
}


USHORT NodeCopier::getFieldId(jrd_nod* input)
{
	return (USHORT)(IPTR) input->nod_arg[e_fld_id];
}

// Copy an expression tree remapping field streams. If the map isn't present, don't remap.
jrd_nod* NodeCopier::copy(thread_db* tdbb, jrd_nod* input)
{
	jrd_nod* node;
	USHORT stream;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(input, type_nod);

	if (!input)
		return NULL;

	// special case interesting nodes

	USHORT args = input->nod_count;

	switch (input->nod_type)
	{
	case nod_class_exprnode_jrd:
		{
			ExprNode* exprNode = reinterpret_cast<ExprNode*>(input->nod_arg[0]);
			ExprNode* copy = exprNode->copy(tdbb, *this);

			if (copy != exprNode)
			{
				node = PAR_make_node(tdbb, 1);
				node->nod_type = input->nod_type;
				node->nod_count = input->nod_count;
				node->nod_arg[0] = (jrd_nod*) copy;
			}
		}
		return node;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		args = e_any_length;
		break;

	case nod_for:
		///args = e_for_length;
		fb_assert(false);
		break;

	case nod_argument:
		if (remapArgument())
			return input;
		node = PAR_make_node(tdbb, e_arg_length);
		node->nod_count = input->nod_count;
		node->nod_flags = input->nod_flags;
		node->nod_type = input->nod_type;
		node->nod_arg[e_arg_number] = input->nod_arg[e_arg_number];
		// dimitr:	IMPORTANT!!!
		//			nod_message copying must be done in the only place
		//			(the nod_procedure code below). Hence we don't call
		//			copy() here to keep argument->nod_arg[e_arg_message]
		//			and procedure->nod_arg[e_prc_in_msg] in sync. The
		//			message is passed to copy() as a parameter. If the
		//			passed message is NULL, it means nod_argument is
		//			cloned outside nod_procedure (e.g. in the optimizer)
		//			and we must keep the input message.
		// ASF: We should only use "message" if its number matches the number
		// in nod_argument. If it don't, it may be an input parameter cloned
		// in convertNeqAllToNotAny - see CORE-3094.

		if (message &&
			(IPTR) message->nod_arg[e_msg_number] ==
				(IPTR) input->nod_arg[e_arg_message]->nod_arg[e_msg_number])
		{
			node->nod_arg[e_arg_message] = message;
		}
		else
			node->nod_arg[e_arg_message] = input->nod_arg[e_arg_message];

		node->nod_arg[e_arg_flag] = copy(tdbb, input->nod_arg[e_arg_flag]);
		node->nod_arg[e_arg_indicator] = copy(tdbb, input->nod_arg[e_arg_indicator]);
		return node;

	case nod_assignment:
		args = e_asgn_length;
		break;

	case nod_erase:
		args = e_erase_length;
		break;

	case nod_modify:
		args = e_mod_length;
		break;

	case nod_variable:
		if (csb->csb_remap_variable != 0)
		{
			node = PAR_make_node(tdbb, e_var_length);
			node->nod_type = input->nod_type;
			node->nod_count = input->nod_count;

			USHORT n = csb->csb_remap_variable + (USHORT)(IPTR) input->nod_arg[e_var_id];
			node->nod_arg[e_var_id] = (jrd_nod*)(IPTR) n;
			node->nod_arg[e_var_variable] = input->nod_arg[e_var_variable];
			node->nod_arg[e_var_info] = input->nod_arg[e_var_info];
			return node;
		}
		return input;

	case nod_literal:
		return input;

	case nod_field:
		{
			USHORT fldId = getFieldId(input);

			stream = (USHORT)(IPTR) input->nod_arg[e_fld_stream];
			fldId = remapField(stream, fldId);

			if (remap)
			{
#ifdef CMP_DEBUG
				csb->dump("remap nod_field: %d -> %d\n", stream, remap[stream]);
#endif
				stream = remap[stream];
			}

			jrd_nod* temp_node = PAR_gen_field(tdbb, stream, fldId);
			if (input->nod_type == nod_field && input->nod_arg[e_fld_default_value])
				temp_node->nod_arg[e_fld_default_value] = input->nod_arg[e_fld_default_value];

			return temp_node;
		}

	case nod_derived_expr:
	{
		node = PAR_make_node(tdbb, e_derived_expr_length);
		node->nod_count = e_derived_expr_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_derived_expr_expr] = copy(tdbb, input->nod_arg[e_derived_expr_expr]);

		if (remap)
		{
			const UCHAR streamCount = (UCHAR)(IPTR) input->nod_arg[e_derived_expr_stream_count];
			const USHORT* oldStreamList = (USHORT*) input->nod_arg[e_derived_expr_stream_list];
			USHORT* newStreamList = FB_NEW(*tdbb->getDefaultPool()) USHORT[streamCount];

#ifdef CMP_DEBUG
			csb->dump("remap nod_derived_expr:\n");
			for (UCHAR i = 0; i < streamCount; ++i)
				csb->dump("\t%d: %d -> %d\n", i, oldStreamList[i], remap[oldStreamList[i]]);
#endif

			for (UCHAR i = 0; i < streamCount; ++i)
				newStreamList[i] = remap[oldStreamList[i]];

			node->nod_arg[e_derived_expr_stream_list] = (jrd_nod*) newStreamList;
		}
		else
			node->nod_arg[e_derived_expr_stream_list] = input->nod_arg[e_derived_expr_stream_list];

		node->nod_arg[e_derived_expr_stream_count] = input->nod_arg[e_derived_expr_stream_count];
		return node;
	}

	case nod_current_time:
	case nod_current_timestamp:
		fb_assert(e_current_time_length == e_current_timestamp_length);
		node = PAR_make_node(tdbb, e_current_time_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[0] = input->nod_arg[0];
		return (node);

	case nod_gen_id:
	case nod_gen_id2:			// 20001013 PJPG
		node = PAR_make_node(tdbb, e_gen_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_gen_value] = copy(tdbb, input->nod_arg[e_gen_value]);
		node->nod_arg[e_gen_id] = input->nod_arg[e_gen_id];
		return (node);

	case nod_cast:
		node = PAR_make_node(tdbb, e_cast_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_cast_source] = copy(tdbb, input->nod_arg[e_cast_source]);
		node->nod_arg[e_cast_fmt] = input->nod_arg[e_cast_fmt];
		return (node);

	case nod_extract:
		node = PAR_make_node(tdbb, e_extract_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_extract_value] = copy(tdbb, input->nod_arg[e_extract_value]);
		node->nod_arg[e_extract_part] = input->nod_arg[e_extract_part];
		return (node);

	case nod_strlen:
		node = PAR_make_node(tdbb, e_strlen_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_strlen_value] = copy(tdbb, input->nod_arg[e_strlen_value]);
		node->nod_arg[e_strlen_type] = input->nod_arg[e_strlen_type];
		return (node);

	case nod_trim:
		node = PAR_make_node(tdbb, e_trim_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_trim_characters] = copy(tdbb, input->nod_arg[e_trim_characters]);
		node->nod_arg[e_trim_value] = copy(tdbb, input->nod_arg[e_trim_value]);
		node->nod_arg[e_trim_specification] = input->nod_arg[e_trim_specification];
		return (node);

	case nod_count:
	case nod_max:
	case nod_min:
	case nod_total:
	case nod_average:
	case nod_from:
		args = e_stat_length;
		break;

	case nod_class_recsrcnode_jrd:
		node = PAR_make_node(tdbb, 1);
		node->nod_type = input->nod_type;
		node->nod_count = input->nod_count;
		node->nod_arg[0] = reinterpret_cast<jrd_nod*>(
			reinterpret_cast<RecordSourceNode*>(input->nod_arg[0])->copy(tdbb, *this));
		return node;

	case nod_message:
		node = PAR_make_node(tdbb, e_msg_length);
		node->nod_type = input->nod_type;
		node->nod_count = input->nod_count;
		node->nod_arg[e_msg_number] = input->nod_arg[e_msg_number];
		node->nod_arg[e_msg_format] = input->nod_arg[e_msg_format];
		node->nod_arg[e_msg_impure_flags] = input->nod_arg[e_msg_impure_flags];
		return node;

	case nod_sort:
		args *= 3;
		break;

	case nod_domain_validation:
		node = PAR_make_node(tdbb, e_domval_length);
		node->nod_type = nod_domain_validation;
		node->nod_count = 0;
		*(dsc*) (node->nod_arg + e_domval_desc) = *(dsc*) (input->nod_arg + e_domval_desc);
		return node;

	case nod_dbkey:
		stream = (USHORT)(IPTR) input->nod_arg[0];
		if (remap)
		{
#ifdef CMP_DEBUG
			csb->dump("remap nod_dbkey: %d -> %d\n", stream, remap[stream]);
#endif
			stream = remap[stream];
		}

		node = PAR_make_node(tdbb, 1);
		node->nod_type = input->nod_type;
		node->nod_count = 0;
		node->nod_arg[0] = (jrd_nod*)(IPTR) stream;
		return node;

	case nod_dcl_variable:
		if (csb->csb_remap_variable != 0)
		{
			node = PAR_make_node(tdbb, e_dcl_length);
			node->nod_type = input->nod_type;
			node->nod_count = input->nod_count;

			const USHORT n = csb->csb_remap_variable + (USHORT)(IPTR) input->nod_arg[e_dcl_id];
			node->nod_arg[e_dcl_id] = (jrd_nod*)(IPTR) n;
			*(dsc*) (node->nod_arg + e_dcl_desc) = *(dsc*) (input->nod_arg + e_dcl_desc);

			csb->csb_variables =
				vec<jrd_nod*>::newVector(*tdbb->getDefaultPool(), csb->csb_variables, n);

			return node;
		}
		break;

	case nod_dcl_cursor:
		node = PAR_make_node(tdbb, e_dcl_cur_length);
		node->nod_count = input->nod_count;
		node->nod_flags = input->nod_flags;
		node->nod_type = input->nod_type;
		node->nod_arg[e_dcl_cur_rse] = copy(tdbb, input->nod_arg[e_dcl_cur_rse]);
		node->nod_arg[e_dcl_cur_refs] = copy(tdbb, input->nod_arg[e_dcl_cur_refs]);
		node->nod_arg[e_dcl_cur_number] = input->nod_arg[e_dcl_cur_number];
		break;

	case nod_cursor_stmt:
		node = PAR_make_node(tdbb, e_cursor_stmt_length);
		node->nod_count = input->nod_count;
		node->nod_flags = input->nod_flags;
		node->nod_type = input->nod_type;
		node->nod_arg[e_cursor_stmt_op] = input->nod_arg[e_cursor_stmt_op];
		node->nod_arg[e_cursor_stmt_number] = input->nod_arg[e_cursor_stmt_number];
		node->nod_arg[e_cursor_stmt_scroll_op] = input->nod_arg[e_cursor_stmt_scroll_op];
		node->nod_arg[e_cursor_stmt_scroll_val] = copy(tdbb, input->nod_arg[e_cursor_stmt_scroll_val]);
		node->nod_arg[e_cursor_stmt_into] = copy(tdbb, input->nod_arg[e_cursor_stmt_into]);
		break;

	default:
		break;
	}

	// fall thru on generic nodes

	node = PAR_make_node(tdbb, args);
	node->nod_count = input->nod_count;
	node->nod_type = input->nod_type;
	node->nod_flags = input->nod_flags;

	jrd_nod** arg1 = input->nod_arg;
	jrd_nod** arg2 = node->nod_arg;

	for (const jrd_nod* const* const end = arg1 + input->nod_count; arg1 < end; arg1++, arg2++)
	{
		if (*arg1)
			*arg2 = copy(tdbb, *arg1);
	}

	// finish off sort

	if (input->nod_type == nod_sort)
	{
		for (jrd_nod** end = arg1 + input->nod_count * 2; arg1 < end; arg1++, arg2++)
			*arg2 = *arg1;
	}

	return node;
}


static void expand_view_nodes(thread_db* tdbb,
							  CompilerScratch* csb,
							  USHORT stream,
							  NodeStack& stack,
							  nod_t type,
							  bool allStreams)
{
/**************************************
 *
 *	e x p a n d _ v i e w _ n o d e s
 *
 **************************************
 *
 * Functional description
 *	Expand dbkey for view.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	// if the stream's dbkey should be ignored, do so

	if (!allStreams && (csb->csb_rpt[stream].csb_flags & csb_no_dbkey))
		return;

	// if the stream references a view, follow map
	const UCHAR* map = csb->csb_rpt[stream].csb_map;
	if (map)
	{
		++map;
		while (*map)
			expand_view_nodes(tdbb, csb, *map++, stack, type, allStreams);
		return;
	}

	// relation is primitive - make dbkey node

	if (allStreams || csb->csb_rpt[stream].csb_relation)
	{
		jrd_nod* node = PAR_make_node(tdbb, 1);
		node->nod_count = 0;
		node->nod_type = type;
		node->nod_arg[0] = (jrd_nod*)(IPTR) stream;
		stack.push(node);
	}
}


static jrd_nod* make_defaults(thread_db* tdbb, CompilerScratch* csb, USHORT stream, jrd_nod* statement)
{
/**************************************
 *
 *	m a k e _ d e f a u l t s
 *
 **************************************
 *
 * Functional description
 *	Build an default value assignments.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(statement, type_nod);

	jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return statement;

	UCHAR local_map[JrdStatement::MAP_LENGTH];
	UCHAR* map = csb->csb_rpt[stream].csb_map;
	if (!map)
	{
		map = local_map;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR relevant, too?
		map[0] = (UCHAR) stream;
		map[1] = 1;
		map[2] = 2;
	}

	NodeStack stack;

	USHORT field_id = 0;
	vec<jrd_fld*>::iterator ptr1 = vector->begin();
	for (const vec<jrd_fld*>::const_iterator end = vector->end(); ptr1 < end; ++ptr1, ++field_id)
	{
		jrd_nod* value;

		if (!*ptr1 || !((*ptr1)->fld_generator_name.hasData() || (value = (*ptr1)->fld_default_value)))
			continue;

		fb_assert(statement->nod_type == nod_list);
		if (statement->nod_type == nod_list)
		{
			bool inList = false;

			for (unsigned i = 0; i < statement->nod_count; ++i)
			{
				const jrd_nod* assign = statement->nod_arg[i];

				fb_assert(assign->nod_type == nod_assignment);
				if (assign->nod_type == nod_assignment)
				{
					const jrd_nod* to = assign->nod_arg[e_asgn_to];

					fb_assert(to->nod_type == nod_field);
					if (to->nod_type == nod_field &&
						(USHORT)(IPTR) to->nod_arg[e_fld_stream] == stream &&
						(USHORT)(IPTR) to->nod_arg[e_fld_id] == field_id)
					{
						inList = true;
						break;
					}
				}
			}

			if (inList)
				continue;

			jrd_nod* node = PAR_make_node(tdbb, e_asgn_length);
			node->nod_type = nod_assignment;
			node->nod_arg[e_asgn_to] = PAR_gen_field(tdbb, stream, field_id);
			stack.push(node);

			if ((*ptr1)->fld_generator_name.hasData())
			{
				// Make a gen_id(<generator name>, 1) expression.

				jrd_nod* genNode = PAR_make_node(tdbb, e_gen_length);
				genNode->nod_type = nod_gen_id;
				genNode->nod_count = 1;

				const SLONG tmp = MET_lookup_generator(tdbb, (*ptr1)->fld_generator_name.c_str());
				genNode->nod_arg[e_gen_id] = (jrd_nod*)(IPTR) tmp;

				const int count = lit_delta + (sizeof(SLONG) + sizeof(jrd_nod*) - 1) / sizeof(jrd_nod*);
				jrd_nod* literalNode = genNode->nod_arg[e_gen_value] = PAR_make_node(tdbb, count);
				literalNode->nod_type = nod_literal;
				literalNode->nod_count = 0;
				Literal* literal = (Literal*) literalNode;
				literal->lit_desc.makeLong(0, (SLONG*) literal->lit_data);
				*(SLONG*) literal->lit_data = 1;

				node->nod_arg[e_asgn_from] = genNode;
			}
			else //if (value)
			{
				// Clone the field default value.
				node->nod_arg[e_asgn_from] =
					RemapFieldNodeCopier(csb, map, field_id).copy(tdbb, value);
			}
		}
	}

	if (stack.isEmpty())
		return statement;

	// We have some default - add the original statement and make a list out of the whole mess.

	stack.push(statement);

	return PAR_make_list(tdbb, stack);
}


static jrd_nod* make_validation(thread_db* tdbb, CompilerScratch* csb, USHORT stream)
{
/**************************************
 *
 *	m a k e _ v a l i d a t i o n
 *
 **************************************
 *
 * Functional description
 *	Build a validation list for a relation, if appropriate.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

	vec<jrd_fld*>* vector = relation->rel_fields;
	if (!vector)
		return NULL;

	UCHAR local_map[JrdStatement::MAP_LENGTH];
	UCHAR* map = csb->csb_rpt[stream].csb_map;
	if (!map)
	{
		map = local_map;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR still relevant for the bitmap?
		map[0] = (UCHAR) stream;
	}

	NodeStack stack;

	USHORT field_id = 0;
	vec<jrd_fld*>::iterator ptr1 = vector->begin();
	for (const vec<jrd_fld*>::const_iterator end = vector->end(); ptr1 < end; ++ptr1, ++field_id)
	{
		jrd_nod* validation;
		if (*ptr1 && (validation = (*ptr1)->fld_validation))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			jrd_nod* node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_boolean] =
				RemapFieldNodeCopier(csb, map, field_id).copy(tdbb, validation);
			node->nod_arg[e_val_value] = PAR_gen_field(tdbb, stream, field_id);
			stack.push(node);
		}

		if (*ptr1 && (validation = (*ptr1)->fld_not_null))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			jrd_nod* node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_boolean] =
				RemapFieldNodeCopier(csb, map, field_id).copy(tdbb, validation);
			node->nod_arg[e_val_value] = PAR_gen_field(tdbb, stream, field_id);
			stack.push(node);
		}
	}

	if (stack.isEmpty())
		return NULL;

	return PAR_make_list(tdbb, stack);
}


// Look at all RseNode's which are lower in scope than the RseNode which this field is
// referencing, and mark them as variant - the rule is that if a field from one RseNode is
// referenced within the scope of another RseNode, the inner RseNode can't be invariant.
// This won't optimize all cases, but it is the simplest operating assumption for now.
static void mark_variant(CompilerScratch* csb, USHORT stream)
{
	if (csb->csb_current_nodes.isEmpty())
		return;

	for (LegacyNodeOrRseNode* node = csb->csb_current_nodes.end() - 1;
		 node != csb->csb_current_nodes.begin(); --node)
	{
		if (node->rseNode)
		{
			if (node->rseNode->containsStream(stream))
				break;
			node->rseNode->flags |= RseNode::FLAG_VARIANT;
		}
		else
			node->legacyNode->nod_flags &= ~nod_invariant;
	}
}


jrd_nod* CMP_pass1(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	C M P _ p a s s 1
 *
 **************************************
 *
 * Functional description
 *	Merge missing values, computed values, validation expressions,
 *	and views into a parsed request.
 *
 * The csb->csb_validate_expr becomes true if an ancestor of the
 * current node (the one being passed in) in the parse tree has nod_type
 * == nod_validate. "ancestor" does not include the current node
 * being passed in as an argument.
 * If we are in a "validate subtree" (as determined by the
 * csb->csb_validate_expr), we must not post update access to the fields involved
 * in the validation clause. (see the call for CMP_post_access in this
 * function.)
 *
 **************************************/
	jrd_nod* sub;
	jrd_nod** ptr;
	USHORT stream;
	CompilerScratch::csb_repeat* tail;
	jrd_prc* procedure;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);
	DEV_BLKCHK(view, type_rel);

	if (!node)
		return node;

	AutoSetRestore<bool> autoValidateExpr(&csb->csb_validate_expr,
		csb->csb_validate_expr || node->nod_type == nod_validate);

	jrd_rel* const view = csb->csb_view;

	// if there is processing to be done before sub expressions, do it here

	switch (node->nod_type)
	{
	case nod_like:
	case nod_similar:
		ptr = node->nod_arg;
		ptr[0] = CMP_pass1(tdbb, csb, ptr[0]);
		// We need to take care of invariantness of like/similar pattern expression to be
		// able to pre-compile its pattern
		node->nod_flags |= nod_invariant;
		csb->csb_current_nodes.push(node);
		ptr[1] = CMP_pass1(tdbb, csb, ptr[1]);
		if (node->nod_count == 3)
		{
			// escape symbol also needs to be taken care of
			ptr[2] = CMP_pass1(tdbb, csb, ptr[2]);
		}
		csb->csb_current_nodes.pop();

		// If there is no top-level RSE present and patterns are not constant,
		// unmark node as invariant because it may be dependent on data or variables.
		// See the same for nod_contains and nod_starts below.
		if ((node->nod_flags & nod_invariant) && (ptr[1]->nod_type != nod_literal ||
			(node->nod_count == 3 && ptr[2]->nod_type != nod_literal)))
		{
			LegacyNodeOrRseNode* ctx_node, *end;
			for (ctx_node = csb->csb_current_nodes.begin(), end = csb->csb_current_nodes.end();
				 ctx_node != end; ++ctx_node)
			{
				if (ctx_node->rseNode)
					break;
			}

			if (ctx_node >= end)
				node->nod_flags &= ~nod_invariant;
		}
		return node;

	case nod_contains:
	case nod_starts:
		ptr = node->nod_arg;
		ptr[0] = CMP_pass1(tdbb, csb, ptr[0]);
		// We need to take care of invariantness of contains and starts
		// expression to be able to pre-compile it for searching
		node->nod_flags |= nod_invariant;
		csb->csb_current_nodes.push(node);
		ptr[1] = CMP_pass1(tdbb, csb, ptr[1]);
		csb->csb_current_nodes.pop();

		// If there is no top-level RSE present and patterns are not constant,
		// unmark node as invariant because it may be dependent on data or variables.
		// See the same for nod_like above.
		if ((node->nod_flags & nod_invariant) && (ptr[1]->nod_type != nod_literal))
		{
			LegacyNodeOrRseNode* ctx_node, *end;
			for (ctx_node = csb->csb_current_nodes.begin(), end = csb->csb_current_nodes.end();
				 ctx_node != end; ++ctx_node)
			{
				if (ctx_node->rseNode)
					break;
			}

			if (ctx_node >= end)
				node->nod_flags &= ~nod_invariant;
		}
		return node;

	case nod_variable:
		{
			const USHORT n = (USHORT)(IPTR) node->nod_arg[e_var_id];
			vec<jrd_nod*>* vector = csb->csb_variables;
			if (!vector || n >= vector->count() || !(node->nod_arg[e_var_variable] = (*vector)[n]))
			{
				PAR_syntax_error(csb, "variable identifier");
			}
		}
		break;

	case nod_init_variable:
		{
			const USHORT n = (USHORT)(IPTR) node->nod_arg[e_init_var_id];
			vec<jrd_nod*>* vector = csb->csb_variables;
			if (!vector || n >= vector->count() || !(node->nod_arg[e_var_variable] = (*vector)[n]))
			{
				PAR_syntax_error(csb, "variable identifier");
			}
		}
		break;

	case nod_argument:
		break;

	case nod_cast:
		{
			dsc desc;
			CMP_get_desc(tdbb, csb, node, &desc);

			const USHORT ttype = INTL_TEXT_TYPE(desc);

			// Are we using a collation?
			if (TTYPE_TO_COLLATION(ttype) != 0)
			{
				CMP_post_resource(&csb->csb_resources, INTL_texttype_lookup(tdbb, ttype),
								  Resource::rsc_collation, ttype);
			}

			break;
		}

	case nod_field:
		{
			stream = (USHORT)(IPTR) node->nod_arg[e_fld_stream];

			mark_variant(csb, stream);

			jrd_fld* field;
			tail = &csb->csb_rpt[stream];
			jrd_rel* relation = tail->csb_relation;
			if (!relation ||
				!(field = MET_get_field(relation, (USHORT)(IPTR) node->nod_arg[e_fld_id])))
			{
				break;
			}

			dsc desc;
			CMP_get_desc(tdbb, csb, node, &desc);

			const USHORT ttype = INTL_TEXT_TYPE(desc);

			// Are we using a collation?
			if (TTYPE_TO_COLLATION(ttype) != 0)
			{
				Jrd::Collation* collation = NULL;

				try
				{
					ThreadStatusGuard local_status(tdbb);

					collation = INTL_texttype_lookup(tdbb, ttype);
				}
				catch (Firebird::Exception&)
				{
					// ASF: Swallow the exception if we fail to load the collation here.
					// This allows us to backup databases when the collation isn't available.
					if (!(tdbb->getAttachment()->att_flags & ATT_gbak_attachment))
						throw;
				}

				if (collation)
				{
					CMP_post_resource(&csb->csb_resources, collation, Resource::rsc_collation, ttype);
				}
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

			const SLONG viewId = (tail->csb_view) ? tail->csb_view->rel_id :
										(view ? view->rel_id : 0);

			if (tail->csb_flags & csb_modify)
			{
				if (!csb->csb_validate_expr)
				{
					CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
									SCL_sql_update, SCL_object_table,
									relation->rel_name);
					CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
									SCL_sql_update, SCL_object_column,
									field->fld_name, relation->rel_name);
				}
			}
			else if (tail->csb_flags & csb_erase)
			{
				CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
								SCL_sql_delete, SCL_object_table,
								relation->rel_name);
			}
			else if (tail->csb_flags & csb_store)
			{
				CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
								SCL_sql_insert, SCL_object_table,
								relation->rel_name);
				CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
								SCL_sql_insert, SCL_object_column,
								field->fld_name, relation->rel_name);
			}
			else
			{
				CMP_post_access(tdbb, csb, relation->rel_security_name, viewId,
								SCL_read, SCL_object_table, relation->rel_name);
				CMP_post_access(tdbb, csb, field->fld_security_name, viewId,
								SCL_read, SCL_object_column, field->fld_name, relation->rel_name);
			}

			if (!(sub = field->fld_computation) && !(sub = field->fld_source))
			{

				if (!relation->rel_view_rse)
					break;

				ERR_post(Arg::Gds(isc_no_field_access) << Arg::Str(field->fld_name) <<
														  Arg::Str(relation->rel_name));
				// Msg 364 "cannot access column %s in view %s"
			}

			// The previous test below is an apparent temporary fix
			// put in by Root & Harrison in Summer/Fall 1991.
			// Old Code:
			// if (tail->csb_flags & (csb_view_update | csb_trigger))
			//   break;
			// If the field is a computed field - we'll go on and make
			// the substitution.
			// Comment 1994-August-08 David Schnepper

			if (tail->csb_flags & (csb_view_update | csb_trigger))
			{
				// dimitr:	added an extra check for views, because we don't
				//			want their old/new contexts to be substituted
				if (relation->rel_view_rse || !field->fld_computation)
					break;
			}

			UCHAR local_map[JrdStatement::MAP_LENGTH];
			UCHAR* map = tail->csb_map;
			if (!map)
			{
				map = local_map;
				fb_assert(stream + 2 <= MAX_STREAMS);
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
				if (sub->nod_type != nod_field &&
					sub->nod_type != nod_map &&
					sub->nod_type != nod_dbkey)
				{
					NodeStack stack;
					expand_view_nodes(tdbb, csb, stream, stack, nod_dbkey, false);
					const UCHAR streamCount = (UCHAR) stack.getCount();

					if (streamCount)
					{
						jrd_nod* new_node = PAR_make_node(tdbb, e_derived_expr_length);
						new_node->nod_type = nod_derived_expr;
						new_node->nod_count = e_derived_expr_count;
						new_node->nod_arg[e_derived_expr_expr] = sub;

						USHORT* streamList = FB_NEW(*tdbb->getDefaultPool()) USHORT[streamCount];

						new_node->nod_arg[e_derived_expr_stream_list] = (jrd_nod*) streamList;
						new_node->nod_arg[e_derived_expr_stream_count] = (jrd_nod*)(IPTR) streamCount;

						for (NodeStack::iterator i(stack); i.hasData(); ++i)
							*streamList++ = (USHORT)(IPTR) i.object()->nod_arg[0];

						sub = new_node;
					}
				}

				return CMP_pass1(tdbb, csb, sub);	// note: scope of AutoSetRestore
			}

			return CMP_pass1(tdbb, csb, sub);
		}

	case nod_derived_expr:
		{
			UCHAR streamCount = (UCHAR)(IPTR) node->nod_arg[e_derived_expr_stream_count];
			USHORT* streamList = (USHORT*) node->nod_arg[e_derived_expr_stream_list];
			NodeStack stack;

#ifdef CMP_DEBUG
			csb->dump("expand nod_derived_expr:");
			for (UCHAR i = 0; i < streamCount; ++i)
				csb->dump(" %d", streamList[i]);
			csb->dump("\n");
#endif

			for (UCHAR i = 0; i < streamCount; ++i)
			{
				mark_variant(csb, streamList[i]);
				expand_view_nodes(tdbb, csb, streamList[i], stack, nod_dbkey, true);
			}

			streamCount = (UCHAR) stack.getCount();
			streamList = FB_NEW(*tdbb->getDefaultPool()) USHORT[streamCount];

			node->nod_arg[e_derived_expr_stream_list] = (jrd_nod*) streamList;
			node->nod_arg[e_derived_expr_stream_count] = (jrd_nod*)(IPTR) streamCount;

#ifdef CMP_DEBUG
			for (NodeStack::iterator i(stack); i.hasData(); ++i)
				csb->dump(" %d", (USHORT)(IPTR) i.object()->nod_arg[0]);
			csb->dump("\n");
#endif

			for (NodeStack::iterator i(stack); i.hasData(); ++i)
				*streamList++ = (USHORT)(IPTR) i.object()->nod_arg[0];
		}
		break;

	case nod_assignment:
		{
			sub = node->nod_arg[e_asgn_from];
			if (sub->nod_type == nod_field)
			{
				stream = (USHORT)(IPTR) sub->nod_arg[e_fld_stream];
				jrd_fld* field = MET_get_field(csb->csb_rpt[stream].csb_relation,
									 		   (USHORT)(IPTR) sub->nod_arg[e_fld_id]);
				if (field)
					node->nod_arg[e_asgn_missing2] = field->fld_missing_value;
			}

			sub = node->nod_arg[e_asgn_to];
			if (sub->nod_type != nod_field)
				break;
			stream = (USHORT)(IPTR) sub->nod_arg[e_fld_stream];
			tail = &csb->csb_rpt[stream];
			jrd_fld* field = MET_get_field(tail->csb_relation, (USHORT)(IPTR) sub->nod_arg[e_fld_id]);
			if (!field) {
				break;
			}
			if (field->fld_missing_value)
			{
				node->nod_arg[e_asgn_missing] = field->fld_missing_value;
				node->nod_count = 3;
			}
		}
		break;

	case nod_modify:
		pass1_modify(tdbb, csb, node);
		break;

	case nod_erase:
		pass1_erase(tdbb, csb, node);
		break;

	case nod_exec_proc:
		procedure = (jrd_prc*) node->nod_arg[e_esp_procedure];
		// Post access to procedure
		CMP_post_procedure_access(tdbb, csb, procedure);
		CMP_post_resource(&csb->csb_resources, procedure,
						  Resource::rsc_procedure, procedure->getId());
		break;

	case nod_store:
		if (pass1_store(tdbb, csb, node))
		{
			fb_assert(node->nod_arg[e_sto_relation]->nod_type == nod_class_recsrcnode_jrd);
			RelationSourceNode* recSource = reinterpret_cast<RelationSourceNode*>(
				node->nod_arg[e_sto_relation]->nod_arg[0]);
			fb_assert(recSource->type == RelationSourceNode::TYPE);

			stream = recSource->getStream();
			node->nod_arg[e_sto_statement] =
				make_defaults(tdbb, csb, stream, node->nod_arg[e_sto_statement]);
		}
		break;

	case nod_cursor_stmt:
		node->nod_arg[e_cursor_stmt_scroll_val] = CMP_pass1(tdbb, csb, node->nod_arg[e_cursor_stmt_scroll_val]);
		node->nod_arg[e_cursor_stmt_into] = CMP_pass1(tdbb, csb, node->nod_arg[e_cursor_stmt_into]);
		break;

	case nod_max:
	case nod_min:
	case nod_average:
	case nod_from:
	case nod_count:
	case nod_total:
		{
			fb_assert(node->nod_arg[e_stat_rse]->nod_type == nod_class_recsrcnode_jrd);
			RseNode* rseNode = reinterpret_cast<RseNode*>(node->nod_arg[e_stat_rse]->nod_arg[0]);
			fb_assert(rseNode->type == RseNode::TYPE);
			rseNode->ignoreDbKey(tdbb, csb, view);
			rseNode->pass1(tdbb, csb, csb->csb_view);
			csb->csb_current_nodes.push(node->nod_arg[e_stat_rse]);
			node->nod_arg[e_stat_value] = CMP_pass1(tdbb, csb, node->nod_arg[e_stat_value]);
			node->nod_arg[e_stat_default] = CMP_pass1(tdbb, csb, node->nod_arg[e_stat_default]);
			csb->csb_current_nodes.pop();
		}
		return node;

	case nod_class_recsrcnode_jrd:
		reinterpret_cast<RecordSourceNode*>(node->nod_arg[0])->pass1(tdbb, csb, view);
		break;

	case nod_gen_id:
	case nod_gen_id2:
		node->nod_arg[e_gen_value] = CMP_pass1(tdbb, csb, node->nod_arg[e_gen_value]);
		return node;

	case nod_rec_version:
	case nod_dbkey:
		{
			const nod_t type = node->nod_type;
			stream = (USHORT)(IPTR) node->nod_arg[0];

			mark_variant(csb, stream);

			if (!csb->csb_rpt[stream].csb_map)
				return node;
			NodeStack stack;
			expand_view_nodes(tdbb, csb, stream, stack, type, false);

#ifdef CMP_DEBUG
			csb->dump("expand nod_dbkey: %d\n", stream);
#endif

			if (stack.hasData())
			{
				const size_t stackCount = stack.getCount();

				// If that is a DB_KEY of a view, it's possible (in case of
				// outer joins) that some sub-stream have a NULL DB_KEY.
				// In this case, we build a COALESCE(DB_KEY, _OCTETS x"0000000000000000"),
				// for the concatenation of sub DB_KEYs not result in NULL.
				if (type == nod_dbkey && stackCount > 1)
				{
					NodeStack stack2;

					for (NodeStack::iterator i(stack); i.hasData(); ++i)
					{
#ifdef CMP_DEBUG
						csb->dump(" %d", (USHORT)(IPTR) i.object()->nod_arg[0]);
#endif

						jrd_nod* new_node = PAR_make_node(tdbb, 3);
						new_node->nod_type = nod_value_if;
						new_node->nod_count = 3;

						// build an IF (RDB$DB_KEY IS NOT NULL)
						new_node->nod_arg[0] = PAR_make_node(tdbb, 1);
						new_node->nod_arg[0]->nod_type = nod_not;
						new_node->nod_arg[0]->nod_count = 1;
						new_node->nod_arg[0]->nod_arg[0] = PAR_make_node(tdbb, 1);
						new_node->nod_arg[0]->nod_arg[0]->nod_type = nod_missing;
						new_node->nod_arg[0]->nod_arg[0]->nod_count = 1;
						new_node->nod_arg[0]->nod_arg[0]->nod_arg[0] = i.object();

						new_node->nod_arg[1] = i.object();	// THEN

						const SSHORT count = lit_delta +
							(0 + sizeof(jrd_nod*) - 1) / sizeof(jrd_nod*);
						new_node->nod_arg[2] = PAR_make_node(tdbb, count);	// ELSE
						new_node->nod_arg[2]->nod_type = nod_literal;
						new_node->nod_arg[2]->nod_count = 0;
						Literal* literal = (Literal*) new_node->nod_arg[2];
						literal->lit_desc.dsc_dtype = dtype_text;
						literal->lit_desc.dsc_ttype() = CS_BINARY;
						literal->lit_desc.dsc_scale = 0;
						literal->lit_desc.dsc_length = 8;
						literal->lit_desc.dsc_address = reinterpret_cast<UCHAR*>(
							const_cast<char*>("\0\0\0\0\0\0\0\0"));	// safe const_cast

						stack2.push(new_node);
					}

					stack.clear();

					// stack2 is in reverse order, pushing everything in stack
					// will correct the order.
					for (NodeStack::iterator i2(stack2); i2.hasData(); ++i2)
						stack.push(i2.object());
				}

				node = catenate_nodes(tdbb, stack);

				if (type == nod_dbkey && stackCount > 1)
				{
					// ASF: If the view is in null state (with outer joins) we need to transform
					// the view RDB$KEY to NULL. (CORE-1245)

					jrd_nod* new_node = PAR_make_node(tdbb, 3);
					new_node->nod_type = nod_value_if;
					new_node->nod_count = 3;

					// build an IF (RDB$DB_KEY = '')
					new_node->nod_arg[0] = PAR_make_node(tdbb, 2);
					new_node->nod_arg[0]->nod_type = nod_eql;
					new_node->nod_arg[0]->nod_flags = nod_comparison;
					new_node->nod_arg[0]->nod_arg[0] = NodeCopier::copy(tdbb, csb, node, NULL);
					const SSHORT count = lit_delta + (0 + sizeof(jrd_nod*) - 1) / sizeof(jrd_nod*);
					new_node->nod_arg[0]->nod_arg[1] = PAR_make_node(tdbb, count);
					new_node->nod_arg[0]->nod_arg[1]->nod_type = nod_literal;
					new_node->nod_arg[0]->nod_arg[1]->nod_count = 0;
					Literal* literal = (Literal*) new_node->nod_arg[0]->nod_arg[1];
					literal->lit_desc.dsc_dtype = dtype_text;
					literal->lit_desc.dsc_ttype() = CS_BINARY;
					literal->lit_desc.dsc_scale = 0;
					literal->lit_desc.dsc_length = 0;
					literal->lit_desc.dsc_address = reinterpret_cast<UCHAR*>(literal->lit_data);

					new_node->nod_arg[1] = PAR_make_node(tdbb, 0);	// THEN: NULL
					new_node->nod_arg[1]->nod_type = nod_null;
					new_node->nod_arg[2] = node;					// ELSE: RDB$DB_KEY

					node = new_node;
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

			node = PAR_make_node(tdbb, 1);
			node->nod_count = 0;
			node->nod_type = type;
			node->nod_flags |= nod_agg_dbkey;
			node->nod_arg[0] = (jrd_nod*)(IPTR) stream;
			return node;
		}

	case nod_not:
		sub = node->nod_arg[0];
		if (sub->nod_type == nod_ansi_any)
			sub->nod_flags |= nod_deoptimize;
		if (sub->nod_type == nod_ansi_any || sub->nod_type == nod_ansi_all)
			sub->nod_flags |= nod_ansi_not;
		break;

	case nod_ansi_all:
		{
			jrd_nod* newNode = convertNeqAllToNotAny(tdbb, csb, node);
			if (newNode)
				return CMP_pass1(tdbb, csb, newNode);

			node->nod_flags |= nod_deoptimize;
		}
		// fall into

	case nod_ansi_any:
		if (node->nod_flags & nod_deoptimize)
		{
			node->nod_flags &= ~nod_deoptimize;

			fb_assert(node->nod_arg[e_any_rse]->nod_type == nod_class_recsrcnode_jrd);
			RseNode* rseNode = reinterpret_cast<RseNode*>(node->nod_arg[e_any_rse]->nod_arg[0]);
			fb_assert(rseNode->type == RseNode::TYPE);

			// Deoptimize the conjunct, not the ALL node itself
			jrd_nod* boolean = rseNode->rse_boolean;
			if (boolean)
			{
				if (boolean->nod_type == nod_and)
					boolean = boolean->nod_arg[1];

				// Deoptimize the injected boolean of a quantified predicate
				// when it's necessary. ALL predicate does not require an index scan.
				// This fixes bug SF #543106.
				boolean->nod_flags |= nod_deoptimize;
			}
		}
		// fall into

	case nod_any:
	case nod_exists:
	case nod_unique:
		{
			fb_assert(node->nod_arg[e_any_rse]->nod_type == nod_class_recsrcnode_jrd);
			RseNode* rseNode = reinterpret_cast<RseNode*>(node->nod_arg[e_any_rse]->nod_arg[0]);
			fb_assert(rseNode->type == RseNode::TYPE);
			rseNode->ignoreDbKey(tdbb, csb, view);
		}
		break;

	case nod_src_info:
		node->nod_arg[e_src_info_node] = CMP_pass1(tdbb, csb, node->nod_arg[e_src_info_node]);
		return node;

	case nod_class_exprnode_jrd:
	case nod_class_stmtnode_jrd:
		{
			DmlNode* dmlNode = reinterpret_cast<DmlNode*>(node->nod_arg[0]);
			node->nod_arg[0] = reinterpret_cast<jrd_nod*>(dmlNode->pass1(tdbb, csb, node));
		}
		return node;

	case nod_dcl_variable:
		{
			const USHORT n = (USHORT)(IPTR) node->nod_arg[e_dcl_id];
			vec<jrd_nod*>* vector = csb->csb_variables =
				vec<jrd_nod*>::newVector(*tdbb->getDefaultPool(), csb->csb_variables, n + 1);
			fb_assert(!(*vector)[n]);
			(*vector)[n] = node;
		}
		break;

	default:
		break;
	}

	// handle sub-expressions here

	ptr = node->nod_arg;

	for (const jrd_nod* const* const end = ptr + node->nod_count; ptr < end; ptr++) {
		*ptr = CMP_pass1(tdbb, csb, *ptr);
	}

	// perform any post-processing here

	if (node->nod_type == nod_assignment)
	{
		sub = node->nod_arg[e_asgn_to];
		switch (sub->nod_type)
		{
		case nod_field:
			stream = (USHORT)(IPTR) sub->nod_arg[e_fld_stream];
			tail = &csb->csb_rpt[stream];
			// assignments to the OLD context are prohibited
			// for all trigger types
			if ((tail->csb_flags & csb_trigger) && stream == 0)
			{
				ERR_post(Arg::Gds(isc_read_only_field));
			}
			// assignments to the NEW context are prohibited
			// for post-action triggers
			if ((tail->csb_flags & csb_trigger) && stream == 1 &&
				(csb->csb_g_flags & csb_post_trigger))
			{
				ERR_post(Arg::Gds(isc_read_only_field));
			}
			break;
		case nod_argument:
		case nod_variable:
		case nod_null:
			// Nothing to do here
			break;
		default:
			ERR_post(Arg::Gds(isc_read_only_field));
		}
	}

	return node;
}


static void pass1_erase(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	p a s s 1 _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Checkout an erase statement.  If it references a view, and
 *	is kosher, fix it up.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// if updateable views with triggers are involved, there
	// maybe a recursive call to be ignored

	if (node->nod_arg[e_erase_sub_erase])
		return;

	// to support nested views, loop until we hit a table or
	// a view with user-defined triggers (which means no update)

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parent_stream = 0;

	for (;;)
	{
		USHORT new_stream = (USHORT)(IPTR) node->nod_arg[e_erase_stream];
		const USHORT stream = new_stream;

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_erase;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent) {
			parent = tail->csb_view;
		}

		post_trigger_access(csb, relation, ExternalAccess::exa_delete, view);

		// Check out delete. If this is a delete thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_delete;
		if (parent) {
			priv |= SCL_read;
		}

		const trig_vec* trigger = relation->rel_pre_erase ?
			relation->rel_pre_erase : relation->rel_post_erase;

		// if we have a view with triggers, let's expand it

		if (relation->rel_view_rse && trigger)
		{
			new_stream = csb->nextStream();
			node->nod_arg[e_erase_stream] = (jrd_nod*) (IPTR) new_stream;
			CMP_csb_element(csb, new_stream)->csb_relation = relation;

			node->nod_arg[e_erase_statement] = pass1_expand_view(tdbb, csb, stream, new_stream, false);
			node->nod_count = MAX(node->nod_count, (USHORT) e_erase_statement + 1);
		}

		// get the source relation, either a table or yet another view

		RelationSourceNode* source = pass1_update(tdbb, csb, relation, trigger, stream,
			new_stream, priv, parent, parent_stream);

		if (!source)
			return;	// no source means we're done

		parent = relation;
		parent_stream = stream;

		// remap the source stream

		UCHAR* map = csb->csb_rpt[stream].csb_map;

		if (trigger)
		{
			// set up the new target stream

			jrd_nod* view_node = NodeCopier::copy(tdbb, csb, node, map);
			view_node->nod_arg[e_erase_statement] = NULL;
			view_node->nod_arg[e_erase_sub_erase] = NULL;

			node->nod_arg[e_erase_sub_erase] = view_node;
			node->nod_count = MAX(node->nod_count, (USHORT) e_erase_sub_erase + 1);

			// substitute the original delete node with the newly created one
			node = view_node;
		}
		else
		{
			// this relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view)
			csb->csb_rpt[new_stream].csb_flags &= ~csb_view_update;
		}

		// let's reset the target stream
		new_stream = source->getStream();
		node->nod_arg[e_erase_stream] = (jrd_nod*)(IPTR) map[new_stream];
	}
}


static jrd_nod* pass1_expand_view(thread_db* tdbb,
								 CompilerScratch* csb,
								 USHORT org_stream,
								 USHORT new_stream,
								 bool remap)
{
/**************************************
 *
 *	p a s s 1 _ e x p a n d _ v i e w
 *
 **************************************
 *
 * Functional description
 *	Process a view update performed by a trigger.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	NodeStack stack;
	jrd_rel* relation = csb->csb_rpt[org_stream].csb_relation;
	vec<jrd_fld*>* fields = relation->rel_fields;

	dsc desc;
	USHORT id = 0, new_id = 0;
	vec<jrd_fld*>::iterator ptr = fields->begin();
	for (const vec<jrd_fld*>::const_iterator end = fields->end(); ptr < end; ++ptr, ++id)
	{
		if (*ptr)
		{
			if (remap)
			{
				const jrd_fld* field = MET_get_field(relation, id);
				if (field->fld_source) {
					new_id = (USHORT)(IPTR) (field->fld_source)->nod_arg[e_fld_id];
				}
				else {
					new_id = id;
				}
			}
			else {
				new_id = id;
			}
			jrd_nod* node = PAR_gen_field(tdbb, new_stream, new_id);
			CMP_get_desc(tdbb, csb, node, &desc);
			if (!desc.dsc_address)
			{
				delete node;
				continue;
			}
			jrd_nod* assign = PAR_make_node(tdbb, e_asgn_length);
			assign->nod_type = nod_assignment;
			assign->nod_arg[e_asgn_to] = node;
			assign->nod_arg[e_asgn_from] = PAR_gen_field(tdbb, org_stream, id);
			stack.push(assign);
		}
	}

	return PAR_make_list(tdbb, stack);
}


static void pass1_modify(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	p a s s 1 _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Process a source for a modify statement.  This can
 *	get a little tricky if the relation is a view.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// if updateable views with triggers are involved, there
	// maybe a recursive call to be ignored

	if (node->nod_arg[e_mod_sub_mod])
		return;

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parent_stream = 0;

	// to support nested views, loop until we hit a table or
	// a view with user-defined triggers (which means no update)

	for (;;)
	{
		USHORT stream = (USHORT)(IPTR) node->nod_arg[e_mod_org_stream];
		USHORT new_stream = (USHORT)(IPTR) node->nod_arg[e_mod_new_stream];

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[new_stream];
		tail->csb_flags |= csb_modify;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent) {
			parent = tail->csb_view;
		}

		post_trigger_access(csb, relation, ExternalAccess::exa_update, view);

		// Check out update. If this is an update thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_update;
		if (parent) {
			priv |= SCL_read;
		}

		const trig_vec* trigger = (relation->rel_pre_modify) ?
			relation->rel_pre_modify : relation->rel_post_modify;

		// if we have a view with triggers, let's expand it

		if (relation->rel_view_rse && trigger)
		{
			node->nod_arg[e_mod_map_view] = pass1_expand_view(tdbb, csb, stream, new_stream, false);
			node->nod_count = MAX(node->nod_count, (USHORT) e_mod_map_view + 1);
		}

		// get the source relation, either a table or yet another view

		RelationSourceNode* source = pass1_update(tdbb, csb, relation, trigger, stream,
			new_stream, priv, parent, parent_stream);

		if (!source)
		{
			// no source means we're done

			if (!relation->rel_view_rse)
			{
				// apply validation constraints

				if ( (node->nod_arg[e_mod_validate] = make_validation(tdbb, csb, new_stream)) )
					node->nod_count = MAX(node->nod_count, (USHORT) e_mod_validate + 1);
			}

			return;
		}

		parent = relation;
		parent_stream = stream;

		// remap the source stream

		UCHAR* map = csb->csb_rpt[stream].csb_map;

		stream = source->getStream();
		stream = map[stream];

		// copy the view source

		map = CMP_alloc_map(tdbb, csb, (SSHORT)(IPTR) node->nod_arg[e_mod_new_stream]);
		NodeCopier copier(csb, map);
		source = source->copy(tdbb, copier);

		if (trigger)
		{
			// set up the new target stream

			const USHORT view_stream = new_stream;
			new_stream = source->getStream();
			fb_assert(new_stream <= MAX_STREAMS);
			map[view_stream] = new_stream;

			jrd_nod* view_node = ViewNodeCopier(csb, map).copy(tdbb, node);
			view_node->nod_arg[e_mod_map_view] = NULL;
			view_node->nod_arg[e_mod_statement] =
				pass1_expand_view(tdbb, csb, view_stream, new_stream, true);

			node->nod_arg[e_mod_sub_mod] = view_node;
			node->nod_count = MAX(node->nod_count, (USHORT) e_mod_sub_mod + 1);

			// substitute the original update node with the newly created one
			node = view_node;
		}
		else
		{
			// this relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view)

			csb->csb_rpt[new_stream].csb_flags &= ~csb_view_update;
		}

		// let's reset streams to represent the mapped source and target
		node->nod_arg[e_mod_org_stream] = (jrd_nod*)(IPTR) stream;
		node->nod_arg[e_mod_new_stream] = (jrd_nod*)(IPTR) source->getStream();
	}
}


static bool pass1_store(thread_db* tdbb, CompilerScratch* csb, jrd_nod* node)
{
/**************************************
 *
 *	p a s s 1 _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Process a source for a store statement.  This can get a little tricky if
 *	the relation is a view.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// if updateable views with triggers are involved, there
	// may be a recursive call to be ignored

	if (node->nod_arg[e_sto_sub_store])
		return false;

	jrd_rel* parent = NULL;
	jrd_rel* view = NULL;
	USHORT parent_stream = 0;

	// to support nested views, loop until we hit a table or
	// a view with user-defined triggers (which means no update)

	for (;;)
	{
		fb_assert(node->nod_arg[e_sto_relation]->nod_type == nod_class_recsrcnode_jrd);
		RelationSourceNode* relSource = reinterpret_cast<RelationSourceNode*>(
			node->nod_arg[e_sto_relation]->nod_arg[0]);
		fb_assert(relSource->type == RelationSourceNode::TYPE);

		const USHORT stream = relSource->getStream();

		CompilerScratch::csb_repeat* tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_store;

		jrd_rel* relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent) {
			parent = tail->csb_view;
		}

		post_trigger_access(csb, relation, ExternalAccess::exa_insert, view);

		const trig_vec* trigger = (relation->rel_pre_store) ?
			relation->rel_pre_store : relation->rel_post_store;

		// Check out insert. If this is an insert thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		SecurityClass::flags_t priv = SCL_sql_insert;
		if (parent) {
			priv |= SCL_read;
		}

		// get the source relation, either a table or yet another view

		relSource = pass1_update(tdbb, csb, relation, trigger, stream, stream, priv,
			parent, parent_stream);

		if (!relSource)
		{
			CMP_post_resource(&csb->csb_resources, relation, Resource::rsc_relation, relation->rel_id);

			if (!relation->rel_view_rse)
			{
				// apply validation constraints
				if ( (node->nod_arg[e_sto_validate] = make_validation(tdbb, csb, stream)) )
					node->nod_count = MAX(node->nod_count, (USHORT) e_sto_validate + 1);
			}

			return true;
		}

		parent = relation;
		parent_stream = stream;

		UCHAR* map = CMP_alloc_map(tdbb, csb, stream);
		NodeCopier copier(csb, map);

		if (trigger)
		{
			CMP_post_resource(&csb->csb_resources, relation, Resource::rsc_relation, relation->rel_id);

			// set up the new target stream

			jrd_nod* view_node = NodeCopier::copy(tdbb, csb, node, map);
			view_node->nod_arg[e_sto_sub_store] = NULL;

			relSource = relSource->copy(tdbb, copier);

			view_node->nod_arg[e_sto_relation] = PAR_make_node(tdbb, 1);
			view_node->nod_arg[e_sto_relation]->nod_type = nod_class_recsrcnode_jrd;
			view_node->nod_arg[e_sto_relation]->nod_count = 0;
			view_node->nod_arg[e_sto_relation]->nod_arg[0] = reinterpret_cast<jrd_nod*>(relSource);

			const USHORT new_stream = relSource->getStream();
			view_node->nod_arg[e_sto_statement] =
				pass1_expand_view(tdbb, csb, stream, new_stream, true);

			// dimitr:	I don't think the below code is required, but time will show
			//	view_node->nod_arg[e_sto_statement] =
			//		NodeCopier::copy(tdbb, csb, view_node->nod_arg[e_sto_statement], NULL);

			// bug 8150: use of blr_store2 against a view with a trigger was causing
			// the second statement to be executed, which is not desirable

			view_node->nod_arg[e_sto_statement2] = NULL;

			node->nod_arg[e_sto_sub_store] = view_node;
			node->nod_count = MAX(node->nod_count, (USHORT) e_sto_sub_store + 1);

			// substitute the original update node with the newly created one
			node = view_node;
		}
		else
		{
			// this relation is not actually being updated as this operation
			// goes deeper (we have a naturally updatable view)

			csb->csb_rpt[stream].csb_flags &= ~csb_view_update;

			relSource = relSource->copy(tdbb, copier);

			node->nod_arg[e_sto_relation] = PAR_make_node(tdbb, 1);
			node->nod_arg[e_sto_relation]->nod_type = nod_class_recsrcnode_jrd;
			node->nod_arg[e_sto_relation]->nod_count = 0;
			node->nod_arg[e_sto_relation]->nod_arg[0] = reinterpret_cast<jrd_nod*>(relSource);
		}
	}
}


static RelationSourceNode* pass1_update(thread_db* tdbb, CompilerScratch* csb, jrd_rel* relation,
	const trig_vec* trigger, USHORT stream, USHORT update_stream, SecurityClass::flags_t priv,
	jrd_rel* view, USHORT view_stream)
{
/**************************************
 *
 *	p a s s 1 _ u p d a t e
 *
 **************************************
 *
 * Functional description
 *	Check out a prospective update to a relation.  If it fails
 *	security check, bounce it.  If it's a view update, make sure
 *	the view is updatable,  and return the view source for redirection.
 *	If it's a simple relation, return NULL.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(relation, type_rel);
	DEV_BLKCHK(view, type_rel);

	// unless this is an internal request, check access permission

	CMP_post_access(tdbb, csb, relation->rel_security_name, (view ? view->rel_id : 0),
					priv, SCL_object_table, relation->rel_name);

	// ensure that the view is set for the input streams,
	// so that access to views can be checked at the field level

	fb_assert(view_stream <= MAX_STREAMS);
	CMP_csb_element(csb, stream)->csb_view = view;
	CMP_csb_element(csb, stream)->csb_view_stream = (UCHAR) view_stream;
	CMP_csb_element(csb, update_stream)->csb_view = view;
	CMP_csb_element(csb, update_stream)->csb_view_stream = (UCHAR) view_stream;

	// if we're not a view, everything's cool

	RseNode* rse = relation->rel_view_rse;
	if (!rse)
		return NULL;

	// a view with triggers is always updatable

	if (trigger)
	{
		bool user_triggers = false;
		for (size_t i = 0; i < trigger->getCount(); i++)
		{
			if (!(*trigger)[i].sys_trigger)
			{
				user_triggers = true;
				break;
			}
		}

		if (user_triggers)
		{
			csb->csb_rpt[update_stream].csb_flags |= csb_view_update;
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

	csb->csb_rpt[update_stream].csb_flags |= csb_view_update;

	return static_cast<RelationSourceNode*>(rse->rse_relations[0].getObject());
}


static jrd_nod* pass2_validation(thread_db* tdbb, CompilerScratch* csb, const Item& item)
{
/**************************************
 *
 *	p a s s 2 _ v a l i d a t i o n
 *
 **************************************
 *
 * Functional description
 *	Copy items' information into appropriate node
 *
 **************************************/
	ItemInfo itemInfo;
	return csb->csb_map_item_info.get(item, itemInfo) ?
		reinterpret_cast<jrd_nod*>(FB_NEW(*tdbb->getDefaultPool())
			ItemInfo(*tdbb->getDefaultPool(), itemInfo)) : 0;
}


jrd_nod* CMP_pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* const node, jrd_nod* parent)
{
/**************************************
 *
 *	C M P _ p a s s 2
 *
 **************************************
 *
 * Functional description
 *	Allocate and assign impure space for various nodes.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);
	DEV_BLKCHK(parent, type_nod);

	if (!node) {
		return node;
	}

	if (parent) {
		node->nod_parent = parent;
	}

	// if there is processing to be done before sub expressions, do it here
	USHORT stream;

	DEBUG;
	RseNode* rse_node = NULL;
	RecordSource** rsb_ptr = NULL;
	Cursor** cursor_ptr = NULL;

	switch (node->nod_type)
	{
	case nod_class_stmtnode_jrd:
		reinterpret_cast<StmtNode*>(node->nod_arg[0])->pass2Cursor(rse_node, cursor_ptr);
		break;

	case nod_dcl_cursor:
		fb_assert(node->nod_arg[e_dcl_cur_rse]->nod_type == nod_class_recsrcnode_jrd);
		rse_node = reinterpret_cast<RseNode*>(node->nod_arg[e_dcl_cur_rse]->nod_arg[0]);
		fb_assert(rse_node->type == RseNode::TYPE);
		cursor_ptr = (Cursor**) &node->nod_arg[e_dcl_cur_cursor];
		break;

	case nod_cursor_stmt:
		CMP_pass2(tdbb, csb, node->nod_arg[e_cursor_stmt_scroll_val], node);
		CMP_pass2(tdbb, csb, node->nod_arg[e_cursor_stmt_into], node);
		break;

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_average:
	case nod_total:
	case nod_from:
		fb_assert(node->nod_arg[e_stat_rse]->nod_type == nod_class_recsrcnode_jrd);
		rse_node = reinterpret_cast<RseNode*>(node->nod_arg[e_stat_rse]->nod_arg[0]);

		if (!rse_node)
			ERR_post(Arg::Gds(isc_wish_list));

		fb_assert(rse_node->type == RseNode::TYPE);

		if (!(rse_node->flags & RseNode::FLAG_VARIANT))
		{
			node->nod_flags |= nod_invariant;
			csb->csb_invariants.push(node);
		}

		rsb_ptr = (RecordSource**) &node->nod_arg[e_stat_rsb];
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		fb_assert(node->nod_arg[e_any_rse]->nod_type == nod_class_recsrcnode_jrd);
		rse_node = reinterpret_cast<RseNode*>(node->nod_arg[e_any_rse]->nod_arg[0]);
		fb_assert(rse_node->type == RseNode::TYPE);

		if (!(rse_node->flags & RseNode::FLAG_VARIANT))
		{
			node->nod_flags |= nod_invariant;
			csb->csb_invariants.push(node);
		}

		rsb_ptr = (RecordSource**) &node->nod_arg[e_any_rsb];
		break;

	case nod_like:
	case nod_contains:
	case nod_similar:
	case nod_starts:
		if (node->nod_flags & nod_invariant) {
			csb->csb_invariants.push(node);
		}
		break;

	case nod_sort:
		{
			jrd_nod** ptr = node->nod_arg;
			for (jrd_nod** end = ptr + node->nod_count; ptr < end; ptr++)
			{
				(*ptr)->nod_flags |= nod_value;
			}
		}
		break;

	case nod_src_info:
		node->nod_arg[e_src_info_node] = CMP_pass2(tdbb, csb, node->nod_arg[e_src_info_node], node);
		return node;

	case nod_variable:
		node->nod_arg[e_var_info] =
			pass2_validation(tdbb, csb, Item(nod_variable, (IPTR) node->nod_arg[e_var_id]));
		break;

	case nod_init_variable:
		node->nod_arg[e_init_var_info] =
			pass2_validation(tdbb, csb, Item(nod_variable, (IPTR) node->nod_arg[e_init_var_id]));
		break;

	case nod_argument:
		node->nod_arg[e_arg_info] =
			pass2_validation(tdbb, csb, Item(nod_argument,
				(IPTR) node->nod_arg[e_arg_message]->nod_arg[e_msg_number],
				(IPTR) node->nod_arg[e_arg_number]));
		break;

	default:
		break;
	}

	if (rse_node)
		rse_node->pass2Rse(tdbb, csb);

	// handle sub-expressions here

	// AB: Mark the streams involved with INSERT/UPDATE statements active.
	// So that the optimizer can use indices for eventually used sub-selects.
	if (node->nod_type == nod_modify)
	{
		stream = (USHORT)(IPTR) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
		stream = (USHORT)(IPTR) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
	}
	else if (node->nod_type == nod_store)
	{
		fb_assert(node->nod_arg[e_sto_relation]->nod_type == nod_class_recsrcnode_jrd);
		RelationSourceNode* recSource = reinterpret_cast<RelationSourceNode*>(
			node->nod_arg[e_sto_relation]->nod_arg[0]);
		fb_assert(recSource->type == RelationSourceNode::TYPE);

		stream = recSource->getStream();
		csb->csb_rpt[stream].csb_flags |= csb_active;
	}

	jrd_nod** ptr = node->nod_arg;
	// This "end" is used later.
	const jrd_nod* const* const end = ptr + node->nod_count;

	for (; ptr < end; ptr++)
		CMP_pass2(tdbb, csb, *ptr, node);

	// AB: Remove the previous flags
	if (node->nod_type == nod_modify)
	{
		stream = (USHORT)(IPTR) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
		stream = (USHORT)(IPTR) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
	}
	else if (node->nod_type == nod_store)
	{
		fb_assert(node->nod_arg[e_sto_relation]->nod_type == nod_class_recsrcnode_jrd);
		RelationSourceNode* recSource = reinterpret_cast<RelationSourceNode*>(
			node->nod_arg[e_sto_relation]->nod_arg[0]);
		fb_assert(recSource->type == RelationSourceNode::TYPE);

		stream = recSource->getStream();
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
	}

	// Handle any residual work

	node->nod_impure = CMP_impure(csb, 0);

	switch (node->nod_type)
	{
	case nod_class_recsrcnode_jrd:
		reinterpret_cast<RecordSourceNode*>(node->nod_arg[0])->pass2(tdbb, csb);
		break;

	case nod_assignment:
		CMP_pass2(tdbb, csb, node->nod_arg[e_asgn_missing2], node);
		break;

	case nod_average:
		node->nod_flags |= nod_double;
		// FALL INTO

	case nod_max:
	case nod_min:
	case nod_from:
	case nod_count:
		node->nod_count = 0;
		csb->csb_impure += sizeof(impure_value_ex);
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		if (node->nod_flags & nod_invariant) {
			csb->csb_impure += sizeof(impure_value);
		}
		break;

	case nod_block:
		csb->csb_impure += sizeof(SLONG);
		break;

	case nod_dcl_variable:
		{
			const dsc* desc = (DSC*) (node->nod_arg + e_dcl_desc);
			csb->csb_impure += sizeof(impure_value) + desc->dsc_length;
		}
		break;

	case nod_total:
		{
			node->nod_count = 0;
			csb->csb_impure += sizeof(impure_value_ex);
			dsc descriptor_a;
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
		}
		break;

	case nod_message:
		{
			const Format* format = (Format*) node->nod_arg[e_msg_format];
			fb_assert(format);
			csb->csb_impure += FB_ALIGN(format->fmt_length, 2);
			node->nod_arg[e_msg_impure_flags] = (jrd_nod*)(IPTR) CMP_impure(csb, 0);
			csb->csb_impure += sizeof(USHORT) * format->fmt_count;
		}
		break;

	case nod_modify:
		{
			stream = (USHORT)(IPTR) node->nod_arg[e_mod_org_stream];
			csb->csb_rpt[stream].csb_flags |= csb_update;
			const Format* format = CMP_format(tdbb, csb, stream);
			Format::fmt_desc_const_iterator desc = format->fmt_desc.begin();
			for (ULONG id = 0; id < format->fmt_count; ++id, ++desc)
			{
				if (desc->dsc_dtype) {
					SBM_SET(tdbb->getDefaultPool(), &csb->csb_rpt[stream].csb_fields, id);
				}
			}
			csb->csb_impure += sizeof(impure_state);
		}
		break;

	case nod_list:
		// We are using the same "node" always. The "end" was set in the middle
		// of the two huge switch() statements (this is the second).
		if (node->nod_count > 0)
		{
			node->nod_type = nod_asn_list;
			for (ptr = node->nod_arg; ptr < end; ptr++)
			{
				if (*ptr && (*ptr)->nod_type != nod_assignment)
				{
					node->nod_type = nod_list;
					break;
				}
			}
		}
		// FALL INTO

	case nod_store:
		csb->csb_impure += sizeof(impure_state);
		break;

	case nod_erase:
		stream = (USHORT)(IPTR) node->nod_arg[e_erase_stream];
		csb->csb_rpt[stream].csb_flags |= csb_update;
		break;

	case nod_field:
		{
			stream = (USHORT)(IPTR) node->nod_arg[e_fld_stream];
			// SMB_SET uses ULONG, not USHORT
			const ULONG id = (ULONG)(IPTR) node->nod_arg[e_fld_id];
			SBM_SET(tdbb->getDefaultPool(), &csb->csb_rpt[stream].csb_fields, id);
			if (csb->csb_rpt[stream].csb_relation || csb->csb_rpt[stream].csb_procedure)
				node->nod_arg[e_fld_format] = (jrd_nod*) CMP_format(tdbb, csb, stream);
			csb->csb_impure += sizeof(impure_value_ex);
			break;
		}

	case nod_argument:
	case nod_variable:
		csb->csb_impure += (node->nod_flags & nod_value) ?
			sizeof(impure_value_ex) : sizeof(dsc);
		break;

	case nod_literal:
	case nod_dbkey:
	case nod_rec_version:
	case nod_negate:
	case nod_substr:
	case nod_trim:
	case nod_divide:
	case nod_null:
	case nod_user_name:
	case nod_current_role:
	case nod_internal_info:
	case nod_gen_id:
	case nod_gen_id2:
	case nod_upcase:
	case nod_lowcase:
	case nod_prot_mask:
	case nod_lock_state:
	case nod_scalar:
	case nod_cast:
	case nod_extract:
	case nod_strlen:
	case nod_current_time:
	case nod_current_timestamp:
	case nod_current_date:
	case nod_derived_expr:
		{
			dsc descriptor_a;
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
			csb->csb_impure += sizeof(impure_value);
		}
		break;

	// compute the target descriptor to compute computational class

	case nod_multiply:
	case nod_add:
	case nod_subtract:
	case nod_add2:
	case nod_subtract2:
	case nod_multiply2:
	case nod_divide2:
		{
			dsc descriptor_a;
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
			csb->csb_impure += sizeof(impure_value);
		}
		break;

	// boolean nodes taking three values as inputs
	case nod_like:
	case nod_between:
	case nod_similar:
	case nod_sleuth:
		if (node->nod_count > 2)
		{
			if (node->nod_arg[2]->nod_flags & nod_agg_dbkey) {
				ERR_post(Arg::Gds(isc_bad_dbkey));
			}
			dsc descriptor_c;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_c);
			if (DTYPE_IS_DATE(descriptor_c.dsc_dtype))
			{
				node->nod_arg[0]->nod_flags |= nod_date;
				node->nod_arg[1]->nod_flags |= nod_date;
			}
		}
		// FALLINTO

		// boolean nodes taking two values as inputs
	case nod_matches:
	case nod_contains:
	case nod_starts:
	case nod_equiv:
	case nod_eql:
	case nod_neq:
	case nod_geq:
	case nod_gtr:
	case nod_lss:
	case nod_leq:
		{
			if ((node->nod_arg[0]->nod_flags & nod_agg_dbkey) ||
				(node->nod_arg[1]->nod_flags & nod_agg_dbkey))
			{
				ERR_post(Arg::Gds(isc_bad_dbkey));
			}
			dsc descriptor_a, descriptor_b;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &descriptor_b);
			if (DTYPE_IS_DATE(descriptor_a.dsc_dtype))
				node->nod_arg[1]->nod_flags |= nod_date;
			else if (DTYPE_IS_DATE(descriptor_b.dsc_dtype))
				node->nod_arg[0]->nod_flags |= nod_date;
		}
		if (node->nod_flags & nod_invariant)
		{
			// This may currently happen for nod_like, nod_contains and nod_similar
			csb->csb_impure += sizeof(impure_value);
		}
		break;

	// boolean nodes taking one value as input
	case nod_missing:
		{
			if (node->nod_arg[0]->nod_flags & nod_agg_dbkey) {
				ERR_post(Arg::Gds(isc_bad_dbkey));
			}

			// check for syntax errors in the calculation
			dsc descriptor_a;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
		}
		break;

	case nod_exec_into:
		csb->csb_impure += sizeof(ExecuteStatement);
		csb->csb_exec_sta.push(node);
		break;

	case nod_exec_stmt:
		csb->csb_impure += sizeof(void**);
		break;

	case nod_class_exprnode_jrd:
	case nod_class_stmtnode_jrd:
		node->nod_arg[0] = reinterpret_cast<jrd_nod*>(
			reinterpret_cast<DmlNode*>(node->nod_arg[0])->pass2(tdbb, csb, node));
		break;

	default:
		// note: no fb_assert(false); here as too many nodes are missing
		break;
	}

	// Bind values of invariant nodes to top-level RSE (if present)
	if (node->nod_flags & nod_invariant)
	{
		if (csb->csb_current_nodes.hasData())
		{
			LegacyNodeOrRseNode& top_rse_node = csb->csb_current_nodes[0];
			fb_assert(top_rse_node.rseNode);

			if (!top_rse_node.rseNode->rse_invariants)
			{
				top_rse_node.rseNode->rse_invariants =
					FB_NEW(*tdbb->getDefaultPool()) VarInvariantArray(*tdbb->getDefaultPool());
			}

			top_rse_node.rseNode->rse_invariants->add(node->nod_impure);
		}
	}

	// finish up processing of record selection expressions

	if (rse_node)
	{
		RecordSource* const rsb = post_rse(tdbb, csb, rse_node);

		// for ansi ANY clauses (and ALL's, which are negated ANY's)
		// the unoptimized boolean expression must be used, since the
		// processing of these clauses is order dependant (see FilteredStream.cpp)

		if (node->nod_type == nod_ansi_any || node->nod_type == nod_ansi_all)
		{
			const bool ansiAny = (node->nod_type == nod_ansi_any);
			const bool ansiNot = (node->nod_flags & nod_ansi_not);
			FilteredStream* const filter = reinterpret_cast<FilteredStream*>(rsb);
			filter->setAnyBoolean(rse_node->rse_boolean, ansiAny, ansiNot);
		}

		csb->csb_fors.add(rsb);

		if (rsb_ptr)
		{
			*rsb_ptr = rsb;
		}

		if (cursor_ptr)
		{
			Cursor* const cursor = FB_NEW(*tdbb->getDefaultPool()) Cursor(
				csb, rsb, rse_node->rse_invariants, (rse_node->flags & RseNode::FLAG_SCROLLABLE));
			*cursor_ptr = cursor;
		}
	}

	return node;
}


void CMP_post_procedure_access(thread_db* tdbb, CompilerScratch* csb, jrd_prc* procedure)
{
/**************************************
 *
 *	C M P _ p o s t _ p r o c e d u r e _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *
 *	The request will inherit access requirements to all the objects
 *	the called stored procedure has access requirements for.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	// allow all access to internal requests

	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
		return;

	const TEXT* prc_sec_name = procedure->getSecurityName().nullStr();

	// this request must have EXECUTE permission on the stored procedure
	if (procedure->getName().package.isEmpty())
	{
		CMP_post_access(tdbb, csb, prc_sec_name, 0, SCL_execute, SCL_object_procedure,
						procedure->getName().identifier.c_str());
	}
	else
	{
		CMP_post_access(tdbb, csb, prc_sec_name, 0, SCL_execute, SCL_object_package,
						procedure->getName().package.c_str());
	}

	// Add the procedure to list of external objects accessed
	ExternalAccess temp(ExternalAccess::exa_procedure, procedure->getId());
	size_t idx;
	if (!csb->csb_external.find(temp, idx))
		csb->csb_external.insert(idx, temp);
}


static RecordSource* post_rse(thread_db* tdbb, CompilerScratch* csb, RseNode* rse)
{
/**************************************
 *
 *	p o s t _ r s e
 *
 **************************************
 *
 * Functional description
 *	Perform actual optimization of an RseNode and clear activity.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);

	RecordSource* rsb = OPT_compile(tdbb, csb, rse, NULL);

	if (rse->flags & RseNode::FLAG_SINGULAR)
		rsb = FB_NEW(*tdbb->getDefaultPool()) SingularStream(csb, rsb);

	if (rse->flags & RseNode::FLAG_WRITELOCK)
	{
		for (USHORT i = 0; i < csb->csb_n_stream; i++)
			csb->csb_rpt[i].csb_flags |= csb_update;

		rsb = FB_NEW(*tdbb->getDefaultPool()) LockedStream(csb, rsb);
	}

	if (rse->flags & RseNode::FLAG_SCROLLABLE)
		rsb = FB_NEW(*tdbb->getDefaultPool()) BufferedStream(csb, rsb);

	// mark all the substreams as inactive

	const NestConst<RecordSourceNode>* ptr = rse->rse_relations.begin();
	for (const NestConst<RecordSourceNode>* const end = rse->rse_relations.end(); ptr != end; ++ptr)
	{
		const RecordSourceNode* const node = *ptr;

		StreamsArray streams;
		node->getStreams(streams);

		for (StreamsArray::iterator i = streams.begin(); i != streams.end(); ++i)
			csb->csb_rpt[*i].csb_flags &= ~csb_active;
	}

	return rsb;
}


static void post_trigger_access(CompilerScratch* csb,
								jrd_rel* owner_relation,
								ExternalAccess::exa_act operation, jrd_rel* view)
{
/**************************************
 *
 *	p o s t _ t r i g g e r _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *	Inherit access to triggers to be fired.
 *
 *	When we detect that a trigger could be fired by a request,
 *	then we add the access list for that trigger to the access
 *	list for this request.  That way, when we check access for
 *	the request we also check access for any other objects that
 *	could be fired off by the request.
 *
 *	Note that when we add the access item, we specify that
 *	   Trigger X needs access to resource Y.
 *	In the access list we parse here, if there is no "accessor"
 *	name then the trigger must access it directly.  If there is
 *	an "accessor" name, then something accessed by this trigger
 *	must require the access.
 *
 *  CVC: The third parameter is the owner of the triggers vector
 *   and was added to avoid triggers posting access checks to
 *   their base tables, since it's nonsense and causes weird
 *   messages about false REFERENCES right failures.
 *
 **************************************/
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	// allow all access to internal requests
	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
		return;

	// Post trigger access
	ExternalAccess temp(operation, owner_relation->rel_id, view ? view->rel_id : 0);
	size_t i;
	if (!csb->csb_external.find(temp, i))
		csb->csb_external.insert(i, temp);
}
