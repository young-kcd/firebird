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
#include "../common/classes/timestamp.h"
#include "../jrd/ibase.h"
#include "../jrd/ods.h"
#include "../jrd/os/guid.h"
#include "../jrd/nbak.h"
#include "../jrd/gds_proto.h"

#include "../utilities/gstat/ppg_proto.h"

// gstat directly reads database files, therefore
using namespace Ods;

void PPG_print_header(const header_page* header, SLONG page,
					  bool nocreation, Firebird::UtilSvc* uSvc)
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
	if (page == HEADER_PAGE)
		uSvc->printf("Database header page information:\n");
	else
		uSvc->printf("Database overflow header page information:\n");


	if (page == HEADER_PAGE)
	{
		uSvc->printf("\tFlags\t\t\t%d\n", header->hdr_header.pag_flags);
		uSvc->printf("\tChecksum\t\t%d\n", header->hdr_header.pag_checksum);
		uSvc->printf("\tGeneration\t\t%"ULONGFORMAT"\n", header->hdr_header.pag_generation);
		uSvc->printf("\tPage size\t\t%d\n", header->hdr_page_size);
		uSvc->printf("\tODS version\t\t%d.%d\n",
				header->hdr_ods_version & ~ODS_FIREBIRD_FLAG, header->hdr_ods_minor);
		uSvc->printf("\tOldest transaction\t%"SLONGFORMAT"\n", header->hdr_oldest_transaction);
		uSvc->printf("\tOldest active\t\t%"SLONGFORMAT"\n", header->hdr_oldest_active);
		uSvc->printf("\tOldest snapshot\t\t%"SLONGFORMAT"\n", header->hdr_oldest_snapshot);
		uSvc->printf("\tNext transaction\t%"SLONGFORMAT"\n", header->hdr_next_transaction);
		uSvc->printf("\tBumped transaction\t%"SLONGFORMAT"\n", header->hdr_bumped_transaction);
		uSvc->printf("\tSequence number\t\t%d\n", header->hdr_sequence);

		uSvc->printf("\tNext attachment ID\t%"SLONGFORMAT"\n", header->hdr_attachment_id);
		uSvc->printf("\tImplementation ID\t%d\n", header->hdr_implementation);
		uSvc->printf("\tShadow count\t\t%"SLONGFORMAT"\n", header->hdr_shadow_count);
		uSvc->printf("\tPage buffers\t\t%"ULONGFORMAT"\n", header->hdr_page_buffers);
	}

	uSvc->printf("\tNext header page\t%"ULONGFORMAT"\n", header->hdr_next_page);
#ifdef DEV_BUILD
	uSvc->printf("\tClumplet End\t\t%d\n", header->hdr_end);
#endif

	if (page == HEADER_PAGE)
	{

		/* If the database dialect is not set to 3, then we need to
		 * assume it was set to 1.  The reason for this is that a dialect
		 * 1 database has no dialect information written to the header.
		 */
		if (header->hdr_flags & hdr_SQL_dialect_3)
			uSvc->printf("\tDatabase dialect\t3\n");
		else
			uSvc->printf("\tDatabase dialect\t1\n");

		if (!nocreation)
		{
			struct tm time;
			isc_decode_timestamp(reinterpret_cast<const ISC_TIMESTAMP*>(header->hdr_creation_date),
							&time);
			uSvc->printf("\tCreation date\t\t%s %d, %d %d:%02d:%02d\n",
					FB_SHORT_MONTHS[time.tm_mon], time.tm_mday, time.tm_year + 1900,
					time.tm_hour, time.tm_min, time.tm_sec);
		}
	}

	ULONG flags;
	int flag_count = 0;

	if ((page == HEADER_PAGE) && (flags = header->hdr_flags))
	{
		uSvc->printf("\tAttributes\t\t");
		if (flags & hdr_force_write) {
			uSvc->printf("force write");
			flag_count++;
		}
		if (flags & hdr_no_reserve) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("no reserve");
		}
/*
		if (flags & hdr_disable_cache) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("shared cache disabled");
		}
*/
		if (flags & hdr_active_shadow) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("active shadow");
		}

		const USHORT sd_flags = flags & hdr_shutdown_mask;
		if (sd_flags == hdr_shutdown_multi) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("multi-user maintenance");
		}

		if (sd_flags == hdr_shutdown_single) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("single-user maintenance");
		}

		if (sd_flags == hdr_shutdown_full) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("full shutdown");
		}

		if (flags & hdr_read_only) {
			if (flag_count++)
				uSvc->printf(", ");
			uSvc->printf("read only");
		}
		if (flags & hdr_backup_mask)
		{
			if (flag_count++)
				uSvc->printf(", ");
			if ((flags & hdr_backup_mask) == Jrd::nbak_state_stalled)
				uSvc->printf("backup lock");
			else
				if ((flags & hdr_backup_mask) == Jrd::nbak_state_merge)
					uSvc->printf("backup merge");
				else
					uSvc->printf("wrong backup state %d", flags & hdr_backup_mask);
		}
		uSvc->printf("\n");
	}

	uSvc->printf("\n    Variable header data:\n");

	SLONG number;
	TEXT temp[257];

	const UCHAR* p = header->hdr_data;
	for (const UCHAR* const end = p + header->hdr_page_size; p < end && *p != HDR_end; p += 2 + p[1])
	{
		switch (*p)
		{
		case HDR_root_file_name:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tRoot file name:\t\t%s\n", temp);
			break;
/*
		case HDR_journal_server:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tJournal server:\t\t%s\n", temp);
			break;
*/
		case HDR_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tContinuation file:\t\t%s\n", temp);
			break;

		case HDR_last_page:
			memcpy(&number, p + 2, sizeof(number));
			uSvc->printf("\tLast logical page:\t\t%ld\n", number);
			break;
/*
		case HDR_unlicensed:
			memcpy(&number, p + 2, sizeof(number));
			uSvc->printf("\tUnlicensed accesses:\t\t%ld\n", number);
			break;
*/
		case HDR_sweep_interval:
			memcpy(&number, p + 2, sizeof(number));
			uSvc->printf("\tSweep interval:\t\t%ld\n", number);
			break;

		case HDR_log_name:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tReplay logging file:\t\t%s\n", temp);
			break;
/*
		case HDR_cache_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tShared Cache file:\t\t%s\n", temp);
			break;
*/
		case HDR_difference_file:
			memcpy(temp, p + 2, p[1]);
			temp[p[1]] = '\0';
			uSvc->printf("\tBackup difference file:\t%s\n", temp);
			break;

		case HDR_backup_guid:
		{
			char buff[GUID_BUFF_SIZE];
			GuidToString(buff, reinterpret_cast<const FB_GUID*>(p + 2));
			uSvc->printf("\tDatabase backup GUID:\t%s\n", buff);
			break;
		}

		default:
			if (*p > HDR_max)
				uSvc->printf("\tUnrecognized option %d, length %d\n", p[0], p[1]);
			else
				uSvc->printf("\tEncoded option %d, length %d\n", p[0], p[1]);
			break;
		}
	}

	uSvc->printf("\t*END*\n");
}
