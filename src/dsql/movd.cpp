/*
*	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		movd.cpp
 *	DESCRIPTION:	Data mover and converter and comparator, etc.
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
#include "../jrd/common.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "../dsql/dsql.h"
#include "gen/iberror.h"
#include "../jrd/iberr.h"
#include "../dsql/errd_proto.h"
#include "../dsql/movd_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/thd.h"

static void post_error(ISC_STATUS, ...);


/**
  
 	MOVD_move
  
    @brief	Move (and possible convert) something to something else.
 

    @param from
    @param to

 **/
void MOVD_move(const dsc* from, dsc* to)
{

	CVT_move(from, to, post_error);
}


/**
  
 	post_error
  
    @brief	A conversion error occurred.  Complain.
 

    @param status
    @param 

 **/
static void post_error( ISC_STATUS status, ...)
{
	ISC_STATUS *v;
	const ISC_STATUS* temp, *v_end;
	ISC_STATUS_ARRAY temp_status;

	tsql* tdsql = DSQL_get_thread_data();

/* copy into a temporary array any other arguments which may 
 * have been handed to us, then post the error.
 * N.B., the last supplied error should be a 0.
 */

	STUFF_STATUS(temp_status, status);

	v = tdsql->tsql_status;
	v_end = v + ISC_STATUS_LENGTH;
	*v++ = isc_arg_gds;
	*v++ = isc_dsql_error;
	*v++ = isc_arg_gds;
	*v++ = isc_sqlerr;
	*v++ = isc_arg_number;
	*v++ = -303;

	for (temp = temp_status; v < v_end && (*v = *temp) != isc_arg_end;
		 v++, temp++)
	{
		switch (*v) {
		case isc_arg_cstring:
			*++v = *++temp;
			*++v = *++temp;
			break;
		default:
			*++v = *++temp;
			break;
		}
	}

	ERRD_punt();
}

