/*
 *      PROGRAM:        JRD Sort
 *      MODULE:         sort.cpp
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
 * $Id: sort.cpp,v 1.75 2005-05-27 22:44:05 asfernandes Exp $
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
#include "gen/iberror.h"
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
#include "../jrd/thread_proto.h"

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

#ifdef WIN_NT
/* for SEEK_SET */
#include <io.h> // lseek, read, write, close
#endif

const ULONG IO_RETRY			= 20;
const USHORT RUN_GROUP			= 8;
const USHORT MAX_MERGE_LEVEL	= 2;

#ifdef VMS
double MTH$CVT_D_G(), MTH$CVT_G_D();
#endif

using namespace Jrd;

// The sort buffer size should be just under a multiple of the
// hardware memory page size to account for memory allocator
// overhead. On most platorms, this saves 4KB to 8KB per sort
// buffer from being allocated but not used.

const ULONG SORT_BUFFER_CHUNK_SIZE	= 4096;
const ULONG MIN_SORT_BUFFER_SIZE	= SORT_BUFFER_CHUNK_SIZE * 4;
const ULONG MAX_SORT_BUFFER_SIZE	= SORT_BUFFER_CHUNK_SIZE * 32;

const ULONG MAX_TEMPFILE_SIZE		= 1073741824;	// 1GB

#define DIFF_LONGS(a, b)         ((a) - (b))
#define SWAP_LONGS(a, b, t)       {t = a; a = b; b = t;}

// Compare p and q both SORTP pointers for l 32-bit longwords
// l != 0 if p and q are not equal for l bytes
#define  DO_32_COMPARE(p, q, l)   do if (*p++ != *q++) break; while (--l);

#define MOVE_32(len, from, to)      memcpy(to, from, len * 4)

// These values are not defined as const as they are passed to
// the diddle_key routines which mangles them.
// As the diddle_key routines differ on VAX (little endian) and non VAX
// (big endian) patforms, making the following const caused a core on the
// Intel Platforms, while Solaris was working fine.

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
static sort_record*	get_merge(merge_control*, sort_context*, RSE_GET_MODE);
#else
static void diddle_key(UCHAR *, sort_context*, bool);
static sort_record*	get_merge(merge_control*, sort_context*);
#endif

static UCHAR* sort_alloc(sort_context*, ULONG);
static void error_memory(sort_context*);
static ULONG find_file_space(sort_context*, ULONG, sort_work_file**);
static void free_file_space(sort_context*, sort_work_file*, ULONG, ULONG);
static void init(sort_context*);
static bool local_fini(sort_context*, Attachment*);
static void merge_runs(sort_context*, USHORT);
static void quick(SLONG, SORTP **, ULONG);
static ULONG order(sort_context*);
static void put_run(sort_context*);
static void sort(sort_context*);
#ifdef NOT_USED_OR_REPLACED
#ifdef DEBUG
static void validate(sort_context*);
#endif
#endif

#ifdef DEBUG_SORT_TRACE
static void write_trace(UCHAR*, sort_work_file*, ULONG, BLOB_PTR*, ULONG);
#include <stdio.h>
FILE *trace_file = NULL;
#endif

#ifdef SMALL_FILE_NAMES
static const char* SCRATCH		= "fb_s";
#else
static const char* SCRATCH		= "fb_sort_";
#endif

#ifdef SCROLLABLE_CURSORS
#ifdef WORDS_BIGENDIAN
void SORT_diddle_key(UCHAR* record, sort_context* scb, bool direction)
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
 *      direction - true for SORT_put() and false for SORT_get()
 *
 **************************************/
	const sort_key_def* key = scb->scb_description;
	for (const sort_key_def* const end = key + scb->scb_keys; key < end; key++)
	{
		UCHAR* p = record + key->skd_offset;
		USHORT n = key->skd_length;
		bool complement = key->skd_flags & SKD_descending;

		switch (key->skd_dtype) {
		case SKD_ulong:
		case SKD_ushort:
		case SKD_bytes:
			break;

			// Stash embedded control info for non-fixed data types in the sort
			// record and zap it so that it doesn't interfere with collation

		case SKD_varying:
			if (direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) =
						((vary*) p)->vary_length;
					const UCHAR fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					UCHAR* fill_pos = p + sizeof(USHORT) + ((vary*) p)->vary_length;
					const USHORT fill = n - sizeof(USHORT) - ((vary*) p)->vary_length;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((vary*) p)->vary_length = 0;
			}
			break;

		case SKD_cstring:
			if (direction) {
				const UCHAR fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					const USHORT l = strlen(p);
					*((USHORT *) (record + key->skd_vary_offset)) = l;
					UCHAR* fill_pos = p + l;
					const USHORT fill = n - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				else {
					const USHORT l = *((USHORT *) (record + key->skd_vary_offset));
					*(p + l) = fill_char;
				}
			}
			break;

		case SKD_text:
			break;

		case SKD_float:
		case SKD_double:
			{
				const USHORT flag = (direction || !complement) ? (direction ? TRUE : FALSE) : TRUE;
				if (flag ^ (*p >> 7))
					*p ^= 1 << 7;
				else
					complement = !complement;
				break;
			}

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
			fb_assert(false);
			break;
		}
		if (complement && n)
			do {
				*p++ ^= -1;
			} while (--n);

		// Flatter but don't complement control info for non-fixed
		// data types when restoring the data

		if (key->skd_dtype == SKD_varying && !direction) {
			p = record + key->skd_offset;
			((vary*) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = record + key->skd_offset;
			const USHORT l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}


#else
void SORT_diddle_key(UCHAR* record, sort_context* scb, bool direction)
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
 *      direction - true for SORT_put() and false for SORT_get()
 *
 **************************************/
	UCHAR c1, c2, fill_char, *fill_pos;
	USHORT w, l, fill;
	SSHORT longs, flag;
	ULONG lw;
#ifdef VMS
	double *dp;
#endif

	const sort_key_def* key = scb->scb_description;
	for (const sort_key_def* const end = key + scb->scb_keys; key < end; key++)
	{
		BLOB_PTR* p = (BLOB_PTR *) record + key->skd_offset;
		USHORT* wp = (USHORT *) p;
		SORTP* lwp = (SORTP *) p;
		bool complement = key->skd_flags & SKD_descending;
		USHORT n = ROUNDUP(key->skd_length, sizeof(SLONG));

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

			// Stash embedded control info for non-fixed data types in the sort
			// record and zap it so that it doesn't interfere with collation

			if (key->skd_dtype == SKD_varying && direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						((vary*) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + l;
					fill = n - sizeof(USHORT) - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((vary*) p)->vary_length = 0;
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
			p = (BLOB_PTR*) wp;
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

			// INT64's fit in TWO LONGS, and hence the SWAP has to happen
			// here for the right order comparison using DO_32_COMPARE

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

#else // IEEE
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
#endif // IEEE

		default:
			fb_assert(false);
			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		// Flatter but don't complement control info for non-fixed
		// data types when restoring the data

		if (key->skd_dtype == SKD_varying && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			((vary*) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
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


void SORT_error(ISC_STATUS* status_vector,
				sort_work_file* sfb, TEXT* string, ISC_STATUS operation, int errcode)
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

	fb_assert(status_vector != NULL);

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_io_error;
	*status_vector++ = isc_arg_string;
// CVC: Warning, converting pointer to ISC_STATUS => SLONG in 32 bits.
	*status_vector++ = (ISC_STATUS) string;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS) (U_IPTR) ERR_cstring(sfb->sfb_file_name);
	*status_vector++ = isc_arg_gds;
	*status_vector++ = operation;
	if (errcode) {
		*status_vector++ = SYS_ERR;
		*status_vector++ = errcode;
	}
	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_sort_err;	// Msg355: sort error
	*status_vector = isc_arg_end;

	ERR_punt();
}


void SORT_fini(sort_context* scb, Attachment* att)
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
void SORT_get(ISC_STATUS * status_vector,
			  sort_context* scb, ULONG ** record_address, RSE_GET_MODE mode)
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
 *      If the stream is exhausted, SORT_get puts NULL in <record_address>.
 *
 **************************************/
	sort_record* record;

	scb->scb_status_vector = status_vector;

	// If there were runs, get the records from the merge
	// tree. Otherwise everything fit in memory.

	if (scb->scb_merge)
		record = get_merge(scb->scb_merge, scb, mode);
	else
		switch (mode) {
		case RSE_get_forward:
			if (scb->scb_flags & scb_initialized)
				scb->scb_flags &= ~scb_initialized;

			while (true) {
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
			else {
				// By definition, the next pointer is on the next record,
				// so we have to go back one to get to the last fetched record.
				// This is easier than changing the sense of the next pointer.

				scb->scb_next_pointer--;
				if (scb->scb_next_pointer <= scb->scb_first_pointer + 1) {
					record = NULL;
					scb->scb_next_pointer++;
					break;
				}
			}

			while (true) {
				scb->scb_next_pointer--;
				if (scb->scb_next_pointer <= scb->scb_first_pointer) {
					record = NULL;
					scb->scb_next_pointer++;
					break;
				}
				if (record = *scb->scb_next_pointer)
					break;
			}

			// Reset next pointer to one greater than the last fetched

			scb->scb_next_pointer++;
			break;

#ifdef PC_ENGINE
		case RSE_get_current:
			if (scb->scb_next_pointer <= scb->scb_first_pointer ||
				scb->scb_next_pointer > scb->scb_last_pointer)
			{
				record = NULL;
			}
			record = *scb->scb_next_pointer;
			break;
#endif

		default:
			fb_assert(FALSE);
			break;
		}

	if (record)
		SORT_diddle_key((UCHAR *) record->sort_record_key, scb, false);

	*record_address = (ULONG *) record;
}
#else
void SORT_get(ISC_STATUS* status_vector, sort_context* scb, ULONG** record_address)
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
 *      If the stream is exhausted, SORT_get puts NULL in <record_address>.
 *
 **************************************/
	sort_record* record;

	scb->scb_status_vector = status_vector;

	// If there weren't any runs, everything fit in memory. Just return stuff.

	if (!scb->scb_merge)
		while (true) {
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
		diddle_key((UCHAR *) record->sort_record_key, scb, false);
	}
}
#endif


sort_context* SORT_init(ISC_STATUS* status_vector,
			  USHORT record_length,
			  USHORT keys,
			  USHORT unique_keys,
			  const sort_key_def* key_description,
			  FPTR_REJECT_DUP_CALLBACK call_back,
			  void* user_arg,
			  Attachment* att,
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
 * hvlad: when duplicates are eliminating only first unique_keys will be
 *		compared. This is used at creation of unique index since sort key
 *		includes index key (which must be unique) and record numbers
 *
 **************************************/

	// Allocate and setup a sort context block, including copying the
	// key description vector. Round the record length up to the next
	// longword, and add a longword to a pointer back to the pointer slot.

	sort_context* scb = (sort_context*) gds__alloc((SLONG) SCB_LEN(keys));
	if (!scb) {
		// FREE: scb is freed by SORT_fini(), called by higher level cleanup
		// FREE: or later in this module in error cases
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_sort_mem_err;
		*status_vector = isc_arg_end;
		return NULL;
	}
	memset((UCHAR*) scb, 0, SCB_LEN(keys));

	scb->scb_status_vector = status_vector;
	scb->scb_length = record_length;
	scb->scb_longs =
		ROUNDUP(record_length + sizeof(SLONG*), sizeof(SLONG*)) >> SHIFTLONG;
	scb->scb_dup_callback = call_back;
	scb->scb_dup_callback_arg = user_arg;
	scb->scb_keys = keys;

	scb->scb_max_records = max_records;

	fb_assert(unique_keys <= keys);
	sort_key_def* p = scb->scb_description;
	const sort_key_def* q = key_description;
	do {
		*p++ = *q++;
	} while (--keys);

	--p;
	scb->scb_key_length =
		ROUNDUP(p->skd_offset + p->skd_length, sizeof(SLONG)) >> SHIFTLONG;

	while (unique_keys < scb->scb_keys) {
		p--;
		unique_keys++;
	}
	scb->scb_unique_length =
		ROUNDUP(p->skd_offset + p->skd_length, sizeof(SLONG)) >> SHIFTLONG;

	// Next, try to allocate a "big block". How big? Big enough!
#ifdef DEBUG_MERGE
	// To debug the merge algorithm, force the in-memory pool to be VERY small
	scb->scb_size_memory = 2000;
	scb->scb_memory =
		(SORTP *) gds__alloc((SLONG) scb->scb_size_memory);
	// FREE: scb_memory is freed by local_fini()
#else
	// Try to get a big chunk of memory, if we can't try smaller and
	// smaller chunks until we can get the memory. If we get down to
	// too small a chunk - punt and report not enough memory.

	for (scb->scb_size_memory = MAX_SORT_BUFFER_SIZE;;
		scb->scb_size_memory -= SORT_BUFFER_CHUNK_SIZE)
	{
		if (scb->scb_size_memory < MIN_SORT_BUFFER_SIZE)
			break;
		else if ( (scb->scb_memory =
			 (SORTP *) gds__alloc((SLONG) scb->scb_size_memory)) )
		{
		// FREE: scb_memory is freed by local_fini()
			break;
		}
	}
#endif // DEBUG_MERGE
	if (!scb->scb_memory) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_sort_mem_err; // Msg356: sort error: not enough memory
		*status_vector = isc_arg_end;
		gds__free(scb);
		return NULL;
	}

	scb->scb_end_memory =
		(SORTP *) ((BLOB_PTR *) scb->scb_memory + scb->scb_size_memory);
	scb->scb_first_pointer = (sort_record**) scb->scb_memory;

	// Set up to receive the first record

	init(scb);

	// If a linked list pointer was given, link in new sort block

	if (att) {
		scb->scb_next = att->att_active_sorts;
		att->att_active_sorts = scb;
		scb->scb_attachment = att;
	}

	return scb;
}


void SORT_put(ISC_STATUS * status_vector, sort_context* scb, ULONG ** record_address)
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

	scb->scb_status_vector = status_vector;

	// Find the last record passed in, and zap the keys something comparable
	// by unsigned longword compares

	SR* record = scb->scb_last_record;

	if (record != (SR *) scb->scb_end_memory)
#ifdef SCROLLABLE_CURSORS
		SORT_diddle_key((UCHAR *) (record->sr_sort_record.sort_record_key),
						scb, true);
#else
		diddle_key((UCHAR *) (record->sr_sort_record.sort_record_key), scb,
				   true);
#endif

	// If there isn't room for the record, sort and write the run_control.
	// Check that we are not at the beginning of the buffer in addition
	// to checking for space for the record. This avoids the pointer
	// record from underflowing in the second condition.

	if ((BLOB_PTR *) record < (BLOB_PTR *) (scb->scb_memory + scb->scb_longs)
		|| (BLOB_PTR *) NEXT_RECORD(record) <= (BLOB_PTR *) (scb->scb_next_pointer + 1))
	{
		put_run(scb);
		while (true) {
			run_control* run = scb->scb_runs;
			const USHORT depth = run->run_depth;
			if (depth == MAX_MERGE_LEVEL)
				break;
			USHORT count = 1;
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

	// Make sure the first longword of the record points to the pointer
	scb->scb_last_record = record;
	record->sr_bckptr = scb->scb_next_pointer;

	// Move key_id into *scb->scb_next_pointer and then
	// increment scb->scb_next_pointer
	*scb->scb_next_pointer++ =
		reinterpret_cast<sort_record*>(record->sr_sort_record.sort_record_key);
#ifndef SCROLLABLE_CURSORS
	scb->scb_records++;
#endif
	*record_address = (ULONG *) record->sr_sort_record.sort_record_key;
}


#ifdef SCROLLABLE_CURSORS
void SORT_read_block(
#else
ULONG SORT_read_block(
#endif
						 ISC_STATUS* status_vector,
						 sort_work_file* sfb,
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
	ULONG read_len, i;

#ifdef DEBUG_SORT_TRACE
	UCHAR *org_address;
	ULONG org_length, org_seek;

	org_address = address;
	org_length = length;
	org_seek = seek;
#endif

	// Checkout of engine on sort I/O

	THREAD_EXIT();

	// The following is a crock induced by a VMS C bug

	while (length) {
		const ULONG len = length;
		for (i = 0; i < IO_RETRY; i++) {
			if (lseek(sfb->sfb_file, LSEEK_OFFSET_CAST seek, SEEK_SET) == -1) {
				THREAD_ENTER();
				SORT_error(status_vector, sfb, "lseek", isc_io_read_err, errno);
			}
			if ((read_len = read(sfb->sfb_file, address, len)) == len)
				break;
			else if ((SSHORT) read_len == -1 && !SYSCALL_INTERRUPTED(errno)) {
				THREAD_ENTER();
				SORT_error(status_vector, sfb, "read", isc_io_read_err, errno);
			}
		}

		if (i == IO_RETRY) {
			THREAD_ENTER();
			SORT_error(status_vector, sfb, "read", isc_io_read_err, errno);
		}
		length -= read_len;
		address += read_len;
		seek += read_len;
	}

	THREAD_ENTER();

#ifdef DEBUG_SORT_TRACE
	write_trace("Read", sfb, org_seek, org_address, org_length);
#endif
#ifndef SCROLLABLE_CURSORS
	return seek;
#endif
}


void SORT_shutdown(Attachment* att)
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

	// We ignore the result from local_fini,
	// since the expectation is that from the
	// way we are passing in the structures
	// that every sort_context *IS* part of the ptr
	// chain. Also, we're not freeing the
	// structure here, so if something goes
	// wrong, it's not *CRITICAL*.  -- mrs

	while (att->att_active_sorts)
		local_fini(att->att_active_sorts, att);
}


bool SORT_sort(ISC_STATUS * status_vector, sort_context* scb)
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
 *      If there were runs, sort and write out the last run_control and
 *      build a merge tree.
 *
 **************************************/
	run_control* run;
	merge_control* merge;
	merge_control* merge_pool;

	scb->scb_status_vector = status_vector;

	if (scb->scb_last_record != (SR *) scb->scb_end_memory)
#ifdef SCROLLABLE_CURSORS
		SORT_diddle_key((UCHAR *) KEYOF(scb->scb_last_record), scb, true);
#else
		diddle_key((UCHAR *) KEYOF(scb->scb_last_record), scb, true);
#endif

	// If there aren't any runs, things fit nicely in memory. Just sort the mess
	// and we're ready for output.

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
		return true;
	}

	// Write the last records as a run_control

	put_run(scb);

	// Build a merge tree for the run_control blocks. Start by laying them all out
	// in a vector. This is done to allow us to build a merge tree from the
	// bottom up, ensuring that a balanced tree is built.

	ULONG run_count;
	for (run_count = 0, run = scb->scb_runs; run; run = run->run_next) {
		if (run->run_buff_alloc) {
			gds__free(run->run_buffer);
			run->run_buff_alloc = 0;
		}
		++run_count;
	}

	run_merge_hdr* streams_local[200];
	run_merge_hdr** streams;
	if ((run_count * sizeof(run_merge_hdr*)) > sizeof(streams_local))
		streams =
			(run_merge_hdr**) gds__alloc((SLONG) run_count * sizeof(run_merge_hdr*));
		// FREE: streams is freed later in this routine
	else
		streams = streams_local;
	if (!streams) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_sort_mem_err;
		*status_vector = isc_arg_end;
		return false;
	}

	run_merge_hdr** m1 = streams;
	for (run = scb->scb_runs; run; run = run->run_next)
		*m1++ = (run_merge_hdr*) run;
	ULONG count = run_count;

	// We're building a b-tree of the sort merge blocks, we have (count)
	// leaves already, so we *know* we need (count-1) merge blocks.

	if (count > 1) {
		fb_assert(!scb->scb_merge_pool);	// shouldn't have a pool
		scb->scb_merge_pool =
			(merge_control*) gds__alloc((SLONG) (count - 1) * sizeof(merge_control));
		// FREE: smb_merge_pool freed in local_fini() when the scb is released
		merge_pool = scb->scb_merge_pool;
		if (!merge_pool) {
			gds__free(streams);
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_sort_mem_err;
			*status_vector = isc_arg_end;
			return false;
		}
		memset(merge_pool, 0, (count - 1) * sizeof(merge_control));
	}
	else {
		// Merge of 1 or 0 runs doesn't make sense
		fb_assert(false);				// We really shouldn't get here
		merge = (merge_control*) * streams;	// But if we do...
	}

	// Each pass through the vector builds a level of the merge tree
	// by condensing two runs into one.
	// We will continue to make passes until there is a single item.
	//
	// See also kissing cousin of this loop in merge_runs()

	while (count > 1) {
		run_merge_hdr** m2 = m1 = streams;

		// "m1" is used to sequence through the runs being merged,
		// while "m2" points at the new merged run

		while (count >= 2) {
			merge = merge_pool++;
			merge->mrg_header.rmh_type = TYPE_MRG;

			fb_assert(((*m1)->rmh_type == TYPE_MRG) ||	// garbage watch
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_a = *m1++;

			fb_assert(((*m1)->rmh_type == TYPE_MRG) ||	// garbage watch
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_b = *m1++;

			merge->mrg_record_a = NULL;
			merge->mrg_record_b = NULL;

			*m2++ = (run_merge_hdr*) merge;
			count -= 2;
		}

		if (count)
			*m2++ = *m1++;
		count = m2 - streams;
	}

	if (streams != streams_local)
		gds__free(streams);

	SORTP* buffer = (SORTP *) scb->scb_first_pointer;
	merge->mrg_header.rmh_parent = NULL;
	scb->scb_merge = merge;
	scb->scb_longs -= SIZEOF_SR_BCKPTR_IN_LONGS;

	// Divvy up the sort space among buffers for runs. Although something slightly
	// better could be arranged, for now give them all the same size hunk.

	ULONG size;
	const ULONG temp = DIFF_LONGS(scb->scb_end_memory, buffer);
	count = temp / (scb->scb_longs * run_count);
	if (count) {
		size = count * (SSHORT) scb->scb_longs;
		count = run_count;
	}
	else {
		size = (SSHORT) scb->scb_longs;
		count = temp / scb->scb_longs;
	}

	// Allocate buffer space for either all the runs, if they fit, or for
	// as many as allow

	for (run = scb->scb_runs; run && count; count--, run = run->run_next) {
		run->run_buffer = buffer;
		buffer += size;
		run->run_record =
			reinterpret_cast<sort_record*>(run->run_end_buffer = buffer);
	}

	// If there was not enough buffer space, get some more for the remaining runs
	// allocating enough for the merge space plus a link

	for (; run; run = run->run_next) {
		run->run_buffer =
			(ULONG *) gds__alloc((SLONG) (size * sizeof(ULONG)));
		// FREE: smb_merge_space freed in local_fini() when the scb is released
		if (!run->run_buffer) {
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_sort_mem_err;
			*status_vector = isc_arg_end;
			return false;
		}
		// Link the new buffer into the chain of buffers
		run->run_buff_alloc = 1;
		run->run_record =
			reinterpret_cast<sort_record*>(run->run_end_buffer =
										   run->run_buffer + size);
	}

	scb->scb_flags |= scb_sorted;
	return true;
}


ULONG SORT_write_block(ISC_STATUS* status_vector,
					   sort_work_file* sfb, ULONG seek, BLOB_PTR* address, ULONG length)
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
	ULONG write_len, i;

#ifdef DEBUG_SORT_TRACE
	write_trace("Write", sfb, seek, address, length);
#endif

	// Check out of engine on sort I/O

	THREAD_EXIT();

	// The following is a crock induced by a VMS C bug

	while (length) {
		ULONG len = length;
		for (i = 0; i < IO_RETRY; i++) {
			if (lseek(sfb->sfb_file, LSEEK_OFFSET_CAST seek, SEEK_SET) == -1) {
				THREAD_ENTER();
				SORT_error(status_vector, sfb, "lseek", isc_io_write_err,
						   errno);
			}
			if ((write_len = write(sfb->sfb_file, address, len)) == len)
				break;
			else {
				if (write_len >= 0)
					// If write returns value that is not equal len, then
					// most likely there is not enough space, try to write
					// one more time to get meaningful errno
					write_len = write(sfb->sfb_file, address + write_len,
									  len - write_len);
				if ((SSHORT) write_len == -1 && !SYSCALL_INTERRUPTED(errno)) {
					THREAD_ENTER();
					SORT_error(status_vector, sfb, "write", isc_io_write_err,
							   errno);
				}
			}
		}

		if (i == IO_RETRY) {
			THREAD_ENTER();
			SORT_error(status_vector, sfb, "write", isc_io_write_err, errno);
		}
		length -= write_len;
		address += write_len;
		seek += write_len;
	}

	THREAD_ENTER();

	return seek;
}


static UCHAR *sort_alloc(sort_context* scb, ULONG size)
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
 *        - Most things allocated have pointers placed in the sort_context.
 *          (sort control block)
 *        - There is an error handler set up by our caller, which will
 *          call back to SORT_fini(), which frees all the memory
 *          chains that hang off the sort_context.
 *        - There are some short-term allocations done (for instance,
 *          when sorting a run_control before writing it to disk).  There appears
 *          to be no need to have an error handler to free them as
 *          no errors can be posted during the process.
 *
 *      1994-October-11 David Schnepper
 *
 **************************************/
	UCHAR* const block =
		reinterpret_cast<UCHAR*>(gds__alloc(size));
	// FREE: caller responsible for freeing
	if (!block)
	{
		error_memory(scb);
		return NULL;
	}

	memset(block, 0, size);
	return block;
}

#ifndef SCROLLABLE_CURSORS
#ifdef WORDS_BIGENDIAN
static void diddle_key(UCHAR * record, sort_context* scb, bool direction)
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
	UCHAR *fill_pos, fill_char;
	USHORT l, fill, flag;

	for (sort_key_def* key = scb->scb_description, *end = key + scb->scb_keys;
		 key < end; key++)
	{
		UCHAR* p = record + key->skd_offset;
		USHORT n = key->skd_length;
		USHORT complement = key->skd_flags & SKD_descending;

		switch (key->skd_dtype) {
		case SKD_ulong:
		case SKD_ushort:
		case SKD_bytes:
		case SKD_sql_time:
			break;

			// Stash embedded control info for non-fixed data types in the sort
			// record and zap it so that it doesn't interfere with collation

		case SKD_varying:
			if (direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) =
						((vary*) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + ((vary*) p)->vary_length;
					fill = n - sizeof(USHORT) - ((vary*) p)->vary_length;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((vary*) p)->vary_length = 0;
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
		case SKD_float:
		case SKD_double:
			flag = (direction || !complement)
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
			fb_assert(false);
			break;
		}
		if (complement && n)
			do
				*p++ ^= -1;
			while (--n);

		// Flatter but don't complement control info for non-fixed
		// data types when restoring the data

		if (key->skd_dtype == SKD_varying && !direction) {
			p = record + key->skd_offset;
			((vary*) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
		}

		if (key->skd_dtype == SKD_cstring && !direction) {
			p = record + key->skd_offset;
			l = *((USHORT *) (record + key->skd_vary_offset));
			*(p + l) = 0;
		}
	}
}


#else
static void diddle_key(UCHAR * record, sort_context* scb, bool direction)
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
	UCHAR c1, fill_char, *fill_pos;
	USHORT l, fill;
	SSHORT longs, flag;
	ULONG lw;
#ifdef VMS
	double *dp;
#endif
#ifndef IEEE
	USHORT w;
#endif

	for (sort_key_def* key = scb->scb_description, *end = key + scb->scb_keys;
		 key < end; key++)
	{
		BLOB_PTR* p = (BLOB_PTR *) record + key->skd_offset;
		USHORT* wp = (USHORT *) p;
		SORTP* lwp = (SORTP *) p;
		USHORT complement = key->skd_flags & SKD_descending;
		USHORT n = ROUNDUP(key->skd_length, sizeof(SLONG));

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

			// Stash embedded control info for non-fixed data types in the sort
			// record and zap it so that it doesn't interfere with collation

			if (key->skd_dtype == SKD_varying && direction) {
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						((vary*) p)->vary_length;
					fill_char =
						(key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
					fill_pos = p + sizeof(USHORT) + l;
					fill = n - sizeof(USHORT) - l;
					if (fill)
						memset(fill_pos, fill_char, fill);
				}
				((vary*) p)->vary_length = 0;
			}

			if (key->skd_dtype == SKD_cstring && direction) {
				fill_char = (key->skd_flags & SKD_binary) ? 0 : ASCII_SPACE;
				if (!(scb->scb_flags & scb_sorted)) {
					*((USHORT *) (record + key->skd_vary_offset)) = l =
						strlen(reinterpret_cast<const char*>(p));
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
			// INT64's fit in TWO LONGS, and hence the SWAP has to happen
			// here for the right order comparison using DO_32_COMPARE
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

#else // IEEE
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
#endif // IEEE

		default:

		// Don't want the debug version to
        // stop because of skd_type = 0
		// FSG 22.Dez.2000
		//
        //	fb_assert(false);
			break;
		}
		if (complement && n)
			do {
				*p++ ^= -1;
			} while (--n);

		// Flatter but don't complement control info for non-fixed
		// data types when restoring the data

		if (key->skd_dtype == SKD_varying && !direction) {
			p = (BLOB_PTR *) record + key->skd_offset;
			((vary*) p)->vary_length = *((USHORT *) (record + key->skd_vary_offset));
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


static void error_memory(sort_context* scb)
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
	ISC_STATUS* status_vector = scb->scb_status_vector;

	fb_assert(status_vector != NULL);

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_sort_mem_err;
	*status_vector = isc_arg_end;
	ERR_punt();
}


static ULONG find_file_space(sort_context* scb, ULONG size, sort_work_file** ret_sfb)
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
	TEXT file_name[MAXPATHLEN];

	// Find the best available space. This is defined as the smallest free space
	// that is big enough. This preserves large blocks.

	work_file_space** best = NULL;
	sort_work_file* last_sfb = NULL;
	file_name[0] = '\0';

	// Search through the available space in the work file list

	sort_work_file* best_sfb = 0;
	sort_work_file* sfb;
	sort_work_file** sfb_ptr;
	for (sfb_ptr = &scb->scb_sfb; (sfb = *sfb_ptr); sfb_ptr = &sfb->sfb_next) {
		work_file_space* space;
		for (work_file_space** ptr = &sfb->sfb_file_space; (space = *ptr);
			 ptr = &(*ptr)->wfs_next)
		{

			// If this is smaller than our previous best, use it

			if (space->wfs_size >= size &&
                (!best || (space->wfs_size < (*best)->wfs_size)))
			{
				best = ptr;
				best_sfb = sfb;
			}
		}

		// Save the previous sfb pointer because when we get out of this
		// for loop, sfb would be a NULL pointer

		last_sfb = sfb;
	}
	sfb = last_sfb;

	// If we didn't find any space, allocate it at the end of the file

	if (!best) {

		// If there is no file allocated yet or the size requested is bigger
		// than available space in the current directory, create a new file
		// and return

		if (!sfb || !DLS_get_temp_space(size, sfb) ||
			(sfb->sfb_file_size + size >= MAX_TEMPFILE_SIZE))
		{

			sfb = (sort_work_file*) sort_alloc(scb, (ULONG) sizeof(sort_work_file));
			// FREE: scb_sfb chain is freed in local_fini()

			// Is the last dir_list at it's size limit? If so, add a new dir_list
			// M.E.G

			if (last_sfb && (last_sfb->sfb_dls->dls_inuse + size >= MAX_TEMPFILE_SIZE))
				if (!DLS_add_dir(MAX_TEMPFILE_SIZE, last_sfb->sfb_dls->dls_directory))
					error_memory(scb);

			if (last_sfb)
				last_sfb->sfb_next = sfb;
			else
				scb->scb_sfb = sfb;

			// Find a free space

			sfb->sfb_dls = NULL;
			if (!DLS_get_temp_space(size, sfb))
				// There is not enough space
				error_memory(scb);

			// Create a scratch file

			sfb->sfb_file =
				(int) (IPTR) gds__temp_file(FALSE, SCRATCH, file_name,
									 sfb->sfb_dls->dls_directory, TRUE);

			// allocate the file name even if the file is not open,
			// because the error routine depends on it.
			// This is released during local_fini()

			sfb->sfb_file_name =
				FB_NEW(*getDefaultMemoryPool()) char[strlen(file_name) + 1];
			// FREE: sfb_file_name is freed in local_fini()

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

	// Set up the return parameters

	*ret_sfb = best_sfb;
	work_file_space* space = *best;

	// If the hunk was an exact fit, remove the work file space block from the
	// list and splice it into the free list

	if (space->wfs_size == size) {
		*best = space->wfs_next;
		space->wfs_next = best_sfb->sfb_free_wfs;
		best_sfb->sfb_free_wfs = space;
		return space->wfs_position;
	}

	// The best block is too big - chop the needed space off the end

	space->wfs_size -= size;

	return space->wfs_position + space->wfs_size;
}


static void free_file_space(sort_context* scb, sort_work_file* sfb,
							ULONG position, ULONG size)
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
	work_file_space* space;
	work_file_space** ptr;

	fb_assert(size > 0);
	fb_assert(position < sfb->sfb_file_size);	// Block starts in file
	const ULONG end = position + size;
	fb_assert(end <= sfb->sfb_file_size);		// Block ends in file

	// Search through work file space blocks looking for an adjacent block

	for (ptr = &sfb->sfb_file_space; (space = *ptr); ptr = &space->wfs_next) {
		if (end >= space->wfs_position)
			break;
	}

	if (space) {
		// may have found an adjacent block - try to join them together

		if (end == space->wfs_position) {
			// newly freed block starts just before previously freed
			space->wfs_position -= size;
			space->wfs_size += size;
			return;
		}

		if (position == space->wfs_position + space->wfs_size) {
			// newly freed block starts just after previously freed
			space->wfs_size += size;

			work_file_space* next = space->wfs_next;
			if (next && end == next->wfs_position) {
				// The NEXT freed block is adjacent, join it too
				space->wfs_size += next->wfs_size;
				space->wfs_next = next->wfs_next;
				next->wfs_next = sfb->sfb_free_wfs;
				sfb->sfb_free_wfs = next;
			}
			return;
		}

		// Blocks weren't adjacent - just nearby

		// Check that block to free doesn't overlap existing free block
		fb_assert(position >= space->wfs_position + space->wfs_size);
	}

/* Block didn't seem to append nicely to an existing block */

	if ( (space = sfb->sfb_free_wfs) )
		sfb->sfb_free_wfs = space->wfs_next;
	else
		space = (work_file_space*) sort_alloc(scb, (ULONG) sizeof(work_file_space));
		// FREE: wfs_next chain is freed in local_fini()

	space->wfs_next = *ptr;
	*ptr = space;
	space->wfs_size = size;
	space->wfs_position = position;
}


static sort_record* get_merge(merge_control* merge, sort_context* scb
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
 *      Get next record from a merge tree and/or run_control.
 *
 **************************************/
	SORTP *p;				// no more than 1 SORTP* to a line
	SORTP *q;				// no more than 1 SORTP* to a line
	ULONG l;
#ifdef SCROLLABLE_CURSORS
	ULONG space_available, data_remaining;
#else
	ULONG n;
#endif

	sort_record* record = NULL;
	bool eof = false;

	while (merge) {
		// If node is a run_control, get the next record (or not) and back to parent

		if (merge->mrg_header.rmh_type == TYPE_RUN) {
			run_control* run = (run_control*) merge;
			merge = run->run_header.rmh_parent;

			// check for end-of-file condition in either direction

#ifdef SCROLLABLE_CURSORS
			if (
				(mode == RSE_get_backward
				 && run->run_records >= run->run_max_records - 1)
				|| (mode == RSE_get_forward && run->run_records == 0))
#else
			if (run->run_records == 0)
#endif
			{
				record = (sort_record*) - 1;
				eof = true;
				continue;
			}

			eof = false;

			// Find the appropriate record in the buffer to return

#ifdef SCROLLABLE_CURSORS
			if (mode == RSE_get_forward) {
				run->run_record = NEXT_RUN_RECORD(run->run_record);
#endif
				if ((record = (sort_record*) run->run_record) <
					(sort_record*) run->run_end_buffer)
				{
#ifndef SCROLLABLE_CURSORS
					run->run_record =
						reinterpret_cast<sort_record*>(NEXT_RUN_RECORD(run->run_record));
#endif
					--run->run_records;
					continue;
				}
#ifndef SCROLLABLE_CURSORS
				// There are records remaining, but the buffer is full.
				// Read a buffer full.

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
				if ((record = (sort_record*) run->run_record) >=
					run->run_buffer)
				{
					++run->run_records;
					continue;
				}
			}

			// There are records remaining, but we have stepped over the
			// edge of the cache. Read the next buffer full of records.

			fb_assert((BLOB_PTR *) run->run_end_buffer >
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
				record = reinterpret_cast<sort_record*>(run->run_buffer);
#ifndef SCROLLABLE_CURSORS
				run->run_record =
					reinterpret_cast<sort_record*>(NEXT_RUN_RECORD(record));
#endif
				--run->run_records;
#ifdef SCROLLABLE_CURSORS
			}
			else {
				record = PREV_RUN_RECORD(run->run_end_buffer);
				++run->run_records;
			}

			run->run_record = (sort_record*) record;
#endif
			continue;
		}

		// If've we got a record, somebody asked for it. Find out who.

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


		// If either streams need a record and is still active, loop back to pick
		// up the record. If either stream is dry, return the record of the other.
		// If both are dry, indicate eof for this stream.

		record = NULL;
		eof = false;

		if (!merge->mrg_record_a && merge->mrg_stream_a) {
			merge = (merge_control*) merge->mrg_stream_a;
			continue;
		}

		if (!merge->mrg_record_b)
			if (merge->mrg_stream_b) {
				merge = (merge_control*) merge->mrg_stream_b;
				continue;
			}
			else if ( (record = merge->mrg_record_a) ) {
				merge->mrg_record_a = NULL;
				merge = merge->mrg_header.rmh_parent;
				continue;
			}
			else {
				eof = true;
				record = (sort_record*) - 1;
				merge = merge->mrg_header.rmh_parent;
				continue;
			}

		if (!merge->mrg_record_a) {
			record = merge->mrg_record_b;
			merge->mrg_record_b = NULL;
			merge = merge->mrg_header.rmh_parent;
			continue;
		}

		// We have prospective records from each of the sub-streams. Compare them.
		// If equal, offer each to user routine for possible sacrifice.

		p = merge->mrg_record_a->sort_record_key;
		q = merge->mrg_record_b->sort_record_key;
		//l = scb->scb_key_length;
		l = scb->scb_unique_length;

		DO_32_COMPARE(p, q, l);

		if (l == 0 && scb->scb_dup_callback) {
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) merge->mrg_record_a, scb, false);
			SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, false);
#else
			diddle_key((UCHAR *) merge->mrg_record_a, scb, false);
			diddle_key((UCHAR *) merge->mrg_record_b, scb, false);
#endif
			if ((*scb->scb_dup_callback) ((const UCHAR*) merge->mrg_record_a,
										  (const UCHAR*) merge->mrg_record_b,
										  scb->scb_dup_callback_arg))
			{
				merge->mrg_record_a = NULL;
#ifdef SCROLLABLE_CURSORS
				SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, true);
#else
				diddle_key((UCHAR *) merge->mrg_record_b, scb, true);
#endif
				continue;
			}
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) merge->mrg_record_a, scb, true);
			SORT_diddle_key((UCHAR *) merge->mrg_record_b, scb, true);
#else
			diddle_key((UCHAR *) merge->mrg_record_a, scb, true);
			diddle_key((UCHAR *) merge->mrg_record_b, scb, true);
#endif
		}

		if (l == 0) {
			l = scb->scb_key_length - scb->scb_unique_length;
			if (l != 0)
				DO_32_COMPARE(p, q, l);
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

	// Merge pointer is null; we're done. Return either the most
	// recent record, or end of file, as appropriate.

	return (eof) ? NULL : record;
}


static void init(sort_context* scb)
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

	*scb->scb_next_pointer++ = reinterpret_cast<sort_record*>(low_key);
}


static bool local_fini(sort_context* scb, Attachment* att)
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
	work_file_space* space;
	ULONG** merge_buf;
	sort_context** ptr;

	bool found_it = true;

	if (att) {

		// Cover case where a posted error caused reuse by another thread

		if (scb->scb_attachment != att)
			att = scb->scb_attachment;
		found_it = false;
	}

	// Start by unlinking from que, if present

	if (att)
		for (ptr = &att->att_active_sorts; *ptr; ptr = &(*ptr)->scb_next)
			if (*ptr == scb) {
				*ptr = scb->scb_next;
				found_it = true;
				break;
			}

	// *NO*. I won't free it if it's not in
    // the pointer list that has been passed
    // to me. THIS MEANS MEMORY LEAK.  -- mrs

	if (!found_it)
		return false;

	// Loop through the sfb list and close work files

	sort_work_file* sfb;
	while ( (sfb = scb->scb_sfb) ) {
		scb->scb_sfb = sfb->sfb_next;
		DLS_put_temp_space(sfb);

		delete sfb->sfb_mem;

		close(sfb->sfb_file);

		if (sfb->sfb_file_name) {
			delete[] sfb->sfb_file_name;
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

	// Get rid of extra merge space

	while ( (merge_buf = (ULONG **) scb->scb_merge_space) ) {
		scb->scb_merge_space = *merge_buf;
		gds__free(merge_buf);
	}

	// If runs are allocated and not in the big block, release them.
	// Then release the big block.

	if (scb->scb_memory) {
		gds__free(scb->scb_memory);
		scb->scb_memory = NULL;
	}

	// Clean up the runs that were used

	run_control* run;
	while ( (run = scb->scb_runs) ) {
		scb->scb_runs = run->run_next;
		if (run->run_buff_alloc)
			gds__free(run->run_buffer);
		gds__free(run);
	}

	// Clean up the free runs also

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
 	scb->scb_attachment = NULL;
 	scb->scb_impure = NULL;
 	scb->scb_next = NULL;

	return true;
}


static void merge_runs(sort_context* scb, USHORT n)
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
	USHORT count;
	merge_control blks[32];

	fb_assert((n - 1) <= FB_NELEM(blks));	// stack var big enough?

	scb->scb_longs -= SIZEOF_SR_BCKPTR_IN_LONGS;

	// Make a pass thru the runs allocating buffer space, computing work file
	// space requirements, and filling in a vector of streams with run pointers

	const USHORT rec_size = scb->scb_longs << SHIFTLONG;
	const USHORT buffers = scb->scb_size_memory / rec_size;
	ULONG size = rec_size * (buffers / (USHORT) (2 * n));
	BLOB_PTR* buffer = (BLOB_PTR *) scb->scb_first_pointer;
	run_control temp_run;
	temp_run.run_end_buffer =
		(SORTP *) (buffer + (scb->scb_size_memory / rec_size) * rec_size);
	temp_run.run_size = 0;
	temp_run.run_buff_alloc = 0;

	run_merge_hdr* streams[32];
	run_merge_hdr** m1 = streams;

	run_control* run;

	for (run = scb->scb_runs, count = 0; count < n;
		 run = run->run_next, count++)
	{
		*m1++ = (run_merge_hdr*) run;

		// size = 0 indicates the record is too big to divvy up the
		// big sort buffer, so separate buffers must be allocated

		if (!size) {
			if (!run->run_buff_alloc) {
				run->run_buffer =
					(ULONG *) gds__alloc((SLONG) rec_size * 2);
				// FREE: smb_merge_space freed in local_fini() when sort_context released
				if (!run->run_buffer)
					error_memory(scb);
				run->run_buff_alloc = 1;
			}
			run->run_end_buffer =
				reinterpret_cast<ULONG*>((BLOB_PTR *) run->run_buffer + (rec_size * 2));
			run->run_record =
				reinterpret_cast<sort_record*>(run->run_end_buffer);
		}
		else {
			run->run_buffer = (ULONG *) buffer;
			buffer += size;
			run->run_record =
				reinterpret_cast<sort_record*>(run->run_end_buffer =
												   (ULONG *) buffer);
		}
		temp_run.run_size += run->run_size;
	}
	temp_run.run_record = reinterpret_cast<sort_record*>(buffer);
	temp_run.run_buffer = reinterpret_cast<ULONG*>(temp_run.run_record);

	// Build merge tree bottom up.
	//
	// See also kissing cousin of this loop in SORT_sort()

	merge_control* merge;
	for (count = n, merge = blks; count > 1;) {
		run_merge_hdr** m2 = m1 = streams;
		while (count >= 2) {
			merge->mrg_header.rmh_type = TYPE_MRG;

			fb_assert(((*m1)->rmh_type == TYPE_MRG) ||	// garbage watch
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_a = *m1++;

			fb_assert(((*m1)->rmh_type == TYPE_MRG) ||	// garbage watch
				   ((*m1)->rmh_type == TYPE_RUN));

			(*m1)->rmh_parent = merge;
			merge->mrg_stream_b = *m1++;

			merge->mrg_record_a = NULL;
			merge->mrg_record_b = NULL;
			*m2++ = (run_merge_hdr*) merge;
			merge++;
			count -= 2;
		}
		if (count)
			*m2++ = *m1++;
		count = m2 - streams;
	}

	--merge;
	merge->mrg_header.rmh_parent = NULL;

	// Merge records into run

	sort_record* q = reinterpret_cast<sort_record*>(temp_run.run_buffer);
	ULONG seek = temp_run.run_seek =
		find_file_space(scb, temp_run.run_size, &temp_run.run_sfb);
	temp_run.run_records = 0;

	const sort_record* p;
#ifdef SCROLLABLE_CURSORS
	while (p = get_merge(merge, scb, RSE_get_forward))
#else
	while ( (p = get_merge(merge, scb)) )
#endif
	{
		if (q >= (sort_record*) temp_run.run_end_buffer) {
			size = (BLOB_PTR *) q - (BLOB_PTR *) temp_run.run_buffer;
			seek = temp_run.run_sfb->sfb_mem->write(scb->scb_status_vector, seek,
													reinterpret_cast<char*>(temp_run.run_buffer),
													size);
			q = reinterpret_cast<sort_record*>(temp_run.run_buffer);
		}
		count = scb->scb_longs;
		do {
			*q++ = *p++;
		} while (--count);
		++temp_run.run_records;
	}
#ifdef SCROLLABLE_CURSORS
	temp_run.run_max_records = temp_run.run_records;
#endif

	// Write the tail of the new run and return any unused space

	if ( (size = (BLOB_PTR *) q - (BLOB_PTR *) temp_run.run_buffer) )
		seek = temp_run.run_sfb->sfb_mem->write(scb->scb_status_vector, seek,
												reinterpret_cast<char*>(temp_run.run_buffer),
												size);

	// If the records did not fill the allocated run (such as when duplicates are
	// rejected), then free the remainder and diminish the size of the run accordingly

	if (seek - temp_run.run_seek < temp_run.run_size) {
		free_file_space(scb, temp_run.run_sfb, seek,
						temp_run.run_seek + temp_run.run_size - seek);
		temp_run.run_size = seek - temp_run.run_seek;
	}

	// Make a final pass thru the runs releasing space, blocks, etc.

	for (count = 0; count < n; count++) {
		// Remove run from list of in-use run blocks
		run = scb->scb_runs;
		scb->scb_runs = run->run_next;
#ifdef SCROLLABLE_CURSORS
		seek = run->run_seek + run->run_cached - run->run_size;
#else
		seek = run->run_seek - run->run_size;
#endif
		// Free the sort file space associated with the run

		free_file_space(scb, run->run_sfb, seek, run->run_size);

		// Add run descriptor to list of unused run descriptor blocks

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


static void quick(SLONG size, SORTP** pointers, ULONG length)
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
	SORTP* temp;

#define exchange(x, y)  {temp = x; x = y; y = temp;}

	SORTP** stack_lower[50];
	SORTP*** sl = stack_lower;

	SORTP** stack_upper[50];
	SORTP*** su = stack_upper;

	*sl++ = pointers;
	*su++ = pointers + size - 1;

	while (sl > stack_lower) {

		// Pick up the next interval off the respective stacks

		SORTP** r = *--sl;
		SORTP** j = *--su;

		// Compute the interval. If two or less, defer the sort to a final pass.

		const SLONG interval = j - r;
		if (interval < 2)
			continue;

		// Go guard against pre-ordered data, swap the first record with the
		// middle record. This isn't perfect, but it is cheap.

		SORTP** i = r + interval / 2;
		((SORTP ***) (*r))[-1] = i;
		((SORTP ***) (*i))[-1] = r;
		exchange(*r, *i);

		// Prepare to do the partition. Pick up the first longword of the
		// key to speed up comparisons.

		i = r + 1;
		const ULONG key = **r;

		// From each end of the interval converge to the middle swapping out of
		// parition records as we go. Stop when we converge.

		while (true) {
			while (**i < key)
				i++;
			if (**i == key)
				while (true) {
					const SORTP* p = *i;
					const SORTP* q = *r;
					ULONG tl = length - 1;
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
					const SORTP* p = *j;
					const SORTP* q = *r;
					ULONG tl = length - 1;
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

		// We have formed two partitions, separated by a slot for the
		// initial record "r". Exchange the record currently in the
		// slot with "r".

		((SORTP ***) (*r))[-1] = j;
		((SORTP ***) (*j))[-1] = r;
		exchange(*r, *j);

		// Finally, stack the two intervals, longest first

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


static ULONG order(sort_context* scb)
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
	sort_record** ptr = scb->scb_first_pointer + 1;	// 1st ptr is low key

	// Last inserted record, also the top of the memory where SORT_RECORDS can
	// be written
	sort_record* output = reinterpret_cast<sort_record*>(scb->scb_last_record);
	sort_ptr_t* lower_limit = reinterpret_cast<sort_ptr_t*>(output);

	ULONG temp[1024];
	SORTP* buffer = 0;
	if ((scb->scb_longs * sizeof(ULONG)) > sizeof(temp))
		buffer =
			(ULONG *) gds__alloc((SLONG) (scb->scb_longs * sizeof(ULONG)));
		// FREE: buffer is freed later in this routine
	else
		buffer = temp;

	if (!buffer)
		error_memory(scb);

	// Check out the engine

	THREAD_EXIT();

	// Length of the key part of the record
	const SSHORT length = scb->scb_longs - SIZEOF_SR_BCKPTR_IN_LONGS;

	// scb_next_pointer points to the end of pointer memory or the beginning of
	// records
	while (ptr < scb->scb_next_pointer) {
		// If the next pointer is null, it's record has been eliminated as a
		// duplicate. This is the only easy case.

		SR* record = reinterpret_cast<SR*>(*ptr++);
		if (!record)
			continue;

		// Make record point back to the starting of SR struct,
		// as all scb* pointer point to the key_id locations!
		record =
			reinterpret_cast<SR*>(((SORTP *) record) - SIZEOF_SR_BCKPTR_IN_LONGS);

		// If the lower limit of live records points to a deleted or used record,
		// advance the lower limit

		while (!*(lower_limit)
			   && (lower_limit < (sort_ptr_t*) scb->scb_end_memory))
		{
			lower_limit =
				reinterpret_cast<sort_ptr_t*>(((SORTP *) lower_limit) + scb->scb_longs);
		}

		// If the record we want to move won't interfere with lower active
		// record, just move the record into position

		if (record->sr_sort_record.sort_record_key == (ULONG *) lower_limit) {
			MOVE_32(length, record->sr_sort_record.sort_record_key, output);
			output =
				reinterpret_cast<sort_record*>((SORTP *) output + length);
			continue;
		}

		if (((SORTP *) output) + scb->scb_longs - 1 <= (SORTP *) lower_limit) {
			// null the bckptr for this record
			record->sr_bckptr = NULL;
			MOVE_32(length, record->sr_sort_record.sort_record_key, output);
			output =
				reinterpret_cast<sort_record*>((SORTP *) output + length);
			continue;
		}

		// There's another record sitting where we want to put our record. Move
		// the next logical record to a temp, move the lower limit record to the
		// next record's old position (adjusting pointers as we go), then move
		// the current record to output.

		MOVE_32(length, (SORTP *) record->sr_sort_record.sort_record_key,
				buffer);

		**((sort_ptr_t***) lower_limit) =
			reinterpret_cast<sort_ptr_t*>(record->sr_sort_record.sort_record_key);
		MOVE_32(scb->scb_longs, lower_limit, record);
		lower_limit = (sort_ptr_t*) ((SORTP *) lower_limit + scb->scb_longs);

		MOVE_32(length, buffer, output);
		output =
			reinterpret_cast<sort_record*>((sort_ptr_t*) ((SORTP *) output + length));
	}

	// Check back into the engine

	THREAD_ENTER();

	// It's OK to free this after checking back into the engine, there's
	// only fatal failures possible there

	if (buffer != temp)
		if (buffer != NULL)
			gds__free(buffer);

	return (((SORTP *) output) -
			((SORTP *) scb->scb_last_record)) / (scb->scb_longs -
												 SIZEOF_SR_BCKPTR_IN_LONGS);
}


static void put_run(sort_context* scb)
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
	run_control* run = scb->scb_free_runs;

	if (run)
		scb->scb_free_runs = run->run_next;
	else {
		run = (run_control*) sort_alloc(scb, (ULONG) sizeof(run_control));
		// FREE: run will be either on the scb_runs or scb_free_runs list,
		//       which are freed in local_fini()
	}

	run->run_next = scb->scb_runs;
	scb->scb_runs = run;
	run->run_header.rmh_type = TYPE_RUN;
	run->run_depth = 0;

	// Do the in-core sort. The first phase a duplicate handling we be performed
	// in "sort".

	sort(scb);

	// Re-arrange records in physical order so they can be dumped in a single write
	// operation

#ifdef SCROLLABLE_CURSORS
	run->run_records = run->run_max_records = order(scb);
	run->run_cached = 0;
#else
	run->run_records = order(scb);
#endif

	// Write records to scratch file. Keep track of the number of bytes
	// written, etc.

	run->run_size =
		run->run_records * (scb->scb_longs -
							SIZEOF_SR_BCKPTR_IN_LONGS) * sizeof(ULONG);
	run->run_seek = find_file_space(scb, run->run_size, &run->run_sfb);
	run->run_sfb->sfb_mem->write(scb->scb_status_vector, run->run_seek,
								 reinterpret_cast<char*>(scb->scb_last_record),
								 run->run_size);
}


static void sort(sort_context* scb)
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
	SORTP* temp;

	// Check out the engine

	THREAD_EXIT();

	// First, insert a pointer to the high key

	*scb->scb_next_pointer = reinterpret_cast<sort_record*>(high_key);

	// Next, call QuickSort. Keep in mind that the first pointer is the
	// low key and not a record.

	SORTP** j = (SORTP **) (scb->scb_first_pointer) + 1;
	const ULONG n = (SORTP **) (scb->scb_next_pointer) - j;	// calculate # of records

	quick(n, j, scb->scb_longs);

	// Scream through and correct any out of order pairs

	while (j < (SORTP **) scb->scb_next_pointer) {
		SORTP** i = j;
		j++;
		if (**i >= **j) {
			const SORTP* p = *i;
			const SORTP* q = *j;
			ULONG tl = scb->scb_longs - 1;
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

	// If duplicate handling hasn't been requested, we're done

	if (!scb->scb_dup_callback) {
		// Check back into the engine
		THREAD_ENTER();
		return;
	}

	// Make another pass and eliminate duplicates. It's possible to do this
	// is the same pass the final ordering, but the logic is complicated enough
	// to screw up register optimizations. Better two fast passes than one
	// slow pass, I suppose. Prove me wrong and win a trip for two to
	// Cleveland, Ohio.

	j = reinterpret_cast<SORTP**>(scb->scb_first_pointer + 1);

	while (j < (SORTP **) scb->scb_next_pointer) {
		SORTP** i = j;
		j++;
		if (**i != **j)
			continue;
		const SORTP* p = *i;
		const SORTP* q = *j;

		ULONG l = scb->scb_unique_length;
		DO_32_COMPARE(p, q, l);
		if (l == 0) {
#ifdef SCROLLABLE_CURSORS
			SORT_diddle_key((UCHAR *) * i, scb, false);
			SORT_diddle_key((UCHAR *) * j, scb, false);
#else
			diddle_key((UCHAR *) * i, scb, false);
			diddle_key((UCHAR *) * j, scb, false);
#endif
			if ((*scb->scb_dup_callback) ((const UCHAR*) *i, (const UCHAR*) *j, scb->scb_dup_callback_arg))
			{
				((SORTP ***) (*i))[-1] = NULL;
				*i = NULL;
			}
			else
#ifdef SCROLLABLE_CURSORS
				SORT_diddle_key((UCHAR *) * i, scb, true);
			SORT_diddle_key((UCHAR *) * j, scb, true);
#else
				diddle_key((UCHAR *) * i, scb, true);
			diddle_key((UCHAR *) * j, scb, true);
#endif
		}
	}

	// Check back into the engine

	THREAD_ENTER();
}

#ifdef NOT_USED_OR_REPLACED
#ifdef DEBUG
static void validate(sort_context* scb)
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
	for (SORTP** ptr = (SORTP **) (scb->scb_first_pointer + 1);
		 ptr < (SORTP **) scb->scb_next_pointer; ptr++)
	{
		SORTP* record = *ptr;
		if (record[-1] != (SORTP) ptr) {
			ISC_STATUS* status_vector = scb->scb_status_vector;
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_crrp_data_err; // Msg360: corruption in data structure
			*status_vector = isc_arg_end;
			ERR_punt();
		}
	}
}
#endif
#endif

#ifdef DEBUG_SORT_TRACE
static void write_trace(
						UCHAR* operation,
						sort_work_file* sfb, ULONG seek, BLOB_PTR* address,
						ULONG length)
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
	UCHAR file_name[40];

	if (!trace_file) {
#if (defined WIN_NT)
		strcpy(file_name, "/interbas/stXXXXXX");
#else
		strcpy(file_name, "/interbase/DEBUG_SORT_TRACE_XXXXXX");
#endif
#ifdef HAVE_MKSTEMP
		const int fd = mkstemp(file_name);
		trace_file = fdopen(fd, "w");
#else
		mktemp(file_name);
		trace_file = fopen(file_name, "w");
#endif
	}

	if (!trace_file)
		return;

	UCHAR data[41];
	UCHAR* p;
	for (p = data; p < data + sizeof(data) - 1; address++)
		*p++ = (*address) ? *address : '.';

	*p = 0;

	fprintf(trace_file, "Fid: %d, %.5s %.7ld - %.7ld\t/%s/\n",
			   sfb->sfb_file, operation, seek, seek + length, data);
}
#endif

