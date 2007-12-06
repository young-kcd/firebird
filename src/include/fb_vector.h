/*
 *  fb_vector.h
 *  firebird_test
 *
 *  Created by john on Fri Dec 14 2001.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FB_VECTOR_H
#define FB_VECTOR_H
 
#include "../include/fb_types.h"
#include "../common/classes/alloc.h"

#include <vector>

namespace Firebird
{
	template<class T>
	class vector : public std::vector<T, Firebird::allocator<T> >
	{
	public:
		vector(int len) : std::vector<T, Firebird::allocator<T> >(len) {}
		vector(int len, MemoryPool &p, SSHORT type = 0)
			: std::vector<T, Firebird::allocator<T> >(len, T(),
				Firebird::allocator<T>(p, type)) {}
		vector(MemoryPool &p, SSHORT type = 0)
			: std::vector<T, Firebird::allocator<T> >(
					Firebird::allocator<T>(p, type) ) {}
	};
};

#endif	// FB_VECTOR_H
