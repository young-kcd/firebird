/*
 *	PROGRAM:		Utility classes
 *	MODULE:			fb_vector.h
 *	DESCRIPTION:	Wrapper around STL's vector (deprecated)
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
 *  The Original Code was created by John Bellardo
 *  for the Firebird Open Source RDBMS project on Fri Dec 14 2001.
 *
 *  Copyright (c) 2001 John Bellardo <bellardo at cs.ucsd.edu>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#ifndef INCLUDE_FB_VECTOR_H
#define INCLUDE_FB_VECTOR_H
 
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
		vector(int len, MemoryPool& p, SSHORT type = 0)
			: std::vector<T, Firebird::allocator<T> >(len, T(),
				Firebird::allocator<T>(p, type)) {}
		vector(MemoryPool& p, SSHORT type = 0)
			: std::vector<T, Firebird::allocator<T> >(
					Firebird::allocator<T>(p, type) ) {}
	};
}  // namespace Firebird

#endif	// INCLUDE_FB_VECTOR_H

