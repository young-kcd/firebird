/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sch.cpp
 *	DESCRIPTION:	Voluntary thread scheduler
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
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include "../jrd/ThreadStart.h"
#include "../jrd/isc.h"
#include "../jrd/ibase.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/os/thd_priority.h"
#include "../common/classes/locks.h"
#include "../common/classes/init.h"

#ifdef	WIN_NT
#include <windows.h>
#endif


static FB_THREAD_ID thread_id = 0;
static Firebird::GlobalPtr<Firebird::Mutex> thread_mutex;


int API_ROUTINE gds__thread_enable(int enable_flag)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Check-in with thread traffic cop.
 *
 **************************************/

	return true;
}


void API_ROUTINE gds__thread_enter()
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	Check-in with thread traffic cop.
 *
 **************************************/

	thread_mutex->enter();
	fb_assert(!thread_id);
	thread_id = getThreadId();
}


void API_ROUTINE gds__thread_exit()
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Check-out with thread traffic cop.
 *
 **************************************/

	fb_assert(thread_id == getThreadId());
	thread_id = 0;
	thread_mutex->leave();
}
