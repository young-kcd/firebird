/*
*	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		movd.c
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
#include "../jrd/ib_stdio.h"
#include <stdarg.h>
#include <string.h>

#include "../dsql/dsql.h"
#include "gen/codes.h"
#include "../jrd/iberr.h"
#include "../dsql/errd_proto.h"
#include "../dsql/movd_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/thd_proto.h"

static void post_error(ISC_STATUS, ...);


/**
  
 	MOVD_move
  
    @brief	Move (and possible convert) something to something else.
 

    @param from
    @param to

 **/
void MOVD_move( DSC * from, DSC * to)
{

	CVT_move(from, to, (FPTR_VOID) post_error);
}


/**
  
 	post_error
  
    @brief	A conversion error occurred.  Complain.
 

    @param status
    @param 

 **/
static void post_error( ISC_STATUS status, ...)
{
	TSQL tdsql;
	ISC_STATUS *v, *v_end, *temp, temp_status[ISC_STATUS_LENGTH];

	tdsql = GET_THREAD_DATA;

/* copy into a temporary array any other arguments which may 
 * have been handed to us, then post the error.
 * N.B., the last supplied error should be a 0.
 */

	STUFF_STATUS(temp_status, status);

	v = tdsql->tsql_status;
	v_end = v + 20;
	*v++ = gds_arg_gds;
	*v++ = gds_dsql_error;
	*v++ = gds_arg_gds;
	*v++ = gds_sqlerr;
	*v++ = gds_arg_number;
	*v++ = -303;

	for (temp = temp_status; v < v_end && (*v = *temp) != gds_arg_end;
		 v++, temp++)
		switch (*v) {
		case gds_arg_cstring:
			*++v = *++temp;
			*++v = *++temp;
			break;
		default:
			*++v = *++temp;
			break;
		};

	ERRD_punt();
}
