/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		shut.cpp
 *	DESCRIPTION:	Database shutdown handler
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
#include "../jrd/jrd.h"
#include "../jrd/scl.h"
#include "../jrd/ibase.h"
#include "../jrd/ods.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/err_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/rng_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/shut_proto.h"
#include "../jrd/thd.h"
#include "../jrd/thread_proto.h"
#include "../jrd/tra_proto.h"

using namespace Jrd;

const SSHORT SHUT_WAIT_TIME	= 5;

// Shutdown lock data
union shutdown_data {
	struct {
		SSHORT flag;
		SSHORT delay;
	} data_items;
	SLONG data_long;
};


// Define this to true if you need to allow no-op behavior when requested shutdown mode 
// matches current. Logic of jrd8_create_database may need attention in this case too
const bool IGNORE_SAME_MODE = false;

static bool bad_mode(bool ignore);
static bool notify_shutdown(Database*, SSHORT, SSHORT);
static bool shutdown_locks(Database*, SSHORT);


bool SHUT_blocking_ast(Database* dbb)
{
/**************************************
 *
 *	S H U T _ b l o c k i n g _ a s t
 *
 **************************************
 *
 * Functional description
 *	Read data from database lock for
 *	shutdown instructions.
 *
 **************************************/
	shutdown_data data;
	data.data_long = LCK_read_data(dbb->dbb_lock);
	const SSHORT flag = data.data_items.flag;
	const SSHORT delay = data.data_items.delay;

/* Database shutdown has been cancelled. */

	// Delay of -1 means we're going online
	if (delay == -1) {
		dbb->dbb_ast_flags &=
			~(DBB_shut_attach | DBB_shut_tran | DBB_shut_force |
			  DBB_shutdown | DBB_shutdown_single | DBB_shutdown_full);
		switch (flag & isc_dpb_shut_mode_mask) {
		case isc_dpb_shut_normal:
			break;
		case isc_dpb_shut_multi:
			dbb->dbb_ast_flags |= DBB_shutdown;
			break;
		case isc_dpb_shut_single:
			dbb->dbb_ast_flags |= DBB_shutdown | DBB_shutdown_single;
			break;
		case isc_dpb_shut_full:
			dbb->dbb_ast_flags |= DBB_shutdown | DBB_shutdown_full;
			break;
		}
					
		dbb->dbb_shutdown_delay = 0;
		for (Attachment* attachment = dbb->dbb_attachments; attachment;
			 attachment = attachment->att_next)
		{
			 attachment->att_flags &= ~ATT_shutdown_notify;
		}
		return false;
	}

	if ((flag & isc_dpb_shut_force) && !delay)
		return shutdown_locks(dbb, flag);
	else {
		if (flag & isc_dpb_shut_attachment)
			dbb->dbb_ast_flags |= DBB_shut_attach;
		if (flag & isc_dpb_shut_force)
			dbb->dbb_ast_flags |= DBB_shut_force;
		if (flag & isc_dpb_shut_transaction)
			dbb->dbb_ast_flags |= DBB_shut_tran;
		dbb->dbb_shutdown_delay = delay;
		return false;
	}
}


bool SHUT_database(Database* dbb, SSHORT flag, SSHORT delay)
{
/**************************************
 *
 *	S H U T _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Schedule database for shutdown
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Attachment* attachment = tdbb->tdbb_attachment;

/* Only platform's user locksmith can shutdown or bring online
   a database. */

	if (!(attachment->att_user->usr_flags & (USR_locksmith | USR_owner))) {
		return false;
	}

	// Check if requested shutdown mode is valid
	// Note that if we are already in requested mode we just return true.
	// This is required to ensure backward compatible behavior (gbak relies on that, 
	// user-written scripts may rely on this behaviour too)
	int shut_mode = flag & isc_dpb_shut_mode_mask;
	switch (shut_mode) {
	case isc_dpb_shut_full:
		if (dbb->dbb_ast_flags & DBB_shutdown_full) 
			return bad_mode(IGNORE_SAME_MODE);
		break;
	case isc_dpb_shut_multi:
		if ((dbb->dbb_ast_flags & DBB_shutdown_full) ||
			(dbb->dbb_ast_flags & DBB_shutdown_single))
		{
			return bad_mode(false);
		}
		if (dbb->dbb_ast_flags & DBB_shutdown)
			return bad_mode(IGNORE_SAME_MODE);
		break;
	case isc_dpb_shut_single:
		if (dbb->dbb_ast_flags & DBB_shutdown_full)
			return bad_mode(false);
		if (dbb->dbb_ast_flags & DBB_shutdown_single)
			return bad_mode(IGNORE_SAME_MODE);
		break;
	case isc_dpb_shut_normal:	
		if (!(dbb->dbb_ast_flags & DBB_shutdown))
			return bad_mode(IGNORE_SAME_MODE);
		return bad_mode(false);
	default:
		return bad_mode(false); // unexpected mode
	}
	
	try {

	attachment->att_flags |= ATT_shutdown_manager;
	--dbb->dbb_use_count;

/* Database is being shutdown. First notification gives shutdown
   type and delay in seconds. */

	bool exclusive = notify_shutdown(dbb, flag, delay);

/* Notify local attachments */

	SHUT_blocking_ast(dbb);

/* Try to get exclusive database lock periodically up to specified delay. If we
   haven't gotten it report shutdown error for weaker forms. For forced shutdown
   keep notifying until successful. */

	SSHORT timeout = delay - SHUT_WAIT_TIME;

	if (!exclusive)
	{
		for (; timeout >= 0; timeout -= SHUT_WAIT_TIME)
		{
			if ((exclusive = notify_shutdown(dbb, flag, timeout)) ||
				!(dbb->dbb_ast_flags & (DBB_shut_attach | DBB_shut_tran |
										DBB_shut_force)))
			{
				break;
			}
		}
	}

	if (!exclusive && (timeout > 0 ||
					   flag & (isc_dpb_shut_attachment |
							   isc_dpb_shut_transaction)))
	{
		notify_shutdown(dbb, 0, 0);	/* Tell everyone we're giving up */
		SHUT_blocking_ast(dbb);
		attachment->att_flags &= ~ATT_shutdown_manager;
		++dbb->dbb_use_count;
		ERR_post(isc_shutfail, 0);
	}

/* Once there are no more transactions active, force all remaining
   attachments to shutdown. */

	if (flag & isc_dpb_shut_transaction) {
		exclusive = false;
		flag = isc_dpb_shut_force | (flag & isc_dpb_shut_mode_mask);
	}

	dbb->dbb_ast_flags |= DBB_shutdown;
	dbb->dbb_ast_flags &= ~(DBB_shutdown_single | DBB_shutdown_full);
	if (flag & isc_dpb_shut_single)
		dbb->dbb_ast_flags |= DBB_shutdown_single;
	else if (flag & isc_dpb_shut_full)
		dbb->dbb_ast_flags |= DBB_shutdown_full;

	if (!exclusive && (flag & isc_dpb_shut_force)) {
		// TMN: Ugly counting!
		while (!notify_shutdown(dbb, flag, 0));
	}

	++dbb->dbb_use_count;
	dbb->dbb_ast_flags &= ~(DBB_shut_force | DBB_shut_attach | DBB_shut_tran);
	WIN window(HEADER_PAGE);
	Ods::header_page* header =
		(Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	// Set appropriate shutdown mode in database header
	header->hdr_flags &= ~Ods::hdr_shutdown_mask;
	switch (flag & isc_dpb_shut_mode_mask) {
	case isc_dpb_shut_normal:
		break;
	case isc_dpb_shut_multi:
		header->hdr_flags |= Ods::hdr_shutdown_multi;
		break;
	case isc_dpb_shut_single:
		header->hdr_flags |= Ods::hdr_shutdown_single;
		break;
	case isc_dpb_shut_full:
		header->hdr_flags |= Ods::hdr_shutdown_full;
		break;
	}
	CCH_RELEASE(tdbb, &window);
	CCH_release_exclusive(tdbb);

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		return false;
	}

	return true;
}


bool SHUT_init(Database* dbb)
{
/**************************************
 *
 *	S H U T _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Read data from database lock for
 *	shutdown instructions.
 *
 **************************************/

	return SHUT_blocking_ast(dbb);
}


bool SHUT_online(Database* dbb, SSHORT flag)
{
/**************************************
 *
 *	S H U T _ o n l i n e
 *
 **************************************
 *
 * Functional description
 *	Move database to "more online" state
 *
 **************************************/

	thread_db* tdbb = JRD_get_thread_data();
	Attachment* attachment = tdbb->tdbb_attachment;

/* Only platform's user locksmith can shutdown or bring online
   a database. */

	if (!(attachment->att_user->usr_flags & (USR_locksmith | USR_owner))) {
		return false;
	}
	
	// Check if requested shutdown mode is valid
	int shut_mode = flag & isc_dpb_shut_mode_mask;
	switch (shut_mode) {
	case isc_dpb_shut_normal:
		if (!(dbb->dbb_ast_flags & DBB_shutdown)) 
			return bad_mode(IGNORE_SAME_MODE); // normal -> normal
		break;
	case isc_dpb_shut_multi:
		if (!(dbb->dbb_ast_flags & DBB_shutdown))
			return bad_mode(false); // normal -> multi
		if (!(dbb->dbb_ast_flags & DBB_shutdown_full) && 
		    !(dbb->dbb_ast_flags & DBB_shutdown_single)) 
		{
			return bad_mode(IGNORE_SAME_MODE); // multi -> multi
		}
		break;
	case isc_dpb_shut_single:		
		if (dbb->dbb_ast_flags & DBB_shutdown_single)
			return bad_mode(IGNORE_SAME_MODE); //single -> single
		if (!(dbb->dbb_ast_flags & DBB_shutdown_full))
			return bad_mode(false); // !full -> single
		break;		
	case isc_dpb_shut_full:
		if (dbb->dbb_ast_flags & DBB_shutdown_full)
		{
			return bad_mode(IGNORE_SAME_MODE); // full -> full
		}
		return bad_mode(false);
	default: // isc_dpb_shut_full
		return bad_mode(false); // unexpected mode
	}
	

	try {

	/* Clear shutdown flag on database header page */

	WIN window(HEADER_PAGE);
	Ods::header_page* header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	// Set appropriate shutdown mode in database header
	header->hdr_flags &= ~Ods::hdr_shutdown_mask;
	switch (shut_mode) {
	case isc_dpb_shut_normal:
		break;
	case isc_dpb_shut_multi:
		header->hdr_flags |= Ods::hdr_shutdown_multi;
		break;
	case isc_dpb_shut_single:
		header->hdr_flags |= Ods::hdr_shutdown_single;
		break;
	case isc_dpb_shut_full:
		header->hdr_flags |= Ods::hdr_shutdown_full;
		break;
	}
	CCH_RELEASE(tdbb, &window);

	/* Notify existing database clients that a currently
	   scheduled shutdown is cancelled. */

	if (notify_shutdown(dbb, shut_mode, -1))
		CCH_release_exclusive(tdbb);

	/* Notify local attachments */

	SHUT_blocking_ast(dbb);

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		return false;
	}
	
	return true;
}


static bool bad_mode(bool ignore) {
	if (!ignore) {
		thread_db* tdbb = JRD_get_thread_data();
		
		ISC_STATUS* status = tdbb->tdbb_status_vector;
		*status++ = isc_arg_gds;
		*status++ = isc_bad_shutdown_mode;
		*status++ = isc_arg_string;
		*status++ = (ISC_STATUS) (IPTR) 
			ERR_cstring(reinterpret_cast<const TEXT*>(tdbb->tdbb_database->dbb_filename.c_str()));
		*status++ = isc_arg_end;
	}
	return ignore;
}


static bool notify_shutdown(Database* dbb, SSHORT flag, SSHORT delay)
{
/**************************************
 *
 *	n o t i f y _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *	Notify database users that shutdown
 *	status of a database is changing.
 *	Pulse database lock and pass shutdown
 *	flags and delay via lock data.
 *
 **************************************/

	thread_db* tdbb = JRD_get_thread_data();
	shutdown_data data;

	data.data_items.flag = flag;
	data.data_items.delay = delay;

	LCK_write_data(dbb->dbb_lock, data.data_long);

/* Send blocking ASTs to database users */

	bool exclusive =
		CCH_exclusive(tdbb, LCK_PW, delay > 0 ? -SHUT_WAIT_TIME : LCK_NO_WAIT);

	if (exclusive && (delay != -1)) {
		return shutdown_locks(dbb, flag);
	}
	if ((flag & isc_dpb_shut_force) && !delay) {
		return shutdown_locks(dbb, flag);
	}
	if ((flag & isc_dpb_shut_transaction) &&
		!(TRA_active_transactions(tdbb, dbb)))
	{
		return true;
	}

	return exclusive;
}


static bool shutdown_locks(Database* dbb, SSHORT flag)
{
/**************************************
 *
 *	s h u t d o w n _ l o c k s
 *
 **************************************
 *
 * Functional description
 *	Release all attachment and database
 *	locks if database is quiet.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* Mark database and all active attachments as shutdown. */

	dbb->dbb_ast_flags &= ~(DBB_shutdown | DBB_shutdown_single | DBB_shutdown_full);
	
	switch (flag & isc_dpb_shut_mode_mask) {
	case isc_dpb_shut_multi:
		dbb->dbb_ast_flags |= DBB_shutdown;
		break;
	case isc_dpb_shut_single:
		dbb->dbb_ast_flags |= DBB_shutdown | DBB_shutdown_single;
		break;
	case isc_dpb_shut_full:
		dbb->dbb_ast_flags |= DBB_shutdown | DBB_shutdown_full;
		break;
	}

	Attachment* attachment;
	
	for (attachment = dbb->dbb_attachments; attachment;
		 attachment = attachment->att_next)
	{
		if (!(attachment->att_flags & ATT_shutdown_manager))
			attachment->att_flags |= ATT_shutdown;
	}

	if (dbb->dbb_use_count) {
		/* Let active database threads rundown */

		THREAD_EXIT();
		THREAD_SLEEP(1 * 1000);
		THREAD_ENTER();
		return false;
	}

/* Since no attachment is actively running, release all
   attachment-specfic locks while they're not looking. */

	const Attachment* shut_attachment = NULL;

	for (attachment = dbb->dbb_attachments; attachment;
		 attachment = attachment->att_next)
	{
		if (attachment->att_flags & ATT_shutdown_manager) {
			shut_attachment = attachment;
			continue;
		}

		if (attachment->att_id_lock)
			LCK_release(tdbb, attachment->att_id_lock);

#ifdef PC_ENGINE
		RNG_shutdown_attachment(attachment);
#endif
		RLCK_shutdown_attachment(attachment);
		TRA_shutdown_attachment(tdbb, attachment);
	}

/* Release database locks that are shared by all attachments.
   These include relation and index existence locks, as well
   as, relation interest and record locking locks for PC semantic
   record locking. */

	RLCK_shutdown_database(dbb);
	CMP_shutdown_database(tdbb);

/* If shutdown manager is here, leave enough database lock context
   to run as a normal attachment. Otherwise, get rid of the rest
   of the database locks.*/

	if (!shut_attachment) {
		CCH_shutdown_database(dbb);
		if (dbb->dbb_shadow_lock)
			LCK_release(tdbb, dbb->dbb_shadow_lock);
		if (dbb->dbb_retaining_lock)
			LCK_release(tdbb, dbb->dbb_retaining_lock);
		if (dbb->dbb_lock)
			LCK_release(tdbb, dbb->dbb_lock);
		dbb->backup_manager->shutdown_locks();
		dbb->dbb_ast_flags |= DBB_shutdown_locks;
	}

	return true;
}

