/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rng.cpp
 *	DESCRIPTION:	Refresh Range Handler
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

#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/ods.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "gen/iberror.h"
#include "../jrd/rse.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/rng.h"
#include "../jrd/sbm.h"
#include "../jrd/all.h"
#include "../jrd/intl.h"
#include "../jrd/tra.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/rng_proto.h"
#include "../jrd/thd.h"
#include "../jrd/tra_proto.h"
#include "../jrd/event_proto.h"

#ifdef PC_ENGINE
static void delete_range(RefreshRange*);
static void post_event(RefreshRange*);
static int post_event_ast(void*);
static void stop_creating(RefreshRange*);
#endif


#ifdef PC_ENGINE
void RNG_add_page(ULONG page_number)
{
/**************************************
 *
 *	R N G _ a d d _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Add page lock to any refresh range(s) 
 *	currently being defined.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	RefreshRange* next_refresh_range;
	for (RefreshRange* refresh_range = request->req_begin_ranges; refresh_range;
		 refresh_range = next_refresh_range)
	{
		next_refresh_range = refresh_range->rng_next;
		if (refresh_range->rng_flags & RNG_posted)
			continue;

		/* see if this refresh range already has a page lock on this page */

		bool next_range = false;
		VEC page_locks = refresh_range->rng_page_locks;
		if (page_locks) {
			for (size_t i = 0; i < refresh_range->rng_pages; i++) {
				Lock* page_lock = (Lock*) page_locks->vec_object[i];
				if (page_lock->lck_key.lck_long == page_number) {
					next_range = true;
					break;
				}
			}
		}
		if (next_range)
			continue;

		/* allocate and set up the page lock for this page */

		Lock* page_lock = CCH_page_lock(tdbb);
		page_lock->lck_key.lck_long = page_number;
		page_lock->lck_ast = post_event_ast;
		page_lock->lck_object = refresh_range;

		/* if we can get the lock, place it in a vector of page locks for this
		   refresh range; otherwise just post the range event assuming that the
		   page is being updated */

		if (LCK_lock_non_blocking(tdbb, page_lock, LCK_read, LCK_NO_WAIT)) {
			/* place all page locks into a vector */

			page_locks = ALL_vector(request->req_pool,
									&refresh_range->rng_page_locks,
									EXTEND(refresh_range->rng_pages));
			page_locks->vec_object[refresh_range->rng_pages] =
				(BLK) page_lock;
			refresh_range->rng_pages++;
		}
		else {
			post_event(refresh_range);
			delete_range(refresh_range);
		}
	}
}
#endif


#ifdef PC_ENGINE
void RNG_add_record(record_param* rpb)
{
/**************************************
 *
 *	R N G _ a d d _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Add record to refresh range(s) 
 *	currently being defined.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	RefreshRange* next_refresh_range;
	for (RefreshRange* refresh_range = request->req_begin_ranges; refresh_range;
		 refresh_range = next_refresh_range)
	{
		next_refresh_range = refresh_range->rng_next;
		if (refresh_range->rng_flags & RNG_posted)
			continue;

		/* see if this refresh range already has a lock on this record */

		bool next_range = false;
		VEC record_locks = refresh_range->rng_record_locks;
		if (record_locks) {
			for (size_t i = 0; i < refresh_range->rng_records; i++) {
				Lock* record_lock = (Lock*) record_locks->vec_object[i];
				if (record_lock->lck_key.lck_long == rpb->rpb_number) {
					next_range = true;
					break;
				}
			}
		}
		if (next_range)
			continue;

		/* get a lock on the passed record; if we can't, don't bother
		   to wait for it or retry, just post the event to let the
		   client know that an update is in process--if we're lucky
		   the lock will be released before the client tries to reestablish
		   the refresh range, otherwise we will thrash trying to establish */

		Lock* record_lock = RLCK_lock_record(rpb, LCK_PR,
									   post_event_ast,
									   refresh_range);
		if (!record_lock) {
			post_event(refresh_range);
			delete_range(refresh_range);
		}

		/* place all record locks into a vector for easy access later */

		record_locks = ALL_vector(request->req_pool,
								  &refresh_range->rng_record_locks,
								  EXTEND(refresh_range->rng_records));
		record_locks->vec_object[refresh_range->rng_records] =
			(BLK) record_lock;
		refresh_range->rng_records++;
	}

}
#endif


#ifdef PC_ENGINE
jrd_nod* RNG_add_relation(jrd_nod* node)
{
/**************************************
 *
 *	R N G _ a d d _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Add relation to refresh range(s) 
 *	currently being defined.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	if (request->req_operation == req_evaluate) {
		const dsc* desc = EVL_expr(tdbb, node->nod_arg[e_range_relation_number]);
		const size_t range_number = (USHORT) MOV_get_long(desc, 0);

		jrd_nod* relation_node = node->nod_arg[e_range_relation_relation];
		jrd_rel* relation = (jrd_rel*) relation_node->nod_arg[e_rel_relation];

		/* check to see if the range exists */

		RefreshRange* refresh_range;
		VEC refresh_ranges = request->req_refresh_ranges;
		if (refresh_ranges &&
			(range_number < refresh_ranges->vec_count) &&
			(refresh_range = (RefreshRange*) refresh_ranges->vec_object[range_number]))
		{
			/* lock the relation in such a way that we'll be notified 
			   if it is locked for update; if we can't get the lock just
			   give up and post the range */

			Lock* relation_lock = RLCK_range_relation(request->req_transaction,
												relation,
												post_event_ast,
												refresh_range);
			if (!relation_lock) {
				post_event(refresh_range);
				delete_range(refresh_range);
			}

			/* place all relation locks into a vector for easy access later */

			VEC relation_locks = ALL_vector(request->req_pool,
										&refresh_range->rng_relation_locks,
										EXTEND(refresh_range->rng_relations));
			relation_locks->vec_object[refresh_range->rng_relations] =
				(BLK) relation_lock;
			relation_locks =
				ALL_vector(request->req_pool,
						   &refresh_range->rng_relation_trans,
						   EXTEND(refresh_range->rng_relations));
			relation_locks->vec_object[refresh_range->rng_relations] =
				(BLK) request->req_transaction;
			refresh_range->rng_relations++;

		}
		request->req_operation = req_return;
	}

	return node->nod_parent;
}
#endif


#ifdef PC_ENGINE
void RNG_add_uncommitted_record(record_param* rpb)
{
/**************************************
 *
 *	R N G _ a d d _ u n c o m m i t t e d _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Add interest in an uncommitted record version to  
 *	any refresh ranges being created.  This is done by 
 *	registering interest in the transaction that created it.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	RefreshRange* next_refresh_range;
	for (RefreshRange* refresh_range = request->req_begin_ranges; refresh_range;
		 refresh_range = next_refresh_range)
	{
		next_refresh_range = refresh_range->rng_next;
		if (refresh_range->rng_flags & RNG_posted)
			continue;

		/* see if this refresh range already has a lock on this record */

		bool next_range = false;
		VEC transaction_locks = refresh_range->rng_transaction_locks;
		if (transaction_locks)
		{
			for (size_t i = 0; i < refresh_range->rng_transactions; i++) {
				Lock* transaction_lock = (Lock*) transaction_locks->vec_object[i];
				if (transaction_lock->lck_key.lck_long ==
					rpb->rpb_transaction_nr)
				{
					next_range = true;
					break;
				}
			}
		}
		if (next_range)
			continue;

		// We aren't using this code, but passing txn number is plain wrong.
		// Passing refresh_range (type compatible) will achieve the same result
		// than having to correct the lck_object data member.
		Lock* transaction_lock =
			TRA_transaction_lock(tdbb, refresh_range);//rpb->rpb_transaction_nr);
		transaction_lock->lck_key.lck_long = rpb->rpb_transaction_nr;
		transaction_lock->lck_ast = post_event_ast;
		//transaction_lock->lck_object = refresh_range;

		/* try to get a shared read on the transaction lock, which will force
		   the holder of an exclusive lock to downgrade; this is also his notification
		   that he needs to upgrade back to exclusive at transaction commit  */

		if (LCK_lock_non_blocking(tdbb, transaction_lock, LCK_SR, LCK_WAIT)) {
			/* place all transaction locks into a vector */

			transaction_locks = ALL_vector(request->req_pool,
										   &refresh_range->rng_transaction_locks,
										   EXTEND
										   (refresh_range->rng_transactions));
			transaction_locks->vec_object[refresh_range->rng_transactions] =
				(BLK) transaction_lock;
			refresh_range->rng_transactions++;
		}
		else {
			post_event(refresh_range);
			delete_range(refresh_range);
		}
	}
}
#endif


#ifdef PC_ENGINE
DSC *RNG_begin(jrd_nod* node, impure_value* impure)
{
/**************************************
 *
 *	R N G _ b e g i n
 *
 **************************************
 *
 * Functional description
 *	Initialize a refresh range.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;
	Database* dbb = tdbb->tdbb_database;
	jrd_tra* transaction = request->req_transaction;

/* check to make sure the range number is not already in use */

	const dsc* desc2 = EVL_expr(tdbb, node->nod_arg[e_brange_number]);
	const size_t range_number = (USHORT) MOV_get_long(desc2, 0);

	RefreshRange* refresh_range;
	VEC refresh_ranges = request->req_refresh_ranges;
	if (refresh_ranges &&
		(range_number < refresh_ranges->vec_count) &&
		(refresh_range = (RefreshRange*) refresh_ranges->vec_object[range_number]))
	{
		if (refresh_range->rng_flags & RNG_posted)
			delete_range(refresh_range);
		else
			ERR_post(isc_range_in_use, isc_arg_number, (SLONG) range_number,
					 0);
	}

/* generate a name unique to this particular event within this database,
   of the form "ISC_$RANGE_<tid>_<range_id>
   use the transaction id as a start, even though this event is not 
   related to the transction per se; it is just a means of generating
   an id unique to the database */

	char event_name[RANGE_NAME_LENGTH];
	char* p = event_name;
	for (const char* q = "ISC$RANGE_"; *q;)
		*p++ = *q++;

/* move in the transaction id */

	sprintf(p, "%ld_", transaction->tra_number);
	while (*p)
		p++;

/* move in the transaction-unique cache range id */

	sprintf(p, "%ld", transaction->tra_range_id++);
	while (*p)
		p++;

/* allocate the range block and move the event name into it */

	refresh_range = FB_NEW_RPT(*tdbb->getDefaultPool(), p - event_name + 1) RefreshRange();
	refresh_range->rng_event_length = p - event_name;
	strcpy(refresh_range->rng_event, event_name);

/* fill out the descriptor to point to the event name */

	impure->vlu_desc.dsc_dtype = dtype_text;
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.dsc_ttype() = ttype_ascii;
	impure->vlu_desc.dsc_address = refresh_range->rng_event;
	impure->vlu_desc.dsc_length = refresh_range->rng_event_length;

/* save the range's attachment */

	refresh_range->rng_attachment = tdbb->tdbb_attachment;

/* put the refresh range into a vector of ranges for this request */

	refresh_range->rng_number = range_number;
	refresh_ranges =
		ALL_vector(tdbb->getDefaultPool(), &request->req_refresh_ranges,
				   EXTEND(range_number));
	refresh_ranges->vec_object[range_number] = (BLK) refresh_range;

/* insert the range into the list of ranges actively being created */

	refresh_range->rng_next = request->req_begin_ranges;
	request->req_begin_ranges = refresh_range;
	++dbb->dbb_refresh_ranges;

/* downgrade ourselves to avoid selfdeadlock if another user attaches */

	CCH_down_grade_dbb(dbb);

	return &impure->vlu_desc;
}
#endif


#ifdef PC_ENGINE
jrd_nod* RNG_delete(jrd_nod* node)
{
/**************************************
 *
 *	R N G _ d e l e t e
 *
 **************************************
 *
 * Functional description
 *	Delete a previously created refresh range.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	if (request->req_operation == req_evaluate) {
		const dsc* desc = EVL_expr(tdbb, node->nod_arg[e_drange_number]);
		const size_t range_number = (USHORT) MOV_get_long(desc, 0);

		/* check to see if the range exists */
		RefreshRange* refresh_range;
		VEC refresh_ranges = request->req_refresh_ranges;
		if (!refresh_ranges ||
			(range_number >= refresh_ranges->vec_count) ||
			!(refresh_range = (RefreshRange*) refresh_ranges->vec_object[range_number]))
		{
			ERR_post(isc_range_not_found, isc_arg_number,
					 (SLONG) range_number, 0);
		}
		
		delete_range(refresh_range);

		request->req_operation = req_return;
	}

	return node->nod_parent;
}
#endif


#ifdef PC_ENGINE
void RNG_delete_ranges(jrd_req* request)
{
/**************************************
 *
 *	R N G _ d e l e t e _ r a n g e s
 *
 **************************************
 *
 * Functional description
 *	Delete all refresh ranges in a request.
 *
 **************************************/

/* release all refresh ranges associated with request */
	VEC refresh_ranges = request->req_refresh_ranges;
	if (refresh_ranges) {
		for (size_t range_number = 0; range_number < refresh_ranges->vec_count;
			 range_number++)
		{
			RefreshRange* refresh_range = 
				(RefreshRange*) refresh_ranges->vec_object[range_number];
			if (refresh_range)
				delete_range(refresh_range);
		}
	}
}
#endif


#ifdef PC_ENGINE
jrd_nod* RNG_end(jrd_nod* node)
{
/**************************************
 *
 *	R N G _ e n d
 *
 **************************************
 *
 * Functional description
 *	Stop adding records to refresh range.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

	if (request->req_operation == req_evaluate) {
		const dsc* desc = EVL_expr(tdbb, node->nod_arg[e_erange_number]);
		const size_t range_number = (USHORT) MOV_get_long(desc, 0);

		/* check to see if the range exists */
		RefreshRange* refresh_range;
		VEC refresh_ranges = request->req_refresh_ranges;
		if (refresh_ranges &&
			(range_number < refresh_ranges->vec_count) &&
			(refresh_range = (RefreshRange*) refresh_ranges->vec_object[range_number]))
		{
			/* if we've already posted the range, go ahead
			   and delete it; otherwise stop adding locks */

			if (refresh_range->rng_flags & RNG_posted)
				delete_range(refresh_range);
			else
				stop_creating(refresh_range);
		}

		request->req_operation = req_return;
	}

	return node->nod_parent;
}
#endif


#ifdef PC_ENGINE
void RNG_release_locks(RefreshRange* refresh_range)
{
/**************************************
 *
 *	R N G _ r e l e a s e _ l o c k s
 *
 **************************************
 *
 * Functional description
 *	Release all locks held by a refresh range.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* release all the relation locks */

	if (refresh_range->rng_relation_locks) {
		Lock** lock_ptr = (Lock**) refresh_range->rng_relation_locks->vec_object;
		for (int i = 0; i < refresh_range->rng_relations; i++) {
			LCK_release(tdbb, *lock_ptr);
			ALL_release(*lock_ptr);
			*lock_ptr = NULL;
			lock_ptr++;
		}
	}
	refresh_range->rng_relation_locks = 0;

/* release all the record locks */

	if (refresh_range->rng_record_locks) {
		Lock** lock_ptr = (Lock**) refresh_range->rng_record_locks->vec_object;
		for (int i = 0; i < refresh_range->rng_records; i++) {
			RLCK_unlock_record(*lock_ptr, 0);
			*lock_ptr = NULL;
			lock_ptr++;
		}
	}
	refresh_range->rng_record_locks = 0;

/* release all page locks */

	if (refresh_range->rng_page_locks) {
		Lock** lock_ptr = (Lock**) refresh_range->rng_page_locks->vec_object;
		for (int i = 0; i < refresh_range->rng_pages; i++) {
			LCK_release(tdbb, *lock_ptr);
			ALL_release(*lock_ptr);
			*lock_ptr = NULL;
			lock_ptr++;
		}
	}
	refresh_range->rng_page_locks = 0;

/* release all transaction locks */

	if (refresh_range->rng_transaction_locks) {
		Lock** lock_ptr = (Lock**) refresh_range->rng_transaction_locks->vec_object;
		for (int i = 0; i < refresh_range->rng_transactions; i++) {
			LCK_release(tdbb, *lock_ptr);
			ALL_release(*lock_ptr);
			*lock_ptr = NULL;
			lock_ptr++;
		}
	}
	refresh_range->rng_transaction_locks = 0;

}
#endif


#ifdef PC_ENGINE
void RNG_release_ranges(jrd_req* request)
{
/**************************************
 *
 *	R N G _ r e l e a s e _ r a n g e s
 *
 **************************************
 *
 * Functional description
 *	Release the locks for all ranges in a request.
 *
 **************************************/
	Database* dbb = GET_DBB();

	VEC refresh_ranges = request->req_refresh_ranges;
	if (refresh_ranges) {
		for (size_t range_number = 0; range_number < refresh_ranges->vec_count;
			 range_number++)
		{
			RefreshRange* refresh_range =
				(RefreshRange*) refresh_ranges->vec_object[range_number];
			if (refresh_range) {
				RNG_release_locks(refresh_range);
				--dbb->dbb_refresh_ranges;
			}
		}
	}
}
#endif


#ifdef PC_ENGINE
void RNG_shutdown_attachment(Attachment* attachment)
{
/**************************************
 *
 *	R N G _ s h u t d o w n _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Release refresh range locks for an attachment.
 *	This may be called at AST level, don't release memory.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	for (jrd_req* request = attachment->att_requests; request;
		 request = request->req_request)
	{
		VEC refresh_ranges = request->req_refresh_ranges;
		if (refresh_ranges)
		{
			for (size_t range_number = 0; range_number < refresh_ranges->vec_count;
				range_number++)
			{
				RefreshRange* refresh_range =
					(RefreshRange*) refresh_ranges->vec_object[range_number];
				if (refresh_range)
				{
					/* Shutdown range page locks */

					if (refresh_range->rng_page_locks) {
						Lock** lock_ptr =
							(Lock**) refresh_range->rng_page_locks->vec_object;
						for (int i = 0; i < refresh_range->rng_pages; i++) {
							LCK_release(tdbb, *lock_ptr);
							lock_ptr++;
						}
					}
					/* Shutdown range transaction locks */

					if (refresh_range->rng_transaction_locks) {
						Lock** lock_ptr =
							(Lock**) refresh_range->
							rng_transaction_locks->vec_object;
						for (int i = 0; i < refresh_range->rng_transactions; i++) {
							LCK_release(tdbb, *lock_ptr);
							lock_ptr++;
						}
					}
				}
			}
		}
	}
}
#endif


#ifdef PC_ENGINE
static void delete_range(RefreshRange* refresh_range)
{
/**************************************
 *
 *	d e l e t e _ r a n g e
 *
 **************************************
 *
 * Functional description
 *	Release all resources associated with
 *	a refresh range.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->tdbb_database;
	jrd_req* request = tdbb->tdbb_request;

/* remove from the vector of refresh ranges for the request */

	VEC refresh_ranges = request->req_refresh_ranges;
	refresh_ranges->vec_object[refresh_range->rng_number] = NULL;

	stop_creating(refresh_range);

	if (!(refresh_range->rng_flags & RNG_posted))
		RNG_release_locks(refresh_range);

	ALL_release(refresh_range);
	--dbb->dbb_refresh_ranges;
}
#endif


#ifdef PC_ENGINE
static void post_event(RefreshRange* refresh_range)
{
/**************************************
 *
 *	p o s t _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Post the event associated with the 
 *	refresh range.
 *
 **************************************/
	Database* dbb = GET_DBB();
	Lock* dbb_lock = dbb->dbb_lock;

/* detect duplicate posts and filter them out */

	if (refresh_range->rng_flags & RNG_posted)
		return;
	refresh_range->rng_flags |= RNG_posted;

/* post the event to the interested client */

	ISC_STATUS_ARRAY status;
	EVENT_post(status,
			   dbb_lock->lck_length,
			   (TEXT *) & dbb_lock->lck_key,
			   refresh_range->rng_event_length, refresh_range->rng_event, 1);
}
#endif


#ifdef PC_ENGINE
static int post_event_ast(void* refresh_range_void)
{
/**************************************
 *
 *	p o s t _ e v e n t _ a s t
 *
 **************************************
 *
 * Functional description
 *	Post the event associated with the 
 *	refresh range.
 *
 **************************************/
	thread_db thd_context, *tdbb;

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	JRD_set_thread_data(tdbb, thd_context);

	RefreshRange* refresh_range = static_cast<RefreshRange*>(refresh_range_void);
	tdbb->tdbb_database = refresh_range->rng_attachment->att_database;
	tdbb->tdbb_attachment = refresh_range->rng_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

	post_event(refresh_range);
	RNG_release_locks(refresh_range);

/* Restore the prior thread context */

	JRD_restore_thread_data();
	return 0;
}
#endif


#ifdef PC_ENGINE
static void stop_creating(RefreshRange* refresh_range)
{
/**************************************
 *
 *	s t o p _ c r e a t i n g
 *
 **************************************
 *
 * Functional description
 *	Remove the refresh range from the list of
 *	those being created.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	jrd_req* request = tdbb->tdbb_request;

/* delete from the list of ranges being created */

	for (RefreshRange** ptr = &request->req_begin_ranges; *ptr; 
		ptr = &(*ptr)->rng_next)
	{
		if (*ptr == refresh_range) {
			*ptr = (*ptr)->rng_next;
			break;
		}
	}
}
#endif

