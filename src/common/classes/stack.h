/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		stack.h
 *	DESCRIPTION:	Stack.
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
 * Stack is implemented as a linked list of vectors.
 * This makes implementation of merge and split operations
 * enough efficcient for large stacks (used in opt.cpp).
 *
 * Created by: Alex Peshkov <peshkoff@mail.ru>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
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

		Stack<Object, Capacity>& operator= (Stack<Object, Capacity>& s) {
			fb_assert(&getPool() == &s.getPool());
			delete stk;
			stk = s.stk;
			s.stk = 0;
			return *this;
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

			operator bool() const
			{
				return stk;
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

			bool operator== (const Stack<Object, Capacity>& s) const
			{
				return (stk == s.stk) && (s.stk) && 
					   (elem == s.stk->getCount());
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

		// returns topmost element on the stack
		Object object() const
		{
			fb_assert(stk);
			return stk->getObject(stk->getCount() - 1);
		}

		// returns true if stack is not empty
		operator bool() const
		{
			return stk;
		}

		bool operator== (iterator &i) const
		{
			return (i.stk == stk) && (stk) && 
				   (i.elem == stk->getCount());
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
