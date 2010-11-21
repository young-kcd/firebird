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
#include "../common/common.h"
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
#include "../common/gdsassert.h"
#include "../jrd/cmp_proto.h"
#include "../common/dsc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/fun_proto.h"
#include "../yvalve/gds_proto.h"
#include "../jrd/idx_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../common/dsc_proto.h"
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
#include "../dsql/BoolNodes.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/StmtNodes.h"

using namespace Jrd;
using namespace Firebird;


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
				fldId = field->fld_source->as<FieldNode>()->fieldId;

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
		virtual USHORT getFieldId(FieldNode* field)
		{
			if (field->byId && field->fieldId == 0 && field->fieldStream == 0)
				return fldId;

			return NodeCopier::getFieldId(field);
		}

	private:
		USHORT fldId;
	};
}	// namespace

static jrd_nod* make_defaults(thread_db*, CompilerScratch*, USHORT, jrd_nod*);
static jrd_nod* make_validation(thread_db*, CompilerScratch*, USHORT);
static void pass1_erase(thread_db*, CompilerScratch*, jrd_nod*);
static jrd_nod* pass1_expand_view(thread_db*, CompilerScratch*, USHORT, USHORT, bool);
static void pass1_modify(thread_db*, CompilerScratch*, jrd_nod*);
static bool pass1_store(thread_db*, CompilerScratch*, jrd_nod*);
static RelationSourceNode* pass1_update(thread_db*, CompilerScratch*, jrd_rel*, const trig_vec*, USHORT, USHORT,
	SecurityClass::flags_t, jrd_rel*, USHORT);
static void	post_trigger_access(CompilerScratch*, jrd_rel*, ExternalAccess::exa_act, jrd_rel*);

#ifdef CMP_DEBUG
#include <stdarg.h>
IMPLEMENT_TRACE_ROUTINE(cmp_trace, "CMP")
#endif


// Clone a node.
ValueExprNode* CMP_clone_node(thread_db* tdbb, CompilerScratch* csb, ValueExprNode* node)
{
	return NodeCopier::copy(tdbb, csb, node, NULL);
}


// Clone a value node for the optimizer.
// Make a copy of the node (if necessary) and assign impure space.

ValueExprNode* CMP_clone_node_opt(thread_db* tdbb, CompilerScratch* csb, ValueExprNode* node)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	if (node->is<ParameterNode>())
		return node;

	ValueExprNode* clone = NodeCopier::copy(tdbb, csb, node, NULL);
	CMP_pass2(tdbb, csb, clone);

	return clone;
}

BoolExprNode* CMP_clone_node_opt(thread_db* tdbb, CompilerScratch* csb, BoolExprNode* node)
{
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	NodeCopier copier(csb, NULL);
	BoolExprNode* clone = copier.copy(tdbb, node);

	CMP_pass2(tdbb, csb, clone);

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
	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	switch (node->nod_type)
	{
	case nod_class_exprnode_jrd:
		{
			ValueExprNode* exprNode = reinterpret_cast<ValueExprNode*>(node->nod_arg[0]);
			exprNode->getDesc(tdbb, csb, desc);
			return;
		}

	default:
		fb_assert(false);
		ERR_post(Arg::Gds(isc_datype_notsup));	// data type not supported for arithmetic
		break;
	}
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


USHORT NodeCopier::getFieldId(FieldNode* field)
{
	return field->fieldId;
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

			if (copy == exprNode)
				node = input;
			else
			{
				copy->nodFlags = exprNode->nodFlags;

				node = PAR_make_node(tdbb, 1);
				node->nod_type = input->nod_type;
				node->nod_count = input->nod_count;
				node->nod_arg[0] = reinterpret_cast<jrd_nod*>(copy);
			}
		}
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

	case nod_init_variable:
		if (csb->csb_remap_variable != 0)
		{
			node = PAR_make_node(tdbb, e_init_var_length);
			node->nod_type = input->nod_type;
			node->nod_count = input->nod_count;

			USHORT n = csb->csb_remap_variable + (USHORT)(IPTR) input->nod_arg[e_init_var_id];
			node->nod_arg[e_init_var_id] = (jrd_nod*)(IPTR) n;
			node->nod_arg[e_init_var_variable] = input->nod_arg[e_init_var_variable];
			node->nod_arg[e_init_var_info] = input->nod_arg[e_init_var_info];
			return node;
		}
		return input;

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
				vec<jrd_nod*>::newVector(*tdbb->getDefaultPool(), csb->csb_variables, n + 1);

			return node;
		}
		return input;

	case nod_dcl_cursor:
		node = PAR_make_node(tdbb, e_dcl_cur_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_dcl_cur_rse] = copy(tdbb, input->nod_arg[e_dcl_cur_rse]);
		node->nod_arg[e_dcl_cur_refs] = copy(tdbb, input->nod_arg[e_dcl_cur_refs]);
		node->nod_arg[e_dcl_cur_number] = input->nod_arg[e_dcl_cur_number];
		break;

	case nod_cursor_stmt:
		node = PAR_make_node(tdbb, e_cursor_stmt_length);
		node->nod_count = input->nod_count;
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

	jrd_nod** arg1 = input->nod_arg;
	jrd_nod** arg2 = node->nod_arg;

	for (const jrd_nod* const* const end = arg1 + input->nod_count; arg1 < end; arg1++, arg2++)
	{
		if (*arg1)
			*arg2 = copy(tdbb, *arg1);
	}

	return node;
}


// Expand dbkey for view.
void CMP_expand_view_nodes(thread_db* tdbb, CompilerScratch* csb, USHORT stream,
	ValueExprNodeStack& stack, UCHAR blrOp, bool allStreams)
{
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
			CMP_expand_view_nodes(tdbb, csb, *map++, stack, blrOp, allStreams);
		return;
	}

	// relation is primitive - make dbkey node

	if (allStreams || csb->csb_rpt[stream].csb_relation)
	{
		RecordKeyNode* node = FB_NEW(csb->csb_pool) RecordKeyNode(csb->csb_pool, blrOp);
		node->recStream = stream;
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
		ValueExprNode* value;

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

					const FieldNode* fieldNode = ExprNode::as<FieldNode>(to);
					fb_assert(fieldNode);

					if (fieldNode && fieldNode->fieldStream == stream && fieldNode->fieldId == field_id)
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

				LiteralNode* literal = FB_NEW(csb->csb_pool) LiteralNode(csb->csb_pool);
				SLONG* increment = FB_NEW(csb->csb_pool) SLONG(1);
				literal->litDesc.makeLong(0, increment);

				GenIdNode* genNode = FB_NEW(csb->csb_pool) GenIdNode(csb->csb_pool,
					(csb->csb_g_flags & csb_blr_version4), (*ptr1)->fld_generator_name);
				genNode->id = MET_lookup_generator(tdbb, (*ptr1)->fld_generator_name.c_str());
				genNode->arg = literal;

				jrd_nod* genNod = PAR_make_node(tdbb, 1);
				genNod->nod_type = nod_class_exprnode_jrd;
				genNod->nod_count = 0;
				genNod->nod_arg[0] = reinterpret_cast<jrd_nod*>(genNode);
				node->nod_arg[e_asgn_from] = genNod;
			}
			else //if (value)
			{
				// Clone the field default value.

				jrd_nod* nod = PAR_make_node(tdbb, 1);
				nod->nod_type = nod_class_exprnode_jrd;
				nod->nod_count = 0;
				nod->nod_arg[0] = reinterpret_cast<jrd_nod*>(
					RemapFieldNodeCopier(csb, map, field_id).copy(tdbb, value));

				node->nod_arg[e_asgn_from] = nod;
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
		BoolExprNode* validation;
		jrd_nod* validationStmt;

		if (*ptr1 && (validation = (*ptr1)->fld_validation))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			RemapFieldNodeCopier copier(csb, map, field_id);

			if ((validationStmt = (*ptr1)->fld_validation_stmt))
				validationStmt = copier.copy(tdbb, validationStmt);

			validation = copier.copy(tdbb, validation);

			jrd_nod* boolNod = PAR_make_node(tdbb, 1);
			boolNod->nod_type = nod_class_exprnode_jrd;
			boolNod->nod_count = 0;
			boolNod->nod_arg[0] = reinterpret_cast<jrd_nod*>(validation);

			jrd_nod* node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_stmt] = validationStmt;
			node->nod_arg[e_val_boolean] = boolNod;
			node->nod_arg[e_val_value] = PAR_gen_field(tdbb, stream, field_id);
			stack.push(node);
		}

		if (*ptr1 && (validation = (*ptr1)->fld_not_null))
		{
			AutoSetRestore<USHORT> autoRemapVariable(&csb->csb_remap_variable,
				(csb->csb_variables ? csb->csb_variables->count() : 0) + 1);

			RemapFieldNodeCopier copier(csb, map, field_id);

			if ((validationStmt = (*ptr1)->fld_not_null_stmt))
				validationStmt = copier.copy(tdbb, validationStmt);

			validation = copier.copy(tdbb, validation);

			jrd_nod* boolNod = PAR_make_node(tdbb, 1);
			boolNod->nod_type = nod_class_exprnode_jrd;
			boolNod->nod_count = 0;
			boolNod->nod_arg[0] = reinterpret_cast<jrd_nod*>(validation);

			jrd_nod* node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_stmt] = validationStmt;
			node->nod_arg[e_val_boolean] = boolNod;
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
void CMP_mark_variant(CompilerScratch* csb, USHORT stream)
{
	if (csb->csb_current_nodes.isEmpty())
		return;

	for (RseOrExprNode* node = csb->csb_current_nodes.end() - 1;
		 node != csb->csb_current_nodes.begin(); --node)
	{
		if (node->rseNode)
		{
			if (node->rseNode->containsStream(stream))
				break;
			node->rseNode->flags |= RseNode::FLAG_VARIANT;
		}
		else if (node->exprNode)
			node->exprNode->nodFlags &= ~ExprNode::FLAG_INVARIANT;
	}
}


ValueExprNode* CMP_pass1(thread_db* tdbb, CompilerScratch* csb, ValueExprNode* node)
{
	if (!node)
		return NULL;

	return node->pass1(tdbb, csb);
}

BoolExprNode* CMP_pass1(thread_db* tdbb, CompilerScratch* csb, BoolExprNode* node)
{
	if (!node)
		return NULL;

	return node->pass1(tdbb, csb);
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
	ValueExprNode* sub;
	jrd_nod** ptr;
	USHORT stream;
	CompilerScratch::csb_repeat* tail;
	jrd_prc* procedure;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	if (!node)
		return node;

	AutoSetRestore<bool> autoValidateExpr(&csb->csb_validate_expr,
		csb->csb_validate_expr || node->nod_type == nod_validate);

	// if there is processing to be done before sub expressions, do it here

	switch (node->nod_type)
	{
	case nod_init_variable:
		{
			const USHORT n = (USHORT)(IPTR) node->nod_arg[e_init_var_id];
			vec<jrd_nod*>* vector = csb->csb_variables;
			if (!vector || n >= vector->count() || !(node->nod_arg[e_init_var_variable] = (*vector)[n]))
			{
				PAR_syntax_error(csb, "variable identifier");
			}
		}
		break;

	case nod_assignment:
		{
			sub = node->nod_arg[e_asgn_from]->asValue();
			FieldNode* fieldNode;

			if ((fieldNode = sub->as<FieldNode>()))
			{
				stream = fieldNode->fieldStream;
				jrd_fld* field = MET_get_field(csb->csb_rpt[stream].csb_relation, fieldNode->fieldId);

				if (field)
				{
					jrd_nod* nod = field->fld_missing_value ? PAR_make_node(tdbb, 1) : NULL;

					if (nod)
					{
						nod->nod_count = 0;
						nod->nod_type = nod_class_exprnode_jrd;
						nod->nod_arg[0] = reinterpret_cast<jrd_nod*>(field->fld_missing_value);
					}

					node->nod_arg[e_asgn_missing2] = nod;
				}
			}

			sub = node->nod_arg[e_asgn_to]->asValue();
			if (!(fieldNode = sub->as<FieldNode>()))
				break;

			stream = fieldNode->fieldStream;
			tail = &csb->csb_rpt[stream];
			jrd_fld* field = MET_get_field(tail->csb_relation, fieldNode->fieldId);

			if (!field)
				break;

			if (field->fld_missing_value)
			{
				jrd_nod* nod = PAR_make_node(tdbb, 1);
				nod->nod_count = 0;
				nod->nod_type = nod_class_exprnode_jrd;
				nod->nod_arg[0] = reinterpret_cast<jrd_nod*>(field->fld_missing_value);

				node->nod_arg[e_asgn_missing] = nod;
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

	case nod_class_recsrcnode_jrd:
		reinterpret_cast<RecordSourceNode*>(node->nod_arg[0])->pass1(tdbb, csb);
		break;

	case nod_src_info:
		node->nod_arg[e_src_info_node] = CMP_pass1(tdbb, csb, node->nod_arg[e_src_info_node]);
		return node;

	case nod_class_exprnode_jrd:
		{
			ExprNode* exprNode = reinterpret_cast<ExprNode*>(node->nod_arg[0]);
			node->nod_arg[0] = reinterpret_cast<jrd_nod*>(exprNode->pass1(tdbb, csb));
		}
		return node;

	case nod_class_stmtnode_jrd:
		{
			StmtNode* stmtNode = reinterpret_cast<StmtNode*>(node->nod_arg[0]);
			stmtNode->setNode(node);
			node->nod_arg[0] = reinterpret_cast<jrd_nod*>(stmtNode->pass1(tdbb, csb));
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
		sub = node->nod_arg[e_asgn_to]->asValue();
		FieldNode* fieldNode;

		if ((fieldNode = sub->as<FieldNode>()))
		{
			stream = fieldNode->fieldStream;
			tail = &csb->csb_rpt[stream];

			// assignments to the OLD context are prohibited for all trigger types
			if ((tail->csb_flags & csb_trigger) && stream == 0)
				ERR_post(Arg::Gds(isc_read_only_field));

			// assignments to the NEW context are prohibited for post-action triggers
			if ((tail->csb_flags & csb_trigger) && stream == 1 &&
				(csb->csb_g_flags & csb_post_trigger))
			{
				ERR_post(Arg::Gds(isc_read_only_field));
			}
		}
		else if (!(sub->is<ParameterNode>() || sub->is<VariableNode>() || sub->is<NullNode>()))
			ERR_post(Arg::Gds(isc_read_only_field));
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

				if (field->fld_source)
					new_id = field->fld_source->as<FieldNode>()->fieldId;
				else
					new_id = id;
			}
			else
				new_id = id;

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


// Copy items' information into appropriate node.
ItemInfo* CMP_pass2_validation(thread_db* tdbb, CompilerScratch* csb, const Item& item)
{
	ItemInfo itemInfo;
	return csb->csb_map_item_info.get(item, itemInfo) ?
		FB_NEW(*tdbb->getDefaultPool()) ItemInfo(*tdbb->getDefaultPool(), itemInfo) : NULL;
}


// Allocate and assign impure space for various nodes.
ExprNode* CMP_pass2(thread_db* tdbb, CompilerScratch* csb, ExprNode* node)
{
	NodeRefImpl<ExprNode> temp(&node);
	temp.pass2(tdbb, csb);
	return node;
}

// Allocate and assign impure space for various nodes.
ValueExprNode* CMP_pass2(thread_db* tdbb, CompilerScratch* csb, ValueExprNode* node)
{
	return static_cast<ValueExprNode*>(CMP_pass2(tdbb, csb, static_cast<ExprNode*>(node)));
}

// Allocate and assign impure space for various nodes.
BoolExprNode* CMP_pass2(thread_db* tdbb, CompilerScratch* csb, BoolExprNode* node)
{
	return static_cast<BoolExprNode*>(CMP_pass2(tdbb, csb, static_cast<ExprNode*>(node)));
}

// Allocate and assign impure space for various nodes.
jrd_nod* CMP_pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* const node, jrd_nod* parent)
{
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

	case nod_src_info:
		node->nod_arg[e_src_info_node] = CMP_pass2(tdbb, csb, node->nod_arg[e_src_info_node], node);
		return node;

	case nod_init_variable:
		node->nod_arg[e_init_var_info] = reinterpret_cast<jrd_nod*>(CMP_pass2_validation(tdbb,
			csb, Item(Item::TYPE_VARIABLE, (IPTR) node->nod_arg[e_init_var_id])));
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

	switch (node->nod_type)
	{
	case nod_class_recsrcnode_jrd:
		reinterpret_cast<RecordSourceNode*>(node->nod_arg[0])->pass2(tdbb, csb);
		break;

	case nod_assignment:
		CMP_pass2(tdbb, csb, node->nod_arg[e_asgn_missing2], node);
		break;

	case nod_block:
		node->nod_impure = CMP_impure(csb, sizeof(SLONG));
		break;

	case nod_dcl_variable:
		{
			const dsc* desc = (DSC*) (node->nod_arg + e_dcl_desc);
			node->nod_impure = CMP_impure(csb, sizeof(impure_value) + desc->dsc_length);
		}
		break;

	case nod_message:
		{
			const Format* format = (Format*) node->nod_arg[e_msg_format];
			fb_assert(format);
			node->nod_impure = CMP_impure(csb, FB_ALIGN(format->fmt_length, 2));
			node->nod_arg[e_msg_impure_flags] = (jrd_nod*)(IPTR)
				CMP_impure(csb, sizeof(USHORT) * format->fmt_count);
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
			node->nod_impure = CMP_impure(csb, sizeof(impure_state));
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
		node->nod_impure = CMP_impure(csb, sizeof(impure_state));
		break;

	case nod_erase:
		stream = (USHORT)(IPTR) node->nod_arg[e_erase_stream];
		csb->csb_rpt[stream].csb_flags |= csb_update;
		break;

	case nod_exec_into:
		node->nod_impure = CMP_impure(csb, sizeof(ExecuteStatement));
		csb->csb_exec_sta.push(node);
		break;

	case nod_exec_stmt:
		node->nod_impure = CMP_impure(csb, sizeof(void**));
		break;

	case nod_class_exprnode_jrd:
		{
			ExprNode* exprNode = CMP_pass2(tdbb, csb, reinterpret_cast<ExprNode*>(node->nod_arg[0]));
			node->nod_arg[0] = reinterpret_cast<jrd_nod*>(exprNode);
			break;
		}

	case nod_class_stmtnode_jrd:
		{
			StmtNode* stmtNode = reinterpret_cast<StmtNode*>(node->nod_arg[0]);
			stmtNode->setNode(node);
			node->nod_arg[0] = reinterpret_cast<jrd_nod*>(stmtNode->pass2(tdbb, csb));
		}
		break;

	default:
		// note: no fb_assert(false); here as too many nodes are missing
		break;
	}

	// finish up processing of record selection expressions

	if (rse_node)
	{
		RecordSource* const rsb = CMP_post_rse(tdbb, csb, rse_node);

		csb->csb_fors.add(rsb);

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


RecordSource* CMP_post_rse(thread_db* tdbb, CompilerScratch* csb, RseNode* rse)
{
/**************************************
 *
 *	C M P _ p o s t _ r s e
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
