/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cch.cpp
 *	DESCRIPTION:	Disk cache manager
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */
#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../jrd/jrd.h"
#include "../jrd/que.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/os/pio.h"
#include "../jrd/cch.h"
#include "gen/iberror.h"
#include "../jrd/lls.h"
#include "../jrd/req.h"
#include "../jrd/sdw.h"
#include "../jrd/tra.h"
#include "../jrd/sbm.h"
#include "../jrd/iberr.h"
#include "../jrd/rse.h"
#include "../jrd/btr.h"
#include "../jrd/btn.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/sdw_proto.h"
#include "../jrd/shut_proto.h"
#include "../jrd/thd.h"
#include "../jrd/thread_proto.h"
#include "../jrd/tra_proto.h"
#include "../common/config/config.h"
#include "../jrd/jrd_time.h"

using namespace Jrd;
using namespace Ods;

/* In the superserver mode, no page locks are acquired through the lock manager.
   Instead, a latching mechanism is used.  So the calls to lock subsystem for
   database pages in the original code should not be made, lest they should cause
   any undesirable side-effects.  The following defines help us achieve that.  */

#ifdef SUPERSERVER
#define	CACHE_WRITER
#define PAGE_LATCHING
#endif

#ifdef SUPERSERVER_V2
#define CACHE_READER
#endif

#ifdef PAGE_LATCHING
#define PAGE_LOCK(lock, lock_type, wait)
#define PAGE_LOCK_RELEASE(lock)  
#define PAGE_LOCK_ASSERT(lock)
#define PAGE_LOCK_OPT(lock, lock_type, wait)
#define PAGE_LOCK_RE_POST(lock) 
#define PAGE_OVERHEAD	(sizeof (bcb_repeat) + sizeof(BufferDesc) + \
			 (int) dbb->dbb_page_size)
#else
#define PAGE_LOCK(lock, lock_type, wait)	LCK_lock (tdbb, lock, lock_type, wait)
#define PAGE_LOCK_RELEASE(lock)			LCK_release (tdbb, lock)
#define PAGE_LOCK_ASSERT(lock)			LCK_assert (tdbb, lock)
#define PAGE_LOCK_OPT(lock, lock_type, wait)	LCK_lock_opt (tdbb, lock, lock_type, wait)
#define PAGE_LOCK_RE_POST(lock)			LCK_re_post (lock)
#define PAGE_OVERHEAD	(sizeof (bcb_repeat) + sizeof(BufferDesc) + \
			 sizeof (Lock) + (int) dbb->dbb_page_size)
#endif


static BufferDesc* alloc_bdb(thread_db*, BufferControl*, UCHAR **);
#ifndef PAGE_LATCHING
static int blocking_ast_bdb(void*);
#endif
static void btc_flush(thread_db*, SLONG, const bool, ISC_STATUS*);
static void btc_insert_balanced(Database*, BufferDesc*);
static bool btc_insert_balance(BufferDesc**, bool, SCHAR);
static void btc_insert_unbalanced(Database*, BufferDesc*);
static void btc_remove_balanced(BufferDesc*);
static bool btc_remove_balance(BufferDesc**, bool, SCHAR);
static void btc_remove_unbalanced(BufferDesc*);
static void cache_bugcheck(int);
#ifdef CACHE_READER
static THREAD_ENTRY_DECLARE cache_reader(THREAD_ENTRY_PARAM);
#endif
#ifdef CACHE_WRITER
static THREAD_ENTRY_DECLARE cache_writer(THREAD_ENTRY_PARAM);
#endif
static void check_precedence(thread_db*, WIN *, SLONG);
static void clear_precedence(Database*, BufferDesc*);
static BufferDesc* dealloc_bdb(BufferDesc*);
#ifndef PAGE_LATCHING
static void down_grade(thread_db*, BufferDesc*);
#endif
static void expand_buffers(thread_db*, ULONG);
static BufferDesc* get_buffer(thread_db*, SLONG, LATCH, SSHORT);
static SSHORT latch_bdb(thread_db*, LATCH, BufferDesc*, SLONG, SSHORT);
static SSHORT lock_buffer(thread_db*, BufferDesc*, SSHORT, SCHAR);
static ULONG memory_init(thread_db*, BufferControl*, ULONG);
static void page_validation_error(thread_db*, win*, SSHORT);
#ifdef CACHE_READER
static void prefetch_epilogue(Prefetch*, ISC_STATUS *);
static void prefetch_init(Prefetch*, thread_db*);
static void prefetch_io(Prefetch*, ISC_STATUS *);
static void prefetch_prologue(Prefetch*, SLONG *);
#endif
static SSHORT related(const BufferDesc*, const BufferDesc*, SSHORT);
static void release_bdb(thread_db*, BufferDesc*, const bool, const bool, const bool);
static bool writeable(const BufferDesc*);
static int write_buffer(thread_db*, BufferDesc*, SLONG, const bool, ISC_STATUS*, const bool);
static bool write_page(thread_db*, BufferDesc*, const bool, ISC_STATUS*, const bool);
static void unmark(thread_db*, WIN *);
static void update_write_direction(thread_db*, BufferDesc*);

// comment this macro out to revert back to the old tree
#define BALANCED_DIRTY_PAGE_TREE

#ifdef BALANCED_DIRTY_PAGE_TREE
#define btc_insert btc_insert_balanced
#define btc_remove btc_remove_balanced
#else
#define btc_insert btc_insert_unbalanced
#define btc_remove btc_remove_unbalanced
#endif

const int BTREE_STACK_SIZE = 40;

const SLONG MIN_BUFFER_SEGMENT = 65536;

/* Given pointer a field in the block, find the block */

#define BLOCK(fld_ptr, type, fld) (type)((SCHAR*) fld_ptr - OFFSET (type, fld))

#ifdef MULTI_THREAD
#ifndef VMS
#define INTERLOCK_CACHE
#endif
#endif

//
//#define BCB_MUTEX_ACQUIRE
//#define BCB_MUTEX_RELEASE
//
//#define PRE_MUTEX_ACQUIRE 
//#define PRE_MUTEX_RELEASE 
//
//#define BTC_MUTEX_ACQUIRE 
//#define BTC_MUTEX_RELEASE 
//
//#define LATCH_MUTEX_ACQUIRE
//#define LATCH_MUTEX_RELEASE
//

const int JOURNAL_PAGE		= -1;
const int SHADOW_PAGE		= -2;
const int FREE_PAGE			= -3;
const int CHECKPOINT_PAGE	= -4;
const int MIN_PAGE_NUMBER	= -5;

const int PRE_SEARCH_LIMIT	= 100;
const int PRE_EXISTS		= -1;
const int PRE_UNKNOWN		= -2;

const int DUMMY_CHECKSUM	= 12345;

bool set_write_direction(Database* dbb, BufferDesc* bdb, SSHORT direction)
{
#ifdef SUPERSERVER
	NBAK_TRACE(("set_write_direction page=%d old=%d new=%d", bdb->bdb_page,
		bdb->bdb_write_direction, direction));
	if (bdb->bdb_write_direction == BDB_write_normal ||
		bdb->bdb_write_direction == BDB_write_both) 
	{
		if (direction != BDB_write_normal && direction != BDB_write_both)		
			dbb->backup_manager->release_sw_database_lock();
	}
	else {
		if (direction == BDB_write_normal || direction == BDB_write_both)		
			dbb->backup_manager->get_sw_database_lock(true);
	}
	bdb->bdb_write_direction = direction;
#else
	LCK_ast_inhibit();
	switch (bdb->bdb_write_direction) {
	case BDB_write_normal:
	case BDB_write_both:
		switch (direction) {
		case BDB_write_diff:
			dbb->backup_manager->increment_diff_use_count();
			dbb->backup_manager->release_sw_database_lock();
			break;
		case BDB_write_undefined:
			dbb->backup_manager->release_sw_database_lock();
			break;
		}
		break;
	case BDB_write_diff:
		switch (direction) {
		case BDB_write_normal:
		case BDB_write_both:
			dbb->backup_manager->decrement_diff_use_count();
			bdb->bdb_write_direction = direction;
			// We ask this function to enable signals
			if (!dbb->backup_manager->get_sw_database_lock(true)) {
				bdb->bdb_write_direction = BDB_write_undefined;
				return false;
			}
			return true;
		case BDB_write_undefined:
			dbb->backup_manager->decrement_diff_use_count();
		}
		break;
	case BDB_write_undefined:
		switch (direction) {
		case BDB_write_diff:
			dbb->backup_manager->increment_diff_use_count();
			break;			
		case BDB_write_normal:
		case BDB_write_both:
			bdb->bdb_write_direction = direction;
			// We ask this function to enable signals
			if (!dbb->backup_manager->get_sw_database_lock(true)) {
				bdb->bdb_write_direction = BDB_write_undefined;
				return false;
			}
			return true;
		}
		break;
	}
	bdb->bdb_write_direction = direction;
	LCK_ast_enable();
#endif
	return true;
}

void CCH_flush_database(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ f l u s h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Flush all buffers coming from database file.
 *	May be called from AST
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	BufferControl* bcb = dbb->dbb_bcb;

#ifdef SUPERSERVER
	// This is called on architectures with shared buffer cache (like SuperServer)

	// Redirect pages to the difference file

	// Need to reconsider this protection for possible deadlocks when MT-safety is implemented
//	BCB_MUTEX_ACQUIRE; 
	for (ULONG i = 0; (bcb = dbb->dbb_bcb) && i < bcb->bcb_count; i++) {
		BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (bdb->bdb_write_direction != BDB_write_normal &&
			bdb->bdb_write_direction != BDB_write_both)
		{
			continue;
		}
		NBAK_TRACE(("Redirect page=%d use=%d flags=%d", bdb->bdb_page, bdb->bdb_use_count, bdb->bdb_flags));
		update_write_direction(tdbb, bdb);
	}
//	BCB_MUTEX_RELEASE;
#else		
	/* Do some fancy footwork to make sure that pages are
	   not removed from the btc tree at AST level.  Then
	   restore the flag to whatever it was before. */

	const bool keep_pages = bcb->bcb_flags & BCB_keep_pages;
	dbb->dbb_bcb->bcb_flags |= BCB_keep_pages;
	for (ULONG i = 0; (bcb = dbb->dbb_bcb) && i < bcb->bcb_count; i++) {
		BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
		if (!(bdb->bdb_flags & (BDB_dirty | BDB_db_dirty)) ||
			bdb->bdb_write_direction == BDB_write_diff)
		{
			continue;
		}
		down_grade(tdbb, bdb);
	}
	if (!keep_pages) {
		bcb->bcb_flags &= ~BCB_keep_pages;
	}
#endif
}

USHORT CCH_checksum(BufferDesc* bdb)
{
/**************************************
 *
 *	C C H _ c h e c k s u m
 *
 **************************************
 *
 * Functional description
 *	Compute the checksum of a page.
 *
 **************************************/
#ifdef NO_CHECKSUM
	return DUMMY_CHECKSUM;
#else
	Database* dbb = bdb->bdb_dbb;
#ifdef WIN_NT
/* ODS_VERSION8 for NT was shipped before page checksums
   were disabled on other platforms. Continue to compute
   checksums for ODS_VERSION8 databases but eliminate them
   for ODS_VERSION9 databases. The following code can be
   deleted when development on ODS_VERSION10 begins and
   NO_CHECKSUM is defined for all platforms. */

	if (dbb->dbb_ods_version >= ODS_VERSION9) {
		return DUMMY_CHECKSUM;
	}
#endif
	pag* page = bdb->bdb_buffer;

	const ULONG* const end = (ULONG *) ((SCHAR *) page + dbb->dbb_page_size);
	const USHORT old_checksum = page->pag_checksum;
	page->pag_checksum = 0;
	const ULONG* p = (ULONG *) page;
	ULONG checksum = 0;

	do {
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
		checksum += *p++;
	} while (p < end);

	page->pag_checksum = old_checksum;

	if (checksum) {
		return (USHORT) checksum;
	}

/* If the page is all zeros, return an artificial checksum */

	for (p = (ULONG *) page; p < end;) {
		if (*p++)
			return (USHORT) checksum;
	}

/* Page is all zeros -- invent a checksum */

	return 12345;
#endif
}


int CCH_down_grade_dbb(void* ast_object)
{
/**************************************
 *
 *	C C H _ d o w n _ g r a d e _ d b b
 *
 **************************************
 *
 * Functional description
 *	Down grade the lock on the database in response to a blocking
 *	AST.
 *
 **************************************/
	Database* dbb = static_cast<Database*>(ast_object);

/* Ignore the request if the database or lock block does not appear
   to be valid . */
	Lock* lock;
	if ((MemoryPool::blk_type(dbb) != type_dbb) ||
		!(lock = dbb->dbb_lock) ||
		(MemoryPool::blk_type(lock) != type_lck) || !(lock->lck_id))
	{
		return 0;
	}

/* Since this routine will be called asynchronously, we must establish
   a thread context. */
	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	ISC_STATUS_ARRAY ast_status;
	tdbb->tdbb_database = dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	tdbb->tdbb_status_vector = ast_status;

	dbb->dbb_ast_flags |= DBB_blocking;

/* Database shutdown will release the database lock; just return. */

	if (SHUT_blocking_ast(dbb)) {
		dbb->dbb_ast_flags &= ~DBB_blocking;
		JRD_restore_thread_data();
		return 0;
	}

/*
if (dbb->dbb_use_count)
    {
	JRD_restore_thread_data();
    return;
    }
*/

/* If we are already shared, there is nothing more we can do.
   If any case, the other guy probably wants exclusive access,
   and we can't give it anyway */

	if ((lock->lck_logical == LCK_SW) || (lock->lck_logical == LCK_SR)) {
		JRD_restore_thread_data();
		return 0;
	}

	if (dbb->dbb_flags & DBB_bugcheck) {
		LCK_convert(tdbb, lock, LCK_SW, LCK_WAIT);
		dbb->dbb_ast_flags &= ~DBB_blocking;
		JRD_restore_thread_data();
		return 0;
	}

/* If we are supposed to be exclusive, stay exclusive */

	if ((dbb->dbb_flags & DBB_exclusive) || (dbb->dbb_ast_flags & DBB_shutdown_single)) {
		JRD_restore_thread_data();
		return 0;
	}

/* Assert any page locks that have been requested, but not asserted */

	ISC_ast_enter();

	dbb->dbb_ast_flags |= DBB_assert_locks;
	BufferControl* bcb = dbb->dbb_bcb;
	if (bcb) {
		if (bcb->bcb_count) {
            const bcb_repeat* tail = bcb->bcb_rpt;
			for (const bcb_repeat* const end = tail + bcb->bcb_count;
				tail < end; tail++)
			{
				PAGE_LOCK_ASSERT(tail->bcb_bdb->bdb_lock);
			}
		}
	}

/* Down grade the lock on the database itself */

	if (lock->lck_physical == LCK_EX) {
		LCK_convert(tdbb, lock, LCK_PW, LCK_WAIT);	/* This lets waiting cache manager in first */
	}
	else {
		LCK_convert(tdbb, lock, LCK_SW, LCK_WAIT);
	}

	dbb->dbb_ast_flags &= ~DBB_blocking;
	ISC_ast_exit();

/* Restore the prior thread context */

	JRD_restore_thread_data();
	return 0;
}


bool CCH_exclusive(thread_db* tdbb, USHORT level, SSHORT wait_flag)
{
/**************************************
 *
 *	C C H _ e x c l u s i v e
 *
 **************************************
 *
 * Functional description
 *	Get exclusive access to a database.  If we get it, return true.
 *	If the wait flag is FALSE, and we can't get it, give up and
 *	return false. There are two levels of database exclusivity: LCK_PW
 *	guarantees there are  no normal users in the database while LCK_EX
 *	additionally guarantes background database processes like the
 *	shared cache manager have detached.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

#ifdef SUPERSERVER
	if (!CCH_exclusive_attachment(tdbb, level, wait_flag)) {
		return false;
	}
#endif
	Lock* lock = dbb->dbb_lock;
	if (!lock) {
		return false;
	}

	dbb->dbb_flags |= DBB_exclusive;

	switch (level) {
	case LCK_PW:
		if ((lock->lck_physical >= LCK_PW)
			|| LCK_convert(tdbb, lock, LCK_PW, wait_flag))
		{
			return true;
		}
		break;

	case LCK_EX:
		if (lock->lck_physical == LCK_EX ||
			LCK_convert(tdbb, lock, LCK_EX, wait_flag))
		{
			return true;
		}
		break;

	default:
		break;
	}

/* If we are supposed to wait (presumably patiently),
   but can't get the lock, generate an error */

	if (wait_flag == LCK_WAIT) {
		ERR_post(isc_deadlock, 0);
	}

	dbb->dbb_flags &= ~DBB_exclusive;

	return false;
}


bool CCH_exclusive_attachment(thread_db* tdbb, USHORT level, SSHORT wait_flag)
{
/**************************************
 *
 *	C C H _ e x c l u s i v e _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Get exclusive access to a database. If we get it, return true.
 *	If the wait flag is FALSE, and we can't get it, give up and
 *	return false.
 *
 **************************************/
const int CCH_EXCLUSIVE_RETRY_INTERVAL	=1;	/* retry interval in seconds */

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;
	if (attachment->att_flags & ATT_exclusive) {
		return true;
	}

	attachment->att_flags |=
		(level == LCK_none) ? ATT_attach_pending : ATT_exclusive_pending;

	const SLONG timeout =
		(SLONG) (wait_flag < 0) ? -wait_flag :
			((wait_flag == LCK_WAIT) ? 1L << 30 : CCH_EXCLUSIVE_RETRY_INTERVAL);

/* If requesting exclusive database access, then re-position attachment as the
   youngest so that pending attachments may pass. */

	if (level != LCK_none) {
		for (Attachment** ptr = &dbb->dbb_attachments; *ptr; ptr = &(*ptr)->att_next) {
			if (*ptr == attachment) {
				*ptr = attachment->att_next;
				break;
			}
		}
		attachment->att_next = dbb->dbb_attachments;
		dbb->dbb_attachments = attachment;
	}

	for (SLONG remaining = timeout; remaining > 0;
		 remaining -= CCH_EXCLUSIVE_RETRY_INTERVAL)
	{
		if (tdbb->tdbb_attachment->att_flags & ATT_shutdown) {
			break;
		}

		bool found = false;
		for (attachment = tdbb->tdbb_attachment->att_next; attachment;
			 attachment = attachment->att_next)
		{
			if (attachment->att_flags & ATT_shutdown) {
				continue;
			}

			if (level == LCK_none) {	/* Wait for other attachments requesting exclusive access */
				if (attachment->att_flags &
					(ATT_exclusive | ATT_exclusive_pending))
				{
					found = true;
					break;
				}
				// Forbid multiple attachments in single-user maintenance mode
				if (attachment != tdbb->tdbb_attachment &&
					(dbb->dbb_ast_flags & DBB_shutdown_single) )
				{
					found = true;
					break;
				}
			}
			else {				/* Requesting exclusive database access */

				found = true;
				if (attachment->att_flags & ATT_exclusive_pending) {
					tdbb->tdbb_attachment->att_flags &=
						~ATT_exclusive_pending;
					if (wait_flag == LCK_WAIT) {
						ERR_post(isc_deadlock, 0);
					}
					else {
						return false;
					}
				}
				break;
			}
		}

		if (!found) {
			tdbb->tdbb_attachment->att_flags &=
				~(ATT_exclusive_pending | ATT_attach_pending);
			if (level != LCK_none) {
				tdbb->tdbb_attachment->att_flags |= ATT_exclusive;
			}
			return true;
		}

		/* Our thread needs to sleep for CCH_EXCLUSIVE_RETRY_INTERVAL seconds. */

		if (remaining > CCH_EXCLUSIVE_RETRY_INTERVAL)
		{
			THREAD_EXIT();
			THREAD_SLEEP(CCH_EXCLUSIVE_RETRY_INTERVAL * 1000);
			THREAD_ENTER();
		}

#ifdef CANCEL_OPERATION
		if (tdbb->tdbb_attachment->att_flags & ATT_cancel_raise) {
			if (JRD_reschedule(tdbb, 0, false)) {
				tdbb->tdbb_attachment->att_flags &=
					~(ATT_exclusive_pending | ATT_attach_pending);
				ERR_punt();
			}
		}
#endif
	}

	tdbb->tdbb_attachment->att_flags &=
		~(ATT_exclusive_pending | ATT_attach_pending);
	return false;
}


void CCH_expand(thread_db* tdbb, ULONG number)
{
/**************************************
 *
 *	C C H _ e x p a n d
 *
 **************************************
 *
 * Functional description
 *	Expand the cache to at least a given number of buffers.  If
 *	it's already that big, don't do anything.
 *
 **************************************/
	SET_TDBB(tdbb);

//	BCB_MUTEX_ACQUIRE;
	expand_buffers(tdbb, number);
//	BCB_MUTEX_RELEASE;
}


pag* CCH_fake(thread_db* tdbb, WIN * window, SSHORT latch_wait)
{
/**************************************
 *
 *	C C H _ f a k e
 *
 **************************************
 *
 * Functional description
 *	Fake a fetch to a page.  Rather than reading it, however,
 *	zero it in memory.  This is used when allocating a new page.
 *
 * input
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				or an IO would be necessary, then give
 *				up and return 0.
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	pag pointer if successful.
 *	NULL pointer if timeout occurred (only possible if latch_wait <> 1).
 *	NULL pointer if latch_wait=0 and the faked page would have to be
 *			before reuse.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* if there has been a shadow added recently, go out and
   find it before we grant any more write locks */

	if (dbb->dbb_ast_flags & DBB_get_shadows) {
		SDW_get_shadows();
	}

	BufferDesc* bdb = get_buffer(tdbb, window->win_page, LATCH_exclusive, latch_wait);
	if (!bdb) {
		return NULL;			/* latch timeout occurred */
	}

/* If a dirty orphaned page is being reused - better write it first
   to clear current precedences and checkpoint state. This would also
   update the bcb_free_pages field appropriately. */

	if (bdb->bdb_flags & (BDB_dirty | BDB_db_dirty)) {
		/* If the caller didn't want to wait at all, then
		   return 'try to fake an other page' to the caller. */

		if (!latch_wait) {
			release_bdb(tdbb, bdb, false, false, false);
			return NULL;
		}

		if (!write_buffer
			(tdbb, bdb, bdb->bdb_page, true, tdbb->tdbb_status_vector, true))
		{
			CCH_unwind(tdbb, true);
		}
	}
	else if (QUE_NOT_EMPTY(bdb->bdb_lower)) {
		/* Clear residual precedence left over from AST-level I/O. */
		clear_precedence(dbb, bdb);
	}

	bdb->bdb_flags = (BDB_writer | BDB_faked);
	bdb->bdb_scan_count = 0;

	lock_buffer(tdbb, bdb, LCK_WAIT, pag_undefined);

	MOVE_CLEAR(bdb->bdb_buffer, (SLONG) dbb->dbb_page_size);
	window->win_buffer = bdb->bdb_buffer;
	window->win_expanded_buffer = NULL;
	window->win_bdb = bdb;
	window->win_flags = 0;
	CCH_MARK(tdbb, window);

	return bdb->bdb_buffer;
}


pag* CCH_fetch(
		thread_db* tdbb,
		WIN* window,
		USHORT lock_type,
		SCHAR page_type,
		SSHORT checksum,
		SSHORT latch_wait,
		bool read_shadow)
{
/**************************************
 *
 *	C C H _ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch a specific page.  If it's already in cache,
 *	so much the better.
 *
 * input
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				give up and return 0.
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	PAG if successful.
 *	NULL pointer if timeout occurred (only possible if latch_wait <> 1).
 *
 **************************************/
	SET_TDBB(tdbb);

/* FETCH_LOCK will return 0, 1, -1 or -2 */

	const SSHORT fetch_lock_return =
		CCH_FETCH_LOCK(tdbb, window, lock_type, LCK_WAIT, latch_wait,
					   page_type);

	if (fetch_lock_return == 1) {
		CCH_FETCH_PAGE(tdbb, window, checksum, read_shadow);	/* must read page from disk */
	}
	else if (fetch_lock_return == -2 || fetch_lock_return == -1) {
		return NULL;			/* latch or lock timeout */
	}

	BufferDesc* bdb = window->win_bdb;

/* If a page was read or prefetched on behalf of a large scan
   then load the window scan count into the buffer descriptor.
   This buffer scan count is decremented by releasing a buffer
   with CCH_RELEASE_TAIL.

   Otherwise zero the buffer scan count to prevent the buffer
   from being queued to the LRU tail. */

	if (window->win_flags & WIN_large_scan) {
		if (fetch_lock_return == 1 ||
			bdb->bdb_flags & BDB_prefetch || bdb->bdb_scan_count < 0)
		{
			bdb->bdb_scan_count = window->win_scans;
		}
	}
	else if (window->win_flags & WIN_garbage_collector) {
		if (fetch_lock_return == 1) {
			bdb->bdb_scan_count = -1;
		}
		if (bdb->bdb_flags & BDB_garbage_collect) {
			window->win_flags |= WIN_garbage_collect;
		}
	}
	else if (window->win_flags & WIN_secondary) {
		if (fetch_lock_return == 1) {
			bdb->bdb_scan_count = -1;
		}
	}
	else {
		bdb->bdb_scan_count = 0;
		if (bdb->bdb_flags & BDB_garbage_collect) {			
			bdb->bdb_flags &= ~BDB_garbage_collect;
		}
	}

/* Validate the fetched page matches the expected type */

	if (bdb->bdb_buffer->pag_type != page_type &&
		page_type != pag_undefined)
	{
		page_validation_error(tdbb, window, page_type);
	}

	return window->win_buffer;
}


SSHORT CCH_fetch_lock(
		thread_db* tdbb,
		WIN * window,
		USHORT lock_type,
		SSHORT wait,
		SSHORT latch_wait,
		SCHAR page_type)
{
/**************************************
 *
 *	C C H _ f e t c h _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Fetch a lock for a specific page.
 *	Return TRUE if the page needs to be
 *	read and FALSE if not. If a timeout
 *	was passed (wait < 0) and the lock
 *	could not be granted return wait.
 *
 * input
 *
 *	wait: LCK_WAIT = TRUE = 1	=> Wait as long a necessary to get the lock.
 *	      LCK_NO_WAIT = FALSE = 0	=> If the lock can't be acquired immediately,
 *						give up and return -1.
 *	      <negative number>		=> Lock timeout interval in seconds.
 *
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				give up and return -2.
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	0:	fetch & lock were successful, page doesn't need to be read.
 *	1:	fetch & lock were successful, page needs to be read from disk.
 *	-1:	lock timed out, fetch failed.
 *	-2:	latch timed out, fetch failed, lock not attempted.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* if there has been a shadow added recently, go out and
   find it before we grant any more write locks */

	if (dbb->dbb_ast_flags & DBB_get_shadows) {
		SDW_get_shadows();
	}

/* Look for the page in the cache. */

	BufferDesc* bdb = get_buffer(tdbb, window->win_page,
					 ((lock_type >= LCK_write) ? LATCH_exclusive :
					  LATCH_shared), latch_wait);
	if ((latch_wait != 1) && (bdb == 0)) {
		return -2;				/* latch timeout */
	}

	if (lock_type >= LCK_write) {
		bdb->bdb_flags |= BDB_writer;
	}

/* the expanded index buffer is only good when the page is
   fetched for read; if it is ever fetched for write, it must
   be discarded */

	if (bdb->bdb_expanded_buffer && (lock_type > LCK_read)) {
		delete bdb->bdb_expanded_buffer;
		bdb->bdb_expanded_buffer = NULL;
	}

	window->win_bdb = bdb;
	window->win_buffer = bdb->bdb_buffer;
	window->win_expanded_buffer = bdb->bdb_expanded_buffer;

/* lock_buffer returns 0 or 1 or -1. */
	return lock_buffer(tdbb, bdb, wait, page_type);
}


void CCH_fetch_page(
					thread_db* tdbb,
					WIN * window,
					SSHORT compute_checksum, bool read_shadow)
{
/**************************************
 *
 *	C C H _ f e t c h _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Fetch a specific page.  If it's already in cache,
 *	so much the better.  When "compute_checksum" is 1, compute
 * 	the checksum of the page.  When it is 2, compute
 *	the checksum only when the page type is nonzero.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	BufferDesc* bdb = window->win_bdb;

	ISC_STATUS* status = tdbb->tdbb_status_vector;

	pag* page = bdb->bdb_buffer;
	bdb->bdb_incarnation = ++dbb->dbb_page_incarnation;

	AST_CHECK();
	++dbb->dbb_reads;
#ifdef SUPERSERVER
	THREAD_EXIT();
#endif
	page = bdb->bdb_buffer;
	jrd_file* file = dbb->dbb_file;
	SSHORT retryCount = 0;

/* We will read a page, and if there is an I/O error we will try to
   use the shadow file, and try reading again, for a maximum of
   3 tries, before it gives up.

   The read_shadow flag is set to false only in the call to
   FETCH_NO_SHADOW, which is only called from validate
   code.

   read_shadow = false -> IF an I/O error occurs give up (exit
   the loop, clean up, and return). So the caller,
   validate in most cases, can know about it and attempt
   to remedy the situation.

   read_shadow = true -> IF an I/O error occurs attempt
   to rollover to the shadow file.  If the I/O error is
   persistant (more than 3 times) error out of the routine by
   calling CCH_unwind, and eventually punting out. */
   
	if (!dbb->backup_manager->lock_state(false)) 
	{
		PAGE_LOCK_RELEASE(bdb->bdb_lock);
		CCH_unwind(tdbb, true);
	}
	const int bak_state = dbb->backup_manager->get_state();
	ULONG diff_page = 0;
	if (bak_state == nbak_state_stalled || bak_state == nbak_state_merge) {
		if (!dbb->backup_manager->lock_alloc(false)) 
		{
			PAGE_LOCK_RELEASE(bdb->bdb_lock);
			dbb->backup_manager->unlock_state();
			CCH_unwind(tdbb, true);
		}
		diff_page = dbb->backup_manager->get_page_index(bdb->bdb_page);
		dbb->backup_manager->unlock_alloc();
		NBAK_TRACE(("Reading page %d, state=%d, diff page=%d", bdb->bdb_page, bak_state, diff_page));
	}

	if (bak_state == nbak_state_normal || 
		(bak_state == nbak_state_stalled && !diff_page) ||
		// In merge mode, if we are reading past beyond old end of file and page is in .delta file
		// then we maintain actual page in difference file. Always read it from there.
		(bak_state == nbak_state_merge && 
			(!diff_page || (bdb->bdb_page < dbb->backup_manager->get_backup_pages())))
	   ) 
	{
		NBAK_TRACE(("Reading page %d, state=%d, diff page=%d from DISK",
			bdb->bdb_page, bak_state, diff_page));
		// Read page from disk as normal
		while (!PIO_read(file, bdb, page, status)) {
			if (!read_shadow) {
				break;
			}
#ifdef SUPERSERVER
			THREAD_ENTER();
#endif
			if (!CCH_rollover_to_shadow(dbb, file, false)) {
				PAGE_LOCK_RELEASE(bdb->bdb_lock);
				dbb->backup_manager->unlock_state();
				CCH_unwind(tdbb, true);
			}
			if (file != dbb->dbb_file) {
				file = dbb->dbb_file;
			}
			else {
				if (retryCount++ == 3) {
					fprintf(stderr,
							   "IO error loop Unwind to avoid a hang\n");
					PAGE_LOCK_RELEASE(bdb->bdb_lock);
					dbb->backup_manager->unlock_state();
					CCH_unwind(tdbb, true);
				}
			}
#ifdef SUPERSERVER
			THREAD_EXIT();
#endif
		}
	}
	
	if (diff_page && (
		bak_state == nbak_state_stalled || 
	    (bak_state == nbak_state_merge && 
		 (bdb->bdb_page >= dbb->backup_manager->get_backup_pages() ||
		  page->pag_scn < dbb->backup_manager->get_current_scn())
		)))
	{		
		NBAK_TRACE(("Reading page %d, state=%d, diff page=%d from DIFFERENCE", 
			bdb->bdb_page, bak_state, diff_page));
		if (!dbb->backup_manager->read_difference(diff_page, page)) {
			PAGE_LOCK_RELEASE(bdb->bdb_lock);
			dbb->backup_manager->unlock_state();
			CCH_unwind(tdbb, true);
		}
		if (page->pag_type == pag_undefined) {
			// Page was marked as allocated inside the difference file, but not really used
			// this is very rare, but possible case (after certain errors). 
			// Read (or re-read) page from database
			NBAK_TRACE(("Re-reading page %d, state=%d, diff page=%d from DISK", 
				bdb->bdb_page, bak_state, diff_page));
			while (!PIO_read(file, bdb, page, status)) {
				if (!read_shadow) {
					break;
				}
#ifdef SUPERSERVER
				THREAD_ENTER();
#endif
				if (!CCH_rollover_to_shadow(dbb, file, false)) {
					PAGE_LOCK_RELEASE(bdb->bdb_lock);
					dbb->backup_manager->unlock_state();
					CCH_unwind(tdbb, true);
				}
				if (file != dbb->dbb_file) {
					file = dbb->dbb_file;
				}
				else {
					if (retryCount++ == 3) {
						fprintf(stderr,
								   "IO error loop Unwind to avoid a hang\n");
						PAGE_LOCK_RELEASE(bdb->bdb_lock);
						dbb->backup_manager->unlock_state();
						CCH_unwind(tdbb, true);
					}
				}
#ifdef SUPERSERVER
				THREAD_EXIT();
#endif
			}
		}
	}
	
	dbb->backup_manager->unlock_state();

#ifndef NO_CHECKSUM
	if (((compute_checksum == 1)
		 || ((compute_checksum == 2) && page->pag_type))
		&& ((page->pag_checksum != CCH_checksum(bdb))
			&& !(dbb->dbb_flags & DBB_damaged)))
	{
#ifdef SUPERSERVER
		THREAD_ENTER();
#endif
		IBERR_build_status(tdbb->tdbb_status_vector,
						   isc_db_corrupt,
						   isc_arg_string, "",
						   isc_arg_gds, isc_bad_checksum,
						   isc_arg_gds, isc_badpage,
						   isc_arg_number, (SLONG) bdb->bdb_page, 0);
		/* We should invalidate this bad buffer. */

		PAGE_LOCK_RELEASE(bdb->bdb_lock);
		CCH_unwind(tdbb, true);
	}
#endif /* NO_CHECKSUM */

#ifdef SUPERSERVER
	THREAD_ENTER();
#endif
	AST_CHECK();

	bdb->bdb_flags &= ~(BDB_not_valid | BDB_read_pending);
	window->win_buffer = bdb->bdb_buffer;
}


void CCH_fini(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Shut down buffer operation.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	bool flush_error = false;
	
	// CVC: Patching a conversion error FB1->FB2 with crude logic
	for (int i = 0; i < 2; ++i)
	{

		try {

	/* If we've been initialized, either flush buffers
	   or release locks, depending on where we've been
	   bug-checked; as a defensive programming measure,
	   make sure that the buffers were actually allocated */

		bcb_repeat* tail;
		BufferControl* bcb = dbb->dbb_bcb;
		if (bcb && (tail = bcb->bcb_rpt) && (tail->bcb_bdb)) {
			if (dbb->dbb_flags & DBB_bugcheck || flush_error) {
				for (const bcb_repeat* const end = bcb->bcb_rpt + bcb->bcb_count;
					tail < end; tail++)
				{
					BufferDesc* bdb = tail->bcb_bdb;
					if (bdb->bdb_expanded_buffer) {
						delete bdb->bdb_expanded_buffer;
						bdb->bdb_expanded_buffer = NULL;
					}
					PAGE_LOCK_RELEASE(bdb->bdb_lock);
				}
			}
			else {
				CCH_flush(tdbb, FLUSH_FINI, (SLONG) 0);
			}
		}


#ifdef CACHE_READER

	/* Shutdown the dedicated cache reader for this database. */

		if ((bcb = dbb->dbb_bcb) && (bcb->bcb_flags & BCB_cache_reader)) {
			event_t* event = dbb->dbb_reader_event;
			bcb->bcb_flags &= ~BCB_cache_reader;
			ISC_event_post(event);
			SLONG count = ISC_event_clear(event);
			THREAD_EXIT();
			ISC_event_wait(1, &event, &count, 0, NULL, 0);
			THREAD_ENTER();
			/* Now dispose off the cache reader associated semaphore */
			ISC_event_fini(event);
		}
#endif

#ifdef CACHE_WRITER

	/* Shutdown the dedicated cache writer for this database. */

		if ((bcb = dbb->dbb_bcb) && (bcb->bcb_flags & BCB_cache_writer)) {
			event_t* event = dbb->dbb_writer_event_fini;
			/* initialize initialization event */
			ISC_event_init(event, 0, 0);
			SLONG count = ISC_event_clear(event);

			bcb->bcb_flags &= ~BCB_cache_writer;
			ISC_event_post(dbb->dbb_writer_event); /* Wake up running thread */
			THREAD_EXIT();
			ISC_event_wait(1, &event, &count, 0, NULL, 0);
			THREAD_ENTER();
			/* Cleanup initialization event */
			ISC_event_fini(event);
		}
#endif

	/* close the database file and all associated shadow files */

		PIO_close(dbb->dbb_file);
		SDW_close();

		if ( (bcb = dbb->dbb_bcb) ) 
		{
			while (bcb->bcb_memory.hasData()) 
			{
				gds__free(bcb->bcb_memory.pop());
			}
#ifdef CACHE_WRITER
			/* Dispose off any associated latching semaphores */
			while (QUE_NOT_EMPTY(bcb->bcb_free_lwt)) 
			{
				QUE que_inst = bcb->bcb_free_lwt.que_forward;
				QUE_DELETE((*que_inst));
				LatchWait* lwt = (LatchWait*) BLOCK(que_inst, LatchWait*, lwt_waiters);
				ISC_event_fini(&lwt->lwt_event);
			}
#endif
		}

		}	// try
		catch (const std::exception& ex)
		{
			Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
			if (!flush_error) {
				flush_error = true;
			}
			else {
				ERR_punt();
			}
		}
		
		if (!flush_error) { // wasn't set in the catch => no failure, just exit
			break;
		}

	} // for

}


void CCH_flush(thread_db* tdbb, USHORT flush_flag, SLONG tra_number)
{
/**************************************
 *
 *	C C H _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Flush all buffers.  If the release flag is set,
 *	release all locks.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	BufferControl* bcb = dbb->dbb_bcb;
	ISC_STATUS* status = tdbb->tdbb_status_vector;

/* note that some of the code for btc_flush()
   replicates code in the for loop
   to minimize call overhead -- changes should
   be made in both places */

	if (flush_flag & (FLUSH_TRAN | FLUSH_SYSTEM)) {
		const SLONG transaction_mask =
			(tra_number) ? 1L << (tra_number & (BITS_PER_LONG - 1)) : 0;
		bool sys_only = false;
		if (!transaction_mask && (flush_flag & FLUSH_SYSTEM)) {
			sys_only = true;
		}

#ifdef SUPERSERVER_V2
//		if (!dbb->dbb_wal && A && B) becomes
//		if (true && A && B) then finally (A && B)
		if (!(dbb->dbb_flags & DBB_force_write) && transaction_mask) {
			dbb->dbb_flush_cycle |= transaction_mask;
			if (!(bcb->bcb_flags & BCB_writer_active))
				ISC_event_post(dbb->dbb_writer_event);
		}
		else
#endif
			btc_flush(tdbb, transaction_mask, sys_only, status);
	}
	else {
		const bool all_flag = (flush_flag & FLUSH_ALL) != 0;
		const bool release_flag = (flush_flag & FLUSH_RLSE) != 0;
		const bool write_thru = release_flag;
		const bool sweep_flag = (flush_flag & FLUSH_SWEEP) != 0;
		LATCH latch = (release_flag) ? LATCH_exclusive : LATCH_none;

		for (ULONG i = 0; (bcb = dbb->dbb_bcb) && i < bcb->bcb_count; i++) {
			BufferDesc* bdb = bcb->bcb_rpt[i].bcb_bdb;
			if (!release_flag
				&& !(bdb->bdb_flags & (BDB_dirty | BDB_db_dirty)))
			{
				continue;
			}
			if (latch == LATCH_exclusive
				&& latch_bdb(tdbb, latch, bdb, bdb->bdb_page, 1) == -1)
			{
				cache_bugcheck(302);	/* msg 302 unexpected page change */
			}
			if (latch == LATCH_exclusive && bdb->bdb_use_count > 1)
				cache_bugcheck(210);	/* msg 210 page in use during flush */
#ifdef SUPERSERVER
			if (bdb->bdb_flags & BDB_db_dirty) {
				if (all_flag
					|| (sweep_flag
						&& (!bdb->bdb_parent && bdb != bcb->bcb_btree)))
				{
					if (!write_buffer
						(tdbb, bdb, bdb->bdb_page, write_thru, status, true))
					{
						CCH_unwind(tdbb, true);
					}
				}
			}
#else
			if (bdb->bdb_flags & BDB_dirty) {
				if (!write_buffer
					(tdbb, bdb, bdb->bdb_page, false, status, true))
				{
					CCH_unwind(tdbb, true);
				}
			}
#endif
			if (release_flag)
			{
				PAGE_LOCK_RELEASE(bdb->bdb_lock);
			}
			if (latch == LATCH_exclusive)
				release_bdb(tdbb, bdb, false, false, false);
		}
	}

	// 
	// Check if flush needed
	//
	const int max_unflushed_writes = Config::getMaxUnflushedWrites();
	const time_t max_unflushed_write_time = Config::getMaxUnflushedWriteTime();
	bool max_num = (max_unflushed_writes >= 0);
	bool max_time = (max_unflushed_write_time >= 0);

	bool doFlush = false;

	if (!(dbb->dbb_file->fil_flags & FIL_force_write) && (max_num || max_time))
	{
		const time_t now = time(0);

		THD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_flush_count);
		
		// If this is the first commit set last_flushed_write to now
		if (!dbb->last_flushed_write)
		{
			dbb->last_flushed_write = now;
		}

		// test max_num condition and max_time condition
		max_num = max_num 
			&& (dbb->unflushed_writes == max_unflushed_writes);
		max_time = max_time
			&& (now - dbb->last_flushed_write > max_unflushed_write_time);

		if (max_num || max_time)
		{
			doFlush = true;
			dbb->unflushed_writes = 0;
			dbb->last_flushed_write = now;
		}
		else
		{
			dbb->unflushed_writes++;
		}

		THD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_flush_count);
	}

	if (doFlush)
	{
		PIO_flush(dbb->dbb_file);
		if (dbb->dbb_shadow)
		{
			PIO_flush(dbb->dbb_shadow->sdw_file);
		}
	}

/* take the opportunity when we know there are no pages
   in cache to check that the shadow(s) have not been
   scheduled for shutdown or deletion */

	SDW_check();
}


bool CCH_free_page(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ f r e e _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Check if the cache is below its free pages
 *	threshold and write a page on the LRU tail.
 *
 **************************************/

/* Called by VIO/garbage_collector() when it is idle to
   help quench the thirst for free pages. */

	Database* dbb = tdbb->tdbb_database;
	BufferControl* bcb = dbb->dbb_bcb;

	if (dbb->dbb_flags & DBB_read_only) {
		return false;
	}

	BufferDesc* bdb;
	if (bcb->bcb_flags & BCB_free_pending &&
		(bdb = get_buffer(tdbb, FREE_PAGE, LATCH_none, 1)))
	{
		if (!write_buffer
			(tdbb, bdb, bdb->bdb_page, true, tdbb->tdbb_status_vector, true))
		{
			CCH_unwind(tdbb, false);
		}
		else
			return true;
	}

	return false;
}


SLONG CCH_get_incarnation(WIN * window)
{
/**************************************
 *
 *	C C H _ g e t _ i n c a r n a t i o n
 *
 **************************************
 *
 * Functional description
 *	Get page incarnation associated with buffer.
 *
 **************************************/
	return window->win_bdb->bdb_incarnation;
}


pag* CCH_handoff(
		thread_db*	tdbb,
		WIN* window,
		SLONG	page,
		SSHORT	lock,
		SCHAR	page_type,
		SSHORT	latch_wait,
		SSHORT	release_tail)
{
/**************************************
 *
 *	C C H _ h a n d o f f
 *
 **************************************
 *
 * Functional description
 *	Follow a pointer handing off the lock.  Fetch the new page
 *	before retiring the old page lock.
 *
 * input
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				give up and return 0.
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	PAG if successful.
 *	0 if a latch timeout occurred (only possible if latch_wait <> 1).
 *		The latch on the fetched page is downgraded to shared.
 *		The fetched page is unmarked.
 *
 **************************************/

/* The update, if there was one, of the input page is complete.
   The cache buffer can be 'unmarked'.  It is important to
   unmark before CCH_unwind is (might be) called. */

	SET_TDBB(tdbb);

	unmark(tdbb, window);

/* If the 'from-page' and 'to-page' of the handoff are the
   same and the latch requested is shared then downgrade it. */

	if ((window->win_page == page) && (lock == LCK_read)) {
		release_bdb(tdbb, window->win_bdb, false, true, false);
		return window->win_buffer;
	}

	WIN temp = *window;
	window->win_page = page;
	const SSHORT must_read =
		CCH_FETCH_LOCK(tdbb, window, lock, LCK_WAIT, latch_wait, page_type);

/* Latch or lock timeout, return failure. */

	if (must_read == -2 || must_read == -1) {
		*window = temp;
		CCH_RELEASE(tdbb, window);
		return NULL;
	}

	if (release_tail)
		CCH_RELEASE_TAIL(tdbb, &temp);
	else
		CCH_RELEASE(tdbb, &temp);

	if (must_read) {
		CCH_FETCH_PAGE(tdbb, window, TRUE, true);
	}

	BufferDesc* bdb = window->win_bdb;

/* If a page was read or prefetched on behalf of a large scan
   then load the window scan count into the buffer descriptor.
   This buffer scan count is decremented by releasing a buffer
   with CCH_RELEASE_TAIL.

   Otherwise zero the buffer scan count to prevent the buffer
   from being queued to the LRU tail. */

	if (window->win_flags & WIN_large_scan) {
		if (must_read == 1 ||
			bdb->bdb_flags & BDB_prefetch || bdb->bdb_scan_count < 0)
		{
			bdb->bdb_scan_count = window->win_scans;
		}
	}
	else if (window->win_flags & WIN_garbage_collector) {
		if (must_read == 1) {
			bdb->bdb_scan_count = -1;
		}
		if (bdb->bdb_flags & BDB_garbage_collect) {
			window->win_flags |= WIN_garbage_collect;
		}
	}
	else if (window->win_flags & WIN_secondary) {
		if (must_read == 1) {
			bdb->bdb_scan_count = -1;
		}
	}
	else {
		bdb->bdb_scan_count = 0;
		if (bdb->bdb_flags & BDB_garbage_collect) {
			bdb->bdb_flags &= ~BDB_garbage_collect;
		}
	}

/* Validate the fetched page matches the expected type */

	if (bdb->bdb_buffer->pag_type != page_type &&
		page_type != pag_undefined)
	{
		page_validation_error(tdbb, window, page_type);
	}

	return window->win_buffer;
}


void CCH_init(thread_db* tdbb, ULONG number)
{
/**************************************
 *
 *	C C H _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the cache.  Allocate buffers control block,
 *	buffer descriptors, and actual buffers.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* Check for database-specific page buffers */

	if (dbb->dbb_page_buffers) {
		number = dbb->dbb_page_buffers;
	}

/* Enforce page buffer cache constraints */

	if (number < MIN_PAGE_BUFFERS) {
		number = MIN_PAGE_BUFFERS;
	}
	if (number > MAX_PAGE_BUFFERS) {
		number = MAX_PAGE_BUFFERS;
	}

	SLONG count = number;

/* Allocate and initialize buffers control block */
	BufferControl* bcb = 0;
	while (!bcb) {
		try {
			bcb = FB_NEW_RPT(*dbb->dbb_bufferpool, number) BufferControl(*dbb->dbb_bufferpool);
		} catch(const std::exception& ex) {
			Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
			/* If the buffer control block can't be allocated, memory is
			very low. Recalculate the number of buffers to account for
			page buffer overhead and reduce that number by a 25% fudge
			factor. */

			number = (sizeof(bcb_repeat) * number) / PAGE_OVERHEAD;
			number -= number >> 2;

			if (number < MIN_PAGE_BUFFERS) {
				ERR_post(isc_cache_too_small, 0);
			}
		}
	}

	dbb->dbb_bcb = bcb;
	QUE_INIT(bcb->bcb_in_use);
	QUE_INIT(bcb->bcb_empty);
	QUE_INIT(bcb->bcb_free_lwt);

/* initialization of memory is system-specific */

	bcb->bcb_count = memory_init(tdbb, bcb, number);
	bcb->bcb_free_minimum = (SSHORT) MIN(bcb->bcb_count / 4, 128);

	if (bcb->bcb_count < MIN_PAGE_BUFFERS) {
		ERR_post(isc_cache_too_small, 0);
	}

/* Log if requested number of page buffers could not be allocated. */

	if (count != (SLONG) bcb->bcb_count) {
		gds__log
			("Database: %s\n\tAllocated %ld page buffers of %ld requested",
			 tdbb->tdbb_attachment->att_filename.c_str(), bcb->bcb_count, count);
	}

	if (dbb->dbb_lock->lck_logical != LCK_EX) {
		dbb->dbb_ast_flags |= DBB_assert_locks;
	}

#ifdef CACHE_READER
	event_t* event = dbb->dbb_reader_event;
	ISC_event_init(event, 0, 0);
	count = ISC_event_clear(event);
	if (gds__thread_start(cache_reader, dbb, THREAD_high, 0, 0))
	{
		ERR_bugcheck_msg("cannot start thread");
	}

	THREAD_EXIT();
	ISC_event_wait(1, &event, &count, 5 * 1000000, NULL, 0);
	THREAD_ENTER();
#endif

#ifdef CACHE_WRITER
	if (!(dbb->dbb_flags & DBB_read_only)) {
		event_t* event = dbb->dbb_writer_event_init;
		/* Initialize initialization event */
		ISC_event_init(event, 0, 0);
		count = ISC_event_clear(event);

		if (gds__thread_start(cache_writer, dbb,
			THREAD_high, 0, 0))
		{
			ERR_bugcheck_msg("cannot start thread");
		}
		THREAD_EXIT();
		ISC_event_wait(1, &event, &count, 5 * 1000000, NULL, 0);
		THREAD_ENTER();
		/* Clean up initialization event */
		ISC_event_fini(event);
	}
#endif
}


void CCH_mark(thread_db* tdbb, WIN * window, USHORT mark_system)
{
/**************************************
 *
 *	C C H _ m a r k
 *
 **************************************
 *
 * Functional description
 *	Mark a window as dirty.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	dbb->dbb_marks++;
	BufferControl* bcb = dbb->dbb_bcb;
	BufferDesc* bdb = window->win_bdb;
	BLKCHK(bdb, type_bdb);

/* A LATCH_mark is needed before the BufferDesc can be marked.
   This prevents a write while the page is being modified. */

	if (latch_bdb(tdbb, LATCH_mark, bdb, bdb->bdb_page, 1) == -1) {
		cache_bugcheck(302);	/* msg 302 unexpected page change */
	}

	bdb->bdb_incarnation = ++dbb->dbb_page_incarnation;

	if (!(bdb->bdb_flags & BDB_writer)) {
		BUGCHECK(208);			/* msg 208 page not accessed for write */
	}

/* mark the dirty bit vector for this specific transaction,
   if it exists; otherwise mark that the system transaction
   has updated this page */

	SLONG number;
	jrd_tra* transaction = tdbb->tdbb_transaction;
	if (transaction && (number = transaction->tra_number)) {
		if (!(tdbb->tdbb_flags & TDBB_sweeper)) {
			const ULONG trans_bucket = number & (BITS_PER_LONG - 1);
			bdb->bdb_transactions |= (1L << trans_bucket);
			if (number > bdb->bdb_mark_transaction) {
				bdb->bdb_mark_transaction = number;
			}
		}
	}
	else {
		bdb->bdb_flags |= BDB_system_dirty;
	}

	if (mark_system) {
		bdb->bdb_flags |= BDB_system_dirty;
	}

	if (!(tdbb->tdbb_flags & TDBB_sweeper) ||
		bdb->bdb_flags & BDB_system_dirty)
	{
		if (!bdb->bdb_parent && bdb != bcb->bcb_btree) {
			btc_insert(dbb, bdb);
		}
	}

#ifdef SUPERSERVER
	bdb->bdb_flags |= BDB_db_dirty;
#endif

	bdb->bdb_flags |= (BDB_dirty | BDB_marked);
	update_write_direction(tdbb, bdb);
}


void CCH_mark_must_write(thread_db* tdbb, WIN * window)
{
/**************************************
 *
 *	C C H _ m a r k _ m u s t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Mark a window as dirty and must_write.
 *	This will prevent the page from being
 *	inserted in the dirty binary tree when
 *	the intention is to write the page
 *	immediately any way.
 *
 **************************************/
	SET_TDBB(tdbb);
	BufferDesc* bdb = window->win_bdb;
	BLKCHK(bdb, type_bdb);

	if (!(bdb->bdb_flags & BDB_writer)) {
		BUGCHECK(208);			/* msg 208 page not accessed for write */
	}

	bdb->bdb_flags |= (BDB_dirty | BDB_must_write);

	CCH_MARK(tdbb, window);
	update_write_direction(tdbb, bdb);
}


void CCH_must_write(WIN * window)
{
/**************************************
 *
 *	C C H _ m u s t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Mark a window as "must write".
 *
 **************************************/
	BufferDesc* bdb = window->win_bdb;
	BLKCHK(bdb, type_bdb);

	if (!(bdb->bdb_flags & BDB_marked)) {
		BUGCHECK(208);			/* msg 208 page not accessed for write */
	}

	bdb->bdb_flags |= (BDB_dirty | BDB_must_write);
	update_write_direction(JRD_get_thread_data(), bdb);
}



Lock* CCH_page_lock(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ p a g e _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a page-type lock.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	Lock* lock = FB_NEW_RPT(*dbb->dbb_bufferpool, sizeof(SLONG)) Lock;
	lock->lck_type = LCK_bdb;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_length = sizeof(SLONG);

	lock->lck_dbb = dbb;
	lock->lck_parent = dbb->dbb_lock;

	return lock;
}


void CCH_precedence(thread_db* tdbb, WIN * window, SLONG page)
{
/**************************************
 *
 *	C C H _ p r e c e d e n c e
 *
 **************************************
 *
 * Functional description
 *	Given a window accessed for write and a page number,
 *	establish a precedence relationship such that the
 *	specified page will always be written before the page
 *	associated with the window.
 *
 *	If the "page number" is negative, it is really a transaction
 *	id.  In this case, the precedence relationship is to the
 *	database header page from which the transaction id was
 *	obtained.  If the header page has been written since the
 *	transaction id was assigned, no precedence relationship
 *	is required.
 *
 **************************************/
/* If the page is zero, the caller isn't really serious */

	if (page == 0) {
		return;
	}

	check_precedence(tdbb, window, page);
}


void CCH_prefetch(thread_db* tdbb, SLONG * pages, SSHORT count)
{
/**************************************
 *
 *	C C H _ p r e f e t c h
 *
 **************************************
 *
 * Functional description
 *	Given a vector of pages, set corresponding bits
 *	in global prefetch bitmap. Initiate an asynchronous
 *	I/O and get the cache reader reading in our behalf
 *	as well.
 *
 **************************************/
#ifdef CACHE_READER
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	BufferControl* bcb = dbb->dbb_bcb;

	if (!count || !(bcb->bcb_flags & BCB_cache_reader))	{
		/* Caller isn't really serious. */
		return;
	}

/* Switch default pool to permanent pool for setting bits in
   prefetch bitmap. */
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_bufferpool);

/* The global prefetch bitmap is the key to the I/O coalescense
   mechanism which dovetails all thread prefetch requests to
   minimize sequential I/O requests. It also enables multipage
   I/O by implicitly sorting page vector requests. */

	SLONG first_page = 0;
	for (const SLONG* const end = pages + count; pages < end;) {
		const SLONG page = *pages++;
		if (page) {
			SBM_set(tdbb, &bcb->bcb_prefetch, page);
			if (!first_page)
				first_page = page;
		}
	}

/* Not likely that the caller's page vector was
   empty but check anyway. */

	if (first_page) {
		prf prefetch;

		prefetch_init(&prefetch, tdbb);
		prefetch_prologue(&prefetch, &first_page);
		prefetch_io(&prefetch, tdbb->tdbb_status_vector);
		prefetch_epilogue(&prefetch, tdbb->tdbb_status_vector);
	}
#endif
}


bool CCH_prefetch_pages(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ p r e f e t c h _ p a g e s
 *
 **************************************
 *
 * Functional description
 *	Check the prefetch bitmap for a set
 *	of pages and read them into the cache.
 *
 **************************************/

/* Place holder to be implemented when predictive prefetch is
   enabled. This is called by VIO/garbage_collector() when it
   is idle to help satisfy the prefetch demand. */

	return false;
}


void invalidate_and_release_buffer(thread_db* tdbb, BufferDesc* bdb)
{
	// This function should be called before difference processing is done.
	// So there should be no need to no need to release difference locks though
	Database* dbb = tdbb->tdbb_database;
	bdb->bdb_flags |= BDB_not_valid;
	bdb->bdb_flags &= ~BDB_dirty;
	set_write_direction(dbb, bdb, BDB_write_undefined);
	TRA_invalidate(dbb, bdb->bdb_transactions);
	bdb->bdb_transactions = 0;
	release_bdb(tdbb, bdb, false, false, false);
}


void update_write_direction(thread_db* tdbb, BufferDesc* bdb)
{
	Database* dbb = tdbb->tdbb_database;
	// Determine location of the page in difference file and write destination
	// so BufferDesc AST handlers and write_page routine can safely use this information
	if (!dbb->backup_manager->lock_state(true)) {
		invalidate_and_release_buffer(tdbb, bdb);
		CCH_unwind(tdbb, true);
	}
#ifndef SUPERSERVER
	bdb->bdb_diff_generation = dbb->backup_manager->get_current_generation();
#endif
	if (bdb->bdb_page != HEADER_PAGE)
	{
		// SCN of header page is adjusted in nbak.cpp
		bdb->bdb_buffer->pag_scn = dbb->backup_manager->get_current_scn(); // Set SCN for the page
	}

	SSHORT write_direction;
	const int backup_state = dbb->backup_manager->get_state();
	switch (backup_state) {
	case nbak_state_normal:
		write_direction = BDB_write_normal; 
		break;
	case nbak_state_stalled:
		write_direction = BDB_write_diff;
		break;
	case nbak_state_merge:
		if (tdbb->tdbb_flags & TDBB_backup_merge || 
			bdb->bdb_page < dbb->backup_manager->get_backup_pages())
		{
			write_direction = BDB_write_normal;
		}
		else {
			write_direction = BDB_write_both;
		}
		break;
	}
	switch (write_direction) {
	case BDB_write_diff:
		if (!dbb->backup_manager->lock_alloc(true)) {
			dbb->backup_manager->unlock_state();
			invalidate_and_release_buffer(tdbb, bdb);
			CCH_unwind(tdbb, true);
		}
		bdb->bdb_difference_page = dbb->backup_manager->get_page_index(bdb->bdb_page);
		dbb->backup_manager->unlock_alloc();
		if (!bdb->bdb_difference_page) {
			if (!dbb->backup_manager->lock_alloc_write(true)) {
				dbb->backup_manager->unlock_state();
				invalidate_and_release_buffer(tdbb, bdb);
				CCH_unwind(tdbb, true);
			}
			bdb->bdb_difference_page = dbb->backup_manager->allocate_difference_page(bdb->bdb_page);
			dbb->backup_manager->unlock_alloc_write();
			if (!bdb->bdb_difference_page) {
				dbb->backup_manager->unlock_state();
				invalidate_and_release_buffer(tdbb, bdb);
				CCH_unwind(tdbb, true);
			}
			NBAK_TRACE(("Allocate difference page %d for database page %d", 
				bdb->bdb_difference_page, bdb->bdb_page));
		}
		else {
			NBAK_TRACE(("Map existing difference page %d to database page %d", 
				bdb->bdb_difference_page, bdb->bdb_page));
		}
		break;
	case BDB_write_both:
		if (!dbb->backup_manager->lock_alloc(true)) {
			dbb->backup_manager->unlock_state();
			invalidate_and_release_buffer(tdbb, bdb);
			CCH_unwind(tdbb, true);
		}
		bdb->bdb_difference_page = dbb->backup_manager->get_page_index(bdb->bdb_page);
		dbb->backup_manager->unlock_alloc();
		if (bdb->bdb_difference_page) {
			NBAK_TRACE(("Map existing difference page %d to database page %d (write_both)", 
				bdb->bdb_difference_page, bdb->bdb_page));
		}
		else {
			// This may really happen. Database file can grow while in merge mode
			write_direction = BDB_write_normal;
		}
		break;
	}
	if (!set_write_direction(dbb, bdb, write_direction)) {
		dbb->backup_manager->unlock_state();
		invalidate_and_release_buffer(tdbb, bdb);
		CCH_unwind(tdbb, true);
	}
	dbb->backup_manager->unlock_state();
}

void CCH_release(thread_db* tdbb, WIN * window, bool release_tail)
{
/**************************************
 *
 *	C C H _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a window. If the release_tail
 *	flag is true then make the buffer
 *	least-recently-used.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	BufferDesc* bdb = window->win_bdb;
	BLKCHK(bdb, type_bdb);

/* if an expanded buffer has been created, retain it
   for possible future use */

	bdb->bdb_expanded_buffer = window->win_expanded_buffer;
	window->win_expanded_buffer = NULL;

/* A large sequential scan has requested that the garbage
   collector garbage collect. Mark the buffer so that the
   page isn't released to the LRU tail before the garbage
   collector can process the page. */

	if (window->win_flags & WIN_large_scan &&
		window->win_flags & WIN_garbage_collect)
	{
		bdb->bdb_flags |= BDB_garbage_collect;
		window->win_flags &= ~WIN_garbage_collect;
	}

	if (bdb->bdb_use_count == 1)
	{
		bool marked = bdb->bdb_flags & BDB_marked;
		bdb->bdb_flags &= ~(BDB_writer | BDB_marked | BDB_faked);		
		if (marked)
		{
			release_bdb(tdbb, bdb, false, false, true);
		}
		if (bdb->bdb_flags & BDB_must_write)
		{
			/* Downgrade exclusive latch to shared to allow concurrent share access
			   to page during I/O. */

			release_bdb(tdbb, bdb, false, true, false);
			if (!write_buffer(	tdbb,
								bdb,
								bdb->bdb_page,
								false,
								tdbb->tdbb_status_vector,
								true))
			{
				btc_insert(dbb, bdb);	/* Don't lose track of must_write */
				CCH_unwind(tdbb, true);
			}
		}
#ifndef PAGE_LATCHING
		if (bdb->bdb_flags & BDB_no_blocking_ast)
		{
			if (bdb->bdb_flags & (BDB_db_dirty | BDB_dirty))
			{
				if (!write_buffer(	tdbb,
									bdb,
									bdb->bdb_page,
									false,
									tdbb->tdbb_status_vector,
									true))
				{
					/* Reassert blocking AST after write failure with dummy lock convert
					   to same level. This will re-enable blocking AST notification. */

					LCK_convert_opt(tdbb, bdb->bdb_lock,
									bdb->bdb_lock->lck_logical);
					CCH_unwind(tdbb, true);
				}
			}

			PAGE_LOCK_RELEASE(bdb->bdb_lock);
			bdb->bdb_flags &= ~BDB_no_blocking_ast;
			bdb->bdb_ast_flags &= ~BDB_blocking;
		}
#endif

		/* Make buffer the least-recently-used by queueing it
		   to the LRU tail. */

		if (release_tail)
		{
			if ((window->win_flags & WIN_large_scan &&
				 bdb->bdb_scan_count > 0 &&
				 !(--bdb->bdb_scan_count) &&
				 !(bdb->bdb_flags & BDB_garbage_collect)) ||
				(window->win_flags & WIN_garbage_collector &&
				 bdb->bdb_flags & BDB_garbage_collect &&
				 !(bdb->bdb_scan_count)))
			{
				if (window->win_flags & WIN_garbage_collector)
				{
					bdb->bdb_flags &= ~BDB_garbage_collect;
				}
				BufferControl* bcb = dbb->dbb_bcb;
				QUE_LEAST_RECENTLY_USED(bdb->bdb_in_use);
#ifdef CACHE_WRITER
				if (bdb->bdb_flags & (BDB_dirty | BDB_db_dirty))
				{
					bcb->bcb_flags |= BCB_free_pending;
					if (bcb->bcb_flags & BCB_cache_writer &&
						!(bcb->bcb_flags & BCB_writer_active))
					{
						ISC_event_post(dbb->dbb_writer_event);
					}
				}
#endif
			}
		}
	}

	release_bdb(tdbb, bdb, false, false, false);
	const SSHORT use_count = bdb->bdb_use_count;

	if (use_count < 0) {
		BUGCHECK(209);			/* msg 209 attempt to release page not acquired */
	}

	if (!use_count && (bdb->bdb_ast_flags & BDB_blocking))
	{
		PAGE_LOCK_RE_POST(bdb->bdb_lock);
	}

	window->win_bdb = NULL;
}


void CCH_release_exclusive(thread_db* tdbb)
{
/**************************************
 *
 *	C C H _ r e l e a s e _ e x c l u s i v e
 *
 **************************************
 *
 * Functional description
 *	Release exclusive access to database.
 *
 **************************************/
	SET_TDBB(tdbb);	
	Database* dbb = tdbb->tdbb_database;
	dbb->dbb_flags &= ~DBB_exclusive;

	Attachment* attachment = tdbb->tdbb_attachment;
	if (attachment) {
		attachment->att_flags &= ~ATT_exclusive;
	}

	if (dbb->dbb_ast_flags & DBB_blocking) {
		LCK_re_post(dbb->dbb_lock);
	}
}


bool CCH_rollover_to_shadow(Database* dbb, jrd_file* file, const bool inAst)
{
/**************************************
 *
 *	C C H _ r o l l o v e r _ t o _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	An I/O error has been detected on the
 *	main database file.  Roll over to use
 *	the shadow file.
 *
 **************************************/
/* Is the shadow subsystem yet initialized */
	if (!dbb->dbb_shadow_lock) {
		return false;
	}
/* notify other process immediately to ensure all read from sdw
   file instead of db file */
	return SDW_rollover_to_shadow(file, inAst);
}


void CCH_shutdown_database(Database* dbb)
{
/**************************************
 *
 *	C C H _ s h u t d o w n _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Shutdown database physical page locks.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	bcb_repeat* tail;
	BufferControl* bcb = dbb->dbb_bcb;
	if (bcb && (tail = bcb->bcb_rpt) && (tail->bcb_bdb)) {
		for (const bcb_repeat* const end = tail + bcb->bcb_count;
			tail < end; tail++)
		{
			BufferDesc* bdb = tail->bcb_bdb;
			bdb->bdb_flags &= ~(BDB_dirty | BDB_db_dirty);
			PAGE_LOCK_RELEASE(bdb->bdb_lock);
		}
	}

#ifndef SUPERSERVER
	PIO_close(dbb->dbb_file);
	SDW_close();
#endif
}

void CCH_unwind(thread_db* tdbb, bool punt)
{
/**************************************
 *
 *	C C H _ u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Synchronously unwind cache after I/O or lock error.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* CCH_unwind is called when any of the following occurs:
	- IO error
	- journaling error => obsolete
	- bad page checksum
	- wrong page type
	- page locking (not latching) deadlock (doesn't happen on Netware) */

	BufferControl* bcb = dbb->dbb_bcb;
	if (!bcb || (tdbb->tdbb_flags & TDBB_no_cache_unwind)) {
		if (punt) {
			ERR_punt();
		}
		else {
			return;
		}
	}

/* A cache error has occurred. Scan the cache for buffers
   which may be in use and release them. */

	bcb_repeat* tail = bcb->bcb_rpt;
	for (const bcb_repeat* const end = tail + bcb->bcb_count; tail < end; tail++)
	{
		BufferDesc* bdb = tail->bcb_bdb;
#ifndef SUPERSERVER
//		if (bdb->bdb_length || !bdb->bdb_use_count) becomes (false || expr)
		if (!bdb->bdb_use_count) {
			continue;
		}
		if (bdb->bdb_flags & BDB_marked) {
			cache_bugcheck(268);	/* msg 268 buffer marked during cache unwind */
		}
		bdb->bdb_flags &= ~BDB_writer;
		while (bdb->bdb_use_count) {
			release_bdb(tdbb, bdb, true, false, false);
		}
		pag* page = bdb->bdb_buffer;
		if ((page->pag_type == pag_header) ||
			(page->pag_type == pag_transactions))
		{
			++bdb->bdb_use_count;
			// Adjust backup page locks
			if (bdb->bdb_flags & BDB_dirty) {
				set_write_direction(dbb, bdb, BDB_write_undefined);
			}
			bdb->bdb_flags &= ~(BDB_dirty | 
				BDB_writer | BDB_marked | BDB_faked | BDB_db_dirty);
			PAGE_LOCK_RELEASE(bdb->bdb_lock);
			--bdb->bdb_use_count;
		}
#else
		if (!bdb->bdb_use_count) {
			continue;
		}
		if (bdb->bdb_io == tdbb) {
			release_bdb(tdbb, bdb, true, false, false);
		}
		if (bdb->bdb_exclusive == tdbb) {
			/* The sanity check below can't be enforced for the log page because
			   the ail.c code does IO while having the log page marked (should be
			   fixed in the future). */
			if ((bdb->bdb_flags & BDB_marked) && (bdb->bdb_page != LOG_PAGE)) {
				cache_bugcheck(268);	/* msg 268 buffer marked during cache unwind */
			}
			bdb->bdb_flags &= ~(BDB_writer | BDB_faked | BDB_must_write);
			release_bdb(tdbb, bdb, true, false, false);
		}
		for (int i = 0; i < BDB_max_shared; ++i) {
			if (bdb->bdb_shared[i] == tdbb) {
				release_bdb(tdbb, bdb, true, false, false);
			}
		}
#endif
	}

	if (punt) {
		ERR_punt();
	}
}


bool CCH_validate(WIN * window)
{
/**************************************
 *
 *	C C H _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Give a page a quick once over looking for unhealthyness.
 *
 **************************************/
	BufferDesc* bdb = window->win_bdb;

/* If page is marked for write, checksum is questionable */

	if ((bdb->bdb_flags & (BDB_dirty | BDB_db_dirty))) {
		return true;
	}

	pag* page = window->win_buffer;
	const USHORT sum = CCH_checksum(bdb);

	if (sum == page->pag_checksum) {
		return true;
	}

	return false;
}


bool CCH_write_all_shadows(thread_db* tdbb,
							  Shadow* shadow,
							  BufferDesc* bdb,
							  ISC_STATUS* status, USHORT checksum,
							  const bool inAst)
{
/**************************************
 *
 *	C C H _ w r i t e _ a l l _ s h a d o w s
 *
 **************************************
 *
 * Functional description
 *	Compute a checksum and write a page out to all shadows
 *	detecting failure on write.
 *	If shadow is null, write to all shadows, otherwise only to specified
 *	shadow.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	Shadow* sdw = shadow ? shadow : dbb->dbb_shadow;

	if (!sdw) {
		return true;
	}

	bool result = true;
	SLONG* spare_buffer = NULL;

	try {

	pag* page;
	pag* old_buffer = 0;
	if (bdb->bdb_page == HEADER_PAGE) {
		/* allocate a spare buffer which is large enough,
		   and set up to release it in case of error */

		spare_buffer =
			(SLONG*) dbb->dbb_bufferpool->allocate((SLONG) dbb->dbb_page_size, 0
#ifdef DEBUG_GDS_ALLOC
			  ,__FILE__,__LINE__
#endif
			);

		page = (pag*) spare_buffer;
		MOVE_FAST((const UCHAR*) bdb->bdb_buffer, (UCHAR*) page, HDR_SIZE);
		old_buffer = bdb->bdb_buffer;
		bdb->bdb_buffer = page;
	}
	else {
		page = bdb->bdb_buffer;
		if (checksum) {
			page->pag_checksum = CCH_checksum(bdb);
		}
	}

	for (; sdw; sdw = sdw->sdw_next) {
		/* don't bother to write to the shadow if it is no longer viable */

		/* Fix for bug 7925. drop_gdb fails to remove secondary file if
		   the shadow is conditional. Reason being the header page not
		   being correctly initialized.

		   The following block was not being performed for a conditional
		   shadow since SDW_INVALID expanded to include conditional shadow

		   -Sudesh  07/10/95
		   old code --> if (sdw->sdw_flags & SDW_INVALID)
		 */

		if ((sdw->sdw_flags & SDW_INVALID) &&
			!(sdw->sdw_flags & SDW_conditional))
		{
			continue;
		}

		if (bdb->bdb_page == HEADER_PAGE) {
			/* fixup header for shadow file */
			jrd_file* shadow_file = sdw->sdw_file;
			header_page* header = (header_page*) page;
			const UCHAR* q = (UCHAR *) dbb->dbb_file->fil_string;
			header->hdr_data[0] = HDR_end;
			header->hdr_end = HDR_SIZE;
			header->hdr_next_page = 0;

			PAG_add_header_entry(header, HDR_root_file_name,
								 (USHORT) strlen((const char*) q), q);

			jrd_file* next_file = shadow_file->fil_next;
			if (next_file) {
				q = (UCHAR *) next_file->fil_string;
				const SLONG last = next_file->fil_min_page - 1;
				PAG_add_header_entry(header, HDR_file,
									 (USHORT) strlen((const char*) q), q);
				PAG_add_header_entry(header, HDR_last_page, sizeof(last),
									 (const UCHAR*) & last);
			}

			header->hdr_flags |= hdr_active_shadow;
			header->hdr_header.pag_checksum = CCH_checksum(bdb);
		}

/* This condition makes sure that PIO_write is performed in case of
   conditional shadow only if the page is Header page

	-Sudesh 07/10/95
*/
		if ((sdw->sdw_flags & SDW_conditional) &&
			(bdb->bdb_page != HEADER_PAGE))
		{
			continue;
		}

		/* if a write failure happens on an AUTO shadow, mark the
		   shadow to be deleted at the next available opportunity when we
		   know we don't have a page fetched */

		if (!PIO_write(sdw->sdw_file, bdb, page, status)) {
			if (sdw->sdw_flags & SDW_manual) {
				result = false;
			}
			else {
				sdw->sdw_flags |= SDW_delete;
				if (!inAst && SDW_check_conditional()) {
					if (SDW_lck_update((SLONG) 0)) {
						SDW_notify();
						CCH_unwind(tdbb, false);
						SDW_dump_pages();
						ERR_post(isc_deadlock, 0);
					}
				}
			}
		}
		/* If shadow was specified, break out of loop */

		if (shadow) {
			break;
		}
	}

	if (bdb->bdb_page == HEADER_PAGE) {
		bdb->bdb_buffer = old_buffer;
	}

	if (spare_buffer) {
		dbb->dbb_bufferpool->deallocate(spare_buffer);
	}

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		if (spare_buffer) {
			dbb->dbb_bufferpool->deallocate(spare_buffer);
		}
		ERR_punt();
	}

	return result;
}


static BufferDesc* alloc_bdb(thread_db* tdbb, BufferControl* bcb, UCHAR** memory)
{
/**************************************
 *
 *	a l l o c _ b d b
 *
 **************************************
 *
 * Functional description
 *	Allocate buffer descriptor block.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	BufferDesc* bdb = FB_NEW(*dbb->dbb_bufferpool) BufferDesc;
	bdb->bdb_dbb = dbb;

#ifndef PAGE_LATCHING
	Lock* lock;
	try {
		bdb->bdb_lock = lock = CCH_page_lock(tdbb);
	}
	catch (const std::exception&) {
		delete bdb;
		throw;
	}
	lock->lck_ast = blocking_ast_bdb;
	lock->lck_object = bdb;
#endif

	bdb->bdb_buffer = (pag*) *memory;
	*memory += dbb->dbb_page_size;

	QUE_INIT(bdb->bdb_higher);
	QUE_INIT(bdb->bdb_lower);
	QUE_INIT(bdb->bdb_waiters);
	QUE_INSERT(bcb->bcb_empty, bdb->bdb_que);

	return bdb;
}


#ifndef PAGE_LATCHING
static int blocking_ast_bdb(void* ast_object)
{
/**************************************
 *
 *	b l o c k i n g _ a s t _ b d b
 *
 **************************************
 *
 * Functional description
 *	Blocking AST for buffer control blocks.  This is called at
 *	AST (signal) level to indicate that a lock is blocking another
 *	process.  If the BufferDesc* is in use, set a flag indicating that the
 *	lock is blocking and return.  When the BufferDesc is released at normal
 *	level the lock will be down graded.  If the BufferDesc* is not in use,
 *	write the page if dirty then downgrade lock.  Things would be
 *	much hairier if UNIX supported asynchronous IO, but it doesn't.
 *	WHEW!
 *
 **************************************/
	BufferDesc* bdb = static_cast<BufferDesc*>(ast_object);

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */
	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	BLKCHK(bdb, type_bdb);
	
	ISC_STATUS_ARRAY ast_status;
	Database* dbb = bdb->bdb_dbb;

	tdbb->tdbb_database = dbb;
	tdbb->tdbb_attachment = NULL;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	tdbb->tdbb_status_vector = ast_status;

/* Do some fancy footwork to make sure that pages are
   not removed from the btc tree at AST level.  Then
   restore the flag to whatever it was before. */

	const bool keep_pages = (dbb->dbb_bcb->bcb_flags & BCB_keep_pages) != 0;
	dbb->dbb_bcb->bcb_flags |= BCB_keep_pages;
	ast_status[1] = 0;

	down_grade(tdbb, bdb);

	if (!keep_pages) {
		dbb->dbb_bcb->bcb_flags &= ~BCB_keep_pages;
	}

	if (ast_status[1]) {
		gds__log_status(dbb->dbb_file->fil_string, ast_status);
	}

/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();

    return 0;
}
#endif


static void btc_flush(thread_db* tdbb,
					  SLONG transaction_mask,
					  const bool sys_only, ISC_STATUS* status)
{
/**************************************
 *
 *	b t c _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Walk the dirty page binary tree, flushing all buffers
 *	that could have been modified by this transaction.
 *	The pages are flushed in page order to roughly
 *	emulate an elevator-type disk controller. Iteration
 *	is used to minimize call overhead.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* traverse the tree, flagging to prevent pages
   from being removed from the tree during write_page() --
   this simplifies worrying about random pages dropping
   out when dependencies have been set up */

	SLONG max_seen = MIN_PAGE_NUMBER;

/* Pick starting place at leftmost node */

//	BTC_MUTEX_ACQUIRE;
	BufferDesc* next = dbb->dbb_bcb->bcb_btree;
	while (next && next->bdb_left) {
		 next = next->bdb_left;
	}

	SLONG next_page = 0;
	if (next) {
		next_page = next->bdb_page;
	}

/* Walk tree.  If we get lost, reposition and continue */

	BufferDesc* bdb;
	while ( (bdb = next) ) {
		/* If we're lost, reposition */

		if ((bdb->bdb_page != next_page) ||
			(!bdb->bdb_parent && (bdb != dbb->dbb_bcb->bcb_btree)))
		{
			for (bdb = dbb->dbb_bcb->bcb_btree; bdb;) {
				if (bdb->bdb_left && (max_seen < bdb->bdb_page)) {
					bdb = bdb->bdb_left;
				}
				else if (bdb->bdb_right && (max_seen > bdb->bdb_page)) {
					bdb = bdb->bdb_right;
				}
				else {
					break;
				}
			}
			if (!bdb) {
				break;
			}
		}

		/* Decide where to go next.  The options are (right, then down to the left)
		   or up */

		if (bdb->bdb_right && (max_seen < bdb->bdb_right->bdb_page)) {
			for (next = bdb->bdb_right; next->bdb_left;
				 next = next->bdb_left);
		}
		else {
			next = bdb->bdb_parent;
		}

		if (next) {
			next_page = next->bdb_page;
		}

		if (max_seen >= bdb->bdb_page) {
			continue;
		}

		max_seen = bdb->bdb_page;

		/* forget about this page if it was written out
		   as a dependency while we were walking the tree */

		if (!(bdb->bdb_flags & BDB_dirty)) {
//			BTC_MUTEX_RELEASE;
			btc_remove(bdb);
//			BTC_MUTEX_ACQUIRE;
			continue;
		}

		/* this code replicates code in CCH_flush() --
		   changes should be made in both places */

		const SLONG page = bdb->bdb_page;
//		BTC_MUTEX_RELEASE;

#ifndef SUPERSERVER
		if (bdb->bdb_use_count)
			cache_bugcheck(210);	/* msg 210 page in use during flush */
#endif
		/* if any transaction has dirtied this page,
		   check to see if it could have been this one */

		if ((transaction_mask & bdb->bdb_transactions) ||
			(bdb->bdb_flags & BDB_system_dirty) ||
			(!transaction_mask && !sys_only) || (!bdb->bdb_transactions))
		{
			if (!write_buffer(tdbb, bdb, page, false, status, true)) {
				CCH_unwind(tdbb, true);
			}
		}

		/* re-post the lock only if it was really written */

		if ((bdb->bdb_ast_flags & BDB_blocking) &&
			!(bdb->bdb_flags & BDB_dirty))
		{
			PAGE_LOCK_RE_POST(bdb->bdb_lock);
		}
//		BTC_MUTEX_ACQUIRE;
	}

//	BTC_MUTEX_RELEASE;
}


static void btc_insert_balanced(Database* dbb, BufferDesc* bdb)
{
/**************************************
 *
 *	b t c _ i n s e r t _ b a l a n c e d
 *
 **************************************
 *
 * Functional description
 *	Insert a buffer into the dirty page
 *	AVL-binary tree.
 *
 **************************************/

	// avoid recursion when rebalancing tree
	// (40 - enough to hold 2^32 nodes)
	BalancedTreeNode stack[BTREE_STACK_SIZE];

/* if the page is already in the tree (as in when it is
   written out as a dependency while walking the tree),
   just leave well enough alone -- this won't check if
   it's at the root but who cares then */

	if (bdb->bdb_parent) {
		return;
	}

	SET_DBB(dbb);

/* if the tree is empty, this is now the tree */

//	BTC_MUTEX_ACQUIRE;
	BufferDesc* p = dbb->dbb_bcb->bcb_btree;
	if (!p) {
		dbb->dbb_bcb->bcb_btree = bdb;
		bdb->bdb_parent = bdb->bdb_left = bdb->bdb_right = NULL;
		bdb->bdb_balance = 0;
//		BTC_MUTEX_RELEASE;
		return;
	}

/* insert the page sorted by page number;
   do this iteratively to minimize call overhead */

	const SLONG page = bdb->bdb_page;

/* find where new node should fit in tree */

	int stackp = -1;
	SCHAR comp = 0;

	while (p)
	{
		if (page == p->bdb_page)
		{
			comp = 0;
		}
		else if (page > p->bdb_page)
		{
			comp = 1;
		}
		else
		{
			comp = -1;
		}
	
		if (comp == 0)
		{
//			BTC_MUTEX_RELEASE;
			return;
		} // already in the tree

		stackp++;
		fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
		stack[stackp].bdb_node = p;
		stack[stackp].comp = comp;

		p = (comp > 0) ? p->bdb_right : p->bdb_left;
	}

/* insert new node */

	fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
	if (comp > 0)
	{
		stack[stackp].bdb_node->bdb_right = bdb;
	}
	else
	{
		stack[stackp].bdb_node->bdb_left = bdb;
	}

	bdb->bdb_parent = stack[stackp].bdb_node;
	bdb->bdb_left = bdb->bdb_right = NULL;
	bdb->bdb_balance = 0;

/* unwind the stack and rebalance */

	bool subtree = true;

	while (stackp >= 0 && subtree)
	{
		fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
		if (stackp == 0)
		{
			subtree = btc_insert_balance(&dbb->dbb_bcb->bcb_btree,
										 subtree, stack[0].comp);
		}
		else
		{
			if (stack[stackp - 1].comp > 0)
			{
				subtree = btc_insert_balance(&stack[stackp - 1].bdb_node->bdb_right,
											 subtree, stack[stackp].comp);
			}
			else
			{
				subtree = btc_insert_balance(&stack[stackp - 1].bdb_node->bdb_left,
											 subtree, stack[stackp].comp);
			}
		}
		stackp--;
	}
//	BTC_MUTEX_RELEASE;
}


static bool btc_insert_balance(BufferDesc** bdb, bool subtree, SCHAR comp)
{
/**************************************
 *
 *	b t c _ i n s e r t _ b a l a n c e
 *
 **************************************
 *
 * Functional description
 *	Rebalance the AVL-binary tree.
 *
 **************************************/

	BufferDesc *p1, *p2;
	BufferDesc* p = *bdb;

	if (p->bdb_balance == -comp)
	{
		p->bdb_balance = 0;
		subtree = false;
	}
	else
	{
	    if (p->bdb_balance == 0)
		{
			p->bdb_balance = comp;
		}
		else
		{
			if (comp > 0)
			{
				p1 = p->bdb_right;

				if (p1->bdb_balance == comp)
				{
					if ( (p->bdb_right = p1->bdb_left) )
					{
						p1->bdb_left->bdb_parent = p;
					}

					p1->bdb_left = p;
					p1->bdb_parent = p->bdb_parent;
					p->bdb_parent = p1;
					p->bdb_balance = 0;
					p = p1;
				}
				else
				{
					p2 = p1->bdb_left;

					if ( (p1->bdb_left = p2->bdb_right) )
					{
						p2->bdb_right->bdb_parent = p1;
					}

					p2->bdb_right = p1;
					p1->bdb_parent = p2;

					if ( (p->bdb_right = p2->bdb_left) )
					{
						p2->bdb_left->bdb_parent = p;
					}

					p2->bdb_left = p;
					p2->bdb_parent = p->bdb_parent;
					p->bdb_parent = p2;

					if (p2->bdb_balance == comp)
					{
						p->bdb_balance = -comp;
					}
					else
					{
						p->bdb_balance = 0;
					}

					if (p2->bdb_balance == -comp)
					{
						p1->bdb_balance = comp;
					}
					else
					{
						p1->bdb_balance = 0;
					}

					p = p2;
	            }
		    }
			else
			{
				p1 = p->bdb_left;
				
				if (p1->bdb_balance == comp)
				{
					if ( (p->bdb_left = p1->bdb_right) )
					{
						p1->bdb_right->bdb_parent = p;
					}
	
					p1->bdb_right = p;
					p1->bdb_parent = p->bdb_parent;
					p->bdb_parent = p1;
					p->bdb_balance = 0;
					p = p1;
				}
				else
				{
					p2 = p1->bdb_right;

					if ( (p1->bdb_right = p2->bdb_left) )
					{
						p2->bdb_left->bdb_parent = p1;
					}

					p2->bdb_left = p1;
					p1->bdb_parent = p2;

					if ( (p->bdb_left = p2->bdb_right) )
					{
						p2->bdb_right->bdb_parent = p;
					}

					p2->bdb_right = p;
					p2->bdb_parent = p->bdb_parent;
					p->bdb_parent = p2;

					if (p2->bdb_balance == comp)
					{
						p->bdb_balance = -comp;
					}
					else
					{
						p->bdb_balance = 0;
					}

					if (p2->bdb_balance == -comp)
					{
						p1->bdb_balance = comp;
					}
					else
					{
						p1->bdb_balance = 0;
					}

					p = p2;
	            }
	        }
			p->bdb_balance = 0;
			subtree = false;
			*bdb = p;
		}
	}

	return subtree;
}


static void btc_insert_unbalanced(Database* dbb, BufferDesc* bdb)
{
/**************************************
 *
 *	b t c _ i n s e r t _ u n b a l a n c e d
 *
 **************************************
 *
 * Functional description
 *	Insert a buffer into the dirty page
 *	binary tree.
 *
 **************************************/

/* if the page is already in the tree (as in when it is
   written out as a dependency while walking the tree),
   just leave well enough alone -- this won't check if
   it's at the root but who cares then */

	if (bdb->bdb_parent) {
		return;
	}

	SET_DBB(dbb);

/* if the tree is empty, this is now the tree */

//	BTC_MUTEX_ACQUIRE;
	BufferDesc* node = dbb->dbb_bcb->bcb_btree;
	if (!node) {
		dbb->dbb_bcb->bcb_btree = bdb;
//		BTC_MUTEX_RELEASE;
		return;
	}

/* insert the page sorted by page number;
   do this iteratively to minimize call overhead */

	const SLONG page = bdb->bdb_page;

	while (true) {
		if (page == node->bdb_page) {
			break;
		}

		if (page < node->bdb_page) {
			if (!node->bdb_left) {
				node->bdb_left = bdb;
				bdb->bdb_parent = node;
				break;
			}
			else {
				node = node->bdb_left;
			}
		}
		else {
			if (!node->bdb_right) {
				node->bdb_right = bdb;
				bdb->bdb_parent = node;
				break;
			}
			else
				node = node->bdb_right;
		}
	}

//	BTC_MUTEX_RELEASE;
}


static void btc_remove_balanced(BufferDesc* bdb)
{
/**************************************
 *
 *	b t c _ r e m o v e _ b a l a n c e d
 *
 **************************************
 *
 * Functional description
 * 	Remove a page from the dirty page
 *  AVL-binary tree.
 *
 **************************************/

	// avoid recursion when rebalancing tree
	// (40 - enough to hold 2^32 nodes)
	BalancedTreeNode stack[BTREE_STACK_SIZE];

	Database* dbb = bdb->bdb_dbb;

/* engage in a little defensive programming to make
   sure the node is actually in the tree */

//	BTC_MUTEX_ACQUIRE;
	BufferControl* bcb = dbb->dbb_bcb;

	if ((!bcb->bcb_btree) ||
		(!bdb->bdb_parent &&
		 !bdb->bdb_left && !bdb->bdb_right && (bcb->bcb_btree != bdb)))
	{
		if ((bdb->bdb_flags & BDB_must_write) || !(bdb->bdb_flags & BDB_dirty))
		{
			/* Must writes aren't worth the effort */
//			BTC_MUTEX_RELEASE;
			return;
		}
		else {
			cache_bugcheck(211);
			/* msg 211 attempt to remove page from dirty page list when not there */
		}
	}

/* stack the way to node from root */

	const SLONG page = bdb->bdb_page;

	BufferDesc* p = bcb->bcb_btree;
	int stackp = -1;
	SCHAR comp;

	while (true)
	{
		if (page == p->bdb_page)
		{
			comp = 0;
		}
		else if (page > p->bdb_page)
		{
			comp = 1;
		}
		else
		{
			comp = -1;
		}
	
		stackp++;
		fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);

		if (comp == 0)
		{
			stack[stackp].bdb_node = p;
			stack[stackp].comp = -1;
			break;
		}
		else
		{
			stack[stackp].bdb_node = p;
			stack[stackp].comp = comp;
			
			p = (comp > 0) ? p->bdb_right : p->bdb_left;

			// node not found, bad tree
			if (!p)
			{
				cache_bugcheck(211);
			}
		}
	}

	// wrong node found, bad tree

	if (bdb != p)
	{
		cache_bugcheck(211);
	}

/* delete node */

	if (!bdb->bdb_right || !bdb->bdb_left)
	{
		// node has at most one branch
		stackp--;
		p = bdb->bdb_right ? bdb->bdb_right : bdb->bdb_left;

		if (stackp == -1)
		{
			if ( (bcb->bcb_btree = p) )
			{
				p->bdb_parent = NULL;
			}
		}
		else
		{
			fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
			if (stack[stackp].comp > 0)
			{
                stack[stackp].bdb_node->bdb_right = p;
			}
			else
			{
				stack[stackp].bdb_node->bdb_left = p;
			}
		
			if (p)
			{
				p->bdb_parent = stack[stackp].bdb_node;
			}
		}
	}
	else
	{
		// node has two branches, stack nodes to reach one with no right child

		p = bdb->bdb_left;

		if (!p->bdb_right)
		{
			if (stack[stackp].comp > 0)
			{
				cache_bugcheck(211);
			}

			if ( (p->bdb_parent = bdb->bdb_parent) )
			{
				if (p->bdb_parent->bdb_right == bdb)
				{
					p->bdb_parent->bdb_right = p;
				}
				else
				{
					p->bdb_parent->bdb_left = p;
				}
			}
			else
			{
				bcb->bcb_btree = p;	// new tree root
			}

			if ( (p->bdb_right = bdb->bdb_right) )
			{
				bdb->bdb_right->bdb_parent = p;
			}

			p->bdb_balance = bdb->bdb_balance;
		}
		else
		{
			const int stackp_save = stackp; 

			while (p->bdb_right)
			{
				stackp++;
				fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
				stack[stackp].bdb_node = p;
				stack[stackp].comp = 1;
				p = p->bdb_right;
			}

			if (p->bdb_parent = bdb->bdb_parent)
			{
				if (p->bdb_parent->bdb_right == bdb)
				{
					p->bdb_parent->bdb_right = p;
				}
				else
				{
					p->bdb_parent->bdb_left = p;
				}
			}
			else
			{
				bcb->bcb_btree = p;	// new tree root
			}

			if ( (stack[stackp].bdb_node->bdb_right = p->bdb_left) )
			{
				p->bdb_left->bdb_parent = stack[stackp].bdb_node;
			}

			if ( (p->bdb_left = bdb->bdb_left) )
			{
				p->bdb_left->bdb_parent = p;
			}

			if ( (p->bdb_right = bdb->bdb_right) )
			{
				p->bdb_right->bdb_parent = p;
			}

			p->bdb_balance = bdb->bdb_balance;
			stack[stackp_save].bdb_node = p; // replace BufferDesc in stack
		}
	}

/*unwind the stack and rebalance */

	bool subtree = true;

	while (stackp >= 0 && subtree)
	{
		fb_assert(stackp >= 0 && stackp < BTREE_STACK_SIZE);
		if (stackp == 0)
		{
			subtree = btc_remove_balance(&bcb->bcb_btree,
										 subtree, stack[0].comp);
		}
		else
		{
			if (stack[stackp - 1].comp > 0)
			{
				subtree = btc_remove_balance(&stack[stackp - 1].bdb_node->bdb_right,
											 subtree, stack[stackp].comp);
			}
			else
			{
				subtree = btc_remove_balance(&stack[stackp - 1].bdb_node->bdb_left,
											 subtree, stack[stackp].comp);
			}
		}
		stackp--;
	}

/* initialize the node for next usage */

	bdb->bdb_left = bdb->bdb_right = bdb->bdb_parent = NULL;
//	BTC_MUTEX_RELEASE;
}


static bool btc_remove_balance(BufferDesc** bdb, bool subtree, SCHAR comp)
{
/**************************************
 *
 *	b t c _ r e m o v e _ b a l a n c e
 *
 **************************************
 *
 * Functional description
 * 	Rebalance the AVL-binary tree.
 *
 **************************************/

	SCHAR b1, b2;
	BufferDesc *p1, *p2;
	BufferDesc* p = *bdb;

	if (p->bdb_balance == comp)
	{
		p->bdb_balance = 0;
	}
	else
	{
		if (p->bdb_balance == 0)
		{
			p->bdb_balance = -comp;
			subtree = false;
        }
		else
		{
			if (comp < 0)
			{
				p1 = p->bdb_right;
				b1 = p1->bdb_balance;

				if ((b1 == 0) || (b1 == -comp))
				{
					// single RR or LL rotation

					if ( (p->bdb_right = p1->bdb_left) )
					{
						p1->bdb_left->bdb_parent = p;
					}

					p1->bdb_left = p;
					p1->bdb_parent = p->bdb_parent;
					p->bdb_parent = p1;

					if (b1 == 0)
					{
						p->bdb_balance = -comp;
						p1->bdb_balance = comp;
						subtree = false;
					}
					else
					{
						p->bdb_balance = 0;
						p1->bdb_balance = 0;
					}

					p = p1;
				}
				else
				{
					// double RL or LR rotation

					p2 = p1->bdb_left; 
					b2 = p2->bdb_balance;

					if ( (p1->bdb_left = p2->bdb_right) )
					{
						p2->bdb_right->bdb_parent = p1;
					}

					p2->bdb_right = p1; 
					p1->bdb_parent = p2;

					if ( (p->bdb_right = p2->bdb_left) )
					{
						p2->bdb_left->bdb_parent = p;
					}

					p2->bdb_left = p;
					p2->bdb_parent = p->bdb_parent; 
					p->bdb_parent = p2;

					if (b2 == -comp)
					{
						p->bdb_balance = comp;
					}
					else
					{
						p->bdb_balance = 0;
					}

					if (b2 == comp)
					{
						p1->bdb_balance = -comp;
					}
					else
					{
						p1->bdb_balance = 0;
					}

					p = p2;
					p2->bdb_balance = 0;
				}
			}
			else
			{
				p1 = p->bdb_left;
				b1 = p1->bdb_balance;

				if ((b1 == 0) || (b1 == -comp))
				{
					// single RR or LL rotation

					if ( (p->bdb_left = p1->bdb_right) )
					{
						p1->bdb_right->bdb_parent = p;
					}

					p1->bdb_right = p;
					p1->bdb_parent = p->bdb_parent; 
					p->bdb_parent = p1;

					if (b1 == 0)
					{
						p->bdb_balance = -comp;
						p1->bdb_balance = comp;
						subtree = false;
					}
					else
					{
						p->bdb_balance = 0;
						p1->bdb_balance = 0;
					}

					p = p1;
				}
				else
				{
					// double RL or LR rotation

					p2 = p1->bdb_right;
					b2 = p2->bdb_balance;

					if ( (p1->bdb_right = p2->bdb_left) )
					{
						p2->bdb_left->bdb_parent = p1;
					}

					p2->bdb_left = p1;
					p1->bdb_parent = p2;

					if ( (p->bdb_left = p2->bdb_right) )
					{
						p2->bdb_right->bdb_parent = p;
					}

					p2->bdb_right = p; 
					p2->bdb_parent = p->bdb_parent;
					p->bdb_parent = p2;

					if (b2 == -comp) 
					{
						p->bdb_balance = comp;
					}
					else
					{
						p->bdb_balance = 0;
					}

					if (b2 == comp)
					{
						p1->bdb_balance = -comp;
					}
					else
					{
						p1->bdb_balance = 0;
					}

					p = p2;
					p2->bdb_balance = 0;
				}
			}

			*bdb = p;
		}
	}

	return subtree;
}


static void btc_remove_unbalanced(BufferDesc* bdb)
{
/**************************************
 *
 *	b t c _ r e m o v e _ u n b a l a n c e d
 *
 **************************************
 *
 * Functional description
 * 	Remove a page from the dirty page binary tree.
 *	The idea is to place the left child of this
 *	page in this page's place, then make the
 *	right child of this page a child of the left
 *	child -- this removal mechanism won't promote
 *	a balanced tree but that isn't of primary
 *	importance.
 *
 **************************************/
	Database* dbb = bdb->bdb_dbb;

/* engage in a little defensive programming to make
   sure the node is actually in the tree */

//	BTC_MUTEX_ACQUIRE;
	BufferControl* bcb = dbb->dbb_bcb;
	if ((!bcb->bcb_btree) ||
		(!bdb->bdb_parent &&
		 !bdb->bdb_left && !bdb->bdb_right && (bcb->bcb_btree != bdb)))
	{
		if ((bdb->bdb_flags & BDB_must_write) || !(bdb->bdb_flags & BDB_dirty))
		{
			/* Must writes aren't worth the effort */
//			BTC_MUTEX_RELEASE;
			return;
		}
		else {
			cache_bugcheck(211);
			/* msg 211 attempt to remove page from dirty page list when not there */
		}
	}

/* make a new child out of the left and right children */

	BufferDesc* new_child = bdb->bdb_left;
	if (new_child) {
		BufferDesc* ptr = new_child;
		while (ptr->bdb_right) {
			ptr = ptr->bdb_right;
		}
		if ( (ptr->bdb_right = bdb->bdb_right) ) {
			ptr->bdb_right->bdb_parent = ptr;
		}
	}
	else {
		new_child = bdb->bdb_right;
	}

/* link the parent with the child node --
   if no parent place it at the root */

	BufferDesc* bdb_parent = bdb->bdb_parent;
	if (!bdb_parent) {
		bcb->bcb_btree = new_child;
	}
	else if (bdb_parent->bdb_left == bdb) {
		bdb_parent->bdb_left = new_child;
	}
	else {
		bdb_parent->bdb_right = new_child;
	}

	if (new_child) {
		new_child->bdb_parent = bdb_parent;
	}

/* initialize the node for next usage */

	bdb->bdb_left = bdb->bdb_right = bdb->bdb_parent = NULL;
//	BTC_MUTEX_RELEASE;
}


static void cache_bugcheck(int number)
{
/**************************************
 *
 *	c a c h e _ b u g c h e c k
 *
 **************************************
 *
 * Functional description
 *	There has been a bugcheck during a cache operation.  Release
 *	the cache mutex and post the bugcheck.
 *
 **************************************/
	BUGCHECK(number);
}


#ifdef CACHE_READER
static THREAD_ENTRY_DECLARE cache_reader(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	c a c h e _ r e a d e r
 *
 **************************************
 *
 * Functional description
 *	Prefetch pages into cache for sequential scans.
 *	Use asynchronous I/O to keep two prefetch requests
 *	busy at a time.
 *
 **************************************/
	Database* dbb = (Database*)arg;
	THREAD_ENTER();

/* Establish a thread context. */
/* Note: Since this function operates as its own thread,
   we have no need to restore the THREAD CONTEXT on exit.
   Once we reach the end, the thread will die, thus implicitly
   killing all its contexts. */
	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	ISC_STATUS_ARRAY status_vector;
/* Dummy attachment needed for lock owner identification. */
	tdbb->tdbb_database = dbb;
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_bufferpool);
	tdbb->tdbb_status_vector = status_vector;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_attachment = FB_NEW(*dbb->dbb_bufferpool) Attachment();
	tdbb->tdbb_attachment->att_database = dbb;
	tdbb->tdbb_attachment->att_filename = dbb->dbb_filename;

/* This try block is specifically to protect the LCK_init call: if
   LCK_init fails we won't be able to accomplish anything anyway, so
   return, unlike the other try blocks further down the page. */
   
	event_t* reader_event = 0;
	BufferControl* bcb = 0;

	try {

		LCK_init(tdbb, LCK_OWNER_attachment);
		reader_event = dbb->dbb_reader_event;
		bcb = dbb->dbb_bcb;
		bcb->bcb_flags |= BCB_cache_reader;
		ISC_event_post(reader_event);	/* Notify our creator that we have started  */
	}
	catch (const std::exception& ex) {
		Firebird::stuff_exception(status_vector, ex);
		gds__log_status(dbb->dbb_file->fil_string, status_vector);
		THREAD_EXIT();
		return -1;
	}

	try {

/* Set up dual prefetch control blocks to keep multiple prefetch
   requests active at a time. Also helps to prevent the cache reader
   from becoming dedicated or locked into a single request's prefetch
   demands. */
	prf prefetch1, prefetch2;
	prefetch_init(&prefetch1, tdbb);
	prefetch_init(&prefetch2, tdbb);

	while (bcb->bcb_flags & BCB_cache_reader) {
		SLONG count = ISC_event_clear(reader_event);
		bcb->bcb_flags |= BCB_reader_active;
		bool found = false;
		SLONG starting_page = -1;
		prf* next_prefetch = &prefetch1;

		if (dbb->dbb_flags & DBB_suspend_bgio) {
			THREAD_EXIT();
			ISC_event_wait(1, &reader_event, &count, 10 * 1000000, NULL, 0);
			THREAD_ENTER();
			continue;
		}

		/* Make a pass thru the global prefetch bitmap looking for something
		   to read. When the entire bitmap has been scanned and found to be
		   empty then wait for new requests. */

		do {
			if (!(prefetch1.prf_flags & PRF_active) &&
				SBM_next(bcb->bcb_prefetch, &starting_page, RSE_get_forward)) 
			{
				found = true;
				prefetch_prologue(&prefetch1, &starting_page);
				prefetch_io(&prefetch1, status_vector);
			}

			if (!(prefetch2.prf_flags & PRF_active) &&
				SBM_next(bcb->bcb_prefetch, &starting_page, RSE_get_forward)) 
			{
				found = true;
				prefetch_prologue(&prefetch2, &starting_page);
				prefetch_io(&prefetch2, status_vector);
			}

			prf* post_prefetch = next_prefetch;
			next_prefetch =
				(post_prefetch == &prefetch1) ? &prefetch2 : &prefetch1;
			if (post_prefetch->prf_flags & PRF_active) {
				prefetch_epilogue(post_prefetch, status_vector);
			}

			if (found) {
				/* If the cache writer or garbage collector is idle, put
				   them to work prefetching pages. */
#ifdef CACHE_WRITER
				if (bcb->bcb_flags & BCB_cache_writer &&
					!(bcb->bcb_flags & BCB_writer_active))
				{
						ISC_event_post(dbb->dbb_writer_event);
				}
#endif
#ifdef GARBAGE_THREAD
				if (dbb->dbb_flags & DBB_garbage_collector &&
					!(dbb->dbb_flags & DBB_gc_active))
				{
					ISC_event_post(dbb->dbb_gc_event);
				}
#endif
			}
		} while (prefetch1.prf_flags & PRF_active
				 || prefetch2.prf_flags & PRF_active);

		/* If there's more work to do voluntarily ask to be rescheduled.
		   Otherwise, wait for event notification. */
		BufferDesc* bdb;
		if (found) {
			JRD_reschedule(tdbb, 0, true);
		}
		else if (bcb->bcb_flags & BCB_free_pending &&
				 (bdb = get_buffer(tdbb, FREE_PAGE, LATCH_none, 1)))
		{
			/* In our spare time, help writer clean the cache. */

			write_buffer(tdbb, bdb, bdb->bdb_page, true, status_vector, true);
		}
		else {
			bcb->bcb_flags &= ~BCB_reader_active;
			THREAD_EXIT();
			ISC_event_wait(1, &reader_event, &count, 10 * 1000000, NULL, 0);
			THREAD_ENTER();
		}
		bcb = dbb->dbb_bcb;
	}

	LCK_fini(tdbb, LCK_OWNER_attachment);
	delete tdbb->tdbb_attachment;
	bcb->bcb_flags &= ~BCB_cache_reader;
	ISC_event_post(reader_event);
	THREAD_EXIT();

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(status_vector, ex);
		bcb = dbb->dbb_bcb;
		gds__log_status(dbb->dbb_file->fil_string, status_vector);
	}
	return 0;
}
#endif


#ifdef CACHE_WRITER
static THREAD_ENTRY_DECLARE cache_writer(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	c a c h e _ w r i t e r
 *
 **************************************
 *
 * Functional description
 *	Write dirty pages to database to maintain an
 *	adequate supply of free pages. If WAL is enabled,
 *	perform database checkpoint when WAL subsystem
 *	deems it necessary.
 *
 **************************************/
	Database* dbb = (Database*)arg;
	THREAD_ENTER();

/* Establish a thread context. */
/* Note: Since this function operates as its own thread,
   we have no need to restore the THREAD CONTEXT on exit.
   Once we reach the end, the thread will die, thus implicitly
   killing all its contexts. */
	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	ISC_STATUS_ARRAY status_vector;
/* Dummy attachment needed for lock owner identification. */

	tdbb->tdbb_database = dbb;
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_bufferpool);
	tdbb->tdbb_status_vector = status_vector;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_attachment = FB_NEW(*dbb->dbb_bufferpool) Attachment(dbb);
	tdbb->tdbb_attachment->att_filename = dbb->dbb_filename;

/* This try block is specifically to protect the LCK_init call: if
   LCK_init fails we won't be able to accomplish anything anyway, so
   return, unlike the other try blocks further down the page. */
	event_t* writer_event = 0;
	BufferControl* bcb = 0;

	try {
		writer_event = dbb->dbb_writer_event;
		ISC_event_init(writer_event, 0, 0);
		LCK_init(tdbb, LCK_OWNER_attachment);
		bcb = dbb->dbb_bcb;
		bcb->bcb_flags |= BCB_cache_writer;

		/* Notify our creator that we have started */
		ISC_event_post(dbb->dbb_writer_event_init);
	}
	catch (const std::exception& ex) {
		Firebird::stuff_exception(status_vector, ex);
		gds__log_status(dbb->dbb_file->fil_string, status_vector);
		ISC_event_fini(writer_event);
		THREAD_EXIT();
		return (THREAD_ENTRY_RETURN)(-1);
	}

	try {
		while (bcb->bcb_flags & BCB_cache_writer)
		{
			SLONG count = ISC_event_clear(writer_event);
			bcb->bcb_flags |= BCB_writer_active;
			SLONG starting_page = -1;

			if (dbb->dbb_flags & DBB_suspend_bgio) {
				THREAD_EXIT();
				ISC_event_wait(1, &writer_event, &count, 10 * 1000000, NULL, 0);
				THREAD_ENTER();
				continue;
			}
	
#ifdef SUPERSERVER_V2
			/* Flush buffers for lazy commit */
			SLONG commit_mask;
			if (!(dbb->dbb_flags & DBB_force_write) &&
				(commit_mask = dbb->dbb_flush_cycle))
			{
				dbb->dbb_flush_cycle = 0;
				btc_flush(tdbb, commit_mask, false, status_vector);
			}
#endif

			THREAD_EXIT();
			THREAD_YIELD();
			THREAD_ENTER();

			if (bcb->bcb_flags & BCB_free_pending) {
				BufferDesc* bdb = get_buffer(tdbb, FREE_PAGE, LATCH_none, 1);
				if (bdb) {
					write_buffer(tdbb, bdb, bdb->bdb_page, true, status_vector, true);
					bcb = dbb->dbb_bcb;
				}

				/* If the cache reader or garbage collector is idle, put
				   them to work freeing pages. */
#ifdef CACHE_READER
				if (bcb->bcb_flags & BCB_cache_reader &&
					!(bcb->bcb_flags & BCB_reader_active))
				{
						ISC_event_post(dbb->dbb_reader_event);
				}
#endif
#ifdef GARBAGE_THREAD
				if (dbb->dbb_flags & DBB_garbage_collector &&
					!(dbb->dbb_flags & DBB_gc_active))
				{
						ISC_event_post(dbb->dbb_gc_event);
				}
#endif
			}

			/* If there's more work to do voluntarily ask to be rescheduled.
			   Otherwise, wait for event notification. */

			if ((bcb->bcb_flags & BCB_free_pending) ||
				bcb->bcb_checkpoint || dbb->dbb_flush_cycle)
			{
				JRD_reschedule(tdbb, 0, true);
			}
#ifdef CACHE_READER
			else if (SBM_next(bcb->bcb_prefetch, &starting_page, RSE_get_forward))
			{
				/* Prefetch some pages in our spare time and in the process
				   garbage collect the prefetch bitmap. */
				prf prefetch;

				prefetch_init(&prefetch, tdbb);
				prefetch_prologue(&prefetch, &starting_page);
				prefetch_io(&prefetch, status_vector);
				prefetch_epilogue(&prefetch, status_vector);
			}
#endif
			else {
				bcb->bcb_flags &= ~BCB_writer_active;
				THREAD_EXIT();
				ISC_event_wait(1, &writer_event, &count, 10 * 1000000, NULL, 0);
				THREAD_ENTER();
			}
			bcb = dbb->dbb_bcb;
		}

		LCK_fini(tdbb, LCK_OWNER_attachment);
		delete tdbb->tdbb_attachment;
		bcb->bcb_flags &= ~BCB_cache_writer;
		/* Notify the finalization caller that we're finishing. */
		ISC_event_post(dbb->dbb_writer_event_fini);
		ISC_event_fini(writer_event);
		THREAD_EXIT();

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(status_vector, ex);
		bcb = dbb->dbb_bcb;
		gds__log_status(dbb->dbb_file->fil_string, status_vector);
	}
	return 0;
}
#endif


static void check_precedence(thread_db* tdbb, WIN * window, SLONG page)
{
/**************************************
 *
 *	c h e c k _ p r e c e d e n c e
 *
 **************************************
 *
 * Functional description
 *	Given a window accessed for write and a page number,
 *	establish a precedence relationship such that the
 *	specified page will always be written before the page
 *	associated with the window.
 *
 *	If the "page number" is negative, it is really a transaction
 *	id.  In this case, the precedence relationship is to the
 *	database header page from which the transaction id was
 *	obtained.  If the header page has been written since the
 *	transaction id was assigned, no precedence relationship
 *	is required.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

/* If this is really a transaction id, sort things out */

	if (page < 0) {
		if (-page <= dbb->dbb_last_header_write) {
			return;
		}
		else {
			page = 0;
		}
	}

/* Start by finding the buffer containing the high priority page */

//	BCB_MUTEX_ACQUIRE;
//	PRE_MUTEX_ACQUIRE;
	BufferControl* bcb = dbb->dbb_bcb;
	QUE mod_que = &bcb->bcb_rpt[page % bcb->bcb_count].bcb_page_mod;

	BufferDesc* high = 0;
	QUE que_inst;
	for (que_inst = mod_que->que_forward; que_inst != mod_que; que_inst = que_inst->que_forward) {
		if ((high = BLOCK(que_inst, BufferDesc*, bdb_que))->bdb_page == page) {
			break;
		}
	}

//	BCB_MUTEX_RELEASE;
	if (que_inst == mod_que) {
//		PRE_MUTEX_RELEASE;
		return;
	}

/* Found the higher precedence buffer.  If it's not dirty, don't sweat it.
   If it's the same page, ditto.  */

	if (!(high->bdb_flags & BDB_dirty)
		|| (high->bdb_page == window->win_page))
	{
//		PRE_MUTEX_RELEASE;
		return;
	}

	BufferDesc* low = window->win_bdb;

	if ((low->bdb_flags & BDB_marked) && !(low->bdb_flags & BDB_faked))
		cache_bugcheck(212);	/* msg 212 CCH_precedence: block marked */

/* If already related, there's nothing more to do. If the precedence
   search was too complex to complete, just write the high page and
   forget about about establishing the relationship. */

	if (QUE_NOT_EMPTY(high->bdb_lower)) {
		const SSHORT relationship = related(low, high, PRE_SEARCH_LIMIT);
		if (relationship == PRE_EXISTS) {
//			PRE_MUTEX_RELEASE;
			return;
		}
		else if (relationship == PRE_UNKNOWN) {
			const SLONG high_page = high->bdb_page;
//			PRE_MUTEX_RELEASE;
			if (!write_buffer
				(tdbb, high, high_page, false, tdbb->tdbb_status_vector, true))
			{
				CCH_unwind(tdbb, true);
			}
			return;
		}
	}

/* Check to see if we're going to create a cycle or the precedence search
   was too complex to complete.  If so, force a write of the "after"
   (currently fetched) page.  Assuming everyone obeys the rules and calls
   precedence before marking the buffer, everything should be ok */

	if (QUE_NOT_EMPTY(low->bdb_lower)) {
		const SSHORT relationship = related(high, low, PRE_SEARCH_LIMIT);
		if (relationship == PRE_EXISTS || relationship == PRE_UNKNOWN) {
			const SLONG low_page = low->bdb_page;
//			PRE_MUTEX_RELEASE;
			if (!write_buffer
				(tdbb, low, low_page, false, tdbb->tdbb_status_vector, true))
			{
				CCH_unwind(tdbb, true);
			}
//			PRE_MUTEX_ACQUIRE;
		}
	}

/* We're going to establish a new precedence relationship.  Get a block,
   fill in the appropriate fields, and insert it into the various ques */

	bcb = dbb->dbb_bcb;			/* Re-initialize */

	Precedence* precedence = bcb->bcb_free;
	if (precedence) {
		bcb->bcb_free = (Precedence*) precedence->pre_hi;
	}
	else {
		precedence = FB_NEW(*dbb->dbb_bufferpool) Precedence;
	}

	precedence->pre_low = low;
	precedence->pre_hi = high;
	precedence->pre_flags = 0;
	QUE_INSERT(low->bdb_higher, precedence->pre_higher);
	QUE_INSERT(high->bdb_lower, precedence->pre_lower);
//	PRE_MUTEX_RELEASE;
}



static void clear_precedence(Database* dbb, BufferDesc* bdb)
{
/**************************************
 *
 *	c l e a r _ p r e c e d e n c e
 *
 **************************************
 *
 * Functional description
 *	Clear precedence relationships to lower precedence block.
 *
 **************************************/
	SET_DBB(dbb);

//	PRE_MUTEX_ACQUIRE;
	BufferControl* bcb = dbb->dbb_bcb;

/* Loop thru lower precedence buffers.  If any can be downgraded,
   by all means down grade them. */

	while (QUE_NOT_EMPTY(bdb->bdb_lower)) {
		QUE que_inst = bdb->bdb_lower.que_forward;
		Precedence* precedence = BLOCK(que_inst, Precedence*, pre_lower);
		BufferDesc* low_bdb = precedence->pre_low;
		QUE_DELETE(precedence->pre_higher);
		QUE_DELETE(precedence->pre_lower);
		precedence->pre_hi = (BufferDesc*) bcb->bcb_free;
		bcb->bcb_free = precedence;
		if (!(precedence->pre_flags & PRE_cleared)) {
			if (low_bdb->bdb_ast_flags & BDB_blocking)
			{
				PAGE_LOCK_RE_POST(low_bdb->bdb_lock);
			}
		}
	}

//	PRE_MUTEX_RELEASE;
}


static BufferDesc* dealloc_bdb(BufferDesc* bdb)
{
/**************************************
 *
 *	d e a l l o c _ b d b
 *
 **************************************
 *
 * Functional description
 *	Deallocate buffer descriptor block.
 *
 **************************************/
	if (bdb) {
#ifndef PAGE_LATCHING
		if (bdb->bdb_lock) {
			delete bdb->bdb_lock;
		}
#endif
		QUE_DELETE(bdb->bdb_que);
		delete bdb;
	}

	return 0;
}


#ifndef PAGE_LATCHING
// CVC: Nobody was interested in the result from this function, so I made it
// void instead of bool, but preserved the returned values in comments.
static void down_grade(thread_db* tdbb, BufferDesc* bdb)
{
/**************************************
 *
 *	d o w n _ g r a d e
 *
 **************************************
 *
 * Functional description
 *	A lock on a page is blocking another process.  If possible, downgrade
 *	the lock on the buffer.  This may be called from either AST or
 *	regular level.  Return true if the down grade was successful.  If the
 *	down grade was deferred for any reason, return false.
 *
 **************************************/
	SET_TDBB(tdbb);

	bdb->bdb_ast_flags |= BDB_blocking;
	Lock* lock = bdb->bdb_lock;
	Database* dbb = bdb->bdb_dbb;

	if (dbb->dbb_flags & DBB_bugcheck) {
		PAGE_LOCK_RELEASE(bdb->bdb_lock);
		bdb->bdb_ast_flags &= ~BDB_blocking;
		// Release backup pages lock as buffer is no longer dirty
		if (bdb->bdb_flags & BDB_dirty) {
			bdb->bdb_flags &= ~BDB_dirty;
			set_write_direction(dbb, bdb, BDB_write_undefined);
		}
		return; // true;
	}

/* If the BufferDesc is in use and, being written or already
   downgraded to read, mark it as blocking and exit. */

	if (bdb->bdb_use_count) {
		return; // false;
	}

	latch_bdb(tdbb, LATCH_io, bdb, bdb->bdb_page, 1);

/* If the page isn't dirty, the lock can be quietly downgraded. */

	if (!(bdb->bdb_flags & BDB_dirty)) {
		bdb->bdb_ast_flags &= ~BDB_blocking;
#ifdef VMS
		if (lock->lck_logical == LCK_write) {
			LCK_convert(tdbb, lock, LCK_read, LCK_WAIT);
		}
		else {
			PAGE_LOCK_RELEASE(bdb->bdb_lock);
		}
#else
		LCK_downgrade(tdbb, lock);
#endif
		release_bdb(tdbb, bdb, false, false, false);
		return; // true;
	}

	bool in_use = false, invalid = false;

	if (bdb->bdb_flags & BDB_not_valid) {
		invalid = true;
	}

/* If there are higher precedence guys, see if they can be written. */
	QUE que_inst;
	for (que_inst = bdb->bdb_higher.que_forward; que_inst != &bdb->bdb_higher;
		 que_inst = que_inst->que_forward)
	{
		Precedence* precedence = BLOCK(que_inst, Precedence*, pre_higher);
		if (precedence->pre_flags & PRE_cleared)
			continue;
		if (invalid) {
			precedence->pre_flags |= PRE_cleared;
			continue;
		}
		BufferDesc* blocking_bdb = precedence->pre_hi;
		if (blocking_bdb->bdb_flags & BDB_dirty) {
			down_grade(tdbb, blocking_bdb);
			if (blocking_bdb->bdb_flags & BDB_dirty) {
				in_use = true;
			}
			if (blocking_bdb->bdb_flags & BDB_not_valid) {
				invalid = true;
				in_use = false;
				que_inst = bdb->bdb_higher.que_forward;
			}
		}
	}

/* If any higher precedence buffer can't be written, mark this buffer
   as blocking and exit. */

	if (in_use) {
		release_bdb(tdbb, bdb, false, false, false);
		return; // false;
	}

/* Everything is clear to write this buffer.  Do so and reduce the lock */

	if (invalid
		|| !write_page(tdbb, bdb, false, tdbb->tdbb_status_vector, true))
	{
		bdb->bdb_flags |= BDB_not_valid;
		// Release backup pages lock
		bdb->bdb_flags &= ~BDB_dirty;
		set_write_direction(dbb, bdb, BDB_write_undefined);
		bdb->bdb_ast_flags &= ~BDB_blocking;
		TRA_invalidate(dbb, bdb->bdb_transactions);
		bdb->bdb_transactions = 0;
		PAGE_LOCK_RELEASE(bdb->bdb_lock);
	}
	else {
		bdb->bdb_ast_flags &= ~BDB_blocking;
#ifdef VMS
		LCK_convert(tdbb, lock, LCK_read, LCK_WAIT);
#else
		LCK_downgrade(tdbb, lock);
#endif
	}

/* Clear precedence relationships to lower precedence buffers.  Since it
   isn't safe to tweak the que pointers from AST level, just mark the
   precedence links as cleared.  Somebody else will clean up the precedence
   blocks. */

	for (que_inst = bdb->bdb_lower.que_forward; que_inst != &bdb->bdb_lower;
		 que_inst = que_inst->que_forward)
	{
		Precedence* precedence = BLOCK(que_inst, Precedence*, pre_lower);
		BufferDesc* blocking_bdb = precedence->pre_low;
		if (bdb->bdb_flags & BDB_not_valid)
			blocking_bdb->bdb_flags |= BDB_not_valid;
		precedence->pre_flags |= PRE_cleared;
		if (blocking_bdb->bdb_flags & BDB_not_valid ||
			blocking_bdb->bdb_ast_flags & BDB_blocking)
		{
			down_grade(tdbb, blocking_bdb);
		}
	}

	bdb->bdb_flags &= ~BDB_not_valid;
	release_bdb(tdbb, bdb, false, false, false);

	return; // true;
}
#endif


static void expand_buffers(thread_db* tdbb, ULONG number)
{
/**************************************
 *
 *	e x p a n d _ b u f f e r s
 *
 **************************************
 *
 * Functional description
 *	Expand the cache to at least a given number of buffers.  If
 *	it's already that big, don't do anything.
 *
 * Nickolay Samofatov, 08-Mar-2004.
 *  This function does not handle exceptions correctly,
 *  it looks like good handling requires rewrite.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	BufferControl* old = dbb->dbb_bcb;

	if (number <= old->bcb_count || number > MAX_PAGE_BUFFERS) {
		return;
	}

/* for Win16 platform, we want to ensure that no cache buffer ever
   ends on a segment boundary */

	ULONG num_per_seg = number - old->bcb_count;
	ULONG left_to_do = num_per_seg;

/* Allocate and initialize buffers control block */
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_bufferpool);

	old = dbb->dbb_bcb;
	const bcb_repeat* const old_end = old->bcb_rpt + old->bcb_count;

	BufferControl* new_block =
		FB_NEW_RPT(*dbb->dbb_bufferpool, number) 
		BufferControl(*dbb->dbb_bufferpool);
	new_block->bcb_count = number;
	new_block->bcb_free_minimum = (SSHORT) MIN(number / 4, 128);	/* 25% clean page reserve */
	new_block->bcb_checkpoint = old->bcb_checkpoint;
	new_block->bcb_flags = old->bcb_flags;
	const bcb_repeat* const new_end = new_block->bcb_rpt + number;

/* point at the dirty page binary tree */

	new_block->bcb_btree = old->bcb_btree;

/* point at the free precedence blocks */

	new_block->bcb_free = old->bcb_free;

/* position the new bcb in the in use, empty and latch queues */

	QUE_INSERT(old->bcb_in_use, new_block->bcb_in_use);
	QUE_DELETE(old->bcb_in_use);
	QUE_INSERT(old->bcb_empty, new_block->bcb_empty);
	QUE_DELETE(old->bcb_empty);
	QUE_INSERT(old->bcb_free_lwt, new_block->bcb_free_lwt);
	QUE_DELETE(old->bcb_free_lwt);

/* Copy addresses of previously allocated buffer space to new block */

	for (Jrd::UCharStack::iterator stack(old->bcb_memory); stack.hasData(); ++stack) {
		new_block->bcb_memory.push(stack.object());
	}

/* Initialize tail of new buffer control block */
	bcb_repeat* new_tail;
	for (new_tail = new_block->bcb_rpt; new_tail < new_end; new_tail++) {
		QUE_INIT(new_tail->bcb_page_mod);
	}

/* Move any active buffers from old block to new */

	new_tail = new_block->bcb_rpt;

	for (bcb_repeat* old_tail = old->bcb_rpt; old_tail < old_end;
		old_tail++, new_tail++)
	{
		new_tail->bcb_bdb = old_tail->bcb_bdb;
		while (QUE_NOT_EMPTY(old_tail->bcb_page_mod)) {
			QUE que_inst = old_tail->bcb_page_mod.que_forward;
			BufferDesc* bdb = BLOCK(que_inst, BufferDesc*, bdb_que);
			QUE_DELETE((*que_inst));
			QUE mod_que =
				&new_block->bcb_rpt[bdb->bdb_page % new_block->bcb_count].bcb_page_mod;
			QUE_INSERT((*mod_que), (*que_inst));
		}
	}

/* Allocate new buffer descriptor blocks */

	ULONG num_in_seg = 0;
	UCHAR* memory = 0;
	for (; new_tail < new_end; new_tail++) {
		/* if current segment is exhausted, allocate another */

		if (!num_in_seg) {
			memory = (UCHAR *)gds__alloc((SLONG) dbb->dbb_page_size *
										 (num_per_seg + 1));
			// NOMEM: crash!
			new_block->bcb_memory.push(memory);
			memory = (UCHAR *) (((U_IPTR) memory + dbb->dbb_page_size - 1) &
								~((int) dbb->dbb_page_size - 1));
			num_in_seg = num_per_seg;
			left_to_do -= num_per_seg;
			if (num_per_seg > left_to_do) {
				num_per_seg = left_to_do;
			}
		}
		new_tail->bcb_bdb = alloc_bdb(tdbb, new_block, &memory);
		num_in_seg--;
	}

/* Set up new buffer control, release old buffer control, and clean up */

	dbb->dbb_bcb = new_block;

	delete old;
}


static BufferDesc* get_buffer(thread_db* tdbb, SLONG page, LATCH latch, SSHORT latch_wait)
{
/**************************************
 *
 *	g e t _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Get a buffer.  If possible, get a buffer already assigned
 *	to the page.  Otherwise get one from the free list or pick
 *	the least recently used buffer to be reused.
 *	Note the following special page numbers:
 *	     -1 indicates that a buffer is required for journaling => obsolete
 *	     -2 indicates a special scratch buffer for shadowing
 *
 * input
 *	page:		page to get
 *	latch:		type of latch to acquire on the page.
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				give up and return 0;
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	BufferDesc pointer if successful.
 *	NULL pointer if timeout occurred (only possible is latch_wait <> 1).
 *		     if cache manager doesn't have any pages to write anymore.
 *
 **************************************/
	// CVC: Those two vars are tricky or nonsense to put in minimal scope.
	QUE que_inst;
	BufferControl* bcb;

	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;
	SSHORT walk = dbb->dbb_bcb->bcb_free_minimum;

//	BCB_MUTEX_ACQUIRE;

	while (true) {
	  find_page:

		bcb = dbb->dbb_bcb;
		if (page >= 0) {
			/* Check to see if buffer has already been assigned to page */

			QUE mod_que = &bcb->bcb_rpt[page % bcb->bcb_count].bcb_page_mod;
			for (que_inst = mod_que->que_forward; que_inst != mod_que;
				 que_inst = que_inst->que_forward) 
			{
				BufferDesc* bdb = BLOCK(que_inst, BufferDesc*, bdb_que);
				if (bdb->bdb_page == page) {
#ifdef SUPERSERVER_V2
					if (page != HEADER_PAGE)
#endif
						QUE_MOST_RECENTLY_USED(bdb->bdb_in_use);
//					BCB_MUTEX_RELEASE;
					const SSHORT latch_return =
						latch_bdb(tdbb, latch, bdb, page, latch_wait);
					if (latch_return) {
						if (latch_return == 1) {
							return NULL;	/* permitted timeout happened */
						}
//						BCB_MUTEX_ACQUIRE;
						goto find_page;
					}
					else {
						bdb->bdb_flags &= ~(BDB_faked | BDB_prefetch);
						dbb->dbb_fetches++;
						return bdb;
					}
				}
			}
		}
#ifdef CACHE_WRITER
		else if ((page == FREE_PAGE) || (page == CHECKPOINT_PAGE)) {
			/* This code is only used by the background I/O threads:
			   cache writer, cache reader and garbage collector. */

			THREAD_EXIT();

			for (que_inst = bcb->bcb_in_use.que_backward;
				 que_inst != &bcb->bcb_in_use; que_inst = que_inst->que_backward)
			{
				BufferDesc* bdb = BLOCK(que_inst, BufferDesc*, bdb_in_use);
				if (page == FREE_PAGE) {
					if (bdb->bdb_use_count ||
						bdb->bdb_flags & BDB_free_pending)
					{
						continue;
					}
					if (bdb->bdb_flags & BDB_db_dirty) {
						THREAD_ENTER();
//						BCB_MUTEX_RELEASE;
						return bdb;
					}
					if (!--walk) {
						bcb->bcb_flags &= ~BCB_free_pending;
						break;
					}
				}
				else {			/* if (page == CHECKPOINT_PAGE) */

					if (bdb->bdb_flags & BDB_checkpoint) {
						THREAD_ENTER();
//						BCB_MUTEX_RELEASE;
						return bdb;
					}
				}
			}

			if (page == FREE_PAGE) {
				bcb->bcb_flags &= ~BCB_free_pending;
			}

			THREAD_ENTER();
//			BCB_MUTEX_RELEASE;
			return NULL;
		}
#endif

		for (que_inst = bcb->bcb_in_use.que_backward;
			 que_inst != &bcb->bcb_in_use || QUE_NOT_EMPTY(bcb->bcb_empty);
			 que_inst = que_inst->que_backward)
		{
			bcb = dbb->dbb_bcb;	/* Re-initialize in the loop */
			QUE mod_que = &bcb->bcb_rpt[page % bcb->bcb_count].bcb_page_mod;

			/* If there is an empty buffer sitting around, allocate it */

			if (QUE_NOT_EMPTY(bcb->bcb_empty)) {
				que_inst = bcb->bcb_empty.que_forward;
				QUE_DELETE((*que_inst));
				BufferDesc* bdb = BLOCK(que_inst, BufferDesc*, bdb_que);
				if (page >= 0) {
					QUE_INSERT((*mod_que), (*que_inst));
#ifdef SUPERSERVER_V2
					/* Reserve a buffer for header page with deferred header
					   page write mechanism. Otherwise, a deadlock will occur
					   if all dirty pages in the cache must force header page
					   to disk before they can be written but there is no free
					   buffer to read the header page into. */

					if (page != HEADER_PAGE)
#endif
						QUE_INSERT(bcb->bcb_in_use, bdb->bdb_in_use);
				}

				/* This correction for bdb_use_count below is needed to
				   avoid a deadlock situation in latching code.  It's not
				   clear though how the bdb_use_count can get < 0 for a bdb
				   in bcb_empty queue */
				if (bdb->bdb_use_count < 0) {
					cache_bugcheck(301);	/* msg 301 Non-zero use_count of a buffer in the empty que_inst */
				}

				bdb->bdb_page = page;
				bdb->bdb_flags = BDB_read_pending;
				bdb->bdb_scan_count = 0;
				/* The following latch should never fail because the buffer is 'empty'
				   and the page is not in cache. */
				if (latch_bdb(tdbb, latch, bdb, page, -100) == -1) {
					cache_bugcheck(302);	/* msg 302 unexpected page change */
				}
#ifndef PAGE_LATCHING
				if (page >= 0) {
					bdb->bdb_lock->lck_logical = LCK_none;
				}
				else {
					PAGE_LOCK_RELEASE(bdb->bdb_lock);
				}
#endif
				dbb->dbb_fetches++;
//				BCB_MUTEX_RELEASE;
				return bdb;
			}

			/* get the oldest buffer as the least recently used -- note
			   that since there are no empty buffers this queue cannot be empty */

			if (bcb->bcb_in_use.que_forward == &bcb->bcb_in_use) {
				cache_bugcheck(213);	/* msg 213 insufficient cache size */
			}

			BufferDesc* oldest = BLOCK(que_inst, BufferDesc*, bdb_in_use);
//			LATCH_MUTEX_ACQUIRE;
			if (oldest->bdb_use_count
				|| (oldest->bdb_flags & BDB_free_pending)
				|| !writeable(oldest))
			{
//				LATCH_MUTEX_RELEASE;
				continue;
			}

#ifdef SUPERSERVER_V2
			/* If page has been prefetched but not yet fetched, let
			   it cycle once more thru LRU queue before re-using it. */

			if (oldest->bdb_flags & BDB_prefetch) {
				oldest->bdb_flags &= ~BDB_prefetch;
				que_inst = que_inst->que_forward;
				QUE_MOST_RECENTLY_USED(oldest->bdb_in_use);
//				LATCH_MUTEX_RELEASE;
				continue;
			}
#endif
//			LATCH_MUTEX_RELEASE;
#ifdef CACHE_WRITER
			if (oldest->bdb_flags & (BDB_dirty | BDB_db_dirty)) {
				bcb->bcb_flags |= BCB_free_pending;
				if (bcb->bcb_flags & BCB_cache_writer &&
					!(bcb->bcb_flags & BCB_writer_active))
				{
					ISC_event_post(dbb->dbb_writer_event);
				}
				if (walk) {
					if (!--walk) {
						break;
					}
					else {
						continue;
					}
				}
			}
#endif
			BufferDesc* bdb = oldest;
			if (latch_bdb(tdbb, LATCH_exclusive, bdb, bdb->bdb_page, 0)) {
				continue;		/* BufferDesc changed, continue looking for a buffer */
			}
			QUE_MOST_RECENTLY_USED(bdb->bdb_in_use);
			bdb->bdb_flags |= BDB_free_pending;

			/* If the buffer selected is dirty, arrange to have it written. */

			if (bdb->bdb_flags & (BDB_dirty | BDB_db_dirty)) {
//				BCB_MUTEX_RELEASE;
#ifdef SUPERSERVER
				if (!write_buffer
					(tdbb, bdb, bdb->bdb_page, true, tdbb->tdbb_status_vector,
					 true))
#else
				if (!write_buffer
					(tdbb, bdb, bdb->bdb_page, false,
					 tdbb->tdbb_status_vector, true))
#endif
				{
					bdb->bdb_flags &= ~BDB_free_pending;
					release_bdb(tdbb, bdb, false, false, false);
					CCH_unwind(tdbb, true);
				}
			}

			bcb = dbb->dbb_bcb;	/* Re-initialize */

			/* If the buffer is still in the dirty tree, remove it.
			   In any case, release any lock it may have. */

			if (bdb->bdb_parent || (bdb == bcb->bcb_btree)) {
				btc_remove(bdb);
			}

			/* if the page has an expanded index buffer, release it */

			if (bdb->bdb_expanded_buffer) {
				delete bdb->bdb_expanded_buffer;
				bdb->bdb_expanded_buffer = NULL;
			}

			/* Cleanup any residual precedence blocks.  Unless something is
			   screwed up, the only precedence blocks that can still be hanging
			   around are ones cleared at AST level. */

//			PRE_MUTEX_ACQUIRE;
			while (QUE_NOT_EMPTY(bdb->bdb_higher)) {
				QUE que2 = bdb->bdb_higher.que_forward;
				Precedence* precedence = BLOCK(que2, Precedence*, pre_higher);
				QUE_DELETE(precedence->pre_higher);
				QUE_DELETE(precedence->pre_lower);
				precedence->pre_hi = (BufferDesc*) bcb->bcb_free;
				bcb->bcb_free = precedence;
			}
//			PRE_MUTEX_RELEASE;

			clear_precedence(dbb, bdb);

			/* remove the buffer from the "mod" queue and place it
			   in it's new spot, provided it's not a negative (scratch) page */

//			BCB_MUTEX_ACQUIRE;
			if (bdb->bdb_page >= 0) {
				QUE_DELETE(bdb->bdb_que);
			}
			QUE_INSERT(bcb->bcb_empty, bdb->bdb_que);
			QUE_DELETE(bdb->bdb_in_use)

			bdb->bdb_page = JOURNAL_PAGE;
			release_bdb(tdbb, bdb, false, false, false);
			break;
		}

		if (que_inst == &bcb->bcb_in_use) {
#ifdef SUPERSERVER
			expand_buffers(tdbb, bcb->bcb_count + 75);
#else
			cache_bugcheck(214);	/* msg 214 no cache buffers available for reuse */
#endif
		}
	}
}


#ifdef PAGE_LATCHING
static SSHORT latch_bdb(
						thread_db* tdbb,
						LATCH type, BufferDesc* bdb, SLONG page, SSHORT latch_wait)
{
/**************************************
 *
 *	l a t c h _ b d b
 *
 **************************************
 *
 * Functional description
 *
 * input
 *	type:		LATCH_none, LATCH_exclusive, LATCH_io, LATCH_shared, or LATCH_mark.
 *	bdb:		object to acquire latch on.
 *	page:		page of bdb, for verification.
 *	latch_wait:	1 => Wait as long as necessary to get the latch.
 *				This can cause deadlocks of course.
 *			0 => If the latch can't be acquired immediately,
 *				give up and return 1.
 *	      		<negative number> => Latch timeout interval in seconds.
 *
 * return
 *	0:	latch successfully acquired.
 *	1:	latch not acquired due to a (permitted) timeout.
 *	-1:	latch not acquired, bdb doesn't corresponds to page.
 *
 **************************************/

/* If the buffer has been reassigned to another page
   make the caller deal with it. */

	if (bdb->bdb_page != page) {
		return -1;
	}

//	LATCH_MUTEX_ACQUIRE;

/* Handle the easy case first, no users of the buffer. */

	if (!bdb->bdb_use_count) {
		switch (type) {
		case LATCH_shared:
			++bdb->bdb_use_count;
			bdb->bdb_shared[0] = tdbb;
			break;
		case LATCH_exclusive:
			++bdb->bdb_use_count;
			bdb->bdb_exclusive = tdbb;
			break;
		case LATCH_io:
			++bdb->bdb_use_count;
			bdb->bdb_io = tdbb;
			break;
		case LATCH_mark:
			cache_bugcheck(295);	/* inconsistent LATCH_mark call */
			break;
		case LATCH_none:
			break;
		}
//		LATCH_MUTEX_RELEASE;
		return 0;
	}

/* Grant the latch request if it is compatible with existing
   latch holders.  Pending latches are queued in the order in
   which they are requested except that io/mark latches are queued
   ahead of all other latch requests (to avoid deadlocks and
   this does not cause starvation).  Also, shared latches are granted
   immediately if a disk write is in progress.
   Note that asking for a higher mode latch when already holding a
   share latch results in deadlock.  CCH_handoff routinely acquires a
   shared latch while owning already a shared latch on the page
   (the case of handing off to the same page).  If the BDB_must_write
   flag is set, then an exclusive latch request will be followed by
   an io latch request. */

	SSHORT i;
	
	switch (type) {

	case LATCH_none:
//		LATCH_MUTEX_RELEASE;
		return 0;

	case LATCH_shared:
		if (bdb->bdb_flags & BDB_read_pending) {
			break;
		}
		if (bdb->bdb_exclusive) {
			if (bdb->bdb_exclusive != tdbb) {
				break;			/* someone else owns exclusive latch */
			}
		}
		else {
			/* Note that InterBase often 'hands-off' to the same page, for both
			   shared and exlusive latches. */
			/* Check is we own already an exclusive latch. */
			for (i = 0; (i < BDB_max_shared) && (bdb->bdb_shared[i] != tdbb);
				 i++);
			if (i >= BDB_max_shared) {	/* we don't own a shared latch yet */
				/* If there are latch-waiters, and they are not waiting for an
				   io_latch, then we have to wait also (there must be a exclusive
				   latch waiter).  If there is an IO in progress, the violate the
				   fairness and sneak ahead of the exclusive (or io) waiters. */

				if ((QUE_NOT_EMPTY(bdb->bdb_waiters)) && !bdb->bdb_io) {
					break;		/* be fair and wait behind exclusive latch requests */
				}
			}
		}
		/* Nobody owns an exlusive latch, or sneak ahead of exclusive latch
		   waiters while an io is in progress. */

		for (i = 0; (i < BDB_max_shared) && bdb->bdb_shared[i]; i++); // empty loop body
		if (i >= BDB_max_shared) {
			break;
		}
		++bdb->bdb_use_count;
		bdb->bdb_shared[i] = tdbb;
//		LATCH_MUTEX_RELEASE;
		return 0;

	case LATCH_io:
		if (bdb->bdb_flags & BDB_read_pending) {
			break;
		}
		if (bdb->bdb_io) {
			break;				/* someone else owns the io latch */
		}
		++bdb->bdb_use_count;
		bdb->bdb_io = tdbb;
//		LATCH_MUTEX_RELEASE;
		return 0;

	case LATCH_exclusive:
		/* Exclusive latches wait for existing shared latches and
		   (unfortunately) for existing io latches.  This is not as
		   bad as it sounds because an exclusive latch is typically followed
		   by a mark latch, which then would wait behind the io latch. */
		/* Note that the ail-code latches the same buffer multiple times
		   in shared and exclusive */
		/* Note that InterBase often 'hands-off' to the same page, for both
		   shared and exlusive latches. */
		if (bdb->bdb_use_count && (bdb->bdb_exclusive != tdbb)) {
			break;
		}
		++bdb->bdb_use_count;
		bdb->bdb_exclusive = tdbb;
//		LATCH_MUTEX_RELEASE;
		return 0;

	case LATCH_mark:
		if (bdb->bdb_exclusive != tdbb) {
			cache_bugcheck(295);	/* inconsistent LATCH_mark call */
		}
		/* Some InterBase code marks a buffer more than once. */
		if (bdb->bdb_io && (bdb->bdb_io != tdbb)) {
			break;
		}
		bdb->bdb_io = tdbb;
//		LATCH_MUTEX_RELEASE;
		return 0;

	default:
		break;
	}

/* If the caller doesn't want to wait for this latch, then return now. */
	if (latch_wait == 0) {
//		LATCH_MUTEX_RELEASE;
		return 1;
	}

/* Get or create a latch wait block and wait for someone to grant
   the latch. */

	Database* dbb = tdbb->tdbb_database;
	BufferControl* bcb = dbb->dbb_bcb;

	LatchWait* lwt;
	if (QUE_NOT_EMPTY(bcb->bcb_free_lwt)) {
		QUE que_inst = bcb->bcb_free_lwt.que_forward;
		QUE_DELETE((*que_inst));
		lwt = (LatchWait*) BLOCK(que_inst, LatchWait*, lwt_waiters);
	}
	else {
		lwt = FB_NEW(*dbb->dbb_bufferpool) LatchWait;
		QUE_INIT(lwt->lwt_waiters);
		ISC_event_init(&lwt->lwt_event, 0, 0);
	}

	lwt->lwt_flags |= LWT_pending;
	lwt->lwt_latch = type;
	lwt->lwt_tdbb = tdbb;

/* Give priority to IO.  This might prevent deadlocks while performing
   precedence writes.  This does not cause starvation because an
   exclusive latch is needed to dirty the page again. */
	if ((type == LATCH_io) || (type == LATCH_mark)) {
		QUE_INSERT(bdb->bdb_waiters, lwt->lwt_waiters)
	}
	else {
		QUE_APPEND(bdb->bdb_waiters, lwt->lwt_waiters);
	}

	event_t* event = &lwt->lwt_event;

	int timeout_occurred = FALSE;
/* Loop until the latch is granted or until a timeout occurrs. */
	for (SLONG count = ISC_event_clear(event);
		 ((lwt->lwt_flags & LWT_pending) && !timeout_occurred);
		 count = ISC_event_clear(event))
	{
//		LATCH_MUTEX_RELEASE;
		THREAD_EXIT();
		if (latch_wait == 1) {
			timeout_occurred =
				ISC_event_wait(1, &event, &count, 120 * 1000000, NULL, event);
		}
		else {
			timeout_occurred =
				ISC_event_wait(1, &event, &count, -latch_wait * 1000000,
							   NULL, event);
		}
		THREAD_ENTER();
//		LATCH_MUTEX_ACQUIRE;
	}

	bcb = dbb->dbb_bcb;			/* Re-initialize */
	QUE_DELETE(lwt->lwt_waiters);
	QUE_INSERT(bcb->bcb_free_lwt, lwt->lwt_waiters);

/* If the latch is not granted then a timeout must have occurred. */
	if ((lwt->lwt_flags & LWT_pending) && timeout_occurred) {
//		LATCH_MUTEX_RELEASE;
		if (latch_wait == 1) {
			IBERR_build_status(tdbb->tdbb_status_vector, isc_deadlock, 0);
			CCH_unwind(tdbb, true);
		}
		else {
			return 1;
		}
	}

	if (bdb->bdb_page != page) {
//		LATCH_MUTEX_RELEASE;
		release_bdb(tdbb, bdb, true, false, false);
		return -1;
	}

//	LATCH_MUTEX_RELEASE;
	return 0;
}


#else
static SSHORT latch_bdb(
						thread_db* tdbb,
						LATCH type, BufferDesc* bdb, SLONG page, SSHORT latch_wait)
{
/**************************************
 *
 *	l a t c h _ b d b
 *
 **************************************
 *
 * Functional description
 *	Simple optimized latching for single-threaded
 *	non-SUPERSERVER platforms.
 *
 **************************************/

	++bdb->bdb_use_count;

	switch (type) {
	case LATCH_shared:
		break;
	case LATCH_exclusive:
		bdb->bdb_exclusive = tdbb;
		break;
	case LATCH_io:
		bdb->bdb_io = tdbb;
		break;
	case LATCH_mark:
		bdb->bdb_io = tdbb;
	case LATCH_none:
		--bdb->bdb_use_count;
		break;
	}

	return 0;
}
#endif


static SSHORT lock_buffer(
		thread_db* tdbb,
		BufferDesc* bdb,
		SSHORT wait,
		SCHAR page_type)
{
/**************************************
 *
 *	l o c k _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Get a lock on page for a buffer.  If the lock ever slipped
 *	below READ, indicate that the page must be read.
 *
 * input:
 *	wait: LCK_WAIT = 1	=> Wait as long a necessary to get the lock.
 *	      LCK_NO_WAIT = 0	=> If the lock can't be acquired immediately,
 *						give up and return -1.
 *	      <negative number>		=> Lock timeout interval in seconds.
 *
 * return: 0  => buffer locked, page is already in memroy.
 *	   1  => buffer locked, page needs to be read from disk.
 *	   -1 => timeout on lock occurred, see input parameter 'wait'.
 *
 **************************************/
	SET_TDBB(tdbb);
#ifdef PAGE_LATCHING
	Database* dbb = tdbb->tdbb_database;

	if (dbb->dbb_refresh_ranges && bdb->bdb_flags & BDB_writer &&
		(page_type == pag_data || page_type == pag_index))
	{
		/* This gives refresh cache ranges the potential to work with page
		 * latching by taking out a temporary page lock for notification
		 * purposes.
		 * Fix cache ranges on NetWare due to latching (bug 7199)
		 * Marion change # 18270, 13-Oct-1994
		 */

		Lock refresh;

		refresh.lck_dbb = dbb;
		refresh.lck_type = LCK_bdb;
		refresh.lck_owner_handle =
			LCK_get_owner_handle(tdbb, refresh.lck_type);
		refresh.lck_parent = dbb->dbb_lock;
		refresh.lck_length = sizeof(SLONG);
		refresh.lck_key.lck_long = bdb->bdb_page;

		if (LCK_lock_non_blocking(tdbb, &refresh, LCK_write, -1))
			LCK_release(tdbb, &refresh);
	}

	return ((bdb->bdb_flags & BDB_read_pending) ? 1 : 0);
#else
	const USHORT lock_type =
		(bdb->bdb_flags & (BDB_dirty | BDB_writer)) ? LCK_write : LCK_read;
	Lock* lock = bdb->bdb_lock;

	if (lock->lck_logical >= lock_type) {
		return 0;
	}


	TEXT errmsg[MAX_ERRMSG_LEN + 1];
	ISC_STATUS* status = tdbb->tdbb_status_vector;

	if (lock->lck_logical == LCK_none) {
		/* Prevent header and TIP pages from generating blocking AST
		   overhead. The promise is that the lock will unconditionally
		   be released when the buffer use count indicates it is safe
		   to do so. */

		if (page_type == pag_header || page_type == pag_transactions) {
			fb_assert(lock->lck_ast == blocking_ast_bdb);
			fb_assert(lock->lck_object == bdb);
			lock->lck_ast = 0;
			lock->lck_object = NULL;
		}
		else {
			fb_assert(lock->lck_ast != NULL);
		}

		lock->lck_key.lck_long = bdb->bdb_page;
		if (PAGE_LOCK_OPT(lock, lock_type, wait)) {
			if (!lock->lck_ast) {
				/* Restore blocking AST to lock block if it was swapped
				   out. Flag the BufferDesc so that the lock is released when
				   the buffer is released. */

				fb_assert(page_type == pag_header
					   || page_type == pag_transactions);
				lock->lck_ast = blocking_ast_bdb;
				lock->lck_object = bdb;
				bdb->bdb_flags |= BDB_no_blocking_ast;
			}
			return 1;
		}

		if (!lock->lck_ast) {
			fb_assert(page_type == pag_header || page_type == pag_transactions);
			lock->lck_ast = blocking_ast_bdb;
			lock->lck_object = bdb;
		}

		/* Case: a timeout was specified, or the caller didn't want to wait,
		   return the error. */

		if ((wait == LCK_NO_WAIT)
			|| ((wait < 0) && (status[1] == isc_lock_timeout)))
		{
			release_bdb(tdbb, bdb, false, false, false);
			return -1;
		}

		/* Case: lock manager detected a deadlock, probably caused by locking the
		   BufferDesc's in an unfortunate order.  Nothing we can do about it, return the
		   error, and log it to firebird.log. */

		gds__msg_format(0, JRD_BUGCHK, 215, sizeof(errmsg), errmsg,
						(TEXT *) (IPTR) bdb->bdb_page, 
						(TEXT *) (IPTR) page_type, 0, 0, 0);
		IBERR_append_status(status, isc_random, isc_arg_string,
							ERR_cstring(errmsg), 0);
		ERR_log(JRD_BUGCHK, 215, errmsg);	/* msg 215 page %ld, page type %ld lock conversion denied */

		/* CCH_unwind releases all the BufferDesc's and calls ERR_punt()
		   ERR_punt will longjump. */

		CCH_unwind(tdbb, true);
	}

/* Lock requires an upward conversion.  Sigh.  Try to get the conversion.
   If it fails, release the lock and re-seize. Save the contents of the
   status vector just in case */

	const USHORT must_read = (lock->lck_logical < LCK_read) ? 1 : 0;

	ISC_STATUS_ARRAY alt_status;
	memcpy(alt_status, tdbb->tdbb_status_vector, sizeof(alt_status));

	if (LCK_convert_opt(tdbb, lock, lock_type)) {
		return must_read;
	}
	else if (wait == LCK_NO_WAIT) {
		release_bdb(tdbb, bdb, true, false, false);
		return -1;
	}

	memcpy(tdbb->tdbb_status_vector, alt_status, sizeof(alt_status));

	if (PAGE_LOCK(lock, lock_type, wait)) {
		return 1;
	}

/* Case: a timeout was specified, or the caller didn't want to wait,
   return the error. */

	if ((wait < 0) && (status[1] == isc_lock_timeout)) 
	{
		release_bdb(tdbb, bdb, false, false, false);
		return -1;
	}

/* Case: lock manager detected a deadlock, probably caused by locking the
   BufferDesc's in an unfortunate order.  Nothing we can do about it, return the
   error, and log it to firebird.log. */

	gds__msg_format(0, JRD_BUGCHK, 216, sizeof(errmsg), errmsg,
					(TEXT*) (IPTR) bdb->bdb_page, 
					(TEXT*) (IPTR) page_type, 0, 0, 0);
	IBERR_append_status(status, isc_random, isc_arg_string,
						ERR_cstring(errmsg), 0);
	ERR_log(JRD_BUGCHK, 216, errmsg);	/* msg 216 page %ld, page type %ld lock denied */

	CCH_unwind(tdbb, true);
	return 0;					/* Added to get rid of Compiler Warning */
#endif
}


static ULONG memory_init(thread_db* tdbb, BufferControl* bcb, ULONG number)
{
/**************************************
 *
 *	m e m o r y _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize memory for the cache.
 *	Return number of buffers allocated.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	UCHAR* memory = 0;
	SLONG buffers = 0;
	const SLONG page_size = (SLONG) dbb->dbb_page_size;
	SLONG memory_size = page_size * (number + 1);

	SLONG old_buffers = 0;
	bcb_repeat* old_tail = 0;
	const UCHAR* memory_end = 0;
	bcb_repeat* tail = bcb->bcb_rpt;
	// "end" is changed inside the loop
	for (const bcb_repeat* end = tail + number; tail < end; tail++)
	{
		if (!memory) {
			/* Allocate only what is required for remaining buffers. */

			if (memory_size > (SLONG) (page_size * (number + 1))) {
				memory_size = page_size * (number + 1);
			}

			do {
				try {
					memory = (UCHAR *)gds__alloc(memory_size);
					break;
				}
				catch(const std::exception&) {
					/* Either there's not enough virtual memory or there is
					   but it's not virtually contiguous. Let's find out by
					   cutting the size in half to see if the buffers can be
					   scattered over the remaining virtual address space. */
					memory_size >>= 1;
					if (memory_size < MIN_BUFFER_SEGMENT) {
						/* Diminishing returns */
						return buffers;
					}
				}
			} while (true);

			bcb->bcb_memory.push(memory);
			memory_end = memory + memory_size;

			/* Allocate buffers on an address that is an even multiple
			   of the page size (rather the physical sector size.) This
			   is a necessary condition to support raw I/O interfaces. */
			memory = (UCHAR *) (((U_IPTR) memory + page_size - 1) &
								~((int) page_size - 1));
			old_tail = tail;
			old_buffers = buffers;
		}

		QUE_INIT(tail->bcb_page_mod);

		if (!(tail->bcb_bdb = alloc_bdb(tdbb, bcb, &memory))) {
			/* Whoops! Time to reset our expectations. Release the
			   buffer memory but use that memory size to calculate
			   a new number that takes into account the page buffer
			   overhead. Reduce this number by a 25% fudge factor to
			   leave some memory for useful work. */

			gds__free(bcb->bcb_memory.pop());
			memory = 0;
			for (bcb_repeat* tail2 = old_tail; tail2 < tail; tail2++)
			{
				tail2->bcb_bdb = dealloc_bdb(tail2->bcb_bdb);
			}
			number = memory_size / PAGE_OVERHEAD;
			number -= number >> 2;
			end = old_tail + number;
			tail = --old_tail;	/* For loop continue pops tail above */
			buffers = old_buffers;
			continue;
		}

		buffers++;				/* Allocated buffers */
		number--;				/* Remaining buffers */

		/* Check if memory segment has been exhausted. */

		if (memory + page_size > memory_end) {
			memory = 0;
		}
	}

	return buffers;
}


static void page_validation_error(thread_db* tdbb, WIN * window, SSHORT type)
{
/**************************************
 *
 *	p a g e _ v a l i d a t i o n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	We've detected a validation error on fetch.  Generally
 *	we've detected that the type of page fetched didn't match the
 *	type of page we were expecting.  Report an error and
 *	get out.
 *	This function will only be called rarely, as a page validation
 *	error is an indication of on-disk database corruption.
 *
 **************************************/
	SET_TDBB(tdbb);
	BufferDesc* bdb = window->win_bdb;
	const pag* page = bdb->bdb_buffer;

	IBERR_build_status(tdbb->tdbb_status_vector,
					   isc_db_corrupt,
					   isc_arg_string, "",
					   isc_arg_gds, isc_page_type_err,
					   isc_arg_gds, isc_badpagtyp,
					   isc_arg_number, (SLONG) bdb->bdb_page,
					   isc_arg_number, (SLONG) type,
					   isc_arg_number, (SLONG) page->pag_type, 0);
/* We should invalidate this bad buffer. */
	CCH_unwind(tdbb, true);
}


#ifdef CACHE_READER
static void prefetch_epilogue(Prefetch* prefetch, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	p r e f e t c h _ e p i l o g u e
 *
 **************************************
 *
 * Functional description
 *	Stall on asynchronous I/O completion.
 *	Move data from prefetch buffer to database
 *	buffers, compute the checksum, and release
 *	the latch.
 *
 **************************************/
	if (!(prefetch->prf_flags & PRF_active)) {
		return;
	}

	THREAD_EXIT();
	prefetch->prf_piob.piob_wait = TRUE;
	const bool async_status = PIO_status(&prefetch->prf_piob, status_vector);
	THREAD_ENTER();

/* If there was an I/O error release all buffer latches acquired
   for the prefetch request. */

	thread_db* tdbb = prefetch->prf_tdbb;
	Database* dbb = tdbb->tdbb_database;

	if (!async_status) {
		BufferDesc** next_bdb = prefetch->prf_bdbs;
		for (USHORT i = 0; i < prefetch->prf_max_prefetch; i++) {
			if (*next_bdb) {
				release_bdb(tdbb, *next_bdb, true, false, false);
			}
			next_bdb++;
		}
		prefetch->prf_flags &= ~PRF_active;
		return;
	}

	SCHAR* next_buffer = prefetch->prf_io_buffer;
	BufferDesc** next_bdb = prefetch->prf_bdbs;

	for (USHORT i = 0; i < prefetch->prf_max_prefetch; i++) {
		if (*next_bdb) {
			pag* page = (*next_bdb)->bdb_buffer;
			if (next_buffer != reinterpret_cast<char*>(page)) {
				MOVE_FASTER(next_buffer, (SCHAR *) page,
							(ULONG) dbb->dbb_page_size);
			}
			if (page->pag_checksum == CCH_checksum(*next_bdb)) {
				(*next_bdb)->bdb_flags &= ~(BDB_read_pending | BDB_not_valid);
				(*next_bdb)->bdb_flags |= BDB_prefetch;
			}
			release_bdb(tdbb, *next_bdb, true, false, false);
		}
		next_buffer += dbb->dbb_page_size;
		next_bdb++;
	}

	prefetch->prf_flags &= ~PRF_active;
}


static void prefetch_init(Prefetch* prefetch, thread_db* tdbb)
{
/**************************************
 *
 *	p r e f e t c h _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize prefetch data structure.
 *	Most systems that allow access to "raw" I/O
 *	interfaces want the buffer address aligned.
 *
 **************************************/
	Database* dbb = tdbb->tdbb_database;

	prefetch->prf_tdbb = tdbb;
	prefetch->prf_flags = 0;
	prefetch->prf_max_prefetch = PREFETCH_MAX_TRANSFER / dbb->dbb_page_size;
	prefetch->prf_aligned_buffer =
		(SCHAR
		 *) (((U_IPTR) & prefetch->prf_unaligned_buffer + MIN_PAGE_SIZE -
			  1) & ~((U_IPTR) MIN_PAGE_SIZE - 1));
}


static void prefetch_io(Prefetch* prefetch, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	p r e f e t c h _ i o
 *
 **************************************
 *
 * Functional description
 *	Queue an asynchronous I/O to read
 *	multiple pages into prefetch buffer.
 *
 **************************************/
	thread_db* tdbb = prefetch->prf_tdbb;
	Database* dbb = tdbb->tdbb_database;

	if (!prefetch->prf_page_count) {
		prefetch->prf_flags &= ~PRF_active;
	}
	else {
		/* Get the cache reader working on our behalf too */

		if (!(dbb->dbb_bcb->bcb_flags & BCB_reader_active)) {
			ISC_event_post(dbb->dbb_reader_event);
		}

		THREAD_EXIT();
		const bool async_status =
			PIO_read_ahead(dbb, prefetch->prf_start_page,
						   prefetch->prf_io_buffer, prefetch->prf_page_count,
						   &prefetch->prf_piob, status_vector);
		THREAD_ENTER();
		if (!async_status) {
			BufferDesc** next_bdb = prefetch->prf_bdbs;
			for (USHORT i = 0; i < prefetch->prf_max_prefetch; i++) {
				if (*next_bdb) {
					release_bdb(tdbb, *next_bdb, true, false, false);
				}
				next_bdb++;
			}
			prefetch->prf_flags &= ~PRF_active;
		}
	}
}


static void prefetch_prologue(Prefetch* prefetch, SLONG* start_page)
{
/**************************************
 *
 *	p r e f e t c h _ p r o l o g u e
 *
 **************************************
 *
 * Functional description
 *	Search for consecutive pages to be prefetched
 *	and latch them for I/O.
 *
 **************************************/
	thread_db* tdbb = prefetch->prf_tdbb;
	Database* dbb = tdbb->tdbb_database;
	BufferControl* bcb = dbb->dbb_bcb;

	prefetch->prf_start_page = *start_page;
	prefetch->prf_page_count = 0;
	prefetch->prf_flags |= PRF_active;

	BufferDesc** next_bdb = prefetch->prf_bdbs;
	for (USHORT i = 0; i < prefetch->prf_max_prefetch; i++) {
		*next_bdb = 0;
		if (SBM_clear(bcb->bcb_prefetch, *start_page) &&
			(*next_bdb = get_buffer(tdbb, *start_page, LATCH_shared, 0)))
		{
			if ((*next_bdb)->bdb_flags & BDB_read_pending) {
				prefetch->prf_page_count = i + 1;
			}
			else {
				release_bdb(tdbb, *next_bdb, true, false, false);
				*next_bdb = 0;
			}
		}
		next_bdb++;
		(*start_page)++;
	}

/* Optimize non-sequential list prefetching to transfer
   directly to database buffers. */

	BufferDesc* bdb;
	if (prefetch->prf_page_count == 1 && (bdb = prefetch->prf_bdbs[0])) {
		prefetch->prf_io_buffer = reinterpret_cast<char*>(bdb->bdb_buffer);
	}
	else {
		prefetch->prf_io_buffer = prefetch->prf_aligned_buffer;
	}

/* Reset starting page for next bitmap walk */

	--(*start_page);
}
#endif


static SSHORT related(const BufferDesc* low, const BufferDesc* high, SSHORT limit)
{
/**************************************
 *
 *	r e l a t e d
 *
 **************************************
 *
 * Functional description
 *	See if there are precedence relationships linking two buffers.
 *	Since precedence graphs can become very complex, limit search for
 *	precedence relationship by visiting a presribed limit of higher
 *	precedence blocks.
 *
 **************************************/
	const struct que* base = &low->bdb_higher;

	for (const struct que* que_inst = base->que_forward; que_inst != base; que_inst = que_inst->que_forward)
	{
		if (!--limit) {
			return PRE_UNKNOWN;
		}
		const Precedence* precedence = BLOCK(que_inst, Precedence*, pre_higher);
		if (!(precedence->pre_flags & PRE_cleared)) {
			if (precedence->pre_hi == high) {
				return PRE_EXISTS;
			}
			limit = related(precedence->pre_hi, high, limit);
			if (limit == PRE_EXISTS || limit == PRE_UNKNOWN) {
				return limit;
			}
		}
	}

	return limit;
}


#ifdef PAGE_LATCHING
static void release_bdb(
						thread_db* tdbb,
						BufferDesc* bdb,
						const bool repost,
						const bool downgrade_latch, const bool rel_mark_latch)
{
/**************************************
 *
 *	r e l e a s e _ b d b
 *
 **************************************
 *
 * Functional description
 *	Decrement the use count of a BufferDesc, reposting
 *	blocking AST if required.
 *	If rel_mark_latch is true, the the value of downgrade_latch is ignored.
 *
 **************************************/
	SSHORT i;

//	LATCH_MUTEX_ACQUIRE;

	QUE wait_que = &bdb->bdb_waiters;

/* Releasing a LATCH_mark. */
	if (rel_mark_latch) {
		if ((bdb->bdb_io != tdbb) || (bdb->bdb_exclusive != tdbb)) {
			cache_bugcheck(294);	/* inconsistent LATCH_mark release */
		}
		bdb->bdb_io = 0;
	}
	else
/* Downgrading from an exlusive to a shared latch. */
	if (downgrade_latch) {
		/* Only the transition from exclusive to shared is supported.
		   If an actual state changed, then we need to check if waiters
		   can be granted.  Otherwise, there is nothing further to do. */
		if (bdb->bdb_io == tdbb) {
			cache_bugcheck(296);	/* inconsistent latch downgrade call */
		}
		if (bdb->bdb_exclusive == tdbb) {
			bdb->bdb_exclusive = 0;
			for (i = 0; bdb->bdb_shared[i]; i++); // empty loop body
			bdb->bdb_shared[i] = tdbb;
		}
		else {
//			LATCH_MUTEX_RELEASE;
			return;
		}
	}
	else
/* If the exclusive latch is held, then certain code does funny things:
   ail.c does: exclusive - mark - exclusive */
	if (bdb->bdb_exclusive == tdbb) {
		--bdb->bdb_use_count;
		if (!bdb->bdb_use_count) {	/* All latches are released */
			bdb->bdb_exclusive = bdb->bdb_io = 0;
			for (i = 0; i < BDB_max_shared; bdb->bdb_shared[i++] = 0); // null loop body
		}
		else if (bdb->bdb_io) {	/* This is a release for an io or an exclusive latch */
			if (bdb->bdb_io == tdbb) {	/* We have an io latch */

				/* The BDB_must_write flag causes the system to latch for io, in addition
				   to the already owned latches.  Make sure not to disturb an already existing
				   exclusive latch. */
				/* ail.c does: EX => MARK => SHARED => release => EX => MARK => RELEASE => EX */
				if (!(bdb->bdb_flags & BDB_marked)) {
					bdb->bdb_io = 0;
				}
			}
			else if (bdb->bdb_use_count == 1) {
				/* This must be a release for our exclusive latch */
				bdb->bdb_exclusive = 0;
			}
		}
		else {					/* This is a release for a shared latch */
			for (i = 0; (i < BDB_max_shared) && (bdb->bdb_shared[i] != tdbb);
				 i++); // null loop body
			if (i < BDB_max_shared) {
				bdb->bdb_shared[i] = 0;
			}
		}
	}
	else
/* If the exclusive latch is not held, then things have to behave much nicer. */
	{
		if (bdb->bdb_flags & BDB_marked) {
			cache_bugcheck(297);	/* bdb is unexpectedly marked */
		}
		--bdb->bdb_use_count;
		if (bdb->bdb_io == tdbb) {
			bdb->bdb_io = 0;
		}
		else {
			for (i = 0; (i < BDB_max_shared) && (bdb->bdb_shared[i] != tdbb);
				 i++); // null loop body
			if (i >= BDB_max_shared) {
				cache_bugcheck(300);	/* can't find shared latch */
			}
			bdb->bdb_shared[i] = 0;
		}
	}

	bool granted = false;

	for (QUE que_inst = wait_que->que_forward; que_inst != wait_que; que_inst = que_inst->que_forward)
	{
		/* Note that this loop assumes that requests for LATCH_io and LATCH_mark
		   are queued before LATCH_shared and LATCH_exclusive. */
		LatchWait* lwt = BLOCK(que_inst, LatchWait*, lwt_waiters);
		if (lwt->lwt_flags & LWT_pending) {
			switch (lwt->lwt_latch) {
			case LATCH_exclusive:
				if (bdb->bdb_use_count) {
//					LATCH_MUTEX_RELEASE;
					return;
				}
				else {
					++bdb->bdb_use_count;
					bdb->bdb_exclusive = lwt->lwt_tdbb;
					lwt->lwt_flags &= ~LWT_pending;
					ISC_event_post(&lwt->lwt_event);
//					LATCH_MUTEX_RELEASE;
					return;
				}

			case LATCH_io:
				if (bdb->bdb_io) {
					break;
				}
				else {
					++bdb->bdb_use_count;
					bdb->bdb_io = lwt->lwt_tdbb;
					lwt->lwt_flags &= ~LWT_pending;
					ISC_event_post(&lwt->lwt_event);
					granted = true;
					break;
				}

			case LATCH_mark:
				if (bdb->bdb_exclusive != lwt->lwt_tdbb) {
					cache_bugcheck(298);	/* missing exclusive latch */
				}
				if (bdb->bdb_io) {
					break;
				}
				else {
					bdb->bdb_io = lwt->lwt_tdbb;
					lwt->lwt_flags &= ~LWT_pending;
					ISC_event_post(&lwt->lwt_event);
					granted = true;
					break;
				}

			case LATCH_shared:
				if (bdb->bdb_exclusive) {
					break;		/* defensive programming */
					
					/* correct programming
					   LATCH_MUTEX_RELEASE;
					   return;  */
				}
				for (i = 0; (i < BDB_max_shared) && bdb->bdb_shared[i]; i++);
				if (i >= BDB_max_shared) {
					break;
				}
				++bdb->bdb_use_count;
				bdb->bdb_shared[i] = lwt->lwt_tdbb;
				lwt->lwt_flags &= ~LWT_pending;
				ISC_event_post(&lwt->lwt_event);
				granted = true;
				break;
			}

			if (granted && (bdb->bdb_flags & BDB_read_pending)) {
				/* Allow only one reader to proceed */
				break;
			}
		}
	}

	if (bdb->bdb_use_count || !repost) {
//		LATCH_MUTEX_RELEASE;
		return;
	}

//	LATCH_MUTEX_RELEASE;

	if (bdb->bdb_ast_flags & BDB_blocking)
	{
		PAGE_LOCK_RE_POST(bdb->bdb_lock);
	}
}


#else
static void release_bdb(
						thread_db* tdbb,
						BufferDesc* bdb,
						const bool repost,
						const bool downgrade_latch, const bool rel_mark_latch)
{
/**************************************
 *
 *	r e l e a s e _ b d b
 *
 **************************************
 *
 * Functional description
 *	Decrement the use count of a BufferDesc, reposting
 *	blocking AST if required.
 *	If rel_mark_latch is TRUE, the the value of downgrade_latch is ignored.
 *
 **************************************/

/* Releasing a LATCH_mark. */

	if (rel_mark_latch) {
		bdb->bdb_io = 0;
	}
	else
/* Downgrading from an exlusive to a shared latch. */
	if (downgrade_latch) {
		bdb->bdb_exclusive = 0;
	}
	else
/* If the exclusive latch is held, then certain code does funny things:
   ail.c does: exclusive - mark - exclusive */
	if (bdb->bdb_exclusive == tdbb) {
		if (!--bdb->bdb_use_count) {
			bdb->bdb_exclusive = bdb->bdb_io = 0;
		}
		else if (bdb->bdb_io) {	/* This is a release for an io or an exclusive latch */
			if (bdb->bdb_io == tdbb) {	/* We have an io latch */

				/* The BDB_must_write flag causes the system to latch for io, in addition
				   to the already owned latches.  Make sure not to disturb an already existing
				   exclusive latch. */
				/* ail.c does: EX => MARK => SHARED => release => EX => MARK => RELEASE => EX */
				if (!(bdb->bdb_flags & BDB_marked)) {
					bdb->bdb_io = 0;
				}
			}
			else if (bdb->bdb_use_count == 1) {
				/* This must be a release for our exclusive latch */
				bdb->bdb_exclusive = 0;
			}
		}
	}
	else
		/* If the exclusive latch is not held, then things have to behave much nicer. */
	{
		--bdb->bdb_use_count;
		bdb->bdb_io = 0;
	}

	if (bdb->bdb_use_count || !repost) {
		return;
	}

	if (bdb->bdb_ast_flags & BDB_blocking) {
		PAGE_LOCK_RE_POST(bdb->bdb_lock);
	}
}
#endif


static bool writeable(const BufferDesc* bdblock)
{
/**************************************
 *
 *	w r i t e a b l e
 *
 **************************************
 *
 * Functional description
 *	See if a buffer is writeable.  A buffer is writeable if
 *	neither it nor any of it's higher precedence cousins are
 *	marked for write.
 *
 **************************************/
	if (bdblock->bdb_flags & BDB_marked) {
		return false;
	}

/* If there are buffers that must be written first, check them, too. */

	for (const que* queue = bdblock->bdb_higher.que_forward;
		queue != &bdblock->bdb_higher; queue = queue->que_forward)
	{
		const Precedence* precedence = BLOCK(queue, Precedence*, pre_higher);
		if (!(precedence->pre_flags & PRE_cleared) &&
			!writeable(precedence->pre_hi))
		{
			return false;
		}
	}

	return true;
}


static int write_buffer(
						thread_db* tdbb,
						BufferDesc* bdb,
						SLONG page,
						const bool write_thru,
						ISC_STATUS* status, const bool write_this_page)
{
/**************************************
 *
 *	w r i t e _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Write a dirty buffer.  This may recurse due to
 *	precedence problems.
 *
 * input:  write_this_page
 *		= true if the input page needs to be written
 *			before returning.  (normal case)
 *		= false if the input page is being written
 *			because of precedence.  Only write
 *			one page and return so that the caller
 *			can re-establish the need to write this
 *			page.
 *
 * return: 0 = Write failed.
 *         1 = Page is written.  Page was written by this
 *     		call, or was written by someone else, or the
 *		cache buffer is already reassigned.
 *	   2 = Only possible if write_this_page is false.
 *		This input page is not written.  One
 *		page higher in precedence is written
 *		though.  Probable action: re-establich the
 * 		need to write this page and retry write.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->tdbb_database;

	if (latch_bdb(tdbb, LATCH_io, bdb, page, 1) == -1) {
		return 1;
	}

	if ((bdb->bdb_flags & BDB_marked) && !(bdb->bdb_flags & BDB_faked)) {
		cache_bugcheck(217);	/* msg 217 buffer marked for update */
	}

	if (!(bdb->bdb_flags & BDB_dirty) &&
		!(write_thru && bdb->bdb_flags & BDB_db_dirty))
	{
		clear_precedence(dbb, bdb);
		release_bdb(tdbb, bdb, true, false, false);
		return 1;
	}

/* If there are buffers that must be written first, write them now. */

//	PRE_MUTEX_ACQUIRE;

	while (QUE_NOT_EMPTY(bdb->bdb_higher)) {
		BufferControl* bcb = dbb->dbb_bcb;		/* Re-initialize in the loop */
		QUE que_inst = bdb->bdb_higher.que_forward;
		Precedence* precedence = BLOCK(que_inst, Precedence*, pre_higher);
		if (precedence->pre_flags & PRE_cleared) {
			QUE_DELETE(precedence->pre_higher);
			QUE_DELETE(precedence->pre_lower);
			precedence->pre_hi = (BufferDesc*) bcb->bcb_free;
			bcb->bcb_free = precedence;
		}
		else {
			BufferDesc* hi_bdb = precedence->pre_hi;
			const SLONG hi_page = hi_bdb->bdb_page;
//			PRE_MUTEX_RELEASE;
			release_bdb(tdbb, bdb, false, false, false);
			const int write_status =
				write_buffer(tdbb, hi_bdb, hi_page, write_thru, status,
							 false);
			if (write_status == 0) {
				return 0;		/* return IO error */
			}
#ifdef SUPERSERVER
			if (!write_this_page) {
				return 2;
				/* caller wants to re-establish the need for this write after one precedence write */
			}
#endif
			if (latch_bdb(tdbb, LATCH_io, bdb, page, 1) == -1) {
				return 1;		/* cache buffer reassigned, return 'write successful' */
			}
//			PRE_MUTEX_ACQUIRE;
		}
	}

//	PRE_MUTEX_RELEASE;

#ifdef SUPERSERVER_V2
/* Header page I/O is deferred until a dirty page, which was modified by a
   transaction not updated on the header page, needs to be written. Note
   that the header page needs to be written with the target page latched to
   prevent younger transactions from modifying the target page. */

	if (page != HEADER_PAGE
		&& bdb->bdb_mark_transaction > dbb->dbb_last_header_write)
	{
		TRA_header_write(tdbb, dbb, bdb->bdb_mark_transaction);
	}
#endif

/* Unless the buffer has been faked (recently re-allocated), write
   out the page */

	bool result = true;
	if ((bdb->bdb_flags & BDB_dirty
		 || (write_thru && bdb->bdb_flags & BDB_db_dirty))
		&& !(bdb->bdb_flags & BDB_marked))
	{
		if ( (result = write_page(tdbb, bdb, write_thru, status, false)) ) {
			clear_precedence(dbb, bdb);
		}
	}
	else {
		clear_precedence(dbb, bdb);
	}

	release_bdb(tdbb, bdb, true, false, false);

	if (!result) {
		return 0;
	}
	else
#ifdef SUPERSERVER
	if (!write_this_page) {
		return 2;
	}
	else
#endif
		return 1;
}


static bool write_page(
						  thread_db* tdbb,
						  BufferDesc* bdb,
						  const bool write_thru, ISC_STATUS* status,
						  const bool inAst)
{
/**************************************
 *
 *	w r i t e _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Do actions required when writing a database page,
 *	including journaling, shadowing.
 *
 **************************************/
	if (bdb->bdb_flags & BDB_not_valid) {
		*status++ = isc_arg_gds;
		*status++ = isc_buf_invalid;
		*status++ = isc_arg_number;
		*status++ = bdb->bdb_page;
		*status++ = isc_arg_end;
		return false;
	}

	bool result = true;
	Database* dbb = bdb->bdb_dbb;
	pag* page = bdb->bdb_buffer;

/* Before writing db header page, make sure that the next_transaction > oldest_active
   transaction */
	if (bdb->bdb_page == HEADER_PAGE) {
		header_page* header = (header_page*) page;
		if (header->hdr_next_transaction) {
			if (header->hdr_oldest_active > header->hdr_next_transaction) {
				BUGCHECK(266);	/*next transaction older than oldest active */
			}

			if (header->hdr_oldest_transaction > header->hdr_next_transaction) {
				BUGCHECK(267);	/* next transaction older than oldest transaction */
			}
		}
	}

	page->pag_generation++;

//	if (!dbb->dbb_wal || write_thru) becomes
//	if (true || write_thru) then finally if (true)
// I won't wipe out the if() itself to allow my changes be verified easily by others
	if (true) {
		AST_CHECK();
		dbb->dbb_writes++;

		/* write out page to main database file, and to any
		   shadows, making a special case of the header page */

		if (bdb->bdb_page >= 0) {
			if (bdb->bdb_write_direction == BDB_write_undefined) {
				dbb->dbb_flags |= DBB_bugcheck;
				status[0] = isc_arg_gds;
				status[1] = isc_bug_check;
				status[2] = isc_arg_string;
				status[3] = (ISC_STATUS)ERR_cstring("Undefined page write direction");
				status[4] = isc_arg_end;
				return false;
			}
			page->pag_checksum = CCH_checksum(bdb);
			if (bdb->bdb_write_direction == BDB_write_diff ||
				(bdb->bdb_write_direction == BDB_write_both 
#ifndef SUPERSERVER
				 && bdb->bdb_diff_generation == dbb->backup_manager->get_current_generation()
#endif
				)) 
			{
#ifdef NBAK_DEBUG
				// We cannot call normal trace functions here as they are signal-unsafe
				char buffer[1000], *ptr = buffer;
				strcpy(ptr, "NBAK,Write page ");
				ptr += strlen(ptr);
				gds__ulstr(ptr, bdb->bdb_page, 0, 0);
				ptr += strlen(ptr);
				strcpy(ptr, " at offset ");
				ptr += strlen(ptr);
				gds__ulstr(ptr, bdb->bdb_difference_page, 0, 0);
				ptr += strlen(ptr);
				strcpy(ptr, " in difference file");
				gds__trace(buffer);
#endif
				if (!dbb->backup_manager->write_difference(
					status, bdb->bdb_difference_page, bdb->bdb_buffer)) 
				{
					bdb->bdb_flags |= BDB_io_error;
					dbb->dbb_flags |= DBB_suspend_bgio;
					return false;
				}
			}
			if (bdb->bdb_write_direction == BDB_write_diff) {
				// We finished. Adjust transaction accounting and get ready for exit
				if (bdb->bdb_page == HEADER_PAGE) {
					dbb->dbb_last_header_write =
						((header_page*) page)->hdr_next_transaction;
				}
				set_write_direction(dbb, bdb, BDB_write_undefined);
			}
			else {
				// We need to write our pages to main database files
#ifdef SUPERSERVER
				THREAD_EXIT();
#endif
				jrd_file* file = dbb->dbb_file;
				while (!PIO_write(file, bdb, page, status)) {
#ifdef SUPERSERVER
					THREAD_ENTER();
#endif
					if (!CCH_rollover_to_shadow(dbb, file, inAst)) {
						bdb->bdb_flags |= BDB_io_error;
						dbb->dbb_flags |= DBB_suspend_bgio;
						return false;
					}
#ifdef SUPERSERVER
					THREAD_EXIT();
#endif
					file = dbb->dbb_file;
				}

#ifdef SUPERSERVER
				THREAD_ENTER();
#endif
				if (bdb->bdb_page == HEADER_PAGE) {
					dbb->dbb_last_header_write =
						((header_page*) page)->hdr_next_transaction;
				}
				if (dbb->dbb_shadow) {
					result =
						CCH_write_all_shadows(tdbb, 0, bdb, status, 0, inAst);
				}
				set_write_direction(dbb, bdb, BDB_write_undefined);
			}
		}

#ifdef SUPERSERVER
		if (result) {
#ifdef CACHE_WRITER
			if (bdb->bdb_flags & BDB_checkpoint) {
				--dbb->dbb_bcb->bcb_checkpoint;
			}
#endif
			bdb->bdb_flags &= ~(BDB_db_dirty | BDB_checkpoint);
		}
#endif
		AST_CHECK();
	}

	if (!result) {
		/* If there was a write error then idle background threads
		   so that they don't spin trying to write these pages. This
		   usually results from device full errors which can be fixed
		   by someone freeing disk space. */

		bdb->bdb_flags |= BDB_io_error;
		dbb->dbb_flags |= DBB_suspend_bgio;
	}
	else {
		/* clear the dirty bit vector, since the buffer is now
		   clean regardless of which transactions have modified it */

		bdb->bdb_transactions = bdb->bdb_mark_transaction = 0;
		if (!(dbb->dbb_bcb->bcb_flags & BCB_keep_pages) &&
			(bdb->bdb_parent || bdb == dbb->dbb_bcb->bcb_btree))
		{
			btc_remove(bdb);
		}
		bdb->bdb_flags &= ~(BDB_dirty | BDB_must_write | BDB_system_dirty);
		if (bdb->bdb_flags & BDB_io_error) {
			/* If a write error has cleared, signal background threads
			   to resume their regular duties. If someone has freed up
			   disk space these errors will spontaneously go away. */

			bdb->bdb_flags &= ~BDB_io_error;
			dbb->dbb_flags &= ~DBB_suspend_bgio;
		}
	}

	return result;
}


static void unmark(thread_db* tdbb, WIN * window)
{
/**************************************
 *
 *	u n m a r k
 *
 **************************************
 *
 * Functional description
 *	Unmark a BufferDesc.  Called when the update of a page is
 *	complete and delaying the 'unmarking' could cause
 *	problems.
 *
 **************************************/
	SET_TDBB(tdbb);
	BufferDesc* bdb = window->win_bdb;
	BLKCHK(bdb, type_bdb);

	if (bdb->bdb_use_count == 1) {
		const bool marked = (bdb->bdb_flags & BDB_marked) != 0;
		bdb->bdb_flags &= ~BDB_marked;
		if (marked) {
			release_bdb(tdbb, bdb, false, false, true);
		}
	}
}

