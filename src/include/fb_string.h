/*
 *  fb_string.h
 *  firebird_test
 *
 *  Created by john on Fri Dec 14 2001.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FB_STRING_H
#define FB_STRING_H
 
#include "../include/fb_types.h"
#include "../common/memory/allocators.h"

#include <string>

namespace Firebird
{
	typedef std::basic_string<char, std::char_traits<char>,
				Firebird::allocator<char> > string;


};


#endif	// FB_STRING_H
