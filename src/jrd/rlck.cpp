/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rlck.c
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
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/lck.h"
#include "../jrd/req.h"
#include "gen/codes.h"
#include "../jrd/all.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/isc_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/vio_proto.h"

#ifdef PC_ENGINE
static LCK allocate_record_lock(JRD_TRA, RPB *);
#endif
static LCK allocate_relation_lock(MemoryPool*, JRD_REL);
#ifdef PC_ENGINE
static LCK attachment_relation_lock(JRD_REL);
static void drop_record_lock(LCK);
static LCK find_record_lock(RPB *);
static BOOLEAN obtain_lock(JRD_TRA, LCK, USHORT);
static void start_record_locking(JRD_REL);
#endif


#ifdef PC_ENGINE
LCK RLCK_lock_record(RPB * rpb,
					 USHORT lock_level, int (*ast) (BLK), BLK ast_arg)
{
/**************************************
 *
 *	R L C K _ l o c k _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Lock the current record in the provided
 *	record parameter block at the specified level.
 *
 **************************************/
	JRD_REL relation;
	LCK record_locking, lock;
	SLONG process_count;
	TDBB tdbb;


/* first get a record lock on the desired record;
   if we can't get one then there is no point
   in signalling that this process needs to acquire locks */

	lock = RLCK_lock_record_implicit(0, rpb, lock_level, ast, ast_arg);
	if (!lock)
		return NULL;

	tdbb = GET_THREAD_DATA;

/*
   if the record is trying to be locked, check
   whether the record has already been updated on page
   by a transaction whose updates we cannot see yet--
   in which case we effectively cannot get the lock */

	if (VIO_check_if_updated(tdbb, rpb)) {
		RLCK_unlock_record_implicit(lock, rpb);
		return NULL;
	}

/* if this process has no record locks outstanding,
   indicate through the lock manager that we are
   starting the record locking protocol */

	relation = rpb->rpb_relation;
	if (!relation->rel_explicit_locks) {
		record_locking = RLCK_record_locking(relation);

		/* get a shared write lock to be compatible with other processes
		   that are doing record locking, but incompatible with those who aren't */

		if (record_locking->lck_logical == LCK_none)
			LCK_lock_non_blocking(tdbb, record_locking, LCK_SW, TRUE);
		else
			LCK_convert_non_blocking(tdbb, tdbb, record_locking, LCK_SW,
									 TRUE);
	}

	relation->rel_explicit_locks++;

	return lock;
}
#endif



#ifdef PC_ENGINE
LCK RLCK_lock_record_implicit(JRD_TRA transaction,
							  RPB * rpb,
							  USHORT lock_level,
							  int (*ast) (BLK), BLK ast_arg)
{
/**************************************
 *
 *	R L C K _ l o c k _ r e c o r d _ i m p l i c i t
 *
 **************************************
 *
 * Functional description
 *	Lock the given record for implicit access, a subset
 *	of what we need to do to lock it explicitly.
 *
 *	transaction is NULL for explicit record locking.
 *
 **************************************/
	LCK lock;
	USHORT interest_lock_level = 0;
	JRD_REL relation;

	lock = allocate_record_lock(transaction, rpb);
	lock->lck_ast = ast;
	lock->lck_object = ast_arg;

/* To ensure that relation and record locks respect each other,
   utilize a multigranularity locking scheme, establishing
   an interest lock level in the parent relation according to
   the following scheme:
       read lock on record := protected read
	   (implies) interest read lock on relation := shared read
       write lock on record := exclusive
	   (implies) interest write lock on relation := shared write
   Then as long as PR and EX locks are used to implement read
   and write locking on relations, the relation locks will
   be respected.

   This also guarantees that implicit record locks won't result
   in interest locks being taken out, since implicit record locks
   are SW.  Relations are already implicitly reserved in
   RLCK_reserve_relation(), so there is no need for an additional
   interest lock.
       implicit read lock on record := NO LOCK
	   (implies) NO interest read lock on relation
       implicit write lock on record := shared write
	   (implies) interest write lock on relation := shared write
*/

	relation = rpb->rpb_relation;
	if (lock_level == LCK_EX) {
		interest_lock_level = LCK_SW;
		relation->rel_write_locks++;
		relation->rel_lock_total++;
	}
	else if (lock_level == LCK_PR) {
		interest_lock_level = LCK_SR;
		relation->rel_read_locks++;
		relation->rel_lock_total++;
	}

/* first attempt to get the parent lock then get the record lock,
   using two-phase locking to help prevent deadlocks */

	if (interest_lock_level
		&& !obtain_lock(transaction, lock->lck_parent, interest_lock_level)
		|| !obtain_lock(transaction, lock, lock_level)) {
		RLCK_unlock_record_implicit(lock, rpb);
		return NULL;
	}

	return lock;
}
#endif


#ifdef PC_ENGINE
LCK RLCK_lock_relation(JRD_REL relation,
					   USHORT lock_level, int (*ast) (BLK), BLK ast_arg)
{
/**************************************
 *
 *	R L C K _ l o c k _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Lock a relation at the specified level.
 *
 **************************************/
	LCK lock;

/* allocate a relation lock hanging off the attachment
   block, then keep a count of the number of times it
   is used so that we know when to release it (bug #7478) */

	lock = attachment_relation_lock(relation);
	lock->lck_count++;
	lock->lck_ast = ast;
	lock->lck_object = ast_arg;

	if (!obtain_lock(0, lock, lock_level))
		ERR_post(gds_relation_lock, gds_arg_string, relation->rel_name, 0);

	return lock;
}
#endif


#ifdef PC_ENGINE
LCK RLCK_range_relation(JRD_TRA transaction,
						JRD_REL relation, int (*ast) (BLK), BLK ast_arg)
{
/**************************************
 *
 *	R L C K _ r a n g e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Lock a relation for a refresh range.
 *
 **************************************/
	TDBB tdbb;
	LCK lock;
	USHORT level, wait;
	USHORT result;
	ATT attachment;

	tdbb = GET_THREAD_DATA;
	attachment = tdbb->tdbb_attachment;

	if (transaction->tra_flags & TRA_system)
		return NULL;

	lock = allocate_relation_lock(transaction->tra_pool, relation);
	lock->lck_owner = (BLK) attachment;
	lock->lck_ast = ast;
	lock->lck_object = ast_arg;
	lock->lck_type = LCK_range_relation;

	level = LCK_PR;

	wait = (transaction->tra_flags & TRA_nowait) ? FALSE : TRUE;

/* get lock */

	result = LCK_lock_non_blocking(tdbb, lock, level, wait);

	if (result)
		return lock;
	else
		return NULL;
}
#endif


#ifdef PC_ENGINE
LCK RLCK_record_locking(JRD_REL relation)
{
/**************************************
 *
 *	R L C K _ r e c o r d _ l o c k i n g
 *
 **************************************
 *
 * Functional description
 *	Allocate a record locking lock, which determines
 *	when record locking is necessary for a particular
 *	relation.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	LCK lock;

	if (relation->rel_record_locking)
		return relation->rel_record_locking;

	tdbb = GET_THREAD_DATA;
	dbb = GET_DBB;

	lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) lck();
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_dbb = dbb;
	lock->lck_attachment = tdbb->tdbb_attachment;
	lock->lck_type = LCK_record_locking;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);

/* make the lock specific to the relation */

	lock->lck_length = sizeof(SLONG);
	lock->lck_key.lck_long = relation->rel_id;
	relation->rel_record_locking = lock;

/* set up an ast to start record locking if
   someone gets an incompatible lock */

	lock->lck_ast = start_record_locking;
	lock->lck_object = reinterpret_cast<blk*>(relation);

/* now attempt to get a PR on the lock to detect when
   anyone locks a record explicitly */

	LCK_lock(tdbb, _non_blocking(lock, LCK_PR, FALSE));
	return lock;
}
#endif


#ifdef PC_ENGINE
void RLCK_release_lock(LCK lock)
{
/**************************************
 *
 *	R L C K _ r e l e a s e _ l o c k
 *
 **************************************
 *
 * Functional description
 *      Release a lock of record or relation
 *	type.  The lock handle came from the
 *	user, so validate it carefully.
 *
 **************************************/

/* first do basic validation of the handle */

	if (!lock)
		ERR_post(gds_bad_lock_handle, 0);
	if (((BLK) lock)->blk_type != (UCHAR) type_lck)
		ERR_post(gds_bad_lock_handle, 0);
/* now use the lock type to determine the type
   of lock to release */
	if (lock->lck_type == LCK_relation)
		RLCK_unlock_relation(0, (JRD_REL) lock->lck_object);
	else if (lock->lck_type == LCK_record)
		RLCK_unlock_record(lock, 0);
	else
		ERR_post(gds_bad_lock_handle, 0);
}
#endif


#ifdef PC_ENGINE
void RLCK_release_locks(ATT attachment)
{
/**************************************
 *
 *	R L C K _ r e l e a s e _ l o c k s
 *
 **************************************
 *
 * Functional description
 *	Release all relation and record locks
 *	explicitly taken out during this attachment.
 *
 **************************************/
	VEC vector;
	LCK lock;
	vec::iterator *lptr, *lend;
/* unlock all explicit relation locks */
	if (vector = attachment->att_relation_locks)
		for (lptr = vector->begin(), lend = vector->end(); lptr < lend; lptr++)
			if (lock = ((LCK)(*lptr)) )
				RLCK_unlock_relation(0, (JRD_REL) lock->lck_object);
/* unlock all explicit record locks */
	while (lock = attachment->att_record_locks)
		RLCK_unlock_record(lock, 0);
/* clear the vector of user locks */
	if (vector = attachment->att_lck_quick_ref)
		for (lptr = vector->begin(), lend = vector->end(); lptr < lend; lptr++)
			*lptr = NULL;
}
#endif


LCK RLCK_reserve_relation(TDBB tdbb,
						  JRD_TRA transaction,
						  JRD_REL relation, USHORT write_flag, USHORT error_flag)
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
	LCK lock;
	USHORT level, wait;
	USHORT result;
	if (transaction->tra_flags & TRA_system)
		return NULL;
	if (write_flag && (tdbb->tdbb_database->dbb_flags & DBB_read_only))
		ERR_post(isc_read_only_database, 0);
	if (write_flag && (transaction->tra_flags & TRA_readonly))
		ERR_post(gds_read_only_trans, 0);
	lock = RLCK_transaction_relation_lock(transaction, relation);
/* Next, figure out what kind of lock we need */
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
	if (transaction->tra_flags & TRA_reserving)
		ERR_post(gds_unres_rel, gds_arg_string, relation->rel_name, 0);
	wait = (transaction->tra_flags & TRA_nowait) ? FALSE : TRUE;
/* get lock */
	if (lock->lck_logical)
		result = LCK_convert_non_blocking(NULL, lock, level, wait);
	else
		result = LCK_lock_non_blocking(NULL, lock, level, wait);
	if (result)
		return lock;
	else {
		if (error_flag)
			ERR_post((wait) ? gds_deadlock : gds_lock_conflict, 0);
		return NULL;
	}
}


void RLCK_shutdown_attachment(ATT attachment)
{
/**************************************
 *
 *	R L C K _ s h u t d o w n _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Shutdown the attachment's persistent record
 *	and relation locks. This runs at AST level.
 *
 **************************************/

	vec::iterator lock;

	TDBB tdbb = GET_THREAD_DATA;
/* Release child record locks before parent relation locks */
	for (LCK record_lock = attachment->att_record_locks;
		record_lock;
		record_lock = record_lock->lck_att_next)
	{
		LCK_release(tdbb, record_lock);
	}
	VEC lock_vector = attachment->att_relation_locks;
	if (lock_vector) {
		for (lock = lock_vector->begin(); lock != lock_vector->end(); ++lock) {
			if (*lock) {
				LCK_release(tdbb, (LCK)(*lock));
			}
		}
	}
}


void RLCK_shutdown_database(DBB dbb)
{
/**************************************
 *
 *	R L C K _ s h u t d o w n _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Shutdown the database locks shared for relation
 *	interest and record locking locks. This can be called
 *	at AST level.
 *
 **************************************/
	JRD_REL relation;
	vec::iterator ptr, end;
	VEC vector;
	TDBB tdbb;
	tdbb = GET_THREAD_DATA;
	if (!(vector = dbb->dbb_relations))
		return;
	for (ptr = vector->begin(), end =  vector->end(); ptr < end; ptr++)
		if ( (relation = ((JRD_REL)(*ptr)) ) ) {
			if (relation->rel_record_locking)
				LCK_release(tdbb, relation->rel_record_locking);
			if (relation->rel_interest_lock)
				LCK_release(tdbb, relation->rel_interest_lock);
			relation->rel_explicit_locks = 0;
			relation->rel_read_locks = 0;
			relation->rel_write_locks = 0;
			relation->rel_lock_total = 0;
		}
}


#ifdef PC_ENGINE
void RLCK_signal_refresh(JRD_TRA transaction)
{
/**************************************
 *
 *	R L C K _ s i g n a l _ r e f r e s h
 *
 **************************************
 *
 * Functional description
 *	For each reserved relation, get a refresh relation
 *	lock to signal possible refresh range users.
 *
 **************************************/
	TDBB tdbb;
	VEC vector;
	USHORT i;
	LCK lock;
	LCK local_lock;
	DBB dbb;
	JRD_REL relation;
	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
/* for each relation, take out a range relation lock and then release it */
	vector = transaction->tra_relation_locks;
	if (vector) {

		/* allocate a local lock */

		local_lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) lck();
		local_lock->lck_dbb = dbb;
		local_lock->lck_attachment = tdbb->tdbb_attachment;
		local_lock->lck_length = sizeof(SLONG);
		local_lock->lck_type = LCK_range_relation;
		local_lock->lck_owner_handle =
			LCK_get_owner_handle(tdbb, local_lock->lck_type);
		local_lock->lck_parent = dbb->dbb_lock;
		local_lock->lck_compatible = (BLK) tdbb->tdbb_attachment;
		for (i = 0; i < vector->count(); i++) {
			lock = (LCK *) ((*vector)[i]);
			if (lock) {
				relation = (JRD_REL) lock->lck_object;
				local_lock->lck_key.lck_long = relation->rel_id;
				local_lock->lck_object = reinterpret_cast<blk*>(relation);
				LCK_lock_non_blocking(tdbb, local_lock, LCK_SW, 0);
				LCK_release(tdbb, local_lock);
			}
		}
		ALL_release(local_lock);
	}
}
#endif


LCK RLCK_transaction_relation_lock(JRD_TRA transaction, JRD_REL relation)
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
	LCK lock;
	VEC vector;
	if ((vector = transaction->tra_relation_locks) &&
		(relation->rel_id < vector->count()) &&
		(lock = (LCK) (*vector)[relation->rel_id]))
		return lock;

	vector = transaction->tra_relation_locks =
		vec::newVector(*transaction->tra_pool, transaction->tra_relation_locks,
					   relation->rel_id + 1);
	
	if ( (lock = (LCK) (*vector)[relation->rel_id]) )
		return lock;

	lock = allocate_relation_lock(transaction->tra_pool, relation);
	lock->lck_owner = (BLK) transaction;
/* for relations locked within a transaction, add a second level of
   compatibility within the intra-process lock manager which specifies
   that relation locks are incompatible with locks taken out by other
   transactions, if a transaction is specified */
	lock->lck_compatible2 = (BLK) transaction;
	(*vector)[relation->rel_id] = (BLK) lock;

	return lock;
}


#ifdef PC_ENGINE
void RLCK_unlock_record(LCK lock, RPB * rpb)
{
/**************************************
 *
 *	R L C K _ u n l o c k _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Unlock the specified record lock, or if
 *	it's not available use the current record
 *	in the specified record parameter block.
 *
 **************************************/
	JRD_REL relation;
	LCK record_locking;
	SLONG process_count;
	TDBB tdbb;
	ATT attachment;
	if (rpb)
		relation = rpb->rpb_relation;
	else if (lock)
		relation = (JRD_REL) lock->lck_parent->lck_object;
	else
		relation = NULL;		/* theoretically impossible */
	RLCK_unlock_record_implicit(lock, rpb);
	tdbb = GET_THREAD_DATA;
	attachment = tdbb->tdbb_attachment;
	if (attachment->att_flags & ATT_shutdown)
		return;
/* decrement the count of explicit locks this process has taken out;
   if there are none, go back to a PR on the lock--
   if we cannot obtain this, it means someone else has explicit
   record locks taken out and we should just release the lock */
	if (relation && !--relation->rel_explicit_locks) {
		record_locking = relation->rel_record_locking;
		if (!LCK_convert_non_blocking(tdbb, record_locking, LCK_PR, FALSE))
			LCK_release(tdbb, record_locking);
	}

}
#endif


#ifdef PC_ENGINE
void RLCK_unlock_record_implicit(LCK lock, RPB * rpb)
{
/**************************************
 *
 *	R L C K _ u n l o c k _ r e c o r d _ i m p l i c i t
 *
 **************************************
 *
 * Functional description
 *	Unlock a record-level lock.
 *
 **************************************/
	JRD_REL relation;
	USHORT lock_level;
	ATT attachment;
	TDBB tdbb;
	tdbb = GET_THREAD_DATA;
	if (!lock)
		lock = find_record_lock(rpb);
	lock_level = lock->lck_logical;
	drop_record_lock(lock);
	LCK_release(tdbb, lock);
	attachment = tdbb->tdbb_attachment;
	if (attachment->att_flags & ATT_shutdown) {
		ALL_release(lock);
		return;
	}

/* handle the release half of the multigranularity locking scheme:
   if there are no more write locks, downgrade the interest
   lock on the parent relation; if there are no more read or
   write locks, release the lock entirely

   For implicit locks, there are no relation intrest locks.
   So do nothing.
   */

	relation = (JRD_REL) lock->lck_parent->lck_object;
	if (lock_level == LCK_EX) {
		if (!--relation->rel_write_locks)
			if (!relation->rel_read_locks)
				LCK_release(tdbb, relation->rel_interest_lock);
			else
				LCK_convert_non_blocking(tdbb, relation->rel_interest_lock,
										 LCK_SR, TRUE);
	}
	else if (lock_level == LCK_PR) {
		if (!--relation->rel_read_locks && !relation->rel_write_locks)
			LCK_release(tdbb, relation->rel_interest_lock);
	}

	ALL_release(lock);
}
#endif


#ifdef PC_ENGINE
void RLCK_unlock_relation(LCK lock, JRD_REL relation)
{
/**************************************
 *
 *	R L C K _ u n l o c k _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Release the attachment's persistent lock
 *	on the specified relation.
 *
 **************************************/
	TDBB tdbb;
	ATT attachment;
	VEC vector;
	USHORT id;
	tdbb = GET_THREAD_DATA;
	attachment = tdbb->tdbb_attachment;
	if (!(vector = attachment->att_relation_locks))
		return;
	if (relation) {
		id = relation->rel_id;
		if (id >= vector->count())
			return;
		lock = (LCK) (*vector)[id];
	}
	else
		for (id = 0; id < vector->count(); id++)
			if (lock == (LCK) (*vector)[id])
				break;
	if (!lock)
		return;
/* decrement the use count; if it goes to zero,
   there are no further locks taken out in this
   attachment so we can release the lock (bug #7478) */
	if (lock->lck_count > 1) {
		lock->lck_count--;
		return;
	}

	LCK_release(tdbb, lock);
	ALL_release(lock);
	(*vector)[id] = NULL;
}
#endif


#ifdef PC_ENGINE
static LCK allocate_record_lock(JRD_TRA transaction, RPB * rpb)
{
/**************************************
 *
 *	a l l o c a t e _ r e c o r d _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Create a record lock block for the current
 *	record in the passed rpb.
 *	transaction used only for implicit record locks.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	JRD_REL relation;
	LCK lock;
	ATT attachment;
	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	attachment = tdbb->tdbb_attachment;
	if (!rpb->rpb_record)
		ERR_post(gds_no_cur_rec, 0);
/* allocate a lock block for the record lock */
	lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) lck();
	lock->lck_dbb = dbb;
	lock->lck_attachment = attachment;
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_type = LCK_record;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
/* use the relation lock as the lock parent */
	relation = rpb->rpb_relation;
	if (transaction)
		lock->lck_parent =
			RLCK_transaction_relation_lock(transaction, relation);
	else {
		if (!relation->rel_interest_lock)
			relation->rel_interest_lock =
				allocate_relation_lock(dbb->dbb_permanent, relation);
		lock->lck_parent = relation->rel_interest_lock;
	}

/* indicate that this lock should be compatible at the attachment
   level--meaning that two record locks taken out within the same
   attachment should always be compatible; ditto for the interest lock */

	lock->lck_compatible = (BLK) attachment;
	lock->lck_parent->lck_compatible = (BLK) attachment;
/* link in the record lock with the other record locks
   taken out by this attachment */
	lock->lck_att_next = attachment->att_record_locks;
	attachment->att_record_locks = lock;
/* fill in the lock value using the record number */
	lock->lck_length = sizeof(SLONG);
	lock->lck_key.lck_long = rpb->rpb_number;
	return lock;
}


#endif
static LCK allocate_relation_lock(MemoryPool* pool, JRD_REL relation)
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
	TDBB tdbb;
	DBB dbb;
	LCK lock;
	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	lock = FB_NEW_RPT(*pool, sizeof(SLONG)) lck();
	lock->lck_dbb = dbb;
	lock->lck_attachment = tdbb->tdbb_attachment;
	lock->lck_length = sizeof(SLONG);
	lock->lck_key.lck_long = relation->rel_id;
	lock->lck_type = LCK_relation;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_parent = dbb->dbb_lock;
/* enter all relation locks into the intra-process lock manager and treat
   them as compatible within the attachment according to IPLM rules */
	lock->lck_compatible = (BLK) tdbb->tdbb_attachment;
/* the lck_object is used here to find the relation
   block from the lock block */
	lock->lck_object = reinterpret_cast<blk*>(relation);
	return lock;
}

#ifdef PC_ENGINE
static LCK attachment_relation_lock(JRD_REL relation)
{
/**************************************
 *
 *	a t t a c h m e n t _ r e l a t i o n _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Take out a persistent relation lock at the
 *	attachment level.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	ATT attachment;
	LCK lock;
	VEC vector;
	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	attachment = tdbb->tdbb_attachment;

	if ((vector = attachment->att_relation_locks) &&
		(relation->rel_id < vector->count()) &&
		(lock = (LCK) (*vector)[relation->rel_id]))
		return lock;

	vector = attachment->att_relation_locks =
		vec::newVector(*dbb->dbb_permanent, attachment->att_relation_locks,
					   relation->rel_id + 1);

	if ( (lock = (LCK) (*vector)[relation->rel_id]) )
		return lock;

	lock = allocate_relation_lock(dbb->dbb_permanent, relation);
	lock->lck_owner = (BLK) attachment;
	(*vector)[relation->rel_id] = (BLK) lock;

	return lock;
}
#endif

#ifdef PC_ENGINE
static void drop_record_lock(LCK record_lock)
{
/**************************************
 *
 *	d r o p _ r e c o r d _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Find the record lock in the linked list off
 *	the attachment block, and drop it from the list.
 *
 **************************************/
	TDBB tdbb;
	ATT attachment;
	LCK *lock;
	tdbb = GET_THREAD_DATA;
/* look through all the record locks taken out by this attachment
   looking for one with the same record number and relation id */
	attachment = tdbb->tdbb_attachment;
	for (lock = &attachment->att_record_locks; *lock;
		 lock = &(*lock)->lck_att_next) if (*lock == record_lock) {
			*lock = (*lock)->lck_att_next;
			break;
		}
}
#endif


#ifdef PC_ENGINE
static LCK find_record_lock(RPB * rpb)
{
/**************************************
 *
 *	f i n d _ r e c o r d _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Find the record lock previously
 *	defined for a record.
 *
 **************************************/
	TDBB tdbb;
	LCK lock;
	ATT attachment;
	JRD_REL relation;
	tdbb = GET_THREAD_DATA;
/* look through all the record locks taken out by this attachment
   looking for one with the same record number and relation */
	attachment = tdbb->tdbb_attachment;
	for (lock = attachment->att_record_locks; lock; lock = lock->lck_att_next)
		if ((rpb->rpb_number == lock->lck_key.lck_long)
			&& (rpb->rpb_relation == (JRD_REL) lock->lck_parent->lck_object))
			break;
	return lock;
}
#endif

#ifdef PC_ENGINE
static BOOLEAN obtain_lock(JRD_TRA transaction, LCK lock, USHORT lock_level)
{
/**************************************
 *
 *	o b t a i n _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Obtain the specified lock at the
 *	necessary level.
 *
 **************************************/
	USHORT wait_flag;
	if (transaction)
		wait_flag = (transaction->tra_flags & TRA_nowait) ? FALSE : TRUE;
	else
		wait_flag = FALSE;
/* return if lock level OK and if the lock has not been released
   (like as part of a refresh range)  */
	if ((lock_level <= lock->lck_logical) && (lock->lck_id != -1))
		return TRUE;
	if ((lock->lck_logical) && (lock->lck_id != -1)) {
		if (LCK_convert_non_blocking(NULL, lock, lock_level, wait_flag))
			return TRUE;
	}
	else if (LCK_lock_non_blocking(NULL, lock, lock_level, wait_flag))
		return TRUE;
	return FALSE;
}
#endif

#ifdef PC_ENGINE
static void start_record_locking(JRD_REL relation)
{
/**************************************
 *
 *	s t a r t _ r e c o r d _ l o c k i n g
 *
 **************************************
 *
 * Functional description
 *	A blocking AST has been issued to give up
 *	the lock on the record locking semaphore.
 *	Flag the fact that record locking is now
 *	necessary for reading and writing records.
 *
 **************************************/
	LCK record_locking;
	record_locking = relation->rel_record_locking;
/* if we have shared write, it means we have records
   locked; we won't give up this lock for anyone! */
	if (record_locking->lck_physical == LCK_SW)
		return;
	ISC_ast_enter();
	LCK_release(NULL, record_locking);
	ISC_ast_exit();
}
#endif
