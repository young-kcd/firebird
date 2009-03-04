/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rlck.cpp
 *	DESCRIPTION:	Record and Relation Lock Manager
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/lck.h"
#include "../jrd/req.h"
#include "gen/iberror.h"
#include "../jrd/all.h"
#include "../jrd/err_proto.h"
#include "../jrd/isc_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/thd.h"
#include "../jrd/vio_proto.h"

using namespace Jrd;

static Lock* allocate_relation_lock(MemoryPool*, jrd_rel*);


Lock* RLCK_reserve_relation(thread_db* tdbb,
						  jrd_tra* transaction,
						  jrd_rel* relation, bool write_flag, bool error_flag)
{
/**************************************
 *
 *	R L C K _ r e s e r v e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Lock a relation within a transaction.  If the relation
 *	is already locked at a lower level, upgrade the lock.
 *
 **************************************/
	if (transaction->tra_flags & TRA_system)
		return NULL;
	if (write_flag && (tdbb->getDatabase()->dbb_flags & DBB_read_only))
		ERR_post(isc_read_only_database, isc_arg_end);
	if (write_flag && (transaction->tra_flags & TRA_readonly))
		ERR_post(isc_read_only_trans, isc_arg_end);
	Lock* lock = RLCK_transaction_relation_lock(transaction, relation);
	
/* Next, figure out what kind of lock we need */
	USHORT level;
	if (transaction->tra_flags & TRA_degree3) {
		if (write_flag)
			level = LCK_EX;
		else
			level = LCK_PR;
	}
	else {
		if (write_flag)
			level = LCK_SW;
		else
			level = LCK_none;
	}

/* If the lock is already "good enough", we're done */

	if (level <= lock->lck_logical)
		return lock;

	/* Nobody activates it. Same in FB1.
	if (transaction->tra_flags & TRA_reserving)
	{
		ERR_post(isc_unres_rel, isc_arg_string, 
				relation->rel_name.c_str(), isc_arg_end);
	}
	*/

/* get lock */
	USHORT result;
	if (lock->lck_logical)
		result = LCK_convert_non_blocking(NULL, lock, level,
										  transaction->getLockWait());
	else
		result = LCK_lock_non_blocking(NULL, lock, level,
									   transaction->getLockWait());
	if (result)
		return lock;
	else {
		if (error_flag)
			ERR_punt();
		return NULL;
	}
}


Lock* RLCK_transaction_relation_lock(jrd_tra* transaction, jrd_rel* relation)
{
/**************************************
 *
 *	R L C K _ t r a n s a c t i o n _ r e l a t i o n _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Take out a relation lock within the context of
 *	a transaction.
 *
 **************************************/
	Lock* lock;
	vec<Lock*>* vector = transaction->tra_relation_locks;
	if (vector &&
		(relation->rel_id < vector->count()) &&
		(lock = (*vector)[relation->rel_id]))
	{
		return lock;
	}

	vector = transaction->tra_relation_locks =
		vec<Lock*>::newVector(*transaction->tra_pool, transaction->tra_relation_locks,
					   relation->rel_id + 1);
	
	if ( (lock = (*vector)[relation->rel_id]) )
		return lock;

	lock = allocate_relation_lock(transaction->tra_pool, relation);
	lock->lck_owner = transaction;
/* for relations locked within a transaction, add a second level of
   compatibility within the intra-process lock manager which specifies
   that relation locks are incompatible with locks taken out by other
   transactions, if a transaction is specified */
	lock->lck_compatible2 = transaction;
	(*vector)[relation->rel_id] = lock;

	return lock;
}


static Lock* allocate_relation_lock(MemoryPool* pool, jrd_rel* relation)
{
/**************************************
 *
 *	a l l o c a t e _ r e l a t i o n _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a lock block for a relation lock.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Database* dbb = tdbb->getDatabase();

	SSHORT	relLockLen = relation->getRelLockKeyLength();

	Lock* lock = FB_NEW_RPT(*pool, relLockLen) Lock();
	lock->lck_dbb = dbb;
	lock->lck_length = relLockLen;
	relation->getRelLockKey(tdbb, &lock->lck_key.lck_string[0]);
	lock->lck_type = LCK_relation;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_parent = dbb->dbb_lock;
/* enter all relation locks into the intra-process lock manager and treat
   them as compatible within the attachment according to IPLM rules */
	lock->lck_compatible = tdbb->getAttachment();
/* the lck_object is used here to find the relation
   block from the lock block */
	lock->lck_object = relation;
	return lock;
}

