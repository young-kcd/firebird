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
 */

#include "firebird.h"
#include "../jrd/ibsetjmp.h"
#include <string.h>
#include <stdlib.h>				// abort
#include "../jrd/common.h"
#include "../jrd/y_ref.h"
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
#include "../jrd/dbg_proto.h"	// DBG_supervisor
#include "../jrd/ExecuteStatement.h"

/* Pick up relation ids */

#define RELATION(name,id,ods)	id,
#define FIELD(symbol,name,id,update,ods,new_id,new_ods)
#define END_RELATION

typedef enum rids {
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

#define REQ_TAIL		sizeof (((JRD_REQ) NULL)->req_rpb[0])
#define MAP_LENGTH		256

/* RITTER - changed HP10 to HPUX */
#if defined (HPUX) && defined (SUPERSERVER)
#define MAX_RECURSION		96
#endif

#ifndef MAX_RECURSION
#define MAX_RECURSION		128
#endif

#define MAX_REQUEST_SIZE	10485760	// 10 MB - just to be safe

static UCHAR* alloc_map(TDBB, CSB, USHORT);
static JRD_NOD catenate_nodes(TDBB, LLS);
static JRD_NOD copy(TDBB, CSB, JRD_NOD, UCHAR *, USHORT, bool);
static void expand_view_nodes(TDBB, CSB, USHORT, LLS *, NOD_T);
static void ignore_dbkey(TDBB, CSB, RSE, JRD_REL);
static JRD_NOD make_defaults(TDBB, CSB, USHORT, JRD_NOD);
static JRD_NOD make_validation(TDBB, CSB, USHORT);
static JRD_NOD pass1(TDBB, CSB, JRD_NOD, JRD_REL, USHORT, bool);
static void pass1_erase(TDBB, CSB, JRD_NOD);
static JRD_NOD pass1_expand_view(TDBB, CSB, USHORT, USHORT, bool);
static void pass1_modify(TDBB, CSB, JRD_NOD);
static RSE pass1_rse(TDBB, CSB, RSE, JRD_REL, USHORT);
static void pass1_source(TDBB, CSB, RSE, JRD_NOD, JRD_NOD *, LLS *, JRD_REL, USHORT);
static JRD_NOD pass1_store(TDBB, CSB, JRD_NOD);
static JRD_NOD pass1_update(TDBB, CSB, JRD_REL, TRIG_VEC, USHORT, USHORT, USHORT, JRD_REL,
						USHORT);
static JRD_NOD pass2(TDBB, CSB, JRD_NOD, JRD_NOD);
static void pass2_rse(TDBB, CSB, RSE);
static JRD_NOD pass2_union(TDBB, CSB, JRD_NOD);
static void plan_check(CSB, RSE);
static void plan_set(CSB, RSE, JRD_NOD);
static void post_procedure_access(TDBB, CSB, JRD_PRC);
static RSB post_rse(TDBB, CSB, RSE);
static void	post_trigger_access(TDBB, CSB, JRD_REL, TRIG_VEC, JRD_REL);
static void process_map(TDBB, CSB, JRD_NOD, FMT *);
static bool stream_in_rse(USHORT, RSE);
static SSHORT strcmp_space(const TEXT*, const TEXT*);

#ifdef PC_ENGINE
static USHORT base_stream(CSB, JRD_NOD *, bool);
#endif

#ifdef CMP_DEBUG
IMPLEMENT_TRACE_ROUTINE(cmp_trace, "CMP");
#endif

inline static int strcmp_null(const char* s1, const char* s2) {
	return s1 == NULL ? s2 != NULL : s2 == NULL ? -1 : strcmp(s1, s2);
}

inline static char* clone_cstring(JrdMemoryPool* pool, const char* source) {
	if (!source) return NULL;
	char *result = FB_NEW(*pool) char[strlen(source)+1];
    strcpy(result, source);
    return result;
}

bool CMP_clone_active(JRD_REQ request)
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
		return true;

	if ( (vector = request->req_sub_requests) )
		for (sub_req = vector->begin(), end = vector->end();
			 sub_req < end; sub_req++)
			if (*sub_req && ((JRD_REQ)(*sub_req))->req_flags & req_in_use)
				return true;

	return false;
}


JRD_NOD CMP_clone_node(TDBB tdbb, CSB csb, JRD_NOD node)
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

	clone = copy(tdbb, csb, node, NULL, 0, false);
	pass2(tdbb, csb, clone, 0);

	return clone;
}


JRD_REQ CMP_clone_request(TDBB tdbb, JRD_REQ request, USHORT level, bool validate)
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
	ACC access;
	SCL class_;
	JRD_PRC procedure;
	TEXT *prc_sec_name;

	DEV_BLKCHK(request, type_req);

	SET_TDBB(tdbb);

	// find the request if we've got it

	if (!level)
		return request;

	VEC vector = request->req_sub_requests;

	if (vector && level < vector->count() && (clone = (JRD_REQ) (*vector)[level]))
		return clone;

	// validate the request

	if (validate) {
		if ( (procedure = request->req_procedure) ) {
			prc_sec_name = (procedure->prc_security_name ?
							(TEXT *) procedure->
							prc_security_name->str_data : NULL);
			class_ = SCL_get_class(prc_sec_name);
			SCL_check_access(class_, 0, 0,
							 0, SCL_execute, object_procedure,
							 reinterpret_cast <
							 char *>(procedure->prc_name->str_data));
		}
		for (access = request->req_access; access; access = access->acc_next) {
			class_ = SCL_get_class(access->acc_security_name);
			SCL_check_access(class_, access->acc_view_id, access->acc_trg_name,
							 access->acc_prc_name, access->acc_mask,
							 access->acc_type, access->acc_name);
		}
	}

	// we need to clone the request - find someplace to put it

	vector = request->req_sub_requests =
		vec::newVector(*request->req_pool, request->req_sub_requests, level + 1);

	// clone the request

	USHORT n = (USHORT) ((request->req_impure_size - REQ_SIZE + REQ_TAIL - 1) / REQ_TAIL);
	clone = FB_NEW_RPT(*request->req_pool, n) jrd_req;
	(*vector)[level] = (BLK) clone;
	clone->req_attachment = tdbb->tdbb_attachment;
	clone->req_count = request->req_count;
	clone->req_pool = request->req_pool;
	clone->req_impure_size = request->req_impure_size;
	clone->req_top_node = request->req_top_node;
	clone->req_trg_name = request->req_trg_name;
	clone->req_flags = request->req_flags & REQ_FLAGS_CLONE_MASK;
	clone->req_last_xcp = request->req_last_xcp;

	rpb1 = clone->req_rpb;
	end = rpb1 + clone->req_count;

	for (rpb2 = request->req_rpb; rpb1 < end; rpb1++, rpb2++) {
		if (rpb2->rpb_stream_flags & RPB_s_update)
			rpb1->rpb_stream_flags |= RPB_s_update;
		rpb1->rpb_relation = rpb2->rpb_relation;
	}

	return clone;
}


JRD_REQ CMP_compile(USHORT blr_length, const UCHAR* blr, USHORT internal_flag)
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


JRD_REQ CMP_compile2(TDBB tdbb, const UCHAR* blr, USHORT internal_flag)
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
	ACC access;

	SET_TDBB(tdbb);

	JrdMemoryPool* old_pool = tdbb->tdbb_default;
	// 26.09.2002 Nickolay Samofatov: default memory pool will become statement pool 
	// and will be freed by CMP_release
	JrdMemoryPool* new_pool = JrdMemoryPool::createPool();
	tdbb->tdbb_default = new_pool;

	try {

		CSB csb = PAR_parse(tdbb, blr, internal_flag);
		request = CMP_make_request(tdbb, csb);

		if (internal_flag) {
			request->req_flags |= req_internal;
		}

		for (access = request->req_access; access; access = access->acc_next)
		{
			SCL class_ = SCL_get_class(access->acc_security_name);
			SCL_check_access(class_, access->acc_view_id, access->acc_trg_name,
							 access->acc_prc_name, access->acc_mask,
							 access->acc_type, access->acc_name);
		}

		delete csb;
		tdbb->tdbb_default = old_pool;

	}
	catch (const std::exception&) {
		tdbb->tdbb_default = old_pool;
		if (request) {
			CMP_release(tdbb, request);
		} else if (new_pool) {
			JrdMemoryPool::deletePool(new_pool);
		}
		ERR_punt();
	}

	return request;
}


csb_repeat* CMP_csb_element(CSB csb, USHORT element)
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

	DEV_BLKCHK(csb, type_csb);
	csb_repeat empty_item;
	while (element >= csb->csb_rpt.getCount()) {
		csb->csb_rpt.add(empty_item);
	}
	return &csb->csb_rpt[element];
}


void CMP_expunge_transaction(JRD_TRA transaction)
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


JRD_REQ CMP_find_request(TDBB tdbb, USHORT id, USHORT which)
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

	// if the request hasn't been compiled or isn't active,
	// there're nothing to do

	THD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
	if ((which == IRQ_REQUESTS && !(request = (JRD_REQ) REQUEST(id))) ||
		(which == DYN_REQUESTS && !(request = (JRD_REQ) DYN_REQUEST(id))) ||
		!(request->req_flags & (req_active | req_reserved))) {
		if (request)
			request->req_flags |= req_reserved;
		THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
		return request;
	}

	// Request exists and is in use. Look for clones until we find
	// one that is available.

	for (n = 1;; n++) {
		if (n > MAX_RECURSION) {
			THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
			ERR_post(isc_no_meta_update,
					 isc_arg_gds, isc_req_depth_exceeded,
					 isc_arg_number, (SLONG) MAX_RECURSION, 0);
			// Msg363 "request depth exceeded. (Recursive definition?)"
		}
		clone = CMP_clone_request(tdbb, request, n, false);
		if (!(clone->req_flags & (req_active | req_reserved))) {
			clone->req_flags |= req_reserved;
			THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_cmp_clone);
			return clone;
		}
	}
}


void CMP_fini(TDBB tdbb)
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


FMT CMP_format(TDBB tdbb, CSB csb, USHORT stream)
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

	IBERROR(222);				// msg 222 bad blr - invalid stream
	return NULL;
}


void CMP_get_desc(TDBB tdbb, CSB csb, JRD_NOD node, DSC * desc)
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
			fb_assert(false);
			// FALLINTO
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:
		case dtype_blob:
		case dtype_array:
			// break to error reporting code
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
			fb_assert(false);
			// FALLINTO
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:
		case dtype_blob:
		case dtype_array:
			// break to error reporting code
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
			const USHORT id = (USHORT) (ULONG) node->nod_arg[e_fld_id];
			const fmt* format =
				CMP_format(tdbb, csb, (USHORT) (ULONG) node->nod_arg[e_fld_stream]);
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
			jrd_nod* sub = node->nod_arg[e_scl_field];
			jrd_rel* relation =
				csb->csb_rpt[(USHORT)(ULONG) sub->
							 nod_arg[e_fld_stream]].csb_relation;
			const USHORT id = (USHORT)(ULONG) sub->nod_arg[e_fld_id];
			const jrd_fld* field = MET_get_field(relation, id);
			const arr* array;
			if (!field || !(array = field->fld_array))
				IBERROR(223);	// msg 223 argument of scalar operation must be an array
			*desc = array->arr_desc.ads_rpt[0].ads_desc;
			return;
		}

	case nod_divide:
		{
			DSC desc1, desc2;
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &desc1);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &desc2);
			// for compatibility with older versions of the product, we accept
			// text types for division in blr_version4 (dialect <= 1) only
			if (!(DTYPE_CAN_DIVIDE(desc1.dsc_dtype) ||
				  DTYPE_IS_TEXT(desc1.dsc_dtype))) {
				if (desc1.dsc_dtype != dtype_null)
					break;		// error, dtype not supported by arithmetic
			}
			if (!(DTYPE_CAN_DIVIDE(desc2.dsc_dtype) ||
				  DTYPE_IS_TEXT(desc2.dsc_dtype))) {
				if (desc2.dsc_dtype != dtype_null)
					break;		// error, dtype not supported by arithmetic
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
		// FALL INTO
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

		// In 6.0, the AVERAGE of an exact numeric type is int64 with the
		// same scale. Only AVERAGE on an approximate numeric type can
		// return a double.

	case nod_agg_average2:
	case nod_agg_average_distinct2:
		CMP_get_desc(tdbb, csb, node->nod_arg[0], desc);
		// In V6, the average of an exact type is computed in SINT64,
		// rather than double as in prior releases
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
			   it turns into either integers or longs (with scale). */

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
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
					DTYPE_IS_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
				else
					desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
				node->nod_scale = desc->dsc_scale;
				desc->dsc_sub_type = 0;
				desc->dsc_flags = 0;
				return;

			case dtype_sql_date:
			case dtype_sql_time:
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
					DTYPE_IS_TEXT(desc2.dsc_dtype))
						ERR_post(isc_expression_eval_err, 0);
				// FALL INTO
			case dtype_timestamp:
				node->nod_flags |= nod_date;

				fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) ||
					   DTYPE_IS_DATE(desc2.dsc_dtype));

				if (COULD_BE_DATE(desc1) && COULD_BE_DATE(desc2)) {
					if (node->nod_type == nod_subtract) {
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
						else if ((dtype1 == dtype_timestamp) &&
								 (dtype2 == dtype_sql_date))
								dtype = dtype_timestamp;
						else if ((dtype2 == dtype_timestamp) &&
								 (dtype1 == dtype_sql_date))
								dtype = dtype_timestamp;
						else
							ERR_post(isc_expression_eval_err, 0);

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
							fb_assert(dtype == dtype_timestamp);
							desc->dsc_dtype = DEFAULT_DOUBLE;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
					}
					else if (IS_DATE_AND_TIME(desc1, desc2)) {
						// <date> + <time>
						// <time> + <date>
						desc->dsc_dtype = dtype_timestamp;
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
						// <date> + <date>
						ERR_post(isc_expression_eval_err, 0);
				}
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
						 // <date> +/- <non-date>
						 (node->nod_type == nod_add))
						 // <non-date> + <date>
				{
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
					// <non-date> - <date>
					ERR_post(isc_expression_eval_err, 0);
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
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
					DTYPE_IS_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
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

			// Because dtype_int64 > dtype_double, we cannot just use the MAX macro to set
			// the result dtype. The rule is that two exact numeric operands yield an int64
			// result, while an approximate numeric and anything yield a double result.

			if (DTYPE_IS_EXACT(desc1.dsc_dtype)
				&& DTYPE_IS_EXACT(desc2.dsc_dtype))
				dtype = dtype_int64;

			else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype) &&
					 DTYPE_IS_NUMERIC(desc2.dsc_dtype)) dtype = dtype_double;

			else {
				// mixed numeric and non-numeric:

				fb_assert(COULD_BE_DATE(desc1) || COULD_BE_DATE(desc2));

				// the MAX(dtype) rule doesn't apply with dtype_int64

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

				fb_assert(DTYPE_IS_DATE(desc1.dsc_dtype) ||
					   DTYPE_IS_DATE(desc2.dsc_dtype));

				if ((DTYPE_IS_DATE(dtype1) || (dtype1 == dtype_null)) &&
					(DTYPE_IS_DATE(dtype2) || (dtype2 == dtype_null))) {
					if (node->nod_type == nod_subtract2) {
						// <any date> - <any date>

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
							ERR_post(isc_expression_eval_err, 0);

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
							fb_assert(dtype == dtype_timestamp
								   || dtype == dtype_null);
							desc->dsc_dtype = DEFAULT_DOUBLE;
							desc->dsc_length = type_lengths[desc->dsc_dtype];
							desc->dsc_scale = 0;
							desc->dsc_sub_type = 0;
							desc->dsc_flags = 0;
						}
					}
					else if (IS_DATE_AND_TIME(desc1, desc2)) {
						// <date> + <time>
						// <time> + <date>
						desc->dsc_dtype = dtype_timestamp;
						desc->dsc_length = type_lengths[desc->dsc_dtype];
						desc->dsc_scale = 0;
						desc->dsc_sub_type = 0;
						desc->dsc_flags = 0;
					}
					else
						// <date> + <date>
						ERR_post(isc_expression_eval_err, 0);
				}
				else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
						 // <date> +/- <non-date>
						 (node->nod_type == nod_add2))
						 // <non-date> + <date>
				{
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
					// <non-date> - <date>
					ERR_post(isc_expression_eval_err, 0);
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
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
					DTYPE_IS_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
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
				if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
					DTYPE_IS_TEXT(desc2.dsc_dtype)) desc->dsc_scale = 0;
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
				fb_assert(false);
				// FALLINTO
			case DTYPE_CANNOT:
				// break to error reporting code
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
			if (desc2.dsc_dtype <= dtype_varying)
				rc_len += DSC_string_length (&desc2);
			else
				rc_len += DSC_convert_to_text_length(desc2.dsc_dtype);
			// error() is a local routine in par.cpp, so we use plain ERR_post
			if (rc_len > MAX_FORMAT_SIZE)
				ERR_post(isc_imp_exc, isc_arg_gds, isc_blktoobig, 0);
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
			// QUADDATE - SECOND returns a float, or scaled!
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
			const fmt* format = (FMT) node->nod_arg[e_cast_fmt];
			*desc = format->fmt_desc[0];
			if ((desc->dsc_dtype <= dtype_any_text && !desc->dsc_length) ||
				(desc->dsc_dtype == dtype_varying
				 && desc->dsc_length <= sizeof(USHORT)))
			{
			    dsc desc1;
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
			const jrd_nod* message = node->nod_arg[e_arg_message];
			const fmt* format = (FMT) message->nod_arg[e_msg_format];
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
				const SLONG sl1 = MOV_get_long(&desc1, 0);
				const SLONG sl2 = MOV_get_long(&desc2, 0);
				// error() is a local routine in par.c, so we use plain ERR_post
				if (sl1 < 0 || sl2 < 0 || sl2 > MAX_COLUMN_SIZE - (SLONG) sizeof(USHORT))
					ERR_post(isc_imp_exc, isc_arg_gds, isc_blktoobig, 0);
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
			const fun* function = (FUN) node->nod_arg[e_fun_function];
			// Null value for the function indicates that the function was not
			// looked up during parsing the blr. This is true if the function 
			// referenced in the procedure blr was dropped before dropping the
			// procedure itself. Ignore the case because we are currently trying
			// to drop the procedure.
			// For normal requests, function would never be null. We would have
			// created a valid block while parsing in par_function/par.c.
			if (function)
				*desc = function->fun_rpt[function->fun_return_arg].fun_desc;
			else
				/* Note that CMP_get_desc is always called with a pre-allocated DSC, i.e:
					   DSC desc;
					   CMP_get_desc (.... &desc);
				   Hence the code:
					   *desc = NULL;
				   will not work. What I've done is memset the structure to zero. */
				MOVE_CLEAR(desc, sizeof(DSC));
			return;
		}

	case nod_variable:
		{
			const jrd_nod* value = node->nod_arg[e_var_variable];
			*desc = *(DSC*) (value->nod_arg + e_dcl_desc);
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
		fb_assert(false);
		break;
	}

	if (dtype == dtype_quad)
		IBERROR(224);				// msg 224 quad word arithmetic not supported

	ERR_post(isc_datype_notsup, 0);	// data type not supported for arithmetic
}


IDL CMP_get_index_lock(TDBB tdbb, JRD_REL relation, USHORT id)
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
	DBB dbb = tdbb->tdbb_database;

	DEV_BLKCHK(relation, type_rel);

	if (relation->rel_id < (USHORT) rel_MAX)
		return NULL;

	// for for an existing block

	idl* index;
	for (index = relation->rel_index_locks; index; index = index->idl_next)
		if (index->idl_id == id)
			return index;

	index = FB_NEW(*dbb->dbb_permanent) idl();
	index->idl_next = relation->rel_index_locks;
	relation->rel_index_locks = index;
	index->idl_relation = relation;
	index->idl_id = id;
	index->idl_count = 0;

	lck* lock = FB_NEW_RPT(*dbb->dbb_permanent, 0) lck;
	index->idl_lock = lock;
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_dbb = dbb;
	lock->lck_key.lck_long = relation->rel_id * 1000 + id;
	lock->lck_length = sizeof(lock->lck_key.lck_long);
	lock->lck_type = LCK_idx_exist;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);

	return index;
}


SLONG CMP_impure(CSB csb, USHORT size)
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
		return 0;

	const SLONG offset = FB_ALIGN(csb->csb_impure, ALIGNMENT);
	csb->csb_impure = offset + size;

	return offset;
}


JRD_REQ CMP_make_request(TDBB tdbb, CSB csb)
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
	vec::iterator ptr;

	DEV_BLKCHK(csb, type_csb);

	SET_TDBB(tdbb);

	JRD_REQ old_request = tdbb->tdbb_request;
	tdbb->tdbb_request = NULL;

	try {

	// Once any expansion required has been done, make a pass to assign offsets
	// into the impure area and throw away any unnecessary crude. Execution
	// optimizations can be performed here.

	DEBUG;
	jrd_nod* node = pass1(tdbb, csb, csb->csb_node, 0, 0, false);
	csb->csb_node = node;
	csb->csb_impure = REQ_SIZE + REQ_TAIL * csb->csb_n_stream;
	csb->csb_node = pass2(tdbb, csb, csb->csb_node, 0);

	if (csb->csb_impure > MAX_REQUEST_SIZE)
		IBERROR(226);			// msg 226 request size limit exceeded

	// Build the final request block. First, compute the "effective" repeat
	// count of hold the impure areas.

	int n = (csb->csb_impure - REQ_SIZE + REQ_TAIL - 1) / REQ_TAIL;
	request = FB_NEW_RPT(*tdbb->tdbb_default, n) jrd_req;
	request->req_count = csb->csb_n_stream;
	request->req_pool = tdbb->tdbb_default;
	request->req_impure_size = csb->csb_impure;
	request->req_top_node = csb->csb_node;
	request->req_access = csb->csb_access;
	request->req_variables = csb->csb_variables;
	request->req_resources = csb->csb_resources;
	request->req_records_affected = 0;
	if (csb->csb_g_flags & csb_blr_version4) {
		request->req_flags |= req_blr_version4;
	}

#ifdef SCROLLABLE_CURSORS
	request->req_async_message = csb->csb_async_message;
#endif

	// Take out existence locks on resources used in request. This is
	// a little complicated since relation locks MUST be taken before
	// index locks.

	for (RSC resource = request->req_resources; resource;
		 resource = resource->rsc_next)
	{
		switch (resource->rsc_type)
		{
		case rsc_relation:
			{
				JRD_REL relation = resource->rsc_rel;
				MET_post_existence(tdbb, relation);
				break;
			}
		case rsc_index:
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
		case rsc_procedure:
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
			BUGCHECK(219);		// msg 219 request of unknown resource
		}
	}

    csb_repeat* tail = csb->csb_rpt.begin();
	const csb_repeat* const end  = tail + csb->csb_n_stream;
	DEBUG;

	for (RPB* rpb = request->req_rpb; tail < end; rpb++, tail++)
	{
		// fetch input stream for update if all booleans matched against indices

		if (tail->csb_flags & csb_update && !(tail->csb_flags & csb_unmatched))
		{
			 rpb->rpb_stream_flags |= RPB_s_update;
		}
		rpb->rpb_relation = tail->csb_relation;
		SBM_release(tail->csb_fields);
	}

	USHORT count;

	// make a vector of all used RSEs

	for (temp = csb->csb_fors, count = 0; temp; count++) {
		temp = temp->lls_next;
	}

	if (count) {
		VEC vector = request->req_fors =
			vec::newVector(*request->req_pool, request->req_fors, count + 1);
		ptr = vector->begin();
		while (csb->csb_fors) {
			*ptr++ = (BLK) LLS_POP(&csb->csb_fors);
		}
	}

	// make a vector of all invariant-type nodes, so that we will
	// be able to easily reinitialize them when we restart the request

	for (temp = csb->csb_invariants, count = 0; temp; count++) {
		temp = temp->lls_next;
	}

	if (count) {
		VEC vector = request->req_invariants =
			vec::newVector(*request->req_pool, request->req_invariants, count + 1);
		ptr = vector->begin();
		while (csb->csb_invariants) {
			*ptr++ = (BLK) LLS_POP(&csb->csb_invariants);
		}
	}

	DEBUG;
	tdbb->tdbb_request = old_request;

	} // try
	catch (const std::exception&) {
		tdbb->tdbb_request = old_request;
		ERR_punt();
	}

	return request;
}


void CMP_post_access(TDBB tdbb,
					 CSB csb,
					 TEXT* security_name,
					 SLONG view_id,
					 const TEXT* trig,
					 const TEXT* proc,
					 USHORT mask,
					 const TEXT* type_name,
					 const TEXT* name)
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

	ACC access, last_entry = NULL;

	for (access = csb->csb_access; access; access = access->acc_next)
	{
		if (!strcmp_null(access->acc_security_name, security_name) &&
			access->acc_view_id == view_id &&
			!strcmp_null(access->acc_trg_name, trig) &&
			!strcmp_null(access->acc_prc_name, proc) &&
			access->acc_mask == mask &&
			!strcmp(access->acc_type, type_name) &&
			!strcmp(access->acc_name, name))
		{
			return;
		}
		if (!access->acc_next)
		{
			last_entry = access;
		}
	}

	access = FB_NEW(*tdbb->tdbb_default) acc;

	// append the security class to the existing list

	if (last_entry)
	{
		access->acc_next = NULL;
		last_entry->acc_next = access;
	}
	else
	{
		access->acc_next = csb->csb_access;
		csb->csb_access = access;
	}

	access->acc_security_name = clone_cstring(tdbb->tdbb_default, security_name);
	access->acc_view_id = view_id;
	access->acc_trg_name = clone_cstring(tdbb->tdbb_default, trig);
	access->acc_prc_name = clone_cstring(tdbb->tdbb_default, proc);
	access->acc_mask = mask;
	access->acc_type = type_name;	// no need to clone, should be static
	access->acc_name = clone_cstring(tdbb->tdbb_default, name);

#ifdef DEBUG_TRACE
	ib_printf("%x: require %05X access to %s %s (sec %s view %ld trg %s prc %s)\n",
		 csb, access->acc_mask, access->acc_type, access->acc_name,
		 access->acc_security_name ? access->acc_security_name : "NULL",
		 access->acc_view_id,
		 access->acc_trg_name ? access->acc_trg_name : "NULL",
		 access->acc_prc_name ? access->acc_prc_name : "NULL");
#endif
}


void CMP_post_resource(TDBB tdbb,
						RSC * rsc_ptr,
						BLK rel_or_prc,
						enum rsc_s type,
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
	RSC resource;

	DEV_BLKCHK(*rsc_ptr, type_rsc);

	SET_TDBB(tdbb);

	for (resource = *rsc_ptr; resource; resource = resource->rsc_next)
		if (resource->rsc_type == type && resource->rsc_id == id)
			return;

	resource = FB_NEW(*tdbb->tdbb_default) Rsc;
	resource->rsc_next = *rsc_ptr;
	*rsc_ptr = resource;
	resource->rsc_type = type;
	resource->rsc_id = id;
	switch (type) {
	case rsc_relation:
	case rsc_index:
		resource->rsc_rel = (JRD_REL) rel_or_prc;
		break;
	case rsc_procedure:
		resource->rsc_prc = (JRD_PRC) rel_or_prc;
		break;
	default:
		BUGCHECK(220);			/* msg 220 unknown resource */
		break;
	}
}


void CMP_release_resource(RSC * rsc_ptr, enum rsc_s type, USHORT id)
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
	RSC resource;

	DEV_BLKCHK(*rsc_ptr, type_rsc);

	for (; (resource = *rsc_ptr); rsc_ptr = &resource->rsc_next)
		if (resource->rsc_type == type && resource->rsc_id == id)
			break;

	if (!resource)
		return;

	// take out of the linked list and release

	*rsc_ptr = resource->rsc_next;
	delete resource;
}


void CMP_decrement_prc_use_count(TDBB tdbb, JRD_PRC procedure)
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

	// Actually, it's possible for procedures to have intermixed dependencies, so
	// this routine can be called for the procedure which is being freed itself.
	// Hence we should just silently ignore such a situation.

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

	// Call recursively if and only if the use count is zero AND the procedure
	// in dbb_procedures is different than this procedure. 
	// The procedure will be different than in dbb_procedures only if it is a
	// floating copy, i.e. an old copy or a deleted procedure.
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


void CMP_release(TDBB tdbb, JRD_REQ request)
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
	RSC resource;

	SET_TDBB(tdbb);

	DEV_BLKCHK(request, type_req);

	// release existence locks on references

	att* attachment = request->req_attachment;
	if (!attachment || !(attachment->att_flags & ATT_shutdown))
		for (resource =
			 request->req_resources;
			 resource; resource = resource->rsc_next)
		{
			switch (resource->rsc_type) {
			case rsc_relation:
				{
					relation = resource->rsc_rel;
					MET_release_existence(relation);
					break;
				}
			case rsc_index:
				{
					relation = resource->rsc_rel;
					if ( (index = CMP_get_index_lock(tdbb, relation,
													 resource->rsc_id)) )
					{
						if (!--index->idl_count)
							LCK_release(tdbb, index->idl_lock);
					}
					break;
				}
			case rsc_procedure:
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
		*next; next = &(*next)->req_request)
		{
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


void CMP_shutdown_database(TDBB tdbb)
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
	DBB dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	DEV_BLKCHK(dbb, type_dbb);

	vec* vector = dbb->dbb_relations;
	if (!vector)
		return;

	// go through relations and indices and release
	// all existence locks that might have been taken

	vec::iterator ptr, end;
	for (ptr = vector->begin(), end = vector->end(); ptr < end; ptr++) {
		jrd_rel* relation = (JRD_REL) *ptr;
		if (relation) {
			if (relation->rel_existence_lock) {
				LCK_release(tdbb, relation->rel_existence_lock);
				relation->rel_use_count = 0;
			}
			for (idl* index = relation->rel_index_locks; index;
				 index = index->idl_next)
			{
				if (index->idl_lock) {
					LCK_release(tdbb, index->idl_lock);
					index->idl_count = 0;
				}
			}
		}
	}

	if (!(vector = dbb->dbb_procedures))
		return;

	// release all procedure existence locks that might have been taken

	vec::iterator pptr, pend;
	for (pptr = vector->begin(), pend = vector->end(); pptr < pend; pptr++) {
		jrd_prc* procedure = (JRD_PRC) *pptr;
		if (procedure) {
			if (procedure->prc_existence_lock) {
				LCK_release(tdbb, procedure->prc_existence_lock);
				procedure->prc_use_count = 0;
			}
		}
	}
}


static UCHAR* alloc_map(TDBB tdbb, CSB csb, USHORT stream)
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
	DEV_BLKCHK(csb, type_csb);

	SET_TDBB(tdbb);

	str* string = FB_NEW_RPT(*tdbb->tdbb_default, MAP_LENGTH) str;
	string->str_length = MAP_LENGTH;
	csb->csb_rpt[stream].csb_map = (UCHAR *) string->str_data;
	fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR maybe?
	string->str_data[0] = (UCHAR) stream;

	return (UCHAR*) string->str_data;
}


#ifdef PC_ENGINE
static USHORT base_stream(CSB csb, JRD_NOD * stream_number, bool nav_stream)
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
	DEV_BLKCHK(csb, type_csb);

	// note: *stream_number is NOT a JRD_NOD
	USHORT stream = (USHORT) *stream_number;

	// if the stream references a view, follow map

	UCHAR* map = csb->csb_rpt[stream].csb_map;
	if (map) {
		if (map[2]) {
			if (nav_stream)
				// navigational stream %ld references a view with more than one base table
				ERR_post(isc_complex_view, isc_arg_number, (SLONG) stream, 0);
		}
		else {
			map++;
			stream = *map;
		}
	}

	// if this is a navigational stream, fix up the stream number 
	// in the node tree to point to the base table from now on

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
	SET_TDBB(tdbb);

	DEV_BLKCHK(stack, type_lls);

	jrd_nod* node1 = (JRD_NOD) LLS_POP(&stack);

	if (!stack)
		return node1;

	jrd_nod* cat_node = PAR_make_node(tdbb, 2);
	cat_node->nod_type = nod_concatenate;
	cat_node->nod_arg[0] = node1;
	cat_node->nod_arg[1] = catenate_nodes(tdbb, stack);

	return cat_node;
}


static JRD_NOD copy(TDBB tdbb,
					CSB csb,
					JRD_NOD input,
					UCHAR * remap,
					USHORT field_id,
					bool remap_fld)
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
	USHORT stream, new_stream;

	SET_TDBB(tdbb);


	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(input, type_nod);

	if (!input)
		return NULL;

	// special case interesting nodes

	USHORT args = input->nod_count;

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
		node->nod_arg[e_arg_message] = input->nod_arg[e_arg_message];
		node->nod_arg[e_arg_flag] =
			copy(tdbb, csb, input->nod_arg[e_arg_flag], remap, field_id,
				 remap_fld);
		node->nod_arg[e_arg_indicator] =
			copy(tdbb, csb, input->nod_arg[e_arg_indicator], remap, field_id,
				 remap_fld);
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
				field_id = (USHORT)(ULONG) input->nod_arg[e_fld_id];
			stream = (USHORT)(ULONG) input->nod_arg[e_fld_stream];
			if (remap_fld) {
				JRD_REL relation;
				JRD_FLD field;

				relation = csb->csb_rpt[stream].csb_relation;
				field = MET_get_field(relation, field_id);
				if (field->fld_source)
					field_id = (USHORT)(ULONG) field->fld_source->nod_arg[e_fld_id];
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
				 remap_fld);
		node->nod_arg[e_fun_function] = input->nod_arg[e_fun_function];
		return (node);


	case nod_gen_id:
	case nod_gen_id2:			// 20001013 PJPG
		node = PAR_make_node(tdbb, e_gen_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_gen_value] =
			copy(tdbb, csb, input->nod_arg[e_gen_value], remap, field_id,
				 remap_fld);
		node->nod_arg[e_gen_relation] = input->nod_arg[e_gen_relation];
		return (node);

	case nod_cast:
		node = PAR_make_node(tdbb, e_cast_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_cast_source] =
			copy(tdbb, csb, input->nod_arg[e_cast_source], remap, field_id,
				 remap_fld);
		node->nod_arg[e_cast_fmt] = input->nod_arg[e_cast_fmt];
		return (node);

	case nod_extract:
		node = PAR_make_node(tdbb, e_extract_length);
		node->nod_count = input->nod_count;
		node->nod_type = input->nod_type;
		node->nod_arg[e_extract_value] =
			copy(tdbb, csb, input->nod_arg[e_extract_value], remap, field_id,
				 remap_fld);
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
			RSE old_rse = (RSE) input;
			RSE new_rse =
				(RSE) PAR_make_node(tdbb, old_rse->rse_count + rse_delta + 2);
			new_rse->nod_type = input->nod_type;
			new_rse->nod_count = 0;
			new_rse->rse_count = old_rse->rse_count;
			arg1 = old_rse->rse_relation;
			arg2 = new_rse->rse_relation;
			for (end = arg1 + old_rse->rse_count; arg1 < end; arg1++, arg2++)
				*arg2 = copy(tdbb, csb, *arg1, remap, field_id, remap_fld);
			new_rse->rse_jointype = old_rse->rse_jointype;
			new_rse->rse_writelock = old_rse->rse_writelock;
			new_rse->rse_first =
				copy(tdbb, csb, old_rse->rse_first, remap, field_id,
					 remap_fld);
			new_rse->rse_skip =
				copy (tdbb, csb, old_rse->rse_skip, remap, field_id,
					remap_fld);
			new_rse->rse_boolean =
				copy(tdbb, csb, old_rse->rse_boolean, remap, field_id,
					 remap_fld);
			new_rse->rse_sorted =
				copy(tdbb, csb, old_rse->rse_sorted, remap, field_id,
					 remap_fld);
			new_rse->rse_projection =
				copy(tdbb, csb, old_rse->rse_projection, remap, field_id,
					 remap_fld);
			return (JRD_NOD) new_rse;
		}

	case nod_relation:
		{
			if (!remap)
				BUGCHECK(221);	// msg 221 (CMP) copy: cannot remap
			node = PAR_make_node(tdbb, e_rel_length);
			node->nod_type = input->nod_type;
			node->nod_count = 0;

			stream = (USHORT)(ULONG) input->nod_arg[e_rel_stream];

			// Last entry in the remap contains the the original stream number.
			// Get that stream number so that the flags can be copied 
			// into the newly created child stream.

			const int relative_stream = (stream) ? remap[stream - 1] : stream;
			new_stream = csb->csb_n_stream++;
			fb_assert(new_stream <= MAX_STREAMS);
			node->nod_arg[e_rel_stream] = (JRD_NOD) (SLONG) new_stream;
			remap[stream] = (UCHAR) new_stream;

			node->nod_arg[e_rel_context] = input->nod_arg[e_rel_context];
			node->nod_arg[e_rel_relation] = input->nod_arg[e_rel_relation];
			node->nod_arg[e_rel_view] = input->nod_arg[e_rel_view];

			csb_repeat* element = CMP_csb_element(csb, new_stream);
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
			csb->csb_rpt[new_stream].csb_flags |=
				csb->csb_rpt[relative_stream].csb_flags & csb_no_dbkey;

			return node;
		}

	case nod_procedure:
		{
			if (!remap)
				BUGCHECK(221);	// msg 221 (CMP) copy: cannot remap
			node = PAR_make_node(tdbb, e_prc_length);
			node->nod_type = input->nod_type;
			node->nod_count = input->nod_count;
			node->nod_arg[e_prc_inputs] =
				copy(tdbb, csb, input->nod_arg[e_prc_inputs], remap, field_id,
					 remap_fld);
			node->nod_arg[e_prc_in_msg] = input->nod_arg[e_prc_in_msg];
			stream = (USHORT)(ULONG) input->nod_arg[e_prc_stream];
			new_stream = csb->csb_n_stream++;
			fb_assert(new_stream <= MAX_STREAMS);
			node->nod_arg[e_prc_stream] = (JRD_NOD) (SLONG) new_stream;
			remap[stream] = (UCHAR) new_stream;
			node->nod_arg[e_prc_procedure] = input->nod_arg[e_prc_procedure];
			csb_repeat* element = CMP_csb_element(csb, new_stream);
			// SKIDDER: Maybe we need to check if we really found a procedure?
			element->csb_procedure = MET_lookup_procedure_id(tdbb,
			  (SSHORT)(SLONG) node->nod_arg[e_prc_procedure], FALSE, FALSE, 0);

			csb->csb_rpt[new_stream].csb_flags |=
				csb->csb_rpt[stream].csb_flags & csb_no_dbkey;
			return node;
		}

	case nod_aggregate:
		if (!remap)
			BUGCHECK(221);		// msg 221 (CMP) copy: cannot remap
		node = PAR_make_node(tdbb, e_agg_length);
		node->nod_type = input->nod_type;
		node->nod_count = 0;
		stream = (USHORT)(ULONG) input->nod_arg[e_agg_stream];
		fb_assert(stream <= MAX_STREAMS);
		new_stream = csb->csb_n_stream++;
		fb_assert(new_stream <= MAX_STREAMS);
		node->nod_arg[e_agg_stream] = (JRD_NOD) (SLONG) new_stream;
		// fb_assert(new_stream <= MAX_UCHAR);
		remap[stream] = (UCHAR) new_stream;
		CMP_csb_element(csb, new_stream);

		csb->csb_rpt[new_stream].csb_flags |=
			csb->csb_rpt[stream].csb_flags & csb_no_dbkey;
		node->nod_arg[e_agg_rse] =
			copy(tdbb, csb, input->nod_arg[e_agg_rse], remap, field_id,
				 remap_fld);
		node->nod_arg[e_agg_group] =
			copy(tdbb, csb, input->nod_arg[e_agg_group], remap, field_id,
				 remap_fld);
		node->nod_arg[e_agg_map] =
			copy(tdbb, csb, input->nod_arg[e_agg_map], remap, field_id,
				 remap_fld);
		return node;

	case nod_union:
		if (!remap)
			BUGCHECK(221);		// msg 221 (CMP) copy: cannot remap
		node = PAR_make_node(tdbb, e_uni_length);
		node->nod_type = input->nod_type;
		node->nod_count = 2;
		stream = (USHORT)(ULONG) input->nod_arg[e_uni_stream];
		fb_assert(stream <= MAX_STREAMS);
		new_stream = csb->csb_n_stream++;
		fb_assert(new_stream <= MAX_STREAMS);
		node->nod_arg[e_uni_stream] = (JRD_NOD) (SLONG) new_stream;
		remap[stream] = (UCHAR) new_stream;
		CMP_csb_element(csb, new_stream);

		csb->csb_rpt[new_stream].csb_flags |=
			csb->csb_rpt[stream].csb_flags & csb_no_dbkey;
		node->nod_arg[e_uni_clauses] =
			copy(tdbb, csb, input->nod_arg[e_uni_clauses], remap, field_id,
				 remap_fld);
		return node;

	case nod_sort:
		args += args;
		break;

	default:
		break;
	}

	// fall thru on generic nodes

	node = PAR_make_node(tdbb, args);
	node->nod_count = input->nod_count;
	node->nod_type = input->nod_type;
	node->nod_flags = input->nod_flags;

	arg1 = input->nod_arg;
	arg2 = node->nod_arg;

	for (end = arg1 + input->nod_count; arg1 < end; arg1++, arg2++)
		if (*arg1)
			*arg2 = copy(tdbb, csb, *arg1, remap, field_id, remap_fld);

	// finish off sort

	if (input->nod_type == nod_sort)
		for (end = arg1 + input->nod_count; arg1 < end; arg1++, arg2++)
			*arg2 = *arg1;

	return node;
}


static void expand_view_nodes(TDBB tdbb,
							  CSB csb,
							  USHORT stream,
							  LLS * stack,
							  NOD_T type)
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

	// if the stream's dbkey should be ignored, do so

	if (csb->csb_rpt[stream].csb_flags & csb_no_dbkey)
		return;

	// if the stream references a view, follow map

	if ( (map = csb->csb_rpt[stream].csb_map) ) {
		++map;
		while (*map)
			expand_view_nodes(tdbb, csb, *map++, stack, type);
		return;
	}

	// relation is primitive - make dbkey node

	if (csb->csb_rpt[stream].csb_relation) {
		jrd_nod* node = PAR_make_node(tdbb, 1);
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

			stream = (USHORT)(ULONG) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags |= csb_no_dbkey;
			tail = &csb->csb_rpt[stream];
			if ( (relation = tail->csb_relation) )
				CMP_post_access(tdbb, csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								0, 0, SCL_read, object_table,
								relation->rel_name);
		}
		else if (node->nod_type == nod_rse)
			ignore_dbkey(tdbb, csb, (RSE) node, view);
		else if (node->nod_type == nod_aggregate)
			ignore_dbkey(tdbb, csb, (RSE) node->nod_arg[e_agg_rse], view);
		else if (node->nod_type == nod_union) {
			JRD_NOD *ptr_uni, *end_uni;

			jrd_nod* clauses = node->nod_arg[e_uni_clauses];
			for (ptr_uni = clauses->nod_arg, end_uni =
				 ptr_uni + clauses->nod_count; ptr_uni < end_uni; ptr_uni++)
				ignore_dbkey(tdbb, csb, (RSE) * ptr_uni++, view);
		}
	}
}


static JRD_NOD make_defaults(TDBB tdbb, CSB csb, USHORT stream, JRD_NOD statement)
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
	USHORT field_id;
	UCHAR *map, local_map[MAP_LENGTH];

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(statement, type_nod);

	jrd_rel* relation = csb->csb_rpt[stream].csb_relation;

	if (!(vector = relation->rel_fields))
		return statement;

	if (!(map = csb->csb_rpt[stream].csb_map)) {
		map = local_map;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR relevant, too?
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
				copy(tdbb, csb, value, map, (USHORT) (field_id + 1), false);
			node->nod_arg[e_asgn_to] = PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}

	if (!stack)
		return statement;

	// we have some default - add the original statement and make a list out of
	// the whole mess

	LLS_PUSH(statement, &stack);

	return PAR_make_list(tdbb, stack);
}


static JRD_NOD make_validation(TDBB tdbb, CSB csb, USHORT stream)
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


	DEV_BLKCHK(csb, type_csb);

	relation = csb->csb_rpt[stream].csb_relation;

	if (!(vector = relation->rel_fields))
		return NULL;

	if (!(map = csb->csb_rpt[stream].csb_map)) {
		map = local_map;
		fb_assert(stream <= MAX_STREAMS); // CVC: MAX_UCHAR still relevant for the bitmap?
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
					 false);
			node->nod_arg[e_val_value] =
				PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}

		if (*ptr1 && (validation = ((JRD_FLD)(*ptr1))->fld_not_null)) {
			node = PAR_make_node(tdbb, e_val_length);
			node->nod_type = nod_validate;
			node->nod_arg[e_val_boolean] =
				copy(tdbb, csb, validation, map, (USHORT) (field_id + 1),
					 false);
			node->nod_arg[e_val_value] =
				PAR_gen_field(tdbb, stream, field_id);
			LLS_PUSH(node, &stack);
		}
	}

	if (!stack)
		return NULL;

	return PAR_make_list(tdbb, stack);
}


static JRD_NOD pass1(TDBB tdbb,
					 CSB csb,
					 JRD_NOD node,
					 JRD_REL view,
					 USHORT view_stream,
					 bool validate_expr)
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
 * The argument validate_expr is true if an ancestor of the
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

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);
	DEV_BLKCHK(view, type_rel);

	if (!node)
		return node;

	validate_expr = validate_expr || (node->nod_type == nod_validate);

	// if there is processing to be done before sub expressions, do it here

	switch (node->nod_type) {
	case nod_field:
		{
			LLS stack;
			JRD_REL relation;
			JRD_FLD field;
			UCHAR *map, local_map[MAP_LENGTH];

			stream = (USHORT)(ULONG) node->nod_arg[e_fld_stream];

			// Look at all rse's which are lower in scope than the rse which this field 
			// is referencing, and mark them as varying - the rule is that if a field 
			// from one rse is referenced within the scope of another rse, the first rse 
			// can't be invariant. This won't optimize all cases, but it is the simplest 
			// operating assumption for now.

			for (stack = csb->csb_current_rses; stack;
				 stack = stack->lls_next) {

				RSE rse = (RSE) stack->lls_object;
				if (stream_in_rse(stream, rse))
					break;
				rse->nod_flags |= rse_variant;
			}
			tail = &csb->csb_rpt[stream];
			if (!(relation = tail->csb_relation) ||
				!(field =
				  MET_get_field(relation,
								(USHORT)(ULONG) node->nod_arg[e_fld_id]))) break;

			// if this is a modify or store, check REFERENCES access to any foreign keys

/* CVC: This is against the SQL standard. REFERENCES should be enforced only at the
				time the FK is defined in DDL, not when a DML is going to be executed.
			if (((tail->csb_flags & csb_modify)
				 || (tail->csb_flags & csb_store)) && !(relation->rel_view_rse
														||
														relation->rel_file))
					IDX_check_access(tdbb, csb, tail->csb_view, relation,
									 field);
*/

			// posting the required privilege access to the current relation and field

			// If this is in a "validate_subtree" then we must not
			// post access checks to the table and the fields in the table. 
			// If any node of the parse tree is a nod_validate type node,
			// the nodes in the subtree are involved in a validation
			// clause only, the subtree is a validate_subtree in our notation.

			if (tail->csb_flags & csb_modify) {
				if (!validate_expr) {
					CMP_post_access(tdbb, csb, relation->rel_security_name,
									(tail->csb_view) ? tail->csb_view->rel_id : 
										(view ? view->rel_id : 0),
									0, 0, SCL_sql_update, object_table,
									relation->rel_name);
					CMP_post_access(tdbb, csb, field->fld_security_name,
									(tail->csb_view) ? tail->csb_view->rel_id : 
										(view ? view->rel_id : 0),
									0, 0, SCL_sql_update, object_column,
									field->fld_name);
				}
			}
			else if (tail->csb_flags & csb_erase) {
				CMP_post_access(tdbb, csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id :
									(view ? view->rel_id : 0),
								0, 0, SCL_sql_delete, object_table,
								relation->rel_name);
			}
			else if (tail->csb_flags & csb_store) {
				CMP_post_access(tdbb, csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								0, 0, SCL_sql_insert, object_table,
								relation->rel_name);
				CMP_post_access(tdbb, csb, field->fld_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0), 0,
								0, SCL_sql_insert, object_column, field->fld_name);
			}
			else {
				CMP_post_access(tdbb, csb, relation->rel_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								0, 0, SCL_read, object_table, relation->rel_name);
				CMP_post_access(tdbb, csb, field->fld_security_name,
								(tail->csb_view) ? tail->csb_view->rel_id : 
									(view ? view->rel_id : 0),
								0, 0, SCL_read, object_column, field->fld_name);
			}

			if (!(sub = field->fld_computation) && !(sub = field->fld_source)) {

				if (!relation->rel_view_rse)
					break;

				ERR_post(isc_no_field_access,
						 isc_arg_string, ERR_cstring(field->fld_name),
						 isc_arg_string, ERR_cstring(relation->rel_name), 0);
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

			if (tail->csb_flags & (csb_view_update | csb_trigger)) {
			// dimitr:	this code is rollbacked to its previous state
			//			to allow computed fields in views to be updatable
			//			via triggers
			//												2003.11.05
			//	if (!(field->fld_computation))
					break;
			}

			if (!(map = tail->csb_map)) {
				map = local_map;
				fb_assert(stream + 2 <= MAX_STREAMS);
				local_map[0] = (UCHAR) stream;
				map[1] = stream + 1;
				map[2] = stream + 2;
			}
			sub = copy(tdbb, csb, sub, map, 0, false);
			return pass1(tdbb, csb, sub, view, view_stream, validate_expr);
		}

	case nod_assignment:
		{
			JRD_FLD field;

			sub = node->nod_arg[e_asgn_from];
			if (sub->nod_type == nod_field) {
				stream = (USHORT)(ULONG) sub->nod_arg[e_fld_stream];
				field = MET_get_field(csb->csb_rpt[stream].csb_relation,
									  (USHORT)(ULONG) sub->nod_arg[e_fld_id]);
				if (field)
					node->nod_arg[e_asgn_missing2] = field->fld_missing_value;
			}

			sub = node->nod_arg[e_asgn_to];
			if (sub->nod_type != nod_field)
				break;
			stream = (USHORT)(ULONG) sub->nod_arg[e_fld_stream];
			tail = &csb->csb_rpt[stream];
			if (!
				(field =
				 MET_get_field(tail->csb_relation,
							   (USHORT)(ULONG) sub->nod_arg[e_fld_id]))) break;
			if (field->fld_missing_value) {
				node->nod_arg[e_asgn_missing] = field->fld_missing_value;
				node->nod_count = 3;
			}
		}
		break;

	case nod_modify:
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_new_stream];
		tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_modify;
		pass1_modify(tdbb, csb, node);
		// fb_assert(node->nod_arg [e_mod_new_stream] <= MAX_USHORT);
		if ( (node->nod_arg[e_mod_validate] = make_validation(tdbb, csb,
							(USHORT)(ULONG) node->
							nod_arg[e_mod_new_stream])) ) node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_validate + 1);
		break;

	case nod_erase:
		stream = (USHORT)(ULONG) node->nod_arg[e_erase_stream];
		tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_erase;
		pass1_erase(tdbb, csb, node);
		break;

	case nod_exec_proc:
		procedure = (JRD_PRC) node->nod_arg[e_esp_procedure];
		post_procedure_access(tdbb, csb, procedure);
		CMP_post_resource(tdbb, &csb->csb_resources, (BLK) procedure,
						  rsc_procedure, procedure->prc_id);
		break;

	case nod_store:
		sub = node->nod_arg[e_sto_relation];
		stream = (USHORT)(ULONG) sub->nod_arg[e_rel_stream];
		tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_store;
		sub = pass1_store(tdbb, csb, node);
		if (sub) {
			stream = (USHORT)(ULONG) sub->nod_arg[e_rel_stream];
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
		ignore_dbkey(tdbb, csb, (RSE) node->nod_arg[e_stat_rse], view);
		break;

	case nod_aggregate:
		fb_assert((int)(SLONG) node->nod_arg[e_agg_stream] <= MAX_STREAMS);
		csb->csb_rpt[(USHORT)(ULONG) node->nod_arg[e_agg_stream]].csb_flags |=
			csb_no_dbkey;
		ignore_dbkey(tdbb, csb, (RSE) node->nod_arg[e_agg_rse], view);
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
			stream = (USHORT)(ULONG) node->nod_arg[0];

			if (!csb->csb_rpt[stream].csb_map)
				return node;
			stack = NULL;
			expand_view_nodes(tdbb, csb, stream, &stack, type);
			if (stack)
				return catenate_nodes(tdbb, stack);

			// The user is asking for the dbkey/record version of an aggregate.
			// Humor him with a key filled with zeros.

			node = PAR_make_node(tdbb, 1);
			node->nod_count = 0;
			node->nod_type = type;
			node->nod_flags |= nod_agg_dbkey;
			node->nod_arg[0] = (JRD_NOD) (SLONG) stream;
			return node;
		}

	case nod_function:
		pass1(tdbb, csb, node->nod_arg[e_fun_args], view, view_stream,
			  validate_expr);
		break;

	case nod_abort:
		pass1(tdbb, csb, node->nod_arg[e_xcp_msg], view, view_stream,
			  validate_expr);
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		ignore_dbkey(tdbb, csb, (RSE) node->nod_arg[e_any_rse], view);
		break;

	case nod_cardinality:
		stream = (USHORT)(ULONG) node->nod_arg[e_card_stream];
		csb->csb_rpt[stream].csb_flags |= csb_compute;
		break;

	default:
		break;

	}

	// handle sub-expressions here

	ptr = node->nod_arg;

	for (end = ptr + node->nod_count; ptr < end; ptr++) {
		*ptr = pass1(tdbb, csb, *ptr, view, view_stream, validate_expr);
	}

	// perform any post-processing here

	if (node->nod_type == nod_assignment) {
		sub = node->nod_arg[e_asgn_to];
		if (sub->nod_type != nod_field &&
			sub->nod_type != nod_argument && sub->nod_type != nod_variable)
			ERR_post(isc_read_only_field, 0);
	}

	return node;
}


static void pass1_erase(TDBB tdbb, CSB csb, JRD_NOD node)
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

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// if updateable views with triggers are involved, there
	// maybe a recursive call to be ignored

	if (node->nod_arg[e_erase_sub_erase])
		return;

	parent = view = NULL;

	// to support views of views, loop until we hit a real relation

	for (;;) {
		stream = new_stream = (USHORT)(ULONG) node->nod_arg[e_erase_stream];
		tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_erase;
		relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
	    post_trigger_access(tdbb, csb, relation, relation->rel_pre_erase, view);
		post_trigger_access(tdbb, csb, relation, relation->rel_post_erase, view);

		// if this is a view trigger operation, get an extra stream to play with

		trigger =
			(relation->rel_pre_erase) ? relation->
			rel_pre_erase : relation->rel_post_erase;

		if (relation->rel_view_rse && trigger) {
			new_stream = csb->csb_n_stream++;
			node->nod_arg[e_erase_stream] = (JRD_NOD) (SLONG) new_stream;
			CMP_csb_element(csb, new_stream)->csb_relation = relation;
		}

		// Check out delete. If this is a delete thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		priv = SCL_sql_delete;
		if (parent)
			priv |= SCL_read;
		source =
			pass1_update(tdbb, csb, relation, trigger, stream, new_stream,
						 priv, parent, parent_stream);

		if (csb->csb_rpt[new_stream].csb_flags & csb_view_update) {
			// we have a view either updateable or non-updateable
			node->nod_arg[e_erase_statement] =
				pass1_expand_view(tdbb, csb, stream, new_stream, false);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_erase_statement + 1);
		}

		if (!source)
			return;

		// We have a updateable view. If there is a trigger on it, create a
		// dummy erase record.

		map = csb->csb_rpt[stream].csb_map;
		if (trigger) {
			view_node = copy(tdbb, csb, node, map, 0, false);
			node->nod_arg[e_erase_sub_erase] = view_node;
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_erase_sub_erase + 1);
			node = view_node;
			node->nod_arg[e_erase_statement] = 0;
			node->nod_arg[e_erase_sub_erase] = 0;
		}
		else
			csb->csb_rpt[new_stream].csb_flags &= ~csb_view_update;

		// So far, so good. Lookup view context in instance map to get target
		// stream.

		parent = relation;
		parent_stream = stream;
		new_stream = (USHORT)(ULONG) source->nod_arg[e_rel_stream];
		node->nod_arg[e_erase_stream] = (JRD_NOD) (SLONG) map[new_stream];
	}
}


static JRD_NOD pass1_expand_view(TDBB tdbb,
								 CSB csb,
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
		// dimitr: let's make computed fields updatable in views
		// if (*ptr && !((JRD_FLD)(*ptr))->fld_computation) {
		if (*ptr) {
			if (remap) {
				field = MET_get_field(relation, id);
				if (field->fld_source)
					new_id =
						(USHORT)(ULONG) (field->fld_source)->nod_arg[e_fld_id];
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


static void pass1_modify(TDBB tdbb, CSB csb, JRD_NOD node)
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


	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// if updateable views with triggers are involved, there
	// maybe a recursive call to be ignored

	if (node->nod_arg[e_mod_sub_mod])
		return;

	parent = view = NULL;

	// to support views of views, loop until we hit a real relation

	for (;;) {
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_org_stream];
		new_stream = (USHORT)(ULONG) node->nod_arg[e_mod_new_stream];
		tail = &csb->csb_rpt[new_stream];
		tail->csb_flags |= csb_modify;
		relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
	    post_trigger_access(tdbb, csb, relation, relation->rel_pre_modify, view);
		post_trigger_access(tdbb, csb, relation, relation->rel_post_modify, view);
		trigger =
			(relation->rel_pre_modify) ? relation->
			rel_pre_modify : relation->rel_post_modify;

		// Check out update. If this is an update thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		priv = SCL_sql_update;
		if (parent)
			priv |= SCL_read;
		if (!(source = pass1_update(tdbb, csb, relation, trigger, stream,
									new_stream, priv, parent, parent_stream))) {
			if (csb->csb_rpt[new_stream].csb_flags & csb_view_update) {
				node->nod_arg[e_mod_map_view] =
					pass1_expand_view(tdbb, csb, stream, new_stream, false);
				node->nod_count =
					MAX(node->nod_count, (USHORT) e_mod_map_view + 1);
			}
			return;
		}

		parent = relation;
		parent_stream = stream;
		if (trigger) {
			node->nod_arg[e_mod_map_view] =
				pass1_expand_view(tdbb, csb, stream, new_stream, false);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_map_view + 1);
			map = csb->csb_rpt[stream].csb_map;
			stream = (USHORT)(ULONG) source->nod_arg[e_rel_stream];
			stream = map[stream];
			view_stream = new_stream;

			// next, do update stream

			map =
				alloc_map(tdbb, csb,
						  (SSHORT)(SLONG) node->nod_arg[e_mod_new_stream]);
			source = copy(tdbb, csb, source, map, 0, false);
			fb_assert((int)(IPTR) source->nod_arg[e_rel_stream] <= MAX_STREAMS);
			map[new_stream] = (UCHAR)(ULONG) source->nod_arg[e_rel_stream];
			view_node = copy(tdbb, csb, node, map, 0, true);
			view_node->nod_arg[e_mod_org_stream] = (JRD_NOD) (SLONG) stream;
			view_node->nod_arg[e_mod_new_stream] =
				source->nod_arg[e_rel_stream];
			view_node->nod_arg[e_mod_map_view] = NULL;
			node->nod_arg[e_mod_sub_mod] = view_node;
			new_stream = (USHORT)(ULONG) source->nod_arg[e_rel_stream];
			view_node->nod_arg[e_mod_statement] =
				pass1_expand_view(tdbb, csb, view_stream, new_stream, true);
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_mod_sub_mod + 1);
			node = view_node;
		}
		else {
			csb->csb_rpt[new_stream].csb_flags &= ~csb_view_update;

			// View passes muster - do some translation. Start with source stream.

			map = csb->csb_rpt[stream].csb_map;
			stream = (USHORT)(ULONG) source->nod_arg[e_rel_stream];
			node->nod_arg[e_mod_org_stream] = (JRD_NOD) (SLONG) map[stream];

			// next, do update stream

			map =
				alloc_map(tdbb, csb,
						  (SSHORT)(SLONG) node->nod_arg[e_mod_new_stream]);
			source = copy(tdbb, csb, source, map, 0, false);
			node->nod_arg[e_mod_new_stream] = source->nod_arg[e_rel_stream];
		}
	}
}


static RSE pass1_rse(TDBB tdbb,
					 CSB csb,
					 RSE rse,
					 JRD_REL view,
					 USHORT view_stream)
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


	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(view, type_rel);

	// for scoping purposes, maintain a stack of rse's which are 
	// currently being parsed; if there are none on the stack as
	// yet, mark the rse as variant to make sure that statement-
	// level aggregates are not treated as invariants -- bug #6535

	if (!csb->csb_current_rses)
		rse->nod_flags |= rse_variant;
	LLS_PUSH(rse, &csb->csb_current_rses);

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

	// zip thru rse expanding views and inner joins

	for (arg = rse->rse_relation, end = arg + rse->rse_count; arg < end; arg++)
	{
		pass1_source(tdbb, csb, rse, *arg, &boolean, &stack, view,
		             view_stream);
	}

	// Now, rebuild the rse block. If possible, re-use the old block,
	// otherwise allocate a new one.

	for (count = 0, temp = stack; temp; temp = temp->lls_next)
		++count;

	if (count != rse->rse_count) {
		RSE new_rse;

		new_rse = (RSE) PAR_make_node(tdbb, count + rse_delta + 2);
		*new_rse = *rse;
		new_rse->rse_count = count;
		rse = new_rse;

		// AB: Because we've build an new rse, we must put this one in the stack 
		// of current_rses else could rse's not be flagged an rse_variant.
		// See SF BUG # [ 523589 ] for an example.

		LLS_POP(&csb->csb_current_rses);
		LLS_PUSH(new_rse, &csb->csb_current_rses);
	}


	arg = rse->rse_relation + count;

	while (stack)
		*--arg = (JRD_NOD) LLS_POP(&stack);

	// finish of by processing other clauses

	if (first)
		rse->rse_first = pass1(tdbb, csb, first, view, view_stream, false);
	if (skip)
		rse->rse_skip = pass1(tdbb, csb, skip, view, view_stream, false);

	if (boolean) {
		if (rse->rse_boolean) {
			JRD_NOD additional;

			additional = PAR_make_node(tdbb, 2);
			additional->nod_type = nod_and;
			additional->nod_arg[0] = boolean;
			additional->nod_arg[1] =
				pass1(tdbb, csb, rse->rse_boolean, view, view_stream, false);
			rse->rse_boolean = additional;
		}
		else
			rse->rse_boolean = boolean;
	}
	else
		rse->rse_boolean =
			pass1(tdbb, csb, rse->rse_boolean, view, view_stream, false);

	if (sort)
		rse->rse_sorted = pass1(tdbb, csb, sort, view, view_stream, false);

	if (project)
		rse->rse_projection =
			pass1(tdbb, csb, project, view, view_stream, false);

	if (plan)
		rse->rse_plan = plan;
	
	rse->rse_writelock = writelock;

#ifdef SCROLLABLE_CURSORS
	if (async_message) {
		rse->rse_async_message =
			pass1(tdbb, csb, async_message, view, view_stream, false);
		csb->csb_async_message = rse->rse_async_message;
	}
#endif

	// we are no longer in the scope of this rse

	LLS_POP(&csb->csb_current_rses);

	return rse;
}


static void pass1_source(TDBB     tdbb,
						 CSB      csb,
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

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(rse, type_nod);
	DEV_BLKCHK(source, type_nod);
	DEV_BLKCHK(*boolean, type_nod);
	DEV_BLKCHK(*stack, type_lls);
	DEV_BLKCHK(parent_view, type_rel);

	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	// in the case of an rse, it is possible that a new rse will be generated, 
	// so wait to process the source before we push it on the stack (bug 8039)

	if (source->nod_type == nod_rse)
	{
		// The addition of the JOIN syntax for specifying inner joins causes an 
		// rse tree to be generated, which is undesirable in the simplest case 
		// where we are just trying to inner join more than 2 streams. If possible, 
		// try to flatten the tree out before we go any further.

		RSE sub_rse = (RSE) source;
		if (!rse->rse_jointype && !sub_rse->rse_jointype
			&& !sub_rse->rse_sorted && !sub_rse->rse_projection
			&& !sub_rse->rse_first && !sub_rse->rse_plan) {
			for (arg = sub_rse->rse_relation, end = arg + sub_rse->rse_count;
				 arg < end; arg++)
				pass1_source(tdbb, csb, rse, *arg, boolean, stack,
							 parent_view, view_stream);

			// fold in the boolean for this inner join with the one for the parent

			if (sub_rse->rse_boolean) {
				node =
					pass1(tdbb, csb, sub_rse->rse_boolean, parent_view,
						  view_stream, false);
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

		source = pass1(tdbb, csb, source, parent_view, view_stream, false);
		LLS_PUSH(source, stack);
		return;
	}

	// Assume that the source will be used. Push it on the final stream stack.

	LLS_PUSH(source, stack);

	// special case: procedure

	if (source->nod_type == nod_procedure) {
		pass1(tdbb, csb, source, parent_view, view_stream, false);
		JRD_PRC procedure = MET_lookup_procedure_id(tdbb, 
		  (SSHORT)(SLONG) source->nod_arg[e_prc_procedure], FALSE, FALSE, 0);
		post_procedure_access(tdbb, csb, procedure);
		CMP_post_resource(tdbb, &csb->csb_resources, (BLK) procedure,
						  rsc_procedure, procedure->prc_id);
		return;
	}

	// special case: union

	if (source->nod_type == nod_union) {
		pass1(tdbb, csb, source->nod_arg[e_uni_clauses], parent_view,
			  view_stream, false);
		return;
	}

	// special case: group-by/global aggregates

	if (source->nod_type == nod_aggregate) {
		fb_assert((int)(SLONG) source->nod_arg[e_agg_stream] <= MAX_STREAMS);
		pass1(tdbb, csb, source, parent_view, view_stream, false);
		return;
	}

	// All the special cases are exhausted, so we must have a view or a base table; 
	// prepare to check protection of relation when a field in the stream of the 
	// relation is accessed.

	view = (JRD_REL) source->nod_arg[e_rel_relation];
	CMP_post_resource(tdbb, &csb->csb_resources, (BLK) view, rsc_relation,
					  view->rel_id);
	source->nod_arg[e_rel_view] = (JRD_NOD) parent_view;

	stream = (USHORT)(ULONG) source->nod_arg[e_rel_stream];
	element = CMP_csb_element(csb, stream);
	element->csb_view = parent_view;
	fb_assert(view_stream <= MAX_STREAMS);
	element->csb_view_stream = (UCHAR) view_stream;

	// in the case where there is a parent view, find the context name

	if (parent_view) {
		VCX *vcx_ptr;

		for (vcx_ptr = &parent_view->rel_view_contexts; *vcx_ptr;
			 vcx_ptr = &(*vcx_ptr)->vcx_next)
			if ((*vcx_ptr)->vcx_context ==
				(USHORT)(ULONG) source->nod_arg[e_rel_context]) {
				element->csb_alias = (*vcx_ptr)->vcx_context_name;
				break;
			}
	}

	// check for a view - if not, nothing more to do

	if (!(view_rse = view->rel_view_rse)) {
		return;
	}

	// we've got a view, expand it

	DEBUG;
	LLS_POP(stack);
	map = alloc_map(tdbb, csb, stream);

	// We don't expand the view in two cases: 
	// 1) If the view has a projection, and the query rse already has a projection 
	//    defined; there is probably some way to merge these projections and do them 
	//    both at once, but for now we'll punt on that.
	// 2) If it's part of an outer join. 
	//
	// AB: If the view has an projection we never expand it.
	// Because this can create wierd PLANs with multiple views/tables/sp joins.

	if ((view_rse->rse_projection)
	//if ((view_rse->rse_projection && rse->rse_projection)
		|| rse->rse_jointype) {
		node = copy(tdbb, csb, (JRD_NOD) view_rse, map, 0, false);
		DEBUG;
		LLS_PUSH(pass1(tdbb, csb, node, view, stream, false), stack);
		DEBUG;
		return;
	}

	// if we have a projection which we can bubble up to the parent rse, set the 
	// parent rse to our projection temporarily to flag the fact that we have already 
	// seen one so that lower-level views will not try to map their projection; the 
	// projection will be copied and correctly mapped later, but we don't have all 
	// the base streams yet

	if (view_rse->rse_projection)
		rse->rse_projection = view_rse->rse_projection;

	// disect view into component relations

	for (arg = view_rse->rse_relation, end = arg + view_rse->rse_count;
		 arg < end; arg++) {
		// this call not only copies the node, it adds any streams it finds to the map

		node = copy(tdbb, csb, *arg, map, 0, false);

		// Now go out and process the base table itself. This table might also be a view, 
		// in which case we will continue the process by recursion.

		pass1_source(tdbb, csb, rse, node, boolean, stack, view, stream);
	}

	// When there is a projection in the view, copy the projection up to the query rse.
	// In order to make this work properly, we must remap the stream numbers of the fields
	// in the view to the stream number of the base table. Note that the map at this point 
	// contains the stream numbers of the referenced relations, since it was added during the call 
	// to copy() above. After the copy() below, the fields in the projection will reference the 
	// base table(s) instead of the view's context (see bug #8822), so we are ready to context- 
	// recognize them in pass1() - that is, replace the field nodes with actual field blocks.

	if (view_rse->rse_projection)
		rse->rse_projection =
			pass1(tdbb, csb,
				  copy(tdbb, csb, view_rse->rse_projection, map, 0, false),
				  view, stream, false);

	// if we encounter a boolean, copy it and retain it by ANDing it in with the 
	// boolean on the parent view, if any

	if (view_rse->rse_boolean) {
		node =
			pass1(tdbb, csb,
				  copy(tdbb, csb, view_rse->rse_boolean, map, 0, false), view,
				  stream, false);
		if (*boolean) {
			JRD_NOD additional;

			// The order of the nodes here is important! The
			// boolean from the view must appear first so that
			// it gets expanded first in pass1.

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


static JRD_NOD pass1_store(TDBB tdbb, CSB csb, JRD_NOD node)
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
	USHORT stream, new_stream, parent_stream = 0;
	TRIG_VEC trigger;
	csb_repeat *tail;
	USHORT priv;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(node, type_nod);

	// If updateable views with triggers are involved, there
	// maybe a recursive call to be ignored

	if (node->nod_arg[e_sto_sub_store])
		return NULL;

	parent = view = NULL;
	bool trigger_seen = false;
	very_orig = node->nod_arg[e_sto_relation];

	// to support views of views, loop until we hit a real relation

	for (;;) {
		original = node->nod_arg[e_sto_relation];
		stream = (USHORT)(ULONG) original->nod_arg[e_rel_stream];
		tail = &csb->csb_rpt[stream];
		tail->csb_flags |= csb_store;
		relation = csb->csb_rpt[stream].csb_relation;
		view = (relation->rel_view_rse) ? relation : view;
		if (!parent)
			parent = tail->csb_view;
	    post_trigger_access(tdbb, csb, relation, relation->rel_pre_store, view);
		post_trigger_access(tdbb, csb, relation, relation->rel_post_store, view);
		trigger =
			(relation->rel_pre_store) ? relation->
			rel_pre_store : relation->rel_post_store;

		// Check out insert. If this is an insert thru a view, verify the
		// view by checking for read access on the base table. If field-level select
		// privileges are implemented, this needs to be enhanced.

		priv = SCL_sql_insert;
		if (parent)
			priv |= SCL_read;
		if (!
			(source =
			 pass1_update(tdbb, csb, relation, trigger, stream, stream, priv,
						  parent, parent_stream))) {
			CMP_post_resource(tdbb, &csb->csb_resources, (BLK) relation,
							  rsc_relation, relation->rel_id);
			return very_orig;
		}

		// view passes muster - do some translation

		parent = relation;
		parent_stream = stream;
		map = alloc_map(tdbb, csb, stream);
		if (!trigger) {
			csb->csb_rpt[stream].csb_flags &= ~csb_view_update;
			node->nod_arg[e_sto_relation] =
				copy(tdbb, csb, source, map, 0, false);
			if (!trigger_seen)
				very_orig = node->nod_arg[e_sto_relation];
		}
		else {
			CMP_post_resource(tdbb, &csb->csb_resources, (BLK) relation,
							  rsc_relation, relation->rel_id);
			trigger_seen = true;
			view_node = copy(tdbb, csb, node, map, 0, false);
			node->nod_arg[e_sto_sub_store] = view_node;
			node->nod_count =
				MAX(node->nod_count, (USHORT) e_sto_sub_store + 1);
			view_node->nod_arg[e_sto_sub_store] = 0;
			node = view_node;
			node->nod_arg[e_sto_relation] =
				copy(tdbb, csb, source, map, 0, false);
			new_stream =
				(USHORT)(ULONG) node->nod_arg[e_sto_relation]->nod_arg[e_rel_stream];
			node->nod_arg[e_sto_statement] =
				pass1_expand_view(tdbb, csb, stream, new_stream, true);
			node->nod_arg[e_sto_statement] =
				copy(tdbb, csb, node->nod_arg[e_sto_statement],
					 NULL, 0, false);

			// bug 8150: use of blr_store2 against a view with a trigger was causing 
			// the second statement to be executed, which is not desirable

			node->nod_arg[e_sto_statement2] = NULL;
		}
	}
}


static JRD_NOD pass1_update(TDBB tdbb,
							CSB csb,
							JRD_REL relation,
							TRIG_VEC trigger,
							USHORT stream,
							USHORT update_stream,
							USHORT priv,
							JRD_REL view,
							USHORT view_stream)
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

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(relation, type_rel);
	DEV_BLKCHK(view, type_rel);

	// unless this is an internal request, check access permission

	CMP_post_access(tdbb, csb, relation->rel_security_name, 
					(view ? view->rel_id : 0), 0, 0, 
					priv, object_table, relation->rel_name);

	// ensure that the view is set for the input streams,
	// so that access to views can be checked at the field level

	fb_assert(view_stream <= MAX_STREAMS);
	CMP_csb_element(csb, stream)->csb_view = view;
	CMP_csb_element(csb, stream)->csb_view_stream = (UCHAR) view_stream;
	CMP_csb_element(csb, update_stream)->csb_view = view;
	CMP_csb_element(csb, update_stream)->csb_view_stream = (UCHAR) view_stream;

	// if we're not a view, everything's cool

	if (!(rse = relation->rel_view_rse))
		return NULL;

	// a view with triggers is always updatable

	if (trigger) {
		csb->csb_rpt[update_stream].csb_flags |= csb_view_update;
		return NULL;
	}

	// we've got a view without triggers, let's check whether it's updateable

	if (rse->rse_count != 1 ||
		rse->rse_projection ||
		rse->rse_sorted ||
		!(node = rse->rse_relation[0]) || node->nod_type != nod_relation)
	{
		ERR_post(isc_read_only_view, isc_arg_string, relation->rel_name, 0);
	}

	// for an updateable view, return the view source

	csb->csb_rpt[update_stream].csb_flags |= csb_view_update;
	return rse->rse_relation[0];
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

	// if there is processing to be done before sub expressions, do it here

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

	case nod_dcl_cursor:
		rse_node = node->nod_arg[e_dcl_cursor_rse];
		rsb_ptr = (RSB *) & node->nod_arg[e_dcl_cursor_rsb];
#ifdef SCROLLABLE_CURSORS
		csb->csb_current_rse = rse_node;
#endif
		break;

	case nod_cursor_stmt:
		if ((UCHAR) (IPTR) node->nod_arg[e_cursor_stmt_op] == blr_cursor_fetch) {
			pass2(tdbb, csb, node->nod_arg[e_cursor_stmt_seek], node);
			pass2(tdbb, csb, node->nod_arg[e_cursor_stmt_into], node);
		}
		break;

#ifdef SCROLLABLE_CURSORS
	case nod_seek:
	case nod_seek_no_warn:
		// store the rse in whose scope we are defined
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
		if (! rse_node) {
			ERR_post(isc_wish_list, 0);
		}
		if (!(rse_node->nod_flags & rse_variant)) {
			node->nod_flags |= nod_invariant;
			LLS_PUSH(node, &csb->csb_invariants);
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
			LLS_PUSH(node, &csb->csb_invariants);
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
			function = (FUN) node->nod_arg[e_fun_function];
			pass2(tdbb, csb, value, node);
			// For gbak attachments, there is no need to resolve the UDF function */
			// Also if we are dropping a procedure don't bother resolving the
			// UDF that the procedure invokes.
			if (!(tdbb->tdbb_attachment->att_flags & ATT_gbak_attachment) &&
				!(tdbb->tdbb_flags & TDBB_prc_being_dropped)) {
				node->nod_arg[e_fun_function] =
					(JRD_NOD) FUN_resolve(csb, function, value);
				if (!node->nod_arg[e_fun_function])
					ERR_post(isc_funmismat, isc_arg_string,
							 function->fun_symbol->sym_string, 0);
			}
		}
		break;

#ifdef PC_ENGINE
		// the remainder of the node types are for IDAPI support:
		// fix up the stream to point to the base table, and preserve 
		// the pointers to the navigational rsb for easy reference 
		// later during execution

	case nod_stream:
		{
			JRD_NOD relation;
			RSE rse;

			rse = (RSE) node;
			rse_node = node;
			// setting the stream flag will allow the optimizer to  
			// detect that a SET INDEX may be done on this stream
			rse_node->nod_flags |= rse_stream;
			rsb_ptr = &rse->rse_rsb;
			relation = rse->rse_relation[0];
			stream = base_stream(csb, &relation->nod_arg[e_rel_stream], true);
			csb->csb_rpt[stream].csb_rsb_ptr = &rse->rse_rsb;
		}
		break;

	case nod_find:
		stream = base_stream(csb, &node->nod_arg[e_find_stream], true);
		if (!(node->nod_arg[e_find_rsb] =
			(JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_find_dbkey:
	case nod_find_dbkey_version:
		stream = base_stream(csb, &node->nod_arg[e_find_dbkey_stream], true);
		if (!(node->nod_arg[e_find_dbkey_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_set_index:
		stream = base_stream(csb, &node->nod_arg[e_index_stream], true);
		if (!(node->nod_arg[e_index_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_get_bookmark:
		stream = base_stream(csb, &node->nod_arg[e_getmark_stream], true);
		if (!(node->nod_arg[e_getmark_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_set_bookmark:
		stream = base_stream(csb, &node->nod_arg[e_setmark_stream], true);
		if (!(node->nod_arg[e_setmark_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_lock_record:
		stream = base_stream(csb, &node->nod_arg[e_lockrec_stream], true);
		if (!(node->nod_arg[e_lockrec_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_crack:
	case nod_force_crack:
		stream = base_stream(csb, &node->nod_arg[0], true);
		if (!(node->nod_arg[1] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
			ERR_post(isc_stream_not_defined, 0);
		break;

	case nod_reset_stream:
		stream = base_stream(csb, &node->nod_arg[e_reset_from_stream], true);
		if (!(node->nod_arg[e_reset_from_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

	case nod_cardinality:
		stream = base_stream(csb, &node->nod_arg[e_card_stream], true);
		if (!(node->nod_arg[e_card_rsb] =
			 (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr))
		{
			ERR_post(isc_stream_not_defined, 0);
		}
		break;

		// the following DML nodes need to have their rsb's stored when 
		// they are referencing a navigational stream, so that we can
		// follow proper IDAPI semantics in manipulating a stream

	case nod_erase:
		stream = base_stream(csb, &node->nod_arg[e_erase_stream], false);
		node->nod_arg[e_erase_rsb] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr;
		break;

	case nod_modify:
		stream = base_stream(csb, &node->nod_arg[e_mod_org_stream], false);
		node->nod_arg[e_mod_rsb] = (JRD_NOD) csb->csb_rpt[stream].csb_rsb_ptr;
		break;
#endif

	default:
		break;
	}

	if (rse_node)
		pass2_rse(tdbb, csb, (RSE) rse_node);

	// handle sub-expressions here

	if (node->nod_type == nod_modify) {
		// AB: Mark the streams involved with an UPDATE statement
		// active. So that the optimizer can use indices for 
		// eventually used sub-selects.
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags |= csb_active;
	}

	ptr = node->nod_arg;

	for (end = ptr + node->nod_count; ptr < end; ptr++) {
		pass2(tdbb, csb, *ptr, node);
	}

	if (node->nod_type == nod_modify) {
		// AB: Remove the previous flags
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_org_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
		stream = (USHORT)(ULONG) node->nod_arg[e_mod_new_stream];
		csb->csb_rpt[stream].csb_flags &= ~csb_active;
	}

	// Handle any residual work

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
		// FALL INTO

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
		csb->csb_impure += sizeof(vlux);
		break;

	case nod_ansi_all:
	case nod_ansi_any:
	case nod_any:
	case nod_exists:
	case nod_unique:
		if (node->nod_flags & nod_invariant)
			csb->csb_impure += sizeof(vlu);
		break;

	case nod_block:
		csb->csb_impure += sizeof(SLONG);
		break;

	case nod_dcl_variable:
		{
			DSC* desc = (DSC*) (node->nod_arg + e_dcl_desc);
			csb->csb_impure += sizeof(vlu) + desc->dsc_length;
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
			csb->csb_impure += sizeof(vlu);
			CMP_get_desc(tdbb, csb, node, &descriptor_a);
		}
		break;

	case nod_agg_average2:
	case nod_agg_average_distinct2:
		{
			DSC descriptor_a;

			node->nod_count = 0;
			csb->csb_impure += sizeof(vlux);
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

			stream = (USHORT)(ULONG) node->nod_arg[e_mod_org_stream];
			csb->csb_rpt[stream].csb_flags |= csb_update;
			format = CMP_format(tdbb, csb, stream);
			desc = format->fmt_desc.begin();
			for (id = 0; id < format->fmt_count; id++, desc++)
				if (desc->dsc_dtype)
					SBM_set(tdbb, &csb->csb_rpt[stream].csb_fields, id);
			csb->csb_impure += sizeof(sta);
		}
		break;

	case nod_list:
		node->nod_type = nod_asn_list;
		for (ptr = node->nod_arg; ptr < end; ptr++)
			if ((*ptr)->nod_type != nod_assignment) {
				node->nod_type = nod_list;
				break;
			}
		// FALL INTO

	case nod_store:
		csb->csb_impure += sizeof(sta);
		break;

	case nod_erase:
		stream = (USHORT)(ULONG) node->nod_arg[e_erase_stream];
		csb->csb_rpt[stream].csb_flags |= csb_update;
		break;

	case nod_field:
		stream = (USHORT)(ULONG) node->nod_arg[e_fld_stream];
		id = (USHORT)(ULONG) node->nod_arg[e_fld_id];
		SBM_set(tdbb, &csb->csb_rpt[stream].csb_fields, id);
		if (node->nod_flags & nod_value) {
			csb->csb_impure += sizeof(vlux);
			break;
		}
		// FALL INTO

	case nod_argument:
		csb->csb_impure += sizeof(dsc);
		break;

	case nod_concatenate:
    case nod_literal:
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
			csb->csb_impure += sizeof(vlu);
		}
		break;

	// compute the target descriptor to compute computational class

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
			csb->csb_impure += sizeof(vlu);
		}
		break;

	case nod_aggregate:
		pass2_rse(tdbb, csb, (RSE) node->nod_arg[e_agg_rse]);
		pass2(tdbb, csb, node->nod_arg[e_agg_map], node);
		pass2(tdbb, csb, node->nod_arg[e_agg_group], node);
		stream = (USHORT)(ULONG) node->nod_arg[e_agg_stream];
		fb_assert(stream <= MAX_STREAMS);
		process_map(tdbb, csb, node->nod_arg[e_agg_map],
					&csb->csb_rpt[stream].csb_format);
		break;

		// boolean nodes taking three values as inputs
	case nod_like:
	case nod_between:
	case nod_sleuth:
		if (node->nod_count > 2) {
			DSC descriptor_c;

			if (node->nod_arg[2]->nod_flags & nod_agg_dbkey)
				ERR_post(isc_bad_dbkey, 0);
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_c);
			if (DTYPE_IS_DATE(descriptor_c.dsc_dtype)) {
				node->nod_arg[0]->nod_flags |= nod_date;
				node->nod_arg[1]->nod_flags |= nod_date;
			}
		};
		// FALLINTO

		// boolean nodes taking two values as inputs
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
				ERR_post(isc_bad_dbkey, 0);
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
			CMP_get_desc(tdbb, csb, node->nod_arg[1], &descriptor_b);
			if (DTYPE_IS_DATE(descriptor_a.dsc_dtype))
				node->nod_arg[1]->nod_flags |= nod_date;
			else if (DTYPE_IS_DATE(descriptor_b.dsc_dtype))
				node->nod_arg[0]->nod_flags |= nod_date;
		}
		break;

		// boolean nodes taking one value as input
	case nod_missing:
		{
			DSC descriptor_a;

			if (node->nod_arg[0]->nod_flags & nod_agg_dbkey)
				ERR_post(isc_bad_dbkey, 0);

			// check for syntax errors in the calculation
			CMP_get_desc(tdbb, csb, node->nod_arg[0], &descriptor_a);
		}
		break;

	case nod_exec_into:
		csb->csb_impure += sizeof(class ExecuteStatement);
		break;

	default:
		// note: no fb_assert(false); here as too many nodes are missing
		break;
	}

	// finish up processing of record selection expressions

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
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags |= csb_active;
			pass2(tdbb, csb, node, (JRD_NOD) rse);
		}
		else if (node->nod_type == nod_rse) {
			pass2_rse(tdbb, csb, (RSE) node);
		}
		else if (node->nod_type == nod_procedure) {
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_prc_stream];
			csb->csb_rpt[stream].csb_flags |= csb_active;
			pass2(tdbb, csb, node, (JRD_NOD) rse);
		}
		else if (node->nod_type == nod_aggregate) {
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_agg_stream];
			fb_assert(stream <= MAX_STREAMS);
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

	// if the user has submitted a plan for this rse, check it for correctness

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

	// make up a format block sufficiently large to hold instantiated record

	clauses = node->nod_arg[e_uni_clauses];
	id = (USHORT)(ULONG) node->nod_arg[e_uni_stream];
	format = &csb->csb_rpt[id].csb_format;

	// process alternating rse and map blocks

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

	// if any streams are not marked with a plan, give an error

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end; ptr++)
		if ((*ptr)->nod_type == nod_relation) {
			stream = (USHORT)(ULONG) (*ptr)->nod_arg[e_rel_stream];
			if (!(csb->csb_rpt[stream].csb_plan))
				ERR_post(isc_no_stream_plan, isc_arg_string,
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

	// find the tail for the relation specified in the rse

	stream = (USHORT)(ULONG) plan_relation_node->nod_arg[e_rel_stream];
	tail = &csb->csb_rpt[stream];

	// if the plan references a view, find the real base relation 
	// we are interested in by searching the view map */

	if (tail->csb_map) {
		if (plan_alias)
			p = (TEXT *) plan_alias->str_data;
		else
			p = "\0";

		// if the user has specified an alias, skip past it to find the alias 
		// for the base table (if multiple aliases are specified)

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

		// loop through potentially a stack of views to find the appropriate base table

		while ( (map_base = tail->csb_map) ) {
			map = map_base;
			tail = &csb->csb_rpt[*map];
			view_relation = tail->csb_relation;

			// if the plan references the view itself, make sure that
			// the view is on a single table; if it is, fix up the plan
			// to point to the base relation

			if (view_relation->rel_id == plan_relation->rel_id) {
				if (!map_base[2]) {
					map++;
					tail = &csb->csb_rpt[*map];
				}
				else
					// view %s has more than one base relation; use aliases to distinguish
					ERR_post(isc_view_alias, isc_arg_string,
							 plan_relation->rel_name, 0);

				break;
			}
			else
				view_relation = NULL;

			// if the user didn't specify an alias (or didn't specify one
			// for this level), check to make sure there is one and only one 
			// base relation in the table which matches the plan relation

			if (!*p) {
				duplicate_relation = NULL;
				duplicate_map = map_base;
				map = NULL;
				for (duplicate_map++; *duplicate_map; duplicate_map++) {
					duplicate_tail = &csb->csb_rpt[*duplicate_map];
					relation = duplicate_tail->csb_relation;
					if (relation && relation->rel_id == plan_relation->rel_id) {
						if (duplicate_relation)
							// table %s is referenced twice in view; use an alias to distinguish
							ERR_post(isc_duplicate_base_table,
									 isc_arg_string,
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

			// look through all the base relations for a match

			map = map_base;
			for (map++; *map; map++) {
				tail = &csb->csb_rpt[*map];
				relation = tail->csb_relation;
				alias = tail->csb_alias;

				// match the user-supplied alias with the alias supplied
				// with the view definition; failing that, try the base
				// table name itself

				// CVC: I found that "relation" can be NULL, too. This may be an
				// indication of a logic flaw while parsing the user supplied SQL plan
				// and not an oversight here. It's hard to imagine a csb->csb_rpt with
				// a NULL relation. See exe.h for csb struct and its inner csb_repeat struct.

				if (
					(alias
					 && !strcmp_space(reinterpret_cast <
									  char *>(alias->str_data), p))
					|| (relation && !strcmp_space(relation->rel_name, p)))
					  break;
			}

			// skip past the alias

			while (*p && *p != ' ')
				p++;
			if (*p == ' ')
				p++;

			if (!*map)
				// table %s is referenced in the plan but not the from list
				ERR_post(isc_stream_not_found, isc_arg_string,
						 plan_relation->rel_name, 0);
		}

		// fix up the relation node to point to the base relation's stream

		if (!map || !*map)
			// table %s is referenced in the plan but not the from list
			ERR_post(isc_stream_not_found, isc_arg_string,
					 plan_relation->rel_name, 0);

		plan_relation_node->nod_arg[e_rel_stream] = (JRD_NOD) (SLONG) * map;
	}

	// make some validity checks

	if (!tail->csb_relation)
		// table %s is referenced in the plan but not the from list
		ERR_post(isc_stream_not_found, isc_arg_string,
				 plan_relation->rel_name, 0);

	if ((tail->csb_relation->rel_id != plan_relation->rel_id)
		&& !view_relation)
		// table %s is referenced in the plan but not the from list
		ERR_post(isc_stream_not_found, isc_arg_string,
				 plan_relation->rel_name, 0);

	// check if we already have a plan for this stream

	if (tail->csb_plan)
		// table %s is referenced more than once in plan; use aliases to distinguish
		ERR_post(isc_stream_twice, isc_arg_string,
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
	ACC access;
	TEXT *prc_sec_name;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(procedure, type_prc);

	prc_sec_name = (procedure->prc_security_name ?
					(TEXT *) procedure->
					prc_security_name->str_data : NULL);

	// this request must have EXECUTE permission on the stored procedure
	CMP_post_access(tdbb, csb, prc_sec_name, 0,
					0, 0, SCL_execute,
					object_procedure,
					reinterpret_cast <
					char *>(procedure->prc_name->str_data));

	// this request also inherits all the access requirements that
	// the procedure has
	if (procedure->prc_request)
		for (access = procedure->prc_request->req_access; access;
			 access = access->acc_next) {
			if (access->acc_trg_name ||
				access->acc_prc_name || access->acc_view_id)
				// inherited access needs from the trigger, view or SP
				// that this SP fires off
				CMP_post_access(tdbb, csb, access->acc_security_name,
								access->acc_view_id, access->acc_trg_name,
								access->acc_prc_name, access->acc_mask,
								access->acc_type, access->acc_name);
			else
				// direct access from this SP to a resource
				CMP_post_access(tdbb, csb, access->acc_security_name,
								0, 0,
								reinterpret_cast <
								char *>(procedure->prc_name->str_data),
								access->acc_mask, access->acc_type,
								access->acc_name);
		}
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
	// this flag lets the VIO layer know to add a page to the cache range
	if (rse->nod_flags & rse_stream)
		rsb->rsb_flags |= rsb_stream_type;
#endif

	// mark all the substreams as inactive

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count;
		 ptr < end; ptr++) {
		node = *ptr;
		if (node->nod_type == nod_relation) {
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_rel_stream];
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
		else if (node->nod_type == nod_procedure) {
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_prc_stream];
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
		else if (node->nod_type == nod_aggregate) {
			USHORT stream = (USHORT)(ULONG) node->nod_arg[e_agg_stream];
			fb_assert(stream <= MAX_STREAMS);
			csb->csb_rpt[stream].csb_flags &= ~csb_active;
		}
	}

	LLS_PUSH(rsb, &csb->csb_fors);
#ifdef SCROLLABLE_CURSORS
	rse->rse_rsb = rsb;
#endif

	return rsb;
}


static void post_trigger_access(TDBB tdbb, CSB csb, JRD_REL owner_relation, TRIG_VEC triggers, JRD_REL view)
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
	ACC access;
	trig_vec::iterator ptr, end;
//	USHORT read_only;

	SET_TDBB(tdbb);

	DEV_BLKCHK(csb, type_csb);
	DEV_BLKCHK(view, type_rel);

	if (!triggers)
		return;

	for (ptr = triggers->begin(), end = triggers->end(); ptr < end; ptr++) {
		ptr->compile(tdbb);
		if (ptr->request) {
			/* CVC: Definitely, I'm going to disable this check because REFERENCES should
			be checked only at DDL time. If we discover another thing in the fluffy SQL
			standard, we can revisit those lines.
			read_only = TRUE;
			for (access = ((JRD_REQ)(*ptr))->req_access; access;
				 access = access->acc_next)
			{
				if (access->acc_mask & ~SCL_read) {
					read_only = FALSE;
					break;
				}
			}
			*/

			// for read-only triggers, translate a READ access into a REFERENCES;
			// we must check for read-only to make sure people don't abuse the
			// REFERENCES privilege

			for (access = ptr->request->req_access; access;
				 access = access->acc_next) {
				/* CVC:	Can't make any sense of this code, hence I disabled it.
				if (read_only && (access->acc_mask & SCL_read)) {
					access->acc_mask &= ~SCL_read;
					access->acc_mask |= SCL_sql_references;
				}
				*/
				if (access->acc_trg_name ||
					access->acc_prc_name || access->acc_view_id)
				{
					// If this is not a system relation, we don't post access check if:
					//
					// - The table being checked is the owner of the trigger that's accessing it.
					// - The field being checked is owned by the same table than the trigger
					//   that's accessing the field.
					// - Since the trigger name comes in the access list, we need to validate that
					//   it's a trigger defined on our target table.
					// - Incidentally, access requests made through objects accessed by this trigger
					//   are granted automatically. We should achieve the same propagation in
					//   post_procedure_access() in the future, so the called proc/trg can use the
					//   rights of the caller even if the latter is a procedure or a trigger, with
					//   the difference that proc aren't bound to tables, so we need another place
					//   instead of post_procedure_access() to achieve such propagation.

					if (access->acc_trg_name && !(owner_relation->rel_flags & REL_system))
					{
						if (!strcmp(access->acc_type, object_table)
							&& !strcmp(access->acc_name, owner_relation->rel_name)
							&& MET_relation_owns_trigger(tdbb, access->acc_name, access->acc_trg_name))
							continue;
						if (!strcmp(access->acc_type, object_column)
							&& MET_relation_owns_trigger(tdbb, access->acc_name, access->acc_trg_name)
							&& (MET_lookup_field(tdbb, owner_relation, access->acc_name, access->acc_security_name) >= 0
							|| MET_relation_default_class(tdbb, owner_relation->rel_name, access->acc_security_name)))
							continue;
					}
					// inherited access needs from "object" to acc_security_name
					CMP_post_access(tdbb, csb, access->acc_security_name,
									access->acc_view_id,
									access->acc_trg_name,
									access->acc_prc_name, access->acc_mask,
									access->acc_type, access->acc_name);
				}
				else
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
							continue;
						if (!strcmp(access->acc_type, object_column)
							&& (MET_lookup_field(tdbb, owner_relation, access->acc_name, access->acc_security_name) >= 0
							|| MET_relation_default_class(tdbb, owner_relation->rel_name, access->acc_security_name)))
							continue;
					}
					// a direct access to an object from this trigger
					CMP_post_access(tdbb, csb, access->acc_security_name,
									(access->acc_view_id) ? access->acc_view_id : 
										(view ? view->rel_id : 0),
									ptr->request->req_trg_name, 0, access->acc_mask,
									access->acc_type, access->acc_name);
				}
			}
		}
	}
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

	// process alternating rse and map blocks

	ptr = map->nod_arg;

	for (end = ptr + map->nod_count; ptr < end; ptr++) {
		assignment = *ptr;
		field = assignment->nod_arg[e_asgn_to];
		id = (USHORT)(ULONG) field->nod_arg[e_fld_id];
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
		else if (!min)			// eg: dtype_null
			*desc = desc2;
		else if (min <= dtype_any_text) {	// either field a text field?
			USHORT len1, len2;

			len1 = DSC_string_length(desc);
			len2 = DSC_string_length(&desc2);
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length = MAX(len1, len2) + sizeof(USHORT);

			// pick the max text type, so any transparent casts from ints are 
			// not left in ASCII format, but converted to the richer text format

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
			// different numeric types: if one is inexact use double,
			// if both are exact use int64
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

	// flesh out the format of the record

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


static SSHORT strcmp_space(const TEXT* p, const TEXT* q)
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


static bool stream_in_rse(USHORT stream, RSE rse)
{
/**************************************
 *
 *	s t r e a m _ i n _ r s e
 *
 **************************************
 *
 * Functional description
 *	Return true if stream is contained in 
 *	the specified RSE.
 *
 **************************************/
	JRD_NOD sub, *ptr, *end;

	DEV_BLKCHK(rse, type_nod);

	// look through all relation nodes in this rse to see 
	// if the field references this instance of the relation

	for (ptr = rse->rse_relation, end = ptr + rse->rse_count; ptr < end;
		 ptr++) {
		sub = *ptr;

		// for aggregates, check current rse, if not found then check 
		// the sub-rse
		if (sub->nod_type == nod_aggregate) {
			if ((stream == (USHORT)(ULONG) sub->nod_arg[e_rel_stream]) ||
				(stream_in_rse(stream, (RSE) sub->nod_arg[e_agg_rse])))
				return true;	// do not mark as variant
		}

		if ((sub->nod_type == nod_relation) &&
			(stream == (USHORT)(ULONG) sub->nod_arg[e_rel_stream]))
			return true;		// do not mark as variant
	}

	return false;				// mark this rse as variant
}
