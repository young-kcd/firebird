/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ppg.cpp
 *	DESCRIPTION:	Database page print module
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
 * 2001.08.07 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, second attempt
 */

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd_time.h"
#include "../jrd/ibase.h"
#include "../jrd/ods.h"
#include "../jrd/os/guid.h"
#include "../jrd/nbak.h"
#include "../jrd/gds_proto.h"

#ifdef SUPERSERVER
#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif

#include "../jrd/svc.h"
#include "../jrd/svc_proto.h"
#include "../utilities/gstat/ppg_proto.h"

#define exit(code)	{service->svc_handle = 0; return (code);}
#define FPRINTF		SVC_fprintf
#endif

#ifndef FPRINTF
#define FPRINTF		fprintf
#endif

// gstat directly reads database files, therefore
using namespace Ods;

void PPG_print_header(const header_page* header, SLONG page, bool nocreation,
#ifdef SUPERSERVER
					  Jrd::Service* outfile)
#else
					  FILE* outfile)
#endif
{
/**************************************
 *
 *	P P G _ p r i n t _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Print database header page.
 *
 **************************************/
	SLONG number;
	struct tm time;
	TEXT temp[257];
	SSHORT flag_count = 0;

	if (page == HEADER_PAGE)
		FPRINTF(outfile, "Database header page information:\n");
	else
		FPRINTF(outfile, "Database overflow header page information:\n");


	if (page == HEADER_PAGE) {
		FPRINTF(outfile, "\tFlags\t\t\t%d\n", header->hdr_header.pag_flags);
		FPRINTF(outfile, "\tChecksum\t\t%d\n",
				header->hdr_header.pag_checksum);
		FPRINTF(outfile, "\tGeneration\t\t%"ULONGFORMAT"\n",
				header->hdr_header.pag_generation);
		FPRINTF(outfile, "\tPage size\t\t%d\n", header->hdr_page_size);
		FPRINTF(outfile, "\tODS version\t\t%d.%d\n",
				header->hdr_ods_version & ~ODS_FIREBIRD_FLAG, header->hdr_ods_minor);
		FPRINTF(outfile, "\tOldest transaction\t%"SLONGFORMAT"\n",
				header->hdr_oldest_transaction);
		FPRINTF(outfile, "\tOldest active\t\t%"SLONGFORMAT"\n",
				header->hdr_oldest_active);
		FPRINTF(outfile, "\tOldest snapshot\t\t%"SLONGFORMAT"\n",
				header->hdr_oldest_snapshot);
		FPRINTF(outfile, "\tNext transaction\t%"SLONGFORMAT"\n",
				header->hdr_next_transaction);
		FPRINTF(outfile, "\tBumped transaction\t%"SLONGFORMAT"\n",
				header->hdr_bumped_transaction);
		FPRINTF(outfile, "\tSequence number\t\t%d\n", header->hdr_sequence);

		FPRINTF(outfile, "\tNext attachment ID\t%"SLONGFORMAT"\n",
				header->hdr_attachment_id);
		FPRINTF(outfile, "\tImplementation ID\t%d\n",
				header->hdr_implementation);
		FPRINTF(outfile, "\tShadow count\t\t%"SLONGFORMAT"\n"
				, header->hdr_shadow_count);
		FPRINTF(outfile, "\tPage buffers\t\t%"ULONGFORMAT"\n"
				, header->hdr_page_buffers);
	}

	FPRINTF(outfile, "\tNext header page\t%"ULONGFORMAT"\n"
				, header->hdr_next_page);
#ifdef DEV_BUILD
	FPRINTF(outfile, "\tClumplet End\t\t%d\n", header->hdr_end);
#endif

	if (page == HEADER_PAGE) {

		/* If the database dialect is not set to 3, then we need to
		 * assume it was set to 1.  The reason for this is that a dialect
		 * 1 database has no dialect information written to the header.
		 */
		if (header->hdr_flags & hdr_SQL_dialect_3)
			FPRINTF(outfile, "\tDatabase dialect\t3\n");
		else
			FPRINTF(outfile, "\tDatabase dialect\t1\n");

		if (!nocreation) {
			isc_decode_timestamp(reinterpret_cast<const ISC_TIMESTAMP*>(header->hdr_creation_date),
							&time);
			FPRINTF(outfile, "\tCreation date\t\t%s %d, %d %d:%02d:%02d\n",
					FB_SHORT_MONTHS[time.tm_mon], time.tm_mday, time.tm_year + 1900,
					time.tm_hour, time.tm_min, time.tm_sec);
		}
	}

	USHORT flags;

	if ((page == HEADER_PAGE) && (flags = header->hdr_flags)) {
		FPRINTF(outfile, "\tAttributes\t\t");
		if (flags & hdr_force_write) {
			FPRINTF(outfile, "force write");
			flag_count++;
		}
		if (flags & hdr_no_reserve) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "no reserve");
		}
/*
		if (flags & hdr_disable_cache) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "shared cache disabled");
		}
*/
		if (flags & hdr_active_shadow) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "active shadow");
		}

		const USHORT sd_flags = flags & hdr_shutdown_mask;
		if (sd_flags == hdr_shutdown_multi) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "multi-user maintenance");
		}

		if (sd_flags == hdr_shutdown_single) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "single-user maintenance");
		}

		if (sd_flags == hdr_shutdown_full) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "full shutdown");
		}

		if (flags & hdr_read_only) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			FPRINTF(outfile, "read only");
		}
		if (flags & hdr_backup_mask) {
			if (flag_count++)
				FPRINTF(outfile, ", ");
			if ((flags & hdr_backup_mask) == Jrd::nbak_state_stalled)
				FPRINTF(outfile, "backup lock");
			else
				if ((flags & hdr_backup_mask) == Jrd::nbak_state_merge)
					FPRINTF(outfile, "backup merge");
				else
					FPRINTF(outfile, "wrong backup state %d", flags & hdr_backup_mask);
		}
		FPRINTF(outfile, "\n");
	}

	FPRINTF(outfile, "\n    Variable header data:\n");

	const UCHAR* p = header->hdr_data;
	for (const UCHAR* const end = p + header->hdr_page_size;
		 p < end && *p != HDR_end; p += 2 + p[1])
	{
		switch (*p) {
		case HDR_root_file_name:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tRoot file name:\t\t%s\n", temp);
			break;
/*
		case HDR_journal_server:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tJournal server:\t\t%s\n", temp);
			break;
*/
		case HDR_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tContinuation file:\t\t%s\n", temp);
			break;

		case HDR_last_page:
			memcpy(&number, p + 2, sizeof(number));
			FPRINTF(outfile, "\tLast logical page:\t\t%ld\n", number);
			break;
/*
		case HDR_unlicensed:
			memcpy(&number, p + 2, sizeof(number));
			FPRINTF(outfile, "\tUnlicensed accesses:\t\t%ld\n", number);
			break;
*/
		case HDR_sweep_interval:
			memcpy(&number, p + 2, sizeof(number));
			FPRINTF(outfile, "\tSweep interval:\t\t%ld\n", number);
			break;

		case HDR_log_name:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tReplay logging file:\t\t%s\n", temp);
			break;
/*
		case HDR_cache_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tShared Cache file:\t\t%s\n", temp);
			break;
*/
		case HDR_difference_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			FPRINTF(outfile, "\tBackup difference file:\t%s\n", temp);
			break;

		case HDR_backup_guid:
		{
			char buff[GUID_BUFF_SIZE];
			GuidToString(buff, reinterpret_cast<const FB_GUID*>(p + 2));
			FPRINTF(outfile, "\tDatabase backup GUID:\t%s\n", buff);
			break;
		}

		default:
			if (*p > HDR_max)
				FPRINTF(outfile, "\tUnrecognized option %d, length %d\n",
						p[0], p[1]);
			else
				FPRINTF(outfile, "\tEncoded option %d, length %d\n", p[0],
						p[1]);
			break;
		}
	}

	FPRINTF(outfile, "\t*END*\n");
}


#ifdef NOT_USED_OR_REPLACED
void PPG_print_log(const log_info_page* logp, SLONG page, bool nocreation, 
#ifdef SUPERSERVER
				   Jrd::Service* outfile)
#else
				   FILE* outfile)
#endif
{
/**************************************
 *
 *	P P G _ p r i n t _ l o g
 *
 **************************************
 *
 * Functional description
 *	Print log page information
 *
 **************************************/
	if (page == LOG_PAGE)
		FPRINTF(outfile, "Database log page information:\n");
	else
		FPRINTF(outfile, "Database overflow log page information:\n");

	if (page == LOG_PAGE) {
		if (logp->log_mod_tid)
			FPRINTF(outfile, "\tModified by transaction\t%ld\n",
					logp->log_mod_tid);
		if (logp->log_mod_tip)
			FPRINTF(outfile, "\tModified tip page\t%ld\n", logp->log_mod_tip);

		if (!nocreation) {
			if (!logp->log_creation_date[0] && !logp->log_creation_date[1])
			{
				FPRINTF(outfile, "\tCreation date\n");
			}
			else
			{
				struct tm time;
				isc_decode_timestamp(reinterpret_cast<const ISC_TIMESTAMP*>(logp->log_creation_date),
									&time);
				FPRINTF(outfile,
						"\tCreation date\t%s %d, %d %d:%02d:%02d\n",
						FB_SHORT_MONTHS[time.tm_mon],
						time.tm_mday,
						time.tm_year + 1900,
						time.tm_hour,
						time.tm_min,
						time.tm_sec);
			}
		}

		// Legacy information, zero for FB 2.
		FPRINTF(outfile, "\tLog flags:\t%ld\n", logp->log_flags);
	}

	FPRINTF(outfile, "\tNext log page:\t%ld\n", logp->log_next_page);
#ifdef DEV_BUILD
	FPRINTF(outfile, "\tClumplet End\t%d\n", logp->log_end);
#endif

	/* Firebird doesn't pay attention to this legacy WAL/JOURNAL data.
	FPRINTF(outfile, "\n    Variable log data:\n");

	SLONG ltemp;
	SSHORT stemp;
	USHORT ustemp;
	TEXT temp[257];

	for (const UCHAR* p = logp->log_data; *p != HDR_end; p += 2 + p[1]) {
		switch (*p) {
		case LOG_ctrl_file1:
			FPRINTF(outfile, "\tControl Point 1:\n");
			memcpy(temp, p + 2, logp->log_cp_1.cp_fn_length);
			temp[logp->log_cp_1.cp_fn_length] = '\0';
			FPRINTF(outfile, "\t\tFile name:\t%s\n", temp);
			FPRINTF(outfile, "\t\tPartition offset: %"SLONGFORMAT" ",
					logp->log_cp_1.cp_p_offset);
			FPRINTF(outfile, "\tSeqno: %"SLONGFORMAT" ", logp->log_cp_1.cp_seqno);
			FPRINTF(outfile, "\tOffset: %"SLONGFORMAT" ", logp->log_cp_1.cp_offset);
			FPRINTF(outfile, "\n");

			break;

		case LOG_ctrl_file2:
			FPRINTF(outfile, "\tControl Point 2:\n");
			memcpy(temp, p + 2, logp->log_cp_2.cp_fn_length);
			temp[logp->log_cp_2.cp_fn_length] = '\0';
			FPRINTF(outfile, "\t\tFile name:\t%s\n", temp);
			FPRINTF(outfile, "\t\tPartition offset: %"SLONGFORMAT" ",
					logp->log_cp_2.cp_p_offset);
			FPRINTF(outfile, "\tSeqno: %"SLONGFORMAT" ", logp->log_cp_2.cp_seqno);
			FPRINTF(outfile, "\tOffset: %"SLONGFORMAT" ", logp->log_cp_2.cp_offset);
			FPRINTF(outfile, "\n");

			break;

		case LOG_logfile:
			FPRINTF(outfile, "\tCurrent File:\n");
			memcpy(temp, p + 2, logp->log_file.cp_fn_length);
			temp[logp->log_file.cp_fn_length] = '\0';
			FPRINTF(outfile, "\t\tFile name:\t\t%s\n", temp);
			FPRINTF(outfile, "\t\tPartition offset: %"SLONGFORMAT" ",
					logp->log_file.cp_p_offset);
			FPRINTF(outfile, "\tSeqno: %"SLONGFORMAT" ", logp->log_file.cp_seqno);
			FPRINTF(outfile, "\tOffset: %"SLONGFORMAT" ", logp->log_file.cp_offset);
			FPRINTF(outfile, "\n");

			break;

		case LOG_chkpt_len:
			memcpy(&ltemp, p + 2, p[1]);
			FPRINTF(outfile, "\tCheck Point Length %"SLONGFORMAT"\n", ltemp);
			break;

		case LOG_num_bufs:
			memcpy(&stemp, p + 2, p[1]);
			FPRINTF(outfile, "\tNumber of WAL buffers %d\n", stemp);
			break;

		case LOG_bufsize:
			memcpy(&ustemp, p + 2, p[1]);
			FPRINTF(outfile, "\tWAL buffer Size %u\n", ustemp);
			break;

		case LOG_grp_cmt_wait:
			memcpy(&ltemp, p + 2, p[1]);
			FPRINTF(outfile, "\tGroup Commit Wait Time %"SLONGFORMAT"\n", ltemp);
			break;

		default:
			if (*p > LOG_max)
				FPRINTF(outfile, "\tUnrecognized option %d, length %d\n",
						p[0], p[1]);
			else
				FPRINTF(outfile, "\tEncoded option %d, length %d\n", p[0],
						p[1]);
			break;
		}
	}
	*/
	FPRINTF(outfile, "\t*END*\n");
}
#endif // NOT_USED_OR_REPLACED

