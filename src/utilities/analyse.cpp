/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		Analyse.c
 *	DESCRIPTION:	I/O trace analysis
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

#include "../jrd/common.h"

#ifdef VMS
#include "firebird.h"
#include <types.h>
#include "times.h"
#else
#include <sys/types.h>
#include <sys/times.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#endif

#include "../jrd/ib_stdio.h"
#include <errno.h>
#include "jrd.h"
#include "ods.h"

extern SCHAR *sys_errlist[];

static void analyse(int, SCHAR *, PAG, int);
static SLONG get_long(void);
static void db_error(int);
static void db_open(UCHAR *, USHORT);
static PAG db_read(SLONG);

static IB_FILE *trace;
static int file;

/* Physical IO trace events */

#define trace_create	1
#define trace_open	2
#define trace_page_size	3
#define trace_read	4
#define trace_write	5
#define trace_close	6

static USHORT page_size;
static int map_length, map_base, map_count;
static PAG global_buffer;
static UCHAR *map_region;

#define MAX_PAGES	50000

static USHORT read_counts[MAX_PAGES], write_counts[MAX_PAGES];


void main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Replay all I/O to compute overhead of I/O system.
 *
 **************************************/
	SSHORT event, detail;
	USHORT *r, *w;
	PAG *page;
	SLONG reads, writes, n, cpu, elapsed, system, length, sequence;
	SCHAR string[128], *p, *end;
	struct tms after, before;

	detail = TRUE;
	sequence = 0;

	for (end = argv + argc, ++argv; argv < end; argv++) {
		p = *argv;
		if (*p++ == '-')
			switch (UPPER(*p)) {
			case 'S':
				detail = FALSE;
				break;
			}
	}


	reads = writes = 0;
	trace = ib_fopen("trace.log", "r");
	page_size = 1024;

	elapsed = times(&before);

	while ((event = ib_getc(trace)) != trace_close && event != EOF)
		switch (event) {
		case trace_open:
			n = length = ib_getc(trace);
			p = string;
			while (--n >= 0)
				*p++ = ib_getc(trace);
			*p = 0;
			db_open(string, length);
			break;

		case trace_page_size:
			page_size = get_long();
			if (global_buffer)
				free(global_buffer);
			break;

		case trace_read:
			n = get_long();
			++read_counts[n];
			if (detail && (page = db_read(n)))
				analyse(n, "Read", page, ++sequence);
			reads++;
			break;

		case trace_write:
			n = get_long();
			++write_counts[n];
			if (detail && (page = db_read(n)))
				analyse(n, "Write", page, ++sequence);
			writes++;
			break;

		default:
			ib_printf("don't understand event %d\n", event);
			abort();
		}

	elapsed = times(&after) - elapsed;
	cpu = after.tms_utime - before.tms_utime;
	system = after.tms_stime - before.tms_stime;

	ib_printf
		("File: %s:\n elapsed = %d.%.2d, cpu = %d.%.2d, system = %d.%.2d, reads = %d, writes = %d\n",
		 string, elapsed / 60, (elapsed % 60) * 100 / 60, cpu / 60,
		 (cpu % 60) * 100 / 60, system / 60, (system % 60) * 100 / 60, reads,
		 writes);

	ib_printf("High activity pages:\n");

	for (r = read_counts, w = write_counts, n = 0; n < MAX_PAGES;
		 n++, r++, w++)
		if (*r > 1 || *w > 1) {
			sprintf(string, "  Read: %d, write: %d", *r, *w);
			if (page = db_read(n))
				analyse(n, string, page, 0);
		}
}


static void analyse( int number, SCHAR * string, PAG page, int sequence)
{
/**************************************
 *
 *	a n a l y s e
 *
 **************************************
 *
 * Functional description
 *	Analyse a page event.
 *
 **************************************/

	if (sequence)
		ib_printf("%d.  %s\t%d\t\t", sequence, string, number);
	else
		ib_printf("%s\t%d\t\t", string, number);

	switch (page->pag_type) {
	case pag_header:
		ib_printf("Header page\n");
		break;

	case pag_pages:
		ib_printf("Page inventory page\n");
		break;

	case pag_transactions:
		ib_printf("Transaction inventory page\n");
		break;

	case pag_pointer:
		ib_printf("Pointer page, relation %d, sequence %d\n",
				  ((PPG) page)->ppg_relation, ((PPG) page)->ppg_sequence);
		break;

	case pag_data:
		ib_printf("Data page, relation %d, sequence %d\n",
				  ((DPG) page)->dpg_relation, ((DPG) page)->dpg_sequence);
		break;

	case pag_root:
		ib_printf("Index root page, relation %d\n",
				  ((IRT) page)->irt_relation);
		break;

	case pag_index:
		ib_printf("B-Tree page, relation %d, index %d, level %d\n",
				  ((BTR) page)->btr_relation, ((BTR) page)->btr_id,
				  ((BTR) page)->btr_level);
		break;

	case pag_blob:
		ib_printf
			("Blob page\n\tFlags: %x, lead page: %d, sequence: %d, length: %d\n\t",
			 page->pag_flags, ((BLP) page)->blp_lead_page,
			 ((BLP) page)->blp_sequence, ((BLP) page)->blp_length);

		break;

	default:
		ib_printf("Unknown type %d\n", page->pag_type);
		break;
	}
}


static SLONG get_long(void)
{
/**************************************
 *
 *	g e t _ l o n g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	union {
		SLONG l;
		SSHORT i;
		SCHAR c;
	} value;
	SLONG n, i, x;
	SCHAR *p;

	p = (SCHAR *) & value.l;
	x = i = ib_getc(trace);

	while (--i >= 0)
		*p++ = ib_getc(trace);

	if (x == 1)
		return value.c;
	else if (x == 2)
		return value.i;
	else
		return value.l;
}


#ifdef VMS
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

	ib_printf(sys_errlist[status]);
	abort();
}


static void db_open( UCHAR * file_name, USHORT file_length)
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

	UINT64 offset = ((UINT64)page_number) * ((UINT64)page_size);

	if (!global_buffer)
		global_buffer = malloc(page_size);

	if (lseek (file, offset, 0) == -1)
		db_error(errno);

	if (read(file, global_buffer, page_size) == -1)
		db_error(errno);

	return global_buffer;
}
#endif
