/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		class_test.cpp
 *	DESCRIPTION:	Class library integrity tests
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
 *  $Id: class_test.cpp,v 1.11 2003-11-03 23:50:05 brodsom Exp $
 *
 */

#include "../../include/firebird.h"
#include "tree.h"
#include "alloc.h"
#include <stdio.h>

using namespace Firebird;

void testVector() {
	printf("Test Firebird::Vector: ");
	Vector<int,100> v;
	int i;
	for (i=0;i<100;i++)
		v.add(i);
	for (i=0;i<50;i++)
		v.remove(0);
	bool passed = true;
	for (i=50;i<100;i++)
		if (v[i-50] != i) passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
}

void testSortedVector() {
	printf("Test Firebird::SortedVector: ");
	SortedVector<int,100> v;
	int i;
	for (i=0;i<100;i++)
		v.add(99-i);
	for (i=0;i<50;i++)
		v.remove(0);
	bool passed = true;
	for (i=50;i<100;i++)
		if (v[i-50] != i) passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
}

#define TEST_ITEMS 1000000

struct Test {
	int value;
	int count;
	static const int& generate(void *sender, const Test& value) {
		return value.value;
	}
};

void testBePlusTree() {
    MallocAllocator temp;
    printf("Test Firebird::BePlusTree\n");
	
    printf("Fill array with test data (%d items)...", TEST_ITEMS);
	Vector<int, TEST_ITEMS> v;
	int n = 0;
	int i;
	for (i=0;i<TEST_ITEMS;i++) {
		n = n * 45578 - 17651;
		// Fill it with quasi-random values in range 0...TEST_ITEMS-1
		v.add(((i+n) % TEST_ITEMS + TEST_ITEMS)/2);
	}
	printf(" DONE\n");
	
	printf("Create two trees one with factor 2 and one with factor 13 and fill them with test data: ");
	BePlusTree<Test, int, MallocAllocator, Test, 
		DefaultComparator<int>, 2, 2> tree1(&temp);
	BePlusTree<Test, int, MallocAllocator, Test, 
		DefaultComparator<int>, 13, 13> tree2(&temp);
	int cnt1 = 0, cnt2 = 0;
	for (i=0; i < v.getCount(); i++) {
		if (tree1.locate(locEqual, v[i]))
			tree1.current().count++;
		else {
			Test t;
			t.value = v[i];
			t.count = 1;
			if (!tree1.add(t))
				fb_assert(false);
			cnt1++;
		}
		if (tree2.locate(locEqual, v[i]))
			tree2.current().count++;
		else {
			Test t;
			t.value = v[i];
			t.count = 1;
			if (!tree2.add(t))
				fb_assert(false);
			cnt2++;
		}	
	}
	printf(" DONE\n");
	
	bool passed = true;
	
	printf("Check that tree(2) contains test data: ");
	for (i=0; i< v.getCount(); i++) {
		if(!tree1.locate(locEqual,v[i])) 
			passed = false;
	}
	printf(passed?"PASSED\n":"FAILED\n");
	passed = true;
	
	printf("Check that tree(13) contains test data: ");
	for (i=0; i< v.getCount(); i++) {
		if(!tree2.locate(locEqual,v[i])) 
			passed = false;
	}
	printf(passed?"PASSED\n":"FAILED\n");
	
	passed = true;
	
	printf("Check that tree(2) contains data from the tree(13) and its count is correct: ");
	n = 0;
	if (tree1.getFirst()) do {
		n++;
		if (!tree2.locate(locEqual, tree1.current().value)) 
			passed = false;
	} while (tree1.getNext());
	if (n != cnt1 || cnt1 != cnt2) 
		passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
	
	printf("Check that tree(13) contains data from the tree(2) "\
		   "and its count is correct (check in reverse order): ");
	n = 0;
	if (tree2.getLast()) do {
		n++;
		if (!tree1.locate(locEqual, tree2.current().value)) passed = false;
	} while (tree2.getPrev());
	if (n != cnt2)
		passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
	
	printf("Remove half of data from the trees: ");
	while (v.getCount() > TEST_ITEMS/2) {
		if (!tree1.locate(locEqual, v[v.getCount()-1]))
			fb_assert(false);
		if (tree1.current().count > 1) 
			tree1.current().count--;
		else {
			tree1.fastRemove();
			cnt1--;
		}
		if (!tree2.locate(locEqual, v[v.getCount()-1]))
			fb_assert(false);
		if (tree2.current().count > 1) 
			tree2.current().count--;
		else {
			tree2.fastRemove();
			cnt2--;
		}
		v.shrink(v.getCount()-1);
	}
	printf(" DONE\n");
	
	printf("Check that tree(2) contains test data: ");
	for (i=0; i< v.getCount(); i++) {
		if(!tree1.locate(locEqual,v[i])) 
			passed = false;
	}	
	printf(passed?"PASSED\n":"FAILED\n");
	passed = true;
	
	printf("Check that tree(13) contains test data: ");
	for (i=0; i < v.getCount(); i++) {
		if(!tree2.locate(locEqual,v[i])) 
			passed = false;
	}	
	printf(passed?"PASSED\n":"FAILED\n");
	
	passed = true;
	
	printf("Check that tree(2) contains data from the tree(13) and its count is correct: ");
	n = 0;
	if (tree1.getFirst()) do {
		n++;
		if (!tree2.locate(locEqual, tree1.current().value)) passed = false;
	} while (tree1.getNext());
	if (n != cnt1 || cnt1 != cnt2) 
		passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
	
	passed = true;
	
	printf("Check that tree(13) contains data from the tree(2) "\
		   "and its count is correct (check in reverse order): ");
	n = 0;
	if (tree2.getLast()) do {
		n++;
		if (!tree1.locate(locEqual, tree2.current().value)) passed = false;
	} while (tree2.getPrev());
	if (n != cnt2) 
		passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
	
	passed = true;
	
	printf("Remove the rest of data from the trees: ");
	for (i=0;i < v.getCount(); i++) {
		if (!tree1.locate(locEqual, v[i]))
			fb_assert(false);
		if (tree1.current().count > 1) 
			tree1.current().count--;
		else {
			tree1.fastRemove();
			cnt1--;
		}
		if (!tree2.locate(locEqual, v[i]))
			fb_assert(false);
		if (tree2.current().count > 1)
			tree2.current().count--;
		else {
			tree2.fastRemove();
			cnt2--;
		}
	}
	printf(" DONE\n");

	printf("Check that both trees do not contain anything: ");
	if (tree1.getFirst())
		passed = false;
	if (tree2.getLast())
		passed = false;
	printf(passed?"PASSED\n":"FAILED\n");
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

void testAllocator() {
	printf("Test Firebird::MemoryPool\n");
	MemoryPool* pool = MemoryPool::createPool();
	
	MallocAllocator allocator;
	BePlusTree<AllocItem,AllocItem,MallocAllocator,DefaultKeyValue<AllocItem>,AllocItem> items(&allocator),
		bigItems(&allocator);
	printf("Allocate %d items: ", ALLOC_ITEMS);
	int n = 0;
	int i;
	pool->verify_pool();
	for (i=0;i<ALLOC_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % MAX_ITEM_SIZE + MAX_ITEM_SIZE)/2+1)};
		items.add(temp);
	}
	printf(" DONE\n");
	pool->verify_pool();
	
	printf("Deallocate half of items in quasi-random order: ");
	n = 0;
	if (items.getFirst()) do {
		pool->deallocate(items.current().item);
		n++;
	} while (n < ALLOC_ITEMS/2 && items.getNext());
	printf(" DONE\n");
	pool->verify_pool();
	
	printf("Allocate %d big items: ", BIG_ITEMS);
	n = 0;
	pool->verify_pool();
	for (i=0;i<BIG_ITEMS;i++) {
		n = n * 47163 - 57412;
		AllocItem temp = {n, pool->allocate((n % BIG_SIZE + BIG_SIZE)/2+1)};
		bigItems.add(temp);
	}
	printf(" DONE\n");
	pool->verify_pool();
	
	printf("Deallocate the rest of small items in quasi-random order: ");
	while (items.getNext()) {
		pool->deallocate(items.current().item);
	}
	printf(" DONE\n");
	pool->verify_pool();
	
	printf("Deallocate big items in quasi-random order: ");
	if (bigItems.getFirst()) do {
		pool->deallocate(bigItems.current().item);
	} while (bigItems.getNext());
	printf(" DONE\n");
	pool->verify_pool();
	pool->print_contents(stdout,true);
	MemoryPool::deletePool(pool);
//  TODO:
//	Test critically low memory conditions
}

int main() {
	testVector();
	testSortedVector();
	testBePlusTree();
	testAllocator();
}
