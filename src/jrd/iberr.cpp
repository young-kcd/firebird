/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		iberr.c
 *	DESCRIPTION:	Generic Interbase error handling routines.
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

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "gen/iberror.h"
#include "../jrd/iberr.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"

#ifdef NOT_USED_OR_REPLACED
static void post_error(ISC_STATUS*, const SCHAR*, const bool, ISC_STATUS, ...);
#endif


void IBERR_append_status(ISC_STATUS* status_vector, ISC_STATUS status, ...)
{
/**************************************
 *
 *	I B E R R _  a p p e n d _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Append the given status vector with the passed arguments.
 *
 **************************************/
	ISC_STATUS_ARRAY local_status;

	// First build a local status vector with the passed arguments

	STUFF_STATUS(local_status, status);

	// Now find the end of the passed status vector info

	ISC_STATUS* const status_end = status_vector + ISC_STATUS_LENGTH;
	while (*status_vector && status_vector < status_end)
		++status_vector;

	// If we fail here, an unterminated, illegal status vector was passed as argument.
	fb_assert(status_vector < status_end);

	// Now append the newly built local status vector to the passed one
	// The minimum space is for appending isc_arg_<something>, <value>, isc_arg_end.

	if (status_vector < status_end - 2)
	{
		const ISC_STATUS* appended = local_status;
		while (status_vector < status_end && (*status_vector++ = *appended++))
			; // empty loop
			
		status_end[-1] = 0; // force termination
	}
}


void IBERR_build_status(ISC_STATUS* status_vector, ISC_STATUS status, ...)
{
/**************************************
 *
 *	I B E R R _ b u i l d _ s t a t u s 
 *
 **************************************
 *
 * Functional description
 *	Fill up a status vector with the passed arguments.
 *
 **************************************/

	STUFF_STATUS(status_vector, status);
}


#ifdef NOT_USED_OR_REPLACED
void IBERR_bugcheck(
					ISC_STATUS* status_vector,
					const SCHAR* dbname,
					const bool raise, int number, TEXT* errmsg)
{
/**************************************
 *
 *	I B E R R _ b u g c h e c k
 *
 **************************************
 *
 * Functional description
 *	Things seem to be going poorly today.
 *
 **************************************/
	USHORT flags;

	if (gds__msg_lookup(0, JRD_BUGCHK, number, MAX_ERRMSG_LEN, errmsg,
						&flags) < 1)
		strcpy(errmsg, "Internal error code");

	char strnum[20];
	const size_t len1 = strlen(errmsg);
	const size_t len2 = sprintf(strnum, " (%d)", number);
	if (len1 + len2 < MAX_ERRMSG_LEN)
		strncpy(errmsg + len1, strnum, len2 + 1);

	post_error(status_vector, dbname, raise, isc_bug_check,
			   isc_arg_string, errmsg, 0);
}


void IBERR_error(
				 ISC_STATUS* status_vector,
				 const SCHAR* dbname,
				 const bool raise, int number, TEXT* errmsg)
{
/**************************************
 *
 *	I B E R R _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Post a user-level error.  This is a temporary mechanism
 *	that will eventually disappear.
 *
 **************************************/
	USHORT flags;

	if (gds__msg_lookup(0, JRD_BUGCHK, number, MAX_ERRMSG_LEN, errmsg,
						&flags) < 1)
		sprintf(errmsg, "error code %d", number);

	post_error(status_vector, dbname, raise, isc_random,
			   isc_arg_string, errmsg, 0);
}


static void post_error(
					   ISC_STATUS* status_vector,
					   const SCHAR* dbname,
					   const bool raise, ISC_STATUS status, ...)
{
/**************************************
 *
 *	p o s t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Create a status vector and return to the user.
 *
 **************************************/

	STUFF_STATUS(status_vector, status);

	if (status_vector[1] == isc_db_corrupt ||
		status_vector[1] == isc_bug_check)
	{
			gds__log_status(dbname, status_vector);
	}

	if (raise) {
		Firebird::status_exception::raise(status_vector);
	}
}
#endif

