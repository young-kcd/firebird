/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tpc.c
 *	DESCRIPTION:	TIP Cache for DBB
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
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/tpc.h"
#include "../jrd/ods.h"
#include "../jrd/tra.h"
#include "../jrd/lck.h"
#include "../jrd/pag.h"
#include "gen/iberror.h"
#include "../jrd/iberr.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/tpc_proto.h"
#include "../jrd/tra_proto.h"
#include <memory>

static TPC allocate_tpc(TDBB, ULONG);
static void cache_transactions(TDBB, TPC *, ULONG);
static int extend_cache(TDBB, SLONG);


int TPC_cache_state(TDBB tdbb, SLONG number)
{
/**************************************
 *
 *	T P C _ c a c h e _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the current state of a transaction in the cache.
 *
 **************************************/
	DBB dbb;
	TPC tip_cache;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (!(tip_cache = dbb->dbb_tip_cache)) {
		TPC_initialize_tpc(tdbb, number);
		tip_cache = dbb->dbb_tip_cache;
	}

	if (number && dbb->dbb_pc_transactions)
		if (TRA_precommited(tdbb, number, number))
			return tra_precommitted;

/* if the transaction is older than the oldest
   transaction in our tip cache, it must be committed */

	if (number < tip_cache->tpc_base)
		return tra_committed;

/* locate the specific TIP cache block for the transaction */

	for (; tip_cache; tip_cache = tip_cache->tpc_next)
		if (number < (SLONG) (tip_cache->tpc_base + dbb->dbb_pcontrol->pgc_tpt)) {
			return TRA_state(tip_cache->tpc_transactions,
							 tip_cache->tpc_base, number);
		}

/* Cover all possibilities by returning active */

	return tra_active;
}


void TPC_initialize_tpc(TDBB tdbb, SLONG number)
{
/**************************************
 *
 *	T P C _ i n i t i a l i z e _ t p c
 *
 **************************************
 *
 * Functional description
 *	At transaction startup, intialize the tip cache up to
 *	number.  This is used at TRA_start () time.
 *
 **************************************/
	DBB dbb;
	TPC *tip_cache_ptr, tip_cache;
	ULONG trans_per_tip;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (!(tip_cache = dbb->dbb_tip_cache)) {
		cache_transactions(tdbb, NULL, (ULONG) 0);
		return;
	}

/* If there is already a cache, extend it if required.
 * find the end of the linked list, and cache
 * all transactions from that point up to the
 * most recent transaction
 */

	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;

	for (tip_cache_ptr = &dbb->dbb_tip_cache; *tip_cache_ptr;
		 tip_cache_ptr = &(*tip_cache_ptr)->tpc_next)
		tip_cache = *tip_cache_ptr;

	if (number < (SLONG)(tip_cache->tpc_base + trans_per_tip))
		return;

	cache_transactions(tdbb, tip_cache_ptr,
					   tip_cache->tpc_base + trans_per_tip);
}


void TPC_set_state(TDBB tdbb, SLONG number, SSHORT state)
{
/**************************************
 *
 *	T P C _ s e t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Set the state of a particular transaction
 *	in the TIP cache.
 *
 **************************************/
	DBB dbb;
	TPC tip_cache;
	ULONG byte, trans_per_tip;
	SSHORT shift;
	UCHAR *address;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	byte = TRANS_OFFSET(number % trans_per_tip);
	shift = TRANS_SHIFT(number);

	for (tip_cache = dbb->dbb_tip_cache; tip_cache;
		 tip_cache = tip_cache->tpc_next) 
		if (number < (SLONG)(tip_cache->tpc_base + trans_per_tip)) {
			address = tip_cache->tpc_transactions + byte;
			*address &= ~(TRA_MASK << shift);
			*address |= state << shift;
			break;
		}

/* right now we don't set the state of a transaction on a page
   that has not already been cached -- this should probably be done */
}


int TPC_snapshot_state(TDBB tdbb, SLONG number)
{
/**************************************
 *
 *	T P C _ s n a p s h o t _ s t a t e
 *
 **************************************
 *
 * Functional description
 *	Get the current state of a transaction.
 *	Look at the TIP cache first, but if it
 *	is marked as still alive we must do some 
 *	further checking to see if it really is.
 *
 **************************************/

	SET_TDBB(tdbb);
	DBB dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	TPC tip_cache = dbb->dbb_tip_cache;
	if (!tip_cache) {
		cache_transactions(tdbb, NULL, (ULONG) 0);
		tip_cache = dbb->dbb_tip_cache;
	}

	if (number && dbb->dbb_pc_transactions) {
		if (TRA_precommited(tdbb, number, number)) {
			return tra_precommitted;
		}
	}

/* if the transaction is older than the oldest
   transaction in our tip cache, it must be committed */

	if (number < tip_cache->tpc_base) {
		return tra_committed;
	}

/* locate the specific TIP cache block for the transaction */

	for (; tip_cache; tip_cache = tip_cache->tpc_next)
	{
		if (number < (SLONG) (tip_cache->tpc_base + dbb->dbb_pcontrol->pgc_tpt))
		{
			const USHORT state =
				TRA_state(	tip_cache->tpc_transactions,
							tip_cache->tpc_base,
							number);

			/* committed or dead transactions always stay that 
			   way, so no need to check their current state */

			if (state == tra_committed || state == tra_dead) {
				return state;
			}

			// see if we can get a lock on the transaction; if we can't
			// then we know it is still active
			// We need to create this one in a pool since the
			// receiver of this (ptr) checks its type.
			// Please review this. This lock has _nothing_ to do in the
			// permamnent pool!
			std::auto_ptr<lck> temp_lock(FB_NEW_RPT(*dbb->dbb_permanent, 0) lck);

			//temp_lock.blk_type = type_lck;
			temp_lock->lck_dbb = dbb;
			temp_lock->lck_type = LCK_tra;
			temp_lock->lck_owner_handle =
				LCK_get_owner_handle(tdbb, temp_lock->lck_type);
			temp_lock->lck_parent = dbb->dbb_lock;
			temp_lock->lck_length = sizeof(SLONG);
			temp_lock->lck_key.lck_long = number;

			/* If we can't get a lock on the transaction, it must be active. */

			if (!LCK_lock_non_blocking(tdbb, temp_lock.get(), LCK_read, FALSE)) {
				INIT_STATUS(tdbb->tdbb_status_vector);
				return tra_active;
			}

			INIT_STATUS(tdbb->tdbb_status_vector);
			LCK_release(tdbb, temp_lock.get());

			/* as a last resort we must look at the TIP page to see
			   whether the transaction is committed or dead; to minimize 
			   having to do this again we will check the state of all 
			   other transactions on that page */

			return TRA_fetch_state(tdbb, number);
		}
	}

/* if the transaction has been started since we
   last looked, extend the cache upward */

	return extend_cache(tdbb, number);
}


void TPC_update_cache(TDBB tdbb, TIP tip_page, SLONG sequence)
{
/**************************************
 *
 *	T P C _ u p d a t e _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	A TIP page has been fetched into memory,
 *	so we should take the opportunity to update
 *	the TIP cache with the state of all transactions
 *	on that page.
 *
 **************************************/
	DBB dbb;
	TPC tip_cache;
	SLONG first_trans, trans_per_tip;
	USHORT l;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	first_trans = sequence * trans_per_tip;

/* while we're in the area we can check to see if there are 
   any tip cache pages we can release--this is cheaper and 
   easier than finding out when a TIP page is dropped */

	while ( (tip_cache = dbb->dbb_tip_cache) )
		if (dbb->dbb_oldest_transaction >=
			tip_cache->tpc_base + trans_per_tip) {
			dbb->dbb_tip_cache = tip_cache->tpc_next;
			delete tip_cache;
		}
		else
			break;

/* find the appropriate page in the TIP cache and assign all transaction
   bits -- it's not worth figuring out which ones are actually used */

	for (; tip_cache; tip_cache = tip_cache->tpc_next)
		if (first_trans == tip_cache->tpc_base) {
			l = TRANS_OFFSET(trans_per_tip);
			MOVE_FAST(tip_page->tip_transactions, tip_cache->tpc_transactions,
					  l);
			break;
		}

/* note that a potential optimization here would be to extend the cache
   if the fetched page is not already in cache; it would involve a little
   extra effort to make sure the pages remained in order, and since there
   is a fetched page passed to us we can't fetch any other pages in this
   routine, so I just decided not to do it - djb */
}


static TPC allocate_tpc(TDBB tdbb, ULONG base)
{
/**************************************
 *
 *	a l l o c a t e _ t p c
 *
 **************************************
 *
 * Functional description
 *	Create a tip cache block to hold the state
 *	of all transactions on one page.
 *
 **************************************/
	DBB dbb;
	TPC tip_cache;
	ULONG trans_per_tip;

	dbb = tdbb->tdbb_database;
	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;

/* allocate a TIP cache block with enough room for 
   all desired transactions */

	tip_cache = FB_NEW_RPT(*dbb->dbb_permanent, trans_per_tip / 4) tpc();
	tip_cache->tpc_base = base;

	return tip_cache;
}


static void cache_transactions(TDBB tdbb, TPC * tip_cache_ptr, ULONG oldest)
{
/**************************************
 *
 *	c a c h e _ t r a n s a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Cache the state of all the transactions since
 *	the last time this routine was called, or since
 *	the oldest interesting transaction.
 *
 **************************************/
	DBB dbb;
	WIN window;
	HDR header;
	ULONG top, trans_per_tip, base;

	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* check the header page for the oldest and 
   newest transaction numbers */

#ifdef SUPERSERVER_V2
	top = dbb->dbb_next_transaction;
	oldest = MAX(oldest, dbb->dbb_oldest_transaction);
#else
	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
	top = header->hdr_next_transaction;
	oldest = MAX(oldest, (ULONG) header->hdr_oldest_transaction);
	CCH_RELEASE(tdbb, &window);
#endif

/* allocate tpc blocks to hold all transaction states --
   assign one tpc block per page to simplify cache maintenance */

	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;
	if (!tip_cache_ptr)
		tip_cache_ptr = &dbb->dbb_tip_cache;

	for (base = oldest - oldest % trans_per_tip; base <= top;
		 base += trans_per_tip) {
		*tip_cache_ptr = allocate_tpc(tdbb, base);
		tip_cache_ptr = &(*tip_cache_ptr)->tpc_next;
	}

/* now get the inventory of all transactions, which will
   automatically fill in the tip cache pages */

	TRA_get_inventory(tdbb, NULL, oldest, top);
}


static int extend_cache(TDBB tdbb, SLONG number)
{
/**************************************
 *
 *	e x t e n d _ c a c h e
 *
 **************************************
 *
 * Functional description
 *	Extend the transaction inventory page
 *	cache to include at least all transactions 
 *	up to the passed transaction, and return 
 *	the state of the passed transaction.
 *
 **************************************/
	DBB dbb;
	TPC *tip_cache_ptr, tip_cache;
	ULONG trans_per_tip;

	dbb = tdbb->tdbb_database;
	trans_per_tip = dbb->dbb_pcontrol->pgc_tpt;

/* find the end of the linked list, and cache
   all transactions from that point up to the
   most recent transaction */

	for (tip_cache_ptr = &dbb->dbb_tip_cache; *tip_cache_ptr;
		 tip_cache_ptr = &(*tip_cache_ptr)->tpc_next)
		tip_cache = *tip_cache_ptr;
	cache_transactions(tdbb, tip_cache_ptr,
					   tip_cache->tpc_base + trans_per_tip);

/* find the right block for this transaction and return the state */

	for (tip_cache = dbb->dbb_tip_cache; tip_cache;
		 tip_cache =
		 tip_cache->tpc_next) 
		if (number < (SLONG) (tip_cache->tpc_base + trans_per_tip)) 
			 return TRA_state(tip_cache->tpc_transactions, tip_cache->tpc_base,
				number);

/* we should never get to this point, but if we do the
   safest thing to do is return active */

	return tra_active;
}
