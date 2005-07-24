//____________________________________________________________
//
//		PROGRAM:	Alice (All Else) Utility
//		MODULE:		tdr.cpp
//		DESCRIPTION:	Routines for automated transaction recovery
//
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//
//
//____________________________________________________________
//
//
// 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Apollo" port
//
// 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
//
//

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../alice/alice.h"
#include "../alice/aliceswi.h"
#include "../alice/all.h"
#include "../alice/alice_proto.h"
#include "../alice/all_proto.h"
#include "../alice/alice_meta.h"
#include "../alice/tdr_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/svc_proto.h"
#include "../jrd/thd.h"
#include "../common/classes/ClumpletWriter.h"

static ULONG ask(void);
static void print_description(const tdr*);
static void reattach_database(TDR);
static void reattach_databases(TDR);
static bool reconnect(FB_API_HANDLE, SLONG, const TEXT*, ULONG);


//const char* const NEWLINE = "\n";

static const UCHAR limbo_info[] = { isc_info_limbo, isc_info_end };





//
// The following routines are shared by the command line gfix and
// the windows server manager.  These routines should not contain
// any direct screen I/O (i.e. printf/getc statements).
//


//____________________________________________________________
//
//		Determine the proper action to take
//		based on the state of the various
//		transactions.
//

USHORT TDR_analyze(const tdr* trans)
{
	USHORT advice = TRA_none;

	if (trans == NULL)
		return TRA_none;

//  if the tdr for the first transaction is missing,
//  we can assume it was committed

	USHORT state = trans->tdr_state;
	if (state == TRA_none)
		state = TRA_commit;
	else if (state == TRA_unknown)
		advice = TRA_unknown;

	for (trans = trans->tdr_next; trans; trans = trans->tdr_next) {
		switch (trans->tdr_state) {
			// an explicitly committed transaction necessitates a check for the
			// perverse case of a rollback, otherwise a commit if at all possible

		case TRA_commit:
			if (state == TRA_rollback) {
				ALICE_print(105, 0, 0, 0, 0, 0);
				// msg 105: Warning: Multidatabase transaction is in inconsistent state for recovery.
				ALICE_print(106, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 106: Transaction %ld was committed, but prior ones were rolled back.
				return 0;
			}
			else
				advice = TRA_commit;
			break;

			// a prepared transaction requires a commit if there are missing
			// records up to now, otherwise only do something if somebody else
			// already has

		case TRA_limbo:
			if (state == TRA_none)
				advice = TRA_commit;
			else if (state == TRA_commit)
				advice = TRA_commit;
			else if (state == TRA_rollback)
				advice = TRA_rollback;
			break;

			// an explicitly rolled back transaction requires a rollback unless a
			// transaction has committed or is assumed committed

		case TRA_rollback:
			if ((state == TRA_commit) || (state == TRA_none)) {
				ALICE_print(105, 0, 0, 0, 0, 0);
				// msg 105: Warning: Multidatabase transaction is in inconsistent state for recovery.
				ALICE_print(107, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 107: Transaction %ld was rolled back, but prior ones were committed.

				return 0;
			}
			else
				advice = TRA_rollback;
			break;

			// a missing TDR indicates a committed transaction if a limbo one hasn't
			// been found yet, otherwise it implies that the transaction wasn't
			// prepared

		case TRA_none:
			if (state == TRA_commit)
				advice = TRA_commit;
			else if (state == TRA_limbo)
				advice = TRA_rollback;
			break;

			// specifically advise TRA_unknown to prevent assumption that all are
			// in limbo

		case TRA_unknown:
			if (!advice)
				advice = TRA_unknown;
			break;

		default:
			ALICE_print(67, reinterpret_cast<char*>(trans->tdr_state), 0,
						0, 0, 0);	// msg 67: Transaction state %d not in valid range.
			return 0;
		}
	}

	return advice;
}



//____________________________________________________________
//
//		Attempt to attach a database with a given pathname.
//

bool TDR_attach_database(ISC_STATUS* status_vector,
						 TDR trans,
						 const TEXT* pathname)
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	if (tdgbl->ALICE_data.ua_debug)
		ALICE_print(68, pathname, 0, 0, 0, 0);
		// msg 68: ATTACH_DATABASE: attempted attach of %s

	Firebird::ClumpletWriter dpb(true, MAX_DPB_SIZE, isc_dpb_version1);
	dpb.insertTag(isc_dpb_no_garbage_collect);
	dpb.insertTag(isc_dpb_gfix_attach);
	if (tdgbl->ALICE_data.ua_user) {
		dpb.insertBytes(isc_dpb_user_name, 
						tdgbl->ALICE_data.ua_user,
						strlen(reinterpret_cast<const char*>(tdgbl->ALICE_data.ua_user)));
	}
	if (tdgbl->ALICE_data.ua_password) {
		dpb.insertBytes(tdgbl->sw_service ? isc_dpb_password_enc :
							isc_dpb_password,
						tdgbl->ALICE_data.ua_password, 
						strlen(reinterpret_cast<const char*>(tdgbl->ALICE_data.ua_password)));
	}

	trans->tdr_db_handle = 0;

	isc_attach_database(status_vector, 0, pathname,
						 &trans->tdr_db_handle, dpb.getBufferLength(),
						 reinterpret_cast<const char*>(dpb.getBuffer()));

	if (status_vector[1]) {
		if (tdgbl->ALICE_data.ua_debug) {
			ALICE_print(69, 0, 0, 0, 0, 0);	// msg 69:  failed
			ALICE_print_status(status_vector);
		}
		return false;
	}

	MET_set_capabilities(status_vector, trans);

	if (tdgbl->ALICE_data.ua_debug)
		ALICE_print(70, 0, 0, 0, 0, 0);	// msg 70:  succeeded

	return true;
}



//____________________________________________________________
//
//		Get the state of the various transactions
//		in a multidatabase transaction.
//

void TDR_get_states(TDR trans)
{
	ISC_STATUS_ARRAY status_vector;

	for (TDR ptr = trans; ptr; ptr = ptr->tdr_next)
		MET_get_state(status_vector, ptr);
}



//____________________________________________________________
//
//		Detach all databases associated with
//		a multidatabase transaction.
//

void TDR_shutdown_databases(TDR trans)
{
	ISC_STATUS_ARRAY status_vector;

	for (TDR ptr = trans; ptr; ptr = ptr->tdr_next)
		isc_detach_database(status_vector, &ptr->tdr_db_handle);
}



//
// The following routines are only for the command line utility.
// This should really be split into two files...
//


//____________________________________________________________
//
//		List transaction stuck in limbo.  If the prompt switch is set,
//		prompt for commit, rollback, or leave well enough alone.
//

void TDR_list_limbo(FB_API_HANDLE handle, const TEXT* name, const ULONG switches)
{
	UCHAR buffer[1024];
	ISC_STATUS_ARRAY status_vector;
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	if (isc_database_info(status_vector, &handle, sizeof(limbo_info),
						   reinterpret_cast<const char*>(limbo_info),
						   sizeof(buffer),
						   reinterpret_cast<char*>(buffer))) {
		ALICE_print_status(status_vector);
		return;
	}

    SLONG id;
   	TDR trans;
	UCHAR* ptr = buffer;
	bool flag = true;

	while (flag) {
		const USHORT item = *ptr++;
		const USHORT length = (USHORT) gds__vax_integer(ptr, 2);
		ptr += 2;
		switch (item) {
		case isc_info_limbo:
			id = gds__vax_integer(ptr, length);
			if (switches &
				(sw_commit | sw_rollback | sw_two_phase | sw_prompt))
			{
				TDR_reconnect_multiple(handle, id, name, switches);
				ptr += length;
				break;
			}
			if (!tdgbl->sw_service_thd)
				ALICE_print(71, reinterpret_cast<char*>(id), 0, 0, 0, 0);
				// msg 71: Transaction %d is in limbo.
			if (trans = MET_get_transaction(status_vector, handle, id)) {
#ifdef SUPERSERVER
				SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_multi_tra_id);
				SVC_putc(tdgbl->service_blk, (UCHAR) id);
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 8));
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 16));
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 24));
#endif
				reattach_databases(trans);
				TDR_get_states(trans);
				TDR_shutdown_databases(trans);
				print_description(trans);
			}
#ifdef SUPERSERVER
			else {
				SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_single_tra_id);
				SVC_putc(tdgbl->service_blk, (UCHAR) id);
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 8));
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 16));
				SVC_putc(tdgbl->service_blk, (UCHAR) (id >> 24));
			}
#endif
			ptr += length;
			break;

		case isc_info_truncated:
			if (!tdgbl->sw_service_thd)
				ALICE_print(72, 0, 0, 0, 0, 0);
				// msg 72: More limbo transactions than fit.  Try again

		case isc_info_end:
			flag = false;
			break;

		default:
			if (!tdgbl->sw_service_thd)
				ALICE_print(73, reinterpret_cast<char*>(item), 0, 0, 0, 0);
				// msg 73: Unrecognized info item %d
		}
	}
}



//____________________________________________________________
//
//		Check a transaction's TDR to see if it is
//		a multi-database transaction.  If so, commit
//		or rollback according to the user's wishes.
//		Object strongly if the transaction is in a
//		state that would seem to preclude committing
//		or rolling back, but essentially do what the
//		user wants.  Intelligence is assumed for the
//		gfix user.
//

bool TDR_reconnect_multiple(FB_API_HANDLE handle,
							SLONG id,
							const TEXT* name,
							ULONG switches)
{
	ISC_STATUS_ARRAY status_vector;

//  get the state of all the associated transactions

	TDR trans = MET_get_transaction(status_vector, handle, id);
	if (!trans)
		return reconnect(handle, id, name, switches);

	reattach_databases(trans);
	TDR_get_states(trans);

//  analyze what to do with them; if the advice contradicts the user's
//  desire, make them confirm it; otherwise go with the flow.

	const USHORT advice = TDR_analyze(trans);

	if (!advice) {
		print_description(trans);
		switches = ask();
	}
	else {
		switch (advice) {
		case TRA_rollback:
			if (switches & sw_commit) {
				ALICE_print(74, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 74: A commit of transaction %ld will violate two-phase commit.
				print_description(trans);
				switches = ask();
			}
			else if (switches & sw_rollback)
				switches |= sw_rollback;
			else if (switches & sw_two_phase)
				switches |= sw_rollback;
			else if (switches & sw_prompt) {
				ALICE_print(75, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 75: A rollback of transaction %ld is needed to preserve two-phase commit.
				print_description(trans);
				switches = ask();
			}
			break;

		case TRA_commit:
			if (switches & sw_rollback) {
				ALICE_print(76, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 76: Transaction %ld has already been partially committed.
				ALICE_print(77, 0, 0, 0, 0, 0);
				// msg 77: A rollback of this transaction will violate two-phase commit.
				print_description(trans);
				switches = ask();
			}
			else if (switches & sw_commit)
				switches |= sw_commit;
			else if (switches & sw_two_phase)
				switches |= sw_commit;
			else if (switches & sw_prompt)
			{
				ALICE_print(78, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 78: Transaction %ld has been partially committed.
				ALICE_print(79, 0, 0, 0, 0, 0);
				// msg 79: A commit is necessary to preserve the two-phase commit.
				print_description(trans);
				switches = ask();
			}
			break;

		case TRA_unknown:
			ALICE_print(80, 0, 0, 0, 0, 0);
			// msg 80: Insufficient information is available to determine
			ALICE_print(81, reinterpret_cast<char*>(trans->tdr_id), 0, 0,
						0, 0);
			// msg 81: a proper action for transaction %ld.
			print_description(trans);
			switches = ask();
			break;

		default:
			if (!(switches & (sw_commit | sw_rollback)))
			{
				ALICE_print(82, reinterpret_cast<char*>(trans->tdr_id), 0,
							0, 0, 0);
				// msg 82: Transaction %ld: All subtransactions have been prepared.
				ALICE_print(83, 0, 0, 0, 0, 0);
				// msg 83: Either commit or rollback is possible.
				print_description(trans);
				switches = ask();
			}
		}
	}

    bool error = false;
	if (switches != (ULONG) -1)
	{
		// now do the required operation with all the subtransactions

		if (switches & (sw_commit | sw_rollback))
		{
			for (TDR ptr = trans; ptr; ptr = ptr->tdr_next)
			{
				if (ptr->tdr_state == TRA_limbo)
				{
					reconnect(ptr->tdr_db_handle,
							  ptr->tdr_id, ptr->tdr_filename, switches);
				}
			}
		}
	}
	else
	{
		ALICE_print(84, 0, 0, 0, 0, 0);	// msg 84: unexpected end of input
		error = true;
	}

//  shutdown all the databases for cleanliness' sake

	TDR_shutdown_databases(trans);

	return error;
}



//____________________________________________________________
//
//		format and print description of a transaction in
//		limbo, including all associated transactions
//		in other databases.
//

static void print_description(const tdr* trans)
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	if (!trans)
	{
		return;
	}

	if (!tdgbl->sw_service_thd)
	{
		ALICE_print(92, 0, 0, 0, 0, 0);	// msg 92:   Multidatabase transaction:
	}

	bool prepared_seen = false;
	for (const tdr* ptr = trans; ptr; ptr = ptr->tdr_next)
	{
		if (ptr->tdr_host_site)
		{
			const char* pszHostSize =
				reinterpret_cast<const char*>(ptr->tdr_host_site->str_data);

#ifndef SUPERSERVER
			// msg 93: Host Site: %s
			ALICE_print(93, pszHostSize, 0, 0, 0, 0);
#else
			const size_t nHostSiteLen = strlen(pszHostSize);

			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_host_site);
			SVC_putc(tdgbl->service_blk, (UCHAR) nHostSiteLen);
			SVC_putc(tdgbl->service_blk, (UCHAR) (nHostSiteLen >> 8 ));
			for (int i = 0; i < (int) nHostSiteLen; i++)
			{
				SVC_putc(tdgbl->service_blk, (UCHAR) pszHostSize[i]);
			}
#endif
		}

		if (ptr->tdr_id)
		{
#ifndef SUPERSERVER
			// msg 94: Transaction %ld
			ALICE_print(94, reinterpret_cast<char*>(ptr->tdr_id), 0, 0, 0, 0);
#else
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_id);
			SVC_putc(tdgbl->service_blk, (UCHAR) ptr->tdr_id);
			SVC_putc(tdgbl->service_blk, (UCHAR) (ptr->tdr_id >> 8));
			SVC_putc(tdgbl->service_blk, (UCHAR) (ptr->tdr_id >> 16));
			SVC_putc(tdgbl->service_blk, (UCHAR) (ptr->tdr_id >> 24));
#endif
		}

		switch (ptr->tdr_state)
		{
		case TRA_limbo:
#ifndef SUPERSERVER
			ALICE_print(95, 0, 0, 0, 0, 0);	// msg 95: has been prepared.
#else
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state);
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state_limbo);
#endif
			prepared_seen = true;
			break;

		case TRA_commit:
#ifndef SUPERSERVER
			ALICE_print(96, 0, 0, 0, 0, 0);	// msg 96: has been committed.
#else
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state);
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state_commit);
#endif
			break;

		case TRA_rollback:
#ifndef SUPERSERVER
			ALICE_print(97, 0, 0, 0, 0, 0);	// msg 97: has been rolled back.
#else
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state);
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state_rollback);
#endif
			break;

		case TRA_unknown:
#ifndef SUPERSERVER
			ALICE_print(98, 0, 0, 0, 0, 0);	// msg 98: is not available.
#else
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state);
			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_state_unknown);
#endif
			break;

		default:
#ifndef SUPERSERVER
			if (prepared_seen)
			{
				// msg 99: is not found, assumed not prepared.
				ALICE_print(99, 0, 0, 0, 0, 0);
			}
			else
			{
				// msg 100: is not found, assumed to be committed.
				ALICE_print(100, 0, 0, 0, 0, 0);
			}
#endif
			break;
		}

		if (ptr->tdr_remote_site)
		{
			const char* pszRemoteSite =
				reinterpret_cast<const char*>(ptr->tdr_remote_site->str_data);

#ifndef SUPERSERVER
			//msg 101: Remote Site: %s
			ALICE_print(101, pszRemoteSite, 0, 0, 0, 0);
#else
			const size_t nRemoteSiteLen = strlen(pszRemoteSite);

			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_remote_site);
			SVC_putc(tdgbl->service_blk, (UCHAR) nRemoteSiteLen);
			SVC_putc(tdgbl->service_blk, (UCHAR) (nRemoteSiteLen >> 8));
			for (int i = 0; i < (int) nRemoteSiteLen; i++)
			{
				SVC_putc(tdgbl->service_blk, (UCHAR) pszRemoteSite[i]);
			}
#endif
		}

		if (ptr->tdr_fullpath)
		{
			const char* pszFullpath =
				reinterpret_cast<const char*>(ptr->tdr_fullpath->str_data);

#ifndef SUPERSERVER
			// msg 102: Database Path: %s
			ALICE_print(102, pszFullpath, 0, 0, 0, 0);
#else
			const size_t nFullpathLen = strlen(pszFullpath);

			SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_db_path);
			SVC_putc(tdgbl->service_blk, (UCHAR) nFullpathLen);
			SVC_putc(tdgbl->service_blk, (UCHAR) (nFullpathLen >> 8));
			for (int i = 0; i < (int) nFullpathLen; i++)
			{
				SVC_putc(tdgbl->service_blk, (UCHAR) pszFullpath[i]);
			}
#endif
		}

	}

//  let the user know what the suggested action is

	switch (TDR_analyze(trans))
	{
	case TRA_commit:
#ifndef SUPERSERVER
		// msg 103: Automated recovery would commit this transaction.
		ALICE_print(103, 0, 0, 0, 0, 0);
#else
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise);
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise_commit);
#endif
		break;

	case TRA_rollback:
#ifndef SUPERSERVER
		// msg 104: Automated recovery would rollback this transaction.
		ALICE_print(104, 0, 0, 0, 0, 0);
#else
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise);
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise_rollback);
#endif
		break;

	default:
#ifdef SUPERSERVER
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise);
		SVC_putc(tdgbl->service_blk, (UCHAR) isc_spb_tra_advise_unknown);
#endif
		break;
	}

}



//____________________________________________________________
//
//		Ask the user whether to commit or rollback.
//

static ULONG ask(void)
{
#ifndef SUPERCLIENT
	return (ULONG)-1;
#else
	char response[32];
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	ULONG switches = 0;

	while (true) {
		ALICE_print(85, 0, 0, 0, 0, 0);
		// msg 85: Commit, rollback, or neither (c, r, or n)?
		if (tdgbl->sw_service)
			putc('\001', stdout);
		fflush(stdout);
		int c;
		char* p;
		for (p = response; (c = getchar()) != '\n' && !feof(stdin) && !ferror(stdin);)
			*p++ = c;
		if (p == response)
			return (ULONG) -1;
		*p = 0;
		ALICE_down_case(response, response, sizeof(response));
		if (!strcmp(response, "n") || !strcmp(response, "c")
			|| !strcmp(response, "r"))
		{
			  break;
		}
	}

	if (response[0] == 'c')
		switches |= sw_commit;
	else if (response[0] == 'r')
		switches |= sw_rollback;

	return switches;
#endif
}


//____________________________________________________________
//
//		Generate pathnames for a given database
//		until the database is successfully attached.
//

static void reattach_database(TDR trans)
{
	ISC_STATUS_ARRAY status_vector;
	char buffer[1024];
	// sizeof(buffer) - 1 => leave space for the terminator.
	const char* const end = buffer + sizeof(buffer) - 1;
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	ISC_get_host(buffer, sizeof(buffer));

 //  if this is being run from the same host,
 //  try to reconnect using the same pathname

	if (!strcmp(buffer,
		reinterpret_cast<const char*>(trans->tdr_host_site->str_data)))
	{
		if (TDR_attach_database(status_vector, trans,
								reinterpret_cast<char*>
								(trans->tdr_fullpath->str_data)))
		{
			return;
		}
	}
	else if (trans->tdr_host_site) {
		//  try going through the previous host with all available
		//  protocols, using chaining to try the same method of
		//  attachment originally used from that host
		char* p = buffer;
		const UCHAR* q = trans->tdr_host_site->str_data;
		while (*q && p < end)
			*p++ = *q++;
		*p++ = ':';
		q = trans->tdr_fullpath->str_data;
		while (*q && p < end)
			*p++ = *q++;
		*p = 0;
		if (TDR_attach_database(status_vector, trans, buffer))
		{
			return;
		}
	}

//  attaching using the old method didn't work;
//  try attaching to the remote node directly

	if (trans->tdr_remote_site) {
		char* p = buffer;
		const UCHAR* q = trans->tdr_remote_site->str_data;
		while (*q && p < end)
			*p++ = *q++;
		*p++ = ':';
		q = reinterpret_cast<const UCHAR*>(trans->tdr_filename);
		while (*q && p < end)
			*p++ = *q++;
		*p = 0;
		if (TDR_attach_database (status_vector, trans, buffer))
		{
			return;
		}
	}

//  we have failed to reattach; notify the user
//  and let them try to succeed where we have failed

	ALICE_print(86, reinterpret_cast<char*>(trans->tdr_id), 0, 0, 0, 0);
	// msg 86: Could not reattach to database for transaction %ld.
	ALICE_print(87, reinterpret_cast<char*>(trans->tdr_fullpath->str_data),
				0, 0, 0, 0);	// msg 87: Original path: %s

#ifdef SUPERCLIENT
	for (;;) {
		ALICE_print(88, 0, 0, 0, 0, 0);	// msg 88: Enter a valid path:
		char* p = buffer;
		while (p < end && (*p = getchar()) != '\n' && !feof(stdin) && !ferror(stdin))
			++p;
		*p = 0;
		if (!buffer[0])
			break;
		p = buffer;
		while (*p == ' ')
			++p;
		if (TDR_attach_database(status_vector, trans, p))
		{
			const size_t p_len = strlen(p);
			alice_str* string = FB_NEW_RPT(*tdgbl->getDefaultPool(),
						p_len + 1) alice_str;
			strcpy(reinterpret_cast<char*>(string->str_data), p);
			string->str_length = p_len;
			trans->tdr_fullpath = string;
			trans->tdr_filename = (TEXT *) string->str_data;
			return;
		}
		ALICE_print(89, 0, 0, 0, 0, 0);	// msg 89: Attach unsuccessful.
	}
#endif
}



//____________________________________________________________
//
//		Attempt to locate all databases used in
//		a multidatabase transaction.
//

static void reattach_databases(TDR trans)
{
	for (TDR ptr = trans; ptr; ptr = ptr->tdr_next)
		reattach_database(ptr);
}



//____________________________________________________________
//
//		Commit or rollback a named transaction.
//

static bool reconnect(FB_API_HANDLE handle,
					  SLONG number,
					  const TEXT* name,
					  ULONG switches)
{
	ISC_STATUS_ARRAY status_vector;

	const SLONG id = gds__vax_integer(reinterpret_cast<const UCHAR*>(&number), 4);
	FB_API_HANDLE transaction = 0;
	if (isc_reconnect_transaction(status_vector, &handle, &transaction,
								   sizeof(id),
								   reinterpret_cast<const char*>(&id)))
	{
		ALICE_print(90, name, 0, 0, 0, 0);
		// msg 90: failed to reconnect to a transaction in database %s
		ALICE_print_status(status_vector);
		return true;
	}

	if (!(switches & (sw_commit | sw_rollback))) {
		ALICE_print(91, reinterpret_cast<char*>(number), 0, 0, 0, 0);
		// msg 91: Transaction %ld:
		switches = ask();
		if (switches == (ULONG) -1) {
			ALICE_print(84, 0, 0, 0, 0, 0);
			// msg 84: unexpected end of input
			return true;
		}
	}

	if (switches & sw_commit)
		isc_commit_transaction(status_vector, &transaction);
	else if (switches & sw_rollback)
		isc_rollback_transaction(status_vector, &transaction);
	else
		return false;

	if (status_vector[1]) {
		ALICE_print_status(status_vector);
		return true;
	}

	return false;
}


