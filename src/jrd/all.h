/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		all.h
 *	DESCRIPTION:	Block allocator blocks
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

#ifndef JRD_ALL_H
#define JRD_ALL_H

#include "../jrd/jrd.h"
#include "../common/classes/alloc.h"
#include "../jrd/block_cache.h"
#include "../jrd/lls.h"
#include "fb_string.h"

TEXT* ALL_cstring(const Firebird::string& in_string);
void ALL_fini(void);
void ALL_init(void);
//void ALL_push(BLK , LLS *);
//BLK ALL_pop(LLS *);

struct blk;

namespace Jrd {
	class SparseBitmap;
	class BitmapSegment;
	class Database;
	class DataComprControl;
}

void ALL_print_memory_pool_info(IB_FILE*, Jrd::Database*);

#ifdef DEV_BUILD
void ALL_check_memory(void);
#endif

class JrdMemoryPool : public MemoryPool
{
protected:
	// Dummy constructor and destructor. Should never be called
	JrdMemoryPool() : MemoryPool(NULL, default_stats_group, NULL, NULL), lls_cache(*this) {}
	~JrdMemoryPool() {}	
public:
	static JrdMemoryPool *createDbPool(Firebird::MemoryStats &stats);
	static JrdMemoryPool *createPool();
	static void deletePool(JrdMemoryPool* pool);
	static void noDbbDeletePool(JrdMemoryPool* pool);

	static blk* ALL_pop(lls**);
	static void       ALL_push(blk*, lls**);

    Jrd::SparseBitmap* plb_buckets;   /* available bit map buckets */
    Jrd::BitmapSegment* plb_segments;  /* available bit map segments */
	Jrd::DataComprControl* plb_dccs;

private:
	BlockCache<lls> lls_cache;  /* Was plb_lls */
};

#endif	// JRD_ALL_H

