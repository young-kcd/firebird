/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		stack.h
 *	DESCRIPTION:	Stack.
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef CLASSES_STACK_H
#define CLASSES_STACK_H

#include "../common/classes/vector.h"

namespace Firebird {

	template <typename Object, int Capacity = 16>
		class Stack : public AutoStorage
	{
	private:
		Stack<Object, Capacity>(Stack<Object, Capacity>&);	// not implemented

		class Entry : public Vector<Object, Capacity> {
		private:
			typedef Vector<Object, Capacity> inherited;
		public:
			Entry* next;

			Entry(Object e, Entry* stk) 
				: inherited(), next(stk) 
			{
				add(e);
			}
			
			Entry(Entry* stk) : inherited(), next(stk) { }

			~Entry() 
			{
				delete next;
			}
			
			Entry* push(Object e, MemoryPool& p)
			{
				if (inherited::getCount() < getCapacity())
				{
					add(e);
					return this;
				}
				Entry* newEntry = FB_NEW(p) Entry(e, this);
				return newEntry;
			}

			Object pop()
			{
				fb_assert(inherited::getCount() > 0);
				return data[--count];
			}

			Object getObject(int pos) const
			{
				return data[pos];
			}

			void split(int elem, Entry* target) 
			{
				fb_assert(elem > 0 && elem < count);
				memcpy(target->data, data, elem * sizeof(Object));
				target->count += elem;
				count -= elem;
				memmove(data, &data[elem], count * sizeof(Object));
			}

			Entry* dup(MemoryPool& p)
			{
				Entry* rc = FB_NEW(p) Entry(next ? next->dup(p) : 0);
				rc->join(*next);
				return rc;
			}

			bool hasMore(int value) const
			{
				fb_assert(value >= 0);
				
				if (value <= getCount())
					return true;

				for(const Entry * stk = this; stk && value > 0; stk = stk->next)
					value -= stk->getCount();

				return (value <= 0);
			}

		};

		Entry* stk;

	public:
		explicit Stack<Object, Capacity>(MemoryPool& p) 
			: AutoStorage(p), stk(0) { }

		Stack<Object, Capacity>() : AutoStorage(), stk(0) { }

		~Stack<Object, Capacity>()
		{
			delete stk;
		}

		void push(Object e)
		{
			stk = stk ? stk->push(e, getPool()) 
					  : FB_NEW(getPool()) Entry(e, 0);
		}

		Object pop()
		{
			fb_assert(stk);
			Object tmp = stk->pop();
			if (!stk->getCount()) {
				Entry *oldEntry = stk;
				stk = stk->next;
				oldEntry->next = 0;
				delete oldEntry;
			}
			return tmp;
		}

	private:
		// disable use of default operator=
		Stack<Object, Capacity>& operator= (Stack<Object, Capacity>& s);

	public:
		void takeOwnership (Stack<Object, Capacity>& s) {
			fb_assert(&getPool() == &s.getPool());
			delete stk;
			stk = s.stk;
			s.stk = 0;
		}

		class iterator {
		private:
			friend class ::Firebird::Stack<Object, Capacity>;
			const Entry* stk;
			int elem;

		public:
			explicit iterator(const Stack<Object, Capacity>& s) 
				: stk(s.stk), elem(stk ? stk->getCount() : 0) { }
			iterator(iterator& i) 
				: stk(i.stk), elem(i.elem) { }
			iterator() : stk(0), elem(0) { }

			iterator& operator++()
			{
				fb_assert(stk);
				if (--elem <= 0) {
					if ((stk = stk->next)) 
					{
						elem = stk->getCount();
					}
				}
				return *this;
			}

			bool hasMore(int value) const
			{
				fb_assert(value >= 0);

				if (elem)
				{
					if (value < elem)
						return true;
				
					value -= elem-1;
				}

				if(stk && stk->next)
					return stk->next->hasMore(value);
				else
					return false;
			}

			bool notEmpty() const
			{
				return stk;
			}

			bool isEmpty() const
			{
				return !stk;
			}

			Object object() const
			{
				fb_assert(stk);
				return stk->getObject(elem - 1);
			}

			bool operator== (const iterator &i) const
			{
				return (stk == i.stk) && (elem == i.elem);
			}

			bool operator!= (const iterator &i) const
			{
				return !(*this == i);
			}

			bool operator== (const Stack<Object, Capacity>& s) const
			{
				return (s == *this);
			}

			bool operator!= (const Stack<Object, Capacity>& s) const
			{
				return !(*this == s);
			}

			iterator& operator= (const iterator& i) {
				stk = i.stk;
				elem = i.elem;
				return *this;
			}

			iterator& operator= (const Stack<Object, Capacity>& s) {
				stk = s.stk;
				elem = stk ? stk->getCount() : 0;
				return *this;
			}
		};

		// Merge stack "s" to the end of current one.
		// Returns - iterator to split stacks again.
		const iterator merge(Stack<Object, Capacity>& s)
		{
			fb_assert(&getPool() == &s.getPool());
			iterator rc(s);
			Entry **e = &stk;
			while (*e) 
			{
				e = &((*e)->next);
			}
			*e = s.stk;
			s.stk = 0;
			return rc;
		}

		// split stacks at mark
		void split (const iterator &mark, Stack<Object, Capacity>& s)
		{
			fb_assert(&getPool() == &s.getPool());
			fb_assert(!s.stk);

			// if empty stack was merged, there is nothing to do
			if (!mark.stk)
			{
				return;
			}

			// find entry to split
			Entry **toSplit = &stk;
			while (*toSplit != mark.stk)
			{
				fb_assert(*toSplit);
				toSplit = &((*toSplit)->next);
			}

			// Determine whether some new elements were added
			// to this stack. Depended on this we must
			// split on entries boundary or cut one entry to halfs.
			fb_assert((*toSplit)->getCount() >= mark.elem);
			if ((*toSplit)->getCount() == mark.elem)
			{
				s.stk = *toSplit;
				*toSplit = 0;
			}
			else {
				Entry* newEntry = FB_NEW(getPool()) Entry(0);
				(*toSplit)->split(mark.elem, newEntry);
				s.stk = newEntry;
				(*toSplit)->next = 0;
			}
		}

		// clear stacks until mark
		void clear (const iterator &mark)
		{
			// for empty mark just clear all stack
			if (!mark.stk)
			{
				clear();
				return;
			}

			// find entry to clear
			while (stk != mark.stk)
			{
				if (!stk)
				{
					return;
				}
				Entry *tmp = stk->next;
				stk->next = 0;
				delete stk;
				stk = tmp;
			}

			// remove extra elements from Entry
			fb_assert(stk->getCount() >= mark.elem);
			if (mark.elem == 0)
			{
				Entry *tmp = stk->next;
				stk->next = 0;
				delete stk;
				stk = tmp;
			}
			else {
				stk->shrink(mark.elem);
			}
		}

	private:
		friend class ::Firebird::Stack<Object,Capacity>::iterator;

	public:
		int getCount() const {
			int rc = 0;
			for (Entry* entry = stk; entry; entry = entry->next)
			{
				rc += entry->getCount();
			}
			return rc;
		}

		bool hasMore(int value) const
		{
			fb_assert(value >= 0);
			return (stk && stk->hasMore(value));
		}

		// returns topmost element on the stack
		Object object() const
		{
			fb_assert(stk);
			return stk->getObject(stk->getCount() - 1);
		}

		// returns true if stack is not empty
		bool notEmpty() const
		{
			return stk;
		}

		// returns true if stack is empty
		bool isEmpty() const
		{
			return !stk;
		}

		bool operator== (const iterator &i) const
		{
			return (i.stk == stk) && 
				   (stk ? i.elem == stk->getCount() : true);
		}

		bool operator!= (const iterator &i) const
		{
			return !(*this == i);
		}

		void assign(Stack<Object,Capacity>& v) {
			delete stk;
			stk = v.stk ? v.stk->dup(getPool()) : 0;
		}

		void clear()
		{
			delete stk;
			stk = 0;
		}

		MemoryPool& getPool() 
		{
			return AutoStorage::getPool();
		}
	};
}

#endif // CLASSES_STACK_H
