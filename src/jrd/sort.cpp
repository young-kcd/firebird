/*
 *      PROGRAM:        JRD Sort
 *      MODULE:         sort.c
 *      DESCRIPTION:    Top level sort module
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
 * $Id: sort.cpp,v 1.43 2003-09-28 18:23:26 dimitr Exp $
 *
 * 2001-09-24  SJL - Temporary fix for large sort file bug
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <errno.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/sort.h"
#include "../jrd/sort_mem.h"
#include "gen/codes.h"
#include "../jrd/intl.h"
#include "../jrd/gdsassert.h"
#include "../jrd/rse.h"
#include "../jrd/val.h"
#include "../jrd/err_proto.h"
#include "../jrd/dls_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/all_proto.h"
#include "../jrd/sch_proto.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

/* RITTER - declare seek and off_t - :ATTENTION: for 64bit I/O we might need ib_stdio.h ! */


#ifdef SOLARIS
#include "../jrd/ib_stdio.h"
#endif

#ifdef WIN_NT
/* for SEEK_SET */
#include <stdio.h>
#include <windows.h>
#include <io.h>
#endif

#define IO_RETRY			20
#define RUN_GROUP			8
#define	MAX_MERGE_LEVEL		2

#ifdef VMS
extern double MTH$CVT_D_G(), MTH$CVT_G_D();
#endif

/* The sort buffer size should be just under a multiple of the
   hardware memory page size to account for memory allocator
   overhead. On most platorms, this saves 4KB to 8KB per sort
   buffer from being allocated but not used. */

#define SORT_BUFFER_CHUNK_SIZE  4096
#define MIN_SORT_BUFFER_SIZE    (SORT_BUFFER_CHUNK_SIZE * 4)
#define MAX_SORT_BUFFER_SIZE    (SORT_BUFFER_CHUNK_SIZE * 32)

#define MAX_TEMPFILE_SIZE       1073741824	// 1GB

#define DIFF_LONGS(a,b)         ((a) - (b))
#define SWAP_LONGS(a,b,t)       {t=a; a=b; b=t;}

/* compare p and q both SORTP pointers for l 32-bit longwords
   l != 0 if p and q are not equal for l bytes */
#define  DO_32_COMPARE(p, q, l)   do if (*p++ != *q++) break; while (--l);

#define MOVE_32(len,from,to)      memcpy(to, from, len*4)

#ifndef EINTR
#define EINTR   0
#endif

/* these values are not defined as const as they are passed to 
   the diddle_key routines  which mangles them.
   As the diddle_key routines differ on VAX (little endian) and non VAX
   (big endian) patforms, making the following const caused a core on the 
   Intel Platforms, while Solaris was working fine. */
static ULONG low_key[] = { 0, 0, 0, 0, 0, 0 };
 
static ULONG high_key[] = {
	ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX,
		ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX, ULONG_MAX};


#ifdef SCROLLABLE_CURSORS
static SORT_RECORD *get_merge(MRG, SCB, RSE_GET_MODE);
#else
static void diddle_key(UCHAR *, SCB, USHORT);
static SORT_RECORD *get_merge(MRG, SCB);
#endif

static UCHAR *sort_alloc(SCB, ULONG);
static void error_memory(SCB);
static ULONG find_file_space(SCB, ULONG, SFB *);
static void free_file_space(SCB, SFB, ULONG, ULONG);
static void init(SCB);
static bool local_fini(SCB, ATT);
static void merge_runs(SCB, USHORT);
static void quick(SLONG, SORTP **, USHORT);
static ULONG order(SCB);
static void put_run(SCB);
//static void release_merge(MRG);
static void sort(SCB);
#ifdef NOT_USED_OR_REPLACED
#ifdef DEBUG
static void validate(SCB);
#endif
#endif

#ifdef DEBUG_SORT_TRACE
static void write_trace(UCHAR *, SFB, ULONG, BLOB_PTR *, ULONG);
#include "../jrd/ib_stdio.h"

IB_FILE *trace_file = NULL;
#endif

#ifdef SMALL_FILE_NAMES
#define SCRATCH         "fb_s"
#else
#define SCRATCH         "fb_sort_"
#endif

#ifdef WIN_NT
#define	SYS_ERR		gds_arg_win32
#endif

#ifndef SYS_ERR
#define SYS_ERR		gds_arg_unix
#endif



#ifdef SCROLLABLE_CURSORS
#ifdef WORDS_BIGENDIAN
void SORT_diddle_key(UCHAR * record, SCB scb, USHORT direction)
{
/**************************************
 *
 *      S O R T _ d i d d l e _ k e y             ( n o n - V A X )
 *
 **************************************
 *
 * Functional description
 *      Perform transformation between the natural form of a record
 *      and a form that can be sorted in unsigned comparison order.
 *
 *      direction - TRUE for SORT_put() and FALSE for SORT_get()
 *
 **************************************/
	UCHAR *p, *fill_pos, fill_char;
	USHORT complement, n, l, fill, flag;
	SKD *key, *end;

	for (key = scb->scb_description, end = key + scb->scb_keys; key < end;
		 key++) {
		p = record + key->skd_offset;
		n = key->skd_length;
		complement = key->skd_flags & SKD_descending;
		switch (key->skd_dtype) {
		case SKD_ulong:
		case SKD_ushort:
		case SKD_bytes:
			break;

			/* Stash embedded control info for non-fixed data types in the sort
			   record and zap it so that it doesn't interfere with collation. */

		case SKD_varying:
			if (direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) =
						((VARY *) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + ((VARY *) p)->vary_length;
					fill = n - sizeof(USHORT) - ((VARY *) p)->vary_length;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((VARY *) p)->vary_length = 0;
			}
			break;

		case SKD_cstring:
			if (direction) {
				fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						strlen(p);
					fill_pos = p + l;
					fill = n - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				else {
					l = *((USHORT *) (record + key->skd_vary_offset));
					*(p + l) = fill_char;
				}
			}
			break;

		case SKD_text:
			break;

		case SKD_float:
		case SKD_double:
			flag = (direction || !complement) ? direction : TRUE;
			if (flag ^ (*p >> 7))
				*p ^= 1 << 7;
			else
				complement = !complement;
			break;

		case SKD_long:
		case SKD_short:
		case SKD_quad:
		case SKD_timestamp1:
		case SKD_timestamp2:
		case SKD_sql_time:
		case SKD_sql_date:
		case SKD_int64:
			*p ^= 1 << 7;
			break;

		default:
			assert(FALSE);
			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		/* Flatter but don't complement control info for non-fixed
		   data types when restoring the data. */

		if (key->skd_dtype == SKD_varying && !direction) {
			p = record + key->skd_offset;
			((VARY *) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = record + key->skd_offset;
			l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}


#else
void SORT_diddle_key(UCHAR * record, SCB scb, USHORT direction)
{
/**************************************
 *
 *      S O R T _ d i d d l e _ k e y             ( V A X )
 *
 **************************************
 *
 * Functional description
 *      Perform transformation between the natural form of a record
 *      and a form that can be sorted in unsigned comparison order.
 *
 *      direction - TRUE for SORT_put() and FALSE for SORT_get()
 *
 **************************************/
	BLOB_PTR *p;
	UCHAR c1, c2, fill_char, *fill_pos;
	USHORT complement, n, w, l, fill;
	USHORT *wp;
	SSHORT longs, flag;
	SORTP *lwp;
	ULONG lw;
	SKD *key, *end;
#ifdef  VMS
	double *dp;
#endif

	for (key = scb->scb_description, end = key + scb->scb_keys; key < end;
		 key++) {
		p = (BLOB_PTR *) record + key->skd_offset;
		wp = (USHORT *) p;
		lwp = (SORTP *) p;
		complement = key->skd_flags & SKD_descending;
		n = ROUNDUP(key->skd_length, sizeof(SLONG));
		switch (key->skd_dtype) {
		case SKD_timestamp1:
		case SKD_timestamp2:
		case SKD_sql_date:
		case SKD_sql_time:
			p[3] ^= 1 << 7;
			break;

		case SKD_ulong:
		case SKD_ushort:
			break;

		case SKD_text:
		case SKD_bytes:
		case SKD_cstring:
		case SKD_varying:

			/* Stash embedded control info for non-fixed data types in the sort
			   record and zap it so that it doesn't interfere with collation. */

			if (key->skd_dtype == SKD_varying && direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						((VARY *) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + l;
					fill = n - sizeof(USHORT) - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((VARY *) p)->vary_length = 0;
			}

			if (key->skd_dtype == SKD_cstring && direction) {
				fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						strlen(p);
					fill_pos = p + l;
					fill = n - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				else {
					l = *((USHORT *) (record + key->skd_vary_offset));
					*(p + l) = fill_char;
				}
			}

			longs = n >> SHIFTLONG;
			while (--longs >= 0) {
				c1 = p[3];
				p[3] = *p;
				*p++ = c1;
				c1 = p[1];
				p[1] = *p;
				*p = c1;
				p += 3;
			}
			p = (BLOB_PTR *) wp;
			break;

		case SKD_short:
			p[1] ^= 1 << 7;
			break;

		case SKD_long:
			p[3] ^= 1 << 7;
			break;

		case SKD_quad:
			p[7] ^= 1 << 7;
			break;

		case SKD_int64:
			/* INT64's fit in TWO LONGS, and hence the SWAP has to happen
			   here for the right order comparison using DO_32_COMPARE */
			if (!direction)
				SWAP_LONGS(lwp[0], lwp[1], lw);

			p[7] ^= 1 << 7;

			if (direction)
				SWAP_LONGS(lwp[0], lwp[1], lw);
			break;

#ifdef IEEE
		case SKD_double:
			if (!direction) {
				lw = lwp[0];
				lwp[0] = lwp[1];
				lwp[1] = lw;
			}
			flag = (direction || !complement) ? direction : TRUE;
			if (flag ^ (p[7] >> 7))
				p[7] ^= 1 << 7;
			else
				complement = !complement;
			if (direction) {
				lw = lwp[0];
				lwp[0] = lwp[1];
				lwp[1] = lw;
			}
			break;

		case SKD_float:
			flag = (direction || !complement) ? direction : TRUE;
			if (flag ^ (p[3] >> 7))
				p[3] ^= 1 << 7;
			else
				complement = !complement;
			break;

#else /* IEEE */
#ifdef VMS
		case SKD_d_float:
			dp = (double *) p;
			if (direction)
				*dp = MTH$CVT_D_G(dp);

#endif
		case SKD_double:
			w = wp[2];
			wp[2] = wp[3];
			wp[3] = w;

#ifndef VMS
		case SKD_d_float:
#endif
		case SKD_float:
			if (!direction)
				if (complement) {
					if (p[3] & 1 << 7)
						complement = !complement;
					else
						p[3] ^= 1 << 7;
				}
				else {
					if (p[3] & 1 << 7)
						p[3] ^= 1 << 7;
					else
						complement = !complement;
				}
			w = wp[0];
			wp[0] = wp[1];
			wp[1] = w;
			if (direction)
				if (p[3] & 1 << 7)
					complement = !complement;
				else
					p[3] ^= 1 << 7;
#ifdef VMS
			else if (key->skd_dtype == SKD_d_float)
				*dp = MTH$CVT_G_D(dp);
#endif
			break;
#endif /* IEEE */

		default:
			assert(FALSE);
			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		/* Flatter but don't complement control info for non-fixed
		   data types when restoring the data. */

		if (key->skd_dtype == SKD_varying && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			((VARY *) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}
#endif
#endif


void SORT_error(
				ISC_STATUS * status_vector,
				SFB sfb, TEXT * string, ISC_STATUS operation, int errcode)
{
/**************************************
 *
 *       S O R T _ e r r o r
 *
 **************************************
 *
 * Functional description
 *      Report fatal error.
 *
 **************************************/

	assert(status_vector != NULL);

	*status_vector++ = gds_arg_gds;
	*status_vector++ = isc_io_error;
	*status_vector++ = gds_arg_string;
	*status_vector++ = (ISC_STATUS) string;
	*status_vector++ = gds_arg_string;
	*status_vector++ = (ISC_STATUS) ERR_cstring(sfb->sfb_file_name);
	*status_vector++ = isc_arg_gds;
	*status_vector++ = operation;
	if (errcode) {
		*status_vector++ = SYS_ERR;
		*status_vector++ = errcode;
	}
	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds_sort_err;	/* Msg355: sort error */
	*status_vector = gds_arg_end;

	ERR_punt();
}


void SORT_fini(SCB scb, ATT att)
{
/**************************************
 *
 *      S O R T _ f i n i
 *
 **************************************
 *
 * Functional description
 *      Finish sort, and release all resources.
 *
 **************************************/

	if (scb && local_fini(scb, att))
		gds__free(scb);
}


#ifdef SCROLLABLE_CURSORS
void SORT_get(
			  ISC_STATUS * status_vector,
			  SCB scb, ULONG ** record_address, RSE_GET_MODE mode)
{
/**************************************
 *
 *      S O R T _ g e t ( I B _ V 4 _ 1 )
 *
 **************************************
 *
 * Functional description
 *      Get a record from sort (in order, of course).
 *      The address of the record is returned in <record_address>
 *      If the stream is exhausted, SORT_get ib_puts NULL in <record_address>.
 *
 **************************************/
	SORT_RECORD *record;

	scb->scb_status_vector = status_vector;

/* If there were runs, get the records from the merge
   tree--otherwise everything fit in memory */

	if (scb->scb_merge)
		record = get_merge(scb->scb_merge, scb, mode);
	else
		switch (mode) {
		case RSE_get_forward:
			if (scb->scb_flags & scb_initialized)
				scb->scb_flags &= ~scb_initialized;

			while (TRUE) {
				if (scb->scb_next_pointer > scb->scb_last_pointer) {
					record = NULL;
					break;
				}
				if (record = *scb->scb_next_pointer++)
					break;
			}
			break;

		case RSE_get_backward:
			if (scb->scb_flags & scb_initialized) {
				scb->scb_flags &= ~scb_initialized;
				scb->scb_next_pointer = scb->scb_last_pointer + 1;
			}
			else
				/* by definition, the next pointer is on the next record, 
				   so we have to go back one to get to the last fetched record--
				   this is easier than changing the sense of the next pointer */

			{
				scb->scb_next_pointer--;
				if (scb->scb_next_pointer <= scb->scb_first_pointer + 1) {
					record = NULL;
					scb->scb_next_pointer++;
					break;
				}
			}

			while (TRUE) {
				scb->scb_next_pointer--;
				if (scb->scb_next_pointer <= scb->scb_first_pointer) {
					record = NULL;
					scb->scb_next_pointer++;
					break;
				}
				if (record = *scb->scb_next_pointer)
					break;
			}

			/* reset next pointer to one greater than the last fetched */

			scb->scb_next_pointer++;
			break;

#ifdef PC_ENGINE
		case RSE_get_current:
			if (scb->scb_next_pointer <= scb->scb_first_pointer ||
				scb->scb_next_pointer > scb->scb_last_pointer)
				record = NULL;
			record = *scb->scb_next_pointer;
			break;
#endif

		default:
			assert(FALSE);
			break;
		}

	if (record)
		SORT_diddle_key((UCHAR *) record->sort_record_key, scb, FALSE);

	*record_address = (ULONG *) record;
}
#else
void SORT_get(ISC_STATUS * status_vector, SCB scb, ULONG ** record_address)
{
/**************************************
 *
 *      S O R T _ g e t
 *
 **************************************
 *
 * Functional description
 *      Get a record from sort (in order, of course).
 *      The address of the record is returned in <record_address>
 *      If the stream is exhausted, SORT_get ib_puts NULL in <record_address>.
 *
 **************************************/
	SORT_RECORD *record;

	scb->scb_status_vector = status_vector;

/* If there weren't any runs, everything fit in memory.  Just return stuff. */

	if (!scb->scb_merge)
		while (TRUE) {
			if (scb->scb_records == 0) {
				record = NULL;
				break;
			}
			scb->scb_records--;
			if ( (record = *scb->scb_next_pointer++) )
				break;
		}
	else
		record = get_merge(scb->scb_merge, scb);

	*record_address = (ULONG *) record;

	if (record) {
		diddle_key((UCHAR *) record->sort_record_key, scb, FALSE);
	}
}
#endif


SCB SORT_init(ISC_STATUS * status_vector,
			  USHORT record_length,
			  USHORT keys,
			  SKD * key_description,
			  BOOLEAN(*call_back) (),
			  void *user_arg,
			  ATT att,
			  UINT64 max_records)
{
/**************************************
 *
 *      S O R T _ i n i t
 *
 **************************************
 *
 * Functional description
 *      Initialize for a sort.  All we really need is a description
 *      of the sort keys.  Return the address  of a sort context block.
 *      If duplicate control is required, the user may specify a call
 *      back routine.  If supplied, the call back routine is called
 *      with three argument: the two records and the user supplied
 *      argument.  If the call back routine returns TRUE, the second
 *      duplicate record is eliminated.
 *
 **************************************/
	SCB scb;
	SKD *p, *q;

/* Allocate and setup a sort context block, including copying the
   key description vector.  Round the record length up to the next
   longword, and add a longword to a pointer back to the pointer
   slot.  */

	try {
		scb = (SCB) gds__alloc((SLONG) SCB_LEN(keys));
	} catch(const std::exception&) {
/* FREE: scb is freed by SORT_fini(), called by higher level cleanup */
/* FREE: or later in this module in error cases */
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_sort_mem_err;
		*status_vector = gds_arg_end;
		return NULL;
	}
	memset((UCHAR *) scb, 0, SCB_LEN(keys));

	scb->scb_status_vector = status_vector;
	scb->scb_length = record_length;
	scb->scb_longs =
		ROUNDUP(record_length + sizeof(SLONG *),
				sizeof(SLONG *)) >> SHIFTLONG;
	scb->scb_dup_callback = call_back;
	scb->scb_dup_callback_arg = user_arg;
	scb->scb_keys = keys;

	scb->scb_max_records = max_records;

	p = scb->scb_description;
	q = key_description;
	do
		*p++ = *q++;
	while (--keys);

	--p;
	scb->scb_key_length =
		ROUNDUP(p->skd_offset + p->skd_length, sizeof(SLONG)) >> SHIFTLONG;

/* Next, try to allocate a "big block".  How big?  Big enough! */
	try {
		#ifdef DEBUG_MERGE
		/* To debug the merge algorithm, force the in-memory pool to be VERY small */
		scb->scb_size_memory = 2000;
		scb->scb_memory =
			(SORTP *) gds__alloc((SLONG) scb->scb_size_memory);
		/* FREE: scb_memory is freed by local_fini() */
		#else
		/* Try to get a big chunk of memory, if we can't try smaller and
		smaller chunks until we can get the memory.  If we get down to
		too small a chunk - punt and report not enough memory. */

		for (scb->scb_size_memory = MAX_SORT_BUFFER_SIZE;;
			scb->scb_size_memory -= SORT_BUFFER_CHUNK_SIZE)
			if (scb->scb_size_memory < MIN_SORT_BUFFER_SIZE)
				break;
			else if ( (scb->scb_memory =
				 (SORTP *) gds__alloc((SLONG) scb->scb_size_memory)) )
			/* FREE: scb_memory is freed by local_fini() */
				break;
		#endif /* DEBUG_MERGE */
	} catch(const std::exception&) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_sort_mem_err;
		/* Msg356: sort error: not enough memory */
		*status_vector = gds_arg_end;
		gds__free(scb);
		return NULL;
	}

	scb->scb_end_memory =
		(SORTP *) ((BLOB_PTR *) scb->scb_memory + scb->scb_size_memory);
	scb->scb_first_pointer = (SORT_RECORD **) scb->scb_memory;

/* Set up to receive the first record. */

	init(scb);

/* If a linked list pointer was given, link in new sort block */

	if (att) {
		scb->scb_next = att->att_active_sorts;
		att->att_active_sorts = scb;
		scb->scb_attachment = att;
	}

	return scb;
}


int SORT_put(ISC_STATUS * status_vector, SCB scb, ULONG ** record_address)
{
/**************************************
 *
 *      S O R T _ p u t
 *
 **************************************
 *
 * Functional description
 *      Allocate space for a record for sort.  The caller is responsible
 *      for moving in the record.
 *
 *      Records are added from the top (higher addresses) of sort memory going down.  Record
 *      pointers are added at the bottom (lower addresses) of sort memory going up.  When
 *      they overlap, the records in memory are sorted and written to a "run"
 *      in the scratch files.  The runs are eventually merged.
 *
 **************************************/
	RUN run;
	USHORT count, depth;
	SR *record;

	scb->scb_status_vector = status_vector;

/* Find the last record passed in, and zap the keys something comparable
   by unsigned longword compares. */

	record = scb->scb_last_record;

	if (record != (SR *) scb->scb_end_memory)
#ifdef SCROLLABLE_CURSORS
		SORT_diddle_key((UCHAR *) (record->sr_sort_record.sort_record_key),
						scb, TRUE);
#else
		diddle_key((UCHAR *) (record->sr_sort_record.sort_record_key), scb,
				   TRUE);
#endif

/* If there isn't room for the record, sort and write the run.
   Check that we are not at the beginning of the buffer in addition
   to checking for space for the record.  This avoids the pointer
   record from underflowing in the second condition. */

	if ((BLOB_PTR *) record < (BLOB_PTR *) (scb->scb_memory + scb->scb_longs)
		|| (BLOB_PTR *) NEXT_RECORD(record) <=
		(BLOB_PTR *) (scb->scb_next_pointer + 1)) {
		put_run(scb);
		for (;;) {
			run = scb->scb_runs;
			if ((depth = run->run_depth) == MAX_MERGE_LEVEL)
				break;
			count = 1;
			while ((run = run->run_next) && run->run_depth == depth)
				count++;
			if (count < RUN_GROUP)
				break;
			merge_runs(scb, count);
		}
		init(scb);
		record = scb->scb_last_record;
	}

	record = NEXT_RECORD(record);

/* Make sure the first longword of the record points to the pointer */
	scb->scb_last_record = record;
	record->sr_bckptr = scb->scb_next_pointer;

/* Move key_id into *scb->scb_next_pointer and then
   increment scb->scb_next_pointer */
	*scb->scb_next_pointer++ =
		reinterpret_cast <
		sort_record * >(record->sr_sort_record.sort_record_key);
#ifndef SCROLLABLE_CURSORS
	scb->scb_records++;
#endif
	*record_address = (ULONG *) record->sr_sort_record.sort_record_key;

	return FB_SUCCESS;
}


#ifdef SCROLLABLE_CURSORS
void SORT_read_block(
#else
ULONG SORT_read_block(
#endif
						 ISC_STATUS * status_vector,
						 SFB sfb,
						 ULONG seek, BLOB_PTR * address, ULONG length)
{
/**************************************
 *
 *      S O R T _ r e a d _ b l o c k
 *
 **************************************
 *
 * Functional description
 *      Read a block of stuff from a scratch file.
 *
 **************************************/
	ULONG len, read_len, i;

#ifdef DEBUG_SORT_TRACE
	UCHAR *org_address;
	ULONG org_length, org_seek;

	org_address = address;
	org_length = length;
	org_seek = seek;
#endif

/* Checkout of engine on sort I/O */

	THREAD_EXIT;

/* The following is a crock induced by a VMS C bug */

	while (length) {
		len = length;
		for (i = 0; i < IO_RETRY; i++) {
			if (lseek(sfb->sfb_file, LSEEK_OFFSET_CAST seek, SEEK_SET) == -1) {
				THREAD_ENTER;
				SORT_error(status_vector, sfb, "lseek", isc_io_read_err,
						   errno);
			}
			if ((read_len = read(sfb->sfb_file, address, len)) == len)
				break;
			else if ((SSHORT) read_len == -1 && !SYSCALL_INTERRUPTED(errno)) {
				THREAD_ENTER;
				SORT_error(status_vector, sfb, "read", isc_io_read_err,
						   errno);
			}
		}

		if (i == IO_RETRY) {
			THREAD_ENTER;
			SORT_error(status_vector, sfb, "read", isc_io_read_err, errno);
		}
		length -= read_len;
		address += read_len;
		seek += read_len;
	}

	THREAD_ENTER;

#ifdef DEBUG_SORT_TRACE
	write_trace("Read", sfb, org_seek, org_address, org_length);
#endif
#ifndef SCROLLABLE_CURSORS
	return seek;
#endif
}


void SORT_shutdown(ATT att)
{
/**************************************
 *
 *      S O R T _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *      Clean up any pending sorts.
 *
 **************************************/

/* We ignore the result from local_fini,
   since the expectation is that from the
   way we are passing in the structures
   that every SCB *IS* part of the ptr
   chain.  Also, we're not freeing the
   structure here, so if something goes
   wrong, it's not *CRITICAL*.  --  mrs  */

	while (att->att_active_sorts)
		local_fini(att->att_active_sorts, att);
}


int SORT_sort(ISC_STATUS * status_vector, SCB scb)
{
/**************************************
 *
 *      S O R T _ s o r t
 *
 **************************************
 *
 * Functional description
 *      Perform any intermediate computing before giving records
 *      back.  If there weren't any runs, run sort the buffer.
 *      If there were runs, sort and write out the last run and
 *      build a merge tree.
 *
 **************************************/
	ULONG count, run_count, size, temp;
	RUN run;
	RMH *m1, *m2, *streams, streams_local[200];
	MRG merge;
	MRG merge_pool;
	SORTP *buffer;

	scb->scb_status_vector = status_vector;

	if (scb->scb_last_record != (SR *) scb->scb_end_memory)
#ifdef SCROLLABLE_CURSORS
		SORT_diddle_key((UCHAR *) KEYOF(scb->scb_last_record), scb, TRUE);
#else
		diddle_key((UCHAR *) KEYOF(scb->scb_last_record), scb, TRUE);
#endif

/* If there aren't any runs, things fit nicely in memory.  Just sort the mess
   and we're ready for output */

	if (!scb->scb_runs) {
		sort(scb);
#ifdef SCROLLABLE_CURSORS
		scb->scb_last_pointer = scb->scb_next_pointer - 1;
#endif
		scb->scb_next_pointer = scb->scb_first_pointer + 1;
#ifdef SCROLLABLE_CURSORS
		scb->scb_flags |= scb_initialized;
#endif
		scb->scb_flags |= scb_sorted;
		return FB_SUCCESS;
	}

/* Write the last records as a run */

	put_run(scb);

/* Build a merge tree for the run blocks.  Start by laying them all out
   in a vector; this is done to allow us to build a merge tree from the
   bottom up, ensuring that a balanced tree is built. */

	for (run_count = 0, run = scb->scb_runs; run; run = run->run_next) {
		if (run->run_buff_alloc) {
			gds__free(run->run_buffer);
			run->run_buff_alloc = 0;
		}
		++run_count;
	}

	try {
		if ((run_count * sizeof(RMH)) > sizeof(streams_local))
			streams =
				(RMH *) gds__alloc((SLONG) run_count * sizeof(RMH));
	/* FREE: streams is freed later in this routine */
		else
			streams = streams_local;
	} catch(const std::exception&) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_sort_mem_err;
		*status_vector = gds_arg_end;
		return gds_sort_mem_err;
	}

	m1 = streams;
	for (run = scb->scb_runs; run; run = run->run_next)
		*m1++ = (RMH) run;
	count = run_count;

/* We're building a b-tree of the sort merge blocks, we have (count)
 * leaves already, so we *know* we need (count-1) merge blocks.
 */
	if (count > 1) {
		assert(!scb->scb_merge_pool);	/* shouldn't have a pool */
		try {
			scb->scb_merge_pool =
				(MRG) gds__alloc((SLONG) (count - 1)*sizeof(struct mrg));
		/* FREE: smb_merge_pool freed in local_fini() when the scb is released */

			merge_pool = scb->scb_merge_pool;
		} catch(const std::exception&) {
			gds__free(streams);
			*status_vector++ = gds_arg_gds;
			*status_vector++ = gds_sort_mem_err;
			*status_vector = gds_arg_end;
			return gds_sort_mem_err;
		}
		memset(merge_pool, 0, (count - 1) * sizeof(struct mrg));
	}
	else {
		/* "Merge" of 1 or 0 runs doesn't make sense. */
		assert(FALSE);			/* We really shouldn't get here */
		merge = (MRG) * streams;	/* But if we do... */
	}

/* each pass through the vector builds a level of the merge tree
   by condensing two runs into one
   We will continue to make passes until there is a single item */

/* See also kissing cousin of this loop in merge_runs() */

	while (count > 1) {
		m1 = m2 = streams;

		/* "m1" is used to sequence through the runs being merged, 
		   while "m2" points at the new merged run */

		while (count >= 2) {
			merge = merge_pool++;
			merge->mrg_header.rmh_type = TYPE_MRG;

			assert(((*m1)->rmh_type == TYPE_MRG) ||	/* garbage watch */
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_a = *m1++;

			assert(((*m1)->rmh_type == TYPE_MRG) ||	/* garbage watch */
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_b = *m1++;

			merge->mrg_record_a = NULL;
			merge->mrg_record_b = NULL;

			*m2++ = (RMH) merge;
			count -= 2;
		}

		if (count)
			*m2++ = *m1++;
		count = m2 - streams;
	}

	if (streams != streams_local)
		gds__free(streams);
	buffer = (SORTP *) scb->scb_first_pointer;
	merge->mrg_header.rmh_parent = NULL;
	scb->scb_merge = merge;
	scb->scb_longs -= SIZEOF_SR_BCKPTR_IN_LONGS;

/* Divvy up the sort space among buffers for runs.  Although something slightly
   better could be arranged, for now give them all the same size hunk. */

	temp = DIFF_LONGS(scb->scb_end_memory, buffer);
	count = temp / (scb->scb_longs * run_count);
	if (count) {
		size = count * (SSHORT) scb->scb_longs;
		count = run_count;
	}
	else {
		size = (SSHORT) scb->scb_longs;
		count = temp / scb->scb_longs;
	}

/* allocate buffer space for either all the runs, if they fit, or for
   as many as allow */

	for (run = scb->scb_runs; run && count; count--, run = run->run_next) {
		run->run_buffer = buffer;
		buffer += size;
		run->run_record =
			reinterpret_cast < sort_record * >(run->run_end_buffer = buffer);
	}

/* if there was not enough buffer space, get some more for the remaining runs
   allocating enough for the merge space plus a link */

	for (; run; run = run->run_next) {
		try {
			run->run_buffer =
				(ULONG *) gds__alloc((SLONG) (size * sizeof(ULONG)));
			/* FREE: smb_merge_space freed in local_fini() when the scb is released */
		} catch(const std::exception&) {
			*status_vector++ = gds_arg_gds;
			*status_vector++ = gds_sort_mem_err;
			*status_vector = gds_arg_end;
			return gds_sort_mem_err;
		}
		/* Link the new buffer into the chain of buffers */
		run->run_buff_alloc = 1;
		run->run_record =
			reinterpret_cast < sort_record * >(run->run_end_buffer =
											   run->run_buffer + size);
	}

	scb->scb_flags |= scb_sorted;
	return FB_SUCCESS;
}


ULONG SORT_write_block(ISC_STATUS * status_vector,
					   SFB sfb, ULONG seek, BLOB_PTR * address, ULONG length)
{
/**************************************
 *
 *      S O R T _ w r i t e _ b l o c k
 *
 **************************************
 *
 * Functional description
 *      Write a block of stuff to the scratch file.
 *
 **************************************/
	ULONG len, write_len, i;

#ifdef DEBUG_SORT_TRACE
	write_trace("Write", sfb, seek, address, length);
#endif

/* Check out of engine on sort I/O */

	THREAD_EXIT;

/* The following is a crock induced by a VMS C bug */

	while (length) {
		len = length;
		for (i = 0; i < IO_RETRY; i++) {
			if (lseek(sfb->sfb_file, LSEEK_OFFSET_CAST seek, SEEK_SET) == -1) {
				THREAD_ENTER;
				SORT_error(status_vector, sfb, "lseek", isc_io_write_err,
						   errno);
			}
			if ((write_len = write(sfb->sfb_file, address, len)) == len)
				break;
			else {
				if (write_len >= 0)
					/* If write returns value that is not equal len, then 
					   most likely there is not enough space, try to write
					   one more time to get meaningful errno  */
					write_len = write(sfb->sfb_file, address + write_len,
									  len - write_len);
				if ((SSHORT) write_len == -1 && !SYSCALL_INTERRUPTED(errno)) {
					THREAD_ENTER;
					SORT_error(status_vector, sfb, "write", isc_io_write_err,
							   errno);
				}
			}
		}

		if (i == IO_RETRY) {
			THREAD_ENTER;
			SORT_error(status_vector, sfb, "write", isc_io_write_err, errno);
		}
		length -= write_len;
		address += write_len;
		seek += write_len;
	}

	THREAD_ENTER;

	return seek;
}


static UCHAR *sort_alloc(SCB scb, ULONG size)
{
/**************************************
 *
 *      a l l o c
 *
 **************************************
 *
 * Functional description
 *      Allocate and zero a block of memory.
 *
 *      Notes about memory management in this module.
 *        - Apparently this historical reason (from Deej) that this
 *          module uses ALL_malloc directly, instead of the JRD allocator
 *          is so a large sort will not push up the high-water mark of
 *          memory allocated to a request or attachment (recall this memory
 *          isn't released until the request/attachment finished)
 *        - As a result, the memory blocks allocated here don't have 
 *          the blk_header structure (we'ld have to add it if we ever 
 *          change this)
 *        - Most things allocated have pointers placed in the scb.
 *          (sort control block)
 *        - There is an error handler set up by our caller, which will
 *          call back to SORT_fini(), which frees all the memory
 *          chains that hang off the scb.
 *        - There are some short-term allocations done (for instance,
 *          when sorting a run before writing it to disk).  There appears
 *          to be no need to have an error handler to free them as
 *          no errors can be posted during the process.
 *
 *      1994-October-11 David Schnepper 
 *
 **************************************/
	UCHAR* block = 0;

	try {
		block =
			reinterpret_cast<UCHAR*>(gds__alloc(size));
/* FREE: caller responsible for freeing */
	} catch(const std::exception&) {
		if (!block)
		{
			error_memory(scb);
			return NULL;
		}
	}

	memset(block, 0, size);
	return block;
}

#ifndef SCROLLABLE_CURSORS
#ifdef WORDS_BIGENDIAN
static void diddle_key(UCHAR * record, SCB scb, USHORT direction)
{
/**************************************
 *
 *      d i d d l e _ k e y             ( n o n - V A X )
 *
 **************************************
 *
 * Functional description
 *      Perform transformation between the natural form of a record
 *      and a form that can be sorted in unsigned comparison order.
 *
 *      direction - TRUE for SORT_put() and FALSE for SORT_get()
 *
 **************************************/
	UCHAR *p, *fill_pos, fill_char;
	USHORT complement, n, l, fill, flag;
	SKD *key, *end;

	for (key = scb->scb_description, end = key + scb->scb_keys; key < end;
		 key++) {
		p = record + key->skd_offset;
		n = key->skd_length;
		complement = key->skd_flags & SKD_descending;
		switch (key->skd_dtype) {
		case SKD_ulong:
		case SKD_ushort:
		case SKD_bytes:
		case SKD_sql_time:
			break;

			/* Stash embedded control info for non-fixed data types in the sort
			   record and zap it so that it doesn't interfere with collation. */

		case SKD_varying:
			if (direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) =
						((VARY *) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + ((VARY *) p)->vary_length;
					fill = n - sizeof(USHORT) - ((VARY *) p)->vary_length;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((VARY *) p)->vary_length = 0;
			}
			break;

		case SKD_cstring:
			if (direction) {
				fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						strlen((char*)p);
					fill_pos = p + l;
					fill = n - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				else {
					l = *((USHORT *) (record + key->skd_vary_offset));
					*(p + l) = fill_char;
				}
			}
			break;

		case SKD_text:
			break;

#ifndef VMS
		case SKD_d_float:
#else
			Deliberate_compile_error++;
			Fix for any VMS port.
#endif
			case SKD_float:case SKD_double:flag = (direction || !complement)
				? direction : TRUE;
			if (flag ^ (*p >> 7))
				*p ^= 1 << 7;
			else
				complement = !complement;
			break;

		case SKD_long:
		case SKD_short:
		case SKD_quad:
		case SKD_timestamp1:
		case SKD_timestamp2:
		case SKD_sql_date:
		case SKD_int64:
			*p ^= 1 << 7;
			break;

		default:
			assert(FALSE);
			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		/* Flatter but don't complement control info for non-fixed
		   data types when restoring the data. */

		if (key->skd_dtype == SKD_varying && !direction) {
			p = record + key->skd_offset;
			((VARY *) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = record + key->skd_offset;
			l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}


#else
static void diddle_key(UCHAR * record, SCB scb, USHORT direction)
{
/**************************************
 *
 *      d i d d l e _ k e y             ( V A X )
 *
 **************************************
 *
 * Functional description
 *      Perform transformation between the natural form of a record
 *      and a form that can be sorted in unsigned comparison order.
 *
 *      direction - TRUE for SORT_put() and FALSE for SORT_get()
 *
 **************************************/
	BLOB_PTR *p;
	UCHAR c1, fill_char, *fill_pos;
	USHORT complement, n, l, fill;
	USHORT *wp;
	SSHORT longs, flag;
	SORTP *lwp;
	ULONG lw;
	SKD *key, *end;
#ifdef  VMS
	double *dp;
#endif
#ifndef IEEE
	USHORT w;
#endif

	for (key = scb->scb_description, end = key + scb->scb_keys; key < end;
		 key++) {
		p = (BLOB_PTR *) record + key->skd_offset;
		wp = (USHORT *) p;
		lwp = (SORTP *) p;
		complement = key->skd_flags & SKD_descending;
		n = ROUNDUP(key->skd_length, sizeof(SLONG));
		switch (key->skd_dtype) {
		case SKD_timestamp1:
		case SKD_timestamp2:
		case SKD_sql_time:
		case SKD_sql_date:
			p[3] ^= 1 << 7;
			break;

		case SKD_ulong:
		case SKD_ushort:
			break;

		case SKD_text:
		case SKD_bytes:
		case SKD_cstring:
		case SKD_varying:

			/* Stash embedded control info for non-fixed data types in the sort
			   record and zap it so that it doesn't interfere with collation. */

			if (key->skd_dtype == SKD_varying && direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						((VARY *) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + l;
					fill = n - sizeof(USHORT) - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((VARY *) p)->vary_length = 0;
			}

			if (key->skd_dtype == SKD_cstring && direction) {
				fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						strlen(reinterpret_cast < const char *>(p));
					fill_pos = p + l;
					fill = n - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				else {
					l = *((USHORT *) (record + key->skd_vary_offset));
					*(p + l) = fill_char;
				}
			}

			longs = n >> SHIFTLONG;
			while (--longs >= 0) {
				c1 = p[3];
				p[3] = *p;
				*p++ = c1;
				c1 = p[1];
				p[1] = *p;
				*p = c1;
				p += 3;
			}
			p = (BLOB_PTR *) wp;
			break;

		case SKD_short:
			p[1] ^= 1 << 7;
			break;

		case SKD_long:
			p[3] ^= 1 << 7;
			break;

		case SKD_quad:
			p[7] ^= 1 << 7;
			break;

		case SKD_int64:
			/* INT64's fit in TWO LONGS, and hence the SWAP has to happen
			   here for the right order comparison using DO_32_COMPARE */
			if (!direction)
				SWAP_LONGS(lwp[0], lwp[1], lw);

			p[7] ^= 1 << 7;

			if (direction)
				SWAP_LONGS(lwp[0], lwp[1], lw);
			break;

#ifdef IEEE
		case SKD_double:
			if (!direction) {
				lw = lwp[0];
				lwp[0] = lwp[1];
				lwp[1] = lw;
			}
			flag = (direction || !complement) ? direction : TRUE;
			if (flag ^ (p[7] >> 7))
				p[7] ^= 1 << 7;
			else
				complement = !complement;
			if (direction) {
				lw = lwp[0];
				lwp[0] = lwp[1];
				lwp[1] = lw;
			}
			break;

		case SKD_float:
			flag = (direction || !complement) ? direction : TRUE;
			if (flag ^ (p[3] >> 7))
				p[3] ^= 1 << 7;
			else
				complement = !complement;
			break;

#else /* IEEE */
#ifdef VMS
		case SKD_d_float:
			dp = (double *) p;
			if (direction)
				*dp = MTH$CVT_D_G(dp);

#endif
		case SKD_double:
			w = wp[2];
			wp[2] = wp[3];
			wp[3] = w;

#ifndef VMS
		case SKD_d_float:
#endif
		case SKD_float:
			if (!direction)
				if (complement) {
					if (p[3] & 1 << 7)
						complement = !complement;
					else
						p[3] ^= 1 << 7;
				}
				else {
					if (p[3] & 1 << 7)
						p[3] ^= 1 << 7;
					else
						complement = !complement;
				}
			w = wp[0];
			wp[0] = wp[1];
			wp[1] = w;
			if (direction)
				if (p[3] & 1 << 7)
					complement = !complement;
				else
					p[3] ^= 1 << 7;
#ifdef VMS
			else if (key->skd_dtype == SKD_d_float)
				*dp = MTH$CVT_G_D(dp);
#endif
			break;
#endif /* IEEE */

		default:

/*	Don't want the debug version to
        stop because uf skd_type = 0
FSG 22.Dez.2000

        assert (FALSE);
*/

			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		/* Flatter but don't complement control info for non-fixed
		   data types when restoring the data. */

		if (key->skd_dtype == SKD_varying && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			((VARY *) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}
#endif
#endif


static void error_memory(SCB scb)
{
/**************************************
 *
 *       e r r o r _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *      Report fatal out of memory error.
 *
 **************************************/
	ISC_STATUS *status_vector;

	status_vector = scb->scb_status_vector;

	assert(status_vector != NULL);

	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds_sort_mem_err;
	*status_vector = gds_arg_end;
	ERR_punt();
}


static ULONG find_file_space(SCB scb, ULONG size, SFB * ret_sfb)
{
/**************************************
 *
 *      f i n d _ f i l e _ s p a c e
 *
 **************************************
 *
 * Functional description
 *      Find space of input size in one of the 
 *      open sort files.  If a free block is not 
 *      available, allocate space at the end.
 *
 **************************************/
	WFS space, *ptr, *best;
	SFB sfb, *sfb_ptr, best_sfb, last_sfb;
	TEXT file_name[128];

/* Find the best available space.  This is defined as the smallest free space
   that is big enough.  This preserves large blocks. */

	best = NULL;
	last_sfb = NULL;
	file_name[0] = '\0';

/* Search through the available space in the work file list. */

	for (sfb_ptr = &scb->scb_sfb; (sfb = *sfb_ptr); sfb_ptr = &sfb->sfb_next) {
		for (ptr = &sfb->sfb_file_space; (space = *ptr);
			 ptr = &(*ptr)->wfs_next) {
			/* if this is smaller than our previous best, use it */

			if (space->wfs_size >= size && 
                (!best || (space->wfs_size < (*best)->wfs_size))) {
				best = ptr;
				best_sfb = sfb;
			}
		}

		/* Save the previous sfb pointer because when we get out of this
		   for loop, sfb would be a NULL pointer. */

		last_sfb = sfb;
	}
	sfb = last_sfb;

/* If we didn't find any space, allocate it at the end of the file. */

	if (!best) {
		/* if there is no file allocated yet or the size requested is bigger
		   than available space in the current directory, create a new file
		   and return. */

		if (!sfb || !DLS_get_temp_space(size, sfb) ||
			(sfb->sfb_file_size + size >= MAX_TEMPFILE_SIZE)) {

			sfb = (SFB) sort_alloc(scb, (ULONG) sizeof(struct sfb));
			/* FREE: scb_sfb chain is freed in local_fini() */

			// Is the last DLS at it's size limit? If so, add a new DLS dir   M.E.G
			if (last_sfb && (last_sfb->sfb_dls->dls_inuse + size >= MAX_TEMPFILE_SIZE))
				if (!DLS_add_dir(MAX_TEMPFILE_SIZE, last_sfb->sfb_dls->dls_directory))
					error_memory(scb);

			if (last_sfb)
				last_sfb->sfb_next = sfb;
			else
				scb->scb_sfb = sfb;

			/* Find a free space */

			sfb->sfb_dls = NULL;
			if (!DLS_get_temp_space(size, sfb))
				/* There is not enough space */
				error_memory(scb);

			/* Create a scratch file */

			sfb->sfb_file =
				(int) gds__temp_file(FALSE, SCRATCH, file_name,
									 sfb->sfb_dls->dls_directory, TRUE);

			/* allocate the file name even if the file is not open,
			   because the error routine depends on it
			   This is released during local_fini(). */

			sfb->sfb_file_name =
				(TEXT *) sort_alloc(scb, (ULONG) (strlen(file_name) + 1));
			/* FREE: sfb_file_name is freed in local_fini() */

			strcpy(sfb->sfb_file_name, file_name);

			if (sfb->sfb_file == -1)
				SORT_error(scb->scb_status_vector, sfb, "open",
						   isc_io_open_err, errno);

			sfb->sfb_mem = FB_NEW (*getDefaultMemoryPool()) SortMem(sfb, size);
		}

		*ret_sfb = sfb;
		sfb->sfb_file_size += size;
		return sfb->sfb_file_size - size;
	}

/* set up the return parameters */

	*ret_sfb = best_sfb;
	space = *best;

/* If the hunk was an exact fit, remove the work file space block from the
   list and splice it into the free list */

	if (space->wfs_size == size) {
		*best = space->wfs_next;
		space->wfs_next = best_sfb->sfb_free_wfs;
		best_sfb->sfb_free_wfs = space;
		return space->wfs_position;
	}

/* The best block is too big -- chop the needed space off the end. */

	space->wfs_size -= size;

	return space->wfs_position + space->wfs_size;
}


static void free_file_space(SCB scb, SFB sfb, ULONG position, ULONG size)
{
/**************************************
 *
 *      f r e e _ f i l e _ s p a c e
 *
 **************************************
 *
 * Functional description
 *      Release a segment of work file.
 *
 **************************************/
	WFS space, *ptr, next;
	ULONG end;

	assert(size > 0);
	assert(position < sfb->sfb_file_size);	/* Block starts in file */
	end = position + size;
	assert(end <= sfb->sfb_file_size);	/* Block ends in file */

/* Search through work file space blocks looking for an adjacent block. */

	for (ptr = &sfb->sfb_file_space; (space = *ptr); ptr = &space->wfs_next) {
		if (end >= space->wfs_position)
			break;
	}

	if (space) {
		/* may have found an adjacent block - try to join them together */

		if (end == space->wfs_position) {
			/* newly freed block starts just before previously freed */
			space->wfs_position -= size;
			space->wfs_size += size;
			return;
		}

		if (position == space->wfs_position + space->wfs_size) {
			/* newly freed block starts just after previously freed */
			space->wfs_size += size;

			if ((next = space->wfs_next) && end == next->wfs_position) {
				/* The NEXT freed block is adjacent, join it too */
				space->wfs_size += next->wfs_size;
				space->wfs_next = next->wfs_next;
				next->wfs_next = sfb->sfb_free_wfs;
				sfb->sfb_free_wfs = next;
			}
			return;
		}

		/* Blocks weren't adjacent - just nearby */

		/* Check that block to free doesn't overlap existing free block */
		assert(position >= space->wfs_position + space->wfs_size);
	}

/* Block didn't seem to append nicely to an existing block */

	if ( (space = sfb->sfb_free_wfs) )
		sfb->sfb_free_wfs = space->wfs_next;
	else
		space = (WFS) sort_alloc(scb, (ULONG) sizeof(struct wfs));
	/* FREE: wfs_next chain is freed in local_fini() */

	space->wfs_next = *ptr;
	*ptr = space;
	space->wfs_size = size;
	space->wfs_position = position;
}


static SORT_RECORD *get_merge(MRG merge, SCB scb
#ifdef SCROLLABLE_CURSORS
							  , RSE_GET_MODE mode
#endif
	)
{
/**************************************
 *
 *      g e t _ m e r g e
 *
 **************************************
 *
 * Functional description
 *      Get next record from a merge tree and/or run.
 *
 **************************************/
	SORT_RECORD *record;	/* no more than 1 SORTP* to a line */
	SORTP *p;			/* no more than 1 SORTP* to a line */
	SORTP *q;			/* no more than 1 SORTP* to a line */
	ULONG l;
#ifdef SCROLLABLE_CURSORS
	ULONG space_available, data_remaining;
#else
	ULONG n;
#endif
	USHORT eof;
	RUN run;

	record = NULL;
	eof = FALSE;

	while (merge) {
		/* If node is a run, get the next record (or not) and back to parent. */

		if (merge->mrg_header.rmh_type == TYPE_RUN) {
			run = (RUN) merge;
			merge = run->run_header.rmh_parent;

			/* check for end-of-file condition in either direction */

#ifdef SCROLLABLE_CURSORS
			if (
				(mode == RSE_get_backward
				 && run->run_records >= run->run_max_records - 1)
				|| (mode == RSE_get_forward && run->run_records == 0))
#else
			if (run->run_records == 0)
#endif
			{
				record = (SORT_RECORD *) - 1;
				eof = TRUE;
				continue;
			}

			eof = FALSE;

			/* Find the appropriate record in the buffer to return. */

#ifdef SCROLLABLE_CURSORS
			if (mode == RSE_get_forward) {
				run->run_record = NEXT_RUN_RECORD(run->run_record);
#endif
				if ((record = (SORT_RECORD *) run->run_record) <
					(SORT_RECORD *) run->run_end_buffer) {
#ifndef SCROLLABLE_CURSORS
					run->run_record =
						reinterpret_cast <
						sort_record * >(NEXT_RUN_RECORD(run->run_record));
#endif
					--run->run_records;
					continue;
				}
#ifndef SCROLLABLE_CURSORS
				/* There are records remaining, but the buffer is full.  Read a buffer
				   full. */

				l =
					(ULONG) ((BLOB_PTR *) run->run_end_buffer -
							 (BLOB_PTR *) run->run_buffer);
				n = run->run_records * scb->scb_longs * sizeof(ULONG);
				l = MIN(l, n);
				run->run_seek =
					run->run_sfb->sfb_mem->read(scb->scb_status_vector,
												run->run_seek,
												reinterpret_cast<char*>(run->run_buffer),
												l);
#else
			}
			else {
				run->run_record = PREV_RUN_RECORD(run->run_record);
				if ((record = (SORT_RECORD *) run->run_record) >=
					run->run_buffer) {
					++run->run_records;
					continue;
				}
			}

			/* There are records remaining, but we have stepped over the 
			   edge of the cache.  Read the next buffer full of records. */

			assert((BLOB_PTR *) run->run_end_buffer >
				   (BLOB_PTR *) run->run_buffer);

			space_available =
				(ULONG) ((BLOB_PTR *) run->run_end_buffer -
						 (BLOB_PTR *) run->run_buffer);
			if (mode == RSE_get_forward)
				data_remaining =
					run->run_records * scb->scb_longs * sizeof(ULONG);
			else
				data_remaining =
					(run->run_max_records -
					 run->run_records) * scb->scb_longs * sizeof(ULONG);
			l = MIN(space_available, data_remaining);

			if (mode == RSE_get_forward)
				run->run_seek += run->run_cached;
			else
				run->run_seek -= l;

			run->run_sfb->sfb_mem->read(run->run_seek, run->run_buffer, l);
			run->run_cached = l;

			if (mode == RSE_get_forward) {
#endif
				record = reinterpret_cast < sort_record * >(run->run_buffer);
#ifndef SCROLLABLE_CURSORS
				run->run_record =
					reinterpret_cast <
					sort_record * >(NEXT_RUN_RECORD(record));
#endif
				--run->run_records;
#ifdef SCROLLABLE_CURSORS
			}
			else {
				record = PREV_RUN_RECORD(run->run_end_buffer);
				++run->run_records;
			}

			run->run_record = (SORT_RECORD *) record;
#endif
			continue;
		}

		/* If've we got a record, somebody asked for it.  Find out who. */

		if (record)
			if (merge->mrg_stream_a && !merge->mrg_record_a)
				if (eof)
					merge->mrg_stream_a = NULL;
				else
					merge->mrg_record_a = record;
			else if (eof)
				merge->mrg_stream_b = NULL;
			else
				merge->mrg_record_b = record;


		/* If either streams need a record and is still active, loop back to pick
		   up the record.  If either stream is dry, return the record of the other.
		   If both are dry, indicate eof for this stream.  */

		record = NULL;
		eof = FALSE;

		if (!merge->mrg_record_a && merge->mrg_stream_a) {
			merge = (MRG) merge->mrg_stream_a;
			continue;
		}

		if (!merge->mrg_record_b)
			if (merge->mrg_stream_b) {
				merge = (MRG) merge->mrg_stream_b;
				continue;
			}
			else if ( (record = merge->mrg_record_a) ) {
				merge->mrg_record_a = NULL;
				merge = merge->mrg_header.rmh_parent;
				continue;
			}
			else {
				eof = TRUE;
				record = (SORT_RECORD *) - 1;
				merge = merge->mrg_header.rmh_parent;
				continue;
			}

		if (!merge->mrg_record_a) {
			record = merge->mrg_record_b;
			merge->mrg_record_b = NULL;
			merge = merge->mrg_header.rmh_parent;
			continue;
		}

		/* We have prospective records from each of the sub-streams.  Compare them.
		   If equal, offer each to user routine for possible sacrifice.  */

		p = merge->mrg_record_a->sort_record_key;
		q = merge->mrg_record_b->sort_record_key;
		l = scb->scb_key_length;

		DO_32_COMPARE(p, q, l);

		if (l == 0 && scb->scb_dup_callback) {
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) merge->mrg_record_a, scb, FALSE);
			SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, FALSE);
#else
			diddle_key((UCHAR *) merge->mrg_record_a, scb, FALSE);
			diddle_key((UCHAR *) merge->mrg_record_b, scb, FALSE);
#endif
			if (reinterpret_cast < UCHAR(*)(...) >
				(*scb->scb_dup_callback) (merge->mrg_record_a,
										  merge->mrg_record_b,
										  scb->scb_dup_callback_arg)) {
				merge->mrg_record_a = NULL;
#ifdef SCROLLABLE_CURSORS
				SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, TRUE);
#else
				diddle_key((UCHAR *) merge->mrg_record_b, scb, TRUE);
#endif
				continue;
			}
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) merge->mrg_record_a, scb, TRUE);
			SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, TRUE);
#else
			diddle_key((UCHAR *) merge->mrg_record_a, scb, TRUE);
			diddle_key((UCHAR *) merge->mrg_record_b, scb, TRUE);
#endif
		}

#ifdef SCROLLABLE_CURSORS
		if (mode == RSE_get_forward && p[-1] < q[-1])
#else
		if (p[-1] < q[-1])
#endif
		{
			record = merge->mrg_record_a;
			merge->mrg_record_a = NULL;
		}
		else {
			record = merge->mrg_record_b;
			merge->mrg_record_b = NULL;
		}

		merge = merge->mrg_header.rmh_parent;
	}

/* Merge pointer is null; we're done.  Return either the most
   recent record, or end of file, as appropriate.  */

	return (eof) ? NULL : record;
}


static void init(SCB scb)
{
/**************************************
 *
 *      i n i t
 *
 **************************************
 *
 * Functional description
 *      Initialize the sort control block for a quick sort.
 *
 **************************************/

	scb->scb_next_pointer = scb->scb_first_pointer;
	scb->scb_last_record = (SR *) scb->scb_end_memory;

	*scb->scb_next_pointer++ = reinterpret_cast < sort_record * >(low_key);
}


static bool local_fini(SCB scb, ATT att)
{
/**************************************
 *
 *      l o c a l _ f i n i
 *
 **************************************
 *
 * Functional description
 *      Finish sort, and release all resources.
 *
 **************************************/
	WFS space;
	RUN run;
	SFB sfb;
	ULONG **merge_buf;
	BOOLEAN found_it = TRUE;
	SCB *ptr;

	if (att) {

		/* cover case where a posted error caused reuse by another thread */

		if (scb->scb_attachment != att)
			att = scb->scb_attachment;
		found_it = FALSE;
	}

/* Start by unlinking from que, if present */

	if (att)
		for (ptr = &att->att_active_sorts; *ptr; ptr = &(*ptr)->scb_next)
			if (*ptr == scb) {
				*ptr = scb->scb_next;
				found_it = TRUE;
				break;
			}

/*  *NO*.  I won't free it if it's not in
    the pointer list that has been passed
    to me.  THIS MEANS MEMORY LEAK.  --mrs*/

	if (found_it != TRUE)
		return (FALSE);

/* Loop through the sfb list and close work files. */

	while ( (sfb = scb->scb_sfb) ) {
		scb->scb_sfb = sfb->sfb_next;
		DLS_put_temp_space(sfb);

		delete sfb->sfb_mem;

		close(sfb->sfb_file);

		if (sfb->sfb_file_name) {
			gds__free(sfb->sfb_file_name);
			sfb->sfb_file_name = NULL;
		}

		while ( (space = sfb->sfb_free_wfs) ) {
			sfb->sfb_free_wfs = space->wfs_next;
			gds__free(space);
		}

		while ( (space = sfb->sfb_file_space) ) {
			sfb->sfb_file_space = space->wfs_next;
			gds__free(space);
		}

		gds__free(sfb);
	}

/* get rid of extra merge space */

	while ( (merge_buf = (ULONG **) scb->scb_merge_space) ) {
		scb->scb_merge_space = *merge_buf;
		gds__free(merge_buf);
	}

/* If runs are allocated and not in the big block, release them.  Then release
   the big block. */

	if (scb->scb_memory) {
		gds__free(scb->scb_memory);
		scb->scb_memory = NULL;
	}

/* Clean up the runs that were used */
	while ( (run = scb->scb_runs) ) {
		scb->scb_runs = run->run_next;
		if (run->run_buff_alloc)
			gds__free(run->run_buffer);
		gds__free(run);
	}

/* Clean up the free runs also */
	while ( (run = scb->scb_free_runs) ) {
		scb->scb_free_runs = run->run_next;
		if (run->run_buff_alloc)
			gds__free(run->run_buffer);
		gds__free(run);
	}

	if (scb->scb_merge_pool) {
		gds__free(scb->scb_merge_pool);
		scb->scb_merge_pool = NULL;
	}

	scb->scb_merge = NULL;
	return (TRUE);
}


static void merge_runs(SCB scb, USHORT n)
{
/**************************************
 *
 *      m e r g e _ r u n s
 *
 **************************************
 *
 * Functional description
 *      Merge the first n runs hanging off the sort control block, pushing
 *      the resulting run back onto the sort control block.
 *
 **************************************/
	USHORT count, rec_size, buffers;
	SORT_RECORD *p;
	SORT_RECORD *q;
	ULONG size, seek;
	RUN run;
	struct mrg blks[32];
	struct run temp_run;
	MRG merge;
	RMH *m1, *m2, streams[32];
	BLOB_PTR *buffer;

	assert((n - 1) <= FB_NELEM(blks));	/* stack var big enuf? */

	scb->scb_longs -= SIZEOF_SR_BCKPTR_IN_LONGS;

/* Make a pass thru the runs allocating buffer space, computing work file
   space requirements, and filling in a vector of streams with run pointers. */

	rec_size = scb->scb_longs << SHIFTLONG;
	buffers = scb->scb_size_memory / rec_size;
	size = rec_size * (buffers / (USHORT) (2 * n));
	buffer = (BLOB_PTR *) scb->scb_first_pointer;
	temp_run.run_end_buffer =
		(SORTP *) (buffer + (scb->scb_size_memory / rec_size) * rec_size);
	m1 = streams;
	temp_run.run_size = 0;
	temp_run.run_buff_alloc = 0;

	for (run = scb->scb_runs, count = 0; count < n;
		 run = run->run_next, count++) {
		*m1++ = (RMH) run;

		/* size = 0 indicates the record is too big to divvy up the
		   big sort buffer, so separate buffers must be allocated */

		if (!size) {
			if (!run->run_buff_alloc) {
				try {
					run->run_buffer =
						(ULONG *) gds__alloc((SLONG) rec_size * 2);
				} catch (const std::exception&) {
					/* FREE: smb_merge_space freed in local_fini() when scb released */
					if (!run->run_buffer)
						error_memory(scb);
				}
				run->run_buff_alloc = 1;
			}
			run->run_end_buffer =
				reinterpret_cast <
				ULONG * >((BLOB_PTR *) run->run_buffer + (rec_size * 2));
			run->run_record =
				reinterpret_cast < sort_record * >(run->run_end_buffer);
		}
		else {
			run->run_buffer = (ULONG *) buffer;
			buffer += size;
			run->run_record =
				reinterpret_cast < sort_record * >(run->run_end_buffer =
												   (ULONG *) buffer);
		}
		temp_run.run_size += run->run_size;
	}
	temp_run.run_record = reinterpret_cast < sort_record * >(buffer);
	temp_run.run_buffer = reinterpret_cast < ULONG * >(temp_run.run_record);

/* Build merge tree bottom up */
/* See also kissing cousin of this loop in SORT_sort() */

	for (count = n, merge = blks; count > 1;) {
		m1 = m2 = streams;
		while (count >= 2) {
			merge->mrg_header.rmh_type = TYPE_MRG;

			assert(((*m1)->rmh_type == TYPE_MRG) ||	/* garbage watch */
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_a = *m1++;

			assert(((*m1)->rmh_type == TYPE_MRG) ||	/* garbage watch */
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_b = *m1++;

			merge->mrg_record_a = NULL;
			merge->mrg_record_b = NULL;
			*m2++ = (RMH) merge;
			merge++;
			count -= 2;
		}
		if (count)
			*m2++ = *m1++;
		count = m2 - streams;
	}

	--merge;
	merge->mrg_header.rmh_parent = NULL;

/* Merge records into run */

	q = reinterpret_cast < sort_record * >(temp_run.run_buffer);
	seek = temp_run.run_seek =
		find_file_space(scb, temp_run.run_size, &temp_run.run_sfb);
	temp_run.run_records = 0;

#ifdef SCROLLABLE_CURSORS
	while (p = get_merge(merge, scb, RSE_get_forward))
#else
	while ( (p = get_merge(merge, scb)) )
#endif
	{
		if (q >= (SORT_RECORD *) temp_run.run_end_buffer) {
			size = (BLOB_PTR *) q - (BLOB_PTR *) temp_run.run_buffer;
			seek = temp_run.run_sfb->sfb_mem->write(scb->scb_status_vector, seek,
													reinterpret_cast<char*>(temp_run.run_buffer),
													size);
			q = reinterpret_cast < sort_record * >(temp_run.run_buffer);
		}
		count = scb->scb_longs;
		do
			*q++ = *p++;
		while (--count);
		++temp_run.run_records;
	}
#ifdef SCROLLABLE_CURSORS
	temp_run.run_max_records = temp_run.run_records;
#endif

/* Write the tail of the new run and return any unused space. */

	if ( (size = (BLOB_PTR *) q - (BLOB_PTR *) temp_run.run_buffer) )
		seek = temp_run.run_sfb->sfb_mem->write(scb->scb_status_vector, seek,
												reinterpret_cast<char*>(temp_run.run_buffer),
												size);

/* if the records did not fill the allocated run (such as when duplicates are 
   rejected), then free the remainder and diminish the size of the run accordingly */

	if (seek - temp_run.run_seek < temp_run.run_size) {
		free_file_space(scb, temp_run.run_sfb, seek,
						temp_run.run_seek + temp_run.run_size - seek);
		temp_run.run_size = seek - temp_run.run_seek;
	}

/* Make a final pass thru the runs releasing space, blocks, etc. */

	for (count = 0; count < n; count++) {
		/* Remove run from list of in-use run blocks */
		run = scb->scb_runs;
		scb->scb_runs = run->run_next;
#ifdef SCROLLABLE_CURSORS
		seek = run->run_seek + run->run_cached - run->run_size;
#else
		seek = run->run_seek - run->run_size;
#endif
		/* Free the sort file space associated with the run */

		free_file_space(scb, run->run_sfb, seek, run->run_size);

		/* Add run descriptor to list of unused run descriptor blocks */

		run->run_next = scb->scb_free_runs;
		scb->scb_free_runs = run;
	}

	scb->scb_free_runs = run->run_next;
	if (run->run_buff_alloc) {
		gds__free(run->run_buffer);
		run->run_buff_alloc = 0;
	}
	temp_run.run_header.rmh_type = TYPE_RUN;
	temp_run.run_depth = run->run_depth;
	*run = temp_run;
	run->run_next = scb->scb_runs;
	++run->run_depth;
	scb->scb_runs = run;

	scb->scb_longs += SIZEOF_SR_BCKPTR_IN_LONGS;
}


static void quick(SLONG size, SORTP ** pointers, USHORT length)
{
/**************************************
 *
 *      q u i c k
 *
 **************************************
 *
 * Functional description
 *      Sort an array of record pointers.  The routine assumes the 
 *      following:
 *
 *      a.  Each element in the array points to the key of a record.
 *
 *      b.  Keys can be compared by auto-incrementing unsigned longword
 *          compares.
 *
 *      c.  Relative array positions "-1" and "size" point to guard records
 *          containing the least and the greatest possible sort keys.
 *
 *      ***************************************************************
 *      * Boy, did the assumption below turn out to be pretty stupid! *
 *      ***************************************************************
 *
 *      Note: For the time being, the key length field is ignored on the
 *      assumption that something will eventually stop the comparison.
 *
 *      WARNING: THIS ROUTINE DOES NOT MAKE A FINAL PASS TO UNSCRAMBLE
 *      PARITIONS OF SIZE TWO.  THE POINTER ARRAY REQUIRES ADDITIONAL
 *      PROCESSING BEFORE IT MAY BE USED!
 *
 **************************************/
	SORTP **stack_lower[50];
	SORTP **stack_upper[50];
	SORTP ***sl;
	SORTP ***su;
	SORTP *temp;
	SORTP **r;
	SORTP **i;
	SORTP **j;
	ULONG key;
	SORTP *p;
	SORTP *q;
	SLONG interval;
	USHORT tl;

#define exchange(x, y)  {temp = x; x = y; y = temp;}

	sl = stack_lower;
	su = stack_upper;
	*sl++ = pointers;
	*su++ = pointers + size - 1;

	while (sl > stack_lower) {

		/* Pick up the next interval off the respective stacks */

		r = *--sl;
		j = *--su;

		/* Compute the interval.  If two or less, defer the sort to a final
		   pass. */

		interval = j - r;
		if (interval < 2)
			continue;

		/* Go guard against pre-ordered data, swap the first record with the
		   middle record.  This isn't perfect, but it is cheap. */

		i = r + interval / 2;
		((SORTP ***) (*r))[-1] = i;
		((SORTP ***) (*i))[-1] = r;
		exchange(*r, *i);

		/* Prepare to do the partition.  Pick up the first longword of the
		   key to speed up comparisons. */

		i = r + 1;
		key = **r;

		/* From each end of the interval converge to the middle swapping out of
		   parition records as we go.  Stop when we converge. */

		while (TRUE) {
			while (**i < key)
				i++;
			if (**i == key)
				while (TRUE) {
					p = *i;
					q = *r;
					tl = length - 1;
					while (tl && *p == *q) {
						p++;
						q++;
						tl--;
					}
					if (tl && *p > *q)
						break;
					i++;
				}

			while (**j > key)
				j--;
			if (**j == key)
				while (j != r) {
					p = *j;
					q = *r;
					tl = length - 1;
					while (tl && *p == *q) {
						p++;
						q++;
						tl--;
					}
					if (tl && *p < *q)
						break;
					j--;
				}
			if (i >= j)
				break;
			((SORTP ***) (*i))[-1] = j;
			((SORTP ***) (*j))[-1] = i;
			exchange(*i, *j);
			i++;
			j--;
		}

		/* We have formed two partitions, separated by a slot for the
		   initial record "r".  Exchange the record currently in the
		   slot with "r". */

		((SORTP ***) (*r))[-1] = j;
		((SORTP ***) (*j))[-1] = r;
		exchange(*r, *j);

		/* Finally, stack the two intervals, longest first. */

		i = *su;
		if ((j - r) > (i - j + 1)) {
			*sl++ = r;
			*su++ = j - 1;
			*sl++ = j + 1;
			*su++ = i;
		}
		else {
			*sl++ = j + 1;
			*su++ = i;
			*sl++ = r;
			*su++ = j - 1;
		}
	}
}


static ULONG order(SCB scb)
{
/**************************************
 *
 *      o r d e r
 *
 **************************************
 *
 * Functional description
 *      The memoryfull of record pointers have been sorted, but more
 *      records remain, so the run will have to be written to disk.  To
 *      speed this up, re-arrange the records in physical order so they
 *      can be written with a single disk write.
 *
 **************************************/
	SR *record;
	SORT_RECORD *output;
	SORT_PTR *lower_limit;
	SORT_RECORD **ptr;
	SORTP* buffer = 0;
	SSHORT length;
	ULONG temp[1024];

	ptr = scb->scb_first_pointer + 1;	/* 1st ptr is low key */

/* last inserted record, also the top of the memory where SORT_RECORDS can
   be written */
	lower_limit = reinterpret_cast < SORT_PTR * >(output =
												  reinterpret_cast <
												  sort_record *
												  >(scb->scb_last_record));

	try {
	if ((scb->scb_longs * sizeof(ULONG)) > sizeof(temp))
		buffer =
			(ULONG *) gds__alloc((SLONG) (scb->scb_longs*sizeof(ULONG)));
	/* FREE: buffer is freed later in this routine */
	else
		buffer = temp;
	} catch(const std::exception&) {
		if (!buffer)
			error_memory(scb);
	}

/* Check out the engine */

	THREAD_EXIT;

/* length of the key part of the record  */
	length = scb->scb_longs - SIZEOF_SR_BCKPTR_IN_LONGS;

/* scb_next_pointer points to the end of pointer memory or the beginning of
   records */
	while (ptr < scb->scb_next_pointer) {
		/* If the next pointer is null, it's record has been eliminated as a
		   duplicate.  This is the only easy case. */

		if (!(record = reinterpret_cast < SR * >(*ptr++)))
			continue;

		/* make record point back to the starting of SR struct.
		   as all scb_*_pointer point to the key_id locations! */
		record =
			reinterpret_cast <
			SR * >(((SORTP *) record) - SIZEOF_SR_BCKPTR_IN_LONGS);

		/* If the lower limit of live records points to a deleted or used record,
		   advance the lower limit. */

		while (!*(lower_limit)
			   && (lower_limit < (SORT_PTR *) scb->scb_end_memory))
			lower_limit =
				reinterpret_cast <
				SORT_PTR * >(((SORTP *) lower_limit) + scb->scb_longs);

		/* If the record we want to move won't interfere with lower active
		   record, just move the record into position. */

		if (record->sr_sort_record.sort_record_key == (ULONG *) lower_limit) {
			MOVE_32(length, record->sr_sort_record.sort_record_key, output);
			output =
				reinterpret_cast < sort_record * >((SORTP *) output + length);
			continue;
		}

		if (((SORTP *) output) + scb->scb_longs - 1 <= (SORTP *) lower_limit) {
			/* null the bckptr for this record */
			record->sr_bckptr = NULL;
			MOVE_32(length, record->sr_sort_record.sort_record_key, output);
			output =
				reinterpret_cast < sort_record * >((SORTP *) output + length);
			continue;
		}

		/* There's another record sitting where we want to put our record.  Move
		   the next logical record to a temp, move the lower limit record to the
		   next record's old position (adjusting pointers as we go), then move
		   the current record to output. */

		MOVE_32(length, (SORTP *) record->sr_sort_record.sort_record_key,
				buffer);

		**((SORT_PTR ***) lower_limit) =
			reinterpret_cast <
			SORT_PTR * >(record->sr_sort_record.sort_record_key);
		MOVE_32(scb->scb_longs, lower_limit, record);
		lower_limit = (SORT_PTR *) ((SORTP *) lower_limit + scb->scb_longs);

		MOVE_32(length, buffer, output);
		output =
			reinterpret_cast <
			sort_record * >((SORT_PTR *) ((SORTP *) output + length));
	}

/* Check back into the engine */

	THREAD_ENTER;

/* It's OK to free this after checking back into the engine, there's
 * only fatal failures possible there 
 */

	if (buffer != temp)
		if (buffer != NULL)
			gds__free(buffer);

	return (((SORTP *) output) -
			((SORTP *) scb->scb_last_record)) / (scb->scb_longs -
												 SIZEOF_SR_BCKPTR_IN_LONGS);
}


static void put_run(SCB scb)
{
/**************************************
 *
 *      p u t _ r u n
 *
 **************************************
 *
 * Functional description
 *      Memory has been exhausted.  Do a sort on what we have and write
 *      it to the scratch file.  Keep in mind that since duplicate records
 *      may disappear, the number of records in the run may be less than
 *      were sorted.
 *
 **************************************/
	RUN run;
	//ULONG n, records;

	if ( (run = scb->scb_free_runs) )
		scb->scb_free_runs = run->run_next;
	else {
		run = (RUN) sort_alloc(scb, (ULONG) sizeof(struct run));
		/* FREE: run will be either on the scb_runs or scb_free_runs list,
		 *       which are freed in local_fini() */
	}

	run->run_next = scb->scb_runs;
	scb->scb_runs = run;
	run->run_header.rmh_type = TYPE_RUN;
	run->run_depth = 0;

/* Do the in-core sort.  The first phase a duplicate handling we be performed
   in "sort". */

	sort(scb);

/* Re-arrange records in physical order so they can be dumped in a single write
   operation. */

#ifdef SCROLLABLE_CURSORS
	run->run_records = run->run_max_records = order(scb);
	run->run_cached = 0;
#else
	run->run_records = order(scb);
#endif

/* Write records to scratch file.  Keep track of the number of bytes
   written, etc.  */

	run->run_size =
		run->run_records * (scb->scb_longs -
							SIZEOF_SR_BCKPTR_IN_LONGS) * sizeof(ULONG);
	run->run_seek = find_file_space(scb, run->run_size, &run->run_sfb);
	run->run_sfb->sfb_mem->write(scb->scb_status_vector, run->run_seek,
								 reinterpret_cast<char*>(scb->scb_last_record),
								 run->run_size);
}


static void sort(SCB scb)
{
/**************************************
 *
 *      s o r t
 *
 **************************************
 *
 * Functional description
 *      Set up for and call quick sort.  Quicksort, by design, doesn't
 *      order partitions of length 2, so make a pass thru the data to
 *      straighten out pairs.  While we at it, if duplicate handling has
 *      been requested, detect and handle them.
 *
 **************************************/
	SORTP **i;
	SORTP **j;
	SORTP *p;
	SORTP *q;
	SORTP *temp;
	ULONG n;
	USHORT tl;

/* Check out the engine */

	THREAD_EXIT;

/* First, insert a pointer to the high key. */

	*scb->scb_next_pointer = reinterpret_cast < sort_record * >(high_key);

/* Next, call QuickSort.  Keep in mind that the first pointer is the
   low key and not a record. */

	j = (SORTP **) (scb->scb_first_pointer) + 1;
	n = (SORTP **) (scb->scb_next_pointer) - j;	/* calculate # of records */

	quick(n, j, scb->scb_longs);

/* Scream through and correct any out of order pairs. */

	while (j < (SORTP **) scb->scb_next_pointer) {
		i = j;
		j++;
		if (**i >= **j) {
			p = *i;
			q = *j;
			tl = scb->scb_longs - 1;
			while (tl && *p == *q) {
				p++;
				q++;
				tl--;
			}
			if (tl && *p > *q) {
				((SORTP ***) (*i))[-1] = j;
				((SORTP ***) (*j))[-1] = i;
				temp = *i;
				*i = *j;
				*j = temp;
			}
		}
	}

/* If duplicate handling hasn't been requested, we're done. */

	if (!scb->scb_dup_callback) {
		/* Check back into the engine */

		THREAD_ENTER;
		return;
	}

/* Make another pass and eliminate duplicates.  It's possible to do this
   is the same pass the final ordering, but the logic is complicated enough
   to screw up register optimizations.  Better two fast passes than one
   slow pass, I suppose.  Prove me wrong and win a trip for two to
   Cleveland, Ohio. */

	j = reinterpret_cast < SORTP ** >(scb->scb_first_pointer + 1);

	while (j < (SORTP **) scb->scb_next_pointer) {
		i = j;
		j++;
		if (**i != **j)
			continue;
		p = *i;
		q = *j;
		tl = scb->scb_longs - 1;
		while (tl && *p == *q) {
			p++;
			q++;
			tl--;
		}
		if ( (p > *i) && (ULONG) (p - *i) >= scb->scb_key_length ) {
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) * i, scb, FALSE);
			SORT_diddle_key((UCHAR *) * j, scb, FALSE);
#else
			diddle_key((UCHAR *) * i, scb, FALSE);
			diddle_key((UCHAR *) * j, scb, FALSE);
#endif
			if (reinterpret_cast < UCHAR(*)(...) >
				(*scb->scb_dup_callback) (*i, *j, scb->scb_dup_callback_arg)) {
				((SORTP ***) (*i))[-1] = NULL;
				*i = NULL;
			}
			else
#ifdef SCROLLABLE_CURSORS
				SORT_diddle_key((UCHAR *) * i, scb, TRUE);
			SORT_diddle_key((UCHAR *) * j, scb, TRUE);
#else
				diddle_key((UCHAR *) * i, scb, TRUE);
			diddle_key((UCHAR *) * j, scb, TRUE);
#endif
		}
	}

/* Check back into the engine */

	THREAD_ENTER;
}

#ifdef NOT_USED_OR_REPLACED
#ifdef DEBUG
static void validate(SCB scb)
{
/**************************************
 *
 *      v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *      Validate data structures.
 *
 **************************************/
	SORTP **ptr;
	SORTP *record;
	ISC_STATUS *status_vector;

	for (ptr = (SORTP **) (scb->scb_first_pointer + 1);
		 ptr < (SORTP **) scb->scb_next_pointer; ptr++) {
		record = *ptr;
		if (record[-1] != (SORTP) ptr) {
			status_vector = scb->scb_status_vector;
			*status_vector++ = gds_arg_gds;
			*status_vector++ = gds_crrp_data_err;
			/* Msg360: sort error: corruption in data structure */
			*status_vector = gds_arg_end;
			ERR_punt();
		}
	}
}
#endif
#endif

#ifdef DEBUG_SORT_TRACE
static void write_trace(
						UCHAR * operation,
						SFB sfb, ULONG seek, BLOB_PTR * address, ULONG length)
{
/**************************************
 *
 *      w r i t e _ t r a c e
 *
 **************************************
 *
 * Functional description
 *      Write a trace record.
 *
 **************************************/
	UCHAR file_name[32], data[41], *p;
#ifdef HAVE_MKSTEMP
	int fd;
#endif

	if (!trace_file) {
#if (defined WIN_NT)
		strcpy(file_name, "/interbas/stXXXXXX");
#else
		strcpy(file_name, "/interbase/DEBUG_SORT_TRACE_XXXXXX");
#endif
#ifdef HAVE_MKSTEMP
		fd = mkstemp(file_name);
		trace_file = fdopen(fd, "w");
#else
		mktemp(file_name);
		trace_file = ib_fopen(file_name, "w");
#endif
	}

	if (!trace_file)
		return;

	for (p = data; p < data + sizeof(data) - 1; address++)
		*p++ = (*address) ? *address : '.';

	*p = 0;

	ib_fprintf(trace_file, "Fid: %d, %.5s %.7ld - %.7ld\t/%s/\n",
			   sfb->sfb_file, operation, seek, seek + length, data);
}
#endif
