/*
 *	PROGRAM:	JRD access method
 *	MODULE:		atom.h
 *	DESCRIPTION:	Atomic lock definitions
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

#ifndef _JRD_ATOM_H_
#define _JRD_ATOM_H_

#include "../jrd/thd.h"

#define ATOM_LOCKS_PER_LEVEL	(1 << 6)	/* 64 - must be power of 2 */
#define ATOM_LOCK_LEVELS	2

#ifdef HAS_64BIT_POINTERS
#define ATOM_SHIFT		3		/* 64 bit */
#define ATOM_MASK		(ATOM_LOCKS_PER_LEVEL - 1)
#endif

#ifndef ATOM_SHIFT
#define ATOM_SHIFT		2		/* 32 bit */
#define ATOM_MASK		(ATOM_LOCKS_PER_LEVEL - 1)
#endif

#ifdef ANY_THREADING
#define ATOMIC_LOCK (atom_addr, level, intent) \
		THD_atomic_lock (tdbb, atom_addr, level, intent)

#define ATOMIC_UNLOCK (atom_addr, level) \
		THD_atomic_unlock (tdbb, atom_addr, level)
#else
#define ATOMIC_LOCK (atom_addr, level, intent)
#define ATOMIC_UNLOCK (atom_addr, level)
#endif

struct atomic_lock {
	RWLCK_T atom_lock;
	void *atom_addr;
};

/* ATOM access intent */

#define ATOM_READ	RWLCK_read
#define ATOM_WRITE	RWLCK_write

/* ATOM multilevel locking to avoid deadlocks */

#define ATOM_LEVEL_1	0
#define ATOM_LEVEL_2	1

/* Use atomic hardware intrinsics if platform supports them. */

#ifdef WIN_NT
#define ATOMIC_INCREMENT (lvalue, cast, atom_addr) \
    lvalue = (cast) InterlockedIncrement (atom_addr)

#define ATOMIC_DECREMENT (lvalue, cast, atom_addr) \
    lvalue = (cast) InterlockedDecrement (atom_addr)

#define ATOMIC_SWAP (lvalue, cast, atom_addr, rvalue) \
lvalue = (cast) InterlockedExchange (atom_addr, rvalue)
#endif /* WIN_NT */

/* Default emulation of atomic hardware intrinsics */

#ifndef ATOMIC_INCREMENT
#define ATOMIC_INCREMENT (lvalue, cast, atom_addr) \
    {ATOMIC_LOCK (tdbb, atom_addr, ATOM_LEVEL_1, ATOM_WRITE); \
     lvalue = (cast) ++(*(atom_addr)); \
     ATOMIC_UNLOCK (tdbb, atom_addr, ATOM_LEVEL_1);}
#endif

#ifndef ATOMIC_DECREMENT
#define ATOMIC_DECREMENT (lvalue, cast, atom_addr) \
    {ATOMIC_LOCK (tdbb, atom_addr, ATOM_LEVEL_1, ATOM_WRITE); \
     lvalue = (cast) --(*(atom_addr)); \
     ATOMIC_UNLOCK (tdbb, atom_addr, ATOM_LEVEL_1);}
#endif

#ifndef ATOMIC_SWAP
#define ATOMIC_SWAP (lvalue, cast, atom_addr, rvalue) \
    {ATOMIC_LOCK (tdbb, atom_addr, ATOM_LEVEL_1, ATOM_WRITE); \
     lvalue = (cast) *(atom_addr); *(atom_addr) = (ATOM) rvalue; \
     ATOMIC_UNLOCK (tdbb, atom_addr, ATOM_LEVEL_1);}
#endif


#endif /* _JRD_ATOM_H_ */
