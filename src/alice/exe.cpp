//____________________________________________________________
//
//		PROGRAM:	Alice (All Else) Utility
//		MODULE:		exe.cpp
//		DESCRIPTION:	Does the database calls
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
//	$Id: exe.cpp,v 1.14 2003-09-08 01:46:46 brodsom Exp $
//
// 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
//                         conditionals, as the engine now fully supports
//                         readonly databases.
//
// 2002.10.30 Sean Leyne - Removed obsolete "PC_PLATFORM" define
//

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/gds.h"
#include "../jrd/common.h"
#include "../jrd/ibsetjmp.h"
#include "../alice/alice.h"
#include "../alice/aliceswi.h"
#include "../alice/all.h"
#include "../alice/all_proto.h"
#include "../alice/alice_meta.h"
#include "../alice/tdr_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"


static USHORT build_dpb(UCHAR *, ULONG);
static void extract_db_info(UCHAR *);

static TEXT val_errors[] =
{
	isc_info_page_errors, isc_info_record_errors, isc_info_bpage_errors,
	isc_info_dpage_errors, isc_info_ipage_errors, isc_info_ppage_errors,
	isc_info_tpage_errors, gds_info_end
};

#define STUFF_DPB(blr)  {*d++ = (UCHAR)(blr);}
#define STUFF_DPB_INT(blr)			\
	{								\
		STUFF_DPB(blr);				\
		STUFF_DPB((blr) >> 8);		\
		STUFF_DPB((blr) >> 16);		\
		STUFF_DPB((blr) >> 24);		\
	}



//____________________________________________________________
//
//

int EXE_action(TEXT * database, ULONG switches)
{
	UCHAR dpb[128];
	USHORT dpb_length, error;
	FRBRD *handle;
	UCHAR error_string[128];
	USHORT i;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	ALLA_init();

	for (i = 0; i < MAX_VAL_ERRORS; i++)
		tdgbl->ALICE_data.ua_val_errors[i] = 0;

//  generate the database parameter block for the attach,
//  based on the various switches

	dpb_length = build_dpb(dpb, switches);

	error = FALSE;
	handle = NULL;
	gds__attach_database(tdgbl->status, 0, database, &handle, dpb_length,
						 reinterpret_cast <SCHAR *>(dpb));

	SVC_STARTED(tdgbl->service_blk);

	if (tdgbl->status[1])
		error = TRUE;

	if (tdgbl->status[2] == isc_arg_warning)
		ALICE_print_status(tdgbl->status);

	if (handle != NULL) {
		if ((switches & sw_validate) && (tdgbl->status[1] != isc_bug_check)) {
			gds__database_info(tdgbl->status, &handle, sizeof(val_errors),
							   val_errors, sizeof(error_string),
							   reinterpret_cast < char *>(error_string));

			extract_db_info(error_string);
		}

		if (switches & sw_disable)
			MET_disable_wal(tdgbl->status, handle);

		gds__detach_database(tdgbl->status, &handle);
	}

	ALLA_fini();

	return ((error) ? FINI_ERROR : FINI_OK);
}


//____________________________________________________________
//
//

int EXE_two_phase(TEXT * database, ULONG switches)
{
	UCHAR dpb[128];
	USHORT dpb_length, error;
	FRBRD *handle;
	USHORT i;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	ALLA_init();

	for (i = 0; i < MAX_VAL_ERRORS; i++)
		tdgbl->ALICE_data.ua_val_errors[i] = 0;

//  generate the database parameter block for the attach,
//  based on the various switches

	dpb_length = build_dpb(dpb, switches);

	error = FALSE;
	handle = NULL;
	gds__attach_database(tdgbl->status, 0, database, &handle,
						 dpb_length,  reinterpret_cast < char *>(dpb));

	SVC_STARTED(tdgbl->service_blk);

	if (tdgbl->status[1])
		error = TRUE;
	else if (switches & sw_list)
		TDR_list_limbo((handle), database, switches);
	else if (switches & (sw_commit | sw_rollback | sw_two_phase))
		error =
			TDR_reconnect_multiple((handle),
								   tdgbl->ALICE_data.ua_transaction, database,
								   switches);

	if (handle)
		gds__detach_database(tdgbl->status, &handle);

	ALLA_fini();

	return ((error) ? FINI_ERROR : FINI_OK);
}

//____________________________________________________________
//
//
//  generate the database parameter block for the attach,
//  based on the various switches
//

static USHORT build_dpb(UCHAR * dpb, ULONG switches)
{
	UCHAR *d;
	USHORT dpb_length;
	SSHORT i;
	TEXT *q;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	d = dpb;
	*d++ = gds_dpb_version1;
	*d++ = isc_dpb_gfix_attach;
	*d++ = 0;

	if (switches & sw_sweep) {
		*d++ = gds_dpb_sweep;
		*d++ = 1;
		*d++ = gds_dpb_records;
	}
	else if (switches & sw_activate) {
		*d++ = gds_dpb_activate_shadow;
		*d++ = 0;
	}
	else if (switches & sw_validate) {
		*d++ = gds_dpb_verify;
		*d++ = 1;
		*d = gds_dpb_pages;
		if (switches & sw_full)
			*d |= gds_dpb_records;
		if (switches & sw_no_update)
			*d |= gds_dpb_no_update;
		if (switches & sw_mend)
			*d |= gds_dpb_repair;
		if (switches & sw_ignore)
			*d |= gds_dpb_ignore;
		d++;
	}
	else if (switches & sw_housekeeping) {
		*d++ = gds_dpb_sweep_interval;
		*d++ = 4;
		for (i = 0; i < 4;
			 i++, tdgbl->ALICE_data.ua_sweep_interval =
			 tdgbl->ALICE_data.ua_sweep_interval >> 8)
			/* TMN: Here we should really have the following assert */
			/* assert(tdgbl->ALICE_data.ua_sweep_interval <= MAX_UCHAR); */
			*d++ = (UCHAR) tdgbl->ALICE_data.ua_sweep_interval;
	}
	else if (switches & sw_begin_log) {
		*d++ = gds_dpb_begin_log;
		*d++ = strlen(tdgbl->ALICE_data.ua_log_file);
		for (q = tdgbl->ALICE_data.ua_log_file; *q;)
			*d++ = *q++;
	}
	else if (switches & sw_buffers) {
		*d++ = isc_dpb_set_page_buffers;
		*d++ = 4;
		for (i = 0; i < 4;
			 i++, tdgbl->ALICE_data.ua_page_buffers =
			 tdgbl->ALICE_data.ua_page_buffers >> 8)
			/* TMN: Here we should really have the following assert */
			/* assert(tdgbl->ALICE_data.ua_page_buffers <= MAX_UCHAR); */
			*d++ = (UCHAR) tdgbl->ALICE_data.ua_page_buffers;
	}
	else if (switches & sw_quit_log) {
		*d++ = gds_dpb_quit_log;
		*d++ = 0;
	}
	else if (switches & sw_kill) {
		*d++ = gds_dpb_delete_shadow;
		*d++ = 0;
	}
	else if (switches & sw_write) {
		*d++ = gds_dpb_force_write;
		*d++ = 1;
		*d++ = tdgbl->ALICE_data.ua_force;
	}
	else if (switches & sw_use) {
		*d++ = gds_dpb_no_reserve;
		*d++ = 1;
		*d++ = tdgbl->ALICE_data.ua_use;
	}

	else if (switches & sw_mode) {
		*d++ = isc_dpb_set_db_readonly;
		*d++ = 1;
		*d++ = tdgbl->ALICE_data.ua_read_only;
	}
	else if (switches & sw_shut) {
		*d++ = gds_dpb_shutdown;
		*d++ = 1;
		*d = 0;
		if (switches & sw_attach)
			*d |= gds_dpb_shut_attachment;
		else if (switches & sw_cache)
			*d |= gds_dpb_shut_cache;
		else if (switches & sw_force)
			*d |= gds_dpb_shut_force;
		else if (switches & sw_tran)
			*d |= gds_dpb_shut_transaction;
		d++;
		*d++ = gds_dpb_shutdown_delay;
		*d++ = 2;				/* Build room for shutdown delay */
		/* TMN: Here we should really have the following assert */
		/* assert(tdgbl->ALICE_data.ua_page_buffers <= MAX_USHORT); */
		/* or maybe even compare with MAX_SSHORT */
		*d++ = (UCHAR) tdgbl->ALICE_data.ua_shutdown_delay;
		*d++ = (UCHAR) (tdgbl->ALICE_data.ua_shutdown_delay >> 8);
	}
	else if (switches & sw_online) {
		*d++ = gds_dpb_online;
		*d++ = 0;
	}
	else if (switches & sw_disable) {
		*d++ = isc_dpb_disable_wal;
		*d++ = 0;
	}
	else if (switches & (sw_list | sw_commit | sw_rollback | sw_two_phase)) {
		*d++ = gds_dpb_no_garbage_collect;
		*d++ = 0;
	}
	else if (switches & sw_set_db_dialect) {
		STUFF_DPB(isc_dpb_set_db_sql_dialect);
		STUFF_DPB(4);
		STUFF_DPB_INT(tdgbl->ALICE_data.ua_db_SQL_dialect);
	}

	if (tdgbl->ALICE_data.ua_user) {
		*d++ = gds_dpb_user_name;
		*d++ =
			strlen(reinterpret_cast <
				   const char *>(tdgbl->ALICE_data.ua_user));
		for (q = reinterpret_cast < TEXT * >(tdgbl->ALICE_data.ua_user); *q;)
			*d++ = *q++;
	}

	if (tdgbl->ALICE_data.ua_password) {
		if (!tdgbl->sw_service)
			*d++ = gds_dpb_password;
		else
			*d++ = gds_dpb_password_enc;
		*d++ =
			strlen(reinterpret_cast <
				   const char *>(tdgbl->ALICE_data.ua_password));
		for (q = reinterpret_cast < TEXT * >(tdgbl->ALICE_data.ua_password);
			 *q;)
			*d++ = *q++;
	}

	dpb_length = d - dpb;
	if (dpb_length == 1)
		dpb_length = 0;

	return dpb_length;
}


//____________________________________________________________
//
//		Extract database info from string
//

static void extract_db_info(UCHAR * db_info_buffer)
{
	UCHAR item;
	UCHAR *p;
	SLONG length;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	p = db_info_buffer;

	while ((item = *p++) != gds_info_end) {
		length = gds__vax_integer(p, 2);
		p += 2;

		/* TMN: Here we should really have the following assert */
		/* assert(length <= MAX_SSHORT); */
		/* for all cases that use 'length' as input to 'gds__vax_integer' */
		switch (item) {
		case isc_info_page_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_record_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_RECORD_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_bpage_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_BLOB_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_dpage_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_DATA_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_ipage_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_INDEX_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_ppage_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_POINTER_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_tpage_errors:
			tdgbl->ALICE_data.ua_val_errors[VAL_TIP_PAGE_ERRORS] =
				gds__vax_integer(p, (SSHORT) length);
			p += length;
			break;

		case isc_info_error:
			/* has to be a < V4 database. */

			tdgbl->ALICE_data.ua_val_errors[VAL_INVALID_DB_VERSION] = 1;
			return;

		default:
			;
		}
	}
}

