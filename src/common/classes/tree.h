/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		tree.h
 *	DESCRIPTION:	Generic In-memory B+ Tree
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

#ifndef TREE_H
#define TREE_H

#include <assert.h>
#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h> /* XPG: prototypes for malloc/free have to be in
					   stdlib.h (EKU) */
#endif
#include "vector.h"
// FIXME: Temporary until we switch out of using STL
#include <new>

// This macro controls merging of nodes of all B+ trees
// Now it merges pages only when resulting page will be 3/4 filled or less
// Be careful while changing this expression. N=2 must always cause merge
#define NEED_MERGE(N,COUNT) ((N)*4/3<=(COUNT))

// Note: small values will cause wasting of memory because overhead for
// each page is 28-32 bytes (on 32-bit platforms)
// 100 is an optimal value for range 10^5 - 10^7 items and it generates 
// total tree overhead of ~10%
#define LEAF_PAGE_SIZE 100
#define NODE_PAGE_SIZE 100

/* void* operator new (size_t size, void *place) {
	return place;
}*/

namespace Firebird {

class MallocAllocator {
public:
	void *allocate(size_t size) {
		return malloc(size);
	}
	void deallocate(void *p) {
		free(p);
	}
};

enum LocType { locEqual, locLess, locGreat, locGreatEqual, locLessEqual };

// Fast and simple B+ tree of simple types
// Tree has state (current item) and no iterator classes. 
// This is by design to make it simplier and faster :)))
//
// Notes: 
//
// 1) Items in the tree MUST be unique (this is performance optimization),
// you can always convert set of non-unique items to a set of unique items with count
// like this:
//  struct TreeItem {
//      Value value;
//      int count;
//      static const Key& generate(void *sender, const TreeItem& item) {
//          return KeyOfValue::generate(sender, value);
//      }
//  }
// Small and simple (just a few lines) template derived from BePlusTree can be created
// for this when real need arises. It will still be much faster than allowing duplicates
// in BePlusTree itself
//
// 2) We could store ultimate item count for each node and make tree accessable like 
// an indexed dynamic array without increase of algorithm calculation costs (this is one 
// more classical B+ tree feature). This is also not done to improve tree performance a little
//
template <typename Value, typename Key = Value, typename Allocator = MallocAllocator, 
	typename KeyOfValue = DefaultKeyValue<Value>, 
	typename Cmp = DefaultComparator<Key>, 
	int LeafCount = LEAF_PAGE_SIZE, 
	int NodeCount = NODE_PAGE_SIZE >
class BePlusTree {
public:
	BePlusTree(Allocator *_pool) : pool(_pool), level(0), curPos(0) 
	{
		curr = new (_pool->allocate(sizeof(ItemList))) ItemList();
		root = curr;
	};
    ~BePlusTree() {
		// Find first items page
		void *temp = root;
		for (int i=level;i>0;i--)
			temp = (*(NodeList *)temp)[0];
		ItemList *items = (ItemList *)temp;
		
		// Delete all items pages
		NodeList *lists = items->parent;
		while ( items ) {
			ItemList *t = items->next;
			items->~ItemList();
			pool->deallocate(items);
			items = t;
		}
		
		// Delete all upper layers of tree
		while (lists) {
			NodeList *list = lists;
			lists = lists->parent;
			while ( list ) {
				NodeList *t = list->next;
				list->~NodeList();
				pool->deallocate(list);
				list = t;
			}
		}
	}

	bool add(const Value& item);
	
	
	/* Remove item as quickly as possible. Current position is undefined after this call */
    void fastRemove();
	
	bool locate(Key& key) {
		return locate(locEqual, key);
	}
	
	// Inlining is efficient here because LocType will be known in most cases
	// and compiler will be able to eliminate most of code
	bool locate(LocType lt, Key& key) {
		void *list = root;
		for (int lev=level; lev; lev--) {
			int pos;
			if (!((NodeList *)list)->find(key, pos))
				if ( --pos < 0 ) pos = 0;
			list = (*(NodeList *)list)[pos];
		}

		curr = (ItemList *)list;
		bool found = curr->find(key, curPos);
		switch (lt) {
		case locEqual: return found;
		case locGreatEqual:
			if (curPos == curr->getCount()) {
				curr = curr->next;
				curPos = 0;
			}
			return found || curr;
		case locLessEqual:
			if (found) return true;
		case locLess:
			curPos--;
			if (curPos < 0) {
				curr = curr->prev;
				if (!curr) return false;
				curPos = curr->getCount()-1;
			}
			return true;
		case locGreat:
			if (found) curPos++;
			if (curPos == curr->getCount()) {
				curr = curr->next;
				curPos = 0;
			}
			return curr != 0;
		}
		return false;
	}
	bool getFirst() {
		void *items = root;
		for (int i=level;i>0;i--)
			items = (*(NodeList *)items)[0];
		curr = (ItemList *)items;
		curPos = 0;
		return ((ItemList *)items)->getCount();
	}
	bool getLast() {
		void *items = root;
		for (int i=level;i>0;i--)
			items = (*(NodeList *)items)[((NodeList *)items)->getCount()-1];
		curr = (ItemList *)items;
		curPos = ((ItemList *)items)->getCount()-1;
		return curPos >= 0;
	}
	bool getNext() {
		curPos++;
		if (curPos >= curr->getCount()) {
			curr = curr->next;
			curPos = 0;
		}
		return curr != 0;
	}
	bool getPrev() {
		curPos--;
		if (curPos < 0) {
			curr = curr->prev;
			if (!curr) return false;
			curPos = curr->getCount()-1;
		}
		return true;
	}
    Value& current() const { return (*curr)[curPos]; }
	Value& getAddErrorValue() { return addErrorValue; }
    int getCount() const { return count; }
private:
	BePlusTree(Allocator *_pool, void *rootPage) : 	pool(_pool), level(0), 
		curr(new(rootPage) ItemList()), root(rootPage),	curPos(0)/*, count(0)*/  {};
		
	class NodeList ;
		
    class ItemList : public SortedVector<Value,LeafCount,Key,KeyOfValue,Cmp> {
	public:
		NodeList *parent;
		ItemList *next, *prev;
		// Adds newly created item to doubly-linked list
		ItemList(ItemList *items) : parent(NULL) { 
			if ( (next = items->next) )
				next->prev = this;
			prev = items;
			items->next = this;
		}
		// Create first item in the linked list
		ItemList() : parent(NULL), next(NULL), prev(NULL) {};
                friend class BePlusTree;
                friend class BePlusTree::NodeList;
	};
	
    class NodeList : public SortedVector<void*,NodeCount,Key,NodeList,Cmp> {
	public:
		// Adds newly created item to the doubly-linked list
		NodeList(NodeList *items) : parent(NULL) { 
			if ( (next = items->next) ) 
				next->prev = this;
			prev = items; 
			items->next = this;
		}
		// Create first item in the linked list
		NodeList() : parent(NULL), next(NULL), prev(NULL) {}
		
		int level;
		NodeList *parent;
		NodeList *next, *prev;
		static const Key& generate(void *sender, void *item) { 
			for (int lev = ((NodeList *)sender)->level; lev > 0; lev--)
				item = *((NodeList *)item)->begin();
			return KeyOfValue::generate(item,*((BePlusTree::ItemList *)item)->begin());
		}
		static void setNodeParentAndLevel(void *node, int level, NodeList *parent) {
			if (level) {
				((NodeList *)node)->parent = parent;
				((NodeList *)node)->level = level-1;
			} else
				((ItemList *)node)->parent = parent;
		}		
		static void setNodeParent(void *node, int level, NodeList *parent) {
			if (level)
				((NodeList *)node)->parent = parent;
			else
				((ItemList *)node)->parent = parent;
		}		
	}; 
	
	Allocator *pool;
	int level;
	ItemList *curr;
    void *root;
    int curPos;
	Value addErrorValue;

	void _removePage(int level, void *node);
	
	friend class MemoryPool;
        friend class NodeList;
};

/************************ BePlusTree implementation ******************/

template <typename Value, typename Key, typename Allocator, typename KeyOfValue, typename Cmp, int LeafCount, int NodeCount>
void BePlusTree<Value, Key, Allocator, KeyOfValue, Cmp, LeafCount, NodeCount>::_removePage(int nodeLevel, void *node)
{
	NodeList *list;
	// Get parent and adjust the links
	if (nodeLevel) {
		NodeList *temp = (NodeList *)node;
		if (temp->prev)
			temp->prev->next = temp->next;
		if (temp->next)
			temp->next->prev = temp->prev;
		list = temp->parent;
	}
	else {
		ItemList *temp = (ItemList *)node;
		if (temp->prev)
			temp->prev->next = temp->next;
		if (temp->next)
			temp->next->prev = temp->prev;
		list = temp->parent;
	}
	
	if ( list->getCount() == 1 ) {
		// Only one node left in the list. We cannot remove it directly
		// because is would invalidate our tree structure
		NodeList *temp;
		if ( (temp = list->prev) && NEED_MERGE(temp->getCount(), NodeCount) ) {
			_removePage(nodeLevel+1, list);
		}
		else
		if ( (temp = list->next) && NEED_MERGE(temp->getCount(), NodeCount) ) {
			_removePage(nodeLevel+1, list);
		}
		else
		if ( (temp = list->prev) ) {
			NodeList::setNodeParent( 
				((*list)[0] = (*temp)[temp->getCount()-1]), nodeLevel, list);
			temp->shrink(temp->getCount()-1);
		}
		else
		if ( (temp = list->next) ) {
			NodeList::setNodeParent( 
				((*list)[0] = (*temp)[0]), nodeLevel, list);
			temp->remove(0);
		}
		else
		{
			// It means the tree is broken
			assert(false);
		}
	}
	else
	{
		int pos;
#ifdef NDEBUG
		list->find(NodeList::generate(list,node),pos);
#else
		bool found = list->find(NodeList::generate(list,node),pos);
		assert(found);
#endif
		list->remove(pos);
		
		if (list == root && list->getCount()==1) {
			// We reached the top of the tree and were asked to modify root
			// page so only one node will be left in this case.
			// Reduce the level of the tree
			root = (*list)[0];
			level--;
			NodeList::setNodeParent(root, level, NULL);
			list->~NodeList();
			pool->deallocate(list);
		} else {		
			NodeList *temp;
			if ( (temp = list->prev) && 
				 NEED_MERGE(temp->getCount()+list->getCount(), NodeCount) ) 
			{
				// After join upper levels of the tree remain stable because join doesn't change
				// key of the page. The same applies to lower case too.
				temp->join(*list);
				for (int i=0; i < list->getCount(); i++)
					NodeList::setNodeParent((*list)[i], nodeLevel, temp);
				_removePage(nodeLevel+1, list);
			}
			else
			if ( (temp = list->next) && 
				 NEED_MERGE(temp->getCount()+list->getCount(), NodeCount) ) 
			{
				list->join(*temp);
				for (int i=0; i < temp->getCount(); i++)
					NodeList::setNodeParent((*temp)[i], nodeLevel, list);
				_removePage(nodeLevel+1, temp);
			}
		}
	}
	
	if (nodeLevel)
		((NodeList *)node)->~NodeList();
	else
		((ItemList *)node)->~ItemList();
	pool->deallocate(node);
}

template <typename Value, typename Key, typename Allocator, typename KeyOfValue, typename Cmp, int LeafCount, int NodeCount>
void BePlusTree<Value, Key, Allocator, KeyOfValue, Cmp, LeafCount, NodeCount>::fastRemove() 
{
	if ( !level ) {
		curr->remove(curPos);
		return;
	}
	if ( curr->getCount() == 1 ) {
		// Only one node left in the current page. We cannot remove it directly
		// because is would invalidate our tree structure
		assert(curPos == 0);
		ItemList *temp;
		if ( (temp = curr->prev) && NEED_MERGE(temp->getCount(), LeafCount) ) {
			_removePage(0, curr);
		}
		else
		if ( (temp = curr->next) && NEED_MERGE(temp->getCount(), LeafCount) ) {
			_removePage(0, curr);
		}
		else
		if ( (temp = curr->prev) ) {
			(*curr)[0] = (*temp)[temp->getCount()-1];
			temp->shrink(temp->getCount()-1);
		}
		else
		if ( (temp = curr->next) ) {
			(*curr)[0] = (*temp)[0];
			temp->remove(0);
		}
		else
		{
			// It means the tree is broken
			assert(false);
		}
	}
	else {
		curr->remove(curPos);
		ItemList *temp;
		if ( (temp = curr->prev) && NEED_MERGE(temp->getCount()+curr->getCount(), LeafCount) ) {
			// After join upper levels of the tree remain stable because join doesn't change
			// key of the page. The same applies to lower case too.
			temp->join(*curr);
			_removePage(0, curr);
		}
		else
		if ( (temp = curr->next) && NEED_MERGE(temp->getCount()+curr->getCount(), LeafCount) ) {
			curr->join(*temp);
			_removePage(0, temp);
		}
	}
}
	
template <typename Value, typename Key, typename Allocator, typename KeyOfValue, typename Cmp, int LeafCount, int NodeCount>
bool BePlusTree<Value, Key, Allocator, KeyOfValue, Cmp, LeafCount, NodeCount>::add(const Value& item) 
{
	// Find leaf page for our item
	void *vList = root;
	const Key& key = KeyOfValue::generate(NULL, item);
	for (int lev=level; lev > 0 ; lev--) {
		int pos;
		if (!((NodeList *)vList)->find(key, pos))
			if ( --pos < 0 ) pos = 0;
		vList = (*(NodeList *)vList)[pos];
	}

	ItemList *leaf = (ItemList *)vList;
	
	int pos;
	if (leaf->find(key, pos)) return false;
	
	if (leaf->getCount() < LeafCount) {
		leaf->insert(pos, item);
		return true;
	}
	
	// Page is full. Look up nearby pages for space if possible
	ItemList *temp;
	// Adding items to the next page is cheaper in most cases that
	// is why it is checked first
	if ((temp = leaf->next) && temp->getCount() < LeafCount) {
		// Found space on the next page
		if (pos == LeafCount) {
			// This would be ok if items were unique: temp->insert(0, item);
			// The same applies to all simular cases below
			temp->insert(0,item);
		}
		else {
			// Maybe splitting array by half would make things faster ?
			// It should do it in case of random size items.
			// It would make things slower in case of sequental items addition.
			// Let's leave it as is now.
			temp->insert(0, (*leaf)[LeafCount-1]);
			leaf->shrink(LeafCount-1);
			leaf->insert(pos, item);
		}
		return true;
	}
	
	if ((temp = leaf->prev) && temp->getCount() < LeafCount) {
		// Found space on the previous page
		if (pos == 0) {
			temp->insert(temp->getCount(), item);
		}
		else {
			temp->insert(temp->getCount(), (*leaf)[0]);
			leaf->remove(0);
			leaf->insert(pos-1, item);
		}
		return true;
	}
	
	// Nearby pages are also full. We need to add one more leaf page to the list
	// This shouldn't happen very often. Traverse tree up trying to add node
	ItemList *newLeaf;
	try {	
		newLeaf = new(pool->allocate(sizeof(ItemList))) ItemList(leaf); // No re-enterance allowed !!!
																	    // Exception here doesn't 
																		// invalidate tree structure
	} catch(const std::exception&) {
		addErrorValue = item;
		throw;
	}
	
	if (pos == LeafCount) {
		newLeaf->insert(0,item);
	}
	else {
		newLeaf->insert(0, (*leaf)[LeafCount-1]);
		leaf->shrink(leaf->getCount()-1);
		leaf->insert(pos, item);
	}
	
	void *newNode = newLeaf;
	NodeList *nodeList = leaf->parent;
	int curLevel = 0;
	try {
		while (nodeList) {
			// Easy case. We've got some space on the node page
			if (nodeList->getCount() < NodeCount) {
				NodeList::setNodeParentAndLevel(newNode, curLevel, nodeList);
				nodeList->add(newNode);
				return true;
			}
	
			// Page is full. Look up nearby pages for space if possible
			nodeList->find(NodeList::generate(nodeList, newNode), pos);
			NodeList *list;

			if ((list = nodeList->next) && list->getCount() < NodeCount) {
				// Found space on the next page
				if (pos == NodeCount) {
					NodeList::setNodeParentAndLevel(newNode, curLevel, list);
					list->insert(0, newNode);
				}
				else {
					void *t = (*nodeList)[NodeCount-1];
					NodeList::setNodeParent(t, curLevel, list);
					list->insert(0, t);
					nodeList->shrink(NodeCount-1);
					NodeList::setNodeParentAndLevel(newNode, curLevel, nodeList);
					nodeList->insert(pos, newNode);
				}
				return true;
			}
	
			if ((list = nodeList->prev) && list->getCount() < NodeCount) {
				// Found space on the previous page
				if (pos == 0) {
					NodeList::setNodeParentAndLevel(newNode, curLevel, list);
					list->insert(list->getCount(), newNode);
				}
				else {
					void *t = (*nodeList)[0];
					NodeList::setNodeParent(t, curLevel, list);
					list->insert(list->getCount(), t);
					nodeList->remove(0);
					NodeList::setNodeParentAndLevel(newNode, curLevel, nodeList);
					nodeList->insert(pos-1, newNode);
				}
				return true;
			}
		
			// No space found. Allocate NodeList page and climb up the tree
		
			NodeList *newList = new(pool->allocate(sizeof(NodeList))) NodeList(nodeList); // No re-enterance allowed !!!
																						  // Exceptions from this point
																					  	  // are cleaned up lower
		
			if (pos == NodeCount) {
				NodeList::setNodeParentAndLevel(newNode, curLevel, newList);
				newList->insert(0, newNode);
			}
			else {
				void *t = (*nodeList)[NodeCount-1];
				NodeList::setNodeParent(t, curLevel, newList);
				newList->insert(0, t);
				nodeList->shrink(NodeCount-1);
				NodeList::setNodeParentAndLevel(newNode, curLevel, nodeList);
				nodeList->insert(pos, newNode);
			}
			newNode = newList;		
			nodeList = nodeList->parent;
			curLevel++;
		}
	
		// This is a worst case. We reached the top of tree but were not able to insert node
		// Allocate new root page and increase level of our tree
		nodeList = new(pool->allocate(sizeof(NodeList))) NodeList();
		nodeList->level = level;
		nodeList->insert(0,root);
		NodeList::setNodeParentAndLevel(newNode, level, nodeList);
		NodeList::setNodeParent(root, level, nodeList);
		nodeList->add(newNode);
		root = nodeList;
		level++;
	} catch(const std::exception&) {
		while (curLevel) {
			void *lower = (*(NodeList *)newNode)[0];
			((NodeList *)newNode)->~NodeList();
			pool->deallocate(newNode);
			newNode = lower;
			curLevel--;
		}
		addErrorValue = (*(ItemList*)newNode)[0];
		((ItemList *)newNode)->~ItemList();
		pool->deallocate(newNode);
		throw;
	}
	return true;
}

}; /* namespace Firebird */

#endif
