/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tra.cpp
 *	DESCRIPTION:	Transaction manager
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/ods.h"
#include "../jrd/pag.h"
#include "../jrd/lck.h"
#include "../jrd/ibase.h"
#include "../jrd/lls.h"
#include "../jrd/all.h"
#include "../jrd/btr.h"
#include "../jrd/req.h"
#include "../jrd/exe.h"
#include "../jrd/rse.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/thd.h"
#include "../jrd/all_proto.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dfw_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/tpc_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/vio_proto.h"
#include "../jrd/enc_proto.h"
#include "../jrd/jrd_proto.h"
#include "../common/classes/ClumpletWriter.h"

#ifndef VMS
#include "../lock/lock_proto.h"
#endif


const int DYN_MSG_FAC	= 8;

#ifdef VMS
#include ssdef
#include lckdef

const int EVENT_FLAG	= 15;

static const SCHAR lock_types[] =
{
	0,
	LCK$K_NLMODE,
	LCK$K_CRMODE,
	LCK$K_CWMODE,
	LCK$K_PRMODE,
	LCK$K_PWMODE,
	LCK$K_EXMODE
};
#endif /* VMS */

const int DEFAULT_LOCK_TIMEOUT = -1; // infinite

using namespace Jrd;
using namespace Ods;

#ifdef GARBAGE_THREAD
#include "../jrd/isc_s_proto.h"
#endif

#ifdef SUPERSERVER_V2
static SLONG bump_transaction_id(thread_db*, WIN *);
#else
static header_page* bump_transaction_id(thread_db*, WIN *);
#endif
static void retain_context(thread_db*, jrd_tra*, const bool);
#ifdef VMS
static void compute_oldest_retaining(thread_db*, jrd_tra*, const bool);
#endif
#ifdef PC_ENGINE
static int downgrade_lock(void*);
#endif
static void expand_view_lock(jrd_tra*, jrd_rel*, SCHAR);
static tx_inv_page* fetch_inventory_page(thread_db*, WIN *, SLONG, USHORT);
static SLONG inventory_page(thread_db*, SLONG);
static SSHORT limbo_transaction(thread_db*, SLONG);
static void restart_requests(thread_db*, jrd_tra*);
#ifdef SWEEP_THREAD
static void start_sweeper(thread_db*, Database*);
static THREAD_ENTRY_DECLARE sweep_database(THREAD_ENTRY_PARAM);
#endif
static void transaction_options(thread_db*, jrd_tra*, const UCHAR*, USHORT);
#ifdef VMS
static void vms_convert(Lock*, SLONG*, SCHAR, bool);
#endif

static const UCHAR sweep_tpb[] =
{
	isc_tpb_version1, isc_tpb_read,
	isc_tpb_read_committed, isc_tpb_rec_version
};


void TRA_attach_request(Jrd::jrd_tra* transaction, Jrd::jrd_req* request) {

	// When request finishes normally transaction reference is not cleared.
	// Then if afterwards request is restarted TRA_attach_request is called again.
	if (request->req_transaction) {
		if (request->req_transaction == transaction) 
			return;
		TRA_detach_request(request);
	}

	fb_assert(request->req_transaction == NULL);
	fb_assert(request->req_tra_next == NULL);
	fb_assert(request->req_tra_prev == NULL);
	
	// Assign transaction reference
	request->req_transaction = transaction;

	// Add request to the doubly linked list
	if (transaction->tra_requests) {
		fb_assert(transaction->tra_requests->req_tra_prev == NULL);
		transaction->tra_requests->req_tra_prev = request;
		request->req_tra_next = transaction->tra_requests;
	}
	transaction->tra_requests = request;
}

void TRA_detach_request(Jrd::jrd_req* request) {
	if (!request->req_transaction) return;

	// Remove request from the doubly linked list
	if (request->req_tra_next) {
		fb_assert(request->req_tra_next->req_tra_prev == request);
		request->req_tra_next->req_tra_prev = request->req_tra_prev;
	}

	if (request->req_tra_prev) {
		fb_assert(request->req_tra_prev->req_tra_next == request);
		request->req_tra_prev->req_tra_next = request->req_tra_next;
	} else {
		fb_assert(request->req_transaction->tra_requests == request);
		request->req_transaction->tra_requests = request->req_tra_next;
	}

	// Clear references
	request->req_transaction = NULL;
	request->req_tra_next = NULL;
	request->req_tra_prev = NULL;
}

bool TRA_active_transactions(thread_db* tdbb, Database* dbb)
{
/**************************************
 *
 *	T R A _ a c t i v e _ t r a n s a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Determine if any transactions are active.
 *	Return true is active transactions; otherwise
 *	return false if no active transactions.
 *
 **************************************/
#ifndef VMS
	return ((LCK_query_data(dbb->dbb_lock, LCK_tra, LCK_ANY)) ? true : false);
#else
	SET_TDBB(tdbb);

/* Read header page and allocate transaction number. */

	ULONG number, oldest, active;
#ifdef SUPERSERVER_V2
	number = dbb->dbb_next_transaction;
	oldest = dbb->dbb_oldest_transaction;
	active = MAX(dbb->dbb_oldest_active, dbb->dbb_oldest_transaction);
#else
	if (dbb->dbb_flags & DBB_read_only) {
		number = dbb->dbb_next_transaction;
		oldest = dbb->dbb_oldest_transaction;
		active = MAX(dbb->dbb_oldest_active, dbb->dbb_oldest_transaction);
	}
	else {
		WIN window(HEADER_PAGE);
		const header_page* header = (header_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
		number = header->hdr_next_transaction;
		oldest = header->hdr_oldest_transaction;
		active =
			MAX(header->hdr_oldest_active, header->hdr_oldest_transaction);
		CCH_RELEASE(tdbb, &window);
	}
#endif /* SUPERSERVER_V2 */

	const ULONG base = oldest & ~TRA_MASK;

	jrd_tra* trans =
		FB_NEW_RPT(*dbb->dbb_permanent, (number - base + TRA_MASK) / 4) jrd_tra();

/* Build transaction bitmap to scan for active transactions. */

	TRA_get_inventory(tdbb, trans->tra_transactions, base, number);

	Lock temp_lock;
	temp_lock.lck_dbb = dbb;
	temp_lock.lck_object = trans;
	temp_lock.lck_type = LCK_tra;
	temp_lock.lck_owner_handle =
		LCK_get_owner_handle(tdbb, temp_lock.lck_type);
	temp_lock.lck_parent = dbb->dbb_lock;
	temp_lock.lck_length = sizeof(SLONG);

	for (; active <= number; active++) {
		const ULONG byte = TRANS_OFFSET(active - base);
		const USHORT shift = TRANS_SHIFT(active);
		const USHORT state = (trans->tra_transactions[byte] >> shift) & TRA_MASK;
		if (state == tra_active) {
			temp_lock.lck_key.lck_long = active;
			if (!LCK_lock_non_blocking(tdbb, &temp_lock, LCK_read, LCK_NO_WAIT)) {
				delete trans;
				return true;
			}
			LCK_release(tdbb, &temp_lock);
		}
	}

	delete trans;
	return false;
#endif
}

void TRA_cleanup(thread_db* tdbb)
{
/**************************************
 *
 *	T R A _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	TRA_cleanup is called at startup while an exclusive lock is
 *	held on the database.  Because we haven't started a transaction,
 *	and we have an exclusive lock on the db, any transactions marked
 *	as active on the transaction inventory pages are indeed dead.
 *	Mark them so.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Return without cleaning up the TIP's for a ReadOnly database */
	if (dbb->dbb_flags & DBB_read_only)
		return;

/* First, make damn sure there are no outstanding transactions */

	for (Attachment* attachment = dbb->dbb_attachments; attachment;
		 attachment = attachment->att_next)
	{
		if (attachment->att_transactions)
			return;
	}

	const SLONG trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;

/* Read header page and allocate transaction number.  Since
   the transaction inventory page was initialized to zero, it
   transaction is automatically marked active. */

	WIN window(HEADER_PAGE);
	const header_page* header = (header_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
	const SLONG ceiling = header->hdr_next_transaction;
	const SLONG active = header->hdr_oldest_active;
	CCH_RELEASE(tdbb, &window);

	if (ceiling == 0)
		return;

/* Zip thru transactions from the "oldest active" to the next looking for
   active transactions.  When one is found, declare it dead. */

	const SLONG last = ceiling / trans_per_tip;
	SLONG number = active % trans_per_tip;
	SLONG limbo = 0;

	for (SLONG sequence = active / trans_per_tip; sequence <= last;
		 sequence++, number = 0) 
	{
		window.win_page = inventory_page(tdbb, sequence);
		tx_inv_page* tip = (tx_inv_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_transactions);
		SLONG max = ceiling - (sequence * trans_per_tip);
		if (max > trans_per_tip)
			max = trans_per_tip - 1;
		for (; number <= max; number++) {
			const SLONG trans_offset = TRANS_OFFSET(number);
			UCHAR* byte = tip->tip_transactions + trans_offset;
			const SSHORT shift = TRANS_SHIFT(number);
			const SSHORT state = (*byte >> shift) & TRA_MASK;
			if (state == tra_limbo && limbo == 0)
				limbo = sequence * trans_per_tip + number;
			else if (state == tra_active) {
				CCH_MARK(tdbb, &window);
				*byte &= ~(TRA_MASK << shift);
				*byte |= tra_dead << shift;
			}
		}
#ifdef SUPERSERVER_V2
		if (sequence == last) {
			CCH_MARK(tdbb, &window);
			for (; number < trans_per_tip; number++) {
				const SLONG trans_offset = TRANS_OFFSET(number);
				UCHAR* byte = tip->tip_transactions + trans_offset;
				const SSHORT shift = TRANS_SHIFT(number);
				*byte &= ~(TRA_MASK << shift);
				if (tip->tip_next)
					*byte |= tra_committed << shift;
				else
					*byte |= tra_active << shift;
			}
		}
#endif
		CCH_RELEASE(tdbb, &window);
	}

#ifdef SUPERSERVER_V2
	window.win_page = inventory_page(tdbb, last);
	tx_inv_page* tip =
		(tx_inv_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_transactions);

	while (tip->tip_next) {
		CCH_RELEASE(tdbb, &window);
		window.win_page = inventory_page(tdbb, ++last);
		tip = (tx_inv_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_transactions);
		CCH_MARK(tdbb, &window);
		for (number = 0; number < trans_per_tip; number++) {
			const SLONG trans_offset = TRANS_OFFSET(number);
			UCHAR* byte = tip->tip_transactions + trans_offset;
			const USHORT shift = TRANS_SHIFT(number);
			*byte &= ~(TRA_MASK << shift);
			if (tip->tip_next || !number)
				*byte |= tra_committed << shift;
			else
				*byte |= tra_active << shift;
		}

		if (!tip->tip_next)
			dbb->dbb_next_transaction = last * trans_per_tip;
	}

	CCH_RELEASE(tdbb, &window);
#endif
}


void TRA_commit(thread_db* tdbb, jrd_tra* transaction, const bool retaining_flag)
{
/**************************************
 *
 *	T R A _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	SET_TDBB(tdbb);

/* If this is a commit retaining, and no updates have been performed,
   and no events have been posted (via stored procedures etc)
   no-op the operation */

	if (retaining_flag
		&& !(transaction->tra_flags & TRA_write
			 || transaction->tra_deferred_work))
	{
		transaction->tra_flags &= ~TRA_prepared;
		return;
	}

	if (transaction->tra_flags & TRA_invalidated)
		ERR_post(isc_trans_invalid, 0);

	Jrd::ContextPoolHolder context(tdbb, transaction->tra_pool);

/* Perform any meta data work deferred */

	if (!(transaction->tra_flags & TRA_prepared))
		DFW_perform_work(transaction);

	if (transaction->tra_flags & (TRA_prepare2 | TRA_reconnected))
		MET_update_transaction(tdbb, transaction, true);

/* Check in with external file system */

	EXT_trans_commit(transaction);

#ifdef GARBAGE_THREAD
/* Flush pages if transaction logically modified data */

	if (transaction->tra_flags & TRA_write)
#endif
		CCH_flush(tdbb, FLUSH_TRAN, transaction->tra_number);
#ifdef GARBAGE_THREAD
	else if (transaction->tra_flags & (TRA_prepare2 | TRA_reconnected)) {
		/* If the transaction only read data but is a member of a
		   multi-database transaction with a transaction description
		   message then flush RDB$TRANSACTIONS. */

		CCH_flush(tdbb, FLUSH_SYSTEM, 0);
	}
#endif

/* signal refresh range relations for ExpressLink */

#ifdef PC_ENGINE
	RLCK_signal_refresh(transaction);
#endif

	if (retaining_flag) {
		retain_context(tdbb, transaction, true);
		return;
	}

/* Set the state on the inventory page to be committed */

	TRA_set_state(tdbb, transaction, transaction->tra_number, tra_committed);

/* Perform any post commit work */

	DFW_perform_post_commit_work(transaction);

/* notify any waiting locks that this transaction is committing;
   there could be no lock if this transaction is being reconnected */

	++transaction->tra_use_count;
	Lock* lock = transaction->tra_lock;
	if (lock && (lock->lck_logical < LCK_write))
		LCK_convert(tdbb, lock, LCK_write, TRUE);
	--transaction->tra_use_count;

	TRA_release_transaction(tdbb, transaction);
}


void TRA_extend_tip(thread_db* tdbb, ULONG sequence, WIN * precedence_window)
{
/**************************************
 *
 *	T R A _ e x t e n d _ t i p
 *
 **************************************
 *
 * Functional description
 *	Allocate and link in new TIP (transaction inventory page).
 *	This is called from TRA_start and from validate/repair.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Start by fetching prior transaction page, if any */
	tx_inv_page* prior_tip;
	WIN prior_window(-1);
	if (sequence) {
		prior_tip =
			fetch_inventory_page(tdbb, &prior_window, (SLONG) (sequence - 1),
								 LCK_write);
	}

/* Allocate and format new page */
	WIN window(-1);
	tx_inv_page* tip = (tx_inv_page*) DPM_allocate(tdbb, &window);
	tip->tip_header.pag_type = pag_transactions;

	CCH_must_write(&window);
	CCH_RELEASE(tdbb, &window);

/* Release prior page */

	if (sequence) {
		CCH_MARK_MUST_WRITE(tdbb, &prior_window);
		prior_tip->tip_next = window.win_page;
		CCH_RELEASE(tdbb, &prior_window);
	}

/* Link into internal data structures */

	vcl* vector = dbb->dbb_t_pages =
		vcl::newVector(*dbb->dbb_permanent, dbb->dbb_t_pages, sequence + 1);
	(*vector)[sequence] = window.win_page;

/* Write into pages relation */

	DPM_pages(tdbb, 0, pag_transactions, sequence, window.win_page);
}


int TRA_fetch_state(thread_db* tdbb, SLONG number)
{
/**************************************
 *
 *	T R A _ f e t c h _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Physically fetch the state of a given
 *	transaction on the transaction inventory
 *	page.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* locate and fetch the proper TIP page */

    const ULONG tip_number = (ULONG) number;
	const SLONG trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	const ULONG tip_seq = tip_number / trans_per_tip;
	WIN window(-1);
	const tx_inv_page* tip = fetch_inventory_page(tdbb, &window, tip_seq, LCK_read);

/* calculate the state of the desired transaction */

	const ULONG byte = TRANS_OFFSET(tip_number % trans_per_tip);
	const USHORT shift = TRANS_SHIFT(tip_number);
	const USHORT state = (tip->tip_transactions[byte] >> shift) & TRA_MASK;

	CCH_RELEASE(tdbb, &window);

	return state;
}


void TRA_get_inventory(thread_db* tdbb, UCHAR* bit_vector, ULONG base, ULONG top)
{
/**************************************
 *
 *	T R A _ g e t _ i n v e n t o r y
 *
 **************************************
 *
 * Functional description
 *	Get an inventory of the state of all transactions
 *	between the base and top transactions passed.
 *	To get a consistent view of the transaction
 *	inventory (in case we ever implement sub-transactions),
 *	do handoffs to read the pages in order.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	const ULONG trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	ULONG sequence = base / trans_per_tip;
	const ULONG last = top / trans_per_tip;

/* fetch the first inventory page */

	WIN window(-1);
	const tx_inv_page* tip =
		fetch_inventory_page(tdbb, &window, (SLONG) sequence++, LCK_read);

/* move the first page into the bit vector */

	UCHAR* p = bit_vector;
	if (p) {
		ULONG l = base % trans_per_tip;
		const UCHAR* q = tip->tip_transactions + TRANS_OFFSET(l);
		l = TRANS_OFFSET(MIN((top + TRA_MASK - base), trans_per_tip - l));
		MOVE_FAST(q, p, l);
		p += l;
	}

/* move successive pages into the bit vector */

	while (sequence <= last) {
		base = sequence * trans_per_tip;

		/* release the read lock as we go, so that some one else can
		 * commit without having to signal all other transactions.
		 */

		tip =
			(tx_inv_page*) CCH_HANDOFF(tdbb, &window, inventory_page(tdbb, sequence++),
							  LCK_read, pag_transactions);
		TPC_update_cache(tdbb, tip, sequence - 1);
		if (p) {
			const ULONG l = TRANS_OFFSET(MIN((top + TRA_MASK - base), trans_per_tip));
			MOVE_FAST(tip->tip_transactions, p, l);
			p += l;
		}
	}

	CCH_RELEASE(tdbb, &window);
}


int TRA_get_state(thread_db* tdbb, SLONG number)
{
/**************************************
 *
 *	T R A _ g e t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the state of a given transaction on the
 *	transaction inventory page.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (dbb->dbb_tip_cache)
		return TPC_snapshot_state(tdbb, number);

	if (number && dbb->dbb_pc_transactions)
		if (TRA_precommited(tdbb, number, number))
			return tra_precommitted;

	return TRA_fetch_state(tdbb, number);
}


#ifdef SUPERSERVER_V2
void TRA_header_write(thread_db* tdbb, Database* dbb, SLONG number)
{
/**************************************
 *
 *	T R A _ h e a d e r _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Force transaction ID on header to disk.
 *	Do post fetch check of the transaction
 *	ID header write as a concurrent thread
 *	might have written the header page
 *	while blocked on the latch.
 *
 *	The idea is to amortize the cost of
 *	header page I/O across multiple transactions.
 *
 **************************************/
	SET_TDBB(tdbb);

/* If transaction number is already on disk just return. */

	if (!number || dbb->dbb_last_header_write < number) {
		WIN window(HEADER_PAGE);
		header_page* header = (header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);

		if (header->hdr_next_transaction) {
			if (header->hdr_oldest_active > header->hdr_next_transaction)
				BUGCHECK(266);	/*next transaction older than oldest active */

			if (header->hdr_oldest_transaction > header->hdr_next_transaction)
				BUGCHECK(267);	/* next transaction older than oldest transaction */
		}

		/* The header page might have been written while waiting
		   for the latch; perform a post fetch check and optimize
		   this case by not writing the page again. */

		if (!number || dbb->dbb_last_header_write < number) {
			CCH_MARK_MUST_WRITE(tdbb, &window);
			if (dbb->dbb_next_transaction > header->hdr_next_transaction)
				header->hdr_next_transaction = dbb->dbb_next_transaction;

			if (dbb->dbb_oldest_active > header->hdr_oldest_active)
				header->hdr_oldest_active = dbb->dbb_oldest_active;

			if (dbb->dbb_oldest_transaction > header->hdr_oldest_transaction)
				header->hdr_oldest_transaction = dbb->dbb_oldest_transaction;

			if (dbb->dbb_oldest_snapshot > header->hdr_oldest_snapshot)
				header->hdr_oldest_snapshot = dbb->dbb_oldest_snapshot;
		}

		CCH_RELEASE(tdbb, &window);
	}
}
#endif


void TRA_init(thread_db* tdbb)
{
/**************************************
 *
 *	T R A _ i n i t
 *
 **************************************
 *
 * Functional description
 *	"Start" the system transaction.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	jrd_tra* trans = FB_NEW_RPT(*dbb->dbb_permanent, 0) jrd_tra(*dbb->dbb_permanent);
	dbb->dbb_sys_trans = trans;
	trans->tra_lock_timeout = DEFAULT_LOCK_TIMEOUT;
	trans->tra_flags |= TRA_system | TRA_ignore_limbo;
	trans->tra_pool = dbb->dbb_permanent;
}


void TRA_invalidate(Database* database, ULONG mask)
{
/**************************************
 *
 *	T R A _ i n v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Invalidate any active transactions that may have
 *	modified a page that couldn't be written.
 *
 **************************************/
	for (Attachment* attachment = database->dbb_attachments; attachment;
		 attachment = attachment->att_next)
	{
			 
		for (jrd_tra* transaction = attachment->att_transactions; transaction; 
			transaction = transaction->tra_next) 
		{
			const ULONG transaction_mask =
				1L << (transaction->tra_number & (BITS_PER_LONG - 1));
			if (transaction_mask & mask && transaction->tra_flags & TRA_write)
				transaction->tra_flags |= TRA_invalidated;
		}
	}
}


void TRA_link_transaction(thread_db* tdbb, jrd_tra* transaction)
{
/**************************************
 *
 *	T R A _ l i n k _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Link transaction block into database attachment.
 *
 **************************************/
	SET_TDBB(tdbb);

	Attachment* attachment  = tdbb->tdbb_attachment;
	transaction->tra_attachment = attachment;
	transaction->tra_next = attachment->att_transactions;
	attachment->att_transactions = transaction;
}


void TRA_post_resources(thread_db* tdbb, jrd_tra* transaction, ResourceList& resources)
{
/**************************************
 *
 *	T R A _ p o s t _ r e s o u r c e s
 *
 **************************************
 *
 * Functional description
 *	Post interest in relation/procedure existence to transaction.
 *	This guarantees that the relation/procedure won't be dropped
 *	out from under the transaction.
 *
 **************************************/
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, transaction->tra_pool);

	for (Resource* rsc = resources.begin(); rsc < resources.end(); rsc++) 
	{
		if (rsc->rsc_type == Resource::rsc_relation ||
			rsc->rsc_type == Resource::rsc_procedure)
		{
			size_t i;
			if (!transaction->tra_resources.find(*rsc, i)) 
			{
				transaction->tra_resources.insert(i, *rsc);
				switch (rsc->rsc_type) {
				case Resource::rsc_relation:
					MET_post_existence(tdbb, rsc->rsc_rel);
					break;
				case Resource::rsc_procedure:
					rsc->rsc_prc->prc_use_count++;
#ifdef DEBUG_PROCS
					{
						char buffer[256];
						sprintf(buffer,
								"Called from TRA_post_resources():\n\t Incrementing use count of %s\n",
								rsc->rsc_prc->prc_name->c_str());
						JRD_print_procedure_info(tdbb, buffer);
					}
#endif
					break;
				default:   // shut up compiler warning
					break;
				}
			}
		}
	}
}


bool TRA_precommited(thread_db* tdbb, SLONG old_number, SLONG new_number)
{
/**************************************
 *
 *	T R A _ p r e c o m m i t e d	(s i c)
 *
 **************************************
 *
 * Functional description
 *	Maintain a vector of active precommitted
 *	transactions. If old_number <> new_number
 *	then swap old_number with new_number in
 *	the vector. If old_number equals new_number
 *	then test for that number's presence in
 *	the vector.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	vcl* vector = dbb->dbb_pc_transactions;
	if (!vector) {
		if (old_number == new_number)
			return false;
		vector = dbb->dbb_pc_transactions = vcl::newVector(*dbb->dbb_permanent, 1);
	}

	SLONG* zp = 0;
	for (vcl::iterator p = vector->begin(), end = vector->end(); p < end; p++) {
		if (*p == old_number)
			return (*p = new_number) ? true : false;
		if (!zp && !*p)
			zp = &*p;
	}

	if (old_number == new_number || new_number == 0)
		return false;
	if (zp)
		*zp = new_number;
	else {
		vector->resize(vector->count() + 1);
		(*vector)[vector->count() - 1] = new_number;
	}

	return true;
}


void TRA_prepare(thread_db* tdbb, jrd_tra* transaction, USHORT length,
	const UCHAR* msg)
{
/**************************************
 *
 *	T R A _ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Put a transaction into limbo.
 *
 **************************************/

	SET_TDBB(tdbb);

	if (transaction->tra_flags & TRA_prepared)
		return;

	if (transaction->tra_flags & TRA_invalidated)
		ERR_post(isc_trans_invalid, 0);

/* If there's a transaction description message, log it to RDB$TRANSACTION
   We should only log a message to RDB$TRANSACTION if there is a message
   to log (if the length = 0, we won't log the transaction in RDB$TRANSACTION)
   These messages are used to recover transactions in limbo.  The message indicates
   the action that is to be performed (hence, if nothing is getting logged, don't
   bother).
*/

/* Make sure that if msg is NULL there is no length.  The two
   should go hand in hand
              msg == NULL || *msg == NULL
*/
	fb_assert(!(!msg && length) || (msg && (!*msg && length)));

	if (msg && length) {
		MET_prepare(tdbb, transaction, length, msg);
		transaction->tra_flags |= TRA_prepare2;
	}

/* Check in with external file system */

	EXT_trans_prepare(transaction);

/* Perform any meta data work deferred */

	DFW_perform_work(transaction);

#ifdef GARBAGE_THREAD
/* Flush pages if transaction logically modified data */

	if (transaction->tra_flags & TRA_write)
#endif
		CCH_flush(tdbb, FLUSH_TRAN, transaction->tra_number);
#ifdef GARBAGE_THREAD
	else if (transaction->tra_flags & TRA_prepare2) {
		/* If the transaction only read data but is a member of a
		   multi-database transaction with a transaction description
		   message then flush RDB$TRANSACTIONS. */

		CCH_flush(tdbb, FLUSH_SYSTEM, 0);
	}
#endif

/* Set the state on the inventory page to be limbo */

	transaction->tra_flags |= TRA_prepared;
	TRA_set_state(tdbb, transaction, transaction->tra_number, tra_limbo);
}


jrd_tra* TRA_reconnect(thread_db* tdbb, const UCHAR* id, USHORT length)
{
/**************************************
 *
 *	T R A _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Reconnect to a transaction in limbo.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Cannot work on limbo transactions for ReadOnly database */
	if (dbb->dbb_flags & DBB_read_only)
		ERR_post(isc_read_only_database, 0);


	Jrd::ContextPoolHolder context(tdbb, JrdMemoryPool::createPool());
	jrd_tra* trans = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) jrd_tra(*tdbb->getDefaultPool());
	trans->tra_pool = tdbb->getDefaultPool();
	trans->tra_number = gds__vax_integer(id, length);
	trans->tra_lock_timeout = DEFAULT_LOCK_TIMEOUT;
	trans->tra_flags |= TRA_prepared | TRA_reconnected | TRA_write;

	const UCHAR state = limbo_transaction(tdbb, trans->tra_number);
	if (state != tra_limbo) {
		USHORT message;
		
		switch (state) {
		case tra_active:
			message = 262;		/* ACTIVE */
			break;
		case tra_dead:
			message = 264;		/* ROLLED BACK */
			break;
		case tra_committed:
			message = 263;		/* COMMITTED */
			break;
		default:
			message = 265;		/* ILL DEFINED */
			break;
		}

		const SLONG number = trans->tra_number;
		JrdMemoryPool::deletePool(trans->tra_pool);

		TEXT text[128];
		USHORT flags = 0;
		gds__msg_lookup(0, JRD_BUGCHK, message, sizeof(text), text, &flags);

		ERR_post(isc_no_recon,
				 isc_arg_gds, isc_tra_state,
				 isc_arg_number, number,
				 isc_arg_string, ERR_cstring(text), 0);
	}

	TRA_link_transaction(tdbb, trans);

	return trans;
}


void TRA_release_transaction(thread_db* tdbb, jrd_tra* transaction)
{
/**************************************
 *
 *	T R A _ r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Cleanup a transaction.  This is called by both COMMIT and
 *	ROLLBACK as well as code in JRD to get rid of remote
 *	transactions.
 *
 **************************************/
	SET_TDBB(tdbb);

	if (transaction->tra_blobs.getFirst()) do {
		BlobIndex *current = &transaction->tra_blobs.current();
		if (current->bli_materialized) {
			if (!transaction->tra_blobs.getNext())
				break;
		} else {
			ULONG temp_id = current->bli_temp_id;
			BLB_cancel(tdbb, current->bli_blob_object);
			if (!transaction->tra_blobs.locate(Firebird::locGreat, temp_id))
				break;
		}
	} while (true);

	while (transaction->tra_arrays)
		BLB_release_array(transaction->tra_arrays);

	if (transaction->tra_pool) {
		// Iterate the doubly linked list of requests for transaction and null out the transaction references
		while (transaction->tra_requests)
			TRA_detach_request(transaction->tra_requests);
	}

/* Release interest in relation/procedure existence for transaction */

	for (Resource* rsc = transaction->tra_resources.begin();
		rsc < transaction->tra_resources.end(); rsc++)
	{
		switch (rsc->rsc_type) {
		case Resource::rsc_procedure:
			CMP_decrement_prc_use_count(tdbb, rsc->rsc_prc);
			break;
		default:
			MET_release_existence(rsc->rsc_rel);
			break;
		}
	}

/* Release the locks associated with the transaction */

	vec* vector = transaction->tra_relation_locks;
	if (vector) {
		vec::iterator lock = vector->begin();
		for (ULONG i = 0; i < vector->count();
			 i++, lock++)
		{
			if (*lock)
				LCK_release(tdbb, (Lock*)*lock);
		}
	}

	++transaction->tra_use_count;
	if (transaction->tra_lock)
		LCK_release(tdbb, transaction->tra_lock);
	--transaction->tra_use_count;

/* release the sparse bit map used for commit retain transaction */

	delete transaction->tra_commit_sub_trans;

	if (transaction->tra_flags & TRA_precommitted)
		TRA_precommited(tdbb, transaction->tra_number, 0);

/* Unlink the transaction from the database block */

	for (jrd_tra** ptr = &tdbb->tdbb_attachment->att_transactions;
							*ptr; ptr = &(*ptr)->tra_next) 
	{ 
		if (*ptr == transaction) {
			*ptr = transaction->tra_next;
			break;
		}
	}

/* Release transaction's under-modification-rpb list. */

	delete transaction->tra_rpblist;

/* Release the transaction pool. */

	JrdMemoryPool* tra_pool = transaction->tra_pool;
	if (tra_pool)
		JrdMemoryPool::deletePool(tra_pool);
}


void TRA_rollback(thread_db* tdbb, jrd_tra* transaction, const bool retaining_flag)
{
/**************************************
 *
 *	T R A _ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Rollback a transaction.
 *
 **************************************/
	SET_TDBB(tdbb);

	Jrd::ContextPoolHolder context(tdbb, transaction->tra_pool);

/* Check in with external file system */

	EXT_trans_rollback(transaction);

/* If no writes have been made, commit the transaction instead. */

	if (transaction->tra_flags & (TRA_prepare2 | TRA_reconnected))
		MET_update_transaction(tdbb, transaction, false);

/*  Find out if there is a transaction savepoint we can use to rollback our transaction */
	bool tran_sav = false;
	for (const Savepoint* temp = transaction->tra_save_point; temp;
		temp = temp->sav_next)
	{
		if (temp->sav_flags & SAV_trans_level) {
			tran_sav = true;
			break;
		}
	}
	
/* Measure transaction savepoint size if there is one. We'll use it for undo
  only if it is small enough */
	IPTR count = SAV_LARGE;
	if (tran_sav) {
		for (const Savepoint* temp = transaction->tra_save_point; temp;
			temp = temp->sav_next)
		{
		    count = VIO_savepoint_large(temp, count);
			if (count < 0)
				break;
		}
	}
	
	// We are going to use savepoint to undo transaction
	if (tran_sav && count > 0) {
		// Undo all user savepoints work
		while (transaction->tra_save_point->sav_flags & SAV_user) {
			++transaction->tra_save_point->sav_verb_count;	/* cause undo */
			VIO_verb_cleanup(tdbb, transaction);
		}			
	}
	else {
		// Free all savepoint data
		// We can do it in reverse order because nothing except simple deallocation
		// of memory is really done in VIO_verb_cleanup when we pass NULL as sav_next
		while (transaction->tra_save_point &&
			transaction->tra_save_point->sav_flags & SAV_user)
		{
			Savepoint* const next = transaction->tra_save_point->sav_next;
			transaction->tra_save_point->sav_next = NULL;
			VIO_verb_cleanup(tdbb, transaction);
			transaction->tra_save_point = next;				
		}
		if (transaction->tra_save_point) {
			if (!(transaction->tra_save_point->sav_flags & SAV_trans_level))
				BUGCHECK(287);		/* Too many savepoints */
			/* This transaction savepoint contains wrong data now. Clean it up */
			VIO_verb_cleanup(tdbb, transaction);	/* get rid of transaction savepoint */
		}
	}

	// Only transaction savepoint could be there
	if (transaction->tra_save_point)
	{
		if (!(transaction->tra_save_point->sav_flags & SAV_trans_level))
			BUGCHECK(287);		/* Too many savepoints */

		/* Make sure that any error during savepoint undo is handled by marking
		   the transaction as dead. */

		try {

			/* In an attempt to avoid deadlocks, clear the precedence by writing
			   all dirty buffers for this transaction. */

			if (transaction->tra_flags & TRA_write) {
				CCH_flush(tdbb, FLUSH_TRAN, transaction->tra_number);
				++transaction->tra_save_point->sav_verb_count;	/* cause undo */
				VIO_verb_cleanup(tdbb, transaction);
				CCH_flush(tdbb, FLUSH_TRAN, transaction->tra_number);
			}
			else
				VIO_verb_cleanup(tdbb, transaction);

			/* If this a abort retaining transaction, do not
			 * set the TIP bits.  That will be done in retain_context ().
			 * Only ExpressLink runs in this mode and only DDL is
			 * executed in this manner.
			 */

			if (!retaining_flag)
				TRA_set_state(tdbb, transaction, transaction->tra_number,
							  tra_committed);
		}
		catch (const std::exception&) {
			/* Prevent a bugcheck in TRA_set_state to cause a loop */
			/* Clear the error because the rollback will succeed. */
			tdbb->tdbb_status_vector[0] = isc_arg_gds;
			tdbb->tdbb_status_vector[1] = 0;
			tdbb->tdbb_status_vector[2] = isc_arg_end;
			TRA_set_state(tdbb, transaction, transaction->tra_number,
						  tra_dead);
		}
	}
	else {
		/* Set the state on the inventory page to be dead */
		TRA_set_state(tdbb, transaction, transaction->tra_number, tra_dead);
	}

/* if this is a rollback retain (used only by ExpressLink), abort
 * this transaction and start a new one.
 */

	if (retaining_flag) {
		retain_context(tdbb, transaction, false);
		return;
	}

	TRA_release_transaction(tdbb, transaction);
}


void TRA_set_state(thread_db* tdbb, jrd_tra* transaction, SLONG number, SSHORT state)
{
/**************************************
 *
 *	T R A _ s e t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Set the state of a transaction in the inventory page.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* If we're terminating ourselves and we've
   been precommitted then just return. */

	if (transaction &&
		transaction->tra_number == number &&
		transaction->tra_flags & TRA_precommitted)
	{
		return;
	}

/* If it is a ReadOnly DB, set the new state in the TIP cache and return */
	if ((dbb->dbb_flags & DBB_read_only) && dbb->dbb_tip_cache) {
		TPC_set_state(tdbb, number, state);
		return;
	}

	ULONG trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	const SLONG sequence = number / trans_per_tip;
	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	const ULONG byte = TRANS_OFFSET(number % trans_per_tip);
	const SSHORT shift = TRANS_SHIFT(number);

	WIN window(-1);
	tx_inv_page* tip = fetch_inventory_page(tdbb, &window, (SLONG) sequence, LCK_write);

#ifdef SUPERSERVER_V2
	CCH_MARK(tdbb, &window);
	const ULONG generation = tip->pag_generation;
#else
	CCH_MARK_MUST_WRITE(tdbb, &window);
#endif

/* set the state on the TIP page */

	UCHAR* address = tip->tip_transactions + byte;
	*address &= ~(TRA_MASK << shift);
	*address |= state << shift;

/* set the new state in the TIP cache as well */

	if (dbb->dbb_tip_cache)
		TPC_set_state(tdbb, number, state);

	CCH_RELEASE(tdbb, &window);

#ifdef SUPERSERVER_V2
/* Let the TIP be lazily updated for read-only queries.
   To amortize write of TIP page for update transactions,
   exit engine to allow other transactions to update the TIP
   and use page generation to determine if page was written. */

	if (transaction && !(transaction->tra_flags & TRA_write))
		return;
	else {
		THREAD_EXIT();
		THREAD_ENTER();
		tip = reinterpret_cast<tx_inv_page*>(CCH_FETCH(tdbb, &window, LCK_write, pag_transactions));
		if (generation == tip->pag_generation)
			CCH_MARK_MUST_WRITE(tdbb, &window);
		CCH_RELEASE(tdbb, &window);
	}
#endif

}


void TRA_shutdown_attachment(thread_db* tdbb, Attachment* attachment)
{
/**************************************
 *
 *	T R A _ s h u t d o w n _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Release locks associated with transactions for attachment.
 *
 **************************************/
	SET_TDBB(tdbb);

	for (jrd_tra* transaction = attachment->att_transactions; transaction;
		 transaction = transaction->tra_next) 
	{
		/* Release the relation locks associated with the transaction */

		vec* vector = transaction->tra_relation_locks;
		if (vector) {
			vec::iterator lock = vector->begin();
			for (ULONG i = 0; i < vector->count(); i++, lock++) {
				if (*lock)
					LCK_release(tdbb, (Lock*)*lock);
			}
		}

		/* Release transaction lock itself */

		++transaction->tra_use_count;
		if (transaction->tra_lock)
			LCK_release(tdbb, transaction->tra_lock);
		--transaction->tra_use_count;
	}
}


int TRA_snapshot_state(thread_db* tdbb, const jrd_tra* trans, SLONG number)
{
/**************************************
 *
 *	T R A _ s n a p s h o t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the state of a numbered transaction when a
 *	transaction started.
 *
 **************************************/

	SET_TDBB(tdbb);

	if (number && TRA_precommited(tdbb, number, number))
		return tra_precommitted;

	if (number == trans->tra_number)
		return tra_us;

/* If the transaction is older than the oldest
   interesting transaction, it must be committed. */

	if (number < trans->tra_oldest)
		return tra_committed;

/* If the transaction is the system transaction, it is considered committed. */

	if (number == 0 || (trans->tra_flags & TRA_system))
		return tra_committed;

/* If the transaction is a commited sub-transction - do the easy lookup.
   Since this is not the most common case, and looking up the
   transaction cache for read committed transactions is equally
   fast, just to that instead. */

	if (trans->tra_flags & TRA_read_committed)
		return TPC_snapshot_state(tdbb, number);

	if (trans->tra_commit_sub_trans &&
		UInt32Bitmap::test(trans->tra_commit_sub_trans, number))
	{
		return tra_committed;
	}

/* If the transaction is younger than we are, it must be considered
   active. */

	if (number > trans->tra_top)
		return tra_active;

	return TRA_state(trans->tra_transactions, trans->tra_oldest, number);
}


jrd_tra* TRA_start(thread_db* tdbb, int tpb_length, const SCHAR* tpb)
{
/**************************************
 *
 *	T R A _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start a user transaction.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;
	WIN window(-1);

	if (dbb->dbb_ast_flags & DBB_shut_tran) {
		ERR_post(isc_shutinprog, isc_arg_cstring,
				 tdbb->tdbb_attachment->att_filename.length(),
				 tdbb->tdbb_attachment->att_filename.c_str(),
				 0);
	}

/* To handle the problems of relation locks, allocate a temporary
   transaction block first, sieze relation locks, the go ahead and
   make up the real transaction block. */

	Jrd::ContextPoolHolder context(tdbb, JrdMemoryPool::createPool());
	jrd_tra* temp = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) jrd_tra(*tdbb->getDefaultPool());
	temp->tra_pool = tdbb->getDefaultPool();
	transaction_options(tdbb, temp, reinterpret_cast<const UCHAR*>(tpb),
						tpb_length);

	Lock* lock = TRA_transaction_lock(tdbb, temp);

/* Read header page and allocate transaction number.  Since
   the transaction inventory page was initialized to zero, it
   transaction is automatically marked active. */

	ULONG oldest, number, active, oldest_active, oldest_snapshot;

#ifdef SUPERSERVER_V2
	number = bump_transaction_id(tdbb, &window);
	oldest = dbb->dbb_oldest_transaction;
	active = MAX(dbb->dbb_oldest_active, dbb->dbb_oldest_transaction);
	oldest_active = dbb->dbb_oldest_active;
	oldest_snapshot = dbb->dbb_oldest_snapshot;

#else /* SUPERSERVER_V2 */
	if (dbb->dbb_flags & DBB_read_only) {
		number = ++dbb->dbb_next_transaction;
		oldest = dbb->dbb_oldest_transaction;
		active = MAX(dbb->dbb_oldest_active, dbb->dbb_oldest_transaction);
		oldest_active = dbb->dbb_oldest_active;
		oldest_snapshot = dbb->dbb_oldest_snapshot;
	}
	else {
		const header_page* header = bump_transaction_id(tdbb, &window);
		number = header->hdr_next_transaction;
		oldest = header->hdr_oldest_transaction;
		active =
			MAX(header->hdr_oldest_active, header->hdr_oldest_transaction);
		oldest_active = header->hdr_oldest_active;
		oldest_snapshot = header->hdr_oldest_snapshot;
	}

#endif /* SUPERSERVER_V2 */

/* Allocate pool and transactions block.  Since, by policy,
   all transactions older than the oldest are either committed
   or cleaned up, they can be all considered as committed.  To
   make everything simpler, round down the oldest to a multiple
   of four, which puts the transaction on a byte boundary. */

	ULONG base = oldest & ~TRA_MASK;

	jrd_tra* trans;
	if (temp->tra_flags & TRA_read_committed)
		trans = FB_NEW_RPT(*tdbb->getDefaultPool(), 0) jrd_tra(*tdbb->getDefaultPool());
	else {
		trans = FB_NEW_RPT(*tdbb->getDefaultPool(), (number - base + TRA_MASK) / 4) jrd_tra(*tdbb->getDefaultPool());
	}

	trans->tra_pool = temp->tra_pool;
	trans->tra_relation_locks = temp->tra_relation_locks;
	trans->tra_lock_timeout = temp->tra_lock_timeout;
	trans->tra_flags = temp->tra_flags;
	trans->tra_number = number;
	trans->tra_top = number;
	trans->tra_oldest = oldest;
	trans->tra_oldest_active = active;
	delete temp;

	trans->tra_lock = lock;
	lock->lck_key.lck_long = number;

/* Support refresh range notification in ExpressLink */

#ifdef PC_ENGINE
	lock->lck_ast = downgrade_lock;
#endif

/* Put the TID of the oldest active transaction (from the header page)
   in the new transaction's lock. */

	lock->lck_data = active;
	lock->lck_object = trans;

	if (!LCK_lock_non_blocking(tdbb, lock, LCK_write, LCK_WAIT)) {
#ifndef SUPERSERVER_V2
		if (!(dbb->dbb_flags & DBB_read_only))
			CCH_RELEASE(tdbb, &window);
#endif
		delete trans;
		ERR_post(isc_lock_conflict, 0);
	}

/* Link the transaction to the attachment block before releasing
   header page for handling signals. */

	TRA_link_transaction(tdbb, trans);

#ifndef SUPERSERVER_V2
	if (!(dbb->dbb_flags & DBB_read_only))
		CCH_RELEASE(tdbb, &window);
#endif

	if (dbb->dbb_flags & DBB_read_only) {
		/* Set transaction flags to TRA_precommitted, TRA_readonly */
		trans->tra_flags |= (TRA_readonly | TRA_precommitted);
	}

/* Next, take a snapshot of all transactions between the oldest interesting
   transaction and the current.  Don't bother to get a snapshot for
   read-committed transactions; they use the snapshot off the dbb block
   since they need to know what is currently committed. */

	if (trans->tra_flags & TRA_read_committed)
		TPC_initialize_tpc(tdbb, number);
	else
		TRA_get_inventory(tdbb, trans->tra_transactions, base, number);

/* Next task is to find the oldest active transaction on the system.  This
   is needed for garbage collection.  Things are made ever so slightly
   more complicated by the fact that existing transaction may have oldest
   actives older than they are. */

	Lock temp_lock;
	temp_lock.lck_dbb = dbb;
	temp_lock.lck_object = trans;
	temp_lock.lck_type = LCK_tra;
	temp_lock.lck_owner_handle =
		LCK_get_owner_handle(tdbb, temp_lock.lck_type);
	temp_lock.lck_parent = dbb->dbb_lock;
	temp_lock.lck_length = sizeof(SLONG);

	trans->tra_oldest_active = number;
	base = oldest & ~TRA_MASK;
	oldest_active = number;
	bool cleanup = !(number % TRA_ACTIVE_CLEANUP);
	USHORT oldest_state;

	for (; active < number; active++) {
		if (trans->tra_flags & TRA_read_committed)
			oldest_state = TPC_cache_state(tdbb, active);
		else {
			const ULONG byte = TRANS_OFFSET(active - base);
			const USHORT shift = TRANS_SHIFT(active);
			oldest_state =
				(trans->tra_transactions[byte] >> shift) & TRA_MASK;
		}
		if (oldest_state == tra_active) {
			temp_lock.lck_key.lck_long = active;
			SLONG data = LCK_read_data(&temp_lock);
			if (!data) {
				if (cleanup) {
					if (TRA_wait(tdbb, trans, active, jrd_tra::tra_no_wait) == tra_committed)
						cleanup = false;
					continue;
				}
				else
					data = active;
			}

			oldest_active = MIN(oldest_active, active);

			/* Find the oldest record version that cannot be garbage collected yet
			   by taking the minimum of all all versions needed by all active
			   transactions. */

			if (data < trans->tra_oldest_active)
				trans->tra_oldest_active = data;

			/* If the lock data for any active transaction matches a previously
			   computed value then there is no need to continue. There can't be
			   an older lock data in the remaining active transactions. */

			if (trans->tra_oldest_active == (SLONG) oldest_snapshot)
				break;
#ifndef VMS
			/* Query the minimum lock data for all active transaction locks.
			   This will be the oldest active snapshot used for regulating
			   garbage collection. */

			data = LCK_query_data(dbb->dbb_lock, LCK_tra, LCK_MIN);
			if (data && data < trans->tra_oldest_active)
				trans->tra_oldest_active = data;
			break;
#endif
		}
	}

/* Put the TID of the oldest active transaction (just calculated)
   in the new transaction's lock. */

	if (lock->lck_data != (SLONG) oldest_active)
		LCK_write_data(lock, oldest_active);

/* Scan commit retaining transactions which have started after us but which
   want to preserve an oldest active from an already committed transaction.
   If a previously computed oldest snapshot was matched then there's no
   need to worry about commit retaining transactions. */

#ifdef VMS
	if (trans->tra_oldest_active != oldest_snapshot)
		compute_oldest_retaining(tdbb, trans, false);
#endif

/* Finally, scan transactions looking for the oldest interesting transaction -- the oldest
   non-commited transaction.  This will not be updated immediately, but saved until the
   next update access to the header page */

	oldest_state = tra_committed;

	for (oldest = trans->tra_oldest; oldest < number; oldest++) {
		if (trans->tra_flags & TRA_read_committed)
			oldest_state = TPC_cache_state(tdbb, oldest);
		else {
			const ULONG byte = TRANS_OFFSET(oldest - base);
			const USHORT shift = TRANS_SHIFT(oldest);
			oldest_state =
				(trans->tra_transactions[byte] >> shift) & TRA_MASK;
		}

		if (oldest_state != tra_committed && oldest_state != tra_precommitted)
			break;
	}

#ifdef MULTI_THREAD
	if (--oldest > (ULONG) dbb->dbb_oldest_transaction)
		dbb->dbb_oldest_transaction = oldest;

	if (oldest_active > (ULONG) dbb->dbb_oldest_active)
		dbb->dbb_oldest_active = oldest_active;
#else
	dbb->dbb_oldest_transaction = oldest - 1;
	dbb->dbb_oldest_active = oldest_active;
#endif

	if (trans->tra_oldest_active > dbb->dbb_oldest_snapshot) {
		dbb->dbb_oldest_snapshot = trans->tra_oldest_active;

#if defined(GARBAGE_THREAD)
		if (!(dbb->dbb_flags & DBB_gc_active) &&
			 (dbb->dbb_flags & DBB_gc_background) ) 
		{
			dbb->dbb_flags |= DBB_gc_pending;
			ISC_event_post(dbb->dbb_gc_event);
		}
#endif
	}

/* If the transaction block is getting out of hand, force a sweep */

	if (dbb->dbb_sweep_interval &&
		(trans->tra_oldest_active - trans->tra_oldest >
		 dbb->dbb_sweep_interval) && oldest_state != tra_limbo) {
#ifndef SWEEP_THREAD

		/* force a sweep */

		TRA_sweep(tdbb, trans);

#else
		// Why nobody checks the result? Changed the function to return nothing.
		start_sweeper(tdbb, dbb);
#endif
	}

/* Check in with external file system */

	EXT_trans_start(trans);

/* Start a 'transaction-level' savepoint, unless this is the
   system transaction, or unless the transactions doesn't want
   a savepoint to be started.  This savepoint will be used to
   undo the transaction if it rolls back. */

	if ((trans != dbb->dbb_sys_trans) &&
		!(trans->tra_flags & TRA_no_auto_undo)) {
		VIO_start_save_point(tdbb, trans);
		trans->tra_save_point->sav_flags |= SAV_trans_level;
	}

/* if the user asked us to restart all requests in this attachment,
   do so now using the new transaction */

	if (trans->tra_flags & TRA_restart_requests)
		restart_requests(tdbb, trans);

/* If the transaction is read-only and read committed, it can be
   precommitted because it can't modify any records and doesn't
   need a snapshot preserved. This is being enabled for internal
   transactions used by the sweeper threads. This transaction type
   can run forever without impacting garbage collection or causing
   transaction bitmap growth. It can be turned on for external use
   by removing the test for TDBB_sweeper. */

	if (trans->tra_flags & TRA_readonly &&
		trans->tra_flags & TRA_read_committed) {
		TRA_set_state(tdbb, trans, trans->tra_number, tra_committed);
		LCK_release(tdbb, trans->tra_lock);
		delete trans->tra_lock;
		trans->tra_lock = 0;
		trans->tra_flags |= TRA_precommitted;
	}

	if (trans->tra_flags & TRA_precommitted)
		TRA_precommited(tdbb, 0, trans->tra_number);

	return trans;
}


int TRA_state(const UCHAR* bit_vector, ULONG oldest, ULONG number)
{
/**************************************
 *
 *	T R A _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the state of a transaction from a cached
 *	bit vector.
 *	NOTE: This code is reproduced elsewhere in
 *	this module for speed.  If changes are made
 *	to this code make them in the replicated code also.
 *
 **************************************/
	const ULONG base = oldest & ~TRA_MASK;
	const ULONG byte = TRANS_OFFSET(number - base);
	const USHORT shift = TRANS_SHIFT(number);

	return (bit_vector[byte] >> shift) & TRA_MASK;
}


bool TRA_sweep(thread_db* tdbb, jrd_tra* trans)
{
/**************************************
 *
 *	T R A _ s w e e p
 *
 **************************************
 *
 * Functional description
 *	Make a garbage collection pass thru database.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* No point trying to sweep a ReadOnly database */
	if (dbb->dbb_flags & DBB_read_only)
		return false;

	if (dbb->dbb_flags & DBB_sweep_in_progress)
		return true;

/* fill out a lock block, zeroing it out first */

	Lock temp_lock;
	temp_lock.lck_dbb = dbb;
	temp_lock.lck_object = trans;
	temp_lock.lck_type = LCK_sweep;
	temp_lock.lck_owner_handle =
		LCK_get_owner_handle(tdbb, temp_lock.lck_type);
	temp_lock.lck_parent = dbb->dbb_lock;
	temp_lock.lck_length = sizeof(SLONG);

	if (!LCK_lock_non_blocking
		(tdbb, &temp_lock, LCK_EX, (trans) ? LCK_NO_WAIT : LCK_WAIT))
	{
		return true;
	}

	dbb->dbb_flags |= DBB_sweep_in_progress;

	jrd_tra* transaction = 0;
/* The following line seems to fix a bug that appears on VMS and AIX
   (and perhaps MPE XL).  It probably has to do with the fact that
   the error handler below used to contain the first reference to
   variable transaction, which is actually initialized a few lines
   below that. */

	transaction = *(&transaction);

/* Clean up the temporary locks we've gotten in case anything goes wrong */

	try {

/* Identify ourselves as a sweeper thread. This accomplishes
   two goals: 1) Sweep transaction is started "precommitted"
   and 2) Execution is throttled in JRD_reschedule() by
   yielding the processor when our quantum expires. */

	tdbb->tdbb_flags |= TDBB_sweeper;

/* Start a transaction, if necessary, to perform the sweep.
   Save the transaction's oldest snapshot as it is refreshed
   during the course of the database sweep. Since it is used
   below to advance the OIT we must save it before it changes. */

	if (!(transaction = trans))
		transaction = TRA_start(tdbb,
								sizeof(sweep_tpb),
								reinterpret_cast<const char*>(sweep_tpb));

	SLONG transaction_oldest_active = transaction->tra_oldest_active;

#ifdef GARBAGE_THREAD
/* The garbage collector runs asynchronously with respect to
   our database sweep. This isn't good enough since we must
   be absolutely certain that all dead transactions have been
   swept from the database before advancing the OIT. Turn off
   the "notify garbage collector" flag for the attachment and
   synchronously perform the garbage collection ourselves. */

	transaction->tra_attachment->att_flags &= ~ATT_notify_gc;
#endif

	if (VIO_sweep(tdbb, transaction)) {
		const ULONG base = transaction->tra_oldest & ~TRA_MASK;
		ULONG active;

		if (transaction->tra_flags & TRA_sweep_at_startup)
			active = transaction->tra_oldest_active;
		else {
			for (active = transaction->tra_oldest;
				 active < (ULONG) transaction->tra_top; active++) 
			{
				if (transaction->tra_flags & TRA_read_committed) {
					if (TPC_cache_state(tdbb, active) == tra_limbo)
						break;
				}
				else {
					const ULONG byte = TRANS_OFFSET(active - base);
					const USHORT shift = TRANS_SHIFT(active);
					if (
						((transaction->tra_transactions[byte] >> shift) &
						 TRA_MASK) == tra_limbo) 
					{
						break;
					}
				}
			}
		}

		/* Flush page buffers to insure that no dangling records from
		   dead transactions are left on-disk. This must be done before
		   the OIT is advanced and the header page is written to disk.
		   If the header page was written before flushing the page buffers
		   and there was a server crash, the dead records would appear
		   committed since their TID would now be less than the OIT recorded
		   in the database. */

		CCH_flush(tdbb, FLUSH_SWEEP, 0);

		WIN window(HEADER_PAGE);
		header_page* header =
			(header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);

		if (header->hdr_oldest_transaction < --transaction_oldest_active) {
			CCH_MARK_MUST_WRITE(tdbb, &window);
			header->hdr_oldest_transaction =
				MIN(active, (ULONG) transaction_oldest_active);
		}

		CCH_RELEASE(tdbb, &window);
	}

	if (!trans)
		TRA_commit(tdbb, transaction, false);

	LCK_release(tdbb, &temp_lock);
	dbb->dbb_flags &= ~DBB_sweep_in_progress;

	tdbb->tdbb_flags &= ~TDBB_sweeper;
	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		try {
			if (!trans)
				TRA_commit(tdbb, transaction, false);
			
			LCK_release(tdbb, &temp_lock);
			dbb->dbb_flags &= ~DBB_sweep_in_progress;
			tdbb->tdbb_flags &= ~TDBB_sweeper;
		}
		catch (const std::exception& ex2) {
			Firebird::stuff_exception(tdbb->tdbb_status_vector, ex2);
			LCK_release(tdbb, &temp_lock);
			dbb->dbb_flags &= ~DBB_sweep_in_progress;
			tdbb->tdbb_flags &= ~TDBB_sweeper;
			return false;
		}
	}

	return true;
}


Lock* TRA_transaction_lock(thread_db* tdbb, BLK object)
{
/**************************************
 *
 *	T R A _ t r a n s a c t i o n _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a transaction lock block.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	Lock* lock = FB_NEW_RPT(*tdbb->getDefaultPool(), sizeof(SLONG)) Lock();
	lock->lck_type = LCK_tra;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_length = sizeof(SLONG);

	lock->lck_dbb = dbb;
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_object = object;

	return lock;
}


int TRA_wait(thread_db* tdbb, jrd_tra* trans, SLONG number, jrd_tra::wait_t wait)
{
/**************************************
 *
 *	T R A _ w a i t
 *
 **************************************
 *
 * Functional description
 *	Wait for a given transaction to drop into a stable state (i.e. non-active)
 *	state.  To do this, we first wait on the transaction number.  When we
 *	are able to get the lock, the transaction is not longer bona fide
 *	active.  Next, we determine the state of the transaction from the
 *	transaction inventory page.  If either committed, dead, or limbo,
 *	we return the state.  If the transaction is still marked active,
 *	however, declare the transaction dead, and mark the transaction
 *	inventory page accordingly.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Create, wait on, and release lock on target transaction.  If
   we can't get the lock due to deadlock */

	if (wait != jrd_tra::tra_no_wait) {
		Lock temp_lock;
		temp_lock.lck_dbb = dbb;
		temp_lock.lck_type = LCK_tra;
		temp_lock.lck_owner_handle =
			LCK_get_owner_handle(tdbb, temp_lock.lck_type);
		temp_lock.lck_parent = dbb->dbb_lock;
		temp_lock.lck_length = sizeof(SLONG);
		temp_lock.lck_key.lck_long = number;
		temp_lock.lck_owner = trans;

		const SSHORT timeout =
			(wait == jrd_tra::tra_wait) ? trans->getLockWait() : 0;

		if (!LCK_lock_non_blocking(tdbb, &temp_lock, LCK_read, timeout))
			return tra_active;

		LCK_release(tdbb, &temp_lock);
	}

	USHORT state = TRA_get_state(tdbb, number);

	if (wait != jrd_tra::tra_no_wait && state == tra_committed)
		return state;

	if (state == tra_precommitted)
		return state;

/* If the recorded state of the transaction is active, we know better.  If
   it were active, he'd be alive now.  Mark him dead. */

	if (state == tra_active) {
		state = tra_dead;
		TRA_set_state(tdbb, 0, number, tra_dead);
	}

	if (number > trans->tra_top)
		return state;

/* If the transaction disppeared into limbo, died, for constructively
  died, tweak the transaction state snapshot to reflect the new state.
  This is guarenteed safe. */

	const ULONG byte = TRANS_OFFSET(number - (trans->tra_oldest & ~TRA_MASK));
	const USHORT shift = TRANS_SHIFT(number);

	if (trans->tra_flags & TRA_read_committed)
		TPC_set_state(tdbb, number, state);
	else {
		trans->tra_transactions[byte] &= ~(TRA_MASK << shift);
		trans->tra_transactions[byte] |= state << shift;
	}

	return state;
}


#ifdef SUPERSERVER_V2
static SLONG bump_transaction_id(thread_db* tdbb, WIN * window)
{
/**************************************
 *
 *	b u m p _ t r a n s a c t i o n _ i d
 *
 **************************************
 *
 * Functional description
 *	Fetch header and bump next transaction id.  If necessary,
 *	extend TIP.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	const ULONG number = ++dbb->dbb_next_transaction;

/* No need to write TID onto the TIP page, for a RO DB */
	if (dbb->dbb_flags & DBB_read_only)
		return number;

/* If this is the first transaction on a TIP, allocate the TIP now. */

	const bool new_tip = 
		(number == 1 || (number % dbb->dbb_pcontrol->pgc_tpt) == 0);
	if (new_tip) {
		TRA_extend_tip(tdbb,
					   (ULONG) (number / dbb->dbb_pcontrol->pgc_tpt), window);
	}

	return number;
}
#else


static header_page* bump_transaction_id(thread_db* tdbb, WIN * window)
{
/**************************************
 *
 *	b u m p _ t r a n s a c t i o n _ i d
 *
 **************************************
 *
 * Functional description
 *	Fetch header and bump next transaction id.  If necessary,
 *	extend TIP.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	window->win_page = HEADER_PAGE;
	header_page* header = (header_page*) CCH_FETCH(tdbb, window, LCK_write, pag_header);

/* Before incrementing the next transaction Id, make sure the current one is valid */
	if (header->hdr_next_transaction) {
		if (header->hdr_oldest_active > header->hdr_next_transaction)
			BUGCHECK(266);		/*next transaction older than oldest active */

		if (header->hdr_oldest_transaction > header->hdr_next_transaction)
			BUGCHECK(267);		/* next transaction older than oldest transaction */
	}

	const ULONG number = header->hdr_next_transaction + 1;

/* If this is the first transaction on a TIP, allocate the TIP now. */

	const bool new_tip = 
		(number == 1 || (number % dbb->dbb_pcontrol->pgc_tpt) == 0);
	if (new_tip) {
		TRA_extend_tip(tdbb,
					   (ULONG) (number / dbb->dbb_pcontrol->pgc_tpt), window);
	}

/* Extend, if necessary, has apparently succeeded.  Next, update header
   page */

	CCH_MARK_MUST_WRITE(tdbb, window);
	header->hdr_next_transaction = number;

	if (dbb->dbb_oldest_active > header->hdr_oldest_active)
		header->hdr_oldest_active = dbb->dbb_oldest_active;

	if (dbb->dbb_oldest_transaction > header->hdr_oldest_transaction)
		header->hdr_oldest_transaction = dbb->dbb_oldest_transaction;

	if (dbb->dbb_oldest_snapshot > header->hdr_oldest_snapshot)
		header->hdr_oldest_snapshot = dbb->dbb_oldest_snapshot;

	return header;
}
#endif


#ifdef VMS
static void compute_oldest_retaining(
									 thread_db* tdbb,
									 jrd_tra* transaction, const bool write_flag)
{
/**************************************
 *
 *	c o m p u t e _ o l d e s t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Read the oldest active for all transactions
 *	younger than us up to the youngest retaining
 *	transaction. If an "older" oldest active is
 *	found, by all means use it. Write flag is TRUE
 *	to write retaining lock and FALSE to read it.
 *	The retaining lock holds the youngest commit
 *	retaining transaction.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Get a commit retaining lock, if not present. */

	Lock* lock = dbb->dbb_retaining_lock;
	if (!lock) {
		lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) Lock();
		lock->lck_dbb = dbb;
		lock->lck_type = LCK_retaining;
		lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
		lock->lck_parent = dbb->dbb_lock;
		lock->lck_length = sizeof(SLONG);
		lock->lck_object = reinterpret_cast<blk*>(dbb);
#ifdef VMS
		if (LCK_lock(tdbb, lock, LCK_EX, LCK_NO_WAIT)) {
			number = 0;
			vms_convert(lock, &number, LCK_SR, true);
		}
		else
			LCK_lock(tdbb, lock, LCK_SR, LCK_WAIT);
#else
		LCK_lock_non_blocking(tdbb, lock, LCK_SR, LCK_WAIT);
#endif
		dbb->dbb_retaining_lock = lock;
	}

	SLONG number = transaction->tra_number;

/* Writers must synchronize their lock update so that
   an older retaining is not written over a younger retaining.
   In any case, lock types have been selected so that
   readers and writers don't interfere. */

	SLONG youngest_retaining;
	
	if (write_flag) {
#ifdef VMS
		vms_convert(lock, &youngest_retaining, LCK_PW, true);
		if (number > youngest_retaining)
			vms_convert(lock, &number, LCK_SR, true);
		else
			vms_convert(lock, 0, LCK_SR, true);
#else
		LCK_convert(tdbb, lock, LCK_PW, TRUE);
		youngest_retaining = LOCK_read_data(lock->lck_id);
		if (number > youngest_retaining)
			LCK_write_data(lock, number);
		LCK_convert(tdbb, lock, LCK_SR, TRUE);
#endif
	}
	else {
#ifdef VMS
		vms_convert(lock, &youngest_retaining, LCK_SR, true);
#else
		youngest_retaining = LOCK_read_data(lock->lck_id);
#endif
		if (number > youngest_retaining)
			return;

		/* fill out a lock block, zeroing it out first */
		Lock temp_lock;
		temp_lock.lck_dbb = dbb;
		temp_lock.lck_type = LCK_tra;
		temp_lock.lck_owner_handle =
			LCK_get_owner_handle(tdbb, temp_lock.lck_type);
		temp_lock.lck_parent = dbb->dbb_lock;
		temp_lock.lck_length = sizeof(SLONG);
		temp_lock.lck_object = transaction;

		while (number < youngest_retaining) {
			temp_lock.lck_key.lck_long = ++number;
			const SLONG data = LCK_read_data(&temp_lock);
			if (data && data < transaction->tra_oldest_active)
				transaction->tra_oldest_active = data;
		}
	}
}
#endif

#ifdef PC_ENGINE
static int downgrade_lock(void* transaction_void)
{
/**************************************
 *
 *	d o w n g r a d e _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Someone is trying to establish an interest
 *	lock in this transaction.  Downgrade to a
 *	shared write, to allow transactions to wait
 *	on this transaction or, alternatively, be
 *	notified if and when the transaction commits.
 *
 **************************************/
	ISC_ast_enter();
	jrd_tra* transaction = static_cast<jrd_tra*>(transaction_void);

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

/* Ignore the request if the transaction or lock block does not appear
   to be valid or if the lock is not a write lock. */

	if (transaction->tra_use_count);
	else {
		tdbb->tdbb_database = transaction->tra_attachment->att_database;
		tdbb->tdbb_attachment = transaction->tra_attachment;
		tdbb->tdbb_quantum = QUANTUM;
		tdbb->tdbb_request = NULL;
		tdbb->tdbb_transaction = transaction;

		++transaction->tra_use_count;
		Lock* lock = transaction->tra_lock;
		if (lock && lock->lck_logical == LCK_write) {
			lock->lck_ast = NULL;
			LCK_convert(tdbb, lock, LCK_SW, TRUE);
		}
		--transaction->tra_use_count;
	}

/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();
	return 0;
}
#endif

static void expand_view_lock(jrd_tra* transaction, jrd_rel* relation, SCHAR lock_type)
{
/**************************************
 *
 *	e x p a n d _ v i e w _ l o c k
 *
 **************************************
 *
 * Functional description
 *	A view in a RESERVING will lead to all tables in the
 *	view being locked.
 *
 **************************************/

	thread_db* tdbb = JRD_get_thread_data();

	/* set up the lock on the relation/view */

	Lock* lock = RLCK_transaction_relation_lock(transaction, relation);

	lock->lck_logical = lock_type;

	ViewContext* ctx = relation->rel_view_contexts;
	if (!ctx) {
		return;
	}

	for (; ctx; ctx = ctx->vcx_next)
	{
		jrd_rel* rel = MET_lookup_relation(tdbb,
								reinterpret_cast<const char*>(ctx->vcx_relation_name->str_data));
		if (!rel)
		{
			ERR_post(isc_bad_tpb_content,	/* should be a BUGCHECK */
					isc_arg_gds,
					isc_relnotdef,
					isc_arg_string,
					ERR_cstring(reinterpret_cast<const char*>
						(ctx->vcx_relation_name->str_data)),
					0);
		}

		/* force a scan to read view information */
		MET_scan_relation(tdbb, rel);

		expand_view_lock(transaction, rel, lock_type);
	}
}


static tx_inv_page* fetch_inventory_page(
								thread_db* tdbb,
								WIN * window,
								SLONG sequence, USHORT lock_level)
{
/**************************************
 *
 *	f e t c h _ i n v e n t o r y _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Fetch a transaction inventory page.
 *	Use the opportunity to cache the info
 *	in the TIP cache.
 *
 **************************************/
	SET_TDBB(tdbb);

	window->win_page = inventory_page(tdbb, (int) sequence);
	tx_inv_page* tip =
		(tx_inv_page*) CCH_FETCH(tdbb, window, lock_level, pag_transactions);

	TPC_update_cache(tdbb, tip, sequence);

	return tip;
}


static SLONG inventory_page(thread_db* tdbb, SLONG sequence)
{
/**************************************
 *
 *	i n v e n t o r y _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Get the physical page number of the n-th transaction inventory
 *	page. If not found, try to reconstruct using sibling pointer
 *	from last known TIP page.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	WIN window(-1);
	vcl* vector = dbb->dbb_t_pages;
	while (!vector || sequence >= (SLONG) vector->count()) {
		DPM_scan_pages(tdbb);
		if ((vector = dbb->dbb_t_pages) && sequence < (SLONG) vector->count())
			break;
		if (!vector)
			BUGCHECK(165);		/* msg 165 cannot find tip page */
		window.win_page = (*vector)[vector->count() - 1];
		tx_inv_page* tip =
			(tx_inv_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_transactions);
		const SLONG next = tip->tip_next;
		CCH_RELEASE(tdbb, &window);
		if (!(window.win_page = next))
			BUGCHECK(165);		/* msg 165 cannot find tip page */
		// Type check it
		tip = (tx_inv_page*) CCH_FETCH(tdbb, &window, LCK_read, pag_transactions);
		CCH_RELEASE(tdbb, &window);
		DPM_pages(tdbb, 0, pag_transactions, vector->count(),
				  window.win_page);
	}

	return (*vector)[sequence];
}


static SSHORT limbo_transaction(thread_db* tdbb, SLONG id)
{
/**************************************
 *
 *	l i m b o _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *
 *	limbo_state is called when reconnecting
 *	to an existing transaction to assure that
 *	the transaction is actually in limbo.
 *	It returns the transaction state.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	const SLONG trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;

	const SLONG page = id / trans_per_tip;
	const SLONG number = id % trans_per_tip;

	WIN window(-1);
	const tx_inv_page* tip = fetch_inventory_page(tdbb, &window, page, LCK_write);

	const SLONG trans_offset = TRANS_OFFSET(number);
	const UCHAR* byte = tip->tip_transactions + trans_offset;
	const SSHORT shift = TRANS_SHIFT(number);
	const SSHORT state = (*byte >> shift) & TRA_MASK;
	CCH_RELEASE(tdbb, &window);

	return state;
}


static void restart_requests(thread_db* tdbb, jrd_tra* trans)
{
/**************************************
 *
 *	r e s t a r t _ r e q u e s t s
 *
 **************************************
 *
 * Functional description
 *	Restart all requests in the current
 *	attachment to utilize the passed
 *	transaction.
 *
 **************************************/
	SET_TDBB(tdbb);
	for (jrd_req* request = trans->tra_attachment->att_requests; request;
		 request = request->req_request) 
	{
		if (request->req_transaction) {
			EXE_unwind(tdbb, request);
			EXE_start(tdbb, request, trans);
		}

		/* now take care of any other request levels;
		   start at level 1 since level 0 was just handled */

		vec* vector = request->req_sub_requests;
		if (vector) {
			for (USHORT level = 1; level < vector->count(); level++) {
				jrd_req* clone = (jrd_req*) (*vector)[level];
				if (clone && clone->req_transaction) {
					EXE_unwind(tdbb, clone);
					EXE_start(tdbb, clone, trans);
				}
			}
		}
	}
}


static void retain_context(thread_db* tdbb, jrd_tra* transaction, const bool commit)
{
/**************************************
 *
 *	r e t a i n _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	If 'commit' flag is true, commit the transaction,
 *	else rollback the transaction.
 *
 *	Commit/rollback a transaction while preserving the
 *	context, in particular, its snapshot. The
 *	trick is to insure that the transaction's
 *	oldest active is seen by other transactions
 *	simultaneously starting up.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* The new transaction needs to remember the 'commit-retained' transaction
   because it must see the operations of the 'commit-retained' transaction and
   its snapshot doesn't contain these operations. */

	if (commit) {
		SBM_SET(tdbb->getDefaultPool(), &transaction->tra_commit_sub_trans,
				transaction->tra_number);
	}

/* Create a new transaction lock, inheriting oldest active
   from transaction being committed. */

	WIN window(-1);
	SLONG new_number;
#ifdef SUPERSERVER_V2
	new_number = bump_transaction_id(tdbb, &window);
#else
	if (dbb->dbb_flags & DBB_read_only)
		new_number = ++dbb->dbb_next_transaction;
	else {
		const header_page* header = bump_transaction_id(tdbb, &window);
		new_number = header->hdr_next_transaction;
	}
#endif

	Lock* new_lock = 0;
	Lock* old_lock = transaction->tra_lock;
	if (old_lock) {
		new_lock =
			TRA_transaction_lock(tdbb, transaction);
		new_lock->lck_key.lck_long = new_number;
		new_lock->lck_data = transaction->tra_lock->lck_data;

		/* Support refresh range notification in ExpressLink */

#ifdef PC_ENGINE
		new_lock->lck_ast = downgrade_lock;
#endif

		if (!LCK_lock_non_blocking(tdbb, new_lock, LCK_write, LCK_WAIT)) {
#ifndef SUPERSERVER_V2
			if (!(dbb->dbb_flags & DBB_read_only))
				CCH_RELEASE(tdbb, &window);
#endif
			ERR_post(isc_lock_conflict, 0);
		}
	}

#ifndef SUPERSERVER_V2
	if (!(dbb->dbb_flags & DBB_read_only))
		CCH_RELEASE(tdbb, &window);
#endif

/* Update database notion of the youngest commit retaining
   transaction before committing the first transaction. This
   secures the original snapshot by insuring the oldest active
   is seen by other transactions. */

	const SLONG old_number = transaction->tra_number;
#ifdef VMS
	transaction->tra_number = new_number;
	compute_oldest_retaining(tdbb, transaction, true);
	transaction->tra_number = old_number;
#endif

	if (!(dbb->dbb_flags & DBB_read_only)) {
		/* Set the state on the inventory page */
		if (commit)
			TRA_set_state(tdbb, transaction, old_number, tra_committed);
		else
			TRA_set_state(tdbb, transaction, old_number, tra_dead);
	}
	transaction->tra_number = new_number;

/* Release transaction lock since it isn't needed
   anymore and the new one is already in place. */

	if (old_lock) {
		++transaction->tra_use_count;
		LCK_release(tdbb, old_lock);
		transaction->tra_lock = new_lock;
		--transaction->tra_use_count;
		delete old_lock;
	}

/* Perform any post commit work OR delete entries from deferred list */

	if (commit)
		DFW_perform_post_commit_work(transaction);
	else
		DFW_delete_deferred(transaction, -1);

	transaction->tra_flags &= ~(TRA_write | TRA_prepared);

/* We have to mimic a TRA_commit and a TRA_start while reusing the
   'transaction' control block: get rid of the transaction-level
   savepoint and possibly start a new transaction-level savepoint. */

	// Get rid of all user savepoints
	// Why we can do this in reverse order described in commit method
	while (transaction->tra_save_point && 
		transaction->tra_save_point->sav_flags & SAV_user) 
	{
		Savepoint* const next = transaction->tra_save_point->sav_next;
		transaction->tra_save_point->sav_next = NULL;
		VIO_verb_cleanup(tdbb, transaction);
		transaction->tra_save_point = next;				
	}
	
	if (transaction->tra_save_point) {
		if (!(transaction->tra_save_point->sav_flags & SAV_trans_level))
			BUGCHECK(287);		/* Too many savepoints */
		VIO_verb_cleanup(tdbb, transaction);	/* get rid of transaction savepoint */
		VIO_start_save_point(tdbb, transaction);	/* start new savepoint */
		transaction->tra_save_point->sav_flags |= SAV_trans_level;
	}

	if (transaction->tra_flags & TRA_precommitted) {
		if (!(dbb->dbb_flags & DBB_read_only))
		{
			transaction->tra_flags &= ~TRA_precommitted;
			TRA_set_state(tdbb, transaction, new_number, tra_committed);
			transaction->tra_flags |= TRA_precommitted;
		}

		TRA_precommited(tdbb, old_number, new_number);
	}
}


#ifdef SWEEP_THREAD
static void start_sweeper(thread_db* tdbb, Database* dbb)
{
/**************************************
 *
 *	s t a r t _ s w e e p e r
 *
 **************************************
 *
 * Functional description
 *	Start a thread to sweep the database.
 *
 **************************************/

	if ((dbb->dbb_flags & DBB_sweep_in_progress)
		|| (dbb->dbb_ast_flags & DBB_shutdown))
	{
		return; // false;
	}

	SET_TDBB(tdbb);

	// fill out the lock block
	Lock temp_lock;
	temp_lock.lck_dbb			= dbb;
	temp_lock.lck_type			= LCK_sweep;
	temp_lock.lck_owner_handle	= LCK_get_owner_handle(tdbb, temp_lock.lck_type);
	temp_lock.lck_parent		= dbb->dbb_lock;
	temp_lock.lck_length		= sizeof(SLONG);

	if (!LCK_lock(tdbb, &temp_lock, LCK_EX, LCK_NO_WAIT))
	{
		return; // false;
	}

	LCK_release(tdbb, &temp_lock);

	/* allocate space for the string and a null at the end */
	const char* pszFilename = tdbb->tdbb_attachment->att_filename.c_str();

	char* database = (char*)gds__alloc(strlen(pszFilename) + 1);

	if (!database)
	{
		ERR_log(0, 0, "cannot start sweep thread, Out of Memory");
		return; // false;
	}

	strcpy(database, pszFilename);
	if (gds__thread_start(sweep_database, database,
						  THREAD_medium, 0, 0))
	{
		gds__free(database);
		ERR_log(0, 0, "cannot start sweep thread");
	}

	return; // true;
}


static THREAD_ENTRY_DECLARE sweep_database(THREAD_ENTRY_PARAM database)
{
/**************************************
 *
 *	s w e e p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Sweep database.
 *
 **************************************/
	isc_db_handle db_handle = 0;
	Firebird::ClumpletWriter dpb(true, MAX_DPB_SIZE, isc_dpb_version1);

	const char* szAuthenticator = "sweeper";
	dpb.insertString(isc_dpb_user_name, 
		szAuthenticator, strlen(szAuthenticator));
	const char* szPassword = "none";
	dpb.insertString(isc_dpb_password, 
		szPassword, strlen(szPassword));
	dpb.insertByte(isc_dpb_sweep, isc_dpb_records);

	ISC_STATUS_ARRAY status_vector;

	// Temporary disable security for this thread to proceed with internal attachment
	JRD_thread_security_disable(true);

	isc_attach_database(status_vector, 0, (char*)database,
						&db_handle, dpb.getBufferLength(),
						reinterpret_cast<const char*>(dpb.getBuffer()));

	JRD_thread_security_disable(false);

	if (db_handle)
	{
		isc_detach_database(status_vector, &db_handle);
	}

	gds__free(database);
	return 0;
}
#endif


static void transaction_options(
								thread_db* tdbb,
								jrd_tra* transaction,
								const UCHAR* tpb, USHORT tpb_length)
{
/**************************************
 *
 *	t r a n s a c t i o n _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Process transaction options.
 *
 **************************************/
	SET_TDBB(tdbb);

	if (!tpb_length)
		return;

	const UCHAR* const end = tpb + tpb_length;

	if (*tpb != isc_tpb_version3 && *tpb != isc_tpb_version1)
		ERR_post(isc_bad_tpb_form, isc_arg_gds, isc_wrotpbver, 0);

	transaction->tra_lock_timeout = DEFAULT_LOCK_TIMEOUT;
	bool wait = true, lock_timeout = false;

	++tpb;

	while (tpb < end) {
		const USHORT op = *tpb++;
		switch (op) {
		case isc_tpb_consistency:
			transaction->tra_flags |= TRA_degree3;
			transaction->tra_flags &= ~TRA_read_committed;
			break;

		case isc_tpb_concurrency:
			transaction->tra_flags &= ~TRA_degree3;
			transaction->tra_flags &= ~TRA_read_committed;
			break;

		case isc_tpb_read_committed:
			transaction->tra_flags &= ~TRA_degree3;
			transaction->tra_flags |= TRA_read_committed;
			break;

		case isc_tpb_shared:
		case isc_tpb_protected:
		case isc_tpb_exclusive:
			break;

		case isc_tpb_wait:
			break;

		case isc_tpb_rec_version:
			transaction->tra_flags |= TRA_rec_version;
			break;

		case isc_tpb_no_rec_version:
			transaction->tra_flags &= ~TRA_rec_version;
			break;

		case isc_tpb_nowait:
			if (lock_timeout)
			{
				ERR_post(isc_bad_tpb_content, 0);
			}
			transaction->tra_lock_timeout = 0;
			wait = false;
			break;

		case isc_tpb_read:
			transaction->tra_flags |= TRA_readonly;
			break;

		case isc_tpb_write:
			transaction->tra_flags &= ~TRA_readonly;
			break;

		case isc_tpb_ignore_limbo:
			transaction->tra_flags |= TRA_ignore_limbo;
			break;

		case isc_tpb_no_auto_undo:
			transaction->tra_flags |= TRA_no_auto_undo;
			break;

		case isc_tpb_lock_write:
		case isc_tpb_lock_read:
			{
			SqlIdentifier name;
			UCHAR* p = reinterpret_cast<UCHAR*>(name);
			USHORT l = *tpb++;
			if (l) {
				if (l >= sizeof(name)) {
					TEXT text[BUFFER_TINY];
					USHORT flags = 0;
					gds__msg_lookup(0, DYN_MSG_FAC, 159, sizeof(text),
									text, &flags);
					/* msg 159: Name longer than database column size */
					ERR_post(isc_bad_tpb_content, isc_arg_gds, isc_random,
							 isc_arg_string, ERR_cstring(text), 0);
				}
				do {
					*p++ = *tpb++;
				} while (--l);
			}
			*p = 0;
			jrd_rel* relation = MET_lookup_relation(tdbb, name);
			if (!relation) {
				ERR_post(isc_bad_tpb_content,
						 isc_arg_gds, isc_relnotdef, isc_arg_string,
						 ERR_cstring(name), 0);
			}

			/* force a scan to read view information */
			MET_scan_relation(tdbb, relation);

			SCHAR lock_type = (op == isc_tpb_lock_read) ? LCK_none : LCK_SW;
			if (tpb < end) {
				if (*tpb == isc_tpb_shared)
					tpb++;
				else if (*tpb == isc_tpb_protected
						 || *tpb == isc_tpb_exclusive) 
				{
					tpb++;
					lock_type = (lock_type == LCK_SW) ? LCK_EX : LCK_PR;
				}
			}

			expand_view_lock(transaction, relation, lock_type);
			break;
			}

		case isc_tpb_verb_time:
		case isc_tpb_commit_time:
			{
			const USHORT l = *tpb++;
			tpb += l;
			break;
			}

		case isc_tpb_autocommit:
			transaction->tra_flags |= TRA_autocommit;
			break;

		case isc_tpb_restart_requests:
			transaction->tra_flags |= TRA_restart_requests;
			break;

		case isc_tpb_lock_timeout:
			{
			if (!wait)
			{
				ERR_post(isc_bad_tpb_content, 0);
			}
			const USHORT l = *tpb++;
			transaction->tra_lock_timeout = gds__vax_integer(tpb, l);
			tpb += l;
			lock_timeout = true;
			break;
			}

		default:
			ERR_post(isc_bad_tpb_form, 0);
		}
	}

/* If there aren't any relation locks to seize, we're done. */

	vec* vector = transaction->tra_relation_locks;
	if (!vector)
		return;

/* Try to seize all relation locks.
   If any can't be seized, release all and try again. */

	for (ULONG id = 0; id < vector->count(); id++) {
		Lock* lock = (Lock*) (*vector)[id];
		if (!lock)
			continue;
		USHORT level = lock->lck_logical;
		if (level == LCK_none
			|| LCK_lock_non_blocking(tdbb, lock, level,
									 transaction->getLockWait()))
		{
			continue;
		}
		for (USHORT l = 0; l < id; l++) {
			if ( (lock = (Lock*) (*vector)[l]) ) {
				level = lock->lck_logical;
				LCK_release(tdbb, lock);
				lock->lck_logical = level;
			}
		}
		id = 0;
		ERR_punt();
	}
}


#ifdef VMS
static void vms_convert(Lock* lock, SLONG* data, SCHAR type, bool wait)
{
/**************************************
 *
 *	v m s _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Comply with VMS protocol for lock I/O.
 *
 **************************************/
	lock_status lksb;
	lksb.lksb_lock_id = lock->lck_id;

	if (data && type < lock->lck_physical)
		lksb.lksb_value[0] = *data;

	SLONG flags = LCK$M_CONVERT;

	if (data)
		flags |= LCK$M_VALBLK;

	if (!wait)
		flags |= LCK$M_NOQUEUE;

	SLONG status = sys$enqw(EVENT_FLAG, lock_types[type], &lksb, flags,
						NULL, NULL, NULL,	// AST routine when granted
						NULL,		// ast_argument
						NULL,		// ast_routine
						NULL, NULL);

	if (!wait && status == SS$_NOTQUEUED)
		return; // false;

	if (!(status & 1) || !((status = lksb.lksb_status) & 1))
		ERR_post(isc_sys_request, isc_arg_string,
				 "sys$enqw (commit retaining lock)", isc_arg_vms, status, 0);

	if (data && type >= lock->lck_physical)
		*data = lksb.lksb_value[0];

	lock->lck_physical = lock->lck_logical = type;

	return; // true;
}
#endif

