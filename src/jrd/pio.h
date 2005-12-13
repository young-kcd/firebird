/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pio.h
 *	DESCRIPTION:	File system interface definitions
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _JRD_PIO_H_
#define _JRD_PIO_H_

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

#ifdef UNIX

class fil : public pool_alloc_rpt<SCHAR, type_fil>
{
    public:
	struct fil *fil_next;		/* Next file in database */
	ULONG fil_min_page;			/* Minimum page number in file */
	ULONG fil_max_page;			/* Maximum page number in file */
	USHORT fil_sequence;		/* Sequence number of file */
	USHORT fil_fudge;			/* Fudge factor for page relocation */
	struct plc *fil_connect;	/* Connection to page server */
	int fil_desc;
	int *fil_trace;				/* Trace file, if any */
	MUTX_T fil_mutex[1];
	USHORT fil_flags;
	USHORT fil_length;			/* Length of expanded file name */
	SCHAR fil_string[1];		/* Expanded file name */
};
typedef fil *FIL;

#endif


#ifdef VMS

class fil : public pool_alloc_rpt<SCHAR, type_fil>
{
    public:
	struct fil *fil_next;		/* Next file in database */
	ULONG fil_min_page;			/* Minimum page number in file */
	ULONG fil_max_page;			/* Maximum page number in file */
	USHORT fil_sequence;		/* Sequence number of file */
	USHORT fil_fudge;			/* Fudge factor for page relocation */
	struct plc *fil_connect;	/* Connection to page server */
	int fil_desc;
	int fil_trace;				/* Trace file, if any */
	MUTX_T fil_mutex[1];
	USHORT fil_length;			/* Length of expanded file name */
	USHORT fil_fid[3];			/* File id */
	USHORT fil_did[3];			/* Directory id */
	USHORT fil_flags;
	SCHAR fil_string[1];		/* Expanded file name */
};
tyepdef fil *FIL;

#endif

#ifdef WIN_NT
#ifdef SUPERSERVER_V2
#define MAX_FILE_IO	32			/* Maximum "allocated" overlapped I/O events */
#endif

class fil : public pool_alloc_rpt<SCHAR, type_fil>
{
    public:
	struct fil *fil_next;		/* Next file in database */
	ULONG fil_min_page;			/* Minimum page number in file */
	ULONG fil_max_page;			/* Maximum page number in file */
	USHORT fil_sequence;		/* Sequence number of file */
	USHORT fil_fudge;			/* Fudge factor for page relocation */
	struct plc *fil_connect;	/* Connection to page server */
	SLONG fil_desc;
	SLONG fil_force_write_desc;	/* Handle of force write open */
	int *fil_trace;				/* Trace file, if any */
	MUTX_T fil_mutex[1];
#ifdef SUPERSERVER_V2
	void* fil_io_events[MAX_FILE_IO];	/* Overlapped I/O events */
#endif
	USHORT fil_flags;
	USHORT fil_length;			/* Length of expanded file name */
	SCHAR fil_string[1];		/* Expanded file name */
};
typedef fil *FIL;

#endif


#define FIL_force_write		1
#define FIL_force_write_init	2

/* Physical IO trace events */

#define trace_create	1
#define trace_open	2
#define trace_page_size	3
#define trace_read	4
#define trace_write	5
#define trace_close	6

/* Physical I/O status block */

typedef struct piob {
	FIL piob_file;				/* File being read/written */
	SLONG piob_desc;			/* File descriptor */
	SLONG piob_io_length;		/* Requested I/O transfer length */
	SLONG piob_actual_length;	/* Actual I/O transfer length */
	USHORT piob_wait;			/* Async or synchronous wait */
	UCHAR piob_flags;
	SLONG piob_io_event[8];		/* Event to signal I/O completion */
} *PIOB;

#define	PIOB_error	1			/* I/O error occurred */
#define PIOB_success	2		/* I/O successfully completed */
#define PIOB_pending	4		/* Asynchronous I/O not yet completed */

#endif /* _JRD_PIO_H_ */
