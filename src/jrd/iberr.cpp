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
#include "../jrd/ib_stdio.h"
#include "../jrd/ibsetjmp.h"
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "gen/codes.h"
#include "../jrd/iberr.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"

static void post_error(ISC_STATUS *, SCHAR *, UCHAR *, ISC_STATUS, ...);


void IBERR_append_status(ISC_STATUS * status_vector, ISC_STATUS status, ...)
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
	ISC_STATUS *appended, *status_end;
	ISC_STATUS_ARRAY local_status;

/* First build a local status vector with the passed arguments */

	STUFF_STATUS(local_status, status);

/* Now find the end of the passed status vector info */

	for (status_end = status_vector + ISC_STATUS_LENGTH; *status_vector; status_vector++);

/* Now append the newly built local status vector to the passed one */

	appended = local_status;
	while ((*status_vector++ = *appended++) && status_vector < status_end);
}


void IBERR_build_status(ISC_STATUS * status_vector, ISC_STATUS status, ...)
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


void IBERR_bugcheck(
					ISC_STATUS * status_vector,
					SCHAR * dbname,
					UCHAR * longjmp_addr, int number, TEXT * errmsg)
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
	USHORT flags, len;

	if (gds__msg_lookup(0, JRD_BUGCHK, number, MAX_ERRMSG_LEN, errmsg,
						&flags) < 1)
		strcpy(errmsg, "Internal error code");

	len = strlen(errmsg);
	sprintf(errmsg + len, " (%d)", number);

	post_error(status_vector, dbname, longjmp_addr, gds_bug_check,
			   gds_arg_string, errmsg, 0);
}


void IBERR_error(
				 ISC_STATUS * status_vector,
				 SCHAR * dbname,
				 UCHAR * longjmp_addr, int number, TEXT * errmsg)
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

	post_error(status_vector, dbname, longjmp_addr, gds_random,
			   gds_arg_string, errmsg, 0);
}


static void post_error(
					   ISC_STATUS * status_vector,
					   SCHAR * dbname,
					   UCHAR * longjmp_addr, ISC_STATUS status, ...)
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

	if (status_vector[1] == gds_db_corrupt ||
		status_vector[1] == gds_bug_check)
	{
			gds__log_status(dbname, status_vector);
	}

	if (longjmp_addr) {
		Firebird::status_exception::raise(status_vector[1]);
	}
}
