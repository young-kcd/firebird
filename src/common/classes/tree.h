/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		tree.h
 *	DESCRIPTION:	Generic In-memory B+ Tree
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
 *  $Id: tree.h,v 1.31 2004-06-13 23:49:28 skidder Exp $
 *
 */

#ifndef TREE_H
#define TREE_H

#include <exception>
#include "../jrd/gdsassert.h"
#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h> /* XPG: prototypes for malloc/free have to be in
					   stdlib.h (EKU) */
#endif
#include "vector.h"
#include <new>

namespace Firebird {

// This macro controls merging of nodes of all B+ trees
// Now it merges pages only when resulting page will be 3/4 filled or less
// Be careful while changing this expression. N=2 must always cause merge
static inline bool NEED_MERGE(int current_count, int page_count) {
	return current_count * 4 / 3 <= page_count;
}

// Note: small values will cause wasting of memory because overhead for
// each page is 28-32 bytes (on 32-bit platforms)
// 100 is an optimal value for range 10^5 - 10^7 items and it generates 
// total tree overhead of ~10%
const int LEAF_PAGE_SIZE = 100;
const int NODE_PAGE_SIZE = 100;

// This is maximum level of tree nesting. 10^9 elements for binary tree case
// should be more than enough. No checks are performed in code against overflow of this value
const int MAX_TREE_LEVEL = 30;

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

// Fast and simple B+ tree of simple types.
// Tree is always accessed via accessor classes. There is default accessor
// built into the class to simplify programming in single-threaded 
// non-reenterant access model.
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
	BePlusTree(Allocator *_pool) : pool(_pool), level(0), root(NULL), defaultAccessor(this)	{ };

    ~BePlusTree() {
		// We delete tree which was not fully created
		if (!root) return;
		
		// Find first items page
		void *temp = root;
		for (int i = level; i > 0; i--)
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
	
	// Remove item. Current position moves to next item after this call. 
	// If next item doesn't exist method returns false
    bool fastRemove() { return defaultAccessor.fastRemove(); }
	
	bool locate(Key& key) { return defaultAccessor.locate(locEqual, key); }
	
	bool locate(LocType lt, Key& key) {	return defaultAccessor.locate(lt, key); }
	
	bool getFirst() { return defaultAccessor.getFirst(); }
	
	bool getLast() { return defaultAccessor.getLast(); }
	
	bool getNext() { return defaultAccessor.getNext(); }
	
	bool getPrev() { return defaultAccessor.getPrev(); }
	
    Value& current() const { return defaultAccessor.current(); }
	
private:
	BePlusTree(Allocator *_pool, void *rootPage) : 	pool(_pool), level(0), 
		root(new(rootPage) ItemList()), defaultAccessor(this) {}
		
	class NodeList;
		
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
#ifndef _MSC_VER
		friend class BePlusTree::NodeList;
#endif
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
		static const Key& generate(const void *sender, void *item) { 
			for (int lev = ((NodeList *)sender)->level; lev > 0; lev--)
				item = *((NodeList *)item)->begin();

			// ItemList reference below is ISO C++ compliant.
			// If your compiler has broken name lookup sequence then conditionally
			// add ItemList typedef for you compiler with whichever syntax it likes
			return KeyOfValue::generate(item,*((ItemList *)item)->begin());
		}
		static void setNodeParentAndLevel(void *node, int level, NodeList *parent) {
			if (level) {
				((NodeList *)node)->parent = parent;
				((NodeList *)node)->level = level-1;
			}
			else
				((ItemList *)node)->parent = parent;
		}		
		static void setNodeParent(void *node, int level, NodeList *parent) {
			if (level)
				((NodeList *)node)->parent = parent;
			else
				((ItemList *)node)->parent = parent;
		}		
	}; 

public:
	class Accessor {
	public:
		Accessor(BePlusTree* _tree) : tree(_tree), curr(NULL), curPos(0) {}		
	
		// Remove item. Current position moves to next item after this call. 
		// If next item doesn't exist method returns false
		bool fastRemove() {
			if ( !tree->level ) {
				curr->remove(curPos);
				return curPos < curr->getCount();
			}
			if ( curr->getCount() == 1 ) {
				// Only one node left in the current page. We cannot remove it directly
				// because is would invalidate our tree structure
				fb_assert(curPos == 0);
				ItemList *temp;
				if ( (temp = curr->prev) && NEED_MERGE(temp->getCount(), LeafCount) ) {
					temp = curr->next;
					tree->_removePage(0, curr);
					curr = temp;
					return curr;
				}
				if ( (temp = curr->next) && NEED_MERGE(temp->getCount(), LeafCount) ) {
					tree->_removePage(0, curr);
					curr = temp;
					return true;
				}
				if ( (temp = curr->prev) ) {
					(*curr)[0] = (*temp)[temp->getCount() - 1];
					temp->shrink(temp->getCount() - 1);
					curr = curr->next;
					return curr;
				}
				if ( (temp = curr->next) ) {
					(*curr)[0] = (*temp)[0];
					temp->remove(0);
					return true;
				}
				// It means the tree is broken
				fb_assert(false);
				return false;
			}
			curr->remove(curPos);
			ItemList *temp;
			if ( (temp = curr->prev) && NEED_MERGE(temp->getCount() + curr->getCount(), LeafCount) ) {
				// After join upper levels of the tree remain stable because join doesn't change
				// key of the page. The same applies to lower case too.
				curPos += temp->getCount();
				temp->join(*curr);
				tree->_removePage(0, curr);
				curr = temp;
				// The code below will adjust current position if needed
			} else {
				if ( (temp = curr->next) && NEED_MERGE(temp->getCount() + curr->getCount(), LeafCount) ) {
					curr->join(*temp);
					tree->_removePage(0, temp);
					return true;
				}
			}
			if (curPos >= curr->getCount()) {
				fb_assert(curPos == curr->getCount());
				curPos = 0;
				curr = curr->next;
				return curr;
			}
			return true;
		}
	
		bool locate(Key& key) {
			return locate(locEqual, key);
		}
	
		// Position accessor on item having LocType relationship with given key
		// If method returns false position of accessor is not defined.
		bool locate(LocType lt, Key& key) {
			// Inlining is efficient here because LocType will be known in most cases
			// and compiler will be able to eliminate most of code
			void *list = tree->root;
			if (!list) return false; // Uninitalized tree
			for (int lev = tree->level; lev; lev--) {
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
					curPos = curr->getCount() - 1;
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
		// If method returns false it means list is empty and 
		// position of accessor is not defined.
		bool getFirst() {
			void *items = tree->root;
			if (!items) return false; // Uninitalized tree
			for (int i = tree->level; i > 0; i--)
				items = (*(NodeList *)items)[0];
			curr = (ItemList *)items;
			curPos = 0;
			return ((ItemList *)items)->getCount();
		}
		// If method returns false it means list is empty and 
		// position of accessor is not defined.
		bool getLast() {
			void *items = tree->root;
			if (!items) return false; // Uninitalized tree
			for (int i = tree->level; i > 0; i--)
				items = (*(NodeList *)items)[((NodeList *)items)->getCount() - 1];
			curr = (ItemList *)items;
			curPos = ((ItemList *)items)->getCount() - 1;
			return curPos >= 0;
		}
		// Accessor position must be establised via successful call to getFirst(), 
		// getLast() or locate() before you can call this method
		bool getNext() {
			curPos++;
			if (curPos >= curr->getCount()) {
				if (curr->next) {
					curr = curr->next;
					curPos = 0;
				} else {
					// If we reached end of the list just return false and do not invalidate position
					curPos--;
					return false;
				}
			}
			return true;
		}
		// Accessor position must be establised via successful call to getFirst(), 
		// getLast() or locate() before you can call this method
		bool getPrev() {
			curPos--;
			if (curPos < 0) {
				if (curr->prev) {
					curr = curr->prev;
					curPos = curr->getCount()-1;
				} else {
					// If we reached beginning of the list just return false and do not invalidate position
					curPos = 0;
					return false;
				}
			}
			return true;
		}
	    Value& current() const { return (*curr)[curPos]; }
	private:
		BePlusTree* tree;
		ItemList *curr;
  		int curPos;
	};

private:
	Allocator *pool;
	int level;
    void *root;
	Accessor defaultAccessor;

	void _removePage(int level, void *node);
	
	friend class MemoryPool;
	friend class NodeList;
	friend class Accessor;
};

/************************ BePlusTree implementation ******************/

template <typename Value, typename Key, typename Allocator, typename KeyOfValue, typename Cmp, int LeafCount, int NodeCount>
bool BePlusTree<Value, Key, Allocator, KeyOfValue, Cmp, LeafCount, NodeCount>::add(const Value& item)
{
	// Finish initialization of the tree if necessary
	if (!root) root = new (pool->allocate(sizeof(ItemList))) ItemList();

	// Find leaf page for our item
	void *vList = this->root;
	const Key& key = KeyOfValue::generate(NULL, item);
	for (int lev = this->level; lev > 0 ; lev--) {
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
			temp->insert(0, (*leaf)[LeafCount - 1]);
			leaf->shrink(LeafCount - 1);
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
			leaf->insert(pos - 1, item);
		}
		return true;
	}
			
	// Nearby pages are also full. We need to add one more leaf page to the list
	// This shouldn't happen very often. Traverse tree up trying to add node

	// No re-enterance allowed !!!
	// Since we haven't done anything with tree yet, thus we don't need to recover 
	// anything in case of error thrown at this allocation here
	ItemList *newLeaf = new(this->pool->allocate(sizeof(ItemList))) ItemList(leaf); 
			
	// Start building recovery map.
	// This array contains index of the element we try to add on page of each level
	// -1 means that element is on new page
	// In case of low memory condition we use this data to recover to innocent state
	int recovery_map[MAX_TREE_LEVEL];
			
	if (pos == LeafCount) {
		newLeaf->insert(0,item);
		recovery_map[0] = -1;
	}
	else {
		newLeaf->insert(0, (*leaf)[LeafCount - 1]);
		leaf->shrink(leaf->getCount() - 1);
		leaf->insert(pos, item);
		recovery_map[0] = pos;
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
					void *t = (*nodeList)[NodeCount - 1];
					NodeList::setNodeParent(t, curLevel, list);
					list->insert(0, t);
					nodeList->shrink(NodeCount - 1);
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
					nodeList->insert(pos - 1, newNode);
				}
				return true;
			}
				
			// No space found. Allocate NodeList page and climb up the tree
				
			// No re-enterance allowed !!!
			// Exceptions from this point
			// are cleaned up lower
			NodeList *newList = new(this->pool->allocate(sizeof(NodeList))) NodeList(nodeList); 
				
			if (pos == NodeCount) {
				NodeList::setNodeParentAndLevel(newNode, curLevel, newList);
				newList->insert(0, newNode);
				recovery_map[curLevel + 1] = -1;
			}
			else {
				void *t = (*nodeList)[NodeCount - 1];
				NodeList::setNodeParent(t, curLevel, newList);
				newList->insert(0, t);
				nodeList->shrink(NodeCount - 1);
				NodeList::setNodeParentAndLevel(newNode, curLevel, nodeList);
				nodeList->insert(pos, newNode);
				recovery_map[curLevel + 1] = pos;
			}
			newNode = newList;		
			nodeList = nodeList->parent;
			curLevel++;
		}
			
		// This is the worst case. We reached the top of tree but were not able to insert node
		// Allocate new root page and increase level of our tree
		nodeList = new(this->pool->allocate(sizeof(NodeList))) NodeList();
		nodeList->level = this->level;
		nodeList->insert(0, this->root);
		NodeList::setNodeParentAndLevel(newNode, this->level, nodeList);
		NodeList::setNodeParent(this->root, this->level, nodeList);
		nodeList->add(newNode);
		this->root = nodeList;
		this->level++;
	} catch(const std::exception&) {
		// Recover tree to innocent state
		while (curLevel) {
			NodeList *itemL = reinterpret_cast<NodeList*>(newNode);
			void *lower;
		    if (recovery_map[curLevel] < 0) {
				lower = (*itemL)[0];
			} else {
				lower = (*itemL->prev)[recovery_map[curLevel]];
				itemL->prev->remove(recovery_map[curLevel]);
				itemL->prev->insert(itemL->prev->getCount(), (*itemL)[0]);
				NodeList::setNodeParent((*itemL)[0], curLevel - 1, itemL->prev);
			}
			itemL->~NodeList();
			this->pool->deallocate(newNode);
			newNode = lower;
			curLevel--;
		}
		ItemList *itemL2 = reinterpret_cast<ItemList*>(newNode);
		if (recovery_map[0] >= 0) {
			itemL2->prev->remove(recovery_map[0]);
			itemL2->prev->insert(itemL2->prev->getCount(), (*itemL2)[0]);
		}
		itemL2->~ItemList();
		this->pool->deallocate(newNode);
		throw;
	}
	return true;
}

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
			_removePage(nodeLevel + 1, list);
		}
		else
		if ( (temp = list->next) && NEED_MERGE(temp->getCount(), NodeCount) ) {
			_removePage(nodeLevel + 1, list);
		}
		else
		if ( (temp = list->prev) ) {
			NodeList::setNodeParent( 
				((*list)[0] = (*temp)[temp->getCount() - 1]), nodeLevel, list);
			temp->shrink(temp->getCount() - 1);
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
			fb_assert(false);
		}
	}
	else
	{
		int pos;
#ifndef DEV_BUILD
		list->find(NodeList::generate(list, node), pos);
#else
		bool found = list->find(NodeList::generate(list, node), pos);
		fb_assert(found);
#endif
		list->remove(pos);
		
		if (list == root && list->getCount() == 1) {
			// We reached the top of the tree and were asked to modify root
			// page so only one node will be left in this case.
			// Reduce the level of the tree
			root = (*list)[0];
			level--;
			NodeList::setNodeParent(root, level, NULL);
			list->~NodeList();
			pool->deallocate(list);
		}
		else {		
			NodeList *temp;
			if ( (temp = list->prev) && 
				 NEED_MERGE(temp->getCount() + list->getCount(), NodeCount) ) 
			{
				// After join upper levels of the tree remain stable because join doesn't change
				// key of the page. The same applies to lower case too.
				temp->join(*list);
				for (int i = 0; i < list->getCount(); i++)
					NodeList::setNodeParent((*list)[i], nodeLevel, temp);
				_removePage(nodeLevel + 1, list);
			}
			else
			if ( (temp = list->next) && 
				 NEED_MERGE(temp->getCount() + list->getCount(), NodeCount) ) 
			{
				list->join(*temp);
				for (int i = 0; i < temp->getCount(); i++)
					NodeList::setNodeParent((*temp)[i], nodeLevel, list);
				_removePage(nodeLevel + 1, temp);
			}
		}
	}
	
	if (nodeLevel)
		((NodeList *)node)->~NodeList();
	else
		((ItemList *)node)->~ItemList();
	pool->deallocate(node);
}

}  // namespace Firebird

#endif
