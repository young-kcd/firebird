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

#ifndef FB_PAIR_H
#define FB_PAIR_H
 
#include "../common/classes/alloc.h"

namespace Firebird
{

// Left pair - left object in such pair has MemoryPool'ed constructor,
//	right one - doesn't (typically POD or builtin type)

template<class parLeft, class parRight> 
	struct LeftPair {
		typedef parLeft first_type;
		typedef parRight second_type;
		LeftPair(MemoryPool *p) : first(p), second() {}
		LeftPair(MemoryPool *p, const parLeft& v1, const parRight& v2) 
			: first(p, v1), second(v2) {}
		LeftPair(MemoryPool *p, const LeftPair& lp) 
			: first(p, lp.first), second(lp.second) {}
		parLeft first;
		parRight second;
	};

template<class parLeft, class parRight>
	inline bool operator==(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (v1.first == v2.first && v1.second == v2.second); 
	}

template<class parLeft, class parRight>
	inline bool operator!=(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (!(v1 == v2)); 
	}

template<class parLeft, class parRight>
	inline bool operator<(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (v1.first < v2.first ||
			!(v2.first < v1.first) && v1.second < v2.second); 
	}

template<class parLeft, class parRight>
	inline bool operator>(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (v2 < v1); 
	}

template<class parLeft, class parRight>
	inline bool operator<=(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (!(v2 < v1)); 
	}

template<class parLeft, class parRight>
	inline bool operator>=(const LeftPair<parLeft, parRight>& v1,
		const LeftPair<parLeft, parRight>& v2)
	{
		return (!(v1 < v2)); 
	}

template <typename Pair>
	class FirstKey 
	{
	public:
		typedef typename Pair::first_type Pair_first_type;
		static const Pair_first_type& 
			generate(void* sender, const Pair& Item) 
		{ 
			return Item.first; 
		}
	};
};

#endif	// FB_PAIR_H
