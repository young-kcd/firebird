/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		class_perf.cpp
 *	DESCRIPTION:	Class library performance measurements
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "tree.h"
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
	printf("Add+remove %d elements from array of scale %d took %d milliseconds. \n", 
		TEST_ITEMS,	scale, (int)(d-t)*1000/CLOCKS_PER_SEC);
}

int main() {
	using namespace Firebird;
	printf("Fill array with test data (%d items)...", TEST_ITEMS);
	Vector<int, TEST_ITEMS> *v = new Vector<int, TEST_ITEMS>();
	int n = 0;
	for (int i=0;i<TEST_ITEMS;i++) {
		n = n * 45578 - 17651;
		// Fill it with quasi-random values in range 0...TEST_ITEMS-1
		v->add(((i+n) % TEST_ITEMS + TEST_ITEMS)/2);
	}
	printf(" DONE\n");
	MallocAllocator temp;
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 10, 10> tree10(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree10.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree10.locate((*v)[i]))
			tree10.fastRemove();
	}
	report(10);

	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 50, 50> tree50(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree50.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree50.locate((*v)[i]))
			tree50.fastRemove();
	}
	report(50);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 75, 75> tree75(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree75.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree75.locate((*v)[i]))
			tree75.fastRemove();
	}
	report(75);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 100, 100> tree100(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree100.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree100.locate((*v)[i]))
			tree100.fastRemove();
	}
	report(100);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 200, 200> tree200(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree200.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree200.locate((*v)[i]))
			tree200.fastRemove();
	}
	report(200);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 250, 250> tree250(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree250.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree250.locate((*v)[i]))
			tree250.fastRemove();
	}
	report(250);
	
	start();
	BePlusTree<int, int, MallocAllocator, DefaultKeyValue<int>, 
		DefaultComparator<int>, 500, 500> tree500(NULL);
	for (int i=0; i<TEST_ITEMS;i++)
		tree500.add((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++) {
		if (tree500.locate((*v)[i]))
			tree500.fastRemove();
	}
	report(500);
	
	std::set<int> stlTree;
	start();
	for (int i=0; i<TEST_ITEMS;i++)
		stlTree.insert((*v)[i]);
	for (int i=0; i<TEST_ITEMS;i++)
		stlTree.erase((*v)[i]);
	clock_t d = clock();
	printf("Just a reference: add+remove %d elements from STL tree took %d milliseconds. \n", 
		TEST_ITEMS,	(int)(d-t)*1000/CLOCKS_PER_SEC);
}
