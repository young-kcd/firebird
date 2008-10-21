/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		lock_proto.h
 *	DESCRIPTION:	Prototype header file for lock.cpp
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

#ifndef LOCK_LOCK_PROTO_H
#define LOCK_LOCK_PROTO_H

#include "../common/classes/semaphore.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/init.h"
#include "../common/classes/RefCounted.h"
#include "../jrd/ThreadStart.h"
#include "../lock/lock.h"

namespace Firebird {
	class AtomicCounter;
	class Mutex;
	class RWLock;
}

namespace Jrd {

class thread_db;

class LockManager : public Firebird::RefCounted, public Firebird::GlobalStorage
{
	typedef Firebird::GenericMap<Firebird::Pair<Firebird::NonPooled<int, LockManager*> > > DbLockMgrMap;

	static Firebird::GlobalPtr<DbLockMgrMap> g_lmMap;
	static Firebird::GlobalPtr<Firebird::Mutex> g_mapMutex;
	static const char* PATTERN;

	const int PID;

public:
	static LockManager* create(const Firebird::PathName&);

	bool initializeOwner(thread_db*, LOCK_OWNER_T, UCHAR, SRQ_PTR*);
	void shutdownOwner(thread_db*, SRQ_PTR*);
	bool setOwnerHandle(SRQ_PTR, SRQ_PTR);

	SLONG enqueue(thread_db*, SRQ_PTR, SRQ_PTR, const USHORT, const UCHAR*, const USHORT, UCHAR,
				  lock_ast_t, void*, SLONG, SSHORT, SRQ_PTR);
	bool convert(thread_db*, SRQ_PTR, UCHAR, SSHORT, lock_ast_t, void*);
	UCHAR downgrade(thread_db*, const SRQ_PTR);
	bool dequeue(const SRQ_PTR);

	void repost(thread_db*, lock_ast_t, void*, SRQ_PTR);

	SLONG queryData(SRQ_PTR, USHORT, USHORT);
	SLONG readData(SRQ_PTR);
	SLONG readData2(SRQ_PTR, USHORT, const UCHAR*, USHORT, SRQ_PTR);
	SLONG writeData(SRQ_PTR, SLONG);

private:
	explicit LockManager(int);
	~LockManager();

	bool lockOrdering() const
	{
		return (m_header->lhb_flags & LHB_lock_ordering) ? true : false;
	}

	void acquire_shmem(SRQ_PTR);
	UCHAR* alloc(SSHORT, ISC_STATUS*);
	lbl* alloc_lock(USHORT, ISC_STATUS*);
	void blocking_action(thread_db*, SRQ_PTR, SRQ_PTR);
	void blocking_action_thread();
	void bug(ISC_STATUS*, const TEXT*);
#ifdef DEV_BUILD
	void bug_assert(const TEXT*, ULONG);
#endif
	bool create_owner(ISC_STATUS*, LOCK_OWNER_T, UCHAR, SRQ_PTR*);
	bool create_process(ISC_STATUS*);
	void deadlock_clear();
	lrq* deadlock_scan(own*, lrq*);
	lrq* deadlock_walk(lrq*, bool*);
#ifdef DEBUG
	void debug_delay(ULONG);
#endif
	lbl* find_lock(SRQ_PTR, USHORT, const UCHAR*, USHORT, USHORT*);
	lrq* get_request(SRQ_PTR);
	void grant(lrq*, lbl*);
	SRQ_PTR grant_or_que(thread_db*, lrq*, lbl*, SSHORT);
	void init_owner_block(own*, UCHAR, LOCK_OWNER_T) const;
	void initialize(SH_MEM, bool);
	void insert_data_que(lbl*);
	void insert_tail(SRQ, SRQ);
	bool internal_convert(thread_db*, SRQ_PTR, UCHAR, SSHORT, lock_ast_t, void*);
	void internal_dequeue(SRQ_PTR);
	static USHORT lock_state(const lbl*);
	void post_blockage(thread_db*, lrq*, lbl*);
	void post_history(USHORT, SRQ_PTR, SRQ_PTR, SRQ_PTR, bool);
	void post_pending(lbl*);
	void post_wakeup(own*);
	bool probe_processes();
	void purge_owner(SRQ_PTR, own*);
	void purge_process(prc*);
	void remap_local_owners();
	void remove_que(SRQ);
	void release_shmem(SRQ_PTR);
	void release_mutex();
	void release_request(lrq*);
	bool signal_owner(thread_db*, own*, SRQ_PTR);
//#define VALIDATE_LOCK_TABLE
#ifdef VALIDATE_LOCK_TABLE
	void validate_history(const SRQ_PTR history_header);
	void validate_parent(const lhb*, const SRQ_PTR);
	void validate_lhb(const lhb*);
	void validate_lock(const SRQ_PTR, USHORT, const SRQ_PTR);
	void validate_owner(const SRQ_PTR, USHORT);
	void validate_request(const SRQ_PTR, USHORT, USHORT);
	void validate_shb(const SRQ_PTR);
#endif
	USHORT wait_for_request(thread_db*, lrq*, SSHORT);

	static THREAD_ENTRY_DECLARE blocking_action_thread(THREAD_ENTRY_PARAM);

	static void initialize(void* arg, SH_MEM shmem, bool init)
	{
		LockManager* const lockMgr = static_cast<LockManager*>(arg);
		lockMgr->initialize(shmem, init);
	}

	bool m_bugcheck;
	lhb* volatile m_header;
	prc* m_process;
	SRQ_PTR m_processOffset;

	SH_MEM_T m_shmem;

	Firebird::Mutex m_localMutex;
	Firebird::RWLock m_remapSync;
	Firebird::AtomicCounter m_waitingOwners;

	Firebird::Semaphore m_cleanupSemaphore;
	Firebird::Semaphore m_startupSemaphore;

	const int m_dbId;
	Firebird::PathName m_lockFile;

#ifdef WIN_NT
	struct mtx m_shmemMutex;
#endif
};

} // namespace

#endif // LOCK_LOCK_PROTO_H
