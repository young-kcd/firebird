/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blf.epp
 *	DESCRIPTION:	Blob filter driver
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

/* Note: This file is no longer an e file since it does not
 * contain any more code for gpre to pre-process.  However,
 * since we will soon move to a new build system, I do not 
 * want to change the make files at this time, which is what
 * would be required to change blf.e to blf.c.  So we will continue
 * to pre-process it, and nothing will happen.
 *
 * Marco Romanini (27-January-1999) */

#include "firebird.h"
#include <string.h>
#include <stdio.h>

#include "../jrd/jrd.h"
#include "../jrd/gds.h"
#include "../jrd/blob_filter.h"
#include "../jrd/tra.h"
#include "../jrd/constants.h"
#include "../jrd/gdsassert.h"
#include "../jrd/blf_proto.h"
#include "../jrd/filte_proto.h"
#include "../jrd/flu_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/inf_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/common.h"
#include "../jrd/ibsetjmp.h"
#include "../jrd/thd_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/all_proto.h"
#include "gen/codes.h"

/* System provided internal filters for filtering internal
 * subtypes to text.
 * (from_type in [0..8], to_type == BLOB_text)
 */
static PTR filters[] = {
	filter_text,
	filter_transliterate_text,
	filter_blr,
	filter_acl,
	0,							/* filter_ranges, */
	filter_runtime,
	filter_format,
	filter_trans,
	filter_trans				/* should be filter_external_file */
};


extern "C" {

static ISC_STATUS open_blob(TDBB,
							JRD_TRA,
							CTL*,
							SLONG*,
							USHORT,
							UCHAR*,
							PTR,
							USHORT,
							BLF);


ISC_STATUS BLF_close_blob(TDBB tdbb, CTL * filter_handle)
{
/**************************************
 *
 *	B L F _ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Close a blob and close all the intermediate filters.
 *
 **************************************/
	CTL control, next;
	PTR callback;
	ISC_STATUS status;
	ISC_STATUS *user_status;

	user_status = tdbb->tdbb_status_vector;

/* Walk the chain of filters to find the ultimate source */

	for (next = *filter_handle; next->ctl_to_sub_type;
		 next = next->ctl_source_handle);

	callback = next->ctl_source;
	status = FB_SUCCESS;

	START_CHECK_FOR_EXCEPTIONS( (TEXT*) next->ctl_exception_message)

/* Sign off from filter */
/* Walk the chain again, telling each filter stage to close */
		for (next = *filter_handle; (control = next);) {
		/* Close this stage of the filter */

		control->ctl_status = user_status;
		(*control->ctl_source) (ACTION_close, control);

		/* Find the next stage */

		next = control->ctl_source_handle;
		if (!control->ctl_to_sub_type)
			next = NULL;

		/* Free this stage's control block allocated by calling the
		   final stage with an ACTION_alloc, which is why we call
		   the final stage with ACTION_free here. */

		(*callback) (ACTION_free, control);
	}

	END_CHECK_FOR_EXCEPTIONS((TEXT*)next->ctl_exception_message)

		return 0;
}


ISC_STATUS BLF_create_blob(TDBB tdbb,
							JRD_TRA tra_handle,
							CTL * filter_handle,
							SLONG * blob_id,
							USHORT bpb_length,
							UCHAR * bpb,
							ISC_STATUS (*callback)(),
							BLF filter)
{
/**************************************
 *
 *	B L F _ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open a blob and invoke a filter.
 *
 **************************************/

	return open_blob(tdbb, tra_handle, filter_handle,
					 blob_id, bpb_length, bpb, (ISC_STATUS (*)(USHORT, CTL)) callback, ACTION_create,
					 filter);
}


ISC_STATUS BLF_get_segment(TDBB tdbb,
							CTL * filter_handle,
							USHORT * length,
							USHORT buffer_length,
							UCHAR * buffer)
{
/**************************************
 *
 *	B L F _ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get segment from a blob filter.
 *
 **************************************/
	CTL control;
	ISC_STATUS status;
	ISC_STATUS *user_status;

	user_status = tdbb->tdbb_status_vector;

	control = *filter_handle;
	control->ctl_status = user_status;
	control->ctl_buffer = buffer;
	control->ctl_buffer_length = buffer_length;

	START_CHECK_FOR_EXCEPTIONS((TEXT*) control->ctl_exception_message)

		user_status[0] = gds_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = gds_arg_end;

	status = (*control->ctl_source) (ACTION_get_segment, control);

	if (!status || status == gds_segment)
		*length = control->ctl_segment_length;
	else
		*length = 0;

	if (status != user_status[1]) {
		user_status[1] = status;
		user_status[2] = gds_arg_end;
	}

	END_CHECK_FOR_EXCEPTIONS((TEXT*)control->ctl_exception_message)

		return status;
}


BLF BLF_lookup_internal_filter(TDBB tdbb, SSHORT from, SSHORT to)
{
/**************************************
 *
 *	B L F _ l o o k u p _ i n t e r n a l _ f i l t e r
 *
 **************************************
 *
 * Functional description
 *	Lookup blob filter in data structures.
 *
 **************************************/
	BLF result;
	STR exception_msg;
	DBB dbb;

	dbb = tdbb->tdbb_database;

/* Check for system defined filter */

	if (to == BLOB_text && from >= 0 && from < FB_NELEM(filters)) {
		result = FB_NEW(*dbb->dbb_permanent) blf;
		result->blf_next = NULL;
		result->blf_from = from;
		result->blf_to = to;
		result->blf_filter = filters[from];
		exception_msg = FB_NEW_RPT(*dbb->dbb_permanent, 100) str;
		// SIGN ISSUE, arg 1
		sprintf((char*)exception_msg->str_data,
				"Exception occurred in system provided internal filters for filtering internal subtype %d to text.",
				from);
		result->blf_exception_message = exception_msg;
		return result;
	}

	return NULL;
}


ISC_STATUS BLF_open_blob(TDBB tdbb,
						JRD_TRA tra_handle,
						CTL * filter_handle,
						SLONG * blob_id,
						USHORT bpb_length,
						UCHAR * bpb,
						ISC_STATUS (*callback)(),
						BLF filter)
{
/**************************************
 *
 *	B L F _ o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open a blob and invoke a filter.
 *
 **************************************/

	return open_blob(tdbb, tra_handle, filter_handle,
					 blob_id, bpb_length, bpb, (ISC_STATUS (*)(USHORT, CTL)) callback, ACTION_open, filter);
}


ISC_STATUS BLF_put_segment(TDBB tdbb,
							CTL * filter_handle,
							USHORT length,
							UCHAR * buffer)
{
/**************************************
 *
 *	B L F _ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get segment from a blob filter.
 *
 **************************************/
	CTL control;
	ISC_STATUS status;
	ISC_STATUS *user_status;

	user_status = tdbb->tdbb_status_vector;

	control = *filter_handle;
	control->ctl_status = user_status;
	control->ctl_buffer = buffer;
	control->ctl_buffer_length = length;

	START_CHECK_FOR_EXCEPTIONS( (TEXT*) control->ctl_exception_message)

		user_status[0] = gds_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = gds_arg_end;

	status = (*control->ctl_source) (ACTION_put_segment, control);

	if (status != user_status[1]) {
		user_status[1] = status;
		user_status[2] = gds_arg_end;
	}

	END_CHECK_FOR_EXCEPTIONS((TEXT*)control->ctl_exception_message)

		return status;
}


static ISC_STATUS open_blob(
					TDBB tdbb,
					JRD_TRA tra_handle,
					CTL * filter_handle,
					SLONG * blob_id,
					USHORT bpb_length,
					UCHAR * bpb,
					PTR callback,
					USHORT action,
					BLF filter)
{
/**************************************
 *
 *	o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open a blob and invoke a filter.
 *
 **************************************/
	ISC_STATUS status;
	CTL prior, control;
	SSHORT from, to;
	USHORT from_charset, to_charset;
	ctl temp;
	DBB dbb;
	ISC_STATUS *user_status;

	dbb = tdbb->tdbb_database;
	user_status = tdbb->tdbb_status_vector;

	gds__parse_bpb2(bpb_length, bpb, &from, &to, &from_charset, &to_charset);

	if ((!filter) || (!filter->blf_filter)) {
		*user_status++ = gds_arg_gds;
		*user_status++ = gds_nofilter;
		*user_status++ = gds_arg_number;
		*user_status++ = (ISC_STATUS) from;
		*user_status++ = gds_arg_number;
		*user_status++ = (ISC_STATUS) to;
		*user_status = gds_arg_end;
		return gds_nofilter;
	}

/* Allocate a filter control block and open blob */

	user_status[0] = gds_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = gds_arg_end;

/* utilize a temporary control block just to pass the three 
   necessary internal parameters to the filter */

	temp.ctl_internal[0] = dbb;
	temp.ctl_internal[1] = tra_handle;
	temp.ctl_internal[2] = NULL;
	prior = (CTL) (*callback) (ACTION_alloc, &temp);
	prior->ctl_source = callback;
	prior->ctl_status = user_status;

	prior->ctl_internal[0] = dbb;
	prior->ctl_internal[1] = tra_handle;
	prior->ctl_internal[2] = blob_id;
	if ((*callback) (action, prior)) {
		BLF_close_blob(tdbb, &prior);
		return user_status[1];
	}

	control = (CTL) (*callback) (ACTION_alloc, &temp);
	control->ctl_source = filter->blf_filter;
	control->ctl_source_handle = prior;
	control->ctl_status = user_status;
	control->ctl_exception_message = filter->blf_exception_message->str_data;

/* Two types of filtering can be occuring; either between totally
 * different BLOb sub_types, or between two different
 * character sets (both source & destination subtype must be TEXT
 * in that case).
 * For the character set filter we use the to & from_sub_type fields
 * to tell the filter what character sets to translate between.
 */

	if (filter->blf_filter == (PTR) filter_transliterate_text) {
		assert(to == BLOB_text);
		assert(from == BLOB_text);
		control->ctl_to_sub_type = to_charset;
		control->ctl_from_sub_type = from_charset;
	}
	else {
		control->ctl_to_sub_type = to;
		control->ctl_from_sub_type = from;
	}
	control->ctl_bpb = bpb;
	control->ctl_bpb_length = bpb_length;

	START_CHECK_FOR_EXCEPTIONS( (TEXT*) control->ctl_exception_message)

/* Initialize filter */
		status = (*filter->blf_filter) (action, control);

	END_CHECK_FOR_EXCEPTIONS((TEXT*)control->ctl_exception_message)

		if (status) {
		ISC_STATUS_ARRAY local_status;
		ISC_STATUS *tmp_status;
		tmp_status = tdbb->tdbb_status_vector;
		tdbb->tdbb_status_vector = local_status;
		/* This is OK to do since we know that we will return
		 * from  BLF_close_blob, and get a chance to set the 
		 * status vector back to something valid.  The only
		 * place that BLF_close_blob can go, other than return
		 * is into the exception handling code, where it will 
		 * abort, so no problems there. */
		BLF_close_blob(tdbb, &control);
		tdbb->tdbb_status_vector = tmp_status;
	}
	else
		*filter_handle = control;

	if (status != user_status[1]) {
		user_status[1] = status;
		user_status[2] = gds_arg_end;
	}

	return status;
}

}	// extern "C"
