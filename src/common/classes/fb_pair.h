/*
 *	PROGRAM:	Common class definition
 *	MODULE:		fb_pair.h
 *	DESCRIPTION:	Provides almost that same functionality,
 *			that STL::pair does, but behaves 
 *			MemoryPools friendly.
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
 */

#ifndef CLASSES_FB_PAIR_H
#define CLASSES_FB_PAIR_H
 
#include "../common/classes/alloc.h"

namespace Firebird
{

// Left pair - left object in such pair has MemoryPool'ed constructor,
//	right one - doesn't (typically POD or builtin type)

template<typename parLeft, typename parRight> 
	struct Left {
		typedef parLeft first_type;
		typedef parRight second_type;
		Left(MemoryPool& p)	: first(p), second() { }
		Left(MemoryPool& p, const parLeft& v1, const parRight& v2) 
			: first(p, v1), second(v2) { }
		Left(MemoryPool& p, const Left& lp) 
			: first(p, lp.first), second(lp.second) { }
		parLeft first;
		parRight second;
	};

// Full pair - both objects in such pair have MemoryPool'ed constructors.

template<typename parLeft, typename parRight> 
	struct Full {
		typedef parLeft first_type;
		typedef parRight second_type;
		Full(MemoryPool& p)	: first(p), second(p) { }
		Full(MemoryPool& p, const parLeft& v1, const parRight& v2) 
			: first(p, v1), second(p, v2) { }
		Full(MemoryPool& p, const Full& lp) 
			: first(p, lp.first), second(p, lp.second) { }
		parLeft first;
		parRight second;
	};

// Pair - template providing full bool op-s set

template<typename BasePair> 
	struct Pair : public BasePair {
		typedef typename Pair::first_type Pair_first_type;
		typedef typename Pair::second_type Pair_second_type;
		Pair(MemoryPool& p) : BasePair(p) { }
		Pair(MemoryPool& p, const Pair_first_type& v1, 
			const Pair_second_type& v2) : BasePair(p, v1, v2) { }
		Pair(MemoryPool& p, const Pair& lp) 
			: BasePair(p, lp) { }
		Pair() : BasePair(AutoStorage::getAutoMemoryPool()) { }
		Pair(const Pair_first_type& v1, const Pair_second_type& v2) 
			: BasePair(AutoStorage::getAutoMemoryPool(), v1, v2) { }
		Pair(const Pair& lp) 
			: BasePair(AutoStorage::getAutoMemoryPool(), lp) { }
		bool operator==(const Pair& v)
		{
			return first == v.first && second == v.second; 
		}
		bool operator<(const Pair& v)
		{
			return first < v.first || (!(first < v.first) && second < v.second); 
		}
		bool operator!=(const Pair& v)
		{
			return ! (*this == v); 
		}
		bool operator>(const Pair& v)
		{
			return v < *this; 
		}
		bool operator<=(const Pair& v)
		{
			return ! (v < *this); 
		}
		bool operator>=(const Pair& v)
		{
			return ! (*this < v); 
		}
	};

template <typename P>
	class FirstKey 
	{
	public:
		typedef typename P::first_type Pair_first_type;
		static const Pair_first_type& 
			generate(void* sender, const P& Item) 
		{ 
			return Item.first; 
		}
	};

template <typename P>
	class FirstObjectKey 
	{
	public:
		typedef typename P::first_type Pair_first_type;
		static const Pair_first_type& 
			generate(void* sender, const P* Item) 
		{ 
			return Item->first; 
		}
	};
};

#endif	// CLASSES_FB_PAIR_H
