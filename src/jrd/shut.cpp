/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		shut.c
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
#include "../jrd/jrd.h"
#include "../jrd/scl.h"
#include "../jrd/y_ref.h"
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
#include "../jrd/thd_proto.h"
#include "../jrd/tra_proto.h"

/* Shutdown lock data */

typedef union {
	struct {
		SSHORT flag;
		SSHORT delay;
	} data_items;
	SLONG data_long;
} SDATA;

#define SHUT_WAIT_TIME	5

static BOOLEAN notify_shutdown(DBB, SSHORT, SSHORT);
static BOOLEAN shutdown_locks(DBB);


BOOLEAN SHUT_blocking_ast(DBB dbb)
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
	SDATA data;
	SSHORT flag, delay;
	ATT attachment;

	data.data_long = LCK_read_data(dbb->dbb_lock);
	flag = data.data_items.flag;
	delay = data.data_items.delay;

/* Database shutdown has been cancelled. */

	if (!flag) {
		dbb->dbb_ast_flags &=
			~(DBB_shut_attach | DBB_shut_tran | DBB_shut_force |
			  DBB_shutdown);
		dbb->dbb_shutdown_delay = 0;
		for (attachment = dbb->dbb_attachments; attachment;
			 attachment = attachment->att_next) attachment->att_flags &=
				~ATT_shutdown_notify;
		return FALSE;
	}

	if (flag & isc_dpb_shut_force && !delay)
		return shutdown_locks(dbb);
	else {
		if (flag & isc_dpb_shut_attachment)
			dbb->dbb_ast_flags |= DBB_shut_attach;
		if (flag & isc_dpb_shut_force)
			dbb->dbb_ast_flags |= DBB_shut_force;
		if (flag & isc_dpb_shut_transaction)
			dbb->dbb_ast_flags |= DBB_shut_tran;
		dbb->dbb_shutdown_delay = delay;
		return FALSE;
	}
}


BOOLEAN SHUT_database(DBB dbb, SSHORT flag, SSHORT delay)
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
	TDBB tdbb;
	ATT attachment;
	WIN window;
	HDR header;
	SSHORT timeout, exclusive;

	tdbb = GET_THREAD_DATA;
	attachment = tdbb->tdbb_attachment;

/* Only platform's user locksmith can shutdown or bring online
   a database. */

	if (!(attachment->att_user->usr_flags & (USR_locksmith | USR_owner))) {
		return FALSE;
	}

	try {

/* If shutdown flag is zero then bring database online */

	if (!flag)
	{
		/* Clear shutdown flag on database header page */

		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		header->hdr_flags &= ~hdr_shutdown;
		CCH_RELEASE(tdbb, &window);

		/* Notify existing database clients that a currently
		   scheduled shutdown is cancelled. */

		if (notify_shutdown(dbb, 0, 0))
			CCH_release_exclusive(tdbb);

		/* Notify local attachments */

		SHUT_blocking_ast(dbb);

		return TRUE;
	}

	attachment->att_flags |= ATT_shutdown_manager;
	--dbb->dbb_use_count;

/* Database is being shutdown. First notification gives shutdown
   type and delay in seconds. */

	notify_shutdown(dbb, flag, delay);

/* Notify local attachments */

	SHUT_blocking_ast(dbb);

/* Try to get exclusive database lock periodically up to specified delay. If we
   haven't gotten it report shutdown error for weaker forms. For forced shutdown
   keep notifying until successful. */

	exclusive = FALSE;
	for (timeout = delay; timeout >= 0; timeout -= SHUT_WAIT_TIME)
	{
		if ((exclusive = notify_shutdown(dbb, flag, timeout)) ||
			!(dbb->dbb_ast_flags & (DBB_shut_attach | DBB_shut_tran |
									DBB_shut_force)))
		{
			break;
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
		exclusive = FALSE;
		flag = isc_dpb_shut_force;
	}

	dbb->dbb_ast_flags |= DBB_shutdown;

	if (!exclusive && flag & isc_dpb_shut_force) {
		// TMN: Ugly counting!
		while (!notify_shutdown(dbb, flag, 0));
	}

	++dbb->dbb_use_count;
	dbb->dbb_ast_flags &= ~(DBB_shut_force | DBB_shut_attach | DBB_shut_tran);
	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	header->hdr_flags |= hdr_shutdown;
	CCH_RELEASE(tdbb, &window);
	CCH_release_exclusive(tdbb);

	}	// try
	catch (const std::exception&) {
		return FALSE;
	}

	return TRUE;
}


BOOLEAN SHUT_init(DBB dbb)
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


static BOOLEAN notify_shutdown(DBB dbb, SSHORT flag, SSHORT delay)
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

	TDBB tdbb = GET_THREAD_DATA;
	SDATA data;

	data.data_items.flag = flag;
	data.data_items.delay = delay;

	LCK_write_data(dbb->dbb_lock, data.data_long);

/* Send blocking ASTs to database users */

	if (CCH_exclusive(tdbb, LCK_PW, ((SSHORT) - SHUT_WAIT_TIME)) && flag) {
		return shutdown_locks(dbb);
	}
	if ((flag & isc_dpb_shut_force) && !delay) {
		return shutdown_locks(dbb);
	}
	if ((flag & isc_dpb_shut_transaction) &&
		!(TRA_active_transactions(tdbb, dbb)))
	{
		return TRUE;
	}

	return FALSE;
}


static BOOLEAN shutdown_locks(DBB dbb)
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
	ATT attachment, shut_attachment;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

/* Mark database and all active attachments as shutdown. */

	dbb->dbb_ast_flags |= DBB_shutdown;

	for (attachment = dbb->dbb_attachments; attachment;
		 attachment = attachment->att_next)
	{
		if (!(attachment->att_flags & ATT_shutdown_manager))
			attachment->att_flags |= ATT_shutdown;
	}

	if (dbb->dbb_use_count) {
		/* Let active database threads rundown */

		THREAD_EXIT;
		THREAD_SLEEP(1 * 1000);
		THREAD_ENTER;
		return FALSE;
	}

/* Since no attachment is actively running, release all
   attachment-specfic locks while they're not looking. */

	shut_attachment = NULL;

	for (attachment = dbb->dbb_attachments; attachment;
		 attachment = attachment->att_next) {
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

	return TRUE;
}
