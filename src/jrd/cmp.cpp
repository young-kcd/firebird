/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cmp.c
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include "../jrd/ibsetjmp.h"
#include <string.h>
#include <stdlib.h>				/* abort */
#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/val.h"
#include "../jrd/align.h"
#include "../jrd/lls.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/scl.h"
#include "../jrd/tra.h"
#include "../jrd/all.h"
#include "../jrd/lck.h"
#include "../jrd/irq.h"
#include "../jrd/drq.h"
#include "../jrd/license.h"
#include "../jrd/intl.h"
#include "../jrd/rng.h"
#include "../jrd/btr.h"
#include "../jrd/constants.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/idx_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/rng_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/scl_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/dbg_proto.h"	/* DBG_supervisor */
#include "../jrd/ExecuteStatement.h"

/* Pick up relation ids */

#define RELATION(name,id,ods)	id,
#define FIELD(symbol,name,id,update,ods,new_id,new_ods)
#define END_RELATION

typedef ENUM rids {
#include "../jrd/relations.h"
rel_MAX} RIDS;

#undef RELATION
#undef FIELD
#undef END_RELATION

/* InterBase provides transparent conversion from string to date in
 * contexts where it makes sense.  This macro checks a descriptor to
 * see if it is something that *could* represent a date value
 */
#define COULD_BE_DATE(d)	((DTYPE_IS_DATE((d).dsc_dtype)) || ((d).dsc_dtype <= dtype_any_text))

/* One of d1,d2 is time, the other is date */
#define IS_DATE_AND_TIME(d1,d2)	\
  ((((d1).dsc_dtype==dtype_sql_time)&&((d2).dsc_dtype==dtype_sql_date)) || \
   (((d2).dsc_dtype==dtype_sql_time)&&((d1).dsc_dtype==dtype_sql_date)))

#define REQ_TAIL		sizeof (((JRD_REQ) 0)->req_rpb[0])
#define MAP_LENGTH		256

/* RITTER - changed HP10 to HPUX */
#if defined (HPUX) && defined (SUPERSERVER)
#define MAX_RECURSION		96
#endif

#ifndef MAX_RECURSION
#define MAX_RECURSION		128
#endif

#define MAX_REQUEST_SIZE	10485760	// 10 MB - just to be safe

#ifdef SHLIB_DEFS
#undef access
#endif

static UCHAR *alloc_map(TDBB, CSB *, USHORT);
static JRD_NOD catenate_nodes(TDBB, LLS);
static JRD_NOD copy(TDBB, CSB *, JRD_NOD, UCHAR *, USHORT, JRD_NOD, USHORT);
static void expand_view_nodes(TDBB, CSB, USHORT, LLS *, NOD_T);
static void ignore_dbkey(TDBB, CSB, RSE, JRD_REL);
static JRD_NOD make_defaults(TDBB, CSB *, USHORT, JRD_NOD);
static JRD_NOD make_validation(TDBB, CSB *, USHORT);
static JRD_NOD pass1(TDBB, CSB *, JRD_NOD, JRD_REL, USHORT, BOOLEAN);
static void pass1_erase(TDBB, CSB *, JRD_NOD);
static JRD_NOD pass1_expand_view(TDBB, CSB, USHORT, USHORT, USHORT);
static void pass1_modify(TDBB, CSB *, JRD_NOD);
static RSE pass1_rse(TDBB, CSB *, RSE, JRD_REL, USHORT);
static void pass1_source(TDBB, CSB *, RSE, JRD_NOD, JRD_NOD *, LLS *, JRD_REL, USHORT);
static JRD_NOD pass1_store(TDBB, CSB *, JRD_NOD);
static JRD_NOD pass1_update(TDBB, CSB *, JRD_REL, TRIG_VEC, USHORT, USHORT, USHORT, JRD_REL,
						USHORT);
static JRD_NOD pass2(TDBB, CSB, JRD_NOD, JRD_NOD);
static void pass2_rse(TDBB, CSB, RSE);
static JRD_NOD pass2_union(TDBB, CSB, JRD_NOD);
static void plan_check(CSB, RSE);
static void plan_set(CSB, RSE, JRD_NOD);
static void post_procedure_access(TDBB, CSB, JRD_PRC);
static RSB post_rse(TDBB, CSB, RSE);
static void post_trigger_access(CSB, JRD_REL, ExternalAccess::exa_act, JRD_REL);
static void process_map(TDBB, CSB, JRD_NOD, FMT *);
static BOOLEAN stream_in_rse(USHORT, RSE);
static SSHORT strcmp_space(TEXT *, TEXT *);
static void build_external_access(TDBB tdbb, ExternalAccessList& list, JRD_REQ request);
static void verify_trigger_access(TDBB tdbb, JRD_REL owner_relation, trig_vec* triggers, JRD_REL view);


#ifdef PC_ENGINE
static USHORT base_stream(CSB, JRD_NOD *, BOOLEAN);
#endif


inline static char* clone_cstring(JrdMemoryPool* pool, const char* source) {
	if (!source) return NULL;
	char *result = FB_NEW(*pool) char[strlen(source)+1];
    strcpy(result, source);
    return result;
}

int DLL_EXPORT CMP_clone_active(JRD_REQ request)
{
/**************************************
 *
 *	C M P _ c l o n e _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	Determine if a request or any of its clones are active.
 *
 **************************************/
	VEC vector;
	vec::iterator sub_req, end;

	DEV_BLKCHK(request, type_req);

	if (request->req_flags & req_in_use)
		return TRUE;

	if ( (vector = request->req_sub_requests) )
		for (sub_req = vector->begin(), end = vector->end();
			 sub_req < end; sub_req++)
			if (*sub_req && ((JRD_REQ)(*sub_req))->req_flags & req_in_use)
				return TRUE;

	return FALSE;
}


JRD_NOD DLL_EXPORT CMP_clone_node(TDBB tdbb, CSB csb, JRD_NOD node)
{
/**************************************
 *
 *	C M P _ c l o n e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Clone a value node for the optimizer.  Make a copy of the node
 *	(if necessary) and assign impure space.
 *
 **************************************/
	JRD_NOD clone;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	if (node->nod_type == nod_argument)
		return node;

	clone = copy(tdbb, &csb, node, NULL, 0, NULL, FALSE);
	pass2(tdbb, csb, clone, 0);

	return clone;
}


static void build_external_access(TDBB tdbb, ExternalAccessList& list, JRD_REQ request)
{
/**************************************
 *
 *	b u i l d _ e x t e r n a l _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *  Recursively walk external dependencies (procedures, triggers) for request to assemble full
 *  list of requests it depends on
 *
 **************************************/
	for (ExternalAccess *item = request->req_external.begin(); item < request->req_external.end(); item++)
	{
		int i;
		if (list.find(*item, i)) continue;
		list.insert(i, *item);

		// Add externals recursively
		if (item->exa_action == ExternalAccess::exa_procedure) {
			jrd_prc* prc = MET_lookup_procedure_id(tdbb, item->exa_prc_id, false, false, 0);
			if (prc && prc->prc_request)
				build_external_access(tdbb, list, prc->prc_request);
		} else {
			jrd_rel* relation = MET_lookup_relation_id(tdbb, item->exa_rel_id, false);

			if (!relation) continue;

			trig_vec *vec1, *vec2;
			switch(item->exa_action) {
			case ExternalAccess::exa_insert:
				vec1 = relation->rel_pre_store;
				vec2 = relation->rel_post_store;
				break;
			case ExternalAccess::exa_update:
				vec1 = relation->rel_pre_modify;
				vec2 = relation->rel_post_modify;
				break;
			case ExternalAccess::exa_delete:
				vec1 = relation->rel_pre_erase;
				vec2 = relation->rel_post_erase;
				break;
			default:
				continue; // should never happen, silence the compiler
			}
			trig_vec::iterator ptr;
			if (vec1) {
				ptr = vec1->begin();
				for (const trig_vec::const_iterator end = vec1->end(); ptr < end; ptr++)
				{
					ptr->compile(tdbb);
					if (ptr->request)
						build_external_access(tdbb, list, ptr->request);
				}
			}
			if (vec2) {
				ptr = vec2->begin();
				for (const trig_vec::const_iterator end = vec2->end(); ptr < end; ptr++)
				{
					ptr->compile(tdbb);
					if (ptr->request)
						build_external_access(tdbb, list, ptr->request);
				}
			}
		}
	}
}


static void verify_trigger_access(TDBB tdbb, JRD_REL owner_relation, trig_vec* triggers, JRD_REL view)
{
/**************************************
 *
 *	v e r i f y _ t r i g g e r _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *  Check that we have enough rights to access all resources this list of triggers touches
 *
 **************************************/
	if (!triggers) {
		return;
	}

	trig_vec::iterator ptr = triggers->begin();
	for (const trig_vec::const_iterator end = triggers->end(); ptr < end; ptr++)
	{
		ptr->compile(tdbb);
		if (!ptr->request) continue;


		for (const AccessItem* access = ptr->request->req_access.begin();
			 access < ptr->request->req_access.end(); access++)
		{
			// If this is not a system relation, we don't post access check if:
			//
			// - The table being checked is the owner of the trigger that's accessing it.
			// - The field being checked is owned by the same table than the trigger
			//   that's accessing the field.
			// - Since the trigger name comes in the triggers vector of the table and each
			//   trigger can be owned by only one table for now, we know for sure that
			//   it's a trigger defined on our target table.

			if (!(owner_relation->rel_flags & REL_system))
			{
				if (!strcmp(access->acc_type, object_table)
					&& !strcmp(access->acc_name, owner_relation->rel_name))
				{
					continue;
				}
				if (!strcmp(access->acc_type, object_column)
					&& (MET_lookup_field(tdbb, owner_relation, access->acc_name, access->acc_security_name) >= 0
					|| MET_relation_default_class(tdbb, owner_relation->rel_name, access->acc_security_name)))
				{
					continue;
				}
			}
			// a direct access to an object from this trigger
			const SCL sec_class = SCL_get_class(access->acc_security_name);
			SCL_check_access(sec_class,
							(access->acc_view_id) ? access->acc_view_id :
								(view ? view->rel_id : 0),
							ptr->request->req_trg_name, 0, access->acc_mask,
							access->acc_type, access->acc_name);
		}
	}
}


void CMP_verify_access(TDBB tdbb, JRD_REQ request)
{
/**************************************
 *
 *	C M P _ v e r i f y _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *  Check that we have enough rights to access all resources this request touches including
 *  resources it used indirectecty via procedures or triggers
 *
 **************************************/
	ExternalAccessList external(*tdbb->tdbb_default);
	build_external_access(tdbb, external, request);

	for (ExternalAccess* item = external.begin(); item < external.end(); item++) {
		if (item->exa_action == ExternalAccess::exa_procedure) {
			jrd_prc* prc = MET_lookup_procedure_id(tdbb, item->exa_prc_id, false, false, 0);
			if (!prc->prc_request) continue;
			for (const AccessItem* access = prc->prc_request->req_access.begin();
				 access < prc->prc_request->req_access.end();
				 access++)
			{
				const SCL sec_class = SCL_get_class(access->acc_security_name);
				SCL_check_access(sec_class, access->acc_view_id, NULL, reinterpret_cast <char *> (prc->prc_name->str_data),
								 access->acc_mask, access->acc_type, access->acc_name);
			}
		} else {
			jrd_rel* relation = MET_lookup_relation_id(tdbb, item->exa_rel_id, false);
			jrd_rel* view = NULL;
			if (item->exa_view_id)
				view = MET_lookup_relation_id(tdbb, item->exa_view_id, false);

			if (!relation) continue;

			switch(item->exa_action) {
			case ExternalAccess::exa_insert:
				verify_trigger_access(tdbb, relation, relation->rel_pre_store, view);
				verify_trigger_access(tdbb, relation, relation->rel_post_store, view);
				break;
			case ExternalAccess::exa_update:
				verify_trigger_access(tdbb, relation, relation->rel_pre_modify, view);
				verify_trigger_access(tdbb, relation, relation->rel_post_modify, view);
				break;
			case ExternalAccess::exa_delete:
				verify_trigger_access(tdbb, relation, relation->rel_pre_erase, view);
				verify_trigger_access(tdbb, relation, relation->rel_post_erase, view);
				break;
			default:
				continue; // should never happen, silence the compiler
			}
		}
	}

	for (const AccessItem* access = request->req_access.begin(); access < request->req_access.end();
		access++)
	{
		const SCL sec_class = SCL_get_class(access->acc_security_name);
		SCL_check_access(sec_class, access->acc_view_id, NULL, NULL,
						 access->acc_mask, access->acc_type, access->acc_name);
	}
}


JRD_REQ DLL_EXPORT CMP_clone_request(TDBB tdbb,
								 JRD_REQ request, USHORT level, BOOLEAN validate)
{
/**************************************
 *
 *	C M P _ c l o n e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Get the incarnation of the request appropriate for a given level.
 *	If the incarnation doesn't exist, clone the request.
 *
 **************************************/
	JRD_REQ clone;
	RPB *rpb1, *rpb2, *end;
	USHORT n;
	SCL class_;
	JRD_PRC procedure;
	TEXT *prc_sec_name;

	DEV_BLKCHK(request, type_req);

	SET_TDBB(tdbb);

/* Find the request if we've got it. */

	if (!level)
		return request;

	VEC vector = request->req_sub_requests;

	if (vector && level < vector->count() && (clone = (JRD_REQ) (*vector)[level]))
		return clone;

/* We need to clone the request -- find someplace to put it */

	if (validate) {
		if ( (procedure = request->req_procedure) ) {
			prc_sec_name = (procedure->prc_security_name ?
							(TEXT *) procedure->
							prc_security_name->str_data : (TEXT *) 0);
			class_ = SCL_get_class(prc_sec_name);
			SCL_check_access(class_, 0, 0,
							 0, SCL_execute, object_procedure,
							 reinterpret_cast <
							 char *>(procedure->prc_name->str_data));
		}
		CMP_verify_access(tdbb, request);
	}

	vector = request->req_sub_requests =
		vec::newVector(*request->req_pool, request->req_sub_requests, level + 1);

/* Clone the request */

	n =
		(USHORT) ((request->req_impure_size - REQ_SIZE + REQ_TAIL - 1) /
				  REQ_TAIL);
 	clone = FB_NEW_RPT(*request->req_pool, n) jrd_req(request->req_pool);
	(*vector)[level] = (BLK) clone;
	clone->req_attachment = tdbb->tdbb_attachment;
	clone->req_count = request->req_count;
	clone->req_pool = request->req_pool;
	clone->req_impure_size = request->req_impure_size;
	clone->req_top_node = request->req_top_node;
	clone->req_trg_name = request->req_trg_name;
	clone->req_flags = request->req_flags & REQ_FLAGS_CLONE_MASK;
	clone->req_last_xcp.xcp_type = request->req_last_xcp.xcp_type;
	clone->req_last_xcp.xcp_code = request->req_last_xcp.xcp_code;
	clone->req_last_xcp.xcp_msg = request->req_last_xcp.xcp_msg;

	// We are cloning full lists here, not assigning pointers
 	clone->req_invariants = request->req_invariants;
 	clone->req_fors = request->req_fors;

	rpb1 = clone->req_rpb;
	end = rpb1 + clone->req_count;

	for (rpb2 = request->req_rpb; rpb1 < end; rpb1++, rpb2++) {
		if (rpb2->rpb_stream_flags & RPB_s_update)
			rpb1->rpb_stream_flags |= RPB_s_update;
		rpb1->rpb_relation = rpb2->rpb_relation;
	}

	return clone;
}


JRD_REQ DLL_EXPORT CMP_compile(USHORT blr_length,
						   UCHAR * blr, USHORT internal_flag)
{
/**************************************
 *
 *	C M P _ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *	Compile a BLR request.
 *	Wrapper for CMP_compile2 - an API change
 *      was made for CMP_compile, but as calls to this
 *	are generated by gpre it's necessary to have a
 *	wrapper function to keep the build from breaking.
 *	This function can be removed after the next full
 *	product build is completed.
 *	1997-Jan-20 David Schnepper 
 *
 **************************************/
	return CMP_compile2(GET_THREAD_DATA, blr, internal_flag);
}


JRD_REQ DLL_EXPORT CMP_compile2(TDBB tdbb, UCHAR* blr, USHORT internal_flag)
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
	
	JRD_REQ request = 0;

	SET_TDBB(tdbb);

	JrdMemoryPool* old_pool = tdbb->tdbb_default;
	/* 26.09.2002 Nickolay Samofatov: default memory pool will become statement pool
	  and will be freed by CMP_release	*/
	JrdMemoryPool* new_pool = JrdMemoryPool::createPool();
	tdbb->tdbb_default = new_pool;

	try {

		CSB csb = PAR_parse(tdbb, blr, internal_flag);
		request = CMP_make_request(tdbb, &csb);

		if (internal_flag) {
			request->req_flags |= req_internal;
		}

		CMP_verify_access(tdbb, request);

		delete csb;
		tdbb->tdbb_default = old_pool;

	}
	catch (const std::exception&) {
		tdbb->tdbb_default = old_pool;
		if (request) {
			CMP_release(tdbb, request);
		} else if (new_pool) {
			// TMN: Are we not to release the pool, just beqause
			// we have a request?!
			// Nickolay Samofatv: It well be freed by CMP_release otherwise
			JrdMemoryPool::deletePool(new_pool);
		}
		ERR_punt();
	}

	return request;
}


csb_repeat* DLL_EXPORT CMP_csb_element(CSB* csb, USHORT element)
{
/**************************************
 *
 *	C M P _ c s b _ e l e m e n t
 *
 **************************************
 *
 * Functional description
 *	Find tail element of compile scratch block.  If the csb isn't big
 *	enough, extend it.
 *
 **************************************/

	DEV_BLKCHK(*csb, type_csb);

	if (element >= (*csb)->csb_rpt.size()) {
		(*csb)->csb_rpt.resize(element + 5);
		(*csb)->csb_count = element + 5;
	}

	return &(*csb)->csb_rpt[element];
}


void DLL_EXPORT CMP_expunge_transaction(JRD_TRA transaction)
{
/**************************************
 *
 *	C M P _ e x p u n g e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Get rid of all references to a given transaction in existing
 *	requests.
 *
 **************************************/
	VEC vector;
	JRD_REQ request;
	vec::iterator sub, end;

	DEV_BLKCHK(transaction, type_tra);

	for (request = transaction->tra_attachment->att_requests;
		 request; request = request->req_request) {
		if (request->req_transaction == transaction)
			request->req_transaction = NULL;
		if ( (vector = request->req_sub_requests) )
			for (sub = vector->begin(), end = vector->end();
				 sub < end; sub++)
				if (*sub && ((JRD_REQ)(*sub))->req_transaction == transaction)
					((JRD_REQ)(*sub))->req_transaction = NULL;
	}
}


JRD_REQ DLL_EXPORT CMP_find_request(TDBB tdbb, USHORT id, USHORT which)
{
/**************************************
 *
 *	C M P _ f i n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Find an inactive incarnation of a system request.  If necessary,
 *	clone it.
 *
 **************************************/
	DBB dbb;
	JRD_REQ request, clone;
	USHORT n;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);


/* If the request hasn't been compiled or isn't active, 
   there're nothing to do */

	THD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
	if ((which == IRQ_REQUESTS && !(request = (JRD_REQ) REQUEST(id))) ||
		(which == DYN_REQUESTS && !(request = (JRD_REQ) DYN_REQUEST(id))) ||
		!(request->req_flags & (req_active | req_reserved))) {
		if (request)
			request->req_flags |= req_reserved;
		THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
		return request;
	}

/* Request exists and is in use.  Look for clones until we find
   one that is available */

	for (n = 1;; n++) {
		if (n > MAX_RECURSION) {
			THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
			ERR_post(gds_no_meta_update,
					 gds_arg_gds, gds_req_depth_exceeded,
					 gds_arg_number, (SLONG) MAX_RECURSION, 0);
			/* Msg363 "request depth exceeded. (Recursive definition?)" */
		}
		clone = CMP_clone_request(tdbb, request, n, FALSE);
		if (!(clone->req_flags & (req_active | req_reserved))) {
			clone->req_flags |= req_reserved;
			THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
			return clone;
		}
	}
}


void DLL_EXPORT CMP_fini(TDBB tdbb)
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

	CMP_shutdown_database(tdbb);
}


FMT DLL_EXPORT CMP_format(TDBB tdbb, CSB csb, USHORT stream)
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
	csb_repeat *tail;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);

	tail = &csb->csb_rpt[stream];

	if (tail->csb_format)
		return tail->csb_format;

	if (tail->csb_relation)
		return tail->csb_format = MET_current(tdbb, tail->csb_relation);
	else if (tail->csb_procedure)
		return tail->csb_format = tail->csb_procedure->prc_format;

	IBERROR(222);				/* msg 222 bad blr -- invalid stream */
	return ((FMT) NULL);
}


void DLL_EXPORT CMP_get_desc(
							 TDBB tdbb,
							 CSB csb, JRD_NOD node, DSC * desc)
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
	USHORT dtype, dtype1, dtype2;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	switch (node->nod_type) {
	case nod_max:
	case nod_min:
	case nod_from:
		CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		return;

	case nod_agg_total:
	case nod_agg_total_distinct:
	case nod_total:
		if (node->nod_type == nod_total)
			CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		else
			CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		switch (dtype = desc->dsc_dtype) {
		case dtype_short:
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = sizeof(SLONG);
			node->nod_scale = desc->dsc_scale;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;

		case dtype_null:
			desc->dsc_dtype = dtype_null;
			desc->dsc_length = 0;
			node->nod_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;

		case dtype_long:
		case dtype_int64:
		case dtype_real:
		case dtype_double:
#ifdef VMS
		case dtype_d_float:
#endif
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
			assert(FALSE);
			/* FALLINTO */
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:
		case dtype_blob:
		case dtype_array:
			/* break to error reporting code */
			break;
		}
		break;

	case nod_agg_total2:
	case nod_agg_total_distinct2:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		switch (dtype = desc->dsc_dtype) {
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			desc->dsc_dtype = dtype_int64;
			desc->dsc_length = sizeof(SINT64);
			node->nod_scale = desc->dsc_scale;
			desc->dsc_flags = 0;
			return;

		case dtype_null:
			desc->dsc_dtype = dtype_null;
			desc->dsc_length = 0;
			node->nod_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;

		case dtype_real:
		case dtype_double:
#ifdef VMS
		case dtype_d_float:
#endif
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
			assert(FALSE);
			/* FALLINTO */
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:
		case dtype_blob:
		case dtype_array:
			/* break to error reporting code */
			break;
		}
		break;


	case nod_prot_mask:
	case nod_null:
	case nod_agg_count:
	case nod_agg_count2:
	case nod_agg_count_distinct:
	case nod_count2:
	case nod_count:
	case nod_gen_id:
	case nod_lock_state:
#ifdef PC_ENGINE
	case nod_lock_record:
	case nod_lock_relation:
	case nod_seek:
	case nod_seek_no_warn:
	case nod_crack:
#endif
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

#ifdef PC_ENGINE
	case nod_begin_range:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype = ttype_ascii;
		desc->dsc_scale = 0;
		desc->dsc_length = RANGE_NAME_LENGTH;
		desc->dsc_flags = 0;
		return;
#endif

	case nod_field:
		{
			FMT format;
			USHORT id;

			id = (USHORT) node->nod_arg[e_fld_id];
			format =
				CMP_format(tdbb, csb, (USHORT) node->nod_arg[e_fld_stream]);
			if (id >= format->fmt_count) {
				desc->dsc_dtype = dtype_null;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
			}
			else
				*desc = format->fmt_desc[id];
			return;
		}

	case nod_scalar:
		{
			JRD_NOD sub;
			JRD_REL relation;
			USHORT id;
			JRD_FLD field;
			ARR array;

			sub = node->nod_arg[e_scl_field];
			relation =
				csb->csb_rpt[(USHORT) sub->
							 nod_arg[e_fld_stream]].csb_relation;
			id = (USHORT) sub->nod_arg[e_fld_id];
			field = MET_get_field(relation, id);
			if (!field || !(array = field->fld_array))
				IBERROR(223);	/* msg 223 argument of scalar operation must be an array */
			*desc = array->arr_desc.ads_rpt[0].ads_desc;
			return;
		}

	case nod_divide:
		{
			DSC desc1, desc2;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
			/* For compatibility with older versions of the product, we accept
			   text types for division in blr_version4 (dialect <= 1) only. */
			if (!(DTYPE_CAN_DIVIDE(desc1.dsc_dtype) ||
				  DTYPE_IS_TEXT(desc1.dsc_dtype))) {
				if (desc1.dsc_dtype != dtype_null)
					break;		/* error, dtype not supported by arithmetic */
			}
			if (!(DTYPE_CAN_DIVIDE(desc2.dsc_dtype) ||
				  DTYPE_IS_TEXT(desc2.dsc_dtype))) {
				if (desc2.dsc_dtype != dtype_null)
					break;		/* error, dtype not supported by arithmetic */
			}
		}
		desc->dsc_dtype = DEFAULT_DOUBLE;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

	case nod_agg_average:
	case nod_agg_average_distinct:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		/* FALL INTO */
	case nod_average:
		if (node->nod_type == nod_average)
			CMP_get_desc(tdbb, csb, node->nod_arg[e_stat_value], desc);
		if (!DTYPE_CAN_AVERAGE(desc->dsc_dtype)) {
			if (desc->dsc_dtype != dtype_null)
				break;
		}
		desc->dsc_dtype = DEFAULT_DOUBLE;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		desc->dsc_flags = 0;
		return;

		/* In 6.0, the AVERAGE of an exact numeric type is int64 with the
		   same scale.  Only AVERAGE on an approximate numeric type can
		   return a double. */

	case nod_agg_average2:
	case nod_agg_average_distinct2:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		/* In V6, the average of an exact type is computed in SINT64,
		   rather than double as in prior releases. */
		switch (dtype = desc->dsc_dtype) {
		case dtype_short:
		case dtype_long:
		case dtype_int64:
			desc->dsc_dtype = dtype_int64;
			desc->dsc_length = sizeof(SINT64);
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			node->nod_scale = desc->dsc_scale;
			return;

		case dtype_null:
			desc->dsc_dtype = dtype_null;
			desc->dsc_length = 0;
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			return;

		default:
			if (!DTYPE_CAN_AVERAGE(desc->dsc_dtype))
				break;
			desc->dsc_dtype = DEFAULT_DOUBLE;
			desc->dsc_length = sizeof(double);
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
			node->nod_flags |= nod_double;
			return;
		}
		break;

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
			   it turns into either integers or longs (with scale).
			 */

			dtype1 = desc1.dsc_dtype;
			if (dtype_int64 == dtype1)
				dtype1 = dtype_double;
			dtype2 = desc2.dsc_dtype;
			if (dtype_int64 == dtype2)
				dtype2 = dtype_double;

			if ((dtype1 == dtype_text) || (dtype2 == dtype_text))
				dtype = MAX(MAX(dtype1, dtype2), (UCHAR) DEFAULT_DOUBLE);
			else
				dtype = MAX(dtype1, dtype2);

			switch (dtype) {
			case dtype_short:
				desc->dsc_dtype = dtype_long;
				desc->dsc_length = sizeof(SLONG);
				if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
					IS_DTYPE_ANY_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
				else
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_sql_date:
			case dtype_sql_time:
				if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
					IS_DTYPE_ANY_TEXT(desc2.dsc_dtype))
						ERR_post(gds_expression_eval_err, 0);
				/* FALL INTO */
			case dtype_timestamp:
				node->nod_flags |= nod_date;

				assert(DTYPE_IS_DATE(desc1.dsc_dtype) ||
					   DTYPE_IS_DATE(desc2.dsc_dtype));

				if (COULD_BE_DATE(desc1) && COULD_BE_DATE(desc2)) {
					if (node->nod_type == nod_subtract) {
						/* <any date> - <any date> */

						/* Legal permutations are:
						   <timestamp> - <timestamp>
						   <timestamp> - <date>
						   <date> - <date>
						   <date> - <timestamp>
						   <time> - <time>
						   <timestamp> - <string>
						   <string> - <timestamp> 
						   <string> - <string>   */

						if (IS_DTYPE_ANY_TEXT(dtype1))
							dtype = dtype_timestamp;
						else if (IS_DTYPE_ANY_TEXT(dtype2))
							dtype = dtype_timestamp;
						else if (dtype1 == dtype2)
							dtype = dtype1;
						else if ((dtype1 == dtype_timestamp) &&
								 (dtype2 == dtype_sql_date))
								dtype = dtype_timestamp;
						else if ((dtype2 == dtype_timestamp) &&
								 (dtype1 == dtype_sql_date))
								dtype = dtype_timestamp;
						else
							ERR_post(gds_expression_eval_err, 0);

						if (dtype == dtype_sql_date) {
							desc->dsc_dtype = dtype_long;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else if (dtype == dtype_sql_time) {
							desc->dsc_dtype = dtype_long;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale =
								ISC_TIME_SECONDS_PRECISION_SCALE;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else {
							assert(dtype == dtype_timestamp);
							desc->dsc_dtype = DEFAULT_DOUBLE;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
					}
					else if (IS_DATE_AND_TIME(desc1, desc2)) {
						/* <date> + <time> */
						/* <time> + <date> */
						desc->dsc_dtype = dtype_timestamp;
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
						/* <date> + <date> */
						ERR_post(gds_expression_eval_err, 0);
				}
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
						 /* <date> +/- <non-date> */
						 (node->nod_type == nod_add))
					/* <non-date> + <date> */
				{
					desc->dsc_dtype = desc1.dsc_dtype;
					if (!DTYPE_IS_DATE(desc->dsc_dtype))
						desc->dsc_dtype = desc2.dsc_dtype;
					assert(DTYPE_IS_DATE(desc->dsc_dtype));
					desc->dsc_length = type_lengths[desc->dsc_dtype];
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
				}
				else
					/* <non-date> - <date> */
					ERR_post(gds_expression_eval_err, 0);
				return;

			case dtype_text:
			case dtype_cstring:
			case dtype_varying:
			case dtype_long:
			case dtype_real:
			case dtype_double:
#ifdef VMS
			case dtype_d_float:
#endif
				node->nod_flags |= nod_double;
				desc->dsc_dtype = DEFAULT_DOUBLE;
				desc->dsc_length = sizeof(double);
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_null:
				desc->dsc_dtype = dtype_null;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_quad:
				node->nod_flags |= nod_quad;
				desc->dsc_dtype = dtype_quad;
				desc->dsc_length = sizeof(SQUAD);
				if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
					IS_DTYPE_ANY_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
				else
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
#ifdef NATIVE_QUAD
				return;
#endif
			default:
				assert(FALSE);
				/* FALLINTO */
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
			dtype1 = desc1.dsc_dtype;
			dtype2 = desc2.dsc_dtype;

			/* Because dtype_int64 > dtype_double, we cannot just use the MAX macro to set
			   the result dtype.  The rule is that two exact numeric operands yield an int64
			   result, while an approximate numeric and anything yield a double result. */

			if (DTYPE_IS_EXACT(desc1.dsc_dtype)
				&& DTYPE_IS_EXACT(desc2.dsc_dtype))
				dtype = dtype_int64;

			else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype) &&
					 DTYPE_IS_NUMERIC(desc2.dsc_dtype)) dtype = dtype_double;

			else {
				/* mixed numeric and non-numeric: */

				assert(COULD_BE_DATE(desc1) || COULD_BE_DATE(desc2));

				/* The MAX(dtype) rule doesn't apply with dtype_int64 */

				if (dtype_int64 == dtype1)
					dtype1 = dtype_double;
				if (dtype_int64 == dtype2)
					dtype2 = dtype_double;

				dtype = MAX(dtype1, dtype2);
			}

			switch (dtype) {
			case dtype_timestamp:
			case dtype_sql_date:
			case dtype_sql_time:
				node->nod_flags |= nod_date;

				assert(DTYPE_IS_DATE(desc1.dsc_dtype) ||
					   DTYPE_IS_DATE(desc2.dsc_dtype));

				if ((DTYPE_IS_DATE(dtype1) || (dtype1 == dtype_null)) &&
					(DTYPE_IS_DATE(dtype2) || (dtype2 == dtype_null))) {
					if (node->nod_type == nod_subtract2) {
						/* <any date> - <any date> */

						/* Legal permutations are:
						   <timestamp> - <timestamp>
						   <timestamp> - <date>
						   <date> - <date>
						   <date> - <timestamp>
						   <time> - <time> */

						if (dtype1 == dtype_null)
							dtype1 = dtype2;
						else if (dtype2 == dtype_null)
							dtype2 = dtype1;
						if (dtype1 == dtype2)
							dtype = dtype1;
						else if ((dtype1 == dtype_timestamp) &&
								 (dtype2 == dtype_sql_date))
								dtype = dtype_timestamp;
						else if ((dtype2 == dtype_timestamp) &&
								 (dtype1 == dtype_sql_date))
								dtype = dtype_timestamp;
						else
							ERR_post(gds_expression_eval_err, 0);

						if (dtype == dtype_sql_date) {
							desc->dsc_dtype = dtype_long;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else if (dtype == dtype_sql_time) {
							desc->dsc_dtype = dtype_long;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale =
								ISC_TIME_SECONDS_PRECISION_SCALE;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
						else {
							assert(dtype == dtype_timestamp
								   || dtype == dtype_null);
							desc->dsc_dtype = DEFAULT_DOUBLE;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
					}
					else if (IS_DATE_AND_TIME(desc1, desc2)) {
						/* <date> + <time> */
						/* <time> + <date> */
						desc->dsc_dtype = dtype_timestamp;
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
						/* <date> + <date> */
						ERR_post(gds_expression_eval_err, 0);
				}
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
						 /* <date> +/- <non-date> */
						 (node->nod_type == nod_add2))
					/* <non-date> + <date> */
				{
					desc->dsc_dtype = desc1.dsc_dtype;
					if (!DTYPE_IS_DATE(desc->dsc_dtype))
						desc->dsc_dtype = desc2.dsc_dtype;
					assert(DTYPE_IS_DATE(desc->dsc_dtype));
					desc->dsc_length = type_lengths[desc->dsc_dtype];
					desc->dsc_scale = 0;
					desc->dsc_sub_type = 0;
					desc->dsc_flags = 0;
				}
				else
					/* <non-date> - <date> */
					ERR_post(gds_expression_eval_err, 0);
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
				if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
					IS_DTYPE_ANY_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
				else
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type =
					MAX(desc1.dsc_sub_type, desc2.dsc_sub_type);
				desc->dsc_flags = 0;
				return;

			case dtype_null:
				desc->dsc_dtype = dtype_null;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_quad:
				node->nod_flags |= nod_quad;
				desc->dsc_dtype = dtype_quad;
				desc->dsc_length = sizeof(SQUAD);
				if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
					IS_DTYPE_ANY_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
				else
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
#ifdef NATIVE_QUAD
				return;
#endif
			default:
				assert(FALSE);
				/* FALLINTO */
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
			dtype =
				DSC_multiply_blr4_result[desc1.dsc_dtype][desc2.dsc_dtype];

			switch (dtype) {
			case dtype_long:
				desc->dsc_dtype = dtype_long;
				desc->dsc_length = sizeof(SLONG);
				desc->dsc_scale = node->nod_scale =
					NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_double:
#ifdef VMS
			case dtype_d_float:
#endif
				node->nod_flags |= nod_double;
				desc->dsc_dtype = DEFAULT_DOUBLE;
				desc->dsc_length = sizeof(double);
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_null:
				desc->dsc_dtype = dtype_null;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			default:
				assert(FALSE);
				/* FALLINTO */
			case DTYPE_CANNOT:
				/* break to error reporting code */
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


			switch (dtype) {
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
				desc->dsc_scale = node->nod_scale =
					NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
				desc->dsc_sub_type =
					MAX(desc1.dsc_sub_type, desc2.dsc_sub_type);
				desc->dsc_flags = 0;
				return;

			case dtype_null:
				desc->dsc_dtype = dtype_null;
				desc->dsc_length = 0;
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			default:
				assert(FALSE);
				/* FALLINTO */
			case DTYPE_CANNOT:
				/* break to error reporting code */
				break;
			}
		}
		break;

	case nod_concatenate:
		{
			DSC desc1, desc2;
			ULONG rc_len;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
			desc->dsc_dtype = dtype_text;
			if (desc1.dsc_dtype <= dtype_varying) {
			    rc_len = DSC_string_length(&desc1);
				desc->dsc_ttype = desc1.dsc_ttype;
			}
			else {
			    rc_len = DSC_convert_to_text_length(desc1.dsc_dtype);
				desc->dsc_ttype = ttype_ascii;
			}
			if (desc2.dsc_dtype <= dtype_varying) {
				rc_len += DSC_string_length (&desc2);
				if (((desc->dsc_ttype == CS_ASCII) || (desc->dsc_ttype == CS_NONE)) &&
					(desc2.dsc_ttype != CS_NONE)) 
				{
					desc->dsc_ttype = desc2.dsc_ttype;
				}
			}
			else {
				rc_len += DSC_convert_to_text_length(desc2.dsc_dtype);
			}
			/* error() is a local routine in par.c, so we use plain ERR_post. */
			if (rc_len > MAX_FORMAT_SIZE)
				ERR_post(gds_imp_exc, gds_arg_gds, gds_blktoobig, 0);
			desc->dsc_length = static_cast<USHORT>(rc_len);
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
			return;
		}

	case nod_upcase:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		if (desc->dsc_dtype > dtype_varying) {
			desc->dsc_length = DSC_convert_to_text_length(desc->dsc_dtype);
			desc->dsc_dtype = dtype_text;
			desc->dsc_ttype = ttype_ascii;
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		return;

	case nod_dbkey:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype = ttype_binary;
		desc->dsc_length = 8;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_rec_version:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype = ttype_binary;
		desc->dsc_length = 4;
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
		desc->dsc_ttype = ttype_metadata;
		desc->dsc_length = USERNAME_LENGTH;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_internal_info:
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_extract:
		if ((ULONG) node->nod_arg[e_extract_part] == blr_extract_second) {
			/* QUADDATE - SECOND returns a float, or scaled! */
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = sizeof(ULONG);
			desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
		}
		else {
			desc->dsc_dtype = dtype_short;
			desc->dsc_length = sizeof(SSHORT);
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
		}
		return;

	case nod_agg_min:
	case nod_agg_max:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		return;

	case nod_negate:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		node->nod_flags =
			node->nod_arg[0]->nod_flags & (nod_double | nod_quad);
		return;

	case nod_literal:
		*desc = ((LIT) node)->lit_desc;
		return;

	case nod_cast:
		{
			FMT format;
			DSC desc1;

			format = (FMT) node->nod_arg[e_cast_fmt];
			*desc = format->fmt_desc[0];
			if ((desc->dsc_dtype <= dtype_any_text && !desc->dsc_length) ||
				(desc->dsc_dtype == dtype_varying
				 && desc->dsc_length <= sizeof(USHORT))) {
				CMP_get_desc(tdbb, csb, node->nod_arg[e_cast_source], &desc1);
				desc->dsc_length = DSC_string_length(&desc1);
				if (desc->dsc_dtype == dtype_cstring)
					desc->dsc_length++;
				else if (desc->dsc_dtype == dtype_varying)
					desc->dsc_length += sizeof(USHORT);
			}
			return;
		}

	case nod_argument:
		{
			FMT format;
			JRD_NOD message;

			message = node->nod_arg[e_arg_message];
			format = (FMT) message->nod_arg[e_msg_format];
			*desc = format->fmt_desc[(int) node->nod_arg[e_arg_number]];
			return;
		}

	case nod_substr:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		if (desc->dsc_dtype == dtype_blob)
		{
			DSC	desc1, desc2;
			ULONG rc_len;
			CMP_get_desc(tdbb, csb, node->nod_arg [1], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg [2], &desc2);
			if (desc1.dsc_flags & DSC_null || desc2.dsc_flags & DSC_null)
			{
				rc_len = 0;
				desc->dsc_flags |= DSC_null;
			}
			else
			{
				SLONG sl1 = MOV_get_long(&desc1, 0);
				SLONG sl2 = MOV_get_long(&desc2, 0);
				/* error() is a local routine in par.c, so we use plain ERR_post. */
				if (sl1 < 0 || sl2 < 0 || sl2 > MAX_COLUMN_SIZE - (SLONG) sizeof(USHORT))
					ERR_post(gds_imp_exc, gds_arg_gds, gds_blktoobig, 0);
				rc_len = sl2;
			}
			desc->dsc_dtype = dtype_varying;
			desc->dsc_ttype = desc->dsc_scale;
			desc->dsc_scale = 0;
			desc->dsc_length = static_cast<USHORT>(rc_len) + sizeof(USHORT);
		}
		return;

	case nod_function:
		{
			FUN function;

			function = (FUN) node->nod_arg[e_fun_function];
	/** Null value for the function indicates that the function was not
	    looked up during parsing the blr. This is true if the function 
	    referenced in the procedure blr was dropped before dropping the
	    procedure itself. Ignore the case because we are currently trying
	    to drop the procedure.
	    For normal requests, function would never be null. We would have
	    created a valid block while parsing in par_function/par.c.
	**/
			if (function)
				*desc = function->fun_rpt[function->fun_return_arg].fun_desc;
			else
				/* Note that CMP_get_desc is always called with a pre-allocated
				   DSC i.e 
				   DSC desc;
				   CMP_get_desc (.... &desc); Hence  the code
				   *desc = NULL;  will not work. What I've done is memset the
				   structure to zero.
				 */
				MOVE_CLEAR(desc, sizeof(DSC));
			return;
		}

	case nod_variable:
		{
			JRD_NOD value;

			value = node->nod_arg[e_var_variable];
			*desc = *(DSC *) (value->nod_arg + e_dcl_desc);
			return;
		}

	case nod_value_if:
		CMP_get_desc(tdbb, csb, node->nod_arg[1], desc);
		return;

	case nod_bookmark:
		desc->dsc_dtype = dtype_text;
		desc->dsc_ttype = ttype_binary;
		desc->dsc_length = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	default:
		assert(FALSE);
		break;
	}

	if (dtype == dtype_quad)
		IBERROR(224);			/* msg 224 quad word arithmetic not supported */

	ERR_post(isc_datype_notsup, 0);	/* data type not supported for arithmetic */
}


IDL DLL_EXPORT CMP_get_index_lock(TDBB tdbb, JRD_REL relation, USHORT id)
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
	DBB dbb;
	IDL index;
	LCK lock;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

	DEV_BLKCHK(relation, type_rel);

	if (relation->rel_id < (USHORT) rel_MAX)
		return NULL;

/* For for an existing block */

	for (index = relation->rel_index_locks; index; index = index->idl_next)
		if (index->idl_id == id)
			return index;

	index = FB_NEW(*dbb->dbb_permanent) idl();
	index->idl_next = relation->rel_index_locks;
	relation->rel_index_locks = index;
	index->idl_relation = relation;
	index->idl_id = id;
	index->idl_count = 0;

	index->idl_lock = lock = FB_NEW_RPT(*dbb->dbb_permanent, 0) lck;
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_dbb = dbb;
	lock->lck_key.lck_long = relation->rel_id * 1000 + id;
	lock->lck_length = sizeof(lock->lck_key.lck_long);
	lock->lck_type = LCK_idx_exist;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);

	return index;
}


SLONG DLL_EXPORT CMP_impure(CSB csb, USHORT size)
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
	SLONG offset;

	DEV_BLKCHK(csb, type_csb);

	if (!csb)
		return 0;

	offset = FB_ALIGN(csb->csb_impure, ALIGNMENT);
	csb->csb_impure = offset + size;

	return offset;
}


JRD_REQ DLL_EXPORT CMP_make_request(TDBB tdbb, CSB * csb_ptr)
{
/**************************************
 *
 *	C M P _ m a k e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Turn a parsed request into an executable request.
 *
 **************************************/

	JRD_REQ request = 0;
	LLS temp;

	DEV_BLKCHK(*csb_ptr, type_csb);

	SET_TDBB(tdbb);

	JRD_REQ old_request = tdbb->tdbb_request;
	tdbb->tdbb_request = NULL;
	
	try {

/* Once any expansion required has been done, make a pass to assign offsets
   into the impure area and throw away any unnecessary crude.  Execution
   optimizations can be performed here */

	DEBUG;
	JRD_NOD node = pass1(tdbb, csb_ptr, (*csb_ptr)->csb_node, 0, 0, FALSE);
	CSB csb = *csb_ptr;
	csb->csb_node = node;
	csb->csb_impure = REQ_SIZE + REQ_TAIL * csb->csb_n_stream;
	csb->csb_node = pass2(tdbb, csb, csb->csb_node, 0);

	if (csb->csb_impure > MAX_REQUEST_SIZE)
		IBERROR(226);			/* msg 226 request size limit exceeded */

/* Build the final request block.  First, compute the "effective" repeat
   count of hold the impure areas. */

	int n = (csb->csb_impure - REQ_SIZE + REQ_TAIL - 1) / REQ_TAIL;
	request = FB_NEW_RPT(*tdbb->tdbb_default, n) jrd_req(tdbb->tdbb_default);
	request->req_count = csb->csb_n_stream;
	request->req_pool = tdbb->tdbb_default;
	request->req_impure_size = csb->csb_impure;
	request->req_top_node = csb->csb_node;
	request->req_access = csb->csb_access;
	request->req_external = csb->csb_external;
	request->req_variables = csb->csb_variables;
	request->req_resources = csb->csb_resources; // Assign array contents
	request->req_last_xcp.xcp_type = 0;
	request->req_last_xcp.xcp_msg = 0;
	if (csb->csb_g_flags & csb_blr_version4) {
		request->req_flags |= req_blr_version4;
	}

#ifdef SCROLLABLE_CURSORS
	request->req_async_message = csb->csb_async_message;
#endif

 	// Walk over invariant nodes and bind them to variables so
 	// assignment to variables can clear out dependent invariants
 	jrd_nod **link_ptr, **link_end;
 	for (link_ptr = csb->csb_invariants.begin(), link_end = csb->csb_invariants.end();
 		 link_ptr < link_end; link_ptr++)
 	{
 		RSE rse;
 		switch ((*link_ptr)->nod_type) {
 		case nod_max:
 		case nod_min:
 		case nod_count:
 		case nod_count2:
 		case nod_average:
 		case nod_total:
 		case nod_from:
 			rse = reinterpret_cast<RSE>((*link_ptr)->nod_arg[e_stat_rse]);
 			break;
 		case nod_ansi_all:
 		case nod_ansi_any:
 		case nod_any:
 		case nod_exists:
 		case nod_unique:
 			rse = reinterpret_cast<RSE>((*link_ptr)->nod_arg[e_any_rse]);
 			break;
 		}
 		if (!rse->rse_variables)
 			continue;
 		// Put dependent invariants to variables blocks
 		jrd_nod **ptr, **end;
 		for (ptr = rse->rse_variables->begin(), end = rse->rse_variables->end();
 			 ptr < end; ptr++)
 		{
 			VarInvariantArray **var_invariants;
 			switch((*ptr)->nod_type) {
 			case nod_argument: 
 				{
 					jrd_nod* msg = (*ptr)->nod_arg[e_arg_message];
 					MsgInvariantArray *msg_invariants;
 					if (!msg->nod_arg[e_msg_invariants]) {
 						msg_invariants = FB_NEW(*tdbb->tdbb_default) 
 							MsgInvariantArray(*tdbb->tdbb_default);
 						msg->nod_arg[e_msg_invariants] = 
 							reinterpret_cast<jrd_nod*>(msg_invariants);
 					} else {
 						msg_invariants = reinterpret_cast<MsgInvariantArray *>(
 							msg->nod_arg[e_msg_invariants]);
 					}
 					SLONG arg_number = (SLONG)(IPTR)(*ptr)->nod_arg[e_arg_number];
 					if (msg_invariants->getCount() <= arg_number)
 						msg_invariants->grow(arg_number + 1);
 					var_invariants = &(*msg_invariants)[arg_number];
 					break;
 				}
 			case nod_variable:
 				var_invariants = reinterpret_cast<VarInvariantArray**>(
 					&(*ptr)->nod_arg[e_var_variable]->nod_arg[e_dcl_invariants]);
 				break;
 			}
 
 			if (!(*var_invariants)) {
 				*var_invariants = FB_NEW(*tdbb->tdbb_default)
 					VarInvariantArray(*tdbb->tdbb_default);
 			}
 			int pos;
 			if (!(*var_invariants)->find((*link_ptr)->nod_impure, pos))
 				(*var_invariants)->insert(pos, (*link_ptr)->nod_impure);
 		}
 	}


/* Take out existence locks on resources used in request.  This is
   a little complicated since relation locks MUST be taken before
   index locks */


	for (Resource* resource = request->req_resources.begin(); resource < request->req_resources.end(); resource++)
	{
		switch (resource->rsc_type)
		{
		case Resource::rsc_relation:
			{
				JRD_REL relation = resource->rsc_rel;
				MET_post_existence(tdbb, relation);
				break;
			}
		case Resource::rsc_index:
			{
				JRD_REL relation = resource->rsc_rel;
				IDL index =
					CMP_get_index_lock(tdbb, relation, resource->rsc_id);
				if (index)
				{
					if (!index->idl_count)
					{
						LCK_lock_non_blocking(	tdbb,
												index->idl_lock,
												LCK_SR,
												TRUE);
					}
					++index->idl_count;
				}
				break;
			}
		case Resource::rsc_procedure:
			{
				JRD_PRC procedure = resource->rsc_prc;
				procedure->prc_use_count++;
#ifdef DEBUG_PROCS
				{
					char buffer[256];
					sprintf(buffer,
							"Called from CMP_make_request():\n\t Incrementing use count of %s\n",
							procedure->prc_name->str_data);
					JRD_print_procedure_info(tdbb, buffer);
				}
#endif
				break;
			}
		default:
			BUGCHECK(219);		/* msg 219 request of unknown resource */
		}
	}

	//csb_repeat* tail = &*(csb->csb_rpt.begin());
    csb_repeat* tail = &(csb->csb_rpt[0]);
	csb_repeat* streams_end  = tail + csb->csb_n_stream;
	DEBUG;

	for (RPB* rpb = request->req_rpb; tail < streams_end; rpb++, tail++)
	{
		/* Fetch input stream for update if all booleans matched against indices. */

		if (tail->csb_flags & csb_update
			&& !(tail->csb_flags & csb_unmatched)) rpb->rpb_stream_flags |=
				RPB_s_update;
		rpb->rpb_relation = tail->csb_relation;
		SBM_release(tail->csb_fields);
	}

  	// make a vector of all used RSEs
 	request->req_fors = csb->csb_fors;

/* make a vector of all invariant-type nodes, so that we will
   be able to easily reinitialize them when we restart the request */

	request->req_invariants = csb->csb_invariants;

	DEBUG;
	tdbb->tdbb_request = old_request;

	}	// try
	catch (const std::exception&) {
		tdbb->tdbb_request = old_request;
		ERR_punt();
	}

	return request;
}


void CMP_post_access(TDBB			tdbb,
							   CSB			csb,
							   const TEXT*		security_name,
							   SLONG		view_id,
							   USHORT		mask,
							   const TEXT*	type_name,
							   const TEXT*	name)
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


/* allow all access to internal requests */

	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
		return;

	SET_TDBB(tdbb);

	int i;
	
	AccessItem temp(security_name, view_id, name, type_name, mask);

	if (csb->csb_access.find(temp, i))
	{
	    return;
	}
	
	AccessItem access(
	    clone_cstring(tdbb->tdbb_default, security_name),
	    view_id,
	    clone_cstring(tdbb->tdbb_default, name),
	    type_name,
	    mask
	);
	csb->csb_access.insert(i, access);
	
}


void DLL_EXPORT CMP_post_resource( ResourceList * rsc_ptr,
						  BLK rel_or_prc,
                                                  ENUM Resource::rsc_s type,
                                                  USHORT id)
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
	Resource resource(type, id, NULL, NULL);

	switch (type) {
	case Resource::rsc_relation:
	case Resource::rsc_index:
		resource.rsc_rel = (JRD_REL) rel_or_prc;
		break;
	case Resource::rsc_procedure:
		resource.rsc_prc = (JRD_PRC) rel_or_prc;
		break;
	default:
		BUGCHECK(220);			/* msg 220 unknown resource */
		break;
	}
}


#ifdef PC_ENGINE
void DLL_EXPORT CMP_release_resource(
									 Resource * rsc_ptr,
									 ENUM rsc_s type, USHORT id)
{
/**************************************
 *
 *	C M P _ r e l e a s e _ r e s o u r c e
 *
 **************************************
 *
 * Functional description
 *	Release resource from request.
 *
 **************************************/
	Resource resource;

	DEV_BLKCHK(*rsc_ptr, type_rsc);

	for (; (resource = *rsc_ptr); rsc_ptr = &resource->rsc_next)
		if (resource->rsc_type == type && resource->rsc_id == id)
			break;

	if (!resource)
		return;

/* take out of the linked list and release */

	*rsc_ptr = resource->rsc_next;
	delete resource;
}
#endif

void DLL_EXPORT CMP_decrement_prc_use_count(TDBB tdbb, JRD_PRC procedure)
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

	DEV_BLKCHK(procedure, type_prc);

	/* Actually, it's possible for procedures to have intermixed dependencies, so
	   this routine can be called for the procedure which is being freed itself.
	   Hence we should just silently ignore such a situation. */

//	assert(procedure->prc_use_count > 0); -- commented out by dimitr, 2003.01.30
	if (!procedure->prc_use_count)
		return;

	if (procedure->prc_int_use_count > 0) procedure->prc_int_use_count--;
	
	--procedure->prc_use_count;

#ifdef DEBUG_PROCS
	{
		char buffer[256];
		sprintf(buffer,
				"Called from CMP_decrement():\n\t Decrementing use count of %s\n",
				procedure->prc_name->str_data);
		JRD_print_procedure_info(tdbb, buffer);
	}
#endif

/* Call recursively if and only if the use count is zero AND the procedure
   in dbb_procedures is different than this procedure. 
   The procedure will be different than in dbb_procedures only if it is a
   floating copy .i.e. an old copy or a deleted procedure.
*/
	if ((procedure->prc_use_count == 0) &&
		( (*tdbb->tdbb_database->dbb_procedures)[procedure->prc_id]
		 //!= &procedure->prc_header)) {
		 != (BLK) procedure)) {
		if (procedure->prc_request) {
			CMP_release(tdbb, procedure->prc_request);
		}
		procedure->prc_flags &= ~PRC_being_altered;
		MET_remove_procedure(tdbb, procedure->prc_id, procedure);
	}
}


void DLL_EXPORT CMP_release(TDBB tdbb, JRD_REQ request)
{
/**************************************
 *
 *	C M P _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release an unneeded and unloved request.
 *
 **************************************/
	IDL index;
	JRD_REL relation;
	ATT attachment;

	SET_TDBB(tdbb);

	DEV_BLKCHK(request, type_req);

/* Release existence locks on references */

	if (!(attachment = request->req_attachment)
		|| !(attachment->att_flags & ATT_shutdown))
		for (Resource* resource = request->req_resources.begin();
		resource < request->req_resources.end(); resource++) 
		{
			switch (resource->rsc_type) {
			case Resource::rsc_relation:
				{
					relation = resource->rsc_rel;
					MET_release_existence(relation);
					break;
				}
			case Resource::rsc_index:
				{
					relation = resource->rsc_rel;
					if ( (index = CMP_get_index_lock(tdbb, relation,
													 resource->rsc_id)) )
					{
						if (index->idl_count)
							--index->idl_count;
						if (!index->idl_count)
							LCK_release(tdbb, index->idl_lock);
					}
					break;
				}
			case Resource::rsc_procedure:
				{
					CMP_decrement_prc_use_count(tdbb, resource->rsc_prc);
					break;
				}
			default:
				BUGCHECK(220);	// msg 220 release of unknown resource
				break;
			}
		}

	EXE_unwind(tdbb, request);

#ifdef PC_ENGINE
	RNG_release_ranges(request);
#endif

	if (request->req_attachment) {
		for (JRD_REQ *next = &request->req_attachment->att_requests;
		*next; next = &(*next)->req_request) {
			if (*next == request) {
				*next = request->req_request;
#ifdef DEV_BUILD
				// Once I've seen att_requests == 0x00000014,
				// so some debugging code added to catch it earlier in dev_builds.
				// This place is one of two, where att_requests modified.
				// In another one (jrd.cpp/GDS_COMPILE()), it's value is used 
				// right before pointer assignment. So make some use of pointer here
				// to try to detect false in it earlier ...
				if (*next) {
					JRD_REQ req = (*next)->req_request;
					req++;
				}
#endif
				break;
			}
		}
	}

	JrdMemoryPool::deletePool(request->req_pool);
}


void DLL_EXPORT CMP_shutdown_database(TDBB tdbb)
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
	JRD_REL relation;
	vec::iterator ptr, end;
	JRD_PRC procedure;
	vec::iterator pptr, pend;
	IDL index;
	VEC vector;
	DBB dbb;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	DEV_BLKCHK(dbb, type_dbb);

	if (!(vector = dbb->dbb_relations))
		return;

/* Go through relations and indeces and release
   all existence locks that might have been taken
*/
	for (ptr = vector->begin(), end = vector->end(); ptr < end; ptr++)
		if ( (relation = (JRD_REL)*ptr) ) {
			if (relation->rel_existence_lock) {
				LCK_release(tdbb, relation->rel_existence_lock);
				relation->rel_use_count = 0;
			}
			for (index = relation->rel_index_locks; index;
				 index = index->idl_next)
				if (index->idl_lock) {
					LCK_release(tdbb, index->idl_lock);
					index->idl_count = 0;
				}
		}

	if (!(vector = dbb->dbb_procedures))
		return;

/* Release all procedure existence locks that
   might have been taken
*/
	for (pptr = vector->begin(), pend = vector->end(); pptr < pend; pptr++)
		if ( (procedure = (JRD_PRC)*pptr) ) {
			if (procedure->prc_existence_lock) {
				LCK_release(tdbb, procedure->prc_existence_lock);
				procedure->prc_use_count = 0;
			}
		}
}


static UCHAR *alloc_map(TDBB tdbb, CSB * csb, USHORT stream)
{
/**************************************
 *
 *	a l l o c _ m a p
 *
 **************************************
 *
 * Functional description
 *	Allocate and initialize stream map for view processing.
 *
 **************************************/
	STR string;

	DEV_BLKCHK(*csb, type_csb);

	SET_TDBB(tdbb);

	string = FB_NEW_RPT(*tdbb->tdbb_default, MAP_LENGTH) str;
	string->str_length = MAP_LENGTH;
	(*csb)->csb_rpt[stream].csb_map = (UCHAR *) string->str_data;
/* TMN: Here we should really have the following assert */
/* assert(stream <= MAX_UCHAR); */
	string->str_data[0] = (UCHAR) stream;

	return (UCHAR *) string->str_data;
}


#ifdef PC_ENGINE
static USHORT base_stream(CSB csb, JRD_NOD * stream_number, BOOLEAN nav_stream)
{
/**************************************
 *
 *	b a s e _ s t r e a m
 *
 **************************************
 *
 * Functional description
 *	Find the base stream of a view for navigational
 *	access.  If there is more than one base table, 
 *	give an error.
 *
 **************************************/
	UCHAR *map;
	USHORT stream;

	DEV_BLKCHK(csb, type_csb);
/* Note: *stream_number is NOT a JRD_NOD */

	stream = (USHORT) * stream_number;

/* If the stream references a view, follow map */

	if (map = csb->csb_rpt[stream].csb_map)
		if (map[2]) {
			if (nav_stream)
				/* navigational stream %ld references a view with more than one base table */
				ERR_post(isc_complex_view, gds_arg_number, (SLONG) stream, 0);
		}
		else {
			map++;
			stream = *map;
		}

/* if this is a navigational stream, fix up the stream number 
   in the node tree to point to the base table from now on */

	if (nav_stream)
		*stream_number = (JRD_NOD) stream;
	return stream;
}
#endif


static JRD_NOD catenate_nodes(TDBB tdbb, LLS stack)
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
	JRD_NOD cat_node, node1;

	SET_TDBB(tdbb);

	DEV_BLKCHK(stack, type_lls);

	node1 = (JRD_NOD) LLS_POP(&stack);

	if (!stack)
		return node1;

	cat_node = PAR_make_node(tdbb, 2);
	cat_node->nod_type = nod_concatenate;
	cat_node->nod_arg[0] = node1;
	cat_node->nod_arg[1] = catenate_nodes(tdbb, stack);

	return cat_node;
}


static JRD_NOD copy(TDBB tdbb,
					CSB * csb,
					JRD_NOD input,
					UCHAR * remap,
					USHORT field_id,
					JRD_NOD message,
					USHORT remap_fld)
{
/**************************************
 *
 *	c o p y
 *
 **************************************
 *
 * Functional description
 *	Copy an expression tree remapping field streams.  If the
 *	map isn't present, don't remap.
 *
 **************************************/
	JRD_NOD node, *arg1, *arg2, *end;
	USHORT stream, new_stream, args;

	SET_TDBB(tdbb);


	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(input, type_nod);

	if (!input)
		return NULL;

/* Special case interesting nodes */

	args = input->nod_count;

	switch (input->nod_type) {
	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		args = e_any_length;
		break;

	case nod_for:
		args = e_for_length;
		break;

	case nod_argument:
		if (remap_fld)
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
		node->nod_arg[e_arg_message] =
			message ? message : input->nod_arg[e_arg_message];
		node->nod_arg[e_arg_flag] =
			copy(tdbb, csb, input->nod_arg[e_arg_flag], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_arg_indicator] =
			copy(tdbb, csb, input->nod_arg[e_arg_indicator], remap, field_id,
				 message, remap_fld);
		return (node);

		break;

	case nod_assignment:
		args = e_asgn_length;
		break;

	case nod_erase:
		args = e_erase_length;
		break;

	case nod_user_savepoint:
		args = e_sav_length;
		break;

	case nod_modify:
		args = e_mod_length;
		break;

	case nod_variable:
	case nod_literal:
		return input;

	case nod_field:
		{
			JRD_NOD temp_node;
			if (field_id &&
				(input->nod_flags & nod_id) &&
				!input->nod_arg[e_fld_id] && !input->nod_arg[e_fld_stream])
				--field_id;
			else
				field_id = (USHORT) input->nod_arg[e_fld_id];
			stream = (USHORT) input->nod_arg[e_fld_stream];
			if (remap_fld) {
				JRD_REL relation;
				JRD_FLD field;

				relation = (*csb)->csb_rpt[stream].csb_relation;
				field = MET_get_field(relation, field_id);
				if (field->fld_source)
					field_id = (USHORT) field->fld_source->nod_arg[e_fld_id];
			}
			if (remap)
				stream = remap[stream];

			temp_node = PAR_gen_field(tdbb, stream, field_id);
			if (input->nod_type == nod_field &&
				input->nod_arg[e_fld_default_value])
					temp_node->nod_arg[e_fld_default_value] =
					input->nod_arg[e_fld_default_value];
			return temp_node;
		}

	case nod_function:
		node = PAR_make_node(tdbb, e_fun_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_fun_args] =
			copy(tdbb, csb, input->nod_arg[e_fun_args], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_fun_function] = input->nod_arg[e_fun_function];
		return (node);


	case nod_gen_id:
	case nod_gen_id2:			/* 20001013 PJPG */
		node = PAR_make_node(tdbb, e_gen_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_gen_value] =
			copy(tdbb, csb, input->nod_arg[e_gen_value], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_gen_relation] = input->nod_arg[e_gen_relation];
		return (node);

	case nod_cast:
		node = PAR_make_node(tdbb, e_cast_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_cast_source] =
			copy(tdbb, csb, input->nod_arg[e_cast_source], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_cast_fmt] = input->nod_arg[e_cast_fmt];
		return (node);

	case nod_extract:
		node = PAR_make_node(tdbb, e_extract_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_extract_value] =
			copy(tdbb, csb, input->nod_arg[e_extract_value], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_extract_part] = input->nod_arg[e_extract_part];
		return (node);

	case nod_count:
	case nod_count2:
	case nod_max:
	case nod_min:
	case nod_total:
	case nod_average:
	case nod_from:
		args = e_stat_length;
		break;

	case nod_rse:
	case nod_stream:
		{
			RSE new_rse, old_rse;

			old_rse = (RSE) input;
			new_rse =
				(RSE) PAR_make_node(tdbb, old_rse->rse_count + rse_delta + 2);
			new_rse->nod_type = input->nod_type;
			new_rse->nod_count = 0;
			new_rse->rse_count = old_rse->rse_count;
			arg1 = old_rse->rse_relation;
			arg2 = new_rse->rse_relation;
			for (end = arg1 + old_rse->rse_count; arg1 < end; arg1++, arg2++)
				*arg2 = copy(tdbb, csb, *arg1, remap, field_id, message, remap_fld);
			new_rse->rse_jointype = old_rse->rse_jointype;
			new_rse->rse_writelock = old_rse->rse_writelock;
			new_rse->rse_first =
				copy(tdbb, csb, old_rse->rse_first, remap, field_id,
					 message, remap_fld);
			new_rse->rse_skip =
				copy(tdbb, csb, old_rse->rse_skip, remap, field_id,
					 message, remap_fld);
			new_rse->rse_boolean =
				copy(tdbb, csb, old_rse->rse_boolean, remap, field_id,
					 message, remap_fld);
			new_rse->rse_sorted =
				copy(tdbb, csb, old_rse->rse_sorted, remap, field_id,
					 message, remap_fld);
			new_rse->rse_projection =
				copy(tdbb, csb, old_rse->rse_projection, remap, field_id,
					 message, remap_fld);
			return (JRD_NOD) new_rse;
		}

	case nod_relation:
		{
			csb_repeat *element;
			int relative_stream;

			if (!remap)
				BUGCHECK(221);	/* msg 221 (CMP) copy: cannot remap */
			node = PAR_make_node(tdbb, e_rel_length);
			node->nod_type = input->nod_type;
			node->nod_count = 0;

			stream = (USHORT) input->nod_arg[e_rel_stream];
	/** 
	    Last entry in the remap contains the the original stream number.
	    Get that stream number so that the flags can be copied 
	    into the newly created child stream.
	**/
			relative_stream = (stream) ? remap[stream - 1] : stream;
			new_stream = (*csb)->csb_n_stream++;
			node->nod_arg[e_rel_stream] = (JRD_NOD) (SLONG) new_stream;
			/* TMN: Here we should really have the following assert */
			/* assert(new_stream <= MAX_UCHAR); */
			remap[stream] = (UCHAR) new_stream;

			node->nod_arg[e_rel_context] = input->nod_arg[e_rel_context];
			node->nod_arg[e_rel_relation] = input->nod_arg[e_rel_relation];
			node->nod_arg[e_rel_view] = input->nod_arg[e_rel_view];

			element = CMP_csb_element(csb, new_stream);
			element->csb_relation = (JRD_REL) node->nod_arg[e_rel_relation];
			element->csb_view = (JRD_REL) node->nod_arg[e_rel_view];
			element->csb_view_stream = remap[0];

	/** If there was a parent stream no., then copy the flags 
	    from that stream to its children streams. (Bug 10164/10166)
	    For e.g. 
	    consider a view V1 with 2 streams
	           stream #1 from table T1
		   stream #2 from table T2
	    consider a procedure P1 with 2 streams
	           stream #1  from table X
		   stream #2  from view V1

	    During pass1 of procedure request, the engine tries to expand 
	    all the views into their base tables. It creates a compilier 
	    scratch block which initially looks like this
	         stream 1  -------- X
	         stream 2  -------- V1
		 while expanding V1 the engine calls copy() with nod_relation.
		 A new stream 3 is created. Now the CSB looks like
	         stream 1  -------- X
	         stream 2  -------- V1  map [2,3]
	         stream 3  -------- T1
		 After T1 stream has been created the flags are copied from
		 stream #1 because V1's definition said the original stream
		 number for T1 was 1. However since its being merged with 
		 the procedure request, stream #1 belongs to a different table. 
		 The flags should be copied from stream 2 i.e. V1. We can get
		 this info from variable remap.

		 Since we didn't do this properly before, V1's children got
		 tagged with whatever flags X possesed leading to various
		 errors.

		 We now store the proper stream no in relative_stream and
		 later use it to copy the flags. -Sudesh (03/05/99)
	**/

			(*csb)->csb_rpt[new_stream].csb_flags |=
				(*csb)->csb_rpt[relative_stream].csb_flags & csb_no_dbkey;
			return node;
		}

	case nod_procedure:
		{
			csb_repeat *element;

			if (!remap)
				BUGCHECK(221);	/* msg 221 (CMP) copy: cannot remap */
			node = PAR_make_node(tdbb, e_prc_length);
			node->nod_type = input->nod_type;
			node->nod_count = input->nod_count;
			// dimitr:	see the appropriate code and comment above (in nod_argument).
			//			We must copy the message first and only then use the new
			//			pointer to copy the inputs properly.
			node->nod_arg[e_prc_in_msg] =
				copy(tdbb, csb, input->nod_arg[e_prc_in_msg], remap, field_id,
					 message, remap_fld);
			node->nod_arg[e_prc_inputs] =
				copy(tdbb, csb, input->nod_arg[e_prc_inputs], remap, field_id,
					 node->nod_arg[e_prc_in_msg], remap_fld);
			stream = (USHORT) input->nod_arg[e_prc_stream];
			new_stream = (*csb)->csb_n_stream++;
			node->nod_arg[e_prc_stream] = (JRD_NOD) (SLONG) new_stream;
			/* TMN: Here we should really have the following assert */
			/* assert(new_stream <= MAX_UCHAR); */
			remap[stream] = (UCHAR) new_stream;
			node->nod_arg[e_prc_procedure] = input->nod_arg[e_prc_procedure];
			element = CMP_csb_element(csb, new_stream);
			// SKIDDER: Maybe we need to check if we really found a procedure ?
			element->csb_procedure = MET_lookup_procedure_id(tdbb,
			  (SSHORT)node->nod_arg[e_prc_procedure],FALSE,FALSE,0);

			(*csb)->csb_rpt[new_stream].csb_flags |=
				(*csb)->csb_rpt[stream].csb_flags & csb_no_dbkey;
			return node;
		}

	case nod_aggregate:
		if (!remap)
			BUGCHECK(221);		/* msg 221 (CMP) copy: cannot remap */
		node = PAR_make_node(tdbb, e_agg_length);
		node->nod_type = input->nod_type;
		node->nod_count = 0;
		stream = (USHORT) input->nod_arg[e_agg_stream];
		assert(stream <= MAX_STREAMS);
		new_stream = (*csb)->csb_n_stream++;
		assert(new_stream <= MAX_STREAMS);
		node->nod_arg[e_agg_stream] = (JRD_NOD) (SLONG) new_stream;
		/* TMN: Here we should really have the following assert */
		/* assert(new_stream <= MAX_UCHAR); */
		remap[stream] = (UCHAR) new_stream;
		CMP_csb_element(csb, new_stream);

		(*csb)->csb_rpt[new_stream].csb_flags |=
			(*csb)->csb_rpt[stream].csb_flags & csb_no_dbkey;
		node->nod_arg[e_agg_rse] =
			copy(tdbb, csb, input->nod_arg[e_agg_rse], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_agg_group] =
			copy(tdbb, csb, input->nod_arg[e_agg_group], remap, field_id,
				 message, remap_fld);
		node->nod_arg[e_agg_map] =
			copy(tdbb, csb, input->nod_arg[e_agg_map], remap, field_id,
				 message, remap_fld);
		return node;

	case nod_union:
		if (!remap)
			BUGCHECK(221);		/* msg 221 (CMP) copy: cannot remap */
		node = PAR_make_node(tdbb, e_uni_length);
		node->nod_type = input->nod_type;
		node->nod_count = 2;
		stream = (USHORT) input->nod_arg[e_uni_stream];
		new_stream = (*csb)->csb_n_stream++;
		node->nod_arg[e_uni_stream] = (JRD_NOD) (SLONG) new_stream;
		/* TMN: Here we should really have the following assert */
		/* assert(new_stream <= MAX_UCHAR); */
		remap[stream] = (UCHAR) new_stream;
		CMP_csb_element(csb, new_stream);

		(*csb)->csb_rpt[new_stream].csb_flags |=
			(*csb)->csb_rpt[stream].csb_flags & csb_no_dbkey;
		node->nod_arg[e_uni_clauses] =
			copy(tdbb, csb, input->nod_arg[e_uni_clauses], remap, field_id,
				 message, remap_fld);
		return node;

	case nod_message:
		if (!remap)
			BUGCHECK(221);		// msg 221 (CMP) copy: cannot remap
		node = PAR_make_node(tdbb, e_msg_length);
		node->nod_type = input->nod_type;
		node->nod_count = input->nod_count;
		node->nod_arg[e_msg_number] = input->nod_arg[e_msg_number];
		node->nod_arg[e_msg_format] = input->nod_arg[e_msg_format];
		// dimitr: hmmm, cannot find where the following one is used...
		node->nod_arg[e_msg_next] =
			copy(tdbb, csb, input->nod_arg[e_msg_next], remap, field_id,
				 message, remap_fld);
		return node;

	case nod_sort:
		args += 2 * args;
		break;

	default:
		break;
	}

/* Fall thru on generic nodes */

	node = PAR_make_node(tdbb, args);
	node->nod_count = input->nod_count;
	node->nod_type = input->nod_type;
	node->nod_flags = input->nod_flags;

	arg1 = input->nod_arg;
	arg2 = node->nod_arg;

	for (end = arg1 + input->nod_count; arg1 < end; arg1++, arg2++)
		if (*arg1)
			*arg2 = copy(tdbb, csb, *arg1, remap, field_id, message, remap_fld);

/* Finish off sort */

	if (input->nod_type == nod_sort)
		for (end = arg1 + input->nod_count * 2; arg1 < end; arg1++, arg2++)
			*arg2 = *arg1;

	return node;
}


static void expand_view_nodes(
							  TDBB tdbb,
							  CSB csb, USHORT stream, LLS * stack, NOD_T type)
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
	UCHAR *map;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(*stack, type_lls);

/* If the stream's dbkey should be ignored, do so. */

	if (csb->csb_rpt[stream].csb_flags & csb_no_dbkey)
		return;

/* If the stream references a view, follow map */

	if ( (map = csb->csb_rpt[stream].csb_map) ) {
		++map;
		while (*map)
			expand_view_nodes(tdbb, csb, *map++, stack, type);
		return;
	}

/* Relation is primitive -- make dbkey node */

	if (csb->csb_rpt[stream].csb_relation) {
		JRD_NOD node;

		node = PAR_make_node(tdbb, 1);
		node->nod_count = 0;
		node->nod_type = type;
		node->nod_arg[0] = (JRD_NOD) (SLONG) stream;
		LLS_PUSH(node, stack);
	}
}


static void ignore_dbkey(TDBB tdbb, CSB csb, RSE rse, JRD_REL view)
{
/**************************************
 *
 *	i g n o r e _ d b k e y
 *
 **************************************
 *
 * Functional description
 *	For each relation or aggregate in the
 *	rse, mark it as not having a dbkey.
 *
 **************************************/
	JRD_NOD *ptr, *end, node;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(view, type_rel);

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end;) {
		node = *ptr++;
		if (node->nod_type == nod_relation) {
			USHORT stream;
			csb_repeat *tail;
			JRD_REL relation;

			stream = (USHORT) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags |= csb_no_dbkey;
			tail = &csb->csb_rpt[stream];
			if ( (relation = tail->csb_relation) )
				CMP_post_access(tdbb, csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								SCL_read, object_table,
								relation->rel_name);
		}
		else if (node->nod_type == nod_rse)
			ignore_dbkey(tdbb, csb, (RSE) node, view);
		else if (node->nod_type == nod_aggregate)
			ignore_dbkey(tdbb, csb, (RSE) node->nod_arg[e_agg_rse], view);
		else if (node->nod_type == nod_union) {
			JRD_NOD clauses, *ptr_uni, *end_uni;

			clauses = node->nod_arg[e_uni_clauses];
			for (ptr_uni = clauses->nod_arg, end_uni =
				 ptr_uni + clauses->nod_count; ptr_uni < end_uni; ptr_uni++)
				ignore_dbkey(tdbb, csb, (RSE) * ptr_uni++, view);
		}
	}
}


static JRD_NOD make_defaults(TDBB tdbb, CSB * csb, USHORT stream, JRD_NOD statement)
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
	JRD_NOD node, value;
	LLS stack;
	VEC vector;
	vec::iterator ptr1, end;
	JRD_REL relation;
	USHORT field_id;
	UCHAR *map, local_map[MAP_LENGTH];

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(statement, type_nod);

	relation = (*csb)->csb_rpt[stream].csb_relation;

	if (!(vector = relation->rel_fields))
		return statement;

	if (!(map = (*csb)->csb_rpt[stream].csb_map)) {
		map = local_map;
		/* TMN: Here we should really have the following assert */
		/* assert(stream <= MAX_UCHAR); */
		map[0] = (UCHAR) stream;
		map[1] = 1;
		map[2] = 2;
	}

	stack = NULL;

	for (ptr1 = vector->begin(), end = vector->end(), field_id = 0;
		     ptr1 < end; ptr1++, field_id++)
		if (*ptr1 && (value = ((JRD_FLD)(*ptr1))->fld_default_value)) {
			node = PAR_make_node(tdbb, e_asgn_length);
			node->nod_type = nod_assignment;
			node->nod_arg[e_asgn_from] =
				copy(tdbb, csb, value, map, (USHORT) (field_id + 1), NULL, FALSE);
			node->nod_arg[e_asgn_to] = PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}

	if (!stack)
		return statement;

/* We have some default -- add the original statement and make a list out of
   the whole mess */

	LLS_PUSH(statement, &stack);

	return PAR_make_list(tdbb, stack);
}


static JRD_NOD make_validation(TDBB tdbb, CSB * csb, USHORT stream)
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
	JRD_NOD node, validation;
	LLS stack;
	VEC vector;
	vec::iterator ptr1, end;
	JRD_REL relation;
	USHORT field_id;
	UCHAR *map, local_map[MAP_LENGTH];

	SET_TDBB(tdbb);


	DEV_BLKCHK(*csb, type_csb);

	relation = (*csb)->csb_rpt[stream].csb_relation;

	if (!(vector = relation->rel_fields))
		return NULL;

	if (!(map = (*csb)->csb_rpt[stream].csb_map)) {
		map = local_map;
		/* TMN: Here we should really have the following assert */
		/* assert(stream <= MAX_UCHAR); */
		map[0] = (UCHAR) stream;
	}

	stack = NULL;

	for (ptr1 = vector->begin(), end = vector->end(), field_id = 0;
		     ptr1 < end; ptr1++, field_id++) {
		if (*ptr1 && (validation = ((JRD_FLD)(*ptr1))->fld_validation)) {
			node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_boolean] =
				copy(tdbb, csb, validation, map, (USHORT) (field_id + 1),
					 NULL, FALSE);
			node->nod_arg[e_val_value] =
				PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}

		if (*ptr1 && (validation = ((JRD_FLD)(*ptr1))->fld_not_null)) {
			node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_boolean] =
				copy(tdbb, csb, validation, map, (USHORT) (field_id + 1),
					 NULL, FALSE);
			node->nod_arg[e_val_value] =
				PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}
	}

	if (!stack)
		return NULL;

	return PAR_make_list(tdbb, stack);
}


static JRD_NOD pass1(
				 TDBB tdbb,
				 CSB * csb,
				 JRD_NOD node,
				 JRD_REL view, USHORT view_stream, BOOLEAN validate_expr)
{
/**************************************
 *
 *	p a s s 1
 *
 **************************************
 *
 * Functional description
 *	Merge missing values, computed values, validation expressions,
 *	and views into a parsed request.
 *
 * The argument validate_expr is TRUE if an ancestor of the
 * current node (the one being passed in) in the parse tree has nod_type
 * == nod_validate. "ancestor" does not include the current node 
 * being passed in as an argument.
 * If we are in a "validate subtree" (as determined by the
 * validate_expr), we must not post update access to the fields involved 
 * in the validation clause. (see the call for CMP_post_access in this
 * function.)
 * 
 **************************************/
	JRD_NOD sub, *ptr, *end;
	USHORT stream;
	csb_repeat *tail;
	JRD_PRC procedure;

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(node, type_nod);
	DEV_BLKCHK(view, type_rel);

	if (!node) 
		return node;

	validate_expr = validate_expr || (node->nod_type == nod_validate);

/* If there is processing to be done before sub expressions, do it here */

	switch (node->nod_type) {
	case nod_variable:
	case nod_argument:
		{
			for (RSE *rse = (*csb)->csb_current_rses.begin(); 
				 rse < (*csb)->csb_current_rses.end(); rse++) 
			{
				if (!(*rse)->rse_variables) 
				{
					(*rse)->rse_variables = 
						FB_NEW(*tdbb->tdbb_default) 
							Firebird::Array<jrd_nod*>(*tdbb->tdbb_default);
				}
				(*rse)->rse_variables->add(node);
			}
		}
		break;
	case nod_field:
		{
			JRD_REL relation;
			JRD_FLD field;
			UCHAR *map, local_map[MAP_LENGTH];

			stream = (USHORT) node->nod_arg[e_fld_stream];

			/* Look at all rse's which are lower in scope than the rse which this field 
			   is referencing, and mark them as varying -- the rule is that if a field 
			   from one rse is referenced within the scope of another rse, the first rse 
			   can't be invariant.  This won't optimize all cases, but it is the simplest 
			   operating assumption for now. */

			if ((*csb)->csb_current_rses.getCount()) {
				for (RSE *rse = (*csb)->csb_current_rses.end()-1; 
					 rse >= (*csb)->csb_current_rses.begin(); rse--) 
	 			{  
	 				if (stream_in_rse(stream, *rse))
	  					break;
	 				(*rse)->nod_flags |= rse_variant;
	  			}
			}
			tail = &(*csb)->csb_rpt[stream];
			if (!(relation = tail->csb_relation) ||
				!(field =
				  MET_get_field(relation,
								(USHORT) node->nod_arg[e_fld_id]))) break;

			/* if this is a modify or store, check REFERENCES access to any foreign keys. */

/* CVC: This is against the SQL standard. REFERENCES should be enforced only at the
				time the FK is defined in DDL, not when a DML is going to be executed.
			if (((tail->csb_flags & csb_modify)
				 || (tail->csb_flags & csb_store)) && !(relation->rel_view_rse
														||
														relation->rel_file))
					IDX_check_access(tdbb, *csb, tail->csb_view, relation,
									 field);
*/

			/* Posting the required privilege access to the current relation and field. */

			/* if this is in a "validate_subtree" then we must not
			   post access checks to the table and the fields in the table. 
			   If any node of the parse tree is a nod_validate type node,
			   the nodes in the subtree are involved in a validation
			   clause only, the subtree is a validate_subtree in our
			   notation.  */
			if (tail->csb_flags & csb_modify) {
				if (!validate_expr) {
					CMP_post_access(tdbb, *csb, relation->rel_security_name,
									(tail->csb_view) ? tail->csb_view->rel_id : 
										(view ? view->rel_id : 0),
									SCL_sql_update, object_table,
									relation->rel_name);
					CMP_post_access(tdbb, *csb, field->fld_security_name,
									(tail->csb_view) ? tail->csb_view->rel_id : 
										(view ? view->rel_id : 0),
									SCL_sql_update, object_column,
									field->fld_name);
				}
			}
			else if (tail->csb_flags & csb_erase) {
				CMP_post_access(tdbb, *csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id :
									(view ? view->rel_id : 0),
								SCL_sql_delete, object_table,
								relation->rel_name);
			}
			else if (tail->csb_flags & csb_store) {
				CMP_post_access(tdbb, *csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								SCL_sql_insert, object_table,
								relation->rel_name);
				CMP_post_access(tdbb, *csb, field->fld_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0), 
								SCL_sql_insert, object_column, field->fld_name);
			}
			else {
				CMP_post_access(tdbb, *csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								SCL_read, object_table, relation->rel_name);
				CMP_post_access(tdbb, *csb, field->fld_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								SCL_read, object_column, field->fld_name);
			}

			if (!(sub = field->fld_computation) && !(sub = field->fld_source)) {

				if (!relation->rel_view_rse)
					break;

				ERR_post(gds_no_field_access,
						 gds_arg_string, ERR_cstring(field->fld_name),
						 gds_arg_string, ERR_cstring(relation->rel_name), 0);
				/* Msg 364 "cannot access column %s in view %s" */
			}


			/* The previous test below is an apparent temporary fix
			 * put in by Root & Harrison in Summer/Fall 1991.  
			 * Old Code:
			 * if (tail->csb_flags & (csb_view_update | csb_trigger))
			 *   break;
			 * If the field is a computed field - we'll go on and make
			 * the substitution.
			 * Comment 1994-August-08 David Schnepper
			 */

			if (tail->csb_flags & (csb_view_update | csb_trigger)) {
				// dimitr:	added an extra check for views, because we don't
				//			want their old/new contexts to be substituted
				if (relation->rel_view_rse || !field->fld_computation)
					break;
			}

			if (!(map = tail->csb_map)) {
				map = local_map;
				/* TMN: Here we should really have the following assert */
				/* assert(stream + 2 <= MAX_UCHAR); */
				local_map[0] = (UCHAR) stream;
				map[1] = stream + 1;
				map[2] = stream + 2;
			}
			sub = copy(tdbb, csb, sub, map, 0, NULL, FALSE);
			return pass1(tdbb, csb, sub, view, view_stream, validate_expr);
		}

	case nod_assignment:
		{
			JRD_FLD field;

			sub = node->nod_arg[e_asgn_from];
			if (sub->nod_type == nod_field) {
				stream = (USHORT) sub->nod_arg[e_fld_stream];
				field = MET_get_field((*csb)->csb_rpt[stream].csb_relation,
									  (USHORT) sub->nod_arg[e_fld_id]);
				if (field)
					node->nod_arg[e_asgn_missing2] = field->fld_missing_value;
			}

			sub = node->nod_arg[e_asgn_to];
			if (sub->nod_type != nod_field)
				break;
			stream = (USHORT) sub->nod_arg[e_fld_stream];
			tail = &(*csb)->csb_rpt[stream];
			if (!
				(field =
				 MET_get_field(tail->csb_relation,
							   (USHORT) sub->nod_arg[e_fld_id]))) break;
			if (field->fld_missing_value) {
				node->nod_arg[e_asgn_missing] = field->fld_missing_value;
				node->nod_count = 3;
			}
		}
		break;

	case nod_function:
		pass1(tdbb, csb, node->nod_arg[e_fun_args], view, view_stream,
			  validate_expr);
		break;

	case nod_modify:
		stream = (USHORT) node->nod_arg[e_mod_new_stream];
		tail = &(*csb)->csb_rpt[stream];
		tail->csb_flags |= csb_modify;
		pass1_modify(tdbb, csb, node);
		/* TMN: Here we should really have the following assert */
		/* assert(node->nod_arg [e_mod_new_stream] <= MAX_USHORT); */
		if ( (node->nod_arg[e_mod_validate] =
			make_validation(tdbb, csb,
							(USHORT) node->
							nod_arg[e_mod_new_stream])) ) node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_validate + 1);
		break;

	case nod_erase:
		stream = (USHORT) node->nod_arg[e_erase_stream];
		tail = &(*csb)->csb_rpt[stream];
		tail->csb_flags |= csb_erase;
		pass1_erase(tdbb, csb, node);
		break;

	case nod_exec_proc:
		procedure = (JRD_PRC) node->nod_arg[e_esp_procedure];
		// Post access to procedure
		post_procedure_access(tdbb, *csb, procedure);
		CMP_post_resource(&(*csb)->csb_resources, (BLK) procedure,
						  Resource::rsc_procedure, procedure->prc_id);
		break;

	case nod_store:
		sub = node->nod_arg[e_sto_relation];
		stream = (USHORT) sub->nod_arg[e_rel_stream];
		tail = &(*csb)->csb_rpt[stream];
		tail->csb_flags |= csb_store;
		sub = pass1_store(tdbb, csb, node);
		if (sub) {
			stream = (USHORT) sub->nod_arg[e_rel_stream];
			if ((!node->nod_arg[e_sto_sub_store]) &&
				(node->nod_arg[e_sto_validate] =
				 make_validation(tdbb, csb, stream))) node->nod_count =
					MAX(node->nod_count, (USHORT) e_sto_validate + 1);
			node->nod_arg[e_sto_statement] =
				make_defaults(tdbb, csb, stream,
							  node->nod_arg[e_sto_statement]);
		}
		break;

	case nod_rse:
	case nod_stream:
		return (JRD_NOD) pass1_rse(tdbb, csb, (RSE) node, view, view_stream);

	case nod_max:
	case nod_min:
	case nod_average:
	case nod_from:
	case nod_count:
	case nod_count2:
	case nod_total:
		ignore_dbkey(tdbb, *csb, (RSE) node->nod_arg[e_stat_rse], view);
		break;

	case nod_aggregate:
		assert((int)node->nod_arg[e_agg_stream] <= MAX_STREAMS);
		(*csb)->csb_rpt[(USHORT) node->nod_arg[e_agg_stream]].csb_flags |=
			csb_no_dbkey;
		ignore_dbkey(tdbb, *csb, (RSE) node->nod_arg[e_agg_rse], view);
		node->nod_arg[e_agg_rse] =
			pass1(tdbb, csb, node->nod_arg[e_agg_rse], view, view_stream,
				  validate_expr);
		node->nod_arg[e_agg_map] =
			pass1(tdbb, csb, node->nod_arg[e_agg_map], view, view_stream,
				  validate_expr);
		node->nod_arg[e_agg_group] =
			pass1(tdbb, csb, node->nod_arg[e_agg_group], view, view_stream,
				  validate_expr);
		break;

	case nod_gen_id:
	case nod_gen_id2:
		node->nod_arg[e_gen_value] =
			pass1(tdbb, csb, node->nod_arg[e_gen_value], view, view_stream,
				  validate_expr);
		return node;

	case nod_rec_version:
	case nod_dbkey:
		{
			LLS stack;
			NOD_T type;

			type = node->nod_type;
			stream = (USHORT) node->nod_arg[0];

			if (!(*csb)->csb_rpt[stream].csb_map) 
				return node;
			stack = NULL;
			expand_view_nodes(tdbb, *csb, stream, &stack, type);
			if (stack) 
				return catenate_nodes(tdbb, stack);
			
			/* The user is asking for the dbkey/record version of an aggregate.
			   Humor him with a key filled with zeros.
			 */

			node = PAR_make_node(tdbb, 1);
			node->nod_count = 0;
			node->nod_type = type;
			node->nod_flags |= nod_agg_dbkey;
			node->nod_arg[0] = (JRD_NOD) (SLONG) stream;
			return node;
		}

	case nod_abort:
		pass1(tdbb, csb, node->nod_arg[e_xcp_msg], view, view_stream,
			  validate_expr);
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		ignore_dbkey(tdbb, *csb, (RSE) node->nod_arg[e_any_rse], view);
		break;

	case nod_cardinality:
		stream = (USHORT) node->nod_arg[e_card_stream];
		(*csb)->csb_rpt[stream].csb_flags |= csb_compute;
		break;

	default:
		break;

	}

/* Handle sub-expressions here */

	ptr = node->nod_arg;

	for (end = ptr + node->nod_count; ptr < end; ptr++) {
		*ptr = pass1(tdbb, csb, *ptr, view, view_stream, validate_expr);
	}

/* perform any post-processing here */

	if (node->nod_type == nod_assignment) {
		sub = node->nod_arg[e_asgn_to];
		if (sub->nod_type != nod_field &&
			sub->nod_type != nod_argument && sub->nod_type != nod_variable)
			ERR_post(gds_read_only_field, 0);
	}

	return node;
}


static void pass1_erase(TDBB tdbb, CSB * csb, JRD_NOD node)
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
	JRD_REL relation, parent, view;
	JRD_NOD source, view_node;
	UCHAR *map;
	USHORT stream, new_stream, parent_stream = 0;
	TRIG_VEC trigger;
	csb_repeat *tail;
	USHORT priv;

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(node, type_nod);

/* If updateable views with triggers are involved, there
   maybe a recursive call to be ignored */

	if (node->nod_arg[e_erase_sub_erase])
		return;

	parent = view = NULL;

/* To support views of views, loop until we hit a real relation */

	for (;;) {
		stream = new_stream = (USHORT) node->nod_arg[e_erase_stream];
		tail = &(*csb)->csb_rpt[stream];
		tail->csb_flags |= csb_erase;
		relation = (*csb)->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
		post_trigger_access(*csb, relation, ExternalAccess::exa_delete, view);

		/* If this is a view trigger operation, get an extra stream to play with */

		trigger =
			(relation->rel_pre_erase) ? relation->
			rel_pre_erase : relation->rel_post_erase;

		if (relation->rel_view_rse && trigger) {
			new_stream = (*csb)->csb_n_stream++;
			node->nod_arg[e_erase_stream] = (JRD_NOD) (SLONG) new_stream;
			CMP_csb_element(csb, new_stream)->csb_relation = relation;
		}

		/* Check out delete.  If this is a delete thru a view, verify the
		   view by checking for read access on the base table.  If field-level select
		   privileges are implemented, this needs to be enhanced. */

		priv = SCL_sql_delete;
		if (parent)
			priv |= SCL_read;
		source =
			pass1_update(tdbb, csb, relation, trigger, stream, new_stream,
						 priv, parent, parent_stream);

		if (!source)
		{
			if ((*csb)->csb_rpt[new_stream].csb_flags & csb_view_update) {
				node->nod_arg[e_erase_statement] =
					pass1_expand_view(tdbb, *csb, stream, new_stream, FALSE);
				node->nod_count =
					MAX(node->nod_count, (USHORT) e_erase_statement + 1);
			}
			return;
		}

		/* We have a updateable view.  If there is a trigger on it, create a
		   dummy erase record. */

		map = (*csb)->csb_rpt[stream].csb_map;
		if (trigger) {
			node->nod_arg[e_erase_statement] =
				pass1_expand_view(tdbb, *csb, stream, new_stream, FALSE);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_erase_statement + 1);
			view_node = copy(tdbb, csb, node, map, 0, NULL, FALSE);
			node->nod_arg[e_erase_sub_erase] = view_node;
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_erase_sub_erase + 1);
			node = view_node;
			node->nod_arg[e_erase_statement] = 0;
			node->nod_arg[e_erase_sub_erase] = 0;
		}
		else
			(*csb)->csb_rpt[new_stream].csb_flags &= ~csb_view_update;

		/* So far, so good.  Lookup view context in instance map to get target
		   stream */

		parent = relation;
		parent_stream = stream;
		new_stream = (USHORT) source->nod_arg[e_rel_stream];
		node->nod_arg[e_erase_stream] = (JRD_NOD) (SLONG) map[new_stream];
	}
}


static JRD_NOD pass1_expand_view(
							 TDBB tdbb,
							 CSB csb,
							 USHORT org_stream,
							 USHORT new_stream, USHORT remap)
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
	JRD_NOD assign, node;
	JRD_REL relation;
	VEC fields;
	vec::iterator ptr, end;
	JRD_FLD field;
	LLS stack;
	USHORT id = 0, new_id = 0;
	DSC desc;

	SET_TDBB(tdbb);


	DEV_BLKCHK(csb, type_csb);

	stack = NULL;
	relation = csb->csb_rpt[org_stream].csb_relation;
	fields = relation->rel_fields;

	for (ptr = fields->begin(), end = fields->end(), id = 0;
			ptr < end; ptr++, id++)
		if (*ptr) {
			if (remap) {
				field = MET_get_field(relation, id);
				if (field->fld_source)
					new_id =
						(USHORT) (JRD_NOD) (field->fld_source)->nod_arg[e_fld_id];
				else
					new_id = id;
			}
			else
				new_id = id;
			node = PAR_gen_field(tdbb, new_stream, new_id);
			CMP_get_desc(tdbb, csb, node, &desc);
			if (!desc.dsc_address) {
				delete node;
				continue;
			}
			assign = PAR_make_node(tdbb, e_asgn_length);
			assign->nod_type = nod_assignment;
			assign->nod_arg[e_asgn_to] = node;
			assign->nod_arg[e_asgn_from] =
				PAR_gen_field(tdbb, org_stream, id);
			LLS_PUSH(assign, &stack);
		}

	return PAR_make_list(tdbb, stack);
}


static void pass1_modify(TDBB tdbb, CSB * csb, JRD_NOD node)
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
	JRD_NOD source, view_node;
	JRD_REL relation, parent, view;
	UCHAR *map;
	USHORT view_stream, stream, new_stream, parent_stream = 0;
	TRIG_VEC trigger;
	csb_repeat *tail;
	USHORT priv;

	SET_TDBB(tdbb);


	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(node, type_nod);

/* If updateable views with triggers are involved, there
   maybe a recursive call to be ignored */

	if (node->nod_arg[e_mod_sub_mod])
		return;

	parent = view = NULL;

/* To support views of views, loop until we hit a real relation */

	for (;;) {
		stream = (USHORT) node->nod_arg[e_mod_org_stream];
		new_stream = (USHORT) node->nod_arg[e_mod_new_stream];
		tail = &(*csb)->csb_rpt[new_stream];
		tail->csb_flags |= csb_modify;
		relation = (*csb)->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
		post_trigger_access(*csb, relation, ExternalAccess::exa_update, view);

		trigger =
			(relation->rel_pre_modify) ? relation->
			rel_pre_modify : relation->rel_post_modify;

		/* Check out update.  If this is an update thru a view, verify the
		   view by checking for read access on the base table.  If field-level select
		   privileges are implemented, this needs to be enhanced. */

		priv = SCL_sql_update;
		if (parent)
			priv |= SCL_read;
		if (!(source = pass1_update(tdbb, csb, relation, trigger, stream,
									new_stream, priv, parent, parent_stream))) {
			if ((*csb)->csb_rpt[new_stream].csb_flags & csb_view_update) {
				node->nod_arg[e_mod_map_view] =
					pass1_expand_view(tdbb, *csb, stream, new_stream, FALSE);
				node->nod_count =
					MAX(node->nod_count, (USHORT) e_mod_map_view + 1);
			}
			return;
		}

		parent = relation;
		parent_stream = stream;
		if (trigger) {
			node->nod_arg[e_mod_map_view] =
				pass1_expand_view(tdbb, *csb, stream, new_stream, FALSE);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_map_view + 1);
			map = (*csb)->csb_rpt[stream].csb_map;
			stream = (USHORT) source->nod_arg[e_rel_stream];
			stream = map[stream];
			view_stream = new_stream;

			/* Next, do update stream */

			map =
				alloc_map(tdbb, csb,
						  (SSHORT) node->nod_arg[e_mod_new_stream]);
			source = copy(tdbb, csb, source, map, 0, NULL, FALSE);
			/* TMN: Here we should really have the following assert */
			/* assert(source->nod_arg [e_rel_stream] <= MAX_UCHAR); */
			map[new_stream] = (UCHAR) source->nod_arg[e_rel_stream];
			view_node = copy(tdbb, csb, node, map, 0, NULL, TRUE);
			view_node->nod_arg[e_mod_org_stream] = (JRD_NOD) (SLONG) stream;
			view_node->nod_arg[e_mod_new_stream] =
				source->nod_arg[e_rel_stream];
			view_node->nod_arg[e_mod_map_view] = NULL;
			node->nod_arg[e_mod_sub_mod] = view_node;
			new_stream = (USHORT) source->nod_arg[e_rel_stream];
			view_node->nod_arg[e_mod_statement] =
				pass1_expand_view(tdbb, *csb, view_stream, new_stream, TRUE);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_sub_mod + 1);
			node = view_node;
		}
		else {
			(*csb)->csb_rpt[new_stream].csb_flags &= ~csb_view_update;

			/* View passes muster -- do some translation.  Start with source stream */

			map = (*csb)->csb_rpt[stream].csb_map;
			stream = (USHORT) source->nod_arg[e_rel_stream];
			node->nod_arg[e_mod_org_stream] = (JRD_NOD) (SLONG) map[stream];

			/* Next, do update stream */

			map =
				alloc_map(tdbb, csb,
						  (SSHORT) node->nod_arg[e_mod_new_stream]);
			source = copy(tdbb, csb, source, map, 0, NULL, FALSE);
			node->nod_arg[e_mod_new_stream] = source->nod_arg[e_rel_stream];
		}
	}
}


static RSE pass1_rse(TDBB    tdbb,
					 CSB*    csb,
					 RSE     rse,
					 JRD_REL view,
					 USHORT  view_stream)
{
/**************************************
 *
 *	p a s s 1 _ r s e
 *
 **************************************
 *
 * Functional description
 *	Process a record select expression during pass 1 of compilation.
 *	Mostly this involves expanding views.
 *
 **************************************/
	USHORT count;
	LLS stack, temp;
	JRD_NOD *arg, *end, boolean, sort, project, first, skip, plan;
	BOOLEAN writelock;
#ifdef SCROLLABLE_CURSORS
	JRD_NOD async_message;
#endif

	SET_TDBB(tdbb);


	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(view, type_rel);

/* for scoping purposes, maintain a stack of rse's which are 
   currently being parsed; if there are none on the stack as
   yet, mark the rse as variant to make sure that statement-
   level aggregates are not treated as invariants--bug #6535 */

	if ((*csb)->csb_current_rses.getCount() == 0)
		rse->nod_flags |= rse_variant;
	(*csb)->csb_current_rses.push(rse);
	
	stack = NULL;
	boolean = NULL;
	sort = rse->rse_sorted;
	project = rse->rse_projection;
	first = rse->rse_first;
	skip = rse->rse_skip;
	plan = rse->rse_plan;
	writelock = rse->rse_writelock;
#ifdef SCROLLABLE_CURSORS
	async_message = rse->rse_async_message;
#endif

/* Zip thru rse expanding views and inner joins */

	for (arg = rse->rse_relation, end = arg + rse->rse_count; arg < end; arg++)
	{
		pass1_source(tdbb, csb, rse, *arg, &boolean, &stack, view,
		             view_stream);
	}

/* Now, rebuild the rse block.  If possible, re-use the old block,
   otherwise allocate a new one. */

	for (count = 0, temp = stack; temp; temp = temp->lls_next)
		++count;

	if (count != rse->rse_count) {
		RSE new_rse;

		new_rse = (RSE) PAR_make_node(tdbb, count + rse_delta + 2);
		*new_rse = *rse;
		new_rse->rse_count = count;
		rse = new_rse;

/* AB: Because we've build an new rse, we must put this one in the stack 
  of current_rses else could rse's not be flagged an rse_variant.
  See SF BUG # [ 523589 ] for an example */
 		(*csb)->csb_current_rses.pop();
 		(*csb)->csb_current_rses.push(new_rse);
	}


	arg = rse->rse_relation + count;

	while (stack)
		*--arg = (JRD_NOD) LLS_POP(&stack);

/* Finish of by processing other clauses */

	if (first)
		rse->rse_first = pass1(tdbb, csb, first, view, view_stream, FALSE);
	if (skip)
		rse->rse_skip = pass1(tdbb, csb, skip, view, view_stream, FALSE);

	if (boolean) {
		if (rse->rse_boolean) {
			JRD_NOD additional;

			additional = PAR_make_node(tdbb, 2);
			additional->nod_type = nod_and;
			additional->nod_arg[0] = boolean;
			additional->nod_arg[1] =
				pass1(tdbb, csb, rse->rse_boolean, view, view_stream, FALSE);
			rse->rse_boolean = additional;
		}
		else
			rse->rse_boolean = boolean;
	}
	else
		rse->rse_boolean =
			pass1(tdbb, csb, rse->rse_boolean, view, view_stream, FALSE);

	if (sort)
		rse->rse_sorted = pass1(tdbb, csb, sort, view, view_stream, FALSE);

	if (project)
		rse->rse_projection =
			pass1(tdbb, csb, project, view, view_stream, FALSE);

	if (plan)
		rse->rse_plan = plan;
	
	rse->rse_writelock = writelock;

#ifdef SCROLLABLE_CURSORS
	if (async_message) {
		rse->rse_async_message =
			pass1(tdbb, csb, async_message, view, view_stream, FALSE);
		(*csb)->csb_async_message = rse->rse_async_message;
	}
#endif

/* we are no longer in the scope of this rse */

 	(*csb)->csb_current_rses.pop();

	return rse;
}


static void pass1_source(TDBB     tdbb,
						 CSB*     csb,
						 RSE      rse,
						 JRD_NOD  source,
						 JRD_NOD* boolean,
						 LLS*     stack,
						 JRD_REL  parent_view,
						 USHORT   view_stream)
{
/**************************************
 *
 *	p a s s 1 _ s o u r c e
 *
 **************************************
 *
 * Functional description
 *	Process a single record source stream from an rse.  Obviously,
 *	if the source is a view, there is more work to do.
 *
 **************************************/
	DBB dbb;
	RSE view_rse;
	JRD_NOD *arg, *end, node;
	JRD_REL view;
	UCHAR *map;
	USHORT stream;
	csb_repeat *element;

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(source, type_nod);
	DEV_BLKCHK(*boolean, type_nod);
	DEV_BLKCHK(*stack, type_lls);
	DEV_BLKCHK(parent_view, type_rel);

	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* in the case of an rse, it is possible that a new rse will be generated, 
   so wait to process the source before we push it on the stack (bug 8039) */

	if (source->nod_type == nod_rse)
	{
		/* The addition of the JOIN syntax for specifying inner joins causes an 
		   rse tree to be generated, which is undesirable in the simplest case 
		   where we are just trying to inner join more than 2 streams.  If possible, 
		   try to flatten the tree out before we go any further. */

		RSE sub_rse = (RSE) source;
		if (!rse->rse_jointype && !sub_rse->rse_jointype
			&& !sub_rse->rse_sorted && !sub_rse->rse_projection
			&& !sub_rse->rse_first && !sub_rse->rse_plan) {
			for (arg = sub_rse->rse_relation, end = arg + sub_rse->rse_count;
				 arg < end; arg++)
				pass1_source(tdbb, csb, rse, *arg, boolean, stack,
							 parent_view, view_stream);

			/* fold in the boolean for this inner join with the one for the parent */

			if (sub_rse->rse_boolean) {
				node =
					pass1(tdbb, csb, sub_rse->rse_boolean, parent_view,
						  view_stream, FALSE);
				if (*boolean) {
					JRD_NOD additional;

					additional = PAR_make_node(tdbb, 2);
					additional->nod_type = nod_and;
					additional->nod_arg[0] = node;
					additional->nod_arg[1] = *boolean;
					*boolean = additional;
				}
				else
					*boolean = node;
			}

			return;
		}

		source = pass1(tdbb, csb, source, parent_view, view_stream, FALSE);
		LLS_PUSH(source, stack);
		return;
	}

/* Assume that the source will be used.  Push it on the final
   stream stack */

	LLS_PUSH(source, stack);

/* Special case procedure */

	if (source->nod_type == nod_procedure) {
		JRD_PRC procedure;

		pass1(tdbb, csb, source, parent_view, view_stream, FALSE);
		procedure = MET_lookup_procedure_id(tdbb, 
		  (SSHORT)source->nod_arg[e_prc_procedure], FALSE, FALSE, 0);
		post_procedure_access(tdbb, *csb, procedure);
		CMP_post_resource(&(*csb)->csb_resources, (BLK) procedure,
						  Resource::rsc_procedure, procedure->prc_id);
		return;
	}

/* Special case union */

	if (source->nod_type == nod_union) {
		pass1(tdbb, csb, source->nod_arg[e_uni_clauses], parent_view,
			  view_stream, FALSE);
		return;
	}

/* Special case group-by/global aggregates */

	if (source->nod_type == nod_aggregate) {
		assert((int)source->nod_arg[e_agg_stream] <= MAX_STREAMS);
		pass1(tdbb, csb, source, parent_view, view_stream, FALSE);
		return;
	}

/* All the special cases are exhausted, so we must have a view or a base table;
   prepare to check protection of relation when a field in the stream of the
   relation is accessed */

	view = (JRD_REL) source->nod_arg[e_rel_relation];
	CMP_post_resource(&(*csb)->csb_resources, (BLK) view, Resource::rsc_relation,
					  view->rel_id);
	source->nod_arg[e_rel_view] = (JRD_NOD) parent_view;

	stream = (USHORT) source->nod_arg[e_rel_stream];
	element = CMP_csb_element(csb, stream);
	element->csb_view = parent_view;
/* TMN: Here we should really have the following assert */
/* assert(view_stream <= MAX_UCHAR); */
	element->csb_view_stream = (UCHAR) view_stream;

/* in the case where there is a parent view, find the context name */

	if (parent_view) {
		VCX *vcx_ptr;

		for (vcx_ptr = &parent_view->rel_view_contexts; *vcx_ptr;
			 vcx_ptr = &(*vcx_ptr)->vcx_next)
			if ((*vcx_ptr)->vcx_context ==
				(USHORT) source->nod_arg[e_rel_context]) {
				element->csb_alias = (*vcx_ptr)->vcx_context_name;
				break;
			}
	}

/* Check for a view -- if not, nothing more to do */

	if (!(view_rse = view->rel_view_rse)) {
		return;
	}

/* We've got a view, expand it */

	DEBUG;
	LLS_POP(stack);
	map = alloc_map(tdbb, csb, stream);

/* We don't expand the view in two cases: 
   1) If the view has a projection, and the query rse already has a projection 
      defined; there is probably some way to merge these projections and do them 
	  both at once, but for now we'll punt on that.
   2) If it's part of an outer join. 
   
   AB: If the view has an projection we never expand it.
   Because this can create wierd PLANs with multiple views/tables/sp joins
*/

	if ((view_rse->rse_projection)
	//if ((view_rse->rse_projection && rse->rse_projection)
		|| rse->rse_jointype) {
		node = copy(tdbb, csb, (JRD_NOD) view_rse, map, 0, NULL, FALSE);
		DEBUG;
		LLS_PUSH(pass1(tdbb, csb, node, view, stream, FALSE), stack);
		DEBUG;
		return;
	}

/* if we have a projection which we can bubble up to the parent rse, set the 
   parent rse to our projection temporarily to flag the fact that we have already 
   seen one so that lower-level views will not try to map their projection; the 
   projection will be copied and correctly mapped later, but we don't have all 
   the base streams yet */

	if (view_rse->rse_projection)
		rse->rse_projection = view_rse->rse_projection;

/* Disect view into component relations */

	for (arg = view_rse->rse_relation, end = arg + view_rse->rse_count;
		 arg < end; arg++) {
		/* this call not only copies the node, it adds any streams it finds to the map */

		node = copy(tdbb, csb, *arg, map, 0, NULL, FALSE);

		/* Now go out and process the base table itself.  This table might also be a view, 
		   in which case we will continue the process by recursion.  */

		pass1_source(tdbb, csb, rse, node, boolean, stack, view, stream);
	}

/* When there is a projection in the view, copy the projection up to the query rse.
   In order to make this work properly, we must remap the stream numbers of the fields
   in the view to the stream number of the base table.  Note that the map at this point 
   contains the stream numbers of the referenced relations, since it was added during the call 
   to copy() above.  After the copy() below, the fields in the projection will reference the 
   base table(s) instead of the view's context (see bug #8822), so we are ready to context- 
   recognize them in pass1()--that is, replace the field nodes with actual field blocks. */

	if (view_rse->rse_projection)
		rse->rse_projection =
			pass1(tdbb, csb,
				  copy(tdbb, csb, view_rse->rse_projection, map, 0, NULL, FALSE),
				  view, stream, FALSE);

/* If we encounter a boolean, copy it and retain it by ANDing it in with the 
   boolean on the parent view, if any. */

	if (view_rse->rse_boolean) {
		node =
			pass1(tdbb, csb,
				  copy(tdbb, csb, view_rse->rse_boolean, map, 0, NULL, FALSE), view,
				  stream, FALSE);
		if (*boolean) {
			JRD_NOD additional;

			/* The order of the nodes here is important!  The
			   boolean from the view must appear first so that
			   it gets expanded first in pass1. */

			additional = PAR_make_node(tdbb, 2);
			additional->nod_type = nod_and;
			additional->nod_arg[0] = node;
			additional->nod_arg[1] = *boolean;
			*boolean = additional;
		}
		else
			*boolean = node;
	}

	return;
}


static JRD_NOD pass1_store(TDBB tdbb, CSB * csb, JRD_NOD node)
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
	JRD_NOD source, original, view_node, very_orig;
	JRD_REL relation, parent, view;
	UCHAR *map;
	USHORT stream, new_stream, trigger_seen, parent_stream = 0;
	TRIG_VEC trigger;
	csb_repeat *tail;
	USHORT priv;

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(node, type_nod);

/* If updateable views with triggers are involved, there
   maybe a recursive call to be ignored */

	if (node->nod_arg[e_sto_sub_store])
		return NULL;

	parent = view = NULL;
	trigger_seen = FALSE;
	very_orig = node->nod_arg[e_sto_relation];

/* To support views of views, loop until we hit a real relation */

	for (;;) {
		original = node->nod_arg[e_sto_relation];
		stream = (USHORT) original->nod_arg[e_rel_stream];
		tail = &(*csb)->csb_rpt[stream];
		tail->csb_flags |= csb_store;
		relation = (*csb)->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
		post_trigger_access(*csb, relation, ExternalAccess::exa_insert, view);

		trigger =
			(relation->rel_pre_store) ? relation->
			rel_pre_store : relation->rel_post_store;

		/* Check out insert.  If this is an insert thru a view, verify the
		   view by checking for read access on the base table.  If field-level select
		   privileges are implemented, this needs to be enhanced. */

		priv = SCL_sql_insert;
		if (parent)
			priv |= SCL_read;
		if (!
			(source =
			 pass1_update(tdbb, csb, relation, trigger, stream, stream, priv,
						  parent, parent_stream))) {
			CMP_post_resource(&(*csb)->csb_resources, (BLK) relation,
							  Resource::rsc_relation, relation->rel_id);
			return very_orig;
		}

		/* View passes muster -- do some translation */

		parent = relation;
		parent_stream = stream;
		map = alloc_map(tdbb, csb, stream);
		if (!trigger) {
			(*csb)->csb_rpt[stream].csb_flags &= ~csb_view_update;
			node->nod_arg[e_sto_relation] =
				copy(tdbb, csb, source, map, 0, NULL, FALSE);
			if (!trigger_seen)
				very_orig = node->nod_arg[e_sto_relation];
		}
		else {
			CMP_post_resource(&(*csb)->csb_resources, (BLK) relation,
							  Resource::rsc_relation, relation->rel_id);
			trigger_seen = TRUE;
			view_node = copy(tdbb, csb, node, map, 0, NULL, FALSE);
			node->nod_arg[e_sto_sub_store] = view_node;
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_sto_sub_store + 1);
			view_node->nod_arg[e_sto_sub_store] = 0;
			node = view_node;
			node->nod_arg[e_sto_relation] =
				copy(tdbb, csb, source, map, 0, NULL, FALSE);
			new_stream =
				(USHORT) node->nod_arg[e_sto_relation]->nod_arg[e_rel_stream];
			node->nod_arg[e_sto_statement] =
				pass1_expand_view(tdbb, *csb, stream, new_stream, TRUE);
			node->nod_arg[e_sto_statement] =
				copy(tdbb, csb, node->nod_arg[e_sto_statement],
					 (UCHAR *) NULL, 0, NULL, FALSE);

			/* bug 8150: use of blr_store2 against a view with a trigger was causing 
			   the second statement to be executed, which is not desirable */

			node->nod_arg[e_sto_statement2] = NULL;
		}
	}
}


static JRD_NOD pass1_update(
						TDBB tdbb,
						CSB * csb,
						JRD_REL relation,
						TRIG_VEC trigger,
						USHORT stream,
USHORT update_stream, USHORT priv, JRD_REL view, USHORT view_stream)
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
	RSE rse;
	JRD_NOD node;

	SET_TDBB(tdbb);

	DEV_BLKCHK(*csb, type_csb);
	DEV_BLKCHK(relation, type_rel);
	//DEV_BLKCHK(trigger, type_vec);
	DEV_BLKCHK(view, type_rel);

/* Unless this is an internal request, check access permission */

	CMP_post_access(tdbb, *csb, relation->rel_security_name, 
					(view ? view->rel_id : 0), 
					priv, object_table, relation->rel_name);

/* ensure that the view is set for the input streams,
   so that access to views can be checked at the field level */

	CMP_csb_element(csb, stream)->csb_view = view;
/* TMN: Here we should really have the following assert */
/* assert(view_stream <= MAX_UCHAR); */
	CMP_csb_element(csb, stream)->csb_view_stream = (UCHAR) view_stream;
	CMP_csb_element(csb, update_stream)->csb_view = view;
/* TMN: Here we should really have the following assert */
/* assert(view_stream <= MAX_UCHAR); */
	CMP_csb_element(csb, update_stream)->csb_view_stream =
		(UCHAR) view_stream;

/* If we're not a view, everything's cool */

	if (!(rse = relation->rel_view_rse))
		return NULL;

/* We've got a view, is it updateable? */

	if (rse->rse_count != 1 ||
		rse->rse_projection ||
		rse->rse_sorted ||
		!(node = rse->rse_relation[0]) || node->nod_type != nod_relation) {
		/* We've got a non-updateable view.  If there's a trigger,
		   don't expand it */

		if (trigger) {
			(*csb)->csb_rpt[update_stream].csb_flags |= csb_view_update;
			return NULL;
		}
		else {
			ERR_post(gds_read_only_view,
					 gds_arg_string, relation->rel_name, 0);
			return ((JRD_NOD) NULL);	/* Added to remove compiler warnings */
		}
	}
	else {
		/* It's an updateable view, return the view source */

		(*csb)->csb_rpt[update_stream].csb_flags |= csb_view_update;
		return rse->rse_relation[0];
	}
}


static JRD_NOD pass2(TDBB tdbb, CSB csb, JRD_NOD node, JRD_NOD parent)
{
/**************************************
 *
 *	p a s s 2
 *
 **************************************
 *
 * Functional description
 *	Allocate and assign impure space for various nodes.
 *
 **************************************/
	JRD_NOD rse_node, *ptr, *end;
	ULONG id;
	USHORT stream;
	RSB *rsb_ptr;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);
	DEV_BLKCHK(parent, type_nod);

	if (!node)
		return node;

	if (parent)
		node->nod_parent = parent;

/* If there is processing to be done before sub expressions, do it here */

	DEBUG;
	rse_node = NULL;

	switch (node->nod_type) {
	case nod_rse:
		return NULL;

	case nod_union:
		return pass2_union(tdbb, csb, node);

	case nod_for:
		rse_node = node->nod_arg[e_for_re];
		rsb_ptr = (RSB *) & node->nod_arg[e_for_rsb];
#ifdef SCROLLABLE_CURSORS
		csb->csb_current_rse = rse_node;
#endif
		break;

#ifdef SCROLLABLE_CURSORS
	case nod_seek:
	case nod_seek_no_warn:
		/* store the rse in whose scope we are defined */

		node->nod_arg[e_seek_rse] = (JRD_NOD) csb->csb_current_rse;
		break;
#endif

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_count2:
	case nod_average:
	case nod_total:
	case nod_from:
		rse_node = node->nod_arg[e_stat_rse];
		if (!rse_node) {
			ERR_post(gds_wish_list, 0);
		}
		if (!(rse_node->nod_flags & rse_variant)) {
			node->nod_flags |= nod_invariant;
			csb->csb_invariants.push(node);
		}
		rsb_ptr = (RSB *) & node->nod_arg[e_stat_rsb];
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		rse_node = node->nod_arg[e_any_rse];
		if (!(rse_node->nod_flags & rse_variant)) {
			node->nod_flags |= nod_invariant;
			csb->csb_invariants.push(node);
		}
		rsb_ptr = (RSB *) & node->nod_arg[e_any_rsb];
		break;

	case nod_sort:
		ptr = node->nod_arg;
		for (end = ptr + node->nod_count; ptr < end; ptr++)
			(*ptr)->nod_flags |= nod_value;
		break;

	case nod_function:
		{
			JRD_NOD value;
			FUN function;

			value = node->nod_arg[e_fun_args];
			pass2(tdbb, csb, value, node);
			function = (FUN) node->nod_arg[e_fun_function];
			/* For gbak attachments, there is no need to resolve the UDF function */
			/* Also if we are dropping a procedure don't bother resolving the
			   UDF that the procedure invokes.
			 */
			if (!(tdbb->tdbb_attachment->att_flags & ATT_gbak_attachment) &&
				!(tdbb->tdbb_flags & TDBB_prc_being_dropped)) {
				node->nod_arg[e_fun_function] =
					(JRD_NOD) FUN_resolve(csb, function, value);
				if (!node->nod_arg[e_fun_function])
					ERR_post(gds_funmismat, gds_arg_string,
							 function->fun_symbol->sym_string, 0);
			}
		}
		break;

#ifdef PC_ENGINE
		/* the remainder of the node types are for IDAPI support:
		   fix up the stream to point to the base table, and preserve 
		   the pointers to the navigational rsb for easy reference 
		   later during execution */

	case nod_stream:
		{
			JRD_NOD relation;
			RSE rse;

			rse = (RSE) node;
			rse_node = node;
			/* setting the stream flag will allow the optimizer to  
			   detect that a SET INDEX may be done on this stream */
			rse_node->nod_flags |= rse_stream;
			rsb_ptr = &rse->rse_rsb;
			relation = rse->rse_relation[0];
			stream = base_stream(csb, &relation->nod_arg[e_rel_stream], TRUE);
			csb->csb_rpt[stream].csb_rsb_ptr = &rse->rse_rsb;
		}
		break;

	case nod_find:
		stream = base_stream(csb, &node->nod_arg[e_find_stream], TRUE);
		if (!
			(node->nod_arg[e_find_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_find_dbkey:
	case nod_find_dbkey_version:
		stream = base_stream(csb, &node->nod_arg[e_find_dbkey_stream], TRUE);
		if (!
			(node->nod_arg[e_find_dbkey_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_set_index:
		stream = base_stream(csb, &node->nod_arg[e_index_stream], TRUE);
		if (!
			(node->nod_arg[e_index_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_get_bookmark:
		stream = base_stream(csb, &node->nod_arg[e_getmark_stream], TRUE);
		if (!
			(node->nod_arg[e_getmark_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_set_bookmark:
		stream = base_stream(csb, &node->nod_arg[e_setmark_stream], TRUE);
		if (!
			(node->nod_arg[e_setmark_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_lock_record:
		stream = base_stream(csb, &node->nod_arg[e_lockrec_stream], TRUE);
		if (!
			(node->nod_arg[e_lockrec_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_crack:
	case nod_force_crack:
		stream = base_stream(csb, &node->nod_arg[0], TRUE);
		if (!(node->nod_arg[1] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
			ERR_post(gds__stream_not_defined, 0);
		break;

	case nod_reset_stream:
		stream = base_stream(csb, &node->nod_arg[e_reset_from_stream], TRUE);
		if (!
			(node->nod_arg[e_reset_from_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

	case nod_cardinality:
		stream = base_stream(csb, &node->nod_arg[e_card_stream], TRUE);
		if (!
			(node->nod_arg[e_card_rsb] =
			 (JRD_NOD) csb->
			 csb_rpt[stream].csb_rsb_ptr)) ERR_post(gds__stream_not_defined,
													0);
		break;

		/* the following DML nodes need to have their rsb's stored when 
		   they are referencing a navigational stream, so that we can
		   follow proper IDAPI semantics in manipulating a stream */

	case nod_erase:
		stream = base_stream(csb, &node->nod_arg[e_erase_stream], FALSE);
		node->nod_arg[e_erase_rsb] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr;
		break;

	case nod_modify:
		stream = base_stream(csb, &node->nod_arg[e_mod_org_stream], FALSE);
		node->nod_arg[e_mod_rsb] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr;
		break;
#endif

	default:
		break;
	}

	if (rse_node)
		pass2_rse(tdbb, csb, (RSE) rse_node);

/* Handle sub-expressions here */

	if (node->nod_type == nod_modify) {
		/* AB: Mark the streams involved with an UPDATE statement
		   active. So that the optimizer can use indices for 
		   eventually used sub-selects. */
		stream = (USHORT) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
		stream = (USHORT) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
	}

	ptr = node->nod_arg;

	for (end = ptr + node->nod_count; ptr < end; ptr++) {
		pass2(tdbb, csb, *ptr, node);
	}

	if (node->nod_type == nod_modify) {
		/* AB: Remove the previous flags */
		stream = (USHORT) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
		stream = (USHORT) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
	}

/* Handle any residual work */

	node->nod_impure = CMP_impure(csb, 0);

	switch (node->nod_type) {
	case nod_abort:
		pass2(tdbb, csb, node->nod_arg[e_xcp_msg], node);
		break;

	case nod_assignment:
		pass2(tdbb, csb, node->nod_arg[e_asgn_missing2], node);
		break;

	case nod_average:
	case nod_agg_average:
	case nod_agg_average_distinct:
		node->nod_flags |= nod_double;
		/* FALL INTO */

	case nod_max:
	case nod_min:
	case nod_from:
	case nod_count:
	case nod_agg_count2:
	case nod_agg_count_distinct:
	case nod_count2:
	case nod_agg_min:
	case nod_agg_max:
	case nod_agg_count:
		node->nod_count = 0;
		csb->csb_impure += sizeof(struct vlux);
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		if (node->nod_flags & nod_invariant)
			csb->csb_impure += sizeof(struct vlu);
		break;

	case nod_block:
		csb->csb_impure += sizeof(SLONG);
		break;

	case nod_dcl_variable:
		{
			DSC *desc;

			desc = (DSC *) (node->nod_arg + e_dcl_desc);
			csb->csb_impure += sizeof(struct vlu) + desc->dsc_length;
		}
		break;

	case nod_agg_total:
	case nod_agg_total_distinct:
	case nod_total:
	case nod_agg_total2:
	case nod_agg_total_distinct2:
		{
			DSC descriptor_a;

			node->nod_count = 0;
			csb->csb_impure += sizeof(struct vlu);
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
		}
		break;

	case nod_agg_average2:
	case nod_agg_average_distinct2:
		{
			DSC descriptor_a;

			node->nod_count = 0;
			csb->csb_impure += sizeof(struct vlux);
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
		}
		break;

	case nod_message:
		{
			FMT format;

			format = (FMT) node->nod_arg[e_msg_format];
			if (!((tdbb->tdbb_flags & TDBB_prc_being_dropped) && !format))
				csb->csb_impure += FB_ALIGN(format->fmt_length, 2);
		}
		break;

	case nod_modify:
		{
			FMT format;
			fmt::fmt_desc_iterator desc;

			stream = (USHORT) node->nod_arg[e_mod_org_stream];
			csb->csb_rpt[stream].csb_flags |= csb_update;
			format = CMP_format(tdbb, csb, stream);
			desc = format->fmt_desc.begin();
			for (id = 0; id < format->fmt_count; id++, desc++)
				if (desc->dsc_dtype)
					SBM_set(tdbb, &csb->csb_rpt[stream].csb_fields, id);
			csb->csb_impure += sizeof(struct sta);
		}
		break;

	case nod_list:
		node->nod_type = nod_asn_list;
		for (ptr = node->nod_arg; ptr < end; ptr++)
			if ((*ptr)->nod_type != nod_assignment) {
				node->nod_type = nod_list;
				break;
			}
		/* FALL INTO */

	case nod_store:
		csb->csb_impure += sizeof(struct sta);
		break;

	case nod_erase:
		stream = (USHORT) node->nod_arg[e_erase_stream];
		csb->csb_rpt[stream].csb_flags |= csb_update;
		break;

	case nod_field:
		stream = (USHORT) node->nod_arg[e_fld_stream];
		id = (USHORT) node->nod_arg[e_fld_id];
		SBM_set(tdbb, &csb->csb_rpt[stream].csb_fields, id);
		if (node->nod_flags & nod_value) {
			csb->csb_impure += sizeof(struct vlux);
			break;
		}
		/* FALL INTO */

	case nod_argument:
		csb->csb_impure += sizeof(struct dsc);
		break;

	case nod_concatenate:
	case nod_dbkey:
	case nod_rec_version:
	case nod_negate:
	case nod_substr:
	case nod_divide:
	case nod_null:
	case nod_user_name:
    case nod_current_role:
	case nod_internal_info:
	case nod_gen_id:
	case nod_gen_id2:
	case nod_upcase:
	case nod_prot_mask:
	case nod_lock_state:
#ifdef PC_ENGINE
	case nod_lock_record:
	case nod_lock_relation:
#endif
	case nod_scalar:
	case nod_cast:
	case nod_extract:
	case nod_current_time:
	case nod_current_timestamp:
	case nod_current_date:
#ifdef PC_ENGINE
	case nod_cardinality:
	case nod_seek:
	case nod_seek_no_warn:
	case nod_crack:
	case nod_begin_range:
#endif
		{
			DSC descriptor_a;

			CMP_get_desc(tdbb, csb, node, &descriptor_a);
			csb->csb_impure += sizeof(struct vlu);
		}
		break;

	/* Compute the target descriptor to compute computational class */

	case nod_multiply:
	case nod_add:
	case nod_subtract:
	case nod_function:
	case nod_add2:
	case nod_subtract2:
	case nod_multiply2:
	case nod_divide2:
		{
			DSC descriptor_a;

			CMP_get_desc(tdbb, csb, node, &descriptor_a);
			csb->csb_impure += sizeof(struct vlu);
		}
		break;

	case nod_aggregate:
		pass2_rse(tdbb, csb, (RSE) node->nod_arg[e_agg_rse]);
		pass2(tdbb, csb, node->nod_arg[e_agg_map], node);
		pass2(tdbb, csb, node->nod_arg[e_agg_group], node);
		stream = (USHORT) node->nod_arg[e_agg_stream];
		assert(stream <= MAX_STREAMS);
		process_map(tdbb, csb, node->nod_arg[e_agg_map],
					&csb->csb_rpt[stream].csb_format);
		break;

		/* Boolean Nodes taking three values as inputs */
	case nod_like:
	case nod_between:
	case nod_sleuth:
		if (node->nod_count > 2) {
			DSC descriptor_c;

			if (node->nod_arg[2]->nod_flags & nod_agg_dbkey)
				ERR_post(gds_bad_dbkey, 0);
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_c);
			if (DTYPE_IS_DATE(descriptor_c.dsc_dtype)) {
				node->nod_arg[0]->nod_flags |= nod_date;
				node->nod_arg[1]->nod_flags |= nod_date;
			}
		};
		/* FALLINTO */

		/* Boolean Nodes taking two values as inputs */
	case nod_matches:
	case nod_contains:
	case nod_starts:
	case nod_eql:
	case nod_neq:
	case nod_geq:
	case nod_gtr:
	case nod_lss:
	case nod_leq:
		{
			DSC descriptor_a, descriptor_b;

			if ((node->nod_arg[0]->nod_flags & nod_agg_dbkey) ||
				(node->nod_arg[1]->nod_flags & nod_agg_dbkey))
				ERR_post(gds_bad_dbkey, 0);
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &descriptor_b);
			if (DTYPE_IS_DATE(descriptor_a.dsc_dtype))
				node->nod_arg[1]->nod_flags |= nod_date;
			else if (DTYPE_IS_DATE(descriptor_b.dsc_dtype))
				node->nod_arg[0]->nod_flags |= nod_date;
		}
		break;

		/* Boolean nodes taking 1 Value as input */
	case nod_missing:
		{
			DSC descriptor_a;

			if (node->nod_arg[0]->nod_flags & nod_agg_dbkey)
				ERR_post(gds_bad_dbkey, 0);

			/* Check for syntax errors in the calculation */
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
		}
		break;

	case nod_exec_into:
		csb->csb_impure += sizeof(class ExecuteStatement);
		break;

	default:
		/* Note: no assert (FALSE); here as too many nodes are missing */
		break;
	}

/* Finish up processing of record selection expressions */

	if (rse_node)
		*rsb_ptr = post_rse(tdbb, csb, (RSE) rse_node);

	return node;
}


static void pass2_rse(TDBB tdbb, CSB csb, RSE rse)
{
/**************************************
 *
 *	p a s s 2 _ r s e
 *
 **************************************
 *
 * Functional description
 *	Perform the first half of record selection expression compilation.
 *	The actual optimization is done in "post_rse".
 *
 **************************************/
	JRD_NOD *ptr, *end;

	SET_TDBB(tdbb);
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);

	if (rse->rse_first)
		pass2(tdbb, csb, rse->rse_first, 0);
	if (rse->rse_skip)
	    pass2(tdbb, csb, rse->rse_skip, 0);

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count;
		 ptr < end; ptr++) {
		JRD_NOD node;

		node = *ptr;
		if (node->nod_type == nod_relation) {
			USHORT stream = (USHORT) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags |= csb_active;
			pass2(tdbb, csb, node, (JRD_NOD) rse);
		}
		else if (node->nod_type == nod_rse) {
			pass2_rse(tdbb, csb, (RSE) node);
		}
		else if (node->nod_type == nod_procedure) {
			USHORT stream = (USHORT) node->nod_arg[e_prc_stream];
			csb->csb_rpt[stream].csb_flags |= csb_active;
			pass2(tdbb, csb, node, (JRD_NOD) rse);
		}
		else if (node->nod_type == nod_aggregate) {
			USHORT stream = (USHORT) node->nod_arg[e_agg_stream];
			assert(stream <= MAX_STREAMS);
			csb->csb_rpt[stream].csb_flags |= csb_active;
			pass2(tdbb, csb, node, (JRD_NOD) rse);
		}
		else
			pass2(tdbb, csb, node, (JRD_NOD) rse);
	}

	if (rse->rse_boolean)
		pass2(tdbb, csb, rse->rse_boolean, 0);

	if (rse->rse_sorted)
		pass2(tdbb, csb, rse->rse_sorted, 0);

	if (rse->rse_projection)
		pass2(tdbb, csb, rse->rse_projection, 0);

/* if the user has submitted a plan for this rse, check it for correctness */

	if (rse->rse_plan) {
		plan_set(csb, rse, rse->rse_plan);
		plan_check(csb, rse);
	}

#ifdef SCROLLABLE_CURSORS
	if (rse->rse_async_message)
		pass2(tdbb, csb, rse->rse_async_message, 0);
#endif
}


static JRD_NOD pass2_union(TDBB tdbb, CSB csb, JRD_NOD node)
{
/**************************************
 *
 *	p a s s 2 _ u n i o n
 *
 **************************************
 *
 * Functional description
 *	Process a union clause of an rse.
 *
 **************************************/
	JRD_NOD clauses, *ptr, *end, map;
	FMT *format;
	USHORT id;

	SET_TDBB(tdbb);
	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

/* Make up a format block sufficiently large to hold instantiated record */

	clauses = node->nod_arg[e_uni_clauses];
	id = (USHORT) node->nod_arg[e_uni_stream];
	format = &csb->csb_rpt[id].csb_format;

/* Process alternating rse and map blocks */

	for (ptr = clauses->nod_arg, end = ptr + clauses->nod_count; ptr < end;) {
		pass2_rse(tdbb, csb, (RSE) * ptr++);
		map = *ptr++;
		pass2(tdbb, csb, map, node);
		process_map(tdbb, csb, map, format);
	}

	return node;
}


static void plan_check(CSB csb, RSE rse)
{
/**************************************
 *
 *	p l a n _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check that all streams in the rse have 
 *	a plan specified for them.
 *	If they are not, there are streams
 *	in the rse which were not mentioned
 *	in the plan. 
 *
 **************************************/
	JRD_NOD *ptr, *end;
	USHORT stream;

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);

/* if any streams are not marked with a plan, give an error */

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end;
		 ptr++) if ((*ptr)->nod_type == nod_relation) {
			stream = (USHORT) (*ptr)->nod_arg[e_rel_stream];
			if (!(csb->csb_rpt[stream].csb_plan))
				ERR_post(gds_no_stream_plan, gds_arg_string,
						 csb->csb_rpt[stream].csb_relation->rel_name, 0);
		}
}


static void plan_set(CSB csb, RSE rse, JRD_NOD plan)
{
/**************************************
 *
 *	p l a n _ s e t
 *
 **************************************
 *
 * Functional description
 *	Go through the streams in the plan, find the 
 *	corresponding streams in the rse and store the 
 *	plan for that stream.   Do it once and only once 
 *	to make sure there is a one-to-one correspondence 
 *	between streams in the query and streams in
 *	the plan.
 *
 **************************************/
	JRD_NOD plan_relation_node, *ptr, *end;
	USHORT stream;
	UCHAR *map, *map_base, *duplicate_map;
	JRD_REL relation, plan_relation, view_relation, duplicate_relation;
	STR alias, plan_alias;
	TEXT *p;
	csb_repeat *tail, *duplicate_tail;

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(plan, type_nod);

	if (plan->nod_type == nod_join || plan->nod_type == nod_merge) {
		for (ptr = plan->nod_arg, end = ptr + plan->nod_count; ptr < end;
			 ptr++)
			plan_set(csb, rse, *ptr);
		return;
	}

	if (plan->nod_type != nod_retrieve)
		return;

	plan_relation_node = plan->nod_arg[e_retrieve_relation];
	plan_relation = (JRD_REL) plan_relation_node->nod_arg[e_rel_relation];
	plan_alias = (STR) plan_relation_node->nod_arg[e_rel_alias];

/* find the tail for the relation specified in the rse */

	stream = (USHORT) plan_relation_node->nod_arg[e_rel_stream];
	tail = &csb->csb_rpt[stream];

/* if the plan references a view, find the real base relation 
   we are interested in by searching the view map */

	if (tail->csb_map) {
		if (plan_alias)
			p = (TEXT *) plan_alias->str_data;
		else
			p = "\0";

		/* if the user has specified an alias, skip past it to find the alias 
		   for the base table (if multiple aliases are specified) */

		if (*p &&
			(tail->csb_relation
			 && !strcmp_space(tail->csb_relation->rel_name, p))
			|| (tail->csb_alias
				&& !strcmp_space(reinterpret_cast <
								 char *>(tail->csb_alias->str_data), p))) {
			while (*p && *p != ' ')
				p++;
			if (*p == ' ')
				p++;
		}

		/* loop through potentially a stack of views to find the appropriate base table */

		while ( (map_base = tail->csb_map) ) {
			map = map_base;
			tail = &csb->csb_rpt[*map];
			view_relation = tail->csb_relation;

			/* if the plan references the view itself, make sure that
			   the view is on a single table; if it is, fix up the plan
			   to point to the base relation */

			if (view_relation->rel_id == plan_relation->rel_id) {
				if (!map_base[2]) {
					map++;
					tail = &csb->csb_rpt[*map];
				}
				else
					/* view %s has more than one base relation; use aliases to distinguish */
					ERR_post(gds_view_alias, gds_arg_string,
							 plan_relation->rel_name, 0);

				break;
			}
			else
				view_relation = NULL;

			/* if the user didn't specify an alias (or didn't specify one
			   for this level), check to make sure there is one and only one 
			   base relation in the table which matches the plan relation */

			if (!*p) {
				duplicate_relation = NULL;
				duplicate_map = map_base;
				map = NULL;
				for (duplicate_map++; *duplicate_map; duplicate_map++) {
					duplicate_tail = &csb->csb_rpt[*duplicate_map];
					relation = duplicate_tail->csb_relation;
					if (relation && relation->rel_id == plan_relation->rel_id) {
						if (duplicate_relation)
							/* table %s is referenced twice in view; use an alias to distinguish */
							ERR_post(gds_duplicate_base_table,
									 gds_arg_string,
									 duplicate_relation->rel_name, 0);
						else {
							duplicate_relation = relation;
							map = duplicate_map;
							tail = duplicate_tail;
						}
					}
				}

				break;
			}

			/* look through all the base relations for a match */

			map = map_base;
			for (map++; *map; map++) {
				tail = &csb->csb_rpt[*map];
				relation = tail->csb_relation;
				alias = tail->csb_alias;

				/* match the user-supplied alias with the alias supplied
				   with the view definition; failing that, try the base
				   table name itself */

				/* CVC: I found that "relation" can be NULL, too. This may be an
				indication of a logic flaw while parsing the user supplied SQL plan
				and not an oversight here. It's hard to imagine a csb->csb_rpt with
				a NULL relation. See exe.h for csb struct and its inner csb_repeat struct. */

				if (
					(alias
					 && !strcmp_space(reinterpret_cast <
									  char *>(alias->str_data), p))
					|| (relation && !strcmp_space(relation->rel_name, p)))
					  break;
			}

			/* skip past the alias */

			while (*p && *p != ' ')
				p++;
			if (*p == ' ')
				p++;

			if (!*map)
				/* table %s is referenced in the plan but not the from list */
				ERR_post(gds_stream_not_found, gds_arg_string,
						 plan_relation->rel_name, 0);
		}

		/* fix up the relation node to point to the base relation's stream */

		if (!map || !*map)
			/* table %s is referenced in the plan but not the from list */
			ERR_post(gds_stream_not_found, gds_arg_string,
					 plan_relation->rel_name, 0);

		plan_relation_node->nod_arg[e_rel_stream] = (JRD_NOD) (SLONG) * map;
	}

/* make some validity checks */

	if (!tail->csb_relation)
		/* table %s is referenced in the plan but not the from list */
		ERR_post(gds_stream_not_found, gds_arg_string,
				 plan_relation->rel_name, 0);

	if ((tail->csb_relation->rel_id != plan_relation->rel_id)
		&& !view_relation)
		/* table %s is referenced in the plan but not the from list */
		ERR_post(gds_stream_not_found, gds_arg_string,
				 plan_relation->rel_name, 0);

/* check if we already have a plan for this stream */

	if (tail->csb_plan)
		/* table %s is referenced more than once in plan; use aliases to distinguish */
		ERR_post(gds_stream_twice, gds_arg_string,
				 tail->csb_relation->rel_name, 0);

	tail->csb_plan = plan;
}


static void post_procedure_access(TDBB tdbb, CSB csb, JRD_PRC procedure)
{
/**************************************
 *
 *	p o s t _ p r o c e d u r e _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *
 *	The request will inherit access requirements to all the objects
 *	the called stored procedure has access requirements for.
 *
 **************************************/
	TEXT *prc_sec_name;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(procedure, type_prc);

      	// allow all access to internal requests

	if (csb->csb_g_flags & (csb_internal | csb_ignore_perm))
                return;

	prc_sec_name = (procedure->prc_security_name ?
					(TEXT *) procedure->
					prc_security_name->str_data : (TEXT *) 0);

/* This request must have EXECUTE permission on the stored procedure */
	CMP_post_access(tdbb, csb, prc_sec_name, 0,
					SCL_execute,
					object_procedure,
					reinterpret_cast <
					char *>(procedure->prc_name->str_data));

	// Add the procedure to list of external objects accessed
	ExternalAccess temp(procedure->prc_id);
	int idx;
	if (!csb->csb_external.find(temp, idx))
		csb->csb_external.insert(idx, temp);
}


static RSB post_rse(TDBB tdbb, CSB csb, RSE rse)
{
/**************************************
 *
 *	p o s t _ r s e
 *
 **************************************
 *
 * Functional description
 *	Perform actual optimization of an rse and clear activity.
 *
 **************************************/
	RSB rsb;
	JRD_NOD node, *ptr, *end;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);

	rsb = OPT_compile(tdbb, csb, rse, NULL);

	if (rse->nod_flags & rse_singular)
		rsb->rsb_flags |= rsb_singular;

#ifdef PC_ENGINE
/* this flag lets the VIO layer know to add a page to the 
   cache range */
	if (rse->nod_flags & rse_stream)
		rsb->rsb_flags |= rsb_stream_type;
#endif

/* mark all the substreams as inactive */

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count;
		 ptr < end; ptr++) {
		node = *ptr;
		if (node->nod_type == nod_relation) {
			USHORT stream = (USHORT) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
		else if (node->nod_type == nod_procedure) {
			USHORT stream = (USHORT) node->nod_arg[e_prc_stream];
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
		else if (node->nod_type == nod_aggregate) {
			USHORT stream = (USHORT) node->nod_arg[e_agg_stream];
			assert(stream <= MAX_STREAMS);
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
	}

	csb->csb_fors.push(rsb);
#ifdef SCROLLABLE_CURSORS
	rse->rse_rsb = rsb;
#endif

	return rsb;
}


static void post_trigger_access(CSB csb, JRD_REL owner_relation, ExternalAccess::exa_act operation, JRD_REL view)
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
	int i;
	if (!csb->csb_external.find(temp, i))
		csb->csb_external.insert(i, temp);
}


static void process_map(TDBB tdbb, CSB csb, JRD_NOD map, FMT * input_format)
{
/**************************************
 *
 *	p r o c e s s _ m a p
 *
 **************************************
 *
 * Functional description
 *	Translate a map block into a format.  If the format is
 *	is missing or incomplete, extend it.
 *
 **************************************/
	JRD_NOD *ptr, *end, assignment, field;
	FMT format;
	DSC *desc, desc2;
	USHORT id, min, max, align;

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(map, type_nod);
	DEV_BLKCHK(*input_format, type_fmt);

	SET_TDBB(tdbb);

	if (!(format = *input_format)) {
		format = *input_format = fmt::newFmt(*tdbb->tdbb_default, map->nod_count);
		format->fmt_count = map->nod_count;
	}

/* Process alternating rse and map blocks */

	ptr = map->nod_arg;

	for (end = ptr + map->nod_count; ptr < end; ptr++) {
		assignment = *ptr;
		field = assignment->nod_arg[e_asgn_to];
		id = (USHORT) field->nod_arg[e_fld_id];
		if (id >= format->fmt_count) {
			format->fmt_desc.resize(id + 1);
		}
		desc = &format->fmt_desc[id];
		CMP_get_desc(tdbb, csb, assignment->nod_arg[e_asgn_from], &desc2);
		min = MIN(desc->dsc_dtype, desc2.dsc_dtype);
		max = MAX(desc->dsc_dtype, desc2.dsc_dtype);
		if (max == dtype_blob) {
			desc->dsc_dtype = dtype_quad;
			desc->dsc_length = 8;
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
			desc->dsc_flags = 0;
		}
		else if (!min)			/* eg: dtype_null */
			*desc = desc2;
		else if (min <= dtype_any_text) {	/* either field a text field? */
			USHORT len1, len2;

			len1 = DSC_string_length(desc);
			len2 = DSC_string_length(&desc2);
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length = MAX(len1, len2) + sizeof(USHORT);

			/* pick the Max text type, so any transparent casts from ints are 
			   not left in ASCII format, but converted to the richer text format. */

			INTL_ASSIGN_TTYPE(desc,
							  MAX(INTL_TEXT_TYPE(*desc),
								  INTL_TEXT_TYPE(desc2)));
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		else if (DTYPE_IS_DATE(max) && !DTYPE_IS_DATE(min)) {
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length =
				DSC_convert_to_text_length(max) + sizeof(USHORT);
			desc->dsc_ttype = ttype_ascii;
			desc->dsc_scale = 0;
			desc->dsc_flags = 0;
		}
		else if (max != min) {
			/* different numeric types: if one is inexact use double,
			   if both are exact use int64. */
			if ((!DTYPE_IS_EXACT(max)) || (!DTYPE_IS_EXACT(min))) {
				desc->dsc_dtype = DEFAULT_DOUBLE;
				desc->dsc_length = sizeof(double);
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
			}
			else {
				desc->dsc_dtype = dtype_int64;
				desc->dsc_length = sizeof(SINT64);
				desc->dsc_scale = MIN(desc->dsc_scale, desc2.dsc_scale);
				desc->dsc_sub_type =
					MAX(desc->dsc_sub_type, desc2.dsc_sub_type);
				desc->dsc_flags = 0;
			}
		}
	}

/* Flesh out the format of the record */

/* TMN: Here we should really have the following assert */
/* assert(FLAG_BYTES (format->fmt_count) <= MAX_USHORT); */
	format->fmt_length = (USHORT) FLAG_BYTES(format->fmt_count);
	fmt::fmt_desc_iterator desc3, end_desc;

	for (desc3 = format->fmt_desc.begin(), end_desc= format->fmt_desc.end();
		 desc3 < end_desc; desc3++) {
		align = type_alignments[desc3->dsc_dtype];
		if (align)
			format->fmt_length = FB_ALIGN(format->fmt_length, align);
		desc3->dsc_address = (UCHAR *) (SLONG) format->fmt_length;
		format->fmt_length += desc3->dsc_length;
	}
}


static SSHORT strcmp_space(TEXT * p, TEXT * q)
{
/**************************************
 *
 *	s t r c m p _ s p a c e
 *
 **************************************
 *
 * Functional description
 *	Compare two strings, which could be either
 *	space-terminated or null-terminated.
 *
 **************************************/

	for (; *p && *p != ' ' && *q && *q != ' '; p++, q++)
		if (*p != *q)
			break;

	if ((!*p || *p == ' ') && (!*q || *q == ' '))
		return 0;

	if (*p > *q)
		return 1;
	else
		return -1;
}


static BOOLEAN stream_in_rse(USHORT stream, RSE rse)
{
/**************************************
 *
 *	s t r e a m _ i n _ r s e
 *
 **************************************
 *
 * Functional description
 *	Return TRUE if stream is contained in 
 *	the specified RSE.
 *
 **************************************/
	JRD_NOD sub, *ptr, *end;

	DEV_BLKCHK(rse, type_nod);

/* look through all relation nodes in this rse to see 
   if the field references this instance of the relation */

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end;
		 ptr++) {
		sub = *ptr;

		/* for aggregates, check current rse, if not found then check 
		   the sub-rse */
		if (sub->nod_type == nod_aggregate) {
			if ((stream == (USHORT) sub->nod_arg[e_rel_stream]) ||
				(stream_in_rse(stream, (RSE) sub->nod_arg[e_agg_rse])))
				return TRUE;	/* do not mark as variant */
		}

		if ((sub->nod_type == nod_relation) &&
			(stream == (USHORT) sub->nod_arg[e_rel_stream]))
			return TRUE;		/* do not mark as variant */
	}

	return FALSE;				/* mark this rse as variant */
}
