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
#include "../common/common.h"
#include <stdio.h>
#include <string.h>

#include "../dsql/dsql.h"
#include "gen/iberror.h"
#include "../jrd/jrd.h"
#include "../jrd/cvt_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/movd_proto.h"
#include "../common/cvt.h"

using namespace Jrd;
using namespace Firebird;

static void post_error(const Arg::StatusVector&);


static EngineCallbacks dsqlCallbacks(post_error);


/**

 	MOVD_move

    @brief	Move (and possible convert) something to something else.


    @param from
    @param to

 **/
void MOVD_move(const dsc* from, dsc* to)
{
	CVT_move_common(from, to, &dsqlCallbacks);
}


/**

 	post_error

    @brief	A conversion error occurred.  Complain.


    @param status
    @param

 **/
static void post_error(const Arg::StatusVector& v)
{
	Arg::Gds status_vector(isc_dsql_error);
	status_vector << Arg::Gds(isc_sqlerr) << Arg::Num(-303);
	status_vector.append(v);

	ERRD_punt(status_vector.value());
}
