/*
 *	PROGRAM:	JRD Sort
 *	MODULE:		sort.h
 *	DESCRIPTION:	Sort package definitions
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

#ifndef _JRD_SORT_H_
#define _JRD_SORT_H_

#include "../jrd/common.h"
#include "../jrd/fil.h"

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

// Forward declaration
class SortMem;

#ifdef __cplusplus
extern "C" {
#endif

/* SORTP is used throughout sort.c as a pointer into arrays of
   longwords(32 bits).   For 16 bit Windows, this must be a huge pointer. 

   Use this definition whenever doing pointer arithmetic, as
   interbase variables (eg. scb->scb_longs) are in 32 - bit longwords. */

typedef ULONG SORTP;

/* since the first part of the record contains a back_pointer, whose
   size depends on the platform (ie 16, 32 or 64 bits.).
   This pointer data_type is defined by platform specific SORT_PTR.
*/

typedef IPTR SORT_PTR;

/* # of 32 bit longs in a pointer (ie 1 on 32 bit machines 2 on 64 bit)*/
#define LONGS_PER_POINTER       (sizeof (SLONG*) / sizeof (SLONG))
/* the size of sr_bckptr in # of 32 bit longwords */
#define SIZEOF_SR_BCKPTR_IN_LONGS  LONGS_PER_POINTER

#define PREV_RUN_RECORD(record) (((SORTP *) record - scb->scb_longs))
#define NEXT_RUN_RECORD(record) (((SORTP *) record + scb->scb_longs))

/* a macro to goto the key_id part of a particular record.
   Pls. refer to the SR structure in sort.h for an explanation of
   the record structure . */
#define KEYOF(record) ((SORTP *)(((SR*)record)->sr_sort_record.sort_record_key))

/* macro to point to the next/previous  record for sorting.
   still using scb_longs as we cannot do record++ */
#define PREV_RECORD(record) ((SR*)((SORTP *) record + scb->scb_longs))
#define NEXT_RECORD(record) ((SR*)((SORTP *) record - scb->scb_longs))

/* structure containing the key and data part of the sort record,
   the back pointer is not written to the disk, this is how the records
   look in the run files. */

typedef struct sort_record
{
	ULONG sort_record_key[1];
	/* Sorting key.  Mangled by diddle_key to
	   compare using ULONG word compares (size
	   is rounded upwards if necessary).
	   Min length of 1 ULONG, max indeterminate
	   For current sort, length is stored as
	   scb_key_length.
	   Keys are created by BTR_* routines as
	   sequences of ULONG - this representation
	   cannot be easily changed. */

/*  sort_record_data  is here to explain the sort record.
    To get to the data part of a record add scb->scb_key_length to a pointer
    pointing to the start of the sort_record_key. 

    ULONG       sort_record_data [1];
                                   Data values, not part of key,
                                   that are pumped through sort.
                                   Min length of 0, max indeterminate,
                                   byte data, but starts on ULONG boundary and
                                   rounded up to ULONG size
                                   Sizeof sr_data array would be
                                   (scb_longs - scb_key_length)*sizeof(ULONG) -
                                                 sizeof(sr_bckptr)
*/

} SORT_RECORD;

#define MAX_SORT_RECORD		65535	/* bytes */

/* the record struct actually contains the keyids etc, and the back_pointer
   which points to the SORT_RECORD structure. */
typedef struct sr
{
	struct sort_record **sr_bckptr;	/* Pointer back to sort list entry */
	struct sort_record sr_sort_record;
} SR;

/* scb_longs includes the size of sr_bckptr.  */

/* The sort memory pool is laid out as follows during sorting:

struct sort_memory {
        struct sr       *records [X1];
        ULONG           empty [X2];
        struct sr       data [X1];
};

We pack items into sort_memory, inserting the first pointer into
records [0], and the first data value into data[X1-1];  Continuing
until we are out of records to sort or memory.
(eg: X1*(sizeof(struct sr*) + scb->scb_longs) + X2*sizeof(ULONG) == MAX_MEMORY
*/


/* Sort key definition block */

typedef struct skd
{
	UCHAR	skd_dtype;			/* Data type */
	UCHAR	skd_flags;			/* Flags */
	USHORT	skd_length;			/* Length if string */
	USHORT	skd_offset;			/* Offset from beginning */
	USHORT	skd_vary_offset;	/* Offset to varying/cstring length */
} SKD;

/* skd_dtype */

#define SKD_long		1
#define SKD_ulong		2
#define SKD_short		3
#define SKD_ushort		4
#define SKD_text		5
#define SKD_float		6
#define SKD_double		7
#define SKD_quad		8
#define SKD_timestamp1	9		/* Timestamp as Float */
#define SKD_bytes		10
#define SKD_d_float		11
#define SKD_varying		12		/* non-international */
#define SKD_cstring		13		/* non-international */

#define SKD_sql_time	14
#define SKD_sql_date	15
#define SKD_timestamp2	16		/* Timestamp as Quad */

#define SKD_int64	17

/* Historical alias for pre V6 code */
#define SKD_date	SKD_timestamp1

/* skd_flags */

#define SKD_ascending	0		/* default initializer */
#define SKD_descending	1
#define SKD_insensitive	2
#define SKD_binary		4

#define TYPE_RUN	0
#define TYPE_MRG	1

/* Run/merge common block header */

typedef struct rmh
{
	SSHORT		rmh_type;			/* TYPE_RUN or TYPE_MRG */
	struct mrg*	rmh_parent;
} *RMH;

/* Run control block */

typedef struct run
{
	struct rmh	run_header;
	struct run*	run_next;			/* Next (actually last) run */
	ULONG		run_records;		/* Records (remaining) in run */
#ifdef SCROLLABLE_CURSORS
	ULONG		run_max_records;	/* total number of records in run */
#endif
	USHORT		run_depth;			/* Number of "elementary" runs */
	struct sfb*	run_sfb;			/* Run sort file block */
	ULONG		run_seek;			/* Offset in file of run */
	ULONG		run_size;			/* Length of run in work file */
#ifdef SCROLLABLE_CURSORS
	ULONG		run_cached;			/* amount of cached data from run file */
#endif
	SORT_RECORD*run_record;			/* Next record in run */
	SORTP*		run_buffer;			/* Run buffer */
	SORTP*		run_end_buffer;		/* End of buffer */
	ULONG		run_buff_alloc;		/* ALlocated buffer flag */
} *RUN;

/* Merge control block */

typedef struct mrg
{
	struct rmh		mrg_header;
	SORT_RECORD*	mrg_record_a;
	struct rmh*		mrg_stream_a;
	SORT_RECORD*	mrg_record_b;
	struct rmh*		mrg_stream_b;
} *MRG;

/* Work file space control block */

typedef struct wfs
{
	struct wfs*	wfs_next;
	ULONG		wfs_position;			/* Starting position of free space */
	ULONG		wfs_size;				/* Length of free space */
} *WFS;

/* Sort work file control block */

typedef struct sfb
{
	struct sfb *sfb_next;
	int sfb_file;				/* File descriptor */
	TEXT *sfb_file_name;		/* ALLOC: File name for deletion */
	ULONG sfb_file_size;		/* Real size of the work file */
	struct wfs *sfb_file_space;	/* ALLOC: Available space in work file */
	struct wfs *sfb_free_wfs;	/* ALLOC: Free space in work file */
	DLS sfb_dls;				/* Place where file is created */
	SortMem* sfb_mem;
} *SFB;

/* Sort Context Block */

typedef struct scb
{
	struct scb *scb_next;		/* Next known sort in system */
	SORTP *scb_memory;			/* ALLOC: Memory for sort */
	SORTP *scb_end_memory;		/* End of memory */
	ULONG scb_size_memory;		/* Bytes allocated */
	SR *scb_last_record;		/* Address of last record */
	SORT_RECORD **scb_first_pointer;	/* Memory for sort */
	SORT_RECORD **scb_next_pointer;	/* Address for next pointer */
#ifdef SCROLLABLE_CURSORS
	SORTP **scb_last_pointer;	/* Address for last pointer in block */
#endif
	USHORT scb_length;			/* Record length */
	USHORT scb_longs;			/* Length of record in longwords */
	ULONG scb_keys;				/* Number of keys */
	ULONG scb_key_length;		/* Key length */
	ULONG scb_records;			/* Number of records */
	ULONG scb_max_records;		/* Maximum number of records to store */
	struct sfb *scb_sfb;		/* ALLOC: List of scratch files, if open */
	struct run *scb_runs;		/* ALLOC: Run on scratch file, if any */
	struct mrg *scb_merge;		/* Top level merge block */
	struct run *scb_free_runs;	/* ALLOC: Currently unused run blocks */
	SORTP *scb_merge_space;		/* ALLOC: memory space to do merging */
	ULONG scb_flags;			/* see flag bits below */
	ISC_STATUS *scb_status_vector;	/* Status vector for errors */
	BOOLEAN(*scb_dup_callback) ();	/* Duplicate handling callback */
	void *scb_dup_callback_arg;	/* Duplicate handling callback arg */
	struct dls *scb_dls;
	struct mrg *scb_merge_pool;	/* ALLOC: pool of mrg blocks */
	class att *scb_attachment;	/* back pointer to attachment */
	SKD scb_description[1];
} *SCB;

/* flags as set in scb_flags */

#define scb_initialized		1
#define scb_sorted		2		/* stream has been sorted */

#define SCB_LEN(n_k)	(sizeof (struct scb) + (SLONG)(n_k) * sizeof (SKD))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_SORT_H_ */
