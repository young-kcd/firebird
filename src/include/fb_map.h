/*
 *	PROGRAM:		Utility classes
 *	MODULE:			fb_map.h
 *	DESCRIPTION:	Wrapper around STL's map (deprecated)
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

#ifndef INCLUDE_FB_MAP_H
#define INCLUDE_FB_MAP_H
 
#include "../common/classes/alloc.h"

#include <map>

namespace Firebird
{
	template <class A, class B>
	class map :
		public std::map<A, B, std::less<A>, Firebird::allocator<std::pair<A, B> > >
	{
	};
} // namespace

#endif	// INCLUDE_FB_MAP_H
