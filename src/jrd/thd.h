/*
 *	PROGRAM:	JRD access method
 *	MODULE:		thd.h
 *	DESCRIPTION:	Thread support definitions
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
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * Alex Peshkov
 */

#ifndef JRD_THD_H
#define JRD_THD_H

#include "firebird.h"
#include "../jrd/isc.h"
#include "../common/classes/locks.h"
#include "../common/classes/rwlock.h"

// thread run-ability control
void	THD_sleep(ULONG);
void	THD_yield();

// thread ID
FB_THREAD_ID getThreadId();


/* Thread quanta */

const int QUANTUM			= 100;	/* Default quantum */
const int SWEEP_QUANTUM		= 10;	/* Make sweeps less disruptive */


/* Recursive mutex structure */
struct rec_mutx_t {
	Firebird::Mutex rec_mutx_mtx;
	FB_THREAD_ID rec_mutx_id;
	SLONG rec_mutx_count;

	rec_mutx_t()
		: rec_mutx_id(0), rec_mutx_count(0)
	{}
};

typedef rec_mutx_t REC_MUTX_T;

int THD_rec_mutex_lock(struct rec_mutx_t*);
int THD_rec_mutex_unlock(struct rec_mutx_t*);

#endif // JRD_THD_H
