/*
 *	PROGRAM:	Alice
 *	MODULE:		all.h
 *	DESCRIPTION:	Data allocation declarations
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

#ifndef ALICE_ALL_H
#define ALICE_ALL_H

#include "../jrd/block_cache.h"

class AliceMemoryPool : public MemoryPool
{
protected:
	// Dummy constructor and destructor. Should never be called
	AliceMemoryPool() : MemoryPool(NULL, default_stats_group, NULL, NULL) {}
	~AliceMemoryPool() {}	
public:
	static AliceMemoryPool* createPool();
	static void deletePool(AliceMemoryPool* pool);
};

#endif // ALICE_ALL_H

