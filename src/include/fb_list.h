#ifndef FB_LIST_H
#define FB_LIST_H
 
#include "../common/memory/allocators.h"

#include <list>
#include <slist>

namespace Firebird
{
	template <class T>
	class list : public std::list<T, Firebird::allocator<T> >
	{
	};

	template <class T>
	class slist : public std::slist<T, Firebird::allocator<T> >
	{
	};
};

#endif	// FB_LIST_H
