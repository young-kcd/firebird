/*
 *	PROGRAM:	JRD Cache Manager
 *	MODULE:		sbc_print.cpp
 *	DESCRIPTION:	Shared Cache printer
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "../jrd/common.h"

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

#include "../jrd/isc.h"
#include "../jrd/cash.h"
#include "../jrd/ods.h"
#include "../utilities/gstat/ppg_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/isc_s_proto.h"

#ifdef WIN_NT
#include <io.h>
#include <windows.h>
#define TEXT		SCHAR
#endif

#define SRQ_BASE 			((UCHAR*) CASH_header)

const SLONG DEFAULT_SIZE	= 8192;

static void cache_init(void);
static void db_get_sbc(const SCHAR*, SCHAR*, SLONG*, SSHORT*);

#if (defined WIN_NT)
static void db_error(SLONG);
#else
static void db_error(int);
#endif

static void db_open(const UCHAR*, USHORT);
static PAG db_read(SLONG);
static void print_error(void);
static void print_header(TEXT *);
static void print_page_header(SDB);
static void print_process(PRB);
static void prt_que(UCHAR *, SRQ, FPTR_VOID, int, int);
static void prt_que_back(UCHAR *, SRQ, FPTR_VOID, int, int);
static void print_sccb(void);

static bool sw_mru;
static bool sw_nbuf;
static bool sw_header;
static bool sw_page;
static bool sw_user;
static bool sw_free;
static bool sw_all;
static const TEXT* dbname;
static TEXT sw_file[257];

static SHB CASH_header;
static SCCB sccb;
static SLONG mapped_size = DEFAULT_SIZE;
static SLONG page_no = -1;
static SSHORT page_size = 1024;

static int map_length, map_base, map_count;
static SCHAR *map_region;

#ifdef WIN_NT
static void *file;
#else
static int file;
#endif

SCHAR global_buffer[MAX_PAGE_SIZE];

static FILE* sw_outfile;

const SCHAR* page_type[] = {
	"pag_undefined    ",
	"pag_header       ",		/* Database header page */
	"pag_pages        ",		/* Page inventory page */
	"pag_transactions ",		/* Transaction inventory page */
	"pag_pointer      ",		/* Pointer page */
	"pag_data         ",		/* Data page */
	"pag_root         ",		/* Index root page */
	"pag_index        ",		/* Index (B-tree) page */
	"pag_blob         ",		/* Blob data page */
	"pag_ids          ",		/* Gen-ids */
	"pag_log          "			/* Log page */
};


int CLIB_ROUTINE main( int argc, char* argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Initialize shared cache for process.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	TEXT expanded_filename[256];
	SH_MEM_T shmem_data;
	SLONG cache_buffers;
	SSHORT cache_flags;

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing stdin, stdout, and stderr. */

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		long redir_in = atol(argv[2]);
		long redir_out = atol(argv[3]);
		long redir_err = atol(argv[4]);
#ifdef WIN_NT
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0)
			if (dup2((int) redir_in, 0))
				close((int) redir_in);
		if (redir_out != 1)
			if (dup2((int) redir_out, 1))
				close((int) redir_out);
		if (redir_err != 2)
			if (dup2((int) redir_err, 2))
				close((int) redir_err);
		argv += 4;
		argc -= 4;
	}

	sw_outfile = stderr;

/* Handle switches, etc. */

	dbname = NULL;
	int nbuf = 0;
	int nfree_buf = 0;
	argv++;

	while (--argc) {
		const SCHAR* p = *argv++;

		if (p[0] != '-') {
			dbname = p;
			break;
		}
		SCHAR c;
		while (c = *p++)
			switch (UPPER(c)) {
			case 'M':
				sw_mru = true;
				break;

			case 'N':
				sw_nbuf = true;
				nbuf = atoi(*argv++);
				--argc;
				break;

			case 'P':
				sw_page = true;
				page_no = atoi(*argv++);
				--argc;
				break;

			case 'F':
				sw_free = true;
				nfree_buf = atoi(*argv++);
				--argc;
				break;

			case 'H':
				sw_header = true;
				break;

			case 'A':
				sw_all = true;
				break;

			case 'U':
				sw_user = true;
				break;

			case '-':
				break;

			default:
				print_error();
				exit(FINI_ERROR);
			}
	}

	if (!dbname) {
		print_error();
		exit(FINI_ERROR);
	}

	db_get_sbc(dbname, sw_file, &cache_buffers, &cache_flags);

	if (cache_flags || !cache_buffers) {
		printf("No shared Cache\n");
		exit(FINI_ERROR);
	}

	ISC_expand_filename(sw_file, strlen(sw_file), expanded_filename,
		sizeof(expanded_filename));

#ifdef UNIX
	shmem_data.sh_mem_semaphores = 0;
#endif
	CASH_header = ISC_map_file(status_vector,
							   expanded_filename,
							   cache_init, 0, -mapped_size, &shmem_data);

	if (CASH_header && CASH_header->shb_length > mapped_size) {
		const SLONG length = CASH_header->shb_length;
		ISC_unmap_file(status_vector, &shmem_data, FALSE);
		CASH_header = ISC_map_file(status_vector,
								   expanded_filename,
								   cache_init, 0, length, &shmem_data);
		mapped_size = length;
	}

	if (!CASH_header) {
		printf("Unable to access shared cache\n");
		gds__print_status(status_vector);
		exit(FINI_OK);
	}

	if (sw_all)
		sw_page = sw_header = sw_user = true;

/* print the shared cache header */

	print_header(expanded_filename);

	sccb = (SCCB) SRQ_ABS_PTR(CASH_header->shb_sccb);

/* Print shared cache header block */

	print_sccb();

/* print process queue or just count of processes */

	if (sw_user) {
		prt_que("\tProcesses", &CASH_header->shb_processes, print_process,
				OFFSET(PRB, prb_shb_processes), 0);
		prt_que("\tFree Processes", &CASH_header->shb_free_processes,
				print_process, OFFSET(PRB, prb_shb_processes), 0);
	}
	else {
		prt_que("\tProcesses", &CASH_header->shb_processes, 0, 0, 0);
		prt_que("\tFree Processes", &CASH_header->shb_free_processes, 0, 0,
				0);
	}

/* Print buffers in LRU or MRU order */

	if (!sw_mru) {
		if (sw_header || sw_page) {
			prt_que("\tLRU pages", &sccb->sccb_lru, print_page_header,
					OFFSET(SDB, sdb_lru_free), nbuf);
		}
		else {
			prt_que("\tLRU pages", &sccb->sccb_lru, 0, 0, 0);
		}
	}
	else {
		if (sw_header || sw_page) {
			prt_que_back("\tLRU pages", &sccb->sccb_lru, print_page_header,
						 OFFSET(SDB, sdb_lru_free), nbuf);
		}
		else {
			prt_que_back("\tMRU pages", &sccb->sccb_lru, 0, 0, 0);
		}
	}

/* Print free buffer list */

	if (sw_free) {
		if (sw_header || sw_page) {
			prt_que("\tFree pages", &sccb->sccb_free, print_page_header,
					OFFSET(SDB, sdb_lru_free), nfree_buf);
		}
		else {
			prt_que("\tFree pages", &sccb->sccb_free, 0, 0, 0);
		}
	}

/* Print free waiters */

	prt_que("\tFree waiters", &sccb->sccb_free_waiters, 0, 0, 0);

	printf("\n");

	exit(FINI_OK);
}


static void cache_init(void)
{
/**************************************
 *
 *	c a c h e _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize shared cache for looking 
 *	Just used as a param for ISC_ () call.
 *
 **************************************/
}


static void db_get_sbc(
					   const SCHAR* db,
					   SCHAR* name,
					   SLONG* cache_buffers, SSHORT* cache_flags)
{
/**************************************
 *
 *	d b _ g e t _ s b c
 *
 **************************************
 *
 * Functional description
 *	Get shared cache info.
 *
 **************************************/
	db_open(db, strlen(db));
	header_page* hdr = (header_page*) db_read((SLONG) HEADER_PAGE);

	*cache_buffers = hdr->hdr_cache_buffers;
	*cache_flags = hdr->hdr_flags & hdr_disable_cache;
	strcpy(name, hdr->hdr_cache_file + 2);

	page_size = hdr->hdr_page_size;
}


#ifdef WIN_NT
static void db_error( SLONG status)
{
/**************************************
 *
 *	d b _ e r r o r		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	TEXT s[128];

	if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
					   NULL,
					   status,
					   GetUserDefaultLangID(),
					   s,
					   sizeof(s),
					   NULL))
			sprintf(s, "unknown Windows NT error %ld", status);

	printf(s);
	exit(FINI_ERROR);
}


static void db_open(const UCHAR* file_name, USHORT file_length)
{
/**************************************
 *
 *	d b _ o p e n		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Open a database file.
 *
 **************************************/

	if ((file = CreateFile(file_name,
						   GENERIC_READ | GENERIC_WRITE,
						   FILE_SHARE_READ | FILE_SHARE_WRITE,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
						   0)) == INVALID_HANDLE_VALUE)
			db_error(GetLastError());
}


static PAG db_read( SLONG page_number)
{
/**************************************
 *
 *	d b _ r e a d		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Read a database page.
 *
 **************************************/
	SLONG actual_length;
	LARGE_INTEGER liOffset;

	liOffset.QuadPart = UInt32x32To64((DWORD) page_number, (DWORD) page_size);
	if (SetFilePointer
		(file, (LONG) liOffset.LowPart, &liOffset.HighPart, FILE_BEGIN) == -1)
		db_error(GetLastError());

	if (!ReadFile(file, global_buffer, page_size, &actual_length, NULL) ||
		actual_length != page_size)
		db_error(GetLastError());

	return global_buffer;
}
#endif


#ifndef WIN_NT

static void db_error( int status)
{
/**************************************
 *
 *	d b _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SCHAR *p;

	/* FIXME: The strerror() function returns the appropriate description
	   string, or an unknown error message if the error code is unknown.
	   EKU: p cannot be NULL! */
#if 1
	printf(strerror(status));
#else
#ifndef VMS
	printf(strerror(status));
#else
	if ((p = strerror(status)) || (p = strerror(EVMSERR, status)))
		printf("%s\n", p);
	else
		printf("uninterpreted code %x\n", status);
#endif
#endif
	exit(FINI_ERROR);
}


static void db_open(const UCHAR* file_name, USHORT file_length)
{
/**************************************
 *
 *	d b _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a database file.
 *
 **************************************/

	if ((file = open(file_name, 2)) == -1)
		db_error(errno);
}


static PAG db_read( SLONG page_number)
{
/**************************************
 *
 *	d b _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a database page.
 *
 **************************************/
	SCHAR *p;
	SSHORT length, l;

	UINT64 offset;
	offset = ((UINT64)page_number) * ((UINT64)page_size);
	if (lseek (file, offset, 0) == -1)
		db_error(errno);

	for (p = (SCHAR *) global_buffer, length = page_size; length > 0;) {
		l = read(file, p, length);
		if (l < 0)
			db_error(errno);
		p += l;
		length -= l;
	}

	return global_buffer;
}

#endif	// !WIN_NT


static void print_error(void)
{
/**************************************
 *
 *	p r i n t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	printf("gds_cache_print <switches> <db name>\n");
	printf("Switches are:\n");
	printf("\t-m\t\t- mru order\n");
	printf("\t-n <buffers>\t- number of buffers\n");
	printf("\t-p <page>\t- specific page\n");
	printf("\t-f <buffers>\t- number of free\n");
	printf("\t-h \t\t- page headers\n");
	printf("\t-a \t\t- all except free pages\n");
	printf("\t-u \t\t- user process information\n");
}


static void print_header( TEXT * expanded_filename)
{
/**************************************
 *
 *	p r i n t _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Print shared cache header.
 *
 **************************************/
	float cache_hit;

/* Print shared cache header block */

	printf("\nSHARED_CACHE BLOCK \t%s\n", expanded_filename);
	printf("\tVersion: %d.%d, Active process: %d, Length: %d, Used: %d\n",
			  CASH_header->shb_major_version,
			  CASH_header->shb_minor_version,
			  CASH_header->shb_active_process,
			  CASH_header->shb_length, CASH_header->shb_used);

	printf("\tBuffer waits: %d, IO waits: %d, Cache repairs: %d\n",
			  CASH_header->shb_buffer_waits,
			  CASH_header->shb_io_waits, CASH_header->shb_cache_repairs);

	printf
		("\tLast Checkpoint Seqno: %d, Offset: %d, Partition offset: %d\n",
		 CASH_header->shb_last_chk_seqno, CASH_header->shb_last_chk_offset,
		 CASH_header->shb_last_chk_p_offset);

	printf("\tShort recovers: %d, Long recovers: %d\n",
			  CASH_header->shb_short_recovers,
			  CASH_header->shb_long_recovers);

	printf("\tLogical Reads: %d, Physical Reads: %d\n",
			  CASH_header->shb_logical_reads,
			  CASH_header->shb_physical_reads);

	printf("\tLogical Writes: %d, Physical Writes: %d\n",
			  CASH_header->shb_logical_writes,
			  CASH_header->shb_physical_writes);

	cache_hit =
		(float) (CASH_header->shb_logical_reads -
				 CASH_header->shb_physical_reads) /
		CASH_header->shb_logical_reads;

	printf("\tCache hit ratio: %3.1f\n", cache_hit * 100.);
	printf("\n");
}


static void print_page_header( SDB sdb)
{
/**************************************
 *
 *	p r i n t _ p a g e _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Print database page header.  Print full page for
 *	header and log pages.
 *
 **************************************/
	if ((page_no >= 0) && (sdb->sdb_page != page_no)) {
		return;
	}

/* print sdb information */

	printf
		("\tPage: %ld Generation: %ld Length: %d SDB Flags: %d Precedence: %d\n",
		 sdb->sdb_page, sdb->sdb_generation, sdb->sdb_length, sdb->sdb_flags,
		 sdb->sdb_precedence);

	const pag* page = (pag*) SRQ_ABS_PTR(sdb->sdb_buffer);

/* Print page header */

	printf("\tPage type: %s\n", page_type[(int) page->pag_type]);

	printf("\tFlags: %d.  Generation: %d.  SCN: %d.\n",
			  (int) page->pag_flags, page->pag_generation,
			  page->pag_scn);

/* Print full page information for header and log page */

	if (page_no >= 0) {
		if (page_no == HEADER_PAGE) {
			PPG_print_header(page, HEADER_PAGE, sw_outfile);
		}
		else if (page_no == LOG_PAGE) {
			PPG_print_log(page, LOG_PAGE, sw_outfile);
		}
	}

	if (sdb->sdb_flags) {
		printf("\tSDB Flags\n");

		if (sdb->sdb_flags & SDB_dirty)
			printf("\t\tPage Dirty\n");
		if (sdb->sdb_flags & SDB_marked)
			printf("\t\tPage Marked\n");
		if (sdb->sdb_flags & SDB_faked)
			printf("\t\tPage Faked\n");
		if (sdb->sdb_flags & SDB_journal)
			printf("\t\tJournal records present\n");
		if (sdb->sdb_flags & SDB_journal_page)
			printf("\t\tFull page journal\n");
		if (sdb->sdb_flags & SDB_write_pending)
			printf("\t\tWrite pending\n");
		if (sdb->sdb_flags & SDB_read_pending)
			printf("\t\tRead pending\n");
		if (sdb->sdb_flags & SDB_free)
			printf("\t\tFree\n");
		if (sdb->sdb_flags & SDB_log_recovery)
			printf("\t\tLog recovery\n");
		if (sdb->sdb_flags & SDB_check_use)
			printf("\t\tCheck use\n");
	}

	if (sdb->sdb_journal) {
		SDB journal_sdb = (SDB) SRQ_ABS_PTR(sdb->sdb_journal);
		printf("\tJournal buffer information:\n");
		printf("\t\tCurrent Length %d\n", journal_sdb->sdb_length);
	}

	printf("\n");
}


// CVC: This PRB doesn't match jrd/event.h's prb struct.
static void print_process( PRB process)
{
/**************************************
 *
 *	p r i n t _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Print a formatted process block.
 *
 **************************************/

	printf("PROCESS BLOCK %d\n", SRQ_REL_PTR(process));

	printf("\tProcess id: %d, IO wait: %d, IO read: %d, flags: %d\n",
			  process->prb_process_id, process->prb_waiter,
			  process->prb_reader, process->prb_flags);

	printf("\tPhysical Reads: %d, Logical Reads: %d\n",
			  process->prb_physical_reads, process->prb_logical_reads);

	printf("\tLogical Writes: %d\n", process->prb_logical_writes);

	printf("\n");
}


static void prt_que(
					UCHAR * string,
					SRQ que, void (*routine) (), int param1, int param2)
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print the contents of a self-relative que.
 *	If a routine is passed, print more information about the
 *	elements of the queue.
 *	param 1 - offset of the que structure within the element we
 *		  are traversing.
 *	param 2	- if specified, the number of entries to print.
 *
 **************************************/
	const SLONG offset = SRQ_REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward) {
		printf("%s: *empty*\n\n", string);
		return;
	}

	SLONG count = 0;

	SRQ next;
	SRQ_LOOP((*que), next) {
		++count;

		if ((param2) && (count > param2))
			continue;

		if (routine)
			(*routine) ((UCHAR *) next - param1);
	}

	printf("%s (%ld):\tforward: %d, backward: %d\n\n",
			  string, count, que->srq_forward, que->srq_backward);
}


static void prt_que_back(
						 UCHAR * string,
						 SRQ que, void (*routine) (), int param1, int param2)
{
/**************************************
 *
 *	p r t _ q u e _ b a c k
 *
 **************************************
 *
 * Functional description
 *	Same as prt_que, but traverse in reverse order.
 *
 **************************************/
	const SLONG offset = SRQ_REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward) {
		printf("%s: *empty*\n\n", string);
		return;
	}

	SLONG count = 0;

	SRQ next;
	SRQ_LOOP_BACK((*que), next) {
		++count;

		if ((param2) && (count > param2))
			continue;

		if (routine)
			(*routine) ((UCHAR *) next - param1);
	}

	printf("%s (%ld):\tforward: %d, backward: %d\n\n",
			  string, count, que->srq_forward, que->srq_backward);
}


static void print_sccb(void)
{
/**************************************
 *
 *	p r i n t _ s c c b
 *
 **************************************
 *
 * Functional description
 *	Print sccb header
 *
 **************************************/

/* Print shared cache header block */

	printf("\tBuffers: %d, Free: %d, Free min: %d, Checkpoint : %d\n",
			  sccb->sccb_count,
			  sccb->sccb_free_count,
			  sccb->sccb_free_min, sccb->sccb_checkpoint);

	printf("\tflags: %d\n\n", sccb->sccb_flags);
}

