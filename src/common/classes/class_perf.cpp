/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		class_perf.cpp
 *	DESCRIPTION:	Class library performance measurements
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: class_perf.cpp,v 1.8 2003-09-08 20:23:32 skidder Exp $
 *
 */

#include "tree.h"
#include "alloc.h"
//#include "../memory/memory_pool.h"
#include <stdio.h>
#include <time.h>
#include <set>

clock_t t;

void start() {
	t = clock();
}

#define TEST_ITEMS 1000000

void report(int scale) {
	clock_t d = clock();
	printf("Add+remove %d elements from tree of scale %d took %d milliseconds. \n", 
		TEST_ITEMS,	scale, (int)(d-t)*1000/CLOCKS_PER_SEC);
}

using namespace Firebird;

static void testTree() {
	printf("Fill array with test data (%d items)...", TEST_ITEMS);
	Vector<int, TEST_ITEMS> *v = new Vector<int, TEST_ITEMS>();
	int n = 0;
	int i;
	for (i=0;i<TEST_ITEMS;i++) {
		n = n * 45578 - 17651;
		// Fill it with quasi-random values in range 0...TEST_ITEMS-1
		v->add(((i+n) % TEST_ITEMS + TEST_ITEMS)/2);
	}
	printf(" DONE\n");
	MallocAllocator temp;
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 10, 10> tree10(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree10.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree10.locate((*v)[i]))
			tree10.fastRemove();
	}
	report(10);

	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 50, 50> tree50(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree50.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree50.locate((*v)[i]))
			tree50.fastRemove();
	}
	report(50);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 75, 75> tree75(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree75.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree75.locate((*v)[i]))
			tree75.fastRemove();
	}
	report(75);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 100, 100> tree100(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree100.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree100.locate((*v)[i]))
			tree100.fastRemove();
	}
	report(100);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 200, 200> tree200(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree200.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree200.locate((*v)[i]))
			tree200.fastRemove();
	}
	report(200);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 250, 250> tree250(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree250.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree250.locate((*v)[i]))
			tree250.fastRemove();
	}
	report(250);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 500, 500> tree500(NULL);
	for (i=0; i<TEST_ITEMS;i++)
		tree500.add((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++) {
		if (tree500.locate((*v)[i]))
			tree500.fastRemove();
	}
	report(500);
	
	std::set<int> stlTree;
	start();
	for (i=0; i<TEST_ITEMS;i++)
		stlTree.insert((*v)[i]);
	for (i=0; i<TEST_ITEMS;i++)
		stlTree.erase((*v)[i]);
	clock_t d = clock();
	printf("Just a reference: add+remove %d elements from STL tree took %d milliseconds. \n", 
		TEST_ITEMS,	(int)(d-t)*1000/CLOCKS_PER_SEC);
}


void report() {
	clock_t d = clock();
	printf("Operation took %d milliseconds.\n", (int)(d-t)*1000/CLOCKS_PER_SEC);
}

#define ALLOC_ITEMS 1000000
#define MAX_ITEM_SIZE 50
#define BIG_ITEMS (ALLOC_ITEMS/10)
#define BIG_SIZE (MAX_ITEM_SIZE*5)

struct AllocItem {
	int order;
	void *item;
	static int compare(const AllocItem &i1, const AllocItem &i2) {
		return i1.order > i2.order || (i1.order==i2.order && i1.item > i2.item);
	}
};

static void testAllocatorOverhead() {
	printf("Calculating measurement overhead...\n");
	start();
	MallocAllocator allocator;
	BePlusTree<AllocItem,AllocItem,MallocAllocator,DefaultKeyValue<AllocItem>,AllocItem> items(&allocator),
		bigItems(&allocator);
	// Allocate small items
	int n = 0;
	int i;
	for (i=0;i<ALLOC_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, (void*)i};
		items.add(temp);
	}
	// Deallocate half of small items
	n = 0;
	if (items.getFirst()) do {
		items.current();
		n++;
	} while (n < ALLOC_ITEMS/2 && items.getNext());	
	// Allocate big items
	for (i=0;i<BIG_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, (void*)i};
		bigItems.add(temp);
	}
	// Deallocate the rest of small items
	while (items.getNext()) {
		items.current();
	}
	// Deallocate big items
	if (bigItems.getFirst()) do {
		bigItems.current();
	} while (bigItems.getNext());
	report();
}
	
static void testAllocatorMemoryPool() {
	printf("Test run for Firebird::MemoryPool...\n");
	start();
	Firebird::MemoryPool* pool = Firebird::MemoryPool::createPool();	
	MallocAllocator allocator;
	BePlusTree<AllocItem,AllocItem,MallocAllocator,DefaultKeyValue<AllocItem>,AllocItem> items(&allocator),
		bigItems(&allocator);
	// Allocate small items
	int i, n = 0;	
	for (i=0;i<ALLOC_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % MAX_ITEM_SIZE + MAX_ITEM_SIZE)/2+1)};
		items.add(temp);
	}
	// Deallocate half of small items
	n = 0;
	if (items.getFirst()) do {
		pool->deallocate(items.current().item);
		n++;
	} while (n < ALLOC_ITEMS/2 && items.getNext());	
	// Allocate big items
	for (i=0;i<BIG_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % BIG_SIZE + BIG_SIZE)/2+1)};
		bigItems.add(temp);
	}
	// Deallocate the rest of small items
	while (items.getNext()) {
		pool->deallocate(items.current().item);
	}
	// Deallocate big items
	if (bigItems.getFirst()) do {
		pool->deallocate(bigItems.current().item);
	} while (bigItems.getNext());
	Firebird::MemoryPool::deletePool(pool);
	report();
}

static void testAllocatorMalloc() {
	printf("Test reference run for ::malloc...\n");
	start();
	MallocAllocator allocator;
	BePlusTree<AllocItem,AllocItem,MallocAllocator,DefaultKeyValue<AllocItem>,AllocItem> items(&allocator),
		bigItems(&allocator);
	// Allocate small items
	int i, n = 0;
	for (i=0;i<ALLOC_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, malloc((n % MAX_ITEM_SIZE + MAX_ITEM_SIZE)/2+1)};
		items.add(temp);
	}
	// Deallocate half of small items
	n = 0;
	if (items.getFirst()) do {
		free(items.current().item);
		n++;
	} while (n < ALLOC_ITEMS/2 && items.getNext());	
	// Allocate big items
	for (i=0;i<BIG_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, malloc((n % BIG_SIZE + BIG_SIZE)/2+1)};
		bigItems.add(temp);
	}
	// Deallocate the rest of small items
	while (items.getNext()) {
		free(items.current().item);
	}
	// Deallocate big items
	if (bigItems.getFirst()) do {
		free(bigItems.current().item);
	} while (bigItems.getNext());
	report();
}

/*static void testAllocatorOldPool() {
	printf("Test run for old MemoryPool...\n");
	start();
	::MemoryPool *pool = new ::MemoryPool(0,getDefaultMemoryPool());
	MallocAllocator allocator;
	BePlusTree<AllocItem,AllocItem,MallocAllocator,DefaultKeyValue<AllocItem>,AllocItem> items(&allocator),
		bigItems(&allocator);
	// Allocate small items
	int n = 0;
	int i;
	for (i=0;i<ALLOC_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % MAX_ITEM_SIZE + MAX_ITEM_SIZE)/2+1,0)};
		items.add(temp);
	}
	// Deallocate half of small items
	n = 0;
	if (items.getFirst()) do {
		pool->deallocate(items.current().item);
		n++;
	} while (n < ALLOC_ITEMS/2 && items.getNext());	
	// Allocate big items
	for (i=0;i<BIG_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % BIG_SIZE + BIG_SIZE)/2+1,0)};
		bigItems.add(temp);
	}
	// Deallocate the rest of small items
	while (items.getNext()) {
		pool->deallocate(items.current().item);
	}
	// Deallocate big items
	if (bigItems.getFirst()) do {
		pool->deallocate(bigItems.current().item);
	} while (bigItems.getNext());
	delete pool;
	report();
}*/

int main() {
	testTree();
	testAllocatorOverhead();
	testAllocatorMemoryPool();
	testAllocatorMalloc();
//	testAllocatorOldPool();
}
