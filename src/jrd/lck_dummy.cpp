/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		lock.c
 *	DESCRIPTION:	Ultrix Lock Manager
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
#ifdef UNIX
#include <signal.h>
#endif

#define PTR	SLONG


int LOCK_convert(
				 PTR request_offset,
				 UCHAR type,
				 SSHORT wait,
				 int (*ast_routine) (int *),
int *ast_argument, STATUS * status_vector)
{
/**************************************
 *
 *	L O C K _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Perform a lock conversion, if possible.  If the lock cannot be
 *	granted immediately, either return immediately or wait depending
 *	on a wait flag.  If the lock is granted return TRUE, otherwise
 *	return FALSE.  Note: if the conversion would cause a deadlock,
 *	FALSE is returned even if wait was requested.
 *
 **************************************/

	return TRUE;
}


int LOCK_deq(PTR request_offset)
{
/**************************************
 *
 *	L O C K _ d e q 
 *
 **************************************
 *
 * Functional description
 *	Release an outstanding lock.
 *
 **************************************/

	return TRUE;
}


UCHAR LOCK_downgrade(PTR request_offset, STATUS * status_vector)
{
/**************************************
 *
 *	L O C K _ d o w n g r a d e
 *
 **************************************
 *
 * Functional description
 *	Downgrade an existing lock returning
 *	its new state.
 *
 **************************************/

	return 0;
}


SLONG LOCK_enq(PTR parent_request,
			   USHORT series,
			   UCHAR * value,
			   USHORT length,
			   UCHAR type,
			   int (*ast_routine) (int *),
			   int *ast_argument,
			   SSHORT wait, STATUS * status_vector, PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ e n q
 *
 **************************************
 *
 * Functional description
 *	Enque on a lock.  If the lock can't be granted immediately,
 *	return an event count on which to wait.  If the lock can't
 *	be granted because of deadlock, return NULL.
 *
 **************************************/

	return TRUE;
}


void LOCK_fini(STATUS * status_vector, PTR * owner_offset)
{
/**************************************
 *
 *	L O C K _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Release the process block, any outstanding locks,
 *	and unmap the lock manager.  This is usually called
 *	by the cleanup handler.
 *
 **************************************/

}


int LOCK_init(
			  STATUS * status_vector,
			  SSHORT owner_flag,
			  SLONG owner_id, UCHAR owner_type, SLONG * owner_handle)
{
/**************************************
 *
 *	L O C K _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize lock manager for process.
 *
 **************************************/

	return FB_SUCCESS;
}


void LOCK_manager(PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ m a n a g e r
 *
 **************************************
 *
 * Functional description
 *	LOCK_manager is used only the the privileged lock manager
 *	process.  The routine simply waits on it's semaphore.  It
 *	the semaphore is poked, it wakes up, looks for blocking
 *	signals to be delived, delivers them, and goes back to
 *	sleep.
 *
 **************************************/

}


SLONG LOCK_read_data(PTR request_offset)
{
/**************************************
 *
 *	L O C K _ r e a d _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Read data associated with a lock.
 *
 **************************************/

	return 0;
}


SLONG LOCK_read_data2(PTR parent_request,
					  USHORT series,
					  UCHAR * value, USHORT length, PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ r e a d _ d a t a 2
 *
 **************************************
 *
 * Functional description
 *	Read data associated with transient locks.
 *
 **************************************/

	return 0;
}


void LOCK_re_post(int (*ast) (int *), int *arg, PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ r e _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Re-post an AST that was previously blocked.
 *
 **************************************/

}


SLONG LOCK_write_data(PTR request_offset, SLONG data)
{
/**************************************
 *
 *	L O C K _ w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Write a longword into the lock block.
 *
 **************************************/

	return 0;
}
