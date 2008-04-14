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
 */

#ifndef JRD_THD_H
#define JRD_THD_H

#include "firebird.h"
#include "../jrd/isc.h"
#include "../common/classes/locks.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/alloc.h"

#ifdef MULTI_THREAD
//
//inline void THD_mutex_lock(Firebird::Mutex* m) {
//	m->enter();
//}
//
// THD_mutex_unlock is kept here to preserve INUSE_cleanup()
// logic
//
inline void THD_mutex_unlock(Firebird::Mutex* m) {
	m->leave();
}

extern	Firebird::Mutex global_mutex;
inline void	THD_mutex_lock_global(void) {
	global_mutex.enter();
}
inline void THD_mutex_unlock_global(void) {
	global_mutex.leave();
}
#endif // MULTI_THREAD

// recursive mutex
#ifdef SUPERSERVER
int		THD_rec_mutex_destroy(struct rec_mutx_t*);
int		THD_rec_mutex_init(struct rec_mutx_t*);
int		THD_rec_mutex_lock(struct rec_mutx_t*);
int		THD_rec_mutex_unlock(struct rec_mutx_t*);
#endif

// thread run-ability control
int		THD_resume(THD_T);
void	THD_sleep(ULONG);
int		THD_suspend(THD_T);
void	THD_yield(void);

/* Thread priorities (may be ignored) */

const int THREAD_high			= 1;
const int THREAD_medium_high	= 2;
const int THREAD_medium			= 3;
const int THREAD_medium_low		= 4;
const int THREAD_low			= 5;
const int THREAD_critical		= 6;

/* Thread option flags */

const int THREAD_ast		= 1;	/* Thread can/should run at ast level */
const int THREAD_blast		= 2;	/* Blow away thread during exit handler */
const int THREAD_wait		= 4;	/* Somebody will wait for thread exit */

/* Thread quanta */

const int QUANTUM			= 100;	/* Default quantum */
const int SWEEP_QUANTUM		= 10;	/* Make sweeps less disruptive */


/* Thread specific data */

#if defined(WIN_NT)
#define THREAD_ENTRY_PARAM void*
#define THREAD_ENTRY_RETURN unsigned int
#define THREAD_ENTRY_CALL __stdcall
#elif defined(USE_POSIX_THREADS)
#define THREAD_ENTRY_PARAM void*
#define THREAD_ENTRY_RETURN void*
#define THREAD_ENTRY_CALL
#elif defined(SOLARIS_MT)
#define THREAD_ENTRY_PARAM void*
#define THREAD_ENTRY_RETURN void *
#define THREAD_ENTRY_CALL
#else
// Define correct types for other platforms
#define THREAD_ENTRY_PARAM void*
#define THREAD_ENTRY_RETURN int
#define THREAD_ENTRY_CALL
#endif

#define THREAD_ENTRY_DECLARE THREAD_ENTRY_RETURN THREAD_ENTRY_CALL

// BRS 01/07/2004
// Hack due to a bug in mingw.
// The definition inside the thdd class should be replaced with the following one.
typedef THREAD_ENTRY_DECLARE ThreadEntryPoint(THREAD_ENTRY_PARAM);

class ThreadData
{
public:
	enum ThreadDataType {
		tddGBL = 1,		// used by backup/restore
		tddSQL = 2,		// used by DSQL
		tddDBB = 3,		// used in engine
		tddRDB = 4,		// used in remote interface
		tddDBA = 5,		// used in DBA utility
		tddIDB = 6,		// used by interprocess server // ??
		tddALICE = 7,	// used by gfix
		tddSEC = 8		// used by gsec
	};
private:
	ThreadData*		threadDataPriorContext;
	ThreadDataType	threadDataType;	// what kind of thread context this is

public:
	ThreadData(ThreadDataType t)
		: threadDataPriorContext(0), threadDataType(t) {}
	ThreadDataType getType() const
	{
		return threadDataType;
	}

	static void		start(ThreadEntryPoint* routine, 
						  void* arg, 
						  int priority_arg, 
						  int flags, 
						  void* thd_id);
	static void		init(void) {}
	static void		cleanup(void) {}
	static ThreadData*	getSpecific(void);
	void			putSpecific();
	static void		restoreSpecific(void);
	static FB_THREAD_ID getId(void);
	static void		getSpecificData(void** t_data);
	static void		putSpecificData(void* t_data);

};

/* General purpose in use object structure */

struct iuo {
	struct iuo *iuo_next;
	void *iuo_object[10];
	USHORT iuo_in_use_count;
};

typedef iuo *IUO;

/* Recursive mutex structure */
struct rec_mutx_t {
	Firebird::Mutex rec_mutx_mtx;
	FB_THREAD_ID rec_mutx_id;
	SLONG rec_mutx_count;
};

typedef rec_mutx_t REC_MUTX_T;
//typedef rec_mutx_t *REC_MUTX_PTR;

#ifdef MULTI_THREAD
#define THD_GLOBAL_MUTEX_LOCK		THD_mutex_lock_global()
#define THD_GLOBAL_MUTEX_UNLOCK		THD_mutex_unlock_global()
#else // MULTI_THREAD
#define THD_GLOBAL_MUTEX_LOCK
#define THD_GLOBAL_MUTEX_UNLOCK
#endif // MULTI_THREAD

extern "C" {
	int		API_ROUTINE gds__thread_start(ThreadEntryPoint*, void*, int, int,
										 void*);
}

#endif /* JRD_THD_H */
